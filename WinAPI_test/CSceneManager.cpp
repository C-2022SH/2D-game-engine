#include "pch.h"
#include "CSceneManager.h"

#include "CScene.h"

#include "CScene_Start.h"
#include "CScene_Tool.h"
#include "CScene_White_01.h"
#include "CScene_Violet_01.h"


CSceneManager::CSceneManager() :
	m_arrScene{},
	m_pCurScene(nullptr)
{
}

CSceneManager::~CSceneManager()
{
	// æ¿ ªË¡¶
	for (UINT i = 0; i < (UINT)SCENE_TYPE::END; i++)
	{
		if (m_arrScene[i] != nullptr)
			delete m_arrScene[i];
	}
}

void CSceneManager::initialize()
{
	// æ¿ ª˝º∫
	m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Start;
	m_arrScene[(UINT)SCENE_TYPE::START]->SetName(L"Start");

	m_arrScene[(UINT)SCENE_TYPE::WHITE_01] = new CScene_White_01;
	m_arrScene[(UINT)SCENE_TYPE::WHITE_01]->SetName(L"White_01");

	m_arrScene[(UINT)SCENE_TYPE::VIOLET_01] = new CScene_Violet_01;
	m_arrScene[(UINT)SCENE_TYPE::VIOLET_01]->SetName(L"Violet_01");

	//m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Stage01;
	//m_arrScene[(UINT)SCENE_TYPE::START] = new CScene_Stage02;

	// «ˆ¿Á æ¿ ¡ˆ¡§
	m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::VIOLET_01];
	m_pCurScene->Enter();
}

void CSceneManager::update()
{
	m_pCurScene->update();
	m_pCurScene->finalUpdate();
}

void CSceneManager::render(HDC _dc)
{
	m_pCurScene->render(_dc);
}


void CSceneManager::ChangeScene(SCENE_TYPE _eNext, int _iDelay)
{
	Sleep(_iDelay);

	m_pCurScene->Exit();
	
	m_pCurScene = m_arrScene[(UINT)_eNext];

	m_pCurScene->Enter();
}
