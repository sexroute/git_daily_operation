#include "stdafx.h"
#include "../include/additions/RemoteServer.h"
#include <Dbghelp.h>
#include <Psapi.h>
#include <atlconv.h>
#include <process.h>
#include <time.h>
#include <Psapi.h>
#include <Wtsapi32.h>
#include <Userenv.h>
#include <vector>
#include <atlbase.h>
#include <ATLComTime.h>
#include <TlHelp32.h>

//#include "base64.h"
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"version.lib")
#pragma comment(lib,"psapi.lib")

#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib,"Userenv.lib")


/************************************************************************/
/* 路径处理函数                                                                                    */
/************************************************************************/
void replaceFirst(TCHAR *str1,TCHAR *str2,TCHAR *str3)  
{  
	if (strlen(str1)>=10240)
	{
		return ;
	}
	TCHAR str4[10240]={0};  
	TCHAR *p;  
	strncpy(str4,str1,strlen(str1));  
	if((p=strstr(str1,str2))!=NULL)
	{  
		while(str1!=p&&str1!=NULL)
		{  
			str1++;  
		}  
		str1[0]=_T('\0');
		strncat(str1,str3,strlen(str3));
		strncat(str1,strstr(str4,str2)+strlen(str2),strlen(strstr(str4,str2)+strlen(str2)));
	}  
}
void replace(TCHAR *str1,TCHAR *str2,TCHAR *str3)  
{  
	while(strstr(str1,str2)!=NULL)  
	{  
		replaceFirst(str1,str2,str3);  
	}  

}
#include <WinBase.h>
#ifndef GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
#define GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS (0x00000004)
#endif

HMODULE GetCurrentModule()
{ // NB: XP+ solution!
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModule,
		&hModule);

	return hModule;
}

int GetCurrentExePath(TCHAR * apBuffer,int anLength)
{

	int lnTChars = GetModuleFileName(NULL, apBuffer, anLength);
	if (lnTChars<=0||lnTChars>=anLength)
	{
		return lnTChars;
	}
	apBuffer[lnTChars] = _T('\0');
	replace(apBuffer,_T("\\"),_T("/"));
	TCHAR * lpFound = strrchr(apBuffer,_T('/'));
	if (NULL!=lpFound)
	{
		int lnIndex =lpFound- apBuffer+1;
		apBuffer[lnIndex] = _T('\0');
		return lnIndex;
	}
	return lnTChars;
}


int GetCurrentPath(TCHAR * apBuffer,int anLength)
{
	int lnTChars = GetModuleFileName(GetCurrentModule(), apBuffer, anLength);
	if (lnTChars<=0||lnTChars>=anLength)
	{
		return lnTChars;
	}
	apBuffer[lnTChars] = _T('\0');
	replace(apBuffer,_T("\\"),_T("/"));
	TCHAR * lpFound = strrchr(apBuffer,_T('/'));
	if (NULL!=lpFound)
	{
		int lnIndex =lpFound- apBuffer+1;
		apBuffer[lnIndex] = _T('\0');
		return lnIndex;
	}
	return lnTChars;
}

/************************************************************************/
/* 加密函数定义相关的全局变量                                                             */
/************************************************************************/
typedef unsigned char byte;
typedef  int   ( * sub_DA39A0)(int a1, int a2, int a3, int a4, int a5, byte* * a6, int * a7);
typedef void (* testv)();
int g_offsetDA39A0 = 0x00DA39A0;

/************************************************************************/
/*	机器特征码地址																					*/
/************************************************************************/
int g_offset15665BC = 0x15665BC;

/************************************************************************/
/* 生成kbsync第一个参数的函数地址803D90                                                         */
/************************************************************************/
typedef   int (*sub_GenerateParameter)(int &Exchange);
//int g_offset803D90 = 0x803D90;

/************************************************************************/
/* 构造this的地址函数地址803C50                                                        */
/************************************************************************/
typedef int (*sub_803C50)(int *apThis);
int g_offset803C50 = 0x803C50;

typedef int (*sub_804100)(int * apThis,int a,int b,int c,int * apData);
int g_offset804100 = 0x804100;

