#pragma once

#include <list>

class CRenderObject;

extern template void std::list<CRenderObject*>::push_back(CRenderObject*&&);
