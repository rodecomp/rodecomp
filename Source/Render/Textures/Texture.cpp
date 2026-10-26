#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/Texture.h"

#include <bit>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// STATIC: 0x01312DE0
// GLOBAL: gl_21914
float g_textureOffsetX = 0.5f;
// STATIC: 0x01312DE4
// GLOBAL: gl_21915
float g_textureOffsetY = 0.5f;

// OFFSET: 0x005722E0
// FUNCTION: fn_02463
CTexture::CTexture()
    : m_format(TextureFormat::UnknownTextureFormat), unknown_0008(1), m_textureWidth(0), m_textureHeight(0), m_width(0), m_height(0), unknown_001C(0),
      unknown_0020(0) {
  this->m_name[0] = '\0';
}

// OFFSET: 0x00572380
// OFFSET: 0x005723D0
// FUNCTION: fn_02465
CTexture::~CTexture() = default;

// OFFSET: 0x00572460
// FUNCTION: fn_02469
std::uint32_t CTexture::Retain() {
  this->unknown_0020 = timeGetTime();
  return ++this->unknown_001C;
}

// OFFSET: 0x005724E0
// FUNCTION: fn_02472
std::uint32_t CTexture::Release() {
  return --this->unknown_001C;
}

// OFFSET: 0x00572450
// FUNCTION: fn_02468
std::uint32_t CTexture::Width() {
  return this->m_width;
}

// OFFSET: 0x00572440
// FUNCTION: fn_02467
std::uint32_t CTexture::Height() {
  return this->m_height;
}

// OFFSET: 0x005724A0
// FUNCTION: fn_02471
std::uint32_t CTexture::SetImageSize(std::uint32_t width, std::uint32_t height) {
  while (width > this->m_textureWidth) {
    width >>= 1;
  }

  while (height > this->m_textureHeight) {
    height >>= 1;
  }

  this->m_height = height;
  this->m_width = width;
  return width;
}

// OFFSET: 0x00572480
// FUNCTION: fn_02470
int CTexture::SetName(const char* name) {
  return strncpy_s(this->m_name, sizeof(this->m_name), name, _TRUNCATE);
}

namespace {
  // INLINED
  void
  SetQuad(RenderPrimitive* primitive, CTexture* texture, int x, int y, int width, int height, std::uint32_t color, float u0, float v0, float u1, float v1) {
    constexpr float z = 0.00001f;
    constexpr float rhw = 0.99999f;
    const float right = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(x) + width));
    const float bottom = static_cast<float>(static_cast<std::int32_t>(static_cast<std::uint32_t>(y) + height));

    primitive->texture = texture;
    primitive->quad[0] = {static_cast<float>(x), static_cast<float>(y), z, rhw, color, 0xFFFFFFFF, u0, v0};
    primitive->quad[1] = {right, static_cast<float>(y), z, rhw, color, 0xFFFFFFFF, u1, v0};
    primitive->quad[2] = {static_cast<float>(x), bottom, z, rhw, color, 0xFFFFFFFF, u0, v1};
    primitive->quad[3] = {right, bottom, z, rhw, color, 0xFFFFFFFF, u1, v1};
  }
}

// OFFSET: 0x00544410
// FUNCTION: fn_01817
void CTexture::DrawTo(
  RenderPrimitivePool& pool, RenderQueues& queues, int x, int y, int width, int height, std::uint32_t color, bool depthAlpha, float depthStart, float depthEnd
) {
  const float u0 = g_textureOffsetX / static_cast<float>(this->m_textureWidth);
  const float v0 = g_textureOffsetY / static_cast<float>(this->m_textureHeight);
  const float u1 = (static_cast<float>(width) + g_textureOffsetX) / static_cast<float>(this->m_textureWidth);
  const float v1 = (static_cast<float>(height) + g_textureOffsetY) / static_cast<float>(this->m_textureHeight);
  RenderPrimitive* primitive = pool.Acquire();
  SetQuad(primitive, this, x, y, width, height, color, u0, v0, u1, v1);
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}

// OFFSET: 0x00544610
// FUNCTION: fn_01818
void CTexture::StretchTo(
  RenderPrimitivePool& pool, RenderQueues& queues, int x, int y, int width, int height, bool depthAlpha, float depthStart, float depthEnd
) {
  const float u0 = g_textureOffsetX / static_cast<float>(this->m_textureWidth);
  const float v0 = g_textureOffsetY / static_cast<float>(this->m_textureHeight);
  const float u1 = (static_cast<float>(this->m_width) + g_textureOffsetX) / static_cast<float>(this->m_textureWidth);
  const float v1 = (static_cast<float>(this->m_height) + g_textureOffsetY) / static_cast<float>(this->m_textureHeight);
  RenderPrimitive* primitive = pool.Acquire();
  SetQuad(primitive, this, x, y, width, height, 0xFFFFFFFF, u0, v0, u1, v1);
  queues.Submit(primitive, 513, depthAlpha, depthStart, depthEnd);
}
