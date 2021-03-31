#include "stdafx.h"
#include "service.h"
#include "process.h"
#include "OnlineGlobal.h"
#include "additions/ReaderWriterlock.h"
#include <additions/CTimeOutReadWriteLock.h>
#include "MiddleWare.h"
#include <map>
#include <additions/MyAdapter.h>



long g_nVerMain =0;

/*
#include <IServer.h>
#include "../ICEServer/ICEServerExport.h"
LY::IServer * g_pServer = NULL;
int * g_nServiceMark = 0;
std::map<DWORD,CCustomDB *> * gpDBPool = NULL;
UINT IceService(int SvcCode,ICommInterFace & arefCommInterFace)
{
	bool lbNeedOpenDB = false;

	switch ( SvcCode  )
	{
		//中间件模块
		//得到所有振动测点的五种类型的特征值
		//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
	case SERVICE_SERVICE_ON_RECEIVEALLVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVEDYNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVEPROC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_REVSUDDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEPROC:
		lbNeedOpenDB = true;
		break;
		/ * 接收数采硬件状态 * /
	case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
		lbNeedOpenDB = true;
		break;
		/ * 接收数采软件状态 * /
	case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
		lbNeedOpenDB = true;
		break;
	default:
		return false;
	}

	COleSqlServerDB loDb;

	if (!loDb.IsDbSafe())
	{
		return true;
	}
	DWORD ldwThreadID = ::GetCurrentThreadId();
	std::map<DWORD,CCustomDB *>::iterator lpValue = gpDBPool->find(ldwThreadID);

	CCustomDB * lpDb = NULL;

	if (lpValue == gpDBPool->end())
	{
		lpDb = CreateHandleDataDBConn();
		gpDBPool->insert(make_pair(ldwThreadID,lpDb));
	}else
	{
		lpDb = lpValue->second;
	}
	
	loDb.setPDB(lpDb);

	int lnCode = 0;
	switch ( SvcCode )
	{		//中间件模块
		//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
	case SERVICE_SERVICE_ON_RECEIVEALLVALUE:
		lnCode=Service_OnReceiveVibValue(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_RECEIVEDYNVALUE:
		lnCode=Srv_OnReceiveDynValue(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_RECEIVEPROC:
		lnCode=Service_OnReceiveProc(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_REVSUDDATA:
		lnCode=Service_OnRevSudData(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE:
		lnCode=Srv_NetOffReceiveAllValue(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE:
		lnCode=Srv_NetOffReceiveDynValue(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_NETOFFRECEIVEPROC:
		lnCode=Srv_NetOffReceiveProc(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
		lnCode=Service_OnRcvDAQHWStatus(&arefCommInterFace,&loDb);
		break;

	case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
		lnCode=Service_OnRcvDAQSWStatus(&arefCommInterFace,&loDb);
		break;
	default:
		return false;
	}

	if (lnCode >=0)
	{
		UpdateWatchDogFileTime();
	}

	return true;

}

BOOL InitICEServer()
{
	gpDBPool = new std::map<DWORD,CCustomDB *>();
	//1.get Server
	g_pServer = ::GetServer();
	if (NULL!= g_pServer)
	{
		return TRUE;
	}

	//2.registServer
	g_nServiceMark = new int();
	g_pServer->RegServiceCallBackHandler((int)g_nServiceMark,IceService);

	return FALSE;
}


*/

