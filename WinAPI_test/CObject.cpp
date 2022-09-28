#include "pch.h"
#include "CObject.h"

#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CPathManager.h"
#include "CResourceManager.h"

#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"


CObject::CObject() :
	m_strName{},
	m_vPosition{},
	m_pTexture(nullptr),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_vScale{},
	m_eType(OBJECT_TYPE::DEFAULT),
	m_bIsAlive(true),
	m_bIsStatic(true),
	m_bIsGravity(true),
	m_fGravity(9.8f),
	m_iHP(0),
	m_iDeal(0)
{
}

CObject::CObject(OBJECT_TYPE _eType) :
	m_strName{},
	m_vPosition{},
	m_pTexture(nullptr),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_vScale{},
	m_eType(_eType),
	m_bIsAlive(true),
	m_bIsStatic(true),
	m_bIsGravity(true),
	m_fGravity(9.8f),
	m_iHP(0),
	m_iDeal(0)
{
}

CObject::CObject(const CObject& _origin) :
	m_strName(_origin.m_strName),
	m_vPosition(_origin.m_vPosition),
	m_vScale(_origin.m_vScale),
	m_eType(_origin.m_eType),
	m_pTexture(_origin.m_pTexture),
	m_pCollider(nullptr),
	m_pAnimator(nullptr),
	m_bIsAlive(true),
	m_bIsStatic(_origin.m_bIsStatic),
	m_bIsGravity(_origin.m_bIsGravity),
	m_fGravity(_origin.m_fGravity),
	m_iHP(_origin.m_iHP),
	m_iDeal(_origin.m_iDeal)
{
	if (_origin.m_pCollider)
	{
		m_pCollider = new CCollider(*_origin.m_pCollider, this);


		m_pCollider->SetScale(_origin.m_pCollider->GetScale());
	}

	if (_origin.m_pAnimator)
	{
		m_pAnimator = new CAnimator(*_origin.m_pAnimator, this);
	}
}

CObject::~CObject()
{
 	if (m_pCollider != nullptr)
		delete m_pCollider;

	if (m_pAnimator != nullptr)
		delete m_pAnimator;
}


void CObject::CreateCollider(bool _bIsTrigger)
{
	m_pCollider = new CCollider(this, _bIsTrigger);
}


void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator(this);
}


void CObject::gravityUpdate()
{
	//if (!m_bIsStatic && m_bIsGravity && m_vCollidedDir.y <= 0.f)
	//{
	//	wstring _this = m_strName;
	//	m_vPosition.y += m_fGravity * 5.f * fDeltaTime;
	//}
}

void CObject::finalUpdate()
{
	if (m_pCollider != nullptr)
		m_pCollider->finalUpdate();
}

void CObject::render(HDC _dc)
{
	component_render(_dc);
}

void CObject::component_render(HDC _dc)
{
	if (m_pAnimator != nullptr)
		m_pAnimator->render(_dc);

	if (m_pCollider != nullptr)
		m_pCollider->render(_dc);
}
			