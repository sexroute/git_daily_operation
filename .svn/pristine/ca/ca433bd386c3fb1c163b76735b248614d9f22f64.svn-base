/*!\file KDFilter.h
*	\brief 信号处理算法库dll的接口声明
*	\author 康育哲
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

/// 滤波器类
/** 用于对各种机械的振动信号和往复式机械的动态压力信号做滤波处理，计算各种特征值
*/
class _BHKD_FILTERLIB_API_ CKDFilter
{
public:
	/// 窗类型
	enum WindowType
	{
		RECTANGULAR,	///< 矩形窗
		HANNING,			///< 汉宁窗
		LOWSIDELOBE	///< 低旁瓣
	};

	/// 平均化类型
	enum AveragingType
	{
		LINEAR,				///< 线性
		EXPONENTIAL		///< 指数
	};

	/// 特征值类型
	enum EVType
	{
		PEAK2PEAK,		///< 峰峰值
		PEAK,					///< 峰值
		RMS						///< 有效值
	};

	//! \name 振动信号滤波算法
	//@{

	/// 去直流
	/** 计算去直流的波形数据，并获取直流量(重载1)
	*	\param[in] vfWave_ 时域信号波形(含直流)
	*	\param[out] vfWaveNoDc_ 无直流波形
	*	\return 直流量
	*/
	static float RemoveDirectCurrent(const CNiReal32Vector &vfWave_, CNiReal32Vector &vfWaveNoDc_);

	/// 去直流
	/** 计算去直流的波形数据，并获取直流量(重载2)
	*	\param[in/out] vfWave_ 输入为带直流信号，输出为无直流信号
	*	\return 直流量
	*/
	static float RemoveDirectCurrent(CNiReal32Vector &vfWave_);

	/// 整周期去直流
	/** 计算去直流的波形数据，并获取直流量(重载3)
	*	\param[in] fFreq_ 时域信号频率
	*	\param[in] vfWave_ 时域信号波形(含直流)
	*	\param[out] vfWaveNoDc_ 无直流波形
	*	\return 直流量
	*/
	static float RemoveDirectCurrent(const float fFreq_, const int iSampleRate_, const CNiReal32Vector &vfWave_, CNiReal32Vector &vfWaveNoDc_);

	/// 转速转频率
	/**
	*	\param[in] rpm_ 转速(单位：r/min)
	*	\return 频率(单位：Hz)
	*/
	static float RpmToHz(float rpm_);

	/// 频率转转速
	/**
	*	\param[in] fHz_ 频率(单位：Hz)
	*	\return 转速(单位：r/min)
	*/
	static float HzToRpm(float fHz_);

	/// 求信号峰峰值
	/**
	*	\param[in] vfWave_ 时域信号波形
	*	\param[in] b3Delta_ 是否用3-delta法
	*	\return 峰峰值
	*/
	static float GetPeak2Peak(const CNiReal32Vector &vfWave_, bool b3Delta_ = false, int iblock_ = 3, int ibias = 0.1);

	/// 搜索峰值频率
	/** 在信号的某频谱范围内搜索峰值
	*	\param[in] vfAf_ 频谱波形
	*	\param[in] fDf_ 频谱分辨率
	*	\param[in] fLowFreq_ 待搜索频谱范围的下限
	*	\param[in] fHighFreq_ 待搜索频谱范围的上限
	*	\param[in] fLowVal_ 可接受峰值下限
	*	\param[in] fHighVal_ 可接受峰值上限
	*	\param[out] fFp_ 峰值对应的频率值
	*	\param[out] fPeak_ 峰值
	*	\return 是否搜到峰值
	*/
	static bool DetectPeak(const CNiReal32Vector & vfAf_, float fDf_, float fLowFreq_, float fHighFreq_, float fLowVal_, float fHighVal_, float &fFp_, float &fPeak_);

	/// 单边FFT
	/** 做单边FFT，输出频谱点数为输入信号点数的一半
	*	\param[in] vfWave_ 时域信号波形
	*	\param[out] vfFft_ 频谱
	*	\param[in] ev_ 特征值类型
	*	\return		无
	*/
	static void SingleSideFFT(const CNiReal32Vector &vfWave_, CNiReal32Vector &vfFft_, EVType ev_ = PEAK2PEAK);

	/// 计算任意倍频的幅值
	/** 根据给定工频值计算信号倍频幅值
	*	\param[in] nivfAf_ 信号频谱
	*	\param[in] fDf_ 频率分辨率
	*	\param[in] fF0_ 工频值
	*	\param[in] fMulti_ 倍频因子
	*	\param[in] nWidth_ 频率成分搜索宽度(双边)
	*	\param[out] fFreq_ 实际搜索到的倍频值
	*	\return 倍频幅值
	*/
	static float GetFreqAmplitude(CNiReal32Vector & nivfAf_, float fDf_, float fF0_, float fMulti_, int nWidth_, float &fFreq_);

