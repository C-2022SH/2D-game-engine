#include "pch.h"
#include "CPathManager.h"


CPathManager::CPathManager() :
	m_szContentPath{}
{

}

CPathManager::~CPathManager()
{

}


void CPathManager::initialize()
{
	// 속성 > 고급 > 작업 디렉터리 에서 경로를 빌드 경로와 똑같이 수정해 준다.
	GetCurrentDirectory(255, m_szContentPath);

	// 한 단계 상위 폴더로 이동하기 위하여 문자열을 중간에 잘라준다.
	int iLength = (int)wcslen(m_szContentPath);

	for (int i = iLength - 1; i >= 0; i--)
	{
		if (m_szContentPath[i] == '\\')
		{
			m_szContentPath[i] = '\0';
			break;
		}
	}

	wcscat_s(m_szContentPath, 255, L"\\bin\\contents\\");
}
