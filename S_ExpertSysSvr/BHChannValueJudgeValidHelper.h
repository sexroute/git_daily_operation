#pragma once
#ifndef _BH_JUDGE_DATA_HELPER_
#define _BH_JUDGE_DATA_HELPER_

#include <MiddleWare.h>
#include <float.h>

typedef bool (*UnResonableDataPredict)(float);

#define UNRESONABLE_PREDICT_INIT(OB) map<int, UnResonableDataPredict> OB
#define UNRESONABLE_PREDICT_BIND(OB, charIndex, predict) \
	do { \
	OB.insert(make_pair<int, UnResonableDataPredict>(charIndex, predict)); \
	} while(0)

#define ABNORMAL_ERROR(error) \
	do{ \
	HZLOG_ERROR(error); \
	AbnormalData_SetLastError(error); \
	}while(0)




class CBHChannValueJudgeValidHelper
{

	/*
	中间件判断异常数据的逻辑
	1.	越界数据：
	①	int型：绝对值大于1073741824（即230）
	②	float型：转换成字符串后（%f）带有非数字的字符（非0…9、+、-、.）
	2.	趋势波动的转速：
	取相邻两次采集采到的转速：r(t-1)、r(t)，若|r(t)-r(t-1)|>1000，则判断转速波动过大
	3.	不合理数据：
	各特征值的合理取值范围：
	①	撞击次数：0~16
	②	温度：>-273
	③	压力测点：
	平均压力：>0
	机械效率：0~100
	峭度：>0
	歪度：无范围
	单缸指示功率：>0
	吸气容积效率：0~100
	排气容积效率：0~100
	压缩指数：>0
	膨胀系数：>0
	实际压力比：>=1
	④	往复机械加速度测点：
	峰值：>0
	峭度：>0
	歪度：无范围
	⑤	旋转机械振动测点：
	通频幅值：>0
	各倍频幅值：>0
	各倍频相位：0~360
	残振：>0
	间隙电压：-24~-2
	直流量：-24~-2
	轴位移：无范围
	⑥	往复机械速度测点：
	有效值：>0
	峭度：>0
	歪度：无范围
	⑦	活塞杆应力测点：
	峰值活塞杆压力：>0
	峰值活塞杆张力：大于零
	反转角：0~360
	⑧	活塞杆位置测点：
	沉降平均值：无范围
	峰峰值：>0
	活塞气缸余隙间隙：>0
	1、2倍频幅值：>0
	直流量：-24~-2
	间隙电压：-24~-2
	*/
public:
	static bool AbnormalData_IsUnResonable(
		const map<int, UnResonableDataPredict> &mapCharPredict, int charIndex, float charVal);

	static string g_sAbnormalDataErrorMessage;

	static void  AbnormalData_SetLastError(const CString & error);

	static string AbnormalData_GetLastError();

	static bool  AbnormalData_IsOverflow(double value);

	static bool  AbnormalData_IsOverflow(int value);

	static bool  AbnormalData_IsUnResonable(float value);

	static bool  AbnormalData_IsAmpUnResonable(float value);

	static bool  AbnormalData_IsPhaseUnResonable(float value);

	static bool  AbnormalData_IsKurtosisUnResonable(float value);

	static bool  AbnormalData_IsSkewnessUnResonable(float value);

	static bool  AbnormalData_IsgSEUnResonable(float value);

	static bool  AbnormalData_IsDCUnResonable(float value);

	static bool  AbnormalData_IsGapUnResonable(float value);

	static bool  AbnormalData_IsEfficiencyUnResonable(float value);

	static bool  AbnormalData_IsPreesureRatioUnResonable(float value);

	static bool  AbnormalData_IsImpactUnResonable(float value);

	static bool  AbnormalData_IsTempUnResonable(float value);

	static  bool AbnormalData_IsAngleUnResonable(float value);

	static bool  AbnormalData_IsCharOverflow(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static bool  AbnormalData_IsCharOverflow(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static bool  AbnormalData_IsCharResonable(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static bool AbnormalData_IsCharResonable(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(
		const map<int, UnResonableDataPredict> & mapCharPredict,
		S_VibCharValue * pVibChar,
		S_ChannInfo * pChann,
		CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(
		const map<int, UnResonableDataPredict> & mapCharPredict,
		S_DynCharValue * pDynChar,
		S_ChannInfo * pChann,
		CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(
		const map<int, UnResonableDataPredict> & mapCharPredict,
		const map<int, string> & mapCharDesc,
		S_DynCharValue * pDynChar,
		S_ChannInfo * pChann,
		CPlantInfoBase * pPlant);

	static int  AbnormalData_RotaryPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_ReciprocalPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_WindElectroPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsDynStressCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_ReciprocalPlant_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsCharValid(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

	static int  AbnormalData_IsRevValid(
		float * pfLastRev, float * pfCurrRev, int nRevNum,
		CPlantInfoBase  *  pPlant);


private:
	CBHChannValueJudgeValidHelper(void){};
	virtual ~CBHChannValueJudgeValidHelper(void){};
};
#endif
