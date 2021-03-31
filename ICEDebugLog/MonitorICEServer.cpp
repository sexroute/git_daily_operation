#include "StdAfx.h"
#include ".\monitoriceserver.h"
#include "Resource.h"
#include "DlgMonitor.h"
#include "BHACO.h"
#include "ICEDebugLog.h"

CMonitorICEServer* CMonitorICEServer::pMonitor = NULL;
HWND CMonitorICEServer::hLogWnd = NULL;

CMonitorICEServer::CMonitorICEServer(void)
{	
}

CMonitorICEServer* CMonitorICEServer::GetInstance()
{
	if(NULL == pMonitor)
	{
		pMonitor = new CMonitorICEServer();
	}
	return pMonitor;
}

bool CMonitorICEServer::InitialDialog()
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialog* pDlg = new CDlgMonitor;
	pDlg->Create(IDD_DIALOG_MONITOR,CWnd::FromHandle(GetDesktopWindow()));
	pDlg->ShowWindow(SW_SHOWNORMAL);
	hLogWnd = pDlg->GetSafeHwnd();
	return true;
}

// 查找进程内的ICE服务监测窗口
HWND CMonitorICEServer::FindWindowByProcessID(DWORD dwPID_)
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
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				CString strText;
				pWnd->GetWindowText(strText);
				if(strText == "中间件服务监控")
				{
					return hWnd;
				}
			}
		}
		hWnd = GetNextWindow( hWnd , GW_HWNDNEXT);

	}
	return NULL;
}

void CMonitorICEServer::requestdebuginfo(const char* rhost, 
										 const int ver,
										 const int code,
										 int result, 
										 bool ssl, 
										 const char* time, 
										 const void* request, 
										 const void* response)
{
	debugitem * pdebugitem = new debugitem;
	pdebugitem->_bdebuginfo = response == NULL? false: true;
	pdebugitem->_rows = result;
	pdebugitem->_stripaddr = rhost;
	pdebugitem->_strtime = time;
	pdebugitem->code = code;
	pdebugitem->request  = const_cast<void*>(request);
	pdebugitem->response = const_cast<void*>(response);

	//查找监控窗口
	CWnd *pWnd=CWnd::FindWindow(NULL,_T("中间件服务监控"));    //查找监控窗口

	//没找到，清理数据，返回
	if(pWnd==NULL){
		
		BEGIN_ERROR_HANDLE
		SDataPacket* dp = (SDataPacket*)pdebugitem->request;
		DSE_DeletePointEx(dp);
	
		SDataPacket* dps = (SDataPacket*)pdebugitem->response;
		DSE_DeletePointEx(dps);
	
		delete pdebugitem;
		pdebugitem = NULL;
		END_ERROR_HANDLE
		return;
	}

	if(hLogWnd == NULL)
	{
		DWORD dwPID  =  ::GetCurrentProcessId();
		hLogWnd = this->FindWindowByProcessID(dwPID);
	}
	
	//找到了，显示请求应答信息
	if(hLogWnd != NULL)
	{
		::SendMessage(hLogWnd,WM_MSG_SHOW_LOG,NULL,(LPARAM)pdebugitem);
	}
	else
	{
		BEGIN_ERROR_HANDLE
		SDataPacket* dp = (SDataPacket*)pdebugitem->request;
		DSE_DeletePointEx(dp);

		SDataPacket* dps = (SDataPacket*)pdebugitem->response;
		DSE_DeletePointEx(dps);

		delete pdebugitem;
		pdebugitem = NULL;
		END_ERROR_HANDLE
	}
	
}

bool CMonitorICEServer::DestroyMonitorWindow()
{
	
	DWORD dwPID  =  ::GetCurrentProcessId();
	HWND hWnd = this->FindWindowByProcessID(dwPID);
	
	if(hWnd != NULL)
	{
		::SendMessage(hWnd,WM_MSG_DESTROY_WINDOW,NULL,NULL);
	}
	
	return true;
}

void CMonitorICEServer::SetServerInfo( CString & astrInfo,BOOL abSucceed )
{
	if(hLogWnd == NULL)
	{
		DWORD dwPID  =  ::GetCurrentProcessId();
		hLogWnd = this->FindWindowByProcessID(dwPID);
	}

	//找到了，显示请求应答信息
	if(hLogWnd != NULL)
	{
		::SendMessage(hLogWnd,WM_MSG_SET_INIT_INF,(WPARAM)(&astrInfo),abSucceed);
	}
}
