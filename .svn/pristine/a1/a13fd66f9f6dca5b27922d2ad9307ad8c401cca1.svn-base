#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include <string>
using std::string;

#include <PlantInfoBase.h>

#include <KDCommlib.h>

/* 数采状态文件格式*/
#define DAQ_HW_STATUS_FILE   "DAQ_HW_Status.json"
#define DAQ_SW_STATUS_FILE   "DAQ_SW_Status.json"

#define DAQ_HW_STATUS_FILE_DESCRIPTION   "DAQ Hardware Status Record"
#define DAQ_SW_STATUS_FILE_DESCRIPTION   "DAQ Software Status Record"

#define FILE_DESCRIPTION        "description"
#define DAQ_LAST_UPDATE_TIME    "daqLastUpdateTime"
#define FILE_MODIFIED_TIME      "fileModifiedTime"

#define DAQ_HW_STATUS_CARD_INFO          "cardInfo"
#define DAQ_HW_STATUS_CARD_COUNT         "cardCount"
#define DAQ_HW_STATUS_CARD_INDEX         "cardIndex"
#define DAQ_HW_STATUS_CARD_NAME          "cardName"
#define DAQ_HW_STATUS_USED_AI_CHANNELS   "usedAIChannels"
#define DAQ_HW_STATUS_USED_AI_CHANN_NUM  "usedAIChannNum"
#define DAQ_HW_STATUS_USED_CI_CHANNELS   "usedCIChannels"
#define DAQ_HW_STATUS_USED_CI_CHANN_NUM  "usedCIChannNum"
#define DAQ_HW_STATUS_STATUS             "status"

#define DAQ_SW_STATUS_VERSION            "version"
#define DAQ_SW_STATUS_ACQUIRE_TYPE       "acquireType"
#define DAQ_SW_STATUS_APP_START_TIME     "startTime"
#define DAQ_SW_STATUS_RCV_VIB_TIME       "rcvVibTime"
#define DAQ_SW_STATUS_RCV_DYN_TIME       "rcvDynbTime"
#define DAQ_SW_STATUS_RCV_PROC_TIME      "rcvProcTime"
#define DAQ_SW_STATUS_RCV_GAP_STATUS     "rcvGAPStatus"
#define DAQ_SW_STATUS_LATEST_OFFLINE_SEND_BEGIN      "offlineSendBegin"
#define DAQ_SW_STATUS_LATEST_OFFLINE_SEND_END        "offlineSendEnd"

#define G_UPDATEMONTHTABLE              0        //是否更新保存月表
#define G_FIXCOMM_TIMEOUT               120      //FIXCOMM超时时间

#define D_TREND_DATA_LIMIT   30000

int InitJsonStautsFile(
	const CString & statusFile);


/* 初始化数采硬件配置文件*/
int verifyDAQHWStautsFile();

/* 初始化数采软件配置文件*/
int verifyDAQSWStautsFile();

int SetFieldWithStatus(ICommInterFace * pFixComm,
                           const int field,
                           const int status);

int updateDAQSWStautsFile(const char * field,
	                              const char * value);

/* 守护系统状态文件 */
#define DAEMON_STATUS_FILE   "SystemState.json"

#define DAEMON_STATUS_BASIC_INFO        "BasicInfo"
#define DAEMON_STATUS_SOFTWARE_VERSION  "SoftVersion"
#define DAEMON_STATUS_FILE_PATH         "POSITION"

#define DAEMON_STATUS_CONFIG_INFO          "ConfigInfo"
#define DAEMON_STATUS_CONFIG_ALARM         "AlarmConfig"
#define DAEMON_STATUS_CONFIG_CONNECTED_DB  "ConnectDataBase"
#define DAEMON_STATUS_CONFIG_DATA_SAVE     "DataSaveConfig"
#define DAEMON_STATUS_CONFIG_DATA_SYNC     "DataSynConfig"
#define DAEMON_STATUS_CONFIG_SERVER_TYPE   "SvrType"

#define DAEMON_STATUS_DB_INFO           "DBInfo"
#define DAEMON_STATUS_DB_STORING_INFO   "DBStoringInfo"

