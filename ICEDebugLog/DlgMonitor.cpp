// DlgMonitor.cpp : 实现文件
//

#include "stdafx.h"
#include "ICEDebugLog.h"
#include "DlgMonitor.h"
#include ".\dlgmonitor.h"
#include "MonitorICEServer.h"
#include "DlgDebugInfo.h"
#include "Resource.h"
#include <WinSock.h>
// CDlgMonitor 对话框

const int g_nMaxPackNum = 42949672;

CDlgMonitor::CDlgMonitor(CWnd* pParent /*=NULL*/)
: CDialog(CDlgMonitor::IDD, pParent)
, m_strInitInfo(_T(""))
, m_nRequestPackNum(0)
, m_nResponsePackNum(0)
{
	m_bDebug = false;
	m_bMonitor = false;
}

CDlgMonitor::~CDlgMonitor()
{

}

void CDlgMonitor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_listLog);
	DDX_Text(pDX, IDC_EDIT_IP_ADDRESS, this->m_strInitInfo);
}

BEGIN_MESSAGE_MAP(CDlgMonitor, CDialog)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LOG, OnNMDblclkListLog)
	ON_MESSAGE(WM_MYMESSAGE,OnMyMessage)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_COMMAND(IDR_MUNU_SHOW, OnMenuRestore)
	ON_BN_CLICKED(IDC_BTN_CLEAR, OnBnClickedBtnClear)
	ON_MESSAGE(WM_MSG_SHOW_LOG,OnShowDebugInfo)
	ON_MESSAGE(WM_MSG_DESTROY_WINDOW,OnDestroyWindow)
	ON_MESSAGE(WM_MSG_SET_INIT_INF,OnSetInitInfo)
	ON_BN_CLICKED(IDC_CHECK_MORNITOR_INFO, OnBnClickedCheckMornitorInfo)
	ON_BN_CLICKED(IDC_CHECK_DEBUG_INFO, OnBnClickedCheckDebugInfo)
	ON_REGISTERED_MESSAGE(WM_TASKBARCREATED, OnTaskBarCreated)
	ON_BN_CLICKED(IDC_BTN_SHOW_MAIN_WINDOW, OnBnClickedBtnShowMainWindow)
END_MESSAGE_MAP()


// CDlgMonitor 消息处理程序

BOOL CDlgMonitor::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: 在此添加额外的初始化代码
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	this->m_strInitInfo = _T("initializing ...");
	InitialCtrl();

	UpdateData(FALSE);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}


void CDlgMonitor::InitialCtrl()
{

	CRect rect;
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	m_listLog.GetWindowRect(&rect);

	m_listLog.ModifyStyle(LVS_ICON | LVS_LIST | LVS_REPORT | LVS_SMALLICON|LVS_SORTASCENDING ,0);
	m_listLog.ModifyStyle(0, LVS_REPORT|LVS_NOSORTHEADER);
	m_listLog.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_listLog.InsertColumn(0,"来源",LVCFMT_LEFT,rect.Width()/5);
	m_listLog.InsertColumn(1,"业务",LVCFMT_LEFT,rect.Width()/10);
	m_listLog.InsertColumn(2,"结果",LVCFMT_LEFT,rect.Width()/10);
	m_listLog.InsertColumn(3,"时间",LVCFMT_LEFT,rect.Width()*3/10);
	m_listLog.InsertColumn(4,"备注",LVCFMT_LEFT,rect.Width()*3/10);

	pnid.cbSize = sizeof(NOTIFYICONDATA);
	pnid.hWnd=m_hWnd;                            //窗口句柄  
	pnid.uID=(UINT)m_hIcon;                      //图标句柄  
	pnid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;   //图标有效|自定义消息有效|鼠标指向显示文字有效  
	pnid.uCallbackMessage=WM_MYMESSAGE;          //自定义的消息名称  
	pnid.hIcon=m_hIcon;                          //图标句柄  
	strcpy(pnid.szTip,"ICESvr-正在运行");           //鼠标指向所显示的文字
	Shell_NotifyIcon(NIM_ADD,&pnid);             //在托盘区添加图标

	GetDlgItem(IDC_EDIT_RECEIVE_PACK_NUM)->SetWindowText("0");
	GetDlgItem(IDC_EDIT_SEND_PACK_NUM)->SetWindowText("0");
}

void CDlgMonitor::OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0; 

	if(!((CButton*)GetDlgItem(IDC_CHECK_DEBUG_INFO))->GetCheck())
	{
		return;
	}

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CDlgDebugInfo dlg(this);
	{
		IceUtil::Mutex::Lock guard(__mutexddebugitems);
		int ind = pNMListView->iItem;
		if ( ind < 0)
			return;
		debugitem* item = (debugitem*)m_listLog.GetItemData(ind);
		if(!item)
			return;
		dlg.SetDebugInfo(item);
	}
	dlg.DoModal();	
}

