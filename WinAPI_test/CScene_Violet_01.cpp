#include "pch.h"
#include "CScene_Violet_01.h"

#include "CResourceManager.h"
#include "CCollisionManager.h"

#include "CPlayer.h"
#include "CGround.h"

#include "CCollider.h"
#include "CTexture.h"

#include "CBar.h"


CScene_Violet_01::CScene_Violet_01()
{
	CTexture* pTex = CResourceManager::GetInstance()->LoadTexture(L"violet map", L"texture\\map_test.bmp");
	SetMap(pTex);
}

CScene_Violet_01::~CScene_Violet_01()
{
}


void CScene_Violet_01::Enter()
{
	/********************
	|					|
	|	 오브젝트 추가	|
	|					|
	********************/

	// ====================
	//  player 오브젝트 추가
	// ====================
	//
	CPlayer* pPlayerObj = new CPlayer(Vector2(0.f, 0.f), false);
	//pPlayerObj->GetCollider()->SetScale(Vector2(200.f, 180.f));
	//pPlayerObj->GetCollider()->SetRotation(Vector2(1.f, 1.2f));
	AddObject(pPlayerObj);

	CGameManager::GetInstance()->SetPlayerObject(pPlayerObj);

	// ====================
	//  ground 오브젝트 추가
	// ====================
	//
	CGround* pGroundObj = new CGround(Vector2(0.f, 0.f), Vector2(40.f,0.f), GROUND_TYPE::WALL_TRANS);
	pGroundObj->GetCollider()->SetRotation(Vector2(1.f, 1.f));
	pGroundObj->SetStatic(false);
	AddObject(pGroundObj);

	CGround* pGroundObj_1 = new CGround(Vector2(160.f, 180.f), Vector2(450.f, 10.f), GROUND_TYPE::WALL_TRANS);
	pGroundObj_1->GetCollider()->SetRotation(Vector2(-1.f, 1.f));
	pGroundObj_1->SetStatic(false);
	AddObject(pGroundObj_1);

	// 충돌 지정
	// player | ground
	CCollisionManager::GetInstance()->CheckGroup(OBJECT_TYPE::PLAYER, OBJECT_TYPE::GROUND);



	/********************
	|					|
	|		UI 추가		|
	|					|
	********************/

	// 체력바 그리기
	CBar* pBar = new CBar(Dir4(10, 10, 130, 30), pPlayerObj);
	AddUI(pBar);

	// 카메라가 따라다니는 오브젝트 지정
	CCamera::GetInstance()->SetTarget(pPlayerObj);

	CScene::Enter();
}