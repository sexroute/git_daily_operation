#include "StdAfx.h"
#include "Implement.h"
#include <additions/DebugHelper.h>

class SqlHelperForExpert
{
private:
	SqlHelperForExpert(){};

public:
	enum CHAN_CAT
	{
		CHAN_VIB = 0,
		CHAN_DYN = 1,
		CHAN_PROC =2,
	};

	static BOOL GetSqlForGetSpecChannTypeAlarmStatus(	
		const CPlantInfoExpertSys		* apPlantInfo,	
		const S_ChannInfo				& arefChanninfo,
		CHANNTYPE						  anChannType,
		CBHDateTime					& arefTimeStart,
		CBHDateTime					& arefTimeEnd,	
		CString							& acstrSql,
		CImplement::ALARM_CHANN_TYPE	  aenumType,
		GE_MEASURE_DIRECTION			  aenumDirection =MEASURE_DIRECTION_ALL
		);

	static BOOL GetSqlForSpecChannTypeSensorStatus(	
		const CPlantInfoExpertSys		* apPlantInfo,	
		const S_ChannInfo				& arefChanninfo,
		CHANNTYPE						  anChannType,
		CBHDateTime					& arefTimeStart,
		CBHDateTime					& arefTimeEnd,	
		CString							& acstrSql,
		CImplement::ALARM_CHANN_TYPE	  aenumType);

	//得到给定测点所在缸在给定起止时间段发生报警的温度测点的sql
	static BOOL GetSqlForSpecChannAlarmChannByType( IDBInterFace & arefDB,
													const CPlantInfoExpertSys * apPlantInfo, 
													const S_ChannInfo & arefChanninfo, 
													CHANNTYPE anChannType, 
													CBHDateTime & arefTimeStart, 
													CBHDateTime & arefTimeEnd, 
													CString & acstrSql,
													GE_MEASURE_DIRECTION aiDirection = MEASURE_DIRECTION_ALL,//1表示垂直，0表示水平
													BOOL abAllChann = FALSE);



	//得到给定测点所在缸撞击次数测点的sql
	static BOOL GetSqlForSetImpactTimes(
		IDBInterFace					& arefDB,
		CPlantInfoExpertSys				* apPlantInfo,	
		const S_ChannInfo				& arefChanninfo,
		CHANNTYPE						  anChannType,
		CBHDateTime					& arefTimeStart,
		CBHDateTime					& arefTimeEnd,	
		CString							& acstrSql);



	/*  根据时间获取表类型
	 *	根据当前时间与要求时间两者之间的间隔
	 *	@param[in] time_
	 *  return 表类型 月，日，时，秒
	 */
	static E_TREND_TBL_TYPE SqlHelperForExpert::GetTrendTblType(CBHDateTime  time_);


	// 通过测点类型、时间间隔得到从那个数据库表名的前半部分.单中获得数据和从几条记录中取一条记录的个数。
	static CString GetTblNamePrefixForTrend2(const CHANNTYPE &iChannType_,
											CBHDateTime timeStart_,
											CBHDateTime timeEnd_,
											E_TREND_TBL_TYPE& iTblType_,
											bool & bSubTbl_,
											bool &bSupTbl_);

	// 通过测点类型、时间间隔得到从那个数据库表名的前半部分.单中获得数据和从几条记录中取一条记录的个数。
	static CString GetTblNamePrefixForTrend2Alarm(const CHANNTYPE &iChannType_,
		CBHDateTime timeStart_,
		CBHDateTime timeEnd_,
		E_TREND_TBL_TYPE& iTblType_,
		bool & bSubTbl_,
		bool &bSupTbl_);

	//获取表的前缀
	static CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

	//获取报警表的前缀
	static CString GetTblNamePrefixForAlarm(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

	///判断通道的所属大类。及判断通道一级分类
	static E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);


	/**获得读取趋势的记录
	  *@param pDB 数据库联接指针
	  *@param sTblNameSuffix_ 数据库名后缀
	  *@param sShowField_  需要的数据字段
	  *@param iChannType_  测点类型
	  *@param iTblType_    趋势表类型
	  *@param timeStart_   开始时间。引用方式，如果需要下一级表，这个时间就为新的开始时间
	  *@param timeEnd_     结束时间
	  *@param bNeedSubTbl_ 引用方式，传递出是否需要下级数据
	  *@return 如果为false，说明没有记录可读，否则就可以读记录
	  */
	static bool GetTrendRecords(IDBInterFace *pDB,
								string sTblNameSuffix_,
								string sPlantDbName,
								string sShowField_,
								const CHANNTYPE &iChannType_,
								E_TREND_TBL_TYPE &iTblType_,
								CBHDateTime& timeStart_,
								CBHDateTime timeEnd_,
								bool & bNeedSubTbl_);

