#include "StdAfx.h"
#include ".\Compressorplant.h"
#include "FixCommA\FixCommA.h"


extern "C" __declspec(dllexport) CDieselPlant* CreateObjectofCompressor()
{
	return new CCompressorPlant();
}

CCompressorPlant::CCompressorPlant(void)
{
	this->m_pWave = NULL;
}

CCompressorPlant::~CCompressorPlant(void)
{
	
}


int CCompressorPlant::JudgeVibDieselAlarm(float fCharValue_, CString strFolderName_,int &AlarmCylinderNo_,float& fDistance_)
{
	
	if(NULL == this->m_pWave  || this->m_nWaveNum <= 0 )
	{
		return E_NORMAL;
	}

	CString lstrUint = this->m_stChannInfo._cUnit;

	if( lstrUint.CompareNoCase("m/s2") == 0)
	{
		return JudgeAccChannAlarm(fCharValue_,strFolderName_,AlarmCylinderNo_,fDistance_);
	}
	else if(lstrUint.CompareNoCase("um") == 0)
	{
		return JudgeDisChannAlarm(fCharValue_,strFolderName_,AlarmCylinderNo_,fDistance_);
	}
	else
	{
		return E_NORMAL;
	}

	
}

BOOL CCompressorPlant::ReadModelDataFromDisk( const CString &strFolderName_)
{
	this->InitModelFilePath();

	CString lstrFilePath = this->GetModelFolderPath();

	CString lstrDataPath;

	CString lstrFileName;

	int nSplitePos = strFolderName_.Find("+");

	int lnRev = 0;
	int lnPower = 0;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lnPower = atoi(strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1));
	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerSpliteFactor = 20;

	int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

	lstrFileName.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);

	CFileFind lfileFind;

	lstrFilePath.Append("\\*.txt");

	BOOL lbFound = lfileFind.FindFile(lstrFilePath);

	int lnDimension = GetStableModelDimension();

	CString lstrDataFileName;

	while(lbFound)
	{
		lbFound = lfileFind.FindNextFile();
		if(lfileFind.IsDots())
		{
			continue;
		}
		if(!lfileFind.IsDirectory())
		{
			
			if(lfileFind.GetFileName().Find(lstrFileName) >= 0)
			{
				lstrDataFileName = lfileFind.GetFileName();
				lstrDataPath = lfileFind.GetFilePath();
				break;
			}

		}
	}

	lfileFind.Close();

	int lnDataLen = 0;

	if(lstrDataFileName.GetLength() <= 0)
	{
		return FALSE;
	}
	
	
	CString lstrTmp = lstrDataFileName;

	int lnPos = lstrTmp.ReverseFind('-');

	if(lnPos > 0)
	{
		lstrTmp = lstrTmp.Right(lstrTmp.GetLength() - lnPos - 1);
		lnPos = lstrTmp.Find(".txt");

		if(lnPos > 0)
		{
			lstrTmp = lstrTmp.Left(lnPos);
			lnDataLen = atoi(lstrTmp);
		}
	}

	if(lnDataLen <= 0)
	{
		return FALSE;
	}

	vector<float> lvData;

	lvData.resize(lnDataLen);

	ifstream lf(lstrDataPath,ios::binary);
	if(lf)
	{
		lf.read((char*)&lvData.front(),lnDataLen*sizeof(float));
	}

	lf.close();

	int lnDataCount = lvData[0];

	if(lnDataCount <= 256)
	{
		return FALSE;
	}

	vector<float> lvTmpData;

	for(size_t t = 1; t < lvData.size(); ++t)
	{
		lvTmpData.push_back(lvData[t]);

		if(lvTmpData.size() >= lnDataCount)
		{
			this->m_vFreqData.push_back(lvTmpData);
			lvTmpData.clear();
		}
		
	}

	if(this->m_vFreqData.size() < lnDimension)
	{
		::DeleteFile(lstrDataPath);
		return FALSE;
	}

	return TRUE;
}

