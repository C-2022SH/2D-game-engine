#pragma once
#include "CObject.h"

class CBar;

class CPlayer :
    public CObject
{

private:
    wstring m_strCurrAnim;          // 현재 플레이 중인 애니메이션

    float   m_fPlayerSpeed;
    float   m_fJumpHeight;
    float   m_fJumpHalfDuration;    // 올라가는 데 걸리는 시간
    float   m_fJumpSpeed;           // 초당 이동 속도
    float   m_fJumpAccTime;

    bool    m_bIsJumping;
    float   m_fIsMovingRight;       // 1 이면 오른쪽 -1 이면 왼쪽! 단위벡터로 만들 때 편하도록 +- 1.f 로 설정한다.

    Dir4    m_dBound;


public:
    Dir4 GetBound() { return m_dBound; }
    void SetBound(Dir4 _dBound) { m_dBound = _dBound; }

private:
    void CreateBullet(float _bMovingRight);


public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnCollisionEnter(CCollider* _pOther);

    // 복사 못함!
    virtual CObject* Clone() { return nullptr; }
    CPlayer(const CPlayer& _origin) = delete;

public:
    CPlayer(Vector2 _vPos, bool _bIsStatic, wstring _strName = L"player", Vector2 _vCollider = Vector2(30.f, 30.f), bool _bIsTrigger = false);

    /*
    * 자식파트 복사생성자 굳이 만든다면 이렇게 만든다!
    * 
    * CPlayer(const CPlayer& _origin) :
    *   CObject(_origin)
    * {}
    */

    ~CPlayer();
};

