#pragma pack (1)
#include "stdafx.h"
#include "process.h"
#include "Implement.h"
#include "SysGlobal.h"
#include <MiddleWare.h>
#include <additions/DebugHelper.h>


void DSEDecrypt(CString & m_sPwd, CString & m_sDePwd)
{
	char sPwd[1024]={0};
	char sDePwd[1024]={0};
	strncpy(sPwd,m_sPwd,sizeof(sPwd)-1);
	char key[8]="dse";
	des3_ecb_decrypt(key,sPwd,sDePwd);
	m_sDePwd.Format("%s",sDePwd);
}

void DSEEncrypt(CString & m_sPwd, CString & m_sEnPwd)
{
	/*	char sPwd[1024];
	char sEnPwd[1024]={0};
	strcpy(sPwd,m_sPwd);
	char key[8]="dse";
	des3_ecb_encrypt(key,sPwd,sEnPwd);
	m_sEnPwd.Format("%s",sEnPwd);*/
	m_sEnPwd=m_sPwd;
}


/**通过公司名、分厂名、机组号得到可以做启停车的测点和其对应的额定转速
*输入参数：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*返回值的字段码：
*             测点号    FIELD_SYSTEM_CHANN
*             额定转速  FIELD_SERVICE_REV
*define    SERVICE_SYSTEM_GETPLANTSUDCHANN           25000
*/
int Service_SysGetPlantSudChann(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	//	string sPlantNo = CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	list<string> lPlant;
	list<string>::iterator it;
	typedef pair<int,int> PAIRINT;
	list<PAIRINT> lRev;
	list<PAIRINT>::iterator itRev;
	int irev,irev2;
	int nCount=-1;
	//得到指定机组的两个额定转速
	pDB->Cmd("SELECT %s,%s,%s FROM [%s] ",gc_cRev,gc_cRev2,gc_cPlantNo,gc_cMachineTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());
	if (!(sPlant.empty()))
		pDB->Cmd(" AND [%s]='%s' ",gc_cPlantID,sPlant.c_str());
	while(pDB->More())
	{
		irev=pDB->GetLong(gc_cRev);
		irev2=pDB->GetLong(gc_cRev2);
		lRev.push_back(make_pair(irev,irev2));
		lPlant.push_back(pDB->Get(gc_cPlantNo));
	}
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	itRev=lRev.begin();
	for (it=lPlant.begin();it!=lPlant.end();++it)
	{
		sPlant=(*it);
		pDB->Cmd("SELECT %s ,%s FROM [%s] ",gc_cChannID,gc_cRevType,gc_cChannTable);
		pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND %s='%s' AND %s=%d AND %s =%d ORDER BY ID",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,sPlant.c_str(),gc_cChannType,GE_VIBCHANN,gc_cSensorType,GE_SENSORTYPE_VOTEX);
		int iType=1;
		while(pDB->More())
		{
			pFixComm->SetItem(FIELD_SYSTEM_PLANT,sPlant.c_str());
			pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannID).c_str());
			iType=pDB->GetLong(gc_cRevType);
			if(iType==2)
				pFixComm->SetItem(FIELD_SERVICE_REV,itRev->second);
			else
				pFixComm->SetItem(FIELD_SERVICE_REV,itRev->first);
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			++nCount;
		}
		++itRev;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定分厂下所有设备位号，通过循环依次得到所有设备位号
*输入参数：公司名      FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          装置名ID   FIELD_SYSTEM_SET
*返回值的字段码：
*          设备号       FIELD_SYSTEM_PLANT
*          概貌图类型   FIELD_SYSTEM_PLANT_GRAPHTYPE
*          装置名ID     FIELD_SYSTEM_SET
*          设备类型     FIELD_SYSTEM_TYPE
*          数据源       FIELD_SYSTEM_DATASOURCE
*define     SERVICE_SYSTEM_GETPLANTOFSUBCORP          25001
*/
int Service_SysGetPlantOfSubCorp(ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sSet=pFixComm->Get(FIELD_SYSTEM_SET);

	CString lstrVersion = pFixComm->GetItem(FIELD_SYSTEM_VERSION);

	bool lbVersionDeprecated = DatasourceHelper::IsVersionDeprecated(lstrVersion);

	int nCount=0;

	pDB->Cmd(" SELECT * FROM [%s]",gc_cMachineTable);
	pDB->Cmd("  WHERE [%s]='%s' AND [%s]='%s' ",gc_cFactory_Name,sFactory.c_str(),gc_cCompany,sCompany.c_str());
	//	if (iID!=0)
	//		pDB->Cmd(" AND [%s]='%s' ",gc_cSetID,sSet.c_str());

	pDB->Cmd(" ORDER BY %s DESC",gc_cPlantID);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,pDB->GetLong("GRAPH_TYPE"));
		pFixComm->SetItem(FIELD_SYSTEM_TYPE, pDB->GetLong(gc_cMachine_Type));

		int lnPlantDatasource = DatasourceHelper::GetSpecVersionPlantDatasource(pDB->GetLong(gc_cData_Source), lbVersionDeprecated);
		pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE, lnPlantDatasource);

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/**得到指定公司名、分厂名、机组位号的所有测点名和对应的测点类型，通过循环依次得到
*输入参数：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*返回值的字段码：
*          测点号      FIELD_SYSTEM_CHANN
*          测点类型    FIELD_SYSTEM_CHANN_TYPE
*          传感器类型  FIELD_SYSTEM_SENSOR_TYPE
*define     SERVICE_SYSTEM_GETALLCHANN                25002
*/
int Service_SysGetAllChann(ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	int nCount=-1;
	CImplement impl;
	string sPlantNo=impl.GetPlantNo(sCompany, sFactory, sPlant, pDB);
	pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cChannID,gc_cChannType,gc_cSensorType,gc_cChannTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",gc_cPlantNo,sPlantNo.c_str(),gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,pDB->GetLong(gc_cChannType));
		pFixComm->SetItem(FIELD_SYSTEM_SENSOR_TYPE,pDB->GetLong(gc_cSensorType));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


BOOL GetBearingInfo(CBearingParamInfo & arefBearingInfo,CString &astrChannID,CString &astrCompany,CString &astrFactory,CString & astrSetID,CString & astrPlantNo, int anLocationIdx,IDBInterFace * pDB_)
{
	//得到locationsInfo
	SwitchDbNameEx(pDB_,g_strMainDbName);

	CChannLocations locationsInfo;
	if (anLocationIdx>-1)
	{
		pDB_->getPDB()->Cmd("SELECT * from [%s] ",gc_cLocation_Table);
		pDB_->getPDB()->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]=%d",gc_cCompany,astrCompany,gc_cFactory_Name,astrFactory,gc_cSetID,astrSetID,gc_cPlantNo,astrPlantNo,gc_cLocation_IDX,anLocationIdx);
		if (pDB_->getPDB()->More())
		{
			if (locationsInfo.GetChannLocations(pDB_->getPDB()))
			{
			}else
			{
				return FALSE;
			}

		}else
		{
			return FALSE;
		}
	}else
	{
		return FALSE;
	}
	//获取轴承参数信息
	CBearingParamInfo & bearParamInfro = arefBearingInfo;
	int iBearingTable=locationsInfo._iBearingTblType;

	CString lstrSqlGetBear;
	CString lstrTemp;

	if(iBearingTable==GE_SYSTEM_DEFAULT_BEARING_TABLE)
	{
		lstrTemp.Format("SELECT * FROM [%s] ",gc_cBearing_Table);
		lstrSqlGetBear += lstrTemp;

		lstrTemp.Format(" WHERE [%s]=%d",gc_cBearing_IDX,locationsInfo._iBearingIDX);
		lstrSqlGetBear += lstrTemp;

		if (bearParamInfro.GetBearingParamInfo(lstrSqlGetBear))
		{	
			return TRUE;

		}else
		{
			return FALSE;
		}

	}
	else
	{
		lstrTemp.Format("SELECT * FROM [%s] ",gc_cCustom_Bearing_Table);
		lstrSqlGetBear += lstrTemp;

		lstrTemp.Format(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,astrCompany,gc_cFactory_Name,astrFactory,gc_cSetID,astrSetID,gc_cPlantNo,astrPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);
		lstrSqlGetBear += lstrTemp;

		pDB_->Cmd("SELECT * FROM [%s]",gc_cCustom_Bearing_Table);
		pDB_->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]='%s' AND [%s]= %d ",gc_cCompany,astrCompany,gc_cFactory_Name,astrFactory,gc_cSetID,astrSetID,gc_cPlantNo,astrPlantNo,gc_cBearing_IDX,locationsInfo._iBearingIDX);

		if (pDB_->More())
		{
			if (bearParamInfro._GetBearingParamInfo(pDB_->getPDB()))
			{
				return TRUE;
			}else
			{
				return FALSE;
			}

		}else
		{
			return FALSE;
		}
	}
}

static BOOL GetSectionInfo(CChannSections & arefSectionInfo,
						   CString & astrCompany,
						   CString & astrFactory,
						   CString & astrSetID,
						   CString & astrPlantNo, 
						   int anSectionIdx,
						   IDBInterFace * pDB_)
{

	CCustomDB * lpDB = pDB_->getPDB();

	if (NULL == lpDB)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	lpDB->Cmd("SELECT * from [%s] ",gc_cSection_Table);
	lpDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]=%d",
		gc_cCompany,astrCompany,
		gc_cPlantNo,astrPlantNo,
		gc_cSection_IDX,
		anSectionIdx);
	if (lpDB->More())
	{
		arefSectionInfo.GetChannSections(lpDB);

		return TRUE;
	}

	return FALSE;
}

static BOOL GetLocationInfo(CChannLocations & arefLocationInfo,
							CString &astrCompany,
							CString &astrFactory,
							CString & astrSetID,
							CString & astrPlantNo, 
							int anLocationIdx,
							IDBInterFace * pDB_)
{

	CCustomDB * lpDB = pDB_->getPDB();

	if (NULL == lpDB)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	lpDB->Cmd("SELECT * from [%s] ", gc_cLocation_Table);
	lpDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]=%d",
		gc_cCompany,astrCompany,
		gc_cPlantNo,astrPlantNo,
		gc_cLocation_IDX,
		anLocationIdx);
	if (lpDB->More())
	{
		arefLocationInfo.GetChannLocations(lpDB);

		return TRUE;
	}

	return FALSE;
}

/**通过用户名和密码得到用户级别、集团名、公司名，如果用户级别是集团级，就需要循环得到各个公司
*输入参数：用户名   FIELD_SYSTEM_USEID
*          密码     FIELD_SYSTEM_PASSWORD
*返回值的字段码：
*          用户级别 FIELD_SYSTEM_USER_LEVEL
*          集团名   FIELD_SYSTEM_GROUP
*          公司名   FIELD_SYSTEM_COMPANY
*          分厂名   FIELD_SYSTEM_FACTORY
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_GETCOMPANYTOFUSER             25004
*/
BOOL IsAllowAnonymous()
{
		try
		{
			char cAppLocation[5000]={0};
		
			GetCurrentDirectory(5000,cAppLocation);
		
			strcat(cAppLocation,"\\DseSvr.ini");
		
			BOOL lbSkipAlarm = ::GetPrivateProfileInt("SYSTEM","ALLOW_ANONYMOUSE",TRUE,cAppLocation);

			CString lstrData;
			lstrData.Format(_T("%d"),lbSkipAlarm);

			::WritePrivateProfileString("SYSTEM","ALLOW_ANONYMOUSE",lstrData.GetBuffer(0),cAppLocation);
		
			return lbSkipAlarm;
		
		}catch(...)
		{
			return FALSE;
		}
}


