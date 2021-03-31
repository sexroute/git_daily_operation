#include "stdafx.h"
#include "vibdata.h"
#include "additions/DebugHelper.h"



CVibChannParam::CVibChannParam(void)
{
	_pChannWave=NULL;
	_sAllCharField.clear();
	memset(_fCustomParamData,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
	memset(_fCustomParamData_Real,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
	this->_nChannWavePointNum = 0;
	this->SecondTblLastValue(G_F_INIT_VALUE);
	this->DayTblLastValue(G_F_INIT_VALUE);
	this->HourTblLastValue(G_F_INIT_VALUE);
	this->MonthTblLastValue(G_F_INIT_VALUE);
	this->m_nChannOnline = -1;
	this->m_nChannNormal = -1;
	this->m_nDataUpdate  = -1;
	this->m_nBatteryPower= -1;
	m_oUpdateTime = CBHDateTime::GetCurrentTime();
}

CVibChannParam::~CVibChannParam(void)
{
}
 
void CVibChannParam::SyncData()
{
	this->_sVibCharValue = this->_sVibCharValue_Real;

	if (this->_sVibCharValue._iSmpNum > this->_nChannWavePointNum)
	{
		this->_sVibCharValue._iSmpNum = this->_nChannWavePointNum;
	}
	
	try
	{
		memcpy(this->_fCustomParamData,this->_fCustomParamData_Real,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
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
	

	try
	{
		memcpy(this->_pChannWave,this->_pChannWave_real,this->_sVibCharValue_Real._iSmpNum*sizeof(DATATYPE_WAVE));
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
	
}

 const S_VibCharValue CVibChannParam::GetCharValue() const
{
	return _sVibCharValue;
}

 S_VibCharValue CVibChannParam::GetCharValueEx() 
{
	return _sVibCharValue;
}
 S_VibCharValue CVibChannParam::GetRealCharValueEx()
 {
	 return this->_sVibCharValue_Real;
 }

 S_VibCharValue CVibChannParam::GetCharValueEx(int anIndex) 
{
	return _sVibCharValue;
}

float * CVibChannParam::GetCustomParamData()
{
	return this->_fCustomParamData;
}

float * CVibChannParam::GetRealCustomParamData()
{
	return this->_fCustomParamData_Real;
}


bool CVibChannParam::SetChannValue(BHKD::S_VibCharValue *pVibCharValue_)
{
	if (pVibCharValue_==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

     _sVibCharValue_Real=*pVibCharValue_;

	 if (_sVibCharValue_Real._iSmpNum > this->_nChannWavePointNum)
	 {
		
		 //log here
		 CString lstrErrorLog;
		 lstrErrorLog.Format(_T("*******************Error Occoured!%s %d****************** : 数采配置的采样点数:%d 大于中间件组态配置的采样点数:%d，忽略多余点数"),
			 __FILE__,
			 __LINE__,_sVibCharValue_Real._iSmpNum,
			 _nChannWavePointNum);

		 CHZLogManage::Error(lstrErrorLog);
		 zdlTraceLine(lstrErrorLog);

		   _sVibCharValue_Real._iSmpNum = this->_nChannWavePointNum;

	 }
	 
	 return true;
}

void CVibChannParam::SetChannInfo(const S_ChannInfo &channInfo_)
{
	CChannInfoBase::SetChannInfo(channInfo_);
	string sChannNo=channInfo_._cChannNo;
	_sCharAll="ALL_";
	_sCharAll.append(sChannNo);
	_sCharOne="ONE_";
	_sCharOne.append(sChannNo);
	_sCharTwo="TWO_";
	_sCharTwo.append(sChannNo);
	_sCharThree="THREE_";
	_sCharThree.append(sChannNo);
	_sCharHalf="HALF_";
	_sCharHalf.append(sChannNo);
	_sDC="DC_";
	_sDC.append(sChannNo);
	_sCharRes="RES_";
	_sCharRes.append(sChannNo);
	_sGap="GAP_";
	_sGap.append(sChannNo);
	_sPhaseOne="ONEPHASE_";
	_sPhaseOne.append(sChannNo);
	_sPhaseTwo="TWOPHASE_";
	_sPhaseTwo.append(sChannNo);
	_sPhaseThree="THREEPHASE_";
	_sPhaseThree.append(sChannNo);
	_sPhaseHalf="HALFPHASE_";
	_sPhaseHalf.append(sChannNo);
	_sWave="WAVE_";
	_sWave.append(sChannNo);
	_sSmpFreq="SAMPLE_FREQ_";
	_sSmpFreq.append(sChannNo);
	_sSmpNum="SAMPLE_NUM_";
	_sSmpNum.append(sChannNo);
	_sRev="REV_";
	_sRev.append(sChannNo);
}

bool CVibChannParam::SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_)
{
   if ((pfWave_==NULL)||(iSmpNum_<0))
   {
	   ASSERT(FALSE);
	   return false;
   }

	//取初始采样点数和传入采样点数中较小的作为实际的采样点数
   int lnReciveDataSize = iSmpNum_ * sizeof(DATATYPE_WAVE);

   int lnBufferSize = this->_nChannWavePointNum  * sizeof(DATATYPE_WAVE);

   if (lnReciveDataSize>lnBufferSize)
   {
	   lnReciveDataSize = lnBufferSize;

	   iSmpNum_ = lnReciveDataSize/sizeof(DATATYPE_WAVE);
	   
   }

   _sVibCharValue_Real.SetSpecTypeData(lnReciveDataSize/sizeof(DATATYPE_WAVE),GE_SAMPLENUM); 

   memcpy(_pChannWave_real,pfWave_,lnReciveDataSize);

   return true;
}

int CVibChannParam::GetWaveNum()
{
	return this->_nChannWavePointNum;
}

const DATATYPE_WAVE * CVibChannParam::GetCurrWavePoint()
{
	ASSERT(NULL != _pChannWave);
	return _pChannWave;
}

const DATATYPE_WAVE * CVibChannParam::GetCurrRealWavePoint()
{
	ASSERT(NULL != _pChannWave_real);
	return _pChannWave_real;
}

void CVibChannParam::SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	_pChannWave=pWave_;
	
	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数
}


void CVibChannParam::SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	_pChannWave_real=pWave_;

	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数
}


string CVibChannParam::GetCharValueField()
{
    if (_sAllCharField.empty())
	{
		_sAllCharField.append("[");
		_sAllCharField.append(_sCharAll);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharOne);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharTwo);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharThree);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharHalf);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharRes);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sDC);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sGap);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sPhaseOne);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sPhaseTwo);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sPhaseThree);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sPhaseHalf);
		_sAllCharField.append("]");
    }
	return _sAllCharField.c_str();
}

