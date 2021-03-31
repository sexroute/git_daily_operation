#ifndef   __FIXDEF_DEFINE__
#define	  __FIXDEF_DEFINE__
///字段码定义
#include  "FixDef_Field.h"
///全局的枚举定义和宏定义
#include "DSE_Define.h"



#define    FIELD_CODE             1001     //返回码
#define    FIELD_MESSAGE          1002     //返回说明


//*********************功能码****************************//
#define FUNC_MODULE_TESTFUNC      10001	//测试函数
//************************实时数据读取部分******************************//
/**读取指定机组所有振动测点的各种类型的特征值，通过循环依次取得各个测点的所有类型特征值，
  *每循环依次可得到一个测点的特征值结构体
  *输入参数值：
  *                 公司ID   FIELD_SYSTEM_COMPANY
  *			         分厂名   FIELD_SYSTEM_FACTORY
  *                 机组ID   FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *                 测点ID         FIELD_SERVICE_CHANN1
  *                 特征值结构体   FIELD_SERVICE_CHARVALUE_S
  *                 报警值         FIELD_SYSTEM_ALARM_LOW
  *                 危险值         FIELD_SYSTEM_ALARM_HIGH
  *                 单位           FIELD_SERVICE_CHANN1_UNIT
  *                 指定机组测点的转速    FIELD_SERVICE_REV
  *                 时间           FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000      //读取指定机组所有振动测点的五种特征值

/**读取指定任意多个测点的各种类型的特征值。
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
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
  *          时间           FIELD_SERVICE_TIME
  *          测点类型       FIELD_SYSTEM_CHANN_TYPE
  */
#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR        20001    //读取指定机组指定振动测点的五种特征值。

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
  *                 单位           FIELD_SERVICE_CHANN1_UNIT
  *            时间            FIELD_SERVICE_TIME
  *          指定机组测点的转速    FIELD_SERVICE_REV
  *          测点类型          FIELD_SYSTEM_CHANN_TYPE
  *          报警限类型        FIELD_SYSTEM_TYPE
  *          报警上限          FIELD_SYSTEM_ALARM_LOW
  *          报警上上限        FIELD_SYSTEM_ALARM_HIGH
  *          报警下限          FIELD_SYSTEM_ALARM_LOW2
  *          报警下下限        FIELD_SYSTEM_ALARM_HIGH2
  */
#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002   //得到往复式机械设备运行状态实时趋势

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
  *          特征值结构体                     FIELD_SERVICE_CHARVALUE_S
  *          指定机组测点的转速               FIELD_SERVICE_REV
  *          时间                             FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETSPECTRUM                20003

/**得到指定任意多个振动测点的指定类型特征值
  *输入参数：
  *          公司ID         FIELD_SYSTEM_COMPANY
  *			 分厂名         FIELD_SYSTEM_FACTORY
  *          机组ID         FIELD_SERVICE_PLANTID
  *          特征值类型     FIELD_SERVICE_TREND_CHARTYPE
  *          测点buff       FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数       FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          测点特征值         FIELD_SERVICE_TREND_VALUE
  *          测点ID             FIELD_SERVICE_CHANN1
  *          时间           FIELD_SERVICE_TIME
  *          单位             FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_ON_GETFOURSPECCHAR       20004

/**得到指定机组的指定过程量测点类型的所有过程量测点的值。注意：如果测点类型是振动测点，则不返回任何值。
  *输入参数：机组ID         FIELD_SERVICE_PLANTID
  *          公司ID         FIELD_SYSTEM_COMPANY
  *			     分厂名         FIELD_SYSTEM_FACTORY
  *          测点类型       FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *          过程量值       FIELD_SERVICE_TREND_PROCESS
  *          测点ID         FIELD_SERVICE_CHANN1
  *            时间           FIELD_SERVICE_TIME
  *          单位             FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_ON_GETALLPROCVALUE        20005

/**得到指定多个过程量测点的值。
  *输入参数：
  *          公司ID           FIELD_SYSTEM_COMPANY
  *			     分厂名           FIELD_SYSTEM_FACTORY
  *          机组ID           FIELD_SERVICE_PLANTID
  *          测点buff         FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数         FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          过程量测点值     FIELD_SERVICE_TREND_PROCESS
  *          测点ID           FIELD_SERVICE_CHANN1
  *          时间             FIELD_SERVICE_TIME
  *          单位             FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_ON_GETSPECPROCVALUE       20006

/**得到指定任意多个振动测点的实时波形、频谱和所有特征值。（测点之间“，”分开）。如果多个测点，就循环提取
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
  *          测点名                           FIELD_SERVICE_CHANN1
  *          测点位号                         FIELD_SYSTEM_CHANN_ALIAS
  *          测点的波形数据                   FIELD_SERVICE_TIMEWAVE1
  *          测点的频谱数据                   FIELD_SERVICE_FREQWAVE1
  *          测点实时波形上一组数据的个数     FIELD_SERVICE_TIMEWAVE_NUM
  *          测点实际频谱一组数据的个数       FIELD_SERVICE_FREQWAVE_NUM
  *          振动测点频谱DF                   FIELD_SERVICE_FREQWAVE_DF
  *          所有特征值                       FIELD_SERVICE_CHARVALUE_S
  *          报警值                           FIELD_SYSTEM_ALARM_LOW
  *          危险值                           FIELD_SYSTEM_ALARM_HIGH
  *          单位                             FIELD_SERVICE_CHANN1_UNIT
  *          指定机组测点的转速               FIELD_SERVICE_REV
  *          时间                             FIELD_SERVICE_TIME
  *          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
  *          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
  *          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
  *          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
  *          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
  *          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
  *          测点起始索引                          FIELD_SERVICE_STARTINDEX
  *          测点结束索引                          FIELD_SERVICE_ENDINDEX
  *          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
  *          自定义参数数据BUFFSIZE                FIELD_SERVICE_CHANN_BUFFSIZE
  */
#define     SERVICE_SERVICE_ON_GETWAVE                20007

/**得到某一组轴心轨迹的波形数据。
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID   FIELD_SERVICE_PLANTID
  *          轴承位置描述     FIELD_SYSTEM_AXISLOCATION
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
  *          时间                             FIELD_SERVICE_TIME
  *          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
  *          进动方向：                       FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
  */
#define     SERVICE_SERVICE_ON_GETAXESTRACK           20008

/**得到指定机组指定一组测点轴心位置值。
  *输入参数：
  *          公司ID               FIELD_SYSTEM_COMPANY
  *			     分厂名               FIELD_SYSTEM_FACTORY
  *          机组ID               FIELD_SERVICE_PLANTID
  *          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
  *          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
  *          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
  *返回值的字段码：
  *           X方向振动测点位置  FIELD_SERVICE_AXESLOCATION_HOR
  *           Y方向振动测点位置  FIELD_SERVICE_AXESLOCATION_VER
  *            时间           FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009

/**得到指定公司和分厂,任意多个指定机组的转速。如果指定机组只有一个转速，则只取FIELD_SERVICE_REV的值
  *输入参数：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *    			   分厂名   FIELD_SYSTEM_FACTORY
  *            设备号buff     FIELD_SERVICE_CHANN_BUFF
  *            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
  *            设备号个数     FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          机组ID          FIELD_SERVICE_PLANTID
  *          指定机组的转速  FIELD_SERVICE_REV
  *          指定机组的转速2 FIELD_SERVICE_OFF_REV2
  *            时间           FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETREV                 20010

/**得到指定机组任意多个轴位移测点值
  *输入参数值：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			   分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID         FIELD_SERVICE_PLANTID
  *            测点buff         FIELD_SERVICE_CHANN_BUFF
  *            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
  *            测点个数         FIELD_SERVICE_CHANN_NUM
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
  */
#define     SERVICE_SERVICE_ON_GETSPECAXISDISPLACE    20011

/**得到指定机组所有轴位移测点值，通过循环依次得到所有轴位移值
  *输入参数值：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			   分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID   FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *            轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
  *            测点ID         FIELD_SERVICE_CHANN1
  *            报警值         FIELD_SYSTEM_ALARM_LOW
  *            危险值         FIELD_SYSTEM_ALARM_HIGH
  *            单位           FIELD_SERVICE_CHANN1_UNIT
  *            机组测点的转速   FIELD_SERVICE_REV
  *            时间           FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012

/**得到用户所观测机组范围内所有机组的测点运行状态，通过循环依次得到所有报警的测点,如果不需要通道信息则把FIELD_SERVICE_IFCHANN设为-1
  *输入参数值：用户ID         FIELD_SYSTEM_USEID
  *            是否需要通道   FIELD_SERVICE_IFCHANN
  *返回值字段码：
  *            公司ID             FIELD_SYSTEM_COMPANY
  *			       分厂名             FIELD_SYSTEM_FACTORY
  *            报警的机组ID       FIELD_SERVICE_PLANTID
  *            报警的测点ID       FIELD_SERVICE_CHANN1
  *            报警值             FIELD_SERVICE_CHANN1VALUE
  */
#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013

/**得到指定任意多个机组（机组之间通过“，”分开）所有测点特征值，通过循环依次得到所有值。
  *注：对于振动测点来说，指定那种特征类型就给那一种特征。只有振动测点有对应的转速，过程量测点没有对应转速
  *测点状态如果返回0，说明没有报警，1表示常规低报，2表示常规高报，3表示快变报警，4表示换变报警，5表示趋势报警。小的优先级最高
  *输入参数值：
  *            公司ID         FIELD_SYSTEM_COMPANY
  *			   分厂名         FIELD_SYSTEM_FACTORY
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
  *            测点类型       FIELD_SERVICE_CHANNTYPE
  *            测点特征值      FIELD_SERVICE_CHARVALUE_S
  *            转速1          FIELD_SERVICE_REV
  *            转速2          FIELD_SERVICE_REV2
  *            转速3          FIELD_SERVICE_REV3
  *            转速4          FIELD_SERVICE_REV4
  *            转速5          FIELD_SERVICE_REV5
  *            时间           FIELD_SERVICE_TIME
  */
#define     SERVICE_SERVICE_ON_GETALLCHANNVALUE       20014


/**得到指定任意个动态压力测点示功图信息。循环得到各个测点的数据
  *输入参数值：
  *          公司ID         FIELD_SYSTEM_COMPANY
  *			 分厂名         FIELD_SYSTEM_FACTORY
  *          机组ID         FIELD_SERVICE_PLANTID
  *          测点buff         FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数         FIELD_SERVICE_CHANN_NUM
  *            示功图类型     FIELD_SYSTEM_TYPE       P-V,P-a,P-t
  *返回值的字段码：
  *          测点号           FIELD_SERVICE_CHANN1
  *          测点别名         FIELD_SYSTEM_CHANN_ALIAS
  *          波形数据         FIELD_SERVICE_TIMEWAVE1
  *          X坐标数据        FIELD_SERVICE_TIMEWAVE2
  *          所有特征值       FIELD_SERVICE_CHARVALUE_S
  *          采样频率         FIELD_SERVICE_SAMPLE_FREQ
  *          采样点数         FIELD_SERVICE_SAMPLE_NUM
  *          报警值           FIELD_SYSTEM_ALARM_LOW
  *          危险值           FIELD_SYSTEM_ALARM_HIGH
  *          单位             FIELD_SERVICE_CHANN1_UNIT
  *          指定测点的转速   FIELD_SERVICE_REV
  *          时间             FIELD_SERVICE_TIME
  *          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
  *          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
  *          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
  *          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
  */
#define   SERVICE_SERVICE_ON_GETPVDATA               20015

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
  *          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
  *          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
  *          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
  *          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
  *          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
  *          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
  *          测点起始索引                          FIELD_SERVICE_STARTINDEX
  *          测点结束索引                          FIELD_SERVICE_ENDINDEX
  *          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
  *          自定义参数数据BUFFSIZE                FIELD_SERVICE_CHANN_BUFFSIZE
  */
#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016

/**通过分厂名得到下面设备的报警状态,并且得到指定机组的转速
  *输入参数值：
  *            公司ID   FIELD_SYSTEM_COMPANY
  *			       分厂名   FIELD_SYSTEM_FACTORY
  *            机组ID   FIELD_SERVICE_PLANTID
  *返回值字段码：
  *            报警的机组ID         FIELD_SERVICE_PLANTID
  *            指定设备的转速       FIELD_SERVICE_REV
  *            指定机组的转速2      FIELD_SERVICE_OFF_REV2
  */
#define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017

/**得到指定指定机组和指定可以做启停车的测点的波特图数据，一次只能得到一组数据，如果没有指定测点，则发送所有测点的数据
  *输入参数：
  *          公司ID           FIELD_SYSTEM_COMPANY
  *		       分厂名           FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_ON_GETBODE                20018


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
*/
#define     SERVICE_SERVICE_ON_GETWATERFALL            20019

