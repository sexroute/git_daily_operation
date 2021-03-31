#include "StdAfx.h"
#include <MiddleWare.h>

#include <json/json.h>
#include <fstream>

#include <assert.h>
#include <cmath>
#include <cfloat>


int SetFieldWithStatus(ICommInterFace * pFixComm, const int field, const int status)
{
	if(NULL == pFixComm){
		return -1;
	}

	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->SetItem(field, status);
	pFixComm->Write(pFixComm->GetAci());

	return status;
}

int InitJsonStautsFile(
	const CString & statusFile)
{
	Json::Reader reader;
	Json::Value  root;

	CBHDateTime currentTime;
	root[FILE_MODIFIED_TIME] = currentTime.Format(gc_cTimeFormat).GetString();

	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	if(!outStatusStream.is_open()){
		HZLOG_ERROR(STR("无法创建%statusFile"));
		return -1;
	}

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	outStatusStream << convertedStream;
	outStatusStream.close();

	return 0;
}

int verifyJsonStautsFile(
	const CString & statusFile,
	const char *    description
	)
{
	int bRet = 0;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );

	bool openSuccessful = (inStatusStream.is_open() != 0);

	Json::Reader reader;
	Json::Value  root;

	CString currentTime = CBHDateTime::GetCurrentTime().Format(gc_cTimeFormat);

	/*
	 - 已存在 --> 仅更新时间(保留用户已有注释等)
	 - 存在但时间不对-->新建
	 - 不存在--> 新建
	 */
	if(openSuccessful){
		bool parseValid = reader.parse(inStatusStream, root);
		inStatusStream.close();

		if (parseValid)		{
			std::string lastUpdateTime =
				root.get(FILE_MODIFIED_TIME,
				         currentTime.GetString()
				        ).asString();

			root[FILE_MODIFIED_TIME] = currentTime.GetString();
		}
		else {
			root[FILE_DESCRIPTION] = description;

			bRet = -1;
		}
	}
	else
	{
		root[FILE_DESCRIPTION]  = description;

		bRet = -2;
	}

	root[FILE_MODIFIED_TIME] = currentTime.GetString();

	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	outStatusStream << convertedStream;
	outStatusStream.close();

	return bRet;
}


int verifyDAQHWStautsFile()
{
 	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_HW_STATUS_FILE;

	return verifyJsonStautsFile(statusFile, DAQ_HW_STATUS_FILE_DESCRIPTION);
}

int verifyDAQSWStautsFile()
{
 	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_SW_STATUS_FILE;

	return verifyJsonStautsFile(statusFile, DAQ_SW_STATUS_FILE_DESCRIPTION);
}

int updateJsonStatusFile(const char * file, const char * field, const char * value)
{
	int nRet = 0;

	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_SW_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_WARN(STR("无法打开文件%s, 尝试重建%s\n", statusFile));
		InitJsonStautsFile(statusFile);
		return -1;
	}


	/* 读取状态文件 */
	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid) {
		/*
		HZLOG_WARN(STR("无法解析文件%s, 尝试重建%s\n",
			statusFile,
			reader.getFormatedErrorMessages().c_str()));
		InitJsonStautsFile(statusFile); */
		return -2;
	}


	CBHDateTime current = CBHDateTime::GetCurrentTime();

	std::string sFileModifiedTime =
		root.get(FILE_MODIFIED_TIME, "").asString();
	CBHDateTime tFileModifiedTime;
	if(!tFileModifiedTime.ParseDateTime(sFileModifiedTime.c_str())){
		HZLOG_WARN(STR("无法读取文件%s上次更新时间, 尝试重建",
			statusFile));
		InitJsonStautsFile(statusFile);
		return -3;
	}


	/* 更新状态 */
	root[FILE_MODIFIED_TIME] = current.Format(gc_cTimeFormat).GetString();
  	root[field] = value;

	/* 写入状态文件 */
	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	outStatusStream << convertedStream;
	outStatusStream.close();

	return 0;
}


int updateDAQSWStautsFile(const char * field, const char * value)
{
 	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_SW_STATUS_FILE;

	int nRet = 0;
	
	try
	{
		nRet = updateJsonStatusFile(statusFile, field, value);
	}
	catch (...)
	{

		CString strLog;
		strLog.Format("更新json文件异常 %s@%d", __FUNCTION__, __LINE__);
		CHZLogManage::Error(strLog);

		nRet = -1;
	}


	return nRet;
}

/*
中间件判断异常数据的逻辑

1.	越界数据：
①	int型：绝对值大于1073741824（即230）
②	float型：转换成字符串后（%f）带有非数字的字符（非0…9、+、-、.）
2.	趋势波动的转速：
取相邻两次采集采到的转速：r(t-1)、r(t)，若|r(t)-r(t-1)|>1000，则判断转速波动过大
3.	不合理数据：
各特征值的合理取值范围：
①	撞击次数：0~16
②	温度：>-273
③	压力测点：
	平均压力：>0
	机械效率：0~100
	峭度：>0
	歪度：无范围
	单缸指示功率：>0
	吸气容积效率：0~100
	排气容积效率：0~100
	压缩指数：>0
	膨胀系数：>0
	实际压力比：>=1
④	往复机械加速度测点：
	峰值：>0
	峭度：>0
	歪度：无范围
⑤	旋转机械振动测点：
	通频幅值：>0
	各倍频幅值：>0
	各倍频相位：0~360
	残振：>0
	间隙电压：-24~-2
	直流量：-24~-2
	轴位移：无范围
⑥	往复机械速度测点：
	有效值：>0
	峭度：>0
	歪度：无范围
⑦	活塞杆应力测点：
	峰值活塞杆压力：>0
	峰值活塞杆张力：大于零
	反转角：0~360
⑧	活塞杆位置测点：
    沉降平均值：无范围
    峰峰值：>0
    活塞气缸余隙间隙：>0
    1、2倍频幅值：>0
    直流量：-24~-2
    间隙电压：-24~-2
*/

static string g_sAbnormalDataErrorMessage;
static void AbnormalData_SetLastError(const CString & error)
{
	g_sAbnormalDataErrorMessage = error;
}

