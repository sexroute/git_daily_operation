#if !defined(AFX_CustomCOMBOBOX_H__74CC8E24_2958_11D3_8029_00105A1FCFF5__INCLUDED_)
#define AFX_CustomCOMBOBOX_H__74CC8E24_2958_11D3_8029_00105A1FCFF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomComboBox window

class AFX_EXT_CLASS CCustomComboBox : public CComboBox
{
// Construction
public:
	CCustomComboBox();

// Attributes
public:

// Operations
public:
// Overrides
	int AddString( LPCTSTR lpszString );
	int AddString( LPCTSTR lpszValue,LPCTSTR lpszNote);
	int InsertString( int nIndex, LPCTSTR lpszString );
	int InsertString( int nIndex, LPCTSTR lpszValue,LPCTSTR lpszNote );
	int DeleteString( UINT nIndex );
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetContent();
	CString GetString(int nIndex = -1);
	int SetSel( LPCTSTR lpszString );
    CString GetNote(int nIndex =-1);
	virtual ~CCustomComboBox();

	// Generated message map functions
protected:
	CStringArray m_arItem;
	BOOL m_bAutoComplete;

	//{{AFX_MSG(CCustomComboBox)
	afx_msg void OnEditupdate();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CustomCOMBOBOX_H__74CC8E24_2958_11D3_8029_00105A1FCFF5__INCLUDED_)
