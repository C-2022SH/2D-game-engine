#include "pch.h"
#include "CCore.h"

#include "CTimeManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"
#include "CEventManager.h"
#include "CGameManager.h"
#include "CCamera.h"

#include "SelectGDI.h"

#include "CResourceManager.h"
#include "CScene.h"
#include "CTexture.h"
#include "CPlayer.h"


CCore::CCore() :
	m_hwnd(0),
	m_ptResolution{},
	m_hDC(0),
	m_hBit(0),
	m_memDC(0),
	m_arrBrush{},
	m_arrPen{}
{

}

CCore::~CCore()
{
	ReleaseDC(m_hwnd, m_hDC);
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);

	for (int i = 0; i < (UINT)BRUSH_TYPE::END; i++)
	{
		DeleteObject(m_arrBrush[i]);
	}

	for (int i = 0; i < (UINT)PEN_TYPE::END; i++)
	{
		DeleteObject(m_arrPen[i]);
	}
}

int CCore::initalize(HWND _hwnd, POINT _ptResolution)
{
	m_hwnd = _hwnd;
	m_ptResolution = _ptResolution;

	RECT rt = {0, 0, m_ptResolution.x, m_ptResolution.y};

	// 해상도에 맞게 윈도우 조절
	// 해상도 : 물체가 그려질 수 있는 영역
	
	// SetWindowPos 는 해상도 + 메뉴 + 보더 등을 모두 포함하는 크기
	// 따라서 먼저 그걸 감안한 사이즈를 구한다
	// 마지막 인자는 메뉴바 포함 여부
	// 얘가 직접 rt 에 접근해서 값을 수정한다!!
	// 리턴 값이 너무 큰 경우 이런 식으로 사용한다. 주석언어 _Inout_
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(m_hwnd, nullptr, 150, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	m_hDC = GetDC(m_hwnd);

	// 이중 버퍼링 용도의 비트맵과 DC 생성
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);
	m_memDC = CreateCompatibleDC(m_hDC);

	HBITMAP holdBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	// DC 기본값이 반환됨. 필요 없으므로 삭제
	DeleteObject(holdBit);

	// 자주 사용하는 펜, 브러쉬 생성
	CreateBrushPen();
	
	// manager 초기화
	CPathManager::GetInstance()->initialize();
	CTimeManager::GetInstance()->initialize();
	CKeyManager::GetInstance()->initialize();
	CSceneManager::GetInstance()->initialize();
	CCollisionManager::GetInstance()->initialize();

	return S_OK;
}



void CCore::progress()
{
	// ================
	//  manager update
	// ================
	CTimeManager::GetInstance()->update();
	CKeyManager::GetInstance()->update();
	CCamera::GetInstance()->update();


	// ================
	//   scene update
	// ================
	CSceneManager::GetInstance()->update();

	// 충돌 체크
	CCollisionManager::GetInstance()->update();


	// ================
	//    rendering
	// ================
	// 우선 버퍼에 그린다

	// 화면 지우기
	// 디폴트 배경 이미지
	SelectGDI _brush(m_memDC, BRUSH_TYPE::BACKGROUND);
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
	
	// 맵 이미지
	CTexture* pTexture = CSceneManager::GetInstance()->GetCurrentScene()->GetMap();

	// 맵도 보정된 위치에 그려야 함
	Vector2 vRenderCam = CCamera::GetInstance()->GetRenderPosition(Vector2(m_ptResolution) * -0.5f);

	BitBlt(m_memDC, (int)vRenderCam.x, (int)vRenderCam.y, pTexture->Width(), pTexture->Height(),
		pTexture->GetDC(), 0, 0, SRCCOPY);

	// 씬 이미지
	// 씬이 늦게 따라와도! 맵이 앞에 그려져 있으니까 자연스러움
	CSceneManager::GetInstance()->render(m_memDC);
	
	// hp 그리기
	//RECT rectArea = { 10,10,100,100 };
	//CPlayer* pPlayer = (CPlayer*) CSceneManager::GetInstance()->GetCurrentScene()->GetPlayerObject();
	//wstring strHP = std::to_wstring(pPlayer->GetPlayerHP());
	//DrawText(m_memDC, strHP.c_str(), -1, &rectArea, DT_LEFT);

	// 메인 DC 에 옮겨 그려 준다.
	// 목적지, 복사받을 위치, 복사할 그림, 복사할 위치
	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y,
		m_memDC, 0, 0, SRCCOPY);


	// ================
	//  delayed events
	// ================
	CEventManager::GetInstance()->update();
}


void CCore::CreateBrushPen()
{
	// brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BACKGROUND] = CreateSolidBrush(RGB(180, 150, 230));
	m_arrBrush[(UINT)BRUSH_TYPE::YELLOW] = CreateSolidBrush(RGB(254, 248, 189));

	// pen
	m_arrPen[(UINT)PEN_TYPE::HOLLOW] = CreatePen(PS_NULL, 0, RGB(0, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::RED] = CreatePen(PS_SOLID, 2, RGB(229, 71, 103));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = CreatePen(PS_SOLID, 2, RGB(167, 219, 76));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = CreatePen(PS_SOLID, 1, RGB(50, 50, 240));
	m_arrPen[(UINT)PEN_TYPE::PURPLE] = CreatePen(PS_SOLID, 4, RGB(174, 130, 188));
}
