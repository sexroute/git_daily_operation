#include "StdAfx.h"
#include "NetOffDataSave.h"
#include "additions/DebugHelper.h"
#include "MiddleWare.h"


//------------------------------------------------------------//
CNetOffPlantInfo::CNetOffPlantInfo()
{
	CBHDateTime timefirstSave= CBHDateTime(2000,1,1,1,1,1);
    _timeNetOffSaveVibHour=timefirstSave;
    _timeNetOffSaveVibDay=timefirstSave;
    _timeNetOffSaveVibMonth=timefirstSave;

    _timeNetOffSaveDynHour=timefirstSave;
    _timeNetOffSaveDynDay=timefirstSave;
    _timeNetOffSaveDynMonth=timefirstSave;

    _timeNetOffSaveProcHour=timefirstSave;
    _timeNetOffSaveProcDay=timefirstSave;
    _timeNetOffSaveProcMonth=timefirstSave;

	_pRealPlantInfo=NULL;

   CHZLogManage::Info("construction","CNetOffPlantInfo","CNetOffPlantInfo");
}

CNetOffPlantInfo::~CNetOffPlantInfo()
{
	Destory();
}

void CNetOffPlantInfo::Destory()
{
	KD_DeletePoint(_pWave);
	KD_DeletePoint(_pWaveDyn);
	KD_DeletePoint(_pWave_real);
	KD_DeletePoint(_pWaveDyn_real);
	_pRealPlantInfo=NULL;
}

CRealPlantInfo *  CNetOffPlantInfo::GetRealPlantInfo()
{
	CRealPlantInfo * lpPlantInfo = NULL;

	_pLockChannInfo->BeginRead();

	lpPlantInfo = this->_pRealPlantInfo;

	_pLockChannInfo->EndRead();

	return lpPlantInfo;
}
void CNetOffPlantInfo::AddChannInfoEnd(CCustomDB * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的组成数据保存时，插入的SQL语句
	//振动数据
	int iSmpNum = _sPlantInfo._iSmpNum;
	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave      = new DATATYPE_WAVE[iNum];
	_pWave_real = new DATATYPE_WAVE[iNum];
	//memset( _pWave, 0, iNum*sizeof(DATATYPE_WAVE));
	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real  = new DATATYPE_WAVE[iNum];
	//memset( _pWaveDyn, 0, iNum*sizeof(DATATYPE_WAVE));
	CString strInfo;
	strInfo.Format("vibNum: %d,dynNum: %d, SampleNum: %d,%d", _sPlantInfo._iVibChannNum,_sPlantInfo._iDynChannNum, iSmpNum, iNum);
	CHZLogManage::Info(strInfo, "Online", "CRealPlantInfo::AddChannInfoEnd");
	//构造振动通道数据表单名字
	V_PCHANNINFO::iterator itVib;
	_sVibFieldSel= "";
	_sVibFieldIns= "";
	CString strTmp="",strTmp1="";
	CVibChannParam *pVibChann=NULL;
	int index=0;
	
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		pVibChann=(CVibChannParam*)(*itVib);
		_sVibFieldIns.append(pVibChann->GetCharValueField());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-3).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-4).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-5).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-6).c_str());
		_sVibFieldIns.append(",");
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-1).c_str());
		_sVibFieldSel+=strTmp;
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-7).c_str());
		_sVibFieldSel+=strTmp;
		pVibChann->SetWaveMemoryPoint(&(_pWave[index*iSmpNum]),iSmpNum);
		pVibChann->SetWaveMemoryPoint_real(&(_pWave_real[index*iSmpNum]),iSmpNum);

	

		++index;
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sVibFieldIns += strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sVibFieldSel += strTmp;
	///动态测点，构造动态数据表单
	V_PCHANNINFO::iterator itDyn;
	_sDynFieldIns= "";
	_sDynFieldSel= "";
	CDynChannParam *pDynChann=NULL;
	index = 0;
	for (itDyn = _vDynChann.begin(); itDyn != _vDynChann.end(); ++itDyn)
	{
		pDynChann = (CDynChannParam*)(*itDyn);
		_sDynFieldIns.append(pDynChann->GetCharValueField());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-3).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-4).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-5).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-6).c_str());
		_sDynFieldIns.append(",");
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-1).c_str());
		_sDynFieldSel+=strTmp;
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-7).c_str());
		_sDynFieldSel+=strTmp;

		pDynChann->SetWaveMemoryPoint(&(_pWaveDyn[index*iSmpNum]),iSmpNum);
		pDynChann->SetWaveMemoryPoint_real(&(_pWaveDyn_real[index*iSmpNum]),iSmpNum);

		++index;
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format(" %s ", ConsCommonField());
	_sDynFieldSel+=strTmp;


	//过程量数据，构造过程量数据表单
	V_PCHANNINFO::iterator it;
	_sProcField=" ";
	S_ChannInfo channInfo;
	for (it=_vProcChann.begin();it!=_vProcChann.end();++it)
	{
		channInfo=(*it)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);
	CString strLog;strLog.Format("%s-%s-%s Finish",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CNetOffPlantInfo","AddChannInfoEnd");
}

