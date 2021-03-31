#pragma once
/*
* Copyright (c) 2010, BH �������
* All rights reserved.
* 
* �ļ����ƣ� ZdlDbWrapper.cpp
* ժ    Ҫ�� oledb ���ݿ��װ��
* ��    �ߣ� �Դ���
* �� �� ��1���Դ��� �޸�ʱ�䣺2010/10/15 �޸����ݣ���֧�ֶ���������֧��д������֧��һ�ζ�ȡ���blob����
* ��    ���� 4300b* 
*/
#include <CustomDB.h>
#include <atldbcli.h>
#include "IDBInterFace.h"


#ifndef __BH_OLE_SQL_SERVER_DB__
#define __BH_OLE_SQL_SERVER_DB__


#define _USE_OLE_DB_DIRECT_ 1



// CZdlDbWrapper ����Ŀ��
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
	//���ò�������Ϣ
	virtual void setPDB(CCustomDB * apDb);
	virtual CCustomDB * getPDB();

	//!<�����޸ĺ��Ƿ��Զ����½����
	virtual void SetAutoUpdateRst(bool bAuto)
	{
		//û��ʵ��
		ASSERT(FALSE);
	}; 

	//!<�����Ƿ��Զ��ύ����
	virtual void SetAutoCommit(bool autoCommit)
	{
		//û��ʵ��
		ASSERT(FALSE);
	}; 

	//!<����SQLָ������
	virtual void SetCommandType(CommandTypeEnum intCommandType)
	{		//û��ʵ��
		ASSERT(FALSE);
	};  

	///<�����Զ��������Ϣ
	virtual void SetErrMsg(const char *pszMsg)

	{		//û��ʵ��
		ASSERT(FALSE);
	}; 

	///<�������ݿ����Ӵ�
	virtual void SetConnectionString(LPCTSTR lpstrConnectionString)
	{
		//û��ʵ��
		ASSERT(FALSE);
	};    


	//ȡ��������Ϣ
	///<ȡ���ݿ�����ָ��
	virtual _ConnectionPtr GetConn()
	{		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};

	///<ȡ�Զ��������Ϣ
	virtual CString GetLastErrMsg();
	//!<ȡ��һ��SQL�������
	virtual DWORD GetLastError();


	//!<ȡ���ݿ����Ӵ�
	virtual CString GetConnectionString()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return _T("");
	}; 

	//!<ȡ��һ��SQL����Ӱ�������
	virtual int GetRecordsAffected()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};      


	//Rst����
	//!<�ڽ�����������µļ�¼
	virtual bool AddNewRecord()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	}; 

	//!<ɾ��������еĵ�ǰ��¼
	virtual bool DelCurrentRecord()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};  

	//!<ȡ���������Ŀǰ��ǰ�������޸�
	virtual bool CancelUpdateRst()
	{
		return false;
	}; 

	//!<�ύ������е�ǰ���޸�
	virtual bool UpdateRst()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};               


	virtual bool SetFieldValue(int id, CString sValue);             //!<���ý�������ַ����ֶ�ֵ
	virtual bool SetFieldValue(const char *name, CString sValue);   //!<���ý�����������ֶ�ֵ
	virtual bool SetFieldValue(const char *name,const char *value); //!<���ý�������ַ����ֶ�ֵ
	virtual bool SetFieldValue(const char *name, double dblValue);  //!<���ý������˫�������ֶ�ֵ
	virtual bool SetFieldValue(const char *name, long lValue);      //!<���ý�����г������ֶ�ֵ
	virtual bool SetFieldValue(const char *name, CBHDateTime time);//!<���ý�������������ֶ�ֵ
	virtual bool SetFieldValue(const char *name, int nValue);       //!<���ý�����������ֶ�ֵ
	virtual bool SetFieldValue(const char *name, _variant_t vtValue);   //!<���ý�����ֶ�ֵ


	//!<ȡ�������������
	virtual long GetNumCols()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};   

	//!<ȡ��Ӧ�еĶ��峤��
	virtual long GetColLen(int index)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};   

	//!<ȡ��Ӧ�е��ֶ���
	virtual char * GetColName(const int id)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};   

	//!<���ݾݲ���,���ؽ������¼��
	virtual long GetCount(int nType = 0);    


	//!<���ؽ������¼��
	virtual DWORD GetRecordCount()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};            

