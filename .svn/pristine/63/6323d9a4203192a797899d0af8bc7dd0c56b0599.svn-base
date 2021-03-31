#include "stdafx.h"
#include "ZoomAnalysis.h"
#include <fstream>
#include <assert.h>
#include "PlantInfoBase.h"
#include "MiddleWare.h"
#include <list>
#include <additions/DebugHelper.h>
#include <additions/BHOleDbWrapper.h>
#include <additions/CTimeOutReadWriteLock.h>

using std::list;

#include <cmath>
#include <algorithm>

#include <SigMath.h>

CChannInfoBase::CChannInfoBase()
{
  _pLock = NULL;
  _sRevField = gc_cRev;
  _vCustomParamInfo.clear();
  _vCustomAlarmParamInfo.clear();
  m_bChannUpdate = false;
  
  this->AlarmConfirmType(ALARM_CONFIRM_TYPE_NONE);
  this->AlarmConfirmStartTime(CBHDateTime(TIME_INIT_VALUE));
  this->AlarmConfirmEndTime(CBHDateTime(TIME_INIT_VALUE));
  
}

CChannInfoBase::~CChannInfoBase()
{
	_vCustomParamInfo.clear();
	_vCustomAlarmParamInfo.clear();
}

void CChannInfoBase::SetRwLock(IReadWriteLock * pLock_)
{
	_pLock = pLock_;
}

CCustomParamInfo CChannInfoBase::GetSpecIndexCustomParamInfo(const int& index_)
{
	CCustomParamInfo paraminfo;
	paraminfo._iID = -1;
	int iSize = GetCustomParamSize();
	if (index_ <0 ||  index_> iSize )
		return paraminfo;

	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		try
		{
			paraminfo = _vCustomParamInfo[index_];
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLock->EndRead();
	}
	else
		paraminfo = _vCustomParamInfo[index_];
	return paraminfo;
}

bool CChannInfoBase::SetCustomParamInfo(const int& iStartIndex_, const int& ilen_, CCustomParamInfo * pCustomParamInfo_)
{
	if (iStartIndex_ <0 ||  ilen_ <1 )
		return false;

	int iSize =GetCustomParamSize();
	if ((iStartIndex_ + ilen_) > iSize)
	{
		CCustomParamInfo paramInfo;
		paramInfo._iID = -1;
		int iSpan = iStartIndex_ + ilen_ - iSize ;
		if (_pLock != NULL)
		{
			_pLock->BeginWrite();
			_vCustomParamInfo.insert(_vCustomParamInfo.end(), iSpan,paramInfo);
			_pLock->EndWrite();
		}
		else
			_vCustomParamInfo.insert(_vCustomParamInfo.end(), iSpan,paramInfo);
	}

	if (_pLock != NULL)
	{
		_pLock->BeginWrite();
		for (int nloop = 0; nloop < ilen_; ++nloop)
			_vCustomParamInfo[nloop + iStartIndex_] = pCustomParamInfo_[nloop];
		_pLock->EndWrite();
	}
	else
		for (int nloop = 0; nloop < ilen_; ++nloop)
			_vCustomParamInfo[nloop + iStartIndex_] = pCustomParamInfo_[nloop];
	return true;
}

bool CChannInfoBase::SetCustomAlarmParamInfo(const int& iStartIndex_, const int& ilen_, CCustomAlarmParamInfo * pCustomAlarmParamInfo_)
{
	if (iStartIndex_ <0 ||  ilen_ <1 )
		return false;

	int iSize =GetCustomAlarmParamSize();
	if ((iStartIndex_ + ilen_) > iSize)
	{
		CCustomAlarmParamInfo paramInfo;
		paramInfo._iID = -1;
		int iSpan = iStartIndex_ + ilen_ - iSize ;
		if (_pLock != NULL)
		{
			_pLock->BeginWrite();
		
			try
			{
				_vCustomAlarmParamInfo.insert(_vCustomAlarmParamInfo.end(), iSpan,paramInfo);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

			_pLock->EndWrite();
		}
		else
			_vCustomAlarmParamInfo.insert(_vCustomAlarmParamInfo.end(), iSpan,paramInfo);
	}

	if (_pLock != NULL)
	{
		_pLock->BeginWrite();
		try
		{
			for (int nloop = 0; nloop < ilen_; ++nloop)
				_vCustomAlarmParamInfo[nloop + iStartIndex_] = pCustomAlarmParamInfo_[nloop];
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLock->EndWrite();
	}
	else
		for (int nloop = 0; nloop < ilen_; ++nloop)
			_vCustomAlarmParamInfo[nloop + iStartIndex_] = pCustomAlarmParamInfo_[nloop];
	return true;
}

CCustomAlarmParamInfo CChannInfoBase::GetSpecIndexCustomAlarmParamInfo(const int& index_)
{
	CCustomAlarmParamInfo paraminfo;
	paraminfo._iID = -1;
	int iSize = GetCustomAlarmParamSize();
	if (index_ <0 ||  index_> iSize )
		return paraminfo;

	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		try
		{
			paraminfo = _vCustomAlarmParamInfo[index_];
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

		_pLock->EndRead();
	}
	else
		paraminfo = _vCustomAlarmParamInfo[index_];
	return paraminfo;
}

int CChannInfoBase::GetCustomParamSize()
{
	int iSize;
	if(_pLock != NULL)
	{
		_pLock->BeginRead();
		iSize = _vCustomParamInfo.size();
		_pLock->EndRead();
	}
	else
		iSize = _vCustomParamInfo.size();

	return iSize;
}

int CChannInfoBase::GetCustomAlarmParamSize()
{
	int iSize;
	if(_pLock != NULL)
	{
		_pLock->BeginRead();
		iSize = _vCustomAlarmParamInfo.size();
		_pLock->EndRead();
	}
	else
		iSize = _vCustomAlarmParamInfo.size();

	return iSize;
}

void CChannInfoBase::SetChannLocationsInfo(const CChannLocations &channLocations_)
{
	if ( NULL != _pLock)
	{
		_pLock->BeginWrite();
		_channLocations=channLocations_;
		_pLock->EndWrite();
	}
	else
		_channLocations=channLocations_;
}

CChannLocations CChannInfoBase::GetChannLocationsInfo()
{
	CChannLocations channLocations;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		channLocations=_channLocations;
		_pLock->EndRead();
	}
	else
		channLocations=_channLocations;

	return channLocations;
}

void CChannInfoBase::SetChannSectionsInfo(const CChannSections &channSections_)
{
	if ( NULL != _pLock)
	{
		_pLock->BeginWrite();
		_channSections=channSections_;
		_pLock->EndWrite();
	}
	else
		_channSections=channSections_;
}

CChannSections CChannInfoBase::GetChannSectionsInfo()
{
	CChannSections channSections;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		channSections=_channSections;
		_pLock->EndRead();
	}
	else
		channSections=_channSections;

	return channSections;
}

void CChannInfoBase::SetBearingParamInfo(const CBearingParamInfo &bearingParams_)
{
	if ( NULL != _pLock)
	{
		_pLock->BeginWrite();
		_bearingParams=bearingParams_;
		_pLock->EndWrite();
	}
	else
		_bearingParams=bearingParams_;
}

CBearingParamInfo CChannInfoBase::GetBearingParamInfo()
{
	CBearingParamInfo bearingParams;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		bearingParams=_bearingParams;
		_pLock->EndRead();
	}
	else
		bearingParams=_bearingParams;

	return bearingParams;
}

void CChannInfoBase::SetGearBoxParamInfo(const CGearBoxParamInfo &gearBoxParams_)
{
	if ( NULL != _pLock)
	{
		_pLock->BeginWrite();
		_gearBoxParams=gearBoxParams_;
		_pLock->EndWrite();
	}
	else
		_gearBoxParams=gearBoxParams_;
}

void CChannInfoBase::WaveHead( const CWaveHead& afWaveHead_ )
{
	if(_pLock != NULL)
	{
		
		_pLock->BeginWrite();
		BEGIN_ERROR_HANDLE
		m_sWaveHead = afWaveHead_;
		END_ERROR_HANDLE
		_pLock->EndWrite();
		
	}
	else
	{
		m_sWaveHead = afWaveHead_;
	}
	
}

CWaveHead CChannInfoBase::WaveHead()
{
	CWaveHead sWaveHead;
	if(_pLock != NULL)
	{
		
		_pLock->BeginRead();
		BEGIN_ERROR_HANDLE
		sWaveHead = m_sWaveHead;
		END_ERROR_HANDLE
		_pLock->EndRead();
		
	}

	return sWaveHead;
}

CGearBoxParamInfo CChannInfoBase::GetGearBoxParamInfo()
{
	CGearBoxParamInfo gearBoxParams;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		gearBoxParams=_gearBoxParams;
		_pLock->EndRead();
	}
	else
		gearBoxParams=_gearBoxParams;

	return gearBoxParams;
}

S_ChannInfo CChannInfoBase::GetChannInfo()
{
	S_ChannInfo channInfo;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		channInfo=_channInfo;
		_pLock->EndRead();
	}
	else
	  channInfo=_channInfo;

	return channInfo;
}

const string CChannInfoBase::GetRevField() const
{
	string sRevField;
	if (_pLock != NULL)
	{
		_pLock->BeginRead();
		sRevField= _sRevField;
		_pLock->EndRead();
	}
	else
		sRevField= _sRevField;
	return sRevField;
}

void CChannInfoBase::SetChannInfo(const S_ChannInfo &channInfo_)
{
	if (_pLock != NULL)
	{
		_pLock->BeginWrite();
		_channInfo=channInfo_;
		if (_channInfo._iChannType==GE_AXIALCHANN)
		{
			_channInfo._chAlarmParam._bCharTrend=false;
			_channInfo._chAlarmParam._bPhaseTrend=false;
		}
		switch( channInfo_._iRevType )
		{
		case 2:
			_sRevField = gc_cRev2;
			break;
		case 3:
			_sRevField = gc_cRev3;
			break;
		case 4:
			_sRevField = gc_cRev4;
			break;
		case 5:
			_sRevField = gc_cRev5;
			break;
		case 1:
		default:
			_sRevField = gc_cRev;
			break;
		}
		_pLock->EndWrite();
	}
	else
	{
		_channInfo=channInfo_;
		if (_channInfo._iChannType==GE_AXIALCHANN)
		{
			_channInfo._chAlarmParam._bCharTrend=false;
			_channInfo._chAlarmParam._bPhaseTrend=false;
		}
		switch( channInfo_._iRevType )
		{
		case 2:
			_sRevField = gc_cRev2;
			break;
		case 3:
			_sRevField = gc_cRev3;
			break;
		case 4:
			_sRevField = gc_cRev4;
			break;
		case 5:
			_sRevField = gc_cRev5;
			break;
		case 1:
		default:
			_sRevField = gc_cRev;
			break;
		}
	}
}

int CChannInfoBase::SetCharInfo(IDBInterFace *pDB_)
{
	return _channInfo._charInfo.size();
}


int CChannInfoBase::SetCharInfo(CCustomDB *pDB_)
{
	return _channInfo._charInfo.size();
}

/////////////////////////
CAxisChannInfoBase::CAxisChannInfoBase()
{
	_pLock = NULL;
}

CAxisChannInfoBase::~CAxisChannInfoBase()
{
}

void CAxisChannInfoBase::SetRwLock(IReadWriteLock * pLock_)
{
	_pLock = pLock_;
}

void CAxisChannInfoBase::GetChannInfo(CAxisChannInfo &axisChannInfo_)
{
	if ( NULL != _pLock)
	{
	_pLock->BeginRead();
	axisChannInfo_=_channInfo;
	_pLock->EndRead();
	}
	else
		axisChannInfo_=_channInfo;
}

CAxisChannInfo CAxisChannInfoBase::GetChannInfo()
{
	CAxisChannInfo channInfo;
	if (NULL != _pLock)
	{
		_pLock->BeginRead();
		channInfo=_channInfo;
		_pLock->EndRead();
	}
	else
		channInfo=_channInfo;
	return channInfo;
}

void CAxisChannInfoBase::SetChannInfo(const CAxisChannInfo &channInfo_)
{
	if ( NULL != _pLock)
	{
		_pLock->BeginWrite();
		_channInfo=channInfo_;
		_pLock->EndWrite();
	}
	else
		_channInfo=channInfo_;
}



int CAxisChannInfoBase::ConfirmAlarm( GE_ALARM_CONFIRM_TYPE anType,CBHDateTime &arefStartTime,CBHDateTime& arefEndTime )
{
	if (NULL!= this->_pLock)
	{
		this->_pLock->BeginWrite();

		BEGIN_ERROR_HANDLE;

		this->AlarmConfirmType(anType);
		this->AlarmConfirmStartTime(arefStartTime);
		this->AlarmConfirmEndTime(arefEndTime);

		END_ERROR_HANDLE;

		this->_pLock->EndWrite();

		return TRUE;
	}

	return FALSE;
}

BOOL CAxisChannInfoBase::IsAlarmConfirmed()
{
	BOOL lbAlarmConfirmed = FALSE;

	if (NULL!= _pLock)
	{	
		_pLock->BeginRead();

		switch(this->m_nAlarmConfirmType)
		{
		case ALARM_CONFIRM_TYPE_ONCE:
			lbAlarmConfirmed = TRUE;
			break;
		case ALARM_CONFIRM_TYPE_INTERVAL:
			{
				CBHDateTime loTimeNow = CBHDateTime::GetCurrentTime();

				if (loTimeNow >= this->m_oAlarmConfirmStartTime 
					&& loTimeNow <= this->m_oAlarmConfirmEndTime)
				{
					lbAlarmConfirmed = TRUE;
				}else
				{
					//
					if (loTimeNow>this->m_oAlarmConfirmEndTime)
					{
						//this->m_nAlarmConfirmType = ALARM_CONFIRM_TYPE_NONE;
					}else
					{
						lbAlarmConfirmed = FALSE;
					}

				}
				break;					
			}
		case ALARM_CONFIRM_TYPE_FOREVER:
			lbAlarmConfirmed = TRUE;
			break;
		default:
			lbAlarmConfirmed = FALSE;
			break;
		}	

		_pLock->EndRead();
	}

	return lbAlarmConfirmed;
}

GE_ALARM_CONFIRM_TYPE CAxisChannInfoBase::AlarmConfirmType() const
{
	return m_nAlarmConfirmType;
}

void CAxisChannInfoBase::AlarmConfirmType( GE_ALARM_CONFIRM_TYPE val )
{
	m_nAlarmConfirmType = val;
}

CBHDateTime CAxisChannInfoBase::AlarmConfirmStartTime() const
{
	return m_oAlarmConfirmStartTime;
}

void CAxisChannInfoBase::AlarmConfirmStartTime( CBHDateTime val )
{
	m_oAlarmConfirmStartTime = val;
}

CBHDateTime CAxisChannInfoBase::AlarmConfirmEndTime() const
{
	return m_oAlarmConfirmEndTime;
}

void CAxisChannInfoBase::AlarmConfirmEndTime( CBHDateTime val )
{
	m_oAlarmConfirmEndTime = val;
}


int CChannInfoBase::ConfirmAlarm( GE_ALARM_CONFIRM_TYPE  anType,CBHDateTime &arefStartTime,CBHDateTime& arefEndTime )
{
	if (NULL!= _pLock)
	{
		_pLock->BeginWrite();

		BEGIN_ERROR_HANDLE;

		this->AlarmConfirmType(anType);
		this->AlarmConfirmStartTime(arefStartTime);
		this->AlarmConfirmEndTime(arefEndTime);

		END_ERROR_HANDLE;

		_pLock->EndWrite();

		return TRUE;
	}

	return FALSE;
}

GE_ALARM_CONFIRM_TYPE  CChannInfoBase::AlarmConfirmType() const
{
	return m_nAlarmConfirmType;
}

void CChannInfoBase::AlarmConfirmType( GE_ALARM_CONFIRM_TYPE  val )
{
	m_nAlarmConfirmType = val;
}

CBHDateTime CChannInfoBase::AlarmConfirmStartTime() const
{
	return m_oAlarmConfirmStartTime;
}

void CChannInfoBase::AlarmConfirmStartTime( CBHDateTime val )
{
	m_oAlarmConfirmStartTime = val;
}

CBHDateTime CChannInfoBase::AlarmConfirmEndTime() const
{
	return m_oAlarmConfirmEndTime;
}

void CChannInfoBase::AlarmConfirmEndTime( CBHDateTime val )
{
	
	m_oAlarmConfirmEndTime = val;
}

BOOL CChannInfoBase::IsAlarmConfirmed()
{
	BOOL lbAlarmConfirmed = FALSE;

	if (NULL!= _pLock)
	{	
		_pLock->BeginRead();

		switch(this->m_nAlarmConfirmType)
		{
			case ALARM_CONFIRM_TYPE_ONCE:
				 lbAlarmConfirmed = TRUE;
				 break;
			case ALARM_CONFIRM_TYPE_INTERVAL:
				{
					CBHDateTime loTimeNow = CBHDateTime::GetCurrentTime();
					
					if (loTimeNow >= this->m_oAlarmConfirmStartTime 
						&& loTimeNow <= this->m_oAlarmConfirmEndTime)
					{
						 lbAlarmConfirmed = TRUE;
					}else
					{
						//
						if (loTimeNow>this->m_oAlarmConfirmEndTime)
						{
							//this->m_nAlarmConfirmType = ALARM_CONFIRM_TYPE_NONE;
						}else
						{
							lbAlarmConfirmed = FALSE;
						}
						 
					}
					break;					
				}
			case ALARM_CONFIRM_TYPE_FOREVER:
				lbAlarmConfirmed = TRUE;
				break;
			default:
				lbAlarmConfirmed = FALSE;
				break;
		}	

		_pLock->EndRead();
	}

	return lbAlarmConfirmed;
}

