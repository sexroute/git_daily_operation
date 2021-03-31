#pragma once
/**@brief
  *GD_ 代表全局宏定义
  *GE_ 代表全局枚举定义
  *@author Marble
  */

#ifndef _BHKD_G_UTILITIES_H_MARBLE_
#define _BHKD_G_UTILITIES_H_MARBLE_
#define  TIME_INIT_VALUE 1980,1,1,1,1,1
#define  DATE_TIME_FORMAT			"%Y-%m-%d %H:%M:%S"			// 时间标准格式
#define  COUNTRY_DEFAULT  _T("CHINA")

///全局的枚举定义和宏定义
#include "DSE_Define.h"
#include "KDCommlib.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/BHDateTime.h>

#include <map>
using std::map;

extern CString g_strMainDbName;
extern int g_iDbType;

BOOL IsFloatValid(float f);
CString GetModuleFilePath();



#define _HZLOG_TRACE(info)
#define _HZLOG_DEBUG(info)
#define _HZLOG_INFO(info)
#define _HZLOG_WARN(info)
#define _HZLOG_ERROR(info)
#define _HZLOG_FATAL(info)
#define _HZLOG_ARRAY(ArrayName, ArrayData, ArrayNum)


#define HZLOG_TRACE(info)
#define HZLOG_DEBUG(info)
#define HZLOG_INFO(info)
#define HZLOG_WARN(info)
#define HZLOG_ERROR(info)
#define HZLOG_FATAL(info)
#define HZLOG_ARRAY(ArrayName, ArrayData, ArrayNum)


#define HZLOG_ARRAY(ArrayName, ArrayData, ArrayNum) 

#define HZLOG_ID(id)

#define HZLOG_CONFIG(info) 


#define gc_cTimeFormat          "%Y-%m-%d %H:%M:%S"



namespace BHKD
{

#define DataIsMiddle(a,low,high)  ( ((a)>(low)) && ((a)<(high)) )

#define KD_DeletePoint(pt)          { if(NULL != (pt)) { delete [] (pt); (pt)=NULL;} }

#define KD_DeleteSinglePoint(pt)          { if(NULL != (pt)) { delete  (pt); (pt)=NULL;} }

//每个通道判断报警的特征值类型数目
#define D_BHKD_ALARM_CHARTYPE_NUM 10
	//报警类型字段宏定义
#define D_BHKD_AT_FAST            0x01000000       //快变
#define D_BHKD_AT_SLOW            0x02000000       //缓变
#define D_BHKD_AT_TREND           0x04000000       //趋势
#define D_BHKD_AT_REGULAR         0x08000000       //常规报警

#define D_BHKD_AT_LOCATION        0x00010000       //轴心位置报警
#define D_BHKD_AT_TEMP            0x00020000       //温度
#define D_BHKD_AT_PRESS           0x00040000       //压力
#define D_BHKD_AT_FLUX            0x00080000       //流量
#define D_BHKD_AT_OTHERPROC       0x00100000       //其它类型过程量
#define D_BHKD_AT_RADIALVIB       0x00200000       //径向振动
#define D_BHKD_AT_AXISVIB         0x00400000       //轴向振动
#define D_BHKD_AT_DYN             0x00800000       //动态信号通道。通道类型是11~20


#define D_BHKD_AT_PHASEONE        0x00000100       //1X相位
#define D_BHKD_AT_PHASETWO        0x00000200       //2X相位
#define D_BHKD_AT_PHASETHREE      0x00000400       //3X相位
#define D_BHKD_AT_PHASEHALF       0x00000800       //1/2X相位
#define D_BHKD_AT_TRENDHOUR       0x00001000       //小时趋势报警
#define D_BHKD_AT_TRENDDAY        0x00002000       //天趋势报警
#define D_BHKD_AT_TRENDMONTH      0x00004000       //月趋势报警
#define D_BHKD_AT_TRENDSEASON     0x00008000       //季度趋势报警

#define D_BHKD_AT_CHARONE         0x00000100       //1X特征值
#define D_BHKD_AT_CHARTWO         0x00000200       //2X特征值
#define D_BHKD_AT_CHARTHREE       0x00000400       //3X特征值
#define D_BHKD_AT_CHARHALF        0x00000800       //1/2X特征值
#define D_BHKD_AT_CHARRES         0x00001000       //残振特征值
#define D_BHKD_AT_CHARALL         0x00002000       //通频特征值
#define D_BHKD_AT_DC              0x00010000       //直流量
#define D_BHKD_AT_GAP             0x00020000       //间隙电压
#define D_BHKD_AT_AXISDISPLACE    0x00004000       //轴位移报警
#define D_BHKD_AT_REGHIGH         0x00008000       //常规报警值
#define D_BHKD_AT_REGLOW          0x00000000       //常规危险报警

#define D_BHKD_AT_TRENDSLOWASC    0x00000001       //趋势缓慢增长
#define D_BHKD_AT_TRENDFASTASC    0x00000002       //趋势较快增长
#define D_BHKD_AT_TRENDSLOWDSC    0x00000004       //趋势缓慢降低
#define D_BHKD_AT_TRENDFASTDSC    0x00000008       //趋势较快降低

#define D_BHKD_AT_CUSTOM          0x10000000       //自定义报警
#define D_BHKD_AT_CUSTOMLOW       0x00000001       //自定义低限报警
#define D_BHKD_AT_CUSTOMHIGH      0x00000002       //自定义高限报警

	//常用字段的长度
	const int D_BHKD_COMPANYNAME_LEN =   100;  ///公司名长度
	const int D_BHKD_FACTORYNAME_LEN =   100;  ///分厂名长度
	const int D_BHKD_SETID_LEN       =   20;   ///中间件服务器ID长度
	const int D_BHKD_PLANTID_LEN     =   50;   ///设备ID长度
	const int D_BHKD_PLANTNO_LEN     =   10;   ///设备NO长度
	const int D_BHKD_CHANNID_LEN     =   50;   ///通道ID长度
	const int D_BHKD_CHANNNO_LEN     =   10;   ///通道NO长度
	const int D_BHKD_CHANNALIAS_LEN  =   100;  ///通道别名长度
	const int D_BHKD_IP_LEN          =   30;   ///IP长度
	const int D_BHKD_VERSION_LEN     =   20;   ///版本号长段
	const int D_BHKD_STATUS_LEN      =   16;   ///状态长段

