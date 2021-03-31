#include "stdafx.h"
#include "process.h"
#include <math.h>
#include <algorithm>
#include <string>
#include <map>
#include "OnlineGlobal.h"
#include "TransactData.h"
#include "TransactDaqData.h"


CTransactData tranData;
CTransactDaqData tranDaq;
//...�����ʵ��ҵ����
/**�õ������񶯲����������͵�����ֵ��ͨ��ѭ������ȡ�ø�������������������ֵ��
  *ÿѭ�����οɵõ�һ��������������ֵ
  *#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000
  */
int Service_OnGetAllFiveChar(ICommInterFace * pFixComm, IDBInterFace *pDB )
{
	return tranData.GetAllFiveChar(pFixComm);
}

/**�õ�ָ������ָ���񶯲����������͵�����ֵ��
  */
//#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR      20001
int Service_OnGetSpecFiveChar(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSpecFiveChar(pFixComm);
}

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

int Srv_OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnBatchGetMonitorAlarmStat(pFixComm,pDB->getPDB());
}

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
int Srv_OnGetAllTypeChar(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetAllTypeChar(pFixComm);
}

/**�õ�ָ���������񶯲���Ƶ�ס������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
#define     SERVICE_SERVICE_ON_GETSPECTRUM                20003
*/
int Srv_OnGetSpectrum(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSpectrum(pFixComm,pDB->getPDB());
}

/**#define     SERVICE_SERVICE_ON_GETFOURSPECCHAR        20004
   */
int SerVice_OnGetFourSpecChar(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetFourSpecChar(pFixComm);
}

  /**�õ�ָ����������й���������ֵ��
  *����������豸ID         FIELD_SERVICE_PLANTID
  *          �������       FIELD_SERVICE_CHANNTYPE
  *����ֵ���ֶ��룺
  *          ������ֵ       FIELD_SERVICE_TREND_PROCESS
  *          ���ID         FIELD_SERVICE_CHANN1
  *#define     SERVICE_SERVICE_ON_GETALLPROCVALUE       20005
  */
int SerVice_OnGetAllProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
  	return tranData.GetAllProcValue(pFixComm);
}

/**�õ�ָ������������������ֵ��
  *#define     SERVICE_SERVICE_ON_GETSPECPROCVALUE  20006
  */
int SerVice_OnGetSpecProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSpecProcValue(pFixComm);
}

/**�õ�ָ������ָ��һ�������Ĺ켣�Ĳ������ݡ�
  *define     SERVICE_SERVICE_ON_GETAXESTRACK           20008
  */
int Service_OnGetAxesTrack(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
    return tranData.GetAxesTrack(pFixComm);
}
/**�õ�ָ�������񶯲���ʵʱ���κ�Ƶ�ס�������񶯲��2�����ò�������ֻ��ȡһ�飩
#define     SERVICE_SERVICE_ON_GETWAVE                20007
*/
int Service_OnGetWave(ICommInterFace * pFixComm,IDBInterFace *pDB)
{
	return tranData.GetTwoWave(pFixComm);
}

/**�õ�ָ������ָ��һ��������λ��ֵ��
  *����������豸ID               FIELD_SERVICE_PLANTID
  *          ˮƽ�����񶯲��ID   FIELD_SERVICE_CHANNID_HOR
  *          ��ֱ�����񶯲��ID   FIELD_SERVICE_CHANNID_VER
  *����ֵ���ֶ��룺ˮƽ�����񶯲��λ��  FIELD_SERVICE_AXESLOCATION_HOR
  *                ��ֱ�����񶯲��λ��  FIELD_SERVICE_AXESLOCATION_VER
  *define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009
  */
int Service_OnGetAxesLocation(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	 return tranData.GetAxisLocation(pFixComm);
}


int Service_OnGetRev(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetPlantRev(pFixComm);
}

/**�õ�ָ������ָ����λ�Ʋ��ֵ
  *define     SERVICE_SERVICE_ON_GETSPECAXISDISPLACE    20011
  */
int Service_OnGetSpecAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSpecAxisDisplaceValue(pFixComm);
}

