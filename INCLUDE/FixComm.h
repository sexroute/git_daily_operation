// FixComm.h: interface for the CFixComm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXCOMM_H__5FD3A646_FDB8_477E_8BA4_7E5CD9FCB9A5__INCLUDED_)
#define AFX_FIXCOMM_H__5FD3A646_FDB8_477E_8BA4_7E5CD9FCB9A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "aci.h"
#include "fixcode.h"
#include "fixdef.h"
#include <afxtempl.h>
//	#include <string>
//	using namespace std;

#define	ERROR_LINK	      -301
#define MESSAGE_LINKERR   "连接异常"

#define	ERROR_RELINK	  -302
#define MESSAGE_RELINKERR   "重新连接异常"

#define	ERROR_CONNECT	   -300
#define CONNECT_ERR        "通讯错误"
#define ERROR_RECSIZE      -303
#define ERROR_RECSIZE_MSG  "接收数据域出错"
#define ERROR_RECNO        -304
#define ERROR_RECNO_MSG    "接收数据的记录异常"
#define ERROR_NUM          -305
#define ERROR_NUM_MSG      "发送域个数不正确"

#define ERROR_SENDDOWN     -306
#define MESSAGE_SENDDOWN   "发送数据时连接中断"

#define ERROR_SEND         -307
#define MESSAGE_SEND       "发送数据失败"

#define	ERROR_READTIMEOUT	-308
#define	MESSAGE_READTIMEOUT	"等待数据超时"

#define	ERROR_UNCOMPRESS	-309
#define	MESSAGE_UNCOMPRESS	"解压缩数据失败"

#define	ERROR_UNCOMPRESS_LEN	-310
#define	MESSAGE_UNCOMPRESS_LEN	"解压缩数据长度不符"

#define	ERROR_CONNECT_DB	-311
#define	MESSAGE_ERROR_CONNECT_DB	"获取数据库连接失败"

typedef struct BufInfo
{
	char			  *m_pSendBuf					;	// 发送的数据 
	long			   m_nSendSize					;	// 发送的数据大小
	long			   m_nSendFixID					;	// 发送的FIXID	
}BufInfo;

typedef CTypedPtrArray <CPtrArray, BufInfo * >  CBufInfo;

class CAnswer{
public:
  CStringArray arValue ;
  CDWordArray  arCode  ;
  CAnswer(){}
  virtual    ~CAnswer(){
	   arValue.RemoveAll();
	   arCode .RemoveAll();
  }
};
typedef CTypedPtrArray <CPtrArray, CAnswer  * >  CAnswerArray;
class AFX_EXT_CLASS CFixComm 
{
public:
	void CancelRead();
	BOOL IsDead();
	void FreeBufInfo();
	void SetCompressLevel(int level);
	BOOL SetItem( int nFixCode , COleDateTime        tValue   );
	COleDateTime GetTime( int nFixCode , int         nRow=-1  );
	bool GetItemBuf( int nFixCode, char *pBuf, int nSize );
	bool	GetDebugState();
	void	SetDebugState(bool bDebug);
	bool	GetAllItem(CDWordArray &arID,CStringArray &arValue,int nRow=-1);
	bool	HaveItem( int nFixCode, int  nRow=-1  );
	bool	AnalyReq();
	void	SetRouteID( long nRouteID );
	long	GetRouteID();
	BOOL	More();
	void	SetErrMsg( const char *pszMsg);

//	BOOL	setString( int FixCode, const string &value);
//	string	getString( int FixCode , int nRow=-1);

	CString GetUser();
	CString	GetDestBranch();
	CString	GetBranch();
	CString	GetNode();
	CString	GetLY();

	CString GetRetNote();
	long	GetRetCode();
	long	GetFunc();
	BOOL	Init();
	void	CreateEndMak();
	void	SetIsSvr( BOOL bSvr );
    ACI		*GetAci();
	
    //=========================================================
	//  标准包头设置函数
	//=========================================================
	void    SetVersion ( const char *pszVersion );  // 设置版本号
	void    SetDestDept ( const char *pszDestDept );  // 设置目标营业部编号(供应用路由使用)
	void    SetBranch    ( const char *pszDept     );  // 设置操作营业部
	void    SetQuestWay( const char *pszWay     );  // 设置委托方式 
	void    SetUser    ( const char *pszUser    );  // 设置柜员号
	void    SetNode    ( const char *pszNode    );  // 设置操作网卡
  
