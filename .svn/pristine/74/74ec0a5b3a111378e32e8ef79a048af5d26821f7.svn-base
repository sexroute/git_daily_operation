#include "stdafx.h"
#include ".\businessobject.h"
#include <additions\DebugHelper.h>
#include <map>



BOOL CBHExpertBizObj::CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	return TRUE;
}
BOOL CBHExpertBizObj::PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	return TRUE;
}
BOOL CBHExpertBizObj::ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	return TRUE;
}
int CBHExpertBizObj::WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{

	return FALSE;
}
int CBHExpertBizObj::Do(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	int lnRet = (this->CheckPara(arefFixComm,arefDBIntefFace));

	if (lnRet>0)
	{
		lnRet = (this->PrepareData(arefFixComm,arefDBIntefFace));

		if (lnRet>0)
		{
			lnRet = (this->ProcessData(arefFixComm,arefDBIntefFace));

			if (lnRet>0)
			{
				lnRet = (this->WriteBackData(arefFixComm,arefDBIntefFace));
			}
		}
	}

	return lnRet;	
}


BOOL CBODiagnosisW::CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	return TRUE;
}


BOOL CBODiagnosisW::PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	return TRUE;
}

BOOL CBODiagnosisW::ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	this->moMapRet["S-RM-TestPosi"] = 4;
	this->moMapRet["S-RM-GIEV"] = 2;
	this->moMapRet["S-RM-RBearStyle"] = 1;
	this->moMapRet["S-RM-EFre"] = 3;
	this->moMapRet["S-RM-GearFreT"] = 2;
	this->moMapRet["S-RM-StateElv-TestPo"] = 3;
	this->moMapRet["S-RM-SpeedTrend"] = 2;
	this->moMapRet["S-RM-SEA-GFre"] = 2;
	this->moMapRet["S-RM-SEA-LFre"] = 1;
	this->moMapRet["S-RMP1-SpeedTrend"] = 1;
	this->moMapRet["S-RMP2-SpeedTrend"] = 1;
	this->moMapRet["S-RMP1-SpeedTrend"] = 2;

	return TRUE;
}

BOOL CBODiagnosisW::WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	arefFixComm.CreateAnswerMul(arefFixComm.GetFunc());

	int lnDataCount = 0;

	for (std::map<string,int>::iterator loIt =  this->moMapRet.begin();loIt!= moMapRet.end();loIt++)
	{
		zdlTraceLine(_T("%s | %d \r\n"),loIt->first.c_str(),loIt->second);

		this->WriteFertureInfo(loIt->first,loIt->second,&arefFixComm);

		lnDataCount++;

	}

	arefFixComm.CreateEndMak();

	arefFixComm.Write(arefFixComm.GetAci());

	return lnDataCount;
}

CBODiagnosisW::CBODiagnosisW()
{

}

CBODiagnosisW::~CBODiagnosisW()
{

}


CBHExpertBizObj::CBHExpertBizObj()
{

}

CBHExpertBizObj::~CBHExpertBizObj()
{

}



