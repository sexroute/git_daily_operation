#include "stdafx.h"
#include ".\realdata.h"
#include "OnlineGlobal.h"
#include "ZoomAnalysis.h"

CRealData::CRealData(void)
{
//	_fFreqDataDf=0;
	_iRealPointNum=0;
	_iSmpFreq=0;
	_iSmpNum=0;
	_iRealRev1=0;
	_iRealRev2=0;
	_bSendCenter=true;
	_pfSendCenterWaveBuf=new float[25000];
	memset(_pfSendCenterWaveBuf,0,25000);
	_vectorAxisChann.clear();
	_vectorProcChann.clear();
	_vectorVibChann.clear();
}

CRealData::~CRealData(void)
{
	if(_pfSendCenterWaveBuf!=NULL)
          delete []_pfSendCenterWaveBuf;
}

/**@brief 设置装置号和设备号*/
void CRealData::SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iRatingRev1,int ag_iRatingRev2,int ag_iGraphType)
{
	_strSet=ag_strSet;
	_strPlant=ag_strPlant;
	_iRatingRev1=ag_iRatingRev1;
	_iRatingRev2=ag_iRatingRev2;
	_iGraphType=ag_iGraphType;
}

/**@brief 添加测点信息*/
void CRealData::AddVibChannelInfo(string ag_strChann,string ag_strUnit,int ag_index,int ag_iListofFrame,int ag_iSensorType,int ag_iofRev)
{
   S_ChannInfo schaninfo;
   schaninfo.iChannType=GE_VIBCHANN;
   schaninfo.iListofFrame=ag_iListofFrame;
   schaninfo.index=ag_index;
   schaninfo.iOfRev=ag_iofRev;
   schaninfo.strChannName=ag_strChann;
   schaninfo.strUnit=ag_strUnit;
   schaninfo.iSensorType=ag_iSensorType;
   _vectorVibChann.push_back(schaninfo);

}

/**@brief 添加过程量测点信息*/
void CRealData::AddProcChannInfo(string ag_strChann,string ag_strUnit,int ag_index,int ag_iListofFrame, int ag_iChannType,int ag_iSensorType,int ag_iofRev)
{
	S_ChannInfo schaninfo;
	schaninfo.iChannType=ag_iChannType;
	schaninfo.iListofFrame=ag_iListofFrame;
	schaninfo.index=ag_index;
	schaninfo.iOfRev=ag_iofRev;
	schaninfo.strChannName=ag_strChann;
	schaninfo.strUnit=ag_strUnit;
	schaninfo.iSensorType=ag_iSensorType;
	_vectorProcChann.push_back(schaninfo);
}

/**@brief 添加轴位移测点信息*/
void CRealData::AddAxisChannInfo(string ag_strChann,string ag_strUnit,int ag_index,int ag_iListofFrame,int ag_iofRev){
	S_ChannInfo schaninfo;
	schaninfo.iChannType=GE_AXISCHANN;
	schaninfo.iListofFrame=ag_iListofFrame;
	schaninfo.index=ag_index;
	schaninfo.iOfRev=ag_iofRev;
	schaninfo.strChannName=ag_strChann;
	schaninfo.strUnit=ag_strUnit;
	schaninfo.iSensorType=GE_SENSORTYPE_VOTEX;
	_vectorAxisChann.push_back(schaninfo);
}

/// 添加所有测点信息结束，在这个函数中计算有个这个机组相关信息
void CRealData::AddChannInfoEnd(void){
	_iVibChannNum=_vectorVibChann.size();
	_iProcChannNum=_vectorProcChann.size();
	_iAxisChannNum=_vectorAxisChann.size();
	_cvectorEndVib=_vectorVibChann.end();
	_cvectorEndProc=_vectorProcChann.end();
	_cvectorEndAxis=_vectorAxisChann.end();
}

