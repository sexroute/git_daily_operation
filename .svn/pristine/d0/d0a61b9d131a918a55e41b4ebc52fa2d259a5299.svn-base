// Utility.cpp: implementation of the CUtility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Utility.h"
#include <wininet.h>
#include <afxinet.h>
#include <shlwapi.h>
#include <atlutil.h>
#include <mshtml.h>
#include <vector>
#include <map>
#include <afxtempl.h>
const TCHAR MAX_ABOUT_BLANK[] = _T("about:blank");

#ifndef _ERIC_FILE_NOT_EXSIT
#define  _ERIC_FILE_NOT_EXSIT 0xFFFFFFFF
#endif

#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*
常用函数
*/

#pragma comment(lib,"version.lib")

CString CUtility::m_strAppPath = _T("");

typedef BOOL (WINAPI * lpfn)(HWND hWnd, COLORREF cr,BYTE bAlpha, DWORD dwFlags) ;



CString CUtility::GetAppPath()
{
	if (m_strAppPath != "")
		return m_strAppPath;

	::GetModuleFileName(NULL, m_strAppPath.GetBuffer(256), 256);

	m_strAppPath.ReleaseBuffer();

	int n = m_strAppPath.ReverseFind( _T('\\') );

	m_strAppPath = m_strAppPath.Left(n + 1);

	CString strTempPathName;

	::GetLongPathName(m_strAppPath,strTempPathName.GetBuffer(256), 256);

	strTempPathName.ReleaseBuffer();

	m_strAppPath = strTempPathName;

	return m_strAppPath;
}

CString CUtility::GetAppFullPath()
{
	CString lstrAppPath;

	::GetModuleFileName(NULL, lstrAppPath.GetBuffer(256), 256);

	lstrAppPath.ReleaseBuffer();

	return lstrAppPath;

}

VOID CUtility::GetINT64STR(__int64 i64,CString & str)
{
	char c64[30]={0};
	//memset(c64, 0, 30);
	_i64toa(i64, c64, 10);

	str.Format(_T("%s"), c64); 
}

CString CUtility::GetHostName(CString&  m_url)
{
	CString strServer, strObject;

	INTERNET_PORT wPort;

	DWORD dwType;

	if(AfxParseURL(LPCTSTR(m_url), dwType, strServer, strObject, wPort))
	{
		return strServer;

	}
	else 
		return "";
}
BOOL CUtility::GetFileExtName(CString& strFileName,CString &strDest)
{
	int iIndex  =0;
	BOOL result = FALSE;
	iIndex = strFileName.ReverseFind(_T('.'));
	if (iIndex >0)
	{
		TCHAR * BUFFERSTART = strFileName.GetBuffer(0)+iIndex+1;
		strDest = BUFFERSTART;
		strDest.MakeLower();
		result = TRUE;
	}
	return result;
}

BOOL CUtility::GetFileNameFromFullName(CString& strFileNameFullName,CString &strFileName)
{
	int iIndex  =0;
	BOOL result = TRUE;
	iIndex = strFileNameFullName.ReverseFind(_T('\\'));
	if (iIndex < 0)
	{
		iIndex = strFileNameFullName.ReverseFind(_T('/'));
	}
	if (iIndex >0)
	{
		TCHAR * BUFFERSTART = strFileNameFullName.GetBuffer(0)+iIndex+1;
		strFileName = BUFFERSTART;
	}else
	{
		strFileName = strFileNameFullName;
	}
	return result;
}

BOOL CUtility::GetFilePathFromFullName(CString& strFileNameFullName,CString &strPath)
{
	int iIndex  =0;

	BOOL result = TRUE;

	CString lstrTemp = strFileNameFullName;

	lstrTemp.Replace(_T('\\'),_T('/'));

	iIndex = lstrTemp.ReverseFind(_T('\\'));

	if (iIndex < 0)
	{
		iIndex = lstrTemp.ReverseFind(_T('/'));
	}
	if (iIndex >0)
	{

		TCHAR * temp = strFileNameFullName.GetBuffer(0);
		TCHAR _tempChar = temp[iIndex];
		temp[iIndex] = _T('\0');
		strPath = temp;
		temp[iIndex] = _tempChar;
		strFileNameFullName.ReleaseBuffer();

	}else
	{
		strPath = strFileNameFullName;
	}
	return result;
}

CString CUtility::GetFileName(CString& m_url)
{
	CString strServer, strObject;

	INTERNET_PORT wPort;

	DWORD dwType;

	AfxParseURL(LPCTSTR(m_url), dwType, strServer, strObject, wPort);

	if (strObject == "")
	{
		return "";
	}
	if (strServer == "")
	{
		return "";
	}

	int iIndex = strObject.ReverseFind(_T('/'));

	TRACE(_T("*********** strServer:%s  strObject:%s ***********\r\n"),strServer,strObject);


	if (iIndex >= 0)
	{
		iIndex = strObject.GetLength() - iIndex -1;

		strObject = strObject.Right(iIndex);
	}

	iIndex = strObject.ReverseFind(_T('.'));

	int strLength = strObject.GetLength();

	int iFileEndIndex = strLength;

	int iFileStartIndex = 0;	

	LPTSTR strFile = strObject.GetBuffer(strLength);	

	if (iIndex >= 0)
	{
		//向右找是否有问号,&,#,切到此
		for (int i = iIndex;i <strLength; i++)
		{
			if ((strFile[i]) == _T('?') ||(strFile[i]) == _T('=') || (strFile[i]) == _T('#') || (strFile[i]) == _T('&') || (strFile[i]) == _T('+'))
			{
				iFileEndIndex = i;

				break;
			}
		}

		//向左找，是否有问号,&,#,切到此

		for (int i = iIndex-1 ;i >=0; i--)
		{
			if ((strFile[i]) == _T('?')||(strFile[i]) == _T('=') || (strFile[i]) == _T('#') || (strFile[i]) == _T('&') || (strFile[i]) == _T('+'))
			{
				iFileStartIndex = i+1;

				break;
			}
		}	
	}

	TCHAR temp = strFile[iFileEndIndex-1];

	LPTSTR tempLP = strFile;

	strFile[iFileEndIndex] = _T('\0');

	strFile = strFile + iFileStartIndex;

	CString strTempForDebug  = strFile;

	tempLP[iFileEndIndex] = temp;

	strTempForDebug = CUtility::UrlDecode(strTempForDebug);

	strTempForDebug.TrimRight(_T("\r\n"));

	strTempForDebug.TrimRight(_T("\r"));

	strTempForDebug.TrimRight(_T("\n"));

	return strTempForDebug;
}

CString CUtility::LoadString(UINT uID)
{


	HINSTANCE hInstance = NULL;

	hInstance = GetModuleHandle(NULL);

	CString strTemp=_T("");

	::LoadString(hInstance,uID,strTemp.GetBuffer(MAX_STR_SIZE),MAX_STR_SIZE);

	strTemp.ReleaseBuffer();

	return strTemp;

}