void CNetOffPlantInfo::AddChannInfoEnd(IDBInterFace * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的组成数据保存时，插入的SQL语句
	//振动数据
	int iSmpNum = _sPlantInfo._iSmpNum;
	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave      = new DATATYPE_WAVE[iNum];
	_pWave_real = new DATATYPE_WAVE[iNum];
	//memset( _pWave, 0, iNum*sizeof(DATATYPE_WAVE));
	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real  = new DATATYPE_WAVE[iNum];
	//memset( _pWaveDyn, 0, iNum*sizeof(DATATYPE_WAVE));
	CString strInfo;
	strInfo.Format("vibNum: %d,dynNum: %d, SampleNum: %d,%d", _sPlantInfo._iVibChannNum,_sPlantInfo._iDynChannNum, iSmpNum, iNum);
	CHZLogManage::Info(strInfo, "Online", "CRealPlantInfo::AddChannInfoEnd");
	//构造振动通道数据表单名字
	V_PCHANNINFO::iterator itVib;
	_sVibFieldSel= "";
	_sVibFieldIns= "";
	CString strTmp="",strTmp1="";
	CVibChannParam *pVibChann=NULL;
	int index=0;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
		pVibChann=(CVibChannParam*)(*itVib);
		_sVibFieldIns.append(pVibChann->GetCharValueField());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-3).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-4).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-5).c_str());
		_sVibFieldIns.append(",");
		_sVibFieldIns.append(pVibChann->GetSpecCharTypeField(-6).c_str());
		_sVibFieldIns.append(",");
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-1).c_str());
		_sVibFieldSel+=strTmp;
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-7).c_str());
		_sVibFieldSel+=strTmp;

		pVibChann->SetWaveMemoryPoint(&(_pWave[index*iSmpNum]),iSmpNum);
		pVibChann->SetWaveMemoryPoint_real(&(_pWave_real[index*iSmpNum]),iSmpNum);
	
		++index;
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sVibFieldIns += strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
	_sVibFieldSel += strTmp;
	///动态测点，构造动态数据表单
	V_PCHANNINFO::iterator itDyn;
	_sDynFieldIns= "";
	_sDynFieldSel= "";
	CDynChannParam *pDynChann=NULL;
	index = 0;
	for (itDyn = _vDynChann.begin(); itDyn != _vDynChann.end(); ++itDyn)
	{
		pDynChann = (CDynChannParam*)(*itDyn);
		_sDynFieldIns.append(pDynChann->GetCharValueField());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-3).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-4).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-5).c_str());
		_sDynFieldIns.append(",");
		_sDynFieldIns.append(pDynChann->GetSpecCharTypeField(-6).c_str());
		_sDynFieldIns.append(",");
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-1).c_str());
		_sDynFieldSel+=strTmp;
		strTmp.Format(" [%s],",pDynChann->GetSpecCharTypeField(-7).c_str());
		_sDynFieldSel+=strTmp;
		pDynChann->SetWaveMemoryPoint(&(_pWaveDyn[index*iSmpNum]),iSmpNum);
		pDynChann->SetWaveMemoryPoint_real(&(_pWaveDyn_real[index*iSmpNum]),iSmpNum);
		
		++index;
	}
	strTmp.Format(" %s,[%s]",
		ConsCommonField(),gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format(" %s ", ConsCommonField());
	_sDynFieldSel+=strTmp;


	//过程量数据，构造过程量数据表单
	V_PCHANNINFO::iterator it;
	_sProcField=" ";
	S_ChannInfo channInfo;
	for (it=_vProcChann.begin();it!=_vProcChann.end();++it)
	{
		channInfo=(*it)->GetChannInfo();
		strTmp.Format(" [%s],",channInfo._cChannNo);
		_sProcField.append(strTmp);
	}
	strTmp.Format("[%s],[%s],[%s],[%s],[%s],[%s]",gc_cTime,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5);
	_sProcField.append(strTmp);
    CString strLog;strLog.Format("%s-%s-%s Finish",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CNetOffPlantInfo","AddChannInfoEnd");
}

CBHDateTime CNetOffPlantInfo::GetCurrVibDataTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	_pLockVibData->BeginRead();
	dt=_timeCurrVib;
	_pLockVibData->EndRead();
	return dt;
}

