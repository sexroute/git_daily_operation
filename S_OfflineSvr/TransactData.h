#pragma once
#include "StdAfx.h"
#include "PlantInfo.h"
#include "additions/BHOleDbWrapper.h"
/**用于处理调用
*/
typedef pair<CThreshold *,int> P_TH;
typedef vector<P_TH>       VP_TH;
typedef pair<string,string> P_CHANNIDNO;
typedef list<P_CHANNIDNO> lCHANNID_NO;
typedef pair<int,int> P_MAXMINREV;

extern int IsEnableSearchAllTable();

class CTransactData
{;
public:
	CTransactData(void);
	~CTransactData(void);
	// 得到当前趋势
	int GetCurTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);

private:
    ///判断通道的所属大类。及判断通道一级分类
	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);
    ///根据报警类型枚举得到文字描述
	CString GetAlarmTypeDiscribe(unsigned int iAlarmType_);

	///向通信接口中填写报警限、单位和测点名信息
	void WriteChannInfoToFixComm(ICommInterFace *pFixComm_,S_ChannInfo channInfo_);
	// 得到表名前缀
	// 通过测点类型、时间间隔得到从那个数据库表名的前半部分.单中获得数据和从几条记录中取一条记录的个数。
	CString GetTblNamePrefixForTrend(int ag_iChannType,CBHDateTime ag_timeStart,CBHDateTime ag_timeEnd);
	//取波形数据时的表名
	CString GetTblNamePrefixForWave(int ag_iChannType,CBHDateTime timeEnd_,int& iTableType_);

	//获取表的前缀
	CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

	//获取表的前缀
	CString GetTblNamePrefixSUD(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);


	//获取表的前缀包括启停车，报警
	CString GetTblNamePrefixAlarmSud(const CHANNTYPE &iChannType_,GE_DATA_SEARCH_MODE  iTableMode);
	//根据时间来判断其所属的表类型
	E_TREND_TBL_TYPE GetTrendTblType(CBHDateTime time_);
	//根据时间来判断其所属的表类型
	E_TREND_TBL_TYPE GetTrendTblTypeEx(const CHANNTYPE &iChannType_,IDBInterFace *pDB_,CBHDateTime time_,const string& sSuffix_);

	E_TREND_TBL_TYPE GetTrendTblTypeNotOverwrite(CBHDateTime & arefTimeStart,CBHDateTime & arefTimeEnd);


	/**得到指定阈值ID的阈值结构体
	  *@param vID 阈值ID向量
	  *@return 阈值信息
	  */
	VP_TH GetSpecIDThreshold(vector<int> vID,IDBInterFace *pDB_,string sSuffix_);

	CThreshold GetSpecIDThreshold(int iID_,IDBInterFace *pDB_,string sSuffix_);

	/**获得读取趋势的记录
	  *@param pDB 数据库联接指针
	  *@param sTblNameSuffix_ 数据库名后缀
	  *@param sShowField_ 需要的数据字段
	  *@param iChannType_ 测点类型
	  *@param iTblType_ 趋势表类型
	  *@param timeStart_  开始时间。引用方式，如果需要下一级表，这个时间就为新的开始时间
	  *@param timeEnd_    结束时间
	  *@param bNeedSubTbl_ 引用方式，传递出是否需要下级数据
	  *@return 如果为false，说明没有记录可读，否则就可以读记录
	  */
	bool GetTrendRecords(IDBInterFace *pDB,string sTblNameSuffix_,string sShowField_,const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE &iTblType_,CBHDateTime &timeStart_,CBHDateTime timeEnd_,bool & bNeedSubTbl_);

	bool GetTrendRecordsEx(IDBInterFace *pDB,string sTblNameSuffix_,string sShowField_,const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE &iTblType_,CBHDateTime &timeStart_,CBHDateTime timeEnd_,bool & bNeedSubTbl_,int iInterval_);
	
	int GetTrendRecordsNum(IDBInterFace *pDB,string sTblNameSuffix_,string sShowField_,const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE &iTblType_,CBHDateTime &timeStart_,CBHDateTime timeEnd_,bool & bNeedSubTbl_);
	
	vector<CBHDateTime> GetTrendFastRecordTimeArray(IDBInterFace *pDB,string sTblNameSuffix_,const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE& iTblType_,CBHDateTime &timeStart_,CBHDateTime & timeEnd_);

	CString GetTimeStringByTimeArray(const vector<CBHDateTime>& vDateTime_);
	/*
	 * 增加跨表查询功能
	 */
	bool GetTrendRecordsSearch(IDBInterFace *pDB,
						string sTblNameSuffix_,
						string sShowField_,
						const CHANNTYPE &iChannType_,
						E_TREND_TBL_TYPE &iTblType_,
						CBHDateTime &timeStart_,
						CBHDateTime timeEnd_,
						bool & bNeedSubTbl_,
						int SearchMode);

	// 得到表名前缀
	// 通过测点类型、时间间隔得到从那个数据库表名的前半部分.单中获得数据和从几条记录中取一条记录的个数。
	CString GetTblNamePrefixForTrend2(	const CHANNTYPE &iChannType_,
										CBHDateTime timeStart_,
										CBHDateTime timeEnd_,
										E_TREND_TBL_TYPE& iTblType_,
										bool & bSubTbl_,
										bool &bSupTbl_);

	CString GetTblNamePrefixForTrendNotOverWrite(	const CHANNTYPE &iChannType_,
													CBHDateTime timeStart_,
													CBHDateTime timeEnd_,
													E_TREND_TBL_TYPE& iTblType_,
													bool & bSubTbl_,
													bool &bSupTbl_);
	//取波形数据时的表名
	CString GetTblNamePrefixForWave2(const CHANNTYPE &iChannType_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_);

	//取波形数据时的表名
	CString GetTblNamePrefixForWave2Ex(const CHANNTYPE &iChannType_,IDBInterFace *pDB_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_, const string& sSuffix_);

	BOOL GetSpecWaveByTimeRange(
		IDBInterFace *pDB_,
		const CString& sTableName_,
		E_TREND_TBL_TYPE iTblType_,
		CBHDateTime& timeSpec_,
		const CString& sShowField_,
		int iSampNum_,
		const CString& sChannNo_,
		int iDataSource_);
	
	CString GetTblNamePrefixForWaveNotOverWrite(const CHANNTYPE &iChannType_, 
												CBHDateTime timeStart_, 
												CBHDateTime timeEnd_,
												E_TREND_TBL_TYPE& iTblType_);


	///得到指定趋势类型的截止时间
	CBHDateTime GetSpecTrendTimeOff(E_TREND_TBL_TYPE  iTblType_);

	// 读取振动测点小时、天、月趋势
	int ReadVibTrendSpecType(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		          int iCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);
	// 读取振动测点所有特征值小时、天、月趋势
	int ReadVibTrendAllCharValue(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		          CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);
	int ReadVibTrendAllCharValueEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
				  CBHDateTime timeStart_,CBHDateTime timeEnd_,int iChannType_);
	/// 读取动态测点小时、天、月趋势。
	/// iCharType_如果为-5就表示要读取所有特征值
	int ReadDynTrendSpecType(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		int iCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);

	// 读过程量小时、天、月趋势
	int ReadProcTrendSpecType(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		              CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);

	// 读取振动测点小时、天、月趋势
	int ReadVibTrendAmpAndPhase(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string ag_strPlant,\
		string ag_strChann1, string ag_strChann2, string ag_strChann3,string ag_strChann4,\
		int ag_iCharType,CBHDateTime ag_timeStart,CBHDateTime ag_timeEnd,bool ag_bCurr=false);

	// 读取振动测点所有自定义参数小时、天、月趋势
	int ReadVibCustomParamTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);

	/// 读取动态测点所有自定义参数小时、天、月趋势。
	int ReadDynCustomParamTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);

	/**读取指定测点、时间的波形和频谱，并赋给网络连接指针
	  */
	int ReadVibChannSpecTimeWaveAndSpectrum(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,\
		int iLines_,int iWaveNum_,CBHDateTime timeSpec_);

	/**读取指定测点、时间的波形和细化频谱，并赋给网络连接指针
	*/
	int ReadVibChannSpecTimeWaveAndZoomSpectrum(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,\
		int iLines_,int iWaveNum_, CBHDateTime timeSpec_, float iStartFreq_, float iStopFreq_ = -1);

	/**读取指定测点、时间的波形和倒谱，并赋给网络连接指针
	*/
	int ReadVibChannSpecTimeWaveAndCepstrum(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,\
		int iWaveNum_, CBHDateTime timeSpec_);

	/**读取指定振动测点、时间的波形和所有特征值
	  */
	int ReadVibChannSpecTimeWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, int &iWaveNum_, S_VibCharValue &vibCharValue_, int &iSmpFreq_, float &fRev_, CBHDateTime &timeSpec_,float* pfWave_,CWaveHead& sWaveHead_,bool bExactTime_=false);

	/**读取指定动态测点、时间的波形和所有特征值
	  */
	int ReadDynChannSpecTimeWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_,const string &sFactory_, const string &sPlant_, const string &sChann_,\
		 DATATYPE_WAVE *pWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, CBHDateTime &timeSpec_,float *pfWave_,CWaveHead& sWaveHead_,bool IsExactTime_=true);

	/**读取指定振动测点、时间的特征值和报警限信息，并赋给网络连接指针
	*/
	int ReadVibChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_=0,bool bExactTime_=true,float *pfRev_=NULL);

	int ReadVibChannSpecTimeCharValueInfoEx(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_=0,bool bExactTime_=true,float *pfRev_=NULL);

	/**读取指定振动测点、时间的特征值和报警限信息，并赋给网络连接指针
	*/
	int ReadVibChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,lCHANNID_NO lChann_,CBHDateTime timeSpec_);

	/**读取指定动态测点、时间的特征值和报警限信息，并赋给网络连接指针
	*/
	int ReadDynChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_=0,bool bExactTime_=true);

	/**读取指定动态测点、时间的特征值和报警限信息，并赋给网络连接指针
	*/
	int ReadDynChannSpecTimeCharValueInfoEx(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,int iCharType_=0,bool bExactTime_=true);
	/**读取指定振动测点、事件ID的报警数据特值等信息，并赋给网络连接指针
	  */
	int ReadVibChannSpecEventIDAlarmData(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,lCHANNID_NO lChann_,int iEventID_);

	/**读取指定过程量测点、时间的特征值和报警限信息，并赋给网络连接指针
	*/
	int ReadProcChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,lCHANNID_NO lChann_,CBHDateTime timeSpec_);

	int ReadProcChannSpecTimeCharValueInfo(ICommInterFace *pFixComm_,IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,string sChann_,CBHDateTime timeSpec_,bool bExactTime_=true);
	/* 获取离线数据源过程量测点的特征值*/
	int GetProcChannCharValUsingOnlineDataSourcByTime(
	             ICommInterFace *pFixComm_,IDBInterFace *pDB_,
	             const S_ChannInfo &channInfo_, const CBHDateTime & timeSpec_,
	             const string &sSuffix, bool bExactTime_);

	/* 获取在线数据源过程量测点的特征值*/
	int GetProcChannCharValUsingOfflineDataSourcByTime(
	             ICommInterFace *pFixComm_,IDBInterFace *pDB_,
	             const S_ChannInfo &channInfo_, const CBHDateTime & timeSpec_,
	             const string &sSuffix, bool bExactTime_);

	/* 根据转速类型获取转速*/
	void SetRevByRevType(ICommInterFace *pFixComm_,IDBInterFace *pDB_, int iRevType_);

	///往复压缩机用 读取振动测点所有特征值小时、天、月趋势
	int ReadVibSpecCharValueTrend(
		ICommInterFace * pFixComm, IDBInterFace *pDB,
		string sCompany_,string sFactory_,string sPlant_,
		char *pcChannBuf_,int iChannNum_,
		int iClyCharType_, int iTraCharType_, int iOtherCharType_,
		CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);
	
	int ReadVibSpecCharValueTrendEx(
		ICommInterFace * pFixComm, IDBInterFace *pDB,
		string sCompany_,string sFactory_,string sPlant_,
		char *pcChannBuf_,int iChannNum_,
		int iClyCharType_, int iTraCharType_, int iOtherCharType_,
		CBHDateTime timeStart_,CBHDateTime timeEnd_);

	/// 往复压缩机用 读取动态测点小时、天、月趋势。
	/// iCharType_如果为-5就表示要读取所有特征值
	int ReadDynSpecCharValueTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		int iPreCharType_,int iRodCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);

	int ReadDynSpecCharValueTrendEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		int iPreCharType_,int iRodCharType_,CBHDateTime timeStart_,CBHDateTime timeEnd_);

	///往复压缩机用  读过程量小时、天、月趋势
	int ReadProcSpecValueTrend(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		CBHDateTime timeStart_,CBHDateTime timeEnd_,bool bCurr_=false);
	int ReadProcSpecValueTrendEx(ICommInterFace * pFixComm, IDBInterFace *pDB,string sCompany_,string sFactory_,string sPlant_,char *pcChannBuf_,int iChannNum_,\
		CBHDateTime timeStart_,CBHDateTime timeEnd_);

	/**构造趋势表中的公共字段
	  *@return 字段串
	  */
	CString ConsCommonField();

	/**构造指定振动测点在趋势表中的字段
	  *@param sChann_ 指定的测点名
	  *@param bWave_ 是否需要波形字段
	  *@return 构造的测点特征值字段
	  */
	CString ConsVibChannField(string sChann_,bool bWave_=false);

	/**构造指定振动测点指定类型在趋势表中的字段
	*@param sChann_ 指定的测点名
	*@param iType_ 特征值类型。除了枚举的特征值类型外，如果为-1，则构造波形字段；如果为-2，则类型字段（报警数据表使用）
	*@return 构造的测点特征值字段
	*/
	string ConsVibChannSpecTypeField(string sChann_,int iType_=GE_ALLCHAR);


	/**构造指定动态测点在趋势表中的字段
	*@param sChann_ 指定的测点名
	*@param bWave_ 是否需要波形字段
	*@return 构造的测点特征值字段
	*/
	CString ConsDynChannField(string sChann_,bool bWave_=false);

	/**构造指定动态测点指定类型在趋势表中的字段
	  *@param sChann_ 指定的测点名
	  *@param iType_ 特征值类型。除了枚举的特征值类型外，如果为-1，则构造波形字段
	  *@return 构造的测点特征值字段
	  */
	string ConsDynChannSpecTypeField(string sChann_,int iType_=0);

	/**从当前数据库连接中读取指定动态测点的各种特征值
	*@param sChann_ 振动测点名
	*@param pDB_  数据库连接指针
	*@return 振动特征值
	*/
	S_DynCharValue ReadDynCharValue(string sChann_,IDBInterFace *pDB_);

	/**从当前数据库连接中读取指定振动测点的各种特征值
	  *@param sChann_ 振动测点名
	  *@param pDB_  数据库连接指针
	  *@return 振动特征值
	  */
	S_VibCharValue ReadVibCharValue(string sChann_,IDBInterFace *pDB_);

	/**比较两个振动特征值结构体中各种特征值的最大值,并返回最大值
	  */
    S_VibCharValue MaxVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_);

	/**比较两个振动特征值结构体中各种特征值的最小值,并返回最小值
	*/
	S_VibCharValue MinVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_);

	/**两个振动特征值结构体中各种特征值的相加,并返回相加后结果
	*/
	S_VibCharValue SumVibCharValue(S_VibCharValue vibValue1_,S_VibCharValue vibValue2_);

	/**解析buffer中的测点名
	  *@param pChannBuf_  测点序列缓冲区
	  *@param iNum_       测点序列缓冲区中测点的个数
	  *@return 测点联表
	  */
    list<string>  ParseChannList(char *pChannBuf_,int iNum_);

	/**解析buffer中的测点名，并根据设备指针解析出对应的测点No，最后输出测点名和测点No的list链表
	  *@param pChannBuf_  测点序列缓冲区
	  *@param iNum_       测点序列缓冲区中测点的个数
	  *@return 测点ID和测点No映射联表
	  */
	lCHANNID_NO  ParseVibChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,int iNum_);
	lCHANNID_NO  ParseDynChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,const int &iNum_);
	lCHANNID_NO  ParseProcChannList(CPlantInfoOffLine *pPlantInfo_,char *pChannBuf_,int iNum_);

	/**从指定的数据库连接指针中获取相关测点信息
	  *@param pDB 数据库连接指针
	  *@return 测点详细信息
	  */
    S_ChannInfo ReadChannInfo(IDBInterFace *pDB_);

	// get specification channel sensor type and the type of rev
	int GetSpecChannSensorType(string sCompany_,string ag_strPlant, string ag_strChann, int &iRevType_,IDBInterFace *pDB);

  /**从指定网络连接中读取并判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	  *@param sCompany_ 公司名，是一个引用
	  *@param sFactory_ 分厂名，是一个引用
	  *@param sPlant_ 设备名，是一个引用
	  *@param sAlias_ 是一个引用，用于把公司别名传出来
	  *@param pFixComm_ 网络连接指针
	  *@param bRtn_  是否返回网络连接信息
	  *@return 如果验证成功，则返回1，否则返回NULL
	  */
    CPlantInfoOffLine * ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);


	/**判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	  *@param sCompany_ 公司名，是一个引用
	  *@param sFactory_ 分厂名，是一个引用
	  *@param sPlant_ 设备名，是一个引用
	  *@param sAlias_ 是一个引用，用于把公司别名传出来
	  *@param pFixComm_ 网络连接指针
	  *@param bRtn_  是否返回网络连接信息
	  *@return 如果验证成功，则返回1，否则返回NULL
	  */
    CPlantInfoOffLine * CheckCompanyInfo(const string &sCompany_, const string &sFactory_, const string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);


	///解释报警状态，如果返回0，说明没有报警，1表示常规低报，2表示常规高报，3表示快变报警，4表示换变报警，5表示趋势报警。
	///数字越小，越优先,即如果存在常规报警情况下，就不会向下判断，是否有快变、缓变等。
	int ParseAlarmStatus(unsigned int iState_);

	int SpectrumAnalysis(CPlantInfoOffLine * pPlant_, DATATYPE_WAVE *pWave_,int iLen_,DATATYPE_WAVE *pSpectrum_,int &iLines_,int iSensorType_,int iSmpFreq_,float &fDf_,float* pfWave_,float * pfSpectrum_,CWaveHead& sWaveHead_);
	int SpectrumAnalysis(CPlantInfoOffLine * pPlant_, DATATYPE_WAVE *pWave_,int iLen_,DATATYPE_WAVE *pSpectrum_,int &iLines_,int iSensorType_,int iSmpFreq_,float &fDf_, float &fStartFreq_, float fStopFreq_,float* pfWave_,float * pfSpectrum_,CWaveHead& sWaveHead_);

	//读取指定振动测点、报警ID和时间的波形和所有特征值
	int ReadVibChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_,DATATYPE_CHAR* pfWave_, int &iWaveNum_, S_VibCharValue &vibCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_);
    //读取指定动态测点、报警ID和时间的波形和所有特征值
	int ReadDynChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_,DATATYPE_CHAR* pfWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_);

	//读取指定振动测点、报警ID和时间的波形和所有特征值
	int ReadVibChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, int &iWaveNum_, S_VibCharValue &vibCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_);
	//读取指定动态测点、报警ID和时间的波形和所有特征值
	int ReadDynChannSpecAlarmWaveAndCharValue(ICommInterFace *pFixComm_,IDBInterFace *pDB_,const string &sCompany_, const string &sFactory_, const string &sPlant_, const string &sChann_, DATATYPE_WAVE *pWave_, int &iWaveNum_, S_DynCharValue &dynCharValue_, int &iSmpFreq_, float &fRev_, const CBHDateTime &timeSpec_,const int &iEventID_,CWaveHead& sWaveHead_);

