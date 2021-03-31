// SysGlobal.h
#include "StdAfx.h"
#include <fstream>
#include <additions/CTimeOutReadWriteLock.h>
#include "../ServerStatus/IServerStatusExport.h"

extern LY::IServerStatus * g_pServerStatus ;
extern CCriticalSection * g_pServerStatusLock;
#ifndef _SYSTEMSVR_SYSGLOBAL_H_
#define _SYSTEMSVR_SYSGLOBAL_H_
/*!是否开启停车仅保存月表策略，默认为0,即不开启*/
extern int G_TEMP_ONLINE;
extern BOOL IsTempMode();
extern bool g_bInit;
///当前中间件服务器的级别
extern int g_iSvrLevel;

class CSetIpDetailInfo
{
public:
	CSetIpDetailInfo()
	{
		_sSetID="";
		_sSetDisplay = "";
		_sParentSetID="";
		_sSetIP="";
		_sCompanyIP="";
		_iChildFlag=-1;
		_iSetPort=7001;
		_iCompanyPort=7001;
		_iSetSortNO = 0;
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

	string GetSetDisplay()
	{
		return _sSetDisplay;
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

	int GetSetSortNO()
	{
		return _iSetSortNO;
	}

    void ReadSetInfo(CCustomDB * pDB_)
	{
		_sSetIP=pDB_->Get(gc_cCenterMw_IP);
		_sSetDisplay = pDB_->Get(gc_cDisplay);
		_iSetPort=pDB_->GetLong(gc_cCenterMw_Port);
		_iID=pDB_->GetLong(gc_cID);
		_sParentSetID=pDB_->Get(gc_cParent_ID);
		_iChildFlag=pDB_->GetLong(gc_cChild_Flag);
		_sSetID=pDB_->Get(gc_cSetID);
	    _sTmIP=pDB_->Get(gc_cTm_IP);
		_iTmPort=pDB_->GetLong(gc_cTm_Port);
		_iSetSortNO = pDB_->GetLong(gc_cSortNO);
	}
private:
	string _sSetID;
	string _sSetDisplay;
	string _sParentSetID;
	int _iChildFlag;
	string _sCompanyIP;//它的上级服务器IP地址
    int    _iCompanyPort;//它的上级服务器端口
	string _sSetIP;
	int _iSetPort;
	int _iID;
	string _sTmIP;//对于子中间件服务器ID来说为转发IP地址；对于当前中间件服务器ID来说就是自己的服务器IP地址
	int _iTmPort;   //对于子中间件服务器ID来说为转发Port；对于当前中间件服务器ID来说就是自己的服务器Port
	int _iSetSortNO;
    
};

typedef map<string,CSetIpDetailInfo> MAP_SETIPINFO;
extern MAP_SETIPINFO g_mapSetInfo;

///25010接口缓存
typedef list<S_PlantInfo> FactoryDataType;
typedef list<S_PlantInfoEx> FactoryDataTypeEx;
extern std::map<CString,FactoryDataTypeEx> g_FactoryData;

///25025接口缓存
extern FactoryDataTypeEx g_ListPlantInfoEx;

///25028接口缓存 
extern int g_IServerLevel;


typedef struct _SET_INFO
{
	CString m_strSetID;
	CString m_strSetIP;
	long	m_nSetPort;
	int		m_nLevel;
	_SET_INFO()
	{
		m_nSetPort = 0;
		m_nLevel = 0;
	}
}SET_INFO , * LP_SET_INFO;

typedef std::vector<SET_INFO> SET_DATA_TYPE;

extern SET_DATA_TYPE g_SET_DATA;


#define UM_ERROR_INVALID_USERNAME    (-1)
#define UM_ERROR_INVALID_PASSWORD    (-2)
#define UM_ERROR_CREATE_USER         (-3)
#define UM_ERROR_DELETE_USER         (-4)
#define UM_ERROR_CHANGE_PASSWORD     (-5)
#define UM_ERROR_ADD_PLANT           (-6)
#define UM_ERROR_USER_NOT_EXIST      (-7)
#define UM_ERROR_USER_ALREADY_EXIST  (-8)
#define UM_ERROR_SQL_CMD             (-9)
#define UM_ERROR_PLANT_LIST_INVALID  (-10)
#define UM_ERROR_ACTION_TYPE_INVALID (-11)
#define UM_SUCCESS                   (1)
#define UM_FAILURE                   (0)

typedef WORD Id_t;
typedef std::set<Id_t>  IdSet_t;
typedef std::vector<Id_t> IdArray_t;

IdSet_t UserTable_GetMachine( CSimADO * thiz, int anMachineNum = -1 );

IdSet_t UserTable_GetMachineEx( IDBInterFace * thiz, int anMachineNum = -1 );

bool UserTable_SetMachine( CSimADO * thiz, int user_id, const IdSet_t &update_machine_ids);
bool UserTable_SetMachine( IDBInterFace * thiz, int user_id, const IdSet_t &update_machine_ids);

typedef struct _ST_DBID_COMPANY_GROUP
{
	int     _nDbID;
	CString _sCompnay;
	CString _sGroup;
} ST_DBID_COMPANY_GROUP;

typedef ST_DBID_COMPANY_GROUP CompanyInfo_t;
typedef std::list<CompanyInfo_t>  CompanyInfoList_t;

typedef struct _ST_DB_USER_INFO
{
	int     UserId;
	int     UserLevel;
	CString UserName;
	CString Group;
	CString Company;
	CString Factory;

	/*
	InitMachineNum 和 InitMachineIDs 仅在初始化时加载数据库初始值, 当接口25042接口不会动态更改,
	请使用UserManager接口取得用户关联的设备ID
	*/
	int     InitMachineNum;
	IdSet_t InitMachineIDs;

	_ST_DB_USER_INFO()
	{
		this->UserId     = -1;
		this->UserLevel  = -1;
		this->UserName   = "";
		this->Group      = "";
		this->Company    = "";
		this->Factory    = "";
		this->InitMachineNum = -1;
		this->InitMachineIDs = IdSet_t();
	}

	_ST_DB_USER_INFO(CSimADO * pDB)
	{
		ASSERT(pDB);

		this->UserId     = pDB->GetLong(gc_cID);

		this->UserLevel  = pDB->GetLong(gc_cUserLevel);

		this->UserName   = pDB->Get(gc_cUserName);
		this->UserName.Trim();

		this->Group      = pDB->Get(gc_cGroup);
		this->Group.Trim();

		this->Company    = pDB->Get(gc_cCompany);
		this->Company.Trim();

		this->Factory    = pDB->Get(gc_cFactory_Name);
		this->Factory.Trim();

		this->InitMachineNum = pDB->GetLong(gc_cNum);
		this->InitMachineIDs = UserTable_GetMachine(pDB);

		if (this->InitMachineNum >= 0)
		{
			if (this->InitMachineNum !=  this->InitMachineIDs.size())
			{
				BHLOG_ERROR(STR("用户表配置错误: 用户%s的关联设备数目和设备列表不一致", this->UserName));
			}
		}
	}

} ST_DB_USER_INFO;

typedef ST_DB_USER_INFO UserInfo_t;
typedef std::list<UserInfo_t> UserInfoList_t;

CString UserInfoList_ToString( const UserInfoList_t& self, const TCHAR * sep = _T(",") );

typedef struct _ST_DB_PLANT_INFO
{
	int     MachineId;
	CString CompanyName;
	CString FactoryName;
	CString SetID;
	CString PlantID;
	CString PlantErpCode;

	CString GroupName;
	CString GroupDisplayName;
	CString CompanyDisplayName;
	CString FactoryDisplayName;
	CString PlantNameDisplay;
	CString PlantNo;
	CString CompanyAlias;

	int     GraphType;
	int     MachineType;
	int     DataSource;
	int     Db_Id;
	int     GroupSortNO;
	int     CompanySortNO;
	int     FactorySortNO;
	int     PlantSortNO;

	_ST_DB_PLANT_INFO()
	{
		this->MachineId = -1;
		this->CompanyName = "";
		this->FactoryName = "";
		this->SetID   = "";
		this->PlantID = "";
		this->PlantNameDisplay = "";
		this->PlantErpCode = "";
		this->PlantNo = "";
		this->CompanyAlias = "";

		this->GroupName = "";
		this->GraphType = 0;
		this->MachineType = 0;
		this->DataSource  = 0;
		this->Db_Id       = 0;
		this->GroupSortNO = 0;
		this->CompanySortNO = 0;
		this->FactorySortNO = 0;
		this->PlantSortNO = 0;
	}

	_ST_DB_PLANT_INFO(CSimADO * pDB)
	{
		ASSERT(pDB);
		
		this->MachineId = pDB->GetLong(gc_cID);
		ASSERT(this->MachineId > 0);

		this->CompanyName = pDB->Get(gc_cCompany);
		this->CompanyName.Trim();

		this->FactoryName = pDB->Get(gc_cFactory_Name);
		this->PlantID.Trim();

		this->SetID   = pDB->Get(gc_cSetID);
		this->SetID.Trim();

		this->PlantID = pDB->Get(gc_cPlantID);
		this->PlantID.Trim();

		this->PlantNameDisplay = pDB->Get(gc_cDisplay);
		this->PlantNameDisplay.Trim();

		this->PlantErpCode = pDB->Get(gc_cErpCode);
		this->PlantErpCode.Trim();

		this->GraphType = pDB->GetLong(gc_cGraphType);

		this->MachineType = pDB->GetLong(gc_cMachine_Type);

		this->DataSource  = pDB->GetLong(gc_cData_Source);
		
		this->PlantSortNO = pDB->GetLong(gc_cSortNO);

		this->PlantNo     = pDB->Get(gc_cPlantNo);

		this->CompanyAlias = pDB->Get(gc_cCompanyAlias);
	}

} ST_DB_PLANT_INFO;

typedef ST_DB_PLANT_INFO PlantInfo_t;
typedef std::list<ST_DB_PLANT_INFO> PlantInfoList_t;

IdSet_t IdSet_FromString(CString csv, const TCHAR * sep = _T(","));
CString IdSet_ToString(const IdSet_t& self, const TCHAR * sep = _T(","));

IdSet_t IdSet_Intersection(const IdSet_t& first, const IdSet_t& second);
IdSet_t IdSet_Difference(const IdSet_t& first, const IdSet_t& second);
IdSet_t IdSet_Union(const IdSet_t& first, const IdSet_t& second);

typedef std::vector<CString> NameList_t;
typedef int (*PlantIteratorFunc_t)(const PlantInfo_t& aPlantInfo, const UserInfoList_t& aUserInfoList, void * aParam);


class UserManager
{
public:
	static int CreateUser( const UserInfo_t& aUserInfo, const IdSet_t& aMachineIdList );
	static int DeleteUser(const CString& asUserName);

	static bool IsUserExist(const CString& asUserName);
	static bool GetUserInfo( const CString& asUserName, UserInfo_t& outUserInfo );

	static bool IsPlantExist( const CString& asCompany, const CString& asFactoryName, const CString& asPlantName );
	static bool GetPlantInfo(const CString& asCompany, const CString& asFactoryName, const CString& asPlantName, PlantInfo_t& outPlantInfo);

	static IdSet_t GetTotalMachineIDList(void);
	static bool GetMachineIDList( int anUserID, IdSet_t& outMachineIDList );

	static bool Bind( int anUserID, const IdSet_t& aMachineIdList );
	static bool Unbind( int anUserID, const IdSet_t& aMachineIdList );

	static bool SetUserLevel(int user_id, int user_level);

	static int ForEachPlant( const CString& asCompany, const CString& asFactoryName, const CString& asPlantName,
		PlantIteratorFunc_t aHandler, void * aExtra );
	static bool EnumeratePlant(PlantInfo_t& outPlantInfo);

	static bool GetSpecMachineIDPlantInfo(int anMachineId, PlantInfo_t& outPlantInfo);

	static int Initialize(CSimADO *pDB);

public:
	static int UserDump(ICommInterFace* pFixComm);
	static int MachineDump(ICommInterFace* pFixComm);

private:
	UserManager();
};

namespace DatasourceHelper
{
	int CompareVersionNumber(const CString &lft, const CString &rgt, int max=-1);

	bool IsVersionDeprecated(const CString& aVersion);

	int GetSpecVersionPlantDatasource(int aDatasource, bool aDeprecated);
	int GetSpecVersionPlantDatasource(int aDatasource, const CString& aVersion);
}


///初始化分厂用户对应表
int InitPlantUserMap(CCustomDB *_pDB);

///设备详细信息
int InitGlobalVar(CCustomDB *pDB_);

void QuitClearGlobalVar();
///得到日志标志位
int GetLogOutputFlag(CString &strLogPath_);
//得到主数据库名
void GetMainDBName();
//检测当前进程打开的句柄，如果增长后长时间存在，且类型为TCP则关闭该句柄
void CheckCurrentHandleThread(LPVOID apVoid);

///调试之用函数
int writefile(float * ag_fwave, int ag_num,int);
int writefile(double * ag_fwave, int ag_num,int);
int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int);


#endif//_SYSTEMSVR_SYSGLOBAL_H_