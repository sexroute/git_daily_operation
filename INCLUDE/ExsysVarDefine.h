////ר��ϵͳʹ�õ�һЩ��������

#pragma  once


///����Ƶ�ʶ���
enum E_LeadFreq
{
	E_LeadFreq_JTless95 = 0,    /// ͬƵ��С��ͨƵֵ95%��
	E_LeadFreq_JTgreater95 = 1, /// ͬƵ������ͨƵֵ95%��
	E_LeadFreq_JF = 2,          ///��ȷ��Ƶ
	E_LeadFreq_JB = 3,          ///��ȷ��Ƶ
	E_LeadFreq_TZ = 4,          ///ͬƵ�ͺ�
	E_LeadFreq_BZ = 5,          ///��Ƶ�ͺ�
	E_LeadFreq_YY = 6,          ///���첽
	E_LeadFreq_TD_0_010   = 7,      ///�ص�Ƶ 0~0.1
	E_LeadFreq_TD_010_020 = 8,      ///�ص�Ƶ 0.1~0.2
	E_LeadFreq_TD_020_030 = 9,      ///�ص�Ƶ 0.2~0.3
	E_LeadFreq_FZ_030_040 = 10,      ///��Ƶ�ͺ� 0.3~0.4
	E_LeadFreq_FZ_040_045 = 11,      ///��Ƶ�ͺ� 0.4~0.45
	E_LeadFreq_FZ_045_050 = 12,      ///��Ƶ�ͺ� 0.45~0.5
	E_LeadFreq_WS = 13,         ///50Hz����
	E_LeadFreq_TG = 14,          ///�ظ�Ƶ
	E_LeadFreq_UNKNOW = 20      ///δ֪
};

///��������
enum E_PrecessionDirect
{
	E_PrecessionDirect_UNKNOWN = 0,   ///��ȷ��
	E_PrecessionDirect_Positive = 1,  ///������
	E_PrecessionDirect_Negative = 2   ///������
};

///����Ƶ�� 
enum E_AccomFreq
{
	E_AccomFreq_010_020X     = 0,      ///0.1~0.2��Ƶ
	E_AccomFreq_040_045X     = 1,      ///0.4����0.45��Ƶ
	E_AccomFreq_045_050X     = 2,      ///0.45����0.5��Ƶ
	E_AccomFreq_060_090      = 3,      ///0.6����0.9��Ƶ
	E_AccomFreq_NO           = 4,      ///������Ƶ��
	E_AccomFreq_JT           = 5,      ///ͬƵ
	E_AccomFreq_2X           = 6,      ///2��Ƶ
	E_AccomFreq_DP           = 7,      ///1/2X,1/3X,1/4X...��1X��Ƶ��ֵ
	E_AccomFreq_TG           = 8,      ///�ظ�Ƶ >10X��Ƶ�ɷ�
	E_AccomFreq_50HZ         = 9,      ///50Hz
	E_AccomFreq_BP_Peaks     = 10,     ///�������ֵ
	E_AccomFreq_UNKNOWN      = 11      ///δ֪
};

///�������
enum E_BearingType
{
	E_BearingType_Slide    = 0,     ///�������
	E_BearingType_Roll     = 1     ///�������
	
};

///����λ���������Ĺ�ϵ
enum E_MeasurePtandGearBoxRelation
{
    E_MeasurePtandGearBoxRelation_OutRollBearing  = 0,  ///Զ���ڳ�����Ĺ������λ��  
	E_MeasurePtandGearBoxRelation_NearRollBearing = 1, ///�����ڳ�����Ĺ������λ��
	E_MeasurePtandGearBoxRelation_ON = 2   ///�ڳ���������
	  
	  
};

///gSE���Ƶı仯���
enum E_gSETrendType
{
	E_gSETrendType_Incr    = 0,       //����
	E_gSETrendType_UnSure  = 1,       //��ȷ��
	E_gSETrendType_NoChange= 2        //����
};

