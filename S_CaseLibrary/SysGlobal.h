// SysGlobal.h
#include "StdAfx.h"
#include <fstream>
#include "../ServerStatus/IServerStatusExport.h"
extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;
#ifndef _SYSTEMSVR_SYSGLOBAL_H_
#define _SYSTEMSVR_SYSGLOBAL_H_


extern bool g_bInit;
///��ǰ�м���������ļ���
extern int g_iSvrLevel;

class CSetIpDetailInfo
{
public:
	CSetIpDetailInfo()
	{
		_sSetID="";
		_sParentSetID="";
		_sSetIP="";
		_sCompanyIP="";
		_iChildFlag=-1;
		_iSetPort=7001;
		_iCompanyPort=7001;
	}
	~CSetIpDetailInfo()
	{
	}
	int GetParentPort()
	{
		return _iCompanyPort;
	}
	string GetParentIP()
	{
		return _sCompanyIP;
	}
	int GetSetPort()
	{//��Ϊ�м���ķ�������IDΪ0ʱ�����Լ���IP������TM��־��Ӧ�ֶ�
		if(_iID==0)
			return _iTmPort;
		else
			return _iSetPort;
	}
    string GetSetIP()
	{
		if(_iID==0)
			return _sTmIP;	
		else
			return _sSetIP;
	}
	
	string GetSetID()
	{
		return _sSetID;
	}
    int GetID()
	{
		return _iID;
	}
    
	string GetParentSetID()
	{
		return _sParentSetID;
	}
    
	string GetTmIP()
	{
		return _sTmIP;
	}

	int GetTmPort()
	{
       return _iTmPort;
	}

	void SetParentIP(string sIP_)
	{
		_sCompanyIP=sIP_;
	}
	void SetParentPort(int iPort_)
	{
		_iCompanyPort=iPort_;
	}

    void ReadSetInfo(CCustomDB * pDB_)
	{
		_sSetIP=pDB_->Get(gc_cCenterMw_IP);
		_iSetPort=pDB_->GetLong(gc_cCenterMw_Port);
		_iID=pDB_->GetLong(gc_cID);
		_sParentSetID=pDB_->Get(gc_cParent_ID);
		_iChildFlag=pDB_->GetLong(gc_cChild_Flag);
		_sSetID=pDB_->Get(gc_cSetID);
	    _sTmIP=pDB_->Get(gc_cTm_IP);
		_iTmPort=pDB_->GetLong(gc_cTm_Port);
	}
private:
	string _sSetID;
	string _sParentSetID;
	int _iChildFlag;
	string _sCompanyIP;//�����ϼ�������IP��ַ
    int    _iCompanyPort;//�����ϼ��������˿�
	string _sSetIP;
	int _iSetPort;
	int _iID;
	string _sTmIP;//�������м��������ID��˵Ϊת��IP��ַ�����ڵ�ǰ�м��������ID��˵�����Լ��ķ�����IP��ַ
	int _iTmPort;   //�������м��������ID��˵Ϊת��Port�����ڵ�ǰ�м��������ID��˵�����Լ��ķ�����Port
    
};

typedef map<string,CSetIpDetailInfo> MAP_SETIPINFO;
extern MAP_SETIPINFO g_mapSetInfo;
///�豸��ϸ��Ϣ
int InitGlobalVar(CCustomDB *pDB_);

void QuitClearGlobalVar();
///�õ���־��־λ
int GetLogOutputFlag(CString &strLogPath_);
//�õ������ݿ���
void GetMainDBName();

///����֮�ú���
int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,CString & astrPath);

#endif//_SYSTEMSVR_SYSGLOBAL_H_