#pragma once
#include "CScene.h"


class CScene_White_01 :
    public CScene
{
private:
    int m_iMonsterCount;

public:
    virtual void update();
    virtual void Enter();


public:
    CScene_White_01();
    ~CScene_White_01();
};

