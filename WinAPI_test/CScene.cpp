#include "pch.h"
#include "CScene.h"

#include "CCore.h"

#include "CCollisionManager.h"

#include "CObject.h"
#include "CPlayer.h"
#include "CTexture.h"

#include "CUI.h"
#include "CText.h"


CScene::CScene() :
	m_arrObj{},
	m_strName{},
	m_pMapImage(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			delete m_arrObj[i][j];
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			delete m_arrUI[i][j];
		}
	}
}


void CScene::update()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			if (m_arrObj[i][j]->IsAlive())
			{
				m_arrObj[i][j]->update();
				m_arrObj[i][j]->gravityUpdate();
			}
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			m_arrUI[i][j]->update();
		}
	}
}

// component update
// UI 오브젝트는 콜라이더가 없으므로 진행하지 않는다
void CScene::finalUpdate()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); j++)
		{
			// 충돌 처리는 마무리해야 하기 때문에 dead 오브젝트라도 final update 는 실행
			m_arrObj[i][j]->finalUpdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		vector<CObject*>::iterator iter = m_arrObj[i].begin();

		for (; iter != m_arrObj[i].end();)
		{
			if ((*iter)->IsAlive())
			{
				(*iter)->render(_dc);
				iter++;
			}

			else
				iter = m_arrObj[i].erase(iter);
		}
	}

	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_arrUI[i].size(); j++)
		{
			m_arrUI[i][j]->render(_dc);
		}
	}
}


void CScene::Enter()
{
	// ============
	//  바운드 계산
	// ============
	//
	// 윈도우 창 가로, 세로 / 2
	int pHalfResX = ((int)CCore::GetInstance()->GetResolution().x) / 2;
	int pHalfResY = ((int)CCore::GetInstance()->GetResolution().y) / 2;

	// 맵 가로, 세로
	int iMapWidth = m_pMapImage->Width();
	int iMapHeight = m_pMapImage->Height();

	// 카메라 센터
	int vCameraCenterX = (int)CCamera::GetInstance()->GetCenter().x;
	int vCameraCenterY = (int)CCamera::GetInstance()->GetCenter().y;

	// 상하좌우 바운드 계산
	int iLeftBound = -vCameraCenterX;
	int iUpperBound = -vCameraCenterY;
	int iRightBound = iMapWidth - vCameraCenterX;
	int iLowerBound = iMapHeight - vCameraCenterY;

	// 맵이 화면보다 작은 경우 예외처리
	// 가로가 짧은 경우
	if (pHalfResX * 2 >= iMapWidth)
	{
		iLeftBound = -pHalfResX;
		iRightBound = pHalfResX;
	}

	// 세로가 짧은 경우
	else if (pHalfResY * 2 >= iMapHeight)
	{
		iUpperBound = -pHalfResY;
		iLowerBound = pHalfResY;
	}
	

	// ============
	//  바운드 설정
	// ============
	//
	// 플레이어
	CPlayer* pPlayer = CGameManager::GetInstance()->GetPlayerObject();

	if (pPlayer)
	{
		Dir4 dPlayerBound = Dir4(iLeftBound, iUpperBound, iRightBound, iLowerBound);

		// 플레이어 특수 바운드 반영
		pPlayer->SetBound(dPlayerBound + pPlayer->GetBound());

		// 카메라
		CCamera::GetInstance()->SetBound(dPlayerBound + Dir4(pHalfResX, pHalfResY, -pHalfResX, -pHalfResY));
	}
}

void CScene::Exit()
{
	DeleteAllObjects();
	DeleteAllUI();
	CCollisionManager::GetInstance()->ReleaseCollider();
}


void CScene::AddObject(CObject* _pObj)
{
	OBJECT_TYPE eType = _pObj->GetType();
	m_arrObj[(UINT)eType].push_back(_pObj);
}

void CScene::AddUI(CUI* _pUI)
{
	UI_TYPE eType = _pUI->GetType();
	m_arrUI[(UINT)eType].push_back(_pUI);
}

void CScene::ShowMessage(wstring _strMsg)
{
	CText* pText = new CText(L"Wonderful!", Dir4(-100, -20, 100, 20));

}


void CScene::DeleteObjectsByType(OBJECT_TYPE _eType)
{
	Delete_Vector<CObject*> (m_arrObj[(UINT)_eType]);
}


void CScene::DeleteUIByType(UI_TYPE _eType)
{
	Delete_Vector<CUI*> (m_arrUI[(UINT)_eType]);
}

void CScene::DeleteAllObjects()
{
	for (UINT i = 0; i < (UINT)OBJECT_TYPE::END; i++)
	{
		DeleteObjectsByType((OBJECT_TYPE)i);
	}
}

void CScene::DeleteAllUI()
{
	for (UINT i = 0; i < (UINT)UI_TYPE::END; i++)
	{
		DeleteUIByType((UI_TYPE)i);
	}
}
