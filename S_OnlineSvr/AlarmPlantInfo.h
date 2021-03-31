#pragma once
#include "PlantInfo.h"
#include "ChannAlarm.h"
#include "../include/DieselPlantAlarm.h"

class CAlarmPlantInfo
	: public CPlantInfoOnLine
{
public:
	CAlarmPlantInfo(void);
	~CAlarmPlantInfo(void);
public:
	///判断过程量测点报警
    int JudgeProcAlarm(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);
    ///判断振动测点的缓变报警，并保存数据
    void JudgeVibSlowAlarm(IDBInterFace *pDB_);
    ///判断振动测点常规报警
	int JudgeVibRegAlarm(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);
	///判断振动测点常规报警
	virtual int JudgeRegAlarmEx(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);
	
	///判断振动测点常规报警
	virtual int JudgeFastRegAlarmEx(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);

	virtual int JudgeAlarmByOtherPlant(IDBInterFace *pDB_,CVibChannParamAlarm *pVibChannParamAlarm_);


	///判断振动测点轴心位置的趋势报警
	void JudgeAxisLocationAlarm(IDBInterFace * pDb_);
    ///判断振动测点的趋势报警
	void JudgeVibTrendAlarm(IDBInterFace * pDb_);
	///判断振动测点自定义报警
	void JudgeVibCustomAlarm(IDBInterFace *pDB_);
	

    ///保存缓变报警数据
	int SaveVibSlowAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_);
	///保存报警数据
	int FirstVibSlowAlarmOP(IDBInterFace *pDB);
    ///振动测点缓变报警停止后操作
	int VibSlowAlarmStopOP(IDBInterFace *pDB_);
	///振动测点缓变报警期间的正常数据保存操作
	int VibSlowAlarmOP(IDBInterFace *pDB_,bool bNewAlarm_=false);
	///保存振动报警详细数据
	int SaveAlarmDetailData(IDBInterFace *pDB_,int iEventID_,int iCharValueIndex_,int iWaveIndex_,float *pfRev_,int iSmpNums_,int iSmpFreq_,bool& bSaved,CBHDateTime& tSaveTime_,int iAlarmType_=D_BHKD_AT_REGULAR);

	///保存动态报警详细数据
	int SaveDynAlarmDetailData(IDBInterFace *pDB_,int iEventID_,int iCharValueIndex_,int iWaveIndex_,float *pfRev_,int iSmpNums_,int iSmpFreq_,bool& bSaved,CBHDateTime& tSaveTime_,int iAlarmType_=D_BHKD_AT_REGULAR);

	///保存振动自定义报警数据
	int SaveVibCustomAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_);
	///保存动态自定义报警数据
	int SaveDynCustomAlarmData(IDBInterFace *pDB_,int iCharValueIndex_,int iWaveIndex_);

	/**当前一次添加这次报警日志需要调用的接口
	  *@param sName_ 通道名字或者其它标识
	  *@param dtStart_ 报警开始时间
	  *@param iType_   报警类型
	  *@param iChannType_ 通道类型
	  *@param iThID_    阈值ID
	  *@return 返回申请添加这次报警日志的事件ID,小于0表示失败
	  */
	int AddAlarmLog(IDBInterFace *pDB_,string sName_,long & iID_,long & iEventID_,CBHDateTime dtStart_,int iType_,int iChannType_,int iThID_);

	/**更新报警日志的结束时间和新的阈值ID
	  *@return 0表示成功,小于0表示失败
	  */
    int SetAlarmLogEndTimeAndNewThID(IDBInterFace *pDB_,string sName_,int iID_,int iEventID_,CBHDateTime dtEnd_,int iChannType_,int iThID_);

	/**@brief 初始化各种ID，包括各种日志中的事件ID，门限值中门限ID
	  *@return 如果返回true表示初始化成功，否则失败
	  */
	bool InitEventID(CCustomDB *pDB_);

	bool InitEventID(IDBInterFace *pDB_);

	/**@arefLastFirstSaveAllTime	上一次添加报警日志的时间
	   @arefLastSaveAllTime			上一次保存全部报警数据的时间
	  *@arefNow						当前判断报警的时间
	  */
	BOOL IsAlaramTimeLimitReached(const CBHDateTime & arefLastFirstSaveAllTime,
								  const CBHDateTime & arefLastSaveAllTime,
								  const CBHDateTime & arefNow);

