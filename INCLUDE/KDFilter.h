/*!\file KDFilter.h
*	\brief �źŴ����㷨��dll�Ľӿ�����
*	\author ������
*	\version 1.0
*/

#pragma once

#ifdef _BHKD_DLLEXPORT_H_
#define _BHKD_FILTERLIB_API_ __declspec(dllexport)
#else  
#define _BHKD_FILTERLIB_API_ __declspec(dllimport) 
#ifdef _DEBUG
#pragma comment(lib, "KDFilterLibD.lib")
#else
#pragma comment(lib, "KDFilterLib.lib")
#endif
#endif

/// �˲�����
/** ���ڶԸ��ֻ�е�����źź�����ʽ��е�Ķ�̬ѹ���ź����˲����������������ֵ
*/
class _BHKD_FILTERLIB_API_ CKDFilter
{
public:
	/// ������
	enum WindowType
	{
		RECTANGULAR,	///< ���δ�
		HANNING,			///< ������
		LOWSIDELOBE	///< ���԰�
	};

	/// ƽ��������
	enum AveragingType
	{
		LINEAR,				///< ����
		EXPONENTIAL		///< ָ��
	};

	/// ����ֵ����
	enum EVType
	{
		PEAK2PEAK,		///< ���ֵ
		PEAK,					///< ��ֵ
		RMS						///< ��Чֵ
	};

	//! \name ���ź��˲��㷨
	//@{

	/// ȥֱ��
	/** ����ȥֱ���Ĳ������ݣ�����ȡֱ����(����1)
	*	\param[in] vfWave_ ʱ���źŲ���(��ֱ��)
	*	\param[out] vfWaveNoDc_ ��ֱ������
	*	\return ֱ����
	*/
	static float RemoveDirectCurrent(const CNiReal32Vector &vfWave_, CNiReal32Vector &vfWaveNoDc_);

	/// ȥֱ��
	/** ����ȥֱ���Ĳ������ݣ�����ȡֱ����(����2)
	*	\param[in/out] vfWave_ ����Ϊ��ֱ���źţ����Ϊ��ֱ���ź�
	*	\return ֱ����
	*/
	static float RemoveDirectCurrent(CNiReal32Vector &vfWave_);

	/// ������ȥֱ��
	/** ����ȥֱ���Ĳ������ݣ�����ȡֱ����(����3)
	*	\param[in] fFreq_ ʱ���ź�Ƶ��
	*	\param[in] vfWave_ ʱ���źŲ���(��ֱ��)
	*	\param[out] vfWaveNoDc_ ��ֱ������
	*	\return ֱ����
	*/
	static float RemoveDirectCurrent(const float fFreq_, const int iSampleRate_, const CNiReal32Vector &vfWave_, CNiReal32Vector &vfWaveNoDc_);

	/// ת��תƵ��
	/**
	*	\param[in] rpm_ ת��(��λ��r/min)
	*	\return Ƶ��(��λ��Hz)
	*/
	static float RpmToHz(float rpm_);

	/// Ƶ��תת��
	/**
	*	\param[in] fHz_ Ƶ��(��λ��Hz)
	*	\return ת��(��λ��r/min)
	*/
	static float HzToRpm(float fHz_);

	/// ���źŷ��ֵ
	/**
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\param[in] b3Delta_ �Ƿ���3-delta��
	*	\return ���ֵ
	*/
	static float GetPeak2Peak(const CNiReal32Vector &vfWave_, bool b3Delta_ = false, int iblock_ = 3, int ibias = 0.1);

