#include "pch.h"
#include "CTimeManager.h"

#include "CCore.h"


CTimeManager::CTimeManager() :
	m_llPrevCount{},
	m_llCurrCount{},
	m_llFrequency{},
	m_dDeltaTime(0.),
	m_iCallCount(0)
{}

CTimeManager::~CTimeManager()
{}

void CTimeManager::initialize()
{
	// 현재 카운트 값; 초당 100만단위로 카운트
	QueryPerformanceCounter(&m_llPrevCount);

	// 초당 카운트 프리퀀시
	QueryPerformanceFrequency(&m_llFrequency);
}

// 한 프레임에 걸린 시간 계산
void CTimeManager::update()
{
	QueryPerformanceCounter(&m_llCurrCount);

	// 이전 프레임의 카운트와 현재 프레임의 카운트 차이를 초당 카운트 횟수로 나눈다 - 몇 초 걸렸는지 알 수 있음!
	m_dDeltaTime = (m_llCurrCount.QuadPart - m_llPrevCount.QuadPart) / (double) m_llFrequency.QuadPart;

	// 이전 카운트 값 갱신
	m_llPrevCount = m_llCurrCount;


#ifdef _DEBUG
	if (m_dDeltaTime > (1. / 60.))
		m_dDeltaTime = (1. / 60.);
#endif // _DEBUG


	++m_iCallCount;
	m_dAccum += m_dDeltaTime;

	if (m_dAccum >= 1.)
	{
		m_iFPS = m_iCallCount;

		m_dAccum = 0.;
		m_iCallCount = 0;

		wchar_t szBuffer[255] = {};
		swprintf_s(szBuffer, L"FPS : %d,	DT : %f", m_iFPS, m_dDeltaTime);
		SetWindowText(CCore::GetInstance()->GetMainHwnd(), szBuffer);
	}
}