/**得到指定机组振动测点的细化频谱分析和波形。
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020

/**得到指定指定机组和测点的相关信息
  *输入参数：
  *          公司ID           FIELD_SYSTEM_COMPANY
  *			  分厂名           FIELD_SYSTEM_FACTORY
  *          机组ID           FIELD_SERVICE_PLANTID
  *          测点号           FIELD_SERVICE_CHANN1
  *返回值的字段码：
  *          额定转速：       FIELD_SERVICE_REV
  *          实时转速：       FIELD_SERVICE_REV2
  *          采样频率         FIELD_SERVICE_SAMPLE_FREQ
  *          采样频率         FIELD_SERVICE_SAMPLE_NUM
  */
#define     SERVICE_SERVICE_ON_GETSPECCHANNINFO           20021

/**读取指定设备振动测点的指定倍频的特征值和相位，
  *如果指定测点了，就只取指定测点的特征值和相位
  *如果没有指定测点，则获得所有测点通过循环依次取得各个测点的对应倍频特征值和相位，每循环依次可得到一个测点指定趋势特征值和相位
  *输入参数值：
  *            公司ID      FIELD_SYSTEM_COMPANY
  *			       分厂名      FIELD_SYSTEM_FACTORY
  *            机组ID      FIELD_SERVICE_PLANTID
  *            测点ID      FIELD_SERVICE_CHANN1
  *            特征值类型  FIELD_SERVICE_TREND_CHARTYPE
  *返回值的字段码：
  *            测点ID      FIELD_SERVICE_CHANN1
  *            指定特征值  FIELD_SERVICE_TREND_VALUE
  *            对应的相位  FIELD_SERVICE_TREND_PHASE
  *            转速：      FIELD_SERVICE_REV
  */
#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022

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
*          测点名称                   FIELD_SERVICE_CHANN1
*          测点波形                   FIELD_SERVICE_TIMEWAVE1
*          测点特征值                 FIELD_SERVICE_CHARVALUE_S
*          横坐标                     FIELD_SERVICE_FREQWAVE2
*          转速                       FIELD_SERVICE_REV
*          时间                       FIELD_SERVICE_TIME
*          吸气阀开启相位角           FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角           FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM4
*/
#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023

/**集团级实时报警统计
  *输入参数值：  公司ID          FIELD_SYSTEM_COMPANY
  *		         分厂名          FIELD_SYSTEM_FACTORY
  *返回值的字段码：
  *		         设备数          FIELD_SERVICE_PLANT_NUM
  *		         报警设备数      FIELD_SERVICE_ALARM_PLANTNUM
  *		         运行设备数      FIELD_SERVICE_RUN_PLANTNUM
  *		         停车设备数      FIELD_SERVICE_STOP_PLANTNUM
  */
#define   SERVICE_SERVICE_ON_GROUPALARMSTAT              20024

/**批量读取监测站级实时报警统计
*输入参数值：公司ID,分厂ID,设备ID;          FIELD_SYSTEM_COMPANY2
*		     
*返回值的字段码：
*		     测点名           FIELD_SERVICE_CHANN1
*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		     特征值           FIELD_SERVICE_CHANN1VALUE
*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
*		     转速1            FIELD_SERVICE_REV
*		     报警ID           FIELD_SERVICE_ALARMID
*		     报警同步标识     FIELD_SERVICE_ALARM_SYNC (-1: 未知0: 自己判断报警 1: 同步自下级)
*/
#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT           20033

/**读取公司级实时报警统计
  *输入参数值：公司ID          FIELD_SYSTEM_COMPANY
  *		     分厂名           FIELD_SYSTEM_FACTORY
  *	         设备名           FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *          公司名           FIELD_SYSTEM_COMPANY
  *		     分厂名           FIELD_SYSTEM_FACTORY
  *		     设备名           FIELD_SERVICE_PLANTID
  *		     测点数           FIELD_SERVICE_CHANN_NUM
  *		     报警状态         FIELD_SERVICE_ALARM_STATUS
  *		     启停车状态       FIELD_SERVICE_SUD_STATUS (1, 停车 0 开车)
  *		     转速1            FIELD_SERVICE_REV
  *		     报警ID           FIELD_SERVICE_ALARMID
  *		     报警同步标识	 FIELD_SERVICE_ALARM_SYNC  (-1: 未知0: 自己判断报警 1: 同步自下级)
  */
#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT         20025

/**读取监测站级实时报警统计
  *输入参数值：公司ID          FIELD_SYSTEM_COMPANY
  *		     分厂名           FIELD_SYSTEM_FACTORY
  *	         设备名           FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *		     测点名           FIELD_SERVICE_CHANN1
  *		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
  *		     特征值           FIELD_SERVICE_CHANN1VALUE
  *		     报警状态         FIELD_SERVICE_CHANN1_STATUS
  *		     转速1            FIELD_SERVICE_REV
  *		     报警ID           FIELD_SERVICE_ALARMID
  *		     报警同步标识	 FIELD_SERVICE_ALARM_SYNC  (-1: 未知0: 自己判断报警 1: 同步自下级)
   */
#define     SERVICE_SERVICE_ON_MONITORALARMSTAT         20026

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
*		   轴心轨迹圆点X坐标   FIELD_SERVICE_AXIS_COORX
*		   轴心轨迹圆点Y坐标   FIELD_SERVICE_AXIS_COORY
*		   报警椭圆水平轴半径  FIELD_SERVICE_HOR_RADIUS
*		   报警椭圆垂直轴半径  FIELD_SERVICE_VER_RADIUS
*/
#define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION         20027

/**往复式系统应力监测，得到指定机组指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		       分厂名          　　　  FIELD_SYSTEM_FACTORY
*          机组ID          　　　  FIELD_SERVICE_PLANTID
*          汽缸号                  FIELD_SERVICE_CYLINDERNO
*          缸体振动测点            FIELD_SERVICE_CHANN1
*返回值的字段码：
*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*          振动波形数据   　　　　 FIELD_SERVICE_FREQWAVE2
*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
*		       特征值结构体		         FIELD_SERVICE_CHARVALUE_S
*		       特征值结构体2		       FIELD_SERVICE_CHARVALUE_S2
*		       测点名				   				 FIELD_SYSTEM_CHANN
*		       测点名2				   			 FIELD_SYSTEM_CHANN2
*		   		 测点1角度参数1		   		 FIELD_SERVICE_ANGLEPARAM1
*		       测点1角度参数2		   		 FIELD_SERVICE_ANGLEPARAM2
*		       测点1角度参数3		       FIELD_SERVICE_ANGLEPARAM3
*		       测点1角度参数4		       FIELD_SERVICE_ANGLEPARAM4
*		       测点2角度参数1		       FIELD_SERVICE_ANGLEPARAM5
*		       测点2角度参数2		       FIELD_SERVICE_ANGLEPARAM6
*		       测点2角度参数3		       FIELD_SERVICE_ANGLEPARAM7
*		       测点2角度参数4		       FIELD_SERVICE_ANGLEPARAM8
*		       时间				   	   			 FIELD_SERVICE_TIME 
*/
#define     SERVICE_SERVICE_ON_GETDYNSTRESSWAVE                20028


/**得到某一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*		   分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          波形点数             FIELD_SERVICE_TIMEWAVE_NUM
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          指定机组测点的转速             FIELD_SERVICE_REV
*          转子旋向：                     FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
*/
#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029

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
*/
#define     SERVICE_SERVICE_ON_GETCEPSTRUM                   20030

/**得到指定一对振动测点的实时全谱和全谱瀑布图。
*输入参数：
*          公司ID                           FIELD_SYSTEM_COMPANY
*		   分厂名                           FIELD_SYSTEM_FACTORY
*          机组ID                           FIELD_SERVICE_PLANTID
*          轴承位置描述                     FIELD_SYSTEM_AXISLOCATION
*          水平方向振动测点ID               FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID               FIELD_SERVICE_CHANNID_VER
*          波形点数                         FIELD_SERVICE_TIMEWAVE_NUM
*          是否应用新算法                   FIELD_SERVICE_FUNCTION_TYPE
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          时间                             FIELD_SERVICE_TIME
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*/
#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031

/** 获取异常数据检测结果
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *            分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            测点                   FIELD_SYSTEM_CHANN
  *            异常时间               FIELD_SERVICE_TIME
  *            异常类别               FIELD_SYSTEM_TYPE
  *            异常原因               FIELD_SYSTEM_STATUS_DISCRIPTION
  *\description
               异常类别(FIELD_SYSTEM_TYPE)字段类型： GE_ABNORMAL_DATA_CATEGORY
                                     包括(越界数据、趋势波动的转速，不合理数据)
  */
#define     SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD       20032


/***************************历史读取数据部分*************************/
/**得到当前时间从向后推一个趋势类型的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *@param：  输入参数字段码
  *          公司ID       FIELD_SYSTEM_COMPANY
  *			 分厂名       FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_OFF_GETCURTREND           21001

/**得到指定公司、分厂、机组和测点指定时刻的频谱数据
  *输入参数：
  *          公司ID       FIELD_SYSTEM_COMPANY
  *	    	 分厂名       FIELD_SYSTEM_FACTORY
  *          机组ID       FIELD_SERVICE_PLANTID
  *          测点ID       FIELD_SERVICE_OFF_CHANN1
  *          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
  *          分析线数：   FIELD_SERVICE_OFF_FREQANALYSIS_LINES
  *          时间         FIELD_SERVICE_OFF_TIME
  *          起始频率     FIELD_SERVICE_ZOOM_STARTFREQ
  *          截止频率     FIELD_SERVICE_ZOOM_STOPFREQ
  *返回值的字段码：
  *          波形数据        FIELD_SERVICE_OFF_WAVE
  *          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
  *          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
  *          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
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
  *          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
  *          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
  *          测点起始索引                          FIELD_SERVICE_STARTINDEX
  *          测点结束索引                          FIELD_SERVICE_ENDINDEX
  *          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
  */
#define     SERVICE_SERVICE_OFF_GETSPECFREQWAVE       21002

/**得到指定时间段、特征值类型、任意多个测点的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *输入参数字段码：
  *          特征值类型   FIELD_SERVICE_TREND_CHARTYPE
  *          测点类型：   FIELD_SERVICE_OFF_CHANNTYPE
  *          公司ID       FIELD_SYSTEM_COMPANY
  *			 分厂名       FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_OFF_GETTREND              21003


/**得到指定公司、分厂、机组号、振动测点号、起始时间段的所有特征值趋势数据
  *输入参数字段码：
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	    	 分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID      FIELD_SERVICE_PLANTID
  *          测点类型：  FIELD_SERVICE_CHANNTYPE
  *          测点ID      FIELD_SERVICE_OFF_CHANN1
  *          起始时间    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间    FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          特征值结构体     FIELD_SERVICE_CHARVALUE_S
  *          时间             FIELD_SERVICE_OFF_TIME
  */
#define    SERVICE_SERVICE_OFF_GETAVIBCHANNTREND      21004

/**得到指定公司、分厂名、机组ID、时间段、水平测点ID和垂直测点ID的轴心位置历史数据，通过循环依次得到所有数据和对应时间，每次取得数据对应     时间都是相同的
  *输入参数字段码：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *   	     分厂名   FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND  21005


/**得到指定两个振动测点的特征值信息、报警限等信息（循环两次得到两个测点的数据）
  *输入参数：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			 分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID          FIELD_SERVICE_PLANTID
  *          测点一          FIELD_SERVICE_OFF_CHANN1
  *          测点一时间值    FIELD_SERVICE_OFF_TIME
  *          公司ID2         FIELD_SYSTEM_COMPANY2
  *			 分厂名2         FIELD_SYSTEM_FACTORY2
  *          机组ID2         FIELD_SERVICE_PLANTID2
  *          测点二          FIELD_SERVICE_OFF_CHANN2
  *          测点二时间值    FIELD_SERVICE_OFF_TIME2
  *返回值的字段码：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			 分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID          FIELD_SERVICE_PLANTID
  *          测点ID          FIELD_SERVICE_CHANN1
  *          测点别名        FIELD_SYSTEM_CHANN_ALIAS
  *          特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *          转速            FIELD_SERVICE_OFF_REV
  *          报警值          FIELD_SYSTEM_ALARM_LOW
  *          危险值          FIELD_SYSTEM_ALARM_HIGH
  *          单位            FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO    21006

/**得到指定历史时刻、机组振动测点的细化频谱分析和波形。
  *输入参数：
  *          公司ID           FIELD_SYSTEM_COMPANY
  *			     分厂名           FIELD_SYSTEM_FACTORY
  *          机组ID           FIELD_SERVICE_PLANTID
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
  */
#define     SERVICE_SERVICE_OFF_GETZOOMSPEC             21007


