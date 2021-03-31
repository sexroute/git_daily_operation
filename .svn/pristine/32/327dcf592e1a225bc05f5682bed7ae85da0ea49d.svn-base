#include "StdAfx.h"
#include <CaseLibrary/MyLog.h>
#include <ATLComTime.h>

#define LOG_FILENAME_STR _T(".\\log\\CaseLibrary.log")

CMyLog::CMyLog(void)
{
}

CMyLog::~CMyLog(void)
{
}



void CMyLog::Trace_Log( char* str )
{
/*	ASSERT(FALSE);*/
	return;

	CBHDateTime dt = CBHDateTime::GetCurrentTime();

	FILE* pFile = fopen(LOG_FILENAME_STR,"a+");
	fprintf(pFile,"【%s】%s\n",dt.Format(),str);
	fclose(pFile);
}

void CMyLog::Clear_Log()
{
//	ASSERT(FALSE);
	return;

	FILE* pFile = fopen(LOG_FILENAME_STR,"w");
	fclose(pFile);	
}

void CMyLog::Trace_CreateHead_ID( int nID_ )
{
//	ASSERT(FALSE);
	return;

	CString strInfo;
	strInfo.Format("访问中间件功能号：%d",nID_);
	CMyLog::Trace_Log(strInfo.GetBuffer());
}