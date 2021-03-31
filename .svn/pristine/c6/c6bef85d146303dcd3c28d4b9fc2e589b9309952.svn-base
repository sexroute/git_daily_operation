#pragma once

#include <additions/ICommInterFace.h>

#include <additions/IDBInterFace.h>


class CCaseHandle
{
public:
	static int GetTestInt(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	// 添加案例基本信息                                             
	//CASELIBRARY_MODULE_CASE_ADD_BASIC_INFO											
	
	static bool CaseAddBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);					
	
	//修改基本信息                                                  
	//CASELIBRARY_MODULE_CASE_UPDATE_BASIC_INFO											
	static bool CaseUpdateBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	//添加故障信息                                                  
	//CASELIBRARY_MODULE_CASE_ADD_FAULT_INFO																
	static bool CaseAddFaultInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////添加征兆信息                                                  
	//CASELIBRARY_MODULE_CASE_ADD_SYMPTOM_INFO												
	static bool CaseAddSymptomInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////添加附件信息                                                  
	//CASELIBRARY_MODULE_CASE_ADD_ATTACHMENT_INFO										
	static bool CaseAddAttachmentInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////案例提交                                                      
	//CASELIBRARY_MODULE_CASE_SUBMIT																	
	static bool CaseSubmit(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////案例审核                                                      
	//CASELIBRARY_MODULE_CASE_VERIFY				
	static bool CaseVerify(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除附件信息                                                  
	//CASELIBRARY_MODULE_CASE_DEL_ATTACHMENT_INFO										
	static bool CaseDelAttachmentInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除征兆信息                                                  
	//CASELIBRARY_MODULE_CASE_DEL_SYMPTOM_INFO												
	static bool CaseDelSymptomInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除故障信息                                                  
	//CASELIBRARY_MODULE_CASE_DEL_FAULT_INFO													
	static bool CaseDelFaultInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除案例                                                      
	//CASELIBRARY_MODULE_CASE_DEL																		
	static bool CaseDel(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询所有案例                                                  
	//CASELIBRARY_MODULE_CASE_QUERY_BASIC_INFO		
	static bool CaseQueryBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询案例故障信息                                              
	//CASELIBRARY_MODULE_CASE_QUERY_FAULT_INFO												
	static bool CaseQueryFaultInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询案例征兆信息                                              
	//CASELIBRARY_MODULE_CASE_QUERY_SYMPTOM_INFO											
	static bool CaseQuerySymptomInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询案例附件记录(不包括二进制数据)                                          
	//CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_RECORD									
	static bool CaseQueryAttachmentRecord(ICommInterFace & pFixComm, IDBInterFace  & pDB);
	
	////查询案例附件二进制数据
	//CASELIBRARY_MODULE_CASE_QUERY_ATTACHMENT_DATA
	static bool CaseQueryAttachmentData(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////基于案例推理                                   
	//CASELIBRARY_MODULE_CASE_BASED_REASONING									
	static bool CaseBasedReasoning(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////增加检维修记录                                                
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_BASIC_INFO						
	static bool MaintenanceRecordAddBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////给指定检维修记录添加一个附件                                  
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_ADD_ATTACHMENT_INFO			
	static bool MaintenanceRecordAddAttachmentInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////更新检维修记录基本信息                                        
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_UPDATE_BASIC_INFO				
	static bool MaintenanceRecordUpdateBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除检维修记录的附件信息                                      
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_ATTACHMENT_INFO			
	static bool MaintenanceRecordDelAttachmentInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////删除一条检维修记录                                            
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_DEL_BASIC_INFO						
	static bool MaintenanceRecordDelBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询检维修记录                                                
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_BASIC_INFO		
	static bool MaintenanceRecordQueryBasicInfo(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询指定检维修记录的附件记录（不包括二进制数据）
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_RECORD		
	static bool MaintenanceRecordQueryAttachmentRecord(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询指定检维修记录的附件二进制数据                    
	//CASELIBRARY_MODULE_MAINTENANCE_RECORD_QUERY_ATTACHMENT_DATA
	static bool MaintenanceRecordQueryAttachmentData(ICommInterFace & pFixComm, IDBInterFace  & pDB);
	

	////添加一条开停车记录                                            
	//CASELIBRARY_MODULE_SUD_RECORD_ADD			
	static bool SudRecordAdd(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////更新一条开停车记录                                            
	//CASELIBRARY_MODULE_SUD_RECORD_UPDATE							
	static bool SudRecordUpdate(ICommInterFace & pFixComm, IDBInterFace  & pDB);							

	////删除一条开停车记录                                            
	//CASELIBRARY_MODULE_SUD_RECORD_DEL															
	static bool SudRecordDel(ICommInterFace & pFixComm, IDBInterFace  & pDB);

	////查询开停车记录                                                
	//CASELIBRARY_MODULE_SUD_RECORD_QUERY														
	static bool SudRecordQuery(ICommInterFace & pFixComm, IDBInterFace  & pDB);
	

private:
	static void SwitchDbNameEx(IDBInterFace *pDB_)
	{
		CString sDbName = GetCaseDBName();
		{
			if (pDB_ == NULL)
			{
				return ;
			}

			pDB_->DbUse("master");
			try
			{
				if (sDbName.Find(_T('[')) >=0)
				{

				}else
				{
					sDbName  = _T('[') + sDbName;

					sDbName += _T(']');
				}

				pDB_->DbUse(sDbName);

			}
			catch (...)
			{
				return;
			}

		}
	}

	static CString GetCaseDBName();
};
