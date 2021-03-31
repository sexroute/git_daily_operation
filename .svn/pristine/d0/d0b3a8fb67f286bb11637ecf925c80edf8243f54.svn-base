/**@brief 程序中所有枚举类型和一些全局宏定义
  *GD_ 代表全局宏定义
  *GE_ 代表全局枚举定义
  *@author Marble
  */

#if!defined  AFX_DSE_DEFINE_MIDDLEWAVE_MARBLE_
#define AFX_DSE_DEFINE_MIDDLEWAVE_MARBLE_


/*! the maxim number of one vibrate channel acquire data.
#define  GD_DAQPOINTNUM        5120 */
/*! the maxim number of vibrate channel
#define  GD_VIBCHANNNUM        16 */
/** the maxim number of axis displace channal
#define  GD_AXISCHANNNUM       4 */
/** the maxim number of technics channal
#define  GD_TECHNICSCHANNNUM   8*/
/** the maxim number of plant
#define  GD_TECHNICSCHANNNUM   8*/

//与同步助手交互，计算分几次发送，因为最多一次可以发送100K，这里按照90K计算,1K=1024，所以最多可以发送92160字节
//因为波形数据为是2个字节，因该发送46080个数,2*46080
#define D_SENDDATA_BYTES   92160
#define D_SENDDATA_MAXNUMS 46080

/**中间上的历史数据每组波形点数以及默认向客户端发送的波形数据个数
  */
#define GD_MIDDLEWARE_WAVE_POINT    819200
#define GD_MIDDLEWARE_WAVE_POINT_RC 819200
#define GD_MIDDLEWARE_WAVE_POINT_WIND 819200
#define GD_MIDDLEWARE_WAVE_POINT_MAX 819200 

#define GD_FLOAT_SIZE  4

#define GD_CUSTOMPARAM_BUFFSIZE   50

///秒趋势，1分钟的保存记录条数
#define GE_SECONDTREND_PERMINUTE_SAVENUM   20

/*!400线*/
#define 	GD_400LINES    400

/*!800线*/
#define  	GD_800LINES    800

/*!1600线*/
#define 	GD_1600LINES   1600

/*!3200线*/
#define     GD_3200LINES   3200



///提纯轴心轨迹计算各个谐波所需要的参数
#define     GD_HIGHFCNUM   4   /*要提取的高次谐波的数目*/
#define     GD_LOWFCNUM    2   /*要提取的分频谐波的数目*/
#define     GD_HIGHFCTH    10   /*高次谐波的阈值，即超过这个阈值才提取该频率的谐波*/
#define     GD_LOWFCTH     5   /*分频谐波的阈值，即超过这个阈值才提取该频率的谐波*/

// 数据类型
enum DATATYPE
{
  GE_DATA_DISPLACEMENT,             ///< 位移
  GE_DATA_ACCELERATE,               ///< 加速度
  GE_DATA_VELOCITY,                 ///< 速度
  GE_DATA_TEMP,                     ///< 温度
  GE_DATA_PRESS,                    ///< 压力
  GE_DATA_LOAD,                     ///< 应力
  GE_DATA_FLUX,                     ///< 流量
  GE_DATA_IMPACT                    ///< 撞击次数
};


/** enumerate the character value type 枚举 */
enum CHARTYPE
{
	GE_ALLCHAR,        /*!通频特征*/
	GE_ONECHAR,        /*!一倍频特征*/
	GE_TWOCHAR,        /*!二倍频特征*/
	GE_THREECHAR,      /*!三倍频特征*/
	GE_HALFCHAR,       /*!一分频特征，即1/2倍频特征*/
    GE_RESCHAR,        /*!残振特征值*/
	GE_DCCHAR,         ///直流量
	GE_GAP,            ///间隙电压
	GE_PHASECHARONE,   ///一倍频相位
	GE_PHASECHARTWO,   ///二倍频相位
	GE_PHASECHARTHREE, ///三倍频相位
	GE_PHASECHARHALF,   ///分倍频相位
	GE_SAMPLENUM,  // 采样点数,12
	GE_SAMPLEFREQ, // 采样频率
	GE_REV // 转速
};

enum SERVER_TYPE
{
	GE_SRV_TYPE_SET,          ///装置级中间件服务器
	GE_SRV_TYPE_COMPANY,      ///公司级中间件服务器
	GE_SRV_TYPE_GROUP         ///集团级中间件服务器
};

enum PVTYPE
{
	GE_PVTYPE_PV,             ///P-V图
	GE_PVTYPE_Pa,             ///P-a图
	GE_PVTYPE_Pt              ///P-t图
};

#define GE_AXIALDISPLACE GE_PHASECHARTHREE

