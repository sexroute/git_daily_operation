#pragma once
#include "PlantInfoBase.h"
#include <vector>

#define CROSSS_HEAD_LOCATION_TYPE 1
#define TEMP_INVAL_LOCATION_TYPE  3 //吸气阀
#define TEMP_OUTVAL_LOCATION_TYPE 4 //排气阀

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
	///启停车测点的特征值字段
	vector<string>  _vSudCharValueField;
	///启停车测点的波形字段
	vector<string>  _vSudWaveField;
	//振动测点的特征值字段
	vector<string>  _vVibCharValueField;
	///振动测点的波形字段
	vector<string>  _vVibWaveField;
	///动态测点的特征值字段
	vector<string> _vDynCharValueField;
	///动态测点的波形字段
	vector<string> _vDynWaveField;
	//专家系统设备事实名称
	CString _strPlantFactName;
public:
	virtual void AddChannInfoEnd();
};
