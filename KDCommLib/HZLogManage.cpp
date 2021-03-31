#include "StdAfx.h"
#include "hzlogmanage.h"
#include "HZLog.h"
#include "MutexRW.h"
#include <map>

#include "HZLogTxt.h"
CHZLog *CHZLogManage::m_pLog = NULL;

CHZLogManage::CHZLogManage(void)
{

}

CHZLogManage::~CHZLogManage(void)
{
}


void CHZLogManage::InitLog( CString strKey, CString strFile )
{
	if( NULL != m_pLog )
	{
		TRACE0( "Log已初始化!" );
	}


	//简易类工厂生成
	//没有考虑配置文件
	m_pLog = new CHZLogTxt( CHZLogTxt::MODE_DEBUG, "", "" );
	
	return ;
}

BOOL CHZLogManage::IsDebugEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsDebugEnabled();
}

BOOL CHZLogManage::IsErrorEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsErrorEnabled();
}

BOOL CHZLogManage::IsFatalEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsFatalEnabled();
}

BOOL CHZLogManage::IsInfoEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsInfoEnabled();
}

BOOL CHZLogManage::IsTraceEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsTraceEnabled();
}

BOOL CHZLogManage::IsWarnEnabled()
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->IsWarnEnabled();
}

void CHZLogManage::Trace(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Trace( strMsg, strModule, strOperType );
}

void CHZLogManage::Debug(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Debug( strMsg, strModule, strOperType );
}

void CHZLogManage::Info(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Info( strMsg, strModule, strOperType );
}

void CHZLogManage::Warn(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Warn( strMsg, strModule, strOperType );
}

void CHZLogManage::Error(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Error( strMsg, strModule, strOperType );
}

void CHZLogManage::Fatal(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	ValidLog();
	ASSERT( m_pLog );

	return  m_pLog->Fatal( strMsg, strModule, strOperType );
}

void CHZLogManage::ValidLog()
{
	if( NULL == m_pLog )
	{
		//InitLog();
		//StartLogTxt();
	}
}

void CHZLogManage::StartLogTxt( CString strFileDir_,CString strFileName,int iLogLevel_)
{
    m_pLog = new CHZLogTxt( iLogLevel_, strFileName, strFileDir_, "", "" );
}

std::map<CString,CHZLog *> g_mapLoggers;

CMutexRW g_oReadWriteLock;

UINT g_HANDLE_MASK = 0x11110000;

void CHZLogManage::Log(UINT anLoggHandle,const CString &strMsg, const CString &strModule /* =  */, const CString &strOperType /* =  */,int iLogLevel_)
{
	CHZLog * lpLog = (CHZLog *)(anLoggHandle ^ g_HANDLE_MASK);

	switch(iLogLevel_)
	{
	case CHZLog::MODE_DEBUG:
		lpLog->Debug(strMsg,strModule,strOperType);
		break;
	case CHZLog::MODE_ERROR:
		lpLog->Error(strMsg,strModule,strOperType);
		break;
	case  CHZLog::MODE_FATAL:
		lpLog->Error(strMsg,strModule,strOperType);
		break;
	case CHZLog::MODE_INFO:
		lpLog->Info(strMsg,strModule,strOperType);
		break;
	case CHZLog::MODE_TRACE:
		lpLog->Trace(strMsg,strModule,strOperType);
		break;
	case CHZLog::MODE_WARN:
		lpLog->Warn(strMsg,strModule,strOperType);
		break;
	default:
		lpLog->Debug(strMsg,strModule,strOperType);
		break;
	}
}

UINT CHZLogManage::GetLogger(CString strFileDir_,CString strFileName,int iLogLevel_ /* = CHZLog::MODE_DEBUG */ )
{
	CHZLog * lpLog = NULL;

	g_oReadWriteLock.BeginWrite();

	CString lstrKey ;
	
	lstrKey += strFileDir_;	
	
	lstrKey += strFileName;

	std::map<CString,CHZLog *>::iterator loitLogger = g_mapLoggers.find(lstrKey);

	if (g_mapLoggers.end() != loitLogger)
	{
		lpLog = loitLogger->second;
	
	}else
	{
		lpLog =  new CHZLogTxt(iLogLevel_,strFileName,strFileDir_,"","");

		g_mapLoggers[lstrKey] = lpLog;
	}	

	g_oReadWriteLock.EndWrite();

	ASSERT(lpLog!= NULL);

	UINT lnPesdoHandle = ((UINT)lpLog)^g_HANDLE_MASK;

	return lnPesdoHandle;
	
}