#pragma  once
#include "StdAfx.h"
#include "PlantInfo.h"
#include "additions/ReaderWriterlock.h"
#include "../ServerStatus/IServerStatusExport.h"
extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;
#define D_SENDWAVENUM  513

//#define D_TRENDRECORDS 300


//秒趋势表中保存记录的小时数7,
#define D_SECONDTREND_DAYS 7
//小时趋势表中保存记录的天数
#define D_HOURTREND_DAYS 90
//天趋势表中保存记录的天数3*360
#define D_DAYTREND_DAYS  1080
///为了提高减少读取记录的条数
#define TRENDRECORDS_COEF 0.7

//当查询趋势数据、按准确时间查询为空时，按一段时间的区间查询趋势数据
#define TRENDRECORDS_QUERYTIMESPAN 8
//当查询报警数据、按准确时间查询为空时，按一段时间的区间查询报警数据，单位秒
#define ALARMRECORDS_QUERYTIMESPAN 20
//当查询报警数据、按准确时间查询为空时，按一段时间的区间查询报警数据最大时间窗长度，单位秒
#define ALARMRECORDS_QUERYTIMESPAN_MAX 600

#define TREND_RECORD_NUM             300        //返回给客户端历史趋势记录数
#define G_SAVEALLFASTDATAWAVE           1        //是否保存所有快变数据

/*!是否开启停车仅保存月表策略，默认为0,即不开启*/
extern int G_TEMP_ONLINE;
/*!是否开启跨表查询策略，默认为0,即不开启,1为开启趋势表跨表查询功能,2在1的基础上增加报警数据表的联查功能,3在2的基础上开启启停车联查功能*/
extern int G_ENABLE_SEARCH_TABLE;
int IsEnableSearchAllTable();

//设备详细映射，包括测点信息。string 为公司名和设备名字符串相连。比如：辽化、PLANT1连接后：辽化PLANT1
typedef map<string,CPlantInfoOffLine *> MAP_PLANTINFO;
extern CReaderWriterLock g_MapLock;
//通过这个映射表，可以查询设备、测点等详细信息
extern MAP_PLANTINFO g_mapPlantInfo;
extern bool g_bInit;
extern int  g_SaveAllFastDataWave;

void InitGlobalVar(CCustomDB *pDB_);
void QuitClearGlobalVar();
void GetMainDBName();//得到主数据库名
///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_);
BOOL SyncSampleRecords();
///调试之用函数
int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);
COleDateTimeSpan GetJudgeAlarmDatetime();
int GetAlarmOverridTime();
bool isLocalJudgeAlarm();
int GetConfigTrendRecordsNum();
extern CBHDateTime g_local_judge_alarm_time;
extern BOOL g_bIsDiskFull;
