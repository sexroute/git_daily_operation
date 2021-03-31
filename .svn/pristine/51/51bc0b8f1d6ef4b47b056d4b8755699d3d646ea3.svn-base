/*!\file CRotationAxisDisComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include "CRotationAxisDisComputer.h"
using namespace Computation;

void CRotationAxisDisComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/*	业务流程：
		1. 按振动通道计算特征值
		2. 计算轴位移
		3. 轴位移写进3倍频相位
	*/
	// 1
	CRotationRadialDisComputer::Compute(nivfWave_, nivfFFT_, fDt_, fFref_, nivfParam_, nivfEV_);
	// 2
	float fAxis = (nivfParam_[IDX_IN_DC] - nivfParam_[IDX_IN_GAP]) * nivfParam_[IDX_IN_SSK] * nivfParam_[IDX_IN_UNITRATIO];
	// 3
	nivfEV_[IDX_EV_THREE_PHASE] = fAxis;
}
