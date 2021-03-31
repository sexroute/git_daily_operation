#pragma once
#include <additions/CTimeOutReadWriteLock.h>
class CTransactDaqData
{
public:
	CTransactDaqData(void);
	~CTransactDaqData(void);
   int ReceiveVibValue(ICommInterFace * pFixComm,IDBInterFace *pDB);

   int ReceiveVibValueForGap(ICommInterFace * pFixComm,IDBInterFace *pDB);

   /**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
     #define     SERVICE_SERVICE_ON_RECEIVEDYNVALUE        24001
     */
   int ReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   int ReceiveDynValueForGap(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**确认指定测点的报警信息
  *输入参数字段码：
  *输入参数：  公司名     FIELD_SYSTEM_COMPANY
  *            分厂名     FIELD_SYSTEM_FACTORY
  *			   装置名	  FIELD_SYSTEM_SET
  *            机组号     FIELD_SYSTEM_PLANT
  *            测点号     FIELD_SYSTEM_CHANN
  *			   确认类型	  FIELD_SERVICE_ALARM_CONFIRM_TYPE
  *            确认起始时间   FIELD_SERVICE_OFF_TIMESTART
  *	           确认结束时间   FIELD_SERVICE_OFF_TIMEEND
  *			   确认当前时间	  FIELD_SERVICE_TIME
  *			   用户名		  FIELD_SYSTEM_USEID
  *			   故障原因代码   FIELD_SERVICE_DATA_CODE
  *			   备注			  FIELD_SERVICE_REMARK
  *			   客户端IP		  FIELD_SYSTEM_CLIENT_IP
  *返回值的字段码：
  *            操作成功与否          FIELD_SERVICE_DATA_CODE 0失败 1成功
  *				#define     SERVICE_SERVICE_ON_ALARM_CONFIRM        24069
  */
   int Srv_Alarm_Confirm(ICommInterFace *pFixComm,IDBInterFace *pDB);

   int ReceiveRev(ICommInterFace *pFixComm,CCustomDB *pDB);
   int ReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB);
   int ReceiveChannState(ICommInterFace *pFixComm,CCustomDB * pDB);
   int ReceiveChannStateWithAlarmLevel(ICommInterFace *pFixComm,CCustomDB * pDB);
   int ReciveSudData(ICommInterFace *pFixComm,IDBInterFace * pDB);
   int SrvSaveData(ICommInterFace *pFixComm,IDBInterFace * pDB);
   int SrvJudgeAlarm(ICommInterFace *pFixComm,IDBInterFace * pDB);

   int GetSpecPlantLenVibData(ICommInterFace *pFixComm,CCustomDB * pDB);
   int GetSpecPlantProcData(ICommInterFace *pFixComm,CCustomDB * pDB);
   int GetSpecPlantAllState(ICommInterFace *pFixComm,CCustomDB * pDB);
   int GetSpecPlantLenDynData(ICommInterFace *pFixComm,CCustomDB * pDB);
   //同步振动实时数据
   int GetSpecPlantLengthVibData(ICommInterFace *pFixComm,CCustomDB * pDB);
   int GetSpecPlantLengthDynData(ICommInterFace *pFixComm,CCustomDB * pDB);

   /**申请阈值ID，如果数组长度小于1表示失败
   *输入参数字段码
   *           公司ID          FIELD_SYSTEM_COMPANY
   *		   分厂名          FIELD_SYSTEM_FACTORY
   *           机组ID：        FIELD_SERVICE_PLANTID
   *           通道个数        FIELD_SERVICE_CHANN_NUM
   *返回值的字段码：
   *           阈值ID数组      FIELD_SERVICE_THRESHOLD_ID
   *           数组长度（通道个数） FIELD_SERVICE_CHANN_NUM
   #define     SERVICE_SERVICE_ON_REQTHESHOLDID       24011
   */
   int ReqThresholdID(ICommInterFace *pFixComm,CCustomDB *pDB);

   /**申请指定公司、分厂、设备的快变报警事件ID
   *输入参数字段码：
   *             公司ID         FIELD_SYSTEM_COMPANY
   *			 分厂名         FIELD_SYSTEM_FACTORY
   *             机组ID：       FIELD_SERVICE_PLANTID
   *             通道类型       FIELD_SERVICE_CHANNTYPE
   *             通道索引       FIELD_SERVICE_ON_SEND_STARTID
   *             报警类型       FIELD_SERVICE_ALARM_TYPE
   *             开始时间       FIELD_SERVICE_OFF_TIMESTART
   *返回值的字段码：
   *             快变报警事件ID:   FIELD_SYSTEM_EVENT_ID
   #define     SERVICE_SERVICE_ON_REQFASTALARMEVENTID        24012
   */
   int ReqFastAlarmEventID(ICommInterFace *pFixComm,CCustomDB * pDB);

