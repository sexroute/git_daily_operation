#include "stdafx.h"
#include "ChannAlarm.h"
#include "AlarmPlantInfo.h"
#include "additions/DebugHelper.h"
#include "MiddleWare.h"

//------------------------------------//
CVibChannParamAlarm::CVibChannParamAlarm()
{
	CVibChannParam();
	_bAlarmStatusS=false;
    _bFirstAlarmS=false;
	_bLearnTh=false;
	_bAlarmLearnTh=false;
	_iCurrCharValueIndex=0;
	CBHDateTime timeTmp=CBHDateTime::GetCurrentTime();
	_timeCurr=timeTmp;
	_timeSlowThLearn=timeTmp;
	_timeFastThLearn=timeTmp;
	_timeTrendCheckHour=timeTmp;
	_timeTrendCheckDay=timeTmp;
	_timeTrendCheckMonth=timeTmp;
	_timeTrendCheckSeason=timeTmp;
	_tEndSlowAlarmTime=CBHDateTime(1999,1,1,1,1,1);
	_timeNewAlarmLast=CBHDateTime(1999,1,1,1,1,1);
	_timeNewCustomAlarmLast=CBHDateTime(1999,1,1,1,1,1);
	_vVibCharValue.clear();
	_iFastStatus=0;
	_iSlowStatus=0;
	_iTrendStatus=0;
	_iCustomStatus=0;
	m_nRegStatus =0;
    _fRegValueLast=.0;
	m_nFastRegStatus=0;

	_iCurrThIDFast = 0;
	_iCurrThIDSlow = 0;
	_iRegAlarmFlag = -1;
	_iSlowAlarmFlag = -1;
	_iCustomAlarmFlag=-1;
	_iAlarmBit     = D_BHKD_AT_RADIALVIB;
	_siNewAlarmCountsS = 0;
	for (int i=0;i<10;i++)
	{
		_siCountsS[i]      = 0;
		_siAlarmCountsS[i] = 0;
	}
	for (int nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
	{
		_pthSlowLast[nloop]._fThreshold1=.0;
		_pthSlowLast[nloop]._fThreshold2=.0;
		_pthSlow[nloop]._fThreshold1=.0;
		_pthSlow[nloop]._fThreshold2=.0;
	}
	for (int nloop=0;nloop<G_D_ALARM_SAVEWAVE_NUM;++nloop)
	{
		_ppfWaveBuf[nloop]=NULL;
	}

	for (int nloop=0;nloop<D_ALARM_SAVECHAR_NUM;++nloop)
	{
		_ptimeBuf[nloop]=timeTmp;
	}

	//modified by zdl 2010/10/19 没有初始化数据
	this->_iEventIDSlow = 0;
	this->_iIDSlow = 0;
	this->_iEventIDFast = 0;
	this->_iCurrWavePIndex = 0;
	this->m_fLastAlarmValue = .0;
	this->m_fAxisValue = 0;

	this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
	this->InnerAlarmStatus(CHANN_ALARM_STATUS_INIT);
	this->m_oHalfNormalTimer5 = CBHDateTime::GetCurrentTime();
	this->ResetContinuousMoreThanTimes();
	this->PlantNewAlarmOccoured(FALSE);
	this->LastMaxAlarmValue(0);
	this->TimerForAxisValueLogic(CBHDateTime::GetCurrentTimeEx());
	this->NewAlarmValue(G_F_INIT_VALUE);

	this->m_nLastAlarmType = E_NORMAL;

	this->m_dtLastAlarmTime = CBHDateTime(2000,1,1,1,1,1,0);

	this->m_nAlarmCount = 0;

	this->m_nAlarmLevel = ALARM_NORMAL;

	this->m_fLastDieselAlarmValue = 0;
}

CVibChannParamAlarm::~CVibChannParamAlarm()
{
	_vVibCharValue.clear();
}

int CVibChannParamAlarm::JudgeAlarm()
{
	int iRtn=0;
/*	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;
	if (!_bLearnTh)
	{
		_iCurrThIDSlow=LearnAddSlowThreshold(E_TRIG_TH_RESTART);
//		_iCurrThIDFast=LearnFastThreshold(E_TRIG_TH_RESTART);
		_bLearnTh=true;
	}
  	iRtn=JudgeTrendAlarm();
    iRtn=iRtn|JudgeSlowAlarm();
	iRtn=iRtn|JudgeRegAlarm();*/
	return iRtn;
}

int CVibChannParamAlarm::JudgeSlowAlarm(IDBInterFace *pDb_)
{
	if (_channInfo._iChannType==GE_VIBCHANN)
		return JudgeRadialVibSlowAlarm(pDb_);
	else
		return JudgeAxisVibSlowAlarm(pDb_);
}

int CVibChannParamAlarm::JudgeAxisVibSlowAlarm(IDBInterFace * pDb_)
{
	int iStatus=0,iRtn=0;
/*	int iAlarmNum=0;
	bool bTmp=false;
	if (!_channInfo._chAlarmParam._bCharSlow)
	{//不判断缓变报警
		if (_bAlarmStatusS)
		{
			HandleSlowAlarmStop();
			return 0;
		}
	}
	else
	{
		if (_bAlarmStatusS)
		{
			if (JudgeAxisSlowNewAlarm())
				return 2;
			else
				return 1;
		}
		else
		{
			iStatus=JudgeAxisSlowCharValue(iAlarmNum,bTmp);
			if (iAlarmNum>1)
			{
				_bAlarmStatusS=true;
				iStatus|=D_BHKD_AT_SLOW;
				iStatus|=D_BHKD_AT_AXISVIB;
				_iSlowStatus|=iStatus;
				AddSlowAlarmLog();
				iRtn=3;
				goto QUIT_JUDGEAXIALVIBSLOWALARM;
			}
			if (_bFirstAlarmS)
			{
				if (iAlarmNum>0)
					_siAlarmCountsS++;
				_siCountsS++;
				if (!(_siCountsS < D_ALARM_COUNTS))
				{
					if (!(_siAlarmCountsS<D_ALARM_ALARM_COUNTS))
					{
						_bAlarmStatusS=true;
						iStatus|=D_BHKD_AT_SLOW;
						iStatus|=D_BHKD_AT_AXISVIB;
						_iSlowStatus|=iStatus;
						AddSlowAlarmLog();
						iRtn=3;
						goto QUIT_JUDGEAXIALVIBSLOWALARM;
					}
					else
					{
						_bFirstAlarmS=false;
						_siAlarmCountsS=0;
						_siCountsS=0;
						iRtn=0;
					}
				}
				else
					iRtn=0;
			}
			else
			{
				if (iAlarmNum>0)
				{
					_bFirstAlarmS=true;
					_siCountsS++;
					_siAlarmCountsS++;
				}
				iRtn=0;
			}
		}
	}*/
QUIT_JUDGEAXIALVIBSLOWALARM:
	return iRtn;
}

int CVibChannParamAlarm::JudgeRadialVibSlowAlarm(IDBInterFace *pDb_)
{
	int iRtn=-1;
	COleDateTimeSpan span;
	const int iCharNum=10;
	int iAlarmNum[iCharNum];
	for (int i=0;i<iCharNum;i++)
		iAlarmNum[i]=0;
	int iStatus=0;
	bool bTmp=false;

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;

	bool isPlantRunning = TRUE;

	if (NULL != pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun))
		{
			isPlantRunning = FALSE;
		}
	}

	if (!_channInfo._chAlarmParam._bCharSlow || !isPlantRunning)
	{//不判断缓变报警
		if (_bAlarmStatusS)
		{
			//CHZLogManage::Info("开始处理缓变量报警停止后的操作","CVibChannParamAlarm","JudgeRadialVibSlowAlarm");
			CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
			if(pAlarmPlant == NULL)
			{
				return 0;
			}
			pAlarmPlant->_bVibSlowAlarmStatus=false;//将测点缓变报警状态置位false
			HandleSlowAlarmStop(pDb_);
			return 0;
		}
	}
	else
	{
		if (!_bLearnTh)
		{//初次启动缓变报警判断时，系统触发一次自学习
			//CHZLogManage::Info("初次启动缓变报警判断时，系统触发一次自学习","CVibChannParamAlarm","JudgeRadialVibSlowAlarm");
			_iCurrThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_RESTART);
			_bLearnTh=true;
		}
		if(_iSlowStatus==0)
		{//不处于报警状态，判断门限学习
			span=_timeCurr-_timeSlowThLearn;
			if (span.GetTotalHours()>D_ALARM_STH_LEARN_INTV)
			{
				//CHZLogManage::Info("开始定时自学习","CVibChannParamAlarm","JudgeRadialVibSlowAlarm");
				_iCurrThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_TIME);
				_timeSlowThLearn=_timeCurr;

			}
		}
        //上一次报警结束过后30分钟，触发自学习一次，且这期间不判断缓变报警，避免因自学习了不稳定的数据而导致阈值不准确，重复报警
		span=_timeCurr-_tEndSlowAlarmTime;
		//CString strTmp;strTmp.Format("报警结束的时间:%s,时间段:%f",TimeConvertString(_tEndSlowAlarmTime),span.GetTotalMinutes());
		//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","HandleSlowAlarmStop");
		if (span.GetTotalMinutes()<=D_ALARM_SLOW_ENDTIME)
		{
			if (span.GetTotalMinutes()==D_ALARM_SLOW_ENDTIME && (!_bAlarmLearnTh))
			{
				_iCurrThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_ALARM);
				_timeSlowThLearn=_timeCurr;
				_bAlarmLearnTh=true;
			}
			return 0;
		}
        //如处于报警状态时，判断是否有新的报警产生
		if (_bAlarmStatusS)
		{
			if (JudgeRadialSlowNewAlarm())
				return 2;
			else
				return 1;
		}
		else
		{
			iStatus=JudgeRadialSlowCharValue(iAlarmNum,iCharNum,bTmp);
			for (int i=0;i<iCharNum;i++)
			{
				if (_bFirstAlarmS)
				{
					_siCountsS[i]++;
					if (iAlarmNum[i]>0)
						_siAlarmCountsS[i]++;
					//CString sTemp;sTemp.Format("_siAlarmCountsS[%d]:%d,_siCountsS[%d]:%d",i,_siAlarmCountsS[i],i,_siCountsS[i]);
					//CHZLogManage::Debug(sTemp,"CVibChannParamAlarm","JudgeRadialVibSlowAlarm");
					if (!(_siCountsS[i] < D_ALARM_COUNTS))
					{
						if (!(_siAlarmCountsS[i]<D_ALARM_ALARM_COUNTS))
						{
							_bAlarmStatusS=true;
							_siAlarmCountsS[i]=0;
							_siCountsS[i]=0;
							iStatus|=D_BHKD_AT_SLOW;
							iStatus|=D_BHKD_AT_RADIALVIB;
							_iSlowStatus|=iStatus;
							if (_iSlowAlarmFlag<0)
							{
								if(AddSlowAlarmLog(pDb_)<0)
								{
									CString strInfo;
									strInfo.Format("%s 径向振动添加缓变报警日志失败",_channInfo._cChannID);
									CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRadialVibSlowAlarm");
								}
								_iSlowAlarmFlag=1;
							}
							iRtn=3;
							goto QUIT_JUDGESLOWALARM;
						}
						else
						{
							_bFirstAlarmS=false;
							_siAlarmCountsS[i]=0;
							_siCountsS[i]=0;
							if (_iSlowAlarmFlag ==1)
							{
							    _iSlowAlarmFlag=0;
							}
							iRtn=0;
						}
					}
					else
						iRtn=0;
				}
				else
				{
					if (iAlarmNum[i]>0)
					{
						_bFirstAlarmS=true;
					}
					iRtn=0;
				}
			}
		}
	}
QUIT_JUDGESLOWALARM:
	return iRtn;
}


int CVibChannParamAlarm::JudgeTrendAlarm(IDBInterFace * pDb_)
{
   COleDateTimeSpan span=_timeCurr-_timeTrendCheckHour;
   CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
   int iStatus=0;
   DWORD32 iEnumChannType=D_BHKD_AT_RADIALVIB;
   _iTrendStatus=0;
   if (!(_channInfo._chAlarmParam._bCharTrend))
         return 0;
   if (_channInfo._iChannType==GE_AXIALCHANN)
	   iEnumChannType=D_BHKD_AT_AXISVIB;
   else
	   iEnumChannType=D_BHKD_AT_RADIALVIB;
   if (span.GetTotalMinutes()>60)
   {//判断小时趋势报警
     iStatus=JudgeSpecTimeIntvAlarm(pDb_,_timeTrendCheckHour,_timeCurr,D_BHKD_AT_TRENDHOUR);
     if (iStatus>0)
	 {

		 iStatus=D_BHKD_AT_TRENDHOUR|iStatus|iEnumChannType;
		 _iTrendStatus|=iStatus;
		 if(AddTrendAlarmLog(pDb_,iStatus,D_BHKD_AT_TRENDHOUR)<0)
		 {
#ifdef _DEBUG
			 AfxMessageBox("添加小时趋势报警日志失败");
#endif
		 }
	 }
	 _timeTrendCheckHour=_timeCurr;
   }
   span=_timeCurr-_timeTrendCheckDay;
   if (span.GetTotalHours()>24)
   {//判断天趋势报警
	   iStatus=JudgeSpecTimeIntvAlarm(pDb_,_timeTrendCheckDay,_timeCurr,D_BHKD_AT_TRENDDAY);
	   if (iStatus>0)
	   {
		   iStatus=D_BHKD_AT_TRENDDAY|iStatus|iEnumChannType;
		   _iTrendStatus|=iStatus;
		   if(AddTrendAlarmLog(pDb_,iStatus,D_BHKD_AT_TRENDDAY)<0)
		   {
#ifdef _DEBUG
			   AfxMessageBox("添加天趋势报警日志失败");
#endif
		   }
	   }
	   _timeTrendCheckDay=_timeCurr;
   }
   span=_timeCurr-_timeTrendCheckMonth;
   if (span.GetTotalDays()>30)
   {//判断月趋势报警
	   iStatus=JudgeSpecTimeIntvAlarm(pDb_,_timeTrendCheckMonth,_timeCurr,D_BHKD_AT_TRENDMONTH);
	   if (iStatus>0)
	   {
		   iStatus=D_BHKD_AT_TRENDMONTH|iStatus|iEnumChannType;
		   _iTrendStatus|=iStatus;
		   if(AddTrendAlarmLog(pDb_,iStatus,D_BHKD_AT_TRENDMONTH)<0)
		   {
#ifdef _DEBUG
			   AfxMessageBox("添加月趋势报警日志失败");
#endif
		   }
	   }
	   _timeTrendCheckMonth=_timeCurr;
   }
   span=_timeCurr-_timeTrendCheckSeason;
   if (span.GetTotalDays()>90)
   {//判断季趋势报警
	   iStatus=JudgeSpecTimeIntvAlarm(pDb_,_timeTrendCheckSeason,_timeCurr,D_BHKD_AT_TRENDSEASON);
	   if (iStatus>0)
	   {
		   iStatus=D_BHKD_AT_TRENDSEASON|iStatus|iEnumChannType;
		   _iTrendStatus|=iStatus;
		   if(AddTrendAlarmLog(pDb_,iStatus,D_BHKD_AT_TRENDSEASON)<0)
		   {
#ifdef _DEBUG
			   AfxMessageBox("添加季度趋势报警日志失败");
#endif
		   }
	   }
        _timeTrendCheckSeason=_timeCurr;
   }
   return _iTrendStatus;
}

int CVibChannParamAlarm::JudgeFastAlarm()
{
	return 0;
}
int CVibChannParamAlarm::JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,S_VibCharValue& CharValue_)
{
	
	//1.停车状态判断，停车设备取消报警状态
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL != pAlarmPlant)
	{
		bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pAlarmPlant,lbRun))
		{
			return 0; //停车不再进行报警
		}
	}

	S_ChannInfo sChannInfo=_channInfo;

	float fValue=CharValue_._fCharAll;

	if (GE_AXIALCHANN==sChannInfo._iChannType)
	{
		fValue=CharValue_._fPhaseThree;
	}

	int iRtn=0;

	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fValue<sChannInfo._chAlarmParam._fAlarmLH)
		{
			if (fValue<sChannInfo._chAlarmParam._fAlarmLL)
			{
				iRtn=2;
			}
			else
			{	
				iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fValue>sChannInfo._chAlarmParam._fAlarmHL)||(fValue<sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fValue>sChannInfo._chAlarmParam._fAlarmHH)||(fValue<sChannInfo._chAlarmParam._fAlarmLL))
			{
				iRtn=2;
			}
			else
			{	 iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		if((fValue<sChannInfo._chAlarmParam._fAlarmHH)&&(fValue>sChannInfo._chAlarmParam._fAlarmLL))
		{
			if((fValue<sChannInfo._chAlarmParam._fAlarmHL)&&(fValue>sChannInfo._chAlarmParam._fAlarmLH))
			{	
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
		break;
	default:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
	}

	if(iRtn > 0)
	{
		int iRegStatus = 0;

		if (GE_AXIALCHANN==_channInfo._iChannType)
		{	
			iRegStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
		}
		else
		{	
			iRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
		}
		if (iRtn==2)
		{   
			iRegStatus=iRegStatus|D_BHKD_AT_REGHIGH;
		}
		else
		{	iRegStatus=iRegStatus|D_BHKD_AT_REGLOW;
		}

		if(this->m_nRegStatus <= 0)
		{
			pAlarmPlant->AddFastAlarmLog(pDB_,_channInfo._cChannNo,dtTime_,iRegStatus,_channInfo._iChannType);
		}
	}
	return iRtn;
}
int CVibChannParamAlarm::AddFastAlarmLog(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant = (CAlarmPlantInfo*)_pParent;

	if(NULL != pAlarmPlant)
	{
		return pAlarmPlant->AddFastAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate,
			_iFastStatus,_channInfo._iChannType);
	}
	else
	{
		return pAlarmPlant->AddFastAlarmLog(pDB_,_channInfo._cChannID,_timeCurr,_iFastStatus,
			_channInfo._iChannType);
	}
}
int CVibChannParamAlarm::AddSlowAlarmLog(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!=pAlarmPlant)
	{
		return pAlarmPlant->AddSlowAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate
			,_iSlowStatus,_channInfo._iChannType,_iCurrThIDSlow);

	}else
	{
		return pAlarmPlant->AddSlowAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,_iSlowStatus,
			_channInfo._iChannType,_iCurrThIDSlow);

	}
}

int CVibChannParamAlarm::HandleSlowAlarmStop(IDBInterFace *pDB_)
{
	_iSlowStatus=0;;
	_bFirstAlarmS=false;
	_siNewAlarmCountsS=0;//新的缓变报警计数器清零
	for (int i=0;i<10;i++)
	{
		_siAlarmCountsS[i]=0;
		_siCountsS[i]     =0;
	}
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	
	if (NULL!= pAlarmPlant)
	{
		pAlarmPlant->SetSlowAlarmLogEndTimeAndThID(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate
			,_channInfo._iChannType,_iCurrThIDSlow);

	}else
	{
		pAlarmPlant->SetSlowAlarmLogEndTimeAndThID(pDB_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType,_iCurrThIDSlow);

	}
	_bAlarmStatusS=false;//在处于报警状态时，报警结束，先完成自学习后再修改报警状态
	_tEndSlowAlarmTime=CBHDateTime::GetCurrentTime();//报警结束时间
	_bAlarmLearnTh=false;//设置触发报警结束后的一次自学习
	//CString strTmp;strTmp.Format("报警结束的时间:%s",TimeConvertString(_tEndSlowAlarmTime));
	//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","HandleSlowAlarmStop");
	return 1;
}

int CVibChannParamAlarm::AddRegAlarmLog(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!=pAlarmPlant)
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}
}

int CVibChannParamAlarm::AddCustomAlarmLog(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->AddCustomAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate,
			_iCustomStatus,_channInfo._iChannType);
	}else
	{
		return pAlarmPlant->AddCustomAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,_iCustomStatus,_channInfo._iChannType);

	}}

int CVibChannParamAlarm::HandleRegAlarmStop(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate,_channInfo._iChannType);
	}else
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDB_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType);
	}
}

int CVibChannParamAlarm::HandleCustomAlarmStop(IDBInterFace *pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL != pAlarmPlant)
	{
		return pAlarmPlant->SetCustomAlarmLogEndTime(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate,_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->SetCustomAlarmLogEndTime(pDB_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType);
	}
}

void CVibChannParamAlarm::ResetRegAlarmStatus()
{
	m_nRegStatus=0;
	_iRegAlarmFlag=-1;
}

void CVibChannParamAlarm::ResetSlowAlarmStatus()
{
	_iSlowAlarmFlag=-1;
}

void CVibChannParamAlarm::ResetCustomAlarmStatus()
{
	_iCustomStatus=0;
	_iCustomAlarmFlag=-1;
}

int CVibChannParamAlarm::LearnAddSlowThreshold(IDBInterFace *pDB_,E_TRIG_TH_LEARN_TYPE iType_)
{
	//CHZLogManage::Info("开始自学习","CVibChannParamAlarm","LearnAddSlowThreshold");
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iLearnLen=D_ALARM_STH_LEARNDATA_LEN;
	int iThID=-1;
	int iAlarmType=D_BHKD_AT_SLOW|D_BHKD_AT_RADIALVIB;
    if (_channInfo._iChannType==GE_AXIALCHANN)
	    iAlarmType=D_BHKD_AT_SLOW|D_BHKD_AT_AXISVIB;
	S_VibCharValue * pVibValue=NULL;
	if (iLearnLen>D_ALARM_SAVECHAR_NUM)
	  iLearnLen=D_ALARM_SAVECHAR_NUM;
	pVibValue=new S_VibCharValue[iLearnLen];
	CBHDateTime timeStart = CBHDateTime(1999,1,1,1,1,1);
	CBHDateTime timeEnd = CBHDateTime(1999,1,1,1,1,1);
	///学习过程
	//读取学习数据
	int index=_iCurrCharValueIndex-iLearnLen+1;
	//CString strTmp;strTmp.Format("读取学习数据的索引:%d,当前特征值索引:%d,学习长度:%d",index,_iCurrCharValueIndex,iLearnLen);
	//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnAddSlowThreshold");
    if (index<0)
		index=D_ALARM_SAVECHAR_NUM+index;
	else
		index=D_ALARM_SAVECHAR_NUM-index;

	if (index < 0)
	{
		index = 0;
	}
	//strTmp.Format("开始学习的索引:%d",index);
	//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnAddSlowThreshold");
    pVibValue[0]=_pCharValueBuf[index];
	timeStart=_ptimeBuf[index];
	for (int nloop=1;nloop<iLearnLen;++nloop)
	{
		++index;
		if (!(index<D_ALARM_SAVECHAR_NUM))
			index=0;
		pVibValue[nloop]=_pCharValueBuf[index];
	}
	timeEnd=_ptimeBuf[index];
	int iTemp=LearnSlowThreshold(iLearnLen,pVibValue);
	//strTmp.Format("学习长度:%d,自学习开始时间:%s,自学习结束时间:%s,临时变量:%d",iLearnLen,TimeConvertString(timeStart),TimeConvertString(timeEnd),iTemp);
	//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnAddSlowThreshold");

    //判断报警阈值的相似性,及保存阈值操作
	bool bNewThS=false;
	for (int nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
	{
		if ((nloop==E_TH_ALLCHAR)||(nloop==E_TH_ONECHAR)||(nloop==E_TH_TWOCHAR)||(nloop==E_TH_THREECHAR)||(nloop==E_TH_HALFCHAR)||(nloop==E_TH_RESCHAR))
		{
			if (_pthSlow[nloop]._fThreshold1>_pthSlowLast[nloop]._fThreshold1)
			{
				//CString strTmp;strTmp.Format("循环次数%d,上一次幅值缓变门限%f",nloop,_pthSlowLast[nloop]._fThreshold2);
				//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnAddSlowThreshold");
				_pthSlowLast[nloop]._fThreshold1=_pthSlow[nloop]._fThreshold1;
				bNewThS=true;
			}
			else
			{
				_pthSlow[nloop]._fThreshold1=_pthSlowLast[nloop]._fThreshold1;
			}
			//CString strTmp;strTmp.Format("循环次数%d,当前幅值缓变门限:%f",nloop,_pthSlow[nloop]._fThreshold2);
			//CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnAddSlowThreshold");
		}
	}
	//若各特征值均没有比上一次大的阈值，则根据时间段保存新的阈值，在超过两天内，则不保存阈值
	if (!bNewThS)
	{
		COleDateTimeSpan timeSpan;
		timeSpan=_timeCurr-_timeSlowThLearn;
		if (timeSpan.GetTotalHours()<D_ALARM_SLOW_JUDGESIMILAR_SAVETHTIME)
			iTemp=-1;//不保存阈值
	}
	pPlant->_pLockThID->BeginWrite();
	try
	{
		pDB->BeginTran();
		if (iTemp<0)
			goto QUIT_LearnAddSlowThreshold;
		////添加门限值到门限表
		iThID=pPlant->_iThID+1;
		pDB->Cmd("INSERT INTO [%s] ",pPlant->GetThTblName().c_str());
		pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s,%s)",gc_cID,gc_cThreshold_ID,gc_cName,gc_cStart_T,gc_cChannType,gc_cType,gc_cSynFlag,gc_cAlarm_Type);
		pDB->Cmd(" VALUES(%d,%d,'%s','%s',%d,%d,0,%d) ",iThID,iThID,_channInfo._cChannNo,TimeConvertString(timeStart),_channInfo._iChannType,iType_,iAlarmType);
		if (pDB->Exec())
		{
			pDB->Cmd("SELECT * from [%s] ",pPlant->GetThTblName().c_str());
			pDB->Cmd(" WHERE [%s]=%d ",gc_cThreshold_ID,iThID);
			if (pDB->More(TRUE))
			{
				pDB->SetBuffer(gc_cThreshold,(BYTE*)_pthSlow,D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CThreshold));
				pDB->SetFieldValue(gc_cThreshold_Len,D_BHKD_ALARM_CHARTYPE_NUM);
				pDB->SetBuffer(gc_cData,(BYTE*)pVibValue,iLearnLen*sizeof(S_VibCharValue));
				pDB->SetFieldValue(gc_cData_Len,iLearnLen);
				pDB->SetBuffer(gc_cLearnParam,(BYTE*)(_channInfo._chAlarmParam._pThLearnParamS),D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CThresholdLearnParam));
				pDB->SetFieldValue(gc_cLearnParam_Len,D_BHKD_ALARM_CHARTYPE_NUM);
				pDB->SetFieldValue(gc_cStart_T,timeStart);
				pDB->SetFieldValue(gc_cEnd_T,timeEnd);
				pPlant->_iThID=iThID;
				_timeSlowThLearn=_timeCurr;
			}
			else
			{
				iThID=-1;
				CHZLogManage::Error("振动测点学习缓变阈值失败!"," CVibChannParamAlarm","LearnAddSlowThreshold");
			}
		}
		else
		{
			CHZLogManage::Error("振动测点添加缓变阈值失败!"," CVibChannParamAlarm","LearnAddSlowThreshold");
			iThID=-1;
			goto QUIT_LearnAddSlowThreshold;
		}
	QUIT_LearnAddSlowThreshold:
		if (iThID>0)
		{
			pPlant->_iThID=iThID;
			pDB->CommitTran();
		}
		else
			pDB->RollbackTran();
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	pPlant->_pLockThID->EndWrite();
	if (pVibValue!=NULL)  delete [] pVibValue;
	return iThID;
}

int CVibChannParamAlarm::LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_)
{
	_timeFastThLearn=_timeCurr;
	return 0;
}

int CVibChannParamAlarm::JudgeRadialSlowCharValue(int *iAlarmNum_,int iCharNum_,bool &bHaveNew_,bool bCheckNew_)
{
	S_VibCharValue charValue=_pCharValueBuf[_iCurrCharValueIndex];
	S_VibCharValue charValueLast;
	for (int i=0;i<iCharNum_;i++)
	{
		iAlarmNum_[i]=0;
	}
	bHaveNew_=false;
	if (bCheckNew_)
	{//需要和上次数据比较
		if (_iCurrCharValueIndex==0)
			charValueLast=_pCharValueBuf[D_ALARM_SAVECHAR_NUM-1];
		else
			charValueLast=_pCharValueBuf[_iCurrCharValueIndex-1];
	}
	int iStatue=0;
	float fValue=.0,fLast=.0;
	fValue=charValue.GetSpecTypeData(GE_ALLCHAR);
	if (fValue>(_pthSlow[E_TH_ALLCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_ALLCHAR]._fParam5))
	{
		if (bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_ALLCHAR);
			if (fLast>(_pthSlow[E_TH_ALLCHAR]._fThreshold1) && fLast>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_ALLCHAR]._fParam5))
			{//一定要确保上次数据已经报警
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
			}
		}
		iStatue|=D_BHKD_AT_CHARALL;
		iAlarmNum_[0]++;
	}

	fValue=charValue.GetSpecTypeData(GE_ONECHAR);
	if(fValue>(_pthSlow[E_TH_ONECHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_ONECHAR]._fParam5))
	{
		iStatue|=D_BHKD_AT_CHARONE;
		iAlarmNum_[1]++;
		if ((!bHaveNew_)&&bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_ONECHAR);
			if (fLast>(_pthSlow[E_TH_ONECHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_ONECHAR]._fParam5))
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
		}
	}
	fValue=charValue.GetSpecTypeData(GE_TWOCHAR);
	if(fValue>(_pthSlow[E_TH_TWOCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_TWOCHAR]._fParam5))
	{
		iStatue|=D_BHKD_AT_CHARTWO;
		iAlarmNum_[2]++;
		if ((!bHaveNew_)&&bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_TWOCHAR);
			if (fLast>(_pthSlow[E_TH_TWOCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_TWOCHAR]._fParam5))
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
		}
	}
	fValue=charValue.GetSpecTypeData(GE_THREECHAR);
	if(fValue>(_pthSlow[E_TH_THREECHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_THREECHAR]._fParam5))
	{
		iStatue|=D_BHKD_AT_CHARTHREE;
		iAlarmNum_[3]++;
		if ((!bHaveNew_)&&bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_THREECHAR);
			if(fLast>(_pthSlow[E_TH_THREECHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_THREECHAR]._fParam5))
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
		}
	}
	fValue=charValue.GetSpecTypeData(GE_HALFCHAR);
	if(fValue>(_pthSlow[E_TH_HALFCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_HALFCHAR]._fParam5))
	{
		iStatue|=D_BHKD_AT_CHARHALF;
		iAlarmNum_[4]++;
		if ((!bHaveNew_)&&bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_HALFCHAR);
			if (fLast>(_pthSlow[E_TH_HALFCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_HALFCHAR]._fParam5))
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
		}
	}

	fValue=charValue.GetSpecTypeData(GE_RESCHAR);
	if(fValue>(_pthSlow[E_TH_RESCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_RESCHAR]._fParam5))
	{
		iStatue|=D_BHKD_AT_CHARRES;
		iAlarmNum_[5]++;
		if ((!bHaveNew_)&&bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_RESCHAR);
			if (fLast>(_pthSlow[E_TH_RESCHAR]._fThreshold1) && fValue>(_channInfo._chAlarmParam._pThLearnParamS[E_TH_RESCHAR]._fParam5))
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
		}
	}
	if(_channInfo._chAlarmParam._bPhaseSlow)
	{//判断相位报警
		fValue=charValue.GetSpecTypeData(GE_PHASECHARONE);
		if (CheckSpecPhaseValueAlarm(fValue,_pthSlow[E_TH_ONEPHASE]))
		{
			iStatue|=D_BHKD_AT_PHASEONE;
			iAlarmNum_[6]++;
			if ((!bHaveNew_)&&bCheckNew_)
			{
				fLast=charValueLast.GetSpecTypeData(GE_PHASECHARONE);
				bHaveNew_=CheckPhaseIFNewAlarm(fValue,fLast,_pthSlow[E_TH_ONEPHASE]);
			}
		}
		fValue=charValue.GetSpecTypeData(GE_PHASECHARTWO);
		if (CheckSpecPhaseValueAlarm(fValue,_pthSlow[E_TH_TWOPHASE]))
		{
			iStatue|=D_BHKD_AT_PHASETWO;
			iAlarmNum_[7]++;
			if ((!bHaveNew_)&&bCheckNew_)
			{
				fLast=charValueLast.GetSpecTypeData(GE_PHASECHARTWO);
				bHaveNew_=CheckPhaseIFNewAlarm(fValue,fLast,_pthSlow[E_TH_TWOPHASE]);
			}
		}
		fValue=charValue.GetSpecTypeData(GE_PHASECHARTHREE);
		if (CheckSpecPhaseValueAlarm(fValue,_pthSlow[E_TH_THREEPHASE]))
		{
			iStatue|=D_BHKD_AT_PHASETHREE;
			iAlarmNum_[8]++;
			if ((!bHaveNew_)&&bCheckNew_)
			{
				fLast=charValueLast.GetSpecTypeData(GE_PHASECHARTHREE);
				bHaveNew_=CheckPhaseIFNewAlarm(fValue,fLast,_pthSlow[E_TH_THREEPHASE]);
			}
		}
		fValue=charValue.GetSpecTypeData(GE_PHASECHARHALF);
		if (CheckSpecPhaseValueAlarm(fValue,_pthSlow[E_TH_HALFPHASE]))
		{
			iStatue|=D_BHKD_AT_PHASEHALF;
			iAlarmNum_[9]++;
			if ((!bHaveNew_)&&bCheckNew_)
			{
				fLast=charValueLast.GetSpecTypeData(GE_PHASECHARHALF);
				bHaveNew_=CheckPhaseIFNewAlarm(fValue,fLast,_pthSlow[E_TH_HALFPHASE]);
			}
		}
	}
	return iStatue;
}

