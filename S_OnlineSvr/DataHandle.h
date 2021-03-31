#pragma once
#include "stdafx.h"

/**һЩע�����
  *�����ݴ���ʱ������ͱ������ʱ����λ�����ݺ͹��������ݶ�ͬ�ȶԴ������������ݸ���ʱ���Ƿֱ�Դ�
  */
class CDataHandle
{
public:
	CDataHandle(void);
	~CDataHandle(void);
private:
	struct S_VibInfo
	{
		string   strChann;
		string   strAll;
		string   strOne;
		string   strTwo;
		string   strThree;
		string   strHalf;
		string   strRes;//����ֵ
		string   strWave;
		string   strPhaseOne;
		string   strPhaseTwo;
		string   strPhaseThree;
		string   strPhaseHalf;
		string   strDC;
		float  fLowAlarmValue;
		float  fHighAlarmValue;
		float  fBeforeValue;
		int    iContinueNum;
		int    iIntervalNum;
		bool   bHighAlarm;
		bool   bCheckAlarm;//�Ƿ��жϱ���
		COleDateTime  timeCheck;
		int   iOfRev;   //�����Ǹ�ת��
	};
	/*!@brief �����������Ϣ�ṹ��
	  */
	struct S_ProcInfo
	{
		string strChannel;
		int    iType;
		float  fLowAlarmValue;
		float  fHighAlarmValue;
		float  fBeforeValue;
		int    iContinueNum;
		int    iIntervalNum;
		bool   bHighAlarm;
		bool   bCheckAlarm;//�Ƿ��жϱ���
		COleDateTime  timeCheck;
	};
	
	///װ�ú�
	string    _strSet;
	///�豸��
	string    _strPlant;
	string    _strVibAlarmTable;
	string    _strProcAlarmTable;
	int       _iVibChannNum;
	int       _iProcChannNum;
	int       _iAxisDisplaceNum;
	int       _iSmpNum;

	///���Ĳ����������������ݵĳ��Ⱦ��ǰ���������ݷ����
	int       _iSmpMaxNum;
	
	int       _iSmpFreq;
	float     *_pfVibCharAll;
	float     *_pfVibCharOne;
	float     *_pfVibCharTwo;
	float     *_pfVibCharThree;
	float     *_pfVibCharHalf;
    float     *_pfVibCharRes;
	float     *_pfPhaseOne;
	float     *_pfPhaseTwo;
	float     *_pfPhaseThree;
	float     *_pfPhaseHalf;
	float     *_pfDC;
	float     **_ppfWave;
	float     *_pfProcValue;
	float     *_pfAxisDisplaceValue;
	COleDateTime     _timeCurrent;
	CString          _strTimeCurr;
	CSimADO   *_pDB;
	float     _iRev1;
	float     _iRev2;

	/*!���豸���й��������������״̬������֪ͨ�м�����豸���й������������״̬��  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbProcChannelState;

	/*!���豸������λ�Ʋ��������״̬������֪ͨ�м�����豸������λ�Ʋ������״̬��  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbAxisChannelState;

	/*!���豸�����񶯲��������״̬������֪ͨ�м�����豸�����񶯲������״̬��
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbVibChannelState;    
    
	///���������ݶ�д��
	CMutexRW *_lockDataProc;

	///�񶯲�����ݶ�д��
	CMutexRW *_lockDataVib;

	///�������״̬������
    CMutexRW * _lockDataRunState;

	///���ڼ�鱨��ʱ���ݸ���״̬�����Ķ�д��
    CMutexRW * _lockDataUpdatedAlarm;

	///������ʷ����ʱ���ݸ���״̬�����Ķ�д��
	CMutexRW * _lockDataUpdatedHis;

protected:
	typedef vector<S_VibInfo> VECTOR_S_VIBINFO;
	typedef vector<S_ProcInfo> VECTOR_S_PROCINFO;

	/*!@brief �����񶯲��������˳��������ݿ���ͨ�����е�˳����ͬ
	*/
	VECTOR_S_VIBINFO _vectorVibChann;

	/*!@brief ���й�����������λ�Ʋ��������˳��������ݿ���ͨ�����е�˳����ͬ
	*/
	VECTOR_S_PROCINFO _vectorProcChann;