	typedef float DATATYPE_CHAR;
	typedef short DATATYPE_WAVE;
	typedef DATATYPE_WAVE * PDATATYPE_WAVE;
	typedef unsigned int   DATATYPE_ALARMSTATE;
	typedef int DATATYPE_INT;

	//每个通道报警判断种类 10个 宏定义"BHKD_ALARMCHAR_NUM" 就是这个枚举的数目
	enum E_ALARM_CHARTYPE
	{
		E_TH_ALLCHAR=0,
		E_TH_ONECHAR,
		E_TH_TWOCHAR,
		E_TH_THREECHAR,
		E_TH_HALFCHAR,
		E_TH_RESCHAR,
		E_TH_ONEPHASE,
		E_TH_TWOPHASE,
		E_TH_THREEPHASE,
		E_TH_HALFPHASE
	};
	//报警判断方式：低通、高通、带通、带阻
	enum E_ALARMCHECK_TYPE
	{
		E_ALARMCHECK_LOWPASS=0,
		E_ALARMCHECK_HIGHPASS,
		E_ALARMCHECK_BANDPASS,
		E_ALARMCHECK_BANDSTOP
	};

	///触发门限学习的种类
    enum E_TRIG_TH_LEARN_TYPE
	{
		E_TRIG_TH_TIME=0,   ///定时触发
		E_TRIG_TH_ALARM,    ///报警触发
        E_TRIG_TH_RESTART,  ///系统重启触发
		E_TRIG_TH_SUD,       ///启停车触发
		E_TRIG_TH_PARAMCHANGED ///阈值学习参数改变
	};

	///自定义参数信息相关枚举
	//数据类型
	enum E_CUSTOMPARAM_DATATYPE
	{
		E_CUSTOMPARAM_TIMEDOMAIN = 1,// 时域
		E_CUSTOMPARAM_FREQDOMAIN,  // 频域
		E_CUSTOMPARAM_MAXDOMAIN    // MAX值
	};
    //单位类型
	enum E_CUSTOMPARAM_UNITTYPE
	{
		E_CUSTOMPARAM_TIME=1,//时间
        E_CUSTOMPARAM_PERIOD,//周期
        E_CUSTOMPARAM_POINT, //点数
		E_CUSTOMPARAM_FREQ,  //频率
		E_CUSTOMPARAM_ORDER  //阶次
	};

	//信号分析方式
	enum E_CUSTOMPARAM_SIGANATYPE
	{
		E_CUSTOMPARAM_TIMEWAVE=1,     //时域波形
		E_CUSTOMPARAM_PEAKSPEC,       //峰值谱
		E_CUSTOMPARAM_PEAKPEAKSPEC,   //峰峰值谱
		E_CUSTOMPARAM_POWERSPEC,      //功率谱
		E_CUSTOMPARAM_WRAPPERSPEC,    //包络谱
		E_CUSTOMPARAM_POWERSPEC_INTEGRAL,      //功率谱积分
		E_CUSTOMPARAM_ENVELOPEWAVE    //包络波形
	};

	//特征值类型
	enum E_CUSTOMPARAM_CHARTYPE
	{
		E_CUSTOMPARAM_PEAKVALUE=1,   //峰值
		E_CUSTOMPARAM_PEAKPEAKVALUE, //峰峰值
		E_CUSTOMPARAM_RMSVALUE,      //有效值
		E_CUSTOMPARAM_ENERGYVALUE,    //能量值
		E_CUSTOMPARAM_MAXVALUE,       //最大值
		E_CUSTOMPARAM_TRUE_PEAKVALUE, //真峰值
		E_CUSTOMPARAM_TRUE_PEAKPEAKVALUE  //真真峰值
	};

	// 预处理方式
	#define E_CUSTOMPARAM_PREPROC_NONE -1  // 无
	enum E_CUSTOMPARAM_PREPROCTYPE
	{
		E_CUSTOMPARAM_PREPROC_INT = 0, // 积分
		E_CUSTOMPARAM_PREPROC_DIFF,    // 微分
		E_CUSTOMPARAM_PREPROC_LINEAR,    // 线性
		E_CUSTOMPARAM_PREPROC_NUM

	};


	//门限学习参数
	class CThresholdLearnParam
	{
	public:
		CThresholdLearnParam();
		float _fParam1;      //期望系数
		float _fParam2;      //放大系数
		float _fParam3;
		float _fParam4;
		float _fParam5;
		CThresholdLearnParam& operator=(const CThresholdLearnParam& thLParam_);
	};
	//门限结果
	class CThreshold
	{
	public:
		float _fThreshold1;
		float _fThreshold2;
		float _fThreshold3;
		float _fThreshold4;
		float _fThreshold5;
		bool _bThreshhold1;
		CThreshold & operator=(const CThreshold& th_);
		CThreshold();
	};
	//趋势报警判断使用参数
	class CTrendCheckParam
	{
	public:
		CTrendCheckParam();
		float _fParam1;
		float _fParam2;
		float _fParam3;
		float _fParam4;
		float _fParam5;
		CTrendCheckParam& operator=(const CTrendCheckParam& tParam_);
	};

	//轴心位置判断报警的相关参数
	class CAxisAlarmParam
	{
	public:
		CAxisAlarmParam();
		~CAxisAlarmParam();
		//缓变阈值学习参数
		CThresholdLearnParam _thLearnParamS;
		//快变阈值学习参数
		CThresholdLearnParam _thLearnParamF;
		//趋势报警判断参数
		CTrendCheckParam _trendCheckParam;
		//是否检查缓变
		bool _bCheckS;
		//是否检查快变
		bool _bCheckF;
		//是否检查趋势报警
		bool _bCheckTrend;
		CAxisAlarmParam& operator=(const CAxisAlarmParam& aParam_);
	};

