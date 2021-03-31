#pragma once
#include "stdafx.h"

/**一些注意事项：
  *在数据处理时，保存和报警检查时，轴位移数据和过程量数据都同等对待，但是在数据更新时候是分别对待
  */
class CDataHandle
{
public:
	CDataHandle(void);
	~CDataHandle(void);
private:
	struct S_VibInfo
	{
		string   strChann;
		string   strAll;
		string   strOne;
		string   strTwo;
		string   strThree;
		string   strHalf;
		string   strRes;//残振值
		string   strWave;
		string   strPhaseOne;
		string   strPhaseTwo;
		string   strPhaseThree;
		string   strPhaseHalf;
		string   strDC;
		float  fLowAlarmValue;
		float  fHighAlarmValue;
		float  fBeforeValue;
		int    iContinueNum;
		int    iIntervalNum;
		bool   bHighAlarm;
		bool   bCheckAlarm;//是否判断报警
		COleDateTime  timeCheck;
		int   iOfRev;   //属于那个转速
	};
	/*!@brief 过程量测点信息结构体
	  */
	struct S_ProcInfo
	{
		string strChannel;
		int    iType;
		float  fLowAlarmValue;
		float  fHighAlarmValue;
		float  fBeforeValue;
		int    iContinueNum;
		int    iIntervalNum;
		bool   bHighAlarm;
		bool   bCheckAlarm;//是否判断报警
		COleDateTime  timeCheck;
	};
	
	///装置号
	string    _strSet;
	///设备号
	string    _strPlant;
	string    _strVibAlarmTable;
	string    _strProcAlarmTable;
	int       _iVibChannNum;
	int       _iProcChannNum;
	int       _iAxisDisplaceNum;
	int       _iSmpNum;

	///最大的采样点数，波形数据的长度就是按照这个数据分配的
	int       _iSmpMaxNum;
	
	int       _iSmpFreq;
	float     *_pfVibCharAll;
	float     *_pfVibCharOne;
	float     *_pfVibCharTwo;
	float     *_pfVibCharThree;
	float     *_pfVibCharHalf;
    float     *_pfVibCharRes;
	float     *_pfPhaseOne;
	float     *_pfPhaseTwo;
	float     *_pfPhaseThree;
	float     *_pfPhaseHalf;
	float     *_pfDC;
	float     **_ppfWave;
	float     *_pfProcValue;
	float     *_pfAxisDisplaceValue;
	COleDateTime     _timeCurrent;
	CString          _strTimeCurr;
	CSimADO   *_pDB;
	float     _iRev1;
	float     _iRev2;

	/*!此设备所有过程量测点是运行状态，用以通知中间件此设备所有过程量测点运行状态。  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbProcChannelState;

	/*!此设备所有轴位移测点是运行状态，用以通知中间件此设备所有轴位移测点运行状态。  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbAxisChannelState;

	/*!此设备所有振动测点是运行状态，用以通知中间件此设备所有振动测点运行状态。
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbVibChannelState;    
    
	///过程量数据读写锁
	CMutexRW *_lockDataProc;

	///振动测点数据读写锁
	CMutexRW *_lockDataVib;

	///测点运行状态数据锁
    CMutexRW * _lockDataRunState;

	///用于检查报警时数据更新状态变量的读写锁
    CMutexRW * _lockDataUpdatedAlarm;

	///用于历史保存时数据更新状态变量的读写锁
	CMutexRW * _lockDataUpdatedHis;

protected:
	typedef vector<S_VibInfo> VECTOR_S_VIBINFO;
	typedef vector<S_ProcInfo> VECTOR_S_PROCINFO;

	/*!@brief 所有振动测点向量，顺序和在数据库中通道表单中的顺序相同
	*/
	VECTOR_S_VIBINFO _vectorVibChann;

	/*!@brief 所有过程量测点和轴位移测点向量，顺序和在数据库中通道表单中的顺序相同
	*/
	VECTOR_S_PROCINFO _vectorProcChann;