string AbnormalData_GetLastError()
{
	return g_sAbnormalDataErrorMessage;
}

inline bool AbnormalData_IsOverflow(double value)
{
	if (_isnan(value) || !_finite (value))
		return true;

	return false;
}

inline bool AbnormalData_IsOverflow(int value)
{
	if (value < AD_MIX_INT || value > AD_MAX_INT)
		return true;

	return false;
}

inline bool AbnormalData_IsUnResonable(float value)
{
	return (value < 0);
}

inline bool AbnormalData_IsAmpUnResonable(float value)
{
	return (value < 0);
}

inline bool AbnormalData_IsPhaseUnResonable(float value)
{
	return (value < 0 || value > 360);
}

inline bool AbnormalData_IsKurtosisUnResonable(float value)
{
	return (value < 0);
}

inline bool AbnormalData_IsSkewnessUnResonable(float value)
{
	return false;
}

inline bool AbnormalData_IsgSEUnResonable(float value)
{
	return false;
}

inline bool AbnormalData_IsDCUnResonable(float value)
{
	return (value < -24 || value > -2);
}

inline bool AbnormalData_IsGapUnResonable(float value)
{
	return (value < -24 || value > -2);
}

inline bool AbnormalData_IsEfficiencyUnResonable(float value)
{
	return (value < 0 || value > 100);
}

inline bool AbnormalData_IsPreesureRatioUnResonable(float value)
{
	return (value < 1);
}

inline bool AbnormalData_IsImpactUnResonable(float value)
{
	return (value < 0 || value > 16);
}

inline bool AbnormalData_IsTempUnResonable(float value)
{
	return (value < -273);
}

inline bool AbnormalData_IsAngleUnResonable(float value)
{
	return (value < 0 || value > 360);
}

typedef bool (*UnResonableDataPredict)(float);

#define UNRESONABLE_PREDICT_INIT(OB) map<int, UnResonableDataPredict> OB
#define UNRESONABLE_PREDICT_BIND(OB, charIndex, predict) \
do { \
	OB.insert(make_pair<int, UnResonableDataPredict>(charIndex, predict)); \
} while(0)

bool AbnormalData_IsUnResonable(
	const map<int, UnResonableDataPredict> &mapCharPredict, int charIndex, float charVal)
{
	map<int, UnResonableDataPredict>::const_iterator it = mapCharPredict.find(charIndex);
	if (it == mapCharPredict.end())
		return false;

	return (*it->second)(charVal);
}

bool AbnormalData_IsCharOverflow(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return false;
}

bool AbnormalData_IsCharOverflow(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return false;
}

bool AbnormalData_IsCharResonable(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	return true;
}

#define ABNORMAL_ERROR(error) \
do{ \
	HZLOG_ERROR(error); \
	AbnormalData_SetLastError(error); \
}while(0)

bool AbnormalData_IsCharResonable(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	assert( NULL != pChann && NULL != pPlant);

	switch(pChann->_iChannType)
	{
	case GE_IMPACTCHANN:
		if (AbnormalData_IsImpactUnResonable(procChar)) {
			ABNORMAL_ERROR(STR("异常数据[不合理] : 撞击次数异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				procChar,
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return false;
		}
		break;
	case GE_TEMPCHANN:
		if (AbnormalData_IsTempUnResonable(procChar)) {
			ABNORMAL_ERROR(STR("异常数据[不合理] : 温度异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				procChar,
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return false;
		}
		break;
	default:
		break;
	}

	return true;
}

static int AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	S_VibCharValue * pVibChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, S_CharInfo>::iterator it = pChann->_charInfo.begin();
		it != pChann->_charInfo.end(); ++it)
	{
		charIndex = it->first;
		S_CharInfo &charInfo  = it->second;

		charVal = pVibChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("异常数据[越界] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("异常数据[不合理] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}


	return GE_DATA_VALUE_NO_ERROR;
}


static int AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	S_DynCharValue * pDynChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, S_CharInfo>::iterator it = pChann->_charInfo.begin();
		it != pChann->_charInfo.end(); ++it)
	{
		charIndex = it->first;
		S_CharInfo &charInfo  = it->second;

		charVal = pDynChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("异常数据[越界] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("异常数据[不合理] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				charInfo.GetCharName().c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}

	return GE_DATA_VALUE_NO_ERROR;
}


static int AbnormalData_IsCharValid(
	const map<int, UnResonableDataPredict> & mapCharPredict,
	const map<int, string> & mapCharDesc,
	S_DynCharValue * pDynChar,
	S_ChannInfo * pChann,
	CPlantInfoBase * pPlant)
{
	int   charIndex = 0;
	float charVal   = 0;
	for(map<int, S_CharInfo>::iterator it = pChann->_charInfo.begin();
		it != pChann->_charInfo.end(); ++it)
	{
		charIndex = it->first;
		S_CharInfo &charInfo  = it->second;

		string sCharName = getStringFormattedAs("特征值%d", charIndex);
		map<int, string>::const_iterator ir = mapCharDesc.find(charIndex);
		if(ir != mapCharDesc.end())
			sCharName = ir->second;

		charVal = pDynChar->GetSpecTypeData(charIndex);
		if(AbnormalData_IsOverflow(charVal)){
			ABNORMAL_ERROR(STR("异常数据[越界] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				sCharName.c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_BOUNDS_EXCEEDED;
		}
		else if (AbnormalData_IsUnResonable(mapCharPredict, charIndex, charVal)) {
			ABNORMAL_ERROR(STR("异常数据[不合理] : %s异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
				charVal,
				sCharName.c_str(),
				pChann->_cChannID, pChann->_cChannNo,
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
			return GE_DATA_VALUE_UNRESONABLE;
		}

	}


	return GE_DATA_VALUE_NO_ERROR;
}


static int AbnormalData_RotaryPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	assert(NULL != pVibChar && NULL != pChann && NULL != pPlant);

	if(AbnormalData_IsOverflow(pVibChar->_fCharAll)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 通频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharAll,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if (AbnormalData_IsAmpUnResonable(pVibChar->_fCharAll)) {
		ABNORMAL_ERROR(STR("异常数据[不合理] : 通频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharAll,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharHalf)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 1/2倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if (AbnormalData_IsAmpUnResonable(pVibChar->_fCharHalf)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 1/2倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}


	if(AbnormalData_IsOverflow(pVibChar->_fCharOne)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 1倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharOne)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 1倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharTwo)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 2倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharTwo)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 2倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharThree)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 3倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharThree)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 3倍频幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fCharRes)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 残振幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharRes,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsAmpUnResonable(pVibChar->_fCharRes)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 残振幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fCharRes,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fDC)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 直流量异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fDC,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsDCUnResonable(pVibChar->_fDC)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 直流量异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fDC,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fGap)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 间隙电压幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fGap,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsGapUnResonable(pVibChar->_fGap)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 间隙电压幅值异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fGap,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseHalf)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 1/2倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseHalf)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 1/2倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseHalf,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseOne)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 1倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseOne)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 1倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseOne,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseTwo)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 2倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseTwo)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 2倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseTwo,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	if(AbnormalData_IsOverflow(pVibChar->_fPhaseThree)){
		ABNORMAL_ERROR(STR("异常数据[越界] : 3倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_BOUNDS_EXCEEDED;
	}
	else if(AbnormalData_IsPhaseUnResonable(pVibChar->_fPhaseThree)){
		ABNORMAL_ERROR(STR("异常数据[不合理] : 3倍频相位异常(%f),测点(%s/%s) 公司(%s) 分厂(%s) 设备(%s/%s)",
			pVibChar->_fPhaseThree,
			pChann->_cChannID, pChann->_cChannNo,
			pPlant->GetCompanyName().c_str(),
			pPlant->GetFactoryName().c_str(),
			pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));
		return GE_DATA_VALUE_UNRESONABLE;
	}

	return GE_DATA_VALUE_NO_ERROR;
}

static int AbnormalData_ReciprocalPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	UNRESONABLE_PREDICT_INIT(mapCharPredict);

	switch(pChann->_iInfoType)
	{
	case 100:
		/* 往复机械加速度测点
		{100,0,"峰值",0,0,"m/s2"},
		{100,1,"峭度",0,0," "},
		{100,2,"歪度",0,0," "},
		*/
		if(pChann->_iSensorType == GE_SENSORTYPE_ACCELERATE){
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsKurtosisUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsSkewnessUnResonable);
		}
		break;
	case 101:
		/* 往复机械速度测点
		{101,0,"有效值",0,0,"mm/s"},
		{101,1,"峭度",0,0," "},
		{101,2,"歪度",0,0," "},
		*/
		if(pChann->_iSensorType == GE_SENSORTYPE_SPEED){
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsKurtosisUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsSkewnessUnResonable);
		}
		break;
	default:
		break;
	}

	return AbnormalData_IsCharValid(mapCharPredict, pVibChar, pChann, pPlant);
}

