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

/**@brief ����װ�úź��豸��*/
void CRealData::SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iRatingRev1,int ag_iRatingRev2,int ag_iGraphType)
{
	_strSet=ag_strSet;
	_strPlant=ag_strPlant;
	_iRatingRev1=ag_iRatingRev1;
	_iRatingRev2=ag_iRatingRev2;
	_iGraphType=ag_iGraphType;
}

/**@brief ��Ӳ����Ϣ*/
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

/**@brief ��ӹ����������Ϣ*/
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

/**@brief �����λ�Ʋ����Ϣ*/
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

/// ������в����Ϣ����������������м����и�������������Ϣ
void CRealData::AddChannInfoEnd(void){
	_iVibChannNum=_vectorVibChann.size();
	_iProcChannNum=_vectorProcChann.size();
	_iAxisChannNum=_vectorAxisChann.size();
	_cvectorEndVib=_vectorVibChann.end();
	_cvectorEndProc=_vectorProcChann.end();
	_cvectorEndAxis=_vectorAxisChann.end();
}

/**@brief ��ʼ����������ÿ������Ӧ�ı���ֵ,������.ע�⣺�˺���һ��Ҫ��������������в����Ϣ��ϡ�
*���ڴ˺������к󣬾Ͳ���������Ӳ������Ϣ�ˣ�������������AddVibChannelInfo��AddProcChannInfo�������������
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

/// �õ��豸��
string CRealData::GetPlantID(void){
	return _strPlant;
}


/// �����豸ת��
void CRealData::UpdateRealPlantRev(float ag_iRev1,float ag_iRev2)
{
	_lockRev.BeginWrite();
	_iRealRev1=ag_iRev1;
	_iRealRev2=ag_iRev2;
	_lockRev.EndWrite();
}

///�����񶯲������,ֵ��ע����Ǵ˴��Ĳ��������Ǹ�һά���飬��Ϊ���������Ҫ������������ķ��͵��м�������ݣ����õ������ݾ���һά
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
    //��������
	for (int loop=0;loop<reallen;++loop){
		memcpy(_ppfWave[loop+ag_startIndex],&(ag_fwave[loop*ag_iRealPointNum]),_iRealPointNum*GD_FLOAT_SIZE);
	//	writefile(_ppfWave[loop+ag_startIndex],realPointNum,10+loop);
    }
	_lockDataVib.EndWrite();
	
	return reallen;  
}

///���ò������״̬����
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

///���ù������������״̬����
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

///������λ�Ʋ������״̬����
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

///���ù���������
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

///������λ������
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

///�õ�����豸����������
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

///�õ�����豸��λ������
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

/*!@brief �õ����豸�����񶯲�����
*/
int CRealData::GetVibChannelNum(void){
	return _iVibChannNum;
}

/*!@brief �õ����豸���й�����������
*/
int CRealData::GetProcChannelNum(void){
	return _iProcChannNum;
}

///�õ�����豸��λ��ͨ���ĸ���
int CRealData::GetAxisDiplaceChannelNum(void){
	return _iAxisChannNum;
}

///�õ�ָ�����͵����в����Ϣ
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

///�õ�ָ�����������Ĺ���������
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

///�õ�ָ����������������Ĺ���������
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

///�õ�ָ����λ�Ʋ�����λ������
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

///�õ�ָ����λ�Ʋ����������λ������
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

///�õ�ָ���񶯲�������ֵ���͵���������
int CRealData::GetSpecChannAndCharTypeVibData(string ag_strChann,float &ag_data,int ag_iCharType){
	int index=GetChannIndex(ag_strChann,GE_VIBCHANN);
    return GetSpecChannAndCharTypeVibData(index,ag_data,ag_iCharType);
}

///�õ�ָ���񶯲������������ֵ���͵���������
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

