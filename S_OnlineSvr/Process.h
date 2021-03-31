#include "StdAfx.h"
#include "additions/BHOleDbWrapper.h"

#if!defined(AFX_PROCESS_DSE_MARBLE)
#define AFX_PROCESS_DSE_MARBLE

/**监测站级实时报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY2

*返回值的字段码：
*		     测点名           FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT          20033
*/
int Srv_OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

///定义函数
/**得到所有振动测点的所有类型的特征值，通过循环依次取得各个测点的所有类型特征值，
  *输入参数值：
  *                 公司ID   FIELD_SYSTEM_COMPANY
  *			        分厂名   FIELD_SYSTEM_FACTORY
  *                 机组ID   FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *                 测点ID         FIELD_SERVICE_CHANN1
  *                 特征值结构体   FIELD_SERVICE_CHARVALUE_S
  *                 报警值         FIELD_SYSTEM_ALARM_LOW
  *                 危险值         FIELD_SYSTEM_ALARM_HIGH
  *                 单位           FIELD_SERVICE_CHANN1_UNIT
  *          指定机组测点的转速    FIELD_SERVICE_REV
  *            时间           FIELD_SERVICE_TIME
  *#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000
  */
int Service_OnGetAllFiveChar(ICommInterFace * pFixComm, IDBInterFace *pDB );

