#include "StdAfx.h"
#include ".\dieselplant956.h"


extern "C" __declspec(dllexport) CDieselPlant* CreateObjectof956()
{
	return new CDieselPlant956();
}

CDieselPlant956::CDieselPlant956(void)
{
	m_fAlarmValue = 0;
	this->m_pWave = NULL;
	this->m_nWaveNum = 0;
}

CDieselPlant956::~CDieselPlant956(void)
{
	
}


int CDieselPlant956::JudgeVibDieselAlarm(float fCharValue_, CString strFolderName_,int &AlarmCylinderNo_,float& fDistance_)
{

	this->ClearData();

	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	float lfSimilaryCoef = GetSimilarityCoef(lstrPlantNo);

	std::vector<float> lvWaveData,lvSpectrumData;

	std::vector<float> lvMeanValue,lvVarValue;

	float lfMeanValue = 0;

	float lfVarValue = 0;

	CString lstrChannNo = this->m_stChannInfo._cUnit;

	//非缸体振动测点
	if( this->m_stChannInfo._iCylinderNo == 0 )
	{
		//速度测点
		if(lstrChannNo.CompareNoCase(_T("mm/s")) == 0)
		{
			std::vector<vector<float> > lvNormalFreqData;
			std::vector<float> lvFreqData;
			float lfResult = this->CalcSegWaveEnergy(lvFreqData,E_CALC_SPECTRUM);

			if(IsFloatZero(lfResult))
			{
				AlarmCylinderNo_ = -1;
				fDistance_ = fCharValue_;
				return E_NORMAL;
			}

			for(int n = 0; n < lvFreqData.size(); ++n)
			{
				lvFreqData[n] += fCharValue_/10.0;
			}
			
			if(this->ReadNormalSpectrumDataFromDisk(strFolderName_,lvNormalFreqData))
			{

				vector<float> lvMeanDis;

				float lfMinDis = 0xFFFFFFF;
				vector<float> lvMinDisValue;
				vector<float> lvDis;
				vector<float> lvMeanValue;
				
				int lnPos = 0;
				for(size_t t = 0; t < lvNormalFreqData.size(); ++t)
				{

					float lfDis = this->CalcDistance(lvFreqData,lvNormalFreqData[t]);

					lvDis.push_back(lfDis);

					if(lfMinDis > lfDis)
					{
						lfMinDis = lfDis;
						lvMinDisValue = lvNormalFreqData[t];
						lnPos = t;
					}


				}
				float lfMeanDis1 = this->CalcMeanValue(lvDis);
				lvDis.clear();
				for(size_t t = 0; t < lvNormalFreqData.size(); ++t)
				{

					if(t == lnPos)
					{
						continue;
					}
					float lfDis = this->CalcDistance(lvMinDisValue,lvNormalFreqData[t]);

					lvDis.push_back(lfDis);


				}

				float lfMeanDis2 = this->CalcMeanValue(lvDis);

				int lnAlarmDistance = this->GetAlarmDistance();

				if(lfMeanDis1 < lnAlarmDistance*lfMeanDis2 || lfMeanDis2 <= 0 )
				{
					if(lfMeanDis1 < 1.4*lfMeanDis2 && lfMeanDis1 > 0)
					{
						lvNormalFreqData.erase(lvNormalFreqData.begin());
						lvNormalFreqData.push_back(lvFreqData);
						this->WriteNormalSpectrumDataToDisk(strFolderName_,lvNormalFreqData);
					}

					return E_NORMAL;

				}
				else
				{
					if(fCharValue_ > 2)
					{

						if(lfMeanDis1 > 2*lnAlarmDistance*lfMeanDis2)
						{
							return E_FAULT_TYPE_OTHER_HIGH;
						}
						return E_FAULT_TYPE_OTHER_LOW;

					}


				}

			}
			else
			{
				lvNormalFreqData.push_back(lvFreqData);

				this->WriteNormalSpectrumDataToDisk(strFolderName_,lvNormalFreqData);
			}
		}
		else
		{
			return E_NORMAL;
		}

	}

	if(this->m_stChannInfo._iCylinderNo <= 0 || this->m_stChannInfo._iCylinderNo >= 30)
	{
		return E_NORMAL;
	}

	std::vector<vector<float> > lvNormalWaveData;

	//分段计算波形能量
	float lfResult = this->CalcSegWaveEnergy(lvWaveData,E_CALC_WAVE);

	if(IsFloatZero(lfResult))
	{
		AlarmCylinderNo_ = -1;
		fDistance_ = fCharValue_;
		return 0;
	}

	if(this->ReadNormalWaveDataFromDisk(strFolderName_,lvNormalWaveData))
	{
		float lfCoef = 0;

		for(size_t t = 0; t < lvNormalWaveData.size(); ++t)
		{
			float lfSimCoef = this->CalcDataSimilarity(lvWaveData,lvNormalWaveData[t]);

			vector<float> lvWaveData1;
			
			for(int t1 = lvWaveData.size()/2; t1 < lvWaveData.size();++t1)
			{
				lvWaveData1.push_back(lvWaveData[t1]);
			}
			for(int t1 = 0; t1 < lvWaveData.size()/2; ++t1)
			{
				lvWaveData1.push_back(lvWaveData[t1]);
			}

			float lfSimCoef1 = this->CalcDataSimilarity(lvWaveData1,lvNormalWaveData[t]);

			if(lfSimCoef < lfSimCoef1)
			{
				lfSimCoef = lfSimCoef1;
			}
			lfCoef += lfSimCoef;
		}

		lfCoef /= lvNormalWaveData.size();

		if(lfCoef < lfSimilaryCoef)
		{
			return E_FAULT_TYPE_OTHER_LOW;
		}
		else if(lfCoef > 0.8)
		{
			lvNormalWaveData.erase(lvNormalWaveData.begin());
			lvNormalWaveData.push_back(lvWaveData);
			WriteNormalWaveDataToDisk(strFolderName_,lvNormalWaveData);
		}
	}
	else
	{
		lvNormalWaveData.push_back(lvWaveData);
		this->WriteNormalWaveDataToDisk(strFolderName_,lvNormalWaveData);
	}

	return E_NORMAL;
}


