// stdafx.cpp : source file that includes just the standard includes
//	S_OnlineSvr.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include "OnlineGlobal.h"
#include "SpeedUpDownDataSave.h"
#include "NetOffDataSave.h"
#include "additions/ReaderWriterlock.h"
#include "additions/DebugHelper.h"
#include <BHKD_utilities.h>
#include "process.h"
#include <MiddleWare.h>
#include "MonitorICEServer.h"
#include "mdump.h"

bool _g_bInit=false;

 int g_UseNewAlarmLogic = 0;
 int g_nNewAlarmMaxTime = 2;					 //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
 int g_nSaveIndenseTime =5;						 //密集保存时间间隔,							默认5分钟
 int g_nSaveHalfIndenseTime =30 ;				 //半密集保存时间间隔,							默认30秒
 int g_nHalfNormalNotSaveTime = 20;				 //半正常状态下持续保存的时间长度，				默认20分钟
 int g_nSaveSparseTime =30;						 //就报警每30分钟保存一组的时间间隔,			默认30分钟
 int g_nHalfNormalTime =2 ;						 //半正常状态维持时间间隔						默认2个小时
 int g_nChannHalfNormalTime =1;					 //测点半正常状态维持时间间隔					默认1个小时
 int g_UseNewDCAlarmLogic = 0;					 //是否使用新的直流量报警逻辑					默认否
 int g_SaveDataIntervalForNewDCAlarmLogic = 120; //为新的直流量报警逻辑保存数据的时间间隔		默认120分钟
 int g_wave_diff_ratio = 80;					 //配置波形相似度的值							默认80%
 int g_alarm_larger_times = 3;					 //连续超过上次报警的次数						默认3
 int g_rev_threshold = 15;						//开停车判断判定开停车最小转速阈值	            默认250RPM
 int g_UseNewAxisAlarmLogic = 0;				//是否使用新的轴位移报警逻辑					默认否
 int g_SaveFastAndRegularAlarmData	= 1;         //快变报警判断是否同时保存快变和常规报警数据    默认是
 int g_SaveDataOnStop				= 1;        //停车状态下是否保存日常数据表中的秒表，小时表，天表数据
 int g_SaveMonthDataOnStop		= 1;			//非临时在线模式下停车状态下是否保存日常数据表中的月表数据
 int g_SaveAllFastData              = 0;        //是否保存所有快变数据，0：仅保存快变且发生报警的数据，1：保存所有快变数据，默认0
 int g_SaveAllFastDataWave          = 1;        //是否保存所有快变数据波形
 int g_SaveFastDataWaveInterval     = 10;       //保存快变波形时间间隔
 int g_SaveUseOptimizeMode         = 0;        //使用优化存储模式
 int g_UpdateProcDataOverTime = 5;              //过程量测点数据更新超时时间                    默认5分钟
 int g_UpdatePlantDataOverTime = 30;             //设备数据更新超时时间                         默认30分钟


 int g_rev_threshold_sink  = 250; 

 std::map< pair<CBHDateTime,CBHDateTime>,map<CString,int> > g_map_SpecSaveTime;

std::map< int,int > g_map_RealTimeInterface;

std::map< int,int > g_map_OtherInterface;


#include <IServer.h>
#include "../ICEServer/ICEServerExport.h"
LY::IServer * g_pServer = NULL;
int * g_nServiceMark = 0;
std::map<DWORD,CCustomDB *> * gpDBPool = NULL;
CReaderWriterLock *g_pConnPoolLock = NULL;
int g_nDefaultNetOffTimeSpan = 0;


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

bool QuitProcess(DWORD dwPID_)
{
	HWND hWnd = ::GetTopWindow(0);

	while ( hWnd )
	{

		DWORD dwPid = 0;

		DWORD dwTheardId = GetWindowThreadProcessId( hWnd,&dwPid);

		if (dwTheardId != 0)
		{
			if ( dwPid == dwPID_)
			{
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				CString strText;
				pWnd->GetWindowText(strText);
				if(strText.Find("业务服务系统")>=0)
				{
					PostMessage(hWnd,WM_QUIT,NULL,NULL);
				}
			}
		}
		hWnd = ::GetNextWindow( hWnd , GW_HWNDNEXT);

	}
	return TRUE;
}

