//Implement.h
#pragma once
#include "StdAfx.h"
#include "PlantInfoExpertSys.h"
#include <additions/IDBInterFace.h>
#include "ExpertSysGlobal.h"

#ifndef _EXPERTSYSTEMSVR_IMPLEMENT_H_
#define _EXPERTSYSTEMSVR_IMPLEMENT_H_

class CImplement
{
public:
	CImplement(void);
	~CImplement(void);
public:

	/**得到指定报警ID时,专家诊断系统特征提取的知识
	*输入参数：   公司名     FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SERVICE_PLANTID
	*             报警ID     FIELD_SERVICE_ALARMID
	*返回的字段码：
	*             报警通道   FIELD_SERVICE_OFF_CHANN1
	*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
	*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
	*/
	int ExpertSysGetRegAlarmFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
	*输入参数：   公司名     FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SERVICE_PLANTID
	*             测点ID     FIELD_SERVICE_CHANN1
	*             起始时间   FIELD_SERVICE_OFF_TIMESTART
	*             终止时间   FIELD_SERVICE_OFF_TIMEEND
	*返回的字段码：
	*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
	*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
	#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT      30002
	*/
    int ExpertSysGetSpecChannFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
	*输入参数：   公司名     FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SERVICE_PLANTID
	*返回的字段码：
	*             公司名     FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SERVICE_PLANTID
	*             事实名     FIELD_SERVICE_REMARK     //对应为字符串
	*#define    SERVICE_EXPERTSYS_GET_PLANTFACTNAME      30004
	*/
	int ExpertSysGetPlantFactName(ICommInterFace *pFixComm,IDBInterFace *pDB);

	enum ALARM_CHANN_TYPE
	{
		EXPERT_CHANN_TYPE_TEMP_INVAL    =0x0000,
		EXPERT_CHANN_TYPE_TEMP			=0x0001,
		EXPERT_CHANN_TYPE_SINK			=0x0010,
		EXPERT_CHANN_TYPE_CROSSHEAD		=0x0011,
		EXPERT_CHANN_TYPE_IMPACTS		=0x0100,
		EXPERT_CHANN_TYPE_CRANK			=0x0101,
		EXPERT_CHANN_TYPE_DEFLECT		=0x0110,
		EXPERT_CHANN_TYPE_PV			=0x0111,
		EXPERT_CHANN_TYPE_ALL			=0x1000,
	};

	public:

	/**写入事实ID及其对应的值
	*@param sFactID_ 事实ID
	*@param iValue_ 事实ID对应的值
	*@param pFixComm_ 网络连接指针
	*/
    void WriteFertureInfo(string sFactID_,int iValue_,ICommInterFace *pFixComm_);

	/**写入专家诊断系统的机组信息特征提取知识
	*@param pPlantInfo_ 设备信息指针
	*@param pFixComm_ 网络连接指针
	*/
    void WriteMachineFertureInfo(CPlantInfoExpertSys *pPlantInfo_ ,ICommInterFace *pFixComm_);

