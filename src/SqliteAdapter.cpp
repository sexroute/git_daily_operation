
#include ".\sqliteadapter.h"
#include "SqlLite\SqliteADO.h"
#include <additions\DebugHelper.h>


#define  MAKE_SQLITE_PTR(p) ((CSqliteADO *)p)

#define  GET_SQLITE_PTR() ((CSqliteADO *)(this->m_pSqlLiteDB))


CSqliteAdapter::CSqliteAdapter(void)
{
	this->m_pSqlLiteDB = NULL;

	this->m_bOpened = FALSE;
	
}

BOOL CSqliteAdapter::CheckSafeDB()
{
	if (this->m_pSqlLiteDB == NULL || !this->m_bOpened)
	{
		return this->Init();
	}

	return TRUE;
}

BOOL CSqliteAdapter::exec(CString & astrSql)
{
	if (this->CheckSafeDB())
	{
		GET_SQLITE_PTR()->Cmd(astrSql);

		return GET_SQLITE_PTR()->More();
	}

	return FALSE;
}

BOOL CSqliteAdapter::Close()
{
	try
	{
		if (this->m_pSqlLiteDB!= NULL )
		{
			if (this->m_bOpened)
			{
				 GET_SQLITE_PTR()->Close();

				 this->m_bOpened = FALSE;

				 return TRUE;
			}
		}

		return TRUE;
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);	
		return FALSE;
	}

}

BOOL CSqliteAdapter::Init()
{
	try
	{
		if (this->m_pSqlLiteDB == NULL)
		{
			this->m_pSqlLiteDB = new CSqliteADO();
		}

		if (!this->m_bOpened)
		{
			this->m_bOpened = MAKE_SQLITE_PTR(this->m_pSqlLiteDB)->Open(".\\plantex.db3");			
		}

		return this->m_bOpened;
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

	return FALSE;
}

CSqliteAdapter::~CSqliteAdapter(void)
{
	try
	{
		if (this->m_pSqlLiteDB!= NULL)
		{
			if (this->m_bOpened)
			{
				this->Close();
			}

			delete this->m_pSqlLiteDB;
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
	}

}



double CSqliteAdapter::GetDouble(const char* strField_)
{
	if (this->CheckSafeDB())
	{
		return GET_SQLITE_PTR()->GetDouble(strField_);
	}

	ASSERT(FALSE);

	return 0;
}

double CSqliteAdapter::GetDouble(int col)
{
	if (this->CheckSafeDB())
	{
		return GET_SQLITE_PTR()->GetDouble(col);
	}

	ASSERT(FALSE);

	return 0;
}

int CSqliteAdapter::GetLong(const char* strField_)
{
	if (this->CheckSafeDB())
	{
		return GET_SQLITE_PTR()->GetLong(strField_);
	}

	ASSERT(FALSE);

	return 0;
}


int CSqliteAdapter::GetLong(int col)
{
	if (this->CheckSafeDB())
	{
		return GET_SQLITE_PTR()->GetLong(col);
	}

	return -1;
}

std::string CSqliteAdapter::Get(const char* strField_)
{

	if (this->CheckSafeDB())
	{
		const CHAR * lpRet =  GET_SQLITE_PTR()->Get(strField_);

		if (!AfxIsValidString(lpRet))
		{
			return "";
		}

		std::string lstr =  lpRet;

		return lstr;
	}

	return "";
}


std::string CSqliteAdapter::Get(int col)
{

	if (this->CheckSafeDB())
	{
		const CHAR * lpRet =  GET_SQLITE_PTR()->Get(col);

		if (!AfxIsValidString(lpRet))
		{
			return "";
		}

		std::string lstr =  lpRet;

		return lstr.c_str();

	}

	return "";
}