int Service_SysGetCompanyOfUser(ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	SwitchDbNameEx(pDB,g_strMainDbName);

	CString strUser  = pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strDePwd = pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);
	if ((strUser.CompareNoCase("bhxz") == 0))
	{

	}else
	{
		strUser.Replace("'","''");
	}

	int lbJustLogin = pFixComm->GetLong(FIELD_SYSTEM_DATA);
	int lnFeedMark  = pFixComm->GetLong(FIELD_SYSTEM_MODIFY_USER); //标识需要取得全部公司分厂

	strUser.Trim();
	strDePwd.Trim();

	CString strCompany;
	CString strGroup;

	typedef pair<ST_DBID_COMPANY_GROUP, int> PairCompany;

	list<PairCompany> vCompany;
	list<PairCompany>::iterator it;

	ST_DBID_COMPANY_GROUP loDbIDCompanyGroupInfo;

	CString lsSQL;

	int iDbID = 0;

	int lnPlantCount   = 0;

	BOOL lbIsAllowAnonymous = IsAllowAnonymous();

	if (strUser.IsEmpty() && strDePwd.IsEmpty()) //匿名登陆：用户名和密码为空
	{
		if (lbIsAllowAnonymous || lnFeedMark)
		{
			pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ",gc_cGroup,gc_cCompany,gc_cDB_ID,gc_cCompanyTable);
			while (pDB->More())
			{
				loDbIDCompanyGroupInfo._nDbID    = pDB->GetLong(gc_cDB_ID);
				loDbIDCompanyGroupInfo._sCompnay = pDB->Get(gc_cCompany).c_str();
				loDbIDCompanyGroupInfo._sGroup   = pDB->Get(gc_cGroup).c_str();

				loDbIDCompanyGroupInfo._sCompnay.Trim();
				loDbIDCompanyGroupInfo._sGroup.Trim();

				if (loDbIDCompanyGroupInfo._sGroup.IsEmpty() || loDbIDCompanyGroupInfo._sCompnay.IsEmpty())
				{
					ASSERT(FALSE);
					continue;
				}

				vCompany.push_back(make_pair(loDbIDCompanyGroupInfo, loDbIDCompanyGroupInfo._nDbID));
			}

			//依次获取所有公司的分厂
			pFixComm->CreateAnswerMul(pFixComm->GetFunc());
			for (it=vCompany.begin();it!=vCompany.end();++it)
			{			
				ST_DBID_COMPANY_GROUP * lpDbIDCompanyGroupInfo = &(it->first);
				if (NULL == lpDbIDCompanyGroupInfo)
				{
					ASSERT(FALSE);
					break;
				}

				iDbID = it->second;

				if (iDbID>0) {
					pDB->Cmd("SELECT distinct [%s] from [%s_%d]",gc_cFactory_Name,gc_cMachineTable,iDbID);
				}
				else {
					pDB->Cmd("SELECT distinct [%s] from [%s]",gc_cFactory_Name,gc_cMachineTable);
				}

				lsSQL.Append(pDB->GetSql());

				pDB->Cmd(" WHERE %s='%s'", gc_cCompany, lpDbIDCompanyGroupInfo->_sCompnay);
				while(pDB->More())
				{
					lnPlantCount++;

					pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL, 1);
					pFixComm->SetItem(FIELD_SYSTEM_GROUP,   lpDbIDCompanyGroupInfo->_sGroup);
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY, lpDbIDCompanyGroupInfo->_sCompnay);
					pFixComm->SetItem(FIELD_SYSTEM_FACTORY, pDB->Get(gc_cFactory_Name).c_str());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
					if (lbJustLogin)
					{
						pFixComm->CreateEndMak();
						pFixComm->Write(pFixComm->GetAci());
						return 1;
					}
				}
			}

			if (lnPlantCount < 0)
			{
				CString sLog;
				sLog.Format("匿名登录失败! SQL=%s", lsSQL);
				CString sLoc;
				sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
				CHZLogManage::Error(sLog, sLoc);
				zdlTraceLine(sLog + sLoc);
			}
		}

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return lnPlantCount;
	}
	else
	{
		/* 用户名不区分大小写, 密码区分大小写 */
		pDB->Cmd("SELECT [%s],[%s],[%s],[%s],[%s],%s,%s FROM [%s]",
				  gc_cUserLevel,
				  gc_cPassWord,
				  gc_cGroup,
				  gc_cCompany,
				  gc_cFactory_Name,
				  SimAdo_Expression_ISNULL(gc_cNum, 0),
				  gc_cMachines,
				  gc_cUserTable);
		pDB->Cmd("  WHERE USERNAME='%s'",strUser);

		if (!pDB->More())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -1);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "用户不存在!");
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		CString lsDBPassword = pDB->Get(gc_cPassWord).c_str();
		lsDBPassword.Trim();

		if (lsDBPassword != strDePwd)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "密码不正确!");
			pFixComm->Write(pFixComm->GetAci());

			return -2;
		}

		int iUserLevel     = pDB->GetLong(gc_cUserLevel);

		CString strCompany = pDB->Get(gc_cCompany).c_str();
		CString strGroup   = pDB->Get(gc_cGroup).c_str();
		CString strFactory = pDB->Get(gc_cFactory_Name).c_str();

		strCompany.Trim();
		strGroup.Trim();
		strFactory.Trim();

		int iMachineNum = pDB->GetLong(gc_cNum);

		IdSet_t loMachineIDs;

		if (iMachineNum > 0)
		{
			loMachineIDs = UserTable_GetMachineEx(pDB, iMachineNum);
		}



		IDBInterFace * lpSubDb = pDB;

		lsSQL.Format("SELECT * FROM [%s] ",gc_cCompanyTable);

		switch(iUserLevel)
		{
		case GE_LOCAL_OPERATOR:
		case GE_LOCAL_ENGINEER:
		case GE_LOCAL_ADMIN:
		case GE_WHOLE_OPERATOR:
		case GE_WHOLE_ENGINEER:
		case GE_WHOLE_ADMIN:
		case GE_WHOLE_EXPERT:
			if (!strGroup.IsEmpty() && !strCompany.IsEmpty())
			{
				lsSQL.AppendFormat(" where [%s]='%s' AND [%s]='%s'",
					gc_cCompany,strCompany,
					gc_cGroup,strGroup);
			}
			break;
		case GE_GROUP_OPERATOR:
		case GE_GROUP_ENGINEER:
		case GE_GROUP_ADMIN:
		case GE_GROUP_EXPERT:
			if (!strGroup.IsEmpty())
			{
				lsSQL.AppendFormat(" WHERE [%s]='%s' ",
					gc_cGroup,strGroup);
			}
		case GE_EXPERT:
		case GE_ADMIN:
			{
				if (0 == iMachineNum)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s未关联任何设备", strUser);

					pFixComm->CreateAnswer(pFixComm->GetFunc());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog);
					pFixComm->Write(pFixComm->GetAci());

					CString sLoc;
					sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
					CHZLogManage::Error(sLog, sLoc);
					zdlTraceLine(sLog + sLoc);

					return 0;

				}

				if (iMachineNum > 0 && loMachineIDs.size() < 1)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s关联设备不正确", strUser);

					pFixComm->CreateAnswer(pFixComm->GetFunc());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog);
					pFixComm->Write(pFixComm->GetAci());

					CString sLoc;
					sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
					CHZLogManage::Error(sLog, sLoc);
					zdlTraceLine(sLog + sLoc);

					return 0;
				}

				typedef std::map<std::string, std::string> Company_Group_Map_t;

				Company_Group_Map_t loCompanyGroupMap;

				lpSubDb->Cmd("SELECT [%s],[%s] FROM %s", gc_cCompany, gc_cGroup, gc_cCompanyTable);
				while(lpSubDb->More())
				{
					loCompanyGroupMap.insert(std::make_pair(lpSubDb->Get(gc_cCompany), lpSubDb->Get(gc_cGroup)));
				}

				if (iMachineNum < 0)
				{
					lpSubDb->Cmd("SELECT distinct [%s],[%s] from [%s]",gc_cCompany, gc_cFactory_Name,gc_cMachineTable);
				}
				else if (iMachineNum > 0)
				{
					lpSubDb->Cmd("SELECT distinct [%s],[%s] from [%s]",gc_cCompany, gc_cFactory_Name,gc_cMachineTable);
					lpSubDb->Cmd(" WHERE [%s] in (%s) ", gc_cID, IdSet_ToString(loMachineIDs));
				}
		
				lsSQL.Append(lpSubDb->GetSql());

				lnPlantCount = 0;

				pFixComm->CreateAnswerMul(pFixComm->GetFunc());

				while(lpSubDb->More())
				{
					lnPlantCount++;

					std::string sRetCompany = lpSubDb->Get(gc_cCompany);
					std::string sRetFactory = lpSubDb->Get(gc_cFactory_Name);


					std::string sRetGroup;

					Company_Group_Map_t::iterator it = loCompanyGroupMap.find(sRetCompany);

					if (it != loCompanyGroupMap.end())
					{
						sRetGroup = it->second;
					}
					else
					{
						ASSERT(FALSE);
						sRetGroup = "";
					}

					pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,iUserLevel);
					pFixComm->SetItem(FIELD_SYSTEM_GROUP,   sRetGroup.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY, sRetCompany.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_FACTORY, sRetFactory.c_str());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();

					if (lbJustLogin)
					{
						pFixComm->CreateEndMak();
						pFixComm->Write(pFixComm->GetAci());
						return 1;
					}
				}

				if (lnPlantCount < 1)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s关联设备不存在", strUser);

					pFixComm->SetItem(FIELD_SYSTEM_STATUS, -4);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();

					sLog.AppendFormat("SQL=%s", lsSQL);

					CString sLoc;
					sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
					CHZLogManage::Error(sLog, sLoc);
					zdlTraceLine(sLog + sLoc);

				} //if (lnPlantCount < 1)

				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());

				return lnPlantCount;
			}

			break;

		default:
			break;
		}

		lpSubDb->Cmd(lsSQL);

		while (lpSubDb->More())
		{
			loDbIDCompanyGroupInfo._nDbID    = lpSubDb->GetLong(gc_cDB_ID);

			loDbIDCompanyGroupInfo._sCompnay = lpSubDb->Get(gc_cCompany).c_str();
			loDbIDCompanyGroupInfo._sGroup   = lpSubDb->Get(gc_cGroup).c_str();

			iDbID = loDbIDCompanyGroupInfo._nDbID;

			vCompany.push_back(make_pair(loDbIDCompanyGroupInfo, iDbID));
		}

		if (vCompany.size() < 1)
		{
			CString sLog;

			if (iUserLevel >= GE_GROUP_OPERATOR)
			{
				sLog.Format("登录失败: 用户%s所属的(集团:%s)下未设置公司!", 
					strUser, strGroup);
			}
			else
			{
				sLog.Format("登录失败: 用户%s所属的(集团:%s|公司:%s)不存在!", 
					strUser, strGroup, strCompany);
			}

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -3);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
			pFixComm->Write(pFixComm->GetAci());

			sLog.AppendFormat("SQL=%s", lsSQL);

			CString sLoc;
			sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
			CHZLogManage::Error(sLog, sLoc);
			zdlTraceLine(sLog + sLoc);

			return -3;
		}

		lnPlantCount   = 0;	

		pFixComm->CreateAnswerMul(pFixComm->GetFunc());

		for (it=vCompany.begin();it!=vCompany.end();++it)
		{
			lsSQL = "";

			ST_DBID_COMPANY_GROUP * lpDbIDCompanyGroupInfo = &(it->first);

			iDbID = it->second;

			if (iDbID>0)
				lpSubDb->Cmd("SELECT distinct [%s] from [%s_%d]",gc_cFactory_Name,gc_cMachineTable,iDbID);
			else
				lpSubDb->Cmd("SELECT distinct [%s] from [%s]",gc_cFactory_Name,gc_cMachineTable);

			lpSubDb->Cmd(" WHERE [%s]='%s' ",gc_cCompany, lpDbIDCompanyGroupInfo->_sCompnay);

			if ((iUserLevel <= GE_LOCAL_ADMIN) && !strFactory.IsEmpty()) {
				lpSubDb->Cmd(" and [%s]='%s'",gc_cFactory_Name, strFactory);
			}

			lsSQL.Append(lpSubDb->GetSql());

			while(lpSubDb->More())
			{
				lnPlantCount++;

				pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,iUserLevel);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP,   lpDbIDCompanyGroupInfo->_sGroup);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY, lpDbIDCompanyGroupInfo->_sCompnay);
				pFixComm->SetItem(FIELD_SYSTEM_FACTORY, lpSubDb->Get(gc_cFactory_Name).c_str());
				pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();

				if (lbJustLogin)
				{
					pFixComm->CreateEndMak();
					pFixComm->Write(pFixComm->GetAci());
					return 1;
				}
			}
		} //for (it=vCompany.begin();it!=vCompany.end();++it)

		if (lnPlantCount < 1)
		{
			CString sLog;

			sLog.Format("登录失败: 用户%s所属的", strUser);

			if (iUserLevel >= GE_GROUP_OPERATOR)
			{
				sLog.AppendFormat("(集团:%s)下无设备! ", strGroup);
			}
			else if (iUserLevel >= GE_WHOLE_OPERATOR)
			{
				sLog.AppendFormat("(集团:%s|公司:%s)下无设备! ", strGroup, strCompany);
			}
			else
			{
				sLog.AppendFormat("(集团:%s|公司:%s|分厂:%s)下无设备! ", strGroup, strCompany,strFactory);
			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();

			sLog.AppendFormat("SQL=%s", lsSQL);

			CString sLoc;
			sLoc.Format("%s@%d", __FUNCTION__, __LINE__);
			CHZLogManage::Error(sLog, sLoc);
			zdlTraceLine(sLog + sLoc);

		} //if (lnPlantCount < 1)

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return lnPlantCount;
	}

	return -2;
}

/**得到指定公司名、分厂名、机组位号的所有组轴心轨迹对应的测点对，通过循环依次得到，如果没有指定设备号，则是服务器所有轴心轨迹对
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             机组号    FIELD_SYSTEM_PLANT
*返回值的字段码：
*             轴心位置位置描述   FIELD_SYSTEM_AXISLOCATION
*             垂直方向测点    FIELD_SYSTEM_CHANN_VER
*             水平方向测点    FIELD_SYSTEM_CHANN_HOR
*             轴心位置信息结构体 FIELD_SYSTEM_SETSVR_PLANTINFO
*define     SERVICE_SYSTEM_GETALLAXISCHANN      25005
*/
int Service_SysGetAllAxisChann(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sCompany=pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	int nCount=-1;
	string sPlantNo = CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	CAxisChannInfo axisChannInfo;
	int iSize=sizeof(CAxisChannInfo);
	pDB->Cmd("SELECT * FROM [%s] ",gc_cPlantAxisTable);
	pDB->Cmd(" WHERE %s='%s' AND %s='%s' AND [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cPlantNo, sPlantNo.c_str(),gc_cFactory_Name,sFactory.c_str());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		nCount++;
		axisChannInfo.GetAxisChannInfo(pDB);
		pFixComm->SetItemBuf(FIELD_SYSTEM_SETSVR_PLANTINFO,(char*)&(axisChannInfo),iSize);
		pFixComm->SetItem(FIELD_SYSTEM_AXISLOCATION,axisChannInfo._cLocation);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_VER,axisChannInfo._cVerChann);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_HOR,axisChannInfo._cHorChann);
		//CString strTmp;strTmp.Format("%s,%s,%s",axisChannInfo._cLocation,axisChannInfo._cVerChann,axisChannInfo._cHorChann);
		//CHZLogManage::Debug( strTmp, "System","Service_SysGetAllAxisChann");
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}


/**得到指定公司、分厂、设备的设备详细信息
*输入参数：    公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             机组号    FIELD_SYSTEM_PLANT
*返回值的字段码：
*             设备信息  FIELD_SYSTEM_SETSVR_PLANTINFO
#define     SERVICE_SYSTEM_GETPLANTINFO      25007
*/
int Srv_SysGetPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int iRtn=-1;
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	S_PlantInfo plantInfo;
	CImplement impl;
	int iDbID=impl.GetCompanyDBID(pDB,sCompany);
	if (iDbID>0)
		pDB->Cmd("select * from [%s_%d] ",gc_cMachineTable,iDbID);
	else
		pDB->Cmd("select * from [%s] ",gc_cMachineTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantID,sPlant.c_str());
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if (pDB->More())
	{
		if (plantInfo.GetPlantInfo(pDB))
		{
			pFixComm->SetItemBuf(FIELD_SYSTEM_SETSVR_PLANTINFO,(char *)&plantInfo,sizeof(S_PlantInfo));
			iRtn=1;
		}
	}
	pFixComm->Write(pFixComm->GetAci());
	return iRtn;
}

