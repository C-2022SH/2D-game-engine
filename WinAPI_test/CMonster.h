#pragma once
#include "CObject.h"

class CTexture;

class CMonster :
    public CObject
{
private:
    float   m_fSpeed;


public:
    float GetSpeed() { return m_fSpeed; }

    void SetSpeed(float _f) { m_fSpeed = _f; }


public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnTriggerEnter(CCollider* _pOther);

    CLONE(CMonster);


public:
    CMonster(Vector2 _vPos, wstring _strName = L"monster", Vector2 _vCollider = Vector2(30.f, 30.f));
    ~CMonster();
};

