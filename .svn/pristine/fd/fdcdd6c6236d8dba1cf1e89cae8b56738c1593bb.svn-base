#include "StdAfx.h"
#include ".\dieselplant703.h"


extern "C" __declspec(dllexport) CDieselPlant* CreateObjectof703()
{
	return new CDieselPlant703();
}

CDieselPlant703::CDieselPlant703(void)
{
	m_fAlarmValue = 0;
	this->m_pWave = NULL;
	this->m_nWaveNum = 0;
}

CDieselPlant703::~CDieselPlant703(void)
{
	m_fNormalData.clear();
	m_fFlameOutData.clear();
	m_fCylinderKnockData.clear();
	m_fWattAbrasionData.clear();
	m_sFlameOutDataFileName.clear();
	m_sNormalDataFileName.clear();
	m_sCylinderKnockDataFileName.clear();
	m_sWattAbrasionDataFileName.clear();
}


int CDieselPlant703::JudgeVibDieselAlarm(float fCharValue_, CString strFolderName_,int &AlarmCylinderNo_,float& fDistance_)
{

	return JudgeVibChannAlarm(fCharValue_,strFolderName_,AlarmCylinderNo_,fDistance_);

	std::vector<float> lvFileData,lvData,lvOriginalCoord;

	std::map< CString,vector<float> > lmFileData;

	std::vector<CString> lvFileName;

	std::vector<float> lvTmpData;

	std::vector<int>   lvFireSequence;

	std::vector<CString> lvFolderName;

	CString lstrFilePath;

	float lfRadius = 0,lfDistance = 0;

	bool lbShouldJudgeAlarm = true;

	fDistance_ = fCharValue_;

	//分段计算波形能量
	float lfResult = this->CalcSegWaveEnergy(lvData);

	if(IsFloatZero(lfResult))
	{
		AlarmCylinderNo_ = -1;
		fDistance_ = fCharValue_;
		return 0;
	}

	//判断失火
	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	if(this->GetFireSequence(lstrPlantNo,lvFireSequence))
	{
		int lnResult = 0;

		float lfValveEnergy = this->CalcValveEnergy(lvData,lvFireSequence);

		if(GetFlameOutJudgeType() == 0)
		{
			lnResult = this->JudgeFlameOut(lvData,lvFireSequence,lfValveEnergy,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_FLAMEOUT;
			}
		}

		//用气阀能量做标准进行判断
		if(!IsFloatZero(lfValveEnergy))
		{
			lnResult = this->JudgeFlameOut(lvData,lvFireSequence,lfValveEnergy,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_FLAMEOUT;
			}

			lnResult = this->JudgeCylinderKnock(lvData,lvFireSequence,lfValveEnergy,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_CYLINDER_KNOCK;
			}

			lnResult = this->JudgeWattAbrasion(lvData,lvFireSequence,lfValveEnergy,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_WATT_ABRASION;
			}
		}

	}

	int lnDataCount = GetInitStableVectorSpaceSize();

	bool lbCreateFile = false;


	//读取数据文件所在文件目录
	if(!ReadFolderNameFromDisk(lstrFilePath,strFolderName_))
	{
		MakeSureFolderExist(lstrFilePath);
	}

	//读取所有数据文件名称
	if(ReadFileNameFromDisk(lstrFilePath,lvFileName))
	{
		for(unsigned int i = 0; i < lvFileName.size(); i++)
		{
			//读取每个数据文件数据
			if(ReadModelFileData(lvFileName[i],lvFileData))
			{
				lmFileData.insert(make_pair(lvFileName[i],lvFileData));

				for(unsigned int j = 0; j < lvFileData.size(); j++)
				{
					lvTmpData.push_back(lvFileData[j]);
				}

				//计算原点坐标
				float lfCoord = 0;
				for(unsigned int j = 0; j < lvFileData.size(); j++)
				{
					lfCoord += lvFileData[j];
				}
				lvOriginalCoord.push_back(lfCoord*1.0/lvFileData.size());

			}
			else
			{

				lbShouldJudgeAlarm = false;

				//数据不够，存到磁盘
				if(lvFileData.size() < lnDataCount)
				{
					if(lvData.size() > i)
					{
						PushDataIntoStableVectorSpace(lvFileData,lvData[i]);
						WriteDataToFile(lvFileName[i],lvFileData);
					}
				}
				else
				{
					while(lvFileData.size() > lnDataCount)
					{
						PopDataFromStableVectorSpace(lvFileData);
					}
					WriteDataToFile(lvFileName[i],lvFileData);
				}

			}

		}

	}
	else
	{
		//没有文件目录，建立文件目录
		lbShouldJudgeAlarm = false;

		lbCreateFile = true;

		CString lstrTmp,lstrTmpFile;

		for(unsigned int i = 0; i < lvData.size(); i++)
		{
			lvFileData.clear();
			lstrTmpFile = lstrFilePath;

			lstrTmp.Format("Part%d.txt",i+1);
			lstrTmpFile += "\\" + lstrTmp;

			PushDataIntoStableVectorSpace(lvFileData,lvData[i]);
			WriteDataToFile(lstrTmpFile,lvFileData);

		}

		lvFileData.clear();
		lstrTmpFile = lstrFilePath + "\\Total.txt";

		PushDataIntoStableVectorSpace(lvFileData,lfResult);
		WriteDataToFile(lstrTmpFile,lvFileData);

	}

	//计算半径 
	lfRadius = this->CalcRadius(lvTmpData,lvOriginalCoord);

	if(lbShouldJudgeAlarm)
	{	

		if(IsFloatZero(lfRadius))
		{
			return E_NORMAL;
		}

		lfDistance = this->CalcDistance(lvData,lvOriginalCoord);

		if(lfDistance > lfRadius * 1.5)
		{

			if(lfDistance > lfRadius * 2.0)
			{
				fDistance_ = lfDistance;
				return E_FAULT_TYPE_OTHER_HIGH;
			}
			fDistance_ = lfDistance;
			return E_FAULT_TYPE_OTHER_LOW;

		}
		else
		{
			//没有报警，往模型里推送数据
			int i = 0;
			for(map< CString,vector<float> >::iterator it = lmFileData.begin(); it != lmFileData.end(); ++it)
			{
				if(it->second.size() >= GetInitStableVectorSpaceSize())
				{
					if(lvData.size() > i)
					{
						this->PopDataFromStableVectorSpace(it->second);
						this->PushDataIntoStableVectorSpace(it->second,lvData[i]);
						this->WriteDataToFile(it->first,it->second);
					}
				}
				i++;
			}

			//波形总能量数据
			if(ReadFileNameFromDisk(lstrFilePath,lvFileName,false))
			{
				if(ReadModelFileData(lvFileName[0],lvFileData))
				{
					PopDataFromStableVectorSpace(lvFileData);
					PushDataIntoStableVectorSpace(lvFileData,lfResult);
					WriteDataToFile(lvFileName[0],lvFileData);
				}
			}
		}

	}
	else
	{
		if(!lbCreateFile)
		{
			if(ReadFileNameFromDisk(lstrFilePath,lvFileName,false))
			{
				if(!ReadModelFileData(lvFileName[0],lvFileData))
				{
					if(lvFileData.size() < lnDataCount)
					{
						PushDataIntoStableVectorSpace(lvFileData,lfResult);
						WriteDataToFile(lvFileName[0],lvFileData);
					}
					else
					{
						while(lvFileData.size() > lnDataCount)
						{
							PopDataFromStableVectorSpace(lvFileData);
						}
						WriteDataToFile(lvFileName[0],lvFileData);
					}

				}
			}
		}

	}

	AlarmCylinderNo_ = m_nAlarmCylinderNo;

	return E_NORMAL;
}

