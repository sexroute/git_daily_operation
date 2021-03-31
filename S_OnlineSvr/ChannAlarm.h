#ifndef _BHKD_CHANNALARM_H_MARBLE_
#define _BHKD_CHANNALARM_H_MARBLE_

#include "VibData.h"
#include "OnlineGlobal.h"
#include "../include/DieselPlantAlarm.h"
const float G_F_INIT_VALUE = -12345678;
enum _CHANN_ALARM_STATUS_
{
	CHANN_ALARM_STATUS_INIT                   = 0x00000000,
	CHANN_ALARM_STATUS_NORMAL                 = 0x00000001,
	CHANN_ALARM_STATUS_HALF_NORMAL            = 0x00000010,
	CHANN_ALARM_STATUS_OLD_ALARM_LOW		  = 0x00000100,
	CHANN_ALARM_STATUS_OLD_ALARM_HIGH		  = 0x00001000,
	CHANN_ALARM_STATUS_NEW_ALARM			  = 0x00010000,
};

class CVibChannParamAlarm: public CVibChannParam
{
public:
	CVibChannParamAlarm();
	~CVibChannParamAlarm();
private:
	//是否处于初次报警状态
	bool _bFirstAlarmS;
	//是否处于报警状态
    bool _bAlarmStatusS;
	//是否学习过门限
	bool _bLearnTh;
	//是否报警结束后触发一次自学习
	bool _bAlarmLearnTh;
	//累计报警次数，如果该累计次数达到指定数目，就认为处于报警状态
	short _siAlarmCountsS[10];
	//累计次数。初次报警后，判断报警的累计次数
	short _siCountsS[10];
	//新产生的缓变报警累计次数。如果该累计次数达到指定数目，就认为是相似的一次报警
	int _siNewAlarmCountsS;
	//测点缓变报警状态
	int _iSlowStatus;
    //测点趋势报警状态
	int _iTrendStatus;
	//快变报警状态
	int _iFastStatus;
	//根据通道类型判断报警的字节位
	int _iAlarmBit;
	//常规报警判断
	int m_nRegStatus;
    ///常规报警判断标志。-1表示没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
    int _iRegAlarmFlag;
	//常规报警:上次触发一次报警时刻的特征值
	float _fRegValueLast;
	float m_fLastAlarmValue;

	//常规报警:上次触发一次新报警时刻的时间
	CBHDateTime _timeNewAlarmLast;

	//自定义报警判断
	int _iCustomStatus;
	///自定义报警判断标志。-1表示没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
	int _iCustomAlarmFlag;
	//自定义报警:上次触发一次新报警时刻的时间
	CBHDateTime _timeNewCustomAlarmLast;


	//当前缓变报警事件ID
	long _iEventIDSlow;
	long _iIDSlow;
	//当前快变报警事件ID
	long _iEventIDFast;
	//当前快变阈值ID
	long _iCurrThIDFast;
	//当前缓变阈值ID
	long _iCurrThIDSlow;
	//缓变报警判断标志。-1表示没有报警；0：当前缓变报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
	int _iSlowAlarmFlag;
    //缓变报警的结束时间
	CBHDateTime _tEndSlowAlarmTime;

	LPVOID _pParent;

	//缓变报警当前自学习获得的阈值
	CThreshold _pthSlow[D_BHKD_ALARM_CHARTYPE_NUM];
	CThreshold _pthFast[D_BHKD_ALARM_CHARTYPE_NUM];

	//缓变报警上一次自学习获得的阈值
	CThreshold _pthSlowLast[D_BHKD_ALARM_CHARTYPE_NUM];

    //当前时间
	CBHDateTime _timeCurr;
	//上次学习缓变门限阈值参数时间
    CBHDateTime _timeSlowThLearn;
	//上次学习快变门限阈值参数时间
	CBHDateTime _timeFastThLearn;
    //波形数据缓存的指针
 	DATATYPE_WAVE*  _ppfWaveBuf[D_ALARM_SAVEWAVE_NUM];
    //当前波形数据指针的位置
	int _iCurrWavePIndex;
	//当前特征值位置
	int _iCurrCharValueIndex;
    //特征值缓存数组
	S_VibCharValue _pCharValueBuf[D_ALARM_SAVECHAR_NUM];
	//特征值对应的时间缓存.对波形数据来说,最新的D_ALARM_SAVEWAVE_NUM这么多个时间就对应波形数据缓存
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];

	//和趋势报警判断相关参数
	//上次检查小时趋势报警的时间
	CBHDateTime _timeTrendCheckHour;
	//上次检查天趋势报警的时间
	CBHDateTime _timeTrendCheckDay;
    //上次检查月趋势报警的时间
	CBHDateTime _timeTrendCheckMonth;
	//上次检查季趋势报警的时间
	CBHDateTime _timeTrendCheckSeason;

	typedef vector<S_VibCharValue> VECTOR_VIBCHAR;
	//趋势判断使用数据缓存
	VECTOR_VIBCHAR _vVibCharValue;
//和报警保存相关参数
	//初次开始保存报警数据时间
	CBHDateTime _timeSlowFirstSave;
    //上次保存缓变报警数据的时间
	CBHDateTime _timeSlowSaveData;
    //数据库连接指针
//	CBHOleDbWrapper *_pDB;

	//快变报警状态
	int m_nFastRegStatus;

