#include "StdAfx.h"

#ifdef _PIDMS
#include "../S_Inc/debug.h"
#endif

#include <CaseLibrary/CaseHandle.h>
#include <CaseLibrary/CaseSvcDef.h>
#include <CaseLibrary/CaseSvcField.h>
#include <CaseLibrary/Glbs.h>
#include <CaseLibrary/MyLog.h>
#include <CaseLibrary/ConnDB.h>
#include <CaseLibrary/BufferCache.h>
#include <CaseLibrary/BHCaseLibrary_Define.h>
#include <algorithm>

// 使用宏替换中间件向客户端传递的处理结果标志
int GetConnMWStatus(int nRet)
{
	if(0 == nRet)
	{
		return CONNMW_SUCCESS;
	}
	else
	{
		return CONNMW_FAIL;
	}
}
float CalcSimilarity(const vector<ST_Symptom>& vSource_,const vector<ST_CaseSymptom>& vTarget_)
{
	int nSameSamptomCount = 0;
	float fSim = 0.0;

	for (vector<ST_CaseSymptom>::const_iterator it=vTarget_.begin();it != vTarget_.end();++it)
	{
		for (vector<ST_Symptom>::const_iterator itTo=vSource_.begin();itTo != vSource_.end();++itTo)
		{
			if (it->_strSymptomID == itTo->_strSymptomID &&
				it->_nSymptomValueID == itTo->_nSymptomValueID)
			{
				fSim += (1-abs(it->_fImportanceDegree - itTo->_fImportanceDegree));
				nSameSamptomCount ++;
			}
		}
	}	
	fSim /= (float)vTarget_.size();

	return fSim;
}

int CCaseHandle::GetTestInt( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	return 0;
}



bool CCaseHandle::CaseAddBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;

	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_NAME]			= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_NAME);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_DETAIL]		= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_DETAIL);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_TIME]			= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_INPUT_USER]	= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_INPUT_USER);

	dataIn.Trace();


	string strError = "";
	int nCaseID = -1;
	int nRet = -1;
	CConnDB db(pDB);
	nRet = db.AddCase(
		dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()			,
		dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()		,
		dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		,
		dataIn._mapString[FIELD_CASELIBRARY_CASE_NAME].c_str()		,
		dataIn._mapString[FIELD_CASELIBRARY_CASE_DETAIL].c_str()	,
		dataIn._mapString[FIELD_CASELIBRARY_CASE_TIME].c_str()		,
		dataIn._mapString[FIELD_CASELIBRARY_CASE_INPUT_USER].c_str(),
		nCaseID);

	if (0 != nRet)
	{
		strError = string(db.GetLastError().GetBuffer());
	}

	
	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp._mapInteger[FIELD_CASELIBRARY_CASE_ID] = nCaseID;
	
	temp.Trace();

	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}


