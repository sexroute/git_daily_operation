// testMwDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testMw.h"
#include "testMwDlg.h"
#include <list>
#include <fstream>
#include <utility>
#include <algorithm>
#include "math.h"
#include "zipimpl.h"
#include "BHKD_utilities.h"
#include  "FixDef_Daq.h"
#include <cfloat>
#include <vector>
using namespace BHKD;




#include <MiddleWare.h>
#include <json/json.h>
#include <stdarg.h>
#include ".\testmwdlg.h"
//#include  "SyncHelperWrapper.h"

//typedef pair<string,int> PAIR_PLANTID_NUM;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

int writefile(const DATATYPE_WAVE * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("c:\\wave%d.txt",index);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//SyncHelperWrapper _g_wrapper;
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtestMwDlg 对话框


void DSEDecrypt(CString & m_sPwd, CString & m_sDePwd)
{
	char sPwd[1024]={0};
	char sDePwd[1024]={0};
	strcpy(sPwd,m_sPwd);
	char key[8]="dse";
	des3_ecb_decrypt(key,sPwd,sDePwd);
	m_sDePwd.Format("%s",sDePwd);
}

void DSEEncrypt(CString & m_sPwd, CString & m_sEnPwd)
{
	char sPwd[1024];
	char sEnPwd[1024]={0};
	strcpy(sPwd,m_sPwd);
	char key[8]="dse";
	des3_ecb_encrypt(key,sPwd,sEnPwd);
	m_sEnPwd.Format("%s",sEnPwd);
}



CtestMwDlg::CtestMwDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestMwDlg::IDD, pParent)
	, m_iFixCode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestMwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CWGRAPH1, m_pGraph);
	DDX_Control(pDX, IDC_COMBO_FIXCODE, m_cFixCode);
	DDX_Text(pDX, IDC_COMBO_FIXCODE, m_iFixCode);
}

BEGIN_MESSAGE_MAP(CtestMwDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_TEST, OnBnClickedBtnTest)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_DB, OnBnClickedButtonDb)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnBnClickedButtonLink)
END_MESSAGE_MAP()


// CtestMwDlg 消息处理程序

BOOL CtestMwDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

 	// TODO: 在此添加额外的初始化代码
	SetTimer(1,2000,0);
//	if(theComm.Link("192.168.123.18,7001/tcp","")==FALSE)
 //   theComm.SetTimeOut(5);
	COleDateTime dt1,dt2;
	dt1=COleDateTime::GetCurrentTime();


#define FIX_BIND(fixcode, tester) \
do { \
	m_FixCodeInfo.insert(make_pair(fixcode, # fixcode)); \
	m_FixCodeTester.insert(make_pair(fixcode,tester));	\
} while(0)

	FIX_BIND(SERVICE_SERVICE_ON_GETSPECFIVECHAR, Test20001);
	FIX_BIND(SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD, Test20032);
	FIX_BIND(SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS, Test24030);
	FIX_BIND(SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS, Test24031);
	FIX_BIND(SERVICE_SYSTEM_GET_MODULE_VERSION, Test25029);
	FIX_BIND(SERVICE_SYSTEM_GET_DB_STATUS,      Test25030);
	FIX_BIND(SERVICE_SYSTEM_GET_SERVER_STATUS,  Test25031);
	FIX_BIND(SERVICE_SYSTEM_GET_DAQ_HW_STATUS, Test25032);
	FIX_BIND(SERVICE_SYSTEM_GET_DAQ_SW_STATUS, Test25033);
	FIX_BIND(SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO,      Test25034);
	FIX_BIND(SERVICE_SERVICE_OFF_GETVIBCHANNSTREND,     Test21017);
	FIX_BIND(SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA,Test20023);

	int index = -1;
	for (map<int, string>::iterator it = m_FixCodeInfo.begin();
		it != m_FixCodeInfo.end();
		++it)
	{
		TRACE("%d-%s\n", it->first, it->second.c_str());

		index = m_cFixCode.AddString(getStringFormattedAs("%d-%s",
			it->first,
			it->second.c_str()).c_str());

		m_cFixCode.SetItemData(index, it->first);
	}

	OnBnClickedButtonLink();


	//index = m_cFixCode.AddString()
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CtestMwDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestMwDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

int writefile(float * ag_fwave, int ag_num,int index)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("d:\\wave%d.txt",index);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm,ios::out);
	fileWave<<"	";
	for(int i=0;i<ag_num;i++)
		fileWave<< ag_fwave[i]<<"	";
	fileWave<<"\n";
	fileWave.close();
	return ag_num;
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CtestMwDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestMwDlg::OnBnClickedBtnTest()
{
	int nSel = m_cFixCode.GetCurSel();
	if (nSel == CB_ERR) {
		return;
	}

	int(CtestMwDlg::* pTestFunc)() = NULL;

	map<int, int(CtestMwDlg::*)()>::iterator it =
		m_FixCodeTester.find(m_cFixCode.GetItemData(nSel));

	if (it == m_FixCodeTester.end()) {
		HZLOG_ERROR(STR("未找到%d对应的测试函数", m_cFixCode.GetItemData(nSel)));
		return;
	}

	pTestFunc = (int(CtestMwDlg::*)())it->second;
	if (NULL == pTestFunc) {
		return;
	}

	int nRet = (this->*pTestFunc)();
	SetDlgItemText(IDC_LABEL_FIXRET, getStringFormattedAs("%d", nRet).c_str());

//	InsertAlarmParam();
//	GetCompanyOfUser();
//	Get25010();
	//TestGetVibChannAlarmLog();
	//Test21024();
//	Test21012();
//	Test25012();
//	test20013();
//testGetMWinfo();
//	test20014();
//testUsage();
//	testMiddleWare();
//	testMidWare25003();
//    testSysLogin();
  //testGetiRev();
//	testGetAllChann();
//testGetWave();
//testAllAxis();
//TestZoom();
//	testWaterFall();
//	testGetHistoryAxesTrack();
//	testGetSpecTimeChannSud();
//	testWaterFall();
	//test21015();
//	TestSpecPlantChannState();
//	testGetAllValue();
//TestTrendAmpAndPhase();
//	TestGetTrend();
//TestGetSpecCharAndPhase();
//	testMidWare25005();

//	test21002();
//	testAVibChannTrend();
	//testGetAllValue();
//	testGetiRev();
//	TstHisZoomSpm();
//	Test20018();

	//Test25019();
	//Test20007();

//	Test20024();
//    Test20025();
//   Test20026();

//	Test21052();

	//Test21054();
	//Test21055();
//	Test21056();
//	  Test21057();
	//Test21058();

	//Test25112();
	//Test20015();
	//Test20016();


   // Test21017();
	//Test20027();
	//Test21059();
//	Test21060();
	//Test21063();
//	Test20023();
//	Test21037();

//	testGetAxesTrack();
//	testAxisLocationTrend();
//	Test20028();
//	Test24050();
}

void CtestMwDlg::testMiddleWare(void)
{
	/*
	2004-9-15 14:54:26
	*/

/*	float fcharall[8],fcharone[8],fchartwo[8],fcharthree[8],fcharhalf[8];
	int nCount=0,index=0;
	float fchar1=0,fchar2=0,fchar3=0,fchar4=0;
	COleDateTime time;
	//127.0.0.1@7001/tcp


    char chPlant[]={"10C101"};
	char chChann1[]={"1H"};
    char chChann2[]={"1T"};
	char chChann3[]={"10"};
	char chChann4[]={"聚酯一厂"};
	CString strChann;
	float fwave[2048],ffreq[1024];
	theComm.CreateHead(SERVICE_SYSTEM_GETALLCHANN);
	theComm.SetItem(FIELD_SYSTEM_PLANT,chPlant);
//	theComm.SetItem(FIELD_SYSTEM_SET,chChann3);
//	theComm.SetItem(FIELD_SYSTEM_SUBCORP,chChann4);
//	theComm.SetItem(FIELD_SERVICE_OFF_CHANNTYPE,1);
//	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,"2004-09-15 13:47:48");
	COleDateTime timetmp(2004,9,15,14,54,26);
 //  theComm.SetItem(FIELD_SERVICE_OFF_TIME,"2004-09-15 14:54:26");
   // theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChann1);
   // theComm.SetItem(FIELD_SERVICE_OFF_CHANN2,chChann2);
 	//theComm.SetItem(FIELD_SERVICE_OFF_CHANN3,chChann3);
	//theComm.SetItem(FIELD_SERVICE_OFF_CHANN4,chChann4);
	while(theComm.More())
	{
		fchar1=theComm.GetDouble(FIELD_SERVICE_TREND_OVERALLCHAR);
		fchar2=theComm.GetDouble(FIELD_SERVICE_TREND_ONECHAR);
		fchar3=theComm.GetDouble(FIELD_SERVICE_TREND_TWOCHAR);
		fchar4=theComm.GetDouble(FIELD_SERVICE_TREND_HALFCHAR);

		theComm.GetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char*)ffreq,1024*4);
		strChann=theComm.GetItem(FIELD_SYSTEM_CHANN);
		index=theComm.GetLong(FIELD_SYSTEM_CHANN_TYPE);
	//	fcharhalf[1]=theComm.GetDouble(FIELD_SERVICE_OFF_CHANN4VALUE);
	//	time=theComm.GetTime(FIELD_SERVICE_OFF_TIME);
		nCount++;
		TRACE("\n1: %f,2: %f,3: %f,4: %s;rev, %d\n",fchar1,fchar2,fchar3,strChann,index);
	//	TRACE("time:%d-%d-%d %d:%d:%d \n",time.GetYear(),time.GetMonth(),time.GetDay(),time.GetHour(),time.GetMinute(),time.GetSecond());
	//	nCount=0;
	}*/
	int nCount;//=testGetAllChann();
    AfxMessageBox("Start");
    Sleep(5000);

	for(nCount=0;nCount<200;nCount++){
//      theComm.CreateHead(SERVICE_SERVICE_ON_RECEIVEALLVIBWAVE);
      theComm.SetItem(FIELD_SERVICE_ON_SEND_PLANTID,1);
   theComm.SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,16/2);
   theComm.SetItem(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM,2048);
   float fft[8][2048];
    float ffff;
	int iloop,iloop1;
	for(iloop=0;iloop<8;iloop++)
		for(iloop1=0;iloop1<2048;iloop1++)
		{   ffff=iloop;
			fft[iloop][iloop1]=2*sin(100*2*3.14*iloop1/2048)+10*sin(10+ffff*2*3.14*iloop1/2048);
		}
//10*sin(10+iloop*2*3.14*iloop1/D_DAQ_POINTNUM);
   theComm.SetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char *)(fft),8*2048*sizeof(float));
   theComm.Write();
   theComm.Empty();
	}
	CString strtmp;
	strtmp.Format("%d",nCount);
	AfxMessageBox(strtmp);

}

void CtestMwDlg::testGetWave(void)
{
	/*          公司ID   FIELD_SYSTEM_COMPANY
		*		   分厂名   FIELD_SYSTEM_FACTORY
		*          机组ID           FIELD_SERVICE_PLANTID
		*          测点buff         FIELD_SERVICE_CHANN_BUFF
		*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
		*          测点个数         FIELD_SERVICE_CHANN_NUM*/
	theComm.CreateHead(SERVICE_SERVICE_ON_GETWAVE);

	CString strChann="2H";
	char pcChann[]={"2H"};
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"PLANT1");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
//    theComm.SetItem(FIELD_SERVICE_CHANN_BUFF,strChann);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
    int iWaveNum,iWaveNumAll;
	float *fWave1,*fWave2,*fFreqWave1,*fFreqWave2;
	float fDf;
	if(theComm.More())
	{
		iWaveNum=theComm.GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
		fDf=theComm.GetDouble(FIELD_SERVICE_FREQWAVE_DF);
		fWave1=new float[iWaveNum];
        fWave2=new float[iWaveNum];
		iWaveNumAll=theComm.GetLong(FIELD_SERVICE_FREQWAVE_NUM);
		fFreqWave1=new float[iWaveNumAll];
        fFreqWave2=new float[iWaveNumAll];
		theComm.GetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)fWave1,sizeof(float)*iWaveNum);
        theComm.GetItemBuf(FIELD_SERVICE_TIMEWAVE2,(char *)fWave2,sizeof(float)*iWaveNum);
		theComm.GetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)fFreqWave1,sizeof(float)*iWaveNumAll);
		theComm.GetItemBuf(FIELD_SERVICE_FREQWAVE2,(char *)fFreqWave2,sizeof(float)*iWaveNumAll);
//     DrawGraph(fFreqWave2,iWaveNumAll);
		//writefile(fWave1,iWaveNum,1);
		delete fWave1,fWave2,fFreqWave1,fFreqWave2;
	}
	else
	{
		AfxMessageBox("超时退出");
	}

}

void CtestMwDlg::OnDestroy()
{
	theComm.Close();
}

void CtestMwDlg::OnTimer(UINT nIDEvent)
{
   //testGetWave();
	//AfxMessageBox("begain");
   //OnBnClickedBtnTest();
	CDialog::OnTimer(nIDEvent);
}

void CtestMwDlg::testUsage(void)
{
	CSimADO *pDB=new CSimADO;
	if(!(pDB->Open("MARBLE\\PDE","mb","123","DSE")))
		AfxMessageBox("error");
	pDB->SetSqlLogPath(".\\LOG1");
	pDB->Cmd("SELECT WAVE_1V,ID FROM TREND_VIBHOUR_10C101 WHERE ID=12");
    if(pDB->More())
	{
		float ff[10]={1,2,3,4,5,6,7,8,9,0};
		//pDB->SetFieldValue("SAMPLE_NUM",1000);
	    pDB->SetBuffer("WAVE_1V",(BYTE *)ff,10*4);
	}
	AfxMessageBox("adsf");
}

void CtestMwDlg::DrawGraph(float *fWave,int num)
{
	VARIANT varx,vary;
	SAFEARRAY		*psa1;

    psa1=SafeArrayCreateVector(VT_R4,0,num);
//	psa2=SafeArrayCreateVector(VT_R4,0,1024);,*psa2
	BYTE *pByte=NULL;
	SafeArrayAccessData(psa1,(void **)&pByte);

	memcpy(pByte,fWave,1024*8);
    SafeArrayUnaccessData(psa1);
	varx.vt=VT_ARRAY|VT_R8;
	varx.parray=psa1;
    /*  SafeArrayAccessData(psa2,(void **)&pByte);
	memcpy(pByte,fWavey,1024*4);
    SafeArrayUnaccessData(psa2);

	vary.vt=VT_ARRAY|VT_R4;
	vary.parray=psa2;*/
//
	VARIANT vb,vfirst,vinc;
	vfirst.vt=VT_I4;
	vfirst.intVal=0;
	vinc.vt=VT_I4;
	vinc.intVal=1;
	vb.vt=VT_BOOL;
	vb.boolVal=1;

	m_pGraph.PlotY(varx,vfirst,vinc,vb);
	//	m_cGraph.PlotXvsY(varx,vary,vb);
//	SafeArrayDestroy(psa2);
    SafeArrayDestroy(psa1);
}

int CtestMwDlg::testMidWare25003(void)
{
	/**得到指定设备指定测点类型的所有测点名，通过循环依次得到
	*输入参数：   公司名    FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SYSTEM_PLANT
	*             测点类型   FIELD_SYSTEM_CHANN_TYPE
	*返回值的字段码：
	*             测点号     FIELD_SYSTEM_CHANN
	*             测点单位   FIELD_SERVICE_CHANN1_UNIT
	*             测点类型    FIELD_SYSTEM_CHANN_TYPE
	*             传感器类型 FIELD_SYSTEM_SENSOR_TYPE
	*             所属汽缸号 FIELD_SERVICE_CYLINDERNO
	*             信息类型   FIELD_SYSTEM_TYPE
	*             报警限类型        FIELD_SYSTEM_EVENT_TYPE
	*             报警上限          FIELD_SYSTEM_ALARM_LOW
	*             报警上上限        FIELD_SYSTEM_ALARM_HIGH
	*             报警下限          FIELD_SYSTEM_ALARM_LOW2
	*             报警下下限        FIELD_SYSTEM_ALARM_HIGH2
	*define     SERVICE_SYSTEM_GETSPECTYPECHANN           25003
	*/

	int nCount = 0,iStatus,iCylinderNo,iType;
	float fLow=.0,fHigh=.0;
	string strChann,strUnit;
	theComm.CreateHead(SERVICE_SYSTEM_GETSPECTYPECHANN);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SYSTEM_PLANT,"BH5000R");
    theComm.SetItem(FIELD_SYSTEM_CHANN_TYPE,0);
	while (theComm.More())
	{
	   strChann=theComm.GetItem(FIELD_SYSTEM_CHANN);
	   strUnit = theComm.GetItem(FIELD_SERVICE_CHANN1_UNIT);
	   iType = theComm.GetLong(FIELD_SYSTEM_TYPE);
	   iCylinderNo = theComm.GetLong(FIELD_SERVICE_CYLINDERNO);
	   fLow = theComm.GetDouble(FIELD_SYSTEM_ALARM_LOW);
	   fHigh = theComm.GetDouble(FIELD_SYSTEM_ALARM_HIGH);
	   CString sTemp;sTemp.Format("%d,%f,%f",iType,fLow,fHigh);
	   AfxMessageBox(sTemp);
	   ++nCount;
	}
	return nCount;
}

int CtestMwDlg::testMid25004(void)
{
	/**得到指定设备指定测点类型的所有测点名，通过循环依次得到
	*输入参数：   设备号    FIELD_SYSTEM_PLANT
	*             测点类型  FIELD_SYSTEM_CHANN_TYPE
	*返回值的字段码：
	*             测点号    FIELD_SYSTEM_CHANN
	*define     SERVICE_SYSTEM_GETSPECTYPECHANN           25004
	*/

	/**设置指定机组和测点的是否进行报警检查，如果状态字段输入为1，则检查报警；为0，则不检查报警
	*返回值说明：状态如果为1，说明修改成功，否则可以从状态描述中得到错误原因
	*输入参数：   机组号    FIELD_SYSTEM_PLANT
	*             测点号    FIELD_SYSTEM_CHANN
	*             状态     FIELD_SYSTEM_STATUS
	*返回值的字段码：
	*            状态     FIELD_SYSTEM_STATUS
	*            状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
	#define     SERVICE_SYSTEM_SETCHECKALARMFLAG          25018
	*/
	int nCount=0;
	int itype=1;
	char chPlant[]={"3-K1"};

	CString strChann;
//	theComm.CreateHead(SERVICE_SYSTEM_GETSPECTYPECHANN);
//	theComm.SetItem(FIELD_SYSTEM_PLANT,chPlant);
//	theComm.SetItem(FIELD_SYSTEM_CHANN_TYPE,itype);
    theComm.CreateHead(SERVICE_SYSTEM_SETCHECKALARMFLAG);
	theComm.SetItem(FIELD_SYSTEM_PLANT,"P401A");
	theComm.SetItem(FIELD_SYSTEM_CHANN,"1Vv");
	theComm.SetItem(FIELD_SYSTEM_STATUS,0);
	while(theComm.More())
	{
		nCount=theComm.GetLong(FIELD_SYSTEM_STATUS);
//		strChann=theComm.GetItem(FIELD_SYSTEM_CHANN);
//		nCount++;
//		TRACE("4: %s;\n",strChann);
	}
	return nCount;
}

int CtestMwDlg::testMidWare25005(void)
{
	/**得到指定公司名、分厂名、机组位号的所有组轴心轨迹对应的测点对，通过循环依次得到，如果没有指定设备号，则是服务器所有轴心轨迹对
	*输入参数：   公司名    FIELD_SYSTEM_COMPANY
	*             分厂名    FIELD_SYSTEM_FACTORY
	*             机组号    FIELD_SYSTEM_PLANT
	*返回值的字段码：
	*             轴心位置位置描述   FIELD_SYSTEM_AXISLOCATION
	*             垂直方向测点    FIELD_SYSTEM_CHANN_VER
	*             水平方向测点    FIELD_SYSTEM_CHANN_HOR
	*             轴心位置信息结构体 FIELD_SYSTEM_SETSVR_PLANTINFO
	*define     SERVICE_SYSTEM_GETALLAXISCHANN      25005
	*/
	int nCount=0;
	theComm.CreateHead(SERVICE_SYSTEM_GETALLAXISCHANN);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SYSTEM_PLANT,"BH5000R");
	while(theComm.More())
	{
		string strChannV=theComm.GetItem(FIELD_SYSTEM_CHANN_VER);
		string strChannH=theComm.GetItem(FIELD_SYSTEM_CHANN_HOR);
		nCount++;
		TRACE("%s,%s;\n",strChannV.c_str(),strChannH.c_str());
	}
	/**修改测点信息。注：测点名不可以修改。所有查询都通过测点别名
  *input param：设备名 FIELD_SYSTEM_PLANT
  *             测点名 FIELD_SYSTEM_CHANNEL
  *             测点类型 FIELD_SYSTEM_CHANN_TYPE
  *             报警上限 FIELD_SYSTEM_ALARM_HIGH
  *             报警下限 FIELD_SYSTEM_ALARM_LOW
  *return field code：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
 #define    SERVICE_SYSTEM_MODIFY_CHANN                 25106

    int nCount=0;
	int itype=1;
	char chUser[]={"1-GT1/1-K1"};
	char chPsw[]={"Sv1"};
	CString strChann;
	theComm.CreateHead(SERVICE_SYSTEM_MODIFY_CHANN);
	theComm.SetItem(FIELD_SYSTEM_PLANT,chUser);
	theComm.SetItem(FIELD_SYSTEM_CHANN,chPsw);
	theComm.SetItem(FIELD_SYSTEM_CHANN_TYPE,1);
    theComm.SetItem(FIELD_SYSTEM_ALARM_HIGH,15);
	theComm.SetItem(FIELD_SYSTEM_ALARM_LOW,10);
	if(theComm.More())
	{
		strChann=theComm.GetItem(FIELD_SYSTEM_PLANT);
		nCount++;
		TRACE("4: %s;\n",strChann);
	} */

	return nCount;
}

int CtestMwDlg::testSysGetSetOfUser(void)
{
	/**通过用户名和密码得到用户可操作的所有装置号，通过循环依次得到所有装置号
	*输入参数：用户名   FIELD_SYSTEM_USEID
	*          密码     FIELD_SYSTEM_PASSWORD
	*返回值的字段码：
	*          装置号   FIELD_SYSTEM_SET
	*define     SERVICE_SYSTEM_GETSETOFUSER               25006
	*/
	int nCount=0;
	char chUser[]={"mb"};
	char chPsw[]={"123"};
	CString strChann;
//	theComm.CreateHead(SERVICE_SYSTEM_GETSETOFUSER);
	theComm.SetItem(FIELD_SYSTEM_USEID,chUser);
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,chPsw);

	while(theComm.More())
	{
		strChann=theComm.GetItem(FIELD_SYSTEM_SET);
		nCount++;
		TRACE("4: %s;\n",strChann);
	}
	return nCount;
}

