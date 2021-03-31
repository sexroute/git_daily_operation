#ifndef ERRCODE_H_INCLUDED
#define ERRCODE_H_INCLUDED

#define SUCCESS_CODE			1
#define SUCCESS_MESG			"操作成功"

#define FAIL_CODE				-1
#define FAIL_MESG				"操作失败"

#define CORRUPT_DATA_CODE		-1001
#define	HANDLER_MISSED_CODE		-1002
#define INPARAM_MISSED_CODE		-1003

#define USERID_INVALID_CODE		-101
#define CLIENT_INVALID_CODE		-102
#define PASSWD_INVALID_CODE		-103
#define FJSXLB_INVALID_CODE		-104
#define DATA_NOT_FOUND_CODE		-105
#define DATA_ALREADY_EXIST_CODE	-106
#define DATA_REFERENCE_ERROR_CODE	-107

#define BAD_PREFIXSEG_MESG		"数据包前缀错误"
#define BAD_SERIALSEG_MESG		"数据包序列错误"
#define BAD_HEADSEG_MESG		"数据包头部错误"
#define BAD_BODYSEG_MESG		"数据包实体错误"
#define BAD_ENDSEG_MESG			"数据包尾部错误"
#define HANDLER_MISSED_MESG		"未找到对应服务程序"
#define INPARAM_MISSED_MESG		"输入参数有误"

#define USERID_INVALID_MESG		"柜员代码不存在"
#define CLIENT_INVALID_MESG		"客户号不存在"
#define PASSWD_INVALID_MESG		"密码有误"
#define FJSXLB_INVALID_MESG		"属性类别不存在"
#define DATA_NOT_FOUND_MESG		"未找到对应资料"
#define DATA_ALREADY_EXIST_MESG	"对应资料已存在"
const string DATA_REFERENCE_ERROR_MESG	= "表中存在关联数据";
#endif /* ERRCODE_H_INCLUDED */