#define GE_MACHINETYPE_SINGLE_STAGE_PUMP                      (1001)
#define GE_MACHINETYPE_MULTI_STAGE_PUMP                       (1002)
#define GE_MACHINETYPE_PISTON_PUMP                            (1004) //活塞泵
#define GE_MACHINETYPE_DIAPHRAGM_PUMP                         (1005)
#define GE_MACHINETYPE_GEAR_PUMP                              (1007)
#define GE_MACHINETYPE_SCREW_PUMP                             (1008)
#define GE_MACHINETYPE_VANE_PUMP                              (1009)
#define GE_MACHINETYPE_LOBE_PUMP                              (1010)
#define GE_MACHINETYPE_ROOTS_PUMP                             (1012)
#define GE_MACHINETYPE_PLUNGER_PUMP                           (1013)//柱塞泵
#define GE_MACHINETYPE_LIQUID_RING_PUMP                       (1015)
#define GE_MACHINETYPE_CENTRIFUGAL_COMPRESSOR                 (1025)
#define GE_MACHINETYPE_PISTON_COMPRESSOR                      (1026)//活塞式压缩机
#define GE_MACHINETYPE_DIAPHRAGM_COMPRESSOR                   (1027)
#define GE_MACHINETYPE_SCREW_COMPRESSOR                       (1029)
#define GE_MACHINETYPE_AXIAL_COMPRESSOR                       (1031)
#define GE_MACHINETYPE_CENTRIFUGAL_FAN                        (1034)
#define GE_MACHINETYPE_ROOTS_BLOWER                           (1035)
#define GE_MACHINETYPE_AXIAL_FAN                              (1036)
#define GE_MACHINETYPE_BACK_PRESSURE_STEAM_TURBINE            (1042)
#define GE_MACHINETYPE_CONDENSING_STEAM_TURBINE               (1043)
#define GE_MACHINETYPE_EXTRACTION_CONDENSING_STEAM_TURBINE    (1044)
#define GE_MACHINETYPE_SINGLE_STAGE_GAS_TURBINE               (1046)
#define GE_MACHINETYPE_TWO_STAGE_GAS_TURBINE                  (1047)

///设备类型
enum MACHINETYPE
{
	GE_MACHINETYPE_COMPR=0,    ///离心压缩机compressor
	GE_MACHINETYPE_FAN,        ///风机fan
	GE_MACHINETYPE_TURB,       ///汽轮机turbine
	GE_MACHINETYPE_PUMP,       ///离心泵pump
	GE_MACHINETYPE_COMPR1,     ///轴流式压缩机或轴流+离心压缩机
	GE_MACHINETYPE_OTHERS,     ///其他
	GE_MACHINETYPE_RC,         ///往复式压缩机 reciprocating compressor
	GE_MACHINETYPE_KEYPUMP,    ///关键机泵
	GE_MACHINETYPE_WINDPEQ,    ///风电设备
	GE_MACHINETYPE_SMOKESTEAM,  ///烟汽轮机
	GE_MACHINETYPE_DISEL,       ///柴油机
	GE_MACHINETYPE_ICE,         ///内燃机/柴油机
	GE_MACHINETYPE_ALL          ///总概貌图
};
/**通道（测点）类型的枚举
  */
enum CHANNTYPE
{
	GE_ALLPROC          = 0,        /*!所有过程量测点*/
	GE_VIBCHANN         = 1,        /*!径向振动测点.*/
	GE_AXIALCHANN       = 2,        /*!轴向振动测点.axial displacement*/
	GE_PRESSCHANN       = 3,        /*!压力测点.*/
	GE_TEMPCHANN        = 4,        /*!温度测点.*/
	GE_FLUXCHANN        = 5,        /*!流量测点.*/
	GE_OTHERCHANN       = 6,        /*!其它.*/
	GE_IMPACTCHANN      = 7,        ///撞击通道
	GE_CURRENTCHANN     = 8,        ///电流测点
	GE_DYNPRESSCHANN    = 11,       ///动态压力信号
	GE_RODSINKCHANN     = 12,       ///活塞杆下沉量信号
	GE_DYNSTRESSCHANN   = 13,       /* !动态应力测点*/
	GE_AXISLOCATIONCHANN= 100       /*!轴心位置*/
};

enum DRIVETYPE
{
	GE_DRIVETYPE_INDUSTRIAL_TURBINE = 0,
	GE_DRIVETYPE_MOTOR_WITH_GEARBOX,
	GE_DRIVETYPE_MOTOR,
	GE_DRIVETYPE_FLUE_GAS_TRUBINE,
	GE_DRIVETYPE_OTHER
};

/*
 * 轴承类别
 */
enum BEARING_TYPE_EN
{
	GE_ROLLING_BEARING    = 1        , /// 滚动轴承"
	GE_SLIDING_BEARING_TILTING_PAD   , /// 可倾瓦滑动轴承
	GE_SLIDING_BEARING_ROUND_FLAPPER , /// 圆瓣式滑动轴承
};


/*
 * 齿轮类型
 */
enum GEAR_BOX_TYPE_EN
{
	GEAR_BOX_TYPE_COMMON = 0, /* 常规齿轮 */
	GEAR_BOX_TYPE_PLANETARY,  /* 行星齿轮 */
	GEAR_BOX_TYPE_NUM         /* 齿轮类型个数 */
};

/*
 * 行星齿轮驱动方式
 */
enum PLANETARY_GEAR_DRIVE_TYPE_EN
{
	GE_PLANETARY_GEAR_DRIVE_SUN    = 0, /// 太阳轮驱动
	GE_PLANETARY_GEAR_DRIVE_PLANET    , /// 行星轮驱动
	GE_PLANETARY_GEAR_DRIVE_GALAXY    , /// 银河系驱动
	GE_PLANETARY_GEAR_DRIVE_TYPE_NUM   /// 驱动方式总数
};

/*
 * 轴承表类别
 */
#define GE_INVALID_BEARING_TABLE -1
enum BEARING_TABLE_TYPE_EN
{
	GE_SYSTEM_DEFAULT_BEARING_TABLE = 0,
	GE_USER_DEFINED_BEARING_TABLE
};

#define GEAR_BOX_LEVEL_SUPPORTED   (5)

/*
 * 测点数据源 (Value)
 */
