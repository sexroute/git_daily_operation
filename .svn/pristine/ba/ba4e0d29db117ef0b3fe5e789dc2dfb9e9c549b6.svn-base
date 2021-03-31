#include "StdAfx.h"
#include "speedupdowndatasave.h"
#include <additions/DebugHelper.h>




CSudPlantInfo::CSudPlantInfo()
{
   CHZLogManage::Info("construction","CSudPlantInfo","CSudPlantInfo");
	_iEventID=0;
	_iCurrEventID=0;
	_bProcDataUpdateSend = false;
    _bSudFinishStatus = true;
	_sVibRevField.clear();
	_sVibRevField.clear();
	_sProcRevField.clear();
}

CSudPlantInfo::~CSudPlantInfo()
{
	Destory();
   
}

void CSudPlantInfo::Destory()
{
    KD_DeletePoint(_pWave);
}

float  CSudPlantInfo::GetRevMarkInterval()
{
	float lfRevMarkInterval = 30.0;
	_DECLARE_PERF_MEASURE_TIME()
	_BEGIN_PERF_MEASURE_TIME();
	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		int lnInterval = ::GetPrivateProfileInt("SYSTEM","G_REV_INTERVAL",30,cAppLocation);

		lfRevMarkInterval = lnInterval;



	}catch(...)
	{
	}
	_END_PERF_MEASURE_TIME("ReadConfigFile");
	return lfRevMarkInterval;
}

int CSudPlantInfo::GetSpecChannRadialAllChar(string sChann_,S_VibCharValue &sVibData_,string &sUnit_)
{
	int index=-1;
	V_PCHANNINFO::iterator it;
	S_ChannInfo channInfo;
	for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
	{
		++index;
		channInfo=(*it)->GetChannInfo();
		if (strcmp(channInfo._cChannID,sChann_.c_str())==0)
		{
			_pLockVibData->BeginWrite();
			((CVibChannParam*)(*it))->ComputeEigenValue(GetPlantInfo()._iMachineType,index);
			_pLockVibData->EndWrite();


			_pLockVibData->BeginRead();			
			sVibData_=((CVibChannParam*)(*it))->GetCharValueEx();
			_pLockVibData->EndRead();
			sUnit_=channInfo._cUnit;
			return index;
		}
	}
	return -1;
}