BOOL CDieselPlant956::InitModelFilePath(CString strFolderName_)
{
	
	CString lstrGroupName = this->m_strGroup;
	CString lstrCompanyName = this->m_stPlantInfo._cCompany;
	CString lstrFactoryName = this->m_stPlantInfo._cFactory;
	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;
	CString lstrChannNo = this->m_stChannInfo._cChannNo;

	int nSplitePos = strFolderName_.Find("+");

	int lnRev = 0;
	int lnPower = 0;
	CString lstrFolderName;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lnPower = atoi(strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1));
	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerLowerBound = (lnPower/50)*50;
	int lnPowerUpperBound = lnPowerLowerBound + 49;

	lstrFolderName.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);

	CString lstrPath=::GetAppPath()+_T("AlarmData\\");


	const TCHAR * lpPathSplit = _T("\\");


	lstrPath.Append(lstrGroupName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrCompanyName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrFactoryName);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrPlantNo);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrChannNo);
	lstrPath.Append(lpPathSplit);

	lstrPath.Append(lstrFolderName);

	MakeSureFolderExist(lstrPath);

	this->SetModelFolderPath(lstrPath);

	return TRUE;
}


float CDieselPlant956::CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_)
{
	int lnDimension = GetStableModelDimension();

	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	if(nType_ == E_CALC_WAVE)
	{
		if(m_pWave && m_nTwoCyclePtNum > 0)
		{
			vfData = this->CalcDataSegEnergyPercentage(m_pWave,m_nTwoCyclePtNum,18);
			return 1;
		}
	}
	else
	{
		if(m_pWave && m_nWaveNum > 0)
		{
			int lnSampleFreq = this->m_nSmpFreq;
			
			float lfDf = 0;
			float lfStartFreq = 0;
			
			float *lpFreq = new float[m_nWaveNum];
			memset(lpFreq,0,sizeof(float)*m_nWaveNum);

			m_nWaveNum = GetNextPowPtNum(m_nWaveNum);
			int lnLines = this->m_nWaveNum/2.56;

			if(ZoomSpectrumAnalysis(m_pWave,m_nWaveNum,lnSampleFreq,lpFreq,lnLines,lfDf,lfStartFreq) != -1)
			{
				vfData.resize(lnLines);
				memcpy(&vfData.front(),lpFreq,lnLines*sizeof(float));
			}

			KD_DeletePoint(lpFreq);

			return 1;
		}
	}


	return 0;

}

CString CDieselPlant956::GetModelFolderPath()
{
	return this->m_strFolderPath;
}

void CDieselPlant956::SetModelFolderPath( const CString& strPath_ )
{
	this->m_strFolderPath = strPath_;
}


