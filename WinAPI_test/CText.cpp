#include "pch.h"
#include "CText.h"

#include "CCamera.h"


CText::CText(wstring _strContent, Dir4 _dPos, CUI* _pParent) :
	CUI(UI_TYPE::TEXT),
	m_strContent(_strContent),
	m_pParentUI(_pParent)
{
	m_strName = L"text";
	SetPosition(_dPos);

	if (m_pParentUI != nullptr)
	{
		int iCenter_x_diff = m_pParentUI->Center_x() - Center_x();
		int iCentery_y_diff = m_pParentUI->Center_y() - Center_y();

		SetPosition(GetPosition() + Dir4(iCenter_x_diff, iCentery_y_diff, iCenter_x_diff, iCentery_y_diff));
	}
}

CText::~CText()
{
}

void CText::update()
{
}

void CText::render(HDC _dc)
{
	Dir4 dPos = GetPosition();

	SetBkMode(_dc, TRANSPARENT);
	SetTextColor(_dc, RGB(100, 100, 50));

	// ±Û¾¾ Ç¥½Ã
	RECT rectArea = { dPos.left, dPos.up, dPos.right, dPos.down };

	DrawText(_dc, m_strContent.c_str(), -1, &rectArea, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}