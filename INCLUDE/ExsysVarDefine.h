////专家系统使用的一些变量定义

#pragma  once


///主导频率定义
enum E_LeadFreq
{
	E_LeadFreq_JTless95 = 0,    /// 同频（小于通频值95%）
	E_LeadFreq_JTgreater95 = 1, /// 同频（大于通频值95%）
	E_LeadFreq_JF = 2,          ///精确分频
	E_LeadFreq_JB = 3,          ///精确倍频
	E_LeadFreq_TZ = 4,          ///同频滞后
	E_LeadFreq_BZ = 5,          ///倍频滞后
	E_LeadFreq_YY = 6,          ///亚异步
	E_LeadFreq_TD_0_010   = 7,      ///特低频 0~0.1
	E_LeadFreq_TD_010_020 = 8,      ///特低频 0.1~0.2
	E_LeadFreq_TD_020_030 = 9,      ///特低频 0.2~0.3
	E_LeadFreq_FZ_030_040 = 10,      ///分频滞后 0.3~0.4
	E_LeadFreq_FZ_040_045 = 11,      ///分频滞后 0.4~0.45
	E_LeadFreq_FZ_045_050 = 12,      ///分频滞后 0.45~0.5
	E_LeadFreq_WS = 13,         ///50Hz主导
	E_LeadFreq_TG = 14,          ///特高频
	E_LeadFreq_UNKNOW = 20      ///未知
};

///进动方向
enum E_PrecessionDirect
{
	E_PrecessionDirect_UNKNOWN = 0,   ///不确定
	E_PrecessionDirect_Positive = 1,  ///正进动
	E_PrecessionDirect_Negative = 2   ///负进动
};

///常伴频率 
enum E_AccomFreq
{
	E_AccomFreq_010_020X     = 0,      ///0.1~0.2倍频
	E_AccomFreq_040_045X     = 1,      ///0.4——0.45倍频
	E_AccomFreq_045_050X     = 2,      ///0.45——0.5倍频
	E_AccomFreq_060_090      = 3,      ///0.6——0.9倍频
	E_AccomFreq_NO           = 4,      ///无其他频率
	E_AccomFreq_JT           = 5,      ///同频
	E_AccomFreq_2X           = 6,      ///2倍频
	E_AccomFreq_DP           = 7,      ///1/2X,1/3X,1/4X...和1X倍频峰值
	E_AccomFreq_TG           = 8,      ///特高频 >10X高频成分
	E_AccomFreq_50HZ         = 9,      ///50Hz
	E_AccomFreq_BP_Peaks     = 10,     ///倍数多峰值
	E_AccomFreq_UNKNOWN      = 11      ///未知
};

///轴承类型
enum E_BearingType
{
	E_BearingType_Slide    = 0,     ///滑动轴承
	E_BearingType_Roll     = 1     ///滚动轴承
	
};

///监测点位置与齿轮箱的关系
enum E_MeasurePtandGearBoxRelation
{
    E_MeasurePtandGearBoxRelation_OutRollBearing  = 0,  ///远离在齿轮箱的滚动轴承位置  
	E_MeasurePtandGearBoxRelation_NearRollBearing = 1, ///靠近在齿轮箱的滚动轴承位置
	E_MeasurePtandGearBoxRelation_ON = 2   ///在齿轮箱体上
	  
	  
};

///gSE趋势的变化情况
enum E_gSETrendType
{
	E_gSETrendType_Incr    = 0,       //增长
	E_gSETrendType_UnSure  = 1,       //不确定
	E_gSETrendType_NoChange= 2        //不变
};

enum E_BoolExType
{
	E_BoolExType_NO = 0,                   //否
	E_BoolExType_YES = 1,                  //是
	E_BoolExType_Unsure = 2,               //不确定
	E_BoolExType_NoChann =10,               ///无测点，无法设置
	E_BoolExType_Error = -1,
};

enum E_PumpFanMachineType
{
	E_PumpFanMachineType_MT1 = 0,  //离心式 带或不带外部齿轮箱
	E_PumpFanMachineType_MT2 = 1,  //离心-整体式齿轮（轴向）  
	E_PumpFanMachineType_MT3 = 2,  //离心-整体式齿轮（径向）
	E_PumpFanMachineType_MT4 = 3,  //齿轮箱输入轴
	E_PumpFanMachineType_MT5 = 4   //齿轮箱输出轴
};

///报警状态枚举
enum E_RC_AlarmStatus
{
    E_RC_AlarmStatus_Normal    = 0,    ///正常
	E_RC_AlarmStatus_AbNormal  = 1,    ///异常
	E_RC_AlarmStatus_UnKnown   = 2,    ///不确定
	E_RC_AlarmStatus_NoChann   = 10,    ///无测点
	E_RC_AlarmStatus_Error	   = -1,
};

//和某个值进行比较的枚举
enum E_CompareValue_1
{
	E_CompareValue_1_Less = 0,            ///小于某个值
	E_CompareValue_1_GreaterEqual = 1,    ///大于等于某个值
	E_CompareValue_1_Unsure       = 2     ///不确定
};

//和某3个值进行比较的枚举结果
enum E_CompareValue_3
{
	E_CompareValue_3_Less1 = 0,                ///小于第一个值
	E_CompareValue_3_GreaterEqual1Less2 = 1,   ///小于第二个值大于等于第一个值
	E_CompareValue_3_GreaterEqual2Less3 = 2,   ///小于第三个值大于等于第二个值
    E_CompareValue_3_GreaterEqual3      = 3,   ///大于等于第三个值
	E_CompareValue_3_Unsure             = 4    ///不确定
};

///频谱的能量分布
enum E_RC_FreqEnergyDistribute
{
   E_RC_FreqEnergyH_Less30     = 0,    ///高频段能量小于30%
   E_RC_FreqEnergyH_More30     = 1,    ///高频段能量大于等于30%
   E_RC_FreqEnergyH_UnKnown    = 2     ///高频段能量占比例未知
};

///时域波形的一个周期能量大于报警前的相比的状态
enum E_RC_PerWaveRMS_Comp
{
    E_RC_PerWaveRMS_Comp_LessValue = 0,   ///一个周期能量与报警前相比小于某个百分比
	E_RC_PerWaveRMS_Comp_MoreValue = 1,   ///一个周期能量与报警前相比大于等于某个百分比
	E_RC_PerWaveRMS_Comp_UnKnown   = 2    ///一个周期能量与报警前相比，状态未知
};

///活塞杆的沉降或偏磨的突变级别（10s内的突变）
enum E_RC_RodSink_BreakChange
{
   E_RC_RodSink_BreakChange_LessL           = 0,  ///小于低限 
   E_RC_RodSink_BreakChange_MoreL_LessM     = 1,  ///超过中限小于高限
   E_RC_RodSink_BreakChange_MoreM_LessH     = 2,  ///超过中限小于高限
   E_RC_RodSink_BreakChange_MoreH_LessRange = 3,  ///超过高限小于量程范围
   E_RC_RodSink_BreakChange_MoreRange       = 4   ///超过量程范围
};