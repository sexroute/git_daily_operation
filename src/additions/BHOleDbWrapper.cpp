/*
* Copyright (c) 2010, BH 网软件部
* All rights reserved.
* 
* 文件名称： ZdlDbWrapper.cpp
* 摘    要： oledb 数据库封装类
* 作    者： 赵大力
* 修 改 者1：赵大力 修改时间：2010/11/15 修改内容：支持读操作，支持写操作，支持一次读写取多个blob数据
* 
*/
// ZdlDbWrapper.cpp : 实现文件
//

#include <StdAfx.h>
#include <additions/BHOleDbWrapper.h>
#include <additions/DebugHelper.h>
#include <additions/BlobStream.h>
#include <additions/ReaderWriterlock.h> 
#include <BaseFunc.h>
#include <atlbase.h>
#include <comutil.h>
#include <KDCommlib.h>
#include <ATLComTime.h>
#include <ocdb.h>
#include <atlbase.h>




volatile UINT g_pLoger = NULL;

CReaderWriterLock g_oLoggerLock;

volatile int COleSqlServerDB::m_nDbServerType = FALSE;

const unsigned short g_MagicNumber = 97;

int DecryptBuffer(void ** ppData,void * pBuf,long &nSize,DBLENGTH lnDataLengthToBeRead,CWaveHead &sWaveHead_)
{
	BEGIN_ERROR_HANDLE;
	
	int lnPeddingCount = sizeof(short)*2;

	if (lnDataLengthToBeRead<=lnPeddingCount || ppData == NULL || *ppData == NULL || pBuf == NULL || nSize<=0)
	{
		return 0;
	}

	static const unsigned short pnSeed[] = {13183,13187,13217,13219,13229,13241,13249,13259};
	
	unsigned short * lpHead = (unsigned short *)(*ppData);

	unsigned short lnHeaderComputed = *lpHead; 

	lpHead = lpHead+1;

	unsigned short lnHeaderOriginal = *lpHead;

	bool lbEncrypt = false;

	int lnArrayLength = (sizeof(pnSeed)/sizeof(short));

	for(int i = 0; i < lnArrayLength; i++)
	{
		if(lnHeaderOriginal == pnSeed[i])
		{
			lbEncrypt = true;
			break;
		}
	}

	if(!lbEncrypt)
	{
		return 0;
	}

	unsigned short lnComputed =  (lnHeaderOriginal ^ (lnHeaderOriginal+g_MagicNumber));

	if (lnComputed!=lnHeaderComputed)
	{
		return 0;
	}
	

	byte * lpBufferDst = (byte *)pBuf;

	byte * lpBufferSrc = (byte *)(*ppData);

	
	CWaveHead lsHead;

	int lnLen = sizeof(lsHead.m_nDataLen) + sizeof(lsHead.m_nMagicValue);

	if(lnDataLengthToBeRead > lnPeddingCount + lnLen)
	{
		
		byte * lpBufferDesTmp = (byte*)&lsHead;

		for (int i=lnPeddingCount;i< lnLen + lnPeddingCount;i++)
		{
			lpBufferDesTmp[i-lnPeddingCount] = lpBufferSrc[i]^lnHeaderOriginal;
		}

		if(lsHead.m_nMagicValue == WAVE_HEAD_MAGIC_NUMBER)
		{
			lnPeddingCount += lnLen;

			if(lsHead.m_nDataLen > 0)
			{
				lsHead.SetLength(lsHead.m_nDataLen);

				lpBufferDesTmp = (byte*)&lsHead.m_pParam.front();

				lnLen = lsHead.m_nDataLen * sizeof(float);

				for(int i = lnPeddingCount; i < lnPeddingCount + lnLen; i++)
				{
					lpBufferDesTmp[i-lnPeddingCount] = lpBufferSrc[i]^lnHeaderOriginal;
				}

				sWaveHead_ = lsHead;

				lnPeddingCount += lnLen;
			}
			
		}
	}
	
	if (lnDataLengthToBeRead > ((ULONG)nSize+lnPeddingCount))
	{
		lnDataLengthToBeRead = nSize+lnPeddingCount;
	}	

	for (int i=lnPeddingCount;i<lnDataLengthToBeRead;i++)
	{
		lpBufferDst[i-lnPeddingCount] = lpBufferSrc[i]^lnHeaderOriginal;
	}


	return lnDataLengthToBeRead;
	
	END_ERROR_HANDLE;

	return 0;
}

int EncryptBuffer(CBlobStream * lpBlob,BYTE * pBuf,ULONG nSize, ULONG __RPC_FAR *lnDataWritten)
{
	BEGIN_ERROR_HANDLE;
	
	unsigned short pnSeed[] = {13183,13187,13217,13219,13229,13241,13249,13259};

	if (NULL == lpBlob || NULL == pBuf || nSize<=0 || NULL == lnDataWritten )
	{
		return 0;
	}

	int lnArrayLength = (sizeof(pnSeed)/sizeof(short));

	int lnSeedIndex = (unsigned)time(NULL)%lnArrayLength;

	unsigned short lnSeed = pnSeed[lnSeedIndex];

	int lnPeddingCount = sizeof(short)*2;

	std::vector<BYTE> loBuffer(nSize+lnPeddingCount);

	byte * lpWrittenBuffer = (byte *)&loBuffer.front();

	short * lpHead = (short *)lpWrittenBuffer;

	*lpHead = (lnSeed+g_MagicNumber)^lnSeed;

	lpHead = lpHead+1;

	*lpHead = lnSeed;

	for (int i=0;i<nSize;i++)
	{
		lpWrittenBuffer[i+lnPeddingCount] = pBuf[i]^lnSeed;
	}

	ULONG lnDataWrote = 0;

	HRESULT lhr = lpBlob->Write(lpWrittenBuffer,nSize+lnPeddingCount,&lnDataWrote);

	if (S_OK == lhr)
	{
		*lnDataWritten = lnDataWrote;

		return lnDataWrote;
	}	
	
	END_ERROR_HANDLE;

	return 0;
}

/*
//!<设置修改后是否自动更新结果集
void CZdlDbWrapper::SetAutoUpdateRst(bool bAuto)
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetAutoUpdateRst(bAuto);
}; 


//!<设置是否自动提交事务
void CZdlDbWrapper::SetAutoCommit(bool autoCommit)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetAutoCommit(autoCommit);
}


//!<设置SQL指令类型
void CZdlDbWrapper::SetCommandType(CommandTypeEnum intCommandType)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetCommandType(intCommandType);
}


///<设置自定义错误信息
void CZdlDbWrapper::SetErrMsg(const char *pszMsg)
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetErrMsg(pszMsg);
}

///<设置数据库连接串
void CZdlDbWrapper::SetConnectionString(LPCTSTR lpstrConnectionString)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetConnectionString(lpstrConnectionString);
}


///<设置sql错误日志的保存路径
void CZdlDbWrapper::SetSqlLogPath(CString sLogPath)
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->SetSqlLogPath(sLogPath);
}




//取类运行信息

///<取数据库连接指针
_ConnectionPtr CZdlDbWrapper::GetConn()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetConn();
};   


///<取自定义错误信息
CString CZdlDbWrapper::GetLastErrMsg()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetLastErrMsg();
}


///<取上一次SQL错误信息
CString CZdlDbWrapper::GetLastErrorString()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetLastErrorString();
}


//!<取数据库连接串
CString CZdlDbWrapper::GetConnectionString()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetConnectionString();
}; 


//!<取上一次SQL命令影响的行数
int CZdlDbWrapper::GetRecordsAffected()
{


	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetRecordsAffected();
};       



//Rst操作
bool CZdlDbWrapper::AddNewRecord(){ASSERT(FALSE); 
ASSERT(FALSE);
ASSERT(this->m_pDb!=NULL);
return this->m_pDb->AddNewRecord();
};            
//!<在结果集中增加新的记录
bool CZdlDbWrapper::DelCurrentRecord()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->DelCurrentRecord();
};        
//!<删除结果集中的当前记录
bool CZdlDbWrapper::CancelUpdateRst()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->CancelUpdateRst();
};         
//!<取消结果集中目前所前所做的修改
bool CZdlDbWrapper::UpdateRst()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->UpdateRst();
};               
//!<提交结果集中当前的修改


bool CZdlDbWrapper::SetFieldValue(int id, CString sValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(id,sValue);
};             
//!<设置结果集中字符型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, CString sValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,sValue);
};   
//!<设置结果集中字型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name,const char *value)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,value);
}; 
//!<设置结果集中字符型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, double dblValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,dblValue);
};  
//!<设置结果集中双精度型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, long lValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,lValue);
};      
//!<设置结果集中长整型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, CBHDateTime time)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,time);
};
//!<设置结果集中日期型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, int nValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,nValue);
};       
//!<设置结果集中整型字段值
bool CZdlDbWrapper::SetFieldValue(const char *name, _variant_t vtValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetFieldValue(name,vtValue);
};   
//!<设置结果集字段值

long CZdlDbWrapper::GetNumCols()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetNumCols();
};              
//!<取结果集返回列数
long CZdlDbWrapper::GetColLen(int index)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetColLen(index);
};      
//!<取对应列的定义长度
char * CZdlDbWrapper::GetColName(const int id)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetColName(id);
};    
//!<取对应列的字段名

DWORD CZdlDbWrapper::GetRecordCount()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetRecordCount();
};             
//!<返回结果集记录数




//数据访问




/ *
char * CZdlDbWrapper::Get(const char *name,CString strDateFormat)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Get(name,strDateFormat);
};						
//根据字段名取字符型字段值
char * CZdlDbWrapper::Get(int id)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Get(id);
};				* /






//!<移动结果集的记录至下一行
void CZdlDbWrapper::Cancel()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Cancel();
};                  
//!<取消当前正在执行的命令
bool CZdlDbWrapper::Exec()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Exec();
};                    
//!<执行Sql语名
bool CZdlDbWrapper::ExecForMore()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->ExecForMore();
};             

void CZdlDbWrapper::CmdEnd()
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->CmdEnd();
};                  
//!<根据Cmd的指令,判断语句类型,并自动处理
bool CZdlDbWrapper::Open(const char *svr, const char *usr, const char *pwd, const char *db)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Open(svr,usr,pwd,db);
};   
//!<建立数据库连接
long CZdlDbWrapper::IsOpen()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->IsOpen();
};
//存储过程相关
CBHDateTime CZdlDbWrapper::GetOleDateTimeParamValue(const char *name)
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetOleDateTimeParamValue(name);
};
_variant_t CZdlDbWrapper::GetVariantParamValue(const char *name)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetVariantParamValue(name);
};
double CZdlDbWrapper::GetDoubleParamValue(const char *name)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetDoubleParamValue(name);
};
long CZdlDbWrapper::GetLongParamValue(const char *name)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetLongParamValue(name);
};
char * CZdlDbWrapper::GetParamValue(const char *name,CString strDateFormat / *= CString("")* /)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetParamValue(name,strDateFormat);
};

bool CZdlDbWrapper::SetParamValue(const char *name, CBHDateTime time)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,time);
};
bool CZdlDbWrapper::SetParamValue(const char *name, _variant_t vtValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,vtValue);
};
bool CZdlDbWrapper::SetParamValue(const char *name, long lValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,lValue);
};
bool CZdlDbWrapper::SetParamValue(const char *name,double dblValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,dblValue);
};
bool CZdlDbWrapper::SetParamValue(const char *name,const char *value)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,value);
};
bool CZdlDbWrapper::SetParamValue(const char *name,CString strValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,strValue);
};
long CZdlDbWrapper::GetReturnValue()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->GetReturnValue();
};
bool CZdlDbWrapper::SetParamValue(const char *name, int nValue)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->SetParamValue(name,nValue);
};

//事务相关
long CZdlDbWrapper::BeginTran()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->BeginTran();
}; 
long CZdlDbWrapper::CommitTran()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->CommitTran();
}; 
long CZdlDbWrapper::RollbackTran()
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->RollbackTran();
}; 


void CZdlDbWrapper::MoveFirst() 
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->MoveFirst();
}; 
//前移记录到记录头
void CZdlDbWrapper::MoveNext()
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->MoveNext();
};  
//后移记录一条
void CZdlDbWrapper::MovePrevious()
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->MovePrevious();
}; 
//前移记录一条
void CZdlDbWrapper::MoveLast()
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->MoveLast();
}; 
//后移记录至记录末
bool CZdlDbWrapper::Move(int NumRecords)
{

	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	return this->m_pDb->Move(NumRecords);
};


void CZdlDbWrapper::ClearSql() 
{
	ASSERT(FALSE);
	ASSERT(this->m_pDb!=NULL);
	this->m_pDb->ClearSql();
};
*/

