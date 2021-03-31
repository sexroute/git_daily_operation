//#include "OnlineGlobal.h"
#pragma once
#include <list>
#include <string>
#include "additions/DebugHelper.h"

struct S_PlantStatus;

struct structChannel
{		
	int m_nType;
	int m_nOrd;
	int m_nXPos;
	int m_nYPos;
	int m_nWidth;
	int m_nHeight;	
	//char * m_strComments;
	char m_strComments[100];
};


//key
struct AFX_EXT_CLASS SUB_SYSTEM_KEY
{
	int		m_nNumber;		//部件的客户端编号(与graph.mdb中的不一样)
	int		m_nImageKind;	//0:概貌图, 1:装配图, 2:原理图
	BOOL	m_bHightAlarm;	//高报/低报
};

//部件信息
struct AFX_EXT_CLASS SUB_SYSTEM
{
	int		m_nXOffset;
	int		m_nYOffset;
	BYTE *	m_imageBuffer;
};

inline BOOL WriteBackDataForService_SysGetSpecTypeChann(ICommInterFace * pFixComm,const S_ChannInfo & loInfo,CChannInfoBase * lpVib)
{
	if (lpVib== NULL)
	{
		return FALSE;
	}

	try
	{
		pFixComm->SetItem(FIELD_SERVICE_ON_SEND_PLANTID,loInfo._cPlantNo);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN,loInfo._cChannID);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1,loInfo._cChannNo);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_ALIAS,loInfo._cChannAlias);
		pFixComm->SetItem(FIELD_SERVICE_CHANN1_UNIT,loInfo._cUnit);
		pFixComm->SetItem(FIELD_SYSTEM_CHANN_TYPE,loInfo._iChannType);
		pFixComm->SetItem(FIELD_SYSTEM_SENSOR_TYPE,loInfo._iSensorType);
		pFixComm->SetItem(FIELD_SERVICE_CYLINDERNO,loInfo._iCylinderNo);
		pFixComm->SetItem(FIELD_SYSTEM_TYPE,loInfo._iInfoType);


		pFixComm->SetItem(FIELD_SYSTEM_EVENT_TYPE,loInfo._chAlarmParam._iRegJudgeType);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW,loInfo._chAlarmParam._fAlarmHL);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH,loInfo._chAlarmParam._fAlarmHH);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_LOW2,loInfo._chAlarmParam._fAlarmLH);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_HIGH2,loInfo._chAlarmParam._fAlarmLL);

		pFixComm->SetItemBuf(FIELD_SERVICE_THRESHOLD_BUFF, (char*)&(loInfo._chAlarmParam), sizeof(loInfo._chAlarmParam));

		pFixComm->SetItem(FIELD_SYSTEM_TEMPID,loInfo._cTempID);
		pFixComm->SetItem(FIELD_SYSTEM_ALARM_TEMPID,loInfo._cAlarmTempID);
		pFixComm->SetItem(FIELD_SYSTEM_DATASOURCE,loInfo._iDataSource);

		CChannSections loSectionInfo = lpVib->GetChannSectionsInfo();

		if (loSectionInfo._iSectionIDX <=0)
		{
			pFixComm->SetItem(FIELD_SERVICE_SEC_NAME, _T("默认部位"));
			pFixComm->SetItem(FIELD_SERVICE_SEC_IDX,  -1);	

		}else
		{
			pFixComm->SetItem(FIELD_SERVICE_SEC_NAME, loSectionInfo._cSectionName);
			pFixComm->SetItem(FIELD_SERVICE_SEC_IDX,  loSectionInfo._iSectionIDX);
		}

		CChannLocations loLocationInfo = lpVib->GetChannLocationsInfo();

		if (loLocationInfo._iLocationIDX>0)
		{
			pFixComm->SetItem(FIELD_SERVICE_LOC_NAME, loLocationInfo._cLocationName);
			pFixComm->SetItem(FIELD_SERVICE_LOC_IDX,  loLocationInfo._iLocationIDX);
		}
		else
		{
			//ASSERT(FALSE);
			pFixComm->SetItem(FIELD_SERVICE_LOC_NAME, _T("默认位置"));
			pFixComm->SetItem(FIELD_SERVICE_LOC_IDX,  -1);
		}

		CBearingParamInfo loBearingInfo = lpVib->GetBearingParamInfo();

		BOOL lbRetBearingSet = FALSE;

		if (loBearingInfo._iBearingIDX>0)
		{
			if (loBearingInfo._iBearingType == GE_ROLLING_BEARING)
			{
				lbRetBearingSet = TRUE;
				pFixComm->SetItem(FIELD_SYSTEM_GEARVALID,1);
				pFixComm->SetItem(FIELD_SYSTEM_PITCHDIAM,loBearingInfo._fPitchDiameter);
				pFixComm->SetItem(FIELD_SYSTEM_NUMOFROLLING,loBearingInfo._iElementsNum);
				pFixComm->SetItem(FIELD_SYSTEM_CONTACKANGLE,loBearingInfo._fContactAngle);
				pFixComm->SetItem(FIELD_SYSTEM_ROLLINDIAM,loBearingInfo._fElementDiameter);
			}
		}


		pFixComm->SetItem(FIELD_SERVICE_CHANN1_LIST, loInfo._iList);
		pFixComm->SetItem(FIELD_SERVICE_REV, loInfo._iRevType);
		pFixComm->SetItem(FIELD_SERVICE_KEY_IF, loInfo._bKeyIF);
		pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY, loInfo._fSensitivity);
		pFixComm->SetItem(FIELD_SERVICE_SENSITIVITY1, loInfo._fSensitivity1);
		pFixComm->SetItem(FIELD_SERVICE_TREND_GAP, loInfo._fGapVoltage);
		pFixComm->SetItem(FIELD_SERVICE_LOCATION_NO, loInfo._iLocationNo);
		pFixComm->SetItem(FIELD_SERVICE_MEASURE_DIRECTION, loInfo._iMeassureDirection);
		pFixComm->SetItem(FIELD_SERVICE_DATA_TYPE, loInfo._iDataType);
		pFixComm->SetItem(FIELD_SERVICE_DC_AMPLIFY, loInfo._fDCAmp);
		pFixComm->SetItem(FIELD_SERVICE_AC_AMPLIFY, loInfo._fACAmp);

		pFixComm->Write(pFixComm->GetAci());
		pFixComm->CreateNextAnswer();
	}
	catch (...)
	{
		//log here
		ASSERT(FALSE);
		CString lstrErrorLog;
		lstrErrorLog.Format(_T("*******************Error Occoured!%s %d******************"),__FILE__,__LINE__);
		CHZLogManage::Fatal(lstrErrorLog);
		zdlTraceLine(lstrErrorLog);
		return FALSE;
	}

	return TRUE;

};

