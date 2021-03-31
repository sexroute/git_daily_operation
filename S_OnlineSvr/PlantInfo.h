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

	/**�����Ϸ��������������������gSEֵ
	  *@param pwave_ 1ά����������Ϊilen_
	  *@param ilen_ �㼯�ĳ���
	  *@param iFs_  ����Ƶ��
	  *@param iCutoffFreq_ ��ͨ�˲��Ľ�ֹƵ��
	  *@param fgSEValue_ ���ؼ���õ���gSEֵ
	  *@param pEnvWave_ ���س����Ϸ�����Ĳ��Σ�����Ԥ�ȷ�����ڴ棬����Ϊilen_���������Ҫ���ؾ�����ΪNULL
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�
	  *@return С��0��ʾʧ��
	  */

	int gSEAnalysis(const double* pwave_, 
					const int& ilen_, 
					const int& iFs_,
					const int& iCutoffFreq_, 
					double & fgSEValue_, 
					double *pEnvWave_ = NULL,
					unsigned int iPrecision_ = 2);

	/**@brief Ƶ�׷���
	*@param pwave ʱ��������
	*@param n     ���εĳ���
	*@param pout  Ƶ�����ݵ�������������ȷ�����ڴ棬����Ϊn/2
	*@param type  Ƶ������0����ֵ�ף�1:������
	*@param iWinType_ �Ӵ������ͣ��������Ͷ��塣
	*@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�
	*/

	int SpectralMeasureMent(const double *pwave,
							int n,double *pout,
							E_SpectrumType type,
							E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
							unsigned int iPrecision_ = 2);
	///�����vOut_����Ϊn/2��
	int SpectralMeasureMent(const double *pwave,
							int n,vector<double>& vOut_,
							E_SpectrumType type,
							E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
							unsigned int iPrecision_ = 2);

	/**��Ƶ�׽��л���
	*@param pSpectrum ����Ҫ���ֵ�Ƶ�����ݣ�����ɹ���Ϊ���ֺ�Ľ��
	*@param df Ƶ�׷ֱ���
	*@param ilen_ pSpectrum���ݵ���Ŀ
	*@param fSensitivity_ ������ϵ�� �����(���ֺ�)=����/fSensitivity_
	*@param eSpectrumType_  ��Ҫ����Ƶ�����ݵ�Ƶ������
	*@param iStartidx_ ��ʼ���ֵ�����
	*@return  0 ��ʾ��ȷ��С��0��ʾ����
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
	std::vector<double> m_ovDoubleWaveBuffer;					//short 2 double��Ļ���
	std::vector<double> m_ovMapIntegrateDoubleWaveBuffer;		//short 2 double���ֺ�Ļ���
	std::vector<double> m_ovMapDiffDoubleWaveBuffer;			//short 2 double΢�ֺ�Ļ���
	std::vector<double> m_ovMapDoubleWaveBufferSpectrum;		//Ƶ�׼����õ���ʱ����
	std::vector<double> m_ovMapDoubleEnvelopeWaveBuffer;		//Ƶ�׼�����粨�εĻ���
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
	//ɾ��һЩ���������
	virtual void Destory();
protected:
	vector<S_AxisData>     _vAxisChannValue;

	//��������ָ��,����״̬����
	DATATYPE_WAVE *_pWave;
	//��̬��㲨������ָ��,����״̬����
	DATATYPE_WAVE *_pWaveDyn;

	//��������ָ�룬ʵʱ���£�����״̬��һ������
	DATATYPE_WAVE *_pWave_real;
	//��̬��㲨������ָ�룬ʵʱ���£�����״̬��һ������
	DATATYPE_WAVE *_pWaveDyn_real;

	//ʵʱת��1~5
	float _pfRealRev[5];
	float _pfLastRev[5];
	float		  m_fMaxRealRev;
	long		  m_lMaxRealRevLast;

	float		  m_fMaxRev;

	CBHDateTime _tLastRevTime;

	//�����Ƿ����
    bool _bVibDataUpdate;
	bool _bProcDataUpdate;
	bool _bDynDataUpdate;
    //�����Ϣ����
	bool _bVibChannInfoUpdate;
	bool _bDynChannInfoUpdate;

	//���ܵ������źŵ����в��ε���
	int _iVibRealAllNum;
	//���ܵ��Ķ�̬�źŵ����в��ε���
	int _iDynRealAllNum;

	//һ���豸�������źŵ��ܵĲ��ε���
	int _iVibAllWaveNum;
	//һ���豸���ж�̬�źŵ��ܵĲ��ε���
	int _iDynAllWaveNum;

	///��ͣ������
	E_SUD_TYPE _iType;
	///�ж��Ƿ�����ͣ��״̬
	bool _bSudStatus;
	///��������,����ת��,��������
	IReadWriteLock *_pLockVibData;

	///��̬ͨ��������,������������
	IReadWriteLock *_pLockDynData;

	///������������
	IReadWriteLock *_pLockProcData;
public:
	//��������״̬��
	IReadWriteLock *_pLockDataUpdateStatues;

	//����λ�����ݶ�д��
	IReadWriteLock * _pLockAxisLocationData;
private:
	 //ʵʱ��������
	int _iRealSmpNum;
	//ʵʱ����Ƶ��
	int _iRealSmpFreq;
	//��̬�źŵĲ���Ƶ��
	int _iDynRealSmpFreq;
	//��̬�źŵĲ�������
	int _iDynRealSmpNum;

public:
	/*!�����Ʊ���ǰ���������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ����������ܹ���3600����¼����Сʱ����Ϊ��������ⲿͨ����ʱ������Ϊһ����ִ��һ��*/
	int           _iCurrIDVibTrendSecond;
	int           _iCurrIDDynTrendSecond;
	int           _iCurrIDProcTrendSecond;
	/*!�����Ʊ�Сʱ���Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDVibTrendHour;
	int           _iCurrIDDynTrendHour;
	int           _iCurrIDProcTrendHour;
	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDVibTrendDay;
	int           _iCurrIDDynTrendDay;
	int           _iCurrIDProcTrendDay;
	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDProcTrendMonth;
	int           _iCurrIDDynTrendMonth;
	int           _iCurrIDVibTrendMonth;

	/*!�����Ʊ���ǰ���������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ�����ݱ�������ר��*/
	int           _iNegCurrIDVibTrendSecond;
	int           _iNegCurrIDDynTrendSecond;
	int           _iNegCurrIDProcTrendSecond;
	/*!�����Ʊ�Сʱ���Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ�����ݱ�������ר��*/
	int           _iNegCurrIDVibTrendHour;
	int           _iNegCurrIDDynTrendHour;
	int           _iNegCurrIDProcTrendHour;
	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ�����ݱ�������ר��*/
	int           _iNegCurrIDVibTrendDay;
	int           _iNegCurrIDDynTrendDay;
	int           _iNegCurrIDProcTrendDay;
	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ�����ݱ�������ר��*/
	int           _iNegCurrIDProcTrendMonth;
	int           _iNegCurrIDDynTrendMonth;
	int           _iNegCurrIDVibTrendMonth;

	/*!�����Ʊ�������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDVibTrendFast;
	int           _iCurrIDDynTrendFast;
	int           _iCurrIDProcTrendFast;

	/*!�����Ʊ�������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iNegCurrIDVibTrendFast;
	int           _iNegCurrIDDynTrendFast;
	int           _iNegCurrIDProcTrendFast;

public:
    string _sTimeCurr;//�������ݸ���ʱ��
	string _sTimeCurrProc;//���������ݸ���ʱ��
	string _sTimeCurrDyn;//��̬������ݸ���ʱ��

	//���ݸ���ʱ��
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

    //����ͨ���Զ��������Ϣ
	virtual void AddCustomParamInfo(CCustomDB *pDB_);
	//����ͨ��λ����Ϣ
    virtual void AddChannLocationsInfo(CCustomDB *pDB_);
	//����ͨ����λ��Ϣ
	virtual void AddChannSectionsInfo(CCustomDB *pDB_);


	virtual void AddVibChannInfo(IDBInterFace *pDB_);
	virtual void AddDynChannInfo(IDBInterFace *pDB_);
	virtual void AddProcChannInfo(IDBInterFace *pDB_);
	virtual void AddChannInfoEnd(IDBInterFace * pDB_);

	//����ͨ���Զ��������Ϣ
	virtual void AddCustomParamInfo(IDBInterFace *pDB_);
	//����ͨ��λ����Ϣ
	virtual void AddChannLocationsInfo(IDBInterFace *pDB_);
	//����ͨ����λ��Ϣ
	virtual void AddChannSectionsInfo(IDBInterFace *pDB_);

	/**@brief  ���ÿ�ͣ�������״̬��Ϣ
	  *@param bSud_ �Ƿ��ڿ�ͣ��״̬
	  *@param iType_ Ŀǰ�ǿ���״̬������ͣ��״̬������δ֪
	  */
	virtual void SetSudStatus(bool bSud_, E_SUD_TYPE iType_);
	/**@brief  �õ���ͣ�������״̬��Ϣ
	  *@param  bSud_ �Ƿ��ڿ�ͣ��״̬
	  *@return Ŀǰ�ǿ���״̬������ͣ��״̬������δ֪
	  */
	virtual int GetSudStatus(bool & bSud_ );

	//�����3ϵ�з������ֵ�����ֵת��Ϊ4ϵ�ж�Ӧ������ֵ
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

	//���´ӷ������ֻ�õ�������
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
    //���¹���������
	virtual int UpdateProcData(float *pfData_,int iLen_,const CBHDateTime& tDateTime_,int iStartIndex_=0);

	//�������߲��״̬
	virtual int UpdateVibChannStatus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex);
	virtual int UpdateProcChannStstus(S_CHANN_STATUS * apChannStatus,const int &anLen,const int &anStartIndex);
    void UpdateRealRev(const float *pfRev_);
	virtual int UpdateProcSUDStatus(int anProcJudgedPlantStatus,int anLen_,int anStartIndex_=0);
	virtual int GetProcSUDStatus();
	virtual bool IsAllChannUpdate();//
	virtual bool IsHasChannNotUpdate();//
	virtual bool IsPlantUpdateOverTime();

	virtual int CalcVibDataGrade(S_VibCharValue *pVibCharValue_,int iLen_,int iStartIndex_);

	/**@brief ���ʵʱת��
	  *@param pfRev_��ȡת��BUFFָ�룬
	  *@param iLen_ Buff����,����ȡ����ʵʱת��
	  *@return ʵ�ʻ�ȡת�ٸ���
	  */
    int GetRealRev(float *pfRev_, int iLen_);
	int GetLastRev(float *pfRev_, int iLen_);
	int GetRealRevFromChannel(float *pfRev_, int iLen_);
	CBHDateTime GetLastRevTime();

	///����ת�����ͣ��õ���Ӧ��ʵʱת�١����bSafe_Ϊtrue����ʹ�ö�д�������false����ʹ�ö�д��
	float GetSpecTypeRealRev(const int &iRevType_=1,const bool &bSafe_=true );

    ///�õ���ǰ�����ݵĸ���ʱ��
	CBHDateTime GetRealVibDataDateTime();
	///�õ���ǰ��̬ͨ�����ݵĸ���ʱ��
	CBHDateTime GetRealDynDataDateTime();
    ///�õ���ǰ���������ݵĸ���ʱ��
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


	///�õ�ָ�����͵����в����Ϣ
	int GetSpecTypeChannInfo(S_ChannInfo * pSchannInfo_,int iType_=GE_ALLPROC);
	///�õ�ָ�����͵����в����Ϣ
	int GetSpecTypeChannInfo(S_ChannInfo &sChannInfo_,int iIndex_=0,int iType_=GE_ALLPROC);

	///�õ�ָ���񶯲�����ڵ���ʼ�����ͽ�������
	virtual int GetSpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///�õ���ȼ��ָ���񶯲�����ڵ���ʼ�����ͽ�������
	virtual int GetICESpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///�õ�ָ����̬������ڵ���ʼ�����ͽ�������
	virtual int GetSpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	
	///�õ���ȼ��ָ���񶯲�����ڵ���ʼ�����ͽ�������
	virtual int GetICESpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///�õ�ָ���񶯲�����ڵ���ʼ�����ͽ�������
	virtual int GetSpecVibChannStartEndIndex(int index_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecVibChannStartEndIndex(string sChann_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	///�õ���ȼ��ָ���񶯲�����ڵ���ʼ�����ͽ�������
	virtual int GetICESpecVibChannStartEndIndex(int index_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecVibChannStartEndIndex(string sChann_,const S_VibCharValue& VibCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///�õ�ָ����̬������ڵ���ʼ�����ͽ�������
	virtual int GetSpecDynChannStartEndIndex(int index_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetSpecDynChannStartEndIndex(string sChann_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///�õ���ȼ��ָ����̬������ڵ���ʼ�����ͽ�������
	virtual int GetICESpecDynChannStartEndIndex(int index_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);
	virtual int GetICESpecDynChannStartEndIndex(string sChann_,const S_DynCharValue& DynCharValue_,int &iStartIndex_,int &iEndIndex_,bool abRealTime = false);

	///��������ģ��������Զ����������
    virtual int CalcCustomParamData(int iStartIndex_,int iLen_,CHANNTYPE channType = GE_VIBCHANN);

	///[TBD] ���������ģ����Զ����������:GE_MACHINETYPE_WINDPEQ|GE_MACHINETYPE_KEYPUMP|GE_MACHINETYPE_PUMP	
	virtual int CalcCustomParamDataWindPump(int iStartIndex_,int iLen_,CHANNTYPE channType  = GE_VIBCHANN);

	///[TBD] ���������ģ����Զ����������:GE_MACHINETYPE_WINDPEQ|GE_MACHINETYPE_KEYPUMP|GE_MACHINETYPE_PUMP	
	
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


	///���θ���ʵʱ���ݵ����ⲿʹ�õ����ݣ��ͻ��ˣ�
	virtual void SyncVibData();
	virtual void SyncDynData();
	
	float MaxRealRev() const;
	void MaxRealRev(float val);


	///������ͣ��״̬ʹ��

	/**������ͣ��״̬�����ж��Ƿ���Ҫ����
	*@param IDBInterFace ���ݿ�ӿ�
	*@param bFlag_    �Ƿ��ڿ�ͣ��״̬
	*@param bSave_    �Ƿ���Ҫ����
	*@param iEventID_ ��ͣ���¼�id_
	*@param iSudType_ ��ͣ��״̬��
	*@return  
	*/
	virtual void SetDataStatuEx(    IDBInterFace * pDB_,
									bool bFlag_,
									long iEventID_,
									E_SUD_TYPE iSudType_);

	/**���õ�ǰ���ݵ�ʱ�䣬��������
	*@param dataTime_        ����
	*@param iMicroSecond_    ����
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
	///��ͣ����ز���������ʱ����ʹ��
	///��ǰ��ͣ���¼���ID
	long _iCurrEventID;
	long _iEventID;
	int _iMicroSecond;
	CBHDateTime _Timesud;
protected:
	
	//�ڸ�����Ƶ���У����ݸ�����һ��Ƶ�ʣ��ҵ�����Ƶ�������У�ÿһ����ӽ�����Ƶ�ʵķ�ֵ
	int _FindSpectrumData(
		std::map<int,float> & arefFeatureData,
		float * apCalcResultArray,
		int anResultArrayLength,
		double * arefPSpectrumValueArray,
		int anSpectrumArrayLength,
		double afDf									
		);
	
	//�ڸ�����Ƶ���У����ݸ�����һ��Ƶ�ʣ��ҵ���ӽ�����Ƶ�ʵķ�ֵ
	double _FindSpectrumData(
		double afFreq,
		double * arefPSpectrumValueArray,
		int anSpectrumArrayLength,
		double afDf									
		);

	//�ڸ�����Ƶ���У����ݸ�����һ��Ƶ�׷�ֵ���飬�ҵ�����ֵ���Լ���Ӧ������
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
	//������ù�����
	int _CalcSpeedSpectrumPowerWindPump(
		const S_VibCharValue & aoVibCharVal,
		CVibChannParam * apVib, 
		float * apCalcResultArray,
		int anResultArrayLength,
		double * apWave,
		float afWorkFreq,
		double adblDf
		);

	//������÷�ֵ��
	int _CalcCustomSpectrumAmpliferWindPump(
		const S_VibCharValue & aoVibCharVal,
		CVibChannParam * apVib, 
		float * apCalcResultArray,
		int anResultArrayLength,
		double * apWave,
		float afWorkFreq,
		double adblDf
		);

	//������ð�����
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
	///���㵥��������Զ������������
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
									  BufferForCustomCalc & aRefBufferObj, //����ʹ�õĻ������
									  double adf
									 );

	//����ʱ�����ݼ����Զ�����ز���
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

	
	/**����ʱ�����ݼ����Զ�����ز���
	/*@param pfWave_					[in]	ʱ��������                                                  
	/*@param ilen_						[in]	�������ݳ���                                              
	/*@param iSmpFreq_					[in]	����Ƶ��                                                 
	/*@param iStartIndex_				[in]	��������ڵĿ�ʼ����                                    
	/*@param iEndIndex_					[in]	��������ڵĽ�������                                    
	/*@param iUnitType_					[in]	�Զ������ģ��Ŀ�ʼֵ������ֵ�ĵ�λ����                  
	/*@param iSigAnaType_				[in]	�Զ������ģ����źŷ�������                              
	/*@param iSegNum_					[in]	�Զ������ģ��Ķ����Զ����������,�Լ����ڲ������ݺ���ķֶθ���                        
	/*@param iSegStartNo_				[in]	�Զ������ģ����Զ�������ο�ʼ����   
	/*@param iCharTye_					[in]	�Զ������ģ����Զ����������ֵ����  
	/*@param fStartValue_				[in]	�Զ������ģ����Զ��������ʼֵ��ʼ-������  
	/*@param fEndValue_					[in]	�Զ������ģ����Զ��������ʼֵ����-������  
	/*@param fCustomParamValue_		[in/out]	�������Զ������ģ����Զ������ֵ  
	/*@param aRefBufferedSigMath		[in]	���л�����㷨���װ��  
	/*@param aRefDoubleWaveBuffer		[in]	����short��floatת�������黺��  
	*/
	void CalcWaveCustomParamValue_New(	const DATATYPE_WAVE *pfWave_,									//[in]	��������   
										int& ilen_,														//[in]	�������ݳ���        
										int iSmpFreq_,													//[in]	����Ƶ��  
										int iStartIndex_,												//[in]	��������ڵĿ�ʼ����
										int iEndIndex_,													//[in]	��������ڵĽ������� 
										int iUnitType_,													//[in]	�Զ������ģ��Ŀ�ʼֵ������ֵ�ĵ�λ����        
										int iSigAnaType_,												//[in]	�Զ������ģ����źŷ�������       
										int iSegNum_,													//[in]	�Զ������ģ��Ķ����Զ����������	
										int iSegStartNo_,												//[in]	�Զ������ģ����Զ�������ο�ʼ����	
										int iCharTye_,													//[in]	�Զ������ģ����Զ����������ֵ����	
										float fStartValue_,												//[in]	�Զ������ģ����Զ��������ʼֵ��ʼ-������	
										float fEndValue_,												//[in]	�Զ������ģ����Զ��������ʼֵ����-������	
										float *fCustomParamValue_,										//[in/out]	�������Զ������ģ����Զ������ֵ	
										BufferedBaseChannSigMathBasedOnSpecChann & aRefBufferedSigMath,	//[in]	���л�����㷨���װ��	
										std::vector<double> & aRefDoubleWaveBuffer				//[in]	����short��floatת�������黺��		
										);
	//����Ƶ�����ݼ����Զ�����ز���
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
	
	/**����Ƶ�����ݼ����Զ�����ز���
	/*@param pfWave_					[in]	Ƶ��Ƶ������                                                  
	/*@param ilen_						[in]	�������ݳ���                                              
	/*@param iRev_						[in]	ת��                                                 
	/*@param iUnitType_					[in]	�Զ������ģ��Ŀ�ʼֵ������ֵ�ĵ�λ����                                     
	/*@param iSigAnaType_				[in]	�Զ������ģ����źŷ�������                                    
	/*@param iSegNum_					[in]	�Զ������ģ��Ķ����Զ�������������Լ�����Ƶ�׺���ķֶθ���                        
	/*@param iSegStartNo_				[in]	�Զ������ģ����Զ�������ο�ʼ����   
	/*@param iCharTye_					[in]	�Զ������ģ����Զ����������ֵ����  
	/*@param fStartValue_				[in]	�Զ������ģ����Զ��������ʼֵ��ʼ-������  
	/*@param fEndValue_					[in]	�Զ������ģ����Զ��������ʼֵ����-������  
	/*@param defectFreq_				[in]	�Զ��������Ӧ�����������ȱ��Ƶ��  
	/*@param gearboxCharFreq_			[in]	�Զ��������Ӧ�������ڳ�����ȱ��Ƶ��  
	/*@param fCustomParamValue_		[in/out]	�������Զ������ģ����Զ������ֵ  
	/*@param adbldf						[in]	Ƶ�׺����Ƶ�ʼ��  
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



	//���ݹ�����в�����Ϣ����ȱ������Ƶ�ʽṹ����Ϣ,���ع��������Ŀ
	int CalcBearingDefectFreq(CBearingParamInfo &bearingParamInfo_,int iWorkRev_,S_BearingDefectFreq& defectFreq_);
	//���ݳ����������Ϣ��������Ƶ�ʽṹ����Ϣ,���س��ּ���
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
	//������Щbool����Ϊ���ж��ϴα��������Ƿ��Ѿ����¹���
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
    ///�ж��Ƿ������ڱ�������״̬
	bool _bSaveDataStatus;
	IReadWriteLock *_pLockSaveDataStatus;

private:
	int m_nProcJudgedRunStatus; //�����������жϵ��豸״̬
	int m_nVibJudgedRunStatus; //�񶯲����жϵ��豸״̬
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

	/**�õ��豸��ָ������Ӧ��ת��
      *@param strChann_ ָ���Ĳ���
	  *@param fRealRev_ �õ�ʵʱת��
	  *@return �ת��
	  */
	float GetSpecChannRev(string strChann_,float &fRealRev_);

	///�õ�ָ�����������Ĺ���������
	int GetSpecChannProcData(string sChann_,float &fData_,string &sUnit_,int iChannType_=GE_ALLPROC);

	///�õ�ָ����������������Ĺ���������
	int GetSpecChannProcData(int index_,float &fData_,string &sUnit_,int iChannType_=GE_ALLPROC);

	///�õ�ָ������������ֵ����
	int GetSpecChannAxisData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_DCCHAR);
    int GetSpecChannAxisData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_DCCHAR);

	///�õ�ָ������������
	int GetSpecChannRadialData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(string sChann_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	int GetSpecChannRadialData(int index_,float &fAmp_,float &fPhase_,float &fRev_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	///�õ�ָ����������������ֵ����
	int GetSpecChannRadialAllChar(string sChann_,S_VibCharValue &sVibData_,string &sUnit_);
	int GetSpecChannRadialAllChar(int index_,S_VibCharValue &sVibData_,string &sUnit_);
	///�õ�ָ���񶯲�������ֵ���͵���������
	int GetSpecChannAndCharTypeVibData(string sChann_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);

	///�õ�ָ��������ͨ�����Զ�������ֵ
	int GetSpecRadialVibChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecRadialVibChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	///�õ�ָ���񶯲������������ֵ���͵���������
	int GetSpecChannAndCharTypeVibData(int index_,float &fData_,string &sUnit_,int iCharType_=GE_ALLCHAR);
	///�õ�ָ���񶯲��Ĳ��κ�Ƶ������
	int GetSpecChannVibWaveAndFreq(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_,float * pfFreq_,BOOL abSkipFreq=FALSE);
	///�õ�ָ���񶯲��Ĳ��κ͵�������
	int GetSpecChannVibWaveAndCepstrum(string sChann_,DATATYPE_WAVE *pfData_,int &iPtNum_,double *pfFreqData_,int &iCepsNums_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_);
	///�õ�ָ���񶯲��Ƶ������
	int GetSpecChannVibFreq(string sChann_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float *pfFreq_,float& fScaleCoef_,int& PrecisionEnabled_);

	///�õ�ָ���񶯲��Ĳ���
	int GetSpecChannVibWave(string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///�õ�ָ���񶯲��Ĳ���
	int GetSpecChannVibWave(string sChann_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///�õ�ָ����̬���Ĳ���,����-1��˵��û��������
	int GetSpecDynChannWave(const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,float *pfWave_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);
	///�õ�ָ����̬���Ĳ���,����-1��˵��û��������
	int GetSpecDynChannWave(const string &sChannID_,DATATYPE_WAVE *pfData_,int& ilen_,DATATYPE_WAVE *pfRawData_,float & fScaleCoef_,int &nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);

	///�õ�ָ����̬ͨ�����Զ�������ֵ
	int GetSpecDynChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecDynChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	///�õ�ָ����̬���Ĳ��κ�Ƶ�����ݡ�����-1��˵��û��������
	int GetSpecDynChannWaveAndFreq(const string &sChannID_,DATATYPE_WAVE *pfData_,int &iPtNum_,DATATYPE_WAVE *pfFreqData_,int &iLines_,float &fDf_,float& fScaleCoef_,int& nPrecisionEnabled_,float *pfWave_,float *pfFreq_);
    ///�õ�ָ����̬��������ֵ������-1��˵��û��������
	int GetSpecDynChannCharValue(const string &sChannID_,S_DynCharValue &sDynCharValue_);

	//�õ�ָ�������Ĳ����
	string GetSpecIndexRadialVibChannID(int index_);
    string GetSpecIndexAxisVibChannID(int index_);
	string GetSpecIndexProcChannID(int index_);

	//�õ�ָ������λ������������λ��
    bool GetSpecChannAxisLocationData(string sLocation_,float &fX_,float &fY_);
	///�õ�ָ��ˮƽ����ʹ�ֱ�����������λ������
	bool GetSpecChannAxisLocationData(string sChannHor_,string sChannVer_,float &fX_,float &fY_);

	//�õ�ָ����̬���Ļ�����λ������������λ������
	bool GetSpecDynIndexAxisLocationData(int index_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynIndexAxisLocationData(int index_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sLocation_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sLocation_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sChannHor_,string sChannVer_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool GetSpecDynChannAxisLocationData(string sChannHor_,string sChannVer_,int iWaveNum_,float *pHorWave_,float *pVerWave_,float *pHorAxesWave_,float *pVerAxesWave_);

	/**�õ�ָ���񶯲��ϸ����
	  *param��
	  *       ag_strChann��string,����
	  *       ag_pfZoomSpec:float*,ϸ��������
	  *       ag_iLines:int&,����������������Ϻ���޸�ʵ�ʷ�����������������ϸ���׵����ݳ���
	  *       ag_fStartFreq:float&, ϸ����������ʼƵ�ʣ�������Ϻ��ʾϸ�������ʼƵ��
	  *       ag_iStopFreq:int,ϸ����������ֹƵ��
	  *       ag_fDf��float&,ϸ���׵ķֱ���
	  *       ag_pfWave:��������
	  *       ag_iPtNum:�������ݳ���
	  *return:0��ʾ������ȷ,������ʾ������
	  */
	int GetSpecChannZoomSpec(string sChann_,DATATYPE_WAVE *pfZoomSpec_,long &iLines_,float &fStartFreq_,int iStopFreq_,float &fDf_,DATATYPE_WAVE *pfWave_,int &iPtNum_,float &fScaleCoef,int& nPrecisionEnabled_,float *pfWaveData_,float *pfFreqData_);

	// ��ʼ���������Ƶĵ�ǰ��¼IDֵ
	void TrendInitID(CCustomDB * pDB_);
	void TrendInitID(IDBInterFace * pDB_);
	int SaveTrendData(IDBInterFace * pDb);

	int GetProcData(float *pfData_,int iNum_,int iStartIndex_=0);
	//��Ӧ4ϵ��ͬ������
	int GetAllVibData(DATATYPE_WAVE *pWave_,S_VibCharValue * pVibCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_=0);
	int GetAllDynData(DATATYPE_WAVE *pWave_,S_DynCharValue * pDynCharValue_,int &iAllWaveNum_,vector<int> lSmpNums_,int iLen_,DATATYPE_WAVE *pRawWave_,float& fScaleCoef_,int& nPrecisionEnabled_,vector<float>& fMaxValue,vector<float>& fMinValue_,int iStartIndex_=0);


	float GetVibDataWaveCoef(int iLen_,int iStartIndex_=0);
	float GetDynDataWaveCoef(int iLen_,int iStartIndex_=0);

	//�õ�ָ�����ŵ���ͣ����Ϣ��ת��
	int GetSpecChannIndex(string sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);
	//�õ�ָ�����ŵ���ͣ����Ϣ��ת��
	int GetSpecChannIndex(int index_,string &sChannID_,float &fRev_,float &fPhase_,float &fAmp_,int &iSmpNum_,int &iSmpFreq_,int iCharType_=GE_ALLCHAR);

		/**�õ���ͣ����״̬����ͣ�����ͺͿ�ͣ���¼�ID
	  *@param iType_ ��ͣ������,�����ñ���
	  *@param iEventID_ ��ǰ��ͣ�����¼�ID,�����ñ���
	  *@return ��ͣ��״̬
	  */
	bool GetStatusType(E_SUD_TYPE & iType_,int & iEventID_);

	//���߲���״̬

	bool GetSpecWireLessChannStatus(int anIndex,string &asChannID, int &anChannOnline, int &anChannNormal, int &anDataUpdate,int &anBatteryPower, int anChannType);

private:
	//���涼���뱣����ʷ�����й�
	///������ʷ������Ҫ�ĸ��������ͱ���
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



	// �жϱ����¼�Ƿ�����ʱ���ظ�
	bool IsDateTimeDifferent( CBHDateTime Last_, CBHDateTime Curr_);
	// �����ڵ�ǰ���Ʊ��е��������ݣ�ֻ����һ��Сʱ֮��á�һ���ӱ���һ�Σ����ⲿͨ����ʱ�����ƣ���
	int SaveSecondTrend(IDBInterFace * pDb_);
	// �����Ʊ���
	int SaveDayTrend(IDBInterFace * pDb_);
	// Сʱ���Ʊ���
	int SaveHourTrend(IDBInterFace * pDb_);
	// �����Ʊ���
	int SaveMonthTrend(IDBInterFace * pDb_);
	
	// �����Ƽ�¼����
	void SaveVibRecord(IDBInterFace * pDB_,
						string sTableName_,
						int iCurID_, 
						const CBHDateTime &tDatetime_, 
						bool bMonth_=false);

	// ��̬������Ƽ�¼����
	void SaveDynRecord( IDBInterFace * pDB_,
						const string &sTableName_,
						const int &iCurID_,
						const CBHDateTime &tDatetime_,
						const bool &bMonth_=false);

	// ���������Ƽ�¼����
	void SaveProcRecord(IDBInterFace * pDB_,
						string sTableName_,
						int iCurID_, 
						const CBHDateTime &tDatetime_, 
						bool bMonth_=false);

	/**�õ�ָ������λ������������λ������
      *@param index_ ����λ�õ����������������ָ���豸������λ�ñ��˳��
	  *@param fX_    X��ˮƽ���򣩵�����
      *@param fY_    Y����ֱ���򣩵�����
	  *@return �����ȷ�õ����ͷ���true�������false
	  */
    bool GetSpecIndexAxisLocationData(int index_,float &fX_,float &fY_);

// ������ݱ���
public:
	// �����Ƽ�¼����
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
	// ��̬���Ƽ�¼����
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

	// ���������Ƽ�¼����
	int SaveProcRecord(IDBInterFace *pDB_,
		string sTableName_,
		int    iCurID_,
		float *pfData_,
		int iLen_,
		const CBHDateTime& aoDateTime_,
		int iEventID_,
		int iStartIndex_);
	//�����񶯿����������
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

	//���涯̬�����������
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
	
	//������̿����������
	int SaveProcFastTrend(
		IDBInterFace *pDB_,
		float *pfData_,
		int iLen_,
		const CBHDateTime& aoDateTime_,
		int iEventID_,
		int iAlarmStatus_,
		int iStartIndex_=0);


	//�豸�ж������Դʱ���񶯷ǿ�����ݴ���ͨ�������ݿ�������
	int InsertVibFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);
	//�豸�ж������Դʱ���񶯷ǿ�����ݴ���ͨ�������ݿ�������
	int UpdateVibFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);

	//�豸�ж������Դʱ����̬�ǿ�����ݴ���ͨ�������ݿ�������
	int InsertDynFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);
	//�豸�ж������Դʱ����̬�ǿ�����ݴ���ͨ�������ݿ�������
	int UpdateDynFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_,bool bSaveWave_);

	//�豸�ж������Դʱ�����̷ǿ�����ݴ���ͨ�������ݿ�������
	int InsertProcFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_);
	//�豸�ж������Դʱ�����̷ǿ�����ݴ���ͨ�������ݿ�������
	int UpdateProcFastDataFromTrendData(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,int iLen_);


	//�豸�ж������Դʱ���񶯷ǿ�����ݴ���ͨ���Ʊ�������
	int InsertVibFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);
	//�豸�ж������Դʱ���񶯷ǿ�����ݴ���ͨ���Ʊ�������
	int UpdateVibFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);

	//�豸�ж������Դʱ����̬�ǿ�����ݴ���ͨ���Ʊ�������
	int InsertDynFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);
	//�豸�ж������Դʱ����̬�ǿ�����ݴ���ͨ���Ʊ�������
	int UpdateDynFastDataromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_,bool bSaveWave_);

	//�豸�ж������Դʱ���������ǿ�����ݴ���ͨ���Ʊ�������
	int InsertProcFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_);
	//�豸�ж������Դʱ���������ǿ�����ݴ���ͨ���Ʊ�������
	int UpdateProcFastDataFromTrendTable(IDBInterFace *pDB_,string sTable_,int iCurID_,CBHDateTime dtTime_,int iStartIndex_);


	bool IsRecordExist(IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_);
	int GetRecordDataID(IDBInterFace *pDB_, string sTable_,CBHDateTime tDateTime_);


	//����������ʱ����ڵļ�¼ID��Ϊ��ֵ
	int SaveSpecTimeData(IDBInterFace * pDB_,string sTable_,CBHDateTime tDateTime_,int nCurrID_);

	//��������ʱ��λ��沢д���ļ�
	int SaveSpecTime(IDBInterFace * pDB_,const std::pair<CBHDateTime,CBHDateTime>& apDatetime,const CString& sTblName_,int nStatus_);

	//ɾ������������ݱ���������ʱ���
	int RemoveSpecTime(IDBInterFace * pDB_,const string& strTblName_);

	//��ʼ��������������ID
	int InitTableNegCurID(IDBInterFace *pDB_,string sTable_);

	//�����Ʊ�ID��0����ʼ��������
	int ResetTrendTableCurID(IDBInterFace *pDB_,string sTable_,int nCurrID_);

	///�Ƿ�洢�񶯲���
	bool ShouldSaveVibWaveData(IDBInterFace *pDB_,string sTable_,CVibChannParam* pVibChannParam_,const S_VibCharValue& aCharValue_);
	///�Ƿ�洢��̬����
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
	///�񶯲��Ĳ����ֶ�
	vector<CString>  _vVibWaveField;
	///��̬���Ĳ����ֶ�
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