BOOL CCompressorPlant::InitModelFilePath()
{
	
	CString lstrGroupName = this->m_strGroup;
	CString lstrCompanyName = this->m_stPlantInfo._cCompany;
	CString lstrFactoryName = this->m_stPlantInfo._cFactory;
	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;
	CString lstrChannNo = this->m_stChannInfo._cChannNo;

	CString lstrPath=::GetAppPath()+_T("AlarmData\\");

	CString lstrDirectory ;


	const TCHAR * lpPathSplit = _T("\\");


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

	MakeSureFolderExist(lstrPath);

	this->SetModelFolderPath(lstrPath);

	return TRUE;
}




CString CCompressorPlant::GetModelFolderPath()
{
	return this->m_strFolderPath;
}

void CCompressorPlant::SetModelFolderPath( const CString& strPath_ )
{
	this->m_strFolderPath = strPath_;
}




bool CCompressorPlant::SetWaveData( const vector<DATATYPE_WAVE>& vWave_ )
{
	this->m_vWave.clear();
	this->m_vWave.resize(vWave_.size());
	memcpy(&this->m_vWave.front(),&vWave_.front(),vWave_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CCompressorPlant::SetWaveData( float *pWave_ )
{
	this->m_pWave = pWave_;
	return NULL != this->m_pWave;
}


bool CCompressorPlant::SetWaveSegInfo( const vector<DATATYPE_WAVE>& vWaveSegInfo_ )
{
	this->m_vWaveSegInfo.clear();
	this->m_vWaveSegInfo.resize(vWaveSegInfo_.size());

	memcpy(&this->m_vWaveSegInfo.front(),&vWaveSegInfo_.front(),vWaveSegInfo_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CCompressorPlant::SetFlameOutCylinder( const vector<int>& vFlameOutCylinder_ )
{
	this->m_vFlameOutCylinder.clear();
	this->m_vFlameOutCylinder.resize(vFlameOutCylinder_.size());

	memcpy(&this->m_vFlameOutCylinder.front(),&vFlameOutCylinder_.front(),vFlameOutCylinder_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CCompressorPlant::SetChannInfo( const S_ChannInfo& sChannInfo_ )
{
	this->m_stChannInfo = sChannInfo_;
	return true;
}

bool CCompressorPlant::SetPlantInfo( CString strGroup_,const S_PlantInfo& sPlantInfo_)
{
	this->m_stPlantInfo = sPlantInfo_;
	this->m_strGroup    = strGroup_;
	return true;
}




int CCompressorPlant::JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}

int CCompressorPlant::JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}


bool CCompressorPlant::SetSampleFreq( const int nSampleFreq_ )
{
	this->m_nSmpFreq = nSampleFreq_;
	return true;
}

bool CCompressorPlant::SetValveEnergy( float fValveEnergy_ )
{
	this->m_fValveEnergy = fValveEnergy_;
	return true;
}

bool CCompressorPlant::SetWavePtNum( const int nWavePtNum_ )
{
	this->m_nWaveNum = nWavePtNum_;
	return true;
}

bool CCompressorPlant::SetTowCycleWavePtNum( const int nWavePtNum_ )
{
	this->m_nTwoCyclePtNum = nWavePtNum_;

	return true;
}

BOOL CCompressorPlant::WriteModelDataToDisk( const CString &strFolderName_ )
{
	if(this->m_vFreqData.size() <= 0)
	{
		return FALSE;
	}
	this->InitModelFilePath();

	CString lstrFilePath = this->GetModelFolderPath();

	CString lstrDataPath;

	CString lstrFileName;

	CString lstrWaveFile = lstrFilePath + "\\Wave";

	CString lstrWaveTime = lstrWaveFile;
    

	MakeSureFolderExist(lstrWaveFile);
	

	int nSplitePos = strFolderName_.Find("+");

	int lnRev = 0;
	int lnPower = 0;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lnPower = atoi(strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1));
	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerSpliteFactor = 20;

	int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

	lstrFileName.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);

	lstrFilePath.Append("\\");
	lstrFilePath.Append(lstrFileName);

	vector<float> lvTmpData;

	int lnSize = this->m_vFreqData[0].size();

	lvTmpData.push_back(lnSize);

	for(size_t i = 0; i < m_vFreqData.size(); ++i)
	{
		if(m_vFreqData[i].size() > 0)
		{
			m_vFreqData[i].resize(lnSize,0);
		}
		for(size_t j = 0; j < m_vFreqData[i].size();++j)
		{
			lvTmpData.push_back(m_vFreqData[i][j]);
		}
	}

	CString lstrLen;
	lstrLen.Format("-%d",lvTmpData.size());

	lstrFilePath.Append(lstrLen);
	lstrFilePath.Append(".txt");

	ofstream lf(lstrFilePath,ios::binary);
	if(lf)
	{
		lf.write((char*)&lvTmpData.front(),lvTmpData.size()*sizeof(float));
	}

	lf.flush();
	lf.close();

	
	lstrWaveFile.AppendFormat("\\%s-%d.txt",lstrFileName,this->m_nWaveNum+1);

	lstrWaveTime.AppendFormat("\\%s-time.txt",lstrFileName);

	int lnWaveDataSize = this->m_nWaveNum+1;

	lvTmpData.resize(lnWaveDataSize);

	for(int i = 0; i < lnWaveDataSize; ++i)
	{
		if(i == 0)
		{
			lvTmpData[i] = this->m_nSmpFreq;
		}
		else
		{
			lvTmpData[i] = this->m_pWave[i-1];
		}
	}

	try
	{
		lf.open(lstrWaveFile,ios::binary);
		lf.write((char*)&lvTmpData.front(),lvTmpData.size()*sizeof(float));
		lf.flush();
		lf.close();
	}
	catch (...)
	{

	}
	try
	{
		lf.open(lstrWaveTime);
		string lstrTime = this->m_stPlantInfo._firstRunT.Format();

		lf<<lstrTime;
		lf.flush();
		lf.close();
	}
	catch (...)
	{

	}
	

	return true;
}

float CCompressorPlant::CalcMeanValue( const vector<float>& avData_ )
{
	if(avData_.size() <= 0)
	{
		return 0;
	}
	float lfMeanValue = 0;

	for(size_t i = 0; i < avData_.size(); i++)
	{
		lfMeanValue += avData_[i];
	}

	lfMeanValue = lfMeanValue/avData_.size();

	return lfMeanValue;
}

float CCompressorPlant::CalcMinDistance( const vector<float>& vXData_,const vector<float>& vYData_ )
{
	float lfResult = 0;
	float lfResult1 = 0;
	float lfResult2 = 0;

	int lnLen = vXData_.size()/2;

	if(lnLen > vYData_.size()/2)
	{
		lnLen = vYData_.size()/2;
	}

	for(int i = 0; i < lnLen; i++)
	{
		lfResult += (vXData_[i]-vYData_[i])*(vXData_[i]-vYData_[i]);
	}

	for(int i = 0; i< lnLen-1; i++)
	{
		lfResult1 +=  (vXData_[i]-vYData_[i+1])*(vXData_[i]-vYData_[i+1]);
	}

	for(int i = 0; i< lnLen-1; i++)
	{
		lfResult2 +=  (vXData_[i+1]-vYData_[i])*(vXData_[i+1]-vYData_[i]);
	}

	if(lfResult > lfResult1)
	{
		lfResult = lfResult1;
	}
	if(lfResult > lfResult2)
	{
		lfResult = lfResult2;
	}

	return sqrtf(lfResult);
	

}

vector<float> CCompressorPlant::CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ )
{
	vector<float> lvfEnergyPercentage;

	if(NULL == pData_ || nWaveNum_ <= 0 || nSegNum_ <= 0)
	{
		return lvfEnergyPercentage;
	}

	int lnPtNumPerSeg = nWaveNum_/nSegNum_;

	if(lnPtNumPerSeg <= 0)
	{
		return lvfEnergyPercentage;
	}

	float lfTotal = 0;

	for(int i = 0; i < nSegNum_; i++)
	{
		float lfTmp = 0;
		for(int j = i*lnPtNumPerSeg; j < (i+1)*lnPtNumPerSeg; j++)
		{
			lfTmp += fabs(pData_[j]);
		}

		lvfEnergyPercentage.push_back(lfTmp);
		lfTotal += lfTmp;
	}

	if(lvfEnergyPercentage.size() != nSegNum_)
	{
		lvfEnergyPercentage.clear();
		return lvfEnergyPercentage;
	}

	for(size_t i = 0; i < lvfEnergyPercentage.size();i++)
	{
		lvfEnergyPercentage[i] = lvfEnergyPercentage[i]/lfTotal;
	}

	return lvfEnergyPercentage;
}

float CCompressorPlant::CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ )
{
	float lfResult = 0;

	if(avfWave1_.size() <= 0 || avfWave2_.size() <= 0)
	{
		return 0;
	}	

	int lnWaveLen = avfWave1_.size() > avfWave2_.size()?avfWave2_.size():avfWave1_.size();
	
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

	for (int i=0;i<lnWaveLen;i++)
	{
		ldblSumX += avfWave1_[i];
		ldblSumY += avfWave2_[i];

		ldblSumX2 += avfWave1_[i]*avfWave1_[i];
		ldblSumY2 += avfWave2_[i]*avfWave2_[i];

		ldblSumXY+= avfWave1_[i]*avfWave2_[i];
	}

	ldblSum2X = ldblSumX*ldblSumX;
	ldblSum2Y = ldblSumY*ldblSumY;

	ldblNum = ldblSumXY - (ldblSumX*ldblSumY)/lnWaveLen;
	ldblDen = (ldblSumX2-ldblSum2X/lnWaveLen)*(ldblSumY2-ldblSum2Y/lnWaveLen);

	ldblDen = sqrt(ldblDen);

	lfResult = ldblNum/ldblDen;
	
	return lfResult;
}
float CCompressorPlant::GetPowerSpliteFactor( CString strPlantNo_ )
{
	char pDir[1000] = {0};

	char pValue[100] = {0};

	::GetCurrentDirectory(1000,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	float lfRatio = 1.0;

	DWORD lnCount = GetPrivateProfileString("POWER_SPLITE",strPlantNo_,"",pValue,100,pDir);

	if(lnCount <= 0)
	{
		::WritePrivateProfileString("POWER_SPLITE",strPlantNo_,"1.0",pDir);
	}
	else
	{
		CString lstrRatio = pValue;
		lfRatio = atof(lstrRatio);
	}

	return lfRatio;
}

vector<float> CCompressorPlant::CalcSpecEigValue( const vector<float>& avfWave_,float fDf_,int nRev_ )
{
	vector<float> lvResult;

	if(nRev_ < 300 )
	{
		return lvResult;
	}

	float lfAnalysisFreq = fDf_*(avfWave_.size()-1);

	float lfHalfWorkRevFreq = nRev_/120.0;

	vector<float> lvTmpData;

	int lnStartPos = 0;

	float lfMeanValue = 0;

	for(int nOrder = 1; nOrder <= 14; nOrder++)
	{
		if(nOrder*lfHalfWorkRevFreq > lfAnalysisFreq)
		{
			lvResult.push_back(0);
			continue;
		}

		int lnEndPos = nOrder*lfHalfWorkRevFreq/fDf_ + 2;

		lvTmpData.clear();
		for(int n = lnStartPos; n < lnEndPos; n++)
		{
			if(n < avfWave_.size())
			{
				lvTmpData.push_back(avfWave_[n]);
			}
		}

		lfMeanValue = CalcMeanValue(lvTmpData);

		lvResult.push_back(lfMeanValue);

		lnStartPos = lnEndPos+1;

	}

	lvTmpData.clear();

	for(int n = lnStartPos; n < avfWave_.size(); n++)
	{
		lvTmpData.push_back(avfWave_[n]);
	}

	lfMeanValue = CalcMeanValue(lvTmpData);
	lvResult.push_back(lfMeanValue);

	return lvResult;

}

bool CCompressorPlant::JudgeAccChannAlarm( float fCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	int lnAnaLines = this->m_nWaveNum/2.56;
	float lfDf = 0;
	float lfStartFreq = 0;

	vector<float> lvFreqData(this->m_nWaveNum,0);

	vector<float> lvTempWave;
	lvTempWave.resize(this->m_nWaveNum);

	for(int n = 0; n < this->m_nWaveNum; n++)
	{
		lvTempWave[n] = this->m_pWave[n];
	}

	int lnResult = this->ZoomSpectrumAnalysis(&lvTempWave.front(),this->m_nWaveNum,this->m_nSmpFreq,&lvFreqData.front(),lnAnaLines,lfDf,lfStartFreq);

	if(lnResult == -1)
	{
		return E_NORMAL;
	}

	lvFreqData.resize(lnAnaLines);

	for(size_t t = 0; t < lvFreqData.size(); ++t)
	{
		if(t*lfDf > 10 && t*lfDf < 160)
		{
			lvFreqData[t] = lvFreqData[t]*1000.0/(2*PI*t*lfDf);
		}
		if(t*lfDf >= 160)
		{
			break;
		}
	}

	float lfFreqMean = this->CalcMeanValue(lvFreqData);
	float lfFreqMean1 = 0;

	if(this->ReadModelDataFromDisk(strFolderName_))
	{

		vector<float> lvMeanDis;

		float lfMinDis = 0xFFFFFFF;
		vector<float> lvMinDisValue;
		vector<float> lvDis;
		vector<float> lvMeanValue;
		vector<float> lvSimCoef;

		int lnPos = 0;

		for(size_t t = 0; t < m_vFreqData.size(); ++t)
		{

			lfFreqMean1 += this->CalcMeanValue(m_vFreqData[t]);

			float lfDis = this->CalcDistance(lvFreqData,m_vFreqData[t]);

			vector<float> lvSeg1,lvSeg2;
			int lnPerSegPtNum = 10;
			int lnSegNum = lvFreqData.size()/lnPerSegPtNum;

			lvSeg1 = this->CalcDataSegEnergyPercentage(&lvFreqData.front(),lvFreqData.size(),lnSegNum);

			lvSeg2 = this->CalcDataSegEnergyPercentage(&m_vFreqData[t].front(),m_vFreqData[t].size(),lnSegNum);


			float lfSimCoef = this->CalcDataSimilarity(lvSeg1,lvSeg2);

			lvDis.push_back(lfDis);
			lvSimCoef.push_back(lfSimCoef);

			if(lfMinDis > lfDis)
			{
				lfMinDis = lfDis;
				lvMinDisValue = m_vFreqData[t];
				lnPos = t;
			}
		}

		lfFreqMean1 /= m_vFreqData.size();

		float lfMeanDis1 = this->CalcMeanValue(lvDis);
		lvDis.clear();
		for(size_t t = 0; t < m_vFreqData.size(); ++t)
		{

			if(t == lnPos)
			{
				continue;
			}
			float lfDis = this->CalcDistance(lvMinDisValue,m_vFreqData[t]);

			lvDis.push_back(lfDis);

		}

		float lfMeanDis2 = this->CalcMeanValue(lvDis);

		float lfMeanSimCoef = this->CalcMeanValue(lvSimCoef);

		int lnAlarmDistance = this->GetAlarmDistance();

		if(lfMeanDis1 < lnAlarmDistance*lfMeanDis2 || lfMeanDis2 <= 0 )
		{
			if(lfMeanDis1 < 1.2*lfMeanDis2 && lfMeanDis1 > 0)
			{
				m_vFreqData.erase(m_vFreqData.begin());
				m_vFreqData.push_back(lvFreqData);
				this->WriteModelDataToDisk(strFolderName_);
			}

			return E_NORMAL;

		}
		else
		{
			if(fCharValue_ > 5 && lfFreqMean > 1.5*lfFreqMean1 && lfFreqMean1 > 0)
			{
				return E_FAULT_TYPE_OTHER_LOW;
			}
		}

	}
	else
	{
		if(fCharValue_ > 3.0)
		{
			this->m_vFreqData.push_back(lvFreqData);

			this->WriteModelDataToDisk(strFolderName_);
		}

	}
	return E_NORMAL;
}

bool CCompressorPlant::JudgeDisChannAlarm( float fCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	if(fCharValue_ < 5.0)
	{
		return E_NORMAL;
	}
	int lnAnaLines = this->m_nWaveNum/2.56;
	float lfDf = 0;
	float lfStartFreq = 0;

	vector<float> lvTempWave;
	lvTempWave.resize(this->m_nWaveNum);

	for(int n = 0; n < this->m_nWaveNum; n++)
	{
		lvTempWave[n] = this->m_pWave[n];
	}

	vector<float> lvFreqData(this->m_nWaveNum,0);

	int lnResult = this->ZoomSpectrumAnalysis(&lvTempWave.front(),this->m_nWaveNum,this->m_nSmpFreq,&lvFreqData.front(),lnAnaLines,lfDf,lfStartFreq);

	if(lnResult == -1 || lfDf > 10.0)
	{
		CString lstrMsg;

		lstrMsg.Format("柴油机振动测点%s最终判断结果2- %d,%f",this->m_stChannInfo._cChannNo,lnResult,lfDf);
		OutputDebugString(lstrMsg);
		return E_NORMAL;
	}

	lvFreqData.resize(lnAnaLines);

	int nSplitePos = strFolderName_.Find("+");

	int lnRev = 0;
	int lnPower = 0;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
	}

	float lfFreqMean = this->CalcMeanValue(lvFreqData);
	float lfFreqMean1 = 0;

	if(this->ReadModelDataFromDisk(strFolderName_))
	{

		vector<float> lvMeanDis;

		float lfMinDis = 0xFFFFFFF;
		vector<float> lvMinDisValue;
		vector<float> lvDis;
		vector<float> lvMeanValue;
		vector<float> lvSimCoef;

		int lnPos = 0;

		for(size_t t = 0; t < m_vFreqData.size(); ++t)
		{

			lfFreqMean1 += this->CalcMeanValue(m_vFreqData[t]);

			float lfDis = this->CalcMinDistance(lvFreqData,m_vFreqData[t]);

			vector<float> lvSeg1,lvSeg2;
			int lnPerSegPtNum = 10;
			int lnSegNum = lvFreqData.size()/lnPerSegPtNum;

			lvDis.push_back(lfDis);

			if(lfMinDis > lfDis)
			{
				lfMinDis = lfDis;
				lvMinDisValue = m_vFreqData[t];
				lnPos = t;
			}
		}

		lfFreqMean1 /= m_vFreqData.size();

		float lfMeanDis1 = this->CalcMeanValue(lvDis);
		lvDis.clear();
		for(size_t t = 0; t < m_vFreqData.size(); ++t)
		{

			if(t == lnPos)
			{
				continue;
			}
			float lfDis = this->CalcMinDistance(lvMinDisValue,m_vFreqData[t]);

			lvDis.push_back(lfDis);

		}

		float lfMeanDis2 = this->CalcMeanValue(lvDis);

		int lnAlarmDistance = this->GetAlarmDistance();

		if(lfMeanDis1 < 4.0*lfMeanDis2 || lfMeanDis2 <= 0 )
		{
			if(lfMeanDis1 < 1.2*lfMeanDis2 && lfMeanDis1 > 0)
			{
				bool lbShouldSave = true;

				for(int i = 0; i < m_vFreqData.size(); i++)
				{
					float lfDis = this->CalcMinDistance(lvFreqData,m_vFreqData[i]);
					if(lfDis > -0.000001 && lfDis < 0.000001)
					{
						lbShouldSave = false;
						break;
					}
				}
				
				if(lbShouldSave)
				{
					m_vFreqData.erase(m_vFreqData.begin());
					m_vFreqData.push_back(lvFreqData);
					
				}

				this->WriteModelDataToDisk(strFolderName_);

				
			}

			return E_NORMAL;

		}
		else
		{
			if(fCharValue_ > 5.0 && lfFreqMean > 1.5*lfFreqMean1 && lfFreqMean1 > 0)
			{
				if(ConfirmAlarm(fCharValue_,strFolderName_,AlarmCylinderNo_,fDistance_))
				{
					return E_FAULT_TYPE_OTHER_LOW;
				}
			}
		}

	}
	else
	{
		if(fCharValue_ > 5.0)
		{
			bool lbShouldSave = true;

			if(this->m_vFreqData.size() <= 0)
			{
				lbShouldSave = true;
			}
			else
			{
				for(int i = 0; i < m_vFreqData.size(); i++)
				{
					float lfDis = this->CalcMinDistance(lvFreqData,m_vFreqData[i]);
					if(lfDis > -0.000001 && lfDis < 0.000001)
					{
						lbShouldSave = false;
						break;
					}
				}
			}

			if(lbShouldSave)
			{
				this->m_vFreqData.push_back(lvFreqData);
				
			}
			this->WriteModelDataToDisk(strFolderName_);
			
		}

	}
	return E_NORMAL;
}

