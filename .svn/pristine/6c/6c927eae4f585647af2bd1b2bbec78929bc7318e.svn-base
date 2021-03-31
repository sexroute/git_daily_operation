#pragma once

#include <additions/BHOleDbWrapper.h>
#include <additions/IBusiObjectInterFace.h>
#include <additions/IReadWriteLock.h>
#include <additions/CTimeOutReadWriteLock.h>
#include "BHKD_utilities.h"
#include "Sigdefine.h"


#include <vector>
using std::vector;

using namespace BHKD;

typedef struct S_CHANN_STATUS 
{
	int _nStatus_Online;
	int _nStatus_Normal;
	int _nStatus_DataUpdate;
	int _nStatus_BatteryPower;
	S_CHANN_STATUS()
	{
		_nStatus_Online = -1;
		_nStatus_Normal = -1;
		_nStatus_DataUpdate = -1;
		_nStatus_BatteryPower = -1;
	}
};
///通道信息基类
class CChannInfoBase
{
	
public:
      CChannInfoBase();
	  virtual ~CChannInfoBase();
	  S_ChannInfo GetChannInfo();
	  virtual void SetChannInfo(const S_ChannInfo &channInfo_);
	  virtual int SetCharInfo(CCustomDB *pDB_);
	  virtual int SetCharInfo(IDBInterFace *pDB_);
	  const string GetRevField() const;
	  void SetRwLock(IReadWriteLock * pLock_);

	  ///得到指定的自定义参数信息
      CCustomParamInfo GetSpecIndexCustomParamInfo(const int& index_);

	  int GetCustomParamSize();
	  /**设置自定义参数信息
	    *@param iStartIndex_ 设置的自定义参数的索引
	    *@param ilen_     设置的个数
	    *@param pCustomParamInfo_ 的长度必须为ilen_
	     */
	  bool SetCustomParamInfo(const int& iStartIndex_, const int& ilen_, CCustomParamInfo * pCustomParamInfo_);

	  int GetCustomAlarmParamSize();
	  /**设置自定义报警参数信息
	  *@param iStartIndex_ 设置的自定义报警参数的索引
	  *@param ilen_     设置的个数
	  *@param pCustomAlarmParamInfo_ 的长度必须为ilen_
	  */
	  bool SetCustomAlarmParamInfo(const int& iStartIndex_, const int& ilen_, CCustomAlarmParamInfo * pCustomAlarmParamInfo_);

	  ///得到指定的自定义参数信息
	  CCustomAlarmParamInfo GetSpecIndexCustomAlarmParamInfo(const int& index_);

	  //设置测点位置信息
	  void SetChannLocationsInfo(const CChannLocations &channLocations_);
	  //得到测点位置信息
	  CChannLocations GetChannLocationsInfo();

	  //设置测点部位信息
	  void SetChannSectionsInfo(const CChannSections &channSections_);
	  //得到测点部位信息
	  CChannSections GetChannSectionsInfo();

	  //设置轴承参数信息
	  void SetBearingParamInfo(const CBearingParamInfo &bearingParams_);
	  //得到轴承参数信息
	  CBearingParamInfo GetBearingParamInfo();

	  //设置齿轮箱参数信息
	  void SetGearBoxParamInfo(const CGearBoxParamInfo &gearBoxParams_);
	  //得到齿轮箱参数信息
	  CGearBoxParamInfo GetGearBoxParamInfo();


	  //报警确认相关接口开始
	  virtual int ConfirmAlarm(GE_ALARM_CONFIRM_TYPE  anType,CBHDateTime &arefStartTime,CBHDateTime& arefEndTime);

	  virtual BOOL IsAlarmConfirmed();
	  //报警相关接口结束

	  static E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);

	  virtual GE_ALARM_CONFIRM_TYPE  AlarmConfirmType() const;
	  virtual void AlarmConfirmType(GE_ALARM_CONFIRM_TYPE  val);

	  virtual CBHDateTime AlarmConfirmStartTime() const;
	  virtual void AlarmConfirmStartTime(CBHDateTime val);

	  virtual CBHDateTime AlarmConfirmEndTime() const;
	  virtual void AlarmConfirmEndTime(CBHDateTime val);

	  virtual bool JudgeRegResetPlantAlarmTimer(float fCurrValue_,float fValueLast_);

	  virtual void WaveHead(const CWaveHead& afWaveHead_);
	  virtual CWaveHead WaveHead();


