/**字段码
  *
  */
#ifndef   __FIXDEF_FIELD_DEFINE__
#define	  __FIXDEF_FIELD_DEFINE__

///－－－－－－－－－字段宏－－－－－－－－－－///

#define FIELD_SYSTEM_SUCCESS   1
#define FIELD_SYSTEM_FAILURE   0
#define FIELD_SYSTEM_MAX_TRANSFER_DATA_LIMIT_IN_BYTE 92160 //最大传输上限，单位:90*1024字节
#define FIELD_SYSTEM_MAX_TRANSFER_DATA_LIMIT_IN_NUM 300 //最大传输上限，单位:个数


///－－－－－－－－－字段码－－－－－－－－－－///

/**通频特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_OVERALLCHAR          10000

/**1倍频特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_ONECHAR              10001

/**2倍频特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_TWOCHAR              10002

/**3倍频特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_THREECHAR            10003

/**1/2倍频特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_HALFCHAR             10004

/**过程量特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_PROCESS              10005


/**单个特征值
*类型：float
*/
#define     FIELD_SERVICE_TREND_VALUE                10006

/**轴位移特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_AXISDISPLACE         10007

/**特征值类型的枚举,
  *类型：整数.
  *值范围：通频特征值为0，1倍频特征值为1，2倍频特征值为2，3倍频特征值为3，1/2倍频特征值为4
  */
#define     FIELD_SERVICE_TREND_CHARTYPE             10008

/**日期时间
  *类型:字符串
  */
#define     FIELD_SERVICE_TIME                       10009

/*!测点类型,是所有过程量测点，还是振动测点，或者是某一过程量测点
  *类型：int
  *值范围：0-5
  *       GE_ALLPROC:所有过程量测点;
  *	      GE_VIBCHANN:振动测点;
  *       GE_AXISCHANN:轴位移测点;
  *       GE_PRESSCHANN:压力测点;
  *       GE_TEMPCHANN:温度测点;
  *       GE_FLUXCHANN:流量测点;
  *       GE_ONTHERCHANN:其它.
  */
#define    FIELD_SERVICE_CHANNTYPE                   10010

/**读取波形数据的频谱数据个数
  *类型：int
  */
#define     FIELD_SERVICE_FREQWAVE_NUM               10011

/**装置ID，是字符的字符串。如:83
  *类型：字符串
  */
#define     FIELD_SERVICE_SETID                      10012

/**机组ID，是个5位字符的字符串。如:83C102
  *类型：字符串
  */
#define     FIELD_SERVICE_PLANTID                    10013

/**测点一，两位字符的字符串.如:1H
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANN1                     10014

/**测点二，两位字符的字符串.如:1H
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANN2                     10015

/**多个测点组合，中间以“,”分开
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANN_BUFF                 10016

/**测点字符串Buff的长度（即字符的个数）
  *类型：int
  */
#define     FIELD_SERVICE_CHANN_BUFFSIZE             10017

/**测点个数
  *类型：int
  */
#define     FIELD_SERVICE_CHANN_NUM                  10018


/**机组ID2，是个5位字符的字符串。如:83C102
  *类型：字符串
  */
#define     FIELD_SERVICE_PLANTID2                    10019

/**测点一特征值
  *类型：float
  */
#define     FIELD_SERVICE_CHANN1VALUE                10020

/**测点二特征值
  *类型：float
  */
#define     FIELD_SERVICE_CHANN2VALUE                10021

/**细化谱的数据序列
  *类型：float数组
  */
#define     FIELD_SERVICE_ZOOMSPECTRUM               10022

/**测点实时波形上一个波形数据的个数
  *类型：int
  */
#define     FIELD_SERVICE_TIMEWAVE_NUM               10023

/**频谱的df，一个机组的同一时刻的频谱df都一样
  *类型：float
  */
#define     FIELD_SERVICE_FREQWAVE_DF                10024

/**细化谱的分析线数
  *类型：int
  */
#define     FIELD_SERVICE_ZOOM_LINES                 10025

/**细化谱的起始频率
  *类型：float
  */
#define     FIELD_SERVICE_ZOOM_STARTFREQ             10026

/**细化谱的终止频率
  *类型：int
  */
#define     FIELD_SERVICE_ZOOM_STOPFREQ              10027


/**一组轴心位置的水平测点ID，两位字符的字符串.如:1V
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANNID_HOR                10028

/**一组轴心位置的垂直测点ID，两位字符的字符串.如:1H
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANNID_VER                10029

/**轴心轨迹的波形数据，垂直方向波形数据。
  *类型：short数组
  */
#define     FIELD_SERVICE_AXESTRACK_WAVE_VER         10030

/**轴心轨迹的波形数据，水平方向波形数据。
  *类型：short数组
  */
#define     FIELD_SERVICE_AXESTRACK_WAVE_HOR         10031

/**轴心轨迹的一个的波形数据个数
  *类型：int
  */
#define     FIELD_SERVICE_AXESTRACK_WAVENUM          10032

/**轴心位置的垂直方向值
  *类型：float
  */
#define     FIELD_SERVICE_AXESLOCATION_VER           10033

/**轴心位置的水平方向值
  *类型：float
  */
#define     FIELD_SERVICE_AXESLOCATION_HOR           10034

/**机组转速.
  *类型：float
  */
#define     FIELD_SERVICE_REV                        10035

/**机组转速2.
  *类型：float
  */
#define     FIELD_SERVICE_REV2                       10036

/**测点一单位
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANN1_UNIT                10037

/**测点二单位.如:MPa
  *类型：字符串
  */
#define     FIELD_SERVICE_CHANN2_UNIT                10038

/**测点所属列表框
  *类型：int
  */
#define     FIELD_SERVICE_CHANN1_LIST                10041

/**测点所处的状态。注：0表示没有报警，1表示处于报警状态
  *类型：int
  */
#define     FIELD_SERVICE_CHANN1_STATUS              10042

/**测点的相位值
 *类型：float
 */
#define     FIELD_SERVICE_TREND_PHASE                10043

/**测点的1X相位值
*类型：float
*/
#define     FIELD_SERVICE_TREND_ONEPHASE             10044

/**测点的2X相位值
*类型：float
*/
#define     FIELD_SERVICE_TREND_TWOPHASE             10045

/**直流量
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_DC2                  10046

/**间隙电压
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_GAP2                 10047


/**残振特征值
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_RESCHAR              10048

/**直流量
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_DC                   10049

/**间隙电压
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_TREND_GAP                   10050

/**如果机组通道多，可能需要发送多次，那多发送的最后一次需要把这个标识设为1，其它情况下为0
  */
#define     FIELD_SERVICE_SEND_END                   10051

/**是否需要通道信息
  *类型：int
  */
#define     FIELD_SERVICE_IFCHANN               10052

