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
		/**@brief ���õ�ǰ��ͨ���Ĳ�������
		  *@param pfWave_ ��������ָ��
		  *@param iSmpNum_ �������ݲ�������
		  *@return true denotes successed, or false denotes failed
		  */
		virtual bool SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_);

        /**@brief �õ���ǰͨ����������ָ��
		  *@return ͨ����������ָ��
		  */
        virtual const DATATYPE_WAVE * GetCurrWavePoint();

		virtual int GetWaveNum();

		virtual const DATATYPE_WAVE * GetCurrRealWavePoint();


		/**@brief ָ���������ݻ����ָ��.
		*@param pWave_ �������ݻ���ָ��
		*@param iSmpNum �������ݵĲ�������
		*/
		virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

		/**@brief ָ���������ݻ����ָ��.
		*@param pWave_ �������ݻ���ָ��
		*@param iSmpNum �������ݵĲ�������
		*/
		virtual void SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_);

		///�õ���������ֵ�ֶ�
		string GetCharValueField();

		/**�õ�ָ�����͵�����ֵ�ֶΣ�-1���Σ�-2��������
		  */
		string GetSpecCharTypeField(int iCharType_=GE_ALLCHAR);

		/**�õ���������ֵ
		*/
		const S_VibCharValue GetCharValue() const;


		//�õ�ȫ������ֵ
		virtual  S_VibCharValue GetCharValueEx() ;
		//�õ���ʱ������ֵ
		virtual  S_VibCharValue GetRealCharValueEx();

		virtual  S_VibCharValue GetCharValueEx(int index_) ;
		
		//�õ��Զ����������
		virtual  float * GetCustomParamData();
		virtual  float * GetRealCustomParamData();
		virtual void SyncData();

		/************************************************************************/
		/* �Ƿ���Ҫ��������ֵ                                                   */
		/************************************************************************/
		virtual BOOL IsNeedComputeEigenValue();

		/************************************************************************/
		/* ����ֵ�Ƿ�û�м���                                                   */
		/************************************************************************/
		virtual BOOL IsEigenValueEmpty();

		/************************************************************************/
		/* ��������ֵ															 */
		/************************************************************************/
		virtual BOOL ComputeEigenValue(int aEnumMachineType,int anSelftIndexInPlant);

		/************************************************************************/
		/* �ж��Ƿ�Ҫ���������												*/
		/************************************************************************/
		virtual BOOL ShouldSaveSecondTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* �ж��Ƿ�Ҫ����Сʱ����												*/
		/************************************************************************/
		virtual BOOL ShouldSaveHourTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* �ж��Ƿ�Ҫ���������												*/
		/************************************************************************/
		virtual BOOL ShouldSaveDayTblWave(const S_VibCharValue& aCharValue_);

		/************************************************************************/
		/* �ж��Ƿ�Ҫ�����±���												*/
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
		int _nChannWavePointNum;//modified by zdl,�����䲨���ڴ��Ԫ�صĸ�������λ����
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
		//�����Զ��������Ϣ���������
		float  _fCustomParamData[GD_CUSTOMPARAM_BUFFSIZE];
		S_VibCharValue _sVibCharValue;
	
		//�����Զ��������Ϣ��������鼴ʱֵ
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

		//����Ƿ�����
		int m_nChannOnline;
		//����Ƿ�����
		int m_nChannNormal;
		//�����Ƿ�����
		int m_nDataUpdate;
		//��ص���
		int m_nBatteryPower;

		
};

class CDynChannParam :public CChannInfoBase
{
public:
	CDynChannParam();
	virtual ~CDynChannParam();
	virtual bool SetChannValue(const BHKD::S_DynCharValue *pDynCharValue_);
	virtual void SetChannInfo(const S_ChannInfo &channInfo_);

	/**@brief ���õ�ǰ��ͨ���Ĳ�������
	  *@param pfWave_ ��������ָ��
	  *@param iSmpNum_ �������ݲ�������
	  *@return true denotes successes, or false denotes failed
	  */
	virtual bool SetChannWave(const DATATYPE_WAVE *pfWave_,const int &iSmpNum_);

