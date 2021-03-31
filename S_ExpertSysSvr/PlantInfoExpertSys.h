#pragma once
#include "PlantInfoBase.h"
#include <vector>

#define CROSSS_HEAD_LOCATION_TYPE 1
#define TEMP_INVAL_LOCATION_TYPE  3 //������
#define TEMP_OUTVAL_LOCATION_TYPE 4 //������

class CPlantInfoExpertSys: public CPlantInfoBase
{
public:
	CPlantInfoExpertSys();
	virtual ~CPlantInfoExpertSys(void);
	int GetSectionCrankNo();
	int GetCrossHeadLocationIndex(int anSectionNo);
	BOOL GetCrankChanninfo(std::vector<S_ChannInfo>& arefCrankChanninfos);
	BOOL GetCrossHeadChannInfo(std::vector<S_ChannInfo>& arefCrankChanninfos,int anSectionNo);
	BOOL IsValveGasIn(S_ChannInfo & aRefChann);
	void SetPlantFactName(CString strPlantFactName_);
	CString GetPlantFactName();
protected:
	///��ͣ����������ֵ�ֶ�
	vector<string>  _vSudCharValueField;
	///��ͣ�����Ĳ����ֶ�
	vector<string>  _vSudWaveField;
	//�񶯲�������ֵ�ֶ�
	vector<string>  _vVibCharValueField;
	///�񶯲��Ĳ����ֶ�
	vector<string>  _vVibWaveField;
	///��̬��������ֵ�ֶ�
	vector<string> _vDynCharValueField;
	///��̬���Ĳ����ֶ�
	vector<string> _vDynWaveField;
	//ר��ϵͳ�豸��ʵ����
	CString _strPlantFactName;
public:
	virtual void AddChannInfoEnd();
};
