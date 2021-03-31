#pragma once

#include "HZLog.h"

#ifndef HZ_COMMON_LIB_HZ_LOG_MANAGE_VERSITION

#define HZ_COMMON_LIB_HZ_LOG_MANAGE_VERSITION		"1.0.1"					//版本

class _KDCOMMLIB_API_ CHZLogManage 
{
public:
	CHZLogManage(void);
	~CHZLogManage(void);

public:
	static BOOL IsDebugEnabled();

	static BOOL IsErrorEnabled(); 

	static BOOL IsFatalEnabled();

	static BOOL IsInfoEnabled();

	static BOOL IsTraceEnabled(); 

	static BOOL IsWarnEnabled();

	static void Trace(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ); 

	static void Debug(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  ); 

	static void Info(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" );

	static void Warn(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );

	static void Error(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );

	static void Fatal(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );


	/*
	函数作用：初始化Log文档
	返回值：无
	参数作用：
			[in]enLevel:log级别
			[in]strPreFileName:日志文档的前辍
	*/
	static void StartLogTxt(CString strFileDir_,CString strFileName,int iLogLevel_ = 2 );

	static UINT GetLogger(CString strFileDir_,CString strFileName,int iLogLevel_ = 2 );

	static void Log(UINT anLoggHandle,const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ,int iLogLevel_= 2);


protected:

	/*
	函数作用：通过给出的strKey节点初始化文档节点
	返回值：无
	参数作用：
	strKey：配置文档中的strKey节点
	*/
	static void InitLog( CString strKey = "", CString strFile = "" );

	static void ValidLog();

protected:
	static CHZLog *m_pLog;					//日志接口
};

#endif
