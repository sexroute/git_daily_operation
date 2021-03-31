#pragma once

#include "stdafx.h"
#include "PlantInfo.h"
#include "additions/ReaderWriterlock.h"
#include <additions/CTimeOutReadWriteLock.h>
class CNetOffPlantInfo: public CPlantInfoOnLine
{
public:
	 CNetOffPlantInfo();
	 ~CNetOffPlantInfo();
private:
	///��ʷ������Ҫ�ĸ��������ͱ���
	string        _sProcField;
	string        _sVibFieldSel;
	string        _sVibFieldIns;
	string        _sDynFieldSel;
	string        _sDynFieldIns;
	// initialize current ID
	int _iEventID;
	int _iCurrEventID;

	CTimeOutReadWriteLock _oPrivteDataLock;

	CBHDateTime _timeNetOffSaveVibHour;
	CBHDateTime _timeNetOffSaveVibDay;
	CBHDateTime _timeNetOffSaveVibMonth;

	CBHDateTime _timeNetOffSaveDynHour;
	CBHDateTime _timeNetOffSaveDynDay;
	CBHDateTime _timeNetOffSaveDynMonth;

	CBHDateTime _timeNetOffSaveProcHour;
	CBHDateTime _timeNetOffSaveProcDay;
	CBHDateTime _timeNetOffSaveProcMonth;

    CRealPlantInfo* _pRealPlantInfo;

	

	virtual void SetVibTrendSecondCurrID(const int &iCurrIDVibTrendSecond_);
	virtual void SetVibTrendHourCurrID(const int &iCurrIDVibTrendHour_);
	virtual void SetVibTrendDayCurrID(const int &iCurrIDVibTrendDay_);
	virtual void SetVibTrendMonthCurrID(const int &iCurrIDVibTrendMonth_);

	virtual void SetDynTrendSecondCurrID(const int &iCurrIDDynTrendSecond_);
	virtual void SetDynTrendHourCurrID(const int &iCurrIDDynTrendHour_);
	virtual void SetDynTrendDayCurrID(const int &iCurrIDDynTrendDay_);
	virtual void SetDynTrendMonthCurrID(const int &iCurrIDDynTrendMonth_);

	virtual void SetProcTrendSecondCurrID(const int &iCurrIDProcTrendSecond_);
	virtual void SetProcTrendHourCurrID(const int &iCurrIDProcTrendHour_);
	virtual void SetProcTrendDayCurrID(const int &iCurrIDProcTrendDay_);
	virtual void SetProcTrendMonthCurrID(const int &iCurrIDProcTrendMonth_);

	virtual void GetVibTrendSecondCurrID(int &iCurrIDVibTrendSecond_);
	virtual void GetVibTrendHourCurrID(int &iCurrIDVibTrendHour_);
	virtual void GetVibTrendDayCurrID(int &iCurrIDVibTrendDay_);
	virtual void GetVibTrendMonthCurrID(int &iCurrIDVibTrendMonth_);

	virtual void GetDynTrendSecondCurrID(int &iCurrIDDynTrendSecond_);
	virtual void GetDynTrendHourCurrID(int &iCurrIDDynTrendHour_);
	virtual void GetDynTrendDayCurrID(int &iCurrIDDynTrendDay_);
	virtual void GetDynTrendMonthCurrID(int &iCurrIDDynTrendMonth_);

	virtual void GetProcTrendSecondCurrID(int &iCurrIDProcTrendSecond_);
	virtual void GetProcTrendHourCurrID(int &iCurrIDProcTrendHour_);
	virtual void GetProcTrendDayCurrID(int &iCurrIDProcTrendDay_);
	virtual void GetProcTrendMonthCurrID(int &iCurrIDProcTrendMonth_);

	virtual void Destory();
public:

	virtual CRealPlantInfo * GetRealPlantInfo();
	virtual void AddChannInfoEnd(CCustomDB * pDb_);
	virtual void AddChannInfoEnd(IDBInterFace * pDb_);
	//�Ѷ���������ʱ������Ϊ��ǰ���ݵ�ʱ��
	void SetVibDataTime(CBHDateTime dataTime_);
	//�Ѷ�����̬����ʱ������Ϊ��ǰ���ݵ�ʱ��
	void SetDynDataTime(CBHDateTime dataTime_);
	//�Ѷ�������������ʱ������Ϊ��ǰ���ݵ�ʱ��
	void SetProcDataTime(CBHDateTime dataTime_);

	//�õ���ǰ����������ʱ��
	CBHDateTime GetCurrVibDataTime();
	//�õ���ǰ������̬����ʱ��
	CBHDateTime GetCurrDynDataTime();
	//�õ���ǰ��������������ʱ��
	CBHDateTime GetCurrProcDataTime();

	void SetVibDataStatu(IDBInterFace * pDB_,bool bSave_);
	void SetDynDataStatu(IDBInterFace * pDB_,bool bSave_);
	void SetProcDataStatu(IDBInterFace * pDB_,bool bSave_);

	void SaveVibNetOffData(IDBInterFace *pDB_);
	void SaveDynNetOffData(IDBInterFace *pDB_);
	void SaveProcNetOffData(IDBInterFace *pDB_);

	void SetIDPointer(CRealPlantInfo *pRealPlantInfo_);

	// �����Ƽ�¼����
	void SaveVibRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_=false);
	// ��̬������Ƽ�¼����
	void SaveDynRecord(IDBInterFace * pDB_,const string &sTableName_,const int &iCurID_,const bool &bMonth_=false);
	// ���������Ƽ�¼����
	void SaveProcRecord(IDBInterFace * pDB_,string sTableName_,int iCurID_,bool bMonth_=false);

	// ������ݱ���
	int SaveFastTrendRecord(IDBInterFace *pDB_,E_TBL_CHANNTYPE eChannType_);

	//�����񶯿����������
	int SaveVibFastTrend(IDBInterFace * pDB_);
	//���涯̬�����������
	int SaveDynFastTrend(IDBInterFace * pDB_);
	//��������������������
	int SaveProcFastTrend(IDBInterFace * pDB_);

	CReaderWriterLock m_oWriteToDbLock;
	CReaderWriterLock m_oDynWriteToDbLock;
	CReaderWriterLock m_oProcWriteToDbLock;
};