int CSudPlantInfo::GetSpecChannRadialAllChar(int index_,S_VibCharValue &sVibData_,string &sUnit_)
{
	//	if((index_>-1)&&(index_<_iRadialVibChannNum))//2007.4.10修改，by Marble
	if((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
	{
		_pLockVibData->BeginWrite();
		((CVibChannParam*)_vVibChann[index_])->ComputeEigenValue(GetPlantInfo()._iMachineType,index_);
		_pLockVibData->EndWrite();
	
		_pLockVibData->BeginRead();		
		sVibData_=((CVibChannParam*)_vVibChann[index_])->GetCharValueEx();
		_pLockVibData->EndRead();
		sUnit_=_vVibChann[index_]->GetChannInfo()._cUnit;
	}
	else
		index_=-1;
	return index_;
}


void CSudPlantInfo::SaveData(IDBInterFace *pDB_)
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	string sTime;
	int iEventID = -1;
	int iMilliSecond = -1;
	_pLockVibData->BeginRead();
	BEGIN_ERROR_HANDLE
	 sTime=TimeConvertString(_timeCurr);
	 iEventID = _iCurrEventID;
	 iMilliSecond = _iMicroSecond;
	END_ERROR_HANDLE
	_pLockVibData->EndRead();


	pDB_->Cmd("select top 1 %s from %s where %s = %d and %s = '%s' and %s = %d",\
		gc_cTime,_sSudDataTblName.c_str(),gc_cEventID,iEventID,gc_cTime,sTime.c_str(),gc_cMicroSecond,iMilliSecond);
	if(pDB_->More())
	{
		return;
	}

	if(g_bIsDiskFull)
	{

		V_PCHANNINFO::iterator it=_vVibChann.begin();
		int lnTotalWavePtNum = 0;
		_pLockVibData->BeginRead();
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			lnTotalWavePtNum += ((CVibChannParam*)(*it))->GetCharValueEx()._iSmpNum;
		}
		_pLockVibData->EndRead();

		vector<CString> lvChannNo;

		for(int n = 0; n < _vVibChann.size(); n++)
		{
			lvChannNo.push_back(_vVibChann[n]->GetChannInfo()._cChannNo);
		}

		this->DeleteSudVibRecord(pDB_,iEventID,lvChannNo,lnTotalWavePtNum);
		
	}


    pDB_->Cmd("INSERT INTO [%s](%s,%s,%s) ",_sSudDataTblName.c_str(),gc_cEventID,gc_cSynFlag,_sFieldIns.c_str());
	pDB_->Cmd("VALUES (%d,0,",iEventID);
	V_PCHANNINFO::iterator it=_vVibChann.begin();
	S_VibCharValue vibCharValue;
	CVibChannParam *pVibChann=NULL;
	
	float pfRev[5];
	int iSmpNum = 0, iSmpFreq = 0;
	GetRealRev(pfRev, 5);
	GetRealSmpInfo( iSmpNum, iSmpFreq ,true);
	
	
	_pLockVibData->BeginRead();
	try
	{
		for (;it!=_vVibChann.end();++it)
		{
			pVibChann=(CVibChannParam*)(*it);
			vibCharValue=((CVibChannParam*)(*it))->GetCharValueEx();
			pDB_->Cmd("%.2f,",vibCharValue._fCharAll);
	        pDB_->Cmd("%.2f,",vibCharValue._fCharOne);
			pDB_->Cmd("%.2f,",vibCharValue._fCharTwo);
			pDB_->Cmd("%.2f,",vibCharValue._fCharThree);
			pDB_->Cmd("%.2f,",vibCharValue._fCharHalf);
			pDB_->Cmd("%.2f,",vibCharValue._fCharRes);
			pDB_->Cmd("%.2f,",vibCharValue._fDC);
			pDB_->Cmd("%.2f,",vibCharValue._fGap);
			pDB_->Cmd("%.2f,",vibCharValue._fPhaseOne);
			pDB_->Cmd("%.2f,",vibCharValue._fPhaseTwo);
			pDB_->Cmd("%.2f,",vibCharValue._fPhaseThree);
			pDB_->Cmd("%.2f,",vibCharValue._fPhaseHalf);
			pDB_->Cmd("%d,",vibCharValue._iSmpFreq);
			pDB_->Cmd("%d,",vibCharValue._iSmpNum);
			pDB_->Cmd("%d,",vibCharValue._iRev);
			pDB_->Cmd("'%s',",pVibChann->GetChannInfo()._cTempID);
		}
		pDB_->Cmd("%d,",iSmpNum);
		pDB_->Cmd("%d,",iSmpFreq);
		for (int nloop=0;nloop<5;++nloop)
		{
		  pDB_->Cmd("%.2f,",pfRev[nloop]);
		}
		pDB_->Cmd("'%s',",sTime.c_str());
		pDB_->Cmd("%d)",iMilliSecond);
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
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("保存启停车数据失败（插入状态下）","CSudPlantInfo","SaveData");
		return;
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sSudDataTblName.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sSudDataTblName.c_str());
		pDB_->Cmd(" WHERE %s = %d AND %s=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cMicroSecond,iMilliSecond,gc_cTime,sTime.c_str());
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}

	pDB_->Cmd("SELECT %s FROM [%s]",_sFieldSel.c_str(),_sSudDataTblName.c_str());
	_pLockVibData->BeginRead();
	try
	{
		if(lnID > 0)
		{
			pDB_->Cmd(" WHERE %s = %d ",gc_cID,lnID);

		}
		else
		{
			pDB_->Cmd(" WHERE %s = %d AND %s=%d AND [%s]='%s'",gc_cEventID,iEventID,gc_cMicroSecond,iMilliSecond,gc_cTime,sTime.c_str());

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
	_pLockVibData->EndRead();
	
	if(pDB_->More(TRUE))
	{   
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
		{
			pVibChann=(CVibChannParam*)(*it);
			_pLockVibData->BeginRead();	
			try
			{
				int iSmpNums=pVibChann->GetCharValue()._iSmpNum;
				vector<BYTE> lvWave;
				CWaveHead lsHead = pVibChann->WaveHead();
				WaveAddHead(pVibChann->GetCurrWavePoint(),iSmpNums,lsHead,lvWave);

				pDB_->SetBuffer(pVibChann->GetSpecCharTypeField(-1).c_str(),(BYTE *)(&lvWave.front()),(int)lvWave.size());
				memcpy(pfCustomData,pVibChann->GetCustomParamData(), sizeof(float)*GD_CUSTOMPARAM_BUFFSIZE);
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
			pDB_->SetBuffer(pVibChann->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		}
		KD_DeletePoint(pfCustomData);
	}
	else
	{
        CHZLogManage::Error("保存启停车数据时，读取记录失败（更新波形状态下）","CSudPlantInfo","SaveData");
		return;
	}
}

int CSudPlantInfo::GetSpecChannIndex(string sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_/* =GE_ALLCHAR */)
{
   int iCharType=0;
   int iPhaseType=0;
   switch(iCharType_)
   {
   case GE_ALLCHAR://因为目前没有计算通频相位，所以用一倍频替代
	   iCharType=GE_ALLCHAR;
	   iPhaseType=GE_PHASECHARONE;
	   break;
   case GE_ONECHAR:
   case GE_PHASECHARONE:
	   iCharType=GE_ONECHAR;
	   iPhaseType=GE_PHASECHARONE;
	   break;
   case GE_TWOCHAR:
   case GE_PHASECHARTWO:
	   iCharType=GE_TWOCHAR;
	   iPhaseType=GE_PHASECHARTWO;
	   break;
   case GE_THREECHAR:
   case GE_PHASECHARTHREE:
	   iCharType=GE_THREECHAR;
	   iPhaseType=GE_PHASECHARTHREE;
	   break;
   case GE_HALFCHAR:
   case GE_PHASECHARHALF:
	   iCharType=GE_HALFCHAR;
	   iPhaseType=GE_PHASECHARHALF;
	   break;
   default://缺省情况下，就是用一倍频的幅值和相位
	   iCharType=GE_ONECHAR;
	   iPhaseType=GE_PHASECHARONE;
   }
   int index=GetSpecVibChannIndex(sChannID_);
   if (index>-1)
   {
	   CVibChannParam *vibChannParam=(CVibChannParam*)_vVibChann[index];
	   S_ChannInfo channInfo=vibChannParam->GetChannInfo();
	   _pLockVibData->BeginRead();
	   try
	   {
		   fAmp_=vibChannParam->GetCharValueEx().GetSpecTypeData(iCharType);
		   fPhase_=vibChannParam->GetCharValueEx().GetSpecTypeData(iPhaseType);
		   iSmpFreq_=vibChannParam->GetCharValue()._iSmpFreq;
		   iSmpNum_=vibChannParam->GetCharValue()._iSmpNum;
		   int iRev=vibChannParam->GetCharValue()._iRev;
		   fRev_ = (float)iRev;
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
  }
   return index;
}

int CSudPlantInfo::GetSpecChannWave(string sChannID_,DATATYPE_WAVE *pfWave_,int &ilen_)
{
	int iSensorType=0;
	int index=GetSpecVibChannIndex(sChannID_);
	if (index==-1){   
		return -1;
	}
    int iSmpNum = 1, iSmpFreq = 1;
	iSmpNum=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
	//判断客户指定波形数据是否能满足,如果不满足,则就按照实际数据发送过去
	if(ilen_> iSmpNum) 
		ilen_= iSmpNum;
	//得到客户需要显示的波形数据
	_pLockVibData->BeginRead();
	try
	{
		memcpy(pfWave_,((CVibChannParam*)_vVibChann[index])->GetCurrWavePoint(),ilen_*sizeof(DATATYPE_WAVE));
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

int CSudPlantInfo::GetSpecChannIndex(int index_,string &sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_/* =GE_ALLCHAR */)
{
	if ((index_>-1)&&(index_< _sPlantInfo._iVibChannNum))
		return -1;
	S_ChannInfo channInfo=_vVibChann[index_]->GetChannInfo();
	sChannID_=channInfo._cChannID;
    return GetSpecChannIndex(sChannID_,fRev_,fPhase_,fAmp_,iSmpNum_,iSmpFreq_,iCharType_);
}

int CSudPlantInfo::GetSpecChannVibWaveAndFreq(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float &fScaleCoef_,int& nPrecisionEnabled_,float *pWave_,float *pFreq_)
{
	int iSensorType=0;
	int index=GetSpecVibChannIndex(sChann_);
	if (index==-1){   
		return -1;
	}
	S_ChannInfo channInfo=_vVibChann[index]->GetChannInfo();
	iSensorType=channInfo._iSensorType;
	float *pfWave=NULL;
	float *pfSpec=NULL;
	//得到客户指定分析线数需要波形数据个数
	iLines_=iLines_<2?16:iLines_;
	int iLinesPt=iLines_*2.56;
/*	_pLockVibData->BeginRead();
	//判断客户指定波形数据是否能满足,如果不满足,则就按照实际数据发送过去
	iPtNum_=iPtNum_>_iRealSmpNum?_iRealSmpNum:iPtNum_;
	//判断实际数据是否满足谱分析的波形数据个数
	iLinesPt=iLinesPt>_iRealSmpNum?_iRealSmpNum:iLinesPt;
	iSmpFreq=_iRealSmpFreq;
	pfWave=new float[iLinesPt];
	pfSpec=new float[iLinesPt];
	//得到谱分析需要的波形数据
	ConvertDTWaveToFloat(pfWave,_vVibChann[index].GetCurrWavePoint(),iLinesPt);
//	writefile((DATATYPE_WAVE*)_vVibChann[index].GetCurrWavePoint(),iLinesPt,19);
//	writefile(pfWave,iLinesPt,20);
	//得到客户需要显示的波形数据
	memcpy(pfData_,_vVibChann[index].GetCurrWavePoint(),iPtNum_*sizeof(DATATYPE_WAVE));
	_pLockVibData->EndRead();

	//	CString sTmp;sTmp.Format("%d,%d,%d",_iRealPointNum,ag_iPtNum,iLinesPt);AfxMessageBox(sTmp);
	iLines_=iLinesPt/2.56;
	if(iLines_>1)
	{
		fDf_=(float)iSmpFreq/iLinesPt;
		iLines_+=1;
		if(iSensorType==GE_SENSORTYPE_VOTEX){//峰峰值谱
			SpectralMeasureMent(pfWave,iLinesPt,pfSpec,EA_PEAK_PEAK_SPEC);	
		}
		else{//峰值谱
			SpectralMeasureMent(pfWave,iLinesPt,pfSpec,EA_PEAK_SPEC);
		}
//		writefile(pfSpec,iLinesPt,11);
		ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_);
	}
*/
	{
		int iSmpFreq = 0;
		int iSmpNums = 0;
		int iRtn;
		iSmpNums=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpNum;
		iSmpFreq=((CVibChannParam*)_vVibChann[index])->GetCharValue()._iSmpFreq;
		iPtNum_ = iPtNum_>iSmpNums ? iSmpNums:iPtNum_;
		
		float fStartFreq=0;
		pfWave=new float[iSmpNums];
		pfSpec=new float[(iLines_*1.28)+10];//加10是为了分配的内存不够
		CVibChannParam *pVibChann=(CVibChannParam*)(_vVibChann[index]);
		CWaveHead lsWaveHead;
		//得到客户需要的波形数据和谱分析需要的波形数据
		_pLockVibData->BeginRead();
		try
		{
			lsWaveHead = pVibChann->WaveHead();
			ConvertDTWaveToFloat(pfWave,pVibChann->GetCurrWavePoint(),iSmpNums,lsWaveHead);
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

		ConvertFloatToDTWave(pfData_,pfWave,iSmpNums,lsWaveHead);

		if(lsWaveHead.GetSize() <= 0)
		{
			fScaleCoef_ = 10.0;
			nPrecisionEnabled_ = 0;
		}
		else
		{
			fScaleCoef_ = lsWaveHead.m_pParam[0];
			if(lsWaveHead.GetSize() > 1)
			{
				nPrecisionEnabled_ = lsWaveHead.m_pParam[1];
			}
			else
			{
				nPrecisionEnabled_ = 0;
			}
		}

		iRtn=ZoomSpectrumAnalysis(pfWave,iSmpNums,iSmpFreq,pfSpec,iLines_,fDf_,fStartFreq);
		if (iRtn<0)
			iLines_=0;
		else
		{
			if(iRtn==2)
			{//正常峰值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2);
					ScaleWave(pFreq_,pfSpec,iLines_,2);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1);
					ScaleWave(pFreq_,pfSpec,iLines_,1);
				}
			}
			else
			{//细化谱,细化谱为有效值谱
				if(iSensorType==GE_SENSORTYPE_VOTEX)//峰峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,2.828);
					ScaleWave(pFreq_,pfSpec,iLines_,2.828);
				}
				else//峰值谱
				{
					ConvertFloatToDTWave(pfFreqData_,pfSpec,iLines_,lsWaveHead,1.414);
					ScaleWave(pFreq_,pfSpec,iLines_,1.414);
				}
			}
		}
	}
   if(NULL!=pfWave)	delete [] pfWave;
   if(NULL!=pfSpec)	 delete [] pfSpec;
   return index;
}

void CSudPlantInfo::SetDataStatu(IDBInterFace * pDB_,bool bFlag_,bool bSave_,long iEventID_,E_SUD_TYPE iSudType_)
{
	SetSudStatus( bFlag_, iSudType_);
	_pLockVibData->BeginWrite();
	_iCurrEventID=iEventID_;
	_pLockVibData->EndWrite();

	if (bSave_)
	{
		if (!G_TEMP_ONLINE)
		{
			SaveData(pDB_);
		}	
	}
}

void CSudPlantInfo::InitCurrID(CCustomDB * pDb_)
{
	SwichDbName(pDb_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	pDb_->Cmd("SELECT max([%s]) FROM [%s] ",gc_cEventID,_sSudLogTblName.c_str());
	//  _pDB->Cmd(" ORDER BY [%s] DESC",gc_cStart_T);
	if (pDb_->More())
		_iEventID=pDb_->GetLong(gc_cEventID);
	else
		_iEventID=0;
}

void CSudPlantInfo::AddVibChannInfo(IDBInterFace *pDB_)
{
	if(!G_TEMP_ONLINE)
	{
		SwitchDbNameEx(pDB_,g_strMainDbName);
		int index=0;
		S_ChannInfo sChannInfo;
		CVibChannParam *pvibParam=NULL;
		//设置数据处理连接

		_vVibChann.clear();
		_mapVibChannIndex.clear();
		//径向振动通道
		pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND %s =%d AND %s = %d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN,gc_cSensorType,GE_SENSORTYPE_VOTEX);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			pvibParam=new CVibChannParam;
			pvibParam->SetRwLock(_pLockChannInfo);
			sChannInfo.GetChannInfo(pDB_);
			pvibParam->SetChannInfo(sChannInfo);
			_vVibChann.push_back(pvibParam);
			_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
			++index;
		}
	}
	else
	{
		CPlantInfoOnLine::AddVibChannInfo(pDB_);
	}
}

void CSudPlantInfo::AddVibChannInfo(CCustomDB *pDB_)
{
	if(!G_TEMP_ONLINE)
	{
		SwichDbName(pDB_,g_strMainDbName);
		int index=0;
		S_ChannInfo sChannInfo;
		CVibChannParam *pvibParam=NULL;
		//设置数据处理连接

		_vVibChann.clear();
		_mapVibChannIndex.clear();
		//径向振动通道
		pDB_->Cmd("SELECT * from [%s] ",gc_cChannTable);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND %s =%d AND %s = %d",gc_cCompany,_sCompany.c_str(),gc_cPlantNo,_sPlantInfo._cPlantNo,gc_cChannType,GE_VIBCHANN,gc_cSensorType,GE_SENSORTYPE_VOTEX);
		pDB_->Cmd(" ORDER BY [%s] ",gc_cID);
		while (pDB_->More())
		{
			pvibParam=new CVibChannParam;
			pvibParam->SetRwLock(_pLockChannInfo);
			sChannInfo.GetChannInfo(pDB_);
			pvibParam->SetChannInfo(sChannInfo);
			_vVibChann.push_back(pvibParam);
			_mapVibChannIndex.insert(make_pair(sChannInfo._cChannID,index));
			++index;
		}
	}
	else
	{
		CPlantInfoOnLine::AddVibChannInfo(pDB_);
	}
}

void CSudPlantInfo::AddChannInfoEnd()
{
	CString strLog="";
	_sPlantInfo._iVibChannNum=_vVibChann.size();
	_sPlantInfo._iProcChannNum=0;
	_iAxialVibChannNum=0;
	_iRadialVibChannNum=_sPlantInfo._iVibChannNum;
	
	int iSmpNum= _sPlantInfo._iSmpNum;
	int iNum=_iRadialVibChannNum* iSmpNum;
	int index=0;
	_pWave=new DATATYPE_WAVE[iNum];
	_pWave_real =new DATATYPE_WAVE[iNum];
	//构造振动通道数据表单名字
	V_PCHANNINFO::iterator itVib;
	_sFieldSel="";
	_sFieldIns="";
	CString strTmp="",strTmp1="";
	CVibChannParam *pVibChann=NULL;
	for (itVib=_vVibChann.begin();itVib!=_vVibChann.end();++itVib)
	{
        pVibChann=(CVibChannParam*)(*itVib);
		_sFieldIns.append(pVibChann->GetCharValueField());
		_sFieldIns.append(",");
		_sFieldIns.append(pVibChann->GetSpecCharTypeField(-3));
		_sFieldIns.append(",");
		_sFieldIns.append(pVibChann->GetSpecCharTypeField(-4));
		_sFieldIns.append(",");
		_sFieldIns.append(pVibChann->GetSpecCharTypeField(-5));
		_sFieldIns.append(",");
		_sFieldIns.append(pVibChann->GetSpecCharTypeField(-6));
		_sFieldIns.append(",");

		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-1).c_str());
		_sFieldSel+=strTmp;
		strTmp.Format(" [%s],",pVibChann->GetSpecCharTypeField(-7).c_str());
		_sFieldSel+=strTmp;
		pVibChann->SetWaveMemoryPoint(&(_pWave[index*iSmpNum]),iSmpNum);
		pVibChann->SetWaveMemoryPoint_real(&(_pWave_real[index*iSmpNum]),iSmpNum);

		++index;
	}
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s] ,[%s],[%s],[%s] ",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime,gc_cMicroSecond);
	_sFieldIns+=strTmp;
	_sFieldSel.append("1");
//	InitCurrID();
	_iCurrEventID=-1;
	_bSudStatus=false;
    strLog.Format("%s-%s-%s Finish",_sCompany.c_str(),_sFactory.c_str(),_sPlantInfo._cPlantID);
	CHZLogManage::Info(strLog,"CSudPlantInfo","AddChannInfoEnd");
}

void CSudPlantInfo::AddChannInfoEnd(IDBInterFace * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的
	//组成数据保存时，插入的SQL语句
	//振动数据
	/*
	int iSmpNum = _sPlantInfo._iSmpNum;
	*/
	int iSmpNum = _sPlantInfo._iSmpNum;

	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave			= new DATATYPE_WAVE[iNum];
	_pWave_real      = new DATATYPE_WAVE[iNum];


	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real = new  DATATYPE_WAVE[iNum];

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
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
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
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
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

}


void CSudPlantInfo::AddChannInfoEnd(CCustomDB * pDb_)
{
	CPlantInfoOnLine::AddChannInfoEnd(pDb_);
	//添加需要补充的
	//组成数据保存时，插入的SQL语句
	//振动数据
	/*
	int iSmpNum = _sPlantInfo._iSmpNum;
	*/
	int iSmpNum = _sPlantInfo._iSmpNum;

	int iNum    = _sPlantInfo._iVibChannNum * iSmpNum;
	_pWave			= new DATATYPE_WAVE[iNum];
	_pWave_real      = new DATATYPE_WAVE[iNum];


	iNum        = _sPlantInfo._iDynChannNum * iSmpNum;
	_pWaveDyn   = new DATATYPE_WAVE[iNum];
	_pWaveDyn_real = new  DATATYPE_WAVE[iNum];

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
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
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
	strTmp.Format(" [%s],[%s],[%s],[%s],[%s],[%s],[%s],[%s]",gc_cSampleNum,gc_cSampleFreq,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cTime);
	_sDynFieldIns+=strTmp;
	strTmp.Format("[%s] ",gc_cSampleNum);
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

}

int CSudPlantInfo::AddSudLog(CCustomDB * pDB_)
{
    SwichDbName(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	int iRtn=-1;
    _iCurrEventID=_iEventID+1;
//	ID,	EVENT_ID,START_T,END_T,TYPE,STOP_FLAG,SYN_FLAG
	pDB_->Cmd("INSERT INTO [%s]",_sSudLogTblName.c_str());
	pDB_->Cmd("(%s,%s,%s,%s,%s,%s)",gc_cID,gc_cEventID,gc_cStart_T,gc_cType,gc_cStop_Flag,gc_cSynFlag);
	pDB_->Cmd("  VALUES(%d,%d,'%s',%d,0,0)",_iCurrEventID,_iCurrEventID,_sTimeCurr.c_str(),_iType);
	if (pDB_->Exec())
	{
        iRtn=_iCurrEventID;
		_iEventID=_iCurrEventID;
	}
	return iRtn;
}

CBHDateTime CSudPlantInfo::GetCurrDataTime(int& iMicroSecond_)
{
	CBHDateTime dt=CBHDateTime::GetCurrentTime();
	_pLockVibData->BeginRead();
	dt=_timeCurr;
	iMicroSecond_=_iMicroSecond;
	_pLockVibData->EndRead();
	return dt;
}

void CSudPlantInfo::SetDataTime(CBHDateTime dataTime_,int iMicroSecond_)
{
	_pLockVibData->BeginWrite();
	_timeCurr=dataTime_;
	_iMicroSecond=iMicroSecond_;
	_pLockVibData->EndWrite();
}

bool CSudPlantInfo::GetStatusType(E_SUD_TYPE & iType_,int & iEventID_)
{
	bool bStatus=false;
	_pLockVibData->BeginRead();
	bStatus=_bSudStatus;
	iType_=_iType;
	iEventID_=_iCurrEventID;
	_pLockVibData->EndRead();
	return bStatus;
}

float CSudPlantInfo::GetRevMark(IDBInterFace * pDB_,
								string sTableName_, 
								int iCurID_,
								float fRev_,
								CString sField_,
								const CBHDateTime& dtTime_,
								int iMicroSecond_)
{
	float lfRev5 = 0.0;
	_DECLARE_PERF_MEASURE_TIME()
		_BEGIN_PERF_MEASURE_TIME();

	bool bFirstRecord = false;

	float lfRev = 0.0,lfRev1 = 0.0, lfRev2 = 0.0, lfRev3 = 0.0;

	int lnMark = 0, lnMark1 = 0, lnMark2 = 0;

	float lfRevMarkInterval = GetRevMarkInterval();

	//第一条记录做标记
	pDB_->Cmd("select top 1 %s from [%s] where [%s] = %d and [%s] = %f order by [%s] asc",
		sField_,sTableName_.c_str(),gc_cEventID,iCurID_,gc_cRev5,1.0,gc_cTime);
	if(pDB_->More())
	{
		lfRev = pDB_->GetDouble(sField_);
	}
	else
	{
		lfRev5 = 1.0;
		lnMark = 1;
		bFirstRecord = true;
	}

	if(!bFirstRecord)
	{
		//转速变化超过标记间隔
		pDB_->Cmd("select top 1 %s,%s from [%s] where [%s] = %d and \
				  [%s] in(%f,%f,%f,%f) and [%s] <= '%s' order by [%s] desc",
				  sField_,gc_cTime,sTableName_.c_str(),gc_cEventID,iCurID_,\
				  gc_cRev5,1.0,3.0,5.0,7.0,gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			lfRev1 = pDB_->GetDouble(sField_);
			CBHDateTime dtTime = pDB_->GetTime(gc_cTime);
			if(dtTime == dtTime_)
			{
				pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' \
						  and [%s] <= %d order by [%s] desc",\
						  sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime_.Format(),\
						  gc_cMicroSecond,iMicroSecond_,gc_cMicroSecond);
				while(pDB_->More())
				{
					float fRev = pDB_->GetDouble(gc_cRev5);
					if((int)fRev == 1 || (int)fRev == 3 || (int)fRev == 5 || (int)fRev == 7)
					{
						lfRev1 = pDB_->GetDouble(sField_);
						break;
					}
				}
			}
			else
			{
				pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' order by [%s] desc",\
						  sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime.Format(),gc_cMicroSecond);
				while(pDB_->More())
				{
					float fRev = pDB_->GetDouble(gc_cRev5);
                    if((int)fRev == 1 || (int)fRev == 3 || (int)fRev == 5 || (int)fRev == 7)
                    {
                        lfRev1 = pDB_->GetDouble(sField_);
                        break;
                    }
				}
			}
			if(fRev_ - lfRev1 >= lfRevMarkInterval || fRev_ - lfRev1 <= -lfRevMarkInterval)
			{
				lnMark = 1;
			}
		}
		else
		{
			if(fRev_ - lfRev >= lfRevMarkInterval || fRev_ - lfRev <= -lfRevMarkInterval)
			{
				lnMark = 1;
			}
		}

		//转速变化超过2*标记间隔
		pDB_->Cmd("select top 1 %s,%s from [%s] where [%s] = %d and \
				  [%s] in(%f,%f,%f,%f) and [%s] <= '%s' order by [%s] desc",
				  sField_,gc_cTime,sTableName_.c_str(),gc_cEventID,iCurID_,\
				  gc_cRev5,2.0,3.0,6.0,7.0,gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			lfRev2 = pDB_->GetDouble(sField_);
			CBHDateTime dtTime = pDB_->GetTime(gc_cTime);
			if(dtTime == dtTime_)
			{
				pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' \
						  and [%s] <= %d order by [%s] desc",\
						  sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime_.Format(),\
						  gc_cMicroSecond,iMicroSecond_,gc_cMicroSecond);
				while(pDB_->More())
				{
					float fRev = pDB_->GetDouble(gc_cRev5);
					if((int)fRev == 2 || (int)fRev == 3 || (int)fRev == 6 || (int)fRev == 7)
					{
						lfRev2 = pDB_->GetDouble(sField_);
						break;
					}
				}
			}
			else
			{
                pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' order by [%s] desc",\
                    sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime.Format(),gc_cMicroSecond);
                while(pDB_->More())
                {
                    float fRev = pDB_->GetDouble(gc_cRev5);
                    if((int)fRev == 2 || (int)fRev == 3 || (int)fRev == 6 || (int)fRev == 7)
                    {
                        lfRev2 = pDB_->GetDouble(sField_);
                        break;
                    }
                }
			}
			if(fRev_ - lfRev2 >= 2*lfRevMarkInterval || fRev_ - lfRev2 <= -2*lfRevMarkInterval)
			{
				lnMark1 = 2;
			}
		}
		else
		{
			if(fRev_ - lfRev >= 2*lfRevMarkInterval || fRev_ - lfRev <= -2*lfRevMarkInterval)
			{
				lnMark1 = 2;
			}
		}

		//转速变化超过4*标记间隔
		pDB_->Cmd("select top 1 %s,%s from [%s] where [%s] = %d and \
				  [%s] in(%f,%f,%f,%f) and [%s] <= '%s' order by [%s] desc",
				  sField_,gc_cTime,sTableName_.c_str(),gc_cEventID,iCurID_,\
				  gc_cRev5,4.0,5.0,6.0,7.0,gc_cTime,dtTime_.Format(),gc_cTime);
		if(pDB_->More())
		{
			lfRev3 = pDB_->GetDouble(sField_);
			CBHDateTime dtTime = pDB_->GetTime(gc_cTime);
			if(dtTime == dtTime_)
			{
				pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' \
						  and [%s] <= %d order by [%s] desc",\
						  sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime_.Format(),\
						  gc_cMicroSecond,iMicroSecond_,gc_cMicroSecond);
				while(pDB_->More())
				{
					float fRev = pDB_->GetDouble(gc_cRev5);
					if((int)fRev == 4 || (int)fRev == 5 || (int)fRev == 6 || (int)fRev == 7)
					{
						lfRev3 = pDB_->GetDouble(sField_);
						break;
					}
				}
			}
			else
			{
                pDB_->Cmd("select %s,%s from [%s] where [%s] = '%s' order by [%s] desc",\
                    sField_,gc_cRev5,sTableName_.c_str(),gc_cTime,dtTime.Format(),gc_cMicroSecond);
                while(pDB_->More())
                {
                    float fRev = pDB_->GetDouble(gc_cRev5);
                    if((int)fRev == 4 || (int)fRev == 5 || (int)fRev == 6 || (int)fRev == 7)
                    {
                        lfRev3 = pDB_->GetDouble(sField_);
                        break;
                    }
                }
			}

			if(fRev_ - lfRev3 >= 4*lfRevMarkInterval || fRev_ - lfRev3 <= -4*lfRevMarkInterval)
			{
				lnMark2 = 4;
			}
		}
		else
		{
			if(fRev_ - lfRev >= 4*lfRevMarkInterval || fRev_ - lfRev <= -4*lfRevMarkInterval)
			{
				lnMark2 = 4;
			}
		}
	}

	lfRev5 = lnMark+lnMark1+lnMark2;
	_END_PERF_MEASURE_TIME("Rev test");
	return lfRev5;
}

int CSudPlantInfo::SaveSudVibRecord( IDBInterFace * pDB_,
									string sTableName_, 
									int iCurID_, 
									const CBHDateTime &tDatetime_, 
									int anMilliSecond,
									bool bMonth_/*=false*/ )
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
	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_VibCharValue vibValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev( pfRev, 5);
	GetRealSmpInfo(iSmpNum, iSmpFreq);

	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	float fRev = 0;
	string sRev;

	V_PCHANNINFO::iterator it;
	CVibChannParam *pVibChannParam=NULL;

	bool bStop = true;
	for(int i = 0; i < 5; i++)
	{
		if(pfRev[i] > 0)
		{
			bStop = false;
			break;
		}
	}
	if(bStop && this->GetSudFinishStatus() == true)
	{
		sRev = gc_cRev;
		fRev = pfRev[0];
		_pLockVibData->BeginWrite();
		this->_sVibRevField.clear();
		_pLockVibData->EndWrite();
		
	}
	else
	{
		_pLockVibData->BeginRead();
		sRev = this->_sVibRevField;
		_pLockVibData->EndRead();

		if(!sRev.empty())
		{
			for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
			{
				pVibChannParam=(CVibChannParam*)(*it);
				if(pVibChannParam->GetSpecCharTypeField(-5) == sRev)
				{
					_pLockVibData->BeginRead();
					try
					{
						vibValue=pVibChannParam->GetCharValueEx();
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

					fRev = vibValue._iRev;
					break;
				}
			}
			if(fRev < 0)
			{
				for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
				{
					pVibChannParam=(CVibChannParam*)(*it);

					_pLockVibData->BeginRead();
					try
					{
						vibValue=pVibChannParam->GetCharValueEx();
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

					if(vibValue._iRev > 0)
					{
						fRev = vibValue._iRev;
						sRev = pVibChannParam->GetSpecCharTypeField(-5);

						_pLockVibData->BeginWrite();
						this->_sVibRevField = sRev;
						_pLockVibData->EndWrite();
						break;
					}
				}
			}
		}	
		else
		{
			for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
			{
				pVibChannParam=(CVibChannParam*)(*it);

				_pLockVibData->BeginRead();
				try
				{
					vibValue=pVibChannParam->GetCharValueEx();
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

				if(vibValue._iRev > 0)
				{
					fRev = vibValue._iRev;
					sRev = pVibChannParam->GetSpecCharTypeField(-5);

					_pLockVibData->BeginWrite();
					this->_sVibRevField = sRev;
					_pLockVibData->EndWrite();
					break;
				}
			}
		}

		if(sRev.empty())
		{
			sRev = gc_cRev;
			fRev = pfRev[0];
		}
	}
	
	pfRev[4] = GetRevMark(pDB_,sTableName_,iCurID_,fRev,sRev.c_str(),tDatetime_,anMilliSecond);

	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*	int iSmpNumMax = GetPlantInfo()._iSmpNum;*/



	int iSmpNumMax = iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{
		iSaveWaveNum=iSmpNumMax;
	}



	ASSERT(iSaveWaveNum >0);

	int lnDataBufferMax = this->GetMiddleWareWavePointNumMax();

	iAllBytes=lnDataBufferMax*sizeof(DATATYPE_WAVE);

	
	


	pDB_->Cmd("INSERT INTO [%s](%s,%s,%s,%s) ",
		sTableName_.c_str(),
		gc_cEventID,
		gc_cMicroSecond,
		gc_cSynFlag,
		_sVibFieldIns.c_str());

	pDB_->Cmd("VALUES (%d,",iCurID_);
	pDB_->Cmd(" %d,",anMilliSecond);
	pDB_->Cmd(" %d,",0);

	index=0;
	int lnCharIndex = -1;

	//判断是否该存储
	BOOL lbShouldSave = TRUE;
	for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
	{
		lnCharIndex++;
		pVibChannParam=(CVibChannParam*)(*it);

		_pLockVibData->BeginWrite();
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

		//特征值全部为0，且配置了不保存空特征值则跳过
		if (!g_bIsShouldSaveEmptyValue)
		{			
			if (pVibChannParam->IsEigenValueEmpty())
			{
				lbShouldSave = FALSE;
			}
		}
		_pLockVibData->EndWrite();

		if (!lbShouldSave)
		{
			break;
		}

		_pLockVibData->BeginRead();
		try
		{
			vibValue=pVibChannParam->GetCharValueEx();
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
		if (vibValue._iSmpNum ==0)
		{
			S_ChannInfo loInfo;

			loInfo = this->GetSpecVibChannInfo(index);		

			CString lstrData;

			lstrData.Format(_T("CRealPlantInfo Setting Error , 参数不正确，采样点数为 0 ，采样点 ID:%s  No:%s\r") , loInfo._cChannID, loInfo._cChannNo );

			CHZLogManage::Error(lstrData,"CRealPlantInfo","SaveSudVibRecord");

			/*			continue;*/
		}

		index ++;

		iSaveWaveNum = vibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}

		if (iSaveWaveNum != 0)
		{
			iSmpNum=iSaveWaveNum;
		}		

		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ALLCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_ONECHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_TWOCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_THREECHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_HALFCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_RESCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_DCCHAR));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_GAP));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARONE));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTWO));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARTHREE));
		pDB_->Cmd("%.2f,",vibValue.GetSpecTypeData(GE_PHASECHARHALF));
		pDB_->Cmd("%d,",vibValue._iSmpFreq);
		pDB_->Cmd("%d,",iSaveWaveNum);
		ASSERT(iSmpNum != 0);
		pDB_->Cmd("%d,",vibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}

	if (!lbShouldSave)
	{
		return 0;
	}

	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);

	for (int nloop=0;nloop<5;++nloop)
	{	pDB_->Cmd("%.2f,",pfRev[nloop]);
	}

	pDB_->Cmd("'%s')",sTimeCurr.c_str());

	//_DECLARE_PERF_MEASURE_TIME();
	//_BEGIN_PERF_MEASURE_TIME();

	if(!pDB_->Exec())
	{
		CHZLogManage::Error("insert record failed by dse！","CRealPlantInfo","SaveSudVibRecord");
		return 0;
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",sTableName_.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d AND [%s]='%s'",gc_cMicroSecond,_iMicroSecond,gc_cTime,sTimeCurr.c_str());
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}

	if(lnID > 0)
	{
		pDB_->Cmd("SELECT %s FROM [%s]",_sVibFieldSel.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d",gc_cID,lnID);

	}
	else
	{
		pDB_->Cmd("SELECT %s FROM [%s]",_sVibFieldSel.c_str(),sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d AND [%s]='%s'",gc_cMicroSecond,_iMicroSecond,gc_cTime,sTimeCurr.c_str());

	}
	
	if(pDB_->More(TRUE))
	{
		//_END_PERF_MEASURE_TIME("Select Wave data");
		index=0;
		int lnCharIndex = -1;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		for (it=_vVibChann.begin();it!=_vVibChann.end();it++)
		{
			try
			{
				lnCharIndex++;
				pVibChannParam=(CVibChannParam*)(*it);

				BOOL lbShouldSave = TRUE;
				_pLockVibData->BeginWrite();
				pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

				//特征值全部为0，且配置了不保存空特征值则跳过
				if (!g_bIsShouldSaveEmptyValue)
				{
					if (pVibChannParam->IsEigenValueEmpty())
					{
						lbShouldSave = FALSE;
					}
				}
				_pLockVibData->EndWrite();

				if (!lbShouldSave)
				{
					continue;
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
			//_BEGIN_PERF_MEASURE_TIME()
			bool lbRet = pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
			lbRet =pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			//_END_PERF_MEASURE_TIME("Insert wave data");
		
		}
		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
		
	}
	else
	{
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveSudVibRecord");
	}
	return 1;
}

int CSudPlantInfo::SaveSudTrend( IDBInterFace * pDb_,int anSaveType )
{
	string lstrTableName;
	int lbRet = 0;
	switch(anSaveType)
	{
		//振动数据
	case SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE:
		lstrTableName.append("SUD_");
		lstrTableName.append(this->GetCompanyAlias());
		lstrTableName.append(this->GetPlantNo());
		if(g_bIsDiskFull)
		{
			SwitchDbNameEx(pDb_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

			V_PCHANNINFO::iterator it=_vVibChann.begin();
			int lnTotalWavePtNum = 0;
			vector<CString> lvChannNo;

			_pLockVibData->BeginRead();
			for (it=_vVibChann.begin();it!=_vVibChann.end();++it)
			{
				lnTotalWavePtNum += ((CVibChannParam*)(*it))->GetCharValueEx()._iSmpNum;
				lvChannNo.push_back(((CVibChannParam*)(*it))->GetChannInfo()._cChannNo);
			}
			_pLockVibData->EndRead();

			this->DeleteSudVibRecord(pDb_,this->_iCurrEventID,lvChannNo,lnTotalWavePtNum);
		}
		lbRet =this->SaveSudVibRecord(pDb_,lstrTableName,this->_iCurrEventID,this->_timeCurr,this->_iMicroSecond);
		break;
	case SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE:
		lstrTableName.append("SUD_DYN_");
		lstrTableName.append(this->GetCompanyAlias());
		lstrTableName.append(this->GetPlantNo());
		if(g_bIsDiskFull)
		{
			SwitchDbNameEx(pDb_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
			this->DeleteSudDynRecord(pDb_,this->_iCurrEventID);
		}
		lbRet =this->SaveSudDynRecord(pDb_,lstrTableName,this->_iCurrEventID,this->_timeCurr,this->_iMicroSecond);
		break;
	case SERVICE_SERVICE_ON_SUD_RECEIVEPROC:
		lstrTableName.append("SUD_PROC_");
		lstrTableName.append(this->GetCompanyAlias());
		lstrTableName.append(this->GetPlantNo());
		if(g_bIsDiskFull)
		{
			SwitchDbNameEx(pDb_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
			this->DeleteSudProcRecord(pDb_,this->_iCurrEventID);
		}
		lbRet =this->SaveSudProcRecord(pDb_,lstrTableName,this->_iCurrEventID,this->_timeCurr,this->_iMicroSecond);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return lbRet;
}


int CSudPlantInfo::SaveSudDynRecord( IDBInterFace * pDB_, 
									string sTableName_, 
									int iCurID_, 
									const CBHDateTime &tDatetime_, 
									int anMilliSecond, 
									bool bMonth_/*=false*/ )
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

	if (	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_KEYPUMP 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_WINDPEQ 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_FAN 
		||	(GetPlantInfo()._iMachineType) == GE_MACHINETYPE_PUMP)
	{
		iSaveWaveNum = GD_MIDDLEWARE_WAVE_POINT_WIND;
	}

	S_DynCharValue vibValue;
	string sTimeCurr;
	float pfRev[5]={.0};
	int iSmpNum=0,iSmpFreq=0;
	int iAllBytes=0;
	int nloop=0;

	GetRealRev(pfRev,5);
	GetDynRealSmpInfo(iSmpNum, iSmpFreq);


	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	float fRev = 0;
	string sRev;

	V_PCHANNINFO::iterator it;
	CDynChannParam *pVibChannParam=NULL;

	bool bStop = true;
	for(int i = 0; i < 5; i++)
	{
		if(pfRev[i] > 0)
		{
			bStop = false;
			break;
		}
	}
	if(bStop && this->GetSudFinishStatus() == true)
	{
		sRev = gc_cRev;
		fRev = pfRev[0];
		_pLockDynData->BeginWrite();
		this->_sDynRevField.clear();
		_pLockDynData->EndWrite();

	}
	else
	{
		_pLockDynData->BeginRead();
		sRev = this->_sDynRevField;
		_pLockDynData->EndRead();

		if(!sRev.empty())
		{
			for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
			{
				pVibChannParam=(CDynChannParam*)(*it);
				if(pVibChannParam->GetSpecCharTypeField(-5) == sRev)
				{
					_pLockDynData->BeginRead();
					try
					{
						vibValue=pVibChannParam->GetCharValueEx();
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

					fRev = vibValue._iRev;
					break;
				}
			}
			if(fRev < 0)
			{
				for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
				{
					pVibChannParam=(CDynChannParam*)(*it);

					_pLockDynData->BeginRead();
					try
					{
						vibValue=pVibChannParam->GetCharValueEx();
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

					if(vibValue._iRev > 0)
					{
						fRev = vibValue._iRev;
						sRev = pVibChannParam->GetSpecCharTypeField(-5);

						_pLockDynData->BeginWrite();
						this->_sDynRevField = sRev;
						_pLockDynData->EndWrite();
						break;
					}
				}
			}
		}	
		else
		{
			for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
			{
				pVibChannParam=(CDynChannParam*)(*it);

				_pLockDynData->BeginRead();
				try
				{
					vibValue=pVibChannParam->GetCharValueEx();
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

				if(vibValue._iRev > 0)
				{
					fRev = vibValue._iRev;
					sRev = pVibChannParam->GetSpecCharTypeField(-5);

					_pLockDynData->BeginWrite();
					this->_sDynRevField = sRev;
					_pLockDynData->EndWrite();
					break;
				}
			}
		}

		if(sRev.empty())
		{
			sRev = gc_cRev;
			fRev = pfRev[0];
		}
	}

	pfRev[4] = GetRevMark(pDB_,sTableName_,iCurID_,fRev,sRev.c_str(),tDatetime_,anMilliSecond);
	//组态配置的采样点数为测点最大的采样点数，以此分配内存即可
	/*int iSmpNumMax = GetPlantInfo()._iSmpNum;*/

	int iSmpNumMax =iSaveWaveNum;

	if(iSaveWaveNum>iSmpNumMax)
	{	
		iSaveWaveNum=iSmpNumMax;
	}

	iAllBytes=this->GetMiddleWareWavePointNumMax()*sizeof(DATATYPE_WAVE);

	
	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s,%s,%s) ",sTableName_.c_str(),
		gc_cEventID,
		gc_cMicroSecond,
		gc_cSynFlag,
		_sDynFieldIns.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	pDB_->Cmd(" %d, ",anMilliSecond);
	pDB_->Cmd(" %d,",0);
	index=0;
	int lnCharIndex =-1;
	BOOL lbShouldSave = TRUE;
	for ( it= _vDynChann.begin(); it!= _vDynChann.end(); it++)
	{
		lnCharIndex++;
		pVibChannParam = (CDynChannParam*)(*it);



		_pLockDynData->BeginWrite();
		pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);
		if (!g_bIsShouldSaveEmptyValue)
		{
			if (pVibChannParam->IsEigenValueEmpty())
			{
				lbShouldSave = FALSE;
			}
		}
		_pLockDynData->EndWrite();

		if (!lbShouldSave)
		{
			break;
		}

		_pLockDynData->BeginRead();
		try
		{
			vibValue = pVibChannParam->GetCharValueEx();
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

		iSaveWaveNum = vibValue._iSmpNum;

		if(iSaveWaveNum>iSmpNumMax)
		{
			iSaveWaveNum=iSmpNumMax;
		}

		iSmpNum=iSaveWaveNum;
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
		pDB_->Cmd("%d,",iSmpNum);
		pDB_->Cmd("%d,",vibValue._iRev);
		pDB_->Cmd("'%s',",pVibChannParam->GetChannInfo()._cTempID);
	}
	pDB_->Cmd("%d,",iSmpNum);
	pDB_->Cmd("%d,",iSmpFreq);
	for (int nloop=0;nloop<5;++nloop)
		pDB_->Cmd("%.2f,",pfRev[nloop]);
	pDB_->Cmd("'%s')",sTimeCurr.c_str());
	zdlTraceLine(pDB_->GetSql());

	if (!lbShouldSave)
	{
		return 0;
	}
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("insert record failed by dse ","CRealPlantInfo","SaveDynRecord");
		return 0;
	}

	int lnID = -1;

	pDB_->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",sTableName_.c_str(),gc_cID);
	if(pDB_->More())
	{
		pDB_->Cmd("SELECT [%s] FROM [%s] ",gc_cID,sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d AND [%s]='%s'",gc_cMicroSecond,_iMicroSecond,gc_cTime,sTimeCurr.c_str());
		if(pDB_->More())
		{
			lnID = pDB_->GetLong(gc_cID);
		}
	}

	if(lnID > 0)
	{
		pDB_->Cmd("SELECT %s FROM [%s]", _sDynFieldSel.c_str(), sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d'",gc_cID,lnID);
	}
	else
	{
		pDB_->Cmd("SELECT %s FROM [%s]", _sDynFieldSel.c_str(), sTableName_.c_str());
		pDB_->Cmd(" WHERE %s=%d AND [%s]='%s'",gc_cMicroSecond,_iMicroSecond,gc_cTime,sTimeCurr.c_str());
	}
	
	if(pDB_->More(TRUE))
	{
		index=0;
		int lnCharIndex = -1;
		BYTE  *pBuf=new BYTE[iAllBytes];
		float *pfCustomData=new float[GD_CUSTOMPARAM_BUFFSIZE];
		for (it=_vDynChann.begin();it!=_vDynChann.end();it++)
		{
			lnCharIndex++;
			pVibChannParam=(CDynChannParam*)(*it);

			BOOL lbShouldSave = TRUE;
			_pLockDynData->BeginWrite();

			pVibChannParam->ComputeEigenValue(GetPlantInfo()._iMachineType,lnCharIndex);

			if (!g_bIsShouldSaveEmptyValue)
			{
				if (pVibChannParam->IsEigenValueEmpty())
				{
					lbShouldSave = FALSE;
				}
			}
			_pLockDynData->EndWrite();

			if (!lbShouldSave)
			{
				continue;
			}

			int iRealSmpNum= 0;
			_pLockDynData->BeginRead();
			try
			{
				iRealSmpNum=pVibChannParam->GetCharValue()._iSmpNum;
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
			/*			if(iSaveWaveNum>iRealSmpNum)*/

			iSaveWaveNum = iRealSmpNum;

			if(iSaveWaveNum>iSmpNumMax)
			{
				iSaveWaveNum=iSmpNumMax;
			}

			int lnTotalSize = 0;
			_pLockDynData->BeginRead();

			try
			{
				CWaveHead lsHead = pVibChannParam->WaveHead();
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

			_pLockDynData->EndRead();
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),pBuf,lnTotalSize);
			pDB_->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE*)pfCustomData,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
			//CString sTemp;sTemp.Format("_fCustomParam[0]:%f,_fCustomParam[1]:%f,_fCustomParam[2]:%f,_fCustomParam[3]:%f",pVibChannParam->GetChannInfo()._fCustomParam[0],pVibChannParam->GetChannInfo()._fCustomParam[1],pVibChannParam->GetChannInfo()._fCustomParam[2],pVibChannParam->GetChannInfo()._fCustomParam[3]);BHTRACE_DEBUG(sTemp);
			//CHZLogManage::Debug(sTemp,"CRealPlantInfo","SaveDynRecord");
		}
		KD_DeletePoint(pBuf);
		KD_DeletePoint(pfCustomData);
	}
	else
	{
		CHZLogManage::Error("select record failed by dse","CRealPlantInfo","SaveDynRecord");
	}
	return 1;
}


int CSudPlantInfo::SaveSudProcRecord( IDBInterFace * pDB_, 
									 string sTableName_, 
									 int iCurID_, 
									 const CBHDateTime &tDatetime_, 
									 int anMilliSecond, 
									 bool bMonth_/*=false*/ )
{
	SwitchDbNameEx(pDB_,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());
	float pfRev[5];
	float fData=.0;
	string sTimeCurr;
	GetRealRev( pfRev, 5);

	sTimeCurr = TimeConvertString(tDatetime_).GetString();

	float fRev = 0;
	string sRev;

	bool bStop = true;
	for(int i = 0; i < 5; i++)
	{
		if(pfRev[i] > 0)
		{
			bStop = false;
			break;
		}
	}
	if(bStop && this->GetSudFinishStatus() == true)
	{
		sRev = gc_cRev;
		fRev = pfRev[0];
		_pLockProcData->BeginWrite();
		this->_sProcRevField.clear();
		_pLockProcData->EndWrite();

	}
	else
	{
		_pLockProcData->BeginRead();
		sRev = this->_sProcRevField;
		_pLockProcData->EndRead();

		if(!sRev.empty())
		{
			for(int i = 0; i < 4; i++)
			{
				CString strRev;
				if(i == 0)
				{
					strRev = "Rev";
				}
				else
				{
					strRev.Format("Rev%d",i+1);
				}

				if(strRev == sRev.c_str())
				{
					fRev = pfRev[i];
					break;
				}
			}
			if(fRev < 0)
			{
				for(int i = 0; i < 4; i++)
				{
					CString strRev;
					if(i == 0)
					{
						strRev = "Rev";
					}
					else
					{
						strRev.Format("Rev%d",i+1);
					}
					if(pfRev[i] > 0)
					{
						fRev = pfRev[i];
						sRev = strRev;

						_pLockProcData->BeginWrite();
						this->_sProcRevField = sRev;
						_pLockProcData->EndWrite();
						break;
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < 4; i++)
			{
				CString strRev;
				if(i == 0)
				{
					strRev = "Rev";
				}
				else
				{
					strRev.Format("Rev%d",i+1);
				}
				if(pfRev[i] > 0)
				{
					fRev = pfRev[i];
					sRev = strRev;
					_pLockProcData->BeginWrite();
					this->_sProcRevField = sRev;
					_pLockProcData->EndWrite();
					break;
				}
			}
		}

		if(sRev.empty())
		{
			sRev = gc_cRev;
			fRev = pfRev[0];
		}
	}
	
	pfRev[4] = GetRevMark(pDB_,sTableName_,iCurID_,fRev,sRev.c_str(),tDatetime_,anMilliSecond);

	V_PCHANNINFO::iterator result;
	CProcChannParam *pProcChann=NULL;
	//对于月趋势就是直接插入记录，因为月趋势没有条数限制。对于天和小时趋势来说就是如果没有这个ID的记录，就插入。
	pDB_->Cmd("INSERT INTO [%s](%s,%s,%s,%s) ",sTableName_.c_str(),gc_cEventID,gc_cMicroSecond,gc_cSynFlag,_sProcField.c_str());
	pDB_->Cmd("VALUES (%d,",iCurID_);
	pDB_->Cmd(" %d,",anMilliSecond);
	pDB_->Cmd(" %d,",0);
	for (result=_vProcChann.begin();result!=_vProcChann.end();result++)
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
	pDB_->Cmd("'%s',",sTimeCurr.c_str());
	pDB_->Cmd(" %.2f,",pfRev[0]);
	pDB_->Cmd(" %.2f,",pfRev[1]);
	pDB_->Cmd(" %.2f,",pfRev[2]);
	pDB_->Cmd(" %.2f,",pfRev[3]);
	pDB_->Cmd(" %.2f)",pfRev[4]);
	if(!pDB_->Exec())
	{
		CHZLogManage::Error("插入过程量趋势记录失败","CRealPlantInfo","SaveProcRecord");
	}

	return 0;
}


int CSudPlantInfo::GetProcData(float *pfData_,int iNum_,int iStartIndex_/* =0 */)
{
	int reallen=(iStartIndex_+iNum_) > _sPlantInfo._iProcChannNum ?(_sPlantInfo._iProcChannNum-iStartIndex_):iNum_;
	if ((reallen<1)||(iStartIndex_<0)){
		return -1;
	}
	bool bUpdate;
	_pLockDataUpdateStatues->BeginRead();
	bUpdate=_bProcDataUpdateSend;
	_pLockDataUpdateStatues->EndRead();
	if (bUpdate)
	{
		_pLockProcData->BeginRead();
		try
		{
			for(int loop=0;loop<reallen;++loop)
				pfData_[loop]=((CProcChannParam*)_vProcChann[loop+iStartIndex_])->GetData();
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
	}
	else
		return -2;
	return reallen;
}

void CSudPlantInfo::SetSudFinishStatus(bool bFinishStatus_)
{
    _pLockVibData->BeginWrite();
    this->_bSudFinishStatus = bFinishStatus_;
    _pLockVibData->EndWrite();
}

bool CSudPlantInfo::GetSudFinishStatus()
{
    bool bStatus = true; 
    _pLockVibData->BeginRead();
     bStatus = this->_bSudFinishStatus;
     _pLockVibData->EndRead();
     return bStatus;
}

int CSudPlantInfo::SaveSudVibData( IDBInterFace *apDB, 
								  long aEventID,
								  CBHDateTime aDatetime, 
								  int aMicroSecond, 
								  int iStartIndex_, 
								  int aChannNum, 
								  S_VibCharValue *apCharValue, 
								  DATATYPE_WAVE *apWave, 
								  int aWaveNum, 
								  S_CustomParamData *apParam, 
								  string aTemplate, 
								  int aSmpNum, 
								  int aSmpFreq, 
								  float apRev[5],
								  const float& fScaleCoef_,
								  int anPrecisionEnabled,
								  std::vector<float>& afPreciousMin ,
								  std::vector<float>&  afPreciousMax )
{
	SwitchDbNameEx(apDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	std::string lsChannNo;

	int lnVibChannNum = _vVibChann.size();

	CVibChannParam *pVibChannParam=NULL;

	int iRet = 0;

	if ((iStartIndex_<0) || (aChannNum<1) || ((iStartIndex_+aChannNum) > lnVibChannNum))
		return -1;

	int lnWaveOffset = 0;

	for (int i=0;i<aChannNum;++i)
	{
		if (lnWaveOffset + apCharValue[i]._iSmpNum > aWaveNum)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	int lnChannIndex = 0;
	int lnEndIndex   = iStartIndex_ + aChannNum;

	int nCount=0;

	aSmpFreq=apCharValue[nCount]._iSmpFreq;
	aSmpNum=apCharValue[nCount]._iSmpNum;
	int lpnRevFlag[5] = {0,0,0,0,0};
	for (int nloop=iStartIndex_;nloop<lnEndIndex;++nloop)
	{
		int iRevType=_vVibChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			apRev[0] = apCharValue[nCount]._iRev;
			lpnRevFlag[0] = 1;
			break;
		case 2:
			apRev[1] = apCharValue[nCount]._iRev;
			lpnRevFlag[1] = 1;
			break;
		case 3:
			apRev[2] = apCharValue[nCount]._iRev;
			lpnRevFlag[2] = 1;
			break;
		case 4:
			apRev[3] = apCharValue[nCount]._iRev;
			lpnRevFlag[3] = 1;
			break;
		case 5:
			apRev[4] = apCharValue[nCount]._iRev;
			lpnRevFlag[4] = 1;
			break;
		default:
			apRev[0] = apCharValue[nCount]._iRev;
			lpnRevFlag[0] = 1;
		}
		++nCount;
	}


	apDB->BeginTran();

	apDB->Cmd("SELECT 1");
	apDB->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());
	apDB->Cmd(" WHERE  [%s]=%d AND [%s]=%d AND [%s]='%s' ",
		gc_cEventID, aEventID,
		gc_cMicroSecond,aMicroSecond,
		gc_cTime,TimeConvertString(aDatetime));
	if (!apDB->More())
	{
		apDB->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sSudDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);

		for (lnChannIndex=iStartIndex_;lnChannIndex<lnEndIndex;++lnChannIndex)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParam *)_vVibChann[lnChannIndex];
			END_ERROR_HANDLE
				_pLockVibData->EndRead();

			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-3).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-4).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-5).c_str());
			apDB->Cmd(",[%s]",pVibChannParam->GetSpecCharTypeField(-6).c_str());
		}

		apDB->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",aEventID,TimeConvertString(aDatetime),aMicroSecond,apRev[0],apRev[1],apRev[2],apRev[3],apRev[4],aSmpFreq,aSmpNum);
		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			const S_VibCharValue& vibCharValue = apCharValue[lnChannIndex];

			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParam *)_vVibChann[lnChannIndex + iStartIndex_];
			END_ERROR_HANDLE
				_pLockVibData->EndRead();

			apDB->Cmd(",%.2f",vibCharValue._fCharAll);
			apDB->Cmd(",%.2f",vibCharValue._fCharOne);
			apDB->Cmd(",%.2f",vibCharValue._fCharTwo);
			apDB->Cmd(",%.2f",vibCharValue._fCharThree);
			apDB->Cmd(",%.2f",vibCharValue._fCharHalf);
			apDB->Cmd(",%.2f",vibCharValue._fCharRes);
			apDB->Cmd(",%.2f",vibCharValue._fDC);
			apDB->Cmd(",%.2f",vibCharValue._fGap);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseOne);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseTwo);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseThree);
			apDB->Cmd(",%.2f",vibCharValue._fPhaseHalf);
			apDB->Cmd(",%d",vibCharValue._iSmpFreq);
			apDB->Cmd(",%d",vibCharValue._iSmpNum);
			apDB->Cmd(",%d",vibCharValue._iRev);
			apDB->Cmd(",'%s'",pVibChannParam->GetChannInfo()._cAlarmTempID);
		}

		apDB->Cmd(")");
		if (!(apDB->Exec()))
		{
			iRet= -2;
			goto QUIT_SAVE_SUD_VIB_DATA;
		}
	}
	else
	{
		apDB->Cmd("UPDATE [%s] SET ",_sSudDataTblName.c_str());

		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			const S_VibCharValue& vibCharValue = apCharValue[lnChannIndex];

			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParam *)_vVibChann[lnChannIndex + iStartIndex_];
			END_ERROR_HANDLE
				_pLockVibData->EndRead();
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_ALLCHAR).c_str(),vibCharValue._fCharAll);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_ONECHAR).c_str(),vibCharValue._fCharOne);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_TWOCHAR).c_str(),vibCharValue._fCharTwo);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_THREECHAR).c_str(),vibCharValue._fCharThree);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_HALFCHAR).c_str(),vibCharValue._fCharHalf);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_RESCHAR).c_str(),vibCharValue._fCharRes);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_DCCHAR).c_str(),vibCharValue._fDC);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_GAP).c_str(),vibCharValue._fGap);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARONE).c_str(),vibCharValue._fPhaseOne);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTWO).c_str(),vibCharValue._fPhaseTwo);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARTHREE).c_str(),vibCharValue._fPhaseThree);
			apDB->Cmd(" [%s]=%.2f,",pVibChannParam->GetSpecCharTypeField(GE_PHASECHARHALF).c_str(),vibCharValue._fPhaseHalf);
			apDB->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-3).c_str(),vibCharValue._iSmpFreq);
			apDB->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-4).c_str(),vibCharValue._iSmpNum);
			apDB->Cmd(" [%s]=%d,",pVibChannParam->GetSpecCharTypeField(-5).c_str(),vibCharValue._iRev);
			apDB->Cmd(" [%s]='%s',",pVibChannParam->GetSpecCharTypeField(-6).c_str(),pVibChannParam->GetChannInfo()._cAlarmTempID);
		}
		apDB->Cmd("[%s]=%d ",gc_cMicroSecond,aMicroSecond);
		if(lpnRevFlag[0] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev,apRev[0]);
		}
		if(lpnRevFlag[1] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev2,apRev[1]);
		}
		if(lpnRevFlag[2] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev3,apRev[2]);
		}
		if(lpnRevFlag[3] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev4,apRev[3]);
		}
		if(lpnRevFlag[4] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev5,apRev[4]);
		}
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond, aMicroSecond,
			gc_cTime, TimeConvertString(aDatetime));
		if (!(apDB->Exec()))
		{
			iRet= -3;
			goto QUIT_SAVE_SUD_VIB_DATA;
		}
	}


	int lnID = -1;

	apDB->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sSudDataTblName.c_str(),gc_cID);
	if(apDB->More())
	{
		apDB->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sSudDataTblName.c_str());
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond,aMicroSecond,
			gc_cTime,TimeConvertString(aDatetime));
		if(apDB->More())
		{
			lnID = apDB->GetLong(gc_cID);
		}
	}

	apDB->Cmd("SELECT 1");
	for (lnChannIndex=iStartIndex_;lnChannIndex<lnEndIndex;++lnChannIndex)
	{
		_pLockVibData->BeginRead();
		BEGIN_ERROR_HANDLE
			pVibChannParam = (CVibChannParam *)_vVibChann[lnChannIndex];
		END_ERROR_HANDLE
			_pLockVibData->EndRead();

		apDB->Cmd(",%s,%s",pVibChannParam->GetSpecCharTypeField(-1).c_str(),pVibChannParam->GetSpecCharTypeField(-7).c_str());
	}
	apDB->Cmd(" FROM [%s] ",_sSudDataTblName.c_str());

	if(lnID > 0)
	{
		apDB->Cmd(" WHERE [%s]=%d ",gc_cID,lnID);
	}
	else
	{
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond,aMicroSecond,
			gc_cTime,TimeConvertString(aDatetime));
	}
	
	if (apDB->More(TRUE))
	{
		lnWaveOffset = 0;

		CWaveHead lsHead;
		lsHead.SetLength(4);

		for (int i=0;i<aChannNum;++i)
		{
			_pLockVibData->BeginRead();
			BEGIN_ERROR_HANDLE
				pVibChannParam = (CVibChannParam *)_vVibChann[i + iStartIndex_];
			END_ERROR_HANDLE
				_pLockVibData->EndRead();

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
			int lnTotalSize = WaveAddHead(&apWave[lnWaveOffset],apCharValue[i]._iSmpNum,lsHead,lvWave);

			if(!(apDB->SetBuffer(pVibChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&lvWave.front(),lnTotalSize)))
			{
				iRet=-4;
				goto QUIT_SAVE_SUD_VIB_DATA;
			}

			apDB->SetBuffer(pVibChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(apParam[i].pfCustomParom),sizeof(S_CustomParamData));

			lnWaveOffset += apCharValue[i]._iSmpNum;

			iRet++;
		}
	}

