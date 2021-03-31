#pragma once

#include <additions/IDBInterFace.h>

#include "BHCaseLibrary_Define.h"

class CConnDB
{
public:
	explicit CConnDB(IDBInterFace  & pDB);
	~CConnDB(void);


	/** 连接mysql数据库
	*	\return BOOL TRUE 连接成功，FALSE 连接失败
	*/
	BOOL Conn();
	

	/** 获得最近的错误信息
	*	\return CString 错误信息
	*/
	CString GetLastError();


	/** 添加案例，写入基本信息  30001
	*	\param[in] strPlantNO_ 设备号
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\param[in] strCaseName_ 案例名称
	*	\param[in] strCaseDetail_ 案例描述
	*	\param[in] strCaseTime_ 案例发生时间
	*	\param[in] strInputUser_ 案例添加人员
	*	\param[out] nCaseID_ 案例ID
	*	\return int 0-成功
	*/
	int AddCase(CString strPlantNO_,CString strFactory_,CString strCompany_,CString strCaseName_,
		CString strCaseDetail_,CString strCaseTime_,CString strInputUser_,int& nCaseID_);

	
	/** 更新案例 - 基本信息   30002
	*	\param[in] nCaseID_ 案例ID
	*	\param[in] strName_ 案例名称
	*	\param[in] strDetail_ 案例描述
	*	\param[in] dtCase_ 案例发生时间
	*	\param[in] strPlantNO_ 设备号
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\return int 0-添加成功
	*/
	int UpdateCase_BasicInfo(int nCaseID_,CString strName_,CString strDetail_,CBHDateTime dtCase_,
		const CString& strPlantNO_,const CString& strFactory_,
		const CString& strCompany_);

	//!\name 提交案例
	//@{

	// 30003
	int AddCaseFault(int nCaseID_,CString strFaultID_);
	// 30004
	int AddCaseSymptom(int nCaseID_,CString strSymptomID_,int nSymptomValueID_,float fDegree_,
		int nCaseSymptomCount_,int nCaseSymptomIndex_,CString strWeightMatrix_);
	// 30005
	int AddCaseAttachment(int nCaseID_,CString strAttaName_,CString strFileName_,int nAttaType_,long nAttaSize_,
		BYTE* pAttaData_,CString strChann_);
	// 30006
	int SubmitCase(int nCaseID_);

	//@}
	
	// 审核案例 30007
	int SetCaseVerifyStatus(int nCaseID_,int nStatus_,CString strUserName_);

	//!\name 删除一个案例
	//@{

	int DeleteCaseAttachment(int nCaseID_);  // 30008
	int DeleteCaseSymptom(int nCaseID_);     // 30009
	int DeleteCaseFault(int nCaseID_);       // 30010
	int DeleteCaseBasicInfo(int nCaseID_);   // 30011

	//@}
	//!\name 查询案例，获得一个案例的故障、征兆、附件信息
	//@{
	/** 查询案例，返回基本信息   30012
	*	\param[out] vCaseInfo_    案例信息
	*	\param[in] strPlantName   设备名称
	*	\param[in] strFactory_    分厂名称
	*	\param[in] strCompany_    公司名称
	*	\param[in] eCaseStatus_   案例状态
	*	\param[in] strFaultID_      故障ID
	*	\param[in] nSymptomID_    征兆ID
	*	\param[in] vCaseID_       案例列表
	*	\return int 返回值说明
	*/
	int QueryCase(vector<ST_Case_Info>& vCaseInfo_,CString strPlantName_="",CString strFactory_="",CString strCompany_="",E_CASE_VERIFY_STATUS eCaseStatus_=CASE_VERIFY_NONE,
		CString strFaultID_="",int nSymptomID_=-1,CString strCaseIDArr_="",
		const CBHDateTime& dtBegin_=COLEDATETIME_NULL,
		const CBHDateTime& dtEnd_=COLEDATETIME_NULL);  
	// 30013
	int GetCaseFault(vector<ST_CaseFault>& vCaseFault_,int nCaseID_);      
	// 30014
	int GetCaseSymptom(vector<ST_CaseSymptom>& vCaseSymptom_,int nCaseID_); 
	// 30015
	int GetCaseAttachRecord(vector<ST_Attachment>& vCaseAtta_,int nCaseID_); 
	