/**得到指定公司的所有分厂名，通过循环依次得到所有分厂名。如果没有指定公司名，就得到所有分厂名
*输入参数：
*                公司名    FIELD_SYSTEM_COMPANY
*返回值的字段码：
*                公司名    FIELD_SYSTEM_COMPANY
*                分厂名    FIELD_SYSTEM_FACTORY
*                分厂别名  FIELD_SYSTEM_SUBCORP_ALIAS
#define     SERVICE_SYSTEM_GETALLFACTORY        25008
*/
#define TO_INT(f) ((int)(f))
int Srv_SysGetAllFactory(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);

	int nCount=0;

	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);

	pDB->Cmd("SELECT * FROM [%s] ",gc_cFactoryTable);
	if (!(sCompany.empty()))
		pDB->Cmd(" WHERE [%s]='%s' ",gc_cCompany,sCompany.c_str());

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	float fLongitude = 0;
	float fLatitude  = 0;


	while(pDB->More())
	{
		fLongitude = pDB->GetDouble(gc_cLongitude);

		if (TO_INT(fLongitude) == 0)
		{
			fLongitude = 116.41;
		}

		fLatitude  = pDB->GetDouble(gc_cLatitude);
		if (TO_INT(fLatitude) == 0)
		{
			fLatitude = 39.97;
		}

		pFixComm->SetItem(FIELD_SYSTEM_COMPANY, pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_FACTORY, pDB->Get(gc_cFactory_Name).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SUBCORP_ALIAS, pDB->Get(gc_cFactory_Alias).c_str());
		pFixComm->SetItem(FIELD_SERVICE_LONGITUDE, fLongitude);
		pFixComm->SetItem(FIELD_SERVICE_LATITUDE,  fLatitude);

		pFixComm->Write(pFixComm->GetAci()); 
		pFixComm->CreateNextAnswer();

		nCount++;
	}

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

	return nCount;
}

/**得到指定所有公司信息
*返回值的字段码：
*                公司名    FIELD_SYSTEM_COMPANY
*                公司别名  FIELD_SYSTEM_COMPANY2
*                集团名    FIELD_SYSTEM_GROUP
*                国家名    FIELD_SYSTEM_COUNTRY
#define     SERVICE_SYSTEM_GETALLCOMPANY        25009
*/
int Srv_SysGetAllCompany(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	pDB->Cmd("SELECT * from [%s]",gc_cCompanyTable);
	while (pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY,pDB->Get(gc_cCompany).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,pDB->Get(gc_cCompanyAlias).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_GROUP,pDB->Get(gc_cGroup).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_COUNTRY,pDB->Get(gc_cCountry).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		nCount++;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
*输入参数： 公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*返回值的字段码：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
*          集团级IP   FIELD_SYSTEM_GROUP_MW_IP
*          集团级Port FIELD_SYSTEM_GROUP_MW_PORT
*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
*          装置级IP   FIELD_SYSTEM_MW_IP
*          装置级Port FIELD_SYSTEM_MW_PORT
*          装置级中间件ID   FIELD_SYSTEM_SET
*          设备类型       FIELD_SYSTEM_TYPE
*          数据源         FIELD_SYSTEM_DATASOURCE
*          显示名称       FIELD_SYSTEM_PLANT_DISPLAY
*          ERP编码        FIELD_SYSTEM_PLANT_ERP_CODE
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
*/
int Svr_SysGetPlantIPofSubCorp(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetPlantIPofSubCorp(pFixComm,pDB);
}


int Svr_SysGetPlantIPofSubCorpEx(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	CImplement impl;
	return impl.SysGetPlantIPofSubCorpEx(pFixComm,pDB);
}

/**得到指定设备位号的所对应的最高分析线数对应的宏定义
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             机组号    FIELD_SYSTEM_PLANT
*返回值的字段码：
*             最高分析线数宏定义   FIELD_SYSTEM_ANALYSISLINES
*             按照采样信息计算的分析线数    FIELD_SERVICE_ZOOM_LINES
*define     SERVICE_SYSTEM_GETPLANTANALYSISLINES
*/
int Service_SysGetPlantAnalysislines(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	int nlines=0;

	pDB->Cmd("SELECT [%s],[%s] FROM [%s] ",gc_cSampleNum,gc_cPlantID,gc_cMachineTable);
	pDB->Cmd(" WHERE [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str());
	if (!(sPlant.empty()))
		pDB->Cmd(" AND [%s]='%s' ",gc_cPlantID,sPlant.c_str());
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		nlines=(pDB->GetLong(gc_cSampleNum))/2.56;
		if(nlines==0)
			nlines=GD_400LINES;
		pFixComm->SetItem(FIELD_SYSTEM_ANALYSISLINES,3200);
		pFixComm->SetItem(FIELD_SERVICE_ZOOM_LINES,nlines);
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantID).c_str());
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nlines;
}

/**通过用户名和密码,修改密码
*输入参数：用户名   FIELD_SYSTEM_USEID
*          密码     FIELD_SYSTEM_PASSWORD
*          新密码   FIELD_SYSTEM_RESERVE_STRING
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*                 如果为-1，表示没有修改成功，为1表示修改成功
*#define     SERVICE_SYSTEM_MODIFYPWD                  25011
*/
int Service_SysModifyPwd(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string strUseID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strDePwd=pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);
	string strPwdNew=pFixComm->GetItem(FIELD_SYSTEM_RESERVE_STRING);

	CString strPwd;
	DSEEncrypt(strDePwd,strPwd);

	pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' AND %s='%s'",\
		gc_cPassWord,gc_cUserTable,gc_cUserName,strUseID.c_str(),gc_cPassWord,strPwd);

	int lnCount = 0;

	if(pDB->More())
	{
		lnCount = pDB->GetCount();
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());

	if (lnCount > 0)
	{
		pDB->ClearSql();
		pDB->Cmd("UPDATE %s SET %s='%s' WHERE %s='%s' AND %s='%s'",\
			gc_cUserTable,gc_cPassWord,strPwdNew.c_str(),gc_cUserName,strUseID.c_str(),gc_cPassWord,strPwd);

		//	AfxMessageBox(pDB->GetSql());

		//pDB->Exec();
		if(pDB->Exec())
		{
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
		}

	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
	}

	pFixComm->Write(pFixComm->GetAci());

	return 1;
}


/**通过测点类型和传感器类型得到特征值描述
*输入参数：
*             设备类型    FIELD_SYSTEM_TYPE
*             测点类型    FIELD_SYSTEM_CHANN_TYPE
*             传感器类型  FIELD_SYSTEM_SENSOR_TYPE
*返回值的字段码：
*             顺序     FIELD_SYSTEM_STATUS
*             对应描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*             特征值单位   FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
*/
int Srv_SysGetCharTypeDesc(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.GetCharTypeDesc(pFixComm,pDB);
}

/**通过用户名得到客户端页面的更新时间
*输入参数：用户名   FIELD_SYSTEM_USEID
*返回值的字段码：
*          刷新时间 FIELD_SYSTEM_REFRESHTIME
#define     SERVICE_SYSTEM_GET_REFRESHTIME            25013
*/
int Service_SysGetRefreshTime(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string strUserID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
	pDB->Cmd("SELECT %s FROM %s WHERE  %s='%s'",\
		gc_cRefreshTime,gc_cUserTable,gc_cUserName,strUserID.c_str());

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_REFRESHTIME,pDB->GetLong(gc_cRefreshTime));
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_REFRESHTIME,2);
	}
	pFixComm->Write(pFixComm->GetAci());
	return 1;
}


/**得到指定设备指定测点类型的所有测点名和对应的报警上下限，通过循环依次得到
*输入参数：
*             公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             设备号    FIELD_SYSTEM_PLANT
*             测点类型  FIELD_SYSTEM_CHANN_TYPE
*返回值的字段码：
*             测点号    FIELD_SYSTEM_CHANN
*             报警上限  FIELD_SYSTEM_ALARM_HIGH
*             报警下限  FIELD_SYSTEM_ALARM_LOW
#define     SERVICE_SYSTEM_GETALARMTHRESHOLD           25014
*/
int Service_SysGetThreshold(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	int iType=pFixComm->GetLong(FIELD_SYSTEM_CHANN_TYPE);
	int nCount=0;
	string sPlantNo=CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);

	pDB->Cmd("SELECT [%s],[%s] FROM [%s] ", gc_cChannID,gc_cAlarmParam,gc_cChannTable);
	pDB->Cmd("WHERE [%s]='%s' AND [%s]='%s' AND [%s]='%s' ",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantNo,sPlantNo.c_str());
	if(iType!=0)
		pDB->Cmd(" AND %s=%d",gc_cChannType,iType);
	CChannAlarmParam alarmParam;
	long iSize=sizeof(CChannAlarmParam);
	//	AfxMessageBox(pDB->GetSql());
	CWaveHead sWaveHead;
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while(pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannID).c_str());
		SimADO_SafeGetBufferEx(pDB, gc_cAlarmParam, (BYTE*)&alarmParam, iSize, sWaveHead);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,alarmParam._fAlarmHH);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,alarmParam._fAlarmHL);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**通过用户名和密码登陆，如果登陆成功。如果用户登录成功，则状态为1，并返回用户级别和刷新时间；否则为-1，并且不返回用户级别和刷新时间。
*输入参数：用户名   FIELD_SYSTEM_USEID
*          密码     FIELD_SYSTEM_PASSWORD
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*          用户级别 FIELD_SYSTEM_USER_LEVEL
*          刷新时间 FIELD_SYSTEM_REFRESHTIME
*          浏览装置 FIELD_SYSTEM_BROWSE_SET （如果为1，则浏览装置，否则不浏览）
#define     SERVICE_SYSTEM_LOGIN                      25015
*/
int Service_SysLogin(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string strUseID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strDePwd=pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);

	CString strPwd;
	DSEEncrypt(strDePwd,strPwd);
	string strStatus="1";


	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pDB->Cmd("SELECT %s,%s,%s FROM %s WHERE %s='%s' AND %s='%s'",\
		gc_cUserLevel,gc_cRefreshTime,gc_cBrowseSet,gc_cUserTable,gc_cUserName,strUseID.c_str(),gc_cPassWord,strPwd);

	//	AfxMessageBox(pDB->GetSql());
	//如果有结果集返回证明登陆正确,否则先验证是否有这个用户,如果有,说明密码不正确
	if(pDB->More()){
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
		pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,pDB->GetLong(gc_cUserLevel));
		pFixComm->SetItem(FIELD_SYSTEM_REFRESHTIME,pDB->GetLong(gc_cRefreshTime));
	}
	else{
		//验证是否有这个用户
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' ",\
			gc_cUserLevel,gc_cUserTable,gc_cUserName,strUseID.c_str());

		if(pDB->More())
			strStatus="密码不正确";
		else
			strStatus="用户不存在";

	}

	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strStatus.c_str());
	pFixComm->Write(pFixComm->GetAci());

	return 1;
}

/**修改用户级别。如果成功，则返回状态为1，否则为-1，并说明原因
*注意：只有管理员才有权限修改用户级别，并且如果是分厂管理员只能修改所在分厂的用户级别。
*input param：用户名         FIELD_SYSTEM_USEID
*             密码           FIELD_SYSTEM_PASSWORD
*             被修改的用户名 FIELD_SYSTEM_MODIFY_USER
*             用户级别       FIELD_SYSTEM_USER_LEVEL
*return field code：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_MODIFY_USERLEVEL            25016
*/
int Service_SysModifyUserLevel(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string strUseID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strDePwd=pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);
	string strModUserID=pFixComm->GetItem(FIELD_SYSTEM_MODIFY_USER);
	int    iLevel=pFixComm->GetLong(FIELD_SYSTEM_USER_LEVEL);

	CString strPwd;
	DSEEncrypt(strDePwd,strPwd);

	string strStatus="";
	string strFactory="";
	int itmp=0;

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	//判断是否用户级别对不,如果不是,则直接返回错误;
	if((iLevel<7)||(iLevel>0)){
		pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s' AND %s='%s'",\
			gc_cUserLevel,gc_cFactory_Name,gc_cUserTable,gc_cUserName,strUseID.c_str(),gc_cPassWord,strPwd);

		//	AfxMessageBox(pDB->GetSql());
		//如果有结果集返回证明有这个用户,否则先验证是否有这个用户,如果有,说明密码不正确
		if(pDB->More()){
			itmp=pDB->GetLong(gc_cUserLevel);
			strFactory=pDB->Get(gc_cFactory_Name);
			//判断用户级别是否够修改其它用户的级别
			if(itmp==6){//具有修改所有用户级别的权限
				pDB->Cmd("UPDATE %s SET %s=%d WHERE %s='%s'",\
					gc_cUserTable,gc_cUserLevel,iLevel,gc_cUserName,strUseID.c_str());
				pDB->Exec();
				if((pDB->GetRecordsAffected())==0){
					strStatus="被修改的用户不存在";
					pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
				}
				else
					pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);

			}
			else if(itmp==3){//具有修改所在分厂用户级别的权限
				pDB->Cmd("UPDATE %s SET %s=%d WHERE %s='%s' AND %s='%s'",\
					gc_cUserTable,gc_cUserLevel,iLevel,gc_cUserName,strUseID.c_str(),gc_cFactory_Name,strFactory.c_str());
				pDB->Exec();
				if((pDB->GetRecordsAffected())==0){
					strStatus="被修改的用户不在这个分厂";
					pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
				}
				else
					pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
			}
			else{//没有修改用户级别的权限
				strStatus="用户没有权限修改别的用户的级别";
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
			}

		}
		else{
			//验证是否有这个用户
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
			pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' ",\
				gc_cUserLevel,gc_cUserTable,gc_cUserName,strUseID.c_str());
			if(pDB->More())
				strStatus="密码不正确";
			else
				strStatus="用户不存在";
		}
	}
	else{//一些基本条件都不满足
		strStatus="级别不在范围之内";
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strStatus.c_str());
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strStatus.c_str());
	pFixComm->Write(pFixComm->GetAci());

	return 1;

}