	/// ������ֵƵ��
	/** ���źŵ�ĳƵ�׷�Χ��������ֵ
	*	\param[in] vfAf_ Ƶ�ײ���
	*	\param[in] fDf_ Ƶ�׷ֱ���
	*	\param[in] fLowFreq_ ������Ƶ�׷�Χ������
	*	\param[in] fHighFreq_ ������Ƶ�׷�Χ������
	*	\param[in] fLowVal_ �ɽ��ܷ�ֵ����
	*	\param[in] fHighVal_ �ɽ��ܷ�ֵ����
	*	\param[out] fFp_ ��ֵ��Ӧ��Ƶ��ֵ
	*	\param[out] fPeak_ ��ֵ
	*	\return �Ƿ��ѵ���ֵ
	*/
	static bool DetectPeak(const CNiReal32Vector & vfAf_, float fDf_, float fLowFreq_, float fHighFreq_, float fLowVal_, float fHighVal_, float &fFp_, float &fPeak_);

	/// ����FFT
	/** ������FFT�����Ƶ�׵���Ϊ�����źŵ�����һ��
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\param[out] vfFft_ Ƶ��
	*	\param[in] ev_ ����ֵ����
	*	\return		��
	*/
	static void SingleSideFFT(const CNiReal32Vector &vfWave_, CNiReal32Vector &vfFft_, EVType ev_ = PEAK2PEAK);

	/// �������ⱶƵ�ķ�ֵ
	/** ���ݸ�����Ƶֵ�����źű�Ƶ��ֵ
	*	\param[in] nivfAf_ �ź�Ƶ��
	*	\param[in] fDf_ Ƶ�ʷֱ���
	*	\param[in] fF0_ ��Ƶֵ
	*	\param[in] fMulti_ ��Ƶ����
	*	\param[in] nWidth_ Ƶ�ʳɷ��������(˫��)
	*	\param[out] fFreq_ ʵ���������ı�Ƶֵ
	*	\return ��Ƶ��ֵ
	*/
	static float GetFreqAmplitude(CNiReal32Vector & nivfAf_, float fDf_, float fF0_, float fMulti_, int nWidth_, float &fFreq_);

	/// �������ֵ
	/** ���ݸ�����Ƶֵ�����źŲ���ֵ
	*	\param[in] nivfAf_ �ź�Ƶ��
	*	\param[in] fDf_ Ƶ�ʷֱ���
	*	\param[in] fF0_ ��Ƶֵ
	*	\param[in] nWidth_ Ƶ�ʳɷ��������(˫��)
	*	\return ����ֵ
	*/
	static float GetResVib(const CNiReal32Vector & nivfAf_, float fDf_, float fF0_, int nWidth_);
	
	/// ���źž�����
	/**
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\param[in] window_ ����������
	*		- Rectangular ���δ�
	*		- Hanning ������
	*		- LowSideLobe ���԰괰
	*	\param[in] avg_ ƽ��������
	*		- Linear ������
	*		- Exponential ָ����
	*	\return ������
	*/
	static float GetRMS(const CNiReal32Vector &vfWave_, CKDFilter::WindowType window_ = CKDFilter::RECTANGULAR, CKDFilter::AveragingType avg_ = CKDFilter::LINEAR);

	/// ����λ
	/** ���������źźͱ�׼�����źŵĻ�����������ĳһƵ�ʷ���(�ο�Ƶ��)����λ
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\param[in] fDt_ ʱ��ֱ���
	*	\param[in] fRefFreq_ �ο�Ƶ��
	*	\return ��λֵ
	*/
	static float GetPhase(const CNiReal32Vector &vfWave_, float fDt_, float fRefFreq_);

	/// ��ȡ����Ƕ�
	/*	*����Ƕ�ת����׼�Ƕ�(0~360��֮��)
	*	\param[in] fDeg_ �Ƕ�ֵ
	*	\return 0~360��֮��ĽǶ�
	*/
	static float GetRegularDegree(float fDeg_);

	/// �����Ͷ�
	/**
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\return �Ͷ�
	*/
	static float GetKurtosis(const CNiReal32Vector & vfWave_);

	/// �������
	/**
	*	\param[in] vfWave_ ʱ���źŲ���
	*	\return ���
	*/
	static float GetSkewness(const CNiReal32Vector & vfWave_);

