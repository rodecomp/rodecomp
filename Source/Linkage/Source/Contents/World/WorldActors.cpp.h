#pragma once

#include <list>

class CGameObject;

template __declspec(noinline) void std::list<CGameObject*>::push_back(CGameObject*&&);
