// SysGlobal.h
#include "StdAfx.h"
#include <fstream>
#include "../ServerStatus/IServerStatusExport.h"
extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;
#ifndef _SYSTEMSVR_SYSGLOBAL_H_
#define _SYSTEMSVR_SYSGLOBAL_H_


extern bool g_bInit;
///当前中间件服务器的级别
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
	{//因为中间件的服务器的ID为0时，他自己的IP就是有TM标志对应字段
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
	string _sCompanyIP;//它的上级服务器IP地址
    int    _iCompanyPort;//它的上级服务器端口
	string _sSetIP;
	int _iSetPort;
	int _iID;
	string _sTmIP;//对于子中间件服务器ID来说为转发IP地址；对于当前中间件服务器ID来说就是自己的服务器IP地址
	int _iTmPort;   //对于子中间件服务器ID来说为转发Port；对于当前中间件服务器ID来说就是自己的服务器Port
    
};

typedef map<string,CSetIpDetailInfo> MAP_SETIPINFO;
extern MAP_SETIPINFO g_mapSetInfo;
///设备详细信息
int InitGlobalVar(CCustomDB *pDB_);

void QuitClearGlobalVar();
///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_);
//得到主数据库名
void GetMainDBName();

///调试之用函数
int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,CString & astrPath);

#endif//_SYSTEMSVR_SYSGLOBAL_H_