E_TBL_CHANNTYPE CChannInfoBase::GetChannOneType( const CHANNTYPE &iChannType_ )
{
	switch(iChannType_)
	{
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		return E_TBL_CHANNTYPE_PROC;
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
	case GE_AXISLOCATIONCHANN:
		return E_TBL_CHANNTYPE_VIB;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
		return E_TBL_CHANNTYPE_DYN;
	default:
		return E_TBL_CHANNTYPE_VIB;
	}
}

bool CChannInfoBase::JudgeRegResetPlantAlarmTimer( float fCurrValue_,float fValueLast_ )
{
#ifndef ALARM_THRESHOLD
	return false;
#else
	if (IsFloatZero(fValueLast_-G_F_INIT_VALUE))
	{
		return true;
	}

	if (IsFloatZero(fCurrValue_-G_F_INIT_VALUE))
	{
		return false;
	}
	bool bRtn=false;
	S_ChannInfo sChannInfo=_channInfo;

	switch(sChannInfo._chAlarmParam._iRegJudgeType)
	{
	case E_ALARMCHECK_LOWPASS:
		{
			float lfBase = abs(_channInfo._chAlarmParam._fAlarmHH - _channInfo._chAlarmParam._fAlarmHL);

			if (lfBase<abs(fValueLast_))
			{
				lfBase = abs(fValueLast_);
			}

			if (fCurrValue_<=(fValueLast_+lfBase*(G_ALARM_THRESHOLD)/100.0))
			{
				bRtn=false;
			}
			else
			{
				bRtn=true;
			}
		}
		break;
	case E_ALARMCHECK_HIGHPASS:

		{
			float lfBase = abs(_channInfo._chAlarmParam._fAlarmLL - _channInfo._chAlarmParam._fAlarmLH);

			if (lfBase<abs(fValueLast_))
			{
				lfBase = abs(fValueLast_);
			}

			if (fCurrValue_>=(fValueLast_-lfBase*(G_ALARM_THRESHOLD)/100.0))
			{
				bRtn=false;
			}
			else
			{
				bRtn=true;
			}
		}

		break;
	case E_ALARMCHECK_BANDPASS:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_<=fValueLast_*(100.0+G_ALARM_THRESHOLD)/100.0)
			{

				bRtn=false;
			}
			else
			{
				bRtn=true;
			}
		}
		break;
	case E_ALARMCHECK_BANDSTOP:

		{
			float lfbase = (sChannInfo._chAlarmParam._fAlarmHH -  sChannInfo._chAlarmParam._fAlarmLL)/2;

			fCurrValue_ = abs(fCurrValue_-lfbase);
			fValueLast_ = abs(fValueLast_-lfbase);

			if (fCurrValue_>=fValueLast_*(100.0-G_ALARM_THRESHOLD)/100.0)
			{

				bRtn=false;
			}
			else
			{
				bRtn=true;
			}
		}
		break;
	default:
		{
			float lfBase = abs(_channInfo._chAlarmParam._fAlarmHH - _channInfo._chAlarmParam._fAlarmHL);

			if (lfBase<abs(fValueLast_))
			{
				lfBase = abs(fValueLast_);
			}

			if (fCurrValue_<=(fValueLast_+lfBase*(G_ALARM_THRESHOLD)/100.0))
			{
				bRtn=false;
			}
			else
			{
				bRtn=true;
			}
			break;
		}
	}
	return bRtn;

#endif
}

/////////////////////////////////////////
CPlantInfoBase::CPlantInfoBase()
{
	CHZLogManage::Info("construction","CPlantInfoBase","CPlantInfoBase");
    _pLockChannInfo = new CTimeOutReadWriteLock();
	_vProcChann.clear();
	_vVibChann.clear();
	_vDynChann.clear();
	_vAxisChann.clear();
    _vSudChann.clear();

	_mapProcChannIndex.clear();
	_mapVibChannIndex.clear();
	_mapDynChannIndex.clear();

  	_iAxesPairNum=0;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	_iSudChannNum=0;
	_timeCurr=CBHDateTime::GetCurrentTime();
	_timeCurrVib=CBHDateTime::GetCurrentTime();
	_timeCurrDyn=CBHDateTime::GetCurrentTime();
	_timeCurrProc=CBHDateTime::GetCurrentTime();
	this->Country(COUNTRY_DEFAULT);
}

 int CPlantInfoBase::GetMiddleWareWavePointNumMax()
{
	//组态中设置的
	int lnWavePointMax = this->_sPlantInfo._iSmpNum;
	
	//中间件宏定义的
	if (lnWavePointMax < GD_MIDDLEWARE_WAVE_POINT_MAX)
	{
		lnWavePointMax = GD_MIDDLEWARE_WAVE_POINT_MAX;
	}
	return lnWavePointMax;
}

 int CPlantInfoBase::GetSafeWavePointNum(int anRealSampleWaveNum /* = -1 */)
{
	//根据当前的设备类型取得程序支持的采样点数
	int iSafeWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_ICE || GetPlantInfo()._iMachineType == GE_MACHINETYPE_DISEL)
	{
		iSafeWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
	
	} else 	if ((GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSafeWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	//根据当前中间件组态的采样点数
	int lnWaveNumInMiddleWareDB = this->GetPlantInfo()._iSmpNum;

	if (iSafeWaveNum > lnWaveNumInMiddleWareDB)
	{
		iSafeWaveNum = lnWaveNumInMiddleWareDB;
	}

	if (anRealSampleWaveNum >=0)
	{
		if (iSafeWaveNum > anRealSampleWaveNum)
		{
			iSafeWaveNum = anRealSampleWaveNum;
		}
	}

	return iSafeWaveNum;
}

CPlantInfoBase::~CPlantInfoBase(void)
{
	Destory();
}

void CPlantInfoBase::Destory()
{
	V_PCHANNINFO::iterator it;
	{
		it=_vVibChann.begin();
		CChannInfoBase *pVibChann=NULL;
		for (;it!=_vVibChann.end();++it)
		{
			pVibChann=*it;
			delete pVibChann;
			pVibChann=NULL;
		}

		for (it = _vSudChann.begin();it!=_vSudChann.end();++it)
		{
			pVibChann=*it;
			delete pVibChann;
			pVibChann=NULL;
		}
	}


	{
		CChannInfoBase *pProcChann=NULL;
		for(it=_vProcChann.begin();it!=_vProcChann.end();++it)
		{
			pProcChann=*it;
			delete pProcChann;
			pProcChann=NULL;
		}
	}
	{
		CAxisChannInfoBase *pAxisChann=NULL;
		V_PAXISCHANNINFO::iterator it1;
		for (it1=_vAxisChann.begin();it1!=_vAxisChann.end();++it1)
		{
			pAxisChann=*it1;
			delete pAxisChann;
			pAxisChann=NULL;
		}
	}
    _vProcChann.clear();
	_vVibChann.clear();
	_vAxisChann.clear();
	_vDynChann.clear();
    _mapProcChannIndex.clear();
	_mapVibChannIndex.clear();
	_mapDynChannIndex.clear();

	KD_DeletePoint(_pLockChannInfo);
}

const string CPlantInfoBase::GetRevField(const int &iRevType_ ) const
{
	string sRevField = gc_cRev;
	switch( iRevType_ )
	{
	case 2:
		sRevField = gc_cRev2;
		break;
	case 3:
		sRevField = gc_cRev3;
		break;
	case 4:
		sRevField = gc_cRev4;
		break;
	case 5:
		sRevField = gc_cRev5;
		break;
	case 1:
	default:
		sRevField = gc_cRev;
		break;
	}
	return sRevField;
}

int CPlantInfoBase::GetSpecProcChannIndex(string sChann_)
{
	map<string,int>::iterator it;
	it=_mapProcChannIndex.find(sChann_);
	if (it==_mapProcChannIndex.end())
	  return -1;
	return it->second;
}

int CPlantInfoBase::GetSpecVibChannIndex(string sChann_)
{
	map<string,int>::iterator it;
	it=_mapVibChannIndex.find(sChann_);
	if (it==_mapVibChannIndex.end())
		return -1;
	return it->second;
}

int CPlantInfoBase::GetSpecDynChannIndex(string sChann_)
{
	map<string,int>::iterator it;
	it=_mapDynChannIndex.find(sChann_);
	if (it==_mapDynChannIndex.end())
		return -1;
	return it->second;
}

int CPlantInfoBase::GetSpecProcChannIndexNo(const string &sChannNo_)
{
	V_PCHANNINFO::iterator it;
	int index=-1;
	for (it=_vProcChann.begin();it!=_vProcChann.end();++it)
	{
		++index;
		if((*it)->GetChannInfo()._cChannNo==sChannNo_)
                break;
	}
	if (it==_vProcChann.end())
		index=-1;
	return index;
}

int CPlantInfoBase::GetSpecVibChannIndexNo(const string &sChannNo_)
{
	V_PCHANNINFO::iterator it;
	int index=-1;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		if((*it)->GetChannInfo()._cChannNo==sChannNo_)
			break;
	}
	if (it==_vVibChann.end())
		index=-1;
	return index;
}

int CPlantInfoBase::GetSpecDynChannIndexNo(const string &sChannNo_)
{
	V_PCHANNINFO::iterator it;
	int index=-1;
	for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
	{
		++index;
		if(sChannNo_ == (*it)->GetChannInfo()._cChannNo)
			break;
	}
	if (it==_vDynChann.end())
		index=-1;

	return index;
}


int CPlantInfoBase::GetSpecAxisLocationIndex(string sLocation_)
{
	V_PAXISCHANNINFO::iterator it=_vAxisChann.begin();
	int index=-1;
	for (;it!=_vAxisChann.end();++it)
	{
	    index++;
		if (strcmp((*it)->GetChannInfo()._cLocation,sLocation_.c_str())==0)
			return index;
    }
	return -1;
}

int CPlantInfoBase::GetSpecAxisLocationIndex(string sChannHorID_,string sChannVerID_)
{
	string sChannHorNo=GetSpecVibChannInfo(GetSpecVibChannIndex(sChannHorID_))._cChannNo;
	string sChannVerNo=GetSpecVibChannInfo(GetSpecVibChannIndex(sChannVerID_))._cChannNo;
	V_PAXISCHANNINFO::iterator it=_vAxisChann.begin();
	int index=-1;
	for (;it!=_vAxisChann.end();++it)
	{
		index++;
		if ((strcmp((*it)->GetChannInfo()._cVerChann,sChannVerNo.c_str())==0)&&(strcmp((*it)->GetChannInfo()._cHorChann,sChannHorNo.c_str())==0))
			return index;
	}
	return -1;
}

int CPlantInfoBase::GetSpecDynAxisLocationIndex(string sChannHorID_,string sChannVerID_)
{
	string sChannHorNo=GetSpecDynChannInfo(GetSpecDynChannIndex(sChannHorID_))._cChannNo;
	string sChannVerNo=GetSpecDynChannInfo(GetSpecDynChannIndex(sChannVerID_))._cChannNo;
	V_PAXISCHANNINFO::iterator it=_vAxisChann.begin();
	int index=-1;
	for (;it!=_vAxisChann.end();++it)
	{
		index++;
		if ((strcmp((*it)->GetChannInfo()._cVerChann,sChannVerNo.c_str())==0)&&(strcmp((*it)->GetChannInfo()._cHorChann,sChannHorNo.c_str())==0))
			return index;
	}
	return -1;
}

int CPlantInfoBase::GetSpecChannsAxisRotation(string sChannHor_,string sChannVer_)
{
	int iRotation = -1;
	int index = GetSpecAxisLocationIndex(sChannHor_,sChannVer_);
	if (index<0)
		return -1;
	CAxisChannInfo AxisChannInfo;
	_vAxisChann[index]->GetChannInfo(AxisChannInfo);
	iRotation = AxisChannInfo._iRotateDirection;
	return iRotation;
}

int CPlantInfoBase::SubtractWaveDC(DATATYPE_WAVE *pfWave_,DATATYPE_WAVE *pfWaveOut_,int iLen_)
{
	BEGIN_ERROR_HANDLE;
	
	float fSum=.0,fAverage=.0;
	float *pfWaveData = NULL;
	float *pfWaveOut  =NULL;
	if (iLen_>0)
	{
		pfWaveData = new float[iLen_];
		pfWaveOut = new float[iLen_];
		ConvertDTToFloat(pfWaveData,pfWave_,iLen_);
		for (int nloop =0;nloop< iLen_; ++nloop)
			fSum = fSum + pfWaveData[nloop];
		fAverage = fSum/iLen_;
		for (int nloop = 0;nloop<iLen_; ++nloop)
			pfWaveOut[nloop] = pfWaveData[nloop]-fAverage;
		ConvertFloatToDT(pfWaveOut_,pfWaveOut,iLen_);
	}
	KD_DeletePoint(pfWaveData);
	KD_DeletePoint(pfWaveOut);
	
	END_ERROR_HANDLE;
	return iLen_;
}

int CPlantInfoBase::SubtractWaveDC( float *pfWave_,float *pfWaveOut_,int iLen_)
{
	BEGIN_ERROR_HANDLE;

	float fSum=.0,fAverage=.0;
	if (iLen_>0)
	{
	
		for (int nloop =0;nloop< iLen_; ++nloop)
			fSum = fSum + pfWave_[nloop];
		fAverage = fSum/iLen_;
		for (int nloop = 0;nloop<iLen_; ++nloop)
			pfWaveOut_[nloop] = pfWave_[nloop]-fAverage;
		
	}
	

	END_ERROR_HANDLE;
	return iLen_;
}
int CPlantInfoBase::GetDynPressChann(CCustomDB *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_)
{
	int index=-1;
	vector<string> vChann;
	SwichDbName(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNPRESSCHANN);
	while (pDB_->More())
	{
		vChann.push_back(pDB_->Get(gc_cChannID));
	}
	if (vChann.size()>1)
	{
		sDynChann1_=vChann[0];
		sDynChann2_=vChann[1];
		index=1;
	}
	vChann.clear();
	return index;
}

int CPlantInfoBase::GetDynStressChann(CCustomDB *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_)
{
	int index=-1;
	SwichDbName(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNSTRESSCHANN);
	if (pDB_->More())
	{
		sDynStressChann_ =pDB_->Get(gc_cChannID);
		if (!sDynStressChann_.empty())
			index=1;
	}
	else
		index=-1;
	return index;
}

int CPlantInfoBase::GetDynPressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_)
{
	int index=-1;
	vector<string> vChann;
	SwitchDbNameEx(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNPRESSCHANN);
	while (pDB_->More())
	{
		vChann.push_back(pDB_->Get(gc_cChannID));
	}
	if (vChann.size()>1)
	{
		sDynChann1_=vChann[0];
		sDynChann2_=vChann[1];
		index=1;
	}
	vChann.clear();
	return index;
}

int CPlantInfoBase::GetDynStressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_)
{
	int index=-1;
	SwitchDbNameEx(pDB_,g_strMainDbName);
	string sPlantNo=GetPlantInfo()._cPlantNo;
	pDB_->Cmd("SELECT %s FROM [%s] ",gc_cChannID,gc_cChannTable);
	pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND %s=%d AND %s=%d",gc_cCompany,sCompany_.c_str(),gc_cFactory_Name,sFactory_.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cCylinderNo,iCylinderNo_,gc_cChannType,GE_DYNSTRESSCHANN);
	if (pDB_->More())
	{
		sDynStressChann_ =pDB_->Get(gc_cChannID);
		if (!sDynStressChann_.empty())
			index=1;
	}
	else
		index=-1;
	return index;
}

const int CPlantInfoBase::GetDynChannNum() const
{
	return _sPlantInfo._iDynChannNum;
}

const int CPlantInfoBase::GetVibChannNum() const
{
	return _sPlantInfo._iVibChannNum;
}
const int CPlantInfoBase::GetProcChannNum() const
{
	return _sPlantInfo._iProcChannNum;
}

const int CPlantInfoBase::GetAxialVibChannNum()const
{
	return _iAxialVibChannNum;
}

const int CPlantInfoBase::GetAxisPairNum() const
{
	return _iAxesPairNum;
}

const int CPlantInfoBase::GetRadialVibChannNum() const
{
     return _iRadialVibChannNum;
}

const S_PlantInfo& CPlantInfoBase::GetPlantInfo() const
{
	return _sPlantInfo;
}


void CPlantInfoBase::SetDBConn(CCustomDB *pDB_)
{
		//_pDB=pDB_;
}

void CPlantInfoBase::SetDBConn(IDBInterFace *pDB_)
{
	//_pDB=pDB_;
}


void CPlantInfoBase::AddVibChannInfo(IDBInterFace *pDB_)
{
	int index=0;
	S_ChannInfo sChannInfo;
	CChannInfoBase *pVibParam=NULL;
	CChannInfoBase *pVibParamSud=NULL;
	CString strLog="";
	//设置数据处理连接
	_vVibChann.clear();
	_mapVibChannIndex.clear();
	_vAxisChann.clear();

	SwitchDbNameEx(pDB_,g_strMainDbName);
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN);

	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	strLog.Format("%s-%s-%s 添加径向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddVibChannInfo");


	while (pDB_->More()) 
	{
		pVibParam=new CChannInfoBase;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		if (sChannInfo._iSensorType==GE_SENSORTYPE_VOTEX)
		{//如果是电涡流传感器说明可以做启停车，且是径向振动
			pVibParamSud=new CChannInfoBase;
			pVibParamSud->SetRwLock(_pLockChannInfo);
			pVibParamSud->SetChannInfo(sChannInfo);
			_vSudChann.push_back(pVibParamSud);
		}

		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);

	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);

	strLog.Format("%s-%s-%s 添加轴向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CPlantInfoBase","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CChannInfoBase;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}

	if(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR&&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL)
	{
		//得到轴心位置通道信息
		CAxisChannInfoBase *axisChannInfoBase=NULL;
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfoBase","AddVibChannInfo");
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannInfoBase=new CAxisChannInfoBase;
			axisChannInfoBase->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannInfoBase->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannInfoBase);
		}
	}
	strLog.Format("%s-%s-%s 添加振动通道信息,振动数目%d,轴心位置数目%d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_vVibChann.size(),_vAxisChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddVibChannInfo");
}



