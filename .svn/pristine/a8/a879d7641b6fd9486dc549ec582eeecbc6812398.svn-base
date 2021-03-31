#pragma once
#include "../include/DieselPlantAlarm.h"

class CDieselPlant956 : public CDieselPlant
{
public:
	CDieselPlant956(void);
	~CDieselPlant956(void);

public:
	virtual int JudgeVibDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual int JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual int JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual bool SetWaveData(const vector<DATATYPE_WAVE>& vWave_);

	virtual bool SetWaveSegInfo(const vector<DATATYPE_WAVE>& vWaveSegInfo_);

	virtual bool SetFlameOutCylinder(const vector<int>& vFlameOutCylinder_);

	virtual bool SetChannInfo(const S_ChannInfo& sChannInfo_);

	virtual bool SetPlantInfo(CString strGroup_,const S_PlantInfo& sPlantInfo_);

	virtual bool SetValveEnergy(float fValveEnergy_);

	virtual bool SetWaveData(float *pWave_);

	virtual bool SetWavePtNum(const int nWavePtNum_);

	virtual bool SetTowCycleWavePtNum(const int nWavePtNum_);

	virtual bool SetSampleFreq(const int nSampleFreq_);
protected:

	///计算每段波形数据能量
	float CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_ = E_CALC_WAVE);

	
	
	///初始化文件保存路径
	BOOL InitModelFilePath(CString strFolderName_);

	CString GetModelFolderPath();
	void SetModelFolderPath(const CString& strPath_);

	BOOL ChannWaveEnergyAlarm(const CString& astrPath_);

	void ClearData();

	//计算各个频率段频谱能量百分比
	vector<float> CalcDataSegEnergyPercentage(float *pData_,int nWaveNum_,int nSegNum_ );

	vector<float> CalcDataSegEnergyMeanValue(float *pData_,int nWaveNum_,int nSegNum_ );

	//计算相似度
	float CalcDataSimilarity(const vector<float>& avfWave1_,const vector<float>& avfWave2_);

	float CalcAverageSimilarity(const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_);

	//根据均值和标准差判断判断报警
	int   JudgeMeanAndVarValueAlarm(const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_);
	
	//读取正常速度频谱数据
	int ReadNormalSpectrumDataFromDisk( CString strFolderName_,vector<vector<float> >& vFreqData_ );
	
	//读取正常波形数据
	int ReadNormalWaveDataFromDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ );

	//将正常波形写入到文件
	int WriteNormalWaveDataToDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ );

	//将正常频谱写入到文件
	int WriteNormalSpectrumDataToDisk( CString strFolderName_,vector<vector<float> >& vFreqData_ );


	float CalcMeanValue(const vector<float>& vData_ );


private:

	float  m_fAlarmValue;
	CCriticalSection        m_CriticalSection;

	vector< vector<float> > m_fNormalWaveData;
	vector< vector<float> > m_fNormalSpectrumData;

};

class CMyCriticalSection
{
public:
	CMyCriticalSection(CCriticalSection *pCriticalSection_)
	{
		m_pCriticalSection = NULL;
		if(NULL != pCriticalSection_)
		{
			m_pCriticalSection = pCriticalSection_;
			m_pCriticalSection->Lock();
		}
	}
	~CMyCriticalSection()
	{
		if(m_pCriticalSection)
		{
			m_pCriticalSection->Unlock();
		}
	}
private:
	CCriticalSection * m_pCriticalSection;
};
