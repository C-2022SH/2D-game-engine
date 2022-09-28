#include "pch.h"
#include "CBar.h"

#include "SelectGDI.h"

#include "CPlayer.h"


CBar::CBar(Dir4 _dPos, CObject* _pStandard) :
	CUI(UI_TYPE::BAR),
	m_fProportionalConst(0.f),
	pValueStandard(_pStandard)
{
	SetPosition(_dPos);

	if(pValueStandard)
		m_fProportionalConst = (float)(_dPos.right - _dPos.left) / pValueStandard->GetHP();
}

CBar::~CBar()
{
}


void CBar::update()
{
}

void CBar::render(HDC _dc)
{
	// outline
	PEN_TYPE ePen = PEN_TYPE::PURPLE;

	SelectGDI _pen1(_dc, ePen);
	SelectGDI _brush1(_dc, BRUSH_TYPE::HOLLOW);

	Dir4 dOrigPos = GetPosition();

	Dir4 dPos = dOrigPos + Dir4(-3, -3, 3, 3);

	Rectangle(_dc,
		dPos.left, dPos.up,
		dPos.right, dPos.down);

	// bar
	ePen = PEN_TYPE::HOLLOW;

	SelectGDI _pen2(_dc, ePen);
	SelectGDI _brush2(_dc, BRUSH_TYPE::YELLOW);

	if (pValueStandard)
		dOrigPos.right = dOrigPos.left + (int)m_fProportionalConst * pValueStandard->GetHP();

	Rectangle(_dc,
		dOrigPos.left, dOrigPos.up, dOrigPos.right, dOrigPos.down);
}