// Utility.h: all by eric/zdl/sexroute
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <mshtml.h>
#include <map>
#include <afxtempl.h>
using namespace std;
#if !defined(AFX_UTILITY_H__2BBBE9E3_09B5_41F5_9DFD_2F4D86CFD5B6__INCLUDED_)
#define AFX_UTILITY_H__2BBBE9E3_09B5_41F5_9DFD_2F4D86CFD5B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MAX_STR_SIZE 1024
#define FILE_NOT_EXIST 0XFFFFFFFF

#ifndef LWA_COLORKEY  
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#endif

#ifndef ABS_BOTH
#define ABS_BOTH 0x00000003
#endif

#ifndef MAX_LONG_STR_LENGTH_RADIX_10
#define MAX_LONG_STR_LENGTH_RADIX_10 11
#endif
struct DOWNLOAD_LINK_INFO
{
	CString mstrUrl;
	CString mstrTitle;
	DOWNLOAD_LINK_INFO()
	{
	}
	DOWNLOAD_LINK_INFO(CString &astrUrl, CString &astrTitle)
	{
		mstrUrl = astrUrl;
		mstrTitle = astrTitle;
	}
};

typedef struct _HttpProxyStruct
{
	CString m_strUrl;
	int		m_nPort;
	CString m_username;
	CString m_password;
	BOOL	m_bNeedAuth;

	_HttpProxyStruct()
	{
		this->m_nPort = 80;

		this->m_bNeedAuth = 0;
	}

} HttpProxyStruct,* lpHttpProxyStruct;

class CUtility  
{
public:
	static BOOL CopyFile(CString & lsSrc,CString & lsDst,BOOL lbOverWrite = TRUE);
	static BOOL WriteFileContent(CHAR * alpFileContent,int anFileContentLength,LPCTSTR alpctFilePath,BOOL bAppend = FALSE);
	static BOOL IsSafeFileType(CString& astrFullFileName);
	static LONGLONG GetFileSize(CString & lstrFileName);
	static LONGLONG GetDiskFreeSize(CString & strDir);
	static BOOL TranslateFileSize2String(LONGLONG ulFileSize,CString & strFileSize);
	static BOOL TranslateNetSize2String(LONGLONG ullFileSize, CString &strFileSize);
	static void DrawGradientRect(CDC* pDC, CRect& rcGradient, COLORREF clrBegin, COLORREF clrEnd,UINT nDirection = 0);
	static BOOL IsValidUrl(CString & strUrl);
	static BOOL GetClipBoardString(CString & strText);
	static HWND GetToolBarHWND();
	static BOOL ConvertCSTRING2LONG(CONST  CString & str,LONG & result);
	static BOOL TranslateTime2String(unsigned long ulTime, CString &strTimeFormated);
	static BOOL IsUtf8(char * b,int & nByteLength,int aMaxCount);
	static CString GetFileVersion(CString & astrFilename);
	static LONGLONG  CUtility::GetFileVersion2(CString & astrFilename);
	static HANDLE OpenFile(CString & strFullname,BOOL bPromptAlert);
	
	static CString CutStrByPixel(CDC *pDc, CString & strText,int pixelLimit);

	static BOOL TestConnection(CString& asUrl,CString asCompareValue = _T(""), lpHttpProxyStruct pProxy = NULL,BOOL abAsync = FALSE,HWND ahCallbackWindow = NULL,UINT anCallbackMessage = 0);
		