enum GE_DATA_SOURCE_EN
{
	GE_DATA_SOURCE_ONLINE   = 0, // 在线采集测点
	GE_DATA_SOURCE_OFFLINE  = 1, // 离线巡检测点
	GE_DATA_SOURCE_WIRELESS = 2, // 无线采集测点
};

/*
 * V4.7前设备数据源定义
 */
enum GE_DEPRECATED_MACHINE_DATA_SOURCE_EN
{
	GE_DEPRECATED_MACHINE_DATA_SOURCE_ONLINE   = 0, // 全是在线采集测点
	GE_DEPRECATED_MACHINE_DATA_SOURCE_OFFLINE  = 1, // 全是离线巡检测点
	GE_DEPRECATED_MACHINE_DATA_SOURCE_MIX      = 2, // 既有在线采集测点,又有离线采集测点
};

/*
 * 设备数据源 (BitSet)
 */
const unsigned int GE_MACHINE_DATA_SOURCE_ONLINE   = 1 << GE_DATA_SOURCE_ONLINE;
const unsigned int GE_MACHINE_DATA_SOURCE_OFFLINE  = 1 << GE_DATA_SOURCE_OFFLINE;
const unsigned int GE_MACHINE_DATA_SOURCE_WIRELESS = 1 << GE_DATA_SOURCE_WIRELESS;


///设备信息类型表中关键字定义
// GE_MI_
enum MACHINEINFOTYPE
{
	GE_MACHINETYPE                  = 1,         ///机器类型
	GE_DRIVERTYPE                   = 2,         ///机器的驱动类型
	GE_BEARINGTYPE                  = 3,         ///轴承类型
	GE_ROTORTYPE                    = 4,         ///转子类型
	GE_WORKINGREV                   = 5,         ///工作转速类型
	GE_CHANNTYPE                    = 6,         ///测点类型
	GE_SENSORTYPE                   = 7,         ///传感器类型
	GE_UNITTYPE                     = 8,         ///单位类型
	GE_VIBCHARTYPE                  = 9,
	GE_MI_GEARBOX_TYPE              = 10,       /// 齿轮箱类型
	GE_MI_MEASUREDIRECTION          = 11,       /// 测量方向
	GE_MI_PLANETARY_GEAR_DRIVE_TYPE = 12,       /// 行星齿轮驱动方式
	GE_MI_DATASOURCE                = 13,       /// 数据源, 类型为GE_DATA_SOURCE_EN
	GE_VIBCHARTYPE_A  = 100,    ///振动信号，采用加速度传感器，对应的特征值类型描述，个数12（0~11）
    GE_DYNCHARTYPE    = 111,    ///动态压力信号的特征值类型描述，个数10（0~9）
	GE_RODSINKCHARTYPE= 112,     ///下沉量信号的特征值类型描述,，个数10（0~9）
	GE_MI_WINDPUMPCHARTYPE_ACC = 113,    /// 风电机泵信号的特征值类型描述, 加速度信号
	GE_MI_WINDPUMPCHARTYPE_VEL = 114    /// 风电机泵信号的特征值类型描述, 加速度信号
};


enum SENSORTYPE
{
	GE_SENSORTYPE_VOTEX=1,      //电涡流传感器
	GE_SENSORTYPE_ACCELERATE=2, //加速度传感器
	GE_SENSORTYPE_SPEED=3,      //速度传感器
	GE_SENSORTYPE_PRESSOR=11,   //压力传感器
	GE_SENSORTYPE_FLUX=12,      //流量传感器
	GE_SENSORTYPE_TEMP=13,      //温度传感器
	GE_SENSORTYPE_IMPACT=14,    //撞击传感器
	GE_SENSORTYPE_OTHER=10      //其它
};

///启停车类型
enum E_SUD_TYPE
{
	GE_SUD_UNKNOWN=0,      ///未知
	GE_SUD_START=1,       ///开车
	GE_SUD_STOP=2        ///停车
};

/**历史趋势类型枚举
  */
enum TRENDTYPE
{
    GE_FIVEMINUTETREND,   /*!五分钟趋势*/
	GE_HALFHOURTREND,     /*!三十分钟趋势*/
	GE_HOURTREND,         /*!小时趋势*/
	GE_DAYTREND,          /*!天趋势*/
	GE_MONTHTREND,        /*!月趋势*/
	GE_QUARTERTREND,      /*!季度趋势*/
	GE_YEARTREND,          /*!年趋势*/
	GE_WEEKTREND           /*!周趋势*/
//	GE_ONEMINUTETREND,    /*!一分钟趋势*/
//	GE_TENMINUTETREND,    /*!十分钟时趋势*/
};

///测点通道的大类。是振动，过程量，动态测点
enum E_TBL_CHANNTYPE
{
	E_TBL_CHANNTYPE_VIB=0,         ///振动通道。“VIB”
	E_TBL_CHANNTYPE_PROC,          ///过程量通道。“PROC”
	E_TBL_CHANNTYPE_DYN            ///动态测点通道。“DYN”
};

enum E_TREND_TBL_TYPE
{
	GE_TREND_TBLTYPE_UNKNOWN=0,     ///未知表类型
	GE_TREND_TBLTYPE_SECOND=1,      ///秒趋势表类型,理论上1秒一条记录
	GE_TREND_TBLTYPE_HOUR,          ///小时趋势表类型,理论上1分钟一条记录
	GE_TREND_TBLTYPE_DAY,           ///天趋势表类型，理论上20分钟一条记录
	GE_TREND_TBLTYPE_MONTH,          ///月趋势表类型，理论上3小时一条记录
	GE_TREND_TBLTYPE_FAST            ///快变数据趋势表类型
};

