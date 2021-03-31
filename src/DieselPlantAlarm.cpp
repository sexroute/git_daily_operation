#include "stdafx.h"
#include "DieselPlantAlarm.h"


CDieselPlant::CDieselPlant(void)
{
}

CDieselPlant::~CDieselPlant(void)
{
}


int CDieselPlant::GetStableModelDimension()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"G_MODEL_DIMENSION"
		,6,
		cAppLocation);

	return lnData;
}

int CDieselPlant::IsCylinderAndSensorEqual()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"CYLINDER_SENSOR_EQUAL"
		,1,
		cAppLocation);

	return lnData;
}

int CDieselPlant::GetInitStableVectorSpaceSize()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"G_DATA_SIZE"
		,100,
		cAppLocation);

	return lnData;

}

int CDieselPlant::GetKeyStartPosition()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"KEY_START_POSITION"
		,1,
		cAppLocation);

	return lnData;

}
BOOL CDieselPlant::IsStableVectorSpaceFull(const vector<float>& fVectorSpace_)
{
	return (BOOL)(fVectorSpace_.size() >= GetInitStableVectorSpaceSize());
}

BOOL CDieselPlant::PushDataIntoStableVectorSpace(vector<float>& vfData_, float fData_)
{
	vfData_.push_back(fData_);
	return TRUE;
}

BOOL CDieselPlant::PopDataFromStableVectorSpace(vector<float>& vfData_)
{
	if(vfData_.size() > 0)
	{
		vfData_.erase(vfData_.begin());
	}
	return TRUE;
}

BOOL CDieselPlant::WriteDataToFile( const CString& strFileName_,const vector<float>& vfData_ )
{
	if(strFileName_.GetLength() < 3)
	{
		return FALSE;
	}

	ofstream lfile(strFileName_);

	if(!lfile)
	{
		return FALSE;
	}

 	for(unsigned int i = 0; i < vfData_.size(); i++)
 	{
 		lfile<<vfData_[i]<<" ";
 	}

	lfile.flush();

	lfile.close();

	return TRUE;
}

BOOL CDieselPlant::ReadModelFileData( const CString& strPath_,vector<float>& vfData_ )
{
	vfData_.clear();
	ifstream lfile(strPath_);
	if(!lfile)
	{
		return FALSE;
	}

	float lfData = 0;

	while(lfile>>lfData)
	{
		vfData_.push_back(lfData);
	}

	lfile.close();
	return vfData_.size() > 0;
	//return (BOOL)(vfData_.size() == GetInitStableVectorSpaceSize());
}

float CDieselPlant::CalcDistance( const vector<float>& vfXData_, const vector<float>& vfYData_ )
{
	if( vfXData_.size() <= 0 || vfYData_.size() <= 0 )
	{
		return 0;
	}

	float lfResult = 0;

	int lnLen = vfXData_.size();

	if(lnLen > vfYData_.size())
	{
		lnLen = vfYData_.size();
	}

	for(unsigned int i = 0; i < lnLen; i++)
	{
		lfResult += (vfXData_[i]-vfYData_[i])*(vfXData_[i]-vfYData_[i]);
	}

	lfResult = sqrtf(lfResult);

	return lfResult;
}


int CDieselPlant::GetFlameOutJudgeType()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"JUDGE_FLAME_OUT"
		,0,
		cAppLocation);

	return lnData;
}


