// GasTurbineAlarm.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "GasTurbineAlarm.h"

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

// CGasTurbineAlarmApp

BEGIN_MESSAGE_MAP(CGasTurbineAlarmApp, CWinApp)
END_MESSAGE_MAP()


// CGasTurbineAlarmApp ����

CGasTurbineAlarmApp::CGasTurbineAlarmApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGasTurbineAlarmApp ����

CGasTurbineAlarmApp theApp;


// CGasTurbineAlarmApp ��ʼ��

BOOL CGasTurbineAlarmApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