   /**更新指定公司、分厂、设备的振动测点快变报警状态
   *输入参数字段码：
   *             公司ID            FIELD_SYSTEM_COMPANY
   *			 分厂名            FIELD_SYSTEM_FACTORY
   *             机组ID：          FIELD_SERVICE_PLANTID
   *             通道索引          FIELD_SERVICE_ON_SEND_STARTID
   *             振动的报警状态    FIELD_SERVICE_ON_SEND_ALLVIBSTATE
   *             振动通道个数      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
   *返回值的字段码：
   *             成功标识            FIELD_SYSTEM_STATUS
   #define     SERVICE_SERVICE_ON_UPDATEFASTALARMSTATUS     24013
   */
   int UpdateFastAlarmStatus(ICommInterFace *pFixComm,CCustomDB *pDB);

   /**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
   *输入参数字段码：
   *            公司ID                          FIELD_SYSTEM_COMPANY
   *			 分厂名                          FIELD_SYSTEM_FACTORY
   *            机组ID                          FIELD_SERVICE_PLANTID
   *            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
   *            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
   *            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
   *            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
   *            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
   *            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
   *            时间值                          FIELD_SERVICE_TIME
   *            是否这次更新数据完成            FIELD_SERVICE_SEND_END
   *返回值的字段码：
   *            测点个数                 FIELD_SYSTEM_STATUS
   #define     SERVICE_SERVICE_ON_NETOFFRECEIVEALLVALUE        24021
   */
   int NetOffReceiveAllValue(ICommInterFace * pFixComm,IDBInterFace *pDB);

   /**发送指定机组的动态测点断网后保存的所有特征值波形数据到中间件模块
   *输入参数字段码：
   *            公司ID                 FIELD_SYSTEM_COMPANY
   *			 分厂名                 FIELD_SYSTEM_FACTORY
   *            机组ID                 FIELD_SERVICE_PLANTID
   *            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
   *            起始索引               FIELD_SERVICE_ON_SEND_STARTID
   *            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
   *            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
   *            总的波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
   *            压缩后的字节数         FIELD_SERVICE_ZIPWAVE_BYTESNUM
   *            时间值                 FIELD_SERVICE_TIME
   *            是否这次更新数据完成   FIELD_SERVICE_SEND_END
   *返回值的字段码：
   *            测点个数                FIELD_SYSTEM_STATUS
   #define     SERVICE_SERVICE_ON_NETOFFRECEIVEDYNVALUE        24022
   */
   int NetOffReceiveDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送断网后保存的ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
   *输入参数字段码：
   *                公司ID            FIELD_SYSTEM_COMPANY
   *                分厂名            FIELD_SYSTEM_FACTORY
   *                机组ID            FIELD_SERVICE_PLANTID
   *                起始索引          FIELD_SERVICE_ON_SEND_STARTID
   *                测点类型          FIELD_SERVICE_CHANNTYPE
   *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
   *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
   *                时间值            FIELD_SERVICE_TIME
   *返回值的字段码：
   *                测点个数          FIELD_SYSTEM_STATUS
   #define     SERVICE_SERVICE_ON_NETOFFRECEIVEPROC            24023
   */
   int NetOffReceiveProc(ICommInterFace *pFixComm,IDBInterFace * pDB);

   /**发送指定机组的振动测点断网后保存的所有特征值、轴位置数据到中间件模块
   *输入参数字段码：
   *            公司ID                          FIELD_SYSTEM_COMPANY
   *			 分厂名                          FIELD_SYSTEM_FACTORY
   *            机组ID                          FIELD_SERVICE_PLANTID
   *            振动测点特征值数据              FIELD_SERVICE_ON_SEND_ALLCHARVALUE
   *            起始索引                        FIELD_SERVICE_ON_SEND_STARTID
   *            振动测点个数                    FIELD_SERVICE_ON_SEND_VIBCHANNNUM
   *            波形数据                        FIELD_SERVICE_ON_SEND_VIBWAVE
   *            总的波形点数                    FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
   *            压缩后的字节数                  FIELD_SERVICE_ZIPWAVE_BYTESNUM
   *            时间值                          FIELD_SERVICE_TIME
   *            时间值                          FIELD_SERVICE_MICROSECOND
   *            是否这次更新数据完成            FIELD_SERVICE_SEND_END
   *返回值的字段码：
   *            测点个数                 FIELD_SYSTEM_STATUS
   #define     SERVICE_SERVICE_ON_NETOFFRECEIVEALL_WIRELESS_VALUE         24024
   */
   int  NetOffReceiveAllWirelessValue(ICommInterFace *pFixComm,IDBInterFace * pDB);


