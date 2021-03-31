#include "stdafx.h"
#ifdef _M_BHKD_SERVER_SIDE_


#include "service.h"
#include "process.h"
#include <additions/BHCommWrapper.h>
#include <CaseLibrary/CaseHandle.h>
#include <CaseLibrary/CaseSvcDef.h>
long g_nVerMain =0;
#include <additions/MyAdapter.h>
BOOL     (*g_pCallBackFunc)( struct Service_Module * pModule )=NULL;
Service_Module module= {
	100 ,
		"BHXZ" ,
		"�õ����ֲ�����Ϣ"			,
		"������"            ,
		""	        ,
		NULL    ,
		0		,
		NULL	,
		NULL    ,
		0		,

		NULL	,
		NULL	,
		NULL    ,
		NULL    ,
		NULL    ,
		NULL    ,

		Config  ,
		Init	,
		Quit	,
		Run		,
		Do		,
		DoNext	,
		DoReserve
};


Service_Module *Service(){
	CString sDate			;
	sDate.Format("%s",__DATE__	 );
	module.pszBuildDate = new char[sDate.GetLength() + 2 ];
	strncpy( module.pszBuildDate , sDate,sDate.GetLength() + 1  );
	return &module; 
}

bool Config	( struct Service_Module * pModule )   //���ú���
{
	return true;
}

bool Init	( struct Service_Module * pModule )   //��ʼ������
{
	if (Winet::CAdapterHelper::Check()!=TRUE)
	{
		return false;
	}
	CString sNote;
	if( pModule->dwInputType == 0 )
	{
		if( sizeof( g_nVerMain ) != pModule->dwSize  )
		{
			sNote.Format("%s��ʼ��ʧ��(%li-%li)",module.pszDescription , sizeof( g_nVerMain ),pModule->dwSize);
			AfxMessageBox( sNote );
			return FALSE;
		}
		memcpy((void *)&g_nVerMain ,pModule->pInputPointer ,pModule->dwSize  );
	}
	if( pModule->dwInputType == 1 )
	{
		g_pCallBackFunc=(BOOL (_cdecl *)( struct Service_Module * pModule ))module.pInputPointer ;
	}

	if( pModule->dwInputType == 2 ) //ˢ��������Ϣ
	{
		return true;
	}

#ifndef _DEBUG
	if( pModule->dwInputType == 3 ) //��ȡ��������ϸ
	{


		CStringArray *pVal=(CStringArray *)pModule->pInputPointer ;
		CString sVal ;
		sVal.Format("%li,�õ�������Ϣ,�޸ļ�������м����ַ��Ϣ,����",SERVICE_SYSTEM_MOIDFY_MWINFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��Ӱ���������Ϣ",CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li �޸Ļ�����Ϣ",CASELIBRARY_MODULE_CASE_UPDATE_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ӹ�����Ϣ",
			CASELIBRARY_MODULE_CASE_ADD_FAULT_INFO);
		(*pVal).Add(sVal);;

		sVal.Format("%li ���������Ϣ",
			CASELIBRARY_MODULE_CASE_ADD_SYMPTOM_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��Ӹ�����Ϣ",
			CASELIBRARY_MODULE_CASE_ADD_ATTACHMENT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li �����ύ",
			CASELIBRARY_MODULE_CASE_SUBMIT);
		(*pVal).Add(sVal);

		sVal.Format("%li �������",
			CASELIBRARY_MODULE_CASE_VERIFY);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ��������Ϣ",
			CASELIBRARY_MODULE_CASE_DEL_ATTACHMENT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ��������Ϣ",
			CASELIBRARY_MODULE_CASE_DEL_SYMPTOM_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ��������Ϣ",
			CASELIBRARY_MODULE_CASE_DEL_FAULT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ������",
			CASELIBRARY_MODULE_CASE_DEL);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ���а���",
			CASELIBRARY_MODULE_CASE_QUERY_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ����������Ϣ",
			CASELIBRARY_MODULE_CASE_QUERY_FAULT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ����������Ϣ",
			CASELIBRARY_MODULE_CASE_QUERY_SYMPTOM_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ����������¼�����������������ݣ�",
			CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_RECORD);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ������������������",
			CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_DATA);
		(*pVal).Add(sVal);

		sVal.Format("%li ���ڰ�������",
			CASELIBRARY_MODULE_CASE_BASED_REASONING);
		(*pVal).Add(sVal);

		sVal.Format("%li ���Ӽ�ά�޼�¼",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ָ����ά�޼�¼���һ������",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_ATTACHMENT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ���¼�ά�޼�¼������Ϣ",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_UPDATE_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ����ά�޼�¼�ĸ�����Ϣ",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_ATTACHMENT_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ��һ����ά�޼�¼",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ��ά�޼�¼",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_BASIC_INFO);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯָ����ά�޼�¼�ĸ�����¼(����������������)",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_RECORD);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯָ����ά�޼�¼�ĸ�������������",
			CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_DATA);
		(*pVal).Add(sVal);

		sVal.Format("%li ���һ����ͣ����¼",
			CASELIBRARY_MODULE_SUD_RECORD_ADD);
		(*pVal).Add(sVal);

		sVal.Format("%li ����һ����ͣ����¼",
			CASELIBRARY_MODULE_SUD_RECORD_UPDATE);
		(*pVal).Add(sVal);

		sVal.Format("%li ɾ��һ����ͣ����¼",
			CASELIBRARY_MODULE_SUD_RECORD_DEL);
		(*pVal).Add(sVal);

		sVal.Format("%li ��ѯ��ͣ����¼",
			CASELIBRARY_MODULE_SUD_RECORD_QUERY);
		(*pVal).Add(sVal);


		return true;
	}
