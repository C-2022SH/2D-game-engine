#pragma once
#include "CUI.h"


class CText :
    public CUI
{
private:
    wstring m_strContent;
    CUI*    m_pParentUI;       // 텍스트를 가지고 있는 유아이 오브젝트

public:
    CUI* GetParentUI() { return m_pParentUI; }

    void SetContent(wstring _strContent) { m_strContent = _strContent; }
    void SetParentUI(CUI* _pParent) { m_pParentUI = _pParent; }

    virtual void update();
    virtual void render(HDC _dc);

public:
    CText(wstring _strContent, Dir4 _dPos, CUI* _pParent = nullptr);
    ~CText();
};