BOOL CDieselPlant703::ReadFolderNameFromDisk( CString & strFolderPath_,const CString &strFolderName_)
{
	this->InitModelFilePath();

	strFolderPath_ = this->GetModelFolderPath();

	CString lstrFolderName;

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

	int lnPowerLowerBound = (lnPower/50)*50;
	int lnPowerUpperBound = lnPowerLowerBound + 49;

	lstrFolderName.Format("%d-%d+%d-%d",lnRevLowerBound,lnRevUpperBound,lnPowerLowerBound,lnPowerUpperBound);

	strFolderPath_.Append("\\");
	strFolderPath_.Append(lstrFolderName);

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(strFolderPath_);

	lfileFind.Close();

	return lbFound;
}



BOOL CDieselPlant703::ReadFileNameFromDisk(const CString& strFolderPath_, vector<CString>& vFileName_,bool bSegWave/*=true*/,E_DIESEL_CALC_TYPE eType_/*=E_CALC_WAVE*/ )
{
	CFileFind lfileFind;
	vFileName_.clear();

	CString lstrFolderPath = strFolderPath_;
	lstrFolderPath.Append("\\*.txt");
	BOOL lbFound = lfileFind.FindFile(lstrFolderPath);

	map<CString,CString> lmFileName;

	map<CString,CString>::iterator it;

	int lnDimension = GetStableModelDimension();


	while(lbFound)
	{
		lbFound = lfileFind.FindNextFile();
		if(lfileFind.IsDots())
		{
			continue;
		}
		if(!lfileFind.IsDirectory())
		{
			if(bSegWave)
			{
				if(lfileFind.GetFileName().Find("Part") >= 0)
				{
					lmFileName.insert(make_pair(lfileFind.GetFileName(),lfileFind.GetFilePath()));
				}
			}
			else
			{
				if(lfileFind.GetFileName().Find("Total") >= 0)
				{
					vFileName_.push_back(lfileFind.GetFilePath());
				}
			}

		}
	}

	lfileFind.Close();


	if(bSegWave)
	{

		if(eType_ == E_CALC_WAVE)
		{
			lnDimension = 1;
		}

		for(int i = 0; i < lnDimension; i++)
		{
			CString lstrFile;
			lstrFile.Format("Part%d.txt",i+1);
			if((it = lmFileName.find(lstrFile)) != lmFileName.end())
			{
				vFileName_.push_back(it->second);
			}
		}
		if(vFileName_.size() <= 0)
		{
			for(size_t i = 0; i < lnDimension; i++)
			{
				CString lstrFile;

				lstrFile.Format("%s\\Part%d.txt",strFolderPath_,i+1);

				vFileName_.push_back(lstrFile);
			}
		}
		return (BOOL)(vFileName_.size() == lnDimension);
	}
	else
	{
		if(vFileName_.size() <= 0)
		{
			CString lstrFile;
			lstrFile = strFolderPath_ + "\\Total.txt";
			vFileName_.push_back(lstrFile);
		}
		return vFileName_.size() > 0;
	}


}

BOOL CDieselPlant703::InitModelFilePath()
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


float CDieselPlant703::CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_)
{
	int lnDimension = GetStableModelDimension();

	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	int lnCalcRange = GetCalcSegWaveRange(lstrPlantNo);

	if(m_pWave && m_nWaveNum > 0)
	{
		int lnLines = 1600;
		int lnSampleFreq = this->m_nSmpFreq;
		float lfDf = 0;
		float lfStartFreq = 0;
		float *lpFreq = new float[m_nWaveNum];
		memset(lpFreq,0,sizeof(float)*m_nWaveNum);

		m_nWaveNum = GetNextPowPtNum(m_nWaveNum);

		if(ZoomSpectrumAnalysis(m_pWave,m_nWaveNum,lnSampleFreq,lpFreq,lnLines,lfDf,lfStartFreq) != -1)
		{
			if(lpFreq)
			{
				lpFreq[0] = 0;
			}
		
			if(nType_ == E_CALC_WAVE)
			{
				vfData = this->CalcDataSegEnergyMeanValue(lpFreq,lnLines,1);
			}
			else
			{
				vfData = this->CalcDataSegEnergyPercentage(lpFreq,lnLines,lnDimension);
			}

		}

		KD_DeletePoint(lpFreq);

		return vfData.size() > 0;
	}



	/*if(nType_ == E_CALC_WAVE)
	{
		if(m_pWave && m_nTwoCyclePtNum > 0)
		{
			vfData = this->CalcDataSegEnergyPercentage(m_pWave,m_nTwoCyclePtNum,lnDimension);
			return 1;
		}
	}
	else
	{
		if(m_pWave && m_nWaveNum > 0)
		{
			int lnLines = 1600;
			int lnSampleFreq = 25600;
			float lfDf = 0;
			float lfStartFreq = 0;
			float *lpFreq = new float[m_nWaveNum];
			memset(lpFreq,0,sizeof(float)*m_nWaveNum);

			m_nWaveNum = GetNextPowPtNum(m_nWaveNum);

			if(ZoomSpectrumAnalysis(m_pWave,m_nWaveNum,lnSampleFreq,lpFreq,lnLines,lfDf,lfStartFreq) != -1)
			{
				CString lstr;
				lstr.Format(_T("柴油机振动测点%s分析线数%d"),this->m_stChannInfo._cChannNo,lnLines);
				OutputDebugString(lstr);
				
				vfData = this->CalcDataSegEnergyPercentage(lpFreq,lnLines,lnDimension);
			}

			KD_DeletePoint(lpFreq);

			return 1;
		}
	}*/


	return 0;
}