int g_nUpdateWatchDog = 1;
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
	case SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT:
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
	case SERVICE_SERVICE_ON_TEST:
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
	case SERVICE_SERVICE_ON_SET_PLANT_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GET_PLANT_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_NOTIFY_ICE:
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
	case SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT:
		lbNeedOpenDB=true;
		break;
	case  SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM:
		lbNeedOpenDB = true;
		break;
	case  SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM:
		lbNeedOpenDB = true;
		break;
	default:
		return false;
	}

	if (!lbNeedOpenDB)
	{
		return false;
	}

	COleSqlServerDB loDb;

	if (!loDb.IsDbSafe())
	{
		return -1;
	}

	int lnCode = -1;

	int lnUpdateWatchDog = 1;

	try
	{
		CCustomDB * lpDb = GetPDB();
		loDb.setPDB(lpDb);

		switch ( SvcCode )
		{
		case SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT:
			lnCode=Srv_OnBatchGetMonitorAlarmStat(&arefCommInterFace,&loDb);
			break;//中间件模块
			//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
		case SERVICE_SERVICE_ON_GETALLFIVECHAR:
			lnCode =  Service_OnGetAllFiveChar(&arefCommInterFace , &loDb );
			break;
			//得到指定机组指定测点的五种类型特征值
		case SERVICE_SERVICE_ON_GETSPECFIVECHAR:
			lnCode=Service_OnGetSpecFiveChar(&arefCommInterFace , &loDb);
			break;
			//得到指定机组的任意测点的类型特征值
		case SERVICE_SERVICE_ON_GETALLTYPECHAR:
			lnCode= Srv_OnGetAllTypeChar(&arefCommInterFace , &loDb);
			break;
			//得到指定多个振动测点的频谱数据
		case SERVICE_SERVICE_ON_GETSPECTRUM:
			lnCode=Srv_OnGetSpectrum(&arefCommInterFace,&loDb);
			break;
			//得到指定机组指定四个振动测点指定类型的特征值
		case SERVICE_SERVICE_ON_GETFOURSPECCHAR:
			lnCode=SerVice_OnGetFourSpecChar(&arefCommInterFace , &loDb);
			break;
			//得到所有过程量的测点值
		case SERVICE_SERVICE_ON_GETALLPROCVALUE:
			lnCode=SerVice_OnGetAllProcValue(&arefCommInterFace , &loDb);
			break;
			//得到指定机组指定过程量测点值
		case SERVICE_SERVICE_ON_GETSPECPROCVALUE:
			lnCode=SerVice_OnGetSpecProcValue(&arefCommInterFace , &loDb);
			break;
			//得到指定机组两个振动测点的波形和频谱数据
		case SERVICE_SERVICE_ON_GETWAVE:
			lnCode= Service_OnGetWave(&arefCommInterFace , &loDb );
			break;
			//得到指定机组指定一组测点的轴心轨迹
		case SERVICE_SERVICE_ON_GETAXESTRACK:
			lnCode= Service_OnGetAxesTrack(&arefCommInterFace , &loDb );
			break;
			//得到指定机组指定一组测点轴心位置值
		case SERVICE_SERVICE_ON_GETAXESLOCATION:
			lnCode= Service_OnGetAxesLocation(&arefCommInterFace , &loDb );
			break;
			//得到指定机组的设备转速
		case SERVICE_SERVICE_ON_GETREV:
			lnCode= Service_OnGetRev(&arefCommInterFace , &loDb );
			break;
			//得到指定机组指定轴位移测点值
		case SERVICE_SERVICE_ON_GETSPECAXISDISPLACE:
			lnCode= Service_OnGetSpecAxisDisplace(&arefCommInterFace , &loDb );
			break;
			//得到指定机组所有轴位移的值
		case SERVICE_SERVICE_ON_GETALLAXISDISPLACE:
			lnCode= Service_OnGetAllAxisDisplace(&arefCommInterFace , &loDb );
			break;
			//得到指定用户所属设备的报警状态
		case SERVICE_SERVICE_ON_GETALLCHANNELSTATE:
			lnCode= Service_OnGetAllChannelState(&arefCommInterFace , &loDb );
			break;
			//得到指定机组所有测点的值
		case SERVICE_SERVICE_ON_GETALLCHANNVALUE:
			lnCode= Service_OnGetAllChannelValue(&arefCommInterFace , &loDb );
			break;
			//得到指定机组振动测点的相位
		case SERVICE_SERVICE_ON_GETPVDATA:
			lnCode= Srv_OnGetPVData(&arefCommInterFace , &loDb );
			break;
		case SERVICE_SERVICE_ON_GETRODSINKDATA:
			lnCode=Srv_OnGetRodSinkData(&arefCommInterFace,&loDb);
			break;
			//得到指定分厂下面设备的状态
		case SERVICE_SERVICE_ON_GETSUBCORPSTATE:
			lnCode= Service_OnGetSubCorpState(&arefCommInterFace , &loDb );
			break;
			//得到指定机组测点的波特图
		case SERVICE_SERVICE_ON_GETBODE:
			lnCode= Service_OnGetBode(&arefCommInterFace , &loDb );
			break;
			//得到指定机组测点的瀑布图
		case SERVICE_SERVICE_ON_GETWATERFALL:
			lnCode= Service_OnGetWaterFall(&arefCommInterFace , &loDb );
			break;
			//得到指定机组测点的细化谱图
		case SERVICE_SERVICE_ON_GETZOOMSPEC:
			lnCode= Service_OnGetZoomSpec(&arefCommInterFace , &loDb );
			break;
			//得到指定机组和测点的转速和采样信息
		case SERVICE_SERVICE_ON_GETSPECCHANNINFO:
			lnCode=Service_OnGetSpecChannInfo(&arefCommInterFace , &loDb );
			break;
			//得到指定机组的振动测点的幅值和相位
		case SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE:
			lnCode=Service_OnGetAllSpecCharAndPhase(&arefCommInterFace , &loDb );
			break;
			//得到指定机组的多参数分析数据
		case SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA:
			lnCode=Srv_OnGetRcMultiParamData(&arefCommInterFace,&loDb);
			break;
			//得到集团级报警统计数据
		case SERVICE_SERVICE_ON_GROUPALARMSTAT:
			lnCode=Srv_OnGetGroupAlarmStat(&arefCommInterFace,&loDb);
			break;
			//得到公司级报警统计数据
		case SERVICE_SERVICE_ON_COMPANYALARMSTAT:
			lnCode=Srv_OnGetCompanyAlarmStat(&arefCommInterFace,&loDb);
			break;
			//得到监测站级报警统计数据
		case SERVICE_SERVICE_ON_MONITORALARMSTAT:
			lnCode=Srv_OnGetMonitorAlarmStat(&arefCommInterFace,&loDb);
			break;
			//得到指定机组指定一组测点的活塞杆轴心位置轨迹
		case SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION:
			lnCode=Srv_OnGetRodSinkAxesLocation(&arefCommInterFace,&loDb);
			break;
			//得到指定机组指定两个测点的活塞杆应力变化数据
		case SERVICE_SERVICE_ON_GETDYNSTRESSWAVE:
			lnCode=Srv_OnGetDynStressWave(&arefCommInterFace,&loDb);
			break;
			//得到某一组提纯轴心轨迹的波形数据
		case SERVICE_SERVICE_ON_GETPURIFYAXESTRACK:
			lnCode=Srv_OnGetPurifyAxesTrack(&arefCommInterFace,&loDb);
			break;

			//接受传递进来的所有测点的实时特征值、轴位移和过程量数据,轴位置
		case SERVICE_SERVICE_ON_RECEIVEALLVALUE:
			lnCode=Service_OnReceiveVibValue(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEDYNVALUE:
			lnCode=Srv_OnReceiveDynValue(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEREV:
			lnCode=Service_OnReceiveRev(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVEPROC:
			lnCode=Service_OnReceiveProc(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVECHANNSTATE:
			lnCode=Service_OnReceiveChannState(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVECHANNSTATE_WITH_ALARM_LEVEL:
			lnCode=Service_OnReceiveChannStateWithAlarmLevel(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_REVSUDDATA:
			lnCode=Service_OnRevSudData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SAVEDATA:
			lnCode=Service_OnSrvSaveData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_JUDGEALARM:
			lnCode=Service_OnSrvJudgeAlarm(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_REQTHESHOLDID:
			lnCode=Service_OnReqThresholdID(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_REQFASTALARMEVENTID:
			lnCode=Srv_OnReqFastAlarmEventID(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS:
			lnCode=Srv_OnUpdateFastAlarmStatus(&arefCommInterFace,&loDb);
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
		case SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE:
			lnCode=Srv_NetOffReceiveAllWirelessValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA:
			lnCode=Service_OnGetSpecPlantProcData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE:
			lnCode=Service_OnGetSpecPlantAllState(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETCEPSTRUM:
			lnCode=Srv_OnGetCepstrum(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETFULLSPECSTRUM:
			lnCode=Srv_OnGetFullSpectrum(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA:
			lnCode=Service_OnGetSpecPlantLengthVibData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA:
			lnCode=Service_OnGetSpecPlantLengthDynData(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD:
			lnCode=Service_OnGetAbnormalDataRecord(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
			lnCode=Service_OnRcvDAQHWStatus(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
			lnCode=Service_OnRcvDAQSWStatus(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SYSTEM_GETSPECTYPECHANN:
			lnCode=Service_SysGetSpecTypeChann(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE:
			lnCode=Service_SysGetMachineInfoType(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_VIBINFO:
			lnCode=Srv_GetVibInfo(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_GET_DYNINFO:
			lnCode=Srv_GetDynInfo(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_SET_DISK_FULL:
			lnCode= 1;
			g_bIsDiskFull = TRUE;
			break;

		case SERVICE_SERVICE_ON_ALARM_CONFIRM:
			lnCode= Srv_Alarm_Confirm(&arefCommInterFace,&loDb);;
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE:
			lnCode=ReceiveSudVibValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE:
			lnCode=ReceiveSudDynValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_SUD_RECEIVEPROC:
			lnCode=ReceiveSudProcValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE:
			lnCode=ReceiveVibFastChangeValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE:
			lnCode=ReceiveDynFastChangeValue(&arefCommInterFace,&loDb);
			break;

		case SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE:
			lnCode=ReceiveProcFastChangeValue(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECPROCFASTALARMDATA:
			lnCode=Srv_OnReceiveProcFastAlarmData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECVIBFASTALARMDATA:
			lnCode=Srv_OnReceiveVibFastAlarmData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECDYNFASTALARMDATA:
			lnCode=Srv_OnReceiveDynFastAlarmData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE:
			lnCode=Srv_OnReceiveVibWirelessValue(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SAVESPECTIMEDATA:
			lnCode = SaveSpecTypeDataByTimeRegion(&arefCommInterFace,&loDb);
			break;	
		case  SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION:
			lnCode = ChangeSpecChannAlarmLimitation(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_ALARM_LIMITATION:
			lnCode = GetSpecChannAlarmLimitation(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION:
			lnCode = GetPlantGraphInformationS(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO:
			lnCode = GetPlantGraphPositonAndAlarmInfo(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS:
			lnCode = SetWireLessChannStatus(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS:
			lnCode = GetWireLessChannStatus(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_PLANT_STATUS:
			lnCode = Srv_SetPlantStatus(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_PLANT_STATUS:
			lnCode = Srv_GetPlantStatus(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_TEST:
			lnCode = Srv_test(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_NOTIFY_ICE:
			g_nUpdateWatchDog = NotUpdateICEWatchDog(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_VIB_DATA:
			lnCode= Srv_OnReceiveSudVibData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_DYN_DATA:
			lnCode= Srv_OnReceiveSudDynData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_SUD_PROC_DATA:
			lnCode= Srv_OnReceiveSudProcData(&arefCommInterFace,&loDb);
			break;
		case SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT:
			lnCode= Srv_OnGetLogFileContent(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD:
			lnCode = Srv_OnGetChannGradeThreshold(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD:
			lnCode = Srv_OnSetChannGradeThreshold(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM:
			lnCode = Srv_OnGetAxesLocationParam(&arefCommInterFace,&loDb);
			break;
		case  SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM:
			lnCode = Srv_OnSetAxesLocationParam(&arefCommInterFace,&loDb);
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
		lnCode = -1;
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

	if ( lnCode >=0 && g_nUpdateWatchDog > 0 && (lbRealTimeInterfaceFeedWatchDog || lbOtherInterfaceFeedWatchDog))
	{ 
		UpdateWatchDogFileTime();
	}



	return lnCode;

}

BOOL InitICEServer()
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









CCompanyInfoPool * g_pCompanyPoolReal=NULL;
CCompanyInfoPool * g_pCompanyPoolAlarm=NULL;
CCompanyInfoPool * g_pCompanyPoolSud=NULL;
CCompanyInfoPool * g_pCompanyPoolNetOff=NULL;

MachIneInfoTypeCollection * g_pMachIneInfoTypeCollection;

CString g_strDbUser;
CString g_strDbPwd;
CString g_strDbName;
CString g_strDbServer;


CBHDateTime g_dtInitTime=CBHDateTime::GetCurrentTime();

CReaderWriterLock * g_pDBConnectLock = NULL;

BOOL g_bSkipNetoffFunc = FALSE;

BOOL g_bSkipAlarm = FALSE;

BOOL g_bIsShouldSaveEmptyValue = FALSE;

BOOL g_bIsDiskFull = FALSE;

BOOL g_bOpenNetOffStatus = TRUE;

CRITICAL_SECTION * g_pSavingCS = NULL;
CRITICAL_SECTION * g_MapLock = NULL;
CRITICAL_SECTION * g_pUpdateParamLock = NULL;

int G_D_ALARM_SAVEWAVE_NUM = D_ALARM_SAVEWAVE_NUM;

/*!秒趋势（当前趋势）表单中，保存记录的最大条数。根据宏和配置文件来取*/
int G_D_TREND_SECONDRANGE =0;

/*!小时趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
int G_D_TREND_HOURRANGE =0;

/*!天趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
int G_D_TREND_DAYRANGE =0;

/*!快变趋势表单中，保存记录的最大条数。根据宏和配置文件来取*/
int G_D_TREND_FASTRANGE =0;

/*!启停车表单中，保存记录的最大条数。根据宏和配置文件来取*/
int G_D_SUD_DATARANGE =0;

/*!是否开启停车仅保存月表策略*/
int G_TEMP_ONLINE = 0;

/*!最小采样点数，小于此采样点数则中间件不处理*/
int G_SAMPLE_LIMITATION = 256;

/*!报警判断判定区间超限阈值百分比为整数*/
int G_ALARM_THRESHOLD = 15;

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

int InitServerStatus ()
{
	if (NULL == g_pServerStatusLock)
	{
		g_pServerStatusLock = new CCriticalSection();
	}
	g_pServerStatusLock->Lock();
	
BEGIN_ERROR_HANDLE;

	g_pServerStatus  = ::GetServerStatus();

END_ERROR_HANDLE;

	g_pServerStatusLock->Unlock();

	return TRUE;
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


BOOL InitSaveDataPolicy();

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

BOOL IsEnableTempOnline()
{
	char cAppLocation[5000]={0};

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	//秒表数据行数
	int lnData = ::GetPrivateProfileInt("SYSTEM",
		"D_TREND_SECONDRANGE"
		,D_TREND_SECONDRANGE,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = D_TREND_SECONDRANGE;
	}

	G_D_TREND_SECONDRANGE = lnData;

	BHLOG_DEBUG(STR(_T("秒表保存条目数 :%d"),G_D_TREND_SECONDRANGE));

	//小时表数据行数
	lnData = ::GetPrivateProfileInt("SYSTEM",
		"D_TREND_HOURRANGE"
		,D_TREND_HOURRANGE,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = D_TREND_HOURRANGE;
	}

	G_D_TREND_HOURRANGE = lnData;

	BHLOG_DEBUG(STR(_T("小时表保存条目数: %d"),G_D_TREND_HOURRANGE));

	//天表数据行数
	lnData = ::GetPrivateProfileInt("SYSTEM",
		"D_TREND_DAYRANGE"
		,D_TREND_DAYRANGE,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = D_TREND_DAYRANGE;
	}

	G_D_TREND_DAYRANGE = lnData;
	BHLOG_DEBUG(STR(_T("天表保存条目数: %d"),G_D_TREND_DAYRANGE));

	//天表数据行数
	lnData = ::GetPrivateProfileInt("SYSTEM",
		"D_TREND_FASTRANGE"
		,D_TREND_FASTRANGE,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = D_TREND_FASTRANGE;
	}

	G_D_TREND_FASTRANGE = lnData;
	BHLOG_DEBUG(STR(_T("快变表保存条目数: %d"),G_D_TREND_FASTRANGE));

	//启停车表数据行数
	lnData = ::GetPrivateProfileInt("SYSTEM",
		"D_SUD_DATARANGE"
		,D_SUD_DATARANGE,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = D_SUD_DATARANGE;
	}

	G_D_SUD_DATARANGE = lnData;
	BHLOG_DEBUG(STR(_T("启停车表保存条目数: %d"),G_D_SUD_DATARANGE));


	//开启停车仅保存月表策略
	lnData = ::GetPrivateProfileInt("SYSTEM",
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

	G_TEMP_ONLINE = lnData;

	BHLOG_DEBUG(STR(_T("开启停车仅保存月表策略: %d"),G_TEMP_ONLINE));

	return TRUE;
}


int DefaultNetOffTimeSpan()
{

	char cAppLocation[500]={0};

	GetCurrentDirectory(500,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	int lnDefaultNetOffTimeSpan = ::GetPrivateProfileInt("SYSTEM","DEFAULT_NET_OFF_TIME_SPAN",10,cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lnDefaultNetOffTimeSpan);
	::WritePrivateProfileString("SYSTEM","DEFAULT_NET_OFF_TIME_SPAN",lstrData.GetBuffer(0),cAppLocation);

	return lnDefaultNetOffTimeSpan;
}

BOOL IsSkipNetoffFunc()
{
	char cAppLocation[500]={0};

	GetCurrentDirectory(500,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbSkipNetOff = ::GetPrivateProfileInt("SYSTEM","SKIP_NET_OFF_SAVE",FALSE,cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lbSkipNetOff);
	::WritePrivateProfileString("SYSTEM","SKIP_NET_OFF_SAVE",lstrData.GetBuffer(0),cAppLocation);

	return lbSkipNetOff;
}

BOOL IsSkipAlarmFunc()
{
	char cAppLocation[5000]={0};

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbSkipAlarm = ::GetPrivateProfileInt("SYSTEM","SKIP_ALARM",TRUE,cAppLocation);

	CString lstrSkipAlarm;

	lstrSkipAlarm.Format(_T("%d"),lbSkipAlarm);

	::WritePrivateProfileString("SYSTEM","SKIP_ALARM",lstrSkipAlarm.GetBuffer(0),cAppLocation);

	return lbSkipAlarm;
}

BOOL IsOpenNetOffStatus()
{
	char cAppLocation[5000]={0};

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbOpenNetOffStatus = ::GetPrivateProfileInt("SYSTEM","OPEN_NET_OFF_STATUS",TRUE,cAppLocation);

	CString lstrOpenNetOffStatus;

	lstrOpenNetOffStatus.Format(_T("%d"),lbOpenNetOffStatus);

	::WritePrivateProfileString("SYSTEM","OPEN_NET_OFF_STATUS",lstrOpenNetOffStatus.GetBuffer(0),cAppLocation);

	return lbOpenNetOffStatus;
}

BOOL IsSaveEmptyValue()
{
	char cAppLocation[5000]={0};

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbSaveEmptyValue = ::GetPrivateProfileInt("SYSTEM","SAVE_EMPTY_VALUE",FALSE,cAppLocation);

	CString lstrData;
	lstrData.Format(_T("%d"),lbSaveEmptyValue);
	::WritePrivateProfileString("SYSTEM","SAVE_EMPTY_VALUE",lstrData.GetBuffer(0),cAppLocation);

	return lbSaveEmptyValue;
}

BOOL InitRetainDataTime()
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
	CString lstrSplitValue1 = _T("|");
	CString lstrBuffer;
	CStringArray loPlantArray;
	CStringArray loValueArray;
	CStringArray loTimeTblArray;

	std::map< pair<CBHDateTime,CBHDateTime>, map<CString,int> >::iterator it;

	int lnData = ::GetPrivateProfileString("SAVE_DATA",
		"SAVE_SPEC_DATA_TIME"
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
			loTimeTblArray.RemoveAll();
			GetSplitStrArray(lstrKey,lstrSplitValue1,loTimeTblArray,TRUE);
			if(loTimeTblArray.GetSize() == 3)
			{
				CBHDateTime ldtStart,ldtEnd;

				CString lstrBeginTime = loTimeTblArray.GetAt(0);
				CString lstrEndTime = loTimeTblArray.GetAt(1);
				CString lstrTbleName = loTimeTblArray.GetAt(2);

				lstrTbleName = lstrTbleName.MakeUpper();


				ldtStart.ParseDateTime(lstrBeginTime);
				ldtEnd.ParseDateTime(lstrEndTime);
				CString lstrValue = loValueArray.GetAt(1);
				int lnStatus = _ttoi(lstrValue.GetBuffer(0));
				std::pair<CBHDateTime,CBHDateTime> lpTimeRange(ldtStart,ldtEnd);
				if((it = g_map_SpecSaveTime.find(lpTimeRange)) != g_map_SpecSaveTime.end())
				{
					it->second.insert(make_pair(lstrTbleName,lnStatus));
					g_map_SpecSaveTime.insert(make_pair(lpTimeRange,it->second));
				}
				else
				{
					std::map<CString,int> lmTmp;
					lmTmp.insert(make_pair(lstrTbleName,lnStatus));
					g_map_SpecSaveTime.insert(make_pair(lpTimeRange,lmTmp));
				}
				
				BHLOG_DEBUG(STR(_T("%s|%d"),lstrKey,lnStatus));
			}


		}
	}
	return TRUE;
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
/*****************************初始化函数和退出一些处理***************************************/
void InitGlobalVar(CCustomDB *apDB_)
{	

	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
	SetGlobalErrorMode(2);
	::SetUnhandledExceptionFilter(Winet::GEHExceptionFilter);
	g_pSavingCS = new CRITICAL_SECTION();
	g_MapLock   = new CRITICAL_SECTION();

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	if (g_pDBConnectLock == NULL)
	{
		g_pDBConnectLock = new CReaderWriterLock();
	}
	if(g_pUpdateParamLock == NULL)
	{
		g_pUpdateParamLock = new CRITICAL_SECTION();
	}
	

    CString strLogPath="";
	int iLogFlag=0;
	iLogFlag=GetLogOutputFlag(strLogPath);
	CHZLogManage::StartLogTxt(strLogPath,"KDMW",iLogFlag);
	{
		CBHDateTime dtTmp;
		dtTmp=CBHDateTime::GetCurrentTime();
		g_dtInitTime.SetDateTime(dtTmp.GetYear(),dtTmp.GetMonth(),1,1,0,0);
	}

	g_bSkipNetoffFunc = IsSkipNetoffFunc();

	g_bSkipAlarm = IsSkipAlarmFunc();

	g_bIsShouldSaveEmptyValue = IsSaveEmptyValue();

	g_nDefaultNetOffTimeSpan = DefaultNetOffTimeSpan();

	g_bOpenNetOffStatus = IsOpenNetOffStatus();


	InitAlarmParameters();
	InitMWRevThreshold();
	InitSaveDataPolicy();
	InitRetainDataTime();
	InitFeedWatchDogInterface();
	

	if (g_bSkipAlarm)
	{
		G_D_ALARM_SAVEWAVE_NUM = 1;
	}else
	{
		G_D_ALARM_SAVEWAVE_NUM = D_ALARM_SAVEWAVE_NUM;
	}

	G_SAMPLE_LIMITATION = GetMinSampleNum();

	G_ALARM_THRESHOLD = GetAlarmThreshold();

	IsEnableTempOnline();

	InitializeCriticalSection(g_pSavingCS);
	InitializeCriticalSection(g_MapLock);
	InitializeCriticalSection(g_pUpdateParamLock);

	g_pCompanyPoolReal=new CCompanyInfoPool;
	g_pCompanyPoolAlarm=new CCompanyInfoPool;
	g_pCompanyPoolSud=new CCompanyInfoPool;
	g_pCompanyPoolNetOff= new CCompanyInfoPool;
    CString strLog="";//日志输出信息
	map<string,string> mapCompany;
	map<CString,CString> mapCompanyCountry;
	map<CString,CString> mapGroupDisplay;
	map<CString,CString> mapFactoryDisplay;
	map<CString,CString> mapSetDisplay;
	map<CString,CString> mapCompanyDisplay;
	list<string> listGroup;
	map<string,string>::iterator itCompany;
	list<string>::iterator itGroup;
	list<string> listFactory;
	list<CRealPlantInfo *> listPlantReal;
	list<CAlarmPlantInfo *> listPlantAlarm;
	list<CSudPlantInfo *> listPlantSud;
	list<CNetOffPlantInfo *> listPlantNetOff;
	string sAlias="";
	string sCompany="";
	string sGroup="";
	listGroup.clear();
	listFactory.clear();
	listPlantReal.clear();
	listPlantAlarm.clear();
	listPlantSud.clear();
	listPlantNetOff.clear();
	mapCompany.clear();

/*
	CString strserver,strdb,struser,strpwd;
	GetDBUidPwd(strdb,struser,strpwd,strserver);*/

	CCustomDB * lpDb =CreateHandleDataDBConn();	


	if (NULL==lpDb)
	{		
		CHZLogManage::Error("Connect DB failed!","OnlineGlobal","InitGlobalVar");	
	}

	if (!lpDb)
	{
		return ;
	}

	COleSqlServerDB loDb;

	loDb.setPDB(lpDb);

	if (!InitMachineInfoType(loDb))
	{
		ASSERT(FALSE);
	}

	
	
// 	IDBInterFace * pDB_ = NULL;
// 	pDB_ = &loDb;
// 	SwitchDbNameEx(pDB_,g_strMainDbName);
	InitServerStatus();
	CCustomDB * pDB_ = apDB_;
	SwichDbName(pDB_,g_strMainDbName);

	//得到集团信息
	pDB_->Cmd("SELECT * FROM [%s]",gc_cGroupTable);
	while(pDB_->More())
	{
		CString lstrGroupID = pDB_->Get(gc_cGroup);
		CString lstrGroupDisplay;
		BEGIN_ERROR_HANDLE;		
		lstrGroupDisplay= pDB_->Get(gc_cDisplay);
		lstrGroupDisplay.Trim();
		if (lstrGroupDisplay.IsEmpty())
		{
			lstrGroupDisplay = lstrGroupID;
		}
		END_ERROR_HANDLE;
		
		mapGroupDisplay[lstrGroupID] = lstrGroupDisplay;
	}

	pDB_->Cmd("SELECT distinct [%s],[%s],[%s],* FROM [%s]",gc_cCompany,gc_cCompanyAlias,gc_cGroup,gc_cCompanyTable);
    while(pDB_->More())//得到公司信息
	{
		sCompany=pDB_->Get(gc_cCompany);
		sAlias=pDB_->Get(gc_cCompanyAlias);
		sGroup=pDB_->Get(gc_cGroup);
		CString lstrGroupID = pDB_->Get(gc_cGroup);
		CString lstrCompanyID = pDB_->Get(gc_cCompany);
        mapCompany.insert(make_pair(sCompany,sAlias));
		
		CString lstrCompanyDisplay;
		
		BEGIN_ERROR_HANDLE;		
		lstrCompanyDisplay= pDB_->Get(gc_cDisplay);
		lstrCompanyDisplay.Trim();
		if (lstrCompanyDisplay.IsEmpty())
		{
			lstrCompanyDisplay = lstrCompanyID;
		}
		END_ERROR_HANDLE;

		CString lstrCompanyKey = lstrCompanyID;
		mapCompanyDisplay[lstrCompanyKey] = lstrCompanyDisplay;

		CString lstrCountry;
		BEGIN_ERROR_HANDLE;		
		lstrCountry =pDB_->Get(gc_cCountry);		
		END_ERROR_HANDLE;
		if (lstrCountry.IsEmpty())
		{
			lstrCountry = COUNTRY_DEFAULT;
		}
		mapCompanyCountry.insert(make_pair(CString(sCompany.c_str()),lstrCountry));
		listGroup.push_back(sGroup);

		strLog.Format("得到公司：%s,%s",sCompany.c_str(),sGroup.c_str());
		HZLOG_INFO(STR(strLog));
	}
	CFactoryInfoPool *pFactoryPoolReal=NULL;
	CFactoryInfoPool *pFactoryPoolAlarm=NULL;
	CFactoryInfoPool *pFactoryPoolSud=NULL;
	CFactoryInfoPool *pFactoryPoolNetOff=NULL;
	itGroup=listGroup.begin();
	for (itCompany=mapCompany.begin();itCompany!=mapCompany.end();++itCompany)
	{//得到公司下面的各个设备信息
		listFactory.clear();
		sCompany=itCompany->first;
		CString lstrCompany = sCompany.c_str();
		CString lstrCountry = mapCompanyCountry[lstrCompany];
		if (lstrCountry.IsEmpty())
		{
			lstrCountry = COUNTRY_DEFAULT;
		}
        sAlias=itCompany->second;
		sGroup=(*itGroup);
		++itGroup;
		pDB_->Cmd("SELECT * FROM %s ",gc_cFactoryTable);
		pDB_->Cmd(" WHERE [%s]='%s' ",gc_cCompany,sCompany.c_str());
		while (pDB_->More())
		{ //得到分厂列表
		   listFactory.push_back(pDB_->Get(gc_cFactory_Name));

		   CString lstrFactoryID = pDB_->Get(gc_cFactory_Name);
		   CString lstrCompanyID  = pDB_->Get(gc_cCompany);
		   CString lstrKey = lstrCompanyID +":" + lstrFactoryID;
		   CString lstrFactoryDisplay ;
		   BEGIN_ERROR_HANDLE;

		   lstrFactoryDisplay = pDB_->Get(gc_cDisplay);
		   lstrFactoryDisplay.Trim();
		   if (lstrFactoryDisplay.IsEmpty())
		   {
			   lstrFactoryDisplay = lstrFactoryID;
		   }
		   END_ERROR_HANDLE;
		   mapFactoryDisplay[lstrKey] = lstrFactoryDisplay;
		}

		pDB_->Cmd("SELECT * FROM %s ",gc_cSetTable);
		pDB_->Cmd(" WHERE [%s]='%s' ",gc_cCompany,sCompany.c_str());
		while (pDB_->More())
		{ //得到装置列表
			CString lstrFactoryID = pDB_->Get(gc_cFactory_Name);
			CString lstrCompanyID  = pDB_->Get(gc_cCompany);
			CString lstrSetID = pDB_->Get(gc_cSetID);
			CString lstrKey = lstrCompanyID +":" + lstrFactoryID + ":"+lstrSetID;
			CString lstrSetDisplay ;
			BEGIN_ERROR_HANDLE;

			lstrSetDisplay = pDB_->Get(gc_cDisplay);
			lstrSetDisplay.Trim();
			if (lstrSetDisplay.IsEmpty())
			{
				lstrSetDisplay = lstrSetID;
			}
			END_ERROR_HANDLE;
			mapSetDisplay[lstrKey] = lstrSetDisplay;
		}

		strLog.Format("%s 分厂个数：%d",sCompany.c_str(),listFactory.size());
		BHLOG_DEBUG(STR(strLog));

		if (listFactory.size()<1)
		{
			continue;
		}
		pFactoryPoolReal=new CFactoryInfoPool;
		pFactoryPoolAlarm=new CFactoryInfoPool;
		pFactoryPoolSud=new CFactoryInfoPool;
		pFactoryPoolNetOff=new CFactoryInfoPool;
		CPlantInfoPool *pPlantPoolReal=NULL;
		CPlantInfoPool *pPlantPoolAlarm=NULL;
		CPlantInfoPool *pPlantPoolSud=NULL;
		CPlantInfoPool *pPlantPoolNetOff=NULL;
		list<string>::iterator itFactory=listFactory.begin();
		for (;itFactory!=listFactory.end();++itFactory)
		{
			strLog.Format("初始化设备信息：%s,%s",sCompany.c_str(),(*itFactory).c_str());
			BHLOG_DEBUG(STR(strLog));

			CRealPlantInfo *pRealPlantInfo=NULL;
			CAlarmPlantInfo *pAlarmPlantInfo=NULL;
			CSudPlantInfo *pSudPlantInfo=NULL;
			CNetOffPlantInfo *pNetOffPlantInfo=NULL;
			listPlantSud.clear();
			listPlantReal.clear();
			listPlantAlarm.clear();
			listPlantNetOff.clear();
			S_PlantInfo sPlantInfo;
			S_PlantInfoParam _sPlantInfoParam;
			pDB_->Cmd("SELECT * from %s ",gc_cMachineTable);
			pDB_->Cmd(" WHERE %s='%s' AND %s='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,(*itFactory).c_str());
			while (pDB_->More())
			{
				sPlantInfo.GetPlantInfo(pDB_);

			    strLog.Format("设备初始化：%s,%s,%s,%f,%f,%f",sCompany.c_str(),(*itFactory).c_str(),sPlantInfo._cPlantID,_sPlantInfoParam._fCrankShaftDia,_sPlantInfoParam._fPistonDia,_sPlantInfoParam._fConnRodLength);
				BHLOG_DEBUG(STR(strLog));

				pRealPlantInfo=new CRealPlantInfo;
				pAlarmPlantInfo=new CAlarmPlantInfo;
				pSudPlantInfo=new CSudPlantInfo;

				pRealPlantInfo->Country(lstrCountry);
				pAlarmPlantInfo->Country(lstrCountry);
				pSudPlantInfo->Country(lstrCountry);


				//设备显示名称
				CString lstrDisplayName = pDB_->Get(gc_cDisplay);
				lstrDisplayName.Trim();
				if (lstrDisplayName.Trim().GetLength()==0)
				{
					lstrDisplayName = sPlantInfo._cPlantID;
				}

				pRealPlantInfo->PlantDisplayName(lstrDisplayName);
				pAlarmPlantInfo->PlantDisplayName(lstrDisplayName);
				pSudPlantInfo->PlantDisplayName(lstrDisplayName);
				
				CString lstrGroupID = sGroup.c_str();
				CString lstrGroupKey = lstrGroupID;
				CString lstrCompanyID = sCompany.c_str();
				CString lstrCompanyKey = lstrCompanyID;
				CString lstrFactoryID = (*itFactory).c_str();
				CString lstrFactoryKey = lstrCompanyID + ":" + lstrFactoryID;
				CString lstrSetID = sPlantInfo._cSetID;
				CString lstrSetKey =  lstrCompanyID + ":" + lstrFactoryID + ":"+ lstrSetID;

				CString lstrGroupDisplay = mapGroupDisplay[lstrGroupKey];
				CString lstrCompanyDisplay = mapCompanyDisplay[lstrCompanyKey];
				CString lstrFactoryDisplay = mapFactoryDisplay[lstrFactoryKey];
				CString lstrSetDisplay = mapSetDisplay[lstrSetKey];
				
				pRealPlantInfo->GroupDisplayName(lstrGroupDisplay);
				pRealPlantInfo->CompanyDisplayName(lstrCompanyDisplay);
				pRealPlantInfo->FactoryDisplayName(lstrFactoryDisplay);
				pRealPlantInfo->SetDisplayName(lstrSetDisplay);
				pRealPlantInfo->PlantDisplayName(lstrDisplayName);

				pAlarmPlantInfo->GroupDisplayName(lstrGroupDisplay);
				pAlarmPlantInfo->CompanyDisplayName(lstrCompanyDisplay);
				pAlarmPlantInfo->FactoryDisplayName(lstrFactoryDisplay);
				pAlarmPlantInfo->SetDisplayName(lstrSetDisplay);
				pAlarmPlantInfo->PlantDisplayName(lstrDisplayName);

				pSudPlantInfo->GroupDisplayName(lstrGroupDisplay);
				pSudPlantInfo->CompanyDisplayName(lstrCompanyDisplay);
				pSudPlantInfo->FactoryDisplayName(lstrFactoryDisplay);
				pSudPlantInfo->SetDisplayName(lstrSetDisplay);
				pSudPlantInfo->PlantDisplayName(lstrDisplayName);


				pRealPlantInfo->SetPlantInfo(sGroup,sCompany,sAlias,(*itFactory).c_str(),sPlantInfo._cSetID,sPlantInfo);
				pAlarmPlantInfo->SetPlantInfo(sGroup,sCompany,sAlias,(*itFactory).c_str(),sPlantInfo._cSetID,sPlantInfo);
				pAlarmPlantInfo->SetIDPointer(pRealPlantInfo);
				pSudPlantInfo->SetPlantInfo(sGroup,sCompany,sAlias,(*itFactory).c_str(),sPlantInfo._cSetID,sPlantInfo);
				
				listPlantSud.push_back(pSudPlantInfo);
				listPlantAlarm.push_back(pAlarmPlantInfo);
				listPlantReal.push_back(pRealPlantInfo);				

				if (!g_bSkipNetoffFunc)
				{
					pNetOffPlantInfo=new CNetOffPlantInfo;

					pNetOffPlantInfo->Country(lstrCountry);

					pNetOffPlantInfo->SetPlantInfo(sGroup,sCompany,sAlias,(*itFactory).c_str(),sPlantInfo._cSetID,sPlantInfo);

					listPlantNetOff.push_back(pNetOffPlantInfo);
				}

				strLog.Format("设备初始化：%s,%s,%s 设备信息设置完成",sCompany.c_str(),(*itFactory).c_str(),pRealPlantInfo->GetPlantID().c_str());
				BHLOG_DEBUG(STR(strLog));
			}
			if (listPlantReal.size()<1)
            {    
				 continue;
			}
            
			list<CRealPlantInfo *>::iterator itRealPlantInfo=listPlantReal.begin();
			list<CSudPlantInfo *>::iterator itSudPlantInfo=listPlantSud.begin();
			list<CAlarmPlantInfo *>::iterator itAlarmPlantInfo=listPlantAlarm.begin();
			list<CNetOffPlantInfo *>::iterator itNetOffPlantInfo=listPlantNetOff.begin();
			pPlantPoolAlarm=new CPlantInfoPool;
			pPlantPoolReal=new CPlantInfoPool;
			pPlantPoolSud=new CPlantInfoPool;

			if (!g_bSkipNetoffFunc)
			{
				pPlantPoolNetOff=new CPlantInfoPool;
			}

			strLog.Format("指定分厂的%s,设备数目：报警%d,实时%d,启停车%d,断网设备%d",(*itFactory).c_str(),listPlantAlarm.size(),listPlantReal.size(),listPlantSud.size(),listPlantNetOff.size());
			BHLOG_DEBUG(STR(strLog));

			for (;itRealPlantInfo!=listPlantReal.end();)
			{
				BHLOG_DEBUG(STR("itRealPlantInfo"));
				(*itRealPlantInfo)->SetDBConn(pDB_);
				(*itRealPlantInfo)->AddVibChannInfo(pDB_);
				(*itRealPlantInfo)->AddDynChannInfo(pDB_);
				(*itRealPlantInfo)->AddProcChannInfo(pDB_);
				(*itRealPlantInfo)->AddOfflineChannInfo(pDB_);
				(*itRealPlantInfo)->AddCustomParamInfo(pDB_);//添加通道的自定义参数详细信息
				(*itRealPlantInfo)->AddChannLocationsInfo(pDB_);//添加通道的位置信息
				(*itRealPlantInfo)->AddChannSectionsInfo(pDB_); //添加通道的部位信息
				(*itRealPlantInfo)->SetCharInfo(pDB_);
				(*itRealPlantInfo)->AddChannInfoEnd(pDB_);

				BHLOG_DEBUG(STR("itAlarmPlantInfo"));
				(*itAlarmPlantInfo)->AddVibChannInfo(pDB_);
				(*itAlarmPlantInfo)->AddDynChannInfo(pDB_);
				(*itAlarmPlantInfo)->AddProcChannInfo(pDB_);
				(*itAlarmPlantInfo)->AddOfflineChannInfo(pDB_);
				(*itAlarmPlantInfo)->AddCustomParamInfo(pDB_);//添加通道的自定义参数详细信息
				(*itAlarmPlantInfo)->AddChannLocationsInfo(pDB_);//添加通道的位置信息
				(*itAlarmPlantInfo)->AddChannSectionsInfo(pDB_); //添加通道的部位信息
				(*itAlarmPlantInfo)->AddCustomAlarmParamInfo(pDB_);//添加通道的自定义报警参数详细信息
				(*itAlarmPlantInfo)->AddChannInfoEnd(pDB_);
				
				if (!g_bSkipNetoffFunc)
				{
					BHLOG_DEBUG(STR("itNetOffPlantInfo"));
					(*itNetOffPlantInfo)->AddVibChannInfo(pDB_);
					(*itNetOffPlantInfo)->AddDynChannInfo(pDB_);
					(*itNetOffPlantInfo)->AddProcChannInfo(pDB_);
					(*itNetOffPlantInfo)->AddOfflineChannInfo(pDB_);
					(*itNetOffPlantInfo)->AddCustomParamInfo(pDB_);//添加通道的自定义参数详细信息
					(*itNetOffPlantInfo)->AddChannLocationsInfo(pDB_);//添加通道的位置信息
					(*itNetOffPlantInfo)->AddChannSectionsInfo(pDB_); //添加通道的部位信息
					(*itNetOffPlantInfo)->AddChannInfoEnd(pDB_);
				}


				BHLOG_DEBUG(STR("itSudPlantInfo"));
				(*itSudPlantInfo)->AddVibChannInfo(pDB_);
				if(!G_TEMP_ONLINE)
				{
					(*itSudPlantInfo)->AddChannInfoEnd();
				}
				else
				{
					(*itSudPlantInfo)->AddDynChannInfo(pDB_);
					(*itSudPlantInfo)->AddProcChannInfo(pDB_);
					(*itSudPlantInfo)->AddChannInfoEnd(pDB_);
				}
				

				pPlantPoolReal->AddInfoPool((*itRealPlantInfo)->GetPlantID(),*itRealPlantInfo);
				pPlantPoolAlarm->AddInfoPool((*itAlarmPlantInfo)->GetPlantID(),(*itAlarmPlantInfo));
				pPlantPoolSud->AddInfoPool((*itSudPlantInfo)->GetPlantID(),(*itSudPlantInfo));

				if (!g_bSkipNetoffFunc)
				{
					pPlantPoolNetOff->AddInfoPool((*itNetOffPlantInfo)->GetPlantID(),(*itNetOffPlantInfo));
				}

				strLog.Format("设备通道信息初始化：%s,%s,%s 通道信息设置完成",sCompany.c_str(),(*itFactory).c_str(),(*itRealPlantInfo)->GetPlantID().c_str());
				BHLOG_DEBUG(STR(strLog));


				

				LY::PLANTINFO_STATUS loStatus;
				loStatus.m_strGroup = (*itRealPlantInfo)->GetGroupName().c_str();
				loStatus.m_strCompany = (*itRealPlantInfo)->GetCompanyName().c_str();
				loStatus.m_strFactory = (*itRealPlantInfo)->GetFactoryName().c_str();
				loStatus.m_strPlant = (*itRealPlantInfo)->GetPlantID().c_str();
				loStatus.m_strSet = (*itRealPlantInfo)->GetSetID().c_str();

				loStatus.m_nVibChannCount = (*itRealPlantInfo)->_vVibChann.size();
				loStatus.m_nDynChannCount = (*itRealPlantInfo)->_vDynChann.size();
				loStatus.m_nProcChannCount = (*itRealPlantInfo)->_vProcChann.size();

				CBHDateTime loCurrentTime(1970,1,1,1,1,1,1);
				loStatus.m_strVibUpdateTime = loCurrentTime.Format();
				loStatus.m_strDynUpdateTime = loStatus.m_strVibUpdateTime;
				loStatus.m_strProcUpdateTime = loStatus.m_strVibUpdateTime;
				
				loStatus.m_strVibSaveTime = loStatus.m_strVibUpdateTime;
				loStatus.m_strDynSaveTime = loStatus.m_strVibUpdateTime;
				loStatus.m_strProcSaveTime = loStatus.m_strVibUpdateTime;

				if (NULL!= g_pServerStatus)
				{
					g_pServerStatus->UpdateStatistics(loStatus);

				}

				++itRealPlantInfo;
				++itSudPlantInfo;
				++itAlarmPlantInfo;

				if (!g_bSkipNetoffFunc)
				{
					++itNetOffPlantInfo;
				}
			}
		    pFactoryPoolSud->AddInfoPool((*itFactory),pPlantPoolSud);
			pFactoryPoolAlarm->AddInfoPool((*itFactory),pPlantPoolAlarm);
			pFactoryPoolReal->AddInfoPool((*itFactory),pPlantPoolReal);
			
			if (!g_bSkipNetoffFunc)
			{
				pFactoryPoolNetOff->AddInfoPool((*itFactory),pPlantPoolNetOff);
			}

			strLog.Format("%s指定分厂的%s,设备信息初始化完成，实时设备总数：%d",sCompany.c_str(),(*itFactory).c_str(),listPlantReal.size());
			BHLOG_FILE(STR(strLog));
		}
		g_pCompanyPoolSud->AddInfoPool(sCompany,pFactoryPoolSud);
		g_pCompanyPoolAlarm->AddInfoPool(sCompany,pFactoryPoolAlarm);
		g_pCompanyPoolReal->AddInfoPool(sCompany,pFactoryPoolReal);
		
		if (!g_bSkipNetoffFunc)
		{
			g_pCompanyPoolNetOff->AddInfoPool(sCompany,pFactoryPoolNetOff);
		}

		strLog.Format("%s 公司,设备信息初始化完成",sCompany.c_str());
		BHLOG_DEBUG(STR(strLog));
	}
	lpDb->Close();
	listFactory.clear();
	listPlantReal.clear();
	listPlantAlarm.clear();
	listPlantSud.clear();
	listPlantNetOff.clear();
	mapCompany.clear();
	listGroup.clear();

	verifyDAQHWStautsFile();

	verifyDAQSWStautsFile();

	CharInfoHelper::Initialize(loDb);

	//初始化ICEServer模组
	InitICEServer();

	RegistModuleComplete();
	
	CHZLogManage::Info("Finish","OnlineGlobal","InitGlobalVar");
}

CReaderWriterLock g_oComputeFeatureValueLock;

BOOL IsMWComputeFeatureValueEnabled()
{
	return FALSE;

	BOOL lbRet = FALSE;

	g_oComputeFeatureValueLock.AcquireReaderLock();

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lbRet = ::GetPrivateProfileInt("SYSTEM","COMPUTE_FEATURE_VALUE",FALSE,cAppLocation);


	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
	
	g_oComputeFeatureValueLock.ReleaseReaderLock();

	return lbRet;
}

int GetAlarmThreshold()
{
	int lnRet = 15;

	g_oComputeFeatureValueLock.AcquireReaderLock();

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lnRet = ::GetPrivateProfileInt("ALARM","ALARM_THRESHOLD",lnRet,cAppLocation);

		CString lstrData;
		lstrData.Format(_T("%d"),lnRet);
		::WritePrivateProfileString("ALARM","ALARM_THRESHOLD",lstrData.GetBuffer(0),cAppLocation);


	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	g_oComputeFeatureValueLock.ReleaseReaderLock();

	return lnRet;
}

int GetMinSampleNum()
{
	int lnRet = 256;

	g_oComputeFeatureValueLock.AcquireReaderLock();

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lnRet = ::GetPrivateProfileInt("SYSTEM","SAMPLE_NUM_MIN",lnRet,cAppLocation);

		CString lstrData;
		lstrData.Format(_T("%d"),lnRet);
		::WritePrivateProfileString("SYSTEM","SAMPLE_NUM_MIN",lstrData.GetBuffer(0),cAppLocation);


	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	g_oComputeFeatureValueLock.ReleaseReaderLock();

	return lnRet;
}


int InitMWRevThreshold()
{
	int lnRet = 15;

	g_oComputeFeatureValueLock.AcquireReaderLock();

	try
	{
		char cAppLocation[5000]={0};

		GetCurrentDirectory(5000,cAppLocation);

		strcat(cAppLocation,"\\DseSvr.ini");

		lnRet = ::GetPrivateProfileInt("SYSTEM","REV_THRESHOLD",G_REV_THRESHOLD,cAppLocation);

		CString lstrData;
		lstrData.Format(_T("%d"),lnRet);
		::WritePrivateProfileString("SYSTEM","REV_THRESHOLD",lstrData.GetBuffer(0),cAppLocation);

		g_rev_threshold = lnRet;


		lnRet = ::GetPrivateProfileInt("ALARM","G_REV_THRESHOLD_ROD_SINK",G_REV_THRESHOLD_ROD_SINK,cAppLocation);

		lstrData.Format(_T("%d"),lnRet);
		::WritePrivateProfileString("ALARM","G_REV_THRESHOLD_ROD_SINK",lstrData.GetBuffer(0),cAppLocation);

		g_rev_threshold_sink = lnRet;

	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

	g_oComputeFeatureValueLock.ReleaseReaderLock();

	return lnRet;
}

BOOL IsPlantRevThanGiven(CPlantInfoOnLine & arePlant,int lnRev)
{
	float lfMaxRev = 0;
	lfMaxRev = arePlant.MaxRealRev();
	
	BOOL lbIsPlantRevThanGiven = FALSE;

	if (lfMaxRev >= lnRev)
	{
		lbIsPlantRevThanGiven = TRUE;
	}

	return lbIsPlantRevThanGiven;
}

BOOL IsPlantRunning(CPlantInfoOnLine & arePlant,bool abRun)
{
	bool lbPlantStarting = false;
	float lfMaxRev = 0;
	int lnRevRevThreshold = g_rev_threshold;

	arePlant;

	lfMaxRev = arePlant.MaxRealRevLast();

	
	if (lfMaxRev > lnRevRevThreshold)
	{
		lbPlantStarting = TRUE;
	}
	else
	{
		if(abRun)
		{
			lbPlantStarting = TRUE;
		}
	}

	return lbPlantStarting;
}


BOOL InitMachineInfoType(IDBInterFace & arefDb)
{
	BOOL lbRet = FALSE;
	
	try
	{
		if (g_pMachIneInfoTypeCollection == NULL)
		{
			g_pMachIneInfoTypeCollection = new MachIneInfoTypeCollection(); 

		}
	
		CString lstrSqlFormat = _T("select [%s],[%s],[%s],ISNULL(t1.[%s],-3) as \
								   [%s],ISNULL(t1.[%s],-3) as [%s],[%s] from [%s] t1 order by [%s],[%s]");
		CString lstrSql ;

		lstrSql.Format(			lstrSqlFormat,
			gc_cInfoType,
			gc_cIndex,
			gc_cComment,
			gc_cFiled,
			gc_cFiled,
			gc_cFileld1,
			gc_cFileld1,
			gc_cUnit,
			gc_cMachInfoTypeTable,
			gc_cInfoType,
			gc_cIndex);

		arefDb.Cmd(lstrSql);

		while (arefDb.More())
		{
			int lnInfoType = arefDb.GetLong(gc_cInfoType);
			
			MachIneInfoTypeVo * lpInfoVo = &((*g_pMachIneInfoTypeCollection)[lnInfoType]);

			BHKD::S_Machineinfotype loMachineInfoType;

			if(loMachineInfoType.LoaddData(arefDb))
			{
				lpInfoVo->push_back(loMachineInfoType);;					

			}else
			{
				ASSERT(FALSE);
			}


			
			
		}
	
		lbRet = TRUE;
	}
	catch (...)
	{
	    //log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		lbRet = FALSE;
	}

	return lbRet;
	
}

void QuitClearGlobalVar()
{
	// 停止服务，销毁调试信息界面
	CMonitorICEServer::GetInstance()->DestroyMonitorWindow();

	if (g_pCompanyPoolSud!=NULL)
    	delete g_pCompanyPoolSud;
	if (g_pCompanyPoolAlarm!=NULL)
		delete g_pCompanyPoolAlarm;
	if (g_pCompanyPoolReal!=NULL)
		delete g_pCompanyPoolReal;
	if (g_pCompanyPoolNetOff!=NULL)
		delete g_pCompanyPoolNetOff;

	if (gpWatchDogFileLock!= NULL)
	{
		try
		{
			//not necessary
			delete gpWatchDogFileLock;

			gpWatchDogFileLock = NULL;

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

	if (g_pDBConnectLock!= NULL)
	{
		try
		{
			//not necessary			

			g_pDBConnectLock->ReleaseAllLocks();

			delete g_pDBConnectLock;

			g_pDBConnectLock = NULL;

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

//从ini文件中得到数据库服务器名，数据库名，用户名和密码
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

CPlantInfoOnLine * GetSpecCompanyFactoryPlant(string sCompany_,
											  string sFactory_,
											  string sPlant_,
											  PLANTINFOTYPE plantInfoType_/* =PLANTINFO_REAL */)
{
	CFactoryInfoPool *pFactroyPool=NULL;
	CPlantInfoPool *pPlantPool=NULL;
	switch(plantInfoType_)
	{
	case PLANTINFO_NETOFF:
		pFactroyPool=g_pCompanyPoolNetOff->GetSpecInfoPool(sCompany_);
		break;
	case PLANTINFO_SUD:
		pFactroyPool=g_pCompanyPoolSud->GetSpecInfoPool(sCompany_);
		break;
	case PLANTINFO_ALARM:
		pFactroyPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
		break;
	case PLANTINFO_REAL:
	default:
		pFactroyPool=g_pCompanyPoolReal->GetSpecInfoPool(sCompany_);
	}
	if (pFactroyPool==NULL)
		return NULL;
	pPlantPool=pFactroyPool->GetSpecInfoPool(sFactory_);
	if (pPlantPool!=NULL)
		return pPlantPool->GetSpecInfoPool(sPlant_);
	return NULL;
}

int GetAllPlantInfo(string sCompany_,
					string sFactory_,
					string sPlant_,
					vector<CPlantInfoOnLine *>& arefPlants,
					PLANTINFOTYPE plantInfoType_/*=PLANTINFO_REAL*/)
{
	CPlantInfoOnLine * lpPlantInfo = GetSpecCompanyFactoryPlant(sCompany_,sFactory_,sPlant_,plantInfoType_);
	if (NULL!= lpPlantInfo)
	{
		arefPlants.push_back(lpPlantInfo);
		return arefPlants.size();
	}

	vector<CFactoryInfoPool *> loFactories;
	CFactoryInfoPool *pFactroyPool=NULL;
	CCompanyInfoPool * pCompanyPool = NULL;
	switch(plantInfoType_)
	{
	case PLANTINFO_NETOFF:
		pFactroyPool=g_pCompanyPoolNetOff->GetSpecInfoPool(sCompany_);
		pCompanyPool = g_pCompanyPoolNetOff;
		break;
	case PLANTINFO_SUD:
		pFactroyPool=g_pCompanyPoolSud->GetSpecInfoPool(sCompany_);
		pCompanyPool = g_pCompanyPoolSud;
		break;
	case PLANTINFO_ALARM:
		pFactroyPool=g_pCompanyPoolAlarm->GetSpecInfoPool(sCompany_);
		pCompanyPool = g_pCompanyPoolAlarm;
		break;
	case PLANTINFO_REAL:
	default:
		pFactroyPool=g_pCompanyPoolReal->GetSpecInfoPool(sCompany_);
		pCompanyPool = g_pCompanyPoolReal;
	}

	if (pFactroyPool!=NULL)
	{
		loFactories.push_back(pFactroyPool);
	}
	else if (pCompanyPool == NULL)
	{
		return 0;
	}
	else
	{
		int lnCompanyNum = pCompanyPool->GetInfoNum();
		for (int i=0;i<lnCompanyNum;i++)
		{		
			pFactroyPool = pCompanyPool->GetSpecInfoPoolEx(i);
			if (NULL != pFactroyPool)
			{
				loFactories.push_back(pFactroyPool);
			}
		}
	}

	for (int i=0;i<loFactories.size();i++)
	{
		CFactoryInfoPool * lpFactoryPool = loFactories[i];

		if (NULL== lpFactoryPool)
		{
			continue;
		}

		CPlantInfoPool * lpPlantInfoPool = lpFactoryPool->GetSpecInfoPool(sFactory_);

		if (NULL == lpPlantInfoPool)
		{
			int lnFactoryNum = lpFactoryPool->GetInfoNum();

			for (int i=0;i<lnFactoryNum;i++)
			{
				lpPlantInfoPool =	lpFactoryPool->GetSpecInfoPoolEx(i);

				if (NULL == lpPlantInfoPool)
				{
					continue;
				}

				CPlantInfoOnLine * lpPlantInfo = lpPlantInfoPool->GetSpecInfoPool(sPlant_);

				if (lpPlantInfo!= NULL)
				{
					arefPlants.push_back(lpPlantInfo);
				}else
				{
					int lnPlantNum = lpPlantInfoPool->GetInfoNum();

					for (int i=0;i<lnPlantNum;i++)
					{
						CPlantInfoOnLine * lpPlantInfo =	lpPlantInfoPool->GetSpecInfoPoolEx(i);
						if (NULL != lpPlantInfo)
						{
							arefPlants.push_back(lpPlantInfo);
						}
					}
				}
			}
		}else
		{
			CPlantInfoOnLine * lpPlantInfo = lpPlantInfoPool->GetSpecInfoPool(sPlant_);

			if (lpPlantInfo!= NULL)
			{
				arefPlants.push_back(lpPlantInfo);
			}else
			{
				int lnPlantNum = lpPlantInfoPool->GetInfoNum();

				for (int i=0;i<lnPlantNum;i++)
				{
					CPlantInfoOnLine * lpPlantInfo =	lpPlantInfoPool->GetSpecInfoPoolEx(i);
					if (NULL != lpPlantInfo)
					{
						arefPlants.push_back(lpPlantInfo);
					}
				}
			}
		}
	}

	return arefPlants.size();
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

int CheckDBConnEx(IDBInterFace * pDB_)
{
	return TRUE;

	BOOL lbWriterLockGot = FALSE;

	int lnRet = -1;

	try
	{
		g_pDBConnectLock->AcquireWriterLock();		

		lbWriterLockGot = TRUE;

		pDB_->Cmd("select 1 from sysfiles where 1=0");

		if (!(pDB_->Exec()))
		{//进行重新链接

			pDB_->Close();

			CHZLogManage::Error("数据库连接重新连接","Global","CheckDBConn");

			CString lstrDbServer = g_strDbServer;

			CString lstrDBUser = g_strDbUser;

			CString lstrDbPwd = g_strDbPwd;

			CString lstrDbName = g_strDbName;

			if(pDB_->Login(lstrDbServer,lstrDBUser,lstrDbPwd,lstrDbName))
			{		
				pDB_->SetSqlLogPath(".\\log");

				lnRet = 1;
			}
			else
			{				

				CHZLogManage::Fatal("数据库重新连接失败!","Global","CheckDBConn");

				lnRet  = -1;
			}

		}
		else//数据库链接正常。
		{
			lnRet  = 0;
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

	if (lbWriterLockGot)
	{
		try
		{
			g_pDBConnectLock->ReleaseWriterLock();
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

	return lnRet;
}

int CheckDBConn(CCustomDB * pDB_)
{
	BOOL lbWriterLockGot = FALSE;

	int lnRet = -1;

	try
	{
		g_pDBConnectLock->AcquireWriterLock();		

		lbWriterLockGot = TRUE;

		pDB_->Cmd("select 1 from sysfiles where 1=0");
		
		if (!(pDB_->Exec()))
		{//进行重新链接
				
				pDB_->Close();

				CHZLogManage::Error("数据库连接重新连接","Global","CheckDBConn");

				CString lstrDbServer = g_strDbServer;

				CString lstrDBUser = g_strDbUser;

				CString lstrDbPwd = g_strDbPwd;

				CString lstrDbName = g_strDbName;

				if(pDB_->Login(lstrDbServer,lstrDBUser,lstrDbPwd,lstrDbName))
				{		
					pDB_->SetSqlLogPath(".\\log");

					lnRet = 1;
				}
				else
				{				

					CHZLogManage::Fatal("数据库重新连接失败!","Global","CheckDBConn");

					lnRet  = -1;
				}
	
		}
		else//数据库链接正常。
		{
			lnRet  = 0;
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

	if (lbWriterLockGot)
	{
		try
		{
			g_pDBConnectLock->ReleaseWriterLock();
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

	return lnRet;
}

///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_)
{
	char cAppLocation[500]={0};
	char sLogPath[1024]={0};
	GetCurrentDirectory(500,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	GetPrivateProfileString("SYSTEM","LOGPATH",".\\log",sLogPath,1023,cAppLocation);
	strLogPath_="ddddddddddddddddddddddddddddddddd";
	strLogPath_.Format("%s",sLogPath);
	//得到用户名和密码
	return GetPrivateProfileInt("LOG","KDLOG",0x0040,cAppLocation);
}

int writefile(float * ag_fwave, int ag_num, CString as_title)
{
	ofstream fileWave;
	CString lsModuleFileName;

	if (ag_num < 1)
	{
		ASSERT(FALSE);
		return 0;
	}

	ASSERT(ag_fwave != NULL);
	if(NULL == ag_fwave)
	{
		ASSERT(FALSE);
	}
	
	::GetModuleFileName(
		NULL, 
		lsModuleFileName.GetBufferSetLength(MAX_PATH * 2), 
		MAX_PATH);

	CString lsModuleFileDirName = lsModuleFileName.Left(lsModuleFileName.ReverseFind('\\')); 
	if (lsModuleFileName.IsEmpty())
	{
		ASSERT(FALSE);
		return 0;
	}

	CString lsDataFilename;
	lsDataFilename.AppendFormat("%s\\data\\%s.txt", lsModuleFileDirName.GetString(), as_title);
	
	if (as_title.Find(_T(":"))>=0)
	{
		fileWave.open(as_title.GetString(), ios::out);
	}
	else
	{
		fileWave.open(lsModuleFileDirName.GetString(), ios::out);
	}
	if (!fileWave.is_open())
	{
		ASSERT(FALSE);
		return 0;
	}

	fileWave<<ag_fwave[0];

	for(int i=1;i<ag_num;i++)
		fileWave<<","<<ag_fwave[i];
	fileWave<<endl;

	fileWave<<ag_num;

	fileWave.close();

	return ag_num;
}

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

int writefile( int * ag_fwave, int ag_num,CString & astrPath)
{
	ofstream fileWave;
	char filenm[10240]={0};
	CString strtmp = astrPath;
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,CString & astrPath)
{
	ofstream fileWave;
	char filenm[10240]={0};
	CString strtmp = astrPath;
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}
IReadWriteLock  * gpWatchDogFileLock = NULL;

BOOL UpdateWatchDogFileTime(const CString & astrRefFileName /* = _T */)
{

	gpWatchDogFileLock->BeginWrite();

	try
	{

		CString lstrFileName;

		int lnBufferSize = 2048;

		HANDLE loProcess = ::GetCurrentProcess();
		
		::GetModuleFileName(NULL,lstrFileName.GetBufferSetLength(lnBufferSize),lnBufferSize);

		lstrFileName.ReleaseBuffer(lnBufferSize);

		lstrFileName = lstrFileName.TrimRight();

		int lnStrLength = lstrFileName.StringLength(lstrFileName) -4;

		lstrFileName = lstrFileName.Mid(0,lnStrLength);

		lstrFileName += _T(".txt");

		if (astrRefFileName.CompareNoCase(_T("")) != 0)
		{
			lstrFileName = astrRefFileName ;
		}

		HANDLE lhFile =  CreateFile(lstrFileName.GetBuffer(0),
									GENERIC_READ|GENERIC_WRITE,
									FILE_SHARE_READ|FILE_SHARE_DELETE,
									NULL,OPEN_EXISTING,
									FILE_FLAG_BACKUP_SEMANTICS,
									NULL);

		BOOL lbFileOpened = FALSE;

		if (lhFile == INVALID_HANDLE_VALUE)
		{
			HANDLE lhd = ::CreateFile(lstrFileName.GetBuffer(0),
									GENERIC_READ|GENERIC_WRITE, 
									FILE_SHARE_READ|FILE_SHARE_WRITE,
									NULL, OPEN_ALWAYS, 
									FILE_ATTRIBUTE_NORMAL|FILE_ATTRIBUTE_HIDDEN, 
									NULL);
			
			if (lhd == INVALID_HANDLE_VALUE)
			{
				ASSERT(FALSE);
			}else
			{
				lhFile = lhd;

				lbFileOpened = TRUE;
				
			}
		}else
		{
			lbFileOpened = TRUE;
		}

		if (lbFileOpened)
		{
			FILETIME loLastWriteTime; // 对文件夹的最近修改时间 

			FILETIME loCreationTime; // 文件夹的创建时间 
			
			FILETIME loLastAccessTime; // 对文件夹的最近访问时间 

			BOOL lbRet = GetFileTime(lhFile,&loCreationTime,&loLastAccessTime,&loLastWriteTime);

			if (!lbRet)
			{
				ASSERT(FALSE);
			}

			SYSTEMTIME loSt;

			::GetSystemTime(&loSt);

			::SystemTimeToFileTime(&loSt,&loLastWriteTime);

			SetFileTime(lhFile,&loCreationTime,&loLastAccessTime,&loLastWriteTime);

			CloseHandle(lhFile);
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
	
	gpWatchDogFileLock->EndWrite();
	

	return FALSE;
}

BOOL InitSaveDataPolicy()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");


	//0.g_SaveFastDataToSecondTable

	lnData = ::GetPrivateProfileInt("SAVE_DATA",
									"G_SAVEFASTDATATOSECONDTABLE",
									G_SAVEFASTDATATOSECONDTABLE,
									cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEFASTDATATOSECONDTABLE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEFASTDATATOSECONDTABLE",NULL,cAppLocation);


	//1.g_SaveFastDataToHourTable
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEFASTDATATOHOURTABLE"
		,G_SAVEFASTDATATOHOURTABLE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEFASTDATATOHOURTABLE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEFASTDATATOHOURTABLE",NULL,cAppLocation);


	//2.g_SaveFastDataToDayTable
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEFASTDATATODAYTABLE"
		,G_SAVEFASTDATATODAYTABLE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEFASTDATATODAYTABLE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEFASTDATATODAYTABLE",NULL,cAppLocation);

	//3.g_SaveFastDataToMonthTable
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEFASTDATATOMONTHTABLE"
		,G_SAVEFASTDATATOMONTHTABLE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEFASTDATATOMONTHTABLE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEFASTDATATOMONTHTABLE",NULL,cAppLocation);


	//4.g_SaveDataOnStop
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEDATAONSTOP"
		,G_SAVEDATAONSTOP,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEDATAONSTOP;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEDATAONSTOP",lstrData,cAppLocation);

	g_SaveDataOnStop = lnData;
	BHLOG_ERROR(STR(_T("非临时在线模式下停车状态下是否保存日常数据表中的秒表，小时表，天表数据 :%d"),g_SaveDataOnStop));

	//5.g_SaveMonthDataOnStop
		lnData = ::GetPrivateProfileInt("SAVE_DATA",
										"G_SAVEMONTHDATAONSTOP"
										,G_SAVEMONTHDATAONSTOP,
										cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEMONTHDATAONSTOP;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEMONTHDATAONSTOP",lstrData,cAppLocation);

	g_SaveMonthDataOnStop = lnData;
	BHLOG_ERROR(STR(_T("非临时在线模式下停车状态下是否保存日常数据表中的月表数据 :%d"),g_SaveMonthDataOnStop));

	//6.g_SaveAllFastData
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEALLFASTDATA"
		,G_SAVEALLFASTDATA,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEALLFASTDATA;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEALLFASTDATA",lstrData,cAppLocation);

	g_SaveAllFastData = lnData;
	BHLOG_ERROR(STR(_T("是否保存所有快变数据 :%d"),g_SaveAllFastData));

	//7.g_SaveAllFastDataWave
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

	//8.g_SaveFastDataWaveInterval
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEFASTDATAWAVEINTERVAL"
		,G_SAVEFASTDATAWAVEINTERVAL,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEFASTDATAWAVEINTERVAL;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEFASTDATAWAVEINTERVAL",lstrData,cAppLocation);

	g_SaveFastDataWaveInterval = lnData;
	BHLOG_ERROR(STR(_T("保存快变数据波形间隔 :%d"),g_SaveFastDataWaveInterval));

	lnData = CConfigHelper::GetMonthTableUpdateFlag();
	if (lnData<0)
	{
		lnData = G_UPDATEMONTHTABLE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_UPDATEMONTHTABLE",lstrData,cAppLocation);

	//8.g_SaveUseOptimizeMode
	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_SAVEUSEOPTIMIZEMODE"
		,G_SAVEUSEOPTIMIZEMODE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVEUSEOPTIMIZEMODE;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SAVE_DATA","G_SAVEUSEOPTIMIZEMODE",lstrData,cAppLocation);

	g_SaveUseOptimizeMode = lnData;

	return TRUE;
}

/************************************************************************/
/*  
int g_nNewAlarmMaxTime = 2;					     //距离新报警时间开始更新上次最大报警值的间隔,	默认2天
int g_nSaveIndenseTime =5;						//密集保存时间间隔,								默认5分钟
int g_nSaveHalfIndenseTime =30 ;				//半密集保存时间间隔,							默认30秒
int g_nHalfNormalNotSaveTime = 20;				//半正常状态下持续保存的时间长度，				默认20分钟
int g_nSaveSparseTime =30;						//旧报警每30分钟保存一组的时间间隔,				默认30分钟
int g_nHalfNormalTime =2 ;						//半正常状态维持时间间隔						默认2个小时
int g_nChannHalfNormalTime =1;					//测点半正常状态维持时间间隔					默认1个小时                                                                     */
/************************************************************************/

BOOL InitAlarmParameters()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");
	//0.g_UseNewAlarmLogic

	lnData = ::GetPrivateProfileInt("ALARM",
		"G_USENEWALARMLOGIC"
		,-1,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_USENEWALARMLOGIC;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_USENEWALARMLOGIC",lstrData,cAppLocation);

	g_UseNewAlarmLogic = lnData;
	BHLOG_ERROR(STR(_T("是否使用新报警逻辑 :%d"),g_UseNewAlarmLogic));

	//1.g_nNewAlarmMaxTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NNEWALARMMAXTIME"
		,G_NNEWALARMMAXTIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NNEWALARMMAXTIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NNEWALARMMAXTIME",lstrData,cAppLocation);
	
	g_nNewAlarmMaxTime = lnData;
	BHLOG_ERROR(STR(_T("距离新报警时间开始更新上次最大报警值的间隔 :%d天"),g_nNewAlarmMaxTime));
	

	//2.g_nSaveIndenseTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NSAVEINDENSETIME"
		,G_NSAVEINDENSETIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NSAVEINDENSETIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NSAVEINDENSETIME",lstrData,cAppLocation);

	g_nSaveIndenseTime = lnData;

	BHLOG_ERROR(STR(_T("半密集保存时间间隔 :%d秒"),g_nSaveIndenseTime));



	//4.g_nHalfNormalNotSaveTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NHALFNORMALNOTSAVETIME"
		,G_NHALFNORMALNOTSAVETIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NHALFNORMALNOTSAVETIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NHALFNORMALNOTSAVETIME",lstrData,cAppLocation);

	g_nHalfNormalNotSaveTime = lnData;

	BHLOG_ERROR(STR(_T("半正常状态下持续保存的时间长度 :%d分钟"),g_nHalfNormalNotSaveTime));


	//5.g_nSaveSparseTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NSAVESPARSETIME"
		,G_NSAVESPARSETIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NSAVESPARSETIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NSAVESPARSETIME",lstrData,cAppLocation);

	g_nSaveSparseTime = lnData;

	BHLOG_ERROR(STR(_T("旧报警保存时间间隔 :%d分钟"),g_nSaveSparseTime));



	//6.g_nHalfNormalTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NHALFNORMALTIME"
		,G_NHALFNORMALTIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NHALFNORMALTIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NHALFNORMALTIME",lstrData,cAppLocation);

	g_nHalfNormalTime = lnData;

	BHLOG_ERROR(STR(_T("半正常状态维持时间间隔 :%小时"),g_nHalfNormalTime));


	//7.g_nChannHalfNormalTime
	 lnData = ::GetPrivateProfileInt("ALARM",
		"G_NCHANNHALFNORMALTIME"
		,G_NCHANNHALFNORMALTIME,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_NCHANNHALFNORMALTIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_NCHANNHALFNORMALTIME",lstrData,cAppLocation);

	g_nChannHalfNormalTime = lnData;

	

	BHLOG_ERROR(STR(_T("测点半正常状态维持时间间隔 :%d小时"),g_nChannHalfNormalTime));


	lnData = ::GetPrivateProfileInt("ALARM",
		"G_USENEW_DC_ALARMLOGIC"
		,-1,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_USENEW_DC_ALARMLOGIC;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_USENEW_DC_ALARMLOGIC",lstrData,cAppLocation);

	g_UseNewDCAlarmLogic = lnData;
	BHLOG_ERROR(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_UseNewDCAlarmLogic));


	lnData = ::GetPrivateProfileInt("ALARM",
		"G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC"
		,G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC",lstrData,cAppLocation);

	g_SaveDataIntervalForNewDCAlarmLogic = lnData;
	BHLOG_ERROR(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_SaveDataIntervalForNewDCAlarmLogic));

	lnData = ::GetPrivateProfileInt("ALARM",
		"G_USENEW_AXIS_ALARMLOGIC"
		,-1,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_USENEW_AXIS_ALARMLOGIC;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_USENEW_AXIS_ALARMLOGIC",lstrData,cAppLocation);

	g_UseNewAxisAlarmLogic = lnData;					 
	BHLOG_ERROR(STR(_T("是否使用新的轴位移报警逻辑 :%d"),g_UseNewAxisAlarmLogic));

	lnData = ::GetPrivateProfileInt("ALARM",
		"G_WAVE_DIFF_RATIO"
		,G_WAVE_DIFF_RATIO,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_WAVE_DIFF_RATIO;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_WAVE_DIFF_RATIO",lstrData,cAppLocation);

	g_wave_diff_ratio = lnData;
	BHLOG_ERROR(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_wave_diff_ratio));


	lnData = ::GetPrivateProfileInt("ALARM",
		"G_ALARM_LARGER_TIMES"
		,G_ALARM_LARGER_TIMES,
		cAppLocation);
	if (lnData<=0)
	{
		lnData = G_ALARM_LARGER_TIMES;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_ALARM_LARGER_TIMES",lstrData,cAppLocation);

	g_alarm_larger_times = lnData;
	BHLOG_ERROR(STR(_T("连续超过上次报警的次数 :%d"),g_alarm_larger_times));


	lnData = ::GetPrivateProfileInt("ALARM",
		"G_SAVE_FAST_AND_REGULAR_ALARM_DATA"
		,-1,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_SAVE_FAST_AND_REGULAR_ALARM_DATA;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("ALARM","G_SAVE_FAST_AND_REGULAR_ALARM_DATA",lstrData,cAppLocation);

	g_SaveFastAndRegularAlarmData = lnData;
	BHLOG_ERROR(STR(_T("是否开启同时保存快变和常规报警数据 :%d"),g_SaveFastAndRegularAlarmData));

	lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_UPDATE_PROCDATA_OVERTIME"
		,G_UPDATE_PROCDATA_OVERTIME,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_UPDATE_PROCDATA_OVERTIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SYSTEM","G_UPDATE_PROCDATA_OVERTIME",lstrData,cAppLocation);

	g_UpdateProcDataOverTime = lnData;

	lnData = ::GetPrivateProfileInt("SYSTEM",
		"G_UPDATE_PLANTDATA_OVERTIME"
		,G_UPDATE_PLANTDATA_OVERTIME,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_UPDATE_PLANTDATA_OVERTIME;
	}

	lstrData.Format("%d",lnData);
	WritePrivateProfileString("SYSTEM","G_UPDATE_PLANTDATA_OVERTIME",lstrData,cAppLocation);

	g_UpdatePlantDataOverTime = lnData;

	return TRUE;
}



CBHDateTime g_local_judge_alarm_time;
BOOL UpdateJudgeAlarmTime()
{
	char cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	CBHDateTime loDatetime = CBHDateTime::GetCurrentTime();
	CString lstrData = loDatetime.Format();
	BOOL lbRet = WritePrivateProfileString(_T("SYSTEM"),_T("JUDGE_ALARM_TIME"),lstrData.GetBuffer(0),cAppLocation);

	g_local_judge_alarm_time = loDatetime;
	return TRUE;
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

COleDateTimeSpan GetJudgeAlarmDatetime()
{
	COleDateTimeSpan loSpn = CBHDateTime::GetCurrentTime() -  g_local_judge_alarm_time;
	return loSpn;
}
int G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR = -1;

int GetAlarmOverridTime()
{
	if (G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR>0)
	{
		return G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR;
	}

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

	G_JUDGEALARM_OVERRIDE_SYNC_TIME_IN_HOUR = lnData;

	return lnData;
}

BOOL MakeSureFolderExist( const CString &asAbsoluteFilePath )
{
	//		AfxMessageBox(_T("1"));
	if (INVALID_FILE_ATTRIBUTES != GetFileAttributes(asAbsoluteFilePath))
	{

		return TRUE;
	}

	//AfxMessageBox(_T("2"));

	CString lstrFilePath = asAbsoluteFilePath;

	lstrFilePath.TrimLeft(_T(" "));

	lstrFilePath.TrimLeft(_T("\r"));

	lstrFilePath.TrimLeft(_T("\n"));

	if (lstrFilePath.Find(_T(":\\"))!= 1 && lstrFilePath.Find(_T(":/"))!= 1)
	{
		return FALSE;
	}

	//AfxMessageBox(_T("3"));

	if (lstrFilePath[lstrFilePath.GetLength() - 1] != _T('\\'))
	{
		//lstrFilePath = lstrFilePath.Left(lstrFilePath.ReverseFind('\\'));
		lstrFilePath = lstrFilePath + _T("\\");
	}

	CList<CString, CString&> llstPath;

	while (TRUE)
	{
		if (lstrFilePath.GetLength() < 3)
		{
			TRACE(_T("Error: lstrFilePath.GetLength() < 3\n"));

			//AfxMessageBox(_T("Error"));
			return FALSE;
		}
		else
		{
			if (CreateDirectory(lstrFilePath, NULL))
			{
				break;
			}

			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return TRUE;
			}
			else if (GetLastError() != ERROR_PATH_NOT_FOUND)
			{
				TRACE(_T("Unknown Error: %d\n"), GetLastError());

				//AfxMessageBox(_T("Unknown Error"));
				return  FALSE;
			}
		}
		llstPath.AddTail(lstrFilePath);
		lstrFilePath = lstrFilePath.Left(lstrFilePath.ReverseFind('\\'));
	}

	// 循环创建剩下的目录
	CString lstrPath;
	while (!llstPath.IsEmpty())
	{
		lstrPath = llstPath.RemoveTail();
		if (!CreateDirectory(lstrPath, NULL))
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				return TRUE;
			}
			TRACE(_T("Unknown Error: %d\n"), GetLastError());
			//AfxMessageBox(_T("CreateDirectory Unknown Error"));
			return FALSE;
		}
	}

	return TRUE;
}

int readfile(  DATATYPE_WAVE * ag_fwave,int anFileSize,CString strFile_ )
{
	BEGIN_ERROR_HANDLE;
	
	ifstream infile(strFile_);
	if (!infile)
	{
		return 0;
	}
	int i = 0;
	for (i =0;i<anFileSize;i++)
	{
		DATATYPE_WAVE lnData = 0;
		if (infile >> lnData)
		{
			ag_fwave[i]  = lnData;
		}else
		{
			ASSERT(FALSE);
			break;
		}		
	}
	return i;
	END_ERROR_HANDLE;
	return 0;
}

int readfile( int * ag_fwave,int anFileSize,CString strFile_ )
{
	BEGIN_ERROR_HANDLE;
	ifstream infile(strFile_);
	if (!infile)
	{
		return 0;
	}
	int i = 0;
	for (i =0;i<anFileSize;i++)
	{
		int lnData = 0;
		if (infile >> lnData)
		{
			ag_fwave[i]  = lnData;
		}else
		{
			ASSERT(FALSE);
			break;
		}		
	}
	return i;
	END_ERROR_HANDLE;
	return 0;
}


int readfile( float * ag_fwave,int anFileSize,CString strFile_ )
{
	BEGIN_ERROR_HANDLE;
	FILE * lhFile = freopen(strFile_,"r",stdin);

	if (NULL == lhFile)
	{
		return 0;
	}

	
	
	for (int i=0;i<anFileSize;i++)
	{
		scanf("%f",&ag_fwave[i]);
	}

	fclose(lhFile);
	return anFileSize;
	END_ERROR_HANDLE;
	return 0;
}

LONGLONG GetFileSize( CString & lstrFileName )
{
	LONGLONG nFileSize=0;

	CFileFind finder;

	if(finder.FindFile(lstrFileName))
	{

		finder.FindNextFile();

		nFileSize = finder.GetLength(); //文件大小在这里

		finder.Close();

	}else
	{
		CString lstrTempPath = GetAppPath()+"/"+lstrFileName;

		if(finder.FindFile(lstrTempPath))
		{
			finder.FindNextFile();

			nFileSize = finder.GetLength(); //文件大小在这里

			finder.Close();
		}
	}

	return nFileSize;
}

BOOL SaveDetainDataTime()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	BOOL  lbLockGot = FALSE;
	CString lstrStatusValue;
	CString lstrDateTimeValue;
	try
	{
		lbLockGot = TryEnterCriticalSection(g_MapLock);

		if (lbLockGot)
		{
			std::map< pair<CBHDateTime,CBHDateTime>,map<CString,int> >::iterator it;
			std::map<CString,int>::iterator itTbl;
			for (it = g_map_SpecSaveTime.begin();it!=g_map_SpecSaveTime.end();++it)
			{
				for(itTbl = it->second.begin();itTbl != it->second.end(); ++itTbl)
				{
					CString lstrStatus;
					lstrStatus.Format(_T("%d"),itTbl->second);

					lstrStatusValue   += it->first.first.Format()+_T("|")
						+ it->first.second.Format()+_T("|") 
						+ itTbl->first+"="
						+ lstrStatus+";";
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
			LeaveCriticalSection(g_MapLock);
		}
		catch (...)
		{
			ASSERT(FALSE);
		}

		lbLockGot = FALSE;
	}

	BEGIN_ERROR_HANDLE;
	
	if(!lstrStatusValue.IsEmpty())
	{
		::WritePrivateProfileString("SAVE_DATA","SAVE_SPEC_DATA_TIME",lstrStatusValue,cAppLocation);
	}
	else
	{
		::WritePrivateProfileString("SAVE_DATA","SAVE_SPEC_DATA_TIME",NULL,cAppLocation);
	}

	END_ERROR_HANDLE;

	return TRUE;
}

BOOL UpdateAlarmParam()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbLockGot = FALSE;

	try
	{
		lbLockGot = TryEnterCriticalSection(g_pUpdateParamLock);

		if (lbLockGot)
		{
			//0.g_UseNewAlarmLogic

			lnData = ::GetPrivateProfileInt("ALARM",
				"G_USENEWALARMLOGIC"
				,G_USENEWALARMLOGIC,
				cAppLocation);
			g_UseNewAlarmLogic = lnData;
			zdlTraceLine(STR(_T("是否使用新报警逻辑 :%d"),g_UseNewAlarmLogic));

			//1.g_nNewAlarmMaxTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NNEWALARMMAXTIME"
				,G_NNEWALARMMAXTIME,
				cAppLocation);
			g_nNewAlarmMaxTime = lnData;
			zdlTraceLine(STR(_T("距离新报警时间开始更新上次最大报警值的间隔 :%d天"),g_nNewAlarmMaxTime));


			//2.g_nSaveIndenseTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NSAVEINDENSETIME"
				,G_NSAVEINDENSETIME,
				cAppLocation);
			g_nSaveIndenseTime = lnData;
			zdlTraceLine(STR(_T("半密集保存时间间隔 :%d秒"),g_nSaveIndenseTime));



			//4.g_nHalfNormalNotSaveTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NHALFNORMALNOTSAVETIME"
				,G_NHALFNORMALNOTSAVETIME,
				cAppLocation);
			g_nHalfNormalNotSaveTime = lnData;
			zdlTraceLine(STR(_T("半正常状态下持续保存的时间长度 :%d分钟"),g_nHalfNormalNotSaveTime));


			//5.g_nSaveSparseTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NSAVESPARSETIME"
				,G_NSAVESPARSETIME,
				cAppLocation);
			g_nSaveSparseTime = lnData;
			zdlTraceLine(STR(_T("旧报警保存时间间隔 :%d分钟"),g_nSaveSparseTime));

			//6.g_nHalfNormalTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NHALFNORMALTIME"
				,G_NHALFNORMALTIME,
				cAppLocation);
			g_nHalfNormalTime = lnData;
			zdlTraceLine(STR(_T("半正常状态维持时间间隔 :%小时"),g_nHalfNormalTime));


			//7.g_nChannHalfNormalTime
			lnData = ::GetPrivateProfileInt("ALARM",
				"G_NCHANNHALFNORMALTIME"
				,G_NCHANNHALFNORMALTIME,
				cAppLocation);
			g_nChannHalfNormalTime = lnData;
			zdlTraceLine(STR(_T("测点半正常状态维持时间间隔 :%d小时"),g_nChannHalfNormalTime));

			lnData = ::GetPrivateProfileInt("ALARM",
				"G_USENEW_DC_ALARMLOGIC"
				,G_USENEW_DC_ALARMLOGIC,
				cAppLocation);
			g_UseNewDCAlarmLogic = lnData;
			zdlTraceLine(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_UseNewDCAlarmLogic));


			lnData = ::GetPrivateProfileInt("ALARM",
				"G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC"
				,G_SAVE_DATA_INTERVAL_FOR_NEW_DC_ALARM_LOGIC,
				cAppLocation);
			g_SaveDataIntervalForNewDCAlarmLogic = lnData;
			zdlTraceLine(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_SaveDataIntervalForNewDCAlarmLogic));

			lnData = ::GetPrivateProfileInt("ALARM",
				"G_USENEW_AXIS_ALARMLOGIC"
				,G_USENEW_AXIS_ALARMLOGIC,
				cAppLocation);
			g_UseNewAxisAlarmLogic = lnData;					 
			zdlTraceLine(STR(_T("是否使用新的轴位移报警逻辑 :%d"),g_UseNewAxisAlarmLogic));

			lnData = ::GetPrivateProfileInt("ALARM",
				"G_WAVE_DIFF_RATIO"
				,G_WAVE_DIFF_RATIO,
				cAppLocation);
			g_wave_diff_ratio = lnData;
			zdlTraceLine(STR(_T("是否使用新的直流量报警逻辑 :%d"),g_wave_diff_ratio));


			lnData = ::GetPrivateProfileInt("ALARM",
				"G_ALARM_LARGER_TIMES"
				,G_ALARM_LARGER_TIMES,
				cAppLocation);

			g_alarm_larger_times = lnData;
			zdlTraceLine(STR(_T("连续超过上次报警的次数 :%d"),g_alarm_larger_times));


			lnData = ::GetPrivateProfileInt("ALARM",
				"G_SAVE_FAST_AND_REGULAR_ALARM_DATA"
				,G_SAVE_FAST_AND_REGULAR_ALARM_DATA,
				cAppLocation);

			g_SaveFastAndRegularAlarmData = lnData;
			zdlTraceLine(STR(_T("是否开启同时保存快变和常规报警数据 :%d"),g_SaveFastAndRegularAlarmData));

			lnData = ::GetPrivateProfileInt("SYSTEM","REV_THRESHOLD",G_REV_THRESHOLD,cAppLocation);

			g_rev_threshold = lnData;

			lnData = ::GetPrivateProfileInt("ALARM","G_REV_THRESHOLD_ROD_SINK",G_REV_THRESHOLD_ROD_SINK,cAppLocation);

			g_rev_threshold_sink = lnData;
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
			LeaveCriticalSection(g_pUpdateParamLock);
		}
		catch (...)
		{
			ASSERT(FALSE);
		}

		lbLockGot = FALSE;
	}

	return TRUE;
}

BOOL UpdateTrendDataSaveParam()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbLockGot = FALSE;

	try
	{
		lbLockGot = TryEnterCriticalSection(g_pUpdateParamLock);

		if (lbLockGot)
		{
			//4.g_SaveDataOnStop
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEDATAONSTOP"
				,G_SAVEDATAONSTOP,
				cAppLocation);
			g_SaveDataOnStop = lnData;
			zdlTraceLine(STR(_T("非临时在线模式下停车状态下是否保存日常数据表中的秒表，小时表，天表数据 :%d"),g_SaveDataOnStop));

			//5.g_SaveMonthDataOnStop
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEMONTHDATAONSTOP"
				,G_SAVEMONTHDATAONSTOP,
				cAppLocation);
			g_SaveMonthDataOnStop = lnData;
			zdlTraceLine(STR(_T("非临时在线模式下停车状态下是否保存日常数据表中的月表数据 :%d"),g_SaveMonthDataOnStop));

			//8.g_SaveUseOptimizeMode
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEUSEOPTIMIZEMODE"
				,G_SAVEUSEOPTIMIZEMODE,
				cAppLocation);

			g_SaveUseOptimizeMode = lnData;

			BOOL lbSaveEmptyValue = ::GetPrivateProfileInt("SYSTEM","SAVE_EMPTY_VALUE",FALSE,cAppLocation);

			g_bIsShouldSaveEmptyValue = lbSaveEmptyValue;

			zdlTraceLine(STR(_T("是否保存全零数据 :%d"),g_SaveFastDataWaveInterval));
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
			LeaveCriticalSection(g_pUpdateParamLock);
		}
		catch (...)
		{
			ASSERT(FALSE);
		}

		lbLockGot = FALSE;
	}

	
	return TRUE;
}

BOOL UpdateFastDataSaveParam()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	BOOL lbLockGot = FALSE;

	try
	{
		lbLockGot = TryEnterCriticalSection(g_pUpdateParamLock);

		if (lbLockGot)
		{
			//6.g_SaveAllFastData
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEALLFASTDATA"
				,G_SAVEALLFASTDATA,
				cAppLocation);

			g_SaveAllFastData = lnData;
			zdlTraceLine(STR(_T("是否保存所有快变数据 :%d"),g_SaveAllFastData));

			//7.g_SaveAllFastDataWave
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEALLFASTDATAWAVE"
				,G_SAVEALLFASTDATAWAVE,
				cAppLocation);

			g_SaveAllFastDataWave = lnData;
			zdlTraceLine(STR(_T("是否保存所有快变数据波形 :%d"),g_SaveAllFastDataWave));

			//8.g_SaveFastDataWaveInterval
			lnData = ::GetPrivateProfileInt("SAVE_DATA",
				"G_SAVEFASTDATAWAVEINTERVAL"
				,G_SAVEFASTDATAWAVEINTERVAL,
				cAppLocation);

			g_SaveFastDataWaveInterval = lnData;
			zdlTraceLine(STR(_T("保存快变数据波形间隔 :%d"),g_SaveFastDataWaveInterval));
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
			LeaveCriticalSection(g_pUpdateParamLock);
		}
		catch (...)
		{
			ASSERT(FALSE);
		}

		lbLockGot = FALSE;
	}

	
	return TRUE;
}

BOOL InitFeedWatchDogInterface()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	int lnBufferSize = 10240000;

	static const int lpRealTimeInterface[] = {24000,24001,24003,24104,24105,24106,24115,24006};

	std::vector<TCHAR> loBuffer(lnBufferSize);
	std::vector<TCHAR> loBufferTime(lnBufferSize);
	TCHAR *lpBuffer = &loBuffer.front();
	TCHAR *lpBufferTime = &loBufferTime.front();

	CString lstrSplitInterface = _T(";");

	CString lstrBuffer;
	CStringArray loInterfaceArray;


	std::map< int,int >::iterator it;

	int lnData = ::GetPrivateProfileString("SYSTEM",
		"FEED_WATCHDOG_REALTIME_GROUP"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	
	lstrBuffer = lpBuffer;
	loInterfaceArray.RemoveAll();
	lnData = GetSplitStrArray(lstrBuffer,lstrSplitInterface,loInterfaceArray,TRUE);

	if(lnData <= 0)
	{
		int lnArraySize = sizeof(lpRealTimeInterface)/sizeof(int);

		CString lstrInterfaceArray;
		for(int i = 0; i < lnArraySize; i++)
		{
			if(g_map_RealTimeInterface.find(lpRealTimeInterface[i]) == g_map_RealTimeInterface.end())
			{
				g_map_RealTimeInterface.insert(make_pair(lpRealTimeInterface[i],lpRealTimeInterface[i]));
			}

			lstrInterfaceArray.AppendFormat("%d;",lpRealTimeInterface[i]);
		}

		WritePrivateProfileString("SYSTEM",
			"FEED_WATCHDOG_REALTIME_GROUP",
			lstrInterfaceArray.GetBuffer(),
			cAppLocation);

		lstrInterfaceArray.ReleaseBuffer();

	}
	else
	{
		for (int i=0;i<loInterfaceArray.GetSize();i++)
		{
			CString lstrInterface = loInterfaceArray.GetAt(i);

			int lnInterface = atoi(lstrInterface);

			if(g_map_RealTimeInterface.find(lnInterface) == g_map_RealTimeInterface.end())
			{
				g_map_RealTimeInterface.insert(make_pair(lnInterface,lnInterface));
			}

		}
	}
	

	 lnData = ::GetPrivateProfileString("SYSTEM",
		"FEED_WATCHDOG_OTHER_GROUP"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	
	lstrBuffer = lpBuffer;
	loInterfaceArray.RemoveAll();
	lnData = GetSplitStrArray(lstrBuffer,lstrSplitInterface,loInterfaceArray,TRUE);
	if(lnData <= 0)
	{

		WritePrivateProfileString("SYSTEM",
			"FEED_WATCHDOG_OTHER_GROUP",
			"",
			cAppLocation);

	}
	else
	{
		for (int i=0;i<loInterfaceArray.GetSize();i++)
		{
			int lnInterface = atoi(loInterfaceArray.GetAt(i));

			if(g_map_OtherInterface.find(lnInterface) == g_map_OtherInterface.end())
			{
				g_map_OtherInterface.insert(make_pair(lnInterface,lnInterface));
			}

		}
	}
	return TRUE;
}

int GetAllAlarmPlantInfo( vector<void*>& arefPlants_,CString strMachType_ )
{
	vector<CFactoryInfoPool *> loFactories;
	CFactoryInfoPool *pFactroyPool=NULL;
	CCompanyInfoPool * pCompanyPool = NULL;

	pCompanyPool = g_pCompanyPoolAlarm;

	int lnCompanyNum = pCompanyPool->GetInfoNum();
	for (int i=0;i<lnCompanyNum;i++)
	{		
		pFactroyPool = pCompanyPool->GetSpecInfoPoolEx(i);
		if (NULL != pFactroyPool)
		{
			loFactories.push_back(pFactroyPool);
		}
	}

	for (int i=0;i<loFactories.size();i++)
	{
		CFactoryInfoPool * lpFactoryPool = loFactories[i];

		if (NULL== lpFactoryPool)
		{
			continue;
		}

		CPlantInfoPool * lpPlantInfoPool = NULL;

		int lnFactoryNum = lpFactoryPool->GetInfoNum();

		for (int i=0;i<lnFactoryNum;i++)
		{
			lpPlantInfoPool =	lpFactoryPool->GetSpecInfoPoolEx(i);

			if (NULL == lpPlantInfoPool)
			{
				continue;
			}


			int lnPlantNum = lpPlantInfoPool->GetInfoNum();

			for (int i=0;i<lnPlantNum;i++)
			{
				CPlantInfoOnLine * lpPlantInfo =	lpPlantInfoPool->GetSpecInfoPoolEx(i);
				if (NULL != lpPlantInfo 
					&& strcmp(lpPlantInfo->GetPlantInfo()._cManufacturer,strMachType_.GetBuffer()) == 0
					&& strMachType_.Trim().GetLength() > 0)
				{
					arefPlants_.push_back(lpPlantInfo);
				}
			}

		}
	}

	return arefPlants_.size();
}