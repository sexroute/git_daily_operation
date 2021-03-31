/*!\file LoadComputer.h
*  \brief
*  \date 2009-6-21
*  \author 
*  \version
*/

#include "StdAfx.h"
#include <KDFilter.h>
#include "LoadComputer.h"
using namespace Computation;

void CLoadComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/*	业务流程：
		1. 截取一个周期的数据
		2. 计算最大值和最小值
		3. 计算峰值活塞杆压力，峰值活塞杆张力
		4. 活塞杆反转角
		5. 计算周期起始、终止索引
	*/
	//1
	float fmin,  //最小值
		  fmax;  //最大值
    unsigned int   nminIndex, //最小值索引 
		  nmaxIndex; //最大值索引
    int number = 0;	      
	CNiReal32Vector nivf = nivfWave_;		// 波形数据
	nivfEV_.SetSize((unsigned int)IDX_EV_END);
	int nSize = int(1/fDt_/fFref_+0.5);	// 单周期点数
	if (nSize < 1)
		nSize = 1;
	nivf.Resize(nSize);
	//2
	nivf.MinMax(fmin, nminIndex, fmax, nmaxIndex);
	nivfEV_[IDX_EV_DYN_0] = (fmax > 0) ?  fmax : 0; //峰值活塞杆张力
	//3
	nivfEV_[IDX_EV_DYN_1] = (fmin < 0) ?  fabs(fmin) : 0;//峰值活塞杆压力
	//4
	if (nivfEV_[IDX_EV_DYN_0]>0.1||nivfEV_[IDX_EV_DYN_1]>0.1)
	{
		for (int i = 0; i < nSize; i++)
		{
			if (nivf[i]>0)
				++number;
		}
		nivfEV_[IDX_EV_DYN_2] = float(number)/nSize*360;//活塞杆反转角
	}
	else
	{
		nivfEV_[IDX_EV_DYN_2] = 0;
	}
	//5
	nivfEV_[IDX_EV_START_IDX] = 0;
	nivfEV_[IDX_EV_END_IDX] = nSize-1;

}