bool CCaseHandle::CaseUpdateBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_UPDATE_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_NAME]		= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_NAME);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_DETAIL]	= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_DETAIL);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]			= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]		= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]		= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);

	dataIn.Trace();
	// 访问数据库
	
	int nRet = -1;
	CString strErrorMsg = "";

	
	{
		CConnDB db(pDB);
		CBHDateTime dtCase;
		dtCase.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_CASE_TIME].c_str());
		nRet = db.UpdateCase_BasicInfo(
			dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]				,
			dataIn._mapString[FIELD_CASELIBRARY_CASE_NAME].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_CASE_DETAIL].c_str()	,	
			dtCase														,	
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()			,
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()		,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		
			);
		if (0 != nRet)
		{
			strErrorMsg = db.GetLastError();
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;

	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strErrorMsg;

	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseAddFaultInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_ADD_FAULT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID] = pFixComm.GetItem(FIELD_CASELIBRARY_FAULT_ID);
	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);

		string strFaultID = dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID];
		
		nRet = db.AddCaseFault(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID],strFaultID.c_str());

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseSubmit( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_SUBMIT);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);

		nRet = db.SubmitCase(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseVerify( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_VERIFY);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]				= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS]			= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_STATUS);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_VERIFY_USERNAME]	= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_VERIFY_USERNAME); 

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = ""; 
	
	{
		CConnDB db(pDB);
		nRet = db.SetCaseVerifyStatus(
			dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID],
			dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS],
			dataIn._mapString[FIELD_CASELIBRARY_CASE_VERIFY_USERNAME].c_str()
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseDelAttachmentInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_DEL_ATTACHMENT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		nRet = db.DeleteCaseAttachment(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseDelSymptomInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_DEL_SYMPTOM_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		db.DeleteCaseSymptom(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseDelFaultInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_DEL_FAULT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		db.DeleteCaseFault(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseDel( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_DEL);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		nRet = db.DeleteCaseAttachment(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);
		nRet = db.DeleteCaseFault(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);
		nRet = db.DeleteCaseSymptom(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);
		nRet = db.DeleteCaseBasicInfo(dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseQueryBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_QUERY_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS]		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_STATUS);
	dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID]			= pFixComm.GetItem(FIELD_CASELIBRARY_FAULT_ID);
	dataIn._mapInteger[FIELD_CASELIBRARY_SYMPTOM_ID]		= pFixComm.GetLong(FIELD_CASELIBRARY_SYMPTOM_ID);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_ID_ARRAY]		= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_ID_ARRAY);
	
	CBHDateTime dtBegin,dtEnd;
	string strBegin = pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_BEGIN_TIME);
	string strEnd = pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_END_TIME);

	dtBegin.ParseDateTime(strBegin.c_str());
	dtEnd.ParseDateTime(strEnd.c_str());

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	std::vector<ST_Case_Info> vCaseList;

	CBHDateTime dtNow = CBHDateTime::GetCurrentTime();
	CMyLog::Trace_Log(_T("案例查询时间"));
	CMyLog::Trace_Log(dtNow.Format().GetBuffer());
	
	{
		std::vector<int> vCaseID;

		CConnDB db(pDB);
		E_CASE_VERIFY_STATUS leStatus = (E_CASE_VERIFY_STATUS)dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS];
		nRet = db.QueryCase(vCaseList,
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()			,
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()		,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		,
			leStatus													,
			dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID].c_str()		,
			dataIn._mapInteger[FIELD_CASELIBRARY_SYMPTOM_ID]			,
			dataIn._mapString[FIELD_CASELIBRARY_CASE_ID_ARRAY].c_str()	,
			dtBegin,dtEnd
			);
		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	dtNow = CBHDateTime::GetCurrentTime();
	CMyLog::Trace_Log(dtNow.Format().GetBuffer());

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vCaseList.size();++i)
	{
		ST_Case_Info lcase = vCaseList.at(i);
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		temp._mapInteger[FIELD_CASELIBRARY_CASE_ID]		= lcase._nCaseID;
		temp._mapString[FIELD_CASELIBRARY_CASE_NAME]	= lcase._strCaseName;
		temp._mapString[FIELD_CASELIBRARY_CASE_DETAIL]  = lcase._strCaseDetail.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_COMPANY]		= lcase._strCompany.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_FACTORY]		= lcase._strFactory.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_PLATNO]		= lcase._strPlantNO.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_TIME]	= lcase._dtCase.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_INPUT_USER]	= lcase._strAddUserName;
		temp._mapString[FIELD_CASELIBRARY_CASE_INPUT_TIME]	= lcase._dtAddTime.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_VERIFY_TIME]	= lcase._dtVerify.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapInteger[FIELD_CASELIBRARY_CASE_STATUS]	   	= lcase._eCaseCheckStatus;
		vecOut.push_back(temp);

		temp.Trace();
	}

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseQueryFaultInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_QUERY_FAULT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	std::vector<ST_CaseFault> vCaseFault;

	 
	
	{
		CConnDB db(pDB);
		nRet = db.GetCaseFault(vCaseFault,dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vCaseFault.size();++i)
	{
		ST_CaseFault lfault = vCaseFault.at(i);
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
			temp._mapInteger[FIELD_CASELIBRARY_CASE_ID] = lfault._nCaseID;
		}
		temp._mapString[FIELD_CASELIBRARY_FAULT_ID] = lfault._strFaultID;
		temp.Trace();
		vecOut.push_back(temp);
	}

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::CaseQuerySymptomInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_QUERY_SYMPTOM_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	std::vector<ST_CaseSymptom> vSymptom;

	 
	
	{
		CConnDB db(pDB);
		nRet = db.GetCaseSymptom(vSymptom,dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vSymptom.size();++i)
	{
		ST_CaseSymptom lSymptom = vSymptom.at(i);
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
			temp._mapInteger[FIELD_CASELIBRARY_CASE_ID] = lSymptom._nCaseID;
		}
		temp._mapString[FIELD_CASELIBRARY_SYMPTOM_ID]			= lSymptom._strSymptomID;
		temp._mapInteger[FIELD_CASELIBRARY_SYMPTOM_VALUE_ID]	= lSymptom._nSymptomValueID;
		temp._mapDouble[FIELD_CASELIBRARY_SYMPTOM_IMPORTANT_DEGREE]	= lSymptom._fImportanceDegree;
		temp.Trace();
		vecOut.push_back(temp);
	}

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::MaintenanceRecordAddBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]						= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]					= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]					= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_TYPE]			= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_TYPE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_SECTION]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_SECTION);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_END_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_END_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_CONTENT]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_CONTENT);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REASON]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_REASON);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RESULT]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RESULT);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REMARK]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_REMARK);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME]= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORDER]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RECORDER);


	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	int nRecordID = -1;

	 
	
	{
		CBHDateTime dtBegin,dtEnd,dtAdd;
		dtBegin.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME].c_str());
		dtEnd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_END_TIME].c_str());
		dtAdd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME].c_str());

		CConnDB db(pDB);
		nRet = db.AddMaintenanceRecord(
			nRecordID,
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()							,						
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()						,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()						,					
			(E_MAINTENANCE_TYPE)dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_TYPE]						,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_SECTION].c_str()			,		
			dtBegin,		
			dtEnd,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_CONTENT].c_str()			,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REASON].c_str()				,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID].c_str()			,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RESULT].c_str()				,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REMARK].c_str()				,			
			dtAdd,
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORDER].c_str()					
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID] = nRecordID;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}
bool CCaseHandle::MaintenanceRecordUpdateBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_UPDATE_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]		= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]						= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]					= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]					= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_TYPE]			= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_TYPE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_SECTION]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_SECTION);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_END_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_END_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_CONTENT]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_CONTENT);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REASON]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_REASON);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RESULT]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RESULT);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE]	= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REMARK]			= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_REMARK);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME]= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORDER]		= pFixComm.GetItem(FIELD_CASELIBRARY_MAINTENANCE_RECORDER);


	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	
	 
	
	{
		CBHDateTime dtBegin,dtEnd,dtAdd;
		dtBegin.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME].c_str());
		dtEnd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_END_TIME].c_str());
		dtAdd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME].c_str());
		CConnDB db(pDB);
		nRet = db.UpdateMaintenanceRecord(
			dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]					,
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()							,						
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()						,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()						,					
			(E_MAINTENANCE_TYPE)dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_TYPE]						,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_SECTION].c_str()			,		
			dtBegin,		
			dtEnd,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_CONTENT].c_str()			,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REASON].c_str()				,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID].c_str()			,		
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RESULT].c_str()				,			
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_REMARK].c_str()				,			
			dtAdd,
			dataIn._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORDER].c_str()			
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::MaintenanceRecordDelAttachmentInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_ATTACHMENT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		nRet = db.DeleteMaintenanceRecord_AttachmentInfo(dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::MaintenanceRecordDelBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		nRet = db.DeleteMaintenanceRecord(dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::MaintenanceRecordQueryBasicInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_BASIC_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]					= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]				= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]				= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapString[FIELD_CASELIBRARY_QUERY_BEGIN_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_BEGIN_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_QUERY_END_TIME]			= pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_END_TIME);

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	std::vector<ST_Maintenance_Record_Info> vRecord;

	 
	
	{
		CConnDB db(pDB);

		CBHDateTime dtBegin,dtEnd;
		dtBegin.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_QUERY_BEGIN_TIME].c_str());
		dtEnd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_QUERY_END_TIME].c_str());
		nRet = db.QueryMaintenanceRecord(
			vRecord,
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()		,					
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()	,				
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()	,				
			dtBegin,		
			dtEnd			
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vRecord.size();++i)
	{
		ST_Maintenance_Record_Info record = vRecord.at(i);
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		temp._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]		= record._nRecordID; 
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORD_NAME]      = record._strRecordName;
		temp._mapString[FIELD_CASELIBRARY_PLATNO]				        = record._strPlantNO;
		temp._mapString[FIELD_CASELIBRARY_FACTORY]				        = record._strFactory;
		temp._mapString[FIELD_CASELIBRARY_COMPANY]						= record._strCompany;
		temp._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_TYPE]			= record._eMaintenanceType;  
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_SECTION]		    = record._strSection;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_BEGIN_TIME]		= record._dtBegin.Format(DATE_TIME_FORMAT);  
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_END_TIME]		    = record._dtEnd.Format(DATE_TIME_FORMAT);
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_CONTENT]		    = record._strConent;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_REASON]			= record._strFaultReason;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_FAULT_ID]		    = record._strFaultID;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_RESULT]			= record._strResult;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_MAIN_PEOPLE]	    = record._strMainPeople;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_OTHER_PEOPLE]	    = record._strOtherPeople;  
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_REMARK]			= record._strRemark;
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_ADD_RECORD_TIME]  = record._dtRecordAdd.Format(DATE_TIME_FORMAT);
		temp._mapString[FIELD_CASELIBRARY_MAINTENANCE_RECORDER]		    = record._strRecorder;

		temp.Trace();
		vecOut.push_back(temp);
	}

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::SudRecordAdd( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_SUD_RECORD_ADD);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_TIME]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_TIME);
	dataIn._mapInteger[FIELD_CASELIBRARY_SUD_TYPE]			= pFixComm.GetLong(FIELD_CASELIBRARY_SUD_TYPE);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_REASON]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_REASON);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_FAULT_ID]		= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_FAULT_ID);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_REMARK]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_REMARK);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_RECORDER]		= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_RECORDER);
	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	int nReocrdID = -1;

	 
	
	{
		CBHDateTime dtSudTime;
		dtSudTime.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_SUD_TIME].c_str());
		E_SUD_TYPE eType = (E_SUD_TYPE)dataIn._mapInteger[FIELD_CASELIBRARY_SUD_TYPE];
		CConnDB db(pDB);
		nRet = db.AddSudRecord(
			nReocrdID,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		,  
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()		,  
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()			,  
			dtSudTime,  
			eType,	
			dataIn._mapString[FIELD_CASELIBRARY_SUD_REASON].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_SUD_FAULT_ID].c_str()	,	
			dataIn._mapString[FIELD_CASELIBRARY_SUD_REMARK].c_str()		,	
			dataIn._mapString[FIELD_CASELIBRARY_SUD_RECORDER].c_str()		
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID] = nReocrdID;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::SudRecordUpdate( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_SUD_RECORD_UPDATE);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID]		= pFixComm.GetLong(FIELD_CASELIBRARY_SUD_RECORD_ID);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_TIME]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_TIME);
	dataIn._mapInteger[FIELD_CASELIBRARY_SUD_TYPE]			= pFixComm.GetLong(FIELD_CASELIBRARY_SUD_TYPE);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_REASON]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_REASON);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_FAULT_ID]		= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_FAULT_ID);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_REMARK]			= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_REMARK);
	dataIn._mapString[FIELD_CASELIBRARY_SUD_RECORDER]		= pFixComm.GetItem(FIELD_CASELIBRARY_SUD_RECORDER);
		
	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CBHDateTime dtSudTime;
		dtSudTime.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_SUD_TIME].c_str());
		E_SUD_TYPE eType = (E_SUD_TYPE)dataIn._mapInteger[FIELD_CASELIBRARY_SUD_TYPE];
		CConnDB db(pDB);
		nRet = db.UpdateSudRecord(
			dataIn._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID]		,
			//dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			
			//dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			
			//dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				
			dtSudTime,			
			eType,
			dataIn._mapString[FIELD_CASELIBRARY_SUD_REASON].c_str()	,
			dataIn._mapString[FIELD_CASELIBRARY_SUD_FAULT_ID].c_str(),		
			dataIn._mapString[FIELD_CASELIBRARY_SUD_REMARK].c_str()	
			);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::SudRecordDel( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_SUD_RECORD_DEL);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID] = pFixComm.GetLong(FIELD_CASELIBRARY_SUD_RECORD_ID);
	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	 
	
	{
		CConnDB db(pDB);
		nRet = db.DeleteSudRecord(dataIn._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID]);
		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);

	return true;
}