/**得到指定公司、分厂、机组、测点和时间的一组轴心轨迹的波形数据。
*输入参数：
*          公司ID   FIELD_SYSTEM_COMPANY
*			     分厂名   FIELD_SYSTEM_FACTORY
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
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                       FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*/
#define     SERVICE_SERVICE_OFF_GETAXESTRACK              21008

/**得到指定公司、分厂、设备的任意多个振动测点的特征值信息、报警限等信息（依次循环测点的数据）
  *输入参数：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID          FIELD_SERVICE_PLANTID
  *          测点一时间值    FIELD_SERVICE_OFF_TIME
  *          测点buff        FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数        FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          测点ID          FIELD_SERVICE_CHANN1
  *          测点别名        FIELD_SYSTEM_CHANN_ALIAS
  *          特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *          转速            FIELD_SERVICE_OFF_REV
  *          报警值          FIELD_SYSTEM_ALARM_LOW
  *          危险值          FIELD_SYSTEM_ALARM_HIGH
  *          单位            FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO  21009

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
  */
#define     SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO    21010

/*!得到指定起止时间的指定机组和指定振动测点类型的报警记录
  *输入参数字段码
  *          公司ID   FIELD_SYSTEM_COMPANY
  *    	     分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID： FIELD_SERVICE_PLANTID
  *          测点名： FIELD_SERVICE_CHANN1
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          报警开始时间:   FIELD_SERVICE_OFF_TIMESTART
  *          报警结束时间:   FIELD_SERVICE_OFF_TIMEEND
  *          报警类型：      FIELD_SYSTEM_TYPE
  *          事件ID          FIELD_SYSTEM_EVENT_ID
  *          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
  */
#define     SERVICE_OFF_GETVIBCHANNALARMLOG               21011

/**得到指定时间的指定测点的报警记录的波形和各种特征值，对于多个测点就循环得到
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			 分厂名          FIELD_SYSTEM_FACTORY
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
  *          自定义参数数据BUFF：  FIELD_SERVICE_LEARNPARAM_BUFF
  */
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD       21012


/*!得到指定时间范围的指定机组的启停车记录
  *输入参数字段码
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          启停车类型：  FIELD_SYSTEM_TYPE
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  */
#define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD          21013

/*!得到指定启停车事件ID和指定公司、分厂机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分   *别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *			     分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
  *对应时间的微秒        FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  */
#define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD          21014

/*!得到指定启停车事件ID、指定公司、分厂、机组、指定测点和指定转速的启停车的详细数据。
  *输入参数字段码
  *          公司ID        FIELD_SYSTEM_COMPANY
  *			     分厂名        FIELD_SYSTEM_FACTORY
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
  *          是否有高精度波形           FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO
  *		     高精度波形			FIELD_SERVICE_SEND_FLOAT_WAVE1
  *		     高精度频谱  FIELD_SERVICE_SEND_FLOAT_FREQ1
  */
#define    SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD           21015

/*!得到指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          公司ID      FIELD_SYSTEM_COMPANY
*			     分厂名      FIELD_SYSTEM_FACTORY
*          机组ID：    FIELD_SERVICE_PLANTID
*          测点ID：    FIELD_SERVICE_OFF_CHANN1
*  启停车数据保存时间: FIELD_SERVICE_OFF_TIME
*      对应时间的微秒  FIELD_SERVICE_MICROSECOND
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数    FIELD_SERVICE_ZOOM_LINES
*          调用接口状态：FIELD_SYSTEM_STATUS
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*/
#define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD      21016

/**得到指定公司、分厂、机组、时间段、任意多个振动测点测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *输入参数字段码：
  *          公司ID        FIELD_SYSTEM_COMPANY
  *			     分厂名        FIELD_SYSTEM_FACTORY
  *          机组ID        FIELD_SERVICE_PLANTID
  *          测点类型：    FIELD_SERVICE_CHANNTYPE
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
  */
#define     SERVICE_SERVICE_OFF_GETVIBCHANNSTREND         21017

/**得到当前时间向后推一个趋势类型的趋势数据。指定公司、分厂、机组、任意多个振动测点或者动态测点的所有特征值趋势数据，
  *通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
  *输入参数字段码：
  *          趋势类型      FIELD_SERVICE_OFF_TRENDTYPE
  *          公司ID        FIELD_SYSTEM_COMPANY
  *			     分厂名        FIELD_SYSTEM_FACTORY
  *          机组ID        FIELD_SERVICE_PLANTID
  *          测点类型：    FIELD_SERVICE_CHANNTYPE
  *          测点buff      FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数      FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
  *          测点个数             FIELD_SERVICE_CHANN_NUM
  *          时间值               FIELD_SERVICE_OFF_TIME
  *          转速Buff             FIELD_SERVICE_REV_BUFF
  */
#define     SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND      21018

/**得到指定两个振动测点的历史波形（循环两次得到两个测点的数据）
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
  *          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
  *          分析线数        FIELD_SERVICE_OFF_FREQANALYSIS_LINES
  *返回值的字段码：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID          FIELD_SERVICE_PLANTID
  *          测点ID          FIELD_SERVICE_CHANN1
  *          波形数据        FIELD_SERVICE_OFF_WAVE
  *          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
  *          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
  *          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
  *          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
  *          特征值结构体    FIELD_SERVICE_CHARVALUE_S
  *          报警值          FIELD_SYSTEM_ALARM_LOW
  *          危险值          FIELD_SYSTEM_ALARM_HIGH
  *          单位            FIELD_SERVICE_CHANN1_UNIT
  *          转速            FIELD_SERVICE_OFF_REV
  */
#define     SERVICE_SERVICE_OFF_GETTWOCHANNDATA           21019

/**得到指定公司、分厂、设备的任意多个过程测点的值信息、报警限等信息（依次循环测点的数据）
  *输入参数：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID          FIELD_SERVICE_PLANTID
  *          测点时间值      FIELD_SERVICE_OFF_TIME
  *          测点buff        FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数        FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          测点ID          FIELD_SERVICE_CHANN1
  *          测点别名        FIELD_SYSTEM_CHANN_ALIAS
  *          特征值          FIELD_SERVICE_TREND_PROCESS
  *          转速            FIELD_SERVICE_OFF_REV
  *          报警限          FIELD_SYSTEM_ALARM_LOW
  *          危险限          FIELD_SYSTEM_ALARM_HIGH
  *          单位            FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO   21020

/*!得到指定公司、分厂、机组、任意多测点的指定启停车事件的趋势数据。
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
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
  *          对应时间的微秒       FIELD_SERVICE_MICROSECOND
  *          转速:                FIELD_SERVICE_OFF_REV
  *          转速2：              FIELD_SERVICE_OFF_REV2
  */
#define    SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND       21021

/*!得到指定公司、分厂、机组、事件、任意多测点、保存时间的详细数据
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID：        FIELD_SERVICE_PLANTID
  *          测点buff        FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数        FIELD_SERVICE_CHANN_NUM
  *          启停车事件ID    FIELD_SYSTEM_EVENT_ID
  *    启停车数据保存时间    FIELD_SERVICE_OFF_TIME
  *          对应时间的微秒  FIELD_SERVICE_MICROSECOND
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
  */
#define    SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO    21022

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
  */
#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO      21023

/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
  *输入参数：
  *          公司ID          FIELD_SYSTEM_COMPANY
  *			     分厂名          FIELD_SYSTEM_FACTORY
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
  */
#define     SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND       21024

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
*          报警点标示     FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND       21025

/**得到指定报警事件的报警数据保存的开始截止时间
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID               FIELD_SERVICE_PLANTID
  *          事件ID:              FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  */
#define SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE               21026

/**得到指定报警事件的报警数据保存的开始截止时间
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			     分厂名   FIELD_SYSTEM_FACTORY
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
  *          报警点类型      FIELD_SYSTEM_TYPE
  */
#define SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA            21027

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
*          时间值              FIELD_SERVICE_OFF_TIME
*          报警类型            FIELD_SYSTEM_TYPE
*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
*          测点门限Buff        FIELD_SERVICE_THRESHOLD_BUFF
*          门限个数            FIELD_SERVICE_LEN
*          事件类型            FIELD_SYSTEM_EVENT_TYPE
*          报警点标示          FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND          21028

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
*          报警类型            FIELD_SYSTEM_TYPE
*          报警点标示          FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND      21029

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
*/
#define     SERVICE_SERVICE_OFF_GETPROCALARMTREND           21030

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
*/
#define     SERVICE_SERVICE_OFF_GETPROCALARMDATA            21031

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
*          门限个数            FIELD_SERVICE_LEN
*          报警类型            FIELD_SYSTEM_TYPE
*          转速:               FIELD_SERVICE_OFF_REV
*/
#define     SERVICE_SERVICE_OFF_GETTRENDALARMDATA        21032

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
*/
#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA    21033

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
   *         报警值          FIELD_SYSTEM_ALARM_LOW
  *          危险值          FIELD_SYSTEM_ALARM_HIGH
  */
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE  21034


/**得到指定历史时刻、机组示功图测点的波形。
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
  *          采样频率        FIELD_SERVICE_SAMPLE_FREQ
 *          单位             FIELD_SERVICE_CHANN1_UNIT
 *          测点别名         FIELD_SYSTEM_CHANN_ALIAS
  *          转速             FIELD_SERVICE_REV
  *            吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
  *            吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
  *            排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
  *            排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
  */
#define     SERVICE_SERVICE_OFF_GETPVDATA                   21035

/**得到指定历史时刻、机组活塞杆沉降量测点的波形和频谱。
  *输入参数：
  *          公司ID           FIELD_SYSTEM_COMPANY
  *			     分厂名           FIELD_SYSTEM_FACTORY
  *          机组ID           FIELD_SERVICE_PLANTID
  *          测点             FIELD_SERVICE_OFF_CHANN1
  *          时间             FIELD_SERVICE_OFF_TIME
  *          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
  *返回值的字段码：
  *          测点的波形数据   FIELD_SERVICE_OFF_WAVE
  *          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
  *          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
  *          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
  *          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
  *          特征值结构体     FIELD_SERVICE_CHARVALUE_S
  *          报警值           FIELD_SYSTEM_ALARM_LOW
  *          危险值           FIELD_SYSTEM_ALARM_HIGH
  *          测点别名         FIELD_SYSTEM_CHANN_ALIAS
  *          单位             FIELD_SERVICE_CHANN1_UNIT
  *          转速             FIELD_SERVICE_REV
  *          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
  *          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
  *          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
  *          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
  *          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
  *          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
  *          测点起始索引                          FIELD_SERVICE_STARTINDEX
  *          测点结束索引                          FIELD_SERVICE_ENDINDEX
  *          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
  */
#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036

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
*            测点波形                     FIELD_SERVICE_TIMEWAVE1
*            测点特征值                   FIELD_SERVICE_CHARVALUE_S
*            横坐标                       FIELD_SERVICE_FREQWAVE2
*            转速                         FIELD_SERVICE_OFF_REV
*            吸气阀开启相位角             FIELD_SERVICE_ANGLEPARAM1
*            吸气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM2
*            排气阀开启相位角             FIELD_SERVICE_ANGLEPARAM3
*            排气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM4
  */
#define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA           21037

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
*            转速1          FIELD_SERVICE_REV
*            转速2          FIELD_SERVICE_REV2
*            转速3          FIELD_SERVICE_REV3
*            转速4          FIELD_SERVICE_REV4
*            转速5          FIELD_SERVICE_REV5
*/
#define     SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE    21038

/**得到指定公司、分厂、机组、测点,指定启停车事件和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID               FIELD_SYSTEM_COMPANY
*          分厂名               FIELD_SYSTEM_FACTORY
*          机组ID               FIELD_SERVICE_PLANTID
*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
*          启停车事件ID         FIELD_SYSTEM_EVENT_ID
*          转速                 FIELD_SERVICE_OFF_REV
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          转子旋向：                     FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*          转速                           FIELD_SERVICE_OFF_REV
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
*/
#define     SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK           21039

