#pragma comment(lib,"winmm.lib") 
#pragma comment(lib,"rpcrt4.lib")  
#include "stdafx.h"
#include <additions/ReaderWriterLock.h>
#include <mmsystem.h>



CReaderWriterLockNonReentrance::CReaderWriterLockNonReentrance()
{
	ZeroMemory(this, sizeof(*this));
#if (_WIN32_WINNT >= 0x0403)
	InitializeCriticalSectionAndSpinCount(&m_cs, READER_WRITER_SPIN_COUNT);
#else
	InitializeCriticalSection(&m_cs);
#endif
}

CReaderWriterLockNonReentrance::~CReaderWriterLockNonReentrance()
{
	ASSERT( (NULL == m_hSafeToReadEvent) &&
		(NULL == m_hSafeToWriteEvent) );
	if (m_hSafeToReadEvent!=NULL)
	{
		CloseHandle(m_hSafeToReadEvent);
	}
	if (m_hSafeToWriteEvent!=NULL)
	{
		CloseHandle(m_hSafeToWriteEvent);
	}
	
	DeleteCriticalSection(&m_cs);
}

BOOL CReaderWriterLockNonReentrance::_AcquireReaderLockAndLeaveCS(long alTime_out /* = INFINITE */)
{
	BOOL result = FALSE;
	//EnterCriticalSection(&m_cs);
	long ulTime_Left = alTime_out;
	long ulTime_Temp = 0;
	if(m_iNumOfWriter)
	{
		++m_iNumOfReaderWaiting;
		
		if(NULL == m_hSafeToReadEvent)
		{
			//////////////////////////////////////////////////////
			// This is a long-time task inside Critical Section
			// However, there isn't any effective way to overcome
			m_hSafeToReadEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		}
		
		do
		{	
			LeaveCriticalSection(&m_cs);
			
			if (ulTime_Left !=INFINITE)
			{
				ulTime_Temp = timeGetTime();
			}			
			
			result =WaitForSingleObject(m_hSafeToReadEvent, ulTime_Left);

			if (ulTime_Left != INFINITE)
			{
				ulTime_Left -= timeGetTime() - ulTime_Temp;
				
				if (ulTime_Left < 0)
				{
					ulTime_Left  = 0;
				}
			}	
			

			// There might be one or more Writers entered, that's
			// why we need DO-WHILE loop here
			EnterCriticalSection(&m_cs);
			
			if (result == WAIT_TIMEOUT)
			{								
				result = FALSE;
				
				break;
			}
			result = TRUE;
		}
		while(m_iNumOfWriter);
		
		// Enter successful after wait

			++m_iNumOfReaderEntered;
			
			if(0==--m_iNumOfReaderWaiting)
			{
				HANDLE hTemp = m_hSafeToReadEvent;
				
				m_hSafeToReadEvent = NULL;
				
				LeaveCriticalSection(&m_cs);
				
				CloseHandle(hTemp);
			}
			else
			{
				LeaveCriticalSection(&m_cs);
			}	

		
	}
	else
	{
		// Enter successful without wait
		result = TRUE;

		++m_iNumOfReaderEntered;

		LeaveCriticalSection(&m_cs);
	}
	

	return result;
}

void CReaderWriterLockNonReentrance::_ReleaseReaderLockAndLeaveCS()
{
	INT _iNumOfReaderEntered = --m_iNumOfReaderEntered;
	
	ASSERT(0 <= _iNumOfReaderEntered);
	
	if( (0 == _iNumOfReaderEntered) &&
		(NULL != m_hSafeToWriteEvent) )
	{
		
		
		/////////////////////////////////////////////////////////////////
		// INTERESTING point: We signal one of queued WRITERs outside
		// Critical Section without any problem in logic but better
		// in performance
		VERIFY(SetEvent(m_hSafeToWriteEvent));
		
		LeaveCriticalSection(&m_cs);
	}
	else
	{
		LeaveCriticalSection(&m_cs);
	}
}

BOOL CReaderWriterLockNonReentrance::_AcquireWriterLockAndLeaveCS(BOOL blMustWait,long alTime_out /* = INFINITE */)
{
	//EnterCriticalSection(&m_cs);
	BOOL result = FALSE;

	// Increase Writer-counter & reset Reader-event if necessary
	INT _iNumOfWriter = ++m_iNumOfWriter;

	//没有写线程
	if(	(1 == _iNumOfWriter) && (NULL != m_hSafeToReadEvent) )
	{
		ResetEvent(m_hSafeToReadEvent);
	}
	
	if(blMustWait && (NULL == m_hSafeToWriteEvent) )
	{
		m_hSafeToWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	}
	
	LeaveCriticalSection(&m_cs);
	
	if(blMustWait)
	{
		result = WaitForSingleObject(m_hSafeToWriteEvent, alTime_out);
		
		if (result == WAIT_TIMEOUT)
		{
			result = FALSE;		
			
		}else
		{
			result = TRUE;				 
		}

	}else
	{
		result = TRUE;
	}
	
	return result ;
}