bool CCaseHandle::SudRecordQuery( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_SUD_RECORD_QUERY);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID]			= pFixComm.GetItem(FIELD_CASELIBRARY_FAULT_ID);
	dataIn._mapString[FIELD_CASELIBRARY_QUERY_BEGIN_TIME]	= pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_BEGIN_TIME);
	dataIn._mapString[FIELD_CASELIBRARY_QUERY_END_TIME]		= pFixComm.GetItem(FIELD_CASELIBRARY_QUERY_END_TIME);
	
	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";
	std::vector<ST_SUD_Record_Info> vRecord;

	 
	
	{
		CBHDateTime dtBegin,dtEnd;
		dtBegin.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_QUERY_BEGIN_TIME].c_str());
		dtEnd.ParseDateTime(dataIn._mapString[FIELD_CASELIBRARY_QUERY_END_TIME].c_str());

		CConnDB db(pDB);
		nRet = db.QuerySudRecord(
			vRecord,
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()	,	
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()	,	
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		,		
			dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID].c_str()	,
			dtBegin	,
			dtEnd
			);
		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vRecord.size();++i)
	{
		ST_SUD_Record_Info record = vRecord.at(i);
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		temp._mapInteger[FIELD_CASELIBRARY_SUD_RECORD_ID]	= record._nSudRecordID;		
		temp._mapString[FIELD_CASELIBRARY_COMPANY]			= record._strCompany;
		temp._mapString[FIELD_CASELIBRARY_FACTORY]			= record._strFactory;
		temp._mapString[FIELD_CASELIBRARY_PLATNO]			= record._strPlantNO;
		temp._mapString[FIELD_CASELIBRARY_SUD_TIME]			= record._dtSudTime.Format(DATE_TIME_FORMAT);
		temp._mapInteger[FIELD_CASELIBRARY_SUD_TYPE]		= record._eSudType;
		temp._mapString[FIELD_CASELIBRARY_SUD_REASON]		= record._strSudReason;
		temp._mapString[FIELD_CASELIBRARY_SUD_FAULT_ID]		= record._strFaultID;
		temp._mapString[FIELD_CASELIBRARY_SUD_REMARK]		= record._strRemark;
		temp._mapString[FIELD_CASELIBRARY_SUD_RECORDER]		= record._strRecorder;
		temp.Trace();
		vecOut.push_back(temp);
	}

	OutputResultData(pFixComm, vecOut);

	return true;
}
int WriteAttachmentFile(const BUFFER & buf)
{
#ifndef _DEBUG
	return 0;
#endif
	HANDLE hFile = ::CreateFile(_T("test.jpg"),GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		DWORD dw = ::GetLastError();;
		return -1;
	}
	DWORD dwWrite = 0;
	::WriteFile(hFile,&buf.at(0),(DWORD)buf.size(),&dwWrite,NULL);
	::CloseHandle(hFile);

	return 0;
}
int WriteAttachmentFile( ST_Attachment& stCaseAttachment_ )
{
#ifndef _DEBUG
	return 0;
#endif
	HANDLE hFile = ::CreateFile(stCaseAttachment_._strFile,GENERIC_WRITE,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		DWORD dw = ::GetLastError();
		//::ShowMsgBox(stCaseAttachment_._strFile);
		return -1;
	}
	DWORD dwWrite = 0;
	::WriteFile(hFile,stCaseAttachment_._pbFileData,(DWORD)stCaseAttachment_._lFileSize,&dwWrite,NULL);
	::CloseHandle(hFile);

	ASSERT(dwWrite == stCaseAttachment_._lFileSize);
	return 0;
}
bool CCaseHandle::CaseAddAttachmentInfo( ICommInterFace & pFixComm, IDBInterFace  & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_ADD_ATTACHMENT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL]			= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL);
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]	= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED);

	CUSTOM_GUID guid;
	pFixComm.GetItemBuf(FIELD_CASELIBRARY_ATTACHMENT_DATA_KEY,(char*)&guid,sizeof(CUSTOM_GUID));

	int lSize = pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERRING);
	if (lSize > 0)
	{
		BUFFER buf;
		buf.resize(lSize);
		pFixComm.GetItemBuf(FIELD_CASELIBRARY_ATTACHMENT_DATA,(char*)&buf.at(0),lSize);
		if (0 == dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
		{
			CBufferCache::Clear(guid);
		}

		if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] > 
			dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
		{
			CBufferCache::RemoveEnd(guid,dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
			CBufferCache::AppendBuffer(guid,buf);			
		}
	}



	ST_Attachment atta;
	int nCaseID;
	BUFFER attaData;

	if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] == 
		dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
	{
		nCaseID		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
		atta._strName	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_NAME);
		atta._strFile	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_FILE_NAME);
		atta._eType		= (E_ATTACHMENT_TYPE)pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_TYPE);
		atta._strChann	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_CHANN_INFO);
		atta._lFileSize = dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL];
		CBufferCache::GetBuffer(guid,attaData);
		atta._pbFileData = new BYTE[attaData.size()];
		memcpy(atta._pbFileData,&attaData[0],attaData.size());

		
	}

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] == 
		dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])

	{		
		CConnDB db(pDB);
		nRet = db.AddCaseAttachment(nCaseID,
			atta._strName,
			atta._strFile,
			atta._eType,
			atta._lFileSize,
			atta._pbFileData,
			atta._strChann);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}
	else
	{
		nRet = 0;
	}
	// 填充接口输出

	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);
	return true;
}