public:
	//报警日志表中的当前事件ID及ID
	long _iID;
	long _iEventID;

	// 从下级服务器同步来的报警ID  (下级24052 -> 同步助手-> 上级24004)
	long _iSyncID;

    ///针对振动参数缓变报警判断一些参数
	//缓变报警的事件ID
	long _iEventIdS;
	//振动参数缓变阈值上次学习的时间
    CBHDateTime _timeVibSlowThLearn;
	//初次开始保存缓变报警数据时间
	CBHDateTime _timeVibSlowFirstSave;
	//上次保存缓变报警数据的时间
	CBHDateTime _timeVibSlowSaveData;
	//振动测点缓变报警状态
	bool _bVibSlowAlarmStatus;

    ///针对常规报警的一些变量
	//常规报警事件ID
	long _iEventIdReg;
	//常规上一次报警事件ID
	long _iEventIdRegLast;
	//初次保存常规报警数据的时间
    CBHDateTime _dtVibRegFirstSave;
	//上次保存常规报警数据的时间
	CBHDateTime _dtVibRegDataSave;
	//上次保存完一条报警数据的时间
	CBHDateTime _dtVibRegDataSaveOne;

	//自定义报警的事件ID
	long _iEventIdCustom;
	//上一次自定义报警事件ID
	long _iEventIdCustomLast;
	//初次保存自定义报警数据的时间
	CBHDateTime _dtVibCustomFirstSave;
	//上次保存自定义报警数据的时间
	CBHDateTime _dtVibCustomDataSave;

	//趋势事件ID
	long _iTrendEventID;
	//阈值ID
	long _iThID;

	IReadWriteLock *_pLockThID;
    //振动数据表的插入字段
    string _sInsField;
	string _sVibTrendField;
	//振动数据的波形显示字段
	string _sSelWaveField;
	//动态数据表的插入字段
	string _sDynInsField;
	string _sDynTrendField;

	//动态数据的波形显示字段
	string _sDynSelWaveField;
	
	//快变报警ID
	long _iEventIDF;

	//动态趋势表波形字段
	string _sDynTrendWaveField;
	//振动趋势表波形字段
	string _sVibTrendWaveField;

public:
	int  SetProcJudgeRunStatus(int anProcJudgedRunStatus,int anLen_,int anStartIndex_=0 );
	int  GetProcJudgeRunStatus();
	void SetVibJudgeRunStatus(int anVibJudgedRunStatus);
	int  GetVibJudgeRunStatus();
private:
	//过程量参与判断的设备状态
	int m_nProcJudgedRunStatus;
	//振动参与判断设备状态
	int m_nVibJudgedRunStatus;
