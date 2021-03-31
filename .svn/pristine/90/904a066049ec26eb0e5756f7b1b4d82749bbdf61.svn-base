#pragma once
#ifndef	  _MW_BIZZOBJ_
#define   _MW_BIZZOBJ_
#define _DEFAULT_BO_ERROR_CODE_ -1
#define _DEFAULT_BO_NOT_AVAILABLE_ -100
#include <additions/IBusiObjectInterFace.h>
#include <Synopsis2KB.h>
#include <map>
#include "Implement.h"


/// buffer类型
typedef vector<unsigned char> BUFFER;

/// 数据接口模板
template <typename KEY_TYPE>
struct S_DATA_SET_TEMPL 
{
	std::map<KEY_TYPE, int> _mapInteger;				///< 整型参数
	std::map<KEY_TYPE, double> _mapDouble;				///< 浮点型参数
	std::map<KEY_TYPE, string> _mapString;			    ///< 字符串参数
	std::map<KEY_TYPE, BUFFER> _mapBuffer;				///< buffer参数
	std::map<KEY_TYPE, CBHDateTime> _mapDate;			///< Date参数

	void clear()
	{
		this->_mapInteger.clear();
		this->_mapDouble.clear();
		this->_mapDate.clear();
		this->_mapBuffer.clear();
		this->_mapString.clear();
	}
};

typedef S_DATA_SET_TEMPL<int> S_DataInterface;
typedef S_DATA_SET_TEMPL<string> S_DBDataInterface;


class CBHExpertBizObj : public IBHBusinessObject,public CImplement
{
public:
	virtual BOOL CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual int WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual int Do(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	S_DATA_SET_TEMPL<int> m_map_in;
	S_DATA_SET_TEMPL<int> m_map_out;
	S_DATA_SET_TEMPL<string> m_map_in_s;
	S_DATA_SET_TEMPL<string> m_map_out_s;
	S_DATA_SET_TEMPL<int> m_map_pass;
	S_DATA_SET_TEMPL<string>m_map_pass_s;
	

	CBHExpertBizObj();
	virtual ~CBHExpertBizObj();
};


class CBODiagnosisW : public CBHExpertBizObj
{
public:
	virtual BOOL CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual int WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	CBODiagnosisW();
	virtual ~CBODiagnosisW();

private:
	std::map<string,int> moMapRet;
};


class CBODiagnosisRC : public CBHExpertBizObj
{
public:
	virtual BOOL CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual BOOL ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);
	virtual int WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	CBODiagnosisRC()
	{
		this->m_plantInfo = NULL;
	};
	virtual ~CBODiagnosisRC()
	{

	};


	
	public:

	/*设置测点所在缸体的各个监测点的报警状态*/
	virtual void SetCylinderAlarmStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	/*获取各个监测点的传感器报警状态传感器*/
	void GetSensorsAlarmStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,
		                        E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
								E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
								E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
								E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV);

	/*设置测点所在缸体的各个监测点的传感器故障状态*/
	 void SetCylinderSensorsStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,
									 E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
									 E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
									 E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
									 E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV);

	 /*运行期间，设置传感器故障状态*/
	 void SetCylinderSensorsStatusInRunningStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,int anRev,
												 E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
												 E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
												 E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
												 E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV);

	/*设置诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数*/
	virtual void SetCylinderValueAlarmsAndSensorFaults(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	/*设置报警温度测点前后10分钟内的数据*/
	virtual void SetAlarmValueTempTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	/*设置诊断测点所在缸撞击次数*/
	virtual void SetImpactTimes(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置第一个报警点报警前后10s内的沉降量趋势。 点数不需要超过50个。报警前后各5s数据*/
	virtual int SetSinkTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置诊断测点沉降报警时刻的振动波形。没有报警可以不设置*/
	virtual int SetSinkWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置报警前后10s内的偏摆量。 点数不需要超过50个*/
	virtual int SetDeflectTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置诊断测点偏摆报警时刻的波形*/
	virtual int SetDeflectWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
	virtual int SetCrossHeadWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

	
	/*设置诊断所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
	virtual int SetCrankWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace);

static FixForCompilerError(CBODiagnosisRC & arefThis,ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,int anSeq)
{
	switch(anSeq)
	{
		case 1:
			//arefThis.SetCylinderSensorsStatus(arefFixComm,arefDBIntefFace);
			break;

		case 2:
			arefThis.SetCylinderValueAlarmsAndSensorFaults(arefFixComm,arefDBIntefFace);
			break;

		case 3:
			arefThis.SetImpactTimes(arefFixComm,arefDBIntefFace);
			break;

		case 4:
			arefThis.SetSinkWave(arefFixComm,arefDBIntefFace);
			break;

		case 5:
			arefThis.SetDeflectWave(arefFixComm,arefDBIntefFace);
			break;

		default:
			break;

	}
}

private:
	CDiagnosisRC m_oDiagnosisRC;

	CPlantInfoExpertSys * m_plantInfo;	

	S_ChannInfo m_oChannInfo;
	
	//诊断的起始时间
	CBHDateTime m_oOleTimeStart;
	
	//诊断的结束时间
	CBHDateTime m_oOleTimeEnd;


};
#endif