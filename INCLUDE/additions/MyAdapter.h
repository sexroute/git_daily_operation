#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;

namespace Winet
{
	/// ����������
	class CAdapterHelper
	{
	public:
	    CAdapterHelper(void);
	    ~CAdapterHelper(void);
	
	    /// ˢ��������Ϣ
	    void Refresh(void);

	    /// ��������
        unsigned int GetAdapterCount(void);
	    /// ��ȡ��������
        const char * GetAdapterName(unsigned int iAdapter_);
	    /// ��ȡ����������Ϣ
        const char * GetAdapterDescription(unsigned int iAdapter_);
	    /// ��ȡ���������ַ
        DWORD64 GetPhysicalAddress(unsigned int iAdapter_);

	    /// ��ȡIP��ַ��Ŀ
	    unsigned int GetIPv4AddressCount(unsigned int iAdapter_);
	    /// ��ȡIPv4��ַ
        DWORD GetIPv4Address(unsigned int iAdapter_, unsigned int iAddr_);
	    /// ��ȡ��������
        DWORD GetIPv4SubnetMask(unsigned int iAdapter_, unsigned int iAddr_);

	    /// ��ȡ������Ŀ
	    unsigned int GetIPv4GatewayCount(unsigned int iAdapter_);
	    /// ��ȡ���ص�ַ
        DWORD GetIPv4GatewayAddress(unsigned int iAdapter_, unsigned int iGateway_);

        /// IP��ַ�ַ���ת��
        static DWORD IPv4String2Code(const char *szIPv4_);
        /// IP��ַ��ת�ַ���
        static const char * IPv4Code2String(DWORD dwIPv4_);
        /// MAC��ַ�ַ���ת��
        static DWORD64 MACString2Code(const char *szMac_);
        /// MAC��ַ��ת�ַ���
        static const char * MacCode2String(DWORD64 dw64Mac_);
		static BOOL Check();
	protected:
	    /// ������Ϣ�ṹ��
	    struct NetworkAdapter
	    {
	        string _sName;                                     ///< ��������
	        string _sDescription;                             ///< ����������Ϣ
	        DWORD64 _llMacAddr;                             ///< ���������ַ
	        vector<DWORD> _vdwIPv4Addr;              ///< IPv4��ַ�б�
	        vector<DWORD> _vdwIPv4SubnetMask;  ///< IPv4���������б�
	        vector<DWORD> _vdwIPv4Gateway;        ///< IPv4�����б�
	    };
	
	    vector<NetworkAdapter> _vecAdapters;   ///< ������Ϣ
	};
}
