#pragma once
#include "hzlog.h"
#include "afxmt.h"

#define HZ_COMMON_LIB_HZ_LOG_TXT_VERSITION		"1.0.1"					//°æ±¾

class CHZLogTxt :public CHZLog
{
public:
	CHZLogTxt( int nLogLevel, const CString &strFilePreName,const CString &strFileDir, const CString &strAppenders = "", const CString &strLayouts = "" );
	virtual ~CHZLogTxt(void);

public:

	virtual BOOL IsDebugEnabled();

	virtual BOOL IsErrorEnabled(); 

	virtual BOOL IsFatalEnabled();

	virtual BOOL IsInfoEnabled();

	virtual BOOL IsTraceEnabled(); 

	virtual BOOL IsWarnEnabled();

	virtual void Trace(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  ); 

	virtual void Debug(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" ); 

	virtual void Info(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );

	virtual void Warn(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" );

	virtual void Error(const CString &strMsg, const CString &strModule = "", const CString &strOperType = ""  );

	virtual void Fatal(const CString &strMsg, const CString &strModule = "", const CString &strOperType = "" );

protected:
	void CreateLogFile();
	void WriteInfo( const CString &strInfo );
    CString ConsInfoString(const CString &strType_,const CString &strMsg, const CString &strModule = "", const CString &strOperType = "");
protected:
	CString m_strAppender;
	CString m_strLayouts;
	CString m_strFilePreName;
	CString m_strFileDir;

	CString	m_strFilePath;

	CFile	m_File;

	CCriticalSection	m_CriSec;
};
