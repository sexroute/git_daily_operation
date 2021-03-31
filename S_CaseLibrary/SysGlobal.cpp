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
	//�õ��м������������
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
	//��ѯ�пͻ�������ʱ�漰�����м��������
    list<int> lSvrID;
	int iDbID=0;
	lSvrID.clear();
	pDB_->Cmd("select distinct([%s]) from [%s] ",gc_cDB_ID,gc_cCompanyTable);
	while (pDB_->More())
	{
       iDbID=pDB_->GetLong(gc_cDB_ID);
	   lSvrID.push_back(iDbID);
	}
	//�õ������м��ID��Ϣ
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

//��ini�ļ��еõ����ݿ�������������ݿ���
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
	//�õ����ݿ�������������ݿ���
	GetPrivateProfileString("SERVER","SERVER1","�������,dse,DSE,DSE",sDb,2000,cAppLocation);
	char seps[]=", ";
	token = strtok( sDb, seps );
	for(int iloop=0; token != NULL;iloop++)
	{
		/* While there are tokens in "string" */
		if (iloop==2)
		{
			g_strMainDbName.Format("%s",token);//�õ�ȫ�ֵ������ݿ���
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
	//�õ��Ƿ��л����ݿ�ı�ʶ
	g_iDbType = GetPrivateProfileInt("DBINFO","DBTYPE",0x0000,cAppLocation);
	if (g_iDbType!=0 && g_iDbType!=1)
		CHZLogManage::Error("�������ݿ����������","SysGlobal","GetMainDBName");
	CString sTemp;sTemp.Format("�õ�ȫ�ֵ������ݿ��������ݿ����ͱ�ʶ:%s,%d",g_strMainDbName,g_iDbType);
	CHZLogManage::Debug(sTemp,"SysGlobal","GetMainDBName");
}

///�õ���־��־λ
int GetLogOutputFlag(CString &strLogPath_)
{
	char cAppLocation[500]={0};
	char sLogPath[1024]={0};
	GetCurrentDirectory(500,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	GetPrivateProfileString("SYSTEM","LOGPATH",".\\log",sLogPath,1024,cAppLocation);
	strLogPath_="";
	strLogPath_.Format("%s",sLogPath);
	//�õ��û���������
	return GetPrivateProfileInt("LOG","KDLOG",0x0040,cAppLocation);
}

//����֮�ú���
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

