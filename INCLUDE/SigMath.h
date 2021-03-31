#pragma once

/**@brief ���������е��������H����Ĳ������ݻ�V����Ĳ������ݣ��������ϸ������ϵ�ˮƽ�ʹ�ֱ�������ݡ�
  *������ԣ�������ת��˵���������������źž���H����ģ�����������V����ġ�
  */

#include "Sigdefine.h"
#include "Matrix.h"


class CLASS_DLL_DSE_SigMath CSigMath
{
public:
	CSigMath(void);
	~CSigMath(void);
public:///�׷�����صĺ���
	/**@brief ������Ƶ��У���� ����ƵЧ����12*df���£����С��3*df������У����
	  *@param pWave_ ʱ������
	  *@param iSmpFreq_ ����Ƶ��
	  *@param ilen_     ʱ�����г���
	  *@param fSpecCorrectFreq_ ��ҪУ����Ƶ��
	  *@param fCorrectedFreq_ У�����Ƶ��
	  *@param fCorrectedAmp_ У����Ƶ�ʶ�Ӧ�ķ�ֵ
	  *@param fCorrectedPhase_ У����Ƶ�ʶ�Ӧ����λ
	  */
    int EnergySpecCorrect(const double *pWave_,const int& iSmpFreq_, const int &ilen_ ,const double& fSpecCorrectFreq_, double &fCorretedFreq_, double &fCorretedAmp_, double &fCorrecdPhase_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);

	/**@brief Ƶ�׷���
	  *@param pwave ʱ��������
	  *@param n     ���εĳ���
	  *@param pout  Ƶ�����ݵ�������������ȷ�����ڴ棬����Ϊn/2
	  *@param type  Ƶ������0����ֵ�ף�1:������
	  *@param iWinType_ �Ӵ������ͣ��������Ͷ��塣
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�
	  */
	int SpectralMeasureMent(const double *pwave,int n,double *pout,E_SpectrumType type,E_WINOW_TYPE iWinType_ = WINDOW_HANNING, unsigned int iPrecision_ = 2);
	///�����vOut_����Ϊn/2��
	int SpectralMeasureMent(const double *pwave,int n,vector<double>& vOut_,E_SpectrumType type,E_WINOW_TYPE iWinType_ = WINDOW_HANNING, unsigned int iPrecision_ = 2);

	/**@brief ���׷���
	  *@param pwave ʱ��������
	  *@param n     ���εĳ���
	  *@param pout  Ƶ�����ݵ�������������ȷ�����ڴ棬����Ϊn/2
	  *@param iNums_ ���׵Ĵ�0��ָ�����ȵĵ���Ϊ0��Ŀ��������ǰ�˵ĸ���
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�
	  */
	int Cepstrum(const double *pwave, const int& n,double *pout, const int &iNums_ = 20, unsigned int iPrecision_ = 2);
    
	/**Ƶ��У�����ܣ�����ָ����У����,ʹ�õ�ȫ��λ��������У������ ע�������ǹ����ס� �Զ��жϲ��ú���У��������У��ָ��Ƶ�ʳɷ֡�
	  *�÷���ֱ��У��ָ��Ƶ��
	  *@param pData_ ��������
	  *@param iFs_ ����Ƶ��
	  *@param ilen_ ���ݳ���
	  *@param vSigComponet_ ��ҪУ����Ƶ�ʳɷ�n���������������á����룺Ϊ��ҪУ����Ƶ�ʳɷ���Ϣ����������λ�ͷ�ֵ���������У����Ƶ�ʳɷ���Ϣ(˳���ǰ���Ƶ�ʴӵ�������) ������ֵΪ����У����Ƶ�ʸ�������
	  *@return >=0 ��ʾ��ȷ�� < 0ʧ�ܡ�����0 ����£����ص���ʵ�ʵ�У����Ƶ����Ŀ
	  */
	int SpectrumLinesCalibrate(const double* pData_, const int& iFs_, const int& ilen_,vector<SSigParam>& vSigComponet_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);
    
	/**ȫƵ��У�����ܣ�����ָ����У����,ʹ�õ�ȫ��λ��������У������Ƶ��У��������Ҫָ��Ƶ�ʣ�����ֱ��У����
      **��У��1X��Ȼ��ͨ��1X��������Ƶ�ʺ󣬽�������Ƶ��У��
	  *��У�����ֵ�����Ѿ������Ƶ�׵Ķ�Ӧλ�� ע�������ǹ�����
	  *У����Ƶ�ʳɷ�Ϊ��1/2X,1X,2X,3X,
	  *ע�⣺E_SpectrumType �� E_WINOW_TYPE ���ͱ�������������Ƶ������������ͬ��
	  *@param pData_ ��������
	  *@param iFs_ ����Ƶ��
	  *@param ilen_ ���ݳ���
	  *@param pSpectrumAmp_ ���룺�Ѿ����õ�Ƶ�����ݣ���������ͼʱ��Y�����ݣ���Ƶ�׷�ֵ���� ����ΪiSpectrumlen_ �������У������׷�ֵ
	  *@param pSpectrumf_ ���룺�Ѿ�����Ƶ�׵�Ƶ�ʣ���������ͼʱ��X�����ݡ� ����ΪiSpectrumlen_�������У�����Ƶ�׵�Ƶ��ֵ�����ᣩ
	  *@param iSpectrumLen_ ��������ֵ�����룺У��ǰƵ�����ݵĳ��ȣ������У����Ƶ�����ݵĳ���
	  *@param spectrumType_ Ƶ������
	  *@param eWinType_ �Ӵ�����
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�unsigned int iPrecision_ = 2
	  *@return 0 ��ʾ��ȷ ����ʧ��
	  */
    int SpectrumCalibrate(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING,unsigned int iPrecision_ = 2 );

	/**�ڲ����á�SpectrumCalibrate���ú�����Ϊ�����Ͻӿڣ����Ա���
	  */
	int apSpectrumCalibrateEx(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING,unsigned int iPrecision_ = 2);