/**@brief 初始化测点参数和每个测点对应的报警值,和链表.注意：此函数一定要运行在添加完所有测点信息完毕。
*即在此函数运行后，就不允许再添加测点了信息了，即不允许运行AddVibChannelInfo和AddProcChannInfo这个两个函数了
*/
void CRealData::InitParam(float *ag_pfCharAll,
						  float *ag_pfCharOne,
						  float *ag_pfCharTwo,
						  float *ag_pfCharThree,
						  float *ag_pfCharHalf,
						  float *ag_pfCharRes,
						  float *ag_pfPhaseOne,
						  float *ag_pfPhaseTwo,
						  float *ag_pfPhaseThree,
						  float *ag_pfPhaseHalf,
						  float *ag_pfDC,
						  float **ag_ppfWave,
						  float *ag_pfProc,
						  float *ag_pfAxisDisplace,
						  int   *ag_pbVibChannelState,
						  int   *ag_pbProcChannelState,
						  int   *ag_pbAxisDisplaceChannelState)
{
  _pfVibCharOne=ag_pfCharOne;
  _pfVibCharTwo=ag_pfCharTwo;
  _pfVibCharThree=ag_pfCharThree;
  _pfVibCharHalf=ag_pfCharHalf;
  _pfVibCharAll=ag_pfCharAll;
  _pfVibCharRes=ag_pfCharRes;
  _pfDC=ag_pfDC;
  _pfPhaseOne=ag_pfPhaseOne;
  _pfPhaseHalf=ag_pfPhaseHalf;
  _pfPhaseTwo=ag_pfPhaseTwo;
  _pfPhaseThree=ag_pfPhaseThree;
  
  _ppfWave=ag_ppfWave;

  _pfProcValue=ag_pfProc;
  _pfAxisDisplaceValue=ag_pfAxisDisplace;
  _pbVibChannelState=ag_pbVibChannelState;
  _pbProcChannelState=ag_pbProcChannelState;
  _pbAxisChannelState=ag_pbAxisDisplaceChannelState;

}

/// 得到设备号
string CRealData::GetPlantID(void){
	return _strPlant;
}


/// 设置设备转速
void CRealData::UpdateRealPlantRev(float ag_iRev1,float ag_iRev2)
{
	_lockRev.BeginWrite();
	_iRealRev1=ag_iRev1;
	_iRealRev2=ag_iRev2;
	_lockRev.EndWrite();
}

///设置振动测点数据,值得注意的是此处的波形数据是个一维数组，因为这个函数主要是针对数据中心发送到中间件的数据，而得到的数据就是一维
int CRealData::UpdateVibData(float * ag_pfCharOne,
							 float * ag_pfCharTwo,
							 float * ag_pfCharThree,
							 float * ag_pfCharAll,
							 float * ag_pfCharHalf,
							 float * ag_pfCharRes,
							 float * ag_pfPhaseOne,
							 float * ag_pfPhaseTwo,
							 float * ag_pfPhaseThree,
							 float * ag_pfPhaseHalf,
							 float * ag_pfDC,
							 float * ag_fwave,
							 int ag_iSmpNum,
							 int ag_iSmpFreq,
							 int ag_iRealPointNum,
							 int ag_len,
							 int ag_startIndex)
{
	int reallen=(ag_startIndex+ag_len) > _iVibChannNum ?(_iVibChannNum-ag_startIndex):ag_len;

	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
	_lockSendCenter.BeginWrite();
	_bSendCenter=false;
	_lockSendCenter.EndWrite();

	_lockDataVib.BeginWrite();
	_iSmpFreq=ag_iSmpFreq>0?ag_iSmpFreq:1;
	_iSmpNum=ag_iSmpNum>0?ag_iSmpNum:1;
	_iRealPointNum=ag_iRealPointNum>_iSmpNum?_iSmpNum:ag_iRealPointNum;
//    _fFreqDataDf=float(_iSmpFreq)/_iRealPointNum;
//	CString strtmp;	strtmp.Format("%d,%d,%d,%f",reallen,_iSmpFreq,realPointNum,_fFreqDataDf);
//AfxMessageBox(strtmp);
    memcpy(&(_pfVibCharAll[ag_startIndex]),ag_pfCharAll,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfVibCharOne[ag_startIndex]),ag_pfCharOne,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfVibCharTwo[ag_startIndex]),ag_pfCharTwo,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfVibCharThree[ag_startIndex]),ag_pfCharThree,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfVibCharHalf[ag_startIndex]),ag_pfCharHalf,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfVibCharRes[ag_startIndex]),ag_pfCharRes,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfDC[ag_startIndex]),ag_pfDC,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfPhaseHalf[ag_startIndex]),ag_pfPhaseHalf,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfPhaseOne[ag_startIndex]),ag_pfPhaseOne,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfPhaseTwo[ag_startIndex]),ag_pfPhaseTwo,reallen*GD_FLOAT_SIZE);
	memcpy(&(_pfPhaseThree[ag_startIndex]),ag_pfPhaseThree,reallen*GD_FLOAT_SIZE);
    //波形数据
	for (int loop=0;loop<reallen;++loop){
		memcpy(_ppfWave[loop+ag_startIndex],&(ag_fwave[loop*ag_iRealPointNum]),_iRealPointNum*GD_FLOAT_SIZE);
	//	writefile(_ppfWave[loop+ag_startIndex],realPointNum,10+loop);
    }
	_lockDataVib.EndWrite();
	
	return reallen;  
}

