#pragma once


class CPlayer;

class CGameManager
{
	SINGLETON(CGameManager);

private:
	CPlayer* m_pPlayer;


public:
	CPlayer* GetPlayerObject() { return m_pPlayer; }
	void SetPlayerObject(CPlayer* _pPlayer) { m_pPlayer = _pPlayer; }

};