typedef int (*sub_803D90)(int *apThis,int *apKey);
int g_offset803D90 = 0x803D90;

sub_DA39A0 funcDA39A0 =NULL;
sub_803C50 func803C50=NULL;
sub_803D90 fun803D90=NULL ;
sub_804100 fun804100=NULL ;

DWORD_PTR lpKey =NULL;

/************************************************************************/
/* simulate login                                                                                  */
/************************************************************************/

/************************************************************************/
/*	hook相关的全局变量																		    */
/************************************************************************/
typedef int (__stdcall *OLD_GetComputerNameW)( LPWSTR lpBuffer,
	LPDWORD nSize );
typedef int (__stdcall *OLD_MessageboxW)(  HWND hWnd,
	 LPCWSTR lpText,
	 LPCWSTR lpCaption,
	 UINT uType );
OLD_GetComputerNameW g_procOldGetComputerName = NULL;
OLD_MessageboxW g_procOldMessageboxW = NULL;
int __stdcall  FakeMessageboxW(HWND hWnd,
	LPCWSTR lpText,
	LPCWSTR lpCaption,
	UINT uType)
{
	//MessageBoxA(NULL,"MessageBoxW hooked",NULL,MB_OK);
	int lnRet = g_procOldMessageboxW(hWnd,lpText,lpCaption,uType);
	return lnRet;

}
#include <iostream>
#include <string>

int writefile(byte * lpbuffer,int anSize,char * apfileName)
{
	FILE * outfile;
	 outfile = fopen(apfileName, "wb" ); 
	 fwrite( lpbuffer, sizeof( byte), anSize, outfile );  
	 fclose(outfile);
	return 0;
}



