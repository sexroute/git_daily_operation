// ICEServer.cpp : 定义 DLL 的初始化例程。
//


#include "stdafx.h"
#include "ICEServer.h"
#include ".\iceserver.h"
#include "kdcommlib.h"
#include <map>
#include <vector>
#include <afxmt.h>
#include <ice/Properties.h>
#include <ice/Connection.h>
#include <ice/Communicator.h>
#include <ice/ObjectAdapter.h>
#include "LogI.h"
#include "BHACOI.h"
#include <additions/BHCommWrapper.h>
#include <MonitorICEServer.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CICEServerApp

BEGIN_MESSAGE_MAP(CICEServerApp, CWinApp)
END_MESSAGE_MAP()

typedef struct _SERVICE_MODULE
{
	int m_hInstance;
	LY::CALLBACK_PROC m_pCallBack;

} SERVICE_MODULE, *P_SERVICE_MODULE;


LY::IServer * CICEServerApp::m_pServer			= NULL;
CCriticalSection * g_pLock						= NULL;
std::vector<SERVICE_MODULE> * g_pSerivceModules = NULL;
std::map<int,size_t> * g_pSerivceIndex		= NULL;
CONST UINT OP_TIME_OUT = 10000;		
//TIME OUT FOR OPERATION , 10s
char cAppLocation[5000]={0};




BOOL CreateServer( LY::IServer ** app )
{
	if (NULL == app)
	{
		ASSERT(FALSE);

		return FALSE;
	}
	
	*app = new ICEServer();

	if (NULL == *app)
	{
		return FALSE;
	}

	return TRUE;

}

BOOL DeleteServer( LY::IServer ** app )
{
	if (NULL == app)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (NULL == *app)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	delete *app;
	*app = NULL;
	return TRUE;
}


// CICEServerApp 构造

CICEServerApp::CICEServerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

int RetainLibrayInMemory(
							  HMODULE ahDllHandle,
							  HMODULE *apLocalDllHandle)
{
	if(NULL == apLocalDllHandle)
		return ERROR_INVALID_PARAMETER;

	*apLocalDllHandle = NULL;
	TCHAR moduleName[1024];
	if(0 == GetModuleFileName(
	   	                      ahDllHandle,
		                      moduleName,
		                      sizeof(moduleName)/ sizeof(TCHAR)))
	{
		return GetLastError();
	}

	*apLocalDllHandle = LoadLibrary(moduleName);
	
	if(NULL == *apLocalDllHandle)
	{
		return GetLastError();
	}
	return NO_ERROR;
}

HMODULE GetCurrentModuleHandle()
{
	HMODULE hMod = NULL;
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		               reinterpret_cast<LPCWSTR>(&GetCurrentModuleHandle),
		               &hMod);
	return hMod;
}

// 唯一的一个 CICEServerApp 对象

CICEServerApp theApp;

// CICEServerApp 初始化


BOOL CICEServerApp::InitInstance()
{
	CWinApp::InitInstance();
	HMODULE hMod = NULL;
	RetainLibrayInMemory(::GetCurrentModuleHandle(),&hMod);
	return TRUE;
}

LY::IServer * CICEServerApp::GetServer()
{
	if (NULL == g_pLock)
	{
		g_pLock = new CCriticalSection();
		g_pSerivceModules = new std::vector<SERVICE_MODULE>();
		g_pSerivceIndex = new std::map<int,size_t>();
		GetCurrentDirectory(4999,cAppLocation);
		strcat(cAppLocation,"\\DseSvr.ini");
	}
	if (NULL ==  CICEServerApp::m_pServer)
	{
		BOOL lbRet = CreateServer(&CICEServerApp::m_pServer);
        
		if (!lbRet)
		{
			ASSERT(FALSE);
		}
	}

	return CICEServerApp::m_pServer;
}



ICEServer::ICEServer(void)
{
	this->Inited(FALSE);
	this->Started(FALSE);
}

ICEServer::~ICEServer(void)
{
	if (this->Started())
	{
		this->Stop();
	}
}
static HWND FindWindowByProcessID(DWORD dwPID_)
{
	HWND hWnd = GetTopWindow(0);

	while ( hWnd )
	{

		DWORD dwPid = 0;

		DWORD dwTheardId = GetWindowThreadProcessId( hWnd,&dwPid);

		if (dwTheardId != 0)
		{

			if ( dwPid == dwPID_)
			{
				CWnd   *pWnd   =   CWnd::FromHandle(hWnd);
				if(pWnd)
				{
					CString strText;
					pWnd->GetWindowText(strText);
					if(strText == "中间件服务监控")
					{
						return hWnd;
					}
				}
				
			}
		}
		hWnd = GetNextWindow( hWnd , GW_HWNDNEXT);

	}
	return NULL;
}

