#pragma once

// 전방 선언
// 컴파일 속도에 영향을 주지 않기 위함 + 오브젝트 코드가 수정되면 씬 코드도 매번 수정해야 하는 불편 감소
// 순환참조 방지
class CObject;
class CUI;
class CTexture;


// 모든 개별 씬들을 관리하는 클래스
class CScene
{
private:
	// 관리할 오브젝트를 그룹 별로 저장
	// 부모포인터 타입으로 호출
	//
	// 게임 오브젝트
	vector<CObject*> m_arrObj[(UINT)OBJECT_TYPE::END];
	// UI 오브젝트
	vector<CUI*>	 m_arrUI[(UINT)UI_TYPE::END];
	wstring		   	 m_strName;		// scene 이름

	// 개별 씬의 맵
	CTexture*		m_pMapImage;	// 맵 이미지
	Vector2			m_vSceneSize;	// 맵 크기

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	const wstring& GetName() { return m_strName; }

	void SetMap(CTexture* _pMap) { m_pMapImage = _pMap; }
	CTexture* GetMap() { return m_pMapImage; }

	// 오브젝트 조작
	virtual void update();

	// 프레임 작업 마무리
	void finalUpdate();

	void render(HDC _dc);

	virtual void Enter();		// 씬 진입 시
	virtual void Exit();		// 씬 이탈 시
	

public:
	void AddObject(CObject* _pObj);
	void AddUI(CUI* _pUI);

	const vector<CObject*>& GetGroupObject(OBJECT_TYPE _eType) { return m_arrObj[(UINT)_eType]; }

	void ShowMessage(wstring _strMsg);

protected:
	void DeleteObjectsByType(OBJECT_TYPE _eType);
	void DeleteUIByType(UI_TYPE _eType);
	void DeleteAllObjects();
	void DeleteAllUI();


public:
	CScene();
	virtual ~CScene();
};

