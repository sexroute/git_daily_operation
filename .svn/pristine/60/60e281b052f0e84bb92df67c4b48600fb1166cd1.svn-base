#include "StdAfx.h"


#ifdef _PIDMS
#include "../S_Inc/debug.h"
#endif


#include <CaseLibrary/MyLog.h>
#include <CaseLibrary/ConnDB.h>

typedef vector<unsigned char> BUFFER;

CConnDB::CConnDB( IDBInterFace  & pDB )
{
	this->_pdb = &pDB;
}
CConnDB::~CConnDB(void)
{
}

BOOL CConnDB::Conn()
{
	return TRUE;
}

CString CConnDB::GetLastError()
{
	return _strLastError;
}

int CConnDB::AddCase( CString strPlantNO_,CString strFactory_,CString strCompany_,CString strCaseName_, CString strCaseDetail_,CString strCaseTime_,CString strInputUser_,int& nCaseID_ )
{
	// 中间件接口 60001
	if (!Conn())
	{
		_strLastError = "数据库连接失败！";
		return -1;
	}
	nCaseID_ = 1;

	// 获得案例ID
	CString strSelSql = "select max(CaseID) as 'ID' from CaseBasicInfo";
	_pdb->Cmd(strSelSql.GetBuffer());
	if(_pdb->More())
	{
		nCaseID_ = (int)_pdb->GetLong("ID");
		nCaseID_ += 1;
	}

	// 案例名称不能重复
	//strSelSql.Format("select * from CaseBasicInfo where CaseName='%s'",strCaseName_);
	//_pdb->Cmd(strSelSql);
	//if (_pdb->More())
	//{
	//	_strLastError = "案例名称不能重复";
	//	return -1;
	//}

	CBHDateTime dtNow = CBHDateTime::GetCurrentTime();
	CString strDtNow = dtNow.Format(COLEDATETIME_FORMAT_STR_DATETIME);
	CString strInsertSql;
	strInsertSql.Format("insert into CaseBasicInfo(CaseID,CaseName,CaseDetail,Company,Factory,PlantNO,CaseLoggingTime,CaseLoggingUser,CaseHappenTime,CaseVerifyStatus) \
						values(%d,'%s','%s','%s','%s','%s','%s','%s','%s',%d)",
						nCaseID_,strCaseName_,strCaseDetail_,strCompany_,
						strFactory_,strPlantNO_,strDtNow,strInputUser_,strCaseTime_,CASE_VERIFY_SUBMIT);

	_pdb->Cmd(strInsertSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = _pdb->GetLastErrMsg();
		_strLastError = "案例添加失败";
		return -1;
	}
	return 0;
}