BOOL     (*g_pCallBackFunc)( struct Service_Module * pModule )=NULL;
Service_Module module= {
		100 ,
		"BHKD" ,
		"实时数据传输"			,
		"实时处理"            ,
		"数据处理"	        ,
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
	strncpy( module.pszBuildDate , sDate,sDate.GetLength() + 1 );
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
		sVal.Format("%li,实时趋势,得到指定机组所有振动测点的五种特征数据,保留"	,SERVICE_SERVICE_ON_GETALLFIVECHAR);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定指定机组和指定振动测点五种类型特征值,保留",SERVICE_SERVICE_ON_GETSPECFIVECHAR);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定机组任意测点的特征值,保留" ,SERVICE_SERVICE_ON_GETALLTYPECHAR);(*pVal).Add(sVal);

		sVal.Format("%li,实时趋势,得到指定机组指定四个振动测点指定类型的特征值,保留" ,SERVICE_SERVICE_ON_GETFOURSPECCHAR);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定机组所有过程量的值,保留" ,SERVICE_SERVICE_ON_GETALLPROCVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定机组指定过程量测点值,保留" ,SERVICE_SERVICE_ON_GETSPECPROCVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,实时波形,得到指定机组两个振动测点的波形和频谱数据,保留",SERVICE_SERVICE_ON_GETWAVE);(*pVal).Add(sVal);
		sVal.Format("%li,轴心轨迹,得到指定机组指定一组测点的轴心轨迹,保留" ,SERVICE_SERVICE_ON_GETAXESTRACK);(*pVal).Add(sVal);
		sVal.Format("%li,轴心位置,得到指定机组指定一组测点轴心位置值,保留" ,SERVICE_SERVICE_ON_GETAXESLOCATION);(*pVal).Add(sVal);
		sVal.Format("%li,设备转速,得到指定机组的设备转速,保留" ,SERVICE_SERVICE_ON_GETREV);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定机组指定轴位移测点值,保留" ,SERVICE_SERVICE_ON_GETSPECAXISDISPLACE);(*pVal).Add(sVal);
		sVal.Format("%li,实时趋势,得到指定机组所有轴位移的值,保留" ,SERVICE_SERVICE_ON_GETALLAXISDISPLACE);(*pVal).Add(sVal);
		sVal.Format("%li,报警状态,得到指定用户所属设备的报警状态,保留" ,SERVICE_SERVICE_ON_GETALLCHANNELSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组所有侧点数据,保留" ,SERVICE_SERVICE_ON_GETALLCHANNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组和测点的示功图,保留" ,SERVICE_SERVICE_ON_GETPVDATA);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组和测点的活塞杆下沉量数据,保留" ,SERVICE_SERVICE_ON_GETRODSINKDATA);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定分厂下面设备的状态,保留" ,SERVICE_SERVICE_ON_GETSUBCORPSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组测点波特图,保留" ,SERVICE_SERVICE_ON_GETBODE);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组测点瀑布图,保留" ,SERVICE_SERVICE_ON_GETWATERFALL);(*pVal).Add(sVal);
		sVal.Format("%li,实时数据,得到指定机组测点细化谱,保留" ,SERVICE_SERVICE_ON_GETZOOMSPEC);(*pVal).Add(sVal);
		sVal.Format("%li,实时信息,得到指定机组测点转速和采样信息,保留" ,SERVICE_SERVICE_ON_GETSPECCHANNINFO);(*pVal).Add(sVal);
		sVal.Format("%li,实时信息,得到指定机组幅值和相位,保留" ,SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE);(*pVal).Add(sVal);
		sVal.Format("%li,实时信息,得到指定机组多参数分析数据,保留" ,SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA);(*pVal).Add(sVal);

		sVal.Format("%li,接受数据,接受振动测点实时数据,保留" ,SERVICE_SERVICE_ON_RECEIVEALLVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,接受动态测点实时数据,保留" ,SERVICE_SERVICE_ON_RECEIVEDYNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,转速,保留" ,SERVICE_SERVICE_ON_RECEIVEREV);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,过程量,保留" ,SERVICE_SERVICE_ON_RECEIVEPROC);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,运行状态,保留" ,SERVICE_SERVICE_ON_RECEIVECHANNSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,发送数据,指定机组启停车数据,保留" ,SERVICE_SERVICE_ON_REVSUDDATA);(*pVal).Add(sVal);
		sVal.Format("%li,通知,保存数据,保留" ,SERVICE_SERVICE_ON_SAVEDATA);(*pVal).Add(sVal);
		sVal.Format("%li,通知,判断报警,保留" ,SERVICE_SERVICE_ON_JUDGEALARM);(*pVal).Add(sVal);

		sVal.Format("%li,申请ID,指定机组报警阈值ID,保留" ,SERVICE_SERVICE_ON_REQTHESHOLDID);(*pVal).Add(sVal);
		sVal.Format("%li,申请快变报警ID,申请快变报警ID,保留" ,SERVICE_SERVICE_ON_REQFASTALARMEVENTID);(*pVal).Add(sVal);
		sVal.Format("%li,更新数据,更新快变报警状态,保留" ,SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS);(*pVal).Add(sVal);

		sVal.Format("%li,接受数据,接受振动测点断网后保存的数据,保留" ,SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,接受动态测点断网后保存的数据,保留" ,SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,接受数据,接受过程量测点断网后保存的数据,保留" ,SERVICE_SERVICE_ON_NETOFFRECEIVEPROC);(*pVal).Add(sVal);

		sVal.Format("%li,得到数据,指定设备所有振动数据,保留" ,SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,指定设备所有过程量数据,保留" ,SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,指定设备运行状态,保留" ,SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,指定设备所有动态数据,保留" ,SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA);(*pVal).Add(sVal);

		sVal.Format("%li,得到数据,集团级报警统计,保留" ,SERVICE_SERVICE_ON_GROUPALARMSTAT);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,公司级报警统计,保留" ,SERVICE_SERVICE_ON_COMPANYALARMSTAT);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,监测站级报警统计,保留" ,SERVICE_SERVICE_ON_MONITORALARMSTAT);(*pVal).Add(sVal);

		sVal.Format("%li,得到数据,得到指定机组指定一组测点的活塞杆轴心位置轨迹,保留" ,SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,得到指定机组指定两个测点的活塞杆应力变化数据,保留" ,SERVICE_SERVICE_ON_GETDYNSTRESSWAVE);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,得到某一组提纯轴心轨迹的波形数据,保留" ,SERVICE_SERVICE_ON_GETPURIFYAXESTRACK);(*pVal).Add(sVal);

		sVal.Format("%li,实时波形,得到指定机组振动测点的波形和倒谱数据,保留",SERVICE_SERVICE_ON_GETCEPSTRUM);(*pVal).Add(sVal);
		sVal.Format("%li,得到数据,得到指定一对振动测点的实时全谱和全谱瀑布图,保留",SERVICE_SERVICE_ON_GETFULLSPECSTRUM);(*pVal).Add(sVal);

		/* 守护巡检 */
		sVal.Format("%li,守护巡检,获取异常数据检测结果,保留",SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD);(*pVal).Add(sVal);

		sVal.Format("%li,守护巡检,接收数采硬件状态,保留", SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,守护巡检,接收数采软件状态,保留",SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS);(*pVal).Add(sVal);

		sVal.Format("%li,设备状态,同步设备运行状态,保留" ,SERVICE_SERVICE_ON_SET_PLANT_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,设备状态,获取设备运行状态,保留" ,SERVICE_SERVICE_ON_GET_PLANT_STATUS);(*pVal).Add(sVal);
#endif
		return true;
	}

	if (NULL == gpWatchDogFileLock)
	{
		gpWatchDogFileLock = new CTimeOutReadWriteLock();

		UpdateWatchDogFileTime();
	}

	if((pModule->pDB)!=NULL)
	{
		if(_g_bInit==false)
		{
			_g_bInit=true;

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

/* 线程安全的oledb连接池
std::map<DWORD,CBHOleDbWrapper *> goDbConnectionPool;

CReaderWriterLock goConnectionPoolLock;

CBHOleDbWrapper * GetDBFromPool()
{
	CBHOleDbWrapper * lpRet = NULL;

	goConnectionPoolLock.AcquireWriterLock();

	DWORD lnCurrentThreadId = ::GetCurrentThreadId();

	std::map<DWORD,CBHOleDbWrapper *>::iterator loItConn = goDbConnectionPool.find(lnCurrentThreadId);

	if (goDbConnectionPool.end()!= loItConn)
	{
		lpRet = loItConn->second;
	}else
	{
		lpRet = new CBHOleDbWrapper();

		goDbConnectionPool[lnCurrentThreadId] = lpRet;		
	}

	goConnectionPoolLock.ReleaseWriterLock();

	return lpRet;
}
*/



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

	if (::CheckDBConn(pModule->pDB)	<0)
	{
		* pModule->pCode = -311;

		return false;
	}

	bool lbNeedOpenDB = false;

	int lnCode = -1;

	switch ( pModule->pFixComm->GetFunc() )
	{
		//中间件模块
		//得到所有振动测点的五种类型的特征值
	case SERVICE_SERVICE_ON_GETALLFIVECHAR:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定测点的五种类型特征值
	case SERVICE_SERVICE_ON_GETSPECFIVECHAR:
		lbNeedOpenDB = true;
		break;
		//得到指定机组的任意测点的类型特征值
	case SERVICE_SERVICE_ON_GETALLTYPECHAR:
		lbNeedOpenDB = true;
		break;
		//得到指定多个振动测点的频谱数据
	case SERVICE_SERVICE_ON_GETSPECTRUM:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定四个振动测点指定类型的特征值
	case SERVICE_SERVICE_ON_GETFOURSPECCHAR:
		lbNeedOpenDB = true;
		break;
		//得到所有过程量的测点值
	case SERVICE_SERVICE_ON_GETALLPROCVALUE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定过程量测点值
	case SERVICE_SERVICE_ON_GETSPECPROCVALUE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组两个振动测点的波形和频谱数据
	case SERVICE_SERVICE_ON_GETWAVE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定一组测点的轴心轨迹
	case SERVICE_SERVICE_ON_GETAXESTRACK:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定一组测点轴心位置值
	case SERVICE_SERVICE_ON_GETAXESLOCATION:
		lbNeedOpenDB = true;
		break;
		//得到指定机组的设备转速
	case SERVICE_SERVICE_ON_GETREV:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定轴位移测点值
	case SERVICE_SERVICE_ON_GETSPECAXISDISPLACE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组所有轴位移的值
	case SERVICE_SERVICE_ON_GETALLAXISDISPLACE:
		lbNeedOpenDB = true;
		break;
		//得到指定用户所属设备的报警状态
	case SERVICE_SERVICE_ON_GETALLCHANNELSTATE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组所有测点的值
	case SERVICE_SERVICE_ON_GETALLCHANNVALUE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组振动测点的相位
	case SERVICE_SERVICE_ON_GETPVDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETRODSINKDATA:
		lbNeedOpenDB = true;
		break;
		//得到指定分厂下面设备的状态
	case SERVICE_SERVICE_ON_GETSUBCORPSTATE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组测点的波特图
	case SERVICE_SERVICE_ON_GETBODE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组测点的瀑布图
	case SERVICE_SERVICE_ON_GETWATERFALL:
		lbNeedOpenDB = true;
		break;
		//得到指定机组测点的细化谱图
	case SERVICE_SERVICE_ON_GETZOOMSPEC:
		lbNeedOpenDB = true;
		break;
		//得到指定机组和测点的转速和采样信息
	case SERVICE_SERVICE_ON_GETSPECCHANNINFO:
		lbNeedOpenDB = true;
		break;
		//得到指定机组的振动测点的幅值和相位
	case SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE:
		lbNeedOpenDB = true;
		break;
		//得到指定机组的多参数分析数据
	case SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA:
		lbNeedOpenDB = true;
		break;
		//得到集团级报警统计数据
	case SERVICE_SERVICE_ON_GROUPALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//得到公司级报警统计数据
	case SERVICE_SERVICE_ON_COMPANYALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//得到监测站级报警统计数据
	case SERVICE_SERVICE_ON_MONITORALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定一组测点的活塞杆轴心位置轨迹
	case SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION:
		lbNeedOpenDB = true;
		break;
		//得到指定机组指定两个测点的活塞杆应力变化数据
	case SERVICE_SERVICE_ON_GETDYNSTRESSWAVE:
		lbNeedOpenDB = true;
		break;
		//得到某一组提纯轴心轨迹的波形数据
	case SERVICE_SERVICE_ON_GETPURIFYAXESTRACK:
		lbNeedOpenDB = true;
		break;

		//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
	case SERVICE_SERVICE_ON_RECEIVEALLVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVEDYNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVEREV:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVEPROC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVECHANNSTATE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_REVSUDDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SAVEDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_JUDGEALARM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_REQTHESHOLDID:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_REQFASTALARMEVENTID:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS:
		lbNeedOpenDB = true;
		break;

	case SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEPROC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE:
		lbNeedOpenDB = true;
		break;

	case SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETCEPSTRUM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETFULLSPECSTRUM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA:
		lbNeedOpenDB = true;
		break;

		/* 守护巡检 */
		/* 获取异常数据检测结果 */
	case SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD:
		lbNeedOpenDB = true;
		break;
		/* 接收数采硬件状态 */
	case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
		lbNeedOpenDB = true;
		break;
		/* 接收数采软件状态 */
	case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
		lbNeedOpenDB = true;
		break;
		/*得到监测中心的设备信息表*/
	case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SYSTEM_GETSPECTYPECHANN:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_GET_DYNINFO:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_GET_VIBINFO:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SET_DISK_FULL:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_ALARM_CONFIRM:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SUD_RECEIVEPROC:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECPROCFASTALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECVIBFASTALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECDYNFASTALARMDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SAVESPECTIMEDATA:
		lbNeedOpenDB = true;
		break;	
	case  SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION:
		lbNeedOpenDB = true;
		break;	
	case  SERVICE_SERVICE_ON_GET_ALARM_LIMITATION:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_SET_PLANT_STATUS:
		lbNeedOpenDB = true;
		break;	
	case  SERVICE_SERVICE_ON_GET_PLANT_STATUS:
		lbNeedOpenDB = true;
		break;	
	case SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SUD_VIB_DATA:
		lbNeedOpenDB=true;
		break;
	case SERVICE_SERVICE_ON_SUD_DYN_DATA:
		lbNeedOpenDB=true;
		break;
	case SERVICE_SERVICE_ON_SUD_PROC_DATA:
		lbNeedOpenDB=true;
		break;
	case  SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT:
		lbNeedOpenDB=true;
		break;
	case  SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM:
		lbNeedOpenDB=true;
		break;
	case  SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM:
		lbNeedOpenDB=true;
		break;
	default:
		return false;
	}

	COleSqlServerDB loDb;

	if (!loDb.IsDbSafe())
	{
		*(pModule->pCode) = -311;	

		return true;
	}

	loDb.setPDB(pModule->pDB);

	CBHCommWrapper loComm;

	loComm.SetFixComm(pModule->pFixComm);

	bool lbShouldUpdateWatchDog = true;

	try
	{
		switch ( pModule->pFixComm->GetFunc() )
		{		//中间件模块
			//得到所有振动测点的五种类型的特征值
		case SERVICE_SERVICE_ON_GETALLFIVECHAR:
			*pModule->pCode =  Service_OnGetAllFiveChar(&loComm , &loDb );
			break;
			//得到指定机组指定测点的五种类型特征值
		case SERVICE_SERVICE_ON_GETSPECFIVECHAR:
			*pModule->pCode=Service_OnGetSpecFiveChar(&loComm , &loDb);
			break;
			//得到指定机组的任意测点的类型特征值
		case SERVICE_SERVICE_ON_GETALLTYPECHAR:
			*pModule->pCode= Srv_OnGetAllTypeChar(&loComm , &loDb);
			break;
			//得到指定多个振动测点的频谱数据
		case SERVICE_SERVICE_ON_GETSPECTRUM:
			*pModule->pCode=Srv_OnGetSpectrum(&loComm,&loDb);
			break;
			//得到指定机组指定四个振动测点指定类型的特征值
		case SERVICE_SERVICE_ON_GETFOURSPECCHAR:
			*pModule->pCode=SerVice_OnGetFourSpecChar(&loComm , &loDb);
			break;
			//得到所有过程量的测点值
		case SERVICE_SERVICE_ON_GETALLPROCVALUE:
			*pModule->pCode=SerVice_OnGetAllProcValue(&loComm , &loDb);
			break;
			//得到指定机组指定过程量测点值
		case SERVICE_SERVICE_ON_GETSPECPROCVALUE:
			*pModule->pCode=SerVice_OnGetSpecProcValue(&loComm , &loDb);
			break;
			//得到指定机组两个振动测点的波形和频谱数据
		case SERVICE_SERVICE_ON_GETWAVE:
			*pModule->pCode= Service_OnGetWave(&loComm , &loDb );
			break;
			//得到指定机组指定一组测点的轴心轨迹
		case SERVICE_SERVICE_ON_GETAXESTRACK:
			*pModule->pCode= Service_OnGetAxesTrack(&loComm , &loDb );
			break;
			//得到指定机组指定一组测点轴心位置值
		case SERVICE_SERVICE_ON_GETAXESLOCATION:
			*pModule->pCode= Service_OnGetAxesLocation(&loComm , &loDb );
			break;
			//得到指定机组的设备转速
		case SERVICE_SERVICE_ON_GETREV:
			*pModule->pCode= Service_OnGetRev(&loComm , &loDb );
			break;
			//得到指定机组指定轴位移测点值
		case SERVICE_SERVICE_ON_GETSPECAXISDISPLACE:
			*pModule->pCode= Service_OnGetSpecAxisDisplace(&loComm , &loDb );
			break;
			//得到指定机组所有轴位移的值
		case SERVICE_SERVICE_ON_GETALLAXISDISPLACE:
			*pModule->pCode= Service_OnGetAllAxisDisplace(&loComm , &loDb );
			break;
			//得到指定用户所属设备的报警状态
		case SERVICE_SERVICE_ON_GETALLCHANNELSTATE:
			*pModule->pCode= Service_OnGetAllChannelState(&loComm , &loDb );
			break;
			//得到指定机组所有测点的值
		case SERVICE_SERVICE_ON_GETALLCHANNVALUE:
			*pModule->pCode= Service_OnGetAllChannelValue(&loComm , &loDb );
			break;
			//得到指定机组振动测点的相位
		case SERVICE_SERVICE_ON_GETPVDATA:
			*pModule->pCode= Srv_OnGetPVData(&loComm , &loDb );
			break;
		case SERVICE_SERVICE_ON_GETRODSINKDATA:
			*pModule->pCode=Srv_OnGetRodSinkData(&loComm,&loDb);
			break;
			//得到指定分厂下面设备的状态
		case SERVICE_SERVICE_ON_GETSUBCORPSTATE:
			*pModule->pCode= Service_OnGetSubCorpState(&loComm , &loDb );
			break;
			//得到指定机组测点的波特图
		case SERVICE_SERVICE_ON_GETBODE:
			*pModule->pCode= Service_OnGetBode(&loComm , &loDb );
			break;
			//得到指定机组测点的瀑布图
		case SERVICE_SERVICE_ON_GETWATERFALL:
			*pModule->pCode= Service_OnGetWaterFall(&loComm , &loDb );
			break;
			//得到指定机组测点的细化谱图
		case SERVICE_SERVICE_ON_GETZOOMSPEC:
			*pModule->pCode= Service_OnGetZoomSpec(&loComm , &loDb );
			break;
			//得到指定机组和测点的转速和采样信息
		case SERVICE_SERVICE_ON_GETSPECCHANNINFO:
			*pModule->pCode=Service_OnGetSpecChannInfo(&loComm , &loDb );
			break;
			//得到指定机组的振动测点的幅值和相位
		case SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE:
			*pModule->pCode=Service_OnGetAllSpecCharAndPhase(&loComm , &loDb );
			break;
			//得到指定机组的多参数分析数据
		case SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA:
			*pModule->pCode=Srv_OnGetRcMultiParamData(&loComm,&loDb);
			break;
			//得到集团级报警统计数据
		case SERVICE_SERVICE_ON_GROUPALARMSTAT:
			*pModule->pCode=Srv_OnGetGroupAlarmStat(&loComm,&loDb);
			break;
			//得到公司级报警统计数据
		case SERVICE_SERVICE_ON_COMPANYALARMSTAT:
			*pModule->pCode=Srv_OnGetCompanyAlarmStat(&loComm,&loDb);
			break;
			//得到监测站级报警统计数据
		case SERVICE_SERVICE_ON_MONITORALARMSTAT:
			*pModule->pCode=Srv_OnGetMonitorAlarmStat(&loComm,&loDb);
			break;
			//得到指定机组指定一组测点的活塞杆轴心位置轨迹
		case SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION:
			*pModule->pCode=Srv_OnGetRodSinkAxesLocation(&loComm,&loDb);
			break;
			//得到指定机组指定两个测点的活塞杆应力变化数据
		case SERVICE_SERVICE_ON_GETDYNSTRESSWAVE:
			*pModule->pCode=Srv_OnGetDynStressWave(&loComm,&loDb);
			break;
			//得到某一组提纯轴心轨迹的波形数据
		case SERVICE_SERVICE_ON_GETPURIFYAXESTRACK:
			*pModule->pCode=Srv_OnGetPurifyAxesTrack(&loComm,&loDb);
			break;

			//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
		case SERVICE_SERVICE_ON_RECEIVEALLVALUE:
			*pModule->pCode=Service_OnReceiveVibValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEDYNVALUE:
			*pModule->pCode=Srv_OnReceiveDynValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEREV:
			*pModule->pCode=Service_OnReceiveRev(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEPROC:
			*pModule->pCode=Service_OnReceiveProc(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVECHANNSTATE:
			*pModule->pCode=Service_OnReceiveChannState(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL:
			*pModule->pCode=Service_OnReceiveChannStateWithAlarmLevel(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_REVSUDDATA:
			*pModule->pCode=Service_OnRevSudData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_SAVEDATA:
			*pModule->pCode=Service_OnSrvSaveData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_JUDGEALARM:
			*pModule->pCode=Service_OnSrvJudgeAlarm(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_REQTHESHOLDID:
			*pModule->pCode=Service_OnReqThresholdID(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_REQFASTALARMEVENTID:
			*pModule->pCode=Srv_OnReqFastAlarmEventID(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS:
			*pModule->pCode=Srv_OnUpdateFastAlarmStatus(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE:
			*pModule->pCode=Srv_NetOffReceiveAllValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE:
			*pModule->pCode=Srv_NetOffReceiveDynValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_NETOFFRECEIVEPROC:
			*pModule->pCode=Srv_NetOffReceiveProc(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE:
			*pModule->pCode=Srv_NetOffReceiveAllWirelessValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA:
			*pModule->pCode=Service_OnGetSpecPlantProcData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE:
			*pModule->pCode=Service_OnGetSpecPlantAllState(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETCEPSTRUM:
			*pModule->pCode=Srv_OnGetCepstrum(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETFULLSPECSTRUM:
			*pModule->pCode=Srv_OnGetFullSpectrum(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA:
			*pModule->pCode=Service_OnGetSpecPlantLengthVibData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA:
			*pModule->pCode=Service_OnGetSpecPlantLengthDynData(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD:
			*pModule->pCode=Service_OnGetAbnormalDataRecord(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
			*pModule->pCode=Service_OnRcvDAQHWStatus(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
			*pModule->pCode=Service_OnRcvDAQSWStatus(&loComm,&loDb);
			break;

		case SERVICE_SYSTEM_GETSPECTYPECHANN:
			*pModule->pCode=Service_SysGetSpecTypeChann(&loComm,&loDb);
			break;

		case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
			*pModule->pCode=Service_SysGetMachineInfoType(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_VIBINFO:
			*pModule->pCode=Srv_GetVibInfo(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_DYNINFO:
			*pModule->pCode=Srv_GetDynInfo(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_SET_DISK_FULL:
			*pModule->pCode= 1;
			g_bIsDiskFull = TRUE;
			break;

		case SERVICE_SERVICE_ON_ALARM_CONFIRM:
			*pModule->pCode= Srv_Alarm_Confirm(&loComm,&loDb);;
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE:
			*pModule->pCode=ReceiveSudVibValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE:
			*pModule->pCode=ReceiveSudDynValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEPROC:
			*pModule->pCode=ReceiveSudProcValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE:
			*pModule->pCode=ReceiveVibFastChangeValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE:
			*pModule->pCode=ReceiveDynFastChangeValue(&loComm,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE:
			*pModule->pCode=ReceiveProcFastChangeValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECPROCFASTALARMDATA:
			*pModule->pCode=Srv_OnReceiveProcFastAlarmData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECVIBFASTALARMDATA:
			*pModule->pCode=Srv_OnReceiveVibFastAlarmData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECDYNFASTALARMDATA:
			*pModule->pCode=Srv_OnReceiveDynFastAlarmData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE:
			*pModule->pCode=Srv_OnReceiveVibWirelessValue(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_SAVESPECTIMEDATA:
			*pModule->pCode = SaveSpecTypeDataByTimeRegion(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION:
			*pModule->pCode = ChangeSpecChannAlarmLimitation(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_ALARM_LIMITATION:
			*pModule->pCode = GetSpecChannAlarmLimitation(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION:
			*pModule->pCode = GetPlantGraphInformationS(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO:
			*pModule->pCode = GetPlantGraphPositonAndAlarmInfo(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_PLANT_STATUS:
			*pModule->pCode = Srv_SetPlantStatus(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_PLANT_STATUS:
			*pModule->pCode = Srv_GetPlantStatus(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS:
			*pModule->pCode = SetWireLessChannStatus(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS:
			*pModule->pCode = GetWireLessChannStatus(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS:
			*pModule->pCode = GetMWLinkStatus(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_VIB_DATA:
			*pModule->pCode= Srv_OnReceiveSudVibData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_DYN_DATA:
			*pModule->pCode= Srv_OnReceiveSudDynData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_PROC_DATA:
			*pModule->pCode= Srv_OnReceiveSudProcData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT:
			*pModule->pCode= Srv_OnGetLogFileContent(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD:
			*pModule->pCode = Srv_OnGetChannGradeThreshold(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD:
			*pModule->pCode = Srv_OnSetChannGradeThreshold(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM:
			*pModule->pCode = Srv_OnGetAxesLocationParam(&loComm,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM:
			*pModule->pCode = Srv_OnSetAxesLocationParam(&loComm,&loDb);
			break;
		default:
			return false;
		}
	}
	catch (CMemoryException* e)
	{
		CString lstrErrorMessage;
		e->GetErrorMessage(lstrErrorMessage.GetBufferSetLength(1024),255);
		lstrErrorMessage.ReleaseBuffer();
		BHLOG_FATAL(lstrErrorMessage);
		BOOL lbShouldExit = FALSE;
		lbShouldUpdateWatchDog = false;
		if (lstrErrorMessage.Find(_T("Out of memory"))>=0)
		{
			lbShouldExit = TRUE;
		}
		e->Delete();	
		if (lbShouldExit)
		{
			QuitProcess(::GetCurrentProcessId());
		}
	}
	
	lnCode = *pModule->pCode;

	int SvcCode =  pModule->pFixComm->GetFunc();

	bool lbRealTimeInterfaceFeedWatchDog = false;
	bool lbOtherInterfaceFeedWatchDog = false;

	if(g_map_RealTimeInterface.find(SvcCode) != g_map_RealTimeInterface.end())
	{
		lbRealTimeInterfaceFeedWatchDog = true;
	}

	if(g_map_OtherInterface.find(SvcCode) != g_map_OtherInterface.end())
	{
		lbOtherInterfaceFeedWatchDog = true;
	}

	if ( lnCode >=0 && lbShouldUpdateWatchDog > 0 && (lbRealTimeInterfaceFeedWatchDog || lbOtherInterfaceFeedWatchDog))
	{ 
		UpdateWatchDogFileTime();
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
