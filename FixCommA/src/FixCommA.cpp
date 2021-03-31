//#include <ace/Init_ace.h>
//#include <ace/Log_Msg.h>
//#include <ace/SString.h>
//#include <ace/Thread_Manager.h>
//#include <openssl/crypto.h>
//#include <openssl/x509.h>
//#include <openssl/pem.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>
#include "fixcomma/acoA.h"
#include "fixCommA/FixComma.h"
#include <string>
#include <vector>
using namespace std;


/*static ACE_Thread_Manager* __thrman;

//@brief mutex for openssl threading support
static ACE_Thread_Mutex** __ssl_mtx_vector;

static void ssl_locking_callback(int mode, int type, const char *file, int line){
	if (mode & CRYPTO_LOCK){
		__ssl_mtx_vector[type]->acquire();
	}
	else{
		__ssl_mtx_vector[type]->release();
	}
}

static unsigned long ssl_thread_id(){
	return (unsigned long)__thrman->thr_self();
}

static void aco_init(){
	SSL_load_error_strings();
	SSL_library_init();
	__thrman=ACE_Thread_Manager::instance();

	int nmtx=CRYPTO_num_locks();
	__ssl_mtx_vector=(ACE_Thread_Mutex**)malloc(sizeof(ACE_Thread_Mutex*)*nmtx);
	for(int idx=0;idx<nmtx;++idx){
		__ssl_mtx_vector[idx]=new ACE_Thread_Mutex();
	}

	CRYPTO_set_id_callback(ssl_thread_id);
	CRYPTO_set_locking_callback(ssl_locking_callback);

}

static void aco_fini(){
	int nmtx=CRYPTO_num_locks();
	for(int idx=0;idx<nmtx;++idx){
		delete __ssl_mtx_vector[idx];
	}
	delete[] __ssl_mtx_vector;
}

#ifdef WIN32
#include <windows.h>
bool WINAPI DllMain(HINSTANCE hinstDLL,
					DWORD fdwReason,
					LPVOID lpvReserved){

						switch(fdwReason){
	case DLL_PROCESS_ATTACH:
		aco_init();
		break;
	case DLL_PROCESS_DETACH:
		aco_fini();
		break;
	default:
		break;
						};

						return TRUE;
					}
#else
__attribute__((constructor)) void __aco_constructor(){
	aco_init();
}

__attribute__((destructor)) void __aco_destructor(){
	aco_fini();
}
#endif
*/

CFixCommA::CFixCommA()
{
    _pAco = new ACOA;
}

CFixCommA::~CFixCommA()
{
    delete _pAco;
}

bool CFixCommA::SetItem( int nFixCode , COleDateTime tValue )
{
	char cBuff[100] = {0};
	sprintf( cBuff, "%d-%d-%d %d:%d:%d", tValue.GetYear(), tValue.GetMonth(), tValue.GetDay(), tValue.GetHour(), tValue.GetMinute(), tValue.GetSecond());
	_pAco->SetString( nFixCode, cBuff );
	return true;
}

COleDateTime CFixCommA::GetTime( int nFixCode , int  nRow )
{
   struct tm tmVal;
   tmVal = _pAco->GetTime( nFixCode );
   COleDateTime dtVal(tmVal.tm_year, tmVal.tm_mon, tmVal.tm_mday, tmVal.tm_hour, tmVal.tm_min, tmVal.tm_sec);
   return dtVal;
}

bool  CFixCommA::GetItemBuf( int nFixCode, unsigned char *pBuf, int nSize )
{
/*
   vector<unsigned char> vBuf;
   vBuf = _pAco->GetBytes(nFixCode);
   int iBufSize = vBuf.size();
   if (nSize < iBufSize ) return false;
   for ( int nloop = 0; nloop < iBufSize; ++nloop )
   	   pBuf[nloop] = vBuf[nloop];*/
	int lnRet = _pAco->GetBytesEx(nFixCode,(char *)pBuf,nSize);

	if (lnRet>0)
	{
		return true;
	}

	return false;
}

bool  CFixCommA::GetItemBuf( int nFixCode, char *pBuf, int nSize )
{
	int lnRet = _pAco->GetBytesEx(nFixCode,pBuf,nSize);

	if (lnRet>0)
	{
		return true;
	}

	return false;
}