static int AbnormalData_WindElectroPlant_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	/* 非风电机泵测点可参照其它旋转测点判断*/
	if(pChann->_iInfoType != GE_MI_WINDPUMPCHARTYPE_ACC 
		&& pChann->_iInfoType != GE_MI_WINDPUMPCHARTYPE_VEL)
	{
		return AbnormalData_RotaryPlant_IsCharValid(pVibChar, pChann, pPlant);
	}

	/*
	0,"加速度峰值"
	1,"加速度高频"
	2,"加速度低频"
	3,"加速度波形峭度"
	4,"速度有效值"
	5,"通频gSE值"
	*/
	UNRESONABLE_PREDICT_INIT(mapCharPredict);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsAmpUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 3, AbnormalData_IsKurtosisUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 4, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 5, AbnormalData_IsgSEUnResonable);

	return AbnormalData_IsCharValid(mapCharPredict, pVibChar, pChann, pPlant);
}

int AbnormalData_IsCharValid(S_VibCharValue * pVibChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	const S_PlantInfo & plantInfo = pPlant->GetPlantInfo();

	if(plantInfo._iMachineType == GE_MACHINETYPE_RC || plantInfo._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		plantInfo._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || plantInfo._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR||
		plantInfo._iMachineType == GE_MACHINETYPE_ICE || plantInfo._iMachineType == GE_MACHINETYPE_DISEL)
		return AbnormalData_ReciprocalPlant_IsCharValid(pVibChar, pChann, pPlant);
	else if (plantInfo._iMachineType == GE_MACHINETYPE_WINDPEQ)
		return AbnormalData_WindElectroPlant_IsCharValid(pVibChar, pChann, pPlant);

	return AbnormalData_RotaryPlant_IsCharValid(pVibChar, pChann, pPlant);
}
static int AbnormalData_IsDynStressCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	/*
		峰值活塞杆压力：>0
		峰值活塞杆张力：大于零
		反转角：0~360
	*/
	map<int, string> mapCharDesc;
	mapCharDesc.insert(make_pair(0, "峰值活塞杆压力"));
	mapCharDesc.insert(make_pair(1, "峰值活塞杆张力"));
	mapCharDesc.insert(make_pair(2, "反转角"));

	UNRESONABLE_PREDICT_INIT(mapCharPredict);

	UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsUnResonable);
	UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsAngleUnResonable);

	return AbnormalData_IsCharValid(mapCharPredict, mapCharDesc, pDynChar, pChann, pPlant);
}

