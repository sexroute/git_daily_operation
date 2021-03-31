#include "StdAfx.h"
#include ".\dieselplantwc.h"

extern "C" __declspec(dllexport) CDieselPlant* CreateObjectofWC()
{
	return new CDieselPlantWC();
}

CDieselPlantWC::CDieselPlantWC(void)
{
	m_fAlarmValue = 0;
	this->m_pWave = NULL;
	this->m_nWaveNum = 0;
}

CDieselPlantWC::~CDieselPlantWC(void)
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

int CDieselPlantWC::JudgeVibDieselAlarm(float fCharValue_, CString strFolderName_,int &AlarmCylinderNo_,float& fDistance_)
{

	return this->JudgeVibChannAlarm(fCharValue_,strFolderName_,AlarmCylinderNo_,fDistance_);
	std::vector<float> lvFileData,lvData,lvOriginalCoord;

	std::map< CString,vector<float> > lmFileData;

	std::vector<CString> lvFileName;

	std::vector<float> lvTmpData;

	std::vector<int>   lvFireSequence;

	CString lstrFilePath;

	float lfRadius = 0,lfDistance = 0;

	bool lbShouldJudgeAlarm = true;

	fDistance_ = fCharValue_;

	//分段计算波形能量
	float lfResult = this->CalcSegWaveEnergy(lvData,E_CALC_WAVE);

	if(IsFloatZero(lfResult))
	{
		AlarmCylinderNo_ = -1;
		fDistance_ = fCharValue_;
		return E_NORMAL;
	}

	//判断失火
	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	if(this->GetFireSequence(lstrPlantNo,lvFireSequence))
	{
		int lnResult = 0;

		if(GetFlameOutJudgeType() == 0)
		{
			lnResult = this->JudgeFlameOut(lvData,lvFireSequence,m_vFlameOutCylinder,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_FLAMEOUT;
			}
		}
		else
		{
			
			lnResult = this->JudgeFlameOut(lvData,lvFireSequence,m_vFlameOutCylinder,fDistance_);

			if(lnResult > 0)
			{
				return E_FAULT_TYPE_FLAMEOUT;
			}
		}
		
		lnResult = this->JudgeCylinderKnock(lvData,lvFireSequence,fDistance_);

		if(lnResult > 0)
		{
			return E_FAULT_TYPE_CYLINDER_KNOCK;
		}


		lnResult = this->JudgeWattAbrasion(lvData,lvFireSequence,fDistance_);

		if(lnResult > 0)
		{
			return E_FAULT_TYPE_WATT_ABRASION;
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

			if(this->ChannWaveEnergyAlarm(lstrFilePath))
			{

				if(lfDistance > lfRadius * 2.0)
				{
					fDistance_ = lfDistance;
					return E_FAULT_TYPE_OTHER_HIGH;
				}
				fDistance_ = lfDistance;
				return E_FAULT_TYPE_OTHER_LOW;
			}


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

BOOL CDieselPlantWC::ReadFolderNameFromDisk( CString & strFolderPath_,const CString &strFolderName_)
{
	this->InitModelFilePath();

	strFolderPath_ = this->GetModelFolderPath();

	CString lstrFolderName;

	int nSplitePos = strFolderName_.Find("+");

	int lnRev = 0;
	float lfPower = 0;

	if(nSplitePos > 0)
	{
		lnRev = atoi(strFolderName_.Left(nSplitePos));
		lfPower = atof(strFolderName_.Right(strFolderName_.GetLength() - nSplitePos - 1));
	}

	int lnRevLowerBound = (lnRev/20)*20;
	int lnRevUpperBound = lnRevLowerBound + 19;

	int lnPowerLowerBound = (lfPower/50)*50;
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



BOOL CDieselPlantWC::ReadFileNameFromDisk(const CString& strFolderPath_, vector<CString>& vFileName_,bool bSegWave/*=true*/,E_DIESEL_CALC_TYPE eType_ /*= E_CALC_WAVE*/ )
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
		return vFileName_.size() > 0;
	}


}

BOOL CDieselPlantWC::InitModelFilePath()
{
	
	CString lstrGroupName = m_strGroup;
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


float CDieselPlantWC::CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_)
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
			vfData = this->CalcDataSegEnergyPercentage(m_pWave,m_nTwoCyclePtNum,6);
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
				vfData = this->CalcDataSegEnergyPercentage(lpFreq,lnLines,lnDimension);
			}

			KD_DeletePoint(lpFreq);

			return 1;
		}
	}*/
	
	
	return 0;
	
}

