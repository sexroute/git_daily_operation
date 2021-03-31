/**全局变量声明
  *
  */

#include "StdAfx.h"

#include "PlantInfo.h"
#include "AlarmPlantInfo.h"
#include "PlantPool.h"
#include "KDCommlib.h"
#include "additions/BHOleDbWrapper.h"
#include "../ServerStatus/IServerStatusExport.h"

extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;

#if !defined(AFX_ONLINEGLOBAL_INCLUDED_MARBLE_)
#define AFX_ONLINEGLOBAL_INCLUDED_MARBLE_
#include <fstream>


//#define _DEBUGMB

//#define D_DAQ_SUD_POINTNUM 2048

//#define D_SENDWAVENUM      513    默认向客户端发送的波形数据个数
//#define D_WAVEFORFREQ      1024   1024是因为默认的频谱分析是400线，所以对应的被分析的波形点数是1024

//判断缓变报警累计的次数  120
#define D_ALARM_COUNTS 120

//在判断缓变报警累计次数中，超过这个数目就认为报警 100
#define D_ALARM_ALARM_COUNTS  100

//缓变门限学习的时间间隔，单位：2小时
#define D_ALARM_STH_LEARN_INTV 2

//设定缓变门限学习的数据长度 1200
#define D_ALARM_STH_LEARNDATA_LEN 1200

//报警设备信息中,缓变报警点之前保存的波形组数 120
#define D_ALARM_SAVEWAVE_NUM  120

//报警设备信息中,每个振动通道需要缓存的特征值组数 1200
#define D_ALARM_SAVECHAR_NUM  1200

//对于缓变报警，连续保存的报警数据时间，单位：2分钟
#define D_ALARM_SLOW_CONTSAVETIME   2
//对于缓变报警，连续保存报警数据结束后，间断保存的报警数据时间，单位：8分钟
#define D_ALARM_SLOW_INTVSAVETIME   8

//一次缓变报警结束后，触发自学习的时间间隔，并在该期间内，不判断缓变报警，单位：30分钟
#define D_ALARM_SLOW_ENDTIME   30

//判断缓变报警相似性自学习保存阈值的时间间隔，超过两天则不判断相似性，单位：小时
#define D_ALARM_SLOW_JUDGESIMILAR_SAVETHTIME   48

//间断保存报警数据期间的间隔大小，单位：秒
#define D_ALARM_INTVSAVETIME_INTV   30

/*! 报警值上次和当前报警值比较的上限，如果超过则重新计算当前保存报警记录的条数。*/
#define D_FLUCTUATE_RANGE   1.20

//判断报警相似性的时间间隔，超过两天则不判断相似性，单位：48小时
#define D_ALARM_JUDGESIMILARTIME   48
//在报警相似性范围内，保存报警数据的时间间隔，间隔2小时保存一条报警数据，单位：2小时
#define D_ALARM_INTERVALSAVETIME   2

//对于常规报警，连续保存的报警数据时间，单位：5分钟
#define D_ALARM_CONTSAVETIME   5
//对于常规报警，连续保存报警数据结束后，间断保存的报警数据时间，单位：20分钟
#define D_ALARM_INTVSAVETIME   20

//对于常规报警,在两天之内作重复报警判断操作，单位：2天
#define D_ALARM_REG_DAYTIME   2

//对于常规报警,在报警限附近波动，重复报警时，在两天之内，当此次报警时特征值比上一次报警时的特征值小时，不添加新的报警日志，间隔30分钟保存一条上一次报警ID的报警数据，单位：30分钟
#define D_ALARM_REG_INTVTIME   30

/*!秒趋势（当前趋势）表单中，保存记录的最大条数。1秒钟保存一次，所以小时趋势表单中总共保存7*24+3(171)小时的记录*/
//#define D_TREND_SECONDRANGE   615600
/*!秒趋势（当前趋势）表单中，保存记录的最大条数。3秒钟保存一次，所以小时趋势表单中总共保存7*24+3(171)小时的记录,共10260分钟.*/
#define D_TREND_SECONDRANGE   (10260*GE_SECONDTREND_PERMINUTE_SAVENUM)

/*!小时趋势表单中，保存记录的最大条数。一分钟保存一次，一天保存1440条，所以小时趋势表单中总共保存3个月+3天（93天）的记录*/
#define D_TREND_HOURRANGE   133920

