#ifndef _BHKD_CHANNALARM_H_MARBLE_
#define _BHKD_CHANNALARM_H_MARBLE_

#include "VibData.h"
#include "OnlineGlobal.h"
#include "../include/DieselPlantAlarm.h"
const float G_F_INIT_VALUE = -12345678;
enum _CHANN_ALARM_STATUS_
{
	CHANN_ALARM_STATUS_INIT                   = 0x00000000,
	CHANN_ALARM_STATUS_NORMAL                 = 0x00000001,
	CHANN_ALARM_STATUS_HALF_NORMAL            = 0x00000010,
	CHANN_ALARM_STATUS_OLD_ALARM_LOW		  = 0x00000100,
	CHANN_ALARM_STATUS_OLD_ALARM_HIGH		  = 0x00001000,
	CHANN_ALARM_STATUS_NEW_ALARM			  = 0x00010000,
};

class CVibChannParamAlarm: public CVibChannParam
{
public:
	CVibChannParamAlarm();
	~CVibChannParamAlarm();
private:
	//�Ƿ��ڳ��α���״̬
	bool _bFirstAlarmS;
	//�Ƿ��ڱ���״̬
    bool _bAlarmStatusS;
	//�Ƿ�ѧϰ������
	bool _bLearnTh;
	//�Ƿ񱨾������󴥷�һ����ѧϰ
	bool _bAlarmLearnTh;
	//�ۼƱ���������������ۼƴ����ﵽָ����Ŀ������Ϊ���ڱ���״̬
	short _siAlarmCountsS[10];
	//�ۼƴ��������α������жϱ������ۼƴ���
	short _siCountsS[10];
	//�²����Ļ��䱨���ۼƴ�����������ۼƴ����ﵽָ����Ŀ������Ϊ�����Ƶ�һ�α���
	int _siNewAlarmCountsS;
	//��㻺�䱨��״̬
	int _iSlowStatus;
    //������Ʊ���״̬
	int _iTrendStatus;
	//��䱨��״̬
	int _iFastStatus;
	//����ͨ�������жϱ������ֽ�λ
	int _iAlarmBit;
	//���汨���ж�
	int m_nRegStatus;
    ///���汨���жϱ�־��-1��ʾû�б�����0����ǰ���汨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
    int _iRegAlarmFlag;
	//���汨��:�ϴδ���һ�α���ʱ�̵�����ֵ
	float _fRegValueLast;
	float m_fLastAlarmValue;

	//���汨��:�ϴδ���һ���±���ʱ�̵�ʱ��
	CBHDateTime _timeNewAlarmLast;

	//�Զ��屨���ж�
	int _iCustomStatus;
	///�Զ��屨���жϱ�־��-1��ʾû�б�����0����ǰ���汨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
	int _iCustomAlarmFlag;
	//�Զ��屨��:�ϴδ���һ���±���ʱ�̵�ʱ��
	CBHDateTime _timeNewCustomAlarmLast;


	//��ǰ���䱨���¼�ID
	long _iEventIDSlow;
	long _iIDSlow;
	//��ǰ��䱨���¼�ID
	long _iEventIDFast;
	//��ǰ�����ֵID
	long _iCurrThIDFast;
	//��ǰ������ֵID
	long _iCurrThIDSlow;
	//���䱨���жϱ�־��-1��ʾû�б�����0����ǰ���䱨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
	int _iSlowAlarmFlag;
    //���䱨���Ľ���ʱ��
	CBHDateTime _tEndSlowAlarmTime;

	LPVOID _pParent;

	//���䱨����ǰ��ѧϰ��õ���ֵ
	CThreshold _pthSlow[D_BHKD_ALARM_CHARTYPE_NUM];
	CThreshold _pthFast[D_BHKD_ALARM_CHARTYPE_NUM];

	//���䱨����һ����ѧϰ��õ���ֵ
	CThreshold _pthSlowLast[D_BHKD_ALARM_CHARTYPE_NUM];

    //��ǰʱ��
	CBHDateTime _timeCurr;
	//�ϴ�ѧϰ����������ֵ����ʱ��
    CBHDateTime _timeSlowThLearn;
	//�ϴ�ѧϰ���������ֵ����ʱ��
	CBHDateTime _timeFastThLearn;
    //�������ݻ����ָ��
 	DATATYPE_WAVE*  _ppfWaveBuf[D_ALARM_SAVEWAVE_NUM];
    //��ǰ��������ָ���λ��
	int _iCurrWavePIndex;
	//��ǰ����ֵλ��
	int _iCurrCharValueIndex;
    //����ֵ��������
	S_VibCharValue _pCharValueBuf[D_ALARM_SAVECHAR_NUM];
	//����ֵ��Ӧ��ʱ�仺��.�Բ���������˵,���µ�D_ALARM_SAVEWAVE_NUM��ô���ʱ��Ͷ�Ӧ�������ݻ���
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];

	//�����Ʊ����ж���ز���
	//�ϴμ��Сʱ���Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckHour;
	//�ϴμ�������Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckDay;
    //�ϴμ�������Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckMonth;
	//�ϴμ�鼾���Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckSeason;

	typedef vector<S_VibCharValue> VECTOR_VIBCHAR;
	//�����ж�ʹ�����ݻ���
	VECTOR_VIBCHAR _vVibCharValue;
//�ͱ���������ز���
	//���ο�ʼ���汨������ʱ��
	CBHDateTime _timeSlowFirstSave;
    //�ϴα��滺�䱨�����ݵ�ʱ��
	CBHDateTime _timeSlowSaveData;
    //���ݿ�����ָ��
//	CBHOleDbWrapper *_pDB;

	//��䱨��״̬
	int m_nFastRegStatus;

