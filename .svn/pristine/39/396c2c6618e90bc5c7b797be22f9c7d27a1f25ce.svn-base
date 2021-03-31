#if !defined(AFX_PROCESS)
#define AFX_PROCESS
#include "additions/BHOleDbWrapper.h"
//定义函数

/**得到当前时间从向后推一个趋势类型的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*@param：  输入参数字段码
*          公司ID       FIELD_SYSTEM_COMPANY
*		   分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          趋势类型     FIELD_SERVICE_OFF_TRENDTYPE
*          特征值类型   FIELD_SERVICE_TREND_CHARTYPE
*          测点类型：   FIELD_SERVICE_OFF_CHANNTYPE
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
*@return  返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*define     SERVICE_SERVICE_OFF_GETCURTREND           21001
*/
int Service_OffGetCurTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂机组和测点指定时刻的频谱数据
  *输入参数：
  *          公司ID       FIELD_SYSTEM_COMPANY
  *    		 分厂名       FIELD_SYSTEM_FACTORY
  *          机组ID       FIELD_SERVICE_PLANTID
  *          测点ID       FIELD_SERVICE_OFF_CHANN1
  *          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
  *          分析线数：   FIELD_SERVICE_OFF_FREQANALYSIS_LINES
  *          时间         FIELD_SERVICE_OFF_TIME
  *返回值的字段码：
  *          波形数据        FIELD_SERVICE_OFF_WAVE
  *          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
  *          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
  *          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
  *          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
  *          特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *          报警值          FIELD_SYSTEM_ALARM_LOW
  *          危险值          FIELD_SYSTEM_ALARM_HIGH
  *          单位            FIELD_SERVICE_CHANN1_UNIT
  *          测点别名        FIELD_SYSTEM_CHANN_ALIAS
  *          转速            FIELD_SERVICE_OFF_REV
  *define     SERVICE_SERVICE_OFF_GETSPECFREQWAVE       21002
  */
int Service_OffGetSpecFreqWave(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定时间段、特征值类型、任意多个测点的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          特征值类型   FIELD_SERVICE_TREND_CHARTYPE
*          测点类型：   FIELD_SERVICE_OFF_CHANNTYPE
*          公司ID       FIELD_SYSTEM_COMPANY
*		   分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
*          起始时间     FIELD_SERVICE_OFF_TIMESTART
*          终止时间     FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          测点单位             FIELD_SERVICE_CHANN1_UNIT
*define     SERVICE_SERVICE_OFF_GETTREND      21003
*/
int Service_OffGetTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂、机组号、振动测点号、起始时间段的所有特征值趋势数据
*输入参数字段码：
*          公司ID      FIELD_SYSTEM_COMPANY
*	    		 分厂名      FIELD_SYSTEM_FACTORY
*          机组ID      FIELD_SERVICE_PLANTID
*          测点类型：  FIELD_SERVICE_CHANNTYPE
*          测点ID      FIELD_SERVICE_OFF_CHANN1
*          起始时间    FIELD_SERVICE_OFF_TIMESTART
*          终止时间    FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          时间             FIELD_SERVICE_OFF_TIME
  #define    SERVICE_SERVICE_OFF_GETAVIBCHANNTREND      21004
  */
int Service_OffGetAVibChannTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂名、机组ID、时间段、水平测点ID和垂直测点ID的轴心位置历史数据，通过循环依次得到所有数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID   FIELD_SYSTEM_COMPANY
*   	   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID      FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          起始时间    FIELD_SERVICE_OFF_TIMESTART
*          终止时间    FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          水平方向振动测点数据  FIELD_SERVICE_AXESLOCATION_HOR
*          垂直方向振动测点数据  FIELD_SERVICE_AXESLOCATION_VER
*          时间值                FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND  21005
  */
int Service_OffGetAxisLocationTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定两个振动测点的特征值信息、报警限等信息（循环两次得到两个测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*			     分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一          FIELD_SERVICE_OFF_CHANN1
*          测点一时间值    FIELD_SERVICE_OFF_TIME
*          公司ID2         FIELD_SYSTEM_COMPANY2
*			     分厂名2         FIELD_SYSTEM_FACTORY2
*          机组ID2         FIELD_SERVICE_PLANTID2
*          测点二          FIELD_SERVICE_OFF_CHANN2
*          测点二时间值    FIELD_SERVICE_OFF_TIME2
*返回值的字段码：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO    21006
*/
int Srv_OffGetTwoChannDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定历史时刻、机组振动测点的细化频谱分析和波形。
  *输入参数：机组ID           FIELD_SERVICE_PLANTID
  *          测点             FIELD_SERVICE_OFF_CHANN1
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
  *          时间             FIELD_SERVICE_OFF_TIME
  *返回值的字段码：
  *          测点的波形数据   FIELD_SERVICE_OFF_WAVE
  *          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
  *          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
  *          通频特征值：     FIELD_SERVICE_TREND_OVERALLCHAR
  *          状态             FIELD_SYSTEM_STATUS
  *          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SERVICE_OFF_GETZOOMSPEC             21007
  */
int Service_OffGetZoomSpec(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂、机组、测点和时间的一组轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          时间                 FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          垂直方向波形数据                 FIELD_SERVICE_AXESTRACK_WAVE_VER
*          垂直方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM
*          垂直方向特征值结构体             FIELD_SERVICE_CHARVALUE_S2
*          垂直方向测点报警值               FIELD_SYSTEM_ALARM_LOW2
*          垂直方向测点危险值               FIELD_SYSTEM_ALARM_HIGH2
*          垂直方向测点单位                 FIELD_SERVICE_CHANN2_UNIT
*          垂直方向测点别名                 FIELD_SYSTEM_CHANN2_ALIAS
*          水平方向波形数据                 FIELD_SERVICE_AXESTRACK_WAVE_HOR
*          水平方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM1
*          水平方向特征值结构体             FIELD_SERVICE_CHARVALUE_S
*          水平方向测点报警值               FIELD_SYSTEM_ALARM_LOW
*          水平方向测点危险值               FIELD_SYSTEM_ALARM_HIGH
*          水平方向测点单位                 FIELD_SERVICE_CHANN1_UNIT
*          水平方向测点别名                 FIELD_SYSTEM_CHANN_ALIAS
*          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
*          指定机组测点的转速               FIELD_SERVICE_REV
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
#define     SERVICE_SERVICE_OFF_GETAXESTRACK              21008
*/
int Service_OffGetAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);


/**得到指定公司、分厂、设备的任意多个振动测点的特征值信息、报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*			     分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一时间值    FIELD_SERVICE_OFF_TIME
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO   21009
*/
int Srv_OffGetSpecPlantChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!得到指定测点、时间的基本报警信息
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*			     分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点名称：      FIELD_SERVICE_OFF_CHANN1
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*返回值的字段码
*          特征值：      FIELD_SERVICE_CHARVALUE_S
*          报警门限Buff  FIELD_SERVICE_THRESHOLD_BUFF
*          门限的个数    FIELD_SERVICE_CHANN_NUM
*          报警类型      FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO   21010
*/
int Srv_OffGetSpecVibChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);


