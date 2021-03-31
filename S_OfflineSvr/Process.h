#if !defined(AFX_PROCESS)
#define AFX_PROCESS
#include "additions/BHOleDbWrapper.h"
//���庯��

/**�õ���ǰʱ��������һ���������͵��������ݣ�ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*@param��  ��������ֶ���
*          ��˾ID       FIELD_SYSTEM_COMPANY
*		   �ֳ���       FIELD_SYSTEM_FACTORY
*          ����ID       FIELD_SERVICE_PLANTID
*          ��������     FIELD_SERVICE_OFF_TRENDTYPE
*          ����ֵ����   FIELD_SERVICE_TREND_CHARTYPE
*          ������ͣ�   FIELD_SERVICE_OFF_CHANNTYPE
*          ���buff     FIELD_SERVICE_CHANN_BUFF
*          ���buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*          ������     FIELD_SERVICE_CHANN_NUM
*@return  ����ֵ���ֶ��룺
*          �����������ֵBuff   FIELD_SERVICE_TREND_VALUE_BUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*define     SERVICE_SERVICE_OFF_GETCURTREND           21001
*/
int Service_OffGetCurTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ�����Ͳ��ָ��ʱ�̵�Ƶ������
  *���������
  *          ��˾ID       FIELD_SYSTEM_COMPANY
  *    		 �ֳ���       FIELD_SYSTEM_FACTORY
  *          ����ID       FIELD_SERVICE_PLANTID
  *          ���ID       FIELD_SERVICE_OFF_CHANN1
  *          �������ݸ��� FIELD_SERVICE_OFF_WAVE_NUM
  *          ����������   FIELD_SERVICE_OFF_FREQANALYSIS_LINES
  *          ʱ��         FIELD_SERVICE_OFF_TIME
  *����ֵ���ֶ��룺
  *          ��������        FIELD_SERVICE_OFF_WAVE
  *          Ƶ������        FIELD_SERVICE_OFF_FREQWAVE
  *          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
  *          Ƶ��DF          FIELD_SERVICE_OFF_FREQWAVE_DF
  *          Ƶ�����ݵ���    FIELD_SERVICE_OFF_FREQWAVE_NUM
  *          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
  *          ����ֵ          FIELD_SYSTEM_ALARM_LOW
  *          Σ��ֵ          FIELD_SYSTEM_ALARM_HIGH
  *          ��λ            FIELD_SERVICE_CHANN1_UNIT
  *          ������        FIELD_SYSTEM_CHANN_ALIAS
  *          ת��            FIELD_SERVICE_OFF_REV
  *define     SERVICE_SERVICE_OFF_GETSPECFREQWAVE       21002
  */
int Service_OffGetSpecFreqWave(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ��ʱ��Ρ�����ֵ���͡������������������ݣ�ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*��������ֶ��룺
*          ����ֵ����   FIELD_SERVICE_TREND_CHARTYPE
*          ������ͣ�   FIELD_SERVICE_OFF_CHANNTYPE
*          ��˾ID       FIELD_SYSTEM_COMPANY
*		   �ֳ���       FIELD_SYSTEM_FACTORY
*          ����ID       FIELD_SERVICE_PLANTID
*          ���buff     FIELD_SERVICE_CHANN_BUFF
*          ���buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*          ������     FIELD_SERVICE_CHANN_NUM
*          ��ʼʱ��     FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ��     FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          �����������ֵBuff   FIELD_SERVICE_TREND_VALUE_BUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*          ��㵥λ             FIELD_SERVICE_CHANN1_UNIT
*define     SERVICE_SERVICE_OFF_GETTREND      21003
*/
int Service_OffGetTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ�������š��񶯲��š���ʼʱ��ε���������ֵ��������
*��������ֶ��룺
*          ��˾ID      FIELD_SYSTEM_COMPANY
*	    		 �ֳ���      FIELD_SYSTEM_FACTORY
*          ����ID      FIELD_SERVICE_PLANTID
*          ������ͣ�  FIELD_SERVICE_CHANNTYPE
*          ���ID      FIELD_SERVICE_OFF_CHANN1
*          ��ʼʱ��    FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ��    FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*          ʱ��             FIELD_SERVICE_OFF_TIME
  #define    SERVICE_SERVICE_OFF_GETAVIBCHANNTREND      21004
  */
int Service_OffGetAVibChannTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ���������ID��ʱ��Ρ�ˮƽ���ID�ʹ�ֱ���ID������λ����ʷ���ݣ�ͨ��ѭ�����εõ��������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*��������ֶ��룺
*          ��˾ID   FIELD_SYSTEM_COMPANY
*   	   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID      FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          ��ʼʱ��    FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ��    FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          ˮƽ�����񶯲������  FIELD_SERVICE_AXESLOCATION_HOR
*          ��ֱ�����񶯲������  FIELD_SERVICE_AXESLOCATION_VER
*          ʱ��ֵ                FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND  21005
  */
int Service_OffGetAxisLocationTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ�������񶯲�������ֵ��Ϣ�������޵���Ϣ��ѭ�����εõ������������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*			     �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���һ          FIELD_SERVICE_OFF_CHANN1
*          ���һʱ��ֵ    FIELD_SERVICE_OFF_TIME
*          ��˾ID2         FIELD_SYSTEM_COMPANY2
*			     �ֳ���2         FIELD_SYSTEM_FACTORY2
*          ����ID2         FIELD_SERVICE_PLANTID2
*          ����          FIELD_SERVICE_OFF_CHANN2
*          ����ʱ��ֵ    FIELD_SERVICE_OFF_TIME2
*����ֵ���ֶ��룺
*          ��˾ID          FIELD_SYSTEM_COMPANY
*		   �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���ID          FIELD_SERVICE_CHANN1
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ת��            FIELD_SERVICE_OFF_REV
*          ����ֵ          FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ          FIELD_SYSTEM_ALARM_HIGH
*          ��λ            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO    21006
*/
int Srv_OffGetTwoChannDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����ʷʱ�̡������񶯲���ϸ��Ƶ�׷����Ͳ��Ρ�
  *�������������ID           FIELD_SERVICE_PLANTID
  *          ���             FIELD_SERVICE_OFF_CHANN1
  *          ��������         FIELD_SERVICE_ZOOM_LINES
  *          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
  *          ��ֹƵ��         FIELD_SERVICE_ZOOM_STOPFREQ
  *          ʱ��             FIELD_SERVICE_OFF_TIME
  *����ֵ���ֶ��룺
  *          ���Ĳ�������   FIELD_SERVICE_OFF_WAVE
  *          �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
  *          ����Ƶ������   FIELD_SERVICE_ZOOMSPECTRUM
  *          ��������         FIELD_SERVICE_ZOOM_LINES
  *          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
  *          �񶯲��Ƶ��DF   FIELD_SERVICE_FREQWAVE_DF
  *          ͨƵ����ֵ��     FIELD_SERVICE_TREND_OVERALLCHAR
  *          ״̬             FIELD_SYSTEM_STATUS
  *          ״̬����         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_OFF_GETZOOMSPEC             21007
  */
int Service_OffGetZoomSpec(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢����ʱ���һ�����Ĺ켣�Ĳ������ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          �������ݸ���         FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��                 FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ��ֱ����������                 FIELD_SERVICE_AXESTRACK_WAVE_VER
*          ��ֱ����Ĳ�������ѹ������ֽ��� FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          ��ֱ��������ֵ�ṹ��             FIELD_SERVICE_CHARVALUE_S2
*          ��ֱ�����㱨��ֵ               FIELD_SYSTEM_ALARM_LOW2
*          ��ֱ������Σ��ֵ               FIELD_SYSTEM_ALARM_HIGH2
*          ��ֱ�����㵥λ                 FIELD_SERVICE_CHANN2_UNIT
*          ��ֱ���������                 FIELD_SYSTEM_CHANN2_ALIAS
*          ˮƽ����������                 FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          ˮƽ����Ĳ�������ѹ������ֽ��� FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          ˮƽ��������ֵ�ṹ��             FIELD_SERVICE_CHARVALUE_S
*          ˮƽ�����㱨��ֵ               FIELD_SYSTEM_ALARM_LOW
*          ˮƽ������Σ��ֵ               FIELD_SYSTEM_ALARM_HIGH
*          ˮƽ�����㵥λ                 FIELD_SERVICE_CHANN1_UNIT
*          ˮƽ���������                 FIELD_SYSTEM_CHANN_ALIAS
*          ���Ĺ켣��һ���Ĳ������ݸ���     FIELD_SERVICE_AXESTRACK_WAVENUM
*          ָ���������ת��               FIELD_SERVICE_REV
*          ����Ƶ��                         FIELD_SERVICE_SAMPLE_FREQ
#define     SERVICE_SERVICE_OFF_GETAXESTRACK              21008
*/
int Service_OffGetAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);


/**�õ�ָ����˾���ֳ����豸���������񶯲�������ֵ��Ϣ�������޵���Ϣ������ѭ���������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*			     �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���һʱ��ֵ    FIELD_SERVICE_OFF_TIME
*          ���buff     FIELD_SERVICE_CHANN_BUFF
*          ���buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*          ������     FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ���ID          FIELD_SERVICE_CHANN1
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ת��            FIELD_SERVICE_OFF_REV
*          ����ֵ          FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ          FIELD_SYSTEM_ALARM_HIGH
*          ��λ            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO   21009
*/
int Srv_OffGetSpecPlantChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!�õ�ָ����㡢ʱ��Ļ���������Ϣ
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*			     �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ������ƣ�      FIELD_SERVICE_OFF_CHANN1
*          �¼�ID:         FIELD_SYSTEM_EVENT_ID
*          ����ʱ�䣺      FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ��ĺ���  FIELD_SERVICE_MICROSECOND
*����ֵ���ֶ���
*          ����ֵ��      FIELD_SERVICE_CHARVALUE_S
*          ��������Buff  FIELD_SERVICE_THRESHOLD_BUFF
*          ���޵ĸ���    FIELD_SERVICE_CHANN_NUM
*          ��������      FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO   21010
*/
int Srv_OffGetSpecVibChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);


/*!�õ�ָ����ֹʱ���ָ�������ָ���񶯲�����͵ı�����¼
*��������ֶ���
*          ��˾ID   FIELD_SYSTEM_COMPANY
*    			 �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID�� FIELD_SERVICE_PLANTID
*          ������� FIELD_SERVICE_CHANN1
*          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          ������ʼʱ��:   FIELD_SERVICE_OFF_TIMESTART
*          ��������ʱ��:   FIELD_SERVICE_OFF_TIMEEND
*          �������ͣ�      FIELD_SYSTEM_TYPE
*          �¼�ID          FIELD_SYSTEM_EVENT_ID
#define     SERVICE_OFF_GETVIBCHANNALARMLOG      21011
*/
int Srv_OffGetVibChannAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ��ʱ���ָ�����ı�����¼�Ĳ��κ͸�������ֵ�����ڶ������ѭ���õ�
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*          �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*          �¼�ID:         FIELD_SYSTEM_EVENT_ID
*          ����ʱ�䣺      FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ��ĺ���  FIELD_SERVICE_MICROSECOND
*          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
*          ����������      FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          ���ID��        FIELD_SERVICE_OFF_CHANN1
*          �������ͣ�      FIELD_SYSTEM_TYPE
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ���Σ�          FIELD_SERVICE_OFF_WAVE
*          Ƶ�ף�          FIELD_SERVICE_OFF_FREQWAVE
*          Ƶ��DFֵ��      FIELD_SERVICE_OFF_FREQWAVE_DF
*          ���ε�����      FIELD_SERVICE_OFF_WAVE_NUM
*          Ƶ�����ݵ���    FIELD_SERVICE_ZOOM_LINES
*          ת�٣�          FIELD_SERVICE_OFF_REV
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD     21012
*/
int Service_OffGetSpecAlarmTimeRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);