    /**ʹ��У���׼�������ֵ��0.5X��1X��2X��3X�ķ�ֵ��λ
	  *@param pData_ ��������
	  *@param  f0_   ��׼Ƶ�ʣ�һ��Ϊ��Ƶ��1XƵ��
	  *@param iFs_ ����Ƶ��
	  *@param ilen_ ���ݳ���
	  *@param vSigComponet_ ��ҪУ����Ƶ�ʳɷ�4��������Ҫ���ⲿ�����
	  *@return 0 ��ʾ��ȷ ����ʧ��
	  */
    int GetCalibratedSpectrumCharInfo(const double* pData_,const double& f0_, const int& iFs_, const int& ilen_,vector<SSigParam>& vSigComponet_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak,E_WINOW_TYPE eWinType_ = WINDOW_HANNING);

	/**ȫƵ��У�����ܣ�����ָ����У����,ʹ�õ�ȫ��λ��+ȫƵ����Ƶ����Ƶ��У��������Ҫָ��Ƶ�ʣ�����ֱ��У��������ƵЧ����12*df�����������ϣ�
	*��У�����ֵ�����Ѿ������Ƶ�׵Ķ�Ӧλ�� ע�������ǹ�����
	*У����Ƶ��Ϊ��1/2X,1X,2X,3X,4X,0~2*df�м��һ�����ݣ�0~1X�м�Ƚϴ��һ��ֵ
	*ע�⣺E_SpectrumType���ͱ�������������Ƶ������������ͬ��
	*@param pData_ ��������
	*@param iFs_ ����Ƶ��
	*@param ilen_ ���ݳ���
	*@param pSpectrumAmp_ ���룺�Ѿ����õ�Ƶ�����ݣ���������ͼʱ��Y�����ݣ���Ƶ�׷�ֵ���� ����ΪiSpectrumlen_ �������У������׷�ֵ
	*@param pSpectrumf_ ���룺�Ѿ�����Ƶ�׵�Ƶ�ʣ���������ͼʱ��X�����ݡ� ����ΪiSpectrumlen_�������У�����Ƶ�׵�Ƶ��ֵ�����ᣩ
	*@param iSpectrumLen_ ��������ֵ�����룺У��ǰƵ�����ݵĳ��ȣ������У����Ƶ�����ݵĳ���
	*@param spectrumType_ Ƶ������
	*@param eWinType_ �Ӵ�����
	*@return 0 ��ʾ��ȷ ����ʧ��
	*/
	int apSpectrumCalibrateEx2(const double* pData_, const double& f0_, const int& iFs_, const int& ilen_, double* pSpectrumf_,double * pSpectrumAmp_,int& iSpectrumLen_, E_SpectrumType spectrumType_ = E_SpectrumType_Peak_Peak, E_WINOW_TYPE eWinType_ = WINDOW_HANNING);


	/**@brief ����ȫ��
	*@param  pWaveV_  ���봹ֱ����Ĳ�������
	*@param  pWaveH_  ����ˮƽ����Ĳ�������
	*@param  ilen_ �������ݳ���
	*@param  f0_   ��׼Ƶ�ʣ�һ��Ϊ��Ƶ��1XƵ��
	*@param  fFs_  ����Ƶ��
	*@param  iHiFcNum_ Ҫ��ȡ�ĸߴ�г������Ŀ
	*@param  fHiFcTh_  �ߴ�г������ֵ�������������ֵ����ȡ��Ƶ�ʵ�г��
	*@param  iLoFcNum_ ��Ƶ����Ŀ
	*@param  mapFullSpecInfo_ ȫ�׵���Ϣ������ֵΪ��Ƶֵ����f/f0����secondΪ��Ӧ��ֵ��������ǰ���ⲿ���ڴ����á�����Ϊ��iHiFcNum_+iLoFcNum_��������
	*@param  bRemove50_ �Ƿ�����50Hz����
	*/
	int FullSpectrum(const double *pWaveV_,const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& mapFullSpectumInfo_,bool bRemove50_ = true);

	/**@brief ����ȫ�׵ڶ��ַ������÷����ǲ�������ת�ټ��㣬����ÿ�������ϵ�ֵ����X��Ϊ��-iFs/2,iFs/2����
	*@param  pWaveV_  ���봹ֱ����Ĳ�������
	*@param  pWaveH_  ����ˮƽ����Ĳ�������
	*@param  ilen_ �������ݳ���
	*@param  f0_   ��׼Ƶ�ʣ�һ��Ϊ��Ƶ��1XƵ��
	*@param  fFs_  ����Ƶ��
	*@param  vFullSpectrumX_ ȫ��X���������λ��Hz������Ϊ��ilen_.�����ⲿ�����
	*@param  vFullSpectrumY_ ȫ��Y�����������Ϊ��ilen_.�����ⲿ�����
	*/
	int FullSpectrumII(const double *pWaveV_,const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, vector<double>& vFullSpectrumX_,vector<double>& vFullSpectrumY_);



	/**@brief �������׷���������λ
	  *@param pWave_ �������ݡ���ʱ����������
	  *@param iLen_  �������ݳ���
	  *@param fSpecFreq_ ��Ҫ������λ��Ƶ��
	  *@param iSmpFreq_ �������ݲ���Ƶ��
	  *@param fPhase_   ���ص���λ
      */
	int CrossSpecPhase(const double *pWave_,const int& iLen_, double fSpecFreq_,const int& iSmpFreq_,double & dPhase_);

	int CrossPhaseDetect(const double *pWaveX_, const double *pWaveY_, const int& ilen_, const int& iSmpFreq_,const double& fSpecFreq_,double & fPhase_);