/**@brief Add new user.如果成功，则返回状态为1，否则为-1，并说明原因。
*注意：只有管理员才有权限添加用户，并且，如果是分厂管理员只能添加为所在分厂的用户。
*input param:用户名         FIELD_SYSTEM_USEID
*            密码           FIELD_SYSTEM_PASSWORD
*            新用户名       FIELD_SYSTEM_MODIFY_USER
*            用户级别       FIELD_SYSTEM_USER_LEVEL
*            所在分厂       FIELD_SYSTEM_FACTORY
*return field code：
*            状态     FIELD_SYSTEM_STATUS
*            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_ADD_NEWUSER                 25017
*/
int Service_SysAddNewUser(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string strUseID=pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strEnPwd=pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);
	string strModUserID=pFixComm->GetItem(FIELD_SYSTEM_MODIFY_USER);
	string strSubcorp=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	int    iLevel=pFixComm->GetLong(FIELD_SYSTEM_USER_LEVEL);

	CString strPwd;
	DSEEncrypt(strEnPwd,strPwd);
	string strStatus="";
	string strFactory="";
	CString ccpwd1="000000";
	CString ccpwd;
	DSEEncrypt(ccpwd1,ccpwd);
	int itmp=0;
	int itmplevel=1;
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	//判断用户级别是否满足要求,如果不是,则直接返回错误;
	if((iLevel<7)||(iLevel>0)||(!strFactory.empty())){
		//验证有没有这个分厂,如果为WHOLE，说明为全厂范围内用户
		pDB->Cmd("SELECT DISTINCT %s FROM %s WHERE %s='%s'",\
			gc_cFactory_Name,gc_cMachineTable,gc_cFactory_Name,strSubcorp.c_str());
		if((pDB->More())&&((strSubcorp.c_str())=="WHOLE")){//因为有返回结果集，说明有这个分厂
			//验证是否有这个用户和用户是否有权限
			pDB->Cmd("SELECT %s,%s FROM %s WHERE %s='%s' AND %s='%s'",\
				gc_cUserLevel,gc_cFactory_Name,gc_cUserTable,gc_cUserName,strUseID.c_str(),gc_cPassWord,strPwd);
			//	AfxMessageBox(pDB->GetSql());
			//如果有结果集返回证明有这个用户,否则先验证是否有这个用户,如果有,说明密码不正确
			if(pDB->More()){
				itmp=pDB->GetLong(gc_cUserLevel);
				strFactory=pDB->Get(gc_cFactory_Name);
				//判断用户级别是否够添加新用户
				if(itmp==6){//具有添加全厂范围内所有用户级别的权限
					if(strSubcorp.c_str()=="WHOLE")
						itmplevel=4;
					pDB->Cmd("INSERT INTO %s(%s,%s,%s,%s,%s,%s) VALUES('%s','000000',%d,'%s',5,%d)",\
						gc_cUserTable,gc_cUserName,gc_cPassWord,gc_cUserLevel,gc_cFactory_Name,gc_cRefreshTime,gc_cBrowseSet,\
						strModUserID.c_str(),iLevel,strSubcorp.c_str(),itmplevel);
					pDB->Exec();
					if((pDB->GetRecordsAffected())==0){
						strStatus="添加新用户失败，可能是新加用户已经存在";
						pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
					}
					else
						pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
				}
				else if(itmp==3){//具有添加所在分厂用户级别的权限
					pDB->Cmd("INSERT INTO %s(%s,%s,%s,%s,%s,%s) VALUES('%s','000000',%d,'%s',5,1)",\
						gc_cUserTable,gc_cUserName,gc_cPassWord,gc_cUserLevel,gc_cFactory_Name,gc_cRefreshTime,gc_cBrowseSet,\
						strModUserID.c_str(),iLevel,strFactory.c_str());
					pDB->Exec();
					if((pDB->GetRecordsAffected())==0){
						strStatus="添加新用户失败，可能是新加用户已经存在";
						pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
					}
					else
						pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
				}
				else{//没有修改用户级别的权限
					strStatus="用户没有权限修改别的用户的级别";
					pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
				}
			}
			else{//验证是否有这个用户
				pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
				pDB->Cmd("SELECT %s FROM %s WHERE %s='%s' ",\
					gc_cUserLevel,gc_cUserTable,gc_cUserName,strUseID.c_str());
				if(pDB->More())
					strStatus="密码不正确";
				else
					strStatus="用户不存在";
			}
		}
		else{//没有返回结果集，说明没有这个分厂
			strStatus="没有这个分厂";
			pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		}

	}
	else{
		strStatus="用户级别不在范围之内";
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strStatus.c_str());
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strStatus.c_str());
	pFixComm->Write(pFixComm->GetAci());
	return 1;
}

/**设置指定机组和测点的是否进行报警检查，如果状态字段输入为1，则检查报警；为0，则不检查报警
*返回值说明：状态如果为1，说明修改成功，否则可以从状态描述中得到错误原因
*输入参数：   机组号    FIELD_SYSTEM_PLANT
*             测点号    FIELD_SYSTEM_CHANN
*             状态     FIELD_SYSTEM_STATUS
*返回值的字段码：
*            状态     FIELD_SYSTEM_STATUS
*            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*/
//#define     SERVICE_SYSTEM_SETCHECKALARMFLAG          25018
int Service_SysSetCheckAlarmFlag(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	string strPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string strChann=pFixComm->GetItem(FIELD_SYSTEM_CHANN);
	int iFlag=pFixComm->GetLong(FIELD_SYSTEM_STATUS);
	if (iFlag!=0)
	{//防止客户端发送的数据为错
		iFlag=1;
	}
	//对测点表的报警状态进行更新
	//	   pDB->Cmd("UPDATE [%s] SET %s=%d  ",gc_cChannTable,gc_cCheckAlarm,iFlag);
	//	   pDB->Cmd(" WHERE %s='%s' AND %s='%s'",gc_cPlantID,strPlant.c_str(),gc_cChannID,strChann.c_str());
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	//AfxMessageBox(pDB->GetSql());
	if (pDB->Exec())
	{//正确执行
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	}
	else
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,-1);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,"指定的机组或测点不存在");
	}
	pFixComm->Write(pFixComm->GetAci());
	return iFlag;
}

/**得到指定机组位号的图结构类型
*输入参数：
*             公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             机组号    FIELD_SYSTEM_PLANT
*返回值的字段码：
*				机组号          FIELD_SYSTEM_PLANT
*			    概貌图类型      FIELD_SYSTEM_PLANT_GRAPHTYPE
*             概貌图BUFF      FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF
*             概貌图BUFF尺寸  FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE
#define     SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID          25019
*/
int Service_SysGetGraphTypeByDeviceID(ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);

	long nCount=0;

	pDB->Cmd("SELECT [%s],[%s],[%s],[%s]  FROM [%s]",gc_cPlantID,gc_cGraphType,gc_cGraphBuff,gc_cGraphBuffBytes,gc_cMachineTable);
	pDB->Cmd("where [%s]='%s' and [%s]='%s' and [%s]='%s'",gc_cCompany,sCompany.c_str(),gc_cFactory_Name,sFactory.c_str(),gc_cPlantID,sPlant.c_str());
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	if(pDB->More())
	{
		nCount=pDB->GetLong(gc_cGraphBuffBytes);
		pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE,nCount);
		BYTE *pBuff=new BYTE[nCount];
		CWaveHead sWaveHead;
		SimADO_SafeGetBufferEx(pDB, gc_cGraphBuff, pBuff, nCount, sWaveHead);
		pFixComm->SetItemBuf(FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF,(char*)pBuff,nCount);
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT_GRAPHTYPE,pDB->GetLong("GRAPH_TYPE"));
		KD_DeletePoint(pBuff);
	}
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**修改监测中心中间件IP和Port
*输入参数：
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          中间件IP    FILELD_SYSTEM_MW_IP
*          中间件PORT  FILELD_SYSTEM_MW_PORT
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
int Svr_SysModifyMwInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysModifyMwInfo(pFixComm,pDB);
}


/**得到监测中心中间件IP和Port
*返回值的字段码：
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          中间件IP    FILELD_SYSTEM_MW_IP
*          中间件PORT  FILELD_SYSTEM_MW_PORT
#define     SERVICE_SYSTEM_GET_MWINFO                      25021
*/
int Svr_SysGetMwInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetMwInfo(pFixComm,pDB);
}

/**修改发送到监测中心的时间间隔和是否发送状态
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022
*/
int Svr_SysModifySendInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysModifySendInfo(pFixComm,pDB);
}

/**得到发送到监测中心的时间间隔和是否发送
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*返回值的字段码：
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
#define     SERVICE_SYSTEM_GET_SENDINFO                     25023
*/
int Svr_SysGetSendInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSendInfo(pFixComm,pDB);
}


/**得到装置级中间件信息
*返回的字段码：
*         分厂名字      FIELD_SYSTEM_FACTORY
*         中间件标识    FIELD_SYSTEM_MW_ID
*         装置名ID      FIELD_SYSTEM_SET
*         信息结构体    FIELD_SYSTEM_SETSVR_SETINFO
*         结构体长度    FIELD_SERVICE_LEN
#define    SERVICE_SYSTEM_GET_SETSVRINFO                     25024
*/
int Svr_SysGetSetSvrInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrInfo(pFixComm,pDB);
}

/**得到装置级中间件服务器的设备详细信息
*		 设备起始索引  FIELD_SERVICE_STARTINDEX
 *返回的字段码：
 *		  设备信息		FIELD_SYSTEM_SETSVR_PLANTINFO
 *		  版本			FIELD_SYSTEM_VERSION
 *		  设备信息		FIELD_SYSTEM_SETSVR_PLANTINFO
 *		  显示集团		FIELD_SERVICE_GROUP_VISIBLE
 *		  显示公司		FIELD_SERVICE_COMPANY_VISIBLE
 *		  显示分厂		FIELD_SERVICE_FACTORY_VISIBLE
 *		  显示装置		FIELD_SERVICE_SET_VISIBLE
 *		  显示设备		FIELD_SERVICE_PLANT_VISIBLE
 *		  集团名称		FIELD_SYSTEM_GROUP
 *		  国家名称		FIELD_SYSTEM_COUNTRY
 *		  设备索引		FIELD_SERVICE_ENDINDEX
 *		  设备数目		FIELD_SERVICE_PLANT_NUM
 *		  设备显示名称  FIELD_SYSTEM_PLANT_DISPLAY
 *		  ERP编码       FIELD_SYSTEM_PLANT_ERP_CODE
#define    SERVICE_SYSTEM_GET_SETSVRPLANTINFO                25025
*/
int Svr_SysGetSetSvrPlantInfo(ICommInterFace * pFixComm, IDBInterFace *pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrPlantInfo(pFixComm,pDB);
}

/**得到装置级中间件服务器指定设备的测点详细信息
* 输入字段码：
设备号：FIELD_SYSTEM_PLANT
*返回的字段码：
*        测点信息  FIELD_SYSTEM_SETSVR_CHANNINFO
*        个数      FIELD_SYSTEM_SETSVR_INFONUM
#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN            25026
*/
int Svr_SysGetSetSvrSpecPlantChann(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrSpecPlantChann(pFixComm,pDB);
}

/**得到所有用户信息
*输入字段码：
*返回字段码：
*            用户名         FIELD_SYSTEM_USEID
*            密码           FIELD_SYSTEM_PASSWORD
*            用户级别       FIELD_SYSTEM_USER_LEVEL
*            所在分厂       FIELD_SYSTEM_FACTORY
*            装置名ID       FIELD_SYSTEM_SET
*            是否浏览装置   FIELD_SYSTEM_BROWSE_SET
*            刷新时间 FIELD_SYSTEM_REFRESHTIME
#define    SERVICE_SYSTEM_GET_ALLUSERINFO               25027 */
int Svr_SysGetAllUserInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetAllUserInfo(pFixComm,pDB);
}