static int AbnormalData_ReciprocalPlant_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	UNRESONABLE_PREDICT_INIT(mapCharPredict);

	switch(pChann->_iInfoType)
	{
	case 111:
		/* 往复动态压力
		{111,0,"平均压力",0,0,"KPa"},
		{111,1,"机械效率",0,0,"%"},
		{111,2,"吸气容积效率",0,0,"%"},
		{111,3,"排气容积效率",0,0,"%"},
		{111,4,"单缸指示功率",0,0,"KW"},
		{111,5,"压缩指数",0,0," "},
		{111,6,"膨胀系数",0,0," "},
		{111,7,"实际压力比",0,0," "},
		{111,8,"峭度",0,0," "},
		{111,9,"歪度",0,0," "},
		*/
		if(pChann->_iChannType == GE_DYNPRESSCHANN){
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsEfficiencyUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsEfficiencyUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 3, AbnormalData_IsEfficiencyUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 4, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 5, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 6, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 7, AbnormalData_IsPreesureRatioUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 8, AbnormalData_IsKurtosisUnResonable);
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 9, AbnormalData_IsSkewnessUnResonable);
		}
		break;
	case 112:
		/* 活塞杆位置测点
		{112,0,"沉降量/偏摆量",0,0,"um"},
		{112,1,"峰-峰值",0,0,"um"},
		{112,2,"峰值",0,0,"um"},
		{112,3,"活塞-气缸剩余间隙",0,0,"um"},
		{112,4,"一倍频幅值",0,0,"um"},
		{112,5,"二倍频幅值",0,0,"um"},
		*/
		if(pChann->_iChannType == GE_RODSINKCHANN){
			//UNRESONABLE_PREDICT_BIND(mapCharPredict, 0, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 1, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 2, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 3, AbnormalData_IsUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 4, AbnormalData_IsDCUnResonable);
			UNRESONABLE_PREDICT_BIND(mapCharPredict, 5, AbnormalData_IsGapUnResonable);
		}
		break;
	default:
		if(pChann->_iChannType == GE_DYNSTRESSCHANN){
			return AbnormalData_IsDynStressCharValid(pDynChar, pChann, pPlant);
		}
		break;
	}

	return AbnormalData_IsCharValid(mapCharPredict, pDynChar, pChann, pPlant);
}

int AbnormalData_IsCharValid(S_DynCharValue * pDynChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	const S_PlantInfo & plantInfo = pPlant->GetPlantInfo();

	if(plantInfo._iMachineType == GE_MACHINETYPE_RC || plantInfo._iMachineType == GE_MACHINETYPE_PISTON_PUMP ||
		plantInfo._iMachineType == GE_MACHINETYPE_PLUNGER_PUMP || plantInfo._iMachineType == GE_MACHINETYPE_PISTON_COMPRESSOR ||
		plantInfo._iMachineType == GE_MACHINETYPE_ICE || plantInfo._iMachineType == GE_MACHINETYPE_DISEL)
	{
		return AbnormalData_ReciprocalPlant_IsCharValid(pDynChar, pChann, pPlant);
	}
	return GE_DATA_VALUE_NO_ERROR;
}

int AbnormalData_IsCharValid(float procChar, S_ChannInfo * pChann, CPlantInfoBase * pPlant)
{
	if(AbnormalData_IsCharOverflow(procChar, pChann, pPlant))
		return GE_DATA_BOUNDS_EXCEEDED;

	if(!AbnormalData_IsCharResonable(procChar, pChann, pPlant))
		return GE_DATA_VALUE_UNRESONABLE;

	return GE_DATA_VALUE_NO_ERROR;
}

int AbnormalData_IsRevValid(
	float * pfLastRev, float * pfCurrRev, int nRevNum,
	CPlantInfoBase  *  pPlant)
{
	assert(pPlant != NULL);

	for(int i = 0; i < nRevNum; i++){
		if(AbnormalData_IsOverflow(pfCurrRev[i])){
			ABNORMAL_ERROR(STR("异常数据[越界] : 转速%d越界(%f),公司[%s]分厂[%s]设备[%s/%s]",
				i,
				pfCurrRev[i],
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));

			return GE_DATA_BOUNDS_EXCEEDED;
		}

		if(fabs(pfCurrRev[i] - pfLastRev[i]) > 1000){
			ABNORMAL_ERROR(STR("异常数据[波动] : 转速%d波动过大(%f-->%f),公司[%s]分厂[%s]设备[%s/%s]",
				i,
				pfLastRev[i], pfCurrRev[i],
				pPlant->GetCompanyName().c_str(),
				pPlant->GetFactoryName().c_str(),
				pPlant->GetPlantID().c_str(), pPlant->GetPlantNo().c_str()));

			return GE_DATA_TREND_FLUCTUATING;
		}
	}

	return GE_DATA_VALUE_NO_ERROR;
}

bool DB_IsColumnInTable(CCustomDB *pDB_, const string &sColumnName_, const string &sTblName_)
{
	bool bRet = false;

	pDB_->Cmd("if exists(select * from syscolumns");
	pDB_->Cmd(" where name='%s' and id in (select id from sysobjects where name='%s'))",
		sColumnName_.c_str(),
		sTblName_.c_str());
	pDB_->Cmd(" select 1");
	if(pDB_->More() ){
		bRet = true;
	}

	return bRet;
}

bool DB_IsColumnInTable(IDBInterFace *pDB_, const string &sColumnName_, const string &sTblName_)
{
	bool bRet = false;

	pDB_->Cmd("if exists(select * from syscolumns");
	pDB_->Cmd(" where name='%s' and id in (select id from sysobjects where name='%s'))",
		sColumnName_.c_str(),
		sTblName_.c_str());
	pDB_->Cmd(" select 1");
	if(pDB_->More() ){
		bRet = true;
	}

	return bRet;
}


void SetNullReturnFixcomm(CFixComm * pFixComm )
{
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());
}

/*!
-1: 波形
-2: 类型
-3: 采样频率
-4: 采样点数
-5: 转速
*/
int SimADO_GetSampleFrequency( CCustomDB * pDB_, string sChannNo_)
{
	string sField = gc_cSampleFreq;

	sField.append("_");

	sField.append(sChannNo_);

	// 测点对应的采样频率
	int iSampleFreq = pDB_->GetLong(sField.c_str());
	if (iSampleFreq > 0) return iSampleFreq;

	// 公用的采样频率
	iSampleFreq = pDB_->GetLong(gc_cSampleFreq);
	return iSampleFreq;
}



int SimADO_GetSampleFrequency( CCustomDB * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetSampleFrequency(pDB_, channInfo._cChannNo);
}

int SimADO_GetSampleNum( CCustomDB * pDB_, string sChannNo_)
{
	string sField = gc_cSampleNum;

	sField.append("_");

	sField.append(sChannNo_);

	// 测点对应的采样点数
	int iSampleNum = pDB_->GetLong(sField.c_str());
	if (iSampleNum > 0) return iSampleNum;

	// 公用的采样点数
	iSampleNum = pDB_->GetLong(gc_cSampleNum);
	return iSampleNum;
}