int CDlgMonitor::request(void* debugitem_)
{
	{
		IceUtil::Mutex::Lock guard(__mutexddebugitems);
		int len = cleardebugitems(__ldebugitem);
		__ldebugitem.push_back((debugitem*)debugitem_);
		if (len ==0 )
		{
			m_listLog.DeleteAllItems();
		}
	}
	return FillList();
}
int CDlgMonitor::cleardebugitems(list<debugitem*>& ldebugitem_)
{
	debugitem *req = NULL;
	int nCount = ldebugitem_.size();
	if( ldebugitem_.size() >=MAXIMUM_NUMBER_OF_LOGS)
	{
		BEGIN_ERROR_HANDLE
		for (list<debugitem*>::iterator it = ldebugitem_.begin(); it != ldebugitem_.end(); ++it)
		{
			req = *it;

			SDataPacket* dp = (SDataPacket*)req->request;
			DSE_DeletePointEx(dp);

			SDataPacket* dps = (SDataPacket*)req->response;
			DSE_DeletePointEx(dps);
			
			delete req;
			*it = NULL;
		}
		ldebugitem_.clear();
		END_ERROR_HANDLE
		nCount = 0;
	}
    return nCount;
}

void CDlgMonitor::ClearListDebugInfo()
{
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	m_listLog.DeleteAllItems();
	cleardebugitems(__ldebugitem);
}

// 显示调试信息
int CDlgMonitor::FillList()
{
	debugitem * req = NULL;
	
	CString strItem;
	LVITEM lvi;
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	list<debugitem*>::iterator it = __ldebugitem.end(); --it;
	if (it != __ldebugitem.begin())
	{
		req = *it;
		if (req)
		{
			// Insert the first item
			lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
			strItem.Format(_T("%s"), req->_stripaddr.c_str());

			lvi.iItem = 0;
			lvi.iSubItem = 0;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			m_listLog.InsertItem(&lvi);

			strItem.Format("%d",req->code);
			// Set subitem 1
			lvi.iSubItem =1;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			m_listLog.SetItem(&lvi);

			// Set subitem 2
			strItem.Format("%d",req->_rows);
			lvi.iSubItem =2;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			m_listLog.SetItem(&lvi);

			lvi.iSubItem =3;
			lvi.pszText = (LPTSTR)(LPCTSTR)(req->_strtime.c_str());
			m_listLog.SetItem(&lvi);

			strItem = "";
			lvi.iSubItem =4;
			lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
			m_listLog.SetItem(&lvi);		 
			if(m_bDebug)
			{
				m_listLog.SetItemData(lvi.iItem,(DWORD_PTR)req);
			}
			else
			{
				m_listLog.SetItemData(lvi.iItem,NULL);
			}
		}
	}
	return __ldebugitem.size();
}

void CDlgMonitor::SetPackNum(debugitem* pDebugItem)
{
	
	if (NULL == pDebugItem)
	{
		return;
	}
	IceUtil::Mutex::Lock guard(__mutexddebugitems);

	SDataPacket * pRequest = NULL,* pResponse = NULL;
	pResponse = (SDataPacket*)pDebugItem->response;
	pRequest  = (SDataPacket*)pDebugItem->request;
    
	//接收包
	if(pRequest)
	{
		if(m_nRequestPackNum >= g_nMaxPackNum)
		{
			m_nRequestPackNum = 0;
		}
		else
		{
			m_nRequestPackNum++;
		}
	}

	//发送包
	if(pResponse)
	{
		if(m_nResponsePackNum >= g_nMaxPackNum)
		{
			m_nResponsePackNum = 0;
		}
		else
		{
			if(pResponse->packet.size() > 0)
			{	
				m_nResponsePackNum += pResponse->packet.size();
			}
			else
			{
				m_nResponsePackNum++;
			}
		}
	}

	m_strResponsePackNum.Format("%d",m_nResponsePackNum);
	GetDlgItem(IDC_EDIT_SEND_PACK_NUM)->SetWindowText(m_strResponsePackNum);

	m_strRequestPackNum.Format("%d",m_nRequestPackNum);
	GetDlgItem(IDC_EDIT_RECEIVE_PACK_NUM)->SetWindowText(m_strRequestPackNum);
	
}

void CDlgMonitor::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(nType = SW_MINIMIZE)
	{
		this->ShowWindow(SW_HIDE);//隐藏主窗口   
	}
	// TODO: 在此处添加消息处理程序代码
}

void CDlgMonitor::OnMenuRestore()
{
	// TODO: 在此添加命令处理程序代码
	if(!ShowWindow(SW_SHOWNORMAL)) //显示窗体
		ShowWindow(SW_SHOWNORMAL); 
}

