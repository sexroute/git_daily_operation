#include "stdafx.h"
#include <BHKD_utilities.h>
#include <SqliteAdapter.h>
#include <additions/DebugHelper.h>
#include <vector>
#include <Sigdefine.h>
#include <SigMath.h>
#include <float.h>
using namespace std;

CString g_strMainDbName="";
int g_iDbType=0;


CString GetModuleFilePath()
{
	TCHAR Buffer[_MAX_PATH];
	TCHAR *pChar = NULL;

	CString sPath;

	::GetModuleFileName(NULL,Buffer,sizeof(Buffer)/sizeof(TCHAR));

	pChar = Buffer + _tcslen(Buffer);

	while(pChar != Buffer && *pChar != _T('\\'))
	{
		--pChar;
	}

	*( ++pChar ) = _T('\0');

	return Buffer;
}

CString GetModuleFileBase()
{
	TCHAR Buffer[_MAX_PATH];
	TCHAR *pChar = NULL;

	CString sPath;

	::GetModuleFileName(NULL,Buffer,sizeof(Buffer)/sizeof(TCHAR));

	pChar = Buffer + _tcslen(Buffer);

	while(pChar != Buffer && *pChar != _T('\\'))
	{
		if(*pChar == '.') *pChar = '\0';

		--pChar;
	}

	return pChar + 1;
}



namespace BHKD{
CAxisAlarmParam::CAxisAlarmParam()
{
	_bCheckF=false;
	_bCheckS=false;
	_bCheckTrend=false;
}

CAxisAlarmParam::~CAxisAlarmParam()
{

}

CAxisAlarmParam& CAxisAlarmParam::operator=(const CAxisAlarmParam& aParam_)
{
	_bCheckTrend=aParam_._bCheckTrend;
	_bCheckS=aParam_._bCheckS;
	_bCheckF=aParam_._bCheckF;
	_thLearnParamS=aParam_._thLearnParamS;
	_thLearnParamF=aParam_._thLearnParamF;
	_trendCheckParam=aParam_._trendCheckParam;
	return *this;
}

S_SetInfo::S_SetInfo()
{
	_iID=0;
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cIP,0,D_BHKD_IP_LEN);
	_iPort=7001;
	_iSendInterval=5;
	_iIfSend=-1;
	_iSudSendInterval=-1;
	_iAlarmSendInterval=-1;
	memset(_cTmIP,0,D_BHKD_IP_LEN);
	_iTmPort=7001;
	memset(_cParentID,0,D_BHKD_SETID_LEN);
	_iChildFlag=-1;
}

S_SetInfo& S_SetInfo::operator =(const S_SetInfo& sSetInfo_)
{
	_iID=sSetInfo_._iID;
	strncpy(_cCompany,sSetInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,sSetInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID,sSetInfo_._cSetID,D_BHKD_SETID_LEN-1);
	strncpy(_cParentID,sSetInfo_._cParentID,D_BHKD_SETID_LEN-1);
	strncpy(_cIP,sSetInfo_._cIP,D_BHKD_IP_LEN-1);
	strncpy(_cTmIP,sSetInfo_._cTmIP,D_BHKD_IP_LEN-1);
	_iPort=sSetInfo_._iPort;
	_iTmPort=sSetInfo_._iTmPort;
	_iSendInterval=sSetInfo_._iSendInterval;
	_iIfSend=sSetInfo_._iIfSend;
	_iSudSendInterval=sSetInfo_._iSudSendInterval;
	_iAlarmSendInterval=sSetInfo_._iAlarmSendInterval;
	_iChildFlag=sSetInfo_._iChildFlag;
	return *this;
}

#ifdef _BHKD_SERVER_SIDE_
//从指定的数据连接中得到设备详细信息
bool  S_SetInfo::GetSetInfo(CCustomDB *pDB_)
{
	if (NULL==pDB_) return false;
	return GetSetInfo((CSimADO*)pDB_);

}
//从指定的数据连接中得到设备详细信息
bool  S_SetInfo::GetSetInfo(CSimADO *pDB_)
{
	_iID=pDB_->GetLong(gc_cID);
	strncpy(_cCompany,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN-1);
	strncpy(_cParentID,pDB_->Get(gc_cParent_ID),D_BHKD_SETID_LEN-1);
	strncpy(_cIP,pDB_->Get(gc_cCenterMw_IP),D_BHKD_IP_LEN-1);
	strncpy(_cTmIP,pDB_->Get(gc_cTm_IP),D_BHKD_IP_LEN-1);
	_iPort=pDB_->GetLong(gc_cCenterMw_Port);
	_iTmPort=pDB_->GetLong(gc_cTm_Port);
	_iSendInterval=pDB_->GetLong(gc_cSend_Interval);
	_iIfSend=pDB_->GetLong(gc_cIf_Send);
	_iSudSendInterval=pDB_->GetLong(gc_cSud_Send_Interval);
	_iAlarmSendInterval=pDB_->GetLong(gc_cAlarm_Send_Interval);
	_iChildFlag=pDB_->GetLong(gc_cChild_Flag);
	return true;
}


bool  S_SetInfo::GetSetInfo(IDBInterFace *pDB_)
{
	_iID=pDB_->GetLong(gc_cID);
	strncpy(_cCompany,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cParentID,pDB_->Get(gc_cParent_ID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cIP,pDB_->Get(gc_cCenterMw_IP).c_str(),D_BHKD_IP_LEN-1);
	strncpy(_cTmIP,pDB_->Get(gc_cTm_IP).c_str(),D_BHKD_IP_LEN-1);
	_iPort=pDB_->GetLong(gc_cCenterMw_Port);
	_iTmPort=pDB_->GetLong(gc_cTm_Port);
	_iSendInterval=pDB_->GetLong(gc_cSend_Interval);
	_iIfSend=pDB_->GetLong(gc_cIf_Send);
	_iSudSendInterval=pDB_->GetLong(gc_cSud_Send_Interval);
	_iAlarmSendInterval=pDB_->GetLong(gc_cAlarm_Send_Interval);
	_iChildFlag=pDB_->GetLong(gc_cChild_Flag);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

///////////////////////////////////////////////////////////////////////////

CCustomParamInfo::CCustomParamInfo()
{
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cTempletID,0,20);
	memset(_cSegName,0,100);
	_eDataType = E_CUSTOMPARAM_TIMEDOMAIN;
	_eUnitType= E_CUSTOMPARAM_TIME;
	_eSigAnaType= E_CUSTOMPARAM_TIMEWAVE;
	_eCharType = E_CUSTOMPARAM_PEAKVALUE;
	_iID = 0;
	_iSegNum = 0;
	_iSegStartNo = 0;
	_iPreProcType   = 0;
	_fDataStartValue = .0;
	_fDataEndValue  = .0;
	_fBandStartValue = .0;
	_fBandEndValue   = .0;
	this->_fPREPROC_B = .0;
	this->_fPREPROC_K = 1;
}

CCustomParamInfo::~CCustomParamInfo()
{
}

CCustomParamInfo& CCustomParamInfo::operator=(const CCustomParamInfo& customParamInfo_)
{
	strncpy(_cCompany,customParamInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory, customParamInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID, customParamInfo_._cSetID,D_BHKD_SETID_LEN-1);
	strncpy(_cTempletID, customParamInfo_._cTempletID,sizeof(_cTempletID)-1);
	strncpy(_cSegName, customParamInfo_._cSegName,sizeof(_cSegName)-1);
	_eDataType = customParamInfo_._eDataType;
	_eUnitType= customParamInfo_._eUnitType;
	_eSigAnaType= customParamInfo_._eSigAnaType;
	_eCharType = customParamInfo_._eCharType;
	_iID = customParamInfo_._iID;
	_iSegNum = customParamInfo_._iSegNum;
	_iSegStartNo = customParamInfo_._iSegStartNo;
	_iPreProcType   = customParamInfo_._iPreProcType;
	_fDataStartValue = customParamInfo_._fDataStartValue;
	_fDataEndValue  = customParamInfo_._fDataEndValue;
	_fBandStartValue = customParamInfo_._fBandStartValue;
    _fBandEndValue   = customParamInfo_._fBandEndValue;
	return *this;
}

#ifdef _BHKD_SERVER_SIDE_
bool CCustomParamInfo::GetCustomParamInfo(CCustomDB *pDB_)
{
	return GetCustomParamInfo((CSimADO*)pDB_);
}

bool CCustomParamInfo::GetCustomParamInfo(CSimADO *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN-1);
	strncpy(_cTempletID,pDB_->Get(gc_cTempletID),sizeof(_cTempletID)-1);
	strncpy(_cSegName ,pDB_->Get(gc_cSegName),sizeof(_cSegName)-1);
	_eCharType =(E_CUSTOMPARAM_CHARTYPE)pDB_->GetLong(gc_cChar_Type);
	_eDataType =(E_CUSTOMPARAM_DATATYPE)pDB_->GetLong(gc_cData_Type);
	_eSigAnaType =(E_CUSTOMPARAM_SIGANATYPE)pDB_->GetLong(gc_cSignal_Ana_Type);
	_eUnitType=(E_CUSTOMPARAM_UNITTYPE)pDB_->GetLong(gc_cUnit_Type);
	_fBandEndValue=(float)pDB_->GetDouble(gc_cBand_End_Value);
	_fBandStartValue=(float)pDB_->GetDouble(gc_cBand_Start_Value);
	_fDataEndValue=(float)pDB_->GetDouble(gc_cData_End_Value);
	_fDataStartValue=(float)pDB_->GetDouble(gc_cData_Start_Value);
	_iID=pDB_->GetLong(gc_cID);
	_iPreProcType=pDB_->GetLong(gc_cPreProc_Type);
	_iSegNum=pDB_->GetLong(gc_cSeg_Num);
	_iSegStartNo=pDB_->GetLong(gc_cSeg_Start_NO);
	_fPREPROC_B =  pDB_->GetDouble(gc_cPreProc_B);
	_fPREPROC_K = pDB_->GetDouble(gc_cPreProc_K);
	return true;
}

bool CCustomParamInfo::GetCustomParamInfo(IDBInterFace *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cTempletID,pDB_->Get(gc_cTempletID).c_str(),sizeof(_cTempletID)-1);
	strncpy(_cSegName ,pDB_->Get(gc_cSegName).c_str(),sizeof(_cSegName)-1);
	_eCharType =(E_CUSTOMPARAM_CHARTYPE)pDB_->GetLong(gc_cChar_Type);
	_eDataType =(E_CUSTOMPARAM_DATATYPE)pDB_->GetLong(gc_cData_Type);
	_eSigAnaType =(E_CUSTOMPARAM_SIGANATYPE)pDB_->GetLong(gc_cSignal_Ana_Type);
	_eUnitType=(E_CUSTOMPARAM_UNITTYPE)pDB_->GetLong(gc_cUnit_Type);
	_fBandEndValue=(float)pDB_->GetDouble(gc_cBand_End_Value);
	_fBandStartValue=(float)pDB_->GetDouble(gc_cBand_Start_Value);
	_fDataEndValue=(float)pDB_->GetDouble(gc_cData_End_Value);
	_fDataStartValue=(float)pDB_->GetDouble(gc_cData_Start_Value);
	_iID=pDB_->GetLong(gc_cID);
	_iPreProcType=pDB_->GetLong(gc_cPreProc_Type);
	_iSegNum=pDB_->GetLong(gc_cSeg_Num);
	_iSegStartNo=pDB_->GetLong(gc_cSeg_Start_NO);
	_fPREPROC_B =  pDB_->GetDouble(gc_cPreProc_B);
	_fPREPROC_K = pDB_->GetDouble(gc_cPreProc_K);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

//////////////////////////////////////////////////////////////////////////
CCustomAlarmParamInfo::CCustomAlarmParamInfo()
{
	memset(_cSegName,0,100);
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cAlarmTempletID,0,20);
	_iID = 0;
	_iSegNum=0;
	_iSegStartNo=0;
	_iJudgeType=0;
	_iAlarmType=0;
	_iIF_Alarm=0;
	_iAlarmGroup=0;
	_fAlarmLTH=.0;
	_fAlarmHTH=.0;
}
CCustomAlarmParamInfo::~CCustomAlarmParamInfo()
{
}
CCustomAlarmParamInfo& CCustomAlarmParamInfo::operator=(const CCustomAlarmParamInfo & customAlarmParamInfo_)
{
	strncpy(_cCompany,customAlarmParamInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory, customAlarmParamInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID, customAlarmParamInfo_._cSetID,D_BHKD_SETID_LEN-1);
	strncpy(_cAlarmTempletID, customAlarmParamInfo_._cAlarmTempletID,sizeof(_cAlarmTempletID)-1);
	strncpy(_cSegName, customAlarmParamInfo_._cSegName,sizeof(_cSegName)-1);
	_iID        =customAlarmParamInfo_._iID;
	_iSegStartNo=customAlarmParamInfo_._iSegStartNo;
	_iSegNum    =customAlarmParamInfo_._iSegNum;
	_iJudgeType =customAlarmParamInfo_._iJudgeType;
	_iAlarmType =customAlarmParamInfo_._iAlarmType;
	_iAlarmGroup=customAlarmParamInfo_._iAlarmGroup;
	_iIF_Alarm  =customAlarmParamInfo_._iIF_Alarm;
	_fAlarmLTH  =customAlarmParamInfo_._fAlarmLTH;
	_fAlarmHTH  =customAlarmParamInfo_._fAlarmHTH;
	return *this;
}

#ifdef _BHKD_SERVER_SIDE_
bool CCustomAlarmParamInfo::GetCustomAlarmParamInfo(CCustomDB *pDB_)
{
	return GetCustomAlarmParamInfo((CSimADO*)pDB_);
}

bool CCustomAlarmParamInfo::GetCustomAlarmParamInfo(CSimADO *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN-1);
	strncpy(_cAlarmTempletID,pDB_->Get(gc_cAlarm_TempletID),sizeof(_cAlarmTempletID)-1);
	strncpy(_cSegName ,pDB_->Get(gc_cSegName),sizeof(_cSegName));
	_iID=pDB_->GetLong(gc_cID);
	_iSegNum=pDB_->GetLong(gc_cSeg_Num);
	_iSegStartNo=pDB_->GetLong(gc_cAlarm_Start_SegNO);
	_iJudgeType=pDB_->GetLong(gc_cJudge_Type);
	_iAlarmType=pDB_->GetLong(gc_cAlarm_Type);
	_iIF_Alarm=pDB_->GetLong(gc_cIF_Alarm);
	_iAlarmGroup=pDB_->GetLong(gc_cAlarm_Group);
	_fAlarmLTH=(float)pDB_->GetDouble(gc_cAlarm_LTH);
	_fAlarmHTH=(float)pDB_->GetDouble(gc_cAlarm_HTH);
	return true;
}