HRESULT CUtility::GetCurrentWebDocument(IUnknown * pUnkSite,IHTMLDocument2 ** ppDoc)
{
	HRESULT hr = S_FALSE;

	if (pUnkSite == NULL)
	{
		return hr;
	}

	ASSERT(	pUnkSite!=NULL);

	CComQIPtr<IServiceProvider> pIsp;

	CComPtr<IWebBrowser2> pIWeb;

	CComPtr<IDispatch> pDisp;

	pIsp = pUnkSite;
	try
	{
		hr = pIsp->QueryService(SID_SWebBrowserApp,__uuidof(IWebBrowser2),(void **)&pIWeb);

		if (pIWeb != NULL)
		{ 
			hr = pIWeb->get_Document((IDispatch **)&pDisp);

			if (hr == S_OK && pDisp != NULL)
			{
				hr = pDisp->QueryInterface(__uuidof(IHTMLDocument2),(void **)ppDoc);	
			}			

		}
	}
	catch (...)
	{

		TRACE(_T("********GetCurrenDoc failed! %s %d***********\r\n"),__FILE__,__LINE__);

	}	

	return hr;
}



BOOL CUtility::DeleteFile(CString & FullFilePath)
{
	BOOL result = ::DeleteFile(FullFilePath);

	return result;
}

BOOL CUtility::ConvertCSTRING2INT(CONST  CString & str,int & result)
{
	BOOL bResult = FALSE;

	char * szBuffer = NULL;

	try
	{
		LPCTSTR tempBuffer = NULL;

		tempBuffer = (LPCTSTR)str;

		result = _ttoi(tempBuffer);

		bResult = TRUE;
	}
	catch (...)
	{
		bResult = FALSE;	
	}

	return bResult;
}

BOOL CUtility::ConvertCSTRING2LONG(CONST  CString & str,LONG & result)
{
	BOOL bResult = FALSE;

	try
	{
		LPCTSTR tempBuffer = NULL;

		tempBuffer = (LPCTSTR)str;

		result = _ttol(tempBuffer);

		bResult = TRUE;
	}
	catch (...)
	{
		bResult = FALSE;	
	}

	return bResult;
}
BOOL CUtility::ConvertBSTR2INT(CONST BSTR & src,int & result)
{
	BOOL bResult = FALSE;

	if (src == NULL)
	{
		return FALSE;
	}

	CString strBuffer ;

	try
	{
		strBuffer = src;

		result = _ttoi(strBuffer);

		bResult = TRUE;
	}
	catch (...)
	{
		bResult = FALSE;	
	}



	return bResult;
}

BOOL CUtility::ConvertBSTR2LONG(CONST BSTR & src,LONG & result)
{
	BOOL bResult = FALSE;

	if (src == NULL)
	{
		return FALSE;
	}

	CString strBuffer ;

	try
	{
		strBuffer = src;

		result = _ttol(strBuffer);

		bResult = TRUE;
	}
	catch (...)
	{
		bResult = FALSE;	
	}


	return bResult;
}

INT CUtility::GetSplitStrArray(const CString& strData,const CString& strSplitFlag,CStringArray &array,BOOL SequenceAsOne)
{
	CString sData = strData;

	CString sSplitFlag = strSplitFlag;

	int lnSplitFlagLength = strSplitFlag.GetLength();

	if (sData.Right(1) != sSplitFlag) sData +=sSplitFlag;

	CString sTemp;

	int pos =-1;

	while ((pos=sData.Find(sSplitFlag,0)) != -1)
	{
		sTemp = sData.Left(pos);

		if (!SequenceAsOne)
		{
			array.Add(sTemp);
		}
		else
		{
			if (!sTemp.IsEmpty() && sTemp !="") ////连续的分隔符视为单个处理
			{
				array.Add(sTemp);
			}
		}
		sData = sData.Right(sData.GetLength() - pos - lnSplitFlagLength);
	}

	return array.GetSize();
}

HICON   CUtility::LoadICON(UINT ID,ICON_SIZE size,HINSTANCE hInst)
{	
	HINSTANCE hTempInstance = hInst;

	if (hTempInstance == NULL)
	{
		hTempInstance = AfxGetApp()->m_hInstance;
	}

	HICON hicon= (HICON)LoadImage( hTempInstance, MAKEINTRESOURCE(ID),IMAGE_ICON,size,size,LR_DEFAULTCOLOR);


	return hicon;
}



HWND CUtility::GetToolBarHWND()
{

	HWND temp = ::FindWindowEx(NULL,NULL,_T("Shell_TrayWnd"),NULL);

	return temp;
}

void CUtility::ConvertBSTR2CString(BSTR & abstrSrc,CString & astrDst)
{
	// 	CComBSTR lobstr(abstrSrc);
	// 
	// 	for (int i = 0 ;i < lobstr.Length();i++)
	// 	{
	// 		int nTempLow = i*2+ abstrSrc;
	// 
	// 		int nTempHigh = nTempLow +1;
	// 	}

}
BOOL CUtility::GetClipBoardString(CString &strText)
{
	BOOL result = FALSE;

	LPTSTR   pszClipboard = NULL;

	HANDLE   hClipData = NULL;

	BOOL     fRet = FALSE;

	fRet = OpenClipboard(NULL);

	if (!fRet) 
	{
		//打开剪贴板失败
		return FALSE;
	}

	if (!(hClipData = GetClipboardData(CF_TEXT)))//Get hClipData
	{
		//剪贴板为空
		CloseClipboard();
		return FALSE;
	}

	if (!(pszClipboard = (LPTSTR)GlobalLock(hClipData)))//Get pszClipboard
	{
		CloseClipboard();
		return FALSE;
	}	

	int iDataLength = _tcslen(pszClipboard);

	if (iDataLength > 1024)
	{
		strText = "";
	}
	else
	{
		TCHAR temp = pszClipboard[iDataLength];
		pszClipboard[iDataLength] = _T('\0');
		strText = pszClipboard;
		pszClipboard[iDataLength] = temp;		
	}

	GlobalUnlock(hClipData);

	CloseClipboard();

	strText.TrimRight(_T(" "));

	strText.TrimRight(_T("\r\n"));

	strText.TrimRight(_T("\r"));

	strText.TrimRight(_T("\n"));

	result = TRUE;

	return result;
}

BOOL CUtility::IsValidUrl(CString & strUrl)
{
	if (strUrl.Find(_T("thunder://")) == 0)
	{
		return TRUE;
	}


	CString strFlashGetTest = strUrl;

	strFlashGetTest.MakeLower();

	if (strFlashGetTest.Find(_T("flashget://")) == 0)
	{
		if (CUtility::CrackFlashGetUrl(strUrl))
		{
			return TRUE;
		}else
		{
			return FALSE;
		}
	}

	CString strServer, strObject;

	INTERNET_PORT wPort;

	DWORD dwType;

	if(AfxParseURL(LPCTSTR(strUrl), dwType, strServer, strObject, wPort))
	{
		return TRUE;		
	}
	else 
		return FALSE;
}
INT CUtility::GetOpenFileDlgStructSize()
{
	int nStructSize = 0;

	DWORD dwVersion, dwWindowsMajorVersion, dwWindwosMinorVersion;

	dwVersion = GetVersion();

	dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));

	dwWindwosMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	if(dwVersion < 0x80000000)
	{
		nStructSize = 88;   //2k,xp下显示新的版本
	}
	else
	{
		nStructSize = 76;  //98下显示老版本
	}

	return nStructSize;
}