	/**@brief  �ᴿ��������.�ֱ�õ���Ƶ���κ͸�г���Ĳ��Σ�Ȼ����кϳ�
	  *@param  pWave_ ��������
	  *@param  iLen_ �������ݳ���
	  *@param  f0_   ��׼Ƶ�ʣ�һ��Ϊ��Ƶ��1XƵ��
	  *@param  fFs_  ����Ƶ��
	  *@param  iHiFcNum_ Ҫ��ȡ�ĸߴ�г������Ŀ
	  *@param  fHiFcTh_  �ߴ�г������ֵ�������������ֵ����ȡ��Ƶ�ʵ�г��
      *@param  iLoFcNum_ ��Ƶ����Ŀ
      *@param  vSigParam_ ���������������Ƶ�ʳɷ�,�������ⲿ���������������Ϊ��iHiFcNum_+iLoFcNum_��
	  *@param  pWaveOutput_ �ᴿ��Ĳ��Σ��ڴ�������ⲿ����ã�����ΪiLen_
	  *@param  bRemove50_ �Ƿ�����50Hz����
	  *@return E_DseSigRet
      */
	 int SCorbitpurifier(const double *pWave_,const int& iLen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& vSigParam_,double * pWaveOutput_,bool bRemove50_ = true);
     
	 /**@brief�������ָ���������Ƴ�����Բ�ĳ�����ǣ����ص��Ƕ��������ǻ���
	   *@param  pWaveV_  ���봹ֱ����Ĳ������ݣ���������������
	   *@param  pWaveH_  ����ˮƽ����Ĳ������ݣ���������������
	   *@param  ilen_ �������ݳ��ȡ����߲��γ��ȱ������.���Ȳ���С��36
	   *@param  fAngle_ ����ֵ���������
	   *@return �Ƿ���ȷ
	   */
     int EllipBiasAngle(const double *pWaveV_,const double *pWaveH_, const int& ilen_, double& fAngle_);
	 ///fFreqV_ ��ֱ�����Ƶ�ʣ�fPhaseV_��ֱ�������λ��һ��Ҫ��fFreqV_=fFreqH_
	 int EllipBiasAngle(double fFreqV_, double fPhaseV_, double fAmpV_, double fFreqH_, double fPhaseH_, double fAmpH_, double& fAngle_);



	 /**@brief �ᴿ���Ĺ켣��������
	   *@param pWaveV_  ���봹ֱ����Ĳ������ݣ�������ᴿ������Ĳ�������
	   *@param pWaveH_  ����ˮƽ����Ĳ������ݣ�������ᴿ������Ĳ�������
	   *@param  ilen_ �������ݳ���
	   *@param  f0_   ��׼Ƶ�ʣ�һ��Ϊ��Ƶ��1XƵ��
	   *@param  fFs_  ����Ƶ��
	   *@param  iHiFcNum_ Ҫ��ȡ�ĸߴ�г������Ŀ
	   *@param  fHiFcTh_  �ߴ�г������ֵ�������������ֵ����ȡ��Ƶ�ʵ�г��
	   *@param  iLoFcNum_ ��Ƶ����Ŀ
	   *@param  vSigParamV_ ��ֱ���������������Ƶ�ʳɷ�,�������ⲿ���������������Ϊ��iHiFcNum_+iLoFcNum_��
	   *@param  vSigParamH_ ˮƽ���������������Ƶ�ʳɷ�,�������ⲿ���������������Ϊ��iHiFcNum_+iLoFcNum_��
	   *@param  bRemove50_ �Ƿ�����50Hz����
	   */
     int SCorbitpurifie_Rp(double *pWaveV_, double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, vector<SSigParam>& vSigParamV_,vector<SSigParam>& vSigParamH_,bool bRemove50_ = true);
	 int SCorbitpurifie_Rp(double *pWaveV_, double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, const int& iHiFcNum_, double fHiFcTh_,const int& iLoFcNum_, double fLoFcTh_, bool bRemove50_ = true);

	 /**Method:    CheckOrbitShape  ���Ĺ켣����
      *@brief   FullName:  CSigMath::CheckOrbitShape
      *@param  : double * pWaveV_ ��ֱ����������
      *@param  : double * pWaveH_ ˮƽ����������
      *@param  : const int & ilen_ �������ݳ���
      *@param  : double f0_ ��Ƶ
      *@param  : const int & iFs_ ����Ƶ��
      *@param  : double fThFactor_ ��ֵ����
      *@return:   int С��0��ʾ����֮Ϊ���Ĺ켣��״������ֵ�����Ĺ켣��״�궨�� E_OrbitShape
	  */
	 int CheckOrbitShape(const double *pWaveV_, const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_,double fThFactor_= 0.25);
  

	 /**@brief Fast Discrete Walsh-Hadamard Transform
	 *@param pWave_  ��������
	 *@param ilen_   �������ݵĳ���
	 *@param pWaveOut_  ����Ĳ������ݣ��������ⲿ���ڴ����ã�����Ϊilen_ 
	 */
	 int FWalshHT(const double * pWave_, const int&ilen_, double *pWaveOut_);

	/**��ά���߰���
	  *@param pXInput_,pYInput_ �㼯����������ΪiLen_
      *@param iLen_ �㼯�ĳ���
	  *@param pXOutput_,pYOutput_, ������ߵĵ㼯�������������ⲿԤ�ȷ���ã�����ΪiLen_����������ߵĳ���Ϊ����ֵ
	  *@param fMaxDisCoef_ �������ߵ���󳤶�ϵ������󳤶�Ϊ�㼯x�������ݺ�y���������е������������Ը�ϵ����
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�unsigned int iPrecision_ = 2
      *@return С��0Ϊʧ�ܣ������������ߵ�ʵ�ʵ���
	  */
	int TwoDimCurveEnvelope(const vector<double>& pXInput_,const vector<double>& pYInput_, const int &iLen_,  vector<double>& pXOutput_, vector<double>& pYOutput_, const double& fMaxDisCoef_ = 0.2,unsigned int iPrecision_ = 2);
	int TwoDimCurveEnvelope(const double* pXInput_,const double* pYInput_, const int &iLen_,  double* pXOutput_, double* pYOutput_, const double& fMaxDisCoef_ = 0.2,unsigned int iPrecision_ = 2);