void CPlantInfoBase::AddVibChannInfo(CCustomDB *pDB_)
{
	int index=0;
	S_ChannInfo sChannInfo;
	CChannInfoBase *pVibParam=NULL;
	CChannInfoBase *pVibParamSud=NULL;
	CString strLog="";
	//设置数据处理连接
	_vVibChann.clear();
	_mapVibChannIndex.clear();
	_vAxisChann.clear();

	SwichDbName(pDB_,g_strMainDbName);
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN);
	
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	strLog.Format("%s-%s-%s 添加径向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddVibChannInfo");

	
	while (pDB_->More()) 
	{
		pVibParam=new CChannInfoBase;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
	    if (sChannInfo._iSensorType==GE_SENSORTYPE_VOTEX)
		{//如果是电涡流传感器说明可以做启停车，且是径向振动
            pVibParamSud=new CChannInfoBase;
			pVibParamSud->SetRwLock(_pLockChannInfo);
			pVibParamSud->SetChannInfo(sChannInfo);
			_vSudChann.push_back(pVibParamSud);
		}

		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] =%d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_AXIALCHANN);
	
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);

	strLog.Format("%s-%s-%s 添加轴向振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CPlantInfoBase","AddVibChannInfo");

	while (pDB_->More())
	{
		pVibParam=new CChannInfoBase;
		pVibParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pVibParam->SetChannInfo(sChannInfo);
		_vVibChann.push_back(pVibParam);
		_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(_sPlantInfo._iMachineType != GE_MACHINETYPE_RC && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_PUMP &&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_PLUNGER_PUMP && _sPlantInfo._iMachineType != GE_MACHINETYPE_PISTON_COMPRESSOR&&
		_sPlantInfo._iMachineType != GE_MACHINETYPE_ICE && _sPlantInfo._iMachineType != GE_MACHINETYPE_DISEL)
	{
		//得到轴心位置通道信息
		CAxisChannInfoBase *axisChannInfoBase=NULL;
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置振动通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfoBase","AddVibChannInfo");
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannInfoBase=new CAxisChannInfoBase;
			axisChannInfoBase->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannInfoBase->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannInfoBase);
		}
	}
	strLog.Format("%s-%s-%s 添加振动通道信息,振动数目%d,轴心位置数目%d",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID,_vVibChann.size(),_vAxisChann.size());
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddVibChannInfo");
}

void CPlantInfoBase::AddDynChannInfo(CCustomDB *pDB_)
{
    int index=0;
	S_ChannInfo sChannInfo;
	CChannInfoBase *pDynParam=NULL;
	CString strLog="";
	//设置数据处理连接
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	SwichDbName(pDB_,g_strMainDbName);
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	strLog.Format("%s-%s-%s 添加动态通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddDynChannInfo");

	while (pDB_->More())
	{
		pDynParam=new CChannInfoBase;
		pDynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pDynParam->SetChannInfo(sChannInfo);
		_vDynChann.push_back(pDynParam);
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{
		//得到轴心位置通道信息
		CAxisChannInfoBase *axisChannInfoBase=NULL;
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置沉降量通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfoBase","AddDynChannInfo");
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannInfoBase=new CAxisChannInfoBase;
			axisChannInfoBase->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannInfoBase->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannInfoBase);
		}
	}
}


void CPlantInfoBase::AddDynChannInfo(IDBInterFace *pDB_)
{
	int index=0;
	S_ChannInfo sChannInfo;
	CChannInfoBase *pDynParam=NULL;
	CString strLog="";
	//设置数据处理连接
	_vDynChann.clear();
	_mapDynChannIndex.clear();
	SwitchDbNameEx(pDB_,g_strMainDbName);
	//振动通道
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	index=0;

	strLog.Format("%s-%s-%s 添加动态通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Debug(strLog,"CPlantInfoBase","AddDynChannInfo");

	while (pDB_->More())
	{
		pDynParam=new CChannInfoBase;
		pDynParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		pDynParam->SetChannInfo(sChannInfo);
		_vDynChann.push_back(pDynParam);
		_mapDynChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
	if(GetPlantInfo()._iMachineType == GE_MACHINETYPE_RC || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		GetPlantInfo()._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || GetPlantInfo()._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR)
	{
		//得到轴心位置通道信息
		CAxisChannInfoBase *axisChannInfoBase=NULL;
		S_AxisData axisData;
		axisData._bCalc=false;
		pDB_->Cmd("SELECT * from [%s] ",gc_cPlantAxisTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);

		strLog.Format("%s-%s-%s 添加轴心位置沉降量通道信息",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
		CHZLogManage::Info(strLog,"CPlantInfoBase","AddDynChannInfo");
		CAxisChannInfo axisChannInfo;
		while (pDB_->More())
		{
			axisChannInfoBase=new CAxisChannInfoBase;
			axisChannInfoBase->SetRwLock(_pLockChannInfo);
			axisChannInfo.GetAxisChannInfo(pDB_);
			axisChannInfoBase->SetChannInfo(axisChannInfo);
			_vAxisChann.push_back(axisChannInfoBase);
		}
	}
}


void CPlantInfoBase::AddProcChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	CChannInfoBase *procParam;
	int index=0;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	S_ChannInfo sChannInfo;
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" AND ( t1.[%s] != %d) ",gc_cData_Source,GE_DATA_SOURCE_OFFLINE);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CChannInfoBase;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
}



void CPlantInfoBase::AddProcChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	CChannInfoBase *procParam;
	int index=0;
	_vProcChann.clear();
	_mapProcChannIndex.clear();
	S_ChannInfo sChannInfo;
	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s' AND t1.[%s] in (%d,%d,%d,%d,%d,%d)",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
	pDB_->Cmd(" AND (t1.[%s]=t2.[%s] OR t2.[%s] is null) ", gc_cCompany, gc_cCompany,gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		procParam=new CChannInfoBase;
		procParam->SetRwLock(_pLockChannInfo);
		sChannInfo.GetChannInfo(pDB_);
		procParam->SetChannInfo(sChannInfo);
		_vProcChann.push_back(procParam);
		_mapProcChannIndex.insert(make_pair(sChannInfo._cChannID,index));
		++index;
	}
}


void CPlantInfoBase::AddOfflineChannInfo(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	_vOfflineChann.clear();

	S_ChannInfo sChannInfo;
	COfflineChannParam *pParam;

	string sDataSrcCol    = gc_cData_Source;
	string sChannTbl      = gc_cChannTable;
	bool bExistDataSrcCol = DB_IsColumnInTable(pDB_, sDataSrcCol, sChannTbl);

	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN  [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s'",
		gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo);
	pDB_->Cmd(" AND t1.[%s]=t2.[%s] ", gc_cCompany, gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	if(bExistDataSrcCol){
		pDB_->Cmd(" AND t1.[%s]=%d",
			gc_cData_Source, GE_DATA_SOURCE_OFFLINE);
	}

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		pParam=new COfflineChannParam;

		pParam->SetRwLock(_pLockChannInfo);

		sChannInfo.GetChannInfo(pDB_);
		pParam->SetChannInfo(sChannInfo);

		_vOfflineChann.push_back(pParam);
	}

	for(V_PCHANNINFO::iterator it = _vOfflineChann.begin();
		it != _vOfflineChann.end();
		++it)
	{
		pParam = (COfflineChannParam *)(*it);
		sChannInfo = pParam->GetChannInfo();

		string offine_table_name = "TREND_OFFLINE_";
		string database_name     = _sCompanyAlias + _sPlantInfo._cPlantNo;
		offine_table_name.append(database_name);

		SwichDbName(pDB_, database_name.c_str());

		pDB_->Cmd("SELECT TOP 1 * FROM %s WHERE %s='%s' ORDER BY [%s] desc",
			offine_table_name.c_str(),
			gc_cChannNo, sChannInfo._cChannNo,
			gc_cDateTime);
		float charValue = 0;

		if(pDB_->More())
		{
			CBHDateTime dtTime(pDB_->GetTime(gc_cDateTime));
			pParam->SetData( dtTime,pDB_->GetDouble(gc_cAll));
		}

	}

	SwichDbName(pDB_,g_strMainDbName);
}



void CPlantInfoBase::AddOfflineChannInfo(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,g_strMainDbName);
	_vOfflineChann.clear();

	S_ChannInfo sChannInfo;
	COfflineChannParam *pParam;

	string sDataSrcCol    = gc_cData_Source;
	string sChannTbl      = gc_cChannTable;
	bool bExistDataSrcCol = DB_IsColumnInTable(pDB_, sDataSrcCol, sChannTbl);

	pDB_->Cmd("SELECT ISNULL(t1.[%s],-3) as [%s],* from [%s] t1",gc_cLocation_IDX,gc_cLocation_IDX,gc_cChannTable);
	pDB_->Cmd(" LEFT JOIN  [%s] t2 ",gc_cLocation_Table);
	pDB_->Cmd(" ON t1.[%s] = t2.[%s] ",gc_cLocation_IDX,gc_cLocation_IDX);
	pDB_->Cmd(" WHERE t1.[%s]='%s' AND t1.[%s]='%s'",
		gc_cCompany,_sCompany.c_str(),
		gc_cPlantNo,_sPlantInfo._cPlantNo);
	pDB_->Cmd(" AND t1.[%s]=t2.[%s] ", gc_cCompany, gc_cCompany);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cPlantNo);
	pDB_->Cmd(" AND ( t2.[%s] ='%s' OR  t2.[%s] is null) ",gc_cFactory_Name,_sFactory.c_str(),gc_cFactory_Name);
	if(bExistDataSrcCol){
		pDB_->Cmd(" AND t1.[%s]=%d",
			gc_cData_Source, GE_DATA_SOURCE_OFFLINE);
	}

	pDB_->Cmd(" ORDER BY t1.[%s] ",gc_cID);
	while (pDB_->More())
	{
		pParam=new COfflineChannParam;

		pParam->SetRwLock(_pLockChannInfo);

		sChannInfo.GetChannInfo(pDB_);
		pParam->SetChannInfo(sChannInfo);

		_vOfflineChann.push_back(pParam);
	}

	for(V_PCHANNINFO::iterator it = _vOfflineChann.begin();
		it != _vOfflineChann.end();
		++it)
	{
		pParam = (COfflineChannParam *)(*it);
		sChannInfo = pParam->GetChannInfo();

		string offine_table_name = "TREND_OFFLINE_";
		string database_name     = _sCompanyAlias + _sPlantInfo._cPlantNo;
		offine_table_name.append(database_name);

		SwitchDbNameEx(pDB_, database_name.c_str());

		pDB_->Cmd("SELECT TOP 1 * FROM %s WHERE %s='%s' ORDER BY [%s] desc",
			offine_table_name.c_str(),
			gc_cChannNo, sChannInfo._cChannNo,
			gc_cDateTime);
		float charValue = 0;

		if(pDB_->More())
		{
			pParam->SetData(pDB_->GetTime(gc_cDateTime) ,pDB_->GetDouble(gc_cAll));
		}

	}

	SwitchDbNameEx(pDB_,g_strMainDbName);
}



CAxisChannInfo CPlantInfoBase::GetSpecLocationAxisChannInfo(string sLocation_)
{
	CAxisChannInfo channInfo;
	V_PAXISCHANNINFO::iterator it;
	for (it=_vAxisChann.begin();it!=_vAxisChann.end();++it)
	{
		if(strcmp((*it)->GetChannInfo()._cLocation,sLocation_.c_str())==0)
		{
			(*it)->GetChannInfo(channInfo);
			return channInfo;
		}
	}
	return channInfo;
}

CAxisChannInfo CPlantInfoBase::GetSpecDynChannAxisChannInfo(string sChannHor_,string sChannVer_)
{
	CAxisChannInfo channInfo;
	int index=GetSpecDynAxisLocationIndex(sChannHor_,sChannVer_);
	if (index<0)
		return channInfo;
	_vAxisChann[index]->GetChannInfo(channInfo);
	return channInfo;
}

void CPlantInfoBase::AddChannInfoEnd()
{
	_sPlantInfo._iVibChannNum = _vVibChann.size();
	_sPlantInfo._iProcChannNum= _vProcChann.size();
	_sPlantInfo._iDynChannNum = _vDynChann.size();
	_iSudChannNum=_vSudChann.size();
    _iAxesPairNum=_vAxisChann.size();
	V_PCHANNINFO::iterator it;
	_iRadialVibChannNum=0;
	_iAxialVibChannNum=0;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{

		if (((*it)->GetChannInfo())._iChannType==GE_VIBCHANN)
			++_iRadialVibChannNum;
		else
            ++_iAxialVibChannNum;
	}

	// 设置特征值信息

	return;
}
void CPlantInfoBase::SetPlantInfo(string sGroup_,string sCompany_,string sCompanyAlias_,string sFactory_,string sSet_,const S_PlantInfo &sPlantInfo_)
{
	_sGroup=sGroup_;
	_sCompany=sCompany_;
	_sCompanyAlias=sCompanyAlias_;
	_sFactory=sFactory_;
	_sSetID=sSet_;
	_sPlantInfo=sPlantInfo_;
	if (sPlantInfo_._iSmpNum < 2048)
	{
		_sPlantInfo._iSmpNum = 2048;
	}

	string sSuffix=sCompanyAlias_;
	sSuffix.append(sPlantInfo_._cPlantNo);

	//构造表名
	_sAlarmProcDataTblName="Alarm_PROC_"+sSuffix;
	_sAlarmVibDataTblName="Alarm_VIB_"+sSuffix;
	_sAlarmDynDataTblName="Alarm_DYN_"+sSuffix;
	_sAlarmTrendDataTblName="Alarm_TREND_"+sSuffix;
	_sAlarmAxisDataTblName="Alarm_AXIS_"+sSuffix;
	_sAlarmLogTblName="Alarm_LOG_"+sSuffix;

	_sSudLogTblName="SUD_LOG_"+sSuffix;
	_sSudDataTblName="SUD_"+sSuffix;
	_sSudDynDataTblName="SUD_DYN_"+sSuffix;
	_sSudProcDataTblName="SUD_PROC_"+sSuffix;

	_sThTblName="THRESHOLD_"+sSuffix;

	_sVibTrendDayTblName   = "TREND_VIBDAY_" + sSuffix;
	_sVibTrendMonthTblName = "TREND_VIBMONTH_" + sSuffix;
	_sVibTrendSecTblName   = "TREND_VIBSECOND_" + sSuffix;
	_sVibTrendHourTblName  = "TREND_VIBHOUR_" + sSuffix;

	_sProcTrendDayTblName  = "TREND_ProcDAY_" + sSuffix;
	_sProcTrendMonthTblName= "TREND_PROCMONTH_" + sSuffix;
	_sProcTrendSecTblName  = "TREND_PROCSECOND_" + sSuffix;
	_sProcTrendHourTblName = "TREND_PROCHOUR_" + sSuffix;

	_sDynTrendDayTblName   = "TREND_DYNDAY_" + sSuffix;
	_sDynTrendMonthTblName = "TREND_DYNMONTH_" + sSuffix;
	_sDynTrendSecTblName   = "TREND_DYNSECOND_" + sSuffix;
	_sDynTrendHourTblName  = "TREND_DYNHOUR_" + sSuffix;

	_sVibTrendFastTblName  = "TREND_VIBFAST_" + sSuffix;
	_sDynTrendFastTblName  = "TREND_DYNFAST_" + sSuffix;
	_sProcTrendFastTblName = "TREND_PROCFAST_" + sSuffix;
}

void CPlantInfoBase::SetCharInfo(IDBInterFace *pDB_)
{
	CChannInfoBase * pChannParam = NULL;

	for(int i = 0; i < _vVibChann.size(); i++)
	{
		pChannParam = _vVibChann[i];

		pChannParam->SetCharInfo(pDB_);
	}

	for(int i = 0; i < _vDynChann.size(); i++)
	{
		pChannParam = _vDynChann[i];

		pChannParam->SetCharInfo(pDB_);
	}
}

void CPlantInfoBase::SetCharInfo(CCustomDB *pDB_)
{
	CChannInfoBase * pChannParam = NULL;

	for(int i = 0; i < _vVibChann.size(); i++)
	{
		pChannParam = _vVibChann[i];

		pChannParam->SetCharInfo(pDB_);
	}

	for(int i = 0; i < _vDynChann.size(); i++)
	{
		pChannParam = _vDynChann[i];

		pChannParam->SetCharInfo(pDB_);
	}
}

const string CPlantInfoBase::GetCompanyName() const
{
	return _sCompany;
}

const string CPlantInfoBase::GetCompanyAlias() const
{
	return _sCompanyAlias;
}

const string CPlantInfoBase::GetFactoryName() const
{
	return _sFactory;
}

const string CPlantInfoBase::GetSetID() const
{
	return _sSetID;
}

const string CPlantInfoBase::GetPlantID() const
{
	return _sPlantInfo._cPlantID;
}

