#include "StdAfx.h"
#include "ExpertSysGlobal.h"
#include "../ICEServer/ICEServerExport.h"
#include <MiddleWare.h>
#include <BaseFunc.h>
#include "process.h"


MAP_PLANTINFO g_mapPlantInfo;
bool g_bInit=false;

/************************************************************************/
/*  临时在线是否采集配置
SAMPLE_SETTING_RECOREDS=大庆石化|化工一厂|EC1301=1;大庆石化|化工一厂|EC301=0 */
/*  SAMPLE_TIME_RECOREDS=大庆石化|化工一厂|EC1301=2012-01-02 12:20:20;大庆石化|化工一厂|EC301=2012-01-01 12:20:20 */
/************************************************************************/

LY::IServerStatus * g_pServerStatus = NULL;
CCriticalSection * g_pServerStatusLock = NULL;


LY::IServer * g_pServer = NULL;
int * g_nServiceMark = 0;

std::map<DWORD,CCustomDB *> * gpDBPool = NULL;
CReaderWriterLock *g_pConnPoolLock = NULL;

CString g_strDbUser;
CString g_strDbPwd;
CString g_strDbName;
CString g_strDbServer;

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

std::map<CString,CString> goMapPlantFactName;

BOOL initPlantFactName()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	int lnBufferSize = 10240000;

	std::vector<TCHAR> loBuffer(lnBufferSize);
	TCHAR *lpBuffer = &loBuffer.front();

	CString lstrSplitPlant = _T(";");
	CString lstrSplitValue = _T("=");
	CString lstrBuffer;
	CStringArray loPlantArray;
	CStringArray loValueArray;


	int lnData = ::GetPrivateProfileString("SYSTEM",
		"PLANT_FACT_NAME"
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
		//if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);
			CString lstrFactName = lstrValue.GetBuffer(0);
			goMapPlantFactName[lstrKey] = lstrFactName;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrFactName));
		}
	}

	return TRUE;
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
	case SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT:
			lnCode=Service_ExpertSysGetRegAlarmFeatureExtract(&arefCommInterFace,&loDb);
			break;

		case SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT:
			lnCode=Service_ExpertSysGetSpecChannFeatureExtract(&arefCommInterFace,&loDb);
			break;

/*
		case SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT_RC:
			*pModule->pCode = CBHExpertBizObj::Handler<CBODiagnosisRC>(loCommWrapper,loDbWrapper);

			break;*/
		case  SERVICE_EXPERTSYS_GET_PLANTFACTNAME:
			lnCode = Service_ExpertSysGetPlantFactName(&arefCommInterFace,&loDb);
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

void InitGlobalVar(CCustomDB *pDB_)
{
	SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOALIGNMENTFAULTEXCEPT|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
	SetGlobalErrorMode(2);

	initPlantFactName();
	::SetUnhandledExceptionFilter(Winet::GEHExceptionFilter);
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
	CPlantInfoExpertSys *pPlantInfo=NULL;
	//添加设备详细信息
	for (itPlant=lPlantInfo.begin();itPlant!=lPlantInfo.end();++itPlant)
	{
		pPlantInfo = new CPlantInfoExpertSys;
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
		pPlantInfo->AddChannInfoEnd();
		//增加专家系统设备事实名称
		CString lstrKey = CString(sCompany.c_str())+_T("|")+CString(itPlant->_cFactory)+_T("|") + CString(itPlant->_cPlantID);

		std::map<CString,CString>::iterator lpIt = goMapPlantFactName.find(lstrKey);

		if (lpIt != goMapPlantFactName.end())
		{
			pPlantInfo->SetPlantFactName(lpIt->second);
		}

		sCompany.append(itPlant->_cPlantID);
		//AfxMessageBox(sCompany.c_str());

		g_mapPlantInfo.insert(make_pair(sCompany,pPlantInfo));
	}
	mapCompany.clear();
	lPlantInfo.clear();

	InitICEServer();

	RegistModuleComplete();
}