CString CDieselPlant703::GetModelFolderPath()
{
	return this->m_strFolderPath;
}

void CDieselPlant703::SetModelFolderPath( const CString& strPath_ )
{
	this->m_strFolderPath = strPath_;
}


BOOL CDieselPlant703::ChannWaveEnergyAlarm(const CString& astrPath_)
{
	//缸起始索引存放在自定义参数中
	float lfEnergy = 0;

	vector<CString> lvFileName;

	vector<float>   lvFileData;

	float lfResult = 0;

	if(m_vWaveSegInfo[0] < GD_CUSTOMPARAM_BUFFSIZE)
	{
		int lnCylinderNum = m_vWaveSegInfo[0]/2;

		int lnDataCount = m_vWaveSegInfo[lnCylinderNum];

		for(int nloop = 0; nloop < lnDataCount; nloop++)
		{
			lfEnergy += m_vWave[nloop] * m_vWave[nloop]/100.0;
		}
	}

	if(ReadFileNameFromDisk(astrPath_,lvFileName,false))
	{
		if(ReadModelFileData(lvFileName[0],lvFileData))
		{
			for(unsigned int i = 0; i < lvFileData.size(); i++)
			{
				lfResult += lvFileData[i];
			}

			lfResult = lfResult/lvFileData.size();

			if(abs(lfEnergy - lfResult) > lfResult * 0.5)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

float CDieselPlant703::CalcRadius( const vector<float>& avfData_,const vector<float>& avfOrgCoord_ )
{
	int  lnDimension = GetStableModelDimension();
	int  lnVectorSpaceSize = GetInitStableVectorSpaceSize();

	float lfRadius = 0;

	//计算平均半径
	if(avfData_.size() == lnDimension  * lnVectorSpaceSize)
	{
		std::vector<float> lvRadiusTmp;
		float lfRadiusTmp = 0;

		for(unsigned int j = 0; j < lnVectorSpaceSize; j++)
		{
			lfRadiusTmp = 0;

			for(unsigned int k = 0; k < lnDimension; k++)
			{
				lfRadiusTmp += (avfData_[j+k*lnVectorSpaceSize] - avfOrgCoord_[k])*(avfData_[j+k*lnVectorSpaceSize] - avfOrgCoord_[k]); 
			}

			lfRadiusTmp = sqrtf(lfRadiusTmp);

			lvRadiusTmp.push_back(lfRadiusTmp);

		}

		lfRadiusTmp = 0;

		for(unsigned int j = 0; j < lvRadiusTmp.size(); j++)
		{
			lfRadiusTmp += lvRadiusTmp[j];
		}
		lfRadius = lfRadiusTmp/lvRadiusTmp.size();
	}

	return lfRadius;
}

bool CDieselPlant703::SetWaveData( const vector<DATATYPE_WAVE>& vWave_ )
{
	this->m_vWave.clear();
	this->m_vWave.resize(vWave_.size());
	memcpy(&this->m_vWave.front(),&vWave_.front(),vWave_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant703::SetWaveData( float *pWave_ )
{
	this->m_pWave = pWave_;
	return NULL != this->m_pWave;
}


bool CDieselPlant703::SetWaveSegInfo( const vector<DATATYPE_WAVE>& vWaveSegInfo_ )
{
	this->m_vWaveSegInfo.clear();
	this->m_vWaveSegInfo.resize(vWaveSegInfo_.size());

	memcpy(&this->m_vWaveSegInfo.front(),&vWaveSegInfo_.front(),vWaveSegInfo_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant703::SetFlameOutCylinder( const vector<int>& vFlameOutCylinder_ )
{
	this->m_vFlameOutCylinder.clear();
	this->m_vFlameOutCylinder.resize(vFlameOutCylinder_.size());

	memcpy(&this->m_vFlameOutCylinder.front(),&vFlameOutCylinder_.front(),vFlameOutCylinder_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlant703::SetChannInfo( const S_ChannInfo& sChannInfo_ )
{
	this->m_stChannInfo = sChannInfo_;
	return true;
}

bool CDieselPlant703::SetPlantInfo( CString strGroup_,const S_PlantInfo& sPlantInfo_)
{
	this->m_stPlantInfo = sPlantInfo_;
	this->m_strGroup    = strGroup_;
	return true;
}


vector<int> CDieselPlant703::GetChannFireSequence( const vector<int>& avnSequence_)
{
	vector<int> lvFireSequenceA,lvFireSequenceB,lvFireSequence;

	for(int i = 0; i < avnSequence_.size(); i++)
	{
		int lnCylinderNo = avnSequence_[i];

		if(i%2 == 0)
		{
			lvFireSequenceA.push_back(lnCylinderNo);
		}
		else
		{
			lvFireSequenceB.push_back(lnCylinderNo);
		}
	}

	if(std::find(lvFireSequenceA.begin(),lvFireSequenceA.end(),this->m_stChannInfo._iCylinderNo) != lvFireSequenceA.end())
	{
		lvFireSequence.resize(lvFireSequenceA.size());
		memcpy(&lvFireSequence.front(),&lvFireSequenceA.front(),lvFireSequenceA.size()*sizeof(int));
	}
	else
	{
		lvFireSequence.resize(lvFireSequenceB.size());
		memcpy(&lvFireSequence.front(),&lvFireSequenceB.front(),lvFireSequenceB.size()*sizeof(int));
	}

	return lvFireSequence;

}

float CDieselPlant703::CalcValveEnergy(const vector<float>& avfData_,
											const vector<int>& avnSequence_)
{

	BOOL lbCylinderAndSensorEqual = IsCylinderAndSensorEqual();

	if(!lbCylinderAndSensorEqual)
	{
		return 0;
	}

	vector<int> lvFireSequence = avnSequence_;

	int lnCylinderNum = GetStableModelDimension();

	float lfStepLength = lnCylinderNum/4.0;

	int lnStartPos = -1;

	int lnValveNo = -1;

	for(int i = 0; i < lvFireSequence.size(); i++)
	{
		if(this->m_stChannInfo._iCylinderNo == lvFireSequence[i])
		{
			lnStartPos = i;
			break;
		}
	}

	if(lnStartPos < 0 || lnStartPos >= avfData_.size())
	{
		return 0;
	}

	if(lnStartPos - (int)lfStepLength < 0)
	{
		lnValveNo = lnStartPos - (int)lfStepLength + lnCylinderNum;
	}
	else
	{
		lnValveNo = lnStartPos - int(lfStepLength);
	}

	//吸气阀关闭能量超过点火能量的40%，使用吸气阀能量，否则使用点火能量
	if(avfData_.size() > lnValveNo)
	{
		if(avfData_[lnValveNo] > 0.4*avfData_[lnStartPos])
		{
			return avfData_[lnValveNo];
		}
		else
		{
			return avfData_[lnStartPos];
		}
		
	}
	

	return 0;


}

int CDieselPlant703::JudgeFlameOut( const vector<float>& avfData_,
										const vector<int>& avnSequence_,
										float fValveEnergy_,
										float& afDistance_ )
{
	vector<int> lvFireSequence = avnSequence_;

	if(GetFlameOutJudgeType() == 0)
	{
		if(std::find(m_vFlameOutCylinder.begin(), m_vFlameOutCylinder.end(), m_stChannInfo._iCylinderNo) != m_vFlameOutCylinder.end())
		{
			return 1;
		}
	}
	else
	{
		for(int i = 0; i < lvFireSequence.size(); i++)
		{
			if(lvFireSequence[i] == this->m_stChannInfo._iCylinderNo)
			{
				if(avfData_.size() > i && avfData_.size() == lvFireSequence.size())
				{
					if(fValveEnergy_ > avfData_[i]*10)
					{
						return 1;
					}
				}
			}
		}
	}

	
	return 0;
}

int CDieselPlant703::JudgeCylinderKnock( const vector<float>& avfData_,
											 const vector<int>& avnSequence_,
											 float fValveEnergy_,
											 float& afDistance_ )
{
	int lnPos = 0,lnPos1 = 0;

	int lnResult = 0;

	int lnCylinderNum = GetStableModelDimension();

	int StepLength = lnCylinderNum/2;

	afDistance_ = 0;

	std::vector<int> lvFireSequence = avnSequence_;

	//查找当前测点所在缸点火序号
	for(unsigned int i = 0; i < lvFireSequence.size(); i++)
	{
		if(lvFireSequence[i] == this->m_stChannInfo._iCylinderNo )
		{
			lnPos = i;
			break;
		}
	}

	if(lnPos >= avfData_.size())
	{
		return 0;
	}
	//计算同相位缸
	lnPos1 = (lnPos + StepLength)%lnCylinderNum;

	//当前缸及同相位缸能量大于气阀能量的0.9倍，判断为撞缸
	if(avfData_.size() > lnPos && avfData_.size() > lnPos1)
	{
		if(avfData_[lnPos] > 0.9*fValveEnergy_ && avfData_[lnPos1] > 0.9*fValveEnergy_)
		{
			this->m_nAlarmCylinderNo = avnSequence_[lnPos];
			return 2;
		}
	}

	return 0;
}

int CDieselPlant703::JudgeWattAbrasion( const vector<float>& avfData_,
											const vector<int>& avnSequence_,
											float fValveEnergy_,
											float& afDistance_ )
{

	std::vector<int> lvFireSequence = avnSequence_;

	int lnCylinderNum = GetStableModelDimension();

	float lfStepLength = lnCylinderNum/4.0;

	int lnStartPos = -1;

	int lnValveNo = -1;

	for(int i = 0; i < lvFireSequence.size(); i++)
	{
		if(this->m_stChannInfo._iCylinderNo == lvFireSequence[i])
		{
			lnStartPos = i;
			break;
		}
	}

	if(lnStartPos < 0)
	{
		return 0;
	}

	//向后寻找180度相位的缸
	if(lnStartPos + (int)lfStepLength > lnCylinderNum)
	{
		lnValveNo = lnStartPos + (int)lfStepLength - lnCylinderNum;
	}
	else
	{
		lnValveNo = lnStartPos + int(lfStepLength);
	}

	//当前缸后180度位置能量超过气阀能量的0.9倍，判断为大小头瓦磨损
	if(avfData_.size() > lnValveNo && avfData_.size() > lnStartPos)
	{
		if(avfData_[lnValveNo] > 0.9*fValveEnergy_)
		{
			this->m_nAlarmCylinderNo = avnSequence_[lnStartPos];
			return 1;
		}
	}

	return 0;
}

int CDieselPlant703::JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}

int CDieselPlant703::JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_)
{
	return 0;
}

bool CDieselPlant703::SetValveEnergy( float fValveEnergy_ )
{
	this->m_fValveEnergy = fValveEnergy_;
	return true;
}

float CDieselPlant703::GenNormalData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfNormalData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	float lfTotalEnergy = 0;

	int lnPos = 0;

	for(size_t i = 0; i < vfSegData_.size(); i++)
	{
		vfNormalData_[i] = vfSegData_[i];
		lfTotalEnergy += vfSegData_[i];

		if(vfFireSequence_[i] == this->m_stChannInfo._iCylinderNo)
		{
			lnPos = i;
		}
	}

	if(vfSegData_[lnPos] < lfTotalEnergy/vfSegData_.size())
	{
		vfNormalData_[lnPos] = lfTotalEnergy/vfSegData_.size();
	}

	return lfTotalEnergy;
}

float CDieselPlant703::GenFlameOutData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfFlameoutData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnPos = 0;

	float lfMinEnergy = vfSegData_[0];

	float lfTotalEnergy = 0;
	for(size_t i = 0; i < vfSegData_.size(); i++)
	{
		vfFlameoutData_[i] = vfSegData_[i];
		if(lfMinEnergy > vfSegData_[i])
		{
			lfMinEnergy = vfSegData_[i];
		}
		if(vfFireSequence_[i] == this->m_stChannInfo._iCylinderNo)
		{
			lnPos = i;
		}
		lfTotalEnergy += vfSegData_[i];
	}

	if(lfMinEnergy > lfTotalEnergy/(2*vfSegData_.size()))
	{
		lfMinEnergy = lfMinEnergy/2;
	}
	vfFlameoutData_[lnPos] = lfMinEnergy;

	return lfMinEnergy;

}

float CDieselPlant703::GenCylinderKnockData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfKnockData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnCylinderNum = GetStableModelDimension();

	int StepLength = lnCylinderNum/2;


	float lfMaxEnergy = vfSegData_[0];

	int lnPos = 0;

	//查找当前测点所在缸点火序号
	for(size_t i = 0; i < vfSegData_.size(); i++)
	{
		vfKnockData_[i] = vfSegData_[i];

		if(lfMaxEnergy < vfSegData_[i])
		{
			lfMaxEnergy = vfSegData_[i];
		}

		if(vfFireSequence_[i] == this->m_stChannInfo._iCylinderNo )
		{
			lnPos = i;
		}

	}

	//计算同相位缸
	lnPos = (lnPos + StepLength)%lnCylinderNum;

	vfKnockData_[lnPos] = lfMaxEnergy*1.5;


	return lfMaxEnergy;
}

float CDieselPlant703::GenWattAbrasionData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfWattAbrasionData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnCylinderNum = GetStableModelDimension();

	int lnStepLength = lnCylinderNum/4;

	float lfMaxEnergy = vfSegData_[0];

	int lnPos = 0;

	for(size_t i = 0; i < vfSegData_.size(); i++)
	{
		vfWattAbrasionData_[i] = vfSegData_[i];

		if(lfMaxEnergy < vfSegData_[i])
		{
			lfMaxEnergy = vfSegData_[i];
		}

		if(vfFireSequence_[i] == this->m_stChannInfo._iCylinderNo)
		{
			lnPos = i;
		}

	}

	//计算180相位位置
	int lnPos1 = 0, lnPos2 = 0;

	if((lnPos - lnStepLength - 1) < 0)
	{
		lnPos1 = (lnPos - lnStepLength  - 1 + lnCylinderNum)%lnCylinderNum;
	}
	else
	{
		lnPos1 = (lnPos - lnStepLength - 1)%lnCylinderNum;
	}

	if( (lnPos - lnStepLength) <0 )
	{
		lnPos2 = (lnPos - lnStepLength + lnCylinderNum)%lnCylinderNum;
	}
	else
	{
		lnPos2 = (lnPos - lnStepLength)%lnCylinderNum;
	}


	vfWattAbrasionData_[lnPos1] = lfMaxEnergy*1.5;

	if(GetKeyStartPosition() == 1)
	{
		vfWattAbrasionData_[lnPos2] = lfMaxEnergy;
	}
	return 0;
}

float CDieselPlant703::CalcNormalDistance( const vector<float>& vfSegData_ )
{
	float lfDistance = 0;

	vector<float> lvTotalDistance;

	if(vfSegData_.size() != m_fNormalData.size() || vfSegData_.size() <= 0)
	{
		return lfDistance;
	}

	for(size_t i = 0; i < m_fNormalData[0].size(); i++)
	{
	    lfDistance = 0;
		for(size_t j = 0; j < m_fNormalData.size(); j++)
		{
			lfDistance += (m_fNormalData[j][i]-vfSegData_[j])*(m_fNormalData[j][i]-vfSegData_[j]);
		}

		lfDistance = sqrtf(lfDistance);
		lvTotalDistance.push_back(lfDistance);
		
	}
		
	lfDistance = 0;
	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
	}

	return (lfDistance/lvTotalDistance.size());
}

float CDieselPlant703::CalcFlameOutdistance( const vector<float>& vfSegData_ )
{
	float lfDistance = 0;

	vector<float> lvTotalDistance;

	if(vfSegData_.size() != m_fFlameOutData.size() || vfSegData_.size() <= 0)
	{
		return lfDistance;
	}

	for(size_t i = 0; i < m_fFlameOutData[0].size(); i++)
	{
		lfDistance = 0;
		for(size_t j = 0; j < m_fFlameOutData.size(); j++)
		{
			lfDistance += (m_fFlameOutData[j][i]-vfSegData_[j])*(m_fFlameOutData[j][i]-vfSegData_[j]);
		}

		lfDistance = sqrtf(lfDistance);
		lvTotalDistance.push_back(lfDistance);

	}

	lfDistance = 0;
	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
	}

	return (lfDistance/lvTotalDistance.size());
}

float CDieselPlant703::CalcCylinderKnockDistance( const vector<float>& vfSegData_ )
{
	float lfDistance = 0;

	vector<float> lvTotalDistance;

	if(vfSegData_.size() != m_fCylinderKnockData.size() || vfSegData_.size() <= 0)
	{
		return lfDistance;
	}

	for(size_t i = 0; i < m_fCylinderKnockData[0].size(); i++)
	{
		lfDistance = 0;
		for(size_t j = 0; j < m_fCylinderKnockData.size(); j++)
		{
			lfDistance += (m_fCylinderKnockData[j][i]-vfSegData_[j])*(m_fCylinderKnockData[j][i]-vfSegData_[j]);
		}

		lfDistance = sqrtf(lfDistance);
		lvTotalDistance.push_back(lfDistance);

	}

	lfDistance = 0;
	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
	}

	return (lfDistance/lvTotalDistance.size());
}

