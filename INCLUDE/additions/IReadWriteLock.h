#ifndef __IReadWriteLock__
#define __IReadWriteLock__

class IReadWriteLock
{
public:
	virtual int BeginRead() = 0;
	virtual int EndRead() =0;
	virtual int BeginWrite() = 0;
	virtual int EndWrite() =0;	
	virtual ~IReadWriteLock(){};
};


#endif