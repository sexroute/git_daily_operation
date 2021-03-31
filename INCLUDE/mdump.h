#pragma once
#ifndef DECLSPEC_MINIDUMPER
#define DECLSPEC_MINIDUMPER
// VC7: ships with updated headers
#include <dbghelp.h>

namespace Winet
{
	// based on dbghelp.h
	typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
		CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
		CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
		);

	class MiniDumper
	{
	private:
		static LPCSTR m_szAppName;


	public:
		MiniDumper( LPCSTR szAppName );
		static LONG WINAPI TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo );

	};

	LONG WINAPI GEHExceptionFilter( _EXCEPTION_POINTERS* ExceptionInfo );
}

#endif