void CUtility::ParseKeyWords(const CString& strKeywords,const CString& strSplitFlag,const CString& strKeyValueSplit,map<int,CString>& mapKeywords)
{
	CStringArray keywordsArray;

	CUtility::GetSplitStrArray(strKeywords,strSplitFlag,keywordsArray);

	int nKeyWordSize = keywordsArray.GetSize();

	CString temp;

	for (int i =0 ;i<nKeyWordSize;i++)
	{
		temp = keywordsArray.ElementAt(i);

		CStringArray tempArray;

		CUtility::GetSplitStrArray(temp,strKeyValueSplit,tempArray);

		if (tempArray.GetSize() > 1)
		{
			int nKey = 0;
			try
			{
				nKey = _ttoi(tempArray.ElementAt(0));
			}
			catch (...)
			{
				continue;
			}

			mapKeywords[nKey] = tempArray.ElementAt(1);
		}

	}
}

BOOL CUtility::SafeTtoI(const CString& src, int & nRet)
{
	BOOL result = FALSE;

	try
	{
		int iTemp = _ttoi(src);

		result = TRUE;

		nRet = iTemp;
	}
	catch (...)
	{
		result = FALSE;	
	}

	return result;

}

CString CUtility::ItoT(int i)
{
	CString lstrTemp;

	lstrTemp.Format((_T("%d")),i);

	return lstrTemp;
}

BOOL CUtility::SafeLtoT( CString& strDst,long & nSrc,long lDefault/* = 0*/)
{
	BOOL result = FALSE;

	try
	{
		LPTSTR lsztBuffer = strDst.GetBufferSetLength(MAX_LONG_STR_LENGTH_RADIX_10);

		_ltot(nSrc,lsztBuffer,10);

		strDst.ReleaseBuffer(MAX_LONG_STR_LENGTH_RADIX_10);

	}
	catch (...)
	{
		strDst.Format(_T("%d"),lDefault);

		result = FALSE;	
	}

	return result;
}
void CUtility::DrawGradientRect(CDC *pDC, CRect &rcGradient, COLORREF clrBegin, COLORREF clrEnd,UINT nDirection)
{


	COLORREF clrCptn = clrBegin;

	HBRUSH brCptn = ::GetSysColorBrush(clrBegin);

	int nShift = 5;

	int nSteps = 1 << nShift;

	for (int i = 0; i < nSteps; i++)
	{

		BYTE bR = (BYTE) ((GetRValue(clrCptn) * (nSteps - i) +	GetRValue(clrEnd) * i) >> nShift);

		BYTE bG = (BYTE) ((GetGValue(clrCptn) * (nSteps - i) + 	GetGValue(clrEnd) * i) >> nShift);

		BYTE bB = (BYTE) ((GetBValue(clrCptn) * (nSteps - i) + 	GetBValue(clrEnd) * i) >> nShift);

		CBrush br (RGB(bR, bG, bB));

		CRect r2 = rcGradient;

		if (nDirection > 0 )
		{

			r2.left = rcGradient.left + ((i * rcGradient.Width()) >> nShift);

			r2.right = rcGradient.left +(((i + 1) * rcGradient.Width()) >> nShift);

		}else
		{
			r2.top = rcGradient.top + ((i * rcGradient.Height()) >> nShift);

			r2.bottom = rcGradient.top + (((i + 1) * rcGradient.Height()) >> nShift);
		}

		//TRACE(_T("Bottom: %d top: %d \r\n"),r2.bottom,r2.top);

		if (r2.Width() > 0)
		{

			pDC->FillRect(r2, &br);
		}
	}

	if (brCptn)
	{
		DeleteObject(brCptn);
	}	
}

// url编码的解码工具
CString CUtility::UrlEncode(LPCTSTR lpUrl)
{
	USES_CONVERSION;

	int   nLen; 

	LPWSTR lpBuf = NULL;

	BOOL isNotUrlEncoded = TRUE;	

	int lnlength = _tcsclen(lpUrl);

#ifdef _UNICODE
	lpBuf = (lpUrl);
#else
	lpBuf = A2W(lpUrl);
#endif // _UNICODE

	WCHAR lpOut[102400]={0};

	DWORD lnwlength = wcslen(lpBuf);

	AtlEscapeUrl(lpBuf,lpOut,&lnwlength,102400,ATL_URL_ENCODE_PERCENT);

	CString lstrTemp = CW2T(lpOut);

	return lstrTemp;
}