///设置测点运行状态数据
int CRealData::UpdateVibRunState(int * ag_piState,int ag_len,int ag_startIndex){
	int reallen=(ag_startIndex+ag_len) > _iVibChannNum ?(_iVibChannNum-ag_startIndex):ag_len;
	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
    _lockDataRunState.BeginWrite();
	memcpy(&(_pbVibChannelState[ag_startIndex]),ag_piState,4*reallen);
	_lockDataRunState.EndWrite();
	return reallen;  
}

///设置过程量测点运行状态数据
int CRealData::UpdateProcRunState(int * ag_piState,int ag_len,int ag_startIndex){
	int reallen=(ag_startIndex+ag_len) > _iProcChannNum ?(_iProcChannNum-ag_startIndex):ag_len;
	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
	_lockDataRunState.BeginWrite();
	memcpy(&(_pbProcChannelState[ag_startIndex]),ag_piState,4*reallen);
	_lockDataRunState.EndWrite();
	return reallen;  
}

///设置轴位移测点运行状态数据
int CRealData::UpdateAxisRunState(int * ag_piState,int ag_len,int ag_startIndex){
	int reallen=(ag_startIndex+ag_len) > _iAxisChannNum ?(_iAxisChannNum-ag_startIndex):ag_len;
	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
	_lockDataRunState.BeginWrite();
	memcpy(&(_pbAxisChannelState[ag_startIndex]),ag_piState,4*reallen);
	_lockDataRunState.EndWrite();
	return reallen;  
}

///设置过程量数据
int CRealData::UpdateProcData(float *ag_pfData,int ag_len,int ag_startIndex){
	int reallen=(ag_startIndex+ag_len) > _iProcChannNum ?(_iProcChannNum-ag_startIndex):ag_len;
	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
    _lockDataProc.BeginWrite();
	memcpy(&(_pfProcValue[ag_startIndex]),ag_pfData,GD_FLOAT_SIZE*reallen);
	_lockDataProc.EndWrite();
	return reallen;  
}

///设置轴位移数据
int CRealData::UpdateAxisDisplaceData(float *ag_pfData,int ag_len,int ag_startIndex){
	int reallen=(ag_startIndex+ag_len) > _iAxisChannNum ?(_iAxisChannNum-ag_startIndex):ag_len;
	if ((reallen<1)||(ag_startIndex<0)){
		return -1;
	}
	_lockDataAxis.BeginWrite();
	memcpy(&(_pfAxisDisplaceValue[ag_startIndex]),ag_pfData,GD_FLOAT_SIZE*reallen);
	_lockDataAxis.EndWrite();
	return reallen;  
}

///得到这个设备过程量数据
int CRealData::GetProcData(float *ag_pfData,int ilen,int istart){
	int reallen=(istart+ilen) > _iProcChannNum ?(_iProcChannNum-istart):ilen;
	if ((reallen<1)||(istart<0)){
		return -1;
	}
	_lockDataProc.BeginRead();
	memcpy(ag_pfData,&(_pfProcValue[istart]),GD_FLOAT_SIZE*reallen);
	_lockDataProc.EndRead();
	return reallen;  
}

///得到这个设备轴位移数据
int CRealData::GetAxisData(float *ag_pfData,int ilen,int istart){
	int reallen=(istart+ilen) > _iAxisChannNum ?(_iAxisChannNum-istart):ilen;
	if ((reallen<1)||(istart<0)){
		return -1;
	}
	_lockDataAxis.BeginRead();
	memcpy(ag_pfData,&(_pfAxisDisplaceValue[istart]),reallen*GD_FLOAT_SIZE);
	_lockDataAxis.EndRead();
	return reallen;
}

/*!@brief 得到此设备所有振动测点个数
*/
int CRealData::GetVibChannelNum(void){
	return _iVibChannNum;
}

/*!@brief 得到此设备所有过程量测点个数
*/
int CRealData::GetProcChannelNum(void){
	return _iProcChannNum;
}

