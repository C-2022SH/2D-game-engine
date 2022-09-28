#include "pch.h"
#include "SelectGDI.h"

#include "CCore.h"


SelectGDI::SelectGDI(HDC _dc, PEN_TYPE _eType) :
	m_hDC(_dc),
	m_hDefaultBrush(nullptr),
	m_hDefaultPen(nullptr)
{
	HPEN hPen = CCore::GetInstance()->GetPen(_eType);
	m_hDefaultPen = (HPEN)SelectObject(m_hDC, hPen);
}

SelectGDI::SelectGDI(HDC _dc, BRUSH_TYPE _eType) :
	m_hDC(_dc),
	m_hDefaultBrush(nullptr),
	m_hDefaultPen(nullptr)
{
	HBRUSH hPen = CCore::GetInstance()->GetBrush(_eType);
	m_hDefaultBrush = (HBRUSH)SelectObject(m_hDC, hPen);
}

SelectGDI::~SelectGDI()
{
	if (m_hDefaultBrush != nullptr)
		SelectObject(m_hDC, m_hDefaultBrush);
	
	if (m_hDefaultPen != nullptr)
		SelectObject(m_hDC, m_hDefaultPen);
}