/**特征值结构体
  *类型：特征值结构体
  *值范围：FIELD_SERVICE_TREND_CHARVALUE
  */
#define     FIELD_SERVICE_CHARVALUE_S           10053

/**特征值结构体2
  *类型：特征值结构体
  *值范围：
  */
#define     FIELD_SERVICE_CHARVALUE_S2          10054

/**特征值结构体的buffer
*类型：指针，指向n个特征值结构体的buffer
*/
#define     FIELD_SERVICE_CHARVALUE_SBUFF       10055

/**单个特征值的buffer
*类型：指针，指向n（测点个数）个特征值的buffer
*/
#define     FIELD_SERVICE_TREND_VALUE_BUFF      10056

/**事件ID
  *类型：整型
  */
#define     FIELD_SYSTEM_EVENT_ID                10057

/**类型的buffer
*类型：指针，指向n（测点个数）个类型的buffer
*/
#define     FIELD_SERVICE_TYPE_BUFF              10058


/**长度信息
*类型：int
*/
#define     FIELD_SERVICE_LEN                    10059
/**事件类型
  *类型:int
  */
#define     FIELD_SYSTEM_EVENT_TYPE              10060

/**日志ID
  *类型：整型
  */
#define     FIELD_SYSTEM_ID                      10061

/**测点一对应的实时波形数据
  *类型：float数组
  */
#define     FIELD_SERVICE_TIMEWAVE1                  10080

/**测点二对应的实时波形数据
  *类型：float数组
  */
#define     FIELD_SERVICE_TIMEWAVE2                  10081

/**测点一对应的实时波形频谱数据，个数是波形数据的一半
  *类型：float数组
  */
#define     FIELD_SERVICE_FREQWAVE1                  10082

/**测点二对应的实时波形频谱数据
  *类型：float数组
  */
#define     FIELD_SERVICE_FREQWAVE2                  10083

/**波形数据压缩后的字节数标示符
  *类型：long
  */
#define     FIELD_SERVICE_ZIPWAVE_BYTESNUM           10084

/**波形数据压缩后的字节数标示符1
*类型：long
*/
#define     FIELD_SERVICE_ZIPWAVE_BYTESNUM1          10085

/**表示微秒
  *类型：int
  */
#define    FIELD_SERVICE_MICROSECOND                 10086

/**机组转速3.
*类型：float
*/
#define     FIELD_SERVICE_REV3                       10087

/**机组转速4.
*类型：float
*/
#define     FIELD_SERVICE_REV4                       10088


/**机组转速5.
*类型：float
*/
#define     FIELD_SERVICE_REV5                       10089

/**设备数.
*类型：int
*/
#define     FIELD_SERVICE_PLANT_NUM                  10090

/**报警设备数.
*类型：int
*/
#define     FIELD_SERVICE_ALARM_PLANTNUM             10091

/**运行设备数.
*类型：int
*/
#define     FIELD_SERVICE_RUN_PLANTNUM               10092

/**停车设备数.
*类型：int
*/
#define     FIELD_SERVICE_STOP_PLANTNUM              10093

/**报警ID.
*类型：string
*/
#define     FIELD_SERVICE_ALARMID                    10094

/**起始索引.
*类型：int
*/
#define     FIELD_SERVICE_STARTINDEX                 10095

/**结束索引
*类型：int
*/
#define     FIELD_SERVICE_ENDINDEX                   10096

/*所属汽缸缸号
*类型：int
*/
#define     FIELD_SERVICE_CYLINDERNO                 10097

/*角度参数1
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM1                 10098

/*角度参数2
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM2                 10099

/*角度参数3
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM3                 10100

/*角度参数4
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM4                 10101

/**多个测点2组合，中间以“,”分开
*类型：字符串
*/
#define     FIELD_SERVICE_CHANN_BUFF2                 10102

/**测点2字符串Buff的长度（即字符的个数）
*类型：int
*/
#define     FIELD_SERVICE_CHANN_BUFFSIZE2             10103

/**测点2个数
*类型：int
*/
#define     FIELD_SERVICE_CHANN_NUM2                 10104

/**多个测点3组合，中间以“,”分开
*类型：字符串
*/
#define     FIELD_SERVICE_CHANN_BUFF3                 10105

/**测点3字符串Buff的长度（即字符的个数）
*类型：int
*/
#define     FIELD_SERVICE_CHANN_BUFFSIZE3             10106

/**测点3个数
*类型：int
*/
#define     FIELD_SERVICE_CHANN_NUM3                  10107

/**垂直波形对应的频率
*类型：double
*/
#define     FIELD_SERVICE_VERFREQ                     10108

/**垂直波形对应的幅值
*类型：double
*/
#define     FIELD_SERVICE_VERAMP                      10109

/**垂直波形对应的相位
*类型：double
*/
#define     FIELD_SERVICE_VERPHASE                     10110

/**水平波形对应的频率
*类型：double
*/
#define     FIELD_SERVICE_HORFREQ                     10111

/**水平波形对应的幅值
*类型：double
*/
#define     FIELD_SERVICE_HORAMP                      10112

/**水平波形对应的相位
*类型：double
*/
#define     FIELD_SERVICE_HORPHASE                     10113

/**报警上限
*类型：float
*/
#define     FIELD_SERVICE_ALARM_LOW                     10114

/**报警上限
*类型：float
*/
#define     FIELD_SERVICE_ALARM_HIGH                    10115

/**轴心轨迹圆点X坐标
*类型：float
*/
#define     FIELD_SERVICE_AXIS_COORX                    10116

/**轴心轨迹圆点Y坐标
*类型：float
*/
#define     FIELD_SERVICE_AXIS_COORY                    10117

/**报警椭圆水平轴半径
*类型：float
*/
#define     FIELD_SERVICE_HOR_RADIUS                    10118

/**报警椭圆垂直轴半径
*类型：float
*/
#define     FIELD_SERVICE_VER_RADIUS                    10119


/** 程序/模块名
*类型：char *
*/
#define     FIELD_SERVICE_MODULE_NAME                         10108


/** 文件名
*类型：char *
*/
#define     FIELD_SERVICE_FILE_NAME                           10109


/** 文件路径
*类型：char *
*/
#define     FIELD_SERVICE_FILE_PATH                           10110

/** 文件版本
*类型：char *
*/
#define     FIELD_SERVICE_FILE_VERSION                        10111


/** 报警数据预计保存时间
*类型：float
*/
#define     FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME       10112

/** 报警趋势预计保存时间
*类型：float
*/
#define     FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME      10113

/** 报警数据和趋势预计保存时间
*类型：float
*/
#define     FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME 10114

/** 数据库数目
*类型：int
*/
#define     FIELD_SERVICE_DB_NUM                              10115

/** 数据库文件编号
*类型：int
*0 主库，
*/
#define     FIELD_SERVICE_DB_INDEX                            10116

