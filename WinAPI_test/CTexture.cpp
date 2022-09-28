#include "pch.h"
#include "CTexture.h"

#include "CCore.h"


CTexture::CTexture() :
	m_dc(0),
	m_hBit(0),
	m_bitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_dc);
	DeleteObject(m_hBit);
}

void CTexture::Load(const wstring& _strFilePath)
{
	// 데이터 --> 비트맵
	m_hBit = (HBITMAP)LoadImage(nullptr, _strFilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	assert(m_hBit);

	// DC
	m_dc = CreateCompatibleDC(CCore::GetInstance()->GetMainDC());

	// 비트맵과 DC 연결
	HBITMAP hPrevBit = (HBITMAP)SelectObject(m_dc, m_hBit);
	DeleteObject(hPrevBit);

	// 비트맵의 정보
	// BITMAP 구조체의 주소를 넣어서 그쪽으로 정보를 받아온다.
	GetObject(m_hBit, sizeof(BITMAP), &m_bitInfo);
}