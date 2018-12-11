
// ChildView.cpp : CChildView 類別的實作
//

#include "stdafx.h"
#include "d2d.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace D2D1;

// CChildView

CChildView::CChildView()
{
	m_pD2d1Factory = NULL;
	m_pDCRenderTarget = NULL;
	m_pSolidColorBrush = NULL;
	m_pLinearGradientBrush = NULL;
	m_pRadialGradientBrush = NULL;
	m_pDWriteFactory = NULL;
	m_pTextFormat = NULL;
	CreateDeviceIndependentResource();
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView 訊息處理常式

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), NULL);

	return TRUE;
}

//https://www.cnblogs.com/wdhust/archive/2010/04/28/1723122.html

CStringW str1, str2;
void CChildView::OnPaint()
{
	CPaintDC dc(this); // 繪製的裝置內容

	// TODO: 在此加入您的訊息處理常式程式碼
	CString tmp;
	DWORD st = GetTickCount();
	CreateDeviceDependentResource();
	str1.Format(L"d2d CreateDeviceDependentResource:%u ms\n", GetTickCount() - st);
	OutputDebugStringW(str1);
	st = GetTickCount();
	RECT rc;
	GetClientRect(&rc);
	m_pDCRenderTarget->BindDC(dc.GetSafeHdc(), &rc);
	Render();
	str2.Format(L"d2d Render:%u ms\n",GetTickCount() - st);
	OutputDebugStringW(str2);
	// 不要呼叫描繪訊息的 CWnd::OnPaint()

}

BOOL CChildView::CreateDeviceIndependentResource()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2d1Factory);
	ASSERT(hr == S_OK);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
		ASSERT(hr == S_OK);
	}

	//Create TextFormat object with IDWriteFactory
	if (SUCCEEDED(hr))
	{
		const CStringW fontName(_T("微軟正黑體"));
		const FLOAT fontSize = 32.0f;
		hr = m_pDWriteFactory->CreateTextFormat(
			fontName,
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,
			L"",	//locale
			&m_pTextFormat
		);
		ASSERT(hr == S_OK);
		if (SUCCEEDED(hr))
		{
			//Center alignment vertically and horizontally
			m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		}
	}
	return (hr == S_OK);
}

BOOL CChildView::CreateDeviceDependentResource()
{
	ASSERT(m_pD2d1Factory != NULL);
	if (m_pDCRenderTarget != NULL)	//There is no need to create render target
		return TRUE;

	RECT rc;
	GetClientRect(&rc);
	D2D1_SIZE_U size = SizeU(rc.right - rc.left, rc.bottom - rc.top);
	//HRESULT hr = m_pD2d1Factory->CreateHwndRenderTarget(
	//	RenderTargetProperties(),
	//	HwndRenderTargetProperties(m_hWnd, size),//Bind the HwndRenderTarget to view window
	//	&m_pHwndRenderTarget);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE),
		0,
		0,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);
	HRESULT hr = m_pD2d1Factory->CreateDCRenderTarget(
		&props,
		&m_pDCRenderTarget);
	ASSERT(hr == S_OK);
	if (SUCCEEDED(hr))
	{
		//Create solid color brush
		hr = m_pDCRenderTarget->CreateSolidColorBrush(
			ColorF(ColorF::LightGreen),
			&m_pSolidColorBrush);
		ASSERT(hr == S_OK);
		//Create gradient stops collection,
		//used by linear gradient brush or radial gradient brush
		ID2D1GradientStopCollection* pGradientStops = NULL;
		D2D1_GRADIENT_STOP stops[2];
		stops[0].color = ColorF(ColorF::Yellow);
		stops[0].position = 0.0f;
		stops[1].color = ColorF(ColorF::Red);
		stops[1].position = 1.0f;
		HRESULT hr = m_pDCRenderTarget->CreateGradientStopCollection(
			stops,
			2,
			D2D1_GAMMA_2_2,
			D2D1_EXTEND_MODE_CLAMP,
			&pGradientStops);
		ASSERT(hr == S_OK);

		//Create linear gradient brush
		hr = m_pDCRenderTarget->CreateLinearGradientBrush(
			LinearGradientBrushProperties(Point2F(210, 110), Point2F(290, 190)),
			pGradientStops,
			&m_pLinearGradientBrush);
		ASSERT(hr == S_OK);
		//Create radial gradient brush
		hr = m_pDCRenderTarget->CreateRadialGradientBrush(
			RadialGradientBrushProperties(Point2F(350, 150), Point2F(0, 0), 50, 50),
			pGradientStops,
			&m_pRadialGradientBrush);
		ASSERT(hr == S_OK);
		SafeRelease(&pGradientStops);
	}
	return (hr == S_OK);
}

