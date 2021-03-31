#include "stdafx.h"
#include "AlarmPlantInfo.h"
#include "PlantInfo.h"
#include <additions/DebugHelper.h>
#include <additions/CTimeOutReadWriteLock.h>
#include <MiddleWare.h>


CAlarmPlantInfo::CAlarmPlantInfo(void)
{
	CHZLogManage::Info("construction","CAlarmPlantInfo","CAlarmPlantInfo");
	_bFirstRun         = true;
	_bJudgeAlarm       = false;
	_timeFirstRun      = CBHDateTime::GetCurrentTime();
	_iEventIdS         = -1;
	_iEventIdReg       = -1;
	_iEventIdRegLast   = -1;
	_iEventIdCustom    = -1;
	_iEventIdCustomLast= -1;
	_pLockAlarmEventID = new CTimeOutReadWriteLock();
	_pLockThID         = _pLockAlarmEventID;
	_timeProcDataUpdate= g_dtInitTime;
	_timeVibDataUpdate = g_dtInitTime;

	_timeVibSlowFirstSave = CBHDateTime::GetCurrentTime();
	_timeVibSlowSaveData  = CBHDateTime::GetCurrentTime();
	_timeVibSlowThLearn   = CBHDateTime::GetCurrentTime();

	_dtVibRegDataSaveOne  = CBHDateTime::GetCurrentTime();

	_dtVibCustomFirstSave = CBHDateTime::GetCurrentTime();
	_dtVibCustomDataSave  = CBHDateTime::GetCurrentTime();

	_iSyncID              = -1;

	this->m_nProcJudgedRunStatus = 0;

	this->m_nVibJudgedRunStatus = 0;

	_iEventIDF            = -1;


	this->MaxRealRev(0);
	this->MaxRealRevLast(this->MaxRealRev());

	//初始化报警参数
	this->m_oHalfNormalTime =  CBHDateTime::GetCurrentTime();
	this->m_oLastNewAlarmTime =  CBHDateTime::GetCurrentTime();
	this->m_oLastSaveHalfIndenseTime =  CBHDateTime::GetCurrentTime();
	this->m_oLastSaveSparseTime =  CBHDateTime::GetCurrentTime();


	this->m_nNewAlarmMaxTime = g_nNewAlarmMaxTime;					    //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
	this->m_nSaveIndenseTime = g_nSaveIndenseTime;						//密集保存时间间隔,								默认5分钟
	this->m_nSaveHalfIndenseTime = g_nSaveHalfIndenseTime;				//半密集保存时间间隔,							默认30秒
	this->m_nHalfNormalNotSaveTime = g_nHalfNormalNotSaveTime;			//半正常状态下持续保存的时间长度，				默认20分钟
	this->m_nSaveSparseTime = g_nSaveSparseTime;						//就报警每30分钟保存一组的时间间隔,				默认30分钟
	this->m_nHalfNormalTime = g_nHalfNormalTime;						//半正常状态维持时间间隔						默认2个小时
	this->m_nChannHalfNormalTime = g_nChannHalfNormalTime;				//测点半正常状态维持时间间隔					默认1个小时

	
	this->InnerPlantAlarmStatus(PLANT_STATUS_NORMAL);

	this->AlarmHasOccoured(FALSE);

	//初始化保存时间参数
	this->_timeNetOffSaveVibDay = CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveVibHour = CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveVibMonth =  CBHDateTime::GetCurrentTime();

	this->_timeNetOffSaveDynDay= CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveDynHour= CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveDynMonth= CBHDateTime::GetCurrentTime();

	this->_timeNetOffSaveProcDay= CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveProcHour= CBHDateTime::GetCurrentTime();
	this->_timeNetOffSaveProcMonth = CBHDateTime::GetCurrentTime();

	this->_timeVibDataUpdate =  CBHDateTime::GetCurrentTime();
	this->_timeDynDataUpdate =  CBHDateTime::GetCurrentTime();
	this->_timeProcDataUpdate = CBHDateTime::GetCurrentTime();
	this->_timeCurr = CBHDateTime::GetCurrentTime();
	this->_timeCurrDyn = CBHDateTime::GetCurrentTime();
	this->_timeCurrProc = CBHDateTime::GetCurrentTime();
	this->_timeCurrVib = CBHDateTime::GetCurrentTime();

	this->_pRealPlantInfo = NULL;

	//直流量报警逻辑
	this->IsEnabledNewDCLogic(g_UseNewDCAlarmLogic);

	BOOL lbPlantIsRunning = TRUE;

	this->CurrentRunningStatus(lbPlantIsRunning);
	this->LastRunningStatus(lbPlantIsRunning);

	this->_vVibWaveField.clear();
	this->_vDynWaveField.clear();

	this->NewAlarmAddLog(false);

	this->m_hLibHandle = NULL;

	this->m_fPower = 0;
	this->m_nLowPressRev = 0;
	this->m_nHighPressRev = 0;
}

CAlarmPlantInfo::~CAlarmPlantInfo(void)
{
	KD_DeletePoint(_pWave);
	KD_DeletePoint(_pWaveDyn);
	KD_DeleteSinglePoint(_pLockAlarmEventID);
	this->_vVibWaveField.clear();
	this->_vDynWaveField.clear();
	if(this->m_hLibHandle)
	{
		::FreeLibrary(this->m_hLibHandle);
	}
	//   KD_DeletePoint(_pLockThID);
}

void CAlarmPlantInfo::JudgeAlarm(IDBInterFace *pDB_)
{
	_timeCurr=CBHDateTime::GetCurrentTime();

	COleDateTimeSpan span;

	_sTimeCurr=TimeConvertString(_timeCurr);

	//判断是否首次更新数据，因为缓变报警和快变报警要在首次更新数据后24小时开始判断
	if (_bFirstRun)
	{
		_bFirstRun=false;
		_timeFirstRun=_timeCurr;
	}
	else
	{
		if (g_UseNewAlarmLogic)
		{
			if (g_UseNewDCAlarmLogic||g_UseNewAxisAlarmLogic)
			{
				this->DealWithNewDCLogic();
			}



			this->JudgeRegAlarmEx(pDB_);

		}else
		{
			//振动与动态常规报警
			JudgeVibRegAlarm(pDB_);
		}

		//振动与动态自定义报警
		JudgeVibCustomAlarm(pDB_);
		//判断动态活塞杆轴心位置报警
		JudgeAxisLocationAlarm(pDB_);


		if (g_UseNewAlarmLogic)
		{
			this->JudgeProcAlarmEx(pDB_);
		}else
		{
			//过程量振动常规报警
			JudgeProcAlarm(pDB_);
		}
		
	}
}

float CAlarmPlantInfo::GetSpecChannRev(string strChann_,float &fRealRev_)
{
	float fRatingRev=.0;
	V_PCHANNINFO::iterator it;
	S_ChannInfo chanInfo;
	for(it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		chanInfo=(*it)->GetChannInfo();
		if(strcmp(strChann_.c_str(),chanInfo._cChannID)==0)
		{
			//转速类型是按照1~5写的，而不是0~4
			fRealRev_ = GetSpecTypeRealRev( chanInfo._iRevType, true);
			if ((chanInfo._iRevType>0)&&(chanInfo._iRevType<6))
				fRatingRev=_sPlantInfo._piRev[chanInfo._iRevType-1];
			else
				fRatingRev=_sPlantInfo._piRev[0];
			return fRatingRev;
		}
	}
	return -1;
}

int CAlarmPlantInfo::JudgeProcAlarm(IDBInterFace *pDB_,bool bSaveData_ /* = true */,bool bConfirmAlarm_ /* = true */)
{
	bool bUpdate = false;
	
	_pLockDataUpdateStatues->BeginRead();
	bUpdate = _bProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bProcDataUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
	{	
		return 0;
	}

	int iTmp = 0,iFlag = 0;

	_pLockProcData->BeginWrite();
	
	try
	{
		int nLoop = -1;

		for (V_PCHANNINFO::iterator it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			++nLoop;

			if(!bSaveData_ && 
				_vProcFastChangeChannNo.size() > nLoop &&
				_vProcFastChangeChannNo[nLoop] == 0)
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,((CProcChannParamAlarm*)(*it))->GetChannRegAlarmType(),!bSaveData_);
				continue;
			}

			int iTmp = ((CProcChannParamAlarm*)(*it))->JudgeAlarm(pDB_,bSaveData_,bConfirmAlarm_);

			SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,((CProcChannParamAlarm*)(*it))->GetChannRegAlarmType(),!bSaveData_);

			if(iTmp > 0)
			{
				++iFlag;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndWrite();

	return iFlag;
}

void CAlarmPlantInfo::JudgeAxisLocationAlarm(IDBInterFace * pDb_)
{
	bool bUpdate=false;
	
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=(_bVibDataUpdate || _bDynDataUpdate);
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bVibDataUpdate = false;
		_bDynDataUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
	{	
		return;
	}

	if(_sPlantInfo._iMachineType==GE_MACHINETYPE_RC)
	{
		_pLockVibData->BeginWrite();
	
		try
		{
			for (V_PAXISCHANNINFO::iterator itAxes = _vAxisChann.begin();itAxes!=_vAxisChann.end();++itAxes)
			{
				((CAxisAlarmChannParam*)(*itAxes))->JudgeDynRegAlarm(pDb_);
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndWrite(); 
	}
}

BOOL CAlarmPlantInfo::IsAlaramTimeLimitReached(const CBHDateTime & arefLastFirstSaveAllTime,
											   const CBHDateTime & arefLastSaveAllTime,
											   const CBHDateTime & arefNow)
{
	COleDateTimeSpan dtSpan;

	dtSpan=arefNow-arefLastFirstSaveAllTime;
 
	int iTotalMins=dtSpan.GetTotalMinutes();

	if (iTotalMins>=D_ALARM_CONTSAVETIME)
	{
		if(iTotalMins>=D_ALARM_INTVSAVETIME)
		{
			return FALSE;
		}

		dtSpan=arefNow-arefLastSaveAllTime;

		if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
		{	
			return FALSE;
		}
	}

	return TRUE;
}

int CAlarmPlantInfo::JudgeVibRegAlarm(IDBInterFace *pDb_,bool bSaveData_ /* = true */,bool bConfirmAlarm_ /* = true */)
{
	V_PCHANNINFO::iterator itVib;
	V_PCHANNINFO::iterator itDyn;


	bool bUpdate=false;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=(_bVibDataUpdate || _bDynDataUpdate);
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bVibDataUpdate = false;
		_bDynDataUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
	{	
		return 0;
	}
	
	CBHDateTime ltTime = CBHDateTime::GetCurrentTime();
	
	bool lbSaved = false;

	int lnAlarmCount = 0;

	//保存当前设备的上一次eventid的值;
	int lnOriginalRegAlarmEventID = this->_iEventIdReg;
	
	//是否重置报警逻辑图左侧判断逻辑的开关,注释掉则不重置左侧报警逻辑，不注释则重置左侧报警逻辑
	lnOriginalRegAlarmEventID = -1;
	
	//判断是否有振动通道和动态通道，第一种情况：只有振动通道
	if (_vVibChann.size()>0 && _vDynChann.size()==0)
	{
		bool bSaveOnlyOneAlarmDataRtn=false; //是否需要保存一条
		bool bSaveAllAlarmDataFlag=false;    //是否需要全部保存
		int iTmp=0,iFlag=0,iCharIndex=0,iWaveIndex=0,iFirstFlag=-1;
		
		int nLoop = -1;
		//进入振动通道循环，判断报警，快变报警判断只针对快变通道和非轴位移通道判断
		for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
		{
			
			++nLoop;

			if((!bSaveData_ && 
				_vVibFastChangeChannNo.size() > nLoop &&
				_vVibFastChangeChannNo[nLoop] == 0)
				||(!bSaveData_ && (*itVib)->GetChannInfo()._iChannType == GE_AXIALCHANN)
				||(!bSaveData_ && _vVibFastChangeChannNo.size() <= 0))
			{
				CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

				if (NULL != lpVib)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!bSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!bSaveData_);
				}
				continue;
			}
			
			BOOL lbShouldJudge = TRUE;

			_pLockVibData->BeginWrite();

			try
			{
				//测点的特征值全为0则不参与报警判断
				CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);


				if (NULL != lpVib)
				{
					S_VibCharValue loValue = lpVib->GetCharValue();

					if (!lpVib->IsNeedComputeEigenValue(loValue))
					{
						if (lpVib->IsEigenValueEmpty(loValue))
						{
							lbShouldJudge = FALSE;
						}
					}
				}				
				
				if (lbShouldJudge)
				{
					iTmp=((CVibChannParamAlarm*)(*itVib))->JudgeRegAlarm(pDb_,
																		iFirstFlag,
																		bSaveAllAlarmDataFlag,
																		bSaveOnlyOneAlarmDataRtn,
																		bConfirmAlarm_);

					//快变报警判断保存测点报警状态
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!bSaveData_);
					
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!bSaveData_);
					
				}
				

				iCharIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrCharValueIndex();
				iWaveIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}
			_pLockVibData->EndWrite();

			if (!lbShouldJudge)
			{
				continue;
			}

			if(iTmp>0)
			{
				++iFlag;
				
				++lnAlarmCount;
				
				if(iFirstFlag>0 && (-1 == lnOriginalRegAlarmEventID))
				{
					//如果有新的报警产生，就重新触发新的保存开始
					if(bSaveData_)
					{
						_dtVibRegFirstSave=_timeCurr;
					}
					
				}
			}
		}
		//重置报警状态,快变报警判断没有判断所有测点状态，不重置
		if (iFlag<1 && bSaveData_)
		{
			if(_iEventIdReg>0)
			{
				//重置振动通道报警状态
				for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
				{
					
					_pLockVibData->BeginWrite();
					try
					{
						((CVibChannParamAlarm*)(*itVib))->ResetRegAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}
					_pLockVibData->EndWrite();
				}
		
				_iEventIdReg=-1;
					
			}

			if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				float pfRealRev[5];
				int iSmpNums = 0, iSmpFreq = 0;
				GetRealSmpInfo( iSmpNums, iSmpFreq, true);
				GetRealRev( pfRealRev, 5);

				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginWrite();

				try
				{
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,lnEventId,iCharIndex,iWaveIndex,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime,D_BHKD_AT_REGULAR);
						_dtVibRegDataSave =_timeCurr;
					}
						
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}
				_pLockVibData->EndWrite();

			}
		}
		else
		{
			float pfRealRev[5];
			int iSmpNums = 0, iSmpFreq = 0;
			GetRealSmpInfo( iSmpNums, iSmpFreq, true);
			GetRealRev( pfRealRev, 5);

			//新的报警状态开始
			if (bSaveAllAlarmDataFlag)
			{				
				//保存常规报警波形数据

				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdReg;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);
					}
                     
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,lnEventId,iCharIndex,iWaveIndex,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime,D_BHKD_AT_REGULAR);

						_dtVibRegDataSave =_timeCurr;
					}
					
					_iEventIdRegLast=_iEventIdReg;

					//_dtVibRegFirstSave = _timeCurr;

					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();

			}
			else if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginWrite();

				try
				{
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,lnEventId,iCharIndex,iWaveIndex,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime,D_BHKD_AT_REGULAR);
						_dtVibRegDataSave =_timeCurr;
					}
					

					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();

			}
			else if (bSaveOnlyOneAlarmDataRtn)
			{
				//相似报警的相关操作，保存一条振动报警数据
				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdRegLast;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);

						lnEventId = _iEventIdReg;

						ASSERT(lnEventId >0);
					}
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,
							lnEventId,
							iCharIndex,
							iWaveIndex,
							pfRealRev,
							this->GetSafeWavePointNum(iSmpNums),
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);
						_dtVibRegDataSaveOne=_timeCurr;
					}
					
					

				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();

				return lnAlarmCount;
			}

		}

	}
	//判断是否有振动通道和动态通道，第二种情况：只有动态通道
	else if(_vDynChann.size()>0 && _vVibChann.size()==0)
	{
		bool bSaveOnlyOneAlarmDataRtn=false; //是否需要保存一条
		bool bSaveAllAlarmDataFlag=false;    //是否需要全部保存
		int iDynTmp=0,iDynFlag=0,iDynCharIndex=0,iDynWaveIndex=0,iDynFirstFlag=-1;

		int nLoop = -1;

		//进入动态通道循环，判断报警
		for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
		{
			++nLoop;

			if((!bSaveData_ &&
				_vDynFastChangeChannNo.size() > nLoop &&
				_vDynFastChangeChannNo[nLoop] == 0)
				||(!bSaveData_ && (*itDyn)->GetChannInfo()._iChannType == GE_RODSINKCHANN)
				||(!bSaveData_ && _vDynFastChangeChannNo.size() <= 0))
			{
				CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

				if (NULL != lpDyn)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!bSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!bSaveData_);
				}
				continue;
			}
			

			BOOL lbShouldJudge = TRUE;

			_pLockVibData->BeginWrite();

			try
			{

				CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

				if (NULL != lpDyn)
				{
					S_DynCharValue & loValue = lpDyn->GetCharValue();

					if (!lpDyn->IsNeedComputeEigenValue(loValue))
					{
						if (lpDyn->IsEigenValueEmpty(loValue))
						{
							lbShouldJudge = FALSE;
						}
					}
				}

				if (lbShouldJudge)
				{
					iDynTmp=((CDynChannParamAlarm*)(*itDyn))->JudgeRegAlarm(pDb_,
																			iDynFirstFlag,
																			bSaveAllAlarmDataFlag,
																			bSaveOnlyOneAlarmDataRtn,
																			bConfirmAlarm_);

					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!bSaveData_);
					
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!bSaveData_);
					
				}

				

				iDynCharIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrCharValueIndex();
				iDynWaveIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndWrite();

			if (!lbShouldJudge)
			{
				continue;
			}

			if (iDynTmp>0)
			{
				++iDynFlag;
				
				++lnAlarmCount;
				
				if (iDynFirstFlag>0 && (-1 == lnOriginalRegAlarmEventID ))
				{	
					//如果有新的报警产生，就重新触发新的保存开始
					if(bSaveData_)
					{
						_dtVibRegFirstSave=_timeCurr;
					}
					
				}
			}
		}
		//重置报警状态
		if (iDynFlag<1 && bSaveData_)
		{
			//重置动态通道报警状态
			for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
			{
				_pLockVibData->BeginWrite();
				try
				{
					((CDynChannParamAlarm*)(*itDyn))->ResetRegAlarmStatus();
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();
			}
			
			_iEventIdReg=-1;
			
			if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				float pfRealRev[5];
				int iSmpNums = 0, iSmpFreq = 0;
				GetRealSmpInfo( iSmpNums, iSmpFreq, true);
				GetRealRev( pfRealRev, 5);

				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginRead();

				try
				{
					if(bSaveData_)
					{
						SaveDynAlarmDetailData( pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);


						_dtVibRegDataSave =_timeCurr;
					}
					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndRead();
			}
		}
		else
		{
			float pfRealRev[5];
			int iSmpNums = 0, iSmpFreq = 0;
			GetRealSmpInfo( iSmpNums, iSmpFreq, true);
			GetRealRev( pfRealRev, 5);

			//保存报警波形数据
			COleDateTimeSpan dtSpan;

			//有一个测点的报警值达到了需要保存整个设备特征值和波形的上限
			if (bSaveAllAlarmDataFlag )
			{
				//保存动态报警数据
				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdReg;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);
					}
					
					if(bSaveData_)
					{
						SaveDynAlarmDetailData(	pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);

						//_dtVibRegFirstSave = _timeCurr;

						_dtVibRegDataSave =_timeCurr;
					}
					_iEventIdRegLast=_iEventIdReg;
					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();

			} else if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginWrite();

				try
				{
					if(bSaveData_)
					{
						SaveDynAlarmDetailData( pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);


						_dtVibRegDataSave =_timeCurr;
					}
					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();
			}
			//相似报警的相关操作，保存一条动态报警数据
			else if (bSaveOnlyOneAlarmDataRtn)
			{
				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdRegLast;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);

						lnEventId = _iEventIdReg;

						ASSERT(lnEventId >0);
					}
					if(bSaveData_)
					{
						SaveDynAlarmDetailData(pDb_,
							_iEventIdRegLast,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);

						_dtVibRegDataSaveOne=_timeCurr;
					}
					

				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();

				return lnAlarmCount;
			}

		}
	}
	//判断是否有振动通道和动态通道，第三种情况：既有振动通道，也有动态通道
	else if(_vVibChann.size()>0 && _vDynChann.size()>0)
	{
		bool bSaveOnlyOneAlarmDataRtn=false,bSaveAlarmDataFlag=false;
		int iTmp=0,iFlag=0,iCharIndex=0,iWaveIndex=0,iFirstFlag=-1;

		int nLoop = -1;

		//进入振动通道循环，判断报警
		for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
		{
			++nLoop;
			if((!bSaveData_ &&
				_vVibFastChangeChannNo.size() > nLoop &&
				_vVibFastChangeChannNo[nLoop] == 0)
				||(!bSaveData_ && (*itVib)->GetChannInfo()._iChannType == GE_AXIALCHANN)
				||(!bSaveData_ && _vVibFastChangeChannNo.size() <= 0))
			{
				CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

				if (NULL != lpVib)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!bSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!bSaveData_);
				}
				continue;
			}

			BOOL lbShouldJudge = TRUE;
			_pLockVibData->BeginWrite();

			try
			{
				CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

				if (NULL != lpVib)
				{
					S_VibCharValue & loValue = lpVib->GetCharValue();

					if (!lpVib->IsNeedComputeEigenValue(loValue))
					{
						if (lpVib->IsEigenValueEmpty(loValue))
						{
							lbShouldJudge = FALSE;
						}
					}
				}

				if (lbShouldJudge)
				{
					iTmp=((CVibChannParamAlarm*)(*itVib))->JudgeRegAlarm(pDb_,
																			iFirstFlag,
																			bSaveAlarmDataFlag,
																			bSaveOnlyOneAlarmDataRtn,
																			bConfirmAlarm_);

					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!bSaveData_);
					
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!bSaveData_);
				}


				iCharIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrCharValueIndex();
				iWaveIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndWrite();


			if (!lbShouldJudge)
			{
				continue;
			}

			if(iTmp>0)
			{
				++iFlag;
				
				++lnAlarmCount;
				
				if(iFirstFlag>0 && (-1==lnOriginalRegAlarmEventID) )
				{//如果有新的报警产生，就重新触发新的保存开始
					if(bSaveData_)
					{
						_dtVibRegFirstSave=_timeCurr;
					}
					
				}
			}
		}

		/*bool bDynSaveAlarmDataFlag=false;*/
		int iDynTmp=0,iDynFlag=0,iDynCharIndex=0,iDynWaveIndex=0,iDynFirstFlag=-1;

		nLoop = -1;

		//进入动态通道循环，判断报警
		for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
		{
			
			++nLoop;

			if((!bSaveData_ &&
				_vDynFastChangeChannNo.size() > nLoop &&
				_vDynFastChangeChannNo[nLoop] == 0)
				|| (!bSaveData_ && (*itDyn)->GetChannInfo()._iChannType == GE_RODSINKCHANN)
				||(!bSaveData_ && _vDynFastChangeChannNo.size() <= 0))
			{
				CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

				if (NULL != lpDyn)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!bSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!bSaveData_);
				}
				continue;
			}

			BOOL lbShouldJudge = TRUE;
			_pLockVibData->BeginWrite();


			try
			{
				
				CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

				if (NULL != lpDyn)
				{
					S_DynCharValue & loValue = lpDyn->GetCharValue();

					if (!lpDyn->IsNeedComputeEigenValue(loValue))
					{
						if (lpDyn->IsEigenValueEmpty(loValue))
						{
							lbShouldJudge  = FALSE;
						}
					}
				}	
				
				if (lbShouldJudge)
				{
					iDynTmp=((CDynChannParamAlarm*)(*itDyn))->JudgeRegAlarm(pDb_,
																			iDynFirstFlag,
																			bSaveAlarmDataFlag,
																			bSaveOnlyOneAlarmDataRtn,
																			bConfirmAlarm_);

					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!bSaveData_);
					
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!bSaveData_);
					
				}


				iDynCharIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrCharValueIndex();
				iDynWaveIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}


			_pLockVibData->EndWrite();

			if (!lbShouldJudge)
			{
				continue;
			}

			if (iDynTmp>0)
			{
				++iDynFlag;
				
				++lnAlarmCount;
				
				if (iDynFirstFlag>0 && (-1 == lnOriginalRegAlarmEventID))
				{//如果有新的报警产生，就重新触发新的保存开始
					if(bSaveData_)
					{
						_dtVibRegFirstSave=_timeCurr;
					}
					
				}
			}
		}

		//重置报警状态
		if (iFlag<1 && iDynFlag<1 && bSaveData_)
		{
			if(_iEventIdReg>0)
			{
				//重置振动通道报警状态
				for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
				{
					
					_pLockVibData->BeginWrite();
					try
					{
						((CVibChannParamAlarm*)(*itVib))->ResetRegAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
				//重置动态通道报警状态
				for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
				{

					_pLockVibData->BeginWrite();
					try
					{
						((CDynChannParamAlarm*)(*itDyn))->ResetRegAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
				
				_iEventIdReg=-1;
				
			}

			float pfRealRev[5];
			int iSmpNums = 0, iSmpFreq = 0;
			GetRealSmpInfo( iSmpNums, iSmpFreq, true);
			GetRealRev( pfRealRev, 5);

			if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginWrite();

				try
				{
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,
							lnEventId,
							iCharIndex,
							iWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);
						//保存动态报警数据
						SaveDynAlarmDetailData(pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);


						_dtVibRegDataSave = _timeCurr; 
					}
					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();
			}
		}
		else
		{
			float pfRealRev[5];
			int iSmpNums = 0, iSmpFreq = 0;
			GetRealSmpInfo( iSmpNums, iSmpFreq, true);
			GetRealRev( pfRealRev, 5);

			if ((bSaveAlarmDataFlag))
			{
				//保存振动报警数据
				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdReg;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);
					}
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,lnEventId,iCharIndex,iWaveIndex,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime,D_BHKD_AT_REGULAR);
						//保存动态报警数据
						SaveDynAlarmDetailData(pDb_,lnEventId,iDynCharIndex,iDynWaveIndex,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime,D_BHKD_AT_REGULAR);

						//_dtVibRegFirstSave = _timeCurr;
						_dtVibRegDataSave = _timeCurr;
					}
					
					_iEventIdRegLast=_iEventIdReg;
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();

			}else if(this->IsAlaramTimeLimitReached(_dtVibRegFirstSave, _dtVibRegDataSave,_timeCurr))
			{
				int lnEventId = _iEventIdRegLast;

				if (lnEventId < 0)
				{
					ASSERT(FALSE);
				}

				_pLockVibData->BeginWrite();

				try
				{
					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,
							lnEventId,
							iCharIndex,
							iWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);
						//保存动态报警数据
						SaveDynAlarmDetailData(pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);

						_dtVibRegDataSave = _timeCurr;
					}
					
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}


				_pLockVibData->EndWrite();
			}
			else if (bSaveOnlyOneAlarmDataRtn)
			{
				//相似报警的相关操作，保存一条振动报警数据
				_pLockVibData->BeginWrite();

				try
				{
					int lnEventId = _iEventIdRegLast;

					if (lnEventId < 0)
					{
						ASSERT(FALSE);

						lnEventId = _iEventIdReg;

						ASSERT(lnEventId >0);
					}

					if(bSaveData_)
					{
						SaveAlarmDetailData(pDb_,
							lnEventId,
							iCharIndex,
							iWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);

						//相似报警的相关操作，保存一条动态报警数据
						SaveDynAlarmDetailData(	pDb_,
							lnEventId,
							iDynCharIndex,
							iDynWaveIndex,
							pfRealRev,
							iSmpNums,
							iSmpFreq,
							lbSaved,
							ltTime,
							D_BHKD_AT_REGULAR);

						_dtVibRegDataSaveOne=_timeCurr;
					}
					

				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();

				return lnAlarmCount;
			}

		}
	}
	else
	{	return 0;
	}
	return lnAlarmCount;
}

void CAlarmPlantInfo::JudgeVibCustomAlarm(IDBInterFace *pDb_)
{
	V_PCHANNINFO::iterator itVib;
	V_PCHANNINFO::iterator itDyn;
	bool bUpdate=false;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=(_bVibChannInfoUpdate || _bDynChannInfoUpdate);
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bVibChannInfoUpdate = false;
		_bDynChannInfoUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
		return;
	//CString strTmp;strTmp.Format("_vVibChann.size():%d",_vVibChann.size());BHTRACE_DEBUG(strTmp);
	//判断是否有振动通道和动态通道，第一种情况：只有振动通道
	if (_vVibChann.size()>0 && _vDynChann.size()==0)
	{
		bool bRtn=false;
		int iTmp=0,iFlag=0,iCharIndex=0,iWaveIndex=0,iFirstFlag=-1;
		//进入振动通道循环，判断报警
		for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
		{
			_pLockVibData->BeginRead();
			try
			{
				iTmp=((CVibChannParamAlarm*)(*itVib))->JudgeCustomAlarm(pDb_,iFirstFlag,bRtn);
				iCharIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrCharValueIndex();
				iWaveIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
			if(iTmp>0)
			{
				++iFlag;
				if(iFirstFlag>0)
				{//如果有新的报警产生，就重新触发新的保存开始
					_dtVibCustomFirstSave=_timeCurr;
				}
			}
		}
		//重置报警状态
		if (iFlag<1)
		{
			if(_iEventIdCustom>0)
			{
				//重置振动通道报警状态
				for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
				{
					_pLockVibData->BeginWrite();
					try
					{
						((CVibChannParamAlarm*)(*itVib))->ResetCustomAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
				_iEventIdCustom=-1;
			}
		}
		else
		{
			//CString strTmp;strTmp.Format("iFlag:%d,_iEventIdCustom:%d",iFlag,_iEventIdCustom);BHTRACE_DEBUG(strTmp);
			COleDateTimeSpan dtSpan;
			//判断是否超过了连续保存数据时间段
			dtSpan=_timeCurr-_dtVibCustomFirstSave;
			int iTotalMins=dtSpan.GetTotalMinutes();
			if (iTotalMins>D_ALARM_CONTSAVETIME)
			{
				if(iTotalMins>D_ALARM_INTVSAVETIME)
				{
					return;
				}
				dtSpan=_timeCurr-_dtVibCustomDataSave;
				if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
					return;
			}
			//保存自定义报警波形数据
			_pLockVibData->BeginRead();
			try
			{
				SaveVibCustomAlarmData(pDb_,iCharIndex,iWaveIndex);
				_dtVibCustomDataSave=_timeCurr;
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}
			_pLockVibData->EndRead();
		}
	}
	//判断是否有振动通道和动态通道，第二种情况：只有动态通道
	else if(_vDynChann.size()>0 && _vVibChann.size()==0)
	{
		bool bDynRtn=false;
		int iDynTmp=0,iDynFlag=0,iDynCharIndex=0,iDynWaveIndex=0,iDynFirstFlag=-1;
		//进入动态通道循环，判断报警
		for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
		{
			_pLockVibData->BeginRead();
			try
			{
				iDynTmp=((CDynChannParamAlarm*)(*itDyn))->JudgeCustomAlarm(pDb_,iDynFirstFlag,bDynRtn);
				iDynCharIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrCharValueIndex();
				iDynWaveIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
			if (iDynTmp>0)
			{
				++iDynFlag;
				if (iDynFirstFlag>0)
				{//如果有新的报警产生，就重新触发新的保存开始
					_dtVibCustomFirstSave=_timeCurr;
				}
			}
		}
		//重置报警状态
		if (iDynFlag<1)
		{
			//重置动态通道报警状态
			for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
			{
				_pLockVibData->BeginWrite();
				try
				{
					((CDynChannParamAlarm*)(*itDyn))->ResetCustomAlarmStatus();
				}
				catch (...)
				{
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockVibData->EndWrite();
			}
			_iEventIdCustom=-1;
		}
		else
		{
			//保存报警波形数据
			COleDateTimeSpan dtSpan;
			//判断是否超过了连续保存数据时间段
			dtSpan=_timeCurr-_dtVibCustomFirstSave;
			int iTotalMins=dtSpan.GetTotalMinutes();
			if (iTotalMins>D_ALARM_CONTSAVETIME)
			{
				if(iTotalMins>D_ALARM_INTVSAVETIME)
				{
					return;
				}
				dtSpan=_timeCurr-_dtVibCustomDataSave;
				if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
					return;
			}
			//保存自定义报警波形数据
			//保存动态报警数据
			_pLockVibData->BeginRead();
			try
			{
				SaveDynCustomAlarmData(pDb_,iDynCharIndex,iDynWaveIndex);
				_dtVibCustomDataSave =_timeCurr;
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
	}
	//判断是否有振动通道和动态通道，第三种情况：既有振动通道，也有动态通道
	else if(_vVibChann.size()>0 && _vDynChann.size()>0)
	{
		bool bRtn=false;
		int iTmp=0,iFlag=0,iCharIndex=0,iWaveIndex=0,iFirstFlag=-1;
		//进入振动通道循环，判断报警
		for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
		{
			//BHTRACE_DEBUG("开始振动自定义报警判断");
			_pLockVibData->BeginRead();
			try
			{
				iTmp=((CVibChannParamAlarm*)(*itVib))->JudgeCustomAlarm(pDb_,iFirstFlag,bRtn);
				iCharIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrCharValueIndex();
				iWaveIndex=((CVibChannParamAlarm*)(*itVib))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
			if(iTmp>0)
			{
				++iFlag;
				if(iFirstFlag>0)
				{//如果有新的报警产生，就重新触发新的保存开始
					_dtVibCustomFirstSave=_timeCurr;
				}
			}
		}

		bool bDynRtn=false;
		int iDynTmp=0,iDynFlag=0,iDynCharIndex=0,iDynWaveIndex=0,iDynFirstFlag=-1;
		//进入动态通道循环，判断报警
		for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
		{
			//BHTRACE_DEBUG("开始动态自定义报警判断");
			_pLockVibData->BeginRead();
			try
			{
				iDynTmp=((CDynChannParamAlarm*)(*itDyn))->JudgeCustomAlarm(pDb_,iDynFirstFlag,bDynRtn);
				iDynCharIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrCharValueIndex();
				iDynWaveIndex=((CDynChannParamAlarm*)(*itDyn))->GetCurrWaveIndex();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
			if (iDynTmp>0)
			{
				++iDynFlag;
				if (iDynFirstFlag>0)
				{//如果有新的报警产生，就重新触发新的保存开始
					_dtVibCustomFirstSave=_timeCurr;
				}
			}
		}
		//CString strTmp;strTmp.Format("自定义报警ID:iDynTmp:%d,iTmp:%d,iDynFlag:%d,iFlag:%d",iDynTmp,iTmp,iDynFlag,iFlag);BHTRACE_DEBUG(strTmp);
		//重置报警状态
		if (iFlag<1 && iDynFlag<1)
		{
			if(_iEventIdCustom>0)
			{
				//重置振动通道报警状态
				for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
				{
					_pLockVibData->BeginWrite();
					try
					{
						((CVibChannParamAlarm*)(*itVib))->ResetCustomAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
				//重置动态通道报警状态
				for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
				{
					_pLockVibData->BeginWrite();
					try
					{
						((CDynChannParamAlarm*)(*itDyn))->ResetCustomAlarmStatus();
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
				_iEventIdCustom=-1;
			}
		}
		else
		{
			if (bRtn || bDynRtn)//如果是在两天之内报警，则保存的报警数据时间ID为上一次的报警ID
				_iEventIdCustom=_iEventIdCustomLast;
			//BHTRACE_DEBUG("开始保存数据的时间判断");
			COleDateTimeSpan dtSpan;
			//判断是否超过了连续保存数据时间段
			dtSpan=_timeCurr-_dtVibCustomFirstSave;
			int iTotalMins=dtSpan.GetTotalMinutes();
			if (iTotalMins>D_ALARM_CONTSAVETIME)
			{
				if(iTotalMins>D_ALARM_INTVSAVETIME)
				{
					return;
				}
				dtSpan=_timeCurr-_dtVibCustomDataSave;
				if ((dtSpan.GetTotalSeconds())<D_ALARM_INTVSAVETIME_INTV) //是否满足指定间隔
					return;
			}
			//保存自定义报警波形数据
			//保存振动报警数据
			_pLockVibData->BeginRead();
			try
			{
				SaveVibCustomAlarmData(pDb_,iCharIndex,iWaveIndex);
				//CString strTmp;strTmp.Format("保存振动自定义报警数据成功：自定义报警ID:%d",_iEventIdCustom);BHTRACE_DEBUG(strTmp);
				//保存动态报警数据
				SaveDynCustomAlarmData(pDb_,iDynCharIndex,iDynWaveIndex);
				//CString strTmp;strTmp.Format("保存动态自定义报警数据成功：自定义报警ID:%d",_iEventIdCustom);BHTRACE_DEBUG(strTmp);
				_dtVibCustomDataSave =_timeCurr;
				_iEventIdCustomLast=_iEventIdCustom;
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
	}
	else
		return;
}

void CAlarmPlantInfo::JudgeVibSlowAlarm(IDBInterFace *pDB_)
{
	return;
	V_PCHANNINFO::iterator it;
	int iFlag=0,iTmp=0;
	_pLockVibData->BeginWrite();
	try
	{
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			//CHZLogManage::Debug("Start JudgeVibSlowAlarm","CAlarmPlantInfo","JudgeVibSlowAlarm");
			iTmp=((CVibChannParamAlarm*)(*it))->JudgeSlowAlarm(pDB_);
			if (iTmp>iFlag)
				iFlag=iTmp;
		}
		//CString sTemp;sTemp.Format("iFlag:%d,_bVibSlowAlarmStatus:%d",iFlag,_bVibSlowAlarmStatus?1:0);
		//CHZLogManage::Debug(sTemp,"CAlarmPlantInfo","JudgeVibSlowAlarm");
		//iFlag 0:说明没有报警,1:存在报警;2:有新的报警需要重新开始5分钟数据保存;
		//3:刚刚添加了报警日志,需要重新开始5分钟数据保存
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndWrite();

	if (_bVibSlowAlarmStatus)
	{
		_pLockVibData->BeginWrite();
		try
		{
			if (iFlag>1)
				VibSlowAlarmOP(pDB_,true);
			else
				VibSlowAlarmOP(pDB_,false);
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndWrite();
	}
	else
	{
		if (iFlag>0)
		{
			FirstVibSlowAlarmOP(pDB_);
			// CHZLogManage::Info("初次保存80组数据完成","CAlarmPlantInfo","JudgeVibSlowAlarm");
		}
		else
		{
			_pLockVibData->BeginWrite();
			try
			{
				//重置振动通道报警状态
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{

					((CVibChannParamAlarm*)(*it))->ResetSlowAlarmStatus();
				}
				if (_iEventIdS>0)
				{
					_iEventIdS=-1;//重置缓变报警事件ID
				}
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndWrite();
		}
	}
}

void CAlarmPlantInfo::JudgeVibTrendAlarm(IDBInterFace * pDb_)
{
	_pLockVibData->BeginWrite();

	try
	{
		for (V_PCHANNINFO::iterator it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{	
			((CVibChannParamAlarm*)(*it))->JudgeTrendAlarm(pDb_);
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::AddCustomAlarmParamInfo(CCustomDB *pDB_)
{
	int index=0;
	vector<CCustomAlarmParamInfo> vCustomAlarmParamInfo;
	//添加振动自定义参数信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParamAlarm *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParamAlarm*)(*itVib);
		//获取自定义参数数据
		if (strcmp(pVibChann->GetChannInfo()._cAlarmTempID, "") != 0)
		{
			SwichDbName(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cAlarm_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cAlarm_TempletID,pVibChann->GetChannInfo()._cAlarmTempID);
			while (pDB_->More())
			{
				CCustomAlarmParamInfo paramInfo;
				paramInfo.GetCustomAlarmParamInfo(pDB_);
				vCustomAlarmParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Vib自定义报警参数数据条数 index :%d,%d",index,vCustomAlarmParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pVibChann->SetCustomAlarmParamInfo(0,index,&vCustomAlarmParamInfo[0]);
			vCustomAlarmParamInfo.clear();
		}
	}
	//添加动态自定义参数信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParamAlarm *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParamAlarm*)(*itDyn);
		//获取自定义参数数据
		if (strcmp(pDynChann->GetChannInfo()._cAlarmTempID, "") != 0)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cAlarm_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cAlarm_TempletID,pDynChann->GetChannInfo()._cAlarmTempID);
			while (pDB_->More())
			{
				CCustomAlarmParamInfo paramInfo;
				paramInfo.GetCustomAlarmParamInfo(pDB_);
				vCustomAlarmParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Dyn自定义报警参数数据条数 index :%d,%d",index,vCustomAlarmParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pDynChann->SetCustomAlarmParamInfo(0,index,&vCustomAlarmParamInfo[0]);
			vCustomAlarmParamInfo.clear();
		}
	}
}

void CAlarmPlantInfo::AddVibChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	CString strLog="Start";
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	int index=0;
	S_ChannInfo sChannInfo;
	CVibChannParamAlarm *vibParam=NULL;
	//_pDB=pDB_;
	_vVibChann.clear();
	_mapVibChannIndex.clear();
	//径向振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s'  AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		vibParam=new CVibChannParamAlarm;
		vibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		vibParam->SetChannInfo(sChannInfo);
		vibParam->SetParentPoint(this);
		vibParam->InitValues();
		_vVibChann.push_back(vibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	strLog.Format("%s-%s 径向振动通道数目：%d",_sCompany.c_str(),_sPlantInfo._cPlantID,_vVibChann.size());
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	//轴向振动通道
	pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'  AND %s =%d",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);
	pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
	while (pDB_->More())
	{
		vibParam=new CVibChannParamAlarm;
		vibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		vibParam->SetChannInfo(sChannInfo);
		vibParam->SetParentPoint(this);
		vibParam->InitValues();
		_vVibChann.push_back(vibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR&&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL)
	{	//添加轴心轨迹
		_vAxisChann.clear();
		CAxisAlarmChannParam *axisAlarm=NULL;
		CAxisChannInfo axisChannInfo;
		int iRevType=1;
		int iVerIndex=0,iHorIndex=0;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),
			gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			axisAlarm=new CAxisAlarmChannParam;
			axisAlarm->SetRwLock(_pLockChannInfo);
			axisAlarm->SetDataLock(_pLockVibData);
			axisChannInfo.GetAxisChannInfo(pDB_);
			iVerIndex=GetSpecVibChannIndexNo(axisChannInfo._cVerChann);

			if(iVerIndex < 0) {
				HZLOG_WARN(STR("未能将%s测点加入轴心测点对", axisChannInfo._cVerChann));
				continue;
			}
			iRevType=_vVibChann[iVerIndex]->GetChannInfo()._iRevType;
			iHorIndex=GetSpecVibChannIndexNo(axisChannInfo._cHorChann);
			if(iHorIndex < 0) {
				HZLOG_WARN(STR("未能将%s测点加入轴心测点对", axisChannInfo._cHorChann));
				continue;
			}

			axisAlarm->SetChannInfoEx(axisChannInfo,iVerIndex,iHorIndex,iRevType,_vVibChann[iVerIndex]->GetChannInfo()._fSensitivity,_vVibChann[iHorIndex]->GetChannInfo()._fSensitivity);
			axisAlarm->SetParentPoint(this);
			_vAxisChann.push_back(axisAlarm);
		}
		_iAxesPairNum=_vAxisChann.size();
		strLog.Format("%s-%s 轴心位置数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,_iAxesPairNum);
		CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	}


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwichDbName(pDB_,lstrSubDbName);
	
	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_VIB);
	
	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
					+ DBUtils::ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName) \
					+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
					+  " and " + CString(gc_cChannType) \
					+ " = " +lstrChannType ;

	
	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo);
		
		int lnIndex = this->GetSpecVibChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vVibChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);
				
				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}
		
	}

}

void CAlarmPlantInfo::AddDynChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	CString strLog="Start";
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");
	int index=0;
	S_ChannInfo sChannInfo;
	CDynChannParamAlarm *dynParam=NULL;
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	//动态通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s'  AND t1.[%s] in(%d,%d,%d)",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		dynParam=new CDynChannParamAlarm;
		dynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		dynParam->SetChannInfo(sChannInfo);
		dynParam->SetParentPoint(this);
		
		dynParam->InitValues();
		
		_vDynChann.push_back(dynParam);
		
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	strLog.Format("%s-%s 动态通道数目：%d",_sCompany.c_str(),_sPlantInfo._cPlantID,_vDynChann.size());
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");

	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{	//添加轴心轨迹
		_vAxisChann.clear();
		CAxisAlarmChannParam *axisAlarm=NULL;
		CAxisChannInfo axisChannInfo;
		int iRevType=1;
		int iVerIndex=0,iHorIndex=0;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s'  AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),
			gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			axisAlarm=new CAxisAlarmChannParam;
			axisAlarm->SetRwLock(_pLockChannInfo);
			axisAlarm->SetDataLock(_pLockVibData);
			axisChannInfo.GetAxisChannInfo(pDB_);
			iVerIndex=GetSpecDynChannIndexNo(axisChannInfo._cVerChann);
			if(iVerIndex < 0) continue;
			iRevType=_vDynChann[iVerIndex]->GetChannInfo()._iRevType;
			iHorIndex=GetSpecDynChannIndexNo(axisChannInfo._cHorChann);
			if(iHorIndex < 0) continue;
			axisAlarm->SetChannInfoEx(axisChannInfo,iVerIndex,iHorIndex,iRevType,_vDynChann[iVerIndex]->GetChannInfo()._fSensitivity,_vDynChann[iHorIndex]->GetChannInfo()._fSensitivity);
			axisAlarm->SetParentPoint(this);
			_vAxisChann.push_back(axisAlarm);
		}
		_iAxesPairNum=_vAxisChann.size();
		strLog.Format("%s-%s 轴心位置数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,_iAxesPairNum);
		CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");
	}


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwichDbName(pDB_,lstrSubDbName);

	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_DYN);

	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
		+DBUtils:: ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName) \
		+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
	+  " and " + CString(gc_cChannType) \
		+ " = " +lstrChannType ;


	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo);

		int lnIndex = this->GetSpecDynChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vDynChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);

				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}

	}
}

void CAlarmPlantInfo::AddProcChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	CString strLog;
	CProcChannParamAlarm *procParam;
	int index=0;
	S_ChannInfo sChannInfo;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND %s in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CProcChannParamAlarm;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		procParam->SetParentPoint(this);
		procParam->InitValues();
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}

	strLog.Format("%s-%s 过程量通道数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,index);
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddProcChannInfo");


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwichDbName(pDB_,lstrSubDbName);

	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_DYN);

	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
		+ DBUtils::ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName)\
		+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
	+  " and " + CString(gc_cChannType) \
		+ " = " +lstrChannType ;


	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo);

		int lnIndex = this->GetSpecProcChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vProcChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);

				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}

	}
}

void CAlarmPlantInfo::AddChannInfoEnd(CCustomDB * pDB_)
{
	CPlantInfoOnLine::AddChannInfoEnd( pDB_);
	CString strLog="";
	CString strTmp;
	V_PCHANNINFO::iterator it;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	_sInsField="";
	_sSelWaveField="";
	_sVibTrendWaveField = "";
	_sInsField.append(gc_cRev);
	_sInsField.append(",");
	_sInsField.append(gc_cRev2);
	_sInsField.append(",");
	_sInsField.append(gc_cRev3);
	_sInsField.append(",");
	_sInsField.append(gc_cRev4);
	_sInsField.append(",");
	_sInsField.append(gc_cRev5);
	_sVibTrendField = "";

	_vVibWaveField.clear();
	_vDynWaveField.clear();

	string sTmp;

	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		if (((CVibChannParamAlarm*)(*it))->GetChannInfo()._iChannType==GE_VIBCHANN)
			++_iRadialVibChannNum;
		else
			++_iAxialVibChannNum;
		_sInsField.append(",");

		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetCharValueField());
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
	


		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-2));
 
		_sSelWaveField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1));
		_sSelWaveField.append(",");
		_sSelWaveField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7));
		_sSelWaveField.append(",");

		
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetCharValueField());
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sVibTrendField.append(",");

		sTmp = ((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1);
		sTmp.append(",");
		sTmp+=((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7);
		_vVibWaveField.push_back(sTmp.c_str());
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sVibTrendField += strTmp;
	_sVibTrendWaveField = _sSelWaveField.substr(0,_sSelWaveField.length()-1);

	_sSelWaveField.append(gc_cEventID);

	//构建动态数据表的插入字段和动态数据的波形显示字段
	_sDynInsField="";
	
	_sDynTrendField = "";
	
	_sDynInsField.append(gc_cRev);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev2);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev3);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev4);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev5);
	_sDynSelWaveField="";
	_sDynTrendWaveField="";
	sTmp="";
	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetCharValueField());
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));

		

		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-2));

		_sDynSelWaveField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1));
		_sDynSelWaveField.append(",");
		_sDynSelWaveField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7));
		_sDynSelWaveField.append(",");


		
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetCharValueField());
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sDynTrendField.append(",");

		sTmp = ((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1);
		sTmp.append(",");
		sTmp+=((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7);
		_vDynWaveField.push_back(sTmp.c_str());
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sDynTrendField += strTmp;
	_sDynTrendWaveField = _sDynSelWaveField.substr(0,_sDynSelWaveField.length()-1);

	_sDynSelWaveField.append(gc_cEventID);


	V_PCHANNINFO::iterator it2;
	_sProcField=" ";
	S_ChannInfo channInfo;

	for (it2=_vProcChann.begin();it2!=_vProcChann.end();++it2)
	{
		channInfo=(*it2)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);
	 strLog.Format("%s-%s-%s Finish",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	BHLOG_INFO(strLog);
	//初始化各种ID
	if(!InitEventID(pDB_))
	{
		CHZLogManage::Error("初始化事件ID失败!","CAlarmPlantInfo","AddChannInfoEnd");
	}
	//振动波形数据分配缓存

	long iNum = (_sPlantInfo._iVibChannNum)*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM;
	_pWave			= new DATATYPE_WAVE[iNum];


	iNum      = (_sPlantInfo._iDynChannNum)*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM;
	_pWaveDyn		= new DATATYPE_WAVE[iNum];


	//构造振动通道数据表单名字
	int index=0;

	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		((CVibChannParamAlarm*)(*it))->SetWaveMemoryPoint(&(_pWave[index*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM]),_sPlantInfo._iSmpNum);
		++index;
	}
	index = 0;
	for (it=_vDynChann.begin(); it!=_vDynChann.end(); ++it)
	{
		((CDynChannParamAlarm*)(*it))->SetWaveMemoryPoint(&(_pWaveDyn[index*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM]),_sPlantInfo._iSmpNum);
		++index;
	}
	COleSqlServerDB loDb;
	loDb.setPDB(pDB_);
	CBHDateTime loTimeNow = CBHDateTime::GetCurrentTimeEx();
	this->SetAllAlarmLogEnd(&loDb,loTimeNow);
	CHZLogManage::Info("Finish","CAlarmPlantInfo","AddChannInfoEnd");
	return;
}

int CAlarmPlantInfo::SetVibChannState(S_AlarmState *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iVibChannNum) )
		iNum_=_sPlantInfo._iVibChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			 if (((CVibChannParamAlarm*)_vVibChann[nloop])->IsAlarmConfirmed())
			 {
				 if (ALARM_CONFIRM_TYPE_ONCE == ((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType())
				 {
					 if (!((CVibChannParamAlarm*)_vVibChann[nloop])->JudgeRegAgainAlarmForConfirm())
					 {
						 ((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					 }
				 }
			 }else
			 {
				 ((CVibChannParamAlarm*)_vVibChann[nloop])->SetAlarmStatus(pState[nloop]);
				 ((CVibChannParamAlarm*)_vVibChann[nloop])->SaveCurrentAsLastAlarmValue();
			 }
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::SetVibChannState( S_AlarmStateWithLevel *pState,int iNum_ )
{
	if (iNum_> (_sPlantInfo._iVibChannNum) )
		iNum_=_sPlantInfo._iVibChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CVibChannParamAlarm*)_vVibChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType())
				{
					if (!((CVibChannParamAlarm*)_vVibChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}
				}
			}else
			{
				((CVibChannParamAlarm*)_vVibChann[nloop])->SetAlarmStatus(pState[nloop]);
				((CVibChannParamAlarm*)_vVibChann[nloop])->SaveCurrentAsLastAlarmValue();
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}
int CAlarmPlantInfo::SetVibChannStateEx(S_AlarmStateEx *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iVibChannNum) )
		iNum_=_sPlantInfo._iVibChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CVibChannParamAlarm*)_vVibChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType())
				{
					if (!((CVibChannParamAlarm*)_vVibChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CVibChannParamAlarm*)_vVibChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}
				}
			}else
			{
				((CVibChannParamAlarm*)_vVibChann[nloop])->SetAlarmStatusEx(pState[nloop]);
			}

			float lfLastMaxAlarmValue = ((CVibChannParamAlarm*)_vVibChann[nloop])->LastMaxAlarmValue();
			if(pState[nloop]._iRegState > 0 || pState[nloop]._iFastState > 0 || pState[nloop]._iSlowState > 0)
			{
				if (!((CVibChannParamAlarm*)_vVibChann[nloop])->JudgeRegAgainAlarm(pState[nloop]._fLastAlarmVal,lfLastMaxAlarmValue))
				{
					((CVibChannParamAlarm*)_vVibChann[nloop])->LastMaxAlarmValue(pState[nloop]._fLastAlarmVal);
				}
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::SetDynChannState(S_AlarmState *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iDynChannNum) )
		iNum_=_sPlantInfo._iDynChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{

			if (((CDynChannParamAlarm*)_vDynChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType())
				{
					if (!((CDynChannParamAlarm*)_vDynChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}

				}
			}else
			{
				((CDynChannParamAlarm*)_vDynChann[nloop])->SetAlarmStatus(pState[nloop]);
				((CDynChannParamAlarm*)_vDynChann[nloop])->SaveCurrentAsLastAlarmValue();
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::SetDynChannState( S_AlarmStateWithLevel *pState,int iNum_ )
{
	if (iNum_> (_sPlantInfo._iDynChannNum) )
		iNum_=_sPlantInfo._iDynChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{

			if (((CDynChannParamAlarm*)_vDynChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType())
				{
					if (!((CDynChannParamAlarm*)_vDynChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}

				}
			}else
			{
				((CDynChannParamAlarm*)_vDynChann[nloop])->SetAlarmStatus(pState[nloop]);
				((CDynChannParamAlarm*)_vDynChann[nloop])->SaveCurrentAsLastAlarmValue();
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}
int CAlarmPlantInfo::SetDynChannStateEx(S_AlarmStateEx *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iDynChannNum) )
		iNum_=_sPlantInfo._iDynChannNum;

	_pLockVibData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{

			if (((CDynChannParamAlarm*)_vDynChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType())
				{
					if (!((CDynChannParamAlarm*)_vDynChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CDynChannParamAlarm*)_vDynChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}

				}
			}else
			{
				((CDynChannParamAlarm*)_vDynChann[nloop])->SetAlarmStatusEx(pState[nloop]);
			}

			float lfLastMaxAlarmValue = ((CDynChannParamAlarm*)_vDynChann[nloop])->LastMaxAlarmValue();
			if(pState[nloop]._iRegState > 0 || pState[nloop]._iFastState > 0 || pState[nloop]._iSlowState > 0)
			{
				if (!((CDynChannParamAlarm*)_vDynChann[nloop])->JudgeRegAgainAlarm(pState[nloop]._fLastAlarmVal,lfLastMaxAlarmValue))
				{
					((CDynChannParamAlarm*)_vDynChann[nloop])->LastMaxAlarmValue(pState[nloop]._fLastAlarmVal);
				}
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
	return iNum_;
}


int CAlarmPlantInfo::GetVibChannState(S_AlarmState *pState,int & iNum_)
{
	if (iNum_> (_sPlantInfo._iVibChannNum) )
		iNum_=_sPlantInfo._iVibChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CVibChannParamAlarm*)_vVibChann[nloop])->GetAlarmStatus(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetVibChannStateEx(S_AlarmStateEx *pState,int & iNum_)
{
	if (iNum_> (_sPlantInfo._iVibChannNum) )
		iNum_=_sPlantInfo._iVibChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CVibChannParamAlarm*)_vVibChann[nloop])->GetAlarmStatusEx(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetDynChannState(S_AlarmState *pState,int & iNum_)
{
	if (iNum_> (_sPlantInfo._iDynChannNum) )
		iNum_=_sPlantInfo._iDynChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CDynChannParamAlarm*)_vDynChann[nloop])->GetAlarmStatus(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetDynChannStateEx(S_AlarmStateEx *pState,int & iNum_)
{
	if (iNum_> (_sPlantInfo._iDynChannNum) )
		iNum_=_sPlantInfo._iDynChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CDynChannParamAlarm*)_vDynChann[nloop])->GetAlarmStatusEx(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::SetProcChannState(S_AlarmState *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iProcChannNum) )
		iNum_=_sPlantInfo._iProcChannNum;

	_pLockProcData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CProcChannParamAlarm*)_vProcChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType())
				{
					if (!((CProcChannParamAlarm*)_vProcChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}
				}
			}else
			{
				((CProcChannParamAlarm*)_vProcChann[nloop])->SetAlarmStatus(pState[nloop]);
				((CProcChannParamAlarm*)_vProcChann[nloop])->SaveCurrentAsLastAlarmValue();
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::SetProcChannState( S_AlarmStateWithLevel *pState,int iNum_ )
{
	if (iNum_> (_sPlantInfo._iProcChannNum) )
		iNum_=_sPlantInfo._iProcChannNum;

	_pLockProcData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CProcChannParamAlarm*)_vProcChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType())
				{
					if (!((CProcChannParamAlarm*)_vProcChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}
				}
			}else
			{
				((CProcChannParamAlarm*)_vProcChann[nloop])->SetAlarmStatus(pState[nloop]);
				((CProcChannParamAlarm*)_vProcChann[nloop])->SaveCurrentAsLastAlarmValue();
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
	return iNum_;
}
int CAlarmPlantInfo::SetProcChannStateEx(S_AlarmStateEx *pState,int iNum_)
{
	if (iNum_> (_sPlantInfo._iProcChannNum) )
		iNum_=_sPlantInfo._iProcChannNum;

	_pLockProcData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CProcChannParamAlarm*)_vProcChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType())
				{
					if (!((CProcChannParamAlarm*)_vProcChann[nloop])->JudgeRegAgainAlarmForConfirm())
					{
						((CProcChannParamAlarm*)_vProcChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
					}
				}
			}else
			{
				((CProcChannParamAlarm*)_vProcChann[nloop])->SetAlarmStatusEx(pState[nloop]);
			}

			float lfLastMaxAlarmValue = ((CProcChannParamAlarm*)_vProcChann[nloop])->LastMaxAlarmValue();

			if(pState[nloop]._iRegState > 0 || pState[nloop]._iFastState > 0 || pState[nloop]._iSlowState > 0)
			{
				if (!((CProcChannParamAlarm*)_vProcChann[nloop])->JudgeRegAgainAlarm(pState[nloop]._fLastAlarmVal,lfLastMaxAlarmValue))
				{
					((CProcChannParamAlarm*)_vProcChann[nloop])->LastMaxAlarmValue(pState[nloop]._fLastAlarmVal);
				}
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::GetProcChannState(S_AlarmState *pState,int& iNum_)
{
	if (iNum_> _sPlantInfo._iProcChannNum)
		iNum_=_sPlantInfo._iProcChannNum;

	_pLockProcData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CProcChannParamAlarm*)_vProcChann[nloop])->GetAlarmStatus(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetProcChannStateEx(S_AlarmStateEx *pState,int& iNum_)
{
	if (iNum_> _sPlantInfo._iProcChannNum)
		iNum_=_sPlantInfo._iProcChannNum;

	_pLockProcData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			((CProcChannParamAlarm*)_vProcChann[nloop])->GetAlarmStatusEx(&(pState[nloop]));
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetAxisVibChannState(S_AlarmState *pState,int& iNum_)
{
	if (iNum_>_iAxialVibChannNum)
		iNum_=_iAxialVibChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
			((CVibChannParamAlarm*)_vVibChann[nloop+_iRadialVibChannNum])->GetAlarmStatus(&(pState[nloop]));
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::GetRadialVibChannState(S_AlarmState *pState,int& iNum_)
{
	if (iNum_>_iRadialVibChannNum)
		iNum_=_iRadialVibChannNum;

	_pLockVibData->BeginRead();
	try
	{
		for (int nloop=0;nloop<iNum_;++nloop)
			((CVibChannParamAlarm*)_vVibChann[nloop])->GetAlarmStatus(&(pState[nloop]));
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return iNum_;
}

int CAlarmPlantInfo::SetAxisLocationState(S_AlarmState *pState_,int iNum_)
{
	if (iNum_>_iAxesPairNum)
		iNum_=_iAxesPairNum;
	_pLockAxisLocationData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CAxisAlarmChannParam*)_vAxisChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CAxisAlarmChannParam*)_vAxisChann[nloop])->AlarmConfirmType())
				{
					((CAxisAlarmChannParam*)_vAxisChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
				}
			}else
			{
				((CAxisAlarmChannParam*)_vAxisChann[nloop])->SetAlarmStatus(pState_[nloop]);
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockAxisLocationData->EndWrite();
	return iNum_;
}

int CAlarmPlantInfo::SetAxisLocationState( S_AlarmStateWithLevel *pState_,int iNum_ )
{
	if (iNum_>_iAxesPairNum)
		iNum_=_iAxesPairNum;
	_pLockAxisLocationData->BeginWrite();
	try{
		for (int nloop=0;nloop<iNum_;++nloop)
		{
			if (((CAxisAlarmChannParam*)_vAxisChann[nloop])->IsAlarmConfirmed())
			{
				if (ALARM_CONFIRM_TYPE_ONCE == ((CAxisAlarmChannParam*)_vAxisChann[nloop])->AlarmConfirmType())
				{
					((CAxisAlarmChannParam*)_vAxisChann[nloop])->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
				}
			}else
			{
				((CAxisAlarmChannParam*)_vAxisChann[nloop])->SetAlarmStatus(pState_[nloop]);
			}
		}
	}catch (...) {
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockAxisLocationData->EndWrite();
	return iNum_;
}
int CAlarmPlantInfo::GetAxisLocationState(S_AlarmState *pState_,int &iNum_)
{
	if(iNum_>_iAxesPairNum)
		iNum_=_iAxesPairNum;
	for (int nloop=0;nloop<iNum_;++nloop)
	{
		_pLockAxisLocationData->BeginRead();
		try
		{
			((CAxisAlarmChannParam*)_vAxisChann[nloop])->GetAlarmStatus(&(pState_[nloop]));
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAxisLocationData->EndRead();
	}
	return iNum_;
}

bool CAlarmPlantInfo::GetSpecChannAndTypeAlarmState(string sChann_,float &fData_,unsigned int &iState_,int iType_)
{
	int index=0;
	if (iType_==GE_VIBCHANN)
		index=GetSpecVibChannIndex(sChann_);
	else if(iType_==GE_AXIALCHANN)
		index=GetSpecVibChannIndex(sChann_)-_iAxialVibChannNum;
	else if ((iType_ == GE_RODSINKCHANN) ||(iType_ == GE_DYNPRESSCHANN)||(iType_ == GE_DYNSTRESSCHANN))
		index = GetSpecDynChannIndex(sChann_);
	else
		index=GetSpecProcChannIndex(sChann_);
	return GetSpecChannAndTypeAlarmState(index,sChann_,fData_,iState_,iType_);
}

bool CAlarmPlantInfo::GetSpecChannAndTypeAlarmState(int index_,string &sChann_,float &fData,unsigned int &iState_, int iType_)
{
	int iCharType=GE_ALLCHAR;
	if (iType_==GE_AXIALCHANN)
		iCharType=GE_AXIALDISPLACE;
	string sUnit,sChannAlias;
	int iList=0,iChannType=0;
	return GetSpecChannAndTypeAlarmState(index_,sChann_,sChannAlias,sUnit,iList,fData,iState_,iChannType,iType_,iCharType);
}

bool CAlarmPlantInfo::GetSpecChannAndTypeAlarmState(int index_,
													string &sChann_,
													string &sChannAlias_,
													string &sUnit_,
													int &iList_,
													float &fData_,
													unsigned int &iState_, 
													int &iChannDetailType_,
													int iType_,
													int iCharType_)
{
	if (index_<0)
		return false;
	S_ChannInfo channInfo;
	switch (iType_)
	{
	case GE_VIBCHANN:
		{
			if (index_> _sPlantInfo._iVibChannNum || index_ >= _vVibChann.size())
			{	
				return false;
			}
			channInfo=_vVibChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockVibData->BeginWrite();

			try
			{
				((CVibChannParamAlarm*)_vVibChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			try
			{
				iState_=((CVibChannParamAlarm*)_vVibChann[index_])->GetAlarmStatus();
				
				int lnAlarmStatus = ParseAlarmStatus(iState_);

				if (lnAlarmStatus>0)
				{
					fData_ = ((CVibChannParamAlarm*)_vVibChann[index_])->LastAlarmValue();
				}else
				{
					if (iChannDetailType_ == GE_AXIALCHANN)
					{
						fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(GE_AXIALDISPLACE);
					}
					else
					{
						fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(iCharType_);
					}
				}

				if (iChannDetailType_ == GE_AXIALCHANN)
				{
					sUnit_= CharInfoHelper::GetCharUnit(channInfo._iInfoType, GE_AXIALDISPLACE);
				}
				else
				{
					sUnit_= CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);
				}

			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		break;
	case GE_AXIALCHANN:
		{
			if (index_>_iAxialVibChannNum || index_ > _vVibChann.size())
			{	
				return false;
			}
			index_=index_+_iRadialVibChannNum;
			channInfo=_vVibChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockVibData->BeginWrite();

			try
			{
				((CVibChannParamAlarm*)_vVibChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			try
			{
				iState_=((CVibChannParamAlarm*)_vVibChann[index_])->GetAlarmStatus();
				int lnAlarmStatus = ParseAlarmStatus(iState_);
				
				if (lnAlarmStatus>0)
				{
					fData_=((CVibChannParamAlarm*)_vVibChann[index_])->LastAlarmValue();
				}else
				{
					fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(iCharType_);

				}

				sUnit_=CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);

			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		break;
	case GE_AXISLOCATIONCHANN:
		{
			if (index_>_iAxesPairNum || index_ >= this->_vAxisChann.size())
			{	
				return false;
			}
			CString sAxisLocation;sAxisLocation.Format("轴心位置%d",index_+1);
			sChann_=sAxisLocation.GetString();
			sChannAlias_=sChann_;


			_pLockVibData->BeginRead();
			try
			{
				
				iState_=((CAxisAlarmChannParam*)_vAxisChann[index_])->GetAlarmStatus();
				fData_=((CAxisAlarmChannParam*)_vAxisChann[index_])->GetCurrSpecCharTypeValue();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();

		}
		break;
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		{
			if (index_> _sPlantInfo._iProcChannNum || index_ >= _vProcChann.size())
			{
				return false;
			}
			channInfo=_vProcChann[index_]->GetChannInfo();

			sUnit_=channInfo._cUnit;
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			_pLockProcData->BeginRead();

			try
			{
				iState_=((CProcChannParamAlarm*)_vProcChann[index_])->GetAlarmStatus();

				if(GE_DATA_SOURCE_OFFLINE == channInfo._iDataSource){

					for(V_PCHANNINFO::iterator it = _vOfflineChann.begin();
						it != _vOfflineChann.end();
						++it)
					{
						COfflineChannParam * pParam = (COfflineChannParam *)(*it);
						S_ChannInfo sChannInfo = pParam->GetChannInfo();

						CString sChann = sChannInfo._cChannNo;
						if(sChann == channInfo._cChannNo)

							fData_ = pParam->GetValue();
					}
				}
				else 
				{
					int lnAlarmStatus = ParseAlarmStatus(iState_);

					if (lnAlarmStatus>0)
					{
						fData_=((CProcChannParamAlarm*)_vProcChann[index_])->LastAlarmValue();
					}else
					{
						fData_=((CProcChannParamAlarm*)_vProcChann[index_])->GetData();
					}
				}
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}


			_pLockProcData->EndRead();
		}
		break;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
	default:
		{
			if (index_> _sPlantInfo._iDynChannNum || index_ >= _vDynChann.size())
			{	
				return false;
			}

			channInfo=_vDynChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockDynData->BeginWrite();
			
			try
			{
				((CDynChannParamAlarm*)_vDynChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockDynData->EndWrite();

			_pLockDynData->BeginRead();
			try
			{
				iState_=((CDynChannParamAlarm*)_vDynChann[index_])->GetAlarmStatus();
				int lnAlarmStatus = ParseAlarmStatus(iState_);
				if (lnAlarmStatus>0)
				{
					fData_=((CDynChannParamAlarm*)_vDynChann[index_])->LastAlarmValue();

				}else
				{
					fData_=((CDynChannParamAlarm*)_vDynChann[index_])->GetCurrSpecCharTypeValue(iCharType_);

				}

				sUnit_=CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			//		CString strTmp;strTmp.Format("%s,%d,%f",sChann_.c_str(),index_,fData_);AfxMessageBox(strTmp);
			_pLockDynData->EndRead();
		}
	}
	return true;
}

bool CAlarmPlantInfo::GetSpecChannAndTypeAlarmStateEx(int index_,
													  string &sChann_,
													  string &sChannAlias_,
													  string &sUnit_,
													  int &iList_,
													  float &fData_,
													  unsigned int &iState_, 
													  int &iChannDetailType_,
													  int &iAlarmLevel_,
													  int iType_,
													  int iCharType_)
{
	if (index_<0)
		return false;
	S_ChannInfo channInfo;
	switch (iType_)
	{
	case GE_VIBCHANN:
		{
			if (index_> _sPlantInfo._iVibChannNum || index_ >= _vVibChann.size())
			{	
				return false;
			}
			channInfo=_vVibChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockVibData->BeginWrite();

			try
			{
				((CVibChannParamAlarm*)_vVibChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			try
			{
				iState_=((CVibChannParamAlarm*)_vVibChann[index_])->GetAlarmStatus();

				int lnAlarmStatus = ParseAlarmStatus(iState_);

				if (lnAlarmStatus>0)
				{
					fData_ = ((CVibChannParamAlarm*)_vVibChann[index_])->LastAlarmValue();
				}else
				{
					if (iChannDetailType_ == GE_AXIALCHANN)
					{
						fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(GE_AXIALDISPLACE);
					}
					else
					{
						fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(iCharType_);
					}
				}

				if (iChannDetailType_ == GE_AXIALCHANN)
				{
					sUnit_= CharInfoHelper::GetCharUnit(channInfo._iInfoType, GE_AXIALDISPLACE);
				}
				else
				{
					sUnit_= CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);
				}

				iAlarmLevel_ = ((CVibChannParamAlarm*)_vVibChann[index_])->GetAlarmLevel();


			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		break;
	case GE_AXIALCHANN:
		{
			if (index_>_iAxialVibChannNum || index_ > _vVibChann.size())
			{	
				return false;
			}
			index_=index_+_iRadialVibChannNum;
			channInfo=_vVibChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockVibData->BeginWrite();

			try
			{
				((CVibChannParamAlarm*)_vVibChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			try
			{
				iState_=((CVibChannParamAlarm*)_vVibChann[index_])->GetAlarmStatus();
				int lnAlarmStatus = ParseAlarmStatus(iState_);

				if (lnAlarmStatus>0)
				{
					fData_=((CVibChannParamAlarm*)_vVibChann[index_])->LastAlarmValue();
				}else
				{
					fData_=((CVibChannParamAlarm*)_vVibChann[index_])->GetCurrSpecCharTypeValue(iCharType_);

				}

				sUnit_=CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);

			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
		}
		break;
	case GE_AXISLOCATIONCHANN:
		{
			if (index_>_iAxesPairNum || index_ >= this->_vAxisChann.size())
			{	
				return false;
			}
			CString sAxisLocation;sAxisLocation.Format("轴心位置%d",index_+1);
			sChann_=sAxisLocation.GetString();
			sChannAlias_=sChann_;


			_pLockVibData->BeginRead();
			try
			{

				iState_=((CAxisAlarmChannParam*)_vAxisChann[index_])->GetAlarmStatus();
				fData_=((CAxisAlarmChannParam*)_vAxisChann[index_])->GetCurrSpecCharTypeValue();
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();

		}
		break;
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		{
			if (index_> _sPlantInfo._iProcChannNum || index_ >= _vProcChann.size())
			{
				return false;
			}
			channInfo=_vProcChann[index_]->GetChannInfo();

			sUnit_=channInfo._cUnit;
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;
			_pLockProcData->BeginRead();

			try
			{
				iState_=((CProcChannParamAlarm*)_vProcChann[index_])->GetAlarmStatus();

				if(GE_DATA_SOURCE_OFFLINE == channInfo._iDataSource){

					for(V_PCHANNINFO::iterator it = _vOfflineChann.begin();
						it != _vOfflineChann.end();
						++it)
					{
						COfflineChannParam * pParam = (COfflineChannParam *)(*it);
						S_ChannInfo sChannInfo = pParam->GetChannInfo();

						CString sChann = sChannInfo._cChannNo;
						if(sChann == channInfo._cChannNo)

							fData_ = pParam->GetValue();
					}
				}
				else 
				{
					int lnAlarmStatus = ParseAlarmStatus(iState_);

					if (lnAlarmStatus>0)
					{
						fData_=((CProcChannParamAlarm*)_vProcChann[index_])->LastAlarmValue();
					}else
					{
						fData_=((CProcChannParamAlarm*)_vProcChann[index_])->GetData();
					}
				}
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}


			_pLockProcData->EndRead();
		}
		break;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
	default:
		{
			if (index_> _sPlantInfo._iDynChannNum || index_ >= _vDynChann.size())
			{	
				return false;
			}

			channInfo=_vDynChann[index_]->GetChannInfo();
			sChann_=channInfo._cChannID;
			sChannAlias_=channInfo._cChannAlias;
			iList_=channInfo._iList;
			iChannDetailType_=channInfo._iChannType;

			_pLockDynData->BeginWrite();

			try
			{
				((CDynChannParamAlarm*)_vDynChann[index_])->ComputeEigenValue(this->GetPlantInfo()._iMachineType);
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

			_pLockDynData->EndWrite();

			_pLockDynData->BeginRead();
			try
			{
				iState_=((CDynChannParamAlarm*)_vDynChann[index_])->GetAlarmStatus();
				int lnAlarmStatus = ParseAlarmStatus(iState_);
				if (lnAlarmStatus>0)
				{
					fData_=((CDynChannParamAlarm*)_vDynChann[index_])->LastAlarmValue();

				}else
				{
					fData_=((CDynChannParamAlarm*)_vDynChann[index_])->GetCurrSpecCharTypeValue(iCharType_);

				}

				sUnit_=CharInfoHelper::GetCharUnit(channInfo._iInfoType, iCharType_);
			}
			catch (...)
			{
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			//		CString strTmp;strTmp.Format("%s,%d,%f",sChann_.c_str(),index_,fData_);AfxMessageBox(strTmp);
			_pLockDynData->EndRead();
		}
	}
	return true;
}

bool CAlarmPlantInfo::GetPlantAlarmState()
{
	V_PCHANNINFO::iterator itVib=_vVibChann.begin();
	V_PAXISCHANNINFO::iterator itAxes;
	V_PCHANNINFO::iterator itProc;
	V_PCHANNINFO::iterator itDyn;
	_pLockVibData->BeginRead();
	try
	{
		for (;itVib!=_vVibChann.end();++itVib)
		{
			if (((CVibChannParamAlarm*)(*itVib))->GetAlarmStatus()>0)
			{
				_pLockVibData->EndRead();
				return true;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();

	_pLockProcData->BeginRead();
	try
	{
		for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
		{
			if (((CProcChannParamAlarm*)(*itProc))->GetAlarmStatus()>0)
			{
				_pLockProcData->EndRead();
				return true;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();
	/*
	_pLockDynData->BeginRead();
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
	if (((CDynChannParamAlarm*)(*itDyn))->GetAlarmStatus()>0)
	{
	_pLockDynData->EndRead();
	return true;
	}
	}
	_pLockDynData->EndRead();*/
	return false;
}

#define ALARM_BIT_LOW  0x01
#define ALARM_BIT_HIGH 0x10

#define ALARM_STATUS_NONE  0
#define ALARM_STATUS_LOW   1
#define ALARM_STATUS_HIGH  2

unsigned int CAlarmPlantInfo::GetPlantAlarmStateEx()
{
	unsigned int luAlarmBitStatus = 0;

	try
	{
		CMutexRWReader safeVibReader(this->_pLockVibData);

		V_PCHANNINFO::iterator itVib;
		for (itVib = _vVibChann.begin(); itVib!=_vVibChann.end(); ++itVib)
		{
			unsigned int luAlarmStatus = ((CVibChannParamAlarm*)(*itVib))->GetAlarmStatus();
			int  luParsedStatus = ::AlarmStatus_ParseAlarmStatus(luAlarmStatus);

			if (luParsedStatus > 0)
			{
				luAlarmBitStatus |= ALARM_BIT_LOW;
			}

			if (2 == luParsedStatus || 5 == luParsedStatus)
			{
				luAlarmBitStatus |= ALARM_BIT_HIGH;

				return ALARM_STATUS_HIGH;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	try
	{
		CMutexRWReader safeProcReader(this->_pLockProcData);

		V_PCHANNINFO::iterator itProc;
		for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
		{
			unsigned int luAlarmStatus = ((CProcChannParamAlarm*)(*itProc))->GetAlarmStatus();
			int  luParsedStatus = ::AlarmStatus_ParseAlarmStatus(luAlarmStatus);

			if (luParsedStatus > 0)
			{
				luAlarmBitStatus |= ALARM_BIT_LOW;
			}

			if (2 == luParsedStatus)
			{
				luAlarmBitStatus |= ALARM_BIT_HIGH;

				return ALARM_STATUS_HIGH;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

//#if 0 /* [FIXME] 不知道为什么GetPlantAlarmState注掉了对动态数据的处理， 因之前并无问题，暂不打开 */
	try
	{
		CMutexRWReader safeProcReader(this->_pLockDynData);

		V_PCHANNINFO::iterator itDyn;
		for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
		{
			unsigned int luAlarmStatus = ((CDynChannParamAlarm*)*itDyn)->GetAlarmStatus();
			int  luParsedStatus = ::AlarmStatus_ParseAlarmStatus(luAlarmStatus);

			if (luParsedStatus > 0)
			{
				luAlarmBitStatus |= ALARM_BIT_LOW;
			}

			if (2 == luParsedStatus)
			{
				luAlarmBitStatus |= ALARM_BIT_HIGH;

				return ALARM_STATUS_HIGH;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

//#endif


	if (luAlarmBitStatus > 0)
	{
		if (luAlarmBitStatus & ALARM_BIT_HIGH)
		{
			return ALARM_STATUS_HIGH;
		}

		return ALARM_STATUS_LOW;
	}


	return ALARM_STATUS_NONE;
}


int CAlarmPlantInfo::UpdateProcData(float *pfData_,int iLen_,const CBHDateTime& tDateTime_,int iStartIndex_)
{
	this->_timeCurrProc = CBHDateTime::GetCurrentTimeEx();
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	int nCount=0;
	int iloop=0;

	int lnLowIndex = CConfigHelper::GetLowPressRevIndex();
	int lnHighIndex = CConfigHelper::GetHighPressRevIndex();
	int lnPowerIndex = CConfigHelper::GetPowerIndex(this->GetPlantNo().c_str());

	if (((iLen_+iStartIndex_)> _sPlantInfo._iProcChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		SetProcDataUpdateStatus(tDateTime_);
		iLen_=iLen_+iStartIndex_;
		nCount=0;

		CProcChannParamAlarm * pParam;
		S_ChannInfo sChannInfo;
		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=iStartIndex_;iloop<iLen_;++iloop)
			{
				((CProcChannParamAlarm*)_vProcChann[iloop])->SetData(pfData_[nCount],_timeProcDataUpdate);

				((CProcChannParamAlarm*)_vProcChann[iloop])->SetLastUpdateTime(CBHDateTime::GetCurrentTime());

				((CProcChannParamAlarm*)_vProcChann[iloop])->SetChannUpdate(true);

				if(lnLowIndex >= 0 && nCount == lnLowIndex )
				{
					this->SetLowPressRev((int)pfData_[nCount]);
				}
				if(lnHighIndex >= 0 && nCount == lnHighIndex)
				{
					this->SetHighPressRev((int)pfData_[nCount]);
				}
				if(lnPowerIndex >= 0 && nCount == lnPowerIndex)
				{
					this->SetPlantPower(pfData_[nCount]);
				}

				nCount++;
			}

			if (m_fMaxRealRev <= 0)
			{
				_pfRealRev[0] = -1;
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}

	return nCount;
}

int CAlarmPlantInfo::UpdateProcSUDStatus(int anProcJudgedPlantStatus,int anLen_,int anStartIndex_)
{
	int nCount=0;
	int iloop=0;
	if (((anLen_+anStartIndex_)> _sPlantInfo._iProcChannNum)||(anLen_<1)||(anStartIndex_<0))
		nCount=-1;
	else
	{
		anLen_=anLen_+anStartIndex_;
		nCount=0;

		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=anStartIndex_;iloop<anLen_;++iloop)
			{
				((CProcChannParamAlarm*)_vProcChann[iloop])->SetSUDStatus(anProcJudgedPlantStatus);
				nCount++;
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}

	return nCount;
}

int CAlarmPlantInfo::GetProcSUDStatus()
{
	int lnSpeedCount = 0;

	int lnStopCount = 0;

	bool lbJudgeSUDStatus = false;

	_pLockProcData->BeginRead();
	
	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			bool lbUpdateDataOverTime = ((CProcChannParamAlarm*)_vProcChann[lnProcChannIndex])->IsChannUpdateDataOverTime();
			//如果更新数据时间超时则认为无效，不参与开停车判断
			if (lbUpdateDataOverTime)
			{
				continue;
			}

			int lnProcSUDStatus = ((CProcChannParamAlarm*)_vProcChann[lnProcChannIndex])->GetSUDStatus();
			//0表示不参与判断开停车，1表示开车，2表示停车
			if (lnProcSUDStatus == 0) 
			{
				continue;
			}

			lbJudgeSUDStatus = true;

			if (lnProcSUDStatus == 1)
			{
				++lnSpeedCount;
			}
			else if(lnProcSUDStatus == 2)
			{
				++lnStopCount;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	if (!lbJudgeSUDStatus)
	{
		return 0;
	}

	return lnStopCount<lnSpeedCount?1:2;
}

int CAlarmPlantInfo::ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,S_OldVibCharValue *pOldVibCharValue_,float *pfRev_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_,bool bEnd_/* =false */)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//更新特征值
		int iRealSmpNum=iSmpNum_> _sPlantInfo._iSmpNum? _sPlantInfo._iSmpNum:iSmpNum_;
		nCount=0;
		_pLockVibData->BeginWrite();
		try
		{
			for(int loop=iStartIndex_;loop<(iStartIndex_ + iLen_);++loop)
			{
				float fRev=.0;
				int iRevType=((CVibChannParam*)_vVibChann[loop])->GetChannInfo()._iRevType;;
				switch(iRevType)
				{
				case 1:
					fRev = pfRev_[0];
					break;
				case 2:
					fRev = pfRev_[1];
					break;
				case 3:
					fRev = pfRev_[2];
					break;
				case 4:
					fRev = pfRev_[3];
					break;
				case 5:
					fRev = pfRev_[4];
					break;
				default:
					fRev = pfRev_[0];
				}
				pVibCharValue_[nCount]._fCharAll=pOldVibCharValue_[nCount]._fCharAll;
				pVibCharValue_[nCount]._fCharHalf=pOldVibCharValue_[nCount]._fCharHalf;
				pVibCharValue_[nCount]._fCharOne=pOldVibCharValue_[nCount]._fCharOne;
				pVibCharValue_[nCount]._fCharTwo=pOldVibCharValue_[nCount]._fCharTwo;
				pVibCharValue_[nCount]._fCharThree=pOldVibCharValue_[nCount]._fCharThree;
				pVibCharValue_[nCount]._fCharRes=pOldVibCharValue_[nCount]._fCharRes;
				pVibCharValue_[nCount]._fDC=pOldVibCharValue_[nCount]._fDC;
				pVibCharValue_[nCount]._fGap=pOldVibCharValue_[nCount]._fGap;
				pVibCharValue_[nCount]._fPhaseHalf=pOldVibCharValue_[nCount]._fPhaseHalf;
				pVibCharValue_[nCount]._fPhaseOne=pOldVibCharValue_[nCount]._fPhaseOne;
				pVibCharValue_[nCount]._fPhaseTwo=pOldVibCharValue_[nCount]._fPhaseTwo;
				pVibCharValue_[nCount]._fPhaseThree=pOldVibCharValue_[nCount]._fPhaseThree;
				pVibCharValue_[nCount]._iSmpFreq=iSmpFreq_;
				pVibCharValue_[nCount]._iSmpNum=iSmpNum_;
				pVibCharValue_[nCount]._iRev=(int)fRev;
				nCount++;
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndWrite();
	}
	return nCount;
}

int CAlarmPlantInfo::UpdateVibData(DATATYPE_WAVE *pWave_,
								   S_VibCharValue *pVibCharValue_,	//临时生成的测点数组，长度由数采发过来的测点个数决定，索引从0开始
								   int iAllRealNum_,				//数采发过来的全部的采样点数
								   int iLen_,						//测点个数
								   CBHDateTime & arefDate,           //更新时间
								   int iStartIndex_/* =0 */,		//发送所有实时数据时的起始索引
								   const float& fScaleCoef_,
								   int anPrecisionEnabled,
								   std::vector<float>& afPreciousMin ,
								   std::vector<float>&  afPreciousMax,
								   bool bEnd_/* =false */)			//判断这台设备所有振动通道这次数据更新完成
{
	this->_timeCurrVib = CBHDateTime::GetCurrentTimeEx();
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	int nCount=0;

	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
	{
		nCount=-1;
	}
	else
	{
		if (bEnd_)
		{
			SetVibDataUpdateStatus(arefDate);
			SetRealAllVibWaveNum(_iVibAllWaveNum,true);
			_iVibAllWaveNum=0;
		}

		nCount=0;
		iLen_=iLen_+iStartIndex_;

		//进行数据更新
		_iVibAllWaveNum=_iVibAllWaveNum+iAllRealNum_;

		//更新采样频率、采样点数，其值由数据库组态的采样频率、采样点数决定
		int iRealSmpFreq=pVibCharValue_[nCount]._iSmpFreq > _sPlantInfo._iSmpFreq? _sPlantInfo._iSmpFreq:pVibCharValue_[nCount]._iSmpFreq;
		int iRealSmpNum=pVibCharValue_[nCount]._iSmpNum> _sPlantInfo._iSmpNum? _sPlantInfo._iSmpNum:pVibCharValue_[nCount]._iSmpNum;
		SetRealSmpInfo(iRealSmpNum, iRealSmpFreq, true);

		//更新转速,每一个测点最多有5级转速

		float pfRev[5]={.0};
		int loop=0;
		long iRealPointNum=0;

		CWaveHead sWaveHead;
		sWaveHead.SetLength(4);

		_pLockVibData->BeginWrite();

		try
		{
			if(iStartIndex_ == 0)
			{
				for(int loop = 0; loop < _vVibChann.size(); ++loop)
				{
					((CVibChannParamAlarm*)_vVibChann[loop])->SetChannUpdate(false);
				}
			}
			//根据测点个数，批次更新内存的转速、特征值、波形
			for(loop=iStartIndex_;loop<iLen_;++loop)
			{
				((CVibChannParamAlarm*)_vVibChann[loop])->SetLastUpdateTime(CBHDateTime::GetCurrentTime());

				((CVibChannParamAlarm*)_vVibChann[loop])->SetChannUpdate(true);


				int iRevType=((CVibChannParamAlarm*)_vVibChann[loop])->GetChannInfo()._iRevType;;

				switch(iRevType)
				{
				case 1:
					pfRev[0] = pVibCharValue_[nCount]._iRev;
					break;
				case 2:
					pfRev[1] = pVibCharValue_[nCount]._iRev;
					break;
				case 3:
					pfRev[2] = pVibCharValue_[nCount]._iRev;
					break;
				case 4:
					pfRev[3] = pVibCharValue_[nCount]._iRev;
					break;
				case 5:
					pfRev[4] = pVibCharValue_[nCount]._iRev;
					break;
				default:
					pfRev[0] = pVibCharValue_[nCount]._iRev;
				}

				if ( 0 == loop)
				{
					this->MaxRealRev(0);
				}

				if(iRevType > 0 && iRevType <= 5)
				{
					_pfRealRev[iRevType - 1] = pVibCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<_pfRealRev[iRevType - 1])
					{
						m_fMaxRealRev = _pfRealRev[iRevType - 1];
					}
				}
				else 
				{
					_pfRealRev[0] = pVibCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<_pfRealRev[0])
					{
						m_fMaxRealRev = _pfRealRev[0];
					}
				}

				((CVibChannParamAlarm*)_vVibChann[loop])->SetChannValue(&(pVibCharValue_[nCount]));

				if ((iRealPointNum + pVibCharValue_[nCount]._iSmpNum) <= iAllRealNum_)
				{
					if(pWave_)
					{
						((CVibChannParamAlarm*)_vVibChann[loop])->SetChannWave(&(pWave_[iRealPointNum]),pVibCharValue_[nCount]._iSmpNum);
					}
					else
					{
						((CVibChannParamAlarm*)_vVibChann[loop])->SetChannWave(NULL,0);
					}
					
				}else
				{
					/*ASSERT(FALSE);*/
					CString lstrErrorLog;
					lstrErrorLog.Format("Error:24001接口 %s|%s|%s 一次更新的累计的采样点数iRealPointNum(%d) >=  数采传送过来的总采样点数 iAllRealNum_(%d)",
						this->_sCompany.c_str(),
						this->_sFactory.c_str(),
						this->_sSetID.c_str(),
						iRealPointNum,
						iAllRealNum_);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
					
				}				

				iRealPointNum+=pVibCharValue_[nCount]._iSmpNum;

				if(IsFloatZero(fScaleCoef_))
				{
					sWaveHead.m_pParam[0] = 10.0;
				}
				else
				{
					sWaveHead.m_pParam[0] = fScaleCoef_;
				}

				if(anPrecisionEnabled)
				{
					sWaveHead.m_pParam[1] = 1;
					sWaveHead.m_pParam[2] = afPreciousMin[nCount];
					sWaveHead.m_pParam[3] = afPreciousMax[nCount];
				}
				else
				{
					sWaveHead.m_pParam[1] = 0;

				}
				((CVibChannParamAlarm*)_vVibChann[loop])->WaveHead(sWaveHead);


				nCount++;



			}//end of for(loop=iStartIndex_ : 根据测点个数批次更新内存的转速、特征值、波形
	
			if ((iLen_== _sPlantInfo._iVibChannNum)&&
				(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
				_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR &&
				_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL))
			{
				for (loop=0;loop<_iAxesPairNum;++loop)
					((CAxisAlarmChannParam*)_vAxisChann[loop])->UpdateData();
				
			}

			this->MaxRealRevLast(this->MaxRealRev());
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}
		
		_pLockVibData->EndWrite();
		//更新转速
		//UpdateRealRev(pfRev);
	}
	return nCount;
}

int CAlarmPlantInfo::ChangeDynharValueToNew(S_DynCharValue *pDynCharValue_,S_OldDynCharValue *pOldDynCharValue_,float *pfRev_,const int &iSmpNum_, const int &iSmpFreq_,const int iRealNum_,const int &iLen_,const int &iStartIndex_, const bool &bEnd_/* =false */)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		//转化特征值
		_pLockDynData->BeginWrite();
		try
		{
			for(int loop=iStartIndex_; loop< (iStartIndex_ + iLen_); ++loop)
			{
				float fRev=.0;
				int iRevType=((CDynChannParam*)_vDynChann[loop])->GetChannInfo()._iRevType;
				switch(iRevType)
				{
				case 1:
					fRev = pfRev_[0];
					break;
				case 2:
					fRev = pfRev_[1];
					break;
				case 3:
					fRev = pfRev_[2];
					break;
				case 4:
					fRev = pfRev_[3];
					break;
				case 5:
					fRev = pfRev_[4];
					break;
				default:
					fRev = pfRev_[0];
				}
				pDynCharValue_[nCount]._fCharValue0=pOldDynCharValue_[nCount]._fCharValue0;
				pDynCharValue_[nCount]._fCharValue1=pOldDynCharValue_[nCount]._fCharValue1;
				pDynCharValue_[nCount]._fCharValue2=pOldDynCharValue_[nCount]._fCharValue2;
				pDynCharValue_[nCount]._fCharValue3=pOldDynCharValue_[nCount]._fCharValue3;
				pDynCharValue_[nCount]._fCharValue4=pOldDynCharValue_[nCount]._fCharValue4;
				pDynCharValue_[nCount]._fCharValue5=pOldDynCharValue_[nCount]._fCharValue5;
				pDynCharValue_[nCount]._fCharValue6=pOldDynCharValue_[nCount]._fCharValue6;
				pDynCharValue_[nCount]._fCharValue7=pOldDynCharValue_[nCount]._fCharValue7;
				pDynCharValue_[nCount]._fCharValue8=pOldDynCharValue_[nCount]._fCharValue8;
				pDynCharValue_[nCount]._fCharValue9=pOldDynCharValue_[nCount]._fCharValue9;
				pDynCharValue_[nCount]._fCharValue10=pOldDynCharValue_[nCount]._fCharValue10;
				pDynCharValue_[nCount]._fCharValue11=pOldDynCharValue_[nCount]._fCharValue11;
				pDynCharValue_[nCount]._iSmpFreq=iSmpFreq_;
				pDynCharValue_[nCount]._iSmpNum=iSmpNum_;
				pDynCharValue_[nCount]._iRev=(int)fRev;
				nCount++;
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockDynData->EndWrite();
	}
	return nCount;
}

int CAlarmPlantInfo::UpdateDynData(DATATYPE_WAVE *pWave_,
									S_DynCharValue *pDynCharValue_,
									int iAllRealNum_,
									const int &iLen_,
									CBHDateTime & arefDate,           //更新时间
									const int &iStartIndex_, 
									const float& fScaleCoef_,
									int anPrecisionEnabled,
									std::vector<float>& afPreciousMin ,
									std::vector<float>&  afPreciousMax,
									const bool &bEnd_/* =false */)
{
	this->_timeCurrDyn = CBHDateTime::GetCurrentTimeEx();
	this->TimeLastUpdateTime(COleDateTime::GetCurrentTime());
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		if (bEnd_)
		{
			SetDynDataUpdateStatus(arefDate);
			SetRealAllDynWaveNum(_iDynAllWaveNum,true);
			_iDynAllWaveNum=0;
		}
		//进行数据更新
		_iDynAllWaveNum=_iDynAllWaveNum+iAllRealNum_;
		int iRealSmpFreq = pDynCharValue_[nCount]._iSmpFreq > _sPlantInfo._iSmpFreq ? _sPlantInfo._iSmpFreq : pDynCharValue_[nCount]._iSmpFreq;
		int iRealSmpNum = pDynCharValue_[nCount]._iSmpNum> _sPlantInfo._iSmpNum? _sPlantInfo._iSmpNum : pDynCharValue_[nCount]._iSmpNum;
		SetDynRealSmpInfo(iRealSmpNum, iRealSmpFreq, true);
		float pfRev[5]={.0};
		int loop=0;
		long iRealPointNum=0;
		nCount=0;
		CWaveHead sWaveHead;
		sWaveHead.SetLength(4);
		_pLockDynData->BeginWrite();
		try
		{
			if(iStartIndex_ == 0)
			{
				for(int loop = 0; loop < _vDynChann.size(); ++loop)
				{
					((CDynChannParamAlarm*)_vDynChann[loop])->SetChannUpdate(false);
				}
			}

			for(loop=iStartIndex_;loop< (iStartIndex_ + iLen_);++loop)
			{
				((CDynChannParamAlarm*)_vDynChann[loop])->SetLastUpdateTime(CBHDateTime::GetCurrentTime());
				((CDynChannParamAlarm*)_vDynChann[loop])->SetChannUpdate(true);

				int iRevType=((CDynChannParamAlarm*)_vDynChann[loop])->GetChannInfo()._iRevType;;
				switch(iRevType)
				{
				case 1:
					pfRev[0] = pDynCharValue_[nCount]._iRev;
					break;
				case 2:
					pfRev[1] = pDynCharValue_[nCount]._iRev;
					break;
				case 3:
					pfRev[2] = pDynCharValue_[nCount]._iRev;
					break;
				case 4:
					pfRev[3] = pDynCharValue_[nCount]._iRev;
					break;
				case 5:
					pfRev[4] = pDynCharValue_[nCount]._iRev;
					break;
				default:
					pfRev[0] = pDynCharValue_[nCount]._iRev;
				}

				if ( 0 == loop)
				{
					this->MaxRealRev(0);
				}

				if(iRevType > 0 && iRevType <= 5)
				{
					_pfRealRev[iRevType - 1] = pDynCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<_pfRealRev[iRevType - 1])
					{
						m_fMaxRealRev = _pfRealRev[iRevType - 1];
					}
				}
				else 
				{
					_pfRealRev[0] = pDynCharValue_[nCount]._iRev;

					if (m_fMaxRealRev<_pfRealRev[0])
					{
						m_fMaxRealRev = _pfRealRev[0];
					}
				}

				//CString sTemp;sTemp.Format("_vDynChann[%d]=%s",loop,(CDynChannParamAlarm*)_vDynChann[loop]->GetChannInfo()._cChannID);BHTRACE_DEBUG(sTemp);
				((CDynChannParamAlarm*)_vDynChann[loop])->SetChannValue(&(pDynCharValue_[nCount]));
				
				if ((iRealPointNum + pDynCharValue_[nCount]._iSmpNum )<= iAllRealNum_)
				{
					((CDynChannParamAlarm*)_vDynChann[loop])->SetChannWave(&(pWave_[iRealPointNum]),pDynCharValue_[nCount]._iSmpNum);
				}else
				{
					CString lstrErrorLog;
					lstrErrorLog.Format("Error:24001接口 %s|%s|%s 一次更新的累计的采样点数iRealPointNum(%d) >=  数采传送过来的总采样点数 iAllRealNum_(%d)",
						this->_sCompany.c_str(),
						this->_sFactory.c_str(),
						this->_sSetID.c_str(),
						iRealPointNum,
						iAllRealNum_);
					CHZLogManage::Error(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				iRealPointNum+=pDynCharValue_[nCount]._iSmpNum;

				if(IsFloatZero(fScaleCoef_))
				{
					sWaveHead.m_pParam[0] = 10.0;
				}
				else
				{
					sWaveHead.m_pParam[0] = fScaleCoef_;
				}

				if(anPrecisionEnabled)
				{
					sWaveHead.m_pParam[1] = 1;
					sWaveHead.m_pParam[2] = afPreciousMin[nCount];
					sWaveHead.m_pParam[3] = afPreciousMax[nCount];
				}
				else
				{
					sWaveHead.m_pParam[1] = 0;

				}
				((CDynChannParamAlarm*)_vDynChann[loop])->WaveHead(sWaveHead);


				nCount++;

			}

			this->MaxRealRevLast(this->MaxRealRev());
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		if (bEnd_)
		{
			for (int i =0;i<_vDynChann.size();i++)
			{
				((CDynChannParamAlarm*)_vDynChann[i])->SyncData();
			}
		}
		_pLockDynData->EndWrite();
		//更新转速
		//UpdateRealRev(pfRev);
	}
	return nCount;
}



int CAlarmPlantInfo::CalcCustomParamData(int iStartIndex_,int iLen_,CHANNTYPE channType)
{
	return CPlantInfoOnLine::CalcCustomParamData(iStartIndex_,iLen_,channType);

	int loop = 0, loopCustom =0,iRtn=0;
	CCustomParamInfo paramInfo;
	float fBuf[GD_CUSTOMPARAM_BUFFSIZE] ={.0};
	int iSize = 0;
	switch(channType)
	{
	case GE_VIBCHANN://径向振动
		if((iStartIndex_ + iLen_)< _sPlantInfo._iVibChannNum)
		{
			CVibChannParamAlarm * pVib = NULL;
			S_VibCharValue vibCharValue;
			for (loop = 0; loop < iLen_; ++loop)
			{
				pVib = (CVibChannParamAlarm*)_vVibChann[iStartIndex_+ loop];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pVib->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pVib->GetGearBoxParamInfo();
				CChannLocations chanLocation=pVib->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pVib->GetCustomParamSize();
				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
						continue;
					_pLockVibData->BeginRead();
					int iCharIndex=pVib->GetCurrCharValueIndex();
					vibCharValue = pVib->GetCharValue(iCharIndex);
					_pLockVibData->EndRead();
					CalcBearingDefectFreq(bearingParamInfo,vibCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,vibCharValue._iRev,gearboxCharFreq);
					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					this->_pLockVibData->BeginRead();
					try
					{
						CalcSingleCustomParamData(paramInfo,defectFreq,gearboxCharFreq,pVib->GetCurrWavePoint(),vibCharValue._iSmpNum, vibCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,vibCharValue._iRev,&(fBuf[0]),pVib->WaveHead());
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					this->_pLockVibData->EndRead();

					_pLockVibData->BeginWrite();

					try
					{
						memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	case GE_AXIALCHANN://轴向振动
		if((iStartIndex_ + iLen_)< _iAxialVibChannNum)
		{
			for (loop = 0; loop < iLen_; ++loop)
			{
				CVibChannParamAlarm * pVib = NULL;
				S_VibCharValue vibCharValue;
				pVib = (CVibChannParamAlarm*)_vVibChann[iStartIndex_+loop+_iRadialVibChannNum];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pVib->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pVib->GetGearBoxParamInfo();
				CChannLocations chanLocation=pVib->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pVib->GetCustomParamSize();
				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo = pVib->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
						continue;
					_pLockVibData->BeginRead();
					int iCharIndex=pVib->GetCurrCharValueIndex();
					vibCharValue = pVib->GetCharValue(iCharIndex);
					_pLockVibData->EndRead();
					CalcBearingDefectFreq(bearingParamInfo,vibCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,vibCharValue._iRev,gearboxCharFreq);
					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecVibChannStartEndIndex(pVib->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					_pLockVibData->BeginRead();

					try
					{
						CalcSingleCustomParamData(paramInfo,defectFreq,gearboxCharFreq,pVib->GetCurrWavePoint(),vibCharValue._iSmpNum, vibCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,vibCharValue._iRev,&(fBuf[0]),pVib->WaveHead());
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndRead();

					_pLockVibData->BeginWrite();

					try
					{
						memcpy(pVib->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockVibData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	case GE_RODSINKCHANN://动态测点活塞杆沉降量
		if((iStartIndex_ + iLen_)< _sPlantInfo._iDynChannNum)
		{
			CDynChannParamAlarm * pDyn = NULL;
			S_DynCharValue dynCharValue;
			for (loop = 0; loop < iLen_; ++loop)
			{
				pDyn = (CDynChannParamAlarm*)_vDynChann[iStartIndex_+ loop];
				//计算轴承和齿轮特征频率
				CBearingParamInfo bearingParamInfo=pDyn->GetBearingParamInfo();
				CGearBoxParamInfo gearBoxParamInfo=pDyn->GetGearBoxParamInfo();
				CChannLocations chanLocation=pDyn->GetChannLocationsInfo();
				S_BearingDefectFreq defectFreq;
				S_GearBoxCharFreq gearboxCharFreq;
				iSize = pDyn->GetCustomParamSize();
				//CString sTemp;sTemp.Format("pDyn->GetCustomParamSize():%d",iSize);BHTRACE_DEBUG(sTemp);
				for( loopCustom = 0; loopCustom < iSize; ++loopCustom)
				{
					paramInfo =pDyn->GetSpecIndexCustomParamInfo(loopCustom);
					if (paramInfo._iID < 0)
						continue;
					_pLockDynData->BeginRead();
					int iCharIndex=pDyn->GetCurrCharValueIndex();
					dynCharValue = pDyn->GetCharValue(iCharIndex);
					_pLockDynData->EndRead();

					CalcBearingDefectFreq(bearingParamInfo,dynCharValue._iRev,defectFreq);
					CalcGearboxCharFreq(gearBoxParamInfo,dynCharValue._iRev,gearboxCharFreq);
					int iPeriodStartIndex=0,iPeriodEndIndex=0;
					GetSpecDynChannStartEndIndex(pDyn->GetChannInfo()._cChannID,iPeriodStartIndex,iPeriodEndIndex);

					this->_pLockDynData->BeginRead();
					try
					{
						CalcSingleCustomParamData(paramInfo,defectFreq,gearboxCharFreq,pDyn->GetCurrWavePoint(),dynCharValue._iSmpNum, dynCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,dynCharValue._iRev,&(fBuf[0]),pDyn->WaveHead());
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					this->_pLockDynData->EndRead();
					//CString sTemp;sTemp.Format("pDyn:_iSmpNum:%d,_iSmpFreq:%d,iPeriodStartIndex:%d,iPeriodEndIndex:%d,_iRev:%d",dynCharValue._iSmpNum, dynCharValue._iSmpFreq ,iPeriodStartIndex,iPeriodEndIndex,dynCharValue._iRev);BHTRACE_DEBUG(sTemp);


					_pLockDynData->BeginWrite();

					try
					{
						memcpy(pDyn->GetRealCustomParamData(), &(fBuf[0]), GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);
					}

					_pLockDynData->EndWrite();
				}
			}
			iRtn=1;
		}
		break;
	default:
		iRtn = -1;
	}
	return iRtn;
}



int CAlarmPlantInfo::UpdateVibFAlarmStatus(DATATYPE_ALARMSTATE *piStatus_,int iLen_,int iStartIndex_,bool bEnd_)
{
	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		nCount=-1;
	else
	{
		nCount=0;
		iLen_=iLen_+iStartIndex_;
		//进行数据更新
		_pLockVibData->BeginWrite();

		try
		{
			int loop=0;
			for(loop=iStartIndex_;loop<iLen_;++loop)
			{
				nCount++;
			}
			if (iLen_== _sPlantInfo._iVibChannNum)
			{
				for (loop=0;loop<_iAxesPairNum;++loop)
					((CAxisAlarmChannParam*)_vAxisChann[loop])->UpdateData();
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndWrite();
	}
	return nCount;
}

bool CAlarmPlantInfo::InitEventID(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	//CHZLogManage::Debug(sTemp,"CAlarmPlantInfo","InitEventID");
	CHZLogManage::Info("Start","CAlarmPlantInfo","InitEventID");
	bool bRtn=false;
	//初始化报警日志表事件ID
	pDB_->Cmd("SELECT TOP 1 [%s],[%s] from [%s] ORDER BY [%s] DESC ",gc_cEventID,gc_cID,_sAlarmLogTblName.c_str(),gc_cEventID);//gc_cEventID替代gc_cStart_T
	if (pDB_->More())
	{
		_iEventID=pDB_->GetLong(gc_cEventID);
		//CString sTemp;sTemp.Format("初始化事件ID:%d",_iEventID);AfxMessageBox(sTemp);
		_iID=pDB_->GetLong(gc_cID);
		bRtn=true; 
	}
	else
	{
		_iEventID=0;
		_iID=0;
		bRtn=true;
	}
	_iTrendEventID=0;
	//初始化门限值ID
	pDB_->Cmd("SELECT TOP 1 [%s] from [%s] ORDER BY [%s] DESC ",gc_cThreshold_ID,_sThTblName.c_str(),gc_cThreshold_ID);//gc_cThreshold_ID替代gc_cEnd_T
	if (pDB_->More())
	{
		_iThID=pDB_->GetLong(gc_cThreshold_ID);
		bRtn=true;
	}
	else
		_iThID=0;
	CHZLogManage::Info("End","CAlarmPlantInfo","InitEventID");
	return bRtn;
}


bool CAlarmPlantInfo::InitEventID(CCustomDB *pDB_)
{
	SwichDbName(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	//CString sTemp;sTemp.Format("得到的分数据库名:%s",(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	//CHZLogManage::Debug(sTemp,"CAlarmPlantInfo","InitEventID");
	CHZLogManage::Info("Start","CAlarmPlantInfo","InitEventID");
	bool bRtn=false;
	//初始化报警日志表事件ID
	pDB_->Cmd("SELECT TOP 1 [%s],[%s] from [%s] ORDER BY [%s] DESC ",gc_cEventID,gc_cID,_sAlarmLogTblName.c_str(),gc_cEventID);//gc_cEventID替代gc_cStart_T
	if (pDB_->More())
	{
		_iEventID=pDB_->GetLong(gc_cEventID);
		//CString sTemp;sTemp.Format("初始化事件ID:%d",_iEventID);AfxMessageBox(sTemp);
		_iID=pDB_->GetLong(gc_cID);

		if (g_UseNewAlarmLogic)
		{
			this->_iEventIdReg = this->_iEventID;
		}

		bRtn=true; 
	}
	else
	{
		_iEventID=0;
		_iID=0;
		if (g_UseNewAlarmLogic)
		{
			this->_iEventIdReg = this->_iEventID;
		}
		bRtn=true;
	}
	_iTrendEventID=0;
	//初始化门限值ID
	pDB_->Cmd("SELECT TOP 1 [%s] from [%s] ORDER BY [%s] DESC ",gc_cThreshold_ID,_sThTblName.c_str(),gc_cThreshold_ID);//gc_cThreshold_ID替代gc_cEnd_T
	if (pDB_->More())
	{
		_iThID=pDB_->GetLong(gc_cThreshold_ID);
		bRtn=true;
	}
	else
		_iThID=0;
	CHZLogManage::Info("End","CAlarmPlantInfo","InitEventID");
	return bRtn;
}


int CAlarmPlantInfo::AddAlarmLog(IDBInterFace *pDB_,string sName_,long & iID_,long & iEventID_,CBHDateTime dtStart_,int iType_,int iChannType_,int iThID_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int iRtn=-1;
	{
		int lnRev = this->MaxRealRevLast();
		int lnPower = this->GetPlantPower()*CConfigHelper::GetPowerRatio(_sPlantInfo._cPlantNo);

		CString lstrChannNo = sName_.c_str();
		int lnAlarmLevel = 0;

		int lnIndex = GetSpecVibChannIndexNo(sName_);

		if(lnIndex >= 0 && lnIndex < this->_vVibChann.size())
		{
			lnAlarmLevel = ((CVibChannParamAlarm*)_vVibChann[lnIndex])->GetAlarmLevel();
		}

		if(lnIndex < 0)
		{
			lnIndex = GetSpecDynChannIndexNo(sName_);
			if(lnIndex >= 0 && lnIndex < this->_vDynChann.size())
			{
				lnAlarmLevel = ((CDynChannParamAlarm*)_vDynChann[lnIndex])->GetAlarmLevel();
			}
		}
		if(lnIndex < 0)
		{
			lnIndex = GetSpecProcChannIndexNo(sName_);
			if(lnIndex >= 0 && lnIndex < this->_vProcChann.size())
			{
				lnAlarmLevel = ((CProcChannParamAlarm*)_vProcChann[lnIndex])->GetAlarmLevel();
			}
		}

		CString lstrWorkStatus;

		int lnRevLowerBound = (lnRev/20)*20;
		int lnRevUpperBound = lnRevLowerBound + 19;

		int lnPowerSpliteFactor = CConfigHelper::GetPowerSpliteFactor(_sPlantInfo._cPlantNo);

		if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_TURB)
		{
			int lnLowRev = this->GetLowPressRev();
			int lnHighRev = this->GetHighPressRev();

			int lnPowerLowerBound = (lnLowRev/lnPowerSpliteFactor)*lnPowerSpliteFactor;
			int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

			int lnPower1LowerBound = (lnHighRev/lnPowerSpliteFactor)*lnPowerSpliteFactor;
			int lnPower1UpperBound = lnPower1LowerBound + lnPowerSpliteFactor - 1;

			lstrWorkStatus.Format("%d-%d+%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound,lnPower1LowerBound,lnPower1UpperBound);
 		}
		else
		{
			int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
			int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

			lstrWorkStatus.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);

		}

		
		bool lbInsertWorkStatus = false;
		bool lbInsertAlarmLevel = false;

		pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmLogTblName.c_str(),gc_cWorkStatus);
		if(!pDB_->More())
		{
			pDB_->Cmd("ALTER TABLE %s ADD %s CHAR(100) NULL ",_sAlarmLogTblName.c_str(),gc_cWorkStatus);
			if(pDB_->Exec())
			{
				lbInsertWorkStatus = true;
			}
		}
		else
		{
			lbInsertWorkStatus = true;
		}

		pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmLogTblName.c_str(),gc_cAlarmLevel);
		if(!pDB_->More())
		{
			pDB_->Cmd("ALTER TABLE %s ADD %s INT NULL ",_sAlarmLogTblName.c_str(),gc_cAlarmLevel);
			if(pDB_->Exec())
			{
				lbInsertAlarmLevel = true;
			}
		}
		else
		{
			lbInsertAlarmLevel = true;
		}


		//默认报警完成后新学习的阈值ID为0
		pDB_->Cmd("INSERT INTO [%s](",_sAlarmLogTblName.c_str());
		pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",gc_cID,gc_cEventID,gc_cName,gc_cStart_T,gc_cEnd_T,gc_cStop_Flag,gc_cSynFlag,gc_cType,gc_cChannType,gc_cThreshold_ID,gc_cThreshold_ID_New);
		if(lbInsertWorkStatus)
		{
			pDB_->Cmd(",%s",gc_cWorkStatus);
		}
		if(lbInsertAlarmLevel)
		{
			pDB_->Cmd(",%s",gc_cAlarmLevel);
		}

		pDB_->Cmd(") VALUES(%d,%d,'%s','%s','%s',0,0,%d,%d,%d,0 ",iID_,iEventID_,sName_.c_str(),TimeConvertString(dtStart_),TimeConvertString(dtStart_),iType_,iChannType_,iThID_);

		if(lbInsertWorkStatus)
		{
			pDB_->Cmd(",'%s'",lstrWorkStatus);
		}
		if(lbInsertAlarmLevel)
		{
			pDB_->Cmd(",%d",lnAlarmLevel);
		}
		pDB_->Cmd(")");

		//添加报警日志
		if (pDB_->Exec())
		{
			iRtn=1;
		}
		else
		{
			iRtn=1;

		}
	}
	return iRtn;
}


int CAlarmPlantInfo::SetAllAlarmLogEnd(IDBInterFace *pDB_,CBHDateTime & areBHTime)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	pDB_->Cmd("update [%s] ",_sAlarmLogTblName.c_str());
	pDB_->Cmd(" set %s='%s',%s=1,%s=0 ",gc_cEnd_T,TimeConvertString(areBHTime),gc_cStop_Flag,gc_cSynFlag);
	pDB_->Cmd(" where   %s=0  ",gc_cStop_Flag);
	//更新报警结束时间
	if (pDB_->Exec())
	{
		iRtn=0;
	}
	else
	{
		iRtn=-1;
	}
	return iRtn;
}

int CAlarmPlantInfo::SetAlarmLogEndTimeAndNewThID(IDBInterFace *pDB_,string sName_,int iID_,int iEventID_,CBHDateTime dtEnd_,int iChannType_,int iThID_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
	pDB_->Cmd("update [%s] ",_sAlarmLogTblName.c_str());
	pDB_->Cmd(" set %s='%s',%s=%d,%s=1,%s=0 ",gc_cEnd_T,TimeConvertString(dtEnd_),gc_cThreshold_ID,iThID_,gc_cStop_Flag,gc_cSynFlag);
	pDB_->Cmd(" where %s=%d and %s='%s' and %s=0  ",gc_cEventID,iEventID_,gc_cName,sName_.c_str(),gc_cStop_Flag);
	//更新报警结束时间
	if (pDB_->Exec())
	{
		iRtn=0;
	}
	else
	{
		iRtn=-1;
	}
	return iRtn;
}

///返回大于0,就是事件ID;-2表示添加一条报警事件的日志失败;-1表示在一条报警事件中添加一条小事件日志失败
int CAlarmPlantInfo::AddRegAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_)
{
	long iRtn=-1;//如果大于0，就是事件ID
	if (_iEventIdReg==-1)
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			iRtn=_iEventID+1;
	
			//添加报警日志
			if (AddAlarmLog(pDB_,sName_,iRtn,iRtn,dtStart_,iType_,iChannType_,-1)>0)
			{
				_iEventID=iRtn;
	
				_iEventIdReg=_iEventID;

			}
			else
			{
				ASSERT(FALSE);
				iRtn=-2;
			}
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAlarmEventID->EndWrite();
	}
	else
	{
		_pLockAlarmEventID->BeginWrite();
		BEGIN_ERROR_HANDLE
		if(_iEventIdReg == 0)
		{
			_iEventIdReg += 1;
		}
		END_ERROR_HANDLE
		_pLockAlarmEventID->EndWrite();
		
		if(AddAlarmLog(pDB_,sName_,_iEventIdReg,_iEventIdReg,dtStart_,iType_,iChannType_,-1)>0)
		{
			_iEventID = _iEventIdReg;
			iRtn=_iEventIdReg;
		}
		else
			iRtn=-1;
	}
	return iRtn;
}

///返回大于0,就是事件ID;-2表示添加一条报警事件的日志失败;-1表示在一条报警事件中添加一条小事件日志失败
int CAlarmPlantInfo::AddCustomAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_)
{
	long iRtn=-1;//如果大于0，就是事件ID
	if (_iEventIdCustom==-1)
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			iRtn=_iEventID+1;
			//	int iID=_iID+1;
			//添加报警日志
			if (AddAlarmLog(pDB_,sName_,iRtn,iRtn,dtStart_,iType_,iChannType_,-1)>0)
			{
				_iEventID=iRtn;
				//		_iID=iID;
				_iEventIdCustom=_iEventID;
			}
			else
				iRtn=-2;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAlarmEventID->EndWrite();
	}
	else
	{
		if(AddAlarmLog(pDB_,sName_,_iEventIdCustom,_iEventIdCustom,dtStart_,iType_,iChannType_,-1)>0)
			iRtn=_iEventIdCustom;
		else
			iRtn=-1;
	}
	return iRtn;
}

int CAlarmPlantInfo::AddProcRegAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_)
{
	long iRtn=-1;//如果大于0，就是事件ID
	_pLockAlarmEventID->BeginWrite();
	try
	{
		iRtn=_iEventID+1;
		//添加报警日志
		if (AddAlarmLog(pDB_,sName_,iRtn,iRtn,dtStart_,iType_,iChannType_,-1)>0)
			_iEventID=iRtn;
		else
		{
			iRtn=-2;
			CString strInfo;strInfo.Format("%s-%s 添加过程量报警日志失败",GetPlantID().c_str(),sName_.c_str());
			CHZLogManage::Error(strInfo,"CAlarmPlantInfo","AddProcRegAlarmLog");
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockAlarmEventID->EndWrite();
	return iRtn;
}

int CAlarmPlantInfo::AddFastAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_)
{
	long iRtn=-1;//如果大于0，就是事件ID
	

	if(this->_iEventIDF == -1)
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			iRtn=_iEventID+1;
			//	int iID=_iID+1;
			//iType_=iType_|D_BHKD_AT_FAST;
			//添加报警日志
			if (AddAlarmLog(pDB_,sName_,iRtn,iRtn,dtStart_,iType_,iChannType_,-1)>0)
			{
				_iEventID=iRtn;
				_iEventIDF = _iEventID;
			}
			else
				iRtn=-2;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}
		_pLockAlarmEventID->EndWrite();
	}
	else
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			
			//	int iID=_iID+1;
			//iType_=iType_|D_BHKD_AT_FAST;
			//添加报警日志
			if (AddAlarmLog(pDB_,sName_,_iEventIDF,_iEventIDF,dtStart_,iType_,iChannType_,-1)>0)
			{
				iRtn=_iEventIDF;
			}
			else
				iRtn=-2;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}
		_pLockAlarmEventID->EndWrite();
	}

	
	return iRtn;
}

int CAlarmPlantInfo::SetRegAlarmLogEndTime(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_)
{
	return SetAlarmLogEndTimeAndNewThID(pDB_,sName_,_iEventIdReg,_iEventIdReg,dtEnd_,iChannType_,-1);
}

int CAlarmPlantInfo::SetCustomAlarmLogEndTime(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_)
{
	return SetAlarmLogEndTimeAndNewThID(pDB_,sName_,_iEventIdCustom,_iEventIdCustom,dtEnd_,iChannType_,-1);
}

///返回大于0,就是事件ID;-2表示添加一条报警事件的日志失败;-1表示在一条报警事件中添加一条小事件日志失败
int CAlarmPlantInfo::AddSlowAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_,int iThID_)
{
	long iRtn=-1;//如果大于0，就是事件ID
	if (_iEventIdS==-1)
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			iRtn=_iEventID+1;
			//	int iID=_iID+1;
			//添加报警日志
			//CHZLogManage::Info("添加报警事件日志","CAlarmPlantInfo","AddSlowAlarmLog");
			if (AddAlarmLog(pDB_,sName_,iRtn,iRtn,dtStart_,iType_,iChannType_,iThID_)>0)
			{
				_iEventID=iRtn;
				//		_iID=iID;
				_iEventIdS=_iEventID;
			}
			else
				iRtn=-2;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAlarmEventID->EndWrite();
	}
	else
	{
		_pLockAlarmEventID->BeginWrite();
		try
		{
			//添加报警日志
			//CHZLogManage::Info("在一条报警事件中，添加一条小事件日志","CAlarmPlantInfo","AddSlowAlarmLog");
			if(AddAlarmLog(pDB_,sName_,_iEventIdS,_iEventIdS,dtStart_,iType_,iChannType_,iThID_)>0)
				iRtn=_iEventIdS;
			else
				iRtn=-1;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockAlarmEventID->EndWrite();
	}
	return iRtn;
}

int CAlarmPlantInfo::SetSlowAlarmLogEndTimeAndThID(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_,int iThID_)
{
	return SetAlarmLogEndTimeAndNewThID(pDB_,sName_,_iEventIdS,_iEventIdS,dtEnd_,iChannType_,iThID_);
}

//[TBC]
int CAlarmPlantInfo::SaveVibSlowAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int iRtn=-1;
	V_PCHANNINFO::iterator it;
	S_VibCharValue vibValue;
	string sTime;
	int iSmpNum = 1, iSmpFreq = 1;

	//改为不用读写锁，避免保存常规振动数据和缓变振动数据冲突时，造成中间件死机状态
	GetRealSmpInfo( iSmpNum, iSmpFreq,false);

	//插入特征值数据[TBC]
	pDB_->Cmd("INSERT INTO [%s](",_sAlarmVibDataTblName.c_str());
	pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cTime,gc_cMicroSecond,gc_cSampleNum,gc_cSampleFreq,_sInsField.c_str());


	it=_vVibChann.begin();
	sTime=((CVibChannParamAlarm*)(*it))->GetSpecIndexTime(iCharValueIndex_);
	pDB_->Cmd(" VALUES(%d,0,'%s',0,%d,%d ",_iEventIdS,sTime.c_str(), this->GetSafeWavePointNum(iSmpNum), iSmpFreq);


	for(int nloop=0;nloop<5;++nloop)
	{
		pDB_->Cmd(" ,%.2f",_pfRealRev[nloop]);
	}

	for (;it!=_vVibChann.end();++it)
	{
		vibValue=((CVibChannParamAlarm*)(*it))->GetCharValue(iCharValueIndex_);
		pDB_->Cmd(",%.2f",vibValue._fCharAll);
		pDB_->Cmd(",%.2f",vibValue._fCharOne);
		pDB_->Cmd(",%.2f",vibValue._fCharTwo);
		pDB_->Cmd(",%.2f",vibValue._fCharThree);
		pDB_->Cmd(",%.2f",vibValue._fCharHalf);
		pDB_->Cmd(",%.2f",vibValue._fCharRes);
		pDB_->Cmd(",%.2f",vibValue._fDC);
		pDB_->Cmd(",%.2f",vibValue._fGap);
		pDB_->Cmd(",%.2f",vibValue._fPhaseOne);
		pDB_->Cmd(",%.2f",vibValue._fPhaseTwo);
		pDB_->Cmd(",%.2f",vibValue._fPhaseThree);
		pDB_->Cmd(",%.2f",vibValue._fPhaseHalf);
		pDB_->Cmd(",%d",vibValue._iSmpFreq);
		pDB_->Cmd(",%d",this->GetSafeWavePointNum( vibValue._iSmpNum));
		pDB_->Cmd(",%d",vibValue._iRev);
		pDB_->Cmd(",'%s'",((CVibChannParamAlarm*)(*it))->GetChannInfo()._cAlarmTempID);
		pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetSlowAlarmStatus());
	}

	pDB_->Cmd(")");

	if (!(pDB_->Exec()))
	{
		iRtn=-2;
		goto QUIT_SAVEDATA;
	}

	//更新波形数据
	pDB_->Cmd("SELECT %s FROM [%s] ",_sSelWaveField.c_str(),_sAlarmVibDataTblName.c_str());
	pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=0",gc_cEventID,_iEventIdS,gc_cTime,sTime.c_str(),gc_cMicroSecond);
	if (pDB_->More(TRUE))
	{
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			bool bRtn=false;
			int iSmpNums=this->GetSafeWavePointNum(((CVibChannParamAlarm*)(*it))->GetCharValue(iCharValueIndex_)._iSmpNum);

			vector<BYTE> lvWave;
			CWaveHead lsHead = ((CVibChannParamAlarm*)(*it))->WaveHead();
			int lnTotalSize = this->WaveAddHead(((CVibChannParamAlarm*)(*it))->GetWavePoint(iWaveIndex_),iSmpNums,lsHead,lvWave);

			bRtn=pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1).c_str(),(BYTE*)(lvWave.front()),lnTotalSize);

			pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7).c_str(),(BYTE*)((CVibChannParamAlarm*)(*it))->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if(bRtn)
			{
				iRtn=1;
			}
			else
			{
				iRtn=-4;
				goto QUIT_SAVEDATA;
			}

		}
	}
	else
	{
		iRtn=-3;

		ASSERT(FALSE);
	}

QUIT_SAVEDATA:

	if (iRtn>0)
	{
		_timeVibSlowSaveData=_timeCurr;
	}

	return iRtn;
}
//[TBC]
int CAlarmPlantInfo::SaveVibCustomAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int iRtn=-1;
	V_PCHANNINFO::iterator it;
	S_VibCharValue vibValue;
	string sTime;
	int iSmpNum = 1, iSmpFreq = 1;

	//改为不用读写锁，避免保存常规振动数据和缓变振动数据冲突时，造成中间件死机状态
	GetRealSmpInfo( iSmpNum, iSmpFreq,false);

	//插入特征值数据[TBC]
	pDB_->Cmd("INSERT INTO [%s](",_sAlarmVibDataTblName.c_str());
	pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cTime,gc_cMicroSecond,gc_cSampleNum,gc_cSampleFreq,_sInsField.c_str());
	it=_vVibChann.begin();

	//用当前最新更新的时间保存记录，避免同一设备分发送助手时，保存数据时间出错的问题
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt =GetRealVibDataDateTime();
	sTime=CPlantInfoBase::TimeConvertString(dt);

	pDB_->Cmd(" VALUES(%d,0,'%s',0,%d,%d ",_iEventIdCustom,sTime.c_str(), iSmpNum, iSmpFreq);


	for(int nloop=0;nloop<5;++nloop)
	{
		pDB_->Cmd(" ,%.2f",_pfRealRev[nloop]);
	}

	for (;it!=_vVibChann.end();++it)
	{
		vibValue=((CVibChannParamAlarm*)(*it))->GetCharValue();
		pDB_->Cmd(",%.2f",vibValue._fCharAll);
		pDB_->Cmd(",%.2f",vibValue._fCharOne);
		pDB_->Cmd(",%.2f",vibValue._fCharTwo);
		pDB_->Cmd(",%.2f",vibValue._fCharThree);
		pDB_->Cmd(",%.2f",vibValue._fCharHalf);
		pDB_->Cmd(",%.2f",vibValue._fCharRes);
		pDB_->Cmd(",%.2f",vibValue._fDC);
		pDB_->Cmd(",%.2f",vibValue._fGap);
		pDB_->Cmd(",%.2f",vibValue._fPhaseOne);
		pDB_->Cmd(",%.2f",vibValue._fPhaseTwo);
		pDB_->Cmd(",%.2f",vibValue._fPhaseThree);
		pDB_->Cmd(",%.2f",vibValue._fPhaseHalf);
		pDB_->Cmd(",%d",vibValue._iSmpFreq);
		pDB_->Cmd(",%d",this->GetSafeWavePointNum(vibValue._iSmpNum));
		pDB_->Cmd(",%d",vibValue._iRev);
		pDB_->Cmd(",'%s'",((CVibChannParamAlarm*)(*it))->GetChannInfo()._cAlarmTempID);
		pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetCustomAlarmStatus());
	}

	pDB_->Cmd(")");

	if (!(pDB_->Exec()))
	{
		iRtn=-2;
		goto QUIT_SAVEDATA;
	}

	//更新波形数据
	pDB_->Cmd("SELECT %s FROM [%s] ",_sSelWaveField.c_str(),_sAlarmVibDataTblName.c_str());
	pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=0",gc_cEventID,_iEventIdCustom,gc_cTime,sTime.c_str(),gc_cMicroSecond);

	if (pDB_->More(TRUE))
	{
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			bool bRtn=false;
			int iSmpNums=this->GetSafeWavePointNum(((CVibChannParamAlarm*)(*it))->GetCharValue()._iSmpNum);

			vector<BYTE> lvWave;
			CWaveHead lsHead = ((CVibChannParamAlarm*)(*it))->WaveHead();
			int lnTotalSize = this->WaveAddHead(((CVibChannParamAlarm*)(*it))->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

			bRtn=pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1).c_str(),(BYTE*)(lvWave.front()),lnTotalSize);
			pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7).c_str(),(BYTE*)((CVibChannParamAlarm*)(*it))->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if(bRtn)
			{
				iRtn=1;
			}
			else
			{
				iRtn=-4;
				goto QUIT_SAVEDATA;
			}

		}
	}
	else
	{
		iRtn=-3;

		ASSERT(FALSE);
	}

QUIT_SAVEDATA:

	return iRtn;
}

//[TBC]
int CAlarmPlantInfo::SaveAlarmDetailData(IDBInterFace *pDB_,
										 int iEventID_,
										 int iCharValueIndex_,
										 int iWaveIndex_,
										 float *pfRev_,
										 int iSmpNums_,
										 int iSmpFreq_,
										 bool& bSaved_,
										 CBHDateTime& tSaveTime_,
										 int iAlarmType_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	ASSERT(iEventID_ >0);

	int iRtn=-1;
	int loop=0;
	S_VibCharValue vibValue;
	V_PCHANNINFO::iterator it;
	int iMicroSecond=0;

	bSaved_ = false;

	//用当前最新更新的时间保存记录，避免同一设备分发送助手时，保存数据时间出错的问题
	CBHDateTime dt=CBHDateTime::GetCurrentTime();

	dt =GetRealVibDataDateTime();
	string sTime=CPlantInfoBase::TimeConvertString(dt);
	
	tSaveTime_ = dt;

	iMicroSecond = dt.GetMillisecond();

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d and %s = '%s'",\
		gc_cTime,_sAlarmVibDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,sTime.c_str());
	if(pDB_->More())
	{
		return 1;
	}

	if(g_bIsDiskFull)
	{

		int lnTotalWavePtNum = 0;
		_pLockVibData->BeginRead();
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			lnTotalWavePtNum += ((CVibChannParamAlarm*)(*it))->GetCharValue()._iSmpNum;
		}
		_pLockVibData->EndRead();

		if(!this->DeleteVibTrendRecord(pDB_,lnTotalWavePtNum))
		{
			this->DeleteVibAlarmRecord(pDB_,iEventID_,lnTotalWavePtNum);
		}
	}

	//插入特征值数据[TBC]
	pDB_->Cmd("INSERT INTO [%s](",_sAlarmVibDataTblName.c_str());
	pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cTime,gc_cMicroSecond,gc_cSampleNum,gc_cSampleFreq,_sInsField.c_str());
	pDB_->Cmd(" VALUES(%d,0,'%s',%d,%d,%d ",iEventID_,sTime.c_str(),iMicroSecond,this->GetSafeWavePointNum(iSmpNums_),iSmpFreq_);

	for (loop=0;loop<5;++loop)
	{//5个转速
		pDB_->Cmd(",%.2f",pfRev_[loop]);
	}

	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		((CVibChannParamAlarm*)(*it))->ComputeEigenValue(GetPlantInfo()._iMachineType);

		vibValue=((CVibChannParamAlarm*)(*it))->GetCharValue();
	
		//CString sTemp;sTemp.Format("_cChannNo:%s,vibValue:%f",((CVibChannParam*)(*it))->GetChannInfo()._cChannNo,vibValue._fCharAll);BHTRACE_DEBUG(sTemp);
		pDB_->Cmd(",%.2f",vibValue._fCharAll);
		pDB_->Cmd(",%.2f",vibValue._fCharOne);
		pDB_->Cmd(",%.2f",vibValue._fCharTwo);
		pDB_->Cmd(",%.2f",vibValue._fCharThree);
		pDB_->Cmd(",%.2f",vibValue._fCharHalf);
		pDB_->Cmd(",%.2f",vibValue._fCharRes);
		pDB_->Cmd(",%.2f",vibValue._fDC);
		pDB_->Cmd(",%.2f",vibValue._fGap);
		pDB_->Cmd(",%.2f",vibValue._fPhaseOne);
		pDB_->Cmd(",%.2f",vibValue._fPhaseTwo);
		pDB_->Cmd(",%.2f",vibValue._fPhaseThree);
		pDB_->Cmd(",%.2f",vibValue._fPhaseHalf);
		pDB_->Cmd(",%d",vibValue._iSmpFreq);
		pDB_->Cmd(",%d",this->GetSafeWavePointNum(vibValue._iSmpNum));
		pDB_->Cmd(",%d",vibValue._iRev);
		pDB_->Cmd(",'%s'",((CVibChannParamAlarm*)(*it))->GetChannInfo()._cAlarmTempID);


		switch(iAlarmType_)
		{
		case D_BHKD_AT_REGULAR:
			pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetRegAlarmStatus());
			break;
		case D_BHKD_AT_SLOW:
			pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetSlowAlarmStatus());
			break;
		case D_BHKD_AT_CUSTOM:
			pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetCustomAlarmStatus());
			break;
		default:
			pDB_->Cmd(",%d",((CVibChannParamAlarm*)(*it))->GetRegAlarmStatus());
		}
	}
	pDB_->Cmd(")");
	if (!(pDB_->Exec()))
	{
		iRtn=-2;
		goto QUIT_SAVEDETAILDATA;
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmVibDataTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sAlarmVibDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,sTime.c_str(),gc_cMicroSecond,iMicroSecond);
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}


	//更新波形数据
	if(lnID > 0)
	{
		pDB_->Cmd("SELECT [datetime],%s FROM [%s] ",_sSelWaveField.c_str(),_sAlarmVibDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d ",gc_cID,lnID);

	}
	else
	{
		pDB_->Cmd("SELECT [datetime],%s FROM [%s] ",_sSelWaveField.c_str(),_sAlarmVibDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,sTime.c_str(),gc_cMicroSecond,iMicroSecond);

	}
	
	if (pDB_->More(TRUE))
	{
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			bool bRtn=false;
			vector<BYTE> lvWave;
			vector<float> lvCustomParam;
			int iSmpNums = 0;

			iSmpNums=this->GetSafeWavePointNum(((CVibChannParamAlarm*)(*it))->GetCharValue()._iSmpNum);

			lvCustomParam.resize(GD_CUSTOMPARAM_BUFFSIZE);

			CWaveHead lsHead = ((CVibChannParamAlarm*)(*it))->WaveHead();

			int lnTotalSize = this->WaveAddHead(((CVibChannParamAlarm*)(*it))->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

			memcpy(&lvCustomParam.front(),((CVibChannParamAlarm*)(*it))->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			//memcpy(&lvWave.front(),((CVibChannParamAlarm*)(*it))->GetCurrWavePoint(),iSmpNums*sizeof(DATATYPE_WAVE));


			bRtn=pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1).c_str(),(BYTE*)&lvWave.front(),lnTotalSize);
			pDB_->SetBuffer(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7).c_str(),(BYTE*)&lvCustomParam.front(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if(bRtn)
			{
				bSaved_ = true;
				iRtn=1;
			}
			else
			{
				iRtn=-4;
				goto QUIT_SAVEDETAILDATA;
			}

		}
	}
	else
	{
		//CString strTmp;strTmp.Format("%d",iSmpNums_);AfxMessageBox(strTmp);
		iRtn=-3;

		ASSERT(FALSE);
	}
QUIT_SAVEDETAILDATA:
	return iRtn;
}
//[TBC]
int CAlarmPlantInfo::SaveDynAlarmDetailData(IDBInterFace *pDB_,
											int iEventID_,
											int iCharValueIndex_,
											int iWaveIndex_,
											float *pfRev_,
											int iSmpNums_,
											int iSmpFreq_,
											bool& bSaved_,
											CBHDateTime& tSaveTime_,
											int iAlarmType_
											)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	ASSERT(iEventID_ >0);
	int iRtn=-1;
	int loop=0;
	S_DynCharValue dynValue;
	V_PCHANNINFO::iterator it;

	bSaved_ = false;

	//用当前最新更新的时间保存记录，避免同一设备分发送助手时，保存数据时间出错的问题
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt =GetRealDynDataDateTime();
	string sTime=CPlantInfoBase::TimeConvertString(dt);
	tSaveTime_ = dt;

	int iMicroSecond=dt.GetMillisecond();

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d and %s = '%s'",\
		gc_cTime,_sAlarmDynDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,sTime.c_str());
	if(pDB_->More())
	{
		return 1;
	}

	if(g_bIsDiskFull)
	{

		int lnTotalWavePtNum = 0;
		_pLockDynData->BeginRead();
		for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
		{
			lnTotalWavePtNum += ((CDynChannParamAlarm*)(*it))->GetCharValue()._iSmpNum;
		}
		_pLockDynData->EndRead();

		if(!this->DeleteDynTrendRecord(pDB_,lnTotalWavePtNum))
		{
			this->DeleteDynAlarmRecord(pDB_,iEventID_,lnTotalWavePtNum);
		}
	}

	

	//插入特征值数据
	pDB_->Cmd("INSERT INTO [%s](",_sAlarmDynDataTblName.c_str());
	pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cTime,gc_cMicroSecond,gc_cSampleNum,gc_cSampleFreq,_sDynInsField.c_str());
	pDB_->Cmd(" VALUES(%d,0,'%s',%d,%d,%d ",iEventID_,sTime.c_str(),iMicroSecond,this->GetSafeWavePointNum(iSmpNums_),iSmpFreq_);

	for (loop=0;loop<5;++loop)
	{//5个转速
		pDB_->Cmd(",%.2f",pfRev_[loop]);
	}

	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		((CDynChannParamAlarm*)(*it))->ComputeEigenValue(GetPlantInfo()._iMachineType);

		dynValue=((CDynChannParamAlarm*)(*it))->GetCharValue();

		for (int loop1=0;loop1<12;++loop1)
		{//12个动态特征值
			pDB_->Cmd(",%.2f",dynValue.GetSpecTypeData(loop1));
		}

		pDB_->Cmd(",%d",dynValue._iSmpFreq);
		pDB_->Cmd(",%d",this->GetSafeWavePointNum(dynValue._iSmpNum));
		pDB_->Cmd(",%d",dynValue._iRev);
		pDB_->Cmd(",'%s'",((CDynChannParamAlarm*)(*it))->GetChannInfo()._cAlarmTempID);

		if(D_BHKD_AT_REGULAR==iAlarmType_)
		{
			pDB_->Cmd(",%d",((CDynChannParamAlarm*)(*it))->GetRegAlarmStatus());
		}
		else//缓变
		{
			pDB_->Cmd(",%d",((CDynChannParamAlarm*)(*it))->GetSlowAlarmStatus());
		}
	}

	pDB_->Cmd(")");

	if (!(pDB_->Exec()))
	{
		iRtn=-2;
		goto QUIT_SAVEDETAILDATA;
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmDynDataTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sAlarmDynDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,sTime.c_str(),gc_cMicroSecond,iMicroSecond);
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}


	//更新波形数据

	if(lnID > 0)
	{
		pDB_->Cmd("SELECT %s FROM [%s] ",_sDynSelWaveField.c_str(),_sAlarmDynDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d",gc_cID,lnID);

	}
	else
	{
		pDB_->Cmd("SELECT %s FROM [%s] ",_sDynSelWaveField.c_str(),_sAlarmDynDataTblName.c_str());
		pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,sTime.c_str(),gc_cMicroSecond,iMicroSecond);

	}
	
	if (pDB_->More(TRUE))
	{
		for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
		{
			bool bRtn=false;
			int iSmpNums= 0; 
			vector<BYTE> lvWave;
			vector<float> lvCustomParam;

			iSmpNums = this->GetSafeWavePointNum(((CDynChannParamAlarm*)(*it))->GetCharValue()._iSmpNum);
			lvCustomParam.resize(GD_CUSTOMPARAM_BUFFSIZE);

			CWaveHead lsHead = ((CDynChannParamAlarm*)(*it))->WaveHead();
			int lnTotalSize = this->WaveAddHead(((CDynChannParamAlarm*)(*it))->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);
			
			memcpy(&lvCustomParam.front(),((CDynChannParamAlarm*)(*it))->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			bRtn=pDB_->SetBuffer(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1).c_str(),(BYTE*)&lvWave.front(),lnTotalSize);

			pDB_->SetBuffer(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7).c_str(),(BYTE*)&lvCustomParam.front(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

			if(bRtn)
			{
				iRtn=1;
				bSaved_ = true;
			}
			else
			{
				iRtn=-4;
				goto QUIT_SAVEDETAILDATA;
			}

		}
	}
	else
	{
		//CString strTmp;strTmp.Format("%d",iSmpNums_);AfxMessageBox(strTmp);
		iRtn=-3;

		ASSERT(FALSE);
	}

QUIT_SAVEDETAILDATA:
	return iRtn;
}

//[TBC]
int CAlarmPlantInfo::SaveDynCustomAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int iRtn=-1;
	V_PCHANNINFO::iterator it;
	S_DynCharValue dynValue;
	string sTime;
	int iSmpNum = 1, iSmpFreq = 1;

	GetDynRealSmpInfo( iSmpNum, iSmpFreq,false);


	//插入特征值数据
	pDB_->Cmd("INSERT INTO [%s](",_sAlarmDynDataTblName.c_str());
	pDB_->Cmd("%s,%s,%s,%s,%s,%s,%s) ",gc_cEventID,gc_cSynFlag,gc_cTime,gc_cMicroSecond,gc_cSampleNum,gc_cSampleFreq,_sDynInsField.c_str());
	it=_vDynChann.begin();


	//用当前最新更新的时间保存记录，避免同一设备分发送助手时，保存数据时间出错的问题
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	dt =GetRealDynDataDateTime();
	sTime=CPlantInfoBase::TimeConvertString(dt);

	pDB_->Cmd(" VALUES(%d,0,'%s',0,%d,%d ",_iEventIdCustom,sTime.c_str(),this->GetSafeWavePointNum(iSmpNum), iSmpFreq);

	for(int nloop=0;nloop<5;++nloop)
	{
		pDB_->Cmd(" ,%.2f",_pfRealRev[nloop]);
	}

	for (;it!=_vDynChann.end();++it)
	{
		dynValue=((CDynChannParamAlarm*)(*it))->GetCharValue();

		for (int loop1=0;loop1<12;++loop1)
		{//12个动态特征值
			pDB_->Cmd(",%.2f",dynValue.GetSpecTypeData(loop1));
		}

		pDB_->Cmd(",%d",dynValue._iSmpFreq);
		pDB_->Cmd(",%d",this->GetSafeWavePointNum(dynValue._iSmpNum));
		pDB_->Cmd(",%d",dynValue._iRev);
		pDB_->Cmd(",'%s'",((CDynChannParamAlarm*)(*it))->GetChannInfo()._cAlarmTempID);
		pDB_->Cmd(",%d",((CDynChannParamAlarm*)(*it))->GetCustomAlarmStatus());
	}

	pDB_->Cmd(")");
	if (!(pDB_->Exec()))
	{
		iRtn=-2;
		goto QUIT_SAVEDATA;
	}

	//更新波形数据
	pDB_->Cmd("SELECT %s FROM [%s] ",_sDynSelWaveField.c_str(),_sAlarmDynDataTblName.c_str());
	pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=0",gc_cEventID,_iEventIdCustom,gc_cTime,sTime.c_str(),gc_cMicroSecond);

	if (pDB_->More(TRUE))
	{
		for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
		{
			bool bRtn=false;
			int iSmpNums=this->GetSafeWavePointNum(((CDynChannParamAlarm*)(*it))->GetCharValue()._iSmpNum);

			vector<BYTE> lvWave;
			CWaveHead lsHead = ((CDynChannParamAlarm*)(*it))->WaveHead();
			int lnTotalSize = this->WaveAddHead(((CDynChannParamAlarm*)(*it))->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

			bRtn=pDB_->SetBuffer(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1).c_str(),(BYTE*)(lvWave.front()),lnTotalSize);
			pDB_->SetBuffer(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7).c_str(),(BYTE*)((CDynChannParamAlarm*)(*it))->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			if(bRtn)
			{
				iRtn=1;
			}
			else
			{
				iRtn=-4;
				goto QUIT_SAVEDATA;
			}

		}
	}
	else
	{
		iRtn=-3;
		ASSERT(FALSE);
	}

QUIT_SAVEDATA:
	//strTmp.Format("保存动态自定义报警数据成功标示:%d",iRtn);BHTRACE_DEBUG(strTmp);
	//CHZLogManage::Debug(strTmp,"CAlarmPlantInfo","SaveDynCustomAlarmData");
	return iRtn;
}

int CAlarmPlantInfo::FirstVibSlowAlarmOP(IDBInterFace *pDB)
{
	int iRtn;
	int iLocation,iLocationWave;
	iLocation=((CVibChannParamAlarm*)_vVibChann[0])->GetCurrCharValueIndex();
	iLocationWave=((CVibChannParamAlarm*)_vVibChann[0])->GetCurrWaveIndex();
	pDB->BeginTran();
	for(iRtn=0;iRtn<G_D_ALARM_SAVEWAVE_NUM;++iRtn)
	{
		//CString strTmp;strTmp.Format("初次报警时,保存前80组特征值和波形的当前索引:%d,%d",iLocation,iLocationWave);
		//CHZLogManage::Debug(strTmp,"CAlarmPlantInfo","FirstVibSlowAlarmOP");
		int iStatus = 0;
		iStatus = SaveVibSlowAlarmData(pDB,iLocation,iLocationWave);
		if (iStatus <1)
		{
			iRtn=-2;
			goto QUIT_FIRSTVIBSLOWALARMOP;
		}
		iLocation--;
		iLocationWave--;
		if (iLocation<0)
			iLocation=D_ALARM_SAVECHAR_NUM-1;
		if (iLocationWave<0)
			iLocationWave=G_D_ALARM_SAVEWAVE_NUM-1;
	}
QUIT_FIRSTVIBSLOWALARMOP:
	if (iRtn>0)
	{
		pDB->CommitTran();
		_bVibSlowAlarmStatus=true;
		_timeVibSlowFirstSave=_timeCurr;
	}
	else
		pDB->RollbackTran();
	return iRtn;
}

int CAlarmPlantInfo::VibSlowAlarmStopOP(IDBInterFace *pDb_)
{
	//CHZLogManage::Info("开始缓变报警结束后的操作","CVibChannParamAlarm","VibSlowAlarmStopOP");
	int iRtn=-1;
	_bVibSlowAlarmStatus=false;//将测点缓变报警状态置位false
	V_PCHANNINFO::iterator it;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		S_ChannInfo chanInfo=(*it)->GetChannInfo();
		if (chanInfo._chAlarmParam._bCharSlow)
		{//当通道需要判断缓变报警时，报警结束后才触发自学习，不需要判断缓变报警通道的不触发自学习
			iRtn=((CVibChannParamAlarm*)(*it))->HandleSlowAlarmStop(pDb_);
		}
	}
	return iRtn;
}

int CAlarmPlantInfo::VibSlowAlarmOP(IDBInterFace *pDB_,bool bNewAlarm_)
{
	int iRtn=-1;
	COleDateTimeSpan timeSpan;
	int iSmpNum=0,iSmpFreq=0;
	float fRev=.0;
	int iLocation=((CVibChannParamAlarm*)_vVibChann[0])->GetCurrCharValueIndex();
	int iLocationWave=((CVibChannParamAlarm*)_vVibChann[0])->GetCurrWaveIndex();
	if (bNewAlarm_)
	{//新的报警产生
		//CString strTmp;strTmp.Format("新的报警产生时,保存特征值和波形的当前索引:%d,%d",iLocation,iLocationWave);
		//CHZLogManage::Debug(strTmp,"CAlarmPlantInfo","VibSlowAlarmOP");
		_timeVibSlowFirstSave=_timeCurr;
		//CString strTmp;strTmp.Format("新的报警产生后，首次保存数据的时间:%s",TimeConvertString(_timeVibSlowFirstSave));
		//CHZLogManage::Debug(strTmp,"CAlarmPlantInfo","VibSlowAlarmOP");
		iRtn=SaveVibSlowAlarmData(pDB_,iLocation,iLocationWave);
	}
	else
	{
		timeSpan=_timeCurr-_timeVibSlowFirstSave;
		int iTotalMin=timeSpan.GetTotalMinutes();
		if (iTotalMin>D_ALARM_SLOW_CONTSAVETIME)
		{//连续保存报警数据期间
			if (iTotalMin>D_ALARM_SLOW_INTVSAVETIME)//超过报警数据保存时间，结束报警
				VibSlowAlarmStopOP(pDB_);
			else
			{//处于连续报警保存期间的操作
				timeSpan=_timeCurr-_timeVibSlowSaveData;
				//CString strTmp;strTmp.Format("上次保存数据的时间:%s",TimeConvertString(_timeVibSlowSaveData));
				//CHZLogManage::Debug(strTmp,"CAlarmPlantInfo","VibSlowAlarmOP");
				if (timeSpan.GetTotalSeconds()>D_ALARM_INTVSAVETIME_INTV)
					iRtn=SaveVibSlowAlarmData(pDB_,iLocation,iLocationWave);
			}
		}
		else//没有超过连续保存时间
			iRtn=SaveVibSlowAlarmData(pDB_,iLocation,iLocationWave);
	}
	return iRtn;
}

int CAlarmPlantInfo::ReqThID(int iNums_)
{
	int iRtn=-1;
	_pLockThID->BeginWrite();
	try
	{
		iRtn=_iThID+1;
		//连续分配指定通道数目的阈值ID
		_iThID=iRtn+iNums_-1;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockThID->EndWrite();
	return iRtn;
}

int CAlarmPlantInfo::ReqAlarmEventID()
{
	int iEventID=0;
	_pLockAlarmEventID->BeginWrite();
	++_iEventID;
	iEventID=_iEventID;
	_pLockAlarmEventID->EndWrite();
	return iEventID;
}

int CAlarmPlantInfo::GetCurrAlarmEventID(string sChann_,CCustomDB *pDB_)
{
	return this->_iEventID;
	SwichDbName(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iEventID=0;
	
	if (sChann_ == "")
	{
		//查询当前报警事件ID
		pDB_->Cmd("SELECT TOP 1 [%s] from [%s] ",gc_cEventID,_sAlarmLogTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=1 ",gc_cChannType);//根据通道类型,判断是否为振动通道
		pDB_->Cmd(" ORDER BY [%s] DESC ",gc_cEventID);
	}
	else
	{
		//查询当前报警事件ID
		pDB_->Cmd("SELECT TOP 1 [%s] from [%s] ",gc_cEventID,_sAlarmLogTblName.c_str());
		pDB_->Cmd(" WHERE [%s]='%s' ",gc_cName,sChann_.c_str());
		pDB_->Cmd(" ORDER BY [%s] DESC ",gc_cEventID);
	}
	if (pDB_->More())
		iEventID = pDB_->GetLong(gc_cEventID);
	return iEventID;
}

int CAlarmPlantInfo::GetSpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return GetICESpecVibChannStartEndIndex(index_,iStartIndex_,iEndIndex_);
	}
	S_VibCharValue vib;
	if(index_ > -1)
	{
		_pLockVibData->BeginRead();
		try
		{
			CVibChannParamAlarm *pVibChann=(CVibChannParamAlarm*)(_vVibChann[index_]);
			int iCurrIndex=pVibChann->GetCurrCharValueIndex();
			vib = pVibChann->GetCharValue(iCurrIndex);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockVibData->EndRead();

		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		if (iRev>0)
		{
			if (iEndIndex_ - iStartIndex_ > (iSmpFreq*60/iRev)+10 || iEndIndex_ - iStartIndex_ < (iSmpFreq*60/iRev)-10)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
			}
		}
	}
	return  index_;
}

int CAlarmPlantInfo::GetSpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetSpecVibChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_);
}

int CAlarmPlantInfo::GetICESpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_)
{
	int index=0;
	S_VibCharValue vib;
	if(index_ > -1)
	{
		_pLockVibData->BeginRead();
		try
		{
			
			vib = ((CVibChannParamAlarm*)_vVibChann[index_])->GetRealCharValueEx();
			

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

		_pLockVibData->EndRead();

		CString lstrChann  = this->GetSpecVibChannInfo(index_)._cChannNo;

		int lnStartIndexSpec = GE_THREECHAR;
		int lnEndIndexSpec = GE_HALFCHAR;

		if (lstrChann.CompareNoCase(_T("1InstTime"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;

		}else if (lstrChann.CompareNoCase(_T("1InstVel"))==0)
		{
			lnStartIndexSpec = GE_PHASECHARTWO;
			lnEndIndexSpec = GE_PHASECHARTHREE;

		}else if (lstrChann.CompareNoCase(_T("1TorsVib"))==0)
		{
			lnStartIndexSpec = GE_ALLCHAR;
			lnEndIndexSpec = GE_ONECHAR;
		}

		iStartIndex_ = (int)vib.GetSpecTypeData(lnStartIndexSpec);
		iEndIndex_ = (int)vib.GetSpecTypeData(lnEndIndexSpec);
		//判断得到的周期起始索引和结束索引是否正确，若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=vib._iSmpFreq;
		iSmpNum=vib._iSmpNum;
		iRev=vib._iRev;
		//当设备停机转速为-1时，按额定转速计算起止索引
		iRev=iRev==-1?_sPlantInfo._piRev[0]:iRev;

		{
			iStartIndex_ = 0;
			iEndIndex_ = iSmpNum;
		}

		

	}
	return  index_;
}

int CAlarmPlantInfo::GetICESpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_)
{
	int iRtn=-1;
	iRtn=GetSpecVibChannIndex(sChann_);
	return GetICESpecVibChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_);
}

int CAlarmPlantInfo::GetSpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_)
{
	if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return this->GetICESpecDynChannStartEndIndex(index_,iStartIndex_,iEndIndex_);
	}
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		_pLockDynData->BeginRead();
		try
		{
			CDynChannParamAlarm *pDynChann=(CDynChannParamAlarm*)(_vDynChann[index_]);
			int iCurrIndex=pDynChann->GetCurrCharValueIndex();
			dyn = pDynChann->GetCharValue(iCurrIndex);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockDynData->EndRead();
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		if (iRev >0)
		{
			if (iEndIndex_ - iStartIndex_ > (iSmpFreq*60/iRev)+10 || iEndIndex_ - iStartIndex_ < (iSmpFreq*60/iRev)-10)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
			}
		}
	}
	return index_;
}

int CAlarmPlantInfo::GetSpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetSpecDynChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_);
}

int CAlarmPlantInfo::GetICESpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_)
{
	S_DynCharValue dyn;
	if(index_ > -1)
	{
		_pLockDynData->BeginRead();
		try
		{
			CDynChannParamAlarm *pDynChann=(CDynChannParamAlarm*)(_vDynChann[index_]);
			int iCurrIndex=pDynChann->GetCurrCharValueIndex();
			dyn = pDynChann->GetCharValue(iCurrIndex);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockDynData->EndRead();
		iStartIndex_ = (int)dyn.GetSpecTypeData(10);
		iEndIndex_ = (int)dyn.GetSpecTypeData(11);
		//判断得到的动态周期起始索引和结束索引是否正确,若不正确则返回计算出来的周期起始索引和结束索引
		int iSmpNum = 0,iSmpFreq = 0,iRev=0;
		iSmpFreq=dyn._iSmpFreq;
		iSmpNum=dyn._iSmpNum;
		iRev=dyn._iRev;
		//CString strInfo;strInfo.Format("off原始得到的周期起始和结束索引%d,%d,采样频率%d,采样点数%d,转速%.2f",iStartIndex_, iEndIndex_,iSmpFreq,iSmpNum,pfRev[0]);
		//CHZLogManage::Debug( strInfo, "CRealPlantInfo","GetSpecDynChannStartEndIndex");
		if (iRev >0)
		{
			if(iStartIndex_ > iEndIndex_)
			{
				iStartIndex_ = 0;
				iEndIndex_ =iSmpFreq*60/iRev;
			}
			iEndIndex_ =iEndIndex_>iSmpNum?(iSmpNum-1):iEndIndex_;

		}
	}
	return index_;
}

int CAlarmPlantInfo::GetICESpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_)
{
	int iRtn=-1;
	iRtn=GetSpecDynChannIndex(sChann_);
	return GetICESpecDynChannStartEndIndex(iRtn, iStartIndex_,iEndIndex_);
}

int CAlarmPlantInfo::GetSpecRadialVibChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	int index=-1;
	if (iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginRead();
			try
			{
				memcpy(pfCustom_,&(((CVibChannParamAlarm*)(*it))->GetCustomParamData()[iStartIndex_]),sizeof(float)*iLen_);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();
			return index;
		}
	}
	return -1;
}

int CAlarmPlantInfo::GetSpecRadialVibChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	if (iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	_pLockVibData->BeginRead();
	try
	{
		memcpy(pfCustom_,&(((CVibChannParamAlarm*)(*it))->GetCustomParamData()[iStartIndex_]),sizeof(float)*iLen_);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndRead();
	return 1;
}

int CAlarmPlantInfo::GetSpecDynChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	int index=-1;
	if ( iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		++index;
		if (strcmp((*it)->GetChannInfo()._cChannID,sChann_.c_str())==0)
		{
			_pLockDynData->BeginRead();
			try
			{
				memcpy(pfCustom_,&(((CDynChannParamAlarm*)(*it))->GetCustomParamData()[iStartIndex_]), sizeof(float)*iLen_);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockDynData->EndRead();
			return index;
		}
	}
	return -1;
}

int CAlarmPlantInfo::GetSpecDynChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ /* = 0 */, const int& iLen_ /* = GD_CUSTOMPARAM_BUFFSIZE */)
{
	if ( index_< 0 || (!(index_< _vDynChann.size())) ||  iStartIndex_ < 0 || (iStartIndex_ + iLen_) > GD_CUSTOMPARAM_BUFFSIZE)
		return -1;
	V_PCHANNINFO::iterator it;
	_pLockDynData->BeginRead();
	try
	{
		memcpy(pfCustom_,&(((CDynChannParamAlarm*)(*it))->GetCustomParamData()[iStartIndex_]), sizeof(float)*iLen_);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockDynData->EndRead();
	return 1;
}

CBHDateTime CAlarmPlantInfo::GetRealProcDataDateTime(string sChann)
{

	CBHDateTime dt = CBHDateTime::GetCurrentTime();

	int index = GetSpecProcChannIndex(sChann);
	if (index> _sPlantInfo._iProcChannNum)
		return dt;

	S_ChannInfo channInfo=_vProcChann[index]->GetChannInfo();
	if(GE_DATA_SOURCE_OFFLINE == channInfo._iDataSource){

		for(V_PCHANNINFO::iterator it = _vOfflineChann.begin();
			it != _vOfflineChann.end();
			++it)
		{
			COfflineChannParam * pParam = (COfflineChannParam *)(*it);
			S_ChannInfo sChannInfo = pParam->GetChannInfo();

			if(sChann == sChannInfo._cChannID){
				dt = pParam->GetTime();

				break;
			}
		}
	}
	else {
		dt=CPlantInfoOnLine::GetRealProcDataDateTime();
	}

	return dt;
}

CBHDateTime CAlarmPlantInfo::GetRealProcDataDateTime()
{
	return CPlantInfoOnLine::GetRealProcDataDateTime();
}

int CAlarmPlantInfo::JudgeRegAlarmAndComputeVibEigenValue(S_VibCharValue & arefVibValue,
													   int anIndex,
													   DATATYPE_WAVE  *  apNivfWave_,
													   int anLength)
{
	//找到对应特征值索引的alarmparam
	if (anIndex >= this->_vVibChann.size() || anIndex <0)
	{
		return FALSE;
	}

    BOOL lbRet = FALSE;

	CVibChannParamAlarm* lpVibParam = NULL;

	S_ChannInfo loInfo;

	MACHINETYPE lnMachineType;

	_pLockVibData->BeginRead();

	BEGIN_ERROR_HANDLE;

	lpVibParam =((CVibChannParamAlarm*)_vVibChann[anIndex]);

	lbRet = lpVibParam->JudgeRegDataAlarm(arefVibValue);

	loInfo = lpVibParam->GetChannInfo();

	lnMachineType = (MACHINETYPE)this->GetPlantInfo()._iMachineType;

	END_ERROR_HANDLE;

	_pLockVibData->EndRead();

	//有常规报警
	if (lbRet>0)
	{
		if (lpVibParam->IsNeedComputeEigenValue(arefVibValue))
		{
			EigenValueComputer::ComputeVibEigenValue(arefVibValue,
													 loInfo,
													 arefVibValue._iRev,
													 apNivfWave_,
													 anLength,
													 lnMachineType,
													 (CHANNTYPE)loInfo._iChannType
													 );

			return TRUE;
		}
		
	}

	return FALSE;

}


/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *            报警确认记录是否保存在数据表中 FIELD_SYSTEM_EVENT_TYPE 1:不保存 0：保存 默认0
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *			   #define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */

int CAlarmPlantInfo::AlarmConfirm( ICommInterFace & arefComm,IDBInterFace & arefDb )
{
	int lnRet = 0;
	//1.获取设置的数据
	CString lstrSet    =   arefComm.GetItem(FIELD_SYSTEM_SET);
	CString lstrChannID =  arefComm.GetItem(FIELD_SYSTEM_CHANN);
	GE_ALARM_CONFIRM_TYPE  lnConfirmType = (GE_ALARM_CONFIRM_TYPE)(arefComm.GetLong(FIELD_SERVICE_ALARM_CONFIRM_TYPE));

	CString lstrTimeStart = arefComm.GetItem(FIELD_SERVICE_OFF_TIMESTART);
	CString lstrTimeEnd = arefComm.GetItem(FIELD_SERVICE_OFF_TIMEEND);

	CBHDateTime loTimeStart = arefComm.GetTime(FIELD_SERVICE_OFF_TIMESTART);

	CBHDateTime loTimeEnd = arefComm.GetTime(FIELD_SERVICE_OFF_TIMEEND);

	CBHDateTime loTimeCurrent = CBHDateTime::GetCurrentTimeEx();

	CString lstrUser =  arefComm.GetItem(FIELD_SYSTEM_USEID);
	CString lstrDataCode =  arefComm.GetItem(FIELD_SERVICE_DATA_CODE);

	CString lstrRemark =  arefComm.GetItem(FIELD_SERVICE_REMARK);
	CString lstrIp	=  arefComm.GetItem(FIELD_SYSTEM_CLIENT_IP);

	int lnSaveRecord = arefComm.GetLong(FIELD_SYSTEM_EVENT_TYPE); //报警确认记录是否保存在数据表中 1:不保存 0：保存

	//2.设置当前测点取消报警

	//2.1根据侧点名查找根据给定侧点名对应的测点信息
	
	int lnChannIndex = 0;
	int lnChannType = 0; //0:vib 1:dyn 2:proc
	S_ChannInfo loInfo;

	//2.1.1 查找振动测点

	CChannInfoBase* lpChannParam = NULL;

	if (lnChannIndex>=0)
	{

		_pLockVibData->BeginRead();	

		BEGIN_ERROR_HANDLE;

		lnChannIndex = this->GetSpecVibChannIndex(lstrChannID.GetBuffer(0));

		if (lnChannIndex>=0)
		{
			lpChannParam =_vVibChann[lnChannIndex];
			loInfo = ((CVibChannParamAlarm *)lpChannParam)->GetChannInfo();
			lnRet = ((CVibChannParamAlarm *)lpChannParam)->ConfirmAlarm(lnConfirmType,loTimeStart,loTimeEnd);

		}
		lnChannType = 0;

		END_ERROR_HANDLE;

		_pLockVibData->EndRead();

	} 
	
	

	//2.1.2 查找过程量测点
	if (lnChannIndex<0)
	{
		_pLockProcData->BeginRead();

		BEGIN_ERROR_HANDLE;
		
		lnChannIndex = this->GetSpecProcChannIndex(lstrChannID.GetBuffer(0));
		if (lnChannIndex>=0)
		{
			lpChannParam =_vProcChann[lnChannIndex];
			loInfo = ((CProcChannParamAlarm *)lpChannParam)->GetChannInfo();
			lnRet = ((CProcChannParamAlarm *)lpChannParam)->ConfirmAlarm(lnConfirmType,loTimeStart,loTimeEnd);
		}
		lnChannType = 2;
		
		END_ERROR_HANDLE;
		
		_pLockProcData->EndRead();
	}

	//2.1.3 查找动态测点
	if (lnChannIndex<0)
	{
		_pLockDynData->BeginRead();

		BEGIN_ERROR_HANDLE;
		
		lnChannIndex = this->GetSpecDynChannIndex(lstrChannID.GetBuffer(0));
		if (lnChannIndex>=0)
		{
			lpChannParam =_vDynChann[lnChannIndex];
			loInfo = ((CDynChannParamAlarm *)lpChannParam)->GetChannInfo();
			lnRet = ((CDynChannParamAlarm *)lpChannParam)->ConfirmAlarm(lnConfirmType,loTimeStart,loTimeEnd);
		}
		
		lnChannType = 1;
		END_ERROR_HANDLE;

		_pLockDynData->EndRead();
	}

	//2.1.4 查找轴心轨迹测点,暂不支持
	
	//2.2 设置报警确认
	
	if (loInfo._iChannType>0 && lnChannIndex>=0)//找到了对应测点
	{
		

		//this->JudgeAlarm(&arefDb);

		if (lnConfirmType != ALARM_CONFIRM_TYPE_NONE)
		{
			S_AlarmState loAlarmState;

			BEGIN_ERROR_HANDLE;
			switch (lnChannType)
			{
				//vib
			case 0:
				((CVibChannParamAlarm *)lpChannParam)->SetAlarmStatus(loAlarmState);
				((CVibChannParamAlarm *)lpChannParam)->SetAlarmLevel(ALARM_NORMAL);
				break;
				//dyn
			case 1:
				((CDynChannParamAlarm *)lpChannParam)->SetAlarmStatus(loAlarmState);
				((CDynChannParamAlarm *)lpChannParam)->SetAlarmLevel(ALARM_NORMAL);
				break;
				//proc
			case 2:
				((CProcChannParamAlarm *)lpChannParam)->SetAlarmStatus(loAlarmState);
				((CProcChannParamAlarm *)lpChannParam)->SetAlarmLevel(ALARM_NORMAL);
				break;
			}

			END_ERROR_HANDLE;
		}

		
		
	}

	if(lnSaveRecord == 1)
	{
		return lnRet;
	}


	if (loInfo._iChannType>0)
	{

		CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
		SwitchDbNameEx(&arefDb,lstrSubDbName);
		//3.保存记录
		CString lstrInsertSqlFormat =_T("INSERT INTO  [%s]  ([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) \
													values  ( '%s','%s','%s',%d , '%s', '%s','%s','%s','%s',%d , %d)");

		CString lstrUpdateSqlFormat = _T("UPDATE [%s] set [%s] = %d where ([%s]='%s')");

		int lnTblType = (int)CChannInfoBase::GetChannOneType((CHANNTYPE )(loInfo._iChannType));
		CString lstrSql;

		//3.1 将原有的记录的IS_CURRENT设置为0
		lstrSql.Format(	lstrUpdateSqlFormat,
						DBUtils:: ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName),
						gc_cIsCurrent,
						0,
						gc_cChannNo,
						loInfo._cChannNo);

		arefDb.Cmd(lstrSql);

		if (arefDb.More())
		{
			//3.2 插入一条新的更新记录
			lstrSql.Format(lstrInsertSqlFormat,
							DBUtils:: ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName),	

							gc_cChannNo,
							gc_cStart_T,
							gc_cEnd_T,
							gc_cType,
							gc_cUserName,
							gc_cDateTime,
							gc_cDATACODE,
							gc_cRemark,
							gc_cClientIP,
							gc_cChannType,
							gc_cIsCurrent,

							loInfo._cChannNo,
							loTimeStart.Format(DATE_TIME_FORMAT),
							loTimeEnd.Format(DATE_TIME_FORMAT),
							lnConfirmType,
							lstrUser,
							loTimeCurrent.Format(DATE_TIME_FORMAT),
							lstrDataCode,
							lstrRemark,
							lstrIp,
							lnTblType,
							1
							);


			arefDb.Cmd(lstrSql);

			if (!arefDb.More())
			{
				BHLOG_ERROR(_T("保存报警确认数据失败"));
			}
		}

	}

	return lnRet;
}

void CAlarmPlantInfo::AddVibChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CString strLog="Start";
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	int index=0;
	S_ChannInfo sChannInfo;
	CVibChannParamAlarm *vibParam=NULL;
	//_pDB=pDB_;
	_vVibChann.clear();
	_mapVibChannIndex.clear();
	//径向振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s'  AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		vibParam=new CVibChannParamAlarm;
		vibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		vibParam->SetChannInfo(sChannInfo);
		vibParam->SetParentPoint(this);
		vibParam->InitValues();
		_vVibChann.push_back(vibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	strLog.Format("%s-%s 径向振动通道数目：%d",_sCompany.c_str(),_sPlantInfo._cPlantID,_vVibChann.size());
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	//轴向振动通道
	pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'  AND %s =%d",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);
	pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
	while (pDB_->More())
	{
		vibParam=new CVibChannParamAlarm;
		vibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		vibParam->SetChannInfo(sChannInfo);
		vibParam->SetParentPoint(this);
		_vVibChann.push_back(vibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR && 
		_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL)
	{	//添加轴心轨迹
		_vAxisChann.clear();
		CAxisAlarmChannParam *axisAlarm=NULL;
		CAxisChannInfo axisChannInfo;
		int iRevType=1;
		int iVerIndex=0,iHorIndex=0;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),
			gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			axisAlarm=new CAxisAlarmChannParam;
			axisAlarm->SetRwLock(_pLockChannInfo);
			axisAlarm->SetDataLock(_pLockVibData);
			axisChannInfo.GetAxisChannInfo(pDB_);
			iVerIndex=GetSpecVibChannIndexNo(axisChannInfo._cVerChann);

			if(iVerIndex < 0) {
				HZLOG_WARN(STR("未能将%s测点加入轴心测点对", axisChannInfo._cVerChann));
				continue;
			}
			iRevType=_vVibChann[iVerIndex]->GetChannInfo()._iRevType;
			iHorIndex=GetSpecVibChannIndexNo(axisChannInfo._cHorChann);
			if(iHorIndex < 0) {
				HZLOG_WARN(STR("未能将%s测点加入轴心测点对", axisChannInfo._cHorChann));
				continue;
			}

			axisAlarm->SetChannInfoEx(axisChannInfo,iVerIndex,iHorIndex,iRevType,_vVibChann[iVerIndex]->GetChannInfo()._fSensitivity,_vVibChann[iHorIndex]->GetChannInfo()._fSensitivity);
			axisAlarm->SetParentPoint(this);
			_vAxisChann.push_back(axisAlarm);
		}
		_iAxesPairNum=_vAxisChann.size();
		strLog.Format("%s-%s 轴心位置数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,_iAxesPairNum);
		CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddVibChannInfo");
	}


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwitchDbNameEx(pDB_,lstrSubDbName);

	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_VIB);

	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
		+ DBUtils::ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName) \
		+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
	+  " and " + CString(gc_cChannType) \
		+ " = " +lstrChannType ;


	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo).c_str();

		int lnIndex = this->GetSpecVibChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vVibChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);

				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}

	}

}

void CAlarmPlantInfo::AddDynChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CString strLog="Start";
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");
	int index=0;
	S_ChannInfo sChannInfo;
	CDynChannParamAlarm *dynParam=NULL;
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	//动态通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s'  AND t1.[%s] in(%d,%d,%d)",gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		dynParam=new CDynChannParamAlarm;
		dynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		dynParam->SetChannInfo(sChannInfo);
		dynParam->SetParentPoint(this);
		dynParam->InitValues();
		_vDynChann.push_back(dynParam);
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	strLog.Format("%s-%s 动态通道数目：%d",_sCompany.c_str(),_sPlantInfo._cPlantID,_vDynChann.size());
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");

	if(_sPlantInfo._iMachineType == GE_MACHINETYPE_RC || _sPlantInfo._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		_sPlantInfo._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || _sPlantInfo._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{	//添加轴心轨迹
		_vAxisChann.clear();
		CAxisAlarmChannParam *axisAlarm=NULL;
		CAxisChannInfo axisChannInfo;
		int iRevType=1;
		int iVerIndex=0,iHorIndex=0;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s'  AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),
			gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			axisAlarm=new CAxisAlarmChannParam;
			axisAlarm->SetRwLock(_pLockChannInfo);
			axisAlarm->SetDataLock(_pLockVibData);
			axisChannInfo.GetAxisChannInfo(pDB_);
			iVerIndex=GetSpecDynChannIndexNo(axisChannInfo._cVerChann);
			if(iVerIndex < 0) continue;
			iRevType=_vDynChann[iVerIndex]->GetChannInfo()._iRevType;
			iHorIndex=GetSpecDynChannIndexNo(axisChannInfo._cHorChann);
			if(iHorIndex < 0) continue;
			axisAlarm->SetChannInfoEx(axisChannInfo,iVerIndex,iHorIndex,iRevType,_vDynChann[iVerIndex]->GetChannInfo()._fSensitivity,_vDynChann[iHorIndex]->GetChannInfo()._fSensitivity);
			axisAlarm->SetParentPoint(this);
			_vAxisChann.push_back(axisAlarm);
		}
		_iAxesPairNum=_vAxisChann.size();
		strLog.Format("%s-%s 轴心位置数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,_iAxesPairNum);
		CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddDynChannInfo");
	}


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwitchDbNameEx(pDB_,lstrSubDbName);

	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_DYN);

	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
		+DBUtils:: ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName) \
		+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
	+  " and " + CString(gc_cChannType) \
		+ " = " +lstrChannType ;


	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo).c_str();

		int lnIndex = this->GetSpecDynChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vDynChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);

				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}

	}
}

void CAlarmPlantInfo::AddProcChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CString strLog;
	CProcChannParamAlarm *procParam;
	int index=0;
	S_ChannInfo sChannInfo;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND %s in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CProcChannParamAlarm;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		procParam->SetParentPoint(this);
		procParam->InitValues();
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}

	strLog.Format("%s-%s 过程量通道数目：%d Finish",_sCompany.c_str(),_sPlantInfo._cPlantID,index);
	CHZLogManage::Info(strLog,"CAlarmPlantInfo","AddProcChannInfo");


	//更新报警确认数据
	CString lstrSubDbName = (_sCompanyAlias+_sPlantInfo._cPlantNo).c_str();
	SwitchDbNameEx(pDB_,lstrSubDbName);

	CString lstrChannType;
	lstrChannType.Format(_T("%d"),E_TBL_CHANNTYPE_DYN);

	CString lstrSql = "SELECT CHANNNO,START_T,END_T,TYPE FROM " \
		+ DBUtils::ComposeDataTableName(gc_cAlarmConfirmTableName, lstrSubDbName)\
		+ " WHERE "		+ "(" + CString(gc_cIsCurrent) + "=1) ";
	+  " and " + CString(gc_cChannType) \
		+ " = " +lstrChannType ;


	pDB_->Cmd(lstrSql);

	while(pDB_->More())
	{
		CString lstrChnnNo = pDB_->Get(gc_cChannNo).c_str();

		int lnIndex = this->GetSpecProcChannIndexNo(lstrChnnNo.GetBuffer(0));

		if (lnIndex>=0)
		{
			CChannInfoBase * lpChannInfoBase = this->_vProcChann[lnIndex];

			if (lpChannInfoBase)
			{
				CBHDateTime loTimeStart = pDB_->GetTime(gc_cStart_T);
				CBHDateTime loTimeEnd =	pDB_->GetTime(gc_cEnd_T);		
				GE_ALARM_CONFIRM_TYPE lnType = (GE_ALARM_CONFIRM_TYPE)pDB_->GetLong(gc_cType);

				lpChannInfoBase->ConfirmAlarm(lnType,loTimeStart,loTimeEnd);
			}

		}

	}
}

void CAlarmPlantInfo::AddChannInfoEnd(IDBInterFace * pDB_)
{
	CPlantInfoOnLine::AddChannInfoEnd( pDB_);
	CString strLog="";

	CString strTmp;
	V_PCHANNINFO::iterator it;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	_sInsField="";
	_sSelWaveField="";
	_sVibTrendWaveField="";
	_sInsField.append(gc_cRev);
	_sInsField.append(",");
	_sInsField.append(gc_cRev2);
	_sInsField.append(",");
	_sInsField.append(gc_cRev3);
	_sInsField.append(",");
	_sInsField.append(gc_cRev4);
	_sInsField.append(",");
	_sInsField.append(gc_cRev5);

	_sVibTrendField = "";

	_vVibWaveField.clear();
	_vDynWaveField.clear();
    
	string sTmp;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		if (((CVibChannParamAlarm*)(*it))->GetChannInfo()._iChannType==GE_VIBCHANN)
			++_iRadialVibChannNum;
		else
			++_iAxialVibChannNum;
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetCharValueField());
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sInsField.append(",");
		_sInsField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-2));

		_sSelWaveField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1));
		_sSelWaveField.append(",");
		_sSelWaveField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7));
		_sSelWaveField.append(",");

		
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetCharValueField());
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sVibTrendField.append(",");
		_sVibTrendField.append(((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sVibTrendField.append(",");

		sTmp = ((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-1);
		sTmp.append(",");
		sTmp+=((CVibChannParamAlarm*)(*it))->GetSpecCharTypeField(-7);
		_vVibWaveField.push_back(sTmp.c_str());

	}
	strTmp.Format(", %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sVibTrendField += strTmp;

	_sVibTrendWaveField = _sSelWaveField.substr(0,_sSelWaveField.length()-1);

	_sSelWaveField.append(gc_cEventID);

	//构建动态数据表的插入字段和动态数据的波形显示字段
	_sDynInsField="";
	_sDynSelWaveField="";
	_sDynInsField.append(gc_cRev);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev2);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev3);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev4);
	_sDynInsField.append(",");
	_sDynInsField.append(gc_cRev5);

	_sDynSelWaveField="";
	_sDynTrendWaveField = "";
	sTmp = "";
	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetCharValueField());
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sDynInsField.append(",");
		_sDynInsField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-2));

		_sDynSelWaveField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1));
		_sDynSelWaveField.append(",");
		_sDynSelWaveField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7));
		_sDynSelWaveField.append(",");

		
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetCharValueField());
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-3));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-4));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-5));
		_sDynTrendField.append(",");
		_sDynTrendField.append(((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-6));
		_sDynTrendField.append(",");

		sTmp = ((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-1);
		sTmp.append(",");
		sTmp+=((CDynChannParamAlarm*)(*it))->GetSpecCharTypeField(-7);
		_vDynWaveField.push_back(sTmp.c_str());
	}
	strTmp.Format(", %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sDynTrendField += strTmp;

	_sDynTrendWaveField = _sDynSelWaveField.substr(0,_sDynSelWaveField.length()-1);

	_sDynSelWaveField.append(gc_cEventID);


	V_PCHANNINFO::iterator it2;
	_sProcField=" ";
	S_ChannInfo channInfo;
	
	for (it2=_vProcChann.begin();it2!=_vProcChann.end();++it2)
	{
		channInfo=(*it2)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);
	 strLog.Format("%s-%s-%s Finish",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	BHLOG_INFO(strLog);

	//初始化各种ID
	if(!InitEventID(pDB_))
	{
		CHZLogManage::Error("初始化事件ID失败!","CAlarmPlantInfo","AddChannInfoEnd");
	}
	//振动波形数据分配缓存

	long iNum = (_sPlantInfo._iVibChannNum)*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM;
	_pWave			= new DATATYPE_WAVE[iNum];


	iNum      = (_sPlantInfo._iDynChannNum)*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM;
	_pWaveDyn		= new DATATYPE_WAVE[iNum];


	//构造振动通道数据表单名字
	int index=0;

	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		((CVibChannParamAlarm*)(*it))->SetWaveMemoryPoint(&(_pWave[index*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM]),_sPlantInfo._iSmpNum);
		++index;
	}
	index = 0;
	for (it=_vDynChann.begin(); it!=_vDynChann.end(); ++it)
	{
		((CDynChannParamAlarm*)(*it))->SetWaveMemoryPoint(&(_pWaveDyn[index*(_sPlantInfo._iSmpNum)*G_D_ALARM_SAVEWAVE_NUM]),_sPlantInfo._iSmpNum);
		++index;
	}

	CBHDateTime loTimeNow = CBHDateTime::GetCurrentTimeEx();
	this->SetAllAlarmLogEnd(pDB_,loTimeNow);
	CHZLogManage::Info("Finish","CAlarmPlantInfo","AddChannInfoEnd");
	return;
}




void CAlarmPlantInfo::AddCustomAlarmParamInfo(IDBInterFace *pDB_)
{
	int index=0;
	vector<CCustomAlarmParamInfo> vCustomAlarmParamInfo;
	//添加振动自定义参数信息
	V_PCHANNINFO::iterator itVib;
	CVibChannParamAlarm *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		index=0;
		pVibChann=(CVibChannParamAlarm*)(*itVib);
		//获取自定义参数数据
		if (strcmp(pVibChann->GetChannInfo()._cAlarmTempID, "") != 0)
		{
			SwitchDbNameEx(pDB_,g_strMainDbName);
			pDB_->Cmd("SELECT * from [%s] ",gc_cAlarm_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cAlarm_TempletID,pVibChann->GetChannInfo()._cAlarmTempID);
			while (pDB_->More())
			{
				CCustomAlarmParamInfo paramInfo;
				paramInfo.GetCustomAlarmParamInfo(pDB_);
				vCustomAlarmParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Vib自定义报警参数数据条数 index :%d,%d",index,vCustomAlarmParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pVibChann->SetCustomAlarmParamInfo(0,index,&vCustomAlarmParamInfo[0]);
			vCustomAlarmParamInfo.clear();
		}
	}
	//添加动态自定义参数信息
	V_PCHANNINFO::iterator itDyn;
	CDynChannParamAlarm *pDynChann=NULL;
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		index=0;
		pDynChann=(CDynChannParamAlarm*)(*itDyn);
		//获取自定义参数数据
		if (strcmp(pDynChann->GetChannInfo()._cAlarmTempID, "") != 0)
		{
			pDB_->Cmd("SELECT * from [%s] ",gc_cAlarm_Param_Table);
			pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s'",gc_cCompany,_sCompany.c_str(),gc_cFactory_Name,_sFactory.c_str(),gc_cSetID,_sSetID.c_str(),gc_cAlarm_TempletID,pDynChann->GetChannInfo()._cAlarmTempID);
			while (pDB_->More())
			{
				CCustomAlarmParamInfo paramInfo;
				paramInfo.GetCustomAlarmParamInfo(pDB_);
				vCustomAlarmParamInfo.push_back(paramInfo);
				index++;
			}
			//CString strLog;strLog.Format("初始化获取Dyn自定义报警参数数据条数 index :%d,%d",index,vCustomAlarmParamInfo.size());
			//CHZLogManage::Info(strLog,"CPlantInfoOnLine","AddCustomParamInfo");
			pDynChann->SetCustomAlarmParamInfo(0,index,&vCustomAlarmParamInfo[0]);
			vCustomAlarmParamInfo.clear();
		}
	}
}

int CAlarmPlantInfo::JudgeRegAlarmEx( IDBInterFace *pDB_ ,bool bSaveData_/* = true*/,bool abConfirmAlarm_ /*= true*/)
{
	//0.如果数据没有更新不进行判断
	bool bUpdate=false;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=(_bVibDataUpdate || _bDynDataUpdate);
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bVibDataUpdate = false;
		_bDynDataUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
	{	
		return 0;
	}



	//1.判断全部测点的状态
	CAlarmPlantInfo::_PLANT_STATUS_ lnPlantStatus = this->PredictVibNewStatus(pDB_,abConfirmAlarm_);

	//2.切换状态
	this->SwitchStatus(pDB_,lnPlantStatus,this->InnerPlantAlarmStatus(),bSaveData_);

	return lnPlantStatus;
}

CAlarmPlantInfo::_PLANT_STATUS_ CAlarmPlantInfo::PredictVibNewStatus( IDBInterFace *pDB_ ,bool abConfirmAlarm_ /*= true*/)
{
	V_PCHANNINFO::iterator itVib = NULL;
	V_PCHANNINFO::iterator itDyn = NULL;

	bool bSaveOnlyOneAlarmDataRtn=false,bSaveAlarmDataFlag=false;
	CAlarmPlantInfo::_PLANT_STATUS_ lnVibChannAlarmFlag=CAlarmPlantInfo::PLANT_STATUS_NORMAL;
	int iFirstFlag=-1;
	
	bool lbPlantAlarmed = this->GetPlantAlarmState();
	bool lbJudgeAlarmLocal = isLocalJudgeAlarm();

	int lnUseAIAlarm = CConfigHelper::UseAIAlarm();

	int lnMachineType = this->GetPlantInfo()._iMachineType;

	int nLoop = -1;

	vector<int> lvFlameOutCylinder;
	
	//1.进入振动通道循环，判断报警
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		//快变报警判断只针对快变测点和非轴位移测点
		++nLoop;

		BOOL lbShouldJudge = TRUE;

		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE

		if((!abConfirmAlarm_ && 
			_vVibFastChangeChannNo.size() > nLoop &&
			_vVibFastChangeChannNo[nLoop] == 0)
			||(!abConfirmAlarm_ && (*itVib)->GetChannInfo()._iChannType == GE_AXIALCHANN)
			||(!abConfirmAlarm_ && _vVibFastChangeChannNo.size() <= 0))
		{
			
			lbShouldJudge = FALSE;
		}
		END_ERROR_HANDLE
		_pLockVibData->EndRead();

		if(!lbShouldJudge)
		{
			continue;
		}

		CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);
		if(lpVib->IsSignalError())
		{
			continue;
		}

		_pLockVibData->BeginWrite();

		try
		{
			CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

			if (NULL != lpVib)
			{
				int lnDataSource = lpVib->GetChannInfo()._iDataSource;

				S_VibCharValue & loValue = lpVib->GetCharValue();

				if (!lpVib->IsNeedComputeEigenValue(loValue) && lnDataSource != GE_DATA_SOURCE_WIRELESS)
				{
					if (lpVib->IsEigenValueEmpty(loValue))
					{
						lbShouldJudge = FALSE;
					}
				}

				//如果为无线测点值无效，则不进行报警判断。1：无效 0：有效
				if (lnDataSource == GE_DATA_SOURCE_WIRELESS )
				{
					int lnDataValid = (int)loValue.GetSpecTypeData(GE_HALFCHAR);

					if (lnDataValid == 1)
					{
						lbShouldJudge = FALSE;
					}
				}
			}

			if (lbShouldJudge)
			{
				CAlarmPlantInfo::_PLANT_STATUS_ lnStatus = PLANT_STATUS_NORMAL;


				lnStatus =(CAlarmPlantInfo::_PLANT_STATUS_)
					(((CVibChannParamAlarm*)(*itVib))->PredictChannAlarm(pDB_,
					lbPlantAlarmed,
					abConfirmAlarm_,
					lbJudgeAlarmLocal));

				
				CString lstrUnit = ((CVibChannParamAlarm*)(*itVib))->GetChannInfo()._cUnit;

				if(  lnStatus <= PLANT_STATUS_HALF_NORMAL && lnUseAIAlarm > 0)
				{

					_PLANT_STATUS_ lnDieselStatus = (CAlarmPlantInfo::_PLANT_STATUS_)
						(((CVibChannParamAlarm*)(*itVib))->PredictDieselChannAlarm(pDB_,
						lbPlantAlarmed,
						lvFlameOutCylinder,
						abConfirmAlarm_,
						lbJudgeAlarmLocal));

					if(lnDieselStatus > lnStatus)
					{
						lnStatus = lnDieselStatus;
					}

				}

				if (lnVibChannAlarmFlag<lnStatus)
				{
					lnVibChannAlarmFlag = lnStatus;
				}

				lpVib->SetChannAlarmLevel(lnStatus);
			}

		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		
		_pLockVibData->EndWrite();



		if (!lbShouldJudge)
		{
			continue;
		}

    }

	CAlarmPlantInfo::_PLANT_STATUS_ lnDynChannAlarmFlag=CAlarmPlantInfo::PLANT_STATUS_NORMAL;
		
	int iDynFirstFlag=-1;	

	nLoop = -1;

	//2.进入动态通道循环，判断报警
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		//快变报警判断只针对快变测点
		++nLoop;
		BOOL lbShouldJudge = TRUE;

		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE

		if((!abConfirmAlarm_  &&
			_vDynFastChangeChannNo.size() > nLoop &&
			_vDynFastChangeChannNo[nLoop] == 0)
			||(!abConfirmAlarm_ && (*itDyn)->GetChannInfo()._iChannType == GE_RODSINKCHANN)
			||(!abConfirmAlarm_ && _vDynFastChangeChannNo.size()<=0))
		{
			lbShouldJudge = FALSE;
		}
		END_ERROR_HANDLE
		_pLockVibData->EndRead();

		if(!lbShouldJudge)
		{
			continue;
		}

		_pLockVibData->BeginWrite();

		try
		{

			CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

			if (NULL != lpDyn)
			{
				S_DynCharValue & loValue = lpDyn->GetCharValue();

				if (!lpDyn->IsNeedComputeEigenValue(loValue))
				{
					if (lpDyn->IsEigenValueEmpty(loValue))
					{
						lbShouldJudge  = FALSE;
					}
				}
			}	

			if (lbShouldJudge)
			{
				CAlarmPlantInfo::_PLANT_STATUS_ lnStatus=  (CAlarmPlantInfo::_PLANT_STATUS_)lpDyn->PredictChannAlarm(pDB_,
					lbPlantAlarmed,
					abConfirmAlarm_,
					lbJudgeAlarmLocal);
				
				//动态、振动一起考虑
				if (lnVibChannAlarmFlag<lnStatus)
				{
					lnVibChannAlarmFlag = lnStatus;
				}

				lpDyn->SetChannAlarmLevel(lnStatus);
			}
			

		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockVibData->EndWrite();

	}

	if (lnVibChannAlarmFlag> PLANT_STATUS_HALF_NORMAL)
	{
		this->AlarmHasOccoured(TRUE);
	}

	//3.计算设备下一个状态
	CAlarmPlantInfo::_PLANT_STATUS_ lnComputed = this->ComputeNewStatusForPredict(pDB_,lnVibChannAlarmFlag);	

	return lnComputed;
}

BOOL CAlarmPlantInfo::SwitchStatus( IDBInterFace *pDB_,
								   _PLANT_STATUS_ anNewStatus,
								   _PLANT_STATUS_ arefNoldStatus,
								   bool abShouldSaveData )
{
	//1.执行状态切换前需要进行的动作
	BOOL lbResult = this->DoSwitchThings(pDB_,anNewStatus,arefNoldStatus,abShouldSaveData);		
	
	//2.进行状态切换
	this->InnerPlantAlarmStatus(anNewStatus);			

	return lbResult;
}

BOOL CAlarmPlantInfo::DoSwitchThings( IDBInterFace *pDB_,
									 _PLANT_STATUS_ anNewStatus,
									 _PLANT_STATUS_ arefNoldStatus, 
									 bool abShouldSaveData )
{
	//1.处理Eventid
	if (CAlarmPlantInfo::PLANT_STATUS_NEW_ALARM == anNewStatus)
	{
		//1.1重置所有测点的正常计时器timer5，将所有测点的报警日志状态设置为停止
		this->StopAllChannsAlarmLogForNewAlarm(pDB_);
		//1.2 增加报警日志id
		this->IncreaseEventID();

		if(abShouldSaveData)
		{
			this->NewAlarmAddLog(true);
		}
	}

	//2.处理测点状态，增加测点报警日志，将测点报警状态标识为停止等，返回值表示是否有报警日志增加
	BOOL lbAlarmlogAdded = this->DoSwitchChannsThings(pDB_,anNewStatus,abShouldSaveData);
	
	bool lbSaveData = true;

	if(!abShouldSaveData)
	{
		if(g_SaveFastAndRegularAlarmData)
		{
			lbSaveData = (this->ShouldSaveData(pDB_,anNewStatus)||lbAlarmlogAdded) && abShouldSaveData;
		}
		else
		{
			lbSaveData = abShouldSaveData;
		}
	}
	else
	{
		lbSaveData = ShouldSaveData(pDB_,anNewStatus)||lbAlarmlogAdded;
	}
	//3.处理数据保存
	if (lbSaveData)
	{
		float pfRealRev[5];
		int iSmpNums = 0, iSmpFreq = 0;
		GetRealSmpInfo( iSmpNums, iSmpFreq, true);
		GetRealRev( pfRealRev, 5);

		bool lbSaved;
		CBHDateTime ltTime;

		if (this->_vVibChann.size()>0)
		{
			
			this->SaveVibData(pDB_,this->_iEventIdReg,0,0,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime);

			//if (anNewStatus == PLANT_STATUS_NEW_ALARM || anNewStatus == PLANT_STATUS_OLD_ALARM_HIGH)
			//{
			//	if (lbSaveData)
			//	{
			//		COleSqlServerDB loDb;
			//		//this->SaveVibAlarmDetailDataToTrend(&loDb,lbSaved,ltTime);
			//	}

			//}
		}

		if (this->_vDynChann.size()>0)
		{
			this->SaveDynData(pDB_,this->_iEventIdReg,0,0,pfRealRev,iSmpNums,iSmpFreq,lbSaved,ltTime);
			//if (anNewStatus == PLANT_STATUS_NEW_ALARM || anNewStatus == PLANT_STATUS_OLD_ALARM_HIGH)
			//{
			//	if (lbSaveData)
			//	{
			//		COleSqlServerDB loDb;
			//		//this->SaveDynAlarmDetailDataToTrend(&loDb,lbSaved,ltTime);
			//	}

			//}
		}
		
		if (this->_vProcChann.size()>0)
		{
			if (anNewStatus == PLANT_STATUS_NEW_ALARM || anNewStatus == PLANT_STATUS_OLD_ALARM_HIGH)
			{
				if (lbSaveData)
				{
					//this->SaveProcAlarmDetailDataToTrend(pDB_);
				}

			}
		}		
		
	}

	if(abShouldSaveData)
	{
		this->NewAlarmAddLog(false);
	}

	return lbAlarmlogAdded;
}

CAlarmPlantInfo::_PLANT_STATUS_ CAlarmPlantInfo::InnerPlantAlarmStatus() const
{
	return m_nInnerPlantAlarmStatus;
}

void CAlarmPlantInfo::InnerPlantAlarmStatus( _PLANT_STATUS_ val )
{

	m_nInnerPlantAlarmStatus = val;
}

void CAlarmPlantInfo::IncreaseEventID()
{

	this->_pLockAlarmEventID->BeginWrite();
	BEGIN_ERROR_HANDLE
	if (this->_iEventIdReg<0)
	{
		this->_iEventIdReg =0;
	}	
	this->_iEventIdReg++;
	END_ERROR_HANDLE
	this->_pLockAlarmEventID->EndWrite();
}

void CAlarmPlantInfo::ResetNewAlarmTime()
{
	this->m_oLastNewAlarmTime = this->_timeCurr;	
}

void CAlarmPlantInfo::ResetSaveHalfIndenseTime()
{
	this->m_oLastSaveHalfIndenseTime =  this->_timeCurr;
}

void CAlarmPlantInfo::ResetLastSaveSparseTime()
{
	this->m_oLastSaveSparseTime = this->_timeCurr;
}

void CAlarmPlantInfo::ResetHalfNormalTime()
{
	this->m_oHalfNormalTime =  this->_timeCurr;
}

BOOL CAlarmPlantInfo::ShouldSaveData( IDBInterFace *pDB_, _PLANT_STATUS_ anNewStatus )
{
	//1.下一状态是完全正常状态
	if (CAlarmPlantInfo::PLANT_STATUS_NORMAL ==  anNewStatus )
	{
		return FALSE;
	}
	
	//2.下一状态是旧报警状态但本次报警事件中有一个测点的报警值大于上次测点的报警值
	if (CAlarmPlantInfo::PLANT_STATUS_OLD_ALARM_HIGH == anNewStatus)
	{
		return TRUE;
	}
	
	//3.如果下一个状态是新报警状态，则需重置新报警计时器,timer1,timer2,timer3,timer4
	if (CAlarmPlantInfo::PLANT_STATUS_NEW_ALARM == anNewStatus)
	{
		this->ResetNewAlarmTime();
		//this->ResetHalfNormalTime();
		this->ResetSaveHalfIndenseTime();
		this->ResetLastSaveSparseTime();
	}


	
	//3.其余状态按照timer1,timer2,timer3进行判断
	COleDateTime loCurrentTime = this->_timeCurr;	
	COleDateTimeSpan loSpan = loCurrentTime - this->m_oLastNewAlarmTime;
	int lnMinitues = loSpan.GetTotalMinutes();
	//4.5分钟之内密集保存
	if (lnMinitues<=this->m_nSaveIndenseTime)
	{
		return TRUE;
	}
	
	//5.20分钟之内半密集保存，每30秒存一组重置timer2
	if (loSpan.GetTotalMinutes()<this->m_nHalfNormalNotSaveTime)
	{
		loSpan = loCurrentTime - this->m_oLastSaveHalfIndenseTime;
		if (loSpan.GetTotalSeconds()>= this->m_nSaveHalfIndenseTime)
		{
			this->ResetSaveHalfIndenseTime();
			return TRUE;
		}
	}


	//6.大于20分钟,且有报警发生则30分钟存一组,恢复半正常并不重置计时器
	if (CAlarmPlantInfo::PLANT_STATUS_HALF_NORMAL< anNewStatus)
	{
		loSpan = loCurrentTime - this->m_oLastSaveSparseTime;
		if (loSpan.GetTotalMinutes()>= this->m_nSaveSparseTime)
		{
			//重置timer3
			this->ResetLastSaveSparseTime();
			return TRUE;
		}
	}

	return FALSE;

}

BOOL CAlarmPlantInfo::SaveVibData( IDBInterFace *pDB_,
								  int iEventID_,
								  int iCharValueIndex_,
								  int iWaveIndex_,
								  float *pfRev_,
								  int iSmpNums_,
								  int iSmpFreq_,
								  bool& bSaved_,
								  CBHDateTime& tSaveTime_)
{

	int lbRet = 0;
	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	lbRet = this->SaveAlarmDetailData(pDB_,this->_iEventIdReg,0,0,pfRev_,iSmpNums_,iSmpFreq_,bSaved_,tSaveTime_);
	END_ERROR_HANDLE
	_pLockVibData->EndRead();
	return lbRet;
}

BOOL CAlarmPlantInfo::SaveProcData( IDBInterFace *pDB_,
								   int iEventID_,
								   int iCharValueIndex_,
								   int iWaveIndex_,
								   float *pfRev_,
								   int iSmpNums_,
								   int iSmpFreq_)
{
	//int lbRet = this->SaveAlarmDetailData(pDB_,this->_iEventIdReg,0,0,pfRev_,iSmpNums_,iSmpFreq_);

	return FALSE;

}

BOOL CAlarmPlantInfo::SaveDynData( IDBInterFace *pDB_,
								  int iEventID_,
								  int iCharValueIndex_,
								  int iWaveIndex_,
								  float *pfRev_,
								  int iSmpNums_,
								  int iSmpFreq_,
								  bool& bSaved_,
								  CBHDateTime& tSaveTime_)
{

	int lbRet = 0;
	_pLockDynData->BeginRead();
	BEGIN_ERROR_HANDLE
	lbRet = this->SaveDynAlarmDetailData(pDB_,this->_iEventIdReg,0,0,pfRev_,iSmpNums_,iSmpFreq_,bSaved_,tSaveTime_);
	END_ERROR_HANDLE
	_pLockDynData->EndRead();
	return lbRet;

}

int CAlarmPlantInfo::AddAlarmLogEx( IDBInterFace *pDB_,string sName_,long & iID_,long & iEventID_,COleDateTime dtStart_,int iType_,int iChannType_,int iThID_ )
{
	return TRUE;
}

void CAlarmPlantInfo::SetChannFastAlarmStatus(E_TBL_CHANNTYPE eChannType, int iAlarmType_,bool bSetValue_)
{
	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	switch(eChannType)
	{
	case E_TBL_CHANNTYPE_VIB:
		if(bSetValue_)
		{
			_vVibChannAlarmStatus.push_back(iAlarmType_);
		}
		break;
	case E_TBL_CHANNTYPE_DYN:
		if(bSetValue_)
		{
			_vDynChannAlarmStatus.push_back(iAlarmType_);
		}
		break;
	case E_TBL_CHANNTYPE_PROC:
		if(bSetValue_)
		{
			_vProcChannAlarmStatus.push_back(iAlarmType_);
		}
		break;
	default:
		break;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::SetVibFastChangeChann(vector<int>& vChannNo_)
{
	
	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE

	_vVibFastChangeChannNo.clear();
	if(vChannNo_.size() > 0)
	{
		_vVibFastChangeChannNo.resize(vChannNo_.size());
		copy(vChannNo_.begin(),vChannNo_.end(),_vVibFastChangeChannNo.begin());
	}

	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::JudgeVibFastAlarm(IDBInterFace *pDB_,int &iAlarmStatus_,int& iEventID_,vector<int>& vChannAlarmStatus_)
{
	
	int iRtn = 0;

	this->_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vVibChannAlarmStatus.clear();
	END_ERROR_HANDLE
	this->_pLockVibData->EndWrite();

	if(!isLocalJudgeAlarm())
	{
		UpdateAlarmParam();
	}

	if (g_UseNewAlarmLogic)
	{
		iRtn = this->JudgeFastRegAlarmEx(pDB_,false,false);
		if(isLocalJudgeAlarm())
		{
			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_HIGH)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}
		else
		{

			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_LOW)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}
		
	}else
	{
		iRtn =this->JudgeVibRegAlarm(pDB_,false,false);

	}
	
	vChannAlarmStatus_.clear();
	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(_vVibChannAlarmStatus.size() > 0)
	{
		vChannAlarmStatus_.resize(_vVibChannAlarmStatus.size());
		copy(_vVibChannAlarmStatus.begin(),_vVibChannAlarmStatus.end(),vChannAlarmStatus_.begin());
	}

	
	iEventID_ = ((bool)g_UseNewAlarmLogic==true?this->_iEventIdReg:this->_iEventID);
	END_ERROR_HANDLE
	_pLockVibData->EndRead();

	iAlarmStatus_ = iRtn>0?1:0;

	this->_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vVibFastChangeChannNo.clear();
	END_ERROR_HANDLE
	this->_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::SetDynFastChangeChann(vector<int>& vChannNo_)
{
	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE

	_vDynFastChangeChannNo.clear();
	if(vChannNo_.size() > 0)
	{
		_vDynFastChangeChannNo.resize(vChannNo_.size());
		copy(vChannNo_.begin(),vChannNo_.end(),_vDynFastChangeChannNo.begin());
	}

	END_ERROR_HANDLE
	_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::JudgeDynFastAlarm(IDBInterFace *pDB_,int &iAlarmStatus_,int& iEventID_,vector<int>& vChannAlarmStatus_)
{
	this->_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vDynChannAlarmStatus.clear();
	END_ERROR_HANDLE
	this->_pLockDynData->EndWrite();

	int iRtn = 0;

	if(!isLocalJudgeAlarm())
	{
		UpdateAlarmParam();
	}

	if (g_UseNewAlarmLogic)
	{
		iRtn = this->JudgeFastRegAlarmEx(pDB_,false,false);
		if(isLocalJudgeAlarm())
		{
			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_HIGH)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}
		else
		{
			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_LOW)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}
	}else
	{
		iRtn =this->JudgeVibRegAlarm(pDB_,false,false);

	}

	vChannAlarmStatus_.clear();
	_pLockDynData->BeginRead();
	BEGIN_ERROR_HANDLE

	if(_vDynChannAlarmStatus.size() > 0)
	{
		vChannAlarmStatus_.resize(_vDynChannAlarmStatus.size());
		copy(_vDynChannAlarmStatus.begin(),_vDynChannAlarmStatus.end(),vChannAlarmStatus_.begin());
	}

	iEventID_ = ((bool)g_UseNewAlarmLogic==true?this->_iEventIdReg:this->_iEventID);
	END_ERROR_HANDLE
	_pLockDynData->EndRead();

	iAlarmStatus_ = iRtn>0?1:0;

	this->_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vDynFastChangeChannNo.clear();
	END_ERROR_HANDLE
	this->_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::SetProcFastChangeChann(vector<int>& vChannNo_)
{
	_pLockProcData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vProcFastChangeChannNo.clear();
	if(vChannNo_.size() > 0)
	{
		_vProcFastChangeChannNo.resize(vChannNo_.size());
		copy(vChannNo_.begin(),vChannNo_.end(),_vProcFastChangeChannNo.begin());
	}
	END_ERROR_HANDLE
	_pLockProcData->EndWrite();
}

void CAlarmPlantInfo::JudgeProcFastAlarm(IDBInterFace *pDB_,
										 int &iAlarmStatus_,
										 int& iEventID_,
										 vector<int>& vChannAlarmStatus_)
{

	int iRtn = 0;

	this->_pLockProcData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vProcChannAlarmStatus.clear();
	END_ERROR_HANDLE
	this->_pLockProcData->EndWrite();

	if(!isLocalJudgeAlarm())
	{
		UpdateAlarmParam();
	}

	if (g_UseNewAlarmLogic)
	{
		iRtn = this->JudgeFastProcAlarmEx(pDB_,false,false);

		if(isLocalJudgeAlarm())
		{
			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_HIGH)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}
		else
		{
			if (iRtn>=CHANN_ALARM_STATUS_OLD_ALARM_LOW)
			{
				iRtn = 1;
			}
			else
			{
				iRtn = 0;
			}
		}

	}else
	{
		iRtn = this->JudgeProcAlarm(pDB_,false,false);
	}
	
	vChannAlarmStatus_.clear();
	_pLockProcData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(_vProcChannAlarmStatus.size() > 0)
	{
		vChannAlarmStatus_.resize(_vProcChannAlarmStatus.size());
		copy(_vProcChannAlarmStatus.begin(),_vProcChannAlarmStatus.end(),vChannAlarmStatus_.begin());
	}

	iEventID_ = ((bool)g_UseNewAlarmLogic==true?this->_iEventIdReg:this->_iEventID);
	END_ERROR_HANDLE
	_pLockProcData->EndRead();

	iAlarmStatus_ = iRtn>0?1:0;
	_pLockProcData->BeginWrite();
	BEGIN_ERROR_HANDLE
	_vProcFastChangeChannNo.clear();
	END_ERROR_HANDLE
	_pLockProcData->EndWrite();
}

CAlarmPlantInfo::_PLANT_STATUS_ CAlarmPlantInfo::ComputeNewStatusForPredict( IDBInterFace * pDB_,_PLANT_STATUS_ anNewStatus )
{
	COleDateTimeSpan loTimespan = this->_timeCurr - this->m_oHalfNormalTime;
	CAlarmPlantInfo::_PLANT_STATUS_  lnStatus = anNewStatus;
	int lnHours = 0;

	switch(anNewStatus)
	{
	case PLANT_STATUS_NORMAL:
		
		//如果全部测点状态正常，则计算半正常累计时间，如果超过默认2个小时，则进入正常状态
		if (this->InnerPlantAlarmStatus()!=PLANT_STATUS_NORMAL)
		{
			if (this->InnerPlantAlarmStatus() == PLANT_STATUS_HALF_NORMAL)
			{
				lnHours = loTimespan.GetTotalHours();
				if (lnHours>= this->m_nHalfNormalTime)
				{
					this->ResetHalfNormalTime();
					lnStatus = PLANT_STATUS_NORMAL;
					BHLOG_ERROR(STR("Plant reset to Normal, hours elapsed:%d anNewStatus:%x Current AlarmStatus:%x",
						lnHours,
						anNewStatus,
						this->InnerPlantAlarmStatus()));
				}else
				{
					lnStatus = PLANT_STATUS_HALF_NORMAL;
				}
			}
			else
			{
				lnStatus = PLANT_STATUS_HALF_NORMAL;
				this->ResetHalfNormalTime();
			}
		}

		break;
	case PLANT_STATUS_HALF_NORMAL:
		
		//重置timer4
		if (this->InnerPlantAlarmStatus()!=PLANT_STATUS_HALF_NORMAL)
		{
			this->ResetHalfNormalTime();
		}
		
		lnHours = loTimespan.GetTotalHours();
		if (lnHours>= this->m_nHalfNormalTime)
		{
			this->ResetHalfNormalTime();
			lnStatus = PLANT_STATUS_NORMAL;
			BHLOG_ERROR(STR("Plant reset to Normal, hours elapsed:%d anNewStatus:%x Current AlarmStatus:%x",
							lnHours,
							anNewStatus,
							this->InnerPlantAlarmStatus()));
		}
		break;
	
	case PLANT_STATUS_OLD_ALARM_LOW:		
	case PLANT_STATUS_OLD_ALARM_HIGH:
		this->ResetHalfNormalTime();
		//从正常状态，只要有任一测点报警，则设备为新报警状态
		if (this->InnerPlantAlarmStatus()==PLANT_STATUS_NORMAL)
		{
			BHLOG_ERROR(STR("New Alarm occoured anNewStatus:%x Current AlarmStatus:%x",anNewStatus,this->InnerPlantAlarmStatus()));
			lnStatus = PLANT_STATUS_NEW_ALARM;
		}
		break;
	case PLANT_STATUS_NEW_ALARM:
		this->ResetHalfNormalTime();
		BHLOG_ERROR(STR("New Alarm PLANT_STATUS_NEW_ALARM occoured anNewStatus:%x Current AlarmStatus:%x",anNewStatus,this->InnerPlantAlarmStatus()));

		break;

	}

	return lnStatus;
}

BOOL CAlarmPlantInfo::DoSwitchChannsThings(IDBInterFace *pDB_,_PLANT_STATUS_ anNewStatus,bool abShouldSaveData /* = true */)
{
	V_PCHANNINFO::iterator itVib = NULL;
	V_PCHANNINFO::iterator itDyn = NULL;

	int nLoop = -1;

	BOOL lbNewAlarmLogAdded = FALSE;	

	//1.进入振动通道循环，做报警状态切换处理
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		//快变报警判断只切换快变测点和非轴位移测点状态
		++nLoop;

		BOOL lbShouldContinue = FALSE;

		_pLockVibData->BeginWrite();
		BEGIN_ERROR_HANDLE

		if((!abShouldSaveData &&
			_vVibFastChangeChannNo.size() > nLoop && 
			_vVibFastChangeChannNo[nLoop] == 0)
			||(!abShouldSaveData && (*itVib)->GetChannInfo()._iChannType == GE_AXIALCHANN)
			||(!abShouldSaveData && _vVibFastChangeChannNo.size() <= 0))
		{
			CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

			if (NULL == lpVib)
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!abShouldSaveData);
			}
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!abShouldSaveData);
			}
			
			lbShouldContinue = TRUE;
		}
		END_ERROR_HANDLE
		_pLockVibData->EndWrite();

		if(lbShouldContinue)
		{
			continue;
		}

		
		_pLockVibData->BeginWrite();

		try
		{
			CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

			if (NULL == lpVib)
			{
				ASSERT(FALSE);
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,0,!abShouldSaveData);
				
			}
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_VIB,lpVib->GetChannRegAlarmType(),!abShouldSaveData);


				_CHANN_ALARM_STATUS_  lnStatus = lpVib->PredictedChannInnerAlarmStatus();

				if (lnStatus> anNewStatus)
				{
					lnStatus = (_CHANN_ALARM_STATUS_)anNewStatus;

				}else if ((lnStatus<= CHANN_ALARM_STATUS_HALF_NORMAL) 
					&& ((anNewStatus == PLANT_STATUS_NEW_ALARM)|| lpVib->PlantNewAlarmOccoured()))
				{
					lnStatus = CHANN_ALARM_STATUS_NORMAL;
				}

				BOOL lbChannAlarmLogAdded = lpVib->SwitchAlarmStatus(pDB_,lnStatus);
				if (lbChannAlarmLogAdded)
				{
					lbNewAlarmLogAdded = TRUE;
					if(!abShouldSaveData)
					{
						this->IncVibFastChangeAlarmAddLogTime(this->_timeVibDataUpdate);
					}
					
				}

				lpVib->PlantNewAlarmOccoured(FALSE);
			}
			
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}



		_pLockVibData->EndWrite();

	}

	nLoop = -1;

	//2.进入动态通道循环，做报警状态切换处理
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		++nLoop;
		//快变报警判断只切换快变测点和非活塞杆沉降测点状态
		BOOL lbShouldContinue = FALSE;

		_pLockVibData->BeginWrite();
		BEGIN_ERROR_HANDLE

		if((!abShouldSaveData &&
			_vDynFastChangeChannNo.size() > nLoop &&
			_vDynFastChangeChannNo[nLoop] == 0)
			||(!abShouldSaveData && (*itDyn)->GetChannInfo()._iChannType == GE_RODSINKCHANN )
			||(!abShouldSaveData && _vDynFastChangeChannNo.size() <= 0))
		{

			CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

			if (NULL == lpDyn)
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!abShouldSaveData);
			}	
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!abShouldSaveData);
			}
			
			lbShouldContinue = TRUE;
		}
		END_ERROR_HANDLE
		_pLockVibData->EndWrite();

		if(lbShouldContinue)
		{
			continue;
		}

		_pLockVibData->BeginWrite();

		try
		{

			CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

			if (NULL == lpDyn)
			{
				ASSERT(FALSE);
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,0,!abShouldSaveData);
				
			}
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_DYN,lpDyn->GetChannRegAlarmType(),!abShouldSaveData);

				_CHANN_ALARM_STATUS_  lnStatus = lpDyn->PredictedChannInnerAlarmStatus();
				if (lnStatus> anNewStatus)
				{
					lnStatus = (_CHANN_ALARM_STATUS_)anNewStatus;
				}else if ((lnStatus<= CHANN_ALARM_STATUS_HALF_NORMAL) 
					&& (anNewStatus == PLANT_STATUS_NEW_ALARM|| lpDyn->PlantNewAlarmOccoured()))
				{
					lnStatus = CHANN_ALARM_STATUS_NORMAL;
				}


				BOOL lbChannAlarmLogAdded =lpDyn->SwitchAlarmStatus(pDB_,lnStatus);

				if (lbChannAlarmLogAdded)
				{
					lbNewAlarmLogAdded = TRUE;
					if(!abShouldSaveData)
					{
						this->IncDynFastChangeAlarmAddLogTime(this->_timeDynDataUpdate);
					}
				}

				lpDyn->PlantNewAlarmOccoured(FALSE);
			}
			

			

		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockVibData->EndWrite();

	}

	return lbNewAlarmLogAdded;
}

BOOL CAlarmPlantInfo::StopAllChannsAlarmLogForNewAlarm( IDBInterFace *pDB_ )
{
	V_PCHANNINFO::iterator itVib = NULL;
	V_PCHANNINFO::iterator itDyn = NULL;
	V_PCHANNINFO::iterator itProc = NULL;


	//1.进入振动通道循环，停止报警状态
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{

		_pLockVibData->BeginWrite();

		try
		{
			CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

			if (NULL == lpVib)
			{
				ASSERT(FALSE);
				continue;
			}	


			lpVib->HandleRegAlarmStopEx(pDB_);
			lpVib->ResetHalfNormalTimer5();
			lpVib->PlantNewAlarmOccoured(TRUE);

		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}



		_pLockVibData->EndWrite();




	}

	//2.进入动态通道循环，停止报警状态
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{

		_pLockVibData->BeginWrite();

		try
		{

			CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

			if (NULL == lpDyn)
			{
				ASSERT(FALSE);
				continue;
			}

			lpDyn->HandleRegAlarmStopEx(pDB_);
			lpDyn->ResetHalfNormalTimer5();
			lpDyn->PlantNewAlarmOccoured(TRUE);
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockVibData->EndWrite();

	}

	//3.进入动态通道循环，停止报警状态
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{

		_pLockProcData->BeginWrite();

		try
		{

			CProcChannParamAlarm * lpProc = (CProcChannParamAlarm*)(*itProc);

			if (NULL == lpProc)
			{
				ASSERT(FALSE);
				continue;
			}

			lpProc->HandleRegAlarmStopEx(pDB_);
			lpProc->ResetHalfNormalTimer5();
			lpProc->PlantNewAlarmOccoured(TRUE);
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockProcData->EndWrite();

	}

	return TRUE;
}

BOOL CAlarmPlantInfo::AlarmHasOccoured() const
{
	return m_bAlarmHasOccoured;
}

void CAlarmPlantInfo::AlarmHasOccoured( BOOL val )
{
	m_bAlarmHasOccoured = val;
}

int CAlarmPlantInfo::SaveVibAlarmDetailDataToTrend(IDBInterFace *pDB_,bool bSaved_,CBHDateTime tSaveTime_)
{
	int iRtn = 0;

	if(g_bIsDiskFull)
	{
		return iRtn;
	}

	{
		int iCurrIDVibTrendMonth=0;
		
		if(bSaved_)
		{
			GetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
			iRtn = SaveVibRecord(pDB_,_sAlarmVibDataTblName,_sVibTrendMonthTblName,tSaveTime_,iCurrIDVibTrendMonth,true);
		}

	}

	

	return 0;
}


void CAlarmPlantInfo::SaveVibRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;

	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}
	if ((GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	S_VibCharValue loVibValue;
	CString strTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;
	//	CString strTmp;strTmp.Format("%s",sTableName_.c_str());AfxMessageBox(strTmp);
	GetRealRev( pfRev, 5);
	GetRealSmpInfo(iSmpNum, iSmpFreq);
	//CString sTemp;sTemp.Format("振动趋势: %d,%d",iSmpNum,iSmpNum);BHTRACE_DEBUG(sTemp);
	//CHZLogManage::Debug(sTemp,"CAlarmPlantInfo","JudgeVibRegAlarm");

	

	//得到断网数据当前的时间
	CBHDateTime dtCurr = this->_timeCurr;
	strTimeCurr = CPlantInfoBase::TimeConvertString(dtCurr);

	pDB_->Cmd("SELECT top 1 [%s]  FROM [%s] WHERE [%s]='%s'",
		gc_cDateTime,
		sTableName_.c_str(),
		gc_cDateTime,strTimeCurr);
	if(pDB_->More())
	{
		return;
	}
	
	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	int iSmpNumMax =iSaveWaveNum;
	
	if(iSaveWaveNum>iSmpNumMax)
	{	
		iSaveWaveNum=iSmpNumMax;
	}

	iAllBytes=this->GetMiddleWareWavePointNumMax()*sizeof(DATATYPE_WAVE);

	V_PCHANNINFO::iterator it;
	CVibChannParam *pVibChannParam=NULL;
	if (!bMonth_)
	{
		pDB_->Cmd("SELECT top 1 %s  FROM [%s] ",_sVibTrendField.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
	
		if(pDB_->More(TRUE))
		{
			{
				BYTE  *pBuf=new BYTE[iAllBytes];
				float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];

				int lnIndex = -1;
		
				for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
				{
					lnIndex++;
					pVibChannParam=(CVibChannParam*)(*it);

					_pLockVibData->BeginWrite();
					pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnIndex);
					_pLockVibData->EndWrite();

					_pLockVibData->BeginRead();
					int iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
					_pLockVibData->EndRead();

					if (iRealSmpNum == 0)
					{
						continue;
					}

					iSaveWaveNum = iRealSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}

		
					int lnTotalSize = 0;

					_pLockVibData->BeginRead();

					CWaveHead lsHead = pVibChannParam->WaveHead();

					lnTotalSize = iSaveWaveNum*sizeof(DATATYPE_WAVE);
					lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

					memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
					_pLockVibData->EndRead();
			
					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

				}
				KD_DeletePoint(pBuf);
				KD_DeletePoint(pfCustomData);
			}

			index=0;
			int lnCharIndex = -1;
			pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
			
			for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
			{
				pVibChannParam=(CVibChannParam*)(*it);

				lnCharIndex++;
				_pLockVibData->BeginWrite();
				pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
				_pLockVibData->EndWrite();

				
				_pLockVibData->BeginRead();
				loVibValue=pVibChannParam->GetCharValueEx();
				_pLockVibData->EndRead();
				
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),loVibValue.GetSpecTypeData(GE_ALLCHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),loVibValue.GetSpecTypeData(GE_ONECHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),loVibValue.GetSpecTypeData(GE_TWOCHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),loVibValue.GetSpecTypeData(GE_THREECHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),loVibValue.GetSpecTypeData(GE_HALFCHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),loVibValue.GetSpecTypeData(GE_RESCHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),loVibValue.GetSpecTypeData(GE_PHASECHARONE));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),loVibValue.GetSpecTypeData(GE_PHASECHARTWO));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),loVibValue.GetSpecTypeData(GE_PHASECHARTHREE));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),loVibValue.GetSpecTypeData(GE_PHASECHARHALF));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),loVibValue.GetSpecTypeData(GE_DCCHAR));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),loVibValue.GetSpecTypeData(GE_GAP));
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),loVibValue._iSmpFreq);
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),this->GetSafeWavePointNum(loVibValue._iSmpNum));
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),loVibValue._iRev);


			}
			pDB_->Cmd(" [%s]='%s', ",gc_cTime,strTimeCurr);
			pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,this->GetSafeWavePointNum(iSmpNum));
			pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
			pDB_->Cmd(" [%s]=%.2f ",gc_cRev5,pfRev[4]);
			pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);

			if (!pDB_->More())
			{
				
				CString lstrData;

				lstrData.Format(_T("报警数据 Save Failed Update 操作失败,id 为 :%d\r"),iCurID_);

				BHLOG_ERROR(lstrData);

			}else
			{
				
			}		
			
			return;
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sVibTrendField.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);

	index=0;
	int lnCharIndex = -1;
	for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
	{	
		pVibChannParam=(CVibChannParam*)(*it);
		lnCharIndex++;

		_pLockVibData->BeginWrite();
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
		_pLockVibData->EndWrite();


		_pLockVibData->BeginRead();
		loVibValue=pVibChannParam->GetCharValueEx();		
		_pLockVibData->EndRead();	

		
		
		//该通道没有断网数据
		if (loVibValue._iSmpNum ==0)
		{
			S_ChannInfo loInfo;

			loInfo = this->GetSpecVibChannInfo(index);		
			
			CString lstrData;

			lstrData.Format(_T("报警数据 , 参数不正确，采样点数为 0 ，采样点 ID:%s  No:%s\r") , loInfo._cChannID, loInfo._cChannNo );

			BHLOG_ERROR(lstrData);

		}

		index++;

		iSaveWaveNum = loVibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}
		
		if (iSaveWaveNum!= 0)
		{
			iSmpNum=iSaveWaveNum;
		}
		
		//CString strTmp;strTmp.Format("_iSmpFreq:%d,_iSmpNum:%d,_iRev%d",vibValue._iSmpFreq,vibValue._iSmpNum,vibValue._iRev);BHTRACE_DEBUG(strTmp);
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_ALLCHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_ONECHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_TWOCHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_THREECHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_HALFCHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_RESCHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_DCCHAR));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_GAP));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_PHASECHARONE));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_PHASECHARTWO));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_PHASECHARTHREE));
		pDB_->Cmd("%.2f,",loVibValue.GetSpecTypeData(GE_PHASECHARHALF));
		pDB_->Cmd("%d,",loVibValue._iSmpFreq);
		pDB_->Cmd("%d,",this->GetSafeWavePointNum(iSaveWaveNum));
		pDB_->Cmd("%d,",loVibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}
	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);
	for (int nloop=0;nloop<5;++nloop)
		pDB_->Cmd("%.2f,",pfRev[nloop]);
	pDB_->Cmd("'%s')",strTimeCurr);


	
	if(!pDB_->Exec())
	{

		CString lstrData;

		lstrData.Format(_T("报警数据保存到日常数据表  Save Failed 插入失败:%d \r\n"),iCurID_);

		BHLOG_ERROR(lstrData);

		return;
	}


	pDB_->Cmd("SELECT %s FROM [%s]",_sVibTrendField.c_str(),sTableName_.c_str());
	pDB_->Cmd(" WHERE %s=%d",gc_cID,iCurID_);
	
	if(pDB_->More(TRUE))
	{
		index=0;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		int lnCharIndex = -1;
		for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
		{
			pVibChannParam=(CVibChannParam*)(*it);
			lnCharIndex++;

			_pLockVibData->BeginWrite();
			pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
			_pLockVibData->EndWrite();

			_pLockVibData->BeginRead();
			int iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
			_pLockVibData->EndRead();

			iSaveWaveNum = iRealSmpNum;

			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}
		
			int lnTotalSize = 0;
			_pLockVibData->BeginRead();
			try
			{
				CWaveHead lsHead = pVibChannParam->WaveHead();
				
				lnTotalSize = iSaveWaveNum*sizeof(DATATYPE_WAVE);
				lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

				memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLockVibData->EndRead();

			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

		}

		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
	}
	else
	{
		BHLOG_ERROR("select record failed by dse");
	}
	return;
}

int CAlarmPlantInfo::SaveVibRecord(IDBInterFace * pDB_,string sTableNameSrc_,string sTableNameDes_,CBHDateTime tTime_,int iCurID_,bool bMonth_ /* =false */ )
{
	int iRtn = 0;
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = '%s'",
		gc_cID,sTableNameDes_.c_str(),gc_cTime,tTime_.Format(gc_cTimeFormat));
	if(pDB_->More())
	{
		return iRtn;
	}

	CVibChannParam *pVibChannParam;
	if(sTableNameDes_ != _sVibTrendMonthTblName)
	{
		pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s = %d",gc_cID,sTableNameDes_,gc_cID,iCurID_);
		if(pDB_->More())
		{
			pDB_->Cmd("UPDATE a SET a.%s = %d",gc_cID,iCurID_);

			for(int nloop = 0; nloop < _vVibChann.size(); ++nloop)
			{
				pVibChannParam = (CVibChannParam *)_vVibChann[nloop];
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-1).c_str(),pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-3).c_str(),pVibChannParam->GetSpecCharTypeField(-3).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-4).c_str(),pVibChannParam->GetSpecCharTypeField(-4).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-5).c_str(),pVibChannParam->GetSpecCharTypeField(-5).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetSpecCharTypeField(-6).c_str());
				pDB_->Cmd(",a.%s = b.%s",pVibChannParam->GetSpecCharTypeField(-7).c_str(),pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}

			pDB_->Cmd(",a.%s = b.%s",gc_cSampleNum,gc_cSampleNum);
			pDB_->Cmd(",a.%s = b.%s",gc_cSampleFreq,gc_cSampleFreq);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev,gc_cRev);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev2,gc_cRev2);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev3,gc_cRev3);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev4,gc_cRev4);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev5,gc_cRev5);
			pDB_->Cmd(",a.%s = b.%s)",gc_cTime,gc_cTime);

			pDB_->Cmd(" FROM [%s] a,[%s] b WHERE a.%s = %d and b.%s = '%s'",\
				sTableNameDes_.c_str(),sTableNameSrc_.c_str(),\
				gc_cID,iCurID_,gc_cTime,tTime_.Format(gc_cTimeFormat));
			if(pDB_->Exec())
			{
				iRtn = 1;
			}
			return iRtn;
		}
	}
	pDB_->Cmd("INSERT INTO [%s](%s,%s,%s) SELECT %d,%s,%s FROM [%s] WHERE [%s] = '%s'",
		sTableNameDes_.c_str(),gc_cID,_sVibTrendField.c_str(),_sVibTrendWaveField.c_str(),
		iCurID_,_sVibTrendField.c_str(),_sVibTrendWaveField.c_str(),sTableNameSrc_.c_str(),
		gc_cTime,tTime_.Format(gc_cTimeFormat));
	if(pDB_->Exec())
	{
		iRtn = 1;
	}
	return iRtn;
}

void CAlarmPlantInfo::SaveDynRecord(IDBInterFace * pDB_,const string &sTableName_,const int &iCurID_,const bool &bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR || 
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	}
	if ((GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	S_DynCharValue vibValue;
	CString strTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev(pfRev,5);
	GetDynRealSmpInfo(iSmpNum, iSmpFreq);


	//得到断网数据当前的时间
	CBHDateTime dtCurr = this->_timeCurr;
	strTimeCurr =CPlantInfoBase::TimeConvertString(dtCurr);

	pDB_->Cmd("SELECT top 1 [%s]  FROM [%s] WHERE [%s]='%s'",
		gc_cDateTime,
		sTableName_.c_str(),
		gc_cDateTime, strTimeCurr);

	if(pDB_->More())
	{
		return;
	}


	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	int iSmpNumMax =iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{	
		iSaveWaveNum=iSmpNumMax;
	}


	iAllBytes=this->GetMiddleWareWavePointNumMax()*sizeof(DATATYPE_WAVE);

	V_PCHANNINFO::iterator it;
	CDynChannParam *pVibChannParam=NULL;
	if (!bMonth_)
	{
		pDB_->Cmd("SELECT top 1 %s FROM [%s] ",_sDynTrendField.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
		if(pDB_->More(TRUE))
		{
			{
				BYTE  *pBuf=new BYTE[iAllBytes];
				float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
				int lnCharIndex =-1;
				for (it=_vDynChann.begin(); it!= _vDynChann.end(); ++it)
				{
					lnCharIndex++;
					pVibChannParam=(CDynChannParam*)(*it);

					_pLockDynData->BeginWrite();
					pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
					_pLockDynData->EndWrite();

					_pLockDynData->BeginRead();
					int iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
					_pLockDynData->EndRead();

					iSaveWaveNum = iRealSmpNum;

					if(iSaveWaveNum>iSmpNumMax)
					{
						iSaveWaveNum=iSmpNumMax;
					}

					int lnTotalSize = 0;
					_pLockDynData->BeginRead();
					CWaveHead lsHead = pVibChannParam->WaveHead();
					lnTotalSize = iSaveWaveNum*sizeof(DATATYPE_WAVE);
					lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

					//memcpy(pBuf,pVibChannParam->GetCurrWavePoint(),iSaveWaveNum*sizeof(DATATYPE_WAVE));
					memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
					_pLockDynData->EndRead();

					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

					nloop++;
				}
				KD_DeletePoint(pBuf);
				KD_DeletePoint(pfCustomData);
			}
			index=0;
			pDB_->Cmd("UPDATE [%s] SET ",sTableName_.c_str());
			int lnCharIndex =-1;
			for (it= _vDynChann.begin(); it!= _vDynChann.end(); ++it)
			{
				lnCharIndex++;
				pVibChannParam = (CDynChannParam*)(*it);

				_pLockDynData->BeginWrite();
				pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
				_pLockDynData->EndWrite();

				_pLockDynData->BeginRead();
				vibValue=pVibChannParam->GetCharValueEx();
				_pLockDynData->EndRead();



				iSaveWaveNum = vibValue._iSmpNum;

				if(iSaveWaveNum>iSmpNumMax)
				{
					iSaveWaveNum=iSmpNumMax;
				}

				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(0).c_str(),vibValue.GetSpecTypeData(0));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(1).c_str(),vibValue.GetSpecTypeData(1));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(2).c_str(),vibValue.GetSpecTypeData(2));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(3).c_str(),vibValue.GetSpecTypeData(3));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(4).c_str(),vibValue.GetSpecTypeData(4));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(5).c_str(),vibValue.GetSpecTypeData(5));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(6).c_str(),vibValue.GetSpecTypeData(6));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(7).c_str(),vibValue.GetSpecTypeData(7));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(8).c_str(),vibValue.GetSpecTypeData(8));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(9).c_str(),vibValue.GetSpecTypeData(9));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(10).c_str(),vibValue.GetSpecTypeData(10));
				pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(11).c_str(),vibValue.GetSpecTypeData(11));
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),iSaveWaveNum);
				pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);

			}
			pDB_->Cmd(" [%s]='%s', ",gc_cTime,strTimeCurr);
			pDB_->Cmd(" [%s]=%d, ",gc_cSampleNum,iSmpNum);
			pDB_->Cmd(" [%s]=%d, ",gc_cSampleFreq,iSmpFreq);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
			pDB_->Cmd(" [%s]=%.2f ",gc_cRev5,pfRev[4]);
			pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
			pDB_->More();
			return;
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sDynTrendField.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	index=0;
	int lnCharIndex =-1;
	for ( it= _vDynChann.begin(); it!= _vDynChann.end(); it++)
	{
		lnCharIndex++;
		pVibChannParam = (CDynChannParam*)(*it);

		_pLockDynData->BeginWrite();
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
		_pLockDynData->EndWrite();

		_pLockDynData->BeginRead();
		vibValue=pVibChannParam->GetCharValueEx();
		_pLockDynData->EndRead();


		iSaveWaveNum = vibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(0));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(1));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(2));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(3));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(4));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(5));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(6));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(7));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(8));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(9));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(10));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(11));
		pDB_->Cmd("%d,",vibValue._iSmpFreq);
		pDB_->Cmd("%d,",iSaveWaveNum);
		pDB_->Cmd("%d,",vibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}

	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);

	for (int nloop=0;nloop<5;++nloop)
	{
		pDB_->Cmd("%.2f,",pfRev[nloop]);
	}

	pDB_->Cmd("'%s')",strTimeCurr);

	if(!pDB_->Exec())
	{
		BHLOG_ERROR("insert record failed by dse!");
		return;
	}

	pDB_->Cmd("SELECT %s FROM [%s]", _sDynTrendField.c_str(), sTableName_.c_str());
	pDB_->Cmd(" WHERE %s=%d", gc_cID, iCurID_);

	if(pDB_->More(TRUE))
	{
		index=0;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		int lnCharindex =-1;
		for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
		{
			lnCharindex++;
			pVibChannParam=(CDynChannParam*)(*it);

			_pLockDynData->BeginWrite();
			pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharindex);
			_pLockDynData->EndWrite();

			_pLockDynData->BeginRead();
			int iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
			_pLockDynData->EndRead();

			iSaveWaveNum =iRealSmpNum;

			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}

			int lnTotalSize = 0;
			_pLockDynData->BeginRead();

			CWaveHead lsHead = pVibChannParam->WaveHead();
			lnTotalSize = iSaveWaveNum*sizeof(DATATYPE_WAVE);
			lnTotalSize = WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSaveWaveNum,lsHead,pBuf);

			//memcpy(pBuf,pVibChannParam->GetCurrWavePoint(),iSaveWaveNum*sizeof(DATATYPE_WAVE));
			memcpy(pfCustomData,(&pVibChannParam->GetCustomParamData()[0]), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
			_pLockDynData->EndRead();

			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

		}
		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
	}
	else
	{
		BHLOG_ERROR("select record failed by dse");
	}
	return;
}

int CAlarmPlantInfo::SaveDynRecord(IDBInterFace * pDB_,const string &sTableNameSrc_,const string& sTableNameDes_,const CBHDateTime& tTime_,const int &iCurID_,const bool &bMonth_/* =false */ )
{
	int iRtn = 0;

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = '%s'",
		gc_cID,sTableNameDes_.c_str(),gc_cTime,tTime_.Format(gc_cTimeFormat));
	if(pDB_->More())
	{
		return iRtn;
	}
	CDynChannParam *pDynChannParam;
	if(sTableNameDes_ != _sDynTrendMonthTblName)
	{
		pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s = %d",gc_cID,sTableNameDes_,gc_cID,iCurID_);
		if(pDB_->More())
		{
			pDB_->Cmd("UPDATE a SET a.%s = %d",gc_cID,iCurID_);

			for(int nloop = 0; nloop < _vDynChann.size(); ++nloop)
			{
				pDynChannParam = (CDynChannParam *)_vDynChann[nloop];
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(0).c_str(),pDynChannParam->GetSpecCharTypeField(0).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(1).c_str(),pDynChannParam->GetSpecCharTypeField(1).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(2).c_str(),pDynChannParam->GetSpecCharTypeField(2).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(3).c_str(),pDynChannParam->GetSpecCharTypeField(3).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(4).c_str(),pDynChannParam->GetSpecCharTypeField(4).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(5).c_str(),pDynChannParam->GetSpecCharTypeField(5).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(6).c_str(),pDynChannParam->GetSpecCharTypeField(6).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(7).c_str(),pDynChannParam->GetSpecCharTypeField(7).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(8).c_str(),pDynChannParam->GetSpecCharTypeField(8).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(9).c_str(),pDynChannParam->GetSpecCharTypeField(9).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(10).c_str(),pDynChannParam->GetSpecCharTypeField(10).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(11).c_str(),pDynChannParam->GetSpecCharTypeField(11).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-1).c_str(),pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-3).c_str(),pDynChannParam->GetSpecCharTypeField(-3).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-4).c_str(),pDynChannParam->GetSpecCharTypeField(-4).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-5).c_str(),pDynChannParam->GetSpecCharTypeField(-5).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetSpecCharTypeField(-6).c_str());
				pDB_->Cmd(",a.%s = b.%s",pDynChannParam->GetSpecCharTypeField(-7).c_str(),pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}

			pDB_->Cmd(",a.%s = b.%s",gc_cSampleNum,gc_cSampleNum);
			pDB_->Cmd(",a.%s = b.%s",gc_cSampleFreq,gc_cSampleFreq);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev,gc_cRev);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev2,gc_cRev2);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev3,gc_cRev3);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev4,gc_cRev4);
			pDB_->Cmd(",a.%s = b.%s",gc_cRev5,gc_cRev5);
			pDB_->Cmd(",a.%s = b.%s)",gc_cTime,gc_cTime);

			pDB_->Cmd(" FROM [%s] a,[%s] b WHERE a.%s = %d and b.%s = '%s'",\
				sTableNameDes_.c_str(),sTableNameSrc_.c_str(),gc_cID,iCurID_,gc_cTime,tTime_.Format(gc_cTimeFormat));

			if(pDB_->Exec())
			{
				iRtn = 1;
			}
			return iRtn;
		}
	}
	pDB_->Cmd("INSERT INTO [%s](%s,%s,%s) SELECT %d,%s,%s FROM [%s] WHERE [%s] = '%s'",
		sTableNameDes_.c_str(),gc_cID,_sDynTrendField.c_str(),_sDynTrendWaveField.c_str(),
		iCurID_,_sDynTrendField.c_str(),_sDynTrendWaveField.c_str(),sTableNameSrc_.c_str(),
		gc_cTime,tTime_.Format(gc_cTimeFormat));
	if(pDB_->Exec())
	{
		iRtn = 1;
	}
	return iRtn;
}

void CAlarmPlantInfo::SaveDynAlarmDetailDataToTrend(IDBInterFace * pDB_,bool bSaved_,CBHDateTime tSaveTime_)
{
	
	int iRtn = 0;

	if(g_bIsDiskFull)
	{
		return;
	}

	{
		int iCurrIDDynTrendMonth=0;
		if(bSaved_)
		{
			GetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
			iRtn = SaveDynRecord(pDB_,_sAlarmDynDataTblName,_sDynTrendMonthTblName,tSaveTime_,iCurrIDDynTrendMonth,true);
		}
		
	}

	
}


void CAlarmPlantInfo::SaveProcAlarmDetailDataToTrend(IDBInterFace * pDB_,string sChannNo_,CBHDateTime tTime_)
{


	{
		int iCurrIDProcTrendMonth=0;
		SaveProcRecord(pDB_,_sProcTrendMonthTblName,iCurrIDProcTrendMonth,sChannNo_,tTime_,true);

	}
}

void CAlarmPlantInfo::SaveProcRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,string sChannNo_,CBHDateTime tTime_,bool bMonth_ /* =false */ )
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	float pfRev[5];
	float fData=.0;
	CString strTimeCurr;
	GetRealRev( pfRev, 5);

	//得到断网数据当前的时间
	CBHDateTime dtCurr = CBHDateTime::GetCurrentTimeEx();
	strTimeCurr =CPlantInfoBase::TimeConvertString(dtCurr);

	pDB_->Cmd("SELECT top 1 %s  FROM [%s] WHERE [%s]='%s'",
		gc_cDateTime,
		sTableName_.c_str(),
		gc_cDateTime, strTimeCurr);
	if(pDB_->More())
	{
		return;		
	}

	CProcChannParam *pProcChann=NULL;
	if(!bMonth_)
	{
		return;
	}
	pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' and %s = '%s'",
		gc_cProcValue,_sAlarmProcDataTblName.c_str(),gc_cTime,tTime_.Format(),gc_cChannNo,sChannNo_.c_str());
	if(pDB_->More())
	{
		float lfValue = pDB_->GetDouble(gc_cProcValue);

		pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s'",gc_cTime,sTableName_.c_str(),gc_cTime,tTime_.Format());
		if(pDB_->More())
		{
			pDB_->Cmd("UPDATE %s SET [%s] = %f  WHERE %s = '%s'",sTableName_.c_str(),sChannNo_.c_str(),lfValue,gc_cTime,tTime_.Format());
		}
		else
		{
			int iCurrIDProcTrendMonth=0;
			GetProcTrendMonthCurrID(iCurrIDProcTrendMonth);
			iCurID_ = iCurrIDProcTrendMonth;
			//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
			pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sProcField.c_str());
			pDB_->Cmd("VALUES (%d,",iCurID_);
			for (V_PCHANNINFO::iterator result=_vProcChann.begin();result!=_vProcChann.end();result++)
			{
				_pLockProcData->BeginRead();
				try
				{
					fData=((CProcChannParam*)(*result))->GetData();
					CString lstrChannNo = ((CProcChannParam*)(*result))->GetChannInfo()._cChannNo;
					if( lstrChannNo == sChannNo_.c_str() )
					{
						fData = lfValue;
					}
				}
				catch (...)
				{
					ASSERT(FALSE);
					CString lstrErrorLog;
					lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
					CHZLogManage::Fatal(lstrErrorLog);
					zdlTraceLine(lstrErrorLog);
				}

				_pLockProcData->EndRead();
				pDB_->Cmd(" %.2f,",fData);
			}
			pDB_->Cmd("'%s',",tTime_.Format());
			pDB_->Cmd(" %.2f,",pfRev[0]);
			pDB_->Cmd(" %.2f,",pfRev[1]);
			pDB_->Cmd(" %.2f,",pfRev[2]);
			pDB_->Cmd(" %.2f,",pfRev[3]);
			pDB_->Cmd(" %.2f)",pfRev[4]);

		}

		if(!pDB_->Exec())
		{
			BHLOG_ERROR("select record failed by dse");
		}

	}

}


void CAlarmPlantInfo::GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();

	try
	{
		iCurrIDVibTrendSecond_=_pRealPlantInfo->_iCurrIDVibTrendSecond;
		if (_pRealPlantInfo->_iCurrIDVibTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDVibTrendSecond = 0;
			}
			else
			{
				++_pRealPlantInfo->_iCurrIDVibTrendSecond;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDVibTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendHour_=_pRealPlantInfo->_iCurrIDVibTrendHour;
		if (_pRealPlantInfo->_iCurrIDVibTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDVibTrendHour = 0;
			}
			else
			{
				++_pRealPlantInfo->_iCurrIDVibTrendHour;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDVibTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendDay_=_pRealPlantInfo->_iCurrIDVibTrendDay;
		if (_pRealPlantInfo->_iCurrIDVibTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDVibTrendDay = 0;
			}
			else
			{
				++_pRealPlantInfo->_iCurrIDVibTrendDay;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDVibTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	try
	{
		iCurrIDVibTrendMonth_=_pRealPlantInfo->_iCurrIDVibTrendMonth;

		++_pRealPlantInfo->_iCurrIDVibTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendSecond_=_pRealPlantInfo->_iCurrIDProcTrendSecond;
		if (_pRealPlantInfo->_iCurrIDProcTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDProcTrendSecond = 0;
			}	
			else
			{
				++_pRealPlantInfo->_iCurrIDProcTrendSecond;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDProcTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CAlarmPlantInfo::GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendHour_=_pRealPlantInfo->_iCurrIDProcTrendHour;
		if (_pRealPlantInfo->_iCurrIDProcTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDProcTrendHour = 0;
			}
			else
			{
				++_pRealPlantInfo->_iCurrIDProcTrendHour;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDProcTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CAlarmPlantInfo::GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendDay_=_pRealPlantInfo->_iCurrIDProcTrendDay;
		if (_pRealPlantInfo->_iCurrIDProcTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDProcTrendDay = 0;
			}
			else
			{
				++_pRealPlantInfo->_iCurrIDProcTrendDay;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDProcTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CAlarmPlantInfo::GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockProcData->BeginWrite();
	try
	{
		iCurrIDProcTrendMonth_=_pRealPlantInfo->_iCurrIDProcTrendMonth;

		++_pRealPlantInfo->_iCurrIDProcTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockProcData->EndWrite();
}

void CAlarmPlantInfo::GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendSecond_=_pRealPlantInfo->_iCurrIDDynTrendSecond;
		if (_pRealPlantInfo->_iCurrIDDynTrendSecond>=G_D_TREND_SECONDRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDDynTrendSecond = 0;
			}	
			else
			{
				++_pRealPlantInfo->_iCurrIDDynTrendSecond;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDDynTrendSecond;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendHour_=_pRealPlantInfo->_iCurrIDDynTrendHour;
		if (_pRealPlantInfo->_iCurrIDDynTrendHour>=G_D_TREND_HOURRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDDynTrendHour = 0;
			}	
			else
			{
				++_pRealPlantInfo->_iCurrIDDynTrendHour;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDDynTrendHour;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendDay_=_pRealPlantInfo->_iCurrIDDynTrendDay;
		if (_pRealPlantInfo->_iCurrIDDynTrendDay>=G_D_TREND_DAYRANGE)
		{
			if (!G_TEMP_ONLINE)
			{
				_pRealPlantInfo->_iCurrIDDynTrendDay = 0;
			}	
			else
			{
				++_pRealPlantInfo->_iCurrIDDynTrendDay;
			}
		}
		else
		{
			++_pRealPlantInfo->_iCurrIDDynTrendDay;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockDynData->BeginWrite();
	try
	{
		iCurrIDDynTrendMonth_=_pRealPlantInfo->_iCurrIDDynTrendMonth;

		++_pRealPlantInfo->_iCurrIDDynTrendMonth;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	_pLockDynData->EndWrite();
}

void CAlarmPlantInfo::SetIDPointer( CRealPlantInfo *pRealPlantInfo_ )
{
	ASSERT(NULL == this->_pRealPlantInfo);
	this->_pLockChannInfo->BeginWrite();
	_pRealPlantInfo = pRealPlantInfo_;
	this->_pLockChannInfo->EndWrite();
}

int CAlarmPlantInfo::JudgeProcAlarmEx( IDBInterFace *pDB_,
									  bool abSaveData_ /*= true*/,
									  bool abConfirmAlarm_ /*= true*/ )
{
	bool bUpdate = false;

	_pLockDataUpdateStatues->BeginRead();
	bUpdate = _bProcDataUpdate;
	_pLockDataUpdateStatues->EndRead();

	if (bUpdate)
	{
		_pLockDataUpdateStatues->BeginWrite();
		_bProcDataUpdate = false;
		_pLockDataUpdateStatues->EndWrite();
	}
	else
	{	
		return 0;
	}

	int iTmp = 0;

	_CHANN_ALARM_STATUS_ iFlag = CHANN_ALARM_STATUS_NORMAL;

	_pLockProcData->BeginWrite();

	try
	{
		int nLoop = -1;

		_CHANN_ALARM_STATUS_  lnChannStatus = CHANN_ALARM_STATUS_NORMAL;

		bool lbPlantAlarmed = this->GetPlantAlarmState();

		bool IslocalJudgeAlarm = isLocalJudgeAlarm();

		for (V_PCHANNINFO::iterator it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			++nLoop;
			if(!abSaveData_ && 
				_vProcFastChangeChannNo.size() > nLoop &&
				_vProcFastChangeChannNo[nLoop] == 0)
			{
				
				continue;
			}
			

			CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);
			lnChannStatus = lpProc->PredictChannAlarm(pDB_,
													  lbPlantAlarmed,
													  abConfirmAlarm_,
													  IslocalJudgeAlarm);

			if(iFlag < lnChannStatus)
			{
				iFlag = lnChannStatus;
			}
			lpProc->SetChannAlarmLevel(lnChannStatus);
		}


		if (CHANN_ALARM_STATUS_NEW_ALARM == iFlag )
		{
			//1.1重置所有测点的正常计时器timer5，将所有测点的报警日志状态设置为停止
			this->StopAllChannsAlarmLogForNewAlarm(pDB_);
			//1.2 增加报警日志id
			this->IncreaseEventID();
			
			//1.3 重置数据保存计时器
			this->ResetNewAlarmTime();
			this->ResetHalfNormalTime();
			this->ResetSaveHalfIndenseTime();
			this->ResetLastSaveSparseTime();
		}else if (this->InnerPlantAlarmStatus() == PLANT_STATUS_NEW_ALARM)
		{
			this->StopProcChannsAlarmLogForNewAlarm(pDB_);
		}

		

		nLoop = -1;
		for (V_PCHANNINFO::iterator it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			++nLoop;

			if(!abSaveData_ && 
				_vProcFastChangeChannNo.size() > nLoop &&
				_vProcFastChangeChannNo[nLoop] == 0)
			{
				CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);
				if(lpProc == NULL)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,0,!abSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,lpProc->GetChannRegAlarmType(),!abSaveData_);
				}
			
				continue;
			}

			CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);

			if(lpProc == NULL)
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,0,!abSaveData_);
			}
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,lpProc->GetChannRegAlarmType(),!abSaveData_);


				_CHANN_ALARM_STATUS_ lnChannStatus = lpProc->PredictedChannInnerAlarmStatus();


				if (lnChannStatus <= CHANN_ALARM_STATUS_HALF_NORMAL
					&&
					this->InnerPlantAlarmStatus() == PLANT_STATUS_NEW_ALARM)
				{
					lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
				}

				lpProc->_iEventIDReg = this->_iEventIdReg;

				lpProc->SwitchAlarmStatus(pDB_,
					lnChannStatus,
					abSaveData_);

				lpProc->PlantNewAlarmOccoured(FALSE);
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndWrite();

	return iFlag;
}

BOOL CAlarmPlantInfo::IsEnabledNewDCLogic() const
{
	return m_bIsEnabledNewDCLogic;
}

void CAlarmPlantInfo::IsEnabledNewDCLogic( BOOL val )
{
	m_bIsEnabledNewDCLogic = val;
}

int CAlarmPlantInfo::LastRunningStatus() const
{
	return m_nLastRunningStatus;
}

void CAlarmPlantInfo::LastRunningStatus( int val )
{
	InterlockedExchange((long *)&this->m_nLastRunningStatus,val);
	//m_nLastRunningStatus = val;
}

int CAlarmPlantInfo::CurrentRunningStatus() const
{
	return m_nCurrentRunningStatus;
}

void CAlarmPlantInfo::CurrentRunningStatus( int val )
{
	this->LastRunningStatus(this->m_nCurrentRunningStatus);
	m_nCurrentRunningStatus = val;
}



BOOL CAlarmPlantInfo::DealWithNewDCLogic()
{
	bool lbRun = this->GetVibJudgeRunStatus() || this->GetProcJudgeRunStatus();
	BOOL lbIsPlantRunning = ::IsPlantRunning(*this,lbRun);

	this->CurrentRunningStatus(lbIsPlantRunning);
	
	if (lbIsPlantRunning )
	{
		if (g_UseNewDCAlarmLogic&&g_UseNewAlarmLogic)
		{
			//发生了起停车状态切换
			BOOL lbRunningStatusChanged =(lbIsPlantRunning!=this->LastRunningStatus());
			this->DealWithChannsRodsinkLogic(lbRunningStatusChanged);	
		}

	}else
	{
		if (g_UseNewAxisAlarmLogic&&g_UseNewAlarmLogic)
		{
			BOOL lbRunningStatusChanged =(lbIsPlantRunning!=this->LastRunningStatus());
			this->DealWithChannsAxisLogic(lbRunningStatusChanged);
		}

	}
	return FALSE;
}

BOOL CAlarmPlantInfo::DealWithChannsRodsinkLogic( BOOL abRunningStatusChanged )
{
	V_PCHANNINFO::iterator itDyn =  _vDynChann.end();

	BOOL lbRet = TRUE;
	
	
	//2.进入动态通道循环，做活塞杆沉降/偏摆的直流量处理
	for (itDyn=_vDynChann.begin();itDyn!=_vDynChann.end();++itDyn)
	{
		CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)(*itDyn);

		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE;
		
		if (NULL == lpDyn)
		{
			ASSERT(FALSE);

		}else
		{
			S_ChannInfo loChannInfo = lpDyn->GetChannInfo();

			if (GE_RODSINKCHANN == loChannInfo._iChannType)
			{
				lpDyn->JudgeAndDoForNewDCLogic(abRunningStatusChanged);
			}
		}
		
		END_ERROR_HANDLE;
		_pLockVibData->EndRead();
	}

	return lbRet;
}


BOOL CAlarmPlantInfo::DealWithChannsAxisLogic(BOOL abRunningStatusChanged)
{
	V_PCHANNINFO::iterator itVib =  _vVibChann.end();

	BOOL lbRet = TRUE;


	//处理轴位移测点的轴位移值
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)(*itVib);

		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE;

		if (NULL == lpVib)
		{
			ASSERT(FALSE);

		}else
		{
			S_ChannInfo loChannInfo = lpVib->GetChannInfo();

			if (GE_AXIALCHANN == loChannInfo._iChannType)
			{
				lpVib->JudgeAndDoForNewAxisLogic(abRunningStatusChanged);
			}
		}

		END_ERROR_HANDLE;
		_pLockVibData->EndRead();
	}

	return lbRet;
}

int CAlarmPlantInfo::SaveVibFAlarmData(int iStartIndex_,
									   int iNum_,
									   S_VibCharValue *pVibCharValue_,
									   unsigned int *piType_,
									   DATATYPE_WAVE *pWave_,
									   S_CustomParamData *pfCustomParom_,
									   string sTempletID_,
									   long lEventID_,
									   CBHDateTime dtDataTime_,
									   int iMicroSecond_,
									   int iRealAllNum_,
									   IDBInterFace *pDB_,
									   const float& fScaleCoef_,
									   int anPrecisionEnabled,
									   std::vector<float>& afPreciousMin ,
									   std::vector<float>&  afPreciousMax)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int iRtn=-1;
	if ((iStartIndex_<0)||((iStartIndex_+iNum_)> _sPlantInfo._iVibChannNum)||(iNum_<1))
		return iRtn;

	int nloop=0;
	int iNums=iNum_+iStartIndex_;

	//得到采样频率，采样点数和转速
	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};

	iSmpFreq=pVibCharValue_[nCount]._iSmpFreq;
	iSmpNum=pVibCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=_vVibChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
			break;
		case 2:
			pfRev[1] = pVibCharValue_[nCount]._iRev;
			break;
		case 3:
			pfRev[2] = pVibCharValue_[nCount]._iRev;
			break;
		case 4:
			pfRev[3] = pVibCharValue_[nCount]._iRev;
			break;
		case 5:
			pfRev[4] = pVibCharValue_[nCount]._iRev;
			break;
		default:
			pfRev[0] = pVibCharValue_[nCount]._iRev;
		}
		++nCount;
	}

	if(g_bIsDiskFull && iStartIndex_ + iNum_ == _sPlantInfo._iVibChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pVibCharValue_[n]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < iStartIndex_; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sAlarmVibDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vVibChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pVibCharValue_[n]._iSmpNum;
			}
		}

		if(!DeleteVibTrendRecord(pDB_,lnTotalWavePtNum))
		{
			DeleteVibAlarmRecord(pDB_,lEventID_,lnTotalWavePtNum);
		}
	}

	S_VibCharValue vibCharValue;
	CVibChannParamAlarm *pVibChannParam;
	CString strSql;//构造SQL语句。
	//构造查询这条报警数据记录是否已经存在
	strSql.Format("select 1 from [%s] where [%s]='%s' AND [%s]=%d AND [%s]=%d ",_sAlarmVibDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
	pDB_->BeginTran();
	pDB_->Cmd(strSql);
	//插入记录
	if (!pDB_->More())
	{

		pDB_->ClearSql();
		pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sAlarmVibDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{
			pVibChannParam = (CVibChannParamAlarm*)_vVibChann[nloop];
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-2).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		}
		pDB_->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",lEventID_,TimeConvertString(dtDataTime_),iMicroSecond_,pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4],iSmpFreq,iSmpNum);
		for (nloop=0;nloop<iNum_;++nloop)
		{
			vibCharValue=pVibCharValue_[nloop];
			pDB_->Cmd(",%.2f",vibCharValue._fCharAll);
			pDB_->Cmd(",%.2f",vibCharValue._fCharOne);
			pDB_->Cmd(",%.2f",vibCharValue._fCharTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fCharThree);
			pDB_->Cmd(",%.2f",vibCharValue._fCharHalf);
			pDB_->Cmd(",%.2f",vibCharValue._fCharRes);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseOne);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseTwo);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseThree);
			pDB_->Cmd(",%.2f",vibCharValue._fPhaseHalf);
			pDB_->Cmd(",%.2f",vibCharValue._fDC);
			pDB_->Cmd(",%.2f",vibCharValue._fGap);
			pDB_->Cmd(",%d",piType_[nloop]);
			pDB_->Cmd(",%d",vibCharValue._iSmpFreq);
			pDB_->Cmd(",%d",vibCharValue._iSmpNum);
			pDB_->Cmd(",%d",vibCharValue._iRev);
			pDB_->Cmd(",'%s'",_vVibChann[nloop + iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(")");
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	else
	{
		string sChann;
		int index=0;
		pDB_->Cmd("UPDATE [%s] SET ",_sAlarmVibDataTblName.c_str());
		for (nloop=0;nloop<iNum_;++nloop)
		{
			vibCharValue=pVibCharValue_[nloop];
			sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			pVibChannParam = (CVibChannParamAlarm*)_vVibChann[nloop + iStartIndex_];
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ALLCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_ONECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_TWOCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibCharValue.GetSpecTypeData(GE_THREECHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_HALFCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_RESCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARONE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTWO));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARTHREE));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibCharValue.GetSpecTypeData(GE_PHASECHARHALF));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibCharValue.GetSpecTypeData(GE_DCCHAR));
			pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibCharValue.GetSpecTypeData(GE_GAP));
			pDB_->Cmd(" [%s]=%d,   ",pVibChannParam->GetSpecCharTypeField(-2).c_str(),piType_[nloop]);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibCharValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibCharValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibCharValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd("[%s]=%d, ",gc_cSampleNum,iSmpNum);
		pDB_->Cmd("[%s]=%d, ",gc_cSampleFreq,iSmpFreq);
		pDB_->Cmd("[%s]=%.2f, ",gc_cRev,pfRev[0]);
		pDB_->Cmd("[%s]=%.2f, ",gc_cRev2,pfRev[1]);
		pDB_->Cmd("[%s]=%.2f, ",gc_cRev3,pfRev[2]);
		pDB_->Cmd("[%s]=%.2f, ",gc_cRev4,pfRev[3]);
		pDB_->Cmd("[%s]=%.2f, ",gc_cRev5,pfRev[4]);
		pDB_->Cmd("[%s]=%d ",gc_cMicroSecond,iMicroSecond_);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]=%d AND [%s]=%d ",gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmVibDataTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sAlarmVibDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}

	if(lnID > 0)
	{
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
			pDB_->Cmd(",%s",_vVibWaveField[nloop]);
		pDB_->Cmd(" FROM [%s] ",_sAlarmVibDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,lnID);

	}
	else
	{
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
			pDB_->Cmd(",%s",_vVibWaveField[nloop]);
		pDB_->Cmd(" FROM [%s] ",_sAlarmVibDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

	}

	if (pDB_->More(TRUE))
	{
		string sChann;
		int iRealNum=0;
		CWaveHead lsHead;
		lsHead.SetLength(4);
		for (nloop=0;nloop<iNum_;++nloop)
		{
			sChann=_vVibChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;

			pVibChannParam = (CVibChannParamAlarm *)_vVibChann[nloop + iStartIndex_];

			vector<BYTE> lvWave;
			
			if(!IsFloatZero(fScaleCoef_))
			{
				lsHead.m_pParam[0] = fScaleCoef_;
			}
			if(anPrecisionEnabled)
			{
				lsHead.m_pParam[1] = 1.0;
				lsHead.m_pParam[2] = afPreciousMin[nloop];
				lsHead.m_pParam[3] = afPreciousMax[nloop];
			}
			else
			{
				lsHead.m_pParam[1] = 0;
			}
			int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pVibCharValue_[nloop]._iSmpNum,lsHead,lvWave);

			if(!(pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
			{
				BHLOG_ERROR(STR("同步振动报警数据，保存波形失败"));
			}
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom),sizeof(S_CustomParamData));
			iRealNum+=pVibCharValue_[nloop]._iSmpNum;
			iRtn=nloop+1;
		}
	}

QUIT_SAVEFALARM:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	
	return iRtn;
}

int CAlarmPlantInfo::SaveDynAlarmData(int iStartIndex_,
									  int iNum_,
									  S_DynCharValue *pDynCharValue_,
									  unsigned int *piType_,
									  DATATYPE_WAVE *pWave_,
									  S_CustomParamData *pfCustomParom_,
									  string sTempletID_,
									  long lEventID_,
									  CBHDateTime dtDataTime_,
									  int iMicroSecond_,
									  int iRealAllNum_,
									  IDBInterFace *pDB_,
									  const float& fScaleCoef_,
									  int anPrecisionEnabled,
									  std::vector<float>& afPreciousMin ,
									  std::vector<float>&  afPreciousMax)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn = -1;
	if ((iStartIndex_<0)||((iStartIndex_+iNum_)> _sPlantInfo._iDynChannNum)||(iNum_<1))
		return iRtn;
	
	int nloop=0,nloop1;
	int iNums=iNum_+iStartIndex_;
	//得到采样频率，采样点数和转速

	int nCount=0,iSmpFreq=0,iSmpNum=0;
	float pfRev[5]={.0,.0,.0,.0,.0};
	iSmpFreq=pDynCharValue_[nCount]._iSmpFreq;
	iSmpNum=pDynCharValue_[nCount]._iSmpNum;
	for (nloop=iStartIndex_;nloop<iNums;++nloop)
	{
		int iRevType=(_vDynChann[nloop])->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			pfRev[0] = pDynCharValue_[nCount]._iRev;
			break;
		case 2:
			pfRev[1] = pDynCharValue_[nCount]._iRev;
			break;
		case 3:
			pfRev[2] = pDynCharValue_[nCount]._iRev;
			break;
		case 4:
			pfRev[3] = pDynCharValue_[nCount]._iRev;
			break;
		case 5:
			pfRev[4] = pDynCharValue_[nCount]._iRev;
			break;
		default:
			pfRev[0] = pDynCharValue_[nCount]._iRev;
		}
		++nCount;
	}

	if(g_bIsDiskFull && iStartIndex_ + iNum_ == _sPlantInfo._iDynChannNum)
	{
		int lnTotalWavePtNum = 0;
		if(iStartIndex_ == 0)
		{
			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pDynCharValue_[n]._iSmpNum;
			}
		}
		else
		{
			CString lstrSelField;
			for(int n = 0; n < iStartIndex_; n++)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(this->_vDynChann[n]->_channInfo._cChannNo);
				lstrSelField += ",";
			}

			if(lstrSelField.GetLength() > 0)
			{
				lstrSelField.Delete(lstrSelField.GetLength() - 1);
			}

			pDB_->Cmd("SELECT %s FROM %s WHERE %s = '%s' AND %s = %d",lstrSelField,_sAlarmDynDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

			if(pDB_->More())
			{
				for(int n = 0; n < iStartIndex_; n++)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(this->_vDynChann[n]->_channInfo._cChannNo);
					lnTotalWavePtNum += pDB_->GetLong(lstrColumn);
				}
			}

			for(int n = 0; n < iNum_; n++)
			{
				lnTotalWavePtNum += pDynCharValue_[n]._iSmpNum;
			}
		}

		if(!DeleteDynTrendRecord(pDB_,lnTotalWavePtNum))
		{
			DeleteDynAlarmRecord(pDB_,lEventID_,lnTotalWavePtNum);
		}
	}


	S_DynCharValue dynCharValue;
	CDynChannParam* pDynChannParam;
	CString strSql;//构造SQL语句。
	//构造查询这条报警数据记录是否已经存在
	strSql.Format("select 1 from [%s] where [%s]='%s' AND [%s]=%d AND [%s]=%d ",_sAlarmDynDataTblName.c_str(),gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
	pDB_->BeginTran();
	pDB_->Cmd(strSql);
	//插入记录
	if (!pDB_->More())
	{

		pDB_->ClearSql();
		pDB_->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sAlarmDynDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
		{
			pDynChannParam = (CDynChannParam*)_vDynChann[nloop];

			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-2).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());

		}
		pDB_->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",lEventID_,TimeConvertString(dtDataTime_),iMicroSecond_,pfRev[0],pfRev[1],pfRev[2],pfRev[3],pfRev[4],iSmpFreq,iSmpNum);

		for (nloop=0;nloop<iNum_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			for (nloop1 = 0; nloop1 < 12; nloop1++)
				pDB_->Cmd(",%.2f",dynCharValue.GetSpecTypeData(nloop1));
			pDB_->Cmd(",%d",piType_[nloop]);
			pDB_->Cmd(",%d",dynCharValue._iSmpFreq);
			pDB_->Cmd(",%d",dynCharValue._iSmpNum);
			pDB_->Cmd(",%d",dynCharValue._iRev);
			pDB_->Cmd(",'%s'",_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd(")");
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}
	else
	{
		string sChann;
		int index=0;
		pDB_->Cmd("UPDATE [%s] SET ",_sAlarmDynDataTblName.c_str());
		for (nloop=0;nloop<iNum_;++nloop)
		{
			dynCharValue=pDynCharValue_[nloop];
			pDynChannParam=(CDynChannParam*)(_vDynChann[nloop + iStartIndex_]);
			sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),dynCharValue.GetSpecTypeData(0));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),dynCharValue.GetSpecTypeData(1));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),dynCharValue.GetSpecTypeData(2));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),dynCharValue.GetSpecTypeData(3));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),dynCharValue.GetSpecTypeData(4));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),dynCharValue.GetSpecTypeData(5));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),dynCharValue.GetSpecTypeData(6));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),dynCharValue.GetSpecTypeData(7));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),dynCharValue.GetSpecTypeData(8));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),dynCharValue.GetSpecTypeData(9));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),dynCharValue.GetSpecTypeData(10));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),dynCharValue.GetSpecTypeData(11));
			pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(-2).c_str(),piType_[nloop]);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),dynCharValue._iSmpFreq);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),dynCharValue._iSmpNum);
			pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),dynCharValue._iRev);
			pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cTempID);
		}
		pDB_->Cmd("[%s]=%d ",gc_cMicroSecond,iMicroSecond_);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]=%d AND [%s]=%d ",gc_cTime,TimeConvertString(dtDataTime_),gc_cMicroSecond,iMicroSecond_,gc_cEventID,lEventID_);
		if (!(pDB_->Exec()))
		{
			iRtn= -2;
			goto QUIT_SAVEFALARM;
		}
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sAlarmDynDataTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sAlarmDynDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}

	if(lnID > 0)
	{
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
			pDB_->Cmd(",%s",_vDynWaveField[nloop]);
		pDB_->Cmd(" FROM [%s] ",_sAlarmDynDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d ",gc_cID,lnID);

	}
	else
	{
		pDB_->Cmd("SELECT 1");
		for (nloop=iStartIndex_;nloop<iNums;++nloop)
			pDB_->Cmd(",%s",_vDynWaveField[nloop]);
		pDB_->Cmd(" FROM [%s] ",_sAlarmDynDataTblName.c_str());
		pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMicroSecond_,gc_cTime,TimeConvertString(dtDataTime_),gc_cEventID,lEventID_);

	}

	if (pDB_->More(TRUE))
	{
		string sChann;
		int iRealNum =0;
		CWaveHead lsHead;
		lsHead.SetLength(4);

		for (nloop=0;nloop<iNum_;++nloop)
		{
			sChann=_vDynChann[nloop+iStartIndex_]->GetChannInfo()._cChannNo;

			pDynChannParam = (CDynChannParam *)(_vDynChann[nloop + iStartIndex_]);

			vector<BYTE> lvWave;
			
			if(!IsFloatZero(fScaleCoef_))
			{
				lsHead.m_pParam[0] = fScaleCoef_;
			}
			if(anPrecisionEnabled)
			{
				lsHead.m_pParam[1] = 1.0;
				lsHead.m_pParam[2] = afPreciousMin[nloop];
				lsHead.m_pParam[3] = afPreciousMax[nloop];
			}
			else
			{
				lsHead.m_pParam[1] = 0;
			}

			int lnTotalSize = this->WaveAddHead(&pWave_[iRealNum],pDynCharValue_[nloop]._iSmpNum,lsHead,lvWave);

			if(!(pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
			{
				BHLOG_ERROR(STR("同步动态报警数据，保存波形失败"));
			}

			if(!pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(pfCustomParom_[nloop].pfCustomParom), sizeof(S_CustomParamData)))
			{
				BHLOG_ERROR(STR("同步动态报警数据，保存自定义buff数据失败"));
			}
			iRealNum+=pDynCharValue_[nloop]._iSmpNum;
			iRtn=nloop+1;
		}
	}
QUIT_SAVEFALARM:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	
	return iRtn;
}



int CAlarmPlantInfo::SaveProcAlarmData(
	IDBInterFace *  pDB_,
	int             AlarmType_,
	int             EventID_,
	CBHDateTime    DateTime_,
	int            iMicroSecond_,
	CString         ChannNo_,
	int             ChannType_,
	float           ChannValue_,
	int             Rev_
	)
{
	ASSERT(pDB_);
	ASSERT(!_sCompanyAlias.empty());

	CString lsDataDbName;
	lsDataDbName.Format("%s%s", _sCompanyAlias.c_str(), _sPlantInfo._cPlantNo);

	SwitchDbNameEx(pDB_, lsDataDbName);

	

	int iRtn = 1;

	// 查询这条报警数据记录是否已经存在: 不存在时Insert, 存在时Update

	CString lsCondition;
	lsCondition.Format("[%s]=%d AND [%s]='%s' AND [%s]='%s' and [%s] = %d ",
		gc_cEventID, EventID_,
		gc_cChannNo, ChannNo_,
		gc_cTime, TimeConvertString(DateTime_),
		gc_cMicroSecond,iMicroSecond_);

	pDB_->BeginTran();

	do {
		pDB_->Cmd("select 1 from [%s] where %s", _sAlarmProcDataTblName.c_str(), lsCondition);
		if (!pDB_->More())
		{
			if(g_bIsDiskFull)
			{
				if(!this->DeleteProcTrendRecord(pDB_))
				{
					this->DeleteProcAlarmRecord(pDB_,EventID_);
				}
			}

			pDB_->ClearSql();

			pDB_->Cmd("insert into [%s]", _sAlarmProcDataTblName.c_str());
			pDB_->Cmd("(%s,%s,%s,%s,%s,[%s],[%s],%s,[%s])",
				gc_cEventID, gc_cChannNo, gc_cChannType, gc_cProcValue, gc_cRev, gc_cType, gc_cDateTime, gc_cSynFlag,gc_cMicroSecond); 
			pDB_->Cmd("values(%d,'%s',%d,%.2f,%d,%d,'%s',%d,%d)",
				EventID_, ChannNo_, ChannType_, ChannValue_, Rev_, AlarmType_, TimeConvertString(DateTime_), 0,iMicroSecond_);
			if (!(pDB_->Exec()))
			{
				iRtn= -2;
				break;
			}
		}
		else
		{
			pDB_->Cmd("UPDATE [%s] SET", _sAlarmProcDataTblName.c_str());
			pDB_->Cmd("%s=%d", gc_cChannType, ChannType_);
			pDB_->Cmd(",%s=%.2f", gc_cProcValue, ChannValue_);
			pDB_->Cmd(",%s=%d", gc_cRev, Rev_);
			pDB_->Cmd(",[%s]=%d", gc_cType, AlarmType_);
			pDB_->Cmd(",%s=%d", gc_cSynFlag, 0);
			pDB_->Cmd(" WHERE %s", lsCondition);
			if (!(pDB_->Exec()))
			{
				iRtn = -2;
				break;
			}
		}
	} while(0);


	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();

	return iRtn;
}

BOOL CAlarmPlantInfo::StopProcChannsAlarmLogForNewAlarm( IDBInterFace *pDB_ )
{
	V_PCHANNINFO::iterator itVib = NULL;
	V_PCHANNINFO::iterator itDyn = NULL;
	V_PCHANNINFO::iterator itProc = NULL;



	BEGIN_ERROR_HANDLE;
	
	//1.进入动态通道循环，停止报警状态
	for (itProc=_vProcChann.begin();itProc!=_vProcChann.end();++itProc)
	{

		_pLockProcData->BeginWrite();

		try
		{

			CProcChannParamAlarm * lpProc = (CProcChannParamAlarm*)(*itProc);

			if (NULL == lpProc)
			{
				ASSERT(FALSE);
				continue;
			}

			lpProc->HandleRegAlarmStopEx(pDB_);
			lpProc->ResetHalfNormalTimer5();
			lpProc->PlantNewAlarmOccoured(TRUE);
		}
		catch (...)
		{
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}


		_pLockProcData->EndWrite();

	}
	
	END_ERROR_HANDLE;

	return TRUE;
}

int CAlarmPlantInfo::SetProcJudgeRunStatus( int anProcJudgedRunStatus,int anLen_,int anStartIndex_ )
{
	int nCount=0;
	int iloop=0;
	if (((anLen_+anStartIndex_)> _sPlantInfo._iProcChannNum)||(anLen_<1)||(anStartIndex_<0))
		nCount=-1;
	else
	{
		anLen_=anLen_+anStartIndex_;
		nCount=0;
		_pLockProcData->BeginWrite();
		try
		{
			for (iloop=anStartIndex_;iloop<anLen_;++iloop)
			{
				((CProcChannParamAlarm*)_vProcChann[iloop])->SetSUDStatus(anProcJudgedRunStatus);
				nCount++;
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLockProcData->EndWrite();
	}
	return nCount;
}
int CAlarmPlantInfo::GetProcJudgeRunStatus( )
{
	int lnSpeedCount = 0;

	int lnStopCount = 0;

	bool lbJudgeSUDStatus = false;

	_pLockProcData->BeginRead();

	try
	{
		for (int lnProcChannIndex = 0; lnProcChannIndex < _sPlantInfo._iProcChannNum; lnProcChannIndex++)
		{
			int lnProcSUDStatus = ((CProcChannParamAlarm*)_vProcChann[lnProcChannIndex])->GetSUDStatus();
			//0表示不参与判断开停车，1表示开车，2表示停车
			if (lnProcSUDStatus == 0)
			{
				continue;
			}

			lbJudgeSUDStatus = true;

			if (lnProcSUDStatus == 1)
			{
				++lnSpeedCount;
			}
			else if(lnProcSUDStatus == 2)
			{
				++lnStopCount;
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndRead();

	if (!lbJudgeSUDStatus)
	{
		return 0;
	}

	return lnStopCount<lnSpeedCount?1:0;
}
void CAlarmPlantInfo::SetVibJudgeRunStatus( int anVibJudgedRunStatus )
{
	_pLockVibData->BeginWrite();
	this->m_nVibJudgedRunStatus = anVibJudgedRunStatus;
	_pLockVibData->EndWrite();
}
int CAlarmPlantInfo::GetVibJudgeRunStatus( )
{
	int lnVibJudgeRunStatus = 0;
	_pLockVibData->BeginRead();
	if (this->m_nVibJudgedRunStatus == 1)
	{
		lnVibJudgeRunStatus = 1;
	}
	else
	{
		lnVibJudgeRunStatus = 0;
	}
	//lnVibJudgeRunStatus = this->m_nVibJudgedRunStatus;
	_pLockVibData->EndRead();
	return lnVibJudgeRunStatus;
}

int CAlarmPlantInfo::ChangeVibAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{

	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if (anChannIndex<this->_vVibChann.size())
	{
		CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)this->_vVibChann.at(anChannIndex);

		if(lpVib)
		{
			lpVib->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
			lpVib->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
			lpVib->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
			lpVib->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
			lpVib->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
			lpVib->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
		}
	}

	END_ERROR_HANDLE

	this->_pLockVibData->EndWrite();

	return TRUE;
}

int CAlarmPlantInfo::ChangeDynAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockDynData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if (anChannIndex<this->_vDynChann.size())
	{
		CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)this->_vDynChann.at(anChannIndex);

		if(lpDyn)
		{
			lpDyn->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
			lpDyn->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
			lpDyn->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
			lpDyn->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
			lpDyn->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
			lpDyn->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
		}
	}

	END_ERROR_HANDLE

	this->_pLockDynData->EndWrite();

	return TRUE;
}

int CAlarmPlantInfo::ChangeProcAlarmLimitation( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockProcData->BeginWrite();

	BEGIN_ERROR_HANDLE

	if (anChannIndex<this->_vProcChann.size())
	{
		CProcChannParamAlarm * lpProc = (CProcChannParamAlarm*)this->_vProcChann.at(anChannIndex);

		if(lpProc)
		{
			lpProc->_channInfo._chAlarmParam._bRegular = aChannAlarmParam._bRegular;
			lpProc->_channInfo._chAlarmParam._iRegJudgeType = aChannAlarmParam._iRegJudgeType;
			lpProc->_channInfo._chAlarmParam._fAlarmHH = aChannAlarmParam._fAlarmHH;
			lpProc->_channInfo._chAlarmParam._fAlarmHL = aChannAlarmParam._fAlarmHL;
			lpProc->_channInfo._chAlarmParam._fAlarmLH = aChannAlarmParam._fAlarmLH;
			lpProc->_channInfo._chAlarmParam._fAlarmLL = aChannAlarmParam._fAlarmLL;
		}
	}

	END_ERROR_HANDLE

	this->_pLockProcData->EndWrite();

	return TRUE;
}

int CAlarmPlantInfo::SetVibGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{

	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockVibData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vVibChann.size())
		{
			CVibChannParamAlarm * lpVib = (CVibChannParamAlarm*)this->_vVibChann.at(anChannIndex);

			if(lpVib)
			{
				lpVib->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpVib->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpVib->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockVibData->EndWrite();

		return TRUE;
}

int CAlarmPlantInfo::SetDynGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockDynData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vDynChann.size())
		{
			CDynChannParamAlarm * lpDyn = (CDynChannParamAlarm*)this->_vDynChann.at(anChannIndex);

			if(lpDyn)
			{
				lpDyn->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpDyn->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpDyn->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockDynData->EndWrite();

		return TRUE;
}

int CAlarmPlantInfo::SetProcGradeThreshold( int anChannIndex,const CChannAlarmParam& aChannAlarmParam )
{
	if (anChannIndex<0)
	{
		return -1;
	}

	this->_pLockProcData->BeginWrite();

	BEGIN_ERROR_HANDLE

		if (anChannIndex<this->_vProcChann.size())
		{
			CProcChannParamAlarm * lpProc = (CProcChannParamAlarm*)this->_vProcChann.at(anChannIndex);

			if(lpProc)
			{
				lpProc->_channInfo._chAlarmParam._fAlarm1 = aChannAlarmParam._fAlarm1;
				lpProc->_channInfo._chAlarmParam._fAlarm2 = aChannAlarmParam._fAlarm2;
				lpProc->_channInfo._chAlarmParam._fAlarm3 = aChannAlarmParam._fAlarm3;
			}
		}

		END_ERROR_HANDLE

			this->_pLockProcData->EndWrite();

		return TRUE;
}

int CAlarmPlantInfo::JudgeFastRegAlarmEx( IDBInterFace *pDB_,bool bSaveData_ /*= true*/,bool bConfirmAlarm_ /*= true*/ )
{

	//1.判断全部测点的状态
	CAlarmPlantInfo::_PLANT_STATUS_ lnPlantStatus = this->PredictVibNewStatus(pDB_,bConfirmAlarm_);

	//2.切换状态
	BOOL lbResult = this->SwitchStatus(pDB_,lnPlantStatus,this->InnerPlantAlarmStatus(),bSaveData_);

	if(lnPlantStatus >= PLANT_STATUS_OLD_ALARM_HIGH)
	{
		return lnPlantStatus;
	}

	int lnDataSource = this->GetPlantInfo()._iDataSource;

	if(lbResult && ((lnDataSource & GE_MACHINE_DATA_SOURCE_WIRELESS) == GE_MACHINE_DATA_SOURCE_WIRELESS))
	{
		return PLANT_STATUS_OLD_ALARM_HIGH;
	}

	return lnPlantStatus;
}

int CAlarmPlantInfo::JudgeFastProcAlarmEx( IDBInterFace *pDB_,bool abSaveData_ /*= true*/,bool abConfirmAlarm_ /*= true*/ )
{
	int iTmp = 0;

	_CHANN_ALARM_STATUS_ iFlag = CHANN_ALARM_STATUS_NORMAL;

	_pLockProcData->BeginWrite();

	try
	{
		int nLoop = -1;

		_CHANN_ALARM_STATUS_  lnChannStatus = CHANN_ALARM_STATUS_NORMAL;

		bool lbPlantAlarmed = this->GetPlantAlarmState();

		bool IslocalJudgeAlarm = isLocalJudgeAlarm();

		for (V_PCHANNINFO::iterator it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			++nLoop;
			if(!abSaveData_ && 
				_vProcFastChangeChannNo.size() > nLoop &&
				_vProcFastChangeChannNo[nLoop] == 0)
			{

				continue;
			}


			CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);
			lnChannStatus = lpProc->PredictChannAlarm(pDB_,
				lbPlantAlarmed,
				abConfirmAlarm_,
				IslocalJudgeAlarm);

			if(iFlag < lnChannStatus)
			{
				iFlag = lnChannStatus;
			}
		}


		if (CHANN_ALARM_STATUS_NEW_ALARM == iFlag )
		{
			//1.1重置所有测点的正常计时器timer5，将所有测点的报警日志状态设置为停止
			this->StopAllChannsAlarmLogForNewAlarm(pDB_);
			//1.2 增加报警日志id
			this->IncreaseEventID();

			//1.3 重置数据保存计时器
			this->ResetNewAlarmTime();
			this->ResetHalfNormalTime();
			this->ResetSaveHalfIndenseTime();
			this->ResetLastSaveSparseTime();
		}else if (this->InnerPlantAlarmStatus() == PLANT_STATUS_NEW_ALARM)
		{
			this->StopProcChannsAlarmLogForNewAlarm(pDB_);
		}



		nLoop = -1;
		for (V_PCHANNINFO::iterator it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			++nLoop;

			if(!abSaveData_ && 
				_vProcFastChangeChannNo.size() > nLoop &&
				_vProcFastChangeChannNo[nLoop] == 0)
			{
				CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);
				if(lpProc == NULL)
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,0,!abSaveData_);
				}
				else
				{
					SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,lpProc->GetChannRegAlarmType(),!abSaveData_);
				}

				continue;
			}

			CProcChannParamAlarm* lpProc = (CProcChannParamAlarm*)(*it);

			if(lpProc == NULL)
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,0,!abSaveData_);
			}
			else
			{
				SetChannFastAlarmStatus(E_TBL_CHANNTYPE_PROC,lpProc->GetChannRegAlarmType(),!abSaveData_);


				_CHANN_ALARM_STATUS_ lnChannStatus = lpProc->PredictedChannInnerAlarmStatus();


				if (lnChannStatus <= CHANN_ALARM_STATUS_HALF_NORMAL
					&&
					this->InnerPlantAlarmStatus() == PLANT_STATUS_NEW_ALARM)
				{
					lnChannStatus = CHANN_ALARM_STATUS_NORMAL;
				}

				lpProc->_iEventIDReg = this->_iEventIdReg;

				lpProc->SwitchAlarmStatus(pDB_,
					lnChannStatus,
					abSaveData_);

				lpProc->PlantNewAlarmOccoured(FALSE);
			}
		}
	}
	catch (...)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	_pLockProcData->EndWrite();

	return iFlag;
}


int CAlarmPlantInfo::SaveVibFastDataFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_)
{
	int lnVibChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTime = CBHDateTime(2000,1,1,1,1,1,1);
	_pLockDynData->BeginRead();
	lnVibChannNum = this->GetVibChannNum();
	ltUpdateTime = this->_timeVibDataUpdate;
	_pLockDynData->EndRead();

	if(iStartIndex_ != 0 || (iStartIndex_ == 0 && iLen_ + iStartIndex_ >= lnVibChannNum) || iLen_ <= 0)
	{
		return 1;
	}

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int nloop = 0;

	int lnNum = iStartIndex_ + iLen_;

	
	CVibChannParamAlarm *pVibChannParam = NULL;
	S_VibCharValue vibValue;

	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s <= '%s' order by %s desc",
		gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTime = pDB_->GetTime(gc_cTime);
	}

	if(abs(ltUpdateTime - dtTime_) <= abs(ltTrendDataTime -dtTime_))
	{
		pDB_->Cmd("UPDATE a SET");

		for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam=(CVibChannParamAlarm*)(_vVibChann[nloop]);
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str(),pVibChannParam->GetSpecCharTypeField(-3).c_str());

			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str(),pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str(),pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" a.[%s]=%d,",pVibChannParam->GetSpecCharTypeField(-2).c_str(),0);
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetSpecCharTypeField(-6).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str(),pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str(),pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}
		pDB_->Cmd(" a.[%s] = %d",gc_cSynFlag,0);
		pDB_->Cmd(" FROM %s a, %s b where a.%s = '%s' and b.%s = '%s'",
			_sAlarmVibDataTblName.c_str(),_sVibTrendSecTblName.c_str(),
			gc_cTime,dtTime_.Format(),gc_cTime,ltTrendDataTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}

	}

	pDB_->Cmd("UPDATE [%s] SET ",_sAlarmVibDataTblName.c_str());

	for(nloop = lnNum; nloop < lnVibChannNum; ++nloop)
	{

		BOOL lbShouldSave = TRUE;

		_pLockVibData->BeginRead();

		BEGIN_ERROR_HANDLE
		pVibChannParam=(CVibChannParamAlarm*)(_vVibChann[nloop]);
		vibValue=pVibChannParam->GetCharValue();
		END_ERROR_HANDLE
		_pLockVibData->EndRead();
		
		vibValue._iSmpNum = this->GetSafeWavePointNum(vibValue._iSmpNum);

		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibValue.GetSpecTypeData(GE_ALLCHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibValue.GetSpecTypeData(GE_ONECHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibValue.GetSpecTypeData(GE_TWOCHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibValue.GetSpecTypeData(GE_THREECHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibValue.GetSpecTypeData(GE_HALFCHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibValue.GetSpecTypeData(GE_RESCHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARONE));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTWO));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibValue.GetSpecTypeData(GE_PHASECHARHALF));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibValue.GetSpecTypeData(GE_DCCHAR));
		pDB_->Cmd(" [%s]=%.2f, ",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibValue.GetSpecTypeData(GE_GAP));
		pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibValue._iSmpFreq);

		pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibValue._iSmpNum);
		pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibValue._iRev);
		pDB_->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-2).c_str(),pVibChannParam->GetRegAlarmStatus());
		pDB_->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cAlarmTempID);
	}

	pDB_->Cmd("[%s] = %d",gc_cSynFlag,0);
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMillSecond_,gc_cTime,TimeConvertString(dtTime_),gc_cEventID,iEventID_);
	if(!pDB_->Exec())
	{
		return -1;
	}
	//更新波形数据
	pDB_->Cmd("SELECT 1");
	for (nloop=lnNum;nloop<lnVibChannNum;++nloop)
		pDB_->Cmd(",%s",_vVibWaveField[nloop]);
	pDB_->Cmd(" FROM [%s] ",_sAlarmVibDataTblName.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMillSecond_,gc_cTime,TimeConvertString(dtTime_),gc_cEventID,iEventID_);
	if (pDB_->More(TRUE))
	{
		for (nloop = lnNum; nloop < lnVibChannNum;++nloop)
		{
			bool bRtn=false;
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParamAlarm*)_vVibChann[nloop];
			int iSmpNums=this->GetSafeWavePointNum(pVibChannParam->GetCharValue()._iSmpNum);

			vector<BYTE> lvWave;
			CWaveHead lsHead = pVibChannParam->WaveHead();
			int lnTotalSize = this->WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

			bRtn=pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),&lvWave.front(),lnTotalSize);
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pVibChannParam->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			END_ERROR_HANDLE
			_pLockVibData->EndRead();
		}
	}

	return 1;
}

int CAlarmPlantInfo::SaveDynFastDataFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_)
{
	int lnDynChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTime = CBHDateTime(2000,1,1,1,1,1,1);
	_pLockDynData->BeginRead();
	lnDynChannNum = this->GetDynChannNum();
	ltUpdateTime = this->_timeDynDataUpdate;
	_pLockDynData->EndRead();

	if(iStartIndex_ != 0 || (iStartIndex_ == 0 && iLen_ + iStartIndex_ >= lnDynChannNum) || iLen_ <= 0)
	{
		return 1;
	}

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int lnNum = iStartIndex_ + iLen_;
	int nloop = 0;

	CDynChannParamAlarm *pDynChannParam = NULL;
	S_DynCharValue DynValue;

	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s <= '%s' order by %s desc",
		gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTime = pDB_->GetTime(gc_cTime);
	}

	if(abs(ltUpdateTime - dtTime_) <= abs(ltTrendDataTime -dtTime_))
	{
		pDB_->Cmd("UPDATE a SET");

		for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam=(CDynChannParamAlarm*)(_vDynChann[nloop]);
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(0).c_str(),pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(1).c_str(),pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(2).c_str(),pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(3).c_str(),pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(4).c_str(),pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(5).c_str(),pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(6).c_str(),pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(7).c_str(),pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(8).c_str(),pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(9).c_str(),pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(10).c_str(),pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s], ",pDynChannParam->GetSpecCharTypeField(11).c_str(),pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str(),pDynChannParam->GetSpecCharTypeField(-3).c_str());

			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str(),pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str(),pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" a.[%s]=%d,",pDynChannParam->GetSpecCharTypeField(-2).c_str(),0);
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetSpecCharTypeField(-6).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str(),pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" a.[%s]=b.[%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str(),pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}
		pDB_->Cmd(" a.[%s] = %d",gc_cSynFlag,0);
		pDB_->Cmd(" FROM %s a, %s b where a.%s = '%s' and b.%s = '%s'",
			_sAlarmDynDataTblName.c_str(),_sDynTrendSecTblName.c_str(),
			gc_cTime,dtTime_.Format(),gc_cTime,ltTrendDataTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}

	}

	pDB_->Cmd("UPDATE [%s] SET ",_sAlarmDynDataTblName.c_str());

	for(nloop = lnNum; nloop < lnDynChannNum; ++nloop)
	{

		_pLockDynData->BeginRead();

		BEGIN_ERROR_HANDLE
		pDynChannParam=(CDynChannParamAlarm*)(_vDynChann[nloop]);
		DynValue=pDynChannParam->GetCharValue();
		END_ERROR_HANDLE

		_pLockDynData->EndRead();

		DynValue._iSmpNum = this->GetSafeWavePointNum(DynValue._iSmpNum);

		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(0).c_str(),DynValue.GetSpecTypeData(0));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(1).c_str(),DynValue.GetSpecTypeData(1));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(2).c_str(),DynValue.GetSpecTypeData(2));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(3).c_str(),DynValue.GetSpecTypeData(3));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(4).c_str(),DynValue.GetSpecTypeData(4));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(5).c_str(),DynValue.GetSpecTypeData(5));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(6).c_str(),DynValue.GetSpecTypeData(6));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(7).c_str(),DynValue.GetSpecTypeData(7));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(8).c_str(),DynValue.GetSpecTypeData(8));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(9).c_str(),DynValue.GetSpecTypeData(9));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(10).c_str(),DynValue.GetSpecTypeData(10));
		pDB_->Cmd(" [%s]=%.2f, ",pDynChannParam->GetSpecCharTypeField(11).c_str(),DynValue.GetSpecTypeData(11));
		pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),DynValue._iSmpFreq);

		pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),DynValue._iSmpNum);
		pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),DynValue._iRev);
		pDB_->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-2).c_str(),pDynChannParam->GetRegAlarmStatus());
		pDB_->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetChannInfo()._cAlarmTempID);
	}

	pDB_->Cmd("[%s] = %d",gc_cSynFlag,0);
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMillSecond_,gc_cTime,TimeConvertString(dtTime_),gc_cEventID,iEventID_);
	if(!pDB_->Exec())
	{
		return -1;
	}
	//更新波形数据
	pDB_->Cmd("SELECT 1");
	for (nloop=lnNum;nloop<lnDynChannNum;++nloop)
		pDB_->Cmd(",%s",_vDynWaveField[nloop]);
	pDB_->Cmd(" FROM [%s] ",_sAlarmDynDataTblName.c_str());
	pDB_->Cmd(" WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d ",gc_cMicroSecond,iMillSecond_,gc_cTime,TimeConvertString(dtTime_),gc_cEventID,iEventID_);
	if (pDB_->More(TRUE))
	{
		for (nloop = lnNum; nloop < lnDynChannNum; ++nloop)
		{
			bool bRtn=false;
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParamAlarm*)_vDynChann[nloop];
			int iSmpNums=this->GetSafeWavePointNum(pDynChannParam->GetCharValue()._iSmpNum);
			vector<BYTE> lvWave;
			CWaveHead lsHead = pDynChannParam->WaveHead();
			int lnTotalSize = this->WaveAddHead(pDynChannParam->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

			bRtn=pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),&lvWave.front(),lnTotalSize);
			pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pDynChannParam->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			END_ERROR_HANDLE
			_pLockDynData->EndRead();
		}
	}

	
	return 1;
}

void CAlarmPlantInfo::NewAlarmAddLog( bool bAddLog )
{
	_pLockVibData->BeginWrite();
	this->m_bNewAlarmAddLog = bAddLog;
	_pLockVibData->EndWrite();

}

bool CAlarmPlantInfo::NewAlarmAddLog()
{
	bool lbNewAlarmAddLog = false;
	_pLockVibData->BeginRead();
	lbNewAlarmAddLog = this->m_bNewAlarmAddLog;
	_pLockVibData->EndRead();
	return lbNewAlarmAddLog;
}

bool CAlarmPlantInfo::IsRecordExists( IDBInterFace *pDB_,string sTableName_,int iEventID_,CBHDateTime dtTime_,int iMillsecond )
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("select top 1 %s from %s where %s = %d and %s = '%s' and %s = %d",\
		gc_cTime,sTableName_.c_str(),gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cMicroSecond,iMillsecond);
	if(pDB_->More())
	{
		return true;
	}
	return false;
}

int CAlarmPlantInfo::InsertVibAlarmRecordFromTrendData( IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_ )
{
	/*报警数据缺失前面部分测点补齐
	 1. 查找报警表中当前报警ID是否有记录
	 2. 有记录则将距离当前记录时间最近的一条记录数据拷贝到当前记录
	 3. 没有则查询秒表距离当前记录时间前后的最近一条记录
	 4. 如果实时数据的时间距离当前记录的时间差小于趋势表距离当前记录最近记录的时间差，保存实时记录
	 5. 如果实时数据的时间距离当前记录的时间大于秒表表距离当前记录最近记录的时间差，将秒表记录拷贝到报警表
	*/
	int lnVibChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTimeFormer = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltTrendDataTimeLater = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltAlarmDataTimeFormer = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltAlarmDataTimeLater = CBHDateTime(2000,1,1,1,1,1,1);

	_pLockVibData->BeginRead();
	lnVibChannNum = this->GetVibChannNum();
	ltUpdateTime = this->_timeVibDataUpdate;
	_pLockVibData->EndRead();

	if(iStartIndex_ <= 0 || (iLen_ + iStartIndex_ > lnVibChannNum) || iLen_ <= 0)
	{
		return 1;
	}


	if(this->IsRecordExists(pDB_,_sAlarmVibDataTblName,iEventID_,dtTime_,iMillSecond_))
	{
		return 1;
	}

	
	string lstrSourceTable;

	CBHDateTime ltSourceTime;

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int lnNum = iStartIndex_ + iLen_;
	int nloop = 0;

	bool lbFindRecord = false;

	float pfRealRev[5];
	int iSmpNums = 0, iSmpFreq = 0;
	GetRealSmpInfo( iSmpNums, iSmpFreq, true);
	GetRealRev( pfRealRev, 5);

	CVibChannParamAlarm *pVibChannParam = NULL;
	S_VibCharValue VibValue;

	//1.
	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d AND [%s] >= '%s' ORDER BY [%s] ASC",
		gc_cTime,_sAlarmVibDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTimeLater = pDB_->GetTime(gc_cTime);
		lbFindRecord = true;
	}

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d  AND [%s] <= '%s' ORDER BY [%s] DESC",
		gc_cTime,_sAlarmVibDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltAlarmDataTimeFormer = pDB_->GetTime(gc_cTime);
		lbFindRecord = true;
	}

	
	if(!lbFindRecord)
	{
		pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] <= '%s' ORDER BY [%s] DESC",
			gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			ltTrendDataTimeFormer = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] >= '%s' ORDER BY [%s] ASC",
			gc_cTime,_sVibTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			ltTrendDataTimeLater = pDB_->GetTime(gc_cTime);
		}

	}

	if(g_bIsDiskFull)
	{
		if(!this->DeleteVibTrendRecord(pDB_,0))
		{
			this->DeleteVibAlarmRecord(pDB_,iEventID_,0);
		}
	}

	//2.
	pDB_->Cmd("INSERT INTO [%s]([%s],",_sAlarmVibDataTblName.c_str(),gc_cEventID);

	for(nloop = 0; nloop < lnVibChannNum; ++nloop)
	{
		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE
		pVibChannParam=(CVibChannParamAlarm*)(_vVibChann[nloop]);
		END_ERROR_HANDLE
		_pLockVibData->EndRead();

		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-2).c_str());
		pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		if(lbFindRecord || (!lbFindRecord &&!(abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeFormer - dtTime_)
			&& abs(ltUpdateTime - dtTime_) < abs(ltAlarmDataTimeLater - dtTime_))) )
		{
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}
		
	}
	pDB_->Cmd("[%s],",gc_cTime);
	pDB_->Cmd("[%s],",gc_cMicroSecond);
	pDB_->Cmd("[%s],",gc_cSampleNum);
	pDB_->Cmd("[%s],",gc_cSampleFreq);
	pDB_->Cmd("[%s],",gc_cRev);
	pDB_->Cmd("[%s],",gc_cRev2);
	pDB_->Cmd("[%s],",gc_cRev3);
	pDB_->Cmd("[%s],",gc_cRev4);
	pDB_->Cmd("[%s],",gc_cRev5);
	pDB_->Cmd("[%s])",gc_cSynFlag);

	if(lbFindRecord)
	{
		lstrSourceTable = _sAlarmVibDataTblName;
		if(abs(dtTime_ - ltAlarmDataTimeFormer) < abs(dtTime_ - ltAlarmDataTimeLater))
		{
			ltSourceTime = ltAlarmDataTimeFormer;
		}
		else
		{
			ltSourceTime = ltAlarmDataTimeLater;
		}

		pDB_->Cmd(" SELECT %d,",iEventID_);
		for(nloop = 0; nloop < lnVibChannNum; ++nloop)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParamAlarm*)(_vVibChann[nloop]);
			END_ERROR_HANDLE
			_pLockVibData->EndRead();

			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" %d,",0);
			pDB_->Cmd(" '%s',",pVibChannParam->GetChannInfo()._cAlarmTempID);
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str());
		}

		pDB_->Cmd(" '%s',",dtTime_.Format());
		pDB_->Cmd(" %d,",iMillSecond_);
		pDB_->Cmd(" [%s],",gc_cSampleNum);
		pDB_->Cmd(" [%s],",gc_cSampleFreq);
		pDB_->Cmd(" [%s],",gc_cRev);
		pDB_->Cmd(" [%s],",gc_cRev2);
		pDB_->Cmd(" [%s],",gc_cRev3);
		pDB_->Cmd(" [%s],",gc_cRev4);
		pDB_->Cmd(" [%s],",gc_cRev5);
		pDB_->Cmd(" %d",0);
		pDB_->Cmd(" FROM [%s] WHERE [%s] = '%s'",lstrSourceTable.c_str(),gc_cTime,ltSourceTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}
	}
	//3
	else
	{
		//4
		if(abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeFormer - dtTime_)
			&& abs(ltUpdateTime - dtTime_) < abs(ltAlarmDataTimeLater - dtTime_))
		{
			S_VibCharValue vibCharValue;

			pDB_->Cmd(" VALUES(%d,",iEventID_);
			for(nloop = 0; nloop < lnVibChannNum; ++nloop)
			{
				_pLockVibData->BeginRead();
				BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParamAlarm *)_vVibChann[nloop];
				vibCharValue = pVibChannParam->GetCharValue();
				END_ERROR_HANDLE
				_pLockVibData->EndRead();

				pDB_->Cmd(" %.2f,",vibCharValue._fCharAll);
				pDB_->Cmd(" %.2f,",vibCharValue._fCharOne);
				pDB_->Cmd(" %.2f,",vibCharValue._fCharTwo);
				pDB_->Cmd(" %.2f,",vibCharValue._fCharThree);
				pDB_->Cmd(" %.2f,",vibCharValue._fCharHalf);
				pDB_->Cmd(" %.2f,",vibCharValue._fCharRes);
				pDB_->Cmd(" %.2f,",vibCharValue._fDC);
				pDB_->Cmd(" %.2f,",vibCharValue._fGap);
				pDB_->Cmd(" %.2f,",vibCharValue._fPhaseOne);
				pDB_->Cmd(" %.2f,",vibCharValue._fPhaseTwo);
				pDB_->Cmd(" %.2f,",vibCharValue._fPhaseThree);
				pDB_->Cmd(" %.2f,",vibCharValue._fPhaseHalf);
				pDB_->Cmd(" %d,",vibCharValue._iSmpFreq);
				pDB_->Cmd(" %d,",vibCharValue._iSmpNum);
				pDB_->Cmd(" %d,",vibCharValue._iRev);
				pDB_->Cmd(" %d,",0);
				pDB_->Cmd(" '%s',",pVibChannParam->GetChannInfo()._cAlarmTempID);
			}

			pDB_->Cmd(" '%s',",dtTime_.Format());
			pDB_->Cmd(" %d,",iMillSecond_);
			pDB_->Cmd(" %d,",iSmpNums);
			pDB_->Cmd(" %d,",iSmpFreq);

			pDB_->Cmd(" %d,",pfRealRev[0]);
			pDB_->Cmd(" %d,",pfRealRev[1]);
			pDB_->Cmd(" %d,",pfRealRev[2]);
			pDB_->Cmd(" %d,",pfRealRev[3]);
			pDB_->Cmd(" %d,",pfRealRev[4]);
			pDB_->Cmd(" %d)",0);

			if(!pDB_->Exec())
			{
				return -1;
			}

			pDB_->ClearSql();
			pDB_->Cmd("SELECT [%s],%s FROM [%s] ",gc_cTime,_sSelWaveField.c_str(),_sAlarmVibDataTblName.c_str());
			pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cMicroSecond,iMillSecond_);

			if (pDB_->More(TRUE))
			{
				for (nloop = 0; nloop < lnVibChannNum; ++nloop)
				{
					pVibChannParam = (CVibChannParamAlarm *)_vVibChann[nloop];
					int iSmpNums=this->GetSafeWavePointNum(pVibChannParam->GetCharValue()._iSmpNum);

					vector<BYTE> lvWave;
					CWaveHead lsHead = pVibChannParam->WaveHead();
					int lnTotalSize = this->WaveAddHead(pVibChannParam->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),&lvWave.front(),lnTotalSize);
					pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pVibChannParam->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

				}
			}
		}
		//5
		else
		{

			lstrSourceTable = _sVibTrendSecTblName;

			if(abs(ltTrendDataTimeFormer - dtTime_) < abs(ltTrendDataTimeLater - dtTime_))
			{
				ltSourceTime = ltAlarmDataTimeFormer;
			}
			else
			{
				ltSourceTime = ltTrendDataTimeLater;
			}
			
			pDB_->Cmd(" SELECT %d,",iEventID_);
			for(nloop = 0; nloop < lnVibChannNum; ++nloop)
			{
				_pLockVibData->BeginRead();
				BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParamAlarm*)(_vVibChann[nloop]);
				END_ERROR_HANDLE
				_pLockVibData->EndRead();

				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-3).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-4).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-5).c_str());
				pDB_->Cmd(" %d,",0);
				pDB_->Cmd(" '%s',",pVibChannParam->GetChannInfo()._cAlarmTempID);
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" [%s],",pVibChannParam->GetSpecCharTypeField(-7).c_str());
			}

			pDB_->Cmd(" '%s',",dtTime_.Format());
			pDB_->Cmd(" %d,",iMillSecond_);
			pDB_->Cmd(" [%s],",gc_cSampleNum);
			pDB_->Cmd(" [%s],",gc_cSampleFreq);
			pDB_->Cmd(" [%s],",gc_cRev);
			pDB_->Cmd(" [%s],",gc_cRev2);
			pDB_->Cmd(" [%s],",gc_cRev3);
			pDB_->Cmd(" [%s],",gc_cRev4);
			pDB_->Cmd(" [%s],",gc_cRev5);
			pDB_->Cmd(" %d",0);
			pDB_->Cmd(" FROM [%s] WHERE [%s] = '%s'",lstrSourceTable.c_str(),gc_cTime,ltSourceTime.Format());
			if(!pDB_->Exec())
			{
				return -1;
			}
		}
	}

	return 1;
}

int CAlarmPlantInfo::InsertDynAlarmRecordFromTrendData( IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_ )
{
	
	int lnDynChannNum = 0;
	CBHDateTime ltUpdateTime = CBHDateTime::GetCurrentTimeEx();
	CBHDateTime ltTrendDataTimeFormer = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltTrendDataTimeLater = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltAlarmDataTimeFormer = CBHDateTime(2000,1,1,1,1,1,1);
	CBHDateTime ltAlarmDataTimeLater = CBHDateTime(2000,1,1,1,1,1,1);

	_pLockDynData->BeginRead();
	lnDynChannNum = this->GetDynChannNum();
	ltUpdateTime = this->_timeDynDataUpdate;
	_pLockDynData->EndRead();

	if(iStartIndex_ <= 0 || (iLen_ + iStartIndex_ > lnDynChannNum) || iLen_ <=0)
	{
		return 1;
	}

	if(this->IsRecordExists(pDB_,_sAlarmDynDataTblName,iEventID_,dtTime_,iMillSecond_))
	{
		return 1;
	}

	string lstrSourceTable;

	CBHDateTime ltSourceTime;

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	int lnNum = iStartIndex_ + iLen_;
	int nloop = 0;

	bool lbFindRecord = false;

	float pfRealRev[5];
	int iSmpNums = 0, iSmpFreq = 0;
	GetRealSmpInfo( iSmpNums, iSmpFreq, true);
	GetRealRev( pfRealRev, 5);

	CDynChannParamAlarm *pDynChannParam = NULL;
	S_DynCharValue DynValue;

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d AND [%s] >= '%s' ORDER BY [%s] ASC",
		gc_cTime,_sAlarmDynDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltTrendDataTimeLater = pDB_->GetTime(gc_cTime);
		lbFindRecord = true;
	}

	pDB_->Cmd("SELECT TOP 1 [%s] FROM %s WHERE [%s] = %d  AND [%s] <= '%s' ORDER BY [%s] DESC",
		gc_cTime,_sAlarmDynDataTblName.c_str(),gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cTime);
	if(pDB_->More())
	{
		ltAlarmDataTimeFormer = pDB_->GetTime(gc_cTime);
		lbFindRecord = true;
	}

	if(!lbFindRecord)
	{
		pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] <= '%s' ORDER BY [%s] DESC",
			gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			ltTrendDataTimeFormer = pDB_->GetTime(gc_cTime);
		}

		pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] >= '%s' ORDER BY [%s] ASC",
			gc_cTime,_sDynTrendSecTblName.c_str(),gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			ltTrendDataTimeLater = pDB_->GetTime(gc_cTime);
		}
	}
	

	pDB_->Cmd("INSERT INTO [%s]([%s],",_sAlarmDynDataTblName.c_str(),gc_cEventID);

	for(nloop = 0; nloop < lnDynChannNum; ++nloop)
	{
		_pLockDynData->BeginRead();
		BEGIN_ERROR_HANDLE
		pDynChannParam=(CDynChannParamAlarm*)(_vDynChann[nloop]);
		END_ERROR_HANDLE
		_pLockDynData->EndRead();

		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(0).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(1).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(2).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(3).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(4).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(5).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(6).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(7).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(8).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(9).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(10).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(11).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-2).c_str());
		pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-6).c_str());
		if(lbFindRecord || (!lbFindRecord && !(abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeFormer - dtTime_)
			&& abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeLater - dtTime_))))
		{
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}
		
	}
	pDB_->Cmd(" [%s],",gc_cTime);
	pDB_->Cmd(" [%s],",gc_cMicroSecond);
	pDB_->Cmd(" [%s],",gc_cSampleNum);
	pDB_->Cmd(" [%s],",gc_cSampleFreq);
	pDB_->Cmd(" [%s],",gc_cRev);
	pDB_->Cmd(" [%s],",gc_cRev2);
	pDB_->Cmd(" [%s],",gc_cRev3);
	pDB_->Cmd(" [%s],",gc_cRev4);
	pDB_->Cmd(" [%s],",gc_cRev5);
	pDB_->Cmd(" [%s])",gc_cSynFlag);

	if(lbFindRecord)
	{
		lstrSourceTable = _sAlarmDynDataTblName;
		if(abs(dtTime_ - ltAlarmDataTimeFormer) < abs(dtTime_ - ltAlarmDataTimeLater))
		{
			ltSourceTime = ltAlarmDataTimeFormer;
		}
		else
		{
			ltSourceTime = ltAlarmDataTimeLater;
		}

		pDB_->Cmd(" SELECT %d,",iEventID_);
		for(nloop = 0; nloop < lnDynChannNum; ++nloop)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParamAlarm*)(_vDynChann[nloop]);
			END_ERROR_HANDLE
			_pLockDynData->EndRead();

			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(0).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(1).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(2).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(3).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(4).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(5).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(6).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(7).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(8).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(9).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(10).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(11).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			pDB_->Cmd(" %d,",0);
			pDB_->Cmd(" '%s',",pDynChannParam->GetChannInfo()._cAlarmTempID);
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str());
			pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str());
		}

		pDB_->Cmd(" '%s',",dtTime_.Format());
		pDB_->Cmd(" %d,",iMillSecond_);
		pDB_->Cmd(" [%s],",gc_cSampleNum);
		pDB_->Cmd(" [%s],",gc_cSampleFreq);
		pDB_->Cmd(" [%s],",gc_cRev);
		pDB_->Cmd(" [%s],",gc_cRev2);
		pDB_->Cmd(" [%s],",gc_cRev3);
		pDB_->Cmd(" [%s],",gc_cRev4);
		pDB_->Cmd(" [%s],",gc_cRev5);
		pDB_->Cmd(" %d",0);
		pDB_->Cmd(" FROM [%s] WHERE [%s] = '%s'",lstrSourceTable.c_str(),gc_cTime,ltSourceTime.Format());
		if(pDB_->Exec())
		{
			return 1;
		}
	}
	else
	{
		
		if(abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeFormer - dtTime_)
			&& abs(ltUpdateTime - dtTime_) < abs(ltTrendDataTimeLater - dtTime_))
		{
			S_DynCharValue dynCharValue;

			pDB_->Cmd(" VALUES(%d,",iEventID_);
			for(nloop = 0; nloop < lnDynChannNum; ++nloop)
			{
				_pLockDynData->BeginRead();
				BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParamAlarm *)_vDynChann[nloop];
				dynCharValue = pDynChannParam->GetCharValue();
				END_ERROR_HANDLE
				_pLockDynData->EndRead();

				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(0));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(1));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(2));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(3));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(4));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(5));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(6));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(7));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(8));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(9));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(10));
				pDB_->Cmd(" %.2f,",dynCharValue.GetSpecTypeData(11));
				pDB_->Cmd(" %d,",dynCharValue._iSmpFreq);
				pDB_->Cmd(" %d,",dynCharValue._iSmpNum);
				pDB_->Cmd(" %d,",dynCharValue._iRev);
				pDB_->Cmd(" %d,",0);
				pDB_->Cmd(" '%s',",pDynChannParam->GetChannInfo()._cAlarmTempID);
			}

			pDB_->Cmd(" '%s',",dtTime_.Format());
			pDB_->Cmd(" %d,",iMillSecond_);
			pDB_->Cmd(" %d,",iSmpNums);
			pDB_->Cmd(" %d,",iSmpFreq);

			pDB_->Cmd(" %d,",pfRealRev[0]);
			pDB_->Cmd(" %d,",pfRealRev[1]);
			pDB_->Cmd(" %d,",pfRealRev[2]);
			pDB_->Cmd(" %d,",pfRealRev[3]);
			pDB_->Cmd(" %d,",pfRealRev[4]);
			pDB_->Cmd(" %d)",0);

			if(!pDB_->Exec())
			{
				return -1;
			}

			pDB_->ClearSql();
			pDB_->Cmd("SELECT [%s],%s FROM [%s] ",gc_cTime,_sDynSelWaveField.c_str(),_sAlarmDynDataTblName.c_str());
			pDB_->Cmd("WHERE [%s]=%d AND [%s]='%s' AND [%s]=%d",gc_cEventID,iEventID_,gc_cTime,dtTime_.Format(),gc_cMicroSecond,iMillSecond_);

			if (pDB_->More(TRUE))
			{
				for (nloop = 0; nloop < lnDynChannNum; ++nloop)
				{
					pDynChannParam = (CDynChannParamAlarm *)_vDynChann[nloop];
					int iSmpNums=this->GetSafeWavePointNum(pDynChannParam->GetCharValue()._iSmpNum);

					vector<BYTE> lvWave;
					CWaveHead lsHead = pDynChannParam->WaveHead();
					int lnTotalSize = this->WaveAddHead(pDynChannParam->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

					pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),&lvWave.front(),lnTotalSize);

					pDB_->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pDynChannParam->GetCustomParamData(),GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));

				}
			}
		}
		else
		{
			
			lstrSourceTable = _sDynTrendSecTblName;

			if(abs(ltTrendDataTimeFormer - dtTime_) < abs(ltTrendDataTimeLater - dtTime_))
			{
				ltSourceTime = ltTrendDataTimeFormer;
			}
			else
			{
				ltSourceTime = ltTrendDataTimeLater;
			}

			pDB_->Cmd(" SELECT %d,",iEventID_);
			for(nloop = 0; nloop < lnDynChannNum; ++nloop)
			{
				_pLockDynData->BeginRead();
				BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParamAlarm*)(_vDynChann[nloop]);
				END_ERROR_HANDLE
				_pLockDynData->EndRead();

				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(0).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(1).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(2).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(3).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(4).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(5).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(6).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(7).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(8).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(9).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(10).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(11).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-3).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-4).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-5).c_str());
				pDB_->Cmd(" %d,",0);
				pDB_->Cmd(" '%s',",pDynChannParam->GetChannInfo()._cAlarmTempID);
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-1).c_str());
				pDB_->Cmd(" [%s],",pDynChannParam->GetSpecCharTypeField(-7).c_str());
			}

			pDB_->Cmd(" '%s',",dtTime_.Format());
			pDB_->Cmd(" %d,",iMillSecond_);
			pDB_->Cmd(" [%s],",gc_cSampleNum);
			pDB_->Cmd(" [%s],",gc_cSampleFreq);
			pDB_->Cmd(" [%s],",gc_cRev);
			pDB_->Cmd(" [%s],",gc_cRev2);
			pDB_->Cmd(" [%s],",gc_cRev3);
			pDB_->Cmd(" [%s],",gc_cRev4);
			pDB_->Cmd(" [%s],",gc_cRev5);
			pDB_->Cmd(" %d",0);
			pDB_->Cmd(" FROM [%s] WHERE [%s] = '%s'",_sDynTrendSecTblName.c_str(),gc_cTime,ltSourceTime.Format());
			if(!pDB_->Exec())
			{
				return -1;
			}
		}
		
	}

	return 1;

}

void CAlarmPlantInfo::IncVibFastChangeAlarmAddLogTime( const CBHDateTime& dtTime_ )
{
	
	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
		map<CBHDateTime,CBHDateTime>::iterator it;
	if(this->m_mtVibFastChangeAlarmLogTime.find(dtTime_) == this->m_mtVibFastChangeAlarmLogTime.end())
	{
		if(this->m_mtVibFastChangeAlarmLogTime.size() > D_FAST_CHANGE_LOG_TIME_COUNT)
		{
			this->m_mtVibFastChangeAlarmLogTime.erase(this->m_mtVibFastChangeAlarmLogTime.begin());
		}
		this->m_mtVibFastChangeAlarmLogTime.insert(make_pair(dtTime_,dtTime_));
	}

	END_ERROR_HANDLE
	_pLockVibData->EndWrite();
}

void CAlarmPlantInfo::DescVibFastChangeAlarmAddLogTime( const CBHDateTime& dtTime_ )
{
	int lnCount = 0;

	_pLockVibData->BeginWrite();
	BEGIN_ERROR_HANDLE
	map<CBHDateTime,CBHDateTime>::iterator it = this->m_mtVibFastChangeAlarmLogTime.find(dtTime_);

	if(it != this->m_mtVibFastChangeAlarmLogTime.end())
	{
		this->m_mtVibFastChangeAlarmLogTime.erase(it);
	}

	lnCount = this->m_mtVibFastChangeAlarmLogTime.size();
	END_ERROR_HANDLE
	_pLockVibData->EndWrite();

	zdlTraceLine(STR(_T("剩余振动报警日志时间数 :%d"),lnCount));

}

void CAlarmPlantInfo::IncDynFastChangeAlarmAddLogTime( const CBHDateTime& dtTime_ )
{
	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE
	if(this->m_mtDynFastChangeAlarmLogTime.find(dtTime_) == this->m_mtDynFastChangeAlarmLogTime.end())
	{
		if(this->m_mtDynFastChangeAlarmLogTime.size() > D_FAST_CHANGE_LOG_TIME_COUNT)
		{
			this->m_mtDynFastChangeAlarmLogTime.erase(this->m_mtDynFastChangeAlarmLogTime.begin());
		}
		this->m_mtDynFastChangeAlarmLogTime.insert(make_pair(dtTime_,dtTime_));
	}
	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

}

void CAlarmPlantInfo::DescDynFastChangeAlarmAddLogTime( const CBHDateTime& dtTime_ )
{
	int lnCount = 0;

	_pLockDynData->BeginWrite();
	BEGIN_ERROR_HANDLE
	map<CBHDateTime,CBHDateTime>::iterator it = this->m_mtDynFastChangeAlarmLogTime.find(dtTime_);

	if(it != this->m_mtDynFastChangeAlarmLogTime.end())
	{
		this->m_mtDynFastChangeAlarmLogTime.erase(it);
	}

	lnCount = this->m_mtDynFastChangeAlarmLogTime.size();
	END_ERROR_HANDLE
	_pLockDynData->EndWrite();

	zdlTraceLine(STR(_T("剩余动态报警日志时间数 :%d"),lnCount));
}

bool CAlarmPlantInfo::VibAlarmAddLogTimeExists( const CBHDateTime& dtTime_ )
{
	bool lbExists = false;

	CString lstrTime = dtTime_.Format();

	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(this->m_mtVibFastChangeAlarmLogTime.find(dtTime_) != this->m_mtVibFastChangeAlarmLogTime.end())
	{
		lbExists = true;
	}
	END_ERROR_HANDLE
	_pLockVibData->EndRead();
	return lbExists;
}

bool CAlarmPlantInfo::DynAlarmAddLogTimeExists( const CBHDateTime& dtTime_ )
{
	bool lbExists = false;
	_pLockDynData->BeginRead();
	BEGIN_ERROR_HANDLE
	if(this->m_mtDynFastChangeAlarmLogTime.find(dtTime_) != this->m_mtDynFastChangeAlarmLogTime.end())
	{
		lbExists = true;
	}
	END_ERROR_HANDLE
	_pLockDynData->EndRead();
	return lbExists;
}

int CAlarmPlantInfo::QueryVibAlarmEventIDByTime(IDBInterFace *pDB_, const CBHDateTime& dtTime_)
{
	int lnEventID = -1;

	if(NULL == pDB_)
	{
		return lnEventID;
	}
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = '%s' and [%s] = %d",
		gc_cEventID,_sAlarmLogTblName.c_str(),gc_cStart_T,dtTime_.Format(),gc_cChannType,GE_VIBCHANN);
	if(pDB_->More())
	{
		lnEventID = pDB_->GetLong(gc_cEventID);
	}

	return lnEventID;
}

int CAlarmPlantInfo::QueryDynAlarmEventIDByTime(IDBInterFace *pDB_, const CBHDateTime& dtTime_ )
{
	int lnEventID = -1;

	if(NULL == pDB_)
	{
		return lnEventID;
	}
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = '%s' and [%s] in(%d,%d,%d)",
		gc_cEventID,_sAlarmLogTblName.c_str(),gc_cStart_T,dtTime_.Format(),gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	if(pDB_->More())
	{
		lnEventID = pDB_->GetLong(gc_cEventID);
	}

	return lnEventID;
}

int CAlarmPlantInfo::UpdateAlarmSyncFlag( IDBInterFace *pDB_,const int& iEventID_,const CBHDateTime& dtTime_ )
{
	if(NULL == pDB_)
	{
		return 0;
	}

	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	pDB_->Cmd("SELECT TOP 1 [%s] FROM [%s] WHERE [%s] = %d AND [%s] = '%s'",
		gc_cSynFlag,_sAlarmLogTblName.c_str(),gc_cEventID,iEventID_,gc_cStart_T,dtTime_.Format());
	if(pDB_->More())
	{
		if(pDB_->GetLong(gc_cSynFlag) > 0)
		{
			pDB_->Cmd("UPDATE [%s] SET [%s] = %d WHERE [%s] = %d AND [%s] = '%s'",
				_sAlarmLogTblName.c_str(),gc_cSynFlag,0,gc_cEventID,iEventID_,gc_cStart_T,dtTime_.Format());
			if(pDB_->More())
			{
				return 1;
			}
		}
	}

	return 0;
}

CDieselPlant * CAlarmPlantInfo::GetDieselPlantByType(E_MACHINE_TYPE& eMachineType_)
{
	//根据配置文件生成设备类型进行报警判断

	HMODULE hModule = this->m_hLibHandle;

	CString lstrDieselPlantType = CConfigHelper::GetDieselPlantType(this->GetPlantNo().c_str());

	int lnDieselPlantType = -1;

	if(lstrDieselPlantType.MakeUpper().Find(_T("WC")) >= 0)
	{
		lnDieselPlantType = MACHINE_TYPE_WC;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("16PA6STC")) >= 0)
	{
		lnDieselPlantType = MACHINE_TYPE_16PA6;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("703"))>=0)
	{
		lnDieselPlantType = MACHINE_TYPE_620;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("956"))>=0)
	{
		lnDieselPlantType = MACHINE_TYPE_956;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("GASTURBINE")) >= 0)
	{
		lnDieselPlantType = MACHINE_TYPE_GASTURBINE;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("PUMP")) >= 0)
	{
		lnDieselPlantType = MACHINE_TYPE_PUMP;
	}
	else if(lstrDieselPlantType.MakeUpper().Find(_T("ROTATE")) >= 0)
	{
		lnDieselPlantType = MACHINE_TYPE_ROTATE;
	}
	else if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_KEYPUMP)
	{
		lnDieselPlantType = MACHINE_TYPE_PUMP;
	}
	else if(this->GetPlantInfo()._iMachineType == GE_MACHINETYPE_COMPR)
	{
		lnDieselPlantType = MACHINE_TYPE_ROTATE;
	}

	eMachineType_ = (E_MACHINE_TYPE)lnDieselPlantType;

	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);

	typedef CDieselPlant* (*PFNCreateDiesel)();

	CDieselPlant *pDieselPlant = NULL;

	switch(lnDieselPlantType)
	{
	case MACHINE_TYPE_WC:
		strcat(cAppLocation,"\\DieselAlarmWC.dll");

		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}


		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectofWC"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_16PA6:
		strcat(cAppLocation,"\\DieselAlarm16PA6STC.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectof16PA6STC"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_620:
		strcat(cAppLocation,"\\DieselAlarm703.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectof703"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_956:
		strcat(cAppLocation,"\\DieselAlarm956.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectof956"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_GASTURBINE:
		strcat(cAppLocation,"\\GasTurbineAlarm.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectofGasTurbine"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_PUMP:
		strcat(cAppLocation,"\\PumpAlarm.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectofPump"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_ROTATE:
		strcat(cAppLocation,"\\CompressorAlarm.dll");
		if(NULL == hModule)
		{
			hModule = ::LoadLibrary(cAppLocation);
		}

		if(hModule)
		{
			PFNCreateDiesel pfnCreateDiesel = (PFNCreateDiesel)GetProcAddress(hModule, TEXT("CreateObjectofCompressor"));

			pDieselPlant = (pfnCreateDiesel)();
		}
		break;
	case MACHINE_TYPE_RC:
		break;
	case MACHINE_TYPE_LIFT:
		break;
	default:
		break;

	}
	
	this->m_hLibHandle = hModule;
	return pDieselPlant;
}


float CAlarmPlantInfo::GetPlantPower()
{
	float lfPower = 0;
	_pLockVibData->BeginRead();
	lfPower = this->m_fPower;
	_pLockVibData->EndRead();
	return lfPower;
}

void CAlarmPlantInfo::SetPlantPower( float afPower_ )
{
	_pLockVibData->BeginWrite();
	this->m_fPower = afPower_;
	_pLockVibData->EndWrite();
}

int CAlarmPlantInfo::GetLowPressRev()
{
	int lnRev = 0;
	_pLockProcData->BeginRead();
	lnRev = this->m_nLowPressRev;
	_pLockProcData->EndRead();
	return lnRev;
}

void CAlarmPlantInfo::SetLowPressRev( int nRev_ )
{
	_pLockProcData->BeginWrite();
	this->m_nLowPressRev = nRev_;
	_pLockProcData->EndWrite();
}

int CAlarmPlantInfo::GetHighPressRev()
{
	int lnRev = 0;
	_pLockProcData->BeginRead();
	lnRev = this->m_nHighPressRev;
	_pLockProcData->EndRead();
	return lnRev;
}

void CAlarmPlantInfo::SetHighPressRev( int nRev_ )
{
	_pLockProcData->BeginWrite();
	this->m_nHighPressRev = nRev_;
	_pLockProcData->EndWrite();
}




int CAlarmPlantInfo::JudgeAlarmByOtherPlant( IDBInterFace *pDB_,CVibChannParamAlarm *pVibChannParamAlarm_ )
{
	vector<void*> lvAllPlant;

	float lfCurrCharValue = 0;
	CBHDateTime ltCharValueUpdateTime;

	int lnResult = CHANN_ALARM_STATUS_NORMAL;

	int iFlag = -1;
	if(GetAllAlarmPlantInfo(lvAllPlant,this->GetPlantInfo()._cManufacturer) > 0)
	{

		map<CBHDateTime,float> lmOtherCharValueUpdateTime;

		for(size_t t = 0; t < lvAllPlant.size(); ++t)
		{
			CAlarmPlantInfo *pAlarmPlantInfo = (CAlarmPlantInfo*)lvAllPlant[t];

			lfCurrCharValue = pVibChannParamAlarm_->GetCharValue()._fCharAll;

			ltCharValueUpdateTime = this->_timeVibDataUpdate;

			if(strcmp(pAlarmPlantInfo->GetPlantInfo()._cPlantNo,this->GetPlantInfo()._cPlantNo) == 0)
			{
				continue;
			}

			for (V_PCHANNINFO::iterator itPlant=pAlarmPlantInfo->_vVibChann.begin();itPlant!=pAlarmPlantInfo->_vVibChann.end();++itPlant)
			{

				CVibChannParamAlarm* lpPlantVib = (CVibChannParamAlarm*)(*itPlant);

				bool lbRun = pAlarmPlantInfo->GetVibJudgeRunStatus()||pAlarmPlantInfo->GetProcJudgeRunStatus();
				if (!::IsPlantRunning(*pAlarmPlantInfo,lbRun)) //停车处理
				{
					continue;
				}

				if(strcmp(lpPlantVib->GetChannInfo()._cChannNo,pVibChannParamAlarm_->GetChannInfo()._cChannNo) != 0)
				{
					continue;
				}
				else
				{

					float lfLastAlarmValue  = lpPlantVib->LastAlarmValueEx();
					float lfCharValue = lpPlantVib->GetCharValue()._fCharAll;

					if(!IsFloatZero(lfLastAlarmValue - G_F_INIT_VALUE))
					{
						lfCharValue = (lfLastAlarmValue + lfCharValue)/2.0;	
					}

					CBHDateTime ltUpdateTime = pAlarmPlantInfo->_timeVibDataUpdate;

					ltUpdateTime.SetMillisecond(t);

					CString lstrDt = ltUpdateTime.Format();
					lmOtherCharValueUpdateTime.insert(make_pair(ltUpdateTime,lfCharValue));

				}

			}

		}

		lnResult = pVibChannParamAlarm_->JudgeAlarmByOtherPlant(pDB_,lfCurrCharValue,ltCharValueUpdateTime,lmOtherCharValueUpdateTime);

	}

	return lnResult;
}