string CVibChannParam::GetSpecCharTypeField(int iCharType_)
{
	string sType;
	_pLock->BeginRead();
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		sType=_sCharAll;
		break;
	case GE_ONECHAR:
		sType=_sCharOne;
		break;
	case GE_TWOCHAR:
		sType=_sCharTwo;
		break;
	case GE_THREECHAR:
		sType=_sCharThree;
		break;
	case GE_HALFCHAR:
		sType=_sCharHalf;
		break;
	case GE_RESCHAR:
		sType=_sCharRes;
		break;
	case GE_GAP:
		sType=_sGap;
		break;
	case GE_DCCHAR:
		sType=_sDC;
		break;
    case GE_PHASECHARHALF:
		sType=_sPhaseHalf;
		break;
	case GE_PHASECHARONE:
		sType=_sPhaseOne;
		break;
	case GE_PHASECHARTWO:
		sType=_sPhaseTwo;
		break;
	case GE_PHASECHARTHREE:
		sType=_sPhaseThree;
		break;
	case -1:
		sType="WAVE_";
		sType.append(_channInfo._cChannNo);
		break;
	case -2:
		sType="TYPE_";
		sType.append(_channInfo._cChannNo);
		break;
	case -3:
		sType="SAMPLE_FREQ_";
		sType.append(_channInfo._cChannNo);
		break;
	case -4:
		sType="SAMPLE_NUM_";
		sType.append(_channInfo._cChannNo);
		break;
	case -5:
		sType="REV_";
		sType.append(_channInfo._cChannNo);
		break;
	case -6:
		sType="TEMPLETID_";
		sType.append(_channInfo._cChannNo);
		break;
	case -7:
		sType="CUSTOM_PARAM_";
		sType.append(_channInfo._cChannNo);
		break;
	default:
		sType=_sCharAll;
	}
	_pLock->EndRead();
	return sType.c_str();
}