float CDieselPlant703::CalcWattAbrasionDistance( const vector<float>& vfSegData_ )
{
	float lfDistance = 0;

	vector<float> lvTotalDistance;

	if(vfSegData_.size() != m_fWattAbrasionData.size() || vfSegData_.size() <= 0)
	{
		return lfDistance;
	}

	for(size_t i = 0; i < m_fWattAbrasionData[0].size(); i++)
	{
		lfDistance = 0;
		for(size_t j = 0; j < m_fWattAbrasionData.size(); j++)
		{
			lfDistance += (m_fWattAbrasionData[j][i]-vfSegData_[j])*(m_fWattAbrasionData[j][i]-vfSegData_[j]);
		}

		lfDistance = sqrtf(lfDistance);
		lvTotalDistance.push_back(lfDistance);

	}

	lfDistance = 0;
	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
	}

	return (lfDistance/lvTotalDistance.size());
}

int CDieselPlant703::JudgeVibChannAlarm( float afCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	this->ClearData();

	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	float lfSimilaryCoef = GetSimilarityCoef(lstrPlantNo);

	std::vector<float> lvData,lvNormalData,lvFlameOutData,lvCylinderKnockData,lvWattAbrasionData;

	std::vector<int>   lvFireSequence;

	std::map<float,int> lmAlarmType;

	std::map<float,int>::iterator it;

	std::vector<float> lvWaveData,lvSpectrumData;

	std::vector<float> lvMeanValue,lvVarValue;

	float lfMeanValue = 0;

	float lfVarValue = 0;

	CString lstrChannNo = this->m_stChannInfo._cUnit;

	if( this->m_stChannInfo._iCylinderNo == 0 )
	{
		if(lstrChannNo.CompareNoCase(_T("mm/s")) == 0)
		{
			if(ReadNormalCrankDataFromDisk(strFolderName_) <= 0)
			{
				this->UpdateNormalCrankData(afCharValue_);
				return E_NORMAL;
			}

			//分段计算波形能量
			lvWaveData.clear();

			float lfResult = this->CalcSegWaveEnergy(lvWaveData,E_CALC_WAVE);

			if(IsFloatZero(lfResult))
			{
				AlarmCylinderNo_ = -1;
				fDistance_ = afCharValue_;
				return E_NORMAL;
			}

			E_DIESEL_FAULT eFaultType = this->JudgeCrankDataAlarm(lvWaveData[0]);

			CString lstr;

			lstr.Format("柴油机振动测点%s判断结果%f,%d\n",this->m_stChannInfo._cChannNo,afCharValue_,eFaultType);

			OutputDebugString(lstr);

			return eFaultType;
		}
		else
		{
			return E_NORMAL;
		}

	}


	if(this->m_stChannInfo._iCylinderNo <= 0 || this->m_stChannInfo._iCylinderNo >= 100)
	{
		return E_NORMAL;
	}

	lvWaveData.clear();

	//分段计算波形能量
	float lfResult = this->CalcSegWaveEnergy(lvWaveData,E_CALC_WAVE);

	if(IsFloatZero(lfResult))
	{
		AlarmCylinderNo_ = -1;
		fDistance_ = afCharValue_;
		return E_NORMAL;
	}

	float lfResult1 = this->CalcSegWaveEnergy(lvSpectrumData,E_CALC_SPECTRUM);

	if(IsFloatZero(lfResult1))
	{
		AlarmCylinderNo_ = -1;
		fDistance_ = afCharValue_;
		return E_NORMAL;
	}


	int lnReadWave = this->ReadNormalWaveDataFromDisk(strFolderName_);
	int lnReadSpectrum = this->ReadNormalSpectrumDataFromDisk(strFolderName_);

	if(lnReadWave <= 0 || lnReadSpectrum <= 0)
	{
		this->UpdateNormalWaveData(lvWaveData);
		this->UpdateNormalFreqData(lvSpectrumData);
		return E_NORMAL;
	}

	lfResult1  = this->CalcAverageSimilarity(lvSpectrumData,E_CALC_SPECTRUM);

	for(size_t i = 0; i < m_fNormalWaveData.size(); i++)
	{
		lfMeanValue = 0;

		lfVarValue = 0;

		float lfResult = this->CalcWaveMeanAndStdValue(m_fNormalWaveData[i],lfMeanValue,lfVarValue);

		if(!IsFloatZero(lfResult))
		{
			lvMeanValue.push_back(lfMeanValue);
			lvVarValue.push_back(lfVarValue);
		}

	}

	int lnAlarm = this->JudgeMeanAndVarValueAlarm(lvMeanValue,lvVarValue,lvWaveData);


	CString lstr;

	lstr.Format("柴油机振动测点%s判断结果%f,%f\n",this->m_stChannInfo._cChannNo,lfResult,lfResult1);

	OutputDebugString(lstr);


	if( lfResult1 < lfSimilaryCoef)
	{
		return E_FAULT_TYPE_OTHER_LOW;
	}

	if(lnAlarm < 0)
	{
		this->UpdateNormalWaveData(lvWaveData);
	}

	if(lfResult1 >= 0.95)
	{
		this->UpdateNormalFreqData(lvSpectrumData);
	}

	return E_NORMAL;

}