///�õ�ָ���񶯲��Ĳ��κ�Ƶ������
int CRealData::GetSpecChannVibWaveAndFreq(string ag_strChann,float *ag_pfData,int &ag_iPtNum,float *ag_pfFreqData,int &ag_iLines,float &ag_fDf){
	int iSensorType=0;
	int index=GetChannIndex(ag_strChann,iSensorType,GE_VIBCHANN);
	if (index==-1){   
		return -1;
	}
	float *pfWave;
	//�õ��ͻ�ָ������������Ҫ�������ݸ���
    int iLinesPt=ag_iLines*2.56;
    int iSmpFreq;
	_lockDataVib.BeginRead();
	//�жϿͻ�ָ�����������Ƿ�������,���������,��Ͱ���ʵ�����ݷ��͹�ȥ
	ag_iPtNum=ag_iPtNum>_iRealPointNum?_iRealPointNum:ag_iPtNum;
    //�ж�ʵ�������Ƿ������׷����Ĳ������ݸ���
	iLinesPt=iLinesPt>_iRealPointNum?_iRealPointNum:iLinesPt;
	iSmpFreq=_iSmpFreq;
    pfWave=new float[iLinesPt];
	//�õ��׷�����Ҫ�Ĳ�������
	memcpy(pfWave,_ppfWave[index],(int)iLinesPt*4);
	//�õ��ͻ���Ҫ��ʾ�Ĳ�������
	memcpy(ag_pfData,_ppfWave[index],ag_iPtNum*sizeof(float));
	_lockDataVib.EndRead();
//	CString sTmp;sTmp.Format("%d,%d,%d",_iRealPointNum,ag_iPtNum,iLinesPt);AfxMessageBox(sTmp);
    ag_iLines=iLinesPt/2.56;
	if(iLinesPt>1)
	{
		ag_fDf=(float)iSmpFreq/iLinesPt;
		if(iSensorType==GE_SENSORTYPE_VOTEX){//���ֵ��
			SpectralMeasureMent(pfWave,iLinesPt,ag_pfFreqData,EA_PEAK_PEAK_SPEC);	
		}
		else{//��ֵ��
			SpectralMeasureMent(pfWave,iLinesPt,ag_pfFreqData,EA_PEAK_SPEC);
		}
	}
	delete [] pfWave;
	return index;
}

///�õ�ָ���񶯲��Ĳ���
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

///�õ�ָ����������Ͳ�����͵ı���״̬�Լ�����ֵ�����û�б����ͷ���false
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

///�õ�ָ�����Ͳ�����͵ı���״̬�Լ�����ֵ�����û�б����ͷ���false
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

/**�õ�ָ���񶯲��ϸ����
   *param��
      *       ag_strChann��string,����
	  *       ag_pfZoomSpec:float*,ϸ��������
	  *       ag_iLines:int&,����������������Ϻ���޸�ʵ�ʷ�����������������ϸ���׵����ݳ���
	  *       ag_fStartFreq:float&, ϸ����������ʼƵ�ʣ�������Ϻ��ʾϸ�������ʼƵ��
	  *       ag_iStopFreq:int,ϸ����������ֹƵ��
	  *       ag_fDf��float&,ϸ���׵ķֱ���
	  *return:0��ʾ������ȷ,������ʾ������
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
	{//��ʼƵ�ʱ���С����ֹƵ�ʣ�����Ƶ�ʵ�һ�벻�ܴ�����ֹƵ��
		nErrorCode=-3;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
    }
	ag_fDf=(ag_iStopFreq-ag_fStartFreq)/ag_iLines;
	//Ҫ����СҪϸ��2��
    if(!((2*ag_fDf)<fRealDf))
	{
        nErrorCode=-1;
		goto CLEAN_GETSPECCHANNZOOMSPEC;
	}
    //ϸ���������ܳ���20��
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
///����������λ������
int CRealData::GetAllAxisDisplaceData(CFixComm *pFixComm_)
{
	_lockDataAxis.BeginRead();
	pFixComm_->SetItemBuf(FIELD_SERVICE_ON_SEND_AXISDISPLACE,(char *)_pfAxisDisplaceValue,GD_FLOAT_SIZE*_iAxisChannNum);
	_lockDataAxis.EndRead();
	return _iAxisChannNum;
}
///�������й���������
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