	//通道判断报警的相关参数
	class CChannAlarmParam
	{
	public:
		CChannAlarmParam();
		///是否判断各种类型报警的相关参数
		bool _bRegular;
		//是否判断特征值趋势报警
		bool _bCharTrend;
		//是否判断相位趋势报警
		bool _bPhaseTrend;
		//是否判断特征值缓变报警
		bool _bCharSlow;
		//是否判断特征值快变报警
		bool _bCharFast;
		//是否判断相位缓变报警
		bool _bPhaseSlow;
		//是否判断相位快变报警
		bool _bPhaseFast;
		//缓变门限学习参数
		CThresholdLearnParam _pThLearnParamS[D_BHKD_ALARM_CHARTYPE_NUM];
		//快变门限学习参数
		CThresholdLearnParam _pThLearnParamF[D_BHKD_ALARM_CHARTYPE_NUM];//如果是往复式功能，表示四个角度
		CTrendCheckParam _pTrendParam[D_BHKD_ALARM_CHARTYPE_NUM];
		//常规判断报警的相关参数
		float _fAlarmLL;///如果是单值报警时，表示下下限；2值是表示圆心的X值
		float _fAlarmLH;///如果是单值报警时，表示下限；2值是表示圆心的Y值
		float _fAlarmHL;///如果是单值报警时，表示上限；2值是表示半径的下限
		float _fAlarmHH;///如果是单值报警时，表示上上限；2值是表示半径的上限

		float _fAlarm1; ///预留
		float _fAlarm2; ///预留
		float _fAlarm3; ///预留
		int   _iResv1;   ///预留

		int   _iRegJudgeType;//表示是低通、高通、带通、带阻
	    int _iRegAlarmSingleOrMulti;//表示单个特征值判断报警还是2个特征值判断报警。1表示单个，2表示2个
		///常规报警采用的特征值类型，如果是单值就是E_ALARM_CHARTYPE枚举表示的特征值类型，如果是2个或多个就用D_BHKD_AT_CHARONE方式进行与操作得到。
		int  _iRegCharType;
		CChannAlarmParam& operator=(const CChannAlarmParam& chAlarmParam_);
	};

	// 特征值描述信息
	class S_CharInfo
	{
	public:
		S_CharInfo(int charIndex_, const string& charName_, const string& charUnit_);
		S_CharInfo(const S_CharInfo & charInfo_);

		S_CharInfo & operator=(const S_CharInfo & charInfo_);
	public:
		int    GetCharIndex() const;
		string GetCharName() const;
		string GetCharUnit() const;

	private:
		S_CharInfo();

	private:
		int    _iCharIndex;
		string _sCharName;
		string _sCharUnit;
	};

	///测点信息结构体
	class S_OldChannInfo
	{
	public:
		char _cChannID[D_BHKD_CHANNID_LEN];     //ID of Channel
		char _cChannNo[D_BHKD_CHANNNO_LEN];     //No of Channel
		int _iID;               //ID
		char _cCompany[D_BHKD_COMPANYNAME_LEN];    //
		char _cFactory[D_BHKD_FACTORYNAME_LEN];
		char _cPlantNo[D_BHKD_PLANTNO_LEN];     //No of Plant
		char _cChannAlias[D_BHKD_CHANNALIAS_LEN];  //alias of channel
		int _iChannType;        //the type of channel.For example:vibration ,process
		int _iSensorType;       //the type of sensor.For example: votex,acceleration et al
		int _iRevType;          //测点属于那个转速
		int _iList;             //list number of channel in graph
		char _cUnit[20];        //unit of channel signal
		float _fSensitivity;      //Sensitivity of sensor;
		bool _bKeyIF;            //是否存在键相
		float _fGapVoltage;      //间隙电压
		int _iInfoType;         //信息描述类型
		float _fSensitivity1;  //灵敏度1
		int  _iCylinderNo;      // 部位索引(气缸编号) = SectionNo ,如果是往复的话
		CChannAlarmParam _chAlarmParam;//报警参数
		char _cTempID[20];           //自定义参数模板ID
		char _cAlarmTempID[20];      //自定义参数报警模板ID
		int   _iLocationIdx;       // 位置索引
		int   _iMeassureDirection; // 测量方向
	public:
		S_OldChannInfo();
		S_OldChannInfo& operator=(const S_OldChannInfo& chInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(CSimADO *pDB_);
		//更新通道信息到指定连接
		bool SetChannInfo(CSimADO *pDB_);


		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(IDBInterFace *pDB_);
		//更新通道信息到指定连接
		bool SetChannInfo(IDBInterFace *pDB_);


#endif//_BHKD_SERVER_SIDE_
	};

