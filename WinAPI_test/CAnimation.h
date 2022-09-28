#pragma once


class CAnimator;
class CTexture;

struct tAnimFrame
{
	Vector2	vLeftTop;
	Vector2	vSlice;
	Vector2	vOffset;
	float	fDuration;
};

class CAnimation
{
private:
	wstring				m_strName;
	CAnimator*			m_pAnimator;
	CTexture*			m_pTexture;
	int					m_iCurFrame;			// 애니메이션의 현재 프레임
	float				m_fAccTime;				// 애니메이션 누적 재생 시간
	bool				m_bIsFinishedUpdate;	// 업데이트에서 finish 호출됨
	bool				m_bIsFinished;			// 애니메이션 한 번 재생이 종료되었음을 알림

	vector<tAnimFrame>	m_vecFrame;


private:
	void SetName(const wstring& _strName) { m_strName = _strName; }


public:
	bool IsFinished() { return m_bIsFinished; }
	const wstring& GetName() { return m_strName; }
	tAnimFrame& GetFrame(int _iIndex) { return m_vecFrame[_iIndex]; }
	
	void SetFrame(int _iFrame)
	{
		m_bIsFinishedUpdate = false;
		m_bIsFinished = false;
		m_iCurFrame = _iFrame;
	}

	void CreateAnimation(CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep, float _fDuration,
					UINT _iFrameCount, Vector2 _Offset);


public:
	void update();
	void render(HDC _dc);

public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

