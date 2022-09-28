#pragma once


struct tEvent
{
	EVENT_TYPE	eEvent;		// 이벤트 타입
	// 플랫폼에 따라 자료형(메모리 크기)이 바뀌는 자료형
	DWORD_PTR	lParam;		// 생성할 오브젝트 주소
	DWORD_PTR	pParam;		// 기타 정보
};

class CEventManager
{
	SINGLETON(CEventManager);

private:
	vector<tEvent> m_vecEvent;
	vector<CObject*> m_vecDead;

public:
	void AddEvent(const tEvent& _event) { m_vecEvent.push_back(_event); }

private:
	void ExecuteEvent(const tEvent& _event);


public:
	void update();
};