/*!得到指定起止时间的指定机组和指定振动测点类型的报警记录
*输入参数字段码
*          公司ID   FIELD_SYSTEM_COMPANY
*    			 分厂名   FIELD_SYSTEM_FACTORY
*          机组ID： FIELD_SERVICE_PLANTID
*          测点名： FIELD_SERVICE_CHANN1
*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          报警开始时间:   FIELD_SERVICE_OFF_TIMESTART
*          报警结束时间:   FIELD_SERVICE_OFF_TIMEEND
*          报警类型：      FIELD_SYSTEM_TYPE
*          事件ID          FIELD_SYSTEM_EVENT_ID
#define     SERVICE_OFF_GETVIBCHANNALARMLOG      21011
*/
int Srv_OffGetVibChannAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定时间的指定测点的报警记录的波形和各种特征值，对于多个测点就循环得到
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*          分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数：      FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          测点ID：        FIELD_SERVICE_OFF_CHANN1
*          报警类型：      FIELD_SYSTEM_TYPE
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          波形：          FIELD_SERVICE_OFF_WAVE
*          频谱：          FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：      FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：      FIELD_SERVICE_OFF_WAVE_NUM
*          频谱数据点数    FIELD_SERVICE_ZOOM_LINES
*          转速：          FIELD_SERVICE_OFF_REV
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD     21012
*/
int Service_OffGetSpecAlarmTimeRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);


/*!得到指定时间范围的指定机组的启停车记录
  *输入参数字段码
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          启停车类型：  FIELD_SYSTEM_TYPE
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD   21013
  */
int Service_OffGetSpecTimeRangeSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*！得到指定启停车事件ID指定机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
 *           对应时间的微秒         FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD   21014
  */
int Service_OffGetSpecTimeChannSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*!得到指定启停车事件ID、指定公司、分厂、机组、指定测点和指定转速的启停车的详细数据。
*输入参数字段码
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID：      FIELD_SERVICE_PLANTID
*          测点ID：      FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          转速:         FIELD_SERVICE_OFF_REV
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数      FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD    21015
  */
int Service_OffGetSpecChannRevSud(ICommInterFace * pFixComm, IDBInterFace *pDB);

