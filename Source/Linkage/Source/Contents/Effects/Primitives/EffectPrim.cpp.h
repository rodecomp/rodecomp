#pragma once

#include <list>

class CGameObject;

extern template void std::list<CGameObject*>::push_back(CGameObject*&&);