#define DAEMON_STATUS_ALARM_DATA_ESTIMATE_SVAE_TIME         "AlarmDataSavingTime"
#define DAEMON_STATUS_ALARM_TREND_ESTIMATE_SVAE_TIME        "DataSavingTime"
#define DAEMON_STATUS_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME   "TrendDataSavingTime"

#define DAEMON_STATUS_DB_DISK_INFO   "DiskInfo"
#define DAEMON_STATUS_DB_NUM   "DBNum"

#define DAEMON_STATUS_DB_MAIN   "MainDB"
#define DAEMON_STATUS_DB_SUB    "SubDB"

#define DAEMON_STATUS_STATISTIC_INFO          "StatisticInfo"
#define DAEMON_STATUS_STATISTIC_COMPANY_NUM   "CompanyNum"
#define DAEMON_STATUS_STATISTIC_CHANN_NUM     "ChannelNum"
#define DAEMON_STATUS_STATISTIC_PLANT_NUM     "PlantNum"

#define DAEMON_STATUS_DB_DATA_FILE_NAME   "DBDataFileName"
#define DAEMON_STATUS_DB_DATA_FILE_SIZE   "DBDataFileSize"
#define DAEMON_STATUS_DB_LOG_FILE_NAME    "DBLogFileName"
#define DAEMON_STATUS_DB_LOG_FILE_SIZE    "DBLogFileSize"
#define DAEMON_STATUS_DB_NAME             "DBName"
#define DAEMON_STATUS_DB_TYPE             "DBType"
#define DAEMON_STATUS_FREE_DISK_SPACE     "LeftDiskSpace"
#define DAEMON_STATUS_FREE_DISK_SPACE_RATIO     "LeftDiskSpaceRate"
#define DAEMON_STATUS_FAST_INC_TABLE            "TheFastIncTableName"
#define DAEMON_STATUS_TOTAL_DISK_SPACE          "TotalDiskSpace"

#define DAEMON_STATUS_MW_STATUS              "MiddleWareState"
#define DAEMON_STATUS_MW_CONNECTION_STATUS   "MiddleWareConnectState"
#define DAEMON_STATUS_MW_DATA_RECEIVING_STATUS "ReceivingDataState"
#define DAEMON_STATUS_MW_DATA_SAVING_STATUS    "SavingDataState"
#define DAEMON_STATUS_MW_DATA_SENDING_STATUS   "SendingDataState"

#define DAEMON_STATUS_MW_COMPANY   "Company"
#define DAEMON_STATUS_MW_FACTORY   "Factory"

#define DAEMON_STATUS_MW_PLANT     "Plant"

#define DAEMON_STATUS_MW_VIBDATA   "VibData"
#define DAEMON_STATUS_MW_DYNDATA   "DynData"
#define DAEMON_STATUS_MW_PRCODATA  "ProcData"

#define DAEMON_STATUS_MW_ALARAMDATA  "AlarmData"
#define DAEMON_STATUS_MW_TRENDDATA   "TrendData"
#define DAEMON_STATUS_MW_SUDDATA     "SudData"


#define AD_MAX_INT  (1073741824)
#define AD_MIX_INT  (-1073741824)
#define AD_PRECISION_FLOAT   (0.0001)
#define PLANT_REV_NUM 5

#define  FILE_PATH_LEN  256

string AbnormalData_GetLastError();
int   AbnormalData_IsRevValid(
		float * pfLastRev, float * pfCurrRev, int nRevNum,
		CPlantInfoBase  *  pPlant);
int AbnormalData_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);
int AbnormalData_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);
int AbnormalData_IsCharValid(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant);

bool DB_IsColumnInTable(CCustomDB *pDB_, const string &sColumnName_, const string &sTblName_);
bool DB_IsColumnInTable(IDBInterFace *pDB_, const string &sColumnName_, const string &sTblName_);
void SetNullReturnFixcomm(CFixComm * pFixComm );
void SetNullReturnFixcommEx(ICommInterFace * pFixComm );
/*!
 如果测点对应的采样频率，采样点数，转速为NULL， 取公共的采样频率，采样点数，转速
 */
