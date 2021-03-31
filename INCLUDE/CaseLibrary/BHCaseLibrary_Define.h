#pragma once

#ifndef _BHCASELIBRARY_DEFINE_H
#define _BHCASELIBRARY_DEFINE_H

#include <vector>
#include "DSE_Define.h"
using namespace std;


#define COLEDATETIME_FORMAT_STR_DATETIME		_T("%Y-%m-%d %H:%M:%S")
#define COLEDATETIME_FORMAT_STR_DATETIME_BEGIN	_T("%Y-%m-%d 0:0:0")
#define COLEDATETIME_FORMAT_STR_DATETIME_END	_T("%Y-%m-%d 23:59:59")
#define COLEDATETIME_FORMAT_STR_DATE			_T("%Y-%m-%d")
#define COLEDATETIME_NULL (COleDateTime(1900,1,1,1,1,1))

// 数据库存储案例征兆重要度矩阵分隔符
#define CASE_SYMPTOM_WEIGHT_MATRIX_SPLIT (',')

/// 案例审核状态
enum E_CASE_VERIFY_STATUS
{
	CASE_VERIFY_NONE	    = -3,	// 查询时不考虑
	CASE_VERIFY_SUBMIT	    = -2,	// 未提交
	CASE_VERIFY_WAIT		= 0,	// 未审核
	CASE_VERIFY_PASS		= 1,	// 审核通过
	CASE_VERIFY_REJECT		= -1	// 审核未通过
};


/// 检维修类型
enum E_MAINTENANCE_TYPE
{
	MAINTENANCE_TYPE_SMALL	= 0,	// 小修
	MAINTENANCE_TYPE_MIDDLE	= 1,	// 中修
	MAINTENANCE_TYPE_BIG	= 2		// 大修
};

// 附件类型
enum E_ATTACHMENT_TYPE
{
	ATTACHMENT_TYPE_IMAGE	= 0,	// 图片 
	ATTACHMENT_TYPE_DOC		= 1,	// 文档
	ATTACHMENT_TYPE_GRAPH	= 2,	// 图谱
};
// 附件图谱类型
enum E_ATTACHMENT_GRAPH_TYPE
{
	ATTACHMENT_GRAPH_TYPE_WAVE	= 0,	// 波形图
	ATTACHMENT_GRAPH_TYPE_FREQ	= 1,	// 频谱图
};

// 案例查询范围
enum E_QUERY_RANGE
{
	QUERY_CURRENT_PLANT		= 0,	// 当前设备
	QUERY_CURRENT_FACTORY	= 1,    // 当前分厂
	QUERY_CURRENT_COMPANY	= 2		// 当前公司
};

/** 征兆结构体 **/
// 用途之一是基于案例推理，与中间件之间传递数据，需要POD对象
struct ST_Symptom
{
	char     _strSymptomID[50];		///< 征兆ID
	int		_nSymptomValueID;		///< 征兆值ID
	float	_fImportanceDegree;		///< 重要度
};

/** 案例征兆信息 */
struct ST_CaseSymptom 
{
	int		_nCaseID;				///< 案例ID
	CString _strSymptomID;			///< 征兆ID
	int		_nSymptomValueID;		///< 征兆值ID
	float	_fImportanceDegree;		///< 重要度

	CString _strSymptomName;		///< 征兆名称
	CString _strSymptomValueName;	///< 征兆值名称
	CString _strRemark;				///< 备注

	int     _nCaseSymptomCount;		///< 案例征兆数量
	int     _nSymptomIndex;			///< 征兆顺序索引
	vector<int>  _vRelaWeight;			///< 征兆相对重要度

	CString GetRelaWeightStr()
	{
		CString strRet = "";
		for (int i=0;i<(int)_vRelaWeight.size();i++)
		{
			CString strTemp;
			strTemp.Format("%d",_vRelaWeight.at(i));
			strRet += strTemp;
			strRet += ",";
		}
		return strRet;
	}

	CString GetErrorMsg()
	{
		if (_strSymptomID.IsEmpty())
		{
			return _T("征兆ID为空");
		}
		if (_nSymptomValueID < 0)
		{
			return _T("征兆值ID应大于等于0");
		}
		if (_fImportanceDegree < 0)
		{
			return _T("重要度应大于0");
		}
		if (_strSymptomName.IsEmpty())
		{
			return _T("征兆名称为空");
		}
		if (_strSymptomValueName.IsEmpty())
		{
			return _T("征兆值名称为空");
		}
		if (_strRemark.IsEmpty())
		{
			return _T("备注为空");
		}
		return "";
	}