	int GetCaseAttachData(ST_Attachment& caseAtta_,int nCaseID_);
	//@}

	/** 添加一条检维修记录  31001
	*	\param[out] nRecordID_		记录ID
	*	\param[in] strName_			记录名称
	*	\param[in] strPlantNO_		设备号
	*	\param[in] strFactory_		分厂名称
	*	\param[in] strCompany_		公司名称
	*	\param[in] eType_			检修类型
	*	\param[in] strSection_		检修部位
	*	\param[in] dtBegin_			检修开始时间
	*	\param[in] dtEnd_			检修结束时间
	*	\param[in] strConent_		检修内容
	*	\param[in] strReason_		检修原因描述
	*	\param[in] strFaultID_		检修故障ID
	*	\param[in] strResult_		检修结果
	*	\param[in] strMainPeople	主检修人员
	*	\param[in] strOtherPeople_	其它检修人员
	*	\param[in] strRemark_		备注
	*	\param[in] dtAdd_			记录添加时间
	*	\param[in] strRecorder_		记录人       
	*	\return int 0-成功
	*/
	int AddMaintenanceRecord(int& nRecordID_,CString strName_,
		CString strPlantNO_,CString strFactory_,CString strCompany_,
		E_MAINTENANCE_TYPE eType_,
		CString strSection_,CBHDateTime dtBegin_,CBHDateTime dtEnd_,
		CString strConent_,CString strReason_,CString strFaultID_,
		CString strResult_,CString strMainPeople,CString strOtherPeople_,
		CString strRemark_,CBHDateTime dtAdd_,CString strRecorder_);

	/** 增加指定检维修记录一个附件  31002
	*	\param[in] nRecordID_		检维修记录ID
	*	\param[in] strName_			附件名称
	*	\param[in] nType_			附件类型
	*	\param[in] pData_			附件数据
	*	\param[in] lDataSize_		附件大小
	*	\param[in] strFileName_		附件文件名
	*	\param[in] strChann_		测点信息     
	*	\return int		0-成功
	*/
	int AddMaintenanceRecord_AttachmentInfo(int nRecordID_,CString strName_,
		int nType_,BYTE* pData_,long lDataSize_,CString strFileName_,
		CString strChann_); 

	/** 更新检维修记录基本信息 31003
	*	\param[in] nRecordID_		记录ID
	*	\param[in] strName_			记录名称
	*	\param[in] strPlantNO_		设备号
	*	\param[in] strFactory_		分厂名称
	*	\param[in] strCompany_		公司名称
	*	\param[in] eType_			检修类型
	*	\param[in] strSection_		检修部位
	*	\param[in] dtBegin_			检修开始时间
	*	\param[in] dtEnd_			检修结束时间
	*	\param[in] strConent_		检修内容
	*	\param[in] strReason_		检修原因描述
	*	\param[in] nFaultID_		检修故障ID
	*	\param[in] strResult_		检修结果
	*	\param[in] strMainPeople	主检修人员
	*	\param[in] strOtherPeople_	其它检修人员
	*	\param[in] strRemark_		备注
	*	\param[in] dtAdd_			记录添加时间
	*	\param[in] strRecorder_		记录人
	*	\return int 0-成功
	*/
	int UpdateMaintenanceRecord(int nRecordID_,CString strName_,CString strPlantNO_,
		CString strFactory_,CString strCompany_,
		E_MAINTENANCE_TYPE eType_,
		CString strSection_,CBHDateTime dtBegin_,CBHDateTime dtEnd_,
		CString strConent_,CString strReason_,CString strFaultID_,
		CString strResult_,CString strMainPeople,CString strOtherPeople_,
		CString strRemark_,CBHDateTime dtAdd_,CString strRecorder_); 
	