QUIT_SAVE_SUD_VIB_DATA:

	if (iRet>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	return iRet;
}

int CSudPlantInfo::SaveSudDynData( IDBInterFace *apDB, 
								  long aEventID, 
								  CBHDateTime aDatetime, 
								  int aMicroSecond, 
								  int aStartIndex, 
								  int aChannNum, 
								  S_DynCharValue *apCharValue, 
								  DATATYPE_WAVE *apWave, 
								  int aWaveNum, 
								  S_CustomParamData *apParam, 
								  string aTemplate, 
								  int aSmpNum, 
								  int aSmpFreq, 
								  float apRev[5], 
								  const float& fScaleCoef_,
								  int anPrecisionEnabled,
								  std::vector<float>& afPreciousMin ,
								  std::vector<float>&  afPreciousMax )
{
	SwitchDbNameEx(apDB,(_sCompanyAlias+_sPlantInfo._cPlantNo).c_str());

	std::string lsChannNo;

	CDynChannParam *pDynChannParam=NULL;

	int lnDynChannNum = _vDynChann.size();

	int iRet = 0;

	if ((aStartIndex<0) || (aChannNum<1) || ((aStartIndex+aChannNum) > lnDynChannNum))
		return -1;



	int lnWaveOffset = 0;

	for (int i=0;i<aChannNum;++i)
	{
		if (lnWaveOffset + apCharValue[i]._iSmpNum > aWaveNum)
		{
			ASSERT(FALSE);
			return -1;
		}
	}

	int nCount = 0;
	int lnChannIndex = 0;
	int lnEndIndex   = aStartIndex + aChannNum;

	aSmpFreq=apCharValue[nCount]._iSmpFreq;
	aSmpNum=apCharValue[nCount]._iSmpNum;
	int lpnRevFlag[5] = {0,0,0,0,0};
	for (int nloop=aStartIndex;nloop<lnEndIndex;++nloop)
	{
		int iRevType=_vDynChann[nloop]->GetChannInfo()._iRevType;
		switch(iRevType)
		{
		case 1:
			apRev[0] = apCharValue[nCount]._iRev;
			lpnRevFlag[0] = 1;
			break;
		case 2:
			apRev[1] = apCharValue[nCount]._iRev;
			lpnRevFlag[1] = 1;
			break;
		case 3:
			apRev[2] = apCharValue[nCount]._iRev;
			lpnRevFlag[2] = 1;
			break;
		case 4:
			apRev[3] = apCharValue[nCount]._iRev;
			lpnRevFlag[3] = 1;
			break;
		case 5:
			apRev[4] = apCharValue[nCount]._iRev;
			lpnRevFlag[4] = 1;
			break;
		default:
			apRev[0] = apCharValue[nCount]._iRev;
			lpnRevFlag[0] = 1;
		}
		++nCount;
	}



	apDB->BeginTran();

	apDB->Cmd("SELECT 1");
	apDB->Cmd(" FROM [%s] ",_sSudDynDataTblName.c_str());
	apDB->Cmd(" WHERE  [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond,aMicroSecond,
			gc_cTime,TimeConvertString(aDatetime));
	
	if (!apDB->More())
	{
		apDB->Cmd("insert into [%s](%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",_sSudDynDataTblName.c_str(),gc_cEventID,gc_cTime,gc_cMicroSecond,gc_cRev,gc_cRev2,gc_cRev3,gc_cRev4,gc_cRev5,gc_cSampleFreq,gc_cSampleNum,gc_cSynFlag);

		for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParam *)_vDynChann[lnChannIndex];
			END_ERROR_HANDLE
				_pLockDynData->EndRead();

			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(0).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(1).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(2).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(3).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(4).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(5).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(6).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(7).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(8).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(9).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(10).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(11).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-3).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-4).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-5).c_str());
			apDB->Cmd(",[%s]",pDynChannParam->GetSpecCharTypeField(-6).c_str());
		}

		apDB->Cmd(") VALUES(%d,'%s',%d,%.2f,%.2f,%.2f,%.2f,%.2f,%d,%d,0 ",aEventID,TimeConvertString(aDatetime),aMicroSecond,apRev[0],apRev[1],apRev[2],apRev[3],apRev[4],aSmpFreq,aSmpNum);
		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			S_DynCharValue& dynCharValue = apCharValue[lnChannIndex];
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParam *)_vDynChann[lnChannIndex];
			END_ERROR_HANDLE
				_pLockDynData->EndRead();

			for (int i = 0; i < 12; i++)
				apDB->Cmd(",%.2f",dynCharValue.GetSpecTypeData(i));

			apDB->Cmd(",%d",dynCharValue._iSmpFreq);
			apDB->Cmd(",%d",dynCharValue._iSmpNum);
			apDB->Cmd(",%d",dynCharValue._iRev);
			apDB->Cmd(",'%s'",pDynChannParam->GetChannInfo()._cTempID);
		}

		apDB->Cmd(")");
		if (!(apDB->Exec()))
		{
			iRet= -2;
			goto QUIT_SAVE_DYN_SUD_DATA;
		}
	}
	else
	{
		apDB->Cmd("UPDATE [%s] SET ",_sSudDynDataTblName.c_str());

		for (lnChannIndex=0;lnChannIndex<aChannNum;++lnChannIndex)
		{
			S_DynCharValue& dynCharValue = apCharValue[lnChannIndex];

			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParam *)_vDynChann[lnChannIndex + aStartIndex];
			END_ERROR_HANDLE
				_pLockDynData->EndRead();
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(0).c_str(),dynCharValue._fCharValue0);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(1).c_str(),dynCharValue._fCharValue1);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(2).c_str(),dynCharValue._fCharValue2);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(3).c_str(),dynCharValue._fCharValue3);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(4).c_str(),dynCharValue._fCharValue4);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(5).c_str(),dynCharValue._fCharValue5);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(6).c_str(),dynCharValue._fCharValue6);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(7).c_str(),dynCharValue._fCharValue7);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(8).c_str(),dynCharValue._fCharValue8);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(9).c_str(),dynCharValue._fCharValue9);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(10).c_str(),dynCharValue._fCharValue10);
			apDB->Cmd(" [%s]=%.2f,",pDynChannParam->GetSpecCharTypeField(11).c_str(),dynCharValue._fCharValue11);
			apDB->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-3).c_str(),dynCharValue._iSmpFreq);
			apDB->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-4).c_str(),dynCharValue._iSmpNum);
			apDB->Cmd(" [%s]=%d,",pDynChannParam->GetSpecCharTypeField(-5).c_str(),dynCharValue._iRev);
			apDB->Cmd(" [%s]='%s',",pDynChannParam->GetSpecCharTypeField(-6).c_str(),pDynChannParam->GetChannInfo()._cAlarmTempID);

		}
		apDB->Cmd("[%s]=%d ",gc_cMicroSecond,aMicroSecond);
		if(lpnRevFlag[0] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev,apRev[0]);
		}
		if(lpnRevFlag[1] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev2,apRev[1]);
		}
		if(lpnRevFlag[2] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev3,apRev[2]);
		}
		if(lpnRevFlag[3] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev4,apRev[3]);
		}
		if(lpnRevFlag[4] == 1)
		{
			apDB->Cmd(",[%s]=%d ",gc_cRev5,apRev[4]);
		}
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond, aMicroSecond,
			gc_cTime, TimeConvertString(aDatetime));
		if (!(apDB->Exec()))
		{
			iRet= -3;
			goto QUIT_SAVE_DYN_SUD_DATA;
		}
	}

	int lnID = -1;

	apDB->Cmd("SELECT [name] FROM SYSCOLUMNS  WHERE ID=OBJECT_ID('%s') AND NAME='%s'",_sSudDynDataTblName.c_str(),gc_cID);
	if(apDB->More())
	{
		apDB->Cmd("SELECT [%s] FROM [%s] ",gc_cID,_sSudDynDataTblName.c_str());
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond,aMicroSecond,
			gc_cTime,TimeConvertString(aDatetime));
		if(apDB->More())
		{
			lnID = apDB->GetLong(gc_cID);
		}
	}

	apDB->Cmd("SELECT 1");
	for (lnChannIndex=aStartIndex;lnChannIndex<lnEndIndex;++lnChannIndex)
	{
		_pLockDynData->BeginRead();
		BEGIN_ERROR_HANDLE
			pDynChannParam = (CDynChannParam *)_vDynChann[lnChannIndex];
		END_ERROR_HANDLE
			_pLockDynData->EndRead();

		apDB->Cmd(",%s,%s",pDynChannParam->GetSpecCharTypeField(-1).c_str(),pDynChannParam->GetSpecCharTypeField(-7).c_str());

	}
	apDB->Cmd(" FROM [%s] ",_sSudDynDataTblName.c_str());

	if(lnID > 0)
	{
		apDB->Cmd(" WHERE [%s]=%d ",gc_cID,lnID);
	}
	else
	{
		apDB->Cmd(" WHERE [%s]=%d AND [%s]=%d AND [%s]='%s' ",
			gc_cEventID, aEventID,
			gc_cMicroSecond,aMicroSecond,
			gc_cTime,TimeConvertString(aDatetime));
	}
	
	if (apDB->More(TRUE))
	{
		lnWaveOffset = 0;

		CWaveHead lsHead;
		lsHead.SetLength(4);

		for (int i=0;i<aChannNum;++i)
		{
			_pLockDynData->BeginRead();
			BEGIN_ERROR_HANDLE
				pDynChannParam = (CDynChannParam *)_vDynChann[i + aStartIndex];
			END_ERROR_HANDLE
				_pLockDynData->EndRead();

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

			int lnTotalSize = WaveAddHead(&apWave[lnWaveOffset],apCharValue[i]._iSmpNum,lsHead,lvWave);


			if(!(apDB->SetBuffer(pDynChannParam->GetSpecCharTypeField(-1).c_str(),(BYTE *)&(lvWave.front()),lnTotalSize)))
			{
				iRet=-4;
				goto QUIT_SAVE_DYN_SUD_DATA;
			}

			apDB->SetBuffer(pDynChannParam->GetSpecCharTypeField(-7).c_str(),(BYTE *)(apParam[i].pfCustomParom),sizeof(S_CustomParamData));

			lnWaveOffset += apCharValue[i]._iSmpNum;

			iRet++;
		}
	}

