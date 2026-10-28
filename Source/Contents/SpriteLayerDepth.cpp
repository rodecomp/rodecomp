#include "Contents/Actors/SpriteDrawingPolicy.h"
#include "Contents/Camera/Camera.h"
#include "Render/SpriteGeometry.h"

constexpr float robeTopLayerThreshold = 320.0f;

// GLOBAL: gl_24739
float g_cameraDistance = 300.0f;

// OFFSET: 0x0080DAA0
// FUNCTION: fn_10103
void SpriteDrawingPolicy::OffsetLayerDepth(int layer, SpriteCellQuad& quad, float bottomDepth, float topDepth) const {
  switch (layer) {
  case 2:
    if (g_cameraDistance < robeTopLayerThreshold) {
      quad.bottomDepth = bottomDepth + 0.000027f;
      quad.topDepth = topDepth + 0.000027f;
    } else {
      quad.bottomDepth = bottomDepth;
      quad.topDepth = topDepth;
    }
    break;
  case 3:
    if (g_cameraDistance < robeTopLayerThreshold) {
      quad.bottomDepth = bottomDepth + 0.00004f;
      quad.topDepth = 0.0001f + topDepth;
    } else {
      quad.bottomDepth = bottomDepth;
      quad.topDepth = 0.000045f + topDepth;
    }
    break;
  case 4:
    if (g_cameraDistance < robeTopLayerThreshold) {
      bottomDepth += 0.00003f;
      topDepth += 0.00003f;
    }
    quad.bottomDepth = bottomDepth;
    quad.topDepth = topDepth;
    break;
  }
}