CString CDieselPlantWC::GetModelFolderPath()
{
	return this->m_strFolderPath;
}

void CDieselPlantWC::SetModelFolderPath( const CString& strPath_ )
{
	this->m_strFolderPath = strPath_;
}


BOOL CDieselPlantWC::ChannWaveEnergyAlarm(const CString& astrPath_)
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

			if(fabs(lfEnergy - lfResult) > lfResult * 0.5)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

float CDieselPlantWC::CalcRadius( const vector<float>& avfData_,const vector<float>& avfOrgCoord_ )
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

//判断失火，能量最小的缸
int CDieselPlantWC::JudgeFlameOut( const vector<float>& avfData_,const vector<int>& avnSequence_,const vector<int>& avFlameOutCylinderNo_,float& afDistance_)
{

	BOOL lbCylinderAndSensorEqual = IsCylinderAndSensorEqual();

	int lnCylinderNo = this->m_stChannInfo._iCylinderNo;

	int lnResult = 0;

	if(GetFlameOutJudgeType() == 0)
	{
		if( lbCylinderAndSensorEqual )
		{
			if(std::find(avFlameOutCylinderNo_.begin(), avFlameOutCylinderNo_.end(), lnCylinderNo) != avFlameOutCylinderNo_.end())
			{
				this->m_nAlarmCylinderNo = lnCylinderNo;
				lnResult = 2;
			}
		}
		else
		{
			if(std::find(avFlameOutCylinderNo_.begin(), avFlameOutCylinderNo_.end(), lnCylinderNo) != avFlameOutCylinderNo_.end())
			{
				this->m_nAlarmCylinderNo = lnCylinderNo;
				lnResult = 2;
			}

			if(std::find(avFlameOutCylinderNo_.begin(), avFlameOutCylinderNo_.end(), lnCylinderNo + 1) != avFlameOutCylinderNo_.end())
			{
				this->m_nAlarmCylinderNo = lnCylinderNo + 1;
				lnResult = 2;
			}
		}

		return lnResult;
	}

	if(avfData_.size() <= 0 || avfData_.size() != avnSequence_.size())
	{
		return 0;
	}

	float lfMaxValue = 0;

	int lnMaxEnergyCylinder = 0;

	int lnPos = 0;

	for(unsigned int i = 0; i < avnSequence_.size(); i++)
	{
		if(lnCylinderNo == avnSequence_[i])
		{
			lnPos = i;
			break;
		}
	}

	for(unsigned int i = 0; i < avfData_.size(); i++)
	{
		if(lfMaxValue < avfData_[i])
		{
			lfMaxValue = avfData_[i];
			lnMaxEnergyCylinder = avnSequence_[i];
		}
	}

	if(lnMaxEnergyCylinder != lnCylinderNo)
	{
		if(avfData_[lnPos]*4.0 < lfMaxValue)
		{
			return 2;
		}
	}
	return 0;
}


