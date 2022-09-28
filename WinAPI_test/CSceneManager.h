#pragma once

class CScene;

class CSceneManager
{
	SINGLETON(CSceneManager);

private:
	CScene*	m_arrScene[(UINT)SCENE_TYPE::END];	// ¸ðµç ¾À ¸ñ·Ï
	CScene* m_pCurScene;						// ÇöÀç ¾À


public:
	void initialize();
	void update();
	void render(HDC _dc);


public:
	CScene* GetCurrentScene() { return m_pCurScene; };


private:
	void ChangeScene(SCENE_TYPE _eNext, int _iDelay);
	friend class CEventManager;
};

