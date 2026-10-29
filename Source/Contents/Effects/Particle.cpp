#include "Contents/Effects/Particle.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Textures/TextureCache.h"

#include <cstdlib>

// OFFSET: 0x00B3C440
// FUNCTION: fn_18809
float RandomRange(float minimum, float maximum) {
  if (minimum == maximum) {
    return minimum;
  }

  const int low = static_cast<int>(minimum * 10.0f);
  const int value = rand();
  const int span = std::abs(low + static_cast<int>(maximum * -10.0f)) + 1;
  return static_cast<float>(value % span + low) * 0.1f;
}

// OFFSET: 0x00B3F950

// OFFSET: 0x00B3FC10

// OFFSET: 0x00B3FC80

// OFFSET: 0x00B40120

// OFFSET: 0x00B40160

// OFFSET: 0x00B40190