// url编码的解码工具
CString CUtility::UrlDecode(LPCTSTR   lpUrl)
{	
	int   nLen; 

	LPCSTR lpBuf = NULL;

	BOOL isNotUrlEncoded = TRUE;	

	int lnlength = _tcsclen(lpUrl);

	if (lnlength > INTERNET_MAX_URL_LENGTH)
	{
		TRACE(_T("********* not validate url str :len > %d*********\r\n"),INTERNET_MAX_URL_LENGTH);
		return lpUrl; 
	}

	//判断是否不是url编码的字符串
	for (int i =0;i<lnlength ; i++)
	{
		if (lpUrl[i] == _T('%'))
		{
			isNotUrlEncoded = FALSE;

			break;
		}
	}
	USES_CONVERSION;

#ifdef _UNICODE

	lpBuf = W2A(lpUrl);

#else

	lpBuf = lpUrl;

#endif
	if (isNotUrlEncoded == TRUE) //不含%肯定没有url编码
	{
		int llnlength = strlen(lpBuf);

		if (CUtility::IsUtf8((char *)lpBuf,llnlength,llnlength))
		{
			TRACE(_T("is utf8\r\n"));
		}
		//return lpUrl;
	}


	if(!lpBuf) nLen=0;

	else nLen=::strlen(lpBuf);   

	CHAR s[INTERNET_MAX_URL_LENGTH]={0}; 

	i=0;

	LPCTSTR result = NULL;

	int nIndex = 0;	

	while(i<nLen)   
	{   
		CHAR x = lpBuf[i];

		if(lpBuf[i]=='%')   
		{   
			BYTE   c1=lpBuf[i+1];   
			BYTE   c2=lpBuf[i+2];   
			i+=2;   
			if(	   c1>='0'      &&   c1<='9') c1=(c1-'0')*16;   
			else   if(c1>='A'   &&   c1<='Z') c1=(c1-'A'+10)*16;   
			else   if(c1>='a'   &&   c1<='a') c1=(c1-'a'+10)*16;   
			if(    c2>='0'      &&   c2<='9') c2=c2-'0';   
			else   if(c2>='A'   &&   c2<='Z') c2=c2-'A'+10;   
			else   if(c2>='a'   &&   c2<='z') c2=c2-'a'+10;			
			char   szStr[2]; szStr[0]=c1+c2; szStr[1]=0;

			memcpy(s+nIndex,szStr,sizeof(char));

			nIndex = nIndex + sizeof(char);				

		}   
		else  if(lpBuf[i]=='+') 
		{
			memcpy(s+nIndex,"   ",sizeof(char)*3);

			nIndex = nIndex + sizeof(char)*3;			
		}
		else
		{
			memcpy(s+nIndex,&lpBuf[i],sizeof(char));

			nIndex = nIndex + sizeof(char);

		}
		i++;   
	} 

	BOOL bIsUTF8 = IsUtf8((char *)s,nLen,nLen); //判断url解码后的是否是utf-8编码

	DWORD dwCodePage = CP_UTF8;

	if (!bIsUTF8)
	{
		dwCodePage = CP_ACP;

		BOOL lbIsCA_ACP_ENCODED = MultiByteToWideChar(dwCodePage,MB_ERR_INVALID_CHARS,s,-1,NULL,0);

		if (lbIsCA_ACP_ENCODED == FALSE && GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
		{
			return lpUrl;
		}
	}

	//将utf-8 转为 unicode		

	int unicodeLen = MultiByteToWideChar(dwCodePage,0,s,-1,NULL,0);

	WCHAR *  pUnicode = new  WCHAR[unicodeLen];

	MultiByteToWideChar(dwCodePage,0,s,-1,(LPWSTR)pUnicode,unicodeLen);

#ifdef _UNICODE
	result = 	pUnicode;	
#else	
	//将 unicode 转为 mbcs	
	result = W2A(pUnicode);
#endif 

	CString strDebug(result);

	delete pUnicode;	

	return   strDebug;   

}


BOOL CUtility::TranslateTime2String(unsigned long ulTime, CString &strTimeFormated)
{ 
	BOOL bRet = TRUE;

	if (ulTime < 0)
	{
		strTimeFormated = _T("00:00:00");

		return bRet;
	}
	unsigned long ulHour = ulTime /3600;

	if (ulHour<10)
	{
		strTimeFormated.Format(_T("0%u:"),ulHour);
	}
	else
	{
		strTimeFormated.Format(_T("%u:"),ulHour);
	}

	unsigned long ulMinute = (ulTime % 3600) / 60;
	CString strTemp;
	strTemp = strTimeFormated;
	if (ulMinute < 10)
	{

		strTimeFormated.Format(_T("%s0%u:"),strTemp,ulMinute);
	}
	else
	{

		strTimeFormated.Format(_T("%s%u:"),strTemp,ulMinute);
	}

	strTemp = strTimeFormated;

	unsigned long ulSecond = ulTime - ulHour*3600 - ulMinute*60;

	if (ulSecond < 10)
	{
		strTimeFormated.Format(_T("%s0%u"),strTemp,ulSecond);
	}
	else
	{
		strTimeFormated.Format(_T("%s%u"),strTemp,ulSecond);
	}
	return bRet; 
}

BOOL CUtility::TranslateFileSize2String(LONGLONG ullFileSize, CString &strFileSize)
{
	BOOL bRet = TRUE;

	DOUBLE dblFileSize = 0;

	if (ullFileSize > 1024*1024*1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024*1024*1024) ; 

		strFileSize.Format(_T("%.2fGB"),dblFileSize);

	}
	else if (ullFileSize > 1024*1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024*1024) ; 

		strFileSize.Format(_T("%.2fMB"),dblFileSize);

	}else if (ullFileSize > 1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024) ; 
		strFileSize.Format(_T("%.2fKB"),dblFileSize);

	}else
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024) ; 
		strFileSize.Format(_T("%.2fBytes"),dblFileSize);

	}

	return bRet;			
}
BOOL CUtility::TranslateNetSize2String(LONGLONG ullFileSize, CString &strFileSize)
{
	BOOL bRet = TRUE;

	DOUBLE dblFileSize = 0;

	if (ullFileSize > 1024*1024*1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024*1024*1024) ; 

		if (dblFileSize >=  100)
		{
			strFileSize.Format(_T("%3.0fGB"),dblFileSize);

		}else if (dblFileSize >= 10)
		{
			strFileSize.Format(_T("%2.1fGB"),dblFileSize);

		}else
		{
			strFileSize.Format(_T("%1.2fGB"),dblFileSize);
		}

	}
	else if (ullFileSize > 1024*1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024*1024) ; 

		//strFileSize.Format(_T("%3.2fMB"),dblFileSize);
		if (dblFileSize >=  100)
		{
			strFileSize.Format(_T("%3.0fMB"),dblFileSize);

		}else if (dblFileSize >= 10)
		{
			strFileSize.Format(_T("%2.1fMB"),dblFileSize);

		}else
		{
			strFileSize.Format(_T("%1.2fMB"),dblFileSize);
		}

	}else if (ullFileSize > 1024)
	{
		dblFileSize = ((DOUBLE)ullFileSize) /(1024) ;

		if (dblFileSize >=  100)
		{
			strFileSize.Format(_T("%3.0fKB"),dblFileSize);

		}else if (dblFileSize >= 10)
		{
			strFileSize.Format(_T("%2.1fKB"),dblFileSize);

		}else
		{
			strFileSize.Format(_T("%1.2fKB"),dblFileSize);
		}


	}else
	{	

		if (dblFileSize >=  100)
		{
			strFileSize.Format(_T("%3.0fB"),dblFileSize);

		}else if (dblFileSize >= 10)
		{
			strFileSize.Format(_T("%2.1fB"),dblFileSize);

		}else
		{
			strFileSize.Format(_T("%1.2fB"),dblFileSize);
		}

	}

	return bRet;			
}
LONGLONG CUtility::GetDiskFreeSize(CString &strDir)
{

	LARGE_INTEGER FreeBytesAvailableToCaller = {0};

	LARGE_INTEGER TotalNumberOfBytes = {0};    // receives the number of bytes on disk

	LARGE_INTEGER TotalNumberOfFreeBytes = {0}; // receives the free bytes on disk	

	int nIndex  = strDir.Find(_T(":"),0);

	if (nIndex >0)
	{
		CString strTemp = strDir.Left(nIndex+1);

		GetDiskFreeSpaceEx(strTemp,(ULARGE_INTEGER *)&FreeBytesAvailableToCaller,(ULARGE_INTEGER *)&TotalNumberOfBytes,(ULARGE_INTEGER *)&TotalNumberOfFreeBytes);
	}		

	return TotalNumberOfFreeBytes.QuadPart;
}