	/** 删除检维修记录的附件信息  31004
	*	\param[in] nRecord_ 检维修记录ID
	*	\return int 0-成功
	*/
	int DeleteMaintenanceRecord_AttachmentInfo(int nRecord_); 

	
	/** 删除一条检维修记录  31005
	*	\param[in] nRecordID_ 检维修记录ID
	*	\return int 0-成功
	*/
	int DeleteMaintenanceRecord(int nRecordID_); 

	// 31006
	/** 函数功能详细描述
	*	\param[out] vRecord_ 
	*	\param[in] strPlantNO_ 
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\param[in] dtBegin_
	*	\param[in] dtEnd_ 
	*	\return int 0-成功
	*/
	int QueryMaintenanceRecord(vector<ST_Maintenance_Record_Info>& vRecord_,
		CString strPlantNO_="",CString strFactory_="",CString strCompany_="",
		CBHDateTime dtBegin_=COLEDATETIME_NULL,CBHDateTime dtEnd_=COLEDATETIME_NULL); 

	// 31007
	int QueryMaintenanceRecord_Attachment(vector<ST_Attachment>& vAtta_,int nRecordID_); 
	int QueryMaintenanceRecord_AttachmentData(ST_Attachment& atta_,int nRecordID_); 

	/// IsMaintenanceRecordNameExist
	/** 数据库是否存在该检维修记录名称
	*	\param[in] strName_ 记录名称
	*	\return BOOL TRUE-存在
	*/
	BOOL IsMaintenanceRecordNameExist(CString strName_);


	/// IsMaintenanceRecordNameExist
	/** 检索数据库，是否存在记录，记录名称符合，ID不符合，用于更新时判断
	*	\param[in] strName_ 参数说明
	*	\param[in] nRecordID_ 参数说明
	*	\return BOOL 返回值说明
	*/
	BOOL IsMaintenanceRecordNameExist(CString strName_,int nRecordID_);



	/** 添加开停车记录
	*	\param[out] nSudRecordID_	记录ID
	*	\param[in] strCompany_		公司名称
	*	\param[in] strFactory_		分厂名称
	*	\param[in] strPlantNO_		设备号
	*	\param[in] dtSudTime_		开停车时间
	*	\param[in] eSudType_		开停车类型
	*	\param[in] strSudReason_	开停车原因
	*	\param[in] strFaultID_		故障ID
	*	\param[in] strRemark_		备注
	*	\param[in] strRecorder_		记录人员   
	*	\return int 0-成功
	*/
	int AddSudRecord(int& nSudRecordID_, CString strCompany_, CString strFactory_,
		CString strPlantNO_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, 
		CString strSudReason_, CString strFaultID_, 
		CString strRemark_, CString strRecorder_ ); // 62001


	/** 更新一条开停车记录
	*	\param[in] nSudRecordID_	开停车记录ID
	*	\param[in] dtSudTime_		开停车时间
	*	\param[in] eSudType_		开停车类型
	*	\param[in] strSudReason_	开停车原因
	*	\param[in] strFaultID_		故障ID
	*	\param[in] strRemark_		备注
	*	\return int			0-成功
	*/
	int UpdateSudRecord(int nSudRecordID_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, 
		CString strSudReason_,CString strFaultID_, CString strRemark_); // 62002

	
	/** 删除一条开停车记录
	*	\param[in] nRecordID_ 
	*	\return int 返回值说明
	*/
	int DeleteSudRecord(int nRecordID_);

	/** 查询开停车记录
	*	\param[in] recordList_		开停车记录表
	*	\param[in] strPlantNO		设备号
	*	\param[in] strFactory		分厂名
	*	\param[in] strCompany_		公司名
	*	\param[in] strFaultID_		故障ID
	*	\param[in] dtBegin_			开始时间
	*	\param[in] dtEnd_			结束时间
	*	\return int 0-成功
	*/
	int QuerySudRecord(vector<ST_SUD_Record_Info>& recordList_,CString strPlantNO="",CString strFactory="",
		CString strCompany_="",CString strFaultID_="",CBHDateTime dtBegin_=COLEDATETIME_NULL,
		CBHDateTime dtEnd_=COLEDATETIME_NULL);

private:
	CString _strLastError;

	IDBInterFace* _pdb ;

	BOOL TestDBCon();
};