/**@brief 用户级别枚举
  *       分厂级操作工只能察看所在分厂的设备运行状况；
  *       分厂级工程师可以察看所在分厂的设备运行状况，并且可以添加设备和修改参数，但是只限制在所在的分厂范围
  *       分厂级管理员可以操作所在分厂范围内的所有业务，添加删除用户和修改等级
  *       全厂级操作工察看全厂的设备运行状况；
  *       全厂级工程师可以察看所在全厂的设备运行状况，并且可以添加设备和修改参数
  *       全厂级管理员可以操作全厂范围内的所有业务，添加删除用户和修改等级
  */
enum USERLEVEL
{
	GE_LOCAL_OPERATOR=1,  /*!分厂级操作工*/
	GE_LOCAL_ENGINEER,    /*!分厂级工程师*/
	GE_LOCAL_ADMIN,       /*!分厂级管理员*/
	GE_WHOLE_OPERATOR,    /*!全厂级浏览*/
	GE_WHOLE_ENGINEER,    /*!全厂级工程师*/
	GE_WHOLE_ADMIN,       /*!全厂级管理员*/
	GE_WHOLE_EXPERT,      /*!全厂级专家*/
	GE_GROUP_OPERATOR,    /*!集团级浏览*/
	GE_GROUP_ENGINEER,    /*!集团级工程师*/
	GE_GROUP_ADMIN,       /*!集团级管理员*/
	GE_GROUP_EXPERT,      /*!集团级专家*/
	GE_EXPERT,            /*!专家*/
	GE_ADMIN              /*!管理人员*/
};

//风电机泵自定义特征值类型
enum E_KEYPUMPWIND_USERCHARTYPE
{
	GE_VF_HALF=0,		   ///速度功率谱0.5X倍频
	GE_VF_ONEX,            ///速度功率谱1X倍频
	GE_VF_TWO,             ///速度功率谱2X倍频
	GE_VF_THREE,           ///速度功率谱3X倍频
	GE_VF_FOUR,            ///速度功率谱4X倍频
    GE_VF_FIVE,            ///速度功率谱5X倍频
	GE_VF_BLADE,           ///速度功率谱叶片通过频率
	GE_USER_AENVFREQ1,     ///啮合频率
	GE_USER_AENVFREQ2,     ///输出轴转频
	GE_ENVSPECCF1,         ///内圈特征频率
	GE_ENVSPECCF2,         ///外圈特征频率
	GE_ENVSPECCF3,         ///保持架特征频率
	GE_ENVSPECCF4,         ///滚动体特征频率
	GE_GEARBOXSHAFTCF1,    ///齿轮箱I轴转频
    GE_GEARBOXSHAFTCF2,    ///齿轮箱I轴转频
	GE_GEARBOXSHAFTCF3,    ///齿轮箱I轴转频
	GE_GEARBOXSHAFTCF4     ///齿轮箱I轴转频
};

/*
  异常数据类别
*/
enum GE_ABNORMAL_DATA_CATEGORY
{
	GE_DATA_BOUNDS_EXCEEDED = 0,    /* 越界数据 */
	GE_DATA_TREND_FLUCTUATING,      /* 趋势波动 */
	GE_DATA_VALUE_UNRESONABLE,       /* 不合理数值 */
	GE_DATA_VALUE_NO_ERROR           /*未见异常*/
};


/*
 数据库连接状态
*/
enum GE_DB_STATUS
{
	GE_DB_FUNC_OK = 0, /*正常*/
	GE_DB_FUNC_ERROR   /*异常*/
};

/*
 数采卡工作状况
*/
enum GE_DAQ_CARD_STATUS
{
	GE_DAQ_CARD_ACQUIRING = 0, /* 连接，正常采集*/
	GE_DAQ_CONNECT_NOT_ACQUIRE,   /*连接，未采集*/
	GE_DAQ_NOT_CONNECTED   /*未连接*/
};


/*! 测量方向*/
enum GE_MEASURE_DIRECTION
{
	MEASURE_DIRECTION_HORIZONTAL	= 0,		//! 水平
	MEASURE_DIRECTION_VERTICAL		= 1,        //! 垂直
	MEASURE_DIRECTION_AXIAL			= 2,        //! 轴向
	MEASURE_DIRECTION_RADIAL		= 3,        //! 径向
	MEASURE_DIRECTION_ALL			= 4,		//! 全部方向
};

/*!报警确认类型*/
enum GE_ALARM_CONFIRM_TYPE
{
	ALARM_CONFIRM_TYPE_NONE			= 0,       //没有确认
	ALARM_CONFIRM_TYPE_ONCE			= 1,	   //仅确认一次
	ALARM_CONFIRM_TYPE_INTERVAL		= 2,	   //确认指定的时间周期
	ALARM_CONFIRM_TYPE_FOREVER		= 3,	   //永久确认，暂不支持，通过组态设置
};
/*!趋势数据搜素类型*/
enum GE_DATA_SEARCH_MODE 
{
	NO_SEARCH    = 0x00000000,						   //不搜索，仅查询指定表
	SEARCH_TREND = 0x00000001,                          //搜索趋势表
	SEARCH_ALARM = 0x00000002,                          //搜索报警数据表
	SEARCH_SUD	 = 0x00000004,                           //搜索启停车数据表
};