void CReaderWriterLockNonReentrance::_ReleaseWriterLockAndLeaveCS(BOOL blDowngrade)
{
	
	if(blDowngrade)
	{
		++m_iNumOfReaderEntered;
	}
	
	if(0 == --m_iNumOfWriter)
	{
		if(m_hSafeToReadEvent)
		{
			//////////////////////////////////////////////////////
			// This is a long-time task inside Critical Section
			// However, there isn't any effective way to overcome
			VERIFY(SetEvent(m_hSafeToReadEvent));
		}
		
		if(m_hSafeToWriteEvent)
		{
			HANDLE hTemp = m_hSafeToWriteEvent;
			m_hSafeToWriteEvent = NULL;
			LeaveCriticalSection(&m_cs);
			
			// Close handle outside CS to increase throughput
			CloseHandle(hTemp);
		}
		else
		{
			LeaveCriticalSection(&m_cs);
		}
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// Some WRITERs are queued
		ASSERT( (0 < m_iNumOfWriter) && (NULL != m_hSafeToWriteEvent));
		
		
		if(FALSE == blDowngrade)
		{
			/////////////////////////////////////////////////////////////////
			// INTERESTING point: We signal one of queued WRITERs outside
			// Critical Section without any problem in logic but better
			// in performance
			VERIFY(SetEvent(m_hSafeToWriteEvent));			
		}
		LeaveCriticalSection(&m_cs);
	}
}

void CReaderWriterLockNonReentrance::AcquireReaderLock()
{
	EnterCriticalSection(&m_cs);
	_AcquireReaderLockAndLeaveCS();
}

void CReaderWriterLockNonReentrance::ReleaseReaderLock()
{
	EnterCriticalSection(&m_cs);
	_ReleaseReaderLockAndLeaveCS();
}

void CReaderWriterLockNonReentrance::AcquireWriterLock()
{
	EnterCriticalSection(&m_cs);
	_AcquireWriterLockAndLeaveCS(m_iNumOfWriter + m_iNumOfReaderEntered);
}

void CReaderWriterLockNonReentrance::ReleaseWriterLock()
{
	EnterCriticalSection(&m_cs);
	_ReleaseWriterLockAndLeaveCS(FALSE);
}

BOOL CReaderWriterLockNonReentrance::TryAcquireReaderLock()
{
	EnterCriticalSection(&m_cs);
	BOOL blCanRead = (0 == m_iNumOfWriter);
	if(blCanRead)
	{
		++m_iNumOfReaderEntered;
	}
	LeaveCriticalSection(&m_cs);
	return blCanRead;
}

BOOL CReaderWriterLockNonReentrance::TryAcquireWriterLock()
{
	EnterCriticalSection(&m_cs);
	BOOL blCanWrite = ((0 == m_iNumOfReaderEntered) && (0 == m_iNumOfWriter));
	if(blCanWrite)
	{
		// Increase Writer counter
		++m_iNumOfWriter;
	}
	LeaveCriticalSection(&m_cs);
	return blCanWrite;
}

void CReaderWriterLockNonReentrance::ReleaseReaderAndAcquireWriterLock()
{
	EnterCriticalSection(&m_cs);
	_AcquireWriterLockAndLeaveCS(--m_iNumOfReaderEntered);
}

void CReaderWriterLockNonReentrance::DowngradeFromWriterLock()
{
	EnterCriticalSection(&m_cs);
	_ReleaseWriterLockAndLeaveCS(TRUE);
}

// END CReaderWriterLockNonReentrance implementation
///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// CReaderWriterLock implementation

#define READER_RECURRENCE_UNIT 0x00000001
#define READER_RECURRENCE_MASK 0x0000FFFF
#define WRITER_RECURRENCE_UNIT 0x00010000

CReaderWriterLock::CReaderWriterLock()
{
}

CReaderWriterLock::~CReaderWriterLock()
{
	for (CMapThreadToState::iterator it = this->m_map.begin(); it != this->m_map.end() ; it++)
	{
		TRACE(_T("************Warning: thrread id:%x dosn't release lock before exiting *********\r\n"),it->first);
	}
}


