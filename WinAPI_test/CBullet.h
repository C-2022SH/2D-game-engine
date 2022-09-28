#pragma once
#include "CObject.h"


class CBullet :
    public CObject
{
private:
    int     m_iDeal;

    // 이동 방향
    float   m_fSpeed;
    Vector2 m_vDir;

    float   m_fAccTime;
    float   m_fFlyDuration;


public:
    int GetDeal() { return m_iDeal; }

    void SetDirection(Vector2 _vDir) { m_vDir = _vDir; }


private:
    virtual void update();


public:
    virtual void OnTriggerEnter(CCollider* _pOther);

    CLONE(CBullet);
    

public:
    CBullet(OBJECT_TYPE _eType, UINT _iDeal, wstring _strName = L"bullet");
    ~CBullet();
};