/*!天趋势表单中，保存记录的最大条数。记录为一天为72条记录（即20分钟一个点），总共保存77832条记录。即3*360+1天的记录*/
#define D_TREND_DAYRANGE    77832

/*!天趋势表单中，保存记录的最大条数。*/
#define D_TREND_FASTRANGE    (10260*GE_SECONDTREND_PERMINUTE_SAVENUM)

/*!启停车表单中，保存记录的最大条数。*/
#define D_SUD_DATARANGE      200000

#define G_USENEWALARMLOGIC  1						    //是否使用新报警逻辑							默认1
#define G_NNEWALARMMAXTIME  2					        //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
#define G_NSAVEINDENSETIME  5						    //密集保存时间间隔,								默认5分钟
#define G_NSAVEHALFINDENSETIME  30					    //半密集保存时间间隔,							默认30秒
#define G_NHALFNORMALNOTSAVETIME  20				    //半正常状态下持续保存的时间长度，				默认20分钟
#define G_NSAVESPARSETIME 30						    //就报警每30分钟保存一组的时间间隔,				默认30分钟
#define G_NHALFNORMALTIME 2 						    //半正常状态维持时间间隔						默认2个小时
#define G_NCHANNHALFNORMALTIME 1					    //测点半正常状态维持时间间隔					默认1个小时       
#define G_USENEW_DC_ALARMLOGIC 0					    //是否使用新的直流量报警逻辑					默认0
#define G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC 120 //为新的直流量报警逻辑保存数据的时间间隔		默认120分钟
#define G_WAVE_DIFF_RATIO 80							//配置波形相似度的值							默认80%
#define G_ALARM_LARGER_TIMES 10							//连续超过上次报警的次数					    默认3
#define ALARM_THRESHOLD      15							//超过上次报警限制的阈值						默认15
#define G_REV_THRESHOLD      15							//开停车判断判定开停车最小转速阈值				默认15
#define G_REV_THRESHOLD_ROD_SINK      250				//用来断判定波形相似度的最小转速阈值，          默认为250	
#define G_USENEW_AXIS_ALARMLOGIC 0                      //是否使用新的轴位移报警逻辑，                  默认0不使用
#define G_SAVE_FAST_AND_REGULAR_ALARM_DATA     1        //是否同时保存快变报警数据和常规报警数据        默认1
#define G_SAVEFASTDATATOSECONDTABLE		1			//快变是否保存到秒表    默认是
#define G_SAVEFASTDATATOHOURTABLE		0		//快变是否保存到小时表    默认否
#define G_SAVEFASTDATATODAYTABLE		0		//快变是否保存到天表    默认否
#define G_SAVEFASTDATATOMONTHTABLE		0		//快变是否保存到月表    默认否
#define G_SAVEDATAONSTOP				1        //非临时在线停车状态下是否保存日常数据表中的秒表，小时表，天表数据
#define G_SAVEMONTHDATAONSTOP		 1			//非临时在线模式下停车状态下是否保存日常数据表中的月表数据
#define G_SAVEALLFASTDATA               1        //是否保存所有快变数据
#define G_SAVEALLFASTDATAWAVE           1        //是否保存所有快变数据
#define G_SAVEFASTDATAWAVEINTERVAL      10       //保存波形数据间隔
#define G_SAVEUSEOPTIMIZEMODE           0        //使用优化模式存储数据
#define G_UPDATE_PROCDATA_OVERTIME             5        //过程量测点数据更新超时时间                    默认5分钟
#define G_UPDATE_PLANTDATA_OVERTIME            30       //设备数据更新超时时间                          默认30分钟


/*!秒趋势（当前趋势）表单中，保存记录的最大条数。根据宏和配置文件来取*/
extern int G_D_TREND_SECONDRANGE;

/*!小时趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
extern int G_D_TREND_HOURRANGE;

/*!天趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
extern int G_D_TREND_DAYRANGE;

/*!快变趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
extern int G_D_TREND_FASTRANGE;

/*!启停车表单中，保存记录的最大条数。根据宏和配置文件来取*/
extern int G_D_SUD_DATARANGE;

/*!是否开启停车仅保存月表策略，默认为0,即不开启*/
extern int G_TEMP_ONLINE;

/*!最小采样点数，小于此采样点数则中间件不处理*/
extern int G_SAMPLE_LIMITATION ; 