enum PROBLEM_TYPE {
	unknown,
	NeedOil,
	bearing_light,
	bearing_Mid,
	bearing_Heavy,
	bearing_swing_Heavy,
	bearing_swing_light,
	evacuated_light,
	evacuated_Heavy,
	loose_light,
	loose_Heavy,
	signal_error,
	blocking,
	bearing_sleeve,
	excited_by_noise,
};

/*!数据库角色*/
const char gc_cRole[]={"dbo"};

/*!数据库字段的常量定义，在修改记录时候使用*/
const char gc_cCenterMw_IP[]={"IP"};
const char gc_cCenterMw_Port[]={"PORT"};
const char gc_cSend_Interval[]={"SEND_INTERVAL"};
const char gc_cIf_Send[]={"IF_SEND"};
const char gc_cSud_Send_Interval[]={"SUD_SEND_INTERVAL"};
const char gc_cAlarm_Send_Interval[]={"ALARM_SEND_INTERVAL"};
const char gc_cTm_IP[]={"TM_IP"};
const char gc_cTm_Port[]={"TM_PORT"};
const char gc_cChannNo[]={"CHANNNO"};
const char gc_cChannID[]={"CHANNID"};
const char gc_cChannIDA[]={"CHANNID_ALIAS"};
const char gc_cSetID[]={"SETID"};
const char gc_cPlantID[]={"PLANTID"};
const char gc_cPlantNo[]={"PLANTNO"};
const char gc_cDisplay[]={"DISPLAY"};
const char gc_cErpCode[]={"ERP_CODE"};
const char gc_cCylinderNo[]={"CYLINDERNO"};
const char gc_cRecipComprParam[]={"RECIPCOMPRPARAM"};
const char gc_cVibChann_Num[]={"VIBCHANN_NUM"};
const char gc_cDynChann_Num[]={"DYNCHANN_NUM"};
const char gc_cCylinder_Num[]={"CYLINDER_NUM"};
const char gc_cAxisChann_Num[]={"AXISCHANN_NUM"};
const char gc_cProcChann_Num[]={"PROCCHANN_NUM"};
const char gc_cFluxChann_Num[]={"FLUXCHANN_NUM"};
const char gc_cTempChann_Num[]={"TEMPCHANN_NUM"};
const char gc_cPressChann_Num[]={"PRESSCHANN_NUM"};
const char gc_cOtherChann_Num[]={"OTHERCHANN_NUM"};
const char gc_cMachine_Type[]={"MACHINE_TYPE"};
const char gc_cCritSpeed_1st[]={"CRITSPEED_1ST"};//一阶临界转速
const char gc_cCritSpeed_2nd[]={"CRITSPEED_2ND"};//二阶临界转速
const char gc_cGraphBuff[]={"GRAPHBUFF"};
const char gc_cGraphBuffBytes[]={"GRAPHBUFFBYTES"};//自制概貌图的字节数
const char gc_cManuFacturer[]={"MANUFACTURER"};//制造厂商
const char gc_cFirstRun_T[]={"FIRSTRUN_T"};//首次投运时间
const char gc_cVersion[]={"VERSION"}; //版本号
const char gc_cType[]={"TYPE"};
const char gc_cCoor_X[]={"COOR_X"};
const char gc_cCoor_Y[]={"COOR_Y"};
const char gc_cDC_X[]={"DC_X"};
const char gc_cDC_Y[]={"DC_Y"};
const char gc_cGap_X[]={"GAP_X"};
const char gc_cGap_Y[]={"GAP_Y"};

const char gc_cDriver_Type[]={"DRIVER_TYPE"};
const char gc_cConn_Type[]={"CONN_TYPE"};
const char gc_cBear_Type[]={"BEAR_TYPE"};
const char gc_cRotate_Type[]={"ROTATE_TYPE"};
const char gc_cSeal_Type[]={"SEAL_TYPE"};

///设备工作在第一临界转速还是第二临界转速
const char gc_cRev_Type[]={"REV_TYPE"};
const char gc_cSampleFreq[]={"SAMPLE_FREQ"};
const char gc_cSampleNum[]={"SAMPLE_NUM"};
const char gc_cSensitivity[]={"SENSITIVITY"};
const char gc_cSensitivity1[]={"SENSITIVITY1"};
const char gc_cAlarmLow[]={"ALARM_LOW"};
const char gc_cAlarmHigh[]={"ALARM_HIGH"};
//const char gc_cCheckAlarm[]={"CHECK_ALARM"};
const char gc_cAlarmParam[]={"ALARM_PARAM"};
const char gc_cGapVoltage[]={"GAPVOLTAGE"};
const char gc_cAngle[]="ANGLE";
const char gc_cKeyIF[]={"KEY_IF"};
const char gc_cRev[]={"REV"};
const char gc_cRev2[]={"REV2"};
const char gc_cRev3[]={"REV3"};
const char gc_cRev4[]={"REV4"};
const char gc_cRev5[]={"REV5"};
const char gc_cGraphType[]={"GRAPH_TYPE"};
const char gc_cList[]={"LIST"};
const char gc_cUnit[]={"UNIT"};
const char gc_cSensorType[]={"SENSOR_TYPE"};
///测点转速属于设备第一转速还是第二转速
const char gc_cRevType[]={"REVTYPE"};
const char gc_cFactory_Name[]={"FACTORY_NAME"};
const char gc_cFactory_Alias[]={"FACTORY_ALIAS"};
const char gc_cCompany[]={"COMPANY"};
const char gc_cCompanyAlias[]={"COMPANY_ALIAS"};
const char gc_cCountry[] = {"COUNTRY"};
const char gc_cGroup[]={"GROUP"};
const char gc_cUserName[]={"USERNAME"};
const char gc_cPassWord[]={"PASSWORD"};
const char gc_cRefreshTime[]={"REFRESHTIME"};
const char gc_cUserLevel[]={"USERLEVEL"};
const char gc_cBrowseSet[]={"BROWSESET"};
const char gc_cDaq_IP[]={"DAQ_IP"};
const char gc_cSuffix[]={"SUFFIX"};