    //=============================================================
	//   基本流程函数      < 主要用于客户端编程 >
	//=============================================================
	BOOL    Link       ( const char *pszDest, const char *pszDept  );  // 连接服务器/远程服务器
	CString GetLastErrMsg                                        ();  // 取出系统出错信息
	long    GetCode                                              ();  // 取出系统返回码，包括通讯出错、 业务出错. 正常大于零。
	void    CreateHead ( int nFunc          , int nWidth=-1       );  // 设置请求功能码，及域个数(可选，如果设置发送时，将判断域个数)
    virtual BOOL    Close                                                ();  // 关闭与服务器的连接
	long    GetCount                                             ();  // 取出返回记录数
	int     Read                                   (ACI *pAci=NULL);  // 读取数据
	int     Write                                  (ACI *pAci=NULL);  // 发送数据
	BOOL    Empty                                                ();  // 请空接收数据缓冲
	void    SetTimeOut ( long nMaxSec                             );  // 设置通讯超时秒数	

     
    //=============================================================
	//   基本流程函数      < 只用于服务端编程 >
	//=============================================================
	BOOL   SetAci          ( ACI *pAci );
	BOOL   CreateAnswer    ( int nFunc , int nWidth =-1);
	BOOL   CreateAnswerMul ( int nFunc , int nWidth =-1);
    BOOL   CreateNextAnswer();

    //=============================================================
	//   设置请求域数据 / 读取结果域数据
	//=============================================================
	double	GetDouble   ( int nFixCode , int         nRow=-1  );
	long	GetLong    ( int nFixCode , int         nRow=-1  );
	CString Get		   ( int nFixCode , int         nRow=-1  );
	CString GetItem    ( int nFixCode , int         nRow=-1  ); // 根据FIX代码取出相应的结果。
                                                                // 取出数据函数 , 如果nRow=-1,返回当前行结果集。

	BOOL    SetItem    ( int nFixCode , double      dValue   ); // 设置某请求域浮点性数据 -- 浮点性
	BOOL    SetItem    ( int nFixCode , long        nValue   ); // 设置某请求域整性数据   -- 整性
	BOOL    SetItem    ( int nFixCode , int         nValue   );
	BOOL    SetItem    ( int nFixCode , const char *pszData  ); // 设置某请求域字符串数据 -- 字符串

	CFixComm();
	virtual    ~CFixComm()   ;
	
	CAnswerArray       m_arAnswer					;	// 请求数据缓冲
	CAnswer            m_Query						;	// 接收数据缓冲
	char 		      *m_pData                      ;	// 数据区
	long			   m_nDataSize					;
	CString            m_sSvr                       ;
	CStringArray	   m_arDebugData				;

	//## ##############################################################
	//##
	//##【 发送一些流数据 】 暂时的解决方法
	//##
	//## 以下只对文件发送有意义	
	//## 一般情况下不推荐用以下属性 
	//##					     2002.07.04

    CBufInfo          m_arBufInfo;                  ;   // 流数据集,以下3字段作废
	char			  *m_pSendBuf					;	// 发送的数据 
	long			   m_nSendSize					;	// 发送的数据大小
	long			   m_nSendFixID					;	// 发送的FIXID	
	bool			   SetItemBuf( int nFixCode, char *pBuf, int nSize );
	//## ###############################################################

protected:	
	CString m_sHead                                 ;  // 标准数据包头
    int    ReadAnswer ( ACI *pAci, CAnswer *pAnswer ,int nMaxSec=0           );  // 读出数据               
	ACI                       *m_pAci               ;  // 通讯指针
	BOOL                        m_bMulti            ;  // 用于服务端指明返回数据是否多行
	int							m_nAnswerNo         ;  // 用于服务端指明多行返回数据的组号
	BOOL                        m_bSvr              ;
	int							m_nTimeOut			;
	bool						m_bDebug			;
	
protected:
    char			m_cFieldSplit                ; // 域分隔符
	char			m_cSubFieldSplit             ; // 子域分隔符
    BOOL			m_bMutil                     ; // 系统内部使用是否是多行返回
	CString			m_sErrMsg                    ; // 错误返回码
	long			m_nCount                     ; // 系统返回记录数量	
	int				m_nNo                        ; // 返回记录时，记录号
	long			m_nMaxSec                    ; // 最大超时 
	long			m_nFunc                      ; // 系统请求功能号
	long			m_nErrCode                   ; // 系统返回值
	int				m_nWidth                     ;
    CString			m_sVersion                   ; // 版本号  
    int				m_nStatus                    ; //
	long			m_nRouteID					 ; // 应用路由ID标识
	long			m_nLastRouteID				 ; // 
    CString			m_sDest , m_sRemote          ; // 直接连接的IP/SPX地址
	CString			m_sUser , m_sNode  ,m_sLY  ; // 柜员号、网卡地址、委托来源、
	CString			m_sBranch , m_sDestBranch        ; // 柜员所在营业部编号、目标营业部编号(可以路由到异地)。    
	CStringArray	m_arSvrList					 ; // 服务器列表
	int				m_nSvrIndex					 ;
    int             m_nCompressLevel;
    char            *m_pData1                  ;//保存压缩前数据
    char            *m_pData2                  ;//保存压缩后数据
    char			m_cBodySplit               ;// 包文分隔符,分隔包头及包文
private:
//	bool            bInReadAnswer;				//用于控制退出读取
//	bool			bCancelRead;				//用于控制退出读取
};

#endif // !defined(AFX_FIXCOMM_H__5FD3A646_FDB8_477E_8BA4_7E5CD9FCB9A5__INCLUDED_)
