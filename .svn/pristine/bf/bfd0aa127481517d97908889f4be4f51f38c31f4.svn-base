/*!\file CWindAndPumpAccComputer.cpp
*  \brief
*  \date 2009-8-15
*  \author 
*  \version
*/

#include "StdAfx.h"
#include <KDFilter.h>
#include <SigMath.h>
#include "CWindAndPumpAccComputer.h"

using namespace Computation;

void CWindAndPumpAccComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/*	业务流程:
		1. 计算加速度峰值(有效峰值)
		2. 计算加速度高频(有效峰值)
		3. 计算加速度低频(有效峰值)
		4. 计算加速度波形峭度
		5. 计算速度有效值
		6. 计算通频gSE值
		7. 计算加速度真峰值
		8. 计算加速度高频真峰值
		9. 计算加速度低频真峰值
	*/
	if (fFref_ < 0.1)   
		fFref_ = 0.1;
	if (fDt_ < 0.000004) //采集卡最高采样率250K
		fDt_ = 0.000004;
	CNiReal64Vector nivf,nivfRs,nivftemp;		// 波形数据
	int nSize = int(1/fDt_/fFref_+0.5);	// 单周期点数
	if (nSize < 1)
		nSize = 1;
	if (nivfWave_.GetSize() < nSize)
		nivf.SetSize(nivfWave_);
	else
		nivf.SetSize(nSize);
	nivfEV_.SetSize(IDX_EV_END);
	CSigMath SigMath;
	nivf.SetSize(nivfWave_);
	for (int i = 0; i < nivf.GetSize(); i++)
		nivf[i] = nivfWave_[i];
	// 1
	float fFp = 0;
	nivftemp.SetSize(nivf);
	nivftemp = nivf;
	nivfEV_[0] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///峰值(有效峰值).
	nivfEV_[6] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///真峰值
	// 2
	try
	{
		nivftemp.SetSize(nivf);
	    nivftemp = nivf;
		CNiMath::ButterworthHighPass(nivftemp, 1/fDt_, nivfParam_[IDX_IN_FC]);
	}
	catch (CNiException *pe)
	{
		CString str;
		str = pe->GetErrorMessage();
		pe->Delete();
		CHZLogManage::Error(str, _T("CWindAndPumpAccComputer"), _T("Compute"));
	}
	nivfEV_[1] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///峰值(有效峰值)
	nivfEV_[7] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///真峰值
	// 3
	try
	{
		nivftemp.SetSize(nivf);
	    nivftemp = nivf;
		CNiMath::ButterworthLowPass(nivftemp, 1/fDt_, nivfParam_[IDX_IN_FC]);
	}
	catch (CNiException *pe)
	{
		CString str;
		str = pe->GetErrorMessage();
		pe->Delete();
		CHZLogManage::Error(str, _T("CWindAndPumpAccComputer"), _T("Compute"));
	}
	nivfEV_[2] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///峰值(有效峰值)
	nivfEV_[8] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///真峰值
	// 4
	nivfEV_[3] = CKDFilter::GetKurtosis(nivf);
	// 5
	nivfRs.SetSize(nivf);
	SigMath.IntegratorEx(nivf, nivfRs, nivf.GetSize(), int(1/fDt_+0.5), 5, nivfParam_[IDX_IN_FC]);
	nivfEV_[4] = CKDFilter::GetRMS(nivfRs)*1000;
	// 6
	double gSE;
	SigMath.gSEAnalysis(nivf, nivf.GetSize(), int(1/fDt_+0.5), nivfParam_[IDX_IN_FC], gSE );
	nivfEV_[5] = gSE;
}