public:
      S_ChannInfo _channInfo;
	  string      _sRevField;
	  IReadWriteLock * _pLock;
	  vector<CCustomParamInfo>        _vCustomParamInfo;     ///自定义参数设置信息
	  vector<CCustomAlarmParamInfo>   _vCustomAlarmParamInfo;///自定义报警参数信息
	  CChannLocations   _channLocations;   ///通道位置参数
	  CChannSections    _channSections;    ///通道部位参数
	  CBearingParamInfo _bearingParams;    ///轴承参数信息
	  CGearBoxParamInfo _gearBoxParams;    ///齿轮箱参数信息
	  CBHDateTime m_oUpdateTime;
	  bool        m_bChannUpdate;


	  GE_ALARM_CONFIRM_TYPE  m_nAlarmConfirmType;
	  CBHDateTime m_oAlarmConfirmStartTime;
	  CBHDateTime m_oAlarmConfirmEndTime;

	  CWaveHead m_sWaveHead;




};
///轴心位置信息基类
class CAxisChannInfoBase
{
public:
	CAxisChannInfoBase();
	virtual ~CAxisChannInfoBase();
	void GetChannInfo(CAxisChannInfo &axisChannInfo_);
	CAxisChannInfo GetChannInfo();
	virtual void SetChannInfo(const CAxisChannInfo &channInfo_);
	void SetRwLock(IReadWriteLock * pLock_);


	GE_ALARM_CONFIRM_TYPE  m_nAlarmConfirmType;
	GE_ALARM_CONFIRM_TYPE AlarmConfirmType() const;
	void AlarmConfirmType(GE_ALARM_CONFIRM_TYPE val);


	CBHDateTime m_oAlarmConfirmStartTime;
	CBHDateTime AlarmConfirmStartTime() const;
	void AlarmConfirmStartTime(CBHDateTime val);


	CBHDateTime m_oAlarmConfirmEndTime;
	CBHDateTime AlarmConfirmEndTime() const;
	void AlarmConfirmEndTime(CBHDateTime val);



	//报警确认相关接口开始
	virtual int ConfirmAlarm(GE_ALARM_CONFIRM_TYPE  anType,CBHDateTime &arefStartTime,CBHDateTime& arefEndTime);

	virtual BOOL IsAlarmConfirmed();



protected:
	CAxisChannInfo _channInfo;
	IReadWriteLock * _pLock;
};

class CAxisChannInfoBase;

typedef vector<CChannInfoBase*> V_PCHANNINFO;
typedef vector<CAxisChannInfoBase*> V_PAXISCHANNINFO;
typedef map<string,int> M_CHANNINDEX;

class CPlantInfoBase
{

	
public:
	CPlantInfoBase();
	virtual ~CPlantInfoBase();
	//得到中间件支持的采样点数上限
	virtual int GetMiddleWareWavePointNumMax();

	//得到中间件支持的安全范围的采样点数
	virtual int GetSafeWavePointNum(int anRealSampleWaveNum = -1);




public:
	//删除一些分配的数据
	virtual void Destory();
	///得到指定类型通道向量表
	V_PCHANNINFO GetSpecTypeChannVector(const int &iType_=GE_VIBCHANN);
    ///得到指定类型通道映射表
	M_CHANNINDEX GetSpecTypeChannMap(const int &iType_=GE_VIBCHANN);

	//得到指定轴心位置的索引
	int  GetSpecAxisLocationIndex(string sLocation_);
	//得到指定垂直测点ID和水平方向测点ID的轴心位置索引
	int  GetSpecAxisLocationIndex(string sChannHor_,string sChannVer_);
	//得到指定动态垂直测点ID和水平方向测点ID的轴心位置索引
	int  GetSpecDynAxisLocationIndex(string sChannHor_,string sChannVer_);
	// 振动动态测点公共字段
	virtual CString ConsCommonField();
	
	///得到指定通道ID的索引
	int GetSpecVibChannIndex(string sChannID_);
	int GetSpecDynChannIndex(string sChannID_);
	int GetSpecProcChannIndex(string sChannID_);
	///得到指定通道NO的索引
	int GetSpecVibChannIndexNo(const string &sChannNo_);
	int GetSpecDynChannIndexNo(const string &sChannNo_);
	int GetSpecProcChannIndexNo(const string &sChannNo_);
	


	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);