CBHDateTime CNetOffPlantInfo::GetCurrDynDataTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	_pLockVibData->BeginRead();
	dt=_timeCurrDyn;
	_pLockVibData->EndRead();
	return dt;
}

CBHDateTime CNetOffPlantInfo::GetCurrProcDataTime()
{
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	_pLockVibData->BeginRead();
	dt=_timeCurrProc;
	_pLockVibData->EndRead();
	return dt;
}

void CNetOffPlantInfo::SetVibDataTime(CBHDateTime dataTime_)
{
	_pLockVibData->BeginWrite();
	_timeCurrVib=dataTime_;
	_pLockVibData->EndWrite();
    //BHTRACE_DEBUG(TimeConvertString(dataTime_));
}

void CNetOffPlantInfo::SetDynDataTime(CBHDateTime dataTime_)
{
	_pLockVibData->BeginWrite();
	_timeCurrDyn=dataTime_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetProcDataTime(CBHDateTime dataTime_)
{
	_pLockVibData->BeginWrite();
	_timeCurrProc=dataTime_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetVibDataStatu(IDBInterFace * pDB_,bool bSave_)
{
	if (bSave_)
		SaveVibNetOffData(pDB_);
}

void CNetOffPlantInfo::SetDynDataStatu(IDBInterFace * pDB_,bool bSave_)
{
	if (bSave_)
		SaveDynNetOffData(pDB_);
}

void CNetOffPlantInfo::SetProcDataStatu(IDBInterFace * pDB_,bool bSave_)
{
	if (bSave_)
		SaveProcNetOffData(pDB_);
}

void CNetOffPlantInfo::SaveVibNetOffData(IDBInterFace * pDB_)
{
	{
		int iCurrIDVibTrendSecond=0;
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());
		GetVibTrendSecondCurrID(iCurrIDVibTrendSecond);

		iCurrIDVibTrendSecond = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sVibTrendSecTblName,iCurrIDVibTrendSecond);
		
		SaveVibRecord(&loDb,_sVibTrendSecTblName,iCurrIDVibTrendSecond);
		
	}

	COleDateTimeSpan timeSpan;
	timeSpan=_timeCurrVib-_timeNetOffSaveVibHour;

	if((timeSpan.GetTotalSeconds())>=D_TREND_HOURINTERVAL)
	{
		int iCurrIDVibTrendHour=0;
		GetVibTrendHourCurrID(iCurrIDVibTrendHour);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDVibTrendHour = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sVibTrendHourTblName,iCurrIDVibTrendHour);

		SaveVibRecord(&loDb,_sVibTrendHourTblName,iCurrIDVibTrendHour);

		
		
		CString sTemp;sTemp.Format("NetOff:iCurrIDVibTrendHour:%d",iCurrIDVibTrendHour);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveVibHour=_timeCurrVib;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrVib-_timeNetOffSaveVibDay;

	if((timeSpan.GetTotalSeconds())>=D_TREND_DAYINTERVAL)
	{
		int iCurrIDVibTrendDay=0;
		GetVibTrendDayCurrID(iCurrIDVibTrendDay);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDVibTrendDay = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sVibTrendDayTblName,iCurrIDVibTrendDay);
		
		SaveVibRecord(&loDb,_sVibTrendDayTblName,iCurrIDVibTrendDay);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveVibDay=_timeCurrVib;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrVib-_timeNetOffSaveVibMonth;

	if((timeSpan.GetTotalMinutes())>=D_TREND_MONTHINTERVAL)
	{
		int iCurrIDVibTrendMonth=0;
		GetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
		COleSqlServerDB loDB;
		loDB.setPDB(pDB_->getPDB());

		iCurrIDVibTrendMonth = this->_pRealPlantInfo->ResetTrendTableCurID(&loDB,_sVibTrendMonthTblName,iCurrIDVibTrendMonth);
		SaveVibRecord(&loDB,_sVibTrendMonthTblName,iCurrIDVibTrendMonth,true);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveVibMonth=_timeCurrVib;
		_oPrivteDataLock.EndWrite();
	}
}