int SimADO_GetSampleNum( CCustomDB * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetSampleNum(pDB_, channInfo._cChannNo);
}

int SimADO_GetRev( CCustomDB * pDB_, string sChannNo_, int iRevType_ )
{
	string sField = gc_cRev;

	sField.append("_");

	sField.append(sChannNo_);

	// 测点对应的转速
	int iRev = pDB_->GetLong(sField.c_str());
	if (iRev > 0) return iRev;

	CString sRevField;
	switch( iRevType_ )
	{
	case 2:
		sRevField = gc_cRev2;
		break;
	case 3:
		sRevField = gc_cRev3;
		break;
	case 4:
		sRevField = gc_cRev4;
		break;
	case 5:
		sRevField = gc_cRev5;
		break;
	case 1:
	default:
		sRevField = gc_cRev;
		break;
	}

	// 公用的转速
	iRev = pDB_->GetLong(sRevField);
	return iRev;
}

int SimADO_GetRev( CCustomDB * pDB_, string sChannNo_)
{
	return SimADO_GetRev(pDB_, sChannNo_, 0);
}

int SimADO_GetRev( CCustomDB * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetRev(pDB_, channInfo._cChannNo, channInfo._iRevType);
}



int SimADO_GetRevEx( IDBInterFace * pDB_, string sChannNo_, int iRevType_ )
{
	string sField = gc_cRev;
	sField.append(("_"));
	sField.append(sChannNo_);

	// 测点对应的转速
	int iRev = pDB_->GetLong(sField.c_str());
	if (iRev != 0) return iRev;

	CString sRevField;
	switch( iRevType_ )
	{
	case 2:
		sRevField = gc_cRev2;
		break;
	case 3:
		sRevField = gc_cRev3;
		break;
	case 4:
		sRevField = gc_cRev4;
		break;
	case 5:
		sRevField = gc_cRev5;
		break;
	case 1:
	default:
		sRevField = gc_cRev;
		break;
	}

	// 公用的转速
	iRev = pDB_->GetLong(sRevField);
	return iRev;
}

int SimADO_GetRevEx( IDBInterFace * pDB_, string sChannNo_)
{
	return SimADO_GetRevEx(pDB_, sChannNo_, 0);
}

int SimADO_GetRevEx( IDBInterFace * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetRevEx(pDB_, channInfo._cChannNo, channInfo._iRevType);
}

int SimADO_GetSampleFrequencyEx( IDBInterFace * pDB_, string sChannNo_)
{
	string sField = gc_cRev;
	sField.append(("_"));
	sField.append(sChannNo_);

	// 测点对应的转速
	int iRev = pDB_->GetLong(sField.c_str());

	sField = gc_cSampleFreq;
	sField.append(("_"));
	sField.append(sChannNo_);

	// 测点对应的采样频率
	int iSampleFreq = pDB_->GetLong(sField.c_str());
	if (iRev != 0) return iSampleFreq;

	// 公用的采样频率
	iSampleFreq = pDB_->GetLong(gc_cSampleFreq);
	return iSampleFreq;
}



int SimADO_GetSampleFrequencyEx( IDBInterFace * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetSampleFrequencyEx(pDB_, channInfo._cChannNo);
}

int SimADO_GetSampleNumEx( IDBInterFace * pDB_, string sChannNo_)
{
	string sField = gc_cRev;
	sField.append(("_"));
	sField.append(sChannNo_);

	// 测点对应的转速
	int iRev = pDB_->GetLong(sField.c_str());

	sField = gc_cSampleNum;
	sField.append(("_"));
	sField.append(sChannNo_);

	// 测点对应的采样点数
	int iSampleNum = pDB_->GetLong(sField.c_str());
	if (iRev != 0) return iSampleNum;

	// 公用的采样点数
	iSampleNum = pDB_->GetLong(gc_cSampleNum);
	return iSampleNum;
}

int SimADO_GetSampleNumEx( IDBInterFace * pDB_, const S_ChannInfo &channInfo )
{
	return SimADO_GetSampleNumEx(pDB_, channInfo._cChannNo);
}

CString SimAdo_Expression_ISNULL(
								 const CString & sColumn,
								 int   nDefault)
{
	CString expression;

	expression.Format("ISNULL([%s], %d) AS %s", sColumn, nDefault, sColumn);

	return expression;
}

CString SimAdo_Expression_ISNULL(
								 const CString & sColumn,
								 const CString & sDefault)
{
	CString expression;

	expression.Format("ISNULL([%s], '%s') AS %s", sColumn, sDefault, sColumn);

	return expression;
}

int AlarmStatus_ParseAlarmStatus(unsigned int iState_)
{
	int iStatus=0;
	if(iState_&0xFF000000)
	{
		if ((iState_&D_BHKD_AT_REGULAR)>0)
		{
			if((iState_&D_BHKD_AT_REGHIGH)>0)
				iStatus=2;
			else
				iStatus=1;
		}
		else if (iState_&D_BHKD_AT_FAST)
			iStatus=3;
		else if(iState_&D_BHKD_AT_SLOW)
			iStatus=4;
		else if(iState_&D_BHKD_AT_TREND)
			iStatus=5;
		else if(iState_&D_BHKD_AT_CUSTOM)
		{
			if((iState_&D_BHKD_AT_CUSTOMHIGH)>0)
				iStatus=7;
			else
				iStatus=6;
		}
		else
			iStatus=0;
	}

	return iStatus;
}

CMutexRWReader::CMutexRWReader(IReadWriteLock & aMutexRW ) 
{
	m_pMutexRW = &aMutexRW;

	this->Accquire();
}

CMutexRWReader::CMutexRWReader( IReadWriteLock * pMutexRW )
{
	this->m_pMutexRW = pMutexRW;

	this->Accquire();
}

CMutexRWReader::~CMutexRWReader()
{
	this->Release();
}

bool CMutexRWReader::Accquire()
{
	if (NULL == this->m_pMutexRW)
	{
		ASSERT(FALSE);
		return false;
	}

	this->m_pMutexRW->BeginRead();

	return true;
}