QUIT_SAVE_DYN_SUD_DATA:

	if (iRet>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	return iRet;
}

int CSudPlantInfo::SaveSudProcData( IDBInterFace * apDB, int aEventID, CBHDateTime aDateTime, int aMicrosecond, float* aCharVal, int aStartIndex, int aChannNum, float apRev[5] )
{
	ASSERT(apDB);
	ASSERT(!_sCompanyAlias.empty());


	int lnProcChannNum = _vProcChann.size();

	ASSERT(aStartIndex >= 0 && aChannNum > 0 && (aStartIndex + aChannNum) <= lnProcChannNum);

	GetRealRev( apRev, 5);


	CString lsDataDbName;
	lsDataDbName.Format("%s%s", _sCompanyAlias.c_str(), _sPlantInfo._cPlantNo);

	SwitchDbNameEx(apDB, lsDataDbName);

	int iRtn = 1;

	CString lsCondition;
	lsCondition.Format("[%s]=%d AND [%s]='%s' AND [%s]=%d ",
		gc_cEventID, aEventID,
		gc_cTime, TimeConvertString(aDateTime),
		gc_cMicroSecond, aMicrosecond);

	apDB->BeginTran();

	do {
		apDB->Cmd("select 1 from [%s] where %s", _sAlarmProcDataTblName.c_str(), lsCondition);
		if (!apDB->More())
		{
			CString lsColNames;
			CString lsColVals;


			for (int iChann = 0; iChann < lnProcChannNum; iChann++)
			{
				S_ChannInfo channInfo = this->_vProcChann[aStartIndex + iChann]->GetChannInfo();

				if (lsColNames.IsEmpty())
				{
					lsColNames.Format("[%s]", channInfo._cChannNo);
				}
				else
				{
					lsColNames.AppendFormat(",[%s]", channInfo._cChannNo);
				}

				if (lsColVals.IsEmpty())
				{
					lsColVals.Format("%.2f", aCharVal[iChann]);
				}
				else
				{
					lsColVals.AppendFormat(",%.2f", aCharVal[iChann]);
				}
			}

			apDB->Cmd("insert into [%s]", _sSudProcDataTblName.c_str());
			apDB->Cmd("(%s,%s,%s,%s, %s, %s,%s,%s,%s,%s)",
				gc_cEventID, gc_cDateTime, gc_cMicroSecond, gc_cSynFlag, 
				lsColNames, 
				gc_cRev, gc_cRev2, gc_cRev3, gc_cRev4, gc_cRev5); 
			apDB->Cmd("values(%d,'%s',%d,%d, %s, %d,%d,%d,%d,%d)",
				aEventID,TimeConvertString(aDateTime), aMicrosecond, 0, 
				lsColVals,
				apRev[0], apRev[1],apRev[2],apRev[3],apRev[4]);
			if (!(apDB->Exec()))
			{
				iRtn= -2;
				break;
			}
		}
		else
		{
			apDB->Cmd("UPDATE [%s] SET ", _sSudProcDataTblName.c_str());

			for (int iChann = 0; iChann < lnProcChannNum; iChann++)
			{
				S_ChannInfo channInfo = this->_vProcChann[aStartIndex + iChann]->GetChannInfo();

				apDB->Cmd(" [%s]=%.2f,",channInfo._cChannNo, aCharVal[iChann]);
			}

			apDB->Cmd(" [%s]=%.2f, ",gc_cRev,apRev[0]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev2,apRev[1]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev3,apRev[2]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev4,apRev[3]);
			apDB->Cmd(" [%s]=%.2f, ",gc_cRev5,apRev[4]);
			apDB->Cmd(" WHERE %s", lsCondition);
			if (!(apDB->Exec()))
			{
				iRtn = -2;
				break;
			}
		}
	} while(0);


	if (iRtn>0)
		apDB->CommitTran();
	else
		apDB->RollbackTran();

	return iRtn;
}