int CVibChannParamAlarm::JudgeAxisSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_)
{
	S_VibCharValue charValue=_pCharValueBuf[_iCurrCharValueIndex];
	S_VibCharValue charValueLast;
	iAlarmNum_=0;
	bHaveNew_=false;
	if (bCheckNew_)
	{//需要和上次数据比较
		if (_iCurrCharValueIndex==0)
			charValueLast=_pCharValueBuf[D_ALARM_SAVECHAR_NUM-1];
		else
			charValueLast=_pCharValueBuf[_iCurrCharValueIndex-1];
	}
	int iStatue=0;
	float fValue=.0,fLast=.0;

	fValue=charValue.GetSpecTypeData(GE_PHASECHARTHREE);
	if (fValue>(_pthSlow[E_TH_THREEPHASE]._fThreshold1))
	{
		if (bCheckNew_)
		{
			fLast=charValueLast.GetSpecTypeData(GE_PHASECHARTHREE);
			if (fLast>(_pthSlow[E_TH_THREEPHASE]._fThreshold1)) {//一定要确保上次数据已经报警
				bHaveNew_=CheckLastAndCurrentValue(fLast,fValue);
			}
		}
		iStatue|=D_BHKD_AT_AXISDISPLACE;
		iAlarmNum_++;
	}
	return iStatue;
}
int CVibChannParamAlarm::RadialSlowAlarmStatusOP()
{
	int iRtn=-1;
/*	COleDateTimeSpan timeSpan;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	int iSmpNum=0,iSmpFreq=0;
	float fRev=.0;
    pAlarmPlant->GetRealSmpInfo(iSmpNum,iSmpFreq,false);
	if (_channInfo._iRevType==1)
		fRev=pAlarmPlant->GetRealRev1(false);
	else
		fRev=pAlarmPlant->GetRealRev2(false);
    if (JudgeSlowNewAlarm())
	{//新的报警产生
		_timeSlowFirstSave=_timeCurr;
        SaveData(_iEventIDSlow,_iCurrCharValueIndex,_iCurrWavePIndex,pAlarmPlant->_sTimeCurr,iSmpNum,iSmpFreq,fRev,_iSlowStatus,pAlarmPlant->_pDB,pAlarmPlant->_sAlarmVibDataTblName);
		iRtn=1;
    }
    else
	{
		timeSpan=_timeCurr-_timeSlowFirstSave;
		int iTotalMin=timeSpan.GetTotalMinutes();
		if (iTotalMin>D_ALARM_CONTSAVETIME)
		{//连续保存报警数据期间
			if (iTotalMin>D_ALARM_INTVSAVETIME)//超过报警数据保存时间，结束报警
				HandleSlowAlarmStop();
			else
			{//处于连续报警保存期间的操作
			  timeSpan=_timeCurr-_timeSlowSaveData;
			  if (timeSpan.GetTotalSeconds()>D_ALARM_INTVSAVETIME_INTV)
				  SaveData(_iEventIDSlow,_iCurrCharValueIndex,_iCurrWavePIndex,pAlarmPlant->_sTimeCurr,iSmpNum,iSmpFreq,fRev,_iSlowStatus,pAlarmPlant->_pDB,pAlarmPlant->_sAlarmVibDataTblName);
			}
		}
		else//没有超过连续保存时间
			SaveData(_iEventIDSlow,_iCurrCharValueIndex,_iCurrWavePIndex,pAlarmPlant->_sTimeCurr,iSmpNum,iSmpFreq,fRev,_iSlowStatus,pAlarmPlant->_pDB,pAlarmPlant->_sAlarmVibDataTblName);
		iRtn=2;
	}*/
	return iRtn;
}

int CVibChannParamAlarm::AxisSlowAlarmStatusOP()
{
	int iRtn=-1;
	return iRtn;
}

bool CVibChannParamAlarm::JudgeAxisSlowNewAlarm()
{
//	bool bRtn=false;
//	int iAlarmNum=0;
//	int iStatus=JudgeSlowCharValue(iAlarmNum,bRtn,true);

	S_VibCharValue charValue=_pCharValueBuf[_iCurrCharValueIndex];
/*	S_VibCharValue charValueLast;
	if (_iCurrCharValueIndex==0)
		charValueLast=_pCharValueBuf[D_ALARM_SAVECHAR_NUM-1];
	else
		charValueLast=_pCharValueBuf[_iCurrCharValueIndex-1];
*/
    float	fValue=charValue.GetSpecTypeData(GE_PHASECHARTHREE);
	if(fValue>(_pthSlow[E_TH_THREEPHASE]._fThreshold1))
		return true;
	else
		return false;
//	return bRtn;
}

bool CVibChannParamAlarm::JudgeRadialSlowNewAlarm()
{
	bool bRtn=false;
	bool bHaveNew=false;
	bool bNewAlarm=false;
	const int iCharNum=10;
	int iAlarmNum[iCharNum];
	for (int i=0;i<iCharNum;i++)
	{
		iAlarmNum[i]=0;
	}
	int iStatus=JudgeRadialSlowCharValue(iAlarmNum,iCharNum,bHaveNew,true);
	for (int i=0;i<iCharNum;i++)
	{
		_siCountsS[i]++;
		if (iAlarmNum[i]>0)
			_siAlarmCountsS[i]++;
		//CString sTemp;sTemp.Format("_siAlarmCountsS[%d]:%d,_siCountsS[%d]:%d",i,_siAlarmCountsS[i],i,_siCountsS[i]);
		//CHZLogManage::Debug(sTemp,"CVibChannParamAlarm","JudgeRadialSlowNewAlarm");
		if (!(_siCountsS[i] < D_ALARM_COUNTS))
		{
			if (!(_siAlarmCountsS[i]<D_ALARM_ALARM_COUNTS))
			{
				bNewAlarm=true;
				_siAlarmCountsS[i]=0;
				_siCountsS[i]=0;
				iStatus|=D_BHKD_AT_SLOW;
				iStatus|=D_BHKD_AT_RADIALVIB;
				_iSlowStatus|=iStatus;
				//CHZLogManage::Info("有新的报警产生","CVibChannParamAlarm","JudgeRadialSlowNewAlarm");
				goto QUIT_JUDGENEWSLOWALARM;
			}
			else
			{
				_siAlarmCountsS[i]=0;
				_siCountsS[i]=0;
			}
		}
	}
QUIT_JUDGENEWSLOWALARM:
	iStatus|=D_BHKD_AT_SLOW;
	iStatus|=D_BHKD_AT_RADIALVIB;
	if (bNewAlarm && bHaveNew)
	{
		_siNewAlarmCountsS++;
		_iSlowStatus=iStatus;
		bRtn=true;
	}
	if (_siNewAlarmCountsS>2)
		bRtn=false;
	return bRtn;
}

bool CVibChannParamAlarm::CheckSpecPhaseValueAlarm(float fValue_,CThreshold th_)
{
	bool bRtn=false;

	return bRtn;
}

bool CVibChannParamAlarm::CheckPhaseIFNewAlarm(float fValue_,float fLastValue_,CThreshold th_)
{
	bool bRtn=false;

	return bRtn;
}

int CVibChannParamAlarm::JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_)
{
	int iStatus=0;
	float *pfValue=NULL;
	//获得数据
	ReadHisData(pDb_,timeStart_,timeEnd_);
	int len=_vVibCharValue.size();
	int nloop=0;
	if (len>0)
	{
	   pfValue=new float[len];
       memset(pfValue,0,len*sizeof(float));
       //通频趋势报警判断
       if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ALLCHAR,iJudgeAlarmType_)>0)
	      iStatus|=D_BHKD_AT_CHARALL;
	   //一倍频趋势报警诊断
	   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ONECHAR,iJudgeAlarmType_)>0)
          iStatus|=D_BHKD_AT_CHARONE;
	   //二倍频趋势报警诊断
	   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_TWOCHAR,iJudgeAlarmType_)>0)
		   iStatus|=D_BHKD_AT_CHARTWO;
	   //三倍频趋势报警诊断
	   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_THREECHAR,iJudgeAlarmType_)>0)
		   iStatus|=D_BHKD_AT_CHARTHREE;
	   //分频趋势报警诊断
	   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_HALFCHAR,iJudgeAlarmType_)>0)
		   iStatus|=D_BHKD_AT_CHARHALF;
	   //残振趋势报警诊断
	   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_RESCHAR,iJudgeAlarmType_)>0)
		   iStatus|=D_BHKD_AT_CHARRES;
       if (_channInfo._chAlarmParam._bPhaseTrend)
	   {
		   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ONEPHASE,iJudgeAlarmType_)>0)
			   iStatus|=D_BHKD_AT_PHASEONE;
		   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_TWOPHASE,iJudgeAlarmType_)>0)
			   iStatus|=D_BHKD_AT_PHASETWO;
		   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_THREEPHASE,iJudgeAlarmType_)>0)
			   iStatus|=D_BHKD_AT_PHASETHREE;
		   if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_HALFPHASE,iJudgeAlarmType_)>0)
			   iStatus|=D_BHKD_AT_PHASEHALF;
       }
	}
	else
		iStatus=-1;
	if (pfValue!=NULL) delete [] pfValue;
	return iStatus;
}

int CVibChannParamAlarm::ReadHisData(IDBInterFace *pDB_,CBHDateTime timeStart_,CBHDateTime timeEnd_)
{
	int nCount=0;
    CString strTblName;
	S_VibCharValue vibCharValue;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	COleDateTimeSpan span;
	_vVibCharValue.clear();
	span=timeEnd_-timeStart_;
	if (span.GetMinutes()>59)
	{//从小时表中读取数据
      strTblName.Format("TREND_VIBHOUR_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else if (span.GetHours()>23)
	{//从天表中读取历史数据
      strTblName.Format("TREND_VIBDAY_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else if (span.GetDays()>29)
	{//从月中读取历史数据
	  strTblName.Format("TREND_VIBMONTH_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else
	{//从秒表中读取历史数据
      strTblName.Format("TREND_VIBSECOND_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
    pDB->Cmd("SELECT %s,%s,%s,%s,%s,%s,%s,%s,%s,%s from [%s] ",_sCharOne.c_str(),_sCharAll.c_str(),_sCharTwo.c_str(),_sCharThree.c_str(),_sCharHalf.c_str(),_sCharRes.c_str(),_sPhaseOne.c_str(),_sPhaseTwo.c_str(),_sPhaseThree.c_str(),_sPhaseHalf.c_str(),strTblName);
	pDB->Cmd(" WHERE %s<'%s' AND %s>'%s' ",gc_cTime,TimeConvertString(timeEnd_),gc_cTime,TimeConvertString(timeStart_));
	while(pDB->More())
	{
        vibCharValue._fCharAll=pDB->GetDouble(_sCharAll.c_str());
		vibCharValue._fCharOne=pDB->GetDouble(_sCharOne.c_str());
		vibCharValue._fCharTwo=pDB->GetDouble(_sCharTwo.c_str());
		vibCharValue._fCharThree=pDB->GetDouble(_sCharThree.c_str());
		vibCharValue._fCharHalf=pDB->GetDouble(_sCharHalf.c_str());
		vibCharValue._fCharRes=pDB->GetDouble(_sCharRes.c_str());
		vibCharValue._fPhaseHalf=pDB->GetDouble(_sPhaseHalf.c_str());
		vibCharValue._fPhaseOne=pDB->GetDouble(_sPhaseOne.c_str());
		vibCharValue._fPhaseTwo=pDB->GetDouble(_sPhaseTwo.c_str());
		vibCharValue._fPhaseThree=pDB->GetDouble(_sPhaseThree.c_str());
		_vVibCharValue.push_back(vibCharValue);
		++nCount;
	}
 	return nCount;
}

int CVibChannParamAlarm::JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_)
{
	CTrendCheckParam trendTh=_channInfo._chAlarmParam._pTrendParam[iCharType_];
	int nloop=0;
	int iStatus=-1;
	bool bAmp=true;
	switch(iCharType_)
	{
	case E_TH_ALLCHAR:
        for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharAll;
		bAmp=true;
		break;
	case E_TH_ONECHAR:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharOne;
		bAmp=true;
		break;
	case E_TH_TWOCHAR:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharTwo;
		bAmp=true;
		break;
	case E_TH_THREECHAR:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharThree;
		bAmp=true;
		break;
	case E_TH_HALFCHAR:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharHalf;
		bAmp=true;
		break;
	case E_TH_RESCHAR:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharRes;
		bAmp=true;
		break;
	case E_TH_ONEPHASE:
	    for(nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fPhaseOne;
		bAmp=false;
		break;
	case E_TH_TWOPHASE:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fPhaseTwo;
		bAmp=false;
		break;
	case E_TH_THREEPHASE:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fPhaseThree;
        bAmp=false;
		break;
	case E_TH_HALFPHASE:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fPhaseHalf;
		bAmp=false;
		break;
	default:
		for (nloop=0;nloop<iLen_;++nloop)
			pfValue_[nloop]=_vVibCharValue[nloop]._fCharAll;
		bAmp=true;
	}
	if (nloop>0)
	{
		if (bAmp)
			iStatus=JudgeAmpTrendDataIFAlarm(pfValue_,nloop,trendTh,iJudgeAlarmType_);
		else
			iStatus=JudgePhaseTrendDataIFAlarm(pfValue_,nloop,trendTh,iJudgeAlarmType_);
	}
    return iStatus;
}


int CVibChannParamAlarm::JudgePhaseTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_)
{
	int iStatus=0;
	S_phaseVarThreshold__CalcInput calcInput;
	double *_pdTmp=new double[iLen_];
	for (int nloop=0;nloop<iLen_;++nloop)
		_pdTmp[nloop]=pfData_[nloop];
	calcInput.pPhase=_pdTmp;
	calcInput.f_hope=trendTh_._fParam1;


	calcInput.f_zoomCorfficient=trendTh_._fParam2;
	calcInput.i_length=iLen_;
	CAnalizeDiagnose_MW dgn;
    S_phaseVarResult rst;
 	if(dgn.calcPhaseVarTrendEnent(calcInput,rst))
		iStatus=rst.d_Var;
	else
	    iStatus=-1;
	delete [] _pdTmp;
	return iStatus;
}

int CVibChannParamAlarm::JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_)
{
	int iStatus=0;
    S_TrendEnent_CalcInput calcInput;
	double *pdData=new double[iLen_];
	for (int nloop=0;nloop<iLen_;++nloop)
		pdData[nloop]=pfData_[nloop];
	calcInput.pTrendArrChar=pdData;
	calcInput.d_threshold=trendTh_._fParam1;
	calcInput.i_length=iLen_;
	int iType=1;
	switch(iJudgeAlarmType_)
	{
	case D_BHKD_AT_TRENDHOUR:
		iType=1;
		break;
	case D_BHKD_AT_TRENDDAY:
		iType=2;
		break;
	case D_BHKD_AT_TRENDMONTH:
		iType=3;
		break;
	case D_BHKD_AT_TRENDSEASON:
		iType=4;
		break;
	default:
		iType=1;
	}
	calcInput.i_trendType=iType;
	CAnalizeDiagnose_MW dgn;
	S_TrendEnent_result trendRs;
	if(dgn.calcTrendEnent(calcInput,trendRs))
	{
		switch(trendRs.i_trendEventResult)
		{
		case 0:
			iStatus=0;
			break;
		case 1:
           iStatus=D_BHKD_AT_TRENDSLOWASC;
		   break;
		case 2:
			iStatus=D_BHKD_AT_TRENDFASTASC;
			break;
		case 3:
			iStatus=D_BHKD_AT_TRENDSLOWDSC;
			break;
		case 4:
			iStatus=D_BHKD_AT_TRENDFASTDSC;
			break;
		default:
			iStatus=0;
		}
	}
	else
		iStatus=-1;
	delete [] pdData;
	return iStatus;
}

int CVibChannParamAlarm::AddTrendAlarmLog(IDBInterFace *pDB_,int iStatus_,int iTrendType_)
{
	S_VibCharValue *pVibCharValue=NULL;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
    IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	int iLen=_vVibCharValue.size();
    int iEventID=pAlarmPlant->_iTrendEventID+1;
	float fRev=pAlarmPlant->GetSpecTypeRealRev( _channInfo._iRevType , false);

	if (iLen>0)
	{
		pVibCharValue=new S_VibCharValue[iLen];
		for (int nloop=0;nloop<iLen;++nloop)
			pVibCharValue[nloop]=_vVibCharValue[nloop];
		pDB->BeginTran();
		//[TBC]
		pDB->Cmd("INSERT INTO [%s]",pAlarmPlant->GetAlarmLogTblName().c_str());
		pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cChannNo,gc_cChannType,gc_cRev,gc_cTime,gc_cTrendType,gc_cType,gc_cSynFlag);
		pDB->Cmd(" VALUES(%d,0,'%s',%d,%f,'%s',%d,%d,0) ",iEventID,_channInfo._cChannNo,_channInfo._iChannType,fRev,pAlarmPlant->_sTimeCurr.c_str(),iTrendType_,iStatus_);
		if (pDB->Exec())
		{
			pDB->Cmd("SELECT %s,%s,%s,%s from [%s] ",gc_cData,gc_cData_Len,gc_cThreshold,gc_cThreshold_Len,pAlarmPlant->GetAlarmLogTblName().c_str());
			pDB->Cmd(" WHERE %s=%d  ",gc_cEventID,iEventID);
			if (pDB->More(TRUE))
			{
				pDB->SetFieldValue(gc_cData_Len,iLen);
				pDB->SetFieldValue(gc_cThreshold_Len,D_BHKD_ALARM_CHARTYPE_NUM);
				pDB->SetBuffer(gc_cThreshold,(BYTE *)(_channInfo._chAlarmParam._pTrendParam),D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CTrendCheckParam));
				pDB->SetBuffer(gc_cData,(BYTE *)pVibCharValue,sizeof(S_VibCharValue)*iLen);
				pDB->CommitTran();
				pAlarmPlant->_iTrendEventID=iEventID;
			}
			else
			{
				pDB->RollbackTran();
				iEventID=-1;
			}
		}
		else
		{
			pDB->RollbackTran();
			iEventID=-1;
		}
	}
	if (pVibCharValue!=NULL) delete [] pVibCharValue;
	return iEventID;
}

void CVibChannParamAlarm::SetParentPoint(LPVOID pParent_)
{
	_pParent=pParent_;
	if (GE_VIBCHANN==_channInfo._iChannType)
	  _iAlarmBit=D_BHKD_AT_RADIALVIB;
	else
	  _iAlarmBit=D_BHKD_AT_AXISVIB;
}

int CVibChannParamAlarm::GetDcAndGap(int index_,float &fDC_,float &fGap_)
{
	fDC_=_pCharValueBuf[index_]._fDC;
	fGap_=_pCharValueBuf[index_]._fGap;
	return 1;
}

DATATYPE_CHAR CVibChannParamAlarm::GetSpecCharTypeValue(int index_,int iCharType_/* =GE_ALLCHAR */)
{
	return _pCharValueBuf[index_].GetSpecTypeData(iCharType_);
}

int CVibChannParamAlarm::GetCurrDcAndGap(float &fDC_,float &fGap_)
{
	fDC_=_pCharValueBuf[_iCurrCharValueIndex]._fDC;
	fGap_=_pCharValueBuf[_iCurrCharValueIndex]._fGap;
	return 1;
}

DATATYPE_CHAR CVibChannParamAlarm::GetCurrSpecCharTypeValue(int iCharType_/* =GE_ALLCHAR */)
{
	return _pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(iCharType_);
}

void CVibChannParamAlarm::SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	for (int nloop=0;nloop<G_D_ALARM_SAVEWAVE_NUM;++nloop)
	{
		_ppfWaveBuf[nloop]=&(pWave_[nloop*iSmpNum_]);
	}

	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数，取自中间件组态时设备设定的采样点
}

CString CVibChannParamAlarm::TimeConvertString(CBHDateTime time_)
{
	CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;
}

bool CVibChannParamAlarm::SetChannValue(BHKD::S_VibCharValue *pVibCharValue_)
{
     CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	
	 ++_iCurrCharValueIndex;
	 
	 if (!(_iCurrCharValueIndex<D_ALARM_SAVECHAR_NUM))
	 {
		 _iCurrCharValueIndex=0;
	 }
	
	 CBHDateTime dt=CBHDateTime::GetCurrentTime();
	
	 pPlant->_pLockDataUpdateStatues->BeginRead();
	 dt = pPlant->_timeVibDataUpdate;
	 pPlant->_pLockDataUpdateStatues->EndRead();

	 _ptimeBuf[_iCurrCharValueIndex]=dt;
	
	_pCharValueBuf[_iCurrCharValueIndex]=(*pVibCharValue_);	

	if (_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum > this->_nChannWavePointNum)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d****************** : 数采配置的采样点数:%d 大于中间件组态配置的采样点数:%d，忽略多余点数"),
							__FILE__,
							__LINE__,_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum,
							_nChannWavePointNum);

		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum = this->_nChannWavePointNum;
	}

	return true;
}

bool CVibChannParamAlarm::SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_)
{
    if ((pfWave_==NULL)||(iSmpNum_<0))
	{
		return false;
	}

	++_iCurrWavePIndex;
	
	ASSERT(this);
	
	if (!(_iCurrWavePIndex<G_D_ALARM_SAVEWAVE_NUM))
	{
		_iCurrWavePIndex=0;
	}

	//取初始采样点数和传入采样点数中较小的作为实际的采样点数
	int lnReciveDataSize = iSmpNum_ * sizeof(DATATYPE_WAVE);

	int lnBufferSize = this->_nChannWavePointNum  * sizeof(DATATYPE_WAVE);

	if (lnReciveDataSize>lnBufferSize)
	{
		lnReciveDataSize = lnBufferSize;

		iSmpNum_ = lnReciveDataSize/sizeof(DATATYPE_WAVE);
	}

	_sVibCharValue.SetSpecTypeData(lnReciveDataSize/sizeof(DATATYPE_WAVE),GE_SAMPLENUM);

	memcpy(_ppfWaveBuf[_iCurrWavePIndex],pfWave_,lnReciveDataSize);

	return true;
}

//得到全部特征值
S_VibCharValue CVibChannParamAlarm::GetCharValueEx(int index_)
{
	return _pCharValueBuf[index_];
}

S_VibCharValue CVibChannParamAlarm::GetCharValueEx()
{
	return _pCharValueBuf[GetCurrCharValueIndex()];
}


S_VibCharValue CVibChannParamAlarm::GetCharValue(int index_)
{
	return _pCharValueBuf[index_];
}

S_VibCharValue CVibChannParamAlarm::GetCharValue()
{
	return _pCharValueBuf[GetCurrCharValueIndex()];
}

const DATATYPE_WAVE * CVibChannParamAlarm::GetCurrWavePoint()
{
	return _ppfWaveBuf[GetCurrWaveIndex()];
}

const DATATYPE_WAVE * CVibChannParamAlarm::GetWavePoint(int iIndex_)
{
	return _ppfWaveBuf[iIndex_];
}

unsigned int CVibChannParamAlarm::GetAlarmStatus(S_AlarmState *pAlarmState_)
{
    if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iFastState=_iFastStatus;
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iTrendState=_iTrendStatus;
		pAlarmState_->_iRegState=m_nRegStatus;
    }
	return _iSlowStatus|_iTrendStatus|_iFastStatus|m_nRegStatus;
}

unsigned int CVibChannParamAlarm::GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iFastState=_iFastStatus;
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iTrendState=_iTrendStatus;
		pAlarmState_->_iRegState=m_nRegStatus;
		pAlarmState_->_fLastAlarmVal=m_fLastAlarmValue;
		pAlarmState_->_iTrendState = m_nAlarmLevel;
	}
	return _iSlowStatus|_iTrendStatus|_iFastStatus|m_nRegStatus;
}


void CVibChannParamAlarm::SetAlarmStatus(S_AlarmState& sAlarmState_)
{
	_iFastStatus=sAlarmState_._iFastState;
	m_nRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=sAlarmState_._iTrendState;
}

void CVibChannParamAlarm::SetAlarmStatus( S_AlarmStateWithLevel& sAlarmState_ )
{
	_iFastStatus=sAlarmState_._iFastState;
	m_nRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=sAlarmState_._iTrendState;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iAlarmLevel;
}
void CVibChannParamAlarm::SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_)
{
	_iFastStatus=sAlarmState_._iFastState;
	m_nRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=0;
	m_fLastAlarmValue=sAlarmState_._fLastAlarmVal;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iTrendState;

}


bool CVibChannParamAlarm::CheckLastAndCurrentValue(float fLast_,float fCurr_)
{
	bool bRtn=false;
	if(fLast_!=0)
	{
		if((fCurr_/fLast_)>D_FLUCTUATE_RANGE)
			bRtn=true;
	}
	else
		CHZLogManage::Info("上次特征值为0，不能判断新的报警","CVibChannParamAlarm","CheckLastAndCurrentValue");
	return bRtn;
}

int CVibChannParamAlarm::JudgeRegAlarm(IDBInterFace * pDb_,
									   int &iFlag_,
									   bool &abSaveAllAlarmDataFlag_,
									   bool &abSaveOnlyOneAlarmData_,
									   bool bConfirmAlarm_)
{
	iFlag_=-1;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;

	int lnRegStatus = 0;
	bool lbJudgeAlarm = false;

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();
	if (loTimeSpan.GetTotalHours() < GetAlarmOverridTime())
	{
		lbJudgeAlarm = true;
	}
	
	if(_channInfo._chAlarmParam._bRegular)
	{
		/**@brief 判断常规数据是否报警
		 *@return 如果高限报警就返回2，低限报警就返回1，不报警就返回0
	    */
		int iType=JudgeRegDataAlarm(pDb_,bConfirmAlarm_);

		this->ComputeEigenValue(pPlant->GetPlantInfo()._iMachineType);

		float fCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

		S_ChannInfo sChannInfo=_channInfo;			

		if (GE_AXIALCHANN==sChannInfo._iChannType)
		{	
			fCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;
		}
		
		//有报警发生
		if (iType>0)
		{
			
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnRegStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
			}
			else
			{	
				lnRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			}
			if (iType==2)
			{   
				lnRegStatus=lnRegStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	lnRegStatus=lnRegStatus|D_BHKD_AT_REGLOW;
			}

			m_nFastRegStatus = lnRegStatus;

			if(lbJudgeAlarm)
			{
				this->m_fLastAlarmValue = fCurrValue;
				m_nRegStatus = lnRegStatus;
			}
			

			///常规报警判断标志。-1表示当前没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
			/*if(_iRegAlarmFlag<0)*/
			if(1)
			{
			
				COleDateTimeSpan dtSpan;
				dtSpan=_timeCurr-_timeNewAlarmLast;
				int iTotaldays=dtSpan.GetTotalDays();
			
				if (iTotaldays<D_ALARM_REG_DAYTIME)
				{
					/**@brief 根据报警方式，比较两次报警时刻的特征值，判断重复报警
					 *@param fCurrValue_ 当前报警时刻的特征值
					 @param fValueLast_ 上次报警时刻的特征值
					 *@return 返回状态
					*/
					bool bRtn=JudgeRegAgainAlarm(fCurrValue,_fRegValueLast);
					
					//如果是重复报警
					if (bRtn)
					{
                        CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
						COleDateTimeSpan dtSpan;
						dtSpan=_timeCurr-(pPlant->_dtVibRegDataSaveOne);
						int iTotalMinutes=dtSpan.GetTotalMinutes();

						//判断上次保存报警数据到现在，时间间隔是否大于30分钟
						if (iTotalMinutes>=D_ALARM_REG_INTVTIME)
						{
							//如果大于的话，只是单独保存一条，并不更新任何计数器，状态值
							abSaveOnlyOneAlarmData_=true;

						}						

					}
					else
					{
						if (-1 == this->_iRegAlarmFlag)
						{
							//如果不是重复报警，就给这个测点增加一条报警日志，需要更新报警计时器
					
							if(AddRegAlarmLog(pDb_)<0)
							{
								CString strInfo;
								strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
								CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
								//常规报警状态为0，这个值有什么用？
								m_nRegStatus = 0;	

							}else
							{
								if (this->JudgeRegResetPlantAlarmTimer(fCurrValue,_fRegValueLast))
								{
									abSaveAllAlarmDataFlag_ = true;
									iFlag_=1;

								}else
								{
									abSaveOnlyOneAlarmData_ = true;
								}
							}
							

						}else
						{
							if (this->JudgeRegResetPlantAlarmTimer(fCurrValue,_fRegValueLast))
							{
								abSaveAllAlarmDataFlag_ = true;
								iFlag_=1;

							}else
							{
								abSaveOnlyOneAlarmData_ = true;
							}
						}

						_timeNewAlarmLast=_timeCurr;
						_fRegValueLast=fCurrValue;
						_iRegAlarmFlag=1;
						

					}
				}
				else
				{
					
					if(AddRegAlarmLog(pDb_)<0)
					{
						CString strInfo;
						strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
						CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
						m_nRegStatus = 0;						
					}else
					{
						abSaveAllAlarmDataFlag_=true;
					}	
					
					_timeNewAlarmLast=_timeCurr;
					_fRegValueLast=fCurrValue;
					_iRegAlarmFlag=1;
					iFlag_=1;
				}
			}
			else
			{
				//已经报警的测点不改变任何报警状态值	
			}
		}
		else
		{
			m_nRegStatus=0;
			
			if (_iRegAlarmFlag==1)
			{
				_iRegAlarmFlag=0;
				_fRegValueLast=fCurrValue;
				HandleRegAlarmStop(pDb_);
			}
		}
	}
	else
	{
         if(m_nRegStatus>0)
		 {	 
			 HandleRegAlarmStop(pDb_);
		 }

		 m_nRegStatus=0;
		
		_iRegAlarmFlag=-1;
	}
	return lnRegStatus;
}

int CVibChannParamAlarm::JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveLastAlarmData_)
{
	iFlag_=-1;

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!=pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun))
		{
			return 0;
		}

		//2.报警确认状态判断，已确认报警取消报警状态