bool CMutexRWReader::Release()
{
	if (NULL == this->m_pMutexRW)
	{
		ASSERT(FALSE);
		return false;
	}

	this->m_pMutexRW->EndRead();

	return true;
}

CMutexRWWriter::CMutexRWWriter(IReadWriteLock & aMutexRW ) 
{
	this->m_pMutexRW = &aMutexRW;

	this->Accquire();
}

CMutexRWWriter::CMutexRWWriter( IReadWriteLock * pMutexRW )
{
	this->m_pMutexRW = pMutexRW;

	this->Accquire();	
}

CMutexRWWriter::~CMutexRWWriter()
{
	this->Release();
}

bool CMutexRWWriter::Accquire()
{
	if (NULL == this->m_pMutexRW)
	{
		ASSERT(FALSE);
		return false;
	}

	this->m_pMutexRW->BeginWrite();

	return true;
}

bool CMutexRWWriter::Release()
{
	if (NULL == this->m_pMutexRW)
	{
		ASSERT(FALSE);
		return false;
	}

	this->m_pMutexRW->EndWrite();


	return true;
}

namespace DBUtils
{
	CString ComposeDataTableName(const TCHAR * table_name, const TCHAR * suffix)
	{
		CString table_name_with_suffix;

		table_name_with_suffix.Format(_T("%s_%s"), table_name, suffix);

		return table_name_with_suffix;
	}
}

namespace CharInfoHelper
{
	typedef std::map<int, CharInfoList_t> InfoType_CharInfoList_Map_t;

	static InfoType_CharInfoList_Map_t G_InfoType_CharInfoList_Map;

	int AddCharInfo( int aInfoType, int aCharIndex, const std::string& aCharName, const std::string& aCharUnit )
	{
		InfoType_CharInfoList_Map_t::iterator it = CharInfoHelper::G_InfoType_CharInfoList_Map.find(aInfoType);

		if (it == CharInfoHelper::G_InfoType_CharInfoList_Map.end())
		{
			CharInfoList_t loCharInfoList;

			loCharInfoList.push_back(S_CharInfo(aCharIndex, aCharName, aCharUnit));

			CharInfoHelper::G_InfoType_CharInfoList_Map[aInfoType] = loCharInfoList;

			return 1;
		}
		else
		{
			CharInfoList_t& loCharInfoList = it->second;

			loCharInfoList.push_back(S_CharInfo(aCharIndex, aCharName, aCharUnit));

			return loCharInfoList.size();
		}
	}

	int Initialize( COleSqlServerDB& aDB )
	{
		int lnInfoType  = -1;
		int lnCharIndex = -1;

		std::string lsCharName;
		std::string lsCharUnit;

		int lnCount = 0;

		aDB.Cmd("SELECT [%s],[%s],[%s],[%s] FROM %s", gc_cInfoType, gc_cIndex, gc_cComment, gc_cUnit, gc_cMachInfoTypeTable);
		aDB.Cmd(" WHERE [%s]>=100 ORDER BY [%s],[%s]", gc_cInfoType, gc_cInfoType, gc_cIndex);
		while(aDB.More())
		{
			lnInfoType  = aDB.GetLong(gc_cInfoType);
			lnCharIndex = aDB.GetLong(gc_cIndex);

			lsCharName  = aDB.Get(gc_cComment);
			lsCharUnit  = aDB.Get(gc_cUnit);

			CharInfoHelper::AddCharInfo(lnInfoType, lnCharIndex, lsCharName, lsCharUnit);

			++lnCount;
		}

		return lnCount;
	}

	int Initialize( CSimADO *apDB )
	{
		ASSERT(apDB);

		int lnInfoType  = -1;
		int lnCharIndex = -1;

		std::string lsCharName;
		std::string lsCharUnit;

		int lnCount = 0;

		apDB->Cmd("SELECT [%s],[%s],[%s],[%s] FROM %s", gc_cInfoType, gc_cIndex, gc_cComment, gc_cUnit, gc_cMachInfoTypeTable);
		apDB->Cmd(" WHERE [%s]>=100 ORDER BY [%s],[%s]", gc_cInfoType, gc_cInfoType, gc_cIndex);
		while(apDB->More())
		{
			lnInfoType  = apDB->GetLong(gc_cInfoType);
			lnCharIndex = apDB->GetLong(gc_cIndex);

			lsCharName  = apDB->Get(gc_cComment);
			lsCharUnit  = apDB->Get(gc_cUnit);

			CharInfoHelper::AddCharInfo(lnInfoType, lnCharIndex, lsCharName, lsCharUnit);

			++lnCount;
		}

		return lnCount;
	}

	const CharInfoList_t& GetCharInfoList( int aInfoType )
	{
		static CharInfoList_t g_DummyCharInfoList;

		InfoType_CharInfoList_Map_t::iterator itCharInfoList =  CharInfoHelper::G_InfoType_CharInfoList_Map.find(aInfoType);

		if (itCharInfoList == CharInfoHelper::G_InfoType_CharInfoList_Map.end())
		{
			return g_DummyCharInfoList;
		}
		else
		{
			return itCharInfoList->second;
		}
	}

	std::string GetCharUnit(int aInfoType, int aCharIndex)
	{
		std::string lsCharUnit;

		if (aCharIndex < 0)
		{
			//ASSERT(FALSE);
			return lsCharUnit;
		}

		InfoType_CharInfoList_Map_t::iterator itInfoList =  CharInfoHelper::G_InfoType_CharInfoList_Map.find(aInfoType);

		if (itInfoList == CharInfoHelper::G_InfoType_CharInfoList_Map.end())
		{
			//ASSERT(FALSE);
			return lsCharUnit;
		}

		const CharInfoList_t& loCharInfoList = itInfoList->second;

		if (aCharIndex >= loCharInfoList.size())
		{
			//ASSERT(FALSE);
			return lsCharUnit;
		}

		const S_CharInfo& loCharInfo = loCharInfoList[aCharIndex];

		return loCharInfo.GetCharUnit();
	}
}

void SetNullReturnFixcommEx( ICommInterFace * pFixComm )
{
	pFixComm->CreateAnswer(pFixComm->GetFunc());
	pFixComm->CreateEndMak();
	pFixComm->Write(pFixComm->GetAci());

}

