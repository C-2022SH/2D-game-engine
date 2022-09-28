#pragma once
#include "CObject.h"

enum class GROUND_TYPE
{
    WALL_SOLID,
    WALL_TRANS,
    SMALL,
    MEDIUM,
    
    END
};


class CGround :
    public CObject
{
private:
    Vector2     m_arrScale[(UINT)GROUND_TYPE::END];
    Vector2     m_arrLeftTop[(UINT)GROUND_TYPE::END];

    GROUND_TYPE m_eGroundType;


public:
    CLONE(CGround);

    virtual void update();
    virtual void render(HDC _dc);

private:
    void InitScale()
    {
        m_arrScale[(UINT)GROUND_TYPE::SMALL] = Vector2(38.f, 14.f);
        m_arrScale[(UINT)GROUND_TYPE::MEDIUM] = Vector2(80.f, 14.f);
    }

    void InitLeftTop()
    {
        m_arrLeftTop[(UINT)GROUND_TYPE::SMALL] = Vector2(0.f, 0.f);
        m_arrLeftTop[(UINT)GROUND_TYPE::MEDIUM] = Vector2(38.f, 0.f);
    }

public:
    // 벽 - 기본 불투명
    CGround(Vector2 _vPos, Vector2 _vScale, GROUND_TYPE _eType = GROUND_TYPE::WALL_SOLID);
    CGround(Vector2 _vPos, GROUND_TYPE _eType, bool _bIsStatic, wstring _strName = L"ground", bool _bIsTrigger = false);
    ~CGround();
};