private:
	/**To judge alarm of slow change��Ծ�����
	*@return �������0:˵��û�б���,1:���ڱ���;2:���µı�����Ҫ���¿�ʼ5�������ݱ���;3:�ո�����˱�����־,��Ҫ���¿�ʼ5�������ݱ���
	*/
	int JudgeRadialVibSlowAlarm(IDBInterFace *pDB_);
	//���������
	int JudgeAxisVibSlowAlarm(IDBInterFace *pDB_);

    //�ж�ָ����λֵ����ֵ�ж����Ƿ񱨾�
	bool CheckSpecPhaseValueAlarm(float fValue_,CThreshold th_);
	//�ж�ָ����λֵ���ϴ���λֵ,�ж����Ƿ���Ҫ�����±���
	bool CheckPhaseIFNewAlarm(float fValue_,float fLastValue_,CThreshold th_);

	//�����񶯣����䱨��״̬�µģ�5���ӱ����20���ӱ��������жϲ���
    int RadialSlowAlarmStatusOP();

	//�����񶯣����䱨��״̬�µģ�5���ӱ����20���ӱ��������жϲ���
	int AxisSlowAlarmStatusOP();

	//���±���״̬���ж��Ƿ����µı�������,��Ծ�����
	bool JudgeRadialSlowNewAlarm();

	//���±���״̬���ж��Ƿ����µı�������,���������
	bool JudgeAxisSlowNewAlarm();

	/**�жϸ�������ֵ�ı���״̬,���صľ��Ǳ���״̬����Ծ�����
      *@param iAlarmNum_ ��������ֵ��������Ŀ
	  *@param bHaveNew_ �ж��Ƿ����±���������ͨ����ǰ���ݺ��ϴ����ݵĲ�ֵ�Ƿ񳬹�ָ���ٷֱ�.�Ƿ����±���,���ͨ�������������.
	  *@param bCheckNew_ �Ƿ������±�������
	  *@return ����ֵ�ı���״̬���
	  */
	int JudgeRadialSlowCharValue(int *iAlarmNum_,int iCharNum_,bool &bHaveNew_,bool bCheckNew_=false);

	/**�жϸ�������ֵ�ı���״̬,���صľ��Ǳ���״̬�����������
      *@param iAlarmNum_ ��������ֵ��������Ŀ
	  *@param bHaveNew_ �ж��Ƿ����±���������ͨ����ǰ���ݺ��ϴ����ݵĲ�ֵ�Ƿ񳬹�ָ���ٷֱ�.�Ƿ����±���,���ͨ�������������.
	  *@param bCheckNew_ �Ƿ������±�������
	  *@return ����ֵ�ı���״̬���
	  */
	int JudgeAxisSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_=false);

	/**@brief �ж��ڱ���״̬��,�ϴ�ֵ�͵�ǰֵ֮���Ƿ�仯����ָ������
	  *@param fLast_ �ϴ�ֵ
	  *@param fCurr_ ��ǰֵ
	  *@return �������ָ�������ͷ���true�����򷵻�false
	  */
	bool CheckLastAndCurrentValue(float fLast_,float fCurr_);
 	//���沨������
	//bool SaveData(long lEventID_,int iLocationValue_,int iLocationWave_,string sTime_,int iSmpNum_,int iSmpFreq_,float fRev_,int iAlarmType_,CBHOleDbWrapper *pDB_,string sTblName_);

	//��ӻ��䱨����־
	int AddSlowAlarmLog(IDBInterFace *pDB_);
    ///��ӳ��汨����־
	int AddRegAlarmLog(IDBInterFace *pDB_);
	///����Զ��屨����־
	int AddCustomAlarmLog(IDBInterFace *pDB_);
	///��ӿ�䱨����־
	int AddFastAlarmLog(IDBInterFace *pDB_);
	//To judge alarm of fast change
	int JudgeFastAlarm();


    /**��������ѧϰ�ȴ�����ѧϰ����������ǰ�������ޣ��������ޱ��в���������¼
	  *���ѧϰ�ɹ��ͷ�����ѧ����ID
	  */
	int LearnAddSlowThreshold(IDBInterFace *pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//�������ѧϰ
	int LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_);

	/**��ֵ��������ѧϰ��ͬʱѧϰ�����жϱ���������ֵ
	  *@param iLearnDataLen_ ѧϰʹ�õ����ݳ���
	  *@param pVibValue_ ѧϰʹ�õ�����
	  *@return �����������0��˵��ѧϰ�ɹ���С��0˵��ѧϰʧ��
	  */
    int LearnSlowThreshold(int iLearnDataLen_,S_VibCharValue *pVibValue_);

	/**�ж�ָ��ʱ�������Ʊ���
	  *@return ����ֵΪ��������ֵ��1X��2X��3X�ȣ����ͱ������ܺ�
	  */
	int JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_);

	/**@brief ����ʷ�������ƿ��ȡָ��������,���ݱ�������ĳ�Ա����"_vVibCharValue"��
	  *@param timeStart_ ��ʷ���ݵ���ʼʱ��
	  *@param timeEnd_ ��ʷ���ݵ���ֹʱ��
	  *@return ��ȡ��ʷ���ݵļ�¼����
	  */
	int ReadHisData(IDBInterFace *pDB_,CBHDateTime timeStart_,CBHDateTime timeEnd_);
	//�ж����ݵ������Ƿ񱨾�
	int JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//�ж����ݵ������Ƿ񱨾�
	int JudgePhaseTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//�ж�ָ������ֵ���͵��������ݱ���
	int JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_);
	//������Ʊ�����־,���صľ����¼���ID
	int AddTrendAlarmLog(IDBInterFace *pDB_,int iStatus_,int iTrendType_);

	/**@brief �жϳ��������Ƿ񱨾�
	  *@return ������ޱ����ͷ���2�����ޱ����ͷ���1���������ͷ���0
	  */
	int JudgeRegDataAlarm(IDBInterFace *pDB_,bool bConfirm_ = true);



	/**�����汨�������󣬽��е���صĲ���
	  */
	int HandleRegAlarmStop(IDBInterFace *pDB_);

	/**���Զ��屨�������󣬽��е���صĲ���
	*/
	int HandleCustomAlarmStop(IDBInterFace *pDB_);

	/**@brief ������������Ҫ�Ե�ǰ����ֵ�����޸ģ�ͨ������������в���
	  *@param th_ ��ֵ
	  *@param bPhase_ �Ƿ�����λ��ֵ
	  *@return �޸ĺ����ֵ
	  */
	CThreshold ModifyVibThS(CThreshold th_,bool bPhase_=false);