BOOL CBODiagnosisRC::CheckPara(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{

	this->m_map_in.clear();
	this->m_map_in_s.clear();

	this->m_map_out.clear();
	this->m_map_out_s.clear();

	this->m_map_pass.clear();
	this->m_map_pass_s.clear();


	this->m_map_in._mapDate[FIELD_SERVICE_OFF_TIMESTART] = arefFixComm.GetTime(FIELD_SERVICE_OFF_TIMESTART);
	this->m_map_in._mapDate[FIELD_SERVICE_OFF_TIMEEND]   = arefFixComm.GetTime(FIELD_SERVICE_OFF_TIMEEND);

	this->m_map_in._mapString[FIELD_SERVICE_CHANN1]		 = arefFixComm.Get(FIELD_SERVICE_CHANN1);

	this->m_map_in._mapString[FIELD_SYSTEM_COMPANY]		 = arefFixComm.Get(FIELD_SYSTEM_COMPANY);
	this->m_map_in._mapString[FIELD_SYSTEM_FACTORY]		 = arefFixComm.Get(FIELD_SYSTEM_FACTORY);
	this->m_map_in._mapString[FIELD_SERVICE_PLANTID]     = arefFixComm.Get(FIELD_SERVICE_PLANTID);	



	//1.读取设备信息
	this->m_plantInfo = this->ReadCheckCompanyInfo( this->m_map_pass_s._mapString[gc_cCompany],
		this->m_map_pass_s._mapString[gc_cFactory_Name],
		this->m_map_pass_s._mapString[gc_cPlantID],
		this->m_map_pass_s._mapString[gc_cSuffix],
		&arefFixComm);

	if (NULL == this->m_plantInfo)
	{
		return _DEFAULT_BO_ERROR_CODE_;
	}

	string lstrChannName = this->m_map_in._mapString[FIELD_SERVICE_CHANN1];

	//2.得到测点信息，判断是什么测点
	this->m_oChannInfo = this->m_plantInfo->GetSpecVibChannInfo(lstrChannName);

	if (-1 == this->m_oChannInfo._iChannType )
	{
		this->m_oChannInfo = this->m_plantInfo->GetSpecDynChannInfo(lstrChannName);

		if (-1 == this->m_oChannInfo._iChannType )
		{
			this->m_oChannInfo = this->m_plantInfo->GetSpecProcChannInfo(lstrChannName);

			if (-1 == this->m_oChannInfo._iChannType )
			{
				return _DEFAULT_BO_ERROR_CODE_;
			}
		}
	}

	//3.得到诊断的起始时间
	this->m_oOleTimeStart = this->m_map_in._mapDate[FIELD_SERVICE_OFF_TIMESTART];
	this->m_oOleTimeEnd = this->m_map_in._mapDate[FIELD_SERVICE_OFF_TIMEEND];	

	CBHDateTime loTimeInit = CBHDateTime(2000,1,1,1,1,1);
	int lnJudgeIndex = 0x00;

	if (this->m_oOleTimeStart < loTimeInit )
	{
		lnJudgeIndex = lnJudgeIndex | 0x01;
	}

	if (this->m_oOleTimeEnd < loTimeInit )
	{
		lnJudgeIndex = lnJudgeIndex | 0x10;
	}

	switch(lnJudgeIndex)
	{
	case 0x00:
		break;
	case 0x01:
		this->m_oOleTimeStart = this->m_oOleTimeEnd;
	case 0x10:
		this->m_oOleTimeEnd = this->m_oOleTimeStart;
	case 0x11:
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}
BOOL CBODiagnosisRC::PrepareData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	_DECLARE_PERF_MEASURE_TIME()

	int lnRev = this->GetRevOfAbnormalData(this->m_plantInfo,this->m_oChannInfo,arefDBIntefFace,this->m_oOleTimeStart,this->m_oOleTimeEnd);

	//获取机组额定转速，只取第一级
	int lnRatedRev = this->m_plantInfo->GetPlantInfo()._piRev[0];

	//设置机组工作转速范围，如果转速有异常，则说明键相传感器异常
	this->m_oDiagnosisRC.SetMachineRev(lnRev,lnRatedRev);

	E_BoolExType lnStatusTempIn  = E_BoolExType_NO;
	E_BoolExType lnStatusTempOut = E_BoolExType_NO;
	E_BoolExType lnStatusSink  = E_BoolExType_NO;
	E_BoolExType lnStatusCrossHead = E_BoolExType_NO;
	E_BoolExType lnStatusImpacts  = E_BoolExType_NO;
	E_BoolExType lnStatusCrank = E_BoolExType_NO;
	E_BoolExType lnStatusDeflect  = E_BoolExType_NO;
	E_BoolExType lnStatusPV = E_BoolExType_NO;

	this->GetSensorsAlarmStatus(arefFixComm,arefDBIntefFace,
								lnStatusTempIn,lnStatusTempOut,
								lnStatusSink,lnStatusCrossHead,
								lnStatusImpacts,lnStatusCrank,
								lnStatusDeflect,lnStatusPV);
	//停车期间进行传感器异常信号判断
	if (lnRev < 1)
	{
		this->SetCylinderSensorsStatus(arefFixComm,arefDBIntefFace,
								       lnStatusTempIn,lnStatusTempOut,
			                           lnStatusSink,lnStatusCrossHead,
			                           lnStatusImpacts,lnStatusCrank,
			                           lnStatusDeflect,lnStatusPV);

		//CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,1);
	}
	else//运行期间，首先判断键相是否异常，其次判断传感器异常
	{
		this->SetCylinderSensorsStatusInRunningStatus(arefFixComm,arefDBIntefFace,lnRev,
														lnStatusTempIn,lnStatusTempOut,
														lnStatusSink,lnStatusCrossHead,
														lnStatusImpacts,lnStatusCrank,
														lnStatusDeflect,lnStatusPV);
	}

	if (lnStatusTempIn != E_BoolExType_NO || lnStatusTempOut != E_BoolExType_NO || lnStatusSink  != E_BoolExType_NO ||
		lnStatusCrossHead != E_BoolExType_NO || lnStatusImpacts  != E_BoolExType_NO || lnStatusCrank != E_BoolExType_NO ||
		lnStatusDeflect  != E_BoolExType_NO)
	{
		//return TRUE;
	}

	_BEGIN_PERF_MEASURE_TIME();	
	/*设置测点所在缸体的各个监测点的报警状态*/
	this->SetCylinderAlarmStatus(arefFixComm,arefDBIntefFace);	
	_END_PERF_MEASURE_TIME(_T("SetCylinderAlarmStatus"));

	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数*/
/*	this->SetCylinderValueAlarmsAndSensorFaults(arefFixComm,arefDBIntefFace);*/
	CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,2);
	_END_PERF_MEASURE_TIME(_T("SetCylinderValueAlarmsAndSensorFaults"));

	//_BEGIN_PERF_MEASURE_TIME();
	/**设置测点所在缸体的各个监测点的传感器故障状态*/
	//this->SetCylinderSensorsStatus(arefFixComm,arefDBIntefFace);
	//CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,1);
	//_END_PERF_MEASURE_TIME(_T("SetCylinderSensorsStatus"));

	_BEGIN_PERF_MEASURE_TIME();
	/*设置报警温度测点前后10分钟内的数据*/
	this->SetAlarmValueTempTrend(arefFixComm,arefDBIntefFace);
	_END_PERF_MEASURE_TIME(_T("SetAlarmValueTempTrend"))


	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断测点所在缸撞击次数*/
	/*this->SetImpactTimes(arefFixComm,arefDBIntefFace);*/
	CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,3);
	_END_PERF_MEASURE_TIME(_T("SetImpactTimes"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置报警前后10s内的沉降量趋势。 点数不需要超过50个。报警前后各5s数据*/
	this->SetSinkTrend(arefFixComm,arefDBIntefFace);
	_END_PERF_MEASURE_TIME(_T("SetSinkTrend"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断测点沉降报警时刻的振动波形。没有报警可以不设置*/
	/*this->SetSinkWave(arefFixComm,arefDBIntefFace);*/
	CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,4);
	_END_PERF_MEASURE_TIME(_T("SetSinkWave"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置报警前后10s内的偏摆量。 点数不需要超过50个*/
	this->SetDeflectTrend(arefFixComm,arefDBIntefFace);
	_END_PERF_MEASURE_TIME(_T("SetDeflectTrend"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断测点偏摆报警时刻的波形*/
	/*this->SetDeflectWave(arefFixComm,arefDBIntefFace);*/
	CBODiagnosisRC::FixForCompilerError(*this,arefFixComm,arefDBIntefFace,5);
	_END_PERF_MEASURE_TIME(_T("SetDeflectWave"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
	this->SetCrossHeadWave(arefFixComm,arefDBIntefFace);
	_END_PERF_MEASURE_TIME(_T("SetCrossHeadWave"));


	_BEGIN_PERF_MEASURE_TIME();
	/**设置诊断所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
	this->SetCrankWave(arefFixComm,arefDBIntefFace);
	_END_PERF_MEASURE_TIME(_T("SetCrankWave"));

	return TRUE;
}
BOOL CBODiagnosisRC::ProcessData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	this->m_map_out_s._mapInteger = this->m_oDiagnosisRC.GetExsysVarValueList();

	return TRUE;
}

int CBODiagnosisRC::WriteBackData(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{
	std::map<string,int> loMapRet = this->m_oDiagnosisRC.GetExsysVarValueList();

	arefFixComm.CreateAnswerMul(arefFixComm.GetFunc());

	int lnDataCount = 0;

	//if (loMapRet.find("S-RC-RodXAbn0")==loMapRet.end())
	//{
	//	loMapRet["S-RC-RodXAbn0"] = 4;
	//}

	//if (loMapRet.find("S-RC-RodXAbn1")== loMapRet.end())
	//{
	//	loMapRet["S-RC-RodXAbn1"] = 2;
	//}
	for (std::map<string,int>::iterator loIt =  loMapRet.begin();loIt!= loMapRet.end();loIt++)
	{
		zdlTraceLine(_T("%s | %d \r\n"),loIt->first.c_str(),loIt->second);
		this->WriteFertureInfo(loIt->first,loIt->second,&arefFixComm);
		lnDataCount++;
	}



	arefFixComm.CreateEndMak();
	arefFixComm.Write(arefFixComm.GetAci());

	return lnDataCount;
}

/*第1步、设置测点所在缸体的各个监测点的报警状态*/
void CBODiagnosisRC::SetCylinderAlarmStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	//循环更简洁，但是不直观

	//温度的报警状态
	E_RC_AlarmStatus lnStatusTemp		= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_TEMP);
	if (-1== lnStatusTemp)
	{
		ASSERT(FALSE);
		lnStatusTemp = E_RC_AlarmStatus_UnKnown;
	}
	//沉降
	E_RC_AlarmStatus lnStatusSink		= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_SINK);
	if (-1== lnStatusSink)
	{
		ASSERT(FALSE);
		lnStatusSink = E_RC_AlarmStatus_UnKnown;
	}


	//十字头振动
	E_RC_AlarmStatus lnStatusCrossHead  = this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_CROSSHEAD);
	if (-1== lnStatusCrossHead)
	{
		ASSERT(FALSE);
		lnStatusCrossHead = E_RC_AlarmStatus_UnKnown;
	}

	//撞击次数
	E_RC_AlarmStatus lnStatusImpacts	= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_IMPACTS);
	if (-1== lnStatusImpacts)
	{
		ASSERT(FALSE);
		lnStatusImpacts = E_RC_AlarmStatus_UnKnown;
	}

	//曲轴箱报警状态
	E_RC_AlarmStatus lnStatusCrank		= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_CRANK);
	if (-1== lnStatusCrank)
	{
		ASSERT(FALSE);
		lnStatusCrank = E_RC_AlarmStatus_UnKnown;
	}
	//偏摆
	E_RC_AlarmStatus lnStatusDeflect	= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_DEFLECT	);
	if (-1== lnStatusDeflect)
	{
		ASSERT(FALSE);
		lnStatusDeflect = E_RC_AlarmStatus_UnKnown;
	}

	//动态压力报警情况
	E_RC_AlarmStatus lnStatusPV			= this->GetSpecChannTypeAlarmStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_PV);
	if (-1== lnStatusPV)
	{
		ASSERT(FALSE);
		lnStatusPV = E_RC_AlarmStatus_UnKnown;
	}

	//2.设置数据

	int lnRev = this->GetSpecSectionRev(this->m_plantInfo,this->m_oChannInfo,arefDBIntefFace,this->m_oOleTimeStart,this->m_oOleTimeEnd);

	try
	{
		this->m_oDiagnosisRC.SetCylinderAlarmStatus(	lnStatusTemp,
														lnStatusSink,
														lnStatusCrossHead,
														lnStatusImpacts,
														lnStatusCrank,
														lnRev,
														lnStatusDeflect,
														lnStatusPV
														);

	}catch (...)
	{
		ASSERT(FALSE);		
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
}

void CBODiagnosisRC::GetSensorsAlarmStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,
										   E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
										   E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
										   E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
										   E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	//吸气阀温度的报警状态
	anStatusTempIn = this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_TEMP_INVAL);
	if (-1== anStatusTempIn)
	{
		ASSERT(FALSE);
		anStatusTempIn = E_BoolExType_Unsure;
	}

	//排气阀温度的报警状态
	anStatusTempOut	= this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_TEMP);
	if (-1== anStatusTempOut)
	{
		ASSERT(FALSE);
		anStatusTempOut = E_BoolExType_Unsure;
	}
	
	//沉降
	anStatusSink = this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_SINK);
	if (-1== anStatusSink)
	{
		ASSERT(FALSE);
		anStatusSink = E_BoolExType_Unsure;
	}


	//十字头振动
	anStatusCrossHead  = this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_CROSSHEAD);
	if (-1== anStatusCrossHead)
	{
		ASSERT(FALSE);
		anStatusCrossHead = E_BoolExType_Unsure;
	}

	//撞击次数
	anStatusImpacts	= this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_IMPACTS);
	if (-1== anStatusImpacts)
	{
		ASSERT(FALSE);
		anStatusImpacts = E_BoolExType_Unsure;
	}

	//曲轴箱报警状态
	anStatusCrank		= this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_CRANK);
	if (-1== anStatusCrank)
	{
		ASSERT(FALSE);
		anStatusCrank = E_BoolExType_Unsure;
	}
	//偏摆
	anStatusDeflect	= this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_DEFLECT);
	if (-1== anStatusDeflect)
	{
		ASSERT(FALSE);
		anStatusDeflect = E_BoolExType_Unsure;
	}

	//动态压力报警情况
	anStatusPV	= this->GetSpecChannTypeSensorStatus(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		this->EXPERT_CHANN_TYPE_PV);
	if (-1== anStatusPV)
	{
		ASSERT(FALSE);
		anStatusPV = E_BoolExType_Unsure;
	}
}

