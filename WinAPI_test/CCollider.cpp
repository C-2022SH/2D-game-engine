#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"

// 디버그용 코드
#include "SelectGDI.h"
#include "CSceneManager.h"
#include "CScene.h"
#include "CCamera.h"


// 클래스 전역변수 초기화
UINT CCollider::g_iNextID = 0;


CCollider::CCollider(CObject* _owner, bool _bIsTrigger, Vector2 _vRot) :
	m_pOwner(_owner),
	m_iID(g_iNextID++),
	m_vOffset{},
	m_vScale{},
	m_iCollided(0),
	m_bIsTrigger(_bIsTrigger)
{
	Vector2	vObjectPos = m_pOwner->GetPosition();
	m_vFinalPosition = vObjectPos + m_vOffset;

	assert(m_iCollided >= 0);

	// ==========
	//  회전 계산
	// ==========
	//
	SetRotation(_vRot);

}

// 복사생성자
CCollider::CCollider(const CCollider& _origin, CObject* _owner) :
	m_pOwner(_owner),
	m_vOffset(_origin.m_vOffset),
	m_vScale(_origin.m_vScale),
	m_fCos(_origin.m_fCos),
	m_fSin(_origin.m_fSin),
	m_iID(g_iNextID++),
	m_iCollided(0),
	m_bIsTrigger(_origin.m_bIsTrigger)
{
	Vector2	vObjectPos = m_pOwner->GetPosition();
	m_vFinalPosition = vObjectPos + m_vOffset;

	assert(m_iCollided >= 0);
}

CCollider::~CCollider()
{
	// Delete_Map_Vector(m_mapCollidedDir);
}


void CCollider::finalUpdate()
{
	if (!m_pOwner->IsStatic())
	{
		Vector2	vObjectPos = m_pOwner->GetPosition();
		m_vFinalPosition = vObjectPos + m_vOffset;

		assert(m_iCollided >= 0);
	}
}


void CCollider::render(HDC _dc)
{
	// =============
	//  디버그용 코드
	// =============
	//
	// 충돌 영역 표시
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCollided)
		ePen = PEN_TYPE::RED;

	SelectGDI _pen(_dc, ePen);
	SelectGDI _brush(_dc, BRUSH_TYPE::HOLLOW);

	// 여기는 회전 반영 안 한 부분
	Vector2 vPos = CCamera::GetInstance()->GetRenderPosition(m_vFinalPosition);

	//Rectangle(_dc,
	//	(int)(vPos.x - m_vScale.x * 0.5f),
	//	(int)(vPos.y - m_vScale.y * 0.5f),
	//	(int)(vPos.x + m_vScale.x * 0.5f),
	//	(int)(vPos.y + m_vScale.y * 0.5f));

	// 회전 반영
	Vector2 vLeftTop = CCamera::GetInstance()->GetRenderPosition(GetRotatedVertex(-1.f, -1.f));
	Vector2 vRightTop = CCamera::GetInstance()->GetRenderPosition(GetRotatedVertex(1.f, -1.f));
	Vector2 vRightBottom = CCamera::GetInstance()->GetRenderPosition(GetRotatedVertex(1.f, 1.f));
	Vector2 vLeftBottom = CCamera::GetInstance()->GetRenderPosition(GetRotatedVertex(-1.f, 1.f));

	MoveToEx(_dc, (int)vLeftTop.x, (int)vLeftTop.y, nullptr);
	LineTo(_dc, (int)vRightTop.x, (int)vRightTop.y);
	LineTo(_dc, (int)vRightBottom.x, (int)vRightBottom.y);
	LineTo(_dc, (int)vLeftBottom.x, (int)vLeftBottom.y);
	LineTo(_dc, (int)vLeftTop.x, (int)vLeftTop.y);


	// =============
	//  디버그용 코드
	// =============
	//
	// 충돌 방향 표시
	wstring strColl = std::to_wstring(m_iCollided);
	wstring strLeft = std::to_wstring(m_pOwner->GetCollidedDir().left);
	wstring strUp = std::to_wstring(m_pOwner->GetCollidedDir().up);
	wstring strRight = std::to_wstring(m_pOwner->GetCollidedDir().right);
	wstring strDown = std::to_wstring(m_pOwner->GetCollidedDir().down);

	strColl += L" / ";
	strColl += strLeft += strUp += strRight += strDown;

	DRAW_LOG(_dc, 100, 90, 90, vPos.x, vPos.y, strColl, -40.f);
}

void CCollider::OnCollisionEnter(Dir4 _dCollidedDir, CCollider* _pOther)
{
	// 정적 오브젝트인 경우
	if (m_pOwner->IsStatic())
		++m_iCollided;

	else
	{
		// 중복 체크
		map<CCollider*, Dir4>::iterator iter = m_mapCollidedDir.find(_pOther);

		// 중복이 아닌 경우
		if (iter == m_mapCollidedDir.end())
		{
			++m_iCollided;

			// 오차 방향 디렉션 업데이트
			Dir4 dFinalColDir = GetOwnerObject()->GetCollidedDir() + _dCollidedDir;
			GetOwnerObject()->SetCollidedDir(dFinalColDir);

			m_mapCollidedDir.insert(make_pair(_pOther, _dCollidedDir));
		}
	}
	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	if (m_pOwner->IsStatic())
		--m_iCollided;

	else
	{
		// 중복 체크
		map<CCollider*, Dir4>::iterator iter = m_mapCollidedDir.find(_pOther);

		if (iter != m_mapCollidedDir.end())
		{
			--m_iCollided;

			Dir4 dFinalColDir = m_pOwner->GetCollidedDir() - iter->second;
			m_pOwner->SetCollidedDir(dFinalColDir);

			m_mapCollidedDir.erase(iter);
		}
	}

	m_pOwner->OnCollisionExit(_pOther);
}

void CCollider::OnTriggerEnter(CCollider* _pOther)
{
	++m_iCollided;

	m_pOwner->OnTriggerEnter(_pOther);
}

void CCollider::OnTrigger(CCollider* _pOther)
{
	m_pOwner->OnTrigger(_pOther);
}

void CCollider::OnTriggerExit(CCollider* _pOther)
{
	--m_iCollided;

	m_pOwner->OnTriggerExit(_pOther);
}