// 		if (this->IsAlarmConfirmed())
// 		{
// 			GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();
// 
// 			if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
// 			{
// 				
// 				this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 
// 			}
// 
// 			return 0;
// 		}
	}

	int iSzie=GetCustomAlarmParamSize();
	float pfCustomData[GD_CUSTOMPARAM_BUFFSIZE] = {.0};
	pPlant->GetSpecRadialVibChannCustomData(_channInfo._cChannID,pfCustomData);

	int iRtn=0;
	for (int i=0;i<iSzie;i++)
	{
		CCustomAlarmParamInfo customAlarmParamInfo;
		customAlarmParamInfo=GetSpecIndexCustomAlarmParamInfo(i);
	
		int iStartSegNo=customAlarmParamInfo._iSegStartNo;
		int iSegNum=customAlarmParamInfo._iSegNum;

		for (int j=iStartSegNo;j<iStartSegNo+iSegNum;j++)
		{
			if(customAlarmParamInfo._iIF_Alarm>0)
			{
				//根据报警判断类型获得报警类型
				switch(customAlarmParamInfo._iJudgeType)
				{
				case 1://低通
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
					{
						if(pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
					break;
				case 2://高通
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
					{
						if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
					break;
				default:
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
					{
						if(pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
				}

				if (iRtn>0)
				{
					if (GE_AXIALCHANN==_channInfo._iChannType)
						_iCustomStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_CUSTOM;
					else
						_iCustomStatus=D_BHKD_AT_CUSTOM|_iAlarmBit;
					if (iRtn==2)
						_iCustomStatus=_iCustomStatus|D_BHKD_AT_CUSTOMHIGH;
					else
						_iCustomStatus=_iCustomStatus|D_BHKD_AT_CUSTOMLOW;

					if(_iCustomAlarmFlag<0)
					{
						COleDateTimeSpan dtSpan;
						dtSpan=_timeCurr-_timeNewCustomAlarmLast;
						int iTotaldays=dtSpan.GetTotalDays();
					
						//自定义报警间隔两天新增一条报警日志
						if (iTotaldays>D_ALARM_REG_DAYTIME)
						{
							//添加自定义报警日志
							if(AddCustomAlarmLog(pDb_)<0)
							{
								CString strInfo;
								strInfo.Format("%s-%s 振动添加自定义报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
								CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarmEx");
							}
							_iCustomAlarmFlag=1;
							iFlag_=1;
							_timeNewCustomAlarmLast=_timeCurr;
						}
						else
						{
							_iCustomAlarmFlag=1;
							iFlag_=0;
							bSaveLastAlarmData_=true;
						}
					}
				}
				else
				{
					_iCustomStatus=0;
					if (_iCustomAlarmFlag==1)
					{
						_iCustomAlarmFlag=0;
						HandleCustomAlarmStop(pDb_);
					}
				}
			}
			else
			{
				if(_iCustomStatus>0)
				{	
					HandleCustomAlarmStop(pDb_);
				}
				_iCustomStatus=0;
				_iCustomAlarmFlag=-1;
			}
		}
	}

	return _iCustomStatus;
}

bool CVibChannParamAlarm::JudgeRegAgainAlarmForConfirm()
{
	if (!this->_pParent)
	{
		return true;
	}

	float fCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

	S_ChannInfo sChannInfo=_channInfo;			

	if (GE_AXIALCHANN==sChannInfo._iChannType)
	{	
		fCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;
	}

	//新的轴位移报警逻辑
	if (g_UseNewAxisAlarmLogic&&g_UseNewAlarmLogic)
	{
		if (GE_AXIALCHANN == this->_channInfo._iChannType)
		{
			bool lbRun = ((CAlarmPlantInfo *)this->_pParent)->GetVibJudgeRunStatus()||((CAlarmPlantInfo *)this->_pParent)->GetProcJudgeRunStatus();
			if (IsPlantRunning(*((CAlarmPlantInfo *)this->_pParent),lbRun))
			{
				float lfOriginalAxisValue = this->AxisOriginal();

				if (IsFloatZero(G_F_INIT_VALUE -lfOriginalAxisValue))
				{
					this->AxisOriginal(fCurrValue);

					this->SaveAxisValueForNewAxisLogic();
				}

				fCurrValue= abs(fCurrValue - this->AxisOriginal());
			}else
			{
				fCurrValue = 0;
			}
		}			 
	}

	bool bRtn = false;
	if (g_UseNewAlarmLogic)
	{
		bRtn=JudgeRegResetPlantAlarmTimer(fCurrValue,this->LastMaxAlarmValue());
	}else
	{

		bRtn=JudgeRegResetPlantAlarmTimer(fCurrValue,this->_fRegValueLast);	
	}
	return !bRtn;
}


bool CVibChannParamAlarm::JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_)
{
	return CChannInfoBase::JudgeRegResetPlantAlarmTimer(fCurrValue_,fValueLast_);
}

bool CVibChannParamAlarm::JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_)
{
	bool bRtn=false;
	S_ChannInfo sChannInfo=_channInfo;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fCurrValue_<=fValueLast_)
		{
			bRtn=true;
		}
		else
		{
			bRtn=false;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:

		if (fCurrValue_>=fValueLast_)
		{
			bRtn=true;
		}
		else
		{
			bRtn=false;
		}

		break;
	case E_ALARMCHECK_BANDPASS:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;
			
			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);
	
			if (fCurrValue_<=fValueLast_)
			{
	
				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		
		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_>=fValueLast_)
			{

				bRtn=true;
			}
			else
			{
 				bRtn=false;
			}
		}
		break;
	default:
		if (fCurrValue_<=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	}
	return bRtn;
}

int CVibChannParamAlarm::JudgeRegDataAlarm(S_VibCharValue & arefVibValue)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	
	//1.停车状态判断，停车设备取消报警状态
	if (NULL != pAlarmPlant)
	{
		bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pAlarmPlant,lbRun))
		{
			return 0; //停车不再进行报警
		}
	}

	S_ChannInfo sChannInfo=_channInfo;

	float fValue=arefVibValue._fCharAll;

	BOOL lbNeedComputeEigenValue = this->IsNeedComputeEigenValue(arefVibValue);

	if (lbNeedComputeEigenValue)
	{
		fValue = this->ComputeAllCharValue(arefVibValue);
	}

	if (GE_AXIALCHANN==sChannInfo._iChannType)
	{
		fValue=arefVibValue._fPhaseThree;

		if (lbNeedComputeEigenValue)
		{
			fValue = this->ComputeThreePhaseCharValue(arefVibValue);
		}
	}

	int iRtn=0;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fValue<sChannInfo._chAlarmParam._fAlarmLH)
		{
			if (fValue<sChannInfo._chAlarmParam._fAlarmLL)
			{
				iRtn=2;
			}
			else
			{	
				iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fValue>sChannInfo._chAlarmParam._fAlarmHL)||(fValue<sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fValue>sChannInfo._chAlarmParam._fAlarmHH)||(fValue<sChannInfo._chAlarmParam._fAlarmLL))
			{
				iRtn=2;
			}
			else
			{	 iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		if((fValue<sChannInfo._chAlarmParam._fAlarmHH)&&(fValue>sChannInfo._chAlarmParam._fAlarmLL))
		{
			if((fValue<sChannInfo._chAlarmParam._fAlarmHL)&&(fValue>sChannInfo._chAlarmParam._fAlarmLH))
			{	
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
		break;
	default:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
	}
	
	//2.报警确认状态判断，已确认报警取消报警状态
	if (this->IsAlarmConfirmed())
	{
		GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();

		if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
		{
			bool bRtn=JudgeRegAgainAlarmForConfirm();//没超过上次值的百分比默认15%则不再报警

			if (!bRtn)
			{
				this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 
			}

		}

		iRtn = 0;
	}

	return iRtn;
}

int CVibChannParamAlarm::JudgeRegDataAlarm(IDBInterFace *pDB_,bool bConfirmAlarm_ /* = true */)
{
	//1.停车状态判断，停车设备取消报警状态
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL != pAlarmPlant)
	{
		bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pAlarmPlant,lbRun))
		{
			return 0; //停车不再进行报警
		}
	}

	S_ChannInfo sChannInfo=_channInfo;

    float fValue=_pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

	BOOL lbNeedComputeEigenValue = this->IsNeedComputeEigenValue( _pCharValueBuf[_iCurrCharValueIndex]);

	if (lbNeedComputeEigenValue)
	{
		fValue = this->ComputeAllCharValue( _pCharValueBuf[_iCurrCharValueIndex]);
	}

	if (GE_AXIALCHANN==sChannInfo._iChannType)
	{
		fValue=_pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;

		if (lbNeedComputeEigenValue)
		{
			fValue = this->ComputeThreePhaseCharValue(_pCharValueBuf[_iCurrCharValueIndex]);
		}
	}

	//新的轴位移报警逻辑
	if (g_UseNewAxisAlarmLogic&&g_UseNewAlarmLogic)
	{
		if (GE_AXIALCHANN == this->_channInfo._iChannType)
		{
			bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
			if (IsPlantRunning(*pAlarmPlant,lbRun))
			{
				float lfOriginalAxisValue = this->AxisOriginal();

				if (IsFloatZero(G_F_INIT_VALUE -lfOriginalAxisValue))
				{
					this->AxisOriginal(0);

					this->SaveAxisValueForNewAxisLogic();
				}

				fValue= abs(fValue - this->AxisOriginal());
			}else
			{
				fValue = 0;
			}
		}			 
	}

	int iRtn=0;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fValue<sChannInfo._chAlarmParam._fAlarmLH)
		{
			if (fValue<sChannInfo._chAlarmParam._fAlarmLL)
			{
				iRtn=2;
			}
			else
			{	
				iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fValue>sChannInfo._chAlarmParam._fAlarmHL)||(fValue<sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fValue>sChannInfo._chAlarmParam._fAlarmHH)||(fValue<sChannInfo._chAlarmParam._fAlarmLL))
			{
				 iRtn=2;
			}
			 else
			{	 iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		if((fValue<sChannInfo._chAlarmParam._fAlarmHH)&&(fValue>sChannInfo._chAlarmParam._fAlarmLL))
		{
			if((fValue<sChannInfo._chAlarmParam._fAlarmHL)&&(fValue>sChannInfo._chAlarmParam._fAlarmLH))
			{	
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
		break;
	default:
		if (fValue>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fValue>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
	}

	//2.报警确认状态判断，已确认报警取消报警状态
	if(true)
	{
		if (this->IsAlarmConfirmed())
		{
			GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();

			if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
			{
				bool bRtn=JudgeRegAgainAlarmForConfirm();//没超过上次值的百分比默认15%则不再报警

				if (!bRtn)
				{
					this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 
				}

			}

			iRtn = 0;
		}
	}
	

	return iRtn;
}

int CVibChannParamAlarm::GetCurrCharValueIndex()
{
	return _iCurrCharValueIndex;
}

int CVibChannParamAlarm::GetCurrWaveIndex()
{
	return _iCurrWavePIndex;
}

string CVibChannParamAlarm::GetSpecIndexTime(int index_)
{
	string sRtn;
	sRtn=TimeConvertString(_ptimeBuf[index_]);
	return sRtn;
}

int CVibChannParamAlarm::GetSlowAlarmStatus()
{
	return _iSlowStatus;
}

int CVibChannParamAlarm::GetRegAlarmStatus()
{
	if(this->m_nRegStatus > 0)
	{
		return this->m_nRegStatus;
	}
	if(this->_iFastStatus > 0)
	{
		return this->_iFastStatus;
	}
	if(this->_iSlowStatus > 0)
	{
		return this->_iSlowStatus;
	}
	return this->m_nRegStatus;
}

int CVibChannParamAlarm::GetCustomAlarmStatus()
{
	return _iCustomStatus;
}

int CVibChannParamAlarm::GetFastAlarmStatus()
{
	return _iFastStatus;
}


int CVibChannParamAlarm::LearnSlowThreshold(int iLearnDataLen_,S_VibCharValue *pVibValue_)
{
     int iRtn=-1;
	 int nloop=0;
	 double **pdData=new double*[D_BHKD_ALARM_CHARTYPE_NUM];
	 for (nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
	   pdData[nloop]=new double[iLearnDataLen_];
	 S_VibCharValue vibValue;
     for (nloop=0;nloop<iLearnDataLen_;++nloop)
     {
		 vibValue=pVibValue_[nloop];
		 pdData[E_TH_ALLCHAR][nloop]=vibValue._fCharAll;
		 pdData[E_TH_ONECHAR][nloop]=vibValue._fCharOne;
		 pdData[E_TH_TWOCHAR][nloop]=vibValue._fCharTwo;
		 pdData[E_TH_THREECHAR][nloop]=vibValue._fCharThree;
		 pdData[E_TH_HALFCHAR][nloop]=vibValue._fCharHalf;
		 pdData[E_TH_RESCHAR][nloop]=vibValue._fCharRes;
		 pdData[E_TH_ONEPHASE][nloop]=vibValue._fPhaseOne;
		 pdData[E_TH_TWOPHASE][nloop]=vibValue._fPhaseTwo;
		 pdData[E_TH_THREEPHASE][nloop]=vibValue._fPhaseThree;
		 pdData[E_TH_HALFPHASE][nloop]=vibValue._fPhaseHalf;
     }
	 CAnalizeDiagnose_MW diagn;
	 S_phaseThreshold__CalcInput phaseCalcInput;
	 S_charThreshold__CalcInput calcInput;
     S_phaseThreshold phaseTh;
	 S_charThreshold charTh;
	 int iErrorCode=-1;
	 for (nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
	 {
		 iRtn = 1;
		 int iPhaseRtn = 1;//相位自学习标示
		 if ((nloop==E_TH_ONEPHASE)||(nloop==E_TH_TWOPHASE)||(nloop==E_TH_THREEPHASE)||(nloop==E_TH_HALFPHASE))
		 {
			 phaseCalcInput.f_hope=_channInfo._chAlarmParam._pThLearnParamS[nloop]._fParam1;
			 phaseCalcInput.f_zoomCorfficient=_channInfo._chAlarmParam._pThLearnParamS[nloop]._fParam2;
			 phaseCalcInput.i_length=iLearnDataLen_;
			 phaseCalcInput.pPhase=pdData[nloop];
			 if (diagn.calcPhaseSlowChange_threshold(phaseCalcInput,phaseTh))
			 {	//相位缓变门限计算
			  // CHZLogManage::Info("phase","CVibChannParamAlarm","LearnSlowThreshold");
				 _pthSlow[nloop]._bThreshhold1=phaseTh.b_InOneFour;
				 _pthSlow[nloop]._fThreshold1=phaseTh.d_phaseThreshold_up;
				 _pthSlow[nloop]._fThreshold2=phaseTh.d_phaseThreshold_down;
			 }
			 else
			 {
				 iErrorCode=phaseTh.errorCode;
				 iPhaseRtn=-1;
			 }
		 }
		 else
		 {
			 calcInput.pChar=pdData[nloop];
			 calcInput.i_length=iLearnDataLen_;
			 calcInput.f_hope=_channInfo._chAlarmParam._pThLearnParamS[nloop]._fParam1;
			 calcInput.f_zoomCorfficient=_channInfo._chAlarmParam._pThLearnParamS[nloop]._fParam2;
             if(diagn.calcCharSlowChange_threshold(calcInput,charTh))
			 {	//幅值缓变门限计算
			  // CHZLogManage::Info("charvalue","CVibChannParamAlarm","LearnSlowThreshold");
				 _pthSlow[nloop]._fThreshold1=charTh.d_charThreshold_up;
				 _pthSlow[nloop]._fThreshold2=charTh.d_charThreshold_down;
			 }
			 else
			 {
				 iErrorCode=phaseTh.errorCode;
				 iRtn=-1;
			 }
		 }
		 //CString strTmp;strTmp.Format("缓变报警自学习标示:%d,%d",iRtn,iPhaseRtn);
		 //CHZLogManage::Debug(strTmp,"CVibChannParamAlarm","LearnSlowThreshold");
		 if(iRtn<0 && iPhaseRtn<0) //当相位和幅值自学习门限均为成功时，返回iRtn = -1
			 goto QUIT_LEARNAMPSLOWTHRESHOLD;
	 }
     iRtn=1;
QUIT_LEARNAMPSLOWTHRESHOLD:
	 for (nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
		 delete [] pdData[nloop];
	 delete [] pdData;
	 return iRtn;
}

CThreshold CVibChannParamAlarm::ModifyVibThS(CThreshold th_,bool bPhase_)
{
	float fCoef=1.3;
	if (bPhase_)
	{
		th_._fThreshold1=(th_._fThreshold1)*fCoef;
		th_._fThreshold2=(th_._fThreshold2)*fCoef;
		th_._fThreshold3=(th_._fThreshold3)*fCoef;
	}
	else
	{
		th_._fThreshold1=(th_._fThreshold1)*fCoef;
		th_._fThreshold2=(th_._fThreshold2)*fCoef;
		th_._fThreshold3=(th_._fThreshold3)*fCoef;
	}
	return th_;
}

///---------------------------------//
CProcChannParamAlarm::CProcChannParamAlarm()
{
	_iCurrCharValueIndex=0;
	CBHDateTime timeCurr=CBHDateTime::GetCurrentTime();
	_timeCurr=timeCurr;
	_timeSaveDataS=timeCurr;
	_timeFirstSaveS=timeCurr;

	_timeFirstSaveReg=timeCurr;
	_timeSaveDataReg=timeCurr;

	_timeThLearnS=timeCurr;
	_iSlowStatus=0;
	_bFirstAlarmS=false;
	_bAlarmStatusS=false;
	_bLearnTh=false;
    _iRegStatus=0;
	_siAlarmCountsS=0;
	_siCountsS=0;
	_iEventIDReg=-1;
	this->m_fRegValueLast = 0;
	this->m_fLastAlarmValue = .0;
	m_nFastRegStatus = 0;

	this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
	this->InnerAlarmStatus(CHANN_ALARM_STATUS_INIT);
	this->m_oHalfNormalTimer5 = CBHDateTime::GetCurrentTime();
	this->ResetContinuousMoreThanTimes();
	this->PlantNewAlarmOccoured(FALSE);
	this->LastMaxAlarmValue(0);
	this->NewAlarmValue(G_F_INIT_VALUE);
	m_nProcSUDStatus = 0;

	this->m_nAlarmLevel = ALARM_NORMAL;
}

CProcChannParamAlarm::~CProcChannParamAlarm()
{

}

int CProcChannParamAlarm::JudgeAlarm(IDBInterFace * pDb_,bool bSaveData_,bool bConfirmAlarm_ )
{
	int iRtn=0;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pAlarmPlant->_timeCurr;
/*	if (!_bLearnTh)
	{
       _iThIDSlow=LearnAddSlowThreshold(E_TRIG_TH_RESTART);
	   _bLearnTh=true;
	}*/
   	iRtn=JudgeRegAlarm(pDb_,bSaveData_,bConfirmAlarm_);
  //	iRtn|=JudgeSlowAlarm();
	return iRtn;
}

/************************************************************************/
/* 过程量测点开始                                                       */
/************************************************************************/

BOOL CProcChannParamAlarm::InitValues()
{
	if (NULL == this->_pLock)
	{
		return FALSE;
	}
	this->_pLock->BeginWrite();
	BEGIN_ERROR_HANDLE;
	
	BOOL lbRet = TRUE;
	float lfInitValue = .0;
	switch(this->_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_HIGHPASS:
		lfInitValue =  (this->_channInfo._chAlarmParam._fAlarmLH);		
		break;
	case E_ALARMCHECK_BANDPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_BANDSTOP:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmLL);
		break;
	}

	this->LastMaxAlarmValue(lfInitValue);
	this->LastAlarmValueEx(lfInitValue);
	this->LastEverageAlarmValue(G_F_INIT_VALUE);
	this->m_fLastAlarmValue = G_F_INIT_VALUE;
	this->JustStarted(TRUE);
	END_ERROR_HANDLE;
	this->_pLock->EndWrite();
	return TRUE;
}
int CProcChannParamAlarm::JudgeSlowAlarm(IDBInterFace * pDb_)
{
	COleDateTimeSpan span;
	int iAlarmNum=0;
	int iStatus=0;

	if (!_channInfo._chAlarmParam._bCharSlow)
	{
		if (_bAlarmStatusS)
			HandleSlowAlarmStop(pDb_);
	}
	else
	{
		if(_iSlowStatus==0)
		{//不处于报警状态，判断门限学习
			span=_timeCurr-_timeThLearnS;
			if (span.GetTotalHours()>D_ALARM_STH_LEARN_INTV)
			{
				_iThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_TIME);
				_timeThLearnS=_timeCurr;
			}
		}
		if (_bAlarmStatusS)
			SlowAlarmStatusOP(pDb_);
		else
		{
			iStatus=CheckSlowDataAlarm();
			if (_bFirstAlarmS)
			{
				if (iStatus>0)
					_siAlarmCountsS++;
				_siCountsS++;
				if (!(_siCountsS<D_ALARM_COUNTS))
				{
					if (!(_siAlarmCountsS < D_ALARM_ALARM_COUNTS))
					{
						_bAlarmStatusS=true;
						iStatus|=D_BHKD_AT_SLOW;
						iStatus|=_iAlarmBit;
						_iSlowStatus|=iStatus;
						AddSlowAlarmLog(pDb_);
					}
					else
					{
						_bFirstAlarmS=false;
						_siAlarmCountsS=0;
						_siCountsS=0;
					}
				}
			}
			else
			{
				if (iStatus>0)
				{
					_bFirstAlarmS=true;
					_siCountsS++;
					_siAlarmCountsS++;
				}
			}
		}
	}
	return iStatus;
}

int CProcChannParamAlarm::JudgeRegAlarm(IDBInterFace *pDb_,bool bSaveData_ ,bool bConfirmAlarm_ )
{
	int iRtn=0;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;

	int lnRegStatus = 0;
	bool lbJudgeAlarm = false;

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();
	if (loTimeSpan.GetTotalHours() < GetAlarmOverridTime())
	{
		lbJudgeAlarm = true;
	}

	if (_channInfo._chAlarmParam._bRegular)
	{
		iRtn=JudgeRegDataAlarm(bConfirmAlarm_);//判断是否报警，并返回具体值，1表示低报，2表示高报，0表示正常
		if(iRtn>0)
		{
			
			//CHZLogManage::Info("1","CProcChannParamAlarm","JudgeRegAlarm");
			lnRegStatus=iRtn|D_BHKD_AT_REGULAR|_iAlarmBit;
			if(1==iRtn)
				lnRegStatus|=D_BHKD_AT_REGLOW;
			else
				lnRegStatus|=D_BHKD_AT_REGHIGH;

			m_nFastRegStatus = lnRegStatus;

			if(lbJudgeAlarm)
			{
				this->m_fLastAlarmValue = _pfValueBuf[_iCurrCharValueIndex];

				_iRegStatus = lnRegStatus;
			}

			
			//判断是否反复报警
			bool lbRet = JudgeRegAgainAlarm();


			if(_iEventIDReg<0)//添加报警日志
			{
				_timeSaveDataReg=_timeCurr;

				this->_timeFirstSaveReg = this->_timeCurr;

				_iEventIDReg=pPlant->AddProcRegAlarmLog(pDb_,_channInfo._cChannNo,pPlant->_timeProcDataUpdate
														,_iRegStatus,_channInfo._iChannType);
				_timeFirstSaveReg = _timeCurr;
			}
			if(_iEventIDReg>0)//保存报警数据
			{
				COleDateTimeSpan dtSpan;
				//判断是否超过了连续保存数据时间段
				dtSpan=_timeCurr-_timeFirstSaveReg;
				int iTotalMins=dtSpan.GetTotalMinutes();
				if (iTotalMins>D_ALARM_CONTSAVETIME)
				{
					if(iTotalMins>D_ALARM_INTVSAVETIME)
						goto CLOSE_JUDGEREGALARM;
					dtSpan=_timeCurr-_timeSaveDataReg;
					if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
						goto CLOSE_JUDGEREGALARM;
				}
				//保存常规报警数据
				if(bSaveData_)
				{
					CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
					string sChannNo;
					SaveData(pDb_,_iEventIDReg,_iCurrCharValueIndex,_iRegStatus,sChannNo,dtTime);
					_timeSaveDataReg=_timeCurr;
				}
				
			}
		}
		else
		{
			_iRegStatus=0;
			if(_iEventIDReg>0)
			{
				pPlant->SetAlarmLogEndTimeAndNewThID(pDb_,_channInfo._cChannNo,_iEventIDReg,_iEventIDReg,_timeCurr,_channInfo._iChannType,-1);
				_iEventIDReg=-1;
			}
		}
	}
	else
	{
		_iRegStatus=0;
		if(_iEventIDReg>0)
		{
			pPlant->SetAlarmLogEndTimeAndNewThID(pDb_,_channInfo._cChannNo,_iEventIDReg,_iEventIDReg,_timeCurr,_channInfo._iChannType,-1);
			_iEventIDReg=-1;
		}
	}
CLOSE_JUDGEREGALARM:
  return lnRegStatus;
}

int CProcChannParamAlarm::SlowAlarmStatusOP(IDBInterFace * pDb_)
{
	int iRtn=-1;
//	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	COleDateTimeSpan timeSpan;
	if (JudgeNewAlarm())
	{//新的报警产生
		_timeFirstSaveS=_timeCurr;
		CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
		string sChannNo;
		SaveData(pDb_,_iEventIDSlow,_iCurrCharValueIndex,_iSlowStatus,sChannNo,dtTime);
		iRtn=1;
	}
	else
	{
		timeSpan=_timeCurr-_timeFirstSaveS;
		int iTotalMin=timeSpan.GetTotalMinutes();
		if (iTotalMin>D_ALARM_CONTSAVETIME)
		{//连续保存报警数据期间
			if (iTotalMin>D_ALARM_INTVSAVETIME)//超过报警数据保存时间，结束报警
				HandleSlowAlarmStop(pDb_);
			else
			{//处于连续报警保存期间的操作
				timeSpan=_timeCurr-_timeSaveDataS;
				if (timeSpan.GetTotalSeconds()>D_ALARM_INTVSAVETIME_INTV)
				{
					CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
					string sChannNo;
					SaveData(pDb_,_iEventIDSlow,_iCurrCharValueIndex,_iSlowStatus,sChannNo,dtTime);
				}
			}
		}
		else//没有超过连续保存时间
		{
			CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
			string sChannNo;
			SaveData(pDb_,_iEventIDSlow,_iCurrCharValueIndex,_iSlowStatus,sChannNo,dtTime);
		}
		iRtn=2;
	}
	return iRtn;
}

bool CProcChannParamAlarm::JudgeNewAlarm()
{
	float fValue=_pfValueBuf[_iCurrCharValueIndex];
	float fValueLast;
    bool	bHaveNew=false;
	if (_iCurrCharValueIndex==0)
		fValueLast=_pfValueBuf[D_ALARM_SAVECHAR_NUM-1];
	else
		fValueLast=_pfValueBuf[_iCurrCharValueIndex-1];
	int iStatue=0;
    iStatue=CheckSlowDataAlarm();
	if (iStatue>0)
	{
		if(fValueLast!=0)
		{
			if((fValue/fValueLast)>D_FLUCTUATE_RANGE)
				bHaveNew=true;
		}
		else
		{
			if (fValue>D_FLUCTUATE_RANGE)
				bHaveNew=true;
		}
	}
	return bHaveNew;
}

//[TBC]
bool CProcChannParamAlarm::SaveData(IDBInterFace * pDB_,int iEventID_,int iValueIndex_,int iAlarmStatus_,string& sChannNo_,CBHDateTime& tSaveTime_)
{
	bool bRtn=false;
	float fRev=0;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
    IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	fRev=pPlant->GetSpecTypeRealRev(_channInfo._iRevType,false);

	//用当前最新更新的时间保存记录，避免同一设备分发送助手时，保存数据时间出错的问题
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	pPlant->_pLockDataUpdateStatues->BeginRead();
	dt =pPlant->_timeProcDataUpdate;
	pPlant->_pLockDataUpdateStatues->EndRead();
	string sTime=CPlantInfoBase::TimeConvertString(dt);

	tSaveTime_ = dt;
	sChannNo_ = _channInfo._cChannNo;

	int iMilliSecond = dt.GetMillisecond();
	//[TBC]
	pDB->Cmd("INSERT INTO [%s]",pPlant->_sAlarmProcDataTblName.c_str());
	pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cChannNo,gc_cChannType,gc_cProcValue,gc_cRev,gc_cTime,gc_cType,gc_cMicroSecond);
	pDB->Cmd(" VALUES(%d,0,'%s',%d,%.2f,%f,'%s',%d,%d)",iEventID_,_channInfo._cChannNo,_channInfo._iChannType,_pfValueBuf[iValueIndex_],fRev,sTime.c_str(),iAlarmStatus_,iMilliSecond);
	//pDB->Cmd(" VALUES(%d,0,'%s',%d,%.2f,%f,'%s',%d)",iEventID_,_channInfo._cChannNo,_channInfo._iChannType,_pfValueBuf[iValueIndex_],fRev,pPlant->TimeConvertString(_ptimeBuf[iValueIndex_]),iAlarmStatus_);
	if(pDB->Exec())
		bRtn=true;
	else
	{
		CString strInfo;strInfo.Format("%s-%s 保存过程报警数据失败！",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
		CHZLogManage::Error(strInfo,"CProcChannParamAlarm","SaveData");
	}
	return bRtn;
}

int CProcChannParamAlarm::AddSlowAlarmLog(IDBInterFace * pDb_)
{
   int iRtn=-1;
   _timeFirstSaveS=_timeCurr;
   CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
   IDBInterFace *pDB=pDb_;
   SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
   _iEventIDSlow=pAlarmPlant->_iEventID+1;
   _iIDSlow=pAlarmPlant->_iID+1;
   float fValue=.0;
   float fRev=0;
   int iChannType=_channInfo._iChannType;
   int iLocation=0;
   
   //[TBC]
   pDB->BeginTran();
   pDB->Cmd("INSERT INTO [%s]",pAlarmPlant->_sAlarmLogTblName.c_str());
   pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s) ",gc_cID,gc_cEventID,gc_cName,gc_cStart_T,gc_cType,gc_cChannType,gc_cThreshold_ID);
   pDB->Cmd(" VALUES(%d,%d,'%s','%s',%d,%d,%d) ",_iIDSlow,_iEventIDSlow,_channInfo._cChannNo,pAlarmPlant->_sTimeCurr.c_str(),_iSlowStatus,iChannType,_iThIDSlow);
   //添加报警日志
   if (pDB->Exec())
   {
	   pAlarmPlant->_iEventID=_iEventIDSlow;
	   pAlarmPlant->_iID=_iIDSlow;
	   iRtn=_iEventIDSlow;
	   iLocation=_iCurrCharValueIndex;
	   for(iRtn=0;iRtn<G_D_ALARM_SAVEWAVE_NUM;++iRtn)
	   {
		   CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
		   string sChannNo;
		   if(!SaveData(pDb_,_iEventIDSlow,iLocation,_iSlowStatus,sChannNo,dtTime))
		   {
			   iRtn=-2;
			   goto CLEAN_ADDSLOWALARMLOG;
		   }
		   ++iLocation;
		   if (!(iLocation<D_ALARM_SAVECHAR_NUM))
			   iLocation=0;
	   }
   }
CLEAN_ADDSLOWALARMLOG:
   if (iRtn>0)
	   pDB->CommitTran();
   else
	   pDB->RollbackTran();
   return iRtn;
}

int CProcChannParamAlarm::HandleSlowAlarmStop(IDBInterFace * pDb_)
{
	int iRtn=0;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDb_;
    SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	//报警结束，学习门限
	_iThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_ALARM);
	_timeThLearnS=_timeCurr;
    //更新当前测点和当前报警日志的一些变量信息
	pDB->Cmd("UPDATE [%s] ",pAlarmPlant->_sAlarmLogTblName.c_str());
	pDB->Cmd(" SET %s='%s',%s=1,%s=%d",gc_cEnd_T,pAlarmPlant->_sTimeCurr.c_str(),gc_cStop_Flag,gc_cThreshold_ID_New,_iThIDSlow);
	pDB->Cmd(" WHERE %s=%d ",gc_cEventID,_iEventIDSlow);
	if (pDB->Exec())
	{
		iRtn=1;
//设置一些变量为非报警状态
		_bAlarmStatusS=false;
		_iSlowStatus=0;
		_bFirstAlarmS=false;
		_siAlarmCountsS=0;
		_siCountsS=0;
	}
	return iRtn;
}

int CProcChannParamAlarm::LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iThID=pPlant->_iThID+1;
	int iLen=0;

	int iAlarmType=D_BHKD_AT_SLOW|_iAlarmBit;
	CThreshold th;
	float *pfLearnData=NULL;
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	///学习过程
	//读取学习数据

    //添加门限值到门限表
	pDB->Cmd("INSERT INTO [%s] ",pPlant->_sThTblName.c_str());
	pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s)",gc_cID,gc_cThreshold_ID,gc_cName,gc_cChannType,gc_cType,gc_cSynFlag,gc_cAlarm_Type);
	pDB->Cmd(" VALUES(%d,%d,'%s',%d,%d,0,%d) ",iThID,iThID,_channInfo._cChannNo,_channInfo._iChannType,iType_,iAlarmType);
	if (pDB->Exec())
	{
		pDB->Cmd("SELECT * from [%s] ",pPlant->_sThTblName.c_str());
		pDB->Cmd(" WHERE [%s]=%d ",gc_cThreshold_ID,iThID);
		if (pDB->More(TRUE))
		{
			pDB->SetBuffer(gc_cThreshold,(BYTE*)&(th),sizeof(CThreshold));
			pDB->SetFieldValue(gc_cThreshold_Len,1);
			pDB->SetBuffer(gc_cData,(BYTE*)pfLearnData,sizeof(float)*iLen);
			pDB->SetFieldValue(gc_cData_Len,iLen);
			pDB->SetBuffer(gc_cLearnParam,(BYTE*)&(_channInfo._chAlarmParam._pThLearnParamS[0]),sizeof(CThresholdLearnParam));
			pDB->SetFieldValue(gc_cLearnParam_Len,1);
			pDB->SetFieldValue(gc_cStart_T,timeStart);
			pDB->SetFieldValue(gc_cEnd_T,timeEnd);
			pPlant->_iThID=iThID;
			_timeThLearnS=_timeCurr;
		}
		else
		{
			iThID=-1;
			CString strInfo;
			strInfo.Format("%s-%s 学习缓变阈值失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
			CHZLogManage::Error(strInfo,"CProcChannParamAlarm","LearnAddSlowThreshold");
		}

	}
	else
		iThID=-1;
	if (pfLearnData!=NULL) delete [] pfLearnData;
	return iThID;
}
int CProcChannParamAlarm::JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,float fCharValue_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;

	if (NULL != pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun)) //停车处理
		{
			return 0;
		}
	}
	int iRtn=0;
	float fValue=fCharValue_;
	switch(_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fValue>_channInfo._chAlarmParam._fAlarmHL)
		{
			iRtn=1;
			if(fValue>_channInfo._chAlarmParam._fAlarmHH)
				iRtn=2;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fValue<_channInfo._chAlarmParam._fAlarmLH)
		{
			iRtn=1;
			if (fValue<_channInfo._chAlarmParam._fAlarmLL)
				iRtn=2;
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fValue>_channInfo._chAlarmParam._fAlarmHL)||(fValue<_channInfo._chAlarmParam._fAlarmLH))
		{
			iRtn=1;
			if((fValue>_channInfo._chAlarmParam._fAlarmHH)||(fValue<_channInfo._chAlarmParam._fAlarmLL))
				iRtn=2;
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
	default:
		if ((fValue<_channInfo._chAlarmParam._fAlarmHL)&&(fValue>_channInfo._chAlarmParam._fAlarmLH))
		{
			iRtn=1;
			if((fValue<_channInfo._chAlarmParam._fAlarmHH)&&(fValue>_channInfo._chAlarmParam._fAlarmLL))
				iRtn=2;
		}
	}

	if(iRtn > 0)
	{
		int  iRegStatus = iRtn|D_BHKD_AT_REGULAR|_iAlarmBit;
		if(1==iRtn)
			iRegStatus|=D_BHKD_AT_REGLOW;
		else
			iRegStatus|=D_BHKD_AT_REGHIGH;

		if(this->_iRegStatus <= 0)
		{
			pPlant->AddFastAlarmLog(pDB_,_channInfo._cChannNo,dtTime_,iRegStatus,_channInfo._iChannType);
		}
	}
	
	return iRtn;
}
int CProcChannParamAlarm::JudgeRegDataAlarm(bool bConfirmAlarm_ /* = true */)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;

	if (NULL != pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun)) //停车处理
		{
			return 0;
		}
	}
	int iRtn=0;
    float fValue=_pfValueBuf[_iCurrCharValueIndex];
	switch(_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fValue>_channInfo._chAlarmParam._fAlarmHL)
		{
			iRtn=1;
			if(fValue>_channInfo._chAlarmParam._fAlarmHH)
				iRtn=2;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fValue<_channInfo._chAlarmParam._fAlarmLH)
		{
			iRtn=1;
			if (fValue<_channInfo._chAlarmParam._fAlarmLL)
				iRtn=2;
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fValue>_channInfo._chAlarmParam._fAlarmHL)||(fValue<_channInfo._chAlarmParam._fAlarmLH))
		{
			if((fValue>_channInfo._chAlarmParam._fAlarmHH)||(fValue<_channInfo._chAlarmParam._fAlarmLL))
			{		
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
	default:
		if ((fValue<_channInfo._chAlarmParam._fAlarmHH)&&(fValue>_channInfo._chAlarmParam._fAlarmLL))
		{
			if((fValue<_channInfo._chAlarmParam._fAlarmHL)&&(fValue>_channInfo._chAlarmParam._fAlarmLH))
			{
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
	}

	//2.报警确认状态判断，已确认报警取消报警状态
	if(bConfirmAlarm_)
	{
		if (this->IsAlarmConfirmed())
		{
			GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();

			if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
			{			
				if (!this->JudgeRegAgainAlarmForConfirm())
				{
					this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 	
				}	
			}

			iRtn = 0;
		}
	}
	

	return iRtn;
}

int CProcChannParamAlarm::CheckSlowDataAlarm()
{
	int iRtn=0;
    if (_pfValueBuf[_iCurrCharValueIndex]>_thSlow._fThreshold1)
	 iRtn=_iAlarmBit;
	return iRtn;
}

void CProcChannParamAlarm::SetParentPoint(LPVOID pParent_)
{
	_pParent=pParent_;
	switch(_channInfo._iChannType)
	{
	case GE_SENSORTYPE_PRESSOR:
		_iAlarmBit=D_BHKD_AT_PRESS;
		break;
	case GE_SENSORTYPE_TEMP:
		_iAlarmBit=D_BHKD_AT_TEMP;
		break;
	case GE_SENSORTYPE_FLUX:
		_iAlarmBit=D_BHKD_AT_FLUX;
		break;
	default:
		_iAlarmBit=D_BHKD_AT_OTHERPROC;
	}
}

void CProcChannParamAlarm::SetData(float fValue_,CBHDateTime dataTime_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	++_iCurrCharValueIndex;
	if(!(_iCurrCharValueIndex<D_ALARM_SAVECHAR_NUM))
		_iCurrCharValueIndex=0;

    _pfValueBuf[_iCurrCharValueIndex]=fValue_;
	_ptimeBuf[_iCurrCharValueIndex]=dataTime_;
	_fValue=fValue_;
	m_dtUpdateData = dataTime_;

	{
		S_ChannInfo sChannInfo = GetChannInfo();

		CString sLog;
		sLog.Format("Chann=%s,%s Value=%f, Time=%s",
			sChannInfo._cChannID, sChannInfo._cChannNo,
			fValue_, dataTime_.Format());
	}

}

void CProcChannParamAlarm::SetData(int index, float fValue_,CBHDateTime dataTime_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	++_iCurrCharValueIndex;
	if(!(_iCurrCharValueIndex<D_ALARM_SAVECHAR_NUM))
		_iCurrCharValueIndex=0;

    _pfValueBuf[_iCurrCharValueIndex] = fValue_;
	_ptimeBuf[_iCurrCharValueIndex]   = dataTime_;
	_fValue=fValue_;
}

void CProcChannParamAlarm::SetSUDStatus(int anSUDStatus)
{
	m_nProcSUDStatus=anSUDStatus;
}

float CProcChannParamAlarm::GetSUDStatus()
{
	return m_nProcSUDStatus;
}

unsigned int CProcChannParamAlarm::GetAlarmStatus(S_AlarmState *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iRegState=_iRegStatus;
		pAlarmState_->_iFastState=0;
		pAlarmState_->_iTrendState=0;
	}
	return _iSlowStatus|_iRegStatus;
}

unsigned int CProcChannParamAlarm::GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iRegState=_iRegStatus;
		pAlarmState_->_iFastState=0;
		pAlarmState_->_iTrendState=0;
		pAlarmState_->_fLastAlarmVal=m_fLastAlarmValue;
		pAlarmState_->_iTrendState = m_nAlarmLevel;
	}
	return _iSlowStatus|_iRegStatus;
}


void CProcChannParamAlarm::SetAlarmStatus(S_AlarmState& sAlarmState_)
{
//	_iFastStatus=sAlarmState_._iFastState;
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
//	_iTrendStatus=sAlarmState_._iTrendState;
}

void CProcChannParamAlarm::SetAlarmStatus( S_AlarmStateWithLevel& sAlarmState_ )
{
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iAlarmLevel;
	
}
void CProcChannParamAlarm::SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_)
{
	//	_iFastStatus=sAlarmState_._iFastState;
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	//	_iTrendStatus=sAlarmState_._iTrendState;
	//_iTrendStatus =  0;
	m_fLastAlarmValue=sAlarmState_._fLastAlarmVal;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iTrendState;
}

CString CProcChannParamAlarm::TimeConvertString(CBHDateTime time_)
{
	CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;
}

bool CProcChannParamAlarm::JudgeRegResetPlantAlarmTimer( float fCurrValue_,float fValueLast_ )
{
	return CChannInfoBase::JudgeRegResetPlantAlarmTimer(fCurrValue_,fValueLast_);
}

