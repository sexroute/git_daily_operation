#include <StdAfx.h>
#include <additions/CTimeOutReadWriteLock.h>

int CTimeOutReadWriteLock::BeginRead()
{
	BOOL lbRet = this->m_oLock.AcquireReaderLock();

	return lbRet;
}

int CTimeOutReadWriteLock::EndRead()
{
	 this->m_oLock.ReleaseReaderLock();

	return TRUE;
}

int CTimeOutReadWriteLock::BeginWrite()
{
	BOOL lbRet = this->m_oLock.AcquireWriterLock();

	return lbRet;
}

int CTimeOutReadWriteLock::EndWrite()
{
	this->m_oLock.ReleaseWriterLock();

	return TRUE;
}

CTimeOutReadWriteLock::CTimeOutReadWriteLock()
{

}

CTimeOutReadWriteLock::~CTimeOutReadWriteLock()
{
	this->m_oLock.ReleaseAllLocks();
}
