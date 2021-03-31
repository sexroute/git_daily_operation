#ifndef _BHKD_VIBDATA_H_MARBLE_
#define _BHKD_VIBDATA_H_MARBLE_
#include "StdAfx.h"
#include "PlantInfoBase.h"
class CVibChannParam:public CChannInfoBase
{
	public:
		CVibChannParam();
		virtual ~CVibChannParam();
		virtual void SetChannInfo(const S_ChannInfo &channInfo_);

		virtual bool SetLastUpdateTime(CBHDateTime & arefDatatime)
		{
			this->m_oUpdateTime = arefDatatime;

			return true;
		};

		virtual  CBHDateTime GetLastUpdateTime()
		{
			return this->m_oUpdateTime;
		};

		virtual void SetChannUpdate(bool abChannUpdate)
		{
			this->m_bChannUpdate = abChannUpdate;
		}

		virtual bool GetChannUpdate()
		{
			return this->m_bChannUpdate;
		}


		virtual bool SetChannValue(BHKD::S_VibCharValue *pVibCharValue_);
		/**@brief 设置当前的通道的波形数据
		  *@param pfWave_ 波形数据指针
		  *@param iSmpNum_ 波形数据采样点数
		  *@return true denotes successed, or false denotes failed
		  */
		virtual bool SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_);

        /**@brief 得到当前通道波形数据指针
		  *@return 通道波形数据指针
		  */
        virtual const DATATYPE_WAVE * GetCurrWavePoint();

		virtual int GetWaveNum();

		virtual const DATATYPE_WAVE * GetCurrRealWavePoint();


		/**@brief 指定波形数据缓存的指针.
		*@param pWave_ 波形数据缓存指针
		*@param iSmpNum 波形数据的采样点数
		*/
		virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

		/**@brief 指定波形数据缓存的指针.
		*@param pWave_ 波形数据缓存指针
		*@param iSmpNum 波形数据的采样点数
		*/
		virtual void SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_);

		///得到所有特征值字段
		string GetCharValueField();

		/**得到指定类型的特征值字段，-1波形，-2特征类型
		  */
		string GetSpecCharTypeField(int iCharType_=GE_ALLCHAR);

		/**得到所有特征值
		*/
		const S_VibCharValue GetCharValue() const;


		//得到全部特征值
		virtual  S_VibCharValue GetCharValueEx() ;
		//得到即时的特征值
		virtual  S_VibCharValue GetRealCharValueEx();

		virtual  S_VibCharValue GetCharValueEx(int index_) ;
		
		//得到自定义参数数组
		virtual  float * GetCustomParamData();
		virtual  float * GetRealCustomParamData();
		virtual void SyncData();

		/************************************************************************/
		/* 是否需要计算特征值                                                   */
		/************************************************************************/
		virtual BOOL IsNeedComputeEigenValue();

		/************************************************************************/
		/* 特征值是否没有计算                                                   */
		/************************************************************************/
		virtual BOOL IsEigenValueEmpty();

		/************************************************************************/
		/* 计算特征值															 */
		/************************************************************************/
		virtual BOOL ComputeEigenValue(int aEnumMachineType,int anSelftIndexInPlant);

		/************************************************************************/
		/* 判断是否要保存秒表波形												*/
		/************************************************************************/
		virtual BOOL ShouldSaveSecondTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* 判断是否要保存小时表波形												*/
		/************************************************************************/
		virtual BOOL ShouldSaveHourTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* 判断是否要保存天表波形												*/
		/************************************************************************/
		virtual BOOL ShouldSaveDayTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* 判断是否要保存月表波形												*/
		/************************************************************************/
		virtual BOOL ShouldSaveMonthTblWave(const S_VibCharValue& aCharValue_);

		void SecondTblLastValue(const float afValue_);
		float SecondTblLastValue();

		void HourTblLastValue(const float afValue_);
		float HourTblLastValue();

		void DayTblLastValue(const float afValue_);
		float DayTblLastValue();

		void MonthTblLastValue(const float afValue_);
		float MonthTblLastValue();

		void SecondTblLastOnePhaseValue(const float afValue_);
		float  SecondTblLastOnePhaseValue();

		void HourTblLastOnePhaseValue(const float afValue_);
		float  HourTblLastOnePhaseValue();

		void DayTblLastOnePhaseValue(const float afValue_);
		float  DayTblLastOnePhaseValue();

		void MonthTblLastOnePhaseValue(const float afValue_);
		float  MonthTblLastOnePhaseValue();

		void ChannOnline(const int anOnLine);
		int  ChannOnline();

		void ChannNormal(const int anNormal);
		int  ChannNormal();

		void ChannDataUpdate(const int anDataUpdate);
		int  ChannDataUpdate();

		void BatteryPower(const int anBatteryPower);
		int  BatteryPower();

   protected:
		DATATYPE_WAVE *_pChannWave;
		int _nChannWavePointNum;//modified by zdl,所分配波形内存的元素的个数，单位点数
		string _sAllCharField;
		string _sCharAll;
		string _sCharOne;
		string _sCharTwo;
		string _sCharThree;
		string _sCharHalf;
		string _sDC;
		string _sGap;
		string _sCharRes;
		string _sPhaseOne;
		string _sPhaseTwo;
		string _sPhaseThree;
		string _sPhaseHalf;
		string _sWave;
	    string _sSmpFreq;
		string _sSmpNum;
		string _sRev;
    protected:
		//根据自定义参数信息计算的数组
		float  _fCustomParamData[GD_CUSTOMPARAM_BUFFSIZE];
		S_VibCharValue _sVibCharValue;
	
		//根据自定义参数信息计算的数组即时值
		S_VibCharValue _sVibCharValue_Real;		
		float  _fCustomParamData_Real[GD_CUSTOMPARAM_BUFFSIZE];
		DATATYPE_WAVE *_pChannWave_real;

		float _fSecondTblLastValue;
		float _fHourTblLastValue;
		float _fDayTblLastValue;
		float _fMonthTblLastValue;

		float _fSecondTblLastOnePhaseValue;
		float _fHourTblLastOnePhaseValue;
		float _fDayTblLastOnePhaseValue;
		float _fMonthTblLastOnePhaseValue;

		//测点是否在线
		int m_nChannOnline;
		//测点是否正常
		int m_nChannNormal;
		//数据是否正常
		int m_nDataUpdate;
		//电池电量
		int m_nBatteryPower;

		
};

