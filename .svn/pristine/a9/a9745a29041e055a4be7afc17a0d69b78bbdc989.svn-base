#ifndef _BHCommWrapper_
#define _BHCommWrapper_
#include "ICommInterFace.h"
#include "aco/ACO.h"

class CBHCommWrapper : public ICommInterFace
{
public:
	//=============================================================
	//   设置请求域数据 / 读取结果域数据，必须实现
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  );
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  );
	virtual CString Get		         ( int nFixCode , int         nRow=-1  );
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  ); // 根据FIX代码取出相应的结果。
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  );

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   ); // 设置某请求域浮点性数据 -- 浮点性
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   ); // 设置某请求域整性数据   -- 整性
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   );
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  ); // 设置某请求域字符串数据 -- 字符串
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  );


	virtual bool	GetItemBuf( int nFixCode, char *pBuf, int nSize ) ;
	virtual bool	SetItemBuf( int nFixCode, char *pBuf, int nSize ) ;

	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateNextAnswer();
	virtual void   CreateEndMak();

	virtual long   GetFunc();
	virtual ACI	*  GetAci();
	virtual ACO	*  GetAco();
	virtual int    Write                                  (ACI *pAci=NULL);  // 发送数据

	//=============================================================
	//   设置/得到内部的FixComm指针，可以不实现
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm);  //设置内部的FixComm指针
	virtual BOOL        SetFixCommResponse(void * apFixComm);
	virtual void * 		GetFixComm();
	virtual void *      GetFixCommResponse();

	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  ); // 设置某请求域字符串数据 -- 字符串
	CBHCommWrapper();
	virtual ~CBHCommWrapper();
private:
	CFixComm * m_pFixComm;
	int m_nFixCommType;
};

class CBHCommWrapperA : public ICommInterFace
{
public:
	//=============================================================
	//   设置请求域数据 / 读取结果域数据，必须实现
	//=============================================================
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  );
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  );
	virtual CString Get		         ( int nFixCode , int         nRow=-1  );
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  ); // 根据FIX代码取出相应的结果。
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  );

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   ); // 设置某请求域浮点性数据 -- 浮点性
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   ); // 设置某请求域整性数据   -- 整性
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   );
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  ); // 设置某请求域字符串数据 -- 字符串
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  );

	virtual bool	GetItemBuf( int nFixCode, char *pBuf, int nSize ) ;
	virtual bool	SetItemBuf( int nFixCode, char *pBuf, int nSize ) ;

	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateNextAnswer();
	virtual void   CreateEndMak();

	virtual long   GetFunc();
	virtual ACI	*  GetAci();
	virtual ACO	*  GetAco();
	virtual int    Write                                  (ACI *pAci=NULL);  // 发送数据

	//=============================================================
	//   设置/得到内部的FixComm指针，可以不实现
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm);  //设置内部的FixComm指针
	virtual BOOL        SetFixCommResponse(void * apFixComm);
	virtual void * 		GetFixComm();	
	virtual void *      GetFixCommResponse();
    
	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  ); // 设置某请求域字符串数据 -- 字符串

	CBHCommWrapperA();
	virtual ~CBHCommWrapperA();
private:
	ACO * m_pFixComm;
	ACO * m_pFixCommResponse;
	int m_nFixCommType;
};

#endif