const char gc_cID[]={"ID"};
const char gc_cEventID[]={"EVENT_ID"};
const char gc_cEventNAME[]={"EVENT_NAME"};
const char gc_cAll[]={"ALL"};
const char gc_cOne[]={"ONE"};
const char gc_cTwo[]={"TWO"};
const char gc_cThree[]={"THREE"};
const char gc_cHalf[]={"HALF"};
const char gc_cRes[]={"RES"};//残振
const char gc_cTime[]={"DATETIME"};
const char gc_cMicroSecond[]={"MICROSECOND"};
const char gc_cWave[]={"WAVE"};
const char gc_cChannType[]={"CHANN_TYPE"};
const char gc_cProcValue[]={"PROCVALUE"};
const char gc_cTrendType[]={"TREND_TYPE"};
const char gc_cSynFlag[]={"SYN_FLAG"};
const char gc_cName[]={"name"};

const char gc_cPhaseHalf[]={"HALFPHASE"};
const char gc_cPhaseOne[]={"ONEPHASE"};
const char gc_cPhaseTwo[]={"TWOPHASE"};
const char gc_cPhaseThree[]={"THREEPHASE"};
const char gc_cDC[]={"DC"};
const char gc_cGap[]={"GAP"};
const char gc_cDot[]={","};

const char gc_cEventTable[]={"EVENT_RECORD"};
const char gc_cEvent_Time[]={"EVENT_TIME"};
const char gc_cEvent_Content[]={"EVENT_CONTENT"};
const char gc_cDefault_Status[]={"DEFAULT_STATUS"};
const char gc_cResult_Feedback[]={"RESULT_FEEDBACK"};

//const char gc_cDatetime_Start[]={"DATETIME_START"};
const char gc_cStart_T[]={"START_T"};
const char gc_cEnd_T[]={"END_T"};
const char gc_cStop_Flag[]={"STOP_FLAG"};
const char gc_cVerChann[]={"VERCHANN"};
const char gc_cHorChann[]={"HORCHANN"};
const char gc_cLocation[]={"LOCATION"};
const char gc_cLocationType[]={"LOCATION_TYPE"};
const char gc_cAxis_Dia[]={"AXIS_DIA"};
const char gc_cRotate_Direction[]={"ROTATE_DIRECTION"};

const char gc_cData[]={"DATA"};
const char gc_cData_Len[]={"DATA_LEN"};
const char gc_cThreshold[]={"THRESHOLD"};
const char gc_cThreshold_ID[]={"THRESHOLD_ID"};

//阈值的个数,并非阈值结构体的尺寸
const char gc_cThreshold_Len[]={"THRESHOLD_LEN"};
const char gc_cThreshold_ID_New[]={"THRESHOLD_ID_NEW"};
const char gc_cThreshold_Type[]={"THRESHOLD_TYPE"};
//学习参数结构体的个数，并非结构体的尺寸
const char gc_cLearnParam_Len[]={"LEARN_PARAM_LEN"};
const char gc_cLearnParam[]={"LEARN_PARAM"};
const char gc_cAlarm_Type[]={"ALARM_TYPE"};

const char gc_cGroupTable[]={"groups"};
const char gc_cSettingTable[]={"settings"};
const char gc_cCompanyTable[]={"company"};
const char gc_cFactoryTable[]={"factory"};
const char gc_cSetTable[]={"settable"};
const char gc_cChannTable[]={"channel"};
const char gc_cMachineTable[]={"machine"};
const char gc_cUserTable[]={"DSE_USER"};
const char gc_cPlantAxisTable[]={"PLANT_AXIS"};
const char gc_cDB_ID[]={"DB_ID"};
const char gc_cDB_Name[]={"DB_NAME"};
const char gc_cParent_ID[]={"PARENT_ID"};
const char gc_cChild_Flag[]={"CHILD_FLAG"};
const char gc_cSortNO[]={"SORTNO"};

//设备信息类型参数
const char gc_cMachInfoTypeTable[]={"machineinfotype"};
const char gc_cInfoType[]={"INFOTYPE"};
const char gc_cComment[]={"COMMENT"};
const char gc_cComments[]={"COMMENTS"};
const char gc_cIndex[]={"INDEX"};
const char gc_cInfoTypeDetail[]={"INFOTYPEDETAIL"};
const char gc_cPeriod_Start_Index[]={"PERIOD_START_INDEX"};
const char gc_cPeriod_End_Index[]={"PERIOD_END_INDEX"};
const char gc_cFiled[]={"FIELD"};
const char gc_cFileld1[]={"FIELD1"};

/*
 * 自定义参数信息
 */
