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
		//�м��ģ��
		//�õ������񶯲����������͵�����ֵ
		//���ܴ��ݽ��������в���ʵʱ����ֵ����λ�ƺ͹���������,��λ��
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
		/ * ��������Ӳ��״̬ * /
	case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
		lbNeedOpenDB = true;
		break;
		/ * �����������״̬ * /
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
	{		//�м��ģ��
		//���ܴ��ݽ��������в���ʵʱ����ֵ����λ�ƺ͹���������,��λ��
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
		"ʵʱ���ݴ���"			,
		"ʵʱ����"            ,
		"���ݴ���"	        ,
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
#ifndef _DEBUG

		CStringArray *pVal=(CStringArray *)pModule->pInputPointer ;
		CString sVal ;
		sVal.Format("%li,ʵʱ����,�õ�ָ�����������񶯲���������������,����"	,SERVICE_SERVICE_ON_GETALLFIVECHAR);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ��ָ�������ָ���񶯲��������������ֵ,����",SERVICE_SERVICE_ON_GETSPECFIVECHAR);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ�����������������ֵ,����" ,SERVICE_SERVICE_ON_GETALLTYPECHAR);(*pVal).Add(sVal);

		sVal.Format("%li,ʵʱ����,�õ�ָ������ָ���ĸ��񶯲��ָ�����͵�����ֵ,����" ,SERVICE_SERVICE_ON_GETFOURSPECCHAR);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ���������й�������ֵ,����" ,SERVICE_SERVICE_ON_GETALLPROCVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ������ָ�����������ֵ,����" ,SERVICE_SERVICE_ON_GETSPECPROCVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ�����������񶯲��Ĳ��κ�Ƶ������,����",SERVICE_SERVICE_ON_GETWAVE);(*pVal).Add(sVal);
		sVal.Format("%li,���Ĺ켣,�õ�ָ������ָ��һ��������Ĺ켣,����" ,SERVICE_SERVICE_ON_GETAXESTRACK);(*pVal).Add(sVal);
		sVal.Format("%li,����λ��,�õ�ָ������ָ��һ��������λ��ֵ,����" ,SERVICE_SERVICE_ON_GETAXESLOCATION);(*pVal).Add(sVal);
		sVal.Format("%li,�豸ת��,�õ�ָ��������豸ת��,����" ,SERVICE_SERVICE_ON_GETREV);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ������ָ����λ�Ʋ��ֵ,����" ,SERVICE_SERVICE_ON_GETSPECAXISDISPLACE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ������������λ�Ƶ�ֵ,����" ,SERVICE_SERVICE_ON_GETALLAXISDISPLACE);(*pVal).Add(sVal);
		sVal.Format("%li,����״̬,�õ�ָ���û������豸�ı���״̬,����" ,SERVICE_SERVICE_ON_GETALLCHANNELSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ���������в������,����" ,SERVICE_SERVICE_ON_GETALLCHANNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ������Ͳ���ʾ��ͼ,����" ,SERVICE_SERVICE_ON_GETPVDATA);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ������Ͳ��Ļ������³�������,����" ,SERVICE_SERVICE_ON_GETRODSINKDATA);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ���ֳ������豸��״̬,����" ,SERVICE_SERVICE_ON_GETSUBCORPSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ�������㲨��ͼ,����" ,SERVICE_SERVICE_ON_GETBODE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ���������ٲ�ͼ,����" ,SERVICE_SERVICE_ON_GETWATERFALL);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ����,�õ�ָ��������ϸ����,����" ,SERVICE_SERVICE_ON_GETZOOMSPEC);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ��Ϣ,�õ�ָ��������ת�ٺͲ�����Ϣ,����" ,SERVICE_SERVICE_ON_GETSPECCHANNINFO);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ��Ϣ,�õ�ָ�������ֵ����λ,����" ,SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE);(*pVal).Add(sVal);
		sVal.Format("%li,ʵʱ��Ϣ,�õ�ָ������������������,����" ,SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA);(*pVal).Add(sVal);

		sVal.Format("%li,��������,�����񶯲��ʵʱ����,����" ,SERVICE_SERVICE_ON_RECEIVEALLVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,��������,���ܶ�̬���ʵʱ����,����" ,SERVICE_SERVICE_ON_RECEIVEDYNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,��������,ת��,����" ,SERVICE_SERVICE_ON_RECEIVEREV);(*pVal).Add(sVal);
		sVal.Format("%li,��������,������,����" ,SERVICE_SERVICE_ON_RECEIVEPROC);(*pVal).Add(sVal);
		sVal.Format("%li,��������,����״̬,����" ,SERVICE_SERVICE_ON_RECEIVECHANNSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,��������,ָ��������ͣ������,����" ,SERVICE_SERVICE_ON_REVSUDDATA);(*pVal).Add(sVal);
		sVal.Format("%li,֪ͨ,��������,����" ,SERVICE_SERVICE_ON_SAVEDATA);(*pVal).Add(sVal);
		sVal.Format("%li,֪ͨ,�жϱ���,����" ,SERVICE_SERVICE_ON_JUDGEALARM);(*pVal).Add(sVal);

		sVal.Format("%li,����ID,ָ�����鱨����ֵID,����" ,SERVICE_SERVICE_ON_REQTHESHOLDID);(*pVal).Add(sVal);
		sVal.Format("%li,�����䱨��ID,�����䱨��ID,����" ,SERVICE_SERVICE_ON_REQFASTALARMEVENTID);(*pVal).Add(sVal);
		sVal.Format("%li,��������,���¿�䱨��״̬,����" ,SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS);(*pVal).Add(sVal);

		sVal.Format("%li,��������,�����񶯲������󱣴������,����" ,SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,��������,���ܶ�̬�������󱣴������,����" ,SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE);(*pVal).Add(sVal);
		sVal.Format("%li,��������,���ܹ������������󱣴������,����" ,SERVICE_SERVICE_ON_NETOFFRECEIVEPROC);(*pVal).Add(sVal);

		sVal.Format("%li,�õ�����,ָ���豸����������,����" ,SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,ָ���豸���й���������,����" ,SERVICE_SERVICE_ON_GETSPECPLANTPROCDATA);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,ָ���豸����״̬,����" ,SERVICE_SERVICE_ON_GETSPECPLANTALLSTATE);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,ָ���豸���ж�̬����,����" ,SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA);(*pVal).Add(sVal);

		sVal.Format("%li,�õ�����,���ż�����ͳ��,����" ,SERVICE_SERVICE_ON_GROUPALARMSTAT);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,��˾������ͳ��,����" ,SERVICE_SERVICE_ON_COMPANYALARMSTAT);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,���վ������ͳ��,����" ,SERVICE_SERVICE_ON_MONITORALARMSTAT);(*pVal).Add(sVal);

		sVal.Format("%li,�õ�����,�õ�ָ������ָ��һ����Ļ���������λ�ù켣,����" ,SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,�õ�ָ������ָ���������Ļ�����Ӧ���仯����,����" ,SERVICE_SERVICE_ON_GETDYNSTRESSWAVE);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,�õ�ĳһ���ᴿ���Ĺ켣�Ĳ�������,����" ,SERVICE_SERVICE_ON_GETPURIFYAXESTRACK);(*pVal).Add(sVal);

		sVal.Format("%li,ʵʱ����,�õ�ָ�������񶯲��Ĳ��κ͵�������,����",SERVICE_SERVICE_ON_GETCEPSTRUM);(*pVal).Add(sVal);
		sVal.Format("%li,�õ�����,�õ�ָ��һ���񶯲���ʵʱȫ�׺�ȫ���ٲ�ͼ,����",SERVICE_SERVICE_ON_GETFULLSPECSTRUM);(*pVal).Add(sVal);

		/* �ػ�Ѳ�� */
		sVal.Format("%li,�ػ�Ѳ��,��ȡ�쳣���ݼ����,����",SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD);(*pVal).Add(sVal);

		sVal.Format("%li,�ػ�Ѳ��,��������Ӳ��״̬,����", SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,�ػ�Ѳ��,�����������״̬,����",SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS);(*pVal).Add(sVal);

		sVal.Format("%li,�豸״̬,ͬ���豸����״̬,����" ,SERVICE_SERVICE_ON_SET_PLANT_STATUS);(*pVal).Add(sVal);
		sVal.Format("%li,�豸״̬,��ȡ�豸����״̬,����" ,SERVICE_SERVICE_ON_GET_PLANT_STATUS);(*pVal).Add(sVal);
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

bool Quit	( struct Service_Module * pModule )   //�˳�����
{
	return true;
}

bool Run	( struct Service_Module * pModule )   //ִ�к���
{
	return true;
}

/* �̰߳�ȫ��oledb���ӳ�
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

	if (::CheckDBConn(pModule->pDB)	<0)
	{
		* pModule->pCode = -311;

		return false;
	}

	bool lbNeedOpenDB = false;

	int lnCode = -1;

	switch ( pModule->pFixComm->GetFunc() )
	{
		//�м��ģ��
		//�õ������񶯲����������͵�����ֵ
	case SERVICE_SERVICE_ON_GETALLFIVECHAR:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ������������������ֵ
	case SERVICE_SERVICE_ON_GETSPECFIVECHAR:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ����������������������ֵ
	case SERVICE_SERVICE_ON_GETALLTYPECHAR:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������񶯲���Ƶ������
	case SERVICE_SERVICE_ON_GETSPECTRUM:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ���ĸ��񶯲��ָ�����͵�����ֵ
	case SERVICE_SERVICE_ON_GETFOURSPECCHAR:
		lbNeedOpenDB = true;
		break;
		//�õ����й������Ĳ��ֵ
	case SERVICE_SERVICE_ON_GETALLPROCVALUE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ�����������ֵ
	case SERVICE_SERVICE_ON_GETSPECPROCVALUE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ�����������񶯲��Ĳ��κ�Ƶ������
	case SERVICE_SERVICE_ON_GETWAVE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ��һ��������Ĺ켣
	case SERVICE_SERVICE_ON_GETAXESTRACK:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ��һ��������λ��ֵ
	case SERVICE_SERVICE_ON_GETAXESLOCATION:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ��������豸ת��
	case SERVICE_SERVICE_ON_GETREV:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ����λ�Ʋ��ֵ
	case SERVICE_SERVICE_ON_GETSPECAXISDISPLACE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������������λ�Ƶ�ֵ
	case SERVICE_SERVICE_ON_GETALLAXISDISPLACE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ���û������豸�ı���״̬
	case SERVICE_SERVICE_ON_GETALLCHANNELSTATE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ���������в���ֵ
	case SERVICE_SERVICE_ON_GETALLCHANNVALUE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ�������񶯲�����λ
	case SERVICE_SERVICE_ON_GETPVDATA:
		lbNeedOpenDB = true;
		break;
	case SERVICE_SERVICE_ON_GETRODSINKDATA:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ���ֳ������豸��״̬
	case SERVICE_SERVICE_ON_GETSUBCORPSTATE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ��������Ĳ���ͼ
	case SERVICE_SERVICE_ON_GETBODE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ����������ٲ�ͼ
	case SERVICE_SERVICE_ON_GETWATERFALL:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ���������ϸ����ͼ
	case SERVICE_SERVICE_ON_GETZOOMSPEC:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������Ͳ���ת�ٺͲ�����Ϣ
	case SERVICE_SERVICE_ON_GETSPECCHANNINFO:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ��������񶯲��ķ�ֵ����λ
	case SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������Ķ������������
	case SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA:
		lbNeedOpenDB = true;
		break;
		//�õ����ż�����ͳ������
	case SERVICE_SERVICE_ON_GROUPALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//�õ���˾������ͳ������
	case SERVICE_SERVICE_ON_COMPANYALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//�õ����վ������ͳ������
	case SERVICE_SERVICE_ON_MONITORALARMSTAT:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ��һ����Ļ���������λ�ù켣
	case SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION:
		lbNeedOpenDB = true;
		break;
		//�õ�ָ������ָ���������Ļ�����Ӧ���仯����
	case SERVICE_SERVICE_ON_GETDYNSTRESSWAVE:
		lbNeedOpenDB = true;
		break;
		//�õ�ĳһ���ᴿ���Ĺ켣�Ĳ�������
	case SERVICE_SERVICE_ON_GETPURIFYAXESTRACK:
		lbNeedOpenDB = true;
		break;

		//���ܴ��ݽ��������в���ʵʱ����ֵ����λ�ƺ͹���������,��λ��
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

		/* �ػ�Ѳ�� */
		/* ��ȡ�쳣���ݼ���� */
	case SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD:
		lbNeedOpenDB = true;
		break;
		/* ��������Ӳ��״̬ */
	case SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS:
		lbNeedOpenDB = true;
		break;
		/* �����������״̬ */
	case SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS:
		lbNeedOpenDB = true;
		break;
		/*�õ�������ĵ��豸��Ϣ��*/
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
		{		//�м��ģ��
			//�õ������񶯲����������͵�����ֵ
		case SERVICE_SERVICE_ON_GETALLFIVECHAR:
			*pModule->pCode =  Service_OnGetAllFiveChar(&loComm , &loDb );
			break;
			//�õ�ָ������ָ������������������ֵ
		case SERVICE_SERVICE_ON_GETSPECFIVECHAR:
			*pModule->pCode=Service_OnGetSpecFiveChar(&loComm , &loDb);
			break;
			//�õ�ָ����������������������ֵ
		case SERVICE_SERVICE_ON_GETALLTYPECHAR:
			*pModule->pCode= Srv_OnGetAllTypeChar(&loComm , &loDb);
			break;
			//�õ�ָ������񶯲���Ƶ������
		case SERVICE_SERVICE_ON_GETSPECTRUM:
			*pModule->pCode=Srv_OnGetSpectrum(&loComm,&loDb);
			break;
			//�õ�ָ������ָ���ĸ��񶯲��ָ�����͵�����ֵ
		case SERVICE_SERVICE_ON_GETFOURSPECCHAR:
			*pModule->pCode=SerVice_OnGetFourSpecChar(&loComm , &loDb);
			break;
			//�õ����й������Ĳ��ֵ
		case SERVICE_SERVICE_ON_GETALLPROCVALUE:
			*pModule->pCode=SerVice_OnGetAllProcValue(&loComm , &loDb);
			break;
			//�õ�ָ������ָ�����������ֵ
		case SERVICE_SERVICE_ON_GETSPECPROCVALUE:
			*pModule->pCode=SerVice_OnGetSpecProcValue(&loComm , &loDb);
			break;
			//�õ�ָ�����������񶯲��Ĳ��κ�Ƶ������
		case SERVICE_SERVICE_ON_GETWAVE:
			*pModule->pCode= Service_OnGetWave(&loComm , &loDb );
			break;
			//�õ�ָ������ָ��һ��������Ĺ켣
		case SERVICE_SERVICE_ON_GETAXESTRACK:
			*pModule->pCode= Service_OnGetAxesTrack(&loComm , &loDb );
			break;
			//�õ�ָ������ָ��һ��������λ��ֵ
		case SERVICE_SERVICE_ON_GETAXESLOCATION:
			*pModule->pCode= Service_OnGetAxesLocation(&loComm , &loDb );
			break;
			//�õ�ָ��������豸ת��
		case SERVICE_SERVICE_ON_GETREV:
			*pModule->pCode= Service_OnGetRev(&loComm , &loDb );
			break;
			//�õ�ָ������ָ����λ�Ʋ��ֵ
		case SERVICE_SERVICE_ON_GETSPECAXISDISPLACE:
			*pModule->pCode= Service_OnGetSpecAxisDisplace(&loComm , &loDb );
			break;
			//�õ�ָ������������λ�Ƶ�ֵ
		case SERVICE_SERVICE_ON_GETALLAXISDISPLACE:
			*pModule->pCode= Service_OnGetAllAxisDisplace(&loComm , &loDb );
			break;
			//�õ�ָ���û������豸�ı���״̬
		case SERVICE_SERVICE_ON_GETALLCHANNELSTATE:
			*pModule->pCode= Service_OnGetAllChannelState(&loComm , &loDb );
			break;
			//�õ�ָ���������в���ֵ
		case SERVICE_SERVICE_ON_GETALLCHANNVALUE:
			*pModule->pCode= Service_OnGetAllChannelValue(&loComm , &loDb );
			break;
			//�õ�ָ�������񶯲�����λ
		case SERVICE_SERVICE_ON_GETPVDATA:
			*pModule->pCode= Srv_OnGetPVData(&loComm , &loDb );
			break;
		case SERVICE_SERVICE_ON_GETRODSINKDATA:
			*pModule->pCode=Srv_OnGetRodSinkData(&loComm,&loDb);
			break;
			//�õ�ָ���ֳ������豸��״̬
		case SERVICE_SERVICE_ON_GETSUBCORPSTATE:
			*pModule->pCode= Service_OnGetSubCorpState(&loComm , &loDb );
			break;
			//�õ�ָ��������Ĳ���ͼ
		case SERVICE_SERVICE_ON_GETBODE:
			*pModule->pCode= Service_OnGetBode(&loComm , &loDb );
			break;
			//�õ�ָ����������ٲ�ͼ
		case SERVICE_SERVICE_ON_GETWATERFALL:
			*pModule->pCode= Service_OnGetWaterFall(&loComm , &loDb );
			break;
			//�õ�ָ���������ϸ����ͼ
		case SERVICE_SERVICE_ON_GETZOOMSPEC:
			*pModule->pCode= Service_OnGetZoomSpec(&loComm , &loDb );
			break;
			//�õ�ָ������Ͳ���ת�ٺͲ�����Ϣ
		case SERVICE_SERVICE_ON_GETSPECCHANNINFO:
			*pModule->pCode=Service_OnGetSpecChannInfo(&loComm , &loDb );
			break;
			//�õ�ָ��������񶯲��ķ�ֵ����λ
		case SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE:
			*pModule->pCode=Service_OnGetAllSpecCharAndPhase(&loComm , &loDb );
			break;
			//�õ�ָ������Ķ������������
		case SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA:
			*pModule->pCode=Srv_OnGetRcMultiParamData(&loComm,&loDb);
			break;
			//�õ����ż�����ͳ������
		case SERVICE_SERVICE_ON_GROUPALARMSTAT:
			*pModule->pCode=Srv_OnGetGroupAlarmStat(&loComm,&loDb);
			break;
			//�õ���˾������ͳ������
		case SERVICE_SERVICE_ON_COMPANYALARMSTAT:
			*pModule->pCode=Srv_OnGetCompanyAlarmStat(&loComm,&loDb);
			break;
			//�õ����վ������ͳ������
		case SERVICE_SERVICE_ON_MONITORALARMSTAT:
			*pModule->pCode=Srv_OnGetMonitorAlarmStat(&loComm,&loDb);
			break;
			//�õ�ָ������ָ��һ����Ļ���������λ�ù켣
		case SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION:
			*pModule->pCode=Srv_OnGetRodSinkAxesLocation(&loComm,&loDb);
			break;
			//�õ�ָ������ָ���������Ļ�����Ӧ���仯����
		case SERVICE_SERVICE_ON_GETDYNSTRESSWAVE:
			*pModule->pCode=Srv_OnGetDynStressWave(&loComm,&loDb);
			break;
			//�õ�ĳһ���ᴿ���Ĺ켣�Ĳ�������
		case SERVICE_SERVICE_ON_GETPURIFYAXESTRACK:
			*pModule->pCode=Srv_OnGetPurifyAxesTrack(&loComm,&loDb);
			break;

			//���ܴ��ݽ��������в���ʵʱ����ֵ����λ�ƺ͹���������,��λ��
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


bool DoNext   ( struct Service_Module * pModule )    //��������
{
	return true;
}


bool DoReserve( struct Service_Module * pModule )    //Ԥ������
{
	return true;
}
