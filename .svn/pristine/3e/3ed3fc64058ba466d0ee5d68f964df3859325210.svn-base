#pragma once
#include "Resource.h"
#include "IceUtil/IceUtil.h"
#include "DlgDebugInfo.h"
#include <list>
using namespace std;
// CDlgMonitor 对话框
#define MAXIMUM_NUMBER_OF_LOGS          100
#define WM_MYMESSAGE                   WM_USER + 102

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

class CDlgMonitor : public CDialog
{
	//DECLARE_DYNAMIC(CDlgMonitor)

public:
	CDlgMonitor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMonitor();

// 对话框数据
	enum { IDD = IDD_DIALOG_MONITOR };

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	
	virtual void PostNcDestroy();

public:
	afx_msg void OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult);

    //显示界面
	afx_msg void OnMenuRestore();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMyMessage(WPARAM wparam,LPARAM lparam);

	afx_msg void OnBnClickedBtnClear();
	afx_msg LRESULT OnShowDebugInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDestroyWindow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetInitInfo(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedCheckMornitorInfo();
	afx_msg void OnBnClickedCheckDebugInfo();

	int request(void* debugitem_);
	//外部调用，清除调试信息	
	void ClearListDebugInfo();
	afx_msg LRESULT OnTaskBarCreated(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedBtnShowMainWindow();

private:
	int FillList();

	//注意只能内部调用
	int cleardebugitems(list<debugitem*>& ldebugitem_);
    
	//清空所有数据
	void ClearListMonitorInfo();
    
	//设置数据包显示
	void SetPackNum(debugitem* pDebugItem);
   
	//初始化界面控件
	void InitialCtrl();

	HWND FindWindowByProcessID(DWORD dwPID_);

private:
	IceUtil::Mutex __mutexddebugitems;

	///监控信息列表
	list<debugitem*> __ldebugitem;

	NOTIFYICONDATA pnid;

	HICON m_hIcon;

	bool m_bMonitor;

	bool m_bDebug;
	
public:
	CString m_strRequestPackNum;
	CString m_strResponsePackNum;
	int m_nRequestPackNum;
	int m_nResponsePackNum;  
	CString m_strInitInfo;
	int m_iSendPackNum;
	int m_iReceivePackNum;
	CListCtrl m_listLog;
};