void COleSqlServerDB::ClearSql() 
{
	if(this->m_bUseOleDB)
	{

		if (!this->IsDbSafe())
		{
			return ;
		}

		this->m_strOleDBSql = _T("");

		this->m_bOleExecCalled = FALSE;

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			this->m_pDb->ClearSql();
		}

	}
};

void COleSqlServerDB::ClearRecordMemory()
{
	for (ULONG i = 0; i < this->m_pResultSet->m_nColumns; i++)
	{
		DBLENGTH uLength = this->m_pResultSet->m_pColumnInfo[i].ulColumnSize;
		switch (this->m_pResultSet->m_pColumnInfo[i].wType)
		{
		case DBTYPE_STR :
			/*uLength += 1;*/
			break;
		case DBTYPE_WSTR :
			/*uLength = (uLength + 1) * 2;*/
			/*uLength +=2;*/
			break;
		}
		memset((BYTE*)this->m_pResultSet->_GetDataPtr(i), 0, uLength);
	}
}
int COleSqlServerDB::GetColumnIndex(const char * apszColumnName)
{
	if (NULL == apszColumnName || NULL == this->m_pResultSet->m_hRow)
	{
		ASSERT(FALSE);
		return -1;
	}

	DBORDINAL lnColumnIndex = 0;

	if (this->m_pResultSet->GetOrdinal(apszColumnName,&lnColumnIndex))
	{
		return lnColumnIndex;
	}
	ASSERT(FALSE);
	return -1;
}


bool COleSqlServerDB::SetFieldValue(int id, CString sValue)
{
	if(this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		return  this->m_pResultSet->SetValue(id,sValue.GetBuffer(0));

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(id,sValue);
		}

		return false;

	}
};             
//!<设置结果集中字符型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, CString sValue)
{

	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			int lnDbIndex = this->GetColumnIndex(name);

			if (lnDbIndex <0)
			{
				ASSERT(FALSE);
				return false;
			}

			DBTYPE ldwType = 0;

			this->m_pResultSet->GetColumnType(lnDbIndex,&ldwType);

			if (DBTYPE_STR == ldwType)
			{
				return  this->m_pResultSet->SetValue(name,(char *)sValue.GetBuffer(0));
			}

			/*[FIXME] 为什么会抛异常 */
			ASSERT(FALSE);

			return false;	
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}


	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,sValue);
		}

		return false;

	}
};   
//!<设置结果集中字型字段值
bool COleSqlServerDB::SetFieldValue(const char *name,const char *value)
{
	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			int lnDbIndex = this->GetColumnIndex(name);

			if (lnDbIndex <0)
			{
				ASSERT(FALSE);
				return false;
			}

			DBTYPE ldwType = 0;

			this->m_pResultSet->GetColumnType(lnDbIndex,&ldwType);

			if (DBTYPE_STR == ldwType)
			{
				return  this->m_pResultSet->SetValue(name,value);
			}

			ASSERT(FALSE);

			return false;	
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,value);
		}

		return false;
	}
}; 
//!<设置结果集中字符型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, double dblValue)
{
	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			int lnDbIndex = this->GetColumnIndex(name);

			if (lnDbIndex <0)
			{
				ASSERT(FALSE);
				return false;
			}

			DBTYPE ldwType = 0;

			this->m_pResultSet->GetColumnType(lnDbIndex,&ldwType);

			if (DBTYPE_R4 == ldwType)
			{
				return  this->m_pResultSet->SetValue(name,(float)dblValue);

			}else if (DBTYPE_R8 == ldwType)
			{
				return  this->m_pResultSet->SetValue(name,dblValue);
			}

			ASSERT(FALSE);

			return false;	
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,dblValue);
		}

		return false;

	}
};  
//!<设置结果集中双精度型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, long lValue)
{

	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}
			int lnDbIndex = this->GetColumnIndex(name);

			if (lnDbIndex <0)
			{
				ASSERT(FALSE);
				return false;
			}

			DBTYPE ldwType = 0;

			this->m_pResultSet->GetColumnType(lnDbIndex,&ldwType);

			if (DBTYPE_I4 == ldwType)
			{
				return  this->m_pResultSet->SetValue(name,(int)lValue);

			}else if (DBTYPE_I8 == ldwType)
			{
				LONGLONG lbigDecimal = lValue;

				return  this->m_pResultSet->SetValue(name,lbigDecimal);
			}

			ASSERT(FALSE);

			return false;	
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}
	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,lValue);
		}

		return false;

	}
};      
//!<设置结果集中长整型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, CBHDateTime time)
{

	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			int lnDbIndex = this->GetColumnIndex(name);

			if (lnDbIndex <0)
			{
				ASSERT(FALSE);
				return false;
			}

			DBTYPE ldwType = 0;

			this->m_pResultSet->GetColumnType(lnDbIndex,&ldwType);

			if (DBTYPE_DBTIMESTAMP == ldwType)
			{
				DBTIMESTAMP lodbTimeStamp={0};

				UDATE ud = {0};

				if (S_OK != VarUdateFromDate(time.m_dt, 0, &ud))
				{
					return false;
				}

				lodbTimeStamp.year = (SHORT) ud.st.wYear;
				lodbTimeStamp.month = (USHORT) ud.st.wMonth;
				lodbTimeStamp.day = (USHORT) ud.st.wDay;
				lodbTimeStamp.hour = (USHORT) ud.st.wHour;
				lodbTimeStamp.minute = (USHORT) ud.st.wMinute;
				lodbTimeStamp.second = (USHORT) ud.st.wSecond;
				lodbTimeStamp.fraction = 0;

				return  this->m_pResultSet->SetValue(name,lodbTimeStamp);

			}

			ASSERT(FALSE);

			return false;	
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			return false;
		}
	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,time);
		}

		return false;

	}
};
//!<设置结果集中日期型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, int nValue)
{
	if(this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		return  this->m_pResultSet->SetValue(name,nValue);

	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,nValue);
		}
		return false;

	}
};       
//!<设置结果集中整型字段值
bool COleSqlServerDB::SetFieldValue(const char *name, _variant_t vtValue)
{
	if(this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		return  this->m_pResultSet->SetValue(name,vtValue);

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			return this->m_pDb->SetFieldValue(name,vtValue);
		}

		return false;

	}
};   
//!<设置结果集字段值

//事务相关
long COleSqlServerDB::BeginTran()
{
	return TRUE;
}; 
long COleSqlServerDB::CommitTran()
{
	return TRUE;
}; 
long COleSqlServerDB::RollbackTran()
{
	return TRUE;
}; 


void COleSqlServerDB::SetSqlLogPath(CString sLogPath)
{
	g_oLoggerLock.AcquireWriterLock();

	if (NULL == g_pLoger)
	{
		this->m_strSqlLogPath = sLogPath;

		g_pLoger = CHZLogManage::GetLogger(sLogPath,"SQL",0x040|0x020|0x010|0x001|0x02|0x04);

	}else
	{
		zdlTraceLine(_T("sql logger already inited!"));
	}

	g_oLoggerLock.ReleaseWriterLock();

	return;
}