	enum ICON_SIZE
	{
		S16 = 16,
		S32 = 32,
		S48 = 48,
		S64 = 64,
		S256 = 256,
		S512 = 512
	};
	static CString GetAppPath();
	static VOID    GetINT64STR(__int64 i64,CString & str);
	static CString GetHostName(CString&  m_url);
	static CString GetFileName(CString& m_url);
	static BOOL    GetFileExtName(CString& strFileName,CString &strDest);
	static CString LoadString(UINT uID);
	static CString LoadString(LPCTSTR strSection,LPCTSTR key,UINT nResID);
	static BOOL    GetFileNameFromFullName(CString& strFileNameFullName,CString &strFileName);
	static BOOL    GetFilePathFromFullName(CString& strFileNameFullName,CString &strPath);
	static HRESULT GetCurrentWebDocument(IUnknown * pUnkSite,IHTMLDocument2 ** ppDoc);
	static HRESULT OpenDir(CString & strDir,CString & strFileNameToBeSelected,BOOL bCheckIsDir = TRUE);
	static BOOL	   DeleteFile(CString & FullFilePath);
	static BOOL	   ConvertBSTR2INT(CONST BSTR & src,INT & Result);
	static BOOL	   ConvertBSTR2LONG(CONST BSTR & src,LONG & result);
	static INT	   GetSplitStrArray(const CString& strData,const CString& strSplitFlag,CStringArray &array,BOOL SequenceAsOne = FALSE);
	static BOOL	   ConvertCSTRING2INT(CONST  CString & str,int & result);
	static HICON   LoadICON(UINT ID,ICON_SIZE size,HINSTANCE hInst = NULL);
	static void	   ParseKeyWords(const CString& strKeywords,const CString& strSplitFlag,const CString& strKeyValueSplit,map<int,CString>& mapKeywords);
	static BOOL	   SafeTtoI(const CString& src, int & nRet);
	static BOOL    SafeLtoT( CString& strDst,long & nSrc,long lDefault = 0);
	static CString ItoT(int i);
	static CString UrlDecode(LPCTSTR EncodedUrl);
	static CString UrlEncode(LPCTSTR lpUrl);
	//static CString Convert(CString   str,   int   sourceCodepage,   int   targetCodepage);  
	static BOOL    GetRegString(CString & path,CString & strValueName,CString & strValue);
	static BOOL	   IsFileExist(CString & strFilePath);
	static void	   MakeFileNameValid( CString& name );
	static INT	   GetOpenFileDlgStructSize();
	static BOOL	   SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);
	static void	   DrawTransBitMap(CDC *pDC, CRect *lpRect, UINT uResID,COLORREF colorToMask);
	static BOOL    SetWindowTransparent(HWND hwnd,BYTE bAlpha = 190,COLORREF cr = 255, DWORD dwFlags =LWA_ALPHA);
	static BOOL    SetMouseTrack(HWND hwnd,DWORD dwHoverTime = HOVER_DEFAULT,DWORD dwFlags=TME_LEAVE | TME_HOVER);
	//HWND hWnd: 窗口句柄 
	//COLORREF col: 透明化颜色 
	//BYTE bAlpha: ＝0：整个窗口透明, =255 完全不透明 
	//DWORD dwFlags: ＝1：仅颜色 col 透明， =2 :窗口按照bAlpha变量进行透明处理。 
	static BOOL   RegistAutoStart(BOOL bEnable,CString & astrTitle,CString & astrAppPath);
	static CString GetAppFullPath();
	static BOOL   IsFileShouldMonitor(CString& strFileExt);
	static BOOL   GetSiteCookie(CString & strUrl,CString & strCookie);
	static BOOL   GetUrlServer(CString & strUrl,CString & strServer);
	static void   ConvertBSTR2CString(BSTR & abstrSrc,CString & astrDst);
	static BOOL   Base64Decode(unsigned char *src,CString & strOut);
	static BOOL	  Base64Encode(unsigned char *src,int srclen,CString & strOut);
	static BOOL	  CrackFlashGetUrl(CString & strUrl);
	static CRect  GetWorkArea();
	static BOOL	  AddjustWindow(HWND hwnd,CRect & rc_wnd, CRect & border);
	static BOOL   AddjustWindow(HWND hwnd);
	static BOOL   AddjustWindow(CRect & rc_wnd);
	static BOOL   MakeSureFolderExist( const CString &asAbsoluteFilePath );
	static BOOL   GetUrlListFromHTMLFormat(CString &astrUrl, CString &astrHTML, CList<DOWNLOAD_LINK_INFO, DOWNLOAD_LINK_INFO&> &alstLinkInfo);
	static BOOL   CombineURL(LPCTSTR lpszBaseURL,CString& strUrl);
	static BOOL   GetUrlListFromDrop(COleDataObject* pDataObject, CList<DOWNLOAD_LINK_INFO, DOWNLOAD_LINK_INFO&> &alstLink, CString &astrRefer);

	static CString TransUTF8ToT(char * apTextFrom);
	static HRESULT SHAutoComplete(HWND hwndEdit,DWORD dwFlags);
	static BOOL	   GetFileContent(CString& asFileName,CString & asFileContent);
	static CString I64toCString(LONGLONG ai64);
private:
	static CString m_strAppPath;

	// url编码的解码工具
	
};



#endif // !defined(AFX_UTILITY_H__2BBBE9E3_09B5_41F5_9DFD_2F4D86CFD5B6__INCLUDED_)
