// testMwDlg.h : 头文件
//

#pragma once

#include "cdcwgraph.h"
#include "afxwin.h"

#include <string>
#include <map>
using namespace std;

extern "C" __declspec(dllimport)  int TestFft(float *pWave,int num,double *pOut);
// CtestMwDlg 对话框
class CtestMwDlg : public CDialog
{
// 构造
public:
	CtestMwDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTMW_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CFixComm theComm;
	bool bFlag;
	afx_msg void OnBnClickedBtnTest();
	void testMiddleWare(void);
	void testGetWave(void);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
private:
	void testUsage(void);
public:
	CDCWGraph m_pGraph;
	void DrawGraph(float *fWave,int num);
	int testMidWare25003(void);
	int testMid25004(void);
	int testMidWare25005(void);
	int testSysGetSetOfUser(void);
	int testSysGetSubCorpOfUser(void);
	int testSysGetSetOfSubCorp(void);
	int testSysGetPlantOfSet(void);
	int testMidwGetAllFiveChar(void);
	int TestGetTrend(void);
	int testGetAllChann(void);
	int testGetAxesTrack(void);
	int testGetHistoryAxesTrack(void);
	int testAllAxis(void);
	int testGetAxisLocation(void);
	int test20014(void);
    int testWaterFall(void);
	afx_msg void OnBnClickedButtonDb();
	int testGetSpecTimeRecord(void);
private:
	int testModifyPwd(void);
	int testGetThreshold(void);
public:
	int testSysLogin(void);
	int testOffGetCurTrend(void);
	int testPlantIDofUser(void);
	int testGetAllValue(void);
	int testPhaseTrend(void);
	int testAxisLocationTrend(void);
	int testGetRealPhase(void);
	int testAVibChannTrend(void);
	int test21002(void);
	int test20013(void);
	void testGetiRev(void);
	int testGetSpecTimeChannSud();

	/**时间转化为时间字符串(加引号的)
	*@param time_ 时间
	*@return 返回字符串
	*/
	CString TimeConvertString(const COleDateTime &time_);
    //写日志文件
	void writeCString(CString strMsg , CString filename);
private:
	// 测试细化分析
	int TestZoom(void);
	int InsertAlarmParam();
public:
	// 得到历史数据的细化谱
	int TstHisZoomSpm(void);
	int test21015(void);
	int TestSpecPlantChannState(void);
	int TestTrendAmpAndPhase(void);
	int TestGetSpecCharAndPhase(void);
	int testGetMWinfo(void);
	int Get25010(void);
	int GetCompanyOfUser();
	//测试报警的接口
	int TestGetVibChannAlarmLog(void);

	int Test20001();
	int Test21024();
	int Test21012();
	int Test25012();
	//测试实时模块波特图数据
	int Test20018(void);

	int Test25019();
	int Test20007();

	int Test20024();
	int Test20025();
	int Test20026();

	int Test21052();


	int Test21054();
	int Test21055();
	int Test21056();
	int Test21057();
	int Test21058();

	int Test25112();
	int Test20015();
	int Test20016();

	int Test21017();
	int Test20027();
	int Test21059();
	int Test21060();
	int Test21063();

	int Test20023();
	int Test21037();
	int Test20028();
	int Test20032();

	int Test24050();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeEdit1();

public:

	int Test24030();
	int Test24031();

	int Test25029();
	int Test25030();
	int Test25031();

	int Test25032();
	int Test25033();

	int Test25034();

private:
	CComboBox m_cFixCode;
	int m_iFixCode;

	map<int, string> m_FixCodeInfo;
	map<int, int(CtestMwDlg::*)()> m_FixCodeTester;
public:
	afx_msg void OnBnClickedButtonLink();
	afx_msg void OnCbnSelchangeCombo2();
};
