// KDCommLib.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include <afxdllx.h>
#include "MutexRW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE KDCommLibDLL = { NULL, NULL };

CMutexRW  * g_pWatchDogFileLock ;

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("KDCommLib.DLL 正在初始化！\n");
		
		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(KDCommLibDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL(如 ActiveX 控件)隐式链接到
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(KDCommLibDLL);

		if (g_pWatchDogFileLock == NULL)
		{
			g_pWatchDogFileLock = new CMutexRW();
		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("KDCommLib.DLL 正在终止！\n");

		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(KDCommLibDLL);
	}
	return 1;   // 确定
}



extern "C"
{
	bool  __declspec(dllexport) Feed(const CString &strFile_)
	{

		if (NULL == g_pWatchDogFileLock)
		{
			return false;
		}

		bool lbRet = false;

		g_pWatchDogFileLock->BeginWrite();

		try
		{

			CString lstrFileName;

			int lnBufferSize = 2048;

			HANDLE loProcess = ::GetCurrentProcess();

			::GetModuleFileName(NULL,lstrFileName.GetBufferSetLength(lnBufferSize),lnBufferSize);

			lstrFileName.ReleaseBuffer(lnBufferSize);

			lstrFileName = lstrFileName.TrimRight();

			int lnStrLength = lstrFileName.StringLength(lstrFileName) -4;

			lstrFileName = lstrFileName.Mid(0,lnStrLength);

			lstrFileName += _T(".txt");

			if (strFile_.CompareNoCase(_T("")) != 0)
			{
				lstrFileName = strFile_ ;
			}

			HANDLE lhFile =  CreateFile(lstrFileName.GetBuffer(0),
				GENERIC_READ|GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_DELETE,
				NULL,OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS,
				NULL);

			BOOL lbFileOpened = FALSE;

			if (lhFile == INVALID_HANDLE_VALUE)
			{
				HANDLE lhd = ::CreateFile(lstrFileName.GetBuffer(0),
					GENERIC_READ|GENERIC_WRITE, 
					FILE_SHARE_READ|FILE_SHARE_WRITE,
					NULL, OPEN_ALWAYS, 
					FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_HIDDEN, 
					NULL);

				if (lhd == INVALID_HANDLE_VALUE)
				{
					ASSERT(FALSE);
				}else
				{
					lhFile = lhd;

					lbFileOpened = TRUE;

				}
			}else
			{
				lbFileOpened = TRUE;
			}

			if (lbFileOpened)
			{
				FILETIME loLastWriteTime; // 对文件夹的最近修改时间 

				FILETIME loCreationTime; // 文件夹的创建时间 

				FILETIME loLastAccessTime; // 对文件夹的最近访问时间 

				BOOL lbRet = GetFileTime(lhFile,&loCreationTime,&loLastAccessTime,&loLastWriteTime);

				if (!lbRet)
				{
					ASSERT(FALSE);
				}

				SYSTEMTIME loSt;

				::GetSystemTime(&loSt);

				::SystemTimeToFileTime(&loSt,&loLastWriteTime);

				SetFileTime(lhFile,&loCreationTime,&loLastAccessTime,&loLastWriteTime);

				CloseHandle(lhFile);
			}

			lbRet = true;

		}
		catch (...)
		{
			ASSERT(FALSE);

			
		}

		g_pWatchDogFileLock->EndWrite();


		return lbRet;
	}

};

