#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;

namespace Winet
{
	/// 网卡管理类
	class CAdapterHelper
	{
	public:
	    CAdapterHelper(void);
	    ~CAdapterHelper(void);
	
	    /// 刷新网卡信息
	    void Refresh(void);

	    /// 网卡数量
        unsigned int GetAdapterCount(void);
	    /// 获取网卡名称
        const char * GetAdapterName(unsigned int iAdapter_);
	    /// 获取网卡描述信息
        const char * GetAdapterDescription(unsigned int iAdapter_);
	    /// 获取网卡物理地址
        DWORD64 GetPhysicalAddress(unsigned int iAdapter_);

	    /// 获取IP地址数目
	    unsigned int GetIPv4AddressCount(unsigned int iAdapter_);
	    /// 获取IPv4地址
        DWORD GetIPv4Address(unsigned int iAdapter_, unsigned int iAddr_);
	    /// 获取子网掩码
        DWORD GetIPv4SubnetMask(unsigned int iAdapter_, unsigned int iAddr_);

	    /// 获取网关数目
	    unsigned int GetIPv4GatewayCount(unsigned int iAdapter_);
	    /// 获取网关地址
        DWORD GetIPv4GatewayAddress(unsigned int iAdapter_, unsigned int iGateway_);

        /// IP地址字符串转码
        static DWORD IPv4String2Code(const char *szIPv4_);
        /// IP地址码转字符串
        static const char * IPv4Code2String(DWORD dwIPv4_);
        /// MAC地址字符串转码
        static DWORD64 MACString2Code(const char *szMac_);
        /// MAC地址码转字符串
        static const char * MacCode2String(DWORD64 dw64Mac_);
		static BOOL Check();
	protected:
	    /// 网卡信息结构体
	    struct NetworkAdapter
	    {
	        string _sName;                                     ///< 网卡名称
	        string _sDescription;                             ///< 网卡描述信息
	        DWORD64 _llMacAddr;                             ///< 网卡物理地址
	        vector<DWORD> _vdwIPv4Addr;              ///< IPv4地址列表
	        vector<DWORD> _vdwIPv4SubnetMask;  ///< IPv4子网掩码列表
	        vector<DWORD> _vdwIPv4Gateway;        ///< IPv4网关列表
	    };
	
	    vector<NetworkAdapter> _vecAdapters;   ///< 网卡信息
	};
}
