#include "Render/TextureQuad.h"

// OFFSET: 0x0055D310
float RhwToDepth(float rhw) {
  return ((1.0f / rhw - 10.0f) * 1.0067114f) * rhw;
}