int CDieselPlantWC::JudgeCylinderKnock( const vector<float>& avfData_,const vector<int>& avnSequence_,float& afDistance_ )
{
	int lnPos = 0,lnPos1 = 0;

	BOOL lbCylinderAndSensorEqual = IsCylinderAndSensorEqual();

	int lnCylinderNum = GetStableModelDimension();

	int StepLength = lnCylinderNum/2;

	afDistance_ = 0;

	vector<int> lvPos;

	//查找当前测点所在缸点火序号
	for(unsigned int i = 0; i < avnSequence_.size(); i++)
	{
		if( (lbCylinderAndSensorEqual && avnSequence_[i] == this->m_stChannInfo._iCylinderNo) ||
			(!lbCylinderAndSensorEqual && (avnSequence_[i] == this->m_stChannInfo._iCylinderNo || 
			avnSequence_[i] == this->m_stChannInfo._iCylinderNo + 1)))
		{
			lvPos.push_back(i);
		}
	}

	for(unsigned int nloop = 0; nloop < lvPos.size(); nloop++)
	{
		//寻找同相位的缸
		lnPos = lvPos[nloop];

		lnPos1 = (lnPos + StepLength)%lnCylinderNum;

		//判断本缸能量和同相位缸的能量是不是最大的两个
		for(unsigned int i = 0; i < avfData_.size(); i++)
		{
			int lnPos3 = -1;

			//两个缸共用一个传感器去掉相邻缸的能量判断
			if(!IsCylinderAndSensorEqual())
			{
				if(avnSequence_[lnPos] % 2 == 0)
				{
					for(unsigned int j = 0; j < avnSequence_.size(); j++)
					{
						if(avnSequence_[j] == avnSequence_[lnPos] - 1)
						{
							lnPos3 = j;
							break;
						}
					}
				}
				else
				{
					for(unsigned int j = 0; j < avnSequence_.size(); j++)
					{
						if(avnSequence_[j] == avnSequence_[lnPos] + 1)
						{
							lnPos3 = j;
							break;
						}
					}
				}
			}

			if( i == lnPos || i == lnPos1 || i == lnPos3)
			{
				continue;
			}

			//同相位两个缸的能量大于其他缸能量5倍，判断为撞缸
			if(avfData_[i]*5 > avfData_[lnPos] || avfData_[i]*5 > avfData_[lnPos1])
			{
				return 0;
			}
		}
	}

	this->m_nAlarmCylinderNo = avnSequence_[lnPos];
	return 2;

}