std::map<CString,CString> goMapFireSequece;

std::map<CString,CString> goMapTDCPhase;

std::map<CString,CString> goMapFireAdvancedAnglePhase;

std::map<CString,CString> goMapValveOpenPhase;

std::map<CString,CString> goMapValveClosePhase;

CConfigHelper::CConfigHelper()
{

}

CConfigHelper::~CConfigHelper()
{

}

int CConfigHelper::GetMonthTableUpdateFlag()
{
	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	lnData = ::GetPrivateProfileInt("SAVE_DATA",
		"G_UPDATEMONTHTABLE",
		G_UPDATEMONTHTABLE,
		cAppLocation);
	if (lnData<0)
	{
		lnData = G_UPDATEMONTHTABLE;
	}
	
	return lnData;

}

static int g_nFixTimeOut = -1;
int CConfigHelper::GetFixCommTimeOut()
{
	if (g_nFixTimeOut > 0)
	{
		return g_nFixTimeOut;
	}

	char cAppLocation[5000]={0};

	CString lstrData;

	int lnData = 0;

	GetCurrentDirectory(5000,cAppLocation);

	strcat(cAppLocation,"\\DseSvr.ini");

	lnData = ::GetPrivateProfileInt("SYSTEM","TIMEOUT",-1,cAppLocation);

	if (lnData<=0)
	{
		lnData = G_FIXCOMM_TIMEOUT;

		CString lstrTimeOut;

		lstrTimeOut.Format("%d",lnData);

		::WritePrivateProfileString("SYSTEM","TIMEOUT",lstrTimeOut,cAppLocation);
	}

	g_nFixTimeOut = lnData;

	return lnData;
}

bool CConfigHelper::GetAllPlantFireInfo()
{
	TCHAR cAppLocation[5000]={0};
	GetCurrentDirectory(5000,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");
	int lnBufferSize = 10240000;

	std::vector<TCHAR> loBuffer(lnBufferSize);
	std::vector<TCHAR> loBufferSequece(lnBufferSize);
	TCHAR *lpBuffer = &loBuffer.front();
	TCHAR *lpBufferSequece = &loBufferSequece.front();

	CString lstrSplitPlant = _T(";");
	CString lstrSplitValue = _T("=");
	CString lstrBuffer;
	CStringArray loPlantArray;
	CStringArray loValueArray;


	int lnData = ::GetPrivateProfileString("SYSTEM",
		"FIRE_SEQUENCE"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	

	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();

	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);

	for (int lnDataIndex = 0; lnDataIndex < loPlantArray.GetCount(); lnDataIndex++)
	{
		CString lstrPair = loPlantArray.GetAt(lnDataIndex);

		loValueArray.RemoveAll();

		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);

		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);

			goMapFireSequece[lstrKey] = lstrValue;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrValue));
		}
	}

	lnData = ::GetPrivateProfileString("SYSTEM",
		"TDC_PHASE"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	

	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();

	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);

	for (int lnDataIndex = 0; lnDataIndex < loPlantArray.GetCount(); lnDataIndex++)
	{
		CString lstrPair = loPlantArray.GetAt(lnDataIndex);

		loValueArray.RemoveAll();

		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);

		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);

			goMapTDCPhase[lstrKey] = lstrValue;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrValue));
		}
	}

	lnData = ::GetPrivateProfileString("SYSTEM",
		"FIRE_ADVANCE_ANGLE_PHASE"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	

	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();

	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);

	for (int lnDataIndex = 0; lnDataIndex < loPlantArray.GetCount(); lnDataIndex++)
	{
		CString lstrPair = loPlantArray.GetAt(lnDataIndex);

		loValueArray.RemoveAll();

		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);

		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);

			goMapFireAdvancedAnglePhase[lstrKey] = lstrValue;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrValue));
		}
	}

	lnData = ::GetPrivateProfileString("SYSTEM",
		"VALVE_OPEN_PHASE"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	

	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();

	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);

	for (int lnDataIndex = 0; lnDataIndex < loPlantArray.GetCount(); lnDataIndex++)
	{
		CString lstrPair = loPlantArray.GetAt(lnDataIndex);

		loValueArray.RemoveAll();

		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);

		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);

			goMapValveOpenPhase[lstrKey] = lstrValue;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrValue));
		}
	}

	lnData = ::GetPrivateProfileString("SYSTEM",
		"VALVE_CLOSE_PHASE"
		,_T(""),
		lpBuffer,
		lnBufferSize,
		cAppLocation);	

	lstrBuffer = lpBuffer;
	loPlantArray.RemoveAll();

	GetSplitStrArray(lstrBuffer,lstrSplitPlant,loPlantArray,TRUE);

	for (int lnDataIndex = 0; lnDataIndex < loPlantArray.GetCount(); lnDataIndex++)
	{
		CString lstrPair = loPlantArray.GetAt(lnDataIndex);

		loValueArray.RemoveAll();

		GetSplitStrArray(lstrPair,lstrSplitValue,loValueArray,TRUE);

		if (loValueArray.GetSize()==2)
		{
			CString lstrKey = loValueArray.GetAt(0);
			CString lstrValue = loValueArray.GetAt(1);

			goMapValveClosePhase[lstrKey] = lstrValue;
			BHLOG_DEBUG(STR(_T("%s|%s"),lstrKey,lstrValue));
		}
	}

	return true;
}

INT CConfigHelper::GetSplitStrArray(const CString& strData,
					 const CString& strSplitFlag,
					 CStringArray &array,
					 BOOL SequenceAsOne)
{
	CString sData = strData;

	CString sSplitFlag = strSplitFlag;

	int lnSplitFlagLength = strSplitFlag.GetLength();

	if (sData.Right(1) != sSplitFlag) sData +=sSplitFlag;

	CString sTemp;

	int pos =-1;

	while ((pos=sData.Find(sSplitFlag,0)) != -1)
	{
		sTemp = sData.Left(pos);

		if (!SequenceAsOne)
		{
			array.Add(sTemp);
		}
		else
		{
			if (!sTemp.IsEmpty() && sTemp !="") ////连续的分隔符视为单个处理
			{
				array.Add(sTemp);
			}
		}
		sData = sData.Right(sData.GetLength() - pos - lnSplitFlagLength);
	}

	return array.GetSize();
}