bool CCustomAlarmParamInfo::GetCustomAlarmParamInfo(IDBInterFace *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cAlarmTempletID,pDB_->Get(gc_cAlarm_TempletID).c_str(),sizeof(_cAlarmTempletID)-1);
	strncpy(_cSegName ,pDB_->Get(gc_cSegName).c_str(),sizeof(_cSegName));
	_iID=pDB_->GetLong(gc_cID);
	_iSegNum=pDB_->GetLong(gc_cSeg_Num);
	_iSegStartNo=pDB_->GetLong(gc_cAlarm_Start_SegNO);
	_iJudgeType=pDB_->GetLong(gc_cJudge_Type);
	_iAlarmType=pDB_->GetLong(gc_cAlarm_Type);
	_iIF_Alarm=pDB_->GetLong(gc_cIF_Alarm);
	_iAlarmGroup=pDB_->GetLong(gc_cAlarm_Group);
	_fAlarmLTH=(float)pDB_->GetDouble(gc_cAlarm_LTH);
	_fAlarmHTH=(float)pDB_->GetDouble(gc_cAlarm_HTH);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

//////////////////////////////////////////////////////////////////////////

CChannLocations::CChannLocations()
{
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cLocationName,0,100);
	memset(_cComment,0,100);
	_iBearingIDX=0;
	_iShaftNumber=0;
	_iSectionIDX=0;
	_iLocationIDX=0;
	_iBearingTblType=0;
}
CChannLocations::~CChannLocations()
{
}
CChannLocations& CChannLocations::operator=(const CChannLocations& channLocations_)
{
	strncpy(_cCompany,channLocations_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory, channLocations_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID, channLocations_._cSetID,D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo, channLocations_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cLocationName, channLocations_._cLocationName,sizeof(_cLocationName)-1);
	strncpy(_cComment, channLocations_._cComment,sizeof(_cComment)-1);
	_iBearingIDX=channLocations_._iBearingIDX;
	_iShaftNumber=channLocations_._iShaftNumber;
	_iSectionIDX=channLocations_._iSectionIDX;
	_iLocationIDX=channLocations_._iLocationIDX;
	_iBearingTblType=channLocations_._iBearingTblType;
	return *this;
}
#ifdef _BHKD_SERVER_SIDE_
bool CChannLocations::GetChannLocations(CCustomDB *pDB_)
{
	return GetChannLocations((CSimADO*)pDB_);
}
bool CChannLocations::GetChannLocations(CSimADO *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN);
	strncpy(_cLocationName,pDB_->Get(gc_cLocation_Name),sizeof(_cLocationName));
	strncpy(_cComment,pDB_->Get(gc_cComment),sizeof(_cComment));
	_iBearingIDX=pDB_->GetLong(gc_cBearing_IDX);
	_iShaftNumber=pDB_->GetLong(gc_cShaft_Number);
	_iSectionIDX=pDB_->GetLong(gc_cSection_IDX);
	_iLocationIDX=pDB_->GetLong(gc_cLocation_IDX);
	_iBearingTblType=pDB_->GetLong(gc_cBearingTbl_Type);
	return true;
}

bool CChannLocations::GetChannLocations(IDBInterFace *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN);
	strncpy(_cLocationName,pDB_->Get(gc_cLocation_Name).c_str(),sizeof(_cLocationName));
	strncpy(_cComment,pDB_->Get(gc_cComment).c_str(),sizeof(_cComment));
	_iBearingIDX=pDB_->GetLong(gc_cBearing_IDX);
	_iShaftNumber=pDB_->GetLong(gc_cShaft_Number);
	_iSectionIDX=pDB_->GetLong(gc_cSection_IDX);
	_iLocationIDX=pDB_->GetLong(gc_cLocation_IDX);
	_iBearingTblType=pDB_->GetLong(gc_cBearingTbl_Type);
	return true;
}
#endif//_BHKD_SERVER_SIDE_


//////////////////////////////////////////////////////////////////////////
CChannSections::CChannSections()
{
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cSectionName,0,100);
	memset(_cComment,0,100);
	_iSectionIDX=-1;
	_iGearBoxIDX=-1;
}
CChannSections::~CChannSections()
{
}
CChannSections& CChannSections::operator=(const CChannSections& channSections_)
{
	strncpy(_cCompany,channSections_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory, channSections_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID, channSections_._cSetID,D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo, channSections_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cSectionName, channSections_._cSectionName,sizeof(_cSectionName)-1);
	strncpy(_cComment, channSections_._cComment,sizeof(_cComment)-1);
	_iSectionIDX=channSections_._iSectionIDX;
	_iGearBoxIDX=channSections_._iGearBoxIDX;
	return *this;
}
#ifdef _BHKD_SERVER_SIDE_
bool CChannSections::GetChannSections(CCustomDB *pDB_)
{
	return GetChannSections((CSimADO*)pDB_);
}
bool CChannSections::GetChannSections(CSimADO *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cSectionName,pDB_->Get(gc_cSection_Name),sizeof(_cSectionName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment),sizeof(_cComment)-1);
	_iSectionIDX=pDB_->GetLong(gc_cSection_IDX);
	_iGearBoxIDX=pDB_->GetLong(gc_cGearBox_IDX);
	return true;
}



bool CChannSections::GetChannSections(IDBInterFace *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cSectionName,pDB_->Get(gc_cSection_Name).c_str(),sizeof(_cSectionName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment).c_str(),sizeof(_cComment)-1);
	_iSectionIDX=pDB_->GetLong(gc_cSection_IDX);
	_iGearBoxIDX=pDB_->GetLong(gc_cGearBox_IDX);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

//////////////////////////////////////////////////////////////////////////
CBearingParamInfo::CBearingParamInfo()
{
	memset(_cBearingName,0,100);
	memset(_cComment,0,100);
	_fPitchDiameter=.0;
	_fElementDiameter=.0;
	_fContactAngle=.0;
	_fHousingDiameter=.0;
	_fBoreDiameter=.0;
	_iBearingIDX=0;
	_iBearingType=0;
	_iRowsNum=0;
	_iElementsNum=0;
	_iBearingCode=0;
}
CBearingParamInfo::~CBearingParamInfo()
{
}
CBearingParamInfo& CBearingParamInfo::operator=(const CBearingParamInfo& bearingParamInfo_)
{
	strncpy(_cBearingName, bearingParamInfo_._cBearingName,sizeof(_cBearingName)-1);
	strncpy(_cComment, bearingParamInfo_._cComment,sizeof(_cComment)-1);
	_fPitchDiameter=bearingParamInfo_._fPitchDiameter;
	_fElementDiameter=bearingParamInfo_._fElementDiameter;
	_fContactAngle=bearingParamInfo_._fContactAngle;
	_fHousingDiameter=bearingParamInfo_._fHousingDiameter;
	_fBoreDiameter=bearingParamInfo_._fBoreDiameter;
	_iBearingIDX=bearingParamInfo_._iBearingIDX;
	_iBearingType=bearingParamInfo_._iBearingType;
	_iRowsNum=bearingParamInfo_._iRowsNum;
	_iElementsNum=bearingParamInfo_._iElementsNum;
	_iBearingCode=bearingParamInfo_._iBearingCode;
	return *this;
}
#ifdef _BHKD_SERVER_SIDE_

bool CBearingParamInfo::GetBearingParamInfo(CString & astrSql)
{
	CSqliteAdapter loAdaper;

	TRACE(_T("%s\r\n"),astrSql);

	if (loAdaper.exec(astrSql))
	{
		strncpy(_cBearingName,loAdaper.Get(gc_cBearing_Id).c_str(),sizeof(_cBearingName)-1);
		strncpy(_cComment,loAdaper.Get(gc_cComments).c_str(),sizeof(_cComment)-1);
		_fPitchDiameter=loAdaper.GetDouble(gc_cPith_Diameter);
		_fElementDiameter=loAdaper.GetDouble(gc_cElement_Diameter);
		_fContactAngle=loAdaper.GetDouble(gc_cContact_Angle);
		_fHousingDiameter=loAdaper.GetDouble(gc_cHousing_Diameter);
		_fBoreDiameter=loAdaper.GetDouble(gc_cBore_Diameter);
		_iBearingIDX=loAdaper.GetLong(gc_cBearing_IDX);
		_iBearingType=loAdaper.GetLong(gc_cBearing_Type);
		_iRowsNum=loAdaper.GetLong(gc_cRows_Num);
		_iElementsNum=loAdaper.GetLong(gc_cElements_Num);
		_iBearingCode=loAdaper.GetLong(gc_cBearing_Code);

		return  true;
	}

	else
	{
		return false;
	}


}

bool CBearingParamInfo::_GetBearingParamInfo(CCustomDB *pDB_)
{
	return _GetBearingParamInfo((CSimADO*) pDB_);
}


bool CBearingParamInfo::_GetBearingParamInfo(CSimADO *pDB_)
{
	strncpy(_cBearingName,pDB_->Get(gc_cBearing_Name),sizeof(_cBearingName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment),sizeof(_cComment)-1);
	_fPitchDiameter=pDB_->GetDouble(gc_cPith_Diameter);
	_fElementDiameter=pDB_->GetDouble(gc_cElement_Diameter);
	_fContactAngle=pDB_->GetDouble(gc_cContact_Angle);
	_fHousingDiameter=pDB_->GetDouble(gc_cHousing_Diameter);
	_fBoreDiameter=pDB_->GetDouble(gc_cBore_Diameter);
	_iBearingIDX=pDB_->GetLong(gc_cBearing_IDX);
	_iBearingType=pDB_->GetLong(gc_cBearing_Type);
	_iRowsNum=pDB_->GetLong(gc_cRows_Num);
	_iElementsNum=pDB_->GetLong(gc_cElements_Num);
	_iBearingCode=pDB_->GetLong(gc_cBearing_Code);
	return true;
}

bool CBearingParamInfo::_GetBearingParamInfo(IDBInterFace *pDB_)
{
	strncpy(_cBearingName,pDB_->Get(gc_cBearing_Name).c_str(),sizeof(_cBearingName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment).c_str(),sizeof(_cComment)-1);
	_fPitchDiameter=pDB_->GetDouble(gc_cPith_Diameter);
	_fElementDiameter=pDB_->GetDouble(gc_cElement_Diameter);
	_fContactAngle=pDB_->GetDouble(gc_cContact_Angle);
	_fHousingDiameter=pDB_->GetDouble(gc_cHousing_Diameter);
	_fBoreDiameter=pDB_->GetDouble(gc_cBore_Diameter);
	_iBearingIDX=pDB_->GetLong(gc_cBearing_IDX);
	_iBearingType=pDB_->GetLong(gc_cBearing_Type);
	_iRowsNum=pDB_->GetLong(gc_cRows_Num);
	_iElementsNum=pDB_->GetLong(gc_cElements_Num);
	_iBearingCode=pDB_->GetLong(gc_cBearing_Code);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

//////////////////////////////////////////////////////////////////////////
CGearBoxParamInfo::CGearBoxParamInfo()
{
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cSetID,0,D_BHKD_SETID_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cGearBoxName,0,100);
	memset(_cComment,0,100);
	_iGearBoxIDX=0;
	_iGearBoxLevel=0;
	_iGearBoxType=0;
	_iDriverType=0;
	_iPlanetNum=0;
	_iGearNumSun=0;
	_iGearNumPlanet=0;
	_iGearNumGalaxy=0;
	_iIsSync=0;
	_iGearNumZ12=0;
	_iGearNumZ21=0;
	_iGearNumZ22=0;
	_iGearNumZ31=0;
	_iGearNumZ32=0;
	_iGearNumZ41=0;
	_iGearNumZ42=0;
	_iGearNumZ51=0;
	_iGearNumZ52=0;
	_iGearNumZ61=0;
}
CGearBoxParamInfo::~CGearBoxParamInfo()
{
}
CGearBoxParamInfo& CGearBoxParamInfo::operator=(const CGearBoxParamInfo& gearBoxParamInfo_)
{
	strncpy(_cCompany,gearBoxParamInfo_._cCompany,(D_BHKD_COMPANYNAME_LEN-1));
	strncpy(_cFactory, gearBoxParamInfo_._cFactory,(D_BHKD_FACTORYNAME_LEN-1));
	strncpy(_cSetID, gearBoxParamInfo_._cSetID,(D_BHKD_SETID_LEN-1));
	strncpy(_cPlantNo, gearBoxParamInfo_._cPlantNo,(D_BHKD_PLANTNO_LEN-1));
	strncpy(_cGearBoxName, gearBoxParamInfo_._cGearBoxName,sizeof(_cGearBoxName)-1);
	strncpy(_cComment, gearBoxParamInfo_._cComment,sizeof(_cComment)-1);
	_iGearBoxIDX=gearBoxParamInfo_._iGearBoxIDX;
	_iGearBoxLevel=gearBoxParamInfo_._iGearBoxLevel;
	_iGearBoxType=gearBoxParamInfo_._iGearBoxType;
	_iDriverType=gearBoxParamInfo_._iDriverType;
	_iPlanetNum=gearBoxParamInfo_._iPlanetNum;
	_iGearNumSun=gearBoxParamInfo_._iGearNumSun;
	_iGearNumPlanet=gearBoxParamInfo_._iGearNumPlanet;
	_iGearNumGalaxy=gearBoxParamInfo_._iGearNumGalaxy;
	_iIsSync=gearBoxParamInfo_._iIsSync;
	_iGearNumZ12=gearBoxParamInfo_._iGearNumZ12;
	_iGearNumZ21=gearBoxParamInfo_._iGearNumZ21;
	_iGearNumZ22=gearBoxParamInfo_._iGearNumZ22;
	_iGearNumZ31=gearBoxParamInfo_._iGearNumZ31;
	_iGearNumZ32=gearBoxParamInfo_._iGearNumZ32;
	_iGearNumZ41=gearBoxParamInfo_._iGearNumZ41;
	_iGearNumZ42=gearBoxParamInfo_._iGearNumZ42;
	_iGearNumZ51=gearBoxParamInfo_._iGearNumZ51;
	_iGearNumZ52=gearBoxParamInfo_._iGearNumZ52;
	_iGearNumZ61=gearBoxParamInfo_._iGearNumZ61;
	return *this;
}
#ifdef  _BHKD_SERVER_SIDE_
bool CGearBoxParamInfo::GetGearBoxParamInfo(CCustomDB *pDB_)
{
	return GetGearBoxParamInfo((CSimADO*)pDB_);
}
bool CGearBoxParamInfo::GetGearBoxParamInfo(CSimADO *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID),D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cGearBoxName,pDB_->Get(gc_cGearBox_Name),sizeof(_cGearBoxName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment),sizeof(_cComment)-1);
	_iGearBoxIDX=pDB_->GetLong(gc_cGearBox_IDX);
	_iGearBoxLevel=pDB_->GetLong(gc_cGearBox_Level);
	_iGearBoxType=pDB_->GetLong(gc_cGearBox_Type);
	_iDriverType=pDB_->GetLong(gc_cDriver_Type);
	_iPlanetNum=pDB_->GetLong(gc_cPlanet_Num);
	_iGearNumSun=pDB_->GetLong(gc_cGear_Num_Sun);
	_iGearNumPlanet=pDB_->GetLong(gc_cGear_Num_Planet);
	_iGearNumGalaxy=pDB_->GetLong(gc_cGear_Num_Galaxy);
	_iIsSync=pDB_->GetLong(gc_cIs_Sync);
	_iGearNumZ12=pDB_->GetLong(gc_cGear_Num_12);
	_iGearNumZ21=pDB_->GetLong(gc_cGear_Num_21);
	_iGearNumZ22=pDB_->GetLong(gc_cGear_Num_22);
	_iGearNumZ31=pDB_->GetLong(gc_cGear_Num_31);
	_iGearNumZ32=pDB_->GetLong(gc_cGear_Num_32);
	_iGearNumZ41=pDB_->GetLong(gc_cGear_Num_41);
	_iGearNumZ42=pDB_->GetLong(gc_cGear_Num_42);
	_iGearNumZ51=pDB_->GetLong(gc_cGear_Num_51);
	_iGearNumZ52=pDB_->GetLong(gc_cGear_Num_52);
	_iGearNumZ61=pDB_->GetLong(gc_cGear_Num_61);
	return true;
}

