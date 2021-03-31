/*!\file CWindAndPumpVecComputer.cpp
*  \brief
*  \date 2010-11-18
*  \author 
*  \version
*/
#include "stdafx.h"
#include <KDFilter.h>
#include <SigMath.h>
#include "CWindAndPumpVecComputer.h"


using namespace Computation;

template<typename DataType>
void RemoveF0FromFreqSpect(CNiVectorT<DataType> &vecFreq_, int iF0Pos_, int iNeib_ = 3)
{
	if (iF0Pos_ < 0 || iF0Pos_ >= vecFreq_.GetSize())
		return;
	vecFreq_[iF0Pos_] = 0;
	// 消除左邻域
	int lim = (iF0Pos_ - iNeib_ >= 0) ? (iF0Pos_ - iNeib_) : 0;
	for (int i = iF0Pos_-1; i >= lim; i--)
		vecFreq_[i] = 0;
	// 消除右邻域
	lim = (iF0Pos_ + iNeib_ < vecFreq_.GetSize()) ? (iF0Pos_ + iNeib_+1) : vecFreq_.GetSize();
	for (i = iF0Pos_+1; i < lim; i++)
		vecFreq_[i] = 0;
}

 double DotProduct (
									 // Contains the first vector of data to process.
									 const CNiReal32Vector& x, 
									 // Contains the second vector of data to process.
									 const CNiReal32Vector& y, 
									 // Upon return, contains the dot product.
									 double& dotProduct)
 {
	int lnSizeMin = x.GetSize();
	if (lnSizeMin>= y.GetSize())
	{
		lnSizeMin = y.GetSize();
	}

	if (lnSizeMin<=0)
	{
		return 0;
	}

	dotProduct = 0;

	for (int i =0;i<lnSizeMin;i++)
	{
		dotProduct += x[i]*y[i]; 
	}

	return dotProduct;
 }

float GetResVib(const CNiReal32Vector & nivfAf_, float fDf_, float fF0_, int nWidth_)
{
	if (nivfAf_.GetSize() < 1)
		return 0;
	CNiReal32Vector vfTmp = nivfAf_;
	// 去除工频成分
	int i = int(fF0_/fDf_);
	RemoveF0FromFreqSpect<float>(vfTmp, i, nWidth_/2);
	// 去除2倍频成分
	i = int(fF0_*2/fDf_);
	RemoveF0FromFreqSpect<float>(vfTmp, i, nWidth_/2);
	// 去除3倍频成分
	i = int(fF0_*3/fDf_);
	RemoveF0FromFreqSpect<float>(vfTmp, i, nWidth_/2);
	// 去除1/2倍频成分
	i = int(fF0_*0.5/fDf_);
	RemoveF0FromFreqSpect<float>(vfTmp, i, nWidth_/2);
	// 计算残振
	double dPrdt = 0.0;
	if (vfTmp.GetSize() > 0)
	{
		try
		{
			DotProduct(vfTmp, vfTmp, dPrdt);	// 求内积
		}
		catch (CNiMathException *pe)
		{
			CString strDir;
			CNiFile::GetModuleDir(strDir);
			CHZLogManage::StartLogTxt(strDir+_T("log"), _T("KDFilter"), 255);
			CHZLogManage::Error(CString("ComputeAmpAndResVib: ")+pe->GetErrorMessage());
			pe->Delete();
		}
		dPrdt = sqrt(dPrdt);
	}
	return CKDFilter::IsFloatValid(float(dPrdt)) ? float(dPrdt) : 0.0;
}

