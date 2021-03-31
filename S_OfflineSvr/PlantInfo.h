#pragma once

#include "PlantInfoBase.h"
#include <additions/BHOleDbWrapper.h>
#include <additions/BHCommWrapper.h>
class CPlantInfoOffLine: public CPlantInfoBase
{
public:
	CPlantInfoOffLine();
	virtual ~CPlantInfoOffLine(void);
protected:
	//删除一些分配的数据
	virtual void Destory();
	
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
public:
	virtual void AddChannInfoEnd();
	///查询指定测点是否存在轴心位置对应的测点,如果空，则表示没有
	string GetSpecChannAxisChann(string sChann_);
	///得到指定测点的同转速测点链表
	list<string> GetSpecChannSameRevChanns(string sChann_,int &iRevType_);
  
	/**@brief 通过指定的轴心位置描述得到轴心位置的详细信息
	  *@param sLocation_ 轴心位置描述
	  *@return 如果返回大于0，说明计算正确，否则错误
	  */
	int CalcSpecLocationValue(string sLocation_,float fGapVer_,float fDCVer_,float fGapHor_,float fDCHor_,float &fX_,float &fY_);
	int CalcSpecLocationValue(string sChannHor_,string sChannVer_,float fGapVer_,float fDCVer_,float fGapHor_,float fDCHor_,float &fX_,float &fY_);
	
