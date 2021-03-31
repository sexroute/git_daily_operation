#pragma once
#include "PlantInfo.h"
/**接受数据采集器启停车数据和快变报警数据
  */
class CTranDataRecAndSend
{
public:
	CTranDataRecAndSend(void);
	~CTranDataRecAndSend(void);
private:
	/**判断是否公司、分厂、设备号有空，并通过网络连接指针返回,并得到公司别名
	*@param sCompany_ 公司名
	*@param sFactory_ 分厂名
	*@param sPlant_ 设备名
	*@param sAlias_ 是一个引用，用于把公司别名传出来
	*@param pFixComm_ 网络连接指针
	*@param bRtn_ 是否返回网络连接信息，因为如果信息不正确就直接返回了。如果返回true，否则false
	*@return 如果验证成功，则返回1，否则返回-1
	*/
	CPlantInfoOffLine * ReadAndCheckCompanyInfo(string &sCompany_,string &sFactory_,string &sPlant_,string &sAlias_,ICommInterFace *pFixComm_,bool bRtn_=true);
	CPlantInfoOffLine * ReadAndCheckCompanyInfoEx(string &sCompany_,string &sFactory_,string &sPlant_,string &sAlias_,ICommInterFace *pFixComm_,bool bRtn_=true);

public:
	/**申请指定公司、分厂、设备的启停车事件ID
	*/
	int ReqSudEventID(ICommInterFace *pFixComm,IDBInterFace *pDB);
	/**通知指定启停车事件的启停车过程结束。如果成功返回1，失败返回-1
	*/
	int NoticeSudStop(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**启停车结束后，发送指定机组的振动启停车数据到中间件
	#define     SERVICE_SERVICE_OFF_RECSUDDATA        24010
	*/
	int RecSudData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**添加一条完整的启停车日志。如果成功返回启停车事件ID，失败返回-1
	  #define     SERVICE_SERVICE_OFF_ADDWHOLESUDLOG             24053
	  */
	int AddWholeSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**@brief 发送阈值的信息、学习参数、学习数据，6种特征值一起发。返回-1表示失败，返回大于0表示成功
	*输入参数字段码
	*           公司ID          FIELD_SYSTEM_COMPANY
	*			      分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           通道类型        FIELD_SERVICE_CHANNTYPE
	*           通道索引        FIELD_SERVICE_ON_SEND_STARTID
	*           特征值个数      FIELD_SERVICE_CHANN_NUM
	*           学习数据的BUFF  FIELD_SERVICE_ON_SEND_VIBWAVE    //数据依次为不同特征值数组链接起来.数组长度=FIELD_SERVICE_LEN*FIELD_SERVICE_CHANN_NUM
	*           学习数据的长度  FIELD_SERVICE_LEN
	*           阈值的BUFF      FIELD_SERVICE_THRESHOLD_BUFF     //结构体长度为FIELD_SERVICE_CHANN_NUM
	*           学习参数的BUFF  FIELD_SERVICE_LEARNPARAM_BUFF    //结构体长度为FIELD_SERVICE_CHANN_NUM
	*           报警类型        FIELD_SERVICE_ALARM_TYPE
	*           学习数据的开始时间  FIELD_SERVICE_OFF_TIMESTART
	*           学习数据的结束时间  FIELD_SERVICE_OFF_TIMEEND
	*           触发门限学习的类型  FIELD_SYSTEM_TYPE
	*返回值的字段码
	*           成功标识            FIELD_SYSTEM_STATUS
	#define    SERVICE_SERVICE_OFF_SAVETHINFO         24014
	*/
	int SaveThInfo(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**通知指定快变报警事件的报警过程结束。如果成功返回1，失败返回-1
	*输入参数字段码：
	*             公司ID          FIELD_SYSTEM_COMPANY
	*			        分厂名          FIELD_SYSTEM_FACTORY
	*             机组ID：        FIELD_SERVICE_PLANTID
	*             通道类型        FIELD_SERVICE_CHANNTYPE
	*             通道索引        FIELD_SERVICE_ON_SEND_STARTID
	*             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
	*             结束时间        FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_NOTICEFASTALARMSTOP        24015
	*/
	int NoticeFastAlarmStop(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**发送快变报警数据到中间件保存
	*输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             快变报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速   FIELD_SERVICE_REV
	*             指定机组的转速2  FIELD_SERVICE_REV2
	*             指定机组的转速3  FIELD_SERVICE_REV3
	*             指定机组的转速4  FIELD_SERVICE_REV4
	*             指定机组的转速5  FIELD_SERVICE_REV5
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            周期起始通道索引       FIELD_SERVICE_STARTINDEX
	*            周期结束通道索引       FIELD_SERVICE_ENDINDEX
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_RECFASTALARMDATA        24016
	*/
	int RecFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**保存报警后新学习的阈值ID，如果数组长度小于1表示失败
	*输入参数字段码
	*           公司ID          FIELD_SYSTEM_COMPANY
	*			      分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           通道个数        FIELD_SERVICE_CHANN_NUM
	*           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_SAVEALAMREDNEWTHESHOLDID   24017
	*/
	int SaveAlarmedNewThID(ICommInterFace *pFixComm,CCustomDB *pDB);

	/**添加一条完整的报警日志.失败返回值小于0，成功为0
	*输入参数字段码
	*           公司ID          FIELD_SYSTEM_COMPANY
	*			分厂名          FIELD_SYSTEM_FACTORY
	*           机组ID：        FIELD_SERVICE_PLANTID
	*           日志ID          FIELD_SYSTEM_ID
	*           快变报警事件ID  FIELD_SYSTEM_EVENT_ID
	*           通道类型        FIELD_SERVICE_CHANNTYPE
	*           报警类型        FIELD_SERVICE_ALARM_TYPE
	*           开始时间        FIELD_SERVICE_OFF_TIMESTART
	*           结束时间        FIELD_SERVICE_OFF_TIMEEND
	*           阈值ID          FIELD_SERVICE_THRESHOLD_ID
	*           新阈值ID        FIELD_SERVICE_ON_SEND_STARTID
	*返回值的字段码：
	*           成功标识      FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_ADDALARMLOG                24018
	*/
	int AddAlarmLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**新增报警日志，报警开始添加。如果成功返回1，失败返回-1
	*输入参数字段码：
	*             公司ID          FIELD_SYSTEM_COMPANY
	*			        分厂名          FIELD_SYSTEM_FACTORY
	*             机组ID：        FIELD_SERVICE_PLANTID
	*             通道类型        FIELD_SERVICE_CHANNTYPE
	*             通道索引        FIELD_SERVICE_ON_SEND_STARTID
	*             快变报警事件ID  FIELD_SYSTEM_EVENT_ID
	*             起始时间        FIELD_SERVICE_OFF_TIMESTART
	*             阈值ID          FIELD_SERVICE_THRESHOLD_ID
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_ADDFASTALARMSTARTLOG        24019
	*/
	int AddFastAlarmStartLog(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**发送动态报警数据到中间件保存
	*输入参数字段码：
	*             公司ID           FIELD_SYSTEM_COMPANY
	*			  分厂名           FIELD_SYSTEM_FACTORY
	*             机组ID：         FIELD_SERVICE_PLANTID
	*             起始索引         FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组     FIELD_SERVICE_ALARM_TYPE        ///长度：FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             动态报警事件ID   FIELD_SYSTEM_EVENT_ID
	*             特征值数据       FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数         FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据         FIELD_SERVICE_ON_SEND_VIBWAVE
	*             总的波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             对应的时间        FIELD_SERVICE_TIME
	*             对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
    #define     SERVICE_SERVICE_OFF_RECDYNALARMDATA         24055
	*/
    int ReceiveDynAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 发送过程量报警数据到中间件模块
	*输入参数字段码：
	*				 公司ID 				FIELD_SYSTEM_COMPANY
	*				 分厂名 				FIELD_SYSTEM_FACTORY
	*				 机组ID 				FIELD_SERVICE_PLANTID
	*				 报警类型				FIELD_SERVICE_ALARM_TYPE
	*				 报警事件ID 			FIELD_SYSTEM_EVENT_ID
	*				 对应的时间 			FIELD_SERVICE_TIME
	*				 过程量测点号			FIELD_SYSTEM_CHANN
	*				 过程量测点类型 		FIELD_SYSTEM_CHANN_TYPE
	*				 过程量特征值			FIELD_SERVICE_CHANN1VALUE
	*				 转速					FIELD_SERVICE_REV
	*
	*返回值的字段码：
	*				 成功标识				FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_OFF_RECPROCALARMDATA           24056
	*/
	int ReceiveProcAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**请求离线巡检数据同步，返回离线测点索引和个数，及同步时间
	*输入参数字段码：
	*                公司              FIELD_SYSTEM_COMPANY
	*			     分厂名            FIELD_SYSTEM_FACTORY
	*                设备/机组         FIELD_SYSTEM_PLANTID
	*返回值的字段码：
	*                上次同步时间      FIELD_SERVICE_TIME
	*                离线测点起始索引  FIELD_SERVICE_ON_SEND_STARTID
	*                离线测点个数      FIELD_SERVICE_CHANN_NUM
	*                同步响应          FIELD_SYSTEM_STATUS
	（FIELD_SYSTEM_SUCCESS 可以同步/ FIELD_SYSTEM_FAILURE 异常，无法同步）
	*
	#define     SERVICE_SERVICE_OFF_REQUESTOFFLINESYNC
	*/
	int RequestOfflineSync(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**接收离线巡检数据
	*输入参数字段码：
	*                公司名        FIELD_SYSTEM_COMPANY
	*                分厂名        FIELD_SYSTEM_FACTORY
	*                设备号        FIELD_SERVICE_PLANTID  (如1T1, 1T2)
	*                采集时间      FIELD_SERVICE_TIME
	*                测点名        FIELD_SYSTEM_CHANN
	*                特征值        FIELD_SERVICE_OFF_DATA( float)
	*返回值的字段码：
	*                同步状态      FIELD_SYSTEM_STATUS
	                                （FIELD_SYSTEM_SUCCESS/ FIELD_SYSTEM_FAILURE）
	*
	#define     SERVICE_SERVICE_OFF_SYNC_OFFLINE_DATA            24061
	*/
	int SyncOfflineData(ICommInterFace *pFixComm,IDBInterFace *pDB);


	/**临时在线需求，读取或者设置多个正在采集的设备,e
	读取逻辑：如果没有输入（公司、分厂、设备名中任一个长度为零，或者在设备池中没有对应的设备），则返回所有设备的采集状态，如果是多个，客户端应以while(fixcom.more())的方式读取
	设置逻辑：如果有输入（公司、分厂、设备名中任一个长度均不为零，且在设备池中有对应的设备），则返回所有设备的采集状态，以及上次设置的时间，客户端应以while(fixcom.more())的方式读取
	中间件重启后以上次设置的为准，首次启动，均不采集
	*输入参数字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME
					设置状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
	*                
	*返回值的字段码：
	*                公司名         FIELD_SYSTEM_COMPANY
	*                分厂名         FIELD_SYSTEM_FACTORY
	*                设备名称       FIELD_SERVICE_PLANTID  
	*                记录时间       FIELD_SERVICE_OFF_TIME 
					返回状态		FIELD_SYSTEM_STATUS    1.表示采集 0.表示不采集
									

	#define     SERVICE_SERVICE_TEMP_SAMPLE_PLANTS 24070 
	*/
	int Service_temp_sample_plants(ICommInterFace * pFixComm,IDBInterFace *pDB);

  /**发送指定机组的振动测点实时所有特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
  *输入参数字段码：
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *			   分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            振动测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
  *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
  *            振动测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
  *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
  *            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
  *            自定义参数数据BUFF     FIELD_SERVICE_LEARNPARAM_BUF
  *            自定义参数数据BUFFSIZE FIELD_SERVICE_CHANN_BUFFSIZE
  *            采样频率               FIELD_SERVICE_SAMPLE_FREQ
  *            采样点数               FIELD_SERVICE_SAMPLE_NUM
  *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
  *            机组的转速             FIELD_SERVICE_REV
  *            机组的转速2            FIELD_SERVICE_REV2
  *            机组的转速3            FIELD_SERVICE_REV3
  *            机组的转速4            FIELD_SERVICE_REV4
  *            机组的转速5            FIELD_SERVICE_REV5
  *            周期起始通道索引       FIELD_SERVICE_STARTINDEX
  *            周期结束通道索引       FIELD_SERVICE_ENDINDEX
  *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
  *返回值的字段码：
  *            测点个数                FIELD_SYSTEM_STATUS
  *            索引                    FIELD_SERVICE_ON_SEND_STARTID
  *            快变报警学习参数BUFF    FIELD_SERVICE_LEARNPARAM_BUFF
  *            对应的间隙电压          FIELD_SERVICE_TREND_GAP
  *            这个通道是否判断快变    FIELD_SYSTEM_SENDC_IF
  *            学习参数结构体的长度    FIELD_SERVICE_LEN
  *            压缩方式(0 不压缩)      FIELD_SERVICE_DATA_COMPRESSION
  * #define     SERVICE_SERVICE_ON_SUD_RECEIVEALLVALUE        24100
  */



	int ReceiveSudVibValue(ICommInterFace * pFixComm,CCustomDB *pDB);


	/** 启停车结束后，发送指定机组的启停车振动测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_VIB_DATA                         24110
	*/
	int RecSudVibData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 启停车结束后，发送指定机组的启停车动态测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_DYN_DATA                         24111
	*/
	int RecSudDynData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/** 启停车结束后，发送指定机组的启停车过程量测点数据到中间件
	* #define     SERVICE_SERVICE_OFF_SUD_PROC_DATA                        24113
	*/
	int RecSudProcData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**启停车结束后， 同步指定公司、分厂、设备的启停车事件ID
	* #define     SERVICE_SERVICE_OFF_SUD_ADD_LOG                          24114
	*/
	int RecSudLog(ICommInterFace *pFixComm,IDBInterFace *pDB);
};

extern BOOL SyncSampleRecords();