/*!得到指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          机组ID：    FIELD_SERVICE_PLANTID
*          测点ID：    FIELD_SERVICE_OFF_CHANN1
*  启停车数据保存时间: FIELD_SERVICE_OFF_TIME
*对应时间的微秒        FIELD_SERVICE_MICROSECOND
*          分析线数    FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*          调用接口状态：FIELD_SYSTEM_STATUS
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          转速2：     FIELD_SERVICE_OFF_REV2
*          频谱点数：  FIELD_SERVICE_OFF_FREQWAVE_NUM
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD    21016
*/
int Service_offGetSpecChannSaveTimeSud(ICommInterFace * pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、时间段、任意多个振动测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          转速Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETVIBCHANNSTREND         21017
*/
int Srv_OffGetVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到当前时间向后推一个趋势类型的趋势数据。指定公司、分厂、机组、任意多个振动测点的所有特征值趋势数据，
*通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          趋势类型      FIELD_SERVICE_OFF_TRENDTYPE
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*          转速Buff             FIELD_SERVICE_REV_BUFF
#define     SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND      21018
*/
int Srv_OffGetCurrVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定两个振动测点的历史波形（循环两次得到两个测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一          FIELD_SERVICE_OFF_CHANN1
*          时间值          FIELD_SERVICE_OFF_TIME
*          公司ID2         FIELD_SYSTEM_COMPANY2
*			     分厂名2         FIELD_SYSTEM_FACTORY2
*          机组ID2         FIELD_SERVICE_PLANTID2
*          测点二          FIELD_SERVICE_OFF_CHANN2
*          时间值          FIELD_SERVICE_OFF_TIME2
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数        FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*           波形数据        FIELD_SERVICE_OFF_WAVE
*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速            FIELD_SERVICE_OFF_REV
*          报警值                           FIELD_SYSTEM_ALARM_LOW
*          危险值                           FIELD_SYSTEM_ALARM_HIGH
*          单位                             FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDATA           21019
*/
int Srv_OffGetTwoChannData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、设备的任意多个过程测点的值信息、报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点一时间值    FIELD_SERVICE_OFF_TIME
*          测点buff     FIELD_SERVICE_CHANN_BUFF
*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数     FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          特征值          FIELD_SERVICE_TREND_PROCESS
*          转速            FIELD_SERVICE_OFF_REV
*          报警限          FIELD_SYSTEM_ALARM_LOW
*          危险限          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO   21020
*/
int Srv_OffGetSpecPlantProcChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!得到指定公司、分厂、机组、任意多测点的指定启停车事件的趋势数据。
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          特征值类型      FIELD_SERVICE_TREND_CHARTYPE
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          启停车事件ID    FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
*         对应时间的微秒         FIELD_SERVICE_MICROSECOND
*          转速:                FIELD_SERVICE_OFF_REV
*          转速2：              FIELD_SERVICE_OFF_REV2
#define    SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND       21021
*/
int Srv_OffGetSpecEventChannsSudTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/*!得到指定公司、分厂、机组、事件、任意多测点、保存时间的详细数据
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*          分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          启停车事件ID    FIELD_SYSTEM_EVENT_ID
*    启停车数据保存时间    FIELD_SERVICE_OFF_TIME
*            对应时间的微秒         FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*          测点ID               FIELD_SERVICE_CHANN1
*          测点别名             FIELD_SYSTEM_CHANN_ALIAS
*          测点特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          转速:                FIELD_SERVICE_OFF_REV
*          额定转速：           FIELD_SYSTEM_PLANT_REV
*          第一临界转速         FIELD_SERVICE_REV
*          第二临界转速         FIELD_SERVICE_REV2
*          报警限               FIELD_SYSTEM_ALARM_LOW
*          危险限               FIELD_SYSTEM_ALARM_HIGH
*          单位                 FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO    21022
*/
int Srv_OffGetSpecEventTimeChannsSudDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*			     分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*返回值的字段码：
*          测点ID          FIELD_SERVICE_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO       21023
*/
int Srv_OffGetSpecPlantChannInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
*输入参数：
*          公司ID          FIELD_SYSTEM_COMPANY
*		   分厂名          FIELD_SYSTEM_FACTORY
*          机组ID          FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          各个测点特征值Buff  FIELD_SERVICE_CHARVALUE_SBUFF
*          对应报警类型Buff    FIELD_SERVICE_TYPE_BUFF
*          测点个数            FIELD_SERVICE_CHANN_NUM
*          时间值              FIELD_SERVICE_OFF_TIME
*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND        21024
*/
int Srv_OffGetVibChannAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、测点和报警事件ID的轴心位置趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:         FIELD_SERVICE_OFF_REV
*          水平方向值      FIELD_SERVICE_AXESLOCATION_HOR
*          水平方向直流量  FIELD_SERVICE_TREND_DC
*          水平方向间隙电压 FIELD_SERVICE_TREND_GAP
*          垂直方向值      FIELD_SERVICE_AXESLOCATION_VER
*          垂直方向直流量  FIELD_SERVICE_TREND_DC2
*          垂直方向间隙电压 FIELD_SERVICE_TREND_GAP2
*          报警类型      FIELD_SYSTEM_TYPE
*          时间         FIELD_SERVICE_OFF_TIME
*          报警标示     FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND       21025
*/
int Srv_OffGetAxesLocationAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定报警事件的报警数据保存的开始截止时间
*          公司ID   FIELD_SYSTEM_COMPANY
*			分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
#define SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE             21026
*/
int Srv_OffGetAlarmDataTimeRange(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定轴心位置报警事件、时间的报警数据
*          公司ID   FIELD_SYSTEM_COMPANY
*		    分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          时间                 FIELD_SERVICE_OFF_TIME
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:         FIELD_SERVICE_OFF_REV
*          水平方向值      FIELD_SERVICE_AXESLOCATION_HOR
*          水平方向直流量  FIELD_SERVICE_TREND_DC
*          水平方向间隙电压 FIELD_SERVICE_TREND_GAP
*          垂直方向值      FIELD_SERVICE_AXESLOCATION_VER
*          垂直方向直流量  FIELD_SERVICE_TREND_DC2
*          垂直方向间隙电压 FIELD_SERVICE_TREND_GAP2
*          报警点标示      FIELD_SYSTEM_TYPE
#define SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA            21027
*/
int Srv_OffGetAxesLocationAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、振动测点、报警事件ID的振动详细数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_CHARVALUE_S
*          测点门限Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          门限个数            FIELD_SERVICE_LEN
*          时间值              FIELD_SERVICE_OFF_TIME
*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
*          事件类型            FIELD_SYSTEM_EVENT_TYPE
*          报警类型            FIELD_SYSTEM_TYPE
*          报警点标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND          21028
*/
int Srv_OffGetVibAlarmInfoTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、轴位移测点、报警事件ID的数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_TREND_AXISDISPLACE
*          时间值              FIELD_SERVICE_OFF_TIME
*          测点门限            FIELD_SERVICE_THRESHOLD
*          报警点标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND      21029
*/
int Srv_OffGetAxisDisplaceAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、过程量测点、报警事件ID的数据趋势。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点值              FIELD_SERVICE_TREND_PROCESS
*          时间值              FIELD_SERVICE_OFF_TIME
*          测点门限            FIELD_SERVICE_THRESHOLD
*          报警点标示          FIELD_SYSTEM_STATUS
*          报警类型            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETPROCALARMTREND        21030
*/
int Srv_OffGetProcAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**得到指定公司、分厂、机组、过程量测点、报警事件ID、时间的数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*          时间值              FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点值              FIELD_SERVICE_TREND_PROCESS
#define     SERVICE_SERVICE_OFF_GETPROCALARMDATA        21031
*/
int Srv_OffGetProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、测点、报警事件ID的趋势报警数据。
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID               FIELD_SERVICE_PLANTID
  *          测点ID               FIELD_SERVICE_CHANN1
  *          事件ID:              FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
*          特征值Buff          FIELD_SERVICE_CHARVALUE_SBUFF
*          特征Buff长度        FIELD_SERVICE_TIMEWAVE_NUM
*          测点门限Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          门限个数           FIELD_SERVICE_LEN
*          报警类型            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETTRENDALARMDATA        21032
*/
int Srv_OffGetTrendAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、轴位移测点、报警事件ID、时间的详细数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          测点ID               FIELD_SERVICE_CHANN1
*          事件ID:              FIELD_SYSTEM_EVENT_ID
*          时间值              FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          转速:               FIELD_SERVICE_OFF_REV
*          测点特征值          FIELD_SERVICE_TREND_AXISDISPLACE
*          直流量              FIELD_SERVICE_TREND_DC
*          间隙电压            FIELD_SERVICE_TREND_GAP
*          报警类型            FIELD_SYSTEM_TYPE
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA    21033
*/
int Srv_OffGetAxisDisplaceAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**得到指定时间的指定测点的报警记录的各种特征值，对于多个测点就循环得到
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*			     分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点buff        FIELD_SERVICE_CHANN_BUFF
*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数        FIELD_SERVICE_CHANN_NUM
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*          测点ID：        FIELD_SERVICE_OFF_CHANN1
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          报警类型：      FIELD_SYSTEM_TYPE
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          转速：          FIELD_SERVICE_OFF_REV
*          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE    21034
*/
int Srv_OffGetSpecAlarmRecordCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定历史时刻、机组示功图测点的波形。
*输入参数：
*输入参数值：公司ID             FIELD_SYSTEM_COMPANY
*		     分厂名            FIELD_SYSTEM_FACTORY
*            机组ID             FIELD_SERVICE_PLANTID
*            测点buff           FIELD_SERVICE_CHANN_BUFF
*            测点buff长度       FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数           FIELD_SERVICE_CHANN_NUM
*            示功图类型         FIELD_SYSTEM_TYPE       P-V, P-a, P-t
*            时间               FIELD_SERVICE_OFF_TIME
*            波形数据个数      FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          测点的X轴数据    FIELD_SERVICE_ZOOMSPECTRUM
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          报警值           FIELD_SYSTEM_ALARM_LOW
*          危险值           FIELD_SYSTEM_ALARM_HIGH
*          单位             FIELD_SERVICE_CHANN1_UNIT
*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
*          转速             FIELD_SERVICE_REV
*          采样频率        FIELD_SERVICE_SAMPLE_FREQ
#define     SERVICE_SERVICE_OFF_GETPVDATA                   21035
*/
int Srv_OffGetPVData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**得到指定历史时刻、机组活塞杆沉降量测点的波形和频谱。
*输入参数：
*          公司ID           FIELD_SYSTEM_COMPANY
*			     分厂名           FIELD_SYSTEM_FACTORY
*          机组ID           FIELD_SERVICE_PLANTID
*          测点             FIELD_SERVICE_OFF_CHANN1
*          时间             FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          报警值           FIELD_SYSTEM_ALARM_LOW
*          危险值           FIELD_SYSTEM_ALARM_HIGH
*          单位             FIELD_SERVICE_CHANN1_UNIT
*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
*          转速           FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036
*/
int Srv_OffGetRodSinkData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其指定时间波形和特征参数
*如果测点为空就不会返回对应的数据
*输入参数值：
*            公司ID                     FIELD_SYSTEM_COMPANY
*			 分厂名                     FIELD_SYSTEM_FACTORY
*            机组ID                     FIELD_SERVICE_PLANTID
*            振动测点buff               FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数               FIELD_SERVICE_CHANN_NUM
*            动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数               FIELD_SERVICE_CHANN_NUM2
*            活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
*            时间                       FIELD_SERVICE_OFF_TIME
*            波形数据个数               FIELD_SERVICE_OFF_WAVE_NUM
*返回字段：
*            波形数据的点数               FIELD_SERVICE_TIMEWAVE_NUM
*            采样频率                     FIELD_SERVICE_SAMPLE_FREQ
*            测点名称                     FIELD_SERVICE_CHANN1
*            测点波形                     FIELD_SERVICE_TIMEWAVE
*            测点特征值                   FIELD_SERVICE_CHARVALUE_S
*            横坐标                       FIELD_SERVICE_FREQWAVE2
*            转速                         FIELD_SERVICE_OFF_REV
*            吸气阀开启相位角             FIELD_SERVICE_ANGLEPARAM1
*            吸气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM2
*            排气阀开启相位角             FIELD_SERVICE_ANGLEPARAM3
*            排气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM4
#define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA           21037
*/
int Srv_OffGetRcMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB );

/**历史概貌图，得到指定时间、指定机组所有测点数据
*输入参数值：
*            公司ID         FIELD_SYSTEM_COMPANY
*	         分厂名         FIELD_SYSTEM_FACTORY
*            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
*            设备号buff     FIELD_SERVICE_CHANN_BUFF
*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            设备号个数     FIELD_SERVICE_CHANN_NUM
*            时间值         FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            设备ID         FIELD_SERVICE_PLANTID
*            测点ID         FIELD_SERVICE_CHANN1
*            测点值         FIELD_SERVICE_CHANN1VALUE
*            测点单位       FIELD_SERVICE_CHANN1_UNIT
*            测点所属列表   FIELD_SERVICE_CHANN1_LIST
*            测点的状态     FIELD_SERVICE_CHANN1_STATUS
*            测点特征值     FIELD_SERVICE_CHARVALUE_S
*            测点类型       FIELD_SERVICE_CHANNTYPE
*            转速1          FIELD_SERVICE_OFF_REV
#define     SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE    21038
*/
int Srv_OffGetSpecTimeAllChannValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、测点,指定启停车事件和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*          分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          时间                 FIELD_SERVICE_OFF_TIME
*          启停车事件ID         FIELD_SYSTEM_EVENT_ID
*          转速                 FIELD_SERVICE_OFF_REV
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                       FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*          转速                           FIELD_SERVICE_OFF_REV
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
#define     SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK           21039
*/
int Srv_OffGetSudPurifyAxesTrack( ICommInterFace *pFixComm, IDBInterFace *pDB );


/**往复式系统报警多参数分析功能，读取其指定报警时间的波形和特征参数，读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。如果测点为空就不会返回对应的数据。
*输入参数值：
*            公司ID                     FIELD_SYSTEM_COMPANY
*			 分厂名                     FIELD_SYSTEM_FACTORY
*            机组ID                     FIELD_SERVICE_PLANTID
*            振动测点buff               FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数               FIELD_SERVICE_CHANN_NUM
*            动态测点buff               FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数               FIELD_SERVICE_CHANN_NUM2
*            报警时间值                 FIELD_SERVICE_OFF_TIME
*返回字段：
*            波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
*            采样频率                   FIELD_SERVICE_SAMPLE_FREQ
*            测点名称                   FIELD_SERVICE_CHANN1
*            测点波形                   FIELD_SERVICE_TIMEWAVE1
*            测点特征值                 FIELD_SERVICE_CHARVALUE_S
*            横坐标                     FIELD_SERVICE_FREQWAVE2
#define   SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA       21041
*/
int Srv_OffGetSpecTimeAlarmMultiParamData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**往复式系统报警应力监测分析功能，得到指定机组指定报警时间、指定缸号的应力波形和振动测点波形
*输入参数：
*            公司ID        　　　    FIELD_SYSTEM_COMPANY
*		     分厂名          　　　  FIELD_SYSTEM_FACTORY
*            机组ID          　　　  FIELD_SERVICE_PLANTID
*            汽缸号                  FIELD_SERVICE_CYLINDERNO
*            报警时间                FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*            惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*            活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*            波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE       21040
*/
int Srv_OffGetSpecTimeAlarmDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);