public:
	virtual void AddVibChannInfo(CCustomDB *pDB_);
	virtual void AddDynChannInfo(CCustomDB *pDB_);
	virtual void AddProcChannInfo(CCustomDB *pDB_);
	virtual void AddChannInfoEnd(CCustomDB * pDB_);

	//增加通道自定义报警参数信息
	virtual void AddCustomAlarmParamInfo(CCustomDB *pDB_);


	virtual void AddVibChannInfo(IDBInterFace *pDB_);
	virtual void AddDynChannInfo(IDBInterFace *pDB_);
	virtual void AddProcChannInfo(IDBInterFace *pDB_);
	virtual void AddChannInfoEnd(IDBInterFace * pDB_);

	//增加通道自定义报警参数信息
	virtual void AddCustomAlarmParamInfo(IDBInterFace *pDB_);

	virtual int ChangeVibAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int ChangeDynAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int ChangeProcAlarmLimitation(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetVibGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetDynGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);

	virtual int SetProcGradeThreshold(int anChannIndex,const CChannAlarmParam& aChannAlarmParam);



	void JudgeAlarm(IDBInterFace *pDB_);
	float GetSpecChannRev(string strChann_,float &fRealRev_);

	/*!@brief 得到此设备过程量测点运行状态指针
	  !@param pState 运行状态指针
	  !@param iNum_ 运行状态指针长度
	  */
	int GetProcChannState(S_AlarmState *pState,int & iNum_);
	int GetProcChannStateEx(S_AlarmStateEx *pState,int& iNum_);

	/*!@brief 得到此设备轴位移测点运行状态指针
	  */
	int GetAxisVibChannState(S_AlarmState *pState,int &iNum_);

	/*!@brief 得到此设备径向振动测点运行状态指针
	  */
	int GetRadialVibChannState(S_AlarmState *pState,int &iNum_);

	///得到所有振动测点运行状态
	int GetVibChannState(S_AlarmState *pState,int &iNum_);
	int GetVibChannStateEx(S_AlarmStateEx *pState,int & iNum_);

	///得到所有动态测点运行状态
	int GetDynChannState(S_AlarmState *pState,int &iNum_);
	int GetDynChannStateEx(S_AlarmStateEx *pState,int & iNum_);

	///得到轴心位置测点运行状态
	int GetAxisLocationState(S_AlarmState *pState_,int &iNum_);

	/*!@brief 设置此设备过程量测点运行状态指针
	  !@param pState 运行状态指针
	  !@param iNum_ 运行状态指针长度
	  */
	int SetProcChannState(S_AlarmState *pState,int iNum_);
	int SetProcChannState(S_AlarmStateWithLevel *pState,int iNum_);
	int SetProcChannStateEx(S_AlarmStateEx *pState,int iNum_);

	///设置所有振动测点运行状态
	int SetVibChannState(S_AlarmState *pState,int iNum_);
	int SetVibChannState(S_AlarmStateWithLevel *pState,int iNum_);
	int SetVibChannStateEx(S_AlarmStateEx *pState,int iNum_);

	///设置所有动态测点运行状态
	int SetDynChannState(S_AlarmState *pState,int iNum_);
	int SetDynChannState(S_AlarmStateWithLevel *pState,int iNum_);
	int SetDynChannStateEx(S_AlarmStateEx *pState,int iNum_);

	///设置轴心位置的报警状态
	int SetAxisLocationState(S_AlarmState *pState_,int iNum_);
	int SetAxisLocationState(S_AlarmStateWithLevel *pState_,int iNum_);

	///得到指定测点索引和测点类型的报警状态以及报警值，如果没有报警就返回false
	bool GetSpecChannAndTypeAlarmState(int index_,string &sChann_,float &fData,unsigned int &iState_, int iType_=GE_ALLPROC);

	/**得到指定测点索引和测点类型的报警状态以及报警值，如果没有报警就返回false,后面的特征值仅仅对振动测点来说
	*@param index_    需要测点的索引
	*@param sChann_   是个引用变量，测点ID
	*@param sChannAlias_   是个引用变量，测点位号
	*@param sUnit_    是个引用变量，测点单位
	*@param iList_    是个引用变量，测点所属列表框
	*@param fData_    是个引用变量，测点的特征值大小（对于振动测点，是按照其指定的特征值类型来赋值）
	*@param iState_   是个引用变量，测点的报警状态
	*@param iChannDetailType_ 是个引用变量，通道的详细类型。对于振动测点：径向振动和轴向振动；对于过程量测点：压力、流量等。
	*@param iType_     是过程量还是振动参数
	*@param iCharType_ 特征值类型。针对振动测点，因为振动测点有不同特征值类型
	*@return 成功返回true，失败false
	*/
	bool GetSpecChannAndTypeAlarmState(int index_,string &sChann_,string &sChannAlias_,string &sUnit_,int &iList_,float &fData_,unsigned int &iState_, int &iChannDetailType_,int iType_=GE_ALLPROC,int iCharType_=GE_ALLCHAR);

	///得到指定测点和测点类型的报警状态以及报警值，如果没有报警就返回false
	bool GetSpecChannAndTypeAlarmState(string sChann_,float &fData_,unsigned int &iState_,int iType_=GE_ALLPROC);

	bool GetSpecChannAndTypeAlarmStateEx(int index_,string &sChann_,string &sChannAlias_,string &sUnit_,int &iList_,float &fData_,unsigned int &iState_, int &iChannDetailType_,int &iAlarmLevel_,int iType_=GE_ALLPROC,int iCharType_=GE_ALLCHAR);

	//获取设备报警状态
	bool GetPlantAlarmState();
	//获取设备报警状态, 高报 2，低报 1 无状态 0
	unsigned int GetPlantAlarmStateEx();

   //更新通道数据
	virtual int ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,S_OldVibCharValue *pOldVibCharValue_,float *pfRev_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_,bool bEnd_);
	virtual int ChangeDynharValueToNew(S_DynCharValue *pDynCharValue_,S_OldDynCharValue *pOldDynCharValue_,float *pfRev_,const int &iSmpNum_, const int &iSmpFreq_,const int iRealNum_,const int &iLen_,const int &iStartIndex_, const bool &bEnd_=false);
	//更新从发送助手获得的振动数据
	virtual int UpdateVibData( 
								DATATYPE_WAVE *pWave_,
								S_VibCharValue *pVibCharValue_,
								int iAllRealNum_,
								int iLen_,
								CBHDateTime & arefDate,
								int iStartIndex_,
								const float& fScaleCoef_,
								int anPrecisionEnabled,
								std::vector<float>& afPreciousMin ,
								std::vector<float>&  afPreciousMax,
								bool bEnd_=false);

	virtual int UpdateProcData(float *pfData_,int iLen_,const CBHDateTime& tDateTime_,int iStartIndex_ =0);

	virtual int UpdateProcSUDStatus(int anProcJudgedPlantStatus,int anLen_,int anStartIndex_=0);

	virtual int GetProcSUDStatus();

	virtual int UpdateDynData(  
								DATATYPE_WAVE *pWave_,
								S_DynCharValue *pDynCharValue_,
								int iAllRealNum_,
								const int &iLen_,
								CBHDateTime & arefDate,
								const int &iStartIndex_,
								const float& fScaleCoef_,
								int anPrecisionEnabled,
								std::vector<float>& afPreciousMin ,
								std::vector<float>&  afPreciousMax,
								const bool &bEnd_=false);


	///计算整个模板的所有自定义参数数据 modified by zdl
	virtual int CalcCustomParamData(int iStartIndex_,int iLen_,CHANNTYPE channType = GE_VIBCHANN);

    ///更新振动数据和对应快变报警状态
	int UpdateVibFAlarmStatus(DATATYPE_ALARMSTATE *piStatus_,int iLen_,int iStartIndex_ =0,bool bEnd_=false);

 	//根据转速类型，得到对应的实时转速。如果bSafe_为true，则使用读写锁，如果false，则不使用读写锁
 //   float GetRealRev(int iRevType_=1,bool bSafe_=true);

	/**添加常规报警日志,针对振动测点.振动报警判断中，如果一个测点报警，则保存所以振动测点的波形数据
	  */
    int AddRegAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_);

	/**添加自定义报警日志,针对振动测点.振动报警判断中，如果一个测点报警，则保存所以振动测点的波形数据
	*/
	int AddCustomAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_);

	/**添加常规报警日志,针对过程量测点。如果一个测点报警，则只保存这个测点的数据。过程量测点互不干扰
	  */
	int AddProcRegAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_);

	/**@brief 添加快变报警日志
	  *@return 报警事件ID，如果小于0，就是错误
	  */
	int AddFastAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_);

	/**某一个测点常规报警结束时,更新结束时间的操作
	  */
	int SetRegAlarmLogEndTime(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_);

	/**某一个测点自定义报警结束时,更新结束时间的操作
	*/
	int SetCustomAlarmLogEndTime(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_);

	/**添加缓变报警日志
	  */
	int AddSlowAlarmLog(IDBInterFace *pDB_,string sName_,CBHDateTime dtStart_,int iType_,int iChannType_,int iThID_);

	/**某一个测点缓变报警结束时,更新结束时间和阈值ID操作
	  */
	int SetSlowAlarmLogEndTimeAndThID(IDBInterFace *pDB_,string sName_,CBHDateTime dtEnd_,int iChannType_,int iThID_);

    /**申请阈值ID
	  *@return 如果小于0，就是失败，大于0就是阈值ID
	  */
	int ReqThID(int iNums_=6);

	/**申请报警事件ID
	  *@return 返回为报警事件ID
	  */
    int ReqAlarmEventID();

	/**得到当前报警事件ID
	*@return 返回当前报警事件ID
	*/
    int GetCurrAlarmEventID(string sChann_,CCustomDB *pDB_);

	///得到指定报警当前索引振动测点周期的起始索引和结束索引
	int GetSpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_);
	int GetSpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_);

	///得到内燃机指定报警当前索引振动测点周期的起始索引和结束索引
	int GetICESpecVibChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_);
	int GetICESpecVibChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_);

	///得到指定报警当前索引动态测点周期的起始索引和结束索引
	int GetSpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_);
	int GetSpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_);

	///得到指定内燃机报警当前索引动态测点周期的起始索引和结束索引
	int GetICESpecDynChannStartEndIndex(int index_,int &iStartIndex_,int &iEndIndex_);
	int GetICESpecDynChannStartEndIndex(string sChann_,int &iStartIndex_,int &iEndIndex_);

	///得到指定径向振动通道的自定义特征值
	int GetSpecRadialVibChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecRadialVibChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	///得到指定动态通道的自定义特征值
	int GetSpecDynChannCustomData(string sChann_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);
	int GetSpecDynChannCustomData(int index_,float *pfCustom_,const int& iStartIndex_ = 0, const int& iLen_ = GD_CUSTOMPARAM_BUFFSIZE);

	CBHDateTime GetRealProcDataDateTime();
	CBHDateTime GetRealProcDataDateTime(string sChann);

	///计算常规报警以及指定的特征值
	int JudgeRegAlarmAndComputeVibEigenValue (S_VibCharValue & arefVibValue,
											int anIndex,
											DATATYPE_WAVE  *  apNivfWave_,
											int anLength);


		/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *            报警确认记录是否保存在数据表中 FIELD_SYSTEM_EVENT_TYPE 1:不保存 0：保存 默认0
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *			   #define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */
	int AlarmConfirm(ICommInterFace & arefComm,IDBInterFace & arefDb);

	///判断过程量快变数据报警
	void SetProcFastChangeChann(vector<int>& vChannNo_);
	void JudgeProcFastAlarm(IDBInterFace *pDB_,int &iAlarmStatus_,int& iEventID_,vector<int>& vChannAlarmStatus_);

	///判断动态快变数据报警
	void SetDynFastChangeChann(vector<int>& vChannNo_);
	void JudgeDynFastAlarm(IDBInterFace *pDB_,int &iAlarmStatus_,int& iEventID_,vector<int>& vChannAlarmStatus_);

	///判断振动快变数据报警
	void SetVibFastChangeChann(vector<int>& vChannNo_);
	void JudgeVibFastAlarm(IDBInterFace *pDB_,int &iAlarmStatus_,int& iEventID_,vector<int>& vChannAlarmStatus_);
	
	///保存振动快变测点报警类型
	void SetChannFastAlarmStatus(E_TBL_CHANNTYPE eChannType, int iAlarmType_,bool bSetValue_ );
	
	/**保存快变报警数据，同步对应4系列中间件，同时兼容旧版本的数采
	  *@param iStartIndex_ 测点的起始索引
	  */
	int SaveVibFAlarmData(int iStartIndex_,
		int iNum_,
		S_VibCharValue *pVibCharValue_,
		unsigned int *piType_,
		DATATYPE_WAVE *pWave_,
		S_CustomParamData *pfCustomParom_,
		string sTempletID_,long lEventID_,
		CBHDateTime dtDataTime_,
		int iMicroSecond_,
		int iRealAllNum_,
		IDBInterFace *pDB_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax);

	/**保存动态报警数据,同步对应4系列中间件
	*@param iStartIndex_ 测点的起始索引
	*/
	int SaveDynAlarmData(int iStartIndex_,
		int iNum_,
		S_DynCharValue *pDynCharValue_,
		unsigned int *piType_,
		DATATYPE_WAVE *pWave_,
		S_CustomParamData *pfCustomParom_,
		string sTempletID_,
		long lEventID_,
		CBHDateTime dtDataTime_,
		int iMicroSecond_,
		int iRealAllNum_,
		IDBInterFace *pDB_,
		const float& fScaleCoef_,
		int anPrecisionEnabled,
		std::vector<float>& afPreciousMin ,
		std::vector<float>&  afPreciousMax);

	/**保存过程量报警数据,同步对应4系列中间件*/
	int SaveProcAlarmData( IDBInterFace * pDB_, int AlarmType_, int EventID_, CBHDateTime DateTime_, int iMicroSecond_, CString ChannNo_, int ChannType, float ChannValue, int Rev_ );