public:
	// 得到指定机组、测点和时间的波形数据
	int GetSpecFreqWave(ICommInterFace * pFixComm, IDBInterFace * pDB);
	// 得到指定机组、测点、时间段的历史趋势
	int GetTrend(ICommInterFace * pFixComm, IDBInterFace *pDB);
	/*!得到指定测点、时间的基本报警信息
	*/
	int GetSpecVibChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/*!得到指定起止时间的指定机组和指定振动测点类型的报警记录
	*/
	int GetVibChannAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
	*/
	int GetVibChannAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定时间的指定测点的报警记录的波形和各种特征值，对于多个测点就循环得到
	*/
	int GetSpecAlarmTimeRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);
	// 得到指定振动测点和指定时间段的所有类型的趋势数据
	int GetAVibChannTrend(ICommInterFace * pFixComm, IDBInterFace * pDB);
	// 得到轴心位置的历史趋势
	int GetAxisLocationTrend(ICommInterFace * pFixComm, IDBInterFace * pDB);
	//指定机组和时间范围的启停车
	int GetSpecTimeRangeSud(ICommInterFace * pFixComm, IDBInterFace *pDB);
	//得到指定时间、机组和测点的启停车数据
	int GetSpecTimeChannSud(ICommInterFace * pFixComm, IDBInterFace *pDB);
	//得到历史在线指定时间、机组和测点的启停车数据
	int GetSpecTimeChannSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB);
    //得到指定时间、机组和测点的启停车详细数据
	int GetSpecChannRevSud(ICommInterFace * pFixComm, IDBInterFace *pDB);
    //得到指定数据保存时刻,机组和测点的启停车详细数据
	int GetSpecChannSaveTimeSud(ICommInterFace * pFixComm, IDBInterFace *pDB);
	//得到临时在线指定数据保存时刻,机组和测点的启停车详细数据
	int GetSpecChannSaveTimeSudEx(ICommInterFace * pFixComm, IDBInterFace *pDB);
	//得到临时在线指定时间,机组和测点的启停车详细数据
	int GetSpecChannSudByTime(ICommInterFace * pFixComm, IDBInterFace *pDB);
	//得到指定机组、测点和时间细化谱数据和波形数据
	int GetZoomSpec(ICommInterFace * pFixComm, IDBInterFace *pDB);

	//得到指定机组、测点和时间的一组轴心轨迹的波形数据。
	int GetAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);
	/**得到指定公司、分厂、机组、时间段、任意多个振动测点的所有特征值趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
	  */
	int GetVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);
	
	int GetVibChannsTrendEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到当前时间向后推一个趋势类型的趋势数据。指定公司、分厂、机组、任意多个振动测点的所有特征值趋势数据，
	*通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
	#define     SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND      21018
	*/
	int GetCurrVibChannsTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定两个振动测点的历史波形（循环两次得到两个测点的数据）
	#define     SERVICE_SERVICE_OFF_GETTWOCHANNDATA           21019
	*/
	int GetTwoChannData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、设备的任意多个过程测点的值信息、报警限等信息（依次循环测点的数据）
	  #define     SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO   21020
	  */
	int GetSpecPlantProcChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定两个振动测点的特征值信息、报警限等信息（循环两次得到两个测点的数据）
	#define     SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO    21006
	*/
	int GetTwoChannDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、设备的任意多个振动测点的特征值信息、报警限等信息（依次循环测点的数据）
	*输入参数：
	#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO   21009
	*/
	int GetSpecPlantChannsDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
	*输入参数：
	#define     SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO       21023
	*/
	int GetSpecPlantChannInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、测点和报警事件ID的轴心位置趋势。
	  #define     SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND       21025
	  */
	int GetAxesLocationAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/**得到指定报警事件的报警数据保存的开始截止时间
	#define SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE             21026
	*/
	int GetAlarmDataTimeRange(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定轴心位置报警事件、时间的报警数据
	#define SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA            21027
	*/
	int GetAxesLocationAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、振动测点、报警事件ID的振动详细数据趋势。
	#define     SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND          21028
	*/
	int GetVibAlarmInfoTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、轴位移测点、报警事件ID的数据趋势。
	#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND      21029
	*/
	int GetAxisDisplaceAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**得到指定公司、分厂、机组、过程量测点、报警事件ID的数据趋势。
	#define     SERVICE_SERVICE_OFF_GETPROCALARMTREND        21030
	*/
	int GetProcAlarmTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、过程量测点、报警事件ID、时间的数据。
	#define     SERVICE_SERVICE_OFF_GETPROCALARMDATA        21031
	*/
	int GetProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、测点、报警事件ID的趋势报警数据。
	#define     SERVICE_SERVICE_OFF_GETTRENDALARMDATA        21032
	*/
	int GetTrendAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、轴位移测点、报警事件ID、时间的详细数据。
	#define     SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA    21033
	*/
	int GetAxisDisplaceAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**得到指定时间的指定测点的报警记录的各种特征值，对于多个测点就循环得到
	  #define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE    21034
	  */
	int GetSpecAlarmRecordCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/*!得到指定公司、分厂、机组、任意多测点的指定启停车事件的趋势数据。
	*/
	int GetSpecEventChannsSudTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/*!得到指定公司、分厂、机组、事件、任意多测点、保存时间的详细数据
	#define    SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO    21022
	*/
	int GetSpecEventTimeChannsSudDetailInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定时间的月、公司、分厂、机组各个测点的各种特征值最大值最小值平均值，测点特征值buff中依次是最大值、最小值和平均值，特征值个数应该是3
	#define SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT        21051
	*/
	int GetSpecPlantMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂、指定时间范围的报警信息、启停车信息。
	*振动通道报警次数、过程量通道报警、累计停车次数、累计停机时间、累计运行时间，时间单位为小时
	#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052
	*/
	int GetSpecTimeRangeFactoryRunInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**关联诊断。这个报警ID的开始截止时间范围内，返回的记录只有一条，0表示没有，1表示有。
	#define SERVICE_SERVICE_OFF_CORRDIGN                21053
	*/
	int CorrDign(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定历史时刻、机组示功图测点的波形。
	  #define     SERVICE_SERVICE_OFF_GETPVDATA                21035
	  */
	int GetPVData( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**得到指定历史时刻、机组活塞杆沉降量测点的波形和频谱。
	#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036
	*/
	int GetRodSinkData( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**读取指定动态压力测点、活塞杆沉降量测点、振动测点各一个。读取指定时间波形和特征参数
	  *如果测点为空就不会返回对应的数据
	  #define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA         21037
	  */
	int GetRcMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**历史概貌图，得到指定时间、指定机组所有测点数据
	#define     SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE    21038
	*/
	int GetSpecTimeAllChannValue( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**得到指定公司、分厂、机组、测点,指定启停车事件和时间的一组提纯轴心轨迹的波形数据。
	#define     SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK           21039
	*/
	int GetSudPurifyAxesTrack( ICommInterFace *pFixComm, IDBInterFace *pDB );


	/**往复式系统报警多参数分析功能，读取其指定报警时间的波形和特征参数，读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。
	*#define   SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA       21041
	*/
	int GetSpecTimeAlarmMultiParamData( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**往复式系统报警应力监测分析功能，得到指定机组指定报警时间、指定缸号的应力波形和振动测点波形
	*#define     SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE       21040
	*/
    int GetSpecTimeAlarmDynStressWave( ICommInterFace *pFixComm, IDBInterFace *pDB );

	/**集团级历史报警统计
	#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT               21054
	*/
	int GetGroupAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**公司级历史报警统计
	#define    SERVICE_SERVICE_OFF_ COMPANYALARMSTAT             21055
	*/
	int GetCompanyAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**监测站级历史报警统计
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTAT             21056
	*/
	int GetMonitorAlarmStat(ICommInterFace *pFixComm,IDBInterFace *pDB);

		/**监测站级历史报警统计
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTATEX             21088
	*/
	int GetMonitorAlarmStatEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**单通道多特征值报警查询，得到指定报警测点的各种类型特征值
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES        21057
	*/
	int GetSpecChannAlarmMultCharValues(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE        21058
	*/
	int GetSpecChannsAlarmSingleCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹
    #define     SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION              21059
	*/
    int GetSpecChannRodSinkAxesLocation(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到往复式机械设备运行状态历史趋势
    #define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060
	*/
    int GetSpecChannsRunStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);

		/**得到往复式机械设备运行状态历史趋势
    #define     SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX                  21082
	*/
    int GetSpecChannsRunStatusEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到往复式机械设备所有测点的特征值历史趋势比较；
    #define     SERVICE_SERVICE_OFF_GETHISCOMPARE         21061
	*/
    int GetSpecChannsHisCompare(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到往复式机械设备所有通道指定时间的特征值和波形数据比较
	#define     SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE         21062
	*/
    int GetSpecChannsCharValueWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到往复式机械设备振动通道和动态通道指定时间的特征值
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063
	*/
	int GetSpecChannsCharValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到往复式机械设备机组月报表；
	#define     SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT       21064
	*/
	int GetSpecRecipMonthReport(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**监测站级历史报警统计，优化读取数据；
	#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065
	*/
	int GetMonitorAlarmStatChar(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定机组、测点和时间的一组提纯轴心轨迹的波形数据。
	#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK          21066
	*/
	int GetPurifyAxesTrack(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到指定公司、分厂机组和测点指定时刻的倒谱数据
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM         21067
	*/
	int GetSpecChannCepstrum(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**往复机械报警查询，得到指定报警ID最先报警的测点名、特征值趋势
	#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068
	*/
	int GetSpecAlarmIDCharValues(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**往复机械报警查询,得到指定时间的指定活塞杆沉降或动态压力测点的报警记录
	#define     SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD     21069
	*/
	int GetSpecAlarmTimeDynRecord(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到指定公司、分厂机组和一对振动测点指定时刻的全谱数据
	#define     SERVICE_SERVICE_OFF_GETFULLSPECSTRUM          21070
	*/
    int GetSpecTimeFullSpectrum(ICommInterFace *pFixComm, IDBInterFace *pDB);

	/**报警查询功能，查询报警ID报警类型
	#define     SERVICE_SERVICE_OFF_GETALARMLOGINFO          21071
	*/
	int GetAlarmLogInfo(ICommInterFace *pFixComm, IDBInterFace *pDB);

	/**往复式系统应力监测，得到指定机组指定时间、指定缸号的应力波形和振动测点波形
	#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE          21072
	*/
	int GetSpecTimeDynStressWave(ICommInterFace *pFixComm, IDBInterFace *pDB);

	/**得到指定公司、分厂、机组、时间段、任意多个测点的自定义参数，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
	#define     SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND         21073
	*/
	int GetSpecChannsCustomTrend(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 获取离线巡检数据：
	#define     SERVICE_SERVICE_OFF_GET_OFFLINE_DATA         21074
	*/
	int GetOfflineData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**监测站级根据报警ID列表得到报警统计数据
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTAT             21075	*/
	int GetAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**监测站级根据起始时间得到监测站的报警ID列表
    #define     SERVICE_SERVICE_OFF_MONITORALARMSTAT             21076	*/
	int GetAlarmIDS(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**监测站级根据起始时间得到监测站的报警ID列表
	#define     SERVICE_SERVICE_OFF_MONITORALARMSTATEX             21080	*/
	int GetAlarmIDSEx(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/************************************************************************/
	/* 设置动平衡数据                                                       */
	/************************************************************************/
	int SetBalanceData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/************************************************************************/
	/* 得到动平衡数据                                                       */
	/************************************************************************/
	int GetBalanceData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/************************************************************************/
	/* 得到动平衡sql                                                        */
	/************************************************************************/
	CString  GetBalanceSql(	int sqlType,
							int anBalanceType,
							CString lstrSuffix,
							CString astrChannNo1,
							CString astrChannNo2,
							CString astrDateTimeStart,
							CString astrDateTimeEnd,
							int anBufferLen1 = 0,
							int anBufferLen2 = 0);



	enum SQL_TYPE
	{
		SELECT = 0,
		INSERT = 1,
		UPDATE = 2,
		DELETEX = 3,
		SELECT_FOR_UPDATE = 4,
		SEARCH_SUD_NAME = 5,
	};

	CString GetSqlForSud(   int sqlType,
							ICommInterFace * pFixComm,
							CString & arefStringSuffix);

	/*!
	*输入参数字段码
	*          公司ID      FIELD_SYSTEM_COMPANY
	*	     分厂名      FIELD_SYSTEM_FACTORY
	*          机组ID：    FIELD_SERVICE_PLANTID
	*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*          启停车事件ID: FIELD_SYSTEM_EVENT_ID
	*          启停车类型：  FIELD_SYSTEM_TYPE
	*          试车事件名    FIELD_SYSTEM_EVENT_NAME
	*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
	*         #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX    21077 
	*/

	//指定机组和时间范围的启停车id,以及名称
	int GetSpecTimeRangeSudAndName(ICommInterFace * pFixComm, IDBInterFace *pDB);


/*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          查询试车记录字符串  FIELD_SYSTEM_QUERY_EVENTNAME
  *返回值的字段码：
  *          启停车事件ID: FIELD_SYSTEM_EVENT_ID
  *          启停车类型：  FIELD_SYSTEM_TYPE
  *          试车事件名    FIELD_SYSTEM_EVENT_NAME
  *          起始时间：    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：    FIELD_SERVICE_OFF_TIMEEND
  #define    SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME    21078
  */
 
	int CTransactData::GetSpecTimeRangeSudSearchByName( ICommInterFace * pFixComm, 
														IDBInterFace *pDB );



/*!
  *输入参数字段码
  *          公司ID          FIELD_SYSTEM_COMPANY
  *	     分厂名          FIELD_SYSTEM_FACTORY
  *          机组ID：        FIELD_SERVICE_PLANTID
  *          启停车事件ID    FIELD_SYSTEM_EVENT_ID
  *          新启停车事件名  FIELD_SYSTEM_EVENT_NAME
  *返回值的字段码：
  *          是否修改成功：  FIELD_SYSTEM_STATUS
  *  #define    SERVICE_SERVICE_OFF_RENAMEEVENT 21079
  */
	int CTransactData::UpdateRenameEvent( ICommInterFace * pFixComm, 
										  IDBInterFace *pDB );

  /*!
  *输入参数字段码
  *          公司ID      FIELD_SYSTEM_COMPANY
  *	         分厂名      FIELD_SYSTEM_FACTORY
  *          机组ID：    FIELD_SERVICE_PLANTID
  *          起始时间：  FIELD_SERVICE_OFF_TIMESTART
  *          终止时间：  FIELD_SERVICE_OFF_TIMEEND
  *          测点类型    FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *		     是否修改成功：  FIELD_SYSTEM_STATUS
  *#define    SERVICE_SERVICE_OFF_SAVESPECTIMEDATA    21083
  */
	int CTransactData::SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);

 /*!
 *输入参数字段码
 *          公司ID          FIELD_SYSTEM_COMPANY
 *	         分厂名        FIELD_SYSTEM_FACTORY
 *          机组ID：        FIELD_SERVICE_PLANTID
 *          标注时刻：      FIELD_SERVICE_OFF_TIME
 *          测点：          FIELD_SERVICE_CHANN1
 *          事件开始时间：  FIELD_SERVICE_OFF_TIMESTART
 *          事件结束时间：  FIELD_SERVICE_OFF_TIMEEND
 *          标注描述：      FIELD_SERVICE_REMARK    
 *返回值的字段码：
 *		   是否保存成功：  FIELD_SYSTEM_STATUS
 *#define    SERVICE_SERVICE_OFF_SAVESPECMARK    21084
 */
   int SaveSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /*!
   *输入参数字段码
   *          公司ID      FIELD_SYSTEM_COMPANY
   *	       分厂名    FIELD_SYSTEM_FACTORY
   *          机组ID：    FIELD_SERVICE_PLANTID
   *          开始时间：  FIELD_SERVICE_OFF_TIMESTART
   *          结束时间：  FIELD_SERVICE_OFF_TIMEEND
   *          标注描述：  FIELD_SERVICE_REMARK    
   *返回值的字段码：
   *          测点：      FIELD_SERVICE_CHANN1
   *		   标注描述：  FIELD_SERVICE_REMARK 
   *          标注时间：  FIELD_SERVICE_OFF_TIME
   *#define    SERVICE_SERVICE_OFF_GETSPECMARK    21085
   */
   int GetSpecMarkInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

   int GetNormalWave( ICommInterFace *pFixComm,IDBInterFace *pDB );

	/**获取错误信号波形
    #define     SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE       21095
	*/
	int GetErrorSignalWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	///**获取错误信号波形
     //#define     SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM       21096
	//*/
	int SetOffAxesLocationParam(ICommInterFace *pFixComm,IDBInterFace *pDB);
private:
    int GetSudTrendCount();

	int GetSudJilianWaveCount();

	int GetSudSpecEventMaxMinRev(ICommInterFace * pFixComm, IDBInterFace *pDB, map<int,P_MAXMINREV> &arRevInfo);

	int GetSpecChannSaveTimeSudByTimeRange(ICommInterFace *pFixComm, IDBInterFace *pDB);

	vector<CBHDateTime> GetSpecTimeByTimeRange(
		IDBInterFace *pDB,
		string sSuffix_,
		CBHDateTime timeStart_,
		CBHDateTime timeEnd_,
		CString&  strTblName_,
		E_TBL_CHANNTYPE iTblType_,
		bool   bAlarmTable_);

	
};

