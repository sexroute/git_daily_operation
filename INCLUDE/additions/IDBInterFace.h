#ifndef __IDBInterFace__
#define __IDBInterFace__
#include <string>
#include <comutil.h>
#include <atlstr.h>
#include <ATLComTime.h>

#include <additions/BHDateTime.h>

#define WAVE_HEAD_MAGIC_NUMBER   825287
//波形头
#pragma pack (1)
struct CWaveHead
{

	int    m_nMagicValue;
	int    m_nDataLen;
	vector<float> m_pParam;

	CWaveHead()
	{
		BEGIN_ERROR_HANDLE

		m_nMagicValue = WAVE_HEAD_MAGIC_NUMBER;
		m_nDataLen    = 1;
		m_pParam.resize(m_nDataLen,10.0);

		END_ERROR_HANDLE
	}

	int GetSize()
	{
		return (sizeof(m_nMagicValue) + sizeof(m_nDataLen) + sizeof(float)*m_nDataLen);
	}
	void SetLength(int nDataCount)
	{
		BEGIN_ERROR_HANDLE
		m_nDataLen = nDataCount;
		m_pParam.resize(nDataCount,0);
		if(nDataCount > 0)
		{
			m_pParam[0] = 10.0;
		}
		
		END_ERROR_HANDLE
	}
	CWaveHead& operator=(const CWaveHead& cWaveHead_)
	{

		BEGIN_ERROR_HANDLE
		m_nDataLen = cWaveHead_.m_nDataLen;
		m_nMagicValue = cWaveHead_.m_nMagicValue;
		m_pParam.resize(m_nDataLen);
		memcpy(&m_pParam.front(),&cWaveHead_.m_pParam.front(),m_nDataLen*sizeof(float));
		END_ERROR_HANDLE
		return *this;
	}
	int GetLength()
	{
		return m_nDataLen;
	}

};
#pragma pack ()

using std::string;

enum CommandTypeEnum;
class CCustomDB;


class IDBInterFace
{
public:
	//设置参数或信息

	virtual void SetAutoUpdateRst(bool bAuto) =0;      											//!<设置修改后是否自动更新结果集
	virtual void SetAutoCommit(bool autoCommit) =0;    											//!<设置是否自动提交事务
	virtual void SetCommandType(CommandTypeEnum intCommandType) =0;  				//!<设置SQL指令类型
	virtual void SetErrMsg(const char *pszMsg) =0;     											///<设置自定义错误信息
	virtual void SetConnectionString(LPCTSTR lpstrConnectionString) =0;     ///<设置数据库连接串


	//取类运行信息
	virtual _ConnectionPtr GetConn() =0;       ///<取数据库连接指针
	virtual CString GetLastErrMsg() =0;        ///<取自定义错误信息
	virtual DWORD GetLastError() =0;           //!<取上一次SQL错误代码
	virtual CString GetConnectionString() =0;  //!<取数据库连接串
	virtual int GetRecordsAffected() =0;       //!<取上一次SQL命令影响的行数


	//Rst操作
	virtual bool AddNewRecord()=0;            //!<在结果集中增加新的记录
	virtual bool DelCurrentRecord()=0;        //!<删除结果集中的当前记录
	virtual bool CancelUpdateRst()=0;         //!<取消结果集中目前所前所做的修改
	virtual bool UpdateRst()=0;               //!<提交结果集中当前的修改


	virtual bool SetFieldValue(int id, CString sValue)=0;             //!<设置结果集中字符型字段值
	virtual bool SetFieldValue(const char *name, CString sValue)=0;   //!<设置结果集中字型字段值
	virtual bool SetFieldValue(const char *name,const char *value)=0; //!<设置结果集中字符型字段值
	virtual bool SetFieldValue(const char *name, double dblValue)=0;  //!<设置结果集中双精度型字段值
	virtual bool SetFieldValue(const char *name, long lValue)=0;      //!<设置结果集中长整型字段值
	virtual bool SetFieldValue(const char *name, CBHDateTime time)=0;//!<设置结果集中日期型字段值
	virtual bool SetFieldValue(const char *name, int nValue)=0;       //!<设置结果集中整型字段值
	virtual bool SetFieldValue(const char *name, _variant_t vtValue)=0;   //!<设置结果集字段值

	virtual long GetNumCols()=0;              		//!<取结果集返回列数
	virtual long GetColLen(int index)=0;      		//!<取对应列的定义长度
	virtual char * GetColName(const int id)=0;    //!<取对应列的字段名
	virtual long GetCount(int nType = 0)=0;       //!<根据据参数,返回结果集记录数
	virtual DWORD GetRecordCount()=0;             //!<返回结果集记录数



