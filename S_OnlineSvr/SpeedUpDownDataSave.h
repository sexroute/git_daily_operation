#pragma once

#include "stdafx.h"
#include "PlantInfo.h"

class CSudPlantInfo: public CPlantInfoOnLine
{
public:
	 CSudPlantInfo();
	 ~CSudPlantInfo();
private:
	/**启停车的时间字符串
	  */
	string _sSUDTime;
	string _sSudTimeStart;
	int _iMicroSecond;
	string        _sFieldSel;
	string        _sFieldIns;
	///当前启停车事件的ID
	long _iCurrEventID;
	long _iEventID;

	// initialize current ID
	void InitCurrID(CCustomDB * pDb_);
	/**
	  */
	int AddSudLog(CCustomDB * pDB_);
	// save SUD data
	void SaveData(IDBInterFace *pDB_);
	virtual void Destory();
public:
	//设置当前数据的时间
	virtual void SetDataTime(CBHDateTime dataTime_,int iMicroSecond_);
	// 设置启停车状态，并判断是否需要保存
	void SetDataStatu(IDBInterFace * pDB_,bool bFlag_,bool bSave_,long iEventID_,E_SUD_TYPE iSudType_);
	//得到指定测点号的波形,返回值为实际读取得波形点数，如果为-1，就是没有指定的通道
	int GetSpecChannWave(string sChannID_,DATATYPE_WAVE *pfWave_,int &ilen_/*读取得波形点数*/);
	///得到指定振动测点的波形和频谱数据
	int GetSpecChannVibWaveAndFreq(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float &fScaleCoef_,int& nPrecisionEnabled_,float *pWave_,float *pFreq_);
	//得到指定测点号的启停车信息和转速
	int GetSpecChannIndex(string sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);
	//得到指定测点号的启停车信息和转速
	int GetSpecChannIndex(int index_,string &sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);
    //得到当前时间
	CBHDateTime GetCurrDataTime(int& iMicroSecond_);
	virtual void AddVibChannInfo(CCustomDB *pDB_);
	virtual void AddVibChannInfo(IDBInterFace *pDB_);
	virtual void AddChannInfoEnd();
	///得到指定径向振动所有特征值数据
	int GetSpecChannRadialAllChar(string sChann_,S_VibCharValue &sVibData_,string &sUnit_);
	int GetSpecChannRadialAllChar(int index_,S_VibCharValue &sVibData_,string &sUnit_);
	/**得到开停车的状态、开停车类型和开停车事件ID
	  *@param iType_ 开停车类型,是引用变量
	  *@param iEventID_ 当前启停车的事件ID,是引用变量
	  *@return 开停车状态
	  */
	bool GetStatusType(E_SUD_TYPE & iType_,int & iEventID_);
//	virtual int UpdateVibData(DATATYPE_WAVE *pWave_,S_VibCharValue *pVibCharValue_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_=0);

		/**获取开停车数据转速标记
	*@param pDB_      数据库连接指针
	*@param sTableName_    表名
	*@param iCurID_    开停车时间ID
	*@param fRev_      转速
	*@return 转速标记值
	*/

private:
	float GetRevMark(IDBInterFace * pDB_,
		             string sTableName_,
		             int iCurID_,
					 float fRev_,
					 CString sField_,
					 const CBHDateTime& dtTime_,
					 int   iMicroSecond_);
	float  GetRevMarkInterval();

private:
	string        _sProcField;
	string        _sVibFieldSel;
	string        _sVibFieldIns;
	string        _sDynFieldSel;
	string        _sDynFieldIns;
	bool          _bProcDataUpdateSend;

public:

	virtual void AddChannInfoEnd(IDBInterFace *pDB_);
	virtual void AddChannInfoEnd(CCustomDB * pDb_);

	int GetProcData(float *pfData_,int iNum_,int iStartIndex_=0);

	// 保存振动启停车数据到启停车数据表SUD_<plant_no>。
	int SaveSudVibRecord(   IDBInterFace * pDB_,
							string sTableName_,
							int iCurID_, 
							const CBHDateTime &tDatetime_,
							int anMilliSecond,
							bool bMonth_=false);

	int SaveSudDynRecord(   IDBInterFace * pDB_,
							string sTableName_,
							int iCurID_, 
							const CBHDateTime &tDatetime_,
							int anMilliSecond,
							bool bMonth_=false);

	int SaveSudProcRecord(  IDBInterFace * pDB_,
							string sTableName_,
							int iCurID_, 
							const CBHDateTime &tDatetime_,
							int anMilliSecond,
							bool bMonth_=false);

	int SaveSudTrend(IDBInterFace * pDb_,int SaveType);

    void SetSudFinishStatus(bool bFinishStatus_);
    bool GetSudFinishStatus();

	// 保存振动启停车 后处理数据到启停车数据表SUD_<plant_no>。
	int SaveSudVibData(
		IDBInterFace *apDB, 
		long aEventID, 
		CBHDateTime aDatetime, 
		int aMicroSecond, 
		int iStartIndex_,
		int aChannNum, 
		S_VibCharValue *apCharValue, 
		DATATYPE_WAVE *apWave, 
		int aWaveNum, 
		S_CustomParamData *apParam, 
		string aTemplate, 
		int aSmpNum, 
		int aSmpFreq, 
		float apRev[5],
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax);


	int SaveSudDynData(
		IDBInterFace *apDB, 
		long aEventID, 
		CBHDateTime aDatetime, 
		int aMicroSecond, 
		int aStartIndex,
		int aChannNum, 
		S_DynCharValue *apCharValue, 
		DATATYPE_WAVE *apWave, 
		int aWaveNum, 
		S_CustomParamData *apParam, 
		string aTemplate, 
		int aSmpNum, 
		int aSmpFreq, 
		float apRev[5],
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax);

	int SaveSudProcData( 
		IDBInterFace * apDB, 
		int aEventID, 
		CBHDateTime aDateTime, 
		int aMicrosecond, 
		float* aCharVal, 
		int aStartIndex, 
		int aChannNum, 
		float apRev[5]);

	float GetVibDataWaveCoef(int iLen_,int iStartIndex_=0);
	float GetDynDataWaveCoef(int iLen_,int iStartIndex_=0);
private:
    bool  _bSudFinishStatus;
	string   _sVibRevField;
	string   _sDynRevField;
	string   _sProcRevField;

};