void CNetOffPlantInfo::SaveDynNetOffData(IDBInterFace * pDB_)
{

	{
		int iCurrIDDynTrendSecond=0;
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		GetDynTrendSecondCurrID(iCurrIDDynTrendSecond);

		iCurrIDDynTrendSecond = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb, _sDynTrendSecTblName, iCurrIDDynTrendSecond);

		SaveDynRecord(&loDb, _sDynTrendSecTblName, iCurrIDDynTrendSecond);

		

	}

	COleDateTimeSpan timeSpan;
	timeSpan=_timeCurrDyn-_timeNetOffSaveDynHour;
	if((timeSpan.GetTotalSeconds())>=D_TREND_HOURINTERVAL)
	{
		int iCurrIDDynTrendHour=0;
		GetDynTrendHourCurrID(iCurrIDDynTrendHour);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDDynTrendHour = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sDynTrendHourTblName,iCurrIDDynTrendHour);

		SaveDynRecord(&loDb,_sDynTrendHourTblName,iCurrIDDynTrendHour);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveDynHour=_timeCurrDyn;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrDyn-_timeNetOffSaveDynDay;
	if((timeSpan.GetTotalSeconds())>=D_TREND_DAYINTERVAL)
	{
		int iCurrIDDynTrendDay=0;
		GetDynTrendDayCurrID(iCurrIDDynTrendDay);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDDynTrendDay = 	this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sDynTrendDayTblName,iCurrIDDynTrendDay);

		SaveDynRecord(&loDb,_sDynTrendDayTblName,iCurrIDDynTrendDay);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveDynDay=_timeCurrDyn;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrDyn-_timeNetOffSaveDynMonth;
	if((timeSpan.GetTotalMinutes())>=D_TREND_MONTHINTERVAL)
	{
		int iCurrIDDynTrendMonth=0;
		GetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDDynTrendMonth = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sDynTrendMonthTblName,iCurrIDDynTrendMonth);

		SaveDynRecord(&loDb,_sDynTrendMonthTblName,iCurrIDDynTrendMonth,true);

		
		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveDynMonth=_timeCurrDyn;
		_oPrivteDataLock.EndWrite();
	}
}