BOOL CUtility::IsSafeFileType(CString &astrFullFileName)
{
	CString lstrFileExt;

	CUtility::GetFileExtName(astrFullFileName,lstrFileExt);

	if (lstrFileExt.CompareNoCase(_T("mp3")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("torrent")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("mp4")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("avi")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("rmvb")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("dvi")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("flv")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("asf")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("mpeg")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("wmv")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("wma")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("rm")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("bmp")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("jpeg")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("jpg")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("png")) == 0)
	{
		return TRUE;
	}


	if (lstrFileExt.CompareNoCase(_T("gif")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("pdf")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("tiff")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("rar")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("zip")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("gzip")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("txt")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("gz")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("cpp")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("c")) == 0)
	{
		return TRUE;
	}

	if (lstrFileExt.CompareNoCase(_T("h")) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CUtility::IsUtf8(char * b,int & nByteLength,int aMaxCount){

	int lLen=nByteLength,lCharCount=0;

	for(int i=0;i<lLen && lCharCount<aMaxCount;++lCharCount){

		char lByte=b[i++];

		if(lByte>=0) continue;//>=0 is normal ascii

		if(lByte<(char)0xc0 || lByte>(char)0xfd) return FALSE;

		int lCount=lByte>(byte)0xfc?5:lByte>(char)0xf8?4

			:lByte>(char)0xf0?3:lByte>(char)0xe0?2:1;

		if(i+lCount>lLen) return FALSE; //不一定正确，因为有可能是不完整的二进制流,所以总长未必大于声明长度

		for(int j=0;j<lCount;++j,++i) if(b[i]>=(char)0xc0) return FALSE;		
	}

	return true;	
}


BOOL CUtility::GetRegString(CString & path,CString & strValueName,CString & strValue)
{
	BOOL bRet = FALSE;

	HKEY hKey; 

	DWORD nPerIncrementLength = 512;

	DWORD nInitLength = nPerIncrementLength;

	DWORD dwType = REG_SZ;

	BOOL bNeedDelete = TRUE;

	LONG ret0 = (::RegOpenKeyEx(HKEY_CURRENT_USER,path,0,KEY_READ,&hKey));

	if (ret0 != ERROR_SUCCESS )
	{
		return FALSE;
	}

	LPSTR lpData = (LPSTR)(_alloca(nInitLength*sizeof(CHAR)));

	if (lpData == NULL)
	{
		lpData = (LPSTR)(malloc(nInitLength*sizeof(CHAR)));

		bNeedDelete = TRUE;

		if (lpData == NULL)
		{
			return FALSE;
		}
	}

	ret0 = RegQueryValueEx(hKey,strValueName,NULL,&dwType,(LPBYTE)lpData,&nInitLength);

	while (ret0 == ERROR_MORE_DATA)
	{

		nInitLength = nInitLength + nPerIncrementLength;

		if (bNeedDelete)
		{
			delete lpData;

			lpData = NULL;
		}

		lpData = (LPSTR)(_alloca(nInitLength*sizeof(CHAR)));;

		if (lpData == NULL)
		{
			lpData = (LPSTR)(malloc(nInitLength*sizeof(CHAR)));

			bNeedDelete = TRUE;

			if (lpData == NULL)
			{
				return FALSE;
			}
		}

		ret0 = RegQueryValueEx(hKey,strValueName,NULL,&dwType,(LPBYTE)lpData,&nInitLength);

	}

	if (ret0 != ERROR_SUCCESS)
	{
		bRet = FALSE;

	}else
	{
#ifdef _UNICODE

		USES_CONVERSION;

		strValue = A2W(lpData);
#else	

		strValue = lpData;

#endif

		bRet = TRUE;
	}
	return bRet;
}

BOOL CUtility::IsFileExist(CString & strFilePath)
{
	if (GetFileAttributes(strFilePath) == 0xFFFFFFFF) 
	{
		return FALSE;

	}else
	{
		return TRUE;
	}
}

void CUtility::MakeFileNameValid( CString& name )
{
	name.Replace(_T(':'),_T('_'));
	name.Replace(_T('?'),_T('_'));
	name.Replace(_T('*'),_T('_'));
	name.Replace(_T('/'),_T('_'));
	name.Replace(_T('\\'),_T('_'));
	name.Replace(_T('|'),_T('_'));
	name.Replace(_T('<'),_T('_'));
	name.Replace(_T('>'),_T('_'));
	name.Replace(_T('\"'),_T('_'));	
	name.Replace(_T('%'),_T('_'));
	name.Replace(_T('!'),_T('_'));
	name.Replace(_T('='),_T('_'));
} 

//将资源中的一个图片，按照指定的颜色，做mask后，绘制到DC上

void CUtility::DrawTransBitMap(CDC *pDC, CRect *lpRect, UINT uResID,COLORREF colorToMask)
{
	CDC loMemDC;

	CDC loMemMaskDC;	

	CDC loBufferDC ;

	CBitmap *lpOldBitmap;

	CBitmap *lpOldBitmapMask;

	CBitmap loMaskBitmap ;

	CBitmap loBitmapSkinBackground;

	if (!loBitmapSkinBackground.LoadBitmap(uResID))
	{
		return ;
	}	

	loMemDC.CreateCompatibleDC(pDC);

	loMemMaskDC.CreateCompatibleDC(pDC);

	loBufferDC.CreateCompatibleDC( pDC ) ;

	CBitmap loBufferBitmap ;

	loBufferBitmap.CreateCompatibleBitmap(pDC,lpRect->Width(), lpRect->Height() ) ;

	loMaskBitmap.CreateBitmap( lpRect->Width(), lpRect->Height(), 1, 1, NULL ) ;

	CBitmap* lpOldBufferBitmap = loBufferDC.SelectObject( &loBufferBitmap ) ;

	loBufferDC.FillSolidRect( lpRect, colorToMask) ;

	lpOldBitmap=loMemDC.SelectObject(&loBitmapSkinBackground);

	lpOldBitmapMask = loMemMaskDC.SelectObject(&loMaskBitmap);

	loBufferDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), &loMemDC,0, 0, SRCCOPY ) ;

	COLORREF loColorOldBKG = loBufferDC.SetBkColor( colorToMask ) ;

	loMemMaskDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), &loBufferDC,0, 0, SRCCOPY ) ;

	loBufferDC.SetBkColor( loColorOldBKG ) ;

	CDC loCopyDC ;

	loCopyDC.CreateCompatibleDC( pDC ) ;

	CBitmap loBitmapCopyBitmap ;

	loBitmapCopyBitmap.CreateCompatibleBitmap(pDC, lpRect->Width(), lpRect->Height() ) ;

	CBitmap* l_pOldCopyBitmap = loCopyDC.SelectObject( &loBitmapCopyBitmap ) ;

	loCopyDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), pDC,lpRect->left, lpRect->top, SRCCOPY ) ;

	loCopyDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), &loMemMaskDC,0, 0, SRCAND ) ;

	loBufferDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), &loMemMaskDC,0, 0, SRCINVERT ) ;

	loCopyDC.BitBlt( 0, 0, lpRect->Width(), lpRect->Height(), &loBufferDC,0, 0, SRCPAINT ) ;

	pDC->BitBlt( lpRect->left, lpRect->top, lpRect->Width(), lpRect->Height(), &loCopyDC,0, 0, SRCCOPY ) ;

	loCopyDC.SelectObject( l_pOldCopyBitmap ) ;

	loMemDC.SelectObject(lpOldBitmap);

	loMemMaskDC.SelectObject(lpOldBitmapMask);

	loBufferDC.SelectObject(lpOldBufferBitmap);
}



BOOL CUtility::SetWindowTransparent(HWND hwnd,BYTE bAlpha,COLORREF cr, DWORD dwFlags)
{
	//return FALSE;

	if (hwnd == NULL)
	{
		return FALSE;
	}

	BOOL bRet =	::SetWindowLong(hwnd, GWL_EXSTYLE,GetWindowLong(hwnd,GWL_EXSTYLE) | WS_EX_LAYERED);

	if (!bRet)
	{
		return bRet;
	}

/*
	bRet = CUtility::SetLayeredWindowAttributes(hwnd,cr,bAlpha,dwFlags);

	if (!bRet)
	{
		return bRet;
	}*/

	// bRet = ::RedrawWindow(hwnd, NULL, NULL,RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);

	return bRet;
}

BOOL CUtility::SetMouseTrack(HWND hwnd,DWORD dwHoverTime/*HOVER_DEFAULT*/,DWORD dwFlags/*TME_LEAVE | TME_HOVER*/)
{
	if (hwnd == NULL)
	{
		return FALSE;
	}
	TRACKMOUSEEVENT t_MouseEvent;
	t_MouseEvent.cbSize      = sizeof(TRACKMOUSEEVENT);
	t_MouseEvent.dwFlags     = dwFlags;
	t_MouseEvent.hwndTrack   = hwnd;
	t_MouseEvent.dwHoverTime = dwHoverTime;

	return ::_TrackMouseEvent(&t_MouseEvent);
}

BOOL CUtility::RegistAutoStart(BOOL bEnable,CString & astrTitle,CString & astrAppPath)
{
	HKEY hKey; 

	LPCTSTR data_Set= _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//

	LONG ret0 = (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_WRITE| KEY_CREATE_SUB_KEY,&hKey));//

	LPCTSTR lpb = astrAppPath.GetBuffer(0);

	int cbData = astrAppPath.GetLength();

	long bRet =  ERROR_SUCCESS;

	if (bEnable)
	{
		bRet =(::RegSetValueEx(hKey,astrTitle,NULL,REG_SZ,(const unsigned char *)lpb,cbData));//

	}else
	{
		bRet = ::RegDeleteValue(hKey,astrTitle);
	}

	if (bRet != ERROR_SUCCESS)
	{
		return FALSE;
	}else
	{
		return TRUE;
	}

}