enum E_BoolExType
{
	E_BoolExType_NO = 0,                   //��
	E_BoolExType_YES = 1,                  //��
	E_BoolExType_Unsure = 2,               //��ȷ��
	E_BoolExType_NoChann =10,               ///�޲�㣬�޷�����
	E_BoolExType_Error = -1,
};

enum E_PumpFanMachineType
{
	E_PumpFanMachineType_MT1 = 0,  //����ʽ ���򲻴��ⲿ������
	E_PumpFanMachineType_MT2 = 1,  //����-����ʽ���֣�����  
	E_PumpFanMachineType_MT3 = 2,  //����-����ʽ���֣�����
	E_PumpFanMachineType_MT4 = 3,  //������������
	E_PumpFanMachineType_MT5 = 4   //�����������
};

///����״̬ö��
enum E_RC_AlarmStatus
{
    E_RC_AlarmStatus_Normal    = 0,    ///����
	E_RC_AlarmStatus_AbNormal  = 1,    ///�쳣
	E_RC_AlarmStatus_UnKnown   = 2,    ///��ȷ��
	E_RC_AlarmStatus_NoChann   = 10,    ///�޲��
	E_RC_AlarmStatus_Error	   = -1,
};

//��ĳ��ֵ���бȽϵ�ö��
enum E_CompareValue_1
{
	E_CompareValue_1_Less = 0,            ///С��ĳ��ֵ
	E_CompareValue_1_GreaterEqual = 1,    ///���ڵ���ĳ��ֵ
	E_CompareValue_1_Unsure       = 2     ///��ȷ��
};

//��ĳ3��ֵ���бȽϵ�ö�ٽ��
enum E_CompareValue_3
{
	E_CompareValue_3_Less1 = 0,                ///С�ڵ�һ��ֵ
	E_CompareValue_3_GreaterEqual1Less2 = 1,   ///С�ڵڶ���ֵ���ڵ��ڵ�һ��ֵ
	E_CompareValue_3_GreaterEqual2Less3 = 2,   ///С�ڵ�����ֵ���ڵ��ڵڶ���ֵ
    E_CompareValue_3_GreaterEqual3      = 3,   ///���ڵ��ڵ�����ֵ
	E_CompareValue_3_Unsure             = 4    ///��ȷ��
};

///Ƶ�׵������ֲ�
enum E_RC_FreqEnergyDistribute
{
   E_RC_FreqEnergyH_Less30     = 0,    ///��Ƶ������С��30%
   E_RC_FreqEnergyH_More30     = 1,    ///��Ƶ���������ڵ���30%
   E_RC_FreqEnergyH_UnKnown    = 2     ///��Ƶ������ռ����δ֪
};

///ʱ���ε�һ�������������ڱ���ǰ����ȵ�״̬
enum E_RC_PerWaveRMS_Comp
{
    E_RC_PerWaveRMS_Comp_LessValue = 0,   ///һ�����������뱨��ǰ���С��ĳ���ٷֱ�
	E_RC_PerWaveRMS_Comp_MoreValue = 1,   ///һ�����������뱨��ǰ��ȴ��ڵ���ĳ���ٷֱ�
	E_RC_PerWaveRMS_Comp_UnKnown   = 2    ///һ�����������뱨��ǰ��ȣ�״̬δ֪
};

///�����˵ĳ�����ƫĥ��ͻ�伶��10s�ڵ�ͻ�䣩
enum E_RC_RodSink_BreakChange
{
   E_RC_RodSink_BreakChange_LessL           = 0,  ///С�ڵ��� 
   E_RC_RodSink_BreakChange_MoreL_LessM     = 1,  ///��������С�ڸ���
   E_RC_RodSink_BreakChange_MoreM_LessH     = 2,  ///��������С�ڸ���
   E_RC_RodSink_BreakChange_MoreH_LessRange = 3,  ///��������С�����̷�Χ
   E_RC_RodSink_BreakChange_MoreRange       = 4   ///�������̷�Χ
};