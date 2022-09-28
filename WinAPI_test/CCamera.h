#pragma once


class CObject;
class CCore;

class CCamera
{
	SINGLETON(CCamera);

private:
	Vector2		m_vCenter;				// 센터 기준점 벡터 - 이 점에 카메라 센터를 두겠다
	Dir4		m_dBound;				// 카메라 바운더리
	Vector2		m_vLookPosition;		// 카메라가 보고 있는 위치
	Vector2		m_vMovingLookPosition;	// 보정 중 위치
	Vector2		m_vPrevlookPosition;	// 카메라가 이전 프레임에 보고 있는 위치
	Vector2		m_vOffset;				// 카메라 오프셋 - 이 점을 센터라고 한다. (ex: 원점 0.f, 0.f)

	CObject*	m_pTargetObject;		// 카메라가 따라갈 오브젝트
	Vector2		m_vDifference;			// 카메라 위치와 해상도 중앙 위치의 차이

	float		m_fFollowTime;			// 카메라가 위치 보정하는 시간
	float		m_fProportionalConst;	// 속도 비례상수
	float		m_fFollowAccTime;		// 따라가는 동안 누적된 시간


public:
	void SetLookPosition(const float& _fMoveDist)
	{
		m_fProportionalConst = -2.f * _fMoveDist / (m_fFollowTime * m_fFollowTime);
		m_fFollowAccTime = 0.f;
	}

	void SetTarget(CObject* _pTarget) { m_pTargetObject = _pTarget; }
	void SetBound(Dir4 _dBound) { m_dBound = _dBound; }

	Vector2 GetCenter() { return m_vCenter; }
	Vector2 GetLookPosition() { return m_vMovingLookPosition; }
	Vector2 GetRenderPosition(Vector2 _vPos) { return _vPos - m_vDifference; }
	Vector2 GetRealPosition(Vector2 _vRenderPos) { return _vRenderPos + m_vDifference; }


public:
	void update();


private:
	void CalculateDiff();
};

