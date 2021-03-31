#if !defined(AFX_PROCESS)
#define AFX_PROCESS


//定义函数


/**通过公司名、分厂名、机组号得到可以做启停车的测点和其对应的额定转速
*输入参数：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*返回值的字段码：
*             测点号    FIELD_SYSTEM_CHANN
*             额定转速  FIELD_SERVICE_REV
  *define    SERVICE_SYSTEM_GETPLANTSUDCHANN           25000
  */
int Service_SysGetPlantSudChann(ICommInterFace * pFixComm, IDBInterFace * pDB );

/**得到指定分厂下所有设备位号，通过循环依次得到所有设备位号
  *输入参数：公司名     FIELD_SYSTEM_COMPANY
  *          分厂名     FIELD_SYSTEM_FACTORY
  *          装置名ID        FIELD_SYSTEM_SET
  *返回值的字段码：
  *          设备号     FIELD_SYSTEM_PLANT
  *          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
  *define     SERVICE_SYSTEM_GETPLANTOFSUBCORP          25001
  */
int Service_SysGetPlantOfSubCorp(ICommInterFace * pFixComm, IDBInterFace * pDB );

/**得到指定公司名、分厂名、机组位号的所有测点名和对应的测点类型，通过循环依次得到
*输入参数：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             测点号    FIELD_SYSTEM_CHANN
  *             测点类型  FIELD_SYSTEM_CHANN_TYPE
  *define     SERVICE_SYSTEM_GETALLCHANN                25002
  */
int Service_SysGetAllChann(ICommInterFace * pFixComm, IDBInterFace * pDB );


/**得到指定设备指定测点类型的所有测点名，通过循环依次得到
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SYSTEM_PLANT
*             测点类型   FIELD_SYSTEM_CHANN_TYPE
*返回值的字段码：
*             测点号     FIELD_SYSTEM_CHANN
*             测点位号   FIELD_SYSTEM_CHANN_ALIAS
*             测点单位   FIELD_SERVICE_CHANN1_UNIT
*             测点类型    FIELD_SYSTEM_CHANN_TYPE
*             传感器类型 FIELD_SYSTEM_SENSOR_TYPE
*             所属汽缸号 FIELD_SERVICE_CYLINDERNO
*             信息类型   FIELD_SYSTEM_TYPE
*             报警限类型        FIELD_SYSTEM_EVENT_TYPE
*             报警上限          FIELD_SYSTEM_ALARM_LOW
*             报警上上限        FIELD_SYSTEM_ALARM_HIGH
*             报警下限          FIELD_SYSTEM_ALARM_LOW2
*             报警下下限        FIELD_SYSTEM_ALARM_HIGH2
*             部位名称          FIELD_SERVICE_SEC_NAME
*             部位索引          FIELD_SERVICE_SEC_IDX
*             位置名称          FIELD_SERVICE_LOC_NAME
*             位置索引          FIELD_SERVICE_LOC_IDX
*define     SERVICE_SYSTEM_GETSPECTYPECHANN           25003
  */
int Service_SysGetSpecTypeChann(CFixComm * pFixComm, IDBInterFace *pDB );

/**通过用户名和密码得到用户级别、集团名、公司名，如果用户级别是集团级，就需要循环得到各个公司
*输入参数：用户名   FIELD_SYSTEM_USEID
*          密码     FIELD_SYSTEM_PASSWORD
*返回值的字段码：
*          用户级别 FIELD_SYSTEM_USER_LEVEL
*          集团名   FIELD_SYSTEM_GROUP
*          公司名   FIELD_SYSTEM_COMPANY
*          分厂名   FIELD_SYSTEM_FACTORY
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_GETCOMPANYTOFUSER
*/
int Service_SysGetCompanyOfUser(ICommInterFace * pFixComm, IDBInterFace * pDB );

/**得到指定公司名、分厂名、机组位号的所有组轴心轨迹对应的测点对，通过循环依次得到，如果没有指定设备号，则是服务器所有轴心轨迹对
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             轴心位置结构体   FIELD_SYSTEM_AXISLOCATION
  *define     SERVICE_SYSTEM_GETALLAXISCHANN
  */