bool CProcChannParamAlarm::JudgeRegAgainAlarmForConfirm()
{
	float fValue=_pfValueBuf[_iCurrCharValueIndex];
	bool bRtn = false;
	if (g_UseNewAlarmLogic)
	{
		bRtn=JudgeRegResetPlantAlarmTimer(fValue,this->LastMaxAlarmValue());
	}else
	{
		bRtn=JudgeRegResetPlantAlarmTimer(fValue,this->m_fLastAlarmValue);
	}
	return !bRtn;
}

bool CProcChannParamAlarm::JudgeRegAgainAlarm()
{
	CAlarmPlantInfo * lpPlant = (CAlarmPlantInfo *)this->_pParent;
	float fCurrValue_=_pfValueBuf[_iCurrCharValueIndex];
	float fValueLast_ = this->m_fLastAlarmValue;

	if (NULL!= lpPlant)
	{
		bool lbRun = lpPlant->GetVibJudgeRunStatus()||lpPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*lpPlant,lbRun))
		{
			return 0;
		}
	}

	bool bRtn=false;
	S_ChannInfo sChannInfo=_channInfo;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fCurrValue_<=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fCurrValue_>=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_BANDPASS:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_<=fValueLast_)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_>=fValueLast_)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	default:
		if (fCurrValue_<=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	}
	return bRtn;
}

bool CProcChannParamAlarm::JudgeRegAgainAlarm( float afNewValue,float afOldvalue )
{
	CAlarmPlantInfo * lpPlant = (CAlarmPlantInfo *)this->_pParent;
	float lfNewValue= afNewValue;
	float lfOldValue = afOldvalue;

	if (NULL!= lpPlant)
	{
		bool lbRun = lpPlant->GetVibJudgeRunStatus()||lpPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*lpPlant,lbRun))
		{
			return 0;
		}
	}

	bool bRtn=false;
	S_ChannInfo sChannInfo=_channInfo;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (lfNewValue<=lfOldValue)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (lfNewValue>=lfOldValue)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_BANDPASS:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			lfNewValue = abs(lfNewValue-lfbase);
			lfOldValue = abs(lfOldValue-lfbase);

			if (lfNewValue<=lfOldValue)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			lfNewValue = abs(lfNewValue-lfbase);
			lfOldValue = abs(lfOldValue-lfbase);

			if (lfNewValue>=lfOldValue)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	default:
		if (lfNewValue<=lfOldValue)
			bRtn=true;
		else
			bRtn=false;
		break;
	}
	return bRtn;
}

float CProcChannParamAlarm::LastAlarmValue() const
{
	return m_fLastAlarmValue;
}

void CProcChannParamAlarm::LastAlarmValue( float val )
{
	m_fLastAlarmValue = val;
}

void CProcChannParamAlarm::SaveCurrentAsLastAlarmValue()
{
	ASSERT(this->_iCurrCharValueIndex >=0 && this->_iCurrCharValueIndex < D_ALARM_SAVECHAR_NUM);

	this->LastAlarmValue(this->_pfValueBuf[this->_iCurrCharValueIndex]);
}

_CHANN_ALARM_STATUS_ CProcChannParamAlarm::PredictChannAlarm( IDBInterFace * pDb_, 
															 bool abPlantAlarmed,
															 bool abConfirm /*= TRUE*/ ,
															 bool abChangeStatus/*= TRUE*/)
{
	_CHANN_ALARM_STATUS_ lnChannStatus = CHANN_ALARM_STATUS_HALF_NORMAL;

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (!pPlant)
	{
		return lnChannStatus;
	}

	if (lnChannStatus>this->InnerAlarmStatus())
	{
		lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
	}

	_timeCurr=pPlant->_timeCurr;	

	if(_channInfo._chAlarmParam._bRegular)
	{

		//1.判断是否常规报警
		int iType=this->JudgeRegDataAlarm(abConfirm);

		float lfCurrValue=_pfValueBuf[_iCurrCharValueIndex];

		if (iType>0)
		{
			//2.获得上次报警值
			float lfLastAlarmValue = this->LastAlarmValueEx();
			
	
			//3.更新上次报警值
			this->LastAlarmValueEx(lfCurrValue);
			this->LastAlarmValue(lfCurrValue);

			//4.更新报警平均值，并视情况更新上次报警最大值
			this->updateEverageAlarmValue(lfCurrValue);

			int lnPreviousChannRegStatus = _iRegStatus;
			
			int lnChannAlarmStatus = 0;

			//5.更新测点报警状态
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnChannAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
			}
			else
			{	
				lnChannAlarmStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			}
			if (iType==2)
			{   
				lnChannAlarmStatus=lnChannAlarmStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	lnChannAlarmStatus=lnChannAlarmStatus|D_BHKD_AT_REGLOW;
			}

			m_nFastRegStatus = lnChannAlarmStatus;

			if (abChangeStatus)
			{
				this->_iRegStatus = lnChannAlarmStatus;
			}

			//6.至少是旧报警，且不大于上次报警值
			lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_LOW;

			//7.判断是否大于上次报警值
			bool lbIsAgainAlarm=JudgeRegAgainAlarm(lfCurrValue,lfLastAlarmValue);

			if (!lbIsAgainAlarm)
			{
				//7.1不是重复报警则更新上次最大报警值
				float lfLastMaxValue = this->LastMaxAlarmValue();
				if (!JudgeRegAgainAlarm(lfCurrValue,this->LastMaxAlarmValue()))
				{
					this->LastMaxAlarmValue(lfCurrValue);
				}

				//7.2至少是旧报警，且大于上次报警值，且连续三次
				this->IncreaseContinuousMoreThanTimes();
				if (this->ContinuousMoreThanTimes()>=g_alarm_larger_times)
				{
					lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_HIGH;
				}		


				//7.3判断是否新报警
				bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfLastMaxValue);

				if (CHANN_ALARM_STATUS_INIT == this->InnerAlarmStatus())
				{
					lbIsNewAlarm = TRUE;
				}

				if (this->JustStarted())
				{
					lbIsNewAlarm = TRUE;
					this->JustStarted(FALSE);
				}

				if (lbIsNewAlarm)
				{

					//7.4 新报警，且大于上次报警值
					lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
					this->NewAlarmValue(lfCurrValue);

				}else
				{
					//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
					if (lnPreviousChannRegStatus)
					{
						float lfPreviousNewAlarmValue = this->NewAlarmValue();
						bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
						if (lbIsNewAlarm)
						{
							if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
							{
								lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
							}
							this->NewAlarmValue(lfCurrValue);
						}
					}
				}



			}else
			{
				//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
				if (lnPreviousChannRegStatus)
				{
					float lfPreviousNewAlarmValue = this->NewAlarmValue();
					bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
					if (lbIsNewAlarm)
					{
						if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
						{
							lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
						}
						this->NewAlarmValue(lfCurrValue);
					}
				}
				this->ResetContinuousMoreThanTimes();
			}
		}
		else
		{
			this->ResetContinuousMoreThanTimes();
			_iRegStatus=0;			
		}
	}

	this->PredictedChannInnerAlarmStatus(lnChannStatus);

	return lnChannStatus;
}

float CProcChannParamAlarm::LastMaxAlarmValue() const
{
	long lnData = .0;
	InterlockedExchange(&lnData,this->m_fLastMaxAlarmValue);
	float lfData = int_as_float(lnData);
	return lfData;
}

void CProcChannParamAlarm::LastMaxAlarmValue( float val )
{
	if (IsFloatZero(G_F_INIT_VALUE - val))
	{
		ASSERT(FALSE);
		return;
	}
	long lnData = float_as_int(val);
	InterlockedExchange(&this->m_fLastMaxAlarmValue,lnData);	
}

float CProcChannParamAlarm::LastAlarmValueEx() const
{
	return m_fLastAlarmValueEx;
}

void CProcChannParamAlarm::LastAlarmValueEx( float val )
{
	m_fLastAlarmValueEx = val;
}

float CProcChannParamAlarm::updateEverageAlarmValue( float afLastAlarmValue )
{
	//1.更新均值
	float lfCurrentEverageValue = this->LastEverageAlarmValue();
	if (IsFloatZero(lfCurrentEverageValue - G_F_INIT_VALUE))
	{
		lfCurrentEverageValue = afLastAlarmValue;
	}
	float lfPreviousEverageValue = lfCurrentEverageValue;
	lfCurrentEverageValue = (lfCurrentEverageValue+afLastAlarmValue)/2;
	this->LastEverageAlarmValue(lfCurrentEverageValue);

	//2.判断是否需要更新最大报警值
	CAlarmPlantInfo * lpParent = (CAlarmPlantInfo *)this->_pParent;
	if (NULL != lpParent)
	{
		COleDateTimeSpan lpSpan = this->_timeCurr - lpParent->m_oLastNewAlarmTime;
		if (lpSpan.GetTotalDays()>= lpParent->m_nNewAlarmMaxTime)
		{
			this->LastMaxAlarmValue(lfPreviousEverageValue);
		}
	}

	return lfCurrentEverageValue;
}

void CProcChannParamAlarm::ResetHalfNormalTimer5()
{
	this->m_oHalfNormalTimer5 = this->_timeCurr;
}

_CHANN_ALARM_STATUS_ CProcChannParamAlarm::InnerAlarmStatus() const
{
	return m_nInnerAlarmStatus;
}

void CProcChannParamAlarm::InnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nInnerAlarmStatus = val;
}

int CProcChannParamAlarm::SwitchAlarmStatus( IDBInterFace * pDb_,
											 _CHANN_ALARM_STATUS_ anNewStatus ,
											 BOOL abSaveData/*=TRUE*/)
{
	BOOL lbNewAlarmLogAdded = FALSE;

	if (CHANN_ALARM_STATUS_NORMAL == anNewStatus)
	{	
		if(!IsFloatZero(this->LastEverageAlarmValue() - G_F_INIT_VALUE))
		{
			this->LastMaxAlarmValue(this->LastEverageAlarmValue());
		}
		if (this->InnerAlarmStatus()>anNewStatus)
		{
			this->HandleRegAlarmStop(pDb_);
		}
		this->ResetHalfNormalTimer5();
		this->InnerAlarmStatus(anNewStatus);
		this->ResetContinuousMoreThanTimes();


	}else if (CHANN_ALARM_STATUS_HALF_NORMAL == anNewStatus)
	{
		//this->LastMaxAlarmValue(this->LastEverageAlarmValue());
		//从报警到正常
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			this->ResetHalfNormalTimer5();
			this->InnerAlarmStatus(CHANN_ALARM_STATUS_HALF_NORMAL);
			this->ResetContinuousMoreThanTimes();
		}

		//之前是半正常状态
		else if (CHANN_ALARM_STATUS_HALF_NORMAL == this->InnerAlarmStatus())
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;			
			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

			if (NULL!=lpAlarmPlantInfo)
			{
				int lnHours = lpSpan.GetTotalHours();
				if (lnHours>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
					this->HandleRegAlarmStop(pDb_);
				}
			}
		}else
		{
			//ASSERT(FALSE);
		}
		//新状态是报警状态
	}else 
	{
		//旧状态是报警状态
		
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			if (CHANN_ALARM_STATUS_NEW_ALARM == anNewStatus)
			{
				this->HandleRegAlarmStop(pDb_);
				this->AddRegAlarmLog(pDb_);
				lbNewAlarmLogAdded = TRUE;		
			}else
			{
				if (this->PlantNewAlarmOccoured())
				{
					this->AddRegAlarmLog(pDb_);
					lbNewAlarmLogAdded = TRUE;	
				}
			}

			this->InnerAlarmStatus(anNewStatus);

			//旧状态是正常或者半正常状态
			//旧状态是半正常状态，新状态不是新报警状态
		}else if (this->InnerAlarmStatus()== CHANN_ALARM_STATUS_HALF_NORMAL 
			&& anNewStatus!= CHANN_ALARM_STATUS_NEW_ALARM)
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;			
			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

			if (NULL!=lpAlarmPlantInfo)
			{
				int lnHours = lpSpan.GetTotalHours();
				if (lnHours>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(anNewStatus);
					this->HandleRegAlarmStop(pDb_);
					lbNewAlarmLogAdded = TRUE;
				}else
				{
					this->InnerAlarmStatus(anNewStatus);
				}
			}

		}else//旧状态是正常状态
		{
			this->HandleRegAlarmStop(pDb_);
			this->AddRegAlarmLog(pDb_);
			this->InnerAlarmStatus(anNewStatus);
			lbNewAlarmLogAdded = TRUE;
		}

		if (lbNewAlarmLogAdded && abSaveData)
		{
			if(g_bIsDiskFull)
			{
                CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;
				if(NULL != lpAlarmPlantInfo)
				{
					if(!lpAlarmPlantInfo->DeleteProcTrendRecord(pDb_))
					{
						lpAlarmPlantInfo->DeleteProcAlarmRecord(pDb_,_iEventIDReg);
					}
				}
			}

			CBHDateTime dtTime = CBHDateTime::GetCurrentTimeEx();
			string sChannNo;
			this->SaveData(pDb_,_iEventIDReg,_iCurrCharValueIndex,_iRegStatus,sChannNo,dtTime);

			CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
			if(pPlant)
			{
				pPlant->SaveProcAlarmDetailDataToTrend(pDb_,sChannNo,dtTime);
			}

		}
	}

	return lbNewAlarmLogAdded;
}

int CProcChannParamAlarm::HandleRegAlarmStop( IDBInterFace * pDb_ )
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDb_,_channInfo._cChannNo,pAlarmPlant->_timeProcDataUpdate,_channInfo._iChannType);
	}else
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDb_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType);
	}
}

int CProcChannParamAlarm::AddRegAlarmLog( IDBInterFace * pDB_ )
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!=pAlarmPlant)
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeProcDataUpdate,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}
}

int CProcChannParamAlarm::HandleRegAlarmStopEx( IDBInterFace * pDb_ )
{
	return this->HandleRegAlarmStop(pDb_);
}

int CProcChannParamAlarm::ContinuousMoreThanTimes() const
{
	return m_nContinuousMoreThanTimes;
}

void CProcChannParamAlarm::ContinuousMoreThanTimes( int val )
{
	m_nContinuousMoreThanTimes = val;
}

void CProcChannParamAlarm::ResetContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes = 0;
}

void CProcChannParamAlarm::IncreaseContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes++;
}

BOOL CProcChannParamAlarm::PlantNewAlarmOccoured() const
{
	return m_bPlantNewAlarmOccoured;
}

void CProcChannParamAlarm::PlantNewAlarmOccoured( BOOL val )
{
	m_bPlantNewAlarmOccoured = val;
}

float CProcChannParamAlarm::LastEverageAlarmValue() const
{
	return m_fLastEverageAlarmValue;
}

void CProcChannParamAlarm::LastEverageAlarmValue( float val )
{
	m_fLastEverageAlarmValue = val;
}

_CHANN_ALARM_STATUS_ CProcChannParamAlarm::PredictedChannInnerAlarmStatus() const
{
	return m_nPredictedChannInnerAlarmStatus;
}

void CProcChannParamAlarm::PredictedChannInnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nPredictedChannInnerAlarmStatus = val;
}

int CProcChannParamAlarm::GetChannRegAlarmType()
{
	if(isLocalJudgeAlarm())
	{
		return this->_iRegStatus;
	}
	return m_nFastRegStatus;
}

E_ALARM_LEVEL CProcChannParamAlarm::GetAlarmLevel()
{
	return this->m_nAlarmLevel;
}

void CProcChannParamAlarm::SetAlarmLevel( const E_ALARM_LEVEL nAlarmLevel_ )
{
	this->m_nAlarmLevel = nAlarmLevel_;
}

void CProcChannParamAlarm::SetChannAlarmLevel( int eAlarmStatus_ )
{
	if(eAlarmStatus_ <= CHANN_ALARM_STATUS_HALF_NORMAL)
	{
		this->SetAlarmLevel(ALARM_NORMAL);
		return;
	}

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	float lfLastAlarmValue = this->LastAlarmValueEx();

	CString lstrUnit = lsChannInfo._cUnit;

	unsigned int iAlarmState = this->GetAlarmStatus();

	int lnStatus = ParseAlarmStatus(iAlarmState);

	switch(lsChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:

		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
			else
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue <= 1.2*lsChannInfo._chAlarmParam._fAlarmLH)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	default:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	}
}
_CHANN_ALARM_STATUS_ CVibChannParamAlarm::PredictedChannInnerAlarmStatus() const
{
	return m_nPredictedChannInnerAlarmStatus;
}

void CVibChannParamAlarm::PredictedChannInnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nPredictedChannInnerAlarmStatus = val;
}

float CVibChannParamAlarm::LastEverageAlarmValue() const
{
	float lfValue = 0;
	lfValue = m_fLastEverageAlarmValue;
	return lfValue;
}

void CVibChannParamAlarm::LastEverageAlarmValue( float val )
{
	m_fLastEverageAlarmValue = val;
}



//----------------------------------------//
CAxisAlarmChannParam::CAxisAlarmChannParam()
{
	_iCurrIndex=-1;
	_pParent=NULL;
	_iRevType=1;
	_bLearnTh=false;
	_iAxisRegStatus=0;
	_iSlowStatus =0;
	_iFastStatus =0;
	_iRegAlarmFlag=-1;
	_iEventIDReg=-1;
	CBHDateTime timeTmp=CBHDateTime::GetCurrentTime();
	_timeCurr=timeTmp;
	_timeSaveAxisData=timeTmp;
	_timeSaveAxisDataFirst=timeTmp;
	_iAlarmBit      = D_BHKD_AT_LOCATION;
	this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
}

CAxisAlarmChannParam::~CAxisAlarmChannParam()
{

}


int CAxisAlarmChannParam::JudgeFastAlarm()
{
    int iRtn=0;

	return iRtn;
}

int CAxisAlarmChannParam::JudgeSlowAlarm(IDBInterFace * pDb_)
{
	int iRtn=-1;
	COleDateTimeSpan span;
	int iAlarmNum=0;
	int iStatus=0;
	bool bTmp=false;
    CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
    if (!_channInfo._axisAlarmParam._bCheckS)
	{
		if (_bSlowAlarmStatus)
		{
			iRtn=HandleSlowsAlarmStop(pDb_);
			goto QUIT_JUDGESLOWALARM;
		}
	}
	if(_iSlowStatus==0)
	{//不处于报警状态，判断门限学习
		span=_timeCurr-_timeThLearnS;
		if (span.GetTotalHours()>D_ALARM_STH_LEARN_INTV)
		{
			_iCurrThIDSlow=LearnAddThresholdS(pDb_,E_TRIG_TH_TIME);
			if(_iCurrThIDSlow>-1)
			   _timeThLearnS=_timeCurr;
		}
	}
	if (_bSlowAlarmStatus)
		SlowAlarmStatusOP(pDb_);
	else
	{
		bTmp=CheckAxisDataStatusS();
		if (_bFirstSlowAlarm)
		{
			if (bTmp)
				++_iSlowAlarmCounts;
			++_iSlowCounts;
			if (_iSlowCounts>D_ALARM_COUNTS)
			{
				if (_iSlowAlarmCounts>D_ALARM_ALARM_COUNTS)
				{
					_bSlowAlarmStatus=true;
					_iSlowStatus=D_BHKD_AT_SLOW|D_BHKD_AT_LOCATION;
					FirstSlowAlarmOP(pDb_);
					goto QUIT_JUDGESLOWALARM;
				}
				else
				{
					_bFirstSlowAlarm=false;
					_iSlowAlarmCounts=0;
					_iSlowCounts=0;
				}
			}
		}
		else
		{
			if (bTmp)
			{
				_bFirstSlowAlarm=true;
				++_iSlowCounts;
				++_iSlowAlarmCounts;
			}
		}
	}
QUIT_JUDGESLOWALARM:
	return iRtn;
}

int CAxisAlarmChannParam::SlowAlarmStatusOP(IDBInterFace * pDb_)
{
	int iRtn=-1;
	COleDateTimeSpan timeSpan;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	int iSmpNum=0,iSmpFreq=0;
	if (CheckIfNewSlowAlarm())
	{//新的报警产生
		SaveData(pDb_,_iCurrIndex,pAlarmPlant->GetAlarmAxisTblName(),_iEventIDSlow);
		_timeFirstSaveDataS=_timeCurr;
		iRtn=1;
	}
	else
	{
		timeSpan=_timeCurr-_timeFirstSaveDataS;
		int iTotalMin=timeSpan.GetTotalMinutes();
		if (iTotalMin>D_ALARM_CONTSAVETIME)
		{//连续保存报警数据期间
			if (iTotalMin>D_ALARM_INTVSAVETIME)//超过报警数据保存时间，结束报警
				HandleSlowsAlarmStop(pDb_);
			else
			{//处于连续报警保存期间的操作
				timeSpan=_timeCurr-_timeLastSaveDataS;
				if (timeSpan.GetTotalSeconds()>D_ALARM_INTVSAVETIME_INTV)
					SaveData(pDb_,_iCurrIndex,pAlarmPlant->GetAlarmAxisTblName(),_iEventIDSlow);
			}
		}
		else//没有超过连续保存时间
			SaveData(pDb_,_iCurrIndex,pAlarmPlant->GetAlarmAxisTblName(),_iEventIDSlow);
		iRtn=2;
	}
	return iRtn;
}

int CAxisAlarmChannParam::RegAlarmStatusOP(IDBInterFace * pDb_)
{
	int iRtn=-1;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	COleDateTimeSpan dtSpan;
	dtSpan=_timeCurr-_timeSaveAxisDataFirst;
	int iTotalMins=dtSpan.GetTotalMinutes();
	if (iTotalMins>D_ALARM_CONTSAVETIME)
	{
		if(iTotalMins>D_ALARM_INTVSAVETIME)
		{
			return iRtn;
		}
		dtSpan=_timeCurr-_timeSaveAxisData;
		if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
			return iRtn;
	}
	iRtn=SaveData(pDb_,_iCurrIndex,pAlarmPlant->GetAlarmAxisTblName(),_iEventIDReg);
	if (iRtn>0)
	{
	    _timeSaveAxisData=_timeCurr;
		//CHZLogManage::Info("保存常规轴心位置报警成功","CAxisAlarmChannParam","RegAlarmStatusOP");
	}
	return iRtn;
}

bool CAxisAlarmChannParam::UpdateData()
{
   bool bRtn=false;
   
   _iCurrIndex++;
   
   if (!(_iCurrIndex<D_ALARM_SAVECHAR_NUM))
   {
	   _iCurrIndex=0;
   }
 
   S_AxisData *pAxisData=&(_pAxisData[_iCurrIndex]);

   CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

   ((CVibChannParamAlarm*)(pPlant->_vVibChann[_iHorIndex]))->GetCurrDcAndGap(pAxisData->_fHorDC,pAxisData->_fHorGap);
   ((CVibChannParamAlarm*)(pPlant->_vVibChann[_iVerIndex]))->GetCurrDcAndGap(pAxisData->_fVerDC,pAxisData->_fVerGap);

   _pAxisData[_iCurrIndex]._fRev=pPlant->GetSpecTypeRealRev(_iRevType,false);

   bRtn=_pAxisData[_iCurrIndex].CalcAxisLocation(_channInfo._iLocationType,
												 _channInfo._iRotateDirection,
												 _channInfo._fAxisDia,
												 _fSensitivityVer,
												 _fSensitivityHor);
   return bRtn;
}

bool CAxisAlarmChannParam::UpdateDynData()
{
	bool bRtn=false;
	int iSmpNum=0;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_iCurrIndex=((CDynChannParamAlarm*)(pPlant->_vDynChann[_iHorIndex]))->GetCurrCharValueIndex();
	//CString strInfo;strInfo.Format("_iHorIndex:%d,_iVerIndex:%d",_iHorIndex,_iVerIndex);BHTRACE_DEBUG(strInfo);
	_pDateTime[_iCurrIndex]=pPlant->_timeCurr;
    //计算轴心位置
	DATATYPE_WAVE *pWaveHor=NULL;
    DATATYPE_WAVE *pWaveVer=NULL;
	DATATYPE_WAVE *pHorAxesWave=NULL;
	DATATYPE_WAVE *pVerAxesWave=NULL;
    CString sChann=((CDynChannParamAlarm*)(pPlant->_vDynChann[_iHorIndex]))->GetChannInfo()._cChannID;
	iSmpNum=((CDynChannParamAlarm*)(pPlant->_vDynChann[_iHorIndex]))->GetCharValue(_iCurrIndex)._iSmpNum;
	//CString strInfo;strInfo.Format("_iCurrIndex:%d,_iHorIndex:%d,sChann:%s,iSmpNum:%d",_iCurrIndex,_iHorIndex,sChann,iSmpNum);BHTRACE_DEBUG(strInfo);
    if (iSmpNum>0)
    {
		pWaveHor=new DATATYPE_WAVE[iSmpNum];
		pWaveVer=new DATATYPE_WAVE[iSmpNum];
		pHorAxesWave=new DATATYPE_WAVE[iSmpNum];
		pVerAxesWave=new DATATYPE_WAVE[iSmpNum];
		memcpy(pWaveHor,((CDynChannParamAlarm*)(pPlant->_vDynChann[_iHorIndex]))->GetCurrWavePoint(),iSmpNum*sizeof(DATATYPE_WAVE));
		memcpy(pWaveVer,((CDynChannParamAlarm*)(pPlant->_vDynChann[_iVerIndex]))->GetCurrWavePoint(),iSmpNum*sizeof(DATATYPE_WAVE));

		float fVerGapV=.0,fHorGapV=.0,fValue=.0,fTemp=.0,fCoorX=.0,fCoorY=.0;
		fVerGapV=((CDynChannParamAlarm*)(pPlant->_vDynChann[_iVerIndex]))->GetChannInfo()._fGapVoltage;
		fHorGapV=((CDynChannParamAlarm*)(pPlant->_vDynChann[_iHorIndex]))->GetChannInfo()._fGapVoltage;
		_fSensitivityHor==0?8:_fSensitivityHor;
		_fSensitivityVer==0?8:_fSensitivityVer;
		for (int iRtn=0;iRtn<iSmpNum;iRtn++)
		{
			pWaveVer[iRtn]=pWaveVer[iRtn]+(fVerGapV/_fSensitivityHor)*1000*10.0;
			pWaveHor[iRtn]=pWaveHor[iRtn]+(fHorGapV/_fSensitivityVer)*1000*10.0;
		}
		for (int iRtn=0;iRtn<iSmpNum;iRtn++)
		{
			_pAxisData[_iCurrIndex]._fHorDC = pWaveHor[iRtn]*_fSensitivityHor/(1000*10.0);
			_pAxisData[_iCurrIndex]._fHorGap = fHorGapV;
			_pAxisData[_iCurrIndex]._fVerDC = pWaveVer[iRtn]*_fSensitivityVer/(1000*10.0);
			_pAxisData[_iCurrIndex]._fVerGap = fVerGapV;
			//CString strInfo;strInfo.Format("水平原始 %d,%d,%f,%f",pWaveHor[0],pWaveVer[0],_pAxisData[_iCurrIndex]._fHorDC,_pAxisData[_iCurrIndex]._fVerDC);BHTRACE_DEBUG(strInfo);
			//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynIndexAxisLocationData");
			_pAxisData[_iCurrIndex]._fRev=pPlant->GetSpecTypeRealRev(_iRevType,false);
			_pAxisData[_iCurrIndex].CalcAxisLocation(_channInfo._iLocationType,_channInfo._iRotateDirection,_channInfo._fAxisDia,_fSensitivityVer,_fSensitivityHor);
			pHorAxesWave[iRtn]=_pAxisData[_iCurrIndex]._fCoorX;
			pVerAxesWave[iRtn]=_pAxisData[_iCurrIndex]._fCoorY;
			//取距离圆心最大的点为最终计算结果
			fTemp=sqrt(pow(pHorAxesWave[iRtn],2)+pow(pVerAxesWave[iRtn],2));
			if (fValue<fTemp)
			{
				fValue=fTemp;
				fCoorX=pHorAxesWave[iRtn]/10.0;//数采发送波形的波形扩大10发的，因此计算完后应缩小10倍
				fCoorY=pVerAxesWave[iRtn]/10.0;
			}
			//CString strInfo;strInfo.Format("计算后输出的波形值 %d,%d",pHorAxesWave_[iRtn],pVerAxesWave_[iRtn]);
			//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynIndexAxisLocationData");
		}
		if (abs(fCoorX)>0 || abs(fCoorY)>0)
		{
			_pAxisData[_iCurrIndex]._fCoorX=fCoorX;
			_pAxisData[_iCurrIndex]._fCoorY=fCoorY;
			bRtn=true;
		}
		//strInfo.Format("_fCoorX:%f,_fCoorY:%f",_pAxisData[_iCurrIndex]._fCoorX,_pAxisData[_iCurrIndex]._fCoorY);BHTRACE_DEBUG(strInfo);
	}
	KD_DeletePoint(pWaveHor);
	KD_DeletePoint(pWaveVer);
	KD_DeletePoint(pHorAxesWave);
	KD_DeletePoint(pVerAxesWave);
	return bRtn;
}

int CAxisAlarmChannParam::JudgeAlarm(IDBInterFace * pDb_)
{
	int iRtn=-1;
	//得到最新的数据,并计算其轴心位置
    CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	_timeCurr=pPlant->_timeCurr;
	S_AxisData axisData=_pAxisData[_iCurrIndex];

	if (!_bLearnTh)
	{
		_bLearnTh=true;
		_iCurrThIDSlow=LearnAddThresholdS(pDb_,E_TRIG_TH_RESTART);
		_iCurrThIDFast=LearnAddThresholdF(pDb_,E_TRIG_TH_RESTART);
	}

	if(_channInfo._axisAlarmParam._bCheckS||_channInfo._axisAlarmParam._bCheckF)
	{//计算轴心位置
        axisData.CalcAxisLocation(_channInfo._iLocationType,_channInfo._iRotateDirection,_channInfo._fAxisDia,_fSensitivityVer,_fSensitivityHor);
		if ( NULL != _plockRW)
		{
			_plockRW->BeginWrite();
			_pAxisData[_iCurrIndex]=axisData;
			_plockRW->EndWrite();
		}
		else
			_pAxisData[_iCurrIndex]=axisData;
	}
	iRtn=JudgeSlowAlarm(pDb_);
	iRtn|=JudgeFastAlarm();
	return iRtn;
}

int CAxisAlarmChannParam::JudgeDynRegAlarmData()
{
	int iRtn=-1;
	//更新动态测点轴心位置数据
	if (!UpdateDynData())
	{
		return iRtn;
	}
	if (this->_iSlowAlarmCounts)
	{
	}
	float fHorRadius=.0,fVerRadius=.0,fXCircular=.0,fYCircular=.0,fCoorX=.0,fCoorY=.0,fDistance=.0;
	//获得水平轴半径
	fHorRadius=_channInfo._axisAlarmParam._thLearnParamS._fParam1;
	//获得垂直轴半径
	fVerRadius=_channInfo._axisAlarmParam._thLearnParamS._fParam2;
	//获得配置的圆心坐标
    fXCircular=_channInfo._axisAlarmParam._thLearnParamS._fParam3;
	fYCircular=_channInfo._axisAlarmParam._thLearnParamS._fParam4;
	//获得计算的轴心轨迹坐标
	fCoorX=_pAxisData[_iCurrIndex]._fCoorX;
	fCoorY=_pAxisData[_iCurrIndex]._fCoorY;
	//计算到轴心位置轨迹圆心的距离
    fDistance=sqrt(pow((fCoorX-fXCircular),2)+pow((fCoorY-fYCircular),2));
	//CString strInfo;strInfo.Format("判断:_fCoorX:%f,_fCoorY:%f,fHorRradius:%f,fVerRradius:%f",_pAxisData[_iCurrIndex]._fCoorX,_pAxisData[_iCurrIndex]._fCoorY,fHorRradius,fVerRradius);BHTRACE_DEBUG(strInfo);
	if (fDistance>fHorRadius || fDistance>fVerRadius)
	{
		iRtn=_iAlarmBit;
	}
	return iRtn;
}

