#pragma once

#include <additions/IDBInterFace.h>

#include "BHCaseLibrary_Define.h"

class CConnDB
{
public:
	explicit CConnDB(IDBInterFace  & pDB);
	~CConnDB(void);


	/** ����mysql���ݿ�
	*	\return BOOL TRUE ���ӳɹ���FALSE ����ʧ��
	*/
	BOOL Conn();
	

	/** �������Ĵ�����Ϣ
	*	\return CString ������Ϣ
	*/
	CString GetLastError();


	/** ��Ӱ�����д�������Ϣ  30001
	*	\param[in] strPlantNO_ �豸��
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\param[in] strCaseName_ ��������
	*	\param[in] strCaseDetail_ ��������
	*	\param[in] strCaseTime_ ��������ʱ��
	*	\param[in] strInputUser_ ���������Ա
	*	\param[out] nCaseID_ ����ID
	*	\return int 0-�ɹ�
	*/
	int AddCase(CString strPlantNO_,CString strFactory_,CString strCompany_,CString strCaseName_,
		CString strCaseDetail_,CString strCaseTime_,CString strInputUser_,int& nCaseID_);

	
	/** ���°��� - ������Ϣ   30002
	*	\param[in] nCaseID_ ����ID
	*	\param[in] strName_ ��������
	*	\param[in] strDetail_ ��������
	*	\param[in] dtCase_ ��������ʱ��
	*	\param[in] strPlantNO_ �豸��
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\return int 0-��ӳɹ�
	*/
	int UpdateCase_BasicInfo(int nCaseID_,CString strName_,CString strDetail_,CBHDateTime dtCase_,
		const CString& strPlantNO_,const CString& strFactory_,
		const CString& strCompany_);

	//!\name �ύ����
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
	
	// ��˰��� 30007
	int SetCaseVerifyStatus(int nCaseID_,int nStatus_,CString strUserName_);

	//!\name ɾ��һ������
	//@{

	int DeleteCaseAttachment(int nCaseID_);  // 30008
	int DeleteCaseSymptom(int nCaseID_);     // 30009
	int DeleteCaseFault(int nCaseID_);       // 30010
	int DeleteCaseBasicInfo(int nCaseID_);   // 30011

	//@}
	//!\name ��ѯ���������һ�������Ĺ��ϡ����ס�������Ϣ
	//@{
	/** ��ѯ���������ػ�����Ϣ   30012
	*	\param[out] vCaseInfo_    ������Ϣ
	*	\param[in] strPlantName   �豸����
	*	\param[in] strFactory_    �ֳ�����
	*	\param[in] strCompany_    ��˾����
	*	\param[in] eCaseStatus_   ����״̬
	*	\param[in] strFaultID_      ����ID
	*	\param[in] nSymptomID_    ����ID
	*	\param[in] vCaseID_       �����б�
	*	\return int ����ֵ˵��
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

	/** ���һ����ά�޼�¼  31001
	*	\param[out] nRecordID_		��¼ID
	*	\param[in] strName_			��¼����
	*	\param[in] strPlantNO_		�豸��
	*	\param[in] strFactory_		�ֳ�����
	*	\param[in] strCompany_		��˾����
	*	\param[in] eType_			��������
	*	\param[in] strSection_		���޲�λ
	*	\param[in] dtBegin_			���޿�ʼʱ��
	*	\param[in] dtEnd_			���޽���ʱ��
	*	\param[in] strConent_		��������
	*	\param[in] strReason_		����ԭ������
	*	\param[in] strFaultID_		���޹���ID
	*	\param[in] strResult_		���޽��
	*	\param[in] strMainPeople	��������Ա
	*	\param[in] strOtherPeople_	����������Ա
	*	\param[in] strRemark_		��ע
	*	\param[in] dtAdd_			��¼���ʱ��
	*	\param[in] strRecorder_		��¼��       
	*	\return int 0-�ɹ�
	*/
	int AddMaintenanceRecord(int& nRecordID_,CString strName_,
		CString strPlantNO_,CString strFactory_,CString strCompany_,
		E_MAINTENANCE_TYPE eType_,
		CString strSection_,CBHDateTime dtBegin_,CBHDateTime dtEnd_,
		CString strConent_,CString strReason_,CString strFaultID_,
		CString strResult_,CString strMainPeople,CString strOtherPeople_,
		CString strRemark_,CBHDateTime dtAdd_,CString strRecorder_);

	/** ����ָ����ά�޼�¼һ������  31002
	*	\param[in] nRecordID_		��ά�޼�¼ID
	*	\param[in] strName_			��������
	*	\param[in] nType_			��������
	*	\param[in] pData_			��������
	*	\param[in] lDataSize_		������С
	*	\param[in] strFileName_		�����ļ���
	*	\param[in] strChann_		�����Ϣ     
	*	\return int		0-�ɹ�
	*/
	int AddMaintenanceRecord_AttachmentInfo(int nRecordID_,CString strName_,
		int nType_,BYTE* pData_,long lDataSize_,CString strFileName_,
		CString strChann_); 