/*!报警判断判定区间超限阈值百分比为整数*/
extern int G_ALARM_THRESHOLD;

/*!开停车判断判定开停车最小转速阈值，默认为15*/
extern int g_rev_threshold;

/*!用来断判定波形相似度的最小转速阈值，默认为250*/
extern int g_rev_threshold_sink;

/*!用来存储数据保留时间变量（起始时间，结束时间，表名，是否已经开始进行保留操作）*/
extern std::map< pair<CBHDateTime,CBHDateTime>,map<CString,int> > g_map_SpecSaveTime;

/*!用来存储实时数据喂狗接口*/
extern std::map< int,int > g_map_RealTimeInterface;
/*!用来存储其它喂狗接口*/
extern std::map< int,int > g_map_OtherInterface;

/*!保存小时趋势的间隔，一分钟保存一次60秒*/
#define D_TREND_HOURINTERVAL   58

/*!保存天趋势的间隔，20分钟一次（1200秒），单位：秒*/
#define D_TREND_DAYINTERVAL   1198

/*!保存月趋势的间隔，3小时一次（180），单位：分钟*/
#define D_TREND_MONTHINTERVAL  179

/*!上次转速和当前转速差的上限，如果超过上限就保存启停车数据*/
#define D_REV_DIFF_RANGE       0.5

//数据库链接不上出现的错误信息
#define     D_DBCONN_ERROR_CODE       -2147467259

//快变报警日志时间缓存上限
#define    D_FAST_CHANGE_LOG_TIME_COUNT 1000


enum PLANTINFOTYPE
{
	PLANTINFO_REAL,
	PLANTINFO_ALARM,
	PLANTINFO_SUD,
	PLANTINFO_NETOFF
};

extern bool _g_bInit;

///初始化函数和退出一些处理函数
/*!初始化全局变量，从数据库中读出相关数据，对各种全局变量进行内存分配和链表填充等操作
  *@param pDB:数据库连接和操作指针
  */
void InitGlobalVar(CCustomDB *pDB_);
//保存数据使用的指针 Dh表示数据处理类
/*extern CCustomDB *g_pDhDB;*/

extern CCompanyInfoPool *g_pCompanyPoolReal;
extern CCompanyInfoPool *g_pCompanyPoolAlarm;
extern CCompanyInfoPool *g_pCompanyPoolSud;
extern CCompanyInfoPool *g_pCompanyPoolNetOff;

//系统中数据的初始时间，即如果没有数据发送上来，就使用这个时间。
extern CBHDateTime g_dtInitTime;

extern CString g_strDbUser;
extern CString g_strDbPwd;
extern CString g_strDbName;
extern CString g_strDbServer;

extern BOOL g_bSkipAlarm ;
extern int G_D_ALARM_SAVEWAVE_NUM;
extern BOOL g_bIsShouldSaveEmptyValue; 
extern BOOL g_bIsDiskFull;
extern int g_nDefaultNetOffTimeSpan;
extern BOOL g_bOpenNetOffStatus ;

extern CRITICAL_SECTION * g_pSavingCS;
extern CRITICAL_SECTION * g_MapLock;
extern CRITICAL_SECTION * g_pUpdateParamLock;


extern int g_UseNewAlarmLogic;
extern int g_nNewAlarmMaxTime;					    //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
extern int g_nSaveIndenseTime;						//密集保存时间间隔,								默认5分钟
extern int g_nSaveHalfIndenseTime;					//半密集保存时间间隔,							默认30秒
extern int g_nHalfNormalNotSaveTime;				//半正常状态下持续保存的时间长度，				默认20分钟
extern int g_nSaveSparseTime;						//就报警每30分钟保存一组的时间间隔,				默认30分钟
extern int g_nHalfNormalTime;						//半正常状态维持时间间隔						默认2个小时
extern int g_nChannHalfNormalTime;					//测点半正常状态维持时间间隔					默认1个小时
extern int g_UseNewDCAlarmLogic;					//是否使用新的直流量报警逻辑					默认否
extern int g_SaveDataIntervalForNewDCAlarmLogic ;   //为新的直流量报警逻辑保存数据的时间间隔		默认15分钟
extern int g_wave_diff_ratio;						//配置波形相似度的值							默认90%
extern int g_alarm_larger_times;					//连续超过上次报警的次数						默认3
extern int g_UseNewAxisAlarmLogic;					//是否使用新的轴位移报警逻辑					默认否
extern int g_SaveFastAndRegularAlarmData;           //快变报警判断是否同时保存快变和常规报警数据    默认是
extern int g_SaveFastAndRegularAlarmData	;    //快变报警判断是否同时保存快变和常规报警数据    默认是
extern int g_SaveDataOnStop				;    //非临时在线模式下停车状态下是否保存日常数据表中的秒表，小时表，天表数据
extern int g_SaveMonthDataOnStop		;	//非临时在线模式下停车状态下是否保存日常数据表中的月表数据
extern int g_SaveAllFastData;               //是否保存所有快变数据，0：仅保存快变且发生报警的数据，1：保存所有快变数据，默认0
extern int g_SaveAllFastDataWave        ;   //是否保存所有快变数据波形
extern int g_SaveFastDataWaveInterval;      //!b保存快变数据波形时间间隔
extern int g_SaveUseOptimizeMode;           //使用优化存储模式
extern int g_UpdateProcDataOverTime;                 //过程量测点数据更新超时时间                   默认5分钟
extern int g_UpdatePlantDataOverTime;               //设备数据更新超时时间                          默认30分钟


