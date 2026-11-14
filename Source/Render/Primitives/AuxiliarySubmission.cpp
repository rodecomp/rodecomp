#include "Render/Primitives/AuxiliarySubmission.h"
#include "Render/Renderer.h"
#include "Render/Primitives/DepthAlpha.h"

// OFFSET: 0x00569520
// FUNCTION: fn_02262
__declspec(noinline) void LightmappedCommand::ApplyAuxiliaryDepthAlpha() {
  for (int index = 0; index < this->vertexCount; ++index) {
    this->vertices[index].specular = DepthAlpha(this->vertices[index].rhw);
  }
}

// OFFSET: 0x00559C50
// FUNCTION: fn_02072
void CRenderer::SubmitAuxiliaryPrimitive(LightmappedCommand* primitive) {
  primitive->ApplyAuxiliaryDepthAlpha();
  this->queues.queue_0240.push_back(primitive);
}

// INLINED
void SubmitAuxiliaryPrimitive(CRenderer& renderer, LightmappedCommand* primitive, const float& depthStart, const float& depthEnd, int reserved) {
  renderer.SubmitAuxiliaryPrimitive(primitive);
}
