#if !defined(AFX_FIXCODE_H_INCLUDED_)
#define AFX_FIXCODE_H_INCLUDED_
//-----------------------------------------------------------------------------
//            FIX 标准包头定义 
//-----------------------------------------------------------------------------
#define FIXCODE_BEGINSTRING               8  // 开始字符串          未加密，必为第一个域   
#define FIXCODE_BODYLENGTH                9  // 正文长度            未加密，必为第二个域   
#define FIXCODE_MSGTYPE                  35  // 信息类型            未加密，必为第三个域     
#define FIXCODE_SENDERCOMPID             49  // 发送人标识符        未加密       
#define FIXCODE_RECIVERCOMPID            56  // 接收人标识符        未加密   
#define FIXCODE_SECUREDATALEN            90  // 密文长度            未加密
#define FIXCODE_SECUREDATA               91  // 密文                紧随密文长度域
#define FIXCODE_MSGSEQNUM                34  // 信息序号            可加密    (未加密)
#define FIXCODE_POSSDUPFLAG              43  // 重复标志            可加密    (未加密)
#define FIXCODE_POSSRESEND               97  // 重发标志            可加密    (未加密)
#define FIXCODE_SENDINGTIME              52  // 发送时间            可加密    (未加密)
#define FIXCODE_LASTMSGSEQNUMPROCESSED  369  // 已处理的最后信息    可加密序号(未加密)
#define FIXCODE_ORIGINALSIZE            101  // 正文原长度      
#define FIXCODE_COMPRESSSIZE            102  // 正文压缩后长度      
//-----------------------------------------------------------------------------
//说明:
//    开始字符串由以下组成:
//    版本号|操作员代码|委托来源|网卡物理地址|营业部编号|目的营业部
//-----------------------------------------------------------------------------
#define FIXCODE_RECNO                     55  // 回复信息当前组号
#define FIXCODE_RETCODE                 5001  // 
#define FIXCODE_RETNOTE                 5002  //   

#endif