#pragma once

class CObject;

void CreatEventObject(CObject* _pObj);
void DeleteEventObject(CObject* _pObj);
void ChangeScene(SCENE_TYPE _eNext, int _iDelay = 0);

template<typename T>
void Delete_Vector(vector<T>& _vector)
{
	for (size_t i = 0; i < _vector.size(); i++)
	{
		if(_vector[i] != nullptr)
			delete _vector[i];
	}

	_vector.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); iter++)
	{
		if (iter->second != nullptr)
			delete iter->second;
	}

	_map.clear();
}