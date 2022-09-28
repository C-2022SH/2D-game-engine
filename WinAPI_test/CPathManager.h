#pragma once
class CPathManager
{
	SINGLETON(CPathManager);

private:
	wchar_t		m_szContentPath[255];	// 윈도우 최대치 글자 수로 제한

public:
	void initialize();

public:
	const wchar_t* GetContentsPath() { return m_szContentPath; }
};