int CDieselPlant703::UpdateNormalData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fNormalData.size() <= 0)
	{
		vector<float> lvTmp;
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			lvTmp.push_back(avfData_[i]);
			m_fNormalData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fNormalData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			while(m_fNormalData[i].size() >= lnDataSize)
			{
				m_fNormalData[i].erase(m_fNormalData[i].begin());
			}
			m_fNormalData[i].push_back(avfData_[i]);
		}

	}

	if(m_fNormalData.size() == m_sNormalDataFileName.size())
	{	
		for(size_t i = 0; i < m_fNormalData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			WriteDataToFile(m_sNormalDataFileName[i],m_fNormalData[i]);
		}
	}
	
	return 1;
}

int CDieselPlant703::UpdateFlameOutData( const vector<float>& avfData_ )
{
	
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fFlameOutData.size() <= 0)
	{
		vector<float> lvTmp;
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			lvTmp.push_back(avfData_[i]);
			m_fFlameOutData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fFlameOutData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			while(m_fFlameOutData[i].size() >= lnDataSize)
			{
				m_fFlameOutData[i].erase(m_fFlameOutData[i].begin());
			}
			m_fFlameOutData[i].push_back(avfData_[i]);
		}

	}

	if(m_fFlameOutData.size() == m_sFlameOutDataFileName.size())
	{	
		for(size_t i = 0; i < m_fFlameOutData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);
			WriteDataToFile(m_sFlameOutDataFileName[i],m_fFlameOutData[i]);
		}
	}

	return 1;
}	

