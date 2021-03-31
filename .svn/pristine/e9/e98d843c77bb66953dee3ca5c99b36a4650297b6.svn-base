#pragma once
#include "VibData.h"
#include "PlantInfoBase.h"
#include "Sigdefine.h"
#include "SigMath.h"

class HashTableSeedTable
{
public:
	HashTableSeedTable();
	DWORD SeedTable[0x500];

};

class BufferedBaseChannSigMathBasedOnSpecChann
{
public:
	BufferedBaseChannSigMathBasedOnSpecChann();

	/**冲击诊断分析：包络分析，并计算gSE值
	  *@param pwave_ 1维向量，长度为ilen_
	  *@param ilen_ 点集的长度
	  *@param iFs_  采样频率
	  *@param iCutoffFreq_ 高通滤波的截止频率
	  *@param fgSEValue_ 返回计算得到的gSE值
	  *@param pEnvWave_ 返回冲击诊断分析后的波形，必须预先分配好内存，长度为ilen_。如果不需要返回就设置为NULL
	  *@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。
	  *@return 小于0表示失败
	  */

	int gSEAnalysis(const double* pwave_, 
					const int& ilen_, 
					const int& iFs_,
					const int& iCutoffFreq_, 
					double & fgSEValue_, 
					double *pEnvWave_ = NULL,
					unsigned int iPrecision_ = 2);

	/**@brief 频谱分析
	*@param pwave 时域波形数据
	*@param n     波形的长度
	*@param pout  频谱数据的输出，必须事先分配好内存，长度为n/2
	*@param type  频谱类型0：峰值谱，1:功率谱
	*@param iWinType_ 加窗的类型，见窗类型定义。
	*@param iPrecision_ 精度，如果大于6，就不做精度处理，实际是多少就是少。
	*/

	int SpectralMeasureMent(const double *pwave,
							int n,double *pout,
							E_SpectrumType type,
							E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
							unsigned int iPrecision_ = 2);
	///必须把vOut_长度为n/2。
	int SpectralMeasureMent(const double *pwave,
							int n,vector<double>& vOut_,
							E_SpectrumType type,
							E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
							unsigned int iPrecision_ = 2);

	/**对频谱进行积分
	*@param pSpectrum 输入要积分的频谱数据，计算成功后，为积分后的结果
	*@param df 频谱分辨率
	*@param ilen_ pSpectrum数据的数目
	*@param fSensitivity_ 灵敏度系数 即输出(积分后)=输入/fSensitivity_
	*@param eSpectrumType_  需要积分频谱数据的频谱类型
	*@param iStartidx_ 起始积分的索引
	*@return  0 表示正确；小于0表示错误
	*/
	int SpectrumIntegrate(	double * pSpectrum_, 
							const double& df_,
							const int& ilen_, 
							const double& fSensitivity_= 1.0,
							E_SpectrumType eSpectrumType_ = E_SpectrumType_PEAK, 
							const int& iStartidx_ = 2);


	static int Hash(char * apChar,int anLen,int anMask = 2);

public:
	CSigMath m_oSigMath;
private:
	static HashTableSeedTable m_oSeedTable;
	typedef std::vector<double> ValueType;
	std::map<int,ValueType> m_oMapBuffer;
	std::map<int,double> m_oMapDoublebuffer;

};


typedef struct _BufferForCustomCalc
{
	std::vector<double> m_ovDoubleWaveBuffer;					//short 2 double后的缓存
	std::vector<double> m_ovMapIntegrateDoubleWaveBuffer;		//short 2 double积分后的缓存
	std::vector<double> m_ovMapDiffDoubleWaveBuffer;			//short 2 double微分后的缓存
	std::vector<double> m_ovMapDoubleWaveBufferSpectrum;		//频谱计算用的临时缓存
	std::vector<double> m_ovMapDoubleEnvelopeWaveBuffer;		//频谱计算包络波形的缓存
} BufferForCustomCalc,* pBufferForCustomCalc;

namespace Datasource
{
	inline bool IsMixed(unsigned int aDatasource)
	{
		int count = 0;

		while(aDatasource)
		{
			count++;

			if (count > 1)
			{
				return true;
			}

			aDatasource = aDatasource >> 1;
		}

		return false;
	}

	inline bool HasOnline(unsigned int aDatasource)
	{
		return aDatasource & GE_MACHINE_DATA_SOURCE_ONLINE;
	}
}


class CPlantInfoOnLine: public CPlantInfoBase
{
public:
	CPlantInfoOnLine(void);
	virtual ~CPlantInfoOnLine(void);
protected:
	//删除一些分配的数据
	virtual void Destory();
protected:
	vector<S_AxisData>     _vAxisChannValue;

	//波形数据指针,更新状态完整
	DATATYPE_WAVE *_pWave;
	//动态测点波形数据指针,更新状态完整
	DATATYPE_WAVE *_pWaveDyn;

	//波形数据指针，实时更新，更新状态不一定完整
	DATATYPE_WAVE *_pWave_real;
	//动态测点波形数据指针，实时更新，更新状态不一定完整
	DATATYPE_WAVE *_pWaveDyn_real;

	//实时转速1~5
	float _pfRealRev[5];
	float _pfLastRev[5];
	float		  m_fMaxRealRev;
	long		  m_lMaxRealRevLast;

	float		  m_fMaxRev;

	CBHDateTime _tLastRevTime;

	//数据是否更新
    bool _bVibDataUpdate;
	bool _bProcDataUpdate;
	bool _bDynDataUpdate;
    //测点信息更新
	bool _bVibChannInfoUpdate;
	bool _bDynChannInfoUpdate;

	//接受到的振动信号的所有波形点数
	int _iVibRealAllNum;
	//接受到的动态信号的所有波形点数
	int _iDynRealAllNum;

	//一个设备所有振动信号的总的波形点数
	int _iVibAllWaveNum;
	//一个设备所有动态信号的总的波形点数
	int _iDynAllWaveNum;

	///启停车类型
	E_SUD_TYPE _iType;
	///判断是否处于启停车状态
	bool _bSudStatus;
	///振动数据锁,包括转速,采样参数
	IReadWriteLock *_pLockVibData;

	///动态通道数据锁,包括采样参数
	IReadWriteLock *_pLockDynData;

	///过程量数据锁
	IReadWriteLock *_pLockProcData;
public:
	//更新数据状态锁
	IReadWriteLock *_pLockDataUpdateStatues;