private:
	/**To judge alarm of slow change针对径向振动
	*@return 如果返回0:说明没有报警,1:存在报警;2:有新的报警需要重新开始5分钟数据保存;3:刚刚添加了报警日志,需要重新开始5分钟数据保存
	*/
	int JudgeRadialVibSlowAlarm(IDBInterFace *pDB_);
	//针对轴向振动
	int JudgeAxisVibSlowAlarm(IDBInterFace *pDB_);

    //判断指定相位值和阈值判断其是否报警
	bool CheckSpecPhaseValueAlarm(float fValue_,CThreshold th_);
	//判断指定相位值和上次相位值,判断其是否需要触发新报警
	bool CheckPhaseIFNewAlarm(float fValue_,float fLastValue_,CThreshold th_);

	//径向振动，缓变报警状态下的：5分钟保存和20分钟保存等相关判断操作
    int RadialSlowAlarmStatusOP();

	//轴向振动，缓变报警状态下的：5分钟保存和20分钟保存等相关判断操作
	int AxisSlowAlarmStatusOP();

	//更新报警状态并判断是否有新的报警产生,针对径向振动
	bool JudgeRadialSlowNewAlarm();

	//更新报警状态并判断是否有新的报警产生,针对轴向振动
	bool JudgeAxisSlowNewAlarm();

	/**判断各个特征值的报警状态,返回的就是报警状态，针对径向振动
      *@param iAlarmNum_ 各种特征值报警的数目
	  *@param bHaveNew_ 判断是否有新报警产生，通过当前数据和上次数据的差值是否超过指定百分比.是否有新报警,结果通过这个变量传出.
	  *@param bCheckNew_ 是否检查有新报警产生
	  *@return 特征值的报警状态组合
	  */
	int JudgeRadialSlowCharValue(int *iAlarmNum_,int iCharNum_,bool &bHaveNew_,bool bCheckNew_=false);

	/**判断各个特征值的报警状态,返回的就是报警状态，针对轴向振动
      *@param iAlarmNum_ 各种特征值报警的数目
	  *@param bHaveNew_ 判断是否有新报警产生，通过当前数据和上次数据的差值是否超过指定百分比.是否有新报警,结果通过这个变量传出.
	  *@param bCheckNew_ 是否检查有新报警产生
	  *@return 特征值的报警状态组合
	  */
	int JudgeAxisSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_=false);

	/**@brief 判断在报警状态下,上次值和当前值之差是否变化超过指定比例
	  *@param fLast_ 上次值
	  *@param fCurr_ 当前值
	  *@return 如果超过指定比例就返回true，否则返回false
	  */
	bool CheckLastAndCurrentValue(float fLast_,float fCurr_);
 	//保存波形数据
	//bool SaveData(long lEventID_,int iLocationValue_,int iLocationWave_,string sTime_,int iSmpNum_,int iSmpFreq_,float fRev_,int iAlarmType_,CBHOleDbWrapper *pDB_,string sTblName_);

	//添加缓变报警日志
	int AddSlowAlarmLog(IDBInterFace *pDB_);
    ///添加常规报警日志
	int AddRegAlarmLog(IDBInterFace *pDB_);
	///添加自定义报警日志
	int AddCustomAlarmLog(IDBInterFace *pDB_);
	///添加快变报警日志
	int AddFastAlarmLog(IDBInterFace *pDB_);
	//To judge alarm of fast change
	int JudgeFastAlarm();


    /**缓变门限学习等处理。把学习后结果赋给当前缓变门限，并在门限表中插入这条记录
	  *如果学习成功就返回新学门限ID
	  */
	int LearnAddSlowThreshold(IDBInterFace *pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//快变门限学习
	int LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_);

	/**幅值缓变门限学习，同时学习所有判断报警的特征值
	  *@param iLearnDataLen_ 学习使用的数据长度
	  *@param pVibValue_ 学习使用的数据
	  *@return 返回如果大于0，说明学习成功，小于0说明学习失败
	  */
    int LearnSlowThreshold(int iLearnDataLen_,S_VibCharValue *pVibValue_);

	/**判断指定时间间隔趋势报警
	  *@return 返回值为各种特征值（1X、2X、3X等）类型报警的总和
	  */
	int JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_);

	/**@brief 从历史数据趋势库读取指定的数据,数据保存在类的成员变量"_vVibCharValue"中
	  *@param timeStart_ 历史数据的起始时刻
	  *@param timeEnd_ 历史数据的终止时刻
	  *@return 读取历史数据的记录条数
	  */
	int ReadHisData(IDBInterFace *pDB_,CBHDateTime timeStart_,CBHDateTime timeEnd_);
	//判断数据的趋势是否报警
	int JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//判断数据的趋势是否报警
	int JudgePhaseTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//判断指定特征值类型的趋势数据报警
	int JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_);
	//添加趋势报警日志,返回的就是事件的ID
	int AddTrendAlarmLog(IDBInterFace *pDB_,int iStatus_,int iTrendType_);

	/**@brief 判断常规数据是否报警
	  *@return 如果高限报警就返回2，低限报警就返回1，不报警就返回0
	  */
	int JudgeRegDataAlarm(IDBInterFace *pDB_,bool bConfirm_ = true);



	/**当常规报警结束后，进行的相关的操作
	  */
	int HandleRegAlarmStop(IDBInterFace *pDB_);

	/**当自定义报警结束后，进行的相关的操作
	*/
	int HandleCustomAlarmStop(IDBInterFace *pDB_);

	/**@brief 发生报警后，需要对当前的阈值进行修改，通过这个函数进行操作
	  *@param th_ 阈值
	  *@param bPhase_ 是否是相位阈值
	  *@return 修改后的阈值
	  */
	CThreshold ModifyVibThS(CThreshold th_,bool bPhase_=false);