bool CDieselPlant956::SetWaveData( const vector<DATATYPE_WAVE>& vWave_ )
{
	this->m_vWave.clear();
	this->m_vWave.resize(vWave_.size());
	memcpy(&this->m_vWave.front(),&vWave_.front(),vWave_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant956::SetWaveData( float *pWave_ )
{
	this->m_pWave = pWave_;
	return NULL != this->m_pWave;
}


bool CDieselPlant956::SetWaveSegInfo( const vector<DATATYPE_WAVE>& vWaveSegInfo_ )
{
	this->m_vWaveSegInfo.clear();
	this->m_vWaveSegInfo.resize(vWaveSegInfo_.size());

	memcpy(&this->m_vWaveSegInfo.front(),&vWaveSegInfo_.front(),vWaveSegInfo_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant956::SetFlameOutCylinder( const vector<int>& vFlameOutCylinder_ )
{
	this->m_vFlameOutCylinder.clear();
	this->m_vFlameOutCylinder.resize(vFlameOutCylinder_.size());

	memcpy(&this->m_vFlameOutCylinder.front(),&vFlameOutCylinder_.front(),vFlameOutCylinder_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant956::SetChannInfo( const S_ChannInfo& sChannInfo_ )
{
	this->m_stChannInfo = sChannInfo_;
	return true;
}


bool CDieselPlant956::SetPlantInfo(  CString strGroup_,const S_PlantInfo& sPlantInfo_ )
{
	this->m_stPlantInfo = sPlantInfo_;
	this->m_strGroup    = strGroup_;
	return true;
}


int CDieselPlant956::JudgeDynDieselAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	return E_NORMAL;
}

int CDieselPlant956::JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return E_NORMAL;
}

bool CDieselPlant956::SetValveEnergy( float fValveEnergy_ )
{
	this->m_fValveEnergy = fValveEnergy_;
	return true;
}


int CDieselPlant956::ReadNormalWaveDataFromDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ )
{
	this->InitModelFilePath(strFolderName_);

	CString lstrFilePath = this->GetModelFolderPath();

	CString lstrDataPath;

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

			lstrDataFileName = lfileFind.GetFileName();
			lstrDataPath = lfileFind.GetFilePath();
			break;

		}
	}

	lfileFind.Close();

	int lnDataLen = 0;

	if(lstrDataFileName.GetLength() <= 0)
	{
		return FALSE;
	}


	CString lstrTmp = lstrDataFileName;

	int lnPos = lstrTmp.Find(".txt");

	if(lnPos > 0)
	{
		lstrTmp = lstrTmp.Left(lnPos);
		lnDataLen = atoi(lstrTmp);
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
	if(lnDataCount < 18)
	{
		return false;
	}

	vector<float> lvTmpData;

	for(size_t t = 1; t < lvData.size(); ++t)
	{
		lvTmpData.push_back(lvData[t]);

		if(lvTmpData.size() >= lnDataCount)
		{
			vWaveData_.push_back(lvTmpData);
			lvTmpData.clear();
		}

	}

	if(vWaveData_.size() < 30)
	{
		::DeleteFile(lstrDataPath);
		return FALSE;
	}

	return TRUE;
}

int CDieselPlant956::WriteNormalWaveDataToDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ )
{
	if(vWaveData_.size() <= 0)
	{
		return FALSE;
	}
	this->InitModelFilePath(strFolderName_);

	CString lstrFilePath = this->GetModelFolderPath();

	vector<float> lvTmpData;

	int lnSize = vWaveData_[0].size();

	lvTmpData.push_back(lnSize);

	for(size_t i = 0; i < vWaveData_.size(); ++i)
	{
		if(vWaveData_[i].size() > 0)
		{
			vWaveData_[i].resize(lnSize,0);
		}
		for(size_t j = 0; j < vWaveData_[i].size();++j)
		{
			lvTmpData.push_back(vWaveData_[i][j]);
		}
	}

	CString lstrLen;
	lstrLen.Format("\\%d",lvTmpData.size());

	lstrFilePath.Append(lstrLen);
	lstrFilePath.Append(".txt");

	ofstream lf(lstrFilePath,ios::binary);
	if(lf)
	{
		lf.write((char*)&lvTmpData.front(),lvTmpData.size()*sizeof(float));
	}

	lf.flush();
	lf.close();

	return true;
}


bool CDieselPlant956::SetWavePtNum( const int nWavePtNum_ )
{
	this->m_nWaveNum = nWavePtNum_;
	return this->m_nWaveNum > 0;
}

bool CDieselPlant956::SetTowCycleWavePtNum( const int nWavePtNum_ )
{
	this->m_nTwoCyclePtNum = nWavePtNum_;
	return this->m_nTwoCyclePtNum > 0;
}



int CDieselPlant956::ReadNormalSpectrumDataFromDisk( CString strFolderName_,vector<vector<float> >& vFreqData_ )
{
	this->InitModelFilePath(strFolderName_);

	CString lstrFilePath = this->GetModelFolderPath();

	CString lstrDataPath;

	CString lstrFileName;

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

			
			lstrDataFileName = lfileFind.GetFileName();
			lstrDataPath = lfileFind.GetFilePath();
			break;

		}
	}

	lfileFind.Close();

	int lnDataLen = 0;

	if(lstrDataFileName.GetLength() <= 0)
	{
		return FALSE;
	}


	CString lstrTmp = lstrDataFileName;

	int lnPos = lstrTmp.Find(".txt");

	if(lnPos > 0)
	{
		lstrTmp = lstrTmp.Left(lnPos);
		lnDataLen = atoi(lstrTmp);
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

	if(lnDataCount < 256)
	{
		return FALSE;
	}

	vector<float> lvTmpData;

	for(size_t t = 1; t < lvData.size(); ++t)
	{
		lvTmpData.push_back(lvData[t]);

		if(lvTmpData.size() >= lnDataCount)
		{
			vFreqData_.push_back(lvTmpData);
			lvTmpData.clear();
		}

	}

	if(vFreqData_.size() < 30)
	{
		::DeleteFile(lstrDataPath);
		return FALSE;
	}

	return TRUE;
}