	  /**1ά���߰���
		*@param pInput_ 1ά������������ΪiLen_.����С��32����,������Ҫ��3��
		*@param iLen_ �㼯�ĳ���
		*@param pUpperLine_ ���ߵİ������������ߣ��������ⲿԤ�ȷ���ã�����ΪiLen_����������ߵĳ���Ϊ����ֵ
		*@param pLowerLine_ ���ߵİ������������ߣ��������ⲿԤ�ȷ���ã�����ΪiLen_����������ߵĳ���Ϊ����ֵ
		*@param iPerSegMinPts_ ��Ϸֶ�ʱ��ÿ�ε����ٵ���������С��5��
		*@return С��0Ϊʧ�ܣ������������ߵ�ʵ�ʵ���
		*/
	int OneDimCurveEnvelope(const vector<double>& pInput_, const int &iLen_, vector<double>& pUpperLine_, vector<double>& pLowerLine_,const int& iPerSegMinPts_ = 10);

	/**����1ά�ķֶ���Чֵ��ͼ,һ��Ҫ��ilen_ҪԶ����iSegs_
	*@param pwave_ 1ά����������Ϊilen_
	*@param ilen_ �㼯�ĳ���
	*@param pbar_ �������ⲿԤ�ȷ���ã�����ΪiSegs_���������ֵ���
	*@param iSegs_ �ֶε���Ŀ
	*@return С��0Ϊʧ�ܣ�����������Ŀ
	*/
	int CalcRmsBar(const vector<double>& pwave_, const int& ilen_, vector<double>& pbar_, const int& iSegs_ = 8);
	int CalcRmsBar(const double* pwave_, const int& ilen_, double* pbar_, const int& iSegs_ = 8);

	/**����1ά�ķֶη�ֵ��ͼ,һ��Ҫ��ilen_ҪԶ����iSegs_
	*@param pwave_ 1ά����������Ϊilen_
	*@param ilen_ �㼯�ĳ���
	*@param pbar_ �������ⲿԤ�ȷ���ã�����ΪiSegs_���������ֵ���
	*@param iSegs_ �ֶε���Ŀ
	*@return С��0Ϊʧ�ܣ�����������Ŀ
	*/
	int CalcPeakBar(const vector<double>& pwave_, const int& ilen_, vector<double>& pbar_, const int& iSegs_ = 8);
	int CalcPeakBar(const double* pwave_, const int& ilen_, double* pbar_, const int& iSegs_ = 8);

    /**�Զ�Ѱ�Ű������������Զ�Ѱ�����ŵĽ�ֹƵ�� 
	  *@param pwave_ 1ά����������Ϊilen_
	  *@param ilen_ �㼯�ĳ���
	  *@param iFs_  ����Ƶ��
	  *@param fgSEValue_ ���ؼ���õ���gSEֵ
	  *@param pEnvWave_ ���س����Ϸ�����Ĳ��Σ�����Ԥ�ȷ�����ڴ棬����Ϊilen_���������Ҫ���ؾ�����ΪNULL
	  *@return С��0��ʾʧ�ܡ�����0�ͱ�ʾ�Զ�Ѱ�ź�Ľ�ֹƵ��
	  */
    int AutoOptimizeEnvelopeAnalysis(const double* pwave_, const int& ilen_, const int& iFs_,double & fgSEValue_, double *pEnvWave_ = NULL);

	/**�����Ϸ��������������������gSEֵ
	  *@param pwave_ 1ά����������Ϊilen_
	  *@param ilen_ �㼯�ĳ���
	  *@param iFs_  ����Ƶ��
	  *@param iCutoffFreq_ ��ͨ�˲��Ľ�ֹƵ��
	  *@param fgSEValue_ ���ؼ���õ���gSEֵ
	  *@param pEnvWave_ ���س����Ϸ�����Ĳ��Σ�����Ԥ�ȷ�����ڴ棬����Ϊilen_���������Ҫ���ؾ�����ΪNULL
	  *@param iPrecision_ ���ȣ��������6���Ͳ������ȴ���ʵ���Ƕ��پ����١�
	  *@return С��0��ʾʧ��
	  */
	int gSEAnalysis(const double* pwave_, const int& ilen_, const int& iFs_,const int& iCutoffFreq_, double & fgSEValue_, double *pEnvWave_ = NULL,unsigned int iPrecision_ = 2);

    /**PeakVue�㷨
	  *@param pwave_ 1ά����������Ϊilen_
	  *@param ilen_ �㼯�ĳ���
	  *@param iFs_  ����Ƶ��
	  *@param iReSamplesFs_ �ز�����Ƶ�ʡ�������iFs_��������
	  *@param iCutoffFreq_ ��ͨ�˲��Ľ�ֹƵ��
	  *@param pPeakVueWave_ ������PeakVue��Ĳ��Σ�����Ԥ�ȷ�����ڴ棬����Ϊilen_
	  *@return С��0 ����ʾ����ʧ�ܣ�����ΪPeakVue�����Ĳ��ε���
	  */
	int PeakVue(const double *pwave_, const int& ilen_, const int& iFs_, const int& iReSamplesFs_,const int& iCuttoffFreq_,  double * pPeakVueWave_);
	

   //************************************
	// Method:    IntegrateDecimate_Customize �����õ�ר�ü��ٶ��ز������߱����칦�ܣ��ͻ���Ϊ�ٶ��㷨��
	//            �ز�����Ĳ���Ƶ��ֻ֧��5120Hz�����ã���2560Hz��3200Hz�������ٶˣ���80Hz�������ٶˣ���
	// FullName:  CBHDSP::IntegrateDecimate_Customize
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const double * pdata_ �����ֵ�����
	// Parameter: const unsigned int & ilen_ ���ݵĳ���
	// Parameter: const int & iFs_   ���ݵĲ���Ƶ�ʡ��������Ƶ��ʹ��25600Hz��20480Hz��15360Hz
	// Parameter: const unsigned int & iWorkRev_  �ò�����ݶ�Ӧ�Ĺ���ת��
	// Parameter: const int iExtractMutli_    ��ȡ����������ȡ���Ƶ��Ϊ iFs_/iExtractMutli_.
	// Parameter: double * pIntergatedata_    ���ֺ�����ݣ��ڴ�����ڵ���ǰ����ã�����Ϊ iIntegrateDataLen_
	// Parameter: unsigned int & iIntegrateDataLen_ ���룺Ԥ�ȷ�����ֺ����������ĳ��ȣ���������ֺ�����ݳ��ȡ�
	// Parameter: float fSensitivity_ ������ϵ�� �����(���ֺ�)=����/fSensitivity_
	//************************************
	int IntegrateDecimate_Customize(const double* pdata_, const unsigned int& ilen_, const unsigned int& iFs_, const unsigned int& iWorkRev_,const int iExtractMutli_, double *pIntergatedata_, unsigned int& iIntegrateDataLen_, const float& fSensitivity_ = 0.001);
	