/*
* 函数介绍：根据指定字段id，返回结果集中字段实际字节长度
* 输入参数：id 指定字段序号,int
* 输出参数： 
* 返回值  ：结果集中指定字段实际字节长度，long型	
*/
long COleSqlServerDB::GetActualSize(int nFieldIndex)
{
	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			DBLENGTH lnDatalength = 0;

			this->m_pResultSet->GetLength(nFieldIndex,&lnDatalength);

			return lnDatalength;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			return 0;
		}

	}else
	{
		ASSERT(FALSE);
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->GetActualSize(nFieldIndex);
		}

		return -1;

	}
};
/*
* 函数介绍：根据指定字段名，返回结果集中字段实际字节长度
* 输入参数：name 指定字段名,const char *
* 输出参数： 
* 返回值  ：结果集中指定字段实际字节长度，long型	
*/
long COleSqlServerDB::GetActualSize(const char *name)
{
	if(this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			if (name == NULL)
			{
				return 0;
			}

			USES_CONVERSION;

			DBLENGTH lnDatalength = 0;

			this->m_pResultSet->GetLength(A2W(name),&lnDatalength);

			return lnDatalength;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

			return 0;
		}

	}else
	{
		ASSERT(FALSE);
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->GetActualSize(name);
		}

		return -1;

	}
};	
/*
* 函数介绍：内部使用的连接数据库函数
* 输入参数：aoDataSource ATL数据库模板对象,CDataSource型
apHost	数据库主机地址,WCHAR* 型
apDb	数据库实例名,WCHAR* 型
apUser	数据库用户名,WCHAR* 型
apPWD	数据库密码,WCHAR* 型
* 输出参数：	
* 返回值  ：链接成功返回1，链接失败返回0
*/
BOOL COleSqlServerDB::ConnectDatabase(CDataSource & aoDataSource,CONST WCHAR * apHost,CONST WCHAR * apDb,CONST WCHAR * apUser,CONST WCHAR * apPWD)
{   
	_DECLARE_PERF_MEASURE_TIME();

	_BEGIN_PERF_MEASURE_TIME();

	USES_CONVERSION;

	DBPROP rgProperties[4];
	// property 1: the datasource
	rgProperties[0].colid = DB_NULLID;
	rgProperties[0].dwOptions = DBPROPOPTIONS_REQUIRED;
	rgProperties[0].dwPropertyID = DBPROP_INIT_DATASOURCE;
	rgProperties[0].dwStatus = 0;
	rgProperties[0].vValue.vt = VT_BSTR;
	rgProperties[0].vValue.bstrVal = ::SysAllocString(W2COLE(apHost));
	// property 2: the database name
	rgProperties[1].colid = DB_NULLID;
	rgProperties[1].dwOptions = DBPROPOPTIONS_REQUIRED;
	rgProperties[1].dwPropertyID = DBPROP_INIT_CATALOG;
	rgProperties[1].dwStatus = 0;
	rgProperties[1].vValue.vt = VT_BSTR;
	rgProperties[1].vValue.bstrVal = ::SysAllocString(W2COLE(apDb));
	// property 3: the user id
	rgProperties[2].colid = DB_NULLID;
	rgProperties[2].dwOptions = DBPROPOPTIONS_REQUIRED;
	rgProperties[2].dwPropertyID = DBPROP_AUTH_USERID;
	rgProperties[2].dwStatus = 0;
	rgProperties[2].vValue.vt = VT_BSTR;
	rgProperties[2].vValue.bstrVal = ::SysAllocString(W2COLE(apUser));
	// property 4: the user password
	rgProperties[3].colid = DB_NULLID;
	rgProperties[3].dwOptions = DBPROPOPTIONS_REQUIRED;
	rgProperties[3].dwPropertyID = DBPROP_AUTH_PASSWORD;
	rgProperties[3].dwStatus = 0;
	rgProperties[3].vValue.vt = VT_BSTR;
	rgProperties[3].vValue.bstrVal = ::SysAllocString(W2COLE(apPWD));

	// combine the 3 properties into a property set structure
	DBPROPSET rgPropertySet[1];
	rgPropertySet[0].cProperties = 4;
	rgPropertySet[0].guidPropertySet = DBPROPSET_DBINIT;
	rgPropertySet[0].rgProperties = rgProperties;

	// now open the SQL Server database connection

	HRESULT hr = aoDataSource.OpenWithServiceComponents(_T("SQLOLEDB"), rgPropertySet, 1);

	CComBSTR loBstr;

	BOOL lbFlag = FALSE;

	if(S_OK == hr)
	{
		
		lbFlag = TRUE;
	}
	else
	{
		CString lstrRet;

		aoDataSource.GetInitializationString(&loBstr,true);

		zdlTraceLine(_T("%x:%s"),hr,W2T(loBstr.m_str));

	}

	for (int i =0;i<4;i++)
	{
		::SysFreeString(rgProperties[i].vValue.bstrVal);
	}

	return lbFlag;
}


/*
* 函数介绍：数据库对象构造函数
* 输入参数：
* 输出参数：	
* 返回值  ：
*/
COleSqlServerDB::COleSqlServerDB()
{
	this->m_pDb = NULL;

	this->m_bConnected =FALSE;

	this->m_bUseOleDB = _USE_OLE_DB_DIRECT_;

	this->m_ulOleDbDataCount = 0;

	this->m_nLastErrrorCode = 0;
	
	CoInitialize(NULL);

	this->m_pResultSet = new CCommand<CDynamicAccessor,CRowset>();
	this->m_pDbSession = new CSession();
	this->Init();
}

/*
* 函数介绍：数据库对象初始化，完成数据库连接
* 输入参数：
* 输出参数：	
* 返回值  ：初始化成功返回 1，初始化失败返回0
*/
BOOL COleSqlServerDB::Init()
{
	USES_CONVERSION;
	if (this->m_bUseOleDB)
	{
		try
		{

			this->GetDBUidPwd(this->m_strDBCataLog,this->m_strDBUser,this->m_strDBPwd,this->m_strDBServer);

			BOOL lbRet = this->ConnectDatabase(this->m_oDb,T2W(this->m_strDBServer),T2W(this->m_strDBCataLog),T2W(this->m_strDBUser),T2W(this->m_strDBPwd));

			if (lbRet)
			{

				HRESULT lhret = this->m_pDbSession->Open(m_oDb);

				lbRet = ((S_OK == lhret ));

			}else
			{
				this->PopulateErrorMsg();

				this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);
			}
			

			ASSERT(lbRet);

			this->m_bConnected = lbRet;

			this->JudgeDbType();

			return lbRet;

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


			return FALSE;
		}	
	}

	return FALSE;
}

BOOL COleSqlServerDB::Login(const char *pszSvr , const char *pszID, const char *pszPwd , const char *pszDB)
{
	if ((pszSvr!=NULL) && (pszID!=NULL) && (pszPwd!=NULL) && (pszDB!=NULL) )
	{
		this->ClearReSource();

	}else
	{
		return FALSE;
	}

	USES_CONVERSION;

	if (this->m_bUseOleDB)
	{
		try
		{
			this->m_strDBServer = pszSvr;

			this->m_strDBPwd = pszPwd;

			this->m_strDBUser = pszID;

			this->m_strDBCataLog = pszDB;

			BOOL lbRet = this->ConnectDatabase(this->m_oDb,T2W(this->m_strDBServer),T2W(this->m_strDBCataLog),T2W(this->m_strDBUser),T2W(this->m_strDBPwd));

			if (lbRet)
			{

				HRESULT lhret = this->m_pDbSession->Open(this->m_oDb);

				lbRet = ((S_OK == lhret ));

			}

			ASSERT(lbRet);

			this->m_bConnected = lbRet;

			return lbRet;

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);			


			return FALSE;
		}	
	}

	return FALSE;
}

BOOL COleSqlServerDB::IsUseOleDB()
{
	return this->m_bUseOleDB;
}

BOOL COleSqlServerDB::setUseOleDB(BOOL abUseOleDb)
{
	BOOL lbOldUseOleDb = this->m_bUseOleDB;

	this->m_bUseOleDB = abUseOleDb;

	return lbOldUseOleDb;
}

void COleSqlServerDB::ClearReSource()
{

	if (this->m_bConnected)
	{
		try
		{
			this->ClearRecordMemory();

			if (this->m_pResultSet->m_pBuffer!=NULL)
			{
				this->m_pResultSet->Close();
			}

			this->m_pDbSession->Close();

			this->CleanSession();

			this->m_oDb.Close();
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

		this->m_bConnected = FALSE;
	}
}

/*
* 函数介绍：数据库对象析构函数，自动关闭跟数据库对象的相关oledb对象
* 输入参数：
* 输出参数：	
* 返回值  ：
*/
COleSqlServerDB::~COleSqlServerDB()
{
	this->m_bConnected = FALSE;

	this->ClearReSource();

	if (NULL!=this->m_pResultSet)
	{
		delete this->m_pResultSet;
		this->m_pResultSet = NULL;
	}
	this->CleanSession();
}

void COleSqlServerDB::setPDB(CCustomDB * apDb)
{
	ASSERT(apDb != NULL);

	this->m_pDb = apDb;	
}

CCustomDB * COleSqlServerDB::getPDB()
{
	ASSERT(this->m_pDb!= NULL);
	return this->m_pDb;
}

/*
* 函数介绍：根据据参数,返回结果集记录数。
* 输入参数：nType 使用oledb时，无意义，int型
* 输出参数：	
* 返回值  ：结果集记录数，long型 
*/
long COleSqlServerDB::GetCount(int nType/* = 0*/)
{

	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		return this->m_ulOleDbDataCount;
	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetCount(nType);
		}

		return -1;

	}

};       




/*
* 函数介绍：取上一次数据库发生错误后的OLEDB错误号。
* 输入参数：
* 输出参数：	
* 返回值  ：取一次数据库错误的OLEDB错误号DWORD 
*/
DWORD COleSqlServerDB::GetLastError()
{
	if (this->m_bUseOleDB)
	{
		return this->m_nLastErrrorCode;

	}else
	{

		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->GetLastError();
		}

		return 0;

	}

};  


/*
* 函数介绍：取上一次数据库发生错误后的OLEDB错误字符串。
* 输入参数：
* 输出参数：	
* 返回值  ：取一次数据库错误的OLEDB错误号字符串 
*/

CString COleSqlServerDB::GetLastErrMsg()
{
	return this->m_strErrorMsg;
}