void CDieselPlant956::ClearData()
{
	m_fAlarmValue = 0;
}

vector<float> CDieselPlant956::CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ )
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

float CDieselPlant956::CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ )
{
	float lfResult = 0;

	if(avfWave1_.size() <= 0 || avfWave2_.size() <= 0)
	{
		return 0;
	}	

	int lnWaveLen = avfWave1_.size() > avfWave2_.size()?avfWave2_.size():avfWave1_.size();


	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	if(GetCalcSimilarityType(lstrPlantNo) == 0)
	{
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
	}
	else
	{
		double ldblSum2X = .0;
		double ldblSum2Y = .0;

		double ldblSumXY = 0;

		double ldblCoeff = .0;

		for (int i=0;i<lnWaveLen;i++)
		{
			ldblSumXY += avfWave1_[i]*avfWave2_[i];
			ldblSum2X += avfWave1_[i]*avfWave1_[i];
			ldblSum2Y += avfWave2_[i]*avfWave2_[2];
		}

		lfResult = ldblSumXY/(sqrt(ldblSum2X)*sqrt(ldblSum2Y));
	}

	return lfResult;
}

float CDieselPlant956::CalcAverageSimilarity( const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_ )
{
	float lfResult = 0;

	vector<vector<float> > lvfNormalData;
	vector<float>  lvfSimilarity;

	if(eType_ == E_CALC_WAVE)
	{
		if(avfData_.size() != m_fNormalWaveData.size())
		{
			return lfResult;
		}
		if(m_fNormalWaveData.size() <= 0)
		{
			return lfResult;
		}
		for(size_t i = 0; i < m_fNormalWaveData[0].size(); i++)
		{
			vector<float> lvTmp;

			for(size_t j = 0; j < m_fNormalWaveData.size(); j++)
			{
				lvTmp.push_back(m_fNormalWaveData[j][i]);
			}
			lvfNormalData.push_back(lvTmp);
		}
	}
	else
	{
		if(avfData_.size() != m_fNormalSpectrumData.size())
		{
			return lfResult;
		}
		if(m_fNormalSpectrumData.size() <= 0)
		{
			return lfResult;
		}
		for(size_t i = 0; i < m_fNormalSpectrumData[0].size(); i++)
		{
			vector<float> lvTmp;

			for(size_t j = 0; j < m_fNormalSpectrumData.size(); j++)
			{
				lvTmp.push_back(m_fNormalSpectrumData[j][i]);
			}
			lvfNormalData.push_back(lvTmp);
		}
	}


	float lfTotal = 0;

	for(size_t i = 0; i < lvfNormalData.size(); i++)
	{
		lfResult = this->CalcDataSimilarity(avfData_,lvfNormalData[i]);

		vector<float> lvWaveMove360;
		int lnSize = avfData_.size();

		for(int t = lnSize/2; t < lnSize; ++t)
		{
			lvWaveMove360.push_back(avfData_[t]);
		}
		for(int t = 0; t < lnSize/2; t++)
		{
			lvWaveMove360.push_back(avfData_[t]);
		}

		float lfResult1 = this->CalcDataSimilarity(lvWaveMove360,lvfNormalData[i]);

		if(lfResult > lfResult1)
		{
			lvfSimilarity.push_back(lfResult);

			lfTotal += lfResult;
		}
		else
		{
			lvfSimilarity.push_back(lfResult1);

			lfTotal += lfResult1;
		}	

	}

	if(lvfSimilarity.size() <= 0)
	{
		return 0;
	}

	return lfTotal/lvfSimilarity.size();
}