void QuitClearGlobalVar()
{
	MAP_PLANTINFO::iterator it;
	CPlantInfoExpertSys *pPlantInfo=NULL;
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
		token = strtok( NULL, seps );
	}
	//得到是否切换数据库的标识
	g_iDbType = GetPrivateProfileInt("DBINFO","DBTYPE",0x0000,cAppLocation);
	if (g_iDbType!=0 && g_iDbType!=1)
		CHZLogManage::Error("配置数据库的类型有误","ExpertSysGlobal","GetMainDBName");
	CString sTemp;sTemp.Format("得到全局的主数据库名和数据库类型标识:%s,%d",g_strMainDbName,g_iDbType);
	CHZLogManage::Debug(sTemp,"ExpertSysGlobal","GetMainDBName");
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
///将组态所得的连接形式转化为专家系统知识库中的连接形式
int ConvertConnType(int iConnType_)
{
	switch(iConnType_)
	{
	case 1:
		return GE_CONNTYPE_GEAR;
		break;
	case 2:
		return GE_CONNTYPE_ELAST;
	    break;
	case 4:
		return GE_CONNTYPE_STRAP;
		break;
	case 8:
        return GE_CONNTYPE_SUSPE;
		break;
	case 16:
		return GE_CONNTYPE_OTHERS;
		break;
	default:
		return GE_CONNTYPE_GEAR;
	}
}
//将组态所得的转子形式转化为专家系统知识库中的转子形式
int ConvertRotateType (int iRotateType_)
{
	switch(iRotateType_)
	{
	case 1:
		return GE_ROTATETYPE_METAL;
		break;
	case 2:
        return GE_ROTATETYPE_HOLLOW;
		break;
	case 3:
		return GE_ROTATETYPE_OTHERS;
		break;
	default:
		return GE_ROTATETYPE_METAL;
	}
}
//将组态所得的工作转速转化为专家系统知识库中的工作转速
int ConvertWorkRev(int iWorkRev_)
{
	switch(iWorkRev_)
	{
	case 1:
		return GE_WORKREV_BLOWONE;
		break;
	case 2:
		return GE_WORKREV_BETWEENTWO;
		break;
	case 3:
		return GE_WORKREV_UPTWO;
		break;
	default:
		return GE_WORKREV_BLOWONE;
	}
}
//将组态所得的驱动类型转化为专家系统知识库中的驱动类型
int ConvertDriverType(int iDriverType_)
{
	return iDriverType_;

	switch(iDriverType_)
	{
	case 1:
		return GE_DRIVERTYPE_MOTOR;
		break;
	case 2:
		return GE_DRIVERTYPE_MOTORGEAR;
		break;
	case 3:
		return GE_DRIVERTYPE_INDUSTRYTURB;
		break;
	case 4:
		return GE_DRIVERTYPE_OTHERS;
		break;
	default:
		return GE_DRIVERTYPE_MOTOR;
	}
}
//将组态所得的轴承类型转化为专家系统知识库中的轴承类型
int ConvertBearType(int iBearType_)
{
	switch(iBearType_)
	{
	case 1:
		return GE_BEARTYPE_TILT;
		break;
	case 2:
		return GE_BEARTYPE_SLIDE;
		break;
	case 3:
		return GE_BEARTYPE_ROLL;
		break;
	default:
		return GE_BEARTYPE_TILT;
	}
}
//将组态所得的密封形式转化为专家系统知识库中的密封形式
int ConvertSealType(int iSealType_)
{
	switch(iSealType_)
	{
	case 1:
		return GE_SEALTYPE_LABYRINTH;
		break;
	case 2:
		return GE_SEALTYPE_LOOP;
		break;
	case 4:
		return GE_SEALTYPE_OIL;
		break;
	case 8:
		return GE_SEALTYPE_OTHERS;
		break;
	default:
		return GE_SEALTYPE_LABYRINTH;
	}
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