	static bool GetTrendRecordsForAlarm(IDBInterFace *pDB,
										string sTblNameSuffix_,
										string sPlantDbName,
										string sShowField_,
										const CHANNTYPE &iChannType_,
										E_TREND_TBL_TYPE &iTblType_,
										CBHDateTime& timeStart_,
										CBHDateTime timeEnd_,
										bool & bNeedSubTbl_);

	static CString GetMainDBName();

	static CString GetSpecPlantDBName(CPlantInfoBase & arefPlantInfo);

	static CString GetSpecPlantTableSuffix(CPlantInfoBase & arefPlantInfo);


	static int ReadVibChannSpecTimeWaveAndSpectrum( 
													IDBInterFace *pDB_,
													CPlantInfoExpertSys * apPlantInfo, 
													std::vector<double>			& aorefTrendData,
													string						sCompany_,
													string						sFactory_,
													string						sPlant_,
													string						sChann_,
													int							iLines_,
													int							iWaveNum_,
													CBHDateTime				timeSpec_,
													int							& aifreq,
													BOOL						abShouldReadSpectrum = FALSE);

	static int ReadVibChannSpecTimeWaveAndSpectrumForAlarm( 
													IDBInterFace *pDB_,
													CPlantInfoExpertSys * apPlantInfo, 
													std::vector<double>			& aorefTrendData,
													string						sCompany_,
													string						sFactory_,
													string						sPlant_,
													string						sChann_,
													int							iLines_,
													int							iWaveNum_,
													CBHDateTime				timeSpec_,
													int							& aifreq,
													BOOL						abShouldReadSpectrum = FALSE);

	static int ReadDynChannSpecTimeWaveAndSpectrum( 
		IDBInterFace *pDB_,
		CPlantInfoExpertSys * apPlantInfo, 
		std::vector<double>			& aorefTrendData,
		string						sCompany_,
		string						sFactory_,
		string						sPlant_,
		string						sChann_,
		int							iLines_,
		int							iWaveNum_,
		CBHDateTime				timeSpec_,
		int							& aifreq,
		BOOL						abShouldReadSpectrum = FALSE);

	static int ReadDynChannSpecTimeWaveAndSpectrumForAlarm( 
		IDBInterFace *pDB_,
		CPlantInfoExpertSys * apPlantInfo, 
		std::vector<double>			& aorefTrendData,
		string						sCompany_,
		string						sFactory_,
		string						sPlant_,
		string						sChann_,
		int							iLines_,
		int							iWaveNum_,
		CBHDateTime				timeSpec_,
		int							& aifreq,
		BOOL						abShouldReadSpectrum = FALSE);



	BOOL GetCylinderValueAlarmsAndSensorFaults(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		const CPlantInfoExpertSys		* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					& aoletimeStart,
		CBHDateTime					& aoletimeEnd,
		unsigned int					& aiSensorFaults_, 		
		unsigned int					& aiAlarmChannels_, 	
		int								& aiChannels_,
		CString							& astrSql);

	//取波形数据时的表名
	static CString GetTblNamePrefixForWave2(const CHANNTYPE &iChannType_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_);
	static CString GetTblNamePrefixForWave2ForAlarm(const CHANNTYPE &iChannType_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_);


	/**构造指定振动测点在趋势表中的字段
	  *@param sChann_ 指定的测点名
	  *@param bWave_ 是否需要波形字段
	  *@return 构造的测点特征值字段
	  */
	static CString ConsChannField(string sChann_,CHANNTYPE anType,bool bWave_=false);
	
	static CString ConsDynChannField(string sChann_,bool bWave_=false);

	static CString ConsVibChannField(string sChann_,bool bWave_=false);

	static CHAN_CAT GetCharCat(CHANNTYPE anChannType);
	

		/**构造趋势表中的公共字段
	  *@return 字段串
	  */
	static CString ConsCommonField();

	static S_DynCharValue ReadDynCharValue(string sChann_,IDBInterFace *pDB_);

	static S_VibCharValue ReadVibCharValue(string sChann_,IDBInterFace *pDB_);

};