///得到这个设备轴位移通道的个数
int CRealData::GetAxisDiplaceChannelNum(void){
	return _iAxisChannNum;
}

///得到指定类型的所有测点信息
int CRealData::GetSpecTypeChannInfo(S_ChannInfo * ag_schannInfo,int ag_iType){
	LIST_S_CHANNINFO::iterator result;
	int index=0;
	switch(ag_iType){
	case GE_ALLPROC:
		for(result=_vectorProcChann.begin();result!=_cvectorEndProc;++result){
			ag_schannInfo[index].strChannName=result->strChannName;
            ag_schannInfo[index].index=result->index;
			ag_schannInfo[index].iChannType=result->iChannType;
			ag_schannInfo[index].strUnit=result->strUnit;
			ag_schannInfo[index].iListofFrame=result->iListofFrame;
			ag_schannInfo[index].iOfRev=result->iOfRev;
			index++;
		}
		break;
	case GE_AXISCHANN:
		for(result=_vectorAxisChann.begin();result!=_cvectorEndAxis;++result){
			ag_schannInfo[index].strChannName=result->strChannName;
			ag_schannInfo[index].index=result->index;
			ag_schannInfo[index].iChannType=result->iChannType;
			ag_schannInfo[index].strUnit=result->strUnit;
			ag_schannInfo[index].iListofFrame=result->iListofFrame;
			ag_schannInfo[index].iOfRev=result->iOfRev;
			index++;
		}
		break;
	case GE_VIBCHANN:
		for(result=_vectorVibChann.begin();result!=_cvectorEndVib;++result){
			ag_schannInfo[index].strChannName=result->strChannName;
			ag_schannInfo[index].index=result->index;
			ag_schannInfo[index].iChannType=result->iChannType;
			ag_schannInfo[index].strUnit=result->strUnit;
			ag_schannInfo[index].iListofFrame=result->iListofFrame;
			ag_schannInfo[index].iOfRev=result->iOfRev;
			index++;
		}
		break;
	}
	return index;
}

///得到指定过程量测点的过程量数据
int CRealData::GetSpecChannProcData(string ag_strChann,float &ag_data){
    int index=GetChannIndex(ag_strChann,GE_ALLPROC);
	if (index==-1)
	{   
		ag_data=0;
		return -1;
	}
	_lockDataProc.BeginRead();
	ag_data=_pfProcValue[index];
	_lockDataProc.EndRead();
	return index;
}

///得到指定过程量测点索引的过程量数据
int CRealData::GetSpecChannProcData(int ag_index,float &ag_data){
	if ((ag_index>_iProcChannNum)||(ag_index<0))
	{
		ag_data=0;
		return -1;
	}
	_lockDataProc.BeginRead();
	ag_data=_pfProcValue[ag_index];
	_lockDataProc.EndRead();
	return ag_index;
}

///得到指定轴位移测点的轴位移数据
int CRealData::GetSpecChannAxisData(string ag_strChann,float &ag_data){
	int index=GetChannIndex(ag_strChann,GE_AXISCHANN);
	if (index==-1)
	{   
		ag_data=0;
		return -1;
	}
	_lockDataAxis.BeginRead();
	ag_data=_pfAxisDisplaceValue[index];
	_lockDataAxis.EndRead();
	return index;
}

///得到指定轴位移测点索引的轴位移数据
int CRealData::GetSpecChannAxisData(int ag_index,float &ag_data){
	if ((ag_index>_iAxisChannNum)||(ag_index<0))
	{
		ag_data=0;
		return -1;
	}
	_lockDataAxis.BeginRead();
	ag_data=_pfAxisDisplaceValue[ag_index];
	_lockDataAxis.EndRead();
	return ag_index;
}

///得到指定振动测点和特征值类型的特征数据
int CRealData::GetSpecChannAndCharTypeVibData(string ag_strChann,float &ag_data,int ag_iCharType){
	int index=GetChannIndex(ag_strChann,GE_VIBCHANN);
    return GetSpecChannAndCharTypeVibData(index,ag_data,ag_iCharType);
}

