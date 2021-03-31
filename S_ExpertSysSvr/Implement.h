//Implement.h
#pragma once
#include "StdAfx.h"
#include "PlantInfoExpertSys.h"
#include <additions/IDBInterFace.h>
#include "ExpertSysGlobal.h"

#ifndef _EXPERTSYSTEMSVR_IMPLEMENT_H_
#define _EXPERTSYSTEMSVR_IMPLEMENT_H_

class CImplement
{
public:
	CImplement(void);
	~CImplement(void);
public:

	/**�õ�ָ������IDʱ,ר�����ϵͳ������ȡ��֪ʶ
	*���������   ��˾��     FIELD_SYSTEM_COMPANY
	*             �ֳ���     FIELD_SYSTEM_FACTORY
	*             �����     FIELD_SERVICE_PLANTID
	*             ����ID     FIELD_SERVICE_ALARMID
	*���ص��ֶ��룺
	*             ����ͨ��   FIELD_SERVICE_OFF_CHANN1
	*             ��ʵID     FIELD_SERVICE_EX_RULEID     //��ӦΪ�ַ���
	*             ��Ӧ��ֵ   FIELD_SERVICE_EX_IDVALUE    //��ӦΪint��
	*/
	int ExpertSysGetRegAlarmFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�ָ���񶯲�㡢ѡ��ʱ���ʱ,ר�����ϵͳ������ȡ��֪ʶ
	*���������   ��˾��     FIELD_SYSTEM_COMPANY
	*             �ֳ���     FIELD_SYSTEM_FACTORY
	*             �����     FIELD_SERVICE_PLANTID
	*             ���ID     FIELD_SERVICE_CHANN1
	*             ��ʼʱ��   FIELD_SERVICE_OFF_TIMESTART
	*             ��ֹʱ��   FIELD_SERVICE_OFF_TIMEEND
	*���ص��ֶ��룺
	*             ��ʵID     FIELD_SERVICE_EX_RULEID     //��ӦΪ�ַ���
	*             ��Ӧ��ֵ   FIELD_SERVICE_EX_IDVALUE    //��ӦΪint��
	#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT      30002
	*/
    int ExpertSysGetSpecChannFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**�õ�ָ���񶯲�㡢ѡ��ʱ���ʱ,ר�����ϵͳ������ȡ��֪ʶ
	*���������   ��˾��     FIELD_SYSTEM_COMPANY
	*             �ֳ���     FIELD_SYSTEM_FACTORY
	*             �����     FIELD_SERVICE_PLANTID
	*���ص��ֶ��룺
	*             ��˾��     FIELD_SYSTEM_COMPANY
	*             �ֳ���     FIELD_SYSTEM_FACTORY
	*             �����     FIELD_SERVICE_PLANTID
	*             ��ʵ��     FIELD_SERVICE_REMARK     //��ӦΪ�ַ���
	*#define    SERVICE_EXPERTSYS_GET_PLANTFACTNAME      30004
	*/
	int ExpertSysGetPlantFactName(ICommInterFace *pFixComm,IDBInterFace *pDB);

	enum ALARM_CHANN_TYPE
	{
		EXPERT_CHANN_TYPE_TEMP_INVAL    =0x0000,
		EXPERT_CHANN_TYPE_TEMP			=0x0001,
		EXPERT_CHANN_TYPE_SINK			=0x0010,
		EXPERT_CHANN_TYPE_CROSSHEAD		=0x0011,
		EXPERT_CHANN_TYPE_IMPACTS		=0x0100,
		EXPERT_CHANN_TYPE_CRANK			=0x0101,
		EXPERT_CHANN_TYPE_DEFLECT		=0x0110,
		EXPERT_CHANN_TYPE_PV			=0x0111,
		EXPERT_CHANN_TYPE_ALL			=0x1000,
	};

	public:

	/**д����ʵID�����Ӧ��ֵ
	*@param sFactID_ ��ʵID
	*@param iValue_ ��ʵID��Ӧ��ֵ
	*@param pFixComm_ ��������ָ��
	*/
    void WriteFertureInfo(string sFactID_,int iValue_,ICommInterFace *pFixComm_);

	/**д��ר�����ϵͳ�Ļ�����Ϣ������ȡ֪ʶ
	*@param pPlantInfo_ �豸��Ϣָ��
	*@param pFixComm_ ��������ָ��
	*/
    void WriteMachineFertureInfo(CPlantInfoExpertSys *pPlantInfo_ ,ICommInterFace *pFixComm_);

