#ifndef _CTImeOutReadWriteLock_
#define _CTImeOutReadWriteLock_
#include "IReadWriteLock.h"
#include "ReaderWriterlock.h"

class CTimeOutReadWriteLock : public IReadWriteLock
{
public:
	
	virtual int BeginRead();
	virtual int EndRead();
	virtual int BeginWrite() ;
	virtual int EndWrite() ;	
	CTimeOutReadWriteLock();
	virtual ~CTimeOutReadWriteLock();
public:
	CReaderWriterLock m_oLock;
};
#endif _CTImeOutReadWriteLock_