int CAxisAlarmChannParam::JudgeDynRegAlarm(IDBInterFace * pDb_)
{
	int iRtn=-1;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	_timeCurr=pPlant->_timeCurr;
	//判断报警操作
	if(_channInfo._axisAlarmParam._bCheckS)
	{
		iRtn=JudgeDynRegAlarmData();

		if (NULL!=pPlant )
		{
			bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
			if (!IsPlantRunning(*pPlant,lbRun)) //停车处理
			{
				iRtn = -1;
			}

			//2.报警确认状态判断，已确认报警取消报警状态
// 			if (this->IsAlarmConfirmed())
// 			{
// 				GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();
// 
// 				if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
// 				{
// 
// 					this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 
// 				}
// 
// 				return 0;
// 			}
		}
		//CString strInfo;strInfo.Format("iRtn:%d",iRtn);BHTRACE_DEBUG(strInfo);
		if (iRtn>0)
		{
			_iAxisRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			_iAxisRegStatus=D_BHKD_AT_DYN|_iAxisRegStatus;
			if(_iRegAlarmFlag<0)
			{
				//添加报警日志
				if(AddAlarmLog(pDb_,_iAxisRegStatus,-1)<0)
				{
					CString strInfo;
					strInfo.Format("%s-%d 添加轴心位置常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._iLocationType);
					CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
				}
				_iRegAlarmFlag=1;
				_timeSaveAxisDataFirst=_timeCurr;
			}
			//保存数据操作
			RegAlarmStatusOP(pDb_);
			if (_iRegAlarmFlag==1)
				_iRegAlarmFlag=0;
		}
		else
		{
			if(_iAxisRegStatus>0)
		        HandleRegAlarmStop(pDb_);
		}
	}
	else
	{
		if(_iAxisRegStatus>0)
			HandleRegAlarmStop(pDb_);
	}
	return _iAxisRegStatus;
}

int CAxisAlarmChannParam::AddAlarmLog(IDBInterFace * pDB_,int iType_,int iThID_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	SwitchDbNameEx(pDB_,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iEventID=pPlant->_iEventID+1;
	_iEventIDReg=iEventID;
	int iID=iEventID;
	//int iID=pPlant->_iID+1;
	//[TBC]
	CString lstrTime = pPlant->_timeVibDataUpdate.Format();

	pDB_->Cmd("INSERT INTO [%s] ",pPlant->GetAlarmLogTblName().c_str());
	pDB_->Cmd("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",gc_cID,gc_cEventID,gc_cName,gc_cStart_T,gc_cEnd_T,gc_cStop_Flag,gc_cSynFlag,gc_cType,gc_cChannType,gc_cThreshold_ID,gc_cThreshold_ID_New);
	pDB_->Cmd(" VALUES(%d,%d,'%s','%s','%s',0,0,%d,%d,%d,0) ",iID,iEventID,_channInfo._cLocation,lstrTime,lstrTime,iType_,GE_AXISLOCATIONCHANN,iThID_);
	if (pDB_->Exec())
	{
		pPlant->_iID=iID;
		pPlant->_iEventID=iEventID;
	}
	else
		iEventID=-1;
	return iEventID;
}

int CAxisAlarmChannParam::HandleRegAlarmStop(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;

	SwitchDbNameEx(pDB_,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());

	CString lstrTime = pAlarmPlant->_timeVibDataUpdate.Format();

	int iRtn=-1;
	pDB_->Cmd("update [%s] ",pAlarmPlant->GetAlarmLogTblName().c_str());
	pDB_->Cmd(" set %s='%s',%s=%d,%s=1,%s=0 ",gc_cEnd_T,lstrTime,gc_cThreshold_ID,0,gc_cStop_Flag,gc_cSynFlag);
	pDB_->Cmd(" where %s=%d ",gc_cEventID,_iEventIDReg);
	//更新报警结束时间
	if (pDB_->Exec())
	{
		iRtn=_iEventIDReg;
		_iAxisRegStatus=0;
		_iRegAlarmFlag=-1;
	}
	else
		iRtn=-1;
	return iRtn;
}

int CAxisAlarmChannParam::HandleSlowsAlarmStop(IDBInterFace * pDb_)
{
	int iRtn=-1;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	SwitchDbNameEx(pDb_,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	_iCurrThIDSlow=LearnAddThresholdS(pDb_,E_TRIG_TH_ALARM);
	if (_iCurrThIDSlow<0)
	{
		return _iCurrThIDSlow;
	}
	CString lstrTime = pAlarmPlant->_timeVibDataUpdate.Format();

	pAlarmPlant->_iThID=_iCurrThIDSlow;
	//更新当前测点和当前报警日志的一些变量信息
	pDb_->Cmd("UPDATE [%s] ",pAlarmPlant->GetAlarmLogTblName().c_str());
	pDb_->Cmd(" SET %s='%s',%s=1,%s=%d",gc_cEnd_T,lstrTime,gc_cStop_Flag,gc_cThreshold_ID_New,_iCurrThIDSlow);
	pDb_->Cmd(" WHERE %s=%d ",gc_cEventID,_iEventIDSlow);
	if (pDb_->Exec())
	{
		iRtn=_iEventIDSlow;
		//设置一些变量为非报警状态
		_bSlowAlarmStatus=false;
		_iSlowStatus=0;
		_bFirstSlowAlarm=false;
		_iSlowAlarmCounts=0;
		_iSlowCounts=0;
	}
	else
		iRtn=-2;
	return iRtn;
}

int CAxisAlarmChannParam::HandleFastAlarmStop(IDBInterFace * pDB_)
{
	int iRtn=-1;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	SwitchDbNameEx(pDB_,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	int iEventID=pAlarmPlant->_iThID+1;
	iRtn=LearnAddThresholdF(pDB_,E_TRIG_TH_ALARM);
	if (iRtn<0)
	{
		return iRtn;
	}
	CString lstrTime = pAlarmPlant->_timeVibDataUpdate.Format();
	_iEventIDFast=iRtn;
	pAlarmPlant->_iThID=_iEventIDFast;
	//更新当前测点和当前报警日志的一些变量信息
	pDB_->Cmd("UPDATE [%s] ",pAlarmPlant->GetAlarmLogTblName().c_str());
	pDB_->Cmd(" SET %s='%s',%s=1,%s=%d",gc_cEnd_T,lstrTime,gc_cStop_Flag,gc_cThreshold_ID_New,_iCurrThIDFast);
	pDB_->Cmd(" WHERE %s=%d ",gc_cEventID,_iEventIDFast);
	if (pDB_->Exec())
	{
		iRtn=_iEventIDFast;
		//设置一些变量为非报警状态
		_iFastStatus=0;
	}
	else
		iRtn=-2;
	return iRtn;
}

bool CAxisAlarmChannParam::CheckAxisDataStatusF()
{
	bool bRtn=false;

	return bRtn;
}

bool CAxisAlarmChannParam::CheckAxisDataStatusS()
{
	bool bRtn=false;

	return bRtn;
}

bool CAxisAlarmChannParam::CheckIfNewSlowAlarm()
{
	bool bRtn=false;

	return bRtn;
}
//学习结束后,还需要把学习结果保存到阈值表,并把学习结果赋给当前快变阈值ID和阈值
int CAxisAlarmChannParam::LearnAddThresholdF(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iThID=pPlant->_iThID+1;
    int iLen=0;
	int iAlarmType=D_BHKD_AT_FAST|D_BHKD_AT_LOCATION;
	CThreshold th;
	float *pfLearnData=NULL;
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	///学习过程
    //读取学习数据


	//添加门限值到门限表
    pDB->Cmd("INSERT INTO [%s] ",pPlant->GetThTblName().c_str());
	pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s)",gc_cID,gc_cThreshold_ID,gc_cName,gc_cChannType,gc_cType,gc_cSynFlag,gc_cAlarm_Type);
    pDB->Cmd(" VALUES(%d,%d,'%s',%d,%d,0,%d) ",iThID,iThID,_channInfo._cLocation,GE_AXISLOCATIONCHANN,iType_,iAlarmType);
	if (pDB->Exec())
	{
		pDB->Cmd("SELECT * from [%s] ",pPlant->GetThTblName().c_str());
		pDB->Cmd(" WHERE [%s]=%d ",gc_cThreshold_ID,iThID);
		if (pDB->More(TRUE))
		{
			pDB->SetBuffer(gc_cThreshold,(BYTE*)&(th),sizeof(CThreshold));
			pDB->SetFieldValue(gc_cThreshold_Len,1);
			pDB->SetBuffer(gc_cData,(BYTE*)pfLearnData,sizeof(float)*iLen);
			pDB->SetFieldValue(gc_cData_Len,iLen);
			pDB->SetBuffer(gc_cLearnParam,(BYTE*)&(_channInfo._axisAlarmParam._thLearnParamF),sizeof(CThresholdLearnParam));
			pDB->SetFieldValue(gc_cLearnParam_Len,1);
			pDB->SetFieldValue(gc_cStart_T,timeStart);
			pDB->SetFieldValue(gc_cEnd_T,timeEnd);
			_timeThLearnF=_timeCurr;
		}
		else
			iThID=-1;
	}
	else
		iThID=-1;
    if (pfLearnData!=NULL) delete [] pfLearnData;
	return iThID;
}
//学习结束后,还需要把学习结果保存到阈值表,并把学习结果赋给当前缓变阈值ID和阈值
int CAxisAlarmChannParam::LearnAddThresholdS(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iLearnLen=100,iThID=0;
	int iAlarmType=D_BHKD_AT_SLOW|D_BHKD_AT_LOCATION;
	if (iLearnLen>D_ALARM_SAVECHAR_NUM)
		iLearnLen=D_ALARM_SAVECHAR_NUM;
	CThreshold th;
	S_AxisData *pAxisData=new S_AxisData[iLearnLen];
	int index=_iCurrIndex-iLearnLen+1;
    if (index>0)
		index=D_ALARM_SAVECHAR_NUM-index;
    else
		index=D_ALARM_SAVECHAR_NUM+index;
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	timeStart=_pDateTime[index];
	pAxisData[0]=_pAxisData[index];
	///学习过程
	//读取学习数据
    for (int nloop=1;nloop<iLearnLen;++nloop)
	{
		index++;
		if (!(index<D_ALARM_SAVECHAR_NUM))
		  index=0;
		pAxisData[nloop]=_pAxisData[index];
    }
	timeEnd=_pDateTime[index];
	iThID=LearnThresholdS(iLearnLen,pAxisData);
	pDB->BeginTran();
	pPlant->_pLockThID->BeginWrite();
	if (iThID<0)
	  goto QUIT_LEARNADDTHRESHOLDS;

	iThID=pPlant->_iThID+1;
	//添加门限值到门限表
	pDB->Cmd("INSERT INTO [%s] ",pPlant->GetThTblName().c_str());
	pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s)",gc_cID,gc_cThreshold_ID,gc_cName,gc_cChannType,gc_cType,gc_cSynFlag,gc_cAlarm_Type);
	pDB->Cmd(" VALUES(%d,%d,'%s',%d,%d,0,%d) ",iThID,iThID,_channInfo._cLocation,GE_AXISLOCATIONCHANN,iType_,iAlarmType);
	if (!(pDB->Exec()))
	{
	    iThID=-1;
		goto QUIT_LEARNADDTHRESHOLDS;
	}
	pDB->Cmd("SELECT * from [%s] ",pPlant->GetThTblName().c_str());
	pDB->Cmd(" WHERE [%s]=%d ",gc_cThreshold_ID,iThID);
	if (pDB->More(TRUE))
	{
		pDB->SetBuffer(gc_cThreshold,(BYTE*)&(th),sizeof(CThreshold));
		pDB->SetFieldValue(gc_cThreshold_Len,1);
		pDB->SetBuffer(gc_cData,(BYTE*)pAxisData,sizeof(S_AxisData)*iLearnLen);
		pDB->SetFieldValue(gc_cData_Len,iLearnLen);
		pDB->SetBuffer(gc_cLearnParam,(BYTE*)&(_channInfo._axisAlarmParam._thLearnParamS),sizeof(CThresholdLearnParam));
		pDB->SetFieldValue(gc_cLearnParam_Len,1);
		pDB->SetFieldValue(gc_cStart_T,timeStart);
		pDB->SetFieldValue(gc_cEnd_T,timeEnd);
		pPlant->_iThID=iThID;
		_timeThLearnS=_timeCurr;
	}
	else
	{
		iThID=-1;
#ifdef _DEBUG
		AfxMessageBox("学习缓变阈值失败");
#endif
		goto QUIT_LEARNADDTHRESHOLDS;
	}
QUIT_LEARNADDTHRESHOLDS:
    if (iThID>0)
	{
      pPlant->_iThID=iThID;
	  pPlant->_pLockThID->EndWrite();
      pDB->CommitTran();
    }
	else
	{
		pPlant->_pLockThID->EndWrite();
		pDB->RollbackTran();
	}
	if (pAxisData!=NULL) delete [] pAxisData;
	return iThID;
}

int CAxisAlarmChannParam::LearnThresholdS(int iDataLen_,S_AxisData *pAxisData_)
{
	int iRtn=-1;
    double *pdX=NULL,*pdY=NULL;
	pdX=new double[iDataLen_];
	pdY=new double[iDataLen_];
	for (int nloop=0;nloop<iDataLen_;++nloop)
	{
		pdX[nloop]=pAxisData_[nloop]._fCoorX;
		pdY[nloop]=pAxisData_[nloop]._fCoorY;
	}
	S_AxisCenterPositionThreshold__CalcInput calcInput;
	S_AxisCenterPosition_threshold th;
	calcInput.AxisCenterPosition_X=pdX;
	calcInput.AxisCenterPosition_Y=pdY;
	calcInput.f_hope=_channInfo._axisAlarmParam._thLearnParamS._fParam1;
	calcInput.f_zoomCorfficient=_channInfo._axisAlarmParam._thLearnParamS._fParam2;
	calcInput.i_length=iDataLen_;
	CAnalizeDiagnose_MW dgn;
    if (dgn.calcAxisCenterPosition_threshold(calcInput,th))
	{
		_thS._fThreshold1=th.d_center_X;
		_thS._fThreshold2=th.d_center_Y;
		_thS._fThreshold3=th.d_Radius;
        iRtn=1;
    }
	else
	{
		iRtn=-1;
#ifdef _DEBUG
		CString strError;strError.Format("CAxisAlarmChannParam::LearnThresholdS  ErrorNo:%d",th.errorCode);
		AfxMessageBox(strError);
#endif
	}
	if (pdX==NULL) delete [] pdX;
	if (pdY==NULL) delete [] pdY;
	return iRtn;
}

int CAxisAlarmChannParam::SaveData(IDBInterFace * pDB_,int iLocation_,string sTblName_,int iEventID_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iRtn=-1;
	S_AxisData axisData=_pAxisData[iLocation_];
	//[TBC]
	pDB->Cmd("INSERT INTO [%s] ",sTblName_.c_str());
	pDB->Cmd(" (%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cName,gc_cRev,gc_cTime,gc_cType,gc_cCoor_X,gc_cCoor_Y,gc_cDC_X,gc_cGap_X,gc_cDC_Y,gc_cGap_Y);
	pDB->Cmd(" VALUES(%d,0,'%s',%f,'%s',%d,%f,%f,%f,%f,%f,%f)",iEventID_,_channInfo._cLocation,axisData._fRev,TimeConvertString(_pDateTime[iLocation_]),_iAxisRegStatus,axisData._fCoorX,axisData._fCoorY,axisData._fHorDC,axisData._fHorGap,axisData._fVerDC,axisData._fVerGap);
   	if (pDB->Exec())
	  iRtn=iEventID_;
	else
	{
#ifdef _DEBUG
		AfxMessageBox("保存轴位移报警数据失败！");
#endif
	}
	return iRtn;
}

CString CAxisAlarmChannParam::TimeConvertString(CBHDateTime time_)
{
	CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;
}

int CAxisAlarmChannParam::FirstSlowAlarmOP(IDBInterFace * pDb_)
{
   int iRtn=-1;
   CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
   int iEventID=pPlant->_iEventID+1;
   iRtn=AddAlarmLog(pDb_,_iSlowStatus,_iCurrThIDSlow);
   string sTblName=pPlant->GetAlarmAxisTblName();
   if (iRtn>-1)
   {
	   _iEventIDSlow=iEventID;
	   pPlant->_iEventID=iEventID;
	   int index=_iCurrIndex;
	   for(int nloop=0;nloop<G_D_ALARM_SAVEWAVE_NUM;++nloop)
	   {
		   if (SaveData(pDb_,index,sTblName,iRtn)<0)
			   return -2;
		   else
		   {
			   --index;
			   if (index<0)
			     index=D_ALARM_SAVECHAR_NUM-1;
		   }
	   }
   }
   return iRtn;
}

unsigned int CAxisAlarmChannParam::GetAlarmStatus(S_AlarmState *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iFastState=_iFastStatus;
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iRegState=_iAxisRegStatus;
		pAlarmState_->_iTrendState=0;
	}
	return _iSlowStatus|_iFastStatus|_iAxisRegStatus;
}

void CAxisAlarmChannParam::SetAlarmStatus(S_AlarmState sAlarmState_)
{
	_iFastStatus=sAlarmState_._iFastState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iAxisRegStatus=sAlarmState_._iRegState;
}

void CAxisAlarmChannParam::SetAlarmStatus( S_AlarmStateWithLevel sAlarmState_ )
{
	_iFastStatus=sAlarmState_._iFastState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iAxisRegStatus=sAlarmState_._iRegState;
}
DATATYPE_CHAR CAxisAlarmChannParam::GetCurrSpecCharTypeValue()
{
	float fCoorX=.0,fCoorY=.0,fValue=.0;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL== pPlant)
	{
		return 0;
	}
	if (pPlant->GetPlantInfo()._iMachineType==GE_MACHINETYPE_RC)
	{
		//更新动态测点轴心位置数据
		if (!UpdateDynData())
			return fValue;
	}
	fCoorX=_pAxisData[_iCurrIndex]._fCoorX;
	fCoorY=_pAxisData[_iCurrIndex]._fCoorY;
    fValue=sqrt(pow(fCoorX,2)+pow(fCoorY,2));
	return fValue;
}

///动态参数报警类------------------------------------//
CDynChannParamAlarm::CDynChannParamAlarm()
{
	CDynChannParam();
	_bAlarmStatusS=false;
	_bFirstAlarmS=false;
	_bLearnTh=false;
	_iCurrCharValueIndex=0;
	CBHDateTime timeTmp=CBHDateTime::GetCurrentTime();
	_timeCurr=timeTmp;
	_timeSlowThLearn=timeTmp;
	_timeFastThLearn=timeTmp;
	_timeTrendCheckHour=timeTmp;
	_timeTrendCheckDay=timeTmp;
	_timeTrendCheckMonth=timeTmp;
	_timeTrendCheckSeason=timeTmp;
	_vDynCharValue.clear();
	_iFastStatus     = 0;
	_iSlowStatus     = 0;
	_iTrendStatus    = 0;
	_iRegStatus      = 0;
	_iCurrThIDFast   = 0;
	_iCurrThIDSlow   = 0;
	_iCurrWavePIndex = 0;
	_iRegAlarmFlag   = -1;
	_iCustomStatus   = 0;
	_iCustomAlarmFlag= -1;
	_iAlarmBit       = D_BHKD_AT_DYN;
	_fDynRegValueLast=.0;
	m_nFastRegStatus = 0;
	_timeDynNewAlarmLast=CBHDateTime(1999,1,1,1,1,1);
	_timeNewCustomAlarmLast=CBHDateTime(1999,1,1,1,1,1);
	for (int nloop=0;nloop<G_D_ALARM_SAVEWAVE_NUM;++nloop)
		_ppfWaveBuf[nloop]=NULL;
	for (int nloop=0;nloop<D_ALARM_SAVECHAR_NUM;++nloop)
		_ptimeBuf[nloop]=timeTmp;
	this->m_fLastAlarmValue = .0;

	this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
	this->InnerAlarmStatus(CHANN_ALARM_STATUS_INIT);
	this->m_oHalfNormalTimer5 = CBHDateTime::GetCurrentTime();
	this->ResetContinuousMoreThanTimes();
	this->PlantNewAlarmOccoured(FALSE);
	this->LastMaxAlarmValue(0);
	this->TimerForNewDCLogic(CBHDateTime::GetCurrentTimeEx());
	this->NewAlarmValue(G_F_INIT_VALUE);
	this->m_nAlarmLevel = ALARM_NORMAL;


}

CDynChannParamAlarm::~CDynChannParamAlarm()
{
	_vDynCharValue.clear();
}

int CDynChannParamAlarm::JudgeAlarm()
{
	int iRtn=0;
	/*	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;
	if (!_bLearnTh)
	{
	_iCurrThIDSlow=LearnAddSlowThreshold(E_TRIG_TH_RESTART);
	//		_iCurrThIDFast=LearnFastThreshold(E_TRIG_TH_RESTART);
	_bLearnTh=true;
	}
	iRtn=JudgeTrendAlarm();
	iRtn=iRtn|JudgeSlowAlarm();
	iRtn=iRtn|JudgeRegAlarm();*/
	return iRtn;
}

int CDynChannParamAlarm::JudgeSlowAlarm()
{
	return JudgeDynSlowAlarm();
}

int CDynChannParamAlarm::JudgeDynSlowAlarm()
{
	int iRtn=-1;
	return iRtn;
}


int CDynChannParamAlarm::JudgeTrendAlarm()
{
	return 0;
}

int CDynChannParamAlarm::JudgeFastAlarm()
{
	return 0;
}

int CDynChannParamAlarm::JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_, S_DynCharValue& CharValue_)
{
	//1.停车状态判断，停车设备取消报警状态
	S_ChannInfo sChannInfo=_channInfo;

	float fDynRegValueCurr=CharValue_.GetSpecTypeData(0);

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!= pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun)) //停车处理
		{
			return 0;
		}
	}

	int iRtn=0;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLH)
		{
			if (fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLL)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)||(fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)||(fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLL))
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		if((fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmHL)&&(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmHH)&&(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmLL))
				iRtn=1;
			else
			{
				iRtn=2;
			}
		}
		break;
	default:
		if (fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
	}

	if(iRtn > 0)
	{
		
		int iRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;

		if (iRtn==2)
		{	
			iRegStatus=iRegStatus|D_BHKD_AT_REGHIGH;
		}
		else
		{	iRegStatus=iRegStatus|D_BHKD_AT_REGLOW;
		}

		if(this->_iRegStatus <= 0)
		{
			pPlant->AddFastAlarmLog(pDB_,_channInfo._cChannNo,dtTime_,iRegStatus,_channInfo._iChannType);
		}
	}
	return iRtn;
}

int CDynChannParamAlarm::AddSlowAlarmLog(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->AddSlowAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeDynDataUpdate,_iSlowStatus,_channInfo._iChannType,_iCurrThIDSlow);

	}else
	{
		return pAlarmPlant->AddSlowAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,_iSlowStatus,_channInfo._iChannType,_iCurrThIDSlow);

	}}


int CDynChannParamAlarm::HandleSlowAlarmStop(IDBInterFace * pDb_,bool bLearnTh_)
{
	_iSlowStatus=0;
	_bFirstAlarmS=false;
	_siAlarmCountsS=0;
	_siCountsS=0;
	_bAlarmStatusS=false;
	if (bLearnTh_)
		_iCurrThIDSlow=LearnAddSlowThreshold(pDb_,E_TRIG_TH_ALARM);
	return _iCurrThIDSlow;
}


int CDynChannParamAlarm::AddRegAlarmLog(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeDynDataUpdate,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->AddRegAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,this->GetChannRegAlarmType(),_channInfo._iChannType);

	}}

int CDynChannParamAlarm::AddCustomAlarmLog(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->AddCustomAlarmLog(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeDynDataUpdate,_iCustomStatus,_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->AddCustomAlarmLog(pDB_,_channInfo._cChannNo,_timeCurr,_iCustomStatus,_channInfo._iChannType);

	}}

int CDynChannParamAlarm::HandleRegAlarmStop(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeDynDataUpdate,_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->SetRegAlarmLogEndTime(pDB_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType);

	}}

int CDynChannParamAlarm::HandleCustomAlarmStop(IDBInterFace * pDB_)
{
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	if (NULL!= pAlarmPlant)
	{
		return pAlarmPlant->SetCustomAlarmLogEndTime(pDB_,_channInfo._cChannNo,pAlarmPlant->_timeDynDataUpdate,_channInfo._iChannType);

	}else
	{
		return pAlarmPlant->SetCustomAlarmLogEndTime(pDB_,_channInfo._cChannNo,_timeCurr,_channInfo._iChannType);

	}}

void CDynChannParamAlarm::ResetRegAlarmStatus()
{
	_iRegStatus=0;
	_iRegAlarmFlag=-1;
}

void CDynChannParamAlarm::ResetCustomAlarmStatus()
{
	_iCustomStatus=0;
	_iCustomAlarmFlag=-1;
}

int CDynChannParamAlarm::LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDB_;
	SwitchDbNameEx(pDB,(pPlant->GetCompanyAlias()+pPlant->GetPlantNo()).c_str());
	int iLearnLen=100;
	int iThID=-1;
	int iAlarmType=D_BHKD_AT_SLOW|D_BHKD_AT_RADIALVIB;
	if (_channInfo._iChannType==GE_AXIALCHANN)
		iAlarmType=D_BHKD_AT_SLOW|D_BHKD_AT_AXISVIB;
	S_DynCharValue * pVibValue=NULL;
	if (iLearnLen>D_ALARM_SAVECHAR_NUM)
		iLearnLen=D_ALARM_SAVECHAR_NUM;
	pVibValue=new S_DynCharValue[iLearnLen];
	CBHDateTime timeStart=CBHDateTime::GetCurrentTime();
	CBHDateTime timeEnd=CBHDateTime::GetCurrentTime();
	///学习过程
	//读取学习数据
	int index=_iCurrCharValueIndex-iLearnLen+1;
	if (index<0)
		index=D_ALARM_SAVECHAR_NUM+index;
	else
		index=D_ALARM_SAVECHAR_NUM-index;
	pVibValue[0]=_pCharValueBuf[index];
	timeStart=_ptimeBuf[index];
	for (int nloop=1;nloop<iLearnLen;++nloop)
	{
		++index;
		if (!(index<D_ALARM_SAVECHAR_NUM))
			index=0;
		pVibValue[nloop]=_pCharValueBuf[index];
	}
	timeEnd=_ptimeBuf[index];
	int iTemp = LearnSlowThreshold(iLearnLen,pVibValue);
	pPlant->_pLockThID->BeginWrite();
	try
	{
		pDB->BeginTran();
		if (iTemp < 0)
			goto QUIT_LearnAddSlowThreshold;
		////添加门限值到门限表
		iThID = pPlant->_iThID+1;
		pDB->Cmd("INSERT INTO [%s] ",pPlant->GetThTblName().c_str());
		pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s)",gc_cID,gc_cThreshold_ID,gc_cName,gc_cChannType,gc_cType,gc_cSynFlag,gc_cAlarm_Type);
		pDB->Cmd(" VALUES(%d,%d,'%s',%d,%d,0,%d) ",iThID,iThID,_channInfo._cChannNo,_channInfo._iChannType,iType_,iAlarmType);
		if (!(pDB->Exec()))
		{
	
			CHZLogManage::Error("振动测点添加缓变阈值失败!"," CDynChannParamAlarm","LearnAddSlowThreshold");
			iThID=-1;
			goto QUIT_LearnAddSlowThreshold;
		}
		pDB->Cmd("SELECT * from [%s] ",pPlant->GetThTblName().c_str());
		pDB->Cmd(" WHERE [%s]=%d ",gc_cThreshold_ID,iThID);
		if (pDB->More(TRUE))
		{
			pDB->SetBuffer(gc_cThreshold,(BYTE*)_pthSlow,D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CThreshold));
			pDB->SetFieldValue(gc_cThreshold_Len,D_BHKD_ALARM_CHARTYPE_NUM);
			pDB->SetBuffer(gc_cData,(BYTE*)pVibValue,sizeof(S_VibCharValue)*iLearnLen);
			pDB->SetFieldValue(gc_cData_Len,iLearnLen);
			pDB->SetBuffer(gc_cLearnParam,(BYTE*)(_channInfo._chAlarmParam._pThLearnParamS),D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CThresholdLearnParam));
			pDB->SetFieldValue(gc_cLearnParam_Len,D_BHKD_ALARM_CHARTYPE_NUM);
			pDB->SetFieldValue(gc_cStart_T,timeStart);
			pDB->SetFieldValue(gc_cEnd_T,timeEnd);
			pPlant->_iThID=iThID;
			_timeSlowThLearn=_timeCurr;
		}
		else
		{
			iThID=-1;
			CHZLogManage::Error("振动测点学习缓变阈值失败!"," CDynChannParamAlarm","LearnAddSlowThreshold");
		}
	QUIT_LearnAddSlowThreshold:
		if (iThID>0)
		{
			pPlant->_iThID=iThID;
			pDB->CommitTran();
		}
		else
			pDB->RollbackTran();
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	pPlant->_pLockThID->EndWrite();
	if (pVibValue!=NULL)  delete [] pVibValue;
	return iThID;
}

int CDynChannParamAlarm::LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_)
{
	_timeFastThLearn=_timeCurr;
	return 0;
}

int CDynChannParamAlarm::JudgeDynSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_)
{
	return 0;
}


int CDynChannParamAlarm::DynSlowAlarmStatusOP()
{
	int iRtn=-1;
	return iRtn;
}


bool CDynChannParamAlarm::JudgeDynSlowNewAlarm()
{
	bool bRtn=false;
	return bRtn;
}

int CDynChannParamAlarm::JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_)
{
	int iStatus=0;
	float *pfValue=NULL;
	//获得数据
	ReadHisData(pDb_,timeStart_,timeEnd_);
	int len=_vDynCharValue.size();
	int nloop=0;
	if (len>0)
	{
		pfValue=new float[len];
		memset(pfValue,0,len*sizeof(float));
		//通频趋势报警判断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ALLCHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARALL;
		//一倍频趋势报警诊断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ONECHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARONE;
		//二倍频趋势报警诊断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_TWOCHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARTWO;
		//三倍频趋势报警诊断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_THREECHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARTHREE;
		//分频趋势报警诊断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_HALFCHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARHALF;
		//残振趋势报警诊断
		if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_RESCHAR,iJudgeAlarmType_)>0)
			iStatus|=D_BHKD_AT_CHARRES;
		if (_channInfo._chAlarmParam._bPhaseTrend)
		{
			if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_ONEPHASE,iJudgeAlarmType_)>0)
				iStatus|=D_BHKD_AT_PHASEONE;
			if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_TWOPHASE,iJudgeAlarmType_)>0)
				iStatus|=D_BHKD_AT_PHASETWO;
			if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_THREEPHASE,iJudgeAlarmType_)>0)
				iStatus|=D_BHKD_AT_PHASETHREE;
			if (JudgeSpecTypeTrendAlarm(pfValue,len,E_TH_HALFPHASE,iJudgeAlarmType_)>0)
				iStatus|=D_BHKD_AT_PHASEHALF;
		}
	}
	else
		iStatus=-1;
	if (pfValue!=NULL) delete [] pfValue;
	return iStatus;
}

int CDynChannParamAlarm::ReadHisData(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_)
{
	int nCount=0;
	CString strTblName;
	S_DynCharValue vibCharValue;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo *)_pParent;
	IDBInterFace *pDB=pDb_;
	SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	COleDateTimeSpan span;
	_vDynCharValue.clear();
	span=timeEnd_-timeStart_;
	if (span.GetMinutes()>59)
	{//从小时表中读取数据
		strTblName.Format("TREND_VIBHOUR_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else if (span.GetHours()>23)
	{//从天表中读取历史数据
		strTblName.Format("TREND_VIBDAY_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else if (span.GetDays()>29)
	{//从月中读取历史数据
		strTblName.Format("TREND_VIBMONTH_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	else
	{//从秒表中读取历史数据
		strTblName.Format("TREND_VIBSECOND_%s%s",pAlarmPlant->GetCompanyAlias().c_str(),pAlarmPlant->GetPlantID().c_str());
	}
	pDB->Cmd("SELECT %s,%s,%s,%s,%s,%s,%s,%s,%s,%s from [%s] ",_sCharOne.c_str(),_sCharZero.c_str(),_sCharTwo.c_str(),_sCharThree.c_str(),_sCharFour.c_str(),_sCharFive.c_str(),_sCharSix.c_str(),_sCharSeven.c_str(),_sCharEight.c_str(),_sCharNine.c_str(),strTblName);
	pDB->Cmd(" WHERE %s<'%s' AND %s>'%s' ",gc_cTime,TimeConvertString(timeEnd_),gc_cTime,TimeConvertString(timeStart_));
	while(pDB->More())
	{
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharZero.c_str()), 0);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharOne.c_str()), 1);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharTwo.c_str()), 2);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharThree.c_str()), 3);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharFour.c_str()), 4);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharFive.c_str()), 5);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharSix.c_str()), 6);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharSeven.c_str()), 7);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharEight.c_str()), 8);
		vibCharValue.SetSpecTypeData(pDB->GetDouble(_sCharNine.c_str()), 9);
		_vDynCharValue.push_back(vibCharValue);
		++nCount;
	}
	return nCount;
}

int CDynChannParamAlarm::JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_)
{
	CTrendCheckParam trendTh=_channInfo._chAlarmParam._pTrendParam[iCharType_];
	int nloop=0;
	int iStatus=-1;

	pfValue_[nloop]= _vDynCharValue[nloop].GetSpecTypeData(iCharType_);

	iStatus=JudgeAmpTrendDataIFAlarm(pfValue_,nloop,trendTh,iJudgeAlarmType_);
	return iStatus;
}

int CDynChannParamAlarm::JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_)
{
	int iStatus=0;
	S_TrendEnent_CalcInput calcInput;
	double *pdData=new double[iLen_];
	for (int nloop=0;nloop<iLen_;++nloop)
		pdData[nloop]=pfData_[nloop];
	calcInput.pTrendArrChar=pdData;
	calcInput.d_threshold=trendTh_._fParam1;
	calcInput.i_length=iLen_;
	int iType=1;
	switch(iJudgeAlarmType_)
	{
	case D_BHKD_AT_TRENDHOUR:
		iType=1;
		break;
	case D_BHKD_AT_TRENDDAY:
		iType=2;
		break;
	case D_BHKD_AT_TRENDMONTH:
		iType=3;
		break;
	case D_BHKD_AT_TRENDSEASON:
		iType=4;
		break;
	default:
		iType=1;
	}
	calcInput.i_trendType=iType;
	CAnalizeDiagnose_MW dgn;
	S_TrendEnent_result trendRs;
	if(dgn.calcTrendEnent(calcInput,trendRs))
	{
		switch(trendRs.i_trendEventResult)
		{
		case 0:
			iStatus=0;
			break;
		case 1:
			iStatus=D_BHKD_AT_TRENDSLOWASC;
			break;
		case 2:
			iStatus=D_BHKD_AT_TRENDFASTASC;
			break;
		case 3:
			iStatus=D_BHKD_AT_TRENDSLOWDSC;
			break;
		case 4:
			iStatus=D_BHKD_AT_TRENDFASTDSC;
			break;
		default:
			iStatus=0;
		}
	}
	else
		iStatus=-1;
	delete [] pdData;
	return iStatus;
}

int CDynChannParamAlarm::AddTrendAlarmLog(IDBInterFace * pDb_,int iStatus_,int iTrendType_)
{
	S_DynCharValue *pVibCharValue=NULL;
	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;
	IDBInterFace *pDB=pDb_;
	SwitchDbNameEx(pDB,(pAlarmPlant->GetCompanyAlias()+pAlarmPlant->GetPlantNo()).c_str());
	int iLen= _vDynCharValue.size();
	int iEventID=pAlarmPlant->_iTrendEventID+1;
	float fRev=pAlarmPlant->GetSpecTypeRealRev(_channInfo._iRevType,false);

	if (iLen>0)
	{
		pVibCharValue=new S_DynCharValue[iLen];
		//[TBC]
		for (int nloop=0;nloop<iLen;++nloop)
			pVibCharValue[nloop] = _vDynCharValue[nloop];
		pDB->BeginTran();
		pDB->Cmd("INSERT INTO [%s]",pAlarmPlant->GetAlarmLogTblName().c_str());
		pDB->Cmd("(%s,%s,%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cChannNo,gc_cChannType,gc_cRev,gc_cTime,gc_cTrendType,gc_cType,gc_cSynFlag);
		pDB->Cmd(" VALUES(%d,0,'%s',%d,%f,'%s',%d,%d,0) ",iEventID,_channInfo._cChannNo,_channInfo._iChannType,fRev,pAlarmPlant->_sTimeCurr.c_str(),iTrendType_,iStatus_);
		if (pDB->More(TRUE))
		{
			pDB->Cmd("SELECT %s,%s,%s,%s from [%s] ",gc_cData,gc_cData_Len,gc_cThreshold,gc_cThreshold_Len,pAlarmPlant->GetAlarmLogTblName().c_str());
			pDB->Cmd(" WHERE %s=%d  ",gc_cEventID,iEventID);
			if (pDB->More(TRUE))
			{
				pDB->SetFieldValue(gc_cData_Len,iLen);
				pDB->SetFieldValue(gc_cThreshold_Len,D_BHKD_ALARM_CHARTYPE_NUM);
				pDB->SetBuffer(gc_cThreshold,(BYTE *)(_channInfo._chAlarmParam._pTrendParam),D_BHKD_ALARM_CHARTYPE_NUM*sizeof(CTrendCheckParam));
				pDB->SetBuffer(gc_cData,(BYTE *)pVibCharValue,sizeof(S_VibCharValue)*iLen);
				pDB->CommitTran();
				pAlarmPlant->_iTrendEventID=iEventID;
			}
			else
			{
				pDB->RollbackTran();
				iEventID=-1;
			}
		}
		else
		{
			pDB->RollbackTran();
			iEventID=-1;
		}
	}
	if (pVibCharValue!=NULL) delete [] pVibCharValue;
	return iEventID;
}

void CDynChannParamAlarm::SetParentPoint(LPVOID pParent_)
{
	_pParent=pParent_;
	if (GE_VIBCHANN==_channInfo._iChannType)
		_iAlarmBit=D_BHKD_AT_RADIALVIB;
	else
		_iAlarmBit=D_BHKD_AT_AXISVIB;
}



DATATYPE_CHAR CDynChannParamAlarm::GetSpecCharTypeValue(int index_,int iCharType_/* =GE_ALLCHAR */)
{
	return _pCharValueBuf[index_].GetSpecTypeData(iCharType_);
}



DATATYPE_CHAR CDynChannParamAlarm::GetCurrSpecCharTypeValue(int iCharType_/* =GE_ALLCHAR */)
{
	return _pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(iCharType_);
}

void CDynChannParamAlarm::SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	for (int nloop=0; nloop < G_D_ALARM_SAVEWAVE_NUM; ++nloop)
		_ppfWaveBuf[nloop] = &(pWave_[nloop*iSmpNum_]);

	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数

}