const string CPlantInfoBase::GetPlantNo() const
{
	return _sPlantInfo._cPlantNo;
}

const string CPlantInfoBase::GetAlarmLogTblName() const
{
	return _sAlarmLogTblName;
}

const string CPlantInfoBase::GetSudLogTblName() const
{
	return _sSudLogTblName;
}

const string CPlantInfoBase::GetAlarmAxisTblName() const
{
	return _sAlarmAxisDataTblName;
}

const string CPlantInfoBase::GetThTblName() const
{
	return _sThTblName;
}

const string CPlantInfoBase::GetGroupName() const
{
	return _sGroup;
}

bool CPlantInfoBase::CheckPlant(string sCompany_,string sFactory_,string sPlant_)
{
	if ((sCompany_==_sCompany)&&(sPlant_== _sPlantInfo._cPlantID))
		return true;
	else
		return false;
}


BOOL CPlantInfoBase::DeleteVibTrendRecord( IDBInterFace *pDB_,int nTotalWavePtNum_ )
{
	//return FALSE;

	pDB_->Cmd("select count(*) as RecordCount from sysfiles");
	if(pDB_->More())
	{
		int lnRecordCount = pDB_->GetLong("RecordCount");

		if(lnRecordCount >= 3)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	int lnRecordID = -1;

	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;

	for (int n = 0; n < _vVibChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}
	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}


	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sVibTrendSecTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sVibTrendSecTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}

			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendSecTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sVibTrendSecTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vVibChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sVibTrendSecTblName,lnTotalDataSmpNum,nTotalWavePtNum_);
					
				}
			}
		
		}
	}

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sVibTrendHourTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sVibTrendHourTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendHourTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sVibTrendHourTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vVibChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sVibTrendHourTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

				}
			}
		}
	}


	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sVibTrendDayTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sVibTrendDayTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendDayTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sVibTrendDayTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vVibChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sVibTrendDayTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

				}
			}
		}
	}


	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sVibTrendFastTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		
		pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sVibTrendFastTblName.c_str());
		if(pDB_->More())
		{
			lnRecordID = pDB_->GetLong("RecordID");
		}
		if(nTotalWavePtNum_ <= 0)
		{
			if(lnRecordID > 0)
			{
				pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendFastTblName.c_str(),gc_cID,lnRecordID);
				pDB_->Exec();
				return TRUE;
			}
		}
		else
		{
			pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sVibTrendFastTblName.c_str(),gc_cID,lnRecordID);
			if(pDB_->More())
			{
				for (int n = 0; n < _vVibChann.size(); ++n)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
					lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
				}

				return DeleteSpecCountTrendRecord(pDB_,_sVibTrendFastTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

			}
		}
		
	}


	if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
	{
		pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sVibTrendMonthTblName.c_str(),gc_cID);
		if(pDB_->More())
		{
			if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
			{
				pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sVibTrendMonthTblName.c_str());
				if(pDB_->More())
				{
					lnRecordID = pDB_->GetLong("RecordID");
				}
				if(nTotalWavePtNum_ <= 0)
				{
					if(lnRecordID > 0)
					{
						pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendMonthTblName.c_str(),gc_cID,lnRecordID);
						pDB_->Exec();
						return TRUE;
					}
				}
				else
				{
					pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sVibTrendMonthTblName.c_str(),gc_cID,lnRecordID);
					if(pDB_->More())
					{
						for (int n = 0; n < _vVibChann.size(); ++n)
						{
							CString lstrColumn = "SAMPLE_NUM_";
							lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
							lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
						}

						return DeleteSpecCountTrendRecord(pDB_,_sVibTrendMonthTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

					}
				}
			}
		}
	}
	
	return FALSE;
}

BOOL CPlantInfoBase::DeleteDynTrendRecord( IDBInterFace *pDB_,int nTotalWavePtNum_ )
{

	//return FALSE;

	pDB_->Cmd("select count(*) as RecordCount from sysfiles");
	if(pDB_->More())
	{
		int lnRecordCount = pDB_->GetLong("RecordCount");

		if(lnRecordCount >= 3)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	
	int lnRecordID = -1;

	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;

	for (int n = 0; n < _vDynChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}
	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sDynTrendSecTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sDynTrendSecTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendSecTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sDynTrendSecTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vDynChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sDynTrendSecTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

				}
			}
		}
	}
	
	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sDynTrendHourTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sDynTrendHourTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendHourTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sDynTrendHourTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vDynChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sDynTrendHourTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

				}
			}
		}
	}
	

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sDynTrendDayTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sDynTrendDayTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(nTotalWavePtNum_ <= 0)
			{
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendDayTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
			else
			{
				pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sDynTrendDayTblName.c_str(),gc_cID,lnRecordID);
				if(pDB_->More())
				{
					for (int n = 0; n < _vDynChann.size(); ++n)
					{
						CString lstrColumn = "SAMPLE_NUM_";
						lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
						lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
					}

					return DeleteSpecCountTrendRecord(pDB_,_sDynTrendDayTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

				}
			}

		}
	}
	
	pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sDynTrendFastTblName.c_str());
	if(pDB_->More())
	{
		lnRecordID = pDB_->GetLong("RecordID");
	}
	if(nTotalWavePtNum_ <= 0)
	{
		if(lnRecordID > 0)
		{
			pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendFastTblName.c_str(),gc_cID,lnRecordID);
			pDB_->Exec();
			return TRUE;
		}
	}
	else
	{
		pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sDynTrendFastTblName.c_str(),gc_cID,lnRecordID);
		if(pDB_->More())
		{
			for (int n = 0; n < _vDynChann.size(); ++n)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
				lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
			}

			return DeleteSpecCountTrendRecord(pDB_,_sDynTrendFastTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

		}
	}

	if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
	{
		pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sDynTrendMonthTblName.c_str(),gc_cID);
		if(pDB_->More())
		{
			if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
			{
				pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sDynTrendMonthTblName.c_str());
				if(pDB_->More())
				{
					lnRecordID = pDB_->GetLong("RecordID");
				}
				if(nTotalWavePtNum_ <= 0)
				{
					if(lnRecordID > 0)
					{
						pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendMonthTblName.c_str(),gc_cID,lnRecordID);
						pDB_->Exec();
						return TRUE;
					}
				}
				else
				{
					pDB_->Cmd("select %s from %s where %s = %d ",lstrFieldSel,_sDynTrendMonthTblName.c_str(),gc_cID,lnRecordID);
					if(pDB_->More())
					{
						for (int n = 0; n < _vDynChann.size(); ++n)
						{
							CString lstrColumn = "SAMPLE_NUM_";
							lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
							lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
						}

						return DeleteSpecCountTrendRecord(pDB_,_sDynTrendMonthTblName,lnTotalDataSmpNum,nTotalWavePtNum_);

					}
				}
			}
		}
		
	}
	

	return FALSE;
}

BOOL CPlantInfoBase::DeleteProcTrendRecord( IDBInterFace *pDB_ )
{

	//return FALSE;

	pDB_->Cmd("select count(*) as RecordCount from sysfiles");
	if(pDB_->More())
	{
		int lnRecordCount = pDB_->GetLong("RecordCount");

		if(lnRecordCount >= 3)
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	int lnRecordID = -1;

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sProcTrendSecTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sProcTrendSecTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(lnRecordID > 0)
			{
				pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendSecTblName.c_str(),gc_cID,lnRecordID);
				pDB_->Exec();
				return TRUE;
			}
		}
	}


	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sProcTrendHourTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sProcTrendHourTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(lnRecordID > 0)
			{
				pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendHourTblName.c_str(),gc_cID,lnRecordID);
				pDB_->Exec();
				return TRUE;
			}
		}
	}


	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sProcTrendDayTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
		{
			pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sProcTrendDayTblName.c_str());
			if(pDB_->More())
			{
				lnRecordID = pDB_->GetLong("RecordID");
			}
			if(lnRecordID > 0)
			{
				pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendDayTblName.c_str(),gc_cID,lnRecordID);
				pDB_->Exec();
				return TRUE;
			}
		}		
	}


	pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sProcTrendFastTblName.c_str());
	if(pDB_->More())
	{
		lnRecordID = pDB_->GetLong("RecordID");
	}
	if(lnRecordID > 0)
	{
		pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendFastTblName.c_str(),gc_cID,lnRecordID);
		pDB_->Exec();
		return TRUE;
	}

	if(CConfigHelper::GetMonthTableUpdateFlag() > 0)
	{
		pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,_sProcTrendMonthTblName.c_str(),gc_cID);
		if(pDB_->More())
		{
			if(pDB_->GetLong("RecordCount") > D_TREND_DATA_LIMIT/3)
			{
				pDB_->Cmd("select max([%s]) as RecordID from [%s]",gc_cID,_sProcTrendMonthTblName.c_str());
				if(pDB_->More())
				{
					lnRecordID = pDB_->GetLong("RecordID");
				}
				if(lnRecordID > 0)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendMonthTblName.c_str(),gc_cID,lnRecordID);
					pDB_->Exec();
					return TRUE;
				}
			}
		}

	}


	return FALSE;
}

BOOL CPlantInfoBase::DeleteVibAlarmRecord( IDBInterFace *pDB_,int iEventID_,int nTotalWavePtNum_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;

	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;

	for (int n = 0; n < _vVibChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}
	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sAlarmVibDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		if(nTotalWavePtNum_ <= 0)
		{
			pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sAlarmVibDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
			if(pDB_->Exec())
			{
				pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sAlarmVibDataTblName.c_str(),gc_cEventID,lnEventID);
				if(!pDB_->More() && iEventID_ != lnEventID)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d and [%s] in(%d,%d)",_sAlarmLogTblName.c_str(),gc_cEventID,lnEventID,gc_cChannType,GE_VIBCHANN,GE_AXIALCHANN);
					return (pDB_->Exec());
				}
			}
		}
		else
		{
			pDB_->Cmd("select %s from %s where %s = %d and %s = '%s'",lstrFieldSel,_sAlarmVibDataTblName.c_str(),gc_cEventID,lnEventID,gc_cDateTime,ltTime.Format());
			if(pDB_->More())
			{
				for (int n = 0; n < _vVibChann.size(); ++n)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
					lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
				}

				DeleteSpecCountTrendRecord(pDB_,_sAlarmVibDataTblName,lnTotalDataSmpNum,nTotalWavePtNum_,iEventID_);
			}
		}
		

	}
	return TRUE;
}

BOOL CPlantInfoBase::DeleteDynAlarmRecord( IDBInterFace *pDB_,int iEventID_,int nTotalWavePtNum_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;

	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;

	for (int n = 0; n < _vDynChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}
	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sAlarmDynDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		if(nTotalWavePtNum_ <= 0)
		{
			pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sAlarmDynDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
			if(pDB_->Exec())
			{
				pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sAlarmDynDataTblName.c_str(),gc_cEventID,lnEventID);
				if(!pDB_->More() && iEventID_ != lnEventID)
				{
					pDB_->Cmd("delete from [%s] where [%s] = %d and [%s] in(%d,%d,%d)",_sAlarmLogTblName.c_str(),gc_cEventID,lnEventID,gc_cChannType,GE_DYNPRESSCHANN,GE_DYNSTRESSCHANN,GE_RODSINKCHANN);
					return (pDB_->Exec());
				}
			}
		}
		else
		{
			pDB_->Cmd("select %s from %s where %s = %d and %s = '%s'",lstrFieldSel,_sAlarmDynDataTblName.c_str(),gc_cEventID,lnEventID,gc_cDateTime,ltTime.Format());
			if(pDB_->More())
			{
				for (int n = 0; n < _vDynChann.size(); ++n)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
					lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
				}

				DeleteSpecCountTrendRecord(pDB_,_sAlarmDynDataTblName,lnTotalDataSmpNum,nTotalWavePtNum_,iEventID_);
			}
		}


	}
	return TRUE;
}

BOOL CPlantInfoBase::DeleteProcAlarmRecord( IDBInterFace *pDB_,int iEventID_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;
	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sAlarmProcDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sAlarmProcDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
		if(pDB_->Exec())
		{
			pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sAlarmProcDataTblName.c_str(),gc_cEventID,lnEventID);
			if(!pDB_->More() && iEventID_ != lnEventID)
			{
				pDB_->Cmd("delete from [%s] where [%s] = %d and [%s] in (%d,%d,%d,%d,%d,%d)",_sAlarmLogTblName.c_str(),gc_cEventID,lnEventID,gc_cChannType,GE_PRESSCHANN,GE_TEMPCHANN,GE_FLUXCHANN,GE_OTHERCHANN,GE_IMPACTCHANN,GE_CURRENTCHANN);
				return (pDB_->Exec());
			}
		}

	}
	return TRUE;
}

BOOL CPlantInfoBase::DeleteSudVibRecord( IDBInterFace *pDB_,int iEventID_,const vector<CString>& vChannNo_,int nTotalWavePtNum_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;


	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;


	for (int n = 0; n < vChannNo_.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(vChannNo_[n]);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}

	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sSudDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		if(nTotalWavePtNum_ <= 0)
		{
			pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sSudDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
			if(pDB_->Exec())
			{
				pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDataTblName.c_str(),gc_cEventID,lnEventID);
				if(!pDB_->More())
				{
					pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDynDataTblName.c_str(),gc_cEventID,lnEventID);

					if(!pDB_->More())
					{
						pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudProcDataTblName.c_str(),gc_cEventID,lnEventID);

						if(!pDB_->More() && iEventID_ != lnEventID)
						{
							pDB_->Cmd("delete from [%s] where [%s] = %d",_sSudLogTblName.c_str(),gc_cEventID,lnEventID);
							return (pDB_->Exec());
						}
					}

				}

			}

		}
		else
		{
			pDB_->Cmd("select %s from %s where %s = %d and %s = '%s'",lstrFieldSel,_sSudDataTblName.c_str(),gc_cEventID,lnEventID,gc_cDateTime,ltTime.Format());
			if(pDB_->More())
			{

				for (int n = 0; n < vChannNo_.size(); ++n)
				{
					CString lstrColumn = "SAMPLE_NUM_";
					lstrColumn.Append(vChannNo_[n]);
					lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
				}

				DeleteSpecCountTrendRecord(pDB_,_sSudDataTblName,lnTotalDataSmpNum,nTotalWavePtNum_,iEventID_);
			}
		}

	}
	return TRUE;
}

BOOL CPlantInfoBase::DeleteSudDynRecord( IDBInterFace *pDB_,int iEventID_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;
	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sSudDynDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sSudDynDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
		if(pDB_->Exec())
		{
			pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDynDataTblName.c_str(),gc_cEventID,lnEventID);

			if(!pDB_->More())
			{
				pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDataTblName.c_str(),gc_cEventID,lnEventID);

				if(!pDB_->More())
				{
					pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudProcDataTblName.c_str(),gc_cEventID,lnEventID);

					if(!pDB_->More() && iEventID_ != lnEventID)
					{
						pDB_->Cmd("delete from [%s] where [%s] = %d",_sSudLogTblName.c_str(),gc_cEventID,lnEventID);
						return (pDB_->Exec());
					}
				}
			}
			
		}

	}
	return TRUE;
}

BOOL CPlantInfoBase::DeleteSudProcRecord( IDBInterFace *pDB_,int iEventID_ )
{
	CBHDateTime ltTime;
	int lnEventID = 0;
	pDB_->Cmd("select top 1 [%s],[%s] from [%s] order by [%s] asc",gc_cTime,gc_cEventID,_sSudProcDataTblName.c_str(),gc_cTime);
	if(pDB_->More())
	{
		ltTime = pDB_->GetTime(gc_cTime);
		lnEventID = pDB_->GetLong(gc_cEventID);
		pDB_->Cmd("delete from [%s] where [%s] = '%s' and [%s] = %d",_sSudProcDataTblName.c_str(),gc_cTime,ltTime.Format(),gc_cEventID,lnEventID);
		if(pDB_->Exec())
		{
			pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudProcDataTblName.c_str(),gc_cEventID,lnEventID);
			
			if(!pDB_->More())
			{
				pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDataTblName.c_str(),gc_cEventID,lnEventID);

				if(!pDB_->More())
				{
					pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,_sSudDynDataTblName.c_str(),gc_cEventID,lnEventID);

					if(!pDB_->More() && iEventID_ != lnEventID)
					{
						pDB_->Cmd("delete from [%s] where [%s] = %d",_sSudLogTblName.c_str(),gc_cEventID,lnEventID);
						return (pDB_->Exec());
					}
				}
			}

			
		}

	}
	return TRUE;
}