// 计算特征值
void CWindAndPumpVecComputer::Compute(const CNiReal32Vector &nivfWave_, const CNiReal32Vector &nivfFFT_, float fDt_, float fFref_, const CNiReal32Vector &nivfParam_, CNiReal32Vector &nivfEV_)
{
	/* 业务流程：
		1. 求通频峰值
		2. 求各倍频幅值
		3. 求残振
		4. 求各倍频相位
		5. 直流量和间隙电压直接写入结果
	*/
	if (fFref_ < 0.1)		// 校正频率值
		fFref_ = 0.1;
	float df = 1/(fDt_*nivfWave_.GetSize());		// 频域分辨率
	float f[4] = {0, 0, 0, 0};	// 频率分量：[0]0.5×, [1]1×, [2]2×, [3]3×
	nivfEV_.SetSize((unsigned int)IDX_EV_END);
	CNiReal32Vector nivfFFTtemp;
	CNiReal64Vector nivftemp;
	nivfFFTtemp = nivfFFT_;
	nivftemp = nivfWave_; 
	int iSampleRate = int(1.0/fDt_+0.5);
	double fSpecCorrectFreq_;
	fSpecCorrectFreq_ = double(fFref_);
	// 1
	nivfEV_[IDX_EV_ALL] = CKDFilter::GetRMS(nivfWave_);
	// 4
	nivfEV_[IDX_EV_RESVIB] = GetResVib(nivfFFT_, df, fFref_, 10)/2/1.414;	// 残振(有效值)

	if (fFref_ <= 3.1*df || nivfEV_[IDX_EV_ALL] < 0.1 || fFref_*6 > iSampleRate)// 如果信号为0或者信号频率小于3df时，频谱校正算法异常，所以增加此判断
	{
		// 2
		nivfEV_[IDX_EV_ONE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_TWO] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_THREE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 3, 3, f[3])/2/1.414;	//工频幅值(有效值)
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0])/2/1.414;	// 分频幅值(有效值)
		// 3
		nivfEV_[IDX_EV_ONE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_);		// 工频相位
		nivfEV_[IDX_EV_TWO_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*2);		// 2倍频相位
		nivfEV_[IDX_EV_THREE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*3);	// 3倍频相位
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// 分频相位
	}
	else if (fFref_ > 3.1*df && fFref_ <= 6.1*df && nivfEV_[IDX_EV_ALL] >= 0.1 && fFref_*6 <= iSampleRate)
	{
		nivfEV_[IDX_EV_ONE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_TWO] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_THREE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 3, 3, f[3])/2/1.414;	//工频幅值(有效值)
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0])/2/1.414;	// 分频幅值(有效值)
		// 3
		nivfEV_[IDX_EV_ONE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_);		// 工频相位
		nivfEV_[IDX_EV_TWO_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*2);		// 2倍频相位
		nivfEV_[IDX_EV_THREE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*3);	// 3倍频相位
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// 分频相位

/*
		CSigMath SigMath;		
		vector<SSigParam> vSigComponet;
		vSigComponet.resize(4);
		SigMath.GetCalibratedSpectrumCharInfo(nivftemp, fSpecCorrectFreq_, iSampleRate, nivftemp.GetSize(), vSigComponet, E_SpectrumType_RMS);		
		nivfEV_[IDX_EV_ONE] = vSigComponet[1]._fAmp;// 工频幅值(有效值)
		nivfEV_[IDX_EV_ONE_PHASE] = vSigComponet[1]._fPhase;// 工频相位
		nivfEV_[IDX_EV_TWO] = vSigComponet[2]._fAmp;// 2倍频幅值(有效值)
		nivfEV_[IDX_EV_TWO_PHASE] = vSigComponet[2]._fPhase;// 2倍频相位
		nivfEV_[IDX_EV_THREE] = vSigComponet[3]._fAmp;// 3倍频幅值(有效值)
		nivfEV_[IDX_EV_THREE_PHASE] = vSigComponet[3]._fPhase;// 3倍频相位

		CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1]);	// 工频幅值(有效值)
		CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2]);	// 2倍频幅值(有效值)
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0])/2/1.414;	// 分频幅值(有效值)
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// 分频相位
*/
	}
	else
	{

		nivfEV_[IDX_EV_ONE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 1, 3, f[1])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_TWO] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 2, 3, f[2])/2/1.414;	// 工频幅值(有效值)
		nivfEV_[IDX_EV_THREE] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 3, 3, f[3])/2/1.414;	//工频幅值(有效值)
		nivfEV_[IDX_EV_HALF] = CKDFilter::GetFreqAmplitude(nivfFFTtemp, df, fFref_, 0.5, 3, f[0])/2/1.414;	// 分频幅值(有效值)
		// 3
		nivfEV_[IDX_EV_ONE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_);		// 工频相位
		nivfEV_[IDX_EV_TWO_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*2);		// 2倍频相位
		nivfEV_[IDX_EV_THREE_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*3);	// 3倍频相位
		nivfEV_[IDX_EV_HALF_PHASE] = CKDFilter::GetPhase(nivfWave_, fDt_, fFref_*0.5);	// 分频相位
	}

	// 5
	nivfEV_[IDX_EV_DC] = nivfParam_[IDX_IN_DC];		// 直流量
	nivfEV_[IDX_EV_GAP] = nivfParam_[IDX_IN_GAP];	// 间隙电压
	// 添加“#”之类的异常判断
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_ONE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[工频幅值(有效值)]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_ONE_PHASE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[工频相位]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_TWO]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[2倍频幅值]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_TWO_PHASE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[2倍频相位]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_THREE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[3倍频幅值]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_THREE_PHASE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[3倍频相位]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_HALF]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[0.5倍频幅值]异常"));
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
		str.Format("异常数据：%s，采样率：%d，波形长度：%d，工频：%f, 异常值：%f",\
			       str1, iSampleRate, nivftemp.GetSize(), fFref_, nivfEV_[IDX_EV_HALF_PHASE]);
		CHZLogManage::Error(str, _T("CWindAndPumpVecComputer"), _T("[0.5倍频相位]异常"));
		nivfEV_[IDX_EV_HALF_PHASE] = 0;
	}
}