    /* 守护巡检*/
	/* 接收数采硬件状态  */
	int RcvDAQHWStatus(ICommInterFace *pFixComm,CCustomDB * pDB);
	/* 接收数采软件状态 */
	int RcvDAQSWStatus(ICommInterFace *pFixComm,CCustomDB * pDB);


/**启停车过程发送指定机组的振动测点实时所有振动特征值、轴位置数据到中间件模块,并且返回传送的振动通道对应的快变报警门限学习参数
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
  *            对应的时间             FIELD_SERVICE_TIME
  *            对应时间的微秒         FIELD_SERVICE_MICROSECOND
  *            启停车类型             FIELD_SERVICE_SUD_TYPE
  *            启停车标志位           FIELD_SERVICE_SUD_STATUS
  *            启停车事件ID:          FIELD_SYSTEM_EVENT_ID
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
	int ReceiveSudVibValue(ICommInterFace * pFixComm,IDBInterFace *pDB);

	/**发送指定机组的动态测点实时所有特征值波形数据到中间件模块
	*输入参数字段码：
	*            公司ID                 FIELD_SYSTEM_COMPANY
	*			       分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID                 FIELD_SERVICE_PLANTID
	*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
	*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            波形数据               FIELD_SERVICE_ON_SEND_VIBWAVE
	*            一组波形点数           FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
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
	*            是否这次更新数据完成    FIELD_SERVICE_SEND_END
	*            对应的时间             FIELD_SERVICE_TIME
	*            对应时间的微秒         FIELD_SERVICE_MICROSECOND
	*            启停车类型             FIELD_SERVICE_SUD_TYPE
	*            启停车标志位           FIELD_SERVICE_SUD_STATUS
	*            启停车事件ID:          FIELD_SYSTEM_EVENT_ID
	*返回值的字段码：
	*            测点个数                FIELD_SYSTEM_STATUS
	* #define     SERVICE_SERVICE_ON_SUD_RECEIVEDYNVALUE        24101
	*/
   int ReceiveSudDynValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**发送ALLPROC过程量或轴位移数据到中间件模块，并得到指定类型的数据
   *输入参数字段码：
   *                公司ID   FIELD_SYSTEM_COMPANY
   *                分厂名   FIELD_SYSTEM_FACTORY
   *                机组ID      FIELD_SERVICE_PLANTID
   *                起始索引          FIELD_SERVICE_ON_SEND_STARTID
   *                测点类型          FIELD_SERVICE_CHANNTYPE
   *                需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
   *                过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
   *                过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
   *返回值的字段码：如果FIELD_SERVICE_ON_SEND_CHANNTYPE字段不是为0或2，就不返回数据
   *                个数              FIELD_SERVICE_ON_SEND_AXISCHANNNUM
   *                数据              FIELD_SERVICE_ON_SEND_AXISDISPLACE
   *                对应的时间             FIELD_SERVICE_TIME
   *                对应时间的微秒         FIELD_SERVICE_MICROSECOND
   *                启停车类型             FIELD_SERVICE_SUD_TYPE
   *                启停车标志位           FIELD_SERVICE_SUD_STATUS
   *                启停车事件ID:          FIELD_SYSTEM_EVENT_ID
   *    #define     SERVICE_SERVICE_ON_RECEIVEPROC            24103
   */
   int ReceiveSudProcValue(ICommInterFace *pFixComm,IDBInterFace *pDB);