/*
* 函数介绍：根据字段名，取结果集字段到缓冲区，
在传入缓冲区大小和实际字段大小之间选择较小的一个，作为数据传送数。
* 输入参数：name	    字段序名，const char * 型
pBuf		传入的指向缓冲区的指针，由调用者申请，调用者释放，BYTE * 型
nSize		传入的pBuf指向的缓冲区大小，long型
* 输出参数：nSize		实际读取到缓冲区的数据字节数（in byte），long型		
* 返回值  ：读取返回1，读取失败返回0
*/
bool COleSqlServerDB::GetBuffer(  const char	*name,
								BYTE		*pBuf, 
								long		&nSize,
								CWaveHead& sWaveHead_)
{

	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{

		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			USES_CONVERSION;

			DBTYPE dbType;

			DBORDINAL loOrdinal;

			BOOL lbRet = this->m_pResultSet->GetOrdinal(A2W(name),&loOrdinal);

			if (!lbRet||(loOrdinal==-1))
			{
				lbRet = this->m_pResultSet->GetInternalColumnNo(A2W(name),&loOrdinal);
				
				if (!lbRet)
				{
					nSize = 0;
					return FALSE;
				}
			}


			DBSTATUS loStatus;

			this->m_pResultSet->GetStatus(loOrdinal,&loStatus);

			if (DBSTATUS_S_OK != loStatus)
			{
				return false;
			}

			if (!this->m_pResultSet->GetColumnType(loOrdinal,&dbType))
			{
				nSize = 0;

				return FALSE;
			}	

			if (DBTYPE_IUNKNOWN != dbType)
			{
				if (DBTYPE_BYREF  == (dbType & DBTYPE_BYREF) )
				{
					try
					{
						void ** ppData = (void **)this->m_pResultSet->GetValue( name );							

						if (*ppData == NULL)
						{
							return false;
						}	

						DBLENGTH lnDataLengthToBeRead =0;

						this->m_pResultSet->GetLength(name,&lnDataLengthToBeRead);

						if (nSize<0)
						{
							nSize = 0;
						}

						/************************************************************************/
						/* decrypt begin                                                        */
						/************************************************************************/

						if (DecryptBuffer(ppData,pBuf,nSize,lnDataLengthToBeRead,sWaveHead_)>0)
						{
							return true;
						}

						/************************************************************************/
						/* decrypt end                                                          */
						/************************************************************************/


						if (lnDataLengthToBeRead > (ULONG)nSize)
						{
							lnDataLengthToBeRead = nSize;
						}

						memcpy(pBuf,(*ppData),lnDataLengthToBeRead);

						return true;

					}
					catch (...)
					{
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

						ASSERT(FALSE);

						this->PopulateErrorMsg();

						this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


						return false;
					}
				}else
				{
					nSize = 0;

					return false;

				}
			}

			IUnknown ** lpUnk = NULL;

			CComQIPtr<ISequentialStream> loQIsp;

			lpUnk = (IUnknown **) this->m_pResultSet->GetValue(A2W(name));

			if (lpUnk == NULL)
			{
				return FALSE;
			}

			loQIsp = *lpUnk;

			HRESULT lhret = E_FAIL;

			ULONG ulBytesRead = 0;

			if (loQIsp!= NULL)
			{

				/************************************************************************/
				/* decrypt begin                                                        */
				/************************************************************************/
				std::vector<byte> loBuffer(nSize+2);

				byte * pData = &loBuffer.front();

				void ** ppData = (void **)&pData;

				lhret = loQIsp->Read(pData, nSize ,&ulBytesRead);

				if (DecryptBuffer(ppData,pBuf,nSize,ulBytesRead,sWaveHead_)>0)
				{
					return true;
				}

				/************************************************************************/
				/* decrypt end                                                          */
				/************************************************************************/

				lhret = loQIsp->Read(pBuf, nSize ,&ulBytesRead);

				nSize = ulBytesRead;

				return true;

			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


		}

		nSize = 0;

		return false;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetBuffer(name,pBuf,nSize);
		}

		return false;

	}

};	


/*
* 函数介绍：根据字段序号，取结果集字段到缓冲区，在传入缓冲区大小和实际字段大小之间选择较小的一个，作为数据传送数。
* 输入参数：nFieldIndex	字段序号，int型
pBuf		传入的指向缓冲区的指针，由调用者申请，调用者释放，BYTE * 型
nSize		传入的pBuf指向的缓冲区大小，long型
* 输出参数：nSize		实际读取到缓冲区的数据字节数（in byte），long型		
* 返回值  ：读取返回1，读取失败返回0
*/

bool COleSqlServerDB::GetBuffer( 	int nFieldIndex,
								BYTE *pBuf,
								long &nSize,
								CWaveHead& sWaveHead_)
{
	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{

		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}

			USES_CONVERSION;

			DBTYPE dbType;

			this->m_pResultSet->GetColumnType(nFieldIndex,&dbType);

			DBSTATUS loStatus;

			this->m_pResultSet->GetStatus(nFieldIndex,&loStatus);

			if (DBSTATUS_S_OK != loStatus)
			{
				return false;
			}

			if (DBTYPE_IUNKNOWN != dbType)
			{
				if (DBTYPE_BYREF  == (dbType & DBTYPE_BYREF) )
				{
					try
					{
						void ** ppData = (void **)this->m_pResultSet->GetValue( nFieldIndex );							

						if (*ppData == NULL)
						{
							return false;
						}	

						DBLENGTH lnDataLengthToBeRead =0;

						this->m_pResultSet->GetLength(nFieldIndex,&lnDataLengthToBeRead);

						if (nSize<0)
						{
							nSize = 0;
						}

						/************************************************************************/
						/* decrypt begin                                                        */
						/************************************************************************/

						if (DecryptBuffer(ppData,pBuf,nSize,lnDataLengthToBeRead,sWaveHead_)>0)
						{
							return true;
						}

						/************************************************************************/
						/* decrypt end                                                          */
						/************************************************************************/

						if (lnDataLengthToBeRead > (ULONG)nSize)
						{
							lnDataLengthToBeRead = nSize;
						}

						memcpy(pBuf,(*ppData),lnDataLengthToBeRead);


						return true;


					}
					catch (...)
					{
						ASSERT(FALSE);
						CString lstrErrorLog;
						lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
						CHZLogManage::Fatal(lstrErrorLog);
						zdlTraceLine(lstrErrorLog);

						this->PopulateErrorMsg();

						this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


						return false;
					}
				}
				else
				{
					nSize = 0;

					return false;

				}
			}

			IUnknown ** lpUnk = NULL;

			CComQIPtr<ISequentialStream> loQIsp;

			lpUnk = (IUnknown **) this->m_pResultSet->GetValue(nFieldIndex);

			if (lpUnk == NULL)
			{
				return FALSE;
			}

			loQIsp = *lpUnk;

			HRESULT lhret = E_FAIL;

			ULONG ulBytesRead = 0;

			if (loQIsp!= NULL)
			{
				/************************************************************************/
				/* decrypt begin                                                        */
				/************************************************************************/
				std::vector<byte> loBuffer(nSize+2);
				
				byte * pData = &loBuffer.front();

				void ** ppData = (void **)&pData;

				lhret = loQIsp->Read(pData, nSize ,&ulBytesRead);

				if (DecryptBuffer(ppData,pBuf,nSize,ulBytesRead,sWaveHead_)>0)
				{
					return true;
				}

				/************************************************************************/
				/* decrypt end                                                          */
				/************************************************************************/


				lhret = loQIsp->Read(pBuf, nSize ,&ulBytesRead);

				nSize = ulBytesRead;
				

				return true;

			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


		}

		nSize = 0;

		return false;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetBuffer(nFieldIndex,pBuf,nSize);
		}

		return false;
	}

};		



/*
* 函数介绍：根据字段名取字段日期值
* 输入参数：name 字段名，char *
* 输出参数：
* 返回值  ：日期值,COleDataTime，取不到值时为0(midnight, 30 December 1899）
*/
CBHDateTime COleSqlServerDB::GetTime(const char *name)
{
	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		CBHDateTime loColeDateTime;

		try
		{


			if (!this->IsDbSafe())
			{
				return loColeDateTime;
			}
			
			DBORDINAL lnColumnIndex = this->GetColumnIndex(name);

			DBTYPE lnType = DBTYPE_NULL;

			this->m_pResultSet->GetColumnType(lnColumnIndex,&lnType);

			if (DBTYPE_DBTIMESTAMP == lnType)
			{
				DBTIMESTAMP dbTimeStamp;

				USES_CONVERSION;

				this->m_pResultSet->GetValue(A2W(name),&dbTimeStamp);

				CBHDateTime loDatetime =  CBHDateTime(dbTimeStamp.year, dbTimeStamp.month, dbTimeStamp.day,
					dbTimeStamp.hour, dbTimeStamp.minute, dbTimeStamp.second,(int)(dbTimeStamp.fraction/1000000));

				return loDatetime;

			}else if (DBTYPE_WSTR == lnType || DBTYPE_BSTR == lnType)
			{
				USES_CONVERSION;				

				CBHDateTime loDatetime;

				DBORDINAL id = this->GetColumnIndex(name);

				CString lstrCurrentTime =  W2T((LPWSTR)this->m_pResultSet->GetValue(id));

				loDatetime.ParseDateTime(lstrCurrentTime);

				return loDatetime;
			}
			else if (DBTYPE_STR == lnType) 
			{
				USES_CONVERSION;				

				CBHDateTime loDatetime;

				DBORDINAL id = this->GetColumnIndex(name);

				CString lstrCurrentTime =  A2T((LPSTR)this->m_pResultSet->GetValue(id));

				loDatetime.ParseDateTime(lstrCurrentTime);

				return loDatetime;
			}
			else{
				return CBHDateTime::GetCurrentTimeEx();
			}
						

		}
		catch (...)
		{

			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

		return loColeDateTime;
	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetTime(name);
		}

		COleDateTime loColeDateTime;

		return loColeDateTime;
	}

};											
/*
* 函数介绍：根据字段序号取字段日期值
* 输入参数：id 字段序号，int型
* 输出参数：
* 返回值  ：日期值,COleDataTime，取不到值时为0(midnight, 30 December 1899）
*/
CBHDateTime COleSqlServerDB::GetTime(int id)
{

	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		CBHDateTime loColeDateTime;

		try
		{
			if (!this->IsDbSafe())
			{
				return loColeDateTime;
			}

			DBTIMESTAMP dbTimeStamp;

			USES_CONVERSION;

			DBORDINAL lnColumnIndex = id;

			DBTYPE lnType = DBTYPE_NULL;

			this->m_pResultSet->GetColumnType(lnColumnIndex,&lnType);

			if (DBTYPE_DBTIMESTAMP == lnType)
			{
				DBTIMESTAMP dbTimeStamp;

				USES_CONVERSION;

				this->m_pResultSet->GetValue(id,&dbTimeStamp);

				CBHDateTime loDatetime =  CBHDateTime(dbTimeStamp.year, dbTimeStamp.month, dbTimeStamp.day,
					dbTimeStamp.hour, dbTimeStamp.minute, dbTimeStamp.second,(int)(dbTimeStamp.fraction/1000000));

				return loDatetime;

			}else if (DBTYPE_WSTR == lnType || DBTYPE_BSTR == lnType)
			{
				USES_CONVERSION;				

				CBHDateTime loDatetime;

				CString lstrCurrentTime =  W2T((LPWSTR)this->m_pResultSet->GetValue(id));

				loDatetime.ParseDateTime(lstrCurrentTime);

				return loDatetime;
			}else
			{
				return CBHDateTime::GetCurrentTimeEx();
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}



		return loColeDateTime;
	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetTime(id);
		}

		COleDateTime loColeDateTime;

		return loColeDateTime;
	}
};           
/*
* 函数介绍：根据字段名取整型字段值
* 输入参数：name 字段名
* 输出参数：
* 返回值  ：long 数据库中指定字段整型值
*/	
long COleSqlServerDB::GetLong(const char *name)
{


	ASSERT(this->m_pDb!=NULL);


	if (this->m_bUseOleDB)
	{
		long llData = 0;

		if (!this->IsDbSafe())
		{
			return llData;
		}

		try
		{
			USES_CONVERSION;

			DBTYPE loDBType = DBTYPE_NULL;

			DBORDINAL loId = 0;

			if (this->m_pResultSet->GetOrdinal(A2W(name),&loId))
			{	
				if (this->m_pResultSet->GetColumnType(loId,&loDBType))
				{
					if (loDBType == DBTYPE_I8)
					{
						LONGLONG lullData = 0;

						this->m_pResultSet->GetValue(loId,&lullData);

						llData = lullData;

					}else
					{
						this->m_pResultSet->GetValue(loId,&llData);
					}

					return llData;
				}

			}else
			{
				return 0;
			}			

			return llData;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

		return 0L;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetLong(name);
		}

		return 0;
	}
};  

