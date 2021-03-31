/*! \file simado.h
*   \brief a class to access sqlserver
*
*   \author
*   \version 1.0
*   \date    2004
*   \attention please install microsoft mdac at frist
*/


// SimADO.h: interface for the CSimADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMADO_H__06B532F1_2621_48C0_809E_04C5090C5EBA__INCLUDED_)
#define AFX_SIMADO_H__06B532F1_2621_48C0_809E_04C5090C5EBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <Afxmt.h>  //CCriticalSection

#import "c:\Program Files\Common Files\System\ADO\msado15.dll" \
   no_namespace rename("EOF", "EndOfFile") //EndofFile表结束

/** @class CSimADO
 *  @brief CSimADO 类
 *
 *  用ADO实现数据库访问
 *
 *  @author
 *  @version 1.0
 *  @date   2004
 */
class AFX_EXT_CLASS CSimADO  
{
public:
    //设置参数或信息
    void SetAutoUpdateRst(bool bAuto);      //!<设置修改后是否自动更新结果集
    void SetAutoCommit(bool autoCommit);    //!<设置是否自动提交事务
    void SetCommandType(CommandTypeEnum intCommandType);  //!<设置SQL指令类型
    void SetErrMsg(const char *pszMsg);     ///<设置自定义错误信息
    void SetConnectionString(LPCTSTR lpstrConnectionString);    ///<设置数据库连接串
    void SetSqlLogPath(CString sLogPath);   ///<设置sql错误日志的保存路径

    //取类运行信息
    _ConnectionPtr GetConn();       ///<取数据库连接指针
    CString GetLastErrMsg();        ///<取自定义错误信息
    CString GetLastErrorString();   ///<取上一次SQL错误信息
    DWORD GetLastError();           //!<取上一次SQL错误代码
    CString GetConnectionString();  //!<取数据库连接串
    int GetRecordsAffected();       //!<取上一次SQL命令影响的行数


    //Rst操作
    bool AddNewRecord();            //!<在结果集中增加新的记录
    bool DelCurrentRecord();        //!<删除结果集中的当前记录
    bool CancelUpdateRst();         //!<取消结果集中目前所前所做的修改
    bool UpdateRst();               //!<提交结果集中当前的修改


    bool SetFieldValue(int id, CString sValue);             //!<设置结果集中字符型字段值
    bool SetFieldValue(const char *name, CString sValue);   //!<设置结果集中字型字段值
    bool SetFieldValue(const char *name,const char *value); //!<设置结果集中字符型字段值
    bool SetFieldValue(const char *name, double dblValue);  //!<设置结果集中双精度型字段值
    bool SetFieldValue(const char *name, long lValue);      //!<设置结果集中长整型字段值
    bool SetFieldValue(const char *name, COleDateTime time);//!<设置结果集中日期型字段值
    bool SetFieldValue(const char *name, int nValue);       //!<设置结果集中整型字段值
    bool SetFieldValue(const char *name, _variant_t vtValue);   //!<设置结果集字段值

    long GetNumCols();              //!<取结果集返回列数
    long GetColLen(int index);      //!<取对应列的定义长度
    char * GetColName(const int id);    //!<取对应列的字段名
    long GetCount(int nType = 0);       //!<根据据参数,返回结果集记录数
    DWORD GetRecordCount();             //!<返回结果集记录数



    //数据访问
    long GetActualSize(int nFieldIndex);    //!<返回结果集中字段实际长度
    long GetActualSize(const char *name);   //!<返回结果集中字段实际长度
    bool GetBuffer(const char *name,BYTE *pBuf, long &nSize);   //!<取结果集中字段数至缓冲区
    bool GetBuffer( int nFieldIndex,BYTE *pBuf,long &nSize);    //!<取结果集中字段数至缓冲区
    bool SetBuffer( const char *name, BYTE *pBuf, long nSize ); //!<用缓冲区数据,更新结果集中字段数据
    bool SetBuffer( int nFieldIndex, BYTE *pBuf, long nSize );  //!<用缓冲区数据,更新结果集中字段数据

