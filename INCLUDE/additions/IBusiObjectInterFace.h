#pragma once
#ifndef	  _IBHBusinessObject_
#define   _IBHBusinessObject_
#include "ICommInterFace.h"
#include "IDBInterFace.h"
class IBHBusinessObject
{
public:
	virtual BOOL CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)=0;
	virtual BOOL PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)=0;
	virtual BOOL ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)=0;
	virtual int WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)=0;
	virtual int Do(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)=0;
};

#endif