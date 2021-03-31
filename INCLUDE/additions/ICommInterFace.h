#ifndef __ICommInterFace__
#define __ICommInterFace__

#include <atlstr.h>
#include <ATLComTime.h>
#include <FixCommA/FixCommA.h>
#include <FixComm.h>

#include <additions/BHDateTime.h>


class ACO;

#ifdef _PIDMS

class ACI;

#endif

class ICommInterFace
{
public:
	//=============================================================
	//   设置请求域数据 / 读取结果域数据，必须实现
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  )=0;
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  )=0;
	virtual CString Get		         ( int nFixCode , int         nRow=-1  )=0;
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  )=0; // 根据FIX代码取出相应的结果。
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  )=0;
	virtual bool GetItemBuf( int nFixCode, char *pBuf, int nSize ) =0;
	virtual bool SetItemBuf( int nFixCode, char *pBuf, int nSize ) =0;

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   )=0; // 设置某请求域浮点型数据 -- 浮点型
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   )=0; // 设置某请求域整型数据   -- 整型
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   )=0;
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  )=0; // 设置某请求域字符串数据 -- 字符串
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  )=0;
	
	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1)=0;
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1)=0;
	virtual BOOL   CreateNextAnswer()=0;
	virtual void   CreateEndMak()=0;
	virtual long   GetFunc()=0;
	virtual ACO	*  GetAco()=0;
	virtual ACI	*  GetAci()=0;


	virtual int     Write(ACI *pAci=NULL)=0;  // 发送数据

	//=============================================================
	//   设置/得到内部的FixComm指针，可以不实现
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm)=0;  //设置内部的FixComm指针
	virtual BOOL        SetFixCommResponse(void * apFixComm) = 0;
	virtual void * 		GetFixComm()=0;	 //得到内部的FixComm指针
	virtual void *      GetFixCommResponse() = 0; //得到应答FixComm指针


	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  )=0; // 设置某请求域字符串数据 -- 字符串

};



#endif