public:
	//�жϿ�䱨��
	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,S_VibCharValue& CharValue_);

	/**@brief �жϳ��汨��
	  *@param iFlag_ ���ñ������Ƿ��ǵ�һ�α���������ǵ�һ�ξʹ���0������С��0
	  @param bSaveAlarmDataFlag_ ���ñ��������һ�α���ID��������ֵ��֮ǰ����ֵ��ʱ���Ƿ������������ݱ�ʶ
	  @param bSaveOneAlarmData_ ���ñ��������30���Ӻ��Ƿ񱣴����ݱ�ʶ
	  *@return ����ֵΪ����״̬
	  */
	int JudgeRegAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_,bool bConfirmAlarm_);






	/**@brief �ж��Զ��屨��
	*@param iFlag_ ���ñ������Ƿ��ǵ�һ�α���������ǵ�һ�ξʹ���0������С��0
	*@return ����ֵΪ����״̬
	*/
	int JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveLastAlarmData_);

	/**@brief ���ݱ�����ʽ���Ƚ����α���ʱ�̵�����ֵ���ж��ظ�����
	*@param fCurrValue_ ��ǰ����ʱ�̵�����ֵ
	@param fValueLast_ �ϴα���ʱ�̵�����ֵ
	*@return ����״̬
	*/
	bool JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_);


	/**@brief ���ݱ�����ʽ���Ƚ����α���ʱ�̵�����ֵ���ж��Ƿ񳬹������豸��ʱ������ֵ
	*@param fCurrValue_ ��ǰ����ʱ�̵�����ֵ
	@param fValueLast_ �ϴα���ʱ�̵�����ֵ
	*@return ����״̬
	*/
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);
	bool JudgeRegAgainAlarmForConfirm();
	/**���ó��汨��״̬
      */
	void ResetRegAlarmStatus();

	/**�����Զ��屨��״̬
	*/
	void ResetCustomAlarmStatus();

	/**���û��䱨��״̬
	*/
	void ResetSlowAlarmStatus();

	//�����������󣬽��е���صĲ���
	int HandleSlowAlarmStop(IDBInterFace *pDB_);

    //To judge alarm of trend
	int JudgeTrendAlarm(IDBInterFace * pDb_);
	/**To judge alarm of slow change
	  *@return �������0:˵��û�б���,1:���ڱ���;2:���µı�����Ҫ���¿�ʼ5�������ݱ���;3:�ո�����˱�����־,��Ҫ���¿�ʼ5�������ݱ���
	  */
	int JudgeSlowAlarm(IDBInterFace *pDB_);
	//To judge all alarm
	int JudgeAlarm();

	void SetParentPoint(LPVOID pParent_);

	/**@brief �õ���ǰ��㱨��״̬
	   *@param pAlarmState_ ���صı�������ϸ״̬
	  *@return ��㱨��״̬
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///���õ�ǰͨ���ı���״̬
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	/**@�õ���ǰ��㻺�䱨��״̬
      *@return ��㱨��״̬
	  */
	int GetSlowAlarmStatus();

	/**@�õ���ǰ��㳣�汨��״̬
	  *@return ��㳣�汨��״̬
	  */
	int GetRegAlarmStatus();

	/**@�õ���ǰ����Զ��屨��״̬
	*@return ����Զ��屨��״̬
	*/
	int GetCustomAlarmStatus();

	/**@�õ���ǰ����䱨��״̬
	*@return ����䱨��״̬
	*/
	int GetFastAlarmStatus();

	virtual bool SetChannValue(BHKD::S_VibCharValue *pVibCharValue_);

	/**@brief ���õ�ǰ��ͨ���Ĳ�������
	  *@param pfWave_ ��������ָ��
	  *@param iSmpNum_ �������ݲ�������
	  *@return true denotes successed, or false denotes failed
	  */
	virtual bool SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_);

	/**@brief �õ�ָ��ͨ���Ĳ�������ָ��
	  *@param iIndex_ λ������
	  *@return ͨ����������ָ��
	  */
	const DATATYPE_WAVE * GetWavePoint(int iIndex_);

	/**@brief �õ���ǰͨ����������ָ��
	  *@param λ������
	  *@return ͨ����������ָ��
	  */
	virtual const DATATYPE_WAVE * GetCurrWavePoint();
	/**@brief ָ���������ݻ����ָ��.���ָ��ָ���ڴ��а�D_ALARM_SAVEWAVE_NUM*iSmpNum_������,����ֱ�������ǰ��㲨�����ݻ���
	  *@param pWave_ �������ݻ���ָ��
	  *@param iSmpNum �������ݵĲ�������
	  */
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

	/**@brief �õ�ָ��������ֱ�����ͼ�϶��ѹ
	  *@return �������ֵΪ-1��˵��û�еõ�
	  */
	int GetDcAndGap(int index_,float &fDC_,float &fGap_);

	/**@brief �õ�ָ�����������͵�����ֵ
	  *@param iCharType_ ����ֵ����
	  *@return ����ֵ
	  */
	DATATYPE_CHAR GetSpecCharTypeValue(int index_,int iCharType_=GE_ALLCHAR);

	/**@brief �õ���ǰֱ�����ͼ�϶��ѹ
	  *@return �������ֵΪ-1��˵��û�еõ�
	  */
	int GetCurrDcAndGap(float &fDC_,float &fGap_);

    /**@brief �õ���ǰָ�����͵�����ֵ
	  *@param iCharType_ ����ֵ����
	  *@return ����ֵ
	  */
	DATATYPE_CHAR GetCurrSpecCharTypeValue(int iCharType_=GE_ALLCHAR);

	/**@brief �õ�ָ������������ֵ
	  *@return ����ֵ
	  */
	S_VibCharValue GetCharValue(int index_);
	S_VibCharValue GetCharValue();
	virtual  S_VibCharValue GetCharValueEx() ;
	virtual  S_VibCharValue GetCharValueEx(int index_) ;
	/**�õ���ǰ����ֵ��λ������
	  */
	int GetCurrCharValueIndex();

	/**�õ���ǰ�������ݵ�λ������
	  */
	int GetCurrWaveIndex();

	/**�õ�ָ������ʱ��
	  */
	string GetSpecIndexTime(int index_);

	/**ʱ��ת��Ϊʱ���ַ���(�����ŵ�)
	  *@param time_ ʱ��
	  *@return �����ַ���
	  */
	CString TimeConvertString(CBHDateTime time_);

	/************************************************************************/
	/* �Ƿ���Ҫ��������ֵ                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ����ֵ�Ƿ�û�м���                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ��������ֵ															*/
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType);
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anIndex);


	/************************************************************************/
	/* ����ͨƵֵ															*/
	/************************************************************************/
	virtual float ComputeAllCharValue(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ������λ�Ʋ�������洢���ݵ�����Ƶ��λֵ								*/
	/************************************************************************/
	virtual float ComputeThreePhaseCharValue(S_VibCharValue  & arefSVibCharValue);


	int JudgeRegDataAlarm(S_VibCharValue & arefVibValue);

	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	float LastDieselAlarmValue() const;
	void LastDieselAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	//��ȡ��������
	int GetChannRegAlarmType();

	/************************************************************************/
	/* �³��汨���߼�����													*/
	/************************************************************************/
public:
	
	_CHANN_ALARM_STATUS_ PredictChannAlarm(IDBInterFace * pDb_,
											bool abPlantAlarmed,//isPlantPreviousAlarmed ��ǰ�豸�Ƿ񱨾�
											bool abConfirm = TRUE,
											bool abChangeStatus = TRUE);
											

	/************************************************************************/
	/* �ϴα��������ֵ														*/
	/************************************************************************/
	

	long m_fLastMaxAlarmValue;
	
	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* �ϴα���ֵ															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* �ϴα�����ֵ                                                         */
	/************************************************************************/
	volatile float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* ���±�����ֵ                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*����������ʱ����Ĭ�ϳ���1��Сʱ������ȫ����                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* �ڲ�ʹ�õĲ�㱨��״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);
	
	
	/************************************************************************/
	/* �л�����ڲ�����״̬                                                 */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus);

	/************************************************************************/
	/* Ԥ��Ĳ���ڲ�����״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDb_);

	/************************************************************************/
	/* ���������ϴα���ֵ���ۼƴ���                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* �豸�Ƿ������µı���                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);

	/************************************************************************/
	/* ��ʼ���ϴα���ֵ���ϴα������ֵ��ֵ                                 */
	/************************************************************************/
	BOOL InitValues();

	/************************************************************************/
	/*��λ���±����߼�                                                        */
	/************************************************************************/
	BOOL JudgeAndDoForNewAxisLogic(BOOL abRunningStatusChanged);//�жϲ������µ���λ���߼�

	float m_fAxisValue;
	void  AxisOriginal(float val);

	float AxisOriginal();

	BOOL SaveAxisValueForNewAxisLogic();

	BOOL InitFileNameForNewAxisAlarmLogic();					//��ʼ����λ���ļ���

	float GetAxisValueForNewAxisAlarmLogic();

	CString m_strAxisValueFilePath;								//�����λ��ֵ���ļ�·��
	CString AxisFilePath() const;
	void AxisFilePath(CString val);

	CBHDateTime m_oTimerForAxisValueLogic;
	CBHDateTime TimerForAxisValueLogic() const;
	void TimerForAxisValueLogic(CBHDateTime val);
	void ResetTimerForAxisValueLogic();
	BOOL IsReachSaveTimeIntervalForAxisValueLogic(); //�Ƿ�ﵽ�˱��沨�ε�ʱ��

	//��㴥���±���ʱ��ֵ
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }

	///���ͻ����汨���߼�
