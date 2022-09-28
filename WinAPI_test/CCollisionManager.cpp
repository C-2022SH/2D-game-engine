#include "pch.h"
#include "CCollisionManager.h"

#include "CSceneManager.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"


CCollisionManager::CCollisionManager() :
	m_arrCheck{}
{
}

CCollisionManager::~CCollisionManager()
{
}


void CCollisionManager::initialize()
{
}


void CCollisionManager::update()
{
	for (UINT iRow = 0; iRow < (UINT)OBJECT_TYPE::END; iRow++)
	{
		for (UINT iCol = iRow; iCol < (UINT)OBJECT_TYPE::END; iCol++)
		{
			// 두 그룹이 충돌하도록 되어있다면 충돌 체크
			if (m_arrCheck[iRow] & (1 << iCol))
				collisionGroup_update((OBJECT_TYPE)iRow, (OBJECT_TYPE)iCol);
		}
	}
}

void CCollisionManager::collisionGroup_update(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight)
{
	CScene* pCurScene = CSceneManager::GetInstance()->GetCurrentScene();

	// 원본을 반환받을 때는 반환 받는 변수도 레퍼런스 타입으로!
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	// 이전 충돌 정보를 받아 올 iterator
	map<ULONGLONG, bool>::iterator iter;


	// 두 그룹의 물체들을 전부 받아온다
	// 같은 그룹에 있는 경우 두 번 체크된다! 주의
	for (size_t i = 0; i < vecLeft.size(); i++)
	{
		CCollider* pLeftCollider = vecLeft[i]->GetCollider();

		if (pLeftCollider == nullptr)
			continue;


		for (size_t j = 0; j < vecRight.size(); j++)
		{
			CCollider* pRightCollider = vecRight[j]->GetCollider();

			// 자기 자신인 경우에도 체크 안함!!
			if (pRightCollider == nullptr || vecLeft[i] == vecRight[j])
				continue;


			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCollider->GetID();
			ID.iRight_id = pRightCollider->GetID();

			iter = m_mapCollisionInfo.find(ID.ID);


			// 이전에 등록도 되어 있지 않다면 이전 충돌 정보 false
			if (iter == m_mapCollisionInfo.end())
			{
				m_mapCollisionInfo.insert(make_pair(ID.ID, false));
				iter = m_mapCollisionInfo.find(ID.ID);
			}
			

			// ==================
			// |	충돌 액션		|
			// ==================
			// 
			// 현재 충돌 중일 경우
			if (isCollided(pLeftCollider, pRightCollider))
			{
				bool bIsTriggerAction = pLeftCollider->IsTrigger() || pRightCollider->IsTrigger();

				// 이전에도 충돌한 경우
				if (iter->second)
				{
					// 둘 중 하나가 삭제 예정인 경우 해제
					if (!vecLeft[i]->IsAlive() || !vecRight[j]->IsAlive())
					{
						// 둘 중 하나가 트리거라면 트리거 처리
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTriggerExit(pRightCollider);
							pRightCollider->OnTriggerExit(pLeftCollider);
						}
						else
						{
							pLeftCollider->OnCollisionExit(pRightCollider);
							pRightCollider->OnCollisionExit(pLeftCollider);
						}

						iter->second = false;
					}

					// 아닌 경우 충돌
					else
					{
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTrigger(pRightCollider);
							pRightCollider->OnTrigger(pLeftCollider);
						}
						else
						{
							pLeftCollider->OnCollision(pRightCollider);
							pRightCollider->OnCollision(pLeftCollider);
						}
					}
				}

				// 이전에는 충돌하지 않은 경우
				else
				{
					// 둘 다 삭제 예정이 아닌 경우에만 작동
					if (vecLeft[i]->IsAlive() && vecRight[j]->IsAlive())
					{
						if (bIsTriggerAction)
						{
							pLeftCollider->OnTriggerEnter(pRightCollider);
							pRightCollider->OnTriggerEnter(pLeftCollider);
						}
						else
						{
							Dir4 dCollidedDir = check_collisionDir(pLeftCollider, pRightCollider);

							pLeftCollider->OnCollisionEnter(dCollidedDir, pRightCollider);
							pRightCollider->OnCollisionEnter(dCollidedDir.Reverse(), pLeftCollider);
						}

						iter->second = true;
					}
				}
			}

			// 현재 충돌 중이지 않을 경우 액션
			else
			{
				// 이전에는 충돌한 경우
				if (iter->second)
				{
					if (pLeftCollider->IsTrigger() || pRightCollider->IsTrigger())
					{
						pLeftCollider->OnTriggerExit(pRightCollider);
						pRightCollider->OnTriggerExit(pLeftCollider);
					}
					else
					{
						pLeftCollider->OnCollisionExit(pRightCollider);
						pRightCollider->OnCollisionExit(pLeftCollider);
					}

					iter->second = false;
				}
			}
		}
	}
}


