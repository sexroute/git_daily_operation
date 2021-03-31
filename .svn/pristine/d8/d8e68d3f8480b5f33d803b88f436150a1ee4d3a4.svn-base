#pragma once
#include "../include/DieselPlantAlarm.h"

class CPumpPlant :
	public CDieselPlant
{
public:
	CPumpPlant(void);
	~CPumpPlant(void);

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

	///读取测点下所有模型文件名，判断当前数据所在区间
	BOOL ReadModelDataFromDisk(const CString &strFolderName_);
	///读取测点下所有模型文件名，判断当前数据所在区间
	BOOL WriteModelDataToDisk(const CString &strFolderName_);
	
	///初始化文件保存路径
	BOOL InitModelFilePath();

	CString GetModelFolderPath();
	void SetModelFolderPath(const CString& strPath_);

	float CalcMeanValue(const vector<float>& vData_ );

	float CalcMinDistance( const vector<float>& vXData_,const vector<float>& vYData_ );

	vector<float> CalcDataSegEnergyPercentage( float *pData_,int nWaveNum_,int nSegNum_ );

	float CalcDataSimilarity( const vector<float>& avfWave1_,const vector<float>& avfWave2_ );

	float GetPowerSpliteFactor(CString strPlantNo_);

	bool ConfirmAlarm(CString strFolderName_,bool bAlarm_);


private:

	vector<vector<float> > m_vFreqData;


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