BOOL CUtility::GetUrlServer(CString & strUrl,CString & strServer)
{
	CString strObject;

	INTERNET_PORT wPort;

	DWORD dwType;

	if(AfxParseURL(LPCTSTR(strUrl), dwType, strServer, strObject, wPort))
	{
		int lnProtocolIndex = strUrl.Find(_T("//"));

		if (lnProtocolIndex != -1)
		{
			CString strTemp = strUrl.Left(lnProtocolIndex +2);

			strServer = strTemp + strServer;
		}		

		return TRUE;		
	}
	else 
	{

		return FALSE;
	}
}


char*  ch64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

BOOL CUtility::Base64Encode(unsigned char *src,int srclen,CString & strOut)
{
	int n,buflen,i,j;
	int pading=0;
	unsigned char *buf;
	static unsigned char *dst; 

	buf=src;
	buflen=n=srclen;
	if(n%3!=0)  /* pad with '=' by using a temp buffer */
	{
		pading=3-n%3;
		buflen=n+pading;
		buf=(unsigned char *)malloc(buflen+1);
		memset(buf,0,buflen+1);
		memcpy(buf,src,n);
		//  for(i=0;i<pading;i++) no need
		//  buf[n+i]=0;
	}
	dst=(unsigned char *)malloc(buflen*4/3+1);
	memset(dst,0,buflen*4/3+1);
	for(i=0,j=0;i<buflen;i+=3,j+=4)
	{
		dst[j]=(buf[i]&0xFC)>>2;
		dst[j+1]=((buf[i]&0x03)<<4) + ((buf[i+1]&0xF0)>>4);
		dst[j+2]=((buf[i+1]&0x0F)<<2) + ((buf[i+2]&0xC0)>>6);
		dst[j+3]=buf[i+2]&0x3F; 
	}
	for(i=0;i<buflen*4/3;i++) /* map 6 bit value to base64 ASCII character */
		dst[i]=ch64[dst[i]];
	if(pading)
	{
		free(buf);
		if(pading==1)
		{//8 bits padded. 1 =
			dst[buflen*4/3-1]='=';
		}
		else
			if(pading==2)
			{//16 bits padded 2 =
				dst[buflen*4/3-2]='=';
				dst[buflen*4/3-1]='=';

			}
	}

	strOut = dst;

	free(dst);

	return TRUE;
}

BOOL CUtility::Base64Decode(unsigned char *src,CString & strOut)
{
	int n,i,j;

	unsigned char *p;

	static unsigned char *dst; 

	n=strlen((const char *)src);

	if(src[n-1]=='=')n--;

	if(src[n-2]=='=')n--;

	for(i=0;i<n;i++) /* map base64 ASCII character to 6 bit value */
	{
		p=(unsigned char *)strchr(ch64,src[i]);

		if(!p)
		{
			break;
		}

		src[i]=(char *)p-ch64;
	}

	int lnLength = n*3/4+1;

	if (lnLength <= 0)
	{
		return FALSE;
	}

	dst=(unsigned char *)malloc(lnLength);

	memset(dst,0,n*3/4+1);

	for(i=0,j=0;i<n;i+=4,j+=3)
	{
		dst[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);
		dst[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);
		dst[j+2]=((src[i+2]&0x03)<<6) + src[i+3];
	}

	USES_CONVERSION;

	strOut = CString(A2T((char *)dst));

	free(dst);

	return TRUE;
}


BOOL CUtility::CrackFlashGetUrl(CString & strUrl)
{
	CString strFlashGet = _T("flashget://");

	CString lstrUrlTemp = strUrl;

	lstrUrlTemp.MakeLower();

	if (lstrUrlTemp.Find(strFlashGet) < 0)
	{
		return FALSE;
	}

	CString strFlashGet2 = _T("[FLASHGET]");

	strUrl = strUrl.Right(strUrl.GetLength() - strFlashGet.GetLength());

	int lnLastAndIndex = strUrl.ReverseFind(_T('&'));

	if (lnLastAndIndex > 0)
	{
		strUrl = strUrl.Left(lnLastAndIndex);
	}

	LPSTR src = NULL;


	USES_CONVERSION;

	src = T2A(strUrl.GetBuffer(0));

	lstrUrlTemp = _T("");

	CUtility::Base64Decode((unsigned char *)src,lstrUrlTemp);

	lstrUrlTemp;

	CString strTemp2;

	strTemp2 = lstrUrlTemp;

	strTemp2.MakeUpper();

	int StartIndex = strTemp2.Find(strFlashGet2);	

	lstrUrlTemp = lstrUrlTemp.Right(lstrUrlTemp.GetLength() - (StartIndex+strFlashGet2.GetLength()));

	strTemp2 = lstrUrlTemp;

	int FinIndex = strTemp2.Find(strFlashGet2);

	lstrUrlTemp = lstrUrlTemp.Left(FinIndex);

	strUrl = lstrUrlTemp;

	return TRUE;
}

CString CUtility::GetFileVersion(CString & astrFilename)
{
	CString Version;
	DWORD verSize = GetFileVersionInfoSize(astrFilename.GetBuffer(0), 0);
	if (verSize > 0)
	{
		std::vector<BYTE> lvcVer(verSize + 1);
		LPVOID verData = &lvcVer.front();
		if (GetFileVersionInfo(astrFilename.GetBuffer(0), 0, verSize, verData))
		{
			VS_FIXEDFILEINFO *subVer;
			UINT len;
			VerQueryValue(verData, _T("\\"), (void**)&subVer, &len);
			Version.Format(_T("%d.%d.%d.%d"), HIWORD(subVer->dwFileVersionMS),
				LOWORD(subVer->dwFileVersionMS),
				HIWORD(subVer->dwFileVersionLS), 
				LOWORD(subVer->dwFileVersionLS));
		}
	}
	return Version;	
}

LONGLONG  CUtility::GetFileVersion2( CString & astrFilename)
{
	LONGLONG llVersion;
	DWORD verSize = GetFileVersionInfoSize(astrFilename.GetBuffer(0), 0);
	if (verSize > 0)
	{
		std::vector<BYTE> lvcVer(verSize + 1);
		LPVOID verData = &lvcVer.front();
		if (GetFileVersionInfo(astrFilename.GetBuffer(0), 0, verSize, verData))
		{
			VS_FIXEDFILEINFO *subVer;
			UINT len;
			VerQueryValue(verData, _T("\\"), (void**)&subVer, &len);

			llVersion = subVer->dwFileVersionMS << 32;

			llVersion |= subVer->dwFileVersionLS;
		}
	}
	return llVersion;	

}