	VECTOR_S_VIBINFO::const_iterator _cvectorEndVib;
	VECTOR_S_PROCINFO::const_iterator  _cvectorEndProc;
 
public:
	/**@brief 设置装置号和设备号*/
	void SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iMaxSmpNum);
	

	/**@brief 添加测点信息*/
	void AddVibChannelInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm,int ag_iofRev);
	
	/**@brief 添加过程量测点信息*/
	void AddProcAndAxisChannInfo(string ag_strChann, float ag_fLowAlarmValue, float ag_fHighAlarmValue,int ag_iCheckAlarm, int ag_iChannType);
	
	
	/// 添加振动测点信息结束，在这个函数中构造数据库操作中使用的字段序列
	void AddVibChannInfoEnd(void);
	
	/// 添加过程量测点信息结束，在这个函数中构造数据库操作中使用的字段序列
	void AddProcChannInfoEnd(void);

	/**@brief 初始化测点参数和每个测点对应的报警值,和链表.注意：此函数一定要运行在添加完所有测点信息完毕。
      *即在此函数运行后，就不允许再添加测点了信息了，即不允许运行AddVibChannelInfo和AddProcChannInfo这个两个函数了
	  */
	void InitParam(float *ag_pfCharAll,
		float *ag_pfCharOne,
		float *ag_pfCharTwo,
		float *ag_pfCharThree,
		float *ag_pfCharHalf,
		float *pfCharRes_,
		float *pfPhaseOne_,
		float *pfPhaseTwo_,
		float *pfPhaseThree_,
		float *pfPhaseHalf_,
		float *ag_pfDC,
		float **ag_ppfWave,
		float *ag_pfProc,
		float *ag_pfAxisDisplace,
		int  *ag_pbVibChannelState,
		int  *ag_pbProcChannelState,
		int  *ag_pbAxisDisplaceChannelState,
		CSimADO *ag_pDB);
	
	/// 得到设备号
	string GetPlantID(void);
	
	/// 设置设备转速
	void SetPlantRev(float ag_iRev1,float ag_iRev2);
   
	///更新振动测点数据
    void UpdateVibData(float * ag_pfCharOne,
		            float * ag_pfCharTwo,
					float * ag_pfCharThree,
					float * ag_pfCharAll,
					float * ag_pfCharHalf,
					float * ag_pfCharRes,
					float * ag_pfDC,
					float * pfPhaseOne_,
					float * pfPhaseTwo_,
					float * pfPhaseThree_,
					float * pfPhaseHalf_,
					float * ag_fwave,
					int ag_iSmpNum,
					int ag_iSmpFreq,
					int ag_iRealNum,
					int ag_len,
					int ag_startIndex=0);
    
	///更新过程量数据,不包括轴位移数据
	void UpdateProcData(float *ag_pfData,int ag_len,int ag_startIndex=0);
   
	///轴位移轴位移数据
	void UpdateAxisDisplaceData(float *ag_pfData,int ag_len,int ag_startIndex=0);
    
	/*!@brief 得到此设备运行状态读写锁指针
	*/
	CMutexRW * GetRunStateLock();

  	/*!@brief 得到此设备过程量测点运行状态指针
	*/
	int * GetProcChannelStatePointer(void);

	/*!@brief 得到此设备轴位移测点运行状态指针
	*/
	int * GetAxisDisplaceChannelStatePointer(void);
	
	/*!@brief 得到此设备振动测点运行状态指针
	  */
	int * GetVibChannelStatePointer(void);

	/*!@brief 得到此设备所有振动测点个数
	*/
	int GetVibChannelNum(void);

	/*!@brief 得到此设备所有过程量测点个数
	*/
	int GetProcChannelNum(void);
	
	/// 得到这个设备轴位移通道的个数
	int GetAxisDisplaceChannelNum(void);
    
	/// 得到这个设备振动测点的最大采样点数
	int GetSmpMaxNum(void);
public:
///报警需要的各个函数和变量   
	/**@brief 判断有没有报警
	  */
	void JudgeAlarm(CCustomDB * ag_pDB);
private:
///报警需要的各个函数和变量
	/*!振动测点报警记录的总数目，根据振动测点数目来决定的，振动测点个数乘以1000.*/
	int _iAlarmVibRecordAllNum;

	/*!过程量测点报警记录的总数目，根据过程量测点数目来决定的，过程量测点个数乘以1000.*/
	int _iAlarmProcRecordAllNum;

	/*!振动报警过程中，下次记录应该保存的ID值*/
	int _iCurIDAlarmVib;

	/*!过程量报警过程中，下次记录应该保存的ID值*/
	int _iCurIDAlarmProc;
	
	/// 初始化报警表的最大记录条数，得到当前报警表中的最新记录的ID值，
	void AlarmInitIDAndRecordAllNum(void);
	
	/**@brief 依次判断所有过程量测点是否报警并保存报警测点
	  */
	void JudgeProcAlarm(CCustomDB * ag_pDB);

	/**@brief 依次判断所有振动测点是否报警并保存报警测点*/
	void JudgeVibratedAlarm(CCustomDB * ag_pDB);

	/**@brief 保存振动报警记录数据*/
	void SaveVibratedAlarmRecord(int ag_index,CCustomDB * ag_pDB);

	/**@brief 保存过程量报警记录*/
	void SaveProcAlarmRecord(int ag_index,int ag_index2,CCustomDB * ag_pDB);

	/*!@brief 判断是否达到初级的振动报警记录保存间隔，如果达到就保存报警记录
	  !@param  ag_index:振动测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeVibAlarmPri(int ag_index);

	/*!@brief 判断是否达到高级的过程量报警记录保存间隔，如果达到就保存报警记录
	  !@param  ag_index:过程量测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeProcAlarmSup(int ag_index);

	/*!@brief 判断是否达到初级的过程量报警记录保存间隔，如果达到就保存报警记录
	  !@param  ag_index:过程量测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeProcAlarmPri(int ag_index);

	/*!@brief 判断是否达到高级级的振动报警记录保存间隔，如果达到就保存报警记录
	  !@param  ag_index:振动测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeVibAlarmSup(int ag_index);

	/*!@brief 判断过程量低限报警的一些参数，如果达到就保存报警记录
	  !@param  ag_index:过程量测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeProcAlarmLow(int ag_index);

	/*!@brief 判断振动测点高限报警的一些参数，如果达到就保存报警记录
	  !@param  ag_index:振动测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeVibAlarmHigh(int ag_index);

	/*!@brief 判断过程量高限报警的一些参数，如果达到就保存报警记录
	  !@param  ag_index:过程量测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeProcAlarmHigh(int ag_index);

	/*!@brief 判断振动测点高限报警的一些参数，如果达到就保存报警记录
	  !@param  ag_index:振动测点索引
	  !@return 如果为true，则保存报警记录，false则不保存
	  */
	bool JudgeVibAlarmLow(int ag_index);