//小头瓦磨损故障，寻找缸前后180度位置能量，超过缸能量一定比值，判断报警
int CDieselPlantWC::JudgeWattAbrasion(const vector<float>& avfData_, 
								 const vector<int>& avnSequence_,
								 float& afDistance_)
{
	int lnPos = 0,lnPos1 = 0;

	BOOL lbCylinderAndSensorEqual = IsCylinderAndSensorEqual();

	int lnCylinderNum = GetStableModelDimension();

	float lfStepLength = lnCylinderNum/4.0;

	float lfStepForwardLength = 0,lfStepBackwardLength = 0;


	int lnWavePointNum = 0;


	vector<int> lvPos;

	//测点所属缸点火顺序
	for(unsigned int i = 0; i < avnSequence_.size(); i++)
	{
		if((lbCylinderAndSensorEqual && avnSequence_[i] == this->m_stChannInfo._iCylinderNo) ||
			(!lbCylinderAndSensorEqual && (avnSequence_[i] == this->m_stChannInfo._iCylinderNo || 
			avnSequence_[i] == this->m_stChannInfo._iCylinderNo+ 1)))
		{
			lvPos.push_back(i);
		}
	}

	for(unsigned int nloop = 0; nloop < lvPos.size(); nloop++)
	{
		lnPos = lvPos[nloop];

		//前后寻找180，根据缸的数目计算相隔多少缸为180度
		if(lnPos + lfStepLength > avnSequence_.size())
		{
			lfStepForwardLength = lfStepLength - avnSequence_.size();
			lfStepBackwardLength = lfStepLength;
		}	
		else if(lnPos - lfStepLength < 0)
		{
			lfStepForwardLength = lfStepLength;
			lfStepBackwardLength = lfStepLength - avnSequence_.size();
		}
		else
		{
			lfStepForwardLength = lfStepLength;
			lfStepBackwardLength = lfStepLength;
		}

		//缸底作为键相起始位置补偿半个步长
		if(GetKeyStartPosition() == 1)
		{
			lfStepForwardLength = lfStepForwardLength + 0.5;
			lfStepBackwardLength = lfStepBackwardLength - 0.5;
		}

		int lnStepForwardLength = (int)lfStepForwardLength;
		int lnStepBackwardLength = (int)lfStepBackwardLength;

		int lnSegNum = m_vWaveSegInfo[0];

		//数采发送的两个周期点数不一致，平均一下，减少误差
		lnWavePointNum = m_vWaveSegInfo[lnSegNum];

		int lnPerSegWavePointNum = 0;

		int lnWaveStartPos = 0,lnSegStartPos = 0;

		float lfEnergy = 0;

		//计算距离测点所属缸180度位置前后1/4端波形能量，超过缸能量1.5，判断为磨损
		//使用第二个周期判断，起始位置都加上一个缸数
		if(lfStepForwardLength > 0)
		{
			lnSegStartPos = lnPos + lnStepForwardLength + lnCylinderNum;

			if(lnSegStartPos >= lnSegNum)
			{
				lnSegStartPos -= lnCylinderNum;
			}

			lnWaveStartPos = m_vWaveSegInfo[lnSegStartPos];

			lnPerSegWavePointNum = lnWavePointNum/lnSegNum; 

			if(fabs(lfStepForwardLength - lnStepForwardLength) > 0)
			{
				lnWaveStartPos = lnWaveStartPos + lnPerSegWavePointNum/2;
			}

			for(int i = lnWaveStartPos - lnPerSegWavePointNum/3; i < lnWaveStartPos + lnPerSegWavePointNum/3; i++)
			{
				lfEnergy += m_vWave[i]*m_vWave[i]/100.0;
			}

			if(lfEnergy > avfData_[lnPos]*1.5)
			{
				this->m_nAlarmCylinderNo = avnSequence_[lnPos];
				return 1;
			}

		}
		else
		{
			lnSegStartPos = lnPos + lnStepForwardLength + lnCylinderNum;

			if(lnSegStartPos >= lnSegNum)
			{
				lnSegStartPos -= lnCylinderNum;
			}

			lnPerSegWavePointNum = lnWavePointNum/lnSegNum; 

			lnWaveStartPos = m_vWaveSegInfo[lnSegStartPos];

			if(fabs(lfStepForwardLength - lnStepForwardLength) > 0)
			{
				lnWaveStartPos = lnWaveStartPos - lnPerSegWavePointNum/2;
			}

			for(int i = lnWaveStartPos - lnPerSegWavePointNum/3; i < lnWaveStartPos + lnPerSegWavePointNum/3; i++)
			{
				lfEnergy += m_vWave[i]*m_vWave[i]/100.0;
			}

			if(lfEnergy > avfData_[lnPos]*1.5)
			{
				this->m_nAlarmCylinderNo = avnSequence_[lnPos];
				return 1;
			}
		}

		lfEnergy = 0;

		if(lfStepBackwardLength > 0)
		{
			lnSegStartPos = lnPos - lnStepBackwardLength + lnCylinderNum;

			if(lnSegStartPos >= lnSegNum)
			{
				lnSegStartPos -= lnCylinderNum;
			}

			lnPerSegWavePointNum = lnWavePointNum/lnSegNum; 

			lnWaveStartPos = m_vWaveSegInfo[lnSegStartPos];

			if(fabs(lfStepBackwardLength - lnStepBackwardLength) > 0)
			{
				lnWaveStartPos = lnWaveStartPos - lnPerSegWavePointNum/2;
			}

			for(int i = lnWaveStartPos - lnPerSegWavePointNum/3; i < lnWaveStartPos + lnPerSegWavePointNum/3; i++)
			{
				lfEnergy += m_vWave[i]*m_vWave[i]/100.0;
			}

			if(lfEnergy > avfData_[lnPos]*1.5)
			{
				this->m_nAlarmCylinderNo = avnSequence_[lnPos];
				return 1;
			}
		}
		else
		{
			lnSegStartPos = lnPos - lnStepBackwardLength + lnCylinderNum;

			if(lnSegStartPos >= lnSegNum)
			{
				lnSegStartPos -= lnCylinderNum;
			}

			lnPerSegWavePointNum = lnWavePointNum/lnSegNum; 

			lnWaveStartPos = m_vWaveSegInfo[lnSegStartPos];


			if(fabs(lfStepBackwardLength - lnStepBackwardLength) > 0)
			{
				lnWaveStartPos = lnWaveStartPos + lnPerSegWavePointNum/2;
			}

			for(int i = lnWaveStartPos - lnPerSegWavePointNum/3; i < lnWaveStartPos + lnPerSegWavePointNum/3; i++)
			{
				lfEnergy += m_vWave[i]*m_vWave[i]/100.0;
			}

			if(lfEnergy > avfData_[lnPos]*1.5)
			{
				this->m_nAlarmCylinderNo = avnSequence_[lnPos];
				return 1;
			}
		}
	}


	return 0;
}