const char gc_cTempletID[]={"TEMPLET_ID"};
const char gc_cAlarm_TempletID[]={"ALARM_TEMPLET_ID"};
const char gc_cParam_Comment[]={"PARAM_COMMENT"};
const char gc_cData_Type[] = {"DATA_TYPE"};
const char gc_cUnit_Type[] = {"UNIT_TYPE"};
const char gc_cSignal_Ana_Type[]={"SIGNAL_ANA_TYPE"};
const char gc_cSeg_Start_NO[]={"SEG_START_NO"};
const char gc_cSeg_Num[]={"SEG_NUM"};
const char gc_cData_Start_Value[]={"DATA_START_VALUE"};
const char gc_cData_End_Value[]={"DATA_END_VALUE"};
const char gc_cChar_Type[]={"CHAR_TYPE"};
const char gc_cSegName[]={"SEG_NAME"};

// 放大倍数
const char gc_cChann_Data_Type[] = {"DATATYPE"};
const char gc_cDC_AMP[] = {"DC_AMP"};
const char gc_cAC_AMP[] = {"AC_AMP"};

// 预处理类型，如和分，微分
const char gc_cPreProc_Type[] = {"PREPROC_TYPE"};
const char gc_cPreProc_K[] = {"PREPROC_K"};
const char gc_cPreProc_B[] = {"PREPROC_B"};
// 包络谱开始结束值
const char gc_cBand_Start_Value[]   = {"BAND_START_VALUE"};
const char gc_cBand_End_Value[]     = {"BAND_END_VALUE"};
const char gc_cAlarm_Param_Comment[]={"ALARM_PARAM_COMMENT"};
const char gc_cJudge_Type[]={"JUDGE_TYPE"};
const char gc_cAlarm_Start_SegNO[]={"ALARM_START_SEGNO"};
const char gc_cIF_Alarm[]={"IF_ALARM"};
const char gc_cAlarm_Group[]={"ALARM_GROUP"};
const char gc_cAlarm_LTH[]={"ALARM_LTH"};
const char gc_cAlarm_HTH[]={"ALARM_HTH"};

const char gc_cTemplet_Table[]           = {"templet"};
const char gc_cCustom_Param_Table[]      = {"customparam"};
const char gc_cAlarm_Templet_Table[]     = {"alarmtemplet"};
const char gc_cAlarm_Param_Table[]       = {"alarmparam"};
const char gc_cChann_Alarm_Param_Table[] = {"channelalarmparam"};
// 自定义模板的TEMPLET_ID
const char gc_cTemplateID_SINK[]      = {"SYS_SINK_TEMPLATE"};
const char gc_cTemplateID_SINK_Desc[] = {"活塞杆柱状图沉降模板"};
const char gc_cTemplateID_VIB[]       = {"SYS_VIB_TEMPLATE"};
const char gc_cTemplateID_VIB_Desc[]  = {"振动监测柱状图模板"};
const char gc_cTemplateID_WINDPUMP[]       = {"SYS_WINDPUMP"};
const char gc_cTemplateID_WINDPUMP_Desc[]  = {"风电机泵默认模板"};
// 测点表增加测量方向
const char gc_cMesurement_Direction[]   = {"MEASURE_DIRECTION"};
// 部位表/位置表
const char gc_cSection_Table[]         = {"sections"};    // 部位表
const char gc_cLocation_Table[]        = {"locations"};   // 位置表
const char gc_cBearing_Table[]         = {"bearings"};    // 轴承表
const char gc_cCustom_Bearing_Table[]  = {"custom_bearings"}; // 自定义轴承表
const char gc_cGear_Box_Table[]        = {"gear_boxes"};  // 齿轮箱表
const char gc_cSection_IDX[]           = {"SECTION_IDX"};     // 部位索引
const char gc_cSection_Name[]          = {"SECTION_NAME"};    // 部位名
const char gc_cSection_Pic[]           = {"SECTION_PIC"};      // 图片类型
const char gc_cLocation_IDX[]          = {"LOCATION_IDX"};    // 位置索引
const char gc_cLocation_NO[]           = {"LOCATION_NO"};     // 位置号
const char gc_cLocation_Name[]         = {"LOCATION_NAME"};   // 位置名
const char gc_cFunction_Type[]         = {"FUNCTION_TYPE"};   // 功能类型, 组态自动添加的为-1
const char gc_cSection_Type[]          = {"SECTION_TYPE"};    // 部位类型, 组态自动添加的为-1
const char gc_cPrev_Sec_IDX[]          = {"PREV_SEC_IDX"};    // 前一部位索引
const char gc_cPrev_Loc_IDX[]          = {"PREV_LOC_IDX"};    // 前一位置索引

