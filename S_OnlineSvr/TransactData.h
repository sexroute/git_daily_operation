//#include "OnlineGlobal.h"
#pragma once
#include <list>
#include <string>
#include "additions/DebugHelper.h"

struct S_PlantStatus;

struct structChannel
{		
	int m_nType;
	int m_nOrd;
	int m_nXPos;
	int m_nYPos;
	int m_nWidth;
	int m_nHeight;	
	//char * m_strComments;
	char m_strComments[100];
};


//key
struct AFX_EXT_CLASS SUB_SYSTEM_KEY
{
	int		m_nNumber;		//�����Ŀͻ��˱��(��graph.mdb�еĲ�һ��)
	int		m_nImageKind;	//0:��òͼ, 1:װ��ͼ, 2:ԭ��ͼ
	BOOL	m_bHightAlarm;	//�߱�/�ͱ�
};

//������Ϣ
struct AFX_EXT_CLASS SUB_SYSTEM
{
	int		m_nXOffset;
	int		m_nYOffset;
	BYTE *	m_imageBuffer;
};

inline BOOL WriteBackDataForService_SysGetSpecTypeChann(ICommInterFace * pFixComm,const S_ChannInfo & loInfo,CChannInfoBase * lpVib)
{
	if (lpVib== NULL)
	{
		return FALSE;
	}

	try
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PLANTID,loInfo._cPlantNo);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN,loInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,loInfo._cChannNo);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,loInfo._cChannAlias);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,loInfo._cUnit);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,loInfo._iChannType);
		pFixComm->SetItem(FIELD_SYSTEM_SENSOR_TYPE,loInfo._iSensorType);
		pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,loInfo._iCylinderNo);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,loInfo._iInfoType);


		pFixComm->SetItem(FIELD_SYSTEM_EVENT_TYPE,loInfo._chAlarmParam._iRegJudgeType);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,loInfo._chAlarmParam._fAlarmHL);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,loInfo._chAlarmParam._fAlarmHH);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,loInfo._chAlarmParam._fAlarmLH);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,loInfo._chAlarmParam._fAlarmLL);

		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF, (char*)&(loInfo._chAlarmParam), sizeof(loInfo._chAlarmParam));

		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,loInfo._cTempID);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_TEMPID,loInfo._cAlarmTempID);
		pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE,loInfo._iDataSource);

		CChannSections loSectionInfo = lpVib->GetChannSectionsInfo();

		if (loSectionInfo._iSectionIDX <=0)
		{
			pFixComm->SetItem(FIELD_SERVICE_SEC_NAME, _T("Ĭ�ϲ�λ"));
			pFixComm->SetItem(FIELD_SERVICE_SEC_IDX,  -1);	

		}else
		{
			pFixComm->SetItem(FIELD_SERVICE_SEC_NAME, loSectionInfo._cSectionName);
			pFixComm->SetItem(FIELD_SERVICE_SEC_IDX,  loSectionInfo._iSectionIDX);
		}

		CChannLocations loLocationInfo = lpVib->GetChannLocationsInfo();

		if (loLocationInfo._iLocationIDX>0)
		{
			pFixComm->SetItem(FIELD_SERVICE_LOC_NAME, loLocationInfo._cLocationName);
			pFixComm->SetItem(FIELD_SERVICE_LOC_IDX,  loLocationInfo._iLocationIDX);
		}
		else
		{
			//ASSERT(FALSE);
			pFixComm->SetItem(FIELD_SERVICE_LOC_NAME, _T("Ĭ��λ��"));
			pFixComm->SetItem(FIELD_SERVICE_LOC_IDX,  -1);
		}

		CBearingParamInfo loBearingInfo = lpVib->GetBearingParamInfo();

		BOOL lbRetBearingSet = FALSE;

		if (loBearingInfo._iBearingIDX>0)
		{
			if (loBearingInfo._iBearingType == GE_ROLLING_BEARING)
			{
				lbRetBearingSet = TRUE;
				pFixComm->SetItem(FIELD_SYSTEM_GEARVALID,1);
				pFixComm->SetItem(FIELD_SYSTEM_PITCHDIAM,loBearingInfo._fPitchDiameter);
				pFixComm->SetItem(FIELD_SYSTEM_NUMOFROLLING,loBearingInfo._iElementsNum);
				pFixComm->SetItem(FIELD_SYSTEM_CONTACKANGLE,loBearingInfo._fContactAngle);
				pFixComm->SetItem(FIELD_SYSTEM_ROLLINDIAM,loBearingInfo._fElementDiameter);
			}
		}


		pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST, loInfo._iList);
		pFixComm->SetItem(FIELD_SERVICE_REV, loInfo._iRevType);
		pFixComm->SetItem(FIELD_SERVICE_KEY_IF, loInfo._bKeyIF);
		pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY, loInfo._fSensitivity);
		pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY1, loInfo._fSensitivity1);
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP, loInfo._fGapVoltage);
		pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO, loInfo._iLocationNo);
		pFixComm->SetItem(FIELD_SERVICE_MEASURE_DIRECTION, loInfo._iMeassureDirection);
		pFixComm->SetItem(FIELD_SERVICE_DATA_TYPE, loInfo._iDataType);
		pFixComm->SetItem(FIELD_SERVICE_DC_AMPLIFY, loInfo._fDCAmp);
		pFixComm->SetItem(FIELD_SERVICE_AC_AMPLIFY, loInfo._fACAmp);

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		return FALSE;
	}

	return TRUE;

};

