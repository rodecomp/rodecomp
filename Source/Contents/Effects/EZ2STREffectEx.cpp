#include "Contents/Effects/EZ2STREffectEx.h"

#include "Contents/Effects/EffectMemoryMgr.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x006FF480
// FUNCTION: fn_06962
CEZ2STREffectEx::CEZ2STREffectEx() : CEffect(-1), particle(nullptr), finished(0), unknown_00D4(0), unknown_00D8(0) {
}

// OFFSET: 0x006FF740
// FUNCTION: fn_06964
void CEZ2STREffectEx::Close() {
  if (this->particle) {
    FreeParticle(this->particle);
    this->particle = nullptr;
  }
}

// OFFSET: 0x006FF770
// FUNCTION: fn_06965
void CEZ2STREffectEx::Slot31() {
  this->finished = 1;
}

// OFFSET: 0x006FF780
// FUNCTION: fn_06966
float ScreenAngle(const float* from, const float* to, const float* matrix) {
  float fromX;
  float fromY;
  float fromDepth = 0.0f;
  float toX;
  float toY;
  float toDepth = 0.0f;
  g_renderer->ProjectViewport(from, matrix, &fromX, &fromY, &fromDepth);
  g_renderer->ProjectViewport(to, matrix, &toX, &toY, &toDepth);

  const float dx = toX - fromX;
  const float dy = toY - fromY;
  const float length = SquareRoot(dy * dy + dx * dx);
  if (length == 0.0f) {
    return 0.0f;
  }

  const float angle = static_cast<float>(std::acos(static_cast<double>(dx / length))) / 3.141592f * 180.0f;
  if (fromY > toY) {
    return 360.0f - angle;
  }

  return angle;
}
