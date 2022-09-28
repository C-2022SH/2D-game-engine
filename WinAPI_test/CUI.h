#pragma once

class CTexture;
class CAnimator;


class CUI
{
protected:
	UI_TYPE		m_eType;
	wstring		m_strName;
	CTexture*	m_pTexture;


private:
	Dir4		m_dPosition;
	Vector2		m_vOffset;

	CAnimator*	m_pAnimator;


public:
	Dir4 GetPosition() { return m_dPosition; }
	int Center_x() { return (m_dPosition.right + m_dPosition.left) / 2; }
	int Center_y() { return (m_dPosition.down + m_dPosition.up) / 2; }

	UI_TYPE GetType() { return m_eType; }
	Vector2 GetOffset() { return m_vOffset; }

	void SetPosition(Dir4 _vPos) { m_dPosition = _vPos; }
	void SetType(UI_TYPE _eType) { m_eType = _eType; }
	void SetOffset(Vector2 _vOffset) { m_vOffset = _vOffset; }

	virtual void update() = 0;
	virtual void render(HDC _dc);


private:
	void component_render(HDC _dc);


public:
	CUI(UI_TYPE _eType);
	virtual ~CUI();
};
