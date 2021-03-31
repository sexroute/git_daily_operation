/*!\file CReciprocationVecComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include "CReciprocationVecComputer.h"


using namespace Computation;

void CReciprocationVecComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/*	ҵ������(����)��
	1. ��ȡһ�����ڵ�����
	2. ������Чֵ
	3. �����Ͷ�
	4. �������
	*/
	if (fFref_ < 0.1)		// У��Ƶ��ֵ
		fFref_ = 0.1;
	if (fDt_ < 0.000004)
		fDt_ = 0.000004;
	int nSize = int(1/fDt_/fFref_+0.5);	// �����ڵ���
	if (nSize < 1)
		nSize = 1;
	CNiReal32Vector nivf;		// ��������
	if (nivfWave_.GetSize() < nSize)
		nivf.SetSize(nivfWave_);
	else
		nivf.SetSize(nSize);
	nivfEV_.SetSize(IDX_EV_END);
	// 1
	for (int i = 0; i < nivf.GetSize(); i++)
		nivf[i] = nivfWave_[i];
	// 2
	nivfEV_[IDX_EV_ALL] = CKDFilter::GetRMS(nivf);
	// 3
	nivfEV_[IDX_EV_ONE] = CKDFilter::GetKurtosis(nivf);
	// 4
	nivfEV_[IDX_EV_TWO] = CKDFilter::GetSkewness(nivf);
}