void CNetOffPlantInfo::SaveProcNetOffData(IDBInterFace * pDB_)
{

	{
		int iCurrIDProcTrendSecond=0;
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		GetProcTrendSecondCurrID(iCurrIDProcTrendSecond);

		iCurrIDProcTrendSecond = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sProcTrendSecTblName,iCurrIDProcTrendSecond);

		SaveProcRecord(&loDb,_sProcTrendSecTblName,iCurrIDProcTrendSecond);


	}

	COleDateTimeSpan timeSpan;
	timeSpan=_timeCurrProc-_timeNetOffSaveProcHour;
	if((timeSpan.GetTotalSeconds())>=D_TREND_HOURINTERVAL)
	{
		int iCurrIDProcTrendHour=0;
		GetProcTrendHourCurrID(iCurrIDProcTrendHour);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDProcTrendHour = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sProcTrendHourTblName,iCurrIDProcTrendHour);

		SaveProcRecord(&loDb,_sProcTrendHourTblName,iCurrIDProcTrendHour);

		

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveProcHour=_timeCurrProc;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrProc-_timeNetOffSaveProcDay;
	if((timeSpan.GetTotalSeconds())>=D_TREND_DAYINTERVAL)
	{
		int iCurrIDProcTrendDay=0;
		GetProcTrendDayCurrID(iCurrIDProcTrendDay);
		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDProcTrendDay = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sProcTrendHourTblName,iCurrIDProcTrendDay);

		SaveProcRecord(&loDb,_sProcTrendDayTblName,iCurrIDProcTrendDay);

		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveProcDay=_timeCurrProc;
		_oPrivteDataLock.EndWrite();
	}

	timeSpan=_timeCurrProc-_timeNetOffSaveProcMonth;
	if((timeSpan.GetTotalMinutes())>=D_TREND_MONTHINTERVAL)
	{
		int iCurrIDProcTrendMonth=0;
		GetProcTrendMonthCurrID(iCurrIDProcTrendMonth);

		COleSqlServerDB loDb;
		loDb.setPDB(pDB_->getPDB());

		iCurrIDProcTrendMonth = this->_pRealPlantInfo->ResetTrendTableCurID(&loDb,_sProcTrendMonthTblName,iCurrIDProcTrendMonth);


		SaveProcRecord(&loDb,_sProcTrendMonthTblName,iCurrIDProcTrendMonth,true);

		
		_oPrivteDataLock.BeginWrite();
		_timeNetOffSaveProcMonth=_timeCurrProc;
		_oPrivteDataLock.EndWrite();
	}
}
volatile ULONGLONG gnSaveCount = 0;
void CNetOffPlantInfo::SaveVibRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if ((GetPlantInfo()._iMachineType) == GE_MACHINETYPE_RC ||(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_DISEL||(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_ICE)
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
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
	CBHDateTime dtCurr = GetCurrVibDataTime();
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

	BOOL lbMonth = bMonth_;

	int lnUpdateMonthTable = CConfigHelper::GetMonthTableUpdateFlag();

	if(g_bIsDiskFull && ((!lbMonth)||(lbMonth && lnUpdateMonthTable > 0)))
	{
		int lnTotalWavePtNum = 0;

		_pLockVibData->BeginRead();
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			pVibChannParam=(CVibChannParam*)(*it);	

			lnTotalWavePtNum += pVibChannParam->GetCharValue()._iSmpNum;
		}
		_pLockVibData->EndRead();
		MakeSureVibSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,0);
	}

	if(g_bIsDiskFull &&  lnUpdateMonthTable> 0)
	{
		lbMonth = false;
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT %s  FROM [%s] ",_sVibFieldSel.c_str(),sTableName_.c_str());
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

					//memcpy(pBuf,pVibChannParam->GetCurrWavePoint(),iSaveWaveNum*sizeof(DATATYPE_WAVE));
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

				lstrData.Format(_T("Netoff Save Failed Update 操作失败,id 为 :%d\r"),iCurID_);

				CHZLogManage::Error(lstrData,"CNetOffPlantInfo","SaveVibRecord");

			}else
			{
				gnSaveCount ++;
			}		
			
			return;
		}
	}

	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sVibFieldIns.c_str());
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

			lstrData.Format(_T("Netoff Setting Error , 参数不正确，采样点数为 0 ，采样点 ID:%s  No:%s\r") , loInfo._cChannID, loInfo._cChannNo );

			CHZLogManage::Error(lstrData,"CNetOffPlantInfo","SaveVibRecord");

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
	gnSaveCount ++;
