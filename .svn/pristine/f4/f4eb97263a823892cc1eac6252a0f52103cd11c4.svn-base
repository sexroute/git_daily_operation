#include "StdAfx.h"
#include <iostream>
#include "MiddleWare.h"
#include "SysGlobal.h"
#include <fstream>
#include "HandleCollector.h"
#include <additions/CTimeOutReadWriteLock.h>

#include <BaseFunc.h>
#include "Process.h"
#include <IServer.h>
#include "../ICEServer/ICEServerExport.h"
#include "FixDef.h"

using namespace std;

bool g_bInit=false;
int g_iSvrLevel=GE_SRV_TYPE_SET;
MAP_SETIPINFO g_mapSetInfo;
int G_TEMP_ONLINE = 0;


LY::IServerStatus * g_pServerStatus = NULL;
CCriticalSection * g_pServerStatusLock = NULL;

HMODULE GetCurrentModulex()
{ // NB: XP+ solution!
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModulex,
		&hModule);

	return hModule;
}

int RegistModuleComplete()
{
	if (NULL == g_pServerStatusLock)
	{
		g_pServerStatusLock = new CCriticalSection();
	}
	g_pServerStatusLock->Lock();

	g_pServerStatus  = ::GetServerStatus();
	if (NULL != g_pServerStatus)
	{
		CString lstrModuleName = _T("S_OnlineSvr.dll");
		CString    sPath;   

		GetModuleFileName(GetCurrentModulex(),sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 	

		sPath.ReleaseBuffer();
		int    nPos;   
		nPos=sPath.ReverseFind('\\');   
		if (nPos>=0)
		{
			lstrModuleName = sPath.Mid(nPos+1);
		}
		g_pServerStatus->RegistModuleComplete(lstrModuleName.GetBuffer(0),lstrModuleName.GetLength());
	}

	g_pServerStatusLock->Unlock();
	return TRUE;
}


LY::IServer * g_pServer = NULL;
int * g_nServiceMark = 0;
std::map<DWORD,CCustomDB *> * gpDBPool = NULL;
CReaderWriterLock *g_pConnPoolLock = NULL;

CString g_strDbUser;
CString g_strDbPwd;
CString g_strDbName;
CString g_strDbServer;



void GetDBUidPwd(CString &ag_strDbName, CString &ag_strUID, CString &ag_strPwd, CString &ag_strDBServer)
{
	char sUser[1024]={0};
	char sPwd[1024]={0};
	char sEnUser[1024]={0};
	char sEnPwd[1024]={0};
	char sDb[2000]={0};
	char cAppLocation[500]={0};
	char *token;
	GetCurrentDirectory(500,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	//得到用户名和密码
	GetPrivateProfileString("DBINFO","USER","ac6ffedd23f7b1a2cf7735b055c357d0e0a36f899c023262c3d0a8887338f999",sEnUser,1024,cAppLocation);
	GetPrivateProfileString("DBINFO","PWD","b693a448ed4ba85e83d98bbbb9dc14d67885edbc347a",sEnPwd,1024,cAppLocation);
	char key[8]="dse r&d";//密钥
	//解密
	des3_ecb_decrypt(key,sEnUser,sUser);
	des3_ecb_decrypt(key,sEnPwd,sPwd);
	ag_strUID.Format("%s",sUser);
	ag_strPwd.Format("%s",sPwd);
	//得到数据库服务器名和数据库名
	GetPrivateProfileString("SERVER","SERVER1","监测中心,dse,DSE,DSE",sDb,2000,cAppLocation);
	char seps[]=", ";
	token = strtok( sDb, seps );
	for(int iloop=0; token != NULL;iloop++)
	{
		/* While there are tokens in "string" */
		if (iloop==1)
		{
			ag_strDBServer.Format("%s",token);
			token = strtok( NULL, seps );
			continue;
		}
		if (iloop==2)
		{
			ag_strDbName.Format("%s",token);
			token = strtok( NULL, seps );
			continue;
		}
		if (iloop==3)
		{
			break;
		}
		/* Get next token: */
		token = strtok( NULL, seps );
	}
	//得到是否切换数据库的标识
	g_iDbType = GetPrivateProfileInt("DBINFO","DBTYPE",0x0000,cAppLocation);
	if (g_iDbType!=0 && g_iDbType!=1)
		CHZLogManage::Error("配置数据库的类型有误","OnlineGlobal","GetMainDBName");
	CString sTemp;sTemp.Format("得到全局的数据库类型标识,%d",g_iDbType);
	CHZLogManage::Debug(sTemp,"OnlineGlobal","GetMainDBName");
}

CCustomDB * CreateHandleDataDBConn()
{
	CCustomDB *pDhDB=new CCustomDB;
	CString strserver,strdb,struser,strpwd;
	GetDBUidPwd(strdb,struser,strpwd,strserver);
	g_strDbServer=strserver;
	g_strDbName=strdb;
	g_strDbUser=struser;
	g_strDbPwd=strpwd;
	g_strMainDbName=g_strDbName;//得到全局的主数据库名
	CString sTemp;sTemp.Format("得到全局的主数据库名:%s",g_strMainDbName);
	CHZLogManage::Debug(sTemp,"OnlineGlobal","CreateHandleDataDBConn");
	pDhDB->Open(strserver,struser,strpwd,strdb);
	pDhDB->SetSqlLogPath(".\\log");
	return pDhDB;
}

CCustomDB * GetPDB()
{
	CCustomDB * lpDb = NULL;

	DWORD ldwThreadID = ::GetCurrentThreadId();

	g_pConnPoolLock->AcquireReaderLock();
	BEGIN_ERROR_HANDLE;

	std::map<DWORD,CCustomDB *>::iterator lpValue = gpDBPool->find(ldwThreadID);	
	if (lpValue != gpDBPool->end())
	{
		lpDb = lpValue->second;
	}

	END_ERROR_HANDLE;
	g_pConnPoolLock->ReleaseReaderLock();

	if (lpDb == NULL)
	{
		g_pConnPoolLock->AcquireWriterLock();		
		BEGIN_ERROR_HANDLE;

		std::map<DWORD,CCustomDB *>::iterator  lpValue = gpDBPool->find(ldwThreadID);
		if (lpValue == gpDBPool->end())
		{
			lpDb = CreateHandleDataDBConn();
			gpDBPool->insert(make_pair(ldwThreadID,lpDb));
		}

		END_ERROR_HANDLE;
		g_pConnPoolLock->ReleaseWriterLock();
	}

	return lpDb;
}

UINT IceService(int SvcCode,ICommInterFace & arefCommInterFace)
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

	switch ( SvcCode  )
	{
		//中间件模块
		//得到所有振动测点的五种类型的特征值
		//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
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


	COleSqlServerDB loDb;

	if (!loDb.IsDbSafe())
	{
		return -1;
	}

	CCustomDB * lpDb = GetPDB();
	loDb.setPDB(lpDb);

	int lnCode = 0;
	switch ( SvcCode )
	{		//中间件模块
	case  SERVICE_SYSTEM_GETPLANTSUDCHANN:
		lnCode= Service_SysGetPlantSudChann(&arefCommInterFace , &loDb );
		break;
		//得到指定分厂下所有设备位号,通过循环依次得到所有设备位号
	case SERVICE_SYSTEM_GETPLANTOFSUBCORP:
		lnCode = Service_SysGetPlantOfSubCorp (&arefCommInterFace , &loDb );
		break;
		//得到指定设备位号的所有测点名和对应的测点类型，通过循环依次得到
	case SERVICE_SYSTEM_GETALLCHANN:
		lnCode = Service_SysGetAllChann(&arefCommInterFace , &loDb );
		break;
		//通过用户名和密码得到用户可操作的所有设备，通过循环依次得到所有设备位号
	case SERVICE_SYSTEM_GETCOMPANYTOFUSER:
		lnCode=Service_SysGetCompanyOfUser(&arefCommInterFace , &loDb );
		break;
	case SERVICE_SYSTEM_GETALLAXISCHANN:
		lnCode= Service_SysGetAllAxisChann(&arefCommInterFace,&loDb );
		break;
	case SERVICE_SYSTEM_GETPLANTANALYSISLINES:
		lnCode=Service_SysGetPlantAnalysislines(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETPLANTINFO:
		lnCode=Srv_SysGetPlantInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETALLFACTORY:
		lnCode=Srv_SysGetAllFactory(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETALLCOMPANY:
		lnCode=Srv_SysGetAllCompany(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_MODIFYPWD:
		lnCode=Service_SysModifyPwd(&arefCommInterFace , &loDb);
		break;
	case SERVICE_SYSTEM_GETCHARTYPEDESC:
		lnCode=Srv_SysGetCharTypeDesc(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_REFRESHTIME:
		lnCode=Service_SysGetRefreshTime(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETALARMTHRESHOLD:
		lnCode=Service_SysGetThreshold(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_LOGIN:
		lnCode=Service_SysLogin(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_MODIFY_USERLEVEL:
		lnCode=Service_SysModifyUserLevel(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_ADD_NEWUSER:
		lnCode=Service_SysAddNewUser(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_SETCHECKALARMFLAG:
		lnCode=Service_SysSetCheckAlarmFlag(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID:
		lnCode=Service_SysGetGraphTypeByDeviceID(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_MOIDFY_MWINFO:
		lnCode=Svr_SysModifyMwInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_MWINFO:
		lnCode=Svr_SysGetMwInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_MODIFY_SENDINFO:
		lnCode=Svr_SysModifySendInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SENDINFO:
		lnCode=Svr_SysGetSendInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRINFO:
		lnCode=Svr_SysGetSetSvrInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRPLANTINFO:
		lnCode=Svr_SysGetSetSvrPlantInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN:
		lnCode=Svr_SysGetSetSvrSpecPlantChann(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_ALLUSERINFO:
		lnCode=Svr_SysGetAllUserInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_MW_INFO:
		lnCode=Svr_SysGetMWSubInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_CONN_STATUS:
		lnCode=Svr_SysConnStatus(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETPLANTIPOFSUBCORP:
		lnCode=Svr_SysGetPlantIPofSubCorp(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SYSTEM_GET_CHANNINFO:
		lnCode=Service_SysGetChannInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_PLANTINFO:
		lnCode=Service_SysGetPlantInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SPEC_CHANNINFO:
		lnCode=Service_SysSpecChannInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SPECPLANTINFO:
		lnCode=Service_SysGetSpecPlantInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
		lnCode=Service_SysGetMachineInfoType(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL:
		lnCode=Service_SysGetInfoTypeDetail(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE:
		lnCode=Service_SysGetInfoTypeDetailBaseType(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO:

		if (!loDb.IsDbSafe())
		{
			lnCode = -311;	
			return false;
		}

		lnCode=Service_SysModifySpecChannAlarmInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRTEMPLET:
		lnCode=Service_SysGetSvrTemplet(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM:
		lnCode=Service_SysGetCustomParam(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET:
		lnCode=Service_SysGetSvrAlarmTemplet(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRALARMPARAM:
		lnCode=Service_SysGetSvrAlarmParam(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM:
		lnCode=Service_SysGetSvrChannAlarmParam(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM:
		lnCode=Service_SysGetSpecSvrCustomParam(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM:
		lnCode=Service_SysGetSpecSvrAlarmParam(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL:
		lnCode=Service_SysGetSetSvrCustomBearingTbl(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL:
		lnCode=Service_SysGetSetSvrGearBoxTbl(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL:
		lnCode=Service_SysGetSetSvrLocationTbl(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_SETSVRSECTIONTBL:
		lnCode=Service_SysGetSetSvrSectionTbl(&arefCommInterFace,&loDb);
		break;

		/* 守护巡检 */
		/* 获取各个程序的路径和版本号 */
	case SERVICE_SYSTEM_GET_MODULE_VERSION:
		lnCode=Service_SysGetModuleVersion(&arefCommInterFace,&loDb);
		break;
		/* 获取各个数据库状态 */
	case SERVICE_SYSTEM_GET_DB_STATUS:
		lnCode=Service_SysGetDBStatus(&arefCommInterFace,&loDb);
		break;
		/* 获取中间件运行状态 */
	case SERVICE_SYSTEM_GET_SERVER_STATUS:
		lnCode=Service_SysGetServerStatus(&arefCommInterFace,&loDb);
		break;
		/* 获取数采硬件状态 */
	case SERVICE_SYSTEM_GET_DAQ_HW_STATUS:
		lnCode=Service_SysGetDAQHWStatus(&arefCommInterFace,&loDb);
		break;
		/* 获取数采软件状态 */
	case SERVICE_SYSTEM_GET_DAQ_SW_STATUS:
		lnCode=Service_SysGetDAQSWStatus(&arefCommInterFace,&loDb);
		break;
		/* 获取基本的组态信息 */
	case SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO:
		lnCode=Service_SysGetBasicConfigInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION:
		lnCode=Service_SysGetMiddleWareVersion(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX:
		lnCode=Svr_SysGetPlantIPofSubCorpEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_SET_USERINFO:
		lnCode=Service_UpdateUserInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_GET_PLANT_OF_USER:
		lnCode = Service_SysGetPlantOfUser(&arefCommInterFace , &loDb );
		break;
	case SERVICE_SYSTEM_GET_USER_OF_PLANT:
		lnCode = Service_GetUserOfPlant(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SYSTEM_ACTION_USER_PLANT:
		lnCode = Service_ManageUserPlant(&arefCommInterFace,&loDb);
		break;
	default:
		return false;
	}

	return lnCode;

}


int InitICEServer()
{
	gpDBPool = new std::map<DWORD,CCustomDB *>();
	//1.get Server
	g_pServer = ::GetServer();

	if (NULL== g_pServer)
	{
		return FALSE;
	}

	g_pServer->Init();
	g_pServer->Start();

	if (!g_pConnPoolLock)
	{
		g_pConnPoolLock = new CReaderWriterLock(); 
	}

	//2.registServer
	g_nServiceMark = new int();
	g_pServer->RegServiceCallBackHandler((int)g_nServiceMark,IceService);

	return FALSE;
}



BOOL IsTempMode()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	//开启停车仅保存月表策略
	int lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_TEMP_ONLINE"
		,G_TEMP_ONLINE,
		cAppLocation);

	if (lnData<=0)
	{
		lnData = ::GetPrivateProfileInt("SYSTEM",
			"G_ONLY_SAVE_MONTH_STOP"
			,G_TEMP_ONLINE,
			cAppLocation);

		if (lnData<0)
		{
			lnData = 0;
		}
	}

	CString lstrData;
	lstrData.Format(_T("%d"),lnData);

	::WritePrivateProfileString("SYSTEM",
		"G_TEMP_ONLINE"
		,lstrData,
		cAppLocation);

	G_TEMP_ONLINE = lnData;
	BHLOG_DEBUG(STR(_T("临时在线模式: %d"),G_TEMP_ONLINE));
	return G_TEMP_ONLINE;
}

int GetCompanyDBID(CCustomDB *pDB_,string sCompany_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int iRtn=0;
	pDB_->Cmd("select [%s] from [%s] ",gc_cDB_ID,gc_cCompanyTable);
	pDB_->Cmd(" where [%s]='%s'",gc_cCompany,sCompany_.c_str());
	if (pDB_->More())
	{	
		iRtn=pDB_->GetLong(gc_cDB_ID);
	}
	return iRtn;
}

std::map<CString,FactoryDataTypeEx> g_FactoryData;

int GetPlantData(CString astrGroup, CString astrCompany,CString astrFactory,CCustomDB *pDB)
{
	SwichDbName(pDB,g_strMainDbName);

	CString lstrKey = astrCompany +_T(":") + astrFactory;
	list<S_PlantInfoEx> & lPlantInfo = g_FactoryData[lstrKey];
	string sCompany	=	astrCompany.GetBuffer(0);
	string sFactory	=	astrFactory.GetBuffer(0);
	S_PlantInfo plantInfo;
	S_PlantInfoEx plantInfoEx;
	int iDbID=GetCompanyDBID(pDB,sCompany);
	if (iDbID>0)
		pDB->Cmd("select * from [%s_%d] ",gc_cMachineTable,iDbID);
	else
		pDB->Cmd("select * from [%s] ",gc_cMachineTable);
	pDB->Cmd(" where [%s]='%s' ",gc_cCompany,sCompany.c_str());
	if(!(sFactory.empty()))
		pDB->Cmd(" AND [%s]='%s' ",gc_cFactory_Name,sFactory.c_str());
	pDB->Cmd(" order by [%s]",gc_cSetID);
	while (pDB->More())
	{
		plantInfo.m_nDbId = iDbID;
		plantInfo.GetPlantInfo(pDB);
		plantInfoEx._plantInfo = plantInfo;
		plantInfoEx._plantInfo.m_nDbId = iDbID;
		plantInfoEx.m_strGroup = astrGroup;
		plantInfoEx.m_strPlantDisplayName = pDB->Get(gc_cDisplay);
		plantInfoEx.m_strPlantDisplayName.Trim();
		plantInfoEx.m_strErpCode = pDB->Get(gc_cErpCode);
		plantInfoEx.m_strErpCode.Trim();
		if (iDbID > 0)
		{
			plantInfoEx.m_DeviceId = pDB->GetLong(gc_cID);
		}
		else
		{
			plantInfoEx.m_DeviceId = -1;
		}
		
		lPlantInfo.push_back(plantInfoEx);
	}

	return lPlantInfo.size();
}

typedef struct _COMPANY_INFO
{
	CString m_strGroup;
	CString m_strCompany;
	int m_nDBID;
	_COMPANY_INFO()
	{
		this->m_nDBID = 0;
	}
} COMPANY_INFO, * LP_COMPANY_INFO;

int InitPlantInfoData(CCustomDB *pDB)
{
	SwichDbName(pDB,g_strMainDbName);

	pDB->Cmd(" SELECT * from [%s] ",gc_cCompanyTable);

	std::vector<COMPANY_INFO> loCompanys;

	CString lstrGroup;

	while (pDB->More())
	{
		lstrGroup = pDB->Get(gc_cGroup);

		CString lstrCompany = pDB->Get(gc_cCompany);

		COMPANY_INFO loCompany;

		loCompany.m_strGroup = lstrGroup;

		loCompany.m_strCompany = lstrCompany;

		loCompany.m_nDBID = pDB->GetLong(gc_cDB_ID);

		loCompanys.push_back(loCompany);
	}

	for (int i =0;i<loCompanys.size();i++)
	{
		COMPANY_INFO & loCompany = loCompanys.at(i);

		if (0 == loCompany.m_nDBID)
		{
			pDB->Cmd("SELECT distinct [%s] from [%s]",gc_cFactory_Name,gc_cMachineTable);

		}
		else
		{
			pDB->Cmd("SELECT distinct [%s] from [%s_%d]",gc_cFactory_Name,gc_cMachineTable,loCompany.m_nDBID);

		}

		pDB->Cmd(" WHERE [%s] ='%s'", gc_cCompany, loCompany.m_strCompany);

		std::vector<CString> loFactorys;

		while(pDB->More())
		{
			CString lstrFactory = pDB->Get(gc_cFactory_Name);

			loFactorys.push_back(lstrFactory);
		}

		for (int j=0;j<loFactorys.size();j++)
		{
			GetPlantData(loCompany.m_strGroup,loCompany.m_strCompany,loFactorys.at(j),pDB);
		}
	}

	return g_FactoryData.size();
}

FactoryDataTypeEx g_ListPlantInfoEx;

BOOL SettingsTableGetKV(CCustomDB *pDB, CString lsKey )
{
	if (!SwichDbName(pDB, g_strMainDbName))
	{
		ASSERT(FALSE);
		return false;
	}

	BOOL lbVisible = FALSE;

	pDB->Cmd("SELECT [%s] FROM %s WHERE [%s]='%s'",
		gc_cValue,
		gc_cSettingTable,
		gc_cKey, lsKey);
	if (pDB->More())
	{
		lbVisible = ::atoi(pDB->Get(gc_cValue));
	}

	return lbVisible;
}

#define ENABLE_INDIVIDUAL_VISIBILITY_SETTING 0
int InitPlantInfo(CCustomDB *pDB)
{
	SwichDbName(pDB,g_strMainDbName);

	bool lbGlobalGroupVisible   = TRUE;
	bool lbGlobalFactoryVisible = TRUE;
	bool lbGlobalCompanyVisible = TRUE;
	bool lbGlobalSetVisible     = FALSE;
	bool lbGlobalPlantVisible   = TRUE;


	bool bExistVisibleSetting = false;

	pDB->Cmd("select 1 from sysobjects where id=OBJECT_ID('settings') and type='U'");
	if (pDB->More())
	{
		bExistVisibleSetting = true;
	}

	if (bExistVisibleSetting)
	{
		lbGlobalGroupVisible   = SettingsTableGetKV(pDB, gc_cShowGroup);
		lbGlobalFactoryVisible = SettingsTableGetKV(pDB, gc_cShowFactory);
		lbGlobalCompanyVisible = SettingsTableGetKV(pDB, gc_cShowCompany);
		lbGlobalSetVisible     = SettingsTableGetKV(pDB, gc_cShowSet);
		lbGlobalPlantVisible   = SettingsTableGetKV(pDB, gc_cShowPlant);
	}

	pDB->Cmd(" SELECT a.[%s] as MACHINE_SORTNO, a.[%s] as MACHINE_DISPLAYNAME,* from [%s] a,[%s] b where a.[%s] = b.[%s] ", gc_cSortNO,gc_cDisplay,gc_cMachineTable,gc_cCompanyTable,gc_cCompany,gc_cCompany);

	while (pDB->More())
	{
		S_PlantInfoEx sPlantInfo;

		sPlantInfo._bGroupVisible   = lbGlobalGroupVisible;
		sPlantInfo._bFactoryVisible = lbGlobalFactoryVisible;
		sPlantInfo._bCompanyVisible = lbGlobalCompanyVisible;
		sPlantInfo._bSetVisible     = lbGlobalSetVisible;
		sPlantInfo._bPlantVisible   = lbGlobalPlantVisible;

		CString lstrContry;
		BEGIN_ERROR_HANDLE;		
		lstrContry     = pDB->Get(gc_cCountry);
		END_ERROR_HANDLE;
		if (!lstrContry.IsEmpty())
		{
			sPlantInfo.m_strContry = lstrContry;
		}
#if ENABLE_INDIVIDUAL_VISIBILITY_SETTING
		if (bExistVisibleSetting)
		{
			if (lbGlobalPlantVisible)
			{
				sPlantInfo._bPlantVisible   = pDB->GetLong(gc_cVisible);
			}
		}
#endif

		sPlantInfo.m_strGroup = pDB->Get(gc_cGroup);
		
		sPlantInfo.m_strPlantDisplayName = pDB->Get("MACHINE_DISPLAYNAME");
		sPlantInfo.m_strPlantDisplayName.Trim();

		sPlantInfo.m_strErpCode = pDB->Get(gc_cErpCode);
		sPlantInfo.m_strErpCode.Trim();

		sPlantInfo.m_DeviceId = pDB->GetLong(gc_cID);

		sPlantInfo.m_nPlantSortNO = pDB->GetLong("MACHINE_SORTNO");

		sPlantInfo.m_strCompanyAlias = pDB->Get(gc_cCompanyAlias);

		if(sPlantInfo._plantInfo.GetPlantInfo(pDB))
		{
			g_ListPlantInfoEx.push_back(sPlantInfo);
		}
	}

	for (FactoryDataTypeEx::iterator it = g_ListPlantInfoEx.begin();
		it != g_ListPlantInfoEx.end();
		it++)
	{
		S_PlantInfo & plantInfo = it->_plantInfo;
		
		//从settable中获取装置显示名
		pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s' AND %s='%s' AND %s='%s'", 
			gc_cDisplay,
			gc_cSortNO,
			gc_cSetTable,
			gc_cCompany, plantInfo._cCompany,
			gc_cFactory_Name, plantInfo._cFactory,
			gc_cSetID, plantInfo._cSetID);

		if (pDB->More())
		{
			it->m_strSetDisplayName = pDB->Get(gc_cDisplay);
			it->m_nSetSortNO        = pDB->GetLong(gc_cSortNO);
		}

		pDB->Cmd("SELECT %s,%s FROM %s WHERE [%s]='%s'", 
			gc_cDisplay,
			gc_cSortNO,
			gc_cGroupTable,
			gc_cGroup, it->m_strGroup);
		if (pDB->More())
		{
			it->m_strGroupDisplayName = pDB->Get(gc_cDisplay);
			it->m_nGroupSortNO = pDB->GetLong(gc_cSortNO);
		}

		pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s'", 
			gc_cDisplay,
			gc_cSortNO,
			gc_cCompanyTable,
			gc_cCompany, plantInfo._cCompany);
		if (pDB->More())
		{
			it->m_strCompanyDisplayName = pDB->Get(gc_cDisplay);
			it->m_nCompanySortNO = pDB->GetLong(gc_cSortNO);
		}

		pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s' AND %s='%s'", 
			gc_cDisplay,
			gc_cSortNO,
			gc_cFactoryTable,
			gc_cCompany, plantInfo._cCompany,
			gc_cFactory_Name, plantInfo._cFactory);
		if (pDB->More())
		{
			it->m_strFactoryDisplayName = pDB->Get(gc_cDisplay);
			it->m_nFactorySortNO = pDB->GetLong(gc_cSortNO);
		}
	}


#if ENABLE_INDIVIDUAL_VISIBILITY_SETTING
	if (bExistVisibleSetting)
	{
		for (FactoryDataTypeEx::iterator it = g_ListPlantInfoEx.begin();
			it != g_ListPlantInfoEx.end();
			it++)
		{
			//从settable中获取装置显示名
			pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s' AND %s='%s' AND %s='%s'", 
				gc_cDisplay,
				gc_cSortNO,
				gc_cSetTable,
				gc_cCompany, plantInfo._cCompany,
				gc_cFactory_Name, plantInfo._cFactory,
				gc_cSetID, plantInfo._cSetID);
			
			if (pDB->More())
			{
				it->m_strSetDisplayName = pDB->Get(gc_cDisplay);
				it->m_nSetSortNO        = pDB->GetLong(gc_cSortNO);
			}

			S_PlantInfo & plantInfo = it->_plantInfo;

			if (lbGlobalCompanyVisible)
			{
				pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s'", 
					gc_cVisible,
					gc_cSortNO,
					gc_cCompanyTable,
					gc_cCompany, plantInfo._cCompany);
				if (pDB->More())
				{
					it->_bCompanyVisible = pDB->GetLong(gc_cVisible);

					it->m_nCompanySortNO = pDB->GetLong(gc_cSortNO);
				}
			}

			if (lbGlobalFactoryVisible)
			{
				pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' AND %s='%s'", 
					gc_cVisible,
					gc_cFactoryTable,
					gc_cCompany, plantInfo._cCompany,
					gc_cFactory_Name, plantInfo._cFactory);
				if (pDB->More())
				{
					it->_bFactoryVisible = pDB->GetLong(gc_cVisible);
				}
			}

			if (lbGlobalSetVisible)
			{
				pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' AND %s='%s' AND %s='%s'", 
					gc_cVisible,
					gc_cSetTable,
					gc_cCompany, plantInfo._cCompany,
					gc_cFactory_Name, plantInfo._cFactory,
					gc_cSetID, plantInfo._cSetID);
				if (pDB->More())
				{
					it->_bSetVisible = pDB->GetLong(gc_cVisible);
				}
			}
		}
	}
#endif /* ENABLE_INDIVIDUAL_VISIBILITY_SETTING */

	return g_ListPlantInfoEx.size();
}


int g_IServerLevel = 0;


int CheckSetSvrLevel(CCustomDB *pDB_)
{
	SwichDbName(pDB_,g_strMainDbName);
	int iRtn=GE_SRV_TYPE_SET;
	pDB_->Cmd(" select DISTINCT [%s] from [%s] ",gc_cID,gc_cSetTable);
	if (pDB_->More())
	{
		int nCount=pDB_->GetCount();
		switch(nCount)
		{
		case 1:
			iRtn=GE_SRV_TYPE_SET;
			break;
		case 2:
			iRtn=GE_SRV_TYPE_COMPANY;
			break;
		case 3:
			iRtn=GE_SRV_TYPE_GROUP;
			break;
		}
	}
	return iRtn;
}

SET_DATA_TYPE g_SET_DATA;

int InitSetCache(CCustomDB * pDB_,int anServerLevel)
{
	SwichDbName(pDB_,g_strMainDbName);
	pDB_->Cmd("select * from [%s]",gc_cSetTable);
	pDB_->Cmd("  where [%s]=%d ",gc_cID,anServerLevel);
	int nCount = 0;
	while (pDB_->More())
	{
		SET_INFO loInfo;
		nCount++;
		loInfo.m_strSetID = pDB_->Get(gc_cSetID);
		loInfo.m_strSetIP = pDB_->Get(gc_cCenterMw_IP);
		loInfo.m_nSetPort = pDB_->GetLong(gc_cCenterMw_Port);
		loInfo.m_nLevel   = (int)anServerLevel;
		g_SET_DATA.push_back(loInfo);
	}

	return nCount;
}

BOOL SetGlobalErrorMode(
						DWORD dwErrorMode   // specifies new ErrorMode value
						)
{
	HKEY hKey;
	LONG lRetCode;

	// make sure the value passed isn't out-of-bounds
	if (dwErrorMode > 2) return FALSE;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"SYSTEM\\CurrentControlSet\\Control\\Windows",
		0,
		KEY_SET_VALUE,
		&hKey) != ERROR_SUCCESS) return FALSE;

	lRetCode=RegSetValueEx(hKey,
		"ErrorMode",
		0,
		REG_DWORD,
		(CONST BYTE *) &dwErrorMode,
		sizeof(DWORD) );


	lRetCode=RegSetValueEx(hKey,
		"ShellErrorMode",
		0,
		REG_DWORD,
		(CONST BYTE *) &dwErrorMode,
		sizeof(DWORD) );

	RegCloseKey(hKey);

	if (lRetCode != ERROR_SUCCESS) return FALSE;

	return TRUE;
}

#include "mdump.h"
#include "additions/RemoteServer.h"
int InitGlobalVar(CCustomDB *pDB_)
{
	CRemoteSesrver::DisableStandardAbort71();
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
	SetGlobalErrorMode(2);
	::SetUnhandledExceptionFilter(Winet::GEHExceptionFilter);
	int nCount=0;
	g_mapSetInfo.clear();
	CSetIpDetailInfo setIpInfo;
	CString strInfo;
	int iLogLevel=GetLogOutputFlag(strInfo);
	CHZLogManage::StartLogTxt( strInfo, "KDMW", iLogLevel);
	GetMainDBName();
	//得到中间件服务器级别
	SwichDbName(pDB_,g_strMainDbName);

	InitPlantInfoData(pDB_);

	InitPlantInfo(pDB_);

	IsTempMode();

	CConfigHelper::GetAllPlantFireInfo();

	g_IServerLevel = CheckSetSvrLevel(pDB_);

	InitSetCache(pDB_,g_IServerLevel);

	pDB_->Cmd("select max([%s]) as [%s] from [%s]",gc_cID,gc_cID,gc_cSetTable);
	if(pDB_->More())
	{
		nCount=pDB_->GetLong(gc_cID);
		if(nCount>=2)
			g_iSvrLevel=GE_SRV_TYPE_GROUP;
		else if(nCount==1)
			g_iSvrLevel=GE_SRV_TYPE_COMPANY;
		else
			g_iSvrLevel=GE_SRV_TYPE_SET;
	}
	string sTmp;
	//查询有客户端链接时涉及几个中间件服务器
	list<int> lSvrID;
	int iDbID=0;
	lSvrID.clear();
	pDB_->Cmd("select distinct([%s]) from [%s] ",gc_cDB_ID,gc_cCompanyTable);
	while (pDB_->More())
	{
		iDbID=pDB_->GetLong(gc_cDB_ID);
		lSvrID.push_back(iDbID);
	}
	//得到所有中间件ID信息
	list<int>::iterator itDbID;
	for (itDbID=lSvrID.begin();itDbID!=lSvrID.end();++itDbID)
	{
		iDbID=(*itDbID);
		if(iDbID>0)
			pDB_->Cmd("select * from [%s_%d] ",gc_cSetTable,iDbID);
		else
			pDB_->Cmd("select * from [%s] ",gc_cSetTable);
		while (pDB_->More())
		{
			setIpInfo.ReadSetInfo(pDB_);
			sTmp=setIpInfo.GetSetID();
			CString lstrSetID;
			lstrSetID.Format(_T("%s"),sTmp.c_str());
			BHLOG_DEBUG(lstrSetID);
			g_mapSetInfo.insert(make_pair(sTmp,setIpInfo));
			//g_mapSetInfo[sTmp]=setIpInfo;
		}
	}

	if(g_iSvrLevel!=GE_SRV_TYPE_SET)
	{
		MAP_SETIPINFO::iterator itMap1;
		MAP_SETIPINFO::iterator itMap2;
		string sParentSetID="";
		for (itMap1=g_mapSetInfo.begin();itMap1!=g_mapSetInfo.end();++itMap1)
		{
			if((itMap1->second.GetID())!=0)
			{
				sParentSetID=itMap1->second.GetParentSetID();
				itMap2=g_mapSetInfo.find(sParentSetID);
				//			  CString strTmp;strTmp.Format("%s,%s",sParentSetID.c_str(),itMap2->second.GetParentSetID().c_str());  AfxMessageBox(strTmp);
				if (itMap2!=g_mapSetInfo.end())
				{
					itMap1->second.SetParentIP(itMap2->second.GetSetIP());
					itMap1->second.SetParentPort(itMap2->second.GetSetPort());
				}
			}
		}
	}
	nCount=g_mapSetInfo.size();

	UserManager::Initialize(pDB_);

	verifyDAQHWStautsFile();

	verifyDAQSWStautsFile();

	RegistModuleComplete();

	InitICEServer();

	return nCount;
}



void QuitClearGlobalVar()
{
	CHandleCollector::StopTrack();
	g_mapSetInfo.clear();
}

//从ini文件中得到数据库服务器名，数据库名
void GetMainDBName()
{
	char sUser[1024]={0};
	char sPwd[1024]={0};
	char sEnUser[1024]={0};
	char sEnPwd[1024]={0};
	char sDb[2000]={0};
	char cAppLocation[500]={0};
	char *token;
	GetCurrentDirectory(500,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	//得到数据库服务器名和数据库名
	GetPrivateProfileString("SERVER","SERVER1","监测中心,dse,DSE,DSE",sDb,2000,cAppLocation);
	char seps[]=", ";
	token = strtok( sDb, seps );
	for(int iloop=0; token != NULL;iloop++)
	{
		/* While there are tokens in "string" */
		if (iloop==2)
		{
			g_strMainDbName.Format("%s",token);//得到全局的主数据库名
			token = strtok( NULL, seps );
			continue;
		}
		if (iloop==3)
		{
			break;
		}
		/* Get next token: */
		token = strtok( NULL, seps );
	}
	//得到是否切换数据库的标识
	g_iDbType = GetPrivateProfileInt("DBINFO","DBTYPE",0x0000,cAppLocation);
	if (g_iDbType!=0 && g_iDbType!=1)
		CHZLogManage::Error("配置数据库的类型有误","SysGlobal","GetMainDBName");
	CString sTemp;sTemp.Format("得到全局的主数据库名和数据库类型标识:%s,%d",g_strMainDbName,g_iDbType);
	CHZLogManage::Debug(sTemp,"SysGlobal","GetMainDBName");
}

///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_)
{
	char cAppLocation[500]={0};
	char sLogPath[1024]={0};
	GetCurrentDirectory(500,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	GetPrivateProfileString("SYSTEM","LOGPATH",".\\log",sLogPath,1024,cAppLocation);
	strLogPath_="";
	strLogPath_.Format("%s",sLogPath);
	//得到用户名和密码
	return GetPrivateProfileInt("LOG","KDLOG",0x0040,cAppLocation);
}


//调试之用函数
int writefile(float * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("c:\\wave%d.txt",index);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

int writefile(double * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("c:\\wave%d.txt",index);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("c:\\wave%d.txt",index);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

template<typename TLock>
class CSafeReaderLock
{
public:
	explicit CSafeReaderLock(TLock& lock)
		: _Lock (lock)
	{
		_Lock.BeginRead();
	}

	~CSafeReaderLock()
	{
		_Lock.EndRead();
	}

private:
	TLock & _Lock;

	CSafeReaderLock();
	CSafeReaderLock(const CSafeReaderLock &);
	CSafeReaderLock & operator=(const CSafeReaderLock &);
};

template<typename TLock>
class CSafeWriterLock
{
public:
	explicit CSafeWriterLock(TLock& lock)
		: _Lock (lock)
	{
		_Lock.BeginWrite();
	}

	~CSafeWriterLock()
	{
		_Lock.EndWrite();
	}

private:
	TLock & _Lock;

	CSafeWriterLock();
	CSafeWriterLock(const CSafeWriterLock &);
	CSafeWriterLock & operator=(const CSafeWriterLock &);
};

static CTimeOutReadWriteLock g_UserManagement_lock;

template<class _Container>
class set_insert_iterator
	: public _Outit
{	// wrap inserts into container as output iterator
public:
	typedef _Container container_type;
	typedef typename _Container::reference reference;

	set_insert_iterator(_Container& _Cont)
		: container(&_Cont)
	{	// construct with container and iterator
	}

	set_insert_iterator<_Container>& operator=(
		typename _Container::const_reference _Val)
	{	// insert into container and increment stored iterator
		container->insert(_Val);
		return (*this);
	}

	set_insert_iterator<_Container>& operator*()
	{	// pretend to return designated value
		return (*this);
	}

	set_insert_iterator<_Container>& operator++()
	{	// pretend to preincrement
		return (*this);
	}

	set_insert_iterator<_Container>& operator++(int)
	{	// pretend to postincrement
		return (*this);
	}

protected:
	_Container *container;	// pointer to container
};

// TEMPLATE FUNCTION inserter
template<class _Container> inline
set_insert_iterator<_Container> set_inserter(_Container& _Cont)
{	// return insert_iterator
	return (set_insert_iterator<_Container>(_Cont));
}

/*ID in [1, +oo] */

/* 按sep 拆分子串
  strtok, CStringT.Tokenize/CString.SpanExclude只能按字符串中的一个分割 
  AfxExtractSubString只能按单字符分割
*/
IdSet_t IdSet_FromString(CString csv, const TCHAR * sep/* = _T(",")*/)
{
	IdSet_t loIdSet;

	int buffer_len = csv.GetLength();

	if (buffer_len < 1)
	{
		return loIdSet;
	}

	int sep_len = strlen(sep);

	if (sep_len < 1)
	{
		return loIdSet;
	}

	int id = -1;

	char* buffer   = csv.GetBuffer();

	char* sentinel = buffer + buffer_len;

	char *sep_start = 0;
	while (buffer < sentinel)
	{
		sep_start = strstr(buffer, sep);

		if (sep_start)
		{
			*sep_start = 0;
		}
		else
		{
			sep_start = sentinel;
		}

		id = atoi(buffer);
		if (id > 0)
		{
			loIdSet.insert(id);
		}

		buffer = sep_start + sep_len;
	}

	return loIdSet;
}

CString IdSet_ToString(const IdSet_t& self, const TCHAR * sep/* = _T(",")*/)
{
	CString csv;

	IdSet_t::const_iterator it = self.begin();
	if (it == self.end())
	{
		return csv;
	}

	csv.Format("%d", *it);

	++it;

	while(it != self.end())
	{
		csv.AppendFormat("%s%d", sep, *it);
		++it;
	}

	return csv;
}

IdSet_t IdSet_Intersection(const IdSet_t& first, const IdSet_t& second)
{
	IdSet_t result;

	std::set_intersection(first.begin(), first.end(),
		second.begin(), second.end(),
		set_inserter(result));

	return result;
}

IdSet_t IdSet_Difference(const IdSet_t& first, const IdSet_t& second)
{
	IdSet_t result;

	std::set_difference(first.begin(), first.end(),
		second.begin(), second.end(),
		set_inserter(result));

	return result;
}

IdSet_t IdSet_Union(const IdSet_t& first, const IdSet_t& second)
{
	IdSet_t result;

	std::set_union(first.begin(), first.end(),
		second.begin(), second.end(),
		set_inserter(result));

	return result;
}




CString UserInfoList_ToString( const UserInfoList_t& self, const TCHAR * sep/* = _T(",")*/ )
{
	CString csv;

	UserInfoList_t::const_iterator it = self.begin();
	if (it == self.end())
	{
		return csv;
	}

	const UserInfo_t& loUserInfo = *it;

	csv.Format("%s", loUserInfo.UserName);

	++it;

	while(it != self.end())
	{
		const UserInfo_t& loUserInfo = *it;

		csv.AppendFormat("%s%s", sep, loUserInfo.UserName);
		++it;
	}

	return csv;
}

typedef unsigned int Fnv32_t;

Fnv32_t fnv1a_hash_32(const char *str)
{
	Fnv32_t hval = ((Fnv32_t)0x811c9dc5);

	unsigned char *s = (unsigned char *)str;

	while (*s) {

		hval ^= (Fnv32_t)*s++;

#if 0
		hval *= ((Fnv32_t)0x01000193);
#else
		hval += (hval<<1) + (hval<<4) + (hval<<7) + (hval<<8) + (hval<<24);
#endif
	}

	return hval;
}



// { user_id: user_info }
typedef std::map<int, UserInfo_t>  UserTableCache_t;
//{ machine_id: plant_info }
typedef std::map<int, PlantInfo_t> MachineTableCache_t;

//{ user_id: set([ machine_id ]) }
typedef std::map<int, IdSet_t>  UserPlantMap_t;
//{ machine_id: set([ user_id ]) }
typedef std::map<int, IdSet_t>  PlantUserMap_t;
// { user_name: user_id }
typedef std::map<CString, int> UserIdIndex_t;

// { (company_name, factory_name, plant_id) : machine_id }
class MachineIdIndex_t
{
public:
	typedef std::map<CString, int> Hierarchy_Id_Pair_t;
	typedef std::map<Fnv32_t, Hierarchy_Id_Pair_t>  Hierarchy_Id_Map_t;

public:
	void Insert( const CString& hierarchy_path, int machine_id )
	{
		Fnv32_t hierarchy_hash = fnv1a_hash_32(hierarchy_path.GetString());

		Hierarchy_Id_Map_t::iterator itHash = m_PlantHierarchyMachineIdMap.find(hierarchy_hash);

		if (itHash != m_PlantHierarchyMachineIdMap.end())
		{
			Hierarchy_Id_Pair_t& hierarchy_machineId = itHash->second;

			hierarchy_machineId.insert(std::make_pair(hierarchy_path, machine_id));
		}
		else
		{
			Hierarchy_Id_Pair_t hierarchy_machineId;
			hierarchy_machineId.insert(std::make_pair(hierarchy_path, machine_id));

			m_PlantHierarchyMachineIdMap.insert(std::make_pair(hierarchy_hash, hierarchy_machineId));
		}
	}

	bool Get( const CString& hierarchy_path, int& outMachineId )
	{
		Fnv32_t hierarchy_hash = fnv1a_hash_32(hierarchy_path.GetString());

		Hierarchy_Id_Map_t::const_iterator itHash = m_PlantHierarchyMachineIdMap.find(hierarchy_hash);

		if (itHash == m_PlantHierarchyMachineIdMap.end())
		{
			return false;
		}

		const Hierarchy_Id_Pair_t& hierarchy_machineId = itHash->second;

		Hierarchy_Id_Pair_t::const_iterator itHierarchy = hierarchy_machineId.find(hierarchy_path);

		if (itHierarchy == hierarchy_machineId.end())
		{
			return false;
		}

		outMachineId = itHierarchy->second;

		return true;
	}

private:
	Hierarchy_Id_Map_t m_PlantHierarchyMachineIdMap;
};
// { (company_name, factory_name, plant_id) : [machine_id] }
typedef std::list<int> IdList_t;

class HierarchyPlantMap_t
{
public:
	typedef std::map<CString, IdList_t> Hirearchy_IdList_Pair_t;
	typedef std::map<Fnv32_t, Hirearchy_IdList_Pair_t>  Hirearchy_IdList_Map_t;

public:
	void Insert( const CString& hierarchy_path, int machine_id )
	{
		Fnv32_t hierarchy_hash = fnv1a_hash_32(hierarchy_path.GetString());

		Hirearchy_IdList_Map_t::iterator itHash = m_PlantHierarchyMachineIdMap.find(hierarchy_hash);

		if (itHash != m_PlantHierarchyMachineIdMap.end())
		{
			Hirearchy_IdList_Pair_t& hierarchy_machinedIdList = itHash->second;

			Hirearchy_IdList_Pair_t::iterator itHierachy = hierarchy_machinedIdList.find(hierarchy_path);

			if (itHierachy != hierarchy_machinedIdList.end())
			{
				itHierachy->second.push_back(machine_id);
			}
			else
			{
				// hash collision
				ASSERT(FALSE);

				itHash->second.insert(std::make_pair(hierarchy_path, machine_id));
			}
		}
		else
		{
			IdList_t machinedIdList;
			machinedIdList.push_back(machine_id);

			Hirearchy_IdList_Pair_t hierarchy_machinedIdList;
			hierarchy_machinedIdList.insert(std::make_pair(hierarchy_path, machinedIdList));

			m_PlantHierarchyMachineIdMap.insert(std::make_pair(hierarchy_hash, hierarchy_machinedIdList));
		}
	}

	bool Get( const CString& hierarchy, IdList_t& outMachineIdList )
	{
		Fnv32_t hierarchy_hash = fnv1a_hash_32(hierarchy.GetString());

		Hirearchy_IdList_Map_t::const_iterator itHash = m_PlantHierarchyMachineIdMap.find(hierarchy_hash);

		if (itHash == m_PlantHierarchyMachineIdMap.end())
		{
			return false;
		}

		const Hirearchy_IdList_Pair_t& hierarchy_machinedIdList = itHash->second;

		Hirearchy_IdList_Pair_t::const_iterator itHierachy = hierarchy_machinedIdList.find(hierarchy);

		if (itHierachy == hierarchy_machinedIdList.end())
		{
			return false;
		}

		outMachineIdList = itHierachy->second;

		return true;
	}

private:
	Hirearchy_IdList_Map_t m_PlantHierarchyMachineIdMap;
};

namespace UserManagement
{
	static UserTableCache_t g_User_map;
	static MachineTableCache_t g_Plant_map;

	static PlantUserMap_t g_Plant_User_map;
	static UserPlantMap_t g_User_Plant_map;

	static HierarchyPlantMap_t g_Hierarchy_Plant_map;
	
	static UserIdIndex_t g_Index_UserId;
	static MachineIdIndex_t g_Index_MachineId;

	CString GetHierarchyPath(const NameList_t& aNameList, int aCount=-1)
	{
		const char * aSep = "$";

		if(aNameList.size() == 0 || aCount == 0)
		{
			return aSep;
		}

		CString lsPath;

		int lnCount = 0;

		for(NameList_t::const_iterator it = aNameList.begin();
			it != aNameList.end();
			++it)
		{
			lsPath.AppendFormat("%s%s", aSep, it->GetString());

			lnCount++;

			if (aCount > 0 && lnCount >= aCount)
			{
				return lsPath;
			}
		}

		return lsPath;
	}

	int UserCache_Id(const CString& user_name)
	{
		CString lsUserNameNoCase = user_name;

		lsUserNameNoCase.MakeUpper();

		UserIdIndex_t::const_iterator it = UserManagement::g_Index_UserId.find(lsUserNameNoCase);

		if(it != UserManagement::g_Index_UserId.end())
		{
			return it->second;
		}

		return -1;
	}

	int PlantCache_Id(const CString& asCompany, const CString& asFactoryName, const CString& asPlantName)
	{
		NameList_t loNameList;

		do 
		{
			if (asCompany.IsEmpty())
			{
				return -1;
			}

			loNameList.push_back(asCompany);

			if (asFactoryName.IsEmpty())
			{
				return -1;
			}

			loNameList.push_back(asFactoryName);

			if (asPlantName.IsEmpty())
			{
				return -1;
			}

			loNameList.push_back(asPlantName);

		} while (0);

		CString lsPath = UserManagement::GetHierarchyPath(loNameList);


		int user_id = -1;

		if (UserManagement::g_Index_MachineId.Get(lsPath, user_id))
		{
			return user_id;
		}

		return -1;
	}

	bool PlantCache_Get(int anMachineId, PlantInfo_t& outPlantInfo)
	{
		const MachineTableCache_t& self = UserManagement::g_Plant_map;

		for (MachineTableCache_t::const_iterator it = self.begin();
			it != self.end();
			++it)
		{
			const PlantInfo_t& loPlantInfo = it->second;

			if (loPlantInfo.MachineId == anMachineId)
			{
				outPlantInfo = loPlantInfo;

				return true;
			}
		}

		return false;
	}

	bool UserCache_Get(int user_id, UserInfo_t& outUserInfo)
	{
		const UserTableCache_t& self = UserManagement::g_User_map;

		UserTableCache_t::const_iterator it = self.find(user_id);

		if (it != self.end())
		{
			outUserInfo = it->second;

			return true;
		}

		return false;
	}

	bool UserPlantMap_Get( const int user_id, IdSet_t& outMachineIDList )
	{
		const UserPlantMap_t& self = UserManagement::g_User_Plant_map;

		UserPlantMap_t::const_iterator it = self.find(user_id);

		if (it != self.end())
		{
			outMachineIDList = it->second;

			return true;
		}

		return false;
	}

	void UserPlantMap_Add( const int user_id, const int machine_id )
	{
		UserPlantMap_t& self = UserManagement::g_User_Plant_map;

		UserPlantMap_t::iterator it = self.find(user_id);

		if (it != self.end())
		{
			it->second.insert(machine_id);
		}
		else
		{
			IdSet_t machine_id_list;
			machine_id_list.insert(machine_id);

			self[user_id] = machine_id_list;
		}
	}

	void PlantUserMap_Add( const int machine_id, const int user_id )
	{
		PlantUserMap_t& self = UserManagement::g_Plant_User_map;

		PlantUserMap_t::iterator it = self.find(machine_id);

		if (it != self.end())
		{
			it->second.insert(user_id);
		}
		else
		{
			IdSet_t user_id_list;
			user_id_list.insert(user_id);

			self[machine_id] = user_id_list;
		}
	}

	bool PlantUserMap_Get(const int anMachineId, UserInfoList_t& outUserInfoList)
	{
		PlantUserMap_t::const_iterator itPlant = UserManagement::g_Plant_User_map.find(anMachineId);
		if (itPlant != UserManagement::g_Plant_User_map.end())
		{
			const IdSet_t& user_id_list = itPlant->second;

			for(IdSet_t::const_iterator itUserId = user_id_list.begin();
				itUserId != user_id_list.end();
				++itUserId)
			{
				UserTableCache_t::iterator itUserInfo = UserManagement::g_User_map.find(*itUserId);
				if (itUserInfo != UserManagement::g_User_map.end())
				{
					const UserInfo_t& loUserInfo = itUserInfo->second;
					outUserInfoList.push_back(loUserInfo);
				}
				else
				{
					ASSERT(FALSE);
				}
			}

			return true;
		}

		return false;
	}

	void UserPlantMap_Del( int user_id, int machine_id )
	{
		UserPlantMap_t::iterator it = UserManagement::g_User_Plant_map.find(user_id);

		if (it != UserManagement::g_User_Plant_map.end())
		{
			it->second.erase(machine_id);
		}
	}

	void PlantUserMap_Del(int machine_id, int user_id)
	{
		PlantUserMap_t::iterator it = UserManagement::g_Plant_User_map.find(machine_id);

		if (it != UserManagement::g_Plant_User_map.end())
		{
			it->second.erase(user_id);
		}
	}

	void UserMap_Add(const UserInfo_t& user_info)
	{
		CString lsUserNameNoCase = user_info.UserName;
		lsUserNameNoCase.MakeUpper();
		UserManagement::g_Index_UserId.insert(std::make_pair(lsUserNameNoCase, user_info.UserId));

		UserManagement::g_User_map.insert(std::make_pair(user_info.UserId, user_info));
	}

	void PlantMap_Add(const PlantInfo_t& plant_info)
	{
		NameList_t loNameList;
		UserManagement::g_Hierarchy_Plant_map.Insert(UserManagement::GetHierarchyPath(loNameList), plant_info.MachineId);

		loNameList.push_back(plant_info.CompanyName);
		UserManagement::g_Hierarchy_Plant_map.Insert(UserManagement::GetHierarchyPath(loNameList), plant_info.MachineId);

		loNameList.push_back(plant_info.FactoryName);
		UserManagement::g_Hierarchy_Plant_map.Insert(UserManagement::GetHierarchyPath(loNameList), plant_info.MachineId);

		loNameList.push_back(plant_info.PlantID);
		UserManagement::g_Hierarchy_Plant_map.Insert(UserManagement::GetHierarchyPath(loNameList), plant_info.MachineId);

		CString lsPath = UserManagement::GetHierarchyPath(loNameList);
		UserManagement::g_Index_MachineId.Insert(lsPath, plant_info.MachineId);

		UserManagement::g_Plant_map.insert(std::make_pair(plant_info.MachineId, plant_info));
	}

	void BindUserPlant(const UserInfo_t& aUserInfo, const PlantInfo_t& aPlantInfo)
	{
		UserManagement::UserPlantMap_Add(aUserInfo.UserId, aPlantInfo.MachineId);
		UserManagement::PlantUserMap_Add(aPlantInfo.MachineId, aUserInfo.UserId);
	}

	void BindUserPlant()
	{
		for (UserTableCache_t::const_iterator itUser = UserManagement::g_User_map.begin();
			itUser != UserManagement::g_User_map.end();
			++itUser)
		{
			const UserInfo_t& loUserInfo = itUser->second;

			if (loUserInfo.UserLevel == GE_ADMIN || loUserInfo.UserLevel == GE_EXPERT)
			{
				if (0 == loUserInfo.InitMachineNum)
				{
					continue;
				} 
			}

			CString lstrKey;

			for(MachineTableCache_t::const_iterator itPlant = UserManagement::g_Plant_map.begin();
				itPlant != UserManagement::g_Plant_map.end();
				++itPlant)
			{
				const PlantInfo_t& loPlantInfo = itPlant->second;

				switch(loUserInfo.UserLevel)
				{
				case GE_LOCAL_OPERATOR:
				case GE_LOCAL_ENGINEER:
				case GE_LOCAL_ADMIN:

					if (!loUserInfo.Group.IsEmpty())
					{
						if (loUserInfo.Group != loPlantInfo.GroupName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 分厂级用户%s的集团名空", loUserInfo.UserName));
					}

					if (!loUserInfo.Company.IsEmpty())
					{
						if (loUserInfo.Company != loPlantInfo.CompanyName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 分厂级用户%s的公司名空", loUserInfo.UserName));
					}

					if (!loUserInfo.Factory.IsEmpty())
					{
						if (loUserInfo.Factory != loPlantInfo.FactoryName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 分厂级用户%s的分厂名空", loUserInfo.UserName));
					}

					UserManagement::BindUserPlant(loUserInfo, loPlantInfo);

					break;

				case GE_WHOLE_OPERATOR:
				case GE_WHOLE_ENGINEER:
				case GE_WHOLE_ADMIN:
				case GE_WHOLE_EXPERT:

					if (!loUserInfo.Group.IsEmpty())
					{
						if (loUserInfo.Group != loPlantInfo.GroupName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 公司级用户%s的集团名空", loUserInfo.UserName));
					}

					if (!loUserInfo.Company.IsEmpty())
					{
						if (loUserInfo.Company != loPlantInfo.CompanyName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 公司级用户%s的公司名空", loUserInfo.UserName));
					}

					UserManagement::BindUserPlant(loUserInfo, loPlantInfo);

					break;

				case GE_GROUP_OPERATOR:
				case GE_GROUP_ENGINEER:
				case GE_GROUP_ADMIN:
				case GE_GROUP_EXPERT:

					if (!loUserInfo.Group.IsEmpty())
					{
						if (loUserInfo.Group != loPlantInfo.GroupName)
						{
							continue;
						}
					}
					else
					{
						BHLOG_ERROR(STR("用户表配置错误: 集团级用户%s的集团名空", loUserInfo.UserName));
					}

					UserManagement::BindUserPlant(loUserInfo, loPlantInfo);

					break;

				case GE_EXPERT:
				case GE_ADMIN:

					do 
					{
						if (loUserInfo.InitMachineNum > 0)
						{
							if(loUserInfo.InitMachineIDs.size() < 1)
							{
								break;
							}

							IdSet_t::const_iterator itID = loUserInfo.InitMachineIDs.find(loPlantInfo.MachineId);
							if (itID == loUserInfo.InitMachineIDs.end())
							{
								break;
							}
						}

						UserManagement::BindUserPlant(loUserInfo, loPlantInfo);


					} while (0);

					break;

				default:

					BHLOG_ERROR(STR("用户表配置错误: 用户%s的用户级别不正确", loUserInfo.UserName));

					break;
				}
			} 
		}
	}

}

#define G_USERMANAGEMENT_SHARE_LOCK() CSafeReaderLock<CTimeOutReadWriteLock> lock(g_UserManagement_lock)
#define G_USERMANAGEMENT_EXCLUSIVE_LOCK() CSafeWriterLock<CTimeOutReadWriteLock> lock(g_UserManagement_lock)

bool UserManager::IsUserExist( const CString& asUserName )
{
	G_USERMANAGEMENT_SHARE_LOCK();

	int lnUserId = UserManagement::UserCache_Id(asUserName);

	if (lnUserId < 0)
	{
		return false;
	}

	return true;
}

bool UserManager::GetUserInfo( const CString& asUserName, UserInfo_t& outUserInfo )
{
	G_USERMANAGEMENT_SHARE_LOCK();

	int lnUserId = UserManagement::UserCache_Id(asUserName);

	if (lnUserId < 0)
	{
		return false;
	}

	return UserManagement::UserCache_Get(lnUserId, outUserInfo);
}

bool UserManager::IsPlantExist( const CString& asCompany, const CString& asFactoryName, const CString& asPlantName )
{
	PlantInfo_t loUserInfo;

	if (asCompany.IsEmpty())
	{
		return false;
	}

	if (asFactoryName.IsEmpty())
	{
		return false;
	}

	if (asPlantName.IsEmpty())
	{
		return false;
	}

	G_USERMANAGEMENT_SHARE_LOCK();

	int lnMachineId = UserManagement::PlantCache_Id(asCompany, asFactoryName, asPlantName);

	if (lnMachineId < 0)
	{
		return false;
	}

	return true;
}

bool UserManager::GetPlantInfo(const CString& asCompany, const CString& asFactoryName, const CString& asPlantName, PlantInfo_t& outPlantInfo)
{
	G_USERMANAGEMENT_SHARE_LOCK();

	int lnMachineId = UserManagement::PlantCache_Id(asCompany, asFactoryName, asPlantName);

	if (lnMachineId < 0)
	{
		return false;
	}

	return UserManagement::PlantCache_Get(lnMachineId, outPlantInfo);
}


int UserManager::CreateUser( const UserInfo_t& aUserInfo, const IdSet_t& aMachineIdList )
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	UserManagement::UserMap_Add(aUserInfo);

	for (IdSet_t::const_iterator itMachineId = aMachineIdList.begin();
		itMachineId != aMachineIdList.end();
		++itMachineId)
	{
		UserManagement::PlantUserMap_Add(*itMachineId, aUserInfo.UserId);
	}

	UserManagement::g_User_Plant_map.insert(std::make_pair(aUserInfo.UserId, aMachineIdList));

	return UM_SUCCESS;
}


int UserManager::DeleteUser( const CString& asUserName )
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	int lnUserId = UserManagement::UserCache_Id(asUserName);
	if (lnUserId < 0)
	{
		return UM_ERROR_USER_NOT_EXIST;
	}
	
	IdSet_t loMachineIdList;

	if(UserManagement::UserPlantMap_Get(lnUserId, loMachineIdList))
	{
		for (IdSet_t::const_iterator it = loMachineIdList.begin();
			it != loMachineIdList.end();
			++it)
		{
			int machine_id = *it;

			UserManagement::PlantUserMap_Del(machine_id, lnUserId);
		}
	}
	else
	{
		ASSERT(FALSE);
	}

	{
		CString lsUserNameNoCase = asUserName;
		lsUserNameNoCase.MakeUpper();
		UserManagement::g_Index_UserId.erase(lsUserNameNoCase);
	}

	UserManagement::g_User_map.erase(lnUserId);
	UserManagement::g_User_Plant_map.erase(lnUserId);

	return UM_SUCCESS;
}

bool UserManager::SetUserLevel(int user_id, int user_level)
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	UserTableCache_t::iterator it = UserManagement::g_User_map.find(user_id);

	if (it != UserManagement::g_User_map.end())
	{
		it->second.UserLevel = user_level;

		return true;
	}

	return false;
}

bool UserManager::GetMachineIDList( int anUserID, IdSet_t& outMachineIDList )
{
	G_USERMANAGEMENT_SHARE_LOCK();

	return UserManagement::UserPlantMap_Get(anUserID, outMachineIDList);
}

IdSet_t UserManager::GetTotalMachineIDList(void)
{
	G_USERMANAGEMENT_SHARE_LOCK();

	IdSet_t loMachineIdList;

	for(MachineTableCache_t::const_iterator it = UserManagement::g_Plant_map.begin();
		it != UserManagement::g_Plant_map.end();
		++it)
	{
		loMachineIdList.insert(it->first);
	}

	return loMachineIdList;
}

bool UserManager::Bind( int anUserID, const IdSet_t& aMachineIdList )
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	UserPlantMap_t::iterator itUser = UserManagement::g_User_Plant_map.find(anUserID);
	if (itUser == UserManagement::g_User_Plant_map.end())
	{
		UserPlantMap_t::_Pairib result;

		result = UserManagement::g_User_Plant_map.insert(std::make_pair(anUserID, aMachineIdList));

		ASSERT(result.first != UserManagement::g_User_Plant_map.end());
		ASSERT(result.second);

		itUser = result.first;
	}

	for (IdSet_t::const_iterator it = aMachineIdList.begin();
		it != aMachineIdList.end();
		++it)
	{
		int machine_id = *it;

		itUser->second.insert(*it);

#ifdef _DEBUG
		ASSERT(UserManagement::g_Plant_User_map.find(machine_id) != UserManagement::g_Plant_User_map.end());
#endif
		UserManagement::PlantUserMap_Add(machine_id, anUserID);
	}

	return true;
}

bool UserManager::Unbind( int anUserID, const IdSet_t& aMachineIdList )
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	UserPlantMap_t::iterator itUser = UserManagement::g_User_Plant_map.find(anUserID);
	if (itUser == UserManagement::g_User_Plant_map.end())
	{
		ASSERT(FALSE);
		return true;
	}

	for (IdSet_t::const_iterator itMachineId = aMachineIdList.begin();
		itMachineId != aMachineIdList.end();
		++itMachineId)
	{
		int machine_id = *itMachineId;

		itUser->second.erase(machine_id);

#ifdef _DEBUG
		ASSERT(UserManagement::g_Plant_User_map.find(machine_id) != UserManagement::g_Plant_User_map.end());
#endif
		UserManagement::PlantUserMap_Del(machine_id, anUserID);
	}

	return true;
}


int UserManager::ForEachPlant( const CString& asCompany, const CString& asFactoryName, const CString& asPlantName,
							  PlantIteratorFunc_t aIterator, void * aParam )
{
	if (NULL == aIterator)
	{
		ASSERT(FALSE);
		return -1;
	}

	NameList_t loNameList;

	do 
	{
		if (asCompany.IsEmpty())
		{
			break;
		}

		loNameList.push_back(asCompany);

		if (asFactoryName.IsEmpty())
		{
			break;
		}

		loNameList.push_back(asFactoryName);

		if (asPlantName.IsEmpty())
		{
			break;
		}

		loNameList.push_back(asPlantName);

	} while (0);

	G_USERMANAGEMENT_SHARE_LOCK();

	CString lsPath = UserManagement::GetHierarchyPath(loNameList);

	IdList_t loMachineIdList;

	if (!UserManagement::g_Hierarchy_Plant_map.Get(lsPath, loMachineIdList))
	{
		return 0;
	}

	int lnPlantCount = 0;

	int lnResult = -1;


	for (IdList_t::const_iterator itMachineId = loMachineIdList.begin();
		itMachineId != loMachineIdList.end();
		++itMachineId)
	{
		int lnMachineId = *itMachineId;

		PlantInfo_t loPlantInfo;
		UserInfoList_t loUserInfoList;

		if (!UserManagement::PlantCache_Get(lnMachineId, loPlantInfo))
		{
			ASSERT(FALSE);
			continue;
		}

		UserManagement::PlantUserMap_Get(lnMachineId, loUserInfoList);


		try
		{
			lnResult = (*aIterator)(loPlantInfo, loUserInfoList, aParam);
		}
		catch (...)
		{
			ASSERT(FALSE);
			BHLOG_ERROR(STR("Exception: "));
		
			return -2;
		}

		if (lnResult < 0)
		{
			return lnResult;
		}

		lnPlantCount++;
	}

	return lnPlantCount;
}

struct ST_GROUP_INFO
{
	CString GroupName;
	CString CompanyName;
	CString FactoryName;
	CString GroupDisplayName;
	CString CompanyDisplayName;
	CString FactoryDisplayName;
	int     GroupSortNO;
	int     CompanySortNO;
	int     FactorySortNO;
	int     DbId;
};

int UserManager::Initialize(CSimADO *pDB)
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	ASSERT(pDB);

	CString lsSQL;

	typedef std::map<CString, ST_GROUP_INFO> Company_Group_Map_t;

	Company_Group_Map_t lomapGroupInfo;

	//查询集团表
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM %s",gc_cGroup,gc_cDisplay,gc_cSortNO,gc_cGroupTable);

	while(pDB->More())
	{
		ST_GROUP_INFO loGroupInfo;

		loGroupInfo.GroupName = pDB->Get(gc_cGroup);
		loGroupInfo.GroupName.Trim();

		loGroupInfo.GroupDisplayName= pDB->Get(gc_cDisplay);
		loGroupInfo.GroupDisplayName.Trim();

		loGroupInfo.GroupSortNO = pDB->GetLong(gc_cSortNO);

		lomapGroupInfo.insert(std::make_pair(loGroupInfo.GroupName,loGroupInfo));
	}

	Company_Group_Map_t loCompanyGroupMap;

	//查询公司表
	pDB->Cmd("SELECT [%s],[%s],[%s],[%s],[%s] FROM %s", 
		     gc_cCompany, gc_cGroup, gc_cDB_ID,gc_cDisplay,gc_cSortNO, gc_cCompanyTable);

	Company_Group_Map_t::iterator lpIter = NULL;

	while(pDB->More())
	{
		ST_GROUP_INFO loGroupInfo;

		CString sCompany = pDB->Get(gc_cCompany);
		sCompany.Trim();

		loGroupInfo.GroupName = pDB->Get(gc_cGroup);
		loGroupInfo.GroupName.Trim();

		loGroupInfo.DbId = pDB->GetLong(gc_cDB_ID);

		loGroupInfo.CompanyDisplayName = pDB->Get(gc_cDisplay);
		loGroupInfo.CompanyDisplayName.Trim();

		loGroupInfo.CompanySortNO = pDB->GetLong(gc_cSortNO);

		lpIter = lomapGroupInfo.find(loGroupInfo.GroupName);

		if (lpIter != lomapGroupInfo.end())
		{
			loGroupInfo.GroupDisplayName = lpIter->second.GroupDisplayName;

			loGroupInfo.GroupSortNO = lpIter->second.GroupSortNO;
		}

		loCompanyGroupMap.insert(std::make_pair(sCompany, loGroupInfo));
	}

	pDB->Cmd("SELECT %s,[%s],[%s],[%s],[%s],[%s],%s,%s FROM [%s]",
		SimAdo_Expression_ISNULL(gc_cID, -1),
		gc_cUserLevel,
		gc_cUserName,
		gc_cCompany,
		gc_cGroup,
		gc_cFactory_Name,
		SimAdo_Expression_ISNULL(gc_cNum, -1),
		gc_cMachines,
		gc_cUserTable);		
	pDB->Cmd(" ORDER BY [%s]", gc_cID);

	while(pDB->More())
	{
		int lnUserID   = pDB->GetLong(gc_cID);

		if (lnUserID < 0)
		{
			ASSERT(FALSE);
			BHLOG_ERROR("用户表ID字段不正确, 请检查");

			continue;
		}

		UserInfo_t info(pDB);

		UserManagement::UserMap_Add(info);
	}

	//查询分厂表
	vector<ST_GROUP_INFO> loFactoryInfo;
	
	pDB->Cmd("SELECT [%s],[%s],[%s],[%s] FROM %s",
		      gc_cCompany,gc_cFactory_Name,gc_cDisplay,gc_cSortNO,gc_cFactoryTable);

	while(pDB->More())
	{
		ST_GROUP_INFO loGroupInfo;

		loGroupInfo.CompanyName = pDB->Get(gc_cCompany);
		loGroupInfo.CompanyName.Trim();

		loGroupInfo.FactoryName = pDB->Get(gc_cFactory_Name);
		loGroupInfo.FactoryName.Trim();

		loGroupInfo.FactoryDisplayName = pDB->Get(gc_cDisplay);
		loGroupInfo.FactoryDisplayName.Trim();

		loGroupInfo.FactorySortNO = pDB->GetLong(gc_cSortNO);

		Company_Group_Map_t::const_iterator it = loCompanyGroupMap.find(loGroupInfo.CompanyName);

		if (it != loCompanyGroupMap.end())
		{
			loGroupInfo.GroupName = it->second.GroupName;
			
			loGroupInfo.GroupDisplayName = it->second.GroupDisplayName;

			loGroupInfo.GroupSortNO = it->second.GroupSortNO;

			loGroupInfo.CompanyDisplayName = it->second.CompanyDisplayName;

			loGroupInfo.CompanySortNO = it->second.CompanySortNO;

			loGroupInfo.DbId = it->second.DbId;
		}

		loFactoryInfo.push_back(loGroupInfo);
	}

	//查询设备表
	pDB->Cmd("SELECT ISNULL(a.[%s],-1) [%s],a.[%s],a.[%s],a.[%s],a.[%s],a.[%s],a.[%s],a.%s,a.%s,a.%s,a.%s,a.%s,b.%s from [%s] a,[%s] b where a.[%s] = b.[%s]",
		gc_cID,
		gc_cID,
		gc_cCompany,
		gc_cFactory_Name,
		gc_cPlantID, 
		gc_cSetID,
		gc_cDisplay,
		gc_cErpCode,
		gc_cGraphType,
		gc_cMachine_Type,
		gc_cData_Source,
		gc_cSortNO,
		gc_cPlantNo,
		gc_cCompanyAlias,
		gc_cMachineTable,
		gc_cCompanyTable,
		gc_cCompany,
		gc_cCompany);

	while(pDB->More())
	{
		int lnMachineID   = pDB->GetLong(gc_cID);

		if (lnMachineID < 0)
		{
			ASSERT(FALSE);
			BHLOG_ERROR("设备表ID字段不正确, 请检查");

			continue;
		}

		PlantInfo_t loPlantInfo(pDB);

		//Company_Group_Map_t::const_iterator it = loCompanyGroupMap.find(loPlantInfo.CompanyName);

		//if (it != loCompanyGroupMap.end())
		//{
		//	const ST_GROUP_INFO& loGroupInfo = it->second;

		//	loPlantInfo.GroupName = loGroupInfo.GroupName;
		//	loPlantInfo.Db_Id     = loGroupInfo.DbId;
		//}
		//else
		//{
		//	ASSERT(FALSE);
		//}
		for (vector<ST_GROUP_INFO>::iterator lpIter = loFactoryInfo.begin(); lpIter != loFactoryInfo.end();++lpIter)
		{
			if (lpIter->CompanyName == loPlantInfo.CompanyName && lpIter->FactoryName == loPlantInfo.FactoryName)
			{
				loPlantInfo.GroupName = lpIter->GroupName;
				
				loPlantInfo.Db_Id = lpIter->DbId;

				loPlantInfo.GroupDisplayName = lpIter->GroupDisplayName;

				loPlantInfo.CompanyDisplayName = lpIter->CompanyDisplayName;

				loPlantInfo.FactoryDisplayName = lpIter->FactoryDisplayName;

				loPlantInfo.GroupSortNO = lpIter->GroupSortNO;

				loPlantInfo.CompanySortNO = lpIter->CompanySortNO;

				loPlantInfo.FactorySortNO = lpIter->FactorySortNO;

				break;
			}
		}


		UserManagement::PlantMap_Add(loPlantInfo);
	}

	UserManagement::BindUserPlant();

	return TRUE;
}


int UserManager::UserDump(ICommInterFace* pFixComm)
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	UserTableCache_t::const_iterator itUser = UserManagement::g_User_map.begin();

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	while (itUser != UserManagement::g_User_map.end())
	{
		const UserInfo_t& loUserInfo = itUser->second;

		IdSet_t loMachineIdList;

		if(UserManagement::UserPlantMap_Get(loUserInfo.UserId, loMachineIdList))
		{
			pFixComm->SetItem(FIELD_SYSTEM_USEID, loUserInfo.UserName);

			CString buffer = IdSet_ToString(loMachineIdList);

			pFixComm->SetItem(FIELD_SERVICE_DEVICE_ID, buffer.GetString());
		}
		else
		{
			pFixComm->SetItem(FIELD_SYSTEM_USEID, loUserInfo.UserName);
		}

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();

		++itUser;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return UserManagement::g_User_map.size();
}

int UserManager::MachineDump(ICommInterFace* pFixComm)
{
	G_USERMANAGEMENT_EXCLUSIVE_LOCK();

	MachineTableCache_t::const_iterator itPlant = UserManagement::g_Plant_map.begin();

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	while (itPlant != UserManagement::g_Plant_map.end())
	{
		const PlantInfo_t& loPlantInfo = itPlant->second;

		pFixComm->SetItem(FIELD_SERVICE_DEVICE_ID, loPlantInfo.MachineId);
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY, loPlantInfo.CompanyName);
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY, loPlantInfo.FactoryName);
		pFixComm->SetItem(FIELD_SERVICE_PLANTID, loPlantInfo.PlantID);

		UserInfoList_t loUserInfoList;

		if(UserManagement::PlantUserMap_Get(loPlantInfo.MachineId, loUserInfoList))
		{
			CString buffer = UserInfoList_ToString(loUserInfoList);

			pFixComm->SetItem(FIELD_SYSTEM_USEID, buffer.GetString());
		}

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();

		++itPlant;
	}

	pFixComm->CreateEndMak(); 
	pFixComm->Write(pFixComm->GetAci());

	return UserManagement::g_Plant_map.size();
}

bool UserManager::GetSpecMachineIDPlantInfo(int anMachineId, PlantInfo_t& outPlantInfo)
{
	G_USERMANAGEMENT_SHARE_LOCK();

	return UserManagement::PlantCache_Get(anMachineId,outPlantInfo);
}

IdSet_t UserTable_GetMachine( CSimADO * thiz, int anMachineNum/* = -1*/ )
{
	ASSERT(thiz != NULL);

	if (0 == anMachineNum)
	{
		return IdSet_t();
	}

	long lnActualBytes = thiz->GetActualSize(gc_cMachines);

	if (lnActualBytes < sizeof(Id_t))
	{
		return IdSet_t();
	}

	long lnActualNum = lnActualBytes / sizeof(Id_t);

	IdArray_t loMachineIdArr(lnActualNum + 1);


	try
	{
		thiz->GetBuffer(gc_cMachines, (BYTE*)&loMachineIdArr[0], lnActualBytes);
	}
	catch (...)
	{
		ASSERT(FALSE);
		BHLOG_ERROR(STR("FixComm Database: "));

		return IdSet_t();
	}

	if (anMachineNum > 0)
	{
		if (lnActualNum > anMachineNum)
		{
			loMachineIdArr.resize(anMachineNum);
		}
		else
		{
			loMachineIdArr.resize(lnActualNum);
		}
	}
	else
	{
		loMachineIdArr.resize(lnActualNum);
	}

	return IdSet_t(loMachineIdArr.begin(), loMachineIdArr.end());
}


bool UserTable_SetMachine( CSimADO * thiz, int user_id, const IdSet_t &update_machine_ids)
{
	ASSERT(thiz != NULL);

	thiz->BeginTran();


	do 
	{
		bool bRet = false;

		if (update_machine_ids.size() > 0)
		{
			thiz->Cmd("UPDATE %s SET [%s]=%d", gc_cUserTable,
				gc_cNum, update_machine_ids.size());
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if(!thiz->Exec())
			{
				break;
			}

			thiz->Cmd("SELECT %s FROM %s", gc_cMachines, gc_cUserTable);
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if (thiz->More())
			{
				IdArray_t loMachineIdArr(update_machine_ids.begin(), update_machine_ids.end());

				bool result = false;

				long bytes = update_machine_ids.size() * sizeof(Id_t);

				try
				{
					result = thiz->SetBuffer(gc_cMachines, (BYTE*)&loMachineIdArr[0], bytes);
				}
				catch (...)
				{
					ASSERT(FALSE);
					BHLOG_ERROR(STR("Database Exception: SetBuffer"));

					break;;
				}

				if (!result)
				{
					break;
				}

				bRet = true;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			thiz->Cmd("UPDATE %s SET [%s]=0,[%s]=NULL", gc_cUserTable,
				gc_cNum, gc_cMachines);
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if(!thiz->Exec())
			{
				break;
			}

			bRet = true;
		}
		
		thiz->CommitTran();

		return bRet;
	} while (0);

	thiz->RollbackTran();

	return false;
}

bool UserTable_SetMachine( IDBInterFace * thiz, int user_id, const IdSet_t &update_machine_ids)
{
	ASSERT(thiz != NULL);

	thiz->BeginTran();

	do 
	{
		bool bRet = false;

		if (update_machine_ids.size() > 0)
		{
			thiz->Cmd("UPDATE %s SET [%s]=%d", gc_cUserTable,
			gc_cNum, update_machine_ids.size());
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if(!thiz->Exec())
			{
				break;
			}

			thiz->Cmd("SELECT %s FROM %s", gc_cMachines, gc_cUserTable);
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if (thiz->More(TRUE))
			{
				IdArray_t loMachineIdArr(update_machine_ids.begin(), update_machine_ids.end());

				bool result = false;

				long bytes = update_machine_ids.size() * sizeof(Id_t);

				try
				{
					result = thiz->SetBuffer(gc_cMachines, (BYTE*)&loMachineIdArr[0], bytes,false);
				}
				catch (...)
				{
					ASSERT(FALSE);
					BHLOG_ERROR(STR("Database Exception: SetBuffer"));

					break;;
				}

				if (!result)
				{
					break;
				}

				bRet = true;
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			thiz->Cmd("UPDATE %s SET [%s]=0,[%s]=NULL", gc_cUserTable,
				gc_cNum, gc_cMachines);
			thiz->Cmd(" WHERE [%s]=%d", gc_cID, user_id);
			if(!thiz->Exec())
			{
				break;
			}

			bRet = true;
		}

		thiz->CommitTran();

		return bRet;
	} while (0);

	thiz->RollbackTran();

	return false;
}


namespace DatasourceHelper
{
	/************************************************************************/
	/*  SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX          25210				*/
	/************************************************************************/

	/* 比较版本大小
	* max表示比较次数, -1 全部比较, 大于0 只比较前max位
		ASSERT(CompareVersionNumber("", "") == 0);
		ASSERT(CompareVersionNumber("0", "") == 0);
		ASSERT(CompareVersionNumber("", "0") == 0);
		ASSERT(CompareVersionNumber("8.0", "8.1") < 0);
		ASSERT(CompareVersionNumber("8.8", "8.10") < 0);
		ASSERT(CompareVersionNumber("8", "8.10") < 0);
		ASSERT(CompareVersionNumber("8.0", "10.0") < 0);
		ASSERT(CompareVersionNumber("8.0.1", "8.0.99", 2) == 0);
		ASSERT(CompareVersionNumber("10.0.1600.22", "7.0", 2) > 0);
	*/
	int CompareVersionNumber(const CString &lft, const CString &rgt, int max/*=-1*/)
	{
		const char * sep =  ".";

		int ret = 0;

		int cnt = 0;

		int lpos = 0;
		int rpos = 0;

		CString lstr;
		CString rstr;

		while(lpos != -1 && rpos != -1)
		{
			lstr = lft.Tokenize(sep, lpos);
			rstr = rgt.Tokenize(sep, rpos);

			ret = atoi(lstr) - atoi(rstr);

			if (ret)
			{
				return ret;
			}

			++cnt;

			if (max > 0 && cnt >= max)
			{
				return 0;
			}
		}

		return 0;
	}

	bool IsVersionDeprecated(const CString& aVersion)
	{
		static CString gsWirelessVersion = "4.7";

		if (DatasourceHelper::CompareVersionNumber(aVersion, gsWirelessVersion, 2) < 0)
		{
			return true;
		}

		return false;
	}

	int GetSpecVersionPlantDatasource(int aDatasource, bool aDeprecated)
	{
		if (!aDeprecated)
		{
			return aDatasource;
		}

		if (aDatasource & GE_MACHINE_DATA_SOURCE_OFFLINE)
		{
			if (aDatasource & GE_MACHINE_DATA_SOURCE_ONLINE)
			{
				return GE_DEPRECATED_MACHINE_DATA_SOURCE_MIX;
			}

			return GE_DEPRECATED_MACHINE_DATA_SOURCE_OFFLINE;
		}
		
		return GE_DEPRECATED_MACHINE_DATA_SOURCE_ONLINE;
	}

	int GetSpecVersionPlantDatasource(int aDatasource, const CString& aVersion)
	{
		bool lbRequireDeprecated = DatasourceHelper::IsVersionDeprecated(aVersion);
		return DatasourceHelper::GetSpecVersionPlantDatasource(aDatasource, lbRequireDeprecated);
	}
}

IdSet_t UserTable_GetMachineEx( IDBInterFace * thiz, int anMachineNum /*= -1 */ )
{
	ASSERT(thiz != NULL);

	if (0 == anMachineNum)
	{
		return IdSet_t();
	}

	long lnActualBytes = thiz->GetActualSize(gc_cMachines);

	if (lnActualBytes < sizeof(Id_t))
	{
		return IdSet_t();
	}

	long lnActualNum = lnActualBytes / sizeof(Id_t);

	IdArray_t loMachineIdArr(lnActualNum + 1);

	CWaveHead sWaveHead;

	try
	{
		thiz->GetBuffer(gc_cMachines, (BYTE*)&loMachineIdArr[0], lnActualBytes,sWaveHead);
	}
	catch (...)
	{
		ASSERT(FALSE);
		BHLOG_ERROR(STR("FixComm Database: "));

		return IdSet_t();
	}

	if (anMachineNum > 0)
	{
		if (lnActualNum > anMachineNum)
		{
			loMachineIdArr.resize(anMachineNum);
		}
		else
		{
			loMachineIdArr.resize(lnActualNum);
		}
	}
	else
	{
		loMachineIdArr.resize(lnActualNum);
	}

	return IdSet_t(loMachineIdArr.begin(), loMachineIdArr.end());
}
