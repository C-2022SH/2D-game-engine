#pragma once
#include "CObject.h"


class CGenerator :
    public CObject
{
private:
    int                 m_iEnemyCount;
    float               m_fSpawnDuration;
    float               m_fAccSpawnTime;

private:
    void SpawnMonster(int _iCount);

public:
    virtual void update();
    virtual void render(HDC _dc);

    virtual void OnTriggerEnter(CCollider* _pOther);

    CLONE(CGenerator);

public:
    CGenerator(Vector2 _vPos, Vector2 _vScale);
    ~CGenerator();
};

