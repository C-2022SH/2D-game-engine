

/*
* 
* WINAPI, C++
* 
* 충돌체와 플레이어 스크립트 중 일부입니다.
* 
*/


#include "pch.h"

/*
#include "CPlayer.h"

#include "CSceneManager.h"
#include "CScene.h"

#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CResourceManager.h"

#include "CBullet.h"

#include "CAnimator.h"
#include "CCollider.h"

#include "CAnimation.h"



// =============================
// 
//  통과할 수 없는 사각형 충돌체
// 
// =============================
//
void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	++m_iCollided;

	// 어느 방향에서 부딪혔는지
	if (!m_pOwner->IsStatic())
	{
		Vector2 vOtherFinalPos = _pOther->GetFinalPosition();
		Vector2 vOtherScale = _pOther->GetScale();

		Dir4 dCollidedDir = Dir4();

		float fCheckBottomHit = m_vFinalPosition.y + m_vScale.y * 0.5f - (vOtherFinalPos.y - vOtherScale.y * 0.5f);
		float fCheckTopHit = vOtherFinalPos.y + vOtherScale.y * 0.5f - (m_vFinalPosition.y - m_vScale.y * 0.5f);
		float fCheckRightHit = vOtherFinalPos.x - vOtherScale.x * 0.5f - (m_vFinalPosition.x + m_vScale.x * 0.5f);
		float fCheckLeftHit = vOtherFinalPos.x + vOtherScale.x * 0.5f - (m_vFinalPosition.x - m_vScale.x * 0.5f);

		// 아래에서 충돌
		if (abs(fCheckBottomHit) <= 0.8f)
			dCollidedDir.down = 1;

		// 위에서 충돌
		if (abs(fCheckTopHit) <= 0.8f)
			dCollidedDir.up = 1;

		// 오른쪽에서 충돌
		if (abs(fCheckRightHit) <= 0.8f)
			dCollidedDir.right = 1;

		// 왼쪽에서 충돌
		if (abs(fCheckLeftHit) <= 0.8f)
			dCollidedDir.left = 1;


		// 오차 방향 디렉션 업데이트
		Dir4 dFinalColDir = m_pOwner->GetCollidedDir() + dCollidedDir;
		m_pOwner->SetCollidedDir(dFinalColDir);

		m_mapCollidedDir.insert(make_pair(_pOther, dCollidedDir));
	}

	m_pOwner->OnCollisionEnter(_pOther);
}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	--m_iCollided;

	map<CCollider*, Dir4>::iterator iter = m_mapCollidedDir.find(_pOther);

	if (iter != m_mapCollidedDir.end())
	{
		Dir4 dFinalColDir = m_pOwner->GetCollidedDir() - iter->second;
		m_pOwner->SetCollidedDir(dFinalColDir);

		m_mapCollidedDir.erase(_pOther);
	}

	m_pOwner->OnCollisionExit(_pOther);
}




// ==================================
// 
//  2D 플랫포머: 플레이어 이동과 점프
// 
// ==================================
//
void CPlayer::update()
{
	if (!IsStatic())
	{
		Vector2 vPos = GetPosition();

		// 1 이면 오른쪽 -1 이면 왼쪽! 단위벡터로 만들 때 편하도록 +- 1.f 로 설정한다.
		float bIsMovingRight = 1.f;

		// 애니메이션
		m_strCurrAnim = L"player stop";
		bool bAnimRepeat = true;


		if (KEY_HOLD(KEY::RIGHT))
		{
			// 움직일 수 없어도 애니메이션/방향 설정은 바꾸어 준다.
			m_strCurrAnim = L"player walk_right";

			// 막혀 있지 않은 경우 이동
			if (m_dCollidedDir.right <= 0)
				vPos.x += m_fPlayerSpeed * fDeltaTime;
		}

		if (KEY_HOLD(KEY::LEFT))
		{
			m_strCurrAnim = L"player walk_left";
			bIsMovingRight = -1.f;

			if (m_dCollidedDir.left <= 0)
				vPos.x -= m_fPlayerSpeed * fDeltaTime;
		}

		if (KEY_HOLD(KEY::UP))
		{
			m_strCurrAnim = L"player walk_right";

			if (m_dCollidedDir.up <= 0)
			{
				vPos.y -= m_fPlayerSpeed * fDeltaTime;

				// 길에서 벗어난 경우 벗어나지 못하도록 위치 조정
				if (vPos.y < 56.f)
					vPos.y = 56.f;
			}
		}

		if (KEY_HOLD(KEY::DOWN))
		{
			m_strCurrAnim = L"player walk_left";
			bIsMovingRight = -1.f;

			if (m_dCollidedDir.down <= 0)
			{
				vPos.y += m_fPlayerSpeed * fDeltaTime;

				// 길에서 벗어난 경우: 벗어나지 못하도록 위치 조정
				if (vPos.y > 360.f)
					vPos.y = 360.f;
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

					if (bIsMovingRight)
						m_strCurrAnim = L"player jump_right";
					else
						m_strCurrAnim = L"player jump_left";
				}
			}
		}

		SetPosition(vPos);

		if (KEY_TAP(KEY::A))
			CreateBullet(bIsMovingRight);

		if (GetAnimator())
		{
			GetAnimator()->PlayAnimation(m_strCurrAnim, bAnimRepeat);

			GetAnimator()->update();
		}
	}
}
*/