#include "pch.h"
#include "func.h"

#include "CEventManager.h"

#include "CObject.h"
#include "CUI.h"


void CreatEventObject(CObject* _pObj)
{
	tEvent event = {};

	event.eEvent = EVENT_TYPE::CREATE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;

	CEventManager::GetInstance()->AddEvent(event);
}

void DeleteEventObject(CObject* _pObj)
{
	tEvent event = {};

	event.eEvent = EVENT_TYPE::DELETE_OBJECT;
	event.lParam = (DWORD_PTR)_pObj;

	CEventManager::GetInstance()->AddEvent(event);
}

void ChangeScene(SCENE_TYPE _eNext, int _iDelay)
{
	tEvent event = {};

	event.eEvent = EVENT_TYPE::CHANGE_SCENE;
	event.lParam = (DWORD_PTR)_eNext;
	event.pParam = (DWORD_PTR)_iDelay;

	CEventManager::GetInstance()->AddEvent(event);
}