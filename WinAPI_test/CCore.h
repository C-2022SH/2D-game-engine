#pragma once


class CCore
{
	SINGLETON(CCore);

private:
	HWND	m_hwnd;			// 윈도우 핸들
	POINT	m_ptResolution;	// 메인 윈도우 해상도
	HDC		m_hDC;			// 메인 윈도우에 그릴 DC (윈도우에서 제공하는 아이)

	HBITMAP	m_hBit;
	HDC		m_memDC;		// 사본용 DC

	// 자주 사용하는 GDI Object
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];


public:
	int initalize(HWND _hwnd, POINT _ptResolution);
	void progress();


private:
	void CreateBrushPen();


public:
	HWND GetMainHwnd() { return m_hwnd; };
	HDC GetMainDC() { return m_hDC; };
	POINT GetResolution() { return m_ptResolution; };
	HBRUSH GetBrush(BRUSH_TYPE _eBrush) { return m_arrBrush[(UINT)_eBrush]; }
	HPEN GetPen(PEN_TYPE _ePen) { return m_arrPen[(UINT)_ePen]; }
};