	VECTOR_S_VIBINFO::const_iterator _cvectorEndVib;
	VECTOR_S_PROCINFO::const_iterator  _cvectorEndProc;
 
public:
	/**@brief ����װ�úź��豸��*/
	void SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iMaxSmpNum);
	

	/**@brief ��Ӳ����Ϣ*/
	void AddVibChannelInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm,int ag_iofRev);
	
	/**@brief ��ӹ����������Ϣ*/
	void AddProcAndAxisChannInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm, int ag_iChannType);
	
	
	/// ����񶯲����Ϣ����������������й������ݿ������ʹ�õ��ֶ�����
	void AddVibChannInfoEnd(void);
	
	/// ��ӹ����������Ϣ����������������й������ݿ������ʹ�õ��ֶ�����
	void AddProcChannInfoEnd(void);

	/**@brief ��ʼ����������ÿ������Ӧ�ı���ֵ,������.ע�⣺�˺���һ��Ҫ��������������в����Ϣ��ϡ�
      *���ڴ˺������к󣬾Ͳ���������Ӳ������Ϣ�ˣ�������������AddVibChannelInfo��AddProcChannInfo�������������
	  */
	void InitParam(float *ag_pfCharAll,
		float *ag_pfCharOne,
		float *ag_pfCharTwo,
		float *ag_pfCharThree,
		float *ag_pfCharHalf,
		float *pfCharRes_,
		float *pfPhaseOne_,
		float *pfPhaseTwo_,
		float *pfPhaseThree_,
		float *pfPhaseHalf_,
		float *ag_pfDC,
		float **ag_ppfWave,
		float *ag_pfProc,
		float *ag_pfAxisDisplace,
		int  *ag_pbVibChannelState,
		int  *ag_pbProcChannelState,
		int  *ag_pbAxisDisplaceChannelState,
		CSimADO *ag_pDB);
	
	/// �õ��豸��
	string GetPlantID(void);
	
	/// �����豸ת��
	void SetPlantRev(float ag_iRev1,float ag_iRev2);
   
	///�����񶯲������
    void UpdateVibData(float * ag_pfCharOne,
		            float * ag_pfCharTwo,
					float * ag_pfCharThree,
					float * ag_pfCharAll,
					float * ag_pfCharHalf,
					float * ag_pfCharRes,
					float * ag_pfDC,
					float * pfPhaseOne_,
					float * pfPhaseTwo_,
					float * pfPhaseThree_,
					float * pfPhaseHalf_,
					float * ag_fwave,
					int ag_iSmpNum,
					int ag_iSmpFreq,
					int ag_iRealNum,
					int ag_len,
					int ag_startIndex=0);
    
	///���¹���������,��������λ������
	void UpdateProcData(float *ag_pfData,int ag_len,int ag_startIndex=0);
   
	///��λ����λ������
	void UpdateAxisDisplaceData(float *ag_pfData,int ag_len,int ag_startIndex=0);
    
	/*!@brief �õ����豸����״̬��д��ָ��
	*/
	CMutexRW * GetRunStateLock();

  	/*!@brief �õ����豸�������������״ָ̬��
	*/
	int * GetProcChannelStatePointer(void);

	/*!@brief �õ����豸��λ�Ʋ������״ָ̬��
	*/
	int * GetAxisDisplaceChannelStatePointer(void);
	
	/*!@brief �õ����豸�񶯲������״ָ̬��
	  */
	int * GetVibChannelStatePointer(void);

	/*!@brief �õ����豸�����񶯲�����
	*/
	int GetVibChannelNum(void);

	/*!@brief �õ����豸���й�����������
	*/
	int GetProcChannelNum(void);
	
	/// �õ�����豸��λ��ͨ���ĸ���
	int GetAxisDisplaceChannelNum(void);
    
	/// �õ�����豸�񶯲�������������
	int GetSmpMaxNum(void);
public:
///������Ҫ�ĸ��������ͱ���   
	/**@brief �ж���û�б���
	  */
	void JudgeAlarm(CCustomDB * ag_pDB);
private:
///������Ҫ�ĸ��������ͱ���
	/*!�񶯲�㱨����¼������Ŀ�������񶯲����Ŀ�������ģ��񶯲���������1000.*/
	int _iAlarmVibRecordAllNum;

	/*!��������㱨����¼������Ŀ�����ݹ����������Ŀ�������ģ�����������������1000.*/
	int _iAlarmProcRecordAllNum;

	/*!�񶯱��������У��´μ�¼Ӧ�ñ����IDֵ*/
	int _iCurIDAlarmVib;

	/*!���������������У��´μ�¼Ӧ�ñ����IDֵ*/
	int _iCurIDAlarmProc;
	
	/// ��ʼ�������������¼�������õ���ǰ�������е����¼�¼��IDֵ��
	void AlarmInitIDAndRecordAllNum(void);
	
	/**@brief �����ж����й���������Ƿ񱨾������汨�����
	  */
	void JudgeProcAlarm(CCustomDB * ag_pDB);

	/**@brief �����ж������񶯲���Ƿ񱨾������汨�����*/
	void JudgeVibratedAlarm(CCustomDB * ag_pDB);

	/**@brief �����񶯱�����¼����*/
	void SaveVibratedAlarmRecord(int ag_index,CCustomDB * ag_pDB);

	/**@brief ���������������¼*/
	void SaveProcAlarmRecord(int ag_index,int ag_index2,CCustomDB * ag_pDB);

	/*!@brief �ж��Ƿ�ﵽ�������񶯱�����¼������������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�񶯲������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeVibAlarmPri(int ag_index);

	/*!@brief �ж��Ƿ�ﵽ�߼��Ĺ�����������¼������������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�������������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeProcAlarmSup(int ag_index);

	/*!@brief �ж��Ƿ�ﵽ�����Ĺ�����������¼������������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�������������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeProcAlarmPri(int ag_index);

	/*!@brief �ж��Ƿ�ﵽ�߼������񶯱�����¼������������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�񶯲������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeVibAlarmSup(int ag_index);

	/*!@brief �жϹ��������ޱ�����һЩ����������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�������������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeProcAlarmLow(int ag_index);

	/*!@brief �ж��񶯲����ޱ�����һЩ����������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�񶯲������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeVibAlarmHigh(int ag_index);

	/*!@brief �жϹ��������ޱ�����һЩ����������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�������������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeProcAlarmHigh(int ag_index);

	/*!@brief �ж��񶯲����ޱ�����һЩ����������ﵽ�ͱ��汨����¼
	  !@param  ag_index:�񶯲������
	  !@return ���Ϊtrue���򱣴汨����¼��false�򲻱���
	  */
	bool JudgeVibAlarmLow(int ag_index);