private:
	virtual CString Get(int id,BOOL abTestEncode = FALSE);
	virtual string Get(int id);
	virtual bool GetBuffer( int nFieldIndex,BYTE *pBuf,long &nSize, CWaveHead& sWaveHead_);    //!<ȡ��������ֶ�����������
	virtual long GetActualSize(int nFieldIndex);    //!<���ؽ�������ֶ�ʵ�ʳ���
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize );  //!<�û���������,���½�������ֶ�����
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize, bool abEncrypt );  //!<�û���������,���½�������ֶ�����
	virtual CBHDateTime GetTime(int id);           //!<�����ֶ�����ȡ�������ֶ�ֵ
	virtual long GetLong(int id);               //!<�����ֶ�����ȡ�����ֶ�ֵ
	virtual double GetDouble(int id);           //!<�����ֶ�����ȡ˫�������ֶ�ֵ


public:	//���ݷ���
	virtual long GetActualSize(const char *name);   //!<���ؽ�������ֶ�ʵ�ʳ���
	virtual bool GetBuffer(const char *name,BYTE *pBuf, long &nSize, CWaveHead& sWaveHead_);   //!<ȡ��������ֶ�����������
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize ); //!<�û���������,���½�������ֶ�����
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize,bool abEncrypt ); //!<�û���������,���½�������ֶ�����

	virtual string Get(const char *name,CString strDateFormat = CString(""));
	virtual CString GetAsCString(const char *name);
	virtual CBHDateTime GetTime(const char *name); //!<�����ֶ���ȡ�������ֶ�ֵ
	virtual long GetLong(const char *name);     //!<�����ֶ���ȡ�����ֶ�ֵ
	virtual double GetDouble(const char *name); //!<�����ֶ���ȡ˫�������ֶ�ֵ


	//����
	virtual bool More(BOOL abUpdatableRowSet = FALSE);                    //!<�ƶ�������ļ�¼����һ��
	virtual void Cancel(){};                  //!<ȡ����ǰ����ִ�е�����
	virtual bool Exec();                    //!<ִ��Sql����
	virtual bool ExecForMore(){return false;};             //!<ִ��Sql����,Ĭ�ϱ�More����
	virtual bool DbUse(const char *db);     //!<�л���ǰ���ݿ�
	virtual void CmdEnd(){};                  //!<����Cmd��ָ��,�ж��������,���Զ�����

	//!<�������ݿ�����
	virtual bool Open(const char *svr, const char *usr, const char *pwd, const char *db)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	}; 

	virtual long IsOpen()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual void __cdecl Cmd(const char *cmd,...); 
	virtual void Close(); 
	virtual void SetSqlLogPath(CString lstrPath);



	//�洢�������
	virtual CBHDateTime GetOleDateTimeParamValue(const char *name)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return CBHDateTime();
	};
	virtual _variant_t GetVariantParamValue(const char *name)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return CComVariant();
	};
	virtual double GetDoubleParamValue(const char *name)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual long GetLongParamValue(const char *name)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual char * GetParamValue(const char *name,CString strDateFormat = CString(""))
	{
		//û��ʵ��
		ASSERT(FALSE);
		return NULL;
	};

	virtual bool SetParamValue(const char *name, CBHDateTime time)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, _variant_t vtValue)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, long lValue)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,double dblValue)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,const char *value)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name,CString strValue)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual long GetReturnValue()
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};
	virtual bool SetParamValue(const char *name, int nValue)
	{
		//û��ʵ��
		ASSERT(FALSE);
		return false;
	};

	virtual bool ExecSetBuffer(const char *name, BYTE *pBuf, long nSize,const char *tablename,
		const char* sqlwhere);
	//{
	//	//û��ʵ��
	//	ASSERT(FALSE);
	//	return false;
	//}


	//�������
	virtual long BeginTran(); 
	virtual long CommitTran(); 
	virtual long RollbackTran(); 


	virtual void MoveFirst(){}; //ǰ�Ƽ�¼����¼ͷ
	virtual void MoveNext(){};  //���Ƽ�¼һ��
	virtual void MovePrevious(){}; //ǰ�Ƽ�¼һ��
	virtual void MoveLast(){}; //���Ƽ�¼����¼ĩ
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

