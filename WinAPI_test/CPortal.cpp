#include "pch.h"
#include "CPortal.h"

#include "CResourceManager.h"
#include "CSceneManager.h"

#include "CCollider.h"
#include "CTexture.h"


CPortal::CPortal(Vector2 _vPos, SCENE_TYPE _eDestination) :
	CObject(OBJECT_TYPE::PORTAL),
	m_eNextScene(_eDestination),
	m_fDelayTime(1.f),
	m_fAccTime(m_fDelayTime)
{
	m_strName = L"portal";

	SetPosition(_vPos);
	SetScale(Vector2(100.f, 100.f));

	SetStatic(true);
	SetGravity(false);


	// =========
	//  콜라이더
	// =========
	//
	CreateCollider(true);
	GetCollider()->SetScale(Vector2(100.f, 100.f));


	// ==========
	//   텍스쳐
	// ==========
	// 
	// 텍스쳐 로딩
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"portal", L"texture\\portal.bmp");
}

CPortal::~CPortal()
{
}


void CPortal::update()
{
}

void CPortal::render(HDC _dc)
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

void CPortal::OnTrigger(CCollider* _pOther)
{
	CObject* pObject = _pOther->GetOwnerObject();

	if (pObject->GetType() == OBJECT_TYPE::PLAYER && abs(GetPosition().x - pObject->GetPosition().x) <= 10.f)
		ChangeScene(m_eNextScene, 800);
}