bool CGearBoxParamInfo::GetGearBoxParamInfo(IDBInterFace *pDB_)
{
	strncpy(_cCompany ,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory ,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cSetID ,pDB_->Get(gc_cSetID).c_str(),D_BHKD_SETID_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cGearBoxName,pDB_->Get(gc_cGearBox_Name).c_str(),sizeof(_cGearBoxName)-1);
	strncpy(_cComment,pDB_->Get(gc_cComment).c_str(),sizeof(_cComment)-1);
	_iGearBoxIDX=pDB_->GetLong(gc_cGearBox_IDX);
	_iGearBoxLevel=pDB_->GetLong(gc_cGearBox_Level);
	_iGearBoxType=pDB_->GetLong(gc_cGearBox_Type);
	_iDriverType=pDB_->GetLong(gc_cDriver_Type);
	_iPlanetNum=pDB_->GetLong(gc_cPlanet_Num);
	_iGearNumSun=pDB_->GetLong(gc_cGear_Num_Sun);
	_iGearNumPlanet=pDB_->GetLong(gc_cGear_Num_Planet);
	_iGearNumGalaxy=pDB_->GetLong(gc_cGear_Num_Galaxy);
	_iIsSync=pDB_->GetLong(gc_cIs_Sync);
	_iGearNumZ12=pDB_->GetLong(gc_cGear_Num_12);
	_iGearNumZ21=pDB_->GetLong(gc_cGear_Num_21);
	_iGearNumZ22=pDB_->GetLong(gc_cGear_Num_22);
	_iGearNumZ31=pDB_->GetLong(gc_cGear_Num_31);
	_iGearNumZ32=pDB_->GetLong(gc_cGear_Num_32);
	_iGearNumZ41=pDB_->GetLong(gc_cGear_Num_41);
	_iGearNumZ42=pDB_->GetLong(gc_cGear_Num_42);
	_iGearNumZ51=pDB_->GetLong(gc_cGear_Num_51);
	_iGearNumZ52=pDB_->GetLong(gc_cGear_Num_52);
	_iGearNumZ61=pDB_->GetLong(gc_cGear_Num_61);
	return true;
}
#endif//_BHKD_SERVER_SIDE_

/************************************************************************/
/*                                                                      */
/************************************************************************/

S_ChannInfo::S_ChannInfo()
{
	memset(_cChannID,0,D_BHKD_CHANNID_LEN);
	memset(_cChannNo,0,D_BHKD_CHANNNO_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cChannAlias,0,D_BHKD_CHANNALIAS_LEN);
	memset(_cAlarmTempID,0,20);
	memset(_cTempID ,0,20);
	memset(_cUnit ,0,20);	

	_iDataSource = 0;
	_charInfo.clear();
	_iLocationNo = -1;
	_iLocationIdx =-1;
	this->_iChannType = -1;
}

S_ChannInfo::S_ChannInfo(const S_ChannInfo& chInfo_)
{
	memset(_cChannID,0,D_BHKD_CHANNID_LEN);
	memset(_cChannNo,0,D_BHKD_CHANNNO_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cChannAlias,0,D_BHKD_CHANNALIAS_LEN);
	memset(_cAlarmTempID,0,20);
	memset(_cTempID ,0,20);
	memset(_cUnit ,0,20);	

	_iDataSource = 0;
	_charInfo.clear();
	_iLocationNo = -1;
	_iLocationIdx =-1;
	this->_iChannType = -1;

	_iID=chInfo_._iID;
	strncpy(_cCompany,chInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,chInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,chInfo_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cChannID,chInfo_._cChannID,D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,chInfo_._cChannNo,D_BHKD_CHANNNO_LEN-1);
	strncpy(_cChannAlias,chInfo_._cChannAlias,D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cUnit,chInfo_._cUnit,sizeof(_cUnit)-1);
	_iChannType=chInfo_._iChannType;
	_fSensitivity=chInfo_._fSensitivity;
	_fSensitivity1=chInfo_._fSensitivity1;
	_iList=chInfo_._iList;
	_iSensorType=chInfo_._iSensorType;
	_iRevType=chInfo_._iRevType;
	_bKeyIF=chInfo_._bKeyIF;
	_chAlarmParam=chInfo_._chAlarmParam;
	_fGapVoltage=chInfo_._fGapVoltage;
	_iCylinderNo=chInfo_._iCylinderNo;
	_iLocationIdx=chInfo_._iLocationIdx;
	_iMeassureDirection=chInfo_._iMeassureDirection;
	strncpy(_cTempID,chInfo_._cTempID,sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,chInfo_._cAlarmTempID,sizeof(_cAlarmTempID)-1);
	_iInfoType = chInfo_._iInfoType;
	if(chInfo_._charInfo.size() > 0)
		_charInfo = chInfo_._charInfo;
	else
		_charInfo.clear();
	_iDataSource = chInfo_._iDataSource;
	_iDataType   = chInfo_._iDataType;
	_fDCAmp      = chInfo_._fDCAmp;
	_fACAmp      = chInfo_._fACAmp;
	_iLocationIdx = chInfo_._iLocationIdx;
	_iLocationNo = chInfo_._iLocationNo;
}

S_ChannInfo::S_ChannInfo(const S_OldChannInfo & chInfo_)
{
	memset(_cChannID,0,D_BHKD_CHANNID_LEN);
	memset(_cChannNo,0,D_BHKD_CHANNNO_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cChannAlias,0,D_BHKD_CHANNALIAS_LEN);
	memset(_cAlarmTempID,0,20);
	memset(_cTempID ,0,20);
	memset(_cUnit ,0,20);	

	_iDataSource = 0;
	_charInfo.clear();
	_iLocationNo = -1;
	_iLocationIdx =-1;
	this->_iChannType = -1;

	_iID=chInfo_._iID;
	strncpy(_cCompany,chInfo_._cCompany,(D_BHKD_COMPANYNAME_LEN-1));
	strncpy(_cFactory,chInfo_._cFactory,(D_BHKD_FACTORYNAME_LEN-1));
	strncpy(_cPlantNo,chInfo_._cPlantNo,(D_BHKD_PLANTNO_LEN-1));
	strncpy(_cChannID,chInfo_._cChannID,(D_BHKD_CHANNID_LEN-1));
	strncpy(_cChannNo,chInfo_._cChannNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cChannAlias,chInfo_._cChannAlias,D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cUnit,chInfo_._cUnit,sizeof(_cUnit)-1);
	_iChannType=chInfo_._iChannType;
	_fSensitivity=chInfo_._fSensitivity;
	_fSensitivity1=chInfo_._fSensitivity1;
	_iList=chInfo_._iList;
	_iSensorType=chInfo_._iSensorType;
	_iRevType=chInfo_._iRevType;
	_bKeyIF=chInfo_._bKeyIF;
	_chAlarmParam=chInfo_._chAlarmParam;
	_fGapVoltage=chInfo_._fGapVoltage;
	_iCylinderNo=chInfo_._iCylinderNo;
	_iLocationIdx=chInfo_._iLocationIdx;
	_iMeassureDirection=chInfo_._iMeassureDirection;
	strncpy(_cTempID,chInfo_._cTempID,sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,chInfo_._cAlarmTempID,sizeof(_cAlarmTempID)-1);
	_iInfoType = chInfo_._iInfoType;
	_iDataSource = GE_DATA_SOURCE_ONLINE;
	_iDataType   = 0;
	_fDCAmp      = .0;
	_fACAmp      = .0;	
	_iLocationNo = -1;
	_iLocationIdx =-1;
	_charInfo.clear();
}

S_ChannInfo::S_ChannInfo(const S_ChannInfo * pChannInfo)
{
	memset(_cChannID,0,D_BHKD_CHANNID_LEN);
	memset(_cChannNo,0,D_BHKD_CHANNNO_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cChannAlias,0,D_BHKD_CHANNALIAS_LEN);
	memset(_cAlarmTempID,0,20);
	memset(_cTempID ,0,20);
	memset(_cUnit ,0,20);	

	_iDataSource = 0;
	_charInfo.clear();
	_iLocationNo = -1;
	_iLocationIdx =-1;
	this->_iChannType = -1;

	_iID=pChannInfo->_iID;
	strncpy(_cCompany,pChannInfo->_cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,pChannInfo->_cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,pChannInfo->_cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cChannID,pChannInfo->_cChannID,D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,pChannInfo->_cChannNo,D_BHKD_CHANNNO_LEN-1);
	strncpy(_cChannAlias,pChannInfo->_cChannAlias,D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cUnit,pChannInfo->_cUnit,sizeof(_cUnit)-1);
	_iChannType=pChannInfo->_iChannType;
	_fSensitivity=pChannInfo->_fSensitivity;
	_fSensitivity1 = pChannInfo->_fSensitivity1;
	_iList=pChannInfo->_iList;
	_iSensorType=pChannInfo->_iSensorType;
	_iRevType=pChannInfo->_iRevType;
	_bKeyIF=pChannInfo->_bKeyIF;
	_chAlarmParam=pChannInfo->_chAlarmParam;
	_fGapVoltage=pChannInfo->_fGapVoltage;
	_iCylinderNo=pChannInfo->_iCylinderNo;
	_iLocationIdx=pChannInfo->_iLocationIdx;
	_iMeassureDirection=pChannInfo->_iMeassureDirection;
	strncpy(_cTempID,pChannInfo->_cTempID,sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,pChannInfo->_cAlarmTempID,sizeof(_cAlarmTempID)-1);
	_iInfoType = pChannInfo->_iInfoType;
	_iDataSource = pChannInfo->_iDataSource;
	_iDataType   = pChannInfo->_iDataType;
	_fDCAmp      = pChannInfo->_fDCAmp;
	_fACAmp      = pChannInfo->_fACAmp;
	_iLocationIdx = pChannInfo->_iLocationIdx;
	_iLocationNo = pChannInfo->_iLocationNo;
	_charInfo.clear();
}

S_ChannInfo& S_ChannInfo::operator=(const S_ChannInfo& chInfo_)
{
	if(this == &chInfo_)
		return *this;

	_iID=chInfo_._iID;
	strncpy(_cCompany,chInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,chInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,chInfo_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cChannID,chInfo_._cChannID,D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,chInfo_._cChannNo,D_BHKD_CHANNNO_LEN-1);
	strncpy(_cChannAlias,chInfo_._cChannAlias,D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cUnit,chInfo_._cUnit,sizeof(_cUnit)-1);
	_iChannType=chInfo_._iChannType;
	_fSensitivity=chInfo_._fSensitivity;
	_fSensitivity1 = chInfo_._fSensitivity1;
	_iList=chInfo_._iList;
	_iSensorType=chInfo_._iSensorType;
	_iRevType=chInfo_._iRevType;
	_bKeyIF=chInfo_._bKeyIF;
	_chAlarmParam=chInfo_._chAlarmParam;
	_fGapVoltage=chInfo_._fGapVoltage;
	_iCylinderNo=chInfo_._iCylinderNo;
	_iLocationIdx=chInfo_._iLocationIdx;
	_iMeassureDirection=chInfo_._iMeassureDirection;
	strncpy(_cTempID,chInfo_._cTempID,sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,chInfo_._cAlarmTempID,sizeof(_cAlarmTempID)-1);
	_iInfoType = chInfo_._iInfoType;
	if(chInfo_._charInfo.size() > 0)
		_charInfo = chInfo_._charInfo;
	else
		_charInfo.clear();
	_iDataSource = chInfo_._iDataSource;
	_iDataType   = chInfo_._iDataType;
	_fDCAmp      = chInfo_._fDCAmp;
	_fACAmp      = chInfo_._fACAmp;
	_iLocationIdx = chInfo_._iLocationIdx;
	_iLocationNo = chInfo_._iLocationNo;
	return *this;
}

#ifdef _BHKD_SERVER_SIDE_
S_OldChannInfo::S_OldChannInfo()
{
	memset(_cChannID,0,D_BHKD_CHANNID_LEN);
	memset(_cChannNo,0,D_BHKD_CHANNNO_LEN);
	memset(_cFactory,0,D_BHKD_FACTORYNAME_LEN);
	memset(_cPlantNo,0,D_BHKD_PLANTNO_LEN);
	memset(_cCompany,0,D_BHKD_COMPANYNAME_LEN);
	memset(_cChannAlias,0,D_BHKD_CHANNALIAS_LEN);
	memset(_cAlarmTempID,0,20);
	memset(_cTempID,0,20);
	memset(_cUnit,0,20);
}


S_OldChannInfo& S_OldChannInfo::operator=(const S_OldChannInfo& chInfo_)
{
	_iID=chInfo_._iID;
	strncpy(_cCompany,chInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,chInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,chInfo_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cChannID,chInfo_._cChannID,D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,chInfo_._cChannNo,D_BHKD_CHANNNO_LEN-1);
	strncpy(_cChannAlias,chInfo_._cChannAlias,D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cUnit,chInfo_._cUnit,sizeof(_cUnit)-1);
	_iChannType=chInfo_._iChannType;
	_fSensitivity=chInfo_._fSensitivity;
	_iList=chInfo_._iList;
	_iSensorType=chInfo_._iSensorType;
	_iRevType=chInfo_._iRevType;
	_bKeyIF=chInfo_._bKeyIF;
	_chAlarmParam=chInfo_._chAlarmParam;
	_fGapVoltage=chInfo_._fGapVoltage;
	_iCylinderNo=chInfo_._iCylinderNo;
	_iLocationIdx=chInfo_._iLocationIdx;
	_iMeassureDirection=chInfo_._iMeassureDirection;
	strncpy(_cTempID,chInfo_._cTempID,sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,chInfo_._cAlarmTempID,sizeof(_cAlarmTempID)-1);
	return *this;
}

