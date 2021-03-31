
#include "stdafx.h"
#include <dbghelp.h>
#include "mdump.h"

using namespace Winet;
LPCSTR MiniDumper::m_szAppName;

MiniDumper::MiniDumper( LPCTSTR szAppName )
{
	// if this assert fires then you have two instances of MiniDumper
	// which is not allowed
	if (szAppName == NULL)
	{
		m_szAppName =_T("");
	}
	
	m_szAppName = szAppName ? strdup(szAppName) : _T("Application");

	::SetUnhandledExceptionFilter( TopLevelFilter );
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL;						// find a better value for your app

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	char szDbgHelpPath[_MAX_PATH];

	if (GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
	{
		TCHAR *pSlash = _tcsrchr( szDbgHelpPath, _T('\\') );
		if (pSlash)
		{
			_tcscpy( pSlash+1, _T("DBGHELP.DLL") );
			hDll = ::LoadLibrary( szDbgHelpPath );
		}
	}

	if (hDll==NULL)
	{
		// load any version we can
		hDll = ::LoadLibrary( _T("DBGHELP.DLL") );
	}

	LPCTSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, _T("MiniDumpWriteDump") );
		if (pDump)
		{
			TCHAR szDumpPath[_MAX_PATH]={0};
			TCHAR szScratch [_MAX_PATH]={0};
			TCHAR szDateTime[_MAX_PATH]={0};

			// work out a good place for the dump file
			if (FALSE)
			{
				if (!GetTempPath( _MAX_PATH, szDumpPath ))
				{
					_tcscpy( szDumpPath, _T("c:\\temp\\") );
				}
			}else
			{
				_tcscpy( szDumpPath, _T(".\\") );
			}

			SYSTEMTIME sysTime;
			GetLocalTime( &sysTime );
			_stprintf( szDateTime, _T("%02d_%02d_%02d_%02d_%02d_%02d"), 
				sysTime.wYear, sysTime.wMonth, sysTime.wDay,
				sysTime.wHour, sysTime.wMinute, sysTime.wSecond );

			if (m_szAppName != NULL)
			{
				_tcscat( szDumpPath, m_szAppName);
				_tcscat( szDumpPath, _T("_"));

			}
			_tcscat( szDumpPath, szDateTime );
			_tcscat( szDumpPath, ".dmp" );

			// ask the user if they want to save a dump file
			if (TRUE)
			{
				// create the file
				HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;
					
					// write the dump
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );

					//if (bOK)
					{
						retval = EXCEPTION_EXECUTE_HANDLER;
					}

					::CloseHandle(hFile);
				}
				else
				{

				}
			}
		}
		else
		{
			szResult = "DBGHELP.DLL too old";
		}
	}
	else
	{
		szResult = "DBGHELP.DLL not found";
	}

/*
	if (szResult)
		::MessageBox( NULL, szResult, m_szAppName, MB_OK );

*/
	return retval;
}

LONG WINAPI Winet::GEHExceptionFilter( _EXCEPTION_POINTERS* ExceptionInfo ) 
{ 
#ifdef _DEBUG
	::MessageBox( NULL, "Error Caught", NULL, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1|MB_SYSTEMMODAL );
	MiniDumper::TopLevelFilter(ExceptionInfo); 
	return EXCEPTION_EXECUTE_HANDLER; 
#endif // _DEBUG
	//

	MiniDumper::TopLevelFilter(ExceptionInfo); 

	if (   (ExceptionInfo)
		&& (ExceptionInfo->ExceptionRecord )
		&& (ExceptionInfo->ExceptionRecord->ExceptionCode == 0xC0000005))
	{
		ExitProcess( 0 ); 
	}

	return EXCEPTION_EXECUTE_HANDLER; 
} 