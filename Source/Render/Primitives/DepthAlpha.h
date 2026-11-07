#ifndef RAGNAROK_SOURCE_RENDER_PRIMITIVES_DEPTHALPHA_H
#define RAGNAROK_SOURCE_RENDER_PRIMITIVES_DEPTHALPHA_H

#include <cstdint>

struct RenderCommand;
struct LightmappedPrimitive;

std::uint32_t DepthAlpha(float reciprocalDepth);

#endif
