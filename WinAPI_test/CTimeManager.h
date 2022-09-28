#pragma once

// 시간 동기화 매니저


class CTimeManager
{
	SINGLETON(CTimeManager);

private:
	// FPS; deltaTime
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurrCount;
	LARGE_INTEGER	m_llFrequency;

	double			m_dDeltaTime;	// 프레임 사이의 시간
	double			m_dAccum;		// deltaTime 누적값
	UINT			m_iCallCount;	// 초당 호출 횟수
	UINT			m_iFPS;			// 초당 호출 횟수를 1초 간 누적

public:
	void initialize();
	void update();

public:
	double GetDeltaTime() { return m_dDeltaTime; }
	float GetfDeltaTime() { return (float)m_dDeltaTime; }
};