BOOL SetPrivilege(
	HANDLE hToken,          // token handle
	LPCTSTR Privilege,      // Privilege to enable/disable
	BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
	)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;
	TOKEN_PRIVILEGES tpPrevious;
	DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

	if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;

	// 
	// first pass.  get current privilege setting
	// 
	tp.PrivilegeCount           = 1;
	tp.Privileges[0].Luid       = luid;
	tp.Privileges[0].Attributes = 0;

	AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		&tpPrevious,
		&cbPrevious
		);

	if (GetLastError() != ERROR_SUCCESS) return FALSE;

	// 
	// second pass.  set privilege based on previous setting
	// 
	tpPrevious.PrivilegeCount       = 1;
	tpPrevious.Privileges[0].Luid   = luid;

	if(bEnablePrivilege) {
		tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
	}
	else {
		tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
			tpPrevious.Privileges[0].Attributes);
	}

	AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tpPrevious,
		cbPrevious,
		NULL,
		NULL
		);

	if (GetLastError() != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

int CRemoteSesrver::replace_IAT(const char *pDllName,const char *pApiName,bool bReplace,DWORD * apOriginalFunc,DWORD apFakeFunc)
{
	HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_TERMINATE, FALSE, GetCurrentProcessId());

	if (INVALID_HANDLE_VALUE == hProcess || NULL == hProcess)
	{

		HANDLE hToken = NULL;

		if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
		{
			if (GetLastError() == ERROR_NO_TOKEN)
			{
				if (!ImpersonateSelf(SecurityImpersonation))
					return -4;

				if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
				{
					return -5;
				}
			}
			else
			{
				return -6;
			}
		}

		HandleWatchDog loWatchDog;
		loWatchDog.SetHandle(hToken);

		if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
		{
			return -7;
		}

		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ|PROCESS_TERMINATE, FALSE, GetCurrentProcessId());
	}
	
	const int c_nMaxSize = 1024;
	HMODULE ahMod[c_nMaxSize];
	DWORD dwNeededSize = 0;
	 int lbFound = 0;
	 if( EnumProcessModules(hProcess, ahMod, sizeof(ahMod), &dwNeededSize))
	 {

		 int lnTotalCount = dwNeededSize/sizeof(HMODULE);
		 
		 for (int i=0;i<lnTotalCount;i++)
		 {
			 HMODULE lhDst = ahMod[i];
			 DWORD dwSize = 0;
			 PIMAGE_IMPORT_DESCRIPTOR pImageImport = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(lhDst,TRUE,
				 IMAGE_DIRECTORY_ENTRY_IMPORT,&dwSize);
			 if (NULL == pImageImport)
			{
				continue;
			 }
			 PIMAGE_IMPORT_BY_NAME pImageImportByName = NULL;
			 PIMAGE_THUNK_DATA  pImageThunkOriginal = NULL;
			 PIMAGE_THUNK_DATA  pImageThunkReal  = NULL;
			 while (pImageImport->Name)
			 {
				 if (0 == strcmpi((char*)((PBYTE)lhDst+pImageImport->Name),pDllName))
				 {
					 break;
				 }
				 ++pImageImport;
			 }
			 if (! pImageImport->Name)
			 {
				 continue;
			 }
			 pImageThunkOriginal = (PIMAGE_THUNK_DATA)((PBYTE)lhDst+pImageImport->OriginalFirstThunk  );
			 pImageThunkReal = (PIMAGE_THUNK_DATA)((PBYTE)lhDst+pImageImport->FirstThunk   );
			
			 while (pImageThunkOriginal->u1.Function)
			 {
				 if ((pImageThunkOriginal->u1 .Ordinal & IMAGE_ORDINAL_FLAG) != IMAGE_ORDINAL_FLAG)
				 {
					 pImageImportByName = (PIMAGE_IMPORT_BY_NAME)((PBYTE)lhDst+pImageThunkOriginal->u1 .AddressOfData );
					 char log[1024]={0};
					 sprintf(log,"%s:%s\r\n",pDllName,pImageImportByName->Name);
					 OutputDebugString(log);
					 if (0 == strcmpi(pApiName,(char*)pImageImportByName->Name))
					 {
						 MEMORY_BASIC_INFORMATION mbi_thunk;
						 VirtualQuery(pImageThunkReal, &mbi_thunk, sizeof(MEMORY_BASIC_INFORMATION)); 
						 VirtualProtect(mbi_thunk.BaseAddress,mbi_thunk.RegionSize, PAGE_READWRITE, &mbi_thunk.Protect); 
						 lbFound++;
						 if (true == bReplace)
						 {
							 *apOriginalFunc = (pImageThunkReal->u1.Function); 
							 pImageThunkReal->u1.Function = (DWORD) apFakeFunc;
						 }
						 else
						 {
							 pImageThunkReal->u1.Function = (DWORD)g_procOldGetComputerName;
						 }
						 DWORD dwOldProtect; 
						 VirtualProtect(mbi_thunk.BaseAddress, mbi_thunk.RegionSize, mbi_thunk.Protect, &dwOldProtect); 
						 break;;
					 }


				 }
				 ++pImageThunkOriginal;
				 ++pImageThunkReal;
			 }
			
		 }

	 }

	 if (lbFound>0)
	 {
		 return 0;
	 }else
	 {
		 return -1;
	 }

}
BOOL  CRemoteSesrver::IsPortAvailable(int anPort)
{
	unsigned short lusPortTest = anPort;

	long lSockTest = (long)socket(AF_INET, SOCK_STREAM, 0);

	if(0 > lSockTest)
	{
		return FALSE;
	}

	//绑定本地地址
	struct sockaddr_in  serverAddr;
	memset((char *)&serverAddr, 0, (long)sizeof(serverAddr));     
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = ::inet_addr(_T("0.0.0.0"));
	serverAddr.sin_port = htons(lusPortTest);

	if (0 > bind ((SOCKET)lSockTest, (struct sockaddr *)&serverAddr, sizeof(serverAddr)))
	{
		(void)closesocket((UINT)lSockTest);

		return FALSE;
	}
	shutdown(lSockTest,SD_BOTH);
	(void)closesocket((UINT)lSockTest);

	return TRUE;
}

BOOL CRemoteSesrver::DetectAvailablePort(int & anPort)
{
	if (IsPortAvailable(anPort))
	{
		return TRUE;
	}

	for (int i =25000;i<26000;i++)
	{
		if (IsPortAvailable(i))
		{
			anPort = i;

			return TRUE;
		}
	}

	return FALSE;
}






