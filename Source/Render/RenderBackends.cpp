#include "Render/RenderBackends.h"

// OFFSET: 0x0056A2A0
// FUNCTION: fn_02289
std::vector<RenderBackend> EnumerateRenderBackends() {
  std::vector<RenderBackend> backends;
  backends.push_back({1, "DirectX 7"});
  backends.push_back({2, "DirectX 9"});

  return backends;
}