/*!�õ�ָ��ʱ�䷶Χ��ָ���������ͣ����¼
  *��������ֶ���
  *          ��˾ID   FIELD_SYSTEM_COMPANY
  *			 �ֳ���   FIELD_SYSTEM_FACTORY
  *          ����ID��    FIELD_SERVICE_PLANTID
  *          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
  *          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
  *����ֵ���ֶ��룺
  *          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  *          ��ͣ�����ͣ�  FIELD_SYSTEM_TYPE
  *          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
  *          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
  *define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD   21013
  */
int Service_OffGetSpecTimeRangeSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*���õ�ָ����ͣ���¼�IDָ�����顢ָ��������ͣ����ת�١������������ݵ�ʱ�̡���ֵ����λ���С�ͨ��ѭ���õ�����ת�٣�Ȼ�󣬷ֱ�ͨ��ת�ٻ򱣴��������ݵ�ʱ�̡�����źͲ���
  *��������ֶ���
  *          ��˾ID   FIELD_SYSTEM_COMPANY
  *			 �ֳ���   FIELD_SYSTEM_FACTORY
  *          ����ID��    FIELD_SERVICE_PLANTID
  *          ���ID��    FIELD_SERVICE_OFF_CHANN1
  *          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  *����ֵ���ֶ��룺
  *          ת��:       FIELD_SERVICE_OFF_REV
  *          ʱ��        FIELD_SERVICE_OFF_TIME
 *           ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
  *          ��λ��      FIELD_SERVICE_OFF_PHASE
  *          ��ֵ��      FIELD_SERVICE_OFF_AMP
  *          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD   21014
  */
int Service_OffGetSpecTimeChannSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*!�õ�ָ����ͣ���¼�ID��ָ����˾���ֳ������顢ָ������ָ��ת�ٵ���ͣ������ϸ���ݡ�
*��������ֶ���
*          ��˾ID        FIELD_SYSTEM_COMPANY
*		   �ֳ���        FIELD_SYSTEM_FACTORY
*          ����ID��      FIELD_SERVICE_PLANTID
*          ���ID��      FIELD_SERVICE_OFF_CHANN1
*          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
*          ת��:         FIELD_SERVICE_OFF_REV
*          �������ݸ���  FIELD_SERVICE_OFF_WAVE_NUM
*          ��������      FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          ��λ��      FIELD_SERVICE_OFF_PHASE
*          ��ֵ��      FIELD_SERVICE_OFF_AMP
*          ���Σ�      FIELD_SERVICE_OFF_WAVE
*          Ƶ�ף�      FIELD_SERVICE_OFF_FREQWAVE
*          Ƶ��DFֵ��  FIELD_SERVICE_OFF_FREQWAVE_DF
*          ���ε�����  FIELD_SERVICE_OFF_WAVE_NUM
*          Ƶ�׵�����  FIELD_SERVICE_ZOOM_LINES
*          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD    21015
  */