	//轴心位置数据读写锁
	IReadWriteLock * _pLockAxisLocationData;
private:
	 //实时采样点数
	int _iRealSmpNum;
	//实时采样频率
	int _iRealSmpFreq;
	//动态信号的采样频率
	int _iDynRealSmpFreq;
	//动态信号的采样点数
	int _iDynRealSmpNum;

public:
	/*!在趋势表，当前趋势秒趋势保存过程中，下次记录应该保存的记录ID值，这个表单中总共是3600条记录，即小时，因为这个表单在外部通过定时器设置为一秒钟执行一次*/
	int           _iCurrIDVibTrendSecond;
	int           _iCurrIDDynTrendSecond;
	int           _iCurrIDProcTrendSecond;
	/*!在趋势表，小时趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDVibTrendHour;
	int           _iCurrIDDynTrendHour;
	int           _iCurrIDProcTrendHour;
	/*!在趋势表，天趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDVibTrendDay;
	int           _iCurrIDDynTrendDay;
	int           _iCurrIDProcTrendDay;
	/*!在趋势表，月趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDProcTrendMonth;
	int           _iCurrIDDynTrendMonth;
	int           _iCurrIDVibTrendMonth;

	/*!在趋势表，当前趋势秒趋势保存过程中，下次记录应该保存的记录ID值，数据保留功能专用*/
	int           _iNegCurrIDVibTrendSecond;
	int           _iNegCurrIDDynTrendSecond;
	int           _iNegCurrIDProcTrendSecond;
	/*!在趋势表，小时趋势保存过程中，下次记录应该保存的记录ID值，数据保留功能专用*/
	int           _iNegCurrIDVibTrendHour;
	int           _iNegCurrIDDynTrendHour;
	int           _iNegCurrIDProcTrendHour;
	/*!在趋势表，天趋势保存过程中，下次记录应该保存的记录ID值，数据保留功能专用*/
	int           _iNegCurrIDVibTrendDay;
	int           _iNegCurrIDDynTrendDay;
	int           _iNegCurrIDProcTrendDay;
	/*!在趋势表，月趋势保存过程中，下次记录应该保存的记录ID值，数据保留功能专用*/
	int           _iNegCurrIDProcTrendMonth;
	int           _iNegCurrIDDynTrendMonth;
	int           _iNegCurrIDVibTrendMonth;

	/*!在趋势表，快变趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDVibTrendFast;
	int           _iCurrIDDynTrendFast;
	int           _iCurrIDProcTrendFast;

	/*!在趋势表，快变趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iNegCurrIDVibTrendFast;
	int           _iNegCurrIDDynTrendFast;
	int           _iNegCurrIDProcTrendFast;

public:
    string _sTimeCurr;//振动量数据更新时间
	string _sTimeCurrProc;//过程量数据更新时间
	string _sTimeCurrDyn;//动态测点数据更新时间

	//数据更新时间
	CBHDateTime _timeVibDataUpdate;
	CBHDateTime _timeDynDataUpdate;
	CBHDateTime _timeProcDataUpdate;
	COleDateTime m_oTimeLastUpdateTime;
	virtual COleDateTime TimeLastUpdateTime() const;
	virtual	void TimeLastUpdateTime(COleDateTime val);
public:

	virtual void AddVibChannInfo(CCustomDB *pDB_);
	virtual void AddDynChannInfo(CCustomDB *pDB_);
	virtual void AddProcChannInfo(CCustomDB *pDB_);
	virtual void AddChannInfoEnd(CCustomDB * pDB_);

    //增加通道自定义参数信息
	virtual void AddCustomParamInfo(CCustomDB *pDB_);
	//增加通道位置信息
    virtual void AddChannLocationsInfo(CCustomDB *pDB_);
	//增加通道部位信息
	virtual void AddChannSectionsInfo(CCustomDB *pDB_);


	virtual void AddVibChannInfo(IDBInterFace *pDB_);
	virtual void AddDynChannInfo(IDBInterFace *pDB_);
	virtual void AddProcChannInfo(IDBInterFace *pDB_);
	virtual void AddChannInfoEnd(IDBInterFace * pDB_);

	//增加通道自定义参数信息
	virtual void AddCustomParamInfo(IDBInterFace *pDB_);
	//增加通道位置信息
	virtual void AddChannLocationsInfo(IDBInterFace *pDB_);
	//增加通道部位信息
	virtual void AddChannSectionsInfo(IDBInterFace *pDB_);

	/**@brief  设置开停车的相关状态信息
	  *@param bSud_ 是否处于开停车状态
	  *@param iType_ 目前是开车状态，还是停车状态，或者未知
	  */
	virtual void SetSudStatus(bool bSud_, E_SUD_TYPE iType_);
	/**@brief  得到开停车的相关状态信息
	  *@param  bSud_ 是否处于开停车状态
	  *@return 目前是开车状态，还是停车状态，或者未知
	  */
	virtual int GetSudStatus(bool & bSud_ );