	/// 计算残振值
	/** 根据给定工频值计算信号残振值
	*	\param[in] nivfAf_ 信号频谱
	*	\param[in] fDf_ 频率分辨率
	*	\param[in] fF0_ 工频值
	*	\param[in] nWidth_ 频率成分搜索宽度(双边)
	*	\return 残振值
	*/
	static float GetResVib(const CNiReal32Vector & nivfAf_, float fDf_, float fF0_, int nWidth_);
	
	/// 求信号均方根
	/**
	*	\param[in] vfWave_ 时域信号波形
	*	\param[in] window_ 窗函数类型
	*		- Rectangular 矩形窗
	*		- Hanning 汉宁窗
	*		- LowSideLobe 低旁瓣窗
	*	\param[in] avg_ 平均化类型
	*		- Linear 线性型
	*		- Exponential 指数型
	*	\return 均方根
	*/
	static float GetRMS(const CNiReal32Vector &vfWave_, CKDFilter::WindowType window_ = CKDFilter::RECTANGULAR, CKDFilter::AveragingType avg_ = CKDFilter::LINEAR);

	/// 求相位
	/** 利用输入信号和标准正弦信号的互功率谱来求某一频率分量(参考频率)的相位
	*	\param[in] vfWave_ 时域信号波形
	*	\param[in] fDt_ 时间分辨率
	*	\param[in] fRefFreq_ 参考频率
	*	\return 相位值
	*/
	static float GetPhase(const CNiReal32Vector &vfWave_, float fDt_, float fRefFreq_);

	/// 获取常规角度
	/*	*任意角度转换标准角度(0~360度之间)
	*	\param[in] fDeg_ 角度值
	*	\return 0~360°之间的角度
	*/
	static float GetRegularDegree(float fDeg_);

	/// 计算峭度
	/**
	*	\param[in] vfWave_ 时域信号波形
	*	\return 峭度
	*/
	static float GetKurtosis(const CNiReal32Vector & vfWave_);

	/// 计算歪度
	/**
	*	\param[in] vfWave_ 时域信号波形
	*	\return 歪度
	*/
	static float GetSkewness(const CNiReal32Vector & vfWave_);

	/// 计算互谱
	/** 求两个信号的互功率谱
	*	\param[in] vfWave1_ 时域信号1
	*	\param[in] vfWave2_ 时域信号2
	*	\param[in] fDt_ 时域分辨率
	*	\param[out] vfAf_ 幅频波形
	*	\param[out] vfPhf_ 相频波形
	*	\return 频域分辨率
	*/
	static float ComputeCrossSpectrum(const CNiReal32Vector &vfWave1_, const CNiReal32Vector &vfWave2_, float fDt_, CNiReal32Vector &vfAf_, CNiReal32Vector &vfPhf_);

	/// 去除工频干扰
	/** 滤除信号中某个频率分量的干扰
	*	\param[in/out] vfWave_ 输入为原始时域信号，输出为滤波后的时域信号
	*	\param[in] fDt_ 信号时间间隔
	*	\param[in] fF0_ 干扰频率
	*	\param[out] vfAf_ 滤波后的频谱
	*	\param[in] ev_ 特征值类型
	*	\return 无
	*/
	static void RemoveF0Component(CNiReal32Vector &vfWave_, float fDt_, float fF0_, CNiReal32Vector &vfAf_, EVType ev_ = PEAK2PEAK);
	//@}

	//!\name 往复压缩机动态测点信号滤波算法
	//@{

	/// 计算活塞行程
	/** 计算某一时刻活塞的行程，最小行程定义为0，注意输入量纲的一致
	*	\param[in] fLen_ 连杆长度
	*	\param[in] fRadius_ 轮毂半径
	*	\param[in] fTime_ 采样时刻
	*	\param[in] fF0_ 转频
	*	\return 行程
	*/
	static float GetPistonMoveLength(float fLen_, float fRadius_, float fTime_, float fF0_);

	/// 计算活塞沉降量
	/** 根据传感器监测的活塞杆沉降量来计算活塞的沉降量(活塞杆的最大沉降值)，注意输入量纲的一致
	*	\param[in] fSink_ 传感器监测的沉降量
	*	\param[in] fMove_ 活塞行程
	*	\param[in] fPisRodLen_ 活塞杆长度
	*	\param[in] fPisThick_ 活塞厚度
	*	\param[in] fSenPos_ 传感器位置
	*	\return 活塞沉降量
	*/
	static float GetPistonSinkVal(float fSink_, float fMove_, float fPisRodLen_, float fPisThick_, float fSenPos_);