//从指定的数据连接中得到设备详细信息
bool S_OldChannInfo::GetChannInfo(CCustomDB *pDB_)
{
	return GetChannInfo((CSimADO*)pDB_);
}
//从指定的数据连接中得到设备详细信息
bool S_OldChannInfo::GetChannInfo(CSimADO *pDB_)
{
	strncpy(_cChannAlias,pDB_->Get(gc_cChannIDA),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cChannID,pDB_->Get(gc_cChannID),D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,pDB_->Get(gc_cChannNo),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cCompany,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cUnit,pDB_->Get(gc_cUnit),sizeof(_cUnit)-1);
	_iChannType=pDB_->GetLong(gc_cChannType);
	_iID=pDB_->GetLong(gc_cID);
	_iList=pDB_->GetLong(gc_cList);
	_iRevType=pDB_->GetLong(gc_cRevType);
	if ((_iRevType<1)||(_iRevType>5))
	    _iRevType=1;
	_fSensitivity=pDB_->GetDouble(gc_cSensitivity);
	_iSensorType=pDB_->GetLong(gc_cSensorType);
	_bKeyIF=pDB_->GetLong(gc_cKeyIF)?1:0;
	_fGapVoltage=pDB_->GetDouble(gc_cGapVoltage);
	_fSensitivity1=pDB_->GetDouble(gc_cSensitivity1);
	_iInfoType=pDB_->GetLong(gc_cInfoType);
	_iCylinderNo=pDB_->GetLong(gc_cCylinderNo);
	strncpy(_cTempID,pDB_->Get(gc_cTempletID),sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,pDB_->Get(gc_cAlarm_TempletID),sizeof(_cAlarmTempID)-1);
	_iLocationIdx=pDB_->GetLong(gc_cLocation_IDX);
	_iMeassureDirection=pDB_->GetLong(gc_cMesurement_Direction);

	long iSize=sizeof(CChannAlarmParam);
	return SimADO_GetRecipComprParam(pDB_, (BYTE*)&(_chAlarmParam), iSize);
}

//从指定的数据连接中得到设备详细信息
bool S_OldChannInfo::GetChannInfo(IDBInterFace *pDB_)
{
	strncpy(_cChannAlias,pDB_->Get(gc_cChannIDA).c_str(),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cChannID,pDB_->Get(gc_cChannID).c_str(),D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,pDB_->Get(gc_cChannNo).c_str(),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cCompany,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cUnit,pDB_->Get(gc_cUnit).c_str(),sizeof(_cUnit)-1);
	_iChannType=pDB_->GetLong(gc_cChannType);
	_iID=pDB_->GetLong(gc_cID);
	_iList=pDB_->GetLong(gc_cList);
	_iRevType=pDB_->GetLong(gc_cRevType);
	if ((_iRevType<1)||(_iRevType>5))
		_iRevType=1;
	_fSensitivity=pDB_->GetDouble(gc_cSensitivity);
	_iSensorType=pDB_->GetLong(gc_cSensorType);
	_bKeyIF=pDB_->GetLong(gc_cKeyIF)?1:0;
	_fGapVoltage=pDB_->GetDouble(gc_cGapVoltage);
	_fSensitivity1=pDB_->GetDouble(gc_cSensitivity1);
	_iInfoType=pDB_->GetLong(gc_cInfoType);
	_iCylinderNo=pDB_->GetLong(gc_cCylinderNo);
	strncpy(_cTempID,pDB_->Get(gc_cTempletID).c_str(),sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,pDB_->Get(gc_cAlarm_TempletID).c_str(),sizeof(_cAlarmTempID)-1);
	_iLocationIdx=pDB_->GetLong(gc_cLocation_IDX);
	_iMeassureDirection=pDB_->GetLong(gc_cMesurement_Direction);

	long iSize=sizeof(CChannAlarmParam);
	return SimADO_GetRecipComprParamEx(pDB_, (BYTE*)&(_chAlarmParam), iSize);
}
//更新通道信息到指定连接
bool S_OldChannInfo::SetChannInfo(CSimADO *pDB_)
{
	pDB_->SetFieldValue(gc_cChannIDA,_cChannAlias);
	pDB_->SetFieldValue(gc_cUnit,_cUnit);
	pDB_->SetFieldValue(gc_cChannType,_iChannType);
	pDB_->SetFieldValue(gc_cList,_iList);
	pDB_->SetFieldValue(gc_cRevType,_iRevType);
	pDB_->SetFieldValue(gc_cSensorType,_iSensorType);
	pDB_->SetFieldValue(gc_cSensitivity,_fSensitivity);
	pDB_->SetFieldValue(gc_cKeyIF,(_bKeyIF?1:0));
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cSensitivity1,_fSensitivity1);
	pDB_->SetFieldValue(gc_cCylinderNo,_iCylinderNo);
	pDB_->SetFieldValue(gc_cTempletID,_cTempID);
	pDB_->SetFieldValue(gc_cAlarm_TempletID,_cAlarmTempID);
	pDB_->SetFieldValue(gc_cLocation_IDX,_iLocationIdx);
	pDB_->SetFieldValue(gc_cMesurement_Direction,_iMeassureDirection);
	return SimADO_SetChannAlarmParam(pDB_,
	                                (BYTE*)&_chAlarmParam,
	                                sizeof(CChannAlarmParam));
}

bool S_OldChannInfo::SetChannInfo(IDBInterFace *pDB_)
{
	pDB_->SetFieldValue(gc_cChannIDA,_cChannAlias);
	pDB_->SetFieldValue(gc_cUnit,_cUnit);
	pDB_->SetFieldValue(gc_cChannType,_iChannType);
	pDB_->SetFieldValue(gc_cList,_iList);
	pDB_->SetFieldValue(gc_cRevType,_iRevType);
	pDB_->SetFieldValue(gc_cSensorType,_iSensorType);
	pDB_->SetFieldValue(gc_cSensitivity,_fSensitivity);
	pDB_->SetFieldValue(gc_cKeyIF,(_bKeyIF?1:0));
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cSensitivity1,_fSensitivity1);
	pDB_->SetFieldValue(gc_cCylinderNo,_iCylinderNo);
	pDB_->SetFieldValue(gc_cTempletID,_cTempID);
	pDB_->SetFieldValue(gc_cAlarm_TempletID,_cAlarmTempID);
	pDB_->SetFieldValue(gc_cLocation_IDX,_iLocationIdx);
	pDB_->SetFieldValue(gc_cMesurement_Direction,_iMeassureDirection);
	return ::SimADO_SetChannAlarmParamEx(pDB_,
		(BYTE*)&_chAlarmParam,
		sizeof(CChannAlarmParam));
}
#endif

#ifdef _BHKD_SERVER_SIDE_
//从指定的数据连接中得到设备详细信息
bool S_ChannInfo::GetChannInfo(CCustomDB *pDB_)
{
	return GetChannInfo((CSimADO*)pDB_);
}
//从指定的数据连接中得到设备详细信息
bool S_ChannInfo::GetChannInfo(CSimADO *pDB_)
{
	strncpy(_cChannAlias,pDB_->Get(gc_cChannIDA),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cChannID,pDB_->Get(gc_cChannID),D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,pDB_->Get(gc_cChannNo),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cCompany,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cUnit,pDB_->Get(gc_cUnit),sizeof(_cUnit)-1);
	_iChannType=pDB_->GetLong(gc_cChannType);
	_iID=pDB_->GetLong(gc_cID);
	_iList=pDB_->GetLong(gc_cList);
	_iRevType=pDB_->GetLong(gc_cRevType);
	if ((_iRevType<1)||(_iRevType>5))
	    _iRevType=1;
	_fSensitivity=pDB_->GetDouble(gc_cSensitivity);
	_iSensorType=pDB_->GetLong(gc_cSensorType);
	_bKeyIF=pDB_->GetLong(gc_cKeyIF)?1:0;
	_fGapVoltage=pDB_->GetDouble(gc_cGapVoltage);
	_fSensitivity1=pDB_->GetDouble(gc_cSensitivity1);
	_iInfoType=pDB_->GetLong(gc_cInfoType);
	_iCylinderNo=pDB_->GetLong(gc_cCylinderNo);
	strncpy(_cTempID,pDB_->Get(gc_cTempletID),sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,pDB_->Get(gc_cAlarm_TempletID),sizeof(_cAlarmTempID)-1);
	_iLocationIdx=pDB_->GetLong(gc_cLocation_IDX);
	_iMeassureDirection=pDB_->GetLong(gc_cMesurement_Direction);
	_iDataSource = pDB_->GetLong(gc_cData_Source);
	_iDataType   = pDB_->GetLong(gc_cChann_Data_Type);
	_fDCAmp      = pDB_->GetDouble(gc_cDC_AMP);
	_fACAmp      = pDB_->GetDouble(gc_cAC_AMP);
	_iLocationNo = pDB_->GetLong(gc_cLocation_NO);
	long iSize=sizeof(CChannAlarmParam);
	return SimADO_GetChannAlarmParam(pDB_, (BYTE*)&(_chAlarmParam), iSize);
}


bool S_ChannInfo::GetCharInfo(CSimADO *pDB_)
{
	/*
	SQL:
		select machineinfotype.[INDEX], machineinfotype.COMMENT, machineinfotype.UNIT
		from machineinfotype inner join channel
		on
		machineinfotype.infotype=channel.infotype
		...
		and channel.PLANTNO='1'
		and channel.CHANNNO='3RX'
		order by machineinfotype.[INDEX];
	*/
	pDB_->Cmd("SELECT %s.[%s], %s.%s, %s.%s",
		gc_cMachInfoTypeTable, gc_cIndex,
		gc_cMachInfoTypeTable, gc_cComment,
		gc_cMachInfoTypeTable, gc_cUnit);
	pDB_->Cmd(" FROM %s inner join %s ON",
		gc_cMachInfoTypeTable,
		gc_cChannTable);
	pDB_->Cmd(" %s.%s=%s.%s",
		gc_cMachInfoTypeTable, gc_cInfoType,
		gc_cChannTable, gc_cInfoType);
	pDB_->Cmd(" AND %s.%s='%s'",
		gc_cChannTable, gc_cCompany,
		_cCompany);
	pDB_->Cmd(" AND %s.%s='%s'",
		gc_cChannTable, gc_cPlantNo,
		_cPlantNo);
	pDB_->Cmd(" AND %s.%s='%s'",
		gc_cChannTable, gc_cChannNo,
		_cChannNo);
	pDB_->Cmd(" ORDER BY %s.[%s];",
		gc_cMachInfoTypeTable, gc_cIndex);

	int    nCharIndex;
	string sCharName;
	string sCharUnit;

	while(pDB_->More())
	{
		nCharIndex = pDB_->GetLong(gc_cIndex);
		sCharName  = pDB_->Get(gc_cComment);
		sCharUnit  = pDB_->Get(gc_cUnit);
		_charInfo.insert(
			make_pair(nCharIndex,
			          S_CharInfo(nCharIndex, sCharName, sCharUnit))
			          );
	}

	return _charInfo.size();
}


//从指定的数据连接中得到设备详细信息
bool S_ChannInfo::GetChannInfo(IDBInterFace *pDB_)
{
	strncpy(_cChannAlias,pDB_->Get(gc_cChannIDA).c_str(),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cChannID,pDB_->Get(gc_cChannID).c_str(),D_BHKD_CHANNID_LEN-1);
	strncpy(_cChannNo,pDB_->Get(gc_cChannNo).c_str(),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cCompany,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cUnit,pDB_->Get(gc_cUnit).c_str(),sizeof(_cUnit)-1);
	_iChannType=pDB_->GetLong(gc_cChannType);
	_iID=pDB_->GetLong(gc_cID);
	_iList=pDB_->GetLong(gc_cList);
	_iRevType=pDB_->GetLong(gc_cRevType);
	if ((_iRevType<1)||(_iRevType>5))
		_iRevType=1;
	_fSensitivity=pDB_->GetDouble(gc_cSensitivity);
	_iSensorType=pDB_->GetLong(gc_cSensorType);
	_bKeyIF=pDB_->GetLong(gc_cKeyIF)?1:0;
	_fGapVoltage=pDB_->GetDouble(gc_cGapVoltage);
	_fSensitivity1=pDB_->GetDouble(gc_cSensitivity1);
	_iInfoType=pDB_->GetLong(gc_cInfoType);
	_iCylinderNo=pDB_->GetLong(gc_cCylinderNo);
	strncpy(_cTempID,pDB_->Get(gc_cTempletID).c_str(),sizeof(_cTempID)-1);
	strncpy(_cAlarmTempID,pDB_->Get(gc_cAlarm_TempletID).c_str(),sizeof(_cAlarmTempID)-1);
	_iLocationIdx=pDB_->GetLong(gc_cLocation_IDX);
	_iMeassureDirection=pDB_->GetLong(gc_cMesurement_Direction);
	_iDataSource = pDB_->GetLong(gc_cData_Source);
	_iDataType   = pDB_->GetLong(gc_cChann_Data_Type);
	_fDCAmp      = pDB_->GetDouble(gc_cDC_AMP);
	_fACAmp      = pDB_->GetDouble(gc_cAC_AMP);
	_iLocationNo = pDB_->GetLong(gc_cLocation_NO);
	long iSize=sizeof(CChannAlarmParam);
	return SimADO_GetChannAlarmParamEx(pDB_, (BYTE*)&(_chAlarmParam), iSize);
}

//更新通道信息到指定连接
bool S_ChannInfo::SetChannInfo(CSimADO *pDB_)
{
	pDB_->SetFieldValue(gc_cChannIDA,_cChannAlias);
	pDB_->SetFieldValue(gc_cUnit,_cUnit);
	pDB_->SetFieldValue(gc_cChannType,_iChannType);
	pDB_->SetFieldValue(gc_cList,_iList);
	pDB_->SetFieldValue(gc_cRevType,_iRevType);
	pDB_->SetFieldValue(gc_cSensorType,_iSensorType);
	pDB_->SetFieldValue(gc_cSensitivity,_fSensitivity);
	pDB_->SetFieldValue(gc_cKeyIF,(_bKeyIF?1:0));
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cSensitivity1,_fSensitivity1);
	pDB_->SetFieldValue(gc_cCylinderNo,_iCylinderNo);
	pDB_->SetFieldValue(gc_cTempletID,_cTempID);
	pDB_->SetFieldValue(gc_cAlarm_TempletID,_cAlarmTempID);
	pDB_->SetFieldValue(gc_cLocation_IDX,_iLocationIdx);
	pDB_->SetFieldValue(gc_cMesurement_Direction,_iMeassureDirection);
	pDB_->SetFieldValue(gc_cData_Source, _iDataSource);
	pDB_->SetFieldValue(gc_cInfoType, _iInfoType);
	pDB_->SetFieldValue(gc_cChann_Data_Type, _iDataType);
	pDB_->SetFieldValue(gc_cDC_AMP, _fDCAmp);
	pDB_->SetFieldValue(gc_cAC_AMP, _fACAmp);
	
	return SimADO_SetChannAlarmParam(pDB_,
	                                 (BYTE*)&_chAlarmParam,
	                                 sizeof(CChannAlarmParam));
}