public:

	///�жϵ�ǰ�����Ƿ񱨾�
	int JudgeDiselAlarm(IDBInterFace *pDB_, 
		CDieselPlant *pDieselPlant_,
		bool bConfirmAlarm_,
		const vector<int>& avFlameOutCylinderNo_,
		E_MACHINE_TYPE aeMachineType_);

	_CHANN_ALARM_STATUS_ PredictDieselChannAlarm(IDBInterFace * pDb_,
		bool abPlantAlarmed,//isPlantPreviousAlarmed ��ǰ�豸�Ƿ񱨾�
		const vector<int>& avFlameOutCylinderNo_,
		bool abConfirm /*=TRUE*/,
		bool abChangeStatus/*=TRUE*/);

	_CHANN_ALARM_STATUS_ JudgeAlarmByOtherPlant(  IDBInterFace * pDb_,
		float fCharValue_,
		CBHDateTime tUpdateTime_,
		const map<CBHDateTime,float>& mCharValue_,
		bool abConfirm = TRUE,
		bool abChangeStatus = TRUE);

	CBHDateTime LastCompareAlarmTime();
	void LastCompareAlarmTime(const CBHDateTime atAlarmTime_);

	CBHDateTime LastAlarmTime();
	void LastAlarmTime(const CBHDateTime atAlarmTime_);

	int AlarmCount();
	void AlarmCount(const int anAlarmCount_);
	int IncreaseAlarmCount();
	int DecreaseAlarmCount();

	void SetMultiPointStatus(_CHANN_ALARM_STATUS_ nStatus_);
	_CHANN_ALARM_STATUS_ GetMultiPointStatus();

	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);
	float CalcMeanValue(const vector<float>& vData_);
	bool IsSignalError();
private:
	//�ϴα���ʱ��
	CBHDateTime  m_dtLastAlarmTime;

	CBHDateTime m_dtLastCompareAlarmTime;
	//�ϴα�������
	int         m_nLastAlarmType;

	//��������
	int         m_nAlarmCount;

	E_ALARM_LEVEL m_nAlarmLevel;

	_CHANN_ALARM_STATUS_ m_nMultiPointStatus;

	float m_fLastDieselAlarmValue;
};	


class CProcChannParamAlarm: public CProcChannParam
{
public:
	CProcChannParamAlarm();
	~CProcChannParamAlarm();
public:
	//�Ƿ��Ѿ�ѧϰ����ֵ����
	bool _bLearnTh;
	//�Ƿ��ڳ��α���״̬
	bool _bFirstAlarmS;
	//�Ƿ��ڱ���״̬
	bool _bAlarmStatusS;
	//�ۼƱ���������������ۼƴ����ﵽָ����Ŀ������Ϊ���ڱ���״̬
	short _siAlarmCountsS;
	//�ۼƴ��������α������жϱ������ۼƴ���
	short _siCountsS;
	//��㻺�䱨��״̬
	int _iSlowStatus;
    //��㳣�汨��״̬
    int _iRegStatus;
	///���汨��������¼�ID,���ֵΪ-1��˵��Ŀǰû�б��������߿�����Ϊû����ӹ�������־
	int _iEventIDReg;

	//����ͨ�������жϱ������ֽ�λ
	int _iAlarmBit;
    //�����жϵ���ֵ����
	CThreshold _thSlow;
	//��ǰ���䱨���¼�ID��ID
	long _iEventIDSlow;
    long _iIDSlow;
	//��ǰ�������޵�ID
    long _iThIDSlow;

	//����ֵ��������Ͷ�Ӧʱ�仺��
	float _pfValueBuf[D_ALARM_SAVECHAR_NUM];
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];
	//��ǰ����ֵλ��
	int _iCurrCharValueIndex;

	//�ͱ���������ز���
	//���ο�ʼ���汨������ʱ��
	CBHDateTime _timeFirstSaveS;
	//�ϴα��滺�䱨�����ݵ�ʱ��
	CBHDateTime _timeSaveDataS;
	//�ϴ�ѧϰ����������ֵ����ʱ��
	CBHDateTime _timeThLearnS;

	//���汨�����ο�ʼ���汨������ʱ��
	CBHDateTime _timeFirstSaveReg;
	//�ϴα��泣�汨�����ݵ�ʱ��
	CBHDateTime _timeSaveDataReg;

	//��ǰʱ��
	CBHDateTime _timeCurr;
	//��������ָ��
	LPVOID _pParent;

	float m_fRegValueLast;
	float m_fLastAlarmValue;

	//��䱨��״̬
	int m_nFastRegStatus;

	int m_nProcSUDStatus;//-1:ͣ�� 0:δ�ж� 1������

private:
	//To judge alarm of slow change
	int JudgeSlowAlarm(IDBInterFace * pDb_);
	//���䱨��״̬�µģ�5���ӱ����20���ӱ��������жϲ���
	int SlowAlarmStatusOP(IDBInterFace * pDb_);
	//���±���״̬���ж��Ƿ����µı�������
	bool JudgeNewAlarm();
	//��������
	bool SaveData(IDBInterFace * pDB_,int iEventID_,int iValueIndex_,int iAlarmStatus_,string& sChannNo_,CBHDateTime& tSaveTime_);
	//��ӻ��䱨����־
	int AddSlowAlarmLog(IDBInterFace * pDB_);

	///�����䱨�������󣬽��е���صĲ���
	int HandleSlowAlarmStop(IDBInterFace * pDB_);
	//��������ѧϰ
	int LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//�ж��Ƿ񱨾�
	int JudgeRegDataAlarm(bool bConfirm_ = true);
	//�жϻ������ݱ���
	int CheckSlowDataAlarm();
	/**ʱ��ת��Ϊʱ���ַ���(�����ŵ�)
	*@param time_ ʱ��
	*@return �����ַ���
	*/
	CString TimeConvertString(CBHDateTime time_);