BOOL CVibChannParam::IsNeedComputeEigenValue()
{
	if (IsMWComputeFeatureValueEnabled()) //默认为true
	{
		if (IsEigenValueEmpty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CVibChannParam::IsEigenValueEmpty()
{
	float val = 0;
	
	//判断全部特征值是否为0
	for (int i = GE_ALLCHAR ; i <=GE_RESCHAR ;i++)
	{
		if(i == GE_GAP)
		{
			continue;
		}
		val = this->_sVibCharValue.GetSpecTypeData(i);
		
		if (!BHKD::IsFloatZero(val))
		{
			return FALSE;
		}
	}

	return TRUE;
}

///[TBC]
BOOL CVibChannParam::ComputeEigenValue( int aEnumMachineType,int anSelftIndexInPlant )
{
	try
	{

		if (this->IsNeedComputeEigenValue())
		{
			

			//真实计算
			EigenValueComputer::ComputeVibEigenValue(
													this->_sVibCharValue,
													this->_channInfo,
													this->_sVibCharValue._iRev,
													this->_pChannWave,
													this->_sVibCharValue._iSmpNum,
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
	
	return FALSE;
}

BOOL CVibChannParam::ShouldSaveSecondTblWave(const S_VibCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->SecondTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfCurrValue = aCharValue_._fPhaseThree;
	}
	else
	{
		lfCurrValue = aCharValue_._fCharAll;
	}


	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->SecondTblLastValue(lfLastValue);
		if(lsChannInfo._iChannType == GE_AXIALCHANN)
		{
			this->SecondTblLastOnePhaseValue(aCharValue_._fPhaseOne);
		}
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}

					this->SecondTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}
		
	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->SecondTblLastValue(lfLastValue);
			return TRUE;
		}
	}

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfLastValue = this->SecondTblLastOnePhaseValue();

		lfCurrValue = aCharValue_._fPhaseOne;

		VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

		if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->SecondTblLastOnePhaseValue(lfLastValue);
			return TRUE;
		}
	}
	return FALSE;
}
 
BOOL CVibChannParam::ShouldSaveHourTblWave(const S_VibCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->HourTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfCurrValue = aCharValue_._fPhaseThree;
	}
	else
	{
		lfCurrValue = aCharValue_._fCharAll;
	}

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->HourTblLastValue(lfLastValue);
		if(lsChannInfo._iChannType == GE_AXIALCHANN)
		{
			this->HourTblLastOnePhaseValue(aCharValue_._fPhaseOne);
		}
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > (lfLastValue +  VarRate) || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue +VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->HourTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->HourTblLastValue(lfLastValue);
			return TRUE;
		}
	}

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfLastValue = this->HourTblLastOnePhaseValue();

		lfCurrValue = aCharValue_._fPhaseOne;

		VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

		if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->HourTblLastOnePhaseValue(lfLastValue);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CVibChannParam::ShouldSaveDayTblWave(const S_VibCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->DayTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfCurrValue = aCharValue_._fPhaseThree;
	}
	else
	{
		lfCurrValue = aCharValue_._fCharAll;
	}

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->DayTblLastValue(lfLastValue);
		if(lsChannInfo._iChannType == GE_AXIALCHANN)
		{
			this->DayTblLastOnePhaseValue(aCharValue_._fPhaseOne);
		}
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->DayTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->DayTblLastValue(lfLastValue);
			return TRUE;
		}
	}

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfLastValue = this->DayTblLastOnePhaseValue();

		lfCurrValue = aCharValue_._fPhaseOne;

		VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

		if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->DayTblLastOnePhaseValue(lfLastValue);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CVibChannParam::ShouldSaveMonthTblWave(const S_VibCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->MonthTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfCurrValue = aCharValue_._fPhaseThree;
	}
	else
	{
		lfCurrValue = aCharValue_._fCharAll;
	}

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->MonthTblLastValue(lfLastValue);
		if(lsChannInfo._iChannType == GE_AXIALCHANN)
		{
			this->MonthTblLastOnePhaseValue(aCharValue_._fPhaseOne);
		}
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->MonthTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->MonthTblLastValue(lfLastValue);
			return TRUE;
		}
	}

	if(lsChannInfo._iChannType == GE_AXIALCHANN)
	{
		lfLastValue = this->MonthTblLastOnePhaseValue();

		lfCurrValue = aCharValue_._fPhaseOne;

		VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

		if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->MonthTblLastOnePhaseValue(lfLastValue);
			return TRUE;
		}
	}


	return FALSE;
}