/**�õ�ָ������������λ�Ʋ��ֵ��ͨ��ѭ�����εõ�������λ��ֵ
  *define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012
  */
int Service_OnGetAllAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetAllAxisDisplace(pFixComm);
}

/**�õ��û����۲��豸��Χ�������豸�Ĳ������״̬��ͨ��ѭ�����εõ����б����Ĳ��
  *�������ֵ���û�ID         FIELD_SYSTEM_USEID
  *����ֵ�ֶ��룺
  *            �������豸ID         FIELD_SERVICE_PLANTID
  *            �����Ĳ��ID         FIELD_SERVICE_CHANN1
  *            ����ֵ               FIELD_SERVICE_CHANN1VALUE
  #define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
  */
int Service_OnGetAllChannelState(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetAllChannelState(pFixComm,pDB);
}

///20014
int Service_OnGetAllChannelValue(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return  tranData.GetAllChannValue(pFixComm,pDB->getPDB());
}

/**�õ�ָ���������̬ѹ�����ʾ��ͼ��Ϣ��ѭ���õ�������������
  #define   SERVICE_SERVICE_ON_GETPVDATA               20015
  */
int Srv_OnGetPVData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.GetPVData(pFixComm,pDB->getPDB());
}

/**�õ�ָ������������������ʵʱ���Ρ�Ƶ�׺���������ֵ�������֮�䡰�����ֿ�������������㣬��ѭ����ȡ
  #define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
  */
int Srv_OnGetRodSinkData(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetRodSinkData(pFixComm,pDB->getPDB());
}

/**ͨ���ֳ����õ������豸�ı���״̬
  *�������ֵ���ֳ���  FIELD_SYSTEM_SUBCORP
  *����ֵ�ֶ��룺
  *            �����Ļ���ID         FIELD_SERVICE_PLANTID
  #define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017
  */
int Service_OnGetSubCorpState(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSubCorpState(pFixComm,pDB->getPDB());
}

/**�õ�ָ��ָ�������ָ����������ͣ���Ĳ��Ĳ���ͼ���ݣ�һ��ֻ�ܵõ�һ������
  *���������
  *          ��˾ID           FIELD_SYSTEM_COMPANY
  * 	     �ֳ���           FIELD_SYSTEM_FACTORY
  *          ����ID           FIELD_SERVICE_PLANTID
  *          ����           FIELD_SERVICE_CHANN1
  *          ����ֵ����       FIELD_SERVICE_TREND_CHARTYPE
  *����ֵ���ֶ��룺
  *          ����           FIELD_SERVICE_CHANN1
  *          ��λ��           FIELD_SERVICE_TREND_ONECHAR
  *          ��ֵ��           FIELD_SERVICE_TREND_OVERALLCHAR
  *          ת�٣�           FIELD_SERVICE_REV
  *          ʱ��             FIELD_SERVICE_TIME
  *          ������ֵ�ṹ��    FIELD_SERVICE_CHARVALUE_S
*define     SERVICE_SERVICE_ON_GETBODE                20018
*/
int Service_OnGetBode(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetBode(pFixComm,pDB->getPDB());
}

/**�õ�ָ��ָ�������ָ����������ͣ���Ĳ���ٲ�ͼ��һ��ֻ�ܵõ�һ�鲨��Ƶ��
*�������������ID           FIELD_SERVICE_PLANTID
*          ����           FIELD_SERVICE_CHANN1
*          ��������         FIELD_SERVICE_ZOOM_LINES
*����ֵ���ֶ��룺
*          ��λ��           FIELD_SERVICE_TREND_ONECHAR
*          ��ֵ��           FIELD_SERVICE_TREND_OVERALLCHAR
*          ת�٣�           FIELD_SERVICE_RE
*          Ƶ������         FIELD_SERVICE_FREQWAVE1
*          Ƶ�����ݸ���     FIELD_SERVICE_FREQWAVE_NUM
*          Ƶ��DF           FIELD_SERVICE_FREQWAVE_DFV
*define     SERVICE_SERVICE_ON_GETWATERFALL                20019
*/
int Service_OnGetWaterFall(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetWaterFall(pFixComm,pDB->getPDB());
}

