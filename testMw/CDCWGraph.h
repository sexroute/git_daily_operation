// CDCWGraph.h : 由 Microsoft Visual C++ 创建的 ActiveX 控件包装类的声明

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDCWGraph

class CDCWGraph : public CWnd
{
protected:
	DECLARE_DYNCREATE(CDCWGraph)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xB68DBFAB, 0x16A0, 0x11CE, { 0x80, 0xBF, 0x0, 0x20, 0xAF, 0x31, 0xCE, 0xF9 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:

// 操作
public:

	LPDISPATCH get_Plots()
	{
		LPDISPATCH result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Axes()
	{
		LPDISPATCH result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void PlotXvsY(VARIANT xData, VARIANT YData, VARIANT bPlotPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &xData, &YData, &bPlotPerRow);
	}
	void PlotY(VARIANT YData, VARIANT xFirst, VARIANT xInc, VARIANT bPlotPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &YData, &xFirst, &xInc, &bPlotPerRow);
	}
	void PlotXY(VARIANT XYData, VARIANT bPlotPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &XYData, &bPlotPerRow);
	}
	LPDISPATCH get_Cursors()
	{
		LPDISPATCH result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void ChartY(VARIANT YData, VARIANT xInc, VARIANT bChartPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &YData, &xInc, &bChartPerRow);
	}
	void ChartXY(VARIANT XYData, VARIANT bChartPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &XYData, &bChartPerRow);
	}
	void ChartXvsY(VARIANT xData, VARIANT YData, VARIANT bChartPerRow)
	{
		static BYTE parms[] = VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &xData, &YData, &bChartPerRow);
	}
	void ClearData()
	{
		InvokeHelper(0x13, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ImportStyle(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x202, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void ExportStyle(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x201, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	LPDISPATCH Images(VARIANT Item)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Item);
		return result;
	}
	LPDISPATCH ControlImage()
	{
		LPDISPATCH result;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_CWBindings()
	{
		LPDISPATCH result;
		InvokeHelper(0x115, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Annotations()
	{
		LPDISPATCH result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Refresh()
	{
		InvokeHelper(DISPID_REFRESH, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}


};