/** 数据库数据文件名
*类型：char *
*/
#define     FIELD_SERVICE_DB_DATA_FILE_NAME                   10117

/** 数据库数据文件大小
*类型：float
*/
#define     FIELD_SERVICE_DB_DATA_FILE_SIZE                   10118


/** 数据库日志文件名
*类型：char *
*/
#define     FIELD_SERVICE_DB_LOG_FILE_NAME                    10119

/** 数据库日志文件大小
*类型：float
*/
#define     FIELD_SERVICE_DB_LOG_FILE_SIZE                    10120

/** 数据库名
*类型：char *
*/
#define     FIELD_SERVICE_DB_NAME                             10121

/** 数据库类型
*类型：int
*/
#define     FIELD_SERVICE_DB_TYPE                             10122

/** 数据库所在磁盘剩余空间
*类型：float
*/
#define     FIELD_SERVICE_FREE_DISK_SPACE                     10123

/** 数据库所在磁盘剩余空间比
*类型：float
*/
#define     FIELD_SERVICE_FREE_DISK_SPACE_RATIO               10124

/** 前一天增长最快的表，CSV格式
*类型：char *
*/
#define     FIELD_SERVICE_FAST_INC_TABLE                      10125

/** 数据库所在磁盘总空间，单位为GB
*类型：float
*/
#define     FIELD_SERVICE_TOTAL_DISK_SPACE                    10126

/** 中间件连接状态
*类型：int, GE_MW_LINK_STATUS
*/
#define     FIELD_SERVICE_MW_LINK_STATUS                      10127

/** 中间件连接状态
*类型：int, GE_DB_STATUS
*/
#define     FIELD_SERVICE_DB_STATUS                           10128

/** 中间件正在接收数据的设备数目
*类型：int,
*/
#define     FIELD_SERVICE_DATA_RCV_PLANT_NUM                  10129

/** 中间件数据接收状态
*类型：vector<BHMWDataRcvStatus>
*/
#define     FIELD_SERVICE_DATA_RCV_STATUS                     10130

/** 中间件正在保存数据的设备数目
*类型：int,
*/
#define     FIELD_SERVICE_DATA_SAVE_PLANT_NUM                 10131

/** 中间件数据保存状态
*类型： vector<BHMWDataSaveStatus>
*/
#define     FIELD_SERVICE_DATA_SAVE_STATUS                    10132

/** 中间件正在同步数据的设备数目
*类型：int,
*/
#define     FIELD_SERVICE_DATA_SYNC_PLANT_NUM                 10133

/** 中间件数据同步状态
*类型： vector<BHMWDataSaveStatus>
*/
#define     FIELD_SERVICE_DATA_SYNC_STATUS                    10134


/** 数采卡索引
*类型： int， 0...
*/
#define     FIELD_SERVICE_DAQ_CARD_INDEX                      10135

/** 数采卡数量
*类型： int
*/
#define     FIELD_SERVICE_DAQ_CARD_NUM                        10136

/** 数采卡逻辑名称
*类型： char *
*/
#define     FIELD_SERVICE_DAQ_CARD_NAME                       10137

/** 数采卡使用AI通道(CSV)
*类型： char *，CSV
*/
#define     FIELD_SERVICE_DAQ_USED_AI_CHANNEL                 10138


/** 数采卡工作状况
*类型： int, GE_DAQ_CARD_STATUS
*/
#define     FIELD_SERVICE_DAQ_CARD_STATUS                     10139

/** 数采软件版本号
*类型：char *
*/
#define     FIELD_SERVICE_DAQ_SW_VERSION                      10140

/** 数采软件采集模式
*类型：int (GE_DAQ_SW_ACQUIRE_TYPE)
*/
#define     FIELD_SERVICE_DAQ_ACQUIRE_TYPE                    10141


/** 数采软件间隙电压接收状态
*类型：int (bool) 1 正常, 0异常
*/
#define     FIELD_SERVICE_DAQ_GAP_RCV_STATUS                  10143


/** 断网数据保存时间
*类型：COleDateTime
*/
#define     FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME         10144



/** 断网数据保存时间
*类型：COleDateTime
*/
#define     FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME           10145

/** 振动数据发送时间
*类型：COleDateTime
*/
#define     FIELD_SERVICE_DAQ_VIB_TIME                        10146

/** 动态数据发送时间
*类型：COleDateTime
*/
#define     FIELD_SERVICE_DAQ_DYN_TIME                        10147

/** 过程量数据发送时间
*类型：COleDateTime
*/
#define     FIELD_SERVICE_DAQ_PROC_TIME                       10148

/** 使用CI通道(CSV)
*类型： char *，CSV
*/
#define     FIELD_SERVICE_DAQ_USED_CI_CHANNEL                 10149

/** 数采卡使用AI通道数
*类型：int
*/
#define     FIELD_SERVICE_AI_CHANN_NUM                        10150

/** 数采卡使用CI通道数
*类型：int
*/
#define     FIELD_SERVICE_CI_CHANN_NUM                        10151


/** 报警设置(秒数，0表示未设置)
*类型: int
*/
#define     FIELD_SERVICE_CONFIG_ALARM                        10152

/** 连接的数据库
*类型: 字符串
*/
#define     FIELD_SERVICE_CONFIG_CONNECTED_DB                 10153

/** 保存数据设置(秒数，0表示未设置)
*类型: int
*/
#define     FIELD_SERVICE_CONFIG_DATA_SAVING                  10154

/** 同步数据设置(秒数，0表示未设置)
*类型: int
*/
#define     FIELD_SERVICE_CONFIG_DATA_SYNC                    10155

/** 中间件服务器型(0表示装置级，1表示中心级)
*类型: int
*/
#define     FIELD_SERVICE_CONFIG_SERVER_TYPE                  10156

/** 公司数目
*类型: int
*/
#define     FIELD_SERVICE_COMPANY_NUM                         10157

/** 是否存在键相
*类型: int
*/
#define     FIELD_SERVICE_KEY_IF                              10158

/** 传感器灵敏度
*类型: int
*/
#define     FIELD_SERVICE_SENSITIVITY                         10159

/** 传感器偏置量
*类型: int
*/
#define     FIELD_SERVICE_SENSITIVITY1                        10160

/** 测点方向
*类型: int
*/
#define     FIELD_SERVICE_MEASURE_DIRECTION                   10161

/** 数据类型
*类型: int
*/
#define     FIELD_SERVICE_DATA_TYPE                           10162

/** 直流放大倍数
*类型: float
*/
#define     FIELD_SERVICE_DC_AMPLIFY                          10163

/** 交流放大倍数
*类型: float
*/
#define     FIELD_SERVICE_AC_AMPLIFY                          10164

/**轴心轨迹的波形数据，垂直方向波形数据。
  *类型：float数组
  */
