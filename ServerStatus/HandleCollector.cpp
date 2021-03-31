#include "StdAfx.h"
#include ".\handlecollector.h"
#include <vector>
#include <map>
#include "ntdll.h"
#include <afxmt.h>
#include <afxsock.h>
#include <Iphlpapi.h>
#include <tlhelp32.h>
#include <KDCommlib.h>
#include <algorithm>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "ntdll.lib")
#pragma warning(disable:4786) // identifier was truncated in the debug info
CHandleCollector::CHandleCollector(void)
{

}

CHandleCollector::~CHandleCollector(void)
{
}

HANDLE gHEvent = NULL;

HANDLE ghThread = NULL;

DWORD gMilliSeconds = 10000;

DWORD gLinksPerPeerLimit = 3;

LONG gEnableTrack = FALSE;

typedef NTSTATUS (WINAPI*QuerySysInfo)(ULONG, PVOID, ULONG, PULONG);

const DWORD MS_VC_EXCEPTION=0x406D1388;
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName( DWORD dwThreadID, char* threadName)
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}
HMODULE g_Module_NT = NULL;
HMODULE g_Module_IP = NULL;
void GetHandlesMap(DWORD adwProcessId,std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> & arefMap)
{
	
	ULONG n =32000;
	std::vector<BYTE> loBuffer(n);
	BYTE * p = &(loBuffer.front());
	DWORD size = 0;
	HMODULE l_Module_NT = (HMODULE)InterlockedExchangeAdd((volatile LONG*)&g_Module_NT,0);
	HMODULE l_Module_NT_Current = GetModuleHandle(_T("ntdll.dll"));

	if ((l_Module_NT!=l_Module_NT_Current))
	{
		if (l_Module_NT!=NULL)
		{
			FreeLibrary(l_Module_NT);
		}	
		InterlockedExchange((volatile LONG*)&g_Module_NT,(LONG)l_Module_NT_Current);
	}
	
	QuerySysInfo qsi = reinterpret_cast<QuerySysInfo>(GetProcAddress(l_Module_NT_Current, "NtQuerySystemInformation"));
	while (qsi(SystemExtendedHandleInformation,
		p,  loBuffer.size(),&size) == (STATUS_INFO_LENGTH_MISMATCH))
	{
		if(size)
		{
			loBuffer.resize(size);
		}
		else
		{
			loBuffer.resize(loBuffer.size() * 2);
		}
		p = &(loBuffer.front());
	}

	NT::SYSTEM_HANDLE_INFORMATION_EX* handleInf = reinterpret_cast<NT::SYSTEM_HANDLE_INFORMATION_EX*>(&loBuffer[0]);
	std::vector<NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> loData;
	for (int i=0;i<handleInf->size;i++)
	{
		loData.push_back(handleInf->handles[i]);
	}

	for (size_t i =0;i<loData.size();i++)
	{
		NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX * lpData = &loData[i];
		if (lpData->ProcessId!=adwProcessId)
		{
			continue;
		}
		arefMap[(HANDLE)(loData[i].Handle)] = *lpData;
	}
	return ;
}


typedef std::vector<MIB_TCPROW> TypeVoLinkTable;
typedef std::map<DWORD,TypeVoLinkTable> TypeLinkInfo;


