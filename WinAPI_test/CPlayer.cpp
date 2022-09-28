#include "pch.h"
#include "CPlayer.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"

#include "CBullet.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CTexture.h"

#include "CAnimation.h"


CPlayer::CPlayer(Vector2 _vPos, bool _bIsStatic, wstring _strName, Vector2 _vCollider, bool _bIsTrigger) :
	CObject(OBJECT_TYPE::PLAYER),
	m_fIsMovingRight(1.f),
	m_fPlayerSpeed(200.f),
	m_fJumpHeight(350.f),
	m_fJumpHalfDuration(0.6f),
	m_fJumpSpeed(m_fJumpHeight / m_fJumpHalfDuration),
	m_fJumpAccTime(0.f),
	m_bIsJumping(true),
	m_strCurrAnim(L"player stop")
{
	// 플레이어 오브젝트 이름, 위치, 크기 설정
	m_strName = _strName;
	SetPosition(_vPos);
	SetScale(Vector2(80.f, 80.f));

	// 체력, 공격
	SetHP(10);
	SetDeal(2);

	// 스태틱 여부
	SetStatic(_bIsStatic);


	// =========
	//  콜라이더
	// =========
	CreateCollider(_bIsTrigger);
	GetCollider()->SetScale(Vector2(_vCollider.x, _vCollider.y * 0.5f));
	// GetCollider()->SetOffsetPosition(Vector2(0.f, -_vCollider.y * 0.5f));

	// ===========
	//  애니메이션
	// ===========
	// 
	// 텍스쳐 로딩
	m_pTexture = CResourceManager::GetInstance()->LoadTexture(L"player texture", L"texture\\playerTextures.bmp");

	// 플레이어 애니메이터 생성
	CreateAnimator();

	// 정지
	GetAnimator()->CreateAnimation(L"player stop", m_pTexture,
					Vector2(3.f, 640.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 5, Vector2(0.f, -15.f));

	// 왼쪽으로 걷기
	GetAnimator()->CreateAnimation(L"player walk_left", m_pTexture,
					Vector2(84.f, 158.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.1f, 7, Vector2(0.f, -15.f));

	// 오른쪽으로 걷기
	GetAnimator()->CreateAnimation(L"player walk_right", m_pTexture,
					Vector2(84.f, 0.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.1f, 7, Vector2(0.f, -15.f));

	// 오른쪽으로 점프
	GetAnimator()->CreateAnimation(L"player jump_left", m_pTexture,
					Vector2(151.f, 800.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 6, Vector2(0.f, -15.f));

	// 오른쪽으로 점프
	GetAnimator()->CreateAnimation(L"player jump_right", m_pTexture,
					Vector2(165.f, 720.f), Vector2(80.f, 80.f), Vector2(80.f, 0.f), 0.2f, 6, Vector2(0.f, -15.f));


//	GetAnimator()->PlayAnimation(L"player stop", true);

	// 프레임 별로 오프셋 설정
	//CAnimation* pAnim = GetAnimator()->FindAnimation(_strAnimName);
	//pAnim->GetFrame(0).vOffset = Vector2(0.f, -20.f);
}


CPlayer::~CPlayer()
{
}

void CPlayer::update()
{
	// ===============
	//  무빙/애니메이션
	// ===============
	//
	if (!IsStatic())
	{
		Vector2 vPos = GetPosition();

		// 애니메이션
		m_strCurrAnim = L"player stop";
		bool bAnimRepeat = true;
	

		if (KEY_HOLD(KEY::RIGHT))
		{
			// 움직일 수 없어도 애니메이션/방향 설정은 바꾸어 준다.
			m_strCurrAnim = L"player walk_right";
			m_fIsMovingRight = 1.f;

			// 막혀 있지 않은 경우 이동
			if (m_dCollidedDir.right <= 0)
			{
				vPos.x += m_fPlayerSpeed * fDeltaTime;

				// 맵에서 벗어난 경우 벗어나지 못하도록 위치 조정
				if (vPos.x > (float)m_dBound.right)
					vPos.x = (float)m_dBound.right;
			}
		}

		if (KEY_HOLD(KEY::LEFT))
		{
			m_strCurrAnim = L"player walk_left";
			m_fIsMovingRight = -1.f;

			if (m_dCollidedDir.left <= 0)
			{
				vPos.x -= m_fPlayerSpeed * fDeltaTime;

				// 맵에서 벗어난 경우 벗어나지 못하도록 위치 조정
				if (vPos.x < (float)m_dBound.left)
					vPos.x = (float)m_dBound.left;
			}
		}

		if (KEY_HOLD(KEY::UP))
		{
			m_strCurrAnim = L"player walk_right";

			if (m_dCollidedDir.up <= 0)
			{
				vPos.y -= m_fPlayerSpeed * fDeltaTime;

				// 맵에서 벗어난 경우 벗어나지 못하도록 위치 조정
				if (vPos.y < (float)m_dBound.up)
					vPos.y = (float)m_dBound.up;
			}
		}

		if (KEY_HOLD(KEY::DOWN))
		{
			m_strCurrAnim = L"player walk_left";
			m_fIsMovingRight = -1.f;

			if (m_dCollidedDir.down <= 0)
			{
				vPos.y += m_fPlayerSpeed * fDeltaTime;

				// 맵에서 벗어난 경우: 벗어나지 못하도록 위치 조정
				if (vPos.y > (float)m_dBound.down)
					vPos.y = (float)m_dBound.down;
			}
		}

		// 더블점프 방지
		if (m_dCollidedDir.up <= 0 && KEY_TAP(KEY::SPACE) && m_bIsJumping == false)
			m_bIsJumping = true;


		if (m_bIsJumping)
		{
			// 시간 누적
			m_fJumpAccTime += fDeltaTime;

			float fJumpTimePassed = m_fJumpAccTime - m_fJumpHalfDuration;

			// 위로 막혀 있는 경우 (올라가야 하는데 못 올라가는 경우)
			// 바로 떨어짐
			if (fJumpTimePassed <= 0.f && m_dCollidedDir.up > 0)
				m_fJumpAccTime = m_fJumpHalfDuration;

			// 아래로 막혀 있는 경우 (내려가야 하는데 못 내려가는 경우)
			// 점프 중단
			else if (fJumpTimePassed >= 0.f && m_dCollidedDir.down < 0)
			{
				m_fJumpAccTime = 0.f;
				m_bIsJumping = false;
				GetAnimator()->FindAnimation(m_strCurrAnim)->SetFrame(0);
			}

			// 평범하게 뚫려 있다면 점프 계속
			else
			{
				// 시간이 지났을 경우 점프 중단
				if (fJumpTimePassed >= m_fJumpHalfDuration)
				{
					m_fJumpAccTime = 0.f;
					m_bIsJumping = false;
					GetAnimator()->FindAnimation(m_strCurrAnim)->SetFrame(0);
				}
				else
				{
					vPos.y += fJumpTimePassed * m_fJumpSpeed * fDeltaTime;

					if (m_fIsMovingRight)
						m_strCurrAnim = L"player jump_right";
					else
						m_strCurrAnim = L"player jump_left";
				}
			}
		}

		SetPosition(vPos);

		if (KEY_TAP(KEY::A))
			CreateBullet(m_fIsMovingRight);

		if (GetAnimator())
		{
			GetAnimator()->PlayAnimation(m_strCurrAnim, bAnimRepeat);

			GetAnimator()->update();
		}
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{
	// =============
	//    HP 감소
	// =============
	//
	CObject* pCollidedObj = _pOther->GetOwnerObject();

	if (pCollidedObj->GetType() == OBJECT_TYPE::MONSTER)
	{
		int iHP = GetHP() - pCollidedObj->GetDeal();

		if (iHP < 0)
			iHP = 0;

		SetHP(iHP);
	}
}

void CPlayer::render(HDC _dc)
{
	CObject::render(_dc);

	// =============
	//  디버그용 코드
	// =============
	//
	Vector2 vPos = CCamera::GetInstance()->GetRenderPosition(GetPosition());
	wstring strHP = std::to_wstring(GetHP());

	DRAW_LOG(_dc, 100, 150, 180, vPos.x, vPos.y, strHP, -60.f);
}

void CPlayer::CreateBullet(float _bMovingRight)
{
	// 총알 위치
	Vector2 vPos = GetPosition();
	vPos.x += _bMovingRight * GetScale().x * 0.5f;

	// 총알 생성
	CBullet* pBullet = new CBullet(OBJECT_TYPE::BULLET_PLAYER, GetDeal());

	pBullet->SetPosition(vPos);
	pBullet->SetDirection(Vector2(_bMovingRight, 0.f));

	CreatEventObject(pBullet);
}