#define FIELD_SERVICE_AXESTRACK_WAVE_VER_FLOAT                        10165

/**轴心轨迹的波形数据，水平方向波形数据。
  *类型：float数组
  */
#define FIELD_SERVICE_AXESTRACK_WAVE_HOR_FLOAT                        10166

/******************只有读取历史数据需要的字段码**************************************/

/**读取历史趋势时，测点一的ID
  *类型：string
  */
#define     FIELD_SERVICE_OFF_CHANN1                 11000

/**读取历史趋势时，测点二的ID
  *类型：string
  */
#define     FIELD_SERVICE_OFF_CHANN2                 11001

/**读取历史趋势时，时间值
  *类型：DateTime
  */
#define     FIELD_SERVICE_OFF_TIME                   11002


/**读取历史趋势时，时间值2
  *类型：DateTime
  */
#define     FIELD_SERVICE_OFF_TIME2                  11003

/**读取历史趋势时，测点一的ID值
  *类型：float
  */
#define     FIELD_SERVICE_OFF_CHANN1VALUE            11004

/**读取历史趋势时，测点二的ID值
  *类型：float
  */
#define     FIELD_SERVICE_OFF_CHANN2VALUE            11005

/**读取历史报警信息已经显示条数的值
  *类型：int
  */
#define    FIELD_SERVICE_OFF_SHOW_ALARM_INFO_NUM     11006

/**趋势类型的枚举.
  *类型：整型.
  *值范围：小时0，天1，周2，月3，年4.
  */
#define     FIELD_SERVICE_OFF_TRENDTYPE              11009

/**查询记录时候的起始日期时间
  *类型：时间
  *值范围：
  */
#define     FIELD_SERVICE_OFF_TIMESTART              11010

/**查询记录时候的终止日期时间
  *类型：时间
  *值范围：
  */
#define     FIELD_SERVICE_OFF_TIMEEND                11011

/**读取历史趋势时一个对应点的频谱ID值
  *类型：float数组
  */
#define     FIELD_SERVICE_OFF_FREQWAVE               11012

/**读取历史趋势时一个对应点的波形数据个数
  *类型：int
  */
#define     FIELD_SERVICE_OFF_WAVE_NUM               11013

/**读取历史趋势时一个对应点的频谱DF的ID值
  *类型：float
  */
#define     FIELD_SERVICE_OFF_FREQWAVE_DF            11014

/**读取历史趋势时一个对应点的波形数据
*类型：float数组
*/
#define     FIELD_SERVICE_OFF_WAVE                   11015

/*!测点类型,是所有过程量测点，还是振动测点，或者是某一过程量测点
  *类型：int
  *值范围：0-10
  */
#define    FIELD_SERVICE_OFF_CHANNTYPE               11016

/**读取历史趋势时一个对应点的频谱数据个数
  *类型：int
  */
#define     FIELD_SERVICE_OFF_FREQWAVE_NUM           11017

/**读取对应的波形数据时要求的频谱分析线数
  *类型：int
  *值范围：400，800，1600...
  */
#define     FIELD_SERVICE_OFF_FREQANALYSIS_LINES     11018

/**返回频谱分析的实际线数
  *类型：int
  *值范围：400，800，1600...
  */
#define     FIELD_SERVICE_OFF_FACT_ANALYSISLINES     11019

/*!启停车记录的幅值
  *类型：float
  */
#define    FIELD_SERVICE_OFF_AMP                     11020

/*!启停车记录的相位
  *类型：float
  */
#define    FIELD_SERVICE_OFF_PHASE                   11021


/**读取历史趋势时，测点一的相位值
  *类型：float
  */
#define     FIELD_SERVICE_OFF_CHANN1PHASE            11022

/**读取历史趋势时，测点二的相位值
  *类型：float
  */
#define     FIELD_SERVICE_OFF_CHANN2PHASE            11023

/**读取报警信息时使用，门限结构体
  *类型：结构体
  */
#define     FIELD_SERVICE_THRESHOLD                  11024

/**读取报警门限结构体Buff
  *类型：结构体Buff
  */
#define     FIELD_SERVICE_THRESHOLD_BUFF             11025

/**阈值门限ID
  *类型：int
  */
#define     FIELD_SERVICE_THRESHOLD_ID               11026

/**学习参数BUFF
  *类型：结构体
  */
#define     FIELD_SERVICE_LEARNPARAM_BUFF            11027

/**报警类型
  类型：int
  */
#define     FIELD_SERVICE_ALARM_TYPE                 11028

/*!对应机组的转速
  *类型：int
  */
#define    FIELD_SERVICE_OFF_REV                     11030

/*!对应机组的转速2
  *类型：int
  */
#define    FIELD_SERVICE_OFF_REV2                    11031

/*转速BUFF
  *类型：结构体
  */
#define     FIELD_SERVICE_REV_BUFF                   11032

/* 数据
  *类型：float
  */
#define    FIELD_SERVICE_OFF_DATA                    11033





/****************接受程序发送实时数据的字段码***********/

/**发送所有振动测点一倍频的相位
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_ONEVIBPHASE        14000

/**发送所有振动测点的实时波形数据
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_VIBWAVE            14001

/**发送的实时波形数据的组数，即所有振动测点数目
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_VIBCHANNNUM        14002

/**发送实时波形数据的采集点数，即一组波形的个数
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_WAVEPOINTNUM       14003

/**发送的轴位移数据个数，即轴位移测点数目
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_AXISCHANNNUM       14004

/**发送的过程量数据个数，即过程量测点数目
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_PROCCHANNNUM       14005

/**发送的所有轴位置数据
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_AXISLOCATION       14006

/**发送所有过程量测点的运行状态
  *类型：报警状态结构体数组。
  */
#define     FIELD_SERVICE_ON_SEND_ALLPROCSTATE       14007

/**发送所有振动测点的运行状态（是对通频特征进行判断）
  *类型：报警状态结构体数组
  */
#define     FIELD_SERVICE_ON_SEND_ALLVIBSTATE        14008

/**发送所有轴位置测点的运行状态
  *类型：报警状态结构体数组。
  */
#define     FIELD_SERVICE_ON_SEND_AXISLOCATIONSTATE  14009

/**发送所有机组的转速
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_REV                14010

/**机组号
  *类型:string
  */
#define     FIELD_SERVICE_ON_SEND_PLANTID            14011

/**发送所有实时振动测点通频特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_ALLCHARVALUE       14012

/**发送所有实时振动测点二倍频特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_TWOCHARVALUE       14013

/**发送所有实时振动测点三倍频频特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_THREECHARVALUE     14014

/**发送所有实时振动测点一分频频特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_HALFCHARVALUE      14015

/**发送所有实时振动测点一倍频特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_ONECHARVALUE       14016

/**发送所有实时振动测点轴位移特征值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_AXISDISPLACE       14017

/**发送所有过程量的值值
  *类型：float数组
  */
