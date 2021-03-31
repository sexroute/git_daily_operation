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
	"专家系统知识获取与特征提取"   ,
	"专家系统"            ,
	"专家系统服务"	        ,
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

bool Config	( struct Service_Module * pModule )   //设置函数	 
{	
	 return true;
}
bool Init	( struct Service_Module * pModule )   //初始化函数 
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
			 sNote.Format("%s初始化失败(%li-%li)",module.pszDescription , sizeof( g_nVerMain ),pModule->dwSize);
			 AfxMessageBox( sNote );			 
			 return FALSE;
		 }
		 memcpy((void *)&g_nVerMain ,pModule->pInputPointer ,pModule->dwSize  );
	 }
	 if( pModule->dwInputType == 1 )
     {
		 g_pCallBackFunc=(BOOL (_cdecl *)( struct Service_Module * pModule ))module.pInputPointer ;
	 }
	 
	 if( pModule->dwInputType == 2 ) //刷新配置信息
     {
		 return true;
	 }
	 
	 if( pModule->dwInputType == 3 ) //读取功能码明细
     { 

#ifndef  _DEBUG 
// _DEBUG
		 CStringArray *pVal=(CStringArray *)pModule->pInputPointer ;
		 CString sVal ;		 
		 sVal.Format("%li,专家系统操作,得到指定报警ID时专家诊断系统特征提取的知识,保留"	 ,SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT);(*pVal).Add(sVal);
         sVal.Format("%li,专家系统操作,得到指定振动测点选定时间段时专家系统特征提取的知识,保留"	 ,SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT);(*pVal).Add(sVal);
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

bool Quit	( struct Service_Module * pModule )   //退出函数
{	
	 return true;
}

bool Run	( struct Service_Module * pModule )   //执行函数
{	
	 return true;
}


bool Do( struct Service_Module * pModule )   //操作函数
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
    {//中间件模块
		//得到常规报警时,专家诊断系统特征提取的知识
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


bool DoNext   ( struct Service_Module * pModule )    //操作函数
{		
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //预留函数
{     
	 return true;
}