public:

	///�жϿ�����ݱ���
	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,float fCharValue_);
	//To judge all alarm
	int JudgeAlarm(IDBInterFace * pDb_,bool bSaveData_ ,bool bConfirmAlarm_ );

	///�жϳ��汨��
	int JudgeRegAlarm(IDBInterFace *pDB_,bool bSaveData_,bool bConfirmAlarm_);
	/**@�õ���ǰ��㱨��״̬
	  *@param pAlarmState_ ��������ϸ״̬
	  *@return ��㱨��״̬
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///���õ�ǰͨ���ı���״̬
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	void SetParentPoint(LPVOID pParent_);
    virtual void SetData(float fValue_,CBHDateTime dataTime_);
	virtual void SetData(int index, float fValue_,CBHDateTime dataTime_);

	virtual void SetSUDStatus(int anSUDStatus);
	virtual float GetSUDStatus();
	
	bool JudgeRegAgainAlarm();
	bool JudgeRegAgainAlarm(float afNewValue,float afOldValue);
	bool JudgeRegAgainAlarmForConfirm();
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);

	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	//��ȡ��������
	int GetChannRegAlarmType();

	/************************************************************************/
	/* �³��汨���߼�����													*/
	/************************************************************************/
public:

	_CHANN_ALARM_STATUS_ PredictChannAlarm(  IDBInterFace * pDb_,
											 bool abPlantAlarmed,
											 bool abConfirm = TRUE,
											 bool abChangeStatus = TRUE);//isPlantPreviousAlarmed ��ǰ�豸�Ƿ񱨾�


	/************************************************************************/
	/* �ϴα��������ֵ														*/
	/************************************************************************/
	long m_fLastMaxAlarmValue;
	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* �ϴα���ֵ															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* �ϴα�����ֵ                                                         */
	/************************************************************************/
	float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* ���±�����ֵ                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*����������ʱ����Ĭ�ϳ���1��Сʱ������ȫ����                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* �ڲ�ʹ�õĲ�㱨��״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);


	/************************************************************************/
	/* �л�����ڲ�����״̬                                                 */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus,BOOL abSaveData);

	/************************************************************************/
	/* Ԥ��Ĳ���ڲ�����״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDb_);

	/************************************************************************/
	/* ���������ϴα���ֵ���ۼƴ���                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* �豸�Ƿ������µı���                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);
	int HandleRegAlarmStop( IDBInterFace * pDb_ );
	int AddRegAlarmLog( IDBInterFace * pDb_ );
	BOOL InitValues();

	//��㴥���±���ʱ��ֵ
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }
public:
	E_ALARM_LEVEL m_nAlarmLevel;
	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);

};



/**ÿ������λ�ñ�����Ϣ�����ͻ��䱨��
  */
class CAxisAlarmChannParam: public CAxisChannParam
{
public:
	CAxisAlarmChannParam();
	~CAxisAlarmChannParam();
	int JudgeAlarm(IDBInterFace * pDB_);

	int JudgeDynRegAlarm(IDBInterFace * pDb_);
    //�ж϶�̬�����˹켣���ݱ���
	int JudgeDynRegAlarmData();

	///������ת�豸���Ĺ켣��������λ������
	virtual bool UpdateData();

	///���������豸��̬��������λ������
	virtual bool UpdateDynData();

	/**@brief �õ���ǰ��㱨��״̬
      *@param pAlarmState_ ��������ϸ״̬
	  *@return ��㱨��״̬
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);

	///���õ�ǰ����λ�õı���״̬
	void SetAlarmStatus(S_AlarmState sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel sAlarmState_);

	/**�����汨�������󣬽��е���صĲ���
	*/
	int HandleRegAlarmStop(IDBInterFace * pDB_);

	/**@brief �õ���ǰָ�����͵�����ֵ
	*@param iCharType_ ����ֵ����
	*@return ����ֵ
	*/
	DATATYPE_CHAR GetCurrSpecCharTypeValue();
private:
	//�ж��仺�䱨��
	int JudgeSlowAlarm(IDBInterFace * pDb_);

	/**@brief ��ӱ�����־
	  *@param iType_��ʾ����״̬,�ǿ�仹�ǻ���
      *@return �����ӳɹ�,����Ϊ������־���¼�ID;���ʧ��,�ͷ���-1;
	  */
    int AddAlarmLog(IDBInterFace * pDB_,int iType_,int iThID_);

	/**@brief ���汨���󣬱������ݵ�һЩ����
	*/
	int RegAlarmStatusOP(IDBInterFace * pDb_);

	/**@brief ȷ�����䱨���󣬱������ݵ�һЩ����
	  */
	int SlowAlarmStatusOP(IDBInterFace * pDb_);

	/**@brief ��ӻ��䱨����־�״α��滺�����ݵ�һЩ����
	  */
	int FirstSlowAlarmOP(IDBInterFace * pDb_);

	/**@brief ���䱨��ֹͣ��������һЩ����
	  */
	int HandleSlowsAlarmStop(IDBInterFace * pDB_);

	/**@brief ��䱨��ֹͣ��������һЩ����
	*/
	int HandleFastAlarmStop(IDBInterFace * pDB_);

	//�ж����䱨��
	int JudgeFastAlarm();

	//�жϵ�ǰ����λ�������Ƿ��䱨��
	bool CheckAxisDataStatusF();
	//�жϵ�ǰ����λ�������Ƿ񻺱䱨��
	bool CheckAxisDataStatusS();
    //�����ڻ��䱨��״̬��,�ж��Ƿ����µı�������,����ʹ�ͷ��ʼ��������
	bool CheckIfNewSlowAlarm();
	//ѧϰ��䱨����ֵ,�����浽��ֵ��.����ֵΪ��ֵID.���Ϊ-1,˵��ѧϰʧ��
	int LearnAddThresholdF(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_=E_TRIG_TH_ALARM);
	//ѧϰ���䱨����ֵ,�����浽��ֵ��.����ֵΪ��ֵID.���Ϊ-1,˵��ѧϰʧ��
	int LearnAddThresholdS(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_=E_TRIG_TH_ALARM);
	/**ѧϰ���䱨����ֵ,.���Ϊ-1,˵��ѧϰʧ��
	  *@param iDataLen_ ѧϰʹ�õ����ݳ���
	  *@param pAxisData_ ѧϰʹ�õ�����ָ��
	  *@return ����-1˵��ѧϰʧ�ܣ�����0˵��ѧϰ�ɹ�
	  */
	int LearnThresholdS(int iDataLen_,S_AxisData *pAxisData_);

    /**@brief ��������λ������
	  *@param iLocation_ ��ǰ���ݴ������ݻ�����λ��
	  *@return �������ʧ�ܾͷ���-1�����򷵻ر��汨�����ݵı����¼�ID
	  */
	int SaveData(IDBInterFace * pDB_,int iLocation_,string sTblName_,int iEventID_);


	/**ʱ��ת��Ϊʱ���ַ���(�����ŵ�)
	*@param time_ ʱ��
	*@return �����ַ���
	*/
	CString TimeConvertString(CBHDateTime time_);

private:
	S_AxisData _pAxisData[D_ALARM_SAVECHAR_NUM];
    CBHDateTime _pDateTime[D_ALARM_SAVECHAR_NUM];
	CBHDateTime _timeCurr;
	//��ǰ��������λ��,�����ݻ������������
	int _iCurrIndex;
	//��λ��ͨ����ǰ����״̬
	int _iStatus;

	//����ͨ�������жϱ������ֽ�λ
	int _iAlarmBit;