int CDieselPlant703::UpdateCynlinderKnockData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fCylinderKnockData.size() <= 0)
	{
		vector<float> lvTmp;
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			lvTmp.push_back(avfData_[i]);
			m_fCylinderKnockData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fCylinderKnockData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			if(m_fCylinderKnockData[i].size() >= lnDataSize)
			{
				m_fCylinderKnockData[i].erase(m_fCylinderKnockData[i].begin());
			}
			m_fCylinderKnockData[i].push_back(avfData_[i]);
		}

	}

	if(m_fCylinderKnockData.size() == m_sCylinderKnockDataFileName.size())
	{	
		for(size_t i = 0; i < m_fCylinderKnockData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			WriteDataToFile(m_sCylinderKnockDataFileName[i],m_fCylinderKnockData[i]);
		}
	}

	return 1;
}

int CDieselPlant703::UpdateWattAbrasionData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fWattAbrasionData.size() <= 0)
	{
		vector<float> lvTmp;
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			lvTmp.push_back(avfData_[i]);
			m_fWattAbrasionData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fWattAbrasionData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			while(m_fWattAbrasionData[i].size() >= lnDataSize)
			{
				m_fWattAbrasionData[i].erase(m_fWattAbrasionData[i].begin());
			}
			m_fWattAbrasionData[i].push_back(avfData_[i]);
		}

	}

	if(m_fWattAbrasionData.size() == m_sWattAbrasionDataFileName.size())
	{	
		for(size_t i = 0; i < m_fWattAbrasionData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			WriteDataToFile(m_sWattAbrasionDataFileName[i],m_fWattAbrasionData[i]);
		}
	}

	return 1;
}