CRect CUtility::GetWorkArea()
{
	unsigned int nDesktopHeight;
	unsigned int nDesktopWidth;
	unsigned int nScreenWidth;
	unsigned int nScreenHeight;
	CRect rcDesktop;

	::SystemParametersInfo(SPI_GETWORKAREA,0,&rcDesktop,0);
	nDesktopWidth=rcDesktop.right-rcDesktop.left;
	nDesktopHeight=rcDesktop.bottom-rcDesktop.top;

	nScreenWidth=::GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight=::GetSystemMetrics(SM_CYSCREEN);



	HWND lhTaskBar = ::FindWindowEx(NULL,NULL,_T("Shell_TrayWnd"),NULL);

	BOOL lbShouldCaculateBarByHand = FALSE;

	if (lhTaskBar != NULL)
	{
		APPBARDATA lappBarData;

		ZeroMemory(&lappBarData,sizeof(lappBarData));

		lappBarData.hWnd = lhTaskBar;

		UINT lnBarStatus = SHAppBarMessage(ABM_GETSTATE,&lappBarData);

		if (lnBarStatus == ABS_AUTOHIDE || lnBarStatus == ABS_BOTH )
		{
			BOOL lbret = SHAppBarMessage(ABM_GETTASKBARPOS,&lappBarData);

			if (lbret == TRUE)
			{
				CRect lrcBar(lappBarData.rc);				

				switch(lappBarData.uEdge)
				{
				case ABE_BOTTOM:
					nDesktopHeight = nDesktopHeight - lrcBar.Height();

					break;
				case ABE_LEFT:
					nDesktopWidth = nDesktopWidth - lrcBar.Width();


				case ABE_RIGHT:
					nDesktopWidth = nDesktopWidth - lrcBar.Width();
					break;
				case ABE_TOP:
					nDesktopHeight = nDesktopHeight - lrcBar.Height();				
					break;
				default:

					break;
				}
			}else
			{
				lbShouldCaculateBarByHand = TRUE;
			}
		}else
		{
			lbShouldCaculateBarByHand = TRUE;
		}

	}else
	{
		lbShouldCaculateBarByHand = TRUE;
	}

	return CRect(0,0,nDesktopWidth,nDesktopHeight);
}

BOOL CUtility::AddjustWindow(CRect & rc_wnd)
{
	BOOL bMoved = false;

	CRect border = CUtility::GetWorkArea();

	if (rc_wnd.top < border.top)
	{
		rc_wnd.bottom = border.top + rc_wnd.Height();

		rc_wnd.top = border.top;

		bMoved  =true;
	}

	if (rc_wnd.left < border.left)

	{


		rc_wnd.right = border.left + rc_wnd.Width();

		rc_wnd.left = border.left;

		bMoved  =true;
	}

	if (rc_wnd.right > border.right)
	{
		rc_wnd.left = border.right - rc_wnd.Width();

		rc_wnd.right = border.right;

		bMoved  =true;
	}

	if (rc_wnd.bottom > border.bottom)
	{

		rc_wnd.top = border.bottom - rc_wnd.Height() ;

		rc_wnd.bottom = border.bottom;	

		bMoved  =true;
	}

	return bMoved;
}

BOOL CUtility::AddjustWindow(HWND hwnd,CRect & rc_wnd, CRect & border)
{
	if (!hwnd || !::IsWindow(hwnd))
	{
		return FALSE;
	}

	BOOL bMoved = false;

	if (rc_wnd.top < border.top)
	{
		rc_wnd.bottom = border.top + rc_wnd.Height();

		rc_wnd.top = border.top;

		bMoved  =true;
	}

	if (rc_wnd.left < border.left)

	{


		rc_wnd.right = border.left + rc_wnd.Width();

		rc_wnd.left = border.left;

		bMoved  =true;
	}

	if (rc_wnd.right > border.right)
	{
		rc_wnd.left = border.right - rc_wnd.Width();

		rc_wnd.right = border.right;

		bMoved  =true;
	}

	if (rc_wnd.bottom > border.bottom)
	{

		rc_wnd.top = border.bottom - rc_wnd.Height() ;

		rc_wnd.bottom = border.bottom;	

		bMoved  =true;
	}

	if (bMoved == TRUE)
	{
		bMoved = ::SetWindowPos(hwnd,NULL,rc_wnd.left,rc_wnd.top,rc_wnd.Width(),rc_wnd.Height(),SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOREDRAW);
	}

	return bMoved;
}

BOOL CUtility::AddjustWindow(HWND hwnd)
{
	if(!hwnd || !::IsWindow(hwnd))
	{

		return FALSE;
	}

	CRect rect;

	::GetWindowRect(hwnd,&rect);

	CRect rc_Desktop = CUtility::GetWorkArea();


	BOOL bChanged = CUtility::AddjustWindow(hwnd,rect,rc_Desktop);


	return bChanged;

}

BOOL CUtility::MakeSureFolderExist( const CString &asAbsoluteFilePath )
{

	//		AfxMessageBox(_T("1"));
	if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(asAbsoluteFilePath))
	{

		return TRUE;
	}

	//AfxMessageBox(_T("2"));

	CString lstrFilePath = asAbsoluteFilePath;

	lstrFilePath.TrimLeft(_T(" "));

	lstrFilePath.TrimLeft(_T("\r"));

	lstrFilePath.TrimLeft(_T("\n"));

	if (lstrFilePath.Find(_T(":\\"))!= 1 && lstrFilePath.Find(_T(":/"))!= 1)
	{
		return FALSE;
	}

	//AfxMessageBox(_T("3"));

	if (lstrFilePath[lstrFilePath.GetLength() - 1] != _T('\\'))
	{
		//lstrFilePath = lstrFilePath.Left(lstrFilePath.ReverseFind('\\'));
		lstrFilePath = lstrFilePath + _T("\\");
	}

	CList<CString, CString&> llstPath;

	while (TRUE)
	{
		if (lstrFilePath.GetLength() < 3)
		{
			TRACE(_T("Error: lstrFilePath.GetLength() < 3\n"));

			//AfxMessageBox(_T("Error"));
			return FALSE;
		}
		else
		{
			if (CreateDirectory(lstrFilePath, NULL))
			{
				break;
			}

			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return TRUE;
			}
			else if (GetLastError() != ERROR_PATH_NOT_FOUND)
			{
				TRACE(_T("Unknown Error: %d\n"), GetLastError());

				//AfxMessageBox(_T("Unknown Error"));
				return  FALSE;
			}
		}
		llstPath.AddTail(lstrFilePath);
		lstrFilePath = lstrFilePath.Left(lstrFilePath.ReverseFind('\\'));
	}

	// 循环创建剩下的目录
	CString lstrPath;
	while (!llstPath.IsEmpty())
	{
		lstrPath = llstPath.RemoveTail();
		if (!CreateDirectory(lstrPath, NULL))
		{
			TRACE(_T("Unknown Error: %d\n"), GetLastError());
			//AfxMessageBox(_T("CreateDirectory Unknown Error"));
			return FALSE;
		}
	}

	return TRUE;
} 

HRESULT CUtility::SHAutoComplete(HWND hwndEdit,DWORD dwFlags)
{

	HRESULT (CALLBACK* SHAutoComplete_inner)(HWND ,DWORD);

	HRESULT hr;

	HINSTANCE hIns = LoadLibrary(_T("shlwapi.dll"));

	if(hIns != NULL)
	{
		SHAutoComplete_inner = (HRESULT (CALLBACK* )(HWND ,DWORD))GetProcAddress(hIns, "SHAutoComplete");

		if(SHAutoComplete_inner!=NULL)
		{
			hr =SHAutoComplete_inner(hwndEdit, dwFlags);
		}else
		{
			hr = S_FALSE;
		}

		FreeLibrary(hIns);

	}else
	{
		hr = S_FALSE;
	}

	return hr;
}

