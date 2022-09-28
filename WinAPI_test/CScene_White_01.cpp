#include "pch.h"
#include "CScene_White_01.h"

#include "CResourceManager.h"
#include "CPathManager.h"
#include "CCollisionManager.h"
#include "CKeyManager.h"
#include "CSceneManager.h"
#include "CGameManager.h"
#include "CCamera.h"

#include "CCore.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CBullet.h"
#include "CGround.h"
#include "CGenerator.h"
#include "CPortal.h"

#include "CButton.h"
#include "CBar.h"

#include "CTexture.h"


CScene_White_01::CScene_White_01() :
	m_iMonsterCount(3)
{
	CTexture* pTex = CResourceManager::GetInstance()->LoadTexture(L"white map", L"texture\\map_white.bmp");
	SetMap(pTex);
}

CScene_White_01::~CScene_White_01()
{
}


void CScene_White_01::update()
{
	CScene::update();
}


void CScene_White_01::Enter()
{
	Vector2 vResolution = Vector2(CCore::GetInstance()->GetResolution());

	// ====================
	//  player 오브젝트 추가
	// ====================
	//
	CPlayer* pPlayerObj = new CPlayer(Vector2(0.f, 200.f), false);
	
	// 길 바운드 설정
	pPlayerObj->SetBound(Dir4(0, 414, 0, 0));

	AddObject(pPlayerObj);

	CGameManager::GetInstance()->SetPlayerObject(pPlayerObj);

	// 체력바 그리기
	CBar* pBar = new CBar(Dir4(10, 10, 130, 30), pPlayerObj);
	AddUI(pBar);

	// 카메라가 따라다니는 오브젝트 지정
	CCamera::GetInstance()->SetTarget(pPlayerObj);


	//CObject* pOtherObj = pObj->Clone();
	//pOtherObj->SetPosition(Vector2(vResolution.x * 0.5f + 100.f, vResolution.y * 0.5f));
	//AddObject(pOtherObj, GROUP_TYPE::PLAYER);

	// ========================
	//  CGenerator 오브젝트 추가
	// ========================
	//
	CGenerator* pCGeneratorObj = new CGenerator(Vector2(600.f, -50.f), Vector2(140.f, 140.f));
	AddObject(pCGeneratorObj);


	// ======================
	//   portal 오브젝트 추가
	// ======================
	//
	CPortal* pPortal = new CPortal(Vector2(1000.f, 40.f), SCENE_TYPE::VIOLET_01);
	AddObject(pPortal);


	// 충돌 지정
	// player | monster
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::MONSTER);
	// player | ground
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::GROUND);

	// monster | player bullet
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::MONSTER, OBJECT_TYPE::BULLET_PLAYER);
	// monster | monster
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::MONSTER, OBJECT_TYPE::MONSTER);
	// monster | ground
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::MONSTER, OBJECT_TYPE::GROUND);

	// generator | player bullet
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::GENERATOR, OBJECT_TYPE::BULLET_PLAYER);

	// portal | player
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::PORTAL, OBJECT_TYPE::PLAYER);

	// 카메라 위치 지정
	// CCamera::GetInstance()->SetLookPosition(pObj->GetPosition());

	CScene::Enter();
}

