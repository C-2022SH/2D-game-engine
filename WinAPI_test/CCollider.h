#pragma once


class CObject;
class CText;

class CCollider
{
private:
	static UINT		g_iNextID;

	CObject*		m_pOwner;

	// 크기
	Vector2			m_vScale;				// 콜라이더 크기

	// 위치
	Vector2			m_vOffset;				// 기준으로부터의 상대적인 거리
	Vector2			m_vFinalPosition;		// 오브젝트의 위치를 기준으로 계산된 최종 포지션

	// 회전
	float			m_fCos;					// 방향 벡터에 대한 코사인 값
	float			m_fSin;					// 방향 벡터에 대한 사인 값

	Vector2			m_vRotation;

	UINT			m_iID;					// 겹치지 않는 콜라이더 고유 ID (충돌체 간의 충돌이 고유하도록)
	int				m_iCollided;			// 부딪힌 콜라이더 개수
	bool			m_bIsTrigger;			// 충돌체가 trigger 인지 collision 인지


protected:
	map<CCollider*, Dir4>	m_mapCollidedDir;	// 부딪힌 각 콜라이더가 막힌 방향
												// x > 0 오른쪽, x < 0 왼쪽
												// y > 0 아래, y < 0 위
	Vector2					m_vCollidedDir;		// 모든 막힌 방향의 합


public:
	CObject* GetOwnerObject() { return m_pOwner; }
	Vector2 GetCollidedDir() { return m_vCollidedDir; }

	Vector2 GetOffset() { return m_vOffset; }
	Vector2 GetFinalPosition() { return m_vFinalPosition; }
	Vector2 GetScale() { return m_vScale; }

	Vector2 GetRotation() { return m_vRotation; }
	Vector2 GetDirVector(float _fHori, float _fVerti)
	{
		Vector2 vHalfScale = m_vScale * 0.5f;

		float fX = _fHori * vHalfScale.x * m_fCos - _fVerti * vHalfScale.y * m_fSin;
		float fY = _fHori * vHalfScale.x * m_fSin + _fVerti * vHalfScale.y * m_fCos;

		return Vector2(fX, fY);
	}

	Vector2 GetRotatedVertex(float _fHori, float _fVerti)
	{
		Vector2 vHalfScale = m_vScale * 0.5f;

		float fX = (_fHori * vHalfScale.x * m_fCos - _fVerti * vHalfScale.y * m_fSin) + m_vFinalPosition.x;
		float fY = (_fHori * vHalfScale.x * m_fSin + _fVerti * vHalfScale.y * m_fCos) + m_vFinalPosition.y;

		return Vector2(fX, fY);
	}
	
	UINT GetID() { return m_iID; }
	
	bool IsTrigger() { return m_bIsTrigger; }
	bool IsRotated() { if (m_fCos == 1 && m_fSin == 0) return false; else return true; }

	void SetScale(Vector2 _vScale) { m_vScale = Vector2(abs(_vScale.x), abs(_vScale.y)); }
	void SetOffset(Vector2 _vPos) { m_vOffset = _vPos; }

	void SetRotation(Vector2 _vRot)
	{
		assert(_vRot != Vector2());

		m_vRotation = _vRot;

		float fRotVecLength = _vRot.Length();

		m_fCos = _vRot.x / fRotVecLength;
		m_fSin = _vRot.y / fRotVecLength;
	}

	void SetIsTrigger(bool _bTrig) { m_bIsTrigger = _bTrig; }


public:
	// 오브젝트의 위치를 따라간다.
	void finalUpdate();
	void render(HDC _dc);

public:
	// 충돌할 경우 처리
	void OnCollisionEnter(Dir4 _dCollidedDir, CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);

	void OnTriggerEnter(CCollider* _pOther);
	void OnTrigger(CCollider* _pOther);
	void OnTriggerExit(CCollider* _pOther);


public:
	CCollider(CObject* _owner, bool _bIsTrigger, Vector2 _vRot = Vector2(1.f, 0.f));

	// 복사되는 경우
	CCollider(const CCollider& _origin, CObject* _owner);
	virtual ~CCollider();

	// 콜라이더는 대입할 수 없다.
	CCollider& operator = (CCollider& _origin) = delete;
	//friend class CObject;
};