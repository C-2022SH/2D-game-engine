#pragma once
#include "CResource.h"


class CTexture :
    public CResource
{
private:
    HDC     m_dc;       // mem_dc 에 이미지를 올릴 DC
    HBITMAP m_hBit;     // 이미지의 크기에 맞는 비트맵 핸들
    BITMAP  m_bitInfo;  // 비트맵의 정보


public:
    void Load(const wstring& _strFilePath);

    UINT Width() { return m_bitInfo.bmWidth; }
    UINT Height() { return m_bitInfo.bmHeight; }

    HDC GetDC() { return m_dc; }


private:
    CTexture();
    virtual ~CTexture();

    friend class CResourceManager;
};