#define     FIELD_SERVICE_ON_SEND_ALLPROCVALUE       14018

/**发送所有实时数据时的起始索引
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_STARTID            14019

/**从中间件上得到指定测点类型的所有实时数据
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_CHANNTYPE          14020

/**采样频率
  *类型：int
  */
#define    FIELD_SERVICE_SAMPLE_FREQ                 14021

/**采样点数
*类型：int
*/
#define    FIELD_SERVICE_SAMPLE_NUM                  14022

/**启停车状态标志位，
  *类型：int
  */
#define    FIELD_SERVICE_SUD_STATUS                  14023

/**启停车类型,启停车类型 E_SUD_TYPE
*类型：int
*/
#define    FIELD_SERVICE_SUD_TYPE                    14024

/**发送所有振动测点二倍频的相位
*类型：float数组
*/
#define    FIELD_SERVICE_ON_SEND_TWOVIBPHASE        14025

/**发送所有振动测点三倍频的相位
*类型：浮点数数组
*/
#define   FIELD_SERVICE_ON_SEND_THREEVIBPHASE       14026

/**发送所有振动测点分倍频的相位
*类型：float数组
*/
#define   FIELD_SERVICE_ON_SEND_HALFVIBPHASE        14027

/**发送所有振动测点残振值
*类型：float数组
*/
#define   FIELD_SERVICE_ON_SEND_RESCHARVALUE        14028

/**报警状态标志位
*类型：int
*/
#define    FIELD_SERVICE_ALARM_STATUS               14029

/**发送的动态数据个数，即动态测点数目
*类型：int
*/
#define     FIELD_SERVICE_ON_SEND_DYNCHANNNUM       14030

/**发送所有动态测点的运行状态
*类型：报警状态结构体数组。
*/
#define     FIELD_SERVICE_ON_SEND_ALLDYNSTATE        14031

/**发送所有振动测点的实时波形数据
  *类型：short数组
  */
#define     FIELD_SERVICE_ON_SEND_VIBWAVE2            14601

/** 原始float振动数据的最小值
  *类型：float
  */
#define     FIELD_SERVICE_ON_SEND_ORGINAL_MIN            14602

/** 原始float振动数据的最大值
  *类型：float
  */
#define     FIELD_SERVICE_ON_SEND_ORGINAL_MAX            14603

/** 是否存在精度转换信息
  *类型：int
  */
#define     FIELD_SERVICE_ON_SEND_HAS_PRECISION_INFO            14604

/** 波形
  *类型：float
  */
#define     FIELD_SERVICE_SEND_FLOAT_WAVE1            14605

/** 波形
  *类型：float
  */
#define     FIELD_SERVICE_SEND_FLOAT_WAVE2            14606

/** 波形
  *类型：float
  */
#define     FIELD_SERVICE_SEND_FLOAT_FREQ1            14607

/** 波形
  *类型：float
  */
#define     FIELD_SERVICE_SEND_FLOAT_FREQ2            14608


/**--------------系统级功能调用字段码--------------**/

/**用户名
  *类型：string
  */
#define     FIELD_SYSTEM_USEID                        15000

/**用户密码
  *类型：string
  *值范围：
  */
#define     FIELD_SYSTEM_PASSWORD                     15001

/**装置号
  *类型：string
  */
#define     FIELD_SYSTEM_SET                          15002

/**分厂名
  *类型：string
  */
#define     FIELD_SYSTEM_FACTORY                      15003

/**公司名
  *类型：string
  */
#define     FIELD_SYSTEM_COMPANY                      15004

/**机组位号
  *类型：string
  */
#define     FIELD_SYSTEM_PLANT                        15005

/**集团名
  *类型：string
  */
#define     FIELD_SYSTEM_GROUP                        15006

/**轴承位置描述
  *类型：string
  */
#define     FIELD_SYSTEM_AXISLOCATION                 15007

/**各种类型标示符
  *类型：整型
  */
#define     FIELD_SYSTEM_TYPE                         15008

/**测点名
  *类型：string
  */
#define     FIELD_SYSTEM_CHANN                        15009

/**测点类型
  *类型：int
  *值范围：1代表振动测点，2代表轴位移，3代表压力，4代表温度，5代表流量等
  */
#define     FIELD_SYSTEM_CHANN_TYPE                   15010

/**振动测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_VIBCHANN_NUM                 15011

/**轴位移测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_AXISCHANN_NUM                15012

/**流量测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_FLUXCHANN_NUM                15013

/**压力测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_PRESSCHANN_NUM               15014

/**温度测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_TEMPCHANN_NUM                15015

/**其他测点数目
  *类型：int
  */
#define     FIELD_SYSTEM_OTHERCHANN_NUM               15016

/**垂直方向测点号
  *类型：string
  */
#define     FIELD_SYSTEM_CHANN_VER                    15017

/**水平方向测点号
  *类型：string
  */
#define     FIELD_SYSTEM_CHANN_HOR                    15018

/**发送指定机组的最高分析线数对应的宏定义
  *类型:int
  */
#define     FIELD_SYSTEM_ANALYSISLINES                15019


/**进行某种操作时的返回状态，例如修改密码后返回状态是否修改
  *类型：int
  */
#define     FIELD_SYSTEM_STATUS                       15020

/**进行某种需要字符串的操作，例如：修改密码时的新密码
  *类型：string
  */
#define     FIELD_SYSTEM_RESERVE_STRING               15021

/**客户端察看实时数据时的更新时间的妙数
  *类型：int
  */
#define     FIELD_SYSTEM_REFRESHTIME                  15022

/**报警上限
  *类型:float
  */
#define     FIELD_SYSTEM_ALARM_HIGH                   15023

/**报警下限
  *类型:float
  */
#define     FIELD_SYSTEM_ALARM_LOW                    15024

/**@brief 用户级别
  *type: int
  *value range:参照DSE_Define.h中关于用户等级的枚举
  */
#define     FIELD_SYSTEM_USER_LEVEL                   15025


/**@brief 用户ID，等修改其他用户等级或者添加用户时使用
  *type: string
  */
#define     FIELD_SYSTEM_MODIFY_USER                  15026

/**@brief 状态描述
  *type: string
  */
#define     FIELD_SYSTEM_STATUS_DISCRIPTION           15027

/**@brief 判断是否浏览装置
  *type:int
  */
#define     FIELD_SYSTEM_BROWSE_SET                   15028

/**@brief 采样点数
  *type:int
  */
#define     FIELD_SYSTEM_SAMPLE_NUM                   15029

/**@brief 采样频率
  *type:int
  */
#define     FIELD_SYSTEM_SAMPLE_FREQ                  15030

/**@brief 数采终端IP
  *type:string
  */
#define     FIELD_SYSTEM_DAQIP                        15031

