#pragma once
#include "Glbs.h"
#include <map>
#include <objbase.h>

#include <additions/ReaderWriterlock.h>

/*
	Buffer数据缓存类
	线程安全
*/
typedef struct CUSTOM_GUID : GUID
{
public:

	bool operator <(const CUSTOM_GUID & loTemp) const;	

	CUSTOM_GUID & operator = (const GUID & loGuid) ;	


} CUSTOM_GUID;
bool CUSTOM_GUID::operator <(const CUSTOM_GUID & loTemp) const
{
	if (this->Data1 == loTemp.Data1)
	{
		if (this->Data2 == loTemp.Data2)
		{
			if (this->Data3 == loTemp.Data3)
			{
				int * p1 = (int *)this->Data4;

				int * p2 = (int *)loTemp.Data4;

				if (*p1 == * p2)
				{
					p1 = (int *)&this->Data4[4];

					p2 = (int *)&loTemp.Data4[4];

					return (*p1) < (*p2);

				}else
				{
					return (*p1) < (*p2);
				}

			}else
			{
				return this->Data3 < loTemp.Data3;
			}

		}else
		{
			return this->Data2 < loTemp.Data2;
		}

	}else
	{
		return this->Data1 < loTemp.Data1;
	}

	return FALSE;
}

CUSTOM_GUID & CUSTOM_GUID::operator =(const GUID & loGuid)
{
	memcpy(this,&loGuid,sizeof(GUID));

	return * this;
}
typedef  std::map<CUSTOM_GUID,BUFFER>  BUF_MAP;
class CBufferCache
{
public:
	CBufferCache(void);
	~CBufferCache(void);

	static int Clear(const CUSTOM_GUID& key)
	{
		CBufferCache::_lock.AcquireWriterLock();
		int nRet = -1;
		BUF_MAP::iterator it = 
			_mapBuffer.find(key);
		if (_mapBuffer.end() != it)
		{
			it->second.clear();
			nRet = 0;
		}
		CBufferCache::_lock.ReleaseWriterLock();
		return nRet;
	}

	static int GetBuffer(const CUSTOM_GUID& key,BUFFER& buff_)
	{
		int nRet = -1;
		CBufferCache::_lock.AcquireReaderLock();
		BUF_MAP::iterator it = 
			_mapBuffer.find(key);

		if (_mapBuffer.end() != it)
		{
			buff_.clear();
			buff_.insert(buff_.begin(),it->second.begin(),it->second.end());
			nRet = 0;
		}
		else
		{
			ASSERT(FALSE);
		}
		CBufferCache::_lock.ReleaseReaderLock();
		return nRet;
	}

	static int AppendBuffer(const CUSTOM_GUID& key,const BUFFER& buff_)
	{
		CBufferCache::_lock.AcquireWriterLock();
		int nRet = -1;
		BUF_MAP::iterator it = _mapBuffer.find(key);

		if (_mapBuffer.end() == it)
		{
			BUFFER temp;
			if(!CBufferCache::_mapBuffer.insert(make_pair(key,temp)).second)
			{
				ASSERT(FALSE);
			}
		}
		it = _mapBuffer.find(key);
		if (_mapBuffer.end() != it)
		{
			it->second.insert(it->second.end(),buff_.begin(),buff_.end());
			nRet = 0;
		}
		else
		{
			ASSERT(FALSE);
		}
		CBufferCache::_lock.ReleaseWriterLock();
		return nRet;
	}
	static int RemoveEnd(const CUSTOM_GUID& key,int position)
	{
		// 删除指定位置以及以后的所有元素，避免客户端向中间件传递重复数据,

		CBufferCache::_lock.AcquireWriterLock();
		int nRet = -1;
		BUF_MAP::iterator it = _mapBuffer.find(key);

		if (it != _mapBuffer.end())
		{
			BUFFER::iterator itTemp = it->second.begin() + position;
			it->second.erase(itTemp,it->second.end());
			nRet = 0;
		}
		CBufferCache::_lock.ReleaseWriterLock();
		return nRet;
	}
private:
	static BUF_MAP _mapBuffer;
	static CReaderWriterLock  _lock;
};
BUF_MAP CBufferCache::_mapBuffer;
CReaderWriterLock  CBufferCache::_lock;
