#include "Render/RenderPrimitivePool.h"
#include "Render/Texture.h"
#include "Render/UIRPData.h"

#include <algorithm>
#include <bit>

// OFFSET: 0x00812780
// FUNCTION: fn_10117
UIRPData::UIRPData() : m_texture(nullptr) {
}

// OFFSET: 0x00812800
// FUNCTION: fn_10118
// OFFSET: 0x008128E0
// FUNCTION: fn_10119
UIRPData::~UIRPData() {
  delete this->m_texture;
  this->m_texture = nullptr;
  this->m_primitives.clear();
}

// OFFSET: 0x008129D0
// FUNCTION: fn_10120
void UIRPData::Draw(
  RenderPrimitivePool& pool,
  int x,
  int y,
  CTexture* texture,
  std::uint32_t color,
  int width,
  int height,
  bool stretched
) {
  if (!texture) {
    return;
  }
  RenderPrimitive* primitive = pool.Acquire();
  primitive->texture = texture;
  const float textureWidth = static_cast<float>(texture->TextureWidth());
  const float textureHeight = static_cast<float>(texture->TextureHeight());
  const float u0 = g_textureOffsetX / textureWidth;
  const float v0 = g_textureOffsetY / textureHeight;
  float u1, v1;
  if (stretched) {
    u1 = (static_cast<float>(texture->Width()) + g_textureOffsetX) / textureWidth;
    v1 = (static_cast<float>(texture->Height()) + g_textureOffsetY) / textureHeight;
  } else {
    const int imageWidth = texture->Width();
    const int imageHeight = texture->Height();
    if (!width) {
      width = texture->Width();
    }
    if (!height) {
      height = texture->Height();
    }
    u1 = (static_cast<float>(std::min(width, imageWidth)) + g_textureOffsetX) / textureWidth;
    // Original uses X offset on the <= branch, Y offset on the > branch.
    v1 = (static_cast<float>(std::min(height, imageHeight)) +
          (height <= imageHeight ? g_textureOffsetX : g_textureOffsetY)) /
         textureHeight;
  }
  const float right = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + width));
  const float bottom = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + height));
  constexpr float z = 0.000001f;
  constexpr float rhw = 0.999999f;
  primitive->quad[0] = {static_cast<float>(x), static_cast<float>(y), z, rhw, color, 0xFF000000, u0, v0};
  primitive->quad[1] = {right, static_cast<float>(y), z, rhw, color, 0xFF000000, u1, v0};
  primitive->quad[2] = {static_cast<float>(x), bottom, z, rhw, color, 0xFF000000, u0, v1};
  primitive->quad[3] = {right, bottom, z, rhw, color, 0xFF000000, u1, v1};
  this->m_primitives.push_back(primitive);
}
