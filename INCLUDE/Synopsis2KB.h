#pragma once
/**@brief 完成知识库所需要的特征。特征的值与知识库里面的值是一一对应关系 
  *@author 马波
  */

#ifdef  SYNOPSIS2KB_EXPORTS
#define CLASS_DLL_DSE_SYN2KB  __declspec(dllexport)
#else
#define CLASS_DLL_DSE_SYN2KB  __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib,"Synopsis2Kbd.lib")
#else
#pragma comment(lib,"Synopsis2Kb.lib")
#endif  //_DEBUG
#endif///SYNOPSIS2KB_EXPORTS

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "Sigdefine.h"
#include "ExsysVarDefine.h"
#include "DSE_Define.h"

///主要针对离心压缩机
class CLASS_DLL_DSE_SYN2KB CSynopsis2KB
{
public:
	CSynopsis2KB();
	~CSynopsis2KB();
public:
	/**得到主导频率和常伴频率
      *@param  
	  *@param  iLeadFreqType_ 主导频率对应的值（为知识库中的定义）
	  *@param  iAccomFreqType_ 主导频率对应的值（为知识库中的定义）
      *@param  iPer_      用于判断常伴频率，指主导频率的该百分比以下不考虑为常伴频率。
	  *@param  iTh_       用于判断常伴频率，指如果幅值小于该值，不考虑为常伴频率。
	  *@return 小于0表示失败
      */
	int GetLeadFreqAndAccomFreq(const double *pWave_,  const int& iLen_,const int& iFs_, const double& f0_, int & iLeadFreqType_, int& iAccomFreqType_,const int& iPer_ = 30, const int& iTh_ = 5);

	/**得到轴心轨迹的形状和进动方向。旋转的方向是从H->V 
	  *@param  
	  *@param  iPrecessionDirect_ 1倍频的进动方向	2表示正进动，1表示反进动，0表示不确定。
	  *@param  iOrbitType_ 轴心轨迹形状对应的值（为知识库中的定义）
	  *@return 小于0表示失败
	  */
	int GetOrbitShape(const double *pWaveV_, const double *pWaveH_, const int& ilen_,double f0_, const int& iFs_, int & iPrecessionDirect_, int& iOrbitType_);
};

///机泵、风机、轴承、齿轮箱等诊断
class CLASS_DLL_DSE_SYN2KB CDiagnosisFanPump
{
public:
	CDiagnosisFanPump();
	~CDiagnosisFanPump();
public:///机泵专家系统使用的接口。必须是加速度信号才可以。该public下面函数都需要调用
///监测点是指当前选择进行专家诊断的测点
	
	/**设置监测点所在位置的轴承类型。如果是滚动轴承，则设置滚动轴承参数
	  *@param bearingType_  轴承类型
	  *@param bearingpar_   轴承参数。如果是滑动轴承，就顺便设个值就行，函数中不使用
	  *@param iImpellerBlaseNums_ 叶轮的数目。未知就是-1
	  *@return 小于0：设置有误。0表示OK
	  */
    int SetMeasurePtBearingInfo(E_BearingType bearingType_,S_BearingPar bearingpar_, int iImpellerBlaseNums_ = -1);

	/**设置机组是否有齿轮箱，含有滚动轴承，如果有齿轮箱，并设置监测点与齿轮箱的关系。没有就可以不设置。
      *@param bHaveGearBox_ 是否有齿轮箱
	  *@param eMeasureLocation_   监测点与齿轮箱的位置关系
	  *@return 小于0：设置有误。0表示OK
	  */
	int IsHaveGearBoxRollBearing(bool bHaveGearBox_,bool bHaveRollBearing_, E_MeasurePtandGearBoxRelation eMeasureLocation_ = E_MeasurePtandGearBoxRelation_NearRollBearing);

	/**机组如果有齿轮箱，设置齿轮箱的所有（因为可能是多级齿轮）啮合频率，没有的话，可以不用调用该函数
	*@param vMeshFreq_ 啮合频率向量
	*@return 小于0：设置有误。0表示OK
	*/
	int SetGearBoxMeshFreq(const vector<double>& vMeshFreq_);

	/**监测点位置附近的情况
	  *@param bNearCoupling_  true：靠近联轴节，false 不靠近
	  *@param bNearImpeller_   true：靠近叶轮，false 不靠近
	  *@return 小于0：设置有误。0表示OK
	  */
	int SetMeasurePtNearConditon(bool bNearCoupling_ , bool bNearImpeller_ = false);