bool CCaseHandle::CaseAddSymptomInfo( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_ADD_SYMPTOM_INFO);

	S_DATA_INTERFACE dataIn;
	
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]					= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn._mapString[FIELD_CASELIBRARY_SYMPTOM_ID]					= pFixComm.GetItem(FIELD_CASELIBRARY_SYMPTOM_ID);
	dataIn._mapInteger[FIELD_CASELIBRARY_SYMPTOM_VALUE_ID]			= pFixComm.GetLong(FIELD_CASELIBRARY_SYMPTOM_VALUE_ID);
	dataIn._mapDouble[FIELD_CASELIBRARY_SYMPTOM_IMPORTANT_DEGREE]	= pFixComm.GetDouble(FIELD_CASELIBRARY_SYMPTOM_IMPORTANT_DEGREE);
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_SYMPTOM_COUNT]		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_SYMPTOM_COUNT);
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_SYMPTOM_INDEX]		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_SYMPTOM_INDEX);
	dataIn._mapString[FIELD_CASELIBRARY_CASE_SYMPTOM_MATRIX]		= pFixComm.GetItem(FIELD_CASELIBRARY_CASE_SYMPTOM_MATRIX);

	dataIn.Trace();

	CConnDB db(pDB);

	int nRet = db.AddCaseSymptom(
		dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]				,
		dataIn._mapString[FIELD_CASELIBRARY_SYMPTOM_ID].c_str()		,
		dataIn._mapInteger[FIELD_CASELIBRARY_SYMPTOM_VALUE_ID]		,
		(float)dataIn._mapDouble[FIELD_CASELIBRARY_SYMPTOM_IMPORTANT_DEGREE],
		dataIn._mapInteger[FIELD_CASELIBRARY_CASE_SYMPTOM_COUNT]	,
		dataIn._mapInteger[FIELD_CASELIBRARY_CASE_SYMPTOM_INDEX]	,
		dataIn._mapString[FIELD_CASELIBRARY_CASE_SYMPTOM_MATRIX].c_str()
		);
	CString strError = db.GetLastError();

	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	vecOut.push_back(temp);
	OutputResultData(pFixComm, vecOut);
	return true;
}
bool CCaseHandle::MaintenanceRecordAddAttachmentInfo( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_ATTACHMENT_INFO);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL]			= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL);
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]	= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED);

	CUSTOM_GUID guid;
	pFixComm.GetItemBuf(FIELD_CASELIBRARY_ATTACHMENT_DATA_KEY,(char*)&guid,sizeof(CUSTOM_GUID));

	int lSize = pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERRING);
	if (lSize > 0)
	{
		BUFFER buf;
		buf.resize(lSize);
		pFixComm.GetItemBuf(FIELD_CASELIBRARY_ATTACHMENT_DATA,(char*)&buf.at(0),lSize);
		if (0 == dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
		{
			CBufferCache::Clear(guid);
		}

		if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] > 
			dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
		{
			CBufferCache::RemoveEnd(guid,dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
			CBufferCache::AppendBuffer(guid,buf);
		}
	}
	ST_Attachment atta;
	int nRecordID;
	BUFFER attaData;

	if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] == 
		dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])
	{
		nRecordID		= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);
		atta._strName	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_NAME);
		atta._strFile	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_FILE_NAME);
		atta._eType		= (E_ATTACHMENT_TYPE)pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_TYPE);
		atta._strChann	= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_CHANN_INFO);
		atta._lFileSize = dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL];
		
		CBufferCache::GetBuffer(guid,attaData);
		
		ASSERT(atta._lFileSize == attaData.size());
		
		atta._pbFileData = new BYTE[attaData.size()];
		memcpy(atta._pbFileData,&attaData[0],attaData.size());
		
	}

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	if (dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] == 
		dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED])

	{		
		CConnDB db(pDB);
		nRet = db.AddMaintenanceRecord_AttachmentInfo(
			nRecordID,
			atta._strName,
			atta._eType,
			atta._pbFileData,
			atta._lFileSize,
			atta._strFile,
			atta._strChann);

		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
	}
	else
	{
		nRet = 0;
	}
	// 填充接口输出

	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp.Trace();
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);
	return true;
}