int CtestMwDlg::testSysGetSubCorpOfUser(void)
{
	/**通过用户名和密码得到用户可操作的分厂名，通过循环依次得到所有分厂名
	*输入参数：用户名   FIELD_SYSTEM_USEID
	*          密码     FIELD_SYSTEM_PASSWORD
	*返回值的字段码：
	*          分厂名   FIELD_SYSTEM_SUBCORP
	*define     SERVICE_SYSTEM_GETSUBCORPOFUSER               25007
	*/
	int nCount=0;
	char chUser[]={"zhangjin"};
	char chPsw[]={"000000"};
	CString strChann;
//	theComm.CreateHead(SERVICE_SYSTEM_GETSUBCORPOFUSER);
	theComm.SetItem(FIELD_SYSTEM_USEID,chUser);
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,chPsw);

	while(theComm.More())
	{
//		strChann=theComm.GetItem(FIELD_SYSTEM_SUBCORP);
		nCount++;
		TRACE("4: %s;\n",strChann);
	}
	return nCount;
}

int CtestMwDlg::testSysGetSetOfSubCorp(void)
{
	/**得到指定分厂下所有装置号，通过循环依次得到所有装置号
	*输入参数：分厂名   FIELD_SYSTEM_SUBCORP
	*返回值的字段码：
	*          装置号   FIELD_SYSTEM_SET
	*define     SERVICE_SYSTEM_GETSETOFSUBCORP            25008
	*/
	int nCount=0,index=0;
	char chChann4[]={"聚酯一厂"};
	CString strChann;
//	theComm.CreateHead(SERVICE_SYSTEM_GETSETOFSUBCORP);
//	theComm.SetItem(FIELD_SYSTEM_SUBCORP,chChann4);
	while(theComm.More())
	{
		strChann=theComm.GetItem(FIELD_SYSTEM_SET);
		nCount++;
		TRACE("\n %s",strChann);
	}
	return nCount;
}

int CtestMwDlg::testSysGetPlantOfSet(void)
{
	/**得到指定装置下所有设备号，通过循环依次得到所有设备位号
	*输入参数：装置号   FIELD_SYSTEM_SET
	*返回值的字段码：
	*          设备号   FIELD_SYSTEM_PLANT
	*define     SERVICE_SYSTEM_GETPLANTOFSET              25000
	*/
	int nCount=0;

	char chPlant[]={"10"};
	CString strChann;
	float fwave[2048],ffreq[1024];
//	theComm.CreateHead(SERVICE_SYSTEM_GETPLANTOFSET);
	theComm.SetItem(FIELD_SYSTEM_SET,chPlant);
	while(theComm.More())
	{
		strChann=theComm.GetItem(FIELD_SYSTEM_PLANT);
		nCount++;
		TRACE(" %s;rev\n",strChann);

	}
	return nCount;
}

int CtestMwDlg::testMidwGetAllFiveChar(void)
{
	/**得到所有振动测点的五种类型的特征值，通过循环依次取得各个测点的所有类型特征值，
  *每循环依次可得到一个测点的五种特征值
  *输入参数值：设备ID   FIELD_SERVICE_PLANTID
  *返回值的字段码： 通频特征值     FIELD_SERVICE_TREND_OVERALLCHAR
  *                 1X倍频特征值   FIELD_SERVICE_TREND_ONECHAR
  *                 2X倍频特征值   FIELD_SERVICE_TREND_TWOCHAR
  *                 3X倍频特征值   FIELD_SERVICE_TREND_THREECHAR
  *                 1/2倍频特征值  FIELD_SERVICE_TREND_HALFCHAR
  *#define     SERVICE_SERVICE_ON_GETALLFIVECHAR         20000
  */
    CFixComm *theComm=new CFixComm();
  	if(theComm->Link("192.168.0.1,7001/tcp","")==FALSE)
		AfxMessageBox("连接失败");
	char chPlant[]={"10C101"};
	CString strChann;
	int nCount=0;
	float fchar1,fchar2,fchar3,fchar4;
	theComm->CreateHead(SERVICE_SERVICE_ON_GETALLFIVECHAR);
	theComm->SetItem(FIELD_SERVICE_PLANTID,chPlant);
	while(theComm->More())
	{
		fchar1=theComm->GetDouble(FIELD_SERVICE_TREND_OVERALLCHAR);
		fchar2=theComm->GetDouble(FIELD_SERVICE_TREND_ONECHAR);
		fchar3=theComm->GetDouble(FIELD_SERVICE_TREND_TWOCHAR);
		fchar4=theComm->GetDouble(FIELD_SERVICE_TREND_HALFCHAR);

		nCount++;
		TRACE("\n1: %f,2: %f,3: %f,, %d\n",fchar1,fchar2,fchar3);


	}
	delete theComm;
	return nCount;

}

int CtestMwDlg::TestGetTrend(void)
{/**得到指定时间段指定趋势类型和指定特征值类型的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
     /*输入参数字段码：
		*          特征值类型  FIELD_SERVICE_TREND_CHARTYPE
		*          测点类型：  FIELD_SERVICE_OFF_CHANNTYPE
		*          公司ID      FIELD_SYSTEM_COMPANY
		*		   分厂名      FIELD_SYSTEM_FACTORY
		*          机组ID      FIELD_SERVICE_PLANTID
		*          测点buff    FIELD_SERVICE_CHANN_BUFF
		*          测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
		*          测点个数    FIELD_SERVICE_CHANN_NUM
		*          起始时间    FIELD_SERVICE_OFF_TIMESTART
		*          终止时间    FIELD_SERVICE_OFF_TIMEEND
		*返回值的字段码：
		*          各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
		*          测点个数             FIELD_SERVICE_CHANN_NUM
		*          时间值               FIELD_SERVICE_OFF_TIME
		*          测点单位             FIELD_SERVICE_CHANN1_UNIT
		SERVICE_SERVICE_OFF_GETTREND                       21003
		*/

   int nCount=0;
	CString strChann = "1H";
	float fwave[2048],ffreq[1024],ftmp;
	COleDateTime time1(2008,2,1,14,1,1);
	COleDateTime time2(2008,6,19,19,40,59);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETTREND);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"烯烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201");
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,GE_ALLCHAR);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANNTYPE,GE_ALLPROC);
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	while(theComm.More())
	{
		ftmp=theComm.GetDouble(FIELD_SERVICE_CHANN_NUM);
		nCount++;
	}
	return nCount;
}

int CtestMwDlg::testGetAllChann(void)
{
	/**得到指定设备位号的所有测点名和对应的测点类型，通过循环依次得到
  *输入参数：   设备号    FIELD_SYSTEM_PLANT
  *返回值的字段码：
  *             测点号    FIELD_SYSTEM_CHANN
  *             测点类型  FIELD_SYSTEM_CHANN_TYPE
  *define     SERVICE_SYSTEM_GETALLCHANN                25002
  */
 int nCount=0;

	char chPlant[]={"10C101"};
	char chChan[]={"1V"};
  	CString strChann;
	int itype;
	float fwave[2048],ffreq[1024];
	COleDateTime time1(2001,1,1,1,1,1);
	COleDateTime time2(2005,1,1,1,1,1);
	theComm.CreateHead(SERVICE_SYSTEM_GETALLCHANN);
	theComm.SetItem(FIELD_SYSTEM_PLANT,"PLANT1");
	while(theComm.More())
	{
		strChann=theComm.GetItem(FIELD_SYSTEM_CHANN);
//		itype=theComm.GetLong(FIELD_SYSTEM_CHANN_TYPE);
		nCount++;
		TRACE(" %s;rev\n",strChann);

	}
	return nCount;
}

int CtestMwDlg::testGetAxesTrack(void)
{
	/**得到指定机组指定一组测点轴心轨迹的波形数据。
  *输入参数：设备ID   FIELD_SERVICE_PLANTID
  *          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
  *          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
  *返回值的字段码：
  *          轴心轨迹的垂直方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
  *          轴心轨迹的水平方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
  *          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
  *define     SERVICE_SERVICE_ON_GETAXESTRACK           20008
  */

	/**得到某一组提纯轴心轨迹的波形数据。
	*输入参数：
	*          公司ID               FIELD_SYSTEM_COMPANY
	*		   分厂名               FIELD_SYSTEM_FACTORY
	*          机组ID               FIELD_SERVICE_PLANTID
	*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
	*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
	*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
	*          波形点数             FIELD_SERVICE_TIMEWAVE_NUM
	*返回值的字段码：
	*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
	*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
	*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
	*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
	*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
	*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
	#define     SERVICE_SERVICE_ON_GETPURIFYAXESTRACK           20029
	*/

	/**得到指定公司、分厂、机组、测点和时间的一组提纯轴心轨迹的波形数据。
	*输入参数：
	*          公司ID   FIELD_SYSTEM_COMPANY
	*		   分厂名   FIELD_SYSTEM_FACTORY
	*          机组ID               FIELD_SERVICE_PLANTID
	*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
	*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
	*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
	*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
	*          时间                 FIELD_SERVICE_OFF_TIME
	*返回值的字段码：
	*          垂直输出波形所包含的频率       FIELD_SERVICE_VERFREQ
	*          垂直输出波形所包含的幅值       FIELD_SERVICE_VERAMP
	*          垂直输出波形所包含的相位       FIELD_SERVICE_VERPHASE
	*          水平输出波形所包含的频率       FIELD_SERVICE_HORFREQ
	*          水平输出波形所包含的幅值       FIELD_SERVICE_HORAMP
	*          水平输出波形所包含的相位       FIELD_SERVICE_HORPHASE
	#define     SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK           21066
	*/


	int nCount=0;
	char chChan[]={"1V"};
	char chChan1[]={"1H"};
	COleDateTime time(2009,04,01,14,53,10);
	//theComm.CreateHead(SERVICE_SERVICE_OFF_GETPURIFYAXESTRACK);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETFULLSPECSTRUM);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201A");
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,chChan1);
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,chChan);
	theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,1024);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time);
    /*
	//theComm.CreateHead(SERVICE_SERVICE_ON_GETPURIFYAXESTRACK);
	theComm.CreateHead(SERVICE_SERVICE_ON_GETFULLSPECSTRUM);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201A");
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,chChan1);
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,chChan);
	theComm.SetItem(FIELD_SERVICE_TIMEWAVE_NUM,1024);*/
	typedef pair<float,float> FreqAmp;
	while(theComm.More())
	{
		++nCount;
		int iSize=theComm.GetLong(FIELD_SERVICE_CHANN_BUFFSIZE);
		vector<FreqAmp> vFullInfo;
		vFullInfo.resize(iSize);
		theComm.GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)&vFullInfo[0],iSize*sizeof(FreqAmp));
		for (int i=0;i<iSize;i++)
		{
			float a,b;
			a = vFullInfo[i].first;
			b = vFullInfo[i].second;
			TRACE(" a:%f,b:%f\n",a,b);
		}
//		itype=theComm.GetLong(FIELD_SERVICE_AXESTRACK_WAVENUM);
//		fwave=new float[itype];
//		iAllBytes=theComm.GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
//		BYTE *pByte=new BYTE[iAllBytes];
//        theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pByte,iAllBytes);
//        CZipImpl zipImpl;
//		zipImpl.UnZip(pByte,iAllBytes,(BYTE*)fwave,itype*4,UnzipBytes);
//        writefile(fwave,itype,0);
//		delete pByte;
//        iAllBytes=theComm.GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM1);
//		ffreq=new float[itype];
//		pByte=new BYTE[iAllBytes];
//		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pByte,iAllBytes);
//		zipImpl.UnZip(pByte,iAllBytes,(BYTE*)ffreq,itype*4,UnzipBytes);
//		delete pByte;
//		writefile(ffreq,itype,1);
////		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)fwave,itype*4);
////		nCount++;
//		TRACE(" %s;rev\n",strChann);

	}
	return nCount;

}

int CtestMwDlg::testAllAxis(void)
{
	int nCount=0;

	char chPlant[]={"PLANT1"};
	char chChan[]={"1V"};
	char chChan1[]={"1H"};
  	CString strChann,strChann1;
	int itype;
	float fwave[2048],ffreq[1024];

	theComm.CreateHead(SERVICE_SYSTEM_GETALLAXISCHANN);
	theComm.SetItem(FIELD_SYSTEM_PLANT,chPlant);

	while(theComm.More())
	{
        strChann=theComm.GetItem(FIELD_SYSTEM_CHANN_VER);
		strChann1=theComm.GetItem(FIELD_SYSTEM_CHANN_HOR);
		nCount++;
		TRACE(" %s %s;rev\n",strChann,strChann1);

	}
	return nCount;
}

int CtestMwDlg::testGetAxisLocation(void)
{

/**得到指定机组指定一组测点轴心位置值。
  *输入参数：设备ID               FIELD_SERVICE_PLANTID
  *          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
  *          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
  *返回值的字段码：水平方向振动测点位置  FIELD_SERVICE_AXESLOCATION_HOR
  *                垂直方向振动测点位置  FIELD_SERVICE_AXESLOCATION_VER
  *define     SERVICE_SERVICE_ON_GETAXESLOCATION        20009
  */
	int nCount=0;

	char chPlant[]={"10C101"};
	char chChan[]={"1V"};
	char chChan1[]={"1H"};
  	CString strChann,strChann1;
	int itype;
	float fwave,ffreq;

	theComm.CreateHead(SERVICE_SERVICE_ON_GETAXESLOCATION);
    theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
    theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,chChan);
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,chChan1);

	while(theComm.More())
	{
        fwave=theComm.GetLong(FIELD_SERVICE_AXESLOCATION_HOR);
		ffreq=theComm.GetLong(FIELD_SERVICE_AXESLOCATION_VER);
		nCount++;

	}
	return nCount;
}


int WriteJson()
{
	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_HW_STATUS_FILE;

	ifstream inStatusStream;
	inStatusStream.open(statusFile, ios_base::in );
	if(0 == inStatusStream.is_open()){
		HZLOG_ERROR(STR("无法打开文件, %s\n", statusFile));
		return -1;
	}

	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(inStatusStream, root);
	inStatusStream.close();

	if (!parseValid)
	{
		HZLOG_ERROR(STR("无法解析文件, %s\n", reader.getFormatedErrorMessages().c_str()));
		return -2;
	}

	std::string lastUpdateTime =  root.get(DAQ_LAST_UPDATE_TIME,
		COleDateTime::GetCurrentTime().Format(gc_cTimeFormat).GetString()).asString();
	HZLOG_DEBUG(STR("lastUpdateTime=%s\n", lastUpdateTime.c_str()));

	COleDateTime last;
	COleDateTime current;

	last.ParseDateTime(lastUpdateTime.c_str());
	if (current == last) {
		HZLOG_WARN(STR("%s的状态已写入", current.Format(gc_cTimeFormat)));
		return 0;
	}

	int cardCount = 4;

	root[FILE_DESCRIPTION] =
		"DAQ Hardware Status Record";
	root[DAQ_LAST_UPDATE_TIME] =
		COleDateTime::GetCurrentTime().Format(gc_cTimeFormat).GetString();
	root[DAQ_HW_STATUS_CARD_COUNT] = cardCount;


	Json::Value  card;
	std::string cardId;
	for(int i = 0; i <  cardCount; i++)	{
		cardId = getStringFormattedAs("card%d", i);

		card[DAQ_HW_STATUS_CARD_INDEX] = i;
		card[DAQ_HW_STATUS_CARD_NAME]  = getStringFormattedAs("card%d", i);
		card[DAQ_HW_STATUS_USED_AI_CHANNELS]  = "1,2,3";

		card[DAQ_HW_STATUS_USED_CI_CHANNELS]  = "4,6,6";
		card[DAQ_HW_STATUS_STATUS]  = -1;

		root[DAQ_HW_STATUS_CARD_INFO][cardId] = card;
	}

	cardId = getStringFormattedAs("card%d", 2);
	root[DAQ_HW_STATUS_CARD_INFO][cardId][DAQ_HW_STATUS_STATUS] = 0;

	ofstream outStatusStream;
	outStatusStream.open(statusFile, ios_base::out);

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	HZLOG_DEBUG(STR("convertedStream=\n%s\n", convertedStream.c_str()));

	outStatusStream << convertedStream;
	outStatusStream.close();

	return 0;
}


int ReadJson()
{
	CString modulePath = GetModuleFilePath();
	CString statusFile = modulePath + DAQ_HW_STATUS_FILE;

	ifstream statusStream;
	statusStream.open(statusFile, ios_base::in);

	Json::Reader reader;
	Json::Value  root;

	bool parseValid = reader.parse(statusStream, root);
	statusStream.close();

	if (!parseValid)
	{
		HZLOG_ERROR(STR("无法解析文件, %s\n", reader.getFormatedErrorMessages().c_str()));
		return -3;
	}

	Json::StyledWriter writer;
	std::string convertedStream = writer.write(root);

	HZLOG_DEBUG(STR("convertedStream=\n%s\n", convertedStream.c_str()));


	std::string lastUpdateTime =  root.get(DAQ_LAST_UPDATE_TIME,
		COleDateTime::GetCurrentTime().Format(gc_cTimeFormat).GetString()).asString();
	HZLOG_DEBUG(STR("lastUpdateTime=%s\n", lastUpdateTime.c_str()));

	return 0;
}


void CtestMwDlg::OnBnClickedButtonDb()
{
/*	CSimADO db1;
	int icount;
	CString strtmp;
	db1.Open("DSE-MARBLE\\MARBLE","sa","123","dse");
	db1.Cmd("SELECT * FROM channel");
	if(db1.More())
	{
		icount=db1.GetCount();
		while(db1.More())
		{
			if(!(db1.Move(20)))
				db1.MovePrevious();
			strtmp=db1.Get("PLANTID");
			AfxMessageBox(strtmp);
		}
	}
*/
	float ag_fwave[2]={2,3};
	int ag_num=2;
	ofstream fileWave;
    fileWave.open("c:\\wave.dat",ios::out);
    fileWave<<"	";
    for(int i=0;i<ag_num;i++)
       fileWave<< ag_fwave[i]<<"	";
    fileWave<<"\n";
    fileWave.close();

}

int CtestMwDlg::testGetSpecTimeRecord(void)
{
	/**得到指定时间的指定测点的报警记录的波形和各种特征值
  *输入参数字段码
  *          设备ID：        FIELD_SERVICE_PLANTID
  *          测点ID：        FIELD_SERVICE_OFF_CHANN1
  *          报警时间：      FIELD_SERVICE_OFF_TIME
  *          分析线数：      FIELD_SERVICE_OFF_FREQANALYSIS_LINES
  *返回值的字段码：
  *          通频特征值：    FIELD_SERVICE_TREND_OVERALLCHAR
  *          一倍频特征值：  FIELD_SERVICE_TREND_ONECHAR
  *          二倍频特征值：  FIELD_SERVICE_TREND_TWOCHAR
  *          三倍频特征值：  FIELD_SERVICE_TREND_THREECHAR
  *          1/2分频特征值： FIELD_SERVICE_TREND_HALFCHAR
  *          波形：          FIELD_SERVICE_OFF_WAVE
  *          频谱：          FIELD_SERVICE_OFF_FREQWAVE
  *          频率DF值：      FIELD_SERVICE_OFF_FREQWAVE_DF
  *          波形点数：      FIELD_SERVICE_OFF_WAVE_NUM
  *          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
  *          转速：          FIELD_SERVICE_OFF_REV
  #define    SERVICE_SERVICE_OFF_GETSPECTIMERECORD     21012
*/
	int nCount=0;

	char chPlant[]={"10C101"};
	char chChan[]={"1V"};
	char chChan1[]={"1H"};
  	CString strChann,strChann1;
	int itype;
	float fwave[2048],ffreq[1024];
	COleDateTime timetmp=COleDateTime(2004,9,15,9,13,17);

//	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECTIMERECORD);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,"2004-11-04 16:12:15");
	theComm.SetItem(FIELD_SERVICE_OFF_FREQANALYSIS_LINES,400);

	if(theComm.More())
	{
		itype=theComm.GetLong(FIELD_SERVICE_OFF_WAVE_NUM);

        theComm.GetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)fwave,itype*4);
	//	strChann1=theComm.GetItem(FIELD_SYSTEM_CHANN_HOR);
	//	nCount++;
	//	TRACE(" %s;rev\n",strChann);

	}
	return nCount;
}

int CtestMwDlg::testModifyPwd(void)
{
	/**通过用户名和密码,修改密码
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *          新密码   FIELD_SYSTEM_RESERVE_STRING
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *                 如果为-1，表示没有修改成功，为1表示修改成功
  *#define     SERVICE_SYSTEM_MODIFYPWD                  25011

	char chPlant[]={"DSE"};
	char chChan[]={"12346"};
	char chChan1[]={"123"};

	int itype;


	theComm.CreateHead(SERVICE_SYSTEM_MODIFYPWD);
	theComm.SetItem(FIELD_SYSTEM_USEID,chPlant);
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,chChan);
	theComm.SetItem(FIELD_SYSTEM_RESERVE_STRING,chChan1);

	if(theComm.More())
	{
		itype=theComm.GetLong(FIELD_SYSTEM_STATUS);
    }*/

/**通过用户名修改页面的更新时间
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  *返回值的字段码：无#define     SERVICE_SYSTEM_MODIFY_REFRESHTIME         25012

  */

/**通过用户名得到客户端页面的更新时间
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *返回值的字段码：
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  #define     SERVICE_SYSTEM_GET_REFRESHTIME            25013
 */
    char chPlant[]={"DSE"};
	char chChan[]={"12346"};
	char chChan1[]={"123"};

	int itype;


	theComm.CreateHead(SERVICE_SYSTEM_GET_REFRESHTIME);
	theComm.SetItem(FIELD_SYSTEM_USEID,chPlant);
	theComm.SetItem(FIELD_SYSTEM_REFRESHTIME,10);


	if(theComm.More())
	{
		itype=theComm.GetLong(FIELD_SYSTEM_REFRESHTIME);
	}

	return 0;
}

int CtestMwDlg::testGetThreshold(void)
{

/**得到指定设备指定测点类型的所有测点名和对应的报警上下限，通过循环依次得到
  *输入参数：   设备号    FIELD_SYSTEM_PLANT
  *             测点类型  FIELD_SYSTEM_CHANN_TYPE
  *返回值的字段码：
  *             测点号    FIELD_SYSTEM_CHANN
  *             报警上限  FIELD_SYSTEM_ALARM_HIGH
  *             报警下限  FIELD_SYSTEM_ALARM_LOW
#define     SERVICE_SYSTEM_GETALARMTHRESHOLD           25014
  */
    int nCount=0;

	char chPlant[]={"10C101"};
	CString strChann1;
	int itype=1;
	float fwave,ffreq;

	theComm.CreateHead(SERVICE_SYSTEM_GETALARMTHRESHOLD);
	theComm.SetItem(FIELD_SYSTEM_PLANT,chPlant);
	theComm.SetItem(FIELD_SYSTEM_CHANN_TYPE,itype);


	while(theComm.More())
	{
		fwave=theComm.GetDouble(FIELD_SYSTEM_ALARM_LOW);
		ffreq=theComm.GetDouble(FIELD_SYSTEM_ALARM_HIGH);
    	strChann1=theComm.GetItem(FIELD_SYSTEM_CHANN);
	  nCount++;

	}
	return nCount;
}

int CtestMwDlg::testSysLogin(void)
{/**通过用户名和密码登陆，如果登陆成功。如果用户登录成功，则状态为1，并返回用户级别和刷新时间；否则为-1，并且不返回用户级别和刷新时间。
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          状态     FIELD_SYSTEM_STATUS
  *          状态描述 FIELD_SYSTEM_STATUS_DISCRIPTION
  *          用户级别 FIELD_SYSTEM_USER_LEVEL
  *          刷新时间 FIELD_SYSTEM_REFRESHTIME
  *          浏览装置 FIELD_SYSTEM_BROWSE_SET （如果为1，则浏览装置，否则不浏览）
  #define     SERVICE_SYSTEM_LOGIN                      25015
  */
	int nCount=0;
	int itype=0;
    int iTime;
	theComm.CreateHead(SERVICE_SYSTEM_LOGIN);
	theComm.SetItem(FIELD_SYSTEM_USEID,"1");
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,"1");
	while(theComm.More())
	{
		itype = theComm.GetLong(FIELD_SYSTEM_STATUS);
		iTime=theComm.GetLong(FIELD_SYSTEM_REFRESHTIME);
		CString sTemp;sTemp.Format("%d,%d",itype,iTime);
	    nCount++;

	}
	return nCount;
}

