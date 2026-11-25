#ifndef RAGNAROK_SOURCE_RENDER_DEVICE_RENDERBACKENDS_H
#define RAGNAROK_SOURCE_RENDER_DEVICE_RENDERBACKENDS_H

#include <cstdint>
#include <string>
#include <vector>

enum RenderBackendType : std::int32_t {
  RENDER_BACKEND_NOT_INITIALIZED = -1,
  RENDER_BACKEND_DEFAULT = 0,
  RENDER_BACKEND_DX7 = 1,
  RENDER_BACKEND_DX9 = 2
};

struct RenderBackend {
  /* 0000 */ std::int32_t identity;
  /* 0004 */ std::string name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderBackend) == 28);
#endif

std::vector<RenderBackend> EnumerateRenderBackends();

#endif
