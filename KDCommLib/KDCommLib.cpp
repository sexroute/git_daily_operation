// KDCommLib.cpp : ���� DLL �ĳ�ʼ�����̡�
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
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("KDCommLib.DLL ���ڳ�ʼ����\n");
		
		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(KDCommLibDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL(�� ActiveX �ؼ�)��ʽ���ӵ�
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(KDCommLibDLL);

		if (g_pWatchDogFileLock == NULL)
		{
			g_pWatchDogFileLock = new CMutexRW();
		}

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("KDCommLib.DLL ������ֹ��\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(KDCommLibDLL);
	}
	return 1;   // ȷ��
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
				FILETIME loLastWriteTime; // ���ļ��е�����޸�ʱ�� 

				FILETIME loCreationTime; // �ļ��еĴ���ʱ�� 

				FILETIME loLastAccessTime; // ���ļ��е��������ʱ�� 

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

