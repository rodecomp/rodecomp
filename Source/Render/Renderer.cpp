#include "Render/Renderer.h"

// OFFSET: 0x00560A40
// FUNCTION: fn_02161
CRenderer::CRenderer(CRenderSystemBase& system)
    : frame(0), unknown_003C(0), unknown_0040(0), unknown_0044(0), twoPass(0), unknown_004C(0) {
  this->atlas.Initialize(system, 50);
  this->fog.enabled = 0;
  this->fog.dirty = 1;
  this->fog.supported = 0;
}

// OFFSET: 0x00562090
// FUNCTION: fn_02175
// OFFSET: 0x00562260
// FUNCTION: fn_02176
CRenderer::~CRenderer() = default;