	//数据访问
	virtual long GetActualSize(int nFieldIndex)=0;    													//!<返回结果集中字段实际长度
	virtual long GetActualSize(const char *name)=0;   													//!<返回结果集中字段实际长度
	virtual bool GetBuffer(const char *name,BYTE *pBuf, long &nSize,CWaveHead& sWaveHead_)=0;   			//!<取结果集中字段数至缓冲区
	virtual bool GetBuffer( int nFieldIndex,BYTE *pBuf,long &nSize, CWaveHead& sWaveHead_)=0;    			//!<取结果集中字段数至缓冲区
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize )=0; 			//!<用缓冲区数据,更新结果集中字段数据
	virtual bool SetBuffer( const char *name, BYTE *pBuf, long nSize, bool abEncrypt )=0; //!<用缓冲区数据,更新结果集中字段数据
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize )=0;   //!<用缓冲区数据,更新结果集中字段数据
	virtual bool SetBuffer(const int nFieldIndex, BYTE *pBuf, long nSize, bool abEncrypt)=0;   //!<用缓冲区数据,更新结果集中字段数据

	virtual CString Get(int id,BOOL abTestEncode = FALSE)=0;
	virtual string Get(int id)=0;
	virtual string Get(const char *name,CString strDateFormat = CString(""))=0;
	virtual CBHDateTime GetTime(const char *name)=0; //!<根据字段名取日期型字段值
	virtual CBHDateTime GetTime(int id)=0;           //!<根据字段索引取日期型字段值
	virtual long GetLong(const char *name)=0;     		//!<根据字段名取整型字段值
	virtual long GetLong(int id)=0;               		//!<根据字段索引取整型字段值
	virtual double GetDouble(const char *name)=0; 		//!<根据字段名取双精度型字段值
	virtual double GetDouble(int id)=0;           		//!<根据字段索引取双精度型字段值


	//运行
	virtual bool More(BOOL abUpdatableRowSet = FALSE)=0;                    //!<移动结果集的记录至下一行
	virtual void Cancel()=0;                  //!<取消当前正在执行的命令
	virtual bool Exec()=0;                    //!<执行Sql语名
	virtual bool ExecForMore()=0;             //!<执行Sql语名,默认被More调用
	virtual bool DbUse(const char *db)=0;     //!<切换当前数据库
	virtual void CmdEnd()=0;                  //!<根据Cmd的指令,判断语句类型,并自动处理
	virtual bool Open(const char *svr, const char *usr, const char *pwd, const char *db)=0;   //!<建立数据库连接
	virtual long IsOpen()=0;
	virtual void __cdecl Cmd(const char *cmd,...)=0; 
	virtual void Close()=0; 
	virtual void SetSqlLogPath(CString lstrPath)=0;


	// 完成设置buffer数据操作
	virtual bool ExecSetBuffer(const char *name, BYTE *pBuf, long nSize,const char *tablename,
		const char* sqlwhere)=0;

	//存储过程相关
	virtual CBHDateTime GetOleDateTimeParamValue(const char *name)=0;
	virtual _variant_t GetVariantParamValue(const char *name)=0;
	virtual double GetDoubleParamValue(const char *name)=0;
	virtual long GetLongParamValue(const char *name)=0;
	virtual char * GetParamValue(const char *name,CString strDateFormat = CString(""))=0;

	virtual bool SetParamValue(const char *name, CBHDateTime time)=0;
	virtual bool SetParamValue(const char *name, _variant_t vtValue)=0;
	virtual bool SetParamValue(const char *name, long lValue)=0;
	virtual bool SetParamValue(const char *name,double dblValue)=0;
	virtual bool SetParamValue(const char *name,const char *value)=0;
	virtual bool SetParamValue(const char *name,CString strValue)=0;
	virtual long GetReturnValue()=0;
	virtual bool SetParamValue(const char *name, int nValue)=0;


	//事务相关
	virtual long BeginTran()=0; 
	virtual long CommitTran()=0; 
	virtual long RollbackTran()=0; 


	virtual void MoveFirst()=0; //前移记录到记录头
	virtual void MoveNext()=0;  //后移记录一条
	virtual void MovePrevious()=0; //前移记录一条
	virtual void MoveLast()=0; //后移记录至记录末
	virtual bool Move(int NumRecords)=0;

	virtual CString GetSql()=0;
	virtual void ClearSql()=0;
	virtual BOOL Login(const char *pszSvr , const char *pszID, const char *pszPwd , const char *pszDB)=0;

	virtual CString GetOleDbSql()=0;
	virtual void ClearReSource()=0;

	virtual BOOL IsUseOleDB()=0;
	virtual BOOL setUseOleDB(BOOL abUseOleDb)=0;


	virtual CString GetLastErrorString()=0;

	virtual BOOL IsDbSafe(BOOL abForceCheckDbSql = FALSE)=0;

	virtual BOOL JudgeDbType()=0;

	virtual INT GetDBServerType()=0;

	//老的CCustomDB 兼容，如不需要兼容老的CCustomDB，可以不实现
	virtual void setPDB(CCustomDB * apDb)=0;
	virtual CCustomDB * getPDB()=0;

};
#endif