BOOL CReaderWriterLock::AcquireReaderLock(long alTimeOut)
{
	const DWORD dwCurrentThreadId = GetCurrentThreadId();
	
	BOOL result = FALSE;
	
	EnterCriticalSection(&m_cs);
	
	const DWORD dwThreadState = (m_map[dwCurrentThreadId] += READER_RECURRENCE_UNIT);	
	
	if(READER_RECURRENCE_UNIT == dwThreadState)
	{
		result = CReaderWriterLockNonReentrance::_AcquireReaderLockAndLeaveCS(alTimeOut);	
		
 		if (!result)
		{
			this->ReleaseReaderLock();
 			
		}
	}
	else
	{	
		result = TRUE;
		
		LeaveCriticalSection(&m_cs);
	}
	

	return result ;
}

void CReaderWriterLock::ReleaseReaderLock()
{
	const DWORD dwCurrentThreadId = GetCurrentThreadId();	
	
	EnterCriticalSection(&m_cs);
	
	CMapThreadToState::iterator ite = m_map.find(dwCurrentThreadId);	

	ASSERT( (ite != m_map.end()) );

	ASSERT((READER_RECURRENCE_MASK & ite->second));
	
	const DWORD dwThreadState = (ite->second -= READER_RECURRENCE_UNIT);

	if(0 == dwThreadState)
	{

		m_map.erase(ite);

		CReaderWriterLockNonReentrance::_ReleaseReaderLockAndLeaveCS();
	}
	else
	{
		LeaveCriticalSection(&m_cs);
	}
	//TRACE(_T("%d\r\n"),m_iNumOfReaderEntered);
}

BOOL CReaderWriterLock::TryReleaseReaderAndAcquireWriterLock()
{
	//const DWORD dwCurrentThreadId = GetCurrentThreadId();	
	

	if (this->TryAcquireWriterLock())
	{
		return TRUE;

	}else
	{
		return FALSE;
	}



}

BOOL CReaderWriterLock::AcquireWriterLock(long ulTimeOut)
{
	const DWORD dwCurrentThreadId = GetCurrentThreadId();
	
	BOOL result = FALSE;
	
	EnterCriticalSection(&m_cs);
	
	const DWORD dwThreadState = (m_map[dwCurrentThreadId] += WRITER_RECURRENCE_UNIT);

	if(WRITER_RECURRENCE_UNIT == dwThreadState)
	{
		result = CReaderWriterLockNonReentrance::_AcquireWriterLockAndLeaveCS(m_iNumOfWriter + m_iNumOfReaderEntered,ulTimeOut);

		if (!result)
		{
			this->ReleaseWriterLock();
		}
		
	}
	else if ( (WRITER_RECURRENCE_UNIT*2) <= dwThreadState)
	{
		result = TRUE;
		// This thread was already a writer
		LeaveCriticalSection(&m_cs);
		
	}
	else
	{
		// Upgrading from reader to writer
		result = CReaderWriterLockNonReentrance::_AcquireWriterLockAndLeaveCS(--m_iNumOfReaderEntered,ulTimeOut);
	
		if (!result)
		{
			this->ReleaseWriterLock();
		}
		
	}

	return result;
}

void CReaderWriterLock::ReleaseWriterLock()
{
	const DWORD dwCurrentThreadId = GetCurrentThreadId();

	EnterCriticalSection(&m_cs);
	
	CMapThreadToState::iterator ite = m_map.find(dwCurrentThreadId);

	ASSERT( (ite != m_map.end()) && (WRITER_RECURRENCE_UNIT <= ite->second));

	
	const DWORD dwThreadState = (ite->second -= WRITER_RECURRENCE_UNIT);

	if(0 == dwThreadState)
	{
		m_map.erase(ite);

		CReaderWriterLockNonReentrance::_ReleaseWriterLockAndLeaveCS(FALSE);
	}
	else if (WRITER_RECURRENCE_UNIT <= dwThreadState)
	{
		// Still be is a writer
		LeaveCriticalSection(&m_cs);
	}
	else
	{
		// Down-grading from writer to reader
		CReaderWriterLockNonReentrance::_ReleaseWriterLockAndLeaveCS(TRUE);
	}
}