/**得到其子中间件服务器IP,端口和属性信息
*输入字段:
*        公司ID        FIELD_SYSTEM_COMPANY
*返回字段：
*        中间件ID         FIELD_SYSTEM_SET
*        中间件IP         FIELD_SYSTEM_MW_IP
*        中间件Port       FIELD_SYSTEM_MW_PORT
*        中间件级别       FIELD_SYSTEM_USER_LEVEL
#define  SERVICE_SYSTEM_GET_MW_INFO                   25028
*/
int  Svr_SysGetMWSubInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	unsigned int iLevel=GE_SRV_TYPE_SET;
	int nCount=-1;
	//服务器为监测中心级:中间件服务器表的最大ID为1;
	//服务器为装置级:    中间件服务器的最大ID为0;
	//服务器为集团监测中心级:中间件服务器的最大ID为2;
	//得到当前中间件服务器的级别或者类型.
	CImplement impl;
	iLevel= impl.CheckSetSvrLevel(pDB);
	if (iLevel==0)
	{
		pFixComm->CreateAnswer(pFixComm->GetFunc());
		pFixComm->Write(pFixComm->GetAci());
		return -1;
	}
	iLevel-=1;
	pDB->Cmd("select %s,%s,%s,%s from [%s]",
		gc_cSetID, gc_cCenterMw_IP,gc_cCenterMw_Port, 
		SimAdo_Expression_ISNULL(gc_cMwType, 0),//EN_FIXCOMM_TYPE_MFC:0 EN_FIXCOMM_TYPE_ICE:1
		gc_cSetTable);
	pDB->Cmd("  where [%s]=%d ",gc_cChild_Flag,-1);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	while (pDB->More())
	{
		nCount++;
		pFixComm->SetItem(FIELD_SYSTEM_SET,pDB->Get(gc_cSetID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_MW_IP,pDB->Get(gc_cCenterMw_IP).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,pDB->GetLong(gc_cCenterMw_Port));
		pFixComm->SetItem(FIELD_SYSTEM_MW_TYPE,pDB->GetLong(gc_cMwType));
		pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,(int)iLevel);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}

	/*
	int lnMax = 0;

	for (SET_DATA_TYPE::iterator lpIt = g_SET_DATA.begin();
	lpIt!= g_SET_DATA.end();
	lpIt++)
	{
	nCount++;
	SET_INFO & loData = (*lpIt);
	pFixComm->SetItem(FIELD_SYSTEM_SET,loData.m_strSetID);
	pFixComm->SetItem(FIELD_SYSTEM_MW_IP,loData.m_strSetIP);
	pFixComm->SetItem(FIELD_SYSTEM_MW_PORT,loData.m_nSetPort);
	pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,loData.m_nLevel);

	CString lstrData;

	lstrData.Format(_T("%s %s %d %d\r\n"),loData.m_strSetID,loData.m_strSetIP,loData.m_nSetPort,loData.m_nLevel);

	OutputDebugString(lstrData);

	if (loData.m_nLevel>lnMax)
	{
	lnMax = loData.m_nLevel;
	}
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();

	}*/


	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**验证是否中间件是否连通。如果连通则返回值为1。
*输入字段码：
*返回字段码：
*          状态     FIELD_SYSTEM_STATUS
#define    SERVICE_SYSTEM_CONN_STATUS                   25100
*/
int Svr_SysConnStatus(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,1);
	pFixComm->Write(pFixComm->GetAci());
	return 1;
}


/**得到指定设备的所有测点信息。
*input param：设备名 FIELD_SYSTEM_PLANT
*return field code：
*             测点别名 FIELD_SYSTEM_CHANN_ALIAS
*             测点名   FIELD_SYSTEM_CHANN
*             测点类型 FIELD_SYSTEM_CHANN_TYPE
*             报警上限 FIELD_SYSTEM_ALARM_HIGH
*             报警下限 FIELD_SYSTEM_ALARM_LOW
*             状态     FIELD_SYSTEM_STATUS
*             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*define    SERVICE_SYSTEM_GET_CHANNINFO                25108
*/
int Service_SysGetChannInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sPlantNo=CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	pDB->Cmd("SELECT * FROM [%s] WHERE %s='%s'",gc_cChannTable,gc_cPlantNo,sPlantNo.c_str());
	while(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,pDB->Get(gc_cChannIDA).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_CHANN,pDB->Get(gc_cChannID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,pDB->GetLong(gc_cChannType));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,pDB->GetDouble(gc_cAlarmHigh));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,pDB->GetDouble(gc_cAlarmLow));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++nCount;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定分厂的所有设备信息。
*input param: 分厂名  FIELD_SYSTEM_FACTORY
*return field code：
*          设备名   FIELD_SYSTEM_PLANT
*          采样点数 FIELD_SYSTEM_SAMPLE_NUM
*          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
*          数据采集设备IP  FIELD_SYSTEM_DAQIP
*          转速     FIELD_SYSTEM_PLANT_REV
*define    SERVICE_SYSTEM_GET_PLANTINFO                25109
*/
int Service_SysGetPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=0;
	string strSubCorp=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	pDB->Cmd("SELECT * FROM %s WHERE %s='%s'",gc_cMachineTable,gc_cFactory_Name,strSubCorp.c_str());
	while(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SAMPLE_NUM,pDB->GetLong(gc_cSampleNum));
		pFixComm->SetItem(FIELD_SYSTEM_SAMPLE_FREQ,pDB->GetLong(gc_cSampleFreq));
		pFixComm->SetItem(FIELD_SYSTEM_DAQIP,pDB->Get(gc_cDaq_IP).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,pDB->GetLong(gc_cRev));
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
		++nCount;
	}
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定设备的指定测点信息
*input param：设备名 FIELD_SYSTEM_PLANT
*             测点名 FIELD_SYSTEM_CHANN
*return field code：
*             测点别名 FIELD_SYSTEM_CHANN_ALIAS
*             测点类型 FIELD_SYSTEM_CHANN_TYPE
*             报警上限 FIELD_SYSTEM_ALARM_HIGH
*             报警下限 FIELD_SYSTEM_ALARM_LOW
*             状态     FIELD_SYSTEM_STATUS
*             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SPEC_CHANNINFO           25110
*/
int Service_SysSpecChannInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=1;
	string strErrorInfo="";
	string sCompany=pFixComm->Get(FIELD_SYSTEM_COMPANY);
	string sFactory=pFixComm->Get(FIELD_SYSTEM_FACTORY);
	string sPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	string sPlantNo = CImplement::GetPlantNo( sCompany, sFactory, sPlant, pDB);
	string strChann=pFixComm->GetItem(FIELD_SYSTEM_CHANN);
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pDB->Cmd("SELECT * FROM %s WHERE %s='%s' AND %s='%s'",gc_cChannTable,gc_cPlantNo,sPlantNo.c_str(),gc_cChannID,strChann.c_str());
	if(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,pDB->GetLong(gc_cChannIDA));
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,pDB->GetLong(gc_cChannType));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,pDB->GetDouble(gc_cAlarmHigh));
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,pDB->GetDouble(gc_cAlarmLow));
	}
	else{
		nCount=-1;
		strErrorInfo="the channel donot exist by dsem";
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,nCount);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strErrorInfo.c_str());
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到指定分厂的所有设备信息。
*input param: 分厂名  FIELD_SYSTEM_FACTORY
*             设备名   FIELD_SYSTEM_PLANT
*return field code：
*          采样点数 FIELD_SYSTEM_SAMPLE_NUM
*          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
*          数据采集设备IP  FIELD_SYSTEM_DAQIP
*          转速     FIELD_SYSTEM_PLANT_REV
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*define    SERVICE_SYSTEM_GET_SPECPLANTINFO            25111
*/
int Service_SysGetSpecPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);
	int nCount=1;
	string strErrorInfo="";
	string strSubCorp=pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	string strPlant=pFixComm->GetItem(FIELD_SYSTEM_PLANT);
	pFixComm->CreateAnswer(pFixComm->GetFunc());

	pDB->Cmd("SELECT * FROM %s WHERE %s='%s' AND %s='%s'",gc_cMachineTable,gc_cFactory_Name,strSubCorp.c_str(),gc_cPlantID,strPlant.c_str());
	if(pDB->More())
	{
		pFixComm->SetItem(FIELD_SYSTEM_PLANT,pDB->Get(gc_cPlantID).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_SAMPLE_NUM,pDB->GetLong(gc_cSampleNum));
		pFixComm->SetItem(FIELD_SYSTEM_SAMPLE_FREQ,pDB->GetLong(gc_cSampleFreq));
		pFixComm->SetItem(FIELD_SYSTEM_DAQIP,pDB->Get(gc_cDaq_IP).c_str());
		pFixComm->SetItem(FIELD_SYSTEM_PLANT_REV,pDB->GetLong(gc_cRev));
	}
	else
	{
		nCount=-1;
		strErrorInfo="the plant donot exist by dsem";
	}
	pFixComm->SetItem(FIELD_SYSTEM_STATUS,nCount);
	pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION,strErrorInfo.c_str());
	pFixComm->Write(pFixComm->GetAci());
	return nCount;
}

/**得到监测中心的设备信息表
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SYSTEM_PLANT
*返回的字段码：
*             测点号     FIELD_SYSTEM_CHANN
*             索引       FIELD_SYSTEM_STATUS
*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*             单位       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
*/
int Service_SysGetMachineInfoType(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetMachineInfoType(pFixComm,pDB);
}

/**得到监测中心的信息描述表
* 输入字段码：
*返回的字段码：
*        信息类型      FIELD_SYSTEM_TYPE
*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
*/
int Service_SysGetInfoTypeDetail(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetInfoTypeDetail(pFixComm,pDB);
}

/**根据测点信息类型,得到监测中心的信息描述表
* 输入字段码：
*        信息类型   FIELD_SYSTEM_TYPE
*返回的字段码：
*        索引       FIELD_SYSTEM_STATUS
*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*        单位       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
*/
int Service_SysGetInfoTypeDetailBaseType(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetInfoTypeDetailBaseType(pFixComm,pDB);
}

/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
*输入参数：   公司名         FIELD_SYSTEM_COMPANY
*             分厂名         FIELD_SYSTEM_FACTORY
*             机组名         FIELD_SYSTEM_PLANT
*             测点名         FIELD_SYSTEM_CHANN
*             是否常规报警   FIELD_SYSTEM_STATUS //1为判断报警，0为不判断报警
*             报警限类型     FIELD_SYSTEM_EVENT_TYPE
*             报警上限       FIELD_SYSTEM_ALARM_LOW
*             报警上上限     FIELD_SYSTEM_ALARM_HIGH
*             报警下限       FIELD_SYSTEM_ALARM_LOW2
*             报警下下限     FIELD_SYSTEM_ALARM_HIGH2
*返回值的字段码：
*             返回状态       FIELD_SYSTEM_STATUS
#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115
*/
int Service_SysModifySpecChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB)
{
	CImplement impl;
	return impl.SysModifySpecChannAlarmInfo(pFixComm,pDB);
}

/**得到监测中心的自定义参数模板表
* 输入字段码：
*返回的字段码：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID       FIELD_SYSTEM_TEMPID
*         自定义信息描述         FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRTEMPLET      25116
*/
int Service_SysGetSvrTemplet(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSvrTemplet(pFixComm,pDB);
}

/**得到监测中心的自定义参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
*         预处理参数K             FIELD_SERVICE_PREPROC_K
*         预处理参数B             FIELD_SERVICE_PREPROC_B
*         单位                   FIELD_SERVICE_UNIT
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM      25117
*/
int Service_SysGetCustomParam(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetCustomParam(pFixComm,pDB);
}

/**得到监测中心的自定义参数报警参数模板表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义参数模板ID         FIELD_SYSTEM_TEMPID
*         自定义报警模板ID         FIELD_SYSTEM_ALARM_TEMPID
*         自定义报警描述           FIELD_SERVICE_ALARM_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET      25118
*/
int Service_SysGetSvrAlarmTemplet(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSvrAlarmTemplet(pFixComm,pDB);
}

/**得到监测中心的自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119
*/
int Service_SysGetSvrAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSvrAlarmParam(pFixComm,pDB);
}

/**得到监测中心的通道自定义参数报警参数信息表
*输入参数值：
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         公司名                 FIELD_SYSTEM_COMPANY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         测点号                 FIELD_SYSTEM_CHANN
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM     25120
*/
int Service_SysGetSvrChannAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSvrChannAlarmParam(pFixComm,pDB);
}

/**得到指定公司、分厂和装置ID的自定义参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备名                FIELD_SERVICE_PLANTID
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备号                 FIELD_SYSTEM_PLANT
*         索引ID                 FIELD_SYSTEM_STATUS
*         高级参数模板ID         FIELD_SYSTEM_TEMPID
*         数据类型               FIELD_SYSTEM_DATA
*         单位类型               FIELD_SYSTEM_UNIT
*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
*         段起始号               FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         段名                   FIELD_SYSTEM_SEG_NAME
*         数据起始值             FIELD_SYSTEM_START_VALUE
*         数据结束值             FIELD_SYSTEM_END_VALUE
*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121
*/
int Service_SysGetSpecSvrCustomParam(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSpecSvrCustomParam(pFixComm,pDB);
}

/**得到指定公司、分厂和装置ID的自定义参数报警参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*返回参数值：
*         索引ID                 FIELD_SYSTEM_STATUS
*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
*         判断方式               FIELD_SYSTEM_TYPE
*         报警方式               FIELD_SERVICE_ALARM_TYPE
*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM       25122
*/
int Service_SysGetSpecSvrAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSpecSvrAlarmParam(pFixComm,pDB);
}

/**得到监测中心配置的自定义轴承参数表
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123
*/
int Service_SysGetSetSvrCustomBearingTbl(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrCustomBearingTbl(pFixComm,pDB);
}

/**得到监测中心的齿轮箱参数表
#define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124
*/
int Service_SysGetSetSvrGearBoxTbl(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrGearBoxTbl(pFixComm,pDB);
}

/**得到监测中心的位置参数表
#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125
*/
int Service_SysGetSetSvrLocationTbl(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrLocationTbl(pFixComm,pDB);
}

/**得到监测中心的部位参数表
#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126
*/
int Service_SysGetSetSvrSectionTbl(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetSetSvrSectionTbl(pFixComm,pDB);
}

/* 守护巡检 */
/* 获取各个程序的路径和版本号 */
int Service_SysGetModuleVersion(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetModuleVersion(pFixComm,pDB);
}

/* 获取各个数据库状态 */
int Service_SysGetDBStatus(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetDBStatus(pFixComm,pDB);
}

/* 获取中间件运行状态 */
int Service_SysGetServerStatus(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetServerStatus(pFixComm,pDB);
}

/* 获取数采硬件状态 */
int Service_SysGetDAQHWStatus(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetHWDAQStatus(pFixComm,pDB);
}

/* 获取数采软件状态 */
int Service_SysGetDAQSWStatus(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetDAQSWStatus(pFixComm,pDB);
}

/* 获取基本的组态息 */
int Service_SysGetBasicConfigInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetBasicConfigInfo(pFixComm,pDB);
}

int Service_SysGetMiddleWareVersion(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	CImplement impl;
	return impl.SysGetMiddlewareVersion(pFixComm,pDB);
}

