#include "pch.h"
#include "CGenerator.h"

#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CTimeManager.h"

#include "CCollider.h"
#include "CTexture.h"
#include "CScene.h"
#include "CMonster.h"


// trigger, non-static
CGenerator::CGenerator(Vector2 _vPos, Vector2 _vScale) :
	CObject(OBJECT_TYPE::GENERATOR),
	m_iEnemyCount(3),
	m_fSpawnDuration(1.f),
	m_fAccSpawnTime(1.f)
{
	m_strName = L"monster portal";
	
	SetPosition(_vPos);
	SetScale(_vScale);

	SetStatic(true);
	SetGravity(false);


	// =========
	//  콜라이더
	// =========
	CreateCollider(true);
	GetCollider()->SetScale(_vScale);


	// ===========
	//   텍스쳐
	// ===========
	// 
	// 텍스쳐 로딩
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"generator", L"texture\\generator.bmp");
}

CGenerator::~CGenerator()
{
}


void CGenerator::SpawnMonster(int _iCount)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurrentScene();

	if (m_fAccSpawnTime >= m_fSpawnDuration)
	{
		CObject* pMonsterObj = new CMonster(GetPosition());
		pCurScene->AddObject(pMonsterObj);
		m_fAccSpawnTime = 0.f;
	}

	m_fAccSpawnTime += fDeltaTime;
}

void CGenerator::update()
{
	CObject* pPlayer = (CObject*) CGameManager::GetInstance()->GetPlayerObject();

	if (pPlayer && GetPosition().x - pPlayer->GetPosition().x <= 200.f && GetPosition().x - pPlayer->GetPosition().x >= -200.f)
	{
		SpawnMonster(m_iEnemyCount);
	}
}

void CGenerator::render(HDC _dc)
{
	if (m_pTexture)
	{
		Vector2 vPos = GetPosition();
		vPos = CCamera::GetInstance()->GetRenderPosition(vPos);	// 렌더링 위치

		Vector2 vScale = GetScale();

		GdiTransparentBlt(_dc,
			int(vPos.x - vScale.x * 0.5f),
			int(vPos.y - vScale.y * 0.5f),
			(int)vScale.x, (int)vScale.y,
			m_pTexture->GetDC(),
			0, 0,
			(int)vScale.x, (int)vScale.y,
			RGB(255, 0, 255));	// 투명화할 색
	}

	CObject::render(_dc);
}


void CGenerator::OnTriggerEnter(CCollider* _pOther)
{
	CObject* pCollidedObject = _pOther->GetOwnerObject();

	if (pCollidedObject->GetType() == OBJECT_TYPE::BULLET_PLAYER)
		DeleteEventObject(this);
}