int CtestMwDlg::testOffGetCurTrend(void)
{
	/**得到当前时间从向后推一个趋势类型的趋势数据，通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
*@param：  输入参数字段码
*          趋势类型    FIELD_SERVICE_OFF_TRENDTYPE
*          特征值类型  FIELD_SERVICE_TREND_CHARTYPE
*          测点类型：  FIELD_SERVICE_OFF_CHANNTYPE
*          设备ID      FIELD_SERVICE_PLANTID
*          测点ID1     FIELD_SERVICE_OFF_CHANN1
*          测点ID2     FIELD_SERVICE_OFF_CHANN2
*          测点ID3     FIELD_SERVICE_OFF_CHANN3
*          测点ID4     FIELD_SERVICE_OFF_CHANN4
*@return  返回值的字段码：
*          测点ID1值   FIELD_SERVICE_OFF_CHANN1VALUE
*          测点ID2值   FIELD_SERVICE_OFF_CHANN2VALUE
*          测点ID3值   FIELD_SERVICE_OFF_CHANN3VALUE
*          测点ID4值   FIELD_SERVICE_OFF_CHANN4VALUE
*          时间值      FIELD_SERVICE_OFF_TIME
*define     SERVICE_SERVICE_OFF_GETCURTREND           21001
*/

	int nCount=0;

	char chPlant[]={"10C101"};
	CString strChann1;
	int itype=1;
	float fwave,ffreq;

	theComm.CreateHead(SERVICE_SERVICE_OFF_GETCURTREND);
	theComm.SetItem(FIELD_SERVICE_OFF_TRENDTYPE,0);
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,0);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANNTYPE,1);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"1V");


	while(theComm.More())
	{
		itype=theComm.GetDouble(FIELD_SERVICE_OFF_CHANN1VALUE);

	  nCount++;

	}
	return nCount;

}

int CtestMwDlg::testPlantIDofUser(void)
{/**通过用户名和密码得到用户可操作的所有设备，通过循环依次得到所有设备位号
  *输入参数：用户名   FIELD_SYSTEM_USEID
  *          密码     FIELD_SYSTEM_PASSWORD
  *返回值的字段码：
  *          设备号   FIELD_SYSTEM_PLANT
  *define     SERVICE_SYSTEM_GETPLANTOFUSER             25005
  */int nCount=0;

	char chPlant[]={"10C101"};
	CString strChann1;
	int itype=1;
	float fwave,ffreq;

//	theComm.CreateHead(SERVICE_SYSTEM_GETPLANTOFUSER);
	theComm.SetItem(FIELD_SYSTEM_USEID,"dse");
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,"dsedse");



	while(theComm.More())
	{
		strChann1=theComm.GetItem(FIELD_SYSTEM_PLANT);

	    nCount++;

	}
	return nCount;

}

int CtestMwDlg::testGetAllValue(void)
{/**得到指定机组所有测点特征值，通过循环依次得到所有值。注：对于振动测点来说，指定那种特征类型就给那一种
 *输入参数值：设备ID         FIELD_SERVICE_PLANTID
 *            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
 *            机组2ID         FIELD_SERVICE_CHANN2
 *            机组3ID         FIELD_SERVICE_CHANN3
 *            机组4ID         FIELD_SERVICE_CHANN4
 *返回值的字段码：
 *            设备ID         FIELD_SERVICE_PLANTID
 *            测点ID         FIELD_SERVICE_CHANN1
 *            测点值         FIELD_SERVICE_CHANN1VALUE
 *            测点单位       FIELD_SERVICE_CHANN1_UNIT
 *            测点所属列表   FIELD_SERVICE_CHANN1_LIST
 *            测点的状态     FIELD_SERVICE_CHANN1_STATUS
 #define     SERVICE_SERVICE_ON_GETALLCHANNVALUE       20014
 */
	int nCount=0;

	char chPlant[]={"P401A"};
	CString strChann1,strPlant;
	int itype=1;
	float fwave,ffreq;

//	theComm.CreateHead(SERVICE_SERVICE_ON_GETALLCHANNVALUE);
	theComm.CreateHead(SERVICE_SERVICE_ON_GETREV);
	theComm.SetItem(FIELD_SERVICE_PLANTID,"P401B");//"1-GT1/1-K1");//
	theComm.SetItem(FIELD_SERVICE_CHANN2,"P4011A");
//	theComm.SetItem(FIELD_SERVICE_CHANN3,"K200");
//	theComm.SetItem(FIELD_SERVICE_CHANN4,"K201A");
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,0);

	while(theComm.More())
	{
      /*  strPlant=theComm.GetItem(FIELD_SERVICE_PLANTID);
		strChann1=theComm.GetItem(FIELD_SERVICE_CHANN1);
		fwave=theComm.GetDouble(FIELD_SERVICE_CHANN1VALUE);
		itype=theComm.GetLong(FIELD_SERVICE_CHANN1_STATUS);*/
		itype=theComm.GetLong(FIELD_SERVICE_REV);
		itype=theComm.GetLong(FIELD_SERVICE_REV2);
		strPlant=theComm.GetItem(FIELD_SERVICE_PLANTID);
        //TRACE("%s,%s,%.2f\n",strPlant,strChann1,fwave);
	    nCount++;

	}
	/**得到指定过程量测点的值。
  *输入参数：机组ID           FIELD_SERVICE_PLANTID
  *          测点ID           FIELD_SERVICE_CHANN1
  *返回值的字段码：
  *          过程量测点值     FIELD_SERVICE_TREND_PROCESS
 #define     SERVICE_SERVICE_ON_GETSPECPROCVALUE       20006

int nCount=0;

	char chPlant[]={"1-GT1/1-K1"};
	CString strChann1="6A";
	int itype=1;


	theComm.CreateHead(SERVICE_SERVICE_ON_GETSPECPROCVALUE);
	theComm.SetItem(FIELD_SERVICE_PLANTID,"3-K1");//"1-GT1/1-K1");//
	theComm.SetItem(FIELD_SERVICE_CHANN1,strChann1);

	while(theComm.More())
	{
//		strChann1=theComm.GetItem(FIELD_SERVICE_CHANN1);
	   float fwave=theComm.GetDouble(FIELD_SERVICE_TREND_PROCESS);
//		itype=theComm.GetLong(FIELD_SERVICE_CHANN1_STATUS);
        TRACE("%.2f\n",fwave);
	    nCount++;

	} */
	return nCount;

}

int CtestMwDlg::testPhaseTrend(void)
{/**得到指定机组ID、振动测点ID以及时间段的历史相位数据
  *输入参数字段码：
  *          机组ID      FIELD_SERVICE_PLANTID
  *          测点ID1     FIELD_SERVICE_OFF_CHANN1
  *          测点ID2     FIELD_SERVICE_OFF_CHANN2
  *          测点ID3     FIELD_SERVICE_OFF_CHANN3
  *          测点ID4     FIELD_SERVICE_OFF_CHANN4
  *          起始时间    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间    FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  *          测点ID1值   FIELD_SERVICE_OFF_CHANN1VALUE
  *          测点ID2值   FIELD_SERVICE_OFF_CHANN2VALUE
  *          测点ID3值   FIELD_SERVICE_OFF_CHANN3VALUE
  *          测点ID4值   FIELD_SERVICE_OFF_CHANN4VALUE
  *          时间值      FIELD_SERVICE_OFF_TIME
#define     SERVICE_SERVICE_OFF_GETPHASETREND         21006
  */
	int nCount=0;

	CString strChann1;
	float itype=1;
	float fwave,ffreq;

//	theComm.CreateHead(SERVICE_SERVICE_OFF_GETPHASETREND);
	theComm.SetItem(FIELD_SERVICE_PLANTID,"1-GT1/1-K1");
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"5V");
    COleDateTime time1(2005,2,28,10,1,1);
	COleDateTime time2(2005,2,28,16,50,1);

	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);

	while(theComm.More())
	{
		itype=theComm.GetDouble(FIELD_SERVICE_OFF_CHANN1VALUE);

	  nCount++;

	}
	return nCount;
}

int CtestMwDlg::testAxisLocationTrend(void)
{
	/**得到指定机组ID、时间段、水平测点ID和垂直测点ID的轴心位置历史数据，通过循环依次得到所有数据和对应时间，每次取得数据对应时间都是相同的
	*输入参数字段码：
	*          公司ID   FIELD_SYSTEM_COMPANY
	*   	   分厂名   FIELD_SYSTEM_FACTORY
	*          机组ID   FIELD_SERVICE_PLANTID
	*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
	*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
	*          轴承位置描述         FIELD_SYSTEM_AXISLOCATION
	*          起始时间    FIELD_SERVICE_OFF_TIMESTART
	*          终止时间    FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*          水平方向振动测点数据  FIELD_SERVICE_AXESLOCATION_HOR
	*          垂直方向振动测点数据  FIELD_SERVICE_AXESLOCATION_VER
	*          时间值                FIELD_SERVICE_OFF_TIME
	#define     SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND  21005
	*/
	int nCount=0;
	float fwave,ffreq;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETAXISLOCATIONTREND);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201I");
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,"1H");
    theComm.SetItem(FIELD_SERVICE_CHANNID_VER,"1V");
	COleDateTime time1(2009,3,1,10,1,1);
	COleDateTime time2(2009,3,24,8,50,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	while(theComm.More())
	{
		ffreq=theComm.GetDouble(FIELD_SERVICE_AXESLOCATION_HOR);
		fwave=theComm.GetDouble(FIELD_SERVICE_AXESLOCATION_VER);
	    nCount++;
	}
	return nCount;
}

int CtestMwDlg::Test20016(void)
{
	/**得到指定任意多个沉降量测点的实时波形、频谱和所有特征值。（测点之间“，”分开）。如果多个测点，就循环提取
	*输入参数：
	*          公司ID   FIELD_SYSTEM_COMPANY
	*		   分厂名   FIELD_SYSTEM_FACTORY
	*          机组ID           FIELD_SERVICE_PLANTID
	*          测点buff         FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数         FIELD_SERVICE_CHANN_NUM
	*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
	*          分析线数         FIELD_SERVICE_ZOOM_LINES
	*返回值的字段码：
	*          测点名                  FIELD_SERVICE_CHANN1
	*          测点别名                FIELD_SYSTEM_CHANN_ALIAS
	*          测点的波形数据          FIELD_SERVICE_TIMEWAVE1
	*          测点的频谱数据          FIELD_SERVICE_FREQWAVE1
	*          测点波形数据的点数      FIELD_SERVICE_TIMEWAVE_NUM
	*          测点频谱数据的点数      FIELD_SERVICE_FREQWAVE_NUM
	*          振动测点频谱DF          FIELD_SERVICE_FREQWAVE_DF
	*          所有特征值              FIELD_SERVICE_CHARVALUE_S
	*          报警值                  FIELD_SYSTEM_ALARM_LOW
	*          危险值                  FIELD_SYSTEM_ALARM_HIGH
	*          单位                    FIELD_SERVICE_CHANN1_UNIT
	*          指定机组测点的转速       FIELD_SERVICE_REV
	*          时间                    FIELD_SERVICE_TIME
	*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
	*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
	*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
	*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
	*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
	*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
	*          测点起始索引                          FIELD_SERVICE_STARTINDEX
	*          测点结束索引                          FIELD_SERVICE_ENDINDEX
	#define     SERVICE_SERVICE_ON_GETRODSINKDATA       20016
	*/
	int nCount = 0,iStatus=0;
	CString strChann="1缸杆偏摆量X,1缸杆沉降量Y";
	theComm.CreateHead(SERVICE_SERVICE_ON_GETRODSINKDATA);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,2);
	theComm.SetItem(FIELD_SERVICE_TIMEWAVE_NUM,2048);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,800);
	while(theComm.More())
	{
		float fAlarmLow=theComm.GetDouble(FIELD_SERVICE_ALARM_LOW);
		float fAlarmHigh=theComm.GetDouble(FIELD_SERVICE_ALARM_HIGH);
		int iStartIndex=theComm.GetLong(FIELD_SERVICE_STARTINDEX);
		int iEndIndex=theComm.GetLong(FIELD_SERVICE_ENDINDEX);
		float df=theComm.GetDouble(FIELD_SERVICE_FREQWAVE_DF);
		float fValue[GD_CUSTOMPARAM_BUFFSIZE];
		theComm.GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)fValue,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			CString sTemp;sTemp.Format("接受:fValue[%d]:%f\n",i,fValue[i]);TRACE(sTemp);
		}
		nCount++;
		CString sTemp;sTemp.Format("df:%f,fAlarmLow:%f,fAlarmHigh:%f,iStartIndex:%d,iEndIndex:%d\n",df,fAlarmLow,fAlarmHigh,iStartIndex,iEndIndex);TRACE(sTemp);
	}
	return nCount;
}

int CtestMwDlg::testAVibChannTrend(void)
{/**得到指定机组号、振动测点号和指定起始时间段的趋势数据
  *输入参数字段码：
  *          机组ID      FIELD_SERVICE_PLANTID
  *          测点ID      FIELD_SERVICE_OFF_CHANN1
  *          起始时间    FIELD_SERVICE_OFF_TIMESTART
  *          终止时间    FIELD_SERVICE_OFF_TIMEEND
  *返回值的字段码：
  * 		 测点（通频）值   FIELD_SERVICE_TREND_OVERALLCHAR
  *          测点（1/2X）值   FIELD_SERVICE_TREND_HALFCHAR
  *          测点（1X）值     FIELD_SERVICE_TREND_ONECHAR
  *          测点（2X）值     FIELD_SERVICE_TREND_TWOCHAR
  *          测点（3X）值     FIELD_SERVICE_TREND_THREECHAR
  *          时间             FIELD_SERVICE_OFF_TIME
  #define    SERVICE_SERVICE_OFF_GETAVIBCHANNTREND      21004
  */
	int nCount=0;

	char chPlant[]={"PLANT1"};
	char chChan[]={"1V"};
	char chChan1[]={"5H"};
	float itype;

	theComm.CreateHead(SERVICE_SERVICE_OFF_GETAVIBCHANNTREND);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan);
	COleDateTime time1(2006,6,1,10,1,1);//2006-06-15 15:52:56
	COleDateTime time2(2006,6,26,10,50,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	while(theComm.More())
	{
		itype=theComm.GetDouble(FIELD_SERVICE_TREND_OVERALLCHAR);
		itype=theComm.GetDouble(FIELD_SERVICE_TREND_HALFCHAR);
		nCount++;

	}
	return nCount;
}

int CtestMwDlg::test21002(void)
{
	/**得到指定公司、分厂机组和测点指定时刻的频谱数据
	*输入参数：
	*          公司ID       FIELD_SYSTEM_COMPANY
	*	    分厂名       FIELD_SYSTEM_FACTORY
	*          机组ID       FIELD_SERVICE_PLANTID
	*          测点ID       FIELD_SERVICE_OFF_CHANN1
	*          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
	*          分析线数：   FIELD_SERVICE_OFF_FREQANALYSIS_LINES
	*          时间         FIELD_SERVICE_OFF_TIME
	*          起始频率     FIELD_SERVICE_ZOOM_STARTFREQ
	*          截止频率     FIELD_SERVICE_ZOOM_STOPFREQ
	*返回值的字段码：
	*          波形数据        FIELD_SERVICE_OFF_WAVE
	*          频谱数据        FIELD_SERVICE_OFF_FREQWAVE
	*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
	*          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
	*          频谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
	*          频谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
	*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
	*          报警值          FIELD_SYSTEM_ALARM_LOW
	*          危险值          FIELD_SYSTEM_ALARM_HIGH
	*          单位            FIELD_SERVICE_CHANN1_UNIT
	*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
	*          转速            FIELD_SERVICE_OFF_REV
	#define     SERVICE_SERVICE_OFF_GETSPECFREQWAVE       21002
	*/
	/**得到指定公司、分厂机组和测点指定时刻的倒谱数据
	*输入参数：
	*          公司ID       FIELD_SYSTEM_COMPANY
	*	       分厂名       FIELD_SYSTEM_FACTORY
	*          机组ID       FIELD_SERVICE_PLANTID
	*          测点ID       FIELD_SERVICE_OFF_CHANN1
	*          波形数据个数 FIELD_SERVICE_OFF_WAVE_NUM
	*          时间         FIELD_SERVICE_OFF_TIME
	*返回值的字段码：
	*          波形数据        FIELD_SERVICE_OFF_WAVE
	*          倒谱数据        FIELD_SERVICE_OFF_FREQWAVE
	*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
	*          倒谱DF          FIELD_SERVICE_OFF_FREQWAVE_DF
	*          倒谱数据点数    FIELD_SERVICE_OFF_FREQWAVE_NUM
	*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
	*          报警值          FIELD_SYSTEM_ALARM_LOW
	*          危险值          FIELD_SYSTEM_ALARM_HIGH
	*          单位            FIELD_SERVICE_CHANN1_UNIT
	*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
	*          转速            FIELD_SERVICE_OFF_REV
	*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
	*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
	*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
	*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM       21067
	*/

	int nCount=0,iWaveNum,iFreqNum;
	float itype;
    float fWave[5024],fFreq[5024],fDf;
	COleDateTime time(2009,07,21,10,42,32);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECFREQWAVE);
	//theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECCHANNCEPSTRUM);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"1缸壳体振动V");
    theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,12000);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time);
	while(theComm.More())
	{
		S_VibCharValue vibValue;
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibValue,sizeof(S_VibCharValue));
        iWaveNum=theComm.GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
		theComm.GetItemBuf(FIELD_SERVICE_OFF_WAVE,(char *)fWave,sizeof(float)*iWaveNum);
		iFreqNum=theComm.GetLong(FIELD_SERVICE_OFF_FREQWAVE_NUM);
		theComm.GetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char *)fFreq,sizeof(float)*iFreqNum);
		fDf=theComm.GetLong(FIELD_SERVICE_OFF_FREQWAVE_DF);
		float *fValue=new float[GD_CUSTOMPARAM_BUFFSIZE];
		theComm.GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)fValue,50*sizeof(float));
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			CString sTemp;sTemp.Format("接受:fValue[%d]:%f\n",i,fValue[i]);TRACE(sTemp);
		}
		nCount++;
		KD_DeletePoint(fValue);
	}
	return nCount;
}

void CtestMwDlg::testGetiRev(void)
{
	/**得到指定机组机组转速。如果指定机组只有一个转速，则只取FIELD_SERVICE_OFF_REV的值
  *输入参数：机组ID          FIELD_SERVICE_PLANTID
  *返回值的字段码：
  *          指定机组的转速  FIELD_SERVICE_REV
  *          指定机组的转速2 FIELD_SERVICE_OFF_REV2
  *
  */
//#define     SERVICE_SERVICE_ON_GETREV                 20010

int nCount=0;

	char chPlant[]={"1-GT1/1-K1"};
	char chChan[]={"5V"};
	char chChan1[]={"5H"};
	int itype1, itype;

	theComm.CreateHead(SERVICE_SERVICE_ON_GETREV);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
//	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan1);
//	COleDateTime time1(2005,3,1,10,1,1);
	while(theComm.More())
	{
		itype1=theComm.GetLong(FIELD_SERVICE_REV);
		itype=theComm.GetLong(FIELD_SERVICE_OFF_REV2);
		TRACE("REV1:%d,REV2:%d\n",itype1,itype);
		nCount++;

	}

}

// 测试细化分析
int CtestMwDlg::TestZoom(void)
{
	/**得到指定机组振动测点的细化频谱分析和波形。
  *输入参数：机组ID           FIELD_SERVICE_PLANTID
  *          测点             FIELD_SERVICE_CHANN1
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
  *返回值的字段码：
  *          测点的波形数据   FIELD_SERVICE_TIMEWAVE1
  *          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
  *          波形数据个数     FIELD_SERVICE_TIMEWAVE_NUM
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
  *          状态             FIELD_SYSTEM_STATUS
  *          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
  #define     SERVICE_SERVICE_ON_GETZOOMSPEC                20020
  */
	theComm.CreateHead(SERVICE_SERVICE_ON_GETZOOMSPEC);
	char chPlant[10]={"PISPLANT"};
	CString strChann="1H";

	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_CHANN1,strChann);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,610);
	theComm.SetItem(FIELD_SERVICE_ZOOM_STARTFREQ,50.0);
	theComm.SetItem(FIELD_SERVICE_ZOOM_STOPFREQ,80);
    int iWaveNum,iLines;
	float fWave1[1024],fFreqWave1[1000];
	float fDf,fStartFreq;
	if(theComm.More())
	{
        iLines=theComm.GetLong(FIELD_SYSTEM_STATUS);
		if(iLines==0)
		{
		iWaveNum=theComm.GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
		fDf=theComm.GetDouble(FIELD_SERVICE_FREQWAVE_DF);
        fStartFreq=theComm.GetDouble(FIELD_SERVICE_ZOOM_STARTFREQ);
		iLines=theComm.GetLong(FIELD_SERVICE_ZOOM_LINES);
		theComm.GetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)fWave1,sizeof(float)*iWaveNum);
        theComm.GetItemBuf(FIELD_SERVICE_ZOOMSPECTRUM,(char *)fFreqWave1,sizeof(float)*iLines);
		}
		else
		{
			TRACE("Error Info:%s\n",theComm.GetItem(FIELD_SYSTEM_STATUS_DISCRIPTION));
		}
	}
	else
	{
		AfxMessageBox("超时退出");
	}
	return 0;
}