int Service_SysGetAllAxisChann(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定设备位号的所对应的最高分析线数对应的宏定义
*输入参数：   公司名    FIELD_SYSTEM_COMPANY
*             分厂名    FIELD_SYSTEM_FACTORY
*             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             最高分析线数宏定义   FIELD_SYSTEM_ANALYSISLINES
  *define     SERVICE_SYSTEM_GETPLANTANALYSISLINES
  */
int Service_SysGetPlantAnalysislines(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定公司、分厂、设备的设备详细信息
  *输入参数：   公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             设备信息  FIELD_SYSTEM_SETSVR_PLANTINFO
  #define     SERVICE_SYSTEM_GETPLANTINFO      25007
  */
int Srv_SysGetPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定公司的所有分厂名，通过循环依次得到所有分厂名。如果没有指定公司名，就得到所有分厂名
*输入参数：
*                公司名    FIELD_SYSTEM_COMPANY
*返回值的字段码：
*                公司名    FIELD_SYSTEM_COMPANY
*                分厂名    FIELD_SYSTEM_FACTORY
*                分厂别名  FIELD_SYSTEM_SUBCORP_ALIAS
#define     SERVICE_SYSTEM_GETALLFACTORY        25008
*/
int Srv_SysGetAllFactory(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定所有公司信息
*返回值的字段码：
*                公司名    FIELD_SYSTEM_COMPANY
*                公司别名  FIELD_SYSTEM_COMPANY2
*                集团名    FIELD_SYSTEM_GROUP
#define     SERVICE_SYSTEM_GETALLCOMPANY        25009
*/
int Srv_SysGetAllCompany(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
*输入参数：公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*返回值的字段码：
*          公司名     FIELD_SYSTEM_COMPANY
*          分厂名     FIELD_SYSTEM_FACTORY
*          机组号     FIELD_SYSTEM_PLANT
*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
*          集团级IP   FIELD_SYSTEM_GROUP_MW_IP
*          集团级Port FIELD_SYSTEM_GROUP_MW_PORT
*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
*          装置级IP   FIELD_SYSTEM_MW_IP
*          装置级Port FIELD_SYSTEM_MW_PORT
*          装置级中间件ID   FIELD_SYSTEM_SET
*          设备类型   FIELD_SYSTEM_TYPE
*          数据源     FIELD_SYSTEM_DATASOURCE
*          显示名称   FIELD_SYSTEM_PLANT_DISPLAY
*          ERP编码    FIELD_SYSTEM_PLANT_ERP_CODE
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
*/
int Svr_SysGetPlantIPofSubCorp(ICommInterFace * pFixComm, IDBInterFace * pDB);

/*
#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25210
*/
int Svr_SysGetPlantIPofSubCorpEx(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**通过用户名和密码,修改密码
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *          新密码   FIELD_SYSTEM_RESERVE_STRING
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *                 如果为-1，表示没有修改成功，为1表示修改成功
  *#define     SERVICE_SYSTEM_MODIFYPWD                  25011
  */
int Service_SysModifyPwd(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**通过测点类型和传感器类型得到特征值描述
*输入参数：
 *             设备类型    FIELD_SYSTEM_TYPE
*             测点类型    FIELD_SYSTEM_CHANN_TYPE
*             传感器类型  FIELD_SYSTEM_SENSOR_TYPE
*返回值的字段码：
*             顺序     FIELD_SYSTEM_STATUS
*             对应描述 FIELD_SYSTEM_STATUS_DISCRIPTION
*             特征值单位   FIELD_SERVICE_CHANN1_UNIT
#define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
*/
int Srv_SysGetCharTypeDesc(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**通过用户名得到客户端页面的更新时间
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *返回值的字段码：
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
#define     SERVICE_SYSTEM_GET_REFRESHTIME            25013
  */
int Service_SysGetRefreshTime(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定设备指定测点类型的所有测点名和对应的报警上下限，通过循环依次得到
  *输入参数：
  *             公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             设备号    FIELD_SYSTEM_PLANT
  *             测点类型  FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *             测点号    FIELD_SYSTEM_CHANN
  *             报警上限  FIELD_SYSTEM_ALARM_HIGH
  *             报警下限  FIELD_SYSTEM_ALARM_LOW
#define     SERVICE_SYSTEM_GETALARMTHRESHOLD           25014
  */
int Service_SysGetThreshold(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**通过用户名和密码登陆，如果登陆成功。如果用户登录成功，则状态为1，并返回用户级别和刷新时间；否则为-1，并且不返回用户级别和刷新时间。
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          浏览装置 FIELD_SYSTEM_BROWSE_SET （果为1，则浏览装置，否则不浏览）
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  #define     SERVICE_SYSTEM_LOGIN                      25015
  */
int Service_SysLogin(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**修改用户级别。如果成功，则返回状态为1，否则为-1，并说明原因
  *注意：只有管理员才有权限修改用户级别，并且如果是分厂管理员只能修改所在分厂的用户级别。
  *input param：用户名         FIELD_SYSTEM_USEID
  *             密码           FIELD_SYSTEM_PASSWORD
  *             被修改的用户名 FIELD_SYSTEM_MODIFY_USER
  *             用户级别       FIELD_SYSTEM_USER_LEVEL
  *return field code：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  #define    SERVICE_SYSTEM_MODIFY_USERLEVEL            25016
  */
int Service_SysModifyUserLevel(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**@brief Add new user.如果成功，则返回状态为1，否则为-1，并说明原因。
  *注意：只有管理员才有权限添加用户，并且，如果是分厂管理员只能添加为所在分厂的用户。
  *input param:用户名         FIELD_SYSTEM_USEID
  *            密码           FIELD_SYSTEM_PASSWORD
  *            新用户名       FIELD_SYSTEM_MODIFY_USER
  *            用户级别       FIELD_SYSTEM_USER_LEVEL
  *            所在分厂       FIELD_SYSTEM_FACTORY
  *return field code：
  *            状态     FIELD_SYSTEM_STATUS
  *            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  #define    SERVICE_SYSTEM_ADD_NEWUSER                 25017
  */
int Service_SysAddNewUser(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**设置指定机组和测点的是否进行报警检查，如果状态字段输入为1，则检查报警；为0，则不检查报警
*返回值说明：状态如果为1，说明修改成功，否则可以从状态描述中得到错误原因
*输入参数：   机组号    FIELD_SYSTEM_PLANT
*             测点号    FIELD_SYSTEM_CHANN
*             状态     FIELD_SYSTEM_STATUS
*返回值的字段码：
*            状态     FIELD_SYSTEM_STATUS
*            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_SETCHECKALARMFLAG          25018
*/
int Service_SysSetCheckAlarmFlag(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定机组位号的图结构类型
  *             公司名    FIELD_SYSTEM_COMPANY
  *             分厂名    FIELD_SYSTEM_FACTORY
  *             机组号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *				机组号          FIELD_SYSTEM_PLANT
  *			    概貌图类型      FIELD_SYSTEM_PLANT_GRAPHTYPE
  *             概貌图BUFF      FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF
  *             概貌图BUFF尺寸  FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE
  #define     SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID          25019
  */
int Service_SysGetGraphTypeByDeviceID(ICommInterFace * pFixComm, IDBInterFace * pDB );


/**修改监测中心中间件IP和Port
*输入参数：
 *           公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          中间件IP    FIELD_SYSTEM_MW_IP
*          中间件PORT  FIELD_SYSTEM_MW_PORT
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MOIDFY_MWINFO                   25020*/
int Svr_SysModifyMwInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);


/**得到监测中心中间件IP和Port
 *           公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*返回值的字段码：
*          中间件IP    FIELD_SYSTEM_MW_IP
*          中间件PORT  FIELD_SYSTEM_MW_PORT
#define     SERVICE_SYSTEM_GET_MWINFO                      25021
*/
int Svr_SysGetMwInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**修改发送到监测中心的时间间隔和是否发送状态
 *           公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
*          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
*返回值的字段码：
*          状态     FIELD_SYSTEM_STATUS
*          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define     SERVICE_SYSTEM_MODIFY_SENDINFO                 25022
*/
int Svr_SysModifySendInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到发送到监测中心的时间间隔和是否发送
*         公司ID        FIELD_SYSTEM_COMPANY
*           所在分厂       FIELD_SYSTEM_FACTORY
*           装置名ID        FIELD_SYSTEM_SET
*返回值的字段码：
*          发送时间间隔    FIELD_SYSTEM_SENDC_INTERVAL
*          是否发送状态    FIELD_SYSTEM_SENDC_IF
*          发送启停车数据间隔  FIELD_SYSTEM_SUD_SENDC_INTERVAL
*          发送报警数据间隔    FIELD_SYSTEM_ALARM_SENDC_INTERVAL
#define     SERVICE_SYSTEM_GET_SENDINFO                     25023
*/
int Svr_SysGetSendInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);


/**得到装置级中间件信息
  *返回的字段码：
  *         分厂名字      FIELD_SYSTEM_FACTORY
  *         中间件标识    FIELD_SYSTEM_MW_ID
  *         装置名ID      FIELD_SYSTEM_SET
  #define    SERVICE_SYSTEM_GET_SETSVRINFO                     25024
  */
int Svr_SysGetSetSvrInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Svr_SysGetSetSvrPlantInfo(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**得到装置级中间件服务器指定设备的测点详细信息
* 输入字段码：
*        公司ID        FIELD_SYSTEM_COMPANY
*		 分厂名        FIELD_SYSTEM_FACTORY
设备号：FIELD_SYSTEM_PLANT
*返回的字段码：
*        测点信息  FIELD_SYSTEM_SETSVR_CHANNINFO
*        个数      FIELD_SYSTEM_SETSVR_INFONUM
#define    SERVICE_SYSTEM_GET_SETSVRSPECPLANTCHANN            25026
*/
int Svr_SysGetSetSvrSpecPlantChann(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Svr_SysGetAllUserInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到其子中间件服务器IP,端口和属性信息
  *输入字段:
  *        公司ID        FIELD_SYSTEM_COMPANY
  *返回字段：
  *        中间件ID         FIELD_SYSTEM_SET
  *        中间件IP         FIELD_SYSTEM_MW_IP
  *        中间件Port       FIELD_SYSTEM_MW_PORT
  *        中间件级别       FIELD_SYSTEM_USER_LEVEL
  #define  SERVICE_SYSTEM_GET_MW_INFO                   25028
  */
int  Svr_SysGetMWSubInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**验证是否中间件是否连通。如果连通则返回值为1。
*输入字段码：
*返回字段码：
*          状态     FIELD_SYSTEM_STATUS
#define    SERVICE_SYSTEM_CONN_STATUS                   25100
*/
int Svr_SysConnStatus(ICommInterFace * pFixComm, IDBInterFace * pDB);


/**得到指定设备的所有测点信息。
  *input param：设备名 FIELD_SYSTEM_PLANT
  *return field code：
  *             测点别名 FIELD_SYSTEM_CHANN_ALIAS
  *             测点名   FIELD_SYSTEM_CHANNEL
  *             测点类型 FIELD_SYSTEM_CHANN_TYPE
  *             报警上限 FIELD_SYSTEM_ALARM_HIGH
  *             报警下限 FIELD_SYSTEM_ALARM_LOW
  *             状态     FIELD_SYSTEM_STATUS
  *             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *define    SERVICE_SYSTEM_GET_CHANNINFO                25108
  */
int Service_SysGetChannInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定分厂的所有设备信息。
  *input param: 分厂名  FIELD_SYSTEM_FACTORY
  *return field code：
  *          设备名   FIELD_SYSTEM_PLANT
  *          采样点数 FIELD_SYSTEM_SAMPLE_NUM
  *          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
  *          数据采集设备IP  FIELD_SYSTEM_DAQIP
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_PLANTINFO                25109
  */
int Service_SysGetPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定设备的指定测点信息
  *input param：设备名 FIELD_SYSTEM_PLANT
  *             测点名 FIELD_SYSTEM_CHANN
  *return field code：
  *             测点别名 FIELD_SYSTEM_CHANN_ALIAS
  *             测点类型 FIELD_SYSTEM_CHANN_TYPE
  *             报警上限 FIELD_SYSTEM_ALARM_HIGH
  *             报警下限 FIELD_SYSTEM_ALARM_LOW
  *             状态     FIELD_SYSTEM_STATUS
  *             状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  #define    SERVICE_SYSTEM_GET_SPEC_CHANNINFO           25110
  */
int Service_SysSpecChannInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定分厂的所有设备信息。
  *input param: 分厂名  FIELD_SYSTEM_FACTORY
  *             设备名   FIELD_SYSTEM_PLANT
  *return field code：
  *          采样点数 FIELD_SYSTEM_SAMPLE_NUM
  *          采样频率 FIELD_SYSTEM_SAMPLE_FREQ
  *          数据采集设备IP  FIELD_SYSTEM_DAQIP
  *          转速     FIELD_SYSTEM_PLANT_REV
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *define    SERVICE_SYSTEM_GET_SPECPLANTINFO            25111
  */
int Service_SysGetSpecPlantInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysGetMachineInfoType(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到监测中心的信息描述表
* 输入字段码：
*返回的字段码：
*        信息类型      FIELD_SYSTEM_TYPE
*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
*/
int Service_SysGetInfoTypeDetail(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**根据测点信息类型,得到监测中心的信息描述表
* 输入字段码：
*        信息类型   FIELD_SYSTEM_TYPE
*返回的字段码：
*        索引       FIELD_SYSTEM_STATUS
*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
*        单位       FIELD_SERVICE_CHANN1_UNIT
#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
*/
int Service_SysGetInfoTypeDetailBaseType(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysModifySpecChannAlarmInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

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
int Service_SysGetSvrTemplet(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysGetCustomParam(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysGetSvrAlarmTemplet(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
*         段数                   FIELD_SYSTEM_SEG_NUM
*         是否报警               FIELD_SYSTEM_IF_ALARM
*         报警低限               FIELD_SERVICE_ALARM_LOW
*         报警高限               FIELD_SERVICE_ALARM_HIGH
*         报警组                 FIELD_SYSTEM_ALARM_GROUP
#define    SERVICE_SYSTEM_GET_SETSVRALARMPARAM      25119
*/
int Service_SysGetSvrAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysGetSvrChannAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到指定公司、分厂和装置ID的自定义参数信息
*输入参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID               FIELD_SYSTEM_SET
*         设备名                FIELD_SERVICE_PLANTID
*返回参数值：
*         公司名                 FIELD_SYSTEM_COMPANY
*         分厂名                 FIELD_SYSTEM_FACTORY
*         装置名ID                 FIELD_SYSTEM_SET
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
#define    SERVICE_SYSTEM_GET_SPECSETSVRCUSTOMPARAM       25121
*/
int Service_SysGetSpecSvrCustomParam(ICommInterFace * pFixComm, IDBInterFace * pDB);

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
int Service_SysGetSpecSvrAlarmParam(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到监测中心配置的自定义轴承参数表
#define    SERVICE_SYSTEM_GET_SETSVRCUSTOMBEARINGTBL     25123
*/
int Service_SysGetSetSvrCustomBearingTbl(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到监测中心的齿轮箱参数表
#define    SERVICE_SYSTEM_GET_SETSVRGEARBOXTBL     25124
*/
int Service_SysGetSetSvrGearBoxTbl(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到监测中心的位置参数表
#define    SERVICE_SYSTEM_GET_SETSVRLOCATIONTBL      25125
*/
int Service_SysGetSetSvrLocationTbl(ICommInterFace * pFixComm, IDBInterFace * pDB);

/**得到监测中心的部位参数表
#define    SERVICE_SYSTEM_GET_SETSVRSECTIONTBL      25126
*/
int Service_SysGetSetSvrSectionTbl(ICommInterFace * pFixComm, IDBInterFace * pDB);


/**得到监测中心的部位参数表
#define    SERVICE_SYSTEM_GET_MILLDLEWARE_VERSION      25127
*/
int Service_SysGetMiddleWareVersion(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 守护巡检 */
/* 获取各个程序的路径和版本号 */
int Service_SysGetModuleVersion(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 获取各个数据库状态 */
int Service_SysGetDBStatus(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 获取中间件运行状态 */
int Service_SysGetServerStatus(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 获取数采硬件状态 */
int Service_SysGetDAQHWStatus(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 获取数采软件状态 */
int Service_SysGetDAQSWStatus(ICommInterFace * pFixComm, IDBInterFace * pDB);

/* 获取基本的组态信息 */
int Service_SysGetBasicConfigInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

// /**更新用户信息。
// *input param 用户名		 FIELD_SYSTEM_USEID
// 			 密  码		 FIELD_SYSTEM_PASSWORD
// 			 公  司		 FIELD_SYSTEM_COMPANY
// 			 分  厂		 FIELD_SYSTEM_FACTORY
// 			 集  团		 FIELD_SYSTEM_GROUP
// 					 
// *return field code：
// 			 返回值		 FIELD_SYSTEM_STATUS
// #define    SERVICE_SYSTEM_SET_USERINFO         24071
// */

int Service_SysGetPlantOfUser( ICommInterFace * pFixComm, IDBInterFace * pDB );

int Service_UpdateUserInfo(ICommInterFace * pFixComm, IDBInterFace * pDB);

int Service_GetUserOfPlant(ICommInterFace * pFixComm, IDBInterFace * pDB);

int Service_ManageUserPlant( ICommInterFace * pFixComm, IDBInterFace * pDB );

#endif