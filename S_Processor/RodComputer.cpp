/*!\file RodComputer.cpp
*  \brief
*  \date 2009-2-16
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include <SigMath.h>
#include "RodComputer.h"
using namespace Computation;

void CRodComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/* ҵ�����̣�
		1. ��ȡһ�����ڵĲ���
		2. ����ȥֱ������ƽ��ֵ
		3. ��������г�
		4. ����ƽ������ϵ��
		5. �������ƽ��ֵ
		6. ��������ֵ����ֵ����Ч��ֵ��
		7. ����ʣ���϶
		8. ����1��Ƶ��2��Ƶ��ֵ
		9. �������ֵ����Ч��ֵ��
		10. ����������ʼ����ֹ����
	*/
	CSigMath SigMath;
	if (fFref_ < 0.1)   
		fFref_ = 0.1;
	if (fDt_<0.000004) //�ɼ�����߲�����250K
		fDt_ = 0.000004;
	CNiReal32Vector nivfRod = nivfWave_;		// ����
	CNiReal64Vector nivdRod;	
	CNiReal32Vector nivfPath;		// �����г�
	nivfEV_.SetSize((unsigned int)IDX_EV_END);
	// 1
	int nSize = int(1/fDt_/fFref_+0.5);		// �����ڵ���
	if (nSize < 1)
		nSize = 1;
	nivfRod.Resize(nSize);
	nivdRod = nivfRod;
	nivfPath.SetSize(nivfRod);
	// 2
	float fAvg = CKDFilter::RemoveDirectCurrent(nivfRod);
	// 3
	for (int j = 0; j < nivfPath.GetSize(); j++)		// ��������г�(��)
		nivfPath[j] = CKDFilter::GetPistonMoveLength(nivfParam_[IDX_IN_ROD_LEN], nivfParam_[IDX_IN_HUB_RDS], fDt_*j, fFref_);
	// 4
	float fK = 0;		// ƽ������ϵ��
	for (j = 0; j < nivfPath.GetSize(); j++)
	{
		float fDist = nivfParam_[IDX_IN_SENS_POS] - nivfPath[j] - nivfParam_[IDX_IN_PIS_THICK] / 2;
		if ((nivfParam_[IDX_IN_PISROD_LEN] - fDist)<1e-4)
			fK += 0;
		else
			fK += nivfParam_[IDX_IN_PISROD_LEN] / (nivfParam_[IDX_IN_PISROD_LEN] - fDist);
	}
	fK /= nivfPath.GetSize();
	// 5
	nivfEV_[IDX_EV_DYN_0] = fAvg * fK;
	nivfEV_[IDX_EV_DYN_6] = fAvg;
	nivfEV_[IDX_EV_DYN_7] = fK;
	// 6
	nivfEV_[IDX_EV_DYN_1] = SigMath.FeatureValue(nivdRod, nivdRod.GetSize(), E_FeatureValueType_PeakPeak_Statistic);///����ֵ
	nivfEV_[IDX_EV_DYN_2] = SigMath.FeatureValue(nivdRod, nivdRod.GetSize(), E_FeatureValueType_Effective);///��ֵ(��Ч��ֵ)
	// 7
	nivfEV_[IDX_EV_DYN_3] = (nivfParam_[IDX_IN_PIS_CW_GAP] * 1e6 - fabs(nivfEV_[IDX_EV_DYN_0]/nivfParam_[IDX_IN_UNITRATIO]))*nivfParam_[IDX_IN_UNITRATIO];
	// 8
	float f1, f2, df = 1/fDt_/nivfWave_.GetSize();
	CNiReal32Vector nivfFFTtemp;
	nivfFFTtemp = nivfFFT_;
	nivfEV_[IDX_EV_DYN_4] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 2, f1);
	nivfEV_[IDX_EV_DYN_5] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 2, f2);
	nivfEV_[IDX_EV_DYN_8] = nivfParam_[IDX_IN_GAP];
	nivfEV_[IDX_EV_DYN_9] = SigMath.FeatureValue(nivdRod, nivdRod.GetSize(), E_FeatureValueType_Peak_Statistic);///���ֵ
	// 9
	nivfEV_[IDX_EV_START_IDX] = 0;
	nivfEV_[IDX_EV_END_IDX] = nSize-1;
}