/*

	ASSERT(pDB_->GetSql().GetLength()<4096);

	if (pDB_->GetSql().GetLength()>=4096)
	{
	
		return ;
	}*/

	
	if(!pDB_->Exec())
	{

		CString lstrData;

		lstrData.Format(_T("Netoff Save Failed 插入失败:%d\r\n"),iCurID_);

		CHZLogManage::Error(lstrData,"CNetOffPlantInfo","SaveVibRecord");

		return;
	}


	pDB_->Cmd("SELECT %s FROM [%s]",_sVibFieldSel.c_str(),sTableName_.c_str());
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
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveVibRecord");
	}
	return;
}

void CNetOffPlantInfo::SaveDynRecord(IDBInterFace * pDB_,const string &sTableName_,const int &iCurID_,const bool &bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int index=0;
	int iSaveWaveNum=GD_MIDDLEWARE_WAVE_POINT;
	if ((GetPlantInfo()._iMachineType) == GE_MACHINETYPE_RC || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_DISEL || (GetPlantInfo()._iMachineType) == GE_MACHINETYPE_ICE)
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_RC;
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
	CBHDateTime dtCurr = GetCurrDynDataTime();
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
	BOOL lbMonth = bMonth_;

	int lnUpdateMonthTable = CConfigHelper::GetMonthTableUpdateFlag();

	if(g_bIsDiskFull && ((!lbMonth)||(lbMonth && lnUpdateMonthTable > 0)))
	{
		int lnTotalWavePtNum = 0;

		_pLockDynData->BeginRead();
		for (it=_vDynChann.begin();it!=_vDynChann.end();++it)
		{
			pVibChannParam =(CDynChannParam*)(*it);	

			lnTotalWavePtNum += pVibChannParam->GetCharValue()._iSmpNum;
		}
		_pLockDynData->EndRead();

		MakeSureDynSaveSpaceSufficient(pDB_,sTableName_,lnTotalWavePtNum,iCurID_,0);
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT %s FROM [%s] ",_sDynFieldSel.c_str(),sTableName_.c_str());
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
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sDynFieldIns.c_str());
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
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveDynRecord");
		return;
	}

	pDB_->Cmd("SELECT %s FROM [%s]", _sDynFieldSel.c_str(), sTableName_.c_str());
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
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveDynRecord");
	}
	return;
}
void CNetOffPlantInfo::SaveProcRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_/* =false */)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	float pfRev[5];
	float fData=.0;
	CString strTimeCurr;
	GetRealRev( pfRev, 5);

	//得到断网数据当前的时间
	CBHDateTime dtCurr = GetCurrProcDataTime();
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
	BOOL lbMonth = bMonth_;

	if(g_bIsDiskFull && CConfigHelper::GetMonthTableUpdateFlag() > 0)
	{
		lbMonth = false;
	}

	if (!lbMonth)
	{
		pDB_->Cmd("SELECT * FROM [%s]",sTableName_.c_str());
		pDB_->Cmd(" WHERE [%s]=%d",gc_cID,iCurID_);
		if(pDB_->More())
		{
			pDB_->Cmd("UPDATE [%s] SET  ",sTableName_.c_str());
			for (V_PCHANNINFO::iterator result =_vProcChann.begin();result!=_vProcChann.end();result++)
			{
				pProcChann=(CProcChannParam*)(*result);
				_pLockProcData->BeginRead();
				fData=pProcChann->GetData();
				_pLockProcData->EndRead();
				pDB_->Cmd(" [%s]=%.2f,",(*result)->GetChannInfo()._cChannNo,fData);
			}
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev,pfRev[0]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev2,pfRev[1]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev3,pfRev[2]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev4,pfRev[3]);
			pDB_->Cmd(" [%s]=%.2f, ",gc_cRev5,pfRev[4]);
			pDB_->Cmd(" [%s]='%s' ",gc_cTime,strTimeCurr);
			pDB_->Cmd("  WHERE [%s]=%d",gc_cID,iCurID_);
			//		TRACE("proc trend save:%s\n ",pDB_->GetSql());
			if(pDB_->Exec())
				return;
		}
	}
	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s) ",sTableName_.c_str(),gc_cID,_sProcField.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	for (V_PCHANNINFO::iterator result=_vProcChann.begin();result!=_vProcChann.end();result++)
	{
		_pLockProcData->BeginRead();
		try
		{
			fData=((CProcChannParam*)(*result))->GetData();
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
	pDB_->Cmd("'%s',",strTimeCurr);
	pDB_->Cmd(" %.2f,",pfRev[0]);
	pDB_->Cmd(" %.2f,",pfRev[1]);
	pDB_->Cmd(" %.2f,",pfRev[2]);
	pDB_->Cmd(" %.2f,",pfRev[3]);
	pDB_->Cmd(" %.2f)",pfRev[4]);
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
	}
}