	/// ���㻥��
	/** �������źŵĻ�������
	*	\param[in] vfWave1_ ʱ���ź�1
	*	\param[in] vfWave2_ ʱ���ź�2
	*	\param[in] fDt_ ʱ��ֱ���
	*	\param[out] vfAf_ ��Ƶ����
	*	\param[out] vfPhf_ ��Ƶ����
	*	\return Ƶ��ֱ���
	*/
	static float ComputeCrossSpectrum(const CNiReal32Vector &vfWave1_, const CNiReal32Vector &vfWave2_, float fDt_, CNiReal32Vector &vfAf_, CNiReal32Vector &vfPhf_);

	/// ȥ����Ƶ����
	/** �˳��ź���ĳ��Ƶ�ʷ����ĸ���
	*	\param[in/out] vfWave_ ����Ϊԭʼʱ���źţ����Ϊ�˲����ʱ���ź�
	*	\param[in] fDt_ �ź�ʱ����
	*	\param[in] fF0_ ����Ƶ��
	*	\param[out] vfAf_ �˲����Ƶ��
	*	\param[in] ev_ ����ֵ����
	*	\return ��
	*/
	static void RemoveF0Component(CNiReal32Vector &vfWave_, float fDt_, float fF0_, CNiReal32Vector &vfAf_, EVType ev_ = PEAK2PEAK);
	//@}

	//!\name ����ѹ������̬����ź��˲��㷨
	//@{

	/// ��������г�
	/** ����ĳһʱ�̻������г̣���С�г̶���Ϊ0��ע���������ٵ�һ��
	*	\param[in] fLen_ ���˳���
	*	\param[in] fRadius_ ��챰뾶
	*	\param[in] fTime_ ����ʱ��
	*	\param[in] fF0_ תƵ
	*	\return �г�
	*/
	static float GetPistonMoveLength(float fLen_, float fRadius_, float fTime_, float fF0_);

	/// �������������
	/** ���ݴ��������Ļ����˳���������������ĳ�����(�����˵�������ֵ)��ע���������ٵ�һ��
	*	\param[in] fSink_ ���������ĳ�����
	*	\param[in] fMove_ �����г�
	*	\param[in] fPisRodLen_ �����˳���
	*	\param[in] fPisThick_ �������
	*	\param[in] fSenPos_ ������λ��
	*	\return ����������
	*/
	static float GetPistonSinkVal(float fSink_, float fMove_, float fPisRodLen_, float fPisThick_, float fSenPos_);

	/// ���㶯̬����ֵ
	/** ���㲿�ֶ�̬ѹ������ֵ
	*	\param[in] vfPress_ ѹ�����飬�ضϵ�����
	*	\param[in] vfVol_ ������飬�ضϵ�����
	*	\param[in] fInPress_ �������ѹ��
	*	\param[in] fOutPress_ ��������ѹ��
	*	\param[in] fClrVol_ ��϶�ݻ�
	*	\param[out] fVEin_ �����ݻ�Ч��
	*	\param[out] fVEout_ �����ݻ�Ч��
	*	\param[out] fCI_ ѹ��ָ��
	*	\param[out] fEC_ ����ϵ��
	*	\param[out] fPR_ ʵ��ѹ����
	*	\return ��
	*/
	static void ComputeDynValues(const CNiReal32Vector &vfPress_, const CNiReal32Vector &vfVol_, float fInPress_, float fOutPress_, float fClrVol_,\
		float &fVEin_, float &fVEout_, float &fCI_, float &fEC_, float &fPR_);

	/// ���㵥��ָʾ����
	/** �����ѹ���������������ǵ����ڵ�
	*	\param[in] vfPress_ ѹ������(KPa)
	*	\param[in] vfVol_ �������(m^3)
	*	\param[in] fF0_ תƵ(Hz)
	*	\return ����ָʾ����(kW)
	*/
	static float GetSingleCylinderPower(const CNiReal32Vector &vfPress_, const CNiReal32Vector &vfVol_, float fF0_);

