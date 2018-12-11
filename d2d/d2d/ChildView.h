
// ChildView.h : CChildView 類別的介面
//


#pragma once
#include "D2dPrerequisite.h"

// CChildView 視窗

class CChildView : public CWnd
{
// 建構
public:
	CChildView();

// 屬性
public:

// 作業
public:
private:
	//Direct2D interface
	ID2D1Factory* m_pD2d1Factory;
	ID2D1HwndRenderTarget* m_pHwndRenderTarget;
	ID2D1SolidColorBrush* m_pSolidColorBrush;
	ID2D1LinearGradientBrush* m_pLinearGradientBrush;
	ID2D1RadialGradientBrush* m_pRadialGradientBrush;
	//DirectWrite interface
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;

	BOOL CreateDeviceIndependentResource();
	BOOL CreateDeviceDependentResource();
	void Render();
	void DiscardDeviceDependentResource();
// 覆寫
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 程式碼實作
public:
	virtual ~CChildView();

	// 產生的訊息對應函式
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

