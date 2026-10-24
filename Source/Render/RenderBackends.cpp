#include "Render/RenderBackends.h"

// The original fills a by-value vector with two constant records, inserting the
// identity before the name and moving each temporary string into place. It does
// not consult the operating system, DirectDraw/Direct3D availability or any
// adapter, so both entries are always present.
// OFFSET: 0x0056A2A0
// FUNCTION: fn_02289
std::vector<RenderBackend> EnumerateRenderBackends() {
  std::vector<RenderBackend> backends;
  backends.push_back({1, "DirectX 7"});
  backends.push_back({2, "DirectX 9"});
  return backends;
}
