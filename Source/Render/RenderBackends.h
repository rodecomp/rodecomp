#ifndef RAGNAROK_SOURCE_RENDER_RENDERBACKENDS_H
#define RAGNAROK_SOURCE_RENDER_RENDERBACKENDS_H

#include <cstdint>
#include <string>
#include <vector>

struct RenderBackend {
  /* 0000 */ std::int32_t identity;
  /* 0004 */ std::string name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderBackend) == 28);
#endif

std::vector<RenderBackend> EnumerateRenderBackends();

#endif