///得到指定振动测点索引和特征值类型的特征数据
int CRealData::GetSpecChannAndCharTypeVibData(int ag_index,float &ag_data,int ag_iCharType){
	int index=ag_index;
	if ((index<0)||(index>_iVibChannNum))
	{   
		ag_data=0;
		return -1;
	}
	_lockDataVib.BeginRead();
	switch(ag_iCharType) {
	case GE_ALLCHAR:
		ag_data=_pfVibCharAll[index];
		break;
	case GE_ONECHAR:
		ag_data=_pfVibCharOne[index];
		break;
	case GE_TWOCHAR:
		ag_data=_pfVibCharTwo[index];
		break;
	case GE_THREECHAR:
		ag_data=_pfVibCharThree[index];
		break;
	case GE_HALFCHAR:
		ag_data=_pfVibCharHalf[index];
		break;
	case GE_RESCHAR:
		ag_data=_pfVibCharRes[index];
		break;
	case GE_DCCHAR:
		ag_data=_pfDC[index];
		break;
	case GE_PHASECHARONE:
		ag_data=_pfPhaseOne[index];
		break;
	case GE_PHASECHARTWO:
		ag_data=_pfPhaseTwo[index];
		break;
	case GE_PHASECHARTHREE:
		ag_data=_pfPhaseThree[index];
		break;
	case GE_PHASECHARHALF:
		ag_data=_pfPhaseHalf[index];
		break;
	default:
		ag_data=0;
	}
	_lockDataVib.EndRead();
	return index;
}

///得到指定振动测点的波形和频谱数据
int CRealData::GetSpecChannVibWaveAndFreq(string ag_strChann,float *ag_pfData,int &ag_iPtNum,float *ag_pfFreqData,int &ag_iLines,float &ag_fDf){
	int iSensorType=0;
	int index=GetChannIndex(ag_strChann,iSensorType,GE_VIBCHANN);
	if (index==-1){   
		return -1;
	}
	float *pfWave;
	//得到客户指定分析线数需要波形数据个数
    int iLinesPt=ag_iLines*2.56;
    int iSmpFreq;
	_lockDataVib.BeginRead();
	//判断客户指定波形数据是否能满足,如果不满足,则就按照实际数据发送过去
	ag_iPtNum=ag_iPtNum>_iRealPointNum?_iRealPointNum:ag_iPtNum;
    //判断实际数据是否满足谱分析的波形数据个数
	iLinesPt=iLinesPt>_iRealPointNum?_iRealPointNum:iLinesPt;
	iSmpFreq=_iSmpFreq;
    pfWave=new float[iLinesPt];
	//得到谱分析需要的波形数据
	memcpy(pfWave,_ppfWave[index],(int)iLinesPt*4);
	//得到客户需要显示的波形数据
	memcpy(ag_pfData,_ppfWave[index],ag_iPtNum*sizeof(float));
	_lockDataVib.EndRead();
//	CString sTmp;sTmp.Format("%d,%d,%d",_iRealPointNum,ag_iPtNum,iLinesPt);AfxMessageBox(sTmp);
    ag_iLines=iLinesPt/2.56;
	if(iLinesPt>1)
	{
		ag_fDf=(float)iSmpFreq/iLinesPt;
		if(iSensorType==GE_SENSORTYPE_VOTEX){//峰峰值谱
			SpectralMeasureMent(pfWave,iLinesPt,ag_pfFreqData,EA_PEAK_PEAK_SPEC);	
		}
		else{//峰值谱
			SpectralMeasureMent(pfWave,iLinesPt,ag_pfFreqData,EA_PEAK_SPEC);
		}
	}
	delete [] pfWave;
	return index;
}

///得到指定振动测点的波形
int CRealData::GetSpecChannVibWave(string ag_strChann,float *ag_pfData,int ag_len){
	int index=GetChannIndex(ag_strChann,GE_VIBCHANN);
	if (index==-1){   
		return -1;
	}
	_lockDataVib.BeginRead();
	memcpy(ag_pfData,_ppfWave[index],ag_len*GD_FLOAT_SIZE);
  	_lockDataVib.EndRead();
	return index;
}

int CRealData::GetChannIndex(string ag_strChann,int ag_iType){
	int iSensorType=0;
	return GetChannIndex(ag_strChann,iSensorType,ag_iType);
}