int CPlantInfoBase::MakeSureVibSaveSpaceSufficient( IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iCurrID_,int iStartIndex_ )
{

	CString lstrFieldSel;
	int lnTotalDataSmpNum = 0;

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,sTableName_.c_str(),gc_cID);
	if(pDB_->More())
	{
		int lnRecordCount = pDB_->GetLong("RecordCount");
		CString lstrTable = sTableName_.c_str();
		if(lstrTable.MakeUpper().Find("SECOND") >= 0)
		{
			if(lnRecordCount <= D_TREND_DATA_LIMIT)
			{
				return 1;
			}
		}
		else if(lstrTable.MakeUpper().Find("FAST") >= 0)
		{
			if(lnRecordCount <= 1)
			{
				return 1;
			}
		}
		else
		{
			if(lnRecordCount <= D_TREND_DATA_LIMIT/3)
			{
				return 1;
			}
		}
	}

	for (int n = 0; n < _vVibChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}
	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	int lnCurrID = iCurrID_;

	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s >= %d ORDER BY %s ASC",gc_cID,sTableName_.c_str(),gc_cID,iCurrID_,gc_cID);
	if(pDB_->More())
	{
		lnCurrID = pDB_->GetLong(gc_cID);
	}

	if(iStartIndex_ == 0)
	{
		pDB_->Cmd("SELECT %s FROM [%s] WHERE %s = %d",lstrFieldSel,sTableName_.c_str(),gc_cID,lnCurrID);
		if(pDB_->More())
		{
			for (int n = 0; n < _vVibChann.size(); ++n)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
				int lnSmpNum = pDB_->GetLong(lstrColumn);

				lnTotalDataSmpNum += lnSmpNum;
			}
		}
	}
	else
	{
		pDB_->Cmd("SELECT TOP 1 %s FROM [%s] WHERE %s > %d ORDER BY %s ASC",lstrFieldSel,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		if(pDB_->More())
		{
			for (int n = 0; n < _vVibChann.size(); ++n)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(_vVibChann[n]->_channInfo._cChannNo);
				int lnSmpNum = pDB_->GetLong(lstrColumn);

				lnTotalDataSmpNum += lnSmpNum;
			}
		}
	}
	

	int lnDataCount = 0;


	if(lnTotalDataSmpNum <= 0)
	{
		lnDataCount = 1;
	}
	else
	{
		if(iTotalWavePtNum_%lnTotalDataSmpNum == 0)
		{
			lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum;
		}
		else
		{
			int lnLeftNum = iTotalWavePtNum_ - (iTotalWavePtNum_/lnTotalDataSmpNum)*lnTotalDataSmpNum;

			if(lnLeftNum*1.0 > 0.1*lnTotalDataSmpNum ||  lnTotalDataSmpNum > iTotalWavePtNum_)
			{
				lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum + 1;
			}
			else
			{
				lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum;
			}
		}
	}


	CString lstrDataID;

	if(iStartIndex_ == 0)
	{
		pDB_->Cmd("SELECT TOP %d %s FROM %s WHERE %s >= %d ORDER BY %s ASC ",lnDataCount,gc_cID,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		while(pDB_->More())
		{
			lstrDataID.AppendFormat("%d,",pDB_->GetLong(gc_cID));
		}
	}
	else
	{
		pDB_->Cmd("SELECT TOP %d %s FROM %s WHERE %s > %d ORDER BY %s ASC ",lnDataCount,gc_cID,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		while(pDB_->More())
		{
			lstrDataID.AppendFormat("%d,",pDB_->GetLong(gc_cID));
		}
	}
	

	if(lstrDataID.GetLength() > 0)
	{
		lstrDataID.Delete(lstrDataID.GetLength() - 1);
		pDB_->Cmd("DELETE FROM %s WHERE %s IN(%s)",sTableName_.c_str(),gc_cID,lstrDataID);
		if(!pDB_->Exec())
		{
			return 0;
		}
	}


	return iCurrID_;
}

int CPlantInfoBase::MakeSureDynSaveSpaceSufficient( IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iCurrID_,int iStartIndex_ )
{

	CString lstrFieldSel;

	int lnTotalDataSmpNum = 0;

	pDB_->Cmd("select count(%s) as RecordCount from %s where %s >= 0",gc_cID,sTableName_.c_str(),gc_cID);
	if(pDB_->More())
	{
		int lnRecordCount = pDB_->GetLong("RecordCount");
		CString lstrTable = sTableName_.c_str();
		if(lstrTable.MakeUpper().Find("SECOND") >= 0)
		{
			if(lnRecordCount <= D_TREND_DATA_LIMIT)
			{
				return 1;
			}
		}
		else if(lstrTable.MakeUpper().Find("FAST") >= 0)
		{
			if(lnRecordCount <= 1)
			{
				return 1;
			}
		}
		else
		{
			if(lnRecordCount <= D_TREND_DATA_LIMIT/3)
			{
				return 1;
			}
		}
	}

	for (int n = 0; n < _vDynChann.size(); ++n)
	{
		CString lstrColumn = "SAMPLE_NUM_";
		lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);

		lstrFieldSel.Append(lstrColumn);
		lstrFieldSel.Append(",");
	}

	if(lstrFieldSel.GetLength() > 0)
	{
		lstrFieldSel.Delete(lstrFieldSel.GetLength() - 1);
	}

	int lnCurrID = iCurrID_;

	pDB_->Cmd("SELECT TOP 1 %s FROM %s WHERE %s >= %d ORDER BY %s ASC",gc_cID,sTableName_.c_str(),gc_cID,iCurrID_,gc_cID);
	if(pDB_->More())
	{
		lnCurrID = pDB_->GetLong(gc_cID);
	}

	if(iStartIndex_ == 0)
	{
		pDB_->Cmd("SELECT %s FROM [%s] WHERE %s = %d",lstrFieldSel,sTableName_.c_str(),gc_cID,lnCurrID);
		if(pDB_->More())
		{
			for (int n = 0; n < _vDynChann.size(); ++n)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
				lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
			}
		}
	}
	else
	{
		pDB_->Cmd("SELECT TOP 1 %s FROM [%s] WHERE %s > %d ORDER BY %s ASC",lstrFieldSel,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		if(pDB_->More())
		{
			for (int n = 0; n < _vDynChann.size(); ++n)
			{
				CString lstrColumn = "SAMPLE_NUM_";
				lstrColumn.Append(_vDynChann[n]->_channInfo._cChannNo);
				lnTotalDataSmpNum += pDB_->GetLong(lstrColumn);
			}
		}
	}
	

	int lnDataCount = 0;


	if(lnTotalDataSmpNum <= 0)
	{
		lnDataCount = 1;
	}
	else
	{
		if(iTotalWavePtNum_%lnTotalDataSmpNum == 0)
		{
			lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum;
		}
		else
		{
			int lnLeftNum = iTotalWavePtNum_ - (iTotalWavePtNum_/lnTotalDataSmpNum)*lnTotalDataSmpNum;

			if(lnLeftNum*1.0 > 0.1*lnTotalDataSmpNum || iTotalWavePtNum_ < lnTotalDataSmpNum)
			{
				lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum + 1;
			}
			else
			{
				lnDataCount = iTotalWavePtNum_/lnTotalDataSmpNum;
			}
		}
	}


	CString lstrDataID;

	if(iStartIndex_ == 0)
	{
		pDB_->Cmd("SELECT TOP %d %s FROM %s WHERE %s >= %d ORDER BY %s ASC",lnDataCount,gc_cID,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		while(pDB_->More())
		{
			lstrDataID.AppendFormat("%d,",pDB_->GetLong(gc_cID));
		}
	}
	else
	{
		pDB_->Cmd("SELECT TOP %d %s FROM %s WHERE %s > %d ORDER BY %s ASC",lnDataCount,gc_cID,sTableName_.c_str(),gc_cID,lnCurrID,gc_cID);
		while(pDB_->More())
		{
			lstrDataID.AppendFormat("%d,",pDB_->GetLong(gc_cID));
		}
	}
	

	if(lstrDataID.GetLength() > 0)
	{
		lstrDataID.Delete(lstrDataID.GetLength() - 1);
	}

	pDB_->Cmd("DELETE FROM %s WHERE %s IN(%s)",sTableName_.c_str(),gc_cID,lstrDataID);
	if(!pDB_->Exec())
	{
		return 0;
	}
	return iCurrID_;
}

S_ChannInfo CPlantInfoBase::GetSpecVibChannInfo(const string &sChannID_)
{
	int index=-1;
	index = GetSpecVibChannIndex(sChannID_);
	return GetSpecVibChannInfo(index);
}

S_ChannInfo CPlantInfoBase::GetSpecVibChannNoInfo(const string &sChannNo_)
{
	int index=-1;
	index = GetSpecVibChannIndexNo(sChannNo_);
	return GetSpecVibChannInfo(index);
}

S_ChannInfo CPlantInfoBase::GetSpecVibChannInfo(const int &index_)
{
	S_ChannInfo channInfo;
	if (DataIsMiddle(index_,-1,_sPlantInfo._iVibChannNum))
		channInfo=_vVibChann[index_]->GetChannInfo();
	return channInfo;
}

S_ChannInfo CPlantInfoBase::GetSpecDynChannInfo(const string &sChannID_)
{
	int index=-1;
	index=GetSpecDynChannIndex(sChannID_);
	return GetSpecDynChannInfo(index);
}


S_ChannInfo CPlantInfoBase::GetSpecDynChannNoInfo(const string &sChannNo_)
{
	int index=-1;
	index=GetSpecDynChannIndexNo(sChannNo_);
	return GetSpecDynChannInfo(index);
}

S_ChannInfo CPlantInfoBase::GetSpecDynChannInfo(const int &index_)
{
	S_ChannInfo channInfo;
//	CString strInfo;strInfo.Format("%d,len:%d",index_,_vDynChann.size());
//	CHZLogManage::Info(strInfo, "Offline", "CPlantInfoBase::GetSpecDynChannInfo");
	if (DataIsMiddle(index_,-1,_sPlantInfo._iDynChannNum))
	{
		channInfo=_vDynChann[index_]->GetChannInfo();
	//	AfxMessageBox("dsf");
	}
	return channInfo;
}
int CPlantInfoBase::UpdateSpecChannInfo(string sChannID_,S_ChannInfo channInfo_)
{
	int index=GetSpecVibChannIndex(sChannID_);
	if (index<0)
	{
		index=GetSpecProcChannIndex(sChannID_);
		if (index<0)
		{
			index=GetSpecDynChannIndex(sChannID_);
			if (index<0)
			   return -1;
			else
				_vDynChann[index]->SetChannInfo(channInfo_);
		}
		else
			_vProcChann[index]->SetChannInfo(channInfo_);
	}
    else
		_vVibChann[index]->SetChannInfo(channInfo_);
	return index;
}

S_ChannInfo CPlantInfoBase::GetSpecProcChannInfo(const int &index_)
{
	S_ChannInfo channInfo;
	if((index_>-1)&&(index_< _sPlantInfo._iProcChannNum))
		channInfo=_vProcChann[index_]->GetChannInfo();
	return channInfo;
}

S_ChannInfo CPlantInfoBase::GetSpecProcChannInfo(const string &sChannID_)
{
	int index=-1;
	index=GetSpecProcChannIndex(sChannID_);
	return GetSpecProcChannInfo(index);
}

S_ChannInfo CPlantInfoBase::GetSpecProcChannNoInfo(const string &sChannNo_)
{
	int index=-1;
	index=GetSpecProcChannIndexNo(sChannNo_);
	return GetSpecProcChannInfo(index);
}

V_PCHANNINFO CPlantInfoBase::GetSpecTypeChannVector(const int &iType_/* =GE_VIBCHANN */)
{
	switch(iType_)
	{
	case GE_VIBCHANN:
         return _vVibChann;
	case GE_ALLPROC:
		 return _vProcChann;
	default:
		 return _vDynChann;
	}
}

M_CHANNINDEX CPlantInfoBase::GetSpecTypeChannMap(const int &iType_/* =GE_VIBCHANN */)
{
	switch(iType_)
	{
	case GE_VIBCHANN:
		return _mapVibChannIndex;
	case GE_ALLPROC:
		return _mapProcChannIndex;
	default:
		return _mapDynChannIndex;
	}
}

int CPlantInfoBase::GetPVXCoorData( const int &iSmpFreq_, const int &iSmpNums_, const int &iPvType_, const float &fRev_, const int &iCylinderNo_,DATATYPE_WAVE *pXCoor_)
{
	int iRtn=-1;
	//提前判断除数是否等于0的情况
    if (iSmpFreq_ < 2 || iSmpNums_ < 2)
        return 0;
	float *pfXCoor = new float[iSmpNums_];
	int   nloop,iRealNums=0,iNum=0;

	iNum = iCylinderNo_ - 1;//汽缸编号减1为当前设备结构体中对应的汽缸
	float fDelta   = 1.0/iSmpFreq_;
	float fTmp;
	for ( nloop =0; nloop< iSmpNums_; ++nloop)
	{
		pfXCoor[nloop] =(float) nloop * fDelta;
	}
	//writefile(pfXCoor,iSmpNums_,11);
	iRealNums = nloop;//因为只需要画一个周期，所以横坐标只取小于360度的.iRealNums是一个周期的点数.
	switch( iPvType_ )
	{
	case GE_PVTYPE_PV:
		{
		//	CString strInfo;strInfo.Format("示功图的V计算参数，%f,%f,%f",_sPlantInfo._sPlantInfoParam._fPistonDia,_sPlantInfo._sPlantInfoParam._fCrankShaftDia,_sPlantInfo._sPlantInfoParam._fConnRodLength);
		//	CHZLogManage::Error(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
			fDelta         = 0.005*( _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fPistonDia) ;
			fDelta         = 3.1415* fDelta *fDelta;///Ap活塞面积
			fDelta         = fDelta * (0.005* _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia);
			float   fLamda = _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia / _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fConnRodLength;
			list<float> lTmp;
			for ( nloop =0; nloop< iSmpNums_; ++nloop )
			{
				fTmp           = pfXCoor[nloop]* fRev_* 6;//6表示180/30
				if( fTmp >360 )
				{
					iRtn  =(int )fTmp /360;
					fTmp  = fTmp- iRtn * 360.0 ;
					break;
				}
				fTmp = fTmp *3.1415 /180;
				pfXCoor[nloop] = fDelta *(1- cos(fTmp) + (sin(fTmp)* sin(fTmp))*fLamda /2) ;
				lTmp.push_back(pfXCoor[nloop]);
			}
			iRealNums = nloop;
			/*writefile(pfXCoor, iRealNums,2);*/
			fDelta = *(max_element(lTmp.begin(), lTmp.end()));
			if ((fDelta <0.00001) && (fDelta > -0.00001) )
			{//值有问题太小
				CString strInfo;strInfo.Format("示功图的V计算有误，最大值出现0,%f",fDelta);
				CHZLogManage::Error(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
				fDelta = 1;
			}
			for ( nloop =0; nloop < iRealNums; ++nloop )
				pfXCoor[nloop] = 100 * pfXCoor[nloop]/fDelta;
		}
		break;
	case GE_PVTYPE_Pa:
		for ( nloop =0; nloop< iSmpNums_; ++nloop )
		{
			fTmp           = pfXCoor[nloop]* fRev_* 6;//6表示180/30
		    if( fTmp >360 )
			{
				iRtn  =(int )fTmp /360;
				fTmp  = fTmp- iRtn * 360.0 ;
				break;
			}
			pfXCoor[nloop] = fTmp;
		}
		iRealNums = nloop;
		break;

	}
	CWaveHead lsWaveHead;
	lsWaveHead.SetLength(1);
	lsWaveHead.m_pParam[0]=10.0;
	ConvertFloatToDTWave(pXCoor_, pfXCoor, iRealNums,lsWaveHead,1);
	KD_DeletePoint( pfXCoor );
	iRtn = iRealNums;
	return iRtn;
}

int CPlantInfoBase::GetPVXCoorData(const int &iSmpFreq_, const int &iPeriodSmpNums_, const int &iPvType_,const int &iCylinderNo_, DATATYPE_WAVE *pXCoor_)
{
	int  nloop = 0,iNum=0;
	//提前判断除数是否等于0的情况
	if (iSmpFreq_ < 2 || iPeriodSmpNums_ < 2)
		return 0;
	float *pfXCoor = new float[iPeriodSmpNums_];
	//CString strInfo;strInfo.Format("示功图的V计算参数iSmpFreq_:%d,iPeriodSmpNums_:%d",iSmpFreq_,iPeriodSmpNums_);
	//CHZLogManage::Debug(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
	iNum = iCylinderNo_ - 1;//汽缸编号减1为当前设备结构体中对应的汽缸
	float fDelta   = 1.0/iSmpFreq_;
	float fTmp;
	int iRealNums = iPeriodSmpNums_ > 0 ? iPeriodSmpNums_:360;
	for ( nloop =0; nloop< iRealNums; ++nloop)
	{
		pfXCoor[nloop] =(float) nloop * fDelta;
	}
	//strInfo.Format("示功图的V计算参数iRealNums:%d",iRealNums);
	//CHZLogManage::Debug(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
	float fRev = 60 * iSmpFreq_/iRealNums;
	//所以横坐标只取小于360度的.iRealNums是一个周期的点数.
	switch( iPvType_ )
	{
	case GE_PVTYPE_PV:
		{
			//CString strInfo;strInfo.Format("示功图的V计算参数，%f,%f,%f",_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fPistonDia,_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia,_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fConnRodLength);
			//CHZLogManage::Debug(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
			fDelta         = 0.005*( _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fPistonDia) ;
			fDelta         = 3.1415* fDelta *fDelta;///Ap活塞面积
			fDelta         = fDelta * (0.005* _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia);
			float   fLamda = _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia / _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fConnRodLength;
			list<float> lTmp;
			for ( nloop =0; nloop< iRealNums; ++nloop )
			{
				fTmp           = pfXCoor[nloop]* fRev* 6;//6表示180/30
				fTmp = fTmp *3.1415 /180;
				pfXCoor[nloop] = fDelta *(1- cos(fTmp) + (sin(fTmp)* sin(fTmp))*fLamda /2) ;
				lTmp.push_back(pfXCoor[nloop]);
			}
			//writefile(pfXCoor, iRealNums,222);
			fDelta = *(max_element(lTmp.begin(), lTmp.end()));
			if ((fDelta <0.00001) && (fDelta > -0.00001) )
			{//值有问题太小
				CString strInfo;strInfo.Format("示功图的V计算有误，最大值出现0,%f",fDelta);
				CHZLogManage::Error(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
				fDelta = 1;
			}
			//strInfo.Format("示功图的V计算参数iRealNums:%d,fDelta:%f",iRealNums,fDelta);
			//CHZLogManage::Debug(strInfo, "PlantInfoBase", "CPlantInfoBase::GetPVXCoorData");
			for ( nloop =0; nloop < iRealNums; ++nloop )
				pfXCoor[nloop] = 100 * pfXCoor[nloop]/fDelta;
		}
		break;
	case GE_PVTYPE_Pa:
		for ( nloop =0; nloop< iRealNums; ++nloop )
		{
			fTmp           = pfXCoor[nloop]* fRev* 6;//6表示180/30
			pfXCoor[nloop] = fTmp;
		}
		//writefile(pfXCoor,iRealNums,0);
		break;
	}
	CWaveHead lsWaveHead;
	lsWaveHead.SetLength(1);
	lsWaveHead.m_pParam[0]=10.0;
	ConvertFloatToDTWave(pXCoor_, pfXCoor, iRealNums,lsWaveHead,1);
	KD_DeletePoint( pfXCoor );
	return iRealNums;
}

int CPlantInfoBase::GetStrssAndInertiaWave(DATATYPE_WAVE *pWave1_, DATATYPE_WAVE *pWave2_,const int &iSmpFreq_,const int &iCylinderNo_,const int &iPeriodSmpNums_,float *pfStressWave_, float *pfInertiaWave_)
{
	int  nloop = 0,iNum=0;
	//提前判断除数是否等于0的情况
	if (iSmpFreq_ < 2 || iPeriodSmpNums_ < 2)
		return 0;
	float *pfWave1 = new float[iPeriodSmpNums_];
	float *pfWave2 = new float[iPeriodSmpNums_];
	float *pfStressWave = new float[iPeriodSmpNums_];
	float *pfInertiaWave = new float[iPeriodSmpNums_];
	float *pfTemp = new float[iPeriodSmpNums_];
	float fDelta   = 1.0/iSmpFreq_;
	int iRealNums = iPeriodSmpNums_ > 0 ? iPeriodSmpNums_:360;
	for ( nloop =0; nloop< iRealNums; ++nloop)
	{
		pfTemp[nloop] =(float) nloop * fDelta;
	}
	float fRev = 60 * iSmpFreq_/iRealNums;
	iNum = iCylinderNo_ - 1;//汽缸编号减1为当前设备结构体中对应的汽缸
	float fPistonDia=0.01*(_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fPistonDia);//活塞直径
	float fRodDia=0.01*(_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fRes1);//活塞杆直径
	float fDeltaOut=fPistonDia*fPistonDia*3.1415/4;// Sout为气缸缸头侧面积，单位：m^2；
	float fDeltaIn=fDeltaOut-fRodDia*fRodDia*3.1415/4;//Sin为气缸曲轴侧面积，单位：m^2；
	ConvertDTToFloat(pfWave1,pWave1_,iRealNums);
	ConvertDTToFloat(pfWave2,pWave2_,iRealNums);
    //计算气体压力
	for ( nloop =0; nloop< iRealNums; ++nloop )
	{
		pfStressWave[nloop]  = 1000*(pfWave1[nloop]*fDeltaIn-pfWave2[nloop]*fDeltaOut);
		pfStressWave_[nloop] = pfStressWave[nloop];//因为读取的压力波形为DATATYPE_WAVE，转化为float时已经除了系数10.0，所以不用除以10
	}
	//计算惯性力
	float fConnRodLength =0.01*(_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fConnRodLength);//连杆长度，单位m；
	float fCrankShaftDia =0.01*(_sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fCrankShaftDia);//曲轴半径，单位m
	float fMass = _sPlantInfo._sRecipComprParam._sPlantInfoParam[iNum]._fRes2;//惯性体质量，单位Kg；
	float fAngleRev=2*3.1415*fRev/60;//角速度
	for ( nloop =0; nloop< iRealNums; ++nloop )
	{
		pfInertiaWave[nloop] = fMass*(fCrankShaftDia*fAngleRev*fAngleRev*(cos(pfTemp[nloop]* fAngleRev))+fCrankShaftDia/fConnRodLength*cos(2*pfTemp[nloop]* fAngleRev));
	    pfInertiaWave_[nloop] = pfInertiaWave[nloop];//因为计算得到的波形是实际的值，所以不用除以10
	}
	KD_DeletePoint(pfWave1);
	KD_DeletePoint(pfWave2);
	KD_DeletePoint(pfStressWave);
	KD_DeletePoint(pfInertiaWave);
	KD_DeletePoint(pfTemp);
	return iRealNums;
}

//using namespace BHKD;
bool CPlantInfoBase::ConvertFloatToDTWave(BHKD::DATATYPE_WAVE *pWave_,const float *pfWave_,int iLen_,CWaveHead& sWaveHead_,float fCoef_)
{
	if(sWaveHead_.GetLength() <= 0)
	{
		const float fPrec=10.0;
		for (int loop=0;loop<iLen_;++loop)
			pWave_[loop]=(BHKD::DATATYPE_WAVE)(pfWave_[loop]*fPrec*fCoef_);
	}
	else
	{
		for (int loop=0;loop<iLen_;++loop)
			pWave_[loop]=(BHKD::DATATYPE_WAVE)(pfWave_[loop]*sWaveHead_.m_pParam[0]*fCoef_);
	}

	return true;
}
bool CPlantInfoBase::ConvertDTWaveToFloat(float *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_)
{
	if(sWaveHead_.GetLength() <= 0)
	{
		const float fPrec=10.0;
		for (int loop=0;loop<iLen_;++loop)
			pfWave_[loop]=(float)(pWave_[loop]/fPrec);
	}
	else if(sWaveHead_.GetLength() == 1)
	{
		
		
		for (int loop=0;loop<iLen_;++loop)
			pfWave_[loop]=(float)(pWave_[loop]/sWaveHead_.m_pParam[0]);

	}
	else
	{
		if((int)sWaveHead_.m_pParam[1] == 1)
		{
			for (int loop=0;loop<iLen_;++loop)
				pfWave_[loop]= ((pWave_[loop] -SHRT_MIN)*1.0)/((SHRT_MAX-SHRT_MIN)*1.0)*(sWaveHead_.m_pParam[3]-sWaveHead_.m_pParam[2])+sWaveHead_.m_pParam[2];
		}
		else
		{
			for (int loop=0;loop<iLen_;++loop)
				pfWave_[loop]=(float)(pWave_[loop]/sWaveHead_.m_pParam[0]);
		}
		
	}
	
	return true;
}

bool CPlantInfoBase::ConvertDoubleToDTWave(BHKD::DATATYPE_WAVE *pWave_,const double *pfWave_,int iLen_,CWaveHead& sWaveHead_,float fCoef_)
{
	if(sWaveHead_.GetLength() <= 0)
	{
		const float fPrec=10.0;
		for (int loop=0;loop<iLen_;++loop)
			pWave_[loop]=(BHKD::DATATYPE_WAVE)(pfWave_[loop]*fPrec*fCoef_);
	}
	else
	{
		for (int loop=0;loop<iLen_;++loop)
			pWave_[loop]=(BHKD::DATATYPE_WAVE)(pfWave_[loop]*sWaveHead_.m_pParam[0]*fCoef_);
	}
	return true;
}
bool CPlantInfoBase::ConvertDTWaveToDouble(double *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_)
{
	if(sWaveHead_.GetLength() <= 0)
	{
		const float fPrec=10.0;
		for (int loop=0;loop<iLen_;++loop)
			pfWave_[loop]=(double)(pWave_[loop]/fPrec);
	}
	else if(sWaveHead_.GetLength() == 1)
	{
		
		
		for (int loop=0;loop<iLen_;++loop)
			pfWave_[loop]=(double)(pWave_[loop]/sWaveHead_.m_pParam[0]);
		
	}
	else
	{
		if((int)sWaveHead_.m_pParam[1] == 1)
		{
			for (int loop=0;loop<iLen_;++loop)
				pfWave_[loop]= ((pWave_[loop] -SHRT_MIN)*1.0)/((SHRT_MAX-SHRT_MIN)*1.0)*(sWaveHead_.m_pParam[3]-sWaveHead_.m_pParam[2])+sWaveHead_.m_pParam[2];
		}
		else
		{
			for (int loop=0;loop<iLen_;++loop)
				pfWave_[loop]=(float)(pWave_[loop]/sWaveHead_.m_pParam[0]);
		}
	}
	return true;
}

bool CPlantInfoBase::ParseDTWaveToDT(DATATYPE_WAVE *pRealWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_)
{
	if(sWaveHead_.GetLength() <= 1)
	{
		memcpy(pRealWave_,pWave_,iLen_*sizeof(DATATYPE_WAVE));
	}
	else
	{
		if((int)sWaveHead_.m_pParam[1] == 1)
		{
			for (int loop=0;loop<iLen_;++loop)
				pRealWave_[loop]= ((pWave_[loop] -SHRT_MIN)*1.0)/((SHRT_MAX-SHRT_MIN)*1.0)*(sWaveHead_.m_pParam[3]-sWaveHead_.m_pParam[2])+sWaveHead_.m_pParam[2];
		}
		else
		{
			memcpy(pRealWave_,pWave_,iLen_*sizeof(DATATYPE_WAVE));
		}
	}
	return true;
}
bool CPlantInfoBase::ConvertFloatToDouble(double *pWave_,const float *pfWave_,int iLen_)
{
	for (int loop=0;loop<iLen_;++loop)
		pWave_[loop]=(float)pfWave_[loop];
	return true;
}
bool CPlantInfoBase::ConvertDoubleToFloat(float *pfWave_,const double *pWave_,int iLen_)
{
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=(double)pWave_[loop];
	return true;
}

bool CPlantInfoBase::ConvertDTToFloat(float *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_)
{
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=(float)pWave_[loop];
	return true;
}

bool CPlantInfoBase::ConvertFloatToDT(DATATYPE_WAVE *pWave_,const float *pfWave_,int iLen_)
{
	for (int loop=0;loop<iLen_;++loop)
		pWave_[loop]=(DATATYPE_WAVE)pfWave_[loop];
	return true;
}

bool CPlantInfoBase::ScaleWave(float *pfWave_,const float *pWave_,int iLen_,float fCoef_)
{
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=pWave_[loop]*fCoef_;
	return true;
}

bool CPlantInfoBase::ScaleWave(double *pfWave_,const double *pWave_,int iLen_,float fCoef_)
{
	for (int loop=0;loop<iLen_;++loop)
		pfWave_[loop]=(double)pWave_[loop]*fCoef_;
	return true;
}
/**频谱分析和细化分析，如果实际的分析线数满足要求，就直接进行分析，如果不满足就进行细化分析
*@param pfWave_  波形数据指针，长度为iSmpNums_.
*@param iSmpNums_  波形数据点数.
*@param iSmpFreq_  波形采样频率.
*@param pfSpec_    谱数据输出，内存是在外部分配，长度必须为iSmpNums_/2.
*@param iLines_    分析线数，返回的为分析后的分析线数
*@param fDf_       谱的分辨率
*@return 正确就为0，错误就小于0
*/
int CPlantInfoBase::ZoomSpectrumAnalysis(float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_)
{
	_DECLARE_PERF_MEASURE_TIME();
	if(iSmpFreq_ < 1 || iSmpNums_ < 10)
	{
		/*ASSERT(FALSE);*/
		return -1;
	}
	int nErrorCode=0;
	iSmpNums_=iSmpNums_>0?iSmpNums_:1;

	int iLinesTmp = iLines_;

	if (iLines_%2!=0)
	{
		iLinesTmp = iLines_ -1;//因为客户端穿的都是401或者1601，3201
	}

	//iSmpFreq_=iSmpFreq_>0?iSmpFreq_:1;
	float fRealDf=.0;//实际的频率分辨率
	float fRealFreq=.0;//实际的分析最高频率
	long iRealLines = 0;//实际的分析线数
	fRealFreq = ((float)iSmpFreq_/2.56);
	fRealDf   = ((float)iSmpFreq_/(float)iSmpNums_);
	iRealLines= fRealFreq/fRealDf;
	//CString sTmp;sTmp.Format("iLinesTmp:%d,%d,iSmpFreq_:%d,%d,%.4f,%d",iLinesTmp,iSmpNums_,iSmpFreq_,iLines_,fRealDf,iRealLines);BHTRACE_DEBUG(sTmp);
	//CHZLogManage::Debug( sTmp, "CPlantInfoBase","ZoomSpectrumAnalysis");
	//在频谱分析前，去掉波形数据的直流量
	float fSum=.0,fAverage=.0;
	float *pfWaveData = NULL;
	double *pWaveData = NULL;
	double *pSpec = NULL;
	pfWaveData = new float[GD_MIDDLEWARE_WAVE_POINT_MAX];
	for (int nloop =0;nloop< iSmpNums_; ++nloop)
		fSum = fSum + pfWave_[nloop];
	fAverage = fSum/iSmpNums_;
	for (int nloop = 0;nloop<iSmpNums_; ++nloop)
		pfWaveData[nloop] = pfWave_[nloop]-fAverage;

	////非离心压缩机类设备不做细化分析 mb 2010年11月1日。因为考虑其他设备测点为加速度信号等原因
	if ((_sPlantInfo._iMachineType != GE_MACHINETYPE_COMPR)  &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_COMPR1) &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_TURB) &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_SMOKESTEAM))
	{
		if(iLinesTmp > iRealLines)
		{	
			iLinesTmp = iRealLines;
		}
	}

//#ifndef __NO_ZOOM_FFT

	if(iLinesTmp > iRealLines)
	{	
		iLinesTmp = iRealLines;
	}

//#endif

	if(iLinesTmp > iRealLines)
	{//细化谱分析
		fDf_=fRealFreq/(float)iLinesTmp;
		//细化分析不能超过20倍
		if((20*fDf_)<fRealDf)
		{
			KD_DeletePoint(pfWaveData);
			return -2;
		}
		iRealLines=iLinesTmp;
		float fdd=fDf_;
		try
		{

#ifndef __NO_ZOOM_FFT

			nErrorCode=ZoomFFT(pfWaveData,(long)iSmpNums_,0,(short)fRealFreq,iLinesTmp,iSmpFreq_,pfSpec_,&iRealLines,&fStartFreq_,&fDf_);

			if (iRealLines >3)
			{
				pfSpec_[iRealLines-1] = 0;
				pfSpec_[iRealLines-2] = 0;
			}
#endif			
		}
		catch (...)
		{			
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			nErrorCode = -1;
		}

		//CString strTmp;strTmp.Format("nErrorCode:%d,%.2f,%.2f,%.2f,%d,%d",nErrorCode,fRealFreq,fdd,fDf_,iLines_,iRealLines);BHTRACE_DEBUG(strTmp);
		//CHZLogManage::Debug( strTmp, "CPlantInfoBase","ZoomSpectrumAnalysis");

		if(iLines_ >iLinesTmp)
		{
			int inum = iLines_ -iLinesTmp;
			for (int nloop = 0; nloop <inum; ++nloop )
				pfSpec_[iLinesTmp+ nloop] =0;
		}
		pfSpec_[iLinesTmp] = 0;
		iLines_ = iLinesTmp;
	}
	else
	{//正常频谱分析
		//调用频谱计算修改后的函数,注：应该根据分析线数去，一定长度的波形点数作频谱分析，如：400线时，对应取400*2.56=1024个点作频谱分析
		pWaveData = new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
		std::vector<double> loBuffer(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pSpec=&loBuffer.front();

		ConvertFloatToDouble(pWaveData,pfWave_,iSmpNums_);

		CSigMath calcSpectral;
		int lnAnalyzeSmpNumber = iSmpNums_;
		if (lnAnalyzeSmpNumber>iLines_*2.56)
		{
			lnAnalyzeSmpNumber = iLines_*2.56;
		}

		fStartFreq_=0;
		fDf_=(float)iSmpFreq_/lnAnalyzeSmpNumber;	

		int lnRet = -1;
		_BEGIN_PERF_MEASURE_TIME();		
		{
			try
			{
				lnRet = CPlantInfoBase::SpectralMeasureMenEx(pWaveData,
					lnAnalyzeSmpNumber,
					loBuffer,
					E_SpectrumType_PEAK,
					WINDOW_HANNING,2,4);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);
			}

		}
		_END_PERF_MEASURE_TIME("SpectralMeasureMent");

		fDf_=(float)iSmpFreq_/lnAnalyzeSmpNumber;
		ConvertDoubleToFloat(pfSpec_,pSpec,ceil((double)lnAnalyzeSmpNumber/2.56));
		// _DEBUG
		iLines_ = ceil((double)lnAnalyzeSmpNumber/2.56);
#ifdef _DEBUG
//		writefile(pfSpec_,iLines_,200071);
#endif
		nErrorCode=2;
		KD_DeletePoint(pWaveData);
		//KD_DeletePoint(pSpec);
	}
	//CString sTmp;sTmp.Format("%d",nErrorCode);AfxMessageBox(sTmp);
	KD_DeletePoint(pfWaveData);
	return nErrorCode;
}

