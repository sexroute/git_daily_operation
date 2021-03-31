#include "StdAfx.h"
#include ".\datahandle.h"

CDataHandle::CDataHandle(void)
{
	_iVibChannNum=0;
	_iProcChannNum=0;
	_iAxisDisplaceNum=0;
	_bProcDataUpdatedForAlarm=false;
	_bVibDataUpdatedForAlarm=false;
	_bDataUpdatedForHis=false;
	_iRev2=0;
	_iRev1=0;
	_lockDataProc=new CMutexRW;
	_lockDataVib=new CMutexRW;
	_lockDataRunState=new CMutexRW;
	_lockDataUpdatedAlarm=new CMutexRW;
	_lockDataUpdatedHis=new CMutexRW;
}

CDataHandle::~CDataHandle(void)
{
	delete _lockDataProc;
	delete _lockDataVib;
	delete _lockDataRunState;
	delete _lockDataUpdatedAlarm;
	delete _lockDataUpdatedHis;
}
// 初始化测点参数和每个测点对应的报警值
void CDataHandle::InitParam(float *ag_pfCharAll,
							float *ag_pfCharOne,
							float *ag_pfCharTwo,
							float *ag_pfCharThree,
							float *ag_pfCharHalf,
							float *pfCharRes_,
							float *pfPhaseOne_,
							float *pfPhaseTwo_,
							float *pfPhaseThree_,
							float *pfPhaseHalf_,
							float *ag_pfDC,
							float **ag_ppfWave,
							float *ag_pfProc,
							float *ag_pfAxisDisplace,
							int  *ag_pbVibChannelState,
							int  *ag_pbProcChannelState,
							int  *ag_pbAxisDisplaceChannelState,
							CSimADO *ag_pDB)
{
	_pfVibCharAll=ag_pfCharAll;
	_pfVibCharOne=ag_pfCharOne;
	_pfVibCharTwo=ag_pfCharTwo;
	_pfVibCharThree=ag_pfCharThree;
	_pfVibCharHalf=ag_pfCharHalf;
	_pfVibCharRes=pfCharRes_;
	_pfPhaseOne=pfPhaseOne_;
	_pfPhaseTwo=pfPhaseTwo_;
	_pfPhaseThree=pfPhaseThree_;
	_pfPhaseHalf=pfPhaseHalf_;
	_pfDC=ag_pfDC;
	_ppfWave=ag_ppfWave;
	_pfProcValue=ag_pfProc;
	_pfAxisDisplaceValue=ag_pfAxisDisplace;
	_pDB=ag_pDB;

	_pbProcChannelState=ag_pbProcChannelState;
	_pbVibChannelState=ag_pbVibChannelState;
	_pbAxisChannelState=ag_pbAxisDisplaceChannelState;

	AlarmInitIDAndRecordAllNum();
	TrendInitIDAndInterval();
}

// 设置装置号和设备号
void CDataHandle::SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iMaxSmpNum)
{
	_iSmpMaxNum=ag_iMaxSmpNum;
	_iSmpNum=0;
	_iSmpFreq=0;
	_strSet=ag_strSet;
	_strPlant=ag_strPlant;
	_strProcAlarmTable="ALARM_PROC_";
	_strProcAlarmTable.append(_strPlant);
	
	_strVibAlarmTable="ALARM_VIB_";
	_strVibAlarmTable.append(_strPlant);

	_strVibMonthTrendTable="TREND_VIBMONTH_"+ag_strPlant;
	_strVibDayTrendTable="TREND_VIBDAY_"+ag_strPlant;
	_strVibHourTrendTable="TREND_VIBHOUR_"+ag_strPlant;
	_strVibSecondTrendTable="TREND_VIBSECOND_"+ag_strPlant;

	_strProcSecondTrendTable="TREND_PROCSECOND_"+ag_strPlant;
	_strProcMonthTrendTable="TREND_PROCMONTH_"+ag_strPlant;
	_strProcDayTrendTable="TREND_PROCDAY_"+ag_strPlant;
	_strProcHourTrendTable="TREND_PROCHOUR_"+ag_strPlant;  
}

// 添加测点信息
void CDataHandle::AddVibChannelInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm,int ag_iofRev)
{
	S_VibInfo s_alarmpar;
	s_alarmpar.bHighAlarm=false;
	s_alarmpar.fLowAlarmValue=ag_fLowAlarmValue;
	s_alarmpar.fHighAlarmValue=ag_fHighAlarmValue;
	s_alarmpar.fBeforeValue=1;
	s_alarmpar.iContinueNum=0;
	s_alarmpar.iIntervalNum=0;
	s_alarmpar.strChann=ag_strChann;
	if (ag_iCheckAlarm==0) {
		s_alarmpar.bCheckAlarm=false;
	}
	else{
		s_alarmpar.bCheckAlarm=true;
	}
	s_alarmpar.iOfRev=ag_iofRev;
	s_alarmpar.strAll="ALL_";
	s_alarmpar.strAll.append(ag_strChann);

	s_alarmpar.strOne="ONE_";
	s_alarmpar.strOne.append(ag_strChann);

	s_alarmpar.strTwo="TWO_";
	s_alarmpar.strTwo.append(ag_strChann);

	s_alarmpar.strThree="THREE_";
	s_alarmpar.strThree.append(ag_strChann);

	s_alarmpar.strHalf="HALF_";
	s_alarmpar.strHalf.append(ag_strChann);

	s_alarmpar.strRes="RES_";
	s_alarmpar.strRes.append(ag_strChann);

	s_alarmpar.strPhaseOne="ONEPHASE_";
	s_alarmpar.strPhaseOne.append(ag_strChann);

	s_alarmpar.strPhaseTwo="TWOPHASE_";
	s_alarmpar.strPhaseTwo.append(ag_strChann);

	s_alarmpar.strPhaseThree="THREEPHASE_";
	s_alarmpar.strPhaseThree.append(ag_strChann);

	s_alarmpar.strPhaseHalf="HALFPHASE_";
	s_alarmpar.strPhaseHalf.append(ag_strChann);

	s_alarmpar.strDC="DC_";
	s_alarmpar.strDC.append(ag_strChann);
	
	s_alarmpar.strWave="WAVE_";
	s_alarmpar.strWave.append(ag_strChann);

	_vectorVibChann.push_back(s_alarmpar);
}

// 添加过程量和轴位移测点信息
void CDataHandle::AddProcAndAxisChannInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm,int ag_iChannType)
{
	S_ProcInfo s_alarmpar;
	s_alarmpar.bHighAlarm=false;
	s_alarmpar.iType=ag_iChannType;
	s_alarmpar.fLowAlarmValue=ag_fLowAlarmValue;
	s_alarmpar.fHighAlarmValue=ag_fHighAlarmValue;
	s_alarmpar.fBeforeValue=1;
	s_alarmpar.iContinueNum=0;
	s_alarmpar.iIntervalNum=0;
	s_alarmpar.strChannel=ag_strChann;
	if (ag_iCheckAlarm==0) {
		s_alarmpar.bCheckAlarm=false;
	}
	else{
		s_alarmpar.bCheckAlarm=true;
	}
	_vectorProcChann.push_back(s_alarmpar);
}