bool CCollisionManager::isCollided(CCollider* _pLeft, CCollider* _pRight)
{
	return check_axisCollision(_pLeft, _pRight) && check_axisCollision(_pRight, _pLeft);
}


bool CCollisionManager::check_axisCollision(CCollider* _pTarget, CCollider* _pOther)
{
	Vector2 vCenterDiff = _pTarget->GetFinalPosition() - _pOther->GetFinalPosition();

	// x축
	Vector2 vBaseAxis = _pTarget->GetRotation();

	Vector2 vBaseRelative = _pTarget->GetDirVector(1.f, 1.f);
	Vector2 vOtherRelative = _pOther->GetDirVector(1.f, 1.f);

	float fTest = abs(vCenterDiff.Dot(vBaseAxis)) - abs(vBaseRelative.Dot(vBaseAxis));

	if ((fTest >= abs(_pOther->GetDirVector(1.f, 1.f).Dot(vBaseAxis)))
		&& (fTest >= abs(_pOther->GetDirVector(-1.f, 1.f).Dot(vBaseAxis))))
		return false;


	// y 축
	vBaseAxis.Reverse();

	if (vBaseAxis.x != 0 && vBaseAxis.y != 0)
	{
		vBaseAxis.x = -vBaseAxis.x;
	}

	fTest = abs(vCenterDiff.Dot(vBaseAxis)) - abs(vBaseRelative.Dot(vBaseAxis));

	if ((fTest >= abs(_pOther->GetDirVector(1.f, 1.f).Dot(vBaseAxis)))
		&& (fTest >= abs(_pOther->GetDirVector(-1.f, 1.f).Dot(vBaseAxis))))
		return false;

	return true;
}