const char gc_cShaft_Number[]          = {"SHAFT_NUMBER"} ;   // 轴号
const char gc_cBearingTbl_Type[]       = {"BEARING_TABLE"};   // 轴承表类别
// 轴承表
const char gc_cBearing_IDX[]      = {"BEARING_IDX"};      // 轴承索引
const char gc_cBearing_Type[]     = {"BEARING_TYPE"};     // 轴承类型: 滑动轴承, 滚动轴承
const char gc_cBearing_Name[]     = {"BEARING_NAME"};     // 轴承名称
const char gc_cBearing_Id[]		  = {"BEARING_ID"};       // 轴承ID
const char gc_cBearing_Code[]     = {"BEARING_CODE"};     // 轴承代码(预留)
const char gc_cRows_Num[]         = {"ROWS_NUM"};         // 滚珠排数
const char gc_cElements_Num[]     = {"ELEMENTS_NUM"};     // 滚珠数
const char gc_cPith_Diameter[]    = {"PITCH_DIAMETER"};   // 节径
const char gc_cElement_Diameter[] = {"ELEMENT_DIAMETER"}; // 滚珠直径
const char gc_cContact_Angle[]    = {"CONTACT_ANGLE"};    // 压力角
const char gc_cHousing_Diameter[] = {"HOUSING_DIAMETER"}; // 外壳直径
const char gc_cBore_Diameter[]    = {"BORE_DIAMETER"};    // 内径
// 齿轮箱
const char gc_cGearBox_IDX      [] = {"GEAR_BOX_IDX"};    // 齿轮箱索引
const char gc_cGearBox_Name     [] = {"GEAR_BOX_NAME"};   // 齿轮箱描述
const char gc_cGearBox_Level    [] = {"GEAR_BOX_LEVEL"};  // 齿轮箱级数
const char gc_cGearBox_Type     [] = {"GEAR_BOX_TYPE"};   // 齿轮箱类别: 常规齿轮,行星齿轮
const char gc_cPlanet_Num       [] = {"PLANET_NUM"};      // 行星轮个数
const char gc_cGear_Num_Sun     [] = {"Z_SUN"};           // 太阳轮齿数
const char gc_cGear_Num_Planet  [] = {"Z_PLANET"};        // 行星轮齿数
const char gc_cGear_Num_Galaxy  [] = {"Z_GALAXY"};        // 银河系齿数
const char gc_cGear_Num_12      [] = {"Z12"};             // 齿数12
const char gc_cGear_Num_21      [] = {"Z21"};             // 齿数21
const char gc_cGear_Num_22      [] = {"Z22"};             // 齿数22
const char gc_cGear_Num_31      [] = {"Z31"};             // 齿数31
const char gc_cGear_Num_32      [] = {"Z32"};             // 齿数32
const char gc_cGear_Num_41      [] = {"Z41"};             // 齿数41
const char gc_cGear_Num_42      [] = {"Z42"};             // 齿数42
const char gc_cGear_Num_51      [] = {"Z51"};             // 齿数51
const char gc_cGear_Num_52      [] = {"Z52"};             // 齿数52
const char gc_cGear_Num_61      [] = {"Z61"};             // 齿数61

const char gc_cParam1           [] = {"Param1"};          // 预留参数, int
const char gc_cParam2           [] = {"Param2"};          // 预留参数, float
const char gc_cParam3           [] = {"Param3"};          // 预留参数, varchar(20)

// 同步标识
const char gc_cIs_Sync  []         = {"IS_SYNC"};         // 同步标识

const char gc_cData_Source[]       = {"DATA_SOURCE"};     // 数据源, 测点表中使用
const char gc_cDateTime[]          = {"DATETIME"};
const char gc_cCustom_Param[]      = {"CUSTOM_PARAM"};

const char gc_balanceTable[]          = {"DynamicBalance"};				//动平衡表名
const char gc_balanceTable_pt1[]	  = {"MEASURE_PT_1_NO"};			// 动平衡测点1测点号字段名
const char gc_balanceTable_pt2[]	  = {"MEASURE_PT_2_NO"};			// 动平衡测点2测点号字段名
const char gc_balancetable_data_pt_1[]	  = {"DATA_PT_1"};				// 动平衡测点1字段名
const char gc_balancetable_data_pt_2[]	  = {"DATA_PT_2"};				// 动平衡测点1字段名
const char gc_balancetable_datetime[]	  = {"DATETIME"};				// 动平衡测点1字段名
const char gc_balancetable_type[]	  = {"TYPE"};						// 动平衡测点1字段名
const char gc_balancetable_data_len_pt_1[]	  = {"DATA_PT_1_LEN"};      // 动平衡测点1数据长度
const char gc_balancetable_data_len_pt_2[]	  = {"DATA_PT_2_LEN"};      // 动平衡测点2数据长度


const char gc_cMwType[]       = {"MWTYPE"}; 
const char gc_cTm_MwType[]    = {"TM_MWTYPE"}; 


const char gc_cLongitude[]    = {"LONGITUDE"};     // 经度
const char gc_cLatitude[]	  = {"LATITUDE"};      // 纬度

const char gc_cVisible[]     = {"VISIBLE"};       // 可见/隐藏
const char gc_cContry[]     = {"COUNTRY"};       // 国家

const char gc_cKey[]       = {"KEY"};       // Key
const char gc_cValue[]     = {"VALUE"};     // Value

const char gc_cNum[]={"NUM"};
const char gc_cUsers[]={"USERS"};
const char gc_cMachines[]={"MACHINES"};

//Settings
const char gc_cShowGroup[]   = {"ShowGroup"};
const char gc_cShowCompany[] = {"ShowCompany"};
const char gc_cShowFactory[] = {"ShowFactory"};
const char gc_cShowSet[]     = {"ShowSet"};
const char gc_cShowPlant[]   = {"ShowPlant"};

const char gc_cAlarmConfirmTableName [] ={"ALARM_CONFIRM"};
const char gc_cIsCurrent[] = {"IS_CURRENT"};
const char gc_cDATACODE[] ={"DATACODE"};
const char gc_cRemark[]={"REMARK"};
const char gc_cClientIP[]={"CLIENT_IP"};

#define gc_cOfflineTable_Prefix    "TREND_OFFLINE"
#define gc_cTimeFormat             "%Y-%m-%d %H:%M:%S"

const char gc_cSUD_EVENT_Prefix[] = {"SUD_EVENT_"};
const char gc_cSUD_LOG_Prefix[] = {"SUD_LOG_"};

const char gc_cWorkStatus[] = {"WORK_STATUS"};
const char gc_cAlarmLevel[] = {"ALARM_LEVEL"};
#endif //AFX_DSE_DEFINE_MIDDLEWAVE_MARBLE_