public:
	//判断快变报警
	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,S_VibCharValue& CharValue_);

	/**@brief 判断常规报警
	  *@param iFlag_ 引用变量。是否是第一次报警，如果是第一次就大于0，负责小于0
	  @param bSaveAlarmDataFlag_ 引用变量。添加一次报警ID或则特征值比之前特征值大时，是否连续保存数据标识
	  @param bSaveOneAlarmData_ 引用变量。间隔30分钟后，是否保存数据标识
	  *@return 返回值为报警状态
	  */
	int JudgeRegAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_,bool bConfirmAlarm_);






	/**@brief 判断自定义报警
	*@param iFlag_ 引用变量。是否是第一次报警，如果是第一次就大于0，负责小于0
	*@return 返回值为报警状态
	*/
	int JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveLastAlarmData_);

	/**@brief 根据报警方式，比较两次报警时刻的特征值，判断重复报警
	*@param fCurrValue_ 当前报警时刻的特征值
	@param fValueLast_ 上次报警时刻的特征值
	*@return 返回状态
	*/
	bool JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_);


	/**@brief 根据报警方式，比较两次报警时刻的特征值，判断是否超过重置设备计时器的阈值
	*@param fCurrValue_ 当前报警时刻的特征值
	@param fValueLast_ 上次报警时刻的特征值
	*@return 返回状态
	*/
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);
	bool JudgeRegAgainAlarmForConfirm();
	/**重置常规报警状态
      */
	void ResetRegAlarmStatus();

	/**重置自定义报警状态
	*/
	void ResetCustomAlarmStatus();

	/**重置缓变报警状态
	*/
	void ResetSlowAlarmStatus();

	//当报警结束后，进行的相关的操作
	int HandleSlowAlarmStop(IDBInterFace *pDB_);

    //To judge alarm of trend
	int JudgeTrendAlarm(IDBInterFace * pDb_);
	/**To judge alarm of slow change
	  *@return 如果返回0:说明没有报警,1:存在报警;2:有新的报警需要重新开始5分钟数据保存;3:刚刚添加了报警日志,需要重新开始5分钟数据保存
	  */
	int JudgeSlowAlarm(IDBInterFace *pDB_);
	//To judge all alarm
	int JudgeAlarm();

	void SetParentPoint(LPVOID pParent_);

	/**@brief 得到当前测点报警状态
	   *@param pAlarmState_ 返回的报警的详细状态
	  *@return 测点报警状态
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///设置当前通道的报警状态
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	/**@得到当前测点缓变报警状态
      *@return 测点报警状态
	  */
	int GetSlowAlarmStatus();

	/**@得到当前测点常规报警状态
	  *@return 测点常规报警状态
	  */
	int GetRegAlarmStatus();

	/**@得到当前测点自定义报警状态
	*@return 测点自定义报警状态
	*/
	int GetCustomAlarmStatus();

	/**@得到当前测点快变报警状态
	*@return 测点快变报警状态
	*/
	int GetFastAlarmStatus();

	virtual bool SetChannValue(BHKD::S_VibCharValue *pVibCharValue_);

	/**@brief 设置当前的通道的波形数据
	  *@param pfWave_ 波形数据指针
	  *@param iSmpNum_ 波形数据采样点数
	  *@return true denotes successed, or false denotes failed
	  */
	virtual bool SetChannWave(DATATYPE_WAVE *pfWave_,int iSmpNum_);

	/**@brief 得到指定通道的波形数据指针
	  *@param iIndex_ 位置索引
	  *@return 通道波形数据指针
	  */
	const DATATYPE_WAVE * GetWavePoint(int iIndex_);

	/**@brief 得到当前通道波形数据指针
	  *@param 位置索引
	  *@return 通道波形数据指针
	  */
	virtual const DATATYPE_WAVE * GetCurrWavePoint();
	/**@brief 指定波形数据缓存的指针.这个指针指向内存有把D_ALARM_SAVEWAVE_NUM*iSmpNum_个数据,把其分别分配给当前测点波形数据缓存
	  *@param pWave_ 波形数据缓存指针
	  *@param iSmpNum 波形数据的采样点数
	  */
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);

	/**@brief 得到指定索引的直流量和间隙电压
	  *@return 如果返回值为-1，说明没有得到
	  */
	int GetDcAndGap(int index_,float &fDC_,float &fGap_);

	/**@brief 得到指定索引、类型的特征值
	  *@param iCharType_ 特征值类型
	  *@return 特征值
	  */
	DATATYPE_CHAR GetSpecCharTypeValue(int index_,int iCharType_=GE_ALLCHAR);

	/**@brief 得到当前直流量和间隙电压
	  *@return 如果返回值为-1，说明没有得到
	  */
	int GetCurrDcAndGap(float &fDC_,float &fGap_);

    /**@brief 得到当前指定类型的特征值
	  *@param iCharType_ 特征值类型
	  *@return 特征值
	  */
	DATATYPE_CHAR GetCurrSpecCharTypeValue(int iCharType_=GE_ALLCHAR);

	/**@brief 得到指定索引的特征值
	  *@return 特征值
	  */
	S_VibCharValue GetCharValue(int index_);
	S_VibCharValue GetCharValue();
	virtual  S_VibCharValue GetCharValueEx() ;
	virtual  S_VibCharValue GetCharValueEx(int index_) ;
	/**得到当前特征值的位置索引
	  */
	int GetCurrCharValueIndex();

	/**得到当前波形数据的位置索引
	  */
	int GetCurrWaveIndex();

	/**得到指定索引时间
	  */
	string GetSpecIndexTime(int index_);

	/**时间转化为时间字符串(加引号的)
	  *@param time_ 时间
	  *@return 返回字符串
	  */
	CString TimeConvertString(CBHDateTime time_);

	/************************************************************************/
	/* 是否需要计算特征值                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 特征值是否没有计算                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 计算特征值															*/
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType);
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anIndex);


	/************************************************************************/
	/* 计算通频值															*/
	/************************************************************************/
	virtual float ComputeAllCharValue(S_VibCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 计算轴位移测点用来存储数据的三倍频相位值								*/
	/************************************************************************/
	virtual float ComputeThreePhaseCharValue(S_VibCharValue  & arefSVibCharValue);


	int JudgeRegDataAlarm(S_VibCharValue & arefVibValue);

	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	float LastDieselAlarmValue() const;
	void LastDieselAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	//获取报警类型
	int GetChannRegAlarmType();

	/************************************************************************/
	/* 新常规报警逻辑变量													*/
	/************************************************************************/
public:
	
	_CHANN_ALARM_STATUS_ PredictChannAlarm(IDBInterFace * pDb_,
											bool abPlantAlarmed,//isPlantPreviousAlarmed 当前设备是否报警
											bool abConfirm = TRUE,
											bool abChangeStatus = TRUE);
											

	/************************************************************************/
	/* 上次报警的最大值														*/
	/************************************************************************/
	

	long m_fLastMaxAlarmValue;
	
	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* 上次报警值															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* 上次报警均值                                                         */
	/************************************************************************/
	volatile float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* 更新报警均值                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*测点半正常计时器，默认超过1个小时就算完全正常                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* 内部使用的测点报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);
	
	
	/************************************************************************/
	/* 切换测点内部报警状态                                                 */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus);

	/************************************************************************/
	/* 预测的测点内部报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDb_);

	/************************************************************************/
	/* 连续超过上次报警值的累计次数                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* 设备是否发生了新的报警                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);

	/************************************************************************/
	/* 初始化上次报警值，上次报警最大值等值                                 */
	/************************************************************************/
	BOOL InitValues();

	/************************************************************************/
	/*轴位移新报警逻辑                                                        */
	/************************************************************************/
	BOOL JudgeAndDoForNewAxisLogic(BOOL abRunningStatusChanged);//判断并处理新的轴位移逻辑

	float m_fAxisValue;
	void  AxisOriginal(float val);

	float AxisOriginal();

	BOOL SaveAxisValueForNewAxisLogic();

	BOOL InitFileNameForNewAxisAlarmLogic();					//初始化轴位移文件名

	float GetAxisValueForNewAxisAlarmLogic();

	CString m_strAxisValueFilePath;								//存放轴位移值的文件路径
	CString AxisFilePath() const;
	void AxisFilePath(CString val);

	CBHDateTime m_oTimerForAxisValueLogic;
	CBHDateTime TimerForAxisValueLogic() const;
	void TimerForAxisValueLogic(CBHDateTime val);
	void ResetTimerForAxisValueLogic();
	BOOL IsReachSaveTimeIntervalForAxisValueLogic(); //是否达到了保存波形的时间

	//测点触发新报警时的值
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }

	///柴油机常规报警逻辑
public:

	///判断当前数据是否报警
	int JudgeDiselAlarm(IDBInterFace *pDB_, 
		CDieselPlant *pDieselPlant_,
		bool bConfirmAlarm_,
		const vector<int>& avFlameOutCylinderNo_,
		E_MACHINE_TYPE aeMachineType_);

	_CHANN_ALARM_STATUS_ PredictDieselChannAlarm(IDBInterFace * pDb_,
		bool abPlantAlarmed,//isPlantPreviousAlarmed 当前设备是否报警
		const vector<int>& avFlameOutCylinderNo_,
		bool abConfirm /*=TRUE*/,
		bool abChangeStatus/*=TRUE*/);

	_CHANN_ALARM_STATUS_ JudgeAlarmByOtherPlant(  IDBInterFace * pDb_,
		float fCharValue_,
		CBHDateTime tUpdateTime_,
		const map<CBHDateTime,float>& mCharValue_,
		bool abConfirm = TRUE,
		bool abChangeStatus = TRUE);

	CBHDateTime LastCompareAlarmTime();
	void LastCompareAlarmTime(const CBHDateTime atAlarmTime_);

	CBHDateTime LastAlarmTime();
	void LastAlarmTime(const CBHDateTime atAlarmTime_);

	int AlarmCount();
	void AlarmCount(const int anAlarmCount_);
	int IncreaseAlarmCount();
	int DecreaseAlarmCount();

	void SetMultiPointStatus(_CHANN_ALARM_STATUS_ nStatus_);
	_CHANN_ALARM_STATUS_ GetMultiPointStatus();

	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);
	float CalcMeanValue(const vector<float>& vData_);
	bool IsSignalError();