// 得到历史数据的细化谱
int CtestMwDlg::TstHisZoomSpm(void)
{
/**得到指定历史时刻、机组振动测点的细化频谱分析和波形。
  *输入参数：机组ID           FIELD_SERVICE_PLANTID
  *          测点             FIELD_SERVICE_OFF_CHANN1
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          终止频率         FIELD_SERVICE_ZOOM_STOPFREQ
  *          时间             FIELD_SERVICE_OFF_TIME
  *返回值的字段码：
  *          测点的波形数据   FIELD_SERVICE_OFF_WAVE
  *          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
  *          测点的频谱数据   FIELD_SERVICE_ZOOMSPECTRUM
  *          分析线数         FIELD_SERVICE_ZOOM_LINES
  *          起始频率         FIELD_SERVICE_ZOOM_STARTFREQ
  *          振动测点频谱DF   FIELD_SERVICE_FREQWAVE_DF
  *          通频特征值：     FIELD_SERVICE_TREND_OVERALLCHAR
  *          状态             FIELD_SYSTEM_STATUS
  *          状态描述         FIELD_SYSTEM_STATUS_DISCRIPTION
 #define     SERVICE_SERVICE_OFF_GETZOOMSPEC             21007
 */
	int nCount=0;

	float itype;

	theComm.CreateHead(SERVICE_SERVICE_OFF_GETZOOMSPEC);
	theComm.SetItem(FIELD_SERVICE_PLANTID,"PISPLANT");
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"1V");
//	COleDateTime time1(2005,3,1,10,1,1);2005-06-26 20:49:08
	COleDateTime time2(2005,11,19,8,23,18);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time2);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,40);
    theComm.SetItem(FIELD_SERVICE_ZOOM_STARTFREQ,50.0);
	theComm.SetItem(FIELD_SERVICE_ZOOM_STOPFREQ,90);
    int iWaveNum,iLines;
	float fWave1[1024],fFreqWave1[1000];
	float fDf,fStartFreq;
	if(theComm.More())
	{
        iLines=theComm.GetLong(FIELD_SYSTEM_STATUS);
		if(iLines==0)
		{
		iWaveNum=theComm.GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
		fDf=theComm.GetDouble(FIELD_SERVICE_FREQWAVE_DF);
        fStartFreq=theComm.GetDouble(FIELD_SERVICE_ZOOM_STARTFREQ);
		iLines=theComm.GetLong(FIELD_SERVICE_ZOOM_LINES);
		theComm.GetItemBuf(FIELD_SERVICE_OFF_WAVE,(char *)fWave1,sizeof(float)*iWaveNum);
        theComm.GetItemBuf(FIELD_SERVICE_ZOOMSPECTRUM,(char *)fFreqWave1,sizeof(float)*iLines);
		}
		else
		{
			TRACE("Error Info:%s\n",theComm.GetItem(FIELD_SYSTEM_STATUS_DISCRIPTION));
		}
	}
	return nCount;
}
int CtestMwDlg::testWaterFall()
{
	/**得到指定指定机组和指定可以做启停车的测点瀑布图，一次只能得到一组波形频谱
	*输入参数：机组ID           FIELD_SERVICE_PLANTID
	*          测点号           FIELD_SERVICE_CHANN1
	*          分析线数         FIELD_SERVICE_ZOOM_LINES
	*返回值的字段码：
	*          相位：           FIELD_SERVICE_TREND_ONECHAR
	*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
	*          转速：           FIELD_SERVICE_REV
	*          频谱数据         FIELD_SERVICE_FREQWAVE1
	*          频谱数据个数     FIELD_SERVICE_FREQWAVE_NUM
	*          频谱DF           FIELD_SERVICE_FREQWAVE_DF
	*define     SERVICE_SERVICE_ON_GETWATERFALL                20019
	*/
	theComm.CreateHead(SERVICE_SERVICE_ON_GETWATERFALL);
	CString chPlant="21C402";
	CString strChann="1V";

	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_CHANN1,strChann);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,400);
	int iWaveNum,iLines;
	float fWave1[2048],fFreqWave1[1000];
	float fDf,fStartFreq;
	if(theComm.More())
	{
		iLines=theComm.GetLong(FIELD_SERVICE_FREQWAVE_NUM);
		if(iLines!=0)
		{
			theComm.GetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)fWave1,sizeof(float)*iLines);
			//theComm.GetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)fWave1,sizeof(float)*iLines);
			//writefile(fWave1,iLines,10);

		}
		else
		{
			TRACE("Error Info:%s\n",theComm.GetItem(FIELD_SYSTEM_STATUS_DISCRIPTION));
		}
	}
	else
	{
		AfxMessageBox("超时退出");
	}
	return 0;
}
int CtestMwDlg::testGetHistoryAxesTrack(void)
{
	/**得到指定机组、测点和时间的一组轴心轨迹的波形数据。
	*输入参数：机组ID               FIELD_SERVICE_PLANTID
	*          水平方向振动测点ID   FIELD_SERVICE_CHANNID_HOR
	*          垂直方向振动测点ID   FIELD_SERVICE_CHANNID_VER
	*          时间                 FIELD_SERVICE_OFF_TIME
	*返回值的字段码：
	*          轴心轨迹的垂直方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_VER
	*          垂直方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM
	*          轴心轨迹的水平方向波形数据       FIELD_SERVICE_AXESTRACK_WAVE_HOR
	*          水平方向的波形数据压缩后的字节数 FIELD_SERVICE_ZIPWAVE_BYTESNUM1
	*          轴心轨迹的一个的波形数据个数     FIELD_SERVICE_AXESTRACK_WAVENUM
	*          指定机组测点的转速               FIELD_SERVICE_REV
	*          采样频率                         FIELD_SERVICE_SAMPLE_FREQ
#define     SERVICE_SERVICE_OFF_GETAXESTRACK              21008	*/


	int nCount=0;

	char chPlant[]={"PLANT1"};
	char chChan[]={"1V"};
	char chChan1[]={"1H"};
	CString strChann;
	int itype;
	long iAllBytes=0,UnzipBytes=0;
	float *fwave=NULL,*ffreq=NULL;
	//2006-07-02 20:24:13
	COleDateTime time1(2006,7,2,20,24,13);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETAXESTRACK);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,chChan);
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,chChan1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time1);
	if(theComm.More())
	{
		itype=theComm.GetLong(FIELD_SERVICE_AXESTRACK_WAVENUM);
		fwave=new float[itype];
		iAllBytes=theComm.GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM);
		BYTE *pByte=new BYTE[iAllBytes];
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pByte,iAllBytes);
		CZipImpl zipImpl;
		zipImpl.UnZip(pByte,iAllBytes,(BYTE*)fwave,itype*4,UnzipBytes);
		//writefile(fwave,itype,0);
		delete pByte;
		iAllBytes=theComm.GetLong(FIELD_SERVICE_ZIPWAVE_BYTESNUM1);
		ffreq=new float[itype];
		pByte=new BYTE[iAllBytes];
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pByte,iAllBytes);
		zipImpl.UnZip(pByte,iAllBytes,(BYTE*)ffreq,itype*4,UnzipBytes);
		delete pByte;
		//writefile(ffreq,itype,1);
		//		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)fwave,itype*4);
		//		nCount++;
		TRACE(" %s;rev\n",strChann);

	}
	delete fwave;
	delete ffreq;
	return nCount;
}

int CtestMwDlg::testGetSpecTimeChannSud()
{
	/*!得到指定启停车开始时刻和指定机组、指定测点的启停车的转速、保存这组数据的时刻、幅值和相位序列。通过循环得到所有转速，然后，分别通过转速或保存这组数据的时刻、机组号和测点号
	*输入参数字段码
	*          机组ID：    FIELD_SERVICE_PLANTID
	*          测点ID：    FIELD_SERVICE_OFF_CHANN1
	*          启停车时间: FIELD_SERVICE_OFF_TIMESTART
	*返回值的字段码：
	*          转速:       FIELD_SERVICE_OFF_REV
	*          转速2：     FIELD_SERVICE_OFF_REV2
	*          时间        FIELD_SERVICE_OFF_TIME
	*          相位：      FIELD_SERVICE_OFF_PHASE
	*          幅值：      FIELD_SERVICE_OFF_AMP
	*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
	*define    SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD   21014
	*/


	int nCount=0;

	char chPlant[]={"P401"};
	char chChan[]={"1V"};
	CString strChann;
	int itype;
	long iAllBytes=0,UnzipBytes=0;
	S_VibCharValue vibCharValue;
	//2006-04-06 13:30:21
	//COleDateTime time1(2008,3,10,16,46,36);
    COleDateTime time1(2008,3,11,15,57,00);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECTIMECHANNSUD);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);

	if(theComm.More())
	{
    //itype=theComm.GetLong(FIELD_SERVICE_AXESTRACK_WAVENUM);
      theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
	  itype=theComm.GetLong(FIELD_SERVICE_OFF_REV);

	  CString strTemp;
	  strTemp.Format("转速：%d,振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",itype,vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
	  AfxMessageBox(strTemp);
	//TRACE(" %s;rev\n",strChann);

	}

	return nCount;
}
int CtestMwDlg::test21015(void)
{/*!得到指定启停车开始时刻、指定机组、指定测点和指定转速的启停车的详细数据。
 *输入参数字段码
 *          机组ID：    FIELD_SERVICE_PLANTID
 *          测点ID：    FIELD_SERVICE_OFF_CHANN1
 *      启停车开始时刻: FIELD_SERVICE_OFF_TIMESTART
 *          转速:       FIELD_SERVICE_OFF_REV
 *          分析线数    FIELD_SERVICE_OFF_FREQANALYSIS_LINES
 *返回值的字段码：
 *          相位：      FIELD_SERVICE_OFF_PHASE
 *          幅值：      FIELD_SERVICE_OFF_AMP
 *          波形：      FIELD_SERVICE_OFF_WAVE
 *          频谱：      FIELD_SERVICE_OFF_FREQWAVE
 *          频率DF值：  FIELD_SERVICE_OFF_FREQWAVE_DF
 *          波形点数：  FIELD_SERVICE_OFF_WAVE_NUM
 *          频谱点数：  FIELD_SERVICE_OFF_FREQWAVE_NUM
#define    SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD    21015 */


	int nCount=0;

	char chPlant[]={"3-K1"};
	char chChan[]={"1V"};
	CString strChann;
	int itype;
	long iAllBytes=0,UnzipBytes=0;
    float ff[1024];
    float *pfwave=NULL;
	//2006-04-06 13:30:21
//	COleDateTime time1(2006,4,28,1,58,27);
		COleDateTime time1(2006,4,28,2,53,15);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECCHANNREVSUD);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_REV,3360);
	if(theComm.More())
	{
		itype=theComm.GetLong(FIELD_SERVICE_OFF_FREQWAVE_NUM);
        theComm.GetItemBuf(FIELD_SERVICE_OFF_FREQWAVE,(char*)ff,itype*4);
		//writefile(ff,itype,4);
		itype=theComm.GetLong(FIELD_SERVICE_OFF_WAVE_NUM);
		pfwave=new float[itype];
		theComm.GetItemBuf(FIELD_SERVICE_OFF_WAVE,(char*)pfwave,itype*4);
		//writefile(pfwave,itype,5);
		delete pfwave;
		TRACE(" %s;rev\n",strChann);

	}
	return 0;
}

int CtestMwDlg::TestSpecPlantChannState(void)
{/**得到指定机组所有测点特征值，通过循环依次得到所有值。注：对于振动测点来说，指定那种特征类型就给那一种
 *输入参数值：机组1ID          FIELD_SERVICE_PLANTID
 *            特征值类型      FIELD_SERVICE_TREND_CHARTYPE
 *            机组2ID         FIELD_SERVICE_CHANN2
 *            机组3ID         FIELD_SERVICE_CHANN3
 *            机组4ID         FIELD_SERVICE_CHANN4
 *返回值的字段码：
 *            设备ID         FIELD_SERVICE_PLANTID
 *            测点ID         FIELD_SERVICE_CHANN1
 *            测点值         FIELD_SERVICE_CHANN1VALUE
 *            测点单位       FIELD_SERVICE_CHANN1_UNIT
 *            测点所属列表   FIELD_SERVICE_CHANN1_LIST
 *            测点的状态     FIELD_SERVICE_CHANN1_STATUS
#define     SERVICE_SERVICE_ON_GETALLCHANNVALUE       20014 */
	int nCount=0;
	char chPlant[]={"3-K1"};
	char chChan[]={"1V"};

	//2006-04-06 13:30:21
	//	COleDateTime time1(2006,4,28,1,58,27);
	theComm.CreateHead(SERVICE_SERVICE_ON_GETALLCHANNVALUE);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	if(theComm.More())
	{


	}
	return 0;
}

int CtestMwDlg::TestTrendAmpAndPhase(void)
{
	/**得到指定设备和振动测点指定时间段和指定特征值类型的倍频幅值和相位数据，
	*通过循环依次得到所有趋势数据和对应时间，每次取得数据对应时间都是相同的
	*输入参数字段码：
	*          机组ID      FIELD_SERVICE_PLANTID
	*          测点ID1     FIELD_SERVICE_OFF_CHANN1
	*          测点ID2     FIELD_SERVICE_OFF_CHANN2
	*          测点ID3     FIELD_SERVICE_OFF_CHANN3
	*          测点ID4     FIELD_SERVICE_OFF_CHANN4
	*          特征值类型  FIELD_SERVICE_TREND_CHARTYPE
	*          起始时间    FIELD_SERVICE_OFF_TIMESTART
	*          终止时间    FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*          测点ID1特征值   FIELD_SERVICE_OFF_CHANN1VALUE
	*          测点ID2特征值   FIELD_SERVICE_OFF_CHANN2VALUE
	*          测点ID3特征值   FIELD_SERVICE_OFF_CHANN3VALUE
	*          测点ID4特征值   FIELD_SERVICE_OFF_CHANN4VALUE
	*          测点ID1相位值   FIELD_SERVICE_OFF_CHANN1PHASE
	*          测点ID2相位值   FIELD_SERVICE_OFF_CHANN2PHASE
	*          测点ID3相位值   FIELD_SERVICE_OFF_CHANN3PHASE
	*          测点ID4相位值   FIELD_SERVICE_OFF_CHANN4PHASE
	*          时间值      FIELD_SERVICE_OFF_TIME
	#define     SERVICE_SERVICE_OFF_GETTRENDAMPANDPHASE         21009
	*/
	int nCount=0;

	char chPlant[]={"PLANT1"};
	char chChan[]={"1V"};
	CString strChann;
	float fwave[2048],ffreq[1024],ftmp;
	COleDateTime time1(2006,2,1,14,1,1);
	COleDateTime time2(2006,6,19,19,40,59);
//	theComm.CreateHead(SERVICE_SERVICE_OFF_GETTRENDAMPANDPHASE);
	//theComm.SetItem(FIELD_SERVICE_OFF_TRENDTYPE,1);
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,GE_PHASECHARONE);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,chChan);
	theComm.SetItem(FIELD_SERVICE_OFF_CHANN2,"2H");
//	theComm.SetItem(FIELD_SERVICE_OFF_CHANN3,"2V");
//	theComm.SetItem(FIELD_SERVICE_OFF_CHANN4,"1H");
//	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	while(theComm.More())
	{
		ftmp=theComm.GetDouble(FIELD_SERVICE_OFF_CHANN1VALUE);
		nCount++;
		TRACE(" %s;rev\n",strChann);

	}
	return nCount;
}

int CtestMwDlg::TestGetSpecCharAndPhase(void)
{
	/**读取指定设备振动测点的指定倍频的特征值和相位，
	*如果指定测点了，就只取指定测点的特征值和相位
	*如果没有指定测点，则获得所有测点通过循环依次取得各个测点的对应倍频特征值和相位，每循环依次可得到一个测点指定趋势特征值和相位
	*输入参数值：机组ID      FIELD_SERVICE_PLANTID
	*            测点ID      FIELD_SERVICE_CHANN1
	*            特征值类型  FIELD_SERVICE_TREND_CHARTYPE
	*返回值的字段码：
	*            测点ID      FIELD_SERVICE_CHANN1
	*            指定特征值  FIELD_SERVICE_TREND_CHAR
	*            对应的相位  FIELD_SERVICE_TREND_PHASE
	#define     SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE     20022
	*/
	int nCount=0;

	char chPlant[]={"PLANT1"};
	char chChan[]={"1V"};
	float ftmp=.0;
	theComm.CreateHead(SERVICE_SERVICE_ON_GETALLSPECCHARANDPHASE);
	//theComm.SetItem(FIELD_SERVICE_OFF_TRENDTYPE,1);
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,GE_PHASECHARONE);
//	theComm.SetItem(FIELD_SERVICE_CHANN1,chChan);
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	while(theComm.More())
	{
//		ftmp=theComm.GetDouble(FIELD_SERVICE_TREND_CHAR);
		nCount++;
		TRACE(" %s;rev %.2f,%.2f\n",theComm.GetItem(FIELD_SERVICE_CHANN1),ftmp,theComm.GetDouble(FIELD_SERVICE_TREND_PHASE));

	}
	return 0;
}

int CtestMwDlg::testGetMWinfo(void)
{/**得到其子中间件服务器IP,端口和属性信息.
 中间件服务器的级别：
 GE_SRV_TYPE_SET,          ///装置级中间件服务器
 GE_SRV_TYPE_COMPANY,      ///公司级中间件服务器
 GE_SRV_TYPE_GROUP         ///集团级中间件服务器
 *输入字段:
 *        公司ID        FIELD_SYSTEM_COMPANY
 *返回字段：
 *        中间件ID         FIELD_SYSTEM_SET
 *        中间件IP         FIELD_SYSTEM_MW_IP
 *        中间件Port       FIELD_SYSTEM_MW_PORT
 *        中间件级别       FIELD_SYSTEM_USER_LEVEL
 #define  SERVICE_SYSTEM_GET_MW_INFO                   25028
 */
	char cCompany[]={"大庆石化"};
	theComm.CreateHead(SERVICE_SYSTEM_GET_MW_INFO);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,cCompany);
	while (theComm.More())
	{
		TRACE("ID:%d\n",theComm.GetLong(FIELD_SYSTEM_SET));
		TRACE("IP:%s\n",theComm.Get(FIELD_SYSTEM_MW_IP));
	}
	return 0;
}

int CtestMwDlg::Get25010(void)
{
	/**得到指定分厂下所有机组位号和相关IP信息，通过循环依次得到所有机组位号。如果直接链接访问的公司级服务器，则公司级IP和Port为空；如果直接链接访问的是装置级则，则公司和装置级IP和Port均为空
	*输入参数：公司名     FIELD_SYSTEM_COMPANY
	*          分厂名     FIELD_SYSTEM_FACTORY
	*返回值的字段码：
	*          机组号     FIELD_SYSTEM_PLANT
	*          概貌图类型 FIELD_SYSTEM_PLANT_GRAPHTYPE
	*          公司级IP   FIELD_SYSTEM_COMPANY_MW_IP
	*          公司级Port FIELD_SYSTEM_COMPANY_MW_PORT
	*          装置级IP   FIELD_SYSTEM_MW_IP
	*          装置级Port FIELD_SYSTEM_MW_PORT
	*          装置级中间件ID   FIELD_SYSTEM_SET
	#define     SERVICE_SYSTEM_GETPLANTIPOFSUBCORP          25010
	*/
	char cCompany[]={"大庆石化"};
	char cFactory[]={"化肥厂"};
	CString strPlant,strSetID,strIP,strParentIP;
	int iPort,iParentPort;
	theComm.CreateHead(SERVICE_SYSTEM_GETPLANTIPOFSUBCORP);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,cCompany);
	theComm.SetItem(FIELD_SYSTEM_FACTORY,cFactory);
	while (theComm.More())
	{
       strPlant=theComm.Get(FIELD_SYSTEM_PLANT);
	   strIP=theComm.Get(FIELD_SYSTEM_MW_IP);
	   iPort=theComm.GetLong(FIELD_SYSTEM_MW_PORT);
	   iParentPort=theComm.GetLong(FIELD_SYSTEM_COMPANY_MW_PORT);
	   strParentIP=theComm.Get(FIELD_SYSTEM_COMPANY_MW_IP);
	   strSetID=theComm.Get(FIELD_SYSTEM_SET);
	   TRACE("设备：%s,%s,%s,%d, %s,%d\n",strSetID,strPlant,strIP,iPort,strParentIP,iParentPort);
	}
	return 0;
}

int CtestMwDlg::GetCompanyOfUser()
{
	/**通过用户名和密码得到用户级别、集团名、公司名，如果用户级别是集团级，就需要循环得到各个公司和厂
	*输入参数：用户名   FIELD_SYSTEM_USEID
	*          密码     FIELD_SYSTEM_PASSWORD
	*返回值的字段码：
	*          用户级别 FIELD_SYSTEM_USER_LEVEL
	*          集团名   FIELD_SYSTEM_GROUP
	*          公司名   FIELD_SYSTEM_COMPANY
	*          分厂名   FIELD_SYSTEM_FACTORY
	#define     SERVICE_SYSTEM_GETCOMPANYTOFUSER          25004
	*/
	CString strPlant,strSetID,strIP,strParentIP;
	int iPort,iParentPort;
	theComm.CreateHead(SERVICE_SYSTEM_GETCOMPANYTOFUSER);
	theComm.SetItem(FIELD_SYSTEM_USEID,"");
	theComm.SetItem(FIELD_SYSTEM_PASSWORD,"");
	while (theComm.More())
	{
		strPlant=theComm.Get(FIELD_SYSTEM_GROUP);
		strIP=theComm.Get(FIELD_SYSTEM_COMPANY);
		strSetID=theComm.Get(FIELD_SYSTEM_FACTORY);
		TRACE("设备：%s,%s,%s\n",strSetID,strPlant,strIP);
	}
	return 0;
}

int CtestMwDlg::InsertAlarmParam()
{
	int nCount=0;
	CSimADO *pDB=new CSimADO;
	CString strSvr="DSE-MARBLE\\MARBLE";
	CString strDbName="SwPlantInfo2000";
	CString strUsr="sa",strPwd="123";
	CAxisAlarmParam axisParam;
	long lSize=sizeof(CAxisAlarmParam);
	axisParam._bCheckF=false;
	axisParam._bCheckS=false;
	axisParam._bCheckTrend=false;
	pDB->SetSqlLogPath("c:\\");
	if(!(pDB->Open(strSvr,strUsr,strPwd,strDbName)))
	{
	  	AfxMessageBox("打开数据库失败");
		return 1;
	}
	for (int loop=1;loop<566;++loop)
	{
		pDB->Cmd("select alarm_param from plant_axis where [id]=%d",loop);
		if(pDB->More())
		{
           nCount++;
		   pDB->SetBuffer("alarm_param",(BYTE*)&axisParam,lSize);
		}
		else
			AfxMessageBox("打开失败");

	}
	delete pDB;
	strDbName.Format("总共完成数目%d",nCount);
	AfxMessageBox(strDbName);
	return 1;
}
int CtestMwDlg::TestGetVibChannAlarmLog(void)
{
	/*!得到指定起止时间的指定机组和指定振动测点类型的报警记录
	*输入参数字段码
	*          公司ID   FIELD_SYSTEM_COMPANY
	*    			 分厂名   FIELD_SYSTEM_FACTORY
	*          机组ID： FIELD_SERVICE_PLANTID
	*          测点名： FIELD_SERVICE_CHANN1
	*          起始时间：  FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：  FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*          报警开始时间:   FIELD_SERVICE_OFF_TIMESTART
	*          报警结束时间:   FIELD_SERVICE_OFF_TIMEEND
	*          报警类型：      FIELD_SYSTEM_TYPE
	*          事件ID          FIELD_SYSTEM_EVENT_ID
	*          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
	#define     SERVICE_OFF_GETVIBCHANNALARMLOG      21011
	*/
	theComm.CreateHead(SERVICE_OFF_GETVIBCHANNALARMLOG);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"大庆石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化肥厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"3102J");
	theComm.SetItem(FIELD_SERVICE_CHANN1,"7H");
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,"2007-7-1 1:1:1");
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,"2007-8-1 1:1:1");
	while (theComm.More())
	{
	}
	return 0;
}

/**读取指定任意多个测点的各种类型的特征值。
  *输入参数：
  *          公司ID   FIELD_SYSTEM_COMPANY
  *			 分厂名   FIELD_SYSTEM_FACTORY
  *          机组ID   FIELD_SERVICE_PLANTID
  *          测点buff       FIELD_SERVICE_CHANN_BUFF
  *          测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
  *          测点个数       FIELD_SERVICE_CHANN_NUM
  *返回值的字段码：
  *          特征值结构体   FIELD_SERVICE_CHARVALUE_S
  *          测点ID         FIELD_SERVICE_CHANN1
  *          测点位号       FIELD_SYSTEM_CHANN_ALIAS
  *          报警值         FIELD_SYSTEM_ALARM_LOW
  *          危险值         FIELD_SYSTEM_ALARM_HIGH
  *          单位           FIELD_SERVICE_CHANN1_UNIT
  *          指定机组测点的转速    FIELD_SERVICE_REV
  *          时间           FIELD_SERVICE_TIME
  *          测点类型       FIELD_SYSTEM_CHANN_TYPE
  */