/*!得到临时在线指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          机组ID：    FIELD_SERVICE_PLANTID
*          测点ID：    FIELD_SERVICE_OFF_CHANN1
*  启停车数据保存时间: FIELD_SERVICE_OFF_TIME
*对应时间的微秒        FIELD_SERVICE_MICROSECOND
*          分析线数    FIELD_SERVICE_OFF_FREQANALYSIS_LINES
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          转速2：     FIELD_SERVICE_OFF_REV2
*          频谱点数：  FIELD_SERVICE_OFF_FREQWAVE_NUM
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX    21042
*/
int Service_offGetSpecChannSaveTimeSudEx(ICommInterFace * pFixComm,IDBInterFace *pDB);


/*！得到临时在线机组指定启停车事件ID指定机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
 *           对应时间的微秒         FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX   21043
  */
int Service_OffGetSpecTimeChannSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB);


/*!得到指定启停车事件ID和指定公司、分厂机组、指定测点指定时间段的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分   *别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *			 分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          开始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          结束时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
  *对应时间的微秒        FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  
  #define    SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME          21044
*/
int Srv_OffGetSpecChannSudByTime (ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定时间的月、公司、分厂、机组各个测点的各种特征值最大值最小值平均值，测点特征值buff中依次是最大值、最小值和平均值，特征值个数应该是3
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          时间值               FIELD_SERVICE_OFF_TIME
*返回值的字段：
*          测点特征值Buff      FIELD_SERVICE_CHARVALUE_SBUFF
*          特征值个数          FIELD_SERVICE_CHANN_NUM
*          测点ID               FIELD_SERVICE_CHANN1
#define SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT        21051
*/
int Srv_OffGetSpecPlantMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、指定时间范围的报警信息、启停车信息。
*振动通道报警次数、过程量通道报警、累计停车次数、累计停机时间、累计运行时间，时间单位为小时
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
*返回的字段：
*          机组ID               FIELD_SERVICE_PLANTID
*          所有测点报警次数     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*          累计停车次数         FIELD_SERVICE_LEN
*          累计停机时间(小时)   FIELD_SERVICE_SAMPLE_FREQ
*          累计运行时间(小时)   FIELD_SERVICE_SAMPLE_NUM
#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052
*/
int Srv_OffGetSpecTimeRangeFactoryRunInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**关联诊断。这个报警ID的开始截止时间范围内，返回的记录只有一条，0表示没有，1表示有。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
*          机组ID   FIELD_SERVICE_PLANTID
*          测点ID   FIELD_SERVICE_CHANN1
*          事件ID   FIELD_SYSTEM_EVENT_ID
*返回值的字段码：
*          同转速测点是否报警               FIELD_SERVICE_OFF_REV
*          同转速测点是否存在               FIELD_SERVICE_OFF_REV2
*          同转速测点是否存在残振或通频报警 FIELD_SERVICE_TREND_RESCHAR
*          对应测点是否报警          FIELD_SERVICE_REV
*          对应测点是否存在          FIELD_SERVICE_REV2
*          对应测点是否存在残振或通频报警   FIELD_SERVICE_TREND_DC
#define SERVICE_SERVICE_OFF_CORRDIGN                21053
*/
int Srv_OffCorrDign(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**集团级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     设备数           FIELD_SERVICE_PLANT_NUM
*		     报警设备数       FIELD_SERVICE_ALARM_PLANTNUM
#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT             21054
*/
int Srv_OffGetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**公司级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            分厂名           FIELD_SYSTEM_FACTORY
*		     设备名           FIELD_SERVICE_PLANTID
*		     报警次数         FIELD_SERVICE_ALARM_ PLANTNUM
*		     停车次数         FIELD_SERVICE_STOP_ PLANTNUM
#define    SERVICE_SERVICE_OFF_COMPANYALARMSTAT         21055
*/
int Srv_OffGetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		        分厂名           FIELD_SYSTEM_FACTORY
*	            设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		        报警ID          FIELD_SERVICE_ ALARM ID
*		        测点编号         FIELD_SERVICE_CHANN1
*		        测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		        特征值           FIELD_SERVICE_TREND_VALUE
*		        转速1            FIELD_SERVICE_REV
*		      报警开始时间       FIELD_SERVICE_OFF_TIMESTART// 报警FIELD_SERVICE_ALARM_TIMESTART
*		      报警结束时间       FIELD_SERVICE_OFF_TIMEEND
*		        报警类型         FIELD_SERVICE_ALARM_TYPE
*                成功标示         FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21056
*/
int Srv_OffGetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		        分厂名           FIELD_SYSTEM_FACTORY
*	            设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		        报警ID          FIELD_SERVICE_ ALARM ID
*		        测点编号         FIELD_SERVICE_CHANN1
*		        测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		      报警开始时间       FIELD_SERVICE_OFF_TIMESTART// 报警FIELD_SERVICE_ALARM_TIMESTART
*		      报警结束时间       FIELD_SERVICE_OFF_TIMEEND
*		        报警类型         FIELD_SERVICE_ALARM_TYPE
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT    21088
*/
int Srv_OffGetMonitorAlarmStatEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**单通道多特征值报警查询，得到指定报警测点的各种类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
*            时间             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES   21057
*/
int Srv_OffGetSpecChannAlarmMultCharValues(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警测点buff     FIELD_SERVICE_CHANN_BUFF
*            报警测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            报警测点个数     FIELD_SERVICE_CHANN_NUM
*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            测点特征值       FIELD_SERVICE_TREND_VALUE
*            测点ID           FIELD_SERVICE_CHANN1
*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
*            时间             FIELD_SERVICE_TIME
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE   21058
*/
int Srv_OffGetSpecChannsAlarmSingleCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹
*输入参数：
*          公司ID            FIELD_SYSTEM_COMPANY
*		   分厂名            FIELD_SYSTEM_FACTORY
*          机组ID            FIELD_SERVICE_PLANTID
*          轴承位置描述      FIELD_SYSTEM_AXISLOCATION
*          水平方向测点ID    FIELD_SERVICE_CHANNID_HOR
*          垂直方向测点ID    FIELD_SERVICE_CHANNID_VER
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          时间值          　FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*         X方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_HOR
*         Y方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_VER
*         实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
*         X方向测点特征值  FIELD_SERVICE_CHARVALUE_S
*         Y方向测点特征值  FIELD_SERVICE_CHARVALUE_S2
*         转速            FIELD_SERVICE_OFF_REV
#define   SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION            21059
*/
int Srv_OffGetSpecChannRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到往复式机械设备运行状态历史趋势
*读取所有各种测点的特征值，通过循环依次取得各个测点的第一个特征值。
*输入参数值：公司ID               FIELD_SYSTEM_COMPANY
*		     分厂名               FIELD_SYSTEM_FACTORY
*            机组ID               FIELD_SERVICE_PLANTID
*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数         FIELD_SERVICE_CHANN_NUM
*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
*            过程量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            过程量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            过程量测点个数       FIELD_SERVICE_CHANN_NUM3
*            起始时间             FIELD_SERVICE_OFF_TIMESTART
*            终止时间             FIELD_SERVICE_OFF_TIMEEND
*            曲轴箱振动特征值类型 FIELD_SERVICE_TREND_CHARTYPE
*            缸体振动特征值类型   FIELD_SERVICE_CHANNTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060
*/
int Srv_OffGetSpecChannsRunStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到往复式机械设备运行状态历史趋势
*读取所有各种测点的特征值，通过循环依次取得各个测点的第一个特征值。
*输入参数值：公司ID               FIELD_SYSTEM_COMPANY
*		     分厂名               FIELD_SYSTEM_FACTORY
*            机组ID               FIELD_SERVICE_PLANTID
*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数         FIELD_SERVICE_CHANN_NUM
*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
*            过程量测点buff       FIELD_SERVICE_CHANN_BUFF3
*            过程量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
*            过程量测点个数       FIELD_SERVICE_CHANN_NUM3
*            起始时间             FIELD_SERVICE_OFF_TIMESTART
*            终止时间             FIELD_SERVICE_OFF_TIMEEND
*            曲轴箱振动特征值类型 FIELD_SERVICE_TREND_CHARTYPE
*            缸体振动特征值类型   FIELD_SERVICE_CHANNTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX                   21082
*/
int Srv_OffGetSpecChannsRunStatusEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到往复式机械设备历史特征值和波形数据趋势横、纵向比较；
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID          FIELD_SERVICE_PLANTID
*            测点buff         FIELD_SERVICE_CHANN_BUFF
*            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数         FIELD_SERVICE_CHANN_NUM
*            测点类型         FIELD_SERVICE_OFF_CHANNTYPE
*            起始时间         FIELD_SERVICE_OFF_TIMESTART
*            终止时间         FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*            转速             FIELD_SERVICE_REV
*            时间值           FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETHISCOMPARE         21061
*/
int Srv_OffGetSpecChannsHisCompare(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到往复式机械设备所有通道指定时间的特征值和波形数据比较
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID          FIELD_SERVICE_PLANTID
*            测点buff         FIELD_SERVICE_CHANN_BUFF
*            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            测点类型：       FIELD_SERVICE_CHANNTYPE
*            测点个数         FIELD_SERVICE_CHANN_NUM
*            时间             FIELD_SERVICE_OFF_TIME
*            波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*            测点的波形数据   FIELD_SERVICE_OFF_WAVE
*            波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            转速             FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE         21062
*/
int Srv_OffGetSpecChannsCharValueWave(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**得到往复式机械设备振动通道和动态通道指定时间的特征值
*输入参数：
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            振动测点个数         FIELD_SERVICE_CHANN_NUM
*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
*            振动特征值时间     FIELD_SERVICE_TIME
*            动态特征值时间     FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
*            转速             FIELD_SERVICE_OFF_REV
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063
*/
int Srv_OffGetSpecChannsCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到往复式机械设备机组月报表；
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            时间值           FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            测点名           FIELD_SERVICE_CHANN1
*            特征值最大值     FIELD_SERVICE_CHANN1VALUE
*            特征值最小值     FIELD_SERVICE_CHANN2VALUE
*            特征值平均值     FIELD_SERVICE_TREND_VALUE
#define     SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT       21064
*/
int Srv_OffGetSpecRecipMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**监测站级历史报警统计，优化读取数据，读取20条记录以后的特征值和转速
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065
*/
int Srv_OffGetMonitorAlarmStatChar(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定公司、分厂、机组、测点和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*		   分厂名   FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          时间                 FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK           21066
*/
int Srv_OffGetPurifyAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂机组和测点指定时刻的倒谱数据
*输入参数：
*          公司ID       FIELD_SYSTEM_COMPANY
*	      分厂名       FIELD_SYSTEM_FACTORY
*          机组ID       FIELD_SERVICE_PLANTID
*          测点ID       FIELD_SERVICE_OFF_CHANN1
*          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
*          时间         FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          波形数据        FIELD_SERVICE_OFF_WAVE
*          倒谱数据        FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*          倒谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
*          倒谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
*          报警值          FIELD_SYSTEM_ALARM_LOW
*          危险值          FIELD_SYSTEM_ALARM_HIGH
*          单位            FIELD_SERVICE_CHANN1_UNIT
*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
*          转速            FIELD_SERVICE_OFF_REV
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM       21067
*/
int Service_OffGetSpecChannCepstrum(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**往复机械报警查询，得到指定报警ID最先报警的测点名、特征值趋势
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*            分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            报警测点名              FIELD_SERVICE_CHANN1
*            报警测点的特征值结构体  FIELD_SERVICE_CHARVALUE_S
*            时间                    FIELD_SERVICE_TIME
*            对应时间的微秒          FIELD_SERVICE_MICROSECOND
#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068
*/
int Srv_OffGetSpecAlarmIDCharValues(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**往复机械报警查询,得到指定时间的指定活塞杆沉降或动态压力测点的报警记录
*输入参数字段码
*          公司ID          FIELD_SYSTEM_COMPANY
*          分厂名          FIELD_SYSTEM_FACTORY
*          机组ID：        FIELD_SERVICE_PLANTID
*          测点            FIELD_SERVICE_OFF_CHANN1
*          事件ID:         FIELD_SYSTEM_EVENT_ID
*          报警时间：      FIELD_SERVICE_OFF_TIME
*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
*返回值的字段码：
*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
*          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
*          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
*          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
*          测点的X轴数据    FIELD_SERVICE_ZOOMSPECTRUM
*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
*          转速             FIELD_SERVICE_REV
*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD     21069
*/
int Srv_OffGetSpecAlarmTimeDynRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂机组和一对振动测点指定时刻的全谱数据
*输入参数：
*          公司ID               			  FIELD_SYSTEM_COMPANY
*		   分厂名               			  FIELD_SYSTEM_FACTORY
*          机组ID               			  FIELD_SERVICE_PLANTID
*          水平方向振动测点ID  			  FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   		       FIELD_SERVICE_CHANNID_VER
*          轴承位置描述        			  FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         			  FIELD_SERVICE_OFF_WAVE_NUM
*          时间                             FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
#define     SERVICE_SERVICE_OFF_GETFULLSPECSTRUM          21070
*/
int Srv_OffGetSpecTimeFullSpectrum(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**报警查询功能，查询报警ID报警类型
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID            FIELD_SERVICE_ALARMID
*		     报警开始时间      FIELD_SERVICE_OFF_TIMESTART
*		     报警类型          FIELD_SERVICE_ALARM_TYPE
*            成功标示          FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_GETALARMLOGINFO         21071
*/
int Srv_OffGetAlarmLogInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**往复式系统应力监测，得到指定机组指定时间、指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		  分厂名          　　　  FIELD_SYSTEM_FACTORY
*          机组ID          　　　  FIELD_SERVICE_PLANTID
*          汽缸号                  FIELD_SERVICE_CYLINDERNO
*          缸体振动测点            FIELD_SERVICE_OFF_CHANN1
*          时间                    FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*          振动波形数据   　　　 　FIELD_SERVICE_FREQWAVE2
*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE              21072
*/
int Srv_OffGetSpecTimeDynStressWave(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂、机组、时间段、任意多个测点的自定义参数，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*输入参数字段码：
*          公司ID        FIELD_SYSTEM_COMPANY
*		   分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
*          测点个数             FIELD_SERVICE_CHANN_NUM
*          时间值               FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND         21073
*/
int Srv_OffGetSpecChannsCustomTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

/** 获取离线巡检数据：
#define     SERVICE_SERVICE_OFF_GET_OFFLINE_DATA         21074
*/
int Srv_OffGetOfflineData(ICommInterFace *pFixComm, IDBInterFace *pDB);

/**监测站级历史报警统计
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*			 报警ID列表	      FIELD_SERVICE_ALARMIDS
*			 报警ID个数	      FIELD_SERVICE_ALARMID_NUM
*			 报警ID缓冲区大小 FIELD_SERVICE_ALARMID_BUFFSIZE
*返回值的字段码：
*		     报警ID           FIELD_SERVICE_ ALARM ID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
*		     报警类型         FIELD_SERVICE_ALARM_TYPE
*            成功标示         FIELD_SYSTEM_STATUS
			 接口数据上限	  FIELD_SERVICE_DATALIMIT
#define     SERVICE_SERVICE_OFF_GETALARMDATA    21075
*/
int Srv_OffGetAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);



/**监测站级历史报警ID列表
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
			 

*返回值的字段码：
			 报警ID		      FIELD_SERVICE_ALARMID
			 报警ID个数       FIELD_SERVICE_ALARMID_NUM

#define     SERVICE_SERVICE_OFF_GETALARMIDS    21076
*/
int Srv_OffGetAlarmIDS(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**监测站级历史报警ID列表
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND


*返回值的字段码：
报警ID		              FIELD_SERVICE_ALARMID
报警ID对应的测点名        FIELD_SERVICE_CHANN1
报警ID对应的时间          FIELD_SERVICE_OFF_TIMESTART
报警类型                  FIELD_SERVICE_ALARM_TYPE

*/
int Srv_OffGetAlarmIDSEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**申请指定公司、分厂、设备的启停车事件ID
*输入参数字段码：
*             公司ID         FIELD_SYSTEM_COMPANY
*			  分厂名         FIELD_SYSTEM_FACTORY
*             机组ID：       FIELD_SERVICE_PLANTID
*             开始时间       FIELD_SERVICE_OFF_TIMESTART
*返回值的字段码：
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
#define     SERVICE_SERVICE_OFF_REQSUDEVENTID        24008
*/
int Srv_OffReqSudEventID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**通知指定启停车事件的启停车过程结束。如果成功返回1，失败返回-1
*输入参数字段码：
*             公司ID        FIELD_SYSTEM_COMPANY
*			  分厂名        FIELD_SYSTEM_FACTORY
*             机组ID：      FIELD_SERVICE_PLANTID
*             启停车事件ID:   FIELD_SYSTEM_EVENT_ID
*             启停车类型     FIELD_SERVICE_SUD_TYPE
*             结束时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_NOTICESUDSTOP        24009
*/
int Srv_OffNoticeSudStop(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**启停车结束后，发送指定机组的振动启停车数据到中间件
*输入参数字段码：
*            公司ID                 FIELD_SYSTEM_COMPANY
*			       分厂名                 FIELD_SYSTEM_FACTORY
*            机组ID                 FIELD_SERVICE_PLANTID
*            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
*            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*            采样频率               FIELD_SERVICE_SAMPLE_FREQ
*            采样点数               FIELD_SERVICE_SAMPLE_NUM
*            机组的转速             FIELD_SERVICE_REV
*            机组的转速2            FIELD_SERVICE_REV2
*            指定机组的转速3        FIELD_SERVICE_REV3
*            指定机组的转速4        FIELD_SERVICE_REV4
*            指定机组的转速5        FIELD_SERVICE_REV5
*            启停车事件ID:          FIELD_SYSTEM_EVENT_ID
*            对应的时间             FIELD_SERVICE_TIME
*            对应时间的微秒         FIELD_SERVICE_MICROSECOND
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECSUDDATA        24010
*/
int Srv_OffRecSudData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**@brief 发送阈值的信息、学习参数、学习数据，6种特征值一起发。返回-1表示失败，返回大于0表示成功
*输入参数字段码
*           公司ID          FIELD_SYSTEM_COMPANY
*			      分厂名          FIELD_SYSTEM_FACTORY
*           机组ID：        FIELD_SERVICE_PLANTID
*           通道类型        FIELD_SERVICE_CHANNTYPE
*           通道索引        FIELD_SERVICE_ON_SEND_STARTID
*           阈值ID          FIELD_SERVICE_THRESHOLD_ID
*           特征值个数      FIELD_SERVICE_CHANN_NUM
*           学习数据的BUFF  FIELD_SERVICE_ON_SEND_VIBWAVE    //数据依次为不同特征值数组链接起来.数组长度=FIELD_SERVICE_LEN*FIELD_SERVICE_CHANN_NUM
*           学习数据的长度  FIELD_SERVICE_LEN
*           阈值的BUFF      FIELD_SERVICE_THRESHOLD_BUFF     //结构体长度为FIELD_SERVICE_CHANN_NUM
*           学习参数的BUFF  FIELD_SERVICE_LEARNPARAM_BUFF    //结构体长度为FIELD_SERVICE_CHANN_NUM
*           报警类型        FIELD_SERVICE_ALARM_TYPE
*           学习数据的开始时间  FIELD_SERVICE_OFF_TIMESTART
*           学习数据的结束时间  FIELD_SERVICE_OFF_TIMEEND
*           触发门限学习的类型  FIELD_SYSTEM_TYPE
*返回值的字段码
*           成功标识            FIELD_SYSTEM_STATUS
#define    SERVICE_SERVICE_OFF_SAVETHINFO         24014
*/
int Srv_OffSaveThInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**通知指定快变报警事件的报警过程结束。如果成功返回1，失败返回-1
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			        分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             通道类型        FIELD_SERVICE_CHANNTYPE
*             通道索引        FIELD_SERVICE_ON_SEND_STARTID
*             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
*             新阈值ID        FIELD_SERVICE_THRESHOLD_ID
*             结束时间        FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP        24015
*/
int Srv_OffNoticeFastAlarmStop(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**发送快变报警数据到中间件保存
*输入参数字段码：
*             公司ID           FIELD_SYSTEM_COMPANY
*			        分厂名           FIELD_SYSTEM_FACTORY
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
*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
*             采样点数         FIELD_SERVICE_SAMPLE_NUM
  *            对应的时间        FIELD_SERVICE_TIME
  *            对应时间的微秒    FIELD_SERVICE_MICROSECOND
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_RECFASTALARMDATA        24016
  */
int Srv_OffRecFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**保存报警后新学习的阈值ID，如果数组长度小于1表示失败
  *输入参数字段码
  *           公司ID          FIELD_SYSTEM_COMPANY
  *			      分厂名          FIELD_SYSTEM_FACTORY
  *           机组ID：        FIELD_SERVICE_PLANTID
  *           通道个数        FIELD_SERVICE_CHANN_NUM
  *           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
  *返回值的字段码：
  *             成功标识      FIELD_SYSTEM_STATUS
  #define     SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID   24017
  */
int Srv_OffSaveAlarmedNewThID(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**添加一条完整的报警日志.失败返回值小于0，成功为0
*输入参数字段码
*           公司ID          FIELD_SYSTEM_COMPANY
*			分厂名          FIELD_SYSTEM_FACTORY
*           机组ID：        FIELD_SERVICE_PLANTID
*           通道ID          FIELD_SERVICE_CHANN1
*           日志ID          FIELD_SYSTEM_ID
*           快变报警事件ID  FIELD_SYSTEM_EVENT_ID
*           通道类型        FIELD_SERVICE_CHANNTYPE
*           报警类型        FIELD_SERVICE_ALARM_TYPE
*           开始时间        FIELD_SERVICE_OFF_TIMESTART
*           结束时间        FIELD_SERVICE_OFF_TIMEEND
*           阈值ID          FIELD_SERVICE_THRESHOLD_ID
*           新阈值ID        FIELD_SERVICE_ON_SEND_STARTID
*返回值的字段码：
*           成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_ADDALARMLOG                24018
*/
int Srv_OffAddAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**新增报警日志，报警开始添加。如果成功返回1，失败返回-1
*输入参数字段码：
*             公司ID          FIELD_SYSTEM_COMPANY
*			        分厂名          FIELD_SYSTEM_FACTORY
*             机组ID：        FIELD_SERVICE_PLANTID
*             通道类型        FIELD_SERVICE_CHANNTYPE
*             通道索引        FIELD_SERVICE_ON_SEND_STARTID
*             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
*             起始时间        FIELD_SERVICE_OFF_TIMESTART
*           报警类型        FIELD_SERVICE_ALARM_TYPE
*             阈值ID          FIELD_SERVICE_THRESHOLD_ID
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG        24019
*/
int Srv_OffAddFastAlarmStartLog(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**添加一条完整的启停车日志。如果成功返回启停车事件ID，失败返回-1
  *输入参数字段码：
  *             公司ID        FIELD_SYSTEM_COMPANY
  *			    分厂名        FIELD_SYSTEM_FACTORY
  *             机组ID：      FIELD_SERVICE_PLANTID
  *             启停车类型    FIELD_SERVICE_SUD_TYPE
  *             开始时间      FIELD_SERVICE_OFF_TIMESTART
  *             结束时间      FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *             启停车事件ID: FIELD_SYSTEM_EVENT_ID
  #define     SERVICE_SERVICE_OFF_ADDWHOLESUDLOG             24053
  */
int Srv_OffAddWholeSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**发送动态报警数据到中间件保存
*输入参数字段码：
*             公司ID           FIELD_SYSTEM_COMPANY
*			  分厂名           FIELD_SYSTEM_FACTORY
*             机组ID：         FIELD_SERVICE_PLANTID
*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
*             报警类型数组     FIELD_SERVICE_ALARM_TYPE
*             动态报警事件ID   FIELD_SYSTEM_EVENT_ID
*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
*             总的波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
*             对应的时间        FIELD_SERVICE_TIME
*             对应时间的微秒    FIELD_SERVICE_MICROSECOND
*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
*返回值的字段码：
*             成功标识      FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECDYNALARMDATA         24055
*/
int Srv_OffReceiveDynAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

/** 发送过程量报警数据到中间件模块
*输入参数字段码：
*                公司ID                 FIELD_SYSTEM_COMPANY
*                分厂名                 FIELD_SYSTEM_FACTORY
*                机组ID                 FIELD_SERVICE_PLANTID
*                报警类型               FIELD_SERVICE_ALARM_TYPE
*                报警事件ID             FIELD_SYSTEM_EVENT_ID
*                对应的时间             FIELD_SERVICE_TIME
*                过程量测点号           FIELD_SYSTEM_CHANN
*                过程量测点类型         FIELD_SYSTEM_CHANN_TYPE
*                过程量特征值           FIELD_SERVICE_CHANN1VALUE
*                转速                   FIELD_SERVICE_REV
*
*返回值的字段码：
*                成功标识               FIELD_SYSTEM_STATUS
#define     SERVICE_SERVICE_OFF_RECPROCALARMDATA           24056
*/
int Srv_OffReceiveProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);



/**请求离线巡检数据同步，返回离线测点索引和个数，及同步时间
*输入参数字段码：
*                公司              FIELD_SYSTEM_COMPANY
*			     分厂名            FIELD_SYSTEM_FACTORY
*                设备/机组         FIELD_SERVICE_PLANTID
*返回值的字段码：
*                上次同步时间      FIELD_SERVICE_TIME
*                离线测点起始索引  FIELD_SERVICE_ON_SEND_STARTID
*                离线测点个数      FIELD_SERVICE_CHANN_NUM
*                同步响应          FIELD_SYSTEM_STATUS
（FIELD_SYSTEM_SUCCESS 可以同步/ FIELD_SYSTEM_FAILURE 异常，无法同步）
*
#define     SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC
*/
int Srv_OffRequestOfflineSync(ICommInterFace *pFixComm, IDBInterFace *pDB);

/**接收离线巡检数据
*输入参数字段码：
*                公司名        FIELD_SYSTEM_COMPANY
*                分厂名        FIELD_SYSTEM_FACTORY
*                设备号        FIELD_SERVICE_PLANTID  (如1T1, 1T2)
*                采集时间      FIELD_SERVICE_TIME
*                测点名        FIELD_SYSTEM_CHANN
*                特征值        FIELD_SERVICE_OFF_DATA( float)
*返回值的字段码：
*                同步状态      FIELD_SYSTEM_STATUS
                                （FIELD_SYSTEM_SUCCESS/ FIELD_SYSTEM_FAILURE）
*
#define     SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA            24061
*/
int Srv_OffSyncOfflineData(ICommInterFace *pFixComm, IDBInterFace *pDB);


/**接收动平衡类型
*输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
*                采集时间       FIELD_SERVICE_TIME
*                测点名         FIELD_SYSTEM_CHANN
				 测点名2        FIELD_SYSTEM_CHANN2
*                BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
				 BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
				 BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
				 BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*返回值的字段码：
*                同步状态      FIELD_SYSTEM_STATUS                                
*
#define     SERVICE_SERVICE_OFF_SET_BALANCE_DATA            24062
*/
int Srv_SetBalanceData(ICommInterFace *pFixComm, IDBInterFace *pDB);



/**返回动平衡类型
*  输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*   返回值的字段码：
*               采集时间       FIELD_SERVICE_TIME
*               测点名         FIELD_SYSTEM_CHANN
*				测点名2        FIELD_SYSTEM_CHANN2
*               BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
*				BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
*				BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
*				BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节                     

*#define     SERVICE_SERVICE_OFF_GET_BALANCE_DATA 24063*/


int Srv_GetBalanceData(ICommInterFace *pFixComm, IDBInterFace *pDB);


	/**临时在线需求，读取或者设置多个正在采集的设备,e
	读取逻辑：如果没有输入（公司、分厂、设备名中任一个长度为零，或者在设备池中没有对应的设备），则返回所有设备的采集状态，如果是多个，客户端应以while(fixcom.more())的方式读取
	设置逻辑：如果有输入（公司、分厂、设备名中任一个长度均不为零，且在设备池中有对应的设备），则返回所有设备的采集状态，以及上次设置的时间，客户端应以while(fixcom.more())的方式读取
	中间件重启后以上次设置的为准，首次启动，均不采集
	*输入参数字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME
					设置状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
	*                
	*返回值的字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME 
					返回状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
									

	#define     SERVICE_SERVICE_TEMP_SAMPLE_PLANTS 24070 
	*/
int Service_temp_sample_plants(ICommInterFace * pFixComm,IDBInterFace *pDB);

	/*!
	*输入参数字段码
	*          公司ID      FIELD_SYSTEM_COMPANY
	*	     分厂名      FIELD_SYSTEM_FACTORY
	*          机组ID：    FIELD_SERVICE_PLANTID
	*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
	*          启停车类型：  FIELD_SYSTEM_TYPE
	*          试车事件名    FIELD_SYSTEM_EVENT_NAME
	*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
	*         #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX    21077 
	*/
int Service_GetSpecTimeRangeSudAndName(ICommInterFace * pFixComm,IDBInterFace *pDB);


/*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          查询试车记录字符串  FIELD_SYSTEM_QUERY_EVENTNAME
  *返回值的字段码：
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          启停车类型：  FIELD_SYSTEM_TYPE
  *          试车事件名    FIELD_SYSTEM_EVENT_NAME
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME    21078
  */
 
	int GetSpecTimeRangeSudSearchByName( ICommInterFace * pFixComm, 
														IDBInterFace *pDB );



/*!
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *	     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID：        FIELD_SERVICE_PLANTID
  *          启停车事件ID    FIELD_SYSTEM_EVENT_ID
  *          新启停车事件名  FIELD_SYSTEM_EVENT_NAME
  *返回值的字段码：
  *          是否修改成功：  FIELD_SYSTEM_STATUS
  *  #define    SERVICE_SERVICE_OFF_RENAMEEVENT 21079
  */
	int UpdateRenameEvent( ICommInterFace * pFixComm, 
										  IDBInterFace *pDB );

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
  *#define    SERVICE_SERVICE_OFF_SAVESPECTIMEDATA    21083
  */
	int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);

 /*!
 *输入参数字段码
 *          公司ID          FIELD_SYSTEM_COMPANY
 *	         分厂名        FIELD_SYSTEM_FACTORY
 *          机组ID：        FIELD_SERVICE_PLANTID
 *          标注时刻：      FIELD_SERVICE_OFF_TIME
 *          测点：          FIELD_SERVICE_CHANN1
 *          事件开始时间：  FIELD_SERVICE_OFF_TIMESTART
 *          事件结束时间：  FIELD_SERVICE_OFF_TIMEEND
 *          标注描述：      FIELD_SERVICE_REMARK    
 *返回值的字段码：
 *		   是否保存成功：  FIELD_SYSTEM_STATUS
 *#define    SERVICE_SERVICE_OFF_SAVESPECMARK    21084
 */
	int SaveSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);


	/*!
	*输入参数字段码
	*          公司ID      FIELD_SYSTEM_COMPANY
	*	       分厂名    FIELD_SYSTEM_FACTORY
	*          机组ID：    FIELD_SERVICE_PLANTID
	*          开始时间：  FIELD_SERVICE_OFF_TIMESTART
	*          结束时间：  FIELD_SERVICE_OFF_TIMEEND
	*          标注描述：  FIELD_SERVICE_REMARK    
	*返回值的字段码：
	*          测点：      FIELD_SERVICE_CHANN1
	*		   标注描述：  FIELD_SERVICE_REMARK 
	*          标注时间：  FIELD_SERVICE_OFF_TIME
	*#define    SERVICE_SERVICE_OFF_GETSPECMARK    21085
	*/
	int GetSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到指定公司、分厂、机组、时间段、任意多个振动测点测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *输入参数字段码：
  *          公司ID        FIELD_SYSTEM_COMPANY
  *			 分厂名        FIELD_SYSTEM_FACTORY
  *          机组ID        FIELD_SERVICE_PLANTID
  *          测点类型：    FIELD_SERVICE_CHANNTYPE
  *          测点buff      FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数      FIELD_SERVICE_CHANN_NUM
  *          起始时间      FIELD_SERVICE_OFF_TIMESTART
  *          终止时间      FIELD_SERVICE_OFF_TIMEEND
  *          查询的趋势类型 FIELD_SYSTEM_TYPE（0:只有常规数据，1:包含报警数据 2:包含启停车数据）
  *返回值的字段码：
  *          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
  *          测点个数             FIELD_SERVICE_CHANN_NUM
  *          时间值               FIELD_SERVICE_OFF_TIME
  *          转速Buff             FIELD_SERVICE_REV_BUFF

  *  #define     SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX         21081
*/
	int Srv_OffGetVibChannsTrendEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 启停车结束后，发送指定机组的启停车振动测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_VIB_DATA                         24110
	*/
	int Srv_OffRecSudVibData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 启停车结束后，发送指定机组的启停车动态测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_DYN_DATA                         24111
	*/
	int Srv_OffRecSudDynData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 启停车结束后，发送指定机组的启停车过程量测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_PROC_DATA                        24113
	*/
	int Srv_OffRecSudProcData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**启停车结束后， 同步指定公司、分厂、设备的启停车事件ID
	* #define     SERVICE_SERVICE_OFF_SUD_ADD_LOG                          24114
	*/
	int Srv_OffRecSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**通知中间件磁盘空间是否已满
	* #define     SERVICE_SERVICE_OFF_SET_DISK_FULL                          21086
	*/
	int Srv_OffSetDiskFull(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/*!
	*输入参数字段码
	*          公司ID    FIELD_SYSTEM_COMPANY
	*	       分厂名    FIELD_SYSTEM_FACTORY
	*          机组ID：  FIELD_SERVICE_PLANTID
	*          测点ID：  FIELD_SYSTEM_CHANN
	*          报警ID：  FIELD_SERVICE_ALARMID
	*          报警开始时间：  FIELD_SERVICE_OFF_TIMESTART  
	*返回值的字段码：
	*          正常波形数据：      FIELD_SERVICE_OFF_WAVE
	*          波形点数：      FIELD_SERVICE_OFF_WAVE_NUM
	*          采样频率：      FIELD_SERVICE_SAMPLE_FREQ

	#define    SERVICE_SERVICE_OFF_GETNORMALWAVE    21089
	*/
	int Srv_OffGetNormalWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

    /**监测站级历史报警统计
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL       21090
	*/
	int Srv_OffGetMonitorAlarmStatWithLevel(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**获取错误信号波形
    #define     SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE       21095
	*/
	int Srv_OffGetErrorSignalWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**设置轴心位置相关参数
    #define     SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM       21096
	*/
	int Srv_OffSetAxesLocationParam(ICommInterFace *pFixComm,IDBInterFace *pDB);
#endif