CString CDynChannParamAlarm::TimeConvertString(CBHDateTime time_)
{
	CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;
}

bool CDynChannParamAlarm::SetChannValue(BHKD::S_DynCharValue *pDynCharValue_)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo *)_pParent;
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	pPlant->_pLockDataUpdateStatues->BeginRead();
	dt = pPlant->_timeDynDataUpdate;
	pPlant->_pLockDataUpdateStatues->EndRead();
	++_iCurrCharValueIndex;
	if (!(_iCurrCharValueIndex<D_ALARM_SAVECHAR_NUM))
		_iCurrCharValueIndex=0;
	_ptimeBuf[_iCurrCharValueIndex]=dt;
    /*
	CString sTemp;
	sTemp.Format("_iCurrCharValueIndex:%d,_ptimeBuf[_iCurrCharValueIndex]:%s",_iCurrCharValueIndex,CPlantInfoBase::TimeConvertString(_ptimeBuf[_iCurrCharValueIndex]));
	BHTRACE_DEBUG(sTemp);
	for (int i = 0; i <= _iCurrCharValueIndex; i++)
	{
		sTemp.Format("_ptimeBuf[%d]=%s, _ptimeBuf=0x%X,this=0x%X",i,_ptimeBuf[i].Format(),_ptimeBuf,this);
		BHTRACE_DEBUG(sTemp);
	}
    */
	_pCharValueBuf[_iCurrCharValueIndex]=(*pDynCharValue_);

	if (_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum > this->_nChannWavePointNum)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d****************** : 数采配置的采样点数:%d 大于中间件组态配置的采样点数:%d，忽略多余点数"),
			__FILE__,
			__LINE__,_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum,
			_nChannWavePointNum);

		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		_pCharValueBuf[_iCurrCharValueIndex]._iSmpNum = this->_nChannWavePointNum;
	}

	return true;
}

bool CDynChannParamAlarm::SetChannWave(const DATATYPE_WAVE *pfWave_,const int &iSmpNum_)
{
	if( iSmpNum_<=0 )  return false;
	++_iCurrWavePIndex;
	if (!(_iCurrWavePIndex < G_D_ALARM_SAVEWAVE_NUM))
		_iCurrWavePIndex=0;

	//取初始采样点数和传入采样点数中较小的作为实际的采样点数
	int lnReciveDataSize = iSmpNum_ * sizeof(DATATYPE_WAVE);

	int lnBufferSize = this->_nChannWavePointNum  * sizeof(DATATYPE_WAVE);

	if (lnReciveDataSize>lnBufferSize)
	{
		lnReciveDataSize = lnBufferSize;
	}

	if (lnReciveDataSize<=0)
	{
		return false;
	}

	this->_sDynCharValue.SetSpecTypeData(lnReciveDataSize/sizeof(DATATYPE_WAVE),GE_SAMPLENUM);

	memcpy(_ppfWaveBuf[_iCurrWavePIndex], pfWave_, lnReciveDataSize);

	return true;
}

S_DynCharValue CDynChannParamAlarm::GetCharValue(int index_)
{
	return _pCharValueBuf[index_];
}


S_DynCharValue CDynChannParamAlarm::GetCharValue()
{
	return _pCharValueBuf[GetCurrCharValueIndex()];
}


S_DynCharValue CDynChannParamAlarm::GetCharValueEx(int index_)
{
	return _pCharValueBuf[index_];
}


S_DynCharValue CDynChannParamAlarm::GetCharValueEx()
{
	return _pCharValueBuf[GetCurrCharValueIndex()];
}

const DATATYPE_WAVE * CDynChannParamAlarm::GetCurrWavePoint()
{
	return _ppfWaveBuf[GetCurrWaveIndex()];
}

const DATATYPE_WAVE * CDynChannParamAlarm::GetWavePoint(int iIndex_)
{
	return _ppfWaveBuf[iIndex_];
}

unsigned int CDynChannParamAlarm::GetAlarmStatus(S_AlarmState *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iFastState=_iFastStatus;
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iTrendState=_iTrendStatus;
		pAlarmState_->_iRegState=_iRegStatus;
	}
	return _iSlowStatus|_iTrendStatus|_iFastStatus|_iRegStatus;
}

unsigned int CDynChannParamAlarm::GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_)
{
	if (NULL!=pAlarmState_)
	{
		pAlarmState_->_iFastState=_iFastStatus;
		pAlarmState_->_iSlowState=_iSlowStatus;
		pAlarmState_->_iTrendState=_iTrendStatus;
		pAlarmState_->_iRegState=_iRegStatus;
		pAlarmState_->_fLastAlarmVal=m_fLastAlarmValue;
		pAlarmState_->_iTrendState = m_nAlarmLevel;
	}
	return _iSlowStatus|_iTrendStatus|_iFastStatus|_iRegStatus;
}

void CDynChannParamAlarm::SetAlarmStatus(S_AlarmState& sAlarmState_)
{
	_iFastStatus=sAlarmState_._iFastState;
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=sAlarmState_._iTrendState;
}

void CDynChannParamAlarm::SetAlarmStatus( S_AlarmStateWithLevel& sAlarmState_ )
{
	_iFastStatus=sAlarmState_._iFastState;
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=sAlarmState_._iTrendState;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iAlarmLevel;
}
void CDynChannParamAlarm::SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_)
{
	_iFastStatus=sAlarmState_._iFastState;
	_iRegStatus=sAlarmState_._iRegState;
	_iSlowStatus=sAlarmState_._iSlowState;
	_iTrendStatus=0;
	m_fLastAlarmValue=sAlarmState_._fLastAlarmVal;
	m_nAlarmLevel = (E_ALARM_LEVEL)sAlarmState_._iTrendState;
}

bool CDynChannParamAlarm::CheckLastAndCurrentValue(float fLast_,float fCurr_)
{
	bool bRtn=false;
	if(fLast_!=0)
	{
		if((fCurr_/fLast_)>D_FLUCTUATE_RANGE)
			bRtn=true;
	}
	else
	{
		if (fCurr_>D_FLUCTUATE_RANGE)
			bRtn=true;
	}
	return bRtn;
}

int CDynChannParamAlarm::JudgeRegAlarm(IDBInterFace * pDb_,
									   int &iFlag_,
									   bool &bSaveAlarmDataFlag_
									   ,bool &bSaveOneAlarmData_,
									   bool bConfirmAlarm_)
{
	iFlag_=-1;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;

	int lnRegStatus = 0;
	bool lbJudgeAlarm = false;

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();
	if (loTimeSpan.GetTotalHours() < GetAlarmOverridTime())
	{
		lbJudgeAlarm = true;
	}

	if(_channInfo._chAlarmParam._bRegular)
	{
		int iType=JudgeRegDataAlarm(bConfirmAlarm_);

		CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

		this->ComputeEigenValue(pAlarmPlant->GetPlantInfo()._iMachineType);

		float fDynCurrValue=_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0);


		if (iType>0)
		{
			lnRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			
			if (iType==2)
			{	
				lnRegStatus=lnRegStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	lnRegStatus=lnRegStatus|D_BHKD_AT_REGLOW;
			}

			m_nFastRegStatus = lnRegStatus;

			if(lbJudgeAlarm)
			{
				this->m_fLastAlarmValue = fDynCurrValue;

				_iRegStatus = lnRegStatus;
			}
			

			/*if(_iRegAlarmFlag<0)*/
			if(1)
			{
				S_ChannInfo sChannInfo=_channInfo;
				COleDateTimeSpan dtSpan;
				dtSpan=_timeCurr-_timeDynNewAlarmLast;
				int iTotaldays=dtSpan.GetTotalDays();

				if (iTotaldays<D_ALARM_REG_DAYTIME)
				{
					bool bRtn=JudgeRegAgainAlarm(fDynCurrValue,_fDynRegValueLast);

					if (bRtn)
					{
						CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
						COleDateTimeSpan dtSpan;
						dtSpan=_timeCurr-(pPlant->_dtVibRegDataSaveOne);
						int iTotalMinutes=dtSpan.GetTotalMinutes();
	
						if (iTotalMinutes>=D_ALARM_REG_INTVTIME)
						{
							bSaveOneAlarmData_=true;

						}
						else
						{	
							
						}

					}
					else
					{
						if (-1 == _iRegAlarmFlag)
						{
							
							if(AddRegAlarmLog(pDb_)<0)
							{
								CString strInfo;
								strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
								CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
								_iRegStatus=0;

							}else
							{
								if (this->JudgeRegResetPlantAlarmTimer(fDynCurrValue,_fDynRegValueLast))
								{
									bSaveAlarmDataFlag_ = true;
									iFlag_=1;

								}else
								{
									bSaveOneAlarmData_ = true;
								}

							}
							

						}else
						{

							if (this->JudgeRegResetPlantAlarmTimer(fDynCurrValue,_fDynRegValueLast))
							{
								bSaveAlarmDataFlag_ = true;
								iFlag_=1;

							}else
							{
								bSaveOneAlarmData_ = true;
							}

						}

						_timeDynNewAlarmLast=_timeCurr;
						_fDynRegValueLast=fDynCurrValue;
						_iRegAlarmFlag=1;
						
					}
				}
				else
				{
					
					if(AddRegAlarmLog(pDb_)<0)
					{
						CString strInfo;
						strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
						CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
						_iRegStatus=0;

					}else
					{
						bSaveAlarmDataFlag_ = true;
					}
					
					_timeDynNewAlarmLast=_timeCurr;
					_fDynRegValueLast=fDynCurrValue;
					_iRegAlarmFlag=1;
					iFlag_=1;

				}
			}
		}
		else
		{
			_iRegStatus=0;
			
			if (_iRegAlarmFlag==1)
			{
				_iRegAlarmFlag=0;
				_fDynRegValueLast=fDynCurrValue;
				HandleRegAlarmStop(pDb_);
			}
		}
	}
	else
	{
		if(_iRegStatus>0)
		{			
			HandleRegAlarmStop(pDb_);
		}
		_iRegStatus=0;
		_iRegAlarmFlag=-1;
	}
	return lnRegStatus;
}

int CDynChannParamAlarm::JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool&bSaveLastAlarmData_)
{
	iFlag_=-1;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	float pfCustomData[GD_CUSTOMPARAM_BUFFSIZE] = {.0};
	pPlant->GetSpecDynChannCustomData(_channInfo._cChannID,pfCustomData);
	int iSzie=GetCustomAlarmParamSize();
	//CString strTmp;strTmp.Format("vCustomAlarmParam.size():%d",vCustomAlarmParam.size());BHTRACE_DEBUG(strTmp);
	int iRtn=0;
	for (int i=0;i<iSzie;i++)
	{
		CCustomAlarmParamInfo customAlarmParamInfo;
		customAlarmParamInfo=GetSpecIndexCustomAlarmParamInfo(i);
		int iStartSegNo=customAlarmParamInfo._iSegStartNo;
		int iSegNum=customAlarmParamInfo._iSegNum;
		//CString strTmp;strTmp.Format("iSegNum:%d,iStartSegNo:%d,_fAlarmL:%f",iSegNum,iStartSegNo,vCustomAlarmParam[i]._fAlarmL);BHTRACE_DEBUG(strTmp);
		for (int j=iStartSegNo;j<iStartSegNo+iSegNum;j++)
		{
			//CString strTmp;strTmp.Format("获得动态自定义参数数据pfCustomData[%d]:%f",j,pfCustomData[j]);BHTRACE_DEBUG(strTmp);
			if(customAlarmParamInfo._iIF_Alarm >0)
			{
				//根据报警判断类型获得报警类型
				switch(customAlarmParamInfo._iJudgeType)
				{
				case 1://低通
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
					{
						if(pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
					break;
				case 2://高通
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
					{
						if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
					break;
				default:
					if (pfCustomData[j]>customAlarmParamInfo._fAlarmLTH)
					{
						if(pfCustomData[j]>customAlarmParamInfo._fAlarmHTH)
						{
							iRtn=2;
						}
						else
							iRtn=1;
					}
				}
				//CString strTmp;strTmp.Format("动态自定义报警iRtn:%d,_iCustomAlarmFlag:%d,_fCustomParam[%d]:%f,vCustomAlarmParam[%d]._fAlarmL:%f,vCustomAlarmParam[%d]._fAlarmH:%f",iRtn,_iCustomAlarmFlag,j,_channInfo._fCustomParam[j],i,vCustomAlarmParam[i]._fAlarmL,i,vCustomAlarmParam[i]._fAlarmH);BHTRACE_DEBUG(strTmp);
				//保存自定义报警日志
				if (iRtn>0)
				{
					_iCustomStatus=D_BHKD_AT_CUSTOM|_iAlarmBit;
					if (iRtn==2)
						_iCustomStatus=_iCustomStatus|D_BHKD_AT_CUSTOMHIGH;
					else
						_iCustomStatus=_iCustomStatus|D_BHKD_AT_CUSTOMLOW;

					if(_iCustomAlarmFlag<0)
					{
						COleDateTimeSpan dtSpan;
						dtSpan=_timeCurr-_timeNewCustomAlarmLast;
						int iTotaldays=dtSpan.GetTotalDays();
						//自定义报警间隔两天新增一条报警日志
						if (iTotaldays>D_ALARM_REG_DAYTIME)
						{
							//添加自定义报警日志
							if(AddCustomAlarmLog(pDb_)<0)
							{
								CString strInfo;
								strInfo.Format("%s-%s 振动添加自定义报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
								CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeCustomAlarm");
							}
							_iCustomAlarmFlag=1;
							iFlag_=1;
							_timeNewCustomAlarmLast=_timeCurr;
						}
						else
						{
							_iCustomAlarmFlag=1;
							iFlag_=0;
							bSaveLastAlarmData_=true;
						}
					}
				}
				else
				{
					_iCustomStatus=0;
					if (_iCustomAlarmFlag==1)
					{
						_iCustomAlarmFlag=0;
						HandleCustomAlarmStop(pDb_);
					}
				}
			}
			else
			{
				if(_iCustomStatus>0)
					HandleCustomAlarmStop(pDb_);
				_iCustomStatus=0;
				_iCustomAlarmFlag=-1;
			}
		}
	}
	//strTmp.Format("_iCustomStatus:%d",_iCustomStatus);BHTRACE_DEBUG(strTmp);
	return _iCustomStatus;
}
bool CDynChannParamAlarm::JudgeRegAgainAlarmForConfirm()
{
	if (!this->_pParent)
	{
		return true;
	}

	float fDynCurrValue=_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0);

	if (g_UseNewDCAlarmLogic&&g_UseNewAlarmLogic)
	{
		if (GE_RODSINKCHANN == this->_channInfo._iChannType)
		{	
			bool lbRun = ((CAlarmPlantInfo*)this->_pParent)->GetVibJudgeRunStatus()||((CAlarmPlantInfo*)this->_pParent)->GetProcJudgeRunStatus();
			if (IsPlantRunning(*((CAlarmPlantInfo *)this->_pParent),lbRun))
			{
				float lfOriginalDC = this->DCOriginal();

				fDynCurrValue= abs(fDynCurrValue - this->DCOriginal()); 

			}else
			{
				fDynCurrValue = 0;
			}
		}			 
	}

	bool bRtn = false;
	if (g_UseNewAlarmLogic)
	{
		bRtn=JudgeRegResetPlantAlarmTimer(fDynCurrValue,this->LastMaxAlarmValue());
	}else
	{
		bRtn=JudgeRegResetPlantAlarmTimer(fDynCurrValue,_fDynRegValueLast);
	}

	return !bRtn;
}

bool CDynChannParamAlarm::JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_)
{
	return CChannInfoBase::JudgeRegResetPlantAlarmTimer(fCurrValue_,fValueLast_);
}

bool CDynChannParamAlarm::JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_)
{
	CAlarmPlantInfo * lpPlant = (CAlarmPlantInfo *)this->_pParent;

	if (NULL!= lpPlant)
	{
		bool lbRun = lpPlant->GetVibJudgeRunStatus()||lpPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*lpPlant,lbRun))
		{
			return 0;
		}
	}

	bool bRtn=false;
	S_ChannInfo sChannInfo=_channInfo;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fCurrValue_<=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fCurrValue_>=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	case E_ALARMCHECK_BANDPASS:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_<=fValueLast_)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_>=fValueLast_)
			{

				bRtn=true;
			}
			else
			{
				bRtn=false;
			}
		}
		break;
	default:
		if (fCurrValue_<=fValueLast_)
			bRtn=true;
		else
			bRtn=false;
		break;
	}
	return bRtn;
}

int CDynChannParamAlarm::JudgeRegDataAlarm(bool bConfirmAlarm_ )
{
	S_ChannInfo sChannInfo=_channInfo;

	float fDynRegValueCurr=_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0);

	BOOL lbNeedComputeEigenValue = this->IsNeedComputeEigenValue(_pCharValueBuf[_iCurrCharValueIndex]);

	if (lbNeedComputeEigenValue)
	{
		fDynRegValueCurr = this->ComputeAllCharValue(_pCharValueBuf[_iCurrCharValueIndex]);
	}
	


	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL!= pPlant)
	{
		bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pPlant,lbRun)) //停车处理
		{
			return 0;
		}
	}

	//新的直流量报警逻辑
	if (g_UseNewDCAlarmLogic&&g_UseNewAlarmLogic)
	{
		if (GE_RODSINKCHANN == this->_channInfo._iChannType)
		{
			sChannInfo._chAlarmParam._iRegJudgeType = E_ALARMCHECK_LOWPASS;
			
			bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
			if (IsPlantRunning(*pPlant,lbRun))
			{
				float lfOriginalDC = this->DCOriginal();

				if (IsFloatZero(G_F_INIT_VALUE- lfOriginalDC))
				{

					this->DCOriginal(fDynRegValueCurr);
					this->SaveDataForNewDCLogic();
				}

				fDynRegValueCurr= abs(fDynRegValueCurr - this->DCOriginal());
			}else
			{
				fDynRegValueCurr = 0;
			}
		}			 
	}

	int iRtn=0;
	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		if (fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if (fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLH)
		{
			if (fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLL)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_BANDPASS:
		if ((fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)||(fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLH))
		{
			if((fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)||(fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmLL))
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
		break;
	case E_ALARMCHECK_BANDSTOP:
		if((fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmHH)&&(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmLL))
		{
			if((fDynRegValueCurr<sChannInfo._chAlarmParam._fAlarmHL)&&(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmLH))
			{
				iRtn=2;
			}
			else
			{
				iRtn=1;
			}
		}
		break;
	default:
		if (fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHL)
		{
			if(fDynRegValueCurr>sChannInfo._chAlarmParam._fAlarmHH)
			{
				iRtn=2;
			}
			else
				iRtn=1;
		}
	}


	//2.报警确认状态判断，已确认报警取消报警状态
	if(bConfirmAlarm_)
	{
		if (this->IsAlarmConfirmed())
		{
			GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();

			if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
			{
				bool bRtn=JudgeRegAgainAlarmForConfirm();//没超过上次值的百分比默认15%则不再报警

				if (!bRtn)
				{
					this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 
				}

			}

			iRtn = 0;
		}
	}
	
	return iRtn;
}

int CDynChannParamAlarm::GetCurrCharValueIndex()
{
	return _iCurrCharValueIndex;
}

int CDynChannParamAlarm::GetCurrWaveIndex()
{
	return _iCurrWavePIndex;
}

string CDynChannParamAlarm::GetSpecIndexTime(int index_)
{
	string sRtn;
	sRtn=TimeConvertString(_ptimeBuf[index_]);
	/*
	CString sTemp;
	sTemp.Format("index_,%d,sRtn:%s",index_,sRtn.c_str());BHTRACE_DEBUG(sTemp);
	for (int i=0;i<=index_;i++)
	{
		sRtn=TimeConvertString(_ptimeBuf[i]);
		sTemp.Format("TimeConvertString(_ptimeBuf[%d])=%s", i,sRtn.c_str());
		BHTRACE_DEBUG(sTemp);
	}*/
	return sRtn;
}

int CDynChannParamAlarm::GetSlowAlarmStatus()
{
	return _iSlowStatus;
}

int CDynChannParamAlarm::GetRegAlarmStatus()
{
	return _iRegStatus;
}

int CDynChannParamAlarm::GetCustomAlarmStatus()
{
	return _iCustomStatus;
}

int CDynChannParamAlarm::LearnSlowThreshold(int iLearnDataLen_,S_DynCharValue *pVibValue_)
{
	int iRtn=-1;
	return iRtn;
}

CThreshold CDynChannParamAlarm::ModifyDynThS(CThreshold th_,bool bPhase_)
{
	float fCoef=1.3;
	if (bPhase_)
	{
		th_._fThreshold1=(th_._fThreshold1)*fCoef;
		th_._fThreshold2=(th_._fThreshold2)*fCoef;
		th_._fThreshold3=(th_._fThreshold3)*fCoef;
	}
	else
	{
		th_._fThreshold1=(th_._fThreshold1)*fCoef;
		th_._fThreshold2=(th_._fThreshold2)*fCoef;
		th_._fThreshold3=(th_._fThreshold3)*fCoef;
	}
	return th_;
}

float CVibChannParamAlarm::ComputeThreePhaseCharValue(S_VibCharValue  & arefSVibCharValue)
{

	
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL == pPlant)
	{
		ASSERT(FALSE);

		return 0;
	}

	float lfVal = EigenValueComputer::ComputeAxisEigenValue(    this->_channInfo,
																(DATATYPE_WAVE *)this->GetCurrWavePoint(),
																arefSVibCharValue._iRev,
																arefSVibCharValue._iSmpFreq,
																arefSVibCharValue._iSmpNum,
																(MACHINETYPE)pPlant->GetPlantInfo()._iMachineType,
																(CHANNTYPE)this->_channInfo._iChannType
																);

	arefSVibCharValue._fPhaseThree = lfVal;

	return lfVal;

}


BOOL CVibChannParamAlarm::IsEigenValueEmpty(S_VibCharValue & arefSVibCharValue)
{
	float val = 0;
	
	for (int i = GE_ALLCHAR ; i <= GE_RESCHAR ;i++)
	{
		val = arefSVibCharValue.GetSpecTypeData(i);

		if (!BHKD::IsFloatZero(val))
		{
			return FALSE;
		}
	}

	return TRUE;
}
 

BOOL CVibChannParamAlarm::IsNeedComputeEigenValue(S_VibCharValue & arefSVibCharValue)
{
	if (IsMWComputeFeatureValueEnabled())
	{
		return this->IsEigenValueEmpty(arefSVibCharValue);
	}else
	{
		return FALSE;
	}
}

BOOL CVibChannParamAlarm::ComputeEigenValue(int aEnumMachineType,int anIndex)
{
	try
	{
		S_VibCharValue & loValue = _pCharValueBuf[anIndex];

		if (this->IsNeedComputeEigenValue(loValue))
		{	//真实计算
			EigenValueComputer::ComputeVibEigenValue(
													loValue,
													this->_channInfo,
													loValue._iRev,
													(DATATYPE_WAVE *)this->GetCurrWavePoint(),
													loValue._iSmpNum,
													(MACHINETYPE)aEnumMachineType,
													(CHANNTYPE)this->_channInfo._iChannType
													);
		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}

	return TRUE;
}



BOOL CVibChannParamAlarm::ComputeEigenValue(int aEnumMachineType)
{
	try
	{
		S_VibCharValue & loValue = _pCharValueBuf[GetCurrCharValueIndex()];

		if (this->IsNeedComputeEigenValue(loValue))
		{	//真实计算
			EigenValueComputer::ComputeVibEigenValue(
														loValue,
														this->_channInfo,
														loValue._iRev,
														(DATATYPE_WAVE *)this->GetCurrWavePoint(),
														loValue._iSmpNum,
														(MACHINETYPE)aEnumMachineType,
														(CHANNTYPE)this->_channInfo._iChannType
														);
		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}

	return TRUE;
}


float CVibChannParamAlarm::ComputeAllCharValue(S_VibCharValue & arefSVibCharValue)
{

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL == pPlant)
	{
		ASSERT(FALSE);

		return 0;
	}

	float lfVal = EigenValueComputer::ComputeFirstEigenValue(this->_channInfo,
															(DATATYPE_WAVE *)this->GetCurrWavePoint(),
															arefSVibCharValue._iSmpNum,
															(MACHINETYPE)pPlant->GetPlantInfo()._iMachineType,
															(CHANNTYPE)this->_channInfo._iChannType
															);

	arefSVibCharValue._fCharAll = lfVal;

	return lfVal;
}

float CVibChannParamAlarm::LastAlarmValue() const
{
	return m_fLastAlarmValue;
}

void CVibChannParamAlarm::LastAlarmValue( float val )
{
	m_fLastAlarmValue = val;
}

void CVibChannParamAlarm::SaveCurrentAsLastAlarmValue()
{
	ASSERT(this->_iCurrCharValueIndex >=0 && this->_iCurrCharValueIndex < D_ALARM_SAVECHAR_NUM);

	const S_VibCharValue& loCharVal = this->_pCharValueBuf[this->_iCurrCharValueIndex];

	if (GE_AXIALCHANN == this->_channInfo._iChannType)
	{
		this->LastAlarmValue(loCharVal._fPhaseThree);
	}
	else
	{
		this->LastAlarmValue(loCharVal._fCharAll);
	}
}

_CHANN_ALARM_STATUS_ CVibChannParamAlarm::PredictChannAlarm(IDBInterFace * pDb_,
															bool abPlantAlarmed,//isPlantPreviousAlarmed 当前设备是否报警
															bool abConfirm /*=TRUE*/,
															bool abChangeStatus/*=TRUE*/)
{

	_CHANN_ALARM_STATUS_ lnChannStatus = CHANN_ALARM_STATUS_HALF_NORMAL;

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (!pPlant)
	{
		return lnChannStatus;
	}

	if (lnChannStatus>this->InnerAlarmStatus())
	{
		lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
	}

	_timeCurr=pPlant->_timeCurr;	
	
	if(_channInfo._chAlarmParam._bRegular)
	{
		
		//1.判断是否常规报警
		int iType=JudgeRegDataAlarm(pDb_,abConfirm);

		this->ComputeEigenValue(pPlant->GetPlantInfo()._iMachineType);

		float lfCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

		S_ChannInfo sChannInfo=_channInfo;			

		if (GE_AXIALCHANN==sChannInfo._iChannType)
		{	
			lfCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;
		}
		
		if (iType>0)
		{
			//2.获得上次报警值
			float lfLastAlarmValue = this->LastAlarmValueEx();
			
			//3.更新上次报警值
			this->LastAlarmValueEx(lfCurrValue);
			this->LastAlarmValue(lfCurrValue);

			//4.更新报警平均值，并视情况更新上次报警最大值
			this->updateEverageAlarmValue(lfCurrValue);

			int lnPreviousChannRegStatus = m_nRegStatus;

			int lnAlarmStatus = 0;
			//5.更新测点报警状态
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
			}
			else
			{	
				lnAlarmStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			}
			if (iType==2)
            {   
				lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGLOW;
			}
			
			m_nFastRegStatus = lnAlarmStatus;

			if (abChangeStatus)
			{
				m_nRegStatus = lnAlarmStatus;
			}
			
			//6.至少是旧报警，且不大于上次报警值
			lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_LOW;

			//7.判断是否大于上次报警值
			bool lbIsAgainAlarm=JudgeRegAgainAlarm(lfCurrValue,lfLastAlarmValue);

			if (!lbIsAgainAlarm )
			{
				//7.1不是重复报警则更新上次最大报警值
				float lfLastMaxValue = this->LastMaxAlarmValue();
				if (!JudgeRegAgainAlarm(lfCurrValue,this->LastMaxAlarmValue()))
				{
					this->LastMaxAlarmValue(lfCurrValue);
				}				
				
				//7.2至少是旧报警，且大于上次报警值，且连续三次
				this->IncreaseContinuousMoreThanTimes();
				if (this->ContinuousMoreThanTimes()>=g_alarm_larger_times)
				{
					lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_HIGH;
				}		


				//7.3判断是否新报警
				bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfLastMaxValue);

				if (CHANN_ALARM_STATUS_INIT == this->InnerAlarmStatus())
				{
					lbIsNewAlarm = TRUE;
				}

				if (this->JustStarted())
				{
					lbIsNewAlarm = TRUE;
					this->JustStarted(FALSE);
		
				}
				
				if (lbIsNewAlarm )
				{
					lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
					this->NewAlarmValue(lfCurrValue);
				}else
				{
					//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
					if (lnPreviousChannRegStatus)
					{
						float lfPreviousNewAlarmValue = this->NewAlarmValue();
						bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
						if (lbIsNewAlarm)
						{
							if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
							{
								lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
							}
							this->NewAlarmValue(lfCurrValue);
						}
					}
				}
				
			}else
			{
				//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
				if (lnPreviousChannRegStatus)
				{
					float lfPreviousNewAlarmValue = this->NewAlarmValue();

					bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
					if (lbIsNewAlarm)
					{
						if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
						{
							lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
						}
						
						this->NewAlarmValue(lfCurrValue);
					}
				}

				this->ResetContinuousMoreThanTimes();
			}
		}
		else
		{
			this->ResetContinuousMoreThanTimes();
			m_nRegStatus=0;			
		}
	}

	this->PredictedChannInnerAlarmStatus(lnChannStatus);

	return lnChannStatus;
}

float CVibChannParamAlarm::LastAlarmValueEx() const
{
	return this->m_fLastAlarmValueEx;
}

void CVibChannParamAlarm::LastAlarmValueEx( float val )
{
	this->m_fLastAlarmValueEx = val;
}





float CVibChannParamAlarm::LastMaxAlarmValue() const
{
	long lnData = .0;
	InterlockedExchange(&lnData,this->m_fLastMaxAlarmValue);
	float lfData = int_as_float(lnData);
	return lfData;
}

void CVibChannParamAlarm::LastMaxAlarmValue( float val )
{
	if (IsFloatZero(G_F_INIT_VALUE - val))
	{
		ASSERT(FALSE);
		return;
	}
	long lnData = float_as_int(val);
	InterlockedExchange(&this->m_fLastMaxAlarmValue,lnData);	
}

float CVibChannParamAlarm::updateEverageAlarmValue( float afLastAlarmValue )
{
	//1.更新均值
	float lfCurrentEverageValue = this->LastEverageAlarmValue();
	if (IsFloatZero(lfCurrentEverageValue - G_F_INIT_VALUE))
	{
		lfCurrentEverageValue = afLastAlarmValue;
	}

	float lfPreviousEverageValue = lfCurrentEverageValue;		
	lfCurrentEverageValue = (lfCurrentEverageValue+afLastAlarmValue)/2;
	this->LastEverageAlarmValue(lfCurrentEverageValue);



	//2.判断是否需要更新最大报警值
	CAlarmPlantInfo * lpParent = (CAlarmPlantInfo *)this->_pParent;
	if (NULL != lpParent)
	{
		COleDateTimeSpan lpSpan = this->_timeCurr - lpParent->m_oLastNewAlarmTime;
		if (lpSpan.GetTotalDays()>= lpParent->m_nNewAlarmMaxTime)
		{
			this->LastMaxAlarmValue(lfPreviousEverageValue);			
		}
	}

	return lfCurrentEverageValue;
}

void CVibChannParamAlarm::ResetHalfNormalTimer5()
{
	this->m_oHalfNormalTimer5 = this->_timeCurr;
}

_CHANN_ALARM_STATUS_ CVibChannParamAlarm::InnerAlarmStatus() const
{
	return m_nInnerAlarmStatus;
}

void CVibChannParamAlarm::InnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nInnerAlarmStatus = val;
}

int CVibChannParamAlarm::SwitchAlarmStatus(IDBInterFace * pDb_, _CHANN_ALARM_STATUS_ anNewStatus )
{	
	BOOL lbNewAlarmLogAdded = FALSE;

	if (CHANN_ALARM_STATUS_NORMAL == anNewStatus)
	{	
		if (this->InnerAlarmStatus()>anNewStatus)
		{
			this->HandleRegAlarmStop(pDb_);
		}
		this->ResetHalfNormalTimer5();
		
		this->ResetContinuousMoreThanTimes();

		int lnFValue = this->LastEverageAlarmValue();

		if (!IsFloatZero(lnFValue-G_F_INIT_VALUE))
		{
			this->LastMaxAlarmValue(lnFValue);
		}

		
		
		this->InnerAlarmStatus(anNewStatus);


	}else if (CHANN_ALARM_STATUS_HALF_NORMAL == anNewStatus)
	{
		
		//this->LastMaxAlarmValue(this->LastEverageAlarmValue());
		//从报警到正常
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			this->ResetHalfNormalTimer5();
			this->InnerAlarmStatus(CHANN_ALARM_STATUS_HALF_NORMAL);
			this->ResetContinuousMoreThanTimes();
		}

		//之前是半正常状态
		else if (CHANN_ALARM_STATUS_HALF_NORMAL == this->InnerAlarmStatus())
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;			
			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;
			
			if (NULL!=lpAlarmPlantInfo)
			{
				int lnHours = lpSpan.GetTotalHours();
				if (lnHours>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
					this->HandleRegAlarmStop(pDb_);
				}
			}
		}else
		{
			//ASSERT(FALSE);
		}
	//新状态是报警状态
	}else 
	{
		//旧状态是报警状态
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			if (CHANN_ALARM_STATUS_NEW_ALARM == anNewStatus)
			{
				this->HandleRegAlarmStop(pDb_);
				this->AddRegAlarmLog(pDb_);
				lbNewAlarmLogAdded = TRUE;
			}else
			{
				if (this->PlantNewAlarmOccoured())
				{
					this->AddRegAlarmLog(pDb_);
					lbNewAlarmLogAdded = TRUE;
				}
			}
			
			this->InnerAlarmStatus(anNewStatus);

		//旧状态是半正常状态，新状态不是新报警状态
		}else if (this->InnerAlarmStatus()== CHANN_ALARM_STATUS_HALF_NORMAL 
					&& anNewStatus!= CHANN_ALARM_STATUS_NEW_ALARM)
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;			
			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

			if (NULL!=lpAlarmPlantInfo)
			{
				int lnHours = lpSpan.GetTotalHours();
				if (lnHours>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(anNewStatus);
					this->HandleRegAlarmStop(pDb_);
				}else
				{
					this->InnerAlarmStatus(anNewStatus);
				}
			}
		
		}else//旧状态是正常状态
		{
			this->HandleRegAlarmStop(pDb_);
			 this->AddRegAlarmLog(pDb_);
			 this->InnerAlarmStatus(anNewStatus);
			 lbNewAlarmLogAdded = TRUE;
		}
	}
	
	return lbNewAlarmLogAdded;
}




int CVibChannParamAlarm::HandleRegAlarmStopEx( IDBInterFace * pDb_ )
{
	return this->HandleRegAlarmStop(pDb_);
}

int CVibChannParamAlarm::ContinuousMoreThanTimes() const
{
	return m_nContinuousMoreThanTimes;
}