private:
	//上次报警时间
	CBHDateTime  m_dtLastAlarmTime;

	CBHDateTime m_dtLastCompareAlarmTime;
	//上次报警类型
	int         m_nLastAlarmType;

	//报警次数
	int         m_nAlarmCount;

	E_ALARM_LEVEL m_nAlarmLevel;

	_CHANN_ALARM_STATUS_ m_nMultiPointStatus;

	float m_fLastDieselAlarmValue;
};	


class CProcChannParamAlarm: public CProcChannParam
{
public:
	CProcChannParamAlarm();
	~CProcChannParamAlarm();
public:
	//是否已经学习过阈值参数
	bool _bLearnTh;
	//是否处于初次报警状态
	bool _bFirstAlarmS;
	//是否处于报警状态
	bool _bAlarmStatusS;
	//累计报警次数，如果该累计次数达到指定数目，就认为处于报警状态
	short _siAlarmCountsS;
	//累计次数。初次报警后，判断报警的累计次数
	short _siCountsS;
	//测点缓变报警状态
	int _iSlowStatus;
    //测点常规报警状态
    int _iRegStatus;
	///常规报警保存的事件ID,如果值为-1，说明目前没有报警，或者可以认为没有添加过报警日志
	int _iEventIDReg;

	//根据通道类型判断报警的字节位
	int _iAlarmBit;
    //缓变判断的阈值参数
	CThreshold _thSlow;
	//当前缓变报警事件ID及ID
	long _iEventIDSlow;
    long _iIDSlow;
	//当前缓变门限的ID
    long _iThIDSlow;

	//特征值缓存数组和对应时间缓存
	float _pfValueBuf[D_ALARM_SAVECHAR_NUM];
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];
	//当前特征值位置
	int _iCurrCharValueIndex;

	//和报警保存相关参数
	//初次开始保存报警数据时间
	CBHDateTime _timeFirstSaveS;
	//上次保存缓变报警数据的时间
	CBHDateTime _timeSaveDataS;
	//上次学习缓变门限阈值参数时间
	CBHDateTime _timeThLearnS;

	//常规报警初次开始保存报警数据时间
	CBHDateTime _timeFirstSaveReg;
	//上次保存常规报警数据的时间
	CBHDateTime _timeSaveDataReg;

	//当前时间
	CBHDateTime _timeCurr;
	//父级连接指针
	LPVOID _pParent;

	float m_fRegValueLast;
	float m_fLastAlarmValue;

	//快变报警状态
	int m_nFastRegStatus;

	int m_nProcSUDStatus;//-1:停车 0:未判断 1：开车

private:
	//To judge alarm of slow change
	int JudgeSlowAlarm(IDBInterFace * pDb_);
	//缓变报警状态下的：5分钟保存和20分钟保存等相关判断操作
	int SlowAlarmStatusOP(IDBInterFace * pDb_);
	//更新报警状态并判断是否有新的报警产生
	bool JudgeNewAlarm();
	//保存数据
	bool SaveData(IDBInterFace * pDB_,int iEventID_,int iValueIndex_,int iAlarmStatus_,string& sChannNo_,CBHDateTime& tSaveTime_);
	//添加缓变报警日志
	int AddSlowAlarmLog(IDBInterFace * pDB_);

	///当缓变报警结束后，进行的相关的操作
	int HandleSlowAlarmStop(IDBInterFace * pDB_);
	//缓变门限学习
	int LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//判断是否报警
	int JudgeRegDataAlarm(bool bConfirm_ = true);
	//判断缓变数据报警
	int CheckSlowDataAlarm();
	/**时间转化为时间字符串(加引号的)
	*@param time_ 时间
	*@return 返回字符串
	*/
	CString TimeConvertString(CBHDateTime time_);




public:

	///判断快变数据报警
	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_,float fCharValue_);
	//To judge all alarm
	int JudgeAlarm(IDBInterFace * pDb_,bool bSaveData_ ,bool bConfirmAlarm_ );

	///判断常规报警
	int JudgeRegAlarm(IDBInterFace *pDB_,bool bSaveData_,bool bConfirmAlarm_);
	/**@得到当前测点报警状态
	  *@param pAlarmState_ 报警的详细状态
	  *@return 测点报警状态
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///设置当前通道的报警状态
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	void SetParentPoint(LPVOID pParent_);
    virtual void SetData(float fValue_,CBHDateTime dataTime_);
	virtual void SetData(int index, float fValue_,CBHDateTime dataTime_);

	virtual void SetSUDStatus(int anSUDStatus);
	virtual float GetSUDStatus();
	
	bool JudgeRegAgainAlarm();
	bool JudgeRegAgainAlarm(float afNewValue,float afOldValue);
	bool JudgeRegAgainAlarmForConfirm();
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);

	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	//获取报警类型
	int GetChannRegAlarmType();

	/************************************************************************/
	/* 新常规报警逻辑变量													*/
	/************************************************************************/
public:

	_CHANN_ALARM_STATUS_ PredictChannAlarm(  IDBInterFace * pDb_,
											 bool abPlantAlarmed,
											 bool abConfirm = TRUE,
											 bool abChangeStatus = TRUE);//isPlantPreviousAlarmed 当前设备是否报警


	/************************************************************************/
	/* 上次报警的最大值														*/
	/************************************************************************/
	long m_fLastMaxAlarmValue;
	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* 上次报警值															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* 上次报警均值                                                         */
	/************************************************************************/
	float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* 更新报警均值                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*测点半正常计时器，默认超过1个小时就算完全正常                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* 内部使用的测点报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);


	/************************************************************************/
	/* 切换测点内部报警状态                                                 */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus,BOOL abSaveData);

	/************************************************************************/
	/* 预测的测点内部报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDb_);

	/************************************************************************/
	/* 连续超过上次报警值的累计次数                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* 设备是否发生了新的报警                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);
	int HandleRegAlarmStop( IDBInterFace * pDb_ );
	int AddRegAlarmLog( IDBInterFace * pDb_ );
	BOOL InitValues();

	//测点触发新报警时的值
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }
public:
	E_ALARM_LEVEL m_nAlarmLevel;
	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);

};



/**每对轴心位置报警信息：快变和缓变报警
  */