	/// 计算动态特征值
	/** 计算部分动态压力特征值
	*	\param[in] vfPress_ 压力数组，截断的数组
	*	\param[in] vfVol_ 体积数组，截断的数组
	*	\param[in] fInPress_ 名义进气压力
	*	\param[in] fOutPress_ 理论排气压力
	*	\param[in] fClrVol_ 余隙容积
	*	\param[out] fVEin_ 吸气容积效率
	*	\param[out] fVEout_ 排气容积效率
	*	\param[out] fCI_ 压缩指数
	*	\param[out] fEC_ 膨胀系数
	*	\param[out] fPR_ 实际压力比
	*	\return 无
	*/
	static void ComputeDynValues(const CNiReal32Vector &vfPress_, const CNiReal32Vector &vfVol_, float fInPress_, float fOutPress_, float fClrVol_,\
		float &fVEin_, float &fVEout_, float &fCI_, float &fEC_, float &fPR_);

	/// 计算单缸指示功率
	/** 输入的压力和体积数组必须是单周期的
	*	\param[in] vfPress_ 压力数组(KPa)
	*	\param[in] vfVol_ 体积数组(m^3)
	*	\param[in] fF0_ 转频(Hz)
	*	\return 单缸指示功率(kW)
	*/
	static float GetSingleCylinderPower(const CNiReal32Vector &vfPress_, const CNiReal32Vector &vfVol_, float fF0_);

	/// 计算机械效率
	/**
	*	\param[in] fSnglPower_ 单缸指示功率(kW)
	*	\param[in] fTotalPower_ 总功率(kW)
	*	\return 机械效率(%)
	*/
	static float GetMachineEfficiency(float fSnglPower_, float fTotalPower_);

	/// 计算所需周期的点数(不满所需周期点数时，直接返回采样点数)
	/**
	*	\param[in] iSampleNum 采样点数
	*	\param[in] iSampleRate 采样率
	*	\param[in] fFreq 信号频率
	*	\param[in] iAllPeriodNum_ 周期数
	*   \param[in] bAutoPeriod_自动控制周期数标志
	*	\return 点数
	*/
	static int ComputePeriodNum(const int iSampleNum_, const int iSampleRate_, const float fFreq_, const int iAllPeriodNum_, bool bAutoPeriod_ = false);

    /// 重采样
    /** 如果期望采样频率小于实际采样频率，则可以进行重采样
    *	\param[in] nivfData_ 波形数据
    *	\param[in] nExpectedSampRate_ 期望采样频率
    *	\param[in] nActualSampRate_ 实际采样频率
    *	\param[in] fCutoffFreq_ 截止频率（抗混滤波用）
    *	\param[in] nOrder_ 阶次（抗混滤波用）
    *   \param[in] nExpectedSampNum_ 期望采样点数：<0表示最大限度的重采样
    *	\return 无
    */
    static bool Resample(CNiReal32Vector &nivfData_, int nActualSampRate_, int nExpectedSampRate_, float fCutoffFreq_, int nOrder_ = 4, int nExpectedSampNum_ = -1);;

    /// 检查浮点数是否异常
    static bool IsFloatValid(float fInput_);

	//@}

protected:
	/// PV对
	struct PVPair
	{
		float pressure;	///< 压力
		float volume;	///< 体积
	};

	//!\name 内部使用的函数
	//@{

	/// 计算PV对
	/** 4组PV对是示功图的4个拐点，必须保证输入的nivfPress_和nivfVol_的长度为单周期的点数
	*	\param[in] nivfPress_ 单周期的压力值数组
	*	\param[in] nivfVol_ 单周期的体积值数组
	*	\param[in] fInPress_ 理论进气压力
	*	\param[in] fOutPress_ 理论排气压力
	*	\param[out] pv1 示功图右下点
	*	\param[out] pv2 示功图右上点
	*	\param[out] pv3 示功图左上点
	*	\param[out] pv4 示功图左下点
	    \param[in] c_fTh 压力阈值1
        \param[in] c_fTh2 压力阈值2
	*	\return 相对余隙容积
	*/
    static float ComputePVPairs(const CNiReal32Vector &nivfPress_, const CNiReal32Vector &nivfVol_, float fInPress_, float fOutPress_, PVPair &pv1_, PVPair &pv2_, PVPair &pv3_, PVPair &pv4_, float fTh_ = 10, float fTh2_ = 100);

	/// 计算压缩指数
	static float GetCompressIndex(const PVPair &pv1_, const PVPair &pv2_);

	/// 计算吸气容积效率
	static float GetInVolumeEfficiency(const PVPair &pv1_, const PVPair &pv3_, const PVPair &pv4_);

	/// 计算排气容积效率
	static float GetOutVolumeEfficiency(const PVPair &pv1_, const PVPair &pv2_, const PVPair &pv3_);

	/// 计算实际压力比
	static float GetPressRatio(const PVPair &pv1_, const PVPair &pv2_);

	/// 计算膨胀系数
	static float GetExpandCoefficient(const PVPair &pv1_, const PVPair &pv3_, const PVPair &pv4_, float fInPress_, float fOutPress_, float fRCV_);

	//@}
};
