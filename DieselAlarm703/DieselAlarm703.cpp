// DieselAlarm703.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DieselAlarm703.h"

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

// CDieselAlarm703App

BEGIN_MESSAGE_MAP(CDieselAlarm703App, CWinApp)
END_MESSAGE_MAP()


// CDieselAlarm703App ����

CDieselAlarm703App::CDieselAlarm703App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDieselAlarm703App ����

CDieselAlarm703App theApp;


// CDieselAlarm703App ��ʼ��

BOOL CDieselAlarm703App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