    char * Get(const char *name,CString strDateFormat = CString(""));   //根据字段名取字符型字段值
    char * Get(int id);     //!<根据字段索引取字符型字段值
    COleDateTime GetTime(const char *name); //!<根据字段名取日期型字段值
    COleDateTime GetTime(int id);           //!<根据字段索引取日期型字段值
    long GetLong(const char *name);     //!<根据字段名取整型字段值
    long GetLong(int id);               //!<根据字段索引取整型字段值
    double GetDouble(const char *name); //!<根据字段名取双精度型字段值
    double GetDouble(int id);           //!<根据字段索引取双精度型字段值


    //运行
    bool More();                    //!<移动结果集的记录至下一行
    void Cancel();                  //!<取消当前正在执行的命令
    bool Exec();                    //!<执行Sql语名
    bool ExecForMore();             //!<执行Sql语名,默认被More调用
    bool DbUse(const char *db);     //!<切换当前数据库
    void CmdEnd();                  //!<根据Cmd的指令,判断语句类型,并自动处理
    bool Open(const char *svr, const char *usr, const char *pwd, const char *db);   //!<建立数据库连接
    long IsOpen();
    void Cmd(const char *cmd,...); 
    virtual void Close();   


    //存储过程相关
    COleDateTime GetOleDateTimeParamValue(const char *name);
    _variant_t GetVariantParamValue(const char *name);
    double GetDoubleParamValue(const char *name);
    long GetLongParamValue(const char *name);
    char * GetParamValue(const char *name,CString strDateFormat = CString(""));

    bool SetParamValue(const char *name, COleDateTime time);
    bool SetParamValue(const char *name, _variant_t vtValue);
    bool SetParamValue(const char *name, long lValue);
    bool SetParamValue(const char *name,double dblValue);
    bool SetParamValue(const char *name,const char *value);
    bool SetParamValue(const char *name,CString strValue);
    long GetReturnValue();
    bool SetParamValue(const char *name, int nValue);


    //事务相关
    long BeginTran(); 
    long CommitTran(); 
    long RollbackTran(); 


    void MoveFirst(); //前移记录到记录头
    void MoveNext();  //后移记录一条
    void MovePrevious(); //前移记录一条
    void MoveLast(); //后移记录至记录末
    bool Move(int NumRecords);

    CString GetSql();
    void ClearSql();


    CSimADO();
    virtual ~CSimADO();

	//其它
    CString GetLogPath(int nType = 0);

    int m_nCommandType;
    CString       m_sUser, m_sPwd , m_sSvr , m_sDB ;
    CString  m_sErr;     //错误信息,如果外部没有设置,则等于m_strLastError
    long m_nRecordsAffected;
    int bstrSize;
    char strProcName[1024];
    CString m_sLogPath;
protected:
    bool bAutoCommit;       //只负责setautocommit函数
    bool bAutoUpdate;       //只负责用m_pRst中数据的自动修改,如果设成true(默认),修改后自动update
    bool bHasTransaction;   //是否有未提交事务
    CString m_strConnectionString;
    CString m_strLastError;
    DWORD m_dwLastError;
    long mCount;              //在more中取数时累加,
    long mCurrentRstCount;    //在more中取数时累加,表示当前Rst中累计数,访问下一个Rst时清0
    long mRecordCount;        //不运行完more而要取数时用,只返回一个Rst情况下用
	
    int mStatus; // 0 :SQL 空闲状态 1 :有SQL语句等待执行 2:运行完,有结果数据待取出
    char strSQL[4096];
    char strBuffer[4096]; //用于返回字符串


    void WriteLogFile(const char *pszLog);
    int dump_com_error_log(_com_error &e);
    int dump_com_error_dialog(_com_error &e);
    int dump_com_error(_com_error &e);
    char * Trim(char * buffer);
    bool DelParameters(); 
    bool AddParameters();


    CString IntToStr(int nVal);
    CString LongToStr(long lVal);
    CString ULongToStr(unsigned long ulVal);
    CString DblToStr(double dblVal, int ndigits);
    CString DblToStr(float fltVal);


    bool IsEOF();
    bool IsBOF();
    
private:
    _ConnectionPtr m_pConn;
    _RecordsetPtr     m_pRst;
    _CommandPtr m_pCmd;
    _ParameterPtr m_pParam;

    void Test();
};

#endif // !defined(AFX_SIMADO_H__06B532F1_2621_48C0_809E_04C5090C5EBA__INCLUDED_)