/**测点别名
  *type：string
  */
#define    FIELD_SYSTEM_CHANN_ALIAS                   15032

/**@brief 机组转速
  *type:int
  */
#define     FIELD_SYSTEM_PLANT_REV                    15033

/**概貌图类型
  *type:int
  */
#define     FIELD_SYSTEM_PLANT_GRAPHTYPE              15034


/**分厂名2
  *类型：string
  */
#define     FIELD_SYSTEM_FACTORY2                      15035

/**公司名2
  *类型：string
  */
#define     FIELD_SYSTEM_COMPANY2                      15036

/**报警上限2
  *类型:float
  */
#define     FIELD_SYSTEM_ALARM_HIGH2                   15037

/**报警下限2
  *类型:float
  */
#define     FIELD_SYSTEM_ALARM_LOW2                    15038

/**传感器类型
  *类型：int
  *值范围：见枚举定义
  */
#define     FIELD_SYSTEM_SENSOR_TYPE                   15039

/**机组概貌图BUFF
  *类型：BUFF
  */
#define    FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF          15040

/**机组概貌图BUFF大小
  *类型：int
  */
#define    FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE      15041

/**测点2别名
*type：string
*/
#define    FIELD_SYSTEM_CHANN2_ALIAS                   15042

/**报警上限3
*类型:float
*/
#define     FIELD_SYSTEM_ALARM_HIGH3                   15043

/**报警下限3
*类型:float
*/
#define     FIELD_SYSTEM_ALARM_LOW3                    15044

/**特征值结构体
  *类型：特征值结构体
  *值范围：根据实际情况来决定。比如是振动测点的特征值结构体还是动态测点的特征值结构体
  */
#define     FIELD_SYSTEM_CHARVALUE_S                  15045

/**机组概貌图字体大小
  *类型：int
  */
#define    FIELD_SYSTEM_PLANT_GRAPHFONT_SIZE          15046

/**集团级中间件服务器IP
*类型：string
*/
#define   FIELD_SYSTEM_GROUP_MW_IP                    15136

/**集团级中间件服务器端口
*类型：int
*/
#define   FIELD_SYSTEM_GROUP_MW_PORT                  15137

/**公司级中间件服务器IP
  *类型：string
  */
#define   FIELD_SYSTEM_COMPANY_MW_IP                  15138

/**公司级中间件服务器端口
  *类型：int
  */
#define   FIELD_SYSTEM_COMPANY_MW_PORT                15139

/**中间件服务器IP
  *类型：string
  */
#define   FIELD_SYSTEM_MW_IP                          15140

/**中间件服务器端口
  *类型：int
  */
#define   FIELD_SYSTEM_MW_PORT                        15141

/**发送数据到监测中心的时间间隔
  *类型：int
  */
#define   FIELD_SYSTEM_SENDC_INTERVAL                 15142

/**是否发送数据到监测中心
  *类型：int
  */
#define   FIELD_SYSTEM_SENDC_IF                       15143


/**分厂别名
  *类型：string
  */
#define   FIELD_SYSTEM_SUBCORP_ALIAS                  15144

/**装置级中间件ID
  *类型：string
  */
#define   FIELD_SYSTEM_MW_ID                          15145


/**装置级中间件设备信息
  *类型：结构体数组
  */
#define   FIELD_SYSTEM_SETSVR_PLANTINFO              15146

/**装置级中间件设备的测点信息
  *类型：结构体数组
  */
#define   FIELD_SYSTEM_SETSVR_CHANNINFO              15147

/**发送历史启停车数据到监测中心的时间间隔
*类型：int
*/
#define   FIELD_SYSTEM_SUD_SENDC_INTERVAL            15148

/**发送历史报警数据到监测中心的时间间隔
*类型：int
*/
#define   FIELD_SYSTEM_ALARM_SENDC_INTERVAL           15149

/**装置级中间件的信息
*类型：结构体
*/
#define   FIELD_SYSTEM_SETSVR_SETINFO                 15150

/**版本信息
*类型：浮点数
*/
#define   FIELD_SYSTEM_VERSION                        15151

/**自定义参数模板ID
*类型：string
*/
#define FIELD_SYSTEM_TEMPID                          15152

/**数据类型
*类型：int
*/
#define  FIELD_SYSTEM_DATA                           15153

/**数据类型
*类型：int
*/
#define  FIELD_SYSTEM_UNIT                           15154

/**信号分析方式
*类型：int
*/
#define  FIELD_SYSTEM_SIGNAL_TYPE                    15155

/**信号分析方式
*类型：int
*/
#define  FIELD_SYSTEM_SEG_STARTNO                    15156

/**段数
*类型：int
*/
#define  FIELD_SYSTEM_SEG_NUM                        15157

/**数据起始值
*类型：float
*/
#define FIELD_SYSTEM_START_VALUE                     15158

/**数据结束值
*类型：float
*/
#define  FIELD_SYSTEM_END_VALUE                      15159

/**特征值类型
*类型：int
*/
#define   FIELD_SYSTEM_CHAR_TYPE                     15160

/**自定义报警模板ID
*类型：string
*/
#define   FIELD_SYSTEM_ALARM_TEMPID                  15161

/**自定义报警描述
*类型：string
*/
#define   FIELD_SERVICE_ALARM_DISCRIPTION            15162

/**是否报警
*类型：int
*/
#define   FIELD_SYSTEM_IF_ALARM                      15163

/**报警组
*类型：int
*/
#define  FIELD_SYSTEM_ALARM_GROUP                    15164

/**段名，即自定义参数的分段描述
*类型：string
*/
#define  FIELD_SYSTEM_SEG_NAME                       15165

/**包络频带起始值
*类型：float
*/
#define FIELD_SYSTEM_BAND_STARTVALUE                  15166

/**包络频带结束值
*类型：float
*/
#define  FIELD_SYSTEM_BAND_ENDVALUE                   15167

/**齿轮Z12的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ12                      15168

/**齿轮Z21的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ21                      15169

/**齿轮Z22的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ22                      15170

/**齿轮Z31的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ31                      15171

/**齿轮Z32的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ32                      15172

/**齿轮Z41的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ41                      15173

/**齿轮Z42的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ42                      15174

/**齿轮Z51的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ51                      15175

/**齿轮Z52的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ52                      15176

/**齿轮Z61的齿数
*类型：int
*/
#define  FIELD_SYSTEM_GEARNUMZ61                      15177

/** 数据源
*类型：int(GE_DATA_SOURCE_EN)
*/
#define  FIELD_SYSTEM_DATASOURCE                      15178

/*! 其它测点对应的特征值类型
 因21060接口区分电动机等需要增加 (部位Number > 100)
 */
#define  FIELD_SERVICE_OTHER_CHARTYPE                 15179