	///测点信息结构体
	class S_ChannInfo
	{
	public:
		char _cChannID[D_BHKD_CHANNID_LEN];     //ID of Channel 对应客户端的name
		char _cChannNo[D_BHKD_CHANNNO_LEN];     //No of Channel 对应客户端的number
		int _iID;               //ID
		char _cCompany[D_BHKD_COMPANYNAME_LEN];    //
		char _cFactory[D_BHKD_FACTORYNAME_LEN];
		char _cPlantNo[D_BHKD_PLANTNO_LEN];     //No of Plant
		char _cChannAlias[D_BHKD_CHANNALIAS_LEN];  //alias of channel
		int _iChannType;        //the type of channel.For example:vibration ,process
		int _iSensorType;       //the type of sensor.For example: votex,acceleration et al
		int _iRevType;          //测点属于那个转速
		int _iList;             //list number of channel in graph
		char _cUnit[20];        //unit of channel signal
		float _fSensitivity;      //Sensitivity of sensor;
		bool _bKeyIF;            //是否存在键相
		float _fGapVoltage;      //间隙电压
		int _iInfoType;         //信息描述类型
		float _fSensitivity1;  //灵敏度1
		int  _iCylinderNo;      // 部位索引(气缸编号)
		CChannAlarmParam _chAlarmParam;//报警参数
		char _cTempID[20];           //自定义参数模板ID
		char _cAlarmTempID[20];      //自定义参数报警模板ID
		int   _iLocationIdx;       // 位置索引
		int   _iMeassureDirection; // 测量方向
		int   _iDataSource;        // 数据源
		int   _iDataType;
		float _fDCAmp;
		float _fACAmp;
		int	  _iLocationNo;
		map<int, S_CharInfo> _charInfo; // 特征值信息
	public:
		S_ChannInfo();
		S_ChannInfo(const S_ChannInfo& chInfo_);
		S_ChannInfo(const S_OldChannInfo & chInfo_);
		S_ChannInfo(const S_ChannInfo * pChannInfo);
		S_ChannInfo& operator=(const S_ChannInfo& chInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(CSimADO *pDB_);
		bool GetCharInfo(CSimADO *pDB_);
		//更新通道信息到指定连接
		bool SetChannInfo(CSimADO *pDB_);

		//从指定的数据连接中得到设备详细信息
		bool GetChannInfo(IDBInterFace *pDB_);
		bool GetCharInfo(IDBInterFace *pDB_);
		//更新通道信息到指定连接
		bool SetChannInfo(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};

	///自定义参数信息
	class CCustomParamInfo
	{
	public:
		CCustomParamInfo();
		~CCustomParamInfo();
	public:
		E_CUSTOMPARAM_DATATYPE		_eDataType;
		E_CUSTOMPARAM_UNITTYPE		_eUnitType;
        E_CUSTOMPARAM_SIGANATYPE	_eSigAnaType;
		E_CUSTOMPARAM_CHARTYPE		_eCharType;
		char	_cCompany[D_BHKD_COMPANYNAME_LEN];
		char	_cFactory[D_BHKD_FACTORYNAME_LEN];
		char	_cSetID[D_BHKD_SETID_LEN];
		char	_cTempletID[20];
		char	_cSegName[100];
        int     _iID;
		int     _iSegNum;
		int     _iSegStartNo;
		int     _iPreProcType;        //预处理类型
		float   _fDataStartValue;
		float   _fDataEndValue;
		float   _fBandStartValue;
		float   _fBandEndValue;
		float	_fPREPROC_K;
		float   _fPREPROC_B;
	public:
		CCustomParamInfo& operator=(const CCustomParamInfo& customParamInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到自定义参数详细信息
		bool GetCustomParamInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到自定义参数详细信息
		bool GetCustomParamInfo(CSimADO *pDB_);

		//从指定的数据连接中得到自定义参数详细信息
		bool GetCustomParamInfo(IDBInterFace *pDB_);

#endif//_BHKD_SERVER_SIDE_
	};

    ///自定义报警参数信息
	class CCustomAlarmParamInfo
	{
	public:
		CCustomAlarmParamInfo();
		~CCustomAlarmParamInfo();
	public:
		char	_cCompany[D_BHKD_COMPANYNAME_LEN];
		char	_cFactory[D_BHKD_FACTORYNAME_LEN];
		char	_cSetID[D_BHKD_SETID_LEN];
		char	_cAlarmTempletID[20];
		char	_cSegName[100];
		int     _iID;
		int     _iSegStartNo;
		int     _iSegNum;
		int     _iJudgeType;
		int     _iAlarmType;
		int     _iIF_Alarm;
		int     _iAlarmGroup;
		float   _fAlarmLTH;
		float   _fAlarmHTH;
	public:
		CCustomAlarmParamInfo& operator=(const CCustomAlarmParamInfo & customAlarmParamInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到自定义报警参数详细信息
		bool GetCustomAlarmParamInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到自定义报警参数详细信息
		bool GetCustomAlarmParamInfo(CSimADO *pDB_);

		//从指定的数据连接中得到自定义报警参数详细信息
		bool GetCustomAlarmParamInfo(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};

    ///测点位置信息
	class CChannLocations
	{
	public:
		CChannLocations();
		~CChannLocations();
	public:
		char	_cCompany[D_BHKD_COMPANYNAME_LEN];
		char	_cFactory[D_BHKD_FACTORYNAME_LEN];
		char	_cSetID[D_BHKD_SETID_LEN];
		char    _cPlantNo[D_BHKD_PLANTNO_LEN];
		char    _cLocationName[100];
		char    _cComment[100];
		int     _iLocationIDX;
		int     _iSectionIDX;
		int     _iBearingTblType;
		int     _iBearingIDX;
		int     _iShaftNumber;
	public:
		CChannLocations& operator=(const CChannLocations &channLocations_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到测点位置信息
		bool GetChannLocations(CCustomDB *pDB_);
		//从指定的数据连接中得到测点位置信息
		bool GetChannLocations(CSimADO *pDB_);
		//从指定的数据连接中得到测点位置信息
		bool GetChannLocations(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};

	///测点部位信息
	class CChannSections
	{
	public:
		CChannSections();
		~CChannSections();
	public:
		char	_cCompany[D_BHKD_COMPANYNAME_LEN];
		char	_cFactory[D_BHKD_FACTORYNAME_LEN];
		char	_cSetID[D_BHKD_SETID_LEN];
		char    _cPlantNo[D_BHKD_PLANTNO_LEN];
		char    _cSectionName[100];
		char    _cComment[100];
		int     _iSectionIDX;
		int     _iGearBoxIDX;
	public:
		CChannSections& operator=(const CChannSections &channSections_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到测点部位信息
		bool GetChannSections(CCustomDB *pDB_);
		//从指定的数据连接中得到测点部位信息
		bool GetChannSections(CSimADO *pDB_);
		//从指定的数据连接中得到测点部位信息
		bool GetChannSections(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};

    ///轴承参数信息
	class CBearingParamInfo
	{
	public:
		CBearingParamInfo();
		~CBearingParamInfo();
	public:
		char    _cBearingName[100];
		char    _cComment[100];
	    float	_fPitchDiameter;
		float	_fElementDiameter;
		float	_fContactAngle;
		float	_fHousingDiameter;
		float	_fBoreDiameter;
		int		_iBearingIDX;
		int		_iBearingType;
		int		_iRowsNum;
		int		_iElementsNum;
		int		_iBearingCode;
	public:
		CBearingParamInfo& operator=(const CBearingParamInfo &bearingParamInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到轴承参数详细信息
		bool _GetBearingParamInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到轴承参数详细信息
		bool _GetBearingParamInfo(CSimADO *pDB_);

		//从指定的数据连接中得到轴承参数详细信息
		bool _GetBearingParamInfo(IDBInterFace *pDB_);


		//从指定的数据连接中得到轴承参数详细信息
		bool GetBearingParamInfo(CString & astrSql);

#endif//_BHKD_SERVER_SIDE_
	};

	///齿轮箱参数信息
	class CGearBoxParamInfo
	{
	public:CGearBoxParamInfo();
		   ~CGearBoxParamInfo();
	public:
		char	_cCompany[D_BHKD_COMPANYNAME_LEN];
		char	_cFactory[D_BHKD_FACTORYNAME_LEN];
		char	_cSetID[D_BHKD_SETID_LEN];
		char    _cPlantNo[D_BHKD_PLANTNO_LEN];
		char    _cGearBoxName[100];
		char    _cComment[100];
		int     _iGearBoxIDX;
		int		_iGearBoxLevel;
		int     _iGearBoxType;
		int		_iDriverType;
		int		_iPlanetNum;
		int		_iGearNumSun;
		int		_iGearNumPlanet;
		int		_iGearNumGalaxy;
		int		_iIsSync;
		int     _iGearNumZ12;
		int     _iGearNumZ21;
		int     _iGearNumZ22;
		int     _iGearNumZ31;
		int     _iGearNumZ32;
		int     _iGearNumZ41;
		int     _iGearNumZ42;
		int     _iGearNumZ51;
		int     _iGearNumZ52;
		int     _iGearNumZ61;
	public:
		CGearBoxParamInfo& operator=(const CGearBoxParamInfo &gearBoxParamInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到齿轮箱参数详细信息
		bool GetGearBoxParamInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到齿轮箱参数详细信息
		bool GetGearBoxParamInfo(CSimADO *pDB_);

		//从指定的数据连接中得到齿轮箱参数详细信息
		bool GetGearBoxParamInfo(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};


	///轴心位置通道信息
	class CAxisChannInfo
	{
	public:
		CAxisChannInfo();
		virtual ~CAxisChannInfo();
		int _iID;               //ID
		char _cCompany[D_BHKD_COMPANYNAME_LEN];
		char _cFactory[D_BHKD_FACTORYNAME_LEN];
		char _cPlantNo[D_BHKD_PLANTNO_LEN];     //No of Plant
		char _cVerChann[D_BHKD_CHANNNO_LEN];
		char _cHorChann[D_BHKD_CHANNNO_LEN];
		int _iLocationType;
		int _iRotateDirection;//旋转方向,0逆时针,1顺时针
		float _fAxisDia;//对应的轴直径
		char _cLocation[D_BHKD_CHANNALIAS_LEN];
		CAxisAlarmParam _axisAlarmParam;
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到轴心通道详细信息
		bool GetAxisChannInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到轴心通道详细信息
		bool GetAxisChannInfo(CSimADO *pDB_);
		//更新数据连接的信息
		bool SetAxisChannInfo(CSimADO *pDB_);

		//从指定的数据连接中得到轴心通道详细信息
		bool GetAxisChannInfo(IDBInterFace *pDB_);
		//更新数据连接的信息
		bool SetAxisChannInfo(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
		virtual CAxisChannInfo& operator=(const CAxisChannInfo& aChannInfo_);
	};

	//轴心位置数据结构体
	class S_AxisData
	{
	public:
		S_AxisData();
		S_AxisData& operator=(const S_AxisData& axisData_);
		float _fRev;
		float _fCoorX;
		float _fCoorY;
		float _fVerDC;
		float _fVerGap;
		float _fHorDC;
		float _fHorGap;
		bool  _bCalc;             ///判断是否已经计算过轴心位置的值
#ifdef _BHKD_SERVER_SIDE_
		bool CalcAxisLocation(int iLocationType_,int iRotateDirection_,float fAxisDia_,float fSensitivityVer_,float fSensitivitypHor_);
#endif//_BHKD_SERVER_SIDE_
	};


    ///往复压缩机单缸设备结构体
	class S_PlantInfoParam_Deprecated
	{//长度单位是厘米，体积单位为立方米，总功率就是千瓦，压力MPa
	public:
		float _fConnRodLength;///连杆长度
		float _fCrankShaftDia;////曲轴半径
		float _fPistonDia;    ///活塞直径
		float _fTotalPower;   ///总功率
		float _fNomPi;        ///名义进气压力
		float _fNomPo;        ///名义出气压力
		float _fClv;          ///余隙（clearance）容积
		float _fRes1;         ///活塞杆直径
		float _fRes2;         ///惯性体质量
		int _iRes1;           ///<气缸作用类型
	public:
		S_PlantInfoParam_Deprecated();
		S_PlantInfoParam_Deprecated& operator=(const S_PlantInfoParam_Deprecated& sPlantInfoParam_);
	};

	#define PLANTINFO_PARAM_NUM                    (10)

	///缸的个数结构体
	class S_RecipComprParam_Deprecated
	{
	public:
		S_PlantInfoParam_Deprecated _sPlantInfoParam[PLANTINFO_PARAM_NUM];
	public:
		S_RecipComprParam_Deprecated();
		S_RecipComprParam_Deprecated& operator=(const S_RecipComprParam_Deprecated& sRecipComprParam_);
	};

    ///往复压缩机单缸设备结构体
	class S_PlantInfoParam
	{//长度单位是厘米，体积单位为立方米，总功率就是千瓦，压力MPa
	public:
		float _fConnRodLength;///连杆长度
		float _fCrankShaftDia;////曲轴半径
		float _fPistonDia;    ///活塞直径
		float _fTotalPower;   ///总功率
		float _fNomPi;        ///名义进气压力
		float _fNomPo;        ///名义出气压力
		float _fClv;          ///余隙（clearance）容积
		float _fRes1;         ///活塞杆直径
		float _fRes2;         ///惯性体质量
		int _iRes1;

		/// 增加

		float _fPistonRodLength;       ///<活塞杆长度
		float _fPistonThickness;       ///<活塞厚度

		float _fPistonCylinderWallGap; ///<(沉降) 活塞-缸壁间隙
		float _fSensorLocation;        ///<(沉降) 传感器位置 Sink
		float _fPistonCylinderWallGap_Deflection; ///<(偏摆) 活塞-缸壁间隙
		float _fSensorLocation_Deflection;        ///<(偏摆) 传感器位置
	public:
		S_PlantInfoParam();
		S_PlantInfoParam(const S_PlantInfoParam_Deprecated& sDeprecated);
		S_PlantInfoParam& operator=(const S_PlantInfoParam& sPlantInfoParam_);
	};

	///缸的个数结构体
	class S_RecipComprParam
	{
	public:
		S_PlantInfoParam _sPlantInfoParam[PLANTINFO_PARAM_NUM];
	public:
		S_RecipComprParam();
		S_RecipComprParam::S_RecipComprParam(const S_RecipComprParam_Deprecated & sDeprecated_);
		S_RecipComprParam& operator=(const S_RecipComprParam& sRecipComprParam_);
	};

	///设备信息结构体
	class S_PlantInfo
	{
	public:
		S_PlantInfo();
		~S_PlantInfo();
		char _cCompany[D_BHKD_COMPANYNAME_LEN];       //
		char _cFactory[D_BHKD_FACTORYNAME_LEN];       //
		char _cManufacturer[100];  //制造厂商
		char _cSetID[D_BHKD_SETID_LEN];          //装置名
		char _cPlantID[D_BHKD_PLANTID_LEN];      //设备名
		char _cPlantNo[D_BHKD_PLANTNO_LEN];      //设备号，英文数字组合
		int _iVibChannNum;         //振动测点数目
		int _iSmpFreq;             //采样频率
		int _iSmpNum;              //采样点数
		int _iProcChannNum;        //过程量测点数目
		int _piRev[5];            //额定转速1~5
		int _iGraphType;           //概貌图类型
		int _iMachineType;         //设备类型
		int _iDriverType;          //驱动机类型
		int _iConnType;            //联结类型（联轴节类型）
		int _iBearingType;         //轴承类型
		int _iRotorType;           //转子类型
		int _iSealType;            //密封类型
		int _iWorkRevType;         //工作转速类型,是处于一阶临界转速附近
		int _iCritSpeed1st;        //一阶临界转速
		int _iCritSpeed2nd;        //二阶临界转速
        CBHDateTime _firstRunT;   //首次投运时间

		int _iCylinderNum;          //气缸个数
		int _iDynChannNum;         //动态数据通道的数目
		int _iGraphBuffBytes;      //自制概貌图的字节数
		BYTE *_pGraphBuff;         //自制概貌图的BUFF指针
    ///机组的一些机械参数结构体。9个浮点数数和1个整数.目前就往复式压缩机用
		S_RecipComprParam _sRecipComprParam; ///缸的个数结构体
		int   _iDataSource;         //数据源
		char _sVersion[D_BHKD_VERSION_LEN] ;           //版本信息
	public:
		S_PlantInfo& operator=(const S_PlantInfo& sPlantInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到设备详细信息
		bool GetPlantInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到设备详细信息
		bool GetPlantInfo(CSimADO *pDB_);
		//更新数据连接的信息
		bool SetPlantInfo(CSimADO *pDB_);

		//从指定的数据连接中得到设备详细信息
		bool GetPlantInfo(IDBInterFace *pDB_);
		//更新数据连接的信息
		bool SetPlantInfo(IDBInterFace *pDB_);

#endif//_BHKD_SERVER_SIDE_
		int m_nDbId;
	};

	class S_PlantInfoEx
	{
	public:
		S_PlantInfo _plantInfo;
		BOOL _bGroupVisible;
		BOOL _bCompanyVisible;
		BOOL _bFactoryVisible;
		BOOL _bSetVisible;
		BOOL _bPlantVisible;
		CString m_strGroup;
		CString m_strContry;
		CString m_strPlantDisplayName;
		CString m_strSetDisplayName;
		CString m_strFactoryDisplayName;
		CString m_strCompanyDisplayName;
		CString m_strGroupDisplayName;
		CString m_strErpCode;
		CString m_strCompanyAlias;
		int     m_DeviceId;
		int     m_nGroupSortNO;
		int     m_nCompanySortNO;
		int     m_nFactorySortNO;
		int     m_nSetSortNO;
		int     m_nPlantSortNO;
	public:
		S_PlantInfoEx();
	};

	//中间件服务器信息
	class S_SetInfo
	{
	public:
	    int _iID;
		char _cCompany[D_BHKD_COMPANYNAME_LEN];       //
		char _cFactory[D_BHKD_FACTORYNAME_LEN];       //
		char _cSetID[D_BHKD_SETID_LEN];
		char _cIP[D_BHKD_IP_LEN];
		int _iPort;
		int _iSendInterval;
		int _iIfSend;
		int _iSudSendInterval;
		int _iAlarmSendInterval;
		char _cTmIP[D_BHKD_IP_LEN];
		int _iTmPort;
		char _cParentID[D_BHKD_SETID_LEN];
		int _iChildFlag;
	public:
		S_SetInfo();
		S_SetInfo& operator=(const S_SetInfo& sSetInfo_);
#ifdef _BHKD_SERVER_SIDE_
		//从指定的数据连接中得到设备详细信息
		bool GetSetInfo(CCustomDB *pDB_);
		//从指定的数据连接中得到设备详细信息
		bool GetSetInfo(CSimADO *pDB_);
		//从指定的数据连接中得到设备详细信息
		bool GetSetInfo(IDBInterFace *pDB_);
#endif//_BHKD_SERVER_SIDE_
	};

	///振动测点特征值结构体
	class S_VibCharValue
	{
	public:
		S_VibCharValue();
		~S_VibCharValue();
		DATATYPE_CHAR _fCharAll;    //通频特征值
		DATATYPE_CHAR _fCharHalf;   //1/2分频特征值
		DATATYPE_CHAR _fCharOne;    //1倍频特征值
		DATATYPE_CHAR _fCharTwo;    //2倍频特征值
		DATATYPE_CHAR _fCharThree;  //3倍频特征值
		DATATYPE_CHAR _fCharRes;    //残振特征值
		DATATYPE_CHAR _fDC;         //直流量
		DATATYPE_CHAR _fGap;        //间隙值
		DATATYPE_CHAR _fPhaseHalf;  //1/2分频相位
		DATATYPE_CHAR _fPhaseOne;   //1倍频相位
		DATATYPE_CHAR _fPhaseTwo;   //2倍频相位
		DATATYPE_CHAR _fPhaseThree; //3倍频相位
		DATATYPE_INT  _iSmpFreq;    //采样频率
		DATATYPE_INT  _iSmpNum;     //采样点数
		DATATYPE_INT  _iRev;        //转速

	public:
		S_VibCharValue& operator=(const S_VibCharValue& vibValue_);
		///得到指定类型的特征值
		DATATYPE_CHAR GetSpecTypeData(int iCharType_=GE_ALLCHAR);
		void SetSpecTypeData(float fData_,int iCharType_=GE_ALLCHAR);
	};

	//为兼容接收老版本数采的特征值结构体
	class S_OldVibCharValue
	{
	public:
		S_OldVibCharValue();
		~S_OldVibCharValue();
		DATATYPE_CHAR _fCharAll;    //通频特征值
		DATATYPE_CHAR _fCharHalf;   //1/2分频特征值
		DATATYPE_CHAR _fCharOne;    //1倍频特征值
		DATATYPE_CHAR _fCharTwo;    //2倍频特征值
		DATATYPE_CHAR _fCharThree;  //3倍频特征值
		DATATYPE_CHAR _fCharRes;    //残振特征值
		DATATYPE_CHAR _fDC;         //直流量
		DATATYPE_CHAR _fGap;        //间隙值
		DATATYPE_CHAR _fPhaseHalf;  //1/2分频相位
		DATATYPE_CHAR _fPhaseOne;   //1倍频相位
		DATATYPE_CHAR _fPhaseTwo;   //2倍频相位
		DATATYPE_CHAR _fPhaseThree; //3倍频相位

	public:
		S_OldVibCharValue& operator=(const S_OldVibCharValue& vibValue_);
		///得到指定类型的特征值
		DATATYPE_CHAR GetSpecTypeData(int iCharType_=GE_ALLCHAR);
		void SetSpecTypeData(float fData_,int iCharType_=GE_ALLCHAR);
	};

	///动态通道的特征值数据结构体
	class S_DynCharValue
	{
	public:
		S_DynCharValue();
		~S_DynCharValue();
		DATATYPE_CHAR _fCharValue0;
		DATATYPE_CHAR _fCharValue1;
		DATATYPE_CHAR _fCharValue2;
		DATATYPE_CHAR _fCharValue3;
		DATATYPE_CHAR _fCharValue4;
		DATATYPE_CHAR _fCharValue5;
		DATATYPE_CHAR _fCharValue6;
		DATATYPE_CHAR _fCharValue7;
		DATATYPE_CHAR _fCharValue8;
		DATATYPE_CHAR _fCharValue9;
		DATATYPE_CHAR _fCharValue10;
		DATATYPE_CHAR _fCharValue11;
		DATATYPE_INT  _iSmpFreq;
		DATATYPE_INT  _iSmpNum;
		DATATYPE_INT  _iRev;
    public:
		S_DynCharValue& operator=(const S_DynCharValue& dynValue_);
		DATATYPE_CHAR GetSpecTypeData(int iCharType_=0);
		void SetSpecTypeData(const float &fData_,int iCharType_=0);
	};

    //为兼容接收老版本数采的特征值结构体
	class S_OldDynCharValue
	{
	public:
		S_OldDynCharValue();
		~S_OldDynCharValue();
		DATATYPE_CHAR _fCharValue0;
		DATATYPE_CHAR _fCharValue1;
		DATATYPE_CHAR _fCharValue2;
		DATATYPE_CHAR _fCharValue3;
		DATATYPE_CHAR _fCharValue4;
		DATATYPE_CHAR _fCharValue5;
		DATATYPE_CHAR _fCharValue6;
		DATATYPE_CHAR _fCharValue7;
		DATATYPE_CHAR _fCharValue8;
		DATATYPE_CHAR _fCharValue9;
		DATATYPE_CHAR _fCharValue10;
		DATATYPE_CHAR _fCharValue11;
	public:
		S_OldDynCharValue& operator=(const S_OldDynCharValue& dynValue_);
		DATATYPE_CHAR GetSpecTypeData(int iCharType_=0);
		void SetSpecTypeData(const float &fData_,int iCharType_=0);
	};

    ///报警状态标识
	struct S_AlarmState
	{
       S_AlarmState()
	   {
		   _iFastState=0;
		   _iSlowState=0;
		   _iTrendState=0;
		   _iRegState=0;
	   }
	   unsigned int _iFastState;//快变报警状态
	   unsigned int _iSlowState;//缓变报警状态
	   unsigned int _iTrendState;//趋势报警状态
	   unsigned int _iRegState;//常规报警状态
	};

	struct S_AlarmStateEx
	{
		S_AlarmStateEx()
		{
			_iFastState=0;
			_iSlowState=0;
			_iTrendState=0;
			_iRegState=0;
			_fLastAlarmVal=0;
		}
		unsigned int _iFastState;//快变报警状态
		unsigned int _iSlowState;//缓变报警状态
		unsigned int _iTrendState;//趋势报警状态
		unsigned int _iRegState;//常规报警状态
		float _fLastAlarmVal; // 最近报警时的特征值
	};

	struct S_AlarmStateWithLevel
	{
		S_AlarmStateWithLevel()
		{
			_iFastState=0;
			_iSlowState=0;
			_iTrendState=0;
			_iRegState=0;
			_iAlarmLevel = 0;
			_fLastAlarmVal=0;
		}
		unsigned int _iFastState;//快变报警状态
		unsigned int _iSlowState;//缓变报警状态
		unsigned int _iTrendState;//趋势报警状态
		unsigned int _iRegState;//常规报警状态
		int          _iAlarmLevel;
		float _fLastAlarmVal; // 最近报警时的特征值
	};

    ///设备报警统计数目结构体
	struct S_PlantNumStatics
	{
		S_PlantNumStatics()
		{
			_iPlantNum      = 0;
			_iAlarmPlantNum = 0;
			_iRunPlantNum   = 0;
			_iStopPlantNum  = 0;
		}
		S_PlantNumStatics& operator=(const S_PlantNumStatics& plantNumStatics_);
		S_PlantNumStatics& operator+(const S_PlantNumStatics& plantNumStatics_);
		int _iPlantNum;//设备数目
		int _iAlarmPlantNum;//报警设备数目
		int _iRunPlantNum;//运行设备数目
		int _iStopPlantNum;//停车设备数目
	};

    ///保存报警日志信息结构体
	struct S_AlarmlogInfo
	{
		int _iAlarmID;//报警事件ID
		int _iAlarmType;//报警类型
		int _cChannType;//通道类型
		string _sName;//测点名称
		CBHDateTime _tStart;//报警开始时间
		CBHDateTime _tEnd ;//报警结束时间
		int _iAlarmLevel;
	};

	///自定义参数数据结构体，长度为50
	struct S_CustomParamData
	{
		float pfCustomParom[GD_CUSTOMPARAM_BUFFSIZE];
	};

	///齿轮箱特征频率结构体
	struct S_GearBoxCharFreq
	{
		float fEnvFreq1;//啮合频率
		float fEnvFreq2;//输出轴转频
		float fShaftCF1;//齿轮箱I轴转频
		float fShaftCF2;//齿轮箱Ⅱ轴转频
		float fShaftCF3;//齿轮箱Ⅲ轴转频
		float fShaftCF4;//齿轮箱Ⅳ轴转频
	};

	/**
	  中间件接收数据状态
	*/
	struct BHMWDataRcvStatus
	{
		char    vibStatus;
		char    dynStatus;
		char    procStatus;
		char    plantName[D_BHKD_PLANTID_LEN];
		BHMWDataRcvStatus()
		{
			memset(plantName,0,D_BHKD_PLANTID_LEN);
		}
	};

	/**
	  中间件保存数据状态
	*/
	struct BHMWDataSyncStatus
	{
		char     trendStatus;
		char     sudStatus[D_BHKD_STATUS_LEN];
		char     alarmStatus[D_BHKD_STATUS_LEN];
		char     plantName[D_BHKD_PLANTID_LEN];
		BHMWDataSyncStatus()
		{
			memset(sudStatus,0,D_BHKD_STATUS_LEN);
			memset(alarmStatus,0,D_BHKD_STATUS_LEN);
			memset(plantName,0,D_BHKD_PLANTID_LEN);
		}
	};


	/**
	  中间件同步数据状态
	*/
	struct BHMWDataSaveStatus
	{
		char   trendStatus;
		char   sudStatus;
		char   alarmStatus;
		char   plantName[D_BHKD_PLANTID_LEN];
		BHMWDataSaveStatus()
		{
			memset(plantName,0,D_BHKD_PLANTID_LEN);
		}
	};

	// Copy_if was dropped from the standard library by accident.
	// Bjarne Stroustrup. The C++ Programming Language (3rd edition). 1997. ISBN: 0-201-88954-4. Chapter 18.12.1: 'Prefer algorithms to loops.
	// Scott Meyers. Effective STL. ISBN: 0-201-74962-9. Item 43: 'Prefer algorithm calls over hand-written loops'
	// richelbilderbeek
	template<typename In, typename Out, typename Pred>
	Out copy_if(In first, In last, Out res, Pred Pr)
	{
	  while (first != last)
	  {
	    if (Pr(*first))
	      *res++ = *first;
	    ++first;
	  }
	  return res;
	}


class S_Machineinfotype
{

public:
	S_Machineinfotype();
	virtual ~S_Machineinfotype();
public:
	int m_nInfoType;
	int m_nIndex;
	CString m_strComment;
	int m_nField;
	int m_nField1;
	CString m_strUnit;
public:
	BOOL LoaddData(IDBInterFace & arefDB);
};

#ifdef _BHKD_SERVER_SIDE_
	bool SwichDbName(CCustomDB *pDB_,CString sDbName_);
	bool SimADO_SafeGetBuffer( CSimADO * pDB, const char * sField, BYTE *pBuf, long &nSize);

	bool SimADO_GetRecipComprParam(CSimADO * pDB_, BYTE *pBuf_, long &nSize_);
	bool SimADO_SetRecipComprParam(CSimADO * pDB_, BYTE *pBuf_, long nSize_);

	bool SimADO_GetChannAlarmParam(CSimADO * pDB_, BYTE *pBuf_, long &nSize_);
	bool SimADO_SetChannAlarmParam(CSimADO * pDB_, BYTE *pBuf_, long nSize_);



	void SwitchDbNameEx(IDBInterFace *pDB_,CString sDbName_);

	bool SimADO_SafeGetBufferEx( IDBInterFace * pDB, const char * sField, BYTE *pBuf, long &nSize, CWaveHead& sWaveHead_);

	bool SimADO_GetRecipComprParamEx(IDBInterFace * pDB_, BYTE *pBuf_, long &nSize_);
	bool SimADO_SetRecipComprParamEx(IDBInterFace * pDB_, BYTE *pBuf_, long nSize_);

	bool SimADO_GetChannAlarmParamEx(IDBInterFace * pDB_, BYTE *pBuf_, long &nSize_);
	bool SimADO_SetChannAlarmParamEx(IDBInterFace * pDB_, BYTE *pBuf_, long nSize_);

#endif
	BOOL IsFloatValid(float f);
	BOOL IsFloatZero(float f);
	BOOL IsDynCharValid(S_DynCharValue * lpCharVal);
	BOOL IsVibCharValid(S_VibCharValue * lpCharVal);
	int   float_as_int(float in) ;
	float int_as_float(int in); 

}

// 默认概貌图ID
enum DEFAULT_GRAPH_ID
{
	RECT_GRAPH_ID      = 100011,		//往复设备默认概貌图ID
	ROTATION_GRAPH_ID  = 100012,		//旋转设备默认概貌图ID
	WINDPOWER_GRAPH_ID = 100013,		//风电设备默认概貌图ID
	PUMP_GRAPH_ID      = 100014,		//机泵/无线机泵设备默认概貌图ID
	TURBINE_GRAPH_ID   = 100015,		//燃气轮机设备默认概貌图ID
	DIESEL_GRAPH_ID    = 100016,		//柴油机设备默认概貌图ID
	ICE_GRAPH_ID       = 100017,		//内燃机设备默认概貌图ID
};

enum E_MACHINE_TYPE
{
	MACHINE_TYPE_WC,
	MACHINE_TYPE_16PA6,
	MACHINE_TYPE_620,
	MACHINE_TYPE_956,
	MACHINE_TYPE_GASTURBINE,
	MACHINE_TYPE_PUMP,
	MACHINE_TYPE_ROTATE,
	MACHINE_TYPE_RC,
	MACHINE_TYPE_LIFT
};

enum E_ALARM_LEVEL
{
	ALARM_NORMAL,
	ALARM_COMPARE_ABNORMAL,
	ALARM_POTENTIAL,
	ALARM_DEVELOPING,
	ALARM_URGENT,
	ALARM_DANGEOUS
};


//报警类型
enum E_ALARM_TYPE
{
	E_ALARM_TYPE_THRESHOLD=0,
	E_ALARM_TYPE_SEFL_LEARNING,
	E_ALARM_TYPE_MULTIDEVICE_COMP,
	E_ALARM_TYPE_MULTIDEVICE_TREND
};

#endif //_BHKD_G_UTILITIES_H_MARBLE_