int Service_OffGetSpecChannRevSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*!�õ�ָ�����顢ָ������ָ��ʱ�䱣����ͣ������ϸ���ݡ�
*��������ֶ���
*          ����ID��    FIELD_SERVICE_PLANTID
*          ���ID��    FIELD_SERVICE_OFF_CHANN1
*  ��ͣ�����ݱ���ʱ��: FIELD_SERVICE_OFF_TIME
*��Ӧʱ���΢��        FIELD_SERVICE_MICROSECOND
*          ��������    FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*          ���ýӿ�״̬��FIELD_SYSTEM_STATUS
*����ֵ���ֶ��룺
*          ��λ��      FIELD_SERVICE_OFF_PHASE
*          ��ֵ��      FIELD_SERVICE_OFF_AMP
*          ���Σ�      FIELD_SERVICE_OFF_WAVE
*          Ƶ�ף�      FIELD_SERVICE_OFF_FREQWAVE
*          Ƶ��DFֵ��  FIELD_SERVICE_OFF_FREQWAVE_DF
*          ���ε�����  FIELD_SERVICE_OFF_WAVE_NUM
*          ת��:       FIELD_SERVICE_OFF_REV
*          ת��2��     FIELD_SERVICE_OFF_REV2
*          Ƶ�׵�����  FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD    21016
*/
int Service_offGetSpecChannSaveTimeSud(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢ʱ��Ρ��������񶯲�����������ֵ�������ݣ�ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*��������ֶ��룺
*          ��˾ID        FIELD_SYSTEM_COMPANY
*		   �ֳ���        FIELD_SYSTEM_FACTORY
*          ����ID        FIELD_SERVICE_PLANTID
*          ���buff      FIELD_SERVICE_CHANN_BUFF
*          ���buff����  FIELD_SERVICE_CHANN_BUFFSIZE
*          ������      FIELD_SERVICE_CHANN_NUM
*          ��ʼʱ��      FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ��      FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          �����������ֵBuff   FIELD_SERVICE_CHARVALUE_SBUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*          ת��Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETVIBCHANNSTREND         21017
*/
int Srv_OffGetVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ���ǰʱ�������һ���������͵��������ݡ�ָ����˾���ֳ������顢�������񶯲�����������ֵ�������ݣ�
*ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*��������ֶ��룺
*          ��������      FIELD_SERVICE_OFF_TRENDTYPE
*          ��˾ID        FIELD_SYSTEM_COMPANY
*		   �ֳ���        FIELD_SYSTEM_FACTORY
*          ����ID        FIELD_SERVICE_PLANTID
*          ���buff      FIELD_SERVICE_CHANN_BUFF
*          ���buff����  FIELD_SERVICE_CHANN_BUFFSIZE
*          ������      FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          �����������ֵBuff   FIELD_SERVICE_CHARVALUE_SBUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*          ת��Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND      21018
*/
int Srv_OffGetCurrVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ�������񶯲�����ʷ���Σ�ѭ�����εõ������������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*		   �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���һ          FIELD_SERVICE_OFF_CHANN1
*          ʱ��ֵ          FIELD_SERVICE_OFF_TIME
*          ��˾ID2         FIELD_SYSTEM_COMPANY2
*			     �ֳ���2         FIELD_SYSTEM_FACTORY2
*          ����ID2         FIELD_SERVICE_PLANTID2
*          ����          FIELD_SERVICE_OFF_CHANN2
*          ʱ��ֵ          FIELD_SERVICE_OFF_TIME2
*          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
*          ��������        FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*����ֵ���ֶ��룺
*          ���ID          FIELD_SERVICE_CHANN1
*           ��������        FIELD_SERVICE_OFF_WAVE
*          Ƶ������        FIELD_SERVICE_OFF_FREQWAVE
*          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
*          Ƶ��DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          Ƶ�����ݵ���    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ת��            FIELD_SERVICE_OFF_REV
*          ����ֵ                           FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ                           FIELD_SYSTEM_ALARM_HIGH
*          ��λ                             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDATA           21019
*/
int Srv_OffGetTwoChannData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ����豸�����������̲���ֵ��Ϣ�������޵���Ϣ������ѭ���������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*		   �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���һʱ��ֵ    FIELD_SERVICE_OFF_TIME
*          ���buff     FIELD_SERVICE_CHANN_BUFF
*          ���buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*          ������     FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ���ID          FIELD_SERVICE_CHANN1
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          ����ֵ          FIELD_SERVICE_TREND_PROCESS
*          ת��            FIELD_SERVICE_OFF_REV
*          ������          FIELD_SYSTEM_ALARM_LOW
*          Σ����          FIELD_SYSTEM_ALARM_HIGH
*          ��λ            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO   21020
*/
int Srv_OffGetSpecPlantProcChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!�õ�ָ����˾���ֳ������顢��������ָ����ͣ���¼����������ݡ�
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*		   �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ����ֵ����      FIELD_SERVICE_TREND_CHARTYPE
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*          ��ͣ���¼�ID    FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          �����������ֵBuff   FIELD_SERVICE_TREND_VALUE_BUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*         ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
*          ת��:                FIELD_SERVICE_OFF_REV
*          ת��2��              FIELD_SERVICE_OFF_REV2
#define    SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND       21021
*/
int Srv_OffGetSpecEventChannsSudTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!�õ�ָ����˾���ֳ������顢�¼���������㡢����ʱ�����ϸ����
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*          �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*          ��ͣ���¼�ID    FIELD_SYSTEM_EVENT_ID
*    ��ͣ�����ݱ���ʱ��    FIELD_SERVICE_OFF_TIME
*            ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
*����ֵ���ֶ��룺
*          ���ID               FIELD_SERVICE_CHANN1
*          ������             FIELD_SYSTEM_CHANN_ALIAS
*          �������ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*          ת��:                FIELD_SERVICE_OFF_REV
*          �ת�٣�           FIELD_SYSTEM_PLANT_REV
*          ��һ�ٽ�ת��         FIELD_SERVICE_REV
*          �ڶ��ٽ�ת��         FIELD_SERVICE_REV2
*          ������               FIELD_SYSTEM_ALARM_LOW
*          Σ����               FIELD_SYSTEM_ALARM_HIGH
*          ��λ                 FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO    21022
*/
int Srv_OffGetSpecEventTimeChannsSudDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ����豸���������񶯲��ı����޵���Ϣ������ѭ���������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*			     �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*����ֵ���ֶ��룺
*          ���ID          FIELD_SERVICE_CHANN1
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          ����ֵ          FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ          FIELD_SYSTEM_ALARM_HIGH
*          ��λ            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO       21023
*/
int Srv_OffGetSpecPlantChannInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ����豸���������񶯲��ı����޵���Ϣ������ѭ���������ݣ�
*���������
*          ��˾ID          FIELD_SYSTEM_COMPANY
*		   �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID          FIELD_SERVICE_PLANTID
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*          �¼�ID:         FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          �����������ֵBuff  FIELD_SERVICE_CHARVALUE_SBUFF
*          ��Ӧ��������Buff    FIELD_SERVICE_TYPE_BUFF
*          ������            FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ���΢��      FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND        21024
*/
int Srv_OffGetVibChannAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢���ͱ����¼�ID������λ�����ơ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ת��:         FIELD_SERVICE_OFF_REV
*          ˮƽ����ֵ      FIELD_SERVICE_AXESLOCATION_HOR
*          ˮƽ����ֱ����  FIELD_SERVICE_TREND_DC
*          ˮƽ�����϶��ѹ FIELD_SERVICE_TREND_GAP
*          ��ֱ����ֵ      FIELD_SERVICE_AXESLOCATION_VER
*          ��ֱ����ֱ����  FIELD_SERVICE_TREND_DC2
*          ��ֱ�����϶��ѹ FIELD_SERVICE_TREND_GAP2
*          ��������      FIELD_SYSTEM_TYPE
*          ʱ��         FIELD_SERVICE_OFF_TIME
*          ������ʾ     FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND       21025
*/
int Srv_OffGetAxesLocationAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ�������¼��ı������ݱ���Ŀ�ʼ��ֹʱ��
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			�ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ��ʼʱ�䣺    FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ�䣺    FIELD_SERVICE_OFF_TIMEEND
#define SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE             21026
*/
int Srv_OffGetAlarmDataTimeRange(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ������λ�ñ����¼���ʱ��ı�������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		    �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          ʱ��                 FIELD_SERVICE_OFF_TIME
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ת��:         FIELD_SERVICE_OFF_REV
*          ˮƽ����ֵ      FIELD_SERVICE_AXESLOCATION_HOR
*          ˮƽ����ֱ����  FIELD_SERVICE_TREND_DC
*          ˮƽ�����϶��ѹ FIELD_SERVICE_TREND_GAP
*          ��ֱ����ֵ      FIELD_SERVICE_AXESLOCATION_VER
*          ��ֱ����ֱ����  FIELD_SERVICE_TREND_DC2
*          ��ֱ�����϶��ѹ FIELD_SERVICE_TREND_GAP2
*          �������ʾ      FIELD_SYSTEM_TYPE
#define SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA            21027
*/
int Srv_OffGetAxesLocationAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢�񶯲�㡢�����¼�ID������ϸ�������ơ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���ID               FIELD_SERVICE_CHANN1
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ת��:               FIELD_SERVICE_OFF_REV
*          �������ֵ          FIELD_SERVICE_CHARVALUE_S
*          �������Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          ���޸���            FIELD_SERVICE_LEN
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ���΢��      FIELD_SERVICE_MICROSECOND
*          �¼�����            FIELD_SYSTEM_EVENT_TYPE
*          ��������            FIELD_SYSTEM_TYPE
*          �������ʾ          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND          21028
*/
int Srv_OffGetVibAlarmInfoTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢��λ�Ʋ�㡢�����¼�ID���������ơ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���ID               FIELD_SERVICE_CHANN1
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ת��:               FIELD_SERVICE_OFF_REV
*          �������ֵ          FIELD_SERVICE_TREND_AXISDISPLACE
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*          �������            FIELD_SERVICE_THRESHOLD
*          �������ʾ          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND      21029
*/
int Srv_OffGetAxisDisplaceAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢��������㡢�����¼�ID���������ơ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���ID               FIELD_SERVICE_CHANN1
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ת��:               FIELD_SERVICE_OFF_REV
*          ���ֵ              FIELD_SERVICE_TREND_PROCESS
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*          �������            FIELD_SERVICE_THRESHOLD
*          �������ʾ          FIELD_SYSTEM_STATUS
*          ��������            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETPROCALARMTREND        21030
*/
int Srv_OffGetProcAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**�õ�ָ����˾���ֳ������顢��������㡢�����¼�ID��ʱ������ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���ID               FIELD_SERVICE_CHANN1
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ת��:               FIELD_SERVICE_OFF_REV
*          ���ֵ              FIELD_SERVICE_TREND_PROCESS
#define     SERVICE_SERVICE_OFF_GETPROCALARMDATA        21031
*/
int Srv_OffGetProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢��㡢�����¼�ID�����Ʊ������ݡ�
  *���������
  *          ��˾ID   FIELD_SYSTEM_COMPANY
  *			     �ֳ���   FIELD_SYSTEM_FACTORY
  *          ����ID               FIELD_SERVICE_PLANTID
  *          ���ID               FIELD_SERVICE_CHANN1
  *          �¼�ID:              FIELD_SYSTEM_EVENT_ID
  *����ֵ���ֶ��룺
*          ����ֵBuff          FIELD_SERVICE_CHARVALUE_SBUFF
*          ����Buff����        FIELD_SERVICE_TIMEWAVE_NUM
*          �������Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          ���޸���           FIELD_SERVICE_LEN
*          ��������            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETTRENDALARMDATA        21032
*/
int Srv_OffGetTrendAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢��λ�Ʋ�㡢�����¼�ID��ʱ�����ϸ���ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ���ID               FIELD_SERVICE_CHANN1
*          �¼�ID:              FIELD_SYSTEM_EVENT_ID
*          ʱ��ֵ              FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ת��:               FIELD_SERVICE_OFF_REV
*          �������ֵ          FIELD_SERVICE_TREND_AXISDISPLACE
*          ֱ����              FIELD_SERVICE_TREND_DC
*          ��϶��ѹ            FIELD_SERVICE_TREND_GAP
*          ��������            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA    21033
*/
int Srv_OffGetAxisDisplaceAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**�õ�ָ��ʱ���ָ�����ı�����¼�ĸ�������ֵ�����ڶ������ѭ���õ�
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*			     �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ���buff        FIELD_SERVICE_CHANN_BUFF
*          ���buff����    FIELD_SERVICE_CHANN_BUFFSIZE
*          ������        FIELD_SERVICE_CHANN_NUM
*          �¼�ID:         FIELD_SYSTEM_EVENT_ID
*          ����ʱ�䣺      FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ��ĺ���  FIELD_SERVICE_MICROSECOND
*����ֵ���ֶ��룺
*          ���ID��        FIELD_SERVICE_OFF_CHANN1
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          �������ͣ�      FIELD_SYSTEM_TYPE
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ת�٣�          FIELD_SERVICE_OFF_REV
*          ������������    FIELD_SYSTEM_RESERVE_STRING
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE    21034
*/
int Srv_OffGetSpecAlarmRecordCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����ʷʱ�̡�����ʾ��ͼ���Ĳ��Ρ�
*���������
*�������ֵ����˾ID             FIELD_SYSTEM_COMPANY
*		     �ֳ���            FIELD_SYSTEM_FACTORY
*            ����ID             FIELD_SERVICE_PLANTID
*            ���buff           FIELD_SERVICE_CHANN_BUFF
*            ���buff����       FIELD_SERVICE_CHANN_BUFFSIZE
*            ������           FIELD_SERVICE_CHANN_NUM
*            ʾ��ͼ����         FIELD_SYSTEM_TYPE       P-V, P-a, P-t
*            ʱ��               FIELD_SERVICE_OFF_TIME
*            �������ݸ���      FIELD_SERVICE_OFF_WAVE_NUM
*����ֵ���ֶ��룺
*          ���Ĳ�������   FIELD_SERVICE_OFF_WAVE
*          �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
*          ����X������    FIELD_SERVICE_ZOOMSPECTRUM
*          ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*          ����ֵ           FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ           FIELD_SYSTEM_ALARM_HIGH
*          ��λ             FIELD_SERVICE_CHANN1_UNIT
*          ������         FIELD_SYSTEM_CHANN_ALIAS
*          ת��             FIELD_SERVICE_REV
*          ����Ƶ��        FIELD_SERVICE_SAMPLE_FREQ
#define     SERVICE_SERVICE_OFF_GETPVDATA                   21035
*/
int Srv_OffGetPVData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**�õ�ָ����ʷʱ�̡���������˳��������Ĳ��κ�Ƶ�ס�
*���������
*          ��˾ID           FIELD_SYSTEM_COMPANY
*			     �ֳ���           FIELD_SYSTEM_FACTORY
*          ����ID           FIELD_SERVICE_PLANTID
*          ���             FIELD_SERVICE_OFF_CHANN1
*          ʱ��             FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ���Ĳ�������   FIELD_SERVICE_OFF_WAVE
*          Ƶ������         FIELD_SERVICE_OFF_FREQWAVE
*          �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
*          Ƶ��DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          Ƶ�����ݵ���     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*          ����ֵ           FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ           FIELD_SYSTEM_ALARM_HIGH
*          ��λ             FIELD_SERVICE_CHANN1_UNIT
*          ������         FIELD_SYSTEM_CHANN_ALIAS
*          ת��           FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036
*/
int Srv_OffGetRodSinkData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**��ȡָ�������̬ѹ����㡢�����˳�������㡢�񶯲�㡣��ȡ��ָ��ʱ�䲨�κ���������
*������Ϊ�վͲ��᷵�ض�Ӧ������
*�������ֵ��
*            ��˾ID                     FIELD_SYSTEM_COMPANY
*			 �ֳ���                     FIELD_SYSTEM_FACTORY
*            ����ID                     FIELD_SERVICE_PLANTID
*            �񶯲��buff               FIELD_SERVICE_CHANN_BUFF
*            �񶯲��buff����           FIELD_SERVICE_CHANN_BUFFSIZE
*            �񶯲�����               FIELD_SERVICE_CHANN_NUM
*            ��̬���buff               FIELD_SERVICE_CHANN_BUFF2
*            ��̬���buff����           FIELD_SERVICE_CHANN_BUFFSIZE2
*            ��̬������               FIELD_SERVICE_CHANN_NUM2
*            �����˳��������buff       FIELD_SERVICE_CHANN_BUFF3
*            �����˳��������buff����   FIELD_SERVICE_CHANN_BUFFSIZE3
*            �����˳�����������       FIELD_SERVICE_CHANN_NUM3
*            ʱ��                       FIELD_SERVICE_OFF_TIME
*            �������ݸ���               FIELD_SERVICE_OFF_WAVE_NUM
*�����ֶΣ�
*            �������ݵĵ���               FIELD_SERVICE_TIMEWAVE_NUM
*            ����Ƶ��                     FIELD_SERVICE_SAMPLE_FREQ
*            �������                     FIELD_SERVICE_CHANN1
*            ��㲨��                     FIELD_SERVICE_TIMEWAVE
*            �������ֵ                   FIELD_SERVICE_CHARVALUE_S
*            ������                       FIELD_SERVICE_FREQWAVE2
*            ת��                         FIELD_SERVICE_OFF_REV
*            ������������λ��             FIELD_SERVICE_ANGLEPARAM1
*            �������ر���λ��             FIELD_SERVICE_ANGLEPARAM2
*            ������������λ��             FIELD_SERVICE_ANGLEPARAM3
*            �������ر���λ��             FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA           21037
*/
int Srv_OffGetRcMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**��ʷ��òͼ���õ�ָ��ʱ�䡢ָ���������в������
*�������ֵ��
*            ��˾ID         FIELD_SYSTEM_COMPANY
*	         �ֳ���         FIELD_SYSTEM_FACTORY
*            ����ֵ����     FIELD_SERVICE_TREND_CHARTYPE
*            �豸��buff     FIELD_SERVICE_CHANN_BUFF
*            �豸��buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*            �豸�Ÿ���     FIELD_SERVICE_CHANN_NUM
*            ʱ��ֵ         FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*            �豸ID         FIELD_SERVICE_PLANTID
*            ���ID         FIELD_SERVICE_CHANN1
*            ���ֵ         FIELD_SERVICE_CHANN1VALUE
*            ��㵥λ       FIELD_SERVICE_CHANN1_UNIT
*            ��������б�   FIELD_SERVICE_CHANN1_LIST
*            ����״̬     FIELD_SERVICE_CHANN1_STATUS
*            �������ֵ     FIELD_SERVICE_CHARVALUE_S
*            �������       FIELD_SERVICE_CHANNTYPE
*            ת��1          FIELD_SERVICE_OFF_REV
#define     SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE    21038
*/
int Srv_OffGetSpecTimeAllChannValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢���,ָ����ͣ���¼���ʱ���һ���ᴿ���Ĺ켣�Ĳ������ݡ�
*���������
*          ��˾ID               FIELD_SYSTEM_COMPANY
*          �ֳ���               FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          �������ݸ���         FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��                 FIELD_SERVICE_OFF_TIME
*          ��ͣ���¼�ID         FIELD_SYSTEM_EVENT_ID
*          ת��                 FIELD_SERVICE_OFF_REV
*����ֵ���ֶ��룺
*          ��ֱ���������������Ƶ��       FIELD_SERVICE_VERFREQ
*          ��ֱ��������������ķ�ֵ       FIELD_SERVICE_VERAMP
*          ��ֱ�����������������λ       FIELD_SERVICE_VERPHASE
*          ˮƽ���������������Ƶ��       FIELD_SERVICE_HORFREQ
*          ˮƽ��������������ķ�ֵ       FIELD_SERVICE_HORAMP
*          ˮƽ�����������������λ       FIELD_SERVICE_HORPHASE
*          ת������                       FIELD_SYSTEM_TYPE //0:Ϊ��ʱ��,1Ϊ˳ʱ��
*          ��������                       FIELD_SYSTEM_STATUS//1:Ϊ������,2Ϊ������
*          ת��                           FIELD_SERVICE_OFF_REV
*          ˮƽ��������ֵ�ṹ��           FIELD_SERVICE_CHARVALUE_S
*          ��ֱ��������ֵ�ṹ��           FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK           21039
*/
int Srv_OffGetSudPurifyAxesTrack( ICommInterFace *pFixComm, IDBInterFace *pDB );


/**����ʽϵͳ����������������ܣ���ȡ��ָ������ʱ��Ĳ��κ�������������ȡָ�������̬ѹ����㡢�����˳�������㡢�񶯲�㡣������Ϊ�վͲ��᷵�ض�Ӧ�����ݡ�
*�������ֵ��
*            ��˾ID                     FIELD_SYSTEM_COMPANY
*			 �ֳ���                     FIELD_SYSTEM_FACTORY
*            ����ID                     FIELD_SERVICE_PLANTID
*            �񶯲��buff               FIELD_SERVICE_CHANN_BUFF
*            �񶯲��buff����           FIELD_SERVICE_CHANN_BUFFSIZE
*            �񶯲�����               FIELD_SERVICE_CHANN_NUM
*            ��̬���buff               FIELD_SERVICE_CHANN_BUFF2
*            ��̬���buff����           FIELD_SERVICE_CHANN_BUFFSIZE2
*            ��̬������               FIELD_SERVICE_CHANN_NUM2
*            ����ʱ��ֵ                 FIELD_SERVICE_OFF_TIME
*�����ֶΣ�
*            �������ݵĵ���             FIELD_SERVICE_TIMEWAVE_NUM
*            ����Ƶ��                   FIELD_SERVICE_SAMPLE_FREQ
*            �������                   FIELD_SERVICE_CHANN1
*            ��㲨��                   FIELD_SERVICE_TIMEWAVE1
*            �������ֵ                 FIELD_SERVICE_CHARVALUE_S
*            ������                     FIELD_SERVICE_FREQWAVE2
#define   SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA       21041
*/
int Srv_OffGetSpecTimeAlarmMultiParamData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ʽϵͳ����Ӧ�����������ܣ��õ�ָ������ָ������ʱ�䡢ָ���׺ŵ�Ӧ�����κ��񶯲�㲨��
*���������
*            ��˾ID        ������    FIELD_SYSTEM_COMPANY
*		     �ֳ���          ������  FIELD_SYSTEM_FACTORY
*            ����ID          ������  FIELD_SERVICE_PLANTID
*            ���׺�                  FIELD_SERVICE_CYLINDERNO
*            ����ʱ��                FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*            ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE1
*            ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE2
*            ������Ӧ���������ݡ�����FIELD_SERVICE_FREQWAVE1
*            �������ݸ���   �������� FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE       21040
*/
int Srv_OffGetSpecTimeAlarmDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);


/*!�õ���ʱ����ָ�����顢ָ������ָ��ʱ�䱣����ͣ������ϸ���ݡ�
*��������ֶ���
*          ����ID��    FIELD_SERVICE_PLANTID
*          ���ID��    FIELD_SERVICE_OFF_CHANN1
*  ��ͣ�����ݱ���ʱ��: FIELD_SERVICE_OFF_TIME
*��Ӧʱ���΢��        FIELD_SERVICE_MICROSECOND
*          ��������    FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*����ֵ���ֶ��룺
*          ��λ��      FIELD_SERVICE_OFF_PHASE
*          ��ֵ��      FIELD_SERVICE_OFF_AMP
*          ���Σ�      FIELD_SERVICE_OFF_WAVE
*          Ƶ�ף�      FIELD_SERVICE_OFF_FREQWAVE
*          Ƶ��DFֵ��  FIELD_SERVICE_OFF_FREQWAVE_DF
*          ���ε�����  FIELD_SERVICE_OFF_WAVE_NUM
*          ת��:       FIELD_SERVICE_OFF_REV
*          ת��2��     FIELD_SERVICE_OFF_REV2
*          Ƶ�׵�����  FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX    21042
*/
int Service_offGetSpecChannSaveTimeSudEx(ICommInterFace * pFixComm,IDBInterFace *pDB);


/*���õ���ʱ���߻���ָ����ͣ���¼�IDָ�����顢ָ��������ͣ����ת�١������������ݵ�ʱ�̡���ֵ����λ���С�ͨ��ѭ���õ�����ת�٣�Ȼ�󣬷ֱ�ͨ��ת�ٻ򱣴��������ݵ�ʱ�̡�����źͲ���
  *��������ֶ���
  *          ��˾ID   FIELD_SYSTEM_COMPANY
  *			 �ֳ���   FIELD_SYSTEM_FACTORY
  *          ����ID��    FIELD_SERVICE_PLANTID
  *          ���ID��    FIELD_SERVICE_OFF_CHANN1
  *          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  *����ֵ���ֶ��룺
  *          ת��:       FIELD_SERVICE_OFF_REV
  *          ʱ��        FIELD_SERVICE_OFF_TIME
 *           ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
  *          ��λ��      FIELD_SERVICE_OFF_PHASE
  *          ��ֵ��      FIELD_SERVICE_OFF_AMP
  *          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX   21043
  */
int Service_OffGetSpecTimeChannSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB);


