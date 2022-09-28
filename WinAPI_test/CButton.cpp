#include "pch.h"
#include "CButton.h"
#include "CUI.h"

#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"

#include "CTexture.h"
#include "CCamera.h"
#include "CText.h"
#include "CScene.h"


CButton::CButton(Dir4 _dPos, bool _bInteractive, wstring _strText) :
	CUI(UI_TYPE::BUTTON),
	m_bIsInteractive(_bInteractive)
{
	m_strName = L"button";

	SetPosition(_dPos);

	// ===========
	//   텍스쳐
	// ===========
	// 
	// 텍스쳐 로딩
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"UI", L"texture\\UI.bmp");


	// ============
	//  텍스트(내용)
	// ============
	if (!_strText.empty())
	{
		CText* pText = new CText(_strText, _dPos, this);
		CSceneManager::GetInstance()->GetCurrentScene()->AddUI(pText);
	}
}

CButton::~CButton()
{
}


void CButton::OnButtonClick()
{
	ChangeScene(SCENE_TYPE::WHITE_01);
}

void CButton::update()
{
	if (m_bIsInteractive)
	{
		Dir4 dPosition = GetPosition();

		if (CHECK_ONCLICK(KEY::CLICK_RIGHT, dPosition))
			OnButtonClick();
	} 
}

void CButton::render(HDC _dc)
{
	if (m_pTexture)
	{
		Dir4 dPos = GetPosition();

		GdiTransparentBlt(_dc,
			dPos.left, dPos.up,
			dPos.right - dPos.left, dPos.down - dPos.up,
			m_pTexture->GetDC(),
			0, 0,
			dPos.right - dPos.left, dPos.down - dPos.up,
			RGB(255, 0, 255));				// 투명화할 색
	}

	CUI::render(_dc);
}