//更新通道信息到指定连接
bool S_ChannInfo::SetChannInfo(IDBInterFace *pDB_)
{
	pDB_->SetFieldValue(gc_cChannIDA,_cChannAlias);
	pDB_->SetFieldValue(gc_cUnit,_cUnit);
	pDB_->SetFieldValue(gc_cChannType,_iChannType);
	pDB_->SetFieldValue(gc_cList,_iList);
	pDB_->SetFieldValue(gc_cRevType,_iRevType);
	pDB_->SetFieldValue(gc_cSensorType,_iSensorType);
	pDB_->SetFieldValue(gc_cSensitivity,_fSensitivity);
	pDB_->SetFieldValue(gc_cKeyIF,(_bKeyIF?1:0));
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cGapVoltage,_fGapVoltage);
	pDB_->SetFieldValue(gc_cSensitivity1,_fSensitivity1);
	pDB_->SetFieldValue(gc_cCylinderNo,_iCylinderNo);
	pDB_->SetFieldValue(gc_cTempletID,_cTempID);
	pDB_->SetFieldValue(gc_cAlarm_TempletID,_cAlarmTempID);
	pDB_->SetFieldValue(gc_cLocation_IDX,_iLocationIdx);
	pDB_->SetFieldValue(gc_cMesurement_Direction,_iMeassureDirection);
	pDB_->SetFieldValue(gc_cData_Source, _iDataSource);
	pDB_->SetFieldValue(gc_cInfoType, _iInfoType);
	pDB_->SetFieldValue(gc_cChann_Data_Type, _iDataType);
	pDB_->SetFieldValue(gc_cDC_AMP, _fDCAmp);
	pDB_->SetFieldValue(gc_cAC_AMP, _fACAmp);

	return ::SimADO_SetChannAlarmParamEx(pDB_,
		(BYTE*)&_chAlarmParam,
		sizeof(CChannAlarmParam));
}
#endif//_BHKD_SERVER_SIDE_

S_CharInfo::S_CharInfo(int charIndex_, const string& charName_, const string& charUnit_) :
	_iCharIndex(charIndex_),
	_sCharName(charName_),
	_sCharUnit(charUnit_)
{
}

S_CharInfo::S_CharInfo(const S_CharInfo & charInfo_)
{
	_iCharIndex = charInfo_._iCharIndex;
	_sCharName  = charInfo_._sCharName;
	_sCharUnit  = charInfo_._sCharUnit;
}

S_CharInfo & S_CharInfo::operator=(const S_CharInfo & charInfo_)
{
	if(this == &charInfo_)
		return *this;

	_iCharIndex = charInfo_._iCharIndex;
	_sCharName  = charInfo_._sCharName;
	_sCharUnit  = charInfo_._sCharUnit;

	return *this;
}

int    S_CharInfo::GetCharIndex() const
{
	return _iCharIndex;
}

string S_CharInfo::GetCharName() const
{
	return _sCharName;
}

string S_CharInfo::GetCharUnit() const
{
	return _sCharUnit;
}

CAxisChannInfo::CAxisChannInfo()
{

}

CAxisChannInfo::~CAxisChannInfo()
{
}

#ifdef _BHKD_SERVER_SIDE_
//从指定的数据连接中得到轴心通道详细信息
bool CAxisChannInfo::GetAxisChannInfo(CCustomDB *pDB_)
{
	return GetAxisChannInfo((CSimADO*)pDB_);
}


//从指定的数据连接中得到轴心通道详细信息
bool CAxisChannInfo::GetAxisChannInfo(CSimADO *pDB_)
{
	_iID=pDB_->GetLong(gc_cID);
	strncpy(_cCompany,pDB_->Get(gc_cCompany),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cLocation,pDB_->Get(gc_cLocation),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cVerChann,pDB_->Get(gc_cVerChann),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cHorChann,pDB_->Get(gc_cHorChann),D_BHKD_CHANNNO_LEN-1);
	_iLocationType=pDB_->GetLong(gc_cLocationType);
	_iRotateDirection=pDB_->GetLong(gc_cRotate_Direction);
	_fAxisDia=pDB_->GetDouble(gc_cAxis_Dia);
	long iSize=sizeof(CAxisAlarmParam);
	return SimADO_GetChannAlarmParam(pDB_, (BYTE *)&(_axisAlarmParam), iSize);
//	return true;
}

//从指定的数据连接中得到轴心通道详细信息
bool CAxisChannInfo::GetAxisChannInfo(IDBInterFace *pDB_)
{
	_iID=pDB_->GetLong(gc_cID);
	strncpy(_cCompany,pDB_->Get(gc_cCompany).c_str(),D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name).c_str(),D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(),D_BHKD_PLANTNO_LEN-1);
	strncpy(_cLocation,pDB_->Get(gc_cLocation).c_str(),D_BHKD_CHANNALIAS_LEN-1);
	strncpy(_cVerChann,pDB_->Get(gc_cVerChann).c_str(),D_BHKD_CHANNNO_LEN-1);
	strncpy(_cHorChann,pDB_->Get(gc_cHorChann).c_str(),D_BHKD_CHANNNO_LEN-1);
	_iLocationType=pDB_->GetLong(gc_cLocationType);
	_iRotateDirection=pDB_->GetLong(gc_cRotate_Direction);
	_fAxisDia=pDB_->GetDouble(gc_cAxis_Dia);
	long iSize=sizeof(CAxisAlarmParam);
	return SimADO_GetChannAlarmParamEx(pDB_, (BYTE *)&(_axisAlarmParam), iSize);
	//	return true;
}

bool CAxisChannInfo::SetAxisChannInfo(CSimADO *pDB_)
{
	pDB_->SetFieldValue(gc_cLocation,_cLocation);
	pDB_->SetFieldValue(gc_cAxis_Dia,_fAxisDia);
	pDB_->SetFieldValue(gc_cLocationType,_iLocationType);
	pDB_->SetFieldValue(gc_cRotate_Direction,_iRotateDirection);
	long iSize=sizeof(CAxisAlarmParam);
	return SimADO_SetChannAlarmParam(pDB_, (BYTE*)&(_axisAlarmParam),iSize);
}

bool CAxisChannInfo::SetAxisChannInfo(IDBInterFace *pDB_)
{
	pDB_->SetFieldValue(gc_cLocation,_cLocation);
	pDB_->SetFieldValue(gc_cAxis_Dia,_fAxisDia);
	pDB_->SetFieldValue(gc_cLocationType,_iLocationType);
	pDB_->SetFieldValue(gc_cRotate_Direction,_iRotateDirection);
	long iSize=sizeof(CAxisAlarmParam);
	return ::SimADO_SetChannAlarmParamEx(pDB_, (BYTE*)&(_axisAlarmParam),iSize);
}
#endif//_BHKD_SERVER_SIDE_

CAxisChannInfo& CAxisChannInfo::operator=(const CAxisChannInfo& aChannInfo_)
{
	_iID=aChannInfo_._iID;
	strncpy(_cCompany,aChannInfo_._cCompany,D_BHKD_COMPANYNAME_LEN-1);
	strncpy(_cFactory,aChannInfo_._cFactory,D_BHKD_FACTORYNAME_LEN-1);
	strncpy(_cPlantNo,aChannInfo_._cPlantNo,D_BHKD_PLANTNO_LEN-1);
	strncpy(_cVerChann,aChannInfo_._cVerChann,D_BHKD_CHANNNO_LEN-1);
	strncpy(_cHorChann,aChannInfo_._cHorChann,D_BHKD_CHANNNO_LEN-1);
	_iLocationType=aChannInfo_._iLocationType;
	_iRotateDirection=aChannInfo_._iRotateDirection;
	_fAxisDia=aChannInfo_._fAxisDia;
	strncpy(_cLocation,aChannInfo_._cLocation,D_BHKD_CHANNALIAS_LEN-1);
	_axisAlarmParam=aChannInfo_._axisAlarmParam;
	return *this;
}

//--------------------------------------------------------//
S_AxisData::S_AxisData()
{
	_bCalc=false;
	_fCoorX=.0;
	_fCoorY=.0;
	_fHorDC=.0;
	_fHorGap=.0;
	_fRev=.0;
	_fVerDC=.0;
	_fVerGap=.0;
}