bool CCaseHandle::MaintenanceRecordQueryAttachmentRecord( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_RECORD);

	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]	= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);
	dataIn.Trace();

	CConnDB db(pDB);
	std::vector<ST_Attachment> vAtta;
	int nRet = db.QueryMaintenanceRecord_Attachment(vAtta,
		dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]);

	CString strError = db.GetLastError();

	std::vector<S_DATA_INTERFACE> vecOut;
	for (unsigned int i=0;i<vAtta.size();++i)
	{
		S_DATA_INTERFACE temp;
		if (vecOut.empty())
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		ST_Attachment atta = vAtta[i];
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME] = atta._strName;
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_FILE_NAME] = atta._strFile;
		temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_TYPE] = atta._eType;
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_CHANN_INFO] = atta._strChann;
		temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE] = atta._lFileSize;
		vecOut.push_back(temp);
	}
	OutputResultData(pFixComm, vecOut);
	return true;
}

bool CCaseHandle::CaseQueryAttachmentRecord( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_RECORD);

	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]	= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn.Trace();

	CConnDB db(pDB);

	std::vector<ST_Attachment> vAtta;
	int nRet = db.GetCaseAttachRecord(vAtta,dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);
	CString strError = db.GetLastError();

	std::vector<S_DATA_INTERFACE> vecOut;
	for (unsigned int i=0;i<vAtta.size();++i)
	{
		S_DATA_INTERFACE temp;
		if (vecOut.empty())
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME]		= vAtta[i]._strName;
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_FILE_NAME]	= vAtta[i]._strFile;
		temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_TYPE]		= vAtta[i]._eType;
		temp._mapString[FIELD_CASELIBRARY_ATTACHMENT_CHANN_INFO]= vAtta[i]._strChann;
		temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE]		= vAtta[i]._lFileSize;
		vecOut.push_back(temp);
	}

	OutputResultData(pFixComm, vecOut);
	return true;
}

