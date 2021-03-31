#pragma once
#include "stdafx.h"

class CRealData
{
public:
	CRealData(void);
	~CRealData(void);
private:
	typedef list<S_ChannInfo> LIST_S_CHANNINFO;

	/*!@brief 所有振动测点向量，顺序和在数据库中通道表单中的顺序相同
	*/
	LIST_S_CHANNINFO _vectorVibChann;

	/*!@brief 所有过程量测点向量，顺序和在数据库中通道表单中的顺序相同
	 */
	LIST_S_CHANNINFO _vectorProcChann;
    
	/*!@brief 所有轴位移测点向量，顺序和在数据库中通道表单中的顺序相同
	  */
	LIST_S_CHANNINFO _vectorAxisChann;

	LIST_S_CHANNINFO::const_iterator _cvectorEndVib;
	LIST_S_CHANNINFO::const_iterator _cvectorEndProc;
	LIST_S_CHANNINFO::const_iterator _cvectorEndAxis;

	///判断是否已经读取数据发送到企业监测中心,如果True就表示已经发送，False表示没有发送
	bool _bSendCenter;
	///读取数据发送到企业监测中心时，常用到缓存，大小100K,及25K个浮点数
    float *_pfSendCenterWaveBuf;

	///装置号
	string    _strSet;
	///设备号
	string    _strPlant;
	int       _iVibChannNum;
	int       _iProcChannNum;
	int       _iAxisChannNum;
	int       _iSmpNum;
	///实际每个通道传递过来的波形点数
	int       _iRealPointNum;
	int       _iSmpFreq;
	float     *_pfVibCharAll;
	float     *_pfVibCharOne;
	float     *_pfVibCharTwo;
	float     *_pfVibCharThree;
	float     *_pfVibCharHalf;
	float     *_pfVibCharRes;//残振
	float     *_pfPhaseOne;
	float     *_pfPhaseHalf;
	float     *_pfPhaseTwo;
	float     *_pfPhaseThree;
	float     *_pfDC;
	float     **_ppfWave;
	float     *_pfProcValue;
	float     *_pfAxisDisplaceValue;
	float     _iRealRev1;
	float     _iRealRev2;
	float     _iRatingRev1;
	float     _iRatingRev2;
    int       _iGraphType;         /*!概貌图类型*/
	
	/*!此设备所有过程量测点是运行状态，用以通知中间件此设备所有过程量测点运行状态。  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbProcChannelState;

	/*!此设备所有轴位移测点是运行状态，用以通知中间件此设备所有轴位移测点运行状态。  
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbAxisChannelState;

	/*!此设备所有振动测点是运行状态，用以通知中间件此设备所有振动测点运行状态。
	*normal state:0
	*low alarm state:1
	*high alarm state:2
	*/
	int      *_pbVibChannelState;    


	///过程量数据读写锁
	CMutexRW _lockDataProc;

	///振动测点数据读写锁
	CMutexRW _lockDataVib;
    
	///轴位移测点数据读写锁
	CMutexRW _lockDataAxis;
  
	///转速读写锁
	CMutexRW _lockRev;

	///测点运行状态数据锁
	CMutexRW _lockDataRunState;

	///发送到监测中心，更新数据读写锁
    CMutexRW _lockSendCenter;
public:
	/**@brief 设置装置号和设备号*/
	void SetSetPlant(string ag_strSet, string ag_strPlant,int ag_iRatingRev1,int ag_iRatingRev2,int ag_iGraphType=0);

	/**@brief 添加测点信息*/
	void AddVibChannelInfo(string ag_strChann,string ag_strUnit,int ag_index,int ag_iListofFrame,int ag_iSensorType,int ag_iofRev=1);

	/**@brief 添加过程量测点信息*/
	void AddProcChannInfo(string ag_strChann, string ag_strUnit,int ag_index,int ag_iListofFrame,int ag_iChannType,int ag_iSensorType,int ag_iofRev=1);
   
	/**@brief 添加轴位移测点信息*/
	void AddAxisChannInfo(string ag_strChann,string ag_strUnit,int ag_index,int ag_iListofFrame,int ag_iofRev=1);

	/// 添加所有测点信息结束，在这个函数中计算有个这个机组相关信息
	void AddChannInfoEnd(void);

	/**@brief 初始化测点参数和每个测点对应的报警值,和链表.注意：此函数一定要运行在添加完所有测点信息完毕。
	*即在此函数运行后，就不允许再添加测点了信息了，即不允许运行AddVibChannelInfo和AddProcChannInfo这个两个函数了
	*/
	void InitParam(float *ag_pfCharAll,
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
		int   *ag_pbAxisDisplaceChannelState);

	/// 得到设备号
	string GetPlantID(void);

	/// 更新设备转速
	void UpdateRealPlantRev(float ag_iRev1,float ag_iRev2);

	///更新振动测点数据
	int UpdateVibData(float * ag_pfCharOne,
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
		int ag_startIndex=0);
   
	///更新测点运行状态数据
	int UpdateVibRunState(int * ag_piState,int ag_len,int ag_startIndex=0);

	///更新过程量测点运行状态数据
	int UpdateProcRunState(int * ag_piState,int ag_len,int ag_startIndex=0);

