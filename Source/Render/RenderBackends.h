#ifndef RAGNAROK_SOURCE_RENDER_RENDERBACKENDS_H
#define RAGNAROK_SOURCE_RENDER_RENDERBACKENDS_H

#include <cstdint>
#include <string>
#include <vector>

// Setup-screen backend choice: a 32-bit identity followed by the display name.
// The original list is built from constants only; it never probes installed
// Direct3D runtimes or creates a device.
struct RenderBackend {
  /* 0000 */ std::int32_t identity;
  /* 0004 */ std::string name;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenderBackend) == 28);
#endif

std::vector<RenderBackend> EnumerateRenderBackends();

#endif
