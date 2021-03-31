#ifndef ERRCODE_H_INCLUDED
#define ERRCODE_H_INCLUDED

#define SUCCESS_CODE			1
#define SUCCESS_MESG			"�����ɹ�"

#define FAIL_CODE				-1
#define FAIL_MESG				"����ʧ��"

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

#define BAD_PREFIXSEG_MESG		"���ݰ�ǰ׺����"
#define BAD_SERIALSEG_MESG		"���ݰ����д���"
#define BAD_HEADSEG_MESG		"���ݰ�ͷ������"
#define BAD_BODYSEG_MESG		"���ݰ�ʵ�����"
#define BAD_ENDSEG_MESG			"���ݰ�β������"
#define HANDLER_MISSED_MESG		"δ�ҵ���Ӧ�������"
#define INPARAM_MISSED_MESG		"�����������"

#define USERID_INVALID_MESG		"��Ա���벻����"
#define CLIENT_INVALID_MESG		"�ͻ��Ų�����"
#define PASSWD_INVALID_MESG		"��������"
#define FJSXLB_INVALID_MESG		"������𲻴���"
#define DATA_NOT_FOUND_MESG		"δ�ҵ���Ӧ����"
#define DATA_ALREADY_EXIST_MESG	"��Ӧ�����Ѵ���"
const string DATA_REFERENCE_ERROR_MESG	= "���д��ڹ�������";
#endif /* ERRCODE_H_INCLUDED */