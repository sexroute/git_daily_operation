#include "StdAfx.h"
#include ".\hzlog.h"

CHZLog::CHZLog(void)
{
	 _iLogLevel=0;
}

CHZLog::~CHZLog(void)
{
}

CHZLog::LOG_LEVEL_ENUM CHZLog::GetLevelByString( CString strLevel )
{
	if( strLevel.CompareNoCase( "TRACE") == 0 )
	{
		return MODE_TRACE;
	}
	else if( strLevel.CompareNoCase( "DEBUG") == 0 )
	{
		return MODE_DEBUG;
	}
	else if( strLevel.CompareNoCase( "INFO") == 0 )
	{
		return MODE_INFO;
	}
	else if( strLevel.CompareNoCase( "WARN") == 0 )
	{
		return MODE_WARN;
	}
	else if( strLevel.CompareNoCase( "ERROR") == 0 )
	{
		return MODE_ERROR;
	}
	else
	{
		return MODE_FATAL;
	}
}