#include "stdafx.h"
#include "service.h"
#include "process.h"
#include "additions/BHOleDbWrapper.h"
#include <additions/BHCommWrapper.h>
#include "off_globe.h"
long g_nVerMain =0;

BOOL     (*g_pCallBackFunc)( struct Service_Module * pModule )=NULL;
Service_Module module= {
		100 ,
	"BHKD" ,
	"提取历史数据",
	"离线数据",
	"数据处理",
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
	CString sDate;
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
	 _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

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
		 sVal.Format("%li,历史数据,得到当前历史趋势,保留"	 ,SERVICE_SERVICE_OFF_GETCURTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定趋势点的波形信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECFREQWAVE);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定时间段历史趋势,保留"	 ,SERVICE_SERVICE_OFF_GETTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间段的各种类型趋势数据,保留"	 ,SERVICE_SERVICE_OFF_GETAVIBCHANNTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间段的轴心位置数据,保留"	 ,SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定两个振动测点、指定时间的特征值、报警限等信息,保留" ,SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间的细化谱,保留"	 ,SERVICE_SERVICE_OFF_GETZOOMSPEC);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间的轴心轨迹波形,保留",SERVICE_SERVICE_OFF_GETAXESTRACK);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定设备任意个测点、指定时间的特征值、报警限等信息,保留",SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间段的各种特征值,保留",SERVICE_SERVICE_OFF_GETVIBCHANNSTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、当前指定趋势类型的各种特征值趋势,保留",SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定两个振动测点波形和频谱,保留",SERVICE_SERVICE_OFF_GETTWOCHANNDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定两个振动测点波形和频谱,保留",SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO);(*pVal).Add(sVal);

		 sVal.Format("%li,历史数据,得到报警详细信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定设备、测点、时间段、报警日志,保留"	 ,SERVICE_OFF_GETVIBCHANNALARMLOG);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定设备、任意个振动测点、报警事件ID的报警数据,保留"	 ,SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定测点和时间的报警记录波形频谱信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定测点报警限信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定事件轴心位置报警趋势,保留"	 ,SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定报警事件数据时间范围,保留"	 ,SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定轴心位置报警事件、时间的数据,保留"	 ,SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点的详细报警信息趋势,保留"	 ,SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定轴位移测点的详细报警趋势,保留"	 ,SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定过程量的详细报警趋势,保留"	 ,SERVICE_SERVICE_OFF_GETPROCALARMTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定过程量测点、时间的报警数据,保留"	 ,SERVICE_SERVICE_OFF_GETPROCALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定指定事件ID的趋势报警的详细数据,保留"	 ,SERVICE_SERVICE_OFF_GETTRENDALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定指定事件ID、时间的轴位移报警的详细数据,保留"	 ,SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA);(*pVal).Add(sVal);

         sVal.Format("%li,历史数据,获得往复压缩机指定测点、时间的示功图的详细数据,保留", SERVICE_SERVICE_OFF_GETPVDATA ); (*pVal).Add(sVal);
         sVal.Format("%li,历史数据,获得往复压缩机指定测点、时间的活塞杆沉降的详细数据,保留", SERVICE_SERVICE_OFF_GETRODSINKDATA ); (*pVal).Add(sVal);
         sVal.Format("%li,历史数据,获得往复压缩机指定测点、时间的的多参数分析详细数据,保留", SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,获得指定时间、指定机组所有测点的历史概貌图数据", SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE ); (*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定启停车事件和时间的一组提纯轴心轨迹的波形数据,保留", SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK ); (*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,获得往复式系统报警多参数分析数据,保留"	 ,SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,获得往复式系统报警应力监测分析数据,保留"  ,SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定机组和时间范围的启停车,保留"	 ,SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定机组、测点和时间的启停车波特图,保留"	 ,SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定机组、测点和时间的启停车详细数据,保留"	 ,SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定机组、测点和保存数据时刻的启停车详细数据,保留"	 ,SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,指定机组、测点和保存数据时刻的启停车详细数据,保留"	 ,SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,报表信息,指定机组月报表信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT);(*pVal).Add(sVal);
		 sVal.Format("%li,报表信息,指定分厂的设备运行信息,保留"	 ,SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO);(*pVal).Add(sVal);

		 sVal.Format("%li,启停车数据,启停车事件的历史趋势,保留"	 ,SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,请求启停车事件ID、并插入这条启停车事件,保留"	 ,SERVICE_SERVICE_OFF_REQSUDEVENTID);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,通知启停车事件结束,保留"	 ,SERVICE_SERVICE_OFF_NOTICESUDSTOP);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,保存启停车数据,保留"	 ,SERVICE_SERVICE_OFF_RECSUDDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,诊断,关联诊断,保留"	 ,SERVICE_SERVICE_OFF_CORRDIGN);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,获取报警记录的特征值,保留"	 ,SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE);(*pVal).Add(sVal);

		 sVal.Format("%li,报警数据,保存阈值的详细信息数据,保留"	 ,SERVICE_SERVICE_OFF_SAVETHINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,通知指定通道的快变报警事件结束,保留"	 ,SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,保存快变报警数据,保留"	 ,SERVICE_SERVICE_OFF_RECFASTALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,保存报警完成后新学习的阈值ID,保留"	 ,SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,添加一条完整报警日志,保留"	 ,SERVICE_SERVICE_OFF_ADDALARMLOG);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,添加一条刚开始报警的报警日志,保留"	 ,SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,添加完整的启停车日志信息,保留"	 ,SERVICE_SERVICE_OFF_ADDWHOLESUDLOG);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,保存动态报警数据,保留"	 ,SERVICE_SERVICE_OFF_RECDYNALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,保存过程量报警数据,保留"	 ,SERVICE_SERVICE_OFF_RECPROCALARMDATA);(*pVal).Add(sVal);
		 
		 sVal.Format("%li,报警数据,根据报警日志列表得到监测站级历史报警统计,保留",SERVICE_SERVICE_OFF_GETALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,监测站级历史报警ID列表,保留",SERVICE_SERVICE_OFF_GETALARMIDS);(*pVal).Add(sVal);
	
		 sVal.Format("%li,报警数据,根据报警日志列表得到监测站级历史报警统计,保留",SERVICE_SERVICE_OFF_GETALARMDATA);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,监测站级历史报警ID列表,保留",SERVICE_SERVICE_OFF_GETALARMIDS);(*pVal).Add(sVal);

		 sVal.Format("%li,报警数据,集团级历史报警统计,保留" ,SERVICE_SERVICE_OFF_GROUPALARMSTAT);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,公司级历史报警统计,保留" ,SERVICE_SERVICE_OFF_COMPANYALARMSTAT);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,监测站级历史报警统计,保留" ,SERVICE_SERVICE_OFF_MONITORALARMSTAT);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,监测站级历史报警统计,优化读取数据,保留", SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR ); (*pVal).Add(sVal);

		 sVal.Format("%li,报警数据,单通道多特征值报警查询，得到指定报警测点的各种类型特征值,保留", SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES ); (*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值,保留", SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE ); (*pVal).Add(sVal);

		 sVal.Format("%li,历史数据,得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹,保留", SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到往复式机械设备运行状态历史趋势,保留", SERVICE_SERVICE_OFF_GETHISRUNSTATUS ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到往复式机械设备所有测点的特征值历史趋势比较,保留", SERVICE_SERVICE_OFF_GETHISCOMPARE ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到往复式机械设备所有通道指定时间的特征值和波形数据比较,保留", SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到往复式机械设备振动通道和动态通道指定时间的特征值,保留", SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE ); (*pVal).Add(sVal);

		 sVal.Format("%li,报表信息,得到往复式机械设备机组月报表,保留", SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT ); (*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定振动测点、指定时间的提纯轴心轨迹波形,保留",SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK);(*pVal).Add(sVal);

		 sVal.Format("%li,历史数据,得到指定公司、分厂机组和测点指定时刻的倒谱数据,保留"	 ,SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,往复机械报警查询、得到指定报警ID最先报警的测点名和特征值趋势,保留",SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES);(*pVal).Add(sVal);
		 sVal.Format("%li,报警数据,往复机械报警查询、得到指定时间的指定活塞杆沉降或动态压力测点的报警记录,保留",SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到指定公司、分厂机组和一对振动测点指定时刻的全谱数据,保留",SERVICE_SERVICE_OFF_GETFULLSPECSTRUM);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,报警查询功能、查询报警ID报警类型,保留",SERVICE_SERVICE_OFF_GETALARMLOGINFO);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,往复式系统应力监测，得到指定机组指定时间、指定缸号的应力波形和振动测点波形,保留",SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE);(*pVal).Add(sVal);

		 sVal.Format("%li,历史数据,得到指定公司、分厂、机组、时间段、任意多个测点的自定义参数,保留",SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND);(*pVal).Add(sVal);
		 sVal.Format("%li,历史数据,得到离线巡检数据",SERVICE_SERVICE_OFF_GET_OFFLINE_DATA);(*pVal).Add(sVal);
		 sVal.Format("%li,离线数据,请求离线巡检数据同步，返回离线测点索引和个数，及同步时间",SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC);(*pVal).Add(sVal);
		 sVal.Format("%li,离线数据,接收离线巡检数据",SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,保存启停车振动数据,保留"	 ,SERVICE_SERVICE_OFF_SUD_VIB_DATA);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,保存启停车动态数据,保留"	 ,SERVICE_SERVICE_OFF_SUD_DYN_DATA);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,保存启停车过程量数据,保留"	 ,SERVICE_SERVICE_OFF_SUD_PROC_DATA);(*pVal).Add(sVal);
		 sVal.Format("%li,启停车数据,同步启停车日志,保留"	 ,SERVICE_SERVICE_OFF_SUD_ADD_LOG);(*pVal).Add(sVal);

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


bool Do		  ( struct Service_Module * pModule )   //操作函数
{
/*
	switch (pModule->pFixComm->GetFunc())
    {
	   case SERVICE_SERVICE_OFF_GETCURTREND:
		   *pModule->pCode= Service_OffGetCurTrend(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECFREQWAVE:
		   *pModule->pCode= Service_OffGetSpecFreqWave(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETTREND:
		   *pModule->pCode= Service_OffGetTrend(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAVIBCHANNTREND:
		   *pModule->pCode= Service_OffGetAVibChannTrend(pModule->pFixComm , pModule->pDB);
		   break;
       case SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND:
		   *pModule->pCode= Service_OffGetAxisLocationTrend(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO:
		   *pModule->pCode=Srv_OffGetTwoChannDetailInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETZOOMSPEC:
           *pModule->pCode= Service_OffGetZoomSpec(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAXESTRACK:
		   *pModule->pCode= Service_OffGetAxesTrack(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO:
		   *pModule->pCode=Srv_OffGetSpecPlantChannsDetailInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND:
		   *pModule->pCode=Srv_OffGetSpecEventChannsSudTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND:
		   *pModule->pCode=Srv_OffGetVibChannAlarmTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND:
		   *pModule->pCode=Srv_OffGetAxesLocationAlarmTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA:
		   *pModule->pCode=Srv_OffGetAxesLocationAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND:
		   *pModule->pCode=Srv_OffGetAxisDisplaceAlarmTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO:
		   *pModule->pCode= Srv_OffGetSpecVibChannAlarmInfo(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_OFF_GETVIBCHANNALARMLOG:
		   *pModule->pCode= Srv_OffGetVibChannAlarmLog(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD:
		   *pModule->pCode= Service_OffGetSpecAlarmTimeRecord(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND:
		   *pModule->pCode=Srv_OffGetVibAlarmInfoTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETPROCALARMTREND:
		   *pModule->pCode=Srv_OffGetProcAlarmTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETPROCALARMDATA:
		   *pModule->pCode=Srv_OffGetProcAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETTRENDALARMDATA:
		   *pModule->pCode=Srv_OffGetTrendAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA:
           *pModule->pCode=Srv_OffGetAxisDisplaceAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE:
		   *pModule->pCode=Srv_OffGetSpecAlarmRecordCharValue(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETPVDATA:
		   *pModule->pCode = Srv_OffGetPVData( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETRODSINKDATA:
		   *pModule->pCode = Srv_OffGetRodSinkData( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA:
		   *pModule->pCode = Srv_OffGetRcMultiParamData( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE:
		   *pModule->pCode = Srv_OffGetSpecTimeAllChannValue( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK:
		   *pModule->pCode = Srv_OffGetSudPurifyAxesTrack( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA:
		   *pModule->pCode = Srv_OffGetSpecTimeAlarmMultiParamData( pModule->pFixComm, pModule->pDB );
		   break;
	   case SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE:
		   *pModule->pCode = Srv_OffGetSpecTimeAlarmDynStressWave( pModule->pFixComm, pModule->pDB );
		   break;

	 //启停车数据
       case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD:
		   *pModule->pCode= Service_OffGetSpecTimeRangeSud(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD:
		   *pModule->pCode= Service_OffGetSpecTimeChannSud(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD:
		   *pModule->pCode= Service_OffGetSpecChannRevSud(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD:
		   *pModule->pCode= Service_offGetSpecChannSaveTimeSud(pModule->pFixComm , pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETVIBCHANNSTREND:
		   *pModule->pCode=Srv_OffGetVibChannsTrend(pModule->pFixComm,pModule->pDB);
	 	   break;
	   case SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND:
		   *pModule->pCode=Srv_OffGetCurrVibChannsTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETTWOCHANNDATA:
		   *pModule->pCode=Srv_OffGetTwoChannData(pModule->pFixComm,pModule->pDB);
		   break;
       case SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO:
		   *pModule->pCode=Srv_OffGetSpecPlantProcChannsDetailInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO:
		   *pModule->pCode=Srv_OffGetSpecEventTimeChannsSudDetailInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO:
		   *pModule->pCode=Srv_OffGetSpecPlantChannInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE:
		   *pModule->pCode=Srv_OffGetAlarmDataTimeRange(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT:
		   *pModule->pCode=Srv_OffGetSpecPlantMonthReport(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO:
		   *pModule->pCode=Srv_OffGetSpecTimeRangeFactoryRunInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_REQSUDEVENTID:
		   *pModule->pCode=Srv_OffReqSudEventID(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_NOTICESUDSTOP:
		   *pModule->pCode=Srv_OffNoticeSudStop(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_RECSUDDATA:
		   *pModule->pCode=Srv_OffRecSudData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_CORRDIGN:
		   *pModule->pCode=Srv_OffCorrDign(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_ADDWHOLESUDLOG:
		   *pModule->pCode=Srv_OffAddWholeSudLog(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_SAVETHINFO:
		   *pModule->pCode=Srv_OffSaveThInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP:
		   *pModule->pCode=Srv_OffNoticeFastAlarmStop(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_RECFASTALARMDATA:
		   *pModule->pCode=Srv_OffRecFastAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID:
		   *pModule->pCode=Srv_OffSaveAlarmedNewThID(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_ADDALARMLOG:
		   *pModule->pCode=Srv_OffAddAlarmLog(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG:
		   *pModule->pCode=Srv_OffAddFastAlarmStartLog(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_RECDYNALARMDATA:
		   *pModule->pCode=Srv_OffReceiveDynAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_RECPROCALARMDATA:
		   *pModule->pCode=Srv_OffReceiveProcAlarmData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GROUPALARMSTAT:
		   *pModule->pCode=Srv_OffGetGroupAlarmStat(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_COMPANYALARMSTAT:
		   *pModule->pCode=Srv_OffGetCompanyAlarmStat(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_MONITORALARMSTAT:
		   *pModule->pCode=Srv_OffGetMonitorAlarmStat(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR:
		   *pModule->pCode=Srv_OffGetMonitorAlarmStatChar(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES:
		   *pModule->pCode=Srv_OffGetSpecChannAlarmMultCharValues(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE:
		   *pModule->pCode=Srv_OffGetSpecChannsAlarmSingleCharValue(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION:
		   *pModule->pCode=Srv_OffGetSpecChannRodSinkAxesLocation(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETHISRUNSTATUS:
		   *pModule->pCode=Srv_OffGetSpecChannsRunStatus(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETHISCOMPARE:
		   *pModule->pCode=Srv_OffGetSpecChannsHisCompare(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE:
		   *pModule->pCode=Srv_OffGetSpecChannsCharValueWave(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE:
		   *pModule->pCode=Srv_OffGetSpecChannsCharValue(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT:
		   *pModule->pCode=Srv_OffGetSpecRecipMonthReport(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK:
		   *pModule->pCode=Srv_OffGetPurifyAxesTrack(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM:
		   *pModule->pCode=Service_OffGetSpecChannCepstrum(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES:
		   *pModule->pCode=Srv_OffGetSpecAlarmIDCharValues(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD:
		   *pModule->pCode=Srv_OffGetSpecAlarmTimeDynRecord(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETFULLSPECSTRUM:
		   *pModule->pCode=Srv_OffGetSpecTimeFullSpectrum(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETALARMLOGINFO:
		   *pModule->pCode=Srv_OffGetAlarmLogInfo(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE:
		   *pModule->pCode=Srv_OffGetSpecTimeDynStressWave(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND:
		   *pModule->pCode=Srv_OffGetSpecChannsCustomTrend(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_GET_OFFLINE_DATA:
		   *pModule->pCode=Srv_OffGetOfflineData(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC:
		   *pModule->pCode=Srv_OffRequestOfflineSync(pModule->pFixComm,pModule->pDB);
		   break;
	   case SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA:
		   *pModule->pCode=Srv_OffSyncOfflineData(pModule->pFixComm,pModule->pDB);
		   break;

	   default:
			return false;
	}
*/
		CBHCommWrapper loComm;

		loComm.SetFixComm(pModule->pFixComm);
		bool lbNeedOpenDB =false;
		switch (pModule->pFixComm->GetFunc())
		{	
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


		case SERVICE_SERVICE_OFF_SAVESPECMARK:
			lbNeedOpenDB = true;
			break;
		case SERVICE_SERVICE_OFF_GETSPECMARK:
			lbNeedOpenDB = true;
			break;
		case SERVICE_SERVICE_OFF_SET_DISK_FULL:
			lbNeedOpenDB=true;
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


	if (lbNeedOpenDB)
	{
		COleSqlServerDB loDb;

		if (!loDb.IsDbSafe())
		{
			*(pModule->pCode) = -311;//数据库连接失败	

			return true;
		}
		loDb.setPDB(pModule->pDB);

		CBHCommWrapper loFixComm;
		loFixComm.SetFixComm(pModule->pFixComm);		

		switch (pModule->pFixComm->GetFunc())
		{	
		case SERVICE_SERVICE_OFF_GETCURTREND:
			*pModule->pCode= Service_OffGetCurTrend(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECFREQWAVE:
			*pModule->pCode= Service_OffGetSpecFreqWave(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETTREND:
			*pModule->pCode= Service_OffGetTrend(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAVIBCHANNTREND:
			*pModule->pCode= Service_OffGetAVibChannTrend(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND:
			*pModule->pCode= Service_OffGetAxisLocationTrend(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETTWOCHANNDETAILINFO:
			*pModule->pCode=Srv_OffGetTwoChannDetailInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETZOOMSPEC:
			*pModule->pCode= Service_OffGetZoomSpec(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXESTRACK:
			*pModule->pCode= Service_OffGetAxesTrack(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSDETAILINFO:
			*pModule->pCode=Srv_OffGetSpecPlantChannsDetailInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECEVENTCHANNSSUDTREND:
			*pModule->pCode=Srv_OffGetSpecEventChannsSudTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND:
			*pModule->pCode=Srv_OffGetVibChannAlarmTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMTREND:
			*pModule->pCode=Srv_OffGetAxesLocationAlarmTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXESLOCATIONALARMDATA:
			*pModule->pCode=Srv_OffGetAxesLocationAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMTREND:
			*pModule->pCode=Srv_OffGetAxisDisplaceAlarmTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECVIBCHANNALARMINFO:
			*pModule->pCode= Srv_OffGetSpecVibChannAlarmInfo(&loFixComm , &loDb);
			break;
		case SERVICE_OFF_GETVIBCHANNALARMLOG:
			*pModule->pCode= Srv_OffGetVibChannAlarmLog(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD:
			*pModule->pCode= Service_OffGetSpecAlarmTimeRecord(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETVIBALARMINFOTREND:
			*pModule->pCode=Srv_OffGetVibAlarmInfoTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETPROCALARMTREND:
			*pModule->pCode=Srv_OffGetProcAlarmTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETPROCALARMDATA:
			*pModule->pCode=Srv_OffGetProcAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETTRENDALARMDATA:
			*pModule->pCode=Srv_OffGetTrendAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETAXISDISPLACEALARMDATA:
			*pModule->pCode=Srv_OffGetAxisDisplaceAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE:
			*pModule->pCode=Srv_OffGetSpecAlarmRecordCharValue(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETPVDATA:
			*pModule->pCode = Srv_OffGetPVData( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETRODSINKDATA:
			*pModule->pCode = Srv_OffGetRodSinkData( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA:
			*pModule->pCode = Srv_OffGetRcMultiParamData( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE:
			*pModule->pCode = Srv_OffGetSpecTimeAllChannValue( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETSUDPURIFYAXESTRACK:
			*pModule->pCode = Srv_OffGetSudPurifyAxesTrack( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA:
			*pModule->pCode = Srv_OffGetSpecTimeAlarmMultiParamData( &loFixComm, &loDb );
			break;
		case SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE:
			*pModule->pCode = Srv_OffGetSpecTimeAlarmDynStressWave(&loFixComm, &loDb );
			break;

			//启停车数据
		case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUD:
			*pModule->pCode= Service_OffGetSpecTimeRangeSud(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD:
			*pModule->pCode= Service_OffGetSpecTimeChannSud(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD_EX:
			*pModule->pCode= Service_OffGetSpecTimeChannSudEx(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD:
			*pModule->pCode= Service_OffGetSpecChannRevSud(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD:
			*pModule->pCode= Service_offGetSpecChannSaveTimeSud(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNSAVETIMESUD_EX:
			*pModule->pCode= Service_offGetSpecChannSaveTimeSudEx(&loFixComm , &loDb);
			break;
		case SERVICE_SERVICE_OFF_GETVIBCHANNSTREND:
			*pModule->pCode=Srv_OffGetVibChannsTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETCURRVIBCHANNSTREND:
			*pModule->pCode=Srv_OffGetCurrVibChannsTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETTWOCHANNDATA:
			*pModule->pCode=Srv_OffGetTwoChannData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECPLANTPROCCHANNSDETAILINFO:
			*pModule->pCode=Srv_OffGetSpecPlantProcChannsDetailInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECEVENTTIMECHANNSSUDDETAILINFO:
			*pModule->pCode=Srv_OffGetSpecEventTimeChannsSudDetailInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECPLANTCHANNSINFO:
			*pModule->pCode=Srv_OffGetSpecPlantChannInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETALARMDATATIMERANGE:
			*pModule->pCode=Srv_OffGetAlarmDataTimeRange(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNSUDBYTIME:
			*pModule->pCode=Srv_OffGetSpecChannSudByTime(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECPLANTMONTHREPORT:
			*pModule->pCode=Srv_OffGetSpecPlantMonthReport(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO:
			*pModule->pCode=Srv_OffGetSpecTimeRangeFactoryRunInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_REQSUDEVENTID:
			*pModule->pCode=Srv_OffReqSudEventID(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_NOTICESUDSTOP:
			*pModule->pCode=Srv_OffNoticeSudStop(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_RECSUDDATA:
			*pModule->pCode=Srv_OffRecSudData(&loComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_CORRDIGN:
			*pModule->pCode=Srv_OffCorrDign(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_ADDWHOLESUDLOG:
			*pModule->pCode=Srv_OffAddWholeSudLog(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SAVETHINFO:
			*pModule->pCode=Srv_OffSaveThInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP:
			*pModule->pCode=Srv_OffNoticeFastAlarmStop(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_RECFASTALARMDATA:
			*pModule->pCode=Srv_OffRecFastAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID:
			*pModule->pCode=Srv_OffSaveAlarmedNewThID(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_ADDALARMLOG:
			*pModule->pCode=Srv_OffAddAlarmLog(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG:
			*pModule->pCode=Srv_OffAddFastAlarmStartLog(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_RECDYNALARMDATA:
			*pModule->pCode=Srv_OffReceiveDynAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_RECPROCALARMDATA:
			*pModule->pCode=Srv_OffReceiveProcAlarmData(&loFixComm,&loDb);
			break;	
		case SERVICE_SERVICE_OFF_GROUPALARMSTAT:
			*pModule->pCode=Srv_OffGetGroupAlarmStat(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_COMPANYALARMSTAT:
			*pModule->pCode=Srv_OffGetCompanyAlarmStat(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_MONITORALARMSTAT:
			*pModule->pCode=Srv_OffGetMonitorAlarmStat(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_MONITORALARMSTATEX:
			*pModule->pCode=Srv_OffGetMonitorAlarmStatEx(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR:
			*pModule->pCode=Srv_OffGetMonitorAlarmStatChar(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES:
			*pModule->pCode=Srv_OffGetSpecChannAlarmMultCharValues(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE:
			*pModule->pCode=Srv_OffGetSpecChannsAlarmSingleCharValue(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION:
			*pModule->pCode=Srv_OffGetSpecChannRodSinkAxesLocation(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETHISRUNSTATUS:
			*pModule->pCode=Srv_OffGetSpecChannsRunStatus(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETHISCOMPARE:
			*pModule->pCode=Srv_OffGetSpecChannsHisCompare(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHARVALUEWAVE:
			*pModule->pCode=Srv_OffGetSpecChannsCharValueWave(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE:
			*pModule->pCode=Srv_OffGetSpecChannsCharValue(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT:
			*pModule->pCode=Srv_OffGetSpecRecipMonthReport(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK:
			*pModule->pCode=Srv_OffGetPurifyAxesTrack(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM:
			*pModule->pCode=Service_OffGetSpecChannCepstrum(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES:
			*pModule->pCode=Srv_OffGetSpecAlarmIDCharValues(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECALARMTIMEDYNRECORD:
			*pModule->pCode=Srv_OffGetSpecAlarmTimeDynRecord(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETFULLSPECSTRUM:
			*pModule->pCode=Srv_OffGetSpecTimeFullSpectrum(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETALARMLOGINFO:
			*pModule->pCode=Srv_OffGetAlarmLogInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE:
			*pModule->pCode=Srv_OffGetSpecTimeDynStressWave(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSEPCCHANNCUSTOMSTREND:
			*pModule->pCode=Srv_OffGetSpecChannsCustomTrend(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GET_OFFLINE_DATA:
			*pModule->pCode=Srv_OffGetOfflineData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC:
			*pModule->pCode=Srv_OffRequestOfflineSync(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA:
			*pModule->pCode=Srv_OffSyncOfflineData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETALARMDATA:
			*pModule->pCode=Srv_OffGetAlarmData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETALARMIDS:
			*pModule->pCode=Srv_OffGetAlarmIDS(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SET_BALANCE_DATA:
			*pModule->pCode=Srv_SetBalanceData(&loFixComm,&loDb);
			break;	
		case SERVICE_SERVICE_OFF_GET_BALANCE_DATA:
			*pModule->pCode=Srv_GetBalanceData(&loFixComm,&loDb);
			break;	
		case SERVICE_SERVICE_TEMP_SAMPLE_PLANTS:
			*pModule->pCode = Service_temp_sample_plants(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDEX:
			*pModule->pCode = Service_GetSpecTimeRangeSudAndName(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECTIMERANGESUDBYEVENTNAME:
			*pModule->pCode = GetSpecTimeRangeSudSearchByName(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_RENAMEEVENT:
			*pModule->pCode = UpdateRenameEvent(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SAVESPECTIMEDATA:
			*pModule->pCode = SaveSpecTypeDataByTimeRegion(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETVIBCHANNSTRENDEX:
			*pModule->pCode=Srv_OffGetVibChannsTrendEx(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETHISRUNSTATUSEX:
			*pModule->pCode=Srv_OffGetSpecChannsRunStatusEx(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SUD_VIB_DATA:
			*pModule->pCode=Srv_OffRecSudVibData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SUD_DYN_DATA:
			*pModule->pCode=Srv_OffRecSudDynData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SUD_PROC_DATA:
			*pModule->pCode=Srv_OffRecSudProcData(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SUD_ADD_LOG:
			*pModule->pCode=Srv_OffRecSudLog(&loFixComm,&loDb);
			break;


		case SERVICE_SERVICE_OFF_SAVESPECMARK:
			*pModule->pCode=SaveSpecMarkInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETSPECMARK:
			*pModule->pCode=GetSpecMarkInfo(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SET_DISK_FULL:
			*pModule->pCode = Srv_OffSetDiskFull(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GETNORMALWAVE:
			*pModule->pCode = Srv_OffGetNormalWave(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_MONITORALARMSTATWITHLEVEL:
			*pModule->pCode=Srv_OffGetMonitorAlarmStatWithLevel(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_GET_ERROR_SIGNAL_WAVE:
			*pModule->pCode=Srv_OffGetErrorSignalWave(&loFixComm,&loDb);
			break;
		case SERVICE_SERVICE_OFF_SET_AXESLOCATION_PARAM:
			*pModule->pCode=Srv_OffSetAxesLocationParam(&loFixComm,&loDb);
			break;
		default:
			return false;
		}


		return true;
	}


	return false;
}


bool DoNext   ( struct Service_Module * pModule )    //操作函数
{
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //预留函数
{
	 return true;
}
