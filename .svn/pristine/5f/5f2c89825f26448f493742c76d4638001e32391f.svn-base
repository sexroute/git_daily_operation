/**
 * @file Calc1.h
 * @brief 动态链接库中私有函数的声明
 * @author Marble
 *
 * $Name: 1.1.1.1 $
 * $Locker$
 * $RCSfile: Calc1.h,v $ 
 * $Revision: 1.1.1.1 $
 * $Source: /backup/code/middleware2/Analysis/Calc1.h,v $
 * $State: Exp $
 * Modified by: $Author: marble $
 * Created:     2004/04/23
 * Last modify: $2005/11/28$
*/

#ifndef _PDE_CACL1_
#define _PDE_CACL1_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _PDE_MARBLE_DLLCOMPILE_

#include "Calc.h"

/** 进行FFT之前对时间序列加窗，定义窗的枚举变量.*/
enum
{
    WINDOW_NO,       /*!< not window.*/
	WINDOW_HANNING,  /*!< hanning.*/
	WINDOW_HAMMING,  /*!< hamming.*/
	WINDOW_BLACKMAN  /*!< blackman.*/
};
/** 计算特征参数值类型.*/
enum
{
	PDE_CHAR_RMS,       /*!有效值.*/
    PDE_CHAR_KUR        /*!峭度.*/
};
/*! 关于pi的宏定义.*/
#define PI 3.14159265  

/*! 对时间序列进行加窗。直接对指针指向的时间序列进行修改的。*/
void  WindowedWave(float *pwave,            /*! 输入的时间序列，直接对这个序列进行加窗.*/
		           int n,                   /*!时间序列长度.*/
				   int type=WINDOW_HANNING  /*! 所加窗的类型.*/
				   );

/*! 进行基2的fft（傅立叶变换）和ifft（逆傅立叶变换），序列的长度必须为2的指数倍.如果nflag为0就是fft，1则是ifft。*/
void  Fft2_DIT(float *pwave,  /*!进行变换的时间序列实部；并且计算完后就变为变换结果的实部，即实部输出.*/
		       float *pi,     /*!进行变换的时间序列虚部，并且计算完后就变为变换结果的虚部，即虚部输出.*/
		       int n,         /*!时间序列长度.*/ 
		       int nflag=0   /*!变换的类型，nflag=0表示fft；nflag=1表示ifft.*/
			   );

/*! 进行hilbert变换，序列的长度必须为2的指数倍.*/
void  Hilbert(float *preal,  /*!进行hilbert变换时间序列；并且计算完后就变为变换结果实部，即结果实部输出.*/
		      float *pimag,  /*!进行hilbert变换时间序列的虚部（全部为0）；并且计算完后就变为变换结果的虚部，即结果虚部输出.*/
			  int n          /*!时间序列长度.*/
		     );
/*! 计算基于2对数，Log2(n).，返回值为结果*/
int CalcLog2(int n);

/*!计算均方根RMS(又称为有效值)，返回值为RMS*/
float CalcRMS(float *pwave, /*!输入的时间序列*/
			  int n         /*!输入时间序列的长度*/
			  );

/*!计算时间序列的峭度值，返回值为峭度值*/
float CalcKurtosis(float *pwave, /*!输入的时间序列*/
			       int n         /*!输入时间序列的长度*/
			       );
#endif //_PDE_CACL1_