void CVibChannParamAlarm::ContinuousMoreThanTimes( int val )
{
	m_nContinuousMoreThanTimes = val;
}

void CVibChannParamAlarm::ResetContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes = 0;
}

void CVibChannParamAlarm::IncreaseContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes++;
}

BOOL CVibChannParamAlarm::PlantNewAlarmOccoured() const
{
	return m_bPlantNewAlarmOccoured;
}

void CVibChannParamAlarm::PlantNewAlarmOccoured( BOOL val )
{
// 	if (val >0)
// 	{
// 		TRACE("t");
// 	}
	m_bPlantNewAlarmOccoured = val;
}

BOOL CVibChannParamAlarm::InitValues()
{
	if (NULL == this->_pLock)
	{
		return FALSE;
	}
	this->_pLock->BeginWrite();
	BEGIN_ERROR_HANDLE;
	
	BOOL lbRet = TRUE;
	float lfInitValue = .0;
	switch(this->_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_HIGHPASS:
		lfInitValue =  (this->_channInfo._chAlarmParam._fAlarmLH);		
		break;
	case E_ALARMCHECK_BANDPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_BANDSTOP:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmLL);
		break;
	}

	this->LastMaxAlarmValue(lfInitValue);
	float lfdata = this->LastMaxAlarmValue();
	this->LastAlarmValueEx(lfInitValue);
	this->LastEverageAlarmValue(G_F_INIT_VALUE);
	this->_fRegValueLast = G_F_INIT_VALUE;
	if ( GE_AXIALCHANN == this->_channInfo._iChannType )
	{
		float lfOriginalAxis = this->GetAxisValueForNewAxisAlarmLogic();
		this->AxisOriginal(lfOriginalAxis);
	}
	this->JustStarted(TRUE);
	END_ERROR_HANDLE;
	this->_pLock->EndWrite();
	return TRUE;
}

int CVibChannParamAlarm::GetChannRegAlarmType()
{
	if(isLocalJudgeAlarm())
	{
		if(this->m_nRegStatus > 0)
		{
			return this->m_nRegStatus;
		}
		if(this->_iFastStatus > 0)
		{
			return this->_iFastStatus;
		}
		if(this->_iSlowStatus > 0)
		{
			return this->_iSlowStatus;
		}

		
	}
	return this->m_nRegStatus;
}

BOOL CVibChannParamAlarm::JudgeAndDoForNewAxisLogic(BOOL abRunningStatusChanged)
{
	BOOL lbRet = FALSE;

	BEGIN_ERROR_HANDLE;

	if (NULL == this->_pParent)
	{
		return FALSE;
	}
	CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

	if (abRunningStatusChanged)
	{
		float lfOldAxisValue = this->GetAxisValueForNewAxisAlarmLogic();

		if (!IsFloatZero(lfOldAxisValue- G_F_INIT_VALUE))
		{	
			float lfAxisValue = _pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;
		
			this->AxisOriginal(lfAxisValue);
		}

		this->SaveAxisValueForNewAxisLogic();
	}
	else
	{
		if (this->IsReachSaveTimeIntervalForAxisValueLogic())
		{
			float lfAxisValue = _pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;

			this->AxisOriginal(lfAxisValue);

			this->SaveAxisValueForNewAxisLogic();
			
			this->ResetTimerForAxisValueLogic();
		}	
	}

	END_ERROR_HANDLE;
	return lbRet;
}

float CVibChannParamAlarm::GetAxisValueForNewAxisAlarmLogic()
{
	this->InitFileNameForNewAxisAlarmLogic();

	int lnFileSize = 1;	

	float lfData = .0;

	int lnReaded = readfile(&lfData,lnFileSize,this->m_strAxisValueFilePath);

	if (lnReaded<=0)
	{
		return G_F_INIT_VALUE;
	}

	return lfData;
}

BOOL CVibChannParamAlarm::InitFileNameForNewAxisAlarmLogic()
{

	if (this->m_strAxisValueFilePath.GetLength()==0)
	{
		if (NULL == this->_pParent)
		{
			return FALSE;

		}
		CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;
		CString lstrGroupName = lpAlarmPlantInfo->GetGroupName().c_str();
		CString lstrCompanyName = lpAlarmPlantInfo->GetCompanyName().c_str();
		CString lstrFactoryName = lpAlarmPlantInfo->GetFactoryName().c_str();
		CString lstrPlantNo = lpAlarmPlantInfo->GetPlantNo().c_str();
		CString lstrChannNo = this->GetChannInfo()._cChannNo;

		CString lstrPath=::GetAppPath()+_T("data\\");
		CString lstrPath2 ;
		CString lstrPath3;
		CString lstrDirectory ;


		const TCHAR * lpPathSplit = _T("\\");
		const TCHAR * lpFileExt = _T(".txt");
		const TCHAR * lpFileAxis = _T("_axisvalue");


		lstrPath.Append(lstrGroupName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrCompanyName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrFactoryName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrPlantNo);
		lstrPath.Append(lpPathSplit);

		lstrDirectory.Append(lstrPath);


		lstrPath.Append(lstrChannNo);

		lstrPath.Append(lpFileAxis);
		lstrPath.Append(lpFileExt);

		this->AxisFilePath(lstrPath);

		MakeSureFolderExist(lstrDirectory);
	}

	return TRUE;
}

CString CVibChannParamAlarm::AxisFilePath() const
{
	return this->m_strAxisValueFilePath;
}

void CVibChannParamAlarm::AxisFilePath( CString val )
{
	this->m_strAxisValueFilePath = val;
}

void CVibChannParamAlarm::AxisOriginal( float val )
{
	this->m_fAxisValue = val;
}

float CVibChannParamAlarm::AxisOriginal()
{
	return this->m_fAxisValue;
}

BOOL CVibChannParamAlarm::SaveAxisValueForNewAxisLogic()
{
	int lnRet = FALSE;

	BEGIN_ERROR_HANDLE;

	if (NULL == this->_pParent)
	{
		return FALSE;
	}

	this->InitFileNameForNewAxisAlarmLogic();

	float lfData = this->AxisOriginal();
	
	int lnRet = writefile(&lfData,1,this->AxisFilePath());

	this->AxisOriginal(lfData);  

	if (!lnRet)
	{
		ASSERT(FALSE);
	}

	END_ERROR_HANDLE;

	return lnRet;
}

CBHDateTime CVibChannParamAlarm::TimerForAxisValueLogic() const
{
	return this->m_oTimerForAxisValueLogic;
}

void CVibChannParamAlarm::TimerForAxisValueLogic( CBHDateTime val )
{
	this->m_oTimerForAxisValueLogic = val;
}

void CVibChannParamAlarm::ResetTimerForAxisValueLogic()
{
	this->m_oTimerForAxisValueLogic = CBHDateTime::GetCurrentTimeEx();
}

BOOL CVibChannParamAlarm::IsReachSaveTimeIntervalForAxisValueLogic()
{
	COleDateTimeSpan loSpan = CBHDateTime::GetCurrentTimeEx() - this->m_oTimerForAxisValueLogic;

	if (loSpan.GetTotalMinutes()>=g_SaveDataIntervalForNewDCAlarmLogic)
	{
		return TRUE;
	}

	return FALSE;
}

int CVibChannParamAlarm::JudgeDiselAlarm(IDBInterFace *pDB_, 
										 CDieselPlant *pDieselPlant_,
										 bool bConfirmAlarm_ ,
										 const vector<int>& avFlameOutCylinderNo_,
										 E_MACHINE_TYPE aeMachineType_)
{

	CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL != pAlarmPlant)
	{
		bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pAlarmPlant,lbRun))
		{
			this->AlarmCount(0);
			return 0; //停车不再进行报警
		}
	}
	else
	{
		this->AlarmCount(0);
		return 0;
	}

	CDieselPlant *pDieselPlant = pDieselPlant_;

	if(NULL == pDieselPlant)
	{
		CString lstrError;
		lstrError.Format(_T("Plant:%s Diesel Plant Type Config Error"),pAlarmPlant->GetPlantNo().c_str());
		zdlTrace(lstrError);
		return 0;
	}

	int lnAlarmCount = CConfigHelper::GetAlarmCount();
	int lnDataSource = pAlarmPlant->GetPlantInfo()._iDataSource;

	int lnAlarmCylinderNo = -1;

	if((lnDataSource & GE_MACHINE_DATA_SOURCE_WIRELESS) == GE_MACHINE_DATA_SOURCE_WIRELESS)
	{
		lnAlarmCount = 1;
		lnAlarmCylinderNo = -2;
	}	

	std::vector<DATATYPE_WAVE> lvWave;
	std::vector<DATATYPE_WAVE> lvWaveSegInfo;
	std::vector<float>         lvfWave;

	int lnSmpNums=0;

	lvWaveSegInfo.resize(GD_CUSTOMPARAM_BUFFSIZE*2);

	float  lfCharValue = 0;

	int lnResult = 0;

	int lnTwoCyclePtNum = 0;

	int lnSampleFreq = 0;

	lnSmpNums = this->GetCharValue()._iSmpNum;

	if(lnSmpNums <= 0)
	{
		return 0;
	}

	CWaveHead lsWaveHead;

	memcpy(&lvWaveSegInfo.front(),(&this->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);

	lnSmpNums = this->GetCharValue()._iSmpNum;
	lvWave.resize(lnSmpNums);
	memcpy(&lvWave.front(),this->GetCurrWavePoint(),lnSmpNums*sizeof(DATATYPE_WAVE));

	lsWaveHead = this->WaveHead();

	lfCharValue = _pCharValueBuf[_iCurrCharValueIndex]._fCharAll;
	lnTwoCyclePtNum = _pCharValueBuf[_iCurrCharValueIndex]._fCharHalf;

	lnSampleFreq = _pCharValueBuf[_iCurrCharValueIndex]._iSmpFreq;

	lvfWave.resize(lnSmpNums);

	pAlarmPlant->ConvertDTWaveToFloat(&lvfWave.front(),&lvWave.front(),lnSmpNums,lsWaveHead);

	int lnLowPressRev = pAlarmPlant->GetLowPressRev();
	int lnHighPressRev = pAlarmPlant->GetHighPressRev();

	int lnPower = pAlarmPlant->GetPlantPower();


	CString lstrTmp;

	if(aeMachineType_ == MACHINE_TYPE_GASTURBINE)
	{
		lstrTmp.Format("%d+%d+%d",_pCharValueBuf[_iCurrCharValueIndex]._iRev,lnLowPressRev,lnHighPressRev);
	}
	else if(aeMachineType_ == MACHINE_TYPE_ROTATE)
	{
		lstrTmp.Format("%d+%d",_pCharValueBuf[_iCurrCharValueIndex]._iRev,lnSampleFreq);
	}
	else
	{
		lstrTmp.Format("%d+%d",_pCharValueBuf[_iCurrCharValueIndex]._iRev,lnPower);
	}
	

	// 设置设备信息
	pDieselPlant->SetPlantInfo(pAlarmPlant->GetGroupName().c_str(),pAlarmPlant->GetPlantInfo());

	//设置测点信息
	pDieselPlant->SetChannInfo(this->_channInfo);

	//设置波形
	//pDieselPlant->SetWaveData(lvWave);

	//设置各个缸波形分段信息
	pDieselPlant->SetWaveSegInfo(lvWaveSegInfo);

	//设置瞬时转速加速度判断的失火缸号
	pDieselPlant->SetFlameOutCylinder(avFlameOutCylinderNo_);

	pDieselPlant->SetWaveData(&lvfWave.front());

	pDieselPlant->SetWavePtNum(lvfWave.size());

	pDieselPlant->SetTowCycleWavePtNum(lnTwoCyclePtNum);

	pDieselPlant->SetSampleFreq(lnSampleFreq);

	BEGIN_ERROR_HANDLE

	lnResult = pDieselPlant->JudgeVibDieselAlarm(lfCharValue,lstrTmp,lnAlarmCylinderNo,lfCharValue);

	END_ERROR_HANDLE

	if(lnResult > 0)
	{
		int lnTmpCount = this->IncreaseAlarmCount();

		if(lnTmpCount >= lnAlarmCount)
		{
			this->AlarmCount(lnAlarmCount);
		}
		else
		{
			lnResult = 0;

		}
	}
	else
	{
		this->AlarmCount(0);
		lnResult = 0;

	}

	if(lnResult > 0)
	{
		if(this->IsAlarmConfirmed())
		{
			GE_ALARM_CONFIRM_TYPE  lnConfirType = this->AlarmConfirmType();

			if (ALARM_CONFIRM_TYPE_ONCE == lnConfirType)//仅确认一次
			{			
				if (!this->JudgeRegAgainAlarmForConfirm())
				{
					this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE); 	
				}	
			}

			lnResult = 0;
		}
	}

	zdlTraceLine(_T("柴油机振动测点%s在时间%s最终判断结果%d"),this->_channInfo._cChannNo,pAlarmPlant->_timeVibDataUpdate.Format(),lnResult);

	return lnResult;

}

_CHANN_ALARM_STATUS_ CVibChannParamAlarm::PredictDieselChannAlarm(IDBInterFace * pDb_,
																  bool abPlantAlarmed,//isPlantPreviousAlarmed 当前设备是否报警
																  const vector<int>& avFlameOutCylinderNo_,
																  bool abConfirm /*=TRUE*/,
																  bool abChangeStatus/*=TRUE*/)
{

	

	_CHANN_ALARM_STATUS_ lnChannStatus = CHANN_ALARM_STATUS_HALF_NORMAL;

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (!pPlant)
	{
		return lnChannStatus;
	}

	if (lnChannStatus>this->InnerAlarmStatus())
	{
		lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
	}

	E_MACHINE_TYPE leMachineType;
	CDieselPlant *pDieselPlant = pPlant->GetDieselPlantByType(leMachineType);


	if(NULL == pDieselPlant)
	{
		CString lstrError;
		lstrError.Format(_T("Plant:%s Diesel Plant Type Config Error"),pPlant->GetPlantNo().c_str());
		zdlTrace(lstrError);
		return CHANN_ALARM_STATUS_NORMAL;
	}

	_timeCurr=pPlant->_timeCurr;

	int lnAlamrType = 0;

	if(_channInfo._chAlarmParam._bRegular)
	{

		//1.判断是否常规报警
		float lfCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

		lnAlamrType=JudgeDiselAlarm(pDb_,pDieselPlant,abConfirm,avFlameOutCylinderNo_,leMachineType);

		int iType = 0;

		switch(lnAlamrType)
		{
		case E_NORMAL:
			iType = 0;
			break;
		case E_FAULT_TYPE_WATT_ABRASION:
		case E_FAULT_TYPE_OTHER_LOW:
			iType = 1;
			break;
		case E_FAULT_TYPE_FLAMEOUT:
		case E_FAULT_TYPE_CYLINDER_KNOCK:
		case E_FAULT_TYPE_OTHER_HIGH:
			iType = 2;
			break;
		default:
			iType = 0;
			break;

		};
		
		S_ChannInfo sChannInfo=_channInfo;			

		if (GE_AXIALCHANN==sChannInfo._iChannType)
		{	
			lfCurrValue=_pCharValueBuf[_iCurrCharValueIndex]._fPhaseThree;
		}

		if (iType>0)
		{

			int lnPreviousChannRegStatus = m_nRegStatus;

			float lfLastAlarmValue = this->LastAlarmValueEx();

			float lfCurrAlarmValue = _pCharValueBuf[_iCurrCharValueIndex]._fCharAll;

			this->LastAlarmValueEx(_pCharValueBuf[_iCurrCharValueIndex]._fCharAll);
			
			this->LastAlarmValue(_pCharValueBuf[_iCurrCharValueIndex]._fCharAll);

			float lfLastMaxValue = this->LastDieselAlarmValue();

			int lnAlarmStatus = 0;
			int lnOtherAlarmStatus = 0;
			//5.更新测点报警状态
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
				lnOtherAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
			}
			else
			{	
				lnAlarmStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
				lnOtherAlarmStatus=D_BHKD_AT_FAST|_iAlarmBit;

			}
			if (iType==2)
			{   
				lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGHIGH;
				lnOtherAlarmStatus=D_BHKD_AT_FAST|_iAlarmBit;

			}
			else
			{	lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGLOW;
			lnOtherAlarmStatus=D_BHKD_AT_FAST|_iAlarmBit;

			}

			this->_iFastStatus = lnOtherAlarmStatus|D_BHKD_AT_REGLOW;

			//6.至少是旧报警，且不大于上次报警值
			lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_LOW;

			BOOL lbIsNewAlarm = FALSE;

			if (CHANN_ALARM_STATUS_INIT == this->InnerAlarmStatus())
			{
				lbIsNewAlarm = TRUE;
			}

			if (this->JustStarted())
			{
				lbIsNewAlarm = TRUE;
				this->JustStarted(FALSE);

			}


			if(lfCurrAlarmValue - lfLastMaxValue  > G_ALARM_THRESHOLD*lfLastMaxValue/100.0)
			{
				lbIsNewAlarm = TRUE;
				this->LastAlarmTime(_timeCurr);
				this->LastDieselAlarmValue(lfCurrAlarmValue);
			}

			if (lbIsNewAlarm )
			{
				lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
			}
		}	
		else
		{
			this->_iFastStatus = 0;			
		}
	}
	else
	{
		this->_iFastStatus = 0;
	}


	KD_DeleteSinglePoint(pDieselPlant);
	this->PredictedChannInnerAlarmStatus(lnChannStatus);

	return lnChannStatus;
}

CBHDateTime CVibChannParamAlarm::LastAlarmTime()
{
	return this->m_dtLastAlarmTime;
}

void CVibChannParamAlarm::LastAlarmTime( const CBHDateTime atAlarmTime_ )
{
	this->m_dtLastAlarmTime = atAlarmTime_;
}


int CVibChannParamAlarm::AlarmCount()
{
	int lnAlarmCount = 0;
	this->_pLock->BeginRead();
	lnAlarmCount = this->m_nAlarmCount;
	this->_pLock->EndRead();
	return lnAlarmCount;
}

void CVibChannParamAlarm::AlarmCount( const int anAlarmCount_ )
{
	this->_pLock->BeginWrite();
	this->m_nAlarmCount = anAlarmCount_;
	this->_pLock->EndWrite();
}

int CVibChannParamAlarm::IncreaseAlarmCount()
{
	int lnAlarmCount = 0;
	this->_pLock->BeginWrite();
	this->m_nAlarmCount += 1;
	lnAlarmCount = this->m_nAlarmCount;
	this->_pLock->EndWrite();
	return lnAlarmCount;
}

int CVibChannParamAlarm::DecreaseAlarmCount()
{

	int lnAlarmCount = 0;
	this->_pLock->BeginWrite();
	this->m_nAlarmCount -= 1;
	lnAlarmCount = this->m_nAlarmCount;
	this->_pLock->EndWrite();
	return lnAlarmCount;
}

void CVibChannParamAlarm::SetMultiPointStatus( _CHANN_ALARM_STATUS_ nStatus_ )
{
	this->m_nMultiPointStatus = nStatus_;
}

_CHANN_ALARM_STATUS_ CVibChannParamAlarm::GetMultiPointStatus()
{
	return this->m_nMultiPointStatus;
}

E_ALARM_LEVEL CVibChannParamAlarm::GetAlarmLevel()
{	
	return this->m_nAlarmLevel;
}

void CVibChannParamAlarm::SetAlarmLevel( const E_ALARM_LEVEL nAlarmLevel_ )
{
	this->m_nAlarmLevel = nAlarmLevel_;
}

void CVibChannParamAlarm::LastCompareAlarmTime( const CBHDateTime atAlarmTime_ )
{
	this->m_dtLastCompareAlarmTime = atAlarmTime_;
}

CBHDateTime CVibChannParamAlarm::LastCompareAlarmTime()
{
	return this->m_dtLastCompareAlarmTime;
}

_CHANN_ALARM_STATUS_ CVibChannParamAlarm::JudgeAlarmByOtherPlant( IDBInterFace * pDb_, float fCharValue_, CBHDateTime tUpdateTime_, const map<CBHDateTime,float>& mCharValue_, bool abConfirm /*= TRUE*/, bool abChangeStatus /*= TRUE*/ )
{
	CAlarmPlantInfo* pAlarmPlantInfo = (CAlarmPlantInfo*)_pParent;

	bool lbRun = pAlarmPlantInfo->GetVibJudgeRunStatus()||pAlarmPlantInfo->GetProcJudgeRunStatus();
	if (!::IsPlantRunning(*pAlarmPlantInfo,lbRun)) //停车处理
	{
		this->_iSlowStatus = 0;
		this->SetMultiPointStatus(CHANN_ALARM_STATUS_NORMAL);

		this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
		return CHANN_ALARM_STATUS_NORMAL;
	}

	_CHANN_ALARM_STATUS_ lnAlarmStatus = this->PredictedChannInnerAlarmStatus();

	if(this->_channInfo._chAlarmParam._bRegular == false)
	{
		this->_iSlowStatus = 0;
		this->SetMultiPointStatus(CHANN_ALARM_STATUS_NORMAL);

		this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
		return CHANN_ALARM_STATUS_NORMAL;
	}


	CString lstrChannNo = this->GetChannInfo()._cChannNo;

	float lfRatio = CConfigHelper::GetAlarmRatio(lstrChannNo);

	vector<float> lvCharValue;

	int lnSize = mCharValue_.size();

	for(map<CBHDateTime,float>::const_iterator it = mCharValue_.begin(); it != mCharValue_.end(); ++it)
	{
		COleDateTimeSpan ltTimeSpan;

		if(it->first > tUpdateTime_)
		{
			ltTimeSpan = it->first - tUpdateTime_;
		}
		else
		{
			ltTimeSpan = tUpdateTime_ - it->first;
		}

		if(ltTimeSpan.GetTotalSeconds() < 30)
		{

			switch(_channInfo._chAlarmParam._iRegJudgeType)
			{
			case E_ALARMCHECK_LOWPASS:
				if(it->second < _channInfo._chAlarmParam._fAlarmHL)
				{
					lvCharValue.push_back(it->second);
				}
				break;
			case E_ALARMCHECK_HIGHPASS:
				if(it->second > _channInfo._chAlarmParam._fAlarmLH)
				{
					lvCharValue.push_back(it->second);
				}
				break;
			default:
				if(it->second < _channInfo._chAlarmParam._fAlarmHL)
				{
					lvCharValue.push_back(it->second);
				}
			}

		}
	}

	if(lvCharValue.size() <= 1)
	{

		if(lnAlarmStatus == CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_HALF_NORMAL);
		}
		else
		{
			this->PredictedChannInnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
		}

		this->_iSlowStatus = 0;
		this->SetMultiPointStatus(lnAlarmStatus);
		return lnAlarmStatus;
	}

	float lfMeanValue = 0;

	if(lvCharValue.size() < 4)
	{
		lfMeanValue = this->CalcMeanValue(lvCharValue);
	}
	else
	{
		lfMeanValue = this->CalcMeanValue(lvCharValue);

		map<float,float> lmTmpValue;

		int lnSize = lvCharValue.size();

		for(size_t t = 0; t < lnSize; ++t)
		{
			lmTmpValue.insert(make_pair(fabs(lvCharValue[t] - lfMeanValue),lvCharValue[t]));
		}

		lvCharValue.clear();

		for(map<float,float>::iterator it = lmTmpValue.begin(); it != lmTmpValue.end(); ++it)
		{
			lvCharValue.push_back(it->second);

			if(lvCharValue.size() >= 0.75*lnSize)
			{
				break;
			}
		}

		lfMeanValue = this->CalcMeanValue(lvCharValue);

	}

	float lfAlarmThreshold = 0;

	CBHDateTime ltLastAlarmTime = this->LastCompareAlarmTime();

	CBHDateTime ltCurrTime = pAlarmPlantInfo->_timeVibDataUpdate;

	COleDateTimeSpan ltTimeSpan = ltCurrTime - ltLastAlarmTime;

	bool lbAlarm = false;

	switch(_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		lfAlarmThreshold = _channInfo._chAlarmParam._fAlarmHH;
		lbAlarm = fCharValue_ - lfMeanValue > lfRatio*(lfAlarmThreshold - lfMeanValue)?true:false;
		break;
	case E_ALARMCHECK_HIGHPASS:
		lfAlarmThreshold = _channInfo._chAlarmParam._fAlarmLL;
		lbAlarm =  lfMeanValue - fCharValue_ > lfRatio*(lfMeanValue - lfAlarmThreshold)?true:false;
		break;
	default:
		lfAlarmThreshold = _channInfo._chAlarmParam._fAlarmHH;
		lbAlarm = fCharValue_ - lfMeanValue > lfRatio*(lfAlarmThreshold - lfMeanValue)?true:false;
	}
	if(lbAlarm)
	{
		int lnChannAlarmStatus = 0;

		{
			//5.更新测点报警状态
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnChannAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_SLOW;
			}
			else
			{	
				lnChannAlarmStatus=D_BHKD_AT_SLOW|_iAlarmBit;
			}

			lnChannAlarmStatus=lnChannAlarmStatus|D_BHKD_AT_REGLOW;

			this->_iSlowStatus = lnChannAlarmStatus;

			float lfLastAlarmValue = this->LastAlarmValueEx();

			this->LastAlarmValue(fCharValue_);
			this->LastAlarmValueEx(fCharValue_);


			CString lstrLog;

			CAlarmPlantInfo *pPlant = (CAlarmPlantInfo*)_pParent;

			lstrLog.Format("TrendAlarm-OtherPlant:%s,%s,%f,%f,%f",pPlant->GetPlantNo().c_str(),this->GetChannInfo()._cChannID,fCharValue_,lfMeanValue,lfAlarmThreshold);


			OutputDebugString(lstrLog);

			if(ltTimeSpan.GetTotalMinutes() > 60*24*7 || this->JudgeRegResetPlantAlarmTimer(fCharValue_,lfLastAlarmValue))
			{
				this->LastCompareAlarmTime(ltCurrTime);
				lnAlarmStatus = CHANN_ALARM_STATUS_NEW_ALARM;
			}
			else
			{
				lnAlarmStatus = CHANN_ALARM_STATUS_OLD_ALARM_LOW;
			}
		}

	}
	else
	{
		this->_iSlowStatus = 0;
	}

	this->SetMultiPointStatus(lnAlarmStatus);

	this->PredictedChannInnerAlarmStatus(lnAlarmStatus);

	return lnAlarmStatus;
}

float CVibChannParamAlarm::CalcMeanValue( const vector<float>& vData )
{
	float lfMeanValue = 0;

	if(vData.size() <= 0)
	{
		return lfMeanValue;
	}

	for(size_t t = 0; t < vData.size(); ++t)
	{
		lfMeanValue += vData[t];
	}

	return lfMeanValue/vData.size();
}

void CVibChannParamAlarm::SetChannAlarmLevel( int eAlarmStatus_ )
{
	if(eAlarmStatus_ <= CHANN_ALARM_STATUS_HALF_NORMAL)
	{
		this->SetAlarmLevel(ALARM_NORMAL);
		return;
	}

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	float lfLastAlarmValue = this->LastAlarmValueEx();

	CString lstrUnit = lsChannInfo._cUnit;

	unsigned int iAlarmState = this->GetAlarmStatus();

	int lnStatus = ParseAlarmStatus(iAlarmState);

	switch(lsChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:

		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
			else
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue <= 1.2*lsChannInfo._chAlarmParam._fAlarmLH)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	default:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	}
}

float CVibChannParamAlarm::LastDieselAlarmValue() const
{
	return this->m_fLastDieselAlarmValue;
}

void CVibChannParamAlarm::LastDieselAlarmValue( float val )
{
	this->m_fLastDieselAlarmValue = val;
}

bool CVibChannParamAlarm::IsSignalError()
{
	CAlarmPlantInfo *pAlarmPlant = (CAlarmPlantInfo*)this->_pParent;

	if (NULL != pAlarmPlant)
	{
		bool lbRun = pAlarmPlant->GetVibJudgeRunStatus()||pAlarmPlant->GetProcJudgeRunStatus();
		if (!::IsPlantRunning(*pAlarmPlant,lbRun))
		{
			return false; //停车不再进行判断
		}
	}

	S_PlantInfo lsPlantInfo = pAlarmPlant->GetPlantInfo();

	if(lsPlantInfo._iMachineType != GE_MACHINETYPE_COMPR && lsPlantInfo._iMachineType != GE_MACHINETYPE_KEYPUMP)
	{
		return false;
	}

	CString lstrUnit = this->GetChannInfo()._cUnit;
	CString lstrChannNo = this->GetChannInfo()._cChannNo;

	if(pAlarmPlant== NULL || (lstrUnit.CompareNoCase("um") != 0 && lstrUnit.CompareNoCase("m/s2") != 0 && lstrUnit.CompareNoCase("mm/s")!=0))
	{
		return false;
	}

	int lnSmpNums=0;
	CWaveHead lsWaveHead;
	_pLock->BeginRead();
	BEGIN_ERROR_HANDLE
	lnSmpNums = this->GetCharValue()._iSmpNum;
	lsWaveHead = this->WaveHead();
	END_ERROR_HANDLE
	_pLock->EndRead();

	if(lnSmpNums <= 0)
	{
		return false;
	}
	
	std::vector<DATATYPE_WAVE> lvWave;
	std::vector<float>         lvfWave;

	float  lfCharValue = 0;

	int lnResult = 0;

	int lnSampleFreq = 0;

	CString lstrAccChannNo;
	if(lstrUnit.CompareNoCase("mm/s") == 0)
	{
		int lnPos = 0;
		bool lbFind = false;
		if(lstrChannNo.Find("v")>0)
		{
			lnPos = lstrChannNo.Find("v");
		}
		else
		{
			if(lstrChannNo.Find("V")>0)
			{
				lnPos = lstrChannNo.Find("V");
			}
		}

		if(lnPos > 0)
		{
			CString lstrChannNoPre = lstrChannNo.Left(lnPos);

			for(int n = 0; n < pAlarmPlant->_vVibChann.size(); n++)
			{
				lstrAccChannNo = lstrChannNoPre+"a";
				if(lstrAccChannNo.Compare(pAlarmPlant->_vVibChann[n]->_channInfo._cChannNo) == 0 && strcmp(pAlarmPlant->_vVibChann[n]->_channInfo._cUnit,"m/s2") == 0)
				{
					_pLock->BeginRead();
					BEGIN_ERROR_HANDLE

					lnSmpNums = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._iSmpNum;
					lvWave.resize(lnSmpNums);
					memcpy(&lvWave.front(),((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCurrWavePoint(),lnSmpNums*sizeof(DATATYPE_WAVE));
					lfCharValue = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._fCharAll;
					lnSampleFreq = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._iSmpFreq;

					END_ERROR_HANDLE
					_pLock->EndRead();
					lbFind = true;
					break;
				}
				lstrAccChannNo = lstrChannNoPre+"A";
				if(lstrAccChannNo.Compare(pAlarmPlant->_vVibChann[n]->_channInfo._cChannNo) == 0 && strcmp(pAlarmPlant->_vVibChann[n]->_channInfo._cUnit,"m/s2") == 0)
				{
					_pLock->BeginRead();
					BEGIN_ERROR_HANDLE

					lnSmpNums = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._iSmpNum;
					lvWave.resize(lnSmpNums);
					memcpy(&lvWave.front(),((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCurrWavePoint(),lnSmpNums*sizeof(DATATYPE_WAVE));
					lfCharValue = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._fCharAll;
					lnSampleFreq = ((CVibChannParamAlarm*)pAlarmPlant->_vVibChann[n])->GetCharValue()._iSmpFreq;

					END_ERROR_HANDLE
					_pLock->EndRead();
					lbFind = true;
					break;
				}
			}
		}

		if(!lbFind)
		{
			return false;
		}
	}
	else
	{
		_pLock->BeginRead();
		BEGIN_ERROR_HANDLE

		lnSmpNums = this->GetCharValue()._iSmpNum;
		lvWave.resize(lnSmpNums);
		memcpy(&lvWave.front(),this->GetCurrWavePoint(),lnSmpNums*sizeof(DATATYPE_WAVE));
		lfCharValue = _pCharValueBuf[_iCurrCharValueIndex]._fCharAll;
		lnSampleFreq = _pCharValueBuf[_iCurrCharValueIndex]._iSmpFreq;

		END_ERROR_HANDLE
		_pLock->EndRead();
	}


	lvfWave.resize(lnSmpNums);

	pAlarmPlant->ConvertDTWaveToFloat(&lvfWave.front(),&lvWave.front(),lnSmpNums,lsWaveHead);

	CString lstrIP;
	int     lnPort = 0;

	bool lbSignalError = false;

	if(CConfigHelper::GetDiagnoseInterface(lstrIP,lnPort))
	{
		CFixCommA *pFixCommA = new CFixCommA;
		BEGIN_ERROR_HANDLE
		if(pFixCommA->Link(lstrIP,lnPort))
		{
			pFixCommA->CreateHead(30005);
			pFixCommA->SetItem(FIELD_SYSTEM_COMPANY,lsPlantInfo._cCompany);
			pFixCommA->SetItem(FIELD_SYSTEM_FACTORY,lsPlantInfo._cFactory);
			pFixCommA->SetItem(FIELD_SYSTEM_PLANT,lsPlantInfo._cPlantID);
			pFixCommA->SetItem(FIELD_SYSTEM_CHANN,this->GetChannInfo()._cChannID);

			pFixCommA->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char*)&lvfWave.front(),lnSmpNums*sizeof(float));
			pFixCommA->SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM,lnSmpNums);
			pFixCommA->SetItem(FIELD_SYSTEM_SAMPLE_FREQ,lnSampleFreq);

			pFixCommA->SetItem(FIELD_SERVICE_PLANT_TYPE,lsPlantInfo._iMachineType);

			pFixCommA->SetItem(16048,0);

			pFixCommA->SetItem(FIELD_SERVICE_CHANN1_STATUS,0);

			CString lstrMessage;


			if(pFixCommA->More())
			{

				bool lbSucc = (bool)pFixCommA->GetLong(FIELD_SERVICE_CHANN1_STATUS);

				CString lstrDescription = pFixCommA->Get(FIELD_SYSTEM_STATUS_DISCRIPTION).c_str();

				int lnID = pFixCommA->GetLong(FIELD_SYSTEM_ID);

				switch (lnID)
				{
				case signal_error:
					lbSignalError = true;
					break;
				default:
					lbSucc = 1;
				}

				CString lstrTime;
				lstrTime.Format("%d-%d-%d-%d-%d-%d-%d",pAlarmPlant->_timeVibDataUpdate.GetYear(),pAlarmPlant->_timeVibDataUpdate.GetMonth(),pAlarmPlant->_timeVibDataUpdate.GetDay(),pAlarmPlant->_timeVibDataUpdate.GetHour(),pAlarmPlant->_timeVibDataUpdate.GetMinute(),pAlarmPlant->_timeVibDataUpdate.GetSecond(),pAlarmPlant->_timeVibDataUpdate.GetMillisecond());
				lstrMessage.Format("Diagnose Result-%s-%s-%s-%s-%s-%d",lsPlantInfo._cCompany,lsPlantInfo._cFactory,lsPlantInfo._cPlantID,this->GetChannInfo()._cChannNo,lstrTime,lnID);

				OutputDebugString(lstrMessage);

			}

		}
		END_ERROR_HANDLE
		
		pFixCommA->Close();
		delete pFixCommA;
		
		if(lbSignalError)
		{

			return true;
		}

	}

	return false;
}
/************************************************************************/
/* 动态测点开始                                                         */
/************************************************************************/

BOOL CDynChannParamAlarm::InitValues()
{
	if (!this->_pLock)
	{
		return FALSE;

	}
	this->_pLock->BeginWrite();

	BEGIN_ERROR_HANDLE;
	
	BOOL lbRet = TRUE;
	float lfInitValue = .0;
	switch(this->_channInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_HIGHPASS:
		lfInitValue =  (this->_channInfo._chAlarmParam._fAlarmLH);		
		break;
	case E_ALARMCHECK_BANDPASS:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmHL);		
		break;
	case E_ALARMCHECK_BANDSTOP:
		lfInitValue = (this->_channInfo._chAlarmParam._fAlarmLL);
		break;
	}

	this->LastMaxAlarmValue(lfInitValue);
	this->LastAlarmValueEx(lfInitValue);
	this->LastEverageAlarmValue(G_F_INIT_VALUE);
	this->_fDynRegValueLast = G_F_INIT_VALUE;
	this->NewAlarmValue(lfInitValue);
	if (GE_RODSINKCHANN == this->_channInfo._iChannType)
	{
		float lfOriginal = this->GetValueForNewDCLogic();
		this->DCOriginal(lfOriginal);
	}
	this->JustStarted(TRUE);
	END_ERROR_HANDLE;

	this->_pLock->EndWrite();

	return TRUE;
}

