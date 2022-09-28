#pragma once


class CCollider;


#define COLL_BOUND 0.7f


union COLLIDER_ID
{
	struct
	{
		UINT	iLeft_id;
		UINT	iRight_id;
	};
	ULONGLONG ID;
};


class CCollisionManager
{
	SINGLETON(CCollisionManager);

private:
	map<ULONGLONG, bool> m_mapCollisionInfo;				// 콜라이더 간 이전 프레임 충돌 정보
	UINT				 m_arrCheck[(UINT)OBJECT_TYPE::END];	// 각 행(배열멤버)의 비트연산으로 collision 을 할지 하지 않을지 여부를 확인한다.


public:
	void initialize();
	void update();
	
public:
	void CheckGroup(OBJECT_TYPE eLeft, OBJECT_TYPE eRight);
	void ReleaseCollider() { memset(m_arrCheck, 0, sizeof(UINT) * (UINT)OBJECT_TYPE::END); }	// 충돌 체크 설정 리셋

private:
	void collisionGroup_update(OBJECT_TYPE _eLeft, OBJECT_TYPE _eRight);
	bool isCollided(CCollider* _pLeft, CCollider* _pRight);

	bool check_axisCollision(CCollider* _pTarget, CCollider* _pOther);
	Dir4 check_collisionDir(CCollider* _pLeft, CCollider* _pRight);
	void check_upDownCollision(Vector2* _arrLeft, Vector2* _arrRight, Vector2 _vLeftCenter, Vector2 _vRightCenter, Dir4* _dCollidedDir);
	void check_leftRightCollision(Vector2* _arrLeft, Vector2* _arrRight, Vector2 _vLeftCenter, Vector2 _vRightCenter, Dir4* _dCollidedDir);
	void order_vertices(Vector2* _arrVertices, CCollider* _pTarget);
};