BOOL CDieselPlant::MakeSureFolderExist( const CString &asAbsoluteFilePath )
{
	
	if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(asAbsoluteFilePath))
	{

		return TRUE;
	}


	CString lstrFilePath = asAbsoluteFilePath;

	lstrFilePath.TrimLeft(_T(" "));

	lstrFilePath.TrimLeft(_T("\r"));

	lstrFilePath.TrimLeft(_T("\n"));

	if (lstrFilePath.Find(_T(":\\"))!= 1 && lstrFilePath.Find(_T(":/"))!= 1)
	{
		return FALSE;
	}


	if (lstrFilePath[lstrFilePath.GetLength() - 1] != _T('\\'))
	{
		lstrFilePath = lstrFilePath + _T("\\");
	}

	CList<CString, CString&> llstPath;

	while (TRUE)
	{
		if (lstrFilePath.GetLength() < 3)
		{
			TRACE(_T("Error: lstrFilePath.GetLength() < 3\n"));

			return FALSE;
		}
		else
		{
			if (CreateDirectory(lstrFilePath, NULL))
			{
				break;
			}

			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return TRUE;
			}
			else if (GetLastError() != ERROR_PATH_NOT_FOUND)
			{
				TRACE(_T("Unknown Error: %d\n"), GetLastError());

				return  FALSE;
			}
		}
		llstPath.AddTail(lstrFilePath);
		lstrFilePath = lstrFilePath.Left(lstrFilePath.ReverseFind('\\'));
	}

	// 循环创建剩下的目录
	CString lstrPath;
	while (!llstPath.IsEmpty())
	{
		lstrPath = llstPath.RemoveTail();
		if (!CreateDirectory(lstrPath, NULL))
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return TRUE;
			}
			TRACE(_T("Unknown Error: %d\n"), GetLastError());
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CDieselPlant::GetFireSequence(CString strPlantNo_, vector<int>& vnSequence )
{
	char cAppLocation[500]={0};
	char sFireSequence[1024]={0};
	GetCurrentDirectory(500,cAppLocation);

	CString lstrSection = strPlantNo_ + "_FIRESEQUENCE";

	strcat(cAppLocation,"\\DseSvr.ini");

	GetPrivateProfileString("ALARM",lstrSection.GetBuffer(),"",sFireSequence,1023,cAppLocation);

	CString lstrSeq = sFireSequence;

	lstrSection.ReleaseBuffer();

	if(lstrSeq.Trim().GetLength() <= 0)
	{
		return FALSE;
	}

	char *pTmp=NULL;
	const char pcSep[]={","};
	string sChann;

	int lnSize = GetStableModelDimension();

	pTmp=strtok(sFireSequence,pcSep);
	while (NULL!=pTmp)
	{
		vnSequence.push_back(atoi(pTmp));
		pTmp=strtok(NULL,pcSep);
	}

	if(vnSequence.size() != lnSize * 2 && vnSequence.size() != lnSize)
	{
		return FALSE;
	}

	return TRUE;

}

int CDieselPlant::ZoomSpectrumAnalysis( float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_ )
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

	float fStopFreq_ = 0;
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

	
	if(fRealDf >((fStopFreq_ - fStartFreq_)/iLinesTmp))
	{
		bNeedZoom = true;
	}

	{//正常频谱分析

		//调用频谱计算修改后的函数,注：应该根据分析线数去，一定长度的波形点数作频谱分析，如：400线时，对应取400*2.56=1024个点作频谱分析
		pWaveData = new double[GD_MIDDLEWARE_WAVE_POINT_MAX];
		pSpec=new double[GD_MIDDLEWARE_WAVE_POINT_MAX];

		for(int i = 0; i < iSmpNums_; i++)
		{
			pWaveData[i] = pfWave_[i];
		}

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
			calcSpectral.SpectralMeasureMent(pWaveData,lnAnalyzeSmpNumber,pSpec,E_SpectrumType_PEAK);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
		}

		int iLines_ = ceil((double)lnAnalyzeSmpNumber/2.56);

		for(int i = 0; i < iLines_; i++)
		{
			pfSpec_[i] = pSpec[i];
		}
		nErrorCode=2;
		KD_DeletePoint(pWaveData);
		KD_DeletePoint(pSpec);
	}

	KD_DeletePoint(pfWaveData);
	return nErrorCode;
}

int CDieselPlant::GetNextPowPtNum(int nWavePtNum_)
{
	float lfPtNum = nWavePtNum_;

	for(int i = 20; i >= 8; i--)
	{
		int lnTmp = (int)pow(2,i);

		if(lnTmp <= nWavePtNum_)
		{
			lfPtNum = lnTmp;
			break;
		}
	}
	return lfPtNum;
}

float CDieselPlant::GetSimilarityCoef( CString strPlantNo_ )
{
	char cAppLocation[MAX_PATH]={0};

	GetCurrentDirectory(MAX_PATH,cAppLocation);

	CString lstrSection = strPlantNo_ + "_SIMICOEF";

	strcat(cAppLocation,"\\DseSvr.ini");

	int lnSimilarityCoef = ::GetPrivateProfileInt("ALARM",lstrSection.GetBuffer(),0,cAppLocation);

	lstrSection.ReleaseBuffer();

	if(lnSimilarityCoef <= 0)
	{
		CString lstrCoef;
		
		lnSimilarityCoef = 90;

		lstrCoef.Format("%d",lnSimilarityCoef);

		::WritePrivateProfileString("ALARM",lstrSection.GetBuffer(),lstrCoef,cAppLocation);

		lstrSection.ReleaseBuffer();
	}

	float lfSimilarityCoef = lnSimilarityCoef*1.0/100.0;

	return lfSimilarityCoef;
}