int CPlantInfoBase::ZoomSpectrumAnalysis(float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_, float fStopFreq_)
{
	int nErrorCode=0;

	if(iSmpFreq_ < 1 || iSmpNums_ < 10)
	{
		ASSERT(FALSE);
		return -1;
	}

	iSmpNums_= iSmpNums_>0 ? iSmpNums_ : 1;

	int iLinesTmp = iLines_ ;

	if (iLines_%2!=0)
	{
		iLinesTmp = iLines_ -1;//因为客户端穿的都是401或者1601，3201
	}

	//iSmpFreq_=iSmpFreq_>0?iSmpFreq_:1;

	//实际的分析最高频率
	float fRealFreq = iSmpFreq_ / 2.56;
	if ( fStopFreq_ > 0)
		fRealFreq  = fStopFreq_;
	else
		fStopFreq_ = fRealFreq;

	//实际的频率分辨率
	float fRealDf    = (float)iSmpFreq_/(float)iSmpNums_;
	//实际的分析线数
	long  iRealLines = fRealFreq / fRealDf;

	//CString strTmp;strTmp.Format("实际的分析最高频率(2000): %.2f,实际的频率分辨率(2.5): %.2f,实际的分析线数(800): %d",fRealFreq,fRealDf,iRealLines);BHTRACE_DEBUG(strTmp);

	//在频谱分析前，去掉波形数据的直流量
	float   fSum       = .0;
	float   fAverage   = .0;
	float  *pfWaveData = NULL;
	double *pWaveData  = NULL;
	double *pSpec      = NULL;
	pfWaveData = new float[iSmpNums_];
	for (int nloop =0;nloop< iSmpNums_; ++nloop)
		fSum = fSum + pfWave_[nloop];
	fAverage = fSum/iSmpNums_;
	for (int nloop = 0;nloop<iSmpNums_; ++nloop)
		pfWaveData[nloop] = pfWave_[nloop]-fAverage;

	bool bNeedZoom = false;

	////实时未对以下设备作细化谱, 历史也不应做 mb 2011-3-18
	if ((_sPlantInfo._iMachineType != GE_MACHINETYPE_COMPR)  &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_COMPR1) &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_TURB) &&
		(_sPlantInfo._iMachineType != GE_MACHINETYPE_SMOKESTEAM))
	{
		bNeedZoom = false;
	}
	else
	{
		if(fRealDf >((fStopFreq_ - fStartFreq_)/iLinesTmp))
		{
			bNeedZoom = true;
		}
	}