int SimADO_GetRev( CCustomDB * pDB_, string sChannNo_, int iRevType_ );
int SimADO_GetRev( CCustomDB * pDB_, string sChannNo_ );
int SimADO_GetRev( CCustomDB * pDB_, const S_ChannInfo &channInfo );
int SimADO_GetSampleNum( CCustomDB * pDB_, string sChannNo_ );
int SimADO_GetSampleNum( CCustomDB * pDB_, const S_ChannInfo &channInfo );
int SimADO_GetSampleFrequency( CCustomDB * pDB_, string sChannNo_ );
int SimADO_GetSampleFrequency( CCustomDB * pDB_, const S_ChannInfo &channInfo );


int SimADO_GetRevEx( IDBInterFace * pDB_, string sChannNo_ );
int SimADO_GetRevEx( IDBInterFace * pDB_, string sChannNo_, int iRevType_ );
int SimADO_GetRevEx( IDBInterFace * pDB_, const S_ChannInfo & channInfo );

int SimADO_GetSampleNumEx( IDBInterFace * pDB_, string sChannNo_ );
int SimADO_GetSampleNumEx( IDBInterFace * pDB_, const S_ChannInfo & channInfo );

int SimADO_GetSampleFrequencyEx( IDBInterFace * pDB_, string sChannNo_ );
int SimADO_GetSampleFrequencyEx( IDBInterFace * pDB_, const S_ChannInfo & channInfo );

int AlarmStatus_ParseAlarmStatus(unsigned int iState_);

CString SimAdo_Expression_ISNULL( const CString & sColumn, int nDefault);
CString SimAdo_Expression_ISNULL( const CString & sColumn, const CString & sDefault);

class CMutexRWReader
{
public:
	explicit CMutexRWReader(IReadWriteLock & aMutexRW);
	explicit CMutexRWReader(IReadWriteLock * pMutexRW);

	virtual ~CMutexRWReader();

public:
	virtual bool Accquire();
	virtual bool Release();

private:
	IReadWriteLock * m_pMutexRW;

	CMutexRWReader();
	CMutexRWReader(const CMutexRWReader &);
	CMutexRWReader & operator=(const CMutexRWReader &);

};

class CMutexRWWriter
{
public:
	explicit CMutexRWWriter(IReadWriteLock & aMutexRW);
	explicit CMutexRWWriter(IReadWriteLock * pMutexRW);

	virtual ~CMutexRWWriter();

public:
	virtual bool Accquire();
	virtual bool Release();

private:
	IReadWriteLock * m_pMutexRW;

	CMutexRWWriter();
	CMutexRWWriter(const CMutexRWWriter &);
	CMutexRWWriter & operator=(const CMutexRWWriter &);
};

namespace DBUtils
{
	CString ComposeDataTableName(const TCHAR * table_name, const TCHAR * suffix);
}

typedef std::vector<S_CharInfo> CharInfoList_t;

namespace CharInfoHelper
{
	int Initialize(CSimADO *pDB);
	int Initialize( COleSqlServerDB& aDB );

	const CharInfoList_t& GetCharInfoList(int aInfoType);

	std::string GetCharUnit(int aInfoType, int aCharIndex);
}

extern std::map<CString,CString> goMapFireSequece;
extern std::map<CString,CString> goMapTDCPhase;
extern std::map<CString,CString> goMapFireAdvancedAnglePhase;
extern std::map<CString,CString> goMapValveOpenPhase;
extern std::map<CString,CString> goMapValveClosePhase;

class CConfigHelper
{
public:
	CConfigHelper();
	~CConfigHelper();
public:
	static int GetMonthTableUpdateFlag();
	static int GetFixCommTimeOut();

	static INT GetSplitStrArray(const CString& strData,const CString& strSplitFlag,CStringArray &array,BOOL SequenceAsOne);
	
	static bool GetAllPlantFireInfo();

	///获取柴油机设备类型
	static CString GetDieselPlantType(CString strPlantNo_);

	static int GetAlarmCount();

	static float GetAlarmRatio( CString strChannNo_ );

	static float GetPowerRatio(CString strPlantNo_);

	static float GetPowerSpliteFactor(CString strPlantNo_);

	static int GetLowPressRevIndex();

	static int GetHighPressRevIndex();

	static int GetPowerIndex(CString strPlantNo_);

	static bool GetDiagnoseInterface( CString& strIP_,int& nPort_ );

	static int UseAIAlarm();
};


#endif