/*! 滚动轴承滚动体（滚珠）的直径，单位：mm.*/
#define  FIELD_SYSTEM_ROLLINDIAM                      15180

/*! 滚动轴承的压力角（又称接触角），单位：DEG.*/
#define  FIELD_SYSTEM_CONTACKANGLE                    15181

/*! 滚动轴承滚动体的个数.*/
#define  FIELD_SYSTEM_NUMOFROLLING                    15182

/*! 滚动轴承的节径，单位：mm.*/
#define  FIELD_SYSTEM_PITCHDIAM                       15183

/*!是否设置了滚动轴承信息*/
#define  FIELD_SYSTEM_GEARVALID                       15184

#define     FIELD_SERVICE_ALARMID_CHANN_NUM           15189


/** 数据压缩方式 0 不压缩 */
enum {
	DATA_NO_COMPRESSION = 0,
	DATA_COMPRESSED     = 1
};
#define     FIELD_SERVICE_DATA_COMPRESSION            15190


/**报警ID数组.
*类型：string
*/
#define     FIELD_SERVICE_ALARMIDS                    15185

#define     FIELD_SERVICE_DATALIMIT                   15186

/**ID个数
  *类型：int
  */
#define     FIELD_SERVICE_ALARMID_NUM                 15187

#define     FIELD_SERVICE_ALARMID_BUFFSIZE            15188

/*
 自定义预处理参数k,b
*/
#define FIELD_SERVICE_PREPROC_K                       15190
#define FIELD_SERVICE_PREPROC_B                       15191


/** 部位位置相关定义
 */
#define FIELD_SERVICE_SEC_IDX                         15192
#define FIELD_SERVICE_SEC_NAME                        15193
#define FIELD_SERVICE_LOC_IDX                         15194
#define FIELD_SERVICE_LOC_NAME                        15195
#define FIELD_SERVICE_PREV_SEC_IDX                    15196
#define FIELD_SERVICE_PREV_LOC_IDX                    15197

#define FIELD_SERVICE_FUNCTION_TYPE                   15198
#define FIELD_SERVICE_SECTION_TYPE                    15199

#define FIELD_SERVICE_SECTION_PIC                     15200
#define FIELD_SERVICE_LOCATION_NO                     15201

#define FIELD_SERVICE_UNIT                            15202

/**
 预留参数
 */
#define FIELD_SERVICE_RESERVED_PARAM1                 15203
#define FIELD_SERVICE_RESERVED_PARAM2                 15204
#define FIELD_SERVICE_RESERVED_PARAM3                 15205

/**--------------专家系统功能调用字段码--------------**/

/**事实规则ID
*类型：string
*/
#define     FIELD_SERVICE_EX_RULEID                   16000

/**事实规则ID对应的值
*类型：int
*/
#define     FIELD_SERVICE_EX_IDVALUE                  16001


/**测点2名
  *类型：string
  */
#define     FIELD_SYSTEM_CHANN2                       16002

/*角度参数5
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM5                 16003

/*角度参数6
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM6                 16004

/*角度参数7
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM7                 16005

/*角度参数8
*类型：float
*/
#define     FIELD_SERVICE_ANGLEPARAM8                 16006

/*经度
*类型：float
*/
#define     FIELD_SERVICE_LONGITUDE                   16007

/*纬度
*类型：float
*/
#define     FIELD_SERVICE_LATITUDE                    16008

/*纬度
*类型：float
*/
#define     FIELD_SERVICE_PLANT_TYPE                    16009


/*设备所属中间件类型 0当前中间件 >0 远程中间件
*类型：int
*/
#define     FIELD_SERVICE_PLANT_DBTYPE                    16010



/*动平衡测点1BUFFER
*类型：buffer
*/
#define     FIELD_SERVICE_OFF_DATA_CHANN_1                    16011


/*动平衡测点2BUFFER
*类型：buffer
*/
#define     FIELD_SERVICE_OFF_DATA_CHANN_2                    16012

/*BUFFER1 长度
*类型：int
*/
#define     FIELD_SERVICE_OFF_DATA_CHANN_1_LEN                    16013


/*BUFFER2 长度
*类型：int
*/
#define     FIELD_SERVICE_OFF_DATA_CHANN_2_LEN                    16014


/*动平衡类型
*类型：int
*/
#define     FIELD_SERVICE_OFF_BALANCE_TYPE  16015

/*设备树结构显示/隐藏
*类型：int
*/
#define FIELD_SERVICE_GROUP_VISIBLE   16016
#define FIELD_SERVICE_COMPANY_VISIBLE 16017
#define FIELD_SERVICE_FACTORY_VISIBLE 16018
#define FIELD_SERVICE_SET_VISIBLE     16019
#define FIELD_SERVICE_PLANT_VISIBLE   16020

/*磁盘满信息
*类型:int
*/
#define FIELD_SERVICE_DISK_FULL   16021


/*报警确认的确认类型,值域为1,2,3 
*类型:int
*/
#define FIELD_SERVICE_ALARM_CONFIRM_TYPE   16022


/*数据代码
*类型:字符串
*/
#define FIELD_SERVICE_DATA_CODE   16023

/*备注
*类型:字符串
*/ 
#define FIELD_SERVICE_REMARK	  16024

/*客户端IP
*类型：字符串
*/
#define FIELD_SYSTEM_CLIENT_IP 16025

/*取国家信息
*类型：字符串
*/
#define FIELD_SYSTEM_COUNTRY 16026

/*启停车名称
*类型：字符串
*/
#define FIELD_SYSTEM_EVENT_NAME 16027

/*设备显示名称
*类型：字符串
*/
#define FIELD_SYSTEM_PLANT_DISPLAY 16028


/* 是否是同步上来的报警
*类型：LONG
*/
#define FIELD_SERVICE_ALARM_SYNC   16029


/*分割符
*类型：字符串
*/
#define FIELD_SERVICE_ITEM_SEPERATOR   16030

/*分割符
*类型：字符串
*/
#define FIELD_SERVICE_GROUP_SEPERATOR  16031

/* 设备标识
*类型：LONG
*/
#define FIELD_SERVICE_DEVICE_ID        16032

/*设备ERP编码
*类型：字符串
*/
#define FIELD_SYSTEM_PLANT_ERP_CODE    16033

/*是否需要判断报警
*类型: LONG
*/
#define FIELD_SERVICE_JUDGE_ALARM      16034

/*装置显示名称
*类型: 字符串
*/
#define FIELD_SYSTEM_SET_DISPLAY      16035

/*快变测点长度
*类型: LONG
*/
#define FIELD_SERVICE_FC_CHANN_BUFFSIZE 16037

/*集团显示名称
*类型：字符串
*/
#define FIELD_SYSTEM_GROUP_DISPLAY 16038

/*公司显示名称
*类型：字符串
*/
#define FIELD_SYSTEM_COMPANY_DISPLAY 16039