void CNetOffPlantInfo::SetIDPointer(CRealPlantInfo *pRealPlantInfo_)
{
	ASSERT(NULL == this->_pRealPlantInfo);
	this->_pLockChannInfo->BeginWrite();
	_pRealPlantInfo = pRealPlantInfo_;
	this->_pLockChannInfo->EndWrite();
}

void CNetOffPlantInfo::SetVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDVibTrendSecond=iCurrIDVibTrendSecond_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetVibTrendHourCurrID(const int &iCurrIDVibTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDVibTrendHour=iCurrIDVibTrendHour_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetVibTrendDayCurrID(const int &iCurrIDVibTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDVibTrendDay=iCurrIDVibTrendDay_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDVibTrendMonth=iCurrIDVibTrendMonth_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDProcTrendSecond=iCurrIDProcTrendSecond_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetProcTrendHourCurrID(const int &iCurrIDProcTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDProcTrendHour=iCurrIDProcTrendHour_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetProcTrendDayCurrID(const int &iCurrIDProcTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDProcTrendDay=iCurrIDProcTrendDay_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDProcTrendMonth=iCurrIDProcTrendMonth_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDDynTrendSecond=iCurrIDDynTrendSecond_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetDynTrendHourCurrID(const int &iCurrIDDynTrendHour_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDDynTrendHour=iCurrIDDynTrendHour_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetDynTrendDayCurrID(const int &iCurrIDDynTrendDay_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDDynTrendDay=iCurrIDDynTrendDay_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::SetDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_)
{
	if (NULL == this->_pRealPlantInfo)
	{
		ASSERT(FALSE);
		return;
	}
	_pLockVibData->BeginWrite();
	_pRealPlantInfo->_iCurrIDDynTrendMonth=iCurrIDDynTrendMonth_;
	_pLockVibData->EndWrite();
}

void CNetOffPlantInfo::GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_)
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

void CNetOffPlantInfo::GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_)
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

void CNetOffPlantInfo::GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_)
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

void CNetOffPlantInfo::GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_)
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

void CNetOffPlantInfo::GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_)
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

void CNetOffPlantInfo::GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_)
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

void CNetOffPlantInfo::GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_)
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

void CNetOffPlantInfo::GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_)
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

void CNetOffPlantInfo::GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_)
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

void CNetOffPlantInfo::GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_)
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

void CNetOffPlantInfo::GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_)
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

void CNetOffPlantInfo::GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_)
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