class CAxisAlarmChannParam: public CAxisChannParam
{
public:
	CAxisAlarmChannParam();
	~CAxisAlarmChannParam();
	int JudgeAlarm(IDBInterFace * pDB_);

	int JudgeDynRegAlarm(IDBInterFace * pDb_);
    //判断动态活塞杆轨迹数据报警
	int JudgeDynRegAlarmData();

	///更新旋转设备轴心轨迹测点的轴心位置数据
	virtual bool UpdateData();

	///更新往复设备动态测点的轴心位置数据
	virtual bool UpdateDynData();

	/**@brief 得到当前测点报警状态
      *@param pAlarmState_ 报警的详细状态
	  *@return 测点报警状态
	  */
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);

	///设置当前轴心位置的报警状态
	void SetAlarmStatus(S_AlarmState sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel sAlarmState_);

	/**当常规报警结束后，进行的相关的操作
	*/
	int HandleRegAlarmStop(IDBInterFace * pDB_);

	/**@brief 得到当前指定类型的特征值
	*@param iCharType_ 特征值类型
	*@return 特征值
	*/
	DATATYPE_CHAR GetCurrSpecCharTypeValue();
private:
	//判断其缓变报警
	int JudgeSlowAlarm(IDBInterFace * pDb_);

	/**@brief 添加报警日志
	  *@param iType_表示报警状态,是快变还是缓变
      *@return 如果添加成功,返回为报警日志的事件ID;如果失败,就返回-1;
	  */
    int AddAlarmLog(IDBInterFace * pDB_,int iType_,int iThID_);

	/**@brief 常规报警后，保存数据的一些操作
	*/
	int RegAlarmStatusOP(IDBInterFace * pDb_);

	/**@brief 确定缓变报警后，保存数据的一些操作
	  */
	int SlowAlarmStatusOP(IDBInterFace * pDb_);

	/**@brief 添加缓变报警日志首次保存缓变数据的一些操作
	  */
	int FirstSlowAlarmOP(IDBInterFace * pDb_);

	/**@brief 缓变报警停止后，所做的一些操作
	  */
	int HandleSlowsAlarmStop(IDBInterFace * pDB_);

	/**@brief 快变报警停止后，所做的一些操作
	*/
	int HandleFastAlarmStop(IDBInterFace * pDB_);

	//判断其快变报警
	int JudgeFastAlarm();

	//判断当前轴心位置数据是否快变报警
	bool CheckAxisDataStatusF();
	//判断当前轴心位置数据是否缓变报警
	bool CheckAxisDataStatusS();
    //当出于缓变报警状态下,判断是否有新的报警触发,如果就从头开始保存数据
	bool CheckIfNewSlowAlarm();
	//学习快变报警阈值,并保存到阈值表.返回值为阈值ID.如果为-1,说明学习失败
	int LearnAddThresholdF(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_=E_TRIG_TH_ALARM);
	//学习缓变报警阈值,并保存到阈值表.返回值为阈值ID.如果为-1,说明学习失败
	int LearnAddThresholdS(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_=E_TRIG_TH_ALARM);
	/**学习缓变报警阈值,.如果为-1,说明学习失败
	  *@param iDataLen_ 学习使用的数据长度
	  *@param pAxisData_ 学习使用的数据指针
	  *@return 返回-1说明学习失败，大于0说明学习成功
	  */
	int LearnThresholdS(int iDataLen_,S_AxisData *pAxisData_);

    /**@brief 保存轴心位置数据
	  *@param iLocation_ 当前数据处于数据缓存中位置
	  *@return 如果保存失败就返回-1，否则返回保存报警数据的报警事件ID
	  */
	int SaveData(IDBInterFace * pDB_,int iLocation_,string sTblName_,int iEventID_);


	/**时间转化为时间字符串(加引号的)
	*@param time_ 时间
	*@return 返回字符串
	*/
	CString TimeConvertString(CBHDateTime time_);

private:
	S_AxisData _pAxisData[D_ALARM_SAVECHAR_NUM];
    CBHDateTime _pDateTime[D_ALARM_SAVECHAR_NUM];
	CBHDateTime _timeCurr;
	//当前数据所处位置,即数据缓存数组的索引
	int _iCurrIndex;
	//轴位移通道当前所处状态
	int _iStatus;

	//根据通道类型判断报警的字节位
	int _iAlarmBit;

	//当前常规报警事件ID
	int _iEventIDReg;
	//常规报警判断
	int _iAxisRegStatus;
	///常规报警判断标志。-1表示没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
	int _iRegAlarmFlag;
	CBHDateTime _timeSaveAxisDataFirst;//首次保存常规报警数据时间
	CBHDateTime _timeSaveAxisData;//上次保存常规报警数据时间

	//测点缓变报警状态
	int _iSlowStatus;
	//测点快变报警状态
	int _iFastStatus;
    ///是否学习了门限
	bool _bLearnTh;
	//首次缓变报警
	bool _bFirstSlowAlarm;
	//是否出于缓变报警状态
	bool _bSlowAlarmStatus;
	//当前缓变报警事件ID
	int _iEventIDSlow;
    //当前快变报警事件ID
	int _iEventIDFast;
	//当前快变阈值ID
	int _iCurrThIDFast;
	//当前缓变阈值ID
	int _iCurrThIDSlow;
	//初次报警阶段使用的两个变量，为判断是否是假报警
    //初次报警阶段累积报警的次数
	int _iSlowAlarmCounts;
	//初次报警阶段累积判断报警的次数
	int _iSlowCounts;
    int _iID;
    CThreshold _thS;//缓变的阈值
	CThreshold _thF;//快变的阈值
	CBHDateTime _timeThLearnS;//缓变阈值的学习时间
    CBHDateTime _timeThLearnF;//快变阈值的学习时间
	CBHDateTime _timeFirstSaveDataS;//首次保存缓存数据时间
	CBHDateTime _timeLastSaveDataS;//上次保存缓存数据时间
    //数据库联接指针
	/*CBHOleDbWrapper *_pDB;*/
};


class CDynChannParamAlarm: public CDynChannParam
{
public:
	CDynChannParamAlarm();
	~CDynChannParamAlarm();
private:
	//是否处于初次报警状态
	bool _bFirstAlarmS;
	//是否处于报警状态
	bool _bAlarmStatusS;
	//是否学习过门限
	bool _bLearnTh;
	//累计报警次数，如果该累计次数达到指定数目，就认为处于报警状态
	short _siAlarmCountsS;
	//累计次数。初次报警后，判断报警的累计次数
	short _siCountsS;
	//测点缓变报警状态
	int _iSlowStatus;
	//测点趋势报警状态
	int _iTrendStatus;
	//快变报警状态
	int _iFastStatus;
	//根据通道类型判断报警的字节位
	int _iAlarmBit;
	//常规报警判断
	int _iRegStatus;
	///常规报警判断标志。-1表示没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
	int _iRegAlarmFlag;

