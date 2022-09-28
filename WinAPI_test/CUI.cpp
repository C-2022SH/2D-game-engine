#include "pch.h"
#include "CUI.h"

#include "CTexture.h"
#include "CAnimator.h"


CUI::CUI(UI_TYPE _eType) :
	m_strName{},
	m_eType(_eType),
	m_pTexture(nullptr),
	m_pAnimator(nullptr)
{
}

CUI::~CUI()
{
}


void CUI::render(HDC _dc)
{
	component_render(_dc);
}

void CUI::component_render(HDC _dc)
{
	if (m_pAnimator != nullptr)
		m_pAnimator->render(_dc);
}

