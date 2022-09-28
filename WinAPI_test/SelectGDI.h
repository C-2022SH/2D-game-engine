#pragma once


class SelectGDI
{
private:
	HDC		m_hDC;
	HBRUSH	m_hDefaultBrush;
	HPEN	m_hDefaultPen;


public:
	SelectGDI(HDC _dc, PEN_TYPE _eType);
	SelectGDI(HDC _dc, BRUSH_TYPE _eType);
	~SelectGDI();
};