int CDieselPlant703::JudgeCrankVibChannAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	if(IsFloatZero(m_fAlarmValue))
	{
		m_fAlarmValue = lfCharValue_;

		return E_NORMAL;
	}
	
	if(lfCharValue_ > m_fAlarmValue * 1.5)
	{
		if(lfCharValue_ > m_fAlarmValue * 2)
		{
			return E_FAULT_TYPE_OTHER_HIGH;
		}
		return E_FAULT_TYPE_OTHER_LOW;
	}
	else if(lfCharValue_ < m_fAlarmValue)
	{
		m_fAlarmValue = sqrtf((lfCharValue_*lfCharValue_ + m_fAlarmValue*m_fAlarmValue)/2);
	}
	else
	{
		return E_NORMAL;
	}

	return E_NORMAL;
}

int CDieselPlant703::ReadNormalDataFromDisk(CString strFolderName_)
{
	//读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\Normal");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sNormalDataFileName,true))
	{
		for(size_t i = 0; i < m_sNormalDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			if(ReadModelFileData(m_sNormalDataFileName[i],lvModelData))
			{
				m_fNormalData.push_back(lvModelData);
			}
		}
	}


	return m_fNormalData.size() > 0;
}

int CDieselPlant703::ReadFlameOutDataFromDisk(CString strFolderName_)
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\FlameOut");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sFlameOutDataFileName,true))
	{
		for(size_t i = 0; i < m_sFlameOutDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			if(ReadModelFileData(m_sFlameOutDataFileName[i],lvModelData))
			{
				m_fFlameOutData.push_back(lvModelData);
			}
		}
	}


	return m_fFlameOutData.size() > 0;
}

int CDieselPlant703::ReadCylinderKnockDataFromDisk(CString strFolderName_)
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\CynliderKnock");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}


	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sCylinderKnockDataFileName,true))
	{
		for(size_t i = 0; i < m_sCylinderKnockDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);
			if(ReadModelFileData(m_sCylinderKnockDataFileName[i],lvModelData))
			{
				m_fCylinderKnockData.push_back(lvModelData);
			}
		}
	}


	return m_fCylinderKnockData.size() > 0;
}

int CDieselPlant703::ReadWattAbrasionDataFromDisk(CString strFolderName_)
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\WattAbrasion");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sWattAbrasionDataFileName,true))
	{
		for(size_t i = 0; i < m_sWattAbrasionDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			if(ReadModelFileData(m_sWattAbrasionDataFileName[i],lvModelData))
			{
				m_fWattAbrasionData.push_back(lvModelData);
			}
		}
	}


	return m_fWattAbrasionData.size() > 0;
}

int CDieselPlant703::ReadCrankAlarmDataFromDisk(CString strFolderName_)
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\CrankData");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<CString> lvFileName;

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,lvFileName,false))
	{
		for(size_t i = 0; i < lvFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);
			if(ReadModelFileData(lvFileName[i],lvModelData))
			{
				m_strNormalDataFileName = lvFileName[i];

				m_fAlarmValue = lvModelData[0];
			}
		}
	}
	return 1;
}

bool CDieselPlant703::SetWavePtNum( const int nWavePtNum_ )
{
	this->m_nWaveNum = nWavePtNum_;
	return this->m_nWaveNum > 0;
}

bool CDieselPlant703::SetTowCycleWavePtNum( const int nWavePtNum_ )
{
	this->m_nTwoCyclePtNum = nWavePtNum_;
	return this->m_nTwoCyclePtNum > 0;
}

int CDieselPlant703::UpdateNormalWaveData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fNormalWaveData.size() <= 0)
	{

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
			lvTmp.push_back(avfData_[i]);
			m_fNormalWaveData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fNormalWaveData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			while(m_fNormalWaveData[i].size() >= lnDataSize)
			{
				m_fNormalWaveData[i].erase(m_fNormalWaveData[i].begin());
			}
			m_fNormalWaveData[i].push_back(avfData_[i]);
		}

	}

	if(m_fNormalWaveData.size() == m_sNormalWaveDataFileName.size())
	{	
		for(size_t i = 0; i < m_fNormalWaveData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			WriteDataToFile(m_sNormalWaveDataFileName[i],m_fNormalWaveData[i]);
		}
	}

	return 1;
}

