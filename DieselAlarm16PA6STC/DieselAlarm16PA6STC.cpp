// DieselAlarm16PA6STC.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DieselAlarm16PA6STC.h"

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

// CDieselAlarm16PA6STCApp

BEGIN_MESSAGE_MAP(CDieselAlarm16PA6STCApp, CWinApp)
END_MESSAGE_MAP()


// CDieselAlarm16PA6STCApp ����

CDieselAlarm16PA6STCApp::CDieselAlarm16PA6STCApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDieselAlarm16PA6STCApp ����

CDieselAlarm16PA6STCApp theApp;


// CDieselAlarm16PA6STCApp ��ʼ��

BOOL CDieselAlarm16PA6STCApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
