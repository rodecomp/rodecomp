#ifndef RAGNAROK_SOURCE_RENDER_DEPTHALPHA_H
#define RAGNAROK_SOURCE_RENDER_DEPTHALPHA_H

#include <cstdint>

struct RenderCommand;
struct LightmappedPrimitive;

std::uint32_t DepthAlpha(float reciprocalDepth, float start, float end);
void ApplyDepthAlpha(RenderCommand& command, float start, float end);
void ApplyDepthAlpha(LightmappedPrimitive& command, float start, float end);

#endif