	//************************************
	// Method:    Integrate_Customize �����õ�ר�ü��ٶȻ���Ϊ�ٶ��㷨������Ƶ��ֻ֧��5120Hz�����ã���2560Hz,3200Hz�������ٶˣ���80Hz�������ٶˣ���
	// FullName:  CBHDSP::Integrate_Customize
	// Access:    public static 
	// Returns:   int
	// Qualifier:
	// Parameter: const double * pdata_ �����ֵ�����
	// Parameter: const unsigned int & ilen_ ���ݵĳ���
	// Parameter: const int & iFs_   ���ݵĲ���Ƶ��,����Ƶ��ֻ֧��5120Hz�����ã���3200Hz�������ٶˣ���80Hz�������ٶˣ���
	// Parameter: const unsigned int & iWorkRev_  �ò�����ݶ�Ӧ�Ĺ���ת��
	// Parameter: double * pIntergatedata_    ���ֺ�����ݣ��ڴ�����ڵ���ǰ����ã�����Ϊ ilen_
	// Parameter: float fSensitivity_ ������ϵ�� �����(���ֺ�)=����/fSensitivity_
	//************************************
	int Integrate_Customize(const double* pdata_, const unsigned int& ilen_, const unsigned int& iFs_, const unsigned int& iWorkRev_,double *pIntergatedata_,const float& fSensitivity_ = 0.001);



	/**���ֻ��ֺ������л��֡�
	 *@param pfData_    ��Ҫ���ֵ�����
	 *@param pfDataRs_  ���ֺ������������Ϊ���ݳ���
	 *@param iDataLen_  ���ݳ���
	 *@param iSmpFreq_  ����Ƶ��     
	 *@param fFreqBandLo_ ��ͨ�˲�����
	 *@param fFreqBandHi_ ��ͨ�˲�����
	 *@Parameter: float fSensitivity_ ������ϵ�� �����(���ֺ�)=����/fSensitivity_
	 *@return  0 ��ʾ��ȷ��-1��ʾ��������ȷ
	 */
	int IntegratorEx(const double *pfData_,double *pfDataRs_,const int& iDataLen_,const int& iSmpFreq_,double fFreqBandLo_ = 5, double fFreqBandHi_ = 1000,const float& fSensitivity_ = 1);


	/**ʹ��Ƶ�򷽷����л������ֻ��ֺ���API��
	 *@param pfData_    ��Ҫ���ֵ�����
	* @param pfDataRs_  ���ֺ������������Ϊ���ݳ���
	*@param iDataLen_  ���ݳ���
	*@param iSmpFreq_  ����Ƶ��     
	*@param fFreqBandLo_ ��ͨ�˲�����
	*@param fFreqBandHi_ ��ͨ�˲�����
	*@param wType_ Ƶ�׼Ӵ�������
	*@return  0 ��ʾ��ȷ��-1��ʾ��������ȷ
	*/
	int IntegratorEx2(const double *pfData_,double *pfDataRs_,const int& iDataLen_,const int& iSmpFreq_, double fFreqBandLo_ = 5, double fFreqBandHi_ = 1000,E_WINOW_TYPE wType_ = WINDOW_HANNING);

	/**��Ƶ�׽��л���
	*@param pSpectrum ����Ҫ���ֵ�Ƶ�����ݣ�����ɹ���Ϊ���ֺ�Ľ��
	*@param df Ƶ�׷ֱ���
	*@param ilen_ pSpectrum���ݵ���Ŀ
	*@param fSensitivity_ ������ϵ�� �����(���ֺ�)=����/fSensitivity_
	*@param eSpectrumType_  ��Ҫ����Ƶ�����ݵ�Ƶ������
	*@param iStartidx_ ��ʼ���ֵ�����
	*@return  0 ��ʾ��ȷ��С��0��ʾ����
	*/
	int SpectrumIntegrate(double * pSpectrum_, const double& df_,const int& ilen_, const double& fSensitivity_= 1.0,E_SpectrumType eSpectrumType_ = E_SpectrumType_PEAK, const int& iStartidx_ = 2);

	/**ʹ�ñ�׼������������������쳣�жϣ��������ŶȺͱ�׼��ı�������󷵻��жϵĽ��
	*@param sampleData_ ��׼��������������n * k ����k������ֵ��Ŀ��n��������Ŀ
	*@param testData_   ���죨���ԣ�������m * k ����k������ֵ��Ŀ��m��������Ŀ
	*@param alpha_     ���Ŷȡ���һ���ٷֱ�����Ĭ�� 0.9����90%
	*@param factorAlarm_    ��׼��ķŴ�ϵ�� �����ڹ��ϣ�Ԥ����
	*@param factorDanger_      ��׼��ķŴ�ϵ�� ���ڹ��ϣ��Ѿ����أ���Ҫͣ����
	*@return 0 ��ʾ����������0���������쳣��С��0����������г����쳣��
	*/
	int AbnormalJudge(const CMatrix& sampleData_,const CMatrix& testData_, const float& alpha_ = 0.9,const float& factorAlarm_ = 2, const float& factorDanger_ = 3 );

