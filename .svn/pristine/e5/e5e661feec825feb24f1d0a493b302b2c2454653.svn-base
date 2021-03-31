#ifndef __MUTEX_READERS_WRITERS_MARBLE_
#define __MUTEX_READERS_WRITERS_MARBLE_
#include "stdafx.h"
#include "MutexRW.h"

volatile int G_LockCount = 0;

CMutexRW::CMutexRW() :m_semReaders(NULL),m_semWriters(NULL),m_nReaders(0)
{
	// initialize the Readers & Writers variables
	m_semReaders	= ::CreateSemaphore(NULL, 1, 1, NULL);
	m_semWriters	= ::CreateSemaphore(NULL, 1, 1, NULL);
	m_nReaders		= 0;
	
	CString lstrTraceData;
	
	G_LockCount +=2;
	
	lstrTraceData.Format(_T("*************** lockCount:%d****************\r\n"),G_LockCount);

	::OutputDebugString(lstrTraceData.GetBuffer(0));
	
		if (m_semReaders == NULL || m_semWriters == NULL)
	{
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
		TRACE(TEXT("***** ERROR: CreateSemaphore: %s\n"), (LPCTSTR)lpMsgBuf );
		LocalFree( lpMsgBuf );		

		
	}
}

CMutexRW::~CMutexRW()
{
	if (m_semWriters)
		VERIFY( ::CloseHandle(m_semWriters) );	

	m_semWriters = NULL;
	if (m_semReaders)
		VERIFY( ::CloseHandle(m_semReaders) );	
	m_semReaders = NULL;
}

void CMutexRW::BeginRead()
{
	DWORD dwEvent = WAIT_TIMEOUT;

	// P( semReaders )
	dwEvent = ::WaitForSingleObject( m_semReaders, INFINITE );
	ASSERT(dwEvent == WAIT_OBJECT_0);

	m_nReaders++;

	if (m_nReaders == 1)
	{
		// P( semWriters )
		dwEvent = ::WaitForSingleObject( m_semWriters, INFINITE );
		ASSERT(dwEvent == WAIT_OBJECT_0);
	}
	// V( semReaders )
	VERIFY( ::ReleaseSemaphore( m_semReaders, 1, NULL ) );
}


void CMutexRW::EndRead()
{
	DWORD dwEvent = WAIT_TIMEOUT;
	// P( semReaders )
	dwEvent = ::WaitForSingleObject( m_semReaders, INFINITE );
	ASSERT(dwEvent == WAIT_OBJECT_0);

	m_nReaders--;

	if (m_nReaders == 0)
	{
		// V( semWriters )
		VERIFY( ::ReleaseSemaphore(m_semWriters, 1, NULL) );
	}
	// V( semReaders )
	VERIFY( ::ReleaseSemaphore( m_semReaders, 1, NULL ) );
}

void CMutexRW::BeginWrite()
{
	DWORD dwEvent = WAIT_TIMEOUT;

	// P( semWriters )
	dwEvent = ::WaitForSingleObject( m_semWriters, INFINITE );
	ASSERT(dwEvent == WAIT_OBJECT_0);
}

void CMutexRW::EndWrite()
{
	// V( semWriters )
	VERIFY( ::ReleaseSemaphore(m_semWriters, 1, NULL) );
}


