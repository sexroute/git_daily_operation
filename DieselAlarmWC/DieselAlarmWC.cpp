// DieselAlarmWC.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DieselAlarmWC.h"

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

// CDieselAlarmWCApp

BEGIN_MESSAGE_MAP(CDieselAlarmWCApp, CWinApp)
END_MESSAGE_MAP()


// CDieselAlarmWCApp ����

CDieselAlarmWCApp::CDieselAlarmWCApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDieselAlarmWCApp ����

CDieselAlarmWCApp theApp;


// CDieselAlarmWCApp ��ʼ��

BOOL CDieselAlarmWCApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