	/// �����еЧ��
	/**
	*	\param[in] fSnglPower_ ����ָʾ����(kW)
	*	\param[in] fTotalPower_ �ܹ���(kW)
	*	\return ��еЧ��(%)
	*/
	static float GetMachineEfficiency(float fSnglPower_, float fTotalPower_);

	/// �����������ڵĵ���(�����������ڵ���ʱ��ֱ�ӷ��ز�������)
	/**
	*	\param[in] iSampleNum ��������
	*	\param[in] iSampleRate ������
	*	\param[in] fFreq �ź�Ƶ��
	*	\param[in] iAllPeriodNum_ ������
	*   \param[in] bAutoPeriod_�Զ�������������־
	*	\return ����
	*/
	static int ComputePeriodNum(const int iSampleNum_, const int iSampleRate_, const float fFreq_, const int iAllPeriodNum_, bool bAutoPeriod_ = false);

    /// �ز���
    /** �����������Ƶ��С��ʵ�ʲ���Ƶ�ʣ�����Խ����ز���
    *	\param[in] nivfData_ ��������
    *	\param[in] nExpectedSampRate_ ��������Ƶ��
    *	\param[in] nActualSampRate_ ʵ�ʲ���Ƶ��
    *	\param[in] fCutoffFreq_ ��ֹƵ�ʣ������˲��ã�
    *	\param[in] nOrder_ �״Σ������˲��ã�
    *   \param[in] nExpectedSampNum_ ��������������<0��ʾ����޶ȵ��ز���
    *	\return ��
    */
    static bool Resample(CNiReal32Vector &nivfData_, int nActualSampRate_, int nExpectedSampRate_, float fCutoffFreq_, int nOrder_ = 4, int nExpectedSampNum_ = -1);;

    /// ��鸡�����Ƿ��쳣
    static bool IsFloatValid(float fInput_);

	//@}

protected:
	/// PV��
	struct PVPair
	{
		float pressure;	///< ѹ��
		float volume;	///< ���
	};

	//!\name �ڲ�ʹ�õĺ���
	//@{

	/// ����PV��
	/** 4��PV����ʾ��ͼ��4���յ㣬���뱣֤�����nivfPress_��nivfVol_�ĳ���Ϊ�����ڵĵ���
	*	\param[in] nivfPress_ �����ڵ�ѹ��ֵ����
	*	\param[in] nivfVol_ �����ڵ����ֵ����
	*	\param[in] fInPress_ ���۽���ѹ��
	*	\param[in] fOutPress_ ��������ѹ��
	*	\param[out] pv1 ʾ��ͼ���µ�
	*	\param[out] pv2 ʾ��ͼ���ϵ�
	*	\param[out] pv3 ʾ��ͼ���ϵ�
	*	\param[out] pv4 ʾ��ͼ���µ�
	    \param[in] c_fTh ѹ����ֵ1
        \param[in] c_fTh2 ѹ����ֵ2
	*	\return �����϶�ݻ�
	*/
    static float ComputePVPairs(const CNiReal32Vector &nivfPress_, const CNiReal32Vector &nivfVol_, float fInPress_, float fOutPress_, PVPair &pv1_, PVPair &pv2_, PVPair &pv3_, PVPair &pv4_, float fTh_ = 10, float fTh2_ = 100);

	/// ����ѹ��ָ��
	static float GetCompressIndex(const PVPair &pv1_, const PVPair &pv2_);

	/// ���������ݻ�Ч��
	static float GetInVolumeEfficiency(const PVPair &pv1_, const PVPair &pv3_, const PVPair &pv4_);

	/// ���������ݻ�Ч��
	static float GetOutVolumeEfficiency(const PVPair &pv1_, const PVPair &pv2_, const PVPair &pv3_);

	/// ����ʵ��ѹ����
	static float GetPressRatio(const PVPair &pv1_, const PVPair &pv2_);

	/// ��������ϵ��
	static float GetExpandCoefficient(const PVPair &pv1_, const PVPair &pv3_, const PVPair &pv4_, float fInPress_, float fOutPress_, float fRCV_);

	//@}
};