bool CDieselPlantWC::SetWaveData( const vector<DATATYPE_WAVE>& vWave_ )
{
	this->m_vWave.clear();
	this->m_vWave.resize(vWave_.size());
	memcpy(&this->m_vWave.front(),&vWave_.front(),vWave_.size()*sizeof(DATATYPE_WAVE));

	return true;
}


bool CDieselPlantWC::SetWaveData( float *pWave_ )
{
	this->m_pWave = pWave_;
	return this->m_pWave != NULL;
}
bool CDieselPlantWC::SetWaveSegInfo( const vector<DATATYPE_WAVE>& vWaveSegInfo_ )
{
	this->m_vWaveSegInfo.clear();
	this->m_vWaveSegInfo.resize(vWaveSegInfo_.size());

	memcpy(&this->m_vWaveSegInfo.front(),&vWaveSegInfo_.front(),vWaveSegInfo_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlantWC::SetFlameOutCylinder( const vector<int>& vFlameOutCylinder_ )
{
	this->m_vFlameOutCylinder.clear();
	this->m_vFlameOutCylinder.resize(vFlameOutCylinder_.size());

	memcpy(&this->m_vFlameOutCylinder.front(),&vFlameOutCylinder_.front(),vFlameOutCylinder_.size()*sizeof(DATATYPE_WAVE));

	return true;
}

bool CDieselPlantWC::SetChannInfo( const S_ChannInfo& sChannInfo_ )
{
	this->m_stChannInfo = sChannInfo_;
	return true;
}


bool CDieselPlantWC::SetPlantInfo(  CString strGroup_,const S_PlantInfo& sPlantInfo_ )
{
	this->m_strGroup = strGroup_;
	this->m_stPlantInfo = sPlantInfo_;
	return true;
}

int CDieselPlantWC::JudgeDynDieselAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	return 0;
}

int CDieselPlantWC::JudgeProcDieselAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	return 0;
}

bool CDieselPlantWC::SetValveEnergy( float fValveEnergy_ )
{
	this->m_fValveEnergy = fValveEnergy_;
	return true;
}


float CDieselPlantWC::GenNormalData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfNormalData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	float lfTotalEnergy = 0;

	int lnPos = 0;

	vfNormalData_.resize(vfSegData_.size());

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

float CDieselPlantWC::GenFlameOutData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfFlameoutData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnPos = 0;

	float lfMinEnergy = vfSegData_[0];

	vfFlameoutData_.resize(vfSegData_.size());

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

float CDieselPlantWC::GenCylinderKnockData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfKnockData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnCylinderNum = GetStableModelDimension();

	int StepLength = lnCylinderNum/2;


	float lfMaxEnergy = vfSegData_[0];

	int lnPos = 0;

	vfKnockData_.resize(vfSegData_.size());

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

