#if !defined(_MBADO__INCLUDED_)
#define _MBADO__INCLUDED_
#pragma once

//#include <afxwin.h>         // MFC 核心和标准组件
//#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <icrsint.h>
#include <atldbcli.h>

#include <string>
#include <fstream>
using namespace std;

#ifdef SIMADO_EXPORTS
    #define _DSEADO_APP_    __declspec(dllexport)
#else
    #define _DSEADO_APP_    __declspec(dllimport)
    #ifdef _DEBUG
       #pragma  comment(lib,"DseAdoD.lib")
    #else
       #pragma  comment(lib,"DseAdo.lib")
    #endif
#endif

#pragma warning(disable:4146)

#define ADO_DB_TYPE_INVALID    (-1)
#define ADO_DB_TYPE_ACCESS     (0)
#define ADO_DB_TYPE_SQLSERVER  (1)
#define ADO_DB_TYPE_MYSQL      (2)

class _DSEADO_APP_ CSimADO
{
public:
	CSimADO();
	virtual ~CSimADO();
public:
	virtual BOOL ExecForMore();
	virtual bool More();
	virtual BOOL ExecuteSQL(const char* strSQL);
	virtual void Close();
	virtual long GetFieldCount();
	///得到记录的条数。注意：这个接口调用后，记录指针就会重新移动到第一条
	virtual long GetRecordCount();
	virtual const char * GetFieldName(long  col);
	virtual int GetLong(int col);
	virtual int GetLong(const char* strField_);
	virtual int GetInt32(int col)=0;
	virtual int GetInt32(const char* strField_)=0;
	virtual double GetDouble(const char* strField_);
	virtual double GetDouble(int col);
	virtual const char * Get(int col);
	virtual const char * Get(const char* strField_);
	virtual const char * GetString(int col)=0;
	virtual const char * GetString(const char* strField_)=0;
	virtual COleDateTime GetTime(const char* strField_);
	virtual COleDateTime GetTime(int nIndex);
	virtual BOOL GetFieldValue(const char* lpFieldName, int& nValue);
	virtual BOOL GetFieldValue(int nIndex, int& nValue);
	virtual BOOL GetFieldValue(const char* lpFieldName, long& lValue);
	virtual BOOL GetFieldValue(int nIndex, long& lValue);
	virtual BOOL GetFieldValue(const char* lpFieldName, unsigned long& ulValue);
	virtual BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
	virtual BOOL GetFieldValue(const char* lpFieldName, double& dbValue);
	virtual BOOL GetFieldValue(int nIndex, double& dbValue);
	virtual BOOL GetFieldValue(const char* lpFieldName, COleDateTime& time);
	virtual BOOL GetFieldValue(int nIndex, COleDateTime& time);
	virtual BOOL GetFieldValue(int nIndex, COleCurrency& cyValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue);
	virtual BOOL GetFieldBuf(const char * lpFieldName, byte * pbuf_, long& iSize_);
	virtual BOOL GetFieldBuf(const int& nIndex, byte * pbuf_, long& iSize_);
	virtual BOOL GetBuffer(const char * lpFieldName, byte * pbuf_, long& iSize_);
	virtual BOOL GetBuffer(const int& nIndex, byte * pbuf_, long& iSize_);
	virtual BOOL SetFieldValue(int nIndex, int nValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, int nValue);
	virtual BOOL SetFieldValue(int nIndex, long lValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, long lValue);
	virtual BOOL SetFieldValue(int nIndex, unsigned long lValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, unsigned long lValue);
	virtual BOOL SetFieldValue(int nIndex, double dblValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, double dblValue);
	virtual BOOL SetFieldValue(int nIndex, const char * strValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, const char * strValue);
	virtual BOOL SetFieldValue(int nIndex, COleDateTime time);
	virtual BOOL SetFieldValue(const char* lpFieldName, COleDateTime time);
	virtual BOOL SetFieldValue(int nIndex, bool bValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, bool bValue);
	virtual BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
	virtual BOOL SetFieldValue(const char* lpFieldName, COleCurrency cyValue);
	virtual BOOL SetFieldBuf(const char * lpFieldName, byte * pbuf_, long iSize_);
	virtual BOOL SetBuffer(const char * lpFieldName, byte * pbuf_, long iSize_);
	virtual BOOL SetBuffer(int nIndex, byte * pbuf_, long iSize_);
	virtual BOOL Exec();
	virtual BOOL Exec(CString strSql_);
	virtual BOOL Update();
	virtual BOOL DbUse(const char* db_);
	//连接Sqlite数据库
	virtual BOOL Open(const char* strDB);
	//连接ACESS数据库
	virtual BOOL Open(const char* strDB, const char* strUID, const char* strPWD);
	//连接SQL数据库
	virtual BOOL Open(const char* strSvr, const char* strUID, const char* strPWD, const char* strDB);
	//链接MySQL数据库
	virtual BOOL Open(const char* strSvr, int  iPort, const char* strUID, const char* strPWD, const char* strDB);
	virtual void CmdEnd();                  //!<根据Cmd的指令,判断语句类型,并自动处理
	virtual void Cmd(const char *cmd,...);
	//transaction support:
	virtual BOOL BeginTran();
	virtual BOOL CommitTran() ;
	virtual BOOL RollbackTran();
	///是否打开
	virtual BOOL IsOpen();
	virtual void SetSqlLogPath(const char* sPath_);
	virtual void ClearSql();
	virtual int GetRecordsAffected();
	virtual const char* GetLastErrMsg();
	virtual const char* GetSql();

	//连接Sqlite数据库
	virtual BOOL Login(const char* strDB);
	//连接ACESS数据库
	virtual BOOL Login(const char* strDB, const char* strUID, const char* strPWD);
	//连接SQL数据库
	virtual BOOL Login(const char* strSvr, const char* strUID, const char* strPWD, const char* strDB);
	//链接MySQL数据库
	virtual BOOL Login(const char* strSvr, int  iPort, const char* strUID, const char* strPWD, const char* strDB);
	virtual void SetRecordBinding(CADORecordBinding* pADOBinding);
	// 获得当前连接的数据库名称
	virtual CString GetDBName(void);
	virtual int GetDBType(void);

public:
	virtual BOOL ExistDB(const char * sDBName);
	virtual BOOL ExistTable(const char * sTableName, const char* sDBName = NULL);

	virtual void CmdUserDB();
	virtual void CmdUserTable(const char * sDBName);

protected:
    ///数据库名称
	CString _strDbName;
    
	///记录集的数量
	unsigned int _iRecordCount;
	///记录的字段数量
	unsigned int _iColumnCount;
};
#endif//_MBADO__INCLUDED_