/**�õ�ָ�������񶯲���ϸ��Ƶ�׷����Ͳ��Ρ�
  *�������������ID           FIELD_SERVICE_PLANTID
  *          ���             FIELD_SERVICE_CHANN1
  *          ��������         FIELD_SERVICE_ZOOM_LINES
  *          ���ε���         FIELD_SERVICE_TIMEWAVE_NUM
  *          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
  *          ��ֹƵ��         FIELD_SERVICE_ZOOM_STOPFREQ
  *����ֵ���ֶ��룺
  *          ���Ĳ�������   FIELD_SERVICE_TIMEWAVE1
  *          ����Ƶ������   FIELD_SERVICE_ZOOMSPECTRUM
  *          �������ݸ���     FIELD_SERVICE_TIMEWAVE_NUM
  *          ��������         FIELD_SERVICE_ZOOM_LINES
  *          ��ʼƵ��         FIELD_SERVICE_ZOOM_STARTFREQ
  *          �񶯲��Ƶ��DF   FIELD_SERVICE_FREQWAVE_DF
  #define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020
  */
int Service_OnGetZoomSpec(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetZoomSpec(pFixComm,pDB->getPDB());
}

/**�õ�ָ��ָ������Ͳ��������Ϣ
*�������������ID           FIELD_SERVICE_PLANTID
*          ����           FIELD_SERVICE_CHANN1
*����ֵ���ֶ��룺
*          �ת�٣�       FIELD_SERVICE_REV
*          ʵʱת�٣�       FIELD_SERVICE_REV2
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_FREQ
*          ����Ƶ��         FIELD_SERVICE_SAMPLE_NUM
#define     SERVICE_SERVICE_ON_GETSPECCHANNINFO           20021
*/
int Service_OnGetSpecChannInfo(ICommInterFace* pFixComm,IDBInterFace *pDB)
{
	return tranData.GetSpecChannInfo(pFixComm,pDB->getPDB());
}

/**��ȡָ���豸�񶯲���ָ����Ƶ������ֵ����λ��
*���ָ������ˣ���ֻȡָ����������ֵ����λ
*���û��ָ����㣬�������в��ͨ��ѭ������ȡ�ø������Ķ�Ӧ��Ƶ����ֵ����λ��ÿѭ�����οɵõ�һ�����ָ����������ֵ����λ
*�������ֵ������ID      FIELD_SERVICE_PLANTID
*            ���ID      FIELD_SERVICE_CHANN1
*            ����ֵ����  FIELD_SERVICE_TREND_CHARTYPE
*����ֵ���ֶ��룺
*            ���ID      FIELD_SERVICE_CHANN1
*            ָ������ֵ  FIELD_SERVICE_TREND_VALUE
*            ��Ӧ����λ  FIELD_SERVICE_TREND_PHASE
#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022
*/
int Service_OnGetAllSpecCharAndPhase(ICommInterFace* pFixComm,IDBInterFace* pDB)
{
	return tranData.GetAllSpecCharAndPhase(pFixComm,pDB->getPDB());
}


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
*          �������                   FIELD_SERVICE_CHANN1
*          ��㲨��                   FIELD_SERVICE_TIMEWAVE1
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
int Srv_OnGetRcMultiParamData(ICommInterFace* pFixComm,IDBInterFace* pDB)
{
	return tranData.OnGetRcMultiParamData(pFixComm,pDB->getPDB());
}

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
int Srv_OnGetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranData.OnGetGroupAlarmStat(pFixComm,pDB->getPDB());
}

