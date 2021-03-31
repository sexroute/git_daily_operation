// DlgDebugInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgDebugInfo.h"
#include ".\dlgdebuginfo.h"
#include "Resource.h"
#include "BHACO.h"

// CDlgDebugInfo 对话框

IMPLEMENT_DYNAMIC(CDlgDebugInfo, CDialog)
CDlgDebugInfo::CDlgDebugInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDebugInfo::IDD, pParent)
{
__bResponse = false;
}

CDlgDebugInfo::~CDlgDebugInfo()
{
}

void CDlgDebugInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_LIST_FIELDINFO, m_listRowInfo);
}


BEGIN_MESSAGE_MAP(CDlgDebugInfo, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SHOWCHG, OnBnClickedButtonShowchg)
	ON_NOTIFY(NM_CLICK, IDC_LIST_RESULT, OnNMClickListResult)
END_MESSAGE_MAP()


// CDlgDebugInfo 消息处理程序

BOOL CDlgDebugInfo::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::Create(lpszTemplateName, pParentWnd);
}

void CDlgDebugInfo::OnBnClickedButtonShowchg()
{
	ChangeInfoState();
}

bool CDlgDebugInfo::ChangeInfoState()
{
	__bResponse = !__bResponse;
	if (__bResponse)
	{
        SetDlgItemText(IDC_BUTTON_SHOWCHG,"Request");
		SetDlgItemText(IDC_STATIC_RESULT, "Response(响应) Information");
	}
	else
	{
		SetDlgItemText(IDC_BUTTON_SHOWCHG,"Response");
		SetDlgItemText(IDC_STATIC_RESULT, "Request(请求) Information");
	}
	SetResultListCol();
	FillResultList();
	return __bResponse;
}


INT_PTR CDlgDebugInfo::DoModal()
{
	return CDialog::DoModal();
}

int CDlgDebugInfo::FillResultList()
{
  m_listResult.DeleteAllItems();
  int nCount = 0;
  SDataPacket *dp = GetSdp();
  if (!dp)
  {
	  return -1;
  }
  int rows = dp->packet.size();
  if (rows <  1)
  {
	  return -2;
  }
  int nFields =dp->packet[0].size();  
  LVITEM lvi;
  CString strItem;
  for (int ind = 0; ind < rows; ++ind)
  {      
     
	  lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
	  strItem.Format(_T("%d"),ind);

	  lvi.iItem = ind;
	  lvi.iSubItem = 0;
	  lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
	  m_listResult.InsertItem(&lvi);

	  int indField = 1;
	  RowDetail::iterator rowEnd = dp->packet[ind].end();
	  for(RowDetail::iterator it = dp->packet[ind].begin();it != rowEnd; ++it)
	  {
		  strItem.Format("%d", it->first);
		  lvi.iSubItem =indField;
		  lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		  m_listResult.SetItem(&lvi);
		  ++indField;
		  if (indField > nFields)
			  break;
	  }
	  nCount++;
  }
  return rows;
}

int CDlgDebugInfo::FillRowDetailList(int row_)
{
	m_listRowInfo.DeleteAllItems();
	SDataPacket* dp = GetSdp();
	if(row_ < 0 ||  (dp == NULL) || (row_ > dp->packet.size()))
		return -1;

	LVITEM lvi;
	CString strItem;
	RowDetail& rd = dp->packet[row_];
	int nCount = 0;
	for (RowDetail::iterator it = rd.begin(); it != rd.end(); ++it )
	{
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
		strItem.Format(_T("%d"),it->first);

		lvi.iItem = nCount;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listRowInfo.InsertItem(&lvi);
        
		SFieldContent& fc = it->second;
		strItem = FieldInfoFormatOutput(fc);
		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listRowInfo.SetItem(&lvi);
	}
	return nCount;
}

int CDlgDebugInfo::SetResultListCol()
{
	int nCount = 0;
	int nColumnCount = m_listResult.GetHeaderCtrl()->GetItemCount();
	for (int ind=0; ind < nColumnCount; ind++)
		m_listResult.DeleteColumn(0);
	
	SDataPacket *dp = GetSdp();
	if (!dp)
	  return -1;
	if (dp->packet.size()< 1)
		return -2;
	nCount = dp->packet.begin()->size();
	if (nCount < 1)
	  return -2;
	CRect rc;
	m_listResult.GetClientRect(&rc);
	int nColInterval = rc.Width()/nCount;
	CString str;
	m_listResult.InsertColumn(0,__T("Row"), LVCFMT_CENTER, nColInterval);
	for (int ind = 0; ind < nCount; ++ind)
	{
		str.Format("Field%d",ind);
		m_listResult.InsertColumn(ind+1,str, LVCFMT_LEFT, nColInterval);		
	}
	return nCount;
}

