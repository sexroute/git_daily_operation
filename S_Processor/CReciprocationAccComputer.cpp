/*!\file CReciprocationAccComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include <SigMath.h>
#include "CReciprocationAccComputer.h"
using namespace Computation;

void CReciprocationAccComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/*	ҵ�����̣�
		1. ��ȡһ�����ڵ�����
		2. �����ֵ(��Ч��ֵ)
		3. �����Ͷ�
		4. �������
		5. �����ֵ(���ֵ)
	*/
	CSigMath SigMath;
	if (fFref_ < 0.1)   
		fFref_ = 0.1;
	if (fDt_<0.000004) //�ɼ�����߲�����250K
		fDt_ = 0.000004;
	int nSize = int(1/fDt_/fFref_+0.5);	// �����ڵ���
	if (nSize < 1)
		nSize = 1;
	CNiReal32Vector nivf;		// ��������
	CNiReal64Vector nivd;		// ��������
	if (nivfWave_.GetSize() < nSize)
		nivf.SetSize(nivfWave_);
	else
		nivf.SetSize(nSize);
	nivfEV_.SetSize(IDX_EV_END);
	// 1
	for (int i = 0; i < nivf.GetSize(); i++)
		nivf[i] = nivfWave_[i];
	nivd = nivf;
	// 2
	nivfEV_[IDX_EV_ALL] = SigMath.FeatureValue(nivd, nivd.GetSize(), E_FeatureValueType_Effective);///��ֵ(��Ч��ֵ)
	// 3
	nivfEV_[IDX_EV_ONE] = CKDFilter::GetKurtosis(nivf);
	// 4
	nivfEV_[IDX_EV_TWO] = CKDFilter::GetSkewness(nivf);
	// 5
	nivfEV_[IDX_EV_THREE] = SigMath.FeatureValue(nivd, nivd.GetSize(), E_FeatureValueType_Peak_Statistic);///���ֵ
}