protected:
	//为了定位测点速度快,建立测点名和对应测点在测点向量中的位置
	M_CHANNINDEX _mapVibChannIndex;
	M_CHANNINDEX _mapDynChannIndex;
	M_CHANNINDEX _mapProcChannIndex;

	//和设备有关的参数
	string _sGroup;
	string _sCompany;
	string _sCompanyAlias;
	string _sFactory;
	string _sSetID;
	S_PlantInfo _sPlantInfo;

	///各种相关表名
	///启停车日志表
	string _sSudLogTblName;
    //启停车数据表名
	string _sSudDataTblName;
	///动态通道数据报警表名
	string _sSudDynDataTblName;
	///过程量数据报警表名
	string _sSudProcDataTblName;

   ///各种趋势数据表名
	///振动趋势数据表名
	string _sVibTrendSecTblName;
	string _sVibTrendHourTblName;
	string _sVibTrendDayTblName;
	string _sVibTrendMonthTblName;
	///动态趋势数据表名
	string _sDynTrendSecTblName;
	string _sDynTrendHourTblName;
	string _sDynTrendDayTblName;
	string _sDynTrendMonthTblName;
	///过程量趋势数据表名
	string _sProcTrendSecTblName;
	string _sProcTrendHourTblName;
	string _sProcTrendDayTblName;
	string _sProcTrendMonthTblName;
	///振动快变趋势数据表名
	string _sVibTrendFastTblName;
	string _sDynTrendFastTblName;
	string _sProcTrendFastTblName;

	///阈值信息表名
	string _sThTblName;
	 	///报警日志表名
	string _sAlarmLogTblName;
	//振动报警数据表名
	string _sAlarmVibDataTblName;
	///动态通道数据报警表名
    string _sAlarmDynDataTblName;
	///过程量数据报警表名
    string _sAlarmProcDataTblName;
	///趋势报警数据表名
    string _sAlarmTrendDataTblName;
	///轴心位置报警表名
	string _sAlarmAxisDataTblName;
	//轴心轨迹对
    int _iAxesPairNum;
	//径向通道数目
	int _iRadialVibChannNum;
	//轴向通道数目
	int _iAxialVibChannNum;
	//可以做启停车的通道数目
	int _iSudChannNum;

	IReadWriteLock * _pLockChannInfo;

public:

	V_PCHANNINFO _vProcChann;
	V_PCHANNINFO _vVibChann;
	V_PCHANNINFO _vDynChann;
	V_PCHANNINFO _vSudChann;//可以做启停车的通道
	V_PAXISCHANNINFO _vAxisChann;
	V_PCHANNINFO     _vOfflineChann; // 离线巡检测点
	//当前时间
	CBHDateTime _timeCurr;
    CBHDateTime _timeCurrVib;
	//当前动态数据时间
	CBHDateTime _timeCurrDyn;
	//当前过程量数据时间
	CBHDateTime _timeCurrProc;
	//数据库连接指针
	//CCustomDB *_pDB;
	