/************************************************************************/
/* 大概原理就是:  首先需要生成两个6字节的机器相关的信息，然后加上你的苹果账号ID，
	传递一个函数里面会生成一个DWORD，后面又会用该DWORD作为参数调用另外一个函数，
	生成一些东西。。最后是BASE64加密。。                                          */
/************************************************************************/


HWND lmshwnd = NULL;


int GetVersion(TCHAR * apBuffer,int anLength)
{
	if (NULL == apBuffer || anLength ==0)
	{
		return 0;
	}
	TCHAR lpSzPath[10241]={0};
	int lnPath = ::GetCurrentExePath(lpSzPath,10240);
	if (lnPath<=0)
	{
		return 0;
	}
	TCHAR lpSzDllFileName[20480]={0};
	sprintf(lpSzDllFileName,_T("%siTunes.dll"),lpSzPath);
	DWORD  verHandle = NULL;
	UINT   size      = 0;
	LPBYTE lpBuffer  = NULL;
	DWORD  verSize   = GetFileVersionInfoSize( lpSzDllFileName, &verHandle);

	if (verSize != NULL)
	{
		LPSTR verData = new char[verSize];

		if (GetFileVersionInfo( lpSzDllFileName, verHandle, verSize, verData))
		{
			if (VerQueryValue(verData,"\\",(VOID FAR* FAR*)&lpBuffer,&size))
			{
				if (size)
				{
					VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
					if (verInfo->dwSignature == 0xfeef04bd)
					{
						int major = HIWORD(verInfo->dwFileVersionMS);
						int minor = LOWORD(verInfo->dwFileVersionMS);
						int lmajor = HIWORD(verInfo->dwFileVersionLS);
						int lminor =  LOWORD(verInfo->dwFileVersionLS);
						sprintf(apBuffer,_T("%d.%d.%d.%d"),major,minor,lmajor,lminor);
					}
				}
			}
		}
		delete[] verData;
	}

	return 1;
}
typedef void (__stdcall * pAbortType)();
pAbortType * g_procOldAbort = NULL;
#include "mdump.h"
void __stdcall CustomAbort()
{
	__try
	{
		volatile int a = 0;
		int b = 1/a;
	}
	__except(Winet::MiniDumper::TopLevelFilter(GetExceptionInformation()))
	{
	}
	
}
BOOL CRemoteSesrver::DisableStandardAbort71()
{
	CString lstrDLL = "msvcr71.dll";
#ifdef _DEBUG
	lstrDLL = "msvcr71d.dll";
#endif
	HMODULE hcrt = LoadLibrary(lstrDLL);
	HandleWatchDog loWatchDog;
	loWatchDog.SetHandle(hcrt);
	VOID *  set_abort_behavior= (VOID *) GetProcAddress(hcrt, "abort");
	if (set_abort_behavior)
	{
		int lnRet = CRemoteSesrver::replace_IAT(lstrDLL,
												"abort",
												true,
												(DWORD *)&g_procOldAbort,
												(DWORD)CustomAbort);

		if (lnRet==0)
		{
			return TRUE;
		}
	}

	return FALSE;
}

const int c_nMaxSize  = 1024;