/**��ȡ��˾��ʵʱ����ͳ������
*�������ֵ����˾ID          FIELD_SYSTEM_COMPANY
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*����ֵ���ֶ��룺
*		     �ֳ���           FIELD_SYSTEM_FACTORY
*		     �豸��           FIELD_SERVICE_PLANTID
*		     �����           FIELD_SERVICE_CHANN_NUM
*		     ����״̬         FIELD_SERVICE_ALARM_STATUS
*		     ��ͣ��״̬       FIELD_SERVICE_SUD_STATUS
*		     ת��1            FIELD_SERVICE_REV
*		     ����ID             FIELD_SERVICE_ALARMID
*		     ����ͬ����ʶ    FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT     20025
*/
int Srv_OnGetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
   return tranData.OnGetCompanyAlarmStat(pFixComm,pDB->getPDB());
}

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
#define     SERVICE_SERVICE_ON_MONITORALARMSTAT          20026
*/
int Srv_OnGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnGetMonitorAlarmStat(pFixComm,pDB->getPDB());
}

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
int Srv_OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnGetRodSinkAxesLocation(pFixComm,pDB->getPDB());
}

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
*		   ����ֵ�ṹ��		       FIELD_SERVICE_CHARVALUE_S
*		   ����ֵ�ṹ��2		   FIELD_SERVICE_CHARVALUE_S2
*		   �����				   FIELD_SYSTEM_CHANN
*		   �����2				   FIELD_SYSTEM_CHANN2
*		   ���1�ǶȲ���1		   FIELD_SERVICE_ANGLEPARAM1
*		   ���1�ǶȲ���2		   FIELD_SERVICE_ANGLEPARAM2
*		   ���1�ǶȲ���3		   FIELD_SERVICE_ANGLEPARAM3
*		   ���1�ǶȲ���4		   FIELD_SERVICE_ANGLEPARAM4
*		   ���2�ǶȲ���1		   FIELD_SERVICE_ANGLEPARAM5
*		   ���2�ǶȲ���2		   FIELD_SERVICE_ANGLEPARAM6
*		   ���2�ǶȲ���3		   FIELD_SERVICE_ANGLEPARAM7
*		   ���2�ǶȲ���4		   FIELD_SERVICE_ANGLEPARAM8
*		   ʱ��				   	   FIELD_SERVICE_TIME 
#define     SERVICE_SERVICE_ON_GETDYNSTRESSWAVE              20028
*/
int Srv_OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnGetDynStressWave(pFixComm,pDB);
}

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
int Srv_OnGetPurifyAxesTrack(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnGetPurifyAxesTrack(pFixComm,pDB->getPDB());
}

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
int Srv_OnGetCepstrum(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.OnGetCepstrum(pFixComm,pDB->getPDB());
}

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
int Srv_OnGetFullSpectrum(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
    return tranData.OnGetFullSpectrum(pFixComm,pDB->getPDB());
}

/**�����񶯲��ʵʱ��������ֵ����λ�����ݵ��м��ģ��
  *define     SERVICE_SERVICE_ON_RECEIVEALLVALUE        24000
  */
int Service_OnReceiveVibValue(ICommInterFace * pFixComm,IDBInterFace * pDB)
{
	return tranDaq.ReceiveVibValue(pFixComm,pDB);
}

/**����ָ������Ķ�̬���ʵʱ��������ֵ�������ݵ��м��ģ��
#define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001
*/
int Srv_OnReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveDynValue(pFixComm,pDB);
}


/**����ת�ٵ��м��ģ��
  *��������ֶ��룺
  *                ����ID      FIELD_SERVICE_PLANTID
  *                �����ת��  FIELD_SERVICE_REV
  *                �����ת��2 FIELD_SERVICE_REV2
  *����ֵ���ֶ��룺��
 #define     SERVICE_SERVICE_ON_RECEIVEREV             24002
 */
int Service_OnReceiveRev(ICommInterFace * pFixComm,IDBInterFace * pDB)
{
	return tranDaq.ReceiveRev(pFixComm,pDB->getPDB());
}

