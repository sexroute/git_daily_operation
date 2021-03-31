#pragma once
/*
* Copyright (c) 2010, BH 网软件部
* All rights reserved.
* 
* 文件名称： ZdlDbWrapper.cpp
* 摘    要： oledb 数据库封装类
* 作    者： 赵大力
* 修 改 者1：赵大力 修改时间：2010/10/15 修改内容：仅支持读操作，不支持写操作，支持一次读取多个blob数据
* 版    本： 4300b* 
*/
#include <CustomDB.h>
#include <atldbcli.h>
#include "IDBInterFace.h"


#ifndef __BH_OLE_SQL_SERVER_DB__
#define __BH_OLE_SQL_SERVER_DB__


#define _USE_OLE_DB_DIRECT_ 1



// CZdlDbWrapper 命令目标
class COleSqlServerDB : public IDBInterFace
{
public:
	COleSqlServerDB();
	virtual ~COleSqlServerDB();
	enum DBSERVER_SERVER_TYPE
	{
		DB_SERVER_TYPE_SQL2000 = 2000,
		DB_SERVER_TYPE_SQL2005 = 2005,
		DB_SERVER_TYPE_SQL2008 = 2008,
		DB_SERVER_TYPE_SQL7 = 7,
		DB_SERVER_TYPE_SQL65 = 65,
	};
private:
	CCustomDB * m_pDb;

public:
	//设置参数或信息
	virtual void setPDB(CCustomDB * apDb);
	virtual CCustomDB * getPDB();

	//!<设置修改后是否自动更新结果集
	virtual void SetAutoUpdateRst(bool bAuto)
	{
		//没有实现
		ASSERT(FALSE);
	}; 

	//!<设置是否自动提交事务
	virtual void SetAutoCommit(bool autoCommit)
	{
		//没有实现
		ASSERT(FALSE);
	}; 

	//!<设置SQL指令类型
	virtual void SetCommandType(CommandTypeEnum intCommandType)
	{		//没有实现
		ASSERT(FALSE);
	};  

	///<设置自定义错误信息
	virtual void SetErrMsg(const char *pszMsg)

	{		//没有实现
		ASSERT(FALSE);
	}; 

	///<设置数据库连接串
	virtual void SetConnectionString(LPCTSTR lpstrConnectionString)
	{
		//没有实现
		ASSERT(FALSE);
	};    


	//取类运行信息
	///<取数据库连接指针
	virtual _ConnectionPtr GetConn()
	{		//没有实现
		ASSERT(FALSE);
		return false;
	};

	///<取自定义错误信息
	virtual CString GetLastErrMsg();
	//!<取上一次SQL错误代码
	virtual DWORD GetLastError();


	//!<取数据库连接串
	virtual CString GetConnectionString()
	{
		//没有实现
		ASSERT(FALSE);
		return _T("");
	}; 

	//!<取上一次SQL命令影响的行数
	virtual int GetRecordsAffected()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};      


	//Rst操作
	//!<在结果集中增加新的记录
	virtual bool AddNewRecord()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	}; 

	//!<删除结果集中的当前记录
	virtual bool DelCurrentRecord()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};  

	//!<取消结果集中目前所前所做的修改
	virtual bool CancelUpdateRst()
	{
		return false;
	}; 

	//!<提交结果集中当前的修改
	virtual bool UpdateRst()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};               


	virtual bool SetFieldValue(int id, CString sValue);             //!<设置结果集中字符型字段值
	virtual bool SetFieldValue(const char *name, CString sValue);   //!<设置结果集中字型字段值
	virtual bool SetFieldValue(const char *name,const char *value); //!<设置结果集中字符型字段值
	virtual bool SetFieldValue(const char *name, double dblValue);  //!<设置结果集中双精度型字段值
	virtual bool SetFieldValue(const char *name, long lValue);      //!<设置结果集中长整型字段值
	virtual bool SetFieldValue(const char *name, CBHDateTime time);//!<设置结果集中日期型字段值
	virtual bool SetFieldValue(const char *name, int nValue);       //!<设置结果集中整型字段值
	virtual bool SetFieldValue(const char *name, _variant_t vtValue);   //!<设置结果集字段值


	//!<取结果集返回列数
	virtual long GetNumCols()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};   

	//!<取对应列的定义长度
	virtual long GetColLen(int index)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};   

	//!<取对应列的字段名
	virtual char * GetColName(const int id)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};   

	//!<根据据参数,返回结果集记录数
	virtual long GetCount(int nType = 0);    


	//!<返回结果集记录数
	virtual DWORD GetRecordCount()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};            

private:
	virtual CString Get(int id,BOOL abTestEncode = FALSE);
	virtual string Get(int id);
	virtual bool GetBuffer( int nFieldIndex,BYTE *pBuf,long &nSize, CWaveHead& sWaveHead_);    //!<取结果集中字段数至缓冲区
	virtual long GetActualSize(int nFieldIndex);    //!<返回结果集中字段实际长度
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize );  //!<用缓冲区数据,更新结果集中字段数据
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize, bool abEncrypt );  //!<用缓冲区数据,更新结果集中字段数据
	virtual CBHDateTime GetTime(int id);           //!<根据字段索引取日期型字段值
	virtual long GetLong(int id);               //!<根据字段索引取整型字段值
	virtual double GetDouble(int id);           //!<根据字段索引取双精度型字段值