#define     SERVICE_SERVICE_ON_GETSPECFIVECHAR        20001    //读取指定机组指定振动测点的五种特征值。
int CtestMwDlg::Test20001()
{
	int nRev;
	CString strChann="1H";
	theComm.CreateHead(SERVICE_SERVICE_ON_GETSPECFIVECHAR);
    theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
    theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"EC");
    theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	while(theComm.More())
	{
		nRev = theComm.GetLong(FIELD_SERVICE_REV);

		HZLOG_DEBUG(STR("Chann=%s, Time=%s Rev=%s",
			theComm.Get(FIELD_SERVICE_CHANN1),
			theComm.Get(FIELD_SERVICE_TIME),
			nRev
			));
	}

	return nRev;

}

int CtestMwDlg::Test21024()
{
	/**得到指定公司、分厂、设备的任意多个振动测点的报警限等信息（依次循环测点的数据）
	*输入参数：
	*          公司ID          FIELD_SYSTEM_COMPANY
	*			     分厂名          FIELD_SYSTEM_FACTORY
	*          机组ID          FIELD_SERVICE_PLANTID
	*          测点buff        FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数        FIELD_SERVICE_CHANN_NUM
	*          事件ID:         FIELD_SYSTEM_EVENT_ID
	*返回值的字段码：
	*          各个测点特征值Buff  FIELD_SERVICE_CHARVALUE_SBUFF
	*          对应报警类型Buff    FIELD_SERVICE_TYPE_BUFF
	*          测点个数            FIELD_SERVICE_CHANN_NUM
	*          时间值              FIELD_SERVICE_OFF_TIME
	*          对应时间的微秒      FIELD_SERVICE_MICROSECOND
	#define     SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND        21024
	*/
	CString strChann="7V";
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETVIBCHANNALARMTREND);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"大庆石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化肥厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"3102J");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SYSTEM_EVENT_ID,1);
	S_VibCharValue vibCharValue;
	int iType=0,int iNums=0,iMicroSecond;
	COleDateTime dtTmp;
	while (theComm.More())
	{
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,(char *)&vibCharValue,sizeof(S_VibCharValue));
		theComm.GetItemBuf(FIELD_SERVICE_TYPE_BUFF,(char*)&iType,sizeof(int));
		iNums=theComm.GetLong(FIELD_SERVICE_CHANN_NUM);
		iMicroSecond=theComm.GetLong(FIELD_SERVICE_MICROSECOND);
		dtTmp=theComm.GetTime(FIELD_SERVICE_OFF_TIME);
		TRACE("Nums:%d,datetime: %d-%d-%d %d:%d:%d.%d  ",iNums,dtTmp.GetYear(),dtTmp.GetMonth(),dtTmp.GetDay(),dtTmp.GetHour(),dtTmp.GetMinute(),dtTmp.GetSecond(),iMicroSecond);
		TRACE("报警类型:%d 振动特征值1X:%.2f,2X:%.2f\n",iType,vibCharValue._fCharOne,vibCharValue._fCharTwo);
	}
	return 0;
}

int CtestMwDlg::Test21012()
{
	/**得到指定时间的指定测点的报警记录的波形和各种特征值，对于多个测点就循环得到
	*输入参数字段码
	*          公司ID          FIELD_SYSTEM_COMPANY
	*			     分厂名          FIELD_SYSTEM_FACTORY
	*          机组ID：        FIELD_SERVICE_PLANTID
	*          测点buff        FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数        FIELD_SERVICE_CHANN_NUM
	*          事件ID:         FIELD_SYSTEM_EVENT_ID
	*          报警时间：      FIELD_SERVICE_OFF_TIME
	*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
	*          波形数据个数    FIELD_SERVICE_OFF_WAVE_NUM
	*          分析线数：      FIELD_SERVICE_ZOOM_LINES
	*返回值的字段码：
	*          测点ID：        FIELD_SERVICE_OFF_CHANN1
	*          报警类型：      FIELD_SYSTEM_TYPE
	*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
	*          波形：          FIELD_SERVICE_OFF_WAVE
	*          频谱：          FIELD_SERVICE_OFF_FREQWAVE
	*          频率DF值：      FIELD_SERVICE_OFF_FREQWAVE_DF
	*          波形点数：      FIELD_SERVICE_OFF_WAVE_NUM
	*          频谱数据点数    FIELD_SERVICE_ZOOM_LINES
	*          转速：          FIELD_SERVICE_OFF_REV
	#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORD     21012
	*/
	/**得到指定时间的指定测点的报警记录的各种特征值，对于多个测点就循环得到
	*输入参数字段码
	*          公司ID          FIELD_SYSTEM_COMPANY
	*			     分厂名          FIELD_SYSTEM_FACTORY
	*          机组ID：        FIELD_SERVICE_PLANTID
	*          测点buff        FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度    FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数        FIELD_SERVICE_CHANN_NUM
	*          事件ID:         FIELD_SYSTEM_EVENT_ID
	*          报警时间：      FIELD_SERVICE_OFF_TIME
	*          对应时间的毫秒  FIELD_SERVICE_MICROSECOND
	*返回值的字段码：
	*          测点ID：        FIELD_SERVICE_OFF_CHANN1
	*          测点别名        FIELD_SYSTEM_CHANN_ALIAS
	*          报警类型：      FIELD_SYSTEM_TYPE
	*          特征值结构体    FIELD_SERVICE_CHARVALUE_S
	*          转速：          FIELD_SERVICE_OFF_REV
	*          报警类型描述    FIELD_SYSTEM_RESERVE_STRING
	*          报警值          FIELD_SYSTEM_ALARM_LOW
	*          危险值          FIELD_SYSTEM_ALARM_HIGH
	#define    SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE    21034
	*/
	CString strChann="7V";
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECALARMTIMERECORDCHARVALUE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD201B往复");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SYSTEM_EVENT_ID,4);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,"2007-08-16 13:35:34");
	theComm.SetItem(FIELD_SERVICE_MICROSECOND,0);
	theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,1024);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,800);
	S_VibCharValue vibCharValue;
	int iType=0,int iNums=0,iMicroSecond;
	COleDateTime dtTmp;
	while (theComm.More())
	{
		}

	return 0;
}

int CtestMwDlg::test20013()
{
	/**得到用户所观测机组范围内所有机组的测点运行状态，通过循环依次得到所有报警的测点,如果不需要通道信息则把FIELD_SERVICE_IFCHANN设为-1
	*输入参数值：用户ID         FIELD_SYSTEM_USEID
	*            是否需要通道   FIELD_SERVICE_IFCHANN
	*返回值字段码：
	*            公司ID             FIELD_SYSTEM_COMPANY
	*			       分厂名             FIELD_SYSTEM_FACTORY
	*            报警的机组ID       FIELD_SERVICE_PLANTID
	*            报警的测点ID       FIELD_SERVICE_CHANN1
	*            报警值             FIELD_SERVICE_CHANN1VALUE
	#define     SERVICE_SERVICE_ON_GETALLCHANNELSTATE     20013
	*/
	/**通过分厂名得到下面设备的报警状态,并且得到指定机组的转速
	*输入参数值：
	*            公司ID   FIELD_SYSTEM_COMPANY
	*			 分厂名   FIELD_SYSTEM_FACTORY
	*            机组ID   FIELD_SERVICE_PLANTID
	*返回值字段码：
	*            报警的机组ID         FIELD_SERVICE_PLANTID
	*            指定设备的转速       FIELD_SERVICE_REV
	*            指定机组的转速2      FIELD_SERVICE_OFF_REV2
	#define     SERVICE_SERVICE_ON_GETSUBCORPSTATE        20017
	*/
    theComm.CreateHead(SERVICE_SERVICE_ON_GETSUBCORPSTATE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
//	theComm.SetItem(FIELD_SERVICE_PLANTID,"DSE201");
	while (theComm.More())
	{
	}
	return 0;
}

int CtestMwDlg::Test25012()
{
	/**通过测点类型和传感器类型得到特征值描述
	*输入参数：
	*             设备类型    FIELD_SYSTEM_TYPE
	*             测点类型    FIELD_SYSTEM_CHANN_TYPE
	*             传感器类型  FIELD_SYSTEM_SENSOR_TYPE
	*返回值的字段码：
	*             顺序     FIELD_SYSTEM_STATUS
	*             对应描述 FIELD_SYSTEM_STATUS_DISCRIPTION
	*             特征值单位   FIELD_SERVICE_CHANN1_UNIT
	#define     SERVICE_SYSTEM_GETCHARTYPEDESC            25012
	*/

	/**修改指定设备指定测点的报警信息，如果为-1，表示没有修改成功，为1表示修改成功
	*输入参数：   公司名         FIELD_SYSTEM_COMPANY
	*             分厂名         FIELD_SYSTEM_FACTORY
	*             机组名         FIELD_SYSTEM_PLANT
	*             测点名         FIELD_SYSTEM_CHANN
	*             是否常规报警   FIELD_SYSTEM_STATUS //1为判断报警，0为不判断报警
	*             报警限类型     FIELD_SYSTEM_EVENT_TYPE
	*             报警上限       FIELD_SYSTEM_ALARM_LOW
	*             报警上上限     FIELD_SYSTEM_ALARM_HIGH
	*             报警下限       FIELD_SYSTEM_ALARM_LOW2
	*             报警下下限     FIELD_SYSTEM_ALARM_HIGH2
	*返回值的字段码：
	*             返回状态       FIELD_SYSTEM_STATUS
	#define     SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO       25115
	*/
/*
	int nCount=0;
	string sUnit="";
	theComm.CreateHead(SERVICE_SYSTEM_GETCHARTYPEDESC);
	theComm.SetItem(FIELD_SYSTEM_TYPE,11);
	theComm.SetItem(FIELD_SYSTEM_CHANN_TYPE,11);
	theComm.SetItem(FIELD_SYSTEM_SENSOR_TYPE,11);
	while (theComm.More())
	{
		CString strTemp;strTemp.Format("单位：%s\n",sUnit);AfxMessageBox(strTemp);
		++ nCount;
	}
	return nCount;*/
	int nCount=0;
	string sUnit="";
	theComm.CreateHead(SERVICE_SYSTEM_MODIFY_SPECCHANNALARMINFO);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SYSTEM_PLANT,"K201A");
	theComm.SetItem(FIELD_SYSTEM_CHANN,"1V");
	theComm.SetItem(FIELD_SYSTEM_STATUS,0);
	theComm.SetItem(FIELD_SYSTEM_EVENT_TYPE,0);
	theComm.SetItem(FIELD_SYSTEM_ALARM_LOW,60.1);
	theComm.SetItem(FIELD_SYSTEM_ALARM_HIGH,70.2);
	theComm.SetItem(FIELD_SYSTEM_ALARM_LOW2,-50.1);
	theComm.SetItem(FIELD_SYSTEM_ALARM_HIGH2,-60.2);
	if (theComm.More())
	{
		int iStatus=theComm.GetLong(FIELD_SYSTEM_STATUS);
		CString strTemp;strTemp.Format("iStatus:%d\n",iStatus);AfxMessageBox(strTemp);
		++ nCount;
	}
	return nCount;
}

int CtestMwDlg::Test20018(void)
{
	/**得到指定指定机组和指定可以做启停车的测点的波特图数据，一次只能得到一组数据，如果没有指定测点，则发送所有测点的数据
	*输入参数：
	*          公司ID           FIELD_SYSTEM_COMPANY
	*		   分厂名           FIELD_SYSTEM_FACTORY
	*          机组ID           FIELD_SERVICE_PLANTID
	*          测点buff         FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数         FIELD_SERVICE_CHANN_NUM
	*          特征值类型       FIELD_SERVICE_TREND_CHARTYPE
	*返回值的字段码：
	*          测点号           FIELD_SERVICE_CHANN1
	*          相位：           FIELD_SERVICE_TREND_ONECHAR
	*          幅值：           FIELD_SERVICE_TREND_OVERALLCHAR
	*          转速：           FIELD_SERVICE_REV
	*          时间             FIELD_SERVICE_TIME
	*          对应时间的微秒   FIELD_SERVICE_MICROSECOND
	*          开停车状态       FIELD_SERVICE_SUD_STATUS
	*          开停车类型       FIELD_SERVICE_SUD_TYPE
	*          启停车事件ID     FIELD_SYSTEM_EVENT_ID
	*          振动特征值结构体    FIELD_SERVICE_CHARVALUE_S
	#define     SERVICE_SERVICE_ON_GETBODE                20018
	*/

	int nCount=0;
	char chPlant[]={"P401"};
	CString strChann="1V";
	//CString strChann;
	int itype;
	S_VibCharValue vibCharValue;
	theComm.CreateHead(SERVICE_SERVICE_ON_GETBODE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,chPlant);
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,0);
	if (theComm.More())
	{
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
		itype=theComm.GetLong(FIELD_SERVICE_OFF_REV);
		CString strTemp;
		strTemp.Format("转速：%d,振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",itype,vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
		AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

/** 获取各个程序的路径和版本号
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            程序名                 FIELD_SERVICE_MODULE_NAME
  *            文件名                 FIELD_SERVICE_FILE_NAME
  *            路径                   FIELD_SERVICE_FILE_PATH
  *            版本号                 FIELD_SERVICE_FILE_VERSION
  *\instance 例:
      [JSON]
      "MiddleWare" : {
         "POSITION" : "E:\\Project\\lib\\S_SystemSvr.dll",
         "POSITION1" : "E:\\Project\\lib\\S_OnlineSvr.dll",
         "POSITION2" : "E:\\Project\\lib\\S_OfflineSvr.dll",
         "S_OfflineSvr.dll" : "3.5.0",
         "S_OnlineSvr.dll" : "3.5.0",
         "S_SystemSvr.dll" : "3.5.0"
      },

      [中间件通信数据格式]
      "MiddleWare"
      "S_OfflineSvr.dll"
      "E:\\Project\\lib\\S_OfflineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_OnlineSvr.dll"
      "E:\\Project\\lib\\S_OnlineSvr.dll"
      "3.5.0"
     $
      "MiddleWare"
      "S_SystemSvr.dll"
      "E:\\Project\\lib\\S_SystemSvr.dll"
      "3.5.0"
 #define     SERVICE_SYSTEM_GET_MODULE_VERSION                 25029
  */
int CtestMwDlg::Test25029()
{
	int nRet = -1;
	theComm.CreateHead(SERVICE_SYSTEM_GET_MODULE_VERSION);
	while(theComm.More()){
		HZLOG_INFO(STR("[Module]=%s, [FILE]=%s, [Path]=%s, [Version]=%s",
		theComm.Get(FIELD_SERVICE_MODULE_NAME),
		theComm.Get(FIELD_SERVICE_FILE_NAME),
		theComm.Get(FIELD_SERVICE_FILE_PATH),
		theComm.Get(FIELD_SERVICE_FILE_VERSION)));

		nRet ++;
	}

	return nRet;
}

/** 获取各个数据库状态
  *输入参数字段码：无
  *返回值的字段码：(n)
  *     报警数据预计保存时间,单位 天(AlarmDataSavingTime)     FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME
  *     趋势数据预计保存时间(DataSavingTime)                  FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME
  *     报警和趋势数据预计保存时间(TrendDataSavingTime)       FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME
  *     数据库数目(DBNum)                                     FIELD_SERVICE_DB_NUM
  *     数据库文件编号,即0, 主数据库文件， 1分数据文件1, ...  FIELD_SERVICE_DB_INDEX
  *     数据文件名(DBDataFileName)                            FIELD_SERVICE_DB_DATA_FILE_NAME
  *     数据文件大小，单位 GB(DBDataFileSize)                 FIELD_SERVICE_DB_DATA_FILE_SIZE
  *     日志文件名(DBLogFileName)                             FIELD_SERVICE_DB_LOG_FILE_NAME
  *     日志文件大小，单位 GB(DBLogFileSize)                  FIELD_SERVICE_DB_LOG_FILE_SIZE
  *     数据库名(DBName)                                      FIELD_SERVICE_DB_NAME
  *     数据库类型，0表示主库，1表示分库(DBType)              FIELD_SERVICE_DB_TYPE
  *     数据库所在磁盘剩余空间，单位为GB(LeftDiskSpace)       FIELD_SERVICE_FREE_DISK_SPACE
  *     数据库所在磁盘剩余空间比(LeftDiskSpaceRate)           FIELD_SERVICE_FREE_DISK_SPACE_RATIO
  *     前一天增长最快的表，CSV格式(TheFastIncTableName)      FIELD_SERVICE_FAST_INC_TABLE
  *     数据库所在磁盘总空间，单位为GB(TotalDiskSpace)        FIELD_SERVICE_TOTAL_DISK_SPACE
  #define     SERVICE_SYSTEM_GET_DB_STATUS                      25030
  */
int CtestMwDlg::Test25030()
{
	theComm.CreateHead(SERVICE_SYSTEM_GET_DB_STATUS);

	while(theComm.More()){

		HZLOG_INFO(STR("AlarmDataSavingTime=%f", theComm.GetDouble(FIELD_SERVICE_ALARM_DATA_ESTIMATE_SVAE_TIME)));
		HZLOG_INFO(STR("DataSavingTime=%f", theComm.GetDouble(FIELD_SERVICE_ALARM_TREND_ESTIMATE_SVAE_TIME)));
		HZLOG_INFO(STR("TrendDataSavingTime=%f", theComm.GetDouble(FIELD_SERVICE_ALARM_DATA_TREND_ESTIMATE_SVAE_TIME)));
		HZLOG_INFO(STR("DBNum=%d",  theComm.GetLong(FIELD_SERVICE_DB_NUM)));
		HZLOG_INFO(STR("DBIndx=%d", theComm.GetLong(FIELD_SERVICE_DB_INDEX)));
		HZLOG_INFO(STR("DBDataFileName=%s", theComm.Get(FIELD_SERVICE_DB_DATA_FILE_NAME)));
		HZLOG_INFO(STR("DBDataFileSize=%f", theComm.GetDouble(FIELD_SERVICE_DB_DATA_FILE_SIZE)));
		HZLOG_INFO(STR("DBLogFileName=%s", theComm.Get(FIELD_SERVICE_DB_LOG_FILE_NAME)));
		HZLOG_INFO(STR("DBLogFileSize=%f", theComm.GetDouble(FIELD_SERVICE_DB_LOG_FILE_SIZE)));
		HZLOG_INFO(STR("DBName=%s", theComm.Get(FIELD_SERVICE_DB_NAME)));
		HZLOG_INFO(STR("DBType=%d", theComm.GetLong(FIELD_SERVICE_DB_TYPE)));
		HZLOG_INFO(STR("LeftDiskSpace=%f", theComm.GetDouble(FIELD_SERVICE_FREE_DISK_SPACE)));
		HZLOG_INFO(STR("FIELD_SERVICE_FREE_DISK_SPACE_RATIO=%f", theComm.GetDouble(FIELD_SERVICE_FREE_DISK_SPACE_RATIO)));
		HZLOG_INFO(STR("TheFastIncTableName=%s", theComm.Get(FIELD_SERVICE_FAST_INC_TABLE)));
		HZLOG_INFO(STR("TotalDiskSpace=%f", theComm.GetDouble(FIELD_SERVICE_TOTAL_DISK_SPACE)));
	}

	return 0;
}


/** 获取中间件运行状态
  *输入参数字段码：无
  *返回值的字段码：(1)
  *            数据库状态             FIELD_SERVICE_DB_STATUS
  *            接收数据的公司名       FIELD_SYSTEM_COMPANY
  *            接收数据的分厂名       FIELD_SYSTEM_FACTORY
  *            接收数据的设备数目     FIELD_SERVICE_DATA_RCV_PLANT_NUM
  *            接收数据的设备状态     FIELD_SERVICE_DATA_RCV_STATUS
  *            保存数据的设备数目     FIELD_SERVICE_DATA_SAVE_PLANT_NUM
  *            保存数据的设备状态     FIELD_SERVICE_DATA_SAVE_STATUS
  *            同步数据的设备数目     FIELD_SERVICE_DATA_SYNC_PLANT_NUM
  *            同步数据的设备状态     FIELD_SERVICE_DATA_SYNC_STATUS
  *
  *\description 类型说明：
  *            数据库状态,     类型 GE_DB_STATUS
  *            接收数据的设备状态，类型 BUFFER, vector<BHMWDataRcvStatus>
  *            保存数据的设备状态，类型 BUFFER, vector<BHMWDataSaveStatus>
  *            同步数据的设备状态，类型 BUFFER, vector<BHMWDataSyncStatus>
  #define     SERVICE_SYSTEM_GET_SERVER_STATUS                  25031
  */
int CtestMwDlg::Test25031()
{
	theComm.CreateHead(SERVICE_SYSTEM_GET_SERVER_STATUS);
	while(theComm.More()){
		HZLOG_INFO(STR("DBStatus==GE_DB_FUNC_OK : %d",  GE_DB_FUNC_OK == theComm.GetLong(FIELD_SERVICE_DB_STATUS)));
		HZLOG_INFO(STR("company=%s", theComm.Get(FIELD_SYSTEM_COMPANY)));
		HZLOG_INFO(STR("factory=%s", theComm.Get(FIELD_SYSTEM_FACTORY)));

		int rcvPlantNum = theComm.GetLong(FIELD_SERVICE_DATA_RCV_PLANT_NUM);
		HZLOG_INFO(STR("rcvPlantNum=%d", rcvPlantNum));

		if(rcvPlantNum > 0){
			vector<BHMWDataRcvStatus> rcvData;
			rcvData.resize(rcvPlantNum);

			theComm.GetItemBuf(FIELD_SERVICE_DATA_RCV_STATUS,
				(char *)&rcvData[0],
				rcvPlantNum * sizeof(BHMWDataRcvStatus));

			for(int i = 0; i < rcvData.size(); i++){
				HZLOG_INFO(STR("PlantName=%s, vib=%d, dyn=%d, proc=%d",
					rcvData[i].plantName,
					rcvData[i].vibStatus,
					rcvData[i].dynStatus,
					rcvData[i].procStatus));
			}
		}

		int savePlantNum = theComm.GetLong(FIELD_SERVICE_DATA_SAVE_PLANT_NUM);
		HZLOG_INFO(STR("rcvSavePlantNum=%d", savePlantNum));

		if(savePlantNum > 0){
			vector<BHMWDataSaveStatus> rcvData;
			rcvData.resize(savePlantNum);

			theComm.GetItemBuf(FIELD_SERVICE_DATA_SAVE_STATUS,
				(char *)&rcvData[0],
				savePlantNum * sizeof(BHMWDataSaveStatus));

			for(int i = 0; i < rcvData.size(); i++){
				HZLOG_INFO(STR("PlantName=%s, trend=%d, alarm=%d, sud=%d",
					rcvData[i].plantName,
					rcvData[i].trendStatus,
					rcvData[i].alarmStatus,
					rcvData[i].sudStatus));
			}
		}

		int syncPlantNum = theComm.GetLong(FIELD_SERVICE_DATA_SYNC_PLANT_NUM);
		HZLOG_INFO(STR("syncPlantNum=%d", syncPlantNum));

		if(syncPlantNum > 0){
			vector<BHMWDataSyncStatus> rcvData;
			rcvData.resize(syncPlantNum);

			theComm.GetItemBuf(FIELD_SERVICE_DATA_SYNC_STATUS,
				(char *)&rcvData[0],
				syncPlantNum * sizeof(BHMWDataSyncStatus));

			int nSize =  rcvData.size();
			for(int i = 0; i < rcvData.size(); i++){
				HZLOG_INFO(STR("PlantName=%s, trend=%d, alarm=%s sud=%s",
					rcvData[i].plantName,
					rcvData[i].trendStatus,
					rcvData[i].alarmStatus,
					rcvData[i].sudStatus));
			}
		}
	}


	return 0;
}


/** 获取数采硬件状态
  *输入参数值：无
  *返回值的字段码：(n)
  *            数采卡索引             FIELD_SERVICE_DAQ_CARD_INDEX
  *            数采卡数量             FIELD_SERVICE_DAQ_CARD_NUM
  *            数采卡逻辑名称         FIELD_SERVICE_DAQ_CARD_NAME
  *            使用通道(CSV)          FIELD_SERVICE_CHANN_BUFF
  *            使用通道CSV串大小      FIELD_SERVICE_CHANN_BUFFSIZE
  *            使用通道个数           FIELD_SERVICE_CHANN_NUM
  *            工作状况(连接|采集)    FIELD_SERVICE_DAQ_CARD_STATUS
  *            获取数采硬件状态时间   FIELD_SERVICE_OFF_TIME
  #define     SERVICE_SYSTEM_GET_DAQ_HW_STATUS                  25032
  */
int CtestMwDlg::Test25032()
{
	theComm.CreateHead(SERVICE_SYSTEM_GET_DAQ_HW_STATUS);
	while (theComm.More()) {
		HZLOG_DEBUG(STR("%d/%d, name=%s ai=%s ci=%s time=%s",
			theComm.GetLong(FIELD_SERVICE_DAQ_CARD_INDEX),
			theComm.GetLong(FIELD_SERVICE_DAQ_CARD_NUM),
			theComm.Get(FIELD_SERVICE_DAQ_CARD_NAME),
			theComm.Get(FIELD_SERVICE_DAQ_USED_AI_CHANNEL),
			theComm.Get(FIELD_SERVICE_DAQ_USED_CI_CHANNEL),
			theComm.Get(FIELD_SERVICE_OFF_TIME)));
		HZLOG_DEBUG(STR("%d/%d status=%s",
			theComm.GetLong(FIELD_SERVICE_DAQ_CARD_INDEX),
			theComm.GetLong(FIELD_SERVICE_DAQ_CARD_NUM),
			theComm.Get(FIELD_SERVICE_DAQ_CARD_STATUS)));
	}

	return 0;
}

/** 获取数采软件状态
  *输入参数值：无
  *返回值的字段码：(1)
  *            版本号                                   FIELD_SERVICE_DAQ_SW_VERSION
  *            采集模式                                 FIELD_SERVICE_DAQ_ACQUIRE_TYPE
  *            启动时间                                 FIELD_SERVICE_TIME
  *            最近一次振动数据发送时间                 FIELD_SERVICE_DAQ_VIB_TIME
  *            最近一次动态数据发送时间                 FIELD_SERVICE_DAQ_DYN_TIME
  *            最近一次过程量数据发送时间               FIELD_SERVICE_DAQ_PROC_TIME
  *            接收间隙电压状态                         FIELD_SERVICE_DAQ_GAP_RCV_STATUS
  *            最近一次断网数据发送开始时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME
  *            最近一次断网数据发送结束时间             FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME
  #define     SERVICE_SYSTEM_GET_DAQ_SW_STATUS                  25033
  */
int CtestMwDlg::Test25033()
{
	theComm.CreateHead(SERVICE_SYSTEM_GET_DAQ_SW_STATUS);
	while (theComm.More()) {
		HZLOG_DEBUG(STR("ver=%s, daqtype=%d starttime=%s vib=%s dyn=%s pro=%s gap=%d off_s=%s off_end=%s time=%s",
			theComm.Get(FIELD_SERVICE_DAQ_SW_VERSION),
			theComm.GetLong(FIELD_SERVICE_DAQ_ACQUIRE_TYPE),
			theComm.Get(FIELD_SERVICE_TIME),
			theComm.Get(FIELD_SERVICE_DAQ_VIB_TIME),
			theComm.Get(FIELD_SERVICE_DAQ_DYN_TIME),
			theComm.GetTime(FIELD_SERVICE_DAQ_PROC_TIME).Format(),
			theComm.GetLong(FIELD_SERVICE_DAQ_GAP_RCV_STATUS),
			theComm.Get(FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME),
			theComm.Get(FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME),
			theComm.Get(FIELD_SERVICE_OFF_TIME)));
	}

	return 0;
}


/** 获取基本的组态信息
  * 输入参数值：无
  * 返回值的字段码：(1)
  *          报警设置(秒数，0表示未设置)                FIELD_SERVICE_CONFIG_ALARM
  *          连接的数据库                               FIELD_SERVICE_CONFIG_CONNECTED_DB
  *          保存数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SAVING
  *          同步数据设置(秒数，0表示未设置)            FIELD_SERVICE_CONFIG_DATA_SYNC
  *          中间件服务器型(0表示装置级，1表示中心级)   FIELD_SERVICE_CONFIG_SERVER_TYPE
  *          统计信息公司数目                           FIELD_SERVICE_COMPANY_NUM
  *          统计信息设备数目                           FIELD_SERVICE_PLANT_NUM
  *          统计信息测点数目                           FIELD_SERVICE_CHANN_NUM
  #define     SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO              25034
*/
int CtestMwDlg::Test25034()
{
	int nRet = -1;
	theComm.CreateHead(SERVICE_SYSTEM_GET_BASIC_CONFIG_INFO);
	while (theComm.More()) {
		HZLOG_DEBUG(STR("AlarmCfg=%d, DB=%s DataSavingCfg=%d DataSyncCfg=%d ServerType=%d CompanyNum=%d PlantNum=%d, ChannNum=%d",
			theComm.GetLong(FIELD_SERVICE_CONFIG_ALARM),
			theComm.Get(FIELD_SERVICE_CONFIG_CONNECTED_DB),
			theComm.GetLong(FIELD_SERVICE_CONFIG_DATA_SAVING),
			theComm.GetLong(FIELD_SERVICE_CONFIG_DATA_SYNC),
			theComm.GetLong(FIELD_SERVICE_CONFIG_SERVER_TYPE),
			theComm.GetLong(FIELD_SERVICE_COMPANY_NUM),
			theComm.GetLong(FIELD_SERVICE_PLANT_NUM),
			theComm.GetLong(FIELD_SERVICE_CHANN_NUM)));

		nRet++;
	}

	return nRet;
}

int CtestMwDlg::Test25019()
{
	/**得到指定机组位号的图结构类型
	*输入参数：
	*             公司名    FIELD_SYSTEM_COMPANY
	*             分厂名    FIELD_SYSTEM_FACTORY
	*             机组号    FIELD_SYSTEM_PLANT
	*返回值的字段码：
	*				机组号          FIELD_SYSTEM_PLANT
	*			    概貌图类型      FIELD_SYSTEM_PLANT_GRAPHTYPE
	*             概貌图BUFF      FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFF
	*             概貌图BUFF尺寸  FIELD_SYSTEM_PLANT_GRAPHTYPE_BUFFSIZE
	#define     SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID          25019
	*/

	int nCount=0,PlantNo=0;
	theComm.CreateHead(SERVICE_SYSTEM_GETGRAPHTYPE_BY_DEVICEID);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD201B往复");

	if (theComm.More())
	{

		PlantNo=theComm.GetLong(FIELD_SYSTEM_PLANT);
		//CString strTemp;
		//strTemp.Format("设备数：%d\n",PlantNum);
		//AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test20007()
{
	/**得到指定两个振动测点的实时波形和频谱。（如何振动测点2不设置参数，则只提取一组）
	*输入参数：
	*          公司ID   FIELD_SYSTEM_COMPANY
	*		   分厂名   FIELD_SYSTEM_FACTORY
	*          机组ID           FIELD_SERVICE_PLANTID
	*          测点buff         FIELD_SERVICE_CHANN_BUFF
	*          测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
	*          测点个数         FIELD_SERVICE_CHANN_NUM
	*          波形点数         FIELD_SERVICE_TIMEWAVE_NUM
	*          分析线数         FIELD_SERVICE_ZOOM_LINES
	*返回值的字段码：
	*          测点名                                FIELD_SERVICE_CHANN1
	*          测点位号                              FIELD_SYSTEM_CHANN_ALIAS
	*          测点的波形数据                        FIELD_SERVICE_TIMEWAVE1
	*          测点的频谱数据                        FIELD_SERVICE_FREQWAVE1
	*          测点实时波形上一组数据的个数          FIELD_SERVICE_TIMEWAVE_NUM
	*          测点实际频谱一组数据的个数            FIELD_SERVICE_FREQWAVE_NUM
	*          振动测点频谱DF                        FIELD_SERVICE_FREQWAVE_DF
	*          所有特征值                            FIELD_SERVICE_CHARVALUE_S
	*          报警值                           FIELD_SYSTEM_ALARM_LOW
	*          危险值                           FIELD_SYSTEM_ALARM_HIGH
	*          单位                             FIELD_SERVICE_CHANN1_UNIT
	*          指定机组测点的转速    FIELD_SERVICE_REV
	*          时间           FIELD_SERVICE_TIME
	#define     SERVICE_SERVICE_ON_GETWAVE                20007
	*/

	int nCount=0;
	float FreqwaveDf;
	int iWaveNum,iWaveNumAll;
	CString strChann="1缸壳体振动V";
	theComm.CreateHead(SERVICE_SERVICE_ON_GETWAVE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),strChann.GetLength());
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,strChann.GetLength());
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SERVICE_TIMEWAVE_NUM,12000);
	theComm.SetItem(FIELD_SERVICE_ZOOM_LINES,401);
	if (theComm.More())
    {
		//float *fWave,*fFreqWave;
		//iWaveNum=theComm.GetLong(FIELD_SERVICE_TIMEWAVE_NUM);
		//fWave=new float[iWaveNum];
		//iWaveNumAll=theComm.GetLong(FIELD_SERVICE_FREQWAVE_NUM);
		//fFreqWave=new float[iWaveNumAll];
		//theComm.GetItemBuf(FIELD_SERVICE_TIMEWAVE1,(char *)fWave,sizeof(float)*iWaveNum);
		//theComm.GetItemBuf(FIELD_SERVICE_FREQWAVE1,(char *)fFreqWave,sizeof(float)*iWaveNumAll);
		//writefile(fWave,iWaveNum,1);
		//delete fWave,fFreqWave;
		FreqwaveDf=theComm.GetLong(FIELD_SERVICE_FREQWAVE_DF);
		float fValue[GD_CUSTOMPARAM_BUFFSIZE];
        theComm.GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)fValue,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			CString sTemp;sTemp.Format("接受:fValue[%d]:%f\n",i,fValue[i]);TRACE(sTemp);
		}
		CString strTemp;strTemp.Format("频谱：%.2f\n",FreqwaveDf);TRACE(strTemp);
		++nCount;
	}
	else
	{
		AfxMessageBox("超时退出");
	}

	return nCount;
}