CString CConfigHelper::GetDieselPlantType( CString strPlantNo_ )
{
	char cAppLocation[FILE_PATH_LEN]={0};
	char sDieselPlantType[1024]={0};
	GetCurrentDirectory(FILE_PATH_LEN,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	CString lstrSection = strPlantNo_.MakeUpper() + "_DIESEL_TYPE";

	GetPrivateProfileString("ALARM",lstrSection.GetBuffer(),"",sDieselPlantType,1023,cAppLocation);

	CString lstrDieselPlant = sDieselPlantType;

	lstrSection.ReleaseBuffer();

	return lstrDieselPlant;
}

int CConfigHelper::GetAlarmCount()
{
	char cAppLocation[FILE_PATH_LEN]={0};
	GetCurrentDirectory(FILE_PATH_LEN,cAppLocation);
	strcat(cAppLocation,"\\DseSvr.ini");

	int lnData = ::GetPrivateProfileInt("ALARM",
		"G_ALARM_COUNT"
		,-1,
		cAppLocation);

	if(lnData <= 0)
	{
		lnData = 6;
		WritePrivateProfileString("ALARM","G_ALARM_COUNT","6",cAppLocation);
	}

	return lnData;
}

float CConfigHelper::GetAlarmRatio( CString strChannNo_ )
{
	char pDir[FILE_PATH_LEN] = {0};

	char pValue[100] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	float lfRatio = 0.5;

	DWORD lnCount = GetPrivateProfileString("ALARM",strChannNo_,"",pValue,100,pDir);

	if(lnCount <= 0)
	{
		::WritePrivateProfileString("ALARM",strChannNo_,"0.8",pDir);
	}
	else
	{
		CString lstrRatio = pValue;
		lfRatio = atof(lstrRatio);
	}

	return lfRatio;
}

float CConfigHelper::GetPowerRatio( CString strPlantNo_ )
{
	char pDir[FILE_PATH_LEN] = {0};

	char pValue[100] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	float lfRatio = 1.0;

	DWORD lnCount = GetPrivateProfileString("POWER",strPlantNo_,"",pValue,100,pDir);

	if(lnCount <= 0)
	{
		::WritePrivateProfileString("POWER",strPlantNo_,"1.0",pDir);
	}
	else
	{
		CString lstrRatio = pValue;
		lfRatio = atof(lstrRatio);
	}

	return lfRatio;
}

float CConfigHelper::GetPowerSpliteFactor( CString strPlantNo_ )
{
	char pDir[FILE_PATH_LEN] = {0};

	char pValue[100] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	float lfRatio = 1.0;

	DWORD lnCount = GetPrivateProfileString("POWER_SPLITE",strPlantNo_,"",pValue,100,pDir);

	if(lnCount <= 0)
	{
		::WritePrivateProfileString("POWER_SPLITE",strPlantNo_,"1.0",pDir);
	}
	else
	{
		CString lstrRatio = pValue;
		lfRatio = atof(lstrRatio);
	}

	return lfRatio;
}


int CConfigHelper::GetLowPressRevIndex()
{
	char pDir[FILE_PATH_LEN] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	int lnIndex = ::GetPrivateProfileInt("ALARM","LOWPRESS",-100,pDir);

	if(lnIndex == -100)
	{
		lnIndex = -1;

		::WritePrivateProfileString("ALARM","LOWPRESS","-1",pDir);
	}

	return lnIndex;


}

int CConfigHelper::GetHighPressRevIndex()
{
	char pDir[FILE_PATH_LEN] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	int lnIndex = ::GetPrivateProfileInt("ALARM","HIGHPRESS",-100,pDir);

	if(lnIndex == -100)
	{
		lnIndex = -1;

		::WritePrivateProfileString("ALARM","HIGHPRESS","-1",pDir);
	}

	return lnIndex;
}

int CConfigHelper::GetPowerIndex(CString strPlantNo_)
{
	char pDir[FILE_PATH_LEN] = {0};

	::GetCurrentDirectory(FILE_PATH_LEN,pDir);

	strcat(pDir,_T("\\AlarmConfig.ini"));

	CString lstrSection = strPlantNo_.MakeUpper();

	lstrSection.Append("_POWER");

	int lnIndex = ::GetPrivateProfileInt("ALARM",lstrSection,-100,pDir);

	if(lnIndex == -100)
	{
		lnIndex = -1;

		::WritePrivateProfileString("ALARM",lstrSection,"-1",pDir);
	}

	return lnIndex;
}

bool CConfigHelper::GetDiagnoseInterface( CString& strIP_,int& nPort_ )
{
	char pDir[1000] = {0};

	char pValue[100] = {0};

	::GetCurrentDirectory(1000,pDir);

	strcat(pDir,_T("\\DseSvr.ini"));


	DWORD lnCount = GetPrivateProfileString("ALARM","JAVA_IP","",pValue,100,pDir);

	if(lnCount <= 0)
	{
		strIP_ = "127.0.0.1";
		::WritePrivateProfileString("ALARM","JAVA_IP","127.0.0.1",pDir);
	}
	else
	{
		strIP_ = pValue;
	}

	nPort_ = GetPrivateProfileInt("ALARM","JAVA_PORT",-1,pDir);
	if(nPort_ < 0)
	{
		nPort_ = 0;
		::WritePrivateProfileString("ALARM","JAVA_PORT","0",pDir);
	}

	if(nPort_ <= 0)
	{
		return false;
	}

	return true;
}

int CConfigHelper::UseAIAlarm()
{
	char pDir[1024] = {0};

	char pData[100] = {0};

	::GetCurrentDirectory(1024,pDir);

	strcat(pDir,"\\DseSvr.ini");

	int lnUseAIAlarm = ::GetPrivateProfileInt("ALARM","USE_AI_ALARM",-1,pDir);

	if(lnUseAIAlarm < 0)
	{
		lnUseAIAlarm = 1;
		::WritePrivateProfileString("ALARM","USE_AI_ALARM","1",pDir);
	}

	return lnUseAIAlarm;
}