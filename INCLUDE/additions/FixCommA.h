#ifndef _FIXCOMMA_H_BH_
#define _FIXCOMMA_H_BH_

#include <Windows.h>
#include <atlcomtime.h>
#include <string>
using namespace std;
#ifdef FIXCOMMA_EXPORTS
#define EXPORT_CFIXCOMMA __declspec(dllexport)
#else
#define EXPORT_CFIXCOMMA __declspec(dllimport)
#ifdef _DEBUG
#pragma  comment(lib,"FixCommAD.lib")
#else
#pragma  comment(lib,"FixCommA.lib")
#endif
#endif
class ACOBase;
class EXPORT_CFIXCOMMA CFixCommA
{
public:
	CFixCommA();
	virtual ~CFixCommA();
	void SetCompressLevel(int level);
	bool SetItem( int nFixCode , COleDateTime tValue   );
	COleDateTime GetTime( int nFixCode , int  nRow=-1  );
	bool    GetItemBuf( int nFixCode, unsigned char *pBuf, int nSize );
	bool    GetItemBuf( int nFixCode, char *pBuf, int nSize );
	bool	HaveItem( int nFixCode, int  nRow=-1  );
	
	///返回值true：结果集；false：无结果集（第一次调用出现false时，也可能是中间件出现问题）。
	bool	More();

	///第一次调用时，如果返回true表示，与中间件通讯成功，可能存在无结果集，所以获取结果前，必须调用GetCount判断是否有结果。
	///其他调用时，返回false表示没有结果
	bool    MoreEx();
	
	void	SetErrMsg( const char *pszMsg);

	string  GetRetNote();
	long	GetRetCode();
	long	GetFunc();
	void	CreateEndMak();
	//=============================================================
	//   基本流程函数      < 主要用于客户端编程 >
	//=============================================================
	bool    Link( const char *pAddr_, const int &iPort_ );      // 连接服务器/远程服务器
    string GetLastErrMsg();                                    // 取出系统出错信息
	long    GetCode ();                                         // 取出系统返回码，包括通讯出错、 业务出错. 正常大于零。
	void    CreateHead ( int nFunc);                            // 设置请求功能码，及域个数(可选，如果设置发送时，将判断域个数)
	bool    Close();                                            // 关闭与服务器的连接
	long    GetCount();                                         // 取出返回记录数
	bool    Empty();                                            // 请空接收数据缓冲
	void    SetTimeOut( long nMaxSec );                         // 设置通讯超时秒数	


	//=============================================================
	//   基本流程函数      < 只用于服务端编程 >
	//=============================================================
	bool   CreateAnswer    ( int nFunc , int nWidth =-1);
	bool   CreateAnswerMul ( int nFunc , int nWidth =-1);
	bool   CreateNextAnswer();

	//=============================================================
	//   设置请求域数据 / 读取结果域数据
	//=============================================================
	double	GetDouble  ( int nFixCode , int         nRow=-1  );
	float	CFixCommA::GetFloat(const int &nFixCode ,const int &nRow = -1 );
	long	GetLong    ( int nFixCode , int         nRow=-1  );
	string  Get   ( int nFixCode , int         nRow=-1  );
	string  GetItem    ( int nFixCode , int         nRow=-1  ); // 根据FIX代码取出相应的结果。
	// 取出数据函数 , 如果nRow=-1,返回当前行结果集。

	bool    SetItem( int nFixCode , double      dValue   ); // 设置某请求域浮点性数据 -- 浮点性
	bool    SetItem( int nFixCode , long        nValue   ); // 设置某请求域整性数据   -- 整性
	bool    SetItem( int nFixCode , int         nValue   );
	bool    SetItem( int nFixCode , const char *pszData  ); // 设置某请求域字符串数据 -- 字符串
	bool	SetItemBuf( int nFixCode, char *pBuf, int nSize );
private:
	ACOBase *_pAco;
};
#endif//_FIXCOMMA_H_BH_