public:
///������ʷ������Ҫ�ĸ��������ͱ���
	// �����������ݣ��ⲿ����
	void SaveTrendData(void);
private:
///������ʷ������Ҫ�ĸ��������ͱ���
	string        _strVibSecondTrendTable;
	string        _strVibDayTrendTable;
	string        _strVibHourTrendTable;
	string        _strVibMonthTrendTable;
	string        _strProcDayTrendTable;
	string        _strProcHourTrendTable;
	string        _strProcMonthTrendTable;
	string        _strProcSecondTrendTable;
	string        _strProcField;
	string        _strVibFieldSel;
	string        _strVibFieldIns;
	
	/*!�����Ʊ���ǰ���������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ����������ܹ���3600����¼����Сʱ����Ϊ��������ⲿͨ����ʱ������Ϊһ����ִ��һ��*/
	int           _iCurrIDTrendSecond;

	/*!�����Ʊ�Сʱ���Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDTrendHour;

	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDTrendDay;

	/*!�����Ʊ������Ʊ�������У��´μ�¼Ӧ�ñ���ļ�¼IDֵ*/
	int           _iCurrIDTrendMonth;

	/*!�����ж��Ƿ�ñ��������Ƽ�¼���������Ŀ��*/
	int _iTrendDayInteral;

	/*!�����ж��Ƿ�ñ��������Ƽ�¼���������Ŀ��*/
	int _iTrendMonthInteral;

	/*!�����ж��Ƿ�ñ���Сʱ���Ƽ�¼���������Ŀ��*/
	int _iTrendHourInteral;

    /**���ж��Ƿ���Ҫ��������ʱʹ�á���Ϊ�������û�и��¾Ͳ������ˡ������ݸ��º���������Ϊtrue�����溯��������Ϊfalse
	  *�ж������Ƿ���¹�,true��ʾ���ݸ����ˣ�false��ʾ���ݸ���
	  */
	bool _bDataUpdatedForHis;
	
	/**���ж��Ƿ���Ҫ�����������ݱ���ʱʹ�á���Ϊ�������û�и��¾Ͳ���鱨���ˡ��ڹ��������ݸ��º���������Ϊtrue��������麯��������Ϊfalse
	  *�жϹ����������Ƿ���¹���������λ������,true��ʾ���ݸ����ˣ�false��ʾ���ݸ���
	  */
	bool _bProcDataUpdatedForAlarm;

	/**���ж��Ƿ���Ҫ��������ݱ���ʱʹ�á���Ϊ�������û�и��¾Ͳ���鱨���ˡ��������ݸ��º���������Ϊtrue��������麯��������Ϊfalse
	  *�ж��������Ƿ���¹���true��ʾ���ݸ����ˣ�false��ʾ���ݸ���
	  */
	bool _bVibDataUpdatedForAlarm;

	// ��ʼ���������Ƶĵ�ǰ��¼IDֵ�������ơ������Ƶļ��ֵ
	void TrendInitIDAndInterval(void);

	// �ж��Ƿ�ñ���Сʱ����
	bool JudgeHourTrendSave(void);
	// �ж��Ƿ񵽱��������Ƽ�¼��ʱ��
	bool JudgeMonthTrendSave(void);
	// �ж��Ƿ񵽱��������Ƶ�ʱ��
	bool JudgeDayTrendSave(void);

	// �����ڵ�ǰ���Ʊ��е��������ݣ�ֻ����һ��Сʱ֮��á�һ���ӱ���һ�Σ����ⲿͨ����ʱ�����ƣ���
	void SaveSecondTrendData(void);
	// �����Ʊ���
	void SaveDayTrend(void);
	// Сʱ���Ʊ���
	void SaveHourTrend(void);
	// �����Ʊ���
	void SaveMonthTrend(void);
	// �����Ƽ�¼����
	void SaveVibRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth=false);
	// ���������Ƽ�¼����
	void SaveProcRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth=false);
	// ���²��ı���������Ϣ
	void UpdateChannAlarmParam(CCustomDB *ag_pDB);
};
