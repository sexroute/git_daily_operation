#include "StdAfx.h"

#include <iostream>
#include "MiddleWare.h"
#include <fstream>
using namespace std;

bool g_bInit=false;
int g_iSvrLevel=GE_SRV_TYPE_SET;
MAP_SETIPINFO g_mapSetInfo;

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

int InitGlobalVar(CCustomDB *pDB_)
{ 

	int nCount=0;
	g_mapSetInfo.clear();
	CSetIpDetailInfo setIpInfo;
	CString strInfo;
	int iLogLevel=GetLogOutputFlag(strInfo);
	CHZLogManage::StartLogTxt( strInfo, "KDMW", iLogLevel);
	GetMainDBName();
	//得到中间件服务器级别
	SwichDbName(pDB_,g_strMainDbName);
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
			g_mapSetInfo.insert(make_pair(sTmp,setIpInfo));
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
			  if (itMap2!=g_mapSetInfo.end())
			  {
				  itMap1->second.SetParentIP(itMap2->second.GetSetIP());
				  itMap1->second.SetParentPort(itMap2->second.GetSetPort());
			  }
		   }
	   }
	}
	nCount=g_mapSetInfo.size();
	RegistModuleComplete();

    return nCount;
}

void QuitClearGlobalVar()
{
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

