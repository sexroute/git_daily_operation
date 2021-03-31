#pragma once

#include "HZLog.h"

#ifndef HZ_COMMON_LIB_HZ_LOG_MANAGE_VERSITION

#define HZ_COMMON_LIB_HZ_LOG_MANAGE_VERSITION		"1.0.1"					//�汾

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
	�������ã���ʼ��Log�ĵ�
	����ֵ����
	�������ã�
			[in]enLevel:log����
			[in]strPreFileName:��־�ĵ���ǰ�
	*/
	static void StartLogTxt(CString strFileDir_,CString strFileName,int iLogLevel_ = 2 );

	static UINT GetLogger(CString strFileDir_,CString strFileName,int iLogLevel_ = 2 );

	static void Log(UINT anLoggHandle,const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ,int iLogLevel_= 2);


protected:

	/*
	�������ã�ͨ��������strKey�ڵ��ʼ���ĵ��ڵ�
	����ֵ����
	�������ã�
	strKey�������ĵ��е�strKey�ڵ�
	*/
	static void InitLog( CString strKey = "", CString strFile = "" );

	static void ValidLog();

protected:
	static CHZLog *m_pLog;					//��־�ӿ�
};

#endif