private:
	///振动测点的波形字段
	vector<CString>  _vVibWaveField;
	///动态测点的波形字段
	vector<CString> _vDynWaveField;

private:
	CBHDateTime _timeFirstRun;
	bool      _bFirstRun;
	//如果首次运行24小时后，就开始判断报警
    bool      _bJudgeAlarm;
	//报警日志事件ID锁
	IReadWriteLock *_pLockAlarmEventID;
    friend CProcChannParamAlarm;
	vector<int> _vVibFastChangeChannNo;
	vector<int> _vDynFastChangeChannNo;
	vector<int> _vProcFastChangeChannNo;

	vector<int> _vVibChannAlarmStatus;
	vector<int> _vDynChannAlarmStatus;
	vector<int> _vProcChannAlarmStatus;
public:
	/************************************************************************/
	/* 1.计时器                                                               */
	/************************************************************************/
	COleDateTime m_oLastNewAlarmTime;			//timer1 新报警事件的计时器
	COleDateTime m_oLastSaveHalfIndenseTime;	//timer2 新报警发生后20分钟内30秒保存一次计时器
	COleDateTime m_oLastSaveSparseTime;			//timer3 旧报警每30分钟保存一组的计时器
	COleDateTime m_oHalfNormalTime;				//timer4 半正常状态维持时间计时器

	int m_nNewAlarmMaxTime;					    //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
	int m_nSaveIndenseTime;						//密集保存时间间隔,								默认5分钟
	int m_nSaveHalfIndenseTime;					//半密集保存时间间隔,							默认30秒
	int m_nHalfNormalNotSaveTime;				//半正常状态下持续保存的时间长度，				默认20分钟
	int m_nSaveSparseTime;						//就报警每30分钟保存一组的时间间隔,				默认30分钟
	int m_nHalfNormalTime;						//半正常状态维持时间间隔						默认2个小时
	int m_nChannHalfNormalTime;					//测点半正常状态维持时间间隔					默认1个小时

	void ResetNewAlarmTime();					//重置 timer1 新报警事件的计时器
	void ResetSaveHalfIndenseTime();			//重置 timer2 新报警发生后20分钟内30秒保存一次计时器
	void ResetLastSaveSparseTime();				//重置 timer3 旧报警每30分钟保存一组的计时器
	void ResetHalfNormalTime();					//重置 timer4 半正常状态维持时间计时器

	
	/************************************************************************/
	/* 2.设备状态                                                              */
	/************************************************************************/
	enum _PLANT_STATUS_
	{
		PLANT_STATUS_NORMAL               = 0x00000001,
		PLANT_STATUS_HALF_NORMAL          = 0x00000010,
		PLANT_STATUS_OLD_ALARM_LOW		  = 0x00000100,
		PLANT_STATUS_OLD_ALARM_HIGH		  = 0x00001000,
		PLANT_STATUS_NEW_ALARM			  = 0x00010000,
	};



	_PLANT_STATUS_ PredictVibNewStatus(IDBInterFace *pDB_,bool bConfirmAlarm_ = true);											 //预测设备的新的状态，返回值为新状态
	BOOL SwitchStatus(IDBInterFace *pDB_,_PLANT_STATUS_ anNewStatus,_PLANT_STATUS_ arefNoldStatus,bool abShouldSaveData = true);  //切换到新的状态,返回值 0.表示切换失败 1表示切换成功
	BOOL DoSwitchThings(IDBInterFace *pDB_,_PLANT_STATUS_ anStatus,_PLANT_STATUS_ arefNoldStatus,bool abShouldSaveData = true);	 //处理切换操作前需要进行的动作
	_PLANT_STATUS_ ComputeNewStatusForPredict(IDBInterFace * pDB_,_PLANT_STATUS_ anNewStatus);		 //根据测点的下一状态计算设备的下一状态
	BOOL DoSwitchChannsThings(IDBInterFace *pDB_,_PLANT_STATUS_ anNewStatus,bool abShouldSaveData = true);						 //切换全部测点报警状态
	BOOL StopAllChannsAlarmLogForNewAlarm(IDBInterFace *pDB_);	//所有测点的报警日志全部标识为停止，重置测点计数器timer5
	BOOL StopProcChannsAlarmLogForNewAlarm(IDBInterFace *pDB_);

	_PLANT_STATUS_ m_nInnerPlantAlarmStatus;
	CAlarmPlantInfo::_PLANT_STATUS_ InnerPlantAlarmStatus() const;							//设备当前状态
	void InnerPlantAlarmStatus(CAlarmPlantInfo::_PLANT_STATUS_ val);


	/************************************************************************/
	/* 3. 设备状态切换时需要进行的动作，日志操作在测点完成														*/
	/************************************************************************/

	BOOL ShouldSaveData(IDBInterFace *pDB_, _PLANT_STATUS_ anStatus);  //是否需要保存数据
	BOOL SaveVibData(IDBInterFace *pDB_,int iEventID_,int iCharValueIndex_,int iWaveIndex_,float *pfRev_,int iSmpNums_,int iSmpFreq_,bool &bSaved_,CBHDateTime& tSaveTime_);		  //保存振动数据
	BOOL SaveProcData(IDBInterFace *pDB_,int iEventID_,int iCharValueIndex_,int iWaveIndex_,float *pfRev_,int iSmpNums_,int iSmpFreq_);      //保存过程量数据
	BOOL SaveDynData(IDBInterFace *pDB_,int iEventID_,int iCharValueIndex_,int iWaveIndex_,float *pfRev_,int iSmpNums_,int iSmpFreq_,bool &bSaved_,CBHDateTime& tSaveTime_);       //保存动态数据
	
	/**当前一次添加这次报警日志需要调用的接口
	  *@param sName_ 通道名字或者其它标识
	  *@param dtStart_ 报警开始时间
	  *@param iType_   报警类型
	  *@param iChannType_ 通道类型
	  *@param iThID_    阈值ID
	  *@return 返回申请添加这次报警日志的事件ID,小于0表示失败
	  */
	int AddAlarmLogEx(IDBInterFace *pDB_,string sName_,long & iID_,long & iEventID_,COleDateTime dtStart_,int iType_,int iChannType_,int iThID_);

	void IncreaseEventID();

	BOOL m_bAlarmHasOccoured;
	COleDateTimeSpan _timeNetOffSaveVibHour;
	COleDateTimeSpan _timeNetOffSaveVibDay;
	COleDateTimeSpan _timeNetOffSaveVibMonth;
	COleDateTimeSpan _timeNetOffSaveDynHour;
	COleDateTimeSpan _timeNetOffSaveDynDay;
	COleDateTimeSpan _timeNetOffSaveDynMonth;
	COleDateTimeSpan _timeNetOffSaveProcHour;
	COleDateTimeSpan _timeNetOffSaveProcDay;
	COleDateTimeSpan _timeNetOffSaveProcMonth;
	BOOL AlarmHasOccoured() const;
	void AlarmHasOccoured(BOOL val);
	
	//将报警数据存到趋势表
	int SaveVibAlarmDetailDataToTrend( IDBInterFace *pDB_,bool bSaved_,CBHDateTime tSaveTime_);
	
	//停止全部未停止的报警日志
	int SetAllAlarmLogEnd(IDBInterFace *pDB_,CBHDateTime & areBHTime);

	void SaveVibRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_ =false );
	int SaveVibRecord(IDBInterFace * pDB_,string sTableNameSrc_,string sTableNameDes_,CBHDateTime tTime_,int iCurID_,bool bMonth_ =false );
	void SaveDynRecord(IDBInterFace * pDB_,const string &sTableName_,const int &iCurID_,const bool &bMonth_=false );
	int SaveDynRecord(IDBInterFace * pDB_,const string &sTableNameSrc_,const string& sTableNameDes_,const CBHDateTime& tTime_,const int &iCurID_,const bool &bMonth_=false );
	void SaveDynAlarmDetailDataToTrend(IDBInterFace * pDB_,bool bSaved_,CBHDateTime tSaveTime_);
	void SaveProcRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,string sChannNo_,CBHDateTime tTime_,bool bMonth_ =false );
	void  SaveProcAlarmDetailDataToTrend(IDBInterFace * pDB_,string sChannNo_,CBHDateTime tTime_);


	CRealPlantInfo* _pRealPlantInfo;
	void SetIDPointer(CRealPlantInfo *pRealPlantInfo_);


	virtual void GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_);
	virtual void GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_);
	virtual void GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_);
	virtual void GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_);

	virtual void GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_);
	virtual void GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_);
	virtual void GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_);
	virtual void GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_);

	virtual void GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_);
	virtual void GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_);
	virtual void GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_);
	virtual void GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_);

	int JudgeProcAlarmEx(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);

	int JudgeFastProcAlarmEx(IDBInterFace *pDB_,bool bSaveData_ = true,bool bConfirmAlarm_ = true);

	string _sProcField; //过程量字段

	/************************************************************************/
	/* 直流量新报警逻辑处理                                                 */
	/************************************************************************/
	BOOL m_bIsEnabledNewDCLogic; //是否启用新的直流量报警逻辑，默认为否
	BOOL IsEnabledNewDCLogic() const;
	void IsEnabledNewDCLogic(BOOL val);

	int m_nLastRunningStatus;    //上一次的设备运行状态
	int LastRunningStatus() const;
	void LastRunningStatus(int val);

	int m_nCurrentRunningStatus; //当前的设备运行状态
	int CurrentRunningStatus() const;
	void CurrentRunningStatus(int val);

	BOOL DealWithNewDCLogic(); //处理新的直流量逻辑
	BOOL DealWithChannsRodsinkLogic(BOOL abRunningStatusChanged); //处理测点的直流量逻辑

	BOOL DealWithNewAxisLogic(); //新的轴位移逻辑
	BOOL DealWithChannsAxisLogic(BOOL abRunningStatusChanged); //处理测点的轴位移逻辑

	//一台设备分两个数采发送时，非快变测点数据拷贝普通趋势数据存储
	int SaveVibFastDataFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_);

	int SaveDynFastDataFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_);

	bool IsRecordExists(IDBInterFace *pDB_,string sTableName_,int iEventID_,CBHDateTime dtTime_,int iMillsecond);

	//一台设备分多个数采发送数据时，后处理数据从趋势数据拷贝补全
	int InsertVibAlarmRecordFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_);
	
	//一台设备分多个数采发送数据时，后处理数据从趋势数据拷贝补全
	int InsertDynAlarmRecordFromTrendData(IDBInterFace *pDB_,int iEventID_,CBHDateTime dtTime_,int iMillSecond_,int iStartIndex_,int iLen_);

