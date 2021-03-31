#if !defined(AFX_APEXTREECTRL_H__0480A725_1C09_11D3_8029_00105A1FCFF5__INCLUDED_)
#define AFX_APEXTREECTRL_H__0480A725_1C09_11D3_8029_00105A1FCFF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ApexTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CApexTreeCtrl window
#define WM_APEX_TREE_ACTIVE   WM_USER+29000+0
#define WM_APEX_TREE_SELECT   WM_USER+29000+1
#define WM_APEX_TREE_EXPAND   WM_USER+29000+2
#define WM_APEX_TREE_KEYDOWN  WM_USER+29000+3

//#include "apexdef.h"

//class _declspec(dllexport) CApexTreeCtrl : public CTreeCtrl
class AFX_EXT_CLASS CApexTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CApexTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApexTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetItem(  LPCTSTR lpszItem, LPCTSTR lpszNote=NULL, int nImage=-1,int nID=-1);
	BOOL DeleteAllItems();
	BOOL DeleteItem( int nID=-1 );
	BOOL DeleteSubItem(int nID=-1);
	int GetClass(int nID=-1);
	BOOL IsExpand(int nID=-1);
	int FindID(const char* szValue,int nPos=0);
	int FindIDByNote( const char * szNote , int nPos = 0 );
	int FindSubID(const char *szValue, int nIndex);
	int FindSubID(int id, int nIndex);

	CImageList* SetImageList( UINT nImgID,int cx = 16,COLORREF crMark=RGB(255,0,255));
	CImageList* SetImageList(const char *pszFileName,int cx=16,COLORREF crMark=RGB(255,0,255));
	int		GetActiveID();
	CString GetString(int nID = -1);
	CString GetNote(int nID = -1);
	CStringArray mItemList;
	int AddItem(int nPos,const char * csItem,const char * csNote = NULL,int nImage = -1);
	virtual ~CApexTreeCtrl();

	CImageList imgList;
	// Generated message map functions
protected:
	BOOL RemoveItemString(int nPos);
	BOOL RemoveConItemString(int nPos,BOOL bFirstCall=TRUE);
	BOOL ResetItemExData(int exData,HTREEITEM hItem,BOOL bFirstCall =TRUE /*must be TRUE*/);
	//{{AFX_MSG(CApexTreeCtrl)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_APEXTREECTRL_H__0480A725_1C09_11D3_8029_00105A1FCFF5__INCLUDED_)
