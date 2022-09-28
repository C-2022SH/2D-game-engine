#pragma once

// 프레임 동기화
// 사용 이유
// 1. 동일 프레임 내에서는 같은 키에 대해 동일한 이벤트를 가져간다. (윈도우 async 를 사용하지 않아도된다)
// 2. 키 입력 이벤트를 처리한다. (ex: tap, hold, away ...)


enum class KEY_STATE
{
	NONE,	// 직전에도 지금도 입력하지 않음
	TAP,	// 직전까지 입력하지 않다가 현재 처음으로 입력함
	HOLD,	// 이전에도 지금도 계속 입력 중
	AWAY,	// 직전까지는 입력되었으나 현재는 입력하지 않음
};

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	CLICK_LEFT,
	CLICK_RIGHT,

	LAST
};

struct tKeyInfo
{
	KEY_STATE	eState;		// 키 입력 상태
	bool		bPrevPush;	// 이전 프레임에서 눌렸는지 여부
};

class CKeyManager
{
	SINGLETON(CKeyManager);

private:
	vector<tKeyInfo>	m_vKey;
	Vector2				m_vCurrMousePosition;

public:
	void initialize();
	void update();

public:
	KEY_STATE GetKeyState(KEY _eKey) { return m_vKey[(int)_eKey].eState; }
	Vector2 GetMousePosition() { return m_vCurrMousePosition; }
};