	/**@brief 通过指定动态测点的活塞杆轴心位置描述得到活塞杆轴心位置的详细信息
	*@param sLocation_ 轴心位置描述
	*@return 如果返回大于0，说明计算正确，否则错误
	*/
	bool CalcSpecDynChannLocationValue(string sLocation_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	bool CalcSpecDynChannLocationValue(string sChannHor_,string sChannVer_,int iWaveNum_,DATATYPE_WAVE *pHorWave_,DATATYPE_WAVE *pVerWave_,DATATYPE_WAVE *pHorAxesWave_,DATATYPE_WAVE *pVerAxesWave_);
	
	/**保存启停车数据，同步对应4系列中间件，同时兼容旧版本的数采
	  *@param iStartIndex_ 测点的起始索引
	  */
	int SaveSudData(int iStartIndex_,int iNum_,S_VibCharValue *pVibCharValue_,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_,IDBInterFace *pDB_);

	 int SaveVibSudData(IDBInterFace *apDB, long aEventID, CBHDateTime aDatetime, int aMicroSecond, int aStartIndex,int aChannNum, S_VibCharValue *apCharValue, DATATYPE_WAVE *apWave, int aWaveNum, S_CustomParamData *apParam, string aTemplate, int aSmpNum, int aSmpFreq, float apRev[5], int apHead[]);

	 int SaveDynSudData(IDBInterFace *apDB, long aEventID, CBHDateTime aDatetime, int aMicroSecond, int aStartIndex,int aChannNum, S_DynCharValue *apCharValue, DATATYPE_WAVE *apWave, int aWaveNum, S_CustomParamData *apParam, string aTemplate, int aSmpNum, int aSmpFreq, float apRev[5], int apHead[]);

	 int SaveProcSudData( IDBInterFace * apDB, int aEventID, CBHDateTime aDateTime, int aMicrosecond, float* aCharVal, int aStartIndex, int aChannNum, float apRev[5]);

	/**保存快变报警数据，同步对应4系列中间件，同时兼容旧版本的数采
	  *@param iStartIndex_ 测点的起始索引
	  */
	int SaveVibFAlarmData( IDBInterFace *pDB_, int iStartIndex_,int iNum_,S_VibCharValue *pVibCharValue_,unsigned int *piType_,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_, int apHead[]);

	/**保存动态报警数据,同步对应4系列中间件
	*@param iStartIndex_ 测点的起始索引
	*/
	int SaveDynAlarmData(IDBInterFace *pDB_,int iStartIndex_,int iNum_,S_DynCharValue *pDynCharValue_,unsigned int *piType_,DATATYPE_WAVE *pWave_,S_CustomParamData *pfCustomParom_,string sTempletID_,long lEventID_,CBHDateTime dtDataTime_,int iMicroSecond_,int iRealAllNum_,int apHead[]);

	/**保存过程量报警数据,同步对应4系列中间件*/
	int SaveProcAlarmData( IDBInterFace * pDB_, int AlarmType_, int EventID_, CBHDateTime DateTime_, int iMicroSecond_, CString ChannNo_, int ChannType, float ChannValue, int Rev_ );

    /**更新快变报警状态结束时间
	  *@return 失败返回-2，成功返回0
	  */
    int UpdateVibAlarmStatus(int iChannIndex_,int iEventID_,CBHDateTime dtEnd_,int iNewThID_,IDBInterFace * pDB_);

    /**添加一条完整的报警日志
	  *@param sChann_ 通道ID
	  *@param iEventID_ 报警事件ID
	  *@param iLogID_  日志ID
      *@param dtStart_ 报警开始时间
	  *@param dtEnd_   报警结束时间
	  *@param iChannType_ 通道类型
	  *@param uiAlarmType_ 报警类型
	  *@param iThID_ 报警前的阈值ID
	  *@param iThIDNew_ 报警后新学习的阈值ID
	  *@return 失败返回-2，成功返回0
	  */
    int AddAlarmLog(string sChann_,int iEventID_,int iLogID_,CBHDateTime dtStart_,CBHDateTime dtEnd_,int iChannType_,unsigned int uiAlarmType_,int iThID_,int iThIDNew_,IDBInterFace *pDB_,int iAlarmLevel = 0,int iStopFlag = 1);

	/**判断是否和当前设备相符
	  *@return 如果相符，就返回true,否则返回false
	  */
    bool CheckPlant(string sCompany_,string sFactory_,string sPlant_);

   /**得到指定启停车多个测点的特征值字段合
     *@param index_ 起始索引
	 *@param iNum_  测点个数
	 *@return 需要的字段
     */
   string GetSudChannCharValueField(int index_,int iNum_);

   /**得到指定启停车多个测点的特征值、波形字段
     *@param index_ 起始索引
     *@param iNum_  测点个数
     *@return 需要的字段
     */
   string GetSudChannAllField(int index_,int iNum_);
   
   /**得到指定测点、指定类型的启停车表字段名
     *@index_ 启停车测点索引
	 *@iType_ 测点特征值类型，如果为-1，则返回波形字段名
	 *@return 返回需要的字段名
	 */
   string GetSudChannSpecTypeField(int index_,int iType_=GE_ALLCHAR);
   string GetSudChannSpecTypeField(const string &sChannNo_,int iType_=GE_ALLCHAR);

   /**得到指定动态测点、指定类型的动态表字段名
   *@index_ 动态测点索引
   *@iType_ 测点特征值类型，如果为-1，则返回波形字段名
   *@return 返回需要的字段名
   */
   string GetDynChannSpecTypeField(int index_,int iType_=0);
   string GetDynChannSpecTypeField(const string &sChannNo_,int iType_=0);

   //得到指定动态测点,指定时刻的周期起始索引和结束索引,当bAlarmData_为true时，为查询报警数据表。
   void GetSpecDynChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_=false);