CString CUtility::CutStrByPixel(CDC * pDC,CString & strText,int pixelLimit)
{
	if (pDC == NULL)
	{
		return "";
	}
	USES_CONVERSION;

	LPWSTR lpBuffer = T2W(strText.GetBuffer(0));	

	int lnTolalLength = wcslen(lpBuffer);

	int lnIndexEnd = lnTolalLength;

	int lnLastIndexEnd = 0;

	SIZE size = {0};	

	::GetTextExtentPoint32W(pDC->GetSafeHdc(),lpBuffer,lnTolalLength,&size);

	int lnStrTestLength = size.cx;

	if (lnStrTestLength <= pixelLimit)
	{
		return strText;
	}

	BOOL bFound = FALSE;

	int lnExtend = abs(lnLastIndexEnd- lnIndexEnd);

	while( lnExtend>3 )
	{	
		lnExtend = abs(lnLastIndexEnd- lnIndexEnd);

		if (lnStrTestLength < pixelLimit)
		{
			lnLastIndexEnd = lnIndexEnd;

			lnIndexEnd = lnIndexEnd + lnExtend /2;			

		}
		else if (lnStrTestLength > pixelLimit)
		{	
			lnLastIndexEnd = lnIndexEnd;

			lnIndexEnd = lnIndexEnd - lnExtend /2;			
		}
		else
		{
			break;

		}

		::GetTextExtentPoint32W(pDC->GetSafeHdc(),lpBuffer,lnIndexEnd,&size);

		lnStrTestLength = size.cx;

	}

	if (lnIndexEnd > lnLastIndexEnd)
	{
		lnIndexEnd = lnLastIndexEnd;
	}

	lpBuffer[lnIndexEnd] = L'\0';

	CString lstResult = W2T(lpBuffer);

	return lstResult;
}


CString CUtility::TransUTF8ToT(char * apTextFrom)
{	
	DWORD ldwMode = 0;
	int liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apTextFrom, -1, NULL, 0);
	if (liOutNeedLen > 0)
	{
		std::vector<WCHAR> lvcWideChar(liOutNeedLen);
		WCHAR * lwpWideChar = &lvcWideChar.front();

		liOutNeedLen = MultiByteToWideChar(CP_UTF8, 0, apTextFrom, -1, lwpWideChar, liOutNeedLen);
		if (liOutNeedLen > 0)
		{
			return CString(lwpWideChar, liOutNeedLen);
		}
	}
	return _T("");
}

BOOL CUtility::TestConnection(CString& asUrl,CString asCompareValue, lpHttpProxyStruct pProxy /* = NULL */,BOOL abAsync /* = FALSE */,HWND ahCallbackWindow /* = NULL */,UINT anCallbackMessage /* = 0 */)
{
	return TRUE;
}

BOOL CUtility::GetFileContent(CString& asFileName, CString & asFileContent)
{
	CStdioFile loFile;

	CFileException loExp;

	try
	{
		if (!loFile.Open(asFileName,CStdioFile::modeRead|loFile.shareDenyNone,&loExp))
		{
			return FALSE;
		}		

		loFile.SeekToBegin();

		CHAR * lpBuffer = asFileContent.GetBufferSetLength(loFile.GetLength());

		ZeroMemory(lpBuffer,loFile.GetLength());

		int lnRead = loFile.Read(lpBuffer,loFile.GetLength());

		asFileContent.ReleaseBuffer(loFile.GetLength());

	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;
}





BOOL CUtility::WriteFileContent(CHAR *alpFileContent, int anFileContentLength, LPCTSTR alpctFilePath,BOOL bAppend /*= FALSE*/)
{
	CString lstrFilePath(alpctFilePath);

	CFile file;

	try
	{
		BOOL lbRet = FALSE;

		if (!bAppend)
		{
			lbRet = file.Open(lstrFilePath,CFile::modeWrite|CFile::modeCreate|CFile::shareDenyWrite);

		}else
		{
			lbRet = file.Open(lstrFilePath,CFile::modeReadWrite|CFile::shareDenyNone|CFile::modeNoTruncate|CFile::modeCreate);		

			int lnLoopCount = 0;

			if (!lbRet)
			{
				try
				{
					if (file.m_hFile!=INVALID_HANDLE_VALUE)
					{
						file.Close();
					}
					
				}
				catch (...)
				{
				}


				while (lnLoopCount <5)
				{
					Sleep(100);

					lbRet = file.Open(lstrFilePath,CFile::modeReadWrite|CFile::shareDenyNone|CFile::modeNoTruncate|CFile::modeCreate);		

					lnLoopCount ++;

					if (lbRet)
					{
						break;
					}
				}
			}



			if (lbRet)
			{
				file.SeekToEnd();
			}

		}

		if(lbRet)
		{
			file.Write(alpFileContent,anFileContentLength);

			file.Close();

			return TRUE;

		}else
		{
			ASSERT(FALSE);

			return FALSE;
		}
	}
	catch (...)
	{
		ASSERT(FALSE);

		return FALSE;
	}
}

LONGLONG CUtility::GetFileSize(CString & lstrFileName)
{
	LONGLONG nFileSize=0;

	CFileFind finder;

	if(finder.FindFile(lstrFileName))
	{

		finder.FindNextFile();

		nFileSize = finder.GetLength(); //文件大小在这里

		finder.Close();

	}else
	{
		CString lstrTempPath =CUtility::GetAppPath()+"/"+lstrFileName;

		if(finder.FindFile(lstrTempPath))
		{
			finder.FindNextFile();

			nFileSize = finder.GetLength(); //文件大小在这里

			finder.Close();
		}
	}

	return nFileSize;
}

BOOL CUtility::CopyFile(CString &lsSrc, CString &lsDst, BOOL lbOverWrite /*= TRUE*/)
{
	try
	{
		CString lstrDir; 

		BOOL lbRet = CUtility::GetFilePathFromFullName(lsDst,lstrDir);

		if (lbRet)
		{
			lbRet = CUtility::MakeSureFolderExist(lstrDir);

			if (lbRet)
			{
				lbRet =  ::CopyFile(lsSrc,lsDst,!lbOverWrite);
			}
		}

		return lbRet;
	}
	catch (...)
	{
		return FALSE;
	}

}


const int c_nMaxSize = 1024;

HANDLE CUtility::OpenFile(CString & strFullname,BOOL bPromptAlert /* = TRUE */)
{
	USES_CONVERSION;

	HRESULT hr = 	S_FALSE;

	SHELLEXECUTEINFOA   loShellInfo;

	CString lstrFilePath;

	CUtility::GetFilePathFromFullName(strFullname,lstrFilePath);

	ZeroMemory(&loShellInfo,sizeof(SHELLEXECUTEINFOA));

	loShellInfo.cbSize = sizeof(SHELLEXECUTEINFOA);

	loShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	loShellInfo.lpFile = T2A(strFullname.GetBuffer(0));

	loShellInfo.nShow = SW_SHOWNORMAL;

	loShellInfo.lpVerb = T2A(_T("open"));

	loShellInfo.hkeyClass = NULL;

	BOOL lbRet = ShellExecuteExA(&loShellInfo);	

	if (!lbRet)
	{
		lbRet = ShellExecuteExA(&loShellInfo);	
	}

	return loShellInfo.hProcess;


}

CString CUtility::I64toCString(LONGLONG ai64)
{
	CString lstrFileSize;

	_i64tot(ai64,lstrFileSize.GetBuffer(64),10);

	lstrFileSize.ReleaseBuffer(64);

	return lstrFileSize;
}