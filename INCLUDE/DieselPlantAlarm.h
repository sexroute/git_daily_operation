#pragma once

#ifndef __DIESEL_PLANT__H__
#define __DIESEL_PLANT__H__

#include "ZoomAnalysis.h"
#include "DSE_Define.h"

enum E_DIESEL_FAULT
{
	E_NORMAL,
	E_FAULT_TYPE_FLAMEOUT,
	E_FAULT_TYPE_CYLINDER_KNOCK,
	E_FAULT_TYPE_WATT_ABRASION,
	E_FAULT_TYPE_OTHER_LOW = 10,
	E_FAULT_TYPE_OTHER_HIGH
};

enum E_DIESEL_CALC_TYPE
{
	E_CALC_WAVE,
	E_CALC_SPECTRUM
};

class  CDieselPlant
{
public:
	CDieselPlant(void);
	virtual ~CDieselPlant(void);

public:
	//振动测点报警判断
	virtual int JudgeVibDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_) = 0;

	//动态测点报警判断
	virtual int JudgeDynDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_) = 0;

	//过程量测点报警判断
	virtual int JudgeProcDieselAlarm(float lfCharValue_,CString strFolderName_, int &AlarmCylinderNo_,float& fDistance_) = 0;

	//设置波形数据
	virtual bool SetWaveData(const vector<DATATYPE_WAVE>& vWave_) = 0;

	//设置各缸索引
	virtual bool SetWaveSegInfo(const vector<DATATYPE_WAVE>& vWaveSegInfo_) = 0;

	//设置根据瞬时转速判断的失火的缸
	virtual bool SetFlameOutCylinder(const vector<int>& vFlameOutCylinder_) = 0;

	//设置测点信息
	virtual bool SetChannInfo(const S_ChannInfo& sChannInfo_) = 0;

	//设置设备信息
	virtual bool SetPlantInfo(CString strGroup_,const S_PlantInfo& sPlantInfo) = 0;

	//设置气阀能量
	virtual bool SetValveEnergy(float fValveEnergy_) = 0;

	//设置波形数据
	virtual bool SetWaveData(float *pWave_) = 0;

	//设置波形点数
	virtual bool SetWavePtNum(const int nWavePtNum_) = 0;

	virtual bool SetTowCycleWavePtNum(const int nWavePtNum_)=0;

	virtual bool SetSampleFreq(const int nSampleFreq_) = 0;
protected:

	///建立稳态模型维数
	int GetStableModelDimension();
	///正常状态模型向量空间大小（可设置）
	int GetInitStableVectorSpaceSize();
	///气缸数是否和传感器数一致
	int IsCylinderAndSensorEqual();
	///正常状态模型向量空间存数是否已满
	BOOL IsStableVectorSpaceFull(const vector<float>& fVectorSpace_);
	///向正常状态模型向量空间存入一个数据，存放到文件中
	BOOL PushDataIntoStableVectorSpace(vector<float>& vfData_, float fData_);
	///从正常状态模型向量空间弹出最老数据，存放到文件中
	BOOL PopDataFromStableVectorSpace(vector<float>& vfData_);
	///将数据写入文件
	BOOL WriteDataToFile(const CString& strFileName_,const vector<float>& vfData_);
	///从文件中读取数据
	BOOL ReadModelFileData( const CString& strPath_,vector<float>& vfData_ );
	///计算当前数据到向量空间的距离
	float CalcDistance(const vector<float>& vfXData_, const vector<float>& vfYData_);

	///获取键相起始位置
	int GetKeyStartPosition();

	///获取失火判断方式
	int GetFlameOutJudgeType();

	///建立文件夹
	BOOL MakeSureFolderExist( const CString &asAbsoluteFilePath );

	/// 获取点火顺序
	BOOL GetFireSequence(CString strPlantNo_,vector<int>& avnSequence_);

    /// 计算频谱
	int ZoomSpectrumAnalysis(float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_);

	/// 圆整数据到2的n次方
	int GetNextPowPtNum(int nWavePtNum_);

	/// 获取相似度计算方法（0：相关系数，1：余弦定理）
	float GetSimilarityCoef(CString strPlantNo_);

	int GetCalcSimilarityType(CString strPlantNo_);

	int GetCalcSegWaveRange(CString strPlantNo_);

	int  GetAlarmDistance();

	float CalcWaveMeanAndStdValue(const vector<float>& afvWave_,float& afMeanValue_,float& afVarValue_);

	bool GetDiagnoseInterface( CString& strIP_,int& nPort_ );



protected:

	CString               m_strFolderPath;     //文件夹路径
	int                   m_nAlarmCylinderNo;  //报警的缸
	float                 m_fValveEnergy;      //气阀能量

	vector<DATATYPE_WAVE> m_vWave;             //波形
	vector<DATATYPE_WAVE> m_vWaveSegInfo;      //各缸分段起始索引
	vector<int>           m_vFlameOutCylinder; //根据瞬时转速判断的失火的缸
	S_ChannInfo           m_stChannInfo;       //测点信息
	S_PlantInfo           m_stPlantInfo;       //设备信息
	CString               m_strGroup;          //集团信息

	int                   m_nWaveNum;          //所有波形点数
	float*                m_pWave;             //波形
	int                   m_nTwoCyclePtNum;    //两周期波形点数
	int                   m_nSmpFreq;


};

#endif