S_AxisData& S_AxisData::operator =(const S_AxisData& axisData_)
{
	_bCalc=axisData_._bCalc;
	_fCoorX=axisData_._fCoorX;
	_fCoorY=axisData_._fCoorY;
	_fHorDC=axisData_._fHorDC;
	_fHorGap=axisData_._fHorGap;
	_fVerDC=axisData_._fVerDC;
	_fVerGap=axisData_._fVerGap;
	_fRev=axisData_._fRev;
	return *this;
}
#ifdef _BHKD_SERVER_SIDE_
bool S_AxisData::CalcAxisLocation(int iLocationType_,int iRotateDirection_,float fAxisDia_,float fSensitivityVer_,float fSensitivitypHor_)
{
	S_AxisLocationInputInfo calcInput;
	fSensitivitypHor_ < 0.01?8:fSensitivitypHor_;
	fSensitivityVer_  < 0.01?8:fSensitivityVer_;
	double ldblVerDC = _fVerDC*((fSensitivitypHor_/fSensitivityVer_));
	calcInput.fDC_H= _fHorDC;
	calcInput.fDC_V= ldblVerDC;
	calcInput.fGapVolt_H=_fHorGap*(1/fSensitivitypHor_);
	calcInput.fGapVolt_V=_fVerGap*(1/fSensitivityVer_);
	//calcInput.i_circumgyrate_aspect=iRotateDirection_;
	calcInput.fRadius=fAxisDia_/2;
	calcInput.fSensitivity = fSensitivitypHor_;
	calcInput.sensorMounting=(E_AxisSensorMounting)iLocationType_;
	//CString strInfo;strInfo.Format("_fHorDC:%.2f,_fVerDC:%.2f,_fHorGap:%.2f,_fVerGap:%.2f,d_DC_H:%f,d_DC_V:%f,GapVolt_H:%.2f,GapVolt_V:%.2f,aspect:%d,Radius:%.2f,mode:%d",_fHorDC,_fVerDC,_fHorGap,_fVerGap,calcInput.fDC_H,calcInput.d_DC_V,calcInput.fGapVolt_H,calcInput.fGapVolt_V,calcInput.i_circumgyrate_aspect,calcInput.fRadius,calcInput.sensorMounting);BHTRACE_DEBUG(strInfo);
	//CHZLogManage::Debug( strInfo, "S_AxisData","CalcAxisLocation");


	double fXValue = 0;
	double fYValue = 0;

	CSigMath sigMath;
	
	int lnRet = -1;
	try
	{
		lnRet = sigMath.CalcAxisCenterLocation(calcInput,fXValue, fYValue);
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!: sigMath.CalcAxisCenterLocation %s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	if (lnRet >= 0)
	{
		_bCalc=true;
		_fCoorX = fXValue * 10000.0;
		_fCoorY = fYValue * 10000.0;
	}
	else
	{
		_bCalc=false;
#ifdef _DEBUG
#endif//_DEBUG
	}
	return _bCalc;
}
#endif//_BHKD_SERVER_SIDE_

//---------------------------------------------------------//
S_PlantInfo::S_PlantInfo()
{
	memset(_cCompany, 0, D_BHKD_COMPANYNAME_LEN);
	memset(_cFactory, 0, D_BHKD_FACTORYNAME_LEN);
	memset(_cManufacturer, 0, 100);
	memset(_cSetID,   0, D_BHKD_SETID_LEN);
	memset(_cPlantID, 0, D_BHKD_PLANTID_LEN);
	memset(_cPlantNo, 0, D_BHKD_PLANTNO_LEN);
	_iVibChannNum=0;         //振动测点数目
	_iSmpFreq=1024;             //采样频率
	_iSmpNum=2048;              //采样点数
	_iDynChannNum=0;           //动态通道数目
	 _iProcChannNum=0;        //过程量测点数目
	 _piRev[0]=3000;                //额定转速1
	 _piRev[1]=3000;                //额定转速2
	 _piRev[2]=3000;
	 _piRev[3]=3000;
     _piRev[4]=3000;
	 _iGraphType=-1;           //概貌图类型
	 _iMachineType=1;         //设备类型
	 _iDriverType=1;          //驱动机类型
	 _iConnType=1;            //联结类型（联轴节类型）
	 _iBearingType=1;         //轴承类型
	 _iRotorType=1;           //转子类型
	 _iSealType=1;            //密封类型
	 _iWorkRevType=1;         //工作转速类型,是处于一阶临界转速附近
	 _iCritSpeed1st=500;        //一阶临界转速
	 _iCritSpeed2nd=1000;        //二阶临界转速
	 _firstRunT=CBHDateTime(2006,1,1,1,1,1);   //首次投运时间
	 _iGraphBuffBytes=0;         //自制概貌图的字节数
	 _pGraphBuff=NULL;         //自制概貌图的BUFF指针
	 memset(_sVersion, 0, D_BHKD_VERSION_LEN);
	 strncpy(_sVersion, "1.0.0.0", D_BHKD_VERSION_LEN - 1);             //版本信息
     _iCylinderNum = 0;         //气缸数目
     _iDataSource = 0;
	 m_nDbId = 0;
}

S_PlantInfo::~S_PlantInfo()
{
	if(NULL!=_pGraphBuff)
		delete _pGraphBuff;
}

S_PlantInfoEx::S_PlantInfoEx()
{
	_bGroupVisible   = TRUE;
	_bFactoryVisible = TRUE;
	_bCompanyVisible = TRUE;
	_bSetVisible     = FALSE;
	_bPlantVisible   = TRUE;
	m_strGroup = "";
	m_strContry = COUNTRY_DEFAULT;
	m_strPlantDisplayName = "";
	m_strSetDisplayName = "";
	m_strErpCode = "";
	m_DeviceId = -1;
	m_strCompanyAlias = "";
}

S_PlantInfo& S_PlantInfo::operator =(const S_PlantInfo& sPlantInfo_)
{
	strncpy(_cCompany,sPlantInfo_._cCompany, D_BHKD_COMPANYNAME_LEN - 1);
	strncpy(_cSetID,sPlantInfo_._cSetID, D_BHKD_SETID_LEN - 1);
	strncpy(_cPlantID,sPlantInfo_._cPlantID, D_BHKD_PLANTID_LEN - 1);
	strncpy(_cPlantNo,sPlantInfo_._cPlantNo, D_BHKD_PLANTNO_LEN - 1);
	strncpy(_cFactory,sPlantInfo_._cFactory, D_BHKD_FACTORYNAME_LEN - 1);
	strncpy(_cManufacturer,sPlantInfo_._cManufacturer, 100 - 1);
	_iVibChannNum=sPlantInfo_._iVibChannNum;
	_iDynChannNum=sPlantInfo_._iDynChannNum;
	_iSmpFreq=sPlantInfo_._iSmpFreq;
	_iSmpNum=sPlantInfo_._iSmpNum;
	_iProcChannNum=sPlantInfo_._iProcChannNum;
	memcpy(_piRev,sPlantInfo_._piRev,sizeof(int)*5);
	_iGraphType=sPlantInfo_._iGraphType;
	_iMachineType=sPlantInfo_._iMachineType;
	_iDriverType=sPlantInfo_._iDriverType;
	_iConnType=sPlantInfo_._iConnType;
	_iBearingType=sPlantInfo_._iBearingType;
	_iRotorType=sPlantInfo_._iRotorType;
	_iSealType=sPlantInfo_._iSealType;
	_iWorkRevType=sPlantInfo_._iWorkRevType;
	_iCritSpeed1st=sPlantInfo_._iCritSpeed1st;
	_iCritSpeed2nd=sPlantInfo_._iCritSpeed2nd;
	_firstRunT=sPlantInfo_._firstRunT;
	_iCylinderNum=sPlantInfo_._iCylinderNum;
	_sRecipComprParam=sPlantInfo_._sRecipComprParam;
	_iDataSource = sPlantInfo_._iDataSource;
	strncpy(_sVersion, sPlantInfo_._sVersion, D_BHKD_VERSION_LEN - 1);
	return *this;
}

S_PlantInfoParam_Deprecated::S_PlantInfoParam_Deprecated()
{
	_fCrankShaftDia=0.1;
	_fConnRodLength=0.1;///连杆长度
	_fPistonDia=0.1;    ///活塞直径
	_fTotalPower=0.1;   ///总功率
	_fNomPi=0.1;        ///名义进气压力
	_fNomPo=0.1;        ///名义出气压力
	_fClv=0.1;          ///余隙（clearance）容积
	_fRes1=0.1;
	_fRes2=0.1;
	_iRes1=-1;
}

S_PlantInfoParam_Deprecated & S_PlantInfoParam_Deprecated::operator =(const S_PlantInfoParam_Deprecated& sPlantInfoParam_)
{
	_fCrankShaftDia=sPlantInfoParam_._fCrankShaftDia;
	_fConnRodLength=sPlantInfoParam_._fConnRodLength;///连杆长度
	_fPistonDia=sPlantInfoParam_._fPistonDia;    ///活塞直径
	_fTotalPower=sPlantInfoParam_._fTotalPower;   ///总功率
	_fNomPi=sPlantInfoParam_._fNomPi;        ///名义进气压力
	_fNomPo=sPlantInfoParam_._fNomPo;        ///名义出气压力
	_fClv=sPlantInfoParam_._fClv;          ///余隙（clearance）容积
	_fRes1=sPlantInfoParam_._fRes1;
	_fRes2=sPlantInfoParam_._fRes2;
	_iRes1=sPlantInfoParam_._iRes1;
	return *this;
}

S_PlantInfoParam::S_PlantInfoParam()
{
	_fCrankShaftDia=0.1;
	_fConnRodLength=0.1;///连杆长度
	_fPistonDia=0.1;    ///活塞直径
	_fTotalPower=0.1;   ///总功率
	_fNomPi=0.1;        ///名义进气压力
	_fNomPo=0.1;        ///名义出气压力
	_fClv=0.1;          ///余隙（clearance）容积
	_fRes1=0.1;
	_fRes2=0.1;
	_iRes1=0;

	_fPistonRodLength                  = 0.5;
	_fPistonThickness                  = 0.05;

	_fPistonCylinderWallGap            = 0.1;
	_fSensorLocation                   = 0.4;
	_fPistonCylinderWallGap_Deflection = 0.1;
	_fSensorLocation_Deflection        = 0.4;
}

S_PlantInfoParam::S_PlantInfoParam(const S_PlantInfoParam_Deprecated & sDeprecated_)
{
	_fCrankShaftDia         = sDeprecated_._fCrankShaftDia;
	_fConnRodLength         = sDeprecated_._fConnRodLength;///连杆长度
	_fPistonDia             = sDeprecated_._fPistonDia;    ///活塞直径
	_fTotalPower            = sDeprecated_._fTotalPower;   ///总功率
	_fNomPi                 = sDeprecated_._fNomPi;        ///名义进气压力
	_fNomPo                 = sDeprecated_._fNomPo;        ///名义出气压力
	_fClv                   = sDeprecated_._fClv;          ///余隙（clearance）容积
	_fRes1                  = sDeprecated_._fRes1;
	_fRes2                  = sDeprecated_._fRes2;
	_iRes1                  = sDeprecated_._iRes1;
	_fPistonRodLength                  = 0.5;
	_fPistonThickness                  = 0.05;
	_fPistonCylinderWallGap            = 0.1;
	_fSensorLocation                   = 0.4;
	_fPistonCylinderWallGap_Deflection = 0.1;
	_fSensorLocation_Deflection        = 0.4;
}

S_PlantInfoParam&S_PlantInfoParam::operator =(const S_PlantInfoParam& sPlantInfoParam_)
{
	_fCrankShaftDia=sPlantInfoParam_._fCrankShaftDia;
	_fConnRodLength=sPlantInfoParam_._fConnRodLength;///连杆长度
	_fPistonDia=sPlantInfoParam_._fPistonDia;    ///活塞直径
	_fTotalPower=sPlantInfoParam_._fTotalPower;   ///总功率
	_fNomPi=sPlantInfoParam_._fNomPi;        ///名义进气压力
	_fNomPo=sPlantInfoParam_._fNomPo;        ///名义出气压力
	_fClv=sPlantInfoParam_._fClv;          ///余隙（clearance）容积
	_fRes1=sPlantInfoParam_._fRes1;
	_fRes2=sPlantInfoParam_._fRes2;
	_iRes1=sPlantInfoParam_._iRes1;

	_fPistonRodLength                  = sPlantInfoParam_._fPistonRodLength;
	_fPistonThickness                  = sPlantInfoParam_._fPistonThickness;
	_fPistonCylinderWallGap            = sPlantInfoParam_._fPistonCylinderWallGap;
	_fSensorLocation                   = sPlantInfoParam_._fSensorLocation;
	_fPistonCylinderWallGap_Deflection =
		sPlantInfoParam_._fPistonCylinderWallGap_Deflection;
	_fSensorLocation_Deflection        =
		sPlantInfoParam_._fSensorLocation_Deflection;

	return *this;
}


S_RecipComprParam_Deprecated::S_RecipComprParam_Deprecated()
{
	/// 在_sPlantInfoParam 上原地依次调用S_PlantInfoParam_Deprecated的构建函数
	for (int nloop=0;nloop<PLANTINFO_PARAM_NUM;++nloop)
	{
		new(_sPlantInfoParam + nloop) S_PlantInfoParam_Deprecated();
	}
}


S_RecipComprParam_Deprecated & S_RecipComprParam_Deprecated::operator =(const S_RecipComprParam_Deprecated& sRecipComprParam_)
{
	for (int nloop=0;nloop<PLANTINFO_PARAM_NUM;++nloop)
	{
		_sPlantInfoParam[nloop]=sRecipComprParam_._sPlantInfoParam[nloop];
	}
	return *this;
}

S_RecipComprParam::S_RecipComprParam()
{
	/// 在_sPlantInfoParam 上原地依次调用S_PlantInfoParam的构建函数
	for (int nloop=0;nloop<PLANTINFO_PARAM_NUM;++nloop)
	{
		new(_sPlantInfoParam + nloop) S_PlantInfoParam();
	}
}

S_RecipComprParam::S_RecipComprParam(const S_RecipComprParam_Deprecated & sDeprecated_)
{
	/// 在_sPlantInfoParam 上原地依次调用S_PlantInfoParam(deprecated)的构建函数
	for (int nloop=0;nloop<PLANTINFO_PARAM_NUM;++nloop)
	{
		new(_sPlantInfoParam + nloop) S_PlantInfoParam(sDeprecated_._sPlantInfoParam[nloop]);
	}
}

S_RecipComprParam&S_RecipComprParam::operator =(const S_RecipComprParam& sRecipComprParam_)
{
	for (int nloop=0;nloop<PLANTINFO_PARAM_NUM;++nloop)
	{
		_sPlantInfoParam[nloop]=sRecipComprParam_._sPlantInfoParam[nloop];
	}
	return *this;
}

#ifdef _BHKD_SERVER_SIDE_
bool S_PlantInfo::GetPlantInfo(CSimADO *pDB_)
{
	if (NULL==pDB_) return false;
	strncpy(_cCompany,pDB_->Get(gc_cCompany), D_BHKD_COMPANYNAME_LEN - 1);
	strncpy(_cSetID,  pDB_->Get(gc_cSetID),   D_BHKD_SETID_LEN - 1);
	strncpy(_cPlantID,pDB_->Get(gc_cPlantID), D_BHKD_PLANTID_LEN - 1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo), D_BHKD_PLANTNO_LEN - 1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name), D_BHKD_FACTORYNAME_LEN - 1);
	strncpy(_cManufacturer,pDB_->Get(gc_cManuFacturer), 100 - 1);
	_iVibChannNum=pDB_->GetLong(gc_cVibChann_Num);
	_iSmpFreq=pDB_->GetLong(gc_cSampleFreq);
	_iSmpNum=pDB_->GetLong(gc_cSampleNum);
	_iProcChannNum=pDB_->GetLong(gc_cProcChann_Num);
	_iDynChannNum=pDB_->GetLong(gc_cDynChann_Num);
	_piRev[0]=pDB_->GetLong(gc_cRev);
	_piRev[1]=pDB_->GetLong(gc_cRev2);
	_piRev[2]=pDB_->GetLong(gc_cRev3);
	_piRev[3]=pDB_->GetLong(gc_cRev4);
	_piRev[4]=pDB_->GetLong(gc_cRev5);
	_iGraphType=pDB_->GetLong(gc_cGraphType);
	_iMachineType=pDB_->GetLong(gc_cMachine_Type);
	_iDriverType=pDB_->GetLong(gc_cDriver_Type);
	_iConnType=pDB_->GetLong(gc_cConn_Type);
	_iBearingType=pDB_->GetLong(gc_cBear_Type);
	_iRotorType=pDB_->GetLong(gc_cRotate_Type);
	_iSealType=pDB_->GetLong(gc_cSeal_Type);
	_iWorkRevType=pDB_->GetLong(gc_cRev_Type);
	_iCritSpeed1st=pDB_->GetLong(gc_cCritSpeed_1st);
	_iCritSpeed2nd=pDB_->GetLong(gc_cCritSpeed_2nd);
	_firstRunT=pDB_->GetTime(gc_cFirstRun_T);
	strncpy(_sVersion, pDB_->Get(gc_cVersion), D_BHKD_VERSION_LEN - 1);
	_iCylinderNum=pDB_->GetLong(gc_cCylinder_Num);
	long iBuffSize = sizeof(S_RecipComprParam);
	SimADO_GetRecipComprParam(pDB_, (BYTE*)&(_sRecipComprParam),  iBuffSize);
	_iDataSource = pDB_->GetLong(gc_cData_Source);
	return true;
}

bool S_PlantInfo::GetPlantInfo(IDBInterFace *pDB_)
{
	if (NULL==pDB_) return false;
	strncpy(_cCompany,pDB_->Get(gc_cCompany).c_str(), D_BHKD_COMPANYNAME_LEN - 1);
	strncpy(_cSetID,  pDB_->Get(gc_cSetID).c_str(),   D_BHKD_SETID_LEN - 1);
	strncpy(_cPlantID,pDB_->Get(gc_cPlantID).c_str(), D_BHKD_PLANTID_LEN - 1);
	strncpy(_cPlantNo,pDB_->Get(gc_cPlantNo).c_str(), D_BHKD_PLANTNO_LEN - 1);
	strncpy(_cFactory,pDB_->Get(gc_cFactory_Name).c_str(), D_BHKD_FACTORYNAME_LEN - 1);
	strncpy(_cManufacturer,pDB_->Get(gc_cManuFacturer).c_str(), 100 - 1);
	_iVibChannNum=pDB_->GetLong(gc_cVibChann_Num);
	_iSmpFreq=pDB_->GetLong(gc_cSampleFreq);
	_iSmpNum=pDB_->GetLong(gc_cSampleNum);
	_iProcChannNum=pDB_->GetLong(gc_cProcChann_Num);
	_iDynChannNum=pDB_->GetLong(gc_cDynChann_Num);
	_piRev[0]=pDB_->GetLong(gc_cRev);
	_piRev[1]=pDB_->GetLong(gc_cRev2);
	_piRev[2]=pDB_->GetLong(gc_cRev3);
	_piRev[3]=pDB_->GetLong(gc_cRev4);
	_piRev[4]=pDB_->GetLong(gc_cRev5);
	_iGraphType=pDB_->GetLong(gc_cGraphType);
	_iMachineType=pDB_->GetLong(gc_cMachine_Type);
	_iDriverType=pDB_->GetLong(gc_cDriver_Type);
	_iConnType=pDB_->GetLong(gc_cConn_Type);
	_iBearingType=pDB_->GetLong(gc_cBear_Type);
	_iRotorType=pDB_->GetLong(gc_cRotate_Type);
	_iSealType=pDB_->GetLong(gc_cSeal_Type);
	_iWorkRevType=pDB_->GetLong(gc_cRev_Type);
	_iCritSpeed1st=pDB_->GetLong(gc_cCritSpeed_1st);
	_iCritSpeed2nd=pDB_->GetLong(gc_cCritSpeed_2nd);
	_firstRunT=pDB_->GetTime(gc_cFirstRun_T);
	strncpy(_sVersion, pDB_->Get(gc_cVersion).c_str(), D_BHKD_VERSION_LEN - 1);
	_iCylinderNum=pDB_->GetLong(gc_cCylinder_Num);
	long iBuffSize = sizeof(S_RecipComprParam);
	SimADO_GetRecipComprParamEx(pDB_, (BYTE*)&(_sRecipComprParam),  iBuffSize);
	_iDataSource = pDB_->GetLong(gc_cData_Source);
	return true;
}
bool S_PlantInfo::GetPlantInfo(CCustomDB *pDB_)
{
	if (NULL==pDB_) return false;
	return GetPlantInfo((CSimADO*)pDB_);
}

bool S_PlantInfo::SetPlantInfo(CSimADO *pDB_)
{
	long iSize=sizeof(S_RecipComprParam);
	return SimADO_SetRecipComprParam(pDB_,(BYTE*)&(_sRecipComprParam),iSize);
}

bool S_PlantInfo::SetPlantInfo(IDBInterFace *pDB_)
{
	long iSize=sizeof(S_RecipComprParam);
	return SimADO_SetRecipComprParamEx(pDB_,(BYTE*)&(_sRecipComprParam),iSize);
}
#endif  //_BHKD_SERVER_SIDE_





//---------------------------------------------------------//
CThreshold& CThreshold::operator =(const CThreshold& th_)
{
	_fThreshold1=th_._fThreshold1;
	_fThreshold2=th_._fThreshold2;
	_fThreshold3=th_._fThreshold3;
	_fThreshold4=th_._fThreshold4;
	_fThreshold5=th_._fThreshold5;
	_bThreshhold1=th_._bThreshhold1;
	return *this;
}

CThreshold::CThreshold()
{
	this->_bThreshhold1 = false;

	this->_fThreshold1 = 0;

	this->_fThreshold2 = 0;

	this->_fThreshold3 = 0;

	this->_fThreshold4 = 0;

	this->_fThreshold5 = 0;
	
}

CTrendCheckParam::CTrendCheckParam()
{
	_fParam1=.0;
	_fParam2=.0;
	_fParam3=.0;
	_fParam4=.0;
	_fParam5=.0;
}

CTrendCheckParam& CTrendCheckParam::operator=(const CTrendCheckParam& tParam_)
{
	_fParam1=tParam_._fParam1;
	_fParam2=tParam_._fParam2;
	_fParam3=tParam_._fParam3;
	_fParam4=tParam_._fParam4;
	_fParam5=tParam_._fParam5;
	return *this;
}


CThresholdLearnParam::CThresholdLearnParam()
{
	_fParam1=.1;
	_fParam2=.1;
	_fParam3=.1;
	_fParam4=.1;
	_fParam5=.1;
}