/**更新用户信息。
*input param 用户名		 FIELD_SYSTEM_USEID
			 密  码		 FIELD_SYSTEM_PASSWORD
			 公  司		 FIELD_SYSTEM_COMPANY
			 分  厂		 FIELD_SYSTEM_FACTORY
			 集  团		 FIELD_SYSTEM_GROUP
					 
*return field code：
			 返回值		 FIELD_SYSTEM_STATUS
#define    SERVICE_SYSTEM_SET_USERINFO         24071
*/


int Service_UpdateUserInfo(ICommInterFace * pFixComm, IDBInterFace * pDB)
{
	SwitchDbNameEx(pDB,g_strMainDbName);

	CString strUser  = pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString strPwd = pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);
	CString lstrCompanyName = pFixComm->GetItem(FIELD_SYSTEM_COMPANY);
	CString lstrFactoryName = pFixComm->GetItem(FIELD_SYSTEM_FACTORY);
	CString lstrGroupName   = pFixComm->GetItem(FIELD_SYSTEM_GROUP);

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());
	bool lnRet = 0;

	if (strUser.IsEmpty())
	{
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnRet);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
		return FALSE;
	}

	CString lstrSqlQuery;
	CString lstrSqlMainPart;
	CString lstrSqlMainFormat = _T(" UPDATE [%s] SET %s WHERE [%s]='%s'");


	CString lstrSqlKeyValueTextFormat = _T(" [%s] = '%s' ");
	CString lstrSqlKeyValueIntFormat  = _T(" [%s] = %d   ");
	CString lstrSqlKeyValuePart;

	CString lstrTemp;

	//password
	if (!strPwd.IsEmpty())
	{
		lstrTemp.Format(lstrSqlKeyValueTextFormat,gc_cPassWord,strPwd);
		lstrSqlKeyValuePart.Append(lstrTemp);
	}

	USERLEVEL lnLevel = GE_ADMIN;

	//group
	if (!lstrGroupName.IsEmpty())
	{
		lstrTemp.Format(lstrSqlKeyValueTextFormat,gc_cGroup,lstrGroupName);
		lstrSqlKeyValuePart.Append(_T(","));
		lstrSqlKeyValuePart.Append(lstrTemp);
		lnLevel = GE_GROUP_ADMIN;
	}


	//company
	if (TRUE)
	{
		lstrTemp.Format(lstrSqlKeyValueTextFormat,gc_cCompany,lstrCompanyName);
		lstrSqlKeyValuePart.Append(_T(","));
		lstrSqlKeyValuePart.Append(lstrTemp);
		if (!lstrCompanyName.IsEmpty())
		{
			lnLevel = GE_WHOLE_ADMIN;
		}
	}

	//factory
	if (TRUE)
	{
		lstrTemp.Format(lstrSqlKeyValueTextFormat,gc_cFactory_Name,lstrFactoryName);
		lstrSqlKeyValuePart.Append(_T(","));
		lstrSqlKeyValuePart.Append(lstrTemp);
		if (!lstrFactoryName.IsEmpty())
		{
			lnLevel = GE_LOCAL_ADMIN;
		}		
	}

	if (lstrSqlKeyValuePart.IsEmpty())
	{
		ASSERT(FALSE);
		pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnRet);
		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());
		return FALSE;
	}

	//userLevel;
	lstrTemp.Format(lstrSqlKeyValueIntFormat,gc_cUserLevel,lnLevel);
	lstrSqlKeyValuePart.Append(_T(","));
	lstrSqlKeyValuePart.Append(lstrTemp);

	//sql main
	lstrSqlQuery.Format(lstrSqlMainFormat,
		gc_cUserTable,
		lstrSqlKeyValuePart,
		gc_cUserName,
		strUser);

	pDB->Cmd(lstrSqlQuery);
	lnRet = pDB->Exec();

	pFixComm->SetItem(FIELD_SYSTEM_STATUS,lnRet);
	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());


	return  lnRet;
}


/**通过用户名和密码得到用户对应的设备
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          集团名   FIELD_SYSTEM_GROUP
  *          公司名   FIELD_SYSTEM_COMPANY
  *          分厂名   FIELD_SYSTEM_FACTORY
  *          设备名   FIELD_SERVICE_PLANTID
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *
  * #define    SERVICE_SYSTEM_GET_PLANT_OF_USER           25040
  */
int Service_SysGetPlantOfUser( ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	SwitchDbNameEx(pDB,  g_strMainDbName);

	CString sUser = pFixComm->GetItem(FIELD_SYSTEM_USEID);
	CString sPass = pFixComm->GetItem(FIELD_SYSTEM_PASSWORD);

	sUser.Trim();
	sPass.Trim();

	CString lsSQL;

	int lnPlantCount   = 0;

	// 匿名登陆：用户名和密码为空
	if (sUser.IsEmpty() && sPass.IsEmpty()) 
	{
		BOOL bIsAllowAnonymous = IsAllowAnonymous();

		if (!bIsAllowAnonymous)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -1);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "请输入用户名和密码!");
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		ST_DBID_COMPANY_GROUP loCompanyInfo;
		CompanyInfoList_t loCompanyInfoList;

		pDB->Cmd("SELECT [%s],[%s],[%s] FROM [%s] ", gc_cGroup, gc_cCompany, gc_cDB_ID, gc_cCompanyTable);
		while (pDB->More())
		{
			loCompanyInfo._nDbID    = pDB->GetLong(gc_cDB_ID);
			loCompanyInfo._sCompnay = pDB->Get(gc_cCompany).c_str();
			loCompanyInfo._sGroup   = pDB->Get(gc_cGroup).c_str();

			loCompanyInfo._sGroup.Trim();
			loCompanyInfo._sCompnay.Trim();

			if (loCompanyInfo._sGroup.IsEmpty() || loCompanyInfo._sCompnay.IsEmpty())
			{
				ASSERT(FALSE);
				continue;
			}

			loCompanyInfoList.push_back(loCompanyInfo);
		}

		pFixComm->CreateAnswerMul(pFixComm->GetFunc());
		for (CompanyInfoList_t::iterator it = loCompanyInfoList.begin(); it != loCompanyInfoList.end(); ++it)
		{			
			CString lsMachineTable = gc_cMachineTable;

			if (it->_nDbID > 0) 
			{
				lsMachineTable.AppendFormat("_%d", it->_nDbID);
			}
	
			pDB->Cmd("SELECT [%s],[%s] from [%s]",gc_cFactory_Name, gc_cPlantID, lsMachineTable);
			pDB->Cmd(" WHERE %s='%s'", gc_cCompany, it->_sCompnay);


			lsSQL.Append(pDB->GetSql());

			while(pDB->More())
			{
				lnPlantCount++;

				pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL, 0);

				pFixComm->SetItem(FIELD_SYSTEM_GROUP,   it->_sGroup);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY, it->_sCompnay);

				CString sFactory = pDB->Get(gc_cFactory_Name).c_str();
				sFactory.Trim();

				CString sPlant = pDB->Get(gc_cPlantID).c_str();
				sPlant.Trim();

				pFixComm->SetItem(FIELD_SYSTEM_FACTORY, sFactory);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID, sPlant);

				pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);

				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		}

		if (lnPlantCount < 0)
		{
			BHLOG_ERROR(STR("匿名登录失败! SQL=%s", lsSQL));
		}

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return lnPlantCount;
	}
	else
	{
		/* 用户名不区分大小写, 密码区分大小写 */

		if (sUser.IsEmpty())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -1);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "请输入用户名和密码!");
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		sUser.Replace("'","''");

		pDB->Cmd("SELECT [%s],[%s],[%s],[%s],[%s],%s,%s FROM [%s]",
			gc_cUserLevel,
			gc_cPassWord,
			gc_cGroup,
			gc_cCompany,
			gc_cFactory_Name,
			SimAdo_Expression_ISNULL(gc_cNum, 0),
			gc_cMachines,
			gc_cUserTable);
		pDB->Cmd("  WHERE USERNAME='%s'",sUser);

		if (!pDB->More())
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -1);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "用户不存在!");
			pFixComm->Write(pFixComm->GetAci());

			return -1;
		}

		CString lsDBPassword = pDB->Get(gc_cPassWord).c_str();
		lsDBPassword.Trim();

		if (lsDBPassword != sPass)
		{
			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -2);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, "密码不正确!");
			pFixComm->Write(pFixComm->GetAci());

			return -2;
		}

		int iUserLevel     = pDB->GetLong(gc_cUserLevel);

		CString strCompany = pDB->Get(gc_cCompany).c_str();
		CString strGroup   = pDB->Get(gc_cGroup).c_str();
		CString strFactory = pDB->Get(gc_cFactory_Name).c_str();

		strGroup.Trim();
		strCompany.Trim();
		strFactory.Trim();

		int lnMachineNum       = pDB->GetLong(gc_cNum);

		IdSet_t loMachineIds;

		if (lnMachineNum > 0)
		{
			loMachineIds  = UserTable_GetMachineEx(pDB, lnMachineNum);
		}



		IDBInterFace * lpSubDb = pDB;

		lsSQL.Format("SELECT * FROM [%s] ",gc_cCompanyTable);

		switch(iUserLevel)
		{
		case GE_LOCAL_OPERATOR:
		case GE_LOCAL_ENGINEER:
		case GE_LOCAL_ADMIN:
			if (!strGroup.IsEmpty() && !strCompany.IsEmpty() && !strFactory.IsEmpty())
			{
				lsSQL.AppendFormat(" where [%s]='%s' AND [%s]='%s' AND [%s]='%s'",
					gc_cGroup, strGroup,
					gc_cCompany, strCompany,
					gc_cFactory_Name, strFactory);
			}
			break;
		case GE_WHOLE_OPERATOR:
		case GE_WHOLE_ENGINEER:
		case GE_WHOLE_ADMIN:
		case GE_WHOLE_EXPERT:
			if (!strGroup.IsEmpty() && !strCompany.IsEmpty())
			{
				lsSQL.AppendFormat(" where [%s]='%s' AND [%s]='%s'",
					gc_cCompany,strCompany,
					gc_cGroup,strGroup);
			}
			break;
		case GE_GROUP_OPERATOR:
		case GE_GROUP_ENGINEER:
		case GE_GROUP_ADMIN:
		case GE_GROUP_EXPERT:
			if (!strGroup.IsEmpty())
			{
				lsSQL.AppendFormat(" WHERE [%s]='%s' ",
					gc_cGroup,strGroup);
			}
		case GE_EXPERT:
		case GE_ADMIN:
			{
				if (0 == lnMachineNum)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s未关联任何设备", sUser);

					pFixComm->CreateAnswer(pFixComm->GetFunc());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, 0);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog);
					pFixComm->Write(pFixComm->GetAci());

					BHLOG_ERROR(sLog);

					return 0;
				}

				if (lnMachineNum > 0 && loMachineIds.size() < 1)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s关联设备不正确", sUser);

					pFixComm->CreateAnswer(pFixComm->GetFunc());
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, 0);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog);
					pFixComm->Write(pFixComm->GetAci());

					BHLOG_ERROR(sLog);

					return 0;
				}

				typedef std::map<std::string, std::string> Company_Group_Map_t;

				Company_Group_Map_t loCompanyGroupMap;

				lpSubDb->Cmd("SELECT [%s],[%s] FROM %s", gc_cCompany, gc_cGroup, gc_cCompanyTable);
				while(lpSubDb->More())
				{
					CString sCompany = lpSubDb->Get(gc_cCompany).c_str();
					sCompany.Trim();

					CString strGroup = lpSubDb->Get(gc_cGroup).c_str();
					strGroup.Trim();

					loCompanyGroupMap.insert(std::make_pair(sCompany.GetString(), strGroup.GetString()));
				}

				lsSQL.Empty();

				if (lnMachineNum < 0)
				{
					lsSQL.Format("SELECT [%s],[%s],[%s] from [%s]", gc_cCompany, gc_cFactory_Name, gc_cPlantID, gc_cMachineTable);
				}
				else
				{
					lsSQL.Format("SELECT [%s],[%s],[%s] from [%s]", gc_cCompany, gc_cFactory_Name, gc_cPlantID, gc_cMachineTable);
					lsSQL.AppendFormat(" WHERE [%s] in (%s) ", gc_cID, IdSet_ToString(loMachineIds));
				}

				lnPlantCount = 0;

				pFixComm->CreateAnswerMul(pFixComm->GetFunc());

				lpSubDb->Cmd(lsSQL);
				while(lpSubDb->More())
				{
					lnPlantCount++;

					CString sRetCompany = lpSubDb->Get(gc_cCompany).c_str();
					CString sRetFactory = lpSubDb->Get(gc_cFactory_Name).c_str();
					CString sRetPlant   = lpSubDb->Get(gc_cPlantID).c_str();

					sRetCompany.Trim();
					sRetFactory.Trim();
					sRetPlant.Trim();

					CString sRetGroup;

					Company_Group_Map_t::iterator it = loCompanyGroupMap.find(sRetCompany.GetString());

					if (it != loCompanyGroupMap.end())
					{
						sRetGroup = it->second.c_str();
					}
					else
					{
						ASSERT(FALSE);
						sRetGroup = "";
					}

					pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,iUserLevel);
					pFixComm->SetItem(FIELD_SYSTEM_GROUP,   sRetGroup);
					pFixComm->SetItem(FIELD_SYSTEM_COMPANY, sRetCompany);
					pFixComm->SetItem(FIELD_SYSTEM_FACTORY, sRetFactory);
					pFixComm->SetItem(FIELD_SERVICE_PLANTID, sRetPlant);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();
				}

				if (lnPlantCount < 1)
				{
					CString sLog;

					sLog.Format("登录失败: 用户%s关联设备不存在", sUser);

					pFixComm->SetItem(FIELD_SYSTEM_STATUS, -4);
					pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
					pFixComm->Write(pFixComm->GetAci());
					pFixComm->CreateNextAnswer();

					sLog.AppendFormat(" SQL=%s", lsSQL);

					BHLOG_ERROR(sLog);

				} //if (lnPlantCount < 1)

				pFixComm->CreateEndMak();
				pFixComm->Write(pFixComm->GetAci());

				return lnPlantCount;
			}

			break;

		default:
			break;
		}

		lpSubDb->Cmd(lsSQL);

		ST_DBID_COMPANY_GROUP loCompanyInfo;
		CompanyInfoList_t loCompanyInfoList;
		

		while (lpSubDb->More())
		{
			loCompanyInfo._nDbID    = lpSubDb->GetLong(gc_cDB_ID);

			loCompanyInfo._sGroup   = lpSubDb->Get(gc_cGroup).c_str();
			loCompanyInfo._sCompnay = lpSubDb->Get(gc_cCompany).c_str();

			loCompanyInfo._sGroup.Trim();
			loCompanyInfo._sCompnay.Trim();

			loCompanyInfoList.push_back(loCompanyInfo);
		}

		if (loCompanyInfoList.size() < 1)
		{
			CString sLog;

			if (iUserLevel >= GE_GROUP_OPERATOR)
			{
				sLog.Format("登录失败: 用户%s所属的(集团:%s)下未设置公司!", 
					sUser, strGroup);
			}
			else
			{
				sLog.Format("登录失败: 用户%s所属的(集团:%s|公司:%s)不存在!", 
					sUser, strGroup, strCompany);
			}

			pFixComm->CreateAnswer(pFixComm->GetFunc());
			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -3);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
			pFixComm->Write(pFixComm->GetAci());

			sLog.AppendFormat(" SQL=%s", lsSQL);

			BHLOG_ERROR(sLog);

			return -3;
		}

		lnPlantCount   = 0;

		pFixComm->CreateAnswerMul(pFixComm->GetFunc());

		for (CompanyInfoList_t::iterator it = loCompanyInfoList.begin(); it != loCompanyInfoList.end(); ++it)
		{
			lsSQL = "";

			if (it->_nDbID > 0)
				lpSubDb->Cmd("SELECT [%s],[%s] from [%s_%d]",gc_cFactory_Name, gc_cPlantID, gc_cMachineTable,it->_nDbID);
			else
				lpSubDb->Cmd("SELECT [%s],[%s] from [%s]",gc_cFactory_Name, gc_cPlantID, gc_cMachineTable);

			lpSubDb->Cmd(" WHERE [%s]='%s' ",gc_cCompany, it->_sCompnay);

			if ((iUserLevel <= GE_LOCAL_ADMIN) && !strFactory.IsEmpty()) {
				lpSubDb->Cmd(" and [%s]='%s'",gc_cFactory_Name, strFactory);
			}

			lsSQL.Append(lpSubDb->GetSql());

			while(lpSubDb->More())
			{
				lnPlantCount++;

				pFixComm->SetItem(FIELD_SYSTEM_USER_LEVEL,iUserLevel);
				pFixComm->SetItem(FIELD_SYSTEM_GROUP,   it->_sGroup);
				pFixComm->SetItem(FIELD_SYSTEM_COMPANY, it->_sCompnay);

				CString sRetFactory = lpSubDb->Get(gc_cFactory_Name).c_str();
				CString sRetPlant   = lpSubDb->Get(gc_cPlantID).c_str();

				pFixComm->SetItem(FIELD_SYSTEM_FACTORY, sRetFactory);
				pFixComm->SetItem(FIELD_SERVICE_PLANTID, sRetPlant);

				pFixComm->SetItem(FIELD_SYSTEM_STATUS, 1);
				pFixComm->Write(pFixComm->GetAci());
				pFixComm->CreateNextAnswer();
			}
		} //for (it=vCompany.begin();it!=vCompany.end();++it)

		if (lnPlantCount < 1)
		{
			CString sLog;

			sLog.Format("登录失败: 用户%s所属的", sUser);

			if (iUserLevel >= GE_GROUP_OPERATOR)
			{
				sLog.AppendFormat("(集团:%s)下无设备! ", strGroup);
			}
			else if (iUserLevel >= GE_WHOLE_OPERATOR)
			{
				sLog.AppendFormat("(集团:%s|公司:%s)下无设备! ", strGroup, strCompany);
			}
			else
			{
				sLog.AppendFormat("(集团:%s|公司:%s|分厂:%s)下无设备! ", strGroup, strCompany,strFactory);
			}

			pFixComm->SetItem(FIELD_SYSTEM_STATUS, -4);
			pFixComm->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, sLog.GetString());
			pFixComm->Write(pFixComm->GetAci());
			pFixComm->CreateNextAnswer();
			
			sLog.AppendFormat(" SQL=%s", lsSQL);

			BHLOG_ERROR(sLog);

		} //if (lnPlantCount < 1)

		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return lnPlantCount;
	}

	return -2;
}