void CVibChannParam::SecondTblLastValue( const float afValue_ )
{
	this->_fSecondTblLastValue = afValue_;
}

float CVibChannParam::SecondTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fSecondTblLastValue;
	return lfValue;
}

void CVibChannParam::HourTblLastValue( const float afValue_ )
{
	this->_fHourTblLastValue = afValue_;
}

float CVibChannParam::HourTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fHourTblLastValue;
	return lfValue;
}

void CVibChannParam::DayTblLastValue( const float afValue_ )
{
	this->_fDayTblLastValue = afValue_;
}

float CVibChannParam::DayTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fDayTblLastValue;
	return lfValue;
}

void CVibChannParam::MonthTblLastValue( const float afValue_ )
{

	this->_fMonthTblLastValue = afValue_;
}

float CVibChannParam::MonthTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fMonthTblLastValue;
	return lfValue;
}

void CVibChannParam::SecondTblLastOnePhaseValue( const float afValue_ )
{
	this->_fSecondTblLastOnePhaseValue = afValue_;
}

float CVibChannParam::SecondTblLastOnePhaseValue()
{
	float lfValue = 0;
	lfValue = this->_fSecondTblLastOnePhaseValue;
	return lfValue;
}

void CVibChannParam::HourTblLastOnePhaseValue( const float afValue_ )
{
	this->_fHourTblLastOnePhaseValue = afValue_;
}

float CVibChannParam::HourTblLastOnePhaseValue()
{
	float lfValue = 0;
	lfValue = this->_fHourTblLastOnePhaseValue;
	return lfValue;
}

void CVibChannParam::DayTblLastOnePhaseValue( const float afValue_ )
{
	this->_fDayTblLastOnePhaseValue = afValue_;
}

float CVibChannParam::DayTblLastOnePhaseValue()
{
	float lfValue = 0;
	lfValue = this->_fDayTblLastOnePhaseValue;
	return lfValue;
}


void CVibChannParam::MonthTblLastOnePhaseValue( const float afValue_ )
{
	this->_fMonthTblLastOnePhaseValue = afValue_;
}

float CVibChannParam::MonthTblLastOnePhaseValue()
{
	float lfValue = 0;
	lfValue = this->_fMonthTblLastOnePhaseValue;
	return lfValue;
}

void CVibChannParam::ChannOnline(const int anOnLine)
{
	this->m_nChannOnline = anOnLine;
}

int  CVibChannParam::ChannOnline()
{
	return this->m_nChannOnline;
}

void CVibChannParam::ChannNormal(const int anNormal)
{
	this->m_nChannNormal = anNormal;
}

int  CVibChannParam::ChannNormal()
{
	return this->m_nChannNormal;
}

void CVibChannParam::ChannDataUpdate(const int anDataUpdate)
{
	this->m_nDataUpdate = anDataUpdate;
}

int  CVibChannParam::ChannDataUpdate()
{
	return this->m_nDataUpdate;
}

void CVibChannParam::BatteryPower(const int anBatteryPower)
{
	this->m_nBatteryPower = anBatteryPower;
}

int  CVibChannParam::BatteryPower()
{
	return this->m_nBatteryPower;
}

//--------------------------------------------//