int CtestMwDlg::Test20024()
{
	/**读取集团级实时报警统计数据
	*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
	*		     分厂名          FIELD_SYSTEM_FACTORY
	*返回值的字段码：
	*		     设备数          FIELD_SERVICE_PLANT_NUM
	*		     报警设备数      FIELD_SERVICE_ALARM_PLANTNUM
	*		     运行设备数      FIELD_SERVICE_RUN_PLANTNUM
	*		     停车设备数      FIELD_SERVICE_STOP_PLANTNUM
	#define     SERVICE_SERVICE_ON_GROUPALARMSTAT             20024
	*/

    int nCount=0;
	int iPlantNum = 0;
	int iAlarmNum = 0;
	int iRunNum,iStopNum;
	theComm.CreateHead(SERVICE_SERVICE_ON_GROUPALARMSTAT);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	while (theComm.More())
	{

		iPlantNum = theComm.GetLong(FIELD_SERVICE_PLANT_NUM);
		iAlarmNum = theComm.GetLong(FIELD_SERVICE_ALARM_PLANTNUM);
		iRunNum=theComm.GetLong(FIELD_SERVICE_RUN_PLANTNUM);
		iStopNum=theComm.GetLong(FIELD_SERVICE_STOP_PLANTNUM);
		CString strTemp;strTemp.Format("%d,%d,%d,%d",iPlantNum,iAlarmNum,iRunNum,iStopNum);AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test20025()
{
	/**读取公司级实时报警统计
	*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*返回值的字段码：
	*		     设备名           FIELD_SERVICE_PLANTID
	*		     测点数           FIELD_SERVICE_CHANN_NUM
	*		     报警状态         FIELD_SERVICE_ALARM_STATUS
	*		     启停车状态       FIELD_SERVICE_SUD_STATUS
	*		     转速1            FIELD_SERVICE_REV
	*		     报警ID           FIELD_SERVICE_ALARMID
	#define     SERVICE_SERVICE_ON_COMPANYALARMSTAT           20025
	*/

	int nCount=0,iChannNum=0,
    float fRev=.0;
	string sPlantID;
	theComm.CreateHead(SERVICE_SERVICE_ON_COMPANYALARMSTAT);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	if (theComm.More())
	{

		sPlantID = theComm.Get(FIELD_SERVICE_PLANTID);
		iChannNum = theComm.GetLong(FIELD_SERVICE_CHANN_NUM);
		fRev=theComm.GetLong(FIELD_SERVICE_REV);
		CString strTemp;strTemp.Format("%s,%d,%.2f",sPlantID.c_str(),iChannNum,fRev);AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test20026()
{
	/**监测站级实时报警统计
	*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*	         设备名           FIELD_SERVICE_PLANTID
	*返回值的字段码：
	*		     测点名           FIELD_SERVICE_CHANN1
	*		     测点位号         FIELD_SYSTEM_CHANN_ALIAS
	*		     特征值           FIELD_SERVICE_CHANN1VALUE
	*		     报警状态         FIELD_SERVICE_CHANN1_STATUS
	*		     转速1            FIELD_SERVICE_REV
	*		     报警ID           FIELD_SERVICE_ALARMID
	#define     SERVICE_SERVICE_ON_MONITORALARMSTAT          20026
	*/

	int nCount=0,iState=0;
	float fRev=.0,fData=.0;
	theComm.CreateHead(SERVICE_SERVICE_ON_MONITORALARMSTAT);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"芳烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD201B");
	if (theComm.More())
	{

		fData = theComm.GetLong(FIELD_SERVICE_CHANN1VALUE);
		fRev=theComm.GetLong(FIELD_SERVICE_REV);
		iState=theComm.GetLong(FIELD_SERVICE_CHANN1_STATUS);
		CString strTemp;strTemp.Format("%.2f,%.2f,%d",fData,fRev,iState);AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test21052()
{
	/**得到指定公司、分厂、指定时间范围的报警信息、启停车信息。
	*振动通道报警次数、过程量通道报警、累计停车次数、累计停机时间、累计运行时间，时间单位为小时
	*输入参数：
	*          公司ID   FIELD_SYSTEM_COMPANY
	*		   分厂名   FIELD_SYSTEM_FACTORY
	*          起始时间：    FIELD_SERVICE_OFF_TIMESTART
	*          终止时间：    FIELD_SERVICE_OFF_TIMEEND
	*返回的字段：
	*          机组ID               FIELD_SERVICE_PLANTID
	*          所有测点报警次数     FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*          累计停车次数         FIELD_SERVICE_LEN
	*          累计停机时间(小时)   FIELD_SERVICE_SAMPLE_FREQ
	*          累计运行时间(小时)   FIELD_SERVICE_SAMPLE_NUM
	#define SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO  21052
	*/
	int nCount=0,iPlantNum=0;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECTIMERANGEFACTORYRUNINFO);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	COleDateTime time1(2008,12,1,0,0,0);//2006-06-15 15:52:56
	COleDateTime time2(2008,12,31,0,0,0);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	while (theComm.More())
	{
        CString sPlant = theComm.GetItem(FIELD_SERVICE_PLANTID);
		int iAlarmNum = theComm.GetLong(FIELD_SERVICE_ON_SEND_VIBCHANNNUM);
		int iStopNum = theComm.GetLong(FIELD_SERVICE_LEN);
		int iStopTime = theComm.GetLong(FIELD_SERVICE_SAMPLE_FREQ);
		int iRunTime = theComm.GetLong(FIELD_SERVICE_SAMPLE_NUM);
		//CString strTemp;
		//strTemp.Format("设备数：%d\n",iPlantNum);
		//AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}


int CtestMwDlg::Test21054()
{
	/**集团级历史报警统计
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
	*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*		     设备数           FIELD_SERVICE_PLANT_NUM
	*		     报警设备数       FIELD_SERVICE_ALARM_PLANTNUM
	#define    SERVICE_SERVICE_OFF_GROUPALARMSTAT             21054
	*/
	int nCount=0,iPlantNum=0;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GROUPALARMSTAT);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"烯烃厂");
	COleDateTime time1(2006,6,1,10,1,1);//2006-06-15 15:52:56
	COleDateTime time2(2008,3,30,10,50,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	if (theComm.More())
	{

		iPlantNum=theComm.GetLong(FIELD_SERVICE_CHANN_NUM);
		//CString strTemp;
		//strTemp.Format("设备数：%d\n",iPlantNum);
		//AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test21055()
{
	/**公司级历史报警统计
	*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
	*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*            分厂名           FIELD_SYSTEM_FACTORY
	*		     设备名           FIELD_SERVICE_PLANTID
	*		     报警次数         FIELD_SERVICE_ALARM_ PLANTNUM
	*		     停车次数         FIELD_SERVICE_STOP_ PLANTNUM
	#define    SERVICE_SERVICE_OFF_COMPANYALARMSTAT         21055
	*/
	int nCount=0,iPlantNum=0;
	theComm.CreateHead(SERVICE_SERVICE_OFF_COMPANYALARMSTAT);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"烯烃厂");
	COleDateTime time1(2006,6,1,10,1,1);//2006-06-15 15:52:56
	COleDateTime time2(2008,3,30,10,50,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	if (theComm.More())
	{

		iPlantNum=theComm.GetLong(FIELD_SERVICE_CHANN_NUM);
		//CString strTemp;
		//strTemp.Format("设备数：%d\n",iPlantNum);
		//AfxMessageBox(strTemp);
		++nCount;
	}
	return nCount;
}

int CtestMwDlg::Test21056()
{
	/**监测站级历史报警统计
	*输入参数值：公司ID          FIELD_SYSTEM_COMPANY
	*		        分厂名           FIELD_SYSTEM_FACTORY
	*	            设备名           FIELD_SERVICE_PLANTID
	*            起始时间：       FIELD_SERVICE_OFF_TIMESTART
	*            终止时间：       FIELD_SERVICE_OFF_TIMEEND
	*返回值的字段码：
	*		        报警ID          FIELD_SERVICE_ALARMID
	*		        测点编号         FIELD_SERVICE_CHANN1
	*		        测点位号         FIELD_SYSTEM_CHANN_ALIAS
	*		        特征值           FIELD_SERVICE_TREND_VALUE
	*		        转速1            FIELD_SERVICE_REV
	*		      报警开始时间       FIELD_SERVICE_OFF_TIMESTART// 报警FIELD_SERVICE_ALARM_TIMESTART
	*		      报警结束时间       FIELD_SERVICE_OFF_TIMEEND
	*		        报警类型         FIELD_SERVICE_ALARM_TYPE
	#define     SERVICE_SERVICE_OFF_MONITORALARMSTAT      21056
	*/

	/**监测站级历史报警统计，优化读取数据，读取20条记录以后的特征值和转速
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*	         设备名           FIELD_SERVICE_PLANTID
	*		     测点编号         FIELD_SERVICE_CHANN1
	*		     报警ID           FIELD_SERVICE_ALARMID
	*返回值的字段码：
	*		     特征值           FIELD_SERVICE_TREND_VALUE
	*		     转速1            FIELD_SERVICE_REV
	#define     SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR       21065
	*/

	/**往复机械报警查询，得到指定报警ID最先报警的测点名、特征值趋势
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*            分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID           FIELD_SERVICE_PLANTID
	*            报警ID           FIELD_SERVICE_ALARMID
	*返回值的字段码：
	*            报警测点名              FIELD_SERVICE_CHANN1
	*            报警测点的特征值结构体  FIELD_SERVICE_CHARVALUE_S
	*            时间                    FIELD_SERVICE_TIME
	*            对应时间的微秒          FIELD_SERVICE_MICROSECOND
	#define     SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES   21068
	*/

	int nCount=0,iRev=0;
	float fValue = .0;
	S_DynCharValue dynCharValue;
	//theComm.CreateHead(SERVICE_SERVICE_OFF_MONITORALARMSTAT);
	//theComm.CreateHead(SERVICE_SERVICE_OFF_MONITORALARMSTATCHAR);21065
    theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECALARMIDCHARVALUES);//21068
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	//theComm.SetItem(FIELD_SERVICE_CHANN1,"1缸杆偏摆量X");
	theComm.SetItem(FIELD_SERVICE_ALARMID,1);
	while (theComm.More())
	{
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &dynCharValue,sizeof(S_DynCharValue));
		for (int i=0;i < 9;i++)
		{
			TRACE("%.2f \n",dynCharValue.GetSpecTypeData(i));
		}
		nCount++;
	}
	return nCount;
}

int CtestMwDlg::Test25112()
{
	/*得到监测中心的设备信息表
	*输入参数：   公司名    FIELD_SYSTEM_COMPANY
	*             分厂名     FIELD_SYSTEM_FACTORY
	*             机组号     FIELD_SERVICE_PLANTID
	*返回的字段码：
	*             测点号     FIELD_SYSTEM_CHANN
	*             索引       FIELD_SYSTEM_STATUS
	*             对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
	*             单位       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE      25112
    */

	/**得到监测中心的信息描述表
	* 输入字段码：
	*返回的字段码：
	*        信息类型      FIELD_SYSTEM_TYPE
	*        对应描述      FIELD_SYSTEM_STATUS_DISCRIPTION
	#define    SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL      25113
	*/

	/**根据测点信息类型,得到监测中心的信息描述表
	* 输入字段码：
	*        信息类型   FIELD_SYSTEM_TYPE
	*返回的字段码：
	*        索引       FIELD_SYSTEM_STATUS
	*        对应描述   FIELD_SYSTEM_STATUS_DISCRIPTION
	*        单位       FIELD_SERVICE_CHANN1_UNIT
	#define    SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE     25114
	*/

	int nCount = 0,iStatus;string sChann;
	//theComm.CreateHead(SERVICE_SYSTEM_GET_SETSVRMACHINEINFOTYPE);
	//theComm.CreateHead(SERVICE_SYSTEM_GET_SETSVRINFOTYPEDETAIL); //TEST25113
    //theComm.CreateHead(SERVICE_SYSTEM_GET_INFOTYPEDETAILBASETYPE); //TEST25114
	theComm.CreateHead(SERVICE_EXPERTSYS_GET_REGALARMFEATUREEXTRACT);//TEST30001
    //theComm.CreateHead(SERVICE_EXPERTSYS_GET_SPECCHANNFEATUREEXTRACT);//TEST30002
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学诊断中心");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"旋转机械");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"DSEWHIP");
	//COleDateTime time1(2009,02,19,10,19,01);
	//COleDateTime time2(2009,02,19,10,19,50);
	//theComm.SetItem(FIELD_SERVICE_CHANN1,"1H");
	//theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	//theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	theComm.SetItem(FIELD_SERVICE_ALARMID,"1");
    while (theComm.More())
    {
		string sTemp = theComm.GetItem(FIELD_SERVICE_EX_RULEID);
		iStatus = theComm.GetLong(FIELD_SERVICE_EX_IDVALUE);
		sChann =theComm.GetItem(FIELD_SERVICE_OFF_CHANN1);
		AfxMessageBox(sChann.c_str());
		/*iStatus = theComm.GetLong(FIELD_SYSTEM_STATUS);
		string sTemp = theComm.GetItem(FIELD_SYSTEM_STATUS_DISCRIPTION);
		string sUnit = theComm.GetItem(FIELD_SERVICE_CHANN1_UNIT);
		CString strTemp;strTemp.Format("%d,%s,%s",iStatus,sTemp.c_str(),sUnit.c_str());AfxMessageBox(strTemp);*/
    }
	return nCount;
}

int CtestMwDlg::Test20015()
{
	/**得到指定任意个动态压力测点示功图信息。循环得到各个测点的数据
	*输入参数值：
	*            公司ID         FIELD_SYSTEM_COMPANY
	*			 分厂名         FIELD_SYSTEM_FACTORY
	*            机组ID         FIELD_SERVICE_PLANTID
	*            测点buff       FIELD_SERVICE_CHANN_BUFF
	*            测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE
	*            测点个数       FIELD_SERVICE_CHANN_NUM
	*            示功图类型     FIELD_SYSTEM_TYPE       P-V,P-a,P-t
	*返回值的字段码：
	*          测点号           FIELD_SERVICE_CHANN1
	*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
	*          波形数据         FIELD_SERVICE_TIMEWAVE1
	*          X坐标数据        FIELD_SERVICE_TIMEWAVE2
	*          所有特征值       FIELD_SERVICE_CHARVALUE_S
	*          采样频率         FIELD_SERVICE_SAMPLE_FREQ
	*          采样点数         FIELD_SERVICE_SAMPLE_NUM
	*          报警值           FIELD_SYSTEM_ALARM_LOW
	*          危险值           FIELD_SYSTEM_ALARM_HIGH
	*          单位             FIELD_SERVICE_CHANN1_UNIT
	*          指定测点的转速   FIELD_SERVICE_REV
	*          时间             FIELD_SERVICE_TIME
	#define   SERVICE_SERVICE_ON_GETPVDATA               20015
	*/

	int nCount = 0,iStatus=0;
	CString strChann="1P,6P";
	theComm.CreateHead(SERVICE_SERVICE_ON_GETPVDATA);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"DSE201");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),5);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,5);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,2);
	theComm.SetItem(FIELD_SYSTEM_TYPE,0);
	if (theComm.More())
	{
		//iStatus = theComm.GetLong(FIELD_SYSTEM_STATUS);
		nCount++;
	}
	return nCount;
}

