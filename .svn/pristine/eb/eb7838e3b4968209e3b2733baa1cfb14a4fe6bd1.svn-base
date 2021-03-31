#pragma once
#include "Resource.h"
#include "BHACO.h"
#include <string>

using namespace std;
using namespace BHAco;

// CDlgDebugInfo 对话框
class debugitem{
public:
	debugitem();
	~debugitem();
	debugitem& operator=(const debugitem& item_);
public:
	///SDataPacket point
	void* request;
	void* response;
	///是否有详细调试信息
	bool _bdebuginfo;
	unsigned int _rows;
	string _strtime;
	string _stripaddr;
	int ver;
	unsigned int code;
private:
	//void clear();
};

class CDlgDebugInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgDebugInfo)

public:
	CDlgDebugInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDebugInfo();

// 对话框数据
	enum { IDD = IDD_DLG_DEBUGINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnBnClickedButtonShowchg();
    
	int SetDebugInfo(const debugitem* item_);
private:
	bool __bResponse;
	///调试信息
    debugitem __debugitem;
private:
	//显示请求或者响应信息切换
	bool ChangeInfoState();
	//填充列表的标题
	int SetResultListCol();
    int SetRowDetailListCol();
	///填充列表内容
	int FillResultList();
	int FillRowDetailList(int row_);

	SDataPacket* GetSdp();

	CString FieldInfoFormatOutput(const SFieldContent& fc_);
public:
	virtual INT_PTR DoModal();
	afx_msg void OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_listResult;
	CListCtrl m_listRowInfo;
	virtual BOOL OnInitDialog();
};