public:
///保存历史数据需要的各个函数和变量
	// 保存趋势数据，外部调用
	void SaveTrendData(void);
private:
///报警历史数据需要的各个函数和变量
	string        _strVibSecondTrendTable;
	string        _strVibDayTrendTable;
	string        _strVibHourTrendTable;
	string        _strVibMonthTrendTable;
	string        _strProcDayTrendTable;
	string        _strProcHourTrendTable;
	string        _strProcMonthTrendTable;
	string        _strProcSecondTrendTable;
	string        _strProcField;
	string        _strVibFieldSel;
	string        _strVibFieldIns;
	
	/*!在趋势表，当前趋势秒趋势保存过程中，下次记录应该保存的记录ID值，这个表单中总共是3600条记录，即小时，因为这个表单在外部通过定时器设置为一秒钟执行一次*/
	int           _iCurrIDTrendSecond;

	/*!在趋势表，小时趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDTrendHour;

	/*!在趋势表，天趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDTrendDay;

	/*!在趋势表，月趋势保存过程中，下次记录应该保存的记录ID值*/
	int           _iCurrIDTrendMonth;

	/*!用于判断是否该保存天趋势记录，即间隔数目。*/
	int _iTrendDayInteral;

	/*!用于判断是否该保存月趋势记录，即间隔数目。*/
	int _iTrendMonthInteral;

	/*!用于判断是否该保存小时趋势记录，即间隔数目。*/
	int _iTrendHourInteral;

    /**在判断是否需要保存数据时使用。因为如果数据没有更新就不保存了。在数据更新函数中设置为true，保存函数中设置为false
	  *判断数据是否更新过,true表示数据更新了，false表示数据更新
	  */
	bool _bDataUpdatedForHis;
	
	/**在判断是否需要检查过程量数据报警时使用。因为如果数据没有更新就不检查报警了。在过程量数据更新函数中设置为true，报警检查函数中设置为false
	  *判断过程量数据是否更新过，包括轴位移数据,true表示数据更新了，false表示数据更新
	  */
	bool _bProcDataUpdatedForAlarm;

	/**在判断是否需要检查振动数据报警时使用。因为如果数据没有更新就不检查报警了。在振动数据更新函数中设置为true，报警检查函数中设置为false
	  *判断振动数据是否更新过，true表示数据更新了，false表示数据更新
	  */
	bool _bVibDataUpdatedForAlarm;

	// 初始化各种趋势的当前记录ID值和天趋势、月趋势的间隔值
	void TrendInitIDAndInterval(void);

	// 判断是否该保存小时趋势
	bool JudgeHourTrendSave(void);
	// 判断是否到保存月趋势记录的时刻
	bool JudgeMonthTrendSave(void);
	// 判断是否到保存天趋势的时候
	bool JudgeDayTrendSave(void);

	// 保存在当前趋势表单中得趋势数据，只保存一个小时之类得。一秒钟保存一次（在外部通过定时器控制）。
	void SaveSecondTrendData(void);
	// 天趋势保存
	void SaveDayTrend(void);
	// 小时趋势保存
	void SaveHourTrend(void);
	// 月趋势保存
	void SaveMonthTrend(void);
	// 振动趋势记录保存
	void SaveVibRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth=false);
	// 过程量趋势记录保存
	void SaveProcRecord(string ag_strTableName,int ag_iCurID,bool ag_bMonth=false);
	// 更新测点的报警参数信息
	void UpdateChannAlarmParam(CCustomDB *ag_pDB);
};