class CTransactData
{
public:
	CTransactData(void);
	~CTransactData(void);
	/**��ȡ���и��ֲ�������ֵ��ͨ��ѭ������ȡ�ø�������������������ֵ��
	#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002
	*/
	int GetAllTypeChar(ICommInterFace* pFixComm);
	// �õ�ָ�����������񶯲����������ֵ
	int GetAllFiveChar(ICommInterFace * pFixComm);
	// �õ�ָ������ָ��������������ֵ
	int GetSpecFiveChar(ICommInterFace * pFixComm);
	/**�õ�ָ���������񶯲���Ƶ�ס������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
	*/
	int GetSpectrum(ICommInterFace * pFixComm,CCustomDB *pDB);
	// �õ�ָ������ָ���ĸ��񶯲���ָ����������ֵ
	int GetFourSpecChar(ICommInterFace * pFixComm);
	// �õ�ָ���������й���������ֵ
	int GetAllProcValue(ICommInterFace * pFixComm);
	// �õ�ָ������ָ������������ֵ
	int GetSpecProcValue(ICommInterFace * pFixComm);
	// �õ�ָ������ָ���������ʵʱ��������
	int GetTwoWave(ICommInterFace * pFixComm);
	// �õ�ָ������ָ��һ��������Ĺ켣����
	int GetAxesTrack(ICommInterFace* pFixComm);
	// �õ�ָ������ָ��һ���������λ��
	int GetAxisLocation(ICommInterFace* pFixComm);
	// �õ�ָ������ָ����λ�Ʋ���ֵ
	int GetSpecAxisDisplaceValue(ICommInterFace* pFixComm);
	// �õ�ָ�������������λ�Ʋ��ֵ
	int GetAllAxisDisplace(ICommInterFace* pFixComm);
	///�ж�ͨ�����������ࡣ���ж�ͨ��һ������
	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);

		/**���ζ�ȡ���վ��ʵʱ����ͳ��
	#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT           20033
	*/
	int OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**�õ��û����۲��豸��Χ�������豸�Ĳ������״̬��ͨ��ѭ�����εõ����б����Ĳ��
  ����*�������ֵ���û�ID         FIELD_SYSTEM_USEID
  ����*����ֵ�ֶ��룺
  ����*            �������豸ID         FIELD_SERVICE_PLANTID
  ����*            �����Ĳ��ID         FIELD_SERVICE_CHANN1
  ����*            ����ֵ               FIELD_SERVICE_CHANN1VALUE
  ����#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
  ����*/
	int GetAllChannelState(ICommInterFace * pFixComm,IDBInterFace *pDB);
	// �õ�ָ���豸�����в������ֵ���񶯲����Ҫ�ƶ������������
	int GetAllChannValue(ICommInterFace * pFixComm, CCustomDB * pDB);

	/**�õ�ָ���������̬ѹ�����ʾ��ͼ��Ϣ��ѭ���õ�������������
     #define   SERVICE_SERVICE_ON_GETPVDATA               20015
	*/
	int GetPVData(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**�õ�ָ������������������ʵʱ���Ρ�Ƶ�׺���������ֵ�������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
	#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
	*/
	int GetRodSinkData(ICommInterFace* pFixComm,CCustomDB *pDB);

	/**��ȡָ����̬ѹ����㡢�����˳�������㡢�񶯲���һ������ȡ�䲨�κ���������
	#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
	*/
	int OnGetRcMultiParamData(ICommInterFace* pFixComm,CCustomDB* pDB);

	/**��ȡ���ż�ʵʱ����ͳ������
	#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
	*/
	int OnGetGroupAlarmStat(ICommInterFace* pFixComm,CCustomDB* pDB);

	/**��ȡ��˾��ʵʱ����ͳ������
	#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT           20025
	*/
	int OnGetCompanyAlarmStat(ICommInterFace *pFixComm,CCustomDB * pDB);

	/**��ȡ���վ��ʵʱ����ͳ��
	#define     SERVICE_SERVICE_ON_MONITORALARMSTAT           20026
	*/
    int OnGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB *pDB);
	int OnGetSpecPlantMonitorAlarmStat(ICommInterFace *pFixComm,
		CCustomDB *pDB,
		CAlarmPlantInfo & arefPlantInfo,
		int anPlantIndex,
		int anTotalPlants);

	/**�õ�ָ������ָ��һ����Ļ���������λ�ù켣
    #define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION     20027
	*/
	int OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**�õ�ָ���豸ָ��������͵����в�����Ͳ��ţ�ͨ��ѭ�����εõ�
    #define     SERVICE_SYSTEM_GETSPECTYPECHANN     25003
	*/
	int Service_SysGetSpecTypeChann(ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**����ʽϵͳӦ����⣬�õ�ָ������ָ���׺ŵ�Ӧ�����κ��񶯲�㲨��
    #define     SERVICE_SERVICE_ON_ GETDYNSTRESSWAVE           20028
	*/
    int OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�ĳһ���ᴿ���Ĺ켣�Ĳ�������
	#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
	*/
	int OnGetPurifyAxesTrack(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**�õ�ָ���񶯲���ʵʱ���κ͵��ס�������񶯲��2�����ò�������ֻ��ȡһ�飩
	#define     SERVICE_SERVICE_ON_GETCEPSTRUM                  20030
	*/
	int OnGetCepstrum(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**�õ�ָ��һ���񶯲���ʵʱȫ�׺�ȫ���ٲ�ͼ��
	#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031
	*/
    int OnGetFullSpectrum(ICommInterFace *pFixComm,CCustomDB *pDB);

	//�õ��񶯲��ķ�ֵ����λ
	int GetAllSpecCharAndPhase(ICommInterFace* pFixComm,CCustomDB* pDB);
	//�õ�ָ����˾,�ֳ����л���ת��
    int GetPlantRev(ICommInterFace *pFixComm);
	int GetSubCorpState(ICommInterFace * pFixComm, CCustomDB * pDB);
	int GetBode(ICommInterFace * pFixComm, CCustomDB * pDB);

    int GetWaterFall(ICommInterFace * pFixComm, CCustomDB * pDB);
	int GetZoomSpec(ICommInterFace * pFixComm,CCustomDB *pDB);
	int GetSpecChannInfo(ICommInterFace *pFixComm,CCustomDB *pDB);

	/* ��ȡ�쳣���ݼ���� */
	int GetAbnormalDataRecord(ICommInterFace *pFixComm,CCustomDB * pDB);

	/**�õ�������ĵ��豸��Ϣ��
	*���������   ��˾��    FIELD_SYSTEM_COMPANY
	*             �ֳ���     FIELD_SYSTEM_FACTORY
	*             �����     FIELD_SYSTEM_PLANT
	*���ص��ֶ��룺
    *             ����     FIELD_SYSTEM_CHANN
	*             ����       FIELD_SYSTEM_STATUS
	*             ��Ӧ����   FIELD_SYSTEM_STATUS_DISCRIPTION
	*             ��λ       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
	*/
	int SysGetMachineInfoType(ICommInterFace *pFixComm,CCustomDB *pDB);

    //�жϻ����Ƿ�������״̬
	bool IsRun(CAlarmPlantInfo *pPlantInfo_);

	int GetBodeForTemp(ICommInterFace * pFixComm, CCustomDB * pDB);

	int GetWaterFallForTemp(ICommInterFace * pFixComm, CCustomDB * pDB);

	 /*!
	*��������ֶ���
	*          ��˾ID      FIELD_SYSTEM_COMPANY
	*	         �ֳ���      FIELD_SYSTEM_FACTORY
	*          ����ID��    FIELD_SERVICE_PLANTID
	*          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
	*          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
	*          �������    FIELD_SYSTEM_CHANN_TYPE
	*����ֵ���ֶ��룺
	*		     �Ƿ��޸ĳɹ���  FIELD_SYSTEM_STATUS
	*#define    SERVICE_SERVICE_ON_SAVESPECTIMEDATA    21080
	*/
   int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**�޸�ָ���豸ָ�����ı�����Ϣ�����Ϊ-1����ʾû���޸ĳɹ���Ϊ1��ʾ�޸ĳɹ�
	*���������  
   *         ��˾��         FIELD_SYSTEM_COMPANY
   *         �ֳ���         FIELD_SYSTEM_FACTORY
   *         ������         FIELD_SYSTEM_PLANT
   *         �����         FIELD_SYSTEM_CHANN
   *         ������ͣ�     FIELD_SERVICE_CHANNTYPE
   *         �Ƿ񳣹汨��   FIELD_SERVICE_DATA_TYPE //1Ϊ�жϱ�����0Ϊ���жϱ���
   *         ����������     FIELD_SYSTEM_ALARM_TYPE
   *         ��������           FIELD_SYSTEM_ALARM_HIGH
   *         ����������         FIELD_SYSTEM_ALARM_HIGH2
   *         ��������           FIELD_SYSTEM_ALARM_LOW
   *         ����������         FIELD_SYSTEM_ALARM_LOW2
   *         �޸ı�����������   FIELD_SYSTEM_HOST_NAME
   *         �޸ı���������IP   FIELD_SYSTEM_CLIENT_IP
   *         �޸ı������û���   FIELD_SYSTEM_USER_NAME
   *����ֵ���ֶ��룺
   *         ����״̬       FIELD_SYSTEM_STATUS
   #define   SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION 20044
   */
   int ChangeSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**��ȡָ���豸ָ�����ı�����Ϣ
   *���������  
   *         ��˾��         FIELD_SYSTEM_COMPANY
   *         �ֳ���         FIELD_SYSTEM_FACTORY
   *         ������         FIELD_SERVICE_PLANTID
   *         �����         FIELD_SYSTEM_CHANN
   *         ������ͣ�     FIELD_SERVICE_CHANNTYPE
   *����ֵ���ֶ��룺
   *         �Ƿ񳣹汨��   FIELD_SERVICE_DATA_TYPE //1Ϊ�жϱ�����0Ϊ���жϱ���
   *         ����������     FIELD_SYSTEM_ALARM_TYPE
   *         ��������           FIELD_SYSTEM_ALARM_HIGH
   *         ����������         FIELD_SYSTEM_ALARM_HIGH2
   *         ��������           FIELD_SYSTEM_ALARM_LOW
   *         ����������         FIELD_SYSTEM_ALARM_LOW2
   *         ����״̬       FIELD_SYSTEM_STATUS // Ϊ1��ʾ��ȡ�ɹ�
   #define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045
   */
   	int GetSpecChannAlarmLimitation( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**�м���л�ȡ��òͼ��Ϣ�ӿ�
	*���������  
	*         ��˾��         FIELD_SYSTEM_COMPANY
	*         �ֳ���         FIELD_SYSTEM_FACTORY
	*         ������         FIELD_SERVICE_PLANTID
	*         �����         FIELD_SYSTEM_CHANN
	*         ������ͣ�     FIELD_SERVICE_CHANNTYPE
	*����ֵ���ֶ��룺
	*         �Ƿ񳣹汨��   FIELD_SERVICE_DATA_TYPE //1Ϊ�жϱ�����0Ϊ���жϱ���
	*         ����������     FIELD_SYSTEM_ALARM_TYPE
	*         ��������       FIELD_SYSTEM_ALARM_LOW
	*         ����������     FIELD_SYSTEM_ALARM_HIGH
	*         ��������       FIELD_SYSTEM_ALARM_LOW2
	*         ����������     FIELD_SYSTEM_ALARM_HIGH2
	*         ����״̬       FIELD_SYSTEM_STATUS // Ϊ1��ʾ��ȡ�ɹ�
	#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION 20048
	*/
    int GetPlantGraphInformationS( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO 20049
	*/
	int GetPlantGraphPositonAndAlarmInfo( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_SET_PLANT_STATUS 24041
	*/
	int SetPlantStatus( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_GET_PLANT_STATUS 24042
	*/
	int GetPlantStatus( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**��ȡָ���豸�����߲��״̬
	#define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047
	*/
	int GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);
	
	/*!
	#define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
	*/
	int GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/*!
	#define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
	*/
	int NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB);

	 /**�м���л�ȡ�����ļ���Ϣ
   *���������  
   *         �ļ�·��                   FIELD_SERVICE_FILE_PATH
   *         �ļ���                     FIELD_SERVICE_FILE_NAME
   *         
   *����ֵ���ֶ��룺
   *         �ļ���                     FIELD_SERVICE_FILE_NAME
   *         �ļ�����                   FIELD_SERVICE_FILE_CONTENT
   *         
   
	#define   SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT 20050
	*/
	int GetLogFileContent(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**
  �����豸���õ��ּ�����
  *��������ֶ���
  *          ��˾��          FIELD_SYSTEM_COMPANY
  *          �ֳ���          FIELD_SYSTEM_FACTORY
  *          �豸��          FIELD_SERVICE_PLANTID
  *          �����          FIELD_SYSTEM_CHANN
  *          �������        FIELD_SERVICE_CHANNTYPE
  *����ֵ���ֶ��룺
  *          һ������        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          ��������        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          ��������        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *          ״̬����        FIELD_SYSTEM_STATUS
  *
 
	#define    SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD         20051
  */
	int GetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);

	/**
  �����豸������÷ּ�����
  *��������ֶ���
  *          ��˾��          FIELD_SYSTEM_COMPANY
  *          �ֳ���          FIELD_SYSTEM_FACTORY
  *          �豸��          FIELD_SERVICE_PLANTID
  *          �����          FIELD_SYSTEM_CHANN
  *          �������        FIELD_SERVICE_CHANNTYPE
  *          һ������        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          ��������        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          ��������        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *����ֵ���ֶ��룺
  *          ״̬����        FIELD_SYSTEM_STATUS
  *
  
	#define    SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD         20052
  */
	int SetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);


	/**
  �����豸���õ�����λ��������Ϣ
  *��������ֶ���
  *          ��˾��          FIELD_SYSTEM_COMPANY
  *          �ֳ���          FIELD_SYSTEM_FACTORY
  *          �豸��          FIELD_SERVICE_PLANTID
  *          �����          FIELD_SYSTEM_CHANN
  *          �������        FIELD_SERVICE_CHANNTYPE
  *����ֵ���ֶ��룺
  *          ��϶��ѹ        FIELD_SERVICE_TREND_GAP
  *          ������������    FIELD_SERVICE_SENSITIVITY
  *          ״̬����        FIELD_SYSTEM_STATUS
  *
  * #define    SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM         20053
  */

  int GetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**
  �����豸������÷ּ�����
  *��������ֶ���
  *          ��˾��          FIELD_SYSTEM_COMPANY
  *          �ֳ���          FIELD_SYSTEM_FACTORY
  *          �豸��          FIELD_SERVICE_PLANTID
  *          �����          FIELD_SYSTEM_CHANN
  *          �������        FIELD_SERVICE_CHANNTYPE
  *          ��϶��ѹ        FIELD_SERVICE_TREND_GAP
  *          ������������    FIELD_SERVICE_SENSITIVITY
  *����ֵ���ֶ��룺
  *          ״̬����        FIELD_SYSTEM_STATUS
  * #define    SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM         20054
  */

  int SetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

private:
	/**����buffer�еĲ����
	  *@param pChannBuf_  ������л�����
	  *@param iNum_       ������л������в��ĸ���
	  *@return �������
	  */
	list<string> ParseChannList(char *pChannBuf_,const int &iNum_);
	//�õ�����״̬
	int GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,CCustomDB *pDB_,bool bChann_=false);
	//�õ�����״̬
	int GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,IDBInterFace *pDB_,bool bChann_=false);
	int GetSpecCompanyChannAlarmState(string sGroup_,string sCompany_,ICommInterFace *pFixComm_,bool bChann_=false);
	int GetSpecFactoryChannAlarmState(string sGroup_,string sCompany_,string sFactory_,ICommInterFace *pFixComm_,bool bChann_=false);
	int GetAllPlantChannAlarmState(ICommInterFace *pFixComm_,bool bChann_=false);


	//�õ�����ͳ����
	int GetSpecGroupAlarmStatics(string sGroup_,CCustomDB *pDB_, S_PlantNumStatics &plantNumStatics_);//int *iPlantNums_ = NULL)
	int GetSpecGroupAlarmStatics(string sGroup_,IDBInterFace *pDB_, S_PlantNumStatics &plantNumStatics_);//int *iPlantNums_ = NULL)
	int GetSpecCompanyAlarmStatics(string sGroup_,string sCompany_,S_PlantNumStatics &plantNumStatics_);
	int GetSpecFactoryAlarmStatics(string sGroup_,string sCompany_,string sFactory_,S_PlantNumStatics &plantNumStatics_);



	/**��ָ�����������ж�ȡ���жϷ�˾���ֳ����豸���пգ����õ������ĺ�׺����ͨ����������ָ�뷵��
	*@param sCompany_ ��˾������һ������
	*@param sFactory_ �ֳ�������һ������
	*@param sPlant_ �豸������һ������
	*@param pFixComm_ ��������ָ��
	*@param bRtn_  �Ƿ񷵻�����������Ϣ
	*@return �����֤�ɹ����򷵻�1�����򷵻�NULL
	*/
	CPlantInfoOnLine * ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_=true,const PLANTINFOTYPE &plantInfoType_=PLANTINFO_REAL);

    /**
	  */
	int SetSpecPlantAlarmState(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_);
	int SetSpecPlantAlarmStateEx(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_);


	int SetPlantAlarmState(CPlantInfoPool * apPlantPool,
		                 std::string asPlant,
		                 ICommInterFace * apFixComm,
		                 CCustomDB * apDB);

	int SetFactoryAlarmState(CFactoryInfoPool * apFactoryPool,
		                 std::string asFactory,
		                 ICommInterFace * apFixComm,
		                 CCustomDB * apDB);

	int SetCompanyAlarmState(CCompanyInfoPool * apCompanyPool,
						 std::string asCompany,
						 ICommInterFace * apFixComm,
						 CCustomDB * apDB);

	/**�жϷ�˾���ֳ����豸���пգ����õ������ĺ�׺����ͨ����������ָ�뷵��
	  *@param sCompany_ ��˾������һ������
	  *@param sFactory_ �ֳ�������һ������
	  *@param sPlant_ �豸������һ������
	  *@param sAlias_ ��һ�����ã����ڰѹ�˾����������
	  *@param pFixComm_ ��������ָ��
	  *@param bRtn_  �Ƿ񷵻�����������Ϣ
	  *@return �����֤�ɹ����򷵻�1�����򷵻�NULL
	*/
	CPlantInfoOnLine * CheckCompanyInfo(const string &sCompany_,const string &sFactory_,const string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_=true,const PLANTINFOTYPE &plantInfoType_=PLANTINFO_REAL);

	/**
	 ����Լ��жϱ���, ����EventID, ���򷵻��¼�ͬ��������EventID
	 */
	static void GetAlarmEventID(CAlarmPlantInfo *apPlantInfo, long & anAlarmId, long & anAlarmSync);

	//��ȡ���ǰ׺
	CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

private:

	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CAlarmPlantInfo* apPlantInfo);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CPlantInfoPool* apPlantPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CPlantInfoPool* apPlantPool, const std::string& asPlant);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CFactoryInfoPool* apFactoryPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CFactoryInfoPool* apFactoryPool, const std::string& asFactory);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CCompanyInfoPool* apCompanyPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CCompanyInfoPool* apCompanyPool, const std::string& asCompany);

	int SkipSpecPlantStatus(S_PlantStatus* apStatus, CString& asError);
	int SetSpecPlantStatus(S_PlantStatus* apStatus, CString& asError);

	int GetDefaultGraphID(int nPlantType_);
};

///���ͱ���״̬���������0��˵��û�б�����1��ʾ����ͱ���2��ʾ����߱���3��ʾ��䱨����4��ʾ���䱨����5��ʾ���Ʊ�����
///����ԽС��Խ����,��������ڳ��汨������£��Ͳ��������жϣ��Ƿ��п�䡢����ȡ�
int ParseAlarmStatus(unsigned int iState_);