void ICEServer::CreateDebugInfoWindow(CString lstrInfo,BOOL abInitSucceed)
{
	DWORD dwPID = ::GetCurrentProcessId();
	HWND hWnd = FindWindowByProcessID(dwPID);
	if(hWnd == NULL)
	{
		CMonitorICEServer::GetInstance()->InitialDialog();
		CMonitorICEServer::GetInstance()->SetServerInfo(lstrInfo,abInitSucceed);
	}
}
int ICEServer::Init()
{
	if (this->Inited())
	{
// 		ASSERT(FALSE);

		return TRUE;
	}

	__logI = new LogI();

	this->__propData.properties = Ice::createProperties();
	
	//ice port,default is 17001
	int lnICEPort   = ::GetPrivateProfileInt("SYSTEM","ICE_PORT",17001,cAppLocation);
	CString lstrData;
	lstrData.Format(_T("%d"),lnICEPort);
	::WritePrivateProfileString("SYSTEM","ICE_PORT",lstrData.GetBuffer(0),cAppLocation);

	//ice thread count, default is 4
	int lnProcessor = ::GetPrivateProfileInt("SYSTEM","ICE_PROCESSOR",4,cAppLocation);
	lstrData.Format(_T("%d"),lnProcessor);
	::WritePrivateProfileString("SYSTEM","ICE_PROCESSOR",lstrData.GetBuffer(0),cAppLocation);	

	//ice packet size, unit is k,default is 102400K = 100MB
	int lnPacketSize = ::GetPrivateProfileInt("SYSTEM","ICE_PACKET_SIZE",102400,cAppLocation);
	lstrData.Format(_T("%d"),lnPacketSize);
	::WritePrivateProfileString("SYSTEM","ICE_PACKET_SIZE",lstrData.GetBuffer(0),cAppLocation);	

	char cinfo[100]={0};
	sprintf(cinfo,"tcp -p  %d", lnICEPort);

	__propData.properties->setProperty("Request.Endpoints",cinfo); 
	
	ZeroMemory(cinfo,100*sizeof(char));
	sprintf(cinfo,"%d", lnProcessor+3);
	__propData.properties->setProperty("Ice.ThreadPool.Server.SizeMax",cinfo);
	
	ZeroMemory(cinfo,100*sizeof(char));
	sprintf(cinfo,"%d", lnProcessor+2);
	__propData.properties->setProperty("Ice.ThreadPool.Server.SizeWarn",cinfo);

	ZeroMemory(cinfo,100*sizeof(char));		
	sprintf(cinfo,"%d", lnProcessor);
	__propData.properties->setProperty("Ice.ThreadPool.Server.Size",cinfo);

	ZeroMemory(cinfo,100*sizeof(char));
	sprintf(cinfo,"%d", lnProcessor);
	__propData.properties->setProperty("Request.ThreadPool.Size",cinfo);

	ZeroMemory(cinfo,100*sizeof(char));
	sprintf(cinfo,"%d", lnPacketSize);
	__propData.properties->setProperty("Ice.MessageSizeMax",cinfo);

	
	CString lstrStartInfo;
	try
	{
		int argc = 0;
		__propData.logger = __logI;
		this->__communicator = Ice::initialize(argc, 0, __propData);
		__adapter = __communicator->createObjectAdapter("Request");
	}
	catch(const IceUtil::Exception& ex)
	{
		ostringstream ostr;
		ostr << ex;
		string s = ostr.str();
		__logI->error(s);
		ostringstream lstrError;
		lstrError<<ex;
		string sTemp = lstrError.str();
		CString lcstrError = sTemp.c_str();

		if (lcstrError.Find("INUSE")>=0)
		{
			lcstrError.Format("端口%d被占用",lnICEPort);
		}
		
		lstrStartInfo.Format(_T("高速通信信道初始化失败 : %s"),lcstrError);
		this->CreateDebugInfoWindow(lstrStartInfo,0);
		return 0;
	}

	Ice::ObjectPtr servant = new CRequestI(this);
	__adapter->add(servant, __communicator->stringToIdentity("request"));

	__adapter->activate();

	BHLOG_INFO(_T("ICE Server started"));

	char Name[255]={0};
	char *IP = NULL;
	CString strIP="";
	PHOSTENT hostinfo;
	if(gethostname (Name, sizeof(Name)) == 0)
	{ 
		
			if((hostinfo = gethostbyname(Name)) != NULL) 
			{ 
				
					IP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
				lstrStartInfo.Format("%s:%d",IP,lnICEPort);
			}
	}
    
	this->Inited(TRUE);	
	
	this->CreateDebugInfoWindow(lstrStartInfo,1);
	
	return TRUE;
}