int CtestMwDlg::Test21057()
{
	/**单通道多特征值报警查询，得到指定报警测点的各种类型特征值
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID           FIELD_SERVICE_PLANTID
	*		     测点编号         FIELD_SERVICE_CHANN1
	*		     报警ID           FIELD_SERVICE_ALARMID
	*返回值的字段码：
	*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
	*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
	*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
	*            时间             FIELD_SERVICE_TIME
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES   21057
	*/
	int nCount = 0,iStatus=0;
	CString strChann1="1H";
	S_VibCharValue vibCharValue;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECCHANNALARMMULTCHARVALUES);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"烯烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201");
	theComm.SetItem(FIELD_SERVICE_CHANN1,strChann1);
	theComm.SetItem(FIELD_SERVICE_ALARMID,1);
	if (theComm.More())
	{
		//COleDateTime tStartTime;
		//tStartTime = theComm.GetTime(FIELD_SERVICE_OFF_TIMESTART);
		//CString sTemp;sTemp.Format("%s",TimeConvertString(tStartTime));
		//AfxMessageBox(sTemp);
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
		CString strTemp;
		strTemp.Format("振动特征值：通频:%.2f,1X:%.2f,2X:%.2f  1X相位:%.2f\n",vibCharValue._fCharAll,vibCharValue._fCharOne,vibCharValue._fCharTwo, vibCharValue._fPhaseOne);
	    AfxMessageBox(strTemp);
	}
	return nCount;
}

int CtestMwDlg::Test21058()
{
	/**多通道单特征值报警查询，得到指定多个报警测点的指定类型特征值
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID           FIELD_SERVICE_PLANTID
	*            报警测点buff     FIELD_SERVICE_CHANN_BUFF
	*            报警测点buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
	*            报警测点个数     FIELD_SERVICE_CHANN_NUM
	*            特征值类型       FIELD_SERVICE_TREND_CHARTYPE
	*		     报警ID           FIELD_SERVICE_ALARMID
	*返回值的字段码：
    *            各个测点特征值Buff   FIELD_SERVICE_TREND_VALUE_BUFF
	*		     报警开始时间     FIELD_SERVICE_OFF_TIMESTART
	*		     报警结束时间     FIELD_SERVICE_OFF_TIMEEND
	*            时间             FIELD_SERVICE_TIME
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE   21058
	*/
	int nCount = 0;
	float fValue=.0;
	CString strChann="1H";
	DATATYPE_CHAR *pfValue=new DATATYPE_CHAR[1];
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECCHANNSALARMSINGLECHARVALUE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"辽阳石化");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"烯烃厂");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"K201");
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),2);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,2);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,0);
	theComm.SetItem(FIELD_SERVICE_ALARMID,1);
	if (theComm.More())
	{
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)pfValue,sizeof(pfValue));
		CString strTemp;
		strTemp.Format("振动特征值：%.2f",pfValue[0]);
		AfxMessageBox(strTemp);
	}
	KD_DeletePoint(pfValue);
	return nCount;
}

int CtestMwDlg::Test21017(void)
{
	/*输入参数字段码：
		*          公司ID        FIELD_SYSTEM_COMPANY
		*		   分厂名        FIELD_SYSTEM_FACTORY
		*          机组ID        FIELD_SERVICE_PLANTID
		*          测点类型：    FIELD_SERVICE_CHANNTYPE
		*          测点buff      FIELD_SERVICE_CHANN_BUFF
		*          测点buff长度  FIELD_SERVICE_CHANN_BUFFSIZE
		*          测点个数      FIELD_SERVICE_CHANN_NUM
		*          起始时间      FIELD_SERVICE_OFF_TIMESTART
		*          终止时间      FIELD_SERVICE_OFF_TIMEEND
		*返回值的字段码：
		*          各个测点特征值Buff   FIELD_SERVICE_CHARVALUE_SBUFF
		*          测点个数             FIELD_SERVICE_CHANN_NUM
		*          时间值               FIELD_SERVICE_OFF_TIME
		*          转速Buff             FIELD_SERVICE_REV_BUFF
      #define     SERVICE_SERVICE_OFF_GETVIBCHANNSTREND         21017
		*/
	int index = -1;
	//theComm.CreateHead(SERVICE_SERVICE_OFF_GETVIBCHANNSTREND);
    theComm.CreateHead(SERVICE_SERVICE_OFF_GETVIBCHANNSTREND);
	CString strChann="1H,1V";
	int nChannNum = 2;
	COleDateTime time1(2009,7,1,14,1,1);
	COleDateTime time2(2010,6,5,19,40,59);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"DSE");
	theComm.SetItem(FIELD_SERVICE_CHANNTYPE,GE_VIBCHANN);
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM, nChannNum);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);
	S_VibCharValue *pValueS = NULL;

	while (theComm.More())
	{
        int retChannNum = theComm.GetLong(FIELD_SERVICE_CHANN_NUM);
        ASSERT(retChannNum == nChannNum);
        pValueS = new S_VibCharValue[retChannNum];
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_SBUFF,
		    (char *)pValueS,
		    retChannNum*sizeof(S_VibCharValue));

		for(int i = 0; i < retChannNum; i++)
		{
			S_VibCharValue vibCharValue = pValueS[i];
			{
			//#if 0
				CString sLog;
				sLog.Format("%d-%d VibCharValue(ac=%f,1c=%f,2c=%f,3c=%f,hc=%f,dc=%f,gap=%f,rc=%f,1p=%f,2p=%f,3p=%f,hp=%f,rev=%d,SmpFreq=%d,SmpNum=%d)",
				index,
				i,
				vibCharValue.GetSpecTypeData(GE_ALLCHAR),
				vibCharValue.GetSpecTypeData(GE_ONECHAR),
				vibCharValue.GetSpecTypeData(GE_TWOCHAR),
				vibCharValue.GetSpecTypeData(GE_THREECHAR),
				vibCharValue.GetSpecTypeData(GE_HALFCHAR),
				vibCharValue.GetSpecTypeData(GE_DCCHAR),
				vibCharValue.GetSpecTypeData(GE_GAP),
				vibCharValue.GetSpecTypeData(GE_RESCHAR),
				vibCharValue.GetSpecTypeData(GE_PHASECHARONE),
				vibCharValue.GetSpecTypeData(GE_PHASECHARTWO),
				vibCharValue.GetSpecTypeData(GE_PHASECHARTHREE),
				vibCharValue.GetSpecTypeData(GE_PHASECHARHALF),
				vibCharValue._iRev,
				vibCharValue._iSmpFreq,
				vibCharValue._iSmpNum
				);
				HZLOG_DEBUG(STR("%s", sLog));
			//#endif
			}
		}

		++index;
	}
    return index;
}

int CtestMwDlg::Test20027()
{
	/**得到指定机组指定一组测点的活塞杆轴心位置轨迹
	*输入参数：
	*          公司ID           FIELD_SYSTEM_COMPANY
	*		   分厂名            FIELD_SYSTEM_FACTORY
	*          机组ID           FIELD_SERVICE_PLANTID
	*          活塞杆位置描述      FIELD_SYSTEM_AXISLOCATION
	*          水平方向测点ID   FIELD_SERVICE_CHANNID_HOR
	*          垂直方向测点ID   FIELD_SERVICE_CHANNID_VER
	*          波形点数          FIELD_SERVICE_TIMEWAVE_NUM
	*返回值的字段码：
	*          水平方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_HOR
	*          垂直方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_VER
	*          实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
	*          水平方向测点特征值  FIELD_SERVICE_CHARVALUE_S
	*          垂直方向测点特征值  FIELD_SERVICE_CHARVALUE_S2
	*          时间                FIELD_SERVICE_TIME
	*		   转速1               FIELD_SERVICE_REV
	#define     SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION         20027
	*/
	int nCount=0;
	CString strChann1;
	float itype=1;
	float fwave,ffreq;
	int iWaveNum = 4096;
	theComm.CreateHead(SERVICE_SERVICE_ON_GETRODSINKAXESLOCATION);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,"2缸杆偏摆量X");
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,"2缸杆沉降量Y");
	theComm.SetItem(FIELD_SERVICE_TIMEWAVE_NUM,iWaveNum);
	DATATYPE_WAVE *pVerAxesWave=NULL;
	DATATYPE_WAVE *pHorAxesWave=NULL;
	pVerAxesWave=new DATATYPE_WAVE[iWaveNum];
	pHorAxesWave=new DATATYPE_WAVE[iWaveNum];
	S_DynCharValue dynCharValue;
	while(theComm.More())
	{
		int iRealWaveNum=theComm.GetLong(FIELD_SERVICE_AXESTRACK_WAVENUM);
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pVerAxesWave,iRealWaveNum*sizeof(DATATYPE_WAVE));
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pHorAxesWave,iRealWaveNum*sizeof(DATATYPE_WAVE));
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &dynCharValue,sizeof(S_DynCharValue));
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *) &dynCharValue,sizeof(S_DynCharValue));
		//for (int i=0;i < iRealWaveNum;i++)
		//{
		//	CString strTemp;strTemp.Format("%d",pHorAxesWave[i]);
		//	AfxMessageBox(strTemp);
		//	TRACE("%d \n",pHorAxesWave[i]);
		//}
		float fX=theComm.GetDouble(FIELD_SERVICE_AXIS_COORX);
		float fY=theComm.GetDouble(FIELD_SERVICE_AXIS_COORY);
		float fHorAxis=theComm.GetDouble(FIELD_SERVICE_HOR_RADIUS);
		float fVerAxis=theComm.GetDouble(FIELD_SERVICE_VER_RADIUS);
		TRACE("fX:%f,fY:%f,fHorAxis:%f,fVerAxis:%f\n",fX,fY,fHorAxis,fVerAxis);
		nCount++;
	}
	return nCount;
}

int CtestMwDlg::Test21059()
{
	/**得到指定机组指定一组测点指定时间的活塞杆轴心位置轨迹
	*输入参数：
	*          公司ID            FIELD_SYSTEM_COMPANY
	*		   分厂名            FIELD_SYSTEM_FACTORY
	*          机组ID            FIELD_SERVICE_PLANTID
	*          轴承位置描述      FIELD_SYSTEM_AXISLOCATION
	*          水平方向测点ID    FIELD_SERVICE_CHANNID_HOR
	*          垂直方向测点ID    FIELD_SERVICE_CHANNID_VER
	*          波形数据个数         FIELD_SERVICE_OFF_WAVE_NUM
	*          时间值          　FIELD_SERVICE_OFF_TIME
	*返回值的字段码：
	*         X方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_HOR
	*         Y方向波形数据    FIELD_SERVICE_AXESTRACK_WAVE_VER
	*         实际波形点数        FIELD_SERVICE_AXESTRACK_WAVENUM
	*         X方向测点特征值  FIELD_SERVICE_CHARVALUE_S
	*         Y方向测点特征值  FIELD_SERVICE_CHARVALUE_S2
	*         转速            FIELD_SERVICE_OFF_REV
	#define     SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION            21059
	*/
	int nCount=0;
	CString strChann1;
	float itype=1;
	int iWaveNum = 4096;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETRODSINKAXESLOCATION);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItem(FIELD_SERVICE_CHANNID_HOR,"2缸杆偏摆量X");
	theComm.SetItem(FIELD_SERVICE_CHANNID_VER,"2缸杆沉降量Y");
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,"2009-07-30 11:43:07");
	theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,iWaveNum);
	DATATYPE_WAVE *pVerAxesWave=NULL;
	DATATYPE_WAVE *pHorAxesWave=NULL;
	pVerAxesWave=new DATATYPE_WAVE[iWaveNum];
	pHorAxesWave=new DATATYPE_WAVE[iWaveNum];
	S_DynCharValue dynCharValue;
	while(theComm.More())
	{ /*
		int iRealWaveNum=theComm.GetLong(FIELD_SERVICE_AXESTRACK_WAVENUM);
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_HOR,(char*)pVerAxesWave,iRealWaveNum*sizeof(DATATYPE_WAVE));
		theComm.GetItemBuf(FIELD_SERVICE_AXESTRACK_WAVE_VER,(char*)pHorAxesWave,iRealWaveNum*sizeof(DATATYPE_WAVE));
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &dynCharValue,sizeof(S_DynCharValue));
		theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S2,(char *) &dynCharValue,sizeof(S_DynCharValue));
		for (int i=0;i < iRealWaveNum;i++)
		{
			//CString strTemp;strTemp.Format("%d",pHorAxesWave[i]);
			//AfxMessageBox(strTemp);
			TRACE("%d \n",pHorAxesWave[i]);
		}*/
		float fX=theComm.GetDouble(FIELD_SERVICE_AXIS_COORX);
		float fY=theComm.GetDouble(FIELD_SERVICE_AXIS_COORY);
		float fHorAxis=theComm.GetDouble(FIELD_SERVICE_HOR_RADIUS);
		float fVerAxis=theComm.GetDouble(FIELD_SERVICE_VER_RADIUS);
		TRACE("fX:%f,fY:%f,fHorAxis:%f,fVerAxis:%f\n",fX,fY,fHorAxis,fVerAxis);
		nCount++;
	}
	return nCount;
}

int CtestMwDlg::Test21060()
{
	/**得到往复式机械设备运行状态历史趋势
	*输入参数值：公司ID               FIELD_SYSTEM_COMPANY
	*		     分厂名               FIELD_SYSTEM_FACTORY
	*            机组ID               FIELD_SERVICE_PLANTID
	*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
	*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
	*            振动测点个数         FIELD_SERVICE_CHANN_NUM
	*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
	*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
	*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
	*            过程量测点buff       FIELD_SERVICE_CHANN_BUFF3
	*            过程量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
	*            过程量测点个数       FIELD_SERVICE_CHANN_NUM3
	*            起始时间             FIELD_SERVICE_OFF_TIMESTART
	*            终止时间             FIELD_SERVICE_OFF_TIMEEND
	*            曲轴箱振动特征值类型 FIELD_SERVICE_TREND_CHARTYPE
	*            缸体振动特征值类型   FIELD_SERVICE_CHANNTYPE
	*            动态压力特征值类型   FIELD_SYSTEM_EVENT_TYPE
	*            活塞杆沉降特征值类型 FIELD_SERVICE_OFF_TRENDTYPE
	*返回值的字段码：
	*            测点特征值Buff       FIELD_SERVICE_CHARVALUE_SBUFF
	*            指定机组测点的转速   FIELD_SERVICE_REV_BUFF
	*            时间值               FIELD_SERVICE_OFF_TIME
	*            测点类型标识         FIELD_SYSTEM_TYPE //0 振动,1动态压力,2 过程量
	#define     SERVICE_SERVICE_OFF_GETHISRUNSTATUS                   21060
	*/
	int index = -1;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETHISRUNSTATUS);
	COleDateTime time1(2008,12,9,14,1,1);
	COleDateTime time2(2008,12,10,19,40,59);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
	theComm.SetItem(FIELD_SERVICE_OFF_TIMESTART,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIMEEND,time2);

	CString strChann="曲轴箱振动1,曲轴箱振动2,1缸壳体振动V";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,3);

	CString strChann2="1缸外动压力P,2缸外动压力P,2缸杆偏摆量X,2缸杆沉降量Y";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF2,(char*)strChann2.GetString(),(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE2,(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM2,4);

	CString strChann3="1缸外吸温度1,1缸外吸温度2,1缸外排温度1";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF3,(char*)strChann3.GetString(),(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE3,(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM3,3);

    theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,GE_ALLCHAR);
    theComm.SetItem(FIELD_SERVICE_CHANNTYPE,GE_ALLCHAR);
    theComm.SetItem(FIELD_SYSTEM_EVENT_TYPE,0);
	theComm.SetItem(FIELD_SERVICE_OFF_TRENDTYPE,0);

	DATATYPE_CHAR *pValueSDyn= NULL;
	pValueSDyn=new DATATYPE_CHAR[4];

	DATATYPE_CHAR *pValueSVib= NULL;
	pValueSVib=new DATATYPE_CHAR[3];

	DATATYPE_CHAR *pfValue=NULL;
	pfValue=new DATATYPE_CHAR[3];
	COleDateTime tCurrTime = COleDateTime::GetCurrentTime();
	CString strTemp;strTemp.Format("%s",TimeConvertString(tCurrTime));
	writeCString(strTemp,"开始读取数据时间");
	while (theComm.More())
	{

		++index;
		int iType = theComm.GetLong(FIELD_SYSTEM_TYPE);
		switch (iType)
		{
		case GE_VIBCHANN:
			{
				theComm.GetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pValueSVib,sizeof(DATATYPE_CHAR)*3);
				DATATYPE_CHAR a = pValueSVib[0];
				DATATYPE_CHAR b = pValueSVib[1];
				DATATYPE_CHAR c = pValueSVib[2];
				break;
			}
		case GE_DYNPRESSCHANN:
			{
				theComm.GetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pValueSDyn,sizeof(DATATYPE_CHAR)*4);
				DATATYPE_CHAR aa = pValueSDyn[0];
				DATATYPE_CHAR bb = pValueSDyn[1];
				DATATYPE_CHAR cc = pValueSDyn[2];
				DATATYPE_CHAR dd = pValueSDyn[3];
				break;
			}
		case GE_ALLPROC:
			{
				theComm.GetItemBuf(FIELD_SERVICE_TREND_VALUE_BUFF,(char *)pfValue,sizeof(DATATYPE_CHAR)*3);
				DATATYPE_CHAR aaa = pfValue[0];
				DATATYPE_CHAR bbb = pfValue[1];
				DATATYPE_CHAR ccc = pfValue[2];
				break;
			}
		}
		//CString strTemp;strTemp.Format("%.2f,%.2f,%.2f",pValueS[0],pValueS[1],pValueS[2]);
		//AfxMessageBox(strTemp);*/
	}
	tCurrTime = COleDateTime::GetCurrentTime();
	strTemp.Format("%s",TimeConvertString(tCurrTime));
	writeCString(strTemp,"读取数据结束时间");
	delete [] pValueSVib;
	delete [] pValueSDyn;
	delete [] pfValue;
	return index;
}

int CtestMwDlg::Test21063()
{
	/**得到往复式机械设备振动通道和动态通道指定时间的特征值
	*输入参数：
	*输入参数值：公司ID           FIELD_SYSTEM_COMPANY
	*		     分厂名           FIELD_SYSTEM_FACTORY
	*            机组ID           FIELD_SERVICE_PLANTID
	*            振动测点buff         FIELD_SERVICE_CHANN_BUFF
	*            振动测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE
	*            振动测点个数         FIELD_SERVICE_CHANN_NUM
	*            动态测点buff         FIELD_SERVICE_CHANN_BUFF2
	*            动态测点buff长度     FIELD_SERVICE_CHANN_BUFFSIZE2
	*            动态测点个数         FIELD_SERVICE_CHANN_NUM2
	*            振动测点时间     FIELD_SERVICE_TIME
	*            动态测点时间     FIELD_SERVICE_OFF_TIME
	*返回值的字段码：
	*            特征值结构体     FIELD_SERVICE_CHARVALUE_S
	*            转速             FIELD_SERVICE_REV
    *            测点类型标识         FIELD_SYSTEM_TYPE //0 振动,1动态压力
	#define     SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE         21063
	*/
	int index = -1;
    theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECRECIPMONTHREPORT);
	//theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECCHANNSCHARVALUE);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"DSE201");

	CString strChann="1H,3H,6H";
	COleDateTime time1(2008,12,01,00,00,00);
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,3);

	CString strChann2="1P,6P,5RX,5RY";
	COleDateTime time2(2008,12,03,00,00,00);
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF2,(char*)strChann2.GetString(),(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE2,(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM2,4);

	theComm.SetItem(FIELD_SERVICE_TIME,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time2);

	S_DynCharValue *pValueSDyn= NULL;
	pValueSDyn=new S_DynCharValue;

	S_VibCharValue *pValueSVib= NULL;
	pValueSVib=new S_VibCharValue;

	while (theComm.More())
	{/*
		++index;
		int iType = theComm.GetLong(FIELD_SYSTEM_TYPE);
		switch (iType)
		{
		case GE_VIBCHANN:
			{
				theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)pValueSVib,sizeof(S_VibCharValue));
				break;
			}
		case GE_DYNPRESSCHANN:
			{
				theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)pValueSDyn,sizeof(S_DynCharValue));
				break;
			}
		}*/
	}
		delete pValueSVib;
		delete pValueSDyn;
		return index;
}