/*第2步、设置测点所在缸体的各个监测点的传感器故障状态*/
void CBODiagnosisRC::SetCylinderSensorsStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,
											  E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
											  E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
											  E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
											  E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}


	//2.设置数据
	try
	{
		this->m_oDiagnosisRC.SetCylinderSensorsStatus(  anStatusTempIn,
														anStatusTempOut,
														anStatusSink,
														anStatusCrossHead,
														anStatusImpacts,
														anStatusCrank,
														anStatusDeflect,
														anStatusPV
														);
	}catch (...)
	{
		ASSERT(FALSE);		
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
};

/*运行期间，设置传感器故障状态*/
void CBODiagnosisRC::SetCylinderSensorsStatusInRunningStatus(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace,int anRev,
															 E_BoolExType & anStatusTempIn, E_BoolExType & anStatusTempOut,
															 E_BoolExType & anStatusSink,   E_BoolExType & anStatusCrossHead,
															 E_BoolExType & anStatusImpacts,E_BoolExType & anStatusCrank,
															 E_BoolExType & anStatusDeflect,E_BoolExType & anStatusPV)
{
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	//如果传感器存在故障则不进行故障诊断
	try
	{
		//温度传感器诊断
		std::vector<double> loTrends;

		double ldblThreshold = 0.0f;

		BOOL lbIsValveGasIn = TRUE;

		bool lbTempInDiagnosis = (anStatusTempIn == E_BoolExType_NO)?false:true;

		this->m_oDiagnosisRC.SetValveFalutDiagnosis(lbTempInDiagnosis,true);

		if (lbTempInDiagnosis)
		{
			if (this->GetTempValueTrend(	arefDBIntefFace,
				arefFixComm,
				loTrends,
				this->m_plantInfo,
				this->m_oChannInfo,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				lbIsValveGasIn
				)>0)
			{
				this->m_oDiagnosisRC.SetValveTempAlarm(&loTrends.front(),loTrends.size(),lbIsValveGasIn);
			}
		}

		lbIsValveGasIn = FALSE;

		bool lbTempOutDiagnosis = (anStatusTempOut == E_BoolExType_NO)?false:true;

		this->m_oDiagnosisRC.SetValveFalutDiagnosis(lbTempOutDiagnosis,false);

		if (lbTempOutDiagnosis)
		{
			if (this->GetTempValueTrend(	arefDBIntefFace,
				arefFixComm,
				loTrends,
				this->m_plantInfo,
				this->m_oChannInfo,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				lbIsValveGasIn
				)>0)
			{
				this->m_oDiagnosisRC.SetValveTempAlarm(&loTrends.front(),loTrends.size(),false);
			}
		}

		//十字头冲击诊断
		bool lbCrossHeadDiagnosis = (anStatusCrossHead == E_BoolExType_NO)?false:true;
		this->m_oDiagnosisRC.SetCrossHeadFaultDiagnosis(lbCrossHeadDiagnosis);

		std::vector<double>				loTrendDataBefore;
		int								lnStartIndBeforeAlar=0; 
		int								lnEndIndBeforeAlar=0; 

		std::vector<double>				loTrendDataAfter;
		int								lnStartIndAfterAlar=0; 
		int								lnEndIndAfterAlar=0; 

		int								lnFsAfterAlarm=0;


		double fLoAlarmValue_ =0;
		double fHiAlarmValue_ =0;

		if (lbCrossHeadDiagnosis)
		{
			int lnRet = this->GetCrossHeadWave( arefDBIntefFace,
				arefFixComm,
				this->m_plantInfo,
				this->m_oChannInfo,
				loTrendDataBefore,
				lnStartIndBeforeAlar,
				lnEndIndBeforeAlar,
				loTrendDataAfter,
				lnStartIndAfterAlar,
				lnEndIndAfterAlar,
				lnFsAfterAlarm,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				fLoAlarmValue_,
				fHiAlarmValue_);
			if (lnRet > 0)
			{
				this->m_oDiagnosisRC.SetCrossHeadPeakValueAndDC(&loTrendDataAfter.front(),loTrendDataAfter.size());
			}
		}

		//活塞杆偏摆量诊断
		bool lbDeflectDiagnosis = (anStatusDeflect == E_BoolExType_NO)?false:true;

		this->m_oDiagnosisRC.SetRodDeflectFaultDiagnosis(lbDeflectDiagnosis);

		std::vector<double> loDeflectPeakPeak;

		std::vector<double> loDeflectDC;

		if(lbDeflectDiagnosis)
		{
			int lnRet = this->GetSpecRodCharTrend(
				arefDBIntefFace,
				arefFixComm,
				loDeflectPeakPeak,
				this->m_plantInfo,
				this->m_oChannInfo,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				E_TH_ONECHAR,
				MEASURE_DIRECTION_HORIZONTAL);

			if (lnRet > 0)
			{
				lnRet = this->GetSpecRodCharTrend(
					arefDBIntefFace,
					arefFixComm,
					loDeflectDC,
					this->m_plantInfo,
					this->m_oChannInfo,
					this->m_oOleTimeStart,
					this->m_oOleTimeEnd,
					E_TH_ONEPHASE,
					MEASURE_DIRECTION_HORIZONTAL);
			}
			if(lnRet > 0)
			{
				this->m_oDiagnosisRC.SetRodDeflectInfo(&loDeflectPeakPeak.front(),&loDeflectDC.front(),loDeflectPeakPeak.size());
			}
		}

		//活塞杆沉降量诊断
		bool lbSinkDiagnosis = (anStatusSink == E_BoolExType_NO)?false:true;

		this->m_oDiagnosisRC.SetRodSinkFaultDiagnosis(lbSinkDiagnosis);

		std::vector<double> loSinkPeakPeak;

		std::vector<double> loSinkGap;

		if(lbSinkDiagnosis)
		{
			int lnRet = this->GetSpecRodCharTrend(
				arefDBIntefFace,
				arefFixComm,
				loSinkPeakPeak,
				this->m_plantInfo,
				this->m_oChannInfo,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				E_TH_ONECHAR,
				MEASURE_DIRECTION_VERTICAL);

			if (lnRet > 0)
			{
				lnRet = this->GetSpecRodCharTrend(
					arefDBIntefFace,
					arefFixComm,
					loSinkGap,
					this->m_plantInfo,
					this->m_oChannInfo,
					this->m_oOleTimeStart,
					this->m_oOleTimeEnd,
					E_TH_THREEPHASE,
					MEASURE_DIRECTION_VERTICAL);
			}
			if(lnRet > 0)
			{
				this->m_oDiagnosisRC.SetRodDeflectInfo(&loSinkPeakPeak.front(),&loSinkGap.front(),loDeflectPeakPeak.size());
			}
		}

		//曲轴箱振动
		bool lbCrankDiagnosis = (anStatusCrank == E_BoolExType_NO)?false:true;

		this->m_oDiagnosisRC.SetCrankFaultDiagnosis(lbCrankDiagnosis);

		if (lbCrankDiagnosis)
		{
			int lnRet = this->GetCrankWave(
				arefDBIntefFace,
				arefFixComm,
				this->m_plantInfo,
				this->m_oChannInfo,
				loTrendDataBefore,
				lnStartIndBeforeAlar,
				lnEndIndBeforeAlar,
				loTrendDataAfter,
				lnStartIndAfterAlar,
				lnEndIndAfterAlar,
				lnFsAfterAlarm,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				fLoAlarmValue_,
				fHiAlarmValue_);
			if (lnRet > 0)
			{
				this->m_oDiagnosisRC.SetCrankWaveDC(&loTrendDataAfter.front(),loTrendDataAfter.size());
			}

			vector<double> loTrendDataRMS;

			lnRet = this->GetCrankTrendData( arefDBIntefFace,
				arefFixComm,
				loTrendDataRMS,
				this->m_plantInfo,
				this->m_oChannInfo,
				this->m_oOleTimeStart,
				this->m_oOleTimeEnd,
				E_TH_ALLCHAR);
			if (lnRet > 0)
			{
				this->m_oDiagnosisRC.SetCrankRMSValue(&loTrendDataRMS.front(),loTrendDataRMS.size());
			}
		}
	}catch(...)
	{
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}
}

