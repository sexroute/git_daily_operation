#pragma  once
#include "StdAfx.h"
#include ".\plantinfoexpertsys.h"
#include <fstream>
#include "../ServerStatus/IServerStatusExport.h"
extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;
#define D_SENDWAVENUM  513

#define D_TRENDRECORDS 300


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

//设备详细映射，包括测点信息。string 为公司名和设备名字符串相连。比如：辽化、PLANT1连接后：辽化PLANT1
typedef map<string,CPlantInfoExpertSys *> MAP_PLANTINFO;
//通过这个映射表，可以查询设备、测点等详细信息
extern MAP_PLANTINFO g_mapPlantInfo;
extern bool g_bInit;
void InitGlobalVar(CCustomDB *pDB_);
void QuitClearGlobalVar();

#define TIME_SPAN_ALARM_VALUE_TEMP_TREND 5					//设置报警温度测点前后各5分钟内的数据
#define TIME_SPAN_ALARM_VALUE_TEMP_TREND_BEFORE 40			//设置报警温度测点前40分钟内的数据
#define TIME_SPAN_ALARM_VALUE_TEMP_TREND_AFTER  20			//设置报警温度测点后20分钟内的数据
#define TIME_SPAN_ALARM_VALUE_TEMP_TREND_DATA_LIMIT  100	//趋势数据个数
#define TIME_SPAN_ALARM_VALUE_SINK_TREND 5					//设置报警沉降量测点前后各5秒钟内的数据
#define TIME_SPAN_ALARM_VALUE_SINK_TREND_BEFORE 50			//设置报警沉降量测点前50秒钟内的数据
#define TIME_SPAN_ALARM_VALUE_SINK_TREND_AFTER 10			//设置报警沉降量测点后10秒钟内的数据
#define TIME_SPAN_ALARM_VALUE_DEFLECT_TREND 5				//设置报警偏摆量测点前后各5秒钟内的数据
#define TIME_SPAN_ALARM_VALUE_DEFLECT_TREND_BEFORE 50		//设置报警偏摆量测点前50秒钟内的数据
#define TIME_SPAN_ALARM_VALUE_DEFLECT_TREND_AFTER 10			//设置报警偏摆量测点后10秒钟内的数据
#define ALARM_VALUE_SINK_TREND_DATA_LIMIT 50
#define ALARM_VALUE_DEFLECT_TREND_DATA_LIMIT 50
#define TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE 0			//设置报警温度测点前后各30分钟内的数据
#define TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE_DATA 0		//设置报警温度测点前后各的数据

#define TIME_SPAN_ALARM_VALUE_CROSS_HEAD_WAVE_NO_ALARM 3	//设置报警温度测点前后各3天的数据
///得到主数据库名
void GetMainDBName();
///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_);

///将组态所得的连接形式转化为专家系统知识库中的连接形式
int ConvertConnType(int iConnType_);
///将组态所得的转子形式转化为专家系统知识库中的转子形式
int ConvertRotateType(int iRotateType_);
///将组态所得的工作转速转化为专家系统知识库中的工作转速
int ConvertWorkRev(int iWorkRev_);
///将组态所得的驱动类型转化为专家系统知识库中的驱动类型
int ConvertDriverType(int iDriverType_);
///将组态所得的轴承类型转化为专家系统知识库中的轴承类型
int ConvertBearType(int iBearType_);
///将组态所得的密封形式转化为专家系统知识库中的密封形式
int ConvertSealType(int iSealType_);
///调试之用函数
int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);