// 添加振动测点信息结束，在这个函数中构造数据库操作中使用的字段序列
void CDataHandle::AddVibChannInfoEnd(void)
{
	VECTOR_S_VIBINFO::iterator result;
	_strVibFieldSel=" ";
	_strVibFieldIns=" ";
	string stdot(",");
	_cvectorEndVib=_vectorVibChann.end();
	for (result=_vectorVibChann.begin();result!=_cvectorEndVib;result++)
	{
		_strVibFieldIns.append(result->strAll);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strOne);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strTwo);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strThree);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strHalf);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strRes);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strPhaseOne);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strPhaseTwo);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strPhaseThree);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strPhaseHalf);
		_strVibFieldIns.append(stdot);
		_strVibFieldIns.append(result->strDC);
		_strVibFieldIns.append(stdot);
			
		
		_strVibFieldSel.append(result->strWave);
		_strVibFieldSel.append(stdot);
	}
	_strVibFieldIns.append(gc_cSampleNum);
	_strVibFieldIns.append(stdot);
	_strVibFieldIns.append(gc_cSampleFreq);
	_strVibFieldIns.append(stdot);
	_strVibFieldIns.append(gc_cRev);
	_strVibFieldIns.append(stdot);
	_strVibFieldIns.append(gc_cRev2);
	_strVibFieldIns.append(stdot);
	_strVibFieldIns.append(gc_cTime);
	_strVibFieldSel.append(gc_cSampleNum);
	_strVibFieldSel.append(stdot);
	_strVibFieldSel.append(gc_cID);

    _iVibChannNum=_vectorVibChann.size();
}

// 添加过程量测点信息结束，在这个函数中构造数据库操作中使用的字段序列
void CDataHandle::AddProcChannInfoEnd(void)
{
	VECTOR_S_PROCINFO::iterator result;
	_strProcField=" ";
	
	_iProcChannNum=0;
	_iAxisDisplaceNum=0;

	_cvectorEndProc=_vectorProcChann.end();
	for (result=_vectorProcChann.begin();result!=_vectorProcChann.end();result++)
	{
		_strProcField.append("[");
		_strProcField.append(result->strChannel);
		_strProcField.append("],");
		
		if((result->iType)!=2)
			_iProcChannNum++;
		else
			_iAxisDisplaceNum++;
	}
	_strProcField+=gc_cTime;
}

void CDataHandle::JudgeVibratedAlarm(CCustomDB * ag_pDB)
{
	int index=0;
	VECTOR_S_VIBINFO::iterator result;

	for (result=_vectorVibChann.begin();result!=_vectorVibChann.end();result++)
	{
		if (result->bCheckAlarm)
		{
			if (_pfVibCharAll[index]>(_vectorVibChann[index].fHighAlarmValue))
			{
				if(JudgeVibAlarmHigh(index))
					SaveVibratedAlarmRecord(index,ag_pDB);
				_lockDataRunState->BeginWrite();
				_pbVibChannelState[index]=2;//set state of a vibrating channel is high alarm
				_lockDataRunState->EndWrite();
			}
			else if(_pfVibCharAll[index]>(_vectorVibChann[index].fLowAlarmValue))
			{
				if(JudgeVibAlarmLow(index))
					SaveVibratedAlarmRecord(index,ag_pDB);
				_lockDataRunState->BeginWrite();
				_pbVibChannelState[index]=1;//set state of a vibrating channel is low alarm
				_lockDataRunState->EndWrite();
			}
			else
			{
				_vectorVibChann[index].iContinueNum=0;
				_lockDataRunState->BeginWrite();			
				_pbVibChannelState[index]=0;//set state of a vibrating channel is normal
				_lockDataRunState->EndWrite();
			}
		}
		else
		{
			_vectorVibChann[index].iContinueNum=0;
			_lockDataRunState->BeginWrite();
			_pbVibChannelState[index]=0;//set state of a vibrating channel is normal
			_lockDataRunState->EndWrite();
		}
		index++;
	}
}
//包括轴位移的过程量
void CDataHandle::JudgeProcAlarm(CCustomDB * ag_pDB)
{
	int index=0,indexproc=0,indexaxis=0;
	VECTOR_S_PROCINFO::iterator result;

	for (result=_vectorProcChann.begin();result!=_vectorProcChann.end();result++)
	{
		if((result->iType)==GE_AXISCHANN)
		{  
			if (result->bCheckAlarm)
			{
				if (abs(_pfAxisDisplaceValue[indexaxis])>(_vectorProcChann[index].fHighAlarmValue))
				{
					if(JudgeProcAlarmHigh(index))
						SaveProcAlarmRecord(index,indexaxis,ag_pDB);
					_lockDataRunState->BeginWrite();
					_pbAxisChannelState[indexaxis]=2;//set state of a axis displace channel is high alarm
					_lockDataRunState->EndWrite();
				}
				else if(abs(_pfAxisDisplaceValue[indexaxis])>(_vectorProcChann[index].fLowAlarmValue))
				{
					if(JudgeProcAlarmLow(index))
						SaveProcAlarmRecord(index,indexaxis,ag_pDB);
					_lockDataRunState->BeginWrite();
					_pbAxisChannelState[indexaxis]=1;//set state of a axis displace channel is low alarm
					_lockDataRunState->EndWrite();
				}
				else
				{
					_vectorProcChann[index].iContinueNum=0;
					_lockDataRunState->BeginWrite();
					_pbAxisChannelState[indexaxis]=0;//set state of a axis displace channel is normal
					_lockDataRunState->EndWrite();
				}
			}
			else
			{   
				_vectorProcChann[index].iContinueNum=0;
				_lockDataRunState->BeginWrite();
				_pbAxisChannelState[indexaxis]=0;
				_lockDataRunState->EndWrite();
			}
			indexaxis++;
		}
		else
		{
			if (result->bCheckAlarm)
			{
				if (_pfProcValue[indexproc]>(_vectorProcChann[index].fHighAlarmValue))
				{
					if(JudgeProcAlarmHigh(index))
						SaveProcAlarmRecord(index,indexproc,ag_pDB);
                    _lockDataRunState->BeginWrite();
					_pbProcChannelState[indexproc]=2;//set state of a process channel is high alarm
					_lockDataRunState->EndWrite();
				}
				else if(_pfProcValue[indexproc]>(_vectorProcChann[index].fLowAlarmValue))
				{
					if(JudgeProcAlarmLow(index))
						SaveProcAlarmRecord(index,indexproc,ag_pDB);
					_lockDataRunState->BeginWrite();
					_pbProcChannelState[indexproc]=1;//set state of a process channel is low alarm
					_lockDataRunState->EndWrite();
				}
				else
				{
					_vectorProcChann[index].iContinueNum=0;
					_lockDataRunState->BeginWrite();
					_pbProcChannelState[indexproc]=0;//set state of a process channel is normal
					_lockDataRunState->EndWrite();
				}
			}
			else
			{
				_vectorProcChann[index].iContinueNum=0;
				_lockDataRunState->BeginWrite();
				_pbProcChannelState[indexproc]=0;//set state of a process channel is normal
				_lockDataRunState->EndWrite();
			}
			indexproc++;
		}
		index++;
	}
}