/*设置诊断测点所在缸的气阀传感器有故障的数目以及没有故障气阀的报警通道数*/
void CBODiagnosisRC::SetCylinderValueAlarmsAndSensorFaults(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	unsigned int lnSensorFaults=0; 	
	unsigned int lnAlarmChannels=0; 	
	int lnChannels=0;
	
	this->GetCylinderValueAlarmsAndSensorFaults(arefDBIntefFace,
												arefFixComm,
												this->m_plantInfo,
												this->m_oChannInfo,
												this->m_oOleTimeStart,
												this->m_oOleTimeEnd,
												lnSensorFaults,
												lnAlarmChannels,
												lnChannels
												);

	try
	{
		this->m_oDiagnosisRC.SetCylinderValueAlarmsAndSensorFaults(lnSensorFaults,lnAlarmChannels,lnChannels);
	}
	catch (...)
	{
		ASSERT(FALSE);		
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);			
	}

};

/*设置报警温度测点前40+后20分钟内的数据*/
void CBODiagnosisRC::SetAlarmValueTempTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	std::vector<double> loTrends;

	double ldblThreshold = 0.0f;

	BOOL lbIsValveGasIn = FALSE;
	
	if (this->GetSpecChannAlarmValueTempTrend(	arefDBIntefFace,
												arefFixComm,
												loTrends,
												ldblThreshold,
												this->m_plantInfo,
												this->m_oChannInfo,
												this->m_oOleTimeStart,
												this->m_oOleTimeEnd,
												lbIsValveGasIn
												)>0)
	{
		try
		{
			this->m_oDiagnosisRC.SetAlarmValueTempTrend(&loTrends.front(),loTrends.size(),ldblThreshold,lbIsValveGasIn);
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}

	}

};