	/**@brief �õ���ǰͨ����������ָ��
	*@return ͨ����������ָ��
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

	/**@brief ָ���������ݻ����ָ��.
	*@param pWave_ �������ݻ���ָ��
	*@param iSmpNum �������ݵĲ�������
	*/
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

			/**@brief ָ���������ݻ����ָ��.
		*@param pWave_ �������ݻ���ָ��
		*@param iSmpNum �������ݵĲ�������
		*/
	virtual void SetWaveMemoryPoint_real(DATATYPE_WAVE *pWave_,int iSmpNum_);

	///�õ���������ֵ�ֶ�
	string GetCharValueField();

	/**�õ�ָ�����͵�����ֵ�ֶΣ�-1���Σ�-2��������
	  */
	string GetSpecCharTypeField(int iCharType_=0);

   /**�õ���������ֵ
     */
	const S_DynCharValue GetCharValue() const;

	//�õ�ȫ������ֵ
	virtual  S_DynCharValue GetCharValueEx() ;

	virtual  S_DynCharValue GetRealCharValueEx() ;


	virtual  S_DynCharValue GetCharValueEx(int index_) ;

	//�õ��Զ����������
	virtual  float * GetCustomParamData();
	virtual  float * GetRealCustomParamData();
	virtual void SyncData();

	/************************************************************************/
	/* �Ƿ���Ҫ��������ֵ                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue();

	/************************************************************************/
	/* ����ֵ�Ƿ�û�м���                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty();

	/************************************************************************/
	/* ��������ֵ															 */
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anCurrentIndexInPlant);

	/* �ж��Ƿ�Ҫ���������												*/
	/************************************************************************/
	virtual BOOL ShouldSaveSecondTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* �ж��Ƿ�Ҫ����Сʱ����												*/
	/************************************************************************/
	virtual BOOL ShouldSaveHourTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* �ж��Ƿ�Ҫ���������												*/
	/************************************************************************/
	virtual BOOL ShouldSaveDayTblWave(const S_DynCharValue& aCharValue_);

	/************************************************************************/
	/* �ж��Ƿ�Ҫ�����±���												*/
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
	int _nChannWavePointNum;//modified by zdl,�����䲨���ڴ��Ԫ�صĸ�������λ����
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
	//�����Զ��������Ϣ���������
	float  _fCustomParamData[GD_CUSTOMPARAM_BUFFSIZE];


	S_DynCharValue _sDynCharValue_real;
	//�����Զ��������Ϣ��������鼴ʱֵ
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

	//����Ƿ�����
	int m_nChannOnline;
	//����Ƿ�����
	int m_nChannNormal;
	//�����Ƿ�����
	int m_nDataUpdate;
	//��ص���
	int m_nBatteryPower;

	CBHDateTime m_dtUpdateData;
	int   m_nProcSUDStatus;//-1:ͣ�� 0:δ�ж� 1������
	CBHDateTime m_dtLastUpdateTime;//������һ�θ���ʱ�䣬ȡ��ǰ������ʱ��
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
	//�����Ƿ�_fCoorVer,_fCoorHor,_bCalcLocation������״̬��д
	IReadWriteLock *_plockRW;
	//�Ƿ��Ѿ������������λ��
	bool _bCalcLocation;
	int _iVerIndex;//��Ӧ�ڸ������񶯲���λ����Ϣ,Ϊ��ȡ����ʹ��.
	int _iHorIndex;//��Ӧ�ڸ������񶯲���λ����Ϣ,Ϊ��ȡ����ʹ��.
    //ˮƽ����������
	float _fSensitivityHor;
	//��ֱ����������
	float _fSensitivityVer;
	S_AxisData _axisData;
	///ת������,���ڵ�һת�ٻ��ǵڶ�ת��
	int _iRevType;
};


#endif //_BHKD_VIBDATA_H_MARBLE_