	/**ʹ�ñ�׼������������������쳣�жϡ�����������ֵ��ĿΪk���������ŶȺͱ�׼��ı�������󷵻��жϵĽ����
	*@param testData_   ���죨���ԣ�������m * k ����k������ֵ��Ŀ��m��������Ŀ
	*@param sigma_   ��׼����� k*k
	*@param vMeanValue_ ��ֵ����������Ϊk
	*@param alpha_     ���Ŷȡ���һ���ٷֱ�����Ĭ�� 0.9����90%
	*@param factorAlarm_    ��׼��ķŴ�ϵ�� �����ڹ��ϣ�Ԥ����
	*@param factorDanger_      ��׼��ķŴ�ϵ�� ���ڹ��ϣ��Ѿ����أ���Ҫͣ����
	*@return 0 ��ʾ����������0���������쳣��С��0����������г����쳣��
	*/
	int AbnormalJudge2(const CMatrix& testData_,const CMatrix& sigma_,const vector<double>& vMeanValue_, const float& alpha_ = 0.9,const float& factorAlarm_ = 2, const float& factorDanger_ = 3 );

	/**����1��ʾ��ͼ������ֵ��7����������ܳ����Ͷȣ���ȣ�walsh�任��õ����趨������ֵ��Ĭ��Ϊ3������
      *@param pV_   1������ʾ��ͼ���ߵ�������У�����Ϊilen_
	  *@param pP_   1������ʾ��ͼ���ߵ�ѹ�����У�����Ϊilen_
	  *@param ilen_ 1������ʾ��ͼ���ߵĵ���
	  *@param fPs_  �������ѹ��
	  *@param fPd_  ��������ѹ��
	  *@param vFeatures_ �����������ڴ������ǰ����ã�����Ϊ4+iSegNums
	  *@param th_   ��ֵ��walsh�任��������ֵ����ʱʹ�ã�Ĭ��Ϊ1
	  *@param iSegNums ������walsh�任�����ֶμ��㳬����ֵ����Ŀ��
	  */
	int PVGraphFeature(const double* pV_,const double* pP_, const int& ilen_,const double& fPs_, const double& fPd_, vector<double>& vFeatures_, const int& th_ = 1, const unsigned int& iSegNums_ = 3 );
	
	/**�Զ�Ѱ���㷨��ֻ������ֵ�����Ѹ���ֵ
	  *@brief pData_ Ѱ�����������
	  *@brief ilen_ ���ݳ���
	  *@brief iPeakNums_ ����Ϊ����Ѱ�����Ŀ�����Ϊʵ��Ѱ�����Ŀ
	  *@brief vPeakInd_ ��ֵ�����������ȱ������ⲿ����ã�����ΪiPeakNums_
	  *@brief iInterval_ ����С�������Ŀ������ֵ����С����ֵ����Ŀ
	  *@brief fth_����ֵ����ֵ������ڸ�ֵ�����ֵ
	  */
	int Peak_Detection(const double* pData_, const int& ilen_, int& iPeakNums_,vector<int>& vPeakInd_, const int& iInterval_ = 2, const double& fth_ = .1);

	/**���������ı�Ƶ��Χ����Ƶ�׷�ֵ�Ķ�ӦƵ�ʺ�ֵ��ע����Ҫ��ǰ��vSigParam_�ߴ�����
	 *�������ĵ�Ƶ3����ͬƵ����Ƶ4��������ߵ�4��Ƶ�����ʵ�ʷֱ���û����ô�ߣ��Ͳ��Ѽ�������Ƶ��
	 *@brief vSigParam_ �����Ľ�����������ΪiLoNums_+ iHiNums_
	 *@brief iLoNums_ �����ĵ�Ƶ��Ŀ
	 *@brief iHiNums_ �����ı�Ƶ��Ŀ��1X��2X��...
	 *@return С��0Ϊʧ�ܣ�����Ϊ�ѵ���������Ϣ����
	 */
	int SpectrumLinesPeakSearchingEx(const double *pWave_, const int& iLen_, const int &iFs_, const double& f0_,vector<SSigParam>& vSigParam_,const int& iLoNums_ = 3, const int & iHiNums_ = 4);

	/**С����ѹ���㷨��ע����Ҫ��ǰ��vSigParam_�ߴ����á�
	  *�������������ź�Ҫ�󣺳��Ȳ���С��64��������2^n��n>6
	  *@return С��0Ϊʧ�ܣ������0Ϊ�ɹ���
	  */
	int WPCompress(const vector<double>& vWave_, const int &iFs_, CCompressedParamInfo & compressedParamInfo_);

	/**vRestronctedData_ �ź����ⲿ�ڴ����ã����ȱ�ʾ�źų���
	  *�������������ź�Ҫ�󣺳��Ȳ���С��64��������2^n��n>6
	  */
	int WPDecompress(const CCompressedParamInfo& compressedParamInfo_, vector<double>& vRestronctedData_);
public:
	/**@fn CalcBearingDefectFreq
	*@brief  ���������еĹ��ϣ�ͨ����Ƶ��
	*@param  bearingpar_ ������и��ֲ���
	*@param  fWorkRev_   ����ת��
    *@param  defectFreq_  [out] �������ȱ��Ƶ��
	*@return 0��ʾ�ɹ���С��0ʧ��
	*/
	int  CalcBearingDefectFreq(const S_BearingPar& bearingpar_, const int& nWorkRev_, S_BearingDefectFreq& defectFreq_);

	/**@fn ApCalcBearingDefectFreq
	*@brief  �򻯽��Ƽ��������еĹ��ϣ�ͨ����Ƶ�ʣ�ע�⣺û�й������ͨ��Ƶ��
	*@param  rollingnum ������й��������
	*@param  fWorkRev_   ����ת��
	*@param  defectFreq_  [out] �������ȱ��Ƶ��
	*@return 0��ʾ�ɹ���С��0ʧ��
	*/
	int  ApCalcBearingDefectFreq(const int& rollingnum_, const int& nWorkRev_, S_BearingDefectFreq& defectFreq_);

   
	/**��������λ�û��߻�����λ�ã��й�ˮƽ����ֱ��XY������μ��ṹ��S_AxisLocationInputInfo��ע�ͣ�
	  *@param inputInfo_ �����������
	  *@param ���ñ��� fXValue_ X����ֵ����λ��mm
      *@param ���ñ��� fYValue_ Y����ֵ����λ��mm
	  *@return 0��ʾ�ɹ���С��0ʧ��
	  */
	int CalcAxisCenterLocation(const S_AxisLocationInputInfo& inputInfo_, double& fXValue_, double& fYValue_); 

public:
///��ƽ��ϵ���㷨��ǰ׺Ϊ��DSB����ʾ˫�涯ƽ�⣬��SSB����ʾ���涯ƽ��
    