int CRealData::GetChannIndex(string ag_strChann,int &ag_iSensorType,int ag_iType){
	LIST_S_CHANNINFO::iterator result;
	int index=-1;
	switch(ag_iType){
	case GE_ALLPROC:
		for(result=_vectorProcChann.begin();result!=_cvectorEndProc;++result){
			index++;
			if ((result->strChannName)==ag_strChann){
				ag_iSensorType=result->iSensorType;
				return index;
			}
		}
		return -1;
		break;
	case GE_AXISCHANN:
		for(result=_vectorAxisChann.begin();result!=_cvectorEndAxis;++result){
			index++;
			if ((result->strChannName)==ag_strChann){
				ag_iSensorType=result->iSensorType;
				return index;
			}
		}
		return -1;
		break;
	case GE_VIBCHANN:
		for(result=_vectorVibChann.begin();result!=_cvectorEndVib;++result){
			index++;
			if ((result->strChannName)==ag_strChann){
				ag_iSensorType=result->iSensorType;
				return index;
			}
		}
		return -1;
		break;
	}
	return -1;
}

///得到指定测点索引和测点类型的报警状态以及报警值，如果没有报警就返回false
bool CRealData::GetSpecChannAndTypeAlarmState(int ag_index,float &ag_data,int &ag_iState, int ag_iType){
	switch(ag_iType) {
	case GE_ALLPROC:
		if((ag_index<0)||(ag_index>_iProcChannNum))
			return false;
		_lockDataRunState.BeginRead();
		ag_iState=_pbProcChannelState[ag_index];
		_lockDataRunState.EndRead();
		if(ag_iState==0)
			return false;
		else
		{
			_lockDataProc.BeginRead();
			ag_data=_pfProcValue[ag_index];
			_lockDataProc.EndRead();
			return true;
		}
		break;
	case GE_VIBCHANN:
		if((ag_index<0)||(ag_index>_iVibChannNum))
			return false;
		_lockDataRunState.BeginRead();
		ag_iState=_pbVibChannelState[ag_index];
		_lockDataRunState.EndRead();
		if(ag_iState==0)
			return false;
		else
		{
			_lockDataVib.BeginRead();
			ag_data=_pfVibCharAll[ag_index];
			_lockDataVib.EndRead();
			return true;
		}
		break;
	case GE_AXISCHANN:
	default:
		if((ag_index<0)||(ag_index>_iAxisChannNum))
			return false;
		_lockDataRunState.BeginRead();
		ag_iState=_pbAxisChannelState[ag_index];
		_lockDataRunState.EndRead();
		if(ag_iState==0)
			return false;
		else
		{
			_lockDataAxis.BeginRead();
			ag_data=_pfAxisDisplaceValue[ag_index];
			_lockDataAxis.EndRead();
			return true;
		}
	}
}

///得到指定测点和测点类型的报警状态以及报警值，如果没有报警就返回false
bool CRealData::GetSpecChannAndTypeAlarmState(string ag_strChann,float &ag_data,int &ag_iState, int ag_iType){
	
	int iState;
	int index=GetChannIndex(ag_strChann,ag_iType);
	if(index<0)
		return false;
	switch(ag_iType) {
	case GE_ALLPROC:
		_lockDataRunState.BeginRead();
		iState=_pbProcChannelState[index];
		_lockDataRunState.EndRead();
		if(iState==0)
			return false;
		else
		{
			ag_iState=iState;
			_lockDataProc.BeginRead();
			ag_data=_pfProcValue[index];
			_lockDataProc.EndRead();
			return true;
		}
		break;
	case GE_VIBCHANN:
		_lockDataRunState.BeginRead();
		iState=_pbVibChannelState[index];
		_lockDataRunState.EndRead();
		if(iState==0)
			return false;
		else
		{
			ag_iState=iState;
			_lockDataVib.BeginRead();
			ag_data=_pfVibCharAll[index];
			_lockDataVib.EndRead();
			return true;
		}
		break;
	case GE_AXISCHANN:
	default:
		_lockDataRunState.BeginRead();
		iState=_pbAxisChannelState[index];
		_lockDataRunState.EndRead();
		if(iState==0)
			return false;
		else
		{
			ag_iState=iState;
			_lockDataAxis.BeginRead();
			ag_data=_pfAxisDisplaceValue[index];
			_lockDataAxis.EndRead();
			return true;
		}
	}
}

void CRealData::GetRealPlantRev(float &ag_iRev1,float & ag_iRev2){
	_lockRev.BeginRead();
	ag_iRev1=_iRealRev1;
	ag_iRev2=_iRealRev2;
	_lockRev.EndRead();
}