/**读取指定振动测点的五种类型的特征值。
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点buff       FIELD_SERVICE_CHANN_BUFF
*          测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数       FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          特征值结构体   FIELD_SERVICE_CHARVALUE_S
*          测点ID         FIELD_SERVICE_CHANN1
*          测点位号       FIELD_SYSTEM_CHANN_ALIAS
*          报警值         FIELD_SYSTEM_ALARM_LOW
*          危险值         FIELD_SYSTEM_ALARM_HIGH
*          单位           FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速    FIELD_SERVICE_REV
 *            特征值结构体   FIELD_SERVICE_CHARVALUE_S
 *            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR        20001
*/
int Service_OnGetSpecFiveChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**读取所有各种测点的特征值，通过循环依次取得各个测点的所有类型特征值。
*振动通道：使用振动类型的特征值结构体；FIELD_SERVICE_CHARVALUE_S
*过程量通道：使用特征值字段 FIELD_SERVICE_TREND_VALUE
*动态通道：使用动态类型的特征值结构体 FIELD_SERVICE_CHARVALUE_S
*每循环依次可得到一个测点指定趋势特征值
*输入参数值：机组ID          FIELD_SERVICE_PLANTID
*            公司ID          FIELD_SYSTEM_COMPANY
*			       分厂名          FIELD_SYSTEM_FACTORY
*            测点buff        FIELD_SERVICE_CHANN_BUFF
*            测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*            测点ID          FIELD_SERVICE_CHANN1
*            特征值          FIELD_SERVICE_TREND_VALUE
*        特征值结构体        FIELD_SERVICE_CHARVALUE_S
*            时间            FIELD_SERVICE_TIME
*          指定机组测点的转速    FIELD_SERVICE_REV
*          测点类型       FIELD_SYSTEM_CHANN_TYPE
*          报警限类型        FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002
*/
int Srv_OnGetAllTypeChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定任意多个振动测点的频谱。（测点之间“，”分开）。如果多个测点，就循环提取
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                           FIELD_SERVICE_CHANN1
*          测点的频谱数据                   FIELD_SERVICE_FREQWAVE1
*          测点实际频谱一组数据的个数       FIELD_SERVICE_FREQWAVE_NUM
*          振动测点频谱DF                   FIELD_SERVICE_FREQWAVE_DF
*          特征值结构体   FIELD_SERVICE_CHARVALUE_S
*          指定机组测点的转速               FIELD_SERVICE_REV
*          时间                             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECTRUM                20003
*/
int Srv_OnGetSpectrum(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**得到指定任意多个振动测点的指定类型特征值
  *输入参数：
  *          公司ID         FIELD_SYSTEM_COMPANY
  *			     分厂名         FIELD_SYSTEM_FACTORY
  *          机组ID         FIELD_SERVICE_PLANTID
  *          特征值类型     FIELD_SERVICE_TREND_CHARTYPE
  *          测点buff     FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数     FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          测点特征值         FIELD_SERVICE_TREND_VALUE
  *          测点ID             FIELD_SERVICE_CHANN1
  *            时间           FIELD_SERVICE_TIME
  #define     SERVICE_SERVICE_ON_GETFOURSPECCHAR        20004
  */
int SerVice_OnGetFourSpecChar(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组的指定过程量测点类型的所有过程量测点的值。注意：如果测点类型是振动测点，则不返回任何值。
*输入参数：机组ID         FIELD_SERVICE_PLANTID
*          公司ID         FIELD_SYSTEM_COMPANY
*			 分厂名         FIELD_SYSTEM_FACTORY
*          测点类型       FIELD_SERVICE_CHANNTYPE
*返回值的字段码：
*          过程量值       FIELD_SERVICE_TREND_PROCESS
*          测点ID         FIELD_SERVICE_CHANN1
*            时间           FIELD_SERVICE_TIME
  *          单位             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETALLPROCVALUE        20005
*/
int SerVice_OnGetAllProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定过程量测点的值。
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          过程量测点值     FIELD_SERVICE_TREND_PROCESS
*          测点ID           FIELD_SERVICE_CHANN1
*            时间           FIELD_SERVICE_TIME
*          单位             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_ON_GETSPECPROCVALUE       20006
*/
int SerVice_OnGetSpecProcValue(ICommInterFace* pFixComm,IDBInterFace *pDB);
/**得到指定两个振动测点的实时波形和频谱。（如何振动测点2不设置参数，则只提取一组）
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                                FIELD_SERVICE_CHANN1
*          测点位号                              FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据                        FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据                        FIELD_SERVICE_FREQWAVE1
*          测点实时波形上一组数据的个数          FIELD_SERVICE_TIMEWAVE_NUM
*          测点实际频谱一组数据的个数            FIELD_SERVICE_FREQWAVE_NUM
*          振动测点频谱DF                        FIELD_SERVICE_FREQWAVE_DF
*          所有特征值                            FIELD_SERVICE_CHARVALUE_S
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETWAVE                20007
*/
int Service_OnGetWave(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**得到某一组轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          轴心轨迹的垂直方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
*          垂直方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          轴心轨迹的水平方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          水平方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          水平报警值                       FIELD_SYSTEM_ALARM_LOW
*          水平危险值                       FIELD_SYSTEM_ALARM_HIGH
*          垂直报警值                       FIELD_SYSTEM_ALARM_LOW2
*          垂直危险值                       FIELD_SYSTEM_ALARM_HIGH2
*          水平方向测点位号                 FIELD_SYSTEM_CHANN_ALIAS
*          垂直方向测点位号                 FIELD_SYSTEM_CHANN2_ALIAS
*          单位                             FIELD_SERVICE_CHANN1_UNIT
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETAXESTRACK           20008
*/
int Service_OnGetAxesTrack(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组指定一组测点轴心位置值。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*		   分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          轴承位置描述     FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*返回值的字段码：
*                X方向振动测点位置  FIELD_SERVICE_AXESLOCATION_HOR
*                Y方向振动测点位置  FIELD_SERVICE_AXESLOCATION_VER
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009
*/
int Service_OnGetAxesLocation(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定公司和分厂机组的转速。如果指定机组只有一个转速，则只取FIELD_SERVICE_REV的值
*输入参数：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            设备号buff     FIELD_SERVICE_CHANN_BUFF
*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            设备号个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          机组ID          FIELD_SERVICE_PLANTID
*          指定机组的转速  FIELD_SERVICE_REV
*          指定机组的转速2 FIELD_SERVICE_OFF_REV2
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETREV                 20010
*/
int Service_OnGetRev(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组指定轴位移测点值
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			       分厂名   FIELD_SYSTEM_FACTORY
*            机组ID         FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*            轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
*            间隙电压       FIELD_SERVICE_TREND_GAP
*            直流量         FIELD_SERVICE_TREND_DC
*            测点ID         FIELD_SERVICE_CHANN1
*            测点位号       FIELD_SYSTEM_CHANN_ALIAS
*            报警值         FIELD_SYSTEM_ALARM_LOW
*            危险值         FIELD_SYSTEM_ALARM_HIGH
*            单位           FIELD_SERVICE_CHANN1_UNIT
*            指定机组测点的转速    FIELD_SERVICE_REV
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECAXISDISPLACE    20011
*/
int Service_OnGetSpecAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组所有轴位移测点值，通过循环依次得到所有轴位移值
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			   分厂名   FIELD_SYSTEM_FACTORY
*            机组ID   FIELD_SERVICE_PLANTID
*返回值的字段码：
*            轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
*            测点ID         FIELD_SERVICE_CHANN1
*          报警值         FIELD_SYSTEM_ALARM_LOW
*          危险值         FIELD_SYSTEM_ALARM_HIGH
*          单位           FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速    FIELD_SERVICE_REV
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012
*/
int Service_OnGetAllAxisDisplace(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到用户所观测机组范围内所有机组的测点运行状态，通过循环依次得到所有报警的测点,如果不需要通道信息则把FIELD_SERVICE_IFCHANN设为-1
*输入参数值：用户ID         FIELD_SYSTEM_USEID
*            是否需要通道   FIELD_SERVICE_IFCHANN
*返回值字段码：
*            公司ID             FIELD_SYSTEM_COMPANY
*			 分厂名             FIELD_SYSTEM_FACTORY
*            报警的机组ID       FIELD_SERVICE_PLANTID
*            报警的测点ID       FIELD_SERVICE_CHANN1
*            报警值             FIELD_SERVICE_CHANN1VALUE
#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
*/
int Service_OnGetAllChannelState(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组所有测点特征值，通过循环依次得到所有值。注：对于振动测点来说，指定那种特征类型就给那一种
*输入参数值：
*            公司ID         FIELD_SYSTEM_COMPANY
*			 分厂名         FIELD_SYSTEM_FACTORY
*            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
*            设备号buff     FIELD_SERVICE_CHANN_BUFF
*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            设备号个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*            设备ID         FIELD_SERVICE_PLANTID
*            测点ID         FIELD_SERVICE_CHANN1
*            测点值         FIELD_SERVICE_CHANN1VALUE
*            测点单位       FIELD_SERVICE_CHANN1_UNIT
*            测点所属列表   FIELD_SERVICE_CHANN1_LIST
*            测点的状态     FIELD_SERVICE_CHANN1_STATUS
*            时间           FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETALLCHANNVALUE       20014
*/
int Service_OnGetAllChannelValue(ICommInterFace* pFixComm,IDBInterFace *pDB);


/**得到指定任意个动态压力测点示功图信息。循环得到各个测点的数据
*输入参数值：
*            公司ID         FIELD_SYSTEM_COMPANY
*			       分厂名         FIELD_SYSTEM_FACTORY
*            机组ID         FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*            示功图类型     FIELD_SYSTEM_TYPE       P-V,P-a,P-t
*返回值的字段码：
*          测点号         FIELD_SERVICE_CHANN1
*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
*          波形数据         FIELD_SERVICE_TIMEWAVE1
*          X坐标数据        FIELD_SERVICE_TIMEWAVE2
*          所有特征值       FIELD_SERVICE_CHARVALUE_S
*          采样频率         FIELD_SERVICE_SAMPLE_FREQ
*          采样频率         FIELD_SERVICE_SAMPLE_NUM
*          报警值           FIELD_SYSTEM_ALARM_LOW
*          危险值           FIELD_SYSTEM_ALARM_HIGH
*          单位             FIELD_SERVICE_CHANN1_UNIT
*          指定测点的转速   FIELD_SERVICE_REV
*          时间             FIELD_SERVICE_TIME
#define   SERVICE_SERVICE_ON_GETPVDATA               20015
*/
int Srv_OnGetPVData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定任意多个沉降量测点的实时波形、频谱和所有特征值。（测点之间“，”分开）。如果多个测点，就循环提取
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点名                  FIELD_SERVICE_CHANN1
*          测点别名                FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据          FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据          FIELD_SERVICE_FREQWAVE1
*          测点波形数据的点数      FIELD_SERVICE_TIMEWAVE_NUM
*          测点频谱数据的点数      FIELD_SERVICE_FREQWAVE_NUM
*          振动测点频谱DF          FIELD_SERVICE_FREQWAVE_DF
*          所有特征值              FIELD_SERVICE_CHARVALUE_S
*          报警值                  FIELD_SYSTEM_ALARM_LOW
*          危险值                  FIELD_SYSTEM_ALARM_HIGH
*          单位                    FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速      FIELD_SERVICE_REV
*          时间                    FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
*/
int Srv_OnGetRodSinkData(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**通过分厂名得到下面设备的报警状态,并且得到指定机组的转速
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID  FIELD_SERVICE_PLANTID
*返回值字段码：
*            报警的机组ID         FIELD_SERVICE_PLANTID
*            指定设备的转速       FIELD_SERVICE_REV
*            指定机组的转速2      FIELD_SERVICE_OFF_REV2
#define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017
*/
int Service_OnGetSubCorpState(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定指定机组和指定可以做启停车的测点的波特图数据，一次只能得到一组数据，如果没有指定测点，则发送所有测点的数据
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*          测点号           FIELD_SERVICE_CHANN1
*          相位：           FIELD_SERVICE_TREND_ONECHAR
*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
*          转速：           FIELD_SERVICE_REV
*          时间             FIELD_SERVICE_TIME
*          对应时间的微秒   FIELD_SERVICE_MICROSECOND
*          开停车状态       FIELD_SERVICE_SUD_STATUS
*          开停车类型       FIELD_SERVICE_SUD_TYPE
*          启停车事件ID     FIELD_SYSTEM_EVENT_ID
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
#define     SERVICE_SERVICE_ON_GETBODE                20018
*/
int Service_OnGetBode(ICommInterFace* pFixComm,IDBInterFace *pDB);


/**得到指定指定机组和指定可以做启停车的测点瀑布图，一次只能得到一组波形频谱
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点号           FIELD_SERVICE_CHANN1
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*          相位：           FIELD_SERVICE_TREND_ONECHAR
*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
*          转速：           FIELD_SERVICE_REV
*          频谱数据         FIELD_SERVICE_FREQWAVE1
*          频谱数据个数     FIELD_SERVICE_FREQWAVE_NUM
*          频谱DF           FIELD_SERVICE_FREQWAVE_DF
*          时间             FIELD_SERVICE_TIME
*          对应时间的微秒   FIELD_SERVICE_MICROSECOND
*          开停车状态       FIELD_SERVICE_SUD_STATUS
*          开停车类型       FIELD_SERVICE_SUD_TYPE
*          启停车事件ID     FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_GETWATERFALL            20019
*/
int Service_OnGetWaterFall(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定机组振动测点的细化频谱分析和波形。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点             FIELD_SERVICE_CHANN1
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_TIMEWAVE1
*          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
*          波形数据个数     FIELD_SERVICE_TIMEWAVE_NUM
*          分析线数         FIELD_SERVICE_ZOOM_LINES
*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
*          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
*          状态             FIELD_SYSTEM_STATUS
*          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020
*/
int Service_OnGetZoomSpec(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**得到指定指定机组和测点的相关信息
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*			 分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点号           FIELD_SERVICE_CHANN1
*返回值的字段码：
*          额定转速：       FIELD_SERVICE_REV
*          实时转速：       FIELD_SERVICE_REV2
*          采样频率         FIELD_SERVICE_SAMPLE_FREQ
*          采样频率         FIELD_SERVICE_SAMPLE_NUM
#define     SERVICE_SERVICE_ON_GETSPECCHANNINFO           20021
*/
int Service_OnGetSpecChannInfo(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**读取指定设备振动测点的指定倍频的特征值和相位，
*如果指定测点了，就只取指定测点的特征值和相位
*如果没有指定测点，则获得所有测点通过循环依次取得各个测点的对应倍频特征值和相位，每循环依次可得到一个测点指定趋势特征值和相位
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID      FIELD_SERVICE_PLANTID
*            测点ID      FIELD_SERVICE_CHANN1
*            特征值类型  FIELD_SERVICE_TREND_CHARTYPE
*返回值的字段码：
*            测点ID      FIELD_SERVICE_CHANN1
*            指定特征值  FIELD_SERVICE_TREND_VALUE
*            对应的相位  FIELD_SERVICE_TREND_PHASE
*            转速：           FIELD_SERVICE_REV
#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022
*/
int Service_OnGetAllSpecCharAndPhase(ICommInterFace* pFixComm,IDBInterFace* pDB);



/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其波形和特征参数
*如果测点为空就不会返回对应的数据
*输入参数值：
*          公司ID                     FIELD_SYSTEM_COMPANY
*	       分厂名                     FIELD_SYSTEM_FACTORY
*          机组ID                     FIELD_SERVICE_PLANTID
*          振动测点buff               FIELD_SERVICE_CHANN_BUFF
*          振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*          振动测点个数               FIELD_SERVICE_CHANN_NUM
*          动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*          动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*          动态测点个数               FIELD_SERVICE_CHANN_NUM2
*          活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
*          活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*          活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
*返回字段：
*          波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
*          采样频率                   FIELD_SERVICE_SAMPLE_FREQ
*          测点名称                   FIELD_SERVICE_CHANN
*          测点波形                   FIELD_SERVICE_TIMEWAVE
*          测点特征值                 FIELD_SERVICE_CHARVALUE_S
*          横坐标                     FIELD_SERVICE_FREQWAVE2
*          转速                       FIELD_SERVICE_REV
*          时间                       FIELD_SERVICE_TIME
*          吸气阀开启相位角           FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角           FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
*/
int Srv_OnGetRcMultiParamData(ICommInterFace* pFixComm,IDBInterFace* pDB);


/**读取集团级实时报警统计数据
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名          FIELD_SYSTEM_FACTORY
*返回值的字段码：
*		     设备数          FIELD_SERVICE_PLANT_NUM
*		     报警设备数      FIELD_SERVICE_ALARM_PLANTNUM
*		     运行设备数      FIELD_SERVICE_RUN_PLANTNUM
*		     停车设备数      FIELD_SERVICE_STOP_PLANTNUM
#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
*/
int Srv_OnGetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**读取公司级实时报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*返回值的字段码：
*		     分厂名           FIELD_SYSTEM_FACTORY
*		     设备名           FIELD_SERVICE_PLANTID
*		     测点数           FIELD_SERVICE_CHANN_NUM
*		     报警状态         FIELD_SERVICE_ALARM_STATUS
*		     启停车状态       FIELD_SERVICE_SUD_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识    FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT         20025
*/
int Srv_OnGetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**监测站级实时报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*返回值的字段码：
*		     测点名           FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC
#define     SERVICE_SERVICE_ON_MONITORALARMSTAT          20026
*/
int Srv_OnGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定机组指定一组测点的活塞杆轴心位置轨迹
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		   分厂名            FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          轴承位置描述      FIELD_SYSTEM_AXISLOCATION
*          水平方向测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数          FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          水平方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          垂直方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_VER
*          实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
*          水平方向测点特征值  FIELD_SERVICE_CHARVALUE_S
*          垂直方向测点特征值  FIELD_SERVICE_CHARVALUE_S2
*          时间                FIELD_SERVICE_TIME
*		   转速1               FIELD_SERVICE_REV
#define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION         20027
*/
int Srv_OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**往复式系统应力监测，得到指定机组指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		  分厂名          　　　  FIELD_SYSTEM_FACTORY
*          机组ID          　　　  FIELD_SERVICE_PLANTID
*          汽缸号                  FIELD_SERVICE_CYLINDERNO
*          缸体振动测点            FIELD_SERVICE_CHANN1
*返回值的字段码：
*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*          振动波形数据   　　　　 FIELD_SERVICE_FREQWAVE2
*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_ON_GETDYNSTRESSWAVE              20028
*/
int Srv_OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到某一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		  分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          水平方向特征值                 FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                 FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
*/
int Srv_OnGetPurifyAxesTrack(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定振动测点的实时波形和倒谱。（如何振动测点2不设置参数，则只提取一组）
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*		  分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点buff         FIELD_SERVICE_CHANN_BUFF
*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数         FIELD_SERVICE_CHANN_NUM
*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          测点名                                FIELD_SERVICE_CHANN1
*          测点位号                              FIELD_SYSTEM_CHANN_ALIAS
*          测点的波形数据                        FIELD_SERVICE_TIMEWAVE1
*          测点的倒谱数据                        FIELD_SERVICE_FREQWAVE1
*          测点实时波形上一组数据的个数          FIELD_SERVICE_TIMEWAVE_NUM
*          测点实际倒谱一组数据的个数            FIELD_SERVICE_FREQWAVE_NUM
*          振动测点倒谱DF                        FIELD_SERVICE_FREQWAVE_DF
*          所有特征值                            FIELD_SERVICE_CHARVALUE_S
*          报警值                                FIELD_SYSTEM_ALARM_LOW
*          危险值                                FIELD_SYSTEM_ALARM_HIGH
*          单位                                  FIELD_SERVICE_CHANN1_UNIT
*          指定机组测点的转速                    FIELD_SERVICE_REV
*          时间                                  FIELD_SERVICE_TIME
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_ON_GETCEPSTRUM          20030
*/
int Srv_OnGetCepstrum(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定一对振动测点的实时全谱和全谱瀑布图。
*输入参数：
*          公司ID                           FIELD_SYSTEM_COMPANY
*		  分厂名                           FIELD_SYSTEM_FACTORY
*          机组ID                           FIELD_SERVICE_PLANTID
*          轴承位置描述                     FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID               FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID               FIELD_SERVICE_CHANNID_VER
*          波形点数                         FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          时间                             FIELD_SERVICE_TIME
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031
*/
int Srv_OnGetFullSpectrum(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*********************************************************************/

/**发送指定机组的振动测点实时所有特征值、轴位置数据到中间件模块
  *输入参数字段码：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID   FIELD_SERVICE_PLANTID
*            振动测点通频特征值数据          FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            采样频率                        FIELD_SERVICE_SAMPLE_FREQ
*            采样点数                        FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速                      FIELD_SERVICE_REV
*            机组的转速2                     FIELD_SERVICE_REV2
*            机组的转速3                     FIELD_SERVICE_REV3
*            机组的转速4                     FIELD_SERVICE_REV4
*            机组的转速5                     FIELD_SERVICE_REV5
*            周期起始通道索引                FIELD_SERVICE_STARTINDEX
*            周期结束通道索引                FIELD_SERVICE_ENDINDEX
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*            是否需要判断报警                FIELD_SERVICE_JUDGE_ALARM
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID
*            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
*            对应的间隙电压          FIELD_SERVICE_TREND_GAP
*            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
*            学习参数结构体的长度    FIELD_SERVICE_LEN
#define     SERVICE_SERVICE_ON_RECEIVEALLVALUE        24000
*/
int Service_OnReceiveVibValue(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			       分厂名           FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
*            是否这次更新数据完成    FIELD_SERVICE_SEND_END
*            是否需要判断报警                FIELD_SERVICE_JUDGE_ALARM
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001
*/
int Srv_OnReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送转速到中间件模块
  *输入参数字段码：
  *                公司ID   FIELD_SYSTEM_COMPANY
  *	    		   分厂名   FIELD_SYSTEM_FACTORY
  *                机组ID      FIELD_SERVICE_PLANTID
  *                机组的转速  FIELD_SERVICE_REV
  *                机组的转速2 FIELD_SERVICE_REV2
  *返回值的字段码：无
 #define     SERVICE_SERVICE_ON_RECEIVEREV             24002
 */
int Service_OnReceiveRev(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*                分厂名   FIELD_SYSTEM_FACTORY
*                机组ID      FIELD_SERVICE_PLANTID
*                起始索引          FIELD_SERVICE_ON_SEND_STARTID
*                测点类型          FIELD_SERVICE_CHANNTYPE
*                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            是否需要判断报警                FIELD_SERVICE_JUDGE_ALARM
*返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
*                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
#define     SERVICE_SERVICE_ON_RECEIVEPROC            24003
*/
int Service_OnReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			       分厂名   FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID
*返回值的字段码：无
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24004
*/
int Service_OnReceiveChannState(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			       分厂名   FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
*            报警EVENTID                     FIELD_SERVICE_ALARMID
*返回值的字段码：无
#define     SERVICE_SERVICE_ON_RECEIVECHANNSTATE      24027
*/
int Service_OnReceiveChannStateWithAlarmLevel(ICommInterFace *pFixComm,IDBInterFace * pDB);
/**发送指定指定设备的启停车数据到中间件
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			  分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             起始索引        FIELD_SERVICE_ON_SEND_STARTID
*             启停车标志位    FIELD_SERVICE_SUD_STATUS
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
*             指定机组的转速  FIELD_SERVICE_REV
*             指定机组的转速2 FIELD_SERVICE_REV2
*             特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数        FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据        FIELD_SERVICE_ON_SEND_VIBWAVE
*             一组波形点数     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             采样频率        FIELD_SERVICE_SAMPLE_FREQ
*             采样点数        FIELD_SERVICE_SAMPLE_NUM
*            对应的时间       FIELD_SERVICE_TIME
*            对应时间的微秒   FIELD_SERVICE_MICROSECOND
*             启停车类型     FIELD_SERVICE_SUD_TYPE
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_REVSUDDATA            24005
*/
int Service_OnRevSudData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**通知中间件服务器，需要保存数据
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*返回值字段码：无
#define     SERVICE_SERVICE_ON_SAVEDATA              24006
*/
int Service_OnSrvSaveData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**通知中间件服务器，需要判断报警
*输入参数字段码：
*            公司ID   FIELD_SYSTEM_COMPANY
*			   分厂名   FIELD_SYSTEM_FACTORY
*返回值字段码：无
#define     SERVICE_SERVICE_ON_JUDGEALARM            24007
*/
int Service_OnSrvJudgeAlarm(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**申请阈值ID，如果数组长度小于1表示失败
*输入参数字段码
*           公司ID          FIELD_SYSTEM_COMPANY
*			      分厂名          FIELD_SYSTEM_FACTORY
*           机组ID：        FIELD_SERVICE_PLANTID
*           通道个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
*           数组长度（通道个数） FIELD_SERVICE_CHANN_NUM
#define     SERVICE_SERVICE_ON_REQTHESHOLDID       24011
*/
int Service_OnReqThresholdID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**申请指定公司、分厂、设备的快变报警事件ID
*输入参数字段码：
*             公司ID         FIELD_SYSTEM_COMPANY
*			  分厂名         FIELD_SYSTEM_FACTORY
*             机组ID：       FIELD_SERVICE_PLANTID
*返回值的字段码：
*             快变报警事件ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012
*/
int Srv_OnReqFastAlarmEventID(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**更新指定公司、分厂、设备的振动测点快变报警状态
*输入参数字段码：
*             公司ID            FIELD_SYSTEM_COMPANY
*			        分厂名            FIELD_SYSTEM_FACTORY
*             机组ID：          FIELD_SERVICE_PLANTID
*             通道索引          FIELD_SERVICE_ON_SEND_STARTID
*             振动的报警状态    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*             振动通道个数      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*返回值的字段码：
*             成功标识            FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS     24013
*/
int Srv_OnUpdateFastAlarmStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE        24021
*/
int Srv_NetOffReceiveAllValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送指定机组的动态测点断网后保存的所有特征值波形数据到中间件模块
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			 分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                 FIELD_SERVICE_TIME
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE        24022
*/
int Srv_NetOffReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送断网后保存的ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID            FIELD_SYSTEM_COMPANY
*                分厂名            FIELD_SYSTEM_FACTORY
*                机组ID            FIELD_SERVICE_PLANTID
*                起始索引          FIELD_SERVICE_ON_SEND_STARTID
*                测点类型          FIELD_SERVICE_CHANNTYPE
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*                时间值            FIELD_SERVICE_TIME
*返回值的字段码：
*                测点个数          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEPROC            24023
*/
int Srv_NetOffReceiveProc(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
*输入参数字段码：
*            公司ID                          FIELD_SYSTEM_COMPANY
*			 分厂名                          FIELD_SYSTEM_FACTORY
*            机组ID                          FIELD_SERVICE_PLANTID
*            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            时间值                          FIELD_SERVICE_TIME
*            时间值                          FIELD_SERVICE_MICROSECOND
*            是否这次更新数据完成            FIELD_SERVICE_SEND_END
*返回值的字段码：
*            测点个数                 FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE         24024
*/

int  Srv_NetOffReceiveAllWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/***-------------------------------------------------------------****/

/**发送ALLPROC过程量到中间件模块，并得到指定类型的数据
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*			       分厂名   FIELD_SYSTEM_FACTORY
*                机组ID            FIELD_SERVICE_PLANTID
*返回值的字段码：
*                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
*                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA        24051
*/
int Service_OnGetSpecPlantProcData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**发送指定机组的报警状态，为报警状态的结构体
*输入参数字段码：
*                公司ID   FIELD_SYSTEM_COMPANY
*			           分厂名   FIELD_SYSTEM_FACTORY
*                机组ID   FIELD_SERVICE_PLANTID
*返回值的字段码：
*            轴心位置状态                    FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
*            轴心位置个数                    FIELD_SERVICE_ON_SEND_AXISCHANNNUM
*            过程量状态                      FIELD_SERVICE_ON_SEND_ALLPROCSTATE
*            过程量个数                      FIELD_SERVICE_ON_SEND_PROCCHANNNUM
*            振动测点状态                    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
*            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            动态测点状态                    FIELD_SERVICE_ON_SEND_ALLDYNSTATE
*            动态测点个数                    FIELD_SERVICE_ON_SEND_DYNCHANNNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE       24052
*/
int Service_OnGetSpecPlantAllState(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**根据指定设备得到指定索引和长度的振动波形数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*			 分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            振动测点特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引                FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数            FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据                FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数            FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            数据的时间              FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA    24050
*/
int Service_OnGetSpecPlantLengthVibData(ICommInterFace *pFixComm,IDBInterFace * pDB);

/**根据指定设备指定索引和长度的动态数据
*输入参数值：
*            公司ID   FIELD_SYSTEM_COMPANY
*		     分厂名   FIELD_SYSTEM_FACTORY
*            机组ID    FIELD_SERVICE_PLANTID
*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            目前数据的时间 FIELD_SERVICE_TIME
*返回参数值：
*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
#define     SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA    24054
*/
int Service_OnGetSpecPlantLengthDynData(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**得到监测中心的设备信息表
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SYSTEM_PLANT
*返回的字段码：
*             测点号     FIELD_SYSTEM_CHANN
*             索引       FIELD_SYSTEM_STATUS
*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*             单位       FIELD_SERVICE_CHANN1_UNIT
  #define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
 */
int Service_SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB);


/* 获取异常数据检测结果 */
int Service_OnGetAbnormalDataRecord(ICommInterFace *pFixComm,IDBInterFace * pDB);
/* 接收数采硬件状态 */
int Service_OnRcvDAQHWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

/* 接收数采软件状态 */
int Service_OnRcvDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace * pDB);

int Service_SysGetSpecTypeChann(ICommInterFace *pFixComm,IDBInterFace * pDB);


/**获得指定机组的振动通道的组态信息
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
			   振动测点个数          FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *返回值的字段码：
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  
  #define     SERVICE_SERVICE_ON_GET_VIBINFO        24066

  */


int Srv_GetVibInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);



/**获得指定机组的动态通道的组态信息
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
			   振动测点个数          FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *返回值的字段码：
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *				#define     SERVICE_SERVICE_ON_GET_DYNINFO        24067
  */

int Srv_GetDynInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);

/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *				#define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */

int Srv_Alarm_Confirm(ICommInterFace * pFixComm,IDBInterFace * pDB);


/**启停车过程发送指定机组的振动测点实时所有振动特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *            对应的时间             FIELD_SERVICE_TIME
  *            对应时间的微秒         FIELD_SERVICE_MICROSECOND
  *            启停车类型             FIELD_SERVICE_SUD_TYPE
  *            启停车标志位           FIELD_SERVICE_SUD_STATUS
  *            启停车事件ID:          FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
  *            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
  * #define     SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE        24100
  */
	int ReceiveSudVibValue(ICommInterFace * pFixComm,IDBInterFace *pDB);

	/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
	*输入参数字段码：
	*            公司ID                 FIELD_SYSTEM_COMPANY
	*			       分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID                 FIELD_SERVICE_PLANTID
	*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
	*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
	*            采样点数               FIELD_SERVICE_SAMPLE_NUM
	*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            机组的转速             FIELD_SERVICE_REV
	*            机组的转速2            FIELD_SERVICE_REV2
	*            机组的转速3            FIELD_SERVICE_REV3
	*            机组的转速4            FIELD_SERVICE_REV4
	*            机组的转速5            FIELD_SERVICE_REV5
	*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
	*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
	*            是否这次更新数据完成    FIELD_SERVICE_SEND_END
	*            对应的时间             FIELD_SERVICE_TIME
	*            对应时间的微秒         FIELD_SERVICE_MICROSECOND
	*            启停车类型             FIELD_SERVICE_SUD_TYPE
	*            启停车标志位           FIELD_SERVICE_SUD_STATUS
	*            启停车事件ID:          FIELD_SYSTEM_EVENT_ID
	*返回值的字段码：
	*            测点个数                FIELD_SYSTEM_STATUS
	* #define     SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE        24101
	*/
   int ReceiveSudDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
   *输入参数字段码：
   *                公司ID   FIELD_SYSTEM_COMPANY
   *                分厂名   FIELD_SYSTEM_FACTORY
   *                机组ID      FIELD_SERVICE_PLANTID
   *                起始索引          FIELD_SERVICE_ON_SEND_STARTID
   *                测点类型          FIELD_SERVICE_CHANNTYPE
   *                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
   *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
   *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
   *返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
   *                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
   *                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
   *                对应的时间             FIELD_SERVICE_TIME
   *                对应时间的微秒         FIELD_SERVICE_MICROSECOND
   *                启停车类型             FIELD_SERVICE_SUD_TYPE
   *                启停车标志位           FIELD_SERVICE_SUD_STATUS
   *                启停车事件ID:          FIELD_SYSTEM_EVENT_ID
   *    #define     SERVICE_SERVICE_ON_RECEIVEPROC            24103
   */
   int ReceiveSudProcValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送指定机组振动快变数据到中间件，并得到报警状态
	*输入参数字段码：
	*            公司ID                 FIELD_SYSTEM_COMPANY
	*			   分厂名                 FIELD_SYSTEM_FACTORY
	*            机组ID                 FIELD_SERVICE_PLANTID
	*            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
	*            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
	*            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
	*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
	*            采样点数               FIELD_SERVICE_SAMPLE_NUM
	*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            机组的转速             FIELD_SERVICE_REV
	*            机组的转速2            FIELD_SERVICE_REV2
	*            机组的转速3            FIELD_SERVICE_REV3
	*            机组的转速4            FIELD_SERVICE_REV4
	*            机组的转速5            FIELD_SERVICE_REV5
	*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
	*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*返回值的字段码：
	*            报警状态                FIELD_SERVICE_ALARM_STATUS
	*            报警事件ID              FIELD_SYSTEM_EVENT_ID
    #define     SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE        24104
	*/
   int ReceiveVibFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送指定机组的动态测点实时快变数据到中间件模块，并得到报警状态和报警事件ID
	*输入参数字段码：
	*            公司ID                 FIELD_SYSTEM_COMPANY
	*			 分厂名                 FIELD_SYSTEM_FACTORY
	*            机组ID                 FIELD_SERVICE_PLANTID
	*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
	*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
	*            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE  
	*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
	*            采样点数               FIELD_SERVICE_SAMPLE_NUM
	*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*            机组的转速             FIELD_SERVICE_REV
	*            机组的转速2            FIELD_SERVICE_REV2
	*            机组的转速3            FIELD_SERVICE_REV3
	*            机组的转速4            FIELD_SERVICE_REV4
	*            机组的转速5            FIELD_SERVICE_REV5
	*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
	*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION　
	*返回值的字段码：
	*            报警状态                FIELD_SERVICE_ALARM_STATUS
	*            报警事件ID              FIELD_SYSTEM_EVENT_ID
	#define     SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE        24105
	*/
   int ReceiveDynFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送ALLPROC过程量或轴位移快变数据到中间件模块，并得到指定类型的数据
  *输入参数字段码：
  *           公司ID   FIELD_SYSTEM_COMPANY
  *			  分厂名   FIELD_SYSTEM_FACTORY
  *           机组ID      FIELD_SERVICE_PLANTID
  *           起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *           测点类型          FIELD_SERVICE_CHANNTYPE
  *           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *返回值的字段码：
  *           报警状态                FIELD_SERVICE_ALARM_STATUS
  *           报警事件ID              FIELD_SYSTEM_EVENT_ID
 
  #define     SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE        24106 
  */
   int ReceiveProcFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送ALLPROC过程量或轴位移快变数据到中间件模块，并得到报警状态和报警事件ID
  *输入参数字段码：
  *           公司ID            FIELD_SYSTEM_COMPANY
  *			  分厂名            FIELD_SYSTEM_FACTORY
  *           机组ID            FIELD_SERVICE_PLANTID
  *           起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *           测点类型          FIELD_SERVICE_CHANNTYPE
  *           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *           报警事件ID        FIELD_SYSTEM_EVENT_ID
  *           对应的时间        FIELD_SERVICE_TIME
  *           转速              FIELD_SERVICE_REV
  *返回值的字段码：
  *           成功标识          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_ON_RECPROCFASTALARMDATA           24107
*/
int Srv_OnReceiveProcFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送振动快变报警数据到中间件保存
  *输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             快变报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速   FIELD_SERVICE_REV
	*             指定机组的转速2  FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
	*             指定机组的转速4  FIELD_SERVICE_REV4
	*             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*            自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*            间隙电压          FIELD_SERVICE_TREND_GAP  
	*            成功标识      FIELD_SYSTEM_STATUS
 
	#define     SERVICE_SERVICE_ON_RECVIBFASTALARMDATA        24108 */
int Srv_OnReceiveVibFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送动态快变报警数据到中间件保存
*输入参数字段码：
*             公司ID              FIELD_SYSTEM_COMPANY
*			  分厂名              FIELD_SYSTEM_FACTORY
*             机组ID：            FIELD_SERVICE_PLANTID
*             起始索引            FIELD_SERVICE_ON_SEND_STARTID
*             报警类型数组        FIELD_SERVICE_ALARM_TYPE
*             动态报警事件ID      FIELD_SYSTEM_EVENT_ID
*             特征值数据		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据            FIELD_SERVICE_ON_SEND_VIBWAVE
*             总的波形点数        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
*             自定义参数模板ID    FIELD_SYSTEM_TEMPID
*             对应的时间          FIELD_SERVICE_TIME
*             对应时间的微秒      FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*             压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
*             间隙电压          FIELD_SERVICE_TREND_GAP  

#define     SERVICE_SERVICE_ON_RECDYNFASTALARMDATA          24109*/
int Srv_OnReceiveDynFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送指定机组振动无线采集数据到中间件
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			   分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
*            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
*            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            机组的转速3            FIELD_SERVICE_REV3
*            机组的转速4            FIELD_SERVICE_REV4
*            机组的转速5            FIELD_SERVICE_REV5
*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*返回值的字段码：：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID

#define     SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE        24115
*/
int Srv_OnReceiveVibWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


		/*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          测点类型    FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *		     是否修改成功：  FIELD_SYSTEM_STATUS
  *#define    SERVICE_SERVICE_ON_SAVESPECTIMEDATA    21080
  */
int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);




   int Srv_test(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**得到指定指定机组由过程量判断的机组开停车状态
   *输入参数：
   *          公司ID           FIELD_SYSTEM_COMPANY
   *		   分厂名           FIELD_SYSTEM_FACTORY
   *          机组ID           FIELD_SERVICE_PLANTID
   *返回值的字段码：
   *          相位：           FIELD_SERVICE_TREND_ONECHAR
   #define    SERVICE_SYSTEM_QUERY_PROC_RUNSTATUS           20043
   */
   int Service_QueryProcRunstatus(ICommInterFace* pFixComm,IDBInterFace *pDB);

  /**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
	*输入参数：  
   *         公司名         FIELD_SYSTEM_COMPANY
   *         分厂名         FIELD_SYSTEM_FACTORY
   *         机组名         FIELD_SYSTEM_PLANT
   *         测点名         FIELD_SYSTEM_CHANN
   *         测点类型：     FIELD_SERVICE_CHANNTYPE
   *         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
   *         报警限类型     FIELD_SYSTEM_ALARM_TYPE
   *         报警上限           FIELD_SYSTEM_ALARM_HIGH
   *         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
   *         报警下限           FIELD_SYSTEM_ALARM_LOW
   *         报警下下限         FIELD_SYSTEM_ALARM_LOW2
   *         修改报警限主机名   FIELD_SYSTEM_HOST_NAME
   *         修改报警限主机IP   FIELD_SYSTEM_CLIENT_IP
   *         修改报警限用户名   FIELD_SYSTEM_USER_NAME
   *返回值的字段码：
   *         返回状态       FIELD_SYSTEM_STATUS
   #define   SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION 20044
   */
  int ChangeSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**获取指定设备指定测点的报警信息
  *输入参数：  
  *         公司名         FIELD_SYSTEM_COMPANY
  *         分厂名         FIELD_SYSTEM_FACTORY
  *         机组名         FIELD_SERVICE_PLANTID
  *         测点名         FIELD_SYSTEM_CHANN
  *         测点类型：     FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
  *         报警限类型     FIELD_SYSTEM_ALARM_TYPE
  *         报警上限           FIELD_SYSTEM_ALARM_HIGH
  *         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
  *         报警下限           FIELD_SYSTEM_ALARM_LOW
  *         报警下下限         FIELD_SYSTEM_ALARM_LOW2
  *         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
  #define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045
  */
  int GetSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**中间件中获取概貌图信息接口
  *输入参数：  
  *         公司名         FIELD_SYSTEM_COMPANY
  *         分厂名         FIELD_SYSTEM_FACTORY
  *         机组名         FIELD_SERVICE_PLANTID
  *         测点名         FIELD_SYSTEM_CHANN
  *         测点类型：     FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
  *         报警限类型     FIELD_SYSTEM_ALARM_TYPE
  *         报警上限       FIELD_SYSTEM_ALARM_LOW
  *         报警上上限     FIELD_SYSTEM_ALARM_HIGH
  *         报警下限       FIELD_SYSTEM_ALARM_LOW2
  *         报警下下限     FIELD_SYSTEM_ALARM_HIGH2
  *         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
  #define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION 20048
  */
  int GetPlantGraphInformationS( ICommInterFace * pFixComm, IDBInterFace *pDB );

  /**中间件中获取部位号信息接口
  *输入参数：  
  *         机组概貌图                 FIELD_SERVICE_BHCLIENT_GROUPHINDEX
  *         机组概貌图类型ID：         FIELD_SYSTEM_PLANT_GRAPHTYPE
  *         
  *返回值的字段码：
  *         客户端的部位号             FIELD_SERVICE_GROUPH_ACCESS_NUMBER
  *         
  *         低报low_alarm_xoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_XOFFSET
  *         低报low_alarm_yoffset      FIELD_SERVICE_GROUPH_LOW_ALARM_YOFFSET
  *         高报low_alarm_xoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_XOFFSET
  *         高报low_alarm_yoffset      FIELD_SERVICE_GROUPH_HIG_ALARM_YOFFSET
  *
  *         低报图片buffer大小         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
  *         低报图片buffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
  *         高报图片buffer大小         FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE
  *         高报图片buffer             FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF
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

  /**设置无线测点状态
  *输入参数：  
  *         公司名         FIELD_SYSTEM_COMPANY
  *         分厂名         FIELD_SYSTEM_FACTORY
  *         机组名         FIELD_SERVICE_PLANTID
  *         测点类型       FIELD_SERVICE_CHANNTYPE
  *         起始索引       FIELD_SERVICE_ON_SEND_STARTID
  *         测点个数       FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *         测点状态       FIELD_SYSTEM_WIRELESS_CHANN_STATUS
  *返回值的字段码：
  *         返回状态       FIELD_SYSTEM_STATUS //测点个数
  *         索引           FIELD_SERVICE_ON_SEND_STARTID
  #define   SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS 20046
  */
  int SetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**获取指定设备的无线测点状态
  *输入参数：  
  *         公司名         FIELD_SYSTEM_COMPANY
  *         分厂名         FIELD_SYSTEM_FACTORY
  *         机组名         FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *         测点名         FIELD_SYSTEM_CHANN
  *         是否在线       FIELD_SYSTEM_ONLINE 
  *         是否正常       FIELD_SYSTEM_NORMAL
  *         数据更新状态   FIELD_SYSTEM_DATAUPDATE
  *         电池电量       FIELD_SYSTEM_BATTERY_POWER
 
  #define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047
  */
  int GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);
  //获取

  /*!
  *输入参数字段码
  *
  *返回值的字段码：
  *          数据采集时间：       FIELD_SERVICE_TIME

  #define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
  */

  int GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /*!
  *输入参数字段码
  *
  *返回值的字段码：
  *          通知状态：  FIELD_SYSTEM_STATUS
  #define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
  */
  int NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB);

  /**启停后处理数据，发送指定机组的启停车振动测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_VIB_DATA                         24210
	*/
   int Srv_OnReceiveSudVibData(ICommInterFace* pFixComm,IDBInterFace *pDB);

	/**启停后处理数据，发送指定机组的启停车动态测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             动态测点个数         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_DYN_DATA                         24211
	*/ 
   int Srv_OnReceiveSudDynData(ICommInterFace* pFixComm,IDBInterFace *pDB);

   /**启停后处理数据，发送指定机组的启停车过程量测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID               FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             过程量个数           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             报警事件ID           FIELD_SYSTEM_EVENT_ID
	*             报警类型             FIELD_SERVICE_SUD_TYPE
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             过程量特征值         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_PROC_DATA                        24212
	*/

   int Srv_OnReceiveSudProcData(ICommInterFace* pFixComm,IDBInterFace *pDB);


     /**中间件中获取报警文件信息
   *输入参数：  
   *         文件路径                   FIELD_SERVICE_FILE_PATH
   *         文件名                     FIELD_SERVICE_FILE_NAME
   *         
   *返回值的字段码：
   *         文件名                     FIELD_SERVICE_FILE_NAME
   *         文件内容                   FIELD_SERVICE_FILE_CONTENT
   *         
   
	#define   SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT 20050
	*/
   int Srv_OnGetLogFileContent(ICommInterFace* pFixComm,IDBInterFace *pDB);

   /**
  根据设备测点得到分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *          一级门限        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          二级门限        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          三级门限        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *          状态代码        FIELD_SYSTEM_STATUS
  *
 
	#define    SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD         20051
  */
  int Srv_OnGetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);
/**
  根据设备测点设置分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          一级门限        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          二级门限        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          三级门限        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  
	#define    SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD         20052
*/
  int Srv_OnSetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);

 /**
  根据设备测点得到轴心位置配置信息
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  * #define    SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM         20053
  */

  int Srv_OnGetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**
  根据设备测点设置分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  * #define    SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM         20054
  */

  int Srv_OnSetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

#endif //AFX_PROCESS_DSE_MARBLE
