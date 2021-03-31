// TestAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestAnalysis.h"
#include "TestAnalysisDlg.h"
#include ".\testanalysisdlg.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "Calc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestAnalysisDlg 对话框



CTestAnalysisDlg::CTestAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAnalysisDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestAnalysisDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_BTN_TEST, OnBnClickedBtnTest)
END_MESSAGE_MAP()


// CTestAnalysisDlg 消息处理程序

BOOL CTestAnalysisDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestAnalysisDlg::OnPaint() 
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

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CTestAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestAnalysisDlg::OnBnClickedBtnTest()
{
  TestSpectrum();
}

// test spectrum analysis
void CTestAnalysisDlg::TestSpectrum(void)
{
  float fAmp=2;
  int iSmpNum=655;
  float *pfWave=new float[iSmpNum];
  float *pfSpec=new float[iSmpNum/2];
  for (int nloop=0;nloop<iSmpNum;++nloop)
  {
	  pfWave[nloop]=fAmp*sin(2*3.1415*nloop/32)+fAmp*sin(2*3.1415*nloop/16);
  }
  writedata( pfWave, iSmpNum, 1);
  SpectralMeasureMent(pfWave,iSmpNum,pfSpec,EA_PEAK_PEAK_SPEC);
  writedata( pfSpec, iSmpNum/2, 2);
  for(int nloop=0;nloop<iSmpNum/2;++nloop)
	  TRACE("%d:%.2f\n",nloop,pfSpec[nloop]);
  delete [] pfSpec;
  delete [] pfWave;

}

void CTestAnalysisDlg::writedata(float *pdata,int n,int index_)
{
	std::fstream filetmp;
	CString strtmp;
	strtmp.Format("d:\\tmp%d.txt", index_);

	filetmp.open( strtmp, std::ios_base::out);
	for(int i=0;i<n;++i)
	{
		strtmp.Format("%.4f",pdata[i]);
		filetmp <<strtmp<<"	";
	}
	filetmp.close();
}