	//��ǰ���汨���¼�ID
	int _iEventIDReg;
	//���汨���ж�
	int _iAxisRegStatus;
	///���汨���жϱ�־��-1��ʾû�б�����0����ǰ���汨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
	int _iRegAlarmFlag;
	CBHDateTime _timeSaveAxisDataFirst;//�״α��泣�汨������ʱ��
	CBHDateTime _timeSaveAxisData;//�ϴα��泣�汨������ʱ��

	//��㻺�䱨��״̬
	int _iSlowStatus;
	//����䱨��״̬
	int _iFastStatus;
    ///�Ƿ�ѧϰ������
	bool _bLearnTh;
	//�״λ��䱨��
	bool _bFirstSlowAlarm;
	//�Ƿ���ڻ��䱨��״̬
	bool _bSlowAlarmStatus;
	//��ǰ���䱨���¼�ID
	int _iEventIDSlow;
    //��ǰ��䱨���¼�ID
	int _iEventIDFast;
	//��ǰ�����ֵID
	int _iCurrThIDFast;
	//��ǰ������ֵID
	int _iCurrThIDSlow;
	//���α����׶�ʹ�õ�����������Ϊ�ж��Ƿ��Ǽٱ���
    //���α����׶��ۻ������Ĵ���
	int _iSlowAlarmCounts;
	//���α����׶��ۻ��жϱ����Ĵ���
	int _iSlowCounts;
    int _iID;
    CThreshold _thS;//�������ֵ
	CThreshold _thF;//������ֵ
	CBHDateTime _timeThLearnS;//������ֵ��ѧϰʱ��
    CBHDateTime _timeThLearnF;//�����ֵ��ѧϰʱ��
	CBHDateTime _timeFirstSaveDataS;//�״α��滺������ʱ��
	CBHDateTime _timeLastSaveDataS;//�ϴα��滺������ʱ��
    //���ݿ�����ָ��
	/*CBHOleDbWrapper *_pDB;*/
};


class CDynChannParamAlarm: public CDynChannParam
{
public:
	CDynChannParamAlarm();
	~CDynChannParamAlarm();
private:
	//�Ƿ��ڳ��α���״̬
	bool _bFirstAlarmS;
	//�Ƿ��ڱ���״̬
	bool _bAlarmStatusS;
	//�Ƿ�ѧϰ������
	bool _bLearnTh;
	//�ۼƱ���������������ۼƴ����ﵽָ����Ŀ������Ϊ���ڱ���״̬
	short _siAlarmCountsS;
	//�ۼƴ��������α������жϱ������ۼƴ���
	short _siCountsS;
	//��㻺�䱨��״̬
	int _iSlowStatus;
	//������Ʊ���״̬
	int _iTrendStatus;
	//��䱨��״̬
	int _iFastStatus;
	//����ͨ�������жϱ������ֽ�λ
	int _iAlarmBit;
	//���汨���ж�
	int _iRegStatus;
	///���汨���жϱ�־��-1��ʾû�б�����0����ǰ���汨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
	int _iRegAlarmFlag;

	//��ǰ���䱨���¼�ID
	long _iEventIDSlow;
	long _iIDSlow;
	//��ǰ��䱨���¼�ID
	long _iEventIDFast;
	//��ǰ�����ֵID
	long _iCurrThIDFast;
	//��ǰ������ֵID
	long _iCurrThIDSlow;

	LPVOID _pParent;

	CThreshold _pthSlow[D_BHKD_ALARM_CHARTYPE_NUM];
	CThreshold _pthFast[D_BHKD_ALARM_CHARTYPE_NUM];
	//��ǰʱ��
	CBHDateTime _timeCurr;
	//�ϴ�ѧϰ����������ֵ����ʱ��
	CBHDateTime _timeSlowThLearn;
	//�ϴ�ѧϰ���������ֵ����ʱ��
	CBHDateTime _timeFastThLearn;
	//�������ݻ����ָ��
	DATATYPE_WAVE*  _ppfWaveBuf[D_ALARM_SAVEWAVE_NUM];
	//��ǰ��������ָ���λ��
	int _iCurrWavePIndex;
	//��ǰ����ֵλ��
	int _iCurrCharValueIndex;
	//����ֵ��������
	S_DynCharValue _pCharValueBuf[D_ALARM_SAVECHAR_NUM];
	//����ֵ��Ӧ��ʱ�仺��.�Բ���������˵,���µ�D_ALARM_SAVEWAVE_NUM��ô���ʱ��Ͷ�Ӧ�������ݻ���
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];


	//�Զ��屨���ж�
	int _iCustomStatus;
	///�Զ��屨���жϱ�־��-1��ʾû�б�����0����ǰ���汨���¼��У��Ѿ�������������Ŀǰû�б�����1����ʾ���ڱ���״̬
	int _iCustomAlarmFlag;
	//�Զ��屨��:�ϴδ���һ���±���ʱ�̵�ʱ��
	CBHDateTime _timeNewCustomAlarmLast;

	//�����Ʊ����ж���ز���
	//�ϴμ��Сʱ���Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckHour;
	//�ϴμ�������Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckDay;
	//�ϴμ�������Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckMonth;
	//�ϴμ�鼾���Ʊ�����ʱ��
	CBHDateTime _timeTrendCheckSeason;

	typedef vector<S_DynCharValue> VECTOR_DYNCHAR;
	//�����ж�ʹ�����ݻ���
	VECTOR_DYNCHAR _vDynCharValue;
	//�ͱ���������ز���
	//���ο�ʼ���汨������ʱ��
	CBHDateTime _timeSlowFirstSave;
	//�ϴα��滺�䱨�����ݵ�ʱ��
	CBHDateTime _timeSlowSaveData;
	//���汨��:�ϴα���ʱ���������ֵ
	float _fDynRegValueLast;
	float m_fLastAlarmValue;

	//���汨��:�ϴδ���һ���±���ʱ�̵�ʱ��
	CBHDateTime _timeDynNewAlarmLast;	

	//��䱨��״̬
	int m_nFastRegStatus;

	//���ݿ�����ָ��
	//	CBHOleDbWrapper *_pDB;
