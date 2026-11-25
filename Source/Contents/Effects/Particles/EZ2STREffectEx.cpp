#include "Contents/Effects/Particles/EZ2STREffectEx.h"

#include "Contents/Effects/Core/EffectMemoryMgr.h"
#include "Contents/Effects/Particles/Particle.h"
#include "Render/Renderer.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

// OFFSET: 0x006FF480
// FUNCTION: fn_06962
CEZ2STREffectEx::CEZ2STREffectEx() : CEffect(-1), emitter(nullptr), timed(0), frameStep(0), alternateRender(0) {
}

// OFFSET: 0x006FF740
// FUNCTION: fn_06964
void CEZ2STREffectEx::Close() {
  if (this->emitter) {
    FreeParticle(this->emitter);
    this->emitter = nullptr;
  }
}

// OFFSET: 0x006FF770
// FUNCTION: fn_06965
void CEZ2STREffectEx::EnableTimeLimit() {
  this->timed = 1;
}

// OFFSET: 0x006FF780
// FUNCTION: fn_06966
float ScreenAngle(const float* from, const float* to, const float* matrix) {
  float fromDepth = 0.0f;
  float toDepth = 0.0f;
  float fromScreen[2];
  float toScreen[2];
  g_renderer->ProjectViewport(from, matrix, &fromScreen[0], &fromScreen[1], &fromDepth);
  g_renderer->ProjectViewport(to, matrix, &toScreen[0], &toScreen[1], &toDepth);

  const float dx = toScreen[0] - fromScreen[0];
  const float dy = toScreen[1] - fromScreen[1];
  const float length = SquareRoot(dy * dy + dx * dx);
  if (0.0f == length) {
    return 0.0f;
  }

  const float angle = static_cast<float>(std::acos(static_cast<double>(dx / length))) / 3.141592f * 180.0f;
  return fromScreen[1] > toScreen[1] ? 360.0f - angle : angle;
}

// OFFSET: 0x006FFA00
// FUNCTION: fn_06967
unsigned char CEZ2STREffectEx::IsFinished() {
  if (!this->emitter) {
    return false;
  }

  return this->emitter->m_finishedClips >= this->emitter->m_clipLimit;
}