/*设置诊断测点所在缸撞击次数*/
void CBODiagnosisRC::SetImpactTimes(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	return ;

	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return;
	}

	return;

	unsigned int lnImpactTimes = 0;

	unsigned int lnImpaceThrethold = 0;
	
	lnImpactTimes = this->GetSpecImpactTimes(arefDBIntefFace,
											 arefFixComm,
											 this->m_plantInfo,
											 this->m_oChannInfo,
											 lnImpactTimes,
											 lnImpaceThrethold,
											 this->m_oOleTimeStart,
											 this->m_oOleTimeEnd);

	if (lnImpactTimes <0)
	{
		ASSERT(FALSE);
		lnImpactTimes = 0;
	}

	if (lnImpaceThrethold <0)
	{
		ASSERT(FALSE);
		lnImpaceThrethold = 0;
	}

	try
	{
		/*this->m_oDiagnosisRC.SetImpactTimes(lnImpactTimes,lnImpaceThrethold);*/
	}
	catch (...)
	{
		ASSERT(FALSE);		
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
	}

};

/*设置第一个报警点报警前后10s内的沉降量趋势。 点数不需要超过50个。报警前后各5s数据*/
int CBODiagnosisRC::SetSinkTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}

	std::vector<double> loTrends;

	double ldblThreshold = 0.0f;

	double	fLoAlarmValue_ =0;
	double	fHiAlarmValue_ =0;
	double	fMaxRange_ =0;
	bool	bHiPass_ =0;

	if (this->GetSpecSinkTrend(
								arefDBIntefFace,
								arefFixComm,
								loTrends,
								ldblThreshold,
								this->m_plantInfo,
								this->m_oChannInfo,
								this->m_oOleTimeStart,
								this->m_oOleTimeEnd,
								fLoAlarmValue_,
								fHiAlarmValue_,
								fMaxRange_,
								bHiPass_)>0)
	{

		try
		{
			double * lpBuffer = &loTrends.front();

			int lnBufferSize = loTrends.size();

			int lnRet = this->m_oDiagnosisRC.SetSinkTrend(  lpBuffer,
															lnBufferSize,
															fLoAlarmValue_,
															fHiAlarmValue_,
															fMaxRange_,
															bHiPass_);

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}

	}
	return 0;
};

