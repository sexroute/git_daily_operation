#include "stdafx.h"
#include "service.h"
#include "process.h"
#include "ExpertSysGlobal.h"
#include "BusinessObject.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/BHCommWrapper.h>
#include "MiddleWare.h"
#include <additions/MyAdapter.h>
long g_nVerMain =0;

BOOL     (*g_pCallBackFunc)( struct Service_Module * pModule )=NULL;
Service_Module module= {
		100 ,
	"BHXZ" ,
	"ר��ϵͳ֪ʶ��ȡ��������ȡ"   ,
	"ר��ϵͳ"            ,
	"ר��ϵͳ����"	        ,
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
	strncpy( module.pszBuildDate , sDate ,sDate.GetLength() + 1);
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
	 
	 if( pModule->dwInputType == 3 ) //��ȡ��������ϸ
     { 

#ifndef  _DEBUG 
// _DEBUG
		 CStringArray *pVal=(CStringArray *)pModule->pInputPointer ;
		 CString sVal ;		 
		 sVal.Format("%li,ר��ϵͳ����,�õ�ָ������IDʱר�����ϵͳ������ȡ��֪ʶ,����"	 ,SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT);(*pVal).Add(sVal);
         sVal.Format("%li,ר��ϵͳ����,�õ�ָ���񶯲��ѡ��ʱ���ʱר��ϵͳ������ȡ��֪ʶ,����"	 ,SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT);(*pVal).Add(sVal);
#endif 
		 return true;
	 }
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
	if (NULL != pModule)
	{
		int lnTimeOut = CConfigHelper::GetFixCommTimeOut();

		pModule->pFixComm->SetTimeOut(lnTimeOut);
	}

	BOOL IsServerStartingComplete = FALSE;
	g_pServerStatusLock->Lock();
	IsServerStartingComplete = g_pServerStatus->IsServerStartupComplete();
	g_pServerStatusLock->Unlock();

	if (!IsServerStartingComplete)
	{
		return false;
	}

	if (!pModule)
	{
		return false;
	}

	CBHCommWrapper loCommWrapper;
	loCommWrapper.SetFixComm(pModule->pFixComm);

	COleSqlServerDB loDbWrapper;
	loDbWrapper.setPDB(pModule->pDB);

	switch ( pModule->pFixComm->GetFunc() ) 
    {//�м��ģ��
		//�õ����汨��ʱ,ר�����ϵͳ������ȡ��֪ʶ
		case SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT:
			*pModule->pCode=Service_ExpertSysGetRegAlarmFeatureExtract(&loCommWrapper,&loDbWrapper);
			break;

		case SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT:
			*pModule->pCode=Service_ExpertSysGetSpecChannFeatureExtract(&loCommWrapper,&loDbWrapper);
			break;

/*
		case SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT_RC:
			*pModule->pCode = CBHExpertBizObj::Handler<CBODiagnosisRC>(loCommWrapper,loDbWrapper);

			break;*/
		case  SERVICE_EXPERTSYS_GET_PLANTFACTNAME:
			*pModule->pCode = Service_ExpertSysGetPlantFactName(&loCommWrapper,&loDbWrapper);
			break;
		default:
			return false;
	}
    return true;
}


bool DoNext   ( struct Service_Module * pModule )    //��������
{		
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //Ԥ������
{     
	 return true;
}