int CConnDB::QueryCase( vector<ST_Case_Info>& vCaseList_,CString strPlantName_/*=""*/,
	CString strFactory_/*=""*/,CString strCompany_/*=""*/,
	E_CASE_VERIFY_STATUS eCaseStatus_/*=CASE_VERIFY_NONE*/, 
	CString strFaultID_/*=""*/,int nSymptomID_/*=-1*/,CString strCaseIDArr_/*=""*/,
	const CBHDateTime& dtBegin_/*=COLEDATETIME_NULL*/,
	const CBHDateTime& dtEnd_/*=COLEDATETIME_NULL*/)
{
	if (!Conn())
	{
		_strLastError = "数据库连接失败！";
		return -1;
	}
	vCaseList_.clear();
	CString strQuerySql = "select * from CaseBasicInfo where 1=1 ";
	if (!strPlantName_.IsEmpty())
	{
		CString strPlantNO = strPlantName_;
		strQuerySql += " and PlantNO='";
		strQuerySql += strPlantNO;
		strQuerySql += "'";
	}
	if (!strFactory_.IsEmpty())
	{
		strQuerySql += " and Factory='";
		strQuerySql += strFactory_;
		strQuerySql += "'";
	}
	if (!strCompany_.IsEmpty())
	{
		strQuerySql += " and Company='";
		strQuerySql += strCompany_;
		strQuerySql += "'";
	}
	if (CASE_VERIFY_NONE != eCaseStatus_)
	{
		CString str = strQuerySql;
		strQuerySql.Format("%s  and CaseVerifyStatus= %d",str, eCaseStatus_);
	}
	if (!strCaseIDArr_.IsEmpty())
	{
		CString str = strQuerySql;
		strQuerySql.Format("%s  and CaseID in (%s)",str, strCaseIDArr_);
	}
	if (COLEDATETIME_NULL != dtBegin_ && dtBegin_ > 0)
	{
		CString str = strQuerySql;
		strQuerySql.Format("%s  and CaseHappenTime >= ('%s')",str, dtBegin_.Format(COLEDATETIME_FORMAT_STR_DATETIME));
	}
	if (COLEDATETIME_NULL != dtEnd_ && dtEnd_ > 0)
	{
		CString str = strQuerySql;
		strQuerySql.Format("%s  and CaseHappenTime <= ('%s')",str, dtEnd_.Format(COLEDATETIME_FORMAT_STR_DATETIME));
	}
	_pdb->Cmd(strQuerySql.GetBuffer());
	while (_pdb->More())
	{
		ST_Case_Info caseInfo;
		caseInfo._dtAddTime = _pdb->GetTime("CaseLoggingTime");
		caseInfo._dtCase	= _pdb->GetTime("CaseHappenTime");
		caseInfo._dtVerify	= _pdb->GetTime("CaseVerifyTime");
		caseInfo._eCaseCheckStatus	= (E_CASE_VERIFY_STATUS)_pdb->GetLong("CaseVerifyStatus");
		caseInfo._nCaseID			= (int)_pdb->GetLong("CaseID");
		caseInfo._strAddUserName	= _pdb->Get("CaseLoggingUser").c_str();
		caseInfo._strCaseDetail		= _pdb->Get("CaseDetail").c_str();
		caseInfo._strCaseName		= _pdb->Get("CaseName").c_str();
		caseInfo._strCompany		= _pdb->Get("Company").c_str();
		caseInfo._strFactory		= _pdb->Get("Factory").c_str();
		caseInfo._strPlantNO		= _pdb->Get("PlantNO").c_str();
		caseInfo._strVerifyUserName	= _pdb->Get("CaseVerifyUser").c_str();

		vCaseList_.push_back(caseInfo);
	}
	return 0;
}