int CDieselPlant703::UpdateNormalFreqData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fNormalSpectrumData.size() <= 0)
	{

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
			lvTmp.push_back(avfData_[i]);
			m_fNormalSpectrumData.push_back(lvTmp);
		}
	}
	else
	{
		if(m_fNormalSpectrumData.size() != avfData_.size())
		{
			return 0;
		}

		for(size_t i = 0; i < avfData_.size(); i++)
		{
			while(m_fNormalSpectrumData[i].size() >= lnDataSize)
			{
				m_fNormalSpectrumData[i].erase(m_fNormalSpectrumData[i].begin());
			}
			m_fNormalSpectrumData[i].push_back(avfData_[i]);
		}

	}

	if(m_fNormalSpectrumData.size() == m_sNormalSpectrumDataFileName.size())
	{	
		for(size_t i = 0; i < m_fNormalSpectrumData.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			WriteDataToFile(m_sNormalSpectrumDataFileName[i],m_fNormalSpectrumData[i]);
		}
	}

	return 1;
}

int CDieselPlant703::ReadNormalWaveDataFromDisk( CString strFolderName_ )
{
	//读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\NormalWave");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sNormalWaveDataFileName,true,E_CALC_WAVE))
	{
		for(size_t i = 0; i < m_sNormalWaveDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			if(ReadModelFileData(m_sNormalWaveDataFileName[i],lvModelData))
			{
				m_fNormalWaveData.push_back(lvModelData);
			}
		}
	}

	if(m_fNormalWaveData.size() > 0 && m_fNormalWaveData[0].size()>=5)
	{
		return 1;
	}

	return 0;
}

int CDieselPlant703::ReadNormalSpectrumDataFromDisk( CString strFolderName_ )
{
	//读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\NormalSpectrum");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,m_sNormalSpectrumDataFileName,true,E_CALC_SPECTRUM))
	{
		for(size_t i = 0; i < m_sNormalSpectrumDataFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);

			if(ReadModelFileData(m_sNormalSpectrumDataFileName[i],lvModelData))
			{
				m_fNormalSpectrumData.push_back(lvModelData);
			}
		}
	}

	if(m_fNormalSpectrumData.size() > 0 && m_fNormalSpectrumData[0].size()>=5)
	{
		return 1;
	}

	return 0;
}

void CDieselPlant703::ClearData()
{
	m_fNormalData.clear();
	m_fFlameOutData.clear();
	m_fCylinderKnockData.clear();
	m_fWattAbrasionData.clear();
	m_sFlameOutDataFileName.clear();
	m_sNormalDataFileName.clear();
	m_sCylinderKnockDataFileName.clear();
	m_sWattAbrasionDataFileName.clear();
	m_sNormalWaveDataFileName.clear();
	m_sNormalSpectrumDataFileName.clear();
	m_fNormalWaveData.clear();
	m_fNormalSpectrumData.clear();
	m_strNormalDataFileName = "";
	m_fAlarmValue = 0;
}

vector<float> CDieselPlant703::CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ )
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

float CDieselPlant703::CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ )
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

float CDieselPlant703::CalcAverageSimilarity( const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_ )
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

		lvfSimilarity.push_back(lfResult);

		lfTotal += lfResult;

	}

	if(lvfSimilarity.size() <= 0)
	{
		return 0;
	}

	return lfTotal/lvfSimilarity.size();
}

int CDieselPlant703::UpdateNormalCrankData( const float afValue_ )
{
	int lnSpaceSize = GetInitStableVectorSpaceSize();

	if(m_fNormalCrankData.size() <= 0)
	{
		m_fNormalCrankData.push_back(afValue_);
		
	}
	else
	{
		while(m_fNormalCrankData.size() >= lnSpaceSize)
		{
			m_fNormalCrankData.erase(m_fNormalCrankData.begin());
		}

		m_fNormalCrankData.push_back(afValue_);
	}

	WriteDataToFile(m_strNormalDataFileName,m_fNormalCrankData);

	return 1;
}

int CDieselPlant703::ReadNormalCrankDataFromDisk( CString strFolderName_ )
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\CrankData");

	CFileFind lfileFind;

	BOOL lbFound = TRUE;

	lbFound = lfileFind.FindFile(lstrFolder);

	lfileFind.Close();

	if(!lbFound)
	{
		MakeSureFolderExist(lstrFolder);
	}

	vector<CString> lvFileName;

	vector<float>   lvModelData;

	if(ReadFileNameFromDisk(lstrFolder,lvFileName,false,E_CALC_WAVE))
	{
		for(size_t i = 0; i < lvFileName.size(); i++)
		{
			CMyCriticalSection lCriticalSection(&m_CriticalSection);
			ReadModelFileData(lvFileName[i],m_fNormalCrankData);
			m_strNormalDataFileName = lvFileName[i];

		}
	}
	return m_fNormalCrankData.size() >= 100;
}

E_DIESEL_FAULT CDieselPlant703::JudgeCrankDataAlarm( float afValue_ )
{
	
	int lnDistance = this->GetAlarmDistance();

	if(m_fNormalCrankData.size() >=30 )
	{
		float lfMeanValue = 0;
		float lfStdValue = 0;


		this->CalcWaveMeanAndStdValue(m_fNormalCrankData,lfMeanValue,lfStdValue);


		if(afValue_ - lfMeanValue > lfStdValue*lnDistance)
		{
			if(afValue_ > 2*lfStdValue*lnDistance)
			{
				return E_FAULT_TYPE_OTHER_HIGH;
			}

			return E_FAULT_TYPE_OTHER_LOW;
		}
		else
		{
			if(fabs(afValue_ - lfMeanValue) < 3*lfStdValue)
			{
				this->UpdateNormalCrankData(afValue_);
			}
		}
	}
	return E_NORMAL;
}



int CDieselPlant703::JudgeMeanAndVarValueAlarm( const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_ )
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
		if(fabs(avfDataValue_[i] - avfMeanValue_[i]) >= 6*sqrtf(avfVarValue_[i]))
		{
			lnAlarm++;
		}
		else
		{
			lnNormal++;
		}

		if(fabs(avfDataValue_[i] - avfMeanValue_[i]) < 3*sqrtf(avfVarValue_[i]))
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

	return -1;

}

vector<float> CDieselPlant703::CalcDataSegEnergyMeanValue( float *pData_,int nWaveNum_,int nSegNum_ )
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

bool CDieselPlant703::SetSampleFreq( const int nSampleFreq_ )
{
	this->m_nSmpFreq = nSampleFreq_;
	return true;
}