/*
* 函数介绍：根据字段索引取整型字段值
* 输入参数：id 列序号,int型
* 输出参数：
* 返回值  ：long 数据库中指定字段整型值
*/	
long COleSqlServerDB::GetLong(int id)
{

	if (this->m_bUseOleDB)
	{
		long llData = 0;

		try
		{
			if (!this->IsDbSafe())
			{
				return llData;
			}

			DBTYPE loDBType = DBTYPE_NULL;

			if (this->m_pResultSet->GetColumnType(id,&loDBType))
			{
				if (loDBType == DBTYPE_I8)
				{
					LONGLONG lullData = 0;

					this->m_pResultSet->GetValue(id,&lullData);

					llData = lullData;

				}else
				{

					this->m_pResultSet->GetValue(id,&llData);
				}

				return llData;
			}				

			return llData;
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


		}

		return 0L;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetLong(id);
		}

		return 0;

	}

};   

/*
* 函数介绍：根据字段名得到数据库对应的double值
* 输入参数：name char *型的字符串
* 输出参数：
* 返回值  ：double型浮点数
*/
double COleSqlServerDB::GetDouble(const char *name)
{


	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		if (this->m_bOleExecCalled)
		{
			float lfData = 0;

			USES_CONVERSION;

			if (!this->IsDbSafe())
			{
				return lfData;
			}

			try
			{
				int lnColumnID = this->GetColumnIndex(name);

				return this->GetDouble(lnColumnID);
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);

				this->PopulateErrorMsg();
				this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);


			}			
		}

		return 0L;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetDouble(name);
		}

		return 0;

	}

}; 

/*
* 函数介绍：根据字段序号得到数据库对应的double值
* 输入参数：id列序号
* 输出参数：
* 返回值  ：double型浮点数
*/
double COleSqlServerDB::GetDouble(int id)
{


	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		if (this->m_bOleExecCalled)
		{
			double lfData = 0;

			if (!this->IsDbSafe())
			{
				return lfData;
			}

			try
			{
				DBTYPE ldbType;

				if (this->m_pResultSet->GetColumnType(id,&ldbType))
				{

					switch (ldbType)
					{
					case  DBTYPE_R8:
						this->m_pResultSet->GetValue(id,&lfData);
						break;
					case DBTYPE_R4:							
						{ 
							float ldfTemp;
							this->m_pResultSet->GetValue(id,&ldfTemp);
							lfData = ldfTemp;
						}	
						break;
					case  DBTYPE_UI8:
					case  DBTYPE_I8:
						{ 
							LONGLONG llTemp;
							this->m_pResultSet->GetValue(id,&llTemp);
							lfData = llTemp;
						}
						break;
					case  DBTYPE_BOOL:
					case  DBTYPE_UI4:
					case  DBTYPE_I4:
						{ 
							int llTemp;
							this->m_pResultSet->GetValue(id,&llTemp);
							lfData = llTemp;
						}
						break;
					case  DBTYPE_UI2:
					case  DBTYPE_I2:
						{ 
							short llTemp;
							this->m_pResultSet->GetValue(id,&llTemp);
							lfData = llTemp;
						}
						break;
					default:
						break;
					}

				}				

				return lfData;
			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);

				this->PopulateErrorMsg();

				this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

			}			
		}

		return 0L;

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetDouble(id);
		}

		return 0;
	}
};


/*
* 函数介绍：切换当前数据库
* 输入参数：db 数据库名称 char *字符串
* 输出参数：
* 返回值  ：成功返回1,失败返回0
*/
bool COleSqlServerDB::DbUse(const char *db)
{

	//ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}


		this->m_ulOleDbDataCount = 0;

		this->m_strDBName = db;

		return true;


	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->DbUse(db);
		}

		return false;

	}

}; 

/*
* 函数介绍：读取结果集的下一行
* 输入参数：
* 输出参数：
* 返回值  ：成功返回1,失败返回0
*/	
bool COleSqlServerDB::More(BOOL abUpdatableRowSet /*= FALSE*/ )
{
	bool lnRet = false;

	bool lbExecQueryCalled = FALSE;	

	if (this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		this->m_pResultSet->FreeRecordMemory();		

		if (!this->m_bOleExecCalled)
		{
			lnRet = this->ExecQuery(abUpdatableRowSet);

			lbExecQueryCalled = TRUE;

			//TRACE(_T("ExecQuery result : %d\r\n"),lnRet);
		}

		if (lbExecQueryCalled && !lnRet)
		{
			this->Log(__FUNCTION__,__LINE__,"ExecQuery Sql Error: %s\r\n%s",this->m_strErrorMsg,this->m_strOleDBSql);

			return FALSE;
		}

		ASSERT(this->m_bOleExecCalled );

		HRESULT lhRet = S_FALSE;


		if (!abUpdatableRowSet)
		{
			if (this->m_pResultSet->m_spRowset != NULL)
			{
				lhRet = this->m_pResultSet->MoveNext();
			}else
			{
				if (lnRet)
				{
					if( 
						this->m_strOleDBSql.TrimLeft().MakeUpper().Find("UPDATE") >=0 ||
						this->m_strOleDBSql.TrimLeft().MakeUpper().Find("INSERT") >=0
						)
					{
						lhRet = S_OK;
					}else
					{
						lhRet = S_FALSE;
					}

				}	
			}				

		}else
		{

			lhRet = this->m_pResultSet->MoveNext();

			if (S_OK != lhRet)
			{
				if (this->m_ulOleDbDataCount > 0)
				{
					lhRet = S_OK;
				}
			}
		}

		lnRet = (lhRet == S_OK);

		if (!lnRet )
		{
			if (lhRet != DB_S_ENDOFROWSET)
			{
				ASSERT(FALSE);

				/*				this->m_nLastErrrorCode = lnRet;	*/	
				this->PopulateErrorMsg();

				zdlTraceLine(_T("OLEDB Error Sql:Error:code:0x%.8x %s \r\n %s"),lhRet,this->m_strErrorMsg,this->GetOleDbSql());

			}			
		}

		return lnRet;

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			lnRet = this->m_pDb->More();

			if (!lnRet)
			{
				zdlTraceLine(_T("OleDB Error code:%d "),this->m_pDb->GetLastError());

				if (0!= this->m_pDb->GetLastError())
				{
					ASSERT(FALSE);

					zdlTraceLine(_T("DB Error custom msg:%s "),this->m_pDb->GetLastErrMsg());

					zdlTraceLine(_T("DB Error system msg:%s "),this->m_pDb->GetLastErrorString());

					zdlTraceLine(_T("Error Sql: %s"),this->m_pDb->GetSql());

					zdlTraceLine(_T("connection string: %s"),this->m_pDb->GetConnectionString());
				}
			}
		}

		return false;


	}


	return lnRet;
}; 



/*
* 函数介绍：拼写sql语句
* 输入参数：szCmdFormat 零结尾的sql格式控制字符串
args		可选参数
* 输出参数：
* 返回值  ：
*/
void __cdecl COleSqlServerDB::Cmd(const char *szCmdFormat,...)
{
	USES_CONVERSION;

	va_list  args;

	va_start(args, szCmdFormat);

	va_list argListSave = args;

	int lnDataLength = GetFormatLength(szCmdFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);

	_vstprintf((TCHAR *)lpszBuffer, szCmdFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

	CString lstrCMD = T2A(lstrTempData.GetBuffer(0));

	if (this->m_bUseOleDB)
	{
		if (this->m_bOleExecCalled)
		{
			this->m_strOleDBSql = _T("");

			this->m_bOleExecCalled = FALSE;
		}

		CString lstrTemp;

		lstrTemp.Format(_T("%s %s"),this->m_strOleDBSql,lstrTempData);

		this->m_strOleDBSql = lstrTemp;

		//zdlTraceLine(_T("Sql string :%s"),this->m_strOleDBSql );

	}else
	{
		if (this->m_pDb)
		{
			this->m_pDb->Cmd("%s",lstrCMD.GetBuffer(0));
		}

	}

}

/*
* 函数介绍：得到当前执行的sql，执行完more后，sql清空
* 输入参数：
* 输出参数：
* 返回值  ：当前执行的sql CString对象 
*/

CString COleSqlServerDB::GetSql()
{

	ASSERT(this->m_pDb!=NULL);

	if (this->m_bUseOleDB)
	{
		return this->GetOleDbSql();

	}else
	{
		if (this->m_pDb)
		{
			return this->m_pDb->GetSql();
		}

		return "";

	}

};

CString COleSqlServerDB::GetAsCString(const char *name)
{
	CString lstrTemp;

	if (this->m_bUseOleDB && (name!= NULL))
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return "";
			}

			USES_CONVERSION;

			DBORDINAL loId = 0;

			if (this->m_pResultSet->GetOrdinal(A2W(name),&loId))
			{
				lstrTemp =  this->Get(loId,FALSE);

			}else
			{
				lstrTemp = CString(_T(""));
			}


		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

	}else
	{
		if (this->m_pDb)
		{
			lstrTemp = this->m_pDb->Get(name);
		}



	}

	return lstrTemp;
}

