#include "StdAfx.h"
#include ".\impactcomputer.h"
#include <KDFilter.h>
using namespace Computation;

void CImpactComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/* 业务流程：
		1. 将一个周期的波形分为16段，每段如果有超过设定阈值的值，则记为1次撞击，即撞击次数在0~16之间
	*/
	const int c_nBlks = 16;
	CNiReal64Vector nivdData;
	nivdData = nivfWave_;
	nivfEV_.SetSize(1);
	float fTh = fabs(nivfParam_[IDX_IN_THRESHOLD]);	// 阈值
	int nSize = int(1/fDt_/fFref_+0.5);	// 单周期点数
	if (nSize < 1)
		nSize = 1;
	int nImpact = 0;	// 撞击次数
	// 1
	if (nivdData.GetSize() > nSize)
		nivdData.Resize(nSize);
	int nBlkSize = nivdData.GetSize() / c_nBlks;
	for (int i = 0; i < c_nBlks; i++)
	{
		for (int j = 0; j < nBlkSize; j++)
		{
			if (fabs(nivdData[i*nBlkSize+j]) > fTh)
			{
				++nImpact;
				break;
			}
		}
	}
	nivfEV_[0] = nImpact;
}