public:	//数据访问
	virtual long GetActualSize(const char *name);   //!<返回结果集中字段实际长度
	virtual bool GetBuffer(const char *name,BYTE *pBuf, long &nSize, CWaveHead& sWaveHead_);   //!<取结果集中字段数至缓冲区
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize ); //!<用缓冲区数据,更新结果集中字段数据
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize,bool abEncrypt ); //!<用缓冲区数据,更新结果集中字段数据

	virtual string Get(const char *name,CString strDateFormat = CString(""));
	virtual CString GetAsCString(const char *name);
	virtual CBHDateTime GetTime(const char *name); //!<根据字段名取日期型字段值
	virtual long GetLong(const char *name);     //!<根据字段名取整型字段值
	virtual double GetDouble(const char *name); //!<根据字段名取双精度型字段值


	//运行
	virtual bool More(BOOL abUpdatableRowSet = FALSE);                    //!<移动结果集的记录至下一行
	virtual void Cancel(){};                  //!<取消当前正在执行的命令
	virtual bool Exec();                    //!<执行Sql语名
	virtual bool ExecForMore(){return false;};             //!<执行Sql语名,默认被More调用
	virtual bool DbUse(const char *db);     //!<切换当前数据库
	virtual void CmdEnd(){};                  //!<根据Cmd的指令,判断语句类型,并自动处理

	//!<建立数据库连接
	virtual bool Open(const char *svr, const char *usr, const char *pwd, const char *db)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	}; 

	virtual long IsOpen()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual void __cdecl Cmd(const char *cmd,...); 
	virtual void Close(); 
	virtual void SetSqlLogPath(CString lstrPath);



	//存储过程相关
	virtual CBHDateTime GetOleDateTimeParamValue(const char *name)
	{
		//没有实现
		ASSERT(FALSE);
		return CBHDateTime();
	};
	virtual _variant_t GetVariantParamValue(const char *name)
	{
		//没有实现
		ASSERT(FALSE);
		return CComVariant();
	};
	virtual double GetDoubleParamValue(const char *name)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual long GetLongParamValue(const char *name)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual char * GetParamValue(const char *name,CString strDateFormat = CString(""))
	{
		//没有实现
		ASSERT(FALSE);
		return NULL;
	};

	virtual bool SetParamValue(const char *name, CBHDateTime time)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, _variant_t vtValue)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, long lValue)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,double dblValue)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,const char *value)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,CString strValue)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual long GetReturnValue()
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, int nValue)
	{
		//没有实现
		ASSERT(FALSE);
		return false;
	};

	virtual bool ExecSetBuffer(const char *name, BYTE *pBuf, long nSize,const char *tablename,
		const char* sqlwhere);
	//{
	//	//没有实现
	//	ASSERT(FALSE);
	//	return false;
	//}


	//事务相关
	virtual long BeginTran(); 
	virtual long CommitTran(); 
	virtual long RollbackTran(); 


	virtual void MoveFirst(){}; //前移记录到记录头
	virtual void MoveNext(){};  //后移记录一条
	virtual void MovePrevious(){}; //前移记录一条
	virtual void MoveLast(){}; //后移记录至记录末
	virtual bool Move(int NumRecords)
	{ 
		return false;
	};

	virtual CString GetSql();
	virtual void ClearSql();
	virtual BOOL Login(const char *pszSvr , const char *pszID, const char *pszPwd , const char *pszDB);

	virtual CString GetOleDbSql();
	virtual void ClearReSource();

	virtual BOOL IsUseOleDB();
	virtual BOOL setUseOleDB(BOOL abUseOleDb);


	virtual CString GetLastErrorString();

	virtual BOOL IsDbSafe(BOOL abForceCheckDbSql = FALSE);

	virtual BOOL JudgeDbType();

	virtual INT GetDBServerType();

	
protected:
	BOOL m_bConnected;
	BOOL m_bUseOleDB;
	static volatile INT m_nDbServerType;
	CDataSource m_oDb;
	CSession * m_pDbSession;
	CCommand<CDynamicAccessor,CRowset> *  m_pResultSet;

	CString m_strDBServer;
	CString m_strDBPwd;
	CString m_strDBUser;
	CString m_strDBCataLog;
	CString m_strOleDBSql;
	CString m_strSqlLogPath;
	ULONG   m_ulOleDbDataCount;
	BOOL	m_bOleExecCalled;
	INT m_nLastErrrorCode;
	CString m_strErrorMsg;
	CString m_strDBName;

	virtual BOOL ConnectDatabase(CDataSource & aoDataSource,CONST WCHAR * apHost,CONST WCHAR * apDb,CONST WCHAR * apUser,CONST WCHAR * apPWD);
	virtual void GetDBUidPwd(CString &ag_strDbName, CString &ag_strUID, CString &ag_strPwd, CString &ag_strDBServer);
	virtual BOOL ExecQuery(BOOL abUpdatableRow,BOOL abClearSession = TRUE);
	virtual BOOL PopulateErrorMsg();
	virtual BOOL Init();
	virtual void Log(CString astrFileOrFunc,int anLineNumber,const char *cmd,...);
	virtual void ClearRecordMemory() throw();
	virtual int	 GetColumnIndex(const char * apszColumnName);	
	virtual CCommand<CDynamicAccessor,CRowset> * MakeUpResultSet();
	virtual void CleanResultSet();

	virtual CSession * MakeUpSession();
	virtual void CleanSession();


};


#endif