void CChildView::Render()
{
	ASSERT(m_pD2d1Factory != NULL);
	if (!m_pDCRenderTarget)	//Render target need to be recreated
	{
		//Recreate device dependent resource
		BOOL succeeded = CreateDeviceDependentResource();
		if (!succeeded)
			return;
	}
	const D2D1_COLOR_F redColor = ColorF(ColorF::Red);
	const D2D1_COLOR_F greenColor = ColorF(ColorF::Green);
	const D2D1_COLOR_F blueColor = ColorF(ColorF::Blue);
	const D2D1_COLOR_F yellowColor = ColorF(ColorF::Yellow);
	const D2D1_COLOR_F pinkColor = ColorF(ColorF::Pink);
	const D2D1_COLOR_F lightBlue = ColorF(ColorF::LightBlue);
	const D2D1_COLOR_F lightGreen = ColorF(ColorF::LightGreen);

	m_pDCRenderTarget->BeginDraw();
	m_pDCRenderTarget->Clear(ColorF(ColorF::White));	//Clear the background
	//Draw line
	//We can set the color and opacity of solid color brush at any time,
	//so there is no need to create brushes for different colors
	m_pSolidColorBrush->SetColor(redColor);
	D2D1_POINT_2F startPoint = Point2F(10, 10);
	D2D1_POINT_2F endPoint = Point2F(90, 90);
	m_pDCRenderTarget->DrawLine(startPoint, endPoint, m_pSolidColorBrush, 5.0);
	//Draw rectangle
	m_pSolidColorBrush->SetColor(greenColor);
	D2D1_RECT_F rect = RectF(110, 10, 190, 90);
	m_pDCRenderTarget->DrawRectangle(rect, m_pSolidColorBrush, 4.0f);
	//Draw rounded rectangle
	m_pSolidColorBrush->SetColor(blueColor);
	rect = RectF(210, 10, 290, 90);
	D2D1_ROUNDED_RECT roundedRect = RoundedRect(rect, 10, 10);
	m_pDCRenderTarget->DrawRoundedRectangle(roundedRect, m_pSolidColorBrush, 3.0f);
	//Draw ellipse
	m_pSolidColorBrush->SetColor(redColor);
	D2D1_POINT_2F center = D2D1::Point2F(350, 50);
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(center, 40, 30);
	m_pDCRenderTarget->DrawEllipse(ellipse, m_pSolidColorBrush, 3.0f);
	//Fill rectangle
	m_pSolidColorBrush->SetColor(pinkColor);
	rect = RectF(10, 110, 90, 190);
	m_pDCRenderTarget->FillRectangle(rect, m_pSolidColorBrush);
	//Fill rounded rectangle
	m_pSolidColorBrush->SetColor(blueColor);
	m_pSolidColorBrush->SetOpacity(0.3f);
	rect = RectF(110, 110, 190, 190);
	roundedRect = RoundedRect(rect, 20, 20);
	m_pDCRenderTarget->FillRoundedRectangle(roundedRect, m_pSolidColorBrush);
	//Fill rectangle with linear gradient brush
	rect = RectF(210, 110, 290, 190);
	m_pDCRenderTarget->FillRectangle(rect, m_pLinearGradientBrush);
	//Fill ellipse with gradient brush
	ellipse = D2D1::Ellipse(Point2F(350, 150), 40, 40);
	m_pDCRenderTarget->FillEllipse(ellipse, m_pRadialGradientBrush);
	//Draw text with a linear gradient brush
	const CStringW text(_T("Text drawed with Direct2D & DWrite!"));
	rect = RectF(20, 260, 580, 340);
	m_pDCRenderTarget->DrawText(
		str1,
		str1.GetLength(),
		m_pTextFormat,
		rect,
		m_pLinearGradientBrush);
	rect.top += 100;
	rect.bottom += 100;
	m_pDCRenderTarget->DrawText(
		str2,
		str2.GetLength(),
		m_pTextFormat,
		rect,
		m_pLinearGradientBrush);
	HRESULT hr = m_pDCRenderTarget->EndDraw();
	if (hr == D2DERR_RECREATE_TARGET)	//Render target need to be recreated
	{
		//Discard all device dependent resources,
		//and recreate them in the next render procedure
		DiscardDeviceDependentResource();
	}
}

void CChildView::DiscardDeviceDependentResource()
{
	SafeRelease(&m_pRadialGradientBrush);
	SafeRelease(&m_pLinearGradientBrush);
	SafeRelease(&m_pSolidColorBrush);
	SafeRelease(&m_pDCRenderTarget);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	return FALSE;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	//CWnd::OnSize(nType, cx, cy);

	// TODO: 在此加入您的訊息處理常式程式碼
	if (m_pDCRenderTarget)
	{
		//m_pDCRenderTarget->Resize(SizeU(cx, cy));
	}
}