   /**发送指定机组振动快变数据到中间件，并得到报警状态
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
	*            报警状态                FIELD_SERVICE_ALARM_STATUS
	*            报警事件ID              FIELD_SYSTEM_EVENT_ID
	*            报警类型                FIELD_SERVICE_ALARM_TYPE
    #define     SERVICE_SERVICE_ON_RECEIVE_VIB_FAST_CHANGE_VALUE        24104
	*/
   int ReceiveVibFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送指定机组的动态测点实时快变数据到中间件模块，并得到报警状态和报警事件ID
	*输入参数字段码：
	*            公司ID                 FIELD_SYSTEM_COMPANY
	*			 分厂名                 FIELD_SYSTEM_FACTORY
	*            机组ID                 FIELD_SERVICE_PLANTID
	*            动态测点特征值数据     FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引               FIELD_SERVICE_ON_SEND_STARTID
	*            动态测点个数           FIELD_SERVICE_ON_SEND_VIBCHANNNUM
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
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION　
	*返回值的字段码：
	*            报警状态                FIELD_SERVICE_ALARM_STATUS
	*            报警事件ID              FIELD_SYSTEM_EVENT_ID
	*            测点个数                FIELD_SYSTEM_STATUS
	*            报警类型                FIELD_SERVICE_ALARM_TYPE
	#define     SERVICE_SERVICE_ON_RECEIVE_DYN_FAST_CHANGE_VALUE        24105
	*/
   int ReceiveDynFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   /**发送ALLPROC过程量或轴位移快变数据到中间件模块，并得到指定类型的数据
  *输入参数字段码：
  *           公司ID   FIELD_SYSTEM_COMPANY
  *			  分厂名   FIELD_SYSTEM_FACTORY
  *           机组ID      FIELD_SERVICE_PLANTID
  *           起始索引          FIELD_SERVICE_ON_SEND_STARTID
  *           测点类型          FIELD_SERVICE_CHANNTYPE
  *           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
  *           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
  *           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
  *返回值的字段码：
  *           报警状态                FIELD_SERVICE_ALARM_STATUS
  *           报警事件ID              FIELD_SYSTEM_EVENT_ID
  *            报警类型                FIELD_SERVICE_ALARM_TYPE
  #define     SERVICE_SERVICE_ON_RECEIVE_PROC_FAST_CHANGE_VALUE        24106 
  */
   int ReceiveProcFastChangeValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

   
	/**发送ALLPROC过程量或轴位移快变数据到中间件模块
	*输入参数字段码：
	*           公司ID            FIELD_SYSTEM_COMPANY
	*			  分厂名            FIELD_SYSTEM_FACTORY
	*           机组ID            FIELD_SERVICE_PLANTID
	*           起始索引          FIELD_SERVICE_ON_SEND_STARTID
	*           测点类型          FIELD_SERVICE_CHANNTYPE
	*           需要数据测点类型  FIELD_SERVICE_ON_SEND_CHANNTYPE
	*           过程量数据        FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*           过程量个数        FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*           报警事件ID        FIELD_SYSTEM_EVENT_ID
	*           对应的时间        FIELD_SERVICE_TIME
	*           转速              FIELD_SERVICE_REV
	*返回值的字段码：
	*           成功标识          FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_RECPROCFASTALARMDATA           24107
	*/
	int ReceiveProcFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**发送振动快变报警数据到中间件保存
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
	*            一组波形点数      FIELD_SERVICE_ON_SEND_WAVEPOINTNUM //对于4系列中间件为总的波形点数
	*             采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数         FIELD_SERVICE_SAMPLE_NUM
	*            自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF ///与同步助手交互新增字段
	*            自定义参数模板ID    FIELD_SYSTEM_TEMPID           ///与同步助手交互新增字段
	*            对应的时间        FIELD_SERVICE_TIME
	*            对应时间的微秒    FIELD_SERVICE_MICROSECOND
	*            是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*            压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*            间隙电压          FIELD_SERVICE_TREND_GAP  
	*            成功标识      FIELD_SYSTEM_STATUS
 
	#define     SERVICE_SERVICE_ON_RECVIBFASTALARMDATA        24108 */
	int ReceiveVibFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**发送动态快变报警数据到中间件保存
	*输入参数字段码：
	*             公司ID              FIELD_SYSTEM_COMPANY
	*			  分厂名              FIELD_SYSTEM_FACTORY
	*             机组ID：            FIELD_SERVICE_PLANTID
	*             起始索引            FIELD_SERVICE_ON_SEND_STARTID
	*             报警类型数组        FIELD_SERVICE_ALARM_TYPE
	*             动态报警事件ID      FIELD_SYSTEM_EVENT_ID
	*             特征值数据		  FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数			  FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据            FIELD_SERVICE_ON_SEND_VIBWAVE
	*             总的波形点数        FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             自定义参数数据BUFF  FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID    FIELD_SYSTEM_TEMPID
	*             对应的时间          FIELD_SERVICE_TIME
	*             对应时间的微秒      FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成   FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)     FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识      FIELD_SYSTEM_STATUS
	*             间隙电压          FIELD_SERVICE_TREND_GAP  

	#define     SERVICE_SERVICE_ON_RECDYNFASTALARMDATA          24109*/
	int ReceiveDynFastAlarmData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**发送指定机组振动无线采集数据到中间件
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
*            数据采集时间           FIELD_SERVICE_TIME
*            数据采集时间（MS）     FIELD_SERVICE_MICROSECOND
*            开停车状态             FIELD_SYSTEM_STATUS
*返回值的字段码：：
*            测点个数                FIELD_SYSTEM_STATUS
*            索引                    FIELD_SERVICE_ON_SEND_STARTID