BOOL CDynChannParamAlarm::IsEigenValueEmpty(S_DynCharValue & arefSVibCharValue)
{
	float val = 0;

	for (int i = 0 ; i <= 5 ;i++)
	{
		if(this->GetChannInfo()._iChannType == GE_RODSINKCHANN && (i == 0 || i == 3))
		{
			continue;
		}

		val = arefSVibCharValue.GetSpecTypeData(i);

		if (!BHKD::IsFloatZero(val))
		{
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CDynChannParamAlarm::IsNeedComputeEigenValue(S_DynCharValue & arefSVibCharValue)
{
	if (IsMWComputeFeatureValueEnabled())
	{
		return this->IsEigenValueEmpty(arefSVibCharValue);
	}else
	{
		return FALSE;
	}
}

BOOL CDynChannParamAlarm::ComputeEigenValue(int aEnumMachineType,int anIndex)
{
	try
	{
		S_DynCharValue & loValue =   _pCharValueBuf[anIndex];

		if (this->IsNeedComputeEigenValue(loValue))
		{
			//真实计算
			EigenValueComputer::ComputeDynEigenValue(
				loValue,
				this->_channInfo,
				loValue._iRev,
				(DATATYPE_WAVE *)this->GetCurrWavePoint(),
				loValue._iSmpNum,
				(MACHINETYPE)aEnumMachineType,
				(CHANNTYPE)this->_channInfo._iChannType
				);
		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}

	return TRUE;
}

BOOL CDynChannParamAlarm::ComputeEigenValue(int aEnumMachineType)
{
	try
	{
		S_DynCharValue & loValue =   _pCharValueBuf[GetCurrCharValueIndex()];

		if (this->IsNeedComputeEigenValue(loValue))
		{
			//真实计算
			EigenValueComputer::ComputeDynEigenValue(
														loValue,
														this->_channInfo,
														loValue._iRev,
														(DATATYPE_WAVE *)this->GetCurrWavePoint(),
														loValue._iSmpNum,
														(MACHINETYPE)aEnumMachineType,
														(CHANNTYPE)this->_channInfo._iChannType
														);
		}
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

	}

	return TRUE;
}

float CDynChannParamAlarm::ComputeThreePhaseCharValue(S_DynCharValue  & arefSVibCharValue)
{
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL == pPlant)
	{
		ASSERT(FALSE);

		return 0;
	}

	float lfVal = EigenValueComputer::ComputeAxisEigenValue(    this->_channInfo,
																(DATATYPE_WAVE *)this->GetCurrWavePoint(),
																arefSVibCharValue._iRev,
																arefSVibCharValue._iSmpFreq,
																arefSVibCharValue._iSmpNum,
																(MACHINETYPE)pPlant->GetPlantInfo()._iMachineType,
																(CHANNTYPE)this->_channInfo._iChannType
																);

	arefSVibCharValue._fCharValue8 = lfVal;

	return lfVal;
}




float CDynChannParamAlarm::ComputeAllCharValue(S_DynCharValue & arefSVibCharValue)
{

	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (NULL == pPlant)
	{
		ASSERT(FALSE);

		return 0;
	}

	float lfVal = EigenValueComputer::ComputeFirstEigenValue(this->_channInfo,
															 (DATATYPE_WAVE *)this->GetCurrWavePoint(),
															 arefSVibCharValue._iSmpNum,
															 (MACHINETYPE)pPlant->GetPlantInfo()._iMachineType,
															 (CHANNTYPE)this->_channInfo._iChannType
															 );

	arefSVibCharValue._fCharValue0 = lfVal;

	return lfVal;
}

float CDynChannParamAlarm::LastAlarmValue() const
{
	return m_fLastAlarmValue;
}

void CDynChannParamAlarm::LastAlarmValue( float val )
{
	m_fLastAlarmValue = val;
}

float CDynChannParamAlarm::LastAlarmValueEx() const
{
	return m_fLastAlarmValueEx;
}

void CDynChannParamAlarm::LastAlarmValueEx( float val )
{
	m_fLastAlarmValueEx = val;
}

void CDynChannParamAlarm::SaveCurrentAsLastAlarmValue()
{
	ASSERT(this->_iCurrCharValueIndex >=0 && this->_iCurrCharValueIndex < D_ALARM_SAVECHAR_NUM);

	this->LastAlarmValue(this->_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0));
}

int CDynChannParamAlarm::JudgeRegAlarmEx( IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_ )
{
	iFlag_=-1;
	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
	_timeCurr=pPlant->_timeCurr;
	if(_channInfo._chAlarmParam._bRegular)
	{
		int iType=JudgeRegDataAlarm();

		CAlarmPlantInfo *pAlarmPlant=(CAlarmPlantInfo*)_pParent;

		this->ComputeEigenValue(pAlarmPlant->GetPlantInfo()._iMachineType);

		float fDynCurrValue=_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0);


		if (iType>0)
		{
			_iRegStatus=D_BHKD_AT_REGULAR|_iAlarmBit;

			if (iType==2)
			{	
				_iRegStatus=_iRegStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	_iRegStatus=_iRegStatus|D_BHKD_AT_REGLOW;
			}

			this->m_fLastAlarmValue = fDynCurrValue;

			/*if(_iRegAlarmFlag<0)*/
			if(1)
			{
				S_ChannInfo sChannInfo=_channInfo;
				COleDateTimeSpan dtSpan;
				dtSpan=_timeCurr-_timeDynNewAlarmLast;
				int iTotaldays=dtSpan.GetTotalDays();

				if (iTotaldays<D_ALARM_REG_DAYTIME)
				{
					bool bRtn=JudgeRegAgainAlarm(fDynCurrValue,_fDynRegValueLast);

					if (bRtn)
					{
						CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;
						COleDateTimeSpan dtSpan;
						dtSpan=_timeCurr-(pPlant->_dtVibRegDataSaveOne);
						int iTotalMinutes=dtSpan.GetTotalMinutes();

						if (iTotalMinutes>=D_ALARM_REG_INTVTIME)
						{
							bSaveOneAlarmData_=true;

						}
						else
						{	

						}

					}
					else
					{
						if (-1 == _iRegAlarmFlag)
						{
							if(AddRegAlarmLog(pDb_)<0)
							{
								CString strInfo;
								strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
								CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
								_iRegStatus=0;

							}else
							{
								if (this->JudgeRegResetPlantAlarmTimer(fDynCurrValue,_fDynRegValueLast))
								{
									bSaveAlarmDataFlag_ = true;
									iFlag_=1;

								}else
								{
									bSaveOneAlarmData_ = true;
								}

							}

						}else
						{

							if (this->JudgeRegResetPlantAlarmTimer(fDynCurrValue,_fDynRegValueLast))
							{
								bSaveAlarmDataFlag_ = true;
								iFlag_=1;

							}else
							{
								bSaveOneAlarmData_ = true;
							}

						}

						_timeDynNewAlarmLast=_timeCurr;
						_fDynRegValueLast=fDynCurrValue;
						_iRegAlarmFlag=1;

					}
				}
				else
				{
					if(AddRegAlarmLog(pDb_)<0)
					{
						CString strInfo;
						strInfo.Format("%s-%s 振动添加常规报警日志失败",pPlant->GetPlantID().c_str(),_channInfo._cChannID);
						CHZLogManage::Error(strInfo,"CVibChannParamAlarm","JudgeRegAlarm");
						_iRegStatus=0;

					}else
					{
						bSaveAlarmDataFlag_ = true;
					}
					_timeDynNewAlarmLast=_timeCurr;
					_fDynRegValueLast=fDynCurrValue;
					_iRegAlarmFlag=1;
					iFlag_=1;

				}
			}
		}
		else
		{
			_iRegStatus=0;

			if (_iRegAlarmFlag==1)
			{
				_iRegAlarmFlag=0;
				_fDynRegValueLast=fDynCurrValue;
				HandleRegAlarmStop(pDb_);
			}
		}
	}
	else
	{
		if(_iRegStatus>0)
		{			
			HandleRegAlarmStop(pDb_);
		}
		_iRegStatus=0;
		_iRegAlarmFlag=-1;
	}
	return _iRegStatus;
}

float CDynChannParamAlarm::LastMaxAlarmValue() const
{
	long lnData = .0;
	InterlockedExchange(&lnData,this->m_fLastMaxAlarmValue);
	float lfData = int_as_float(lnData);
	return lfData;
}

void CDynChannParamAlarm::LastMaxAlarmValue( float val )
{
	if (IsFloatZero(G_F_INIT_VALUE - val))
	{
		ASSERT(FALSE);
		return;
	}
	long lnData = float_as_int(val);
	InterlockedExchange(&this->m_fLastMaxAlarmValue,lnData);	
	
}

float CDynChannParamAlarm::LastEverageAlarmValue() const
{
	return m_fLastEverageAlarmValue;
}

void CDynChannParamAlarm::LastEverageAlarmValue( float val )
{
	m_fLastEverageAlarmValue = val;
}

float CDynChannParamAlarm::updateEverageAlarmValue( float afLastAlarmValue )
{
	//1.更新均值
	float lfCurrentEverageValue = this->LastEverageAlarmValue();
	if (IsFloatZero(lfCurrentEverageValue - G_F_INIT_VALUE))
	{
		lfCurrentEverageValue = afLastAlarmValue;
	}
	lfCurrentEverageValue = (lfCurrentEverageValue+afLastAlarmValue)/2;
	this->LastEverageAlarmValue(lfCurrentEverageValue);

	//2.判断是否需要更新最大报警值
	CAlarmPlantInfo * lpParent = (CAlarmPlantInfo *)this->_pParent;
	if (NULL != lpParent)
	{
		COleDateTimeSpan lpSpan = this->_timeCurr - lpParent->m_oLastNewAlarmTime;
		if (lpSpan.GetTotalDays()>= lpParent->m_nNewAlarmMaxTime)
		{
			this->LastMaxAlarmValue(lfCurrentEverageValue);
		}
	}

	return lfCurrentEverageValue;
}

_CHANN_ALARM_STATUS_ CDynChannParamAlarm::PredictChannAlarm( IDBInterFace * pDb_, 
															bool abPlantAlarmed,
															bool abConfirm /*= TRUE*/,
															bool abChangeStatus /*= TRUE*/)
{
	_CHANN_ALARM_STATUS_ lnChannStatus = CHANN_ALARM_STATUS_HALF_NORMAL;


	CAlarmPlantInfo *pPlant=(CAlarmPlantInfo*)_pParent;

	if (!pPlant)
	{
		return lnChannStatus;
	}


	if (lnChannStatus>this->InnerAlarmStatus())
	{
		lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
	}

	_timeCurr=pPlant->_timeCurr;	

	if(_channInfo._chAlarmParam._bRegular)
	{

		//1.判断是否常规报警
		int iType=JudgeRegDataAlarm(abConfirm);
		this->ComputeEigenValue(pPlant->GetPlantInfo()._iMachineType);
		float lfCurrValue=_pCharValueBuf[_iCurrCharValueIndex].GetSpecTypeData(0);

		float lfCurrValueOriginal = lfCurrValue;

		//1.1新的活塞杆沉降报警逻辑使用累计磨损量报警而不是绝对值
		if (g_UseNewDCAlarmLogic&&g_UseNewAlarmLogic)
		{
			if (GE_RODSINKCHANN == this->_channInfo._iChannType)
			{	
				bool lbRun = pPlant->GetVibJudgeRunStatus()||pPlant->GetProcJudgeRunStatus();
				if (IsPlantRunning(*pPlant,lbRun))
				{
					float lfOriginalDC = this->DCOriginal();

					lfCurrValue= abs(lfCurrValue - this->DCOriginal()); 

				}else
				{
					lfCurrValue = 0;
				}
			}			 
		}

		if (iType>0)
		{
			//2.获得上次报警值
			float lfLastAlarmValue = this->LastAlarmValueEx();

			//3.更新上次报警值
			this->LastAlarmValueEx(lfCurrValue);

			this->LastAlarmValue(lfCurrValue);

			//4.更新报警平均值，并视情况更新上次报警最大值
			this->updateEverageAlarmValue(lfCurrValue);

			int lnPreviousChannRegStatus = _iRegStatus;

			int lnAlarmStatus = 0;
			//5.更新测点报警状态
			if (GE_AXIALCHANN==_channInfo._iChannType)
			{	
				lnAlarmStatus=D_BHKD_AT_AXISVIB|D_BHKD_AT_REGULAR;
			}
			else
			{	
				lnAlarmStatus=D_BHKD_AT_REGULAR|_iAlarmBit;
			}
			if (iType==2)
			{   
				lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGHIGH;
			}
			else
			{	lnAlarmStatus=lnAlarmStatus|D_BHKD_AT_REGLOW;
			}

			m_nFastRegStatus = lnAlarmStatus;

			if (abChangeStatus)
			{
				this->_iRegStatus = lnAlarmStatus;
			}

			//6.至少是旧报警，且不大于上次报警值
			lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_LOW;

			//7.判断是否大于上次报警值
			bool lbIsAgainAlarm=JudgeRegAgainAlarm(lfCurrValue,lfLastAlarmValue);	



			if (!lbIsAgainAlarm )
			{
				float lfLastMaxValue = this->LastMaxAlarmValue();
				//7.1不是重复报警则更新上次最大报警值				
				if (!JudgeRegAgainAlarm(lfCurrValue,this->LastMaxAlarmValue()))
				{
					this->LastMaxAlarmValue(lfCurrValue);
				}

				//7.2至少是旧报警，且大于上次报警值
				this->IncreaseContinuousMoreThanTimes();
				if (this->ContinuousMoreThanTimes()>=g_alarm_larger_times)
				{
					lnChannStatus = CHANN_ALARM_STATUS_OLD_ALARM_HIGH;
				}	
				

				//7.3判断是否新报警
				bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfLastMaxValue);

				if (CHANN_ALARM_STATUS_INIT == this->InnerAlarmStatus())
				{
					lbIsNewAlarm = TRUE;
				}

				if (this->JustStarted())
				{
					lbIsNewAlarm = TRUE;
					this->JustStarted(FALSE);
				}
				
				if (lbIsNewAlarm )
				{

					//7.4 新报警，且大于上次报警值
					BHLOG_ERROR(STR("lnPreviousChannRegStatus :%d 之前测点报警 lfCurrValue:%f lfLastMaxValue:%f",lnPreviousChannRegStatus,lfCurrValue,lfLastMaxValue));
					lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
					this->NewAlarmValue(lfCurrValue);

				}else
				{
					//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
					if (lnPreviousChannRegStatus)
					{
						float lfPreviousNewAlarmValue = this->NewAlarmValue();
						bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
						if (lbIsNewAlarm)
						{
							if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
							{
								lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
							}
							this->NewAlarmValue(lfCurrValue);
						}
					}
				}

			}else
			{
				//与测点本次新报警值比较，超过本次新报警值一定比例，则记为新报警，主要是为了处理缓变
				if (lnPreviousChannRegStatus)
				{
					float lfPreviousNewAlarmValue = this->NewAlarmValue();
					bool lbIsNewAlarm = this->JudgeRegResetPlantAlarmTimer(lfCurrValue,lfPreviousNewAlarmValue);
					if (lbIsNewAlarm)
					{
						if (!IsFloatZero(lfPreviousNewAlarmValue-G_F_INIT_VALUE))
						{
							lnChannStatus = CHANN_ALARM_STATUS_NEW_ALARM;
						}
						this->NewAlarmValue(lfCurrValue);
					}
				}
				
				this->ResetContinuousMoreThanTimes();				
				
			}
		}
		else
		{	
			this->ResetContinuousMoreThanTimes();
			_iRegStatus=0;			
		}
	}

	this->PredictedChannInnerAlarmStatus(lnChannStatus);

	return lnChannStatus;
}

void CDynChannParamAlarm::ResetHalfNormalTimer5()
{
	this->m_oHalfNormalTimer5 = this->_timeCurr;
}



_CHANN_ALARM_STATUS_ CDynChannParamAlarm::InnerAlarmStatus() const
{
	return m_nInnerAlarmStatus;
}

void CDynChannParamAlarm::InnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nInnerAlarmStatus = val;
}

int CDynChannParamAlarm::SwitchAlarmStatus( IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus )
{
	int lbNewAlarmLogAdded = FALSE;

	if (CHANN_ALARM_STATUS_NORMAL == anNewStatus)
	{
		if (this->InnerAlarmStatus()>anNewStatus)
		{
			this->HandleRegAlarmStop(pDb_);
		}
		this->ResetHalfNormalTimer5();
		this->InnerAlarmStatus(anNewStatus);
		this->ResetContinuousMoreThanTimes();

		if(!IsFloatZero(this->LastEverageAlarmValue() - G_F_INIT_VALUE))
		{
			this->LastMaxAlarmValue(this->LastEverageAlarmValue());
		}
		

	}else if (CHANN_ALARM_STATUS_HALF_NORMAL == anNewStatus)
	{
		//this->LastMaxAlarmValue(this->LastEverageAlarmValue());
		//从报警到正常
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			this->ResetHalfNormalTimer5();
			this->InnerAlarmStatus(CHANN_ALARM_STATUS_HALF_NORMAL);
			this->ResetContinuousMoreThanTimes();

		}

		//之前是半正常状态
		else if (CHANN_ALARM_STATUS_HALF_NORMAL == this->InnerAlarmStatus())
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;

			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

			if (NULL!=lpAlarmPlantInfo)
			{
				if (lpSpan.GetTotalHours()>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(CHANN_ALARM_STATUS_NORMAL);
					this->HandleRegAlarmStop(pDb_);
				}
			}
		}else
		{
			ASSERT(FALSE);
		}
		//新状态是报警状态
	}else 
	{
		//旧状态是报警状态
		if (this->InnerAlarmStatus()> CHANN_ALARM_STATUS_HALF_NORMAL)
		{
			if (CHANN_ALARM_STATUS_NEW_ALARM == anNewStatus)
			{
				this->HandleRegAlarmStop(pDb_);
				this->AddRegAlarmLog(pDb_);
				lbNewAlarmLogAdded = TRUE;			
			}else 
			{
				if (this->PlantNewAlarmOccoured())
				{
					this->AddRegAlarmLog(pDb_);
					lbNewAlarmLogAdded = TRUE;	
				}
			}

			this->InnerAlarmStatus(anNewStatus);

			
			//旧状态是半正常状态
			//旧状态是半正常状态，新状态不是新报警状态
		}else if (this->InnerAlarmStatus()== CHANN_ALARM_STATUS_HALF_NORMAL 
			&& anNewStatus!= CHANN_ALARM_STATUS_NEW_ALARM)
		{
			COleDateTimeSpan lpSpan =  this->_timeCurr - this->m_oHalfNormalTimer5;			
			CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

			if (NULL!=lpAlarmPlantInfo)
			{
				int lnHours = lpSpan.GetTotalHours();
				if (lnHours>= lpAlarmPlantInfo->m_nChannHalfNormalTime)
				{
					this->ResetHalfNormalTimer5();
					this->InnerAlarmStatus(anNewStatus);
					this->HandleRegAlarmStop(pDb_);
				}else
				{
					this->InnerAlarmStatus(anNewStatus);
				}
			}

		}else//旧状态是正常状态，或者新报警状态
		{
			this->HandleRegAlarmStop(pDb_);
			this->AddRegAlarmLog(pDb_);
			lbNewAlarmLogAdded = TRUE;	
			this->InnerAlarmStatus(anNewStatus);
		}
	}

	return lbNewAlarmLogAdded;
}

_CHANN_ALARM_STATUS_ CDynChannParamAlarm::PredictedChannInnerAlarmStatus() const
{
	return m_nPredictedChannInnerAlarmStatus;
}

void CDynChannParamAlarm::PredictedChannInnerAlarmStatus( _CHANN_ALARM_STATUS_ val )
{
	m_nPredictedChannInnerAlarmStatus = val;
}

int CDynChannParamAlarm::HandleRegAlarmStopEx( IDBInterFace * pDB_ )
{
	return this->HandleRegAlarmStop(pDB_);
}

int CDynChannParamAlarm::ContinuousMoreThanTimes() const
{
	return m_nContinuousMoreThanTimes;
}

void CDynChannParamAlarm::ContinuousMoreThanTimes( int val )
{
	m_nContinuousMoreThanTimes = val;
}

void CDynChannParamAlarm::ResetContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes=0;
}

void CDynChannParamAlarm::IncreaseContinuousMoreThanTimes()
{
	this->m_nContinuousMoreThanTimes++;
}

BOOL CDynChannParamAlarm::PlantNewAlarmOccoured() const
{
	return m_bPlantNewAlarmOccoured;
}

void CDynChannParamAlarm::PlantNewAlarmOccoured( BOOL val )
{
	m_bPlantNewAlarmOccoured = val;
}

int CDynChannParamAlarm::GetChannRegAlarmType()
{
	if(isLocalJudgeAlarm())
	{
		return this->_iRegStatus;
	}
	return this->m_nFastRegStatus;
	
}

float CDynChannParamAlarm::DCOriginal() const
{
	return m_fDCOriginal;
}

void CDynChannParamAlarm::DCOriginal( float val )
{
	m_fDCOriginal = val;
}

void CDynChannParamAlarm::ResetDCOriginal()
{
	float lfDataValue = this->GetCharValue()._fCharValue0;
	this->DCOriginal(lfDataValue);
}

BOOL CDynChannParamAlarm::SaveDataForNewDCLogic()
{
	int lnRet = FALSE;

	BEGIN_ERROR_HANDLE;
	
	if (NULL == this->_pParent)
	{
		return FALSE;
	}

	CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;

	BOOL lbIsPlantRevMoreThanThrehold = ::IsPlantRevThanGiven(*lpAlarmPlantInfo,g_rev_threshold_sink);

	if (!lbIsPlantRevMoreThanThrehold)
	{
		return FALSE;
	}

	 DATATYPE_WAVE * lpWavePoint = ( DATATYPE_WAVE *)this->GetCurrWavePoint();
	 int lnWavePoint = this->GetCharValue()._iSmpNum;
	 lnWavePoint = lpAlarmPlantInfo->GetSafeWavePointNum(lnWavePoint);

	if ((NULL == lpWavePoint) || (  0 == lnWavePoint))
	{
		return FALSE;
	}
	
	this->InitFileNameForNewDCAlarmLogic();

	int lnRet1 = writefile(lpWavePoint,lnWavePoint,this->WaveFilePath());

	float lfData = this->DCOriginal();
	int lnRet2 = writefile(&lfData,1,this->ValueFilePath());
	
	int lnDataNum = lnWavePoint;

	int lnRet3 = writefile(&lnDataNum,1,this->WaveFileNum());

	this->DCOriginal(lfData);

	 lnRet = lnRet1&&lnRet2;

	if (!lnRet)
	{
		ASSERT(FALSE);
	}
	
	END_ERROR_HANDLE;

	return lnRet;
}	

BOOL CDynChannParamAlarm::GetBufferForNewDCLogic( std::vector<DATATYPE_WAVE> & arefWaveBuffer )
{
	int lnReaded = FALSE;

	BEGIN_ERROR_HANDLE;
	
	this->InitFileNameForNewDCAlarmLogic();
	int lnFileSize = 0;	
	
	readfile(&lnFileSize,1,this->m_strWaveFileNum);

	arefWaveBuffer.clear();
	arefWaveBuffer.resize(lnFileSize);

	DATATYPE_WAVE * lpBuffer = &arefWaveBuffer.front();
	
	 lnReaded = readfile(lpBuffer,lnFileSize,this->m_strWaveFilePath); 
	
	END_ERROR_HANDLE;

	return lnReaded;
}

BOOL CDynChannParamAlarm::JudgeAndDoForNewDCLogic( BOOL abRunningStatusChanged )
{
	BOOL lbRet = FALSE;

	BEGIN_ERROR_HANDLE;

	if (NULL == this->_pParent)
	{
		return FALSE;
	}
	CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;
	
	if (abRunningStatusChanged)
	{

		std::vector<DATATYPE_WAVE> loBufferOld;
		std::vector<DATATYPE_WAVE> loBufferOldSubDC;
		std::vector<DATATYPE_WAVE> loWaveBufferNewSubDC;
		BOOL lbReadSucceed = TRUE;

		lbReadSucceed = this->GetBufferForNewDCLogic(loBufferOld);
		
		if (lbReadSucceed && loBufferOld.size()>0)
		{	

			int lnWaveBufferOldSize = loBufferOld.size();
			loBufferOldSubDC.resize(lnWaveBufferOldSize);

			DATATYPE_WAVE * lpWaveBufferOld = &loBufferOld.front();
			DATATYPE_WAVE * lpWaveBufferOldSubDC = &loBufferOldSubDC.front();	

			lpAlarmPlantInfo->SubtractWaveDC(lpWaveBufferOld,
											 lpWaveBufferOldSubDC,
											 lnWaveBufferOldSize);

			
			
			DATATYPE_WAVE * lpWaveBufferNew = (DATATYPE_WAVE *)this->GetCurrWavePoint();
			int lnWaveBufferNewSize = this->GetCharValue()._iSmpNum;
			loWaveBufferNewSubDC.resize(lnWaveBufferNewSize);
			DATATYPE_WAVE * lpWaveBufferNewSubDC = &loWaveBufferNewSubDC.front();
			
			lpAlarmPlantInfo->SubtractWaveDC(   lpWaveBufferNew,
												lpWaveBufferNewSubDC,
												lnWaveBufferNewSize);
			//判断波形是否相似
			 lbRet = this->IsWaveAlike(lpWaveBufferOldSubDC,
										   lnWaveBufferOldSize,
										   lpWaveBufferNewSubDC,
										   lnWaveBufferNewSize);

			if (!lbRet)
			{
				this->ResetDCOriginal();
				this->SaveDataForNewDCLogic();

			}
		}else
		{
			 lbRet = this->SaveDataForNewDCLogic();	

			 if (lbRet)
			 {
				 this->ResetTimerForNewDCLogic();
			 }
			 
		}

	}else
	{
		if (this->IsReachSaveTimeIntervalForNewDCLogic())
		{
			lbRet =this->SaveDataForNewDCLogic();
			if (lbRet)
			{
				 this->ResetTimerForNewDCLogic();
			}
			
		}	
	}
	
	END_ERROR_HANDLE;
	return lbRet;
}

BOOL CDynChannParamAlarm::IsWaveAlike( DATATYPE_WAVE * apWaveBuffer1,int anWavebuffer1,
									  DATATYPE_WAVE * apWaveBuffer2,int anWavebuffer2 )
{
	if (NULL == apWaveBuffer1 || NULL == apWaveBuffer2 || anWavebuffer1<=0 || anWavebuffer2<=0)
	{
		return FALSE;
	}
	
	if (anWavebuffer1> anWavebuffer2)
	{
		anWavebuffer1 = anWavebuffer2;
	}
	
	double ldblSumX = 0;
	double ldblSumY = 0;

	double ldblSumX2 = 0;
	double ldblSumY2 = 0;

	double ldblSum2X = .0;
	double ldblSum2Y = .0;
	
	double ldblSumXY = 0;

	double ldblNum = .0;
	double ldblDen = .0;

	double ldblCoeff = .0;
	
	for (int i=0;i<anWavebuffer1;i++)
	{
		ldblSumX += apWaveBuffer1[i];
		ldblSumY += apWaveBuffer2[i];

		ldblSumX2 += apWaveBuffer1[i]*apWaveBuffer1[i];
		ldblSumY2 += apWaveBuffer2[i]*apWaveBuffer2[i];

		ldblSumXY+= apWaveBuffer1[i]*apWaveBuffer2[i];
	}

	ldblSum2X = ldblSumX*ldblSumX;
	ldblSum2Y = ldblSumY*ldblSumY;

	ldblNum = ldblSumXY - (ldblSumX*ldblSumY)/anWavebuffer1;
	ldblDen = (ldblSumX2-ldblSum2X/anWavebuffer1)*(ldblSumY2-ldblSum2Y/anWavebuffer1);

	ldblDen = sqrt(ldblDen);

	double ldblResult = (ldblNum/ldblDen)*100;

	if (ldblResult>= g_wave_diff_ratio)
	{
		return TRUE;
	}

	return FALSE;
}

CString CDynChannParamAlarm::WaveFilePath() const
{
	return m_strWaveFilePath;
}

void CDynChannParamAlarm::WaveFilePath( CString val )
{
	m_strWaveFilePath = val;
}

CString CDynChannParamAlarm::ValueFilePath() const
{
	return m_strValueFilePath;
}

void CDynChannParamAlarm::ValueFilePath( CString val )
{
	m_strValueFilePath = val;
}

BOOL CDynChannParamAlarm::InitFileNameForNewDCAlarmLogic()
{

	if (this->m_strWaveFilePath.GetLength()==0)
	{
		if (NULL == this->_pParent)
		{
			return FALSE;

		}
		CAlarmPlantInfo * lpAlarmPlantInfo = (CAlarmPlantInfo *)this->_pParent;
		CString lstrGroupName = lpAlarmPlantInfo->GetGroupName().c_str();
		CString lstrCompanyName = lpAlarmPlantInfo->GetCompanyName().c_str();
		CString lstrFactoryName = lpAlarmPlantInfo->GetFactoryName().c_str();
		CString lstrPlantNo = lpAlarmPlantInfo->GetPlantNo().c_str();
		CString lstrChannNo = this->GetChannInfo()._cChannNo;

		CString lstrPath=::GetAppPath()+_T("data\\");
		CString lstrPath2 ;
		CString lstrPath3;
		CString lstrDirectory ;


		const TCHAR * lpPathSplit = _T("\\");
		const TCHAR * lpFileExt = _T(".txt");
		const TCHAR * lpFileWave = _T("_wave");
		const TCHAR * lpFileValue = _T("_val");
		const TCHAR * lpFileNum = _T("_num");

		lstrPath.Append(lstrGroupName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrCompanyName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrFactoryName);
		lstrPath.Append(lpPathSplit);

		lstrPath.Append(lstrPlantNo);
		lstrPath.Append(lpPathSplit);

		lstrDirectory.Append(lstrPath);
		

		lstrPath.Append(lstrChannNo);

		lstrPath2 = lstrPath;
		lstrPath3 = lstrPath;

		lstrPath.Append(lpFileWave);
		lstrPath.Append(lpFileExt);

		lstrPath2.Append(lpFileValue);
		lstrPath2.Append(lpFileExt);

		lstrPath3.Append(lpFileNum);
		lstrPath3.Append(lpFileExt);

		this->WaveFilePath(lstrPath);
		this->ValueFilePath(lstrPath2);
		this->WaveFileNum(lstrPath3);

		MakeSureFolderExist(lstrDirectory);
	}
	
	return TRUE;
}

float CDynChannParamAlarm::GetValueForNewDCLogic()
{
	this->InitFileNameForNewDCAlarmLogic();

	int lnFileSize = 1;	

	float lfData = .0;

	int lnReaded = readfile(&lfData,lnFileSize,this->m_strValueFilePath);

	if (lnReaded<=0)
	{
		return G_F_INIT_VALUE;
	}

	return lfData;
}

CBHDateTime CDynChannParamAlarm::TimerForNewDCLogic() const
{
	return m_oTimerForNewDCLogic;
}

void CDynChannParamAlarm::TimerForNewDCLogic( CBHDateTime val )
{
	m_oTimerForNewDCLogic = val;
}

void CDynChannParamAlarm::ResetTimerForNewDCLogic()
{
	this->m_oTimerForNewDCLogic = CBHDateTime::GetCurrentTimeEx();
}

BOOL CDynChannParamAlarm::IsReachSaveTimeIntervalForNewDCLogic()
{
	COleDateTimeSpan loSpan = CBHDateTime::GetCurrentTimeEx() - this->m_oTimerForNewDCLogic;

	if (loSpan.GetTotalMinutes()>=g_SaveDataIntervalForNewDCAlarmLogic)
	{
		return TRUE;
	}

	return FALSE;
}

CString CDynChannParamAlarm::WaveFileNum() const
{
	return m_strWaveFileNum;
}

void CDynChannParamAlarm::WaveFileNum( CString val )
{
	m_strWaveFileNum = val;
}

E_ALARM_LEVEL CDynChannParamAlarm::GetAlarmLevel()
{
	return this->m_nAlarmLevel;
}

void CDynChannParamAlarm::SetAlarmLevel( const E_ALARM_LEVEL nAlarmLevel_ )
{
	this->m_nAlarmLevel = nAlarmLevel_;
}

void CDynChannParamAlarm::SetChannAlarmLevel( int eAlarmStatus_ )
{
	if(eAlarmStatus_ <= CHANN_ALARM_STATUS_HALF_NORMAL)
	{
		this->SetAlarmLevel(ALARM_NORMAL);
		return;
	}

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	float lfLastAlarmValue = this->LastAlarmValueEx();

	CString lstrUnit = lsChannInfo._cUnit;

	unsigned int iAlarmState = this->GetAlarmStatus();

	int lnStatus = ParseAlarmStatus(iAlarmState);

	switch(lsChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:

		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
			else
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	case E_ALARMCHECK_HIGHPASS:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue <= 1.2*lsChannInfo._chAlarmParam._fAlarmLH)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	default:
		if(lnStatus == 1)
		{
			this->SetAlarmLevel(ALARM_URGENT);
		}
		else if(lnStatus == 2)
		{
			this->SetAlarmLevel(ALARM_DANGEOUS);
		}
		else if(lnStatus == 3)
		{
			if(lfLastAlarmValue < 0.8*lsChannInfo._chAlarmParam._fAlarmHL)
			{
				this->SetAlarmLevel(ALARM_DEVELOPING);
			}
			else
			{
				this->SetAlarmLevel(ALARM_POTENTIAL);
			}
		}
		else if(lnStatus == 4)
		{
			this->SetAlarmLevel(ALARM_COMPARE_ABNORMAL);
		}
		else
		{
			this->SetAlarmLevel(ALARM_NORMAL);
		}
		break;
	}
}