/**���͹��������ݵ��м��ģ��
 *��������ֶ��룺
 *                ����ID            FIELD_SERVICE_PLANTID
 *                ��ʼ����          FIELD_SERVICE_ON_SEND_STARTID
 *                �������          FIELD_SERVICE_CHANNTYPE
 *                ��Ҫ���ݲ������  FIELD_SERVICE_ON_SEND_CHANNTYPE
 *                ����������        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
 *                ����������        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
 *����ֵ���ֶ��룺���FIELD_SERVICE_ON_SEND_CHANNTYPE�ֶβ���Ϊ0��2���Ͳ���������
 *                ����              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
 *                ����              FIELD_SERVICE_ON_SEND_AXISDISPLACE
  *define     SERVICE_SERVICE_ON_RECEIVEPROC             24003
  */
int Service_OnReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
   //return tranDaq.ReceiveProc(pFixComm,pDB->getPDB());
	return tranDaq.ReceiveProc(pFixComm,pDB);
}


/**���ܻ�������״̬���ݵ��м��ģ�顣������������ݵı���״̬
  *define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24004
  */
int Service_OnReceiveChannState(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
 return tranDaq.ReceiveChannState(pFixComm,pDB->getPDB());
}

/**���ܻ�������״̬���ݵ��м��ģ�顣������������ݵı���״̬
  *define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL      24027
  */
int Service_OnReceiveChannStateWithAlarmLevel(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
 return tranDaq.ReceiveChannStateWithAlarmLevel(pFixComm,pDB->getPDB());
}

/**����ָ��ָ���豸����ͣ�����ݵ��м��
  *��������ֶ��룺
  *define     SERVICE_SERVICE_ON_REVSUDDATA            24005
  */
int Service_OnRevSudData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
  return tranDaq.ReciveSudData(pFixComm,pDB);
}

/**֪ͨ�м������������Ҫ��������24006
*/
int Service_OnSrvSaveData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
 return tranDaq.SrvSaveData(pFixComm,pDB);
}

/**֪ͨ�м������������Ҫ�жϱ���24007
*/
int Service_OnSrvJudgeAlarm(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
   return tranDaq.SrvJudgeAlarm(pFixComm,pDB);
}

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
int Service_OnReqThresholdID(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReqThresholdID(pFixComm,pDB->getPDB());
}

/**����ָ����˾���ֳ����豸�Ŀ�䱨���¼�ID
*��������ֶ��룺
*             ��˾ID         FIELD_SYSTEM_COMPANY
*			  �ֳ���         FIELD_SYSTEM_FACTORY
*             ����ID��       FIELD_SERVICE_PLANTID
*����ֵ���ֶ��룺
*             ��䱨���¼�ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012
*/
int Srv_OnReqFastAlarmEventID(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.ReqFastAlarmEventID(pFixComm,pDB->getPDB());
}

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
int Srv_OnUpdateFastAlarmStatus(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.UpdateFastAlarmStatus(pFixComm,pDB->getPDB());
}

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
int Srv_NetOffReceiveAllValue(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.NetOffReceiveAllValue(pFixComm,pDB);
}

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
int Srv_NetOffReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.NetOffReceiveDynValue(pFixComm,pDB);
}

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
int Srv_NetOffReceiveProc(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.NetOffReceiveProc(pFixComm,pDB);
}

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
int Srv_NetOffReceiveAllWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.NetOffReceiveAllWirelessValue(pFixComm,pDB);
}

/**����ALLPROC���������м��ģ�飬���õ�ָ�����͵�����
#define     SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA        24051
*/
int Service_OnGetSpecPlantProcData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
  return tranDaq.GetSpecPlantProcData(pFixComm,pDB->getPDB());
}

/**����ָ������ı���״̬
#define     SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE          24052
*/
int Service_OnGetSpecPlantAllState(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
   return tranDaq.GetSpecPlantAllState(pFixComm,pDB->getPDB());
}

/**����ָ���豸�õ�ָ�������ͳ��ȵ��񶯲�������
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA     24050
*/
int Service_OnGetSpecPlantLengthVibData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.GetSpecPlantLengthVibData(pFixComm,pDB->getPDB());
}

/**����ָ���豸ָ�������ͳ��ȵĶ�̬����
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA     24054
*/
int Service_OnGetSpecPlantLengthDynData(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.GetSpecPlantLengthDynData(pFixComm,pDB->getPDB());
}


