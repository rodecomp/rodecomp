#include "Render/Device/RenderBackends.h"

// OFFSET: 0x0056A2A0
// FUNCTION: fn_02289
std::vector<RenderBackend> EnumerateRenderBackends() {
  std::vector<RenderBackend> backends;
  backends.push_back({RENDER_BACKEND_DX7, "DirectX 7"});
  backends.push_back({RENDER_BACKEND_DX9, "DirectX 9"});

  return backends;
}