DWORD GetCurrentLogonUserSessionID(int anIndex)
{
	int lnDataCount = 0;

	PWTS_SESSION_INFOA  lpSessionInfo = NULL;

	DWORD ldwCount = 0;

	DWORD ldwSessionID = -1;

	BOOL lbRet = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE,0,1,&lpSessionInfo,&ldwCount);

	char szTmp[c_nMaxSize] = "";

	char szItem[c_nMaxSize] = "";

	sprintf(szItem, "Process%d", anIndex);

	int lnDesiredSessionId = -1;

	int lnStandById = -1;

	if (lbRet)
	{
		if (lnDesiredSessionId>=0)
		{
			for (UINT i =0;i<ldwCount;i++)
			{

				WTS_SESSION_INFO si = lpSessionInfo[i];

				if (((WTSActive == si.State) || (WTSDisconnected == si.State)))
				{
					lnStandById = si.SessionId;
					if (( si.SessionId == lnDesiredSessionId))
					{
						ldwSessionID = si.SessionId;
						break;
					}
				}


			}
		}else
		{
			CString lstrName;
			for (UINT i =0;i<ldwCount;i++)
			{
				lstrName = lpSessionInfo[i].pWinStationName;
				TRACE(_T("%s : %d \r\n"),lpSessionInfo[i].pWinStationName,lpSessionInfo[i].SessionId);	

				if ((WTSActive == lpSessionInfo[i].State) && lstrName.CompareNoCase("Services")!=0)
				{
					ldwSessionID = lpSessionInfo[i].SessionId;

					break;
				}			
			}

			for (UINT i =0;i<ldwCount;i++)
			{
				lstrName = lpSessionInfo[i].pWinStationName;
				TRACE(_T("%s : %d \r\n"),lpSessionInfo[i].pWinStationName,lpSessionInfo[i].SessionId);	

				if ((WTSDisconnected == lpSessionInfo[i].State)&& lstrName.CompareNoCase("Services")!=0)
				{
					ldwSessionID = lpSessionInfo[i].SessionId;

					break;
				}			
			}

		}


		if (lpSessionInfo!=NULL)
		{
			WTSFreeMemory(lpSessionInfo);

			lpSessionInfo = NULL;

		}
	}
	if (ldwSessionID<0)
	{
		if (lnStandById<0)
		{
			ldwSessionID = WTSGetActiveConsoleSessionId();
		}else
		{
			ldwSessionID = lnStandById;
		}

	}

	return ldwSessionID;
}


BOOL GetCurrentUserToken(HANDLE & ahToken,int anIndex)
{

	HANDLE hTokenThis = NULL;

	HANDLE hTokenDup = NULL;

	HANDLE hThisProcess = GetCurrentProcess();

	BOOL lbRet = OpenProcessToken(hThisProcess, TOKEN_ALL_ACCESS, &hTokenThis);

	if (lbRet)
	{
		lbRet = DuplicateTokenEx(hTokenThis, MAXIMUM_ALLOWED,NULL, SecurityIdentification, TokenPrimary, &hTokenDup);

		if (lbRet)
		{
			DWORD ldwSessionId = GetCurrentLogonUserSessionID(anIndex);

			if (-1!=ldwSessionId)
			{
				lbRet = SetTokenInformation(hTokenDup, TokenSessionId, &ldwSessionId, sizeof(DWORD));

				if (lbRet)
				{
					ahToken = hTokenDup;

				}else
				{
					lbRet = FALSE;
				}
			}else
			{
				lbRet = FALSE;
			}
		}else
		{
			lbRet = FALSE;
		}

	}

	if (NULL!= hTokenThis)
	{
		try
		{
			CloseHandle(hTokenThis);

		}catch(...)
		{

		}
	}

	if (NULL!= hThisProcess)
	{
		try
		{
			CloseHandle(hThisProcess);

		}catch(...)
		{

		}
	}

	if (!lbRet)
	{
		try
		{
			CloseHandle(hTokenDup);

		}catch(...)
		{

		}
	}

	return lbRet;
}

INT G_B_DebuggerStarted = FALSE;

