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
	/*	ҵ������:
		1. ������ٶȷ�ֵ(��Ч��ֵ)
		2. ������ٶȸ�Ƶ(��Ч��ֵ)
		3. ������ٶȵ�Ƶ(��Ч��ֵ)
		4. ������ٶȲ����Ͷ�
		5. �����ٶ���Чֵ
		6. ����ͨƵgSEֵ
		7. ������ٶ����ֵ
		8. ������ٶȸ�Ƶ���ֵ
		9. ������ٶȵ�Ƶ���ֵ
	*/
	if (fFref_ < 0.1)   
		fFref_ = 0.1;
	if (fDt_ < 0.000004) //�ɼ�����߲�����250K
		fDt_ = 0.000004;
	CNiReal64Vector nivf,nivfRs,nivftemp;		// ��������
	int nSize = int(1/fDt_/fFref_+0.5);	// �����ڵ���
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
	nivfEV_[0] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///��ֵ(��Ч��ֵ).
	nivfEV_[6] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///���ֵ
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
	nivfEV_[1] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///��ֵ(��Ч��ֵ)
	nivfEV_[7] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///���ֵ
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
	nivfEV_[2] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Effective);///��ֵ(��Ч��ֵ)
	nivfEV_[8] = SigMath.FeatureValue(nivftemp, nivftemp.GetSize(), E_FeatureValueType_Peak_Statistic);///���ֵ
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