/**
  根据设备得到用户关联信息。
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR (默认: ,)
  *返回值的字段码：
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          用户名列表       FIELD_SYSTEM_USEID
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR
  *

  #define    SERVICE_SYSTEM_GET_USER_OF_PLANT         25041
  */

struct Iterator_GetUserOfPlant_Param_t
{
	ICommInterFace * pFixComm;
	CString*         pSeperator;
};


int Iterator_GetUserOfPlant( const PlantInfo_t& aPlantInfo, const UserInfoList_t& aUserInfoList, void * aParam )
{
	Iterator_GetUserOfPlant_Param_t * lpParam = (Iterator_GetUserOfPlant_Param_t*)aParam;

	if(NULL == lpParam)
	{
		ASSERT(FALSE);
		return -11;
	}

	if(NULL == lpParam->pFixComm)
	{
		ASSERT(FALSE);
		return -12;
	}

	if(NULL == lpParam->pSeperator)
	{
		ASSERT(FALSE);
		return -13;
	}

	ICommInterFace * pFixComm = lpParam->pFixComm;

	if (aPlantInfo.MachineId < 0)
	{
		pFixComm->CreateEndMak();
		pFixComm->Write(pFixComm->GetAci());

		return -14;
	}

	pFixComm->SetItem(FIELD_SYSTEM_COMPANY, aPlantInfo.CompanyName);
	pFixComm->SetItem(FIELD_SYSTEM_FACTORY, aPlantInfo.FactoryName);
	pFixComm->SetItem(FIELD_SERVICE_PLANTID, aPlantInfo.PlantID);

	CString lsUserNameCsv = UserInfoList_ToString(aUserInfoList, lpParam->pSeperator->GetString());

	pFixComm->SetItem(FIELD_SYSTEM_USEID, lsUserNameCsv);
	pFixComm->SetItem(FIELD_SERVICE_ITEM_SEPERATOR, lpParam->pSeperator->GetString());

	pFixComm->SetItem(FIELD_SYSTEM_COMPANY2,aPlantInfo.CompanyAlias);

	pFixComm->SetItem(FIELD_SERVICE_PLANTID2,aPlantInfo.PlantNo);

	pFixComm->Write(pFixComm->GetAci());
	pFixComm->CreateNextAnswer();

	return true;
}

int Service_GetUserOfPlant( ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	CString lsCompany = pFixComm->Get(FIELD_SYSTEM_COMPANY);
	CString lsFactory = pFixComm->Get(FIELD_SYSTEM_FACTORY);
	CString lsPlant   = pFixComm->Get(FIELD_SERVICE_PLANTID);

	CString lsSeperator = pFixComm->Get(FIELD_SERVICE_ITEM_SEPERATOR);
	if(lsSeperator.IsEmpty())
	{
		lsSeperator = _T(",");
	}

	lsCompany.Trim();
	lsFactory.Trim();
	lsPlant.Trim();

	pFixComm->CreateAnswerMul(pFixComm->GetFunc());

	Iterator_GetUserOfPlant_Param_t loParam;

	loParam.pFixComm   = pFixComm;
	loParam.pSeperator = &lsSeperator;

	int lnResult = UserManager::ForEachPlant(lsCompany, lsFactory, lsPlant, Iterator_GetUserOfPlant, (void *)&loParam);

	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
	
	return lnResult;
}

enum USER_MANAGEMENT_ENUM
{
	UM_CREATE_USER = 0,
	UM_DELETE_USER,
	UM_CHANGE_PASSWORD,
	UM_ADD_PLANT,
	UM_DEL_PLANT,
	UM_SET_PLANT,
	UM_DUMP_USER,
	UM_DUMP_PLANT
};

inline int Fixcomm_success(ICommInterFace * thiz, int exit_code)
{
	ASSERT(thiz);

	thiz->CreateAnswer(thiz->GetFunc());

	thiz->SetItem(FIELD_SYSTEM_STATUS, exit_code);

	thiz->Write(thiz->GetAci());

	return exit_code;
}

inline int Fixcomm_failure(ICommInterFace * thiz, int err_code, const CString& err_text)
{
	ASSERT(thiz);

	thiz->CreateAnswer(thiz->GetFunc());

	thiz->SetItem(FIELD_SYSTEM_STATUS, err_code);
	thiz->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, err_text);

	thiz->Write(thiz->GetAci());

	return err_code;
}

inline int Fixcomm_warning(ICommInterFace * thiz, const CString& warning)
{
	ASSERT(thiz);

	thiz->CreateAnswer(thiz->GetFunc());

	thiz->SetItem(FIELD_SYSTEM_STATUS, 0);
	thiz->SetItem(FIELD_SYSTEM_STATUS_DISCRIPTION, warning);

	thiz->Write(thiz->GetAci());

	return 0;
}


/** 用户管理接口
  *输入参数：
  *         操作类型        FIELD_SYSTEM_TYPE  (0: 新建用户 [+修改设备], 1: 删除用户, 2: 更改密码, 3: 添加设备, 4: 移除设备, 5: 修改设备)
  *          用户名          FIELD_SYSTEM_USEID
  *          用户级别        FIELD_SYSTEM_USER_LEVEL
  *          密码            FIELD_SYSTEM_PASSWORD
  *          设备列表        FIELD_SYSTEM_PLANT   1,2,3
  *          分割符          FIELD_SERVICE_ITEM_SEPERATOR (默认: ,)
  *返回值的字段码：
  *          状态            FIELD_SYSTEM_STATUS  (<0 失败)
  *          状态描述        FIELD_SYSTEM_STATUS_DISCRIPTION
  *
  *#define    SERVICE_SYSTEM_ACTION_USER_PLANT           25042
  */