	//************************************
	// Method:    SSB_CalcEffectCoef �����涯ƽ��Ӱ��ϵ���ļ���
	// FullName:  CSigMath::SSB_CalcEffectCoef
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0_    ����ǰ��ֵ����λ��um
	// Parameter: const double & fPhase0_  ����ǰ��λ����λ����(��)
	// Parameter: const double & fWeight_    ������������λ��g���ˣ�
	// Parameter: const double & fWeightPhase_ ���ؽǶȣ���λ����(��)
	// Parameter: const double & fAmp1_      ���غ��ֵ����λ��um
	// Parameter: const double & fPhase1_    ���غ���λ����λ����(��)
	// Parameter: & complex<double> & cfA_   �����Ӱ��ϵ��
	//************************************
	int SSB_CalcEffectCoef(const double& fAmp0_,const double& fPhase0_, const double& fWeight_, const double& fWeightPhase_,const double& fAmp1_,const double& fPhase1_, complex<double>& cfCoef_);

	//************************************
	// Method:    SSB_GetCounterWeight  �õ����涯ƽ������ص���������λ��Ϣ
	// FullName:  CSigMath::SSB_GetCounterWeight
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0_  ԭʼ�񶯷�ֵ����λ��um
	// Parameter: const double & fPhase0_ ԭʼ����λ����λ����(��)
	// Parameter: const complex<double> & cfCoef_  Ӱ��ϵ��
	// Parameter: const bool & bRotationClockwise_ ��ת�����Ƿ�Ϊ˳ʱ����ת������������������˿���
	// Parameter: double & fWeight_       ���ص���������λһ���ͼ���Ӱ��ϵ��ʱ�����ص�������ͬ
	// Parameter: double & fWeightPhase_  ���ص���λ����λ����(��)
	//************************************
	int SSB_GetCounterWeight(const double& fAmp0_,const double& fPhase0_,const complex<double>& cfCoef_, const bool& bRotationClockwise_,double& fWeight_, double& fWeightPhase_);

    //************************************
    // Method:    DSB_CalcEffectCoef   ˫�涯ƽ��4��Ӱ��ϵ���ļ��㡣0�ˣ�ָ���������ˣ�1�ˣ�ָԶ��������
    // FullName:  CSigMath::DSB_CalcEffectCoef
    // Access:    public 
    // Returns:   int
    // Qualifier:
    // Parameter: const double & fAmp0      0��ԭʼ�񶯷�ֵ����λ��um
    // Parameter: const double & fPhase0_   0��ԭʼ����λ����λ����(��)
    // Parameter: const double & fAmp1      1��ԭʼ�񶯷�ֵ����λ��um
    // Parameter: const double & fPhase1_   1��ԭʼ����λ����λ����(��)
    // Parameter: const double & fWeight0_       ��0��������������λ��g���ˣ�
    // Parameter: const double & fWeightPhase0_  ��0��������λ����λ����(��)
    // Parameter: const double & fAmp00      ��0������ʱ��0���񶯷�ֵ����λ��um
    // Parameter: const double & fPhase00_   ��0������ʱ��0������λ����λ����(��)
    // Parameter: const double & fAmp10      ��0������ʱ��1���񶯷�ֵ����λ��um
    // Parameter: const double & fPhase10_   ��0������ʱ��1������λ����λ����(��)
    // Parameter: const double & fWeight1_       ��1��������������λ��g���ˣ�
    // Parameter: const double & fWeightPhase1_  ��1��������λ����λ����(��)
    // Parameter: const double & fAmp01      ��1������ʱ��0���񶯷�ֵ����λ��um
    // Parameter: const double & fPhase01_   ��1������ʱ��0������λ����λ����(��)
    // Parameter: const double & fAmp11      ��1������ʱ��1���񶯷�ֵ����λ��um
    // Parameter: const double & fPhase11_   ��1������ʱ��1������λ����λ����(��)
    // Parameter: complex<double> & cfCoef00_  0�˶�0�˵�Ӱ��ϵ��
    // Parameter: complex<double> & cfCoef10_  0�˶�1�˵�Ӱ��ϵ��
    // Parameter: complex<double> & cfCoef01_  1�˶�0�˵�Ӱ��ϵ��
    // Parameter: complex<double> & cfCoef11_  1�˶�1�˵�Ӱ��ϵ��
    //************************************
	int DSB_CalcEffectCoef(const double& fAmp0,const double& fPhase0_,const double& fAmp1,const double& fPhase1_, const double& fWeight0_,const double& fWeightPhase0_,const double& fAmp00,const double& fPhase00_,const double& fAmp10,const double& fPhase10_, const double& fWeight1_,const double& fWeightPhase1_,const double& fAmp01,const double& fPhase01_,const double& fAmp11,const double& fPhase11_, complex<double>& cfCoef00_, complex<double>& cfCoef10_,complex<double>& cfCoef01_,complex<double>& cfCoef11_);
   