LRESULT CDlgMonitor::OnMyMessage(WPARAM wparam,LPARAM lparam)
{
	POINT point;
	GetCursorPos(&point);
	CMenu menu;  
	//如果在托盘图标上单击
	if (lparam == WM_LBUTTONDOWN)  
	{  
		if(!ShowWindow(SW_SHOWNORMAL)) //显示窗体
			ShowWindow(SW_SHOWNORMAL);
	}
	//如果在托盘图标上单击右键
	if ( lparam == WM_RBUTTONDOWN)  
	{        
		menu.LoadMenu(IDR_MENU1);             //加载系统主菜单 
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,point.x, point.y, this); //在点击图标处弹出系统菜单的第一项                  /

	}  

	return 0; 
}

void CDlgMonitor::OnBnClickedBtnClear()
{
	// TODO: 在此添加控件通知处理程序代码
	ClearListDebugInfo();
}

LRESULT CDlgMonitor::OnShowDebugInfo(WPARAM wParam, LPARAM lParam)
{
	void* DebugItem = (void*)lParam;

	debugitem *pDebugItem = (debugitem*)DebugItem;

	SetPackNum(pDebugItem);
	
	{
		if(!m_bMonitor)
		{
			BEGIN_ERROR_HANDLE
			SDataPacket* dp = (SDataPacket*)(pDebugItem->request);
			DSE_DeletePointEx(dp);

			SDataPacket* dps = (SDataPacket*)(pDebugItem->response);
			DSE_DeletePointEx(dps);

			delete pDebugItem;
			pDebugItem = NULL;
			END_ERROR_HANDLE
			
			return 0;
		}
	}
	
	return this->request(DebugItem);	
}

void CDlgMonitor::ClearListMonitorInfo()
{
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	m_listLog.DeleteAllItems();
	debugitem *req = NULL;
	int nCount = __ldebugitem.size();
	
	BEGIN_ERROR_HANDLE
	for (list<debugitem*>::iterator it = __ldebugitem.begin(); it != __ldebugitem.end(); ++it)
	{
		req = *it;

		SDataPacket* dp = (SDataPacket*)req->request;
		DSE_DeletePointEx(dp);

		SDataPacket* dps = (SDataPacket*)req->response;
		DSE_DeletePointEx(dps);

		delete req;
		*it = NULL;
	}
	__ldebugitem.clear();
	END_ERROR_HANDLE
}

LRESULT CDlgMonitor::OnDestroyWindow(WPARAM wParam, LPARAM lParam)
{
	this->ClearListMonitorInfo();
	Shell_NotifyIcon(NIM_DELETE, &pnid);
	this->DestroyWindow();
	return 0;
}
void CDlgMonitor::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialog::PostNcDestroy();
	delete this;
}

LRESULT CDlgMonitor::OnSetInitInfo( WPARAM wParam, LPARAM lParam )
{
	CString lstrData;

	if (NULL!=wParam)
	{
		lstrData = *((CString *)wParam);
	}
	int lnInitSucceed = (int)lParam;	
	this->m_strInitInfo = lstrData;
	UpdateData(FALSE);
	return S_OK;
}

void CDlgMonitor::OnBnClickedCheckMornitorInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	m_bMonitor = ((CButton*)GetDlgItem(IDC_CHECK_MORNITOR_INFO))->GetCheck()?true:false;
}

void CDlgMonitor::OnBnClickedCheckDebugInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	IceUtil::Mutex::Lock guard(__mutexddebugitems);
	m_bDebug = ((CButton*)GetDlgItem(IDC_CHECK_DEBUG_INFO))->GetCheck()?true:false;

}
LRESULT CDlgMonitor::OnTaskBarCreated(WPARAM wParam,LPARAM lParam) 
{
	pnid.cbSize = sizeof(NOTIFYICONDATA);
	pnid.hWnd=m_hWnd;                            //窗口句柄  
	pnid.uID=(UINT)m_hIcon;                      //图标句柄  
	pnid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;   //图标有效|自定义消息有效|鼠标指向显示文字有效  
	pnid.uCallbackMessage=WM_MYMESSAGE;          //自定义的消息名称  
	pnid.hIcon=m_hIcon;                          //图标句柄  
	strcpy(pnid.szTip,"ICESvr-正在运行");           //鼠标指向所显示的文字
	Shell_NotifyIcon(NIM_ADD,&pnid);             //在托盘区添加图标
	return TRUE;
} 
void CDlgMonitor::OnBnClickedBtnShowMainWindow()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwProcessID = ::GetCurrentProcessId();

	HWND hWnd = FindWindowByProcessID(dwProcessID);

	if(hWnd)
	{
		::SendMessage(hWnd, WM_SYSCOMMAND,SC_RESTORE, 0);
	}
}

// 查找进程内的主监测窗口
HWND CDlgMonitor::FindWindowByProcessID(DWORD dwPID_)
{
	HWND hWnd = ::GetTopWindow(0);

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
				if(strText.Find("业务服务系统")>=0)
				{
					return hWnd;
				}
			}
		}
		hWnd = ::GetNextWindow( hWnd , GW_HWNDNEXT);

	}
	return NULL;
}