private:
	/**To judge alarm of slow change��Ծ�����
	*@return �������0:˵��û�б���,1:���ڱ���;2:���µı�����Ҫ���¿�ʼ5�������ݱ���;3:�ո�����˱�����־,��Ҫ���¿�ʼ5�������ݱ���
	*/
	int JudgeDynSlowAlarm();

	//�����񶯣����䱨��״̬�µģ�5���ӱ����20���ӱ��������жϲ���
	int DynSlowAlarmStatusOP();

	//���±���״̬���ж��Ƿ����µı�������,��Ծ�����
	bool JudgeDynSlowNewAlarm();


	/**�жϸ�������ֵ�ı���״̬,���صľ��Ǳ���״̬����Ծ�����
	*@param iAlarmNum_ ��������ֵ��������Ŀ
	*@param bHaveNew_ �ж��Ƿ����±���������ͨ����ǰ���ݺ��ϴ����ݵĲ�ֵ�Ƿ񳬹�ָ���ٷֱ�.�Ƿ����±���,���ͨ�������������.
	*@param bCheckNew_ �Ƿ������±�������
	*@return ����ֵ�ı���״̬���
	*/
	int JudgeDynSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_=false);


	/**@brief �ж��ڱ���״̬��,�ϴ�ֵ�͵�ǰֵ֮���Ƿ�仯����ָ������
	*@param fLast_ �ϴ�ֵ
	*@param fCurr_ ��ǰֵ
	*@return �������ָ�������ͷ���true�����򷵻�false
	*/
	bool CheckLastAndCurrentValue(float fLast_,float fCurr_);
	//���沨������
	//bool SaveData(long lEventID_,int iLocationValue_,int iLocationWave_,string sTime_,int iSmpNum_,int iSmpFreq_,float fRev_,int iAlarmType_,CBHOleDbWrapper *pDB_,string sTblName_);

	//��ӻ��䱨����־
	int AddSlowAlarmLog(IDBInterFace * pDB_);
	///��ӳ��汨����־
	int AddRegAlarmLog(IDBInterFace * pDB_);
	///����Զ��屨����־
	int AddCustomAlarmLog(IDBInterFace * pDB_);
	///��ӿ�䱨����־
	int AddFastAlarmLog(IDBInterFace *pDB_);
	//To judge alarm of fast change
	int JudgeFastAlarm();

	/**��������ѧϰ�ȴ�����ѧϰ����������ǰ�������ޣ��������ޱ��в���������¼
	*���ѧϰ�ɹ��ͷ�����ѧ����ID
	*/
	int LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//�������ѧϰ
	int LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_);

	/**��ֵ��������ѧϰ��ͬʱѧϰ�����жϱ���������ֵ
	*@param iLearnDataLen_ ѧϰʹ�õ����ݳ���
	*@param pVibValue_ ѧϰʹ�õ�����
	*@return �����������0��˵��ѧϰ�ɹ���С��0˵��ѧϰʧ��
	*/
	int LearnSlowThreshold(int iLearnDataLen_,S_DynCharValue *pVibValue_);

	/**�ж�ָ��ʱ�������Ʊ���
	*@return ����ֵΪ��������ֵ��1X��2X��3X�ȣ����ͱ������ܺ�
	*/
	int JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_);

	/**@brief ����ʷ�������ƿ��ȡָ��������,���ݱ�������ĳ�Ա����"_vVibCharValue"��
	*@param timeStart_ ��ʷ���ݵ���ʼʱ��
	*@param timeEnd_ ��ʷ���ݵ���ֹʱ��
	*@return ��ȡ��ʷ���ݵļ�¼����
	*/
	int ReadHisData(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_);
	//�ж����ݵ������Ƿ񱨾�
	int JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//�ж�ָ������ֵ���͵��������ݱ���
	int JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_);
	//������Ʊ�����־,���صľ����¼���ID
	int AddTrendAlarmLog(IDBInterFace * pDb_,int iStatus_,int iTrendType_);

	/**@brief �жϳ��������Ƿ񱨾�
	*@return ������ޱ����ͷ���2�����ޱ����ͷ���1���������ͷ���0
	*/
	int JudgeRegDataAlarm(bool bConfirmAlarm_ = true);

	/**�����汨�������󣬽��е���صĲ���
	*/
	int HandleRegAlarmStop(IDBInterFace * pDB_);

	/**���Զ��屨�������󣬽��е���صĲ���
	*/
	int HandleCustomAlarmStop(IDBInterFace * pDB_);

	/**@brief ������������Ҫ�Ե�ǰ����ֵ�����޸ģ�ͨ������������в���
	*@param th_ ��ֵ
	*@param bPhase_ �Ƿ�����λ��ֵ
	*@return �޸ĺ����ֵ
	*/
	CThreshold ModifyDynThS(CThreshold th_,bool bPhase_=false);
