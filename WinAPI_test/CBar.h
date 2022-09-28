#pragma once
#include "CUI.h"

class CObject;

class CBar :
    public CUI
{
private:
    float m_fProportionalConst;

    CObject* pValueStandard;


public:
    virtual void update();

    virtual void render(HDC _dc);


public:
    CBar(Dir4 _dPos, CObject* _pStandard);
    ~CBar();
};

