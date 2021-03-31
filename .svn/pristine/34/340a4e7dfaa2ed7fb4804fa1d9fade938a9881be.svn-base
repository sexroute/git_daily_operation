#pragma once
#ifndef __MUTEX_READERS_WRITERS_MARBLE_
#define __MUTEX_READERS_WRITERS_MARBLE_

class _KDCOMMLIB_API_ CMutexRW
{
protected:
	HANDLE		m_semReaders;
	HANDLE		m_semWriters;
	int			m_nReaders;
public:
	CMutexRW();
	virtual ~CMutexRW();

	void BeginRead();
	void EndRead();

	void BeginWrite();

	void EndWrite();
};

#endif // end #ifndef __MUTEX_READERS_WRITERS_MARBLE_