CDynChannParam::CDynChannParam(void)
{
	_pChannWave = NULL;
	_pChannWave_real = NULL;
	_sAllCharField.clear();
	memset(_fCustomParamData,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
	memset(_fCustomParamData_real,0,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
	this->SecondTblLastValue(G_F_INIT_VALUE);
	this->HourTblLastValue(G_F_INIT_VALUE);
	this->DayTblLastValue(G_F_INIT_VALUE);
	this->MonthTblLastValue(G_F_INIT_VALUE);
	m_oUpdateTime = CBHDateTime::GetCurrentTime();
}

CDynChannParam::~CDynChannParam(void)
{
}


bool CDynChannParam::SetChannValue(const BHKD::S_DynCharValue *pDynCharValue_)
{
	//_sDynCharValue = pDynCharValue_;
	if (pDynCharValue_==NULL)
		return false;
	_sDynCharValue_real=*pDynCharValue_;
	if (_sDynCharValue_real._iSmpNum > this->_nChannWavePointNum)
	{
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d****************** : 数采配置的采样点数:%d 大于中间件组态配置的采样点数:%d，忽略多余点数"),
			__FILE__,
			__LINE__,_sDynCharValue_real._iSmpNum,
			_nChannWavePointNum);

		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		_sDynCharValue_real._iSmpNum = this->_nChannWavePointNum;
	}
	return true;
}

void CDynChannParam::SetChannInfo(const S_ChannInfo &channInfo_)
{
	CChannInfoBase::SetChannInfo(channInfo_);
	string sChannNo=channInfo_._cChannNo;
	_sCharZero     = "ZERO_";
	_sCharZero.append(sChannNo);
	_sCharOne      = "ONE_";
	_sCharOne.append(sChannNo);
	_sCharTwo      = "TWO_";
	_sCharTwo.append(sChannNo);
	_sCharThree    = "THREE_";
	_sCharThree.append(sChannNo);
	_sCharFour     = "FOUR_";
	_sCharFour.append(sChannNo);
	_sCharFive     = "FIVE_"+ sChannNo;
	_sCharSix      = "SIX_"+  sChannNo;
	_sCharSeven    = "SEVEN_"+sChannNo;
	_sCharEight    = "EIGHT_"+sChannNo;
	_sCharNine     = "NINE_"+ sChannNo;
	_sCharTen      = "PERIOD_START_INDEX_"+ sChannNo;
	_sCharEleven   = "PERIOD_END_INDEX_"+ sChannNo;
	_sWave         = "WAVE_"+ sChannNo;
	_sSmpFreq      ="SAMPLE_FREQ_";
	_sSmpFreq.append(sChannNo);
	_sSmpNum       ="SAMPLE_NUM_";
	_sSmpNum.append(sChannNo);
	_sRev=        "REV_";
	_sRev.append(sChannNo);
}

bool CDynChannParam::SetChannWave(const DATATYPE_WAVE *pfWave_, const int &iSmpNum_)
{
	if ( ( pfWave_ == NULL) || ( iSmpNum_<0 ) )
	{
		return false;
	}

	if (_pChannWave == NULL)
	{
		CHZLogManage::Error("动态测点没有分配波形内存", "Online", "CDynChannParam::SetChannWave");
	}
	
	//取初始采样点数和传入采样点数中较小的作为实际的采样点数
	int lnReciveDataSize = iSmpNum_ * sizeof(DATATYPE_WAVE);

	int lnBufferSize = this->_nChannWavePointNum  * sizeof(DATATYPE_WAVE);

	if (lnReciveDataSize>lnBufferSize)
	{
		lnReciveDataSize = lnBufferSize;
	}
	
	//将测点的波形数据采样点数更新为实际的波形数据采样点数
	_sDynCharValue_real.SetSpecTypeData(lnReciveDataSize/sizeof(DATATYPE_WAVE),GE_SAMPLENUM);

	memcpy(_pChannWave_real,pfWave_,lnReciveDataSize);

	return true;
}

int CDynChannParam::GetWaveNum()
{
	return this->_nChannWavePointNum;
}

const DATATYPE_WAVE * CDynChannParam::GetCurrWavePoint()
{
	ASSERT(NULL != _pChannWave);
	return _pChannWave;
}


const DATATYPE_WAVE * CDynChannParam::GetCurrRealWavePoint()
{
	ASSERT(NULL != _pChannWave_real);
	return _pChannWave_real;
}
void CDynChannParam::SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	_pChannWave = pWave_;

	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数，来自于数据库组态
}


void CDynChannParam::SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_)
{
	_pChannWave_real = pWave_;

	this->_nChannWavePointNum = iSmpNum_;//modified by zdl，构造波形缓存的时候，保存采样点数，来自于数据库组态
}
string CDynChannParam::GetCharValueField()
{
	if (_sAllCharField.empty())
	{
		_sAllCharField.append("[");
		_sAllCharField.append(_sCharZero);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharOne);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharTwo);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharThree);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharFour);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharFive);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharSix);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharSeven);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharEight);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharNine);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharTen);
		_sAllCharField.append("],[");
		_sAllCharField.append(_sCharEleven);
		_sAllCharField.append("]");
	}
	return _sAllCharField.c_str();
}