class CDynChannParam :public CChannInfoBase
{
public:
	CDynChannParam();
	virtual ~CDynChannParam();
	virtual bool SetChannValue(const BHKD::S_DynCharValue *pDynCharValue_);
	virtual void SetChannInfo(const S_ChannInfo &channInfo_);

	/**@brief 设置当前的通道的波形数据
	  *@param pfWave_ 波形数据指针
	  *@param iSmpNum_ 波形数据采样点数
	  *@return true denotes successes, or false denotes failed
	  */
	virtual bool SetChannWave(const DATATYPE_WAVE *pfWave_,const int &iSmpNum_);

	/**@brief 得到当前通道波形数据指针
	*@return 通道波形数据指针
	*/
	virtual const DATATYPE_WAVE * GetCurrWavePoint();

	virtual const DATATYPE_WAVE * GetCurrRealWavePoint();

	virtual int GetWaveNum();

	virtual bool SetLastUpdateTime(CBHDateTime & arefDatatime)
	{
		this->m_oUpdateTime = arefDatatime;

		return true;
	};

	virtual  CBHDateTime GetLastUpdateTime()
	{
		return this->m_oUpdateTime;
	};

	virtual void SetChannUpdate(bool abChannUpdate)
	{
		this->m_bChannUpdate = abChannUpdate;
	}

	virtual bool GetChannUpdate()
	{
		return this->m_bChannUpdate;
	}

	/**@brief 指定波形数据缓存的指针.
	*@param pWave_ 波形数据缓存指针
	*@param iSmpNum 波形数据的采样点数
	*/
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

			/**@brief 指定波形数据缓存的指针.
		*@param pWave_ 波形数据缓存指针
		*@param iSmpNum 波形数据的采样点数
		*/
	virtual void SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_);

	///得到所有特征值字段
	string GetCharValueField();

	/**得到指定类型的特征值字段，-1波形，-2特征类型
	  */
	string GetSpecCharTypeField(int iCharType_=0);

   /**得到所有特征值
     */
	const S_DynCharValue GetCharValue() const;

	//得到全部特征值
	virtual  S_DynCharValue GetCharValueEx() ;

	virtual  S_DynCharValue GetRealCharValueEx() ;


	virtual  S_DynCharValue GetCharValueEx(int index_) ;

	//得到自定义参数数组
	virtual  float * GetCustomParamData();
	virtual  float * GetRealCustomParamData();
	virtual void SyncData();

	/************************************************************************/
	/* 是否需要计算特征值                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue();

	/************************************************************************/
	/* 特征值是否没有计算                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty();

	/************************************************************************/
	/* 计算特征值															 */
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anCurrentIndexInPlant);

	/* 判断是否要保存秒表波形												*/
	/************************************************************************/
	virtual BOOL ShouldSaveSecondTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* 判断是否要保存小时表波形												*/
	/************************************************************************/
	virtual BOOL ShouldSaveHourTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* 判断是否要保存天表波形												*/
	/************************************************************************/
	virtual BOOL ShouldSaveDayTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* 判断是否要保存月表波形												*/
	/************************************************************************/
	virtual BOOL ShouldSaveMonthTblWave(const S_DynCharValue& aCharValue_);

	void SecondTblLastValue(const float afValue_);
	float SecondTblLastValue();

	void HourTblLastValue(const float afValue_);
	float HourTblLastValue();

	void DayTblLastValue(const float afValue_);
	float DayTblLastValue();

	void MonthTblLastValue(const float afValue_);
	float MonthTblLastValue();

