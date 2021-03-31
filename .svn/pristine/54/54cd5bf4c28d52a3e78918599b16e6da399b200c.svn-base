#ifndef ApexMenu_H_INCLUDE
#define ApexMenu_H_INCLUDE

#include <afxtempl.h>

// CApexMenuData class. Fill this class structure to define a single menu item:

class CApexMenuData
{
	wchar_t *m_szMenuText;
public:
	CApexMenuData () {menuIconNormal=-1;xoffset=-1;bitmap=NULL;pContext=NULL;
	nFlags=0;nID=0;syncflag=0;m_szMenuText=NULL;};
	void SetAnsiString(LPCSTR szAnsiString);
	void SetWideString(const wchar_t *szWideString);
	const wchar_t *GetWideString(void) {return m_szMenuText;};
	~CApexMenuData ();
	CString GetString(void);//returns the menu text in ANSI or UNICODE
	int xoffset;
	int menuIconNormal;
	UINT nFlags,nID,syncflag;
	CImageList *bitmap;
	void *pContext; // used to attach user data
};

//struct CMenuItemInfo : public MENUITEMINFO {
struct CMenuItemInfo : public 
//MENUITEMINFO 
#ifndef UNICODE   //SK: this fixes warning C4097: typedef-name 'MENUITEMINFO' used as synonym for class-name 'tagMENUITEMINFOA'
tagMENUITEMINFOA
#else
tagMENUITEMINFOW
#endif
{
	CMenuItemInfo()
	{ memset(this, 0, sizeof(MENUITEMINFO));
	cbSize = sizeof(MENUITEMINFO);
	}
};




typedef enum {Normal,TextOnly} HIGHLIGHTSTYLE;

#ifndef UNICODE
#define AppendMenu AppendMenuA
#define InsertMenu InsertMenuA
#define InsertODMenu InsertODMenuA
#define AppendODMenu AppendODMenuA
#define AppendODPopupMenu AppendODPopupMenuA
#define ModifyODMenu ModifyODMenuA
#else
#define AppendMenu AppendMenuW
#define InsertMenu InsertMenuW
#define InsertODMenu InsertODMenuW
#define AppendODMenu AppendODMenuW
#define ModifyODMenu ModifyODMenuW
#define AppendODPopupMenu AppendODPopupMenuW
#endif


class AFX_EXT_CLASS CApexMenu : public CMenu  // Derived from CMenu
{
	struct ToolBarData	
	{ // struct for toolbar resource; guess you already know it ;)
		WORD wVersion;
		WORD wWidth;			
		WORD wHeight;			
		WORD wItemCount;		
	};
	struct ToolBarItem
	{
		int		iIdx;
		int		iCmd;
		int     iToolID; //add for support more toolbar,xu,2000.04.20
	};
	// Construction
	public:
		CApexMenu(); 
		
		// Attributes
	protected:
		CTypedPtrArray<CPtrArray, CApexMenuData*> m_MenuList;  // Stores list of menu items 
		
		// When loading an owner-drawn menu using a Resource, CApexMenu must keep track of
		// the popup menu's that it creates. Warning, this list *MUST* be destroyed
		// last item first :)
		CTypedPtrArray<CPtrArray, HMENU>  m_SubMenus;  // Stores list of sub-menus 
		
		// Stores a list of all CApexMenu's ever created 
		static CTypedPtrArray<CPtrArray, HMENU>  m_AllSubMenus;
		
		// Operations
	public: 
		// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCustomMenu)
		//}}AFX_VIRTUAL 
		// Implementation
	public:
		static BOOL IsNewShell(void);
		void SetBitmapBackground(COLORREF color);
		void SetDisableOldStyle(void);
		void UnSetDisableOldStyle(void);
		BOOL GetDisableOldStyle(void);
		void UnSetBitmapBackground(void);
		BOOL AddBitmapToImageList(CImageList *list,UINT nResourceID);
		BOOL LoadFromToolBar(UINT nID,UINT nToolBar,int& xoffset);
		void InsertSpaces(void);
		static LRESULT FindKeyboardShortcut(UINT nChar,UINT nFlags,CMenu *pMenu);
		static void UpdateMenu(CMenu *pmenu);
		static BOOL IsMenu(CMenu *submenu);
		static BOOL IsMenu(HMENU submenu);
		void DrawCheckMark(CDC *pDC,int x,int y,COLORREF color);
		void DrawRadioDot(CDC *pDC,int x,int y,COLORREF color);
		CApexMenuData *CApexMenu::FindMenuItem(UINT nID);
		CApexMenu *FindMenuOption(int nId,int& nLoc);
		CApexMenuData *FindMenuOption(wchar_t *lpstrText);
		BOOL GetMenuText(UINT id,CString &string,UINT nFlags = MF_BYPOSITION );
		CImageList *checkmaps;
		BOOL checkmapsshare;
		int m_selectcheck;
		int m_unselectcheck;
		BOOL m_bDynIcons;
		
		void LoadCheckmarkBitmap(int unselect,int select);
		void DitherBlt(HDC hdcDest, int nXDest, int nYDest, int nWidth, 
			int nHeight, HBITMAP hbm, int nXSrc, int nYSrc);
		void DitherBlt2(CDC *drawdc, int nXDest, int nYDest, int nWidth, 
			int nHeight, CBitmap &bmp, int nXSrc, int nYSrc);
		static HBITMAP LoadSysColorBitmap(int nResourceId);
		BOOL GetBitmapFromImageList(CDC* pDC,CImageList *imglist,int nIndex,CBitmap &bmp);
		BOOL ImageListDuplicate(CImageList *il,int xoffset,CImageList *newlist);
		
		
		virtual ~CApexMenu();  // Virtual Destructor 
		// Drawing: 
		virtual void DrawItem( LPDRAWITEMSTRUCT);  // Draw an item
		virtual void MeasureItem( LPMEASUREITEMSTRUCT );  // Measure an item
		
