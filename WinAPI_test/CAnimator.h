#pragma once


class CObject;
class CAnimation;
class CTexture;

class CAnimator
{
private:
	map<wstring, CAnimation*>	m_mapAnimations;	// 모든 애니메이션
	CAnimation*					m_pCurAnimation;	// 현재 재생 중인 애니메이션
	CObject*					m_pOwner;			// 애니메이터를 소유한 오브젝트
	bool						m_bRepeat;			// 반복재생 여부


public:
	CObject* GetOwnerObject() { return m_pOwner; }

public:
	void update();
	void render(HDC _dc);

	void CreateAnimation(const wstring& _strName, CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep,
					float _fDuration, UINT _iFrameCount, Vector2 _vOffset = Vector2(0.f, 0.f));
	CAnimation* FindAnimation(const wstring& _strName);
	void PlayAnimation(const wstring& _strName, bool _bRepeat);


public:
	CAnimator(CObject* _owner);
	CAnimator(CAnimator& _origin, CObject* _owner);
	~CAnimator();

};