int ICEServer::Start()
{
	if (!this->Inited())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (this->Started())
	{
// 		ASSERT(FALSE);
		return TRUE;
	}

	this->Started(TRUE);
	return TRUE;

}

int ICEServer::Stop()
{
	if (!this->Started())
	{
		return FALSE;
	}

	try
	{
		__adapter->deactivate();	
		//__adapter->destroy();
		__communicator->shutdown();
		//__communicator->destroy();
		__communicator = NULL;
		__adapter = NULL;
		exit(0);
	}
	catch(const IceUtil::Exception&)
	{
		return -1;
	}

	this->Started(FALSE);
	this->Inited(FALSE);


	return TRUE;
}

BOOL ICEServer::Inited() const
{
	return m_bInited;
}

void ICEServer::Inited( BOOL val )
{
	m_bInited = val;
}

BOOL ICEServer::Started() const
{
	return m_bStarted;
}

void ICEServer::Started( BOOL val )
{
	m_bStarted = val;
}


int ICEServer::RegServiceCallBackHandler(	int hInstancemain,
												LY::CALLBACK_PROC apfProc )
{
	if (apfProc == NULL || hInstancemain == NULL)
	{
		return FALSE;
	}

	if (g_pLock->Lock())
	{
		BEGIN_ERROR_HANDLE;

		std::map<INT,size_t>::iterator lpItFound = g_pSerivceIndex->find(hInstancemain);
		if (lpItFound == g_pSerivceIndex->end())
		{
			SERVICE_MODULE loModule;
			loModule.m_hInstance = hInstancemain;
			loModule.m_pCallBack = apfProc;

			g_pSerivceModules->push_back(loModule);
			size_t lnIndex = g_pSerivceModules->size()-1;
			g_pSerivceIndex->insert(make_pair(hInstancemain,lnIndex));			
		}else
		{
			g_pSerivceModules->at(lpItFound->second).m_pCallBack = apfProc;	
		}

		END_ERROR_HANDLE;
		g_pLock->Unlock();

		return TRUE;

	}else
	{
		ASSERT(FALSE);
		return 0;
	}	
}

Ice::InitializationData ICEServer::PropData() const
{
	return __propData;
}

void ICEServer::PropData( Ice::InitializationData & val )
{
	__propData = val;
}
#define  SYSTEM_SERVICE_BEGIN 1000


int ICEServer::HandleSrvData( void * apData ,void * response)
{
	if (NULL == apData || NULL == response)
	{
		return 0;
	}

	ACO * lpAco = (ACO *)apData;
	ACO * lpResponse = (ACO *)response;

	if (1000 == lpAco->GetServiceCode())
	{
		//lpAco->SetInt(SYSTEM_SERVICE_BEGIN,1);
		lpResponse->SetRetCode(1);
		return 1;
	}

	std::vector<SERVICE_MODULE> loModules;

	if (g_pLock->Lock())
	{
		BEGIN_ERROR_HANDLE;		
		loModules = *g_pSerivceModules;		
		END_ERROR_HANDLE;

		g_pLock->Unlock();
	}
	
	CBHCommWrapperA loComm;
	loComm.SetFixComm(lpAco);
	loComm.SetFixCommResponse(lpResponse);

	int lnRet = 0;
	for (std::vector<SERVICE_MODULE>::iterator lpIt = loModules.begin();
		 lpIt!= loModules.end();
		 lpIt++)
	{

		try{ 
				try
				{
					lnRet += lpIt->m_pCallBack(lpAco->GetServiceCode(),loComm);	
				}
				catch (CMemoryException* e)
				{
					BHLOG_ERROR(STR("Interface:%d",lpAco->GetServiceCode()));
					WRITE_FATAL(e);
				}
				catch (CFileException* e)
				{
					BHLOG_ERROR(STR("Interface:%d",lpAco->GetServiceCode()));
					WRITE_FATAL(e);
				}
				catch (CException* e)
				{
					BHLOG_ERROR(STR("Interface:%d",lpAco->GetServiceCode()));
					WRITE_FATAL(e);
				}
			}
			catch(...)
			{
				BHLOG_ERROR(STR("Interface:%d",lpAco->GetServiceCode()));
				WRITE_FATAL_LOG;
			}
	
		
		
		
		//END_ERROR_HANDLE;
	}

	return lnRet;	
}

int CICEServerApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	BEGIN_ERROR_HANDLE;	
	if (NULL!= CICEServerApp::m_pServer)
	{
		CICEServerApp::m_pServer->Stop();
	}
	END_ERROR_HANDLE;
	return CWinApp::ExitInstance();
}