bool  CFixCommA::HaveItem( int nFixCode, int  nRow )
{
    return _pAco->HasField( nRow);
}

bool CFixCommA::More()
{
   return _pAco->More();
}

bool CFixCommA::MoreEx(){
	return ((ACOA*)_pAco)->MoreEx();
}

void  CFixCommA::SetErrMsg( const char *pszMsg)
{
       
}

long CFixCommA::GetRetCode()
{
  return _pAco->GetRetCode();
}

long  CFixCommA::GetFunc()
{
	return _pAco->GetServiceCode();
}

void  CFixCommA::CreateEndMak()
{
 
}

//=============================================================
//   基本流程函数      < 主要用于客户端编程 >
//=============================================================
// 连接服务器/远程服务器
bool  CFixCommA::Link( const char *pAddr_, const int &iPort_ )
{
	  _pAco->Close();
      return _pAco->Connect( iPort_, pAddr_);
}
// 取出系统出错信息
string CFixCommA::GetLastErrMsg()
{
   return "";
}
// 取出系统返回码，包括通讯出错、 业务出错. 正常大于零。
long  CFixCommA::GetCode()
{
   return _pAco->GetRetCode();
}
// 设置请求功能码，及域个数(可选，如果设置发送时，将判断域个数)
void  CFixCommA::CreateHead ( int nFunc)
{
   _pAco->Clear();
   _pAco->SetServiceCode( nFunc );
   _pAco->InsertRow();
}
// 关闭与服务器的连接
bool  CFixCommA::Close()
{
	return _pAco->Close();
}

 // 取出返回记录数
long  CFixCommA::GetCount()
{
    return _pAco->GetRowCount();
}

 // 请空接收数据缓冲
bool  CFixCommA::Empty()
{
   return _pAco->Clear();
}
// 设置通讯超时秒数
void  CFixCommA::SetTimeOut( long nMaxSec )
{
    _pAco->SetTimeout( nMaxSec );
}


//=============================================================
//   基本流程函数      < 只用于服务端编程 >
//=============================================================
bool   CFixCommA::CreateAnswer( int nFunc , int nWidth )
{
    return false;
}

bool   CFixCommA::CreateAnswerMul ( int nFunc , int nWidth )
{
    return false;
}

bool   CFixCommA::CreateNextAnswer()
{
	return false;
}

//=============================================================
//   设置请求域数据 / 读取结果域数据
//=============================================================
// 取出数据函数 , 如果nRow=-1,返回当前行结果集。
double	CFixCommA::GetDouble( int nFixCode , int nRow )
{
	return _pAco->GetDouble(nFixCode);
}

float	CFixCommA::GetFloat(const int &nFixCode ,const int &nRow )
{
	return _pAco->GetFloat(nFixCode);
}

long CFixCommA::GetLong( int nFixCode , int  nRow )
{
   return _pAco->GetInt( nFixCode );
}

string CFixCommA::Get( int nFixCode , int  nRow )
{
	return _pAco->GetString(nFixCode);
}

// 根据FIX代码取出相应的结果。
string CFixCommA::GetItem( int nFixCode , int  nRow )
{
   return _pAco->GetString(nFixCode);
}

// 设置某请求域浮点性数据 -- 浮点性
bool CFixCommA::SetItem( int nFixCode , double  dValue)
{
    _pAco->SetDouble( nFixCode, dValue);
	return true;
}

// 设置某请求域整性数据   -- 整性
bool CFixCommA::SetItem( int nFixCode , long  nValue )
{ 
	_pAco->SetInt(nFixCode , nValue);
   return true;
}

bool CFixCommA::SetItem( int nFixCode , int nValue )
{
	_pAco->SetInt(nFixCode , nValue);
	return true;
}

// 设置某请求域字符串数据 -- 字符串
bool CFixCommA::SetItem( int nFixCode , const char *pszData  )
{
	_pAco->SetString( nFixCode, pszData );
    return true;
}

bool CFixCommA::SetItemBuf( int nFixCode, char *pBuf, int nSize )
{
	_pAco->SetBytes( nFixCode, pBuf, nSize);
	return true;
}