//#ifndef __NO_ZOOM_FFT

	if(iLinesTmp > iRealLines)
	{	
		iLinesTmp = iRealLines;
	}
	//#ifdef __NO_ZOOM_FFT
	//	bNeedZoom = false;
//#endif	

	if(iLinesTmp > iRealLines)
	{//细化谱分析
		fDf_=fRealFreq/iLinesTmp;

		//细化分析不能超过20倍
		if((20*fDf_)<fRealDf)
		{
			KD_DeletePoint(pfWaveData);
			return -2;
		}

		iRealLines = iLinesTmp;
		float fdd=fDf_;
		short iStartFreq = fStartFreq_;
		try
		{
#ifndef __NO_ZOOM_FFT
			nErrorCode=ZoomFFT(pfWaveData,(long)iSmpNums_, iStartFreq, (short)fRealFreq,iLinesTmp,iSmpFreq_,pfSpec_,&iRealLines,&fStartFreq_,&fDf_);

			if (iRealLines >3)
			{
				pfSpec_[iRealLines-1] = 0;
				pfSpec_[iRealLines-2] = 0;
			}
#endif
		}
		catch (...)
		{			
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			nErrorCode =-1;
		}



		//strTmp.Format("细化谱分析第三个参数为fDf%.2f,%.2f,%.2f,%d,%d",fRealFreq,fdd,fDf_,iLines_,iRealLines);BHTRACE_DEBUG(strTmp);
		if(iLines_>iLinesTmp)
		{
			int inum = iLines_ -iLinesTmp;
			for (int nloop = 0; nloop <inum; ++nloop )
				pfSpec_[iLinesTmp+ nloop] =0;
		}
		pfSpec_[iLines_] = 0;
		iLines_ = iLinesTmp;

	}
	else
	{//正常频谱分析

		//调用频谱计算修改后的函数,注：应该根据分析线数去，一定长度的波形点数作频谱分析，如：400线时，对应取400*2.56=1024个点作频谱分析
		pWaveData = new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
		std::vector<double> loBuffer(GD_MIDDLEWARE_WAVE_POINT_MAX);
		pSpec= &loBuffer.front();

		ConvertFloatToDouble(pWaveData,pfWave_,iSmpNums_);

		int lnAnalyzeSmpNumber = iSmpNums_;

		if (lnAnalyzeSmpNumber>iLines_*2.56)
		{
			lnAnalyzeSmpNumber = iLines_*2.56;
		}

		fStartFreq_=0;
		fDf_=(float)iSmpFreq_/lnAnalyzeSmpNumber;	

		CSigMath calcSpectral;
		try
		{
			CPlantInfoBase::SpectralMeasureMenEx(pWaveData,
				lnAnalyzeSmpNumber,
				loBuffer,
				E_SpectrumType_PEAK,WINDOW_HANNING,
				2,
				4);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}
		fDf_=(float)iSmpFreq_/lnAnalyzeSmpNumber;
		ConvertDoubleToFloat(pfSpec_,pSpec,ceil((double)lnAnalyzeSmpNumber/2.56));
		iLines_ = ceil((double)lnAnalyzeSmpNumber/2.56);
#ifdef _DEBUG
//		writefile(pfSpec_,iLines_,200070);
#endif // _DEBUG

		//CString strTmp;strTmp.Format("正常频谱分析%.2f,%.2f,%d",fRealFreq,fDf_,iLines_);BHTRACE_DEBUG(strTmp);
		nErrorCode=2;
		KD_DeletePoint(pWaveData);
		//KD_DeletePoint(pSpec);
	}

	KD_DeletePoint(pfWaveData);
	return nErrorCode;
}

int CPlantInfoBase::SpectrumAnalysis( DATATYPE_WAVE *pWave_, int &iSmpNums_, DATATYPE_WAVE *pSpectrum_, const int &iSensorType_,CWaveHead& sWaveHead_)
{
	double *pfWave     = new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
	std::vector<double> loBuffer(GD_MIDDLEWARE_WAVE_POINT_MAX);
	double *pfSpectrum = &loBuffer.front();
	ConvertDTWaveToDouble(pfWave, pWave_, iSmpNums_,sWaveHead_);
	CSigMath calcSpectral;
	try
	{
		CPlantInfoBase::SpectralMeasureMenEx(pfWave,
			iSmpNums_,
			loBuffer,
			E_SpectrumType_PEAK,
			WINDOW_HANNING,
			2,
			4);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	//       writefile(pffreq,iquest_wavenum/2);
	if (iSensorType_==GE_SENSORTYPE_VOTEX)
	{
		for (int nloop=0;nloop< (iSmpNums_/2); ++nloop)
			pfSpectrum[nloop]=pfSpectrum[nloop]*2.0;//江老师要求添加的系数
	}
	else
	{
		for (int nloop=0;nloop<(iSmpNums_/2); ++nloop)
			pfSpectrum[nloop]=pfSpectrum[nloop]*1.414;//江老师要求添加的系数
	}
	ConvertDoubleToDTWave(pSpectrum_,pfSpectrum,iSmpNums_/2,sWaveHead_);
	KD_DeletePoint(pfWave);
	//KD_DeletePoint(pfSpectrum);
	return iSmpNums_;
}

int CPlantInfoBase::SpectrumAnalysis( float *pfWave_, int &iSmpNums_, float *pfSpectrum_, const int &iSensorType_)
{
	double *pfWave     = new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
	std::vector<double> loBuffer(GD_MIDDLEWARE_WAVE_POINT_MAX);
	double *pfSpectrum = &loBuffer.front();
	ConvertFloatToDouble(pfWave, pfWave_, iSmpNums_);
	CSigMath calcSpectral;
	try
	{
		CPlantInfoBase::SpectralMeasureMenEx(pfWave,
			iSmpNums_,
			loBuffer,
			E_SpectrumType_PEAK,
			WINDOW_HANNING,
			2,
			4);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	//       writefile(pffreq,iquest_wavenum/2);
	if (iSensorType_==GE_SENSORTYPE_VOTEX)
	{
		for (int nloop=0;nloop< (iSmpNums_/2); ++nloop)
			pfSpectrum_[nloop]=pfSpectrum[nloop]*2.0;//江老师要求添加的系数
	}
	else
	{
		for (int nloop=0;nloop<(iSmpNums_/2); ++nloop)
			pfSpectrum_[nloop]=pfSpectrum[nloop]*1.414;//江老师要求添加的系数
	}

	KD_DeletePoint(pfWave);
	//KD_DeletePoint(pfSpectrum);
	return iSmpNums_;
}

int CPlantInfoBase::CepstrumAnalysis( DATATYPE_WAVE *pWave_, int &iSmpNums_, double *pSpectrum_,int &iCepsNums_,CWaveHead& sWaveHead_)
{
	double *pfWave     = new double[iSmpNums_];
	ConvertDTWaveToDouble(pfWave, pWave_, iSmpNums_,sWaveHead_);
	//writefile(pfWave,iSmpNums_,100);
	CSigMath calcSpectral;
	try
	{
		calcSpectral.Cepstrum(pfWave,iSmpNums_,pSpectrum_);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	iCepsNums_=iSmpNums_/2;
	//writefile(pSpectrum_,iCepsNums_,101);
	KD_DeletePoint(pfWave);
	return iSmpNums_;
}

int CPlantInfoBase::SaveThresholdInfo(IDBInterFace *pDB_,int iThID_,const string &sName_, CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,int iType_,char *pfLearnData_,int iLearnDataLen_,CThreshold *pTh_,CThresholdLearnParam *pThLearnPar_,int iAlarmType_,int iCharNums_)
{
	int iRtn=-1;
	/*	[ID] [bigint] NOT NULL ,
	[THRESHOLD_ID] [bigint] NOT NULL ,
	[name] [char] (100) COLLATE Chinese_PRC_CI_AS NOT NULL ,
	[START_T] [datetime] NOT NULL ,
	[END_T] [datetime] NULL ,
	[SYN_FLAG] [int] NULL ,
	[TYPE] [int] NOT NULL ,
	[CHANN_TYPE] [int] NULL ,
	[data] [image] NULL ,
	[data_len] [int] NULL ,
	[THRESHOLD] [image] NULL ,
	[THRESHOLD_LEN] [int] NULL ,
	[LEARN_PARAM] [image] NULL ,
	[LEARN_PARAM_LEN] [int] NULL ,
	[ALARM_TYPE] [int] NULL ,*/
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	pDB_->BeginTran();
	pDB_->Cmd("select 1 from [%s] ", _sThTblName.c_str());
	pDB_->Cmd(" where [%s] = %d and [%s] = '%s'", gc_cThreshold_ID, iThID_, gc_cName, sName_.c_str());
	if (pDB_->More())
	{
		iRtn=1;
		goto EXIT_SAVETHINFO;
	}
	pDB_->Cmd("insert into [%s]",_sThTblName.c_str());
	pDB_->Cmd("([%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]) ",gc_cID,gc_cThreshold_ID,gc_cName,gc_cStart_T,gc_cEnd_T,\
		gc_cSynFlag,gc_cType,gc_cChannType,gc_cAlarm_Type);
	pDB_->Cmd("  VALUES(%d,%d,'%s','%s','%s',0,%d,%d,%d)",iThID_,iThID_,sName_.c_str(),TimeConvertString(dtStart_),TimeConvertString(dtEnd_),iType_,iChannType_,iAlarmType_);
	//CHZLogManage::Debug(pDB_->GetSql(),"Base","CPlantInfoBase::SaveThresholdInfo");
    if(!(pDB_->Exec()))
	{
		iRtn=-2;
		goto EXIT_SAVETHINFO;
	}
	pDB_->Cmd("select * from [%s] ",_sThTblName.c_str());
	pDB_->Cmd(" where [%s]=%d",gc_cThreshold_ID,iThID_);
	if (!(pDB_->More(TRUE)))
	{
		iRtn=-3;
		goto EXIT_SAVETHINFO;
	}/**/
	pDB_->SetBuffer(gc_cData,(BYTE*)pfLearnData_,sizeof(float)*iLearnDataLen_);
	pDB_->SetFieldValue(gc_cData_Len,iLearnDataLen_);

	int iBytes=sizeof(CThreshold)*iCharNums_;
	pDB_->SetBuffer(gc_cThreshold,(BYTE*)pTh_,iBytes);
	pDB_->SetFieldValue(gc_cThreshold_Len,iBytes);

	iBytes=sizeof(CThresholdLearnParam)*iCharNums_;
	pDB_->SetBuffer(gc_cLearnParam,(BYTE*)pThLearnPar_,iBytes);
	pDB_->SetFieldValue(gc_cLearnParam_Len,iBytes);
	iRtn=1;
EXIT_SAVETHINFO:
	if (iRtn>0)
		pDB_->CommitTran();
	else
		pDB_->RollbackTran();
	return iRtn;
}

int CPlantInfoBase::SaveThresholdInfo(IDBInterFace *pDB_,int iThID_,const int &iChannIndex_, CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,int iType_,char *pfLearnData_,int iLearnDataLen_,CThreshold *pTh_,CThresholdLearnParam *pThLearnPar_,int iAlarmType_,int iCharNums_)
{//保存阈值信息，阈值数据，学习参数，学习数据等
	int iRtn=-1;
	if ( (iChannIndex_ <0 ) || (iChannIndex_ > GetVibChannNum()))
		return iRtn;
	string sName = GetSpecVibChannInfo( iChannIndex_ )._cChannID;
   iRtn = SaveThresholdInfo(pDB_, iThID_, sName, dtStart_, dtEnd_, iChannType_, iType_, pfLearnData_, iLearnDataLen_, pTh_, pThLearnPar_, iAlarmType_, iCharNums_);
	return iRtn;
}

bool CPlantInfoBase::IsNotDynPeriodIndex(const int iStartIndex_,const int iEndIndex_)
{
	int iPeriodStartIndex=0,iPeriodEndIndex=0,iResult=0;
	iPeriodStartIndex = iStartIndex_;
	iPeriodEndIndex = iEndIndex_;
	//如果结果大于零，则为整周期采样，返回false，否则返回true
	iResult = iPeriodEndIndex-iPeriodStartIndex;
	if (iResult>0)
		return false;
	return true;
}

CString CPlantInfoBase::TimeConvertString(const CBHDateTime &time_)
{
	/*CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;*/
	return time_.Format(gc_cTimeFormat);
}

CString CPlantInfoBase::TimeConvertStringQuote(const CBHDateTime &time_)
{
	CString strTmp;
	strTmp.Format("'%s'",TimeConvertString(time_));
	return strTmp;
}

CString CPlantInfoBase::ConsCommonField()
{
	CString sFields;
	sFields.Format("%s,%s,%s,%s,%s,%s,%s",
		gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	return sFields;
}

E_TBL_CHANNTYPE CPlantInfoBase::GetChannOneType( const CHANNTYPE &iChannType_ )
{
	switch(iChannType_)
	{
	case GE_ALLPROC:
	case GE_PRESSCHANN:
	case GE_TEMPCHANN:
	case GE_FLUXCHANN:
	case GE_OTHERCHANN:
	case GE_CURRENTCHANN:
	case GE_IMPACTCHANN:
		return E_TBL_CHANNTYPE_PROC;
	case GE_VIBCHANN:
	case GE_AXIALCHANN:
	case GE_AXISLOCATIONCHANN:
		return E_TBL_CHANNTYPE_VIB;
	case GE_DYNPRESSCHANN:
	case GE_RODSINKCHANN:
	case GE_DYNSTRESSCHANN:
		return E_TBL_CHANNTYPE_DYN;
	default:
		return E_TBL_CHANNTYPE_VIB;
	}
}

BOOL CPlantInfoBase::DeleteVibTrendFastRecord( IDBInterFace *pDB_ )
{
	int lnRecordID = -1;

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] < 0 order by [%s]",gc_cID,_sVibTrendFastTblName.c_str(),gc_cID,gc_cTime);
	if(pDB_->More())
	{
		lnRecordID = pDB_->GetLong(gc_cID);
		pDB_->Cmd("delete from [%s] where [%s] = %d",_sVibTrendFastTblName.c_str(),gc_cID,lnRecordID);
		return pDB_->Exec();
	}
	
	return FALSE;
}

BOOL CPlantInfoBase::DeleteDynTrendFastRecord( IDBInterFace *pDB_ )
{
	int lnRecordID = -1;

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] < 0 order by [%s]",gc_cID,_sDynTrendFastTblName.c_str(),gc_cID,gc_cTime);
	if(pDB_->More())
	{
		lnRecordID = pDB_->GetLong(gc_cID);
		pDB_->Cmd("delete from [%s] where [%s] = %d",_sDynTrendFastTblName.c_str(),gc_cID,lnRecordID);
		
		return pDB_->Exec();
	}

	return FALSE;
}

BOOL CPlantInfoBase::DeleteProcTrendFastRecord( IDBInterFace *pDB_ )
{
	int lnRecordID = -1;

	pDB_->Cmd("select top 1 [%s] from [%s] where [%s] < 0 order by [%s]",gc_cID,_sProcTrendFastTblName.c_str(),gc_cID,gc_cTime);
	if(pDB_->More())
	{
		lnRecordID = pDB_->GetLong(gc_cID);
		pDB_->Cmd("delete from [%s] where [%s] = %d",_sProcTrendFastTblName.c_str(),gc_cID,lnRecordID);
		return pDB_->Exec();
	}

	return FALSE;
}

int CPlantInfoBase::SpectralMeasureMenEx(  double *pwave,
										 int & anRef, 
										 double *pout,
										 E_SpectrumType type,
										 E_WINOW_TYPE iWinType_ /*= WINDOW_HANNING*/, 
										 unsigned int iPrecision_ /*= 2*/, 
										 int anCUTOFF_TYPE/*=0*/, //0:not cutoff 1:cutoff to 2*n 3:cutoff to pow(2,n)
										 double adblScaleBeforeCalc /*=1.0*/ )
{
	int lnOriginalSize = anRef;

	try
	{
		if (anRef<=1)
		{
			return -1;
		}

		if (NULL == pwave)
		{
			return -2;
		}

		if (NULL == pout)
		{
			return -3;
		}

		if (anCUTOFF_TYPE<0||anCUTOFF_TYPE>2)
		{
			return -4;
		}

		if (adblScaleBeforeCalc<0)
		{
			return -5;
		}

		if (1==anCUTOFF_TYPE)
		{
			if (anRef%2!=0)
			{
				anRef=anRef-1;
			}
		}else if (2==anCUTOFF_TYPE)
		{
			int lnOrder = int(log((double)anRef)/log((double)2.0));

			double ldblValue = pow((double)2.0,(double)lnOrder);

			if (ldblValue>anRef)
			{
				lnOrder = lnOrder -1;

			}
			anRef = pow(2.0,(double)lnOrder);
		}

		if (adblScaleBeforeCalc>0)
		{
			for (int  i=0;i<anRef;i++)
			{
				pwave[i] = pwave[i]*adblScaleBeforeCalc;
			}
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

	try
	{
		_DECLARE_PERF_MEASURE_TIME();
		_BEGIN_PERF_MEASURE_TIME();
		CSigMath calcSpectral;
		int lnRet = calcSpectral.SpectralMeasureMent(pwave,anRef,pout,type,iWinType_,iPrecision_);
		_END_PERF_MEASURE_TIME(STR("calcSpectral.SpectralMeasureMent:%d",anRef));
		if (adblScaleBeforeCalc>0)
		{ 
			int lnLoopCount = anRef;

			if (lnLoopCount>=lnOriginalSize)
			{
				lnLoopCount = lnOriginalSize;
			}

			for (int  i=0;i<lnLoopCount;i++)
			{
				pout[i] = pout[i]/adblScaleBeforeCalc;
			}


			for (int  i=0;i<anRef;i++)
			{
				pwave[i] = pwave[i]/adblScaleBeforeCalc;
			}
		}

		return lnRet;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	return -1;

}

int CPlantInfoBase::SpectralMeasureMenEx(  double *pwave, 
										 int &anRef, 
										 vector<double>& vOut_,
										 E_SpectrumType type, 
										 E_WINOW_TYPE iWinType_ /*= WINDOW_HANNING*/, 
										 unsigned int iPrecision_ /*= 2*/, 
										 int anCUTOFF_TYPE/*=0*/, //0:not cutoff 1:cutoff to 2*n 3:cutoff to pow(2,n)
										 double adblScaleBeforeCalc /*=1.0*/ )
{
	int lnOriginalSize = anRef;

	double * lpDblBuffer = pwave;

	std::vector<double> loDblBuffer;

	try
	{
		if (anRef<=1)
		{
			return -1;
		}

		if (NULL == pwave)
		{
			return -2;
		}

		if (anCUTOFF_TYPE<0||anCUTOFF_TYPE>4)
		{
			return -3;
		}

		if (adblScaleBeforeCalc<0)
		{
			return -4;
		}

		if (1==anCUTOFF_TYPE)
		{
			if (anRef%2!=0)
			{
				anRef=anRef-1;
			}
		}else if (2==anCUTOFF_TYPE)
		{
			int lnOrder = int(log((double)anRef)/log((double)2.0));

			double ldblValue = pow((double)2.0,(double)lnOrder);

			if (ldblValue>anRef)
			{
				lnOrder = lnOrder -1;

			}
			anRef = pow(2.0,(double)lnOrder);

		}else if (3==anCUTOFF_TYPE)
		{
			int lnOrder = int(log((double)anRef)/log((double)2.0));

			double ldblValue = pow((double)2.0,(double)lnOrder);

			if (ldblValue>=anRef)
			{
				lnOrder = lnOrder -1;
			}			

			lnOrder = lnOrder+1;

			anRef = pow(2.0,(double)lnOrder);

			loDblBuffer.resize(anRef);

			int lnIndex = 0;

			for (int i=0;i<loDblBuffer.size();i++)
			{
				lnIndex = i%lnOriginalSize;

				if (lnIndex>=lnOriginalSize)
				{
					ASSERT(FALSE);
					continue;
				}

				loDblBuffer[i] = pwave[lnIndex];
			}

			lpDblBuffer = &loDblBuffer.front(); 

		}else if (4==anCUTOFF_TYPE)
		{
			int lnOrder = int(log((double)anRef)/log((double)2.0));

			double ldblValue = pow((double)2.0,(double)lnOrder);

			if (ldblValue>=anRef)
			{
				lnOrder = lnOrder -1;
			}			

			lnOrder = lnOrder+1;

			anRef = pow(2.0,(double)lnOrder);

			loDblBuffer.resize(anRef);

			for (int i=0;i<loDblBuffer.size();i++)
			{

				if (i<lnOriginalSize)
				{
					loDblBuffer[i] = pwave[i];
				}else
				{
					loDblBuffer[i] = 0;
				}

			}

			lpDblBuffer = &loDblBuffer.front(); 			
		}

		if (adblScaleBeforeCalc>0)
		{

			for (int  i=0;i<anRef;i++)
			{
				lpDblBuffer[i] = lpDblBuffer[i]*adblScaleBeforeCalc;
			}
		}

		if (vOut_.size()< anRef)
		{
			vOut_.resize(anRef);
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

	try
	{
		_DECLARE_PERF_MEASURE_TIME();
		_BEGIN_PERF_MEASURE_TIME();
		CSigMath calcSpectral;
		int lnRet = calcSpectral.SpectralMeasureMent(lpDblBuffer,anRef,vOut_,type,iWinType_,iPrecision_);
		_END_PERF_MEASURE_TIME(STR("calcSpectral.SpectralMeasureMent:%d",anRef));
		if (adblScaleBeforeCalc>0)
		{
			for (int  i=0;i<vOut_.size();i++)
			{
				vOut_[i] = vOut_[i]/adblScaleBeforeCalc;
			}

			for (int  i=0;i<anRef;i++)
			{
				lpDblBuffer[i] = lpDblBuffer[i]/adblScaleBeforeCalc;
			}
		}
		return lnRet;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	return -1;
}

int CPlantInfoBase::WaveAddHead( const DATATYPE_WAVE* pWave_,int nWaveLen_, CWaveHead& WaveHead_,vector<BYTE>& vWave_ )
{
	
	if(nWaveLen_ <= 0)
	{
		vWave_.push_back(0);
		return 0;
	}

	int lnHeadSize = WaveHead_.GetSize();

	int lnFlagSize = sizeof(WaveHead_.m_nDataLen) + sizeof(WaveHead_.m_nMagicValue);

	int lnRealWaveSize = nWaveLen_*sizeof(DATATYPE_WAVE) ;

	int lnTotalSize = lnRealWaveSize + lnHeadSize;

	BEGIN_ERROR_HANDLE

	vWave_.resize(lnTotalSize);

	memcpy(&vWave_.front(),&WaveHead_,lnFlagSize);

	memcpy(&vWave_[lnFlagSize],&WaveHead_.m_pParam.front(),lnHeadSize - lnFlagSize);

	memcpy(&vWave_[lnHeadSize],pWave_,lnRealWaveSize);

	END_ERROR_HANDLE

	return lnTotalSize;

}

int CPlantInfoBase::WaveAddHead( const DATATYPE_WAVE* pWave_,int nWaveLen_, CWaveHead& WaveHead_,BYTE* pOutWave_ )
{
	if(nWaveLen_ <= 0)
	{
		return 0;
	}

	int lnHeadSize = WaveHead_.GetSize();

	int lnFlagSize = sizeof(WaveHead_.m_nDataLen) + sizeof(WaveHead_.m_nMagicValue);

	int lnRealWaveSize = nWaveLen_*sizeof(DATATYPE_WAVE) ;

	int lnTotalSize = lnRealWaveSize + lnHeadSize;

	BEGIN_ERROR_HANDLE

	memcpy(pOutWave_,&WaveHead_,lnFlagSize);

	memcpy(&pOutWave_[lnFlagSize],&WaveHead_.m_pParam.front(),lnHeadSize - lnFlagSize);

	memcpy(&pOutWave_[lnHeadSize],pWave_,lnRealWaveSize);

	END_ERROR_HANDLE

	return lnTotalSize;
}

CString CPlantInfoBase::Country()
{
	CString lstrRet;

	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE

			lstrRet = this->m_strCountry;

		END_ERROR_HANDLE

			lpInfoLocker->EndRead();
	}
	return lstrRet;
}

void CPlantInfoBase::Country( CString val )
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE

			m_strCountry = val;

		END_ERROR_HANDLE

			lpInfoLocker->EndWrite();
	}	
}

CString CPlantInfoBase::PlantDisplayName() 
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	CString lstrRet;

	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE;

		lstrRet = this->m_strPlantDisplayName;

		END_ERROR_HANDLE;

		lpInfoLocker->EndRead();
	}
	return lstrRet;
}

void CPlantInfoBase::PlantDisplayName( CString val )
{	
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE;

		m_strPlantDisplayName = val;

		END_ERROR_HANDLE;

		lpInfoLocker->EndWrite();
	}	
}

CString CPlantInfoBase::CompanyDisplayName()
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	CString lstrRet;

	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE;

		lstrRet = this->m_strCompanyDisplayName;

		END_ERROR_HANDLE;

		lpInfoLocker->EndRead();
	}
	return lstrRet;

}