float CDieselPlantWC::GenWattAbrasionData( const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfWattAbrasionData_ )
{
	if(vfSegData_.size() <= 0 || vfSegData_.size() != vfFireSequence_.size())
	{
		return 0;
	}

	int lnCylinderNum = GetStableModelDimension();

	int lnStepLength = lnCylinderNum/4;

	float lfMaxEnergy = vfSegData_[0];

	int lnPos = 0;

	vfWattAbrasionData_.resize(vfSegData_.size());

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

float CDieselPlantWC::CalcNormalDistance( const vector<float>& vfSegData_ )
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

	if(lvTotalDistance.size() <= 0)
	{
		return lfDistance;
	}


	float lfMaxDistance = lvTotalDistance[0];
	float lfMinDistance = lvTotalDistance[0];

	int lnSize = lvTotalDistance.size();

	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
		if(lfMaxDistance < lvTotalDistance[i])
		{
			lfMaxDistance = lvTotalDistance[i];
		}

		if(lfMinDistance > lvTotalDistance[i])
		{
			lfMinDistance = lvTotalDistance[i];
		}
	}

	/*if(lnSize >= 4)
	{
		lfDistance -= (lfMaxDistance + lfMinDistance);
		lnSize -= 2;
	}*/

	return (lfDistance/lnSize);
}

float CDieselPlantWC::CalcFlameOutdistance( const vector<float>& vfSegData_ )
{
	float lfDistance = 0;

	vector<float> lvTotalDistance;

	if(vfSegData_.size() != m_fFlameOutData.size() || vfSegData_.size() <= 0)
	{
		return lfDistance;
	}

	//计算到每组模型数据的距离
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

	//去掉一个最大距离和一个最小距离，然后计算平均值

	lfDistance = 0;

	if(lvTotalDistance.size() <= 0)
	{
		return lfDistance;
	}


	float lfMaxDistance = lvTotalDistance[0];
	float lfMinDistance = lvTotalDistance[0];

	int lnSize = lvTotalDistance.size();

	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
		if(lfMaxDistance < lvTotalDistance[i])
		{
			lfMaxDistance = lvTotalDistance[i];
		}

		if(lfMinDistance > lvTotalDistance[i])
		{
			lfMinDistance = lvTotalDistance[i];
		}
	}

	/*if(lnSize >= 4)
	{
		lfDistance -= (lfMaxDistance + lfMinDistance);
		lnSize -= 2;
	}*/

	return (lfDistance/lnSize);
}

float CDieselPlantWC::CalcCylinderKnockDistance( const vector<float>& vfSegData_ )
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

	if(lvTotalDistance.size() <= 0)
	{
		return lfDistance;
	}


	float lfMaxDistance = lvTotalDistance[0];
	float lfMinDistance = lvTotalDistance[0];

	int lnSize = lvTotalDistance.size();

	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
		if(lfMaxDistance < lvTotalDistance[i])
		{
			lfMaxDistance = lvTotalDistance[i];
		}

		if(lfMinDistance > lvTotalDistance[i])
		{
			lfMinDistance = lvTotalDistance[i];
		}
	}

	/*if(lnSize >= 4)
	{
		lfDistance -= (lfMaxDistance + lfMinDistance);
		lnSize -= 2;
	}*/

	return (lfDistance/lnSize);
}

float CDieselPlantWC::CalcWattAbrasionDistance( const vector<float>& vfSegData_ )
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

	if(lvTotalDistance.size() <= 0)
	{
		return lfDistance;
	}


	float lfMaxDistance = lvTotalDistance[0];
	float lfMinDistance = lvTotalDistance[0];

	int lnSize = lvTotalDistance.size();

	for(size_t i = 0; i < lvTotalDistance.size(); i++)
	{
		lfDistance += lvTotalDistance[i];
		if(lfMaxDistance < lvTotalDistance[i])
		{
			lfMaxDistance = lvTotalDistance[i];
		}

		if(lfMinDistance > lvTotalDistance[i])
		{
			lfMinDistance = lvTotalDistance[i];
		}
	}

	/*if(lnSize >= 4)
	{
		lfDistance -= (lfMaxDistance + lfMinDistance);
		lnSize -= 2;
	}*/

	return (lfDistance/lnSize);
}