CThresholdLearnParam& CThresholdLearnParam::operator=(const CThresholdLearnParam& thLParam_)
{
	_fParam1=thLParam_._fParam1;
	_fParam2=thLParam_._fParam2;
	_fParam3=thLParam_._fParam3;
	_fParam4=thLParam_._fParam4;
	_fParam5=thLParam_._fParam5;
	return *this;
}

//-------------------------------------------//

S_VibCharValue::S_VibCharValue()
{
	_fCharAll=0;
	_fCharHalf=0;
	_fCharOne=0;
	_fCharTwo=0;
	_fCharThree=0;
	_fCharRes=0;
	_fDC=0;
	_fGap=0;
	_fPhaseHalf=0;
	_fPhaseOne=0;
	_fPhaseTwo=0;
	_fPhaseThree=0;
	_iSmpFreq=0;
	_iSmpNum=0;
	_iRev=0;
}

S_VibCharValue::~S_VibCharValue()
{
}


DATATYPE_CHAR S_VibCharValue::GetSpecTypeData(int iCharType_)
{
	float lfRet = 0;
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		lfRet = _fCharAll;
		break;
	case GE_HALFCHAR:
		lfRet = _fCharHalf;
		break;
	case GE_ONECHAR:
		lfRet =  _fCharOne;
		break;
	case GE_TWOCHAR:
		lfRet = _fCharTwo;
		break;
	case GE_THREECHAR:
		lfRet = _fCharThree;
		break;
	case GE_DCCHAR:
		lfRet =  _fDC;
		break;
	case GE_GAP:
		lfRet = _fGap;
		break;
	case GE_RESCHAR:
		lfRet = _fCharRes;
		break;
	case GE_PHASECHARONE:
		lfRet =  _fPhaseOne;
		break;
	case GE_PHASECHARTWO:
		lfRet = _fPhaseTwo;
		break;
	case GE_PHASECHARTHREE:
		lfRet = _fPhaseThree;
		break;
	case GE_PHASECHARHALF:
		lfRet =  _fPhaseHalf;
		break;
	case GE_SAMPLEFREQ:
		lfRet =  _iSmpFreq;
		break;
	case GE_SAMPLENUM:
		lfRet = _iSmpNum;
		break;
	case GE_REV:
		lfRet = _iRev;
		break;
	default:
		lfRet = _fCharAll;
		break;
	}
	
	return lfRet;
}

void S_VibCharValue::SetSpecTypeData(float fData_,int iCharType_/* =GE_ALLCHAR */)
{
	if (!IsFloatValid(fData_))
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************特征值错误! %f %s %d******************"),fData_,__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		fData_ = 0;
	}
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		_fCharAll=fData_;
		break;
	case GE_HALFCHAR:
		_fCharHalf=fData_;
		break;
	case GE_ONECHAR:
		_fCharOne=fData_;
		break;
	case GE_TWOCHAR:
		_fCharTwo=fData_;
		break;
	case GE_THREECHAR:
		_fCharThree=fData_;
		break;
	case GE_DCCHAR:
		_fDC=fData_;
		break;
	case GE_GAP:
		_fGap=fData_;
		break;
	case GE_RESCHAR:
		_fCharRes=fData_;
		break;
	case GE_PHASECHARONE:
		_fPhaseOne=fData_;
		break;
	case GE_PHASECHARTWO:
		_fPhaseTwo=fData_;
		break;
	case GE_PHASECHARTHREE:
		_fPhaseThree=fData_;
		break;
	case GE_PHASECHARHALF:
		_fPhaseHalf=fData_;
		break;
	case GE_SAMPLEFREQ:
		_iSmpFreq = (int)fData_;
		break;
	case GE_SAMPLENUM:
//		ASSERT(fData_>0);
		_iSmpNum  = (int)fData_;
		break;
	case GE_REV:
		_iRev= (int)fData_;
		break;

	}
}

S_VibCharValue &S_VibCharValue::operator =(const S_VibCharValue& vibValue_)
{
	_fCharAll=vibValue_._fCharAll;
	_fCharHalf=vibValue_._fCharHalf;
	_fCharOne=vibValue_._fCharOne;
	_fCharTwo=vibValue_._fCharTwo;
	_fCharThree=vibValue_._fCharThree;
	_fCharRes=vibValue_._fCharRes;
	_fDC=vibValue_._fDC;
	_fGap=vibValue_._fGap;
	_fPhaseHalf=vibValue_._fPhaseHalf;
	_fPhaseOne=vibValue_._fPhaseOne;
	_fPhaseTwo=vibValue_._fPhaseTwo;
	_fPhaseThree=vibValue_._fPhaseThree;
	_iSmpFreq=vibValue_._iSmpFreq;
//	ASSERT(vibValue_._iSmpNum>0);
	_iSmpNum=vibValue_._iSmpNum;
	_iRev=vibValue_._iRev;
	return *this;
}
//------------------------------------------//
S_OldVibCharValue::S_OldVibCharValue()
{
	_fCharAll=0;
	_fCharHalf=0;
	_fCharOne=0;
	_fCharTwo=0;
	_fCharThree=0;
	_fCharRes=0;
	_fDC=0;
	_fGap=0;
	_fPhaseHalf=0;
	_fPhaseOne=0;
	_fPhaseTwo=0;
	_fPhaseThree=0;
}

S_OldVibCharValue::~S_OldVibCharValue()
{
}

DATATYPE_CHAR S_OldVibCharValue::GetSpecTypeData(int iCharType_)
{
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		return _fCharAll;
	case GE_HALFCHAR:
		return _fCharHalf;
	case GE_ONECHAR:
		return _fCharOne;
	case GE_TWOCHAR:
		return _fCharTwo;
	case GE_THREECHAR:
		return _fCharThree;
	case GE_DCCHAR:
		return _fDC;
	case GE_GAP:
		return _fGap;
	case GE_RESCHAR:
		return _fCharRes;
	case GE_PHASECHARONE:
		return _fPhaseOne;
	case GE_PHASECHARTWO:
		return _fPhaseTwo;
	case GE_PHASECHARTHREE:
		return _fPhaseThree;
	case GE_PHASECHARHALF:
		return _fPhaseHalf;
	default:
		return _fCharAll;
	}
}

void S_OldVibCharValue::SetSpecTypeData(float fData_,int iCharType_/* =GE_ALLCHAR */)
{

	if (!IsFloatValid(fData_))
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************特征值错误! %f %s %d******************"),fData_,__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		fData_ = 0;
	}
	switch(iCharType_)
	{
	case GE_ALLCHAR:
		_fCharAll=fData_;
		break;
	case GE_HALFCHAR:
		_fCharHalf=fData_;
		break;
	case GE_ONECHAR:
		_fCharOne=fData_;
		break;
	case GE_TWOCHAR:
		_fCharTwo=fData_;
		break;
	case GE_THREECHAR:
		_fCharThree=fData_;
		break;
	case GE_DCCHAR:
		_fDC=fData_;
		break;
	case GE_GAP:
		_fGap=fData_;
		break;
	case GE_RESCHAR:
		_fCharRes=fData_;
		break;
	case GE_PHASECHARONE:
		_fPhaseOne=fData_;
		break;
	case GE_PHASECHARTWO:
		_fPhaseTwo=fData_;
		break;
	case GE_PHASECHARTHREE:
		_fPhaseThree=fData_;
		break;
	case GE_PHASECHARHALF:
		_fPhaseHalf=fData_;
		break;
	}
}

S_OldVibCharValue &S_OldVibCharValue::operator =(const S_OldVibCharValue& vibValue_)
{
	_fCharAll=vibValue_._fCharAll;
	_fCharHalf=vibValue_._fCharHalf;
	_fCharOne=vibValue_._fCharOne;
	_fCharTwo=vibValue_._fCharTwo;
	_fCharThree=vibValue_._fCharThree;
	_fCharRes=vibValue_._fCharRes;
	_fDC=vibValue_._fDC;
	_fGap=vibValue_._fGap;
	_fPhaseHalf=vibValue_._fPhaseHalf;
	_fPhaseOne=vibValue_._fPhaseOne;
	_fPhaseTwo=vibValue_._fPhaseTwo;
	_fPhaseThree=vibValue_._fPhaseThree;
	return *this;
}
//------------------------------------------//

S_DynCharValue::S_DynCharValue()
{
	_fCharValue0=0;
	_fCharValue1=0;
	_fCharValue2=0;
	_fCharValue3=0;
	_fCharValue4=0;
	_fCharValue5=0;
	_fCharValue6=0;
	_fCharValue7=0;
	_fCharValue8=0;
	_fCharValue9=0;
	_fCharValue10=0;
	_fCharValue11=0;
    _iSmpFreq=0;
    _iSmpNum=0;
	_iRev=0;
}

S_DynCharValue::~S_DynCharValue()
{

}

S_DynCharValue& S_DynCharValue::operator =(const S_DynCharValue& dynValue_)
{
	_fCharValue0=dynValue_._fCharValue0;
	_fCharValue1=dynValue_._fCharValue1;
	_fCharValue2=dynValue_._fCharValue2;
	_fCharValue3=dynValue_._fCharValue3;
	_fCharValue4=dynValue_._fCharValue4;
	_fCharValue5=dynValue_._fCharValue5;
	_fCharValue6=dynValue_._fCharValue6;
	_fCharValue7=dynValue_._fCharValue7;
	_fCharValue8=dynValue_._fCharValue8;
	_fCharValue9=dynValue_._fCharValue9;
	_fCharValue10=dynValue_._fCharValue10;
	_fCharValue11=dynValue_._fCharValue11;
	_iSmpFreq=dynValue_._iSmpFreq;
	_iSmpNum=dynValue_._iSmpNum;
	_iRev=dynValue_._iRev;
	return *this;
}

DATATYPE_CHAR S_DynCharValue::GetSpecTypeData(int iCharType_/* =0 */)
{
	switch(iCharType_)
	{
	case 0:
		return _fCharValue0;
	case 1:
		return _fCharValue1;
	case 2:
		return _fCharValue2;
	case 3:
		return _fCharValue3;
	case 4:
		return _fCharValue4;
	case 5:
		return _fCharValue5;
	case 6:
		return _fCharValue6;
	case 7:
		return _fCharValue7;
	case 8:
		return _fCharValue8;
	case 9:
		return _fCharValue9;
	case 10:
		return _fCharValue10;
	case 11:
		return _fCharValue11;
	case GE_SAMPLEFREQ:
		return _iSmpFreq;
	case GE_SAMPLENUM:
		return _iSmpNum;
	case GE_REV:
		return _iRev;
	}
	return _fCharValue0;
}

void S_DynCharValue::SetSpecTypeData(const float &arefData,int iCharType_/* =0 */)
{
	float lfTempData = arefData;

	if (!IsFloatValid(lfTempData))
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************特征值错误! %f %s %d******************"),lfTempData,__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		lfTempData = 0;
	}

	switch(iCharType_)
	{
	case 0:
		_fCharValue0=lfTempData;
		break;
	case 1:
		_fCharValue1=lfTempData;
		break;
	case 2:
		_fCharValue2=lfTempData;
		break;
	case 3:
		_fCharValue3=lfTempData;
		break;
	case 4:
		_fCharValue4=lfTempData;
		break;
	case 5:
		_fCharValue5=lfTempData;
		break;
	case 6:
		_fCharValue6=lfTempData;
		break;
	case 7:
		_fCharValue7=lfTempData;
		break;
	case 8:
		_fCharValue8=lfTempData;
		break;
	case 9:
		_fCharValue9=lfTempData;
		break;
	case 10:
		_fCharValue10=lfTempData;
		break;
	case 11:
		_fCharValue11=lfTempData;
		break;
	case GE_SAMPLEFREQ:
		_iSmpFreq = (int)lfTempData;
		break;
	case GE_SAMPLENUM:
		_iSmpNum  = (int)lfTempData;
		break;
	case GE_REV:
		_iRev= (int)lfTempData;
		break;
	default:
		_fCharValue0=lfTempData;
	}
}
//------------------------------------------//

S_OldDynCharValue::S_OldDynCharValue()
{
	_fCharValue0=0;
	_fCharValue1=0;
	_fCharValue2=0;
	_fCharValue3=0;
	_fCharValue4=0;
	_fCharValue5=0;
	_fCharValue6=0;
	_fCharValue7=0;
	_fCharValue8=0;
	_fCharValue9=0;
	_fCharValue10=0;
	_fCharValue11=0;
}

S_OldDynCharValue::~S_OldDynCharValue()
{

}

S_OldDynCharValue& S_OldDynCharValue::operator =(const S_OldDynCharValue& dynValue_)
{
	_fCharValue0=dynValue_._fCharValue0;
	_fCharValue1=dynValue_._fCharValue1;
	_fCharValue2=dynValue_._fCharValue2;
	_fCharValue3=dynValue_._fCharValue3;
	_fCharValue4=dynValue_._fCharValue4;
	_fCharValue5=dynValue_._fCharValue5;
	_fCharValue6=dynValue_._fCharValue6;
	_fCharValue7=dynValue_._fCharValue7;
	_fCharValue8=dynValue_._fCharValue8;
	_fCharValue9=dynValue_._fCharValue9;
	_fCharValue10=dynValue_._fCharValue10;
	_fCharValue11=dynValue_._fCharValue11;
	return *this;
}

DATATYPE_CHAR S_OldDynCharValue::GetSpecTypeData(int iCharType_/* =0 */)
{
	switch(iCharType_)
	{
	case 0:
		return _fCharValue0;
	case 1:
		return _fCharValue1;
	case 2:
		return _fCharValue2;
	case 3:
		return _fCharValue3;
	case 4:
		return _fCharValue4;
	case 5:
		return _fCharValue5;
	case 6:
		return _fCharValue6;
	case 7:
		return _fCharValue7;
	case 8:
		return _fCharValue8;
	case 9:
		return _fCharValue9;
	case 10:
		return _fCharValue10;
	case 11:
		return _fCharValue11;
	}
	return _fCharValue0;
}

void S_OldDynCharValue::SetSpecTypeData(const float &arefData,int iCharType_/* =0 */)
{
	float lfTempData = arefData;

	if (!IsFloatValid(lfTempData))
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************特征值错误! %f %s %d******************"),lfTempData,__FILE__,__LINE__);
		CHZLogManage::Error(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		lfTempData = 0;
	}

	switch(iCharType_)
	{
	case 0:
		_fCharValue0=lfTempData;
		break;
	case 1:
		_fCharValue1=lfTempData;
		break;
	case 2:
		_fCharValue2=lfTempData;
		break;
	case 3:
		_fCharValue3=lfTempData;
		break;
	case 4:
		_fCharValue4=lfTempData;
		break;
	case 5:
		_fCharValue5=lfTempData;
		break;
	case 6:
		_fCharValue6=lfTempData;
		break;
	case 7:
		_fCharValue7=lfTempData;
		break;
	case 8:
		_fCharValue8=lfTempData;
		break;
	case 9:
		_fCharValue9=lfTempData;
		break;
	case 10:
		_fCharValue10=lfTempData;
		break;
	case 11:
		_fCharValue11=lfTempData;
		break;
	default:
		_fCharValue0=lfTempData;
	}
}

//-------------------------------------------//

