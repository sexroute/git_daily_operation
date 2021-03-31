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

	///����ÿ�β�����������
	float CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_ = E_CALC_WAVE);

	
	
	///��ʼ���ļ�����·��
	BOOL InitModelFilePath(CString strFolderName_);

	CString GetModelFolderPath();
	void SetModelFolderPath(const CString& strPath_);

	BOOL ChannWaveEnergyAlarm(const CString& astrPath_);

	void ClearData();

	//�������Ƶ�ʶ�Ƶ�������ٷֱ�
	vector<float> CalcDataSegEnergyPercentage(float *pData_,int nWaveNum_,int nSegNum_ );

	vector<float> CalcDataSegEnergyMeanValue(float *pData_,int nWaveNum_,int nSegNum_ );

	//�������ƶ�
	float CalcDataSimilarity(const vector<float>& avfWave1_,const vector<float>& avfWave2_);

	float CalcAverageSimilarity(const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_);

	//���ݾ�ֵ�ͱ�׼���ж��жϱ���
	int   JudgeMeanAndVarValueAlarm(const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_);
	
	//��ȡ�����ٶ�Ƶ������
	int ReadNormalSpectrumDataFromDisk( CString strFolderName_,vector<vector<float> >& vFreqData_ );
	
	//��ȡ������������
	int ReadNormalWaveDataFromDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ );

	//����������д�뵽�ļ�
	int WriteNormalWaveDataToDisk( CString strFolderName_,vector<vector<float> >& vWaveData_ );

	//������Ƶ��д�뵽�ļ�
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