/**往复式系统报警应力监测分析功能，得到指定机组指定报警时间、指定缸号的应力波形和振动测点波形
*输入参数：
*            公司ID        　　　    FIELD_SYSTEM_COMPANY
*		     分厂名          　　　  FIELD_SYSTEM_FACTORY
*            机组ID          　　　  FIELD_SERVICE_PLANTID
*            汽缸号                  FIELD_SERVICE_CYLINDERNO
*            事件ID                  FIELD_SYSTEM_EVENT_ID
*            报警时间                FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*            气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
*            惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
*            活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
*            波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
*		         特征值结构体		         FIELD_SERVICE_CHARVALUE_S
*		         特征值结构体2		       FIELD_SERVICE_CHARVALUE_S2
*		         测点名				   				 FIELD_SYSTEM_CHANN
*		         测点名2				   			 FIELD_SYSTEM_CHANN2
*		   		   测点1角度参数1		   FIELD_SERVICE_ANGLEPARAM1
*		         测点1角度参数2		   FIELD_SERVICE_ANGLEPARAM2
*		         测点1角度参数3		   FIELD_SERVICE_ANGLEPARAM3
*		         测点1角度参数4		   FIELD_SERVICE_ANGLEPARAM4
*		         测点2角度参数1		   FIELD_SERVICE_ANGLEPARAM5
*		         测点2角度参数2		   FIELD_SERVICE_ANGLEPARAM6
*		         测点2角度参数3		   FIELD_SERVICE_ANGLEPARAM7
*		         测点2角度参数4		   FIELD_SERVICE_ANGLEPARAM8
*			 			 时间				   			 FIELD_SERVICE_TIME 
*/
#define     SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE       21040

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
*            事件ID                     FIELD_SYSTEM_EVENT_ID
*            报警时间值                 FIELD_SERVICE_OFF_TIME
*返回字段：
*            波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
*            采样频率                   FIELD_SERVICE_SAMPLE_FREQ
*            测点名称                   FIELD_SERVICE_CHANN1
*            测点波形                   FIELD_SERVICE_TIMEWAVE1
*            测点特征值                 FIELD_SERVICE_CHARVALUE_S
*            横坐标                     FIELD_SERVICE_FREQWAVE2
*            是否有高精度波形           FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO
*            高精度波形				    FIELD_SERVICE_SEND_FLOAT_WAVE1
*/
#define   SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA      21041

/*!得到临时在线指定机组、指定测点的指定时间保存启停车的详细数据。
*输入参数字段码
*          公司ID      FIELD_SYSTEM_COMPANY
*		   分厂名      FIELD_SYSTEM_FACTORY
*          机组ID：    FIELD_SERVICE_PLANTID
*          测点ID：    FIELD_SERVICE_OFF_CHANN1
*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
*          波形数据个数  FIELD_SERVICE_OFF_WAVE_NUM
*          分析线数    FIELD_SERVICE_ZOOM_LINES
*返回值的字段码：
*          相位：      FIELD_SERVICE_OFF_PHASE
*          幅值：      FIELD_SERVICE_OFF_AMP
*          波形：      FIELD_SERVICE_OFF_WAVE
*          频谱：      FIELD_SERVICE_OFF_FREQWAVE
*          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
*          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
*          转速:       FIELD_SERVICE_OFF_REV
*          频谱点数：  FIELD_SERVICE_ZOOM_LINES
*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
*/
#define    SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX      21042

/*!得到临时在线指定启停车事件ID和指定公司、分厂机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分   *别通过转速或保存这组数据的时刻、机组号和测点号
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *			     分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          测点ID：    FIELD_SERVICE_OFF_CHANN1
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *返回值的字段码：
  *          转速:       FIELD_SERVICE_OFF_REV
  *          时间        FIELD_SERVICE_OFF_TIME
  *对应时间的微秒        FIELD_SERVICE_MICROSECOND
  *          相位：      FIELD_SERVICE_OFF_PHASE
  *          幅值：      FIELD_SERVICE_OFF_AMP
  *          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
  */
#define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX          21043

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
  */
#define    SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME          21044

/**得到指定时间的月、公司、分厂、机组各个测点的各种特征值最大值最小值平均值，测点特征值buff中依次是最大值、最小值和平均值，特征值个数应该是3
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID               FIELD_SERVICE_PLANTID
  *          时间值               FIELD_SERVICE_OFF_TIME
  *返回值的字段：
  *          测点特征值Buff      FIELD_SERVICE_CHARVALUE_SBUFF
  *          特征值个数          FIELD_SERVICE_CHANN_NUM
  *          测点ID              FIELD_SERVICE_CHANN1
  */
#define SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT         21051

/**得到指定公司、分厂、指定时间范围的报警信息、启停车信息。
  *振动通道报警次数、过程量通道报警、累计停车次数、累计停机时间、累计运行时间，时间单位为小时
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  *返回的字段：
  *          机组ID               FIELD_SERVICE_PLANTID
  *          所有测点报警次数     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *          累计停车次数         FIELD_SERVICE_LEN
  *          累计停机时间(小时)   FIELD_SERVICE_SAMPLE_FREQ
  *          累计运行时间(小时)   FIELD_SERVICE_SAMPLE_NUM
  */
#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052

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
  */
#define SERVICE_SERVICE_OFF_CORRDIGN                        21053

/**集团级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     设备数           FIELD_SERVICE_PLANT_NUM
*		     报警设备数       FIELD_SERVICE_ALARM_PLANTNUM
*/
#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT             21054

/**公司级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            分厂名           FIELD_SYSTEM_FACTORY
*		     设备名           FIELD_SERVICE_PLANTID
*		     报警次数         FIELD_SERVICE_ALARM_PLANTNUM
*		     停车次数         FIELD_SERVICE_STOP_PLANTNUM
*/
#define    SERVICE_SERVICE_OFF_COMPANYALARMSTAT         21055

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		        分厂名           FIELD_SYSTEM_FACTORY
*	            设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		        报警ID          FIELD_SERVICE_ALARMID
*		        测点编号         FIELD_SERVICE_CHANN1
*		        测点位号         FIELD_SYSTEM_CHANN_ALIAS
*		        特征值           FIELD_SERVICE_TREND_VALUE
*		        转速1            FIELD_SERVICE_REV
*		      报警开始时间      FIELD_SERVICE_OFF_TIMESTART
*		      报警结束时间      FIELD_SERVICE_OFF_TIMEEND
*		        报警类型          FIELD_SERVICE_ALARM_TYPE
*                成功标示         FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT         21056

/**单通道多特征值报警查询，得到指定报警测点的各种类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*        机组ID           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*        特征值结构体     FIELD_SERVICE_CHARVALUE_S
*        时间             FIELD_SERVICE_TIME
*        对应时间的微秒   FIELD_SERVICE_MICROSECOND
*/
#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES   21057

/**多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		         分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警测点buff     FIELD_SERVICE_CHANN_BUFF
*            报警测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
*            报警测点个数     FIELD_SERVICE_CHANN_NUM
*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*            时间                 FIELD_SERVICE_TIME
*            对应时间的微秒       FIELD_SERVICE_MICROSECOND
*/
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE   21058

/**得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹
*输入参数：
*          公司ID            FIELD_SYSTEM_COMPANY
*		   分厂名            FIELD_SYSTEM_FACTORY
*          机组ID            FIELD_SERVICE_PLANTID
*          轴承位置描述      FIELD_SYSTEM_AXISLOCATION
*          水平方向测点ID    FIELD_SERVICE_CHANNID_HOR
*          垂直方向测点ID    FIELD_SERVICE_CHANNID_VER
*          波形数据个数      FIELD_SERVICE_OFF_WAVE_NUM
*          时间值          　FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*         X方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
*         Y方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
*         实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
*         X方向测点特征值     FIELD_SERVICE_CHARVALUE_S
*         Y方向测点特征值     FIELD_SERVICE_CHARVALUE_S2
*         转速                FIELD_SERVICE_OFF_REV
*		  轴心轨迹圆点X坐标   FIELD_SERVICE_AXIS_COORX
*		  轴心轨迹圆点Y坐标   FIELD_SERVICE_AXIS_COORY
*		  报警椭圆水平轴半径  FIELD_SERVICE_HOR_RADIUS
*		  报警椭圆垂直轴半径  FIELD_SERVICE_VER_RADIUS
*/
#define     SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION            21059

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
*            其它振动特征值类型   FIELD_SERVICE_OTHER_CHARTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
*/
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060

/**得到往复式机械设备所有测点的特征值历史趋势比较；
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            测点buff         FIELD_SERVICE_CHANN_BUFF
*            测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
*            测点个数         FIELD_SERVICE_CHANN_NUM
*            测点类型         FIELD_SERVICE_OFF_CHANNTYPE
*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
*            起始时间         FIELD_SERVICE_OFF_TIMESTART
*            终止时间         FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*            测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
*            转速             FIELD_SERVICE_REV_BUFF
*            时间值           FIELD_SERVICE_OFF_TIME
*/
#define     SERVICE_SERVICE_OFF_GETHISCOMPARE         21061

/**得到往复式机械设备所有通道指定时间的特征值和波形数据比较
*输入参数：
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
*/
#define     SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE         21062

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
*/
#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063

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
*/
#define     SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT       21064

/**监测站级历史报警统计，优化读取数据，读取20条记录以后的特征值和转速
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*		     测点编号         FIELD_SERVICE_CHANN1
*		     报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*		     特征值           FIELD_SERVICE_TREND_VALUE
*		     转速1            FIELD_SERVICE_REV
*/
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065

/**得到指定公司、分厂、机组、测点和时间的一组提纯轴心轨迹的波形数据。
*输入参数：
*          公司ID                         FIELD_SYSTEM_COMPANY
*		   分厂名                         FIELD_SYSTEM_FACTORY
*          机组ID                         FIELD_SERVICE_PLANTID
*          水平方向振动测点ID             FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID             FIELD_SERVICE_CHANNID_VER
*          轴承位置描述                   FIELD_SYSTEM_AXISLOCATION
*          波形数据个数                   FIELD_SERVICE_OFF_WAVE_NUM
*          时间                           FIELD_SERVICE_OFF_TIME
*返回值的字段码：
*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
*          水平方向特征值结构体           FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值结构体           FIELD_SERVICE_CHARVALUE_S2
*          转子旋向：                     FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*          进动方向：                     FIELD_SYSTEM_STATUS//1:为反进动,2为正进动
*/
#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK           21066

/**得到指定公司、分厂机组和测点指定时刻的倒谱数据
*输入参数：
*          公司ID       FIELD_SYSTEM_COMPANY
*	       分厂名       FIELD_SYSTEM_FACTORY
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
*          吸气阀开启相位角     FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角     FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM4
*/
#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM         21067



/**往复机械报警查询，得到指定报警ID最先报警的测点名、特征值趋势
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*            分厂名           FIELD_SYSTEM_FACTORY
*            机组ID           FIELD_SERVICE_PLANTID
*            报警ID           FIELD_SERVICE_ALARMID
*返回值的字段码：
*            报警测点名              FIELD_SERVICE_CHANN1
*            测点类型                FIELD_SERVICE_CHANNTYPE
*            报警测点的特征值结构体  FIELD_SERVICE_CHARVALUE_S
*            时间                    FIELD_SERVICE_TIME
*            对应时间的微秒          FIELD_SERVICE_MICROSECOND
*/
#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068


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
*          吸气阀开启相位角     FIELD_SERVICE_ANGLEPARAM1
*          吸气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM2
*          排气阀开启相位角     FIELD_SERVICE_ANGLEPARAM3
*          排气阀关闭相位角     FIELD_SERVICE_ANGLEPARAM4
*          自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
*/
#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD     21069

/**得到指定公司、分厂机组和一对振动测点指定时刻的全谱数据
*输入参数：
*          公司ID               			FIELD_SYSTEM_COMPANY
*		   分厂名               			FIELD_SYSTEM_FACTORY
*          机组ID               			FIELD_SERVICE_PLANTID
*          水平方向振动测点ID  			    FIELD_SERVICE_CHANNID_HOR
*          垂直方向振动测点ID   		    FIELD_SERVICE_CHANNID_VER
*          轴承位置描述        			    FIELD_SYSTEM_AXISLOCATION
*          波形数据个数         			FIELD_SERVICE_OFF_WAVE_NUM
*          时间                             FIELD_SERVICE_OFF_TIME
*          是否应用新算法                   FIELD_SERVICE_FUNCTION_TYPE
*返回值的字段码：
*          全谱频率和幅值信息结构体         FIELD_SERVICE_LEARNPARAM_BUFF
*          全谱信息结构体大小               FIELD_SERVICE_CHANN_BUFFSIZE
*          指定机组测点的转速               FIELD_SERVICE_REV
*          水平方向特征值                   FIELD_SERVICE_CHARVALUE_S
*          垂直方向特征值                   FIELD_SERVICE_CHARVALUE_S2
*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
*          转子旋向：                       FIELD_SYSTEM_TYPE //0:为逆时针,1为顺时针
*/
#define     SERVICE_SERVICE_OFF_GETFULLSPECSTRUM          21070

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
*/
#define     SERVICE_SERVICE_OFF_GETALARMLOGINFO         21071