public:
	void NewAlarmAddLog(bool bAddLog);
	bool NewAlarmAddLog();
	void IncVibFastChangeAlarmAddLogTime(const CBHDateTime& dtTime_);
	void DescVibFastChangeAlarmAddLogTime(const CBHDateTime& dtTime_);
	void IncDynFastChangeAlarmAddLogTime(const CBHDateTime& dtTime_);
	void DescDynFastChangeAlarmAddLogTime(const CBHDateTime& dtTime_);
	bool VibAlarmAddLogTimeExists(const CBHDateTime& dtTime_);
	bool DynAlarmAddLogTimeExists(const CBHDateTime& dtTime_);

	int  QueryVibAlarmEventIDByTime(IDBInterFace *pDB_, const CBHDateTime& dtTime_);
	int  QueryDynAlarmEventIDByTime(IDBInterFace *pDB_, const CBHDateTime& dtTime_);

	int  UpdateAlarmSyncFlag(IDBInterFace *pDB_,const int& iEventID_,const CBHDateTime& dtTime_);


	CDieselPlant * GetDieselPlantByType(E_MACHINE_TYPE& eMachineType_);


private:
	//设备发生新报警添加日志
	bool m_bNewAlarmAddLog;
	map<CBHDateTime,CBHDateTime> m_mtVibFastChangeAlarmLogTime;
	map<CBHDateTime,CBHDateTime> m_mtDynFastChangeAlarmLogTime;


	HMODULE  m_hLibHandle;

public:
	float GetPlantPower();
	void SetPlantPower(float afPower_);
	int GetLowPressRev();
	void SetLowPressRev(int nRev_);
	int GetHighPressRev();
	void SetHighPressRev(int nRev_);

private:
	int m_nLowPressRev;
	int m_nHighPressRev;
	float   m_fPower;


};

 int ParseAlarmStatus(unsigned int iState_);