Dir4 CCollisionManager::check_collisionDir(CCollider* _pLeft, CCollider* _pRight)
{
	// 꼭짓점 정렬
	Vector2 arrLeftVertices[4];
	Vector2 arrRightVertices[4];

	order_vertices(arrLeftVertices, _pLeft);
	order_vertices(arrRightVertices, _pRight);

	// ===========================
	//  한쪽 방향으로만 충돌하는 경우
	// ===========================
	// 
	// --- 수직(상하) ---
	// 
	// 나의 최상단과 상대의 최하단이 동일하다면 - 위로만 충돌
	if (abs(arrLeftVertices[1].y - arrRightVertices[3].y) < COLL_BOUND)
		return Dir4(0, 1, 0, 0);

	// 나의 최하단과 상대의 최상단이 동일하다면 - 아래로만 충돌
	else if (abs(arrLeftVertices[3].y - arrRightVertices[1].y) < COLL_BOUND)
		return Dir4(0, 0, 0, 1);

	// --- 수평(좌우) ---
	// 
	// 나의 최우단과 상대의 최좌단이 동일하다면 - 오른쪽으로만 충돌
	if (abs(arrLeftVertices[2].x - arrRightVertices[0].x) < COLL_BOUND)
		return Dir4(0, 0, 1, 0);

	// 나의 최좌단과 상대의 최우단이 동일하다면 - 왼쪽으로만 충돌
	else if (abs(arrLeftVertices[0].x - arrRightVertices[2].x) < COLL_BOUND)
		return Dir4(1, 0, 0, 0);


	// ========================
	//  두 방향으로 충돌하는 경우
	// ========================
	// 
	Dir4 dCollidedDir = Dir4();

	// --- 수직(상하) ---
	//
	// 나의 최좌단과 최우단 사이에 상대의 최우단이 있다.
	if (arrLeftVertices[0].x > arrRightVertices[0].x && arrLeftVertices[2].x > arrRightVertices[2].x)
	{
		// 나의 최좌단이 상대의 최우단보다 아래에 있다면 - 위에서 충돌
		if (arrLeftVertices[0].y > arrRightVertices[2].y)
			dCollidedDir += Dir4(0, 1, 0, 0);

		// 아니라면 - 아래에서 충돌
		else
			dCollidedDir += Dir4(0, 0, 0, 1);
	}

	// 나의 최좌단과 최우단 사이에 상대의 최좌단이 있다.
	else if (arrLeftVertices[0].x < arrRightVertices[0].x && arrLeftVertices[2].x < arrRightVertices[2].x)
	{
		// 나의 최우단이 상대의 최좌단보다 아래에 있다면 - 위에서 충돌
		if (arrLeftVertices[2].y > arrRightVertices[0].y)
			dCollidedDir += Dir4(0, 1, 0, 0);

		// 아니라면 아래에서 - 충돌
		else
			dCollidedDir += Dir4(0, 0, 0, 1);
	}

	// 상대의 좌우 폭이 나보다 크다. (내가 상대 안에 쏙 들어가 있다)
	else if (arrRightVertices[0].x < arrLeftVertices[0].x && arrRightVertices[2].x > arrLeftVertices[2].x)
	{
		float fDX = arrRightVertices[0].x - arrRightVertices[2].x;

		// 대각선이 수평인 경우 그냥 y 값만 비교
		if (fDX == 0)
		{
			// 상대의 중심이 나보다 아래쪽에 있다면 - 아래에서 충돌
			if (_pLeft->GetFinalPosition().y < _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);

			// 상대의 중심이 나보다 위쪽에 있다면 - 위에서 충돌
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}

		// 아닌 경우 일차함수 공간 분할으로 비교
		else
		{
			float fSlope = (arrRightVertices[0].y - arrRightVertices[2].y) / fDX;

			// 나의 중심이 대각선보다 위에 있다면 - 아래에서 충돌
			if ((fSlope * (_pLeft->GetFinalPosition().x - _pRight->GetFinalPosition().x) + _pRight->GetFinalPosition().y)
				> _pLeft->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);
			
			// 나의 중심이 대각선보다 아래에 있다면 - 위에서 충돌
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}
	}

	// 나의 좌우 폭이 상대보다 크다. (상대가 내 안에 쏙 들어가 있다)
	else
	{
		float fDX = arrLeftVertices[0].x - arrLeftVertices[2].x;

		// 대각선이 수평인 경우 그냥 y 값만 비교
		if (fDX == 0)
		{
			// 상대의 중심이 나보다 아래쪽에 있다면 - 아래에서 충돌
			if (_pLeft->GetFinalPosition().y < _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 0, 0, 1);

			// 상대의 중심이 나보다 위쪽에 있다면 - 위에서 충돌
			else
				dCollidedDir += Dir4(0, 1, 0, 0);
		}

		// 아닌 경우 일차함수 공간 분할으로 비교
		else
		{
			float fSlope = (arrLeftVertices[0].y - arrLeftVertices[2].y) / fDX;

			// 상대의 중심이 대각선보다 위에 있다면 - 위에서 충돌
			if ((fSlope * (_pRight->GetFinalPosition().x - _pLeft->GetFinalPosition().x) + _pLeft->GetFinalPosition().y)
				> _pRight->GetFinalPosition().y)
				dCollidedDir += Dir4(0, 1, 0, 0);

			// 상대의 중심이 대각선보다 아래에 있다면 - 아래에서 충돌
			else
				dCollidedDir += Dir4(0, 0, 0, 1);
		}
	}


	// --- 수평(좌우) ---
	//
	// 나의 최상단과 최하단 사이에 상대의 최상단이 있다.
	if (arrLeftVertices[1].y < arrRightVertices[1].y && arrLeftVertices[3].y < arrRightVertices[3].y)
	{
		// 나의 최하단이 상대의 최상단보다 오른쪽에 있다면 - 왼쪽에서 충돌
		if (arrLeftVertices[3].x > arrRightVertices[1].x)
			dCollidedDir += Dir4(1, 0, 0, 0);

		// 아니라면 - 오른쪽에서 충돌
		else
			dCollidedDir += Dir4(0, 0, 1, 0);
	}

	// 나의 최상단과 최하단 사이에 상대의 최하단이 있다.
	else if (arrLeftVertices[1].y < arrRightVertices[1].y && arrLeftVertices[3].y < arrRightVertices[3].y)
	{
		// 나의 최상단이 상대의 최하단보다 오른쪽에 있다면 - 오른쪽에서 충돌
		if (arrLeftVertices[1].x > arrRightVertices[3].x)
			dCollidedDir += Dir4(1, 0, 0, 0);

		// 아니라면 - 왼쪽에서 충돌
		else
			dCollidedDir += Dir4(0, 0, 1, 0);
	}

	// 상대의 상하 폭이 나보다 크다. (내가 상대 안에 쏙 들어가 있다)
	else if (arrRightVertices[1].y < arrLeftVertices[1].y && arrRightVertices[3].y > arrLeftVertices[3].y)
	{
		float fDY = arrRightVertices[1].y - arrRightVertices[3].y;

		// 대각선이 수평인 경우 그냥 x 값만 비교
		if (fDY == 0)
		{
			// 상대의 중심이 나보다 오른쪽에 있다면 - 오른쪽에서 충돌
			if (_pLeft->GetFinalPosition().x < _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// 상대의 중심이 나보다 왼쪽에 있다면 - 왼쪽에서 충돌
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}

		// 아닌 경우 일차함수 공간 분할으로 비교
		else
		{
			float fSlope = (arrRightVertices[1].x - arrRightVertices[3].x) / fDY;

			// 나의 중심이 대각선보다 왼쪽에 있다면 - 오른쪽에서 충돌
			if ((fSlope * (_pLeft->GetFinalPosition().y - _pRight->GetFinalPosition().y) + _pRight->GetFinalPosition().x)
				> _pLeft->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// 나의 중심이 대각선보다 오른쪽에 있다면 - 왼쪽에서 충돌
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}
	}

	// 나의 상하 폭이 상대보다 크다. (상대가 내 안에 쏙 들어가 있다)
	else
	{
		float fDY = arrLeftVertices[1].y - arrLeftVertices[3].y;

		// 대각선이 수평인 경우 그냥 x 값만 비교
		if (fDY == 0)
		{
			// 상대의 중심이 나보다 오른쪽에 있다면 - 오른쪽에서 충돌
			if (_pLeft->GetFinalPosition().x < _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(0, 0, 1, 0);

			// 상대의 중심이 나보다 왼쪽에 있다면 - 왼쪽에서 충돌
			else
				dCollidedDir += Dir4(1, 0, 0, 0);
		}

		// 아닌 경우 일차함수 공간 분할으로 비교
		else
		{
			float fSlope = (arrLeftVertices[1].x - arrLeftVertices[3].x) / fDY;

			// 상대의 중심이 대각선보다 왼쪽에 있다면 - 왼쪽에서 충돌
			if ((fSlope * (_pRight->GetFinalPosition().y - _pLeft->GetFinalPosition().y) + _pLeft->GetFinalPosition().x)
			> _pRight->GetFinalPosition().x)
				dCollidedDir += Dir4(1, 0, 0, 0);

			// 상대의 중심이 대각선보다 오른쪽에 있다면 - 오른쪽에서 충돌
			else
				dCollidedDir += Dir4(0, 0, 1, 0);
		}
	}

	return dCollidedDir;
}

// _arrVertices 의 인덱스는 [0 최좌단, 1 최상단, 2 최우단, 3 최하단] 을 의미한다.
void CCollisionManager::order_vertices(Vector2* _arrVertices, CCollider* _pTarget)
{
	// ===========================
	//  수직 방향 극단 점들을 찾는다.
	// ===========================
	// 
	// 가장 오른쪽에 있는 점을 기준으로 한다.
	// 회전각이 0 ~ 90 도 사이일 경우 중심으로부터 [1.f, 1.f] 방향의 점이 가장 오른쪽에 있을 확률이 높으므로 초기값을 이렇게 설정한다.
	float fHoriMax_x = 1.f;
	float fHoriMax_y = 1.f;

	// 이웃하는 점의 절댓값이 나보다 크다면 (걔가 수평방향 극단값이라는 뜻)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x) < abs(_pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).x))
	{
		// 이웃하는 점을 나라고 한다.
		fHoriMax_y *= -1.f;
	}

	// 사실 내가 음수였다면 (== 사실 내가 가장 왼쪽의 점이었다면)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x) != _pTarget->GetDirVector(fHoriMax_x, fHoriMax_y).x)
	{
		// 대각선 방향의 점을 나라고 한다.
		fHoriMax_x *= -1.f;
		fHoriMax_y *= -1.f;
	}

	// 수평 방향 극단 점의 좌표를 저장한다.
	_arrVertices[2] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(fHoriMax_x, fHoriMax_y);
	_arrVertices[0] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(-fHoriMax_x, -fHoriMax_y);


	// ===========================
	//  수직 방향 극단 점들을 찾는다.
	// ===========================
	// 
	// 아까 최하단 점의 후보를 찾았으니 그쪽에서부터 조사를 시작한다.
	// 사실 내가 음수였다면 (== 사실 내가 가장 위쪽의 점이었다면)
	if (abs(_pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).y) != _pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y).y)
	{
		// 양수를 나라고 한다.
		fHoriMax_x *= -1.f;
		fHoriMax_y *= -1.f;
	}

	// 수직 방향 극단 점의 좌표를 저장한다.
	_arrVertices[1] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(-fHoriMax_x, fHoriMax_y);
	_arrVertices[3] = _pTarget->GetFinalPosition() + _pTarget->GetDirVector(fHoriMax_x, -fHoriMax_y);
}


void CCollisionManager::CheckGroup(OBJECT_TYPE eLeft, OBJECT_TYPE eRight)
{
	/*
	 비트를 오른쪽부터 카운트한다. (밀기 편하라고)
	 0 (8,4,2,1,0)
	 1 (8,4,2,1,0)
	 이런 식으로!
	 
	 따라서 행렬의 좌상단 부분만 사용한다. 행 <= 열
	*/
	
	UINT iRow = (UINT)eLeft;
	UINT iCol = (UINT)eRight;

	if (iRow > iCol)
	{
		iCol = (UINT)eLeft;
		iRow = (UINT)eRight;
	}

	// 있으면 넣고 없으면 뺀다.
	m_arrCheck[iRow] ^= (1 << iCol);
}