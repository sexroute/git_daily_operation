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

	//�õ�����������ڸ��ڸ�����ֹʱ��η����������¶Ȳ���sql
	static BOOL GetSqlForSpecChannAlarmChannByType( IDBInterFace & arefDB,
													const CPlantInfoExpertSys * apPlantInfo, 
													const S_ChannInfo & arefChanninfo, 
													CHANNTYPE anChannType, 
													CBHDateTime & arefTimeStart, 
													CBHDateTime & arefTimeEnd, 
													CString & acstrSql,
													GE_MEASURE_DIRECTION aiDirection = MEASURE_DIRECTION_ALL,//1��ʾ��ֱ��0��ʾˮƽ
													BOOL abAllChann = FALSE);



	//�õ�����������ڸ�ײ����������sql
	static BOOL GetSqlForSetImpactTimes(
		IDBInterFace					& arefDB,
		CPlantInfoExpertSys				* apPlantInfo,	
		const S_ChannInfo				& arefChanninfo,
		CHANNTYPE						  anChannType,
		CBHDateTime					& arefTimeStart,
		CBHDateTime					& arefTimeEnd,	
		CString							& acstrSql);



	/*  ����ʱ���ȡ������
	 *	���ݵ�ǰʱ����Ҫ��ʱ������֮��ļ��
	 *	@param[in] time_
	 *  return ������ �£��գ�ʱ����
	 */
	static E_TREND_TBL_TYPE SqlHelperForExpert::GetTrendTblType(CBHDateTime  time_);


	// ͨ��������͡�ʱ�����õ����Ǹ����ݿ������ǰ�벿��.���л�����ݺʹӼ�����¼��ȡһ����¼�ĸ�����
	static CString GetTblNamePrefixForTrend2(const CHANNTYPE &iChannType_,
											CBHDateTime timeStart_,
											CBHDateTime timeEnd_,
											E_TREND_TBL_TYPE& iTblType_,
											bool & bSubTbl_,
											bool &bSupTbl_);

	// ͨ��������͡�ʱ�����õ����Ǹ����ݿ������ǰ�벿��.���л�����ݺʹӼ�����¼��ȡһ����¼�ĸ�����
	static CString GetTblNamePrefixForTrend2Alarm(const CHANNTYPE &iChannType_,
		CBHDateTime timeStart_,
		CBHDateTime timeEnd_,
		E_TREND_TBL_TYPE& iTblType_,
		bool & bSubTbl_,
		bool &bSupTbl_);

	//��ȡ���ǰ׺
	static CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

	//��ȡ�������ǰ׺
	static CString GetTblNamePrefixForAlarm(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

	///�ж�ͨ�����������ࡣ���ж�ͨ��һ������
	static E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);


	/**��ö�ȡ���Ƶļ�¼
	  *@param pDB ���ݿ�����ָ��
	  *@param sTblNameSuffix_ ���ݿ�����׺
	  *@param sShowField_  ��Ҫ�������ֶ�
	  *@param iChannType_  �������
	  *@param iTblType_    ���Ʊ�����
	  *@param timeStart_   ��ʼʱ�䡣���÷�ʽ�������Ҫ��һ�������ʱ���Ϊ�µĿ�ʼʱ��
	  *@param timeEnd_     ����ʱ��
	  *@param bNeedSubTbl_ ���÷�ʽ�����ݳ��Ƿ���Ҫ�¼�����
	  *@return ���Ϊfalse��˵��û�м�¼�ɶ�������Ϳ��Զ���¼
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

	//ȡ��������ʱ�ı���
	static CString GetTblNamePrefixForWave2(const CHANNTYPE &iChannType_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_);
	static CString GetTblNamePrefixForWave2ForAlarm(const CHANNTYPE &iChannType_, CBHDateTime timeEnd_,E_TREND_TBL_TYPE& iTblType_);


	/**����ָ���񶯲�������Ʊ��е��ֶ�
	  *@param sChann_ ָ���Ĳ����
	  *@param bWave_ �Ƿ���Ҫ�����ֶ�
	  *@return ����Ĳ������ֵ�ֶ�
	  */
	static CString ConsChannField(string sChann_,CHANNTYPE anType,bool bWave_=false);
	
	static CString ConsDynChannField(string sChann_,bool bWave_=false);

	static CString ConsVibChannField(string sChann_,bool bWave_=false);

	static CHAN_CAT GetCharCat(CHANNTYPE anChannType);
	

		/**�������Ʊ��еĹ����ֶ�
	  *@return �ֶδ�
	  */
	static CString ConsCommonField();

	static S_DynCharValue ReadDynCharValue(string sChann_,IDBInterFace *pDB_);

	static S_VibCharValue ReadVibCharValue(string sChann_,IDBInterFace *pDB_);

};