bool CCaseHandle::CaseQueryAttachmentData( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_DATA);

	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]						= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_ID);
	dataIn._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME]				= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_NAME);
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]	= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED);
	dataIn.Trace();

	CConnDB db(pDB);

	ST_Attachment atta;
	atta._strName = dataIn._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME].c_str();

	int nRet = db.GetCaseAttachData(atta,dataIn._mapInteger[FIELD_CASELIBRARY_CASE_ID]);
	
	CString strError = db.GetLastError();

	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] = atta._lFileSize;

	int lSizeTransferring = MAX_BUFFER_DATA_TRANSFER;
	if (MAX_BUFFER_DATA_TRANSFER > (atta._lFileSize-dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]))
	{
		lSizeTransferring = (atta._lFileSize-dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
	}

	temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERRING] = lSizeTransferring;
	char* pbuf = (char*)(atta._pbFileData + dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
	BUFFER buf(pbuf,pbuf + lSizeTransferring);
	temp._mapBuffer[FIELD_CASELIBRARY_ATTACHMENT_DATA] = buf;
	char* pTemp = (char*)&temp._mapBuffer[FIELD_CASELIBRARY_ATTACHMENT_DATA].at(0);
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);
	return true;
}

bool CCaseHandle::MaintenanceRecordQueryAttachmentData( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);
	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_DATA);

	S_DATA_INTERFACE dataIn;
	dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]						= pFixComm.GetLong(FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID);
	dataIn._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME]				= pFixComm.GetItem(FIELD_CASELIBRARY_ATTACHMENT_NAME);
	dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]	= pFixComm.GetLong(FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED);
	dataIn.Trace();

	CConnDB db(pDB);

	ST_Attachment atta;
	atta._strName = dataIn._mapString[FIELD_CASELIBRARY_ATTACHMENT_NAME].c_str();

	int nRet = db.QueryMaintenanceRecord_AttachmentData(atta,dataIn._mapInteger[FIELD_CASELIBRARY_MAINTENANCE_RECORD_ID]);
	::WriteAttachmentFile(atta);
	CString strError = db.GetLastError();

	std::vector<S_DATA_INTERFACE> vecOut;
	S_DATA_INTERFACE temp;
	temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
	temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
	temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TOTAL] = atta._lFileSize;

	int lSizeTransferring = MAX_BUFFER_DATA_TRANSFER;
	if (MAX_BUFFER_DATA_TRANSFER > (atta._lFileSize-dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]))
	{
		lSizeTransferring = (atta._lFileSize-dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
	}

	temp._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERRING] = lSizeTransferring;
	char* pbuf = (char*)(atta._pbFileData + dataIn._mapInteger[FIELD_CASELIBRARY_ATTACHMENT_SIZE_TRANSFERED]);
	BUFFER buf(pbuf,pbuf + lSizeTransferring);
	temp._mapBuffer[FIELD_CASELIBRARY_ATTACHMENT_DATA] = buf;
	char* pTemp = (char*)&temp._mapBuffer[FIELD_CASELIBRARY_ATTACHMENT_DATA].at(0);
	vecOut.push_back(temp);

	OutputResultData(pFixComm, vecOut);
	return true;
}