int CDieselPlant956::JudgeMeanAndVarValueAlarm( const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_ )
{
	if(avfMeanValue_.size() != avfVarValue_.size() || avfMeanValue_.size() != avfDataValue_.size())
	{
		return 0;
	}

	int lnAlarm = 0;
	int lnNormal = 0;

	int lnUpdate = 0;

	for(size_t i = 0; i < avfMeanValue_.size(); i++)
	{
		if(fabs(avfDataValue_[i] - avfMeanValue_[i]) > 3*sqrtf(avfVarValue_[i]))
		{
			lnAlarm++;
		}
		else
		{
			lnNormal++;
		}

		if(fabs(avfDataValue_[i] - avfMeanValue_[i]) <=sqrtf(avfVarValue_[i]))
		{
			lnUpdate++;
		}
		CString lstr;

		lstr.Format("柴油机振动测点%s:%s分布判断结果%f,%f,%f\n",this->m_stPlantInfo._cPlantNo,this->m_stChannInfo._cChannNo,avfMeanValue_[i],avfDataValue_[i],sqrtf(avfVarValue_[i]));

		OutputDebugString(lstr);
	}

	
	if(lnAlarm >= lnNormal)
	{
		return 1;
	}
	else
	{
		if(lnUpdate > 0)
		{
			return -1;
		}
	}

	return 0;

}

vector<float> CDieselPlant956::CalcDataSegEnergyMeanValue( float *pData_,int nWaveNum_,int nSegNum_ )
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

	for(int i = 0; i < nSegNum_; i++)
	{
		float lfTmp = 0;
		for(int j = i*lnPtNumPerSeg; j < (i+1)*lnPtNumPerSeg; j++)
		{
			lfTmp += pData_[j];
		}

		lfTmp = lfTmp/lnPtNumPerSeg;

		lvfEnergyPercentage.push_back(lfTmp);
	}

	if(lvfEnergyPercentage.size() != nSegNum_)
	{
		lvfEnergyPercentage.clear();
		return lvfEnergyPercentage;
	}

	return lvfEnergyPercentage;
}	

bool CDieselPlant956::SetSampleFreq( const int nSampleFreq_ )
{
	this->m_nSmpFreq = nSampleFreq_;
	return true;
}

float CDieselPlant956::CalcMeanValue( const vector<float>& vData_ )
{
	if(vData_.size() <= 0)
	{
		return 0;
	}
	float lfMeanValue = 0;

	for(size_t i = 0; i < vData_.size(); i++)
	{
		lfMeanValue += vData_[i];
	}

	lfMeanValue = lfMeanValue/vData_.size();

	return lfMeanValue;
}

int CDieselPlant956::WriteNormalSpectrumDataToDisk( CString strFolderName_,vector<vector<float> >& vFreqData_ )
{
	if(vFreqData_.size() <= 0)
	{
		return FALSE;
	}
	this->InitModelFilePath(strFolderName_);

	CString lstrFilePath = this->GetModelFolderPath();

	vector<float> lvTmpData;

	int lnSize = vFreqData_[0].size();

	lvTmpData.push_back(lnSize);

	for(size_t i = 0; i < vFreqData_.size(); ++i)
	{
		if(vFreqData_[i].size() > 0)
		{
			vFreqData_[i].resize(lnSize,0);
		}
		for(size_t j = 0; j < vFreqData_[i].size();++j)
		{
			lvTmpData.push_back(vFreqData_[i][j]);
		}
	}

	CString lstrLen;
	lstrLen.Format("\\%d",lvTmpData.size());

	lstrFilePath.Append(lstrLen);
	lstrFilePath.Append(".txt");

	ofstream lf(lstrFilePath,ios::binary);
	if(lf)
	{
		lf.write((char*)&lvTmpData.front(),lvTmpData.size()*sizeof(float));
	}

	lf.flush();
	lf.close();

	return true;
}

