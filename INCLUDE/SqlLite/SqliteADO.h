#pragma once
#include "SimADO.h"
#include "sqlite3.h"
#include <string>
using namespace std;

#include <afxdisp.h>
#ifdef SIMADO_EXPORTS
#define _DSEADO_APP_    __declspec(dllexport)
#else
#define _DSEADO_APP_    __declspec(dllimport)
#endif

class _DSEADO_APP_  CSqliteADO : public CSimADO
{
public:
	CSqliteADO();
	virtual ~CSqliteADO();
public:
	//链接Sqlite数据库
	virtual BOOL Open(const char* strDB);
	virtual BOOL Exec();
	virtual BOOL Update();
	virtual void Cmd(const char* cmd,...);
	virtual const char* Get(const char* sFieldName);
	virtual const char* Get(int iCol);
	virtual const char * GetString(int col);
	virtual const char * GetString(const char* strField_);
	virtual int GetLong(const char* sFieldName);
	virtual int GetLong(int iCol);
	virtual int GetInt32(int col);
	virtual int GetInt32(const char* strField_);
	virtual double GetDouble(const char* sFieldName);
	virtual double GetDouble(int iCol);
	virtual COleDateTime GetTime(int nIndex);
	virtual COleDateTime GetTime(const char* sFieldName);
	virtual BOOL SetBuffer(int nIndex, byte* pbuf_, long iSize_);
	virtual BOOL GetBuffer(const char * sFieldName, byte* pbuf_, long& iSize_);
	virtual BOOL GetBuffer(int iCol_, byte* pbuf_, long& iSize_);
	virtual bool More();
	virtual BOOL BeginTran();
	virtual BOOL CommitTran();
	virtual BOOL RollbackTran();
	virtual BOOL IsOpen();
	virtual void SetSqlLogPath(const char* sPath_);
	virtual const char* GetLastErrMsg();
	virtual const char* GetSql();
	virtual void Close();
	virtual void ClearSql();
private:
	sqlite3_stmt* m_stmt;
	sqlite3* m_DB;
	CString strSQL;
	int _iBuffused;
	int _iBufflen;
	char* _pSqlbuff;

	///字符串缓存及缓存长度
	char* _pStrBuf;
	int _iStrBuflen;

	TCHAR * _pCmdBuffer;

	CString sPath;
	CString sError;
private:
	int GetFieldIndex(const char* sFieldName);
	void WriteLog(CString strDate_,CString strMsg);
	void SetErrorInformation(CString strErrInfo_);
private:
	char * gbk2utf8(const char *strGBK);
	CStringW UTF8toUTF16(const CStringA& utf8);
    const char * UTF16ToAnsi(const CStringW& utf16_, string& strRet_);
    const char * UTF8ToAnsi(const CStringA& utf8_, string& strRet_);
};