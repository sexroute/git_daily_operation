#include "StdAfx.h"
#include "hzlogtxt.h"


CHZLogTxt::CHZLogTxt(int nLogLevel, const CString &strFilePreName,const CString &strFileDir, const CString &strAppenders, const CString &strLayouts )
{
	_iLogLevel			= nLogLevel;
	m_strAppender		= strAppenders;
	m_strLayouts		= strLayouts;

	m_strFilePreName	= strFilePreName;
	m_strFileDir		= strFileDir;


	//生成新的文档句柄
	CreateLogFile();

}

CHZLogTxt::~CHZLogTxt(void)
{
	if( m_File.m_hFile != NULL )
	{
		m_File.Close();
	}
}

BOOL CHZLogTxt::IsDebugEnabled()
{
	if( _iLogLevel&CHZLogTxt::MODE_DEBUG )
	{
		return true;
	}

	return false;
}

BOOL CHZLogTxt::IsErrorEnabled()
{
	if(_iLogLevel&CHZLogTxt::MODE_ERROR )
	{
		return true;
	}

	return false;
}

BOOL CHZLogTxt::IsFatalEnabled()
{
	if(_iLogLevel&CHZLogTxt::MODE_FATAL )
	{
		return true;
	}

	return false;
}

BOOL CHZLogTxt::IsInfoEnabled()
{
	if( _iLogLevel&CHZLogTxt::MODE_INFO )
	{
		return true;
	}

	return false;
}

BOOL CHZLogTxt::IsTraceEnabled()
{
	if( _iLogLevel&CHZLogTxt::MODE_TRACE )
	{
		return true;
	}

	return false;
}

BOOL CHZLogTxt::IsWarnEnabled()
{
	if( _iLogLevel&CHZLogTxt::MODE_WARN )
	{
		return true;
	}

	return false;
}

void CHZLogTxt::Trace(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	if( !IsTraceEnabled() )
	{
		return ;
	}

	CString info = ConsInfoString("TRACE",strMsg,strModule,strOperType);

	WriteInfo( info );
}

void CHZLogTxt::Debug(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	if( !IsDebugEnabled() )
	{
		return ;
	}

	CString info = ConsInfoString("DEBUG",strMsg,strModule,strOperType);

	WriteInfo( info );
}

void CHZLogTxt::Info(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	if( !IsInfoEnabled() )
	{
		return ;
	}

	CString info = ConsInfoString("INFO",strMsg,strModule,strOperType);

	WriteInfo( info );
}

void CHZLogTxt::Warn(const CString &strMsg, const CString &strModule, const CString &strOperType )
{

	if( !IsWarnEnabled() )
	{
		return ;
	}

	CString info =ConsInfoString("WARN",strMsg,strModule,strOperType);

	WriteInfo( info );
}

void CHZLogTxt::Error(const CString &strMsg, const CString &strModule, const CString &strOperType )
{

	if( !IsErrorEnabled() )
	{
		return ;
	}

	CString info =ConsInfoString("ERROR",strMsg,strModule,strOperType);

	WriteInfo( info );
}

void CHZLogTxt::Fatal(const CString &strMsg, const CString &strModule, const CString &strOperType )
{
	if( !IsFatalEnabled() )
	{
		return ;
	}

	CString info =ConsInfoString("FATAL",strMsg,strModule,strOperType);
	WriteInfo( info );
}

void CHZLogTxt::CreateLogFile()
{
	//形成文档路径
	{
		if (m_strFileDir.GetLength()<1) 
		   m_strFileDir=".\\log";
		if(m_strFilePreName.GetLength()<1)
			m_strFilePreName="KD";
		m_strFilePath = m_strFileDir+"\\";
		m_strFilePath += m_strFilePreName;
		m_strFilePath += CTime::GetCurrentTime().Format( "_%Y-%m-%d" );

		m_strFilePath += ".log";

		if(0!= _access( m_strFilePath, 00 ) )
		{
			HANDLE fileHandle = CreateFile( m_strFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
			
			CloseHandle( fileHandle );
		}
	}
}

CString CHZLogTxt::ConsInfoString(const CString &strType_,const CString &strMsg, const CString &strModule, const CString &strOperType)
{
	CString info = "{[" + CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") + "]";

	if( strModule.GetLength() > 0 )
	{
		info += "[模块：";
		info += strModule + "]";
	}
	if( strOperType.GetLength() > 0 )
	{
		info += "[函数：";
		info += strOperType + "]";
	}
	info += "}";

	info += "[";

	info += strType_;
   
    info += "]";

	info += strMsg;

	info += "\r\n";

	return info;
}

void CHZLogTxt::WriteInfo( const CString &strInfo )
{
	m_CriSec.Lock();

	//打开文档
	try
	{
		if( m_File.Open( m_strFilePath, CFile::modeWrite ) )
		{
			m_File.SeekToEnd();

			m_File.Write( strInfo, strInfo.GetLength() );
			m_File.Flush();
			m_File.Close();
		}
		else
		{
			CreateLogFile();
		}

	}
	catch (CFileException  *ex )
	{

	}

	m_CriSec.Unlock();
}