/*设置诊断测点沉降报警时刻的振动波形。没有报警可以不设置*/
int CBODiagnosisRC::SetSinkWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}
	std::vector<double> loWave;
	int lnSampleFreq = 0;
	double ldbThrethold =0;

	if (this->GetSpecSinkWave(
								arefDBIntefFace,
								arefFixComm,
								loWave,
								lnSampleFreq,
								this->m_plantInfo,
								this->m_oChannInfo,
								this->m_oOleTimeStart,
								this->m_oOleTimeEnd,
								ldbThrethold)>0)
	{
		try
		{
			int lnRet =this->m_oDiagnosisRC.SetSinkWave(&loWave.front(),loWave.size(),lnSampleFreq,ldbThrethold);

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}

	}
	return 0;
};

/*设置报警前后10s内的偏摆量。 点数不需要超过50个*/
int CBODiagnosisRC::SetDeflectTrend(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}
	std::vector<double> loTrends;

	double ldblThreshold = 0.0f;

	double	fLoAlarmValue_ =0;
	double	fHiAlarmValue_ =0;
	double	fMaxRange_ =0;
	bool	bHiPass_ =0;

	if (this->GetSpecDeflectTrend(
		arefDBIntefFace,
		arefFixComm,
		loTrends,
		ldblThreshold,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		fLoAlarmValue_,
		fHiAlarmValue_,
		fMaxRange_,
		bHiPass_)>-1)
	{
		try
		{
			int lnRet = this->m_oDiagnosisRC.SetDeflectTrend(&loTrends.front(),loTrends.size(),fLoAlarmValue_,fHiAlarmValue_,fMaxRange_,bHiPass_);

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}

	}
	return 0;
};

