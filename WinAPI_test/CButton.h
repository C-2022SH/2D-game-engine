#pragma once
#include "CUI.h"


class CButton :
    public CUI
{
private:
    bool    m_bIsInteractive;


public:
    bool IsInteractive() { return m_bIsInteractive; }

    void SetInteractive(bool _bInteractive) { m_bIsInteractive = _bInteractive; }

    void OnButtonClick();

    virtual void update();
    virtual void render(HDC _dc);


public:
    CButton(Dir4 _dPos, bool _bInteractive, wstring _strText = L"");
    ~CButton();
};