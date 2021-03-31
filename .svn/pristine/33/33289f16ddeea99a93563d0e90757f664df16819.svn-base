#pragma once
#ifndef _M_BHKD_SERVER_SIDE_
#include <additions\icomminterface.h>
typedef std::vector<BYTE> _COMM_BUFFER;
class FixCommRow
{
public:
	std::map<int,int> m_oIntValues;
	std::map<int,CString> m_oCStringValues;
	std::map<int,CBHDateTime> m_oDateTimeValues;
	std::map<int,_COMM_BUFFER> m_oBufferValues;
	std::map<int,string> m_oStdStringValues;
	std::map<int,double> m_oDblValues;
	virtual ~FixCommRow()
	{
		
	}
};

enum EN_FIXCOMM_TYPE
{
	EN_FIXCOMM_TYPE_ACE  = 0,  ///表示使用的是基于ACE库的中间件平台的通讯库
	EN_FIXCOMM_TYPE_MFC  = 1,   ///表示旧版中间件，主要用MFC的库和Window的API写的
	EN_FIXCOMM_TYPE_FAKE = 2
};

enum CommMode
{
	REQUEST = 0,
	RESPONSE = 1
};

class CFakeComm :
	public ICommInterFace
{
public:
	CFakeComm(void);
	virtual ~CFakeComm(void);
	virtual double	GetDouble      ( int nFixCode , int         nRow=-1  );
	virtual long	GetLong          ( int nFixCode , int         nRow=-1  );
	virtual CString Get		         ( int nFixCode , int         nRow=-1  );
	virtual CString GetItem        ( int nFixCode , int         nRow=-1  ); // 根据FIX代码取出相应的结果。
	virtual CBHDateTime GetTime   ( int nFixCode , int         nRow=-1  );
	virtual bool GetItemBuf( int nFixCode, char *pBuf, int nSize ) ;
	virtual bool SetItemBuf( int nFixCode, char *pBuf, int nSize ) ;

	virtual BOOL    SetItem    ( int nFixCode , double      dValue   ); // 设置某请求域浮点型数据 -- 浮点型
	virtual BOOL    SetItem    ( int nFixCode , long        nValue   ); // 设置某请求域整型数据   -- 整型
	virtual BOOL    SetItem    ( int nFixCode , int         nValue   );
	virtual BOOL    SetItem    ( int nFixCode , const char *pszData  ); // 设置某请求域字符串数据 -- 字符串
	virtual BOOL    SetItem    ( int nFixCode , CBHDateTime aoTime  );

	virtual BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
	virtual BOOL   CreateNextAnswer();
	virtual void   CreateEndMak();
	virtual long   GetFunc();
	virtual ACO	*  GetAco();
	virtual ACI	*  GetAci();


	virtual int     Write(ACI *pAci=NULL);  // 发送数据

	//=============================================================
	//   设置/得到内部的FixComm指针，可以不实现
	//=============================================================
	virtual BOOL 		SetFixComm(void * apFixComm);  //设置内部的FixComm指针
	virtual BOOL        SetFixCommResponse(void * apFixComm) ;
	virtual void * 		GetFixComm();	 //得到内部的FixComm指针
	virtual void *      GetFixCommResponse() ; //得到应答FixComm指针


	virtual BOOL      SetTime    ( int nFixCode , CBHDateTime aoTime  ); // 设置某请求域字符串数据 -- 字符串
	virtual bool    More();
	virtual void    CreateHead ( int nFunc          , int nWidth=-1);
	EN_FIXCOMM_TYPE GetFixCommType();
	virtual void ResetRequestData();
	virtual void ReestResponseData();
	void    SetTimeOut( long nMaxSec ); 
public:
	std::vector<FixCommRow> m_oRequest;
	std::vector<FixCommRow> m_oResponse;
private:
	EN_FIXCOMM_TYPE m_enFixCommType;
	void * m_pFixComReqeust;
	void * m_pFixComResponse;
	//状态
	bool m_bIdle;
	int m_noRequestDataIndex;
	int m_noResponseDataIndex;
	long m_nFuncCode;
	CommMode m_nCommMode;

};

extern "C" __declspec(dllexport) BOOL CreateFakeFixComm(CFakeComm ** appFixComm);
extern "C" __declspec(dllexport) BOOL DeleteFakeFixComm(CFakeComm * appFixComm);

extern "C" void * Service();
extern "C" bool Do( CFakeComm * pFixComm );
#endif