BOOL GetAllLinks(DWORD adwProcessId,std::map<DWORD ,TypeLinkInfo> & arefMapTcpLinks)
{	

	HMODULE l_Module_IP = (HMODULE)InterlockedExchangeAdd((volatile LONG*)&g_Module_IP,0);	
	HMODULE hModule = l_Module_IP;

	if (hModule == NULL)
	{
		hModule = ::LoadLibrary("IPHLPAPI.DLL");
		InterlockedExchange((volatile LONG*)&g_Module_IP,(LONG)hModule);
	}

	if (hModule == NULL)
	{
		OutputDebugString(" iphlpapi.dll are not present \n ");
		ASSERT(FALSE);
		return FALSE;
	}

	PMIB_TCPTABLE pTcpTable = NULL;

	DWORD ldwSize = 0;	
	DWORD ldwRetVal = 0;
	std::vector<CHAR> loBuffer;
	while ((ldwRetVal = GetTcpTable(pTcpTable, &ldwSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER)
	{
		loBuffer.resize(ldwSize);
		pTcpTable = (PMIB_TCPTABLE)(&loBuffer.front());
		if (pTcpTable == NULL) 
		{
			OutputDebugString("Error allocating memory\n");
			ASSERT(FALSE);
			return 0;
		}
	}


	for(UINT i = 0; i < pTcpTable->dwNumEntries; ++i)
	{
	  TypeLinkInfo * lpTypeInfo = &	arefMapTcpLinks[pTcpTable->table[i].dwLocalPort];
	  TypeVoLinkTable * lpVoTable =&((*lpTypeInfo)[pTcpTable->table[i].dwRemoteAddr]);
	  lpVoTable->push_back(pTcpTable->table[i]);
	}

	return TRUE;
}

typedef struct __HANDLEDATA
{
	HANDLE m_hHandle;
	NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX m_oSystemHandleInfo;
	MIB_TCPROW m_oTcpLinkInfo;
	NT::OBJECT_BASIC_INFORMATION m_pFileInfo[2];
	CString m_strLinkStatus;
	__HANDLEDATA()
	{
		m_hHandle = NULL;
	}
	~__HANDLEDATA()
	{
		if (m_hHandle)
		{
			/*CloseHandle(m_hHandle);*/
		}
	}
} HANDLEDATA,*PHANDLEDATA;

CString inet_toStr(DWORD dwIPNet)
{
	USES_CONVERSION;
	sockaddr_in   local;
	local.sin_addr.s_addr   =   dwIPNet;
	
	CHAR * lpData =inet_ntoa(local.sin_addr);
	if (lpData)
	{
		CString lstrTmp = A2T(lpData);
		return lstrTmp;
	}
	
	return _T("");
}
typedef struct __SOCK_CALL_PARAMETER
{
	SOCKET s;
	OUT struct sockaddr FAR *name;
	IN OUT int FAR * namelen;
	int m_nRet;

} SOCK_CALL_PARAMETE,*lpSOCK_CALL_PARAMETER;

VOID CALLBACK APCProc(DWORD dwParam)
{

	CString lstrData;
	lstrData.Format(_T("%s"),(TCHAR *)dwParam);
	//BHLOG_DEBUG(STR(_T("%s %d"),lstrData,GetCurrentThreadId()));
}

DWORD WINAPI getsocknameThread(LPVOID apData)
{
	if (NULL == apData)
	{
		return FALSE;
	}
	SOCK_CALL_PARAMETE * lpParameter = (SOCK_CALL_PARAMETE *)apData;
	sockaddr_in loName = {0};
	int lnNamelength = sizeof(sockaddr_in);
	lpParameter->m_nRet = getsockname( lpParameter->s, (sockaddr *)&loName,&lnNamelength );  
	return lpParameter->m_nRet;
}

//LPTHREAD_START_ROUTINE 

UINT IsSocketBlock( IN SOCKET s)
{
	SOCK_CALL_PARAMETE loParameter = {0};
	loParameter.s = s;
	DWORD ldwThreadID =0;
	HANDLE lhThreadHandle = CreateThread(NULL,0,getsocknameThread,&loParameter,CREATE_SUSPENDED,&ldwThreadID);
	
	SetThreadName(ldwThreadID,_T("getsocknameThread"));
	ResumeThread(lhThreadHandle);

	DWORD ldwRet = WaitForSingleObject(lhThreadHandle,1000);
	if (ldwRet ==WAIT_TIMEOUT)
	{
		for (int i=0;i<5;i++)
		{
			BOOL lbRet = QueueUserAPC(APCProc, lhThreadHandle, (DWORD)(PVOID)"stop call APC here/n");
			DWORD ldwError = ::GetLastError();
			Sleep(1000);
		}
		
		ldwRet = WaitForSingleObject(lhThreadHandle,100);
		if (ldwRet ==WAIT_TIMEOUT)
		{
			TerminateThread(lhThreadHandle,0);
			//ASSERT(FALSE);
		}

		CloseHandle(lhThreadHandle);

		return TRUE;
	}
	CloseHandle(lhThreadHandle);
	return FALSE;
}

BOOL FixForCompileError(HANDLE lhHandleSystem,
						std::map<DWORD ,TypeLinkInfo> &loMapTcpLinks,
						std::map<HANDLE,HANDLEDATA> & arefoHandles,
						std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX>::iterator lpItHandleMap)
{
	
	BEGIN_ERROR_HANDLE;
	
	{
		//1.
		SOCKET loSocket = (SOCKET)lhHandleSystem; 
		DWORD ldwRt = IsSocketBlock(loSocket);
		if (ldwRt)
		{
			return FALSE;
		}
		
		//2.get Socket Option
		sockaddr_in loName = {0};  
		sockaddr_in loPeer = {0};
		loName.sin_family = AF_INET; 
		loPeer.sin_family = AF_INET; 
		int lnNamelength = sizeof(sockaddr_in);
		 
		char lpszSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM","SEQ" };  

		int error  = 0;
		socklen_t len = sizeof (int);
		int lnRet = getsockopt(loSocket,SOL_SOCKET, SO_ERROR, (char*)&error , &len );
		lnRet = getsockname( loSocket, (sockaddr*)&loName, &lnNamelength );  
		lnRet = getpeername(loSocket, (sockaddr*)&loPeer, &lnNamelength);
		if ( lnRet != SOCKET_ERROR )
		{ 
			int lnSockType = 0;
			int lnOptlen = sizeof(int); 
			lnRet = getsockopt(loSocket,SOL_SOCKET, SO_TYPE, (char*)&lnSockType, &lnOptlen );
			std::map<DWORD ,TypeLinkInfo>::iterator lpitTcpMapInfo =loMapTcpLinks.find(loName.sin_port);
			if (lpitTcpMapInfo != loMapTcpLinks.end())
			{
				PHANDLEDATA lpData = &arefoHandles[lhHandleSystem];				
				lpData->m_hHandle = lhHandleSystem;
				lpData->m_oSystemHandleInfo = lpItHandleMap->second;


				for (TypeLinkInfo::iterator lpItLinkInfo = lpitTcpMapInfo->second.begin(); 
					lpItLinkInfo!= lpitTcpMapInfo->second.end();
					lpItLinkInfo++)
				{

					TypeVoLinkTable * lpDataTable = &(lpItLinkInfo->second);

					for (int i =0;i<lpDataTable->size();i++)
					{
						MIB_TCPROW loTable = lpDataTable->at(i);
						DWORD ldwRemoteAddr = (DWORD)(loPeer.sin_addr.s_addr);
						if (loTable.dwRemotePort!=loPeer.sin_port 
							|| ldwRemoteAddr!= loTable.dwRemoteAddr)
						{
							continue;
						}
						lpData->m_oTcpLinkInfo =(lpDataTable->at(i));
						switch (lpDataTable->at(i).dwState)
						{
						case MIB_TCP_STATE_CLOSED:
							lpData->m_strLinkStatus = "CLOSED";
							break;
						case MIB_TCP_STATE_TIME_WAIT:
							lpData->m_strLinkStatus = "TIME_WAIT";
							break;
						case MIB_TCP_STATE_LAST_ACK:
							lpData->m_strLinkStatus = "LAST_ACK";
							break;
						case MIB_TCP_STATE_CLOSING:
							lpData->m_strLinkStatus = "CLOSING";
							break;
						case MIB_TCP_STATE_CLOSE_WAIT:
							lpData->m_strLinkStatus = "CLOSE_WAIT";
							break;
						case MIB_TCP_STATE_FIN_WAIT1:
							lpData->m_strLinkStatus = "FIN_WAIT1";
							break;
						case MIB_TCP_STATE_ESTAB:
							lpData->m_strLinkStatus = "ESTABLISHED";
							break;
						case MIB_TCP_STATE_SYN_RCVD:
							lpData->m_strLinkStatus = "SYN_RCVD";
							break;
						case MIB_TCP_STATE_SYN_SENT:
							lpData->m_strLinkStatus = "SYN_SENT";
							break;
						case MIB_TCP_STATE_LISTEN:
							lpData->m_strLinkStatus = "LISTEN";
							break;
						case MIB_TCP_STATE_DELETE_TCB:
							lpData->m_strLinkStatus = "DELETE";
						default:
							lpData->m_strLinkStatus = ("Error: unknown state!");
							break;
						}
						ULONG lnBufferNeed = 0;
						ZeroMemory(&lpData->m_pFileInfo,sizeof(NT::OBJECT_BASIC_INFORMATION));
						break;

					}
				}

			}

		}
	}//if (lbIsSrcValid)
	
	END_ERROR_HANDLE;
	return TRUE;
}



BOOL GetAllSocketsHandle(HANDLE ahProcessHandle,std::map<HANDLE,HANDLEDATA> & arefoHandles)
{
	UINT lnHandleCount = 0;
	ULONG lnDataRequired = 0;
	BOOL lbRet = FALSE;
	AfxSocketInit();
	//1.use to detect socket handle type
	CSocket loScoket;
	lbRet = loScoket.Create(0,SOCK_STREAM,NULL);	
	if (!lbRet)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	DWORD ldwProcessID = ::GetProcessId(ahProcessHandle);
	std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX>  loHandles ;	
	GetHandlesMap(ldwProcessID,loHandles);

	UCHAR  lnSocketHandleType = 0;
	UCHAR  lnThreadHandleType = 0;
	BOOL lbSocketTypeFound = FALSE;
	BOOL lbThreadHandleTypeFound = FALSE;
	HANDLE lhCurrentThread = ghThread;
	std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX>::iterator lpItHandleMap = loHandles.begin();
	for (lpItHandleMap = loHandles.begin();
		lpItHandleMap != loHandles.end();
		lpItHandleMap++)
	{
		if (lpItHandleMap->first ==(HANDLE)loScoket.m_hSocket)
		{
			lnSocketHandleType = lpItHandleMap->second.ObjectTypeNumber;
			lbSocketTypeFound = TRUE;
		
		}else if (lpItHandleMap->first == (lhCurrentThread))//type thread
		{
			lnThreadHandleType = lpItHandleMap->second.ObjectTypeNumber; 
			lbThreadHandleTypeFound = FALSE;
		}

		if (lbSocketTypeFound && lbThreadHandleTypeFound)
		{
			break;
		}
	}

	if (lnSocketHandleType == 0 )
	{
		//no tcp socket handle found
		//ASSERT(FALSE);
		return FALSE;
	}

	//2.get All Active link Satatus
	std::map<DWORD ,TypeLinkInfo>  loMapTcpLinks;
	if (!GetAllLinks(ldwProcessID,loMapTcpLinks))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//3.get socket info
	for (lpItHandleMap = loHandles.begin();
		lpItHandleMap != loHandles.end();
		lpItHandleMap++)

	{
		HANDLE lhHandle = NULL;
		BEGIN_ERROR_HANDLE;

		int lnObjectType =lpItHandleMap->second.ObjectTypeNumber;
		HANDLE lhHandleSystem = lpItHandleMap->first;

		if (lhHandleSystem == lhCurrentThread)
		{
			continue;
		}

		if ((lnObjectType == lnThreadHandleType) && (lnThreadHandleType!=0))
		{
			DWORD ldwRet = WaitForSingleObject(lhHandleSystem,1);
			if (ldwRet != WAIT_TIMEOUT)
			{
				CloseHandle(lhHandleSystem);
				continue;
			}
		}

		if (lnSocketHandleType!= lnObjectType)
		{
			continue;
		}
		
		if (ldwProcessID != lpItHandleMap->second.ProcessId 
			)
		{
			continue;
		}


		
		
		BOOL lbIsSrcValid =DuplicateHandle(ahProcessHandle,
			lhHandleSystem,
			ahProcessHandle,
			&lhHandle,
			0,
			FALSE,DUPLICATE_SAME_ACCESS);

		if (lbIsSrcValid)
		{
			FixForCompileError(lhHandleSystem,loMapTcpLinks,arefoHandles,lpItHandleMap);
		}


	
		END_ERROR_HANDLE;

		if (lhHandle)
		{
			try
			{
				CloseHandle(lhHandle);
			}
			catch (...)
			{
				//log here
				ASSERT(FALSE);

			}
		}

		
	}

	return lbRet;
}



typedef struct _LINK_HANDLE_TIME_STATE 
{
	CString m_strKey;
	HANDLEDATA m_hHandle;
	COleDateTime m_strDate;
	
	_LINK_HANDLE_TIME_STATE()
	{
		m_strDate= COleDateTime(1980,1,1,1,1,1);
	}
} LINK_HANDLE_TIME_STATE ,* PLINK_HANDLE_TIME_STATE;
typedef std::vector<LINK_HANDLE_TIME_STATE> TYPE_VO_LINKSTATUS;
typedef std::map<HANDLE,LINK_HANDLE_TIME_STATE> TYPE_MAP_LINKSTATUS;
typedef std::map<CString,TYPE_MAP_LINKSTATUS> TYPE_MAP_CSTRING_LINKSTATUS;
bool TimeSort (LINK_HANDLE_TIME_STATE & i,LINK_HANDLE_TIME_STATE & j) 
{
	return (i.m_strDate<j.m_strDate); 
}

UINT CheckDeadLink(LPVOID apData)
{
	TYPE_MAP_CSTRING_LINKSTATUS loLinks;	

	int lnMonitorPort = (int)apData;


	std::map<HANDLE,HANDLEDATA> loHandles;
	GetAllSocketsHandle(::GetCurrentProcess(),loHandles);
	for (std::map<HANDLE,HANDLEDATA>::iterator loIt = loHandles.begin();
		loIt != loHandles.end();
		loIt++)
	{

		CString logStr;
		FILETIME lTime;

		PHANDLEDATA lpData = &loIt->second;

		lTime.dwHighDateTime= lpData->m_pFileInfo->CreateTime.LowPart;
		lTime.dwLowDateTime = lpData->m_pFileInfo->CreateTime.HighPart;

		COleDateTime loTime(lTime);
		CString lstrkey;
		lstrkey.Format(_T("%s:%d||%s"),
			inet_toStr(lpData->m_oTcpLinkInfo.dwLocalAddr),
			ntohs( lpData->m_oTcpLinkInfo.dwLocalPort),
			inet_toStr(lpData->m_oTcpLinkInfo.dwRemoteAddr));

		TYPE_MAP_LINKSTATUS * lpMapLinkStatus = &loLinks[lstrkey];
		TYPE_MAP_LINKSTATUS::iterator lpItHandleState = lpMapLinkStatus->find(lpData->m_hHandle);
		PLINK_HANDLE_TIME_STATE lpHandleState = NULL;
		lpHandleState = &(*lpMapLinkStatus)[lpData->m_hHandle];
		//添加
		if (lpItHandleState == lpMapLinkStatus->end())
		{				
			lpHandleState->m_hHandle = *lpData;
			lpHandleState->m_strDate = COleDateTime::GetCurrentTime();

		}else if(lpData->m_oTcpLinkInfo.dwState == MIB_TCP_STATE_CLOSE_WAIT)
		{
			lpHandleState->m_strDate = COleDateTime(1980,1,1,1,1,1);
		}

		COleDateTimeSpan loSpan = (COleDateTime::GetCurrentTime()-lpHandleState->m_strDate);

		logStr.Format(" HANDLE:0x%4x PID=%4d LOCAL=%s:%5d  Remote=%s:%5d State:%s TimeElapsed:%s \r\n",
			lpData->m_hHandle,
			lpData->m_oSystemHandleInfo.ProcessId,
			inet_toStr(lpData->m_oTcpLinkInfo.dwLocalAddr),
			ntohs( lpData->m_oTcpLinkInfo.dwLocalPort ), 
			inet_toStr(lpData->m_oTcpLinkInfo.dwRemoteAddr),
			ntohs(lpData->m_oTcpLinkInfo.dwRemotePort),
			lpData->m_strLinkStatus,
			loSpan.Format(_T("%H:%M:%S")));	

		BHLOG_DEBUG(logStr);

	}

	return TRUE;
}


CCriticalSection * g_pCollector = new CCriticalSection();

void  CHandleCollector::StartTrack()
{
	g_pCollector->Lock();

	if (gHEvent==NULL && gEnableTrack)
	{
		HINSTANCE lhInst = afxCurrentInstanceHandle;
		if (lhInst == NULL)
		{
			AfxWinInit(GetModuleHandle(NULL),NULL,GetCommandLine(),0);
			lhInst =  AfxGetInstanceHandle();
		}

		if (!lhInst)
		{
			ASSERT(FALSE);
			return;
		}
		char cAppLocation[2048] = {0};
		GetCurrentDirectory(4999,cAppLocation);
		strcat(cAppLocation,"\\DseSvr.ini");

		int lnPort = GetPrivateProfileInt(_T("SYSTEM"),_T("TCPPORT"),7001,cAppLocation);
		gHEvent = ::CreateEvent(NULL,FALSE,FALSE,_T("DeadLinkCollector"));	
		CWinThread * lpThread = AfxBeginThread(CheckDeadLink,(LPVOID)lnPort,0,CREATE_SUSPENDED);
		lpThread->m_bAutoDelete = TRUE;
		ghThread = lpThread->m_hThread;
		
		lpThread->ResumeThread();
		SetThreadName(lpThread->m_nThreadID,_T("DeadLinkCollector"));

	}

	g_pCollector->Unlock();

}

void CHandleCollector::SetCheckInterval(DWORD adwMilliSeconds)
{
	gMilliSeconds = adwMilliSeconds;
}

void CHandleCollector::SetConnLimitPerPeer(DWORD adwConnLimitPerPeer)
{
	gLinksPerPeerLimit = adwConnLimitPerPeer;
}


void CHandleCollector::StopTrack()
{
	if (gHEvent)
	{
		::SetEvent(gHEvent);

		DWORD ldwRet = WaitForSingleObject(ghThread,2000);

		if (ldwRet!= WAIT_OBJECT_0)
		{
			try
			{
				::TerminateThread(ghThread,-1);
			}
			catch (...)
			{
				ASSERT(FALSE);
			}

		}

	}
}

/*
读取配置文件, 设置句柄柄检查间隔和连接限制
	CHandleCollector::SetCheckInterval(60*1000);
	CHandleCollector::SetConnLimitPerPeer(50);
*/
namespace OSPath
{
#define PATH_SEP '\\'
	CString GetBaseName(CString asFilePath)
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

	CString GetDirName(CString asFilePath)
	{
		int lnPathSep = asFilePath.ReverseFind(PATH_SEP);
		if (lnPathSep < 0)
		{
			TCHAR cBuff[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, cBuff);
			return cBuff;
		}

		return asFilePath.Left(lnPathSep);
	}
}

bool HandleTypeSort (NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX & i,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX & j)
{
	if (i.ObjectTypeNumber!=j.ObjectTypeNumber)
	{
		return i.ObjectTypeNumber>j.ObjectTypeNumber;
	}

	return i.Handle > j.Handle;
}

void GetHandlesVo(std::vector<NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX> & arefVo )
{
	DWORD ldwProcessID = ::GetProcessId(::GetCurrentProcess());
	std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX>  loHandles ;	
	GetHandlesMap(ldwProcessID,loHandles);
	std::map<HANDLE,NT::SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX>::iterator lpIt = loHandles.begin();
	for (lpIt = loHandles.begin();lpIt!=loHandles.end();lpIt++)
	{
		arefVo.push_back(lpIt->second);
	}
	sort(arefVo.begin(),arefVo.end(),HandleTypeSort);
}

typedef NTSTATUS (NTAPI *NtQueryObjectPtr)(HANDLE Handle,
										   OBJECT_INFORMATION_CLASS ObjectInformationClass,
										   PVOID ObjectInformation,
										   ULONG ObjectInformationLength,
										   PULONG ReturnLength);

CString GetHandleTypeName( HANDLE hHandle)
{
	CString TypeName = _T("UNKOWN");;
	HMODULE hMod = LoadLibrary(_T("NtDll.dll"));
	NtQueryObjectPtr QueryObj =  reinterpret_cast<NtQueryObjectPtr>(::GetProcAddress(hMod, "NtQueryObject"));
	
	ASSERT(QueryObj);
	if (NULL==QueryObj)
	{
		return TypeName;
	}
	ULONG OutSize = 0;
	NTSTATUS NtStatus = QueryObj(hHandle, ObjectTypeInformation, NULL, 0,&OutSize);
	NT::PPUBLIC_OBJECT_TYPE_INFORMATION TypeInfo = (NT::PPUBLIC_OBJECT_TYPE_INFORMATION)calloc(1, OutSize);
	ULONG InSize = OutSize;
	NtStatus = QueryObj(hHandle, ObjectTypeInformation, TypeInfo, InSize, &OutSize);
	if (STATUS_SUCCESS== NtStatus && TypeInfo!=NULL)
	{
		TypeName = TypeInfo->TypeName.Buffer;

	}
	free(TypeInfo);
	return TypeName;
}



BOOL gb_Inited = FALSE;
BOOL g_bEnableDumpMonitor = FALSE;
void CHandleCollector::Init()
{
	if (!gb_Inited)
	{
		CString lsModuleFileName;

		int lnHandleCheckInterval = 60;
		int lnHandleLimitPerPeer  = 50;
		int lnEnableTrack = FALSE;
		int lnEnableDumpMonitor = FALSE;
		CString lstrData;

		::GetModuleFileName(
			NULL, 
			lsModuleFileName.GetBufferSetLength(MAX_PATH * 2), 
			MAX_PATH);

		lsModuleFileName.ReleaseBuffer(MAX_PATH * 2);

		CString lsModuleFileDirName =  
			OSPath::GetDirName(lsModuleFileName);

		CString lsDseSvrIniName 
			= lsModuleFileDirName + _T("\\") +_T("DseSvr.ini");

		lnHandleCheckInterval = ::GetPrivateProfileInt("SYSTEM", "HANDLECHECKINTERVAL", 60, lsDseSvrIniName);
		lnHandleLimitPerPeer  = ::GetPrivateProfileInt("SYSTEM", "HANDLELIMITPERPEER",  25, lsDseSvrIniName);
		if (lnHandleLimitPerPeer < 1)
		{
			lnHandleLimitPerPeer = 1;
		}

		lstrData.Format("%d",lnHandleLimitPerPeer);
		WritePrivateProfileString("SYSTEM", "HANDLELIMITPERPEER",lstrData,lsDseSvrIniName);
		
		lstrData.Format("%d",lnHandleCheckInterval);
		WritePrivateProfileString("SYSTEM", "HANDLECHECKINTERVAL",lstrData,lsDseSvrIniName);

		lnEnableTrack = ::GetPrivateProfileInt("SYSTEM", "ENABLE_HANDLE_COLLECTOR_TRACK",  0, lsDseSvrIniName);

		lstrData.Format("%d",lnEnableTrack);
		WritePrivateProfileString("SYSTEM", "ENABLE_HANDLE_COLLECTOR_TRACK",lstrData,lsDseSvrIniName);

		lnEnableDumpMonitor = ::GetPrivateProfileInt("SYSTEM", "ENABLE_DUMP_MONITOR",  0, lsDseSvrIniName);
		lstrData.Format("%d",lnEnableDumpMonitor);
		WritePrivateProfileString("SYSTEM", "ENABLE_DUMP_MONITOR",lstrData,lsDseSvrIniName);

		g_bEnableDumpMonitor = lnEnableDumpMonitor;
		
		if (lnHandleCheckInterval > 0)
		{
			CHandleCollector::SetCheckInterval(lnHandleCheckInterval*1000);
		}
		else
		{
			CHandleCollector::SetCheckInterval(60*1000);
		}

		if (lnHandleLimitPerPeer > 0)
		{
			CHandleCollector::SetConnLimitPerPeer(lnHandleLimitPerPeer);
		}
		else
		{
			CHandleCollector::SetConnLimitPerPeer(50);
		}

		CHandleCollector::SetEnableTrack(lnEnableTrack);

		gb_Inited = TRUE;
	}

}

void CHandleCollector::SetEnableTrack(BOOL abEnableTrack)
{
	InterlockedExchange(&gEnableTrack,abEnableTrack); 
}