BOOL CReaderWriterLock::TryAcquireReaderLock()
{
	BOOL blCanRead;
	
	const DWORD dwCurrentThreadId = GetCurrentThreadId();

	EnterCriticalSection(&m_cs);

	CMapThreadToState::iterator ite = m_map.find(dwCurrentThreadId);
	
	if(ite != m_map.end())
	{
		//////////////////////////////////////////////////////////////////////////
		// Current thread was already a WRITER or READER
		ASSERT(0 < ite->second);
		
		LeaveCriticalSection(&m_cs);
		ite->second += READER_RECURRENCE_UNIT;
		blCanRead = true;
	}
	else
	{
		if(0 == m_iNumOfWriter)
		{
			//////////////////////////////////////////////////////////////////////////
			// There is NO WRITER on this RW object
			// Current thread is going to be a READER
			VERIFY(m_map.insert(std::make_pair(dwCurrentThreadId, READER_RECURRENCE_UNIT)).second);
			++m_iNumOfReaderEntered;
			LeaveCriticalSection(&m_cs);
			blCanRead = true;
		}
		else
		{
			//////////////////////////////////////////////////////////////////////////
			// There is some WRITERs on this RW object
			LeaveCriticalSection(&m_cs);
			blCanRead = false;
		}
	}
	
	return blCanRead;
}

BOOL CReaderWriterLock::TryAcquireWriterLock()
{
	bool blCanWrite;
	
	const DWORD dwCurrentThreadId = GetCurrentThreadId();

	EnterCriticalSection(&m_cs);

	CMapThreadToState::iterator ite = m_map.find(dwCurrentThreadId);

	
	if(ite != m_map.end())
	{
		//////////////////////////////////////////////////////////////////////////
		// Current thread was already a WRITER or READER
		ASSERT(0 < ite->second);
		
		if(ite->second >= WRITER_RECURRENCE_UNIT)
		{
			//////////////////////////////////////////////////////////////////////////
			// Current thread was already a WRITER
			LeaveCriticalSection(&m_cs);
			ite->second += WRITER_RECURRENCE_UNIT;
			blCanWrite = true;
		}
		else if(1 == m_iNumOfReaderEntered)
		{
			//////////////////////////////////////////////////////////////////////////
			// This object is owned by ONLY current thread for READ
			// There might be some threads queued to be WRITERs
			// but for effectiveness (higher throughput), we allow current
			// thread upgrading to be WRITER right now
			CReaderWriterLockNonReentrance::_AcquireWriterLockAndLeaveCS(--m_iNumOfReaderEntered);
			ite->second += WRITER_RECURRENCE_UNIT;
			blCanWrite = true;
		}
		else
		{
			LeaveCriticalSection(&m_cs);
			blCanWrite = false;
		}
	}
	else
	{
		if( (0 == m_iNumOfReaderEntered) &&
			(0 == m_iNumOfWriter) )
		{
			//////////////////////////////////////////////////////////////////////////
			// This RW object is not owned by any thread
			// --> it's safe to make this thread to be WRITER
			VERIFY(m_map.insert(std::make_pair(dwCurrentThreadId, WRITER_RECURRENCE_UNIT)).second);
			++m_iNumOfWriter;
			LeaveCriticalSection(&m_cs);
			blCanWrite = true;
		}
		else
		{
			LeaveCriticalSection(&m_cs);
			blCanWrite = false;
		}
	}
	
	return blCanWrite;
}

void CReaderWriterLock::ReleaseAllLocks()
{
	const DWORD dwCurrentThreadId = GetCurrentThreadId();
	
	EnterCriticalSection(&m_cs);

	CMapThreadToState::iterator ite = m_map.find(dwCurrentThreadId);

	if(ite != m_map.end())
	{
		const DWORD dwThreadState = ite->second;
	
		m_map.erase(ite);
	
		if(WRITER_RECURRENCE_UNIT <= dwThreadState)
		{
			CReaderWriterLockNonReentrance::_ReleaseWriterLockAndLeaveCS(FALSE);
		}
		else
		{
			ASSERT(0 < dwThreadState);
			CReaderWriterLockNonReentrance::_ReleaseReaderLockAndLeaveCS();
		}
	}
	else
	{
		LeaveCriticalSection(&m_cs);
	}
}

void CReaderWriterLock::GetCurrentThreadStatus(DWORD* lpdwReaderLockCounter,
											   DWORD* lpdwWriterLockCounter) const
{
	DWORD dwThreadState;

	const DWORD dwCurrentThreadId = GetCurrentThreadId();
	
	EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_cs));

	CMapThreadToState::const_iterator ite = m_map.find(dwCurrentThreadId);
	if(ite != m_map.end())
	{
		LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_cs));
		dwThreadState = ite->second;
		ASSERT(dwThreadState > 0);
	}
	else
	{
		LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_cs));
		dwThreadState = 0;
	}
	
	if(NULL != lpdwReaderLockCounter)
	{
		*lpdwReaderLockCounter = (dwThreadState & READER_RECURRENCE_MASK);
	}
	
	if(NULL != lpdwWriterLockCounter)
	{
		*lpdwWriterLockCounter = (dwThreadState / WRITER_RECURRENCE_UNIT);
	}
}