/*设置诊断测点偏摆报警时刻的波形*/
int CBODiagnosisRC::SetDeflectWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}
	std::vector<double> loWave;

	int lnSampleFreq = 0;

	double ldblThreshold = 0.0f;

	if (this->GetSpecDeflectWave(
		arefDBIntefFace,
		arefFixComm,
		loWave,
		lnSampleFreq,
		this->m_plantInfo,
		this->m_oChannInfo,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		ldblThreshold)>0)
	{
		try
		{
			int lnRet =this->m_oDiagnosisRC.SetDeflectWave(&loWave.front(),loWave.size(),lnSampleFreq,ldblThreshold);

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}

	}
	return 0;
};

/*设置诊断所在缸体十字头振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
int CBODiagnosisRC::SetCrossHeadWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}

	std::vector<double>				loTrendDataBefore;
	int								lnStartIndBeforeAlar=0; 
	int								lnEndIndBeforeAlar=0; 

	std::vector<double>				loTrendDataAfter;
	int								lnStartIndAfterAlar=0; 
	int								lnEndIndAfterAlar=0; 

	int								lnFsAfterAlarm=0;


	double fLoAlarmValue_ =0;
	double fHiAlarmValue_ =0;


	int lnRet = this->GetCrossHeadWave(
										arefDBIntefFace,
										arefFixComm,
										this->m_plantInfo,
										this->m_oChannInfo,
										loTrendDataBefore,
										lnStartIndBeforeAlar,
										lnEndIndBeforeAlar,
										loTrendDataAfter,
										lnStartIndAfterAlar,
										lnEndIndAfterAlar,
										lnFsAfterAlarm,
										this->m_oOleTimeStart,
										this->m_oOleTimeEnd,
										fLoAlarmValue_,
										fHiAlarmValue_);
	if (lnRet>=0)
	{
		try
		{
			double *pWaveBeforeAlarm_ =  &(loTrendDataBefore.front());

			lnRet =this->m_oDiagnosisRC.SetCrossHeadWave(
														  pWaveBeforeAlarm_,
														  loTrendDataBefore.size(),
														  lnStartIndBeforeAlar,
														  lnEndIndBeforeAlar,
														  &(loTrendDataAfter.front()),
														  loTrendDataAfter.size(),
														  lnStartIndAfterAlar,
														  lnEndIndAfterAlar,
														  lnFsAfterAlarm,
														  fLoAlarmValue_,
														  fHiAlarmValue_
														  );

#ifdef _DEBUG
			writefile(&(loTrendDataBefore.front()),loTrendDataBefore.size(),1010);
			writefile(&(loTrendDataAfter.front()),loTrendDataAfter.size(),1111);
#endif
			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}
	}else
	{
		ASSERT(FALSE);
	}
	return 0;
};

/*设置诊断所在曲轴箱振动报警（其他测点报警，就算当时报警的时刻）前后的波形。如果没有报警就设置为之前一段时间(前1周)的波形*/
int CBODiagnosisRC::SetCrankWave(ICommInterFace & arefFixComm,IDBInterFace & arefDBIntefFace)
{ 
	if (!this->m_plantInfo)
	{
		ASSERT(FALSE);

		return 0;
	}
	std::vector<double>				loTrendDataBefore;
	int								lnStartIndBeforeAlar=0; 
	int								lnEndIndBeforeAlar=0; 

	std::vector<double>				loTrendDataAfter;
	int								lnStartIndAfterAlar=0; 
	int								lnEndIndAfterAlar=0; 

	int								lnFsAfterAlarm=0;

	double fLoAlarmValue_ =0;
	double fHiAlarmValue_ =0;

	int lnRet = this->GetCrankWave(
		arefDBIntefFace,
		arefFixComm,
		this->m_plantInfo,
		this->m_oChannInfo,
		loTrendDataBefore,
		lnStartIndBeforeAlar,
		lnEndIndBeforeAlar,
		loTrendDataAfter,
		lnStartIndAfterAlar,
		lnEndIndAfterAlar,
		lnFsAfterAlarm,
		this->m_oOleTimeStart,
		this->m_oOleTimeEnd,
		fLoAlarmValue_,
		fHiAlarmValue_);

	if (lnRet>0)
	{
		try
		{
			lnRet =this->m_oDiagnosisRC.SetCrankCaseWave(
				&(loTrendDataBefore.front()),
				loTrendDataBefore.size(),
				lnStartIndBeforeAlar,
				lnEndIndBeforeAlar,
				&(loTrendDataAfter.front()),
				loTrendDataAfter.size(),
				lnStartIndAfterAlar,
				lnEndIndAfterAlar,
				lnFsAfterAlarm,				
				fHiAlarmValue_,
				fLoAlarmValue_
				);

			return lnRet;
		}
		catch (...)
		{
			ASSERT(FALSE);		
			CString lstrErrorLog;
			lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
			CHZLogManage::Fatal(lstrErrorLog);
			zdlTraceLine(lstrErrorLog);			
		}
	}else
	{
		/*ASSERT(FALSE);*/
	}
	return 0;
};