bool CCaseHandle::CaseBasedReasoning( ICommInterFace & pFixComm, IDBInterFace & pDB )
{
	CCaseHandle::SwitchDbNameEx(&pDB);

	CMyLog::Trace_CreateHead_ID(CASELIBRARY_MODULE_CASE_BASED_REASONING);

	// 获得接口输入
	S_DATA_INTERFACE dataIn;
	dataIn._mapString[FIELD_CASELIBRARY_PLATNO]				= pFixComm.GetItem(FIELD_CASELIBRARY_PLATNO);
	dataIn._mapString[FIELD_CASELIBRARY_FACTORY]			= pFixComm.GetItem(FIELD_CASELIBRARY_FACTORY);
	dataIn._mapString[FIELD_CASELIBRARY_COMPANY]			= pFixComm.GetItem(FIELD_CASELIBRARY_COMPANY);
	dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS]		= pFixComm.GetLong(FIELD_CASELIBRARY_CASE_STATUS);
	dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID]			= pFixComm.GetItem(FIELD_CASELIBRARY_FAULT_ID);

	int nSymptomCount = pFixComm.GetLong(FIELD_CASELIBRARY_CASE_SYMPTOM_COUNT);
	
	vector<ST_Symptom> vBuff;
	if (nSymptomCount > 0)
	{
		vBuff.resize(nSymptomCount);
		pFixComm.GetItemBuf(FIELD_CASELIBRARY_SYMPTOM_BUFFER,(char*)&vBuff.at(0),sizeof(ST_Symptom)*nSymptomCount);
	}

	dataIn.Trace();
	// 访问数据库

	int nRet = -1;
	string strError = "";

	vector<ST_CBR_Result> vCBRList;

	{
		std::vector<ST_Case_Info> vCaseList;
		CConnDB db(pDB);
		E_CASE_VERIFY_STATUS leStatus = (E_CASE_VERIFY_STATUS)dataIn._mapInteger[FIELD_CASELIBRARY_CASE_STATUS];
		nRet = db.QueryCase(vCaseList,
			dataIn._mapString[FIELD_CASELIBRARY_PLATNO].c_str()			,
			dataIn._mapString[FIELD_CASELIBRARY_FACTORY].c_str()		,
			dataIn._mapString[FIELD_CASELIBRARY_COMPANY].c_str()		,
			leStatus													,
			dataIn._mapString[FIELD_CASELIBRARY_FAULT_ID].c_str()		
			);
		if (0 != nRet)
		{
			strError = string(db.GetLastError().GetBuffer());
		}
		else
		{
			// 计算案例和传入的征兆BUFFER之间的相似度
			for (std::vector<ST_Case_Info>::iterator it=vCaseList.begin();it!=vCaseList.end();++it)
			{
				nRet = db.GetCaseSymptom(it->_vCaseSymptom,it->_nCaseID);
				if (it->_vCaseSymptom.empty())
				{
					continue;
				}
				float fSim = ::CalcSimilarity(vBuff,it->_vCaseSymptom);
				ST_CBR_Result temp;
				temp._fSimValue = fSim;
				if (temp.IsSimValueValid(fSim))
				{
					temp._caseInfo = *it;
					vCBRList.push_back(temp);
				}
			}
			std::sort(vCBRList.begin(),vCBRList.end());
		}
	}

	// 填充接口输出
	std::vector<S_DATA_INTERFACE> vecOut;
	for (int i=0;i<(int)vCBRList.size();++i)
	{
		ST_CBR_Result lResult = vCBRList.at(i);
		ST_Case_Info& lcase = lResult._caseInfo;
		S_DATA_INTERFACE temp;
		if (0 == i)
		{
			temp._mapInteger[FIELD_CASELIBRARY_RESULT] = GetConnMWStatus(nRet);
			temp._mapString[FIELD_CASELIBRARY_ERROR_MSG] = strError;
		}
		temp._mapInteger[FIELD_CASELIBRARY_CASE_ID]		= lcase._nCaseID;
		temp._mapString[FIELD_CASELIBRARY_CASE_NAME]	= lcase._strCaseName;
		temp._mapString[FIELD_CASELIBRARY_CASE_DETAIL]  = lcase._strCaseDetail.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_COMPANY]		= lcase._strCompany.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_FACTORY]		= lcase._strFactory.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_PLATNO]		= lcase._strPlantNO.GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_TIME]	= lcase._dtCase.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_INPUT_USER]	= lcase._strAddUserName;
		temp._mapString[FIELD_CASELIBRARY_CASE_INPUT_TIME]	= lcase._dtAddTime.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapString[FIELD_CASELIBRARY_CASE_VERIFY_TIME]	= lcase._dtVerify.Format(DATE_TIME_FORMAT).GetBuffer();
		temp._mapInteger[FIELD_CASELIBRARY_CASE_STATUS]	   	= lcase._eCaseCheckStatus;
		temp._mapDouble[FIELD_CASELIBRARY_CBR_SIMILARITY]	= lResult._fSimValue;
		vecOut.push_back(temp);

		temp.Trace();
	}

	OutputResultData(pFixComm, vecOut);
	return true;
}

CString CCaseHandle::GetCaseDBName()
{
	CString strCaseDBName;

#ifdef _S_CASELIBRARY
	char value[1000];

	char group[100],item[100],file[100];
	snprintf(group,100,_T("SERVER"));
	snprintf(item,100,_T("SERVER1"));

	char path[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,path);
	snprintf(file,MAX_PATH,_T("%s/%s"),path,_T("DseSvr.ini"));

	DWORD dword = ::GetPrivateProfileString(group,item,_T(""),value,1000,file);

	CString strTemp;
	strTemp.Format(_T("%s"),value);  //文本格式：监测中心,.,BH5000DB

	for (int i=strTemp.GetLength()-1;i>=0 && strTemp[i] != ',';i--)
	{
		strCaseDBName = strTemp[i] + strCaseDBName;
	}

#endif

#ifdef _PIDMS
	strCaseDBName = _T("CaseLibrary");
#endif
	return strCaseDBName;
}