	/** ���¼�ά�޼�¼������Ϣ 31003
	*	\param[in] nRecordID_		��¼ID
	*	\param[in] strName_			��¼����
	*	\param[in] strPlantNO_		�豸��
	*	\param[in] strFactory_		�ֳ�����
	*	\param[in] strCompany_		��˾����
	*	\param[in] eType_			��������
	*	\param[in] strSection_		���޲�λ
	*	\param[in] dtBegin_			���޿�ʼʱ��
	*	\param[in] dtEnd_			���޽���ʱ��
	*	\param[in] strConent_		��������
	*	\param[in] strReason_		����ԭ������
	*	\param[in] nFaultID_		���޹���ID
	*	\param[in] strResult_		���޽��
	*	\param[in] strMainPeople	��������Ա
	*	\param[in] strOtherPeople_	����������Ա
	*	\param[in] strRemark_		��ע
	*	\param[in] dtAdd_			��¼���ʱ��
	*	\param[in] strRecorder_		��¼��
	*	\return int 0-�ɹ�
	*/
	int UpdateMaintenanceRecord(int nRecordID_,CString strName_,CString strPlantNO_,
		CString strFactory_,CString strCompany_,
		E_MAINTENANCE_TYPE eType_,
		CString strSection_,CBHDateTime dtBegin_,CBHDateTime dtEnd_,
		CString strConent_,CString strReason_,CString strFaultID_,
		CString strResult_,CString strMainPeople,CString strOtherPeople_,
		CString strRemark_,CBHDateTime dtAdd_,CString strRecorder_); 
	
	/** ɾ����ά�޼�¼�ĸ�����Ϣ  31004
	*	\param[in] nRecord_ ��ά�޼�¼ID
	*	\return int 0-�ɹ�
	*/
	int DeleteMaintenanceRecord_AttachmentInfo(int nRecord_); 

	
	/** ɾ��һ����ά�޼�¼  31005
	*	\param[in] nRecordID_ ��ά�޼�¼ID
	*	\return int 0-�ɹ�
	*/
	int DeleteMaintenanceRecord(int nRecordID_); 

	// 31006
	/** ����������ϸ����
	*	\param[out] vRecord_ 
	*	\param[in] strPlantNO_ 
	*	\param[in] strFactory_ 
	*	\param[in] strCompany_ 
	*	\param[in] dtBegin_
	*	\param[in] dtEnd_ 
	*	\return int 0-�ɹ�
	*/
	int QueryMaintenanceRecord(vector<ST_Maintenance_Record_Info>& vRecord_,
		CString strPlantNO_="",CString strFactory_="",CString strCompany_="",
		CBHDateTime dtBegin_=COLEDATETIME_NULL,CBHDateTime dtEnd_=COLEDATETIME_NULL); 

	// 31007
	int QueryMaintenanceRecord_Attachment(vector<ST_Attachment>& vAtta_,int nRecordID_); 
	int QueryMaintenanceRecord_AttachmentData(ST_Attachment& atta_,int nRecordID_); 

	/// IsMaintenanceRecordNameExist
	/** ���ݿ��Ƿ���ڸü�ά�޼�¼����
	*	\param[in] strName_ ��¼����
	*	\return BOOL TRUE-����
	*/
	BOOL IsMaintenanceRecordNameExist(CString strName_);


	/// IsMaintenanceRecordNameExist
	/** �������ݿ⣬�Ƿ���ڼ�¼����¼���Ʒ��ϣ�ID�����ϣ����ڸ���ʱ�ж�
	*	\param[in] strName_ ����˵��
	*	\param[in] nRecordID_ ����˵��
	*	\return BOOL ����ֵ˵��
	*/
	BOOL IsMaintenanceRecordNameExist(CString strName_,int nRecordID_);



	/** ��ӿ�ͣ����¼
	*	\param[out] nSudRecordID_	��¼ID
	*	\param[in] strCompany_		��˾����
	*	\param[in] strFactory_		�ֳ�����
	*	\param[in] strPlantNO_		�豸��
	*	\param[in] dtSudTime_		��ͣ��ʱ��
	*	\param[in] eSudType_		��ͣ������
	*	\param[in] strSudReason_	��ͣ��ԭ��
	*	\param[in] strFaultID_		����ID
	*	\param[in] strRemark_		��ע
	*	\param[in] strRecorder_		��¼��Ա   
	*	\return int 0-�ɹ�
	*/
	int AddSudRecord(int& nSudRecordID_, CString strCompany_, CString strFactory_,
		CString strPlantNO_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, 
		CString strSudReason_, CString strFaultID_, 
		CString strRemark_, CString strRecorder_ ); // 62001


	/** ����һ����ͣ����¼
	*	\param[in] nSudRecordID_	��ͣ����¼ID
	*	\param[in] dtSudTime_		��ͣ��ʱ��
	*	\param[in] eSudType_		��ͣ������
	*	\param[in] strSudReason_	��ͣ��ԭ��
	*	\param[in] strFaultID_		����ID
	*	\param[in] strRemark_		��ע
	*	\return int			0-�ɹ�
	*/
	int UpdateSudRecord(int nSudRecordID_, CBHDateTime dtSudTime_, E_SUD_TYPE eSudType_, 
		CString strSudReason_,CString strFaultID_, CString strRemark_); // 62002

	
	/** ɾ��һ����ͣ����¼
	*	\param[in] nRecordID_ 
	*	\return int ����ֵ˵��
	*/
	int DeleteSudRecord(int nRecordID_);

	/** ��ѯ��ͣ����¼
	*	\param[in] recordList_		��ͣ����¼��
	*	\param[in] strPlantNO		�豸��
	*	\param[in] strFactory		�ֳ���
	*	\param[in] strCompany_		��˾��
	*	\param[in] strFaultID_		����ID
	*	\param[in] dtBegin_			��ʼʱ��
	*	\param[in] dtEnd_			����ʱ��
	*	\return int 0-�ɹ�
	*/
	int QuerySudRecord(vector<ST_SUD_Record_Info>& recordList_,CString strPlantNO="",CString strFactory="",
		CString strCompany_="",CString strFaultID_="",CBHDateTime dtBegin_=COLEDATETIME_NULL,
		CBHDateTime dtEnd_=COLEDATETIME_NULL);

private:
	CString _strLastError;

	IDBInterFace* _pdb ;

	BOOL TestDBCon();
};
