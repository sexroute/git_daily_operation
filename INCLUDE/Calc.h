//////////////////////////////////////////////////////////////////////
/**
 * @file Calc.h
 * @brief 信号处理算法动态链接库Calc.dll的接口声明文件
 * @author Marble
 *
 * $Name: 1.1.1.1 $
 * $Locker$
 * $RCSfile: Calc.h,v $ 
 * $Revision: 1.1.1.1 $
 * $Source: /backup/code/middleware2/Analysis/Calc.h,v $
 * $State: Exp $
 * Modified by: $Author: marble $
 * Created:     2004/04/12
 * Last modify: $Date: 2006/06/15 00:36:18 $
*/
//////////////////////////////////////////////////////////////////////
#ifndef _PDE_CACL_
#define _PDE_CACL_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _PDE_MARBLE_DLLCOMPILE_
#define DLL_INCLUDE  extern "C" __declspec(dllexport)
#else
#define DLL_INCLUDE  extern "C" __declspec(dllimport)
#pragma comment(lib,"Analysis.lib")
#endif //_PDE_MARBLE_DLLCOMPILE_

//-----------------------------------------------------------------------//
/** \struct  S_BearingPar
  * \brief  滚动轴承参数结构体。计算滚动轴承故障（通过）频率时，需要的参数；
  */
struct S_BearingPar
{
	float   fWorkFreq;             /*! 滚动轴承的工作频率，单位：Hz.*/        
	float   fRollingDiam;          /*! 滚动轴承滚动体（滚珠）的直径，单位：mm.*/
	float   fContackAngle;         /*! 滚动轴承的压力角（又称接触角），单位：DEG.*/
	int     nNumofRolling;         /*! 滚动轴承滚动体的个数.*/
    float   fPitchDiam;            /*! 滚动轴承的节径，单位：mm.*/
};

//-----------------------------------------------------------------------//
/** \struct  S_BearingDefectFreq
  * \brief  滚动轴承故障（通过）频率结构体。包括滚动轴承的各种故障频率。
  */
struct S_BearingDefectFreq
{
	float   fInnerFreq;             /*! 滚动轴承内圈通过频率，单位：Hz.*/        
	float   fRollingFreq;           /*! 滚动轴承滚动体（滚珠）的通过频率，单位：Hz.*/
	float   fOuterFreq;             /*! 滚动轴承外圈通过频率，单位：Hz.*/
	float   fCageFreq;              /*! 滚动轴承保持架的通过频率，单位：Hz.*/
};

/** 频谱分析类型的枚举. */
enum
{
	EA_PEAK_SPEC=0,     /*峰值谱*/
	EA_POWER_SPEC,    /*功率谱*/
	EA_RMS_SPEC,      /*有效值谱*/
	EA_PEAK_PEAK_SPEC  /*峰峰值谱*/ 
}ESpectralType;


//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn CharValue
  *@brief 计算特征值 0为有效值；1为峭度
  *@param pwave 时间序列数据（即波形数据）
  *@param n     时间序列的长度，序列长度为2的指数
  *@param type  采用的计算特征值类型
  *@return      计算得到的时间序列特征值
  */
float CalcCharValue(float *pwave,int n,int type=0); 

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn SpectralMeasureMent
  *@brief 频谱分析
  *@param pwave 时间序列数据（即波形数据）
  *@param n     时间序列的长度，序列长度为2的指数
  *@param pout  输出频谱分析的结果，长度是时间序列长度的一般，即n/2
  *@param type  计算何种频谱 0:幅值谱;1:功率谱
  */
void SpectralMeasureMent(float *pwave,int n,float *pout,int type=EA_PEAK_SPEC); 

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn Evenlope
  *@brief  对时间序列数据进行包络后，并进行频谱分析
  *@param  pwave  时间序列数据（即波形数据）
  *@param  n      时间序列长度，序列长度为2的指数
  *@param  pout   包络后时间序列的频谱分析结果输出，长度为时间序列长度的一般，即n/2
  *@param  type   计算何种频谱 0:幅值谱;1:功率谱
  */
void EvenlopeSpectral(float *pwave,int n,float *pout,int type=0);

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn VelocityWave
  *@brief  对加速度波形进行积分并输出
  *@param  pwave  时间序列数据（即波形数据）
  *@param  n      时间序列长度，序列长度为2的指数
  *@param  pout   积分后的速度波形结果输出，长度和时间序列同样长
  */
void VelocityWave(float *pwave,int n,float *pout);

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn CalcBearingDefectFreq
  *@brief  计算滚动轴承的故障（通过）频率
  *@param  bearingpar 滚动轴承各种参数
  *@return 计算所得的滚动轴承故障频率
  */
S_BearingDefectFreq  CalcBearingDefectFreq(S_BearingPar bearingpar);

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn ApCalcBearingDefectFreq
  *@brief  简化近似计算滚动轴承的故障（通过）频率，注意：没有滚动体的通过频率
  *@param  fworkfreq  滚动轴承的工作频率
  *@param  rollingnum 滚动轴承滚动体个数
  *@return 计算所得的滚动轴承通过频率，注意：此时滚动体的通过频率没有。
  */
S_BearingDefectFreq  ApCalcBearingDefectFreq(float fworkfreq,int rollingnum);

//-----------------------------------------------------------------------//
DLL_INCLUDE
/**@fn RcepSpectral
  *@brief  calculate cepstrum
  *@param  pwave  time series data(eq.wave data)
  *@param  n  the length of time-series data
  *@param  pout  the result of Cepstrum analysis ,the length of pout is n.
  */
void  CepstrumAnalysis(float *pwave,int n,float *pout);
//-----------------------------------------------------------------------//

#endif  //_PDE_CACL_