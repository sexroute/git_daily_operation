#pragma once
#include "../include/DieselPlantAlarm.h"

class CDieselPlant16PA6STC :
	public CDieselPlant
{
public:
	CDieselPlant16PA6STC(void);
	~CDieselPlant16PA6STC(void);

public:
	virtual int JudgeVibDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual int JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual int JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	virtual bool SetWaveData(const vector<DATATYPE_WAVE>& vWave_);

	virtual bool SetWaveSegInfo(const vector<DATATYPE_WAVE>& vWaveSegInfo_);

	virtual bool SetFlameOutCylinder(const vector<int>& vFlameOutCylinder_);

	virtual bool SetChannInfo(const S_ChannInfo& sChannInfo_);

	virtual bool SetPlantInfo( CString strGroup_,const S_PlantInfo& sPlantInfo_);
	
	virtual bool SetValveEnergy(float fValveEnergy_);

	virtual bool SetWaveData(float *pWave_);

	virtual bool SetWavePtNum(const int nWavePtNum_);

	virtual bool SetTowCycleWavePtNum(const int nWavePtNum_);

	virtual bool SetSampleFreq(const int nSampleFreq_);

protected:

	///计算每段波形数据能量
	float CalcSegWaveEnergy(vector<float>& vfData,E_DIESEL_CALC_TYPE nType_ = E_CALC_WAVE);

	//生成正常数据
	float GenNormalData(const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfNormalData_);
	//生成缸失火数据
	float GenFlameOutData(const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfFlameoutData_);
	//生成撞缸数据
	float GenCylinderKnockData(const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfKnockData_);
	//生成头瓦磨损数据
	float GenWattAbrasionData(const vector<float>& vfSegData_,const vector<int>& vfFireSequence_,vector<float>& vfWattAbrasionData_);

	//计算到正常数据集的距离
	float CalcNormalDistance(const vector<float>& vfSegData_);
	//计算到失火数据集的距离
	float CalcFlameOutdistance(const vector<float>& vfSegData_);
	//计算到撞缸数据集的距离
	float CalcCylinderKnockDistance(const vector<float>& vfSegData_);
	//计算到小头瓦磨损数据集的距离
	float CalcWattAbrasionDistance(const vector<float>& vfSegData_);

	///读取测点下所有模型文件名，判断当前数据所在区间
	BOOL ReadFolderNameFromDisk(CString & strFolderPath_,const CString &FolderName_);
	///读取文件夹下所有模型文件
	BOOL ReadFileNameFromDisk(const CString& strFolderPath_,vector<CString>& vFileName_,bool bSegWave = true,E_DIESEL_CALC_TYPE eType_ = E_CALC_WAVE);

	///初始化文件保存路径
	BOOL InitModelFilePath();

	CString GetModelFolderPath();
	void SetModelFolderPath(const CString& strPath_);

	BOOL ChannWaveEnergyAlarm(const CString& astrPath_);

	//计算向量空间模型半径
	float CalcRadius(const vector<float>& avfData_,const vector<float>& avfOrgCoord_);

	//计算气阀能量
	float CalcValveEnergy(const vector<float>& avfData_,
		const vector<int>& avnSequence_);

	int JudgeFlameOut(const vector<float>& avfData_,
		const vector<int>& avnSequence_,
		float fValveEnergy_,
		float& afDistance_);

	int JudgeCylinderKnock(const vector<float>& avfData_,
		const vector<int>& avnSequence_,
		float fValveEnergy_,
		float& afDistance_);

	int JudgeWattAbrasion(const vector<float>& avfData_,
		const vector<int>& avnSequence_,
		float fValveEnergy_,
		float& afDistance_);

	//非等相位点火情况下，拆成两个点火顺序
	vector<int> GetChannFireSequence(const vector<int>& avnSequence_);

	//判断振动测点报警状态
	int JudgeVibChannAlarm(float afCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);
	//判断曲轴箱测点报警状态
	int JudgeCrankVibChannAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_);

	//更新常规数据模型
	int UpdateNormalData(const vector<float>& avfData_);
	//更新失火数据模型
	int UpdateFlameOutData(const vector<float>& avfData_);
	//更新撞缸数据模型
	int UpdateCynlinderKnockData(const vector<float>& avfData_);
	//更新轴瓦磨损数据模型
	int UpdateWattAbrasionData(const vector<float>& avfData_);

	//读取正常模型数据
	int ReadNormalDataFromDisk(CString strFolderName_);
	//读取失火模型数据
	int ReadFlameOutDataFromDisk(CString strFolderName_);
	//读取撞缸模型数据
	int ReadCylinderKnockDataFromDisk(CString strFolderName_);
	//读取头瓦磨损模型数据
	int ReadWattAbrasionDataFromDisk(CString strFolderName_);
	//读取曲轴箱模型数据
	int ReadCrankAlarmDataFromDisk(CString strFolderName_);


	//更新常规数据模型
	int UpdateNormalWaveData(const vector<float>& avfData_);

	//更新常规数据模型
	int UpdateNormalFreqData(const vector<float>& avfData_);

	//更新常规曲轴箱数据模型
	int UpdateNormalCrankData(const float afValue_);

	//读取正常模型数据
	int ReadNormalWaveDataFromDisk(CString strFolderName_);
	//读取失火模型数据
	int ReadNormalSpectrumDataFromDisk(CString strFolderName_);

	//读取曲轴箱速度模型数据
	int ReadNormalCrankDataFromDisk(CString strFolderName_);

	//判断曲轴箱测点是否报警
	E_DIESEL_FAULT JudgeCrankDataAlarm(float afValue_);


	void ClearData();

	//计算各个频率段频谱能量百分比
	vector<float> CalcDataSegEnergyPercentage(float *pData_,int nWaveNum_,int nSegNum_ );

	vector<float> CalcDataSegEnergyMeanValue(float *pData_,int nWaveNum_,int nSegNum_ );

	float CalcDataSimilarity(vector<float> avfWave1_,vector<float> avfWave2_);

	float CalcAverageSimilarity(const vector<float>& avfData_,E_DIESEL_CALC_TYPE eType_);

	float CalcWaveMeanAndStdValue(const vector<float>& afvWave_,float& afMeanValue_,float& afVarValue_);

	int   JudgeMeanAndVarValueAlarm(const vector<float>& avfMeanValue_,const vector<float>& avfVarValue_,const vector<float>&avfDataValue_);




private:

	vector< vector<float> > m_fNormalData;
	vector< vector<float> > m_fFlameOutData;
	vector< vector<float> > m_fCylinderKnockData;
	vector< vector<float> > m_fWattAbrasionData;
	vector<CString>         m_sNormalDataFileName;
	vector<CString>         m_sFlameOutDataFileName;
	vector<CString>         m_sCylinderKnockDataFileName;
	vector<CString>         m_sWattAbrasionDataFileName;
	CString                 m_strNormalDataFileName;
	float  m_fAlarmValue;
	CCriticalSection        m_CriticalSection;

	vector< vector<float> > m_fNormalWaveData;
	vector< vector<float> > m_fNormalSpectrumData;

	vector<CString>         m_sNormalWaveDataFileName;
	vector<CString>         m_sNormalSpectrumDataFileName;

	vector<float>           m_fNormalCrankData;


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