class CTransactData
{
public:
	CTransactData(void);
	~CTransactData(void);
	/**读取所有各种测点的特征值，通过循环依次取得各个测点的所有类型特征值。
	#define     SERVICE_SERVICE_ON_GETALLTYPECHAR         20002
	*/
	int GetAllTypeChar(ICommInterFace* pFixComm);
	// 得到指定机组所有振动测点所有特征值
	int GetAllFiveChar(ICommInterFace * pFixComm);
	// 得到指定机组指定测点的所有特征值
	int GetSpecFiveChar(ICommInterFace * pFixComm);
	/**得到指定任意多个振动测点的频谱。（测点之间“，”分开）。如果多个测点，就循环提取
	*/
	int GetSpectrum(ICommInterFace * pFixComm,CCustomDB *pDB);
	// 得到指定机组指定四个振动测点的指定类型特征值
	int GetFourSpecChar(ICommInterFace * pFixComm);
	// 得到指定机组所有过程量测点的值
	int GetAllProcValue(ICommInterFace * pFixComm);
	// 得到指定机组指定过程量测点的值
	int GetSpecProcValue(ICommInterFace * pFixComm);
	// 得到指定机组指定两组测点的实时波形数据
	int GetTwoWave(ICommInterFace * pFixComm);
	// 得到指定机组指定一组测点的轴心轨迹数据
	int GetAxesTrack(ICommInterFace* pFixComm);
	// 得到指定机组指定一组测点的轴心位置
	int GetAxisLocation(ICommInterFace* pFixComm);
	// 得到指定机组指定轴位移测点的值
	int GetSpecAxisDisplaceValue(ICommInterFace* pFixComm);
	// 得到指定机组的所有轴位移测点值
	int GetAllAxisDisplace(ICommInterFace* pFixComm);
	///判断通道的所属大类。及判断通道一级分类
	E_TBL_CHANNTYPE GetChannOneType(const CHANNTYPE &iChannType_);