int Service_ManageUserPlant( ICommInterFace * pFixComm, IDBInterFace * pDB )
{
	int lbSuccess = TRUE;

	int lnAction = pFixComm->GetLong(FIELD_SYSTEM_TYPE);

	CString lsUserName = pFixComm->Get(FIELD_SYSTEM_USEID);
	lsUserName.Trim();

	CString lsPassword = pFixComm->Get(FIELD_SYSTEM_PASSWORD);
	lsPassword.Trim();

	CString lsPlantList = pFixComm->Get(FIELD_SYSTEM_PLANT);
	lsPlantList.Trim();

	CString lsItemSeperator = pFixComm->Get(FIELD_SERVICE_ITEM_SEPERATOR);

	int lnUserLevel = pFixComm->GetLong(FIELD_SYSTEM_USER_LEVEL);

	if (lsUserName.IsEmpty())
	{
		if (UM_DUMP_USER == lnAction)
		{
			return UserManager::UserDump(pFixComm);
		}
		else if (UM_DUMP_PLANT == lnAction)
		{
			return UserManager::MachineDump(pFixComm);
		}

		return Fixcomm_failure(pFixComm, UM_ERROR_INVALID_USERNAME, "请输入用户名!");
	}

	lsUserName.Replace("'", "''");


	switch (lnAction)
	{
	case UM_CREATE_USER:

		{
			if (lsPassword.IsEmpty())
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_INVALID_PASSWORD, "请输入密码!");
			}

			if (UserManager::IsUserExist(lsUserName))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_ALREADY_EXIST, STR("用户%s已存在!", lsUserName));
			}

			UserInfo_t loUserInfo;

			loUserInfo.UserName = lsUserName;

			lsPassword.Replace("'", "''");


			pDB->Cmd("SELECT ISNULL(MAX([%s]), 0) AS %s FROM %s", gc_cID, gc_cID, gc_cUserTable);
			if (!pDB->More())
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_SQL_CMD, STR("用户表错误!", lsUserName));
			}

			loUserInfo.UserId = pDB->GetLong(gc_cID) + 1;

			CString lsSQL;

			IdSet_t loNewMachineIdList;

			if (lsItemSeperator.IsEmpty())
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList);
			}
			else
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList, lsItemSeperator);
			}

			IdSet_t loTotalMachineIdList = UserManager::GetTotalMachineIDList();

			{
				IdSet_t non_exist = IdSet_Difference(loNewMachineIdList, loTotalMachineIdList);

				if (non_exist.size() > 0)
				{
					return Fixcomm_failure(pFixComm, UM_ERROR_PLANT_LIST_INVALID, STR("设备%s不存在!", IdSet_ToString(non_exist)));
				}
			}

			if (lnUserLevel == GE_ADMIN)
			{
				loUserInfo.UserLevel  = GE_ADMIN;
			}
			else
			{
				loUserInfo.UserLevel  = GE_EXPERT;
			}

			lsSQL.Format("INSERT [%s]", gc_cUserTable);
			lsSQL.AppendFormat("([%s],[%s],[%s],[%s])", 
				gc_cID, gc_cUserName, gc_cPassWord, gc_cUserLevel);
			lsSQL.AppendFormat("VALUES(%d,'%s','%s',%d)", 
				loUserInfo.UserId , loUserInfo.UserName, lsPassword, loUserInfo.UserLevel);

			pDB->Cmd(lsSQL);

			try
			{
				lbSuccess = pDB->Exec();
			}
			catch (...)
			{
				ASSERT(FALSE);
				BHLOG_ERROR(STR("Database Exception: 新建用户%s异常! SQL=%s", lsUserName, lsSQL));

				return Fixcomm_failure(pFixComm, UM_ERROR_CREATE_USER, STR("新建用户%s异常!", lsUserName));
			}

			if (!lbSuccess)
			{
				BHLOG_ERROR(STR("新建用户%s失败! SQL=%s", lsUserName, lsSQL));
				return Fixcomm_failure(pFixComm, UM_ERROR_CREATE_USER, STR("新建用户%s失败!", lsUserName));
			}

			if (loNewMachineIdList.size() > 0)
			{
				if (!UserTable_SetMachine(pDB, loUserInfo.UserId, loNewMachineIdList))
				{
					BHLOG_ERROR(STR("设置用户%s关联设备失败! SQL=%s", lsUserName, lsSQL));

					pDB->Cmd("DELETE %s WHERE [%s]='%s'", gc_cUserTable, gc_cUserName, lsUserName);
					return Fixcomm_failure(pFixComm, UM_ERROR_CREATE_USER, STR("新建用户%s失败, 无法关联设备!", lsUserName));
				}
			}

			VERIFY(UserManager::CreateUser(loUserInfo, loNewMachineIdList) == UM_SUCCESS);

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	case UM_DELETE_USER:
		{
			if (!UserManager::IsUserExist(lsUserName))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_NOT_EXIST, STR("用户%s不存在!", lsUserName));
			}

			pDB->Cmd("DELETE %s", gc_cUserTable);
			pDB->Cmd(" WHERE [%s]='%s'", gc_cUserName, lsUserName);

			try
			{
				lbSuccess = pDB->Exec();
			}
			catch (...)
			{
				ASSERT(FALSE);
				BHLOG_ERROR(STR("Database Exception: 删除用户%s异常!", lsUserName));
			
				return Fixcomm_failure(pFixComm, UM_ERROR_DELETE_USER, STR("删除用户%s异常!", lsUserName));
			}

			if (!lbSuccess)
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_DELETE_USER, STR("删除用户%s失败!", lsUserName));
			}

			VERIFY(UserManager::DeleteUser(lsUserName) == UM_SUCCESS);

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	case UM_CHANGE_PASSWORD:

		{
			if (lsPassword.IsEmpty())
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_INVALID_PASSWORD, "请输入密码!");
			}

			if(!UserManager::IsUserExist(lsUserName))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_NOT_EXIST, STR("用户%s不存在!", lsUserName));
			}

			lsPassword.Replace("'", "''");

			pDB->Cmd("UPDATE %s SET %s='%s'", gc_cUserTable,  gc_cPassWord, lsPassword);
			pDB->Cmd(" WHERE [%s]='%s'", gc_cUserName, lsUserName);

			try
			{
				lbSuccess = pDB->Exec();
			}
			catch (...)
			{
				ASSERT(FALSE);
				BHLOG_ERROR(STR("Database Exception: cannot delete user!"));

				return Fixcomm_failure(pFixComm, UM_ERROR_CHANGE_PASSWORD, STR("修改用户%s密码异常", lsUserName));
			}

			if (!lbSuccess)
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_CHANGE_PASSWORD, STR("修改用户%s密码失败", lsUserName));
			}

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	case UM_ADD_PLANT:

		{
			UserInfo_t loUserInfo;

			if (!UserManager::GetUserInfo(lsUserName, loUserInfo))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_NOT_EXIST, STR("用户%s不存在!", lsUserName));
			}

			IdSet_t loCacheMachineIDList;

			UserManager::GetMachineIDList(loUserInfo.UserId, loCacheMachineIDList);

			IdSet_t loNewMachineIdList;

			if (lsItemSeperator.IsEmpty())
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList);
			}
			else
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList, lsItemSeperator);
			}

			if (loNewMachineIdList.size() <= 0)
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_PLANT_LIST_INVALID, "设备列表不正确");
			}

			IdSet_t unchanged = IdSet_Intersection(loCacheMachineIDList, loNewMachineIdList);
			if (unchanged.size() > 0)
			{
				BHLOG_WARN(STR("用户管理: 用户%s己关联设备%s", lsUserName, IdSet_ToString(unchanged)));
			}

			IdSet_t added = IdSet_Difference(loNewMachineIdList, unchanged);

			if (added.size() < 1)
			{
				BHLOG_WARN(STR("用户管理: 用户%s未关联设备%s", lsUserName, IdSet_ToString(loNewMachineIdList)));

				return Fixcomm_success(pFixComm, lbSuccess);
			}

			IdSet_t loTotalMachineIdList = UserManager::GetTotalMachineIDList();

			{
				IdSet_t non_exist = IdSet_Difference(loNewMachineIdList, loTotalMachineIdList);

				if (non_exist.size() > 0)
				{
					return Fixcomm_failure(pFixComm, UM_ERROR_PLANT_LIST_INVALID, STR("设备%s不存在!", IdSet_ToString(non_exist)));
				}
			}

			IdSet_t loUpdateMachineIdList = IdSet_Union(loCacheMachineIDList, loNewMachineIdList);

			if (!UserTable_SetMachine(pDB, loUserInfo.UserId, loUpdateMachineIdList))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("增加用户%s关联的设备失败", lsUserName));
			}

			int lnOldUserLevel = loUserInfo.UserLevel;
			int lnNewUserLevel = -1;


			if (lnOldUserLevel != GE_EXPERT || lnOldUserLevel != GE_EXPERT)
			{
				if (lnOldUserLevel == GE_LOCAL_ADMIN || lnOldUserLevel == GE_WHOLE_ADMIN || lnOldUserLevel == GE_GROUP_ADMIN)
				{
					lnNewUserLevel = GE_ADMIN;
				}
				else
				{
					lnNewUserLevel = GE_EXPERT;
				}

				CString lsSQL;

				lsSQL.Format("UPDATE %s SET [%s]=%d, [%s]='', [%s]='', [%s]=''",
					gc_cUserTable,
					gc_cUserLevel, lnNewUserLevel,
					gc_cGroup, gc_cCompany, gc_cFactory_Name);
				lsSQL.AppendFormat(" WHERE [%s]=%d", gc_cID, loUserInfo.UserId);

				pDB->Cmd(lsSQL);

				try
				{
					lbSuccess = pDB->Exec();
				}
				catch (...)
				{
					ASSERT(FALSE);
					BHLOG_ERROR(STR("User Management Exception: 增加用户%s关联的设备异常! SQL=%s", lsUserName, lsSQL));
					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("增加用户%s关联的设备异常", lsUserName));

					return FALSE;
				}

				if (!lbSuccess)
				{
					BHLOG_ERROR(STR("增加用户%s关联的设备失败! SQL=%s", lsUserName, lsSQL));
					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("增加用户%s关联的设备失败", lsUserName));
				}
			}

			UserManager::Bind(loUserInfo.UserId, added);

			if (lnNewUserLevel > 0)
			{
				UserManager::SetUserLevel(loUserInfo.UserId, lnNewUserLevel);
			}

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	case UM_DEL_PLANT:

		{
			UserInfo_t loUserInfo;

			if (!UserManager::GetUserInfo(lsUserName, loUserInfo))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_NOT_EXIST, STR("用户%s不存在!", lsUserName));
			}

			IdSet_t loCacheMachineIDList;

			if (!UserManager::GetMachineIDList(loUserInfo.UserId, loCacheMachineIDList))
			{
				return Fixcomm_warning(pFixComm, STR("用户%s没有关联的设备列表!", lsUserName));
			}

			IdSet_t loNewMachineIdList;

			if (lsItemSeperator.IsEmpty())
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList);
			}
			else
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList, lsItemSeperator);
			}

			IdSet_t deled = IdSet_Intersection(loCacheMachineIDList, loNewMachineIdList);
			if (deled.size() < 1)
			{
				BHLOG_WARN(STR("用户管理: 用户%s未关联设备%s", lsUserName, IdSet_ToString(loNewMachineIdList)));

				return Fixcomm_success(pFixComm, lbSuccess);
			}

			IdSet_t loUpdateMachineIdList = IdSet_Difference(loCacheMachineIDList, deled);

			if (!UserTable_SetMachine(pDB, loUserInfo.UserId, loUpdateMachineIdList))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("用户管理: 删除用户%s关联的设备失败", lsUserName));
			}

			int lnOldUserLevel = loUserInfo.UserLevel;
			int lnNewUserLevel = -1;

			
			if (lnOldUserLevel != GE_EXPERT || lnOldUserLevel != GE_EXPERT)
			{
				if (lnOldUserLevel == GE_LOCAL_ADMIN || lnOldUserLevel == GE_WHOLE_ADMIN || lnOldUserLevel == GE_GROUP_ADMIN)
				{
					lnNewUserLevel = GE_ADMIN;
				}
				else
				{
					lnNewUserLevel = GE_EXPERT;
				}

				CString lsSQL;

				lsSQL.Format("UPDATE %s SET [%s]=%d, [%s]='', [%s]='', [%s]=''",
					gc_cUserTable,
					gc_cUserLevel, lnNewUserLevel,
					gc_cGroup, gc_cCompany, gc_cFactory_Name);
				lsSQL.AppendFormat(" WHERE [%s]=%d", gc_cID, loUserInfo.UserId);

				pDB->Cmd(lsSQL);

				try
				{
					lbSuccess = pDB->Exec();
				}
				catch (...)
				{
					ASSERT(FALSE);
					BHLOG_ERROR(STR("User Management Exception: 删除用户%s关联的设备异常!SQL=%s", lsUserName, lsSQL));
					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("删除用户%s关联的设备异常", lsUserName));

					return FALSE;
				}

				if (!lbSuccess)
				{
					BHLOG_ERROR(STR("删除用户%s关联的设备失败!SQL=%s", lsUserName, lsSQL));
					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("删除用户%s关联的设备失败", lsUserName));
				}
			}
			

			UserManager::Unbind(loUserInfo.UserId, deled);

			if (lnNewUserLevel > 0)
			{
				UserManager::SetUserLevel(loUserInfo.UserId, lnNewUserLevel);
			}

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	case UM_SET_PLANT:

		{
			UserInfo_t loUserInfo;

			if (!UserManager::GetUserInfo(lsUserName, loUserInfo))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_USER_NOT_EXIST, STR("用户%s不存在!", lsUserName));
			}

			IdSet_t loCacheMachineIDList;

			UserManager::GetMachineIDList(loUserInfo.UserId, loCacheMachineIDList);

			IdSet_t loNewMachineIdList;

			if (lsItemSeperator.IsEmpty())
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList);
			}
			else
			{
				loNewMachineIdList = IdSet_FromString(lsPlantList, lsItemSeperator);
			}

			IdSet_t unchanged = IdSet_Intersection(loCacheMachineIDList, loNewMachineIdList);
			if (unchanged.size() > 0)
			{
				BHLOG_WARN(STR("用户管理: 用户%s己关联设备%s", lsUserName, IdSet_ToString(unchanged)));
			}

			IdSet_t deled = IdSet_Difference(loCacheMachineIDList, unchanged);
			IdSet_t added = IdSet_Difference(loNewMachineIdList, unchanged);

			if (deled.size() < 1 && added.size() < 1)
			{
				BHLOG_WARN(STR("用户管理: 用户%s己关联设备%s", lsUserName, IdSet_ToString(loNewMachineIdList)));

				return Fixcomm_success(pFixComm, lbSuccess);
			}

			IdSet_t loTotalMachineIdList = UserManager::GetTotalMachineIDList();

			{
				IdSet_t non_exist = IdSet_Difference(loNewMachineIdList, loTotalMachineIdList);

				if (non_exist.size() > 0)
				{
					return Fixcomm_failure(pFixComm, UM_ERROR_PLANT_LIST_INVALID, STR("设备%s不存在!", IdSet_ToString(non_exist)));
				}
			}

			IdSet_t& loUpdateMachineIdList = loNewMachineIdList;

			if (!UserTable_SetMachine(pDB, loUserInfo.UserId, loUpdateMachineIdList))
			{
				return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("用户%s的设备关联失败", lsUserName));
			}
			
			int lnOldUserLevel = loUserInfo.UserLevel;
			int lnNewUserLevel = -1;
			
			if (lnOldUserLevel != GE_EXPERT || lnOldUserLevel != GE_EXPERT)
			{
				if (lnOldUserLevel == GE_LOCAL_ADMIN || lnOldUserLevel == GE_WHOLE_ADMIN || lnOldUserLevel == GE_GROUP_ADMIN)
				{
					lnNewUserLevel = GE_ADMIN;
				}
				else
				{
					lnNewUserLevel = GE_EXPERT;
				}

				CString lsSQL;

				lsSQL.Format("UPDATE %s SET [%s]=%d, [%s]='', [%s]='', [%s]=''",
					gc_cUserTable,
					gc_cUserLevel, lnNewUserLevel,
					gc_cGroup, gc_cCompany, gc_cFactory_Name);
				lsSQL.AppendFormat(" WHERE [%s]=%d", gc_cID, loUserInfo.UserId);

				pDB->Cmd(lsSQL);

				try
				{
					lbSuccess = pDB->Exec();
				}
				catch (...)
				{
					ASSERT(FALSE);
					BHLOG_ERROR(STR("User Management Exception: 设备用户%s关联的设备异常! SQL=%s", lsUserName, lsSQL));

					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("用户管理: 设备用户%s关联的设备失败", lsUserName));
				}

				if (!lbSuccess)
				{
					BHLOG_ERROR(STR("设备用户%s关联的设备失败! SQL=%s", lsUserName, lsSQL));

					return Fixcomm_failure(pFixComm, UM_ERROR_ADD_PLANT, STR("用户管理: 设备用户%s关联的设备失败", lsUserName));
				}
			}
			

			if (deled.size() > 0)
			{
				UserManager::Unbind(loUserInfo.UserId, deled);
			}

			if (added.size() > 0)
			{
				UserManager::Bind(loUserInfo.UserId, added);
			}
			
			if (lnNewUserLevel > 0)
			{
				UserManager::SetUserLevel(loUserInfo.UserId, lnNewUserLevel);
			}

			return Fixcomm_success(pFixComm, lbSuccess);
		}

		break;

	default:

		return Fixcomm_failure(pFixComm, UM_ERROR_ACTION_TYPE_INVALID, STR("不支持操作%d!", lnAction));

		break;
	}

	return -1;
}