#define     SERVICE_SERVICE_ON_RECEIVE_VIB_WIRELESS_VALUE        24115
*/
int ReceiveVibWirelessValue(ICommInterFace *pFixComm,IDBInterFace *pDB);

/**设置无线测点状态
#define   SERVICE_SERVICE_ON_SET_WIRELESSCHANN_STATUS 20046
*/
int SetWireLessChannStatus(ICommInterFace * pFixComm, IDBInterFace *pDB);

/**启停后处理数据，发送指定机组的启停车振动测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             通道个数             FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_VIB_DATA                         24210
	*/
	int ReceiveSudVibData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**启停后处理数据，发送指定机组的启停车动态测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID：             FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             启停车类型           FIELD_SERVICE_SUD_TYPE
	*             启停车事件ID         FIELD_SYSTEM_EVENT_ID
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             特征值数据           FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*             动态测点个数         FIELD_SERVICE_ON_SEND_DYNCHANNNUM
	*             波形数据             FIELD_SERVICE_ON_SEND_VIBWAVE
	*             一组波形点数         FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*             采样频率             FIELD_SERVICE_SAMPLE_FREQ
	*             采样点数             FIELD_SERVICE_SAMPLE_NUM
	*             自定义参数数据BUFF   FIELD_SERVICE_LEARNPARAM_BUFF
	*             自定义参数模板ID     FIELD_SYSTEM_TEMPID
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             是否这次更新数据完成 FIELD_SERVICE_SEND_END
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_DYN_DATA                         24211
	*/ 
	int ReceiveSudDynData(ICommInterFace *pFixComm,IDBInterFace *pDB);

	/**启停后处理数据，发送指定机组的启停车过程量测点数据到中间件
	*输入参数字段码：
	*             公司ID               FIELD_SYSTEM_COMPANY
	*             分厂名               FIELD_SYSTEM_FACTORY
	*             机组ID               FIELD_SERVICE_PLANTID
	*             起始索引             FIELD_SERVICE_ON_SEND_STARTID
	*             过程量个数           FIELD_SERVICE_ON_SEND_PROCCHANNNUM
	*             报警事件ID           FIELD_SYSTEM_EVENT_ID
	*             报警类型             FIELD_SERVICE_SUD_TYPE
	*             指定机组的转速       FIELD_SERVICE_REV
	*             指定机组的转速2      FIELD_SERVICE_REV2
	*             指定机组的转速3      FIELD_SERVICE_REV3
	*             指定机组的转速4      FIELD_SERVICE_REV4
	*             指定机组的转速5      FIELD_SERVICE_REV5
	*             过程量特征值         FIELD_SERVICE_ON_SEND_ALLPROCVALUE
	*             对应的时间           FIELD_SERVICE_TIME
	*             对应时间的微秒       FIELD_SERVICE_MICROSECOND
	*             压缩方式(0 不压缩)   FIELD_SERVICE_DATA_COMPRESSION
	*返回值的字段码：
	*             成功标识             FIELD_SYSTEM_STATUS
	#define     SERVICE_SERVICE_ON_SUD_PROC_DATA                        24212
	*/
	int ReceiveSudProcData(ICommInterFace *pFixComm,IDBInterFace *pDB);

private:
	int SaveSpecCompanyData(string sCompany_,IDBInterFace *pDB_);
	int SaveSpecFactoryData(string sCompany_,string sFactory_,IDBInterFace *pDB_);

	int JudgeSpecCompanyAlarm(string sCompany_,IDBInterFace *pDB_);
	int JudgeSpecFactoryAlarm(string sCompany_,string sFactory_,IDBInterFace *pDB_);
	///主要是报警信息
	int UpdateSpecPlantChannInfo(CRealPlantInfo *pPlantInfo_,CAlarmPlantInfo * pAlarmPlantInfo_,IDBInterFace *pDB_);
	//vector <float> _vfGapVoltage;//发送给数采的间隙电压数组
	
	CTimeOutReadWriteLock _lockUpdateChannTime;
	CBHDateTime _dtUpdateChannInfo;
	bool _bUpdateChannInfo;
};
