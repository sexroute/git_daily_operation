#pragma once
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT



#include <additions/ICommInterFace.h>
namespace LY
{
	
	typedef UINT (AFX_CDECL * CALLBACK_PROC)(int SvcCode,ICommInterFace & arefCommInterFace);
	class IServer
	{
	public:
		IServer(void){};
		virtual ~IServer(void){};
		virtual int Init() = 0;
		virtual int Start() = 0;
		virtual int Stop() = 0;
		virtual int RegServiceCallBackHandler(int   hInstancemain,LY::CALLBACK_PROC apfProc) =0;
		virtual int HandleSrvData(void * apData,void * Response){return 0;};
	};
}