CChannAlarmParam::CChannAlarmParam()
{
	_bCharTrend=false;
	_bPhaseTrend=false;
	_bRegular=false;
	_bCharSlow=false;
	_bCharFast=false;
	_bPhaseSlow=false;
	_bPhaseFast=false;
	_fAlarmHH=.3;
	_fAlarmHL=.2;
	_fAlarmLH=.1;
	_fAlarmLL=.0;
	_iRegJudgeType=0;
	_iRegCharType=E_TH_ALLCHAR;
	_iRegAlarmSingleOrMulti=0;
	_fAlarm1= .1;
	_fAlarm2= .1;
	_fAlarm3= .1;
	_iResv1 = 0;
}

CChannAlarmParam& CChannAlarmParam::operator=(const CChannAlarmParam& chAlarmParam_)
{
	_bCharTrend=chAlarmParam_._bCharTrend;
	_bPhaseTrend=chAlarmParam_._bPhaseTrend;
	_bRegular=chAlarmParam_._bRegular;
	_bCharSlow=chAlarmParam_._bCharSlow;
	_bCharFast=chAlarmParam_._bCharFast;
	_bPhaseSlow=chAlarmParam_._bPhaseSlow;
	_bPhaseFast=chAlarmParam_._bPhaseFast;
	for (int nloop=0;nloop<D_BHKD_ALARM_CHARTYPE_NUM;++nloop)
	{
		_pThLearnParamS[nloop]=chAlarmParam_._pThLearnParamS[nloop];
		_pThLearnParamF[nloop]=chAlarmParam_._pThLearnParamF[nloop];
		_pTrendParam[nloop]=chAlarmParam_._pTrendParam[nloop];
	}
	_fAlarmHH=chAlarmParam_._fAlarmHH;
	_fAlarmHL=chAlarmParam_._fAlarmHL;
	_fAlarmLH=chAlarmParam_._fAlarmLH;
	_fAlarmLL=chAlarmParam_._fAlarmLL;
    _iRegJudgeType=chAlarmParam_._iRegJudgeType;
	_iRegAlarmSingleOrMulti=chAlarmParam_._iRegAlarmSingleOrMulti;
	_iRegCharType=chAlarmParam_._iRegCharType;

	_fAlarm1=chAlarmParam_._fAlarm1;
	_fAlarm2=chAlarmParam_._fAlarm2;
	_fAlarm3=chAlarmParam_._fAlarm3;
	_iResv1=chAlarmParam_._iResv1;
	return *this;
}

S_PlantNumStatics &S_PlantNumStatics::operator +(const S_PlantNumStatics& plantNumStatics_)
{
	_iPlantNum+=plantNumStatics_._iPlantNum;
	_iAlarmPlantNum+=plantNumStatics_._iAlarmPlantNum;
	_iRunPlantNum+=plantNumStatics_._iRunPlantNum;
	_iStopPlantNum+=plantNumStatics_._iStopPlantNum;
	return *this;
}

S_PlantNumStatics &S_PlantNumStatics::operator =(const S_PlantNumStatics& plantNumStatics_)
{
	_iPlantNum = plantNumStatics_._iPlantNum;
	_iAlarmPlantNum= plantNumStatics_._iAlarmPlantNum;
	_iRunPlantNum  = plantNumStatics_._iRunPlantNum;
	_iStopPlantNum = plantNumStatics_._iStopPlantNum;
	return *this;
}



#ifdef _BHKD_SERVER_SIDE_

void SwitchDbNameEx(IDBInterFace *pDB_,CString sDbName_)
{
	if (g_iDbType==1)//当配置数据库的类型值为1时，说明是分库的数据库，则应切换数据库
	{
		if (pDB_ == NULL)
		{
			return ;
		}

		pDB_->DbUse("master");
		try
		{
			if (sDbName_.Find(_T('[')) >=0)
			{
				
			}else
			{
				sDbName_  = _T('[') + sDbName_;

				sDbName_ += _T(']');
			}

			pDB_->DbUse(sDbName_);
			
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return;
		}

	}
	else
		return;
}



//当连接分库数据库时，切换数据库名
bool SwichDbName(CCustomDB *pDB_,CString sDbName_)
{
	if (g_iDbType==1)//当配置数据库的类型值为1时，说明是分库的数据库，则应切换数据库
	{
		if (NULL == pDB_)
		{
			return false;
		}

		try
		{
			pDB_->Cmd("use [%s];", sDbName_);
			bool lbRet =  pDB_->Exec();
			return lbRet;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}

	}
	else
		return true;
}

/*!
 \param[out] nSize: GetBuffer大小并非实际读出大小
 */
bool SimADO_SafeGetBuffer( CSimADO * pDB, const char * sField, BYTE *pBuf, long &nSize)
{
	if (NULL == pDB || NULL == pBuf ) {
		HZLOG_ERROR(STR("请求字段%s异常：数据库指针(0x%X), 缓冲区指针(0x%X)", pDB, pBuf));
		return false;
	}

	if (nSize < 0) {
		HZLOG_ERROR(STR("请求字段%s异常：请求大小=%d", nSize));
		return false;
	}



	long nActualSize = pDB->GetActualSize(sField);

    ::memset(pBuf, 0, nSize);

	long anBufferSize = nSize;

	if(nSize <  nActualSize) {
		HZLOG_ERROR(STR("请求字段%s异常：要求大小小于实际大小", sField));
		return false;
	}

	if (nSize > nActualSize)
	{
		anBufferSize = nActualSize;
	}

	if (anBufferSize == 0)
	{
		return false;
	}

	bool bRet = pDB->GetBuffer(sField,  (BYTE *)pBuf,  anBufferSize);

	return bRet;
}

bool SimADO_GetRecipComprParam(
	CSimADO * pDB_, BYTE *pBuf_, long &nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	long nActualSize = pDB_->GetActualSize(gc_cRecipComprParam);

	if(nActualSize == sizeof(S_RecipComprParam_Deprecated) ){
		S_RecipComprParam_Deprecated sDepricatedParam;
		bool bRet = pDB_->GetBuffer(gc_cRecipComprParam,
									(BYTE *)&sDepricatedParam,
									nActualSize);
		// 使用原S_RecipComprParam_Deprecated 构造 S_RecipComprParam
		// 兼容旧数据库
		::new(pBuf_) S_RecipComprParam(sDepricatedParam);
		/// 返回S_RecipComprParam的大小
		nSize_ = sizeof(S_RecipComprParam);
		return bRet;
	}

	if(nActualSize != sizeof(S_RecipComprParam) ){
		HZLOG_ERROR(STR(
			"往复压缩机参数(S_RecipComprParam)与数据库字段%s大小不一致",
			gc_cRecipComprParam));
	}

	return SimADO_SafeGetBuffer(pDB_, gc_cRecipComprParam, pBuf_, nSize_);

}

bool SimADO_SetRecipComprParam(
	CSimADO * pDB_, BYTE *pBuf_, long nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	ASSERT(nSize_ > 0);
	if (nSize_ <= 0)	{
		return false;
	}

	return pDB_->SetBuffer(gc_cRecipComprParam, pBuf_, nSize_);
}

bool SimADO_SetRecipComprParamEx(
							   IDBInterFace * pDB_, BYTE *pBuf_, long nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	ASSERT(nSize_ > 0);
	if (nSize_ <= 0)	{
		return false;
	}

	return pDB_->SetBuffer(gc_cRecipComprParam, pBuf_, nSize_);
}



bool SimADO_GetChannAlarmParam(
							   CSimADO * pDB_, BYTE *pBuf_, long &nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	long nActualSize = pDB_->GetActualSize(gc_cAlarmParam);

	long nObjSize = sizeof(CChannAlarmParam) ;

	if(nActualSize != nObjSize){
		/*ASSERT(FALSE);*/
		//HZLOG_ERROR(STR(
		//	"测点报警参数(CChannAlarmParam)与数据库字段%s大小不一致",
		//	gc_cAlarmParam));
	}

	return SimADO_SafeGetBuffer(pDB_, gc_cAlarmParam, pBuf_, nSize_);

}

bool SimADO_SetChannAlarmParam(
	CSimADO * pDB_, BYTE *pBuf_, long nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	ASSERT(nSize_ > 0);
	if (nSize_ <= 0)	{
		return false;
	}

	return pDB_->SetBuffer(gc_cAlarmParam, pBuf_, nSize_);
}

bool SimADO_SetChannAlarmParamEx(
							   IDBInterFace * pDB_, BYTE *pBuf_, long nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	ASSERT(nSize_ > 0);
	if (nSize_ <= 0)	{
		return false;
	}

	return pDB_->SetBuffer(gc_cAlarmParam, pBuf_, nSize_);
}



S_Machineinfotype::S_Machineinfotype()
{
	this->m_nField = 0;
	this->m_nField1 = 0;
	this->m_nIndex = 0;
	this->m_nInfoType = 0;
	
}


S_Machineinfotype::~S_Machineinfotype()
{
	this->m_nField		=	-1;
	this->m_nField1		=	-1;
	this->m_nIndex		=	-1;
	this->m_nInfoType	=	-1;
}

BOOL S_Machineinfotype::LoaddData(IDBInterFace & arefDB)
{
	BOOL lbRet = FALSE;
	

	try
	{
		this->m_nInfoType = arefDB.GetLong(gc_cInfoType);
		this->m_nField = arefDB.GetLong(gc_cFiled);
		this->m_nField1 = arefDB.GetLong(gc_cFileld1);
		this->m_nIndex = arefDB.GetLong(gc_cIndex);
		this->m_strComment = arefDB.Get(gc_cComment).c_str();
		this->m_strUnit = arefDB.Get(gc_cUnit).c_str();
		lbRet = TRUE;
	}
	catch (...)
	{
	    //log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		lbRet = FALSE;
	}

	return lbRet;
	
}


bool SimADO_SafeGetBufferEx( IDBInterFace * pDB, const char * sField, BYTE *pBuf, long &nSize,  CWaveHead& sWaveHead_)
{
	if (NULL == pDB || NULL == pBuf ) { 
		HZLOG_ERROR(STR("请求字段%s异常：数据库指针(0x%X), 缓冲区指针(0x%X)", pDB, pBuf));
		return false;
	}

	if (nSize < 0) {
		HZLOG_ERROR(STR("请求字段%s异常：请求大小=%d", nSize));
		return false;
	}

	//HZLOG_DEBUG(STR("Field=%s", sField));
	//HZLOG_DEBUG(STR("InPutSize=%d", nSize));

	long nActualSize = pDB->GetActualSize(sField);
	//HZLOG_DEBUG(STR("ActualSize=%d", nActualSize));

	::memset(pBuf, 0, nSize);

	if (nSize!= nActualSize)
	{
		zdlTraceLine(_T("size not equ dbsize:%d objSize:%d"),nActualSize,nSize);

		zdlTraceLine(_T("sql not match :%s "),pDB->GetSql());
		
	}

	if(nSize <  nActualSize) {
		HZLOG_ERROR(STR("请求字段%s异常：要求大小小于实际大小", sField));
/*		return false;*/
	}

	bool bRet = pDB->GetBuffer(sField,  (BYTE *)pBuf,  nSize, sWaveHead_);

	//HZLOG_DEBUG(STR("OutPutSize=%d", nSize));
	//HZLOG_DEBUG(STR("Return=%d", bRet));

	return bRet;
}

bool SimADO_GetRecipComprParamEx(
	IDBInterFace * pDB_, BYTE *pBuf_, long &nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	long nActualSize = pDB_->GetActualSize(gc_cRecipComprParam);

	CWaveHead sWaveHead;

	if(nActualSize == sizeof(S_RecipComprParam_Deprecated) ){
		S_RecipComprParam_Deprecated sDepricatedParam;
		
		bool bRet = pDB_->GetBuffer(gc_cRecipComprParam,
									(BYTE *)&sDepricatedParam,
									nActualSize,
									sWaveHead);
		// 使用原S_RecipComprParam_Deprecated 构造 S_RecipComprParam
		// 兼容旧数据库
		::new(pBuf_) S_RecipComprParam(sDepricatedParam);
		/// 返回S_RecipComprParam的大小
		nSize_ = sizeof(S_RecipComprParam);
		return bRet;
	}

	if(nActualSize != sizeof(S_RecipComprParam) ){
		HZLOG_ERROR(STR(
			"往复压缩机参数(S_RecipComprParam)与数据库字段%s大小不一致",
			gc_cRecipComprParam));
	}

	return SimADO_SafeGetBufferEx(pDB_, gc_cRecipComprParam, pBuf_, nSize_, sWaveHead);

}



bool SimADO_GetChannAlarmParamEx(
	IDBInterFace * pDB_, BYTE *pBuf_, long &nSize_)
{
	ASSERT(pDB_ != NULL);
	if (NULL == pDB_)	{
		return false;
	}

	ASSERT(pBuf_ != NULL);
	if (NULL == pBuf_)	{
		return false;
	}

	long nActualSize = pDB_->GetActualSize(gc_cAlarmParam);

	if(nActualSize != sizeof(CChannAlarmParam) ){
		HZLOG_ERROR(STR(
			"测点报警参数(CChannAlarmParam)与数据库字段%s大小不一致",
			gc_cAlarmParam));
	}

	CWaveHead sWaveHead;
	return SimADO_SafeGetBufferEx(pDB_, gc_cAlarmParam, pBuf_, nSize_, sWaveHead);

}

BOOL IsDynCharValid(BHKD::S_DynCharValue * lpCharVal)
{
	if (lpCharVal == NULL)
	{
		return FALSE;
	}

	for (int i = 0;i<15;i++ )
	{
		float lfData = lpCharVal->GetSpecTypeData(i);

		if (!IsFloatValid(lfData))
		{
			return FALSE;
		}
	}

	if (lpCharVal->_iSmpNum>GD_MIDDLEWARE_WAVE_POINT || lpCharVal->_iSmpNum < 0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL IsVibCharValid(BHKD::S_VibCharValue * lpCharVal)
{
	if (lpCharVal == NULL)
	{
		return FALSE;
	}

	for (int i = 0;i<15;i++ )
	{
		float lfData = lpCharVal->GetSpecTypeData(i);

		if (!IsFloatValid(lfData))
		{
			return FALSE;
		}
	}

	if (lpCharVal->_iSmpNum>GD_MIDDLEWARE_WAVE_POINT || lpCharVal->_iSmpNum<0)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL IsFloatValid(float f)
{
	int lnResult = _fpclass(f);
	switch (lnResult)
	{
	case _FPCLASS_SNAN:
	case _FPCLASS_QNAN:
	case _FPCLASS_NINF:
	case _FPCLASS_ND:
	case _FPCLASS_PD:
	case _FPCLASS_PINF:
		return FALSE;
		break;
	default:
		return TRUE;
	}
}

BOOL IsFloatZero(float f)
{
	if (f < 0.000001 && f > -0.000001)
	{
		return TRUE;
	}

	return FALSE;
}
int float_as_int( float in )
{
	int lndata = *( ( int* ) &in );
	return lndata;
}

float int_as_float( int in )
{

	float lndata = *( ( float* ) &in );
	return lndata;

}
}



#endif  //_BHKD_SERVER_SIDE_