int CDlgDebugInfo::SetRowDetailListCol()
{
	CRect rc;
	m_listRowInfo.GetClientRect(&rc);
	m_listRowInfo.InsertColumn(0,__T("FieldCode"), LVCFMT_CENTER, 80);
	m_listRowInfo.InsertColumn(1, __T("Value"), LVCFMT_LEFT, rc.Width()-80);		
	return 2;
}

int CDlgDebugInfo::SetDebugInfo(const debugitem* item_)
{
	__debugitem = (*item_);
	return 0;
}

void CDlgDebugInfo::OnNMClickListResult(NMHDR *pNMHDR, LRESULT *pResult)
{
	int row =( (NM_LISTVIEW*)pNMHDR)->iItem;
	FillRowDetailList(row);    
	*pResult = 0;
}

SDataPacket* CDlgDebugInfo::GetSdp()
{
	SDataPacket *dp = NULL;
	if (__bResponse)
		dp =(SDataPacket *)( __debugitem.response);
	else
		dp = (SDataPacket *)(__debugitem.request);
	return dp;
}

BOOL CDlgDebugInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_listRowInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listResult.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	SetRowDetailListCol();
	ChangeInfoState();
	CString str;
	str.Format("%d",__debugitem.code);
	SetDlgItemText(IDC_EDIT_SERVICECODE, str);
    
	str = __debugitem._strtime.c_str();
    SetDlgItemText(IDC_EDIT_SENDTIME, str);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

CString CDlgDebugInfo::FieldInfoFormatOutput(const SFieldContent& fc_)
{
	CString str ="";
	switch(fc_.fType)
	{
	case FTFloat:
		str.Format("float: %.4f", fc_.fValue);
		break;
	case FTInt:
		str.Format("int: %d", fc_.iValue);
		break;
	case FTString:
		str.Format("string: %s", fc_.strValue.c_str());
		break;
	case FTDateTime:
		str.Format("datetime: %s", fc_.strValue.c_str());
		break;
	case FTBytes:
		str.Format("bufferfield, size:%d", fc_.buf.size());
		break;
	case FTBool:
		str.Format("bool: %s", fc_.iValue>0? "true":"false");
		break;
	case FTChar:		
		if (fc_.buf.size()>0)
		  str.Format("char: %c", fc_.buf[0]);
		else
		  str.Format("char: null");
		break;
	}
	return str;
}


debugitem::debugitem(){
	request = NULL;
	response = NULL;
}
debugitem::~debugitem(){

}
static void copyDataPacket(SDataPacket& des_, const SDataPacket& src_)
{
	des_ = src_;	return;

	des_.curWarnings = src_.curWarnings;
	des_.msgHeader   = src_.msgHeader;
	des_.nRetCode    = src_.nRetCode;
	des_.packet.clear();
	for (PackageDetail::const_iterator it = src_.packet.begin(); it != src_.packet.end(); ++it)
	{
		RowDetail rd;
		rd.clear();
		for (RowDetail::const_iterator itRow = it->begin(); itRow != it->end(); ++itRow)
			rd.insert(*itRow);
		des_.packet.push_back(rd);
	}
}
debugitem& debugitem::operator=(const debugitem& item_){
	_bdebuginfo = item_._bdebuginfo;
	_rows = item_._rows;
	_strtime = item_._strtime;
	_stripaddr = item_._stripaddr;
	ver = item_.ver;
	code = item_.code;
	if (_bdebuginfo)
	{
		if (item_.request)
		{
			SDataPacket * pdp  = new SDataPacket;
			SDataPacket& dp =*(SDataPacket*)(item_.request);
			copyDataPacket(*pdp, dp);
			request = pdp;
		}
		if (item_.response)
		{
			SDataPacket * pdp  = new SDataPacket;
			SDataPacket& dp = *(SDataPacket*)(item_.response);
			copyDataPacket(*pdp, dp);
			response = pdp;
		}		  
	}
	///是否有详细调试信息
	return *this;
}