void CPlantInfoBase::CompanyDisplayName( CString val )
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE;

		m_strCompanyDisplayName = val;

		END_ERROR_HANDLE;

		lpInfoLocker->EndWrite();
	}	

}

CString CPlantInfoBase::FactoryDisplayName()
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	CString lstrRet;

	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE;

		lstrRet = this->m_strFactoryDisplayName;

		END_ERROR_HANDLE;

		lpInfoLocker->EndRead();
	}
	return lstrRet;
}

void CPlantInfoBase::FactoryDisplayName( CString val )
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE;

		m_strFactoryDisplayName = val;

		END_ERROR_HANDLE;

		lpInfoLocker->EndWrite();
	}		
}

CString CPlantInfoBase::SetDisplayName()
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	CString lstrRet;

	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE;

		lstrRet = this->m_strSetDisplayName;

		END_ERROR_HANDLE;

		lpInfoLocker->EndRead();
	}
	return lstrRet;

}

void CPlantInfoBase::SetDisplayName( CString val )
{
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;

	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE;

		m_strSetDisplayName = val;

		END_ERROR_HANDLE;

		lpInfoLocker->EndWrite();
	}	
}

CString CPlantInfoBase::GroupDisplayName()
{
	CString lstrRet;
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginRead();

		BEGIN_ERROR_HANDLE;

		lstrRet = this->m_strGroupDisplayName;

		END_ERROR_HANDLE;

		lpInfoLocker->EndRead();
	}
	return lstrRet;
}

void CPlantInfoBase::GroupDisplayName( CString val )
{	
	CTimeOutReadWriteLock * lpInfoLocker = &m_oInfoLock;
	if (NULL!=lpInfoLocker)
	{
		lpInfoLocker->BeginWrite();

		BEGIN_ERROR_HANDLE;

		m_strGroupDisplayName = val;

		END_ERROR_HANDLE;

		lpInfoLocker->EndWrite();
	}	
}

bool CPlantInfoBase::GetScaleCoefAndPrecisionFlag( CWaveHead& sWaveHead_,float& fScaleCoef_,int& nPrecisionEnabled_ ,float& fMaxValue_,float& fMinValue_)
{
	if(sWaveHead_.GetLength() <= 0)
	{
		fScaleCoef_ = 10.0;
		nPrecisionEnabled_ = 0;
		fMaxValue_ = 0;
		fMinValue_ = 0;
	}
	else if(sWaveHead_.GetLength() == 1)
	{
		fScaleCoef_ = sWaveHead_.m_pParam[0];
		nPrecisionEnabled_ = 0;
		fMaxValue_ = 0;
		fMinValue_ = 0;
	}
	else
	{
		fScaleCoef_ = sWaveHead_.m_pParam[0];
		nPrecisionEnabled_ = sWaveHead_.m_pParam[1];
		fMinValue_ = sWaveHead_.m_pParam[2];
		fMaxValue_ = sWaveHead_.m_pParam[3];
		if(nPrecisionEnabled_ != 1)
		{
			nPrecisionEnabled_ = 0;
		}
	}
	return true;
}
BOOL CPlantInfoBase::DeleteSpecCountTrendRecord( IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iTotalRealWavePtNum_,int iEventID_ )
{
	int lnDataCount = 0;

	int lnTotalDataSmpNum = iTotalWavePtNum_;

	if(lnTotalDataSmpNum <= 0)
	{
		lnDataCount = 1;
	}
	else
	{
		if(iTotalRealWavePtNum_%lnTotalDataSmpNum == 0)
		{
			lnDataCount = iTotalRealWavePtNum_/lnTotalDataSmpNum;
		}
		else
		{
			int lnLeftNum = iTotalRealWavePtNum_ - (iTotalRealWavePtNum_/lnTotalDataSmpNum)*lnTotalDataSmpNum;

			if(lnLeftNum*1.0 > 0.1*lnTotalDataSmpNum || iTotalRealWavePtNum_ < lnTotalDataSmpNum)
			{
				lnDataCount = iTotalRealWavePtNum_/lnTotalDataSmpNum + 1;
			}
			else
			{
				lnDataCount = iTotalRealWavePtNum_/lnTotalDataSmpNum;
			}
			
		}
	}

	if(lnDataCount <= 0)
	{
		return TRUE;
	}


	CString lstrTableName = sTableName_.c_str();
	lstrTableName.MakeUpper();

	if(lstrTableName.Find("ALARM") >= 0 || lstrTableName.Find("SUD") >= 0)
	{
		CString lstrTime;
		map<int,int> lmEventID;
		pDB_->Cmd("SELECT TOP %d %s,%s FROM %s ORDER BY %s ASC ",lnDataCount,gc_cEventID,gc_cTime,sTableName_.c_str(),gc_cTime);
		while(pDB_->More())
		{
			lstrTime.Append("'");
			lstrTime.AppendFormat("%s",pDB_->GetTime(gc_cTime).Format());
			lstrTime.Append("',");
			
			lmEventID.insert(make_pair(pDB_->GetLong(gc_cEventID),pDB_->GetLong(gc_cEventID)));
			
		}

		if(lstrTime.GetLength() > 0)
		{
			lstrTime.Delete(lstrTime.GetLength() - 1);
			pDB_->Cmd("DELETE FROM %s WHERE %s IN(%s)",sTableName_.c_str(),gc_cTime,lstrTime);
			if(!pDB_->Exec())
			{
				return FALSE;
			}

			if(lstrTableName.Find("ALARM") >= 0)
			{
				for(map<int,int>::iterator it = lmEventID.begin(); it != lmEventID.end(); ++it)
				{
					pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,sTableName_.c_str(),gc_cEventID,it->first);
					if(!pDB_->More() && iEventID_ != it->first)
					{
						if(lstrTableName.Find("VIB") >= 0)
						{
							pDB_->Cmd("delete from [%s] where [%s] = %d and [%s] in(%d,%d)",_sAlarmLogTblName.c_str(),gc_cEventID,it->first,gc_cChannType,GE_VIBCHANN,GE_AXIALCHANN);
							pDB_->Exec();
						}
						else
						{
							pDB_->Cmd("delete from [%s] where [%s] = %d and [%s] in(%d,%d,%d)",_sAlarmLogTblName.c_str(),gc_cEventID,it->first,gc_cChannType,GE_DYNPRESSCHANN,GE_RODSINKCHANN,GE_DYNSTRESSCHANN);
							pDB_->Exec();
						}
						
					}
				}
			}
			else
			{
				for(map<int,int>::iterator it = lmEventID.begin(); it != lmEventID.end(); ++it)
				{
					pDB_->Cmd("select top 1 [%s] from [%s] where [%s] = %d",gc_cEventID,sTableName_.c_str(),gc_cEventID,it->first);
					if(!pDB_->More() && iEventID_ != it->first)
					{
						pDB_->Cmd("delete from [%s] where [%s] = %d ",_sSudLogTblName.c_str(),gc_cEventID,it->first);
						pDB_->Exec();
					}
				}
			}

			
		}
	}
	else
	{
		CString lstrDataID;
		pDB_->Cmd("SELECT TOP %d %s FROM %s ORDER BY %s DESC ",lnDataCount,gc_cID,sTableName_.c_str(),gc_cID);
		while(pDB_->More())
		{
			lstrDataID.AppendFormat("%d,",pDB_->GetLong(gc_cID));
		}

		if(lstrDataID.GetLength() > 0)
		{
			lstrDataID.Delete(lstrDataID.GetLength() - 1);
			pDB_->Cmd("DELETE FROM %s WHERE %s IN(%s)",sTableName_.c_str(),gc_cID,lstrDataID);
			if(!pDB_->Exec())
			{
				return FALSE;
			}
		}
	}

	return TRUE;

}
COfflineChannParam::COfflineChannParam()
{
	_tTime.ParseDateTime("2000-01-01 00:00:00");
	_fValue = 0;
}

COfflineChannParam::~COfflineChannParam()
{
}

int COfflineChannParam::SetData(CBHDateTime &tTime, float fVal)
{
	_tTime   = tTime;
	_fValue  = fVal;

	return 0;
}

int COfflineChannParam::GetData(CBHDateTime & tTime, float & fVal)
{
	tTime = _tTime;
	fVal  = _fValue;

	return _fValue;
}

CBHDateTime COfflineChannParam::GetTime()
{
	return _tTime;
}

float COfflineChannParam::GetValue()
{
	return _fValue;
}