	//将获得3系列发送助手的特征值转化为4系列对应的特征值
	int ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,
								S_OldVibCharValue *pOldVibCharValue_,
								float *pfRev_,
								int iSmpNum_,
								int iSmpFreq_,
								int iRealNum_,
								int iLen_,
								int iStartIndex_);

	int ChangeDynCharValueToNew(S_DynCharValue *pDynCharValue_,
								S_OldDynCharValue *pOldDynCharValue_,
								float *pfRev_,
								const int &iSmpNum_, 
								const int &iSmpFreq_,
								const int iRealNum_,
								const int &iLen_,
								const int &iStartIndex_);

	//更新从发送助手获得的振动数据
	virtual int UpdateVibData(	DATATYPE_WAVE *pWave_,
								S_VibCharValue *pVibCharValue_,
								int iAllRealNum_,
								int iLen_,
								CBHDateTime & aoDataTime,
								int iStartIndex_,
								const float& fScaleCoef_,
								int anPrecisionEnabled,
								std::vector<float>& afPreciousMin ,
								std::vector<float>&  afPreciousMax,
								bool bEnd_=false);

	virtual int UpdateDynData(	DATATYPE_WAVE *pWave_,
								S_DynCharValue *pDynCharValue_,
								int iAllRealNum_,
								const int &iLen_,
								CBHDateTime & arefDataTime,
								const int &iStartIndex_,
								const float& fScaleCoef_,
								int anPrecisionEnabled,
								std::vector<float>& afPreciousMin ,
								std::vector<float>&  afPreciousMax,
								const bool &bEnd_=false);
    //更新过程量数据
	virtual int UpdateProcData(float *pfData_,int iLen_,const CBHDateTime& tDateTime_,int iStartIndex_=0);

	//更新无线测点状态
	virtual int UpdateVibChannStatus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex);
	virtual int UpdateProcChannStstus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex);
    void UpdateRealRev(const float *pfRev_);
	virtual int UpdateProcSUDStatus(int anProcJudgedPlantStatus,int anLen_,int anStartIndex_=0);
	virtual int GetProcSUDStatus();
	virtual bool IsAllChannUpdate();//
	virtual bool IsHasChannNotUpdate();//
	virtual bool IsPlantUpdateOverTime();

	virtual int CalcVibDataGrade(S_VibCharValue *pVibCharValue_,int iLen_,int iStartIndex_);

	/**@brief 获得实时转速
	  *@param pfRev_获取转速BUFF指针，
	  *@param iLen_ Buff长度,即获取几个实时转速
	  *@return 实际获取转速个数
	  */
    int GetRealRev(float *pfRev_, int iLen_);
	int GetLastRev(float *pfRev_, int iLen_);
	int GetRealRevFromChannel(float *pfRev_, int iLen_);
	CBHDateTime GetLastRevTime();

	///根据转速类型，得到对应的实时转速。如果bSafe_为true，则使用读写锁，如果false，则不使用读写锁
	float GetSpecTypeRealRev(const int &iRevType_=1,const bool &bSafe_=true );

    ///得到当前振动数据的更新时间
	CBHDateTime GetRealVibDataDateTime();
	///得到当前动态通道数据的更新时间
	CBHDateTime GetRealDynDataDateTime();
    ///得到当前过程量数据的更新时间
	CBHDateTime GetRealProcDataDateTime();
	virtual void SetVibDataUpdateStatus(const CBHDateTime &tDatetime_);
	virtual void SetDynDataUpdateStatus(const CBHDateTime &tDatetime_);
	virtual void SetProcDataUpdateStatus(const CBHDateTime &tDatetime_);

	virtual void GetRealSmpInfo(int &iSmpNum_,int &iSmpFreq_,bool bSafe_=true);
	virtual void GetDynRealSmpInfo(int &iSmpNum_,int &iSmpFreq_,bool bSafe_=true);
	virtual void SetRealSmpInfo(const int &iSmpNum_,const int &iSmpFreq_,bool bSafe_=true);
	virtual void SetDynRealSmpInfo(const int &iSmpNum_,const int &iSmpFreq_,bool bSafe_=true);

	virtual void GetRealAllVibWaveNum(int &iRealAllNum_,bool bSafe_=true);
	virtual void GetRealAllDynWaveNum(int &iRealAllNum_,bool bSafe_=true);

	virtual void SetRealAllVibWaveNum(const int &iRealAllNum_,bool bSafe_=true);
	virtual void SetRealAllDynWaveNum(const int &iRealAllNum_,bool bSafe_=true);

	virtual void SetVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_);
	virtual void SetVibTrendHourCurrID(const int &iCurrIDVibTrendHour_);
	virtual void SetVibTrendDayCurrID(const int &iCurrIDVibTrendDay_);
	virtual void SetVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_);

	virtual void SetDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_);
	virtual void SetDynTrendHourCurrID(const int &iCurrIDDynTrendHour_);
	virtual void SetDynTrendDayCurrID(const int &iCurrIDDynTrendDay_);
	virtual void SetDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_);

	virtual void SetProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_);
	virtual void SetProcTrendHourCurrID(const int &iCurrIDProcTrendHour_);
	virtual void SetProcTrendDayCurrID(const int &iCurrIDProcTrendDay_);
	virtual void SetProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_);

	virtual void GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_);
	virtual void GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_);
	virtual void GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_);
	virtual void GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_);

	virtual void GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_);
	virtual void GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_);
	virtual void GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_);
	virtual void GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_);

	virtual void GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_);
	virtual void GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_);
	virtual void GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_);
	virtual void GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_);

	virtual void SetNegVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_);
	virtual void SetNegVibTrendHourCurrID(const int &iCurrIDVibTrendHour_);
	virtual void SetNegVibTrendDayCurrID(const int &iCurrIDVibTrendDay_);
	virtual void SetNegVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_);

	virtual void SetNegDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_);
	virtual void SetNegDynTrendHourCurrID(const int &iCurrIDDynTrendHour_);
	virtual void SetNegDynTrendDayCurrID(const int &iCurrIDDynTrendDay_);
	virtual void SetNegDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_);

	virtual void SetNegProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_);
	virtual void SetNegProcTrendHourCurrID(const int &iCurrIDProcTrendHour_);
	virtual void SetNegProcTrendDayCurrID(const int &iCurrIDProcTrendDay_);
	virtual void SetNegProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_);

	virtual void GetNegVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_);
	virtual void GetNegVibTrendHourCurrID(int &iCurrIDVibTrendHour_);
	virtual void GetNegVibTrendDayCurrID(int &iCurrIDVibTrendDay_);
	virtual void GetNegVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_);

	virtual void GetNegDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_);
	virtual void GetNegDynTrendHourCurrID(int &iCurrIDDynTrendHour_);
	virtual void GetNegDynTrendDayCurrID(int &iCurrIDDynTrendDay_);
	virtual void GetNegDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_);

	virtual void GetNegProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_);
	virtual void GetNegProcTrendHourCurrID(int &iCurrIDProcTrendHour_);
	virtual void GetNegProcTrendDayCurrID(int &iCurrIDProcTrendDay_);
	virtual void GetNegProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_);

	virtual void GetVibTrendFastCurrID(int &iCurrIDVibTrendFast_);
	virtual void GetDynTrendFastCurrID(int &iCurrIDDynTrendFast_);
	virtual void GetProcTrendFastCurrID(int &iCurrIDProcTrendFast_);

	virtual void GetNegVibTrendFastCurrID(int &iCurrIDVibTrendFast_);
	virtual void GetNegDynTrendFastCurrID(int &iCurrIDDynTrendFast_);
	virtual void GetNegProcTrendFastCurrID(int &iCurrIDProcTrendFast_);

	virtual void SetVibTrendFastCurrID(const int &iCurrIDVibTrendFast_);
	virtual void SetDynTrendFastCurrID(const int &iCurrIDDynTrendFast_);
	virtual void SetProcTrendFastCurrID(const int &iCurrIDProcTrendFast_);

	virtual void SetNegVibTrendFastCurrID(const int &iCurrIDVibTrendFast_);
	virtual void SetNegDynTrendFastCurrID(const int &iCurrIDDynTrendFast_);
	virtual void SetNegProcTrendFastCurrID(const int &iCurrIDProcTrendFast_);


	///得到指定类型的所有测点信息
	int GetSpecTypeChannInfo(S_ChannInfo * pSchannInfo_,int iType_=GE_ALLPROC);
	///得到指定类型的所有测点信息
	int GetSpecTypeChannInfo(S_ChannInfo &sChannInfo_,int iIndex_=0,int iType_=GE_ALLPROC);

	///得到指定振动测点周期的起始索引和结束索引
	virtual int GetSpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///得到内燃机指定振动测点周期的起始索引和结束索引
	virtual int GetICESpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///得到指定动态测点周期的起始索引和结束索引
	virtual int GetSpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	
	///得到内燃机指定振动测点周期的起始索引和结束索引
	virtual int GetICESpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///得到指定振动测点周期的起始索引和结束索引
	virtual int GetSpecVibChannStartEndIndex(int index_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecVibChannStartEndIndex(string sChann_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///得到内燃机指定振动测点周期的起始索引和结束索引
	virtual int GetICESpecVibChannStartEndIndex(int index_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecVibChannStartEndIndex(string sChann_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///得到指定动态测点周期的起始索引和结束索引
	virtual int GetSpecDynChannStartEndIndex(int index_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecDynChannStartEndIndex(string sChann_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///得到内燃机指定动态测点周期的起始索引和结束索引
	virtual int GetICESpecDynChannStartEndIndex(int index_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecDynChannStartEndIndex(string sChann_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///计算整个模板的所有自定义参数数据
    virtual int CalcCustomParamData(int iStartIndex_,int iLen_,CHANNTYPE channType = GE_VIBCHANN);

	///[TBD] 计算风电机泵模块的自定义参数数据:GE_MACHINETYPE_WINDPEQ|GE_MACHINETYPE_KEYPUMP|GE_MACHINETYPE_PUMP	
	virtual int CalcCustomParamDataWindPump(int iStartIndex_,int iLen_,CHANNTYPE channType  = GE_VIBCHANN);

	///[TBD] 计算风电机泵模块的自定义参数数据:GE_MACHINETYPE_WINDPEQ|GE_MACHINETYPE_KEYPUMP|GE_MACHINETYPE_PUMP	
	
	virtual int CalcCustomParamData_New(int iStartIndex_,int iLen_,CHANNTYPE channType  = GE_VIBCHANN,BOOL abCalledFromAlarm = FALSE);

	virtual int TransferCustomParamData(	int iStartIndex_,
											int iLen_,
											std::vector<char> & arefBuffer,
											CHANNTYPE channType  = GE_VIBCHANN,
											BOOL abCalledFromAlarm = FALSE);

	virtual int CalcCustomParamData_New(int iStartIndex_,
		                                int iLen_,
										void *pCharValue_,
										DATATYPE_WAVE *pWave_,
										S_CustomParamData* pCustomParamData_,
										CHANNTYPE channType  = GE_VIBCHANN,
										BOOL abCalledFromAlarm = FALSE);

	virtual int TransferCustomParamData(	
		int iStartIndex_,
		int iLen_,
		std::vector<char> & arefBuffer,
		S_CustomParamData* pCustomParamData_,
		CHANNTYPE channType  = GE_VIBCHANN,
		BOOL abCalledFromAlarm = FALSE);


	///批次更新实时数据到供外部使用的数据（客户端）
	virtual void SyncVibData();
	virtual void SyncDynData();
	
	float MaxRealRev() const;
	void MaxRealRev(float val);


	///更新启停车状态使用

	/**设置启停车状态，并判断是否需要保存
	*@param IDBInterFace 数据库接口
	*@param bFlag_    是否处于开停车状态
	*@param bSave_    是否需要保存
	*@param iEventID_ 启停车事件id_
	*@param iSudType_ 启停车状态，
	*@return  
	*/
	virtual void SetDataStatuEx(    IDBInterFace * pDB_,
									bool bFlag_,
									long iEventID_,
									E_SUD_TYPE iSudType_);

	/**设置当前数据的时间，来自数采
	*@param dataTime_        日期
	*@param iMicroSecond_    毫秒
	*@return  
	*/
	virtual void SetDataTime(CBHDateTime dataTime_,
							 int iMicroSecond_);


	float MaxRealRevLast() const;
	void MaxRealRevLast(float val);

public:
	virtual BOOL GetVibAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );
	virtual BOOL GetDynAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );
	virtual BOOL GetProcAlarmLimitation( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );

	virtual BOOL GetVibGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );
	virtual BOOL GetDynGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );
	virtual BOOL GetProcGradeThreshold( int anChannIndex, OUT CChannAlarmParam& aChannAlarmParam );

public:
	///启停车相关参数，供临时在线使用
	///当前启停车事件的ID
	long _iCurrEventID;
	long _iEventID;
	int _iMicroSecond;
	CBHDateTime _Timesud;
protected:
	
	//在给定的频谱中，根据给定的一组频率，找到给定频率数组中，每一个最接近给定频率的幅值
	int _FindSpectrumData(
		std::map<int,float> & arefFeatureData,
		float * apCalcResultArray,
		int anResultArrayLength,
		double * arefPSpectrumValueArray,
		int anSpectrumArrayLength,
		double afDf									
		);
	
	//在给定的频谱中，根据给定的一个频率，找到最接近给定频率的幅值
	double _FindSpectrumData(
		double afFreq,
		double * arefPSpectrumValueArray,
		int anSpectrumArrayLength,
		double afDf									
		);

	//在给定的频谱中，根据给定的一组频谱幅值数组，找到最大幅值，以及对应的索引
	double _FindMaxValue(
		double * arefPSpectrumValueArray,
		int anSpectrumArrayLength,
		int & anIndex
		);
	//
	double _CalcEneryValue(		
							double * arefPSpectrumValueArray,
							int anSpectrumArrayLength
						   );
	//计算机泵功率谱
	int _CalcSpeedSpectrumPowerWindPump(
		const S_VibCharValue & aoVibCharVal,
		CVibChannParam * apVib, 
		float * apCalcResultArray,
		int anResultArrayLength,
		double * apWave,
		float afWorkFreq,
		double adblDf
		);

	//计算机泵幅值谱
	int _CalcCustomSpectrumAmpliferWindPump(
		const S_VibCharValue & aoVibCharVal,
		CVibChannParam * apVib, 
		float * apCalcResultArray,
		int anResultArrayLength,
		double * apWave,
		float afWorkFreq,
		double adblDf
		);

	//计算机泵包络谱
	int _CalcCustomSpectrumEnvelopeWindPump(
		const S_VibCharValue & aoVibCharVal,
		CVibChannParam * apVib, 
		float * apCalcResultArray,
		int anResultArrayLength,
		double * apWave,
		float afWorkFreq,
		double adblDf
		);


protected:
	///计算单条定义的自定义参数的数据
	int CalcSingleCustomParamData(CCustomParamInfo& paramInfo_,S_BearingDefectFreq& defectFreq_,S_GearBoxCharFreq& gearboxCharFreq_,const DATATYPE_WAVE *pWaveData_,int& ilen_,int iSmpFreq_,int iStartIndex_,int iEndIndex_,int iRev_,float* pfCustomParamData_,CWaveHead& lsWaveHead_);
	
	int CalcSingleCustomParamData_New(CCustomParamInfo& paramInfo_,
/*
									  S_BearingDefectFreq& defectFreq_,
									  CChannSections &    aRefSection,
									  CChannLocations &   aRefLocation,
									  S_GearBoxCharFreq& gearboxCharFreq_,
*/
									  CChannInfoBase & arefChanninfo,
									  const DATATYPE_WAVE *pWaveData_,
									  int& ilen_,
									  int iSmpFreq_,
									  int iStartIndex_,
									  int iEndIndex_,
									  int iRev_,
									  float* pfCustomParamData_,
									  BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath,
									  BufferForCustomCalc & aRefBufferObj, //计算使用的缓存对象
									  double adf
									 );

	//根据时域数据计算自定义相关参数
	void CalcWaveCustomParamValue(	const DATATYPE_WAVE *pfWave_,
									int& ilen_,
									int iSmpFreq_,
									int iStartIndex_,
									int iEndIndex_,
									int iUnitType_,
									int iSigAnaType_,
									int iSegNum_,
									int iSegStartNo_,
									int iCharTye_,
									float fStartValue_,
									float fEndValue_,
									float *fCustomParamValue_,
									CWaveHead& sWaveHead_
									);

	
	/**根据时域数据计算自定义相关参数
	/*@param pfWave_					[in]	时域波形数据                                                  
	/*@param ilen_						[in]	波形数据长度                                              
	/*@param iSmpFreq_					[in]	采样频率                                                 
	/*@param iStartIndex_				[in]	测点振动周期的开始索引                                    
	/*@param iEndIndex_					[in]	测点振动周期的结束索引                                    
	/*@param iUnitType_					[in]	自定义参数模板的开始值、结束值的单位类型                  
	/*@param iSigAnaType_				[in]	自定义参数模板的信号分析类型                              
	/*@param iSegNum_					[in]	自定义参数模板的段内自定义参数个数,以及对于波形数据横轴的分段个数                        
	/*@param iSegStartNo_				[in]	自定义参数模板的自定义参数段开始索引   
	/*@param iCharTye_					[in]	自定义参数模板的自定义参数特征值类型  
	/*@param fStartValue_				[in]	自定义参数模板的自定义参数开始值起始-无量纲  
	/*@param fEndValue_					[in]	自定义参数模板的自定义参数开始值结束-无量纲  
	/*@param fCustomParamValue_		[in/out]	计算后的自定义参数模板的自定义参数值  
	/*@param aRefBufferedSigMath		[in]	带有缓存的算法库包装类  
	/*@param aRefDoubleWaveBuffer		[in]	做过short到float转换的数组缓存  
	*/
	void CalcWaveCustomParamValue_New(	const DATATYPE_WAVE *pfWave_,									//[in]	波形数据   
										int& ilen_,														//[in]	波形数据长度        
										int iSmpFreq_,													//[in]	采样频率  
										int iStartIndex_,												//[in]	测点振动周期的开始索引
										int iEndIndex_,													//[in]	测点振动周期的结束索引 
										int iUnitType_,													//[in]	自定义参数模板的开始值、结束值的单位类型        
										int iSigAnaType_,												//[in]	自定义参数模板的信号分析类型       
										int iSegNum_,													//[in]	自定义参数模板的段内自定义参数个数	
										int iSegStartNo_,												//[in]	自定义参数模板的自定义参数段开始索引	
										int iCharTye_,													//[in]	自定义参数模板的自定义参数特征值类型	
										float fStartValue_,												//[in]	自定义参数模板的自定义参数开始值起始-无量纲	
										float fEndValue_,												//[in]	自定义参数模板的自定义参数开始值结束-无量纲	
										float *fCustomParamValue_,										//[in/out]	计算后的自定义参数模板的自定义参数值	
										BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath,	//[in]	带有缓存的算法库包装类	
										std::vector<double> & aRefDoubleWaveBuffer				//[in]	做过short到float转换的数组缓存		
										);
	//根据频域数据计算自定义相关参数
	void CalcFreqCustomParamValue(double *pfFreq_,
								  int& ilen_,
								  int iRev_,
								  int iUnitType_,
								  int iSigAnaType_,
								  int iSegNum_,
								  int iSegStartNo_,
								  int iCharTye_,
								  float fStartValue_,
								  float fEndValue_,
/*
								  S_BearingDefectFreq& defectFreq_,
								  S_GearBoxCharFreq& gearboxCharFreq_,*/

								  float *fCustomParamValue_);
	
	/**根据频域数据计算自定义相关参数
	/*@param pfWave_					[in]	频域频谱数据                                                  
	/*@param ilen_						[in]	波形数据长度                                              
	/*@param iRev_						[in]	转速                                                 
	/*@param iUnitType_					[in]	自定义参数模板的开始值、结束值的单位类型                                     
	/*@param iSigAnaType_				[in]	自定义参数模板的信号分析类型                                    
	/*@param iSegNum_					[in]	自定义参数模板的段内自定义参数个数，以及对于频谱横轴的分段个数                        
	/*@param iSegStartNo_				[in]	自定义参数模板的自定义参数段开始索引   
	/*@param iCharTye_					[in]	自定义参数模板的自定义参数特征值类型  
	/*@param fStartValue_				[in]	自定义参数模板的自定义参数开始值起始-无量纲  
	/*@param fEndValue_					[in]	自定义参数模板的自定义参数开始值结束-无量纲  
	/*@param defectFreq_				[in]	自定义参数对应测点的所在轴承缺陷频率  
	/*@param gearboxCharFreq_			[in]	自定义参数对应测点的所在齿轮箱缺陷频率  
	/*@param fCustomParamValue_		[in/out]	计算后的自定义参数模板的自定义参数值  
	/*@param adbldf						[in]	频谱横轴的频率间隔  
	*/
	void CalcFreqCustomParamValue_New(double *pfFreq_,
									  int& ilen_,
									  int iRev_,
									  int iUnitType_,
									  int iSigAnaType_,
									  int iSegNum_,
									  int iSegStartNo_,
									  int iCharTye_,
									  float fStartValue_,
									  float fEndValue_,
/*
									  S_BearingDefectFreq& defectFreq_,
									  S_GearBoxCharFreq& gearboxCharFreq_,*/

									  float *fCustomParamValue_,
									  double adbldf,
									  BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath);



	//根据滚动轴承参数信息计算缺陷特征频率结构体信息,返回滚动体的数目
	int CalcBearingDefectFreq(CBearingParamInfo &bearingParamInfo_,int iWorkRev_,S_BearingDefectFreq& defectFreq_);
	//根据齿轮箱参数信息计算特征频率结构体信息,返回齿轮级数
	int CalcGearboxCharFreq(CGearBoxParamInfo &gearBoxParamInfo_,int iWorkRev_,S_GearBoxCharFreq& gearboxCharFreq_);




};
//


class CRealPlantInfo: public CPlantInfoOnLine
{
public:
	CRealPlantInfo();
	~CRealPlantInfo();
private:
	virtual void Destory();
private:
	//下面这些bool变量为了判断上次保存数据是否已经更新过，
	bool _bVibDataUpdateHour;
	bool _bVibDataUpdateDay;
	bool _bVibDataUpdateMonth;
	bool _bVibDataUpdateSend;

	bool _bDynDataUpdateHour;
	bool _bDynDataUpdateDay;
	bool _bDynDataUpdateMonth;
	bool _bDynDataUpdateSend;

	bool _bProcDataUpdateHour;
	bool _bProcDataUpdateDay;
	bool _bProcDataUpdateMonth;
	bool _bProcDataUpdateSend;
    ///判断是否正处于保存数据状态
	bool _bSaveDataStatus;
	IReadWriteLock *_pLockSaveDataStatus;

private:
	int m_nProcJudgedRunStatus; //过程量参与判断的设备状态
	int m_nVibJudgedRunStatus; //振动参与判断的设备状态
public:
	int  SetProcJudgeRunStatus(int anProcJudgedRunStatus,int anLen_,int anStartIndex_ );
	int  GetProcJudgeRunStatus();
	void SetVibJudgeRunStatus(int anVibJudgedRunStatus);
	int  GetVibJudgeRunStatus();

    virtual void AddChannInfoEnd(CCustomDB * pDb_);
	virtual void AddChannInfoEnd(IDBInterFace * pDb_);
	virtual void SetVibDataUpdateStatus(const CBHDateTime &tDatetime_);
	virtual void SetProcDataUpdateStatus(const CBHDateTime &tDatetime_);
    virtual void SetDynDataUpdateStatus(const CBHDateTime &tDatetime_);

	virtual int ChangeVibAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int ChangeDynAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int ChangeProcAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetVibGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetDynGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetProcGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetVibAxesLocationParam(int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_);

	virtual int SetDynAxesLocationParam(int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_);

	/**得到设备的指定测点对应的转速
      *@param strChann_ 指定的测点号
	  *@param fRealRev_ 得到实时转速
	  *@return 额定转速
	  */
	float GetSpecChannRev(string strChann_,float &fRealRev_);

	///得到指定过程量测点的过程量数据
	int GetSpecChannProcData(string sChann_,float &fData_,string &sUnit_,int iChannType_=GE_ALLPROC);

	///得到指定过程量测点索引的过程量数据
	int GetSpecChannProcData(int index_,float &fData_,string &sUnit_,int iChannType_=GE_ALLPROC);

	///得到指定轴向振动特征值数据
	int GetSpecChannAxisData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_DCCHAR);
    int GetSpecChannAxisData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_DCCHAR);

	///得到指定径向振动数据
	int GetSpecChannRadialData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(string sChann_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(int index_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	///得到指定径向振动所有特征值数据
	int GetSpecChannRadialAllChar(string sChann_,S_VibCharValue &sVibData_,string &sUnit_);
	int GetSpecChannRadialAllChar(int index_,S_VibCharValue &sVibData_,string &sUnit_);
	///得到指定振动测点和特征值类型的特征数据
	int GetSpecChannAndCharTypeVibData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);

	///得到指定径向振动通道的自定义特征值
	int GetSpecRadialVibChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecRadialVibChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	///得到指定振动测点索引和特征值类型的特征数据
	int GetSpecChannAndCharTypeVibData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	///得到指定振动测点的波形和频谱数据
	int GetSpecChannVibWaveAndFreq(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_,float * pfFreq_,BOOL abSkipFreq=FALSE);
	///得到指定振动测点的波形和倒谱数据
	int GetSpecChannVibWaveAndCepstrum(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,double *pfFreqData_,int &iCepsNums_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_);
	///得到指定振动测点频谱数据
	int GetSpecChannVibFreq(string sChann_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float *pfFreq_,float& fScaleCoef_,int& PrecisionEnabled_);

	///得到指定振动测点的波形
	int GetSpecChannVibWave(string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///得到指定振动测点的波形
	int GetSpecChannVibWave(string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///得到指定动态测点的波形,返回-1，说明没有这个测点
	int GetSpecDynChannWave(const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///得到指定动态测点的波形,返回-1，说明没有这个测点
	int GetSpecDynChannWave(const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);

	///得到指定动态通道的自定义特征值
	int GetSpecDynChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecDynChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	///得到指定动态测点的波形和频谱数据。返回-1，说明没有这个测点
	int GetSpecDynChannWaveAndFreq(const string &sChannID_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_,float *pfFreq_);
    ///得到指定动态测点的特征值。返回-1，说明没有这个测点
	int GetSpecDynChannCharValue(const string &sChannID_,S_DynCharValue &sDynCharValue_);

	//得到指定索引的测点名
	string GetSpecIndexRadialVibChannID(int index_);
    string GetSpecIndexAxisVibChannID(int index_);
	string GetSpecIndexProcChannID(int index_);

	//得到指定轴心位置描述的轴心位置
    bool GetSpecChannAxisLocationData(string sLocation_,float &fX_,float &fY_);
	///得到指定水平方向和垂直方向测点的轴心位置数据
	bool GetSpecChannAxisLocationData(string sChannHor_,string sChannVer_,float &fX_,float &fY_);

	//得到指定动态侧点的活塞杆位置描述的轴心位置数据
	bool GetSpecDynIndexAxisLocationData(int index_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynIndexAxisLocationData(int index_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sLocation_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sLocation_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sChannHor_,string sChannVer_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sChannHor_,string sChannVer_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);

	/**得到指定振动测点细化谱
	  *param：
	  *       ag_strChann：string,测点号
	  *       ag_pfZoomSpec:float*,细化谱数据
	  *       ag_iLines:int&,分析线数，分析完毕后会修改实际分析出来的线数，即细化谱的数据长度
	  *       ag_fStartFreq:float&, 细化分析的起始频率，分析完毕后表示细化后的起始频率
	  *       ag_iStopFreq:int,细化分析的终止频率
	  *       ag_fDf：float&,细化谱的分辨率
	  *       ag_pfWave:波形数据
	  *       ag_iPtNum:波形数据长度
	  *return:0表示分析正确,负数表示错误码
	  */
	int GetSpecChannZoomSpec(string sChann_,DATATYPE_WAVE *pfZoomSpec_,long &iLines_,float &fStartFreq_,int iStopFreq_,float &fDf_,DATATYPE_WAVE *pfWave_,int &iPtNum_,float &fScaleCoef,int& nPrecisionEnabled_,float *pfWaveData_,float *pfFreqData_);

	// 初始化各种趋势的当前记录ID值
	void TrendInitID(CCustomDB * pDB_);
	void TrendInitID(IDBInterFace * pDB_);
	int SaveTrendData(IDBInterFace * pDb);

	int GetProcData(float *pfData_,int iNum_,int iStartIndex_=0);
	//对应4系列同步助手
	int GetAllVibData(DATATYPE_WAVE *pWave_,S_VibCharValue * pVibCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_=0);
	int GetAllDynData(DATATYPE_WAVE *pWave_,S_DynCharValue * pDynCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_=0);


	float GetVibDataWaveCoef(int iLen_,int iStartIndex_=0);
	float GetDynDataWaveCoef(int iLen_,int iStartIndex_=0);

	//得到指定测点号的启停车信息和转速
	int GetSpecChannIndex(string sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);
	//得到指定测点号的启停车信息和转速
	int GetSpecChannIndex(int index_,string &sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);

		/**得到开停车的状态、开停车类型和开停车事件ID
	  *@param iType_ 开停车类型,是引用变量
	  *@param iEventID_ 当前启停车的事件ID,是引用变量
	  *@return 开停车状态
	  */
	bool GetStatusType(E_SUD_TYPE & iType_,int & iEventID_);

	//无线测点的状态

	bool GetSpecWireLessChannStatus(int anIndex,string &asChannID, int &anChannOnline, int &anChannNormal, int &anDataUpdate,int &anBatteryPower, int anChannType);

private:
	//下面都是与保存历史数据有关
	///报警历史数据需要的各个函数和变量
	string        _sProcField;
	string        _sVibFieldSel;
	string        _sVibFieldIns;
	string        _sDynFieldSel;
	string        _sDynFieldIns;

	volatile __time64_t _timeSaveVibSecond;
	volatile __time64_t _timeSaveVibHour;
	volatile __time64_t _timeSaveVibDay;
	volatile __time64_t _timeSaveVibMonth;

	volatile __time64_t _timeSaveDynSecond;
	volatile __time64_t _timeSaveDynHour;
	volatile __time64_t _timeSaveDynDay;
	volatile __time64_t _timeSaveDynMonth;

	volatile __time64_t _timeSaveProcSecond;
	volatile __time64_t _timeSaveProcHour;
	volatile __time64_t _timeSaveProcDay;
	volatile __time64_t _timeSaveProcMonth;



	// 判断保存记录是否会造成时间重复
	bool IsDateTimeDifferent( CBHDateTime Last_, CBHDateTime Curr_);
	// 保存在当前趋势表单中得趋势数据，只保存一个小时之类得。一秒钟保存一次（在外部通过定时器控制）。
	int SaveSecondTrend(IDBInterFace * pDb_);
	// 天趋势保存
	int SaveDayTrend(IDBInterFace * pDb_);
	// 小时趋势保存
	int SaveHourTrend(IDBInterFace * pDb_);
	// 月趋势保存
	int SaveMonthTrend(IDBInterFace * pDb_);
	
	// 振动趋势记录保存
	void SaveVibRecord(IDBInterFace * pDB_,
						string sTableName_,
						int iCurID_, 
						const CBHDateTime &tDatetime_, 
						bool bMonth_=false);

	// 动态测点趋势记录保存
	void SaveDynRecord( IDBInterFace * pDB_,
						const string &sTableName_,
						const int &iCurID_,
						const CBHDateTime &tDatetime_,
						const bool &bMonth_=false);

	// 过程量趋势记录保存
	void SaveProcRecord(IDBInterFace * pDB_,
						string sTableName_,
						int iCurID_, 
						const CBHDateTime &tDatetime_, 
						bool bMonth_=false);

	/**得到指定轴心位置索引的轴心位置数据
      *@param index_ 轴心位置的索引，这个索引是指定设备在轴心位置表的顺序
	  *@param fX_    X（水平方向）的数据
      *@param fY_    Y（垂直方向）的数据
	  *@return 如果正确得到，就返回true，否则就false
	  */
    bool GetSpecIndexAxisLocationData(int index_,float &fX_,float &fY_);

// 快变数据保存
public:
	// 振动趋势记录保存
	int SaveVibRecord(
		IDBInterFace * pDB_,
		string sTableName_,
		int    iCurID_,
		DATATYPE_WAVE *pWave_,
		S_VibCharValue *pVibCharValue_,
		S_CustomParamData *pfCustomParom_,
		int iAllRealNum_,
		int iLen_,
		CBHDateTime & aoDataTime,
		int iEventID_,
		int iStartIndex_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax,
		bool bSaveWave = true);
	// 动态趋势记录保存
	int SaveDynRecord(
		IDBInterFace * pDB_,
		string sTableName_,
		int    iCurID_,
		DATATYPE_WAVE *pWave_,
		S_DynCharValue *pDynCharValue_,
		S_CustomParamData *pfCustomParom_,
		int iAllRealNum_,
		int iLen_,
		CBHDateTime & aoDataTime,
		int iEventID_,
		int iStartIndex_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax,
		bool bSaveWave = true);

	// 过程量趋势记录保存
	int SaveProcRecord(IDBInterFace *pDB_,
		string sTableName_,
		int    iCurID_,
		float *pfData_,
		int iLen_,
		const CBHDateTime& aoDateTime_,
		int iEventID_,
		int iStartIndex_);
	//保存振动快变趋势数据
	int SaveVibFastTrend(
		IDBInterFace * pDB_,
		DATATYPE_WAVE *pWave_,
		S_VibCharValue *pVibCharValue_,
		S_CustomParamData *pfCustomParom_,
		int iAllRealNum_,
		int iLen_,
		CBHDateTime & aoDataTime,
		int iStartIndex_,
		int iEventID_,
		int iAlarmStatus_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax,
		bool bEnd_=false);

	//保存动态快变趋势数据
	int SaveDynFastTrend(
		IDBInterFace * pDB_,
		DATATYPE_WAVE *pWave_,
		S_DynCharValue *pDynCharValue_,
		S_CustomParamData *pfCustomParom_,
		int iAllRealNum_,
		int iLen_,
		CBHDateTime & aoDataTime,
		int iStartIndex_,
		int iEventID_,
		int iAlarmStatus_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax,
		bool bEnd_=false);
	
	//保存过程快变趋势数据
	int SaveProcFastTrend(
		IDBInterFace *pDB_,
		float *pfData_,
		int iLen_,
		const CBHDateTime& aoDateTime_,
		int iEventID_,
		int iAlarmStatus_,
		int iStartIndex_=0);


	//设备有多个数据源时，振动非快变数据从普通趋势数据拷贝插入
	int InsertVibFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);
	//设备有多个数据源时，振动非快变数据从普通趋势数据拷贝更新
	int UpdateVibFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);

	//设备有多个数据源时，动态非快变数据从普通趋势数据拷贝插入
	int InsertDynFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);
	//设备有多个数据源时，动态非快变数据从普通趋势数据拷贝更新
	int UpdateDynFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);

	//设备有多个数据源时，过程非快变数据从普通趋势数据拷贝插入
	int InsertProcFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_);
	//设备有多个数据源时，过程非快变数据从普通趋势数据拷贝更新
	int UpdateProcFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_);


	//设备有多个数据源时，振动非快变数据从普通趋势表拷贝插入
	int InsertVibFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);
	//设备有多个数据源时，振动非快变数据从普通趋势表拷贝更新
	int UpdateVibFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);

	//设备有多个数据源时，动态非快变数据从普通趋势表拷贝插入
	int InsertDynFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);
	//设备有多个数据源时，动态非快变数据从普通趋势表拷贝更新
	int UpdateDynFastDataromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);

	//设备有多个数据源时，过程量非快变数据从普通趋势表拷贝插入
	int InsertProcFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_);
	//设备有多个数据源时，过程量非快变数据从普通趋势表拷贝更新
	int UpdateProcFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_);


	bool IsRecordExist(IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_);
	int GetRecordDataID(IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_);


	//将保留数据时间段内的记录ID置为负值
	int SaveSpecTimeData(IDBInterFace * pDB_,string sTable_,CBHDateTime tDateTime_,int nCurrID_);

	//保留数据时间段缓存并写道文件
	int SaveSpecTime(IDBInterFace * pDB_,const std::pair<CBHDateTime,CBHDateTime>& apDatetime,const CString& sTblName_,int nStatus_);

	//删除掉已完成数据保留操作的时间段
	int RemoveSpecTime(IDBInterFace * pDB_,const string& strTblName_);

	//初始化负向增长数据ID
	int InitTableNegCurID(IDBInterFace *pDB_,string sTable_);

	//将趋势表ID置0，开始更新数据
	int ResetTrendTableCurID(IDBInterFace *pDB_,string sTable_,int nCurrID_);

	///是否存储振动波形
	bool ShouldSaveVibWaveData(IDBInterFace *pDB_,string sTable_,CVibChannParam* pVibChannParam_,const S_VibCharValue& aCharValue_);
	///是否存储动态波形
	bool ShouldSaveDynWaveData(IDBInterFace *pDB_,string sTable_,CDynChannParam* pDynChannParam_,const S_DynCharValue& aCharValue_);

	bool VibSecTblSaveTimeReached();

	bool VibHourTblSaveTimeReached();

	bool VibDayTblSaveTimeReached();

	bool VibMonthTblSaveTimeReached();

	bool DynSecTblSaveTimeReached();

	bool DynHourTblSaveTimeReached();

	bool DynDayTblSaveTimeReached();

	bool DynMonthTblSaveTimeReached();

	int  VibSecTblRecordNum();
	void VibSecTblRecordNum(int nRecordNum_);

	int  VibHourTblRecordNum();
	void VibHourTblRecordNum(int nRecordNum_);

	int  VibDayTblRecordNum();
	void VibDayTblRecordNum(int nRecordNum_);

	int  VibMonthTblRecordNum();
	void VibMonthTblRecordNum(int nRecordNum_);

	int  DynSecTblRecordNum();
	void DynSecTblRecordNum(int nRecordNum_);

	int  DynHourTblRecordNum();
	void DynHourTblRecordNum(int nRecordNum_);

	int  DynDayTblRecordNum();
	void DynDayTblRecordNum(int nRecordNum_);

	int  DynMonthTblRecordNum();
	void DynMonthTblRecordNum(int nRecordNum_);


private:
	///振动测点的波形字段
	vector<CString>  _vVibWaveField;
	///动态测点的波形字段
	vector<CString> _vDynWaveField;

	CBHDateTime _tVibLastSaveTime;

	CBHDateTime _tDynLastSaveTime;

	CBHDateTime _tProcLastSaveTime;

	int _iVibSecTblSaveCount;

	int _iVibHourTblSaveCount;

	int _iVibDayTblSaveCount;

	int _iVibMonthTblSaveCount;

	int _iDynSecTblSaveCount;

	int _iDynHourTblSaveCount;

	int _iDynDayTblSaveCount;

	int _iDynMonthTblSaveCount;

	int         _iVibSecTblRecordNum;
	int         _iVibDayTblRecordNum;
	int         _iVibHourTblRecordNum;
	int         _iVibMonthTblRecordNum;

	int         _iDynSecTblRecordNum;
	int         _iDynDayTblRecordNum;
	int         _iDynHourTblRecordNum;
	int         _iDynMonthTblRecordNum;


	

};

extern BOOL SaveDetainDataTime();