int CDieselPlantWC::JudgeVibChannAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
{
	this->ClearData();

	CString lstrPlantNo = this->m_stPlantInfo._cPlantNo;

	float lfSimilaryCoef = GetSimilarityCoef(lstrPlantNo);

	if(this->m_stChannInfo._iCylinderNo == 0 && this->m_stChannInfo._cUnit == _T("mm/s"))
	{
		return JudgeCrankVibChannAlarm(lfCharValue_, strFolderName_, AlarmCylinderNo_, fDistance_);
	}
	else if(this->m_stChannInfo._iCylinderNo > 0 && this->m_stChannInfo._iCylinderNo < 100)
	{
		std::vector<float> lvData,lvNormalData,lvFlameOutData,lvCylinderKnockData,lvWattAbrasionData;

		std::vector<int>   lvFireSequence;

		std::map<float,int> lmAlarmType;

		std::map<float,int>::iterator it;
		
		std::vector<float> lvWaveData,lvSpectrumData;

		std::vector<float> lvMeanValue,lvVarValue;

		float lfMeanValue = 0;
		
		float lfVarValue = 0;

		//分段计算波形能量
		float lfResult = this->CalcSegWaveEnergy(lvWaveData,E_CALC_WAVE);

		if(IsFloatZero(lfResult))
		{
			AlarmCylinderNo_ = -1;
			fDistance_ = lfCharValue_;
			return 0;
		}

		lfResult = this->CalcSegWaveEnergy(lvSpectrumData,E_CALC_SPECTRUM);
		if(IsFloatZero(lfResult))
		{
			AlarmCylinderNo_ = -1;
			fDistance_ = lfCharValue_;
			return 0;
		}

		int lnReadWave = this->ReadNormalWaveDataFromDisk(strFolderName_);
		int lnReadSpectrum = this->ReadNormalSpectrumDataFromDisk(strFolderName_);
		if( lnReadWave <= 0 || lnReadSpectrum <= 0)
		{
			this->UpdateNormalWaveData(lvWaveData);
			this->UpdateNormalFreqData(lvSpectrumData);
			return E_NORMAL;
		}

		float lfResult1 = this->CalcAverageSimilarity(lvSpectrumData,E_CALC_SPECTRUM);

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

		lstr.Format("柴油机振动测点%s判断结果%f,%f,%d\n",this->m_stChannInfo._cChannNo,lfResult,lfResult1,lnAlarm);

		OutputDebugString(lstr);

		//if(lfResult1 < lfSimilaryCoef && lnAlarm > 0)
		if(lnAlarm > 0 && lfResult < lfSimilaryCoef)
		{
			return E_FAULT_TYPE_OTHER_LOW;
		}

		if(lnAlarm <= 0)
		{
			this->UpdateNormalWaveData(lvWaveData);
		}

		if(lfResult1 >= 0.97)
		{
			this->UpdateNormalFreqData(lvSpectrumData);
		}

		return E_NORMAL;

	}
	else
	{
		return E_NORMAL;
	}


	return E_NORMAL;


}

