#include "pch.h"
#include "CScene_Start.h"

#include "CResourceManager.h"

#include "CButton.h"


CScene_Start::CScene_Start()
{
	CTexture* pTex = CResourceManager::GetInstance()->LoadTexture(L"start map", L"texture\\map_white.bmp");
	SetMap(pTex);
}

CScene_Start::~CScene_Start()
{
}


void CScene_Start::Enter()
{
	CButton* pButton = new CButton(Dir4(500, 320, 580, 400), true, L"start");
	AddUI(pButton);
}