bool CRealData::GetPlantRunState(){
	int index;
	_lockDataRunState.BeginRead();
	
	for (index=0;index<_iVibChannNum;++index){
		if (_pbVibChannelState[index]!=0){
			_lockDataRunState.EndRead();
			return true;
		}
	}

	for (index=0;index<_iProcChannNum;++index){
		if (_pbProcChannelState[index]!=0){
			_lockDataRunState.EndRead();
			return true;
		}
	}

	for (index=0;index<_iAxisChannNum;++index){
		if (_pbAxisChannelState[index]!=0){
			_lockDataRunState.EndRead();
			return true;
		}
	}

	_lockDataRunState.EndRead();
	return false;
}

/**得到指定振动测点细化谱
   *param：
      *       ag_strChann：string,测点号
	  *       ag_pfZoomSpec:float*,细化谱数据
	  *       ag_iLines:int&,分析线数，分析完毕后会修改实际分析出来的线数，即细化谱的数据长度
	  *       ag_fStartFreq:float&, 细化分析的起始频率，分析完毕后表示细化后的起始频率
	  *       ag_iStopFreq:int,细化分析的终止频率
	  *       ag_fDf：float&,细化谱的分辨率
	  *return:0表示分析正确,负数表示错误码
	  */
int CRealData::GetSpecChannZoomSpec(string ag_strChann,float *ag_pfZoomSpec,long &ag_iLines,float &ag_fStartFreq,int ag_iStopFreq,float &ag_fDf,float *ag_pfWave,int &ag_iPtNum)
{
	int nErrorCode=0;
	int iSmpFreq;
	int iSmpNum;
    float fRealDf;
	_lockDataVib.BeginRead();
	iSmpFreq=_iSmpFreq;
	iSmpNum=_iSmpNum;
	_lockDataVib.EndRead();

	fRealDf=(float)iSmpFreq/iSmpNum;
    float *pWave=new float[iSmpNum];
	if(!((ag_fStartFreq>0)&&(ag_iStopFreq>ag_fStartFreq)&&((iSmpFreq/2)>ag_iStopFreq)&&(ag_iLines>0)))
	{//起始频率必须小于终止频率，采样频率的一半不能大于终止频率
		nErrorCode=-3;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
    }
	ag_fDf=(ag_iStopFreq-ag_fStartFreq)/ag_iLines;
	//要求最小要细化2倍
    if(!((2*ag_fDf)<fRealDf))
	{
        nErrorCode=-1;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}
    //细化分析不能超过20倍
	if((20*ag_fDf)<fRealDf)
	{
        nErrorCode=-2;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}

	int index=GetChannIndex(ag_strChann,GE_VIBCHANN);
	if (index==-1){   
		nErrorCode=-100;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}

	
	ag_iPtNum=ag_iPtNum>iSmpNum?iSmpNum:ag_iPtNum;

	_lockDataVib.BeginRead();
	memcpy(pWave,_ppfWave[index],iSmpNum*GD_FLOAT_SIZE);
	memcpy(ag_pfWave,_ppfWave[index],ag_iPtNum*4);
	_lockDataVib.EndRead();
    
	long iRealLines=ag_iLines;
	float iRealStartFreq=0;
	
	//ZoomFFT(float data[], long data_len, short startFreq, short stopFreq, short nLines, short SamplingRate, float zoomSpectrum[], long *len, float *f0, float *df);
	nErrorCode=ZoomFFT(pWave,iSmpNum,ag_fStartFreq,ag_iStopFreq,ag_iLines,iSmpFreq,ag_pfZoomSpec,&iRealLines,&iRealStartFreq,&ag_fDf);
	for (int nloop=0;nloop<iRealLines;++nloop)
	{
		ag_pfZoomSpec[nloop]=2*ag_pfZoomSpec[nloop];
	}
	ag_fStartFreq=iRealStartFreq;
	ag_iLines=iRealLines;

CLEAN_GETSPECCHANNZOOMSPEC:
	delete [] pWave;
	return nErrorCode;
}

float CRealData::GetSpecChannRev(string strChann_,float &fRealRev_)
{
	int iRevType=0;
	LIST_S_CHANNINFO::iterator result;
	for(result=_vectorVibChann.begin();result!=_cvectorEndVib;++result)
	{
		if ((result->strChannName)==strChann_){
			iRevType=result->iOfRev;
			break;
		}
	}
	_lockRev.BeginRead();
	if (iRevType==2)
	  fRealRev_=_iRealRev2;
	else
	  fRealRev_=_iRealRev2;
	_lockRev.EndRead();
    
	if (iRevType==2)
	  return _iRatingRev2;
	else 
	  return _iRatingRev1;
}

