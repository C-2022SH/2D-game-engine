#pragma once
#include "CObject.h"

class CScene;

class CPortal :
    public CObject
{
private:
    SCENE_TYPE  m_eNextScene;

    float       m_fAccTime;
    float       m_fDelayTime;


public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnTrigger(CCollider* _pOther);

    CLONE(CPortal);


public:
    CPortal(Vector2 _vPos, SCENE_TYPE _eDestination);
    ~CPortal();
};