int GetAllPlantInfo(string sCompany_,
					string sFactory_,
					string sPlant_,
					vector<CPlantInfoOnLine *>& arefPlants,
					PLANTINFOTYPE plantInfoType_=PLANTINFO_REAL);

CCustomDB * CreateHandleDataDBConn();
void GetDBUidPwd(CString &ag_strDbName, CString &ag_strUID, CString &ag_strPwd, CString &ag_strDBServer);

/*!销毁全局变量，销毁变量，对各种全局变量进行内存销毁和链表清空等操作
  */
void QuitClearGlobalVar();

///判断数据库是否处于连接状态，如果断开就重新连接.
int CheckDBConn(CCustomDB * pDB_);

int CheckDBConnEx(IDBInterFace * pDB_);
///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_);

//得到指定公司、分厂和设备的设备信息指针
CPlantInfoOnLine * GetSpecCompanyFactoryPlant(string sCompany_,
											  string sFactory_,
											  string sPlant_,
											  PLANTINFOTYPE plantInfoType_=PLANTINFO_REAL);

int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);
int writefile(float * ag_fwave, int ag_num, CString as_title);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,CString & astrPath);
int writefile( int * ag_fwave, int ag_num,CString & astrPath);


int readfile( DATATYPE_WAVE * ag_fwave,int anFileSize,CString strFile_);
int readfile( float * ag_fwave,int anFileSize,CString strFile_);
int readfile( int * ag_fwave,int anFileSize,CString strFile_);


LONGLONG GetFileSize(CString & lstrFileName);

BOOL MakeSureFolderExist( const CString &asAbsoluteFilePath ); 

BOOL UpdateWatchDogFileTime(const CString & astrRefFileName = _T(""));
BOOL InitMachineInfoType(IDBInterFace & arefDb);
BOOL IsMWComputeFeatureValueEnabled();
BOOL IsPlantRunning(CPlantInfoOnLine & arePlant,bool abRun);
BOOL IsPlantRevThanGiven(CPlantInfoOnLine & arePlant,int lnRev);
BOOL InitAlarmParameters();
BOOL UpdateJudgeAlarmTime();
int GetMinSampleNum();

int GetAlarmThreshold();
int InitMWRevThreshold();
COleDateTimeSpan GetJudgeAlarmDatetime();
int GetAlarmOverridTime();
extern IReadWriteLock  * gpWatchDogFileLock ; 
typedef std::vector<S_Machineinfotype> MachIneInfoTypeVo;
typedef std::map<int,MachIneInfoTypeVo> MachIneInfoTypeCollection;

extern MachIneInfoTypeCollection *  g_pMachIneInfoTypeCollection;
extern CBHDateTime g_local_judge_alarm_time;
bool isLocalJudgeAlarm();
BOOL SaveDetainDataTime();
BOOL UpdateAlarmParam();
BOOL UpdateTrendDataSaveParam();
BOOL UpdateFastDataSaveParam();

bool QuitProcess(DWORD dwPID_);

BOOL InitFeedWatchDogInterface();

int GetAllAlarmPlantInfo(vector<void *>& arefPlants_,CString strMachType_);
#endif //AFX_ONLINEGLOBAL_INCLUDED_MARBLE_