int CNetOffPlantInfo::SaveFastTrendRecord( IDBInterFace *pDB_,E_TBL_CHANNTYPE eChannType_)
{
	int iRtn = -1;
	
	switch(eChannType_)
	{
	case E_TBL_CHANNTYPE_VIB:
		iRtn = SaveVibFastTrend(pDB_);
		break;
	case E_TBL_CHANNTYPE_DYN:
		iRtn = SaveDynFastTrend(pDB_);
		break;
	case E_TBL_CHANNTYPE_PROC:
		iRtn = SaveProcFastTrend(pDB_);
		break;
	default:
		break;
	}

	return iRtn;
}
int CNetOffPlantInfo::SaveVibFastTrend(IDBInterFace * pDB_)
{
	{
		int iCurrIDVibTrendSecond=0;
		GetVibTrendSecondCurrID(iCurrIDVibTrendSecond);
		SaveVibRecord(pDB_,_sVibTrendSecTblName,iCurrIDVibTrendSecond);
	}


	{
		int iCurrIDVibTrendHour=0;
		GetVibTrendHourCurrID(iCurrIDVibTrendHour);
		SaveVibRecord(pDB_,_sVibTrendHourTblName,iCurrIDVibTrendHour);

	}

	
	{
		int iCurrIDVibTrendDay=0;
		GetVibTrendDayCurrID(iCurrIDVibTrendDay);
		SaveVibRecord(pDB_,_sVibTrendDayTblName,iCurrIDVibTrendDay);

		
	}

	
	{
		int iCurrIDVibTrendMonth=0;
		GetVibTrendMonthCurrID(iCurrIDVibTrendMonth);
		SaveVibRecord(pDB_,_sVibTrendMonthTblName,iCurrIDVibTrendMonth,true);

	}
	return 1;
}

int CNetOffPlantInfo::SaveDynFastTrend(IDBInterFace * pDB_)
{
	{
		int iCurrIDDynTrendSecond=0;
		GetDynTrendSecondCurrID(iCurrIDDynTrendSecond);

		SaveDynRecord(pDB_, _sDynTrendSecTblName, iCurrIDDynTrendSecond);

	}

	
	{
		int iCurrIDDynTrendHour=0;
		GetDynTrendHourCurrID(iCurrIDDynTrendHour);
		SaveDynRecord(pDB_,_sDynTrendHourTblName,iCurrIDDynTrendHour);

		
	}

	
	{
		int iCurrIDDynTrendDay=0;
		GetDynTrendDayCurrID(iCurrIDDynTrendDay);
		SaveDynRecord(pDB_,_sDynTrendDayTblName,iCurrIDDynTrendDay);

	}

	
	{
		int iCurrIDDynTrendMonth=0;
		GetDynTrendMonthCurrID(iCurrIDDynTrendMonth);
		SaveDynRecord(pDB_,_sDynTrendMonthTblName,iCurrIDDynTrendMonth,true);
	}
	return 1;
}

int CNetOffPlantInfo::SaveProcFastTrend(IDBInterFace * pDB_)
{

	{
		int iCurrIDProcTrendSecond=0;
		GetProcTrendSecondCurrID(iCurrIDProcTrendSecond);

		SaveProcRecord(pDB_,_sProcTrendSecTblName,iCurrIDProcTrendSecond);
	}

	{
		int iCurrIDProcTrendHour=0;
		GetProcTrendHourCurrID(iCurrIDProcTrendHour);
		SaveProcRecord(pDB_,_sProcTrendHourTblName,iCurrIDProcTrendHour);

	}

	{
		int iCurrIDProcTrendDay=0;
		GetProcTrendDayCurrID(iCurrIDProcTrendDay);
		SaveProcRecord(pDB_,_sProcTrendDayTblName,iCurrIDProcTrendDay);

	}

	{
		int iCurrIDProcTrendMonth=0;
		GetProcTrendMonthCurrID(iCurrIDProcTrendMonth);
		SaveProcRecord(pDB_,_sProcTrendMonthTblName,iCurrIDProcTrendMonth,true);

	}
	return 1;
}