CString COleSqlServerDB::Get(int id,BOOL abTestEncode /*= FALSE*/)
{

	CString lstrtemp ;

	if (this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return "";
			}

			USES_CONVERSION;

			DBTYPE loDbType;

			this->m_pResultSet->GetColumnType(id,&loDbType);

			CString lstrTempCstr;

			switch(loDbType)
			{
			case DBTYPE_WSTR:
			case DBTYPE_BSTR:
				lstrTempCstr   = W2T((LPWSTR)this->m_pResultSet->GetValue(id));
				break;
			case DBTYPE_STR:
				lstrTempCstr  = A2T((LPSTR)this->m_pResultSet->GetValue(id));
				break;
			default:
				lstrTempCstr = _T("");
				break;
			}			

			return lstrTempCstr;


		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

	}else
	{
		if (this->m_pDb)
		{
			lstrtemp = this->m_pDb->Get(id);
		}

	}

	return lstrtemp;
}

/*
* 函数介绍：根据字段名得到数据库对应的字符串
* 输入参数：name 字段名
* 输出参数：
* 返回值  ：string 数据库中指定字段字符串
*/
string COleSqlServerDB::Get(const char *name,CString strDateFormat /*= CString("")*/)
{

	string lstrtemp ;

	if (this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return "";
			}

			if (name == NULL)
			{
				ASSERT(FALSE);

				return string("");
			}

			USES_CONVERSION;

			DBTYPE loDbType;

			DBORDINAL loId = 0;

			if (this->m_pResultSet->GetOrdinal(A2W(name),&loId))
			{
				DBTYPE loDbType;

				this->m_pResultSet->GetColumnType(loId,&loDbType);

				CString lstrTempCstr;

				switch(loDbType)
				{
				case DBTYPE_WSTR:
				case DBTYPE_BSTR:
					lstrTempCstr   = W2T((LPWSTR)this->m_pResultSet->GetValue(loId));
					break;
				case DBTYPE_STR:
					lstrTempCstr  = A2T((LPSTR)this->m_pResultSet->GetValue(loId));
					break;
				default:
					lstrTempCstr = (LPSTR)this->m_pResultSet->GetValue(A2W(name));
					break;
				}

				string lstrTempData =  lstrTempCstr.TrimRight().GetBuffer(0);
				return lstrTempData.c_str();

			}else
			{

				CString lstrTempCstr = (LPSTR)this->m_pResultSet->GetValue(A2W(name));

				string lstrTempData = lstrTempCstr.TrimRight().GetBuffer(0);

				return lstrTempData.c_str();
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);
		}

	}else
	{
		if (this->m_pDb)
		{
			lstrtemp = this->m_pDb->Get(name,strDateFormat);
		}		
	}

	return lstrtemp;
}

/*
* 函数介绍：根据字段序号得到数据库对应的字符串
* 输入参数：id列序号
* 输出参数：
* 返回值  ：string 数据库中指定字段字符串
*/
string COleSqlServerDB::Get(int id)
{
	CString lstrtemp ;

	if (this->m_bUseOleDB)
	{
		try
		{
			if (!this->IsDbSafe())
			{
				return "";
			}

			lstrtemp = (LPSTR)this->m_pResultSet->GetValue(id);

			CString lstrData = lstrtemp.TrimRight();

			string lstrTmp = lstrData.GetBuffer(0);

			return lstrTmp.c_str();

			//zdlTraceLine(_T("string lstrtemp"));
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();
			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

	}else
	{
		if (this->m_pDb)
		{
			lstrtemp = this->m_pDb->Get(id);
		}

	}

	string lstrTempData = lstrtemp.GetBuffer(0);

	return lstrTempData.c_str();
}

CString gDBName;
CString gtrUID;
CString gstrPWD;
CString gstrDBServer;
CReaderWriterLock m_oSectionDB;

//解析数据库配置文件，内部使用
void COleSqlServerDB::GetDBUidPwd(CString &ag_strDbName, 
								  CString &ag_strUID, 
								  CString &ag_strPwd, 
								  CString &ag_strDBServer)
{
	m_oSectionDB.AcquireWriterLock();
	try
	{
		if (gDBName.IsEmpty())
		{
			char sUser[1024]={0};
			char sPwd[1024]={0};
			char sEnUser[1024]={0};
			char sEnPwd[1024]={0};
			char sDb[2000]={0};
			char cAppLocation[500]={0};
			char *token;
			GetCurrentDirectory(500,cAppLocation);
			strcat(cAppLocation,"\\DseSvr.ini");
			//得到用户名和密码
			GetPrivateProfileString("DBINFO","USER","ac6ffedd23f7b1a2cf7735b055c357d0e0a36f899c023262c3d0a8887338f999",sEnUser,1024,cAppLocation);
			GetPrivateProfileString("DBINFO","PWD","b693a448ed4ba85e83d98bbbb9dc14d67885edbc347a",sEnPwd,1024,cAppLocation);
			char key[8]="dse r&d";//密钥
			//解密
			des3_ecb_decrypt(key,sEnUser,sUser);
			des3_ecb_decrypt(key,sEnPwd,sPwd);
			ag_strUID.Format("%s",sUser);
			ag_strPwd.Format("%s",sPwd);
			//得到数据库服务器名和数据库名
			GetPrivateProfileString("SERVER","SERVER1","监测中心,dse,DSE,DSE",sDb,2000,cAppLocation);
			char seps[]=", ";
			token = strtok( sDb, seps );
			for(int iloop=0; token != NULL;iloop++) 
			{   
				/* While there are tokens in "string" */
				if (iloop==1)
				{
					ag_strDBServer.Format("%s",token);
					token = strtok( NULL, seps );
					continue;
				}
				if (iloop==2)
				{
					ag_strDbName.Format("%s",token);
					token = strtok( NULL, seps );
					continue;
				}
				if (iloop==3)
				{
					break;
				}
				/* Get next token: */
				token = strtok( NULL, seps );
			}
			//得到是否切换数据库的标识
			g_iDbType = GetPrivateProfileInt("DBINFO","DBTYPE",0x0000,cAppLocation);

			if (g_iDbType!=0 && g_iDbType!=1)
			{	
				CHZLogManage::Error("配置数据库的类型有误","OnlineGlobal","GetMainDBName");
			}

			gDBName =ag_strDbName ;
			gtrUID = ag_strUID;
			gstrPWD = ag_strPwd;
			gstrDBServer = ag_strDBServer;
		}else
		{
			ag_strDbName =gDBName ;
			ag_strUID = gtrUID;
			ag_strPwd = gstrPWD;
			ag_strDBServer = gstrDBServer;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);

		this->PopulateErrorMsg();

		this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);
	}
	m_oSectionDB.ReleaseWriterLock();
}

/*
* 函数介绍：得到使用oledb情况下的sql语句
* 输入参数：
* 输出参数：
* 返回值  ：string 数据库中指定字段字符串
*/
CString COleSqlServerDB::GetOleDbSql()
{
	return this->m_strOleDBSql;
}


//执行sql,more语句调用
BOOL COleSqlServerDB::ExecQuery( BOOL abUpdatableRow,BOOL abClearSession /*= TRUE */)
{
	this->m_bOleExecCalled = TRUE;
	USES_CONVERSION;
	if (this->m_bConnected)
	{
		try
		{
			this->ClearRecordMemory();
			this->CleanResultSet();
			this->MakeUpResultSet();
			if (abClearSession)
			{
				this->CleanSession();
				this->MakeUpSession();
			}

			if (!this->m_strDBName.IsEmpty())
			{
				CString lstrSqlDBuse;
				lstrSqlDBuse.Format("use %s",this->m_strDBName);
				CDBPropSet propset(DBPROPSET_ROWSET);
				propset.AddProperty(DBPROP_IRowsetScroll, true);
				propset.AddProperty(DBPROP_CANSCROLLBACKWARDS, true);
				propset.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
				HRESULT lhRet = this->m_pResultSet->Open((*this->m_pDbSession),CT2W(lstrSqlDBuse),&propset);
				this->ClearRecordMemory();
				this->CleanResultSet();
				this->MakeUpResultSet();
			}
			
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

		}

		CDBPropSet propset(DBPROPSET_ROWSET);	

		if (abUpdatableRow)
		{
			propset.AddProperty(DBPROP_IRowsetChange,true);

			if (this->GetDBServerType() != COleSqlServerDB::DB_SERVER_TYPE_SQL2000)
			{
				propset.AddProperty(DBPROP_IRowsetScroll, true);

				propset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);

			}else	
			{
				propset.AddProperty(DBPROP_IRowsetScroll, true);

				propset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE );

			}

			this->m_pResultSet->SetBlobHandling(DBBLOBHANDLING_NOSTREAMS);


		}else
		{
			propset.AddProperty(DBPROP_IRowsetScroll, true);

			propset.AddProperty(DBPROP_CANSCROLLBACKWARDS, true);

			propset.AddProperty(DBPROP_CANFETCHBACKWARDS, true);

			this->m_pResultSet->SetBlobHandling(DBBLOBHANDLING_NOSTREAMS);
		}


		_DECLARE_PERF_MEASURE_TIME();

		_BEGIN_PERF_MEASURE_TIME();			

		HRESULT lhRet = this->m_pResultSet->Open((*this->m_pDbSession),CT2W(this->m_strOleDBSql),&propset);


		if((lhRet == E_NOINTERFACE || lhRet!= S_OK) && abUpdatableRow)
		{
			if (this->GetDBServerType() == COleSqlServerDB::DB_SERVER_TYPE_SQL2000)
			{
				CDBPropSet loPropset(DBPROPSET_ROWSET);

				this->ClearRecordMemory();

				loPropset.AddProperty(DBPROP_IRowsetChange,true);

				loPropset.AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE);

				lhRet = this->m_pResultSet->Open((*this->m_pDbSession),CT2W(this->m_strOleDBSql),&loPropset);

				if (lhRet == E_NOINTERFACE)
				{
					this->Log(__FUNCTION__, 
						__LINE__, 
						"当前数据库不支持DBPROPVAL_UP_CHANGE, 无法更形波形和频谱数据");
				}
			}
		}

		BHLOG_INFO(STR("ExecQuery: %s",this->m_strOleDBSql));

		_END_PERF_MEASURE_TIME("ExecQuery");

		if (S_OK == lhRet )
		{
			DBSTATUS loStatus = NULL;

			if(this->m_pResultSet->m_spRowset != NULL)
			{
				this->m_pResultSet->GetApproximatePosition(0,0,&this->m_ulOleDbDataCount);
			}

			zdlTraceLine(_T("ExecQuery Sql Row Count : %d"),this->m_ulOleDbDataCount);

			return TRUE;

		}else
		{
			/*				ASSERT(FALSE);*/
			this->PopulateErrorMsg();

			return FALSE;
		}
	}else
	{
		ASSERT(FALSE);

		return FALSE;
	}		
}

