#pragma once

#include "Render/Primitives/RenderPrimitive.h"

#include <list>

extern template void std::list<RenderPrimitive*>::push_back(RenderPrimitive* const&);