	/**��ָ�����������ж�ȡ���жϷ�˾���ֳ����豸���пգ����õ������ĺ�׺����ͨ����������ָ�뷵��
	*@param sCompany_ ��˾������һ������
	*@param sFactory_ �ֳ�������һ������
	*@param sPlant_ �豸������һ������
	*@param sAlias_ ��һ�����ã����ڰѹ�˾����������
	*@param pFixComm_ ��������ָ��
	*@param bRtn_  �Ƿ񷵻�����������Ϣ
	*@return �����֤�ɹ����򷵻�1�����򷵻�NULL
	*/
	CPlantInfoExpertSys * ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);

	/**�жϷ�˾���ֳ����豸���пգ����õ������ĺ�׺����ͨ����������ָ�뷵��
	*@param sCompany_ ��˾������һ������
	*@param sFactory_ �ֳ�������һ������
	*@param sPlant_ �豸������һ������
	*@param sAlias_ ��һ�����ã����ڰѹ�˾����������
	*@param pFixComm_ ��������ָ��
	*@param bRtn_  �Ƿ񷵻�����������Ϣ
	*@return �����֤�ɹ����򷵻�1�����򷵻�NULL
	*/
	CPlantInfoExpertSys * CheckCompanyInfo(const string &sCompany_, const string &sFactory_, const string &sPlant_,string &sSuffix_,ICommInterFace *pFixComm_,bool bRtn_=true);

	/**�������Ʊ��еĹ����ֶ�
	  *@return �ֶδ�
	  */
	CString CImplement::ConsCommonField();

	/**����ָ���񶯲�������Ʊ��е��ֶ�
	*@param sChann_      ָ���Ĳ����
	*@param bWave_       �Ƿ���Ҫ�����ֶ�
	*@return ����Ĳ������ֵ�ֶ�
	*/
	CString ConsVibChannField(string sChann_,bool bWave_=false);

	/**�ӵ�ǰ���ݿ������ж�ȡָ���񶯲��ĸ�������ֵ
	*@param sChann_ �񶯲����
	*@param pDB_  ���ݿ�����ָ��
	*@return ������ֵ
	*/
	S_VibCharValue ReadVibCharValue(string sChann_,CCustomDB *pDB_);

	S_VibCharValue ReadVibCharValue(string sChann_,IDBInterFace *pDB_);

	///�ж�ͨ�����������ࡣ���ж�ͨ��һ������
	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);

	//��ȡ���ǰ׺
	CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);



	/**�����豸��Ϣ�������Ϣ����ֹʱ�䣬�õ�������ڸף�����ͨ�����͵Ĳ��ı���״̬
	*@param arefDB		  [in]	   ���ݿ�ӿ�
	*@param arefFixComm	  [in]	   ͨ�Žӿ�
	*@param apPlantInfo   [in]     �豸��Ϣָ��
	*@param arefChanninfo [in]     ������������
	*@param aoletimeStart [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	  [in]     �жϱ�������ʱ��
	*@param  aenumType    [in]     �������
	*@return 0-������1-�쳣��2-��ȷ���� -1���ݴ���
	*/
	E_RC_AlarmStatus GetSpecChannTypeAlarmStatus(
			IDBInterFace						& arefDB,
			ICommInterFace						& arefFixComm,
			const CPlantInfoExpertSys			* apPlantInfo,
			const S_ChannInfo					& arefChanninfo,
			CBHDateTime						    & aoletimeStart,
			CBHDateTime						    & aoletimeEnd,
			ALARM_CHANN_TYPE aenumType = EXPERT_CHANN_TYPE_TEMP
		);


	/**�����豸��Ϣ�������Ϣ����ֹʱ�䣬�õ�������ڸף���������Ĵ���������״̬
	*@param arefDB		  [in]	   ���ݿ�ӿ�
	*@param arefFixComm	  [in]	   ͨ�Žӿ�
	*@param apPlantInfo   [in]     �豸��Ϣָ��
	*@param arefChanninfo [in]     ������������
	*@param aoletimeStart [in]     �жϹ��Ͽ�ʼʱ��
	*@param aoletimeEnd	  [in]     �жϹ��Ͻ���ʱ��
	*@param  aenumType    [in]     �������
	*@return  0-�񣨴�������������1-�ǣ����������ϣ���2-��ȷ����-1���ݴ���
	*/
	E_BoolExType GetSpecChannTypeSensorStatus(
			IDBInterFace						& arefDB,
			ICommInterFace						& arefFixComm,
			const CPlantInfoExpertSys			* apPlantInfo,
			const S_ChannInfo					& arefChanninfo,
			CBHDateTime						    & aoletimeStart,
			CBHDateTime						    & aoletimeEnd,
			ALARM_CHANN_TYPE aenumType = EXPERT_CHANN_TYPE_TEMP
		);

	int GetTempValueTrend(
		IDBInterFace						& arefDB,
		ICommInterFace						& arefFixComm,
		std::vector<double>			        & aorefTrendData,
		const CPlantInfoExpertSys			* apPlantInfo,
		const S_ChannInfo					& arefChanninfo,
		COleDateTime						& aoletimeStart,
		COleDateTime						& aoletimeEnd,
		BOOL						        & abIsValveGasIn
		);
	
	/**�����豸��Ϣ�������Ϣ����ֹʱ�䣬�õ�������ڸ׷����������¶�ͨ����ǰ��10���ӵ����ݣ�ֻȡ��һ�����ֵ��¶Ȳ��ı�������
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param aorefTrendData [in/out] ��������,������Ҫ�����100��
	*@param adblThreshold  [in/out] �������ݵ���ֵ
	*@param apPlantInfo    [in]     �豸��Ϣָ��
	*@param arefChanninfo  [in]     ������������
	*@param aoletimeStart  [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	   [in]     �жϱ�������ʱ��
	*@param  aenumType     [in]     �������
	*@return  0-û�б�����>0 �б������ݣ����ر����������ݵĵ���������aorefTrendData��size
	*/
	int GetSpecChannAlarmValueTempTrend(
			IDBInterFace				& arefDB,
			ICommInterFace				& arefFixComm,
			std::vector<double>			& aorefTrendData,
			double						& adblThreshold,
			const CPlantInfoExpertSys	* apPlantInfo,
			const S_ChannInfo			& arefChanninfo,
			CBHDateTime				    & aoletimeStart,
			CBHDateTime				    & aoletimeEnd,
			BOOL						&  abIsValveGasIn
		);

	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵�ײ������ײ������
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param apPlantInfo    [in]		�豸��Ϣָ��
	*@param arefChanninfo  [in]		������������
	*@return >=0 ײ������ֵ��-1���ݴ���
	*/
	int GetSpecImpactTimes(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
			  CPlantInfoExpertSys		* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
			  unsigned int				& airefTimes_, 
			  unsigned int				& airefTh_,
			  CBHDateTime				& aoletimeStart,
			  CBHDateTime				& aoletimeEnd
		);

	/**�õ���ϲ�����ڸ׵������������й��ϵ���Ŀ�Լ�û�й��������ı���ͨ����
	  *@param aiSensorFaults_  [in/out] �����������й��ϵĸ���
	  *@param aiAlarmChannels_ [in/out] û�д��������ϵ���������ͨ����
	  *@param aiChannels_      [in/out] ���ڸ׵ļ�������¶ȵ�ͨ����
	  */
	void GetCylinderValueAlarmsAndSensorFaults(
		 IDBInterFace					& arefDB,
		 ICommInterFace					& arefFixComm,
		 const CPlantInfoExpertSys		* apPlantInfo,
		 const S_ChannInfo				& arefChanninfo,
		 CBHDateTime					& aoletimeStart,
		 CBHDateTime					& aoletimeEnd,
		 unsigned int					& aiSensorFaults_, 		
		 unsigned int					& aiAlarmChannels_, 	
		 int							& aiChannels_);

	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵ı���ǰ���5s�ĳ��������ơ� ��������Ҫ����50����
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param aorefTrendData [in/out] ��������,������Ҫ�����50��
	*@param adblThreshold  [in/out] ����������ֵ����ʱ������
	*@param apPlantInfo    [in]     �豸��Ϣָ��
	*@param arefChanninfo  [in]     ������������
	*@param aoletimeStart  [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	   [in]     �жϱ�������ʱ��
	*@param  aenumType     [in]     �������
	*@return  0-û�б�����>0 �б������ݣ����ر����������ݵĵ���������aorefTrendData��size
	*/
	int GetSpecSinkTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		double							& adblThreshold,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& aRefFLoAlarmValue_, 
		double							& aRefFHiAlarmValue_, 
		double							& aRefFMaxRange_, 
		bool							& aRefBHiPass_
		);

	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵ĳ�������ʱ�̵��񶯲��Ρ���
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param aorefTrendData [in/out] ��������
	*@param anFs		   [in/out]	����Ƶ��
	*@param apPlantInfo    [in]     �豸��Ϣָ��
	*@param arefChanninfo  [in]     ������������
	*@param aoletimeStart  [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	   [in]     �жϱ�������ʱ��
	*@param  aenumType     [in]     �������
	*@return  0-û�б�����>0 �б������ݣ����ر����������ݵĵ���������aorefTrendData��size
	*/
	int GetSpecSinkWave(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefWaveData,
		int								& anFs,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& ldbThrethold
		);


	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵ı���ǰ���5s�ڵ�ƫ�������ơ� ��������Ҫ����50����
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param aorefTrendData [in/out] ��������,������Ҫ�����50��
	*@param adblThreshold  [in/out] ƫ��������ֵ����ʱ������
	*@param apPlantInfo    [in]     �豸��Ϣָ��
	*@param arefChanninfo  [in]     ������������
	*@param aoletimeStart  [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	   [in]     �жϱ�������ʱ��
	*@param  aenumType     [in]     �������
	*@return  0-û�б�����>0 �б������ݣ����ر����������ݵĵ���������aorefTrendData��size
	*/
	int GetSpecDeflectTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		double							& adblThreshold,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& aRefFLoAlarmValue_, 
		double							& aRefFHiAlarmValue_, 
		double							& aRefFMaxRange_, 
		bool							& aRefBHiPass_
		);
	int GetSpecRodCharTrend(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		E_ALARM_CHARTYPE                 aoCharType,
		GE_MEASURE_DIRECTION             aoMeasureDirection
		);

	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵�ƫ�ڱ���ʱ�̵��񶯲��Ρ���
	*@param arefDB		   [in]		���ݿ�ӿ�
	*@param arefFixComm	   [in]		ͨ�Žӿ�
	*@param aorefTrendData [in/out] ��������,
	*@param anFs		   [in/out]	����Ƶ��
	*@param apPlantInfo    [in]     �豸��Ϣָ��
	*@param arefChanninfo  [in]     ������������
	*@param aoletimeStart  [in]     �жϱ�����ʼʱ��
	*@param aoletimeEnd	   [in]     �жϱ�������ʱ��
	*@param  aenumType     [in]     �������
	*@return  0-û�б�����>0 �б������ݣ����ر����������ݵĵ���������aorefTrendData��size
	*/
	int GetSpecDeflectWave(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		int								& anFs,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					    & aoletimeStart,
		CBHDateTime					    & aoletimeEnd,
		double							& ldbThrethold
		);

	/**�õ�������ڸ���ʮ��ͷ�񶯱�����������㱨�������㵱ʱ������ʱ�̣�ǰ��Ĳ��Ρ����û�б���������Ϊ֮ǰһ��ʱ��(ǰ1��)�Ĳ���
	  *����ǰ1Сʱ�Ĳ���
	  *@param arefDB				[in]		���ݿ�ӿ�
	  *@param arefFixComm			[in]		ͨ�Žӿ�
	  *@param apPlantInfo			[in]		�豸��Ϣָ��
	  *@param arefChanninfo			[in]		������������
	  *@param aorefTrendDataBefore	[in/out]	����ǰ�Ĳ�������
	  *@param aiStartIndBeforeAlarm_[in/out]	����ǰ���ε�һ�����ڵ���ʼ����
	  *@param aiEndIndBeforeAlarm_  [in/out]	����ǰ���ε�һ�����ڵĽ�������
	  *@param aorefTrendDataAfter	[in/out]	������Ĳ�������
	  *@param aiStartIndAfterAlarm_ [in/out]	�������ε�һ�����ڵ���ʼ����
	  *@param aiEndIndAfterAlarm_	[in/out]	��������һ�����ڵĽ�������
	  *@param aiFsAfterAlarm_		[in/out]	�������εĲ���Ƶ��
	  *@return С��0��ʾ��ȡʧ��
	  */
	int GetCrossHeadWave(
		IDBInterFace					&	arefDB,
		ICommInterFace					&	arefFixComm,
		CPlantInfoExpertSys				*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		std::vector<double>				&	aorefTrendDataBefore,
		int								&	aiStartIndBeforeAlarm_, 
		int								&	aiEndIndBeforeAlarm_, 
		std::vector<double>				&	aorefTrendDataAfter,
		int								&	aiStartIndAfterAlarm_, 
		int								&	aiEndIndAfterAlarm_, 
		int								&   aiFsAfterAlarm_,
		CBHDateTime					    &	aoletimeStart,
		CBHDateTime					    &	aoletimeEnd,
		double							&	fLoAlarmValue_, 
		double							&	fHiAlarmValue_);

	/**�õ���ϲ�������������񶯱�����������㱨�������㵱ʱ������ʱ�̣�ǰ��Ĳ��Ρ����û�б���������Ϊ֮ǰһ��ʱ��(ǰ1��)�Ĳ���
	  *����ǰ1Сʱ�Ĳ���
	  *@param arefDB				[in]		���ݿ�ӿ�
	  *@param arefFixComm			[in]		ͨ�Žӿ�
	  *@param apPlantInfo			[in]		�豸��Ϣָ��
	  *@param arefChanninfo			[in]		������������
	  *@param aorefTrendDataBefore	[in/out]	����ǰ�Ĳ�������
	  *@param aiStartIndBeforeAlarm_[in/out]	����ǰ���ε�һ�����ڵ���ʼ����
	  *@param aiEndIndBeforeAlarm_  [in/out]	����ǰ���ε�һ�����ڵĽ�������
	  *@param aorefTrendDataAfter	[in/out]	������Ĳ�������
	  *@param aiStartIndAfterAlarm_ [in/out]	�������ε�һ�����ڵ���ʼ����
	  *@param aiEndIndAfterAlarm_	[in/out]	��������һ�����ڵĽ�������
	  *@param aiFsAfterAlarm_		[in/out]	�������εĲ���Ƶ��
	  *@return С��0��ʾ��ȡʧ��
	  */
	int GetCrankWave(
		IDBInterFace					&	arefDB,
		ICommInterFace					&	arefFixComm,
		CPlantInfoExpertSys				*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		std::vector<double>				&	aorefTrendDataBefore,
		int								&	aiStartIndBeforeAlarm_, 
		int								&	aiEndIndBeforeAlarm_, 
		std::vector<double>				&	aorefTrendDataAfter,
		int								&	aiStartIndAfterAlarm_, 
		int								&	aiEndIndAfterAlarm_, 
		int								&   aiFsAfterAlarm_,
		CBHDateTime					&	aoletimeStart,
		CBHDateTime					&	aoletimeEnd,
		double							&	fLoAlarmValue_, 
		double							&	fHiAlarmValue_);


	int GetCrankTrendData(
		IDBInterFace					& arefDB,
		ICommInterFace					& arefFixComm,
		std::vector<double>				& aorefTrendData,
		CPlantInfoExpertSys				* apPlantInfo,
		const S_ChannInfo				& arefChanninfo,
		CBHDateTime					& aoletimeStart,
		CBHDateTime					& aoletimeEnd,
		E_ALARM_CHARTYPE                aoCharType);
	/**�����豸��Ϣ,�����Ϣ���õ�������ڸ׵Ļ���һ��ת�٣�������¶Ȳ�㣬��ȡͬ�׵��񶯺Ͷ�̬����ת��
	*�������/��̬��㣬ֱ�ӷ��ز���ת��
	*@param apPlantInfo   �豸��Ϣָ��
	*@param arefChanninfo ������������
	*@param IDBInterFace  ���ݿ�ָ�������
	*@return >=0 ת��ֵ��-1���ݴ���
	*/
	int GetSpecSectionRev(
		const CPlantInfoExpertSys		*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		IDBInterFace					&	arefDB,
		CBHDateTime					&	aoletimeStart,
		CBHDateTime					&	aoletimeEnd);

	//��ȡ�쳣���ݵ��ת��
	int GetRevOfAbnormalData(
		const CPlantInfoExpertSys		*	apPlantInfo,
		const S_ChannInfo				&	arefChanninfo,
		IDBInterFace					&	arefDB,
		COleDateTime					&	aoletimeStart,
		COleDateTime					&	aoletimeEnd);



	/**�����������ݣ��Ѿ����汨�����ã��жϸߵͱ����ޣ��Լ��Ƿ�߱�
	*�������/��̬��㣬ֱ�ӷ��ز���ת��
	*@param aoRefTrend						��������������
	*@param aRefChannInfo					��������
	*@param [out] adblRefAlarmHValue		������ֵ������
	*@param [out] adblRefAlarmLValue		������ֵ������
	*@return								����True��ʾ��ͨ false ��ʾ��ͨ
	*/
	BOOL ParseTrendAlarmHighLowValue(
		std::vector<double>			    &	aorefTrendData,
		const S_ChannInfo				&	anRefChannInfo,
		double					        &	aRefFHiAlarmValue_,
		double				            &	aRefFLoAlarmValue_);


	CHANNTYPE TranlateChannType(ALARM_CHANN_TYPE aenumChannType);

	int GetSpecTypeChanns(const CPlantInfoExpertSys	* apPlantInfo,ALARM_CHANN_TYPE arChannType, vector<S_ChannInfo> &arChannInfo);

	int readfile(double * ag_fwave,int len_,CString strFile_);
};


#endif
