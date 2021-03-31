#include "StdAfx.h"
#include ".\GasTurbineplant.h"


extern "C" __declspec(dllexport) CDieselPlant* CreateObjectofGasTurbine()
{
	return new CGasTurbinePlant();
}

CGasTurbinePlant::CGasTurbinePlant(void)
{
	this->m_pWave = NULL;
}

CGasTurbinePlant::~CGasTurbinePlant(void)
{

}


int CGasTurbinePlant::JudgeVibDieselAlarm(float fCharValue_, CString strFolderName_,int &AlarmCylinderNo_,float& fDistance_)
{

	if(NULL == this->m_pWave  || this->m_nWaveNum <= 0 || fCharValue_ < 2.0)
	{
		return E_NORMAL;
	}

	CString lstrChannUint = this->m_stChannInfo._cUnit;

	if(lstrChannUint.CompareNoCase("mm/s") == 0)
	{
		return E_NORMAL;
	}

	int lnAnaLines = this->m_nWaveNum/2.56;
	float lfDf = 0;
	float lfStartFreq = 0;

	vector<float> lvFreqData(this->m_nWaveNum,0);

	int lnResult = this->ZoomSpectrumAnalysis(this->m_pWave,this->m_nWaveNum,this->m_nSmpFreq,&lvFreqData.front(),lnAnaLines,lfDf,lfStartFreq);

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
			if(lfMeanDis1 < 1.1*lfMeanDis2 && lfMeanDis1 > 0)
			{
				m_vFreqData.erase(m_vFreqData.begin());
				m_vFreqData.push_back(lvFreqData);
				this->WriteModelDataToDisk(strFolderName_);
			}

			return E_NORMAL;

		}
		else
		{
			if(fCharValue_ > 5 && lfFreqMean > lfFreqMean1 && lfFreqMean1 > 0)
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

BOOL CGasTurbinePlant::ReadModelDataFromDisk( const CString &strFolderName_)
{
	this->InitModelFilePath();

	CString lstrFilePath = this->GetModelFolderPath();

	CString lstrDataPath;

	CString lstrFileName;

	int nSplitePos = strFolderName_.Find("+");

	CString lstrWorkStatus = strFolderName_;

	int lnRev = 0;
	int lnPower = 0;
	int lnPower1 = 0;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lstrWorkStatus = strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1);

		nSplitePos = lstrWorkStatus.Find("+");

		if(nSplitePos > 0)
		{
			lnPower = atoi(lstrWorkStatus.Left(nSplitePos));

			lnPower1 = atoi(lstrWorkStatus.Right(lstrWorkStatus.GetLength() - nSplitePos - 1));
		}

	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerSpliteFactor = GetPowerSpliteFactor(this->m_stPlantInfo._cPlantNo);

	int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

	int lnPower1LowerBound = (lnPower1/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPower1UpperBound = lnPower1LowerBound + lnPowerSpliteFactor - 1;



	lstrFileName.Format("%d-%d+%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound,lnPower1LowerBound,lnPower1UpperBound);

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

BOOL CGasTurbinePlant::InitModelFilePath()
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




CString CGasTurbinePlant::GetModelFolderPath()
{
	return this->m_strFolderPath;
}

void CGasTurbinePlant::SetModelFolderPath( const CString& strPath_ )
{
	this->m_strFolderPath = strPath_;
}




bool CGasTurbinePlant::SetWaveData( const vector<DATATYPE_WAVE>& vWave_ )
{
	this->m_vWave.clear();
	this->m_vWave.resize(vWave_.size());
	memcpy(&this->m_vWave.front(),&vWave_.front(),vWave_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CGasTurbinePlant::SetWaveData( float *pWave_ )
{
	this->m_pWave = pWave_;
	return NULL != this->m_pWave;
}


bool CGasTurbinePlant::SetWaveSegInfo( const vector<DATATYPE_WAVE>& vWaveSegInfo_ )
{
	this->m_vWaveSegInfo.clear();
	this->m_vWaveSegInfo.resize(vWaveSegInfo_.size());

	memcpy(&this->m_vWaveSegInfo.front(),&vWaveSegInfo_.front(),vWaveSegInfo_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CGasTurbinePlant::SetFlameOutCylinder( const vector<int>& vFlameOutCylinder_ )
{
	this->m_vFlameOutCylinder.clear();
	this->m_vFlameOutCylinder.resize(vFlameOutCylinder_.size());

	memcpy(&this->m_vFlameOutCylinder.front(),&vFlameOutCylinder_.front(),vFlameOutCylinder_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CGasTurbinePlant::SetChannInfo( const S_ChannInfo& sChannInfo_ )
{
	this->m_stChannInfo = sChannInfo_;
	return true;
}

bool CGasTurbinePlant::SetPlantInfo( CString strGroup_,const S_PlantInfo& sPlantInfo_)
{
	this->m_stPlantInfo = sPlantInfo_;
	this->m_strGroup    = strGroup_;
	return true;
}




int CGasTurbinePlant::JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}

int CGasTurbinePlant::JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}


bool CGasTurbinePlant::SetSampleFreq( const int nSampleFreq_ )
{
	this->m_nSmpFreq = nSampleFreq_;
	return true;
}

bool CGasTurbinePlant::SetValveEnergy( float fValveEnergy_ )
{
	this->m_fValveEnergy = fValveEnergy_;
	return true;
}

bool CGasTurbinePlant::SetWavePtNum( const int nWavePtNum_ )
{
	this->m_nWaveNum = nWavePtNum_;
	return true;
}

bool CGasTurbinePlant::SetTowCycleWavePtNum( const int nWavePtNum_ )
{
	this->m_nTwoCyclePtNum = nWavePtNum_;

	return true;
}

BOOL CGasTurbinePlant::WriteModelDataToDisk( const CString &strFolderName_ )
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


	CString lstrWorkStatus = strFolderName_;

	int lnRev = 0;
	int lnPower = 0;
	int lnPower1 = 0;

	int nSplitePos = lstrWorkStatus.Find("+");

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lstrWorkStatus = strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1);

		nSplitePos = lstrWorkStatus.Find("+");

		if(nSplitePos > 0)
		{
			lnPower = atoi(lstrWorkStatus.Left(nSplitePos));

			lnPower1 = atoi(lstrWorkStatus.Right(lstrWorkStatus.GetLength() - nSplitePos - 1));
		}

	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerSpliteFactor = GetPowerSpliteFactor(this->m_stPlantInfo._cPlantNo);

	int lnPowerLowerBound = (lnPower/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPowerUpperBound = lnPowerLowerBound + lnPowerSpliteFactor - 1;

	int lnPower1LowerBound = (lnPower1/lnPowerSpliteFactor)*lnPowerSpliteFactor;
	int lnPower1UpperBound = lnPower1LowerBound + lnPowerSpliteFactor - 1;



	lstrFileName.Format("%d-%d+%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound,lnPower1LowerBound,lnPower1UpperBound);

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
			lvTmpData[i] = this->m_pWave[i-1]/10.0;
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

float CGasTurbinePlant::CalcMeanValue( const vector<float>& avData_ )
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

float CGasTurbinePlant::CalcMinDistance( const vector<float>& vXData_,const vector<float>& vYData_ )
{
	float lfDistance = 0;

	float lfMinDistance = 0;

	int lnDataLen = vXData_.size();

	if(lnDataLen > vYData_.size())
	{
		lnDataLen = vYData_.size();
	}

	if(lnDataLen <= 0)
	{
		return lfDistance;
	}


	vector<float> lvXDataL(lnDataLen,0);
	vector<float> lvXDataR(lnDataLen,0);

	lvXDataL[0] = vXData_[1];
	lvXDataR[0] = vXData_[0];

	for(int i = 0; i < lnDataLen; ++i)
	{
		lfDistance += (vXData_[i] - vYData_[i])*(vXData_[i] - vYData_[i]);

		if(i == 0 || i == lnDataLen - 1)
		{
			continue;
		}

		lvXDataL[i] = vXData_[i+1];
		lvXDataR[i] = vXData_[i-1];
	}
	lvXDataL[lnDataLen-1] = vXData_[lnDataLen-1];
	lvXDataR[lnDataLen-1] = vXData_[lnDataLen-2];

	lfMinDistance = sqrtf(lfDistance);

	lfDistance = 0;

	for(int i = 0; i < lnDataLen; ++i)
	{
		lfDistance += (lvXDataL[i] - vYData_[i])*(lvXDataL[i] - vYData_[i]);
	}

	lfDistance = sqrtf(lfDistance);

	if(lfMinDistance > lfDistance)
	{
		lfMinDistance = lfDistance;
	}

	lfDistance = 0;

	for(int i = 0; i < lnDataLen; ++i)
	{
		lfDistance += (lvXDataR[i] - vYData_[i])*(lvXDataR[i] - vYData_[i]);
	}

	lfDistance = sqrtf(lfDistance);

	if(lfMinDistance > lfDistance)
	{
		lfMinDistance = lfDistance;
	}


	return lfMinDistance;


}

vector<float> CGasTurbinePlant::CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ )
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

float CGasTurbinePlant::CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ )
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
float CGasTurbinePlant::GetPowerSpliteFactor( CString strPlantNo_ )
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