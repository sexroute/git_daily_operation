#include "StdAfx.h"
#include "off_globe.h"
#include <fstream>
#include "additions/DebugHelper.h"
#include <BaseFunc.h>
#include "Process.h"
#include <IServer.h>
#include "../ICEServer/ICEServerExport.h"
#include <MiddleWare.h>

LY::IServer * g_pServer = NULL;

int * g_nServiceMark = 0;
std::map<DWORD,CCustomDB *> * gpDBPool = NULL;
CReaderWriterLock *g_pConnPoolLock = NULL;

CString g_strDbUser;
CString g_strDbPwd;
CString g_strDbName;
CString g_strDbServer;

BOOL g_bIsDiskFull = FALSE;
LY::IServerStatus * g_pServerStatus = NULL;
CCriticalSection * g_pServerStatusLock = NULL;

HMODULE GetCurrentModule()
{ // NB: XP+ solution!
	HMODULE hModule = NULL;
	GetModuleHandleEx(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
		(LPCTSTR)GetCurrentModule,
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

		GetModuleFileName(GetCurrentModule(),sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH); 	

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

	case SERVICE_SERVICE_OFF_GETCURTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECFREQWAVE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAVIBCHANNTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETZOOMSPEC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXESTRACK:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_OFF_GETVIBCHANNALARMLOG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETPROCALARMTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETPROCALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETTRENDALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETPVDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETRODSINKDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE:
		lbNeedOpenDB = true;
		break;

		//启停车数据
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX:
		lbNeedOpenDB = true;
		break; 
	case SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETVIBCHANNSTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETTWOCHANNDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME:
		lbNeedOpenDB = true;
		break; 
	case SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_REQSUDEVENTID:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_NOTICESUDSTOP:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_RECSUDDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_CORRDIGN:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_ADDWHOLESUDLOG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SAVETHINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_RECFASTALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_ADDALARMLOG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_RECDYNALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_RECPROCALARMDATA:
		lbNeedOpenDB = true;
		break;

	case SERVICE_SERVICE_OFF_GROUPALARMSTAT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_COMPANYALARMSTAT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTAT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATEX:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETHISRUNSTATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETHISCOMPARE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETFULLSPECSTRUM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMLOGINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GET_OFFLINE_DATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETALARMIDS:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_GETALARMIDSEX:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_SET_BALANCE_DATA:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_GET_BALANCE_DATA:
		lbNeedOpenDB = true;
		break;	

	case SERVICE_SERVICE_TEMP_SAMPLE_PLANTS:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_RENAMEEVENT:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_SAVESPECTIMEDATA:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SUD_VIB_DATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SUD_DYN_DATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SUD_PROC_DATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SUD_ADD_LOG:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SET_DISK_FULL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SAVESPECMARK:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GETSPECMARK:
		lbNeedOpenDB =true;
		break;
	case SERVICE_SERVICE_OFF_GETNORMALWAVE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM:
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
	case SERVICE_SERVICE_OFF_GETCURTREND:
		lnCode= Service_OffGetCurTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECFREQWAVE:
		lnCode= Service_OffGetSpecFreqWave(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETTREND:
		lnCode= Service_OffGetTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAVIBCHANNTREND:
		lnCode= Service_OffGetAVibChannTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND:
		lnCode= Service_OffGetAxisLocationTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO:
		lnCode=Srv_OffGetTwoChannDetailInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETZOOMSPEC:
		lnCode= Service_OffGetZoomSpec(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXESTRACK:
		lnCode= Service_OffGetAxesTrack(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO:
		lnCode=Srv_OffGetSpecPlantChannsDetailInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND:
		lnCode=Srv_OffGetSpecEventChannsSudTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND:
		lnCode=Srv_OffGetVibChannAlarmTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND:
		lnCode=Srv_OffGetAxesLocationAlarmTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA:
		lnCode=Srv_OffGetAxesLocationAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND:
		lnCode=Srv_OffGetAxisDisplaceAlarmTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO:
		lnCode= Srv_OffGetSpecVibChannAlarmInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_OFF_GETVIBCHANNALARMLOG:
		lnCode= Srv_OffGetVibChannAlarmLog(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD:
		lnCode= Service_OffGetSpecAlarmTimeRecord(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND:
		lnCode=Srv_OffGetVibAlarmInfoTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETPROCALARMTREND:
		lnCode=Srv_OffGetProcAlarmTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETPROCALARMDATA:
		lnCode=Srv_OffGetProcAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETTRENDALARMDATA:
		lnCode=Srv_OffGetTrendAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA:
		lnCode=Srv_OffGetAxisDisplaceAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE:
		lnCode=Srv_OffGetSpecAlarmRecordCharValue(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETPVDATA:
		lnCode = Srv_OffGetPVData( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETRODSINKDATA:
		lnCode = Srv_OffGetRodSinkData( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA:
		lnCode = Srv_OffGetRcMultiParamData( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE:
		lnCode = Srv_OffGetSpecTimeAllChannValue( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK:
		lnCode = Srv_OffGetSudPurifyAxesTrack( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA:
		lnCode = Srv_OffGetSpecTimeAlarmMultiParamData( &arefCommInterFace, &loDb );
		break;
	case SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE:
		lnCode = Srv_OffGetSpecTimeAlarmDynStressWave(&arefCommInterFace, &loDb );
		break;

		//启停车数据
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD:
		lnCode= Service_OffGetSpecTimeRangeSud(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD:
		lnCode= Service_OffGetSpecTimeChannSud(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX:
		lnCode= Service_OffGetSpecTimeChannSudEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD:
		lnCode= Service_OffGetSpecChannRevSud(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD:
		lnCode= Service_offGetSpecChannSaveTimeSud(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX:
		lnCode= Service_offGetSpecChannSaveTimeSudEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETVIBCHANNSTREND:
		lnCode=Srv_OffGetVibChannsTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND:
		lnCode=Srv_OffGetCurrVibChannsTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETTWOCHANNDATA:
		lnCode=Srv_OffGetTwoChannData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO:
		lnCode=Srv_OffGetSpecPlantProcChannsDetailInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO:
		lnCode=Srv_OffGetSpecEventTimeChannsSudDetailInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO:
		lnCode=Srv_OffGetSpecPlantChannInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE:
		lnCode=Srv_OffGetAlarmDataTimeRange(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME:
		lnCode=Srv_OffGetSpecChannSudByTime(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT:
		lnCode=Srv_OffGetSpecPlantMonthReport(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO:
		lnCode=Srv_OffGetSpecTimeRangeFactoryRunInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_REQSUDEVENTID:
		lnCode=Srv_OffReqSudEventID(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_NOTICESUDSTOP:
		lnCode=Srv_OffNoticeSudStop(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_RECSUDDATA:
		lnCode=Srv_OffRecSudData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_CORRDIGN:
		lnCode=Srv_OffCorrDign(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_ADDWHOLESUDLOG:
		lnCode=Srv_OffAddWholeSudLog(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SAVETHINFO:
		lnCode=Srv_OffSaveThInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP:
		lnCode=Srv_OffNoticeFastAlarmStop(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_RECFASTALARMDATA:
		lnCode=Srv_OffRecFastAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID:
		lnCode=Srv_OffSaveAlarmedNewThID(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_ADDALARMLOG:
		lnCode=Srv_OffAddAlarmLog(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG:
		lnCode=Srv_OffAddFastAlarmStartLog(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_RECDYNALARMDATA:
		lnCode=Srv_OffReceiveDynAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_RECPROCALARMDATA:
		lnCode=Srv_OffReceiveProcAlarmData(&arefCommInterFace,&loDb);
		break;	
	case SERVICE_SERVICE_OFF_GROUPALARMSTAT:
		lnCode=Srv_OffGetGroupAlarmStat(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_COMPANYALARMSTAT:
		lnCode=Srv_OffGetCompanyAlarmStat(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTAT:
		lnCode=Srv_OffGetMonitorAlarmStat(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATEX:
		lnCode=Srv_OffGetMonitorAlarmStatEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR:
		lnCode=Srv_OffGetMonitorAlarmStatChar(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES:
		lnCode=Srv_OffGetSpecChannAlarmMultCharValues(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE:
		lnCode=Srv_OffGetSpecChannsAlarmSingleCharValue(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION:
		lnCode=Srv_OffGetSpecChannRodSinkAxesLocation(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETHISRUNSTATUS:
		lnCode=Srv_OffGetSpecChannsRunStatus(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETHISCOMPARE:
		lnCode=Srv_OffGetSpecChannsHisCompare(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE:
		lnCode=Srv_OffGetSpecChannsCharValueWave(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE:
		lnCode=Srv_OffGetSpecChannsCharValue(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT:
		lnCode=Srv_OffGetSpecRecipMonthReport(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK:
		lnCode=Srv_OffGetPurifyAxesTrack(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM:
		lnCode=Service_OffGetSpecChannCepstrum(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES:
		lnCode=Srv_OffGetSpecAlarmIDCharValues(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD:
		lnCode=Srv_OffGetSpecAlarmTimeDynRecord(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETFULLSPECSTRUM:
		lnCode=Srv_OffGetSpecTimeFullSpectrum(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETALARMLOGINFO:
		lnCode=Srv_OffGetAlarmLogInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE:
		lnCode=Srv_OffGetSpecTimeDynStressWave(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND:
		lnCode=Srv_OffGetSpecChannsCustomTrend(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GET_OFFLINE_DATA:
		lnCode=Srv_OffGetOfflineData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC:
		lnCode=Srv_OffRequestOfflineSync(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA:
		lnCode=Srv_OffSyncOfflineData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETALARMDATA:
		lnCode=Srv_OffGetAlarmData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETALARMIDS:
		lnCode=Srv_OffGetAlarmIDS(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETALARMIDSEX:
		lnCode=Srv_OffGetAlarmIDSEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SET_BALANCE_DATA:
		lnCode=Srv_SetBalanceData(&arefCommInterFace,&loDb);
		break;	
	case SERVICE_SERVICE_OFF_GET_BALANCE_DATA:
		lnCode=Srv_GetBalanceData(&arefCommInterFace,&loDb);
		break;	
	case SERVICE_SERVICE_TEMP_SAMPLE_PLANTS:
		lnCode = Service_temp_sample_plants(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX:
		lnCode = Service_GetSpecTimeRangeSudAndName(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME:
		lnCode = GetSpecTimeRangeSudSearchByName(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_RENAMEEVENT:
		lnCode = UpdateRenameEvent(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SAVESPECTIMEDATA:
		lnCode = SaveSpecTypeDataByTimeRegion(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX:
		lnCode=Srv_OffGetVibChannsTrendEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX:
		lnCode=Srv_OffGetSpecChannsRunStatusEx(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SUD_VIB_DATA:
		lnCode=Srv_OffRecSudVibData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SUD_DYN_DATA:
		lnCode=Srv_OffRecSudDynData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SUD_PROC_DATA:
		lnCode=Srv_OffRecSudProcData(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SUD_ADD_LOG:
		lnCode=Srv_OffRecSudLog(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SET_DISK_FULL:
		lnCode=Srv_OffSetDiskFull(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SAVESPECMARK:
		lnCode=SaveSpecMarkInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETSPECMARK:
		lnCode=GetSpecMarkInfo(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GETNORMALWAVE:
		lnCode=Srv_OffGetNormalWave(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL:
		lnCode=Srv_OffGetMonitorAlarmStatWithLevel(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE:
		lnCode=Srv_OffGetErrorSignalWave(&arefCommInterFace,&loDb);
		break;
	case SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM:
		lnCode=Srv_OffSetAxesLocationParam(&arefCommInterFace,&loDb);
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


MAP_PLANTINFO g_mapPlantInfo;
CReaderWriterLock g_MapLock;
bool g_bInit=false;
int g_SaveAllFastDataWave = 1;

/************************************************************************/
/*  临时在线是否采集配置
SAMPLE_SETTING_RECOREDS=大庆石化|化工一厂|EC1301=1;大庆石化|化工一厂|EC301=0 */
/*  SAMPLE_TIME_RECOREDS=大庆石化|化工一厂|EC1301=2012-01-02 12:20:20;大庆石化|化工一厂|EC301=2012-01-01 12:20:20 */
/************************************************************************/



INT GetSplitStrArray(const CString& strData,
					 const CString& strSplitFlag,
					 CStringArray &array,
					 BOOL SequenceAsOne)
{
	CString sData = strData;

	CString sSplitFlag = strSplitFlag;

	int lnSplitFlagLength = strSplitFlag.GetLength();

	if (sData.Right(1) != sSplitFlag) sData +=sSplitFlag;

	CString sTemp;

	int pos =-1;

	while ((pos=sData.Find(sSplitFlag,0)) != -1)
	{
		sTemp = sData.Left(pos);

		if (!SequenceAsOne)
		{
			array.Add(sTemp);
		}
		else
		{
			if (!sTemp.IsEmpty() && sTemp !="") ////连续的分隔符视为单个处理
			{
				array.Add(sTemp);
			}
		}
		sData = sData.Right(sData.GetLength() - pos - lnSplitFlagLength);
	}

	return array.GetSize();
}

std::map<CString,int> goMapSampleSettingRecords;
std::map<CString,CBHDateTime> goMapSampleTimeRecords;
BOOL SyncSampleRecords()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	BOOL  lbLockGot = FALSE;
	CString lstrStatusValue;
	CString lstrDateTimeValue;
	try
	{
		lbLockGot = g_MapLock.AcquireReaderLock();

		if (lbLockGot)
		{
			MAP_PLANTINFO::iterator it;
			for (it = g_mapPlantInfo.begin();it!=g_mapPlantInfo.end();it++)
			{
				CPlantInfoOffLine *pPlantInfo = it->second; 

				ASSERT(NULL!= pPlantInfo);
				if (NULL != pPlantInfo)
				{
					CString lstrStatus;
					lstrStatus.Format(_T("%d"),pPlantInfo->IsSample());
					lstrStatusValue    += CString(pPlantInfo->GetCompanyName().c_str())+_T("|")
						+ CString(pPlantInfo->GetFactoryName().c_str())+_T("|") 
						+ CString(pPlantInfo->GetPlantID().c_str())+"="
						+ lstrStatus+";";
					lstrDateTimeValue +=  CString(pPlantInfo->GetCompanyName().c_str())+_T("|")
						+ CString(pPlantInfo->GetFactoryName().c_str())+_T("|") 
						+ CString(pPlantInfo->GetPlantID().c_str())+"="
						+pPlantInfo->SetIsSampleTime().Format(gc_cTimeFormat)+";";

				}
			}			
		}
	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	if (lbLockGot)
	{
		try
		{
			g_MapLock.ReleaseReaderLock();
		}
		catch (...)
		{
			ASSERT(FALSE);
		}

		lbLockGot = FALSE;
	}

	BEGIN_ERROR_HANDLE;
	
	::WritePrivateProfileString("SYSTEM","SAMPLE_SETTING_RECOREDS",lstrStatusValue,cAppLocation);
	::WritePrivateProfileString("SYSTEM","SAMPLE_TIME_RECOREDS",lstrDateTimeValue,cAppLocation);
	
	END_ERROR_HANDLE;

	return TRUE;

}
BOOL initSampleRecords()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	int lnBufferSize = 10240000;

	std::vector<TCHAR> loBuffer(lnBufferSize);
	std::vector<TCHAR> loBufferTime(lnBufferSize);
	TCHAR *lpBuffer = &loBuffer.front();
	TCHAR *lpBufferTime = &loBufferTime.front();

	CString lstrSplitPlant = _T(";");
	CString lstrSplitValue = _T("=");
	CString lstrBuffer;
	CStringArray loPlantArray;
	CStringArray loValueArray;


	int lnData = ::GetPrivateProfileString("SYSTEM",
		"SAMPLE_SETTING_RECOREDS"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	
	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();
	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);
	for (int i=0;i<loPlantArray.GetSize();i++)
	{
		CString lstrPair = loPlantArray.GetAt(i);
		loValueArray.RemoveAll();
		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);
		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);
			int lnStatus = _ttoi(lstrValue.GetBuffer(0));
			goMapSampleSettingRecords[lstrKey] = lnStatus;
			BHLOG_DEBUG(STR(_T("%s|%d"),lstrKey,lnStatus));
		}
	}


	lnData = ::GetPrivateProfileString("SYSTEM",
		"SAMPLE_TIME_RECOREDS"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	
	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();
	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);
	for (int i=0;i<loPlantArray.GetSize();i++)
	{
		CString lstrPair = loPlantArray.GetAt(i);
		loValueArray.RemoveAll();
		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);
		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);
			BEGIN_ERROR_HANDLE;			
			if (!goMapSampleTimeRecords[lstrKey].ParseDateTime(lstrValue))
			{
				goMapSampleTimeRecords[lstrKey] = CBHDateTime::GetCurrentTime();
			}
			BHLOG_DEBUG(STR(_T("%s|%d"),lstrKey,goMapSampleTimeRecords[lstrKey].Format(gc_cTimeFormat)));
			END_ERROR_HANDLE;
		}
	}

	BOOL lbRet = ( goMapSampleTimeRecords.size()== goMapSampleSettingRecords.size());
	ASSERT(lbRet);
	return lbRet;
}

/*!是否开启停车仅保存月表策略*/
int G_TEMP_ONLINE = 0;


BOOL IsEnableTempOnline()
{
	char cAppLocation[5000]={0};

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	//开启停车仅保存月表策略
	int lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_TEMP_ONLINE"
		,0,
		cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lnData);
	WritePrivateProfileString(  "SYSTEM",
								"G_TEMP_ONLINE"
								,lstrData.GetBuffer(0),
								cAppLocation);

	if (lnData<=0)
	{
		lnData = ::GetPrivateProfileInt("SYSTEM",
			"G_ONLY_SAVE_MONTH_STOP"
			,0,
			cAppLocation);

		if (lnData<0)
		{
			lnData = 0;
		}

		CString lstrData;
		lstrData.Format(_T("%d"),lnData);
		WritePrivateProfileString(  "SYSTEM",
									"G_ONLY_SAVE_MONTH_STOP"
									,lstrData.GetBuffer(0),
									cAppLocation);
	}

	G_TEMP_ONLINE = lnData;
	BHLOG_DEBUG(STR(_T("开启停车仅保存月表策略: %d"),G_TEMP_ONLINE));


	//5.g_SaveAllFastDataWave
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEALLFASTDATAWAVE"
		,G_SAVEALLFASTDATAWAVE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEALLFASTDATAWAVE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEALLFASTDATAWAVE",lstrData,cAppLocation);

	g_SaveAllFastDataWave = lnData;
	BHLOG_ERROR(STR(_T("是否保存所有快变数据波形 :%d"),g_SaveAllFastDataWave));

	return lnData;
}




int G_ENABLE_SEARCH_TABLE = 0;
int IsEnableSearchAllTable()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	//开启停车仅保存月表策略
	int lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_ENABLE_SEARCH_TABLE"
		,0,
		cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lnData);
	
	WritePrivateProfileString(  "SYSTEM",
								"G_ENABLE_SEARCH_TABLE"	,
								lstrData,
								cAppLocation);

	G_ENABLE_SEARCH_TABLE = lnData;	
	BHLOG_DEBUG(STR("跨表查询模式:%d",G_ENABLE_SEARCH_TABLE));
	return G_ENABLE_SEARCH_TABLE;
}
#include "mdump.h"

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
void InitGlobalVar(CCustomDB *pDB_)
{
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
	SetGlobalErrorMode(2);
	::SetUnhandledExceptionFilter(Winet::GEHExceptionFilter);
	BOOL lblockGot = FALSE;

	IsEnableTempOnline();

	initSampleRecords();

	IsEnableSearchAllTable();

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	CharInfoHelper::Initialize(pDB_);

	try
	{
		lblockGot = g_MapLock.AcquireWriterLock();

		if (lblockGot)
		{
			BHLOG_DEBUG(STR(_T("Thread ID:%d"),::GetCurrentThreadId()));
			CString strInfo;
			int iLogLevel=GetLogOutputFlag(strInfo);
			CHZLogManage::StartLogTxt(strInfo,"KDMW",iLogLevel);
			GetMainDBName();
			//初始化设备信息对应映射表
			g_mapPlantInfo.clear();
			//简单设备信息连表
			list<S_PlantInfo> lPlantInfo;
			list<S_PlantInfo>::iterator itPlant;
			map<string,string> mapCompany;
			map<string,string>::iterator itMapCompany;
			S_PlantInfo plantInfo;
			string sCompany,sAlias;

			SwichDbName(pDB_,g_strMainDbName);
			//构造公司信息连表
			pDB_->Cmd("SELECT * from [%s] ",gc_cCompanyTable);
			while (pDB_->More())
			{
				sCompany=pDB_->Get(gc_cCompany);
				sAlias=pDB_->Get(gc_cCompanyAlias);
				mapCompany.insert(make_pair(sCompany,sAlias));
			}
			//构造设备简单信息链表
			pDB_->Cmd("SELECT * from [%s] ",gc_cMachineTable);
			while (pDB_->More())
			{
				plantInfo.GetPlantInfo( pDB_ );
				lPlantInfo.push_back( plantInfo );
			}
			string sTmp;
			CPlantInfoOffLine *pPlantInfo=NULL;
			//添加设备详细信息
			for (itPlant=lPlantInfo.begin();itPlant!=lPlantInfo.end();++itPlant)
			{
				pPlantInfo = new CPlantInfoOffLine;
				sCompany=itPlant->_cCompany;
				itMapCompany=mapCompany.find(sCompany);
				if ( itMapCompany!=mapCompany.end() )
					sAlias = itMapCompany->second;
				else
					sAlias="";
				pPlantInfo->SetPlantInfo(sCompany,sCompany,sAlias,itPlant->_cFactory,itPlant->_cSetID,*itPlant);
				pPlantInfo->AddVibChannInfo(pDB_);
				pPlantInfo->AddProcChannInfo(pDB_);
				pPlantInfo->AddDynChannInfo(pDB_);
				pPlantInfo->AddOfflineChannInfo(pDB_);
				pPlantInfo->AddChannInfoEnd();
				CString lstrCompany = sCompany.c_str();
				sCompany.append(itPlant->_cPlantID);
				CString lstrKey = lstrCompany+_T("|")+CString(itPlant->_cFactory)+_T("|") + CString(itPlant->_cPlantID);
				// 						std::map<CString,int> goMapSampleSettingRecords;
				// 						std::map<CString,CBHDateTime> goMapSampleTimeRecords;
				std::map<CString,int>::iterator lpIt = goMapSampleSettingRecords.find(lstrKey);
				if (lpIt!= goMapSampleSettingRecords.end())
				{
					pPlantInfo->IsSample(lpIt->second);
				}

				std::map<CString,CBHDateTime>::iterator lpIt2 =  goMapSampleTimeRecords.find(lstrKey);
				if (lpIt2!= goMapSampleTimeRecords.end())
				{
					pPlantInfo->SetIsSampleTime(lpIt2->second);
				}
				//AfxMessageBox(sCompany.c_str());
				g_mapPlantInfo.insert(make_pair(sCompany,pPlantInfo));
			}
			mapCompany.clear();
			lPlantInfo.clear();
		}

	}
	catch (...)
	{
		ASSERT(FALSE);
	}

	if (TRUE == lblockGot)
	{
		try
		{
			g_MapLock.ReleaseWriterLock();
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

	}

	SyncSampleRecords();

	GetConfigTrendRecordsNum();

	InitICEServer();

	RegistModuleComplete();
}


void QuitClearGlobalVar()
{
	BOOL lbLockGot = FALSE;
	try
	{
		lbLockGot = g_MapLock.AcquireWriterLock();

		if (lbLockGot)
		{
			BHLOG_DEBUG(STR(_T("Thread ID:%d"),::GetCurrentThreadId()));

			MAP_PLANTINFO::iterator it;
			CPlantInfoOffLine *pPlantInfo=NULL;
			for(it=g_mapPlantInfo.begin();it!=g_mapPlantInfo.end();++it)
			{
				pPlantInfo=it->second;
				if (NULL!=pPlantInfo)
				{
					delete pPlantInfo;
					pPlantInfo=NULL;
				}
			}
			g_mapPlantInfo.clear();
		}
	}
	catch (...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	if (TRUE == lbLockGot)
	{
		try
		{
			g_MapLock.ReleaseWriterLock();
		}
		catch (...)
		{
			ASSERT(FALSE);
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);
		}

	}
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
		CHZLogManage::Error("配置数据库的类型有误","Off_globe","GetMainDBName");
	CString sTemp;sTemp.Format("得到全局的主数据库名和数据库类型标识:%s,%d",g_strMainDbName,g_iDbType);
	CHZLogManage::Debug(sTemp,"Off_globe","GetMainDBName");
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

COleDateTimeSpan GetJudgeAlarmDatetime()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	CString lstrUpdateTime;

	int lnData = ::GetPrivateProfileString( "SYSTEM",
		"JUDGE_ALARM_TIME"
		,"1990-1-1 01:00:00",lstrUpdateTime.GetBuffer(1024),1024,cAppLocation);
	lstrUpdateTime.ReleaseBuffer();

	CBHDateTime loDatetime;
	loDatetime.ParseDateTime(lstrUpdateTime);

	COleDateTimeSpan loSpn = CBHDateTime::GetCurrentTime() - loDatetime;
	return loSpn;
}

int GetAlarmOverridTime()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");


	int lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR"
		,1,
		cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lnData);

	WritePrivateProfileString(  "SYSTEM",
		"G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR"	,
		lstrData,
		cAppLocation);

	return lnData;
}

bool isLocalJudgeAlarm()
{
	bool lbJudgeAlarm = false;

	COleDateTimeSpan loTimeSpan = GetJudgeAlarmDatetime();
	int lnHours = loTimeSpan.GetTotalHours();
	if (lnHours < GetAlarmOverridTime())
	{
		lbJudgeAlarm = true;
	}

	return lbJudgeAlarm;
}

int GetConfigTrendRecordsNum()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 300;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	try
	{
		lnData = ::GetPrivateProfileInt("SYSTEM",
			"TREND_RECORD_NUM"
			,0,
			cAppLocation);
		if (lnData<TREND_RECORD_NUM)
		{
			lnData = TREND_RECORD_NUM;
			lstrData.Format("%d",lnData);
			WritePrivateProfileString("SYSTEM","TREND_RECORD_NUM",lstrData,cAppLocation);
		}
	}
	catch (...)
	{

	}

	return lnData;

}