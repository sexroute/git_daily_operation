#pragma once
#include "math.h"
/*
	中间件自学习和诊断算法模块
	made by DSE.
*/
#ifdef HZ_DIAGNOSE_MW_DLL_INTERNALS
#undef HZ_DIAGNOSE_MW_DLL_INTERNALS
#define HZ_DIAGNOSE_MW_DLL_INTERNALS _declspec(dllexport)					//导出标识符
#else
#undef HZ_DIAGNOSE_MW_DLL_INTERNALS
#define HZ_DIAGNOSE_MW_DLL_INTERNALS _declspec(dllimport)					//导入标识符
#pragma  comment(lib,"Diagnose_MW.lib")
#endif

////////////////////////////////////////////////////////////////////////////////////////
struct S_AxisCenterPosition_CalcInput
{
	double   d_Radius ;				  /*轴半径,单位:mm*/
	double   d_GapVolt_V;			  /*垂直初始间隙电压，单位：mm.*/
	double   d_GapVolt_H;			  /*水平初始间隙电压，单位：mm.*/
	double   d_DC_V;					  /*垂直当前直流量，单位：mm.*/        
	double   d_DC_H;					  /*水平当前直流量，单位：mm.*/
	int      i_circumgyrate_aspect;    /*旋转方向，单位：无.0-逆时针 1-顺时针*/
	int      i_fixing_mode;            /*安装方式，单位：无.2-(45，45)  0-(0-90 水平在左) 1-(0-90 水平在右)*/
};
struct S_AxisCenterPosition
{
	double   AxisCenterPosition_X ;              /*水平坐标 单位：um*/
	double   AxisCenterPosition_Y;               /*垂直坐标 单位：um.*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//轴心位置缓变门限，建议去掉轴心位置快变
struct S_AxisCenterPositionThreshold__CalcInput
{
	double *  AxisCenterPosition_X ;              /*水平坐标数组 单位：um*/
	double *  AxisCenterPosition_Y;               /*垂直坐标数组 单位：um.*/
	int i_length;                                 /*数组长度*/
	float f_hope;                                 /*期望*/
	float f_zoomCorfficient;                      /*放大系数*/
	
};
struct S_AxisCenterPosition_threshold
{
	double   d_center_X ;                          /*圆心 X 单位：um*/
	double   d_center_Y ;                          /*圆心 Y 单位：um*/
	double   d_Radius;								/*半径 R 单位：um.*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//趋势诊断
struct S_TrendEnent_CalcInput
{
	double *  pTrendArrChar ;              /*时间序列 单位：um*/
	int		  i_length;								/*时间序列长度 单位：*/
	int       i_trendType;							/*趋势类型 1 小时 2 天 3 月 4 季度*/
	double    d_threshold;						   /*斜率门限 单位：*/
};
struct S_TrendEnent_result
{
	double *  d_k ;								/*时间序列斜率 单位：*/
	int		  i_trendEventResult;				/*趋势事件诊断结果 0 = 无趋势发展 1 趋势缓慢增长 2 趋势较快增长 3 趋势缓慢降低 4 趋势较快降低   */
	int      errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//幅值缓变门限计算
struct S_charThreshold__CalcInput
{
	double *  pChar ;							/* 特征值数组  单位：*/
	int i_length;                                 /*数组长度*/
	float f_hope;                                 /*期望*/
	float f_zoomCorfficient;                      /*放大系数*/

};
struct S_charThreshold
{
	double   d_charThreshold_up ;                          /*上门限 单位：*/
	double   d_charThreshold_down ;                          /*下门限 单位：*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
//相位缓变门限计算  保证输入相位在0 -- 360度内
struct S_phaseThreshold__CalcInput
{
	double *  pPhase ;							/* 相位数组  单位：度*/
	int i_length;                                 /*数组长度*/
	float f_hope;                                 /*期望*/
	float f_zoomCorfficient;                      /*放大系数*/

};
//上下门限定义 1。上门限在[0，180]之间时 下门限在[-180 ， 上门限]
//             2。上门限在[180，360]之间时 下门限在[0 ， 上门限]
struct S_phaseThreshold
{
	double   d_phaseThreshold_up ;                          /*上门限 单位：*/
	double   d_phaseThreshold_down ;                          /*下门限 单位：*/
	bool     b_InOneFour;									/*门限是否在1 4 象限*/
	int     errorCode;
};
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
//相位方差趋势事件门限自学习,也用于相位方差计算  保证相位方差在 0 -- 360度内,输入相位在0 -- 360度内
struct S_phaseVarThreshold__CalcInput
{
	double *  pPhase ;							  /* 相位数组  单位：度*/
	int i_length;                                 /*数组长度*/
	float f_hope;                                 /*期望*/
	float f_zoomCorfficient;                      /*放大系数*/

};
struct S_phaseVarResult
{
	double   d_Var;                          /*上门限 单位：*/
	bool     b_InOneFour;					/*门限是否在1 4 象限*/
	int     errorCode;
};


class HZ_DIAGNOSE_MW_DLL_INTERNALS CAnalizeDiagnose_MW
{
	
public:
	CAnalizeDiagnose_MW(void);
	~CAnalizeDiagnose_MW(void);

	//轴心轨迹计算 2006 9 10
	//返回值 bool 如果计算出错或超出范围 false
	bool getAxisCenterPosition(S_AxisCenterPosition_CalcInput arg_CalcInput,S_AxisCenterPosition & outArg_AxisCenterPosition);

	//轴心位置缓变门限 
	//返回值 bool 如果计算出错false
	bool calcAxisCenterPosition_threshold(S_AxisCenterPositionThreshold__CalcInput inArg_AxisCenterPositionthreshold,S_AxisCenterPosition_threshold & outArg_AxisCenterPosition_threshold);

	//幅值缓变门限计算 
	//返回值 bool 如果计算出错false
	bool calcCharSlowChange_threshold(S_charThreshold__CalcInput inArg,S_charThreshold & outArg);

	//相位缓变门限计算 
	//返回值 bool 如果计算出错false
	bool calcPhaseSlowChange_threshold(S_phaseThreshold__CalcInput inArg,S_phaseThreshold & outArg);

	//趋势事件诊断
	//返回值 bool 如果计算出错或超出范围 false
	bool calcTrendEnent(S_TrendEnent_CalcInput inArg_TrendEnent,S_TrendEnent_result & outArg_S_TrendEnent_result);

	//相位方差趋势事件计算与诊断 标准差
	//返回值 bool 如果计算出错或超出范围 false
	bool calcPhaseVarTrendEnent(S_phaseVarThreshold__CalcInput inArg,S_phaseVarResult & outArg);




	//math
	double GetDistance(const double x1,const double y1,const double x2,const double y2);

};
