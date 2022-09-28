#include "pch.h"
#include "CKeyManager.h"

#include "CCore.h"


// 열거형과 실제 가상 키값을 대응시키는 배열
int g_arrVK[(int)KEY::LAST] =
{
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU,	//ALT,
	VK_CONTROL,	//CTRL,
	VK_LSHIFT,	//LSHIFT,
	VK_SPACE,	//SPACE,
	VK_RETURN,	//ENTER,
	VK_ESCAPE,	//ESC,
	VK_LBUTTON,	//CLICK_LEFT,
	VK_RBUTTON	//CLICK_RIGHT,

	//LAST
};

CKeyManager::CKeyManager()
{}

CKeyManager::~CKeyManager()
{}

void CKeyManager::initialize()
{
	for (int i = 0; i < (int)KEY::LAST; i++)
	{
		m_vKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}

void CKeyManager::update()
{
	/*
	윈도우가 여러 개인 경우
	* 현재 포커싱 된 윈도우를 확인하고 싶을 때는 이렇게 메인을 불러와서,
	* HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	* 아래 hWnd 와 같은지 아닌지를 체크하면 된다
	*/

	HWND hWnd = GetFocus();		// 현재 포커싱 되어 있는 윈도우

	// 윈도우 포커싱 확인
	if (hWnd != nullptr)
	{
		// 마우스 위치 계산
		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);
		ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptMousePos);

		m_vCurrMousePosition = Vector2(ptMousePos);


		// i : 내가 설정한 가상 키 값
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			// 키가 눌렸다면
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				// 이전에 눌려있었다면
				if (m_vKey[i].bPrevPush)
					m_vKey[i].eState = KEY_STATE::HOLD;
				else
					m_vKey[i].eState = KEY_STATE::TAP;

				m_vKey[i].bPrevPush = true;
			}
			// 키가 안 눌렸다면
			else
			{
				if (m_vKey[i].bPrevPush)
					m_vKey[i].eState = KEY_STATE::AWAY;
				else
					m_vKey[i].eState = KEY_STATE::NONE;

				m_vKey[i].bPrevPush = false;
			}
		}
	}
	
	// 윈도우 포커싱 해제 상태
	else
	{
		for (int i = 0; i < (int)KEY::LAST; i++)
		{
			m_vKey[i].bPrevPush = false;

			if (m_vKey[i].eState == KEY_STATE::TAP || m_vKey[i].eState == KEY_STATE::HOLD)
				m_vKey[i].eState = KEY_STATE::AWAY;

			else if (m_vKey[i].eState == KEY_STATE::AWAY)
				m_vKey[i].eState = KEY_STATE::NONE;
		}
	}
}