	//************************************
	// Method:    DSB_GetCounterWeight   �õ�˫�涯ƽ���0��1�����ص���������λ��Ϣ��0�ˣ�ָ���������ˣ�1�ˣ�ָԶ��������
	// FullName:  CSigMath::DSB_GetCounterWeight
	// Access:    public 
	// Returns:   int
	// Qualifier:
	// Parameter: const double & fAmp0      0��ԭʼ�񶯷�ֵ����λ��um
    // Parameter: const double & fPhase0_   0��ԭʼ����λ����λ����(��)
    // Parameter: const double & fAmp1      1��ԭʼ�񶯷�ֵ����λ��um
    // Parameter: const double & fPhase1_   1��ԭʼ����λ����λ����(��)
	// Parameter: const complex<double> & cfCoef00_  0�˶�0�˵�Ӱ��ϵ��
	// Parameter: const complex<double> & cfCoef10_  0�˶�1�˵�Ӱ��ϵ��
	// Parameter: const complex<double> & cfCoef01_  1�˶�0�˵�Ӱ��ϵ��
	// Parameter: const complex<double> & cfCoef11_  1�˶�1�˵�Ӱ��ϵ��
	// Parameter: const bool & bRotationClockwise_   ��ת�����Ƿ�Ϊ˳ʱ����ת������������������˿���
	// Parameter: double & fWeight0_             0�����ص���������λһ���ͼ���Ӱ��ϵ��ʱ�����ص�������ͬ
	// Parameter: double & fWeightPhase0_        0�����ص���λ����λ����(��)
	// Parameter: double & fWeight1_             1�����ص���������λһ���ͼ���Ӱ��ϵ��ʱ�����ص�������ͬ
	// Parameter: double & fWeightPhase1_        1�����ص���λ����λ����(��)
	//************************************
	int DSB_GetCounterWeight(const double& fAmp0_,const double& fPhase0_,const double& fAmp1_,const double& fPhase1_, const complex<double>& cfCoef00_, const complex<double>& cfCoef10_,const complex<double>& cfCoef01_,const complex<double>& cfCoef11_,const bool& bRotationClockwise_, double& fWeight0_, double& fWeightPhase0_,double& fWeight1_, double& fWeightPhase1_);  

public: ///������������
	/**computes the peak-peak value of  data sequence ��Ч���ֵ
	*@param: pfwave_ denotes data sequence
	*@param: iLen_  denotes the length of data sequence
	*@return: Peak-Peak value
	*/
	float CalcPeak_PeakValue(const float *pfWave_,const int&  iLen_);
	// ��Ч���ֵ
	double CalcPeak_PeakValue(const double *pfWave_,const int& iLen_);
    

	/**c����ָ�����ݵľ������ƽ��ֵ
	*@param: pdata_ ����
	*@param: ilen_  ���ݳ���
	*@param: fstd_ ���ؾ�����ֵ
	*@param: fmean_ ����ƽ��ֵ
	*@return: С��0ʧ�ܡ�0��ʾ�ɹ�
	*/
	int CalcStdMean(const double *pdata_,const int& ilen_, double& fstd_, double& fmean_);

	///��������ֵ
    double FeatureValue(const double *pfWave_,const int& iLen_,E_FeatureValueType featureType_);

	/**�ж�ָ��Ƶ��������Χ�Ƿ���������׷塣
	  *@param pSpectrum_ Ƶ������
	  *@param ilen_   pfWave_�����ݸ���
	  *@param iSpecInd_  ָ������
	  *@param iSideRange_  ��ָ��������������Χ�� iSpecInd_-iSideRange_, iSpecInd+iSideRange_
      *@return С��0:����ִ��ʧ��; 0: ���ǲ�����; ����0:Ϊ�׷��Ӧ����
	  */
    int IsExistSpectrumPeak(const double *pSpectrum_, const unsigned int& ilen_, const unsigned int& iSpecInd_, const unsigned int& iSideRange_  = 2 );

	///��ʱ����һЩ����
	double Test1(double * p1, double * p2_,int i1_);
private:
    /**�����۹�ʽ�����㡢����ָ����ɷֵ�����ȫ�ײ�������Ƶ��������������Բ�뾶��
	  *@param vSigParamV_ ��ֱ������ź���Ϣ
	  *@param vSigParamH_ ��ֱ������ź���Ϣ
	  *@param mapFullSpecInfo_ ȫ�׵���Ϣ������ֵΪ��Ƶֵ����f/f0����secondΪ��Ӧ��ֵ��������ǰ���ⲿ���ڴ����á�����ΪvSigParamV_���ȵ�����
	  */
	int FullSpectrumParaNFreq(const float& f0_,const vector<SSigParam>& vSigParamV_, const vector<SSigParam>& vSigParamH_, vector<SSigParam>& mapFullSpecInfo_);

	/*! ��ʱ�����н��мӴ���ֱ�Ӷ�ָ��ָ���ʱ�����н����޸ĵġ�
	  !@param  *pwave, �����ʱ�����У�ֱ�Ӷ�������н��мӴ�.
	  !@param n,ʱ�����г���
	  !@param type ���Ӵ�������
	  */
	void  WindowedWave(double *pwave,int n,int type=WINDOW_HANNING);

	/**filter 
	  *@brief: cascaded filter stage algorithm using direct-form equations
	  *@param: pfwave_ denotes data sequence
	  *@param: iWaveLen_ denotes the length of data
	  *@param: ppfB_(ns*3) denotes the filter coefficients of numerator in each second-order stage  
	  *@param: ppfA_(ns*2) denotes the filter coefficients of denominator in each second-order stage
	  *@param: iNs_  denotes number of second-order stage
	  *@param: pfOut_ denotes data sequence of filtered
	  *@return: successed(true) of failed(false)
	  */
	bool filterIIR_C(double *pfWave_,int iWaveLen_,double **ppfB_,double **ppfA_,double *pfOut_,int iNs_);
	bool filterIIR_C2(float *pfWave_,int iWaveLen_,float **ppfB_,float **ppfA_,float *pfOut_,int iNs_);
public:
	int writefile(const float * ag_fwave, int ag_num,int index);
	int writefile(const double * ag_fwave, int ag_num,int index);
	int writefile(const vector<double> vData, int index_);

private:///һЩ���źŴ����޹صĹ���������
	///ת������������Ϊ˫��������
	int FaToDa(const float *pfInput_, double * pdOutput_,int ilen_);
	///ת��˫��������Ϊ����������
	int DaToFa(const double *pfInput_, float * pfOutput_,int ilen_);
};
