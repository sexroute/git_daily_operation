#pragma once
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <string>
#include <shtypes.h>
#include <ShlObj.h>
#include <KDCommlib.h>
class CSqliteAdapter
{
public:
	CSqliteAdapter(void);
	
	virtual ~CSqliteAdapter(void);

	virtual std::string Get(int col);
	virtual std::string Get(const char* strField_);
	
	virtual double GetDouble(int col);
	virtual double GetDouble(const char* strField_);

	virtual int GetLong(int col);
	virtual int GetLong(const char* strField_);

	virtual BOOL Init();

	virtual BOOL CheckSafeDB();

	virtual BOOL Close();

	virtual BOOL exec(CString & astrSql);
	
	BOOL m_bOpened;
private:
	void * m_pSqlLiteDB;
};
