//Implement.h
#pragma  once

#ifndef _SYSTEMSVR_IMPLEMENT_H_
#define _SYSTEMSVR_IMPLEMENT_H_
extern BOOL IsTempMode();
extern int G_TEMP_ONLINE;
class CImplement
{
public:
	CImplement(void);
	~CImplement(void);
public:
	/**�޸ļ�������м��IP��Port
	*���������
	*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
	*           װ����ID        FIELD_SYSTEM_SET
	*          �м��IP    FIELD_SYSTEM_MW_IP
	*          �м��PORT  FIELD_SYSTEM_MW_PORT
	*����ֵ���ֶ��룺
	*          ״̬     FIELD_SYSTEM_STATUS
	*          ״̬���� FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
	int SysModifyMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**�õ���������м��IP��Port
	*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
	*           װ����ID        FIELD_SYSTEM_SET
	*����ֵ���ֶ��룺
	*          �м��IP    FIELD_SYSTEM_MW_IP
	*          �м��PORT  FIELD_SYSTEM_MW_PORT
	#define     SERVICE_SYSTEM_GET_MWINFO                      25021
	*/
	int SysGetMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**�޸ķ��͵�������ĵ�ʱ�������Ƿ���״̬
	*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
	*           װ����ID        FIELD_SYSTEM_SET
	*          ����ʱ����    FIELD_SYSTEM_SENDC_INTERVAL
	*          �Ƿ���״̬    FIELD_SYSTEM_SENDC_IF
	*����ֵ���ֶ��룺
	*          ״̬     FIELD_SYSTEM_STATUS
	*          ״̬���� FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022
	*/
	int SysModifySendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**�õ����͵�������ĵ�ʱ�������Ƿ���
	*           ���ڷֳ�       FIELD_SYSTEM_FACTORY
	*           װ����ID        FIELD_SYSTEM_SET
	*����ֵ���ֶ��룺
	*          ����ʱ����    FIELD_SYSTEM_SENDC_INTERVAL
	*          �Ƿ���״̬    FIELD_SYSTEM_SENDC_IF
	#define     SERVICE_SYSTEM_GET_SENDINFO                     25023
	*/
	int SysGetSendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);


	/**�õ�װ�ü��м����Ϣ
	*���ص��ֶ��룺
	*         �ֳ�����      FIELD_SYSTEM_FACTORY
	*         �ֳ�����      FIELD_SYSTEM_FACTORY_ALIAS
	*         �м����ʶ    FIELD_SYSTEM_MW_ID
	*         װ����ID      FIELD_SYSTEM_SET
	#define    SERVICE_SYSTEM_GET_SETSVRINFO                     25024
	*/
	int SysGetSetSvrInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**�õ�װ�ü��м�����������豸��ϸ��Ϣ
	*        �豸��ʼ����  FIELD_SERVICE_STARTINDEX
	*���ص��ֶ��룺
	*        �豸��Ϣ      FIELD_SYSTEM_SETSVR_PLANTINFO
	*        �汾          FIELD_SYSTEM_VERSION
	*        �豸��Ϣ      FIELD_SYSTEM_SETSVR_PLANTINFO
	*        ��ʾ����      FIELD_SERVICE_GROUP_VISIBLE
	*        ��ʾ��˾      FIELD_SERVICE_COMPANY_VISIBLE
	*        ��ʾ�ֳ�      FIELD_SERVICE_FACTORY_VISIBLE
	*        ��ʾװ��      FIELD_SERVICE_SET_VISIBLE
	*        ��ʾ�豸      FIELD_SERVICE_PLANT_VISIBLE
	*        ��������      FIELD_SYSTEM_GROUP
	*        ��������      FIELD_SYSTEM_COUNTRY
	*        �豸����      FIELD_SERVICE_ENDINDEX
	*        �豸��Ŀ      FIELD_SERVICE_PLANT_NUM
	*        �豸��ʾ����  FIELD_SYSTEM_PLANT_DISPLAY
	*        ERP����       FIELD_SYSTEM_PLANT_ERP_CODE
	#define    SERVICE_SYSTEM_GET_SETSVRPLANTINFO                25025
	*/
	int SysGetSetSvrPlantInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**�õ�װ�ü��м��������ָ���豸�Ĳ����ϸ��Ϣ
	* �����ֶ��룺
	�豸�ţ�FIELD_SYSTEM_PLANT
	*���ص��ֶ��룺
	*        �����Ϣ  FIELD_SYSTEM_SETSVR_CHANNINFO
	*        ����      FIELD_SYSTEM_SETSVR_INFONUM
	#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN            25026
	*/
	int SysGetSetSvrSpecPlantChann(ICommInterFace * pFixComm, IDBInterFace *pDB);
	/**�õ������û���Ϣ
	*�����ֶ��룺
	*�����ֶ��룺
	*            �û���         FIELD_SYSTEM_USEID
	*            ����           FIELD_SYSTEM_PASSWORD
	*            �û�����       FIELD_SYSTEM_USER_LEVEL
	*            ���ڷֳ�       FIELD_SYSTEM_FACTORY
	*            װ����ID       FIELD_SYSTEM_SET
    *            �Ƿ����װ��   FIELD_SYSTEM_BROWSE_SET
	*            ˢ��ʱ�� FIELD_SYSTEM_REFRESHTIME
	#define    SERVICE_SYSTEM_GET_ALLUSERINFO               25027 */
	int SysGetAllUserInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);

	/**�õ�ָ���ֳ������л���λ�ź����IP��Ϣ��ͨ��ѭ�����εõ����л���λ�š����ֱ�����ӷ��ʵĹ�˾������������˾��IP��PortΪ�գ����ֱ�����ӷ��ʵ���װ�ü�����˾��װ�ü�IP��Port��Ϊ��
	*�����������˾��     FIELD_SYSTEM_COMPANY
	*          �ֳ���     FIELD_SYSTEM_FACTORY
	*����ֵ���ֶ��룺
	*          �����     FIELD_SYSTEM_PLANT
	*          ��òͼ���� FIELD_SYSTEM_PLANT_GRAPHTYPE
	*          ��˾��IP   FIELD_SYSTEM_COMPANY_MW_IP
	*          ��˾��Port FIELD_SYSTEM_COMPANY_MW_PORT
	*          װ�ü�IP   FIELD_SYSTEM_MW_IP
	*          װ�ü�Port FIELD_SYSTEM_MW_PORT
	*          �豸����   FIELD_SYSTEM_TYPE
	*          ����Դ     FIELD_SYSTEM_DATASOURCE
	*          ��ʾ����   FIELD_SYSTEM_PLANT_DISPLAY
	*          ERP����    FIELD_SYSTEM_PLANT_ERP_CODE
	#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
	*/
	int SysGetPlantIPofSubCorp(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/*
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX          25210
*/
	int SysGetPlantIPofSubCorpEx(ICommInterFace *pFixComm,IDBInterFace *pDB);


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
	int SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ���Ϣ������
	* �����ֶ��룺
	*���ص��ֶ��룺
	*        ��Ϣ����      FIELD_SYSTEM_TYPE
	*        ��Ӧ����      FIELD_SYSTEM_STATUS_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
	*/
	int SysGetInfoTypeDetail(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**���ݲ����Ϣ����,�õ�������ĵ���Ϣ������
	* �����ֶ��룺
	*        ��Ϣ����   FIELD_SYSTEM_TYPE
	*���ص��ֶ��룺
	*        ����       FIELD_SYSTEM_STATUS
	*        ��Ӧ����   FIELD_SYSTEM_STATUS_DISCRIPTION
	*        ��λ       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
	*/
	int SysGetInfoTypeDetailBaseType(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�޸�ָ���豸ָ�����ı�����Ϣ�����Ϊ-1����ʾû���޸ĳɹ���Ϊ1��ʾ�޸ĳɹ�
	*���������   ��˾��         FIELD_SYSTEM_COMPANY
	*             �ֳ���         FIELD_SYSTEM_FACTORY
	*             ������         FIELD_SYSTEM_PLANT
	*             �����         FIELD_SYSTEM_CHANN
	*             �Ƿ񳣹汨��   FIELD_SYSTEM_STATUS //1Ϊ�жϱ�����0Ϊ���жϱ���
	*             ����������     FIELD_SYSTEM_EVENT_TYPE
	*             ��������       FIELD_SYSTEM_ALARM_LOW
	*             ����������     FIELD_SYSTEM_ALARM_HIGH
	*             ��������       FIELD_SYSTEM_ALARM_LOW2
	*             ����������     FIELD_SYSTEM_ALARM_HIGH2
	*����ֵ���ֶ��룺
	*             ����״̬       FIELD_SYSTEM_STATUS
	#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115
	*/
    int SysModifySpecChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ��Զ������ģ���
	* �����ֶ��룺
	*���ص��ֶ��룺
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         �Զ������ģ��ID       FIELD_SYSTEM_TEMPID
	*         �Զ�����Ϣ����         FIELD_SYSTEM_STATUS_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRTEMPLET      25116
	*/
    int SysGetSvrTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ��Զ��������Ϣ��
	*�������ֵ��
	*���ز���ֵ��
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         �߼�����ģ��ID         FIELD_SYSTEM_TEMPID
	*         ��������               FIELD_SYSTEM_DATA
	*         ��λ����               FIELD_SYSTEM_UNIT
	*         �źŷ�����ʽ           FIELD_SYSTEM_SIGNAL_TYPE
	*         ����ʼ��               FIELD_SYSTEM_SEG_STARTNO
	*         ����                   FIELD_SYSTEM_SEG_NUM
	*         ����                   FIELD_SYSTEM_SEG_NAME
	*         ������ʼֵ             FIELD_SYSTEM_START_VALUE
	*         ���ݽ���ֵ             FIELD_SYSTEM_END_VALUE
	*         ����ֵ����             FIELD_SYSTEM_CHAR_TYPE
	*         Ԥ�������K             FIELD_SERVICE_PREPROC_K
	*         Ԥ�������B             FIELD_SERVICE_PREPROC_B
	*         ��λ                   FIELD_SERVICE_UNIT
	#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM      25117
	*/
	int SysGetCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ��Զ��������������ģ���
	*�������ֵ��
	*���ز���ֵ��
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         �Զ������ģ��ID         FIELD_SYSTEM_TEMPID
	*         �Զ��屨��ģ��ID         FIELD_SYSTEM_ALARM_TEMPID
	*         �Զ��屨������           FIELD_SERVICE_ALARM_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET      25118
	*/
    int SysGetSvrAlarmTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ��Զ����������������Ϣ��
	*�������ֵ��
	*���ز���ֵ��
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         �Զ��屨������ģ��ID   FIELD_SYSTEM_ALARM_TEMPID
	*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
	*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
	*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
	*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
	*         ��������               FIELD_SERVICE_ALARM_LOW
	*         ��������               FIELD_SERVICE_ALARM_HIGH
	*         ������                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119
	*/
    int SysGetSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ�ͨ���Զ����������������Ϣ��
	*�������ֵ��
	*���ز���ֵ��
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         ����                 FIELD_SYSTEM_CHANN
	*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
	*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
	*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
	*         ����                   FIELD_SYSTEM_SEG_NUM
	*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
	*         ��������               FIELD_SERVICE_ALARM_LOW
	*         ��������               FIELD_SERVICE_ALARM_HIGH
	*         ������                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM     25120
	*/
    int SysGetSvrChannAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�ָ����˾���ֳ���װ��ID���Զ��������Ϣ
	*�������ֵ��
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                FIELD_SERVICE_PLANTID
	*���ز���ֵ��
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*         �豸��                 FIELD_SYSTEM_PLANT
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         �߼�����ģ��ID         FIELD_SYSTEM_TEMPID
	*         ��������               FIELD_SYSTEM_DATA
	*         ��λ����               FIELD_SYSTEM_UNIT
	*         �źŷ�����ʽ           FIELD_SYSTEM_SIGNAL_TYPE
	*         ����ʼ��               FIELD_SYSTEM_SEG_STARTNO
	*         ����                   FIELD_SYSTEM_SEG_NUM
	*         ����                   FIELD_SYSTEM_SEG_NAME
	*         ������ʼֵ             FIELD_SYSTEM_START_VALUE
	*         ���ݽ���ֵ             FIELD_SYSTEM_END_VALUE
	*         ����ֵ����             FIELD_SYSTEM_CHAR_TYPE
	*		  ����ֵ��λ			 FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121
	*/
    int SysGetSpecSvrCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�ָ����˾���ֳ���װ��ID���Զ����������������Ϣ
	*�������ֵ��
	*         ��˾��                 FIELD_SYSTEM_COMPANY
	*         �ֳ���                 FIELD_SYSTEM_FACTORY
	*         װ����ID               FIELD_SYSTEM_SET
	*���ز���ֵ��
	*         ����ID                 FIELD_SYSTEM_STATUS
	*         �Զ��屨������ģ��ID   FIELD_SYSTEM_ALARM_TEMPID
	*         �жϷ�ʽ               FIELD_SYSTEM_TYPE
	*         ������ʽ               FIELD_SERVICE_ALARM_TYPE
	*         ��������ʼ��           FIELD_SYSTEM_SEG_STARTNO
	*         ����                   FIELD_SYSTEM_SEG_NUM
	*         �Ƿ񱨾�               FIELD_SYSTEM_IF_ALARM
	*         ��������               FIELD_SERVICE_ALARM_LOW
	*         ��������               FIELD_SERVICE_ALARM_HIGH
	*         ������                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM       25122
	*/
    int SysGetSpecSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�����������õ��Զ�����в�����
    #define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123
	*/
    int SysGetSetSvrCustomBearingTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵĳ����������
    #define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124
	*/
	int SysGetSetSvrGearBoxTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵ�λ�ò�����
	#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125
	*/
    int SysGetSetSvrLocationTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�������ĵĲ�λ������
	#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126
	*/
    int SysGetSetSvrSectionTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**�õ��м���İ汾��
	*�������ֵ��
	*���ز���ֵ��
	*         �汾��                 FIELD_SERVICE_FILE_VERSION
	* #define   SERVICE_SYSTEM_GET_MIDDLEWARE_VERSION     25127
	*/
    int SysGetMiddlewareVersion(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/* �ػ�Ѳ�� */
	/* ��ȡ���������·���Ͱ汾�� */
	int SysGetModuleVersion(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* ��ȡ�������ݿ�״̬ */
	int SysGetDBStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* ��ȡ�м������״̬ */
	int SysGetServerStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* ��ȡ����Ӳ��״̬ */
	int SysGetHWDAQStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* ��ȡ�������״̬ */
	int SysGetDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* ��ȡ��������̬Ϣ  */
	int SysGetBasicConfigInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

    /**�õ�ָ��װ��ID������IDֵ����ID��ʶ���������Ϊ�գ��򷵻�0
	  */
	int GetSetID(IDBInterFace * pDB_,string sSetID_);
	static CString GetSetID2( string &sCompany_,  string &sFactory_,  string &sPlantID_, IDBInterFace *pDB_);
	///�ж�װ�ü���������ʲô����
	int CheckSetSvrLevel(IDBInterFace *pDB_);
	/**ͨ���豸���͡�������ͺʹ��������͵õ�����ֵ����
	  #define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
	  */
	int GetCharTypeDesc(ICommInterFace *pFixComm,IDBInterFace *pDB);
    ///���ָ���豸No
	static string GetPlantNo(const string &sCompany_, const string &sFactory_, const string &sPlantID_, IDBInterFace *pDB_);
	//�õ������˾������Դid��
	int GetCompanyDBID(IDBInterFace *pDB_,string sCompany_);
private:
	//�����Ƿ���װ�úźͷֳ��ŵĲ�ѯ����
	string SetSetFactoryCond(string sCompany_,string sFactory_,string sSetID_);
	//�ж�ָ�����м��ID��ʲô�����ID
	int CheckSpecSetIDLevel(IDBInterFace *pDB_,string sSetID_);
	///ͨ���豸���͡�������͡����������͵õ�ָ������ֵ������Ӧ�ؼ�ֵ�����عؼ�ֵ
	int GetCharTypeDescKey( const int &iMachineType_, const int &iChannType_, const int &iSensorType_ );

	CString GetValueByKey(map<CString,CString> &aoMapData,CString astrCompany,CString astrFactory,CString astrPlant);
};
#endif