int CConnDB::UpdateCase_BasicInfo( int nCaseID_,CString strName_,CString strDetail_,CBHDateTime dtCase_ ,
								  const CString& strPlantNO_,const CString& strFactory_,
								  const CString& strCompany_)
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strDtCase = dtCase_.Format(COLEDATETIME_FORMAT_STR_DATETIME);
	CString strUpdateSql;
	strUpdateSql.Format("update CaseBasicInfo set CaseName='%s',\
						CaseDetail='%s',CaseHappenTime='%s',  \
						Company='%s',Factory='%s',PlantNO='%s' where CaseID=%d",
						strName_,strDetail_,strDtCase,
						strCompany_,strFactory_,strPlantNO_,nCaseID_);

	_pdb->Cmd(strUpdateSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = strUpdateSql;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteCaseFault( int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString strDelSql;
	strDelSql.Format("delete from CaseFault where CaseID=%d",nCaseID_);

	_pdb->Cmd(strDelSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = strDelSql;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteCaseSymptom( int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString strDelSql;
	strDelSql.Format("delete from CaseSymptomInfo where CaseID=%d",nCaseID_);

	_pdb->Cmd(strDelSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = strDelSql;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteCaseAttachment( int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString strDelSql;
	strDelSql.Format("delete from CaseAttachment where CaseID=%d",nCaseID_);

	_pdb->Cmd(strDelSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = strDelSql;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteCaseBasicInfo( int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strDelSql;
	strDelSql.Format("delete from CaseBasicInfo where CaseID=%d",nCaseID_);

	_pdb->Cmd(strDelSql.GetBuffer());

	if (!_pdb->Exec())
	{
		_strLastError = strDelSql;
		return -1;
	}
	return 0;
}

int CConnDB::AddCaseFault( int nCaseID_,CString strFaultID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strInsert;
	strInsert.Format("insert into CaseFault(CaseID,FaultID) values(%d,'%s')",nCaseID_,strFaultID_);
	_pdb->Cmd(strInsert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strInsert;
		return -1;
	}
	return 0;
}

int CConnDB::AddCaseSymptom( int nCaseID_,CString strSymptomID_,int nSymptomValueID_,float fDegree_ ,
			int nCaseSymptomCount_,int nCaseSymptomIndex_,CString strWeightMatrix_)
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}		
	CString strInsert;
	strInsert.Format("insert into CaseSymptomInfo(CaseID,SymptomID,SymptomValueID,ImportantDegree,CaseSymptomCount,SymptomIndex,SymptomRowImportanceData)\
					 values(%d,'%s',%d,%f,%d,%d,'%s')",
					 nCaseID_,strSymptomID_,nSymptomValueID_,fDegree_,
					 nCaseSymptomCount_,nCaseSymptomIndex_,strWeightMatrix_);
	_pdb->Cmd(strInsert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strInsert;
		return -1;
	}
	return 0;
}

int CConnDB::AddCaseAttachment( int nCaseID_,CString strAttaName_,CString strFileName_,
		int nAttaType_,long nAttaSize_, BYTE* pAttaData_,CString strChann_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		CMyLog::Trace_Log(_strLastError.GetBuffer());
		return -1;
	}
	CString strInsert;
	strInsert.Format("insert into CaseAttachment \
					 (AttachName,CaseID,AttachType,AttachSize,FileName,ChannInfo) \
					 values('%s',%d,%d,%d,'%s','%s');",
					 strAttaName_,nCaseID_,nAttaType_,nAttaSize_,strFileName_,
					 strChann_
					 );
	_pdb->Cmd(strInsert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strInsert;
		CMyLog::Trace_Log(_strLastError.GetBuffer());
		return -1;
	}

	CString strSqlWhere;
	strSqlWhere.Format(_T("CaseID=%d and AttachName='%s'"),nCaseID_,strAttaName_);

	bool bSetBuf = _pdb->ExecSetBuffer(_T("AttachData"),pAttaData_,nAttaSize_,_T("CaseAttachment"),
		strSqlWhere.GetBuffer());

	if (!bSetBuf)
	{
		_strLastError = _T("设置案例附件BUFFER数据失败！");
		CMyLog::Trace_Log(_strLastError.GetBuffer());
		return -1;
	}

	/*_pdb->Cmd("select * from  CaseAttachment where CaseID=%d and AttachName='%s'",nCaseID_,strAttaName_);

	if (_pdb->More(TRUE))
	{
		_strLastError = "";
		bool bRet = _pdb->SetBuffer("AttachData",pAttaData_,nAttaSize_);
		if (!bRet)
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		ASSERT(FALSE);
		_strLastError = _pdb->GetLastErrMsg();
		return -1;
	}*/
	return 0;
}

int CConnDB::SubmitCase( int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	// 案例状态修改为未审核
	CString strVerifyStatus;
	strVerifyStatus.Format("update CaseBasicInfo set CaseVerifyStatus=0 where CaseID=%d",nCaseID_);
	_pdb->Cmd(strVerifyStatus.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strVerifyStatus;
		return -1;
	}
	return 0;
}

int CConnDB::GetCaseFault( vector<ST_CaseFault>& vCaseFault_,int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	vCaseFault_.clear();
	CString sql;
	sql.Format("select * from CaseFault where CaseID=%d",nCaseID_);

	_pdb->Cmd(sql.GetBuffer());

	while(_pdb->More())
	{
		ST_CaseFault fault;
		fault._strFaultID	= _pdb->Get("FaultID").c_str();

		vCaseFault_.push_back(fault);
		
	}
	return 0;
}

int CConnDB::GetCaseSymptom( vector<ST_CaseSymptom>& vCaseSymptom_,int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString sql;
	sql.Format("select * from CaseSymptomInfo where CaseID=%d",nCaseID_);

	_pdb->Cmd(sql.GetBuffer());

	while(_pdb->More())
	{
		ST_CaseSymptom symptom;
		symptom._fImportanceDegree	= _pdb->GetDouble("ImportantDegree");
		symptom._strSymptomID		= _pdb->Get("SymptomID").c_str();
		symptom._nSymptomValueID	= (int)_pdb->GetLong("SymptomValueID");
		symptom._nCaseSymptomCount	= (int)_pdb->GetLong("CaseSymptomCount");
		symptom._nSymptomIndex		= (int)_pdb->GetLong("SymptomIndex");
		CString strWeightMatrix;
		strWeightMatrix.Format("%s",_pdb->Get("SymptomRowImportanceData").c_str());

		CString strTemp = "";
		for (int i=0;i<strWeightMatrix.GetLength();i++)
		{
			if (strWeightMatrix.GetAt(i) == CASE_SYMPTOM_WEIGHT_MATRIX_SPLIT)
			{
				int nTemp = atoi(strTemp);
				symptom._vRelaWeight.push_back(nTemp);
				strTemp = "";
			}
			else
			{
				strTemp += strWeightMatrix.GetAt(i);
			}
		}

		vCaseSymptom_.push_back(symptom);
	}
	return 0;
}

int CConnDB::GetCaseAttachRecord( vector<ST_Attachment>& vCaseAtta_ ,int nCaseID_)
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString sql;
	sql.Format("select * from CaseAttachment where CaseID=%d",nCaseID_);

	_pdb->Cmd(sql.GetBuffer());

	while(_pdb->More())
	{
		ST_Attachment atta;
		atta._eType			= (E_ATTACHMENT_TYPE)_pdb->GetLong("AttachType");
		atta._lFileSize		= _pdb->GetLong("AttachSize");
		atta._strFile.Format("%s",_pdb->Get("FileName").c_str());
		atta._strChann.Format("%s",_pdb->Get("ChannInfo").c_str());
		atta._strName.Format("%s",_pdb->Get("AttachName").c_str());

		//long lSize = atta._lFileSize;
		//atta._pbFileData	= new BYTE[lSize];
		//_pdb->GetBuffer("AttachData",(unsigned char*)atta._pbFileData,lSize);

		//ASSERT(lSize == atta._lFileSize);
		//assert(lSize == atta._lFileSize);

		atta._strFileSize	= atta.GetFileSizeStr();
		atta._strType		= atta.GetTypeStr();
	
		atta._pbFileData = NULL;

		vCaseAtta_.push_back(atta);
	}
	return 0;
}

int CConnDB::GetCaseAttachData( ST_Attachment& caseAtta_,int nCaseID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString sql;
	sql.Format("select * from CaseAttachment where CaseID=%d and AttachName='%s'",nCaseID_,caseAtta_._strName);

	_pdb->Cmd(sql.GetBuffer());

	CWaveHead sWaveHead;

	while(_pdb->More())
	{
		caseAtta_._eType			= (E_ATTACHMENT_TYPE)_pdb->GetLong("AttachType");
		caseAtta_._lFileSize		= _pdb->GetLong("AttachSize");
		caseAtta_._strFile.Format("%s",_pdb->Get("FileName").c_str());
		caseAtta_._strChann.Format("%s",_pdb->Get("ChannInfo").c_str());
		caseAtta_._strName.Format("%s",_pdb->Get("AttachName").c_str());
		long lSize = caseAtta_._lFileSize;
		caseAtta_._pbFileData	= new BYTE[lSize];
		_pdb->GetBuffer("AttachData",caseAtta_._pbFileData,lSize,sWaveHead);

		ASSERT(lSize == caseAtta_._lFileSize);
	}
	return 0;
}
int CConnDB::SetCaseVerifyStatus( int nCaseID_,int nStatus_,CString strUserName_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CBHDateTime dtNow = CBHDateTime::GetCurrentTime();
	CString strDtNow = dtNow.Format(COLEDATETIME_FORMAT_STR_DATETIME);

	CString strSql;
	strSql.Format("update CaseBasicInfo set CaseVerifyStatus=%d,CaseVerifyUser='%s',CaseVerifyTime='%s' where CaseID=%d",nStatus_,strUserName_,strDtNow,nCaseID_);

	_pdb->Cmd(strSql.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError.Format("%s",_pdb->GetLastErrMsg());
		return -1;
	}
	return 0;
}

int CConnDB::AddMaintenanceRecord( int& nRecordID_,CString strName_,
		CString strPlantNO_,CString strFactory_,CString strCompany_,
		E_MAINTENANCE_TYPE eType_,
		CString strSection_,CBHDateTime dtBegin_,CBHDateTime dtEnd_, CString strConent_,CString strReason_,
		CString strFaultID_, CString strResult_,CString strMainPeople,CString strOtherPeople_, CString strRemark_,
		CBHDateTime dtAdd_,CString strRecorder_ )
{
	if (IsMaintenanceRecordNameExist(strName_))
	{
		_strLastError = "该记录名称已存在";
		return -1;
	}
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	
	nRecordID_ = 1;
	CString strSqlGetID;
	strSqlGetID.Format("select max(RecordID) as 'ID' from MaintenanceRecord");

	_pdb->Cmd(strSqlGetID.GetBuffer());
	if (_pdb->More())
	{
		nRecordID_ = (int)_pdb->GetLong("ID");
	}
	nRecordID_ ++;
	CString strStartTime = dtBegin_.Format();
	CString strEndTime	 = dtEnd_.Format();
	CString strRecordTime = dtAdd_.Format();

	CString strSqlInsert;
	strSqlInsert.Format("insert into MaintenanceRecord \
		(RecordID,RecordName,Company,Factory,PlantNO,MaintenanceType, \
		PlantPosition,StartTime,EndTime,MaintenanceContent,MaintenanceReason, \
		FaultID,MaintenanceResult,MainPerson,OtherPerson,Remark, \
		RecordTime,Recorder) values \
		(%d,'%s','%s','%s','%s',%d, \
		'%s','%s','%s','%s','%s', \
		'%s','%s','%s','%s','%s', \
		'%s','%s')",
		nRecordID_,strName_,strCompany_,strFactory_,strPlantNO_,eType_,
		strSection_,strStartTime,strEndTime,strConent_,strReason_,
		strFaultID_,strResult_,strMainPeople,strOtherPeople_,strRemark_,
		strRecordTime,strRecorder_
		);

	_pdb->Cmd(strSqlInsert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strSqlInsert;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteMaintenanceRecord_AttachmentInfo( int nRecordID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strDel;
	strDel.Format("delete from MaintenanceRecordAttachment where MaintenanceRecordID=%d",nRecordID_);
	_pdb->Cmd(strDel.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strDel;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteMaintenanceRecord( int nRecordID_ )
{
	int nRet = this->DeleteMaintenanceRecord_AttachmentInfo(nRecordID_);
	if (0 == nRet)
	{
		if (!Conn())
		{
			_strLastError = "中间件连接失败！";
			return -1;
		}
		CString strDelete;
		strDelete.Format("delete from MaintenanceRecord where RecordID=%d",nRecordID_);
		_pdb->Cmd(strDelete.GetBuffer());
		if (!_pdb->Exec())
		{
			_strLastError = strDelete;
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int CConnDB::AddMaintenanceRecord_AttachmentInfo( int nRecordID_,CString strName_, int nType_,BYTE* pData_,long lDataSize_,CString strFileName_, CString strChann_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString sqlInsert;
	sqlInsert.Format(_T("insert into MaintenanceRecordAttachment \
		(MaintenanceRecordID,AttachName,AttachType,AttachSize,FileName,ChannInfo) values \
		(%d,'%s',%d,%d,'%s','%s')"),
		nRecordID_,strName_,nType_,lDataSize_,strFileName_,strChann_
		);
	_pdb->Cmd(sqlInsert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = sqlInsert;
		return -1;
	}

	CString strSqlWhere;
	strSqlWhere.Format(_T("MaintenanceRecordID=%d and AttachName='%s'"), nRecordID_,strName_);

	bool bSetBuf = _pdb->ExecSetBuffer(_T("AttachData"),pData_,lDataSize_,_T("MaintenanceRecordAttachment"),strSqlWhere.GetBuffer());
	if (!bSetBuf)
	{
		_strLastError = _T("设置检维修记录的BUFFER数据失败！");
		return -1;
	}

	/*CString strSelect;
	strSelect.Format(_T("select * from MaintenanceRecordAttachment where \
					 MaintenanceRecordID=%d and \
					 AttachName='%s' \
					 "),
					 nRecordID_,strName_
					 );
	_pdb->Cmd(strSelect.GetBuffer());
	while(_pdb->More(TRUE))
	{
		bool bRet = _pdb->SetBuffer(_T("AttachData"),pData_,lDataSize_);
		if (!bRet)
		{
			_strLastError = _T("Setbuffer失败！");
			return -1;
		}
	}*/
	return 0;
}

int CConnDB::UpdateMaintenanceRecord( int nRecordID_,CString strName_,
					CString strPlantNO_,CString strFactory_,CString strCompany_,
					E_MAINTENANCE_TYPE eType_, CString strSection_,
					CBHDateTime dtBegin_,CBHDateTime dtEnd_, CString strConent_,
					CString strReason_,CString strFaultID_, CString strResult_,CString strMainPeople,CString strOtherPeople_, CString strRemark_,CBHDateTime dtAdd_,CString strRecorder_ )
{

	if (IsMaintenanceRecordNameExist(strName_,nRecordID_))
	{
		_strLastError = "该记录名称已存在";
		return -1;
	}
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}

	CString strStartTime = dtBegin_.Format();
	CString strEndTime	 = dtEnd_.Format();
	CString strRecordTime = dtAdd_.Format();

	CString strSqlUpdate;
	strSqlUpdate.Format("update MaintenanceRecord set \
		RecordName='%s',Company='%s',Factory='%s',PlantNO='%s',MaintenanceType=%d, \
		PlantPosition='%s',StartTime='%s',EndTime='%s',MaintenanceContent='%s',MaintenanceReason='%s', \
		FaultID='%s',MaintenanceResult='%s',MainPerson='%s',OtherPerson='%s',Remark='%s', \
		Recorder='%s' where RecordID=%d",
		strName_,strCompany_,strFactory_,strPlantNO_,eType_,
		strSection_,strStartTime,strEndTime,strConent_,strReason_,
		strFaultID_,strResult_,strMainPeople,strOtherPeople_,strRemark_,
		strRecorder_,nRecordID_
		);

	_pdb->Cmd(strSqlUpdate.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strSqlUpdate;
		return -1;
	}
	return 0;
}

int CConnDB::QueryMaintenanceRecord( vector<ST_Maintenance_Record_Info>& vRecord_, 
			CString strPlantNO_/*=""*/,CString strFactory_/*=""*/,
			CString strCompany_/*=""*/, CBHDateTime dtBegin_/*=COLEDATETIME_NULL*/,
			CBHDateTime dtEnd_/*=COLEDATETIME_NULL*/ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strQuery;
	strQuery.Format("select * from MaintenanceRecord where 1=1");
	if (!strPlantNO_.IsEmpty())
	{
		CString strTemp;
		strTemp.Format(" and PlantNO='%s' ",strPlantNO_);
		strQuery += strTemp;
	}
	if (!strFactory_.IsEmpty())
	{
		CString strTemp;
		strTemp.Format(" and Factory='%s' ",strFactory_);
		strQuery += strTemp;
	}
	if (!strCompany_.IsEmpty())
	{
		CString strTemp;
		strTemp.Format(" and Company='%s' ",strCompany_);
		strQuery += strTemp;
	}
	if (COLEDATETIME_NULL != dtBegin_)
	{
		CString strTemp;
		strTemp.Format(" and StartTime>='%s' ",dtBegin_.Format(COLEDATETIME_FORMAT_STR_DATETIME_BEGIN));
		strQuery += strTemp;
	}
	if (COLEDATETIME_NULL != dtEnd_)
	{
		CString strTemp;
		strTemp.Format(" and EndTime<='%s' ",dtEnd_.Format(COLEDATETIME_FORMAT_STR_DATETIME_END));
		strQuery += strTemp;
	}
	_pdb->Cmd(strQuery.GetBuffer());
	while (_pdb->More())
	{
		ST_Maintenance_Record_Info info;
		info._nRecordID			= (int)_pdb->GetLong("RecordID");				///< 记录ID
		info._strRecordName		= CString(_pdb->Get("RecordName").c_str());		///< 记录名称
		//info._strPlantName;		= g_pDB->Get("");	///< 设备名称
		info._strPlantNO		= CString(_pdb->Get("PlantNO").c_str());			///< 设备号
		info._strFactory		= CString(_pdb->Get("Factory").c_str());			///< 分厂名称
		info._strCompany		= CString(_pdb->Get("Company").c_str());			///< 公司名称

		info._eMaintenanceType	= (E_MAINTENANCE_TYPE)_pdb->GetLong("MaintenanceType");		///< 检修类型

		info._strSection		= CString(_pdb->Get("PlantPosition").c_str());			///< 检修部位
		info._dtBegin			= _pdb->GetTime("StartTime");///< 检修开始时间
		info._dtEnd				= _pdb->GetTime("EndTime");				///< 检修结束时间
		info._strConent			= CString(_pdb->Get("MaintenanceContent").c_str());				///< 检修内容
		info._strFaultReason	= CString(_pdb->Get("MaintenanceReason").c_str());		///< 检修故障原因描述
		info._strFaultID		= CString(_pdb->Get("FaultID").c_str());			///< 检修故障原因ID
		info._strResult			= CString(_pdb->Get("MaintenanceResult").c_str());				///< 检修结果
		info._strMainPeople		= CString(_pdb->Get("MainPerson").c_str());			///< 主检修员	
		info._strOtherPeople	= CString(_pdb->Get("OtherPerson").c_str());		///< 其它检修人员
		info._strRemark			= CString(_pdb->Get("Remark").c_str());				///< 备注
		info._strRecorder		= CString(_pdb->Get("Recorder").c_str());			///< 记录人员
		info._dtRecordAdd		= _pdb->GetTime("RecordTime");		///< 记录添加时间
		vRecord_.push_back(info);
	}
	return 0;
}

int CConnDB::QueryMaintenanceRecord_Attachment( vector<ST_Attachment>& vAtta_,int nRecordID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strQuery;
	strQuery.Format("select * from MaintenanceRecordAttachment where MaintenanceRecordID=%d",nRecordID_);
	_pdb->Cmd(strQuery.GetBuffer());
	CWaveHead sWaveHead;
	while(_pdb->More())
	{
		ST_Attachment atta;
		atta._strName	= CString(_pdb->Get("AttachName").c_str());
		atta._eType = (E_ATTACHMENT_TYPE)_pdb->GetLong("AttachType");
		atta._lFileSize = _pdb->GetLong("AttachSize");
		atta._strFile = CString(_pdb->Get("FileName").c_str());
		atta._strChann = CString(_pdb->Get("ChannInfo").c_str());
		long nBufSize = atta._lFileSize;
		atta._pbFileData = new BYTE[nBufSize];
		_pdb->GetBuffer("AttachData",atta._pbFileData,nBufSize,sWaveHead);
		//ASSERT(nBufSize == atta._lFileSize);
		vAtta_.push_back(atta);
	}
	return 0;
}

BOOL CConnDB::IsMaintenanceRecordNameExist( CString strName_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	BOOL bRet = FALSE;
	CString strQuery;
	strQuery.Format("select * from MaintenanceRecord where RecordName='%s'",strName_);
	_pdb->Cmd(strQuery.GetBuffer());
	while(_pdb->More())
	{
		/*int nCount = _pdb->GetCount()
		if (nCount > 0)*/
		{	
			bRet = TRUE;
			break;
		}	
	}
	return bRet;
}

BOOL CConnDB::IsMaintenanceRecordNameExist( CString strName_,int nRecordID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	BOOL bRet = FALSE;
	CString strQuery;
	strQuery.Format("select * from MaintenanceRecord where RecordName='%s' and RecordID != %d",strName_,nRecordID_);
	_pdb->Cmd(strQuery.GetBuffer());
	while(_pdb->More())
	{
		{	
			bRet = TRUE;
			break;
		}	
	}
	return bRet;	
}

int CConnDB::AddSudRecord( int& nSudRecordID_, CString strCompany_, CString strFactory_, CString strPlantNO_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, CString strSudReason_, CString strFaultID_, CString strRemark_, CString strRecorder_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	nSudRecordID_ = 1;
	CString strSql_GetID = "select max(SUDRecordID)+ 1  as 'count'  from SUDRecord";
	_pdb->Cmd(strSql_GetID.GetBuffer());
	if (_pdb->More())
	{
		nSudRecordID_ = (int)_pdb->GetLong("count");
	}
	CString strSUDTime = dtSudTime_.Format(COLEDATETIME_FORMAT_STR_DATETIME);
	CString strSql_Insert;
	strSql_Insert.Format("insert into SUDRecord(SUDRecordID,Company,Factory,PlantNO,SudRecordTime,SudType,Reason,FaultID,Remark,Recorder)    \
		values(%d,'%s','%s','%s','%s',%d,'%s','%s','%s','%s')",
		nSudRecordID_,strCompany_,strFactory_,strPlantNO_,strSUDTime,eSudType_,strSudReason_,strFaultID_,strRemark_,strRecorder_);
	_pdb->Cmd(strSql_Insert.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strSql_Insert;
		return -1;
	}
	return 0;
}

int CConnDB::UpdateSudRecord( int nSudRecordID_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, CString strSudReason_, CString strFaultID_, CString strRemark_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	CString strSql_update;
	CString strSUDTime = dtSudTime_.Format(COLEDATETIME_FORMAT_STR_DATETIME);
	strSql_update.Format("update SUDRecord set SudRecordTime='%s',SudType=%d,Reason='%s',FaultID='%s',Remark='%s' where SUDRecordID=%d",
		strSUDTime,eSudType_,strSudReason_,strFaultID_,strRemark_,nSudRecordID_);
	_pdb->Cmd(strSql_update.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strSql_update;
		return -1;
	}
	return 0;
}

int CConnDB::DeleteSudRecord( int nRecordID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	CString strSql_delete;
	strSql_delete.Format("delete from SUDRecord where SUDRecordID=%d",nRecordID_);
	_pdb->Cmd(strSql_delete.GetBuffer());
	if (!_pdb->Exec())
	{
		_strLastError = strSql_delete;
		return -1;
	}
	return 0;
}

int CConnDB::QuerySudRecord( vector<ST_SUD_Record_Info>& recordList_,CString strPlantNO/*=""*/,
		CString strFactory/*=""*/, CString strCompany_/*=""*/,CString strFaultID_/*=""*/,
		CBHDateTime dtBegin_/*=COLEDATETIME_NULL*/, CBHDateTime dtEnd_/*=COLEDATETIME_NULL*/ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return TRUE;
	}
	recordList_.clear();
	CString strSql_Query = "select * from SUDRecord where 1=1 ";
	if (!strPlantNO.IsEmpty())
	{
		strSql_Query += " and PlantNO='"+strPlantNO+"'";
	}
	if (!strFactory.IsEmpty())
	{
		strSql_Query += " and Factory='"+strFactory+"'";
	}
	if (!strCompany_.IsEmpty())
	{
		strSql_Query += " and Company='"+strCompany_+"'";
	}
	if (!strFaultID_.IsEmpty())
	{
		strSql_Query += " and FaultID='"+strFaultID_+"'";
	}
	if (dtBegin_ != COLEDATETIME_NULL)
	{
		strSql_Query += " and SudRecordTime>='" + dtBegin_.Format(COLEDATETIME_FORMAT_STR_DATETIME_BEGIN) + "'";	
	}
	if (dtEnd_ != COLEDATETIME_NULL)
	{
		strSql_Query += " and SudRecordTime<='" + dtEnd_.Format(COLEDATETIME_FORMAT_STR_DATETIME_END) + "'";	
	}
	_pdb->Cmd(strSql_Query.GetBuffer());
	while(_pdb->More())
	{
		ST_SUD_Record_Info r;
		r._dtSudTime = _pdb->GetTime("SudRecordTime");
		r._eSudType = (E_SUD_TYPE)_pdb->GetLong("SudType");
		r._nSudRecordID = (int)_pdb->GetLong("SUDRecordID");
		r._strCompany = CString(_pdb->Get("Company").c_str());
		r._strFactory = CString(_pdb->Get("Factory").c_str());
		r._strPlantNO = CString(_pdb->Get("PlantNO").c_str());
		r._strFaultID = CString(_pdb->Get("FaultID").c_str());
		r._strRecorder = CString(_pdb->Get("Recorder").c_str());
		r._strRemark = CString(_pdb->Get("Remark").c_str());
		r._strSudReason = CString(_pdb->Get("Reason").c_str());
		recordList_.push_back(r);
	}
	return 0;
}

BOOL CConnDB::TestDBCon()
{
	BOOL bRet = FALSE;
	//if (NULL != this->g_pDB)
	//{
	//	CString strSql = "select count(*) from casebasicinfo";
	//	g_pDB->Cmd(strSql.GetLength() + 100,strSql.GetBuffer());

	//	int nCount = -1;

	//	if (g_pDB->Exec())
	//	{
	//		nCount = g_pDB->GetLong(0);
	//	}
	//	bRet = (nCount >= 0);
	//}
	return bRet;
}

int CConnDB::QueryMaintenanceRecord_AttachmentData( ST_Attachment& atta_,int nRecordID_ )
{
	if (!Conn())
	{
		_strLastError = "中间件连接失败！";
		return -1;
	}
	CString strQuery;
	CWaveHead sWaveHead;
	strQuery.Format("select * from MaintenanceRecordAttachment where MaintenanceRecordID=%d and AttachName='%s'",nRecordID_,atta_._strName);
	_pdb->Cmd(strQuery.GetBuffer());
	while(_pdb->More())
	{
		atta_._strName	= _pdb->Get("AttachName").c_str();
		atta_._eType = (E_ATTACHMENT_TYPE)_pdb->GetLong("AttachType");
		atta_._lFileSize = _pdb->GetLong("AttachSize");
		atta_._strFile = _pdb->Get("FileName").c_str();
		atta_._strChann = _pdb->Get("ChannInfo").c_str();
		atta_._lFileSize = _pdb->GetLong("AttachSize");
		long nBufSize = atta_._lFileSize;
		atta_._pbFileData = new BYTE[nBufSize];
		_pdb->GetBuffer("AttachData",atta_._pbFileData,nBufSize,sWaveHead);
		//ASSERT(nBufSize == atta._lFileSize);
		
	}
	return 0;	
}