int CDieselPlantWC::UpdateNormalData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fNormalData.size() <= 0)
	{
		
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
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

int CDieselPlantWC::UpdateFlameOutData( const vector<float>& avfData_ )
{

	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fFlameOutData.size() <= 0)
	{
		
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
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

int CDieselPlantWC::UpdateCynlinderKnockData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fCylinderKnockData.size() <= 0)
	{
		
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
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

int CDieselPlantWC::UpdateWattAbrasionData( const vector<float>& avfData_ )
{
	int lnDataSize = GetInitStableVectorSpaceSize();

	if(m_fWattAbrasionData.size() <= 0)
	{
		
		for(size_t i = 0; i < avfData_.size(); i++)
		{
			vector<float> lvTmp;
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

int CDieselPlantWC::JudgeCrankVibChannAlarm( float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_ )
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

int CDieselPlantWC::ReadNormalDataFromDisk(CString strFolderName_)
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

	if(ReadFileNameFromDisk(lstrFolder,m_sNormalDataFileName,true,E_CALC_WAVE))
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

	if(m_fNormalData.size() > 0 && m_fNormalData[0].size()>=5)
	{
		return 1;
	}

	return 0;
}

int CDieselPlantWC::ReadFlameOutDataFromDisk(CString strFolderName_)
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

int CDieselPlantWC::ReadCylinderKnockDataFromDisk(CString strFolderName_)
{
	///读取数据文件所在文件目录
	CString lstrFolder;

	if(!ReadFolderNameFromDisk(lstrFolder,strFolderName_))
	{
		MakeSureFolderExist(lstrFolder);
	}

	lstrFolder.Append("\\CylinderKnock");

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

int CDieselPlantWC::ReadWattAbrasionDataFromDisk(CString strFolderName_)
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

int CDieselPlantWC::ReadCrankAlarmDataFromDisk(CString strFolderName_)
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
				m_strAlarmDataFileName = lvFileName[i];

				m_fAlarmValue = lvModelData[0];
			}
		}
	}
	return 1;
}

void CDieselPlantWC::ClearData()
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
	m_strAlarmDataFileName = "";
	m_fAlarmValue = 0;
}

bool CDieselPlantWC::SetWavePtNum( const int nWavePtNum_ )
{
	this->m_nWaveNum = nWavePtNum_;
	return this->m_nWaveNum > 0;
}

vector<float> CDieselPlantWC::CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ )
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

float CDieselPlantWC::CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ )
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

float CDieselPlantWC::CalcAverageSimilarity( const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_ )
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

bool CDieselPlantWC::SetTowCycleWavePtNum( const int nWavePtNum_ )
{
	this->m_nTwoCyclePtNum = nWavePtNum_;
	return this->m_nTwoCyclePtNum > 0;
}

int CDieselPlantWC::ReadNormalWaveDataFromDisk( CString strFolderName_ )
{
	//读取数据文件所在文件目录
	CString lstrFolder;

	int lnSize = GetInitStableVectorSpaceSize();

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

	if(m_fNormalWaveData.size() > 0 && m_fNormalWaveData[0].size()>=lnSize)
	{
		return 1;
	}

	return 0;
}

int CDieselPlantWC::ReadNormalSpectrumDataFromDisk( CString strFolderName_ )
{
	//读取数据文件所在文件目录
	CString lstrFolder;

	int lnSize = GetInitStableVectorSpaceSize();

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

	if(m_fNormalSpectrumData.size() > 0 && m_fNormalSpectrumData[0].size()>=lnSize)
	{
		return 1;
	}

	return 0;
}

int CDieselPlantWC::UpdateNormalWaveData( const vector<float>& avfData_ )
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

int CDieselPlantWC::UpdateNormalFreqData( const vector<float>& avfData_ )
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


int CDieselPlantWC::JudgeMeanAndVarValueAlarm( const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_ )
{
	if(avfMeanValue_.size() != avfVarValue_.size() || avfMeanValue_.size() != avfDataValue_.size())
	{
		return 0;
	}

	int lnAlarm = 0;
	int lnNormal = 0;

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

		CString lstr;

		lstr.Format("柴油机振动测点%s:%s分布判断结果%f,%f,%f\n",this->m_stPlantInfo._cPlantNo,this->m_stChannInfo._cChannNo,avfMeanValue_[i],avfDataValue_[i],sqrtf(avfVarValue_[i]));

		OutputDebugString(lstr);
	}

	if(lnAlarm >= lnNormal)
	{
		return 1;
	}

	return 0;

}

vector<float> CDieselPlantWC::CalcDataSegEnergyMeanValue( float *pData_,int nWaveNum_,int nSegNum_ )
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

bool CDieselPlantWC::SetSampleFreq( const int nSampleFreq_ )
{
	this->m_nSmpFreq = nSampleFreq_;
	return true;
}