
// ChildView.h : CChildView ���O������
//


#pragma once
#include "D2dPrerequisite.h"

// CChildView ����

class CChildView : public CWnd
{
// �غc
public:
	CChildView();

// �ݩ�
public:

// �@�~
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
// �мg
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �{���X��@
public:
	virtual ~CChildView();

	// ���ͪ��T�������禡
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

