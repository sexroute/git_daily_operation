#if !defined(AFX_CustomLISTCTRL_H__5658E9E1_18F8_11D3_8029_00105A1FCFF5__INCLUDED_)
#define AFX_CustomLISTCTRL_H__5658E9E1_18F8_11D3_8029_00105A1FCFF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrlEx window
#define WM_CUSTOM_LISTEX_ACTIVE  WM_USER+28500+0
#define WM_CUSTOM_LISTEX_SELECT  WM_USER+28500+1
#define WM_CUSTOM_LISTEX_CHECK   WM_USER+28500+2

//class   AFX_CLASS_EXPORT CCustomListCtrlEx : public CListCtrl
class   AFX_EXT_CLASS CCustomListCtrlEx : public CListCtrl
{
public:
	enum EHighlight {HIGHLIGHT_NORMAL, HIGHLIGHT_ALLCOLUMNS, HIGHLIGHT_ROW};
protected:
	int  m_nHighlight;
private:
	typedef struct ColStruct{
		char name[11];
		char type;
		//unsigned short offs;
		//char remain[2];
		unsigned char width;
		unsigned char dec;
		//char remain1[14];
	} ListCol;

// Construction
protected:
	CPalette m_pal;
	CBitmap m_bitmap;
	int m_cxBitmap, m_cyBitmap;

public:
	CCustomListCtrlEx();
    BOOL SetBkImage(LPCTSTR lpszResourceName);
	BOOL SetBkImage(UINT nIDResource);
	BOOL SetBkImageFile( const char *pszFileName );
// Attributes
public:
	void SetCheck(int ItemIndex = -1,BOOL bCheck =TRUE);
	void AddExStyle(DWORD dwNewStyle=LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomListCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOwnerDraw(BOOL bDraw=TRUE);
	void AdjustFields(int nType=0);
	BOOL GetCheck(int nRow=-1);
	int GetCount();
	int GetNo();
	BOOL DeleteAllItems();
	BOOL DeleteItem( int nItem=-1);
	void SetOnlyCheck(BOOL bOnlyCheck=TRUE);
	int GetIndex(const char* szName);
	int GetImageIndex( int item);
	CString Get(int nID,int nRow = -1);
	CString Get(const char * szName,int nRow = -1);

	BOOL	Set(int nID,const char * s,int nRow = -1);
	BOOL	Set(int nID,int l,int nRow = -1);
	BOOL	Set(int nID,double v,int nRow = -1);

	BOOL	Set(const char * szName,const char * s,int nRow = -1);
	BOOL	Set(const char * szName,int l,int nRow = -1);
	BOOL	Set(const char * szName,double v,int nRow = -1);


	BOOL SetValue(const char* s,int nRow = -1);
	BOOL SetValue(int l,int nRow = -1);

	BOOL SetImage(int nImage,int nRow = -1);
	BOOL SetImageSub( int nImage, int nSubItem, int nRow=-1);

	CString GetValue(int nRow = -1);


	void Go(int no);
	
	BOOL Append(int nIndex=-1 ,const char* csExData=NULL, int nImage = -1);
	
	BOOL CreateHead(const char* szScr);
	
	CImageList* SetImageList(UINT nImgID,int cx=16,COLORREF crMark=RGB(255,0,255),int nImageType=LVSIL_SMALL );

	virtual ~CCustomListCtrlEx();

	BOOL m_bOnlyCheck;

	CImageList imgList;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomListCtrlEx)
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReturn(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    BOOL PreCreateWindow(CREATESTRUCT& cs);    
	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) ;
    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	CPtrArray 	m_arCol;
	CStringArray m_arExData;
	int         m_nPos;
	int			m_iShowCount;
	BOOL		m_bOwnerDraw;
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CustomLISTCTRL_H__5658E9E1_18F8_11D3_8029_00105A1FCFF5__INCLUDED_)