int CtestMwDlg::Test20023()
{
	/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其波形和特征参数
	*如果测点为空就不会返回对应的数据
	*输入参数值：
	*          公司ID                     FIELD_SYSTEM_COMPANY
	*	       分厂名                     FIELD_SYSTEM_FACTORY
	*          机组ID                     FIELD_SERVICE_PLANTID
	*          振动测点buff               FIELD_SERVICE_CHANN_BUFF
	*          振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
	*          振动测点个数               FIELD_SERVICE_CHANN_NUM
	*          动态测点buff               FIELD_SERVICE_CHANN_BUFF2
	*          动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
	*          动态测点个数               FIELD_SERVICE_CHANN_NUM2
	*          活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
	*          活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
	*          活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
	*返回字段：
	*          波形数据的点数             FIELD_SERVICE_TIMEWAVE_NUM
	*          采样频率                   FIELD_SERVICE_SAMPLE_FREQ
	*          测点名称                   FIELD_SERVICE_CHANN1
	*          测点波形                   FIELD_SERVICE_TIMEWAVE1
	*          测点特征值                 FIELD_SERVICE_CHARVALUE_S
	*          横坐标                     FIELD_SERVICE_FREQWAVE2
	*          转速                       FIELD_SERVICE_REV
	*          时间                       FIELD_SERVICE_TIME
	*          吸气阀开启相位角           FIELD_SERVICE_ANGLEPARAM1
	*          吸气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM2
	*          排气阀开启相位角           FIELD_SERVICE_ANGLEPARAM3
	*          排气阀关闭相位角           FIELD_SERVICE_ANGLEPARAM4
	#define   SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA          20023
	*/
	int index = -1;

	theComm.CreateHead(SERVICE_SERVICE_ON_GETRCMULTIPARAMDATA);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD-B3100B");

	CString strChann="1缸壳体振动V";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,3);

	CString strChann2="1缸外动压力P,1缸内动压力P";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF2,(char*)strChann2.GetString(),(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE2,(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM2,2);

	CString strChann3="1缸杆水平量X,1缸杆水平量Y";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF3,(char*)strChann3.GetString(),(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE3,(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM3,2);

	while (theComm.More())
	{
		++index;

		HZLOG_DEBUG(STR("Chann=%s, TimeWaveNum=%d, Time=%s",
					theComm.Get(FIELD_SERVICE_CHANN1),
					theComm.GetLong(FIELD_SERVICE_TIMEWAVE_NUM),
					theComm.Get(FIELD_SERVICE_TIME)
					));
	}

	return index;
}

int CtestMwDlg::Test21037()
{
	/**读取指定多个动态压力测点、活塞杆沉降量测点、振动测点。读取其指定时间波形和特征参数
	*如果测点为空就不会返回对应的数据
	*输入参数值：
	*            公司ID                     FIELD_SYSTEM_COMPANY
	*			 分厂名                     FIELD_SYSTEM_FACTORY
	*            机组ID                     FIELD_SERVICE_PLANTID
	*            振动测点buff               FIELD_SERVICE_CHANN_BUFF
	*            振动测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE
	*            振动测点个数               FIELD_SERVICE_CHANN_NUM
	*            动态测点buff               FIELD_SERVICE_CHANN_BUFF2
	*            动态测点buff长度           FIELD_SERVICE_CHANN_BUFFSIZE2
	*            动态测点个数               FIELD_SERVICE_CHANN_NUM2
	*            活塞杆沉降量测点buff       FIELD_SERVICE_CHANN_BUFF3
	*            活塞杆沉降量测点buff长度   FIELD_SERVICE_CHANN_BUFFSIZE3
	*            活塞杆沉降量测点个数       FIELD_SERVICE_CHANN_NUM3
	*            时间                       FIELD_SERVICE_OFF_TIME
	*            波形数据个数               FIELD_SERVICE_OFF_WAVE_NUM
	*返回字段：
	*            波形数据的点数               FIELD_SERVICE_TIMEWAVE_NUM
	*            采样频率                     FIELD_SERVICE_SAMPLE_FREQ
	*            测点名称                     FIELD_SERVICE_CHANN
	*            测点波形                     FIELD_SERVICE_TIMEWAVE
	*            测点特征值                   FIELD_SERVICE_CHARVALUE_S
	*            横坐标                       FIELD_SERVICE_FREQWAVE2
	*            转速                         FIELD_SERVICE_REV
	*            吸气阀开启相位角             FIELD_SERVICE_ANGLEPARAM1
	*            吸气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM2
	*            排气阀开启相位角             FIELD_SERVICE_ANGLEPARAM3
	*            排气阀关闭相位角             FIELD_SERVICE_ANGLEPARAM4
	#define   SERVICE_SERVICE_OFF_GETRCMULTIPARAMDATA           21037
	*/

	/**得到指定历史时刻、机组活塞杆沉降量测点的波形和频谱。
	*输入参数：
	*          公司ID           FIELD_SYSTEM_COMPANY
	*		    分厂名           FIELD_SYSTEM_FACTORY
	*          机组ID           FIELD_SERVICE_PLANTID
	*          测点             FIELD_SERVICE_OFF_CHANN1
	*          时间             FIELD_SERVICE_OFF_TIME
	*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
	*返回值的字段码：
	*          测点的波形数据   FIELD_SERVICE_OFF_WAVE
	*          频谱数据         FIELD_SERVICE_OFF_FREQWAVE
	*          波形数据个数     FIELD_SERVICE_OFF_WAVE_NUM
	*          频谱DF           FIELD_SERVICE_OFF_FREQWAVE_DF
	*          频谱数据点数     FIELD_SERVICE_OFF_FREQWAVE_NUM
	*          特征值结构体     FIELD_SERVICE_CHARVALUE_S
	*          报警值           FIELD_SYSTEM_ALARM_LOW
	*          危险值           FIELD_SYSTEM_ALARM_HIGH
	*          测点别名         FIELD_SYSTEM_CHANN_ALIAS
	*          单位             FIELD_SERVICE_CHANN1_UNIT
	*          转速             FIELD_SERVICE_REV
	*          吸气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM1
	*          吸气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM2
	*          排气阀开启相位角                      FIELD_SERVICE_ANGLEPARAM3
	*          排气阀关闭相位角                      FIELD_SERVICE_ANGLEPARAM4
	*          柱状波形报警上限                      FIELD_SERVICE_ALARM_LOW
	*          柱状波形报警上上限                    FIELD_SERVICE_ALARM_HIGH
	*          测点起始索引                          FIELD_SERVICE_STARTINDEX
	*          测点结束索引                          FIELD_SERVICE_ENDINDEX
	*          自定义参数数据BUFF                    FIELD_SERVICE_LEARNPARAM_BUFF
	#define     SERVICE_SERVICE_OFF_GETRODSINKDATA              21036
	*/

	int nCount = 0;
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETALARMRCMULTIPARAMDATA);
	COleDateTime time1(2009,12,24,17,47,54);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD-B3100B");

	CString strChann="1缸壳体振动V";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,3);

	CString strChann2="1缸杆水平量X,1缸外动压力P";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF2,(char*)strChann2.GetString(),(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE2,(strChann2.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM2,2);
	theComm.SetItem(FIELD_SYSTEM_EVENT_ID,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time1);
    /*
	CString strChann3="2缸杆偏摆量X,2缸杆沉降量Y";
	theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF3,(char*)strChann3.GetString(),(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE3,(strChann3.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM3,2);

	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,1024);*/

	/*
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETRODSINKDATA);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"北京化工大学");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"化机楼");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"BH5000R");
    theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"2缸杆偏摆量X");
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time1);
	theComm.SetItem(FIELD_SERVICE_OFF_WAVE_NUM,12000);*/
	while(theComm.More())
	{
		string sChann=theComm.GetItem(FIELD_SERVICE_CHANN1);
		CString strChann=sChann.c_str();
		if (sChann=="1缸壳体振动V")
		{
			S_VibCharValue vibCharValue;
			theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *)&vibCharValue,sizeof(S_VibCharValue));
			for (int i=0;i < 9;i++)
			{
				TRACE("%.2f \n",vibCharValue.GetSpecTypeData(i));
			}
		}

        /*
		float *fValue=new float[GD_CUSTOMPARAM_BUFFSIZE];
		theComm.GetItemBuf(FIELD_SERVICE_LEARNPARAM_BUFF,(char *)fValue,GD_CUSTOMPARAM_BUFFSIZE*sizeof(float));
		for (int i=0;i<GD_CUSTOMPARAM_BUFFSIZE;i++)
		{
			CString sTemp;sTemp.Format("接受:fValue[%d]:%f\n",i,fValue[i]);TRACE(sTemp);
		}
		nCount++;
		KD_DeletePoint(fValue);*/
	}
	return nCount;
}

int CtestMwDlg::Test24030(void)
{
	srand(time(0));

	int nCount=4;

	for(int i = 0; i < nCount; ++i){

		theComm.CreateHead(SERVICE_SERVICE_ON_RCV_DAQ_HW_STATUS);

		theComm.SetItem(FIELD_SERVICE_DAQ_CARD_INDEX,i);
		theComm.SetItem(FIELD_SERVICE_DAQ_CARD_NUM,nCount);
		theComm.SetItem(FIELD_SERVICE_DAQ_CARD_NAME,STR("数采卡%d", i+1).c_str());
		theComm.SetItem(FIELD_SERVICE_DAQ_USED_AI_CHANNEL, "1,2");
		theComm.SetItem(FIELD_SERVICE_AI_CHANN_NUM, 2);
		theComm.SetItem(FIELD_SERVICE_DAQ_USED_CI_CHANNEL, "4,5,6");
		theComm.SetItem(FIELD_SERVICE_CI_CHANN_NUM, 3);
		theComm.SetItem(FIELD_SERVICE_DAQ_CARD_STATUS,"正常采集");

		while(theComm.More())
		{
			TRACE("Status=%d\n", theComm.GetLong(FIELD_SYSTEM_STATUS));
		}
	}

	return 0;
}

int CtestMwDlg::Test24031(void)
{
	theComm.CreateHead(SERVICE_SERVICE_ON_RCV_DAQ_SW_STATUS);

	theComm.SetItem(FIELD_SERVICE_DAQ_SW_VERSION,"4.0.7.5");
	theComm.SetItem(FIELD_SERVICE_DAQ_ACQUIRE_TYPE, 0);
	theComm.SetItem(FIELD_SERVICE_TIME, COleDateTime::GetCurrentTime());
	theComm.SetItem(FIELD_SERVICE_DAQ_OFFLINE_SAVE_BEGIN_TIME, COleDateTime::GetCurrentTime());
	theComm.SetItem(FIELD_SERVICE_DAQ_OFFLINE_SAVE_END_TIME, COleDateTime::GetCurrentTime());
	theComm.SetItem(FIELD_SERVICE_DAQ_GAP_RCV_STATUS, 3);

	while(theComm.More())
	{
		TRACE("Status=%d\n", theComm.GetLong(FIELD_SYSTEM_STATUS));
	}

	return 0;
}

int CtestMwDlg::test20014(void)
{
	// 得到指定设备的所有测点特征值，振动测点需要指定测点特征类型
	/**得到指定机组所有测点特征值，通过循环依次得到所有值。注：对于振动测点来说，指定那种特征类型就给那一种
	*输入参数值：
	*            公司ID         FIELD_SYSTEM_COMPANY
	*			 分厂名         FIELD_SYSTEM_FACTORY
	*            特征值类型     FIELD_SERVICE_TREND_CHARTYPE
	*            设备号buff     FIELD_SERVICE_CHANN_BUFF
	*            设备号buff长度 FIELD_SERVICE_CHANN_BUFFSIZE
	*            设备号个数     FIELD_SERVICE_CHANN_NUM
	*返回值的字段码：
	*            设备ID         FIELD_SERVICE_PLANTID
	*            测点ID         FIELD_SERVICE_CHANN1
	*            测点值         FIELD_SERVICE_CHANN1VALUE
	*            测点单位       FIELD_SERVICE_CHANN1_UNIT
	*            测点所属列表   FIELD_SERVICE_CHANN1_LIST
	*            测点的状态     FIELD_SERVICE_CHANN1_STATUS
	*            测点特征值      FIELD_SERVICE_CHARVALUE_S
	*            测点类型       FIELD_SERVICE_CHANNTYPE
	*            转速1           FIELD_SERVICE_REV
	*            转速2           FIELD_SERVICE_REV2
	*            时间           FIELD_SERVICE_TIME
	#define     SERVICE_SERVICE_ON_GETALLCHANNVALUE    20014
	*/

	/**得到指定机组所有轴位移测点值，通过循环依次得到所有轴位移值
	*输入参数值：设备ID         FIELD_SERVICE_PLANTID
	*返回值的字段码：
	*            轴位移值       FIELD_SERVICE_TREND_AXISDISPLACE
	*            测点ID         FIELD_SERVICE_CHANN
	*define     SERVICE_SERVICE_ON_GETALLAXISDISPLACE     20012
	*/
	int nCount=0;
	CString strChann="KD-B3100B";
    theComm.CreateHead(SERVICE_SERVICE_OFF_GETSPECTIMEALLCHANNVALUE);
    //theComm.CreateHead(SERVICE_SERVICE_ON_GETALLCHANNVALUE);
    theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
    theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
    theComm.SetItem(FIELD_SERVICE_TREND_CHARTYPE,0);
    theComm.SetItemBuf(FIELD_SERVICE_CHANN_BUFF,(char*)strChann.GetString(),(strChann.GetLength()+1));
	theComm.SetItem(FIELD_SERVICE_CHANN_BUFFSIZE,strChann.GetLength()+1);
	theComm.SetItem(FIELD_SERVICE_CHANN_NUM,1);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,"2009-12-24 20:44:51");//20:34:44
	while(theComm.More())
	{
		int iAlarmType=theComm.GetLong(FIELD_SERVICE_CHANN1_STATUS);
		string sChann=theComm.GetItem(FIELD_SERVICE_CHANN1);
		TRACE("sChann:%s,iAlarmType:%d\n",sChann.c_str(),iAlarmType);
		float fValue=.0;
		fValue=theComm.GetDouble(FIELD_SERVICE_CHANN1VALUE);
		TRACE("%.2f \n",fValue);
		/*
        int iChannType=theComm.GetLong(FIELD_SERVICE_CHANNTYPE);
		S_DynCharValue dynCharValue;
		if (iChannType==GE_DYNPRESSCHANN)
		{
			theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &dynCharValue,sizeof(S_DynCharValue));
			for (int i=0;i < 9;i++)
			{
				TRACE("%.2f \n",dynCharValue.GetSpecTypeData(i));
			}
		}
		S_VibCharValue vibCharValue;
		if (iChannType==GE_VIBCHANN)
		{
			theComm.GetItemBuf(FIELD_SERVICE_CHARVALUE_S,(char *) &vibCharValue,sizeof(S_VibCharValue));
			for (int i=0;i < 9;i++)
			{
				TRACE("%.2f \n",vibCharValue.GetSpecTypeData(i));
			}
		}
		float fValue=.0;
		if (iChannType==GE_PRESSCHANN || iChannType==GE_TEMPCHANN || iChannType==GE_FLUXCHANN || iChannType==GE_IMPACTCHANN)
		{
			fValue=theComm.GetDouble(FIELD_SERVICE_CHANN1VALUE);
			TRACE("%.2f \n",fValue);
		}
		*/
		 nCount++;
	}
	return nCount;

}


int CtestMwDlg::Test20028()
{
	/**往复式系统应力监测，得到指定机组指定缸号的应力波形和振动测点波形
	*输入参数：
	*          公司ID        　　　    FIELD_SYSTEM_COMPANY
	*		   分厂名          　　　  FIELD_SYSTEM_FACTORY
	*          机组ID          　　　  FIELD_SERVICE_PLANTID
	*          汽缸号                  FIELD_SERVICE_CYLINDERNO
	*          缸体振动测点            FIELD_SERVICE_CHANN1
	*返回值的字段码：
	*          气体应力波形数据　　　　FIELD_SERVICE_TIMEWAVE1
	*          惯性应力波形数据　　　　FIELD_SERVICE_TIMEWAVE2
	*          活塞杆应力波形数据　　　FIELD_SERVICE_FREQWAVE1
	*          振动波形数据   　　　　 FIELD_SERVICE_FREQWAVE2
	*          波形数据个数   　　　　 FIELD_SERVICE_TIMEWAVE_NUM
	#define     SERVICE_SERVICE_OFF_GETDYNSTRESSWAVE              20028
	*/
	int index = -1;
	//theComm.CreateHead(SERVICE_SERVICE_ON_GETDYNSTRESSWAVE);
	theComm.CreateHead(SERVICE_SERVICE_OFF_GETALARMDYNSTRESSWAVE);
	COleDateTime time1(2010,2,6,9,9,6);
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"KD-B3100B");
	theComm.SetItem(FIELD_SERVICE_CYLINDERNO,1);
	//theComm.SetItem(FIELD_SERVICE_CHANN1,"1缸壳体振动V");
	//theComm.SetItem(FIELD_SERVICE_OFF_CHANN1,"1缸壳体振动V");
    theComm.SetItem(FIELD_SYSTEM_EVENT_ID,40);
	theComm.SetItem(FIELD_SERVICE_OFF_TIME,time1);
	while (theComm.More())
	{
		++index;
		//CString strTemp;strTemp.Format("%.2f,%.2f,%.2f",pValueS[0],pValueS[1],pValueS[2]);
		//AfxMessageBox(strTemp);*/
	}
	return index;

}

/** 获取异常数据检测结果
  *输入参数字段码：无
  *返回值的字段码：(n)
  *            公司ID                 FIELD_SYSTEM_COMPANY
  *            分厂名                 FIELD_SYSTEM_FACTORY
  *            机组ID                 FIELD_SERVICE_PLANTID
  *            测点                   FIELD_SYSTEM_CHANN
  *            异常时间               FIELD_SERVICE_TIME
  *            异常类别               FIELD_SYSTEM_TYPE
  *            异常原因               FIELD_SYSTEM_STATUS_DISCRIPTION
  *\description
               异常类别(FIELD_SYSTEM_TYPE)字段类型： GE_ABNORMAL_DATA_CATEGORY
                                     包括(越界数据、趋势波动的转速，不合理数据)
#define     SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD       20032
  */
int CtestMwDlg::Test20032()
{
	theComm.CreateHead(SERVICE_SERVICE_ON_GET_ABNORMAL_DATA_RECORD);
	int errorCount = 0;
	while(theComm.More())
	{
		int errorType = theComm.GetLong(FIELD_SYSTEM_TYPE);

		HZLOG_INFO(STR("Error %s(%d) occurred for Chann %s on Time(%s) Company(%s) Factory(%s) Plant(%s) 原因:%s",
			errorType==GE_DATA_BOUNDS_EXCEEDED?"数据越界":errorType==GE_DATA_TREND_FLUCTUATING?"波动":"不合理",
			errorType,
			theComm.Get(FIELD_SYSTEM_CHANN),
			theComm.Get(FIELD_SERVICE_TIME),
			theComm.Get(FIELD_SYSTEM_COMPANY),
			theComm.Get(FIELD_SYSTEM_FACTORY),
			theComm.Get(FIELD_SERVICE_PLANTID),
			theComm.Get(FIELD_SYSTEM_STATUS_DISCRIPTION)
			));

		errorCount++;
	}

	return errorCount;
}

int CtestMwDlg::Test24050()
{
	/**根据指定设备得到指定索引和长度的振动波形数据
	*输入参数值：
	*            公司ID   FIELD_SYSTEM_COMPANY
	*			 分厂名   FIELD_SYSTEM_FACTORY
	*            机组ID    FIELD_SERVICE_PLANTID
	*            起始索引  FIELD_SERVICE_ON_SEND_STARTID
	*            长度      FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            目前数据的时间 FIELD_SERVICE_TIME
	*返回参数值：
	*            振动测点特征值数据      FIELD_SERVICE_ON_SEND_ALLCHARVALUE
	*            起始索引                FIELD_SERVICE_ON_SEND_STARTID
	*            振动测点个数            FIELD_SERVICE_ON_SEND_VIBCHANNNUM
	*            波形数据                FIELD_SERVICE_ON_SEND_VIBWAVE
	*            总的波形点数            FIELD_SERVICE_ON_SEND_WAVEPOINTNUM
	*            数据的时间              FIELD_SERVICE_TIME
	#define     SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA     24050
	*/
	int index = -1;
	//theComm.CreateHead(SERVICE_SERVICE_ON_GETSPECPLANTLENVIBDATA);
	theComm.CreateHead(SERVICE_SERVICE_ON_GETSPECPLANTLENDYNDATA);
	COleDateTime time1=COleDateTime::GetCurrentTime();
	theComm.SetItem(FIELD_SYSTEM_COMPANY,"华油");
	theComm.SetItem(FIELD_SYSTEM_FACTORY,"大张坨");
	theComm.SetItem(FIELD_SERVICE_PLANTID,"1#");
	theComm.SetItem(FIELD_SERVICE_ON_SEND_STARTID,0);
	theComm.SetItem(FIELD_SERVICE_ON_SEND_VIBCHANNNUM,19);
	theComm.SetItem(FIELD_SERVICE_TIME,time1);
	while (theComm.More())
	{
		++index;
		int iWaveNum=theComm.GetLong(FIELD_SERVICE_ON_SEND_WAVEPOINTNUM);
		DATATYPE_WAVE *pWave=new DATATYPE_WAVE[iWaveNum];
		theComm.GetItemBuf(FIELD_SERVICE_ON_SEND_VIBWAVE,(char*)pWave,iWaveNum*sizeof(DATATYPE_WAVE));
		//writefile(pWave,iWaveNum,444);
		//writefile(pWave,iWaveNum,333);
	}
	return index;
}

CString CtestMwDlg::TimeConvertString(const COleDateTime &time_)
{
	CString strTmp;
	strTmp.Format("%d-%d-%d %d:%d:%d",time_.GetYear(),time_.GetMonth(),time_.GetDay(),time_.GetHour(),time_.GetMinute(),time_.GetSecond());
	return strTmp;
}

void CtestMwDlg::writeCString(CString strMsg , CString filename)
{
	ofstream fileWave;
	char filenm[100]={0};
	CString strtmp;
	strtmp.Format("c:\\testMw\\log\\%s.txt",filename);
	memcpy(filenm,strtmp,strtmp.GetLength());
	fileWave.open(filenm, ios::app | ios::out);
	fileWave<< "  log : "<<(LPCSTR)strMsg;
}

void CtestMwDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CtestMwDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

void CtestMwDlg::OnBnClickedButtonLink()
{
	// TODO: 在此添加控件通知处理程序代码

	string serverIP = "192.168.1.109";
	int serverPort =  7001;

	string linkString = STR("%s,%d/tcp", serverIP.c_str(), serverPort);

	SetDlgItemText(IDC_LABEL_SERVER, getStringFormattedAs(
		"服务器=%s, 端口=%d, 连接中...",
		serverIP.c_str(), serverPort).c_str());

	if(theComm.Link(linkString.c_str(),"")==FALSE)
	{
		SetDlgItemText(IDC_LABEL_SERVER, getStringFormattedAs(
			"服务器=%s, 端口=%d, 连接失败",
			serverIP.c_str(), serverPort).c_str());
	}
	else
	{
		SetDlgItemText(IDC_LABEL_SERVER, getStringFormattedAs(
			"服务器=%s, 端口=%d, 连接成功",
			serverIP.c_str(), serverPort).c_str());
	}
}