/**往复式系统应力监测，得到指定机组指定时间、指定缸号的应力波形和振动测点波形
*输入参数：
*          公司ID        　　　    FIELD_SYSTEM_COMPANY
*		   分厂名          　　　  FIELD_SYSTEM_FACTORY
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
*		       特征值结构体		         FIELD_SERVICE_CHARVALUE_S
*		       特征值结构体2		       FIELD_SERVICE_CHARVALUE_S2
*		       测点名				           FIELD_SYSTEM_CHANN
*		       测点名2				         FIELD_SYSTEM_CHANN2
*		   		 测点1角度参数1		       FIELD_SERVICE_ANGLEPARAM1
*		       测点1角度参数2		       FIELD_SERVICE_ANGLEPARAM2
*		       测点1角度参数3		       FIELD_SERVICE_ANGLEPARAM3
*		       测点1角度参数4		       FIELD_SERVICE_ANGLEPARAM4
*		       测点2角度参数1		       FIELD_SERVICE_ANGLEPARAM5
*		       测点2角度参数2		       FIELD_SERVICE_ANGLEPARAM6
*		       测点2角度参数3		       FIELD_SERVICE_ANGLEPARAM7
*		       测点2角度参数4		       FIELD_SERVICE_ANGLEPARAM8
*/
#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE              21072

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
*          自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUFF
*          自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数               FIELD_SERVICE_CHANN_NUM
*          时间值                 FIELD_SERVICE_OFF_TIME
*/
#define     SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND         21073


/** 获取离线巡检数据：
*          公司ID        FIELD_SYSTEM_COMPANY
*          分厂名        FIELD_SYSTEM_FACTORY
*          机组ID        FIELD_SERVICE_PLANTID
*          测点类型：    FIELD_SERVICE_CHANNTYPE
*          测点buff      FIELD_SERVICE_CHANN_BUFF
*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
*          测点个数      FIELD_SERVICE_CHANN_NUM
*          起始时间      FIELD_SERVICE_OFF_TIMESTART
*          终止时间      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*          测点ID       FIELD_SERVICE_OFF_CHANN1
*          时间         FIELD_SERVICE_OFF_TIME
*          数据值       FIELD_SERVICE_OFF_DATA
*          转速         FIELD_SERVICE_OFF_REV
*/
#define     SERVICE_SERVICE_OFF_GET_OFFLINE_DATA         21074

/**根据报警ID列表监测站级历史报警统计数据
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
*/

#define     SERVICE_SERVICE_OFF_GETALARMDATA    21075



/**监测站级历史报警ID列表
*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
*		     分厂名           FIELD_SYSTEM_FACTORY
*	         设备名           FIELD_SERVICE_PLANTID
*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
			 

*返回值的字段码：
			 报警ID		      FIELD_SERVICE_ALARMID
			 报警ID个数       FIELD_SERVICE_ALARMID_NUM
			 报警ID对应的报警通道数		FIELD_SERVICE_ALARMID_CHANN_NUM


*/
#define     SERVICE_SERVICE_OFF_GETALARMIDS    21076




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
  */
#define     SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX         21081

/**得到往复式机械设备运行状态历史趋势(报警数据和常规数据同时返回)
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
*            其它振动特征值类型   FIELD_SERVICE_OTHER_CHARTYPE
*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
*返回值的字段码：
*            测点特征值Buff       FIELD_SERVICE_TREND_VALUE_BUFF
*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
*            时间值               FIELD_SERVICE_OFF_TIME
*            测点类型标识         FIELD_SYSTEM_TYPE //振动:GE_VIBCHANN, 动态压力：GE_DYNPRESSCHANN, 过程量:GE_ALLPROC
*/
#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX                   21082

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
#define    SERVICE_SERVICE_OFF_SAVESPECMARK    21084

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
*/
#define    SERVICE_SERVICE_OFF_GETSPECMARK    21085



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
#define     SERVICE_SERVICE_OFF_GETALARMIDSEX    21087

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名          FIELD_SYSTEM_FACTORY
*	         设备名          FIELD_SERVICE_PLANTID
*            起始时间：      FIELD_SERVICE_OFF_TIMESTART
*            终止时间：      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID          FIELD_SERVICE_ALARMID
*		     测点编号        FIELD_SERVICE_CHANN1
*		     测点位号        FIELD_SYSTEM_CHANN_ALIAS
*		     报警开始时间    FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间    FIELD_SERVICE_OFF_TIMEEND
*		     报警类型        FIELD_SERVICE_ALARM_TYPE
*/
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATEX         21088

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
*/
#define    SERVICE_SERVICE_OFF_GETNORMALWAVE    21089

/**监测站级历史报警统计
*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
*		     分厂名          FIELD_SYSTEM_FACTORY
*	         设备名          FIELD_SERVICE_PLANTID
*            起始时间：      FIELD_SERVICE_OFF_TIMESTART
*            终止时间：      FIELD_SERVICE_OFF_TIMEEND
*返回值的字段码：
*		     报警ID          FIELD_SERVICE_ALARMID
*		     测点编号        FIELD_SERVICE_CHANN1
*		     测点位号        FIELD_SYSTEM_CHANN_ALIAS
*		     报警开始时间    FIELD_SERVICE_OFF_TIMESTART
*		     报警结束时间    FIELD_SERVICE_OFF_TIMEEND
*		     报警类型        FIELD_SERVICE_ALARM_TYPE
*            报警级别        FIELD_SERVICE_ALARM_LEVEL
*/
#define     SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL         21090

/**得到异常信号波形
  *        起始索引  FIELD_SERVICE_STARTINDEX
  *返回的字段码：
  *        公司名          FIELD_SYSTEM_COMPANY
  *        分厂名          FIELD_SYSTEM_FACTORY
  *        设备名          FIELD_SERVICE_PLANTID
  *        测点名          FIELD_SYSTEM_CHANN
  *        波形            FIELD_SERVICE_OFF_WAVE
  *        波形点数        FIELD_SERVICE_OFF_WAVE_NUM            
  *        结束索引        FIELD_SERVICE_ENDINDEX
  */
#define    SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE         21095


/**
  根据设备测点设置轴心位置计算参数
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *          位置ID          FIELD_SYSTEM_AXISLOCATION
  *          旋转方向        FIELD_SERVICE_MEASURE_DIRECTION
  *          轴直径          FIELD_SYSTEM_PITCHDIAM
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  */
#define    SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM         21096

/***************************系统级功能*******************************/

/**通过公司名、分厂名、机组号得到可以做启停车的测点和其对应的额定转速
  *输入参数：
  *          公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *          机组号     FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *          测点号    FIELD_SYSTEM_CHANN
  *          额定转速  FIELD_SERVICE_REV
  */
#define    SERVICE_SYSTEM_GETPLANTSUDCHANN           25000

/**得到指定分厂下所有机组位号，通过循环依次得到所有机组位号
  *输入参数：公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *          装置名ID   FIELD_SYSTEM_SET
  *返回值的字段码：
  *          机组号     FIELD_SYSTEM_PLANT
  *          装置名ID     FIELD_SYSTEM_SET
  *          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
  *          设备类型   FIELD_SYSTEM_TYPE
  *          数据源       FIELD_SYSTEM_DATASOURCE
  */
#define     SERVICE_SYSTEM_GETPLANTOFSUBCORP          25001

/**得到指定公司名、分厂名、机组位号的所有测点名和对应的测点类型，通过循环依次得到
  *输入参数：
  *          公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *          机组号     FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *          测点号      FIELD_SYSTEM_CHANN
  *          测点类型    FIELD_SYSTEM_CHANN_TYPE
  *          传感器类型  FIELD_SYSTEM_SENSOR_TYPE
  */
#define     SERVICE_SYSTEM_GETALLCHANN                25002

/**得到指定公司名、分厂名、机组指定测点类型的所有测点名，通过循环依次得到
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名     FIELD_SYSTEM_FACTORY
  *             机组号     FIELD_SYSTEM_PLANT
  *             测点类型   FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *             设备号     FIELD_SERVICE_ON_SEND_PLANTID              
  *             测点名     FIELD_SYSTEM_CHANN
  *             测点号     FIELD_SERVICE_CHANN1
  *             测点位号   FIELD_SYSTEM_CHANN_ALIAS
  *             测点单位   FIELD_SERVICE_CHANN1_UNIT
  *             测点类型    FIELD_SYSTEM_CHANN_TYPE
  *             传感器类型 FIELD_SYSTEM_SENSOR_TYPE
  *             所属汽缸号 FIELD_SERVICE_CYLINDERNO
  *             信息类型   FIELD_SYSTEM_TYPE
  *             报警限类型        FIELD_SYSTEM_EVENT_TYPE
  *             报警上限          FIELD_SYSTEM_ALARM_LOW
  *             报警上上限        FIELD_SYSTEM_ALARM_HIGH
  *             报警下限          FIELD_SYSTEM_ALARM_LOW2
  *             报警下下限        FIELD_SYSTEM_ALARM_HIGH2
  *             自定义参数模板ID          FIELD_SYSTEM_TEMPID
  *             自定义报警参数模板ID      FIELD_SYSTEM_ALARM_TEMPID
  *             数据源                    FIELD_SYSTEM_DATASOURCE
  *             部位名称          FIELD_SERVICE_SEC_NAME
  *             部位索引          FIELD_SERVICE_SEC_IDX
  *             位置名称          FIELD_SERVICE_LOC_NAME
  *             位置索引          FIELD_SERVICE_LOC_IDX
  *             测点所属列表      FIELD_SERVICE_CHANN1_LIST
  *             转速类型          FIELD_SERVICE_REV
  *             是否存在键相      FIELD_SERVICE_KEY_IF
  *             传感器灵敏度      FIELD_SERVICE_SENSITIVITY
  *             传感器偏置量      FIELD_SERVICE_SENSITIVITY1
  *             间隙电压          FIELD_SERVICE_TREND_GAP
  *             位置号            FIELD_SERVICE_LOCATION_NO
  *             测点方向          FIELD_SERVICE_MEASURE_DIRECTION
  *             数据类型          FIELD_SERVICE_DATA_TYPE
  *             直流放大倍数      FIELD_SERVICE_DC_AMPLIFY
  *             交流放大倍数      FIELD_SERVICE_AC_AMPLIFY
  *
  */
#define     SERVICE_SYSTEM_GETSPECTYPECHANN           25003

/**通过用户名和密码得到用户级别、集团名、公司名，如果用户级别是集团级，就需要循环得到各个公司和厂
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          集团名   FIELD_SYSTEM_GROUP
  *          公司名   FIELD_SYSTEM_COMPANY
  *          分厂名   FIELD_SYSTEM_FACTORY
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define     SERVICE_SYSTEM_GETCOMPANYTOFUSER          25004


/**得到指定公司名、分厂名、机组位号的所有组轴心轨迹对应的测点对，通过循环依次得到，如果没有指定设备号，则是服务器所有轴心轨迹对
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
   *            轴心位置位置描述   FIELD_SYSTEM_AXISLOCATION
  *             垂直方向测点    FIELD_SYSTEM_CHANN_VER
  *             水平方向测点    FIELD_SYSTEM_CHANN_HOR
  *             轴心位置信息结构体 FIELD_SYSTEM_SETSVR_PLANTINFO
  */
#define     SERVICE_SYSTEM_GETALLAXISCHANN            25005


/**得到指定机组位号的所对应的最高分析线数对应的宏定义
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             机组号        FIELD_SYSTEM_PLANT
  *             最高分析线数  FIELD_SYSTEM_ANALYSISLINES
  *             按照采样信息计算的分析线数    FIELD_SERVICE_ZOOM_LINES
  */
#define     SERVICE_SYSTEM_GETPLANTANALYSISLINES      25006

/**得到指定公司、分厂、设备的设备详细信息
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             设备信息  FIELD_SYSTEM_SETSVR_PLANTINFO
  */
#define     SERVICE_SYSTEM_GETPLANTINFO      25007


/**得到指定公司的所有分厂名，通过循环依次得到所有分厂名。如果没有指定公司名，就得到所有分厂名
	*输入参数：
	*                公司名    FIELD_SYSTEM_COMPANY
	*返回值的字段码：
	*                公司名    FIELD_SYSTEM_COMPANY
  *                分厂名    FIELD_SYSTEM_FACTORY
	*                分厂别名  FIELD_SYSTEM_SUBCORP_ALIAS
	*/
#define     SERVICE_SYSTEM_GETALLFACTORY        25008

/**得到指定所有公司信息
	*返回值的字段码：
	*                公司名    FIELD_SYSTEM_COMPANY
  *                公司别名  FIELD_SYSTEM_COMPANY2
  *                集团名    FIELD_SYSTEM_GROUP
  *                国家名    FIELD_SYSTEM_COUNTRY
	*/
#define     SERVICE_SYSTEM_GETALLCOMPANY        25009


/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
  *输入参数：公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *返回值的字段码：
  *          公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *          机组号     FIELD_SYSTEM_PLANT
  *          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
  *          集团级IP   FIELD_SYSTEM_GROUP_MW_IP
  *          集团级Port FIELD_SYSTEM_GROUP_MW_PORT
  *          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
  *          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
  *          装置级IP   FIELD_SYSTEM_MW_IP
  *          装置级Port FIELD_SYSTEM_MW_PORT
  *          装置级中间件ID   FIELD_SYSTEM_SET
  *          设备类型   FIELD_SYSTEM_TYPE
  *          数据源     FIELD_SYSTEM_DATASOURCE
  *          显示名称   FIELD_SYSTEM_PLANT_DISPLAY
  *          ERP编码    FIELD_SYSTEM_PLANT_ERP_CODE
  */
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP        25010