	ST_CaseSymptom()
	{

	}
	CString GetImportanceDegreeStr() const
	{
		CString strRet;
		if (this->_fImportanceDegree < 0)
		{
			strRet = _T("未设置");
		}
		else
		{
			strRet.Format("%.2f",_fImportanceDegree);
		}
		
		return strRet;
	}
};
/** 案例故障信息 */
struct ST_CaseFault 
{
	int		_nCaseID;			///< 案例ID
	CString _strFaultID;		///< 故障ID
	CString _strFaultName;		///< 故障名称
	CString	_strRemark;			///< 备注

	ST_CaseFault()
	{
		_nCaseID = -1;
	}
	CString GetErrorMsg()
	{
		if (_strFaultID.IsEmpty())
		{
			return "故障ID为空";
		}
		if (_strFaultName.IsEmpty())
		{
			return "故障名称为空";
		}
		if (_strRemark.IsEmpty())
		{
			return "备注为空";
		}
		return "";
	}
};
/** 附件信息 */
class ST_Attachment 
{
public:
	CString			_strName;		///< 附件名称
	E_ATTACHMENT_TYPE _eType;		///< 附件类型
	CString			_strType;		///< 附件类型
	CString			_strFile;		///< 附件文件名
	long			_lFileSize;		///< 文件大小（单位:Byte）
	CString			_strFileSize;	///< 文件大小
	CString			_strChann;		///< 测点信息

	BYTE*			_pbFileData;	///< 文件二进制数据

	CString GetFileNameByFullPath() const
	{
		int nIndex = -1;
		while (1)
		{
			int nIndexTemp = _strFile.Find("\\",nIndex + 1);
			if(-1 == nIndexTemp)
			{
				nIndexTemp = _strFile.Find("/",nIndex + 1);
			}
			if (nIndexTemp != -1)
			{
				nIndex = nIndexTemp;
			}
			else
			{
				break;
			}
		}
		CString strRet = _strFile;
		//strRet = ::FileNameFilterIllegalChar(strRet);
		if (nIndex >= 0)
		{
			strRet = _strFile.Right(_strFile.GetLength() - nIndex -1);
		}
		return strRet;
	}
	CString GetFileExtension() const
	{
		CString strRet = "";
		for (int i=_strFile.GetLength();i>=0 && _strFile[i] != '.';i--)
		{
			CString strTemp = strRet;
			strRet = _strFile[i] + strTemp;
		}
		return strRet;
	} 
	ST_Attachment()
		: _pbFileData(NULL)
		, _lFileSize(0)

	{
	}
	~ST_Attachment()
	{
		ClearFileData();
	}
	ST_Attachment& operator=(const ST_Attachment& atta_)
	{
		if (&atta_ == this) // 检查给自己赋值的情况
		{
			return *this;
		}
		this->_lFileSize	=	atta_._lFileSize;
		this->_eType		=	atta_._eType;
		this->_strChann		=	atta_._strChann;
		this->_strFile		=	atta_._strFile;
		this->_strFileSize	=	atta_._strFileSize;
		this->_strName		=	atta_._strName;
		this->_strType		=	atta_._strType;
		this->ClearFileData();
		if (atta_._pbFileData)
		{
			this->_pbFileData = new BYTE[atta_._lFileSize];
			memcpy(this->_pbFileData,atta_._pbFileData,atta_._lFileSize);
		}
		else
		{
			this->_pbFileData = NULL;
		}
		return *this;
	}
	ST_Attachment(const ST_Attachment& atta_)
	{
		this->_lFileSize	=	atta_._lFileSize;
		this->_eType		=	atta_._eType;
		this->_strChann		=	atta_._strChann;
		this->_strFile		=	atta_._strFile;
		this->_strFileSize	=	atta_._strFileSize;
		this->_strName		=	atta_._strName;
		this->_strType		=	atta_._strType;
		if (atta_._pbFileData)
		{
			this->_pbFileData = new BYTE[atta_._lFileSize];
			memcpy(this->_pbFileData,atta_._pbFileData,atta_._lFileSize);
		}
		else
		{
			this->_pbFileData = NULL;
		}
	}
	void ClearFileData()
	{
		if (this->_pbFileData)
		{
			delete [] this->_pbFileData;
			this->_pbFileData = NULL;
		}
	}
	CString GetErrorMsg() const
	{
		CString strErr;
		if (_strName.IsEmpty())
		{
			strErr = "附件名称不能为空";
		}
		else if (_strFile.IsEmpty())
		{
			strErr = "附件文件不能为空";
		}
		return strErr;
	}
	CString GetFileSizeStr() const
	{
		CString strRet;
		float fTemp = ((float)this->_lFileSize) / 1024;
		strRet.Format("%.2fKB",fTemp);
		return strRet;
	}
	CString GetTypeStr() const
	{
		CString arrType[] = {"图片","文档","图谱"};
		return arrType[_eType];
	}
};

