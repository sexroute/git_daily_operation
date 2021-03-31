// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__837AD83A_3512_4F37_8216_65AF51510599__INCLUDED_)
#define AFX_STDAFX_H__837AD83A_3512_4F37_8216_65AF51510599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

/*
#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT
*/

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "winsock2.h"
#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include "dbfunc.h"
#include "acifunc.h"
#include "fixdef.h"
#include "FixDef_Daq.h"
#include "KDCommLib.h"
//#include "AnalizeDiagnose_MW.h"
using namespace std;
#include "ErrCode.h"

#include "Expert_Define.h"
#include "BHKD_utilities.h"
#include "SigMath.h"
#include "Synopsis2KB.h"
#include "math.h"
using namespace BHKD;

#include "ExpertSysGlobal.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__837AD83A_3512_4F37_8216_65AF51510599__INCLUDED_)