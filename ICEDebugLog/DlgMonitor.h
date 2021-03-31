#pragma once
#include "Resource.h"
#include "IceUtil/IceUtil.h"
#include "DlgDebugInfo.h"
#include <list>
using namespace std;
// CDlgMonitor �Ի���
#define MAXIMUM_NUMBER_OF_LOGS          100
#define WM_MYMESSAGE                   WM_USER + 102

const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

class CDlgMonitor : public CDialog
{
	//DECLARE_DYNAMIC(CDlgMonitor)

public:
	CDlgMonitor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMonitor();

// �Ի�������
	enum { IDD = IDD_DIALOG_MONITOR };

protected:
	DECLARE_MESSAGE_MAP()

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	
	virtual void PostNcDestroy();

public:
	afx_msg void OnNMDblclkListLog(NMHDR *pNMHDR, LRESULT *pResult);

    //��ʾ����
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
	//�ⲿ���ã����������Ϣ	
	void ClearListDebugInfo();
	afx_msg LRESULT OnTaskBarCreated(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedBtnShowMainWindow();

private:
	int FillList();

	//ע��ֻ���ڲ�����
	int cleardebugitems(list<debugitem*>& ldebugitem_);
    
	//�����������
	void ClearListMonitorInfo();
    
	//�������ݰ���ʾ
	void SetPackNum(debugitem* pDebugItem);
   
	//��ʼ������ؼ�
	void InitialCtrl();

	HWND FindWindowByProcessID(DWORD dwPID_);

private:
	IceUtil::Mutex __mutexddebugitems;

	///�����Ϣ�б�
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