int CDieselPlant::GetCalcSimilarityType( CString strPlantNo_ )
{
	char cAppLocation[MAX_PATH]={0};

	GetCurrentDirectory(MAX_PATH,cAppLocation);

	CString lstrSection = strPlantNo_ + "_SIMITYPE";

	strcat(cAppLocation,"\\DseSvr.ini");

	int lnSimilarityCalcType = ::GetPrivateProfileInt("ALARM",lstrSection.GetBuffer(),-1,cAppLocation);

	lstrSection.ReleaseBuffer();

	if(lnSimilarityCalcType < 0)
	{
		CString lstrCoef;

		lnSimilarityCalcType = 0;

		lstrCoef.Format("%d",lnSimilarityCalcType);

		::WritePrivateProfileString("ALARM",lstrSection.GetBuffer(),lstrCoef,cAppLocation);

		lstrSection.ReleaseBuffer();
	}

	return lnSimilarityCalcType;
}

int CDieselPlant::GetCalcSegWaveRange( CString strPlantNo_ )
{
	char cAppLocation[MAX_PATH]={0};

	GetCurrentDirectory(MAX_PATH,cAppLocation);

	CString lstrSection = strPlantNo_ + "_RANGE";

	strcat(cAppLocation,"\\DseSvr.ini");

	int lnRange= ::GetPrivateProfileInt("ALARM",lstrSection.GetBuffer(),-1,cAppLocation);

	lstrSection.ReleaseBuffer();

	if(lnRange < 0)
	{
		CString lstrCoef;

		lnRange = 640;

		lstrCoef.Format("%d",lnRange);

		::WritePrivateProfileString("ALARM",lstrSection.GetBuffer(),lstrCoef,cAppLocation);

		lstrSection.ReleaseBuffer();
	}

	return lnRange;
}

int CDieselPlant::GetAlarmDistance()
{
	char cAppLocation[1024]={0};
	GetCurrentDirectory(1024,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"G_ALARM_DISTANCE"
		,-1,
		cAppLocation);

	if(lnData <= 0)
	{
		lnData = 10;

		::WritePrivateProfileString("ALARM","G_ALARM_DISTANCE","10",cAppLocation);
	}

	return lnData;
}

float CDieselPlant::CalcWaveMeanAndStdValue( const vector<float>& avfWave_,float& afMeanValue_,float& afVarValue_ )
{
	float lfMeanValue = 0;
	float lfVarValue = 0;
	if(avfWave_.size() <= 0)
	{
		return 0;
	}

	for(size_t i = 0; i < avfWave_.size(); i++)
	{
		lfMeanValue += avfWave_[i];
	}
	afMeanValue_ = lfMeanValue/avfWave_.size();

	for(size_t i = 0; i < avfWave_.size(); i++)
	{
		lfVarValue += (avfWave_[i] - afMeanValue_)*(avfWave_[i] - afMeanValue_);
	}

	afVarValue_ = sqrtf(lfVarValue/(avfWave_.size()-1));

	return 1;

}

bool CDieselPlant::GetDiagnoseInterface( CString& strIP_,int& nPort_ )
{
	char pDir[1024] = {0};

	char pValue[128] = {0};

	::GetCurrentDirectory(1000,pDir);

	strcat(pDir,_T("\\DseSvr.ini"));

	float lfRatio = 1.0;

	DWORD lnCount = GetPrivateProfileString("ALARM","JAVA_IP","",pValue,100,pDir);

	if(lnCount <= 0)
	{
		strIP_ = "127.0.0.1";
		::WritePrivateProfileString("ALARM","JAVA_IP","127.0.0.1",pDir);
	}
	else
	{
		strIP_ = pValue;
	}

	nPort_ = GetPrivateProfileInt("ALARM","JAVA_PORT",-1,pDir);
	if(nPort_ < 0)
	{
		nPort_ = 0;
		::WritePrivateProfileString("ALARM","JAVA_PORT","0",pDir);
	}

	if(nPort_ <= 0)
	{
		return false;
	}

	return true;
}