	///更新轴位移测点运行状态数据
	int UpdateAxisRunState(int * ag_piState,int ag_len,int ag_startIndex=0);

	///更新过程量数据
	int UpdateProcData(float *ag_pfData,int ag_len,int ag_startIndex=0);

	///更新轴位移数据
	int UpdateAxisDisplaceData(float *ag_pfData,int ag_len,int ag_startIndex=0);
 
	///得到这个设备过程量数据
	int GetProcData(float *ag_pfData,int ilen,int istart=0);
    
	///得到这个设备轴位移数据
    int GetAxisData(float *ag_pfData,int ilen,int istart=0);

	/*!@brief 得到此设备所有振动测点个数
	*/
	int GetVibChannelNum(void);

	/*!@brief 得到此设备所有过程量测点个数
	*/
	int GetProcChannelNum(void);

	///得到这个设备轴位移通道的个数
	int GetAxisDiplaceChannelNum(void);

	///得到指定类型的所有测点信息
	int GetSpecTypeChannInfo(S_ChannInfo * ag_schannInfo,int ag_iType=GE_ALLPROC);
	
	///得到指定过程量测点的过程量数据
	int GetSpecChannProcData(string ag_strChann,float &ag_data);

	///得到指定过程量测点索引的过程量数据
	int GetSpecChannProcData(int ag_index,float &ag_data);
	
	///得到指定轴位移测点的轴位移数据
	int GetSpecChannAxisData(string ag_strChann,float &ag_data);

	///得到指定轴位移测点索引的轴位移数据
	int GetSpecChannAxisData(int ag_index,float &ag_data);
	
	///得到指定振动测点和特征值类型的特征数据
	int GetSpecChannAndCharTypeVibData(string ag_strChann,float &ag_data,int ag_iCharType=GE_ALLCHAR);
    
	///得到指定振动测点索引和特征值类型的特征数据
	int GetSpecChannAndCharTypeVibData(int ag_index,float &ag_data,int ag_iCharType=GE_ALLCHAR);

	///得到指定振动测点的波形和频谱数据
	int GetSpecChannVibWaveAndFreq(string ag_strChann,float *ag_pfData,int &ag_iPtNum,float *ag_pfFreqData,int &ag_iLines,float &ag_fDf);
    
	///得到指定测点类型和测点的测点对应索引
    int GetChannIndex(string ag_strChann,int &ag_iSensorType,int ag_iType);
     
	int GetChannIndex(string ag_strChann,int ag_iType);

	///得到指定测点索引和测点类型的报警状态以及报警值，如果没有报警就返回false
	bool GetSpecChannAndTypeAlarmState(int ag_index,float &ag_data,int &ag_iState, int ag_iType=GE_ALLPROC);

	///得到指定测点和测点类型的报警状态以及报警值，如果没有报警就返回false
	bool GetSpecChannAndTypeAlarmState(string ag_strChann,float &ag_data,int &ag_iState, int ag_iType=GE_ALLPROC);

	///得到设备的实时转速
	void GetRealPlantRev(float &ag_iRev1,float & ag_iRev2);
	
	/**得到设备的指定测点对应的转速
	  *@param strChann_ 指定的测点号
      *@param fRealRev_ 得到实时转速
	  *@return 额定转速
	  */
	float GetSpecChannRev(string strChann_,float &fRealRev_);
  
	///得到采样频率和采样点数
	void GetSmpInfo(int &iSmpFreq_,int &iSmpNum_);

	///得到这个设备是否报警，即有没有报警的测点
	bool GetPlantRunState();

    ///得到指定振动测点的波形
	int GetSpecChannVibWave(string ag_strChann,float *ag_pfData,int ag_len=GD_SENDWAVENUM);
	
	/**得到指定振动测点细化谱
      *param：
      *       ag_strChann：string,测点号
	  *       ag_pfZoomSpec:float*,细化谱数据
	  *       ag_iLines:int&,分析线数，分析完毕后会修改实际分析出来的线数，即细化谱的数据长度
	  *       ag_fStartFreq:float&, 细化分析的起始频率，分析完毕后表示细化后的起始频率
	  *       ag_iStopFreq:int,细化分析的终止频率
	  *       ag_fDf：float&,细化谱的分辨率
	  *       ag_pfWave:波形数据
	  *       ag_iPtNum:波形数据长度
	  *return:0表示分析正确,负数表示错误码
	  */
	int GetSpecChannZoomSpec(string ag_strChann,float *ag_pfZoomSpec,long &ag_iLines,float &ag_fStartFreq,int ag_iStopFreq,float &ag_fDf,float *ag_pfWave,int &ag_iPtNum);

public:
	///发送数据到企业中心之类的函数接口
    
	///发送所有振动波形数据,还包括转速
	int GetAllVibData(CFixComm *pFixComm_,int iStartIndex_,int iLen_);
    ///发送所有轴位移数据
	int GetAllAxisDisplaceData(CFixComm *pFixComm_);
	///发送所有过程量数据
	int GetAllProcData(CFixComm *pFixComm_);
	///发送所有测点的运行状态
	int GetAllChannState(CFixComm *pFixComm_);
};
