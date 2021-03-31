#pragma once

#ifndef _KDLOG_H_MARBLE_
#define _KDLOG_H_MARBLE_

#define HZ_COMMON_LIB_HZ_LOG_VERSITION		"1.0.1"					//版本


class CHZLog 
{
public:
	enum LOG_LEVEL_ENUM
	{
		MODE_TRACE =(int) 0x0001,				//比较细节的信息
		MODE_DEBUG =(int) 0x0002,				//系统流程中的细节信息
		MODE_INFO  =(int) 0x0004,				//运行时产生的有意义的事件
		MODE_WARN  =(int) 0x0010,				//运行时不合需要和不合预期的状态但还没有称为“错误”
		MODE_ERROR =(int) 0x0020,				//其它运行期的错误或不是预期的事件
		MODE_FATAL =(int) 0x0040,				//非常严重的错误，导致系统中止
	};
public:
	CHZLog(void);
	virtual ~CHZLog(void);
	
public:

	static CHZLog::LOG_LEVEL_ENUM GetLevelByString( CString strLevel );

	virtual BOOL IsDebugEnabled() = 0;

	virtual BOOL IsErrorEnabled() = 0;

	virtual BOOL IsFatalEnabled() = 0;

	virtual BOOL IsInfoEnabled() = 0;

	virtual BOOL IsTraceEnabled() = 0;

	virtual BOOL IsWarnEnabled() = 0;

	/*
	函数作用：产生比较细节的信息
	返回值：无
	参数：
			[in]strMsg:信息内容
			[in]strModule:操作模块
			[in]strOperType:操作类型
	*/
	virtual void Trace(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;

	/*
	函数作用：产生系统流程中的细节信息
	返回值：无
	参数：
		[in]strMsg:信息内容
		[in]strModule:操作模块
		[in]strOperType:操作类型
	*/
	virtual void Debug(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;


	/*
	函数作用：产生运行时产生的有意义的事件
	返回值：无
	参数：
		[in]strMsg:信息内容
		[in]strModule:操作模块
		[in]strOperType:操作类型
	*/
	virtual void Info(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  ) = 0;

	/*
	函数作用：产生运行时不合需要和不合预期的状态但还没有称为“错误”
	返回值：无
	参数：
		[in]strMsg:信息内容
		[in]strModule:操作模块
		[in]strOperType:操作类型
	*/
	virtual void Warn(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;


	/*
	函数作用：产生其它运行期的错误或不是预期的事件
	返回值：无
	参数：
		[in]strMsg:信息内容
		[in]strModule:操作模块
		[in]strOperType:操作类型
	*/
	virtual void Error(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;


	/*
	函数作用：产生非常严重的错误，导致系统中止
	返回值：无
	参数：
		[in]strMsg:信息内容
		[in]strModule:操作模块
		[in]strOperType:操作类型
	*/
	virtual void Fatal(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ) = 0;

protected:
	int _iLogLevel;
};
#endif//_KDLOG_H_MARBLE_