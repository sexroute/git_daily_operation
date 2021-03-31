// S_SystemSvr.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "SysGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HMODULE g_ModuleSelf = NULL;

int RetainLibrayInMemory(
							  HMODULE ahDllHandle,
							  HMODULE *apLocalDllHandle)
{
	if(NULL == apLocalDllHandle)
		return ERROR_INVALID_PARAMETER;

	*apLocalDllHandle = NULL;
	TCHAR moduleName[1024];
	if(0 == GetModuleFileName(
		ahDllHandle,
		moduleName,
		sizeof(moduleName)/ sizeof(TCHAR)))
	{
		return GetLastError();
	}

	*apLocalDllHandle = LoadLibrary(moduleName);

	if(NULL == *apLocalDllHandle)
	{
		return GetLastError();
	}
	return NO_ERROR;
}
static AFX_EXTENSION_MODULE S_SystemSvrDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		BHLOG_DEBUG("S_SYSTEMSVR.DLL Initializing!");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(S_SystemSvrDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.
		RetainLibrayInMemory(hInstance,&g_ModuleSelf);
		new CDynLinkLibrary(S_SystemSvrDLL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("S_SYSTEMSVR.DLL Terminating!\n");
		QuitClearGlobalVar();
		// Terminate the library before destructors are called
		AfxTermExtensionModule(S_SystemSvrDLL);
	}
	return 1;   // ok
}