	//当前缓变报警事件ID
	long _iEventIDSlow;
	long _iIDSlow;
	//当前快变报警事件ID
	long _iEventIDFast;
	//当前快变阈值ID
	long _iCurrThIDFast;
	//当前缓变阈值ID
	long _iCurrThIDSlow;

	LPVOID _pParent;

	CThreshold _pthSlow[D_BHKD_ALARM_CHARTYPE_NUM];
	CThreshold _pthFast[D_BHKD_ALARM_CHARTYPE_NUM];
	//当前时间
	CBHDateTime _timeCurr;
	//上次学习缓变门限阈值参数时间
	CBHDateTime _timeSlowThLearn;
	//上次学习快变门限阈值参数时间
	CBHDateTime _timeFastThLearn;
	//波形数据缓存的指针
	DATATYPE_WAVE*  _ppfWaveBuf[D_ALARM_SAVEWAVE_NUM];
	//当前波形数据指针的位置
	int _iCurrWavePIndex;
	//当前特征值位置
	int _iCurrCharValueIndex;
	//特征值缓存数组
	S_DynCharValue _pCharValueBuf[D_ALARM_SAVECHAR_NUM];
	//特征值对应的时间缓存.对波形数据来说,最新的D_ALARM_SAVEWAVE_NUM这么多个时间就对应波形数据缓存
	CBHDateTime _ptimeBuf[D_ALARM_SAVECHAR_NUM];


	//自定义报警判断
	int _iCustomStatus;
	///自定义报警判断标志。-1表示没有报警；0：当前常规报警事件中，已经报过警，但是目前没有报警；1：表示处于报警状态
	int _iCustomAlarmFlag;
	//自定义报警:上次触发一次新报警时刻的时间
	CBHDateTime _timeNewCustomAlarmLast;

	//和趋势报警判断相关参数
	//上次检查小时趋势报警的时间
	CBHDateTime _timeTrendCheckHour;
	//上次检查天趋势报警的时间
	CBHDateTime _timeTrendCheckDay;
	//上次检查月趋势报警的时间
	CBHDateTime _timeTrendCheckMonth;
	//上次检查季趋势报警的时间
	CBHDateTime _timeTrendCheckSeason;

	typedef vector<S_DynCharValue> VECTOR_DYNCHAR;
	//趋势判断使用数据缓存
	VECTOR_DYNCHAR _vDynCharValue;
	//和报警保存相关参数
	//初次开始保存报警数据时间
	CBHDateTime _timeSlowFirstSave;
	//上次保存缓变报警数据的时间
	CBHDateTime _timeSlowSaveData;
	//常规报警:上次报警时保存的特征值
	float _fDynRegValueLast;
	float m_fLastAlarmValue;

	//常规报警:上次触发一次新报警时刻的时间
	CBHDateTime _timeDynNewAlarmLast;	

	//快变报警状态
	int m_nFastRegStatus;

	//数据库连接指针
	//	CBHOleDbWrapper *_pDB;
private:
	/**To judge alarm of slow change针对径向振动
	*@return 如果返回0:说明没有报警,1:存在报警;2:有新的报警需要重新开始5分钟数据保存;3:刚刚添加了报警日志,需要重新开始5分钟数据保存
	*/
	int JudgeDynSlowAlarm();

	//径向振动，缓变报警状态下的：5分钟保存和20分钟保存等相关判断操作
	int DynSlowAlarmStatusOP();

	//更新报警状态并判断是否有新的报警产生,针对径向振动
	bool JudgeDynSlowNewAlarm();


	/**判断各个特征值的报警状态,返回的就是报警状态，针对径向振动
	*@param iAlarmNum_ 各种特征值报警的数目
	*@param bHaveNew_ 判断是否有新报警产生，通过当前数据和上次数据的差值是否超过指定百分比.是否有新报警,结果通过这个变量传出.
	*@param bCheckNew_ 是否检查有新报警产生
	*@return 特征值的报警状态组合
	*/
	int JudgeDynSlowCharValue(int &iAlarmNum_,bool &bHaveNew_,bool bCheckNew_=false);


	/**@brief 判断在报警状态下,上次值和当前值之差是否变化超过指定比例
	*@param fLast_ 上次值
	*@param fCurr_ 当前值
	*@return 如果超过指定比例就返回true，否则返回false
	*/
	bool CheckLastAndCurrentValue(float fLast_,float fCurr_);
	//保存波形数据
	//bool SaveData(long lEventID_,int iLocationValue_,int iLocationWave_,string sTime_,int iSmpNum_,int iSmpFreq_,float fRev_,int iAlarmType_,CBHOleDbWrapper *pDB_,string sTblName_);

	//添加缓变报警日志
	int AddSlowAlarmLog(IDBInterFace * pDB_);
	///添加常规报警日志
	int AddRegAlarmLog(IDBInterFace * pDB_);
	///添加自定义报警日志
	int AddCustomAlarmLog(IDBInterFace * pDB_);
	///添加快变报警日志
	int AddFastAlarmLog(IDBInterFace *pDB_);
	//To judge alarm of fast change
	int JudgeFastAlarm();

	/**缓变门限学习等处理。把学习后结果赋给当前缓变门限，并在门限表中插入这条记录
	*如果学习成功就返回新学门限ID
	*/
	int LearnAddSlowThreshold(IDBInterFace * pDB_,E_TRIG_TH_LEARN_TYPE iType_);
	//快变门限学习
	int LearnFastThreshold(E_TRIG_TH_LEARN_TYPE iType_);

	/**幅值缓变门限学习，同时学习所有判断报警的特征值
	*@param iLearnDataLen_ 学习使用的数据长度
	*@param pVibValue_ 学习使用的数据
	*@return 返回如果大于0，说明学习成功，小于0说明学习失败
	*/
	int LearnSlowThreshold(int iLearnDataLen_,S_DynCharValue *pVibValue_);

	/**判断指定时间间隔趋势报警
	*@return 返回值为各种特征值（1X、2X、3X等）类型报警的总和
	*/
	int JudgeSpecTimeIntvAlarm(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_,int iJudgeAlarmType_);

	/**@brief 从历史数据趋势库读取指定的数据,数据保存在类的成员变量"_vVibCharValue"中
	*@param timeStart_ 历史数据的起始时刻
	*@param timeEnd_ 历史数据的终止时刻
	*@return 读取历史数据的记录条数
	*/
	int ReadHisData(IDBInterFace * pDb_,CBHDateTime timeStart_,CBHDateTime timeEnd_);
	//判断数据的趋势是否报警
	int JudgeAmpTrendDataIFAlarm(float *pfData_,int iLen_,CTrendCheckParam trendTh_,int iJudgeAlarmType_);
	//判断指定特征值类型的趋势数据报警
	int JudgeSpecTypeTrendAlarm(float * pfValue_,int iLen_,int iCharType_,int iJudgeAlarmType_);
	//添加趋势报警日志,返回的就是事件的ID
	int AddTrendAlarmLog(IDBInterFace * pDb_,int iStatus_,int iTrendType_);