	/**监测点的加速度信号。必须是加速度
	*@param pData_  加速度时域波形数据
	*@param ilen_   波形数据长度。即数据个数
    *@param iFs_    采样频率
    *@param f0_   工频
	*@param iCutFreq_   做包络的截止频率。如果设置为-1，则表示自动寻优
	*@return 小于0：设置有误。0表示OK
	*/
	int SetMeasurePtWave(const double *pData_, const int& ilen_, const int& iFs_, const double& f0_, const int& iCutFreq_ = 5000);

	/**监测点的gSE趋势数据，时间范围为1个月（选择点往前推1个月）。
	*@param pData_  gSE趋势数据
	*@param ilen_   趋势数据长度。即数据个数
	*@param iDays_  数据的天数。
	*@return 小于0：设置有误。0表示OK
	*/
	int SetMeasurePtgSETrend(const double *pData_, const int& ilen_, const int& iDays_);
	

	/**监测点相同位置的测点加速度信号。必须是加速度。没有，可以不调用这个函数
	  *@param pData_  加速度时域波形数据
	  *@param ilen_   波形数据长度。即数据个数
	  *@param iFs_    采样频率
	  *@param f0_   工频
	  *@param iCutFreq_   做包络的截止频率。如果设置为-1，则表示自动寻优
	  *@return 小于0：设置有误。0表示OK
	  */
	int SetMeasurePtSameLocationPtWave(const double *pData_, const int& ilen_, const int& iFs_, const double& f0_, const int& iCutFreq_ = 5000);

	/**监测点同一部位另一端的测点加速度信号。必须是加速度。没有，可以不调用这个函数
	  *@param pData_  加速度时域波形数据
	  *@param ilen_   波形数据长度。即数据个数
	  *@param iFs_    采样频率
	  *@param f0_   工频
	  *@param iCutFreq_   做包络的截止频率。如果设置为-1，则表示自动寻优
	  *@return 小于0：设置有误。0表示OK
	  */
	int SetMeasurePtSamePartPtWave(const double *pData_, const int& ilen_, const int& iFs_, const double& f0_, const int& iCutFreq_ = 5000);

	/**监测点相邻联轴节的位置测点加速度信号。必须是加速度。没有，可以不调用这个函数
	  *@param pData_  加速度时域波形数据
	  *@param ilen_   波形数据长度。即数据个数
	  *@param iFs_    采样频率
	  *@param f0_   工频
	  *@param iCutFreq_   做包络的截止频率。如果设置为-1，则表示自动寻优
	  *@return 小于0：设置有误。0表示OK
	  */
	int SetMeasurePtNearCouplingPtWave(const double *pData_, const int& ilen_, const int& iFs_, const double& f0_, const int& iCutFreq_ = 5000);

   
	/**设置机组是否存在gSE明显偏大测点，如果机组有齿轮箱，并设置该测点与齿轮箱的关系。没有就可以不设置。
	  *@param bHavegSEGreater_ 是否有gSE明显偏大测点。（值只要大于正常值的3倍，就算）
	  *@param eMeasureLocation_   监测点与齿轮箱的位置关系
	  *@return 小于0：设置有误。0表示OK
	  */
	int IsHavegSEValueGreateMeasurePt(bool bHavegSEGreater_, E_MeasurePtandGearBoxRelation eMeasureLocation_ = E_MeasurePtandGearBoxRelation_NearRollBearing);


	///上述函数设置完成后，调用该函数可以得到知识结果
	const map<string, int> GetKBResult() const;
public:

     /**轴承报警判断、故障诊断。通过波形数据、特征频率及对应阈值、通频阈值判断是否有故障
	   *@param pWave_ 原始波形数据, 指针长度为ilen_
	   *@param ilen_ 波形数据的长度（采样点数）
       *@param iFs_ 采样频率
	   *@param iCutoffFreq_ 高通滤波的截止频率
	   *@param vEigenFreq_ 故障频率向量，长度为n（故障频率的数目）
       *@param vEigenTh_   通过趋势学习得到的各个特征频率阈值向量。长度为0、n，不能为其他值。即可以不设，但是如果设置的话，必须设置都设
	   *@param iEigenFreqInd_ 引用参数。第几个故障频率发生报警
	   *@param fOverallTh_ 通过趋势学习得到的通频阈值。默认为0，即不设置
	   *@param fOverallHTh_ 通频硬阈值,即人为设置的。 默认0.5
	   *@param fFactor_    判断危险需要的系数，即大于该系数乘以阈值大小，认为危险
	   *@param fLowFactor_ 如果没有设置特征频率的阈值，使用硬阈值时，超过该值与硬阈值的大小，认为是危险。
	   *@return 小于0：计算有误。0表示没有故障，1表示判断出有故障警告,2表示危险。
	   */
	int GetgSECondition(const double* pWave_,const int& ilen_, const int& iFs_, const int& iCutoffFreq_, const vector<double>& vEigenFreq_, const vector<double>& vEigenTh_, int& iEigenFreqInd_,\
		const double& fOverallTh_ =0, const double& fOverallHTh_ = 0.5,	const double& fFactor_ = 1.5, const double& fLowFactor_ = 0.1);
	int GetgSECondition(const double* pWave_,const int& ilen_, const int& iFs_, const int& iCutoffFreq_, const vector<double>& vEigenFreq_, int& iEigenFreqInd_);

