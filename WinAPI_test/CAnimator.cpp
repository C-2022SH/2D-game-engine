#include "pch.h"
#include "CAnimator.h"

#include "CObject.h"
#include "CAnimation.h"


CAnimator::CAnimator(CObject* _owner) :
	m_pCurAnimation(nullptr),
	m_pOwner(_owner),
	m_bRepeat(false)
{
}

CAnimator::CAnimator(CAnimator& _origin, CObject* _owner) :
	m_pCurAnimation(nullptr),
	m_pOwner(_owner),
	m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Delete_Map(m_mapAnimations);
}


void CAnimator::update()
{
	if (m_pCurAnimation)
	{
		if (m_bRepeat && m_pCurAnimation->IsFinished())
			m_pCurAnimation->SetFrame(0);

		m_pCurAnimation->update();
	}
}

void CAnimator::render(HDC _dc)
{
	if (m_pCurAnimation)
		m_pCurAnimation->render(_dc);
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTexture, Vector2 _vLeftTop, Vector2 _vSliceSize, Vector2 _vStep,
						float _fDuration, UINT _iFrameCount, Vector2 _vOffset)
{
	CAnimation* pAnim = FindAnimation(_strName);

	if (pAnim == nullptr)
	{
		pAnim = new CAnimation;

		pAnim->SetName(_strName);
		pAnim->m_pAnimator = this;
		pAnim->CreateAnimation(_pTexture, _vLeftTop, _vSliceSize, _vStep, _fDuration, _iFrameCount, _vOffset);

		m_mapAnimations.insert(make_pair(_strName, pAnim));
	}
	else
		return;
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimations.find(_strName);

	if (iter == m_mapAnimations.end())
		return nullptr;

	return iter->second;
}

void CAnimator::PlayAnimation(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnimation = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}
