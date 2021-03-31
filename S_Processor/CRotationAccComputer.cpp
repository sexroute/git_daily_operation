/*!\file CRotationAccComputer.cpp
*  \brief
*  \date 2009-12-02
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include <SigMath.h>
#include "CRotationAccComputer.h"


using namespace Computation;

// ��������ֵ
void CRotationAccComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/* ҵ�����̣�
		1. ��ͨƵ��ֵ
		2. �����Ƶ��ֵ
		3. �����
		4. �����Ƶ��λ
		5. ֱ�����ͼ�϶��ѹֱ��д����
	*/
	if (fFref_ < 0.1)		// У��Ƶ��ֵ
		fFref_ = 0.1;
	float df = 1/(fDt_*nivfWave_.GetSize());		// Ƶ��ֱ���
	float f[4] = {0, 0, 0, 0};	// Ƶ�ʷ�����[0]0.5��, [1]1��, [2]2��, [3]3��
	nivfEV_.SetSize((unsigned int)IDX_EV_END);
	CNiReal32Vector nivfFFTtemp, nivfFFTtemp1;
	CNiReal64Vector nivftemp;
	nivfFFTtemp = nivfFFT_;
	nivfFFTtemp1 = nivfFFT_;
	nivftemp = nivfWave_; 
	int iSampleRate = int(1.0/fDt_+0.5);
	double fSpecCorrectFreq_;
	fSpecCorrectFreq_ = double(fFref_);
	// 1
	nivfEV_[IDX_EV_ALL] = CKDFilter::GetPeak2Peak(nivfWave_, false, 1, 0.01)/2;  // ͨƵ��ֵ
	nivfFFTtemp.Scale(0.5);
	if (fFref_ <= 3.1*df || nivfEV_[IDX_EV_ALL] < 0.1 || fFref_*6 > iSampleRate)// ����ź�Ϊ0�����ź�Ƶ��С��3.1dfʱ��Ƶ��У���㷨�쳣���������Ӵ��ж�
	{
		// 2
		nivfEV_[IDX_EV_ONE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1]);	// ��Ƶ��ֵ
		nivfEV_[IDX_EV_TWO] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2]);	// ��Ƶ��ֵ
		nivfEV_[IDX_EV_THREE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 3, 3, f[3]);	//��Ƶ��ֵ
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0]);	// ��Ƶ��ֵ
		// 3
		nivfEV_[IDX_EV_ONE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_);		// ��Ƶ��λ
		nivfEV_[IDX_EV_TWO_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*2);		// 2��Ƶ��λ
		nivfEV_[IDX_EV_THREE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*3);	// 3��Ƶ��λ
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// ��Ƶ��λ
	}
	else if (fFref_ > 3.1*df && fFref_ <= 6.1*df && nivfEV_[IDX_EV_ALL] >= 0.1 && fFref_*6 <= iSampleRate)
	{
		CSigMath SigMath;
		vector<SSigParam> vSigComponet;
		vSigComponet.resize(4);
		SigMath.GetCalibratedSpectrumCharInfo(nivftemp, fSpecCorrectFreq_, iSampleRate, nivftemp.GetSize(), vSigComponet, E_SpectrumType_PEAK);		
		nivfEV_[IDX_EV_ONE] = vSigComponet[1]._fAmp;// ��Ƶ��ֵ
		nivfEV_[IDX_EV_ONE_PHASE] = vSigComponet[1]._fPhase;// ��Ƶ��λ
		nivfEV_[IDX_EV_TWO] = vSigComponet[2]._fAmp;// 2��Ƶ��ֵ
		nivfEV_[IDX_EV_TWO_PHASE] = vSigComponet[2]._fPhase;// 2��Ƶ��λ
		nivfEV_[IDX_EV_THREE] = vSigComponet[3]._fAmp;// 3��Ƶ��ֵ
		nivfEV_[IDX_EV_THREE_PHASE] = vSigComponet[3]._fPhase;// 3��Ƶ��λ

		CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1]);	// ��Ƶ��ֵ
		CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2]);	// 2��Ƶ��ֵ
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0]);	// ��Ƶ��ֵ
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// ��Ƶ��λ
	}
	else
	{
		CSigMath SigMath;
		vector<SSigParam> vSigComponet;
		vSigComponet.resize(4);
		SigMath.GetCalibratedSpectrumCharInfo(nivftemp, fSpecCorrectFreq_, iSampleRate, nivftemp.GetSize(), vSigComponet, E_SpectrumType_PEAK);		
		nivfEV_[IDX_EV_ONE] = vSigComponet[1]._fAmp;// ��Ƶ��ֵ
		nivfEV_[IDX_EV_ONE_PHASE] = vSigComponet[1]._fPhase;// ��Ƶ��λ
		nivfEV_[IDX_EV_TWO] = vSigComponet[2]._fAmp;// 2��Ƶ��ֵ
		nivfEV_[IDX_EV_TWO_PHASE] = vSigComponet[2]._fPhase;// 2��Ƶ��λ
		nivfEV_[IDX_EV_THREE] = vSigComponet[3]._fAmp;// 3��Ƶ��ֵ
		nivfEV_[IDX_EV_THREE_PHASE] = vSigComponet[3]._fPhase;// 3��Ƶ��λ
		nivfEV_[IDX_EV_HALF] = vSigComponet[0]._fAmp;// ��Ƶ��ֵ
		nivfEV_[IDX_EV_HALF_PHASE] = vSigComponet[0]._fPhase;// ��Ƶ��λ
	}
	// 4
	nivfEV_[IDX_EV_RESVIB] = CKDFilter::GetResVib(nivfFFTtemp1, df, fFref_, 10)/2;	// ����
	// 5
	nivfEV_[IDX_EV_DC] = nivfParam_[IDX_IN_DC];		// ֱ����
	nivfEV_[IDX_EV_GAP] = nivfParam_[IDX_IN_GAP];	// ��϶��ѹ
	// ��ӡ�#��֮����쳣�ж�
	CString str, str1;
	CNiString strtemp;
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_ONE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_ONE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[��Ƶ��ֵ(��Чֵ)]�쳣"));
		nivfEV_[IDX_EV_ONE] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_ONE_PHASE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_ONE_PHASE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[��Ƶ��λ]�쳣"));
		nivfEV_[IDX_EV_ONE_PHASE] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_TWO]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_TWO]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[2��Ƶ��ֵ]�쳣"));
		nivfEV_[IDX_EV_TWO] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_TWO_PHASE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_TWO_PHASE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[2��Ƶ��λ]�쳣"));
		nivfEV_[IDX_EV_TWO_PHASE] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_THREE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_THREE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[3��Ƶ��ֵ]�쳣"));
		nivfEV_[IDX_EV_THREE] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_THREE_PHASE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_THREE_PHASE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[3��Ƶ��λ]�쳣"));
		nivfEV_[IDX_EV_THREE_PHASE] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_HALF]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_HALF]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[0.5��Ƶ��ֵ]�쳣"));
		nivfEV_[IDX_EV_HALF] = 0;
	}
	if (!CKDFilter::IsFloatValid(nivfEV_[IDX_EV_HALF_PHASE]))
	{
		str = str1 = strtemp = "";
		for (int i = 0; i < nivftemp.GetSize();i++)
		{
			strtemp << nivftemp[i] << " ";
		}
		str1 = strtemp;
		str.Format("�쳣���ݣ�%s�������ʣ�%d�����γ��ȣ�%d����Ƶ��%f, �쳣ֵ��%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_HALF_PHASE]);
		CHZLogManage::Error(str, _T("CRotationAccComputer"), _T("[0.5��Ƶ��λ]�쳣"));
		nivfEV_[IDX_EV_HALF_PHASE] = 0;
	}
}