public:
	const int GetVibChannNum() const;
	const int GetProcChannNum() const;
	const int GetDynChannNum() const;
	const int GetAxialVibChannNum() const;
	const int GetRadialVibChannNum() const;
	const int GetAxisPairNum() const;
	const string GetRevField(const int &iRevType_ ) const;
	const S_PlantInfo& GetPlantInfo() const;
	///得到指定振动测点的详细信息
	virtual S_ChannInfo  GetSpecVibChannInfo(const string &sChannID_);
	virtual S_ChannInfo  GetSpecVibChannNoInfo(const string &sChannNo_);
	virtual S_ChannInfo  GetSpecVibChannInfo(const int &index_);

	///得到指定动态信号测点的详细信息
	virtual S_ChannInfo  GetSpecDynChannInfo(const string &sChannID_);
	virtual S_ChannInfo  GetSpecDynChannNoInfo(const string &sChannNo_);
	virtual S_ChannInfo  GetSpecDynChannInfo(const int &index_);

	/**更新指定通道的通道信息。
	  *@return -1表示没有找到指定通道，-2表示更新失败，成功则返回通道的索引
	  */
	virtual int UpdateSpecChannInfo(string sChannID_,S_ChannInfo channInfo_);

	///得到指定过程量测点的详细信息
	virtual S_ChannInfo  GetSpecProcChannInfo(const string &sChannID_);
	virtual S_ChannInfo  GetSpecProcChannNoInfo(const string &sChannNo_);
	virtual S_ChannInfo  GetSpecProcChannInfo(const int &index_);

	/**@brief 通过指定的轴心位置描述得到轴心位置的详细信息
	  *@param sLocation_ 轴心位置描述
	  *@return 轴心位置详细信息
	  */
	virtual CAxisChannInfo GetSpecLocationAxisChannInfo(string sLocation_);
	/**@brief 通过指定动态轴心测点对得到轴心位置的详细信息
	*@param sChannHor_ 水平测点
	*@param sChannVer_ 动态测点
	*@return 轴心位置详细信息
	*/
    virtual CAxisChannInfo GetSpecDynChannAxisChannInfo(string sChannHor_,string sChannVer_);

	virtual void AddVibChannInfo(CCustomDB *pDB_);
	virtual void AddDynChannInfo(CCustomDB *pDB_);
	virtual void AddProcChannInfo(CCustomDB *pDB_);
	void AddOfflineChannInfo(CCustomDB *pDB_);
	virtual void SetCharInfo(CCustomDB *pDB_);

	virtual void AddVibChannInfo(IDBInterFace *pDB_);
	virtual void AddDynChannInfo(IDBInterFace *pDB_);
	virtual void AddProcChannInfo(IDBInterFace *pDB_);
	void AddOfflineChannInfo(IDBInterFace *pDB_);
	virtual void SetCharInfo(IDBInterFace *pDB_);



	virtual void AddChannInfoEnd();
	virtual void SetPlantInfo(string sGroup_,string sCompany_,string sCompanyAlias_,string sFactory_,string sSet_,const S_PlantInfo &sPlantInfo_);
	
	void SetDBConn(CCustomDB *pDB_);
	void SetDBConn(IDBInterFace *pDB_);

	///检查是否是当前设备
	bool CheckPlant(string sCompany_,string sFactory_,string sPlant_);
	//判断是否有动态周期索引，即是否为整周期采样
	bool IsNotDynPeriodIndex(const int iStartIndex_,const int iEndIndex_);

	const string GetPlantID() const;
	const string GetPlantNo() const;
	const string GetSetID() const;
	const string GetFactoryName() const;
	const string GetCompanyName() const;
	const string GetCompanyAlias() const;
	const string GetGroupName() const;
	const string GetAlarmLogTblName() const;
	const string GetSudLogTblName() const;
	const string GetAlarmAxisTblName() const;
	const string GetThTblName() const;

	///通过采样频率和点数，示功图类型，往复机械参数。得到示功图的X坐标数据
	int GetPVXCoorData(const int &iSmpFreq_, const int &iSmpNums_, const int &iPvType_, const float &fRev_, const int &iCylinderNo_, DATATYPE_WAVE *pXCoor_);
	int GetPVXCoorData(const int &iSmpFreq_, const int &iPeriodSmpNums_, const int &iPvType_, const int &iCylinderNo_, DATATYPE_WAVE *pXCoor_);

	///通过动态压力波型，往复机械参数。得到动态压力波形和惯性力波形
	int GetStrssAndInertiaWave(DATATYPE_WAVE *pWave1_, DATATYPE_WAVE *pWave2_,const int &iSmpFreq_,const int &iCylinderNo_,const int &iPeriodSmpNums_,float *pfStressWave_,float *pfInertiaWave_);

	//float数据与波形数据相互转化;
	 bool ConvertFloatToDTWave(BHKD::DATATYPE_WAVE *pWave_,const float *pfWave_,int iLen_,CWaveHead& sWaveHead_,float fCoef_=1);
	 bool ConvertDTWaveToFloat(float *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_);
	 //double数据与波形数据相互转化;
	 bool ConvertDoubleToDTWave(BHKD::DATATYPE_WAVE *pWave_,const double *pfWave_,int iLen_,CWaveHead& sWaveHead_,float fCoef_=1);
	 bool ConvertDTWaveToDouble(double *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_);

	 //还原波形数据
	 bool ParseDTWaveToDT(DATATYPE_WAVE *pRealWave_,const DATATYPE_WAVE *pWave_,int iLen_,CWaveHead& sWaveHead_);

	 //float数据与double数据相互转化;
	 bool ConvertFloatToDouble(double *pWave_,const float *pfWave_,int iLen_);
	 bool ConvertDoubleToFloat(float *pfWave_,const double *pWave_,int iLen_);

	 bool ConvertDTToFloat(float *pfWave_,const DATATYPE_WAVE *pWave_,int iLen_);
	 bool ConvertFloatToDT(DATATYPE_WAVE *pWave_,const float *pfWave_,int iLen_);

	 bool ScaleWave(float *pWave_,const float *pfWave_,int iLen_,float fCoef_=1);

	 bool ScaleWave(double *pWave_,const double *pfWave_,int iLen_,float fCoef_=1);

	 bool GetScaleCoefAndPrecisionFlag(CWaveHead& sWaveHead_,float& fScaleCoef_,int& nPrecisionEnabled_,float& fMaxValue_,float& fMinValue_);

	/**频谱分析和细化分析，如果实际的分析线数满足要求，就直接进行分析，如果不满足就进行细化分析
	*@param pfWave_  波形数据指针，长度为iSmpNums_.
	*@param iSmpNums_  波形数据点数.
	*@param iSmpFreq_  波形采样频率.
	*@param pfSpec_    谱数据输出，内存是在外部分配，长度必须为iSmpNums_/2.
	*@param iLines_    分析线数，返回的为分析后的分析线数
	*@param fDf_       谱的分辨率
	*@param fStartFreq_ 分析后的起始频率
	*@return 正确就为0或者2，错误就小于0；2表示采用频谱分析,没有细化
	*/
	int ZoomSpectrumAnalysis(float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_);
	int ZoomSpectrumAnalysis(float *pfWave_,int iSmpNums_,int iSmpFreq_,float *pfSpec_, int &iLines_,float &fDf_,float &fStartFreq_, float fStopFreq_);

	int SpectrumAnalysis( DATATYPE_WAVE *pWave_, int &iSmpNums_, DATATYPE_WAVE *pSpectrum_, const int &iSensorType_,CWaveHead& sWaveHead_);
	int SpectrumAnalysis( float *pfWave_, int &iSmpNums_, float *pfSpectrum_, const int &iSensorType_);

	int CepstrumAnalysis( DATATYPE_WAVE *pWave_, int &iSmpNums_, double *pSpectrum_,int &iCepsNums_,CWaveHead& sWaveHead_);

	/**@brief 保存阈值数据。阈值结构体和学习参数指针长度为6，即6个特征值的
	*@param iThID_ 阈值ID
	*@param sName_ 通道名或者轴心位置名
	*@param iChannType_ 通道类型
	*@param iType_ 自学习的触发类型
	*/
	int SaveThresholdInfo(IDBInterFace *pDB_,int iThID_,const int &iChannIndex_,CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,int iType_,char *pfLearnData_,int iLearnDataLen_,CThreshold *pTh_,CThresholdLearnParam *pThLearnPar_,int iAlarmType_,int iCharNums_=6);
	int SaveThresholdInfo(IDBInterFace *pDB_,int iThID_,const string &sName_,CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,int iType_,char *pfLearnData_,int iLearnDataLen_,CThreshold *pTh_,CThresholdLearnParam *pThLearnPar_,int iAlarmType_,int iCharNums_=6);//CThreshold ,CThresholdLearnParam

	//得到上一次保存的快变报警阈值ID
	int GetFAlarmLastThID();

	//得到指定一对测点对应转轴的旋转方向:sChannHor_和sChannVer_为测点ID,返回值为旋转方向
	int GetSpecChannsAxisRotation(string sChannHor_,string sChannVer_);

	//对波形数据去掉直流量DC
	int SubtractWaveDC(DATATYPE_WAVE *pfWave_,DATATYPE_WAVE *pfWaveOut_,int iLen_);
	//对波形数据去掉直流量DC
	int SubtractWaveDC(float *pfWave_,float *pfWaveOut_,int iLen_);

	//根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的前两个动态压力测点，如果小于两个则返回 -1
	int GetDynPressChann(CCustomDB *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_);

	//根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的动态应力测点，如果小于两个则返回 -1
	int GetDynStressChann(CCustomDB *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_);


	//根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的前两个动态压力测点，如果小于两个则返回 -1
	int GetDynPressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_);

	//根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的动态应力测点，如果小于两个则返回 -1
	int GetDynStressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_);


	/**时间转化为时间字符串(加引号的)
	*@param time_ 时间
	*@return 返回带引号字符串
	*/
	static CString TimeConvertStringQuote(const CBHDateTime &time_);

	/**时间转化为时间字符串(加引号的)
	*@param time_ 时间
	*@return 返回字符串
	*/
	static CString TimeConvertString(const CBHDateTime &time_);

	//删除振动趋势数据
	BOOL DeleteVibTrendRecord(IDBInterFace *pDB_,int nTotalWavePtNum_ = 0);

	//删除动态趋势数据
	BOOL DeleteDynTrendRecord(IDBInterFace *pDB_,int nTotalWavePtNum_ = 0);

	//删除过程量趋势数据
	BOOL DeleteProcTrendRecord(IDBInterFace *pDB_);

	//删除振动快变数据
	BOOL DeleteVibTrendFastRecord(IDBInterFace *pDB_);

	//删除动态快变数据
	BOOL DeleteDynTrendFastRecord(IDBInterFace *pDB_);

	//删除过程量快变数据
	BOOL DeleteProcTrendFastRecord(IDBInterFace *pDB_);

	//删除振动报警数据
	BOOL DeleteVibAlarmRecord(IDBInterFace *pDB_,int iEventID_,int nTotalWavePtNum_);

	//删除动态报警数据
	BOOL DeleteDynAlarmRecord(IDBInterFace *pDB_,int iEventID_,int nTotalWavePtNum_ );

	//删除过程量报警数据
	BOOL DeleteProcAlarmRecord(IDBInterFace *pDB_,int iEventID_);

	//删除开停车振动数据
	BOOL DeleteSudVibRecord( IDBInterFace *pDB_,int iEventID_,const vector<CString>& vChannNo_,int nTotalWavePtNum_);

	BOOL DeleteSudDynRecord( IDBInterFace *pDB_,int iEventID_ );

	BOOL DeleteSudProcRecord( IDBInterFace *pDB_,int iEventID_ );

	//删除数据确保足够的存储空间
	int  MakeSureVibSaveSpaceSufficient(IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iCurrID_,int iStartIndex_);

	//删除数据确保足够的存储空间
	int  MakeSureDynSaveSpaceSufficient(IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iCurrID_,int iStartIndex_);

	BOOL DeleteSpecCountTrendRecord(IDBInterFace *pDB_,string sTableName_,int iTotalWavePtNum_,int iTotalRealWavePtNum_,int iEventID_ = -1);

	static int SpectralMeasureMenEx( double *pwave,
		int & anRef,
		double *pout, //长度必须同输入的一致
		E_SpectrumType type,
		E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
		unsigned int iPrecision_ = 2,
		int anCUTOFF_TYPE=1, //0:not cutoff 1:cutoff to 2*n 3:cutoff to pow(2,n)
		double adblScaleBeforeCalc =100.0);
	///必须把vOut_长度为n/2。
	static int SpectralMeasureMenEx( double *pwave,
		int & anRef,
		vector<double>& vOut_,//length should same as pwave
		E_SpectrumType type,
		E_WINOW_TYPE iWinType_ = WINDOW_HANNING, 
		unsigned int iPrecision_ = 2,
		int anCUTOFF_TYPE=1, //0:not cutoff 1:cutoff to 2*n 2:cutoff to pow(2,n) 3: append to pow(2,n) with original Data 4: append to pow(2,n) with zero
		double adblScaleBeforeCalc =100.0);

	public:
		virtual int WaveAddHead(const DATATYPE_WAVE* pWave_,int nWaveLen_, CWaveHead& WaveHead_,vector<BYTE>& vWave_ );
		virtual int  WaveAddHead(const DATATYPE_WAVE* pWave_,int nWaveLen_, CWaveHead& WaveHead_,BYTE* pOutWave_ );


public:


    virtual	CString Country();
	virtual void Country(CString val);
	virtual CString PlantDisplayName() ;
	virtual void PlantDisplayName(CString val);
	virtual CString CompanyDisplayName();
	virtual void CompanyDisplayName(CString val);
	virtual CString FactoryDisplayName();
	virtual void FactoryDisplayName(CString val);
	virtual CString SetDisplayName();
	virtual void SetDisplayName(CString val);
	virtual CString GroupDisplayName();
	virtual void GroupDisplayName(CString val);

protected:
	CString m_strCountry;
	CString m_strPlantDisplayName;
	CString m_strCompanyDisplayName;
	CString m_strFactoryDisplayName;
	CString m_strSetDisplayName;
	CString m_strGroupDisplayName;
	CTimeOutReadWriteLock m_oInfoLock;
};

class COfflineChannParam:public CChannInfoBase
{
public:
	COfflineChannParam();
	virtual ~COfflineChannParam();

	int SetData(CBHDateTime &tTime, float fVal);
	int GetData(CBHDateTime & tTime, float & fVal);

	CBHDateTime GetTime();
	float GetValue();

private:
	CBHDateTime _tTime;
	float        _fValue;
};