		/**批次读取监测站级实时报警统计
	#define     SERVICE_SERVICE_ON_BATCH_MONITORALARMSTAT           20033
	*/
	int OnBatchGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**得到用户所观测设备范围内所有设备的测点运行状态，通过循环依次得到所有报警的测点
  　　*输入参数值：用户ID         FIELD_SYSTEM_USEID
  　　*返回值字段码：
  　　*            报警的设备ID         FIELD_SERVICE_PLANTID
  　　*            报警的测点ID         FIELD_SERVICE_CHANN1
  　　*            报警值               FIELD_SERVICE_CHANN1VALUE
  　　#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
  　　*/
	int GetAllChannelState(ICommInterFace * pFixComm,IDBInterFace *pDB);
	// 得到指定设备的所有测点特征值，振动测点需要制定测点特征类型
	int GetAllChannValue(ICommInterFace * pFixComm, CCustomDB * pDB);

	/**得到指定任意个动态压力测点示功图信息。循环得到各个测点的数据
     #define   SERVICE_SERVICE_ON_GETPVDATA               20015
	*/
	int GetPVData(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**得到指定任意多个沉降量测点的实时波形、频谱和所有特征值。（测点之间“，”分开）。如果多个测点，就循环提取
	#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
	*/
	int GetRodSinkData(ICommInterFace* pFixComm,CCustomDB *pDB);

	/**读取指定动态压力测点、活塞杆沉降量测点、振动测点各一个。读取其波形和特征参数
	#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
	*/
	int OnGetRcMultiParamData(ICommInterFace* pFixComm,CCustomDB* pDB);

	/**读取集团级实时报警统计数据
	#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
	*/
	int OnGetGroupAlarmStat(ICommInterFace* pFixComm,CCustomDB* pDB);

	/**读取公司级实时报警统计数据
	#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT           20025
	*/
	int OnGetCompanyAlarmStat(ICommInterFace *pFixComm,CCustomDB * pDB);

	/**读取监测站级实时报警统计
	#define     SERVICE_SERVICE_ON_MONITORALARMSTAT           20026
	*/
    int OnGetMonitorAlarmStat(ICommInterFace *pFixComm,CCustomDB *pDB);
	int OnGetSpecPlantMonitorAlarmStat(ICommInterFace *pFixComm,
		CCustomDB *pDB,
		CAlarmPlantInfo & arefPlantInfo,
		int anPlantIndex,
		int anTotalPlants);

	/**得到指定机组指定一组测点的活塞杆轴心位置轨迹
    #define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION     20027
	*/
	int OnGetRodSinkAxesLocation(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**得到指定设备指定测点类型的所有测点名和测点号，通过循环依次得到
    #define     SERVICE_SYSTEM_GETSPECTYPECHANN     25003
	*/
	int Service_SysGetSpecTypeChann(ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**往复式系统应力监测，得到指定机组指定缸号的应力波形和振动测点波形
    #define     SERVICE_SERVICE_ON_ GETDYNSTRESSWAVE           20028
	*/
    int OnGetDynStressWave(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到某一组提纯轴心轨迹的波形数据
	#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
	*/
	int OnGetPurifyAxesTrack(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**得到指定振动测点的实时波形和倒谱。（如何振动测点2不设置参数，则只提取一组）
	#define     SERVICE_SERVICE_ON_GETCEPSTRUM                  20030
	*/
	int OnGetCepstrum(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**得到指定一对振动测点的实时全谱和全谱瀑布图。
	#define     SERVICE_SERVICE_ON_GETFULLSPECSTRUM          20031
	*/
    int OnGetFullSpectrum(ICommInterFace *pFixComm,CCustomDB *pDB);

	//得到振动测点的幅值和相位
	int GetAllSpecCharAndPhase(ICommInterFace* pFixComm,CCustomDB* pDB);
	//得到指定公司,分厂所有机组转速
    int GetPlantRev(ICommInterFace *pFixComm);
	int GetSubCorpState(ICommInterFace * pFixComm, CCustomDB * pDB);
	int GetBode(ICommInterFace * pFixComm, CCustomDB * pDB);

    int GetWaterFall(ICommInterFace * pFixComm, CCustomDB * pDB);
	int GetZoomSpec(ICommInterFace * pFixComm,CCustomDB *pDB);
	int GetSpecChannInfo(ICommInterFace *pFixComm,CCustomDB *pDB);

	/* 获取异常数据检测结果 */
	int GetAbnormalDataRecord(ICommInterFace *pFixComm,CCustomDB * pDB);

	/**得到监测中心的设备信息表
	*输入参数：   公司名    FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SYSTEM_PLANT
	*返回的字段码：
    *             测点号     FIELD_SYSTEM_CHANN
	*             索引       FIELD_SYSTEM_STATUS
	*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
	*             单位       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
	*/
	int SysGetMachineInfoType(ICommInterFace *pFixComm,CCustomDB *pDB);

    //判断机组是否处于运行状态
	bool IsRun(CAlarmPlantInfo *pPlantInfo_);

	int GetBodeForTemp(ICommInterFace * pFixComm, CCustomDB * pDB);

	int GetWaterFallForTemp(ICommInterFace * pFixComm, CCustomDB * pDB);

	 /*!
	*输入参数字段码
	*          公司ID      FIELD_SYSTEM_COMPANY
	*	         分厂名      FIELD_SYSTEM_FACTORY
	*          机组ID：    FIELD_SERVICE_PLANTID
	*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
	*          测点类型    FIELD_SYSTEM_CHANN_TYPE
	*返回值的字段码：
	*		     是否修改成功：  FIELD_SYSTEM_STATUS
	*#define    SERVICE_SERVICE_ON_SAVESPECTIMEDATA    21080
	*/
   int SaveSpecTypeDataByTimeRegion(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
	*输入参数：  
   *         公司名         FIELD_SYSTEM_COMPANY
   *         分厂名         FIELD_SYSTEM_FACTORY
   *         机组名         FIELD_SYSTEM_PLANT
   *         测点名         FIELD_SYSTEM_CHANN
   *         测点类型：     FIELD_SERVICE_CHANNTYPE
   *         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
   *         报警限类型     FIELD_SYSTEM_ALARM_TYPE
   *         报警上限           FIELD_SYSTEM_ALARM_HIGH
   *         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
   *         报警下限           FIELD_SYSTEM_ALARM_LOW
   *         报警下下限         FIELD_SYSTEM_ALARM_LOW2
   *         修改报警限主机名   FIELD_SYSTEM_HOST_NAME
   *         修改报警限主机IP   FIELD_SYSTEM_CLIENT_IP
   *         修改报警限用户名   FIELD_SYSTEM_USER_NAME
   *返回值的字段码：
   *         返回状态       FIELD_SYSTEM_STATUS
   #define   SERVICE_SERVICE_ON_CHANGE_ALARM_LIMITATION 20044
   */
   int ChangeSpecChannAlarmLimitation(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**获取指定设备指定测点的报警信息
   *输入参数：  
   *         公司名         FIELD_SYSTEM_COMPANY
   *         分厂名         FIELD_SYSTEM_FACTORY
   *         机组名         FIELD_SERVICE_PLANTID
   *         测点名         FIELD_SYSTEM_CHANN
   *         测点类型：     FIELD_SERVICE_CHANNTYPE
   *返回值的字段码：
   *         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
   *         报警限类型     FIELD_SYSTEM_ALARM_TYPE
   *         报警上限           FIELD_SYSTEM_ALARM_HIGH
   *         报警上上限         FIELD_SYSTEM_ALARM_HIGH2
   *         报警下限           FIELD_SYSTEM_ALARM_LOW
   *         报警下下限         FIELD_SYSTEM_ALARM_LOW2
   *         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
   #define   SERVICE_SERVICE_ON_GET_ALARM_LIMITATION 20045
   */
   	int GetSpecChannAlarmLimitation( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**中间件中获取概貌图信息接口
	*输入参数：  
	*         公司名         FIELD_SYSTEM_COMPANY
	*         分厂名         FIELD_SYSTEM_FACTORY
	*         机组名         FIELD_SERVICE_PLANTID
	*         测点名         FIELD_SYSTEM_CHANN
	*         测点类型：     FIELD_SERVICE_CHANNTYPE
	*返回值的字段码：
	*         是否常规报警   FIELD_SERVICE_DATA_TYPE //1为判断报警，0为不判断报警
	*         报警限类型     FIELD_SYSTEM_ALARM_TYPE
	*         报警上限       FIELD_SYSTEM_ALARM_LOW
	*         报警上上限     FIELD_SYSTEM_ALARM_HIGH
	*         报警下限       FIELD_SYSTEM_ALARM_LOW2
	*         报警下下限     FIELD_SYSTEM_ALARM_HIGH2
	*         返回状态       FIELD_SYSTEM_STATUS // 为1表示获取成功
	#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_INFOMATION 20048
	*/
    int GetPlantGraphInformationS( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_GET_PLANTGRAPH_POSITION_SUBSYSTEM_ALARM_INFO 20049
	*/
	int GetPlantGraphPositonAndAlarmInfo( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_SET_PLANT_STATUS 24041
	*/
	int SetPlantStatus( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/*
	#define   SERVICE_SERVICE_ON_GET_PLANT_STATUS 24042
	*/
	int GetPlantStatus( ICommInterFace * pFixComm, IDBInterFace *pDB );

	/**获取指定设备的无线测点状态
	#define   SERVICE_SERVICE_ON_GET_WIRELESSCHANN_STATUS 20047
	*/
	int GetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);
	
	/*!
	#define    SERVICE_SERVICE_ON_TEST_MW_LINK_STATUS    24116
	*/
	int GetMWLinkStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/*!
	#define    SERVICE_SERVICE_ON_NOTIFY_ICE    24117
	*/
	int NotUpdateICEWatchDog(ICommInterFace * pFixComm, IDBInterFace *pDB);

	 /**中间件中获取报警文件信息
   *输入参数：  
   *         文件路径                   FIELD_SERVICE_FILE_PATH
   *         文件名                     FIELD_SERVICE_FILE_NAME
   *         
   *返回值的字段码：
   *         文件名                     FIELD_SERVICE_FILE_NAME
   *         文件内容                   FIELD_SERVICE_FILE_CONTENT
   *         
   
	#define   SERVICE_SERVICE_ON_GET_LOG_FILE_CONTENT 20050
	*/
	int GetLogFileContent(ICommInterFace * pFixComm, IDBInterFace *pDB);

   /**
  根据设备测点得到分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *          一级门限        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          二级门限        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          三级门限        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *          状态代码        FIELD_SYSTEM_STATUS
  *
 
	#define    SERVICE_SERVICE_ON_GET_CHANN_GRADE_THRESHOLD         20051
  */
	int GetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);

	/**
  根据设备测点设置分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          一级门限        FIELD_SERVICE_GRADE_ONE_THRESHOLD
  *          二级门限        FIELD_SERVICE_GRADE_TWO_THRESHOLD
  *          三级门限        FIELD_SERVICE_GRADE_THREE_THRESHOLD
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  
	#define    SERVICE_SERVICE_ON_SET_CHANN_GRADE_THRESHOLD         20052
  */
	int SetChannGradeThreshold(ICommInterFace* pFixComm,IDBInterFace *pDB);


	/**
  根据设备测点得到轴心位置配置信息
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *返回值的字段码：
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *          状态代码        FIELD_SYSTEM_STATUS
  *
  * #define    SERVICE_SERVICE_ON_GET_AXESLOCATION_PARAM         20053
  */

  int GetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

/**
  根据设备测点设置分级门限
  *输入参数字段码
  *          公司名          FIELD_SYSTEM_COMPANY
  *          分厂名          FIELD_SYSTEM_FACTORY
  *          设备名          FIELD_SERVICE_PLANTID
  *          测点名          FIELD_SYSTEM_CHANN
  *          测点类型        FIELD_SERVICE_CHANNTYPE
  *          间隙电压        FIELD_SERVICE_TREND_GAP
  *          传感器灵敏度    FIELD_SERVICE_SENSITIVITY
  *返回值的字段码：
  *          状态代码        FIELD_SYSTEM_STATUS
  * #define    SERVICE_SERVICE_ON_SET_AXESLOCATION_PARAM         20054
  */

  int SetAxesLocationParam(ICommInterFace* pFixComm,IDBInterFace *pDB);

private:
	/**解析buffer中的测点名
	  *@param pChannBuf_  测点序列缓冲区
	  *@param iNum_       测点序列缓冲区中测点的个数
	  *@return 测点联表
	  */
	list<string> ParseChannList(char *pChannBuf_,const int &iNum_);
	//得到报警状态
	int GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,CCustomDB *pDB_,bool bChann_=false);
	//得到报警状态
	int GetSpecGroupChannAlarmState(string sGroup_,ICommInterFace *pFixComm_,IDBInterFace *pDB_,bool bChann_=false);
	int GetSpecCompanyChannAlarmState(string sGroup_,string sCompany_,ICommInterFace *pFixComm_,bool bChann_=false);
	int GetSpecFactoryChannAlarmState(string sGroup_,string sCompany_,string sFactory_,ICommInterFace *pFixComm_,bool bChann_=false);
	int GetAllPlantChannAlarmState(ICommInterFace *pFixComm_,bool bChann_=false);


	//得到报警统计信
	int GetSpecGroupAlarmStatics(string sGroup_,CCustomDB *pDB_, S_PlantNumStatics &plantNumStatics_);//int *iPlantNums_ = NULL)
	int GetSpecGroupAlarmStatics(string sGroup_,IDBInterFace *pDB_, S_PlantNumStatics &plantNumStatics_);//int *iPlantNums_ = NULL)
	int GetSpecCompanyAlarmStatics(string sGroup_,string sCompany_,S_PlantNumStatics &plantNumStatics_);
	int GetSpecFactoryAlarmStatics(string sGroup_,string sCompany_,string sFactory_,S_PlantNumStatics &plantNumStatics_);



	/**从指定网络连接中读取并判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	*@param sCompany_ 公司名，是一个引用
	*@param sFactory_ 分厂名，是一个引用
	*@param sPlant_ 设备名，是一个引用
	*@param pFixComm_ 网络连接指针
	*@param bRtn_  是否返回网络连接信息
	*@return 如果验证成功，则返回1，否则返回NULL
	*/
	CPlantInfoOnLine * ReadCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_=true,const PLANTINFOTYPE &plantInfoType_=PLANTINFO_REAL);

    /**
	  */
	int SetSpecPlantAlarmState(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_);
	int SetSpecPlantAlarmStateEx(ICommInterFace *pFixComm_,CAlarmPlantInfo *pPlantInfo_,CCustomDB *pDB_);


	int SetPlantAlarmState(CPlantInfoPool * apPlantPool,
		                 std::string asPlant,
		                 ICommInterFace * apFixComm,
		                 CCustomDB * apDB);

	int SetFactoryAlarmState(CFactoryInfoPool * apFactoryPool,
		                 std::string asFactory,
		                 ICommInterFace * apFixComm,
		                 CCustomDB * apDB);

	int SetCompanyAlarmState(CCompanyInfoPool * apCompanyPool,
						 std::string asCompany,
						 ICommInterFace * apFixComm,
						 CCustomDB * apDB);

	/**判断否公司、分厂、设备号有空，并得到表名的后缀，并通过网络连接指针返回
	  *@param sCompany_ 公司名，是一个引用
	  *@param sFactory_ 分厂名，是一个引用
	  *@param sPlant_ 设备名，是一个引用
	  *@param sAlias_ 是一个引用，用于把公司别名传出来
	  *@param pFixComm_ 网络连接指针
	  *@param bRtn_  是否返回网络连接信息
	  *@return 如果验证成功，则返回1，否则返回NULL
	*/
	CPlantInfoOnLine * CheckCompanyInfo(const string &sCompany_,const string &sFactory_,const string &sPlant_,ICommInterFace *pFixComm_,const bool &bRtn_=true,const PLANTINFOTYPE &plantInfoType_=PLANTINFO_REAL);

	/**
	 如果自己判断报警, 返回EventID, 否则返回下级同步上来的EventID
	 */
	static void GetAlarmEventID(CAlarmPlantInfo *apPlantInfo, long & anAlarmId, long & anAlarmSync);

	//获取表的前缀
	CString GetTblNamePrefix(const CHANNTYPE &iChannType_,E_TREND_TBL_TYPE & iTblType_);

private:

	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CAlarmPlantInfo* apPlantInfo);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CPlantInfoPool* apPlantPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CPlantInfoPool* apPlantPool, const std::string& asPlant);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CFactoryInfoPool* apFactoryPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CFactoryInfoPool* apFactoryPool, const std::string& asFactory);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CCompanyInfoPool* apCompanyPool);
	int GetSpecPlantStatus(ICommInterFace* apFixComm, IDBInterFace* apDB, CCompanyInfoPool* apCompanyPool, const std::string& asCompany);

	int SkipSpecPlantStatus(S_PlantStatus* apStatus, CString& asError);
	int SetSpecPlantStatus(S_PlantStatus* apStatus, CString& asError);

	int GetDefaultGraphID(int nPlantType_);
};

///解释报警状态，如果返回0，说明没有报警，1表示常规低报，2表示常规高报，3表示快变报警，4表示换变报警，5表示趋势报警。
///数字越小，越优先,即如果存在常规报警情况下，就不会向下判断，是否有快变、缓变等。
int ParseAlarmStatus(unsigned int iState_);