	/**从指定网络连接中读取并判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	*@param sCompany_ 公司名，是一个引用
	*@param sFactory_ 分厂名，是一个引用
	*@param sPlant_ 设备名，是一个引用
	*@param sAlias_ 是一个引用，用于把公司别名传出来
	*@param pFixComm_ 网络连接指针
	*@param bRtn_  是否返回网络连接信息
	*@return 如果验证成功，则返回1，否则返回NULL
	*/
	CPlantInfoExpertSys * ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);

	/**判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	*@param sCompany_ 公司名，是一个引用
	*@param sFactory_ 分厂名，是一个引用
	*@param sPlant_ 设备名，是一个引用
	*@param sAlias_ 是一个引用，用于把公司别名传出来
	*@param pFixComm_ 网络连接指针
	*@param bRtn_  是否返回网络连接信息
	*@return 如果验证成功，则返回1，否则返回NULL
	*/
	CPlantInfoExpertSys * CheckCompanyInfo(const string &sCompany_, const string &sFactory_, const string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);

	/**构造趋势表中的公共字段
	  *@return 字段串
	  */
	CString CImplement::ConsCommonField();

	/**构造指定振动测点在趋势表中的字段
	*@param sChann_      指定的测点名
	*@param bWave_       是否需要波形字段
	*@return 构造的测点特征值字段
	*/
	CString ConsVibChannField(string sChann_,bool bWave_=false);

	/**从当前数据库连接中读取指定振动测点的各种特征值
	*@param sChann_ 振动测点名
	*@param pDB_  数据库连接指针
	*@return 振动特征值
	*/
	S_VibCharValue ReadVibCharValue(string sChann_,CCustomDB *pDB_);

	S_VibCharValue ReadVibCharValue(string sChann_,IDBInterFace *pDB_);

	///判断通道的所属大类。及判断通道一级分类
	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);

	//获取表的前缀
	CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);



	/**根据设备信息，测点信息，起止时间，得到测点所在缸，给定通道类型的测点的报警状态
	*@param arefDB		  [in]	   数据库接口
	*@param arefFixComm	  [in]	   通信接口
	*@param apPlantInfo   [in]     设备信息指针
	*@param arefChanninfo [in]     给定测点的引用
	*@param aoletimeStart [in]     判断报警开始时间
	*@param aoletimeEnd	  [in]     判断报警结束时间
	*@param  aenumType    [in]     测点类型
	*@return 0-正常，1-异常，2-不确定， -1数据错误
	*/
	E_RC_AlarmStatus GetSpecChannTypeAlarmStatus(
			IDBInterFace						& arefDB,
			ICommInterFace						& arefFixComm,
			const CPlantInfoExpertSys			* apPlantInfo,
			const S_ChannInfo					& arefChanninfo,
			CBHDateTime						    & aoletimeStart,
			CBHDateTime						    & aoletimeEnd,
			ALARM_CHANN_TYPE aenumType = EXPERT_CHANN_TYPE_TEMP
		);


	/**根据设备信息，测点信息，起止时间，得到测点所在缸，各个监测点的传感器故障状态
	*@param arefDB		  [in]	   数据库接口
	*@param arefFixComm	  [in]	   通信接口
	*@param apPlantInfo   [in]     设备信息指针
	*@param arefChanninfo [in]     给定测点的引用
	*@param aoletimeStart [in]     判断故障开始时间
	*@param aoletimeEnd	  [in]     判断故障结束时间
	*@param  aenumType    [in]     测点类型
	*@return  0-否（传感器正常），1-是（传感器故障），2-不确定，-1数据错误
	*/
	E_BoolExType GetSpecChannTypeSensorStatus(
			IDBInterFace						& arefDB,
			ICommInterFace						& arefFixComm,
			const CPlantInfoExpertSys			* apPlantInfo,
			const S_ChannInfo					& arefChanninfo,
			CBHDateTime						    & aoletimeStart,
			CBHDateTime						    & aoletimeEnd,
			ALARM_CHANN_TYPE aenumType = EXPERT_CHANN_TYPE_TEMP
		);

	int GetTempValueTrend(
		IDBInterFace						& arefDB,
		ICommInterFace						& arefFixComm,
		std::vector<double>			        & aorefTrendData,
		const CPlantInfoExpertSys			* apPlantInfo,
		const S_ChannInfo					& arefChanninfo,
		COleDateTime						& aoletimeStart,
		COleDateTime						& aoletimeEnd,
		BOOL						        & abIsValveGasIn
		);
	
	/**根据设备信息，测点信息，起止时间，得到测点所在缸发生报警的温度通道的前后10分钟的数据，只取第一个发现的温度测点的报警数据
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param aorefTrendData [in/out] 趋势数据,点数需要不会过100个
	*@param adblThreshold  [in/out] 报警数据的阈值
	*@param apPlantInfo    [in]     设备信息指针
	*@param arefChanninfo  [in]     给定测点的引用
	*@param aoletimeStart  [in]     判断报警开始时间
	*@param aoletimeEnd	   [in]     判断报警结束时间
	*@param  aenumType     [in]     测点类型
	*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
	*/
	int GetSpecChannAlarmValueTempTrend(
			IDBInterFace				& arefDB,
			ICommInterFace				& arefFixComm,
			std::vector<double>			& aorefTrendData,
			double						& adblThreshold,
			const CPlantInfoExpertSys	* apPlantInfo,
			const S_ChannInfo			& arefChanninfo,
			CBHDateTime				    & aoletimeStart,
			CBHDateTime				    & aoletimeEnd,
			BOOL						&  abIsValveGasIn
		);

	/**根据设备信息,测点信息，得到测点所在缸的撞击测点的撞击次数
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param apPlantInfo    [in]		设备信息指针
	*@param arefChanninfo  [in]		给定测点的引用
	*@return >=0 撞击次数值，-1数据错误
	*/
	int GetSpecImpactTimes(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
			  CPlantInfoExpertSys		* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
			  unsigned int				& airefTimes_, 
			  unsigned int				& airefTh_,
			  CBHDateTime				& aoletimeStart,
			  CBHDateTime				& aoletimeEnd
		);

	/**得到诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数
	  *@param aiSensorFaults_  [in/out] 气阀传感器有故障的个数
	  *@param aiAlarmChannels_ [in/out] 没有传感器故障的气阀报警通道数
	  *@param aiChannels_      [in/out] 所在缸的监测气阀温度的通道数
	  */
	void GetCylinderValueAlarmsAndSensorFaults(
		 IDBInterFace					& arefDB,
		 ICommInterFace					& arefFixComm,
		 const CPlantInfoExpertSys		* apPlantInfo,
		 const S_ChannInfo				& arefChanninfo,
		 CBHDateTime					& aoletimeStart,
		 CBHDateTime					& aoletimeEnd,
		 unsigned int					& aiSensorFaults_, 		
		 unsigned int					& aiAlarmChannels_, 	
		 int							& aiChannels_);

	/**根据设备信息,测点信息，得到测点所在缸的报警前后各5s的沉降量趋势。 点数不需要超过50个。
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param aorefTrendData [in/out] 趋势数据,点数需要不会过50个
	*@param adblThreshold  [in/out] 沉降量的阈值，暂时不处理
	*@param apPlantInfo    [in]     设备信息指针
	*@param arefChanninfo  [in]     给定测点的引用
	*@param aoletimeStart  [in]     判断报警开始时间
	*@param aoletimeEnd	   [in]     判断报警结束时间
	*@param  aenumType     [in]     测点类型
	*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
	*/
	int GetSpecSinkTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		double							& adblThreshold,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& aRefFLoAlarmValue_, 
		double							& aRefFHiAlarmValue_, 
		double							& aRefFMaxRange_, 
		bool							& aRefBHiPass_
		);

	/**根据设备信息,测点信息，得到测点所在缸的沉降报警时刻的振动波形。。
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param aorefTrendData [in/out] 波形数据
	*@param anFs		   [in/out]	采样频率
	*@param apPlantInfo    [in]     设备信息指针
	*@param arefChanninfo  [in]     给定测点的引用
	*@param aoletimeStart  [in]     判断报警开始时间
	*@param aoletimeEnd	   [in]     判断报警结束时间
	*@param  aenumType     [in]     测点类型
	*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
	*/
	int GetSpecSinkWave(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefWaveData,
		int								& anFs,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& ldbThrethold
		);


	/**根据设备信息,测点信息，得到测点所在缸的报警前后各5s内的偏摆量趋势。 点数不需要超过50个。
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param aorefTrendData [in/out] 趋势数据,点数需要不会过50个
	*@param adblThreshold  [in/out] 偏摆量的阈值，暂时不处理
	*@param apPlantInfo    [in]     设备信息指针
	*@param arefChanninfo  [in]     给定测点的引用
	*@param aoletimeStart  [in]     判断报警开始时间
	*@param aoletimeEnd	   [in]     判断报警结束时间
	*@param  aenumType     [in]     测点类型
	*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
	*/
	int GetSpecDeflectTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		double							& adblThreshold,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& aRefFLoAlarmValue_, 
		double							& aRefFHiAlarmValue_, 
		double							& aRefFMaxRange_, 
		bool							& aRefBHiPass_
		);
	int GetSpecRodCharTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		E_ALARM_CHARTYPE                 aoCharType,
		GE_MEASURE_DIRECTION             aoMeasureDirection
		);

	/**根据设备信息,测点信息，得到测点所在缸的偏摆报警时刻的振动波形。。
	*@param arefDB		   [in]		数据库接口
	*@param arefFixComm	   [in]		通信接口
	*@param aorefTrendData [in/out] 波形数据,
	*@param anFs		   [in/out]	采样频率
	*@param apPlantInfo    [in]     设备信息指针
	*@param arefChanninfo  [in]     给定测点的引用
	*@param aoletimeStart  [in]     判断报警开始时间
	*@param aoletimeEnd	   [in]     判断报警结束时间
	*@param  aenumType     [in]     测点类型
	*@return  0-没有报警，>0 有报警数据，返回报警趋势数据的点数，等于aorefTrendData的size
	*/
	int GetSpecDeflectWave(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		int								& anFs,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& ldbThrethold
		);

	/**得到诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形
	  *报警前1小时的波形
	  *@param arefDB				[in]		数据库接口
	  *@param arefFixComm			[in]		通信接口
	  *@param apPlantInfo			[in]		设备信息指针
	  *@param arefChanninfo			[in]		给定测点的引用
	  *@param aorefTrendDataBefore	[in/out]	报警前的波形数据
	  *@param aiStartIndBeforeAlarm_[in/out]	报警前波形的一个周期的起始索引
	  *@param aiEndIndBeforeAlarm_  [in/out]	报警前波形的一个周期的结束索引
	  *@param aorefTrendDataAfter	[in/out]	报警后的波形数据
	  *@param aiStartIndAfterAlarm_ [in/out]	报警后波形的一个周期的起始索引
	  *@param aiEndIndAfterAlarm_	[in/out]	报警后波形一个周期的结束索引
	  *@param aiFsAfterAlarm_		[in/out]	报警后波形的采样频率
	  *@return 小于0表示获取失败
	  */
	int GetCrossHeadWave(
		IDBInterFace					&	arefDB,
		ICommInterFace					&	arefFixComm,
		CPlantInfoExpertSys				*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		std::vector<double>				&	aorefTrendDataBefore,
		int								&	aiStartIndBeforeAlarm_, 
		int								&	aiEndIndBeforeAlarm_, 
		std::vector<double>				&	aorefTrendDataAfter,
		int								&	aiStartIndAfterAlarm_, 
		int								&	aiEndIndAfterAlarm_, 
		int								&   aiFsAfterAlarm_,
		CBHDateTime					    &	aoletimeStart,
		CBHDateTime					    &	aoletimeEnd,
		double							&	fLoAlarmValue_, 
		double							&	fHiAlarmValue_);

	/**得到诊断测点所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形
	  *报警前1小时的波形
	  *@param arefDB				[in]		数据库接口
	  *@param arefFixComm			[in]		通信接口
	  *@param apPlantInfo			[in]		设备信息指针
	  *@param arefChanninfo			[in]		给定测点的引用
	  *@param aorefTrendDataBefore	[in/out]	报警前的波形数据
	  *@param aiStartIndBeforeAlarm_[in/out]	报警前波形的一个周期的起始索引
	  *@param aiEndIndBeforeAlarm_  [in/out]	报警前波形的一个周期的结束索引
	  *@param aorefTrendDataAfter	[in/out]	报警后的波形数据
	  *@param aiStartIndAfterAlarm_ [in/out]	报警后波形的一个周期的起始索引
	  *@param aiEndIndAfterAlarm_	[in/out]	报警后波形一个周期的结束索引
	  *@param aiFsAfterAlarm_		[in/out]	报警后波形的采样频率
	  *@return 小于0表示获取失败
	  */
	int GetCrankWave(
		IDBInterFace					&	arefDB,
		ICommInterFace					&	arefFixComm,
		CPlantInfoExpertSys				*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		std::vector<double>				&	aorefTrendDataBefore,
		int								&	aiStartIndBeforeAlarm_, 
		int								&	aiEndIndBeforeAlarm_, 
		std::vector<double>				&	aorefTrendDataAfter,
		int								&	aiStartIndAfterAlarm_, 
		int								&	aiEndIndAfterAlarm_, 
		int								&   aiFsAfterAlarm_,
		CBHDateTime					&	aoletimeStart,
		CBHDateTime					&	aoletimeEnd,
		double							&	fLoAlarmValue_, 
		double							&	fHiAlarmValue_);


	int GetCrankTrendData(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					& aoletimeStart,
		CBHDateTime					& aoletimeEnd,
		E_ALARM_CHARTYPE                aoCharType);
	/**根据设备信息,测点信息，得到测点所在缸的机组一级转速，如果是温度测点，就取同缸的振动和动态测点的转速
	*如果是振动/动态测点，直接返回测点的转速
	*@param apPlantInfo   设备信息指针
	*@param arefChanninfo 给定测点的引用
	*@param IDBInterFace  数据库指针的引用
	*@return >=0 转速值，-1数据错误
	*/
	int GetSpecSectionRev(
		const CPlantInfoExpertSys		*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		IDBInterFace					&	arefDB,
		CBHDateTime					&	aoletimeStart,
		CBHDateTime					&	aoletimeEnd);

	//获取异常数据点的转速
	int GetRevOfAbnormalData(
		const CPlantInfoExpertSys		*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		IDBInterFace					&	arefDB,
		COleDateTime					&	aoletimeStart,
		COleDateTime					&	aoletimeEnd);



	/**根据趋势数据，已经常规报警设置，判断高低报警限，以及是否高报
	*如果是振动/动态测点，直接返回测点的转速
	*@param aoRefTrend						趋势向量的引用
	*@param aRefChannInfo					测点的引用
	*@param [out] adblRefAlarmHValue		报警高值的引用
	*@param [out] adblRefAlarmLValue		报警低值的引用
	*@return								返回True表示高通 false 表示低通
	*/
	BOOL ParseTrendAlarmHighLowValue(
		std::vector<double>			    &	aorefTrendData,
		const S_ChannInfo				&	anRefChannInfo,
		double					        &	aRefFHiAlarmValue_,
		double				            &	aRefFLoAlarmValue_);


	CHANNTYPE TranlateChannType(ALARM_CHANN_TYPE aenumChannType);

	int GetSpecTypeChanns(const CPlantInfoExpertSys	* apPlantInfo,ALARM_CHANN_TYPE arChannType, vector<S_ChannInfo> &arChannInfo);

	int readfile(double * ag_fwave,int len_,CString strFile_);
};


#endif