public:
	/**@brief �жϳ��汨��
	*@param iFlag_ ���ñ������Ƿ��ǵ�һ�α���������ǵ�һ�ξʹ���0������С��0
	@param bSaveAlarmDataFlag_ ���ñ��������һ�α���ID��������ֵ��֮ǰ����ֵ��ʱ���Ƿ������������ݱ�ʶ
	@param bSaveOneAlarmData_ ���ñ��������30���Ӻ��Ƿ񱣴����ݱ�ʶ
	*@return ����ֵΪ����״̬
	*/
	int JudgeRegAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_,bool bConfirmAlarm_ = true);


	/**@brief �жϳ��汨��
	*@param iFlag_ ���ñ������Ƿ��ǵ�һ�α���������ǵ�һ�ξʹ���0������С��0
	@param bSaveAlarmDataFlag_ ���ñ��������һ�α���ID��������ֵ��֮ǰ����ֵ��ʱ���Ƿ������������ݱ�ʶ
	@param bSaveOneAlarmData_ ���ñ��������30���Ӻ��Ƿ񱣴����ݱ�ʶ
	*@return ����ֵΪ����״̬
	*/
	int JudgeRegAlarmEx(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_);


	/**@brief �ж��Զ��屨��
	*@param iFlag_ ���ñ������Ƿ��ǵ�һ�α���������ǵ�һ�ξʹ���0������С��0
	*@return ����ֵΪ����״̬
	*/
	int JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveLastAlarmData_);

	/**@brief ���ݱ�����ʽ���Ƚ����α���ʱ�̵�����ֵ���ж��ظ�����
	*@param fCurrValue_ ��ǰ����ʱ�̵�����ֵ
	@param fValueLast_ �ϴα���ʱ�̵�����ֵ
	*@return ����״̬
	*/
	bool JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_);

	/**@brief ���ݱ�����ʽ���Ƚ����α���ʱ�̵�����ֵ���ж��Ƿ񳬹������豸��ʱ������ֵ
	*@param fCurrValue_ ��ǰ����ʱ�̵�����ֵ
	@param fValueLast_ �ϴα���ʱ�̵�����ֵ
	*@return ����״̬
	*/
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);
	bool JudgeRegAgainAlarmForConfirm();
	/**���ó��汨��״̬
	*/
	void ResetRegAlarmStatus();

	/**�����Զ��屨��״̬
	*/
	void ResetCustomAlarmStatus();

	//�����������󣬽��е���صĲ���
	int HandleSlowAlarmStop(IDBInterFace * pDb_,bool bLearnTh_=true);

	//To judge alarm of trend
	int JudgeTrendAlarm();
	/**To judge alarm of slow change
	*@return �������0:˵��û�б���,1:���ڱ���;2:���µı�����Ҫ���¿�ʼ5�������ݱ���;3:�ո�����˱�����־,��Ҫ���¿�ʼ5�������ݱ���
	*/
	int JudgeSlowAlarm();
	//To judge all alarm
	int JudgeAlarm();

	void SetParentPoint(LPVOID pParent_);

	/**@brief �õ���ǰ��㱨��״̬
	*@param pAlarmState_ ���صı�������ϸ״̬
	*@return ��㱨��״̬
	*/
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///���õ�ǰͨ���ı���״̬
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	/**@�õ���ǰ��㻺�䱨��״̬
	*@return ��㱨��״̬
	*/
	int GetSlowAlarmStatus();

	/**@�õ���ǰ��㳣�汨��״̬
	*@return ��㳣�汨��״̬
	*/
	int GetRegAlarmStatus();

	/**@�õ���ǰ����Զ��屨��״̬
	*@return ����Զ��屨��״̬
	*/
	int GetCustomAlarmStatus();

	virtual bool SetChannValue(BHKD::S_DynCharValue *pVibCharValue_);

	/**@brief ���õ�ǰ��ͨ���Ĳ�������
	*@param pfWave_ ��������ָ��
	*@param iSmpNum_ �������ݲ�������
	*@return true denotes successed, or false denotes failed
	*/
	virtual bool SetChannWave(const DATATYPE_WAVE *pfWave_,const int &iSmpNum_);

	/**@brief �õ�ָ��ͨ���Ĳ�������ָ��
	*@param iIndex_ λ������
	*@return ͨ����������ָ��
	*/
	const DATATYPE_WAVE * GetWavePoint(int iIndex_);

	/**@brief �õ���ǰͨ����������ָ��
	*@param λ������
	*@return ͨ����������ָ��
	*/
	virtual const DATATYPE_WAVE * GetCurrWavePoint();
	/**@brief ָ���������ݻ����ָ��.���ָ��ָ���ڴ��а�D_ALARM_SAVEWAVE_NUM*iSmpNum_������,����ֱ�������ǰ��㲨�����ݻ���
	*@param pWave_ �������ݻ���ָ��
	*@param iSmpNum �������ݵĲ�������
	*/
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);


	/**@brief �õ�ָ�����������͵�����ֵ
	*@param iCharType_ ����ֵ����
	*@return ����ֵ
	*/
	DATATYPE_CHAR GetSpecCharTypeValue(int index_,int iCharType_=0);


	/**@brief �õ���ǰָ�����͵�����ֵ
	*@param iCharType_ ����ֵ����
	*@return ����ֵ
	*/
	DATATYPE_CHAR GetCurrSpecCharTypeValue(int iCharType_=GE_ALLCHAR);

	/**@brief �õ�ָ������������ֵ
	  *@return ����ֵ
	  */
	S_DynCharValue GetCharValue(int index_);
	S_DynCharValue GetCharValue();

	//�õ�ȫ������ֵ
	virtual  S_DynCharValue GetCharValueEx() ;

	virtual  S_DynCharValue GetCharValueEx(int index_) ;

	/**�õ���ǰ����ֵ��λ������
	  */
	int GetCurrCharValueIndex();

	/**�õ���ǰ�������ݵ�λ������
	  */
	int GetCurrWaveIndex();

	/**�õ�ָ������ʱ��
	  */
	string GetSpecIndexTime(int index_);

	/**ʱ��ת��Ϊʱ���ַ���(�����ŵ�)
	  *@param time_ ʱ��
	  *@return �����ַ���
	  */
	CString TimeConvertString(CBHDateTime time_);

	/************************************************************************/
	/* �Ƿ���Ҫ��������ֵ                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ����ֵ�Ƿ�û�м���                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ��������ֵ															*/
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType);
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anIndex);


	/************************************************************************/
	/* ����ͨƵֵ															*/
	/************************************************************************/
	virtual float ComputeAllCharValue(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* ������λ�Ʋ�������洢���ݵ�����Ƶ��λֵ								*/
	/************************************************************************/
	virtual float ComputeThreePhaseCharValue(S_DynCharValue  & arefSVibCharValue);


	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_, S_DynCharValue& CharValue_);

	//��ȡ��������
	int GetChannRegAlarmType();
	

public:
	
	/************************************************************************/
	/* �³��汨���߼�����													*/
	/************************************************************************/
public:

	_CHANN_ALARM_STATUS_ PredictChannAlarm(IDBInterFace * pDb_,
											bool abPlantAlarmed,
											bool abConfirm = TRUE,
											bool abChangeStatus = TRUE);//isPlantPreviousAlarmed ��ǰ�豸�Ƿ񱨾�


	/************************************************************************/
	/* �ϴα��������ֵ														*/
	/************************************************************************/


	long m_fLastMaxAlarmValue;

	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* �ϴα���ֵ															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* �ϴα�����ֵ                                                         */
	/************************************************************************/
	float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* ���±�����ֵ                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*����������ʱ����Ĭ�ϳ���1��Сʱ������ȫ����                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* �ڲ�ʹ�õĲ�㱨��״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);


	/************************************************************************/
	/* �л�����ڲ�����״̬������ֵ��ʾ�Ƿ��б�����־����                   */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus);

	/************************************************************************/
	/* Ԥ��Ĳ���ڲ�����״̬                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDB_);

	/************************************************************************/
	/* ���������ϴα���ֵ���ۼƴ���                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* �豸�Ƿ������µı���                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);
	BOOL InitValues();


public:
	/************************************************************************/
	/* ֱ�������±����߼�                                                   */
	/************************************************************************/
	BOOL GetBufferForNewDCLogic( std::vector<DATATYPE_WAVE> & arefWaveBuffer );  //�õ��µ�ֱ���������߼��ϴδ洢�Ĳ���
	float GetValueForNewDCLogic();
	BOOL IsWaveAlike(DATATYPE_WAVE * apWaveBuffer1,int anWavebuffer1,
					 DATATYPE_WAVE * apWaveBuffer2,int anWavebuffer2);				  //�жϲ����Ƿ�����
	
	float m_fDCOriginal;												  //�ۼƵ�ֱ�����仯
	float DCOriginal() const;
	void DCOriginal(float val);
	void ResetDCOriginal();											  //�����ۼƵ�ֱ�����仯

	BOOL SaveDataForNewDCLogic();										  //Ϊ�µ�ֱ���������߼��洢�ϴο���ʱ�Ĳ���
	BOOL JudgeAndDoForNewDCLogic(BOOL abRunningStatusChanged);			  //�жϲ������µ�ֱ�����߼�
	
	CString m_strWaveFilePath;											  //��Ų����ļ����ļ�·��
	CString WaveFilePath() const;
	void WaveFilePath(CString val);

	CString m_strValueFilePath;											//���ֵ�ļ����ļ�·��
	CString ValueFilePath() const;
	void ValueFilePath(CString val);

	CString m_strWaveFileNum;
	CString WaveFileNum() const;
	void WaveFileNum(CString val);
	
	BOOL InitFileNameForNewDCAlarmLogic();								//��ʼ��ֱ�����ļ���

	CBHDateTime m_oTimerForNewDCLogic;
	CBHDateTime TimerForNewDCLogic() const;
	void TimerForNewDCLogic(CBHDateTime val);
	void ResetTimerForNewDCLogic();
	BOOL IsReachSaveTimeIntervalForNewDCLogic(); //�Ƿ�ﵽ�˱��沨�ε�ʱ��

	//��㴥���±���ʱ��ֵ
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }
public:
	E_ALARM_LEVEL m_nAlarmLevel;
	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);
	

};

#endif //_BHKD_CHANNALARM_H_MARBLE_