/* ��ȡ�쳣���ݼ���� */
int Service_OnGetAbnormalDataRecord(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranData.GetAbnormalDataRecord(pFixComm,pDB->getPDB());
}

/* ��������Ӳ��״̬ */
int Service_OnRcvDAQHWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.RcvDAQHWStatus(pFixComm,pDB->getPDB());
}

/* �����������״̬ */
int Service_OnRcvDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranDaq.RcvDAQSWStatus(pFixComm,pDB->getPDB());
}

/**�õ�ָ���豸ָ��������͵����в�����Ͳ��ţ�ͨ��ѭ�����εõ�
#define     SERVICE_SYSTEM_GETSPECTYPECHANN     25003
*/
int Service_SysGetSpecTypeChann(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranData.Service_SysGetSpecTypeChann(pFixComm, pDB );
}

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
int Service_SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranData.SysGetMachineInfoType(pFixComm,pDB->getPDB());
}

/**���ָ���������ͨ������̬��Ϣ
  *��������ֶ��룺
  *            ��˾ID                 FIELD_SYSTEM_COMPANY
  *			   �ֳ���                 FIELD_SYSTEM_FACTORY
  *            ����ID                 FIELD_SERVICE_PLANTID
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
  *����ֵ���ֶ��룺
  *            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
  
  #define     SERVICE_SERVICE_ON_GET_VIBINFO        24066

  */


int Srv_GetVibInfo(ICommInterFace * pFixComm,IDBInterFace * pDB)
{
	return tranDaq.ReceiveVibValueForGap(pFixComm,pDB);
}



/**���ָ������Ķ�̬ͨ������̬��Ϣ
  *��������ֶ��룺
  *            ��˾ID                 FIELD_SYSTEM_COMPANY
  *			   �ֳ���                 FIELD_SYSTEM_FACTORY
  *            ����ID                 FIELD_SERVICE_PLANTID
  *            ������ʼͨ������       FIELD_SERVICE_STARTINDEX
  *            ���ڽ���ͨ������       FIELD_SERVICE_ENDINDEX
  *            �Ƿ���θ����������   FIELD_SERVICE_SEND_END
  *����ֵ���ֶ��룺
  *            ��Ӧ�ļ�϶��ѹ          FIELD_SERVICE_TREND_GAP
  *				#define     SERVICE_SERVICE_ON_GET_DYNINFO        24067
  */

int Srv_GetDynInfo(ICommInterFace * pFixComm,IDBInterFace * pDB)
{
	return tranDaq.ReceiveDynValueForGap(pFixComm,pDB);

}

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

int Srv_Alarm_Confirm( ICommInterFace * pFixComm,IDBInterFace * pDB )
{
	return tranDaq.Srv_Alarm_Confirm(pFixComm,pDB);
}

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

int ReceiveSudVibValue( ICommInterFace * pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudVibValue(pFixComm,pDB);
}

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
int ReceiveSudDynValue( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudDynValue(pFixComm,pDB);
}

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
int ReceiveSudProcValue( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudProcValue(pFixComm,pDB);
}

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
int ReceiveVibFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveVibFastChangeValue(pFixComm,pDB);
}

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
int ReceiveDynFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveDynFastChangeValue(pFixComm,pDB);
}

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
int ReceiveProcFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveProcFastChangeValue(pFixComm,pDB);
}

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
int Srv_OnReceiveProcFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveProcFastAlarmData(pFixComm,pDB);
}
/**���Ϳ�䱨�����ݵ��м������
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
int Srv_OnReceiveVibFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveVibFastAlarmData(pFixComm,pDB);
}

/**���Ͷ�̬�������ݵ��м������
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
int Srv_OnReceiveDynFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveDynFastAlarmData(pFixComm,pDB);
}

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
int Srv_OnReceiveVibWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	return tranDaq.ReceiveVibWirelessValue(pFixComm,pDB);
}

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
int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.SaveSpecTypeDataByTimeRegion(pFixComm,pDB);
}

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
int ChangeSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.ChangeSpecChannAlarmLimitation(pFixComm,pDB);
}

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
*         ��������       FIELD_SYSTEM_ALARM_LOW
*         ����������     FIELD_SYSTEM_ALARM_HIGH
*         ��������       FIELD_SYSTEM_ALARM_LOW2
*         ����������     FIELD_SYSTEM_ALARM_HIGH2
*         ����״̬       FIELD_SYSTEM_STATUS // Ϊ1��ʾ��ȡ�ɹ�
#define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045
*/
int GetSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.GetSpecChannAlarmLimitation(pFixComm,pDB);
}
 
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
int GetPlantGraphInformationS(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.GetPlantGraphInformationS(pFixComm,pDB);
}