BOOL InternalStartDebugger()
{
	BEGIN_ERROR_HANDLE;
	
	STARTUPINFO startUpInfo = { sizeof(STARTUPINFO),NULL,"",NULL,0,0,0,0,0,0,0,STARTF_USESHOWWINDOW,0,0,NULL,0,0,0};  

	BOOL lbUseCurrentUserDesktop = FALSE;

	HANDLE lhToken = NULL;

	LPVOID lpEnv = NULL;

	CString szCommandLine;
	szCommandLine.Format("DumpMonitor.exe %d",::GetCurrentProcessId());

	char CurrentDesktopName[c_nMaxSize] = "";

	char szTmp[c_nMaxSize] = "";

	char szItem[c_nMaxSize] = "";

	HDESK hCurrentDesktop = NULL;

	sprintf(szItem, "Process%d", 0);

	BOOL bUserInterface = 2;
	// find if there already exits the same process

	// set the correct desktop for the process to be started
	DWORD  ldwCreateFlag = NORMAL_PRIORITY_CLASS;

	if(bUserInterface)
	{
		if (2 == bUserInterface)
		{

			DWORD ldwActiveSessionId = GetCurrentLogonUserSessionID(0);

			if (-1 != ldwActiveSessionId)
			{
				//BOOL lbRet = WTSQueryUserToken(ldwActiveSessionId,&lhToken);

				BOOL lbRet = GetCurrentUserToken(lhToken,0);

				if (lbRet)
				{
					lbUseCurrentUserDesktop = TRUE;

					ldwCreateFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE|CREATE_UNICODE_ENVIRONMENT;

					if (CreateEnvironmentBlock(&lpEnv, lhToken, FALSE))
					{
						lbUseCurrentUserDesktop = TRUE;

					}else
					{
						lbUseCurrentUserDesktop = FALSE;

					}

				}
			}

			if (lbUseCurrentUserDesktop)
			{
				startUpInfo.wShowWindow = SW_HIDE;			
				startUpInfo.lpDesktop = "WinSta0\\default";
			}else
			{
				startUpInfo.wShowWindow = SW_HIDE;
				startUpInfo.lpDesktop = NULL;
			}
		}
		else
		{
			startUpInfo.wShowWindow = SW_HIDE;
			startUpInfo.lpDesktop = NULL;
		}
	}
	else
	{
		hCurrentDesktop = GetThreadDesktop(GetCurrentThreadId());
		DWORD len;
		::GetUserObjectInformation(hCurrentDesktop,UOI_NAME,CurrentDesktopName,MAX_PATH,&len);
		startUpInfo.wShowWindow = SW_HIDE;
		startUpInfo.lpDesktop = CurrentDesktopName;		

	}

	// create the process
	CString lstrWorkDir = GetAppPath();
	unsigned int nPauseSec = 1;

	PROCESS_INFORMATION procInfo[c_nMaxSize] = {0};

	if (lbUseCurrentUserDesktop)
	{

		BOOL lbRet =  FALSE;


		lbRet = CreateProcessAsUser(lhToken,NULL,szCommandLine.GetBuffer(),NULL,NULL,FALSE,ldwCreateFlag,NULL,lstrWorkDir,&startUpInfo,&procInfo[0]);

		DWORD ldwError = ::GetLastError();

		if (NULL != lhToken)
		{
			try
			{
				CloseHandle(lhToken);

			}catch(...)
			{

			}
		}

		if(NULL != lpEnv)
		{
			DestroyEnvironmentBlock(lpEnv);
		}

		if (!lbRet)
		{
			long nError = GetLastError();

			lbRet = CreateProcess(NULL,szCommandLine.GetBuffer(),NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,lstrWorkDir,&startUpInfo,&procInfo[0]);

			if (!lbRet)
			{
				long nError = GetLastError();
			}

			CloseHandle(procInfo[0].hProcess);
			CloseHandle(procInfo[0].hThread);
			return FALSE;
		}

	}
	else if(!CreateProcess(NULL,szCommandLine.GetBuffer(),NULL,NULL,TRUE,NORMAL_PRIORITY_CLASS,NULL,lstrWorkDir,&startUpInfo,&procInfo[0]))
	{
		long nError = GetLastError();
		CloseHandle(procInfo[0].hProcess);
		CloseHandle(procInfo[0].hThread);
		return FALSE;
	}

	try
	{

		CloseHandle(procInfo[0].hProcess);
		CloseHandle(procInfo[0].hThread);
	}
	catch (...)
	{

	}

	if (NULL != hCurrentDesktop)
	{
		CloseDesktop(hCurrentDesktop);
	}

	return TRUE;
	
	END_ERROR_HANDLE;

	return TRUE;
}

BOOL CRemoteSesrver::StartDebugger()
{
	BEGIN_ERROR_HANDLE;

	if (!G_B_DebuggerStarted)
	{
		BOOL lbStarted = InternalStartDebugger();
		if(lbStarted)
		{
			InterlockedExchange((volatile LONG *)&G_B_DebuggerStarted,(LONG)lbStarted);
		}
		
	}
	
	END_ERROR_HANDLE;

	return FALSE;

}
