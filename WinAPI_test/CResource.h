#pragma once


class CResource
{
private:
	wstring	m_strKey;			// 리소스의 고유 아이디 문자열
	wstring m_strRelativePath;	// 리소스의 상대 경로 (게임 폴더 내에서의 경로)

public:
	wstring& GetKey() { return m_strKey; }
	wstring& GetRelativePath() { return m_strRelativePath; }

	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }

public:
	CResource();
	virtual ~CResource();
};