/*!�õ�ָ����ͣ���¼�ID��ָ����˾���ֳ����顢ָ�����ָ��ʱ��ε���ͣ����ת�١������������ݵ�ʱ�̡���ֵ����λ���С�ͨ��ѭ���õ�����ת�٣�Ȼ�󣬷�   *��ͨ��ת�ٻ򱣴��������ݵ�ʱ�̡�����źͲ���
  *��������ֶ���
  *          ��˾ID      FIELD_SYSTEM_COMPANY
  *			 �ֳ���      FIELD_SYSTEM_FACTORY
  *          ����ID��    FIELD_SERVICE_PLANTID
  *          ���ID��    FIELD_SERVICE_OFF_CHANN1
  *          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  *          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
  *          ����ʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
  *����ֵ���ֶ��룺
  *          ת��:       FIELD_SERVICE_OFF_REV
  *          ʱ��        FIELD_SERVICE_OFF_TIME
  *��Ӧʱ���΢��        FIELD_SERVICE_MICROSECOND
  *          ��λ��      FIELD_SERVICE_OFF_PHASE
  *          ��ֵ��      FIELD_SERVICE_OFF_AMP
  *          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
  
  #define    SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME          21044
*/
int Srv_OffGetSpecChannSudByTime (ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ��ʱ����¡���˾���ֳ�������������ĸ�������ֵ���ֵ��Сֵƽ��ֵ���������ֵbuff�����������ֵ����Сֵ��ƽ��ֵ������ֵ����Ӧ����3
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*����ֵ���ֶΣ�
*          �������ֵBuff      FIELD_SERVICE_CHARVALUE_SBUFF
*          ����ֵ����          FIELD_SERVICE_CHANN_NUM
*          ���ID               FIELD_SERVICE_CHANN1
#define SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT        21051
*/
int Srv_OffGetSpecPlantMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ���ָ��ʱ�䷶Χ�ı�����Ϣ����ͣ����Ϣ��
*��ͨ������������������ͨ���������ۼ�ͣ���������ۼ�ͣ��ʱ�䡢�ۼ�����ʱ�䣬ʱ�䵥λΪСʱ
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ��ʼʱ�䣺    FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ�䣺    FIELD_SERVICE_OFF_TIMEEND
*���ص��ֶΣ�
*          ����ID               FIELD_SERVICE_PLANTID
*          ���в�㱨������     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*          �ۼ�ͣ������         FIELD_SERVICE_LEN
*          �ۼ�ͣ��ʱ��(Сʱ)   FIELD_SERVICE_SAMPLE_FREQ
*          �ۼ�����ʱ��(Сʱ)   FIELD_SERVICE_SAMPLE_NUM
#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052
*/
int Srv_OffGetSpecTimeRangeFactoryRunInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**������ϡ��������ID�Ŀ�ʼ��ֹʱ�䷶Χ�ڣ����صļ�¼ֻ��һ����0��ʾû�У�1��ʾ�С�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*			     �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID   FIELD_SERVICE_PLANTID
*          ���ID   FIELD_SERVICE_CHANN1
*          �¼�ID   FIELD_SYSTEM_EVENT_ID
*����ֵ���ֶ��룺
*          ͬת�ٲ���Ƿ񱨾�               FIELD_SERVICE_OFF_REV
*          ͬת�ٲ���Ƿ����               FIELD_SERVICE_OFF_REV2
*          ͬת�ٲ���Ƿ���ڲ����ͨƵ���� FIELD_SERVICE_TREND_RESCHAR
*          ��Ӧ����Ƿ񱨾�          FIELD_SERVICE_REV
*          ��Ӧ����Ƿ����          FIELD_SERVICE_REV2
*          ��Ӧ����Ƿ���ڲ����ͨƵ����   FIELD_SERVICE_TREND_DC
#define SERVICE_SERVICE_OFF_CORRDIGN                21053
*/
int Srv_OffCorrDign(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���ż���ʷ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*		     �豸��           FIELD_SERVICE_PLANT_NUM
*		     �����豸��       FIELD_SERVICE_ALARM_PLANTNUM
#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT             21054
*/
int Srv_OffGetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**��˾����ʷ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*            �ֳ���           FIELD_SYSTEM_FACTORY
*		     �豸��           FIELD_SERVICE_PLANTID
*		     ��������         FIELD_SERVICE_ALARM_ PLANTNUM
*		     ͣ������         FIELD_SERVICE_STOP_ PLANTNUM
#define    SERVICE_SERVICE_OFF_COMPANYALARMSTAT         21055
*/
int Srv_OffGetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���վ����ʷ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		        �ֳ���           FIELD_SYSTEM_FACTORY
*	            �豸��           FIELD_SERVICE_PLANTID
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*		        ����ID          FIELD_SERVICE_ ALARM ID
*		        �����         FIELD_SERVICE_CHANN1
*		        ���λ��         FIELD_SYSTEM_CHANN_ALIAS
*		        ����ֵ           FIELD_SERVICE_TREND_VALUE
*		        ת��1            FIELD_SERVICE_REV
*		      ������ʼʱ��       FIELD_SERVICE_OFF_TIMESTART// ����FIELD_SERVICE_ALARM_TIMESTART
*		      ��������ʱ��       FIELD_SERVICE_OFF_TIMEEND
*		        ��������         FIELD_SERVICE_ALARM_TYPE
*                �ɹ���ʾ         FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21056
*/
int Srv_OffGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���վ����ʷ����ͳ��
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		        �ֳ���           FIELD_SYSTEM_FACTORY
*	            �豸��           FIELD_SERVICE_PLANTID
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*		        ����ID          FIELD_SERVICE_ ALARM ID
*		        �����         FIELD_SERVICE_CHANN1
*		        ���λ��         FIELD_SYSTEM_CHANN_ALIAS
*		      ������ʼʱ��       FIELD_SERVICE_OFF_TIMESTART// ����FIELD_SERVICE_ALARM_TIMESTART
*		      ��������ʱ��       FIELD_SERVICE_OFF_TIMEEND
*		        ��������         FIELD_SERVICE_ALARM_TYPE
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21088
*/
int Srv_OffGetMonitorAlarmStatEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**��ͨ��������ֵ������ѯ���õ�ָ���������ĸ�����������ֵ
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID           FIELD_SERVICE_PLANTID
*		     �����         FIELD_SERVICE_CHANN1
*		     ����ID           FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺
*            �����������ֵBuff   FIELD_SERVICE_TREND_VALUE_BUFF
*		     ������ʼʱ��     FIELD_SERVICE_OFF_TIMESTART
*		     ��������ʱ��     FIELD_SERVICE_OFF_TIMEEND
*            ʱ��             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES   21057
*/
int Srv_OffGetSpecChannAlarmMultCharValues(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**��ͨ��������ֵ������ѯ���õ�ָ�������������ָ����������ֵ
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID           FIELD_SERVICE_PLANTID
*            �������buff     FIELD_SERVICE_CHANN_BUFF
*            �������buff���� FIELD_SERVICE_CHANN_BUFFSIZE
*            ����������     FIELD_SERVICE_CHANN_NUM
*            ����ֵ����       FIELD_SERVICE_TREND_CHARTYPE
*		     ����ID           FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺
*            �������ֵ       FIELD_SERVICE_TREND_VALUE
*            ���ID           FIELD_SERVICE_CHANN1
*		     ������ʼʱ��     FIELD_SERVICE_OFF_TIMESTART
*		     ��������ʱ��     FIELD_SERVICE_OFF_TIMEEND
*            ʱ��             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE   21058
*/
int Srv_OffGetSpecChannsAlarmSingleCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ������ָ��һ����ָ��ʱ��Ļ���������λ�ù켣
*���������
*          ��˾ID            FIELD_SYSTEM_COMPANY
*		   �ֳ���            FIELD_SYSTEM_FACTORY
*          ����ID            FIELD_SERVICE_PLANTID
*          ���λ������      FIELD_SYSTEM_AXISLOCATION
*          ˮƽ������ID    FIELD_SERVICE_CHANNID_HOR
*          ��ֱ������ID    FIELD_SERVICE_CHANNID_VER
*          �������ݸ���         FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��ֵ          ��FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*         X����������    FIELD_SERVICE_AXESTRACK_WAVE_HOR
*         Y����������    FIELD_SERVICE_AXESTRACK_WAVE_VER
*         ʵ�ʲ��ε���        FIELD_SERVICE_AXESTRACK_WAVENUM
*         X����������ֵ  FIELD_SERVICE_CHARVALUE_S
*         Y����������ֵ  FIELD_SERVICE_CHARVALUE_S2
*         ת��            FIELD_SERVICE_OFF_REV
#define   SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION            21059
*/
int Srv_OffGetSpecChannRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�����ʽ��е�豸����״̬��ʷ����
*��ȡ���и��ֲ�������ֵ��ͨ��ѭ������ȡ�ø������ĵ�һ������ֵ��
*�������ֵ����˾ID               FIELD_SYSTEM_COMPANY
*		     �ֳ���               FIELD_SYSTEM_FACTORY
*            ����ID               FIELD_SERVICE_PLANTID
*            �񶯲��buff         FIELD_SERVICE_CHANN_BUFF
*            �񶯲��buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*            �񶯲�����         FIELD_SERVICE_CHANN_NUM
*            ��̬���buff         FIELD_SERVICE_CHANN_BUFF2
*            ��̬���buff����     FIELD_SERVICE_CHANN_BUFFSIZE2
*            ��̬������         FIELD_SERVICE_CHANN_NUM2
*            ���������buff       FIELD_SERVICE_CHANN_BUFF3
*            ���������buff����   FIELD_SERVICE_CHANN_BUFFSIZE3
*            ������������       FIELD_SERVICE_CHANN_NUM3
*            ��ʼʱ��             FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ��             FIELD_SERVICE_OFF_TIMEEND
*            ������������ֵ���� FIELD_SERVICE_TREND_CHARTYPE
*            ����������ֵ����   FIELD_SERVICE_CHANNTYPE
*            ��̬ѹ������ֵ����   FIELD_SYSTEM_EVENT_TYPE
*            �����˳�������ֵ���� FIELD_SERVICE_OFF_TRENDTYPE
*����ֵ���ֶ��룺
*            �������ֵBuff       FIELD_SERVICE_TREND_VALUE_BUFF
*            ָ���������ת��   FIELD_SERVICE_REV_BUFF
*            ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*            ������ͱ�ʶ         FIELD_SYSTEM_TYPE //��:GE_VIBCHANN, ��̬ѹ����GE_DYNPRESSCHANN, ������:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060
*/
int Srv_OffGetSpecChannsRunStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�����ʽ��е�豸����״̬��ʷ����
*��ȡ���и��ֲ�������ֵ��ͨ��ѭ������ȡ�ø������ĵ�һ������ֵ��
*�������ֵ����˾ID               FIELD_SYSTEM_COMPANY
*		     �ֳ���               FIELD_SYSTEM_FACTORY
*            ����ID               FIELD_SERVICE_PLANTID
*            �񶯲��buff         FIELD_SERVICE_CHANN_BUFF
*            �񶯲��buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*            �񶯲�����         FIELD_SERVICE_CHANN_NUM
*            ��̬���buff         FIELD_SERVICE_CHANN_BUFF2
*            ��̬���buff����     FIELD_SERVICE_CHANN_BUFFSIZE2
*            ��̬������         FIELD_SERVICE_CHANN_NUM2
*            ���������buff       FIELD_SERVICE_CHANN_BUFF3
*            ���������buff����   FIELD_SERVICE_CHANN_BUFFSIZE3
*            ������������       FIELD_SERVICE_CHANN_NUM3
*            ��ʼʱ��             FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ��             FIELD_SERVICE_OFF_TIMEEND
*            ������������ֵ���� FIELD_SERVICE_TREND_CHARTYPE
*            ����������ֵ����   FIELD_SERVICE_CHANNTYPE
*            ��̬ѹ������ֵ����   FIELD_SYSTEM_EVENT_TYPE
*            �����˳�������ֵ���� FIELD_SERVICE_OFF_TRENDTYPE
*����ֵ���ֶ��룺
*            �������ֵBuff       FIELD_SERVICE_TREND_VALUE_BUFF
*            ָ���������ת��   FIELD_SERVICE_REV_BUFF
*            ʱ��ֵ               FIELD_SERVICE_OFF_TIME
*            ������ͱ�ʶ         FIELD_SYSTEM_TYPE //��:GE_VIBCHANN, ��̬ѹ����GE_DYNPRESSCHANN, ������:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX                   21082
*/
int Srv_OffGetSpecChannsRunStatusEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�����ʽ��е�豸��ʷ����ֵ�Ͳ����������ƺᡢ����Ƚϣ�
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID          FIELD_SERVICE_PLANTID
*            ���buff         FIELD_SERVICE_CHANN_BUFF
*            ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*            ������         FIELD_SERVICE_CHANN_NUM
*            �������         FIELD_SERVICE_OFF_CHANNTYPE
*            ��ʼʱ��         FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ��         FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*            �������ֵBuff   FIELD_SERVICE_TREND_VALUE_BUFF
*            ת��             FIELD_SERVICE_REV
*            ʱ��ֵ           FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETHISCOMPARE         21061
*/
int Srv_OffGetSpecChannsHisCompare(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�����ʽ��е�豸����ͨ��ָ��ʱ�������ֵ�Ͳ������ݱȽ�
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID          FIELD_SERVICE_PLANTID
*            ���buff         FIELD_SERVICE_CHANN_BUFF
*            ���buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*            ������ͣ�       FIELD_SERVICE_CHANNTYPE
*            ������         FIELD_SERVICE_CHANN_NUM
*            ʱ��             FIELD_SERVICE_OFF_TIME
*            �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
*����ֵ���ֶ��룺
*            ���Ĳ�������   FIELD_SERVICE_OFF_WAVE
*            �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
*            ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*            ת��             FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE         21062
*/
int Srv_OffGetSpecChannsCharValueWave(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**�õ�����ʽ��е�豸��ͨ���Ͷ�̬ͨ��ָ��ʱ�������ֵ
*���������
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID           FIELD_SERVICE_PLANTID
*            �񶯲��buff         FIELD_SERVICE_CHANN_BUFF
*            �񶯲��buff����     FIELD_SERVICE_CHANN_BUFFSIZE
*            �񶯲�����         FIELD_SERVICE_CHANN_NUM
*            ��̬���buff         FIELD_SERVICE_CHANN_BUFF2
*            ��̬���buff����     FIELD_SERVICE_CHANN_BUFFSIZE2
*            ��̬������         FIELD_SERVICE_CHANN_NUM2
*            ������ֵʱ��     FIELD_SERVICE_TIME
*            ��̬����ֵʱ��     FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*            ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*            ת��             FIELD_SERVICE_OFF_REV
*            ������ͱ�ʶ         FIELD_SYSTEM_TYPE //��:GE_VIBCHANN, ��̬ѹ����GE_DYNPRESSCHANN
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063
*/
int Srv_OffGetSpecChannsCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�����ʽ��е�豸�����±���
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID           FIELD_SERVICE_PLANTID
*            ʱ��ֵ           FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*            �����           FIELD_SERVICE_CHANN1
*            ����ֵ���ֵ     FIELD_SERVICE_CHANN1VALUE
*            ����ֵ��Сֵ     FIELD_SERVICE_CHANN2VALUE
*            ����ֵƽ��ֵ     FIELD_SERVICE_TREND_VALUE
#define     SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT       21064
*/
int Srv_OffGetSpecRecipMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���վ����ʷ����ͳ�ƣ��Ż���ȡ���ݣ���ȡ20����¼�Ժ������ֵ��ת��
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*		     �����         FIELD_SERVICE_CHANN1
*		     ����ID           FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺
*		     ����ֵ           FIELD_SERVICE_TREND_VALUE
*		     ת��1            FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065
*/
int Srv_OffGetMonitorAlarmStatChar(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢����ʱ���һ���ᴿ���Ĺ켣�Ĳ������ݡ�
*���������
*          ��˾ID   FIELD_SYSTEM_COMPANY
*		   �ֳ���   FIELD_SYSTEM_FACTORY
*          ����ID               FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
*          ���λ������         FIELD_SYSTEM_AXISLOCATION
*          �������ݸ���         FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��                 FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ��ֱ���������������Ƶ��       FIELD_SERVICE_VERFREQ
*          ��ֱ��������������ķ�ֵ       FIELD_SERVICE_VERAMP
*          ��ֱ�����������������λ       FIELD_SERVICE_VERPHASE
*          ˮƽ���������������Ƶ��       FIELD_SERVICE_HORFREQ
*          ˮƽ��������������ķ�ֵ       FIELD_SERVICE_HORAMP
*          ˮƽ�����������������λ       FIELD_SERVICE_HORPHASE
#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK           21066
*/
int Srv_OffGetPurifyAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ�����Ͳ��ָ��ʱ�̵ĵ�������
*���������
*          ��˾ID       FIELD_SYSTEM_COMPANY
*	      �ֳ���       FIELD_SYSTEM_FACTORY
*          ����ID       FIELD_SERVICE_PLANTID
*          ���ID       FIELD_SERVICE_OFF_CHANN1
*          �������ݸ��� FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��         FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ��������        FIELD_SERVICE_OFF_WAVE
*          ��������        FIELD_SERVICE_OFF_FREQWAVE
*          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
*          ����DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          �������ݵ���    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ����ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*          ����ֵ          FIELD_SYSTEM_ALARM_LOW
*          Σ��ֵ          FIELD_SYSTEM_ALARM_HIGH
*          ��λ            FIELD_SERVICE_CHANN1_UNIT
*          ������        FIELD_SYSTEM_CHANN_ALIAS
*          ת��            FIELD_SERVICE_OFF_REV
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM1
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM2
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM3
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM       21067
*/
int Service_OffGetSpecChannCepstrum(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**������е������ѯ���õ�ָ������ID���ȱ����Ĳ����������ֵ����
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*            �ֳ���           FIELD_SYSTEM_FACTORY
*            ����ID           FIELD_SERVICE_PLANTID
*            ����ID           FIELD_SERVICE_ALARMID
*����ֵ���ֶ��룺
*            ���������              FIELD_SERVICE_CHANN1
*            ������������ֵ�ṹ��  FIELD_SERVICE_CHARVALUE_S
*            ʱ��                    FIELD_SERVICE_TIME
*            ��Ӧʱ���΢��          FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068
*/
int Srv_OffGetSpecAlarmIDCharValues(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**������е������ѯ,�õ�ָ��ʱ���ָ�������˳�����̬ѹ�����ı�����¼
*��������ֶ���
*          ��˾ID          FIELD_SYSTEM_COMPANY
*          �ֳ���          FIELD_SYSTEM_FACTORY
*          ����ID��        FIELD_SERVICE_PLANTID
*          ���            FIELD_SERVICE_OFF_CHANN1
*          �¼�ID:         FIELD_SYSTEM_EVENT_ID
*          ����ʱ�䣺      FIELD_SERVICE_OFF_TIME
*          ��Ӧʱ��ĺ���  FIELD_SERVICE_MICROSECOND
*          �������ݸ���    FIELD_SERVICE_OFF_WAVE_NUM
*����ֵ���ֶ��룺
*          ���Ĳ�������   FIELD_SERVICE_OFF_WAVE
*          Ƶ������         FIELD_SERVICE_OFF_FREQWAVE
*          �������ݸ���     FIELD_SERVICE_OFF_WAVE_NUM
*          Ƶ��DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          Ƶ�����ݵ���     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          ����X������    FIELD_SERVICE_ZOOMSPECTRUM
*          ����ֵ�ṹ��     FIELD_SERVICE_CHARVALUE_S
*          ת��             FIELD_SERVICE_REV
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM1
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM2
*          ������������λ��                      FIELD_SERVICE_ANGLEPARAM3
*          �������ر���λ��                      FIELD_SERVICE_ANGLEPARAM4
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD     21069
*/
int Srv_OffGetSpecAlarmTimeDynRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������һ���񶯲��ָ��ʱ�̵�ȫ������
*���������
*          ��˾ID               			  FIELD_SYSTEM_COMPANY
*		   �ֳ���               			  FIELD_SYSTEM_FACTORY
*          ����ID               			  FIELD_SERVICE_PLANTID
*          ˮƽ�����񶯲��ID  			  FIELD_SERVICE_CHANNID_HOR
*          ��ֱ�����񶯲��ID   		       FIELD_SERVICE_CHANNID_VER
*          ���λ������        			  FIELD_SYSTEM_AXISLOCATION
*          �������ݸ���         			  FIELD_SERVICE_OFF_WAVE_NUM
*          ʱ��                             FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ȫ��Ƶ�ʺͷ�ֵ��Ϣ�ṹ��         FIELD_SERVICE_LEARNPARAM_BUFF
*          ȫ����Ϣ�ṹ���С               FIELD_SERVICE_CHANN_BUFFSIZE
*          ָ���������ת��               FIELD_SERVICE_REV
*          ˮƽ��������ֵ                   FIELD_SERVICE_CHARVALUE_S
*          ��ֱ��������ֵ                   FIELD_SERVICE_CHARVALUE_S2
*          ����Ƶ��                         FIELD_SERVICE_SAMPLE_FREQ
*          ת������                       FIELD_SYSTEM_TYPE //0:Ϊ��ʱ��,1Ϊ˳ʱ��
#define     SERVICE_SERVICE_OFF_GETFULLSPECSTRUM          21070
*/
int Srv_OffGetSpecTimeFullSpectrum(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**������ѯ���ܣ���ѯ����ID��������
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*		     ����ID            FIELD_SERVICE_ALARMID
*		     ������ʼʱ��      FIELD_SERVICE_OFF_TIMESTART
*		     ��������          FIELD_SERVICE_ALARM_TYPE
*            �ɹ���ʾ          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETALARMLOGINFO         21071
*/
int Srv_OffGetAlarmLogInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**����ʽϵͳӦ����⣬�õ�ָ������ָ��ʱ�䡢ָ���׺ŵ�Ӧ�����κ��񶯲�㲨��
*���������
*          ��˾ID        ������    FIELD_SYSTEM_COMPANY
*		  �ֳ���          ������  FIELD_SYSTEM_FACTORY
*          ����ID          ������  FIELD_SERVICE_PLANTID
*          ���׺�                  FIELD_SERVICE_CYLINDERNO
*          �����񶯲��            FIELD_SERVICE_OFF_CHANN1
*          ʱ��                    FIELD_SERVICE_OFF_TIME
*����ֵ���ֶ��룺
*          ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE1
*          ����Ӧ���������ݡ�������FIELD_SERVICE_TIMEWAVE2
*          ������Ӧ���������ݡ�����FIELD_SERVICE_FREQWAVE1
*          �񶯲�������   ������ ��FIELD_SERVICE_FREQWAVE2
*          �������ݸ���   �������� FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE              21072
*/
int Srv_OffGetSpecTimeDynStressWave(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢ʱ��Ρ������������Զ��������ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
*��������ֶ��룺
*          ��˾ID        FIELD_SYSTEM_COMPANY
*		   �ֳ���        FIELD_SYSTEM_FACTORY
*          ����ID        FIELD_SERVICE_PLANTID
*          ������ͣ�    FIELD_SERVICE_CHANNTYPE
*          ���buff      FIELD_SERVICE_CHANN_BUFF
*          ���buff����  FIELD_SERVICE_CHANN_BUFFSIZE
*          ������      FIELD_SERVICE_CHANN_NUM
*          ��ʼʱ��      FIELD_SERVICE_OFF_TIMESTART
*          ��ֹʱ��      FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*          �Զ����������BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
*          ������             FIELD_SERVICE_CHANN_NUM
*          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND         21073
*/
int Srv_OffGetSpecChannsCustomTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/** ��ȡ����Ѳ�����ݣ�
#define     SERVICE_SERVICE_OFF_GET_OFFLINE_DATA         21074
*/
int Srv_OffGetOfflineData(ICommInterFace *pFixComm, IDBInterFace *pDB);

/**���վ����ʷ����ͳ��
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*			 ����ID�б�	      FIELD_SERVICE_ALARMIDS
*			 ����ID����	      FIELD_SERVICE_ALARMID_NUM
*			 ����ID��������С FIELD_SERVICE_ALARMID_BUFFSIZE
*����ֵ���ֶ��룺
*		     ����ID           FIELD_SERVICE_ ALARM ID
*		     �����         FIELD_SERVICE_CHANN1
*		     ���λ��         FIELD_SYSTEM_CHANN_ALIAS
*		     ����ֵ           FIELD_SERVICE_TREND_VALUE
*		     ת��1            FIELD_SERVICE_REV
*		     ������ʼʱ��     FIELD_SERVICE_OFF_TIMESTART
*		     ��������ʱ��     FIELD_SERVICE_OFF_TIMEEND
*		     ��������         FIELD_SERVICE_ALARM_TYPE
*            �ɹ���ʾ         FIELD_SYSTEM_STATUS
			 �ӿ���������	  FIELD_SERVICE_DATALIMIT
#define     SERVICE_SERVICE_OFF_GETALARMDATA    21075
*/
int Srv_OffGetAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);



/**���վ����ʷ����ID�б�
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND
			 

*����ֵ���ֶ��룺
			 ����ID		      FIELD_SERVICE_ALARMID
			 ����ID����       FIELD_SERVICE_ALARMID_NUM

#define     SERVICE_SERVICE_OFF_GETALARMIDS    21076
*/
int Srv_OffGetAlarmIDS(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���վ����ʷ����ID�б�
*�������ֵ����˾ID           FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*	         �豸��           FIELD_SERVICE_PLANTID
*            ��ʼʱ�䣺       FIELD_SERVICE_OFF_TIMESTART
*            ��ֹʱ�䣺       FIELD_SERVICE_OFF_TIMEEND


*����ֵ���ֶ��룺
����ID		              FIELD_SERVICE_ALARMID
����ID��Ӧ�Ĳ����        FIELD_SERVICE_CHANN1
����ID��Ӧ��ʱ��          FIELD_SERVICE_OFF_TIMESTART
��������                  FIELD_SERVICE_ALARM_TYPE

*/
int Srv_OffGetAlarmIDSEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**����ָ����˾���ֳ����豸����ͣ���¼�ID
*��������ֶ��룺
*             ��˾ID         FIELD_SYSTEM_COMPANY
*			  �ֳ���         FIELD_SYSTEM_FACTORY
*             ����ID��       FIELD_SERVICE_PLANTID
*             ��ʼʱ��       FIELD_SERVICE_OFF_TIMESTART
*����ֵ���ֶ��룺
*             ��ͣ���¼�ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_OFF_REQSUDEVENTID        24008
*/
int Srv_OffReqSudEventID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**ָ֪ͨ����ͣ���¼�����ͣ�����̽���������ɹ�����1��ʧ�ܷ���-1
*��������ֶ��룺
*             ��˾ID        FIELD_SYSTEM_COMPANY
*			  �ֳ���        FIELD_SYSTEM_FACTORY
*             ����ID��      FIELD_SERVICE_PLANTID
*             ��ͣ���¼�ID:   FIELD_SYSTEM_EVENT_ID
*             ��ͣ������     FIELD_SERVICE_SUD_TYPE
*             ����ʱ��      FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_NOTICESUDSTOP        24009
*/
int Srv_OffNoticeSudStop(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**��ͣ�������󣬷���ָ�����������ͣ�����ݵ��м��
*��������ֶ��룺
*            ��˾ID                 FIELD_SYSTEM_COMPANY
*			       �ֳ���                 FIELD_SYSTEM_FACTORY
*            ����ID                 FIELD_SERVICE_PLANTID
*            �񶯲������ֵ����     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            ��ʼ����               FIELD_SERVICE_ON_SEND_STARTID
*            �񶯲�����           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            ��������               FIELD_SERVICE_ON_SEND_VIBWAVE
*            һ�鲨�ε���           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            ����Ƶ��               FIELD_SERVICE_SAMPLE_FREQ
*            ��������               FIELD_SERVICE_SAMPLE_NUM
*            �����ת��             FIELD_SERVICE_REV
*            �����ת��2            FIELD_SERVICE_REV2
*            ָ�������ת��3        FIELD_SERVICE_REV3
*            ָ�������ת��4        FIELD_SERVICE_REV4
*            ָ�������ת��5        FIELD_SERVICE_REV5
*            ��ͣ���¼�ID:          FIELD_SYSTEM_EVENT_ID
*            ��Ӧ��ʱ��             FIELD_SERVICE_TIME
*            ��Ӧʱ���΢��         FIELD_SERVICE_MICROSECOND
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECSUDDATA        24010
*/
int Srv_OffRecSudData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**@brief ������ֵ����Ϣ��ѧϰ������ѧϰ���ݣ�6������ֵһ�𷢡�����-1��ʾʧ�ܣ����ش���0��ʾ�ɹ�
*��������ֶ���
*           ��˾ID          FIELD_SYSTEM_COMPANY
*			      �ֳ���          FIELD_SYSTEM_FACTORY
*           ����ID��        FIELD_SERVICE_PLANTID
*           ͨ������        FIELD_SERVICE_CHANNTYPE
*           ͨ������        FIELD_SERVICE_ON_SEND_STARTID
*           ��ֵID          FIELD_SERVICE_THRESHOLD_ID
*           ����ֵ����      FIELD_SERVICE_CHANN_NUM
*           ѧϰ���ݵ�BUFF  FIELD_SERVICE_ON_SEND_VIBWAVE    //��������Ϊ��ͬ����ֵ������������.���鳤��=FIELD_SERVICE_LEN*FIELD_SERVICE_CHANN_NUM
*           ѧϰ���ݵĳ���  FIELD_SERVICE_LEN
*           ��ֵ��BUFF      FIELD_SERVICE_THRESHOLD_BUFF     //�ṹ�峤��ΪFIELD_SERVICE_CHANN_NUM
*           ѧϰ������BUFF  FIELD_SERVICE_LEARNPARAM_BUFF    //�ṹ�峤��ΪFIELD_SERVICE_CHANN_NUM
*           ��������        FIELD_SERVICE_ALARM_TYPE
*           ѧϰ���ݵĿ�ʼʱ��  FIELD_SERVICE_OFF_TIMESTART
*           ѧϰ���ݵĽ���ʱ��  FIELD_SERVICE_OFF_TIMEEND
*           ��������ѧϰ������  FIELD_SYSTEM_TYPE
*����ֵ���ֶ���
*           �ɹ���ʶ            FIELD_SYSTEM_STATUS
#define    SERVICE_SERVICE_OFF_SAVETHINFO         24014
*/
int Srv_OffSaveThInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**ָ֪ͨ����䱨���¼��ı������̽���������ɹ�����1��ʧ�ܷ���-1
*��������ֶ��룺
*             ��˾ID          FIELD_SYSTEM_COMPANY
*			        �ֳ���          FIELD_SYSTEM_FACTORY
*             ����ID��        FIELD_SERVICE_PLANTID
*             ͨ������        FIELD_SERVICE_CHANNTYPE
*             ͨ������        FIELD_SERVICE_ON_SEND_STARTID
*             ��䱨���¼�ID  FIELD_SYSTEM_EVENT_ID
*             ����ֵID        FIELD_SERVICE_THRESHOLD_ID
*             ����ʱ��        FIELD_SERVICE_OFF_TIMEEND
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP        24015
*/
int Srv_OffNoticeFastAlarmStop(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**���Ϳ�䱨�����ݵ��м������
*��������ֶ��룺
*             ��˾ID           FIELD_SYSTEM_COMPANY
*			        �ֳ���           FIELD_SYSTEM_FACTORY
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
*            һ�鲨�ε���      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             ����Ƶ��         FIELD_SERVICE_SAMPLE_FREQ
*             ��������         FIELD_SERVICE_SAMPLE_NUM
  *            ��Ӧ��ʱ��        FIELD_SERVICE_TIME
  *            ��Ӧʱ���΢��    FIELD_SERVICE_MICROSECOND
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
  *����ֵ���ֶ��룺
  *             �ɹ���ʶ      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_RECFASTALARMDATA        24016
  */
int Srv_OffRecFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���汨������ѧϰ����ֵID��������鳤��С��1��ʾʧ��
  *��������ֶ���
  *           ��˾ID          FIELD_SYSTEM_COMPANY
  *			      �ֳ���          FIELD_SYSTEM_FACTORY
  *           ����ID��        FIELD_SERVICE_PLANTID
  *           ͨ������        FIELD_SERVICE_CHANN_NUM
  *           ��ֵID����      FIELD_SERVICE_THRESHOLD_ID
  *����ֵ���ֶ��룺
  *             �ɹ���ʶ      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID   24017
  */
int Srv_OffSaveAlarmedNewThID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���һ�������ı�����־.ʧ�ܷ���ֵС��0���ɹ�Ϊ0
*��������ֶ���
*           ��˾ID          FIELD_SYSTEM_COMPANY
*			�ֳ���          FIELD_SYSTEM_FACTORY
*           ����ID��        FIELD_SERVICE_PLANTID
*           ͨ��ID          FIELD_SERVICE_CHANN1
*           ��־ID          FIELD_SYSTEM_ID
*           ��䱨���¼�ID  FIELD_SYSTEM_EVENT_ID
*           ͨ������        FIELD_SERVICE_CHANNTYPE
*           ��������        FIELD_SERVICE_ALARM_TYPE
*           ��ʼʱ��        FIELD_SERVICE_OFF_TIMESTART
*           ����ʱ��        FIELD_SERVICE_OFF_TIMEEND
*           ��ֵID          FIELD_SERVICE_THRESHOLD_ID
*           ����ֵID        FIELD_SERVICE_ON_SEND_STARTID
*����ֵ���ֶ��룺
*           �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_ADDALARMLOG                24018
*/
int Srv_OffAddAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**����������־��������ʼ��ӡ�����ɹ�����1��ʧ�ܷ���-1
*��������ֶ��룺
*             ��˾ID          FIELD_SYSTEM_COMPANY
*			        �ֳ���          FIELD_SYSTEM_FACTORY
*             ����ID��        FIELD_SERVICE_PLANTID
*             ͨ������        FIELD_SERVICE_CHANNTYPE
*             ͨ������        FIELD_SERVICE_ON_SEND_STARTID
*             ��䱨���¼�ID  FIELD_SYSTEM_EVENT_ID
*             ��ʼʱ��        FIELD_SERVICE_OFF_TIMESTART
*           ��������        FIELD_SERVICE_ALARM_TYPE
*             ��ֵID          FIELD_SERVICE_THRESHOLD_ID
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG        24019
*/
int Srv_OffAddFastAlarmStartLog(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**���һ����������ͣ����־������ɹ�������ͣ���¼�ID��ʧ�ܷ���-1
  *��������ֶ��룺
  *             ��˾ID        FIELD_SYSTEM_COMPANY
  *			    �ֳ���        FIELD_SYSTEM_FACTORY
  *             ����ID��      FIELD_SERVICE_PLANTID
  *             ��ͣ������    FIELD_SERVICE_SUD_TYPE
  *             ��ʼʱ��      FIELD_SERVICE_OFF_TIMESTART
  *             ����ʱ��      FIELD_SERVICE_OFF_TIMEEND
  *����ֵ���ֶ��룺
  *             ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  #define     SERVICE_SERVICE_OFF_ADDWHOLESUDLOG             24053
  */
int Srv_OffAddWholeSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**���Ͷ�̬�������ݵ��м������
*��������ֶ��룺
*             ��˾ID           FIELD_SYSTEM_COMPANY
*			  �ֳ���           FIELD_SYSTEM_FACTORY
*             ����ID��         FIELD_SERVICE_PLANTID
*             ��ʼ����         FIELD_SERVICE_ON_SEND_STARTID
*             ������������     FIELD_SERVICE_ALARM_TYPE
*             ��̬�����¼�ID   FIELD_SYSTEM_EVENT_ID
*             ����ֵ����       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             ͨ������         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             ��������         FIELD_SERVICE_ON_SEND_VIBWAVE
*             �ܵĲ��ε���      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             ��Ӧ��ʱ��        FIELD_SERVICE_TIME
*             ��Ӧʱ���΢��    FIELD_SERVICE_MICROSECOND
*             �Ƿ���θ����������   FIELD_SERVICE_SEND_END
*����ֵ���ֶ��룺
*             �ɹ���ʶ      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECDYNALARMDATA         24055
*/
int Srv_OffReceiveDynAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/** ���͹������������ݵ��м��ģ��
*��������ֶ��룺
*                ��˾ID                 FIELD_SYSTEM_COMPANY
*                �ֳ���                 FIELD_SYSTEM_FACTORY
*                ����ID                 FIELD_SERVICE_PLANTID
*                ��������               FIELD_SERVICE_ALARM_TYPE
*                �����¼�ID             FIELD_SYSTEM_EVENT_ID
*                ��Ӧ��ʱ��             FIELD_SERVICE_TIME
*                ����������           FIELD_SYSTEM_CHANN
*                �������������         FIELD_SYSTEM_CHANN_TYPE
*                ����������ֵ           FIELD_SERVICE_CHANN1VALUE
*                ת��                   FIELD_SERVICE_REV
*
*����ֵ���ֶ��룺
*                �ɹ���ʶ               FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECPROCALARMDATA           24056
*/
int Srv_OffReceiveProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);



/**��������Ѳ������ͬ�����������߲�������͸�������ͬ��ʱ��
*��������ֶ��룺
*                ��˾              FIELD_SYSTEM_COMPANY
*			     �ֳ���            FIELD_SYSTEM_FACTORY
*                �豸/����         FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*                �ϴ�ͬ��ʱ��      FIELD_SERVICE_TIME
*                ���߲����ʼ����  FIELD_SERVICE_ON_SEND_STARTID
*                ���߲�����      FIELD_SERVICE_CHANN_NUM
*                ͬ����Ӧ          FIELD_SYSTEM_STATUS
��FIELD_SYSTEM_SUCCESS ����ͬ��/ FIELD_SYSTEM_FAILURE �쳣���޷�ͬ����
*
#define     SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC
*/
int Srv_OffRequestOfflineSync(ICommInterFace *pFixComm, IDBInterFace *pDB);

/**��������Ѳ������
*��������ֶ��룺
*                ��˾��        FIELD_SYSTEM_COMPANY
*                �ֳ���        FIELD_SYSTEM_FACTORY
*                �豸��        FIELD_SERVICE_PLANTID  (��1T1, 1T2)
*                �ɼ�ʱ��      FIELD_SERVICE_TIME
*                �����        FIELD_SYSTEM_CHANN
*                ����ֵ        FIELD_SERVICE_OFF_DATA( float)
*����ֵ���ֶ��룺
*                ͬ��״̬      FIELD_SYSTEM_STATUS
                                ��FIELD_SYSTEM_SUCCESS/ FIELD_SYSTEM_FAILURE��
*
#define     SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA            24061
*/
int Srv_OffSyncOfflineData(ICommInterFace *pFixComm, IDBInterFace *pDB);


/**���ն�ƽ������
*��������ֶ��룺
*                ��˾��         FIELD_SYSTEM_COMPANY
*                �ֳ���         FIELD_SYSTEM_FACTORY
*                �豸��         FIELD_SERVICE_PLANTID  
*                �ɼ�ʱ��       FIELD_SERVICE_TIME
*                �����         FIELD_SYSTEM_CHANN
				 �����2        FIELD_SYSTEM_CHANN2
*                BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
				 BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
				 BUFFER1����    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   �ֽ�
				 BUFFER2����    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   �ֽ�
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0���� 1˫��
*����ֵ���ֶ��룺
*                ͬ��״̬      FIELD_SYSTEM_STATUS                                
*
#define     SERVICE_SERVICE_OFF_SET_BALANCE_DATA            24062
*/
int Srv_SetBalanceData(ICommInterFace *pFixComm, IDBInterFace *pDB);



/**���ض�ƽ������
*  ��������ֶ��룺
*                ��˾��         FIELD_SYSTEM_COMPANY
*                �ֳ���         FIELD_SYSTEM_FACTORY
*                �豸��         FIELD_SERVICE_PLANTID  
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0���� 1˫��
*   ����ֵ���ֶ��룺
*               �ɼ�ʱ��       FIELD_SERVICE_TIME
*               �����         FIELD_SYSTEM_CHANN
*				�����2        FIELD_SYSTEM_CHANN2
*               BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
*				BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
*				BUFFER1����    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   �ֽ�
*				BUFFER2����    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   �ֽ�                     

*#define     SERVICE_SERVICE_OFF_GET_BALANCE_DATA 24063*/


int Srv_GetBalanceData(ICommInterFace *pFixComm, IDBInterFace *pDB);


	/**��ʱ�������󣬶�ȡ�������ö�����ڲɼ����豸,e
	��ȡ�߼������û�����루��˾���ֳ����豸������һ������Ϊ�㣬�������豸����û�ж�Ӧ���豸�����򷵻������豸�Ĳɼ�״̬������Ƕ�����ͻ���Ӧ��while(fixcom.more())�ķ�ʽ��ȡ
	�����߼�����������루��˾���ֳ����豸������һ�����Ⱦ���Ϊ�㣬�����豸�����ж�Ӧ���豸�����򷵻������豸�Ĳɼ�״̬���Լ��ϴ����õ�ʱ�䣬�ͻ���Ӧ��while(fixcom.more())�ķ�ʽ��ȡ
	�м�����������ϴ����õ�Ϊ׼���״������������ɼ�
	*��������ֶ��룺
	*                ��˾��         FIELD_SYSTEM_COMPANY
	*                �ֳ���         FIELD_SYSTEM_FACTORY
	*                �豸����       FIELD_SERVICE_PLANTID  
	*                ��¼ʱ��       FIELD_SERVICE_OFF_TIME
					����״̬		FIELD_SYSTEM_STATUS    1.��ʾ�ɼ� 0.��ʾ���ɼ�
	*                
	*����ֵ���ֶ��룺
	*                ��˾��         FIELD_SYSTEM_COMPANY
	*                �ֳ���         FIELD_SYSTEM_FACTORY
	*                �豸����       FIELD_SERVICE_PLANTID  
	*                ��¼ʱ��       FIELD_SERVICE_OFF_TIME 
					����״̬		FIELD_SYSTEM_STATUS    1.��ʾ�ɼ� 0.��ʾ���ɼ�
									

	#define     SERVICE_SERVICE_TEMP_SAMPLE_PLANTS 24070 
	*/
int Service_temp_sample_plants(ICommInterFace * pFixComm,IDBInterFace *pDB);

	/*!
	*��������ֶ���
	*          ��˾ID      FIELD_SYSTEM_COMPANY
	*	     �ֳ���      FIELD_SYSTEM_FACTORY
	*          ����ID��    FIELD_SERVICE_PLANTID
	*          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
	*          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
	*����ֵ���ֶ��룺
	*          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
	*          ��ͣ�����ͣ�  FIELD_SYSTEM_TYPE
	*          �Գ��¼���    FIELD_SYSTEM_EVENT_NAME
	*          ��ʼʱ�䣺    FIELD_SERVICE_OFF_TIMESTART
	*          ��ֹʱ�䣺    FIELD_SERVICE_OFF_TIMEEND
	*         #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX    21077 
	*/
int Service_GetSpecTimeRangeSudAndName(ICommInterFace * pFixComm,IDBInterFace *pDB);


/*!
  *��������ֶ���
  *          ��˾ID      FIELD_SYSTEM_COMPANY
  *	         �ֳ���      FIELD_SYSTEM_FACTORY
  *          ����ID��    FIELD_SERVICE_PLANTID
  *          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
  *          ��ֹʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
  *          ��ѯ�Գ���¼�ַ���  FIELD_SYSTEM_QUERY_EVENTNAME
  *����ֵ���ֶ��룺
  *          ��ͣ���¼�ID: FIELD_SYSTEM_EVENT_ID
  *          ��ͣ�����ͣ�  FIELD_SYSTEM_TYPE
  *          �Գ��¼���    FIELD_SYSTEM_EVENT_NAME
  *          ��ʼʱ�䣺    FIELD_SERVICE_OFF_TIMESTART
  *          ��ֹʱ�䣺    FIELD_SERVICE_OFF_TIMEEND
  #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME    21078
  */
 
	int GetSpecTimeRangeSudSearchByName( ICommInterFace * pFixComm, 
														IDBInterFace *pDB );



/*!
  *��������ֶ���
  *          ��˾ID          FIELD_SYSTEM_COMPANY
  *	     �ֳ���          FIELD_SYSTEM_FACTORY
  *          ����ID��        FIELD_SERVICE_PLANTID
  *          ��ͣ���¼�ID    FIELD_SYSTEM_EVENT_ID
  *          ����ͣ���¼���  FIELD_SYSTEM_EVENT_NAME
  *����ֵ���ֶ��룺
  *          �Ƿ��޸ĳɹ���  FIELD_SYSTEM_STATUS
  *  #define    SERVICE_SERVICE_OFF_RENAMEEVENT 21079
  */
	int UpdateRenameEvent( ICommInterFace * pFixComm, 
										  IDBInterFace *pDB );

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
  *#define    SERVICE_SERVICE_OFF_SAVESPECTIMEDATA    21083
  */
	int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);

 /*!
 *��������ֶ���
 *          ��˾ID          FIELD_SYSTEM_COMPANY
 *	         �ֳ���        FIELD_SYSTEM_FACTORY
 *          ����ID��        FIELD_SERVICE_PLANTID
 *          ��עʱ�̣�      FIELD_SERVICE_OFF_TIME
 *          ��㣺          FIELD_SERVICE_CHANN1
 *          �¼���ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
 *          �¼�����ʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
 *          ��ע������      FIELD_SERVICE_REMARK    
 *����ֵ���ֶ��룺
 *		   �Ƿ񱣴�ɹ���  FIELD_SYSTEM_STATUS
 *#define    SERVICE_SERVICE_OFF_SAVESPECMARK    21084
 */
	int SaveSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);


	/*!
	*��������ֶ���
	*          ��˾ID      FIELD_SYSTEM_COMPANY
	*	       �ֳ���    FIELD_SYSTEM_FACTORY
	*          ����ID��    FIELD_SERVICE_PLANTID
	*          ��ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART
	*          ����ʱ�䣺  FIELD_SERVICE_OFF_TIMEEND
	*          ��ע������  FIELD_SERVICE_REMARK    
	*����ֵ���ֶ��룺
	*          ��㣺      FIELD_SERVICE_CHANN1
	*		   ��ע������  FIELD_SERVICE_REMARK 
	*          ��עʱ�䣺  FIELD_SERVICE_OFF_TIME
	*#define    SERVICE_SERVICE_OFF_GETSPECMARK    21085
	*/
	int GetSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**�õ�ָ����˾���ֳ������顢ʱ��Ρ��������񶯲�������������ֵ�������ݣ�ͨ��ѭ�����εõ������������ݺͶ�Ӧʱ�䣬ÿ��ȡ�����ݶ�Ӧʱ�䶼����ͬ��
  *��������ֶ��룺
  *          ��˾ID        FIELD_SYSTEM_COMPANY
  *			 �ֳ���        FIELD_SYSTEM_FACTORY
  *          ����ID        FIELD_SERVICE_PLANTID
  *          ������ͣ�    FIELD_SERVICE_CHANNTYPE
  *          ���buff      FIELD_SERVICE_CHANN_BUFF
  *          ���buff����  FIELD_SERVICE_CHANN_BUFFSIZE
  *          ������      FIELD_SERVICE_CHANN_NUM
  *          ��ʼʱ��      FIELD_SERVICE_OFF_TIMESTART
  *          ��ֹʱ��      FIELD_SERVICE_OFF_TIMEEND
  *          ��ѯ���������� FIELD_SYSTEM_TYPE��0:ֻ�г������ݣ�1:������������ 2:������ͣ�����ݣ�
  *����ֵ���ֶ��룺
  *          �����������ֵBuff   FIELD_SERVICE_CHARVALUE_SBUFF
  *          ������             FIELD_SERVICE_CHANN_NUM
  *          ʱ��ֵ               FIELD_SERVICE_OFF_TIME
  *          ת��Buff             FIELD_SERVICE_REV_BUFF

  *  #define     SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX         21081
*/
	int Srv_OffGetVibChannsTrendEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** ��ͣ�������󣬷���ָ���������ͣ���񶯲�����ݵ��м��
	* #define     SERVICE_SERVICE_OFF_SUD_VIB_DATA                         24110
	*/
	int Srv_OffRecSudVibData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** ��ͣ�������󣬷���ָ���������ͣ����̬������ݵ��м��
	* #define     SERVICE_SERVICE_OFF_SUD_DYN_DATA                         24111
	*/
	int Srv_OffRecSudDynData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** ��ͣ�������󣬷���ָ���������ͣ��������������ݵ��м��
	* #define     SERVICE_SERVICE_OFF_SUD_PROC_DATA                        24113
	*/
	int Srv_OffRecSudProcData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**��ͣ�������� ͬ��ָ����˾���ֳ����豸����ͣ���¼�ID
	* #define     SERVICE_SERVICE_OFF_SUD_ADD_LOG                          24114
	*/
	int Srv_OffRecSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**֪ͨ�м�����̿ռ��Ƿ�����
	* #define     SERVICE_SERVICE_OFF_SET_DISK_FULL                          21086
	*/
	int Srv_OffSetDiskFull(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/*!
	*��������ֶ���
	*          ��˾ID    FIELD_SYSTEM_COMPANY
	*	       �ֳ���    FIELD_SYSTEM_FACTORY
	*          ����ID��  FIELD_SERVICE_PLANTID
	*          ���ID��  FIELD_SYSTEM_CHANN
	*          ����ID��  FIELD_SERVICE_ALARMID
	*          ������ʼʱ�䣺  FIELD_SERVICE_OFF_TIMESTART  
	*����ֵ���ֶ��룺
	*          �����������ݣ�      FIELD_SERVICE_OFF_WAVE
	*          ���ε�����      FIELD_SERVICE_OFF_WAVE_NUM
	*          ����Ƶ�ʣ�      FIELD_SERVICE_SAMPLE_FREQ

	#define    SERVICE_SERVICE_OFF_GETNORMALWAVE    21089
	*/
	int Srv_OffGetNormalWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

    /**���վ����ʷ����ͳ��
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL       21090
	*/
	int Srv_OffGetMonitorAlarmStatWithLevel(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**��ȡ�����źŲ���
    #define     SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE       21095
	*/
	int Srv_OffGetErrorSignalWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**��������λ����ز���
    #define     SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM       21096
	*/
	int Srv_OffSetAxesLocationParam(ICommInterFace *pFixComm,IDBInterFace *pDB);
#endif