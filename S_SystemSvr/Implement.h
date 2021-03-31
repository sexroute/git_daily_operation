//Implement.h
#pragma  once

#ifndef _SYSTEMSVR_IMPLEMENT_H_
#define _SYSTEMSVR_IMPLEMENT_H_
extern BOOL IsTempMode();
extern int G_TEMP_ONLINE;
class CImplement
{
public:
	CImplement(void);
	~CImplement(void);
public:
	/**修改监测中心中间件IP和Port
	*输入参数：
	*           所在分厂       FIELD_SYSTEM_FACTORY
	*           装置名ID        FIELD_SYSTEM_SET
	*          中间件IP    FIELD_SYSTEM_MW_IP
	*          中间件PORT  FIELD_SYSTEM_MW_PORT
	*返回值的字段码：
	*          状态     FIELD_SYSTEM_STATUS
	*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
	int SysModifyMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到监测中心中间件IP和Port
	*           所在分厂       FIELD_SYSTEM_FACTORY
	*           装置名ID        FIELD_SYSTEM_SET
	*返回值的字段码：
	*          中间件IP    FIELD_SYSTEM_MW_IP
	*          中间件PORT  FIELD_SYSTEM_MW_PORT
	#define     SERVICE_SYSTEM_GET_MWINFO                      25021
	*/
	int SysGetMwInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**修改发送到监测中心的时间间隔和是否发送状态
	*           所在分厂       FIELD_SYSTEM_FACTORY
	*           装置名ID        FIELD_SYSTEM_SET
	*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
	*          是否发送状态    FIELD_SYSTEM_SENDC_IF
	*返回值的字段码：
	*          状态     FIELD_SYSTEM_STATUS
	*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022
	*/
	int SysModifySendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到发送到监测中心的时间间隔和是否发送
	*           所在分厂       FIELD_SYSTEM_FACTORY
	*           装置名ID        FIELD_SYSTEM_SET
	*返回值的字段码：
	*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
	*          是否发送状态    FIELD_SYSTEM_SENDC_IF
	#define     SERVICE_SYSTEM_GET_SENDINFO                     25023
	*/
	int SysGetSendInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);


	/**得到装置级中间件信息
	*返回的字段码：
	*         分厂名字      FIELD_SYSTEM_FACTORY
	*         分厂别名      FIELD_SYSTEM_FACTORY_ALIAS
	*         中间件标识    FIELD_SYSTEM_MW_ID
	*         装置名ID      FIELD_SYSTEM_SET
	#define    SERVICE_SYSTEM_GET_SETSVRINFO                     25024
	*/
	int SysGetSetSvrInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到装置级中间件服务器的设备详细信息
	*        设备起始索引  FIELD_SERVICE_STARTINDEX
	*返回的字段码：
	*        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
	*        版本          FIELD_SYSTEM_VERSION
	*        设备信息      FIELD_SYSTEM_SETSVR_PLANTINFO
	*        显示集团      FIELD_SERVICE_GROUP_VISIBLE
	*        显示公司      FIELD_SERVICE_COMPANY_VISIBLE
	*        显示分厂      FIELD_SERVICE_FACTORY_VISIBLE
	*        显示装置      FIELD_SERVICE_SET_VISIBLE
	*        显示设备      FIELD_SERVICE_PLANT_VISIBLE
	*        集团名称      FIELD_SYSTEM_GROUP
	*        国家名称      FIELD_SYSTEM_COUNTRY
	*        设备索引      FIELD_SERVICE_ENDINDEX
	*        设备数目      FIELD_SERVICE_PLANT_NUM
	*        设备显示名称  FIELD_SYSTEM_PLANT_DISPLAY
	*        ERP编码       FIELD_SYSTEM_PLANT_ERP_CODE
	#define    SERVICE_SYSTEM_GET_SETSVRPLANTINFO                25025
	*/
	int SysGetSetSvrPlantInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

	/**得到装置级中间件服务器指定设备的测点详细信息
	* 输入字段码：
	设备号：FIELD_SYSTEM_PLANT
	*返回的字段码：
	*        测点信息  FIELD_SYSTEM_SETSVR_CHANNINFO
	*        个数      FIELD_SYSTEM_SETSVR_INFONUM
	#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN            25026
	*/
	int SysGetSetSvrSpecPlantChann(ICommInterFace * pFixComm, IDBInterFace *pDB);
	/**得到所有用户信息
	*输入字段码：
	*返回字段码：
	*            用户名         FIELD_SYSTEM_USEID
	*            密码           FIELD_SYSTEM_PASSWORD
	*            用户级别       FIELD_SYSTEM_USER_LEVEL
	*            所在分厂       FIELD_SYSTEM_FACTORY
	*            装置名ID       FIELD_SYSTEM_SET
    *            是否浏览装置   FIELD_SYSTEM_BROWSE_SET
	*            刷新时间 FIELD_SYSTEM_REFRESHTIME
	#define    SERVICE_SYSTEM_GET_ALLUSERINFO               25027 */
	int SysGetAllUserInfo(ICommInterFace * pFixComm,IDBInterFace * pDB);

	/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
	*输入参数：公司名     FIELD_SYSTEM_COMPANY
	*          分厂名     FIELD_SYSTEM_FACTORY
	*返回值的字段码：
	*          机组号     FIELD_SYSTEM_PLANT
	*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
	*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
	*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
	*          装置级IP   FIELD_SYSTEM_MW_IP
	*          装置级Port FIELD_SYSTEM_MW_PORT
	*          设备类型   FIELD_SYSTEM_TYPE
	*          数据源     FIELD_SYSTEM_DATASOURCE
	*          显示名称   FIELD_SYSTEM_PLANT_DISPLAY
	*          ERP编码    FIELD_SYSTEM_PLANT_ERP_CODE
	#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
	*/
	int SysGetPlantIPofSubCorp(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/*
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP_EX          25210
*/
	int SysGetPlantIPofSubCorpEx(ICommInterFace *pFixComm,IDBInterFace *pDB);


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
	int SysGetMachineInfoType(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的信息描述表
	* 输入字段码：
	*返回的字段码：
	*        信息类型      FIELD_SYSTEM_TYPE
	*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
	*/
	int SysGetInfoTypeDetail(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**根据测点信息类型,得到监测中心的信息描述表
	* 输入字段码：
	*        信息类型   FIELD_SYSTEM_TYPE
	*返回的字段码：
	*        索引       FIELD_SYSTEM_STATUS
	*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
	*        单位       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
	*/
	int SysGetInfoTypeDetailBaseType(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
	*输入参数：   公司名         FIELD_SYSTEM_COMPANY
	*             分厂名         FIELD_SYSTEM_FACTORY
	*             机组名         FIELD_SYSTEM_PLANT
	*             测点名         FIELD_SYSTEM_CHANN
	*             是否常规报警   FIELD_SYSTEM_STATUS //1为判断报警，0为不判断报警
	*             报警限类型     FIELD_SYSTEM_EVENT_TYPE
	*             报警上限       FIELD_SYSTEM_ALARM_LOW
	*             报警上上限     FIELD_SYSTEM_ALARM_HIGH
	*             报警下限       FIELD_SYSTEM_ALARM_LOW2
	*             报警下下限     FIELD_SYSTEM_ALARM_HIGH2
	*返回值的字段码：
	*             返回状态       FIELD_SYSTEM_STATUS
	#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115
	*/
    int SysModifySpecChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的自定义参数模板表
	* 输入字段码：
	*返回的字段码：
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         自定义参数模板ID       FIELD_SYSTEM_TEMPID
	*         自定义信息描述         FIELD_SYSTEM_STATUS_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRTEMPLET      25116
	*/
    int SysGetSvrTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的自定义参数信息表
	*输入参数值：
	*返回参数值：
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         高级参数模板ID         FIELD_SYSTEM_TEMPID
	*         数据类型               FIELD_SYSTEM_DATA
	*         单位类型               FIELD_SYSTEM_UNIT
	*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
	*         段起始号               FIELD_SYSTEM_SEG_STARTNO
	*         段数                   FIELD_SYSTEM_SEG_NUM
	*         段名                   FIELD_SYSTEM_SEG_NAME
	*         数据起始值             FIELD_SYSTEM_START_VALUE
	*         数据结束值             FIELD_SYSTEM_END_VALUE
	*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
	*         预处理参数K             FIELD_SERVICE_PREPROC_K
	*         预处理参数B             FIELD_SERVICE_PREPROC_B
	*         单位                   FIELD_SERVICE_UNIT
	#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMPARAM      25117
	*/
	int SysGetCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的自定义参数报警参数模板表
	*输入参数值：
	*返回参数值：
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         自定义参数模板ID         FIELD_SYSTEM_TEMPID
	*         自定义报警模板ID         FIELD_SYSTEM_ALARM_TEMPID
	*         自定义报警描述           FIELD_SERVICE_ALARM_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRALARMTEMPLET      25118
	*/
    int SysGetSvrAlarmTemplet(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的自定义参数报警参数信息表
	*输入参数值：
	*返回参数值：
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
	*         判断方式               FIELD_SYSTEM_TYPE
	*         报警方式               FIELD_SERVICE_ALARM_TYPE
	*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
	*         是否报警               FIELD_SYSTEM_IF_ALARM
	*         报警低限               FIELD_SERVICE_ALARM_LOW
	*         报警高限               FIELD_SERVICE_ALARM_HIGH
	*         报警组                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119
	*/
    int SysGetSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的通道自定义参数报警参数信息表
	*输入参数值：
	*返回参数值：
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         测点号                 FIELD_SYSTEM_CHANN
	*         判断方式               FIELD_SYSTEM_TYPE
	*         报警方式               FIELD_SERVICE_ALARM_TYPE
	*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
	*         段数                   FIELD_SYSTEM_SEG_NUM
	*         是否报警               FIELD_SYSTEM_IF_ALARM
	*         报警低限               FIELD_SERVICE_ALARM_LOW
	*         报警高限               FIELD_SERVICE_ALARM_HIGH
	*         报警组                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SETSVRCHANNELALARMPARAM     25120
	*/
    int SysGetSvrChannAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂和装置ID的自定义参数信息
	*输入参数值：
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备名                FIELD_SERVICE_PLANTID
	*返回参数值：
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*         设备号                 FIELD_SYSTEM_PLANT
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         高级参数模板ID         FIELD_SYSTEM_TEMPID
	*         数据类型               FIELD_SYSTEM_DATA
	*         单位类型               FIELD_SYSTEM_UNIT
	*         信号分析方式           FIELD_SYSTEM_SIGNAL_TYPE
	*         段起始号               FIELD_SYSTEM_SEG_STARTNO
	*         段数                   FIELD_SYSTEM_SEG_NUM
	*         段名                   FIELD_SYSTEM_SEG_NAME
	*         数据起始值             FIELD_SYSTEM_START_VALUE
	*         数据结束值             FIELD_SYSTEM_END_VALUE
	*         特征值类型             FIELD_SYSTEM_CHAR_TYPE
	*		  特征值单位			 FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121
	*/
    int SysGetSpecSvrCustomParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到指定公司、分厂和装置ID的自定义参数报警参数信息
	*输入参数值：
	*         公司名                 FIELD_SYSTEM_COMPANY
	*         分厂名                 FIELD_SYSTEM_FACTORY
	*         装置名ID               FIELD_SYSTEM_SET
	*返回参数值：
	*         索引ID                 FIELD_SYSTEM_STATUS
	*         自定义报警参数模板ID   FIELD_SYSTEM_ALARM_TEMPID
	*         判断方式               FIELD_SYSTEM_TYPE
	*         报警方式               FIELD_SERVICE_ALARM_TYPE
	*         报警段起始号           FIELD_SYSTEM_SEG_STARTNO
	*         段数                   FIELD_SYSTEM_SEG_NUM
	*         是否报警               FIELD_SYSTEM_IF_ALARM
	*         报警低限               FIELD_SERVICE_ALARM_LOW
	*         报警高限               FIELD_SERVICE_ALARM_HIGH
	*         报警组                 FIELD_SYSTEM_ALARM_GROUP
	#define    SERVICE_SYSTEM_GET_SPECSETSVRALARMPARAM       25122
	*/
    int SysGetSpecSvrAlarmParam(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心配置的自定义轴承参数表
    #define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123
	*/
    int SysGetSetSvrCustomBearingTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的齿轮箱参数表
    #define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124
	*/
	int SysGetSetSvrGearBoxTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的位置参数表
	#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125
	*/
    int SysGetSetSvrLocationTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**得到监测中心的部位参数表
	#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126
	*/
    int SysGetSetSvrSectionTbl(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**得到中间件的版本号
	*输入参数值：
	*返回参数值：
	*         版本号                 FIELD_SERVICE_FILE_VERSION
	* #define   SERVICE_SYSTEM_GET_MIDDLEWARE_VERSION     25127
	*/
    int SysGetMiddlewareVersion(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/* 守护巡检 */
	/* 获取各个程序的路径和版本号 */
	int SysGetModuleVersion(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* 获取各个数据库状态 */
	int SysGetDBStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* 获取中间件运行状态 */
	int SysGetServerStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* 获取数采硬件状态 */
	int SysGetHWDAQStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* 获取数采软件状态 */
	int SysGetDAQSWStatus(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/* 获取基本的组态息  */
	int SysGetBasicConfigInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

    /**得到指定装置ID的所有ID值，即ID标识，如果检索为空，则返回0
	  */
	int GetSetID(IDBInterFace * pDB_,string sSetID_);
	static CString GetSetID2( string &sCompany_,  string &sFactory_,  string &sPlantID_, IDBInterFace *pDB_);
	///判断装置件服务器是什么级别
	int CheckSetSvrLevel(IDBInterFace *pDB_);
	/**通过设备类型、测点类型和传感器类型得到特征值描述
	  #define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
	  */
	int GetCharTypeDesc(ICommInterFace *pFixComm,IDBInterFace *pDB);
    ///获得指定设备No
	static string GetPlantNo(const string &sCompany_, const string &sFactory_, const string &sPlantID_, IDBInterFace *pDB_);
	//得到这个公司的数据源id号
	int GetCompanyDBID(IDBInterFace *pDB_,string sCompany_);
private:
	//设置是否有装置号和分厂号的查询条件
	string SetSetFactoryCond(string sCompany_,string sFactory_,string sSetID_);
	//判断指定的中间件ID是什么级别的ID
	int CheckSpecSetIDLevel(IDBInterFace *pDB_,string sSetID_);
	///通道设备类型、测点类型、传感器类型得到指定特征值描述对应关键值，返回关键值
	int GetCharTypeDescKey( const int &iMachineType_, const int &iChannType_, const int &iSensorType_ );

	CString GetValueByKey(map<CString,CString> &aoMapData,CString astrCompany,CString astrFactory,CString astrPlant);
};
#endif