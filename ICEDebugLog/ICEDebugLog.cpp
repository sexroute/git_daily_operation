// ICEDebugLog.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "ICEDebugLog.h"
#include "DlgMonitor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	ע�⣡
//
//		����� DLL ��̬���ӵ� MFC
//		DLL���Ӵ� DLL ������
//		���� MFC ���κκ����ں�������ǰ��
//		��������� AFX_MANAGE_STATE �ꡣ
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CICEDebugLogApp

BEGIN_MESSAGE_MAP(CICEDebugLogApp, CWinApp)
END_MESSAGE_MAP()


// CICEDebugLogApp ����

CICEDebugLogApp::CICEDebugLogApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CICEDebugLogApp ����

CICEDebugLogApp theApp;


// CICEDebugLogApp ��ʼ��

BOOL CICEDebugLogApp::InitInstance()
{
	CWinApp::InitInstance();
   
	return TRUE;
}