// 保存振动报警记录数据
void CDataHandle::SaveVibratedAlarmRecord(int ag_index,CCustomDB * ag_pDB)
{
	float iRev=0;
	if (_vectorVibChann[ag_index].iOfRev==1)
		iRev=_iRev1;
	else
		iRev=_iRev2;
	ag_pDB->Cmd("SELECT %s FROM [%s] ",gc_cWave,_strVibAlarmTable.c_str());
	ag_pDB->Cmd(" WHERE ID=%d",_iCurIDAlarmVib);
	if(ag_pDB->More())
	{   
		ag_pDB->SetFieldValue(gc_cTime,_timeCurrent);
		ag_pDB->SetBuffer(gc_cWave,(BYTE *)_ppfWave[ag_index],4*_iSmpNum);
		
		ag_pDB->Cmd("UPDATE [%s] ", _strVibAlarmTable.c_str());
		ag_pDB->Cmd(" SET %s='%s',",gc_cChannID,_vectorVibChann[ag_index].strChann.c_str());
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cAll,_pfVibCharAll[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cOne,_pfVibCharOne[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cTwo,_pfVibCharTwo[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cThree,_pfVibCharThree[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cHalf,_pfVibCharHalf[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cRes,_pfVibCharRes[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cPhaseOne,_pfPhaseOne[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cPhaseTwo,_pfPhaseTwo[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cPhaseThree,_pfPhaseThree[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cPhaseHalf,_pfPhaseHalf[ag_index]);
		ag_pDB->Cmd("[%s]=%.2f, ",gc_cDC,_pfDC[ag_index]);
		ag_pDB->Cmd("[%s]=%d, ",gc_cSampleNum,_iSmpNum);
		ag_pDB->Cmd("[%s]=%d, ",gc_cSampleFreq,_iSmpFreq);
	    ag_pDB->Cmd("[%s]=%.2f, ",gc_cRev,iRev);
        ag_pDB->Cmd("[%s]=%s ",gc_cTime,_strTimeCurr);
		ag_pDB->Cmd(" WHERE [ID]=%d",_iCurIDAlarmVib);
		ag_pDB->More();
	}
	else
	{
		ag_pDB->Cmd("INSERT INTO [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s) ",\
			_strVibAlarmTable.c_str(),gc_cID,gc_cChannID,gc_cAll,gc_cOne,gc_cTwo,gc_cThree,gc_cHalf,gc_cRes,gc_cPhaseOne,gc_cPhaseTwo,gc_cPhaseThree,gc_cPhaseHalf,gc_cDC,gc_cSampleNum,gc_cSampleFreq,gc_cRev);
		ag_pDB->Cmd(" VALUES (%d,'%s',%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%d,%d,%d)",\
			_iCurIDAlarmVib,_vectorVibChann[ag_index].strChann.c_str(),\
			_pfVibCharAll[ag_index],_pfVibCharOne[ag_index],_pfVibCharTwo[ag_index],_pfVibCharThree[ag_index],\
			_pfVibCharHalf[ag_index],_pfVibCharRes[ag_index],_pfPhaseOne[ag_index],_pfPhaseTwo[ag_index],\
			_pfPhaseThree[ag_index],_pfPhaseHalf[ag_index],_pfDC[ag_index],_iSmpNum,_iSmpFreq,iRev);
		if(!ag_pDB->Exec())
		{	
#ifdef _DEBUG
			AfxMessageBox("insert record failed by dse");
#endif
		    return;
		}
		ag_pDB->Cmd("SELECT %s,%s FROM [%s] ",gc_cWave,gc_cTime,_strVibAlarmTable.c_str());
		ag_pDB->Cmd(" WHERE %s=%d",gc_cID,_iCurIDAlarmVib);
		if(ag_pDB->More())
		{
			ag_pDB->SetFieldValue(gc_cTime,_timeCurrent);
			ag_pDB->SetBuffer(gc_cWave,(BYTE *)_ppfWave[ag_index],4*_iSmpNum);
		}
		else
		{
#ifdef _DEBUG
       AfxMessageBox("select record failed by dse!");
#endif
			return ;
		}
	}
	if(_iCurIDAlarmVib>=_iAlarmVibRecordAllNum)
		_iCurIDAlarmVib=0;
	else
		_iCurIDAlarmVib+=1;
	if(_pfVibCharAll[ag_index]/(_vectorVibChann[ag_index].fBeforeValue) > D_FLUCTUATE_RANGE)
	{
		_vectorVibChann[ag_index].iContinueNum=1;
		_vectorVibChann[ag_index].iIntervalNum=0;
	}
	_vectorVibChann[ag_index].timeCheck=_timeCurrent;
	_vectorVibChann[ag_index].fBeforeValue=_pfVibCharAll[ag_index];
}

// 保存过程量报警记录,ag_index对应所有过程量（包括轴位移）在测点信息向量，ag_index2是对应具体数据的index，是过程量（轴位移除外）还是轴位移
void CDataHandle::SaveProcAlarmRecord(int ag_index,int ag_index2,CCustomDB * ag_pDB)
{
	

	float fcuralarm;
	if((_vectorProcChann[ag_index].iType)==GE_AXISCHANN)
		fcuralarm=_pfAxisDisplaceValue[ag_index2];//此时代表轴位移值
	else
		fcuralarm=_pfProcValue[ag_index2];//此时代表过程量值

	ag_pDB->Cmd("SELECT * FROM [%s] ",_strProcAlarmTable.c_str());
	ag_pDB->Cmd(" WHERE ID=%d",_iCurIDAlarmProc);
	
	//如果有这个ID的记录就修改，没有就添加这个ID的记录
	if(ag_pDB->More())
	{
		ag_pDB->SetFieldValue(gc_cChannID,_vectorProcChann[ag_index].strChannel.c_str());
		ag_pDB->SetFieldValue(gc_cValue,fcuralarm);
		ag_pDB->SetFieldValue(gc_cChannType,_vectorProcChann[ag_index].iType);
		ag_pDB->SetFieldValue(gc_cTime,_timeCurrent);
	}
	else
	{//the total number of alarm record have'n super to limit.
		ag_pDB->Cmd("INSERT INTO [%s](%s,%s,%s,%s,%s) ",\
			_strProcAlarmTable.c_str(),gc_cID,gc_cChannID,gc_cValue,gc_cChannType,gc_cTime);
		ag_pDB->Cmd(" VALUES (%d,'%s',%0.2f,%d,%s)",_iCurIDAlarmProc,_vectorProcChann[ag_index].strChannel.c_str(),\
			fcuralarm,_vectorProcChann[ag_index].iType,_strTimeCurr);
		ag_pDB->Exec();

	}
	//使ID值加一，如果达到最大记录限制，就置0，就循环起来
	if(_iCurIDAlarmProc>=_iAlarmProcRecordAllNum)
		_iCurIDAlarmProc=0;
	else
		_iCurIDAlarmProc+=1;
	//判断当前报警值是否超过上次的2％，如果超过就重新开始保存
	if(fcuralarm/(_vectorProcChann[ag_index].fBeforeValue) > D_FLUCTUATE_RANGE)
	{
		_vectorProcChann[ag_index].iContinueNum=1;
		_vectorProcChann[ag_index].iIntervalNum=0;
	}
	//把保存报警记录的时间和报警值大小进行赋值，用于下次判断
	_vectorProcChann[ag_index].timeCheck=_timeCurrent;
	_vectorProcChann[ag_index].fBeforeValue=fcuralarm;
}

/*!*/
bool CDataHandle::JudgeVibAlarmPri(int ag_index)
{
	COleDateTimeSpan timespan;

	timespan=_timeCurrent-(_vectorVibChann[ag_index].timeCheck);

	if ((timespan.GetTotalSeconds())>D_ALARMRECORD_PRITIME)
	{
		_vectorVibChann[ag_index].iIntervalNum+=1;
		return true;
	}
	return false;
}

bool CDataHandle::JudgeProcAlarmPri(int ag_index)
{
	COleDateTimeSpan timespan;

	timespan=_timeCurrent-(_vectorProcChann[ag_index].timeCheck);
	if ((timespan.GetTotalSeconds())>D_ALARMRECORD_PRITIME)
	{  //测点信息中的间隔记录数加一
		_vectorProcChann[ag_index].iIntervalNum+=1;
		return true;
	}
	return false;
}

bool CDataHandle::JudgeVibAlarmSup(int ag_index)
{
	COleDateTimeSpan timespan;
	//如果上次保存报警记录和当前时间相差时间段达到规定间隔，则返回true
	timespan=_timeCurrent-(_vectorVibChann[ag_index].timeCheck);
	if ((timespan.GetTotalSeconds())>D_ALARMRECORD_SUPTIME)
	{
		return true;
	}
	return false;
}

bool CDataHandle::JudgeProcAlarmSup(int ag_index)
{
	COleDateTimeSpan timespan;

	timespan=_timeCurrent-(_vectorProcChann[ag_index].timeCheck);
	if ((timespan.GetTotalSeconds())>D_ALARMRECORD_SUPTIME)
	{
		return true;
	}
	return false;
}

bool CDataHandle::JudgeVibAlarmLow(int ag_index)
{
	_vectorVibChann[ag_index].bHighAlarm=false;
	if (_vectorVibChann[ag_index].iIntervalNum>D_ALARMRECORD_INTERVAL)
	{
		return JudgeVibAlarmSup(ag_index);
	}
	else if(_vectorVibChann[ag_index].iContinueNum>D_ALARMRECORD_CON)
	{
		return JudgeVibAlarmPri(ag_index);
	}
	else
	{
		_vectorVibChann[ag_index].iContinueNum+=1;
		_vectorVibChann[ag_index].iIntervalNum=0;
		return true;
	}

}

bool CDataHandle::JudgeProcAlarmLow(int ag_index)
{
	_vectorProcChann[ag_index].bHighAlarm=false;
	if (_vectorProcChann[ag_index].iIntervalNum>D_ALARMRECORD_INTERVAL)
	{
		return JudgeProcAlarmSup(ag_index);
	}
	else if(_vectorProcChann[ag_index].iContinueNum>D_ALARMRECORD_CON)
	{
		return JudgeProcAlarmPri(ag_index);
	}
	else
	{
		_vectorProcChann[ag_index].iContinueNum+=1;
		_vectorProcChann[ag_index].iIntervalNum=0;
		return true;
	}
}

bool CDataHandle::JudgeVibAlarmHigh(int ag_index)
{
	if (_vectorVibChann[ag_index].bHighAlarm)
	{
		if(_vectorVibChann[ag_index].iIntervalNum>D_ALARMRECORD_INTERVAL)
			return JudgeVibAlarmSup(ag_index);
		else if(_vectorVibChann[ag_index].iContinueNum>D_ALARMRECORD_CON)
		{
			return JudgeVibAlarmPri(ag_index);
		}
		else
		{
			_vectorVibChann[ag_index].iContinueNum+=1;
			return true;
		}
	}
	else
	{
		_vectorVibChann[ag_index].bHighAlarm=true;
		_vectorVibChann[ag_index].iContinueNum=1;
		_vectorVibChann[ag_index].iIntervalNum=0;
		return  true;
	}
}

bool CDataHandle::JudgeProcAlarmHigh(int ag_index)
{
	if (_vectorProcChann[ag_index].bHighAlarm)
	{
		if(_vectorProcChann[ag_index].iIntervalNum>D_ALARMRECORD_INTERVAL)
			return JudgeProcAlarmSup(ag_index);
		else if(_vectorVibChann[ag_index].iContinueNum>D_ALARMRECORD_CON)
		{
			return JudgeProcAlarmPri(ag_index);
		}
		else
		{
			_vectorProcChann[ag_index].iContinueNum+=1;
			return true;
		}
	}
	else
	{
		_vectorProcChann[ag_index].bHighAlarm=true;
		_vectorProcChann[ag_index].iContinueNum=1;
		_vectorProcChann[ag_index].iIntervalNum=0;
		return  true;
	}
}

// 初始化报警表的最大记录条数，得到当前报警表中的最新记录的ID值，
void CDataHandle::AlarmInitIDAndRecordAllNum(void)
{
	//得到过程量报警表和振动报警表的记录总条数限制
	_iAlarmProcRecordAllNum=(_vectorProcChann.size())*D_ALARMRECORD_NUM_PERCHANN;
	_iAlarmVibRecordAllNum=(_vectorVibChann.size())*D_ALARMRECORD_NUM_PERCHANN;
	//得到振动报警表中下一条记录应该保存的ID值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_strVibAlarmTable.c_str(),gc_cTime);
	
	if(_pDB->More())
	{
		_iCurIDAlarmVib=_pDB->GetLong(gc_cID)+1;
		if (_iCurIDAlarmVib>_iAlarmVibRecordAllNum)
		{
			_iCurIDAlarmVib=0;
		}
	}
	else
		_iCurIDAlarmVib=0;
	//得到过程量报警表中下一条记录应该保存的ID值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_strProcAlarmTable.c_str(),gc_cTime);
	if(_pDB->More())
	{
		_iCurIDAlarmProc=_pDB->GetLong(gc_cID)+1;
		if (_iCurIDAlarmProc>_iAlarmProcRecordAllNum)
		{
			_iCurIDAlarmProc=0;
		}
	}
	else
		_iCurIDAlarmProc=0;
}

// 判断有没有报警
void CDataHandle::JudgeAlarm(CCustomDB * ag_pDB)
{
	COleDateTime timeCurrent=COleDateTime::GetCurrentTime();
	_timeCurrent=COleDateTime(timeCurrent.GetYear(),timeCurrent.GetMonth(),timeCurrent.GetDay(),timeCurrent.GetHour(),timeCurrent.GetMinute(),timeCurrent.GetSecond());
	_strTimeCurr.Format("'%d-%d-%d %d:%d:%d'",timeCurrent.GetYear(),timeCurrent.GetMonth(),timeCurrent.GetDay(),timeCurrent.GetHour(),timeCurrent.GetMinute(),timeCurrent.GetSecond());
	
	UpdateChannAlarmParam(ag_pDB);

	bool bVibDataUpdate=true,bProcDataUpdate=true;

	//得到测点数据是否已经更新过了
	_lockDataUpdatedAlarm->BeginRead();
	bVibDataUpdate=_bVibDataUpdatedForAlarm;
	_lockDataUpdatedAlarm->EndRead();

	if (bVibDataUpdate) {
		_lockDataVib->BeginRead();
		JudgeVibratedAlarm(ag_pDB);
		_lockDataVib->EndRead();
		
		_lockDataUpdatedAlarm->BeginWrite();
		_bVibDataUpdatedForAlarm=false;
		_lockDataUpdatedAlarm->EndWrite();

	}

	_lockDataUpdatedAlarm->BeginRead();
	bProcDataUpdate=_bProcDataUpdatedForAlarm;
	_lockDataUpdatedAlarm->EndRead();
	if(bProcDataUpdate){
		_lockDataProc->BeginRead();
		JudgeProcAlarm(ag_pDB);
		_lockDataProc->EndRead();
		
		_lockDataUpdatedAlarm->BeginWrite();
		_bProcDataUpdatedForAlarm=false;
		_lockDataUpdatedAlarm->EndWrite();
	}
}

// 得到设备号
string CDataHandle::GetPlantID(void)
{
	return _strPlant;
}

int CDataHandle::GetSmpMaxNum(void){
	return _iSmpMaxNum;
}

// 设置设备转速
void CDataHandle::SetPlantRev(float ag_iRev1,float ag_iRev2)
{
	_lockDataVib->BeginWrite();
	_iRev1=ag_iRev1;
	_iRev2=ag_iRev2;
	_lockDataVib->EndWrite();
}

int * CDataHandle::GetProcChannelStatePointer(void)
{
	return _pbProcChannelState;
}

CMutexRW * CDataHandle::GetRunStateLock()
{
	return _lockDataRunState;
}

int * CDataHandle::GetAxisDisplaceChannelStatePointer(void)
{
	return _pbAxisChannelState;
}
int * CDataHandle::GetVibChannelStatePointer(void)
{
	return _pbVibChannelState;
}

int CDataHandle::GetVibChannelNum(void)
{
	return _iVibChannNum;
}

int CDataHandle::GetProcChannelNum(void)
{
	return _iProcChannNum;
}

// 得到这个设备轴位移通道的个数
int CDataHandle::GetAxisDisplaceChannelNum(void)
{
	return _iAxisDisplaceNum;
}



// 判断是否到保存天趋势的时候
bool CDataHandle::JudgeDayTrendSave(void)
{
	if (_iTrendDayInteral>D_TREND_DAYINTERVAL)
	{
		_iTrendDayInteral=0;
		return true;
	}
	else
	{
		_iTrendDayInteral+=1;
		return false;
	}	
}

// 判断是否到保存月趋势记录的时刻
bool CDataHandle::JudgeMonthTrendSave(void)
{
	if (_iTrendMonthInteral>D_TREND_MONTHINTERVAL)
	{
		_iTrendMonthInteral=0;
		return true;
	}
	else
	{
		_iTrendMonthInteral+=1;
		return false;
	}
}

// 天趋势保存
void CDataHandle::SaveDayTrend(void)
{
	SaveVibRecord(_strVibDayTrendTable,_iCurrIDTrendDay);
	SaveProcRecord(_strProcDayTrendTable,_iCurrIDTrendDay);
	if (_iCurrIDTrendDay>D_TREND_DAYRANGE)
	{
		_iCurrIDTrendDay=0;
	}
	else
	{
		_iCurrIDTrendDay+=1;
	}
	if (JudgeMonthTrendSave())
	{
		SaveMonthTrend();
	}
}

// 小时趋势保存
void CDataHandle::SaveHourTrend(void)
{
	SaveVibRecord(_strVibHourTrendTable,_iCurrIDTrendHour);
	SaveProcRecord(_strProcHourTrendTable,_iCurrIDTrendHour);
	if (_iCurrIDTrendHour>D_TREND_HOURRANGE)
	{
		_iCurrIDTrendHour=0;
	}
	else
	{
		_iCurrIDTrendHour+=1;
	}
	if (JudgeDayTrendSave())
	{
		SaveDayTrend();
	}
}

// 月趋势保存
void CDataHandle::SaveMonthTrend(void)
{
	SaveVibRecord(_strVibMonthTrendTable,_iCurrIDTrendMonth,true);
	SaveProcRecord(_strProcMonthTrendTable,_iCurrIDTrendMonth,true);
	_iCurrIDTrendMonth+=1;
}

// 振动趋势记录保存,月趋势没有记录总条数得限制，就一直在插入记录，没有修改记录的操作
void CDataHandle::SaveVibRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth)
{
	int index=0;
	VECTOR_S_VIBINFO::iterator result;
    if (!ag_bMonth)
	{
		_pDB->ClearSql();
		_pDB->Cmd("SELECT * FROM [%s] ",ag_strTableName.c_str());
		_pDB->Cmd(" WHERE [%s]=%d",gc_cID,ag_iCurID);
		if(_pDB->More())
		{   
			index=0;
			_lockDataVib->BeginRead();
			for (result=_vectorVibChann.begin();result!=_cvectorEndVib;result++)
			{
				_pDB->SetBuffer(result->strWave.c_str(),(BYTE *)_ppfWave[index],_iSmpNum*4);
				index++;
			}
            _lockDataVib->EndRead();
            
			index=0;
			_pDB->Cmd("UPDATE [%s] SET ",ag_strTableName.c_str());
			_lockDataVib->BeginRead();
			for (result=_vectorVibChann.begin();result!=_cvectorEndVib;result++)
			{
				_pDB->Cmd(" [%s]=%.2f, ",result->strAll.c_str(),_pfVibCharAll[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strOne.c_str(),_pfVibCharOne[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strTwo.c_str(),_pfVibCharTwo[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strThree.c_str(),_pfVibCharThree[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strHalf.c_str(),_pfVibCharHalf[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strRes.c_str(),_pfVibCharRes[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strPhaseOne.c_str(),_pfPhaseOne[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strPhaseTwo.c_str(),_pfPhaseTwo[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strPhaseThree.c_str(),_pfPhaseThree[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strPhaseHalf.c_str(),_pfPhaseHalf[index]);
				_pDB->Cmd(" [%s]=%.2f, ",result->strDC.c_str(),_pfDC[index]);
				index++;
			}
			_pDB->Cmd(" [%s]=%s, ",gc_cTime,_strTimeCurr);
			_pDB->Cmd(" [%s]=%d, ",gc_cSampleNum,_iSmpNum);
			_pDB->Cmd(" [%s]=%d, ",gc_cSampleFreq,_iSmpFreq);
			_pDB->Cmd(" [%s]=%.2f, ",gc_cRev,_iRev1);
			_pDB->Cmd(" [%s]=%.2f",gc_cRev2,_iRev2);
			_pDB->Cmd(" WHERE [%s]=%d",gc_cID,ag_iCurID);
			_lockDataVib->EndRead();

			_pDB->More();
			return;
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	_pDB->ClearSql();
	_pDB->Cmd("INSERT INTO [%s](%s,%s) ",ag_strTableName.c_str(),gc_cID,_strVibFieldIns.c_str());
	_pDB->Cmd("VALUES (%d,",ag_iCurID);
	index=0;
	_lockDataVib->BeginRead();
	for (result=_vectorVibChann.begin();result!=_cvectorEndVib;result++)
	{
		_pDB->Cmd("%0.2f,",_pfVibCharAll[index]);
		_pDB->Cmd("%0.2f,",_pfVibCharOne[index]);
		_pDB->Cmd("%0.2f,",_pfVibCharTwo[index]);
		_pDB->Cmd("%0.2f,",_pfVibCharThree[index]);
		_pDB->Cmd("%0.2f,",_pfVibCharHalf[index]);
		_pDB->Cmd("%0.2f,",_pfVibCharRes[index]);
		_pDB->Cmd("%0.2f,",_pfPhaseOne[index]);
		_pDB->Cmd("%0.2f,",_pfPhaseTwo[index]);
		_pDB->Cmd("%0.2f,",_pfPhaseThree[index]);
		_pDB->Cmd("%0.2f,",_pfPhaseHalf[index]);
		_pDB->Cmd("%0.2f,",_pfDC[index]);
		index++;
	}
	_pDB->Cmd("%d,",_iSmpNum);
	_pDB->Cmd("%d,",_iSmpFreq);
	_pDB->Cmd("%.2f,",_iRev1);
	_pDB->Cmd("%.2f,",_iRev2);
	_pDB->Cmd("%s)",_strTimeCurr);
	_lockDataVib->EndRead();
	if(!_pDB->Exec())
	{
#ifdef _DEBUG
 		AfxMessageBox("insert record failed by dse！");
#endif
		return;
	}
    
	_pDB->Cmd("SELECT %s FROM [%s]",_strVibFieldSel.c_str(),ag_strTableName.c_str());
	_pDB->Cmd(" WHERE %s=%d",gc_cID,ag_iCurID);
	if(_pDB->More())
	{   
		index=0;	
		for (result=_vectorVibChann.begin();result!=_cvectorEndVib;result++)
		{
			_lockDataVib->BeginRead();
			_pDB->SetBuffer(result->strWave.c_str(),(BYTE *)_ppfWave[index],_iSmpNum*4);
			_lockDataVib->EndRead();
			index++;
		}
	}
	else
	{
#ifdef _DEBUG
		AfxMessageBox("select record failed by dse");
#endif
		return;
	}
}

// 过程量趋势记录保存,月趋势没有记录总条数得限制，就一直在插入记录，没有修改记录的操作
void CDataHandle::SaveProcRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth)
{   
	
	int indexproc,indexaxis;
	VECTOR_S_PROCINFO::iterator result;
	if(!ag_bMonth)
	{
		_pDB->ClearSql();
		_pDB->Cmd("SELECT * FROM [%s]",ag_strTableName.c_str());
		_pDB->Cmd(" WHERE %s=%d",gc_cID,ag_iCurID);
		if(_pDB->More())
		{    
		/*	indexproc=0;
			indexaxis=0;
			_lockDataProc->BeginRead();
			for (result=_vectorProcChann.begin();result!=_cvectorEndProc;result++)
			{
				if((result->iType)==GE_AXISCHANN)
				{//axis displace channel 
					_pDB->SetFieldValue(result->strChannel.c_str(),_pfAxisDisplaceValue[indexaxis]);
					indexaxis++;
				}
				else
				{
					_pDB->SetFieldValue(result->strChannel.c_str(),_pfProcValue[indexproc]);
					indexproc++;
				}
			}
			_pDB->SetFieldValue(gc_cTime,_timeCurrent);
			_lockDataProc->EndRead();
			return;

			_pDB->ClearSql();*/
			_pDB->Cmd("UPDATE [%s] SET  ",ag_strTableName.c_str());
			indexproc=0;
			indexaxis=0;
			_lockDataProc->BeginRead();
			for (result=_vectorProcChann.begin();result!=_cvectorEndProc;result++)
			{
				if((result->iType)==GE_AXISCHANN)
				{//axis displace channel 
					_pDB->Cmd(" [%s]=%.2f,",result->strChannel.c_str(),_pfAxisDisplaceValue[indexaxis]);
					indexaxis++;
				}
				else
				{
					_pDB->Cmd(" [%s]=%.2f,",result->strChannel.c_str(),_pfProcValue[indexproc]);
					indexproc++;
				}
			}
			_lockDataProc->EndRead();
			_pDB->Cmd(" [%s]=%s ",gc_cTime,_strTimeCurr);
			_pDB->Cmd("  WHERE [%s]=%d",gc_cID,ag_iCurID);
	//		TRACE("proc trend save:%s\n ",_pDB->GetSql());
			if(_pDB->Exec()){    
				return;
			}
		}
	}
	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	_pDB->ClearSql();
	_pDB->Cmd("INSERT INTO [%s](%s,%s) ",ag_strTableName.c_str(),gc_cID,_strProcField.c_str());
	_pDB->Cmd("VALUES (%d,",ag_iCurID);
	indexproc=0;
	indexaxis=0;
	_lockDataProc->BeginRead();
	for (result=_vectorProcChann.begin();result!=_cvectorEndProc;result++)
	{   
		if((result->iType)==GE_AXISCHANN)
		{//axis displace channel 
			_pDB->Cmd("%0.2f,",_pfAxisDisplaceValue[indexaxis]);
			indexaxis++;
		}
		else
		{
			_pDB->Cmd("%0.2f,",_pfProcValue[indexproc]);
			indexproc++;
		}
	}
	_pDB->Cmd("%s)",_strTimeCurr);
	_lockDataProc->EndRead();
	if(!_pDB->Exec()){
#ifdef _DEBUG
		AfxMessageBox("insert ProcTrend record failed by dse!");
#endif
	}
}

// 保促趋势数据，外部调用
void CDataHandle::SaveTrendData(void)
{
	_timeCurrent=COleDateTime::GetCurrentTime();
	_timeCurrent=COleDateTime(_timeCurrent.GetYear(),_timeCurrent.GetMonth(),_timeCurrent.GetDay(),_timeCurrent.GetHour(),_timeCurrent.GetMinute(),_timeCurrent.GetSecond());
	_strTimeCurr.Format("'%d-%d-%d %d:%d:%d'",_timeCurrent.GetYear(),_timeCurrent.GetMonth(),_timeCurrent.GetDay(),_timeCurrent.GetHour(),_timeCurrent.GetMinute(),_timeCurrent.GetSecond());
	SaveSecondTrendData();

}

// 初始化各种趋势的当前记录ID值和天趋势、月趋势的间隔值
void CDataHandle::TrendInitIDAndInterval(void)
{
	COleDateTimeSpan span;
	int iminute;
	_timeCurrent=COleDateTime::GetCurrentTime();

	//初始化秒趋势(即当前一小时的详细数据)表中最新的记录ID值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_strVibSecondTrendTable.c_str(),gc_cTime);
//	TRACE(("%s\n"),_pDB->GetSql());
	if(_pDB->More())
	{
		_iCurrIDTrendSecond=_pDB->GetLong(gc_cID)+1;
		if (_iCurrIDTrendSecond>D_TREND_SECONDRANGE)
		{
			_iCurrIDTrendSecond=0;
		}
	}
	else
		_iCurrIDTrendSecond=0;

	//初始化小时趋势表中最新的记录ID值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s FROM [%s] ORDER BY %s DESC",gc_cID,_strVibHourTrendTable.c_str(),gc_cTime);
//	TRACE(("%s\n"),_pDB->GetSql());
	_iTrendHourInteral=0;
	if(_pDB->More())
	{
		_iCurrIDTrendHour=_pDB->GetLong(gc_cID)+1;
		if (_iCurrIDTrendHour>D_TREND_HOURRANGE)
		{
			_iCurrIDTrendHour=0;
		}
	}
	else
		_iCurrIDTrendHour=0;
	//初始化天趋势中最新的记录的ID值，和保存天趋势需要的间隔值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s,%s FROM [%s] ORDER BY %s DESC",gc_cID,gc_cTime,_strVibDayTrendTable.c_str(),gc_cTime);
//	TRACE(("%s\n"),_pDB->GetSql());
	if (_pDB->More())
	{
		_iCurrIDTrendDay=(_pDB->GetLong(gc_cID))+1;
		if (_iCurrIDTrendDay>D_TREND_DAYRANGE)
		{
			_iCurrIDTrendDay=0;
		}
		span=_timeCurrent-(_pDB->GetTime(gc_cTime));
		iminute=span.GetTotalMinutes();
		if (iminute>D_TREND_DAYINTERVAL)
		{
			_iTrendDayInteral=D_TREND_DAYINTERVAL;
		}
		else
		{
			_iTrendDayInteral=iminute;
		}
	}
	else
	{
		_iCurrIDTrendDay=0;
		_iTrendDayInteral=20;
	}
	//初始化月趋势中最新的记录ID值，和保存月趋势需要的间隔值
	_pDB->ClearSql();
	_pDB->Cmd("SELECT TOP 1 %s,%s FROM [%s] ORDER BY %s DESC",gc_cID,gc_cTime,_strVibMonthTrendTable.c_str(),gc_cTime);
//	TRACE(("%s\n"),_pDB->GetSql());
	if(_pDB->More())
	{
		_iCurrIDTrendMonth=(_pDB->GetLong(gc_cID))+1;
		span=_timeCurrent-(_pDB->GetTime(gc_cTime));
		iminute=span.GetTotalMinutes();
		if ((iminute/20)>36)
		{
			_iTrendMonthInteral=37;
		}
		else
		{
			_iTrendMonthInteral=iminute/20;
		}

	}
	else
	{
		_iCurrIDTrendMonth=0;
		_iTrendMonthInteral=37;
	}
}


// 保存在当前趋势表单中得趋势数据，只保存一个小时之类得。一秒钟保存一次（在外部通过定时器控制）。
void CDataHandle::SaveSecondTrendData(void)
{
	bool bDataUpdate=true;
    int iSmpNum=0;
	_lockDataUpdatedHis->BeginRead();
	bDataUpdate=_bDataUpdatedForHis;
	_lockDataUpdatedHis->EndRead();
    
	_lockDataVib->BeginRead();
	iSmpNum=_iSmpNum;
	_lockDataVib->EndRead();

	if(bDataUpdate && (iSmpNum>0))//判断是否满足保存条件
	{
		SaveVibRecord(_strVibSecondTrendTable,_iCurrIDTrendSecond);
		SaveProcRecord(_strProcSecondTrendTable,_iCurrIDTrendSecond);
		if (_iCurrIDTrendSecond>D_TREND_SECONDRANGE){
			_iCurrIDTrendSecond=0;
		}
		else{
			_iCurrIDTrendSecond+=1;
		}

    	if (JudgeHourTrendSave())
		{
			SaveHourTrend();
		}
		_lockDataUpdatedHis->BeginWrite();
		_bDataUpdatedForHis=false;
		_lockDataUpdatedHis->EndWrite();
	}
}

// 判断是否该保存小时趋势
bool CDataHandle::JudgeHourTrendSave(void)
{
	if (_iTrendHourInteral>D_TREND_HOURINTERVAL)
	{
		_iTrendHourInteral=0;
		return true;
	}
	else
	{
		_iTrendHourInteral+=1;
		return false;
	}	
}

void CDataHandle::UpdateVibData(float * ag_pfCharOne,
								float * ag_pfCharTwo,
								float * ag_pfCharThree,
								float * ag_pfCharAll,
								float * ag_pfCharHalf,
								float * ag_pfCharRes,
								float * ag_pfDC,
								float * pfPhaseOne_,
								float * pfPhaseTwo_,
								float * pfPhaseThree_,
								float * pfPhaseHalf_,
								float * ag_fwave,
								int ag_iSmpNum,
								int ag_iSmpFreq,
								int ag_iRealNum,
								int ag_len,
							 int ag_startIndex)
{
	int ilen=(ag_startIndex+ag_len)>_iVibChannNum?(_iVibChannNum-ag_startIndex):ag_len;
	if((ilen>0)&&(ag_startIndex>-1)){
        //设置数据更新状态变量为true
		_lockDataUpdatedHis->BeginWrite();
		_bDataUpdatedForHis=true;
		_lockDataUpdatedHis->EndWrite();
         
		//进行数据更新
		_lockDataVib->BeginWrite();
		_iSmpNum=ag_iSmpNum>_iSmpMaxNum?_iSmpMaxNum:ag_iSmpNum;
		_iSmpFreq=ag_iSmpFreq;
		int iRealPointNum=ag_iRealNum>_iSmpMaxNum?_iSmpMaxNum:ag_iRealNum;
		memcpy(&(_pfVibCharOne[ag_startIndex]),ag_pfCharOne,4*ilen);
		memcpy(&(_pfVibCharTwo[ag_startIndex]),ag_pfCharTwo,4*ilen);
		memcpy(&(_pfVibCharThree[ag_startIndex]),ag_pfCharThree,4*ilen);
		memcpy(&(_pfVibCharAll[ag_startIndex]),ag_pfCharAll,4*ilen);
		memcpy(&(_pfVibCharHalf[ag_startIndex]),ag_pfCharHalf,4*ilen);
		memcpy(&(_pfVibCharRes[ag_startIndex]),ag_pfCharRes,4*ilen);
		memcpy(&(_pfDC[ag_startIndex]),ag_pfDC,4*ilen);
		memcpy(&(_pfPhaseOne[ag_startIndex]),pfPhaseOne_,4*ilen);
		memcpy(&(_pfPhaseTwo[ag_startIndex]),pfPhaseTwo_,4*ilen);
		memcpy(&(_pfPhaseThree[ag_startIndex]),pfPhaseThree_,4*ilen);
		memcpy(&(_pfPhaseHalf[ag_startIndex]),pfPhaseHalf_,4*ilen);
		for(int loop=0;loop<ilen;++loop){
			memcpy(_ppfWave[loop+ag_startIndex],&(ag_fwave[loop*ag_iRealNum]),iRealPointNum*GD_FLOAT_SIZE);
			//	writefile(_ppfWave[loop],_iSmpNum,loop+100);
		}
		_lockDataVib->EndWrite();
		
		_lockDataUpdatedAlarm->BeginWrite();
		_bVibDataUpdatedForAlarm=true;
		_lockDataUpdatedAlarm->EndWrite();
	}
}
///设置过程量数据
void CDataHandle::UpdateProcData(float *ag_pfData,int ag_len,int ag_startIndex)
{
   int ilen=(ag_startIndex+ag_len)>_iProcChannNum?(_iProcChannNum-ag_startIndex):ag_len;

   if((ilen>0)&&(ag_startIndex>-1)){
	   //设置过程量数据更新变量为true
	   _lockDataUpdatedHis->BeginWrite();
	   _bDataUpdatedForHis=true;
	   _lockDataUpdatedHis->EndWrite();
 
	   //更新过程量数据
	   _lockDataProc->BeginWrite();
       memcpy(&(_pfProcValue[ag_startIndex]),ag_pfData,GD_FLOAT_SIZE*ilen); 
	   _lockDataProc->EndWrite();
	   
	   _lockDataUpdatedAlarm->BeginWrite();
	   _bProcDataUpdatedForAlarm=true;
	   _lockDataUpdatedAlarm->EndWrite();
   }
}

///设置过程量数据
void CDataHandle::UpdateAxisDisplaceData(float *ag_pfData,int ag_len,int ag_startIndex)
{
   int ilen=(ag_startIndex+ag_len)>_iAxisDisplaceNum?(_iAxisDisplaceNum-ag_startIndex):ag_len;

   if((ilen>0)&&(ag_startIndex>-1)){
	   _lockDataUpdatedHis->BeginWrite();
	   _bDataUpdatedForHis=true;
	   _lockDataUpdatedHis->EndWrite();


	   _lockDataProc->BeginWrite();
       memcpy(&(_pfAxisDisplaceValue[ag_startIndex]),ag_pfData,GD_FLOAT_SIZE*ilen); 
	   _lockDataProc->EndWrite();

	   _lockDataUpdatedAlarm->BeginWrite();
	   _bProcDataUpdatedForAlarm=true;
	   _lockDataUpdatedAlarm->EndWrite();  
   }
}

// 更新测点的报警参数信息
void CDataHandle::UpdateChannAlarmParam(CCustomDB *ag_pDB)
{
	int iChannType=0;
	int iAlarmFlag=0;
	float fAlarmHigh=0;
	float fAlarmLow=0;

	VECTOR_S_VIBINFO::iterator resultVib;
	VECTOR_S_PROCINFO::iterator resultProc;
	ag_pDB->Cmd("SELECT %s,%s,%s FROM [%s] WHERE %s='%s' ORDER BY ID",gc_cChannID,gc_cChannType,gc_cAlarmParam,gc_cChannTable,gc_cPlantID,_strPlant.c_str());
    resultVib=_vectorVibChann.begin();
    resultProc=_vectorProcChann.begin();
	while (ag_pDB->More())
	{
		iChannType=ag_pDB->GetLong(gc_cChannType);
 //       iAlarmFlag=ag_pDB->GetLong(gc_cCheckAlarm);
//		fAlarmLow=ag_pDB->GetDouble(gc_cAlarmLow);
//		fAlarmHigh=ag_pDB->GetDouble(gc_cAlarmHigh);

		if (iChannType==1)
		{//vibrate channel
			if (resultVib!=_cvectorEndVib) {
				if (iAlarmFlag==0) {
					resultVib->bCheckAlarm=false;
				}
				else{
					resultVib->bCheckAlarm=true;
				}
	//			TRACE("UpdateChannAlarmParam:%s,%s,%s,%f,%f \n",_strPlant.c_str(),ag_pDB->Get(gc_cChannID),resultVib->strChann.c_str(),fAlarmLow,fAlarmHigh);
				resultVib->fLowAlarmValue=fAlarmLow;
                resultVib->fHighAlarmValue=fAlarmHigh;
				resultVib++;
			}
			else{
				break;
			}
		}
		else
		{//process and axisdisplace channel
			if (resultProc!=_cvectorEndProc) {
				if (iAlarmFlag==0) {
					resultProc->bCheckAlarm=false;
				}
				else{
					resultProc->bCheckAlarm=true;
				}
				TRACE("UpdateChannAlarmParam:%s,%s,%s,%f,%f \n",_strPlant.c_str(),ag_pDB->Get(gc_cChannID),resultProc->strChannel.c_str(),fAlarmLow,fAlarmHigh);
				resultProc->fLowAlarmValue=fAlarmLow;
				resultProc->fHighAlarmValue=fAlarmHigh;
				resultProc++;
			}
			else{
				break;
			}
		}
	}
}
