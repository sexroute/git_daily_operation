// DieselAlarm956.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DieselAlarm956.h"

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

// CDieselAlarm956App

BEGIN_MESSAGE_MAP(CDieselAlarm956App, CWinApp)
END_MESSAGE_MAP()


// CDieselAlarm956App ����

CDieselAlarm956App::CDieselAlarm956App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDieselAlarm956App ����

CDieselAlarm956App theApp;


// CDieselAlarm956App ��ʼ��

BOOL CDieselAlarm956App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