	 /**轴承齿轮箱6频带报警。（下面函数分别为加速度信号和速度信号的判断）
       *@param  pAccWave_(pVelWave_)  数据
	   *@param  iFs_ 采样频率
	   *@param  iSamples_ 采样点数
	   *@param  f0_   工频
	   *@param  machineType_ 机器类型
       *@param  vMachineState_ 必须提前分配好向量，长度为6。 6频带报警情况 1-良好，0-一般，-1-报警，-2-危险
	   */
	int RollingBearing6FreqBandCheckAlarmForAcc(const double * pAccWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int RollingBearing6FreqBandCheckAlarmForVel(const double * pVelWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int SlidingBearing6FreqBandCheckAlarmForAcc(const double * pAccWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int SlidingBearing6FreqBandCheckAlarmForVel(const double * pVelWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int Gear6FreqBandCheckAlarmForAcc(const double * pAccWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int Gear6FreqBandCheckAlarmForVel(const double * pVelWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_, vector<int>& vMachineState_);
	int Gear6FreqBandCheckAlarmForAcc(const double * pAccWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_,const double& fMeshFreq_, vector<int>& vMachineState_);
	int Gear6FreqBandCheckAlarmForVel(const double * pVelWave_, const int& iFs_, const int& iSamples_, const double& f0_,const E_PumpFanMachineType& machineType_,const double& fMeshFreq_, vector<int>& vMachineState_);
private:
	/**轴承、齿轮箱设备的6频带报警判断。必须是速度信号
	  *@param vThCoef_ 门限系数，长度必须s为6
	  *@param mapFreqband_ 6个起始频带范围，长度必须为6
	  */
    int BearingGear6FreqBandCheckAlarm(const double * pVelWave_, const int& iFs_, const int& iSamples_,const double& f0_,const vector<double>& vThCoef_,const E_PumpFanMachineType& machineType_ ,const map<double, double>& mapFreqband_, vector<int>& vMachineState_);

    /**根据包络谱，gSE值、阈值、特征频率判断是否有故障
	  % 函数功能:   子函数，基于已经预先计算好的包络谱、gSE值、
	  % 特征频率等对确信存在故障的轴承的包络谱进行诊断
	  *	% 参考特征频率，输出故障类型Fault  
	  *@param vEvnSpec_ 包络谱向量。
	  *@param df_       包络谱的频率分辨率
	  *@param vEigenFreq_ 特征频率向量，长度为n
	  *@param vEigenTh_   通过趋势学习得到的各个特征频率阈值向量。长度为0、n，不能为其他值。即可以不设，但是如果设置的话，必须设置都设
	  *@param vFaultStatus_ 导出信息，各个特征频率的故障状态 长度为n。必须事先分配好。故障状态,0:正常，1：报警，2：危险。
	  *@param vEigenAmp_ 导出信息，各个特征频率处的幅值 长度为n。必须事先分配好
	  *@param vEigenLevel_ 导出信息，各个特征频率处周围的幅度平均值。长度为n，必须事先分配好
	  *@param fOverallTh_ 通过趋势学习得到的通频阈值。默认为0，即不设置
      *@param fOverallHTh_ 通频硬阈值,即人为设置的。 默认0.5
	  *@param fFactor_    判断危险需要的系数，即大于该系数乘以阈值大小，认为危险
	  *@param fLowFactor_ 如果没有设置特征频率的阈值，使用硬阈值时，超过该值与硬阈值的大小，认为是危险。
	  *@return 小于0：计算有误。0表示没有故障，1表示判断出有故障警告,2表示危险。
	  */
   int BearingEnvSpecDiag(const vector<double>& vEnvSpec_, const float& df_, const vector<double>& vEigenFreq_, const vector<double>& vEigenTh_,\
	   vector<int>& vFaultStatus_, vector<double>& vEigenAmp_, vector<double>& vEigenLevel_, const double& fOverallTh_ =0, const double& fOverallHTh_ = 0.5,\
	   const double& fFactor_ = 1.5, const double& fLowFactor_ = 0.1); 
};


class CLASS_DLL_DSE_SYN2KB CDiagnosisRC
{
public: 
	CDiagnosisRC();
	~CDiagnosisRC();
public:
	//得到专家系统所需的变量
	const map<string,int>  GetExsysVarValueList() const ;

	/**设置单个缸体的各个监测点的报警状态(E_RC_AlarmStatus)： 0-正常，1-异常，2-不确定cylinder
	  *@param  eTempAlarmStatus_       温度的报警状态
	  *@param  eSinkAlarmStatus_,      //沉降
	  *@param  eCrossHeadAlarmStatus_, //十字头振动
	  *@param  eImpactsAlarmStatus_,   //撞击次数
	  *@param  eCrankAlarmStatus_,     //曲轴箱报警状态
	  *@param  iRev_,                //机组的转速
	  *@param  eDeflectAlarmStatus    //偏摆
	  *@param  ePVAlarmStatus_           //动态压力报警情况
      */
	void SetCylinderAlarmStatus(E_RC_AlarmStatus eTempAlarmStatus_, 
		E_RC_AlarmStatus eSinkAlarmStatus_,
		E_RC_AlarmStatus eCrossHeadAlarmStatus_,
		E_RC_AlarmStatus eImpactsAlarmStatus_, 
		E_RC_AlarmStatus eCrankAlarmStatus_,
		const unsigned int& iRev_,
		E_RC_AlarmStatus eDeflectAlarmStatus = E_RC_AlarmStatus_NoChann,
		E_RC_AlarmStatus ePVAlarmStatus_ = E_RC_AlarmStatus_NoChann 
		);

	/**设置单个缸体的各个监测点的传感器故障状态： 0-否（传感器正常），1-是（传感器故障），2-不确定
	  *@param eTempSensorFaultStatus_,     温度传感器的故障状态
	  *@param eSinkSensorFaultStatus_,      沉降传感器故障状态
	  *@param eCrossHeadSensorFaultStatus_, 十字头振动
	  *@param eImpactsSensorFaultStatus_,   撞击次数
	  *@param eCrankSensorFaultStatus_,     曲轴箱振动传感器的状态
	  *@param eDeflectSensorFaultStatus_    偏摆
	  *@param ePVSensorFaultStatus_         曲轴箱传感器的故障状态
	  */
	void SetCylinderSensorsStatus(E_BoolExType eInValTempSensorFaultStatus_, 
		E_BoolExType eOutValTempSensorFaultStatus_,
		E_BoolExType eSinkSensorFaultStatus_, 
		E_BoolExType eCrossHeadSensorFaultStatus_,
		E_BoolExType eImpactsSensorFaultStatus_,
        E_BoolExType eCrankSensorFaultStatus_,
		E_BoolExType eDeflectSensorFaultStatus_ = E_BoolExType_Unsure, 
		E_BoolExType ePVSensorFaultStatus_ = E_BoolExType_Unsure
		);

	/**设置诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数
	  *@param iSensorFaults_ 气阀传感器有故障的个数
	  *@param iAlarmChannels_ 没有传感器故障的气阀报警通道数
	  *@param iChannels_       所在缸的监测气阀温度的通道数
	  */
	void SetCylinderValueAlarmsAndSensorFaults( const unsigned int& iSensorFaults_,const unsigned int& iAlarmChannels_, const int& iChannels_);

	/**设置报警温度测点前40+后20分钟内的数据。点数不需要超过100个。没有报警，则可以不调用该函数
      *@param pfTrend_ 报警前后的趋势数据
	  *@param ilen_    趋势数据的个数，即pfTrend
	  *@param fAlarmTh_ 该温度测点的报警下限
	  *@param bInletValue_ 是否是进气阀
      */
	void SetAlarmValueTempTrend(const double* pfTrend_, const int& ilen_, double fAlarmTh_, bool bInletValue_);


	/**设置诊断测点所在缸撞击次数
	  *@param iTimes_ 撞击次数
	  *@param iTh_ 阈值
	
	void SetImpactTimes(const unsigned int& iTimes_, const unsigned int& iTh_ = 5);
  */

	////设置沉降或偏摆的数据
	/**设置报警前后1分钟内的沉降量趋势。 点数不需要超过50个。报警前后各50s+10s数据
	  *@param pfTrendData_ 趋势数据
	  *@param ilen_ 趋势点数
      *@param  fLoAlarmValue_ 报警的低限，单位一定要趋势值单位相同
	  *@param  fHiAlarmValue_ 报警的高限，单位一定要趋势值单位相同
	  *@param  fMaxRange_ 最大量程值，单位一定要趋势值单位相同
	  *@param bHiPass_ 是否高通报警
	  *@return 小于0表示设置失败
	  */
	int SetSinkTrend(const double* pfTrendData_, const int& ilen_, double fLoAlarmValue_, double fHiAlarmValue_, double fMaxRange_, bool bHiPass_);
 	
	/**设置诊断测点沉降报警时刻的波形。没有报警可以不设置
	  *@param pWave_ 波形数据
	  *@param ilen_ 波形数据点数
	  *@param iFs_ 采样频率
	  *@param fTh_  沉降量波形波动的阈值,单位是um
	  *@return 小于0表示设置失败
  */
    int SetSinkWave(const double* pWave_, int ilen_, int iFs_, double fTh_ = 300);

	/**设置报警前后1分钟内的偏摆量。 点数不需要超过50个。报警前后各50s+10s数据
	  *@param pfTrendData_ 趋势数据
	  *@param ilen_ 趋势点数
	  *@param  fLoAlarmValue_ 报警的低限，单位一定要趋势值单位相同
	  *@param  fHiAlarmValue_ 报警的高限，单位一定要趋势值单位相同
	  *@param  fMaxRange_ 最大量程值，单位一定要趋势值单位相同
	  *@param bHiPass_ 是否高通报警
	  *@return 小于0表示设置失败
	  */
	int SetDeflectTrend(const double* pfTrendData_, const int& ilen_, double fLoAlarmValue_, double fHiAlarmValue_, double fMaxRange_, bool bHiPass_);

    /**设置诊断测点偏摆报警时刻的波形
	  *@param pWave_ 波形数据
	  *@param ilen_ 波形数据点数
	  *@param iFs_ 采样频率
	  *@param fTh_  偏摆量波形波动的阈值 单位是um
	  *@return 小于0表示设置失败
	  */
    int SetDeflectWave(const double* pWave_, int ilen_, int iFs_, double fTh_ = 300);


		/**设置诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形
	  *报警前1小时的波形
	  *@param pWaveBeforeAlarm_ 报警前的波形数据
	  *@param pWavelenBeforeAlarm_ 报警前波形数据的长度
	  *@param iStartIndBeforeAlarm_ 报警后波形的一个周期的起始索引
	  *@param iEndIndBeforeAlarm_  报警后波形的一个周期的结束索引
	  *@param pWaveAfterAlarm_ 报警后的波形数据
	  *@param pWavelenAfterAlarm_ 报警后波形的长度
	  *@param iStartIndAfterAlarm_ 报警后波形的一个周期的起始索引
	  *@param iEndIndAfterAlarm_ 报警后波形一个周期的结束索引
	  *@param iFsAfterAlarm_ 报警后波形的采样频率
	  *@param fLoAlarmValue_  报警下线（低报）
	  *@param fHiAlarmValue_  报警上限（高报）
	  *@return 小于0表示设置失败
	  */
	int SetCrossHeadWave(const double *pWaveBeforeAlarm_, int iWavelenBeforeAlarm_, int iStartIndBeforeAlarm_, int iEndIndBeforeAlarm_, const double *pWaveAfterAlarm_, int iWavelenAfterAlarm_, int iStartIndAfterAlarm_, int iEndIndAfterAlarm_, int iFsAfterAlarm_, double fLoAlarmValue_, double fHiAlarmValue_);

	/**设置诊断所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周，则取1周内的距离报警时刻最早的数据)的波形
	  *报警前1周的波形
	  *@param pWaveBeforeAlarm_ 报警前的波形数据
	  *@param pWavelenBeforeAlarm_ 报警前波形数据的长度
	  *@param iStartIndBeforeAlarm_ 报警后波形的一个周期的起始索引
	  *@param iEndIndBeforeAlarm_  报警后波形的一个周期的结束索引
	  *@param pWaveAfterAlarm_ 报警后的波形数据
	  *@param pWavelenAfterAlarm_ 报警后波形的长度
	  *@param iStartIndAfterAlarm_ 报警后波形的一个周期的起始索引
	  *@param iEndIndAfterAlarm_ 报警后波形一个周期的结束索引
	  *@param iFsAfterAlarm_ 报警后波形的采样频率
	  *@param fLoAlarmValue_  报警下线（低报）
	  *@param fHiAlarmValue_  报警上限（高报）*@return 小于0表示设置失败
	  */
	int SetCrankCaseWave(const double *pWaveBeforeAlarm_, int iWavelenBeforeAlarm_, int iStartIndBeforeAlarm_, int iEndIndBeforeAlarm_, const double *pWaveAfterAlarm_, int iWavelenAfterAlarm_, int iStartIndAfterAlarm_, int iEndIndAfterAlarm_, int iFsAfterAlarm_, double fLoAlarmValue_, double fHiAlarmValue_);

	/**设置机组转速范围
	*@param arvRevs_ 机组当前转速
	*@param anRatedSpeed 机组额定转速
	*/
	int SetMachineRev(const int anCurRev_, const int anRatedRev_);

	/**是否进行气阀温度传感器异常诊断
	*@param abFaultDiagnosis_ 是否进行诊断
	*@param abInVal_  是否是吸气阀
	*/
	int SetValveFalutDiagnosis(bool abFaultDiagnosis_,bool abInVal_);

	/**设置气阀温度报警
	*@param apTempValue_ 温度趋势数据
	*@param anLen_       数据个数
	*@param abInVal_     是否是吸气阀
	*/
	int SetValveTempAlarm(const double * apTempValue_, int anLen_,bool abInVal_);

	/**是否进行十字头冲击传感器异常诊断
	*@param abFaultDiagnosis_ 是否进行诊断
	*/
	int SetCrossHeadFaultDiagnosis(bool abFaultDiagnosis_);

	/**设置十字头冲击峰峰值以及波形直流量
	*@param apWave_      波形数据
	*@param anLen_       波形数据个数
	*/
	int SetCrossHeadPeakValueAndDC(const double * apWave_, const int anLen_);

	/**是否进行活塞杆偏摆传感器异常诊断
	*@param abFaultDiagnosis_ 是否进行诊断
	*/
	int SetRodDeflectFaultDiagnosis(bool abFaultDiagnosis_);

	/**设置活塞杆偏摆峰峰值以及直流量趋势
	*@param apPeackValue_ 峰峰值趋势数据
	*@param apDC_         直流量趋势数据
	*@param anLen_        数据长度
	*/
	int SetRodDeflectInfo(const double * apPeackValue_,const double * apDC_, const int anLen_);

	/**是否进行活塞杆沉降传感器异常诊断
	*@param abFaultDiagnosis_ 是否进行诊断
	*/
	int SetRodSinkFaultDiagnosis(bool abFaultDiagnosis_);

	/**设置活塞杆沉降峰峰值以及间隙电压趋势
	*@param apPeackValue_ 峰峰值趋势数据
	*@param apGap_        间隙电压趋势数据
	*@param anLen_        数据长度
	*/
	int SetRodSinkInfo(const double * apPeackValue_,const double * apGap_, const int anLen_);

	/**是否进行曲轴箱传感器异常诊断
	*@param abFaultDiagnosis_ 是否进行诊断
	*/
	int SetCrankFaultDiagnosis(bool abFaultDiagnosis_);

	/**设置曲轴箱振动波形直流量（即波形平均值）
	*@param apWave_      波形数据
	*@param anLen_       波形数据个数
	*/
	int SetCrankWaveDC(const double * apWaveDC_,const int anLen_);

	/**设置曲轴箱有效值趋势增量
	*@param adbRMSValue_      有效值趋势增量
	*/
	int SetCrankRMSVariation(const double adbRMSValue_);

	/**设置曲轴箱有效值趋势增量
	*@param adbRMSValue_      有效值趋势数据
	*@param anLen_            数据长度
	*/
	int SetCrankRMSValue(const double * apRMSValue_, const int anLen_);
};