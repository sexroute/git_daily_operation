#include "StdAfx.h"
#include "additions/BHOleDbWrapper.h"

#if!defined(AFX_PROCESS_DSE_MARBLE)
#define AFX_PROCESS_DSE_MARBLE

/**���վ��ʵʱ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY2

*����ֵ���ֶ��룺
*		     �����           FIELD_SERVICE_CHANN1
*		     ���λ��         FIELD_SYSTEM_CHANN_ALIAS
*		     ����ֵ           FIELD_SERVICE_TREND_VALUE
*		     ����״̬         FIELD_SERVICE_CHANN1_STATUS
*		     ת��1            FIELD_SERVICE_REV
*		     ����ID           FIELD_SERVICE_ALARMID
*		     ����ͬ����ʶ     FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT          20033
*/
int Srv_OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

///���庯��
/**�õ������񶯲����������͵�����ֵ��ͨ��ѭ������ȡ�ø�������������������ֵ��
  *�������ֵ��
  *                 ��˾ID   FIELD_SYSTEM_COMPANY
  *			        �ֳ���   FIELD_SYSTEM_FACTORY
  *                 ����ID   FIELD_SERVICE_PLANTID
  *����ֵ���ֶ��룺
  *                 ���ID         FIELD_SERVICE_CHANN1
  *                 ����ֵ�ṹ��   FIELD_SERVICE_CHARVALUE_S
  *                 ����ֵ         FIELD_SYSTEM_ALARM_LOW
  *                 Σ��ֵ         FIELD_SYSTEM_ALARM_HIGH
  *                 ��λ           FIELD_SERVICE_CHANN1_UNIT
  *          ָ���������ת��    FIELD_SERVICE_REV
  *            ʱ��           FIELD_SERVICE_TIME
  *#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000
  */
int Service_OnGetAllFiveChar(ICommInterFace * pFixComm, IDBInterFace *pDB );

/**��ȡָ���񶯲����������͵�����ֵ��
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID   FIELD_SERVICE_PLANTID
*          ���buff       FIELD_SERVICE_CHANN_BUFF
*          ���buff����   FIELD_SERVICE_CHANN_BUFFSIZE
*          ������       FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ����ֵ�ṹ��   FIELD_SERVICE_CHARVALUE_S
*          ���ID         FIELD_SERVICE_CHANN1
*          ���λ��       FIELD_SYSTEM_CHANN_ALIAS
*          ����ֵ         FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ         FIELD_SYSTEM_ALARM_HIGH
*          ��λ           FIELD_SERVICE_CHANN1_UNIT
*          ָ���������ת��    FIELD_SERVICE_REV
 *            ����ֵ�ṹ��   FIELD_SERVICE_CHARVALUE_S
 *            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR        20001
*/
int Service_OnGetSpecFiveChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**��ȡ���и��ֲ�������ֵ��ͨ��ѭ������ȡ�ø�������������������ֵ��
*��ͨ����ʹ�������͵�����ֵ�ṹ�壻FIELD_SERVICE_CHARVALUE_S
*������ͨ����ʹ������ֵ�ֶ� FIELD_SERVICE_TREND_VALUE
*��̬ͨ����ʹ�ö�̬���͵�����ֵ�ṹ�� FIELD_SERVICE_CHARVALUE_S
*ÿѭ�����οɵõ�һ�����ָ����������ֵ
*�������ֵ������ID          FIELD_SERVICE_PLANTID
*            ��˾ID          FIELD_SYSTEM_COMPANY
*			       �ֳ���          FIELD_SYSTEM_FACTORY
*            ���buff        FIELD_SERVICE_CHANN_BUFF
*            ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*            ������        FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*            ���ID          FIELD_SERVICE_CHANN1
*            ����ֵ          FIELD_SERVICE_TREND_VALUE
*        ����ֵ�ṹ��        FIELD_SERVICE_CHARVALUE_S
*            ʱ��            FIELD_SERVICE_TIME
*          ָ���������ת��    FIELD_SERVICE_REV
*          �������       FIELD_SYSTEM_CHANN_TYPE
*          ����������        FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002
*/
int Srv_OnGetAllTypeChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ���������񶯲���Ƶ�ס������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*          ��������         FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          �����                           FIELD_SERVICE_CHANN1
*          ����Ƶ������                   FIELD_SERVICE_FREQWAVE1
*          ���ʵ��Ƶ��һ�����ݵĸ���       FIELD_SERVICE_FREQWAVE_NUM
*          �񶯲��Ƶ��DF                   FIELD_SERVICE_FREQWAVE_DF
*          ����ֵ�ṹ��   FIELD_SERVICE_CHARVALUE_S
*          ָ���������ת��               FIELD_SERVICE_REV
*          ʱ��                             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECTRUM                20003
*/
int Srv_OnGetSpectrum(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**�õ�ָ���������񶯲���ָ����������ֵ
  *���������
  *          ��˾ID         FIELD_SYSTEM_COMPANY
  *			     �ֳ���         FIELD_SYSTEM_FACTORY
  *          ����ID         FIELD_SERVICE_PLANTID
  *          ����ֵ����     FIELD_SERVICE_TREND_CHARTYPE
  *          ���buff     FIELD_SERVICE_CHANN_BUFF
  *          ���buff���� FIELD_SERVICE_CHANN_BUFFSIZE
  *          ������     FIELD_SERVICE_CHANN_NUM
  *����ֵ���ֶ��룺
  *          �������ֵ         FIELD_SERVICE_TREND_VALUE
  *          ���ID             FIELD_SERVICE_CHANN1
  *            ʱ��           FIELD_SERVICE_TIME
  #define     SERVICE_SERVICE_ON_GETFOURSPECCHAR        20004
  */
int SerVice_OnGetFourSpecChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ�������ָ��������������͵����й���������ֵ��ע�⣺�������������񶯲�㣬�򲻷����κ�ֵ��
*�������������ID         FIELD_SERVICE_PLANTID
*          ��˾ID         FIELD_SYSTEM_COMPANY
*			 �ֳ���         FIELD_SYSTEM_FACTORY
*          �������       FIELD_SERVICE_CHANNTYPE
*����ֵ���ֶ��룺
*          ������ֵ       FIELD_SERVICE_TREND_PROCESS
*          ���ID         FIELD_SERVICE_CHANN1
*            ʱ��           FIELD_SERVICE_TIME
  *          ��λ             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETALLPROCVALUE        20005
*/
int SerVice_OnGetAllProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ������������ֵ��
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*		   �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ���������ֵ     FIELD_SERVICE_TREND_PROCESS
*          ���ID           FIELD_SERVICE_CHANN1
*            ʱ��           FIELD_SERVICE_TIME
*          ��λ             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETSPECPROCVALUE       20006
*/
int SerVice_OnGetSpecProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB);
/**�õ�ָ�������񶯲���ʵʱ���κ�Ƶ�ס�������񶯲��2�����ò�������ֻ��ȡһ�飩
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*          ��������         FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          �����                                FIELD_SERVICE_CHANN1
*          ���λ��                              FIELD_SYSTEM_CHANN_ALIAS
*          ���Ĳ�������                        FIELD_SERVICE_TIMEWAVE1
*          ����Ƶ������                        FIELD_SERVICE_FREQWAVE1
*          ���ʵʱ������һ�����ݵĸ���          FIELD_SERVICE_TIMEWAVE_NUM
*          ���ʵ��Ƶ��һ�����ݵĸ���            FIELD_SERVICE_FREQWAVE_NUM
*          �񶯲��Ƶ��DF                        FIELD_SERVICE_FREQWAVE_DF
*          ��������ֵ                            FIELD_SERVICE_CHARVALUE_S
*          ����ֵ                           FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ                           FIELD_SYSTEM_ALARM_HIGH
*          ��λ                             FIELD_SERVICE_CHANN1_UNIT
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETWAVE                20007
*/
int Service_OnGetWave(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**�õ�ĳһ�����Ĺ켣�Ĳ������ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID   FIELD_SERVICE_PLANTID
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*����ֵ���ֶ��룺
*          ���Ĺ켣�Ĵ�ֱ����������       FIELD_SERVICE_AXESTRACK_WAVE_VER
*          ��ֱ����Ĳ�������ѹ������ֽ��� FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          ���Ĺ켣��ˮƽ����������       FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          ˮƽ����Ĳ�������ѹ������ֽ��� FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          ���Ĺ켣��һ���Ĳ������ݸ���     FIELD_SERVICE_AXESTRACK_WAVENUM
*          ָ���������ת��               FIELD_SERVICE_REV
*          ˮƽ��������ֵ                   FIELD_SERVICE_CHARVALUE_S
*          ��ֱ��������ֵ                   FIELD_SERVICE_CHARVALUE_S2
*          ����Ƶ��                         FIELD_SERVICE_SAMPLE_FREQ
*          ˮƽ����ֵ                       FIELD_SYSTEM_ALARM_LOW
*          ˮƽΣ��ֵ                       FIELD_SYSTEM_ALARM_HIGH
*          ��ֱ����ֵ                       FIELD_SYSTEM_ALARM_LOW2
*          ��ֱΣ��ֵ                       FIELD_SYSTEM_ALARM_HIGH2
*          ˮƽ������λ��                 FIELD_SYSTEM_CHANN_ALIAS
*          ��ֱ������λ��                 FIELD_SYSTEM_CHANN2_ALIAS
*          ��λ                             FIELD_SERVICE_CHANN1_UNIT
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETAXESTRACK           20008
*/
int Service_OnGetAxesTrack(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ������ָ��һ��������λ��ֵ��
*���������
*          ��˾ID               FIELD_SYSTEM_COMPANY
*		   �ֳ���               FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���λ������     FIELD_SYSTEM_AXISLOCATION
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*����ֵ���ֶ��룺
*                X�����񶯲��λ��  FIELD_SERVICE_AXESLOCATION_HOR
*                Y�����񶯲��λ��  FIELD_SERVICE_AXESLOCATION_VER
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009
*/
int Service_OnGetAxesLocation(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾�ͷֳ������ת�١����ָ������ֻ��һ��ת�٣���ֻȡFIELD_SERVICE_REV��ֵ
*���������
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*            �豸��buff     FIELD_SERVICE_CHANN_BUFF
*            �豸��buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*            �豸�Ÿ���     FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ����ID          FIELD_SERVICE_PLANTID
*          ָ�������ת��  FIELD_SERVICE_REV
*          ָ�������ת��2 FIELD_SERVICE_OFF_REV2
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETREV                 20010
*/
int Service_OnGetRev(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ������ָ����λ�Ʋ��ֵ
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			       �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID         FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*            ��λ��ֵ       FIELD_SERVICE_TREND_AXISDISPLACE
*            ��϶��ѹ       FIELD_SERVICE_TREND_GAP
*            ֱ����         FIELD_SERVICE_TREND_DC
*            ���ID         FIELD_SERVICE_CHANN1
*            ���λ��       FIELD_SYSTEM_CHANN_ALIAS
*            ����ֵ         FIELD_SYSTEM_ALARM_LOW
*            Σ��ֵ         FIELD_SYSTEM_ALARM_HIGH
*            ��λ           FIELD_SERVICE_CHANN1_UNIT
*            ָ���������ת��    FIELD_SERVICE_REV
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECAXISDISPLACE    20011
*/
int Service_OnGetSpecAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ������������λ�Ʋ��ֵ��ͨ��ѭ�����εõ�������λ��ֵ
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			   �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID   FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*            ��λ��ֵ       FIELD_SERVICE_TREND_AXISDISPLACE
*            ���ID         FIELD_SERVICE_CHANN1
*          ����ֵ         FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ         FIELD_SYSTEM_ALARM_HIGH
*          ��λ           FIELD_SERVICE_CHANN1_UNIT
*          ָ���������ת��    FIELD_SERVICE_REV
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012
*/
int Service_OnGetAllAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ��û����۲���鷶Χ�����л���Ĳ������״̬��ͨ��ѭ�����εõ����б����Ĳ��,�������Ҫͨ����Ϣ���FIELD_SERVICE_IFCHANN��Ϊ-1
*�������ֵ���û�ID         FIELD_SYSTEM_USEID
*            �Ƿ���Ҫͨ��   FIELD_SERVICE_IFCHANN
*����ֵ�ֶ��룺
*            ��˾ID             FIELD_SYSTEM_COMPANY
*			 �ֳ���             FIELD_SYSTEM_FACTORY
*            �����Ļ���ID       FIELD_SERVICE_PLANTID
*            �����Ĳ��ID       FIELD_SERVICE_CHANN1
*            ����ֵ             FIELD_SERVICE_CHANN1VALUE
#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
*/
int Service_OnGetAllChannelState(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ���������в������ֵ��ͨ��ѭ�����εõ�����ֵ��ע�������񶯲����˵��ָ�������������;͸���һ��
*�������ֵ��
*            ��˾ID         FIELD_SYSTEM_COMPANY
*			 �ֳ���         FIELD_SYSTEM_FACTORY
*            ����ֵ����     FIELD_SERVICE_TREND_CHARTYPE
*            �豸��buff     FIELD_SERVICE_CHANN_BUFF
*            �豸��buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*            �豸�Ÿ���     FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*            �豸ID         FIELD_SERVICE_PLANTID
*            ���ID         FIELD_SERVICE_CHANN1
*            ���ֵ         FIELD_SERVICE_CHANN1VALUE
*            ��㵥λ       FIELD_SERVICE_CHANN1_UNIT
*            ��������б�   FIELD_SERVICE_CHANN1_LIST
*            ����״̬     FIELD_SERVICE_CHANN1_STATUS
*            ʱ��           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETALLCHANNVALUE       20014
*/
int Service_OnGetAllChannelValue(ICommInterFace* pFixComm,IDBInterFace *pDB);


/**�õ�ָ���������̬ѹ�����ʾ��ͼ��Ϣ��ѭ���õ�������������
*�������ֵ��
*            ��˾ID         FIELD_SYSTEM_COMPANY
*			       �ֳ���         FIELD_SYSTEM_FACTORY
*            ����ID         FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*            ʾ��ͼ����     FIELD_SYSTEM_TYPE       P-V,P-a,P-t
*����ֵ���ֶ��룺
*          ����         FIELD_SERVICE_CHANN1
*          ������         FIELD_SYSTEM_CHANN_ALIAS
*          ��������         FIELD_SERVICE_TIMEWAVE1
*          X��������        FIELD_SERVICE_TIMEWAVE2
*          ��������ֵ       FIELD_SERVICE_CHARVALUE_S
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_FREQ
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_NUM
*          ����ֵ           FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ           FIELD_SYSTEM_ALARM_HIGH
*          ��λ             FIELD_SERVICE_CHANN1_UNIT
*          ָ������ת��   FIELD_SERVICE_REV
*          ʱ��             FIELD_SERVICE_TIME
#define   SERVICE_SERVICE_ON_GETPVDATA               20015
*/
int Srv_OnGetPVData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ������������������ʵʱ���Ρ�Ƶ�׺���������ֵ�������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*          ��������         FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          �����                  FIELD_SERVICE_CHANN1
*          ������                FIELD_SYSTEM_CHANN_ALIAS
*          ���Ĳ�������          FIELD_SERVICE_TIMEWAVE1
*          ����Ƶ������          FIELD_SERVICE_FREQWAVE1
*          ��㲨�����ݵĵ���      FIELD_SERVICE_TIMEWAVE_NUM
*          ���Ƶ�����ݵĵ���      FIELD_SERVICE_FREQWAVE_NUM
*          �񶯲��Ƶ��DF          FIELD_SERVICE_FREQWAVE_DF
*          ��������ֵ              FIELD_SERVICE_CHARVALUE_S
*          ����ֵ                  FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ                  FIELD_SYSTEM_ALARM_HIGH
*          ��λ                    FIELD_SERVICE_CHANN1_UNIT
*          ָ���������ת��      FIELD_SERVICE_REV
*          ʱ��                    FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
*/
int Srv_OnGetRodSinkData(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**ͨ���ֳ����õ������豸�ı���״̬,���ҵõ�ָ�������ת��
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID  FIELD_SERVICE_PLANTID
*����ֵ�ֶ��룺
*            �����Ļ���ID         FIELD_SERVICE_PLANTID
*            ָ���豸��ת��       FIELD_SERVICE_REV
*            ָ�������ת��2      FIELD_SERVICE_OFF_REV2
#define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017
*/
int Service_OnGetSubCorpState(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ��ָ�������ָ����������ͣ���Ĳ��Ĳ���ͼ���ݣ�һ��ֻ�ܵõ�һ�����ݣ����û��ָ����㣬�������в�������
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*		   �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*          ����ֵ����       FIELD_SERVICE_TREND_CHARTYPE
*����ֵ���ֶ��룺
*          ����           FIELD_SERVICE_CHANN1
*          ��λ��           FIELD_SERVICE_TREND_ONECHAR
*          ��ֵ��           FIELD_SERVICE_TREND_OVERALLCHAR
*          ת�٣�           FIELD_SERVICE_REV
*          ʱ��             FIELD_SERVICE_TIME
*          ��Ӧʱ���΢��   FIELD_SERVICE_MICROSECOND
*          ��ͣ��״̬       FIELD_SERVICE_SUD_STATUS
*          ��ͣ������       FIELD_SERVICE_SUD_TYPE
*          ��ͣ���¼�ID     FIELD_SYSTEM_EVENT_ID
*          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
#define     SERVICE_SERVICE_ON_GETBODE                20018
*/
int Service_OnGetBode(ICommInterFace* pFixComm,IDBInterFace *pDB);


/**�õ�ָ��ָ�������ָ����������ͣ���Ĳ���ٲ�ͼ��һ��ֻ�ܵõ�һ�鲨��Ƶ��
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*		   �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ����           FIELD_SERVICE_CHANN1
*          ��������         FIELD_SERVICE_ZOOM_LINES
*          ����ֵ����       FIELD_SERVICE_TREND_CHARTYPE
*����ֵ���ֶ��룺
*          ��λ��           FIELD_SERVICE_TREND_ONECHAR
*          ��ֵ��           FIELD_SERVICE_TREND_OVERALLCHAR
*          ת�٣�           FIELD_SERVICE_REV
*          Ƶ������         FIELD_SERVICE_FREQWAVE1
*          Ƶ�����ݸ���     FIELD_SERVICE_FREQWAVE_NUM
*          Ƶ��DF           FIELD_SERVICE_FREQWAVE_DF
*          ʱ��             FIELD_SERVICE_TIME
*          ��Ӧʱ���΢��   FIELD_SERVICE_MICROSECOND
*          ��ͣ��״̬       FIELD_SERVICE_SUD_STATUS
*          ��ͣ������       FIELD_SERVICE_SUD_TYPE
*          ��ͣ���¼�ID     FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_GETWATERFALL            20019
*/
int Service_OnGetWaterFall(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ�������񶯲���ϸ��Ƶ�׷����Ͳ��Ρ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID   FIELD_SERVICE_PLANTID
*          ���             FIELD_SERVICE_CHANN1
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*          ��������         FIELD_SERVICE_ZOOM_LINES
*          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
*          ��ֹƵ��         FIELD_SERVICE_ZOOM_STOPFREQ
*����ֵ���ֶ��룺
*          ���Ĳ�������   FIELD_SERVICE_TIMEWAVE1
*          ����Ƶ������   FIELD_SERVICE_ZOOMSPECTRUM
*          �������ݸ���     FIELD_SERVICE_TIMEWAVE_NUM
*          ��������         FIELD_SERVICE_ZOOM_LINES
*          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
*          �񶯲��Ƶ��DF   FIELD_SERVICE_FREQWAVE_DF
*          ״̬             FIELD_SYSTEM_STATUS
*          ״̬����         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020
*/
int Service_OnGetZoomSpec(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**�õ�ָ��ָ������Ͳ��������Ϣ
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*			 �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ����           FIELD_SERVICE_CHANN1
*����ֵ���ֶ��룺
*          �ת�٣�       FIELD_SERVICE_REV
*          ʵʱת�٣�       FIELD_SERVICE_REV2
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_FREQ
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_NUM
#define     SERVICE_SERVICE_ON_GETSPECCHANNINFO           20021
*/
int Service_OnGetSpecChannInfo(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**��ȡָ���豸�񶯲���ָ����Ƶ������ֵ����λ��
*���ָ������ˣ���ֻȡָ����������ֵ����λ
*���û��ָ����㣬�������в��ͨ��ѭ������ȡ�ø������Ķ�Ӧ��Ƶ����ֵ����λ��ÿѭ�����οɵõ�һ�����ָ����������ֵ����λ
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID      FIELD_SERVICE_PLANTID
*            ���ID      FIELD_SERVICE_CHANN1
*            ����ֵ����  FIELD_SERVICE_TREND_CHARTYPE
*����ֵ���ֶ��룺
*            ���ID      FIELD_SERVICE_CHANN1
*            ָ������ֵ  FIELD_SERVICE_TREND_VALUE
*            ��Ӧ����λ  FIELD_SERVICE_TREND_PHASE
*            ת�٣�           FIELD_SERVICE_REV
#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022
*/
int Service_OnGetAllSpecCharAndPhase(ICommInterFace* pFixComm,IDBInterFace* pDB);



/**��ȡָ�������̬ѹ����㡢�����˳�������㡢�񶯲�㡣��ȡ�䲨�κ���������
*������Ϊ�վͲ��᷵�ض�Ӧ������
*�������ֵ��
*          ��˾ID                     FIELD_SYSTEM_COMPANY
*	       �ֳ���                     FIELD_SYSTEM_FACTORY
*          ����ID                     FIELD_SERVICE_PLANTID
*          �񶯲��buff               FIELD_SERVICE_CHANN_BUFF
*          �񶯲��buff����           FIELD_SERVICE_CHANN_BUFFSIZE
*          �񶯲�����               FIELD_SERVICE_CHANN_NUM
*          ��̬���buff               FIELD_SERVICE_CHANN_BUFF2
*          ��̬���buff����           FIELD_SERVICE_CHANN_BUFFSIZE2
*          ��̬������               FIELD_SERVICE_CHANN_NUM2
*          �����˳��������buff       FIELD_SERVICE_CHANN_BUFF3
*          �����˳��������buff����   FIELD_SERVICE_CHANN_BUFFSIZE3
*          �����˳�����������       FIELD_SERVICE_CHANN_NUM3
*�����ֶΣ�
*          �������ݵĵ���             FIELD_SERVICE_TIMEWAVE_NUM
*          ����Ƶ��                   FIELD_SERVICE_SAMPLE_FREQ
*          �������                   FIELD_SERVICE_CHANN
*          ��㲨��                   FIELD_SERVICE_TIMEWAVE
*          �������ֵ                 FIELD_SERVICE_CHARVALUE_S
*          ������                     FIELD_SERVICE_FREQWAVE2
*          ת��                       FIELD_SERVICE_REV
*          ʱ��                       FIELD_SERVICE_TIME
*          ������������λ��           FIELD_SERVICE_ANGLEPARAM1
*          �������ر���λ��           FIELD_SERVICE_ANGLEPARAM2
*          ������������λ��           FIELD_SERVICE_ANGLEPARAM3
*          �������ر���λ��           FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
*/
int Srv_OnGetRcMultiParamData(ICommInterFace* pFixComm,IDBInterFace* pDB);


/**��ȡ���ż�ʵʱ����ͳ������
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���          FIELD_SYSTEM_FACTORY
*����ֵ���ֶ��룺
*		     �豸��          FIELD_SERVICE_PLANT_NUM
*		     �����豸��      FIELD_SERVICE_ALARM_PLANTNUM
*		     �����豸��      FIELD_SERVICE_RUN_PLANTNUM
*		     ͣ���豸��      FIELD_SERVICE_STOP_PLANTNUM
#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
*/
int Srv_OnGetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**��ȡ��˾��ʵʱ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*����ֵ���ֶ��룺
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*		     �豸��           FIELD_SERVICE_PLANTID
*		     �����           FIELD_SERVICE_CHANN_NUM
*		     ����״̬         FIELD_SERVICE_ALARM_STATUS
*		     ��ͣ��״̬       FIELD_SERVICE_SUD_STATUS
*		     ת��1            FIELD_SERVICE_REV
*		     ����ID           FIELD_SERVICE_ALARMID
*		     ����ͬ����ʶ    FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT         20025
*/
int Srv_OnGetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**���վ��ʵʱ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*		     �����           FIELD_SERVICE_CHANN1
*		     ���λ��         FIELD_SYSTEM_CHANN_ALIAS
*		     ����ֵ           FIELD_SERVICE_TREND_VALUE
*		     ����״̬         FIELD_SERVICE_CHANN1_STATUS
*		     ת��1            FIELD_SERVICE_REV
*		     ����ID           FIELD_SERVICE_ALARMID
*		     ����ͬ����ʶ     FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_MONITORALARMSTAT          20026
*/
int Srv_OnGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ������ָ��һ����Ļ���������λ�ù켣
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*		   �ֳ���            FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���λ������      FIELD_SYSTEM_AXISLOCATION
*          ˮƽ������ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ������ID   FIELD_SERVICE_CHANNID_VER
*          ���ε���          FIELD_SERVICE_TIMEWAVE_NUM
*����ֵ���ֶ��룺
*          ˮƽ����������    FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          ��ֱ����������    FIELD_SERVICE_AXESTRACK_WAVE_VER
*          ʵ�ʲ��ε���        FIELD_SERVICE_AXESTRACK_WAVENUM
*          ˮƽ����������ֵ  FIELD_SERVICE_CHARVALUE_S
*          ��ֱ����������ֵ  FIELD_SERVICE_CHARVALUE_S2
*          ʱ��                FIELD_SERVICE_TIME
*		   ת��1               FIELD_SERVICE_REV
#define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION         20027
*/
int Srv_OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ʽϵͳӦ����⣬�õ�ָ������ָ���׺ŵ�Ӧ�����κ��񶯲�㲨��
*���������
*          ��˾ID        ������    FIELD_SYSTEM_COMPANY
*		  �ֳ���          ������  FIELD_SYSTEM_FACTORY
*          ����ID          ������  FIELD_SERVICE_PLANTID
*          ���׺�                  FIELD_SERVICE_CYLINDERNO
*          �����񶯲��            FIELD_SERVICE_CHANN1
*����ֵ���ֶ��룺
*          ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE1
*          ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE2
*          ������Ӧ���������ݡ�����FIELD_SERVICE_FREQWAVE1
*          �񶯲�������   �������� FIELD_SERVICE_FREQWAVE2
*          �������ݸ���   �������� FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_ON_GETDYNSTRESSWAVE              20028
*/
int Srv_OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ĳһ���ᴿ���Ĺ켣�Ĳ������ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		  �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID   FIELD_SERVICE_PLANTID
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*����ֵ���ֶ��룺
*          ��ֱ���������������Ƶ��       FIELD_SERVICE_VERFREQ
*          ��ֱ��������������ķ�ֵ       FIELD_SERVICE_VERAMP
*          ��ֱ�����������������λ       FIELD_SERVICE_VERPHASE
*          ˮƽ���������������Ƶ��       FIELD_SERVICE_HORFREQ
*          ˮƽ��������������ķ�ֵ       FIELD_SERVICE_HORAMP
*          ˮƽ�����������������λ       FIELD_SERVICE_HORPHASE
*          ˮƽ��������ֵ                 FIELD_SERVICE_CHARVALUE_S
*          ��ֱ��������ֵ                 FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
*/
int Srv_OnGetPurifyAxesTrack(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ���񶯲���ʵʱ���κ͵��ס�������񶯲��2�����ò�������ֻ��ȡһ�飩
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*		  �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���buff         FIELD_SERVICE_CHANN_BUFF
*          ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*          ������         FIELD_SERVICE_CHANN_NUM
*          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
*����ֵ���ֶ��룺
*          �����                                FIELD_SERVICE_CHANN1
*          ���λ��                              FIELD_SYSTEM_CHANN_ALIAS
*          ���Ĳ�������                        FIELD_SERVICE_TIMEWAVE1
*          ���ĵ�������                        FIELD_SERVICE_FREQWAVE1
*          ���ʵʱ������һ�����ݵĸ���          FIELD_SERVICE_TIMEWAVE_NUM
*          ���ʵ�ʵ���һ�����ݵĸ���            FIELD_SERVICE_FREQWAVE_NUM
*          �񶯲�㵹��DF                        FIELD_SERVICE_FREQWAVE_DF
*          ��������ֵ                            FIELD_SERVICE_CHARVALUE_S
*          ����ֵ                                FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ                                FIELD_SYSTEM_ALARM_HIGH
*          ��λ                                  FIELD_SERVICE_CHANN1_UNIT
*          ָ���������ת��                    FIELD_SERVICE_REV
*          ʱ��                                  FIELD_SERVICE_TIME
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM1
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM2
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM3
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_ON_GETCEPSTRUM          20030
*/
int Srv_OnGetCepstrum(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ��һ���񶯲���ʵʱȫ�׺�ȫ���ٲ�ͼ��
*���������
*          ��˾ID                           FIELD_SYSTEM_COMPANY
*		  �ֳ���                           FIELD_SYSTEM_FACTORY
*          ����ID                           FIELD_SERVICE_PLANTID
*          ���λ������                     FIELD_SYSTEM_AXISLOCATION
*          ˮƽ�����񶯲��ID               FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID               FIELD_SERVICE_CHANNID_VER
*          ���ε���                         FIELD_SERVICE_TIMEWAVE_NUM
*����ֵ���ֶ��룺
*          ȫ��Ƶ�ʺͷ�ֵ��Ϣ�ṹ��         FIELD_SERVICE_LEARNPARAM_BUFF
*          ȫ����Ϣ�ṹ���С               FIELD_SERVICE_CHANN_BUFFSIZE
*          ָ���������ת��               FIELD_SERVICE_REV
*          ˮƽ��������ֵ                   FIELD_SERVICE_CHARVALUE_S
*          ��ֱ��������ֵ                   FIELD_SERVICE_CHARVALUE_S2
*          ����Ƶ��                         FIELD_SERVICE_SAMPLE_FREQ
*          ʱ��                             FIELD_SERVICE_TIME
*          ת������                       FIELD_SYSTEM_TYPE //0:Ϊ��ʱ��,1Ϊ˳ʱ��
#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031
*/
int Srv_OnGetFullSpectrum(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*********************************************************************/

/**����ָ��������񶯲��ʵʱ��������ֵ����λ�����ݵ��м��ģ��
  *��������ֶ��룺
  *            ��˾ID   FIELD_SYSTEM_COMPANY
  *			 �ֳ���   FIELD_SYSTEM_FACTORY
  *            ����ID   FIELD_SERVICE_PLANTID
*            �񶯲��ͨƵ����ֵ����          FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����                        FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            һ�鲨�ε���                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ����Ƶ��                        FIELD_SERVICE_SAMPLE_FREQ
*            ��������                        FIELD_SERVICE_SAMPLE_NUM
*            ѹ������ֽ���                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            �����ת��                      FIELD_SERVICE_REV
*            �����ת��2                     FIELD_SERVICE_REV2
*            �����ת��3                     FIELD_SERVICE_REV3
*            �����ת��4                     FIELD_SERVICE_REV4
*            �����ת��5                     FIELD_SERVICE_REV5
*            ������ʼͨ������                FIELD_SERVICE_STARTINDEX
*            ���ڽ���ͨ������                FIELD_SERVICE_ENDINDEX
*            �Ƿ���θ����������            FIELD_SERVICE_SEND_END
*            �Ƿ���Ҫ�жϱ���                FIELD_SERVICE_JUDGE_ALARM
*����ֵ���ֶ��룺
*            ������                FIELD_SYSTEM_STATUS
*            ����                    FIELD_SERVICE_ON_SEND_STARTID
*            ��䱨��ѧϰ����BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
*            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
*            ���ͨ���Ƿ��жϿ��    FIELD_SYSTEM_SENDC_IF
*            ѧϰ�����ṹ��ĳ���    FIELD_SERVICE_LEN
#define     SERVICE_SERVICE_ON_RECEIVEALLVALUE        24000
*/
int Service_OnReceiveVibValue(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**����ָ������Ķ�̬���ʵʱ��������ֵ�������ݵ��м��ģ��
*��������ֶ��룺
*            ��˾ID                 FIELD_SYSTEM_COMPANY
*			       �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID                 FIELD_SERVICE_PLANTID
*            ��̬�������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
*            ��̬������           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
*            ��������               FIELD_SERVICE_SAMPLE_NUM
*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            �����ת��             FIELD_SERVICE_REV
*            �����ת��2            FIELD_SERVICE_REV2
*            �����ת��3            FIELD_SERVICE_REV3
*            �����ת��4            FIELD_SERVICE_REV4
*            �����ת��5            FIELD_SERVICE_REV5
*            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
*            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
*            �Ƿ���θ����������    FIELD_SERVICE_SEND_END
*            �Ƿ���Ҫ�жϱ���                FIELD_SERVICE_JUDGE_ALARM
*����ֵ���ֶ��룺
*            ������                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001
*/
int Srv_OnReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ת�ٵ��м��ģ��
  *��������ֶ��룺
  *                ��˾ID   FIELD_SYSTEM_COMPANY
  *	    		   �ֳ���   FIELD_SYSTEM_FACTORY
  *                ����ID      FIELD_SERVICE_PLANTID
  *                �����ת��  FIELD_SERVICE_REV
  *                �����ת��2 FIELD_SERVICE_REV2
  *����ֵ���ֶ��룺��
 #define     SERVICE_SERVICE_ON_RECEIVEREV             24002
 */
int Service_OnReceiveRev(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**����ALLPROC����������λ�����ݵ��м��ģ�飬���õ�ָ�����͵�����
*��������ֶ��룺
*                ��˾ID   FIELD_SYSTEM_COMPANY
*                �ֳ���   FIELD_SYSTEM_FACTORY
*                ����ID      FIELD_SERVICE_PLANTID
*                ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
*                �������          FIELD_SERVICE_CHANNTYPE
*                ��Ҫ���ݲ������  FIELD_SERVICE_ON_SEND_CHANNTYPE
*                ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            �Ƿ���Ҫ�жϱ���                FIELD_SERVICE_JUDGE_ALARM
*����ֵ���ֶ��룺���FIELD_SERVICE_ON_SEND_CHANNTYPE�ֶβ���Ϊ0��2���Ͳ���������
*                ����              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*                ����              FIELD_SERVICE_ON_SEND_AXISDISPLACE
#define     SERVICE_SERVICE_ON_RECEIVEPROC            24003
*/
int Service_OnReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**���ͻ�������״̬���ݵ��м��ģ�顣������������ݵı���״̬
*��������ֶ��룺
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			       �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID                          FIELD_SERVICE_PLANTID
*            ����λ��״̬                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            ����λ�ø���                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            ������״̬                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            ����������                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            �񶯲��״̬                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��̬���״̬                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            ��̬������                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            ����EVENTID                     FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺��
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24004
*/
int Service_OnReceiveChannState(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**���ͻ�������״̬���ݵ��м��ģ�顣������������ݵı���״̬
*��������ֶ��룺
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			       �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID                          FIELD_SERVICE_PLANTID
*            ����λ��״̬                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            ����λ�ø���                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            ������״̬                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            ����������                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            �񶯲��״̬                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��̬���״̬                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            ��̬������                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            ����EVENTID                     FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺��
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24027
*/
int Service_OnReceiveChannStateWithAlarmLevel(ICommInterFace *pFixComm,IDBInterFace * pDB);
/**����ָ��ָ���豸����ͣ�����ݵ��м��
*��������ֶ��룺
*             ��˾ID          FIELD_SYSTEM_COMPANY
*			  �ֳ���          FIELD_SYSTEM_FACTORY
*             ����ID��        FIELD_SERVICE_PLANTID
*             ��ʼ����        FIELD_SERVICE_ON_SEND_STARTID
*             ��ͣ����־λ    FIELD_SERVICE_SUD_STATUS
*             ��ͣ���¼�ID:   FIELD_SYSTEM_EVENT_ID
*             ָ�������ת��  FIELD_SERVICE_REV
*             ָ�������ת��2 FIELD_SERVICE_REV2
*             ����ֵ����      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             ͨ������        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             ��������        FIELD_SERVICE_ON_SEND_VIBWAVE
*             һ�鲨�ε���     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             ����Ƶ��        FIELD_SERVICE_SAMPLE_FREQ
*             ��������        FIELD_SERVICE_SAMPLE_NUM
*            ��Ӧ��ʱ��       FIELD_SERVICE_TIME
*            ��Ӧʱ���΢��   FIELD_SERVICE_MICROSECOND
*             ��ͣ������     FIELD_SERVICE_SUD_TYPE
*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_REVSUDDATA            24005
*/
int Service_OnRevSudData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**֪ͨ�м������������Ҫ��������
*��������ֶ��룺
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*����ֵ�ֶ��룺��
#define     SERVICE_SERVICE_ON_SAVEDATA              24006
*/
int Service_OnSrvSaveData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**֪ͨ�м������������Ҫ�жϱ���
*��������ֶ��룺
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			   �ֳ���   FIELD_SYSTEM_FACTORY
*����ֵ�ֶ��룺��
#define     SERVICE_SERVICE_ON_JUDGEALARM            24007
*/
int Service_OnSrvJudgeAlarm(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**������ֵID��������鳤��С��1��ʾʧ��
*��������ֶ���
*           ��˾ID          FIELD_SYSTEM_COMPANY
*			      �ֳ���          FIELD_SYSTEM_FACTORY
*           ����ID��        FIELD_SERVICE_PLANTID
*           ͨ������        FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*           ��ֵID����      FIELD_SERVICE_THRESHOLD_ID
*           ���鳤�ȣ�ͨ�������� FIELD_SERVICE_CHANN_NUM
#define     SERVICE_SERVICE_ON_REQTHESHOLDID       24011
*/
int Service_OnReqThresholdID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ����˾���ֳ����豸�Ŀ�䱨���¼�ID
*��������ֶ��룺
*             ��˾ID         FIELD_SYSTEM_COMPANY
*			  �ֳ���         FIELD_SYSTEM_FACTORY
*             ����ID��       FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*             ��䱨���¼�ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012
*/
int Srv_OnReqFastAlarmEventID(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**����ָ����˾���ֳ����豸���񶯲���䱨��״̬
*��������ֶ��룺
*             ��˾ID            FIELD_SYSTEM_COMPANY
*			        �ֳ���            FIELD_SYSTEM_FACTORY
*             ����ID��          FIELD_SERVICE_PLANTID
*             ͨ������          FIELD_SERVICE_ON_SEND_STARTID
*             �񶯵ı���״̬    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*             ��ͨ������      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*����ֵ���ֶ��룺
*             �ɹ���ʶ            FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS     24013
*/
int Srv_OnUpdateFastAlarmStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ��������񶯲������󱣴����������ֵ����λ�����ݵ��м��ģ��
*��������ֶ��룺
*            ��˾ID                          FIELD_SYSTEM_COMPANY
*			 �ֳ���                          FIELD_SYSTEM_FACTORY
*            ����ID                          FIELD_SERVICE_PLANTID
*            �񶯲������ֵ����              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����                        FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            �ܵĲ��ε���                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ѹ������ֽ���                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            ʱ��ֵ                          FIELD_SERVICE_TIME
*            �Ƿ���θ����������            FIELD_SERVICE_SEND_END
*����ֵ���ֶ��룺
*            ������                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE        24021
*/
int Srv_NetOffReceiveAllValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ������Ķ�̬�������󱣴����������ֵ�������ݵ��м��ģ��
*��������ֶ��룺
*            ��˾ID                 FIELD_SYSTEM_COMPANY
*			 �ֳ���                 FIELD_SYSTEM_FACTORY
*            ����ID                 FIELD_SERVICE_PLANTID
*            ��̬�������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
*            ��̬������           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
*            �ܵĲ��ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            ʱ��ֵ                 FIELD_SERVICE_TIME
*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
*����ֵ���ֶ��룺
*            ������                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE        24022
*/
int Srv_NetOffReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���Ͷ����󱣴��ALLPROC����������λ�����ݵ��м��ģ�飬���õ�ָ�����͵�����
*��������ֶ��룺
*                ��˾ID            FIELD_SYSTEM_COMPANY
*                �ֳ���            FIELD_SYSTEM_FACTORY
*                ����ID            FIELD_SERVICE_PLANTID
*                ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
*                �������          FIELD_SERVICE_CHANNTYPE
*                ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*                ʱ��ֵ            FIELD_SERVICE_TIME
*����ֵ���ֶ��룺
*                ������          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEPROC            24023
*/
int Srv_NetOffReceiveProc(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ��������񶯲������󱣴����������ֵ����λ�����ݵ��м��ģ��
*��������ֶ��룺
*            ��˾ID                          FIELD_SYSTEM_COMPANY
*			 �ֳ���                          FIELD_SYSTEM_FACTORY
*            ����ID                          FIELD_SERVICE_PLANTID
*            �񶯲������ֵ����              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����                        FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            �ܵĲ��ε���                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ѹ������ֽ���                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            ʱ��ֵ                          FIELD_SERVICE_TIME
*            ʱ��ֵ                          FIELD_SERVICE_MICROSECOND
*            �Ƿ���θ����������            FIELD_SERVICE_SEND_END
*����ֵ���ֶ��룺
*            ������                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE         24024
*/

int  Srv_NetOffReceiveAllWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/***-------------------------------------------------------------****/

/**����ALLPROC���������м��ģ�飬���õ�ָ�����͵�����
*��������ֶ��룺
*                ��˾ID   FIELD_SYSTEM_COMPANY
*			       �ֳ���   FIELD_SYSTEM_FACTORY
*                ����ID            FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*                ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA        24051
*/
int Service_OnGetSpecPlantProcData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**����ָ������ı���״̬��Ϊ����״̬�Ľṹ��
*��������ֶ��룺
*                ��˾ID   FIELD_SYSTEM_COMPANY
*			           �ֳ���   FIELD_SYSTEM_FACTORY
*                ����ID   FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*            ����λ��״̬                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            ����λ�ø���                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            ������״̬                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            ����������                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            �񶯲��״̬                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            �񶯲�����                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��̬���״̬                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            ��̬������                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE       24052
*/
int Service_OnGetSpecPlantAllState(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**����ָ���豸�õ�ָ�������ͳ��ȵ��񶯲�������
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*			 �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID    FIELD_SERVICE_PLANTID
*            ��ʼ����  FIELD_SERVICE_ON_SEND_STARTID
*            ����      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            Ŀǰ���ݵ�ʱ�� FIELD_SERVICE_TIME
*���ز���ֵ��
*            �񶯲������ֵ����      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����                FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����            FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������                FIELD_SERVICE_ON_SEND_VIBWAVE
*            �ܵĲ��ε���            FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ���ݵ�ʱ��              FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA    24050
*/
int Service_OnGetSpecPlantLengthVibData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**����ָ���豸ָ�������ͳ��ȵĶ�̬����
*�������ֵ��
*            ��˾ID   FIELD_SYSTEM_COMPANY
*		     �ֳ���   FIELD_SYSTEM_FACTORY
*            ����ID    FIELD_SERVICE_PLANTID
*            ��ʼ����  FIELD_SERVICE_ON_SEND_STARTID
*            ����      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            Ŀǰ���ݵ�ʱ�� FIELD_SERVICE_TIME
*���ز���ֵ��
*            ��̬�������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
*            ��̬������           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
*            �ܵĲ��ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA    24054
*/
int Service_OnGetSpecPlantLengthDynData(ICommInterFace *pFixComm,IDBInterFace * pDB);


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
int Service_SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB);


/* ��ȡ�쳣���ݼ���� */
int Service_OnGetAbnormalDataRecord(ICommInterFace *pFixComm,IDBInterFace * pDB);
/* ��������Ӳ��״̬ */
int Service_OnRcvDAQHWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

/* �����������״̬ */
int Service_OnRcvDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

int Service_SysGetSpecTypeChann(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**���ָ���������ͨ������̬��Ϣ
  *��������ֶ��룺
  *            ��˾ID                 FIELD_SYSTEM_COMPANY
  *			   �ֳ���                 FIELD_SYSTEM_FACTORY
  *            ����ID                 FIELD_SERVICE_PLANTID
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
			   �񶯲�����          FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *����ֵ���ֶ��룺
  *            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
  
  #define     SERVICE_SERVICE_ON_GET_VIBINFO        24066

  */


int Srv_GetVibInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);



/**���ָ������Ķ�̬ͨ������̬��Ϣ
  *��������ֶ��룺
  *            ��˾ID                 FIELD_SYSTEM_COMPANY
  *			   �ֳ���                 FIELD_SYSTEM_FACTORY
  *            ����ID                 FIELD_SERVICE_PLANTID
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
			   �񶯲�����          FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *����ֵ���ֶ��룺
  *            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
  *				#define     SERVICE_SERVICE_ON_GET_DYNINFO        24067
  */

int Srv_GetDynInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**ȷ��ָ�����ı�����Ϣ
  *��������ֶ��룺
  *���������  ��˾��     FIELD_SYSTEM_COMPANY
  *            �ֳ���     FIELD_SYSTEM_FACTORY
  *			   װ����	  FIELD_SYSTEM_SET
  *            �����     FIELD_SYSTEM_PLANT
  *            ����     FIELD_SYSTEM_CHANN
  *			   ȷ������	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            ȷ����ʼʱ��   FIELD_SERVICE_OFF_TIMESTART
  *	           ȷ�Ͻ���ʱ��   FIELD_SERVICE_OFF_TIMEEND
  *			   ȷ�ϵ�ǰʱ��	  FIELD_SERVICE_TIME
  *			   �û���		  FIELD_SYSTEM_USEID
  *			   ����ԭ�����   FIELD_SERVICE_DATA_CODE
  *			   ��ע			  FIELD_SERVICE_REMARK
  *			   �ͻ���IP		  FIELD_SYSTEM_CLIENT_IP
  *����ֵ���ֶ��룺
  *            �����ɹ����          FIELD_SERVICE_DATA_CODE 0ʧ�� 1�ɹ�
  *				#define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */

int Srv_Alarm_Confirm(ICommInterFace * pFixComm,IDBInterFace * pDB);


/**��ͣ�����̷���ָ��������񶯲��ʵʱ����������ֵ����λ�����ݵ��м��ģ��,���ҷ��ش��͵���ͨ����Ӧ�Ŀ�䱨������ѧϰ����
  *��������ֶ��룺
  *            ��˾ID                 FIELD_SYSTEM_COMPANY
  *			   �ֳ���                 FIELD_SYSTEM_FACTORY
  *            ����ID                 FIELD_SERVICE_PLANTID
  *            �񶯲������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
  *            �񶯲�����           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //����4ϵ���м��Ϊ�ܵĲ��ε���
  *            �Զ����������BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            �Զ����������BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
  *            ��������               FIELD_SERVICE_SAMPLE_NUM
  *            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            �����ת��             FIELD_SERVICE_REV
  *            �����ת��2            FIELD_SERVICE_REV2
  *            �����ת��3            FIELD_SERVICE_REV3
  *            �����ת��4            FIELD_SERVICE_REV4
  *            �����ת��5            FIELD_SERVICE_REV5
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
  *            ��Ӧ��ʱ��             FIELD_SERVICE_TIME
  *            ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
  *            ��ͣ������             FIELD_SERVICE_SUD_TYPE
  *            ��ͣ����־λ           FIELD_SERVICE_SUD_STATUS
  *            ��ͣ���¼�ID:          FIELD_SYSTEM_EVENT_ID
  *����ֵ���ֶ��룺
  *            ������                FIELD_SYSTEM_STATUS
  *            ����                    FIELD_SERVICE_ON_SEND_STARTID
  *            ��䱨��ѧϰ����BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
  *            ���ͨ���Ƿ��жϿ��    FIELD_SYSTEM_SENDC_IF
  *            ѧϰ�����ṹ��ĳ���    FIELD_SERVICE_LEN
  *            ѹ����ʽ(0 ��ѹ��)      FIELD_SERVICE_DATA_COMPRESSION
  * #define     SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE        24100
  */
	int ReceiveSudVibValue(ICommInterFace * pFixComm,IDBInterFace *pDB);

	/**����ָ������Ķ�̬���ʵʱ��������ֵ�������ݵ��м��ģ��
	*��������ֶ��룺
	*            ��˾ID                 FIELD_SYSTEM_COMPANY
	*			       �ֳ���           FIELD_SYSTEM_FACTORY
	*            ����ID                 FIELD_SERVICE_PLANTID
	*            ��̬�������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
	*            ��̬������           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
	*            ��������               FIELD_SERVICE_SAMPLE_NUM
	*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            �����ת��             FIELD_SERVICE_REV
	*            �����ת��2            FIELD_SERVICE_REV2
	*            �����ת��3            FIELD_SERVICE_REV3
	*            �����ת��4            FIELD_SERVICE_REV4
	*            �����ת��5            FIELD_SERVICE_REV5
	*            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
	*            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
	*            �Ƿ���θ����������    FIELD_SERVICE_SEND_END
	*            ��Ӧ��ʱ��             FIELD_SERVICE_TIME
	*            ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
	*            ��ͣ������             FIELD_SERVICE_SUD_TYPE
	*            ��ͣ����־λ           FIELD_SERVICE_SUD_STATUS
	*            ��ͣ���¼�ID:          FIELD_SYSTEM_EVENT_ID
	*����ֵ���ֶ��룺
	*            ������                FIELD_SYSTEM_STATUS
	* #define     SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE        24101
	*/
   int ReceiveSudDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**����ALLPROC����������λ�����ݵ��м��ģ�飬���õ�ָ�����͵�����
   *��������ֶ��룺
   *                ��˾ID   FIELD_SYSTEM_COMPANY
   *                �ֳ���   FIELD_SYSTEM_FACTORY
   *                ����ID      FIELD_SERVICE_PLANTID
   *                ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
   *                �������          FIELD_SERVICE_CHANNTYPE
   *                ��Ҫ���ݲ������  FIELD_SERVICE_ON_SEND_CHANNTYPE
   *                ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
   *                ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
   *����ֵ���ֶ��룺���FIELD_SERVICE_ON_SEND_CHANNTYPE�ֶβ���Ϊ0��2���Ͳ���������
   *                ����              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
   *                ����              FIELD_SERVICE_ON_SEND_AXISDISPLACE
   *                ��Ӧ��ʱ��             FIELD_SERVICE_TIME
   *                ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
   *                ��ͣ������             FIELD_SERVICE_SUD_TYPE
   *                ��ͣ����־λ           FIELD_SERVICE_SUD_STATUS
   *                ��ͣ���¼�ID:          FIELD_SYSTEM_EVENT_ID
   *    #define     SERVICE_SERVICE_ON_RECEIVEPROC            24103
   */
   int ReceiveSudProcValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**����ָ�������񶯿�����ݵ��м�������õ�����״̬
	*��������ֶ��룺
	*            ��˾ID                 FIELD_SYSTEM_COMPANY
	*			   �ֳ���                 FIELD_SYSTEM_FACTORY
	*            ����ID                 FIELD_SERVICE_PLANTID
	*            �񶯲������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
	*            �񶯲�����           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //����4ϵ���м��Ϊ�ܵĲ��ε���
	*            �Զ����������BUFF     FIELD_SERVICE_LEARNPARAM_BUF
	*            �Զ����������BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
	*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
	*            ��������               FIELD_SERVICE_SAMPLE_NUM
	*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            �����ת��             FIELD_SERVICE_REV
	*            �����ת��2            FIELD_SERVICE_REV2
	*            �����ת��3            FIELD_SERVICE_REV3
	*            �����ת��4            FIELD_SERVICE_REV4
	*            �����ת��5            FIELD_SERVICE_REV5
	*            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
	*            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
	*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
	*����ֵ���ֶ��룺
	*            ����״̬                FIELD_SERVICE_ALARM_STATUS
	*            �����¼�ID              FIELD_SYSTEM_EVENT_ID
    #define     SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE        24104
	*/
   int ReceiveVibFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**����ָ������Ķ�̬���ʵʱ������ݵ��м��ģ�飬���õ�����״̬�ͱ����¼�ID
	*��������ֶ��룺
	*            ��˾ID                 FIELD_SYSTEM_COMPANY
	*			 �ֳ���                 FIELD_SYSTEM_FACTORY
	*            ����ID                 FIELD_SERVICE_PLANTID
	*            ��̬�������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
	*            ��̬������           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //����4ϵ���м��Ϊ�ܵĲ��ε���
	*            �Զ����������BUFF     FIELD_SERVICE_LEARNPARAM_BUF
	*            �Զ����������BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE  
	*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
	*            ��������               FIELD_SERVICE_SAMPLE_NUM
	*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            �����ת��             FIELD_SERVICE_REV
	*            �����ת��2            FIELD_SERVICE_REV2
	*            �����ת��3            FIELD_SERVICE_REV3
	*            �����ת��4            FIELD_SERVICE_REV4
	*            �����ת��5            FIELD_SERVICE_REV5
	*            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
	*            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
	*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
	*            ѹ����ʽ(0 ��ѹ��)     FIELD_SERVICE_DATA_COMPRESSION��
	*����ֵ���ֶ��룺
	*            ����״̬                FIELD_SERVICE_ALARM_STATUS
	*            �����¼�ID              FIELD_SYSTEM_EVENT_ID
	#define     SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE        24105
	*/
   int ReceiveDynFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**����ALLPROC����������λ�ƿ�����ݵ��м��ģ�飬���õ�ָ�����͵�����
  *��������ֶ��룺
  *           ��˾ID   FIELD_SYSTEM_COMPANY
  *			  �ֳ���   FIELD_SYSTEM_FACTORY
  *           ����ID      FIELD_SERVICE_PLANTID
  *           ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
  *           �������          FIELD_SERVICE_CHANNTYPE
  *           ��Ҫ���ݲ������  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *����ֵ���ֶ��룺
  *           ����״̬                FIELD_SERVICE_ALARM_STATUS
  *           �����¼�ID              FIELD_SYSTEM_EVENT_ID
 
  #define     SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE        24106 
  */
   int ReceiveProcFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**����ALLPROC����������λ�ƿ�����ݵ��м��ģ�飬���õ�����״̬�ͱ����¼�ID
  *��������ֶ��룺
  *           ��˾ID            FIELD_SYSTEM_COMPANY
  *			  �ֳ���            FIELD_SYSTEM_FACTORY
  *           ����ID            FIELD_SERVICE_PLANTID
  *           ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
  *           �������          FIELD_SERVICE_CHANNTYPE
  *           ��Ҫ���ݲ������  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *           �����¼�ID        FIELD_SYSTEM_EVENT_ID
  *           ��Ӧ��ʱ��        FIELD_SERVICE_TIME
  *           ת��              FIELD_SERVICE_REV
  *����ֵ���ֶ��룺
  *           �ɹ���ʶ          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_RECPROCFASTALARMDATA           24107
*/
int Srv_OnReceiveProcFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�����񶯿�䱨�����ݵ��м������
  *��������ֶ��룺
	*             ��˾ID           FIELD_SYSTEM_COMPANY
	*			  �ֳ���           FIELD_SYSTEM_FACTORY
	*             ����ID��         FIELD_SERVICE_PLANTID
	*             ��ʼ����         FIELD_SERVICE_ON_SEND_STARTID
	*             ������������     FIELD_SERVICE_ALARM_TYPE        ///���ȣ�FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             ��䱨���¼�ID   FIELD_SYSTEM_EVENT_ID
	*             ָ�������ת��   FIELD_SERVICE_REV
	*             ָ�������ת��2  FIELD_SERVICE_REV2
	*             ָ�������ת��3  FIELD_SERVICE_REV3
	*             ָ�������ת��4  FIELD_SERVICE_REV4
	*             ָ�������ת��5  FIELD_SERVICE_REV5
	*             ����ֵ����       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             ͨ������         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             ��������         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            һ�鲨�ε���      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //����4ϵ���м��Ϊ�ܵĲ��ε���
	*             ����Ƶ��         FIELD_SERVICE_SAMPLE_FREQ
	*             ��������         FIELD_SERVICE_SAMPLE_NUM
	*            �Զ����������BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///��ͬ�����ֽ��������ֶ�
	*            �Զ������ģ��ID    FIELD_SYSTEM_TEMPID           ///��ͬ�����ֽ��������ֶ�
	*            ��Ӧ��ʱ��        FIELD_SERVICE_TIME
	*            ��Ӧʱ���΢��    FIELD_SERVICE_MICROSECOND
	*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
	*            ѹ����ʽ(0 ��ѹ��)     FIELD_SERVICE_DATA_COMPRESSION
	*����ֵ���ֶ��룺
	*            ��϶��ѹ          FIELD_SERVICE_TREND_GAP  
	*            �ɹ���ʶ      FIELD_SYSTEM_STATUS
 
	#define     SERVICE_SERVICE_ON_RECVIBFASTALARMDATA        24108 */
int Srv_OnReceiveVibFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���Ͷ�̬��䱨�����ݵ��м������
*��������ֶ��룺
*             ��˾ID              FIELD_SYSTEM_COMPANY
*			  �ֳ���              FIELD_SYSTEM_FACTORY
*             ����ID��            FIELD_SERVICE_PLANTID
*             ��ʼ����            FIELD_SERVICE_ON_SEND_STARTID
*             ������������        FIELD_SERVICE_ALARM_TYPE
*             ��̬�����¼�ID      FIELD_SYSTEM_EVENT_ID
*             ����ֵ����		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             ͨ������			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             ��������            FIELD_SERVICE_ON_SEND_VIBWAVE
*             �ܵĲ��ε���        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             �Զ����������BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
*             �Զ������ģ��ID    FIELD_SYSTEM_TEMPID
*             ��Ӧ��ʱ��          FIELD_SERVICE_TIME
*             ��Ӧʱ���΢��      FIELD_SERVICE_MICROSECOND
*             �Ƿ���θ����������   FIELD_SERVICE_SEND_END
*             ѹ����ʽ(0 ��ѹ��)     FIELD_SERVICE_DATA_COMPRESSION
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
*             ��϶��ѹ          FIELD_SERVICE_TREND_GAP  

#define     SERVICE_SERVICE_ON_RECDYNFASTALARMDATA          24109*/
int Srv_OnReceiveDynFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ�����������߲ɼ����ݵ��м��
*��������ֶ��룺
*            ��˾ID                 FIELD_SYSTEM_COMPANY
*			   �ֳ���                 FIELD_SYSTEM_FACTORY
*            ����ID                 FIELD_SERVICE_PLANTID
*            �񶯲������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //����4ϵ���м��Ϊ�ܵĲ��ε���
*            �Զ����������BUFF     FIELD_SERVICE_LEARNPARAM_BUF
*            �Զ����������BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
*            ��������               FIELD_SERVICE_SAMPLE_NUM
*            ѹ������ֽ���         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            �����ת��             FIELD_SERVICE_REV
*            �����ת��2            FIELD_SERVICE_REV2
*            �����ת��3            FIELD_SERVICE_REV3
*            �����ת��4            FIELD_SERVICE_REV4
*            �����ת��5            FIELD_SERVICE_REV5
*            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
*            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
*            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
*            ���ݲɼ�ʱ��           FIELD_SERVICE_TIME
*            ���ݲɼ�ʱ�䣨MS��     FIELD_SERVICE_MICROSECOND
*����ֵ���ֶ��룺��
*            ������                FIELD_SYSTEM_STATUS
*            ����                    FIELD_SERVICE_ON_SEND_STARTID

#define     SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE        24115
*/
int Srv_OnReceiveVibWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


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




   int Srv_test(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**�õ�ָ��ָ�������ɹ������жϵĻ��鿪ͣ��״̬
   *���������
   *          ��˾ID           FIELD_SYSTEM_COMPANY
   *		   �ֳ���           FIELD_SYSTEM_FACTORY
   *          ����ID           FIELD_SERVICE_PLANTID
   *����ֵ���ֶ��룺
   *          ��λ��           FIELD_SERVICE_TREND_ONECHAR
   #define    SERVICE_SYSTEM_QUERY_PROC_RUNSTATUS           20043
   */
   int Service_QueryProcRunstatus(ICommInterFace* pFixComm,IDBInterFace *pDB);

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
  int GetSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB);

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

  /**�м���л�ȡ��λ����Ϣ�ӿ�
  *���������  
  *         �����òͼ                 FIELD_SERVICE_BHCLIENT_GROUPHINDEX
  *         �����òͼ����ID��         FIELD_SYSTEM_PLANT_GRAPHTYPE
  *         
  *����ֵ���ֶ��룺
  *         �ͻ��˵Ĳ�λ��             FIELD_SERVICE_GROUPH_ACCESS_NUMBER
  *         
  *         �ͱ�low_alarm_xoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_XOFFSET
  *         �ͱ�low_alarm_yoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_YOFFSET
  *         �߱�low_alarm_xoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_XOFFSET
  *         �߱�low_alarm_yoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_YOFFSET
  *
  *         �ͱ�ͼƬbuffer��С         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
  *         �ͱ�ͼƬbuffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
  *         �߱�ͼƬbuffer��С         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
  *         �߱�ͼƬbuffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
  *
  #define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO 20049
  */
  int GetPlantGraphPositonAndAlarmInfo( ICommInterFace * pFixComm, IDBInterFace *pDB );

   /*
  #define   SERVICE_SERVICE_ON_SET_PLANT_STATUS 20041
  */
  int Srv_SetPlantStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

  /*
  #define   SERVICE_SERVICE_ON_GET_PLANT_STATUS 20042
  */
  int Srv_GetPlantStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

  /**�������߲��״̬
  *���������  
  *         ��˾��         FIELD_SYSTEM_COMPANY
  *         �ֳ���         FIELD_SYSTEM_FACTORY
  *         ������         FIELD_SERVICE_PLANTID
  *         �������       FIELD_SERVICE_CHANNTYPE
  *         ��ʼ����       FIELD_SERVICE_ON_SEND_STARTID
  *         ������       FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *         ���״̬       FIELD_SYSTEM_WIRELESS_CHANN_STATUS
  *����ֵ���ֶ��룺
  *         ����״̬       FIELD_SYSTEM_STATUS //������
  *         ����           FIELD_SERVICE_ON_SEND_STARTID
  #define   SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS 20046
  */
  int SetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**��ȡָ���豸�����߲��״̬
  *���������  
  *         ��˾��         FIELD_SYSTEM_COMPANY
  *         �ֳ���         FIELD_SYSTEM_FACTORY
  *         ������         FIELD_SERVICE_PLANTID
  *����ֵ���ֶ��룺
  *         �����         FIELD_SYSTEM_CHANN
  *         �Ƿ�����       FIELD_SYSTEM_ONLINE 
  *         �Ƿ�����       FIELD_SYSTEM_NORMAL
  *         ���ݸ���״̬   FIELD_SYSTEM_DATAUPDATE
  *         ��ص���       FIELD_SYSTEM_BATTERY_POWER
 
  #define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047
  */
  int GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);
  //��ȡ

  /*!
  *��������ֶ���
  *
  *����ֵ���ֶ��룺
  *          ���ݲɼ�ʱ�䣺       FIELD_SERVICE_TIME

  #define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
  */

  int GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /*!
  *��������ֶ���
  *
  *����ֵ���ֶ��룺
  *          ֪ͨ״̬��  FIELD_SYSTEM_STATUS
  #define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
  */
  int NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**��ͣ�������ݣ�����ָ���������ͣ���񶯲�����ݵ��м��
	*��������ֶ��룺
	*             ��˾ID               FIELD_SYSTEM_COMPANY
	*             �ֳ���               FIELD_SYSTEM_FACTORY
	*             ����ID��             FIELD_SERVICE_PLANTID
	*             ��ʼ����             FIELD_SERVICE_ON_SEND_STARTID
	*             ��ͣ������           FIELD_SERVICE_SUD_TYPE
	*             ��ͣ���¼�ID         FIELD_SYSTEM_EVENT_ID
	*             ָ�������ת��       FIELD_SERVICE_REV
	*             ָ�������ת��2      FIELD_SERVICE_REV2
	*             ָ�������ת��3      FIELD_SERVICE_REV3
	*             ָ�������ת��4      FIELD_SERVICE_REV4
	*             ָ�������ת��5      FIELD_SERVICE_REV5
	*             ����ֵ����           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             ͨ������             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             ��������             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             һ�鲨�ε���         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             ����Ƶ��             FIELD_SERVICE_SAMPLE_FREQ
	*             ��������             FIELD_SERVICE_SAMPLE_NUM
	*             �Զ����������BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             �Զ������ģ��ID     FIELD_SYSTEM_TEMPID
	*             ��Ӧ��ʱ��           FIELD_SERVICE_TIME
	*             ��Ӧʱ���΢��       FIELD_SERVICE_MICROSECOND
	*             �Ƿ���θ���������� FIELD_SERVICE_SEND_END
	*             ѹ����ʽ(0 ��ѹ��)   FIELD_SERVICE_DATA_COMPRESSION
	*����ֵ���ֶ��룺
	*             �ɹ���ʶ             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_VIB_DATA                         24210
	*/
   int Srv_OnReceiveSudVibData(ICommInterFace* pFixComm,IDBInterFace *pDB);

	/**��ͣ�������ݣ�����ָ���������ͣ����̬������ݵ��м��
	*��������ֶ��룺
	*             ��˾ID               FIELD_SYSTEM_COMPANY
	*             �ֳ���               FIELD_SYSTEM_FACTORY
	*             ����ID��             FIELD_SERVICE_PLANTID
	*             ��ʼ����             FIELD_SERVICE_ON_SEND_STARTID
	*             ��ͣ������           FIELD_SERVICE_SUD_TYPE
	*             ��ͣ���¼�ID         FIELD_SYSTEM_EVENT_ID
	*             ָ�������ת��       FIELD_SERVICE_REV
	*             ָ�������ת��2      FIELD_SERVICE_REV2
	*             ָ�������ת��3      FIELD_SERVICE_REV3
	*             ָ�������ת��4      FIELD_SERVICE_REV4
	*             ָ�������ת��5      FIELD_SERVICE_REV5
	*             ����ֵ����           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             ��̬������         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             ��������             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             һ�鲨�ε���         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             ����Ƶ��             FIELD_SERVICE_SAMPLE_FREQ
	*             ��������             FIELD_SERVICE_SAMPLE_NUM
	*             �Զ����������BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             �Զ������ģ��ID     FIELD_SYSTEM_TEMPID
	*             ��Ӧ��ʱ��           FIELD_SERVICE_TIME
	*             ��Ӧʱ���΢��       FIELD_SERVICE_MICROSECOND
	*             �Ƿ���θ���������� FIELD_SERVICE_SEND_END
	*             ѹ����ʽ(0 ��ѹ��)   FIELD_SERVICE_DATA_COMPRESSION
	*����ֵ���ֶ��룺
	*             �ɹ���ʶ             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_DYN_DATA                         24211
	*/ 
   int Srv_OnReceiveSudDynData(ICommInterFace* pFixComm,IDBInterFace *pDB);

   /**��ͣ�������ݣ�����ָ���������ͣ��������������ݵ��м��
	*��������ֶ��룺
	*             ��˾ID               FIELD_SYSTEM_COMPANY
	*             �ֳ���               FIELD_SYSTEM_FACTORY
	*             ����ID               FIELD_SERVICE_PLANTID
	*             ��ʼ����             FIELD_SERVICE_ON_SEND_STARTID
	*             ����������           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             �����¼�ID           FIELD_SYSTEM_EVENT_ID
	*             ��������             FIELD_SERVICE_SUD_TYPE
	*             ָ�������ת��       FIELD_SERVICE_REV
	*             ָ�������ת��2      FIELD_SERVICE_REV2
	*             ָ�������ת��3      FIELD_SERVICE_REV3
	*             ָ�������ת��4      FIELD_SERVICE_REV4
	*             ָ�������ת��5      FIELD_SERVICE_REV5
	*             ����������ֵ         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             ��Ӧ��ʱ��           FIELD_SERVICE_TIME
	*             ��Ӧʱ���΢��       FIELD_SERVICE_MICROSECOND
	*             ѹ����ʽ(0 ��ѹ��)   FIELD_SERVICE_DATA_COMPRESSION
	*����ֵ���ֶ��룺
	*             �ɹ���ʶ             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_PROC_DATA                        24212
	*/

   int Srv_OnReceiveSudProcData(ICommInterFace* pFixComm,IDBInterFace *pDB);


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
   int Srv_OnGetLogFileContent(ICommInterFace* pFixComm,IDBInterFace *pDB);

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
  int Srv_OnGetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);
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
  int Srv_OnSetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);

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

  int Srv_OnGetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

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

  int Srv_OnSetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

#endif //AFX_PROCESS_DSE_MARBLE