float CSudPlantInfo::GetVibDataWaveCoef( int iLen_,int iStartIndex_/*=0*/ )
{
	float lfCoef = 0.0;

	CVibChannParam *pVibChann = NULL;

	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iVibChannNum)||(iLen_<1)||(iStartIndex_<0))
		lfCoef = 0.0;
	else
	{
		
		_pLockVibData->BeginRead();
		try
		{
			for(int loop=0;loop<iLen_;++loop)
			{
				pVibChann=(CVibChannParam*)_vVibChann[iStartIndex_+loop];
				CWaveHead lsWaveHead = pVibChann->WaveHead();

				if(lsWaveHead.m_nMagicValue == WAVE_HEAD_MAGIC_NUMBER && lsWaveHead.m_pParam.size() > 0)
				{
					lfCoef = lsWaveHead.m_pParam[0];

				}
				else
				{
					lfCoef = 0.0;
				}


				++nCount;
				break;
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

		_pLockVibData->EndRead();
		
	}
	return lfCoef;
}

float CSudPlantInfo::GetDynDataWaveCoef( int iLen_,int iStartIndex_/*=0*/ )
{
	float lfCoef = 0.0;

	CDynChannParam *pDynChann = NULL;

	int nCount=0;
	if (((iLen_+iStartIndex_)> _sPlantInfo._iDynChannNum)||(iLen_<1)||(iStartIndex_<0))
		lfCoef = 0.0;
	else
	{

		_pLockDynData->BeginRead();
		try
		{
			for(int loop=0;loop<iLen_;++loop)
			{
				pDynChann=(CDynChannParam*)_vDynChann[iStartIndex_+loop];
				CWaveHead lsWaveHead = pDynChann->WaveHead();

				if(lsWaveHead.m_nMagicValue == WAVE_HEAD_MAGIC_NUMBER && lsWaveHead.m_pParam.size() > 0)
				{
					lfCoef = lsWaveHead.m_pParam[0];

				}
				else
				{
					lfCoef = 0.0;
				}


				++nCount;
				break;
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

		_pLockDynData->EndRead();

	}
	return lfCoef;
}