#include "stdafx.h"
#include "service.h"
#include "process.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/BHCommWrapper.h>
#include "HandleCollector.h"
#include "SysGlobal.h"
#include <additions/RemoteServer.h>
#include "MiddleWare.h"
#include <additions/MyAdapter.h>
long g_nVerMain =0;

BOOL     (*g_pCallBackFunc)( struct Service_Module * pModule )=NULL;
Service_Module module= {
	100 ,
		"BHKD" ,
		"得到各种参数信息"			,
		"系统服务"            ,
		"系统"	        ,
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
#ifndef _DEBUG
		CStringArray *pVal=(CStringArray *)pModule->pInputPointer ;
		CString sVal ;
		sVal.Format("%li,得到参数信息,得到指定设备号的启停车测点信息,保留",SERVICE_SYSTEM_GETPLANTSUDCHANN);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定分厂下所有设备位号,保留"	 ,SERVICE_SYSTEM_GETPLANTOFSUBCORP);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定设备位号的所有测点名和对应的测点类型,保留" ,SERVICE_SYSTEM_GETALLCHANN);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定设备指定测点类型的所有测点名,保留"	 ,SERVICE_SYSTEM_GETSPECTYPECHANN);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,通过用户名和密码得到用户可操作的集团、公司,保留"	 ,SERVICE_SYSTEM_GETCOMPANYTOFUSER);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定设备号所有组轴心轨迹测点号,保留"	 ,SERVICE_SYSTEM_GETALLAXISCHANN);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定设备号最高的频谱分析线数,保留"	 ,SERVICE_SYSTEM_GETPLANTANALYSISLINES);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,通过所有分厂名,保留"	 ,SERVICE_SYSTEM_GETALLFACTORY);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,通过所有公司信息,保留"	 ,SERVICE_SYSTEM_GETALLCOMPANY);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,修改密码,保留"	 ,SERVICE_SYSTEM_MODIFYPWD);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,通过测点类型和传感器类型得到特征值描述,保留"	 ,SERVICE_SYSTEM_GETCHARTYPEDESC);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定用户的客户端页面更新时间,保留"	 ,SERVICE_SYSTEM_GET_REFRESHTIME);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定设备号和测点类型的测点号和报警上下限,保留"	 ,SERVICE_SYSTEM_GETALARMTHRESHOLD);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,用户登录,保留"	 ,SERVICE_SYSTEM_LOGIN);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,修改用户级别,保留"	 ,SERVICE_SYSTEM_MODIFY_USERLEVEL);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,添加新用户,保留"	 ,SERVICE_SYSTEM_ADD_NEWUSER);(*pVal).Add(sVal);
		sVal.Format("%li,设置参数信息,修改是否检查报警,保留",SERVICE_SYSTEM_SETCHECKALARMFLAG); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到设备概貌图类型,保留",SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,修改监测中心中间件地址信息,保留",SERVICE_SYSTEM_MOIDFY_MWINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到监测中心中间件地址信息,保留",SERVICE_SYSTEM_GET_MWINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,修改向监测中心传输的信息,保留",SERVICE_SYSTEM_MODIFY_SENDINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到向监测中心传输的信息,保留",SERVICE_SYSTEM_GET_SENDINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到装置级中间件服务器信息,保留",SERVICE_SYSTEM_GET_SETSVRINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到装置级中间件服务器上的设备信息,保留",SERVICE_SYSTEM_GET_SETSVRPLANTINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到装置级中间件服务器上指定设备的测点信息,保留",SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定公司、分厂、设备的设备详细信息,保留",SERVICE_SYSTEM_GETPLANTINFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到中间件信息,得到指定下一级的中间件详细信息,保留",SERVICE_SYSTEM_GET_MW_INFO); (*pVal).Add(sVal);
		sVal.Format("%li,得到连接状态,是否中间件可连接,保留",SERVICE_SYSTEM_CONN_STATUS); (*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到设备名、IP、概貌图等信息,保留",SERVICE_SYSTEM_GETPLANTIPOFSUBCORP); (*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到指定设备的所有测点信息,保留"	 ,SERVICE_SYSTEM_GET_CHANNINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到指定分厂的所有设备信息,保留"	 ,SERVICE_SYSTEM_GET_PLANTINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到指定设备指定测点信息,保留"	 ,SERVICE_SYSTEM_GET_SPEC_CHANNINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到指定设备信息,保留"	 ,SERVICE_SYSTEM_GET_SPECPLANTINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到所有用户信息,保留"	 ,SERVICE_SYSTEM_GET_ALLUSERINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的设备信息表,保留"	 ,SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的信息描述表,保留"	 ,SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,根据测点信息类型、得到监测中心的信息描述表,保留"	 ,SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,修改指定设备指定测点的报警信息,保留"	 ,SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的自定义参数模板表,保留"	 ,SERVICE_SYSTEM_GET_SETSVRTEMPLET);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的自定义参数信息表,保留"  ,SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的自定义参数报警参数模板表,保留"  ,SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的自定义参数报警参数信息表,保留"  ,SERVICE_SYSTEM_GET_SETSVRALARMPARAM);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的通道自定义参数报警参数信息表,保留"  ,SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定公司、分厂和装置ID的自定义参数信息,保留"  ,SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM);(*pVal).Add(sVal);
		sVal.Format("%li,得到参数信息,得到指定公司、分厂和装置ID的自定义报警参数信息,保留"  ,SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心配置的自定义轴承参数表,保留"	 ,SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的齿轮箱参数表,保留"  ,SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的位置参数表,保留"  ,SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL);(*pVal).Add(sVal);
		sVal.Format("%li,系统操作,得到监测中心的部位参数表,保留"  ,SERVICE_SYSTEM_GET_SETSVRSECTIONTBL);(*pVal).Add(sVal);

		/* 守护巡检 */
		sVal.Format("%li,守护巡检,获取各个程序的路径和版本号,保留", SERVICE_SYSTEM_GET_MODULE_VERSION);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,获取各个数据库状态,保留",         SERVICE_SYSTEM_GET_DB_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,获取中间件运行状态,保留",         SERVICE_SYSTEM_GET_SERVER_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,获取数采硬件状态,保留",           SERVICE_SYSTEM_GET_DAQ_HW_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,获取数采软件状态,保留",           SERVICE_SYSTEM_GET_DAQ_SW_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,获取基本的组态信息,保留",         SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO);(*pVal).Add(sVal);
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

	CHandleCollector::Init();
	CHandleCollector::StartTrack();
	if (g_bEnableDumpMonitor)
	{
		CRemoteSesrver::StartDebugger();
	}
	bool lbNeedOpenDB = false;

	switch ( pModule->pFixComm->GetFunc() )
	{
		//得到指定设备号可以做启停车的测点
	case  SERVICE_SYSTEM_GETPLANTSUDCHANN:
		lbNeedOpenDB = true;
		break;
		//得到指定分厂下所有设备位号,通过循环依次得到所有设备位号
	case SERVICE_SYSTEM_GETPLANTOFSUBCORP:
		lbNeedOpenDB = true;
		break;
		//得到指定设备位号的所有测点名和对应的测点类型，通过循环依次得到
	case SERVICE_SYSTEM_GETALLCHANN:
		lbNeedOpenDB = true;
		break;
		//通过用户名和密码得到用户可操作的所有设备，通过循环依次得到所有设备位号
	case SERVICE_SYSTEM_GETCOMPANYTOFUSER:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETALLAXISCHANN:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETPLANTANALYSISLINES:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETPLANTINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETALLFACTORY:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETALLCOMPANY:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_MODIFYPWD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETCHARTYPEDESC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_REFRESHTIME:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETALARMTHRESHOLD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_LOGIN:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_MODIFY_USERLEVEL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_ADD_NEWUSER:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_SETCHECKALARMFLAG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_MOIDFY_MWINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_MWINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_MODIFY_SENDINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SENDINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRPLANTINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_ALLUSERINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_MW_INFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_CONN_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETPLANTIPOFSUBCORP:
		lbNeedOpenDB = true;
		break;

	case SERVICE_SYSTEM_GET_CHANNINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_PLANTINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SPEC_CHANNINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SPECPLANTINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
		lbNeedOpenDB = false;
		break;
	case SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRTEMPLET:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRALARMPARAM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_SETSVRSECTIONTBL:
		lbNeedOpenDB = true;
		break;

		/* 守护巡检 */
		/* 获取各个程序的路径和版本号 */
	case SERVICE_SYSTEM_GET_MODULE_VERSION:
		lbNeedOpenDB = true;
		break;
		/* 获取各个数据库状态 */
	case SERVICE_SYSTEM_GET_DB_STATUS:
		lbNeedOpenDB = true;
		break;
		/* 获取中间件运行状态 */
	case SERVICE_SYSTEM_GET_SERVER_STATUS:
		lbNeedOpenDB = true;
		break;
		/* 获取数采硬件状态 */
	case SERVICE_SYSTEM_GET_DAQ_HW_STATUS:
		lbNeedOpenDB = true;
		break;
		/* 获取数采软件状态 */
	case SERVICE_SYSTEM_GET_DAQ_SW_STATUS:
		lbNeedOpenDB = true;
		break;
		/* 获取基本的组态信息 */
	case SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SYSTEM_SET_USERINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_PLANT_OF_USER:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GET_USER_OF_PLANT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_ACTION_USER_PLANT:
		lbNeedOpenDB = true;
		break;
	default:
		return false;
	}

	if (lbNeedOpenDB)
	{

	}

	if (lbNeedOpenDB)
	{
		COleSqlServerDB loDb;

		loDb.setPDB(pModule->pDB);

		CBHCommWrapper loFixComm;
		loFixComm.SetFixComm(pModule->pFixComm);

		switch ( pModule->pFixComm->GetFunc() )
		{
			//得到指定设备号可以做启停车的测点
		case  SERVICE_SYSTEM_GETPLANTSUDCHANN:
			*pModule->pCode= Service_SysGetPlantSudChann(&loFixComm , &loDb );
			break;
			//得到指定分厂下所有设备位号,通过循环依次得到所有设备位号
		case SERVICE_SYSTEM_GETPLANTOFSUBCORP:
			*pModule->pCode = Service_SysGetPlantOfSubCorp (&loFixComm , &loDb );
			break;
			//得到指定设备位号的所有测点名和对应的测点类型，通过循环依次得到
		case SERVICE_SYSTEM_GETALLCHANN:
			*pModule->pCode = Service_SysGetAllChann(&loFixComm , &loDb );
			break;
			//通过用户名和密码得到用户可操作的所有设备，通过循环依次得到所有设备位号
		case SERVICE_SYSTEM_GETCOMPANYTOFUSER:
			*pModule->pCode=Service_SysGetCompanyOfUser(&loFixComm , &loDb );
			break;
		case SERVICE_SYSTEM_GETALLAXISCHANN:
			*pModule->pCode= Service_SysGetAllAxisChann(&loFixComm,&loDb );
			break;
		case SERVICE_SYSTEM_GETPLANTANALYSISLINES:
			*pModule->pCode=Service_SysGetPlantAnalysislines(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETPLANTINFO:
			*pModule->pCode=Srv_SysGetPlantInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETALLFACTORY:
			*pModule->pCode=Srv_SysGetAllFactory(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETALLCOMPANY:
			*pModule->pCode=Srv_SysGetAllCompany(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_MODIFYPWD:
			*pModule->pCode=Service_SysModifyPwd(&loFixComm , &loDb);
			break;
		case SERVICE_SYSTEM_GETCHARTYPEDESC:
			*pModule->pCode=Srv_SysGetCharTypeDesc(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_REFRESHTIME:
			*pModule->pCode=Service_SysGetRefreshTime(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETALARMTHRESHOLD:
			*pModule->pCode=Service_SysGetThreshold(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_LOGIN:
			*pModule->pCode=Service_SysLogin(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_MODIFY_USERLEVEL:
			*pModule->pCode=Service_SysModifyUserLevel(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_ADD_NEWUSER:
			*pModule->pCode=Service_SysAddNewUser(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_SETCHECKALARMFLAG:
			*pModule->pCode=Service_SysSetCheckAlarmFlag(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID:
			*pModule->pCode=Service_SysGetGraphTypeByDeviceID(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_MOIDFY_MWINFO:
			*pModule->pCode=Svr_SysModifyMwInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_MWINFO:
			*pModule->pCode=Svr_SysGetMwInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_MODIFY_SENDINFO:
			*pModule->pCode=Svr_SysModifySendInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SENDINFO:
			*pModule->pCode=Svr_SysGetSendInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRINFO:
			*pModule->pCode=Svr_SysGetSetSvrInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRPLANTINFO:
			*pModule->pCode=Svr_SysGetSetSvrPlantInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN:
			*pModule->pCode=Svr_SysGetSetSvrSpecPlantChann(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_ALLUSERINFO:
			*pModule->pCode=Svr_SysGetAllUserInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_MW_INFO:
			*pModule->pCode=Svr_SysGetMWSubInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_CONN_STATUS:
			*pModule->pCode=Svr_SysConnStatus(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETPLANTIPOFSUBCORP:
			*pModule->pCode=Svr_SysGetPlantIPofSubCorp(&loFixComm,&loDb);
			break;

		case SERVICE_SYSTEM_GET_CHANNINFO:
			*pModule->pCode=Service_SysGetChannInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_PLANTINFO:
			*pModule->pCode=Service_SysGetPlantInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SPEC_CHANNINFO:
			*pModule->pCode=Service_SysSpecChannInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SPECPLANTINFO:
			*pModule->pCode=Service_SysGetSpecPlantInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
			*pModule->pCode=Service_SysGetMachineInfoType(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL:
			*pModule->pCode=Service_SysGetInfoTypeDetail(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE:
			*pModule->pCode=Service_SysGetInfoTypeDetailBaseType(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO:

			if (!loDb.IsDbSafe())
			{
				*(pModule->pCode) = -311;	

				return false;
			}

			*pModule->pCode=Service_SysModifySpecChannAlarmInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRTEMPLET:
			*pModule->pCode=Service_SysGetSvrTemplet(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM:
			*pModule->pCode=Service_SysGetCustomParam(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET:
			*pModule->pCode=Service_SysGetSvrAlarmTemplet(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRALARMPARAM:
			*pModule->pCode=Service_SysGetSvrAlarmParam(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM:
			*pModule->pCode=Service_SysGetSvrChannAlarmParam(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM:
			*pModule->pCode=Service_SysGetSpecSvrCustomParam(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM:
			*pModule->pCode=Service_SysGetSpecSvrAlarmParam(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL:
			*pModule->pCode=Service_SysGetSetSvrCustomBearingTbl(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL:
			*pModule->pCode=Service_SysGetSetSvrGearBoxTbl(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL:
			*pModule->pCode=Service_SysGetSetSvrLocationTbl(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_SETSVRSECTIONTBL:
			*pModule->pCode=Service_SysGetSetSvrSectionTbl(&loFixComm,&loDb);
			break;

			/* 守护巡检 */
			/* 获取各个程序的路径和版本号 */
		case SERVICE_SYSTEM_GET_MODULE_VERSION:
			*pModule->pCode=Service_SysGetModuleVersion(&loFixComm,&loDb);
			break;
			/* 获取各个数据库状态 */
		case SERVICE_SYSTEM_GET_DB_STATUS:
			*pModule->pCode=Service_SysGetDBStatus(&loFixComm,&loDb);
			break;
			/* 获取中间件运行状态 */
		case SERVICE_SYSTEM_GET_SERVER_STATUS:
			*pModule->pCode=Service_SysGetServerStatus(&loFixComm,&loDb);
			break;
			/* 获取数采硬件状态 */
		case SERVICE_SYSTEM_GET_DAQ_HW_STATUS:
			*pModule->pCode=Service_SysGetDAQHWStatus(&loFixComm,&loDb);
			break;
			/* 获取数采软件状态 */
		case SERVICE_SYSTEM_GET_DAQ_SW_STATUS:
			*pModule->pCode=Service_SysGetDAQSWStatus(&loFixComm,&loDb);
			break;
			/* 获取基本的组态信息 */
		case SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO:
			*pModule->pCode=Service_SysGetBasicConfigInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION:
			*pModule->pCode=Service_SysGetMiddleWareVersion(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX:
			*pModule->pCode=Svr_SysGetPlantIPofSubCorpEx(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_SET_USERINFO:
			*pModule->pCode=Service_UpdateUserInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_GET_PLANT_OF_USER:
			*pModule->pCode = Service_SysGetPlantOfUser(&loFixComm , &loDb );
			break;
		case SERVICE_SYSTEM_GET_USER_OF_PLANT:
			*pModule->pCode = Service_GetUserOfPlant(&loFixComm,&loDb);
			break;
		case SERVICE_SYSTEM_ACTION_USER_PLANT:
			*pModule->pCode = Service_ManageUserPlant(&loFixComm,&loDb);
			break;
		default:
			return false;
		}
	}else
	{
		return false;
	}




	return true;
}


bool DoNext(struct Service_Module * pModule )    //操作函数
{
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //预留函数
{
	return true;
}