/*分厂显示名称
*类型：字符串
*/
#define FIELD_SYSTEM_FACTORY_DISPLAY 16040

/*集团排序号
*类型：整形
*/
#define FIELD_SYSTEM_GROUP_SORTNO 16041

/*公司排序号
*类型：整形
*/
#define FIELD_SYSTEM_COMPANY_SORTNO 16042

/*分厂排序号
*类型：整形
*/
#define FIELD_SYSTEM_FACTORY_SORTNO 16043

/*装置排序号
*类型：整形
*/
#define FIELD_SYSTEM_SET_SORTNO 16044

/*设备排序号
*类型：整形
*/
#define FIELD_SYSTEM_PLANT_SORTNO 16045


/*修改报警限主机名
*类型：字符串
*/
#define FIELD_SYSTEM_HOST_NAME   16046

/*修改报警限主机名
*类型：字符串
*/
#define FIELD_SYSTEM_USER_NAME   16047

/*无线测点是否在线
*类型：整形
*/
#define FIELD_SYSTEM_ONLINE      16048

/*数据是否正常
*类型：整形
*/
#define FIELD_SYSTEM_NORMAL      16049

/*数据是否更新
*类型：整形
*/
#define FIELD_SYSTEM_DATAUPDATE  16050

/*电池电量
*类型：整形
*/
#define FIELD_SYSTEM_BATTERY_POWER   16051


/*无线测点状态
*类型：buffer
*/
#define FIELD_SYSTEM_WIRELESS_CHANN_STATUS   16052

/*中间件类型
*类型：整形
*/
#define   FIELD_SYSTEM_MW_TYPE               16053
#define   FIELD_SYSTEM_TM_MW_TYPE            16054

//概貌图信息

/*概貌图列表框buffer大小
*类型：整形
*/
#define   FIELD_SYSTEM_GRAPH_LISTBOX_BUFFERSIZE     16055

/*概貌图列表框buffer
*类型：buffer
*/
#define   FIELD_SYSTEM_GRAPH_LISTBOX_BUFFER         16056


/*机组概貌图,区分概貌图，装配图和原理图
*类型：整形
*/
#define   FIELD_SERVICE_BHCLIENT_GROUPHINDEX        16057

/*低报机组概貌图大小
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_LOW_ALARM_BUFFSIZE   16058

/*低报机组概貌图
*类型：buffer
*/
#define   FIELD_SYSTEM_GROUPH_LOW_ALARM_BUFF        16059

/*高报机组概貌图大小
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_HIG_ALARM_BUFFSIZE   16060

/*高报机组概貌图
*类型：buffer
*/
#define   FIELD_SYSTEM_GROUPH_HIG_ALARM_BUFF        16061

/*low_alarm_xoffset
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_LOW_ALARM_XOFFSET    16062

/*low_alarm_yoffset
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_LOW_ALARM_YOFFSET    16063

/*部位号
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_ACCESS_NUMBER        16064

/*概貌图类型
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_IMAGEKIND            16065


/*hig_alarm_xoffset
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_HIG_ALARM_XOFFSET    16066

/*hig_alarm_yoffset
*类型：整形
*/
#define   FIELD_SERVICE_GROUPH_HIG_ALARM_YOFFSET    16067

/*报警类型
*类型：整形
*/
#define   FIELD_SERVICE_GRAPH_ALARM_TYPE            16068


/*每次发包大小
*类型：整形
*/
#define   FIELD_SERVICE_PACKAGE_EVERYTIME_BUFFSIZE  16069

/*最后一次发包大小
*类型：整形
*/
#define   FIELD_SERVICE_PACKAGE_END_BUFFSIZE        16070

/*发包个数
*类型：整形
*/
#define   FIELD_SERVICE_PACKAGE_TIMES               16071



/*是否参与判断开停车
*类型：整形
*/
#define   FIELD_SERVICE_JUDGE_SUD             16072

/*点火顺序
*类型：字符串
*/
#define FIELD_SYSTEM_FIRE_SEQUECE     16072 

/*点火提前角顺序
*类型：字符串
*/
#define FIELD_SYSTEM_FIRE_ADVANCE_ANGLE_PHASE     16073 

/*气阀开启相位
*类型：字符串
*/
#define FIELD_SYSTEM_VALVE_OPEN_PHASE     16074 

/*气阀关闭相位
*类型：字符串
*/
#define FIELD_SYSTEM_VALVE_CLOSE_PHASE     16075 

/*上止点相位
*类型：字符串
*/
#define FIELD_SYSTEM_TDC_PHASE     16076  

/*查询标识字符串
*类型：字符串
*/
#define FIELD_SYSTEM_MARKNAME 16077


// 波形放大倍数
#define FIELD_SERVICE_WAVE_COEF 16078


/*获取历史正常波形
*类型：浮点型
*/
#define FIELD_SERVICE_OFF_NORMAL_WAVE 16079

/*文件内容
类型：buffer
*/
#define FIELD_SERVICE_FILE_CONTENT 16079

/*获取报警级别
*类型：整形
*/
#define FIELD_SERVICE_ALARM_LEVEL  16080

/* 波形数据头长度
*类型：整形
*/
#define FIELD_SERVICE_ON_SEND_WAVEHEAD_LENGTH 16081

/* 波形数据头长度
*类型：Buffer
*/
#define FIELD_SERVICE_ON_SEND_WAVEHEAD_BUFFER 16082

/*设备分级一级门限
*类型：浮点型
*/
#define FIELD_SERVICE_GRADE_ONE_THRESHOLD 16083

/*设备分级二级门限
*类型：浮点型
*/
#define FIELD_SERVICE_GRADE_TWO_THRESHOLD 16084

/*设备分级三级门限
*类型：浮点型
*/
#define FIELD_SERVICE_GRADE_THREE_THRESHOLD 16085

/**间隙电压-水平
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_CHAR_GAP_HOR   16086

/**间隙电压-垂直
  *类型：float
  *值范围：
  */
#define     FIELD_SERVICE_CHAR_GAP_VER      16087


/** 传感器灵敏度-水平
*类型: float
*/
#define     FIELD_SERVICE_SENSITIVITY_HOR     16088

/** 传感器灵敏度-垂直
*类型: float
*/
#define     FIELD_SERVICE_SENSITIVITY_VER    16089

/** 轴心位置类型
*类型: int
*/
#define     FIELD_SERVICE_AXIS_LOCATION_TYPE  16090

/** 轴直径
*类型: float
*/
#define     FIELD_SERVICE_AXIS_DIA             16091

/** 旋转方向
*类型: int
*/
#define     FIELD_SERVICE_ROTATE_DIRECTION     16092

/** 报警数据停止标志
*类型: int
*/
#define     FIELD_SERVICE_STOP_FLAG     18000

//////-----------------------------------/////////////
#endif
