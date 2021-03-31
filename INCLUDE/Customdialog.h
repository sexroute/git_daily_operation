#if !defined(AFX_CustomDIALOG_H__73972007_1F13_11D3_8029_00105A1FCFF5__INCLUDED_)
#define AFX_CustomDIALOG_H__73972007_1F13_11D3_8029_00105A1FCFF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomDialog.h : header file
//
#include "dib256.h"

#define ICONINFORMATION 0
#define ICONWARN        1
#define ICONSTOP        2
#define ICONQUESTION    3
//#include "Customdef.h"
/////////////////////////////////////////////////////////////////////////////
// CCustomDialog dialog

//class _declspec(dllexport) CCustomDialog : public CDialog
class AFX_EXT_CLASS CCustomDialog : public CDialog
{
// Construction
public:
	void EnableNote( BOOL bEnable);
	struct CItemSizeInfo {
		CItemSizeInfo();
		HWND m_hWnd;
		CRect m_rtItem;
		DWORD m_dwAlign;
		CItemSizeInfo *m_pNext;
	};

	void SetMinMaxInfo(CSize szMin,CSize szMax);
	int  SetItemAlign(UINT nID,DWORD dwAlign);
	int  SetItemAlign(CWnd* pWnd,DWORD dwAlign);
	void SetNote(UINT nStringID,UINT nType=ICONINFORMATION);
	void SetNote(LPCSTR lpszNote,UINT nType=ICONINFORMATION);
	void SetCtrlTran(BOOL bTran=TRUE);  
	BOOL AddAccelKey(UINT idCmd,BYTE fVirt,WORD key);
	void SetEnterToTab(BOOL bSet=TRUE);
	BOOL m_bEnterToTab;
	void SetNoteVisible(BOOL bVisible=TRUE);
	enum {
		BITMAP_NULL = 0,
		BITMAP_TILE,		// tile the bitmap to fill the dialog
		BITMAP_STRETCH,		// stretch the bitmap so it fits to the dialog
		BITMAP_CENTER		// center the bitmap inside the dialog
	};
	
	enum {
		alRight = 1  << 0,
		alBottom = 1 << 1,
		alWidth = 1 << 2, 
		alHeight = 1 << 3 
	};
	
	
	void AddNote(UINT nID,const char* szNote);
	void AddNote(UINT nID,UINT nStringID);
	BOOL SetBitmap(UINT uResource, int Type = CCustomDialog::BITMAP_STRETCH);
	BOOL SetBitmap(UINT uResource, HMODULE hModule,int Type = CCustomDialog::BITMAP_STRETCH);
	CCustomDialog(UINT nIDTemplate,CWnd* pParent = NULL);   // standard constructor
    ~CCustomDialog();
	static BOOL  CompareClassName(HWND hWnd, LPCTSTR lpszClassName);
// Dialog Data
	//{{AFX_DATA(CCustomDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ScanNote();
    clock_t m_clNote;
	CDWordArray  m_arCtrlID;
	CDWordArray  m_arStringID;
	CStringArray m_arNote;
	BOOL m_bNote;
	BOOL m_bNoteEnable;	
	CStatusBarCtrl m_ctrlNote;
	CWnd * pWndFocus;

	CDIBitmap	m_bmpBackground;
	int			m_nType;		// see enum above
	CBrush		m_HollowBrush;
    long        m_nIDPwd     ;
	BOOL        m_bCtrlTran; //Control is always Transparent

	HACCEL m_hAccel;
	// Generated message map functions
	//{{AFX_MSG(CCustomDialog)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
protected:
	//For resize window.....
	//CDWordArray  m_arItemAlign;
	//CDWordArray  m_arItemHwnd;
	CItemSizeInfo* m_pSizeInfo;

	CRect        m_rtClient;
	BOOL         m_bFirstSize;
	CPoint       m_ptMin,m_ptMax;

	void DelItemSizeInfo();

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CustomDIALOG_H__73972007_1F13_11D3_8029_00105A1FCFF5__INCLUDED_)
