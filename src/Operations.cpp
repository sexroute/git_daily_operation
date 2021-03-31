#include "stdafx.h"

#include "Operations.h"


#define PATH_SEP '\\'

CString GetBaseName(const CString& asFilePath)
{
	int lnPathLen = asFilePath.GetLength();
	if (lnPathLen < 1)
	{
		return asFilePath;
	}

	int lnPathSep = asFilePath.ReverseFind(PATH_SEP);
	if (lnPathSep < 0)
	{
		return asFilePath;
	}

	return asFilePath.Right(lnPathLen - lnPathSep - 1);
}

CString GetDirName(const CString& asFilePath)
{
	int lnPathSep = asFilePath.ReverseFind(PATH_SEP);
	if (lnPathSep < 0)
	{
		return CString();
	}

	return asFilePath.Left(lnPathSep);
}

CString GetExecPath()
{
	CString lsFilePath;

	if (!::GetModuleFileName(NULL, lsFilePath.GetBuffer(MAX_PATH), MAX_PATH))
	{
		return lsFilePath;
	}

	lsFilePath.ReleaseBuffer();

	return lsFilePath;
}

CString GetExecFolder()
{
	return GetDirName(GetExecPath());
}

CString RenameExtension(const CString& asFilePath, LPCSTR asExt)
{
	if (NULL == asExt)
	{
		ASSERT(FALSE);
		return asFilePath;
	}

	int lnPathLen = asFilePath.GetLength();
	if (lnPathLen < 1)
	{
		ASSERT(FALSE);
		return asFilePath;
	}

	int lnExtSep = asFilePath.ReverseFind(_T('.'));
	if (lnExtSep < 0)
	{
		ASSERT(FALSE);
		return asFilePath;
	}

	return asFilePath.Left(lnExtSep) + asExt;
}

CString GetExecConf()
{
	return RenameExtension(GetExecPath(), _T(".ini"));
}






CString JoinPath( const CString & asPath, const CString & asSub )
{
	CString lsPath;

	lsPath.Format("%s\\%s", asPath, asSub);

	return lsPath;
}

bool ExistFile( const CString& asPath )
{
	if (asPath.IsEmpty())
	{
		return false;
	}

	return ::PathFileExists(asPath);
}

bool ExistFolder( const CString& asPath )
{
	if (asPath.IsEmpty())
	{
		return false;
	}

	return ::PathIsDirectory(asPath);
}

class COperationsLogger
{
public:
	COperationsLogger();

public:
	BOOL Log(const CString & asMessage);

private:
	CString GetLogFileName();

private:
	CString m_sLogFileName;
};

COperationsLogger::COperationsLogger()
{
	this->m_sLogFileName = GetLogFileName();

	CFile loLogFile;

	CString lsLogMsg = _T("\r\n");

	CFileException loException;

	if(loLogFile.Open(this->m_sLogFileName,
		CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::shareDenyWrite,
		&loException)) {

		try {
			loLogFile.SeekToEnd();
			loLogFile.Write(lsLogMsg.GetBuffer(), lsLogMsg.GetLength());
			loLogFile.Flush();
			loLogFile.Close();
		} catch (CFileException* e) {
			ASSERT(FALSE);

			TCHAR szCause[255] = {0};
			e->GetErrorMessage(szCause, 255);

			BHLOG_ERROR(STR("CFileException: %s", szCause));

			e->Delete();
		} catch (...) {
			ASSERT(FALSE);

			BHLOG_ERROR(STR("Exception"));
		}

	} else {

		TCHAR szCause[255] = {0};
		loException.GetErrorMessage(szCause, 255);

		BHLOG_ERROR(STR("操作日志文件异常: %s, %s", this->m_sLogFileName, szCause));
	}
}

BOOL COperationsLogger::Log( const CString & asMessage )
{
	if (!ExistFile(this->m_sLogFileName)) {
		this->m_sLogFileName = GetLogFileName();
	}

	CFile loLogFile;

	CFileException loException;

	if(!loLogFile.Open(this->m_sLogFileName,
		CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate|CFile::shareDenyWrite,
		&loException)){

		TCHAR szCause[255] = {0};
		loException.GetErrorMessage(szCause, 255);

		BHLOG_ERROR(STR("操作日志文件异常: %s, %s", this->m_sLogFileName, szCause));
		return FALSE;
	}

	COleDateTime loDateTime = COleDateTime::GetCurrentTime();

	CString lsLogMsg;lsLogMsg.GetBuffer(255);

	lsLogMsg.Append(_T("\r\n"));
	lsLogMsg.AppendFormat("[%s]", loDateTime.Format(gc_cTimeFormat));
	lsLogMsg.Append(asMessage);

	try {
		loLogFile.SeekToEnd();
		loLogFile.Write(lsLogMsg.GetBuffer(), lsLogMsg.GetLength());
		loLogFile.Flush();
		loLogFile.Close();
	} catch (CFileException* e) {
		ASSERT(FALSE);

		TCHAR szCause[255] = {0};
		e->GetErrorMessage(szCause, 255);

		BHLOG_ERROR(STR("CFileException: %s", szCause));

		e->Delete();
	} catch (...) {
		ASSERT(FALSE);

		BHLOG_ERROR(STR("Exception"));
	}

	return TRUE;
}


CString COperationsLogger::GetLogFileName()
{
	CString lsFileName;

	CString lsExecPath = GetExecPath();

	if (lsExecPath.IsEmpty()) {
		ASSERT(FALSE);

		BHLOG_ERROR(STR("操作日志文件异常"));
		return lsFileName;
	}

	CString lsExecFolder = GetDirName(lsExecPath);

	if (!ExistFolder(lsExecFolder)) {
		ASSERT(FALSE);

		BHLOG_ERROR(STR("操作日志文件异常"));
		return lsFileName;
	}

	CString lsLogFolder = JoinPath(lsExecFolder, _T("LOG"));

	if (!ExistFolder(lsLogFolder)) {
		lsLogFolder = lsExecFolder;
	}

	if (!ExistFolder(lsLogFolder)) {
		ASSERT(FALSE);

		BHLOG_ERROR(STR("操作日志文件异常"));
		return lsFileName;
	}

	lsFileName = JoinPath(lsLogFolder, _T("DSESVR-Operations.log"));

	return lsFileName;
}


static COperationsLogger theLogger;

BOOL Operations::Log( const CString & asMessage )
{
	ASSERT(!asMessage.IsEmpty());
	return theLogger.Log(asMessage);
}