/**�м���л�ȡ��òͼ��Ϣ�ӿ�
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
int GetPlantGraphPositonAndAlarmInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.GetPlantGraphPositonAndAlarmInfo(pFixComm,pDB);
}


/*
#define     SERVICE_SERVICE_ON_SET_PLANT_STATUS 24041
*/
int Srv_SetPlantStatus(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranData.SetPlantStatus(pFixComm,pDB);
}

/*
#define     SERVICE_SERVICE_ON_GET_PLANT_STATUS 24042
*/
int Srv_GetPlantStatus(ICommInterFace *pFixComm,IDBInterFace * pDB)
{
	return tranData.GetPlantStatus(pFixComm,pDB);
}

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
int SetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranDaq.SetWireLessChannStatus(pFixComm,pDB);
}

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
int GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.GetWireLessChannStatus(pFixComm,pDB);
}

#include <iceserver/ice/IceUtil/MD5.h>
int Srv_test( ICommInterFace *pFixComm,IDBInterFace *pDB )
{
	int lnSize = pFixComm->GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);

	std::vector<char> loBuffer(lnSize);
	std::vector<char> loBuffer2(lnSize);
	char *  lpBuffer = &loBuffer.front();

	pFixComm->GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,lpBuffer,lnSize);
	IceUtil::MD5 goMD5;
	goMD5.update((const unsigned char *)lpBuffer,loBuffer.size());
	
	
	unsigned char computebyremote[16] = {0};
	unsigned char computebylocal[16] = {0};
	goMD5.finish();

	goMD5.getDigest((unsigned char *)computebylocal);
	
	
	 pFixComm->GetItemBuf(FIELD_SERVICE_THRESHOLD_ID,(char *)computebyremote,16);
	
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if (memcmp(computebyremote,computebylocal,16) ==0)
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	}else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,0);
	}

	pFixComm->Write(pFixComm->GetAci());

	return 0;
}

/*!
*��������ֶ���
*
*����ֵ���ֶ��룺
*          ���ݲɼ�ʱ�䣺       FIELD_SERVICE_TIME

#define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
*/

int GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.GetMWLinkStatus(pFixComm,pDB);
}

/*!
*��������ֶ���
*
*����ֵ���ֶ��룺
*          ֪ͨ״̬��  FIELD_SYSTEM_STATUS
#define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
*/
int NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	return tranData.NotUpdateICEWatchDog(pFixComm,pDB);
}

int Srv_OnReceiveSudVibData( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudVibData(pFixComm,pDB);
}

int Srv_OnReceiveSudDynData( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudDynData(pFixComm,pDB);
}

int Srv_OnReceiveSudProcData( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranDaq.ReceiveSudProcData(pFixComm,pDB);
}

int Srv_OnGetLogFileContent( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranData.GetLogFileContent(pFixComm,pDB);
}
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
int Srv_OnGetChannGradeThreshold( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranData.GetChannGradeThreshold(pFixComm,pDB);
}

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
int Srv_OnSetChannGradeThreshold( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranData.SetChannGradeThreshold(pFixComm,pDB);
}

int Srv_OnGetAxesLocationParam( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranData.GetAxesLocationParam(pFixComm,pDB);
}

int Srv_OnSetAxesLocationParam( ICommInterFace* pFixComm,IDBInterFace *pDB )
{
	return tranData.SetAxesLocationParam(pFixComm,pDB);
}