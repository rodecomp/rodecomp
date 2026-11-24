#pragma once

#include "Render/Primitives/RenderVertexPool.h"
#include <vector>

template __declspec(noinline) std::vector<RenderVertexPool::Vertex>::~vector();