protected:
	DATATYPE_WAVE *_pChannWave;
	int _nChannWavePointNum;//modified by zdl,所分配波形内存的元素的个数，单位点数
	string _sAllCharField;
	string _sCharZero;
	string _sCharOne;
	string _sCharTwo;
	string _sCharThree;
	string _sCharFour;
	string _sCharFive;
	string _sCharSix;
	string _sCharSeven;
	string _sCharEight;
	string _sCharNine;
    string _sCharTen;
    string _sCharEleven;
	string _sWave;
	string _sSmpFreq;
	string _sSmpNum;
	string _sRev;
protected:
	S_DynCharValue _sDynCharValue;
	//根据自定义参数信息计算的数组
	float  _fCustomParamData[GD_CUSTOMPARAM_BUFFSIZE];


	S_DynCharValue _sDynCharValue_real;
	//根据自定义参数信息计算的数组即时值
	float  _fCustomParamData_real[GD_CUSTOMPARAM_BUFFSIZE];

	DATATYPE_WAVE *_pChannWave_real;

	float _fSecondTblLastValue;
	float _fHourTblLastValue;
	float _fDayTblLastValue;
	float _fMonthTblLastValue;

};

class CProcChannParam:public CChannInfoBase
{
public:
	CProcChannParam();
    ~CProcChannParam();

	virtual void SetChannUpdate(bool abChannUpdate)
	{
		this->m_bChannUpdate = abChannUpdate;
	}

	virtual bool GetChannUpdate()
	{
		return this->m_bChannUpdate;
	}

	virtual void SetLastUpdateTime(CBHDateTime aoLastUpdateTime)
	{
		this->m_dtLastUpdateTime = aoLastUpdateTime;
	}

	virtual CBHDateTime GetLastUpdateTime()
	{
		return this->m_dtLastUpdateTime;
	}

	virtual void SetData(float fValue_,CBHDateTime dataTime_);
	virtual float GetData();

	virtual void SetSUDStatus(int anSUDStatus);
	virtual float GetSUDStatus();

	void ChannOnline(const int anOnLine);
	int  ChannOnline();

	void ChannNormal(const int anNormal);
	int  ChannNormal();

	void ChannDataUpdate(const int anDataUpdate);
	int  ChannDataUpdate();

	void BatteryPower(const int anBatteryPower);
	int  BatteryPower();
	virtual CBHDateTime GetUpdateTime();
	virtual bool  IsChannUpdateDataOverTime();
protected:
	float _fValue;

	//测点是否在线
	int m_nChannOnline;
	//测点是否正常
	int m_nChannNormal;
	//数据是否正常
	int m_nDataUpdate;
	//电池电量
	int m_nBatteryPower;

	CBHDateTime m_dtUpdateData;
	int   m_nProcSUDStatus;//-1:停车 0:未判断 1：开车
	CBHDateTime m_dtLastUpdateTime;//测点最近一次更新时间，取当前服务器时间
};

class CAxisChannParam:   public CAxisChannInfoBase
{
public:
	CAxisChannParam();
	virtual ~CAxisChannParam();
public:
	void  SetChannInfoEx(CAxisChannInfo channInfo_,int iVerIndex_,int iHorIndex_,int iRevType_,float fSensitivityVer_,float fSensitivityHor_);
	void  SetParentPoint(LPVOID pParent_);
	void  SetDataLock(IReadWriteLock *pLock_);
	virtual bool  UpdateData();
protected:
	LPVOID _pParent;
	//负责是否_fCoorVer,_fCoorHor,_bCalcLocation变量的状态读写
	IReadWriteLock *_plockRW;
	//是否已经计算过程轴心位置
	bool _bCalcLocation;
	int _iVerIndex;//对应于父级中振动测点的位置信息,为获取数据使用.
	int _iHorIndex;//对应于父级中振动测点的位置信息,为获取数据使用.
    //水平方向灵敏度
	float _fSensitivityHor;
	//垂直方向灵敏度
	float _fSensitivityVer;
	S_AxisData _axisData;
	///转速类型,属于第一转速还是第二转速
	int _iRevType;
};


#endif //_BHKD_VIBDATA_H_MARBLE_