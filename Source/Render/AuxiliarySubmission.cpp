#include "Render/AuxiliarySubmission.h"
#include "Render/Renderer.h"
#include "Render/Primitives/DepthAlpha.h"

// OFFSET: 0x00569520
// FUNCTION: fn_02262
static void ApplyAuxiliaryDepthAlpha(LightmappedPrimitive& primitive, const float& depthStart, const float& depthEnd) {
  for (int index = 0; index < primitive.vertexCount; ++index) {
    primitive.vertices[index].specular = DepthAlpha(primitive.vertices[index].rhw, depthStart, depthEnd);
  }
}

// OFFSET: 0x00559C50
// FUNCTION: fn_02072
void SubmitAuxiliaryPrimitive(CRenderer& renderer, LightmappedPrimitive* primitive, const float& depthStart, const float& depthEnd) {
  ApplyAuxiliaryDepthAlpha(*primitive, depthStart, depthEnd);
  renderer.queues.queue_0240.push_back(primitive);
}