const S_DynCharValue CDynChannParam::GetCharValue() const
{
	return _sDynCharValue;
}

  S_DynCharValue CDynChannParam::GetCharValueEx() 
{
	return _sDynCharValue;
}

S_DynCharValue CDynChannParam::GetRealCharValueEx() 
{
	return _sDynCharValue_real;
}

void CDynChannParam::SyncData()
{
	this->_sDynCharValue = this->_sDynCharValue_real;

	if (this->_sDynCharValue._iSmpNum > this->_nChannWavePointNum)
	{
		this->_sDynCharValue._iSmpNum = this->_nChannWavePointNum;
	}

	try
	{
		memcpy(this->_fCustomParamData,this->_fCustomParamData_real,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
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
	

	try
	{
		memcpy(this->_pChannWave,this->_pChannWave_real,this->_sDynCharValue_real._iSmpNum*sizeof(DATATYPE_WAVE));
	}
	catch (...)
	{
	    //log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d  _nChannWavePointNum:%d ******************"),
							__FILE__,
							__LINE__,_nChannWavePointNum);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		
	}
	
}

 float * CDynChannParam::GetCustomParamData()
 {
	 return this->_fCustomParamData;
 }

 float * CDynChannParam::GetRealCustomParamData()
 {
	 return this->_fCustomParamData_real;
 }

  S_DynCharValue CDynChannParam::GetCharValueEx(int index_) 
{
	return _sDynCharValue;
}

string CDynChannParam::GetSpecCharTypeField(int iCharType_)
{
	string sType;
	_pLock->BeginRead();
	switch(iCharType_)
	{
	case 0:
		sType=_sCharZero;
		break;
	case 1:
		sType=_sCharOne;
		break;
	case 2:
		sType=_sCharTwo;
		break;
	case 3:
		sType=_sCharThree;
		break;
	case 4:
		sType=_sCharFour;
		break;
	case 5:
		sType=_sCharFive;
		break;
	case 6:
		sType=_sCharSix;
		break;
	case 7:
		sType=_sCharSeven;
		break;
	case 8:
		sType=_sCharEight;
		break;
	case 9:
		sType=_sCharNine;
		break;
	case 10:
		sType=_sCharTen;
		break;
	case 11:
		sType=_sCharEleven;
		break;
	case -1:
		sType="WAVE_";
		sType.append(_channInfo._cChannNo);
		break;
	case -2:
		sType="TYPE_";
		sType.append(_channInfo._cChannNo);
		break;
	case -3:
		sType="SAMPLE_FREQ_";
		sType.append(_channInfo._cChannNo);
		break;
	case -4:
		sType="SAMPLE_NUM_";
		sType.append(_channInfo._cChannNo);
		break;
	case -5:
		sType="REV_";
		sType.append(_channInfo._cChannNo);
		break;
	case -6:
		sType="TEMPLETID_";
		sType.append(_channInfo._cChannNo);
		break;
	case -7:
		sType="CUSTOM_PARAM_";
		sType.append(_channInfo._cChannNo);
		break;
	default:
		sType=_sCharZero;
	}
	_pLock->EndRead();
	return sType.c_str();
}


BOOL CDynChannParam::IsNeedComputeEigenValue()
{
	if (IsMWComputeFeatureValueEnabled()) //默认为true
	{
		if (IsEigenValueEmpty())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDynChannParam::IsEigenValueEmpty()
{
	float val = 0;
	
	//判断全部特征值是否为0,跳过沉降量判断，防止波形为0的情况下存数
	for (int i = 0 ; i <= 5 ;i++)
	{
		if(this->GetChannInfo()._iChannType == GE_RODSINKCHANN && (i == 0 || i == 3))
		{
			continue;
		}
		val = this->_sDynCharValue.GetSpecTypeData(i);
		
		if (!BHKD::IsFloatZero(val))
		{
			return FALSE;
		}
	}

	return TRUE;
}

///[TBC]
BOOL CDynChannParam::ComputeEigenValue( int aEnumMachineType,int anCurrentIndexInPlant )
{
	try
	{
		//1.判断报警设备序列中是否有计算完的特征值
		if (this->IsNeedComputeEigenValue())
		{

			//真实计算
			EigenValueComputer::ComputeDynEigenValue(
													this->_sDynCharValue,
													this->_channInfo,
													this->_sDynCharValue._iRev,
													this->_pChannWave,
													this->_sDynCharValue._iSmpNum,
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

	return FALSE;
}


BOOL CDynChannParam::ShouldSaveSecondTblWave(const S_DynCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->SecondTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();
	
	lfCurrValue = aCharValue_._fCharValue0;

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->SecondTblLastValue(lfLastValue);
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate|| lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}

					this->SecondTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}

				this->SecondTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}

			this->SecondTblLastValue(lfLastValue);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDynChannParam::ShouldSaveHourTblWave(const S_DynCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->HourTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	lfCurrValue = aCharValue_._fCharValue0;

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->HourTblLastValue(lfLastValue);
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->HourTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->HourTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->HourTblLastValue(lfLastValue);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDynChannParam::ShouldSaveDayTblWave(const S_DynCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->DayTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	lfCurrValue = aCharValue_._fCharValue0;

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->DayTblLastValue(lfLastValue);
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->DayTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->DayTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->DayTblLastValue(lfLastValue);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CDynChannParam::ShouldSaveMonthTblWave(const S_DynCharValue& aCharValue_)
{
	float lfCurrValue = 0;

	float lfLastValue = this->MonthTblLastValue();

	S_ChannInfo lsChannInfo = this->GetChannInfo();

	lfCurrValue = aCharValue_._fCharValue0;

	if(IsFloatZero(lfLastValue - G_F_INIT_VALUE))
	{
		lfLastValue = lfCurrValue;
		this->MonthTblLastValue(lfLastValue);
		return TRUE;
	}

	float VarRate = abs(lfLastValue)*(G_ALARM_THRESHOLD)/100.0;

	if(lsChannInfo._chAlarmParam._bRegular)
	{
		switch(lsChannInfo._chAlarmParam._iRegJudgeType)
		{
		case E_ALARMCHECK_LOWPASS:
			if(lfCurrValue > lsChannInfo._chAlarmParam._fAlarmHL)
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);

				return TRUE;
			}
			break;
		case E_ALARMCHECK_HIGHPASS:
			if(lfCurrValue < lsChannInfo._chAlarmParam._fAlarmLH)
			{
				return TRUE;
			}
			else if(lfCurrValue < lfLastValue - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;

		case E_ALARMCHECK_BANDPASS:
			if ((lfCurrValue>lsChannInfo._chAlarmParam._fAlarmHL)||(lfCurrValue<lsChannInfo._chAlarmParam._fAlarmLH))
			{
				return TRUE;
			}
			else if(lfCurrValue > lfLastValue + VarRate || lfCurrValue > lfLastValue  - VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		case E_ALARMCHECK_BANDSTOP:
			if((lfCurrValue<lsChannInfo._chAlarmParam._fAlarmHH)&&(lfCurrValue>lsChannInfo._chAlarmParam._fAlarmLL))
			{
				return TRUE;
			}
			else
			{
				if((lfCurrValue >= lsChannInfo._chAlarmParam._fAlarmHH && lfCurrValue < lfLastValue - VarRate) || 
					(lfCurrValue <= lsChannInfo._chAlarmParam._fAlarmLL && lfCurrValue > lfLastValue + VarRate))
				{
					if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
					{
						lfLastValue = (lfCurrValue + lfLastValue)/2;
					}
					else
					{
						lfLastValue = lfCurrValue;
					}
					this->MonthTblLastValue(lfLastValue);
					return TRUE;
				}

			}
			break;
		default:
			if(lfCurrValue > lfLastValue + VarRate)
			{
				if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
				{
					lfLastValue = (lfCurrValue + lfLastValue)/2;
				}
				else
				{
					lfLastValue = lfCurrValue;
				}
				this->MonthTblLastValue(lfLastValue);
				return TRUE;
			}
			break;
		}

	}
	else
	{
		if(lfCurrValue > lfLastValue + VarRate)
		{
			if(!IsFloatZero(lfLastValue - G_F_INIT_VALUE))
			{
				lfLastValue = (lfCurrValue + lfLastValue)/2;
			}
			else
			{
				lfLastValue = lfCurrValue;
			}
			this->MonthTblLastValue(lfLastValue);
			return TRUE;
		}
	}
	return FALSE;
}

void CDynChannParam::SecondTblLastValue( const float afValue_ )
{
	this->_fSecondTblLastValue = afValue_;
}

float CDynChannParam::SecondTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fSecondTblLastValue;
	return lfValue;
}

void CDynChannParam::HourTblLastValue( const float afValue_ )
{
	this->_fHourTblLastValue = afValue_;
}

float CDynChannParam::HourTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fHourTblLastValue;
	return lfValue;
}

void CDynChannParam::DayTblLastValue( const float afValue_ )
{
	this->_fDayTblLastValue = afValue_;
}

float CDynChannParam::DayTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fDayTblLastValue;
	return lfValue;
}

void CDynChannParam::MonthTblLastValue( const float afValue_ )
{
	this->_fMonthTblLastValue = afValue_;
}

float CDynChannParam::MonthTblLastValue()
{
	float lfValue = 0;
	lfValue = this->_fMonthTblLastValue;
	return lfValue;
}


//--------------------------------------------//
CProcChannParam::CProcChannParam()
{
	_fValue=0;
	this->m_nChannOnline = -1;
	this->m_nChannNormal = -1;
	this->m_nDataUpdate  = -1;
	this->m_nBatteryPower= -1;
	this->m_nProcSUDStatus = 0;
	m_dtLastUpdateTime = CBHDateTime::GetCurrentTime();
}

CProcChannParam::~CProcChannParam()
{
}

void CProcChannParam::SetData(float fValue_,CBHDateTime dataTime_)
{
	_fValue=fValue_;

	m_dtUpdateData = dataTime_;
}

float CProcChannParam::GetData()
{
	return _fValue;
}

void CProcChannParam::ChannOnline(const int anOnLine)
{
	this->m_nChannOnline = anOnLine;
}

int  CProcChannParam::ChannOnline()
{
	return this->m_nChannOnline;
}

void CProcChannParam::ChannNormal(const int anNormal)
{
	this->m_nChannNormal = anNormal;
}

int  CProcChannParam::ChannNormal()
{
	return this->m_nChannNormal;
}

void CProcChannParam::ChannDataUpdate(const int anDataUpdate)
{
	this->m_nDataUpdate = anDataUpdate;
}

int  CProcChannParam::ChannDataUpdate()
{
	return this->m_nDataUpdate;
}

void CProcChannParam::BatteryPower(const int anBatteryPower)
{
	this->m_nBatteryPower = anBatteryPower;
}

int  CProcChannParam::BatteryPower()
{
	return this->m_nBatteryPower;
}

void CProcChannParam::SetSUDStatus(int anSUDStatus)
{
	m_nProcSUDStatus=anSUDStatus;
}

float CProcChannParam::GetSUDStatus()
{
	return m_nProcSUDStatus;
}

CBHDateTime CProcChannParam::GetUpdateTime()
{
	return m_dtUpdateData;
}

bool CProcChannParam::IsChannUpdateDataOverTime()
{
	bool lbOverTime = false;

	CBHDateTime loCurrentTime = CBHDateTime::GetCurrentTime();

	COleDateTimeSpan loTimeSpan = loCurrentTime - GetLastUpdateTime();

	int lnSpan = loTimeSpan.GetTotalMinutes();

	if (lnSpan > g_UpdateProcDataOverTime)
	{
		lbOverTime = true;
	}

	return lbOverTime;
}

//------------------------------------------//

CAxisChannParam::CAxisChannParam()
{
	_plockRW = NULL;
}

CAxisChannParam::~CAxisChannParam()
{
}

void CAxisChannParam::SetChannInfoEx(CAxisChannInfo channInfo_,int iVerIndex_,int iHorIndex_,int iRevType_,float fSensitivityVer_,float fSensitivityHor_)
{
    SetChannInfo(channInfo_);
	_iHorIndex=iHorIndex_;
	_iVerIndex=iVerIndex_;
	_iRevType=iRevType_;
	_fSensitivityHor=fSensitivityHor_;
	_fSensitivityVer=fSensitivityVer_;
}

void CAxisChannParam::SetDataLock(IReadWriteLock *pLock_)
{
	_plockRW =pLock_;
}

void CAxisChannParam::SetParentPoint(LPVOID pParent_)
{
	_pParent=pParent_;
}

bool CAxisChannParam::UpdateData()
{
	return true;
}