/**通过用户名和密码,修改密码
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *          新密码   FIELD_SYSTEM_RESERVE_STRING
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *                 如果为-1，表示没有修改成功，为1表示修改成功
  */
#define     SERVICE_SYSTEM_MODIFYPWD                  25011

/**通过测点类型和传感器类型得到特征值描述
  *输入参数：
  *             设备类型    FIELD_SYSTEM_TYPE
  *             测点类型    FIELD_SYSTEM_CHANN_TYPE
  *             传感器类型  FIELD_SYSTEM_SENSOR_TYPE
  *返回值的字段码：
  *             顺序        FIELD_SYSTEM_STATUS
  *             对应描述    FIELD_SYSTEM_STATUS_DISCRIPTION
  *             特征值单位  FIELD_SERVICE_CHANN1_UNIT
  */
#define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012

/**通过用户名得到客户端页面的更新时间
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *返回值的字段码：
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  */
#define     SERVICE_SYSTEM_GET_REFRESHTIME            25013

/**得到指定机组指定测点类型的所有测点名和对应的报警上下限，通过循环依次得到
  *输入参数：
  *             公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *             测点类型  FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *             测点号    FIELD_SYSTEM_CHANN
  *             报警上限  FIELD_SYSTEM_ALARM_HIGH
  *             报警下限  FIELD_SYSTEM_ALARM_LOW
  */
#define     SERVICE_SYSTEM_GETALARMTHRESHOLD          25014

/**通过用户名和密码登陆，如果登陆成功。如果用户登录成功，则状态为1，并返回用户级别和刷新时间；否则为-1，并且不返回用户级别和刷新时间。
  *
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  */
#define     SERVICE_SYSTEM_LOGIN                      25015

/**修改用户级别。如果成功，则返回状态为1，否则为-1，并说明原因
  *注意：只有管理员才有权限修改用户级别，并且如果是分厂管理员只能修改所在分厂的用户级别。
  *input param：用户名         FIELD_SYSTEM_USEID
  *             密码           FIELD_SYSTEM_PASSWORD
  *             被修改的用户名 FIELD_SYSTEM_MODIFY_USER
  *             用户级别       FIELD_SYSTEM_USER_LEVEL
  *return field code：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_MODIFY_USERLEVEL            25016

/**@brief Add new user.如果成功，则返回状态为1，否则为-1，并说明原因。
  *注意：只有管理员才有权限添加用户，并且，如果是分厂管理员只能添加为所在分厂的用户。
  *input param:用户名         FIELD_SYSTEM_USEID
  *            密码           FIELD_SYSTEM_PASSWORD
  *            新用户名       FIELD_SYSTEM_MODIFY_USER
  *            用户级别       FIELD_SYSTEM_USER_LEVEL
  *            所在分厂       FIELD_SYSTEM_SUBCORP
  *return field code：
  *            状态     FIELD_SYSTEM_STATUS
  *            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_ADD_NEWUSER                 25017

/**设置指定机组和测点的是否进行报警检查，如果状态字段输入为1，则检查报警；为0，则不检查报警
  *返回值说明：状态如果为1，说明修改成功，否则可以从状态描述中得到错误原因
  *输入参数：   机组号    FIELD_SYSTEM_PLANT
  *             测点号    FIELD_SYSTEM_CHANN
  *             状态     FIELD_SYSTEM_STATUS
  *返回值的字段码：
  *            状态     FIELD_SYSTEM_STATUS
  *            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define     SERVICE_SYSTEM_SETCHECKALARMFLAG               25018

/**得到指定机组位号的图结构类型信息
  *输入参数：
  *             公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *					    机组号          FIELD_SYSTEM_PLANT
  *			        概貌图类型      FIELD_SYSTEM_PLANT_GRAPHTYPE
  *             概貌图BUFF      FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF
  *             概貌图BUFF尺寸  FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE
  */
#define     SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID        25019


/**修改监测中心中间件IP和Port，如果连接的中间件是监测中心的中间件，则指定分厂和装置名，就修改指定装置级中间件IP和端口
  *输入参数：
   *           公司ID        FIELD_SYSTEM_COMPANY
  *          所在分厂       FIELD_SYSTEM_FACTORY
  *          装置名ID        FIELD_SYSTEM_SET
  *          中间件IP    FIELD_SYSTEM_MW_IP
  *          中间件PORT  FIELD_SYSTEM_MW_PORT
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020

/**得到监测中心中间件IP和Port。如果连接监测中心的中间件，则指定分厂和装置名，就得到指定装置级中间件IP和端口
   *           公司ID        FIELD_SYSTEM_COMPANY
  *           所在分厂       FIELD_SYSTEM_FACTORY
  *           装置名ID        FIELD_SYSTEM_SET
  *返回值的字段码：
  *          中间件IP    FIELD_SYSTEM_MW_IP
  *          中间件PORT  FIELD_SYSTEM_MW_PORT
  */
#define     SERVICE_SYSTEM_GET_MWINFO                      25021

/**修改发送到监测中心的时间间隔和是否发送状态,如果是没有设置分厂和装置号，也可以
  *           公司ID             FIELD_SYSTEM_COMPANY
  *           所在分厂           FIELD_SYSTEM_FACTORY
  *           装置名ID           FIELD_SYSTEM_SET
  *          发送时间间隔        FIELD_SYSTEM_SENDC_INTERVAL
  *          是否发送状态        FIELD_SYSTEM_SENDC_IF
  *          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
  *          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022

/**得到发送到监测中心的时间间隔和是否发送,如果是没有设置分厂和装置号，也可以
  *           公司ID        FIELD_SYSTEM_COMPANY
  *           所在分厂       FIELD_SYSTEM_FACTORY
  *           装置名ID        FIELD_SYSTEM_SET
  *返回值的字段码：
  *          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
  *          是否发送状态    FIELD_SYSTEM_SENDC_IF
  *          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
  *          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
  */
#define     SERVICE_SYSTEM_GET_SENDINFO                    25023

/**得到装置级中间件信息
  *返回的字段码：
  *         公司ID        FIELD_SYSTEM_COMPANY
  *         分厂名字      FIELD_SYSTEM_FACTORY
  *         装置名ID      FIELD_SYSTEM_SET
  *         信息结构体    FIELD_SYSTEM_SETSVR_SETINFO
  *         结构体长度    FIELD_SERVICE_LEN
  */
#define    SERVICE_SYSTEM_GET_SETSVRINFO                25024

/**得到装置级中间件服务器的设备详细信息
  *        设备起始索引  FIELD_SERVICE_STARTINDEX
  *返回的字段码：
  *        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        版本          FIELD_SYSTEM_VERSION
  *        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
  *        显示集团      FIELD_SERVICE_GROUP_VISIBLE
  *        显示公司      FIELD_SERVICE_COMPANY_VISIBLE
  *        显示分厂      FIELD_SERVICE_FACTORY_VISIBLE
  *        显示装置      FIELD_SERVICE_SET_VISIBLE
  *        显示设备      FIELD_SERVICE_PLANT_VISIBLE
  *        集团名称      FIELD_SYSTEM_GROUP
  *        国家名称      FIELD_SYSTEM_COUNTRY
  *        设备索引      FIELD_SERVICE_ENDINDEX
  *        设备数目      FIELD_SERVICE_PLANT_NUM
  *        设备显示名称  FIELD_SYSTEM_PLANT_DISPLAY
  *        ERP编码       FIELD_SYSTEM_PLANT_ERP_CODE
  *        设备标识      FIELD_SERVICE_DEVICE_ID
  */
#define    SERVICE_SYSTEM_GET_SETSVRPLANTINFO           25025

/**得到装置级中间件服务器指定设备的测点详细信息
  * 输入字段码：
  *        公司ID        FIELD_SYSTEM_COMPANY
  *	       分厂名        FIELD_SYSTEM_FACTORY
  *        设备号：      FIELD_SYSTEM_PLANT
  *返回的字段码：
  *        测点信息      FIELD_SYSTEM_SETSVR_CHANNINFO
  *        测点信息长度  FIELD_SERVICE_LEN
  */
#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN      25026

/**得到所有用户信息
  *输入字段码：
  *返回字段码：
  *            用户名         FIELD_SYSTEM_USEID
  *            密码           FIELD_SYSTEM_PASSWORD
  *            用户级别       FIELD_SYSTEM_USER_LEVEL
  *            所在分厂       FIELD_SYSTEM_SUBCORP
  *            装置名ID       FIELD_SYSTEM_SET
  *            是否浏览装置   FIELD_SYSTEM_BROWSE_SET
  *            刷新时间 FIELD_SYSTEM_REFRESHTIME
  */
#define    SERVICE_SYSTEM_GET_ALLUSERINFO               25027

/**得到其子中间件服务器IP,端口和属性信息.
  中间件服务器的级别：
  GE_SRV_TYPE_SET,          ///装置级中间件服务器
  GE_SRV_TYPE_COMPANY,      ///公司级中间件服务器
  GE_SRV_TYPE_GROUP         ///集团级中间件服务器
  *输入字段:
  *        公司ID        FIELD_SYSTEM_COMPANY
  *返回字段：
  *        中间件ID         FIELD_SYSTEM_SET
  *        中间件IP         FIELD_SYSTEM_MW_IP
  *        中间件Port       FIELD_SYSTEM_MW_PORT
  *        中间件级别       FIELD_SYSTEM_USER_LEVEL
  */
#define  SERVICE_SYSTEM_GET_MW_INFO                   25028

/** 获取各个程序的路径和版本号
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            程序名                 FIELD_SERVICE_MODULE_NAME
  *            文件名                 FIELD_SERVICE_FILE_NAME
  *            路径                   FIELD_SERVICE_FILE_PATH
  *            版本号                 FIELD_SERVICE_FILE_VERSION
  *\instance 例:
      [JSON]
      "MiddleWare" : {
         "POSITION" : "E:\\Project\\lib\\S_SystemSvr.dll",
         "POSITION1" : "E:\\Project\\lib\\S_OnlineSvr.dll",
         "POSITION2" : "E:\\Project\\lib\\S_OfflineSvr.dll",
         "S_OfflineSvr.dll" : "3.5.0",
         "S_OnlineSvr.dll" : "3.5.0",
         "S_SystemSvr.dll" : "3.5.0"
      },

      [中间件通信数据格式]
      "MiddleWare"
      "S_OfflineSvr.dll"
      "E:\\Project\\lib\\S_OfflineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_OnlineSvr.dll"
      "E:\\Project\\lib\\S_OnlineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_SystemSvr.dll"
      "E:\\Project\\lib\\S_SystemSvr.dll"
      "3.5.0"
  */
#define     SERVICE_SYSTEM_GET_MODULE_VERSION                 25029

/** 获取各个数据库状态
  *输入参数字段码：无
  *返回值的字段码：(n)
  *     报警数据预计保存时间,单位 天(AlarmDataSavingTime)     FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME
  *     趋势数据预计保存时间(DataSavingTime)                  FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME
  *     报警和趋势数据预计保存时间(TrendDataSavingTime)       FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME
  *     数据库数目(DBNum)                                     FIELD_SERVICE_DB_NUM
  *     数据库文件编号,即0, 主数据库文件， 1分数据文件1, ...  FIELD_SERVICE_DB_INDEX
  *     数据文件名(DBDataFileName)                            FIELD_SERVICE_DB_DATA_FILE_NAME
  *     数据文件大小，单位 GB(DBDataFileSize)                 FIELD_SERVICE_DB_DATA_FILE_SIZE
  *     日志文件名(DBLogFileName)                             FIELD_SERVICE_DB_LOG_FILE_NAME
  *     日志文件大小，单位 GB(DBLogFileSize)                  FIELD_SERVICE_DB_LOG_FILE_SIZE
  *     数据库名(DBName)                                      FIELD_SERVICE_DB_NAME
  *     数据库类型，0表示主库，1表示分库(DBType)              FIELD_SERVICE_DB_TYPE
  *     数据库所在磁盘剩余空间，单位为GB(LeftDiskSpace)       FIELD_SERVICE_FREE_DISK_SPACE
  *     数据库所在磁盘剩余空间比(LeftDiskSpaceRate)           FIELD_SERVICE_FREE_DISK_SPACE_RATIO
  *     前一天增长最快的表，CSV格式(TheFastIncTableName)      FIELD_SERVICE_FAST_INC_TABLE
  *     数据库所在磁盘总空间，单位为GB(TotalDiskSpace)        FIELD_SERVICE_TOTAL_DISK_SPACE
  */
#define     SERVICE_SYSTEM_GET_DB_STATUS                      25030