#endif

	if((pModule->pDB)!=NULL)
	{
		if(g_bInit==false)
		{
			g_bInit=true;
			InitGlobalVar(pModule->pDB);
		}
	}
	return true;
}

bool Quit	( struct Service_Module * pModule )   //�˳�����
{
	return true;
}

bool Run	( struct Service_Module * pModule )   //ִ�к���
{
	return true;
}


bool Do( struct Service_Module * pModule )   //��������
{

	BOOL IsServerStartingComplete = FALSE;
	g_pServerStatusLock->Lock();
	IsServerStartingComplete = g_pServerStatus->IsServerStartupComplete();
	g_pServerStatusLock->Unlock();

	if (!IsServerStartingComplete)
	{
		return false;
	}

	bool lbNeedOpenDB = false;

	switch ( pModule->pFixComm->GetFunc() )
	{

	case SERVICE_SYSTEM_MOIDFY_MWINFO:
		lbNeedOpenDB = true;
		break;
	case TEST_CASE_MW:
		lbNeedOpenDB = true;
		break;
	case CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO:
		lbNeedOpenDB = true;		
		break;
	default:
		lbNeedOpenDB = true;
		break;
		//return false;
	}

	COleSqlServerDB loDb;

	loDb.setPDB(pModule->pDB);

	CBHCommWrapper loWrapper;

	loWrapper.SetFixComm(pModule->pFixComm);

	switch ( pModule->pFixComm->GetFunc() )
	{
		//�õ�ָ���豸�ſ�������ͣ���Ĳ��

	case SERVICE_SYSTEM_MOIDFY_MWINFO:
		*pModule->pCode=Svr_SysModifyMwInfo(loWrapper,loDb);
		break;
	case TEST_CASE_MW:
		*pModule->pCode=CCaseHandle::GetTestInt(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO:
		*pModule->pCode=CCaseHandle::CaseAddBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_UPDATE_BASIC_INFO:
		*pModule->pCode=CCaseHandle::CaseUpdateBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_FAULT_INFO:
		*pModule->pCode=CCaseHandle::CaseAddFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_SYMPTOM_INFO:
		*pModule->pCode=CCaseHandle::CaseAddSymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_ATTACHMENT_INFO:
		*pModule->pCode=CCaseHandle::CaseAddAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_SUBMIT:
		*pModule->pCode=CCaseHandle::CaseSubmit(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_VERIFY:
		*pModule->pCode=CCaseHandle::CaseVerify(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_ATTACHMENT_INFO:
		*pModule->pCode=CCaseHandle::CaseDelAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_SYMPTOM_INFO:
		*pModule->pCode=CCaseHandle::CaseDelSymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_FAULT_INFO:
		*pModule->pCode=CCaseHandle::CaseDelFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL:
		*pModule->pCode=CCaseHandle::CaseDel(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_BASIC_INFO:
		*pModule->pCode=CCaseHandle::CaseQueryBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_FAULT_INFO:
		*pModule->pCode=CCaseHandle::CaseQueryFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_SYMPTOM_INFO:
		*pModule->pCode=CCaseHandle::CaseQuerySymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_RECORD:
		*pModule->pCode=CCaseHandle::CaseQueryAttachmentRecord(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_DATA:
		*pModule->pCode=CCaseHandle::CaseQueryAttachmentData(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_BASED_REASONING:
		*pModule->pCode=CCaseHandle::CaseBasedReasoning(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_BASIC_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordAddBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_ATTACHMENT_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordAddAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_UPDATE_BASIC_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordUpdateBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_ATTACHMENT_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordDelAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_BASIC_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordDelBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_BASIC_INFO:
		*pModule->pCode=CCaseHandle::MaintenanceRecordQueryBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_RECORD:
		*pModule->pCode=CCaseHandle::MaintenanceRecordQueryAttachmentRecord(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_DATA:
		*pModule->pCode=CCaseHandle::MaintenanceRecordQueryAttachmentData(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_ADD:
		*pModule->pCode=CCaseHandle::SudRecordAdd(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_UPDATE:
		*pModule->pCode=CCaseHandle::SudRecordUpdate(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_DEL:
		*pModule->pCode=CCaseHandle::SudRecordDel(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_QUERY:
		*pModule->pCode=CCaseHandle::SudRecordQuery(loWrapper,loDb);
		break;
	default:
		ASSERT(FALSE);
		return false;
	}
	return true;
}


bool DoNext(struct Service_Module * pModule )    //��������
{
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //Ԥ������
{
	return true;
};



#else //����Exeģʽ֧��
#include <additions/ICommInterFace.h>
#include "CaseLibrary/CaseSvcDef.h"
#include <CaseLibrary/CaseHandle.h>
#include "FakeComm.h"
#include "Process.h"
void * Service()
{
	return NULL; 
}

bool Do( CFakeComm * pFixComm )   //��������
{

	if (!pFixComm)
	{
		return false;
	}

	COleSqlServerDB loDb;

	loDb.setPDB(NULL);

	ICommInterFace & loWrapper = *pFixComm;
	int lnReturnCode = 0;

	switch ( pFixComm->GetFunc() )
	{
		//�õ�ָ���豸�ſ�������ͣ���Ĳ��

	case SERVICE_SYSTEM_MOIDFY_MWINFO:
		lnReturnCode=Svr_SysModifyMwInfo(loWrapper,loDb);
		break;
	case TEST_CASE_MW:
		lnReturnCode=CCaseHandle::GetTestInt(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO:
		lnReturnCode=CCaseHandle::CaseAddBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_UPDATE_BASIC_INFO:
		lnReturnCode=CCaseHandle::CaseUpdateBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_FAULT_INFO:
		lnReturnCode=CCaseHandle::CaseAddFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_SYMPTOM_INFO:
		lnReturnCode=CCaseHandle::CaseAddSymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_ADD_ATTACHMENT_INFO:
		lnReturnCode=CCaseHandle::CaseAddAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_SUBMIT:
		lnReturnCode=CCaseHandle::CaseSubmit(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_VERIFY:
		lnReturnCode=CCaseHandle::CaseVerify(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_ATTACHMENT_INFO:
		lnReturnCode=CCaseHandle::CaseDelAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_SYMPTOM_INFO:
		lnReturnCode=CCaseHandle::CaseDelSymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL_FAULT_INFO:
		lnReturnCode=CCaseHandle::CaseDelFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_DEL:
		lnReturnCode=CCaseHandle::CaseDel(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_BASIC_INFO:
		lnReturnCode=CCaseHandle::CaseQueryBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_FAULT_INFO:
		lnReturnCode=CCaseHandle::CaseQueryFaultInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_SYMPTOM_INFO:
		lnReturnCode=CCaseHandle::CaseQuerySymptomInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_RECORD:
		lnReturnCode=CCaseHandle::CaseQueryAttachmentRecord(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_DATA:
		lnReturnCode=CCaseHandle::CaseQueryAttachmentData(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_CASE_BASED_REASONING:
		lnReturnCode=CCaseHandle::CaseBasedReasoning(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_BASIC_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordAddBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_ATTACHMENT_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordAddAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_UPDATE_BASIC_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordUpdateBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_ATTACHMENT_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordDelAttachmentInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_BASIC_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordDelBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_BASIC_INFO:
		lnReturnCode=CCaseHandle::MaintenanceRecordQueryBasicInfo(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_RECORD:
		lnReturnCode=CCaseHandle::MaintenanceRecordQueryAttachmentRecord(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_DATA:
		lnReturnCode=CCaseHandle::MaintenanceRecordQueryAttachmentData(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_ADD:
		lnReturnCode=CCaseHandle::SudRecordAdd(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_UPDATE:
		lnReturnCode=CCaseHandle::SudRecordUpdate(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_DEL:
		lnReturnCode=CCaseHandle::SudRecordDel(loWrapper,loDb);
		break;
	case CASELIBRARY_MODULE_SUD_RECORD_QUERY:
		lnReturnCode=CCaseHandle::SudRecordQuery(loWrapper,loDb);
		break;
	default:
		ASSERT(FALSE);
		return lnReturnCode;
	}
	return lnReturnCode;
}
#endif