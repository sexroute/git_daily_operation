/*!\file DynPressComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include "DynPressComputer.h"
using namespace Computation;




void CDynPressComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/* 业务流程：
		1. 截取一个周期的波形
		2. 计算平均压力
		3. 计算活塞行程
		4. 计算活塞扫过的体积
		5. 计算单缸指示功率
		6. 计算机械效率
		7. 计算峭度
		8. 计算歪度
		9. 计算其它示功图特征值：吸气容积效率、排气容积效率、压缩指数、膨胀系数、实际压力比、相对余隙容积(不用)
		10. 计算周期起始、终止索引
	*/
	if (fFref_ < 0.1)   
		fFref_ = 0.1;
	if (fDt_<0.000004) //采集卡最高采样率250K
		fDt_ = 0.000004;
	CNiReal32Vector nivfVol,	// 活塞扫过的体积
					nivfPress,nivfPressTemp;	// 压力
	nivfEV_.SetSize((int)IDX_EV_END);
	// 1
	int nSize = int(1/fDt_/fFref_+0.5);	// 单周期点数
	if (nSize < 1)
		nSize = 1;
	int nCut = int(CKDFilter::GetRegularDegree(nivfParam_[IDX_IN_REL_DEG])/360*nSize+0.5);	// 理论上截掉的点数
	int nCut1 = (nSize+nCut<nivfWave_.GetSize()) ? nCut : 0;	// 实际需要截掉的点数
	if (nSize < nivfWave_.GetSize())
	{
		nivfPress.SetSize(nSize);
		for (int i = 0; i < nivfPress.GetSize(); i++)
			nivfPress[i] = nivfWave_[i+nCut1];
	}
	else
		nivfPress = nivfWave_;
	nivfVol.SetSize(nivfPress);
	// 2
	double dMean = 0;
	CNiMath::Mean(nivfPress, dMean);
	nivfEV_[IDX_EV_DYN_0] = float(dMean);
	// 3
	for (int i = 0; i < nivfVol.GetSize(); i++)		// 计算活塞行程(米)
		nivfVol[i] = CKDFilter::GetPistonMoveLength(nivfParam_[IDX_IN_ROD_LEN], nivfParam_[IDX_IN_HUB_RDS], fDt_*i, fFref_);
	// 4
	nivfVol.Scale(nivfParam_[IDX_IN_PIST_AREA], nivfParam_[IDX_IN_CLR_VOL]*0.001);
	// 5
	nivfPressTemp = nivfPress;
	nivfPressTemp.Scale(1.0/nivfParam_[IDX_IN_UNITRATIO]);// 将压力单位转化为KPa
	nivfEV_[IDX_EV_DYN_4] = CKDFilter::GetSingleCylinderPower(nivfPressTemp, nivfVol, fFref_);
	// 6
	nivfEV_[IDX_EV_DYN_1] = CKDFilter::GetMachineEfficiency(nivfEV_[IDX_EV_DYN_4], nivfParam_[IDX_IN_TOTAL_POW]);
	// 7
	nivfEV_[IDX_EV_DYN_8] = CKDFilter::GetKurtosis(nivfPress);
	// 8
	nivfEV_[IDX_EV_DYN_9] = CKDFilter::GetSkewness(nivfPress);
	// 9
	CKDFilter::ComputeDynValues(nivfPressTemp, nivfVol, nivfParam_[IDX_IN_IN_PRESS]*1000,\
		nivfParam_[IDX_IN_OUT_PRESS]*1000, nivfParam_[IDX_IN_CLR_VOL]/1000, nivfEV_[IDX_EV_DYN_2],\
		nivfEV_[IDX_EV_DYN_3], nivfEV_[IDX_EV_DYN_5], nivfEV_[IDX_EV_DYN_6], nivfEV_[IDX_EV_DYN_7]);
	// 10
	nivfEV_[IDX_EV_START_IDX] = 0;
	nivfEV_[IDX_EV_END_IDX] = nSize-1;
}