void CRealData::GetSmpInfo(int &iSmpFreq_,int &iSmpNum_)
{
	_lockDataVib.BeginRead();
	iSmpFreq_=_iSmpFreq;
	iSmpNum_=_iSmpNum;
	_lockDataVib.EndRead();
}

int CRealData::GetAllVibData(CFixComm *pFixComm_,int iStartIndex_,int iLen_)
{
   int iRealLen=iStartIndex_+iLen_;
   if (iRealLen>_iVibChannNum) 
	   return -3;
   _lockSendCenter.BeginRead();
   bool bFlag=_bSendCenter;
   _lockSendCenter.EndRead();
   if (bFlag)
	   return -2;
   int iPerWaveBytes=0;
   pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_STARTID,iStartIndex_);
   _lockDataVib.BeginRead();
   pFixComm_->SetItem(FIELD_SERVICE_SAMPLE_FREQ,_iSmpFreq);
   pFixComm_->SetItem(FIELD_SERVICE_SAMPLE_NUM,_iSmpNum);
   pFixComm_->SetItem(FIELD_SERVICE_REV,_iRealRev1);
   pFixComm_->SetItem(FIELD_SERVICE_REV2,_iRealRev2);
   pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM,_iRealPointNum);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLCHARVALUE,(char *)(&(_pfVibCharAll[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ONECHARVALUE,(char *)(&(_pfVibCharOne[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_TWOCHARVALUE,(char *)(&(_pfVibCharTwo[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_THREECHARVALUE,(char *)(&(_pfVibCharThree[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_HALFCHARVALUE,(char *)(&(_pfVibCharHalf[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ONEVIBPHASE,(char *)(&(_pfPhaseOne[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_TWOVIBPHASE,(char *)(&(_pfPhaseTwo[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_THREEVIBPHASE,(char *)(&(_pfPhaseThree[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_HALFVIBPHASE,(char *)(&(_pfPhaseHalf[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISLOCATION,(char *)(&(_pfDC[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_RESCHARVALUE,(char *)(&(_pfVibCharRes[iStartIndex_])),GD_FLOAT_SIZE*iLen_);
   iPerWaveBytes=_iRealPointNum*GD_FLOAT_SIZE;
   for (int loop=0;loop<iLen_;++loop)
	   memcpy(&(_pfSendCenterWaveBuf[loop*_iRealPointNum]),_ppfWave[loop+iStartIndex_],iPerWaveBytes);
   _lockDataVib.EndRead();
   pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)_pfSendCenterWaveBuf,iPerWaveBytes*iLen_);
   if(iRealLen==_iVibChannNum)
   {
	   _lockSendCenter.BeginWrite();
	   _bSendCenter=true;
	   _lockSendCenter.EndWrite();
   }
   return iLen_;
}
///发送所有轴位移数据
int CRealData::GetAllAxisDisplaceData(CFixComm *pFixComm_)
{
	_lockDataAxis.BeginRead();
	pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACE,(char *)_pfAxisDisplaceValue,GD_FLOAT_SIZE*_iAxisChannNum);
	_lockDataAxis.EndRead();
	return _iAxisChannNum;
}
///发送所有过程量数据
int CRealData::GetAllProcData(CFixComm *pFixComm_)
{
	_lockDataProc.BeginRead();
	pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCVALUE,(char *)_pfProcValue,GD_FLOAT_SIZE*_iProcChannNum);
	_lockDataProc.EndRead();
	return _iProcChannNum;
}

int CRealData::GetAllChannState(CFixComm *pFixComm_)
{
	int iVibChannNum=GetVibChannelNum();
	int iAxisChannNum=GetAxisDiplaceChannelNum();
	int iProcChannNum=GetProcChannelNum();

    _lockDataRunState.BeginRead();
    pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLPROCSTATE,(char*)_pbProcChannelState,iProcChannNum*sizeof(int));
	pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACESTATE,(char*)_pbAxisChannelState,iAxisChannNum*sizeof(int));
	pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_ALLVIBSTATE,(char*)_pbVibChannelState,iVibChannNum*sizeof(int));
	_lockDataRunState.EndRead();
	pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_PROCCHANNNUM,iProcChannNum);
	pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_AXISCHANNNUM,iAxisChannNum);
	pFixComm_->SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,iVibChannNum);

	return iVibChannNum+iAxisChannNum+iProcChannNum;
}