/*
* 函数介绍：关闭数据库连接
* 输入参数：
* 输出参数：
* 返回值  ：
*/
void COleSqlServerDB::Close()
{
	if (this->m_bUseOleDB)
	{
		try
		{
			this->m_pResultSet->Close();
			this->m_pDbSession->Close();
			this->m_oDb.Close();
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			ASSERT(FALSE);
		}

		this->m_bConnected = FALSE;

		this->m_bOleExecCalled = FALSE;

		this->m_ulOleDbDataCount = 0;

		this->m_nLastErrrorCode = 0;
	}else
	{
		ASSERT(FALSE);

		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			this->m_pDb->Close();
		}

	}

}

/*
* 函数介绍：用缓冲区数据,更新结果集中字段数据
* 输入参数：@name 数据库字段名
* 输出参数：
* 返回值  ：
*/
bool COleSqlServerDB::SetBuffer( const char *name, BYTE *pBuf, long nSize )
{
	if (this->m_bUseOleDB)
	{
		long llData = 0;

		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}
			USES_CONVERSION;

			DBTYPE loDBType = DBTYPE_NULL;

			DBORDINAL loId = 0;

			if (this->m_pResultSet->GetOrdinal(A2W(name),&loId))
			{
				return this->SetBuffer(loId,pBuf,nSize);

			}else
			{
				this->PopulateErrorMsg();

				return false;
			}

		}catch(...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			zdlTraceLine(_T("SetBuffer( const char *name, BYTE *pBuf, long nSize ) failed "));

			ASSERT(FALSE);

			this->PopulateErrorMsg();

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetBuffer(name,pBuf,nSize);
		}

		return false;

	}

};	

bool COleSqlServerDB::SetBuffer( const char *name, BYTE *pBuf, long nSize, bool anEncrypt )
{
	if (this->m_bUseOleDB)
	{
		long llData = 0;

		try
		{
			if (!this->IsDbSafe())
			{
				return false;
			}
			USES_CONVERSION;

			DBTYPE loDBType = DBTYPE_NULL;

			DBORDINAL loId = 0;

			if (this->m_pResultSet->GetOrdinal(A2W(name),&loId))
			{
				return this->SetBuffer(loId,pBuf,nSize,anEncrypt);

			}else
			{
				this->PopulateErrorMsg();

				return false;
			}

		}catch(...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			zdlTraceLine(_T("SetBuffer( const char *name, BYTE *pBuf, long nSize ) failed "));

			ASSERT(FALSE);

			this->PopulateErrorMsg();

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);
		if (this->m_pDb)
		{
			return this->m_pDb->SetBuffer(name,pBuf,nSize);
		}

		return false;

	}

};	

//!<执行Sql语名
bool COleSqlServerDB::Exec()
{
	this->m_pResultSet->FreeRecordMemory();

	BOOL lbRet = this->ExecQuery(FALSE);

	if (lbRet>0)
	{
		return true;
	}

	this->Log(__FUNCTION__,__LINE__,"ExecQuery Error Sql:%s \r\n %s",this->m_strErrorMsg,this->m_strOleDBSql);

	return false;
}

volatile BOOL gBDBTypeTraced = FALSE;


INT COleSqlServerDB::GetDBServerType()
{
	return COleSqlServerDB::m_nDbServerType;
}

BOOL COleSqlServerDB::JudgeDbType()
{
	USES_CONVERSION;

	if (this->m_bConnected && (!gBDBTypeTraced))
	{

		this->Cmd("select @@version");

		if (this->More())
		{
			CString lstrVersion  = this->Get(1,TRUE).GetBuffer(0);

			if (lstrVersion.Find(_T("9.00."))>=0)
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL2005;

			}else if (lstrVersion.Find(_T("8.00."))>=0)
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL2000;

			}else if (lstrVersion.Find(_T("10.00."))>=0)
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL2008;

			}else if (lstrVersion.Find(_T("7.00."))>=0)
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL7;

			}else if (lstrVersion.Find(_T("6.50."))>=0)
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL7;

			}else 
			{
				COleSqlServerDB::m_nDbServerType = DB_SERVER_TYPE_SQL2000;
			}

			this->Log(__FUNCTION__,__LINE__,"******************** DataBase Env \r\n \t%s",lstrVersion);

			gBDBTypeTraced = TRUE;

		}else
		{
			this->Log(__FUNCTION__,__LINE__,"******************** DataBase Env UnkownDb\r\n ");

			gBDBTypeTraced = TRUE;
		}

		this->ClearSql();
	}

	return COleSqlServerDB::m_nDbServerType;
}

BOOL COleSqlServerDB::IsDbSafe(BOOL abForceCheckDbSql/* = FALSE*/)
{
	if (!this->m_bConnected)
	{
		this->Init();	
	}

	if (this->m_bConnected)
	{
		if (abForceCheckDbSql)
		{
			this->Cmd("select 1 from sysfiles where 1=0");

			if (!this->Exec())
			{
				BOOL lbRet =  this->Init();

				if (!lbRet)
				{
					this->Log(__FUNCTION__,__LINE__,"数据库连接错误!");
				}

				return lbRet;
			}
		}

		return TRUE;
	}

	this->Log(__FUNCTION__,__LINE__,"数据库连接错误!");

	return FALSE;
}
//!<用缓冲区数据,更新结果集中字段数据
bool COleSqlServerDB::SetBuffer(const int lnFieldIndex, BYTE *pBuf, long nSize )
{	
	/*return true;*/

	if (this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		if (NULL == pBuf )
		{
			return false;
		}

		try
		{
			DBTYPE loDWBbType = 0;

			BOOL lbResult = this->m_pResultSet->GetColumnType(lnFieldIndex,&loDWBbType);

			int lnOriginalDatalength = this->GetActualSize(lnFieldIndex);

			DBSTATUS  ldwDbStatus = DBSTATUS_S_DEFAULT;	

			CBlobStream*   lpBlob = new CBlobStream();

			ULONG lnDataWritten = 0;
			
			HRESULT lhr = S_OK;

			/************************************************************************/
			/* encrypt begin                                                        */
			/************************************************************************/

			if (EncryptBuffer(lpBlob,pBuf,nSize,&lnDataWritten)<=0)
			{
				lhr = lpBlob->Write(pBuf,nSize,&lnDataWritten);
			}	

			/************************************************************************/
			/* encrypt end                                                        */
			/************************************************************************/
			
			if (S_OK != lhr)
			{
				this->m_pResultSet->SetStatus(lnFieldIndex,DBSTATUS_S_ISNULL);

				this->m_pResultSet->SetLength(lnFieldIndex,0);

				delete lpBlob;

				lpBlob = NULL;

				ASSERT(FALSE);

				return false;
			}

			IUnknown ** lpUnk = NULL;

			DBTYPE ldwType;

			this->m_pResultSet->GetColumnType(lnFieldIndex,&ldwType);

			DBTYPE ldwTypeComputed = ldwType & DBTYPE_IUNKNOWN;


			try
			{
				this->m_pResultSet->GetStatus(lnFieldIndex,&ldwDbStatus);					

				if (DBSTATUS_S_OK == ldwDbStatus && (ldwTypeComputed == DBTYPE_IUNKNOWN))
				{
					IUnknown **  ppStream = (IUnknown**)this->m_pResultSet->GetValue(lnFieldIndex);

					if (ppStream)
					{
						if (*ppStream)
						{
							(*ppStream)->Release();
						}
					}

				}

				//TRACE(_T("*********** column index :%d ***********\r\n"),lnFieldIndex);

				/*memset(rgBinding, 0, sizeof(DBBINDING));*/

				DBOBJECT       loObjectStruct;

				DBBINDING      loRgBinding[1] = 
				{
					lnFieldIndex,                 // 列
						0,                            // 
						sizeof(IUnknown*),            // 
						0,                            // 
						NULL,                         //
						&loObjectStruct,                // 
						NULL,                         // 
						DBPART_VALUE|DBPART_LENGTH,   // 
						DBMEMOWNER_CLIENTOWNED,       // Consumer owned memory
						DBPARAMIO_NOTPARAM,           // Not a parameter binding
						0,                            // Ignore maxlength
						0,                            // Reserved
						DBTYPE_IUNKNOWN,              // Type DBTYPE_IUNKNOWN
						0,                            // Precision not applicable
						0,                            // Scale not applicable
				};

				loObjectStruct.dwFlags = STGM_READ ;

				loObjectStruct.iid = IID_ISequentialStream;					

				CComQIPtr<IAccessor> lospIAccessor = this->m_pResultSet->m_spRowset;

				HACCESSOR lhAccessor;

				DBBINDSTATUS   lrgStatus[1];

				HRESULT hr = lospIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 
					1, 
					loRgBinding,
					sizeof(IUnknown *) + sizeof(ULONG),
					&lhAccessor, 
					lrgStatus);					

				BYTE* lpData=(BYTE*)CoTaskMemAlloc(sizeof(IUnknown*)+sizeof(DBLENGTH));

				*(ISequentialStream**)(lpData+loRgBinding[0].obValue)=lpBlob;

				*(DBLENGTH*)(lpData+loRgBinding[0].obLength)=lnDataWritten;

				if (this->m_pResultSet->m_spRowsetChange == NULL)
				{

				}else
				{
					lhr = this->m_pResultSet->m_spRowsetChange->SetData(this->m_pResultSet->m_hRow,lhAccessor,lpData);
				}

				CoTaskMemFree(lpData);

				if (lhr!=S_OK && lhr != DB_E_BADROWHANDLE)
				{
					this->PopulateErrorMsg();

					this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);
					this->Log(__FUNCTION__,__LINE__,this->m_strOleDBSql);

					return false;
				}

				return true;

			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);

				ASSERT(FALSE);

				delete lpBlob;

				lpBlob = NULL;

				this->PopulateErrorMsg();

				this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

				return false;
			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			ASSERT(FALSE);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			return this->m_pDb->SetBuffer(lnFieldIndex,pBuf,nSize);
		}

		return false;

	}

};	