   //得到指定动态测点,指定时刻的周期起始索引和结束索引,当bAlarmData_为true时，为查询报警数据表。
   void GetICESpecDynChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_=false);

   //得到指定振动测点,指定时刻的周期起始索引和结束索引,当bAlarmData_为true时，为查询报警数据表。
   void GetSpecVibChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_=false);
   //得到内燃机指定振动测点,指定时刻的周期起始索引和结束索引,当bAlarmData_为true时，为查询报警数据表。
   void GetICESpecVibChannStartEndIndex(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iStartIndex_, int &iEndIndex_,bool bAlarmData_=false);

   //得到指定测点所属的汽缸编号,成功则返回所属汽缸编号，否则返回-1.
   int GetSpecChannCylinderNo(IDBInterFace *pDB_,string sChann_);

   //得到指定动态测点,指定时刻的采样频率，采样点数和转速
   void GetSpecDynChannFreqNumRev(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iSmpFreq_,int &iSmpNum_,int &iRev_,bool bExactTime_=true);
   //得到指定振动测点,指定时刻的采样频率，采样点数和转速
   void GetSpecVibChannFreqNumRev(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,int &iSmpFreq_,int &iSmpNum_,int &iRev_,bool bExactTime_=true);

   //得到指定动态测点,指定时刻的自定义参数
   void GetSpecDynChannCustomParam(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,float *fCustomParom_);
   //得到指定振动测点,指定时刻的自定义参数
   void GetSpecVibChannCustomParam(IDBInterFace *pDB_,string sChann_,CBHDateTime timeSpec_,float *fCustomParom_);

   //将获得3系列发送助手的特征值转化为4系列对应的特征值
   int ChangeVibCharValueToNew(S_VibCharValue *pVibCharValue_,S_OldVibCharValue *pOldVibCharValue_,float *pfRev_,int iSmpNum_,int iSmpFreq_,int iRealNum_,int iLen_,int iStartIndex_);
 
   int ChangeDynCharValueToNew(S_DynCharValue *pDynCharValue_,S_OldDynCharValue *pOldDynCharValue_,float *pfRev_,const int &iSmpNum_, const int &iSmpFreq_, const int iRealNum_,const int &iLen_,const int &iStartIndex_);

   //得到指定时间指定测点类型、特征值类型的测点数据和报警状态
   bool GetSpecTimeChannAndTypeAlarmState(IDBInterFace *pDB_,CBHDateTime timeSpec_,string sSuffix_,int index_,string &sChann_,string &sChannAlias_,string &sUnit_,int &iList_,unsigned int &iState_,int &iChannDetailType_,int iType_);


   //根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的前两个动态压力测点，如果小于两个则返回 -1
   int GetDynPressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynChann1_,string &sDynChann2_);

   //根据缸号从测点表中查询出指定公司、分厂、设备,对应缸号的动态应力测点，如果小于两个则返回 -1
   int GetDynStressChann(IDBInterFace *pDB_,string sCompany_,string sFactory_,string sPlant_,const int iCylinderNo_,string &sDynStressChann_);

   ///得到指定类型的所有测点信息
   int GetSpecTypeChannInfo(S_ChannInfo &sChannInfo_,int iIndex_=0,int iType_=GE_ALLPROC);

   int SetOffVibAxesLocationParam(int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_);

   int SetOffDynAxesLocationParam(int anHorChannIndex,int anVerChannIndex,int anAxisChannIndex,const S_ChannInfo& aHorChannInfo,const S_ChannInfo& aVerChannInfo,const CAxisChannInfo& aAxisChanInfo_);

public:///为了同步（从下一级向上级服务器同步）数据使用的接口
   IReadWriteLock *_pLockSudData;

   /**添加一条完整的启停车日志
     *@param sudType_ 启停车类型
	 *@param dtStart_ 启停车开始时间
	 *@param dtEnd_   启停车结束时间
	 *@return 如果成功返回事件ID，失败返回-1
     */
   int AddWholeSudLog(IDBInterFace *pDB_,E_SUD_TYPE sudType_,CBHDateTime dtStart_,CBHDateTime dtEnd_);

protected:
   /*是否进行采集*/
   BOOL m_bIsSample;
   CBHDateTime m_oSetIsSampleTime;

public:
   BOOL IsSample() const;
   void IsSample(BOOL val);
   CBHDateTime SetIsSampleTime() const;
   void SetIsSampleTime(CBHDateTime val);

};
