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
// FUNCTION: fn_18831
CParticle::~CParticle() {
}

// OFFSET: 0x00B3FC10
// FUNCTION: fn_18836
void CParticle::SetQuad(RenderVertex* vertices, float top, float bottom, float left, float right) {
  const std::uint32_t color = ((this->alpha << 8 | this->red) << 8 | this->green) << 8 | this->blue;
  vertices[0].diffuse = color;
  vertices[1].diffuse = color;
  vertices[2].diffuse = color;
  vertices[3].diffuse = color;

  vertices[0].v = top;
  vertices[1].v = top;
  vertices[0].u = left;
  vertices[1].u = right;
  vertices[2].u = left;
  vertices[2].v = bottom;
  vertices[3].u = right;
  vertices[3].v = bottom;
}

// OFFSET: 0x00B3FC80
// FUNCTION: fn_18837
void InitParticleQuad(RenderVertex* vertices) {
  vertices[0].x = -1.0f;
  vertices[0].y = 1.0f;
  vertices[0].z = 1.0f;
  vertices[1].x = 1.0f;
  vertices[1].y = 1.0f;
  vertices[1].z = 1.0f;
  vertices[2].x = -1.0f;
  vertices[2].y = -1.0f;
  vertices[2].z = 1.0f;
  vertices[3].x = 1.0f;
  vertices[3].y = -1.0f;
  vertices[3].z = 1.0f;
}

// OFFSET: 0x00B40120
// FUNCTION: fn_18842
void CParticle::SetColor(short redValue, short greenValue, short blueValue) {
  if (redValue != -1) {
    this->red = redValue;
  }
  if (greenValue != -1) {
    this->green = greenValue;
  }
  if (blueValue != -1) {
    this->blue = blueValue;
  }
}

// OFFSET: 0x00B40160
// FUNCTION: fn_18843
void CParticle::SetTexture(const char* name) {
  this->texture = g_texMgr.Load(name, 0, false, true, false);
}

// OFFSET: 0x00B40190
// FUNCTION: fn_18844
void CParticle::SetTexture(std::string name) {
  this->texture = g_texMgr.Load(name.c_str(), 0, false, true, false);
}