	/**@brief 判断常规数据是否报警
	*@return 如果高限报警就返回2，低限报警就返回1，不报警就返回0
	*/
	int JudgeRegDataAlarm(bool bConfirmAlarm_ = true);

	/**当常规报警结束后，进行的相关的操作
	*/
	int HandleRegAlarmStop(IDBInterFace * pDB_);

	/**当自定义报警结束后，进行的相关的操作
	*/
	int HandleCustomAlarmStop(IDBInterFace * pDB_);

	/**@brief 发生报警后，需要对当前的阈值进行修改，通过这个函数进行操作
	*@param th_ 阈值
	*@param bPhase_ 是否是相位阈值
	*@return 修改后的阈值
	*/
	CThreshold ModifyDynThS(CThreshold th_,bool bPhase_=false);
public:
	/**@brief 判断常规报警
	*@param iFlag_ 引用变量。是否是第一次报警，如果是第一次就大于0，负责小于0
	@param bSaveAlarmDataFlag_ 引用变量。添加一次报警ID或则特征值比之前特征值大时，是否连续保存数据标识
	@param bSaveOneAlarmData_ 引用变量。间隔30分钟后，是否保存数据标识
	*@return 返回值为报警状态
	*/
	int JudgeRegAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_,bool bConfirmAlarm_ = true);


	/**@brief 判断常规报警
	*@param iFlag_ 引用变量。是否是第一次报警，如果是第一次就大于0，负责小于0
	@param bSaveAlarmDataFlag_ 引用变量。添加一次报警ID或则特征值比之前特征值大时，是否连续保存数据标识
	@param bSaveOneAlarmData_ 引用变量。间隔30分钟后，是否保存数据标识
	*@return 返回值为报警状态
	*/
	int JudgeRegAlarmEx(IDBInterFace * pDb_,int &iFlag_,bool &bSaveAlarmDataFlag_,bool &bSaveOneAlarmData_);


	/**@brief 判断自定义报警
	*@param iFlag_ 引用变量。是否是第一次报警，如果是第一次就大于0，负责小于0
	*@return 返回值为报警状态
	*/
	int JudgeCustomAlarm(IDBInterFace * pDb_,int &iFlag_,bool &bSaveLastAlarmData_);

	/**@brief 根据报警方式，比较两次报警时刻的特征值，判断重复报警
	*@param fCurrValue_ 当前报警时刻的特征值
	@param fValueLast_ 上次报警时刻的特征值
	*@return 返回状态
	*/
	bool JudgeRegAgainAlarm(float fCurrValue_,float fValueLast_);

	/**@brief 根据报警方式，比较两次报警时刻的特征值，判断是否超过重置设备计时器的阈值
	*@param fCurrValue_ 当前报警时刻的特征值
	@param fValueLast_ 上次报警时刻的特征值
	*@return 返回状态
	*/
	virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);
	bool JudgeRegAgainAlarmForConfirm();
	/**重置常规报警状态
	*/
	void ResetRegAlarmStatus();

	/**重置自定义报警状态
	*/
	void ResetCustomAlarmStatus();

	//当报警结束后，进行的相关的操作
	int HandleSlowAlarmStop(IDBInterFace * pDb_,bool bLearnTh_=true);

	//To judge alarm of trend
	int JudgeTrendAlarm();
	/**To judge alarm of slow change
	*@return 如果返回0:说明没有报警,1:存在报警;2:有新的报警需要重新开始5分钟数据保存;3:刚刚添加了报警日志,需要重新开始5分钟数据保存
	*/
	int JudgeSlowAlarm();
	//To judge all alarm
	int JudgeAlarm();

	void SetParentPoint(LPVOID pParent_);

	/**@brief 得到当前测点报警状态
	*@param pAlarmState_ 返回的报警的详细状态
	*@return 测点报警状态
	*/
	unsigned int GetAlarmStatus(S_AlarmState *pAlarmState_=NULL);
	unsigned int GetAlarmStatusEx(S_AlarmStateEx *pAlarmState_=NULL);

	///设置当前通道的报警状态
	void SetAlarmStatus(S_AlarmState& sAlarmState_);
	void SetAlarmStatus(S_AlarmStateWithLevel& sAlarmState_);
	void SetAlarmStatusEx(S_AlarmStateEx& sAlarmState_);

	/**@得到当前测点缓变报警状态
	*@return 测点报警状态
	*/
	int GetSlowAlarmStatus();

	/**@得到当前测点常规报警状态
	*@return 测点常规报警状态
	*/
	int GetRegAlarmStatus();

	/**@得到当前测点自定义报警状态
	*@return 测点自定义报警状态
	*/
	int GetCustomAlarmStatus();

	virtual bool SetChannValue(BHKD::S_DynCharValue *pVibCharValue_);

	/**@brief 设置当前的通道的波形数据
	*@param pfWave_ 波形数据指针
	*@param iSmpNum_ 波形数据采样点数
	*@return true denotes successed, or false denotes failed
	*/
	virtual bool SetChannWave(const DATATYPE_WAVE *pfWave_,const int &iSmpNum_);

	/**@brief 得到指定通道的波形数据指针
	*@param iIndex_ 位置索引
	*@return 通道波形数据指针
	*/
	const DATATYPE_WAVE * GetWavePoint(int iIndex_);

	/**@brief 得到当前通道波形数据指针
	*@param 位置索引
	*@return 通道波形数据指针
	*/
	virtual const DATATYPE_WAVE * GetCurrWavePoint();
	/**@brief 指定波形数据缓存的指针.这个指针指向内存有把D_ALARM_SAVEWAVE_NUM*iSmpNum_个数据,把其分别分配给当前测点波形数据缓存
	*@param pWave_ 波形数据缓存指针
	*@param iSmpNum 波形数据的采样点数
	*/
	virtual void SetWaveMemoryPoint(DATATYPE_WAVE *pWave_,int iSmpNum_);


	/**@brief 得到指定索引、类型的特征值
	*@param iCharType_ 特征值类型
	*@return 特征值
	*/
	DATATYPE_CHAR GetSpecCharTypeValue(int index_,int iCharType_=0);


	/**@brief 得到当前指定类型的特征值
	*@param iCharType_ 特征值类型
	*@return 特征值
	*/
	DATATYPE_CHAR GetCurrSpecCharTypeValue(int iCharType_=GE_ALLCHAR);

	/**@brief 得到指定索引的特征值
	  *@return 特征值
	  */
	S_DynCharValue GetCharValue(int index_);
	S_DynCharValue GetCharValue();

	//得到全部特征值
	virtual  S_DynCharValue GetCharValueEx() ;

	virtual  S_DynCharValue GetCharValueEx(int index_) ;

	/**得到当前特征值的位置索引
	  */
	int GetCurrCharValueIndex();

	/**得到当前波形数据的位置索引
	  */
	int GetCurrWaveIndex();

	/**得到指定索引时间
	  */
	string GetSpecIndexTime(int index_);

	/**时间转化为时间字符串(加引号的)
	  *@param time_ 时间
	  *@return 返回字符串
	  */
	CString TimeConvertString(CBHDateTime time_);

	/************************************************************************/
	/* 是否需要计算特征值                                                   */
	/************************************************************************/
	virtual BOOL IsNeedComputeEigenValue(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 特征值是否没有计算                                                   */
	/************************************************************************/
	virtual BOOL IsEigenValueEmpty(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 计算特征值															*/
	/************************************************************************/
	virtual BOOL ComputeEigenValue(int aEnumMachineType);
	virtual BOOL ComputeEigenValue(int aEnumMachineType,int anIndex);


	/************************************************************************/
	/* 计算通频值															*/
	/************************************************************************/
	virtual float ComputeAllCharValue(S_DynCharValue  & arefSVibCharValue);

	/************************************************************************/
	/* 计算轴位移测点用来存储数据的三倍频相位值								*/
	/************************************************************************/
	virtual float ComputeThreePhaseCharValue(S_DynCharValue  & arefSVibCharValue);


	float LastAlarmValue() const;
	void LastAlarmValue(float val);

	void SaveCurrentAsLastAlarmValue();

	int JudgeFastAlarm(IDBInterFace *pDB_,CBHDateTime dtTime_, S_DynCharValue& CharValue_);

	//获取报警类型
	int GetChannRegAlarmType();
	

public:
	
	/************************************************************************/
	/* 新常规报警逻辑变量													*/
	/************************************************************************/
public:

	_CHANN_ALARM_STATUS_ PredictChannAlarm(IDBInterFace * pDb_,
											bool abPlantAlarmed,
											bool abConfirm = TRUE,
											bool abChangeStatus = TRUE);//isPlantPreviousAlarmed 当前设备是否报警


	/************************************************************************/
	/* 上次报警的最大值														*/
	/************************************************************************/


	long m_fLastMaxAlarmValue;

	float LastMaxAlarmValue() const;
	void LastMaxAlarmValue(float val);

	/************************************************************************/
	/* 上次报警值															*/
	/************************************************************************/
	float m_fLastAlarmValueEx;
	float LastAlarmValueEx() const;
	void LastAlarmValueEx(float val);


	/************************************************************************/
	/* 上次报警均值                                                         */
	/************************************************************************/
	float m_fLastEverageAlarmValue;
	float LastEverageAlarmValue() const;
	void LastEverageAlarmValue(float val);

	/************************************************************************/
	/* 更新报警均值                                                         */
	/************************************************************************/
	float updateEverageAlarmValue(float afLastAlarmValue);

	/************************************************************************/
	/*测点半正常计时器，默认超过1个小时就算完全正常                       */
	/************************************************************************/
	COleDateTime m_oHalfNormalTimer5;
	void ResetHalfNormalTimer5();

	/************************************************************************/
	/* 内部使用的测点报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ InnerAlarmStatus() const;
	void InnerAlarmStatus(_CHANN_ALARM_STATUS_ val);


	/************************************************************************/
	/* 切换测点内部报警状态，返回值表示是否有报警日志增加                   */
	/************************************************************************/
	int SwitchAlarmStatus(IDBInterFace * pDb_,_CHANN_ALARM_STATUS_ anNewStatus);

	/************************************************************************/
	/* 预测的测点内部报警状态                                               */
	/************************************************************************/
	_CHANN_ALARM_STATUS_ m_nPredictedChannInnerAlarmStatus;
	_CHANN_ALARM_STATUS_ PredictedChannInnerAlarmStatus() const;
	void PredictedChannInnerAlarmStatus(_CHANN_ALARM_STATUS_ val);

	int HandleRegAlarmStopEx(IDBInterFace * pDB_);

	/************************************************************************/
	/* 连续超过上次报警值的累计次数                                         */
	/************************************************************************/
	int m_nContinuousMoreThanTimes;
	int ContinuousMoreThanTimes() const;
	void ContinuousMoreThanTimes(int val);
	void ResetContinuousMoreThanTimes();
	void IncreaseContinuousMoreThanTimes();

	/************************************************************************/
	/* 设备是否发生了新的报警                                               */
	/************************************************************************/
	BOOL m_bPlantNewAlarmOccoured;
	BOOL PlantNewAlarmOccoured() const;
	void PlantNewAlarmOccoured(BOOL val);
	BOOL InitValues();


public:
	/************************************************************************/
	/* 直流量的新报警逻辑                                                   */
	/************************************************************************/
	BOOL GetBufferForNewDCLogic( std::vector<DATATYPE_WAVE> & arefWaveBuffer );  //得到新的直流量报警逻辑上次存储的波形
	float GetValueForNewDCLogic();
	BOOL IsWaveAlike(DATATYPE_WAVE * apWaveBuffer1,int anWavebuffer1,
					 DATATYPE_WAVE * apWaveBuffer2,int anWavebuffer2);				  //判断波形是否相似
	
	float m_fDCOriginal;												  //累计的直流量变化
	float DCOriginal() const;
	void DCOriginal(float val);
	void ResetDCOriginal();											  //重置累计的直流量变化

	BOOL SaveDataForNewDCLogic();										  //为新的直流量报警逻辑存储上次开车时的波形
	BOOL JudgeAndDoForNewDCLogic(BOOL abRunningStatusChanged);			  //判断并处理新的直流量逻辑
	
	CString m_strWaveFilePath;											  //存放波形文件的文件路径
	CString WaveFilePath() const;
	void WaveFilePath(CString val);

	CString m_strValueFilePath;											//存放值文件的文件路径
	CString ValueFilePath() const;
	void ValueFilePath(CString val);

	CString m_strWaveFileNum;
	CString WaveFileNum() const;
	void WaveFileNum(CString val);
	
	BOOL InitFileNameForNewDCAlarmLogic();								//初始化直流量文件名

	CBHDateTime m_oTimerForNewDCLogic;
	CBHDateTime TimerForNewDCLogic() const;
	void TimerForNewDCLogic(CBHDateTime val);
	void ResetTimerForNewDCLogic();
	BOOL IsReachSaveTimeIntervalForNewDCLogic(); //是否达到了保存波形的时间

	//测点触发新报警时的值
	float m_fNewAlarmValue;
	float NewAlarmValue() const { return m_fNewAlarmValue; }
	void NewAlarmValue(float val) { m_fNewAlarmValue = val; }

	BOOL m_bJustStarted;
	BOOL JustStarted() const { return m_bJustStarted; }
	void JustStarted(BOOL val) { m_bJustStarted = val; }
public:
	E_ALARM_LEVEL m_nAlarmLevel;
	E_ALARM_LEVEL GetAlarmLevel();
	void SetAlarmLevel(const E_ALARM_LEVEL nAlarmLevel_);
	void SetChannAlarmLevel(int eAlarmStatus_);
	

};

#endif //_BHKD_CHANNALARM_H_MARBLE_