#define STRING_COMPANY		_T("公司名称")
#define STRING_FACTORY		_T("分厂名称")
#define STRING_PLANT_NO		_T("设备号")
#define STRING_PLANT_NAME	_T("设备名称")
#define STRING_PLANT_TYPE	_T("设备类型")


#define STRING_CASE_ID						_T("案例ID")
#define STRING_CASE_NAME					_T("案例名称")
#define STRING_CASE_DETAIL					_T("案例描述")
#define STRING_CASE_ADD_TIME				_T("案例录入时间")
#define STRING_CASE_ADD_USER				_T("录入人员")
#define STRING_CASE_TIME					_T("案例发生时间")
#define STRING_CASE_VERIFY_USER				_T("案例审核人员")
#define STRING_CASE_STATUS					_T("案例状态")
/** 案例信息结构体 **/
struct ST_Case_Info 
{
	int					_nCaseID;			///< 案例ID
	CString				_strCaseName;		///< 案例名称
	CString				_strCaseDetail;		///< 案例描述
	CString				_strCompany;		///< 公司
	CString				_strFactory;		///< 分厂
	CString				_strPlantName;		///< 设备名称
	CString				_strPlantNO;		///< 设备号
	CString				_strPlantType;		///< 设备类型
	COleDateTime		_dtAddTime;			///< 案例录入时间
	CString				_strAddUserName;	///< 录入人员
	COleDateTime		_dtCase;			///< 案例发生时间
	COleDateTime		_dtVerify;			///< 案例审核时间
	CString				_strVerifyUserName;	///< 审核人员
	E_CASE_VERIFY_STATUS _eCaseCheckStatus;	///< 审核状态

	vector<ST_CaseFault>	_vCaseFault;	///< 案例故障信息
	vector<ST_CaseSymptom>	_vCaseSymptom;	///< 案例征兆信息
	vector<ST_Attachment>	_vCaseAttachment;	///< 案例附件信息
};

/** 基于案例推理的结果 **/
struct ST_CBR_Result 
{
	ST_Case_Info	_caseInfo;		// 案例信息
	float			_fSimValue;		// 两个案例的相似度

	BOOL IsSimValueValid(float fSim_) const
	{
		return fSim_ > 0.0;
	}
	bool operator<(const ST_CBR_Result& stResult_) const
	{
		return this->_fSimValue > stResult_._fSimValue;
	}
};

/** 检维修记录信息结构体 **/
struct ST_Maintenance_Record_Info
{
	int			_nRecordID;				///< 记录ID
	CString		_strRecordName;			///< 记录名称
	CString		_strPlantName;			///< 设备名称
	CString		_strPlantNO;			///< 设备号
	CString		_strFactory;			///< 分厂名称
	CString		_strCompany;			///< 公司名称

	E_MAINTENANCE_TYPE	_eMaintenanceType;		///< 检修类型

	CString		_strSection;			///< 检修部位
	COleDateTime	_dtBegin;			///< 检修开始时间
	COleDateTime	_dtEnd;				///< 检修结束时间
	CString		_strConent;				///< 检修内容
	CString		_strFaultReason;		///< 检修故障原因描述
	CString		_strFaultID;			///< 检修故障原因ID
	CString		_strResult;				///< 检修结果
	CString		_strMainPeople;			///< 主检修员	
	CString		_strOtherPeople;		///< 其它检修人员
	CString		_strRemark;				///< 备注
	CString		_strRecorder;			///< 记录人员
	COleDateTime	_dtRecordAdd;		///< 记录添加时间

	vector<ST_Attachment>	_vAttachment; ///< 附件信息
};

struct ST_SUD_Record_Info
{
	int				_nSudRecordID;		///< 开停车记录ID
	CString			_strCompany;		///< 公司名称
	CString			_strFactory;		///< 分厂名称
	CString			_strPlantName;		///< 设备名称
	CString			_strPlantNO;		///< 设备号
	CString			_strPlantType;		///< 设备类型
	COleDateTime	_dtSudTime;			///< 开停车时间
	E_SUD_TYPE		_eSudType;			///< 开停车类型
	CString			_strSudReason;		///< 开停车原因描述
	CString			_strFaultID;		///< 故障原因ID
	CString			_strRemark;			///< 备注
	CString			_strRecorder;		///< 记录人员
};
#endif