BOOL CCompressorPlant::ConfirmAlarm(float fCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	CString lstrIP;
	int lnPort;

	BOOL lbAlarm = TRUE;

	if(GetDiagnoseInterface(lstrIP,lnPort))
	{

		vector<float> lvWaveData(this->m_nWaveNum,0);

		for(size_t t = 0; t < this->m_nWaveNum; ++t)
		{
			lvWaveData[t] = this->m_pWave[t];
		}

		int lnLevel = fCharValue_ > m_stChannInfo._chAlarmParam._fAlarmHL*0.8?3:2;

		this->InitModelFilePath();

		CString lstrFilePath = this->GetModelFolderPath();

		CString lstrDataPath;

		CString lstrFileName;

		CString lstrWaveFile = lstrFilePath + "\\Wave";

		CString lstrWaveTime = lstrWaveFile;


		MakeSureFolderExist(lstrWaveFile);

		int nSplitePos = strFolderName_.Find("+");

		int lnRev = 0;
		int lnPower = 0;

		if(nSplitePos > 0)
		{
			lnRev = atoi(strFolderName_.Left(nSplitePos));
			lnPower = atoi(strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1));
		}

		int lnRevLowerBound = (lnRev/20)*20;
		int lnRevUpperBound = lnRevLowerBound + 19;

		int lnPowerSpliteFactor = 20;

		int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
		int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

		lstrFileName.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);
		lstrWaveFile.AppendFormat("\\%s*.txt",lstrFileName);
		CFileFind lfileFind;
		BOOL lbFound = lfileFind.FindFile(lstrWaveFile);

		int lnDimension = GetStableModelDimension();

		CString lstrDataFileName;

		while(lbFound)
		{
			lbFound = lfileFind.FindNextFile();
			if(lfileFind.IsDots())
			{
				continue;
			}
			if(!lfileFind.IsDirectory())
			{

				CString lstrFileTmp  = lfileFind.GetFileName();

				if(lstrFileTmp.Find(lstrFileName) >= 0 && lstrFileTmp.Find("time") < 0)
				{
					lstrDataFileName = lfileFind.GetFileName();
					lstrDataPath = lfileFind.GetFilePath();
					break;
				}

			}
		}

		lfileFind.Close();

		int lnDataLen = 0;

		if(lstrDataFileName.GetLength() <= 0)
		{
			return lbAlarm;
		}

		CString lstrTmp = lstrDataFileName;

		int lnPos = lstrTmp.ReverseFind('-');

		if(lnPos > 0)
		{
			lstrTmp = lstrTmp.Right(lstrTmp.GetLength() - lnPos - 1);
			lnPos = lstrTmp.Find(".txt");

			if(lnPos > 0)
			{
				lstrTmp = lstrTmp.Left(lnPos);
				lnDataLen = atoi(lstrTmp);
			}
		}

		if(lnDataLen <= 0)
		{
			return lbAlarm;
		}

		vector<float> lvData;
		lvData.resize(lnDataLen);

		ifstream lf(lstrDataPath,ios::binary);
		if(lf)
		{
			lf.read((char*)&lvData.front(),lnDataLen*sizeof(float));
		}

		lf.close();

		for(size_t t = 1; t < lvData.size(); ++t)
		{
			lvWaveData.push_back(lvData[t]);
		}

		CFixCommA *pFixCommA = new CFixCommA;
		BEGIN_ERROR_HANDLE
		if(pFixCommA->Link(lstrIP,lnPort))
		{
			pFixCommA->CreateHead(31001);
			pFixCommA->SetItem(FIELD_SYSTEM_COMPANY, this->m_stPlantInfo._cCompany);
			pFixCommA->SetItem(FIELD_SYSTEM_FACTORY, this->m_stPlantInfo._cFactory);
			pFixCommA->SetItem(FIELD_SERVICE_PLANT_TYPE, GE_MACHINETYPE_COMPR);
			pFixCommA->SetItem(FIELD_SYSTEM_PLANT, this->m_stPlantInfo._cPlantID);
			pFixCommA->SetItem(FIELD_SYSTEM_CHANN,this->m_stChannInfo._cChannID);
			pFixCommA->SetItem(FIELD_SYSTEM_TYPE, 1); //本地测试，不通过远程中间件
			pFixCommA->SetItem(FIELD_SERVICE_ALARMID, 0);
			pFixCommA->SetItem(FIELD_SERVICE_REV, lnRev);
			pFixCommA->SetItem(FIELD_SERVICE_OFF_TIMESTART, CBHDateTime::GetCurrentTimeEx());
			pFixCommA->SetItem(FIELD_SERVICE_ALARM_LEVEL, lnLevel);
			pFixCommA->SetItem(FIELD_SERVICE_SECTION_TYPE, 0); //lnLangType
			pFixCommA->SetItem(FIELD_SYSTEM_ALARM_HIGH,m_stChannInfo._chAlarmParam._fAlarmHH);
			pFixCommA->SetItem(FIELD_SYSTEM_ALARM_LOW,m_stChannInfo._chAlarmParam._fAlarmHL);

			pFixCommA->SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM, this->m_nWaveNum);
			pFixCommA->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char*)&lvWaveData.front(),(int)lvWaveData.size()*sizeof(float));
			pFixCommA->SetItem(FIELD_SYSTEM_SAMPLE_FREQ, this->m_nSmpFreq);
			pFixCommA->SetItem(FIELD_SERVICE_ALARM_DISCRIPTION,lstrFileName);


			if(pFixCommA->More())
			{
				int lnAlarmType = pFixCommA->GetLong(FIELD_SYSTEM_EVENT_TYPE);

				if(lnAlarmType > 0)
				{
					lbAlarm = TRUE;
				}
				else
				{
					lbAlarm = FALSE;
				}
			}

		}
		END_ERROR_HANDLE
		pFixCommA->Close();
		delete pFixCommA;
		
	}

	return lbAlarm;
}
