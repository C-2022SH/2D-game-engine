#include "pch.h"
#include "CGround.h"

#include "CResourceManager.h"

#include "CCollider.h"
#include "CTexture.h"

// 벽 - 기본 불투명
CGround::CGround(Vector2 _vPos, Vector2 _vScale, GROUND_TYPE _eType) :
	CObject(OBJECT_TYPE::GROUND),
	m_arrScale{},
	m_arrLeftTop{}
{
	// 타입에 따라 이름, 텍스쳐 설정
	if (_eType == GROUND_TYPE::WALL_SOLID)
	{
		m_strName = L"solid wall";
		m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"ground", L"texture\\wall.bmp");
	}
	else
		m_strName = L"transparent ground";

	// 오브젝트 위치, 스케일 설정
	SetPosition(_vPos);
	SetScale(_vScale);

	// 스태틱, 중력
	SetStatic(true);
	SetGravity(false);

	// =========
	//  콜라이더
	// =========
	CreateCollider(false);
	GetCollider()->SetScale(_vScale);
}

// 점프하기 위한 발판
CGround::CGround(Vector2 _vPos, GROUND_TYPE _eType, bool _bIsStatic, wstring _strName, bool _bIsTrigger) :
	CObject(OBJECT_TYPE::GROUND),
	m_eGroundType(_eType)
{
	InitLeftTop();
	InitScale();

	// 오브젝트 이름 설정
	m_strName = _strName;

	// 오브젝트 위치 설정
	SetPosition(_vPos);

	// 스태틱, 중력
	SetStatic(_bIsStatic);
	SetGravity(false);


	// =========
	//  콜라이더
	// =========
	//
	CreateCollider(_bIsTrigger);
	GetCollider()->SetScale(m_arrScale[(UINT)m_eGroundType]);


	// ===========
	//   텍스쳐
	// ===========
	// 
	// 텍스쳐 로딩
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"ground", L"texture\\ground.bmp");

	// 이미지에 맞도록 크기 설정
	SetScale(m_arrScale[(UINT)m_eGroundType]);
}

CGround::~CGround()
{
}


void CGround::update()
{
}

void CGround::render(HDC _dc)
{
	if (m_pTexture)
	{
		Vector2 vPos = GetPosition();
		vPos = CCamera::GetInstance()->GetRenderPosition(vPos);	// 렌더링 위치

		Vector2 vScale = GetScale();
		Vector2 vLeftTop = m_arrLeftTop[(UINT)m_eGroundType];


		GdiTransparentBlt(_dc,
			int(vPos.x - vScale.x * 0.5f),
			int(vPos.y - vScale.y * 0.5f),
			(int)vScale.x, (int)vScale.y,
			m_pTexture->GetDC(),
			(int)vLeftTop.x, (int)vLeftTop.y,
			(int)vScale.x, (int)vScale.y,
			RGB(170, 150, 230));	// 투명화할 색
	}

	CObject::render(_dc);
}