		// Customizing:
		
		void SetIconSize (int, int);  // Set icon size
		
		BOOL AppendODMenuA(LPCSTR lpstrText, 
			UINT nFlags = MF_OWNERDRAW,
			UINT nID = 0,
			int nIconNormal = -1);  
		
		BOOL AppendODMenuW(wchar_t *lpstrText, 
			UINT nFlags = MF_OWNERDRAW,
			UINT nID = 0,
			int nIconNormal = -1);  
		
		BOOL AppendODMenuA(LPCSTR lpstrText, 
			UINT nFlags,
			UINT nID,
			CImageList *il,
			int xoffset);
		
		BOOL AppendODMenuW(wchar_t *lpstrText, 
			UINT nFlags,
			UINT nID,
			CImageList *il,
			int xoffset);
		
		BOOL InsertODMenuA(UINT nPosition,
			LPCSTR lpstrText, 
			UINT nFlags = MF_OWNERDRAW,
			UINT nID = 0,
			int nIconNormal = -1); 
		
		BOOL InsertODMenuW(UINT nPosition,
			wchar_t *lpstrText, 
			UINT nFlags = MF_OWNERDRAW,
			UINT nID = 0,
			int nIconNormal = -1);  
		
		BOOL InsertODMenuA(UINT nPosition,
			LPCSTR lpstrText, 
			UINT nFlags,
			UINT nID,
			CImageList *il,
			int xoffset);
		
		BOOL InsertODMenuW(UINT nPosition,
			wchar_t *lpstrText, 
			UINT nFlags,
			UINT nID,
			CImageList *il,
			int xoffset);
		
		
		BOOL ModifyODMenuA(const char *lpstrText,UINT nID=0,int nIconNormal=-1);
		BOOL ModifyODMenuA(const char *lpstrText,UINT nID,CImageList *il,int xoffset);
		BOOL ModifyODMenuA(const char *lpstrText,UINT nID,CBitmap *bmp);
		BOOL ModifyODMenuA(const char *lpstrText,const char *OptionText,int nIconNormal);
		BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID=0,int nIconNormal=-1);
		BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID,CImageList *il,int xoffset);
		BOOL ModifyODMenuW(wchar_t *lpstrText,UINT nID,CBitmap *bmp);
		BOOL ModifyODMenuW(wchar_t *lpstrText,wchar_t *OptionText,int nIconNormal);
		CApexMenuData *NewODMenu(UINT pos,UINT nFlags,UINT nID,CString string);
		void SynchronizeMenu(void);
		void CApexMenu::InitializeMenuList(int value);
		void CApexMenu::DeleteMenuList(void);
		CApexMenuData *CApexMenu::FindMenuList(UINT nID);
		virtual BOOL LoadMenu(LPCTSTR lpszResourceName);  // Load a menu
		virtual BOOL LoadMenu(int nResource);  // ... 
		void AddFromToolBar(CToolBar* pToolBar, int nResourceID);
		BOOL Draw3DCheckmark(CDC *dc, const CRect& rc,BOOL bSelected,
			int hbmCheck);
		BOOL LoadToolbar(UINT nToolBar);
		BOOL LoadToolbars(const UINT *arID,int n);
		
		// new overrides for dynamic menu's
		BOOL	RemoveMenu(UINT uiId,UINT nFlags);
		BOOL	DeleteMenu(UINT uiId,UINT nFlags);
		BOOL  AppendMenuA(UINT nFlags,UINT nIDNewItem=0,const char *lpszNewItem=NULL,int nIconNormal=-1);
		BOOL  AppendMenuA(UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CImageList *il,int xoffset);
		BOOL  AppendMenuA(UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CBitmap *bmp);
		BOOL  AppendMenuW(UINT nFlags,UINT nIDNewItem=0,wchar_t *lpszNewItem=NULL,int nIconNormal=-1);
		BOOL  AppendMenuW(UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CImageList *il,int xoffset);
		BOOL  AppendMenuW(UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CBitmap *bmp);
		BOOL  InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem=0,const char *lpszNewItem=NULL,int nIconNormal=-1);
		BOOL  InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CImageList *il,int xoffset);
		BOOL  InsertMenuA(UINT nPosition,UINT nFlags,UINT nIDNewItem,const char *lpszNewItem,CBitmap *bmp);
		BOOL  InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem=0,wchar_t *lpszNewItem=NULL,int nIconNormal=-1);
		BOOL  InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CImageList *il,int xoffset);
		BOOL  InsertMenuW(UINT nPosition,UINT nFlags,UINT nIDNewItem,wchar_t *lpszNewItem,CBitmap *bmp);
		CApexMenu* AppendODPopupMenuA(LPCSTR lpstrText);
		CApexMenu* AppendODPopupMenuW(wchar_t *lpstrText);
		
		// Destoying:
		
		virtual BOOL DestroyMenu();
		
		// Generated message map functions
protected:
	int m_iconX,m_iconY;
	COLORREF m_bitmapBackground;
	BOOL m_bitmapBackgroundFlag;
	BOOL disable_old_style;
	
	
	//---------new add 2000.04.19 xu-----------
public:
	//---- add for updatecommand-----------
	void EnableMenuItems(CMenu * pMenu, CWnd * pParent);
protected:
	//-------modify for not create toolbar-----
	BOOL LoadToolBarResource(unsigned int resId);
	void AddToolBarItem(const int idx,WORD cmd,const int resID);
	int FindToolBarItem(const int cmd,const int resID);
	int iToolBarItem;
	ToolBarItem * pToolBarItem;
	//------------------------------------------
	
}; 

#endif
