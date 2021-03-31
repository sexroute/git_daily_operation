#if !defined(AFX_PROCESS)
#define AFX_PROCESS

//定义函数
 
/**得到指定报警ID时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             报警ID     FIELD_SERVICE_ALARMID
*返回的字段码：
*             报警通道   FIELD_SERVICE_OFF_CHANN1
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
*/
int Service_ExpertSysGetRegAlarmFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);


/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             测点ID     FIELD_SERVICE_CHANN1
*             起始时间   FIELD_SERVICE_OFF_TIMESTART
*             终止时间   FIELD_SERVICE_OFF_TIMEEND
*返回的字段码：
*             事实ID     FIELD_SERVICE_EX_RULEID     //对应为字符串
*             对应的值   FIELD_SERVICE_EX_IDVALUE    //对应为int形
#define    SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT      30002
*/
int Service_ExpertSysGetSpecChannFeatureExtract(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**得到指定振动测点、选定时间段时,专家诊断系统特征提取的知识
*输入参数：   公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*返回的字段码：
*             公司名     FIELD_SYSTEM_COMPANY
*             分厂名     FIELD_SYSTEM_FACTORY
*             机组号     FIELD_SERVICE_PLANTID
*             事实名     FIELD_SERVICE_REMARK     //对应为字符串
*#define    SERVICE_EXPERTSYS_GET_PLANTFACTNAME      30004
*/
int Service_ExpertSysGetPlantFactName(ICommInterFace *pFixComm,IDBInterFace *pDB);

#endif