/** 获取中间件运行状态
  *输入参数字段码：无
  *返回值的字段码：(1)
  *            数据库状态             FIELD_SERVICE_DB_STATUS
  *            接收数据的公司名       FIELD_SYSTEM_COMPANY
  *            接收数据的分厂名       FIELD_SYSTEM_FACTORY
  *            接收数据的设备数目     FIELD_SERVICE_DATA_RCV_PLANT_NUM
  *            接收数据的设备状态     FIELD_SERVICE_DATA_RCV_STATUS
  *            保存数据的设备数目     FIELD_SERVICE_DATA_SAVE_PLANT_NUM
  *            保存数据的设备状态     FIELD_SERVICE_DATA_SAVE_STATUS
  *            同步数据的设备数目     FIELD_SERVICE_DATA_SYNC_PLANT_NUM
  *            同步数据的设备状态     FIELD_SERVICE_DATA_SYNC_STATUS
  *
  *\description 类型说明：
  *            数据库状态,     类型 GE_DB_STATUS
  *            接收数据的设备状态，类型 BUFFER, BHMWDataRcvStatus[]
  *            保存数据的设备状态，类型 BUFFER, BHMWDataSaveStatus[]
  *            同步数据的设备状态，类型 BUFFER, BHMWDataSyncStatus[]
  */
#define     SERVICE_SYSTEM_GET_SERVER_STATUS                  25031


/** 获取数采硬件状态
  *输入参数值：无
  *返回值的字段码：(n)
  *            数采卡索引             FIELD_SERVICE_DAQ_CARD_INDEX
  *            数采卡数量             FIELD_SERVICE_DAQ_CARD_NUM
  *            数采卡逻辑名称         FIELD_SERVICE_DAQ_CARD_NAME
  *            使用AI通道(CSV)        FIELD_SERVICE_DAQ_USED_AI_CHANNEL
  *            使用CI通道(CSV)        FIELD_SERVICE_DAQ_USED_CI_CHANNEL
  *            工作状况(字符串描述)   FIELD_SERVICE_DAQ_CARD_STATUS
  *            获取数采硬件状态时间   FIELD_SERVICE_OFF_TIME
  */
  #define     SERVICE_SYSTEM_GET_DAQ_HW_STATUS                  25032

/** 获取数采软件状态
  *输入参数值：无
  *返回值的字段码：(1)
  *            版本号                                   FIELD_SERVICE_DAQ_SW_VERSION
  *            采集模式                                 FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            启动时间                                 FIELD_SERVICE_TIME
  *            最近一次振动数据发送时间                 FIELD_SERVICE_DAQ_VIB_TIME
  *            最近一次动态数据发送时间                 FIELD_SERVICE_DAQ_DYN_TIME
  *            最近一次过程量数据发送时间               FIELD_SERVICE_DAQ_PROC_TIME
  *            接收间隙电压状态                         FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            最近一次断网数据发送开始时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            最近一次断网数据发送结束时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  *            获取数采软件状态时间                     FIELD_SERVICE_OFF_TIME
  */
#define     SERVICE_SYSTEM_GET_DAQ_SW_STATUS                  25033


/** 获取基本的组态息
  * 输入参数值：无
  * 返回值的字段码：(1)
  *          报警设置(秒数，0表示未设置)                FIELD_SERVICE_CONFIG_ALARM
  *          连接的数据库                               FIELD_SERVICE_CONFIG_CONNECTED_DB
  *          保存数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SAVING
  *          同步数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SYNC
  *          中间件服务器型(0表示装置级，1表示中心级)   FIELD_SERVICE_CONFIG_SERVER_TYPE
  *          统计信息公司数目                           FIELD_SERVICE_COMPANY_NUM
  *          统计信息设备数目                           FIELD_SERVICE_PLANT_NUM
  *          统计信息测点数目                           FIELD_SERVICE_CHANN_NUM
*/
#define     SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO              25034


/**验证是否中间件是否连通。如果连通则返回值为1。
  *输入字段码：
  *返回字段码：
  *          状态     FIELD_SYSTEM_STATUS
  */
#define    SERVICE_SYSTEM_CONN_STATUS                   25100


/**得到指定机组的所有测点信息。
  *input param：机组名 FIELD_SYSTEM_PLANT
  *return field code：
  *             测点别名 FIELD_SYSTEM_CHANN_ALIAS
  *             测点名   FIELD_SYSTEM_CHANN
  *             测点类型 FIELD_SYSTEM_CHANN_TYPE
  *             报警上限 FIELD_SYSTEM_ALARM_HIGH
  *             报警下限 FIELD_SYSTEM_ALARM_LOW
  *             状态     FIELD_SYSTEM_STATUS
  *             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_GET_CHANNINFO                25108

/**得到指定分厂的所有机组信息。
  *input param: 分厂名  FIELD_SYSTEM_SUBCORP
  *return field code：
  *          机组名   FIELD_SYSTEM_PLANT
  *          采样点数 FIELD_SYSTEM_SAMPLE_NUM
  *          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
  *          数据采集机组IP  FIELD_SYSTEM_DAQIP
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_GET_PLANTINFO                25109

/**得到指定机组的指定测点名信息
  *input param：机组名 FIELD_SYSTEM_PLANT
  *             测点名 FIELD_SYSTEM_CHANN
  *return field code：
  *             测点别名 FIELD_SYSTEM_CHANN_ALIAS
  *             测点类型 FIELD_SYSTEM_CHANN_TYPE
  *             报警上限 FIELD_SYSTEM_ALARM_HIGH
  *             报警下限 FIELD_SYSTEM_ALARM_LOW
  *             状态     FIELD_SYSTEM_STATUS
  *             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_GET_SPEC_CHANNINFO           25110

/**得到指定分厂的所有机组信息。
  *input param: 分厂名  FIELD_SYSTEM_SUBCORP
  *             机组名   FIELD_SYSTEM_PLANT
  *return field code：
  *          采样点数 FIELD_SYSTEM_SAMPLE_NUM
  *          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
  *          数据采集机组IP  FIELD_SYSTEM_DAQIP
  *          转速     FIELD_SYSTEM_PLANT_REV
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_GET_SPECPLANTINFO            25111

/**得到监测中心的设备信息表
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SYSTEM_PLANT
*             测点号     FIELD_SYSTEM_CHANN
*返回的字段码：
*             索引       FIELD_SYSTEM_STATUS
*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*             单位       FIELD_SERVICE_CHANN1_UNIT
  */
#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112

/**得到监测中心的信息描述表
* 输入字段码：
*返回的字段码：
*        信息类型      FIELD_SYSTEM_TYPE
*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
*/
#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL     25113

/**根据测点信息类型,得到监测中心的信息描述表
* 输入字段码：
*        信息类型   FIELD_SYSTEM_TYPE
*返回的字段码：
*        索引       FIELD_SYSTEM_STATUS
*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*        单位       FIELD_SERVICE_CHANN1_UNIT
*/
#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114

/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
*输入参数：   公司名         FIELD_SYSTEM_COMPANY
*             分厂名         FIELD_SYSTEM_FACTORY
*             机组名         FIELD_SYSTEM_PLANT
*             测点名         FIELD_SYSTEM_CHANN
*             是否常规报警   FIELD_SYSTEM_STATUS //1为判断报警，0为不判断报警
*             报警限类型     FIELD_SYSTEM_EVENT_TYPE
*             报警上限       FIELD_SYSTEM_ALARM_LOW
*             报警上上限     FIELD_SYSTEM_ALARM_HIGH
*             报警下限       FIELD_SYSTEM_ALARM_LOW2
*             报警下下限     FIELD_SYSTEM_ALARM_HIGH2
*返回值的字段码：
*             返回状态       FIELD_SYSTEM_STATUS
*/
#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115

/**得到监测中心的自定义参数模板表
* 输入字段码：
*返回的字段码：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID       FIELD_SYSTEM_TEMPID
*         自定义信息描述         FIELD_SYSTEM_STATUS_DISCRIPTION
*/
#define    SERVICE_SYSTEM_GET_SETSVRTEMPLET      25116

/**得到监测中心的自定义参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
*         预处理类型             FIELD_SYSTEM_TYPE
*         包络频带起始值         FIELD_SYSTEM_BAND_STARTVALUE
*         包络频带结束值         FIELD_SYSTEM_BAND_ENDVALUE
*         预处理参数K             FIELD_SERVICE_PREPROC_K
*         预处理参数B             FIELD_SERVICE_PREPROC_B
*         单位                   FIELD_SERVICE_UNIT
*/
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM      25117

/**得到监测中心的自定义参数报警参数模板表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID         FIELD_SYSTEM_TEMPID
*         自定义报警模板ID         FIELD_SYSTEM_ALARM_TEMPID
*         自定义报警描述           FIELD_SERVICE_ALARM_DISCRIPTION
*/
#define    SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET      25118

/**得到监测中心的自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
*/
#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119

/**得到监测中心的通道自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         测点号                 FIELD_SYSTEM_CHANN
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
*/
#define    SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM     25120

/**得到指定公司、分厂和装置ID的自定义参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备名                FIELD_SERVICE_PLANTID
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         索引ID                 FIELD_SYSTEM_STATUS
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
*/
#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121

/**得到指定公司、分厂和装置ID的自定义参数报警参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
*/
#define    SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM       25122

/**得到监测中心配置的自定义轴承参数表
* 输入字段码：
*返回的字段码：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         轴承索引ID             FIELD_SYSTEM_STATUS
*         轴承名称               FIELD_SYSTEM_SEG_NAME
*         轴承类型               FIELD_SYSTEM_TYPE
*         滚珠数目               FIELD_SYSTEM_SEG_NUM
*         滚珠直径               FIELD_SYSTEM_START_VALUE
*         节径                   FIELD_SYSTEM_END_VALUE
*         压力角                 FIELD_SERVICE_ANGLEPARAM1
*         外壳直径               FIELD_SYSTEM_BAND_STARTVALUE
*         内径                   FIELD_SYSTEM_BAND_ENDVALUE
*         滚珠排数               FIELD_SYSTEM_AXISCHANN_NUM
*         轴承代码               FIELD_SYSTEM_CHAR_TYPE
*         同步状态标识           FIELD_SYSTEM_SENDC_IF
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*/
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123

/**得到监测中心的齿轮箱参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         齿轮箱索引ID           FIELD_SYSTEM_STATUS
*         齿轮箱类别             FIELD_SYSTEM_TYPE
*         齿轮箱名称             FIELD_SYSTEM_SEG_NAME
*         行星齿轮驱动方式       FIELD_SYSTEM_CHAR_TYPE
*         太阳轮齿数             FIELD_SERVICE_PLANT_NUM
*         行星轮个数             FIELD_SERVICE_ALARM_PLANTNUM
*         行星轮齿数             FIELD_SERVICE_RUN_PLANTNUM
*         银河系齿数             FIELD_SERVICE_STOP_PLANTNUM
*         齿轮级数               FIELD_SYSTEM_USER_LEVEL
*         齿数Z12                FIELD_SYSTEM_GEARNUMZ12
*         齿数Z21                FIELD_SYSTEM_GEARNUMZ21
*         齿数Z22                FIELD_SYSTEM_GEARNUMZ22
*         齿数Z31                FIELD_SYSTEM_GEARNUMZ31
*         齿数Z32                FIELD_SYSTEM_GEARNUMZ32
*         齿数Z41                FIELD_SYSTEM_GEARNUMZ41
*         齿数Z42                FIELD_SYSTEM_GEARNUMZ42
*         齿数Z51                FIELD_SYSTEM_GEARNUMZ51
*         齿数Z52                FIELD_SYSTEM_GEARNUMZ52
*         齿数Z61                FIELD_SYSTEM_GEARNUMZ61
*         同步状态标识           FIELD_SYSTEM_SENDC_IF
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*/
#define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124

/**得到监测中心的位置参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         位置索引ID             FIELD_SYSTEM_STATUS
*         位置名称               FIELD_SYSTEM_SEG_NAME
*         部位索引ID             FIELD_SYSTEM_CHANN_TYPE
*         轴承索引ID             FIELD_SYSTEM_TYPE
*         轴承表类型             FIELD_SYSTEM_CHAR_TYPE
*         轴号                   FIELD_SYSTEM_SEG_NUM
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         位置号                 FIELD_SERVICE_LOCATION_NO
*/
#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125

/**得到监测中心的部位参数表
*输入参数值：
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         部位索引ID             FIELD_SYSTEM_STATUS
*         部位名称               FIELD_SYSTEM_SEG_NAME
*         齿轮箱索引ID           FIELD_SYSTEM_TYPE
*         备注                   FIELD_SYSTEM_STATUS_DISCRIPTION
*         功能类型               FIELD_SERVICE_FUNCTION_TYPE
*         部位类型               FIELD_SERVICE_SECTION_TYPE
*         当前位置               FIELD_SERVICE_LOC_IDX
*         前一部位               FIELD_SERVICE_PREV_SEC_IDX
*         前一位置               FIELD_SERVICE_PREV_LOC_IDX
*         部位图片               FIELD_SERVICE_PREV_SECTION_PIC
*/
#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126

/**得到中间件的版本号
*输入参数值：
*返回参数值：
*         版本号                 FIELD_SERVICE_FILE_VERSION
		  中间件当前时间         FIELD_SERVICE_TIME
*/

#define   SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION     25127


/***************************专家系统功能部分*******************************/

/**得到指定报警ID时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             报警ID     FIELD_SERVICE_ALARMID
*             测点ID     FIELD_SERVICE_CHANN1
*             时间       FIELD_SERVICE_OFF_TIME
*返回的字段码：
*             报警通道   FIELD_SERVICE_OFF_CHANN1
*             工作转速   FIELD_SERVICE_OFF_REV
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
*/
#define    SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT      30001

/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             测点ID     FIELD_SERVICE_CHANN1
*             起始时间   FIELD_SERVICE_OFF_TIMESTART
*             终止时间   FIELD_SERVICE_OFF_TIMEEND
*返回的字段码：
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
*/
#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT      30002


/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             测点ID     FIELD_SERVICE_CHANN1
*             起始时间   FIELD_SERVICE_OFF_TIMESTART
*             终止时间   FIELD_SERVICE_OFF_TIMEEND
*返回的字段码：
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
*/
#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT_RC      30003

/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*返回的字段码：
*             事实名     FIELD_SERVICE_REMARK     //对应为字符串
*/
#define    SERVICE_EXPERTSYS_GET_PLANTFACTNAME      30004

/** 一次得到全部的设备信息
*输入参数： 起始索引     FIELD_SERVICE_STARTINDEX
*返回值的字段码：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
*          集团级IP   FIELD_SYSTEM_GROUP_MW_IP
*          集团级Port FIELD_SYSTEM_GROUP_MW_PORT
*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
*          装置级IP   FIELD_SYSTEM_MW_IP
*          装置级Port FIELD_SYSTEM_MW_PORT
*          装置级中间件ID   FIELD_SYSTEM_SET
*          设备类型   FIELD_SYSTEM_TYPE
*          数据源     FIELD_SYSTEM_DATASOURCE
*          显示名称   FIELD_SYSTEM_PLANT_DISPLAY
*          ERP编码    FIELD_SYSTEM_PLANT_ERP_CODE
*          结束的索引 FIELD_SERVICE_ENDINDEX
*/
#define    SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX      25210 


/**返回动平衡类型
*  输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
				 记录时间开始   FIELD_SERVICE_OFF_TIME
				 记录时间结束   FIELD_SERVICE_OFF_TIMEEND
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*   返回值的字段码：
*               记录时间       FIELD_SERVICE_OFF_TIME
*               测点名         FIELD_SYSTEM_CHANN
*				测点名2        FIELD_SYSTEM_CHANN2
*               BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
*				BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
*				BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
*				BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节                     
*/
#define     SERVICE_SERVICE_OFF_GET_BALANCE_DATA 24063


/**接收动平衡类型
*输入参数字段码：
*                公司名         FIELD_SYSTEM_COMPANY
*                分厂名         FIELD_SYSTEM_FACTORY
*                设备号         FIELD_SERVICE_PLANTID  
*                记录时间       FIELD_SERVICE_OFF_TIME
*                测点名         FIELD_SYSTEM_CHANN
				 测点名2        FIELD_SYSTEM_CHANN2
*                BUFFER1        FIELD_SERVICE_OFF_DATA_CHANN_1 
				 BUFFER2        FIELD_SERVICE_OFF_DATA_CHANN_2 
				 BUFFER1长度    FIELD_SERVICE_OFF_DATA_CHANN_1_LEN   字节
				 BUFFER2长度    FIELD_SERVICE_OFF_DATA_CHANN_2_LEN   字节
				 BALANCE_TYPE   FIELD_SERVICE_OFF_BALANCE_TYPE       0单面 1双面
*返回值的字段码：
*                同步状态      FIELD_SYSTEM_STATUS               1,保存成功 0，保存失败                 
*/
#define     SERVICE_SERVICE_OFF_SET_BALANCE_DATA 24062    



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
								
*/
#define     SERVICE_SERVICE_TEMP_SAMPLE_PLANTS 24070 


/**更新用户信息。
*input param 用户名		 FIELD_SYSTEM_USEID
			 密  码		 FIELD_SYSTEM_PASSWORD
			 公  司		 FIELD_SYSTEM_COMPANY
			 分  厂		 FIELD_SYSTEM_FACTORY
			 集  团		 FIELD_SYSTEM_GROUP
					 
*return field code：
			 返回值		 FIELD_SYSTEM_STATUS
*/

#define    SERVICE_SYSTEM_SET_USERINFO         24071


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
  */
#define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX    21077    


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
  */
#define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME     21078 

/*!
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *	     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID：        FIELD_SERVICE_PLANTID
  *          启停车事件ID    FIELD_SYSTEM_EVENT_ID
  *          新启停车事件名  FIELD_SYSTEM_EVENT_NAME
  *返回值的字段码：
  *          是否修改成功：  FIELD_SYSTEM_STATUS
  *  
  */

#define    SERVICE_SERVICE_OFF_RENAMEEVENT 21079


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
  */
#define    SERVICE_SERVICE_OFF_SAVESPECTIMEDATA    21083

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
  */
#define    SERVICE_SERVICE_OFF_SET_DISK_FULL    21086


/**
  根据设备得到用户关联信息。
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID：        FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *          用户名          FIELD_SYSTEM_USEID
  *          密码            FIELD_SYSTEM_PASSWORD
  */

/**通过用户名和密码得到用户对应的设备
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          集团名   FIELD_SYSTEM_GROUP
  *          公司名   FIELD_SYSTEM_COMPANY
  *          分厂名   FIELD_SYSTEM_FACTORY
  *          设备名   FIELD_SERVICE_PLANTID
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_GET_PLANT_OF_USER           25040
           

/**
  根据设备得到用户关联信息。
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR (默认: ,)
  *返回值的字段码：
  *          用户名列表      FIELD_SYSTEM_USEID
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR
  *
  */

#define    SERVICE_SYSTEM_GET_USER_OF_PLANT         25041


/** 用户管理接口
  *输入参数：
  *         操作类型         FIELD_SYSTEM_TYPE  (0: 新建用户 [+修改设备], 1: 删除用户, 2: 更改密码, 3: 添加设备, 4: 移除设备, 5: 修改设备)
  *          用户名          FIELD_SYSTEM_USEID
  *          用户级别        FIELD_SYSTEM_USER_LEVEL
  *          密码            FIELD_SYSTEM_PASSWORD
  *          设备列表        FIELD_SYSTEM_PLANT   1,2,3
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR (默认: ,)
  *返回值的字段码：
  *          状态            FIELD_SYSTEM_STATUS  (<0 失败)
  *          状态描述        FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define    SERVICE_SYSTEM_ACTION_USER_PLANT           25042


/**发送机组运行状态数据到中间件模块。即各个测点数据的报警状态
  *输入参数字段码：
  *            版本             FIELD_SYSTEM_VERSION
  *            设备数           FIELD_SERVICE_PLANT_NUM
  if FIELD_SYSTEM_VERSION == VERSION_PLANT_STATUS_IN_BLOB：
      *        长度             FIELD_SERVICE_LEN
      *        状态             FIELD_SYSTEM_STATUS
  if FIELD_SYSTEM_VERSION == VERSION_PLANT_STATUS_IN_FIELDS:
      *        公司ID           FIELD_SYSTEM_COMPANY
      *        分厂名           FIELD_SYSTEM_FACTORY
      *        机组ID           FIELD_SERVICE_PLANTID
      *        轴心位置状态     FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
      *        轴心位置个数     FIELD_SERVICE_ON_SEND_AXISCHANNNUM
      *        过程量状态       FIELD_SERVICE_ON_SEND_ALLPROCSTATE
      *        过程量个数       FIELD_SERVICE_ON_SEND_PROCCHANNNUM
      *        振动测点状态     FIELD_SERVICE_ON_SEND_ALLVIBSTATE
      *        振动测点个数     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
      *        动态测点状态     FIELD_SERVICE_ON_SEND_ALLDYNSTATE
      *        动态测点个数     FIELD_SERVICE_ON_SEND_DYNCHANNNUM
      *        报警EVENTID      FIELD_SERVICE_ALARMID
  *返回值的字段码：
  *            状态代码 FIELD_SYSTEM_STATUS
  *            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  */
#define     SERVICE_SERVICE_ON_SET_PLANT_STATUS       24041


/**发送指定机组的报警状态，为报警状态的结构体
  *输入参数字段码：
  *            公司ID   FIELD_SYSTEM_COMPANY  (可选)
  *            分厂名   FIELD_SYSTEM_FACTORY  (可选)
  *            机组ID   FIELD_SERVICE_PLANTID (可选)
  *返回值的字段码：
  *            轴心位置状态      FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE
  *            轴心位置个数      FIELD_SERVICE_ON_SEND_AXISCHANNNUM
  *            过程量状态        FIELD_SERVICE_ON_SEND_ALLPROCSTATE
  *            过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *            振动测点状态      FIELD_SERVICE_ON_SEND_ALLVIBSTATE
  *            振动测点个数      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            动态测点状态      FIELD_SERVICE_ON_SEND_ALLDYNSTATE
  *            动态测点个数      FIELD_SERVICE_ON_SEND_DYNCHANNNUM
  */
#define     SERVICE_SERVICE_ON_GET_PLANT_STATUS           24042



   /**得到指定指定机组由过程量判断的机组开停车状态
   *输入参数：
   *          公司ID           FIELD_SYSTEM_COMPANY
   *		  分厂名           FIELD_SYSTEM_FACTORY
   *          机组ID           FIELD_SERVICE_PLANTID
   *返回值的字段码：
   *          机组状态：           FIELD_SYSTEM_STATUS
   #define    SERVICE_SYSTEM_QUERY_PROC_RUNSTATUS           20043
   */
#define    SERVICE_SYSTEM_QUERY_PROC_RUNSTATUS           20043

/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
*输入参数：   公司名         FIELD_SYSTEM_COMPANY
*             分厂名         FIELD_SYSTEM_FACTORY
*             机组名         FIELD_SYSTEM_PLANT
*             测点名         FIELD_SYSTEM_CHANN
*             测点类型：     FIELD_SERVICE_CHANNTYPE
*             是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
*             报警限类型     FIELD_SYSTEM_ALARM_TYPE //低通，高通，带通，带阻
*             报警上限           FIELD_SYSTEM_ALARM_HIGH
*             报警上上限         FIELD_SYSTEM_ALARM_HIGH2
*             报警下限           FIELD_SYSTEM_ALARM_LOW
*             报警下下限         FIELD_SYSTEM_ALARM_LOW2
*             修改报警限主机名   FIELD_SYSTEM_HOST_NAME
*             修改报警限主机IP   FIELD_SYSTEM_CLIENT_IP
*             修改报警限用户名   FIELD_SYSTEM_USER_NAME
*返回值的字段码：
*             返回状态       FIELD_SYSTEM_STATUS*/
#define    SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION 20044


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
*         报警上限       FIELD_SYSTEM_ALARM_HIGH
*         报警上上限     FIELD_SYSTEM_ALARM_HIGH2
*         报警下限       FIELD_SYSTEM_ALARM_LOW
*         报警下下限     FIELD_SYSTEM_ALARM_LOW2
*         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
*/
#define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045

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
#define   SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS 20046


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
*/
#define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047


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
*/
#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION 20048

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
*/
#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO 20049


/**中间件中获取报警文件信息
*输入参数：  
*         文件路径                   FIELD_SERVICE_FILE_PATH
*         文件名                     FIELD_SERVICE_FILE_NAME
*         
*返回值的字段码：
*         文件名                     FIELD_SERVICE_FILE_NAME
*         文件内容                   FIELD_SERVICE_FILE_CONTENT
*         
*/
#define   SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT 20050

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
  */
#define    SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD         20051

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
  */
#define    SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD         20052

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
  *          位置ID          FIELD_SYSTEM_AXISLOCATION
  *          旋转方向        FIELD_SERVICE_MEASURE_DIRECTION
  *          轴直径          FIELD_SYSTEM_PITCHDIAM
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  */
#define    SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM         20053

/**
  根据设备测点设置轴心位置配置信息
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *          位置ID          FIELD_SYSTEM_AXISLOCATION
  *          旋转方向        FIELD_SERVICE_MEASURE_DIRECTION
  *          轴直径          FIELD_SYSTEM_PITCHDIAM
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  */
#define    SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM         20054


/////////////
#endif