bool COleSqlServerDB::SetBuffer(const int lnFieldIndex, BYTE *pBuf, long nSize,bool abEncrypt )
{	
	/*return true;*/

	if (this->m_bUseOleDB)
	{
		if (!this->IsDbSafe())
		{
			return false;
		}

		if (NULL == pBuf )
		{
			return false;
		}

		try
		{
			DBTYPE loDWBbType = 0;

			BOOL lbResult = this->m_pResultSet->GetColumnType(lnFieldIndex,&loDWBbType);

			int lnOriginalDatalength = this->GetActualSize(lnFieldIndex);

			DBSTATUS  ldwDbStatus = DBSTATUS_S_DEFAULT;	

			CBlobStream*   lpBlob = new CBlobStream();

			ULONG lnDataWritten = 0;

			HRESULT lhr = S_OK;

			/************************************************************************/
			/* encrypt begin                                                        */
			/************************************************************************/
			if(abEncrypt)
			{
				if (EncryptBuffer(lpBlob,pBuf,nSize,&lnDataWritten)<=0)
				{
					lhr = lpBlob->Write(pBuf,nSize,&lnDataWritten);
				}	
			}
			else
			{
				lhr = lpBlob->Write(pBuf,nSize,&lnDataWritten);
			}

			/************************************************************************/
			/* encrypt end                                                        */
			/************************************************************************/

			if (S_OK != lhr)
			{
				this->m_pResultSet->SetStatus(lnFieldIndex,DBSTATUS_S_ISNULL);

				this->m_pResultSet->SetLength(lnFieldIndex,0);

				delete lpBlob;

				lpBlob = NULL;

				ASSERT(FALSE);

				return false;
			}

			IUnknown ** lpUnk = NULL;

			DBTYPE ldwType;

			this->m_pResultSet->GetColumnType(lnFieldIndex,&ldwType);

			DBTYPE ldwTypeComputed = ldwType & DBTYPE_IUNKNOWN;


			try
			{
				this->m_pResultSet->GetStatus(lnFieldIndex,&ldwDbStatus);					

				if (DBSTATUS_S_OK == ldwDbStatus && (ldwTypeComputed == DBTYPE_IUNKNOWN))
				{
					IUnknown **  ppStream = (IUnknown**)this->m_pResultSet->GetValue(lnFieldIndex);

					if (ppStream)
					{
						if (*ppStream)
						{
							(*ppStream)->Release();
						}
					}

				}

				//TRACE(_T("*********** column index :%d ***********\r\n"),lnFieldIndex);

				/*memset(rgBinding, 0, sizeof(DBBINDING));*/

				DBOBJECT       loObjectStruct;

				DBBINDING      loRgBinding[1] = 
				{
					lnFieldIndex,                 // 列
						0,                            // 
						sizeof(IUnknown*),            // 
						0,                            // 
						NULL,                         //
						&loObjectStruct,                // 
						NULL,                         // 
						DBPART_VALUE|DBPART_LENGTH,   // 
						DBMEMOWNER_CLIENTOWNED,       // Consumer owned memory
						DBPARAMIO_NOTPARAM,           // Not a parameter binding
						0,                            // Ignore maxlength
						0,                            // Reserved
						DBTYPE_IUNKNOWN,              // Type DBTYPE_IUNKNOWN
						0,                            // Precision not applicable
						0,                            // Scale not applicable
				};

				loObjectStruct.dwFlags = STGM_READ ;

				loObjectStruct.iid = IID_ISequentialStream;					

				CComQIPtr<IAccessor> lospIAccessor = this->m_pResultSet->m_spRowset;

				HACCESSOR lhAccessor;

				DBBINDSTATUS   lrgStatus[1];

				HRESULT hr = lospIAccessor->CreateAccessor(DBACCESSOR_ROWDATA, 
					1, 
					loRgBinding,
					sizeof(IUnknown *) + sizeof(ULONG),
					&lhAccessor, 
					lrgStatus);					

				BYTE* lpData=(BYTE*)CoTaskMemAlloc(sizeof(IUnknown*)+sizeof(DBLENGTH));

				*(ISequentialStream**)(lpData+loRgBinding[0].obValue)=lpBlob;

				*(DBLENGTH*)(lpData+loRgBinding[0].obLength)=lnDataWritten;

				if (this->m_pResultSet->m_spRowsetChange == NULL)
				{

				}else
				{
					lhr = this->m_pResultSet->m_spRowsetChange->SetData(this->m_pResultSet->m_hRow,lhAccessor,lpData);
				}

				CoTaskMemFree(lpData);

				if (lhr!=S_OK && lhr != DB_E_BADROWHANDLE)
				{
					this->PopulateErrorMsg();

					this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);
					this->Log(__FUNCTION__,__LINE__,this->m_strOleDBSql);

					return false;
				}

				return true;

			}
			catch (...)
			{
				ASSERT(FALSE);
				CString lstrErrorLog;
				lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
				CHZLogManage::Fatal(lstrErrorLog);
				zdlTraceLine(lstrErrorLog);

				ASSERT(FALSE);

				delete lpBlob;

				lpBlob = NULL;

				this->PopulateErrorMsg();

				this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

				return false;
			}

		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			ASSERT(FALSE);

			this->PopulateErrorMsg();

			this->Log(__FUNCTION__,__LINE__,this->m_strErrorMsg);

			return false;
		}

	}else
	{
		ASSERT(this->m_pDb!=NULL);

		if (this->m_pDb)
		{
			return this->m_pDb->SetBuffer(lnFieldIndex,pBuf,nSize);
		}

		return false;

	}

};	


CString COleSqlServerDB::GetLastErrorString()
{
	return this->m_strErrorMsg;
}

BOOL COleSqlServerDB::PopulateErrorMsg()
{
	BOOL lbRet = FALSE;

	CDBErrorInfo loErrorInfo; 

	ULONG numRec = 0; 

	CComPtr<ISQLErrorInfo> pISQLErrorInfo = NULL; 

	LCID lcLocale = GetSystemDefaultLCID(); 

	loErrorInfo.GetErrorRecords(&numRec); 

	if (numRec) 
	{ 
		USES_CONVERSION;		

		CString lstrErr;

		try
		{
			for (int lnErron = 0;lnErron < numRec;lnErron++)
			{
				CComBSTR lspBstrErrStr,lspBstrSource,lspHelpStr; 

				HRESULT lhr = loErrorInfo.GetAllErrorInfo(lnErron,lcLocale,&lspBstrErrStr,&lspBstrSource,NULL,NULL,&lspHelpStr); 

				if (lhr == S_OK)
				{						

					if(NULL != lspBstrErrStr.m_str)
					{
						lstrErr.Append(W2T(lspBstrErrStr.m_str));
					}


					lstrErr.Append(_T(" "));	

					if(NULL != lspHelpStr.m_str)
					{
						lstrErr.Append(W2T(lspHelpStr.m_str));
					}


					lbRet = TRUE;

				}
			}
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
			this->Log(__FUNCTION__,__LINE__,_T("Get Error Msg Failed"));
		}

		this->m_strErrorMsg.Format(_T("Err:%s"),lstrErr);

		this->m_nLastErrrorCode = numRec;	
	} 				

	return lbRet;
}


void COleSqlServerDB::Log(CString astrFileOrFunc,int anLineNumber,const char *szCmdFormat,...)
{
	if (g_pLoger == NULL)
	{
		CString lstrData = this->m_strSqlLogPath.Trim();

		if (lstrData.GetLength() == 0)
		{
			this->m_strSqlLogPath = ".\\log";

			this->SetSqlLogPath(this->m_strSqlLogPath);
		}
	}

	if (NULL == szCmdFormat || NULL == g_pLoger)
	{
		return ;

	}

	USES_CONVERSION;

	va_list  args;

	va_start(args, szCmdFormat);

	va_list argListSave = args;

	int lnDataLength = GetFormatLength(szCmdFormat,args);

	CString lstrTempData;

	LPCTSTR lpszBuffer =  lstrTempData.GetBufferSetLength(lnDataLength);

	_vstprintf((TCHAR *)lpszBuffer, szCmdFormat, argListSave);

	lstrTempData.ReleaseBuffer(lnDataLength);

	CString lstrCMD = T2A(lstrTempData.GetBuffer(0));

	CString lstrModule;

	lstrModule.Format(_T("%s@%d"),astrFileOrFunc,anLineNumber);

	if (g_pLoger!=NULL)
	{
		try
		{
			CHZLogManage::Log(g_pLoger,lstrCMD,lstrModule,"",0x0020);
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);

			zdlTraceLine2(__FUNCTION__,__LINE__,_T("log error"));
		}

	}
}

bool COleSqlServerDB::ExecSetBuffer( const char *name, BYTE *pBuf, long nSize,const char *tablename, const char* sqlwhere )
{
	bool bRet = false;
	this->Cmd(_T("select * from %s where %s"),tablename,sqlwhere);
	if(this->More(TRUE))
	{
		bRet = this->SetBuffer(name,pBuf,nSize);
		if (!bRet)
		{
			//CMyLog::Trace_Log(_T("SetBuffer 失败！"));
		}
	}
	else
	{
		CString strLog;
		strLog.Format(_T("select * from %s where %s"),tablename,sqlwhere);
		//CMyLog::Trace_Log(strLog.GetBuffer());
	}
	return bRet;
}

CCommand<CDynamicAccessor,CRowset> * COleSqlServerDB::MakeUpResultSet()
{
	if (NULL == this->m_pResultSet)
	{
		this->m_pResultSet = new CCommand<CDynamicAccessor,CRowset>();
	}

	return this->m_pResultSet;
}

void COleSqlServerDB::CleanResultSet()
{
	if (NULL!=this->m_pResultSet)
	{
		delete this->m_pResultSet;
		this->m_pResultSet = NULL;
	}
}

CSession * COleSqlServerDB::MakeUpSession()
{
	if (NULL== this->m_pDbSession)
	{
		this->m_pDbSession = new CSession();
		this->m_pDbSession->Open(this->m_oDb);
	}
	return this->m_pDbSession;
}

void COleSqlServerDB::CleanSession()
{
	if (NULL!=this->m_pDbSession)
	{
		delete this->m_pDbSession;
		this->m_pDbSession = NULL;
	}
}



