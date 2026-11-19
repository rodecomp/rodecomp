#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
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
CTexture::CTexture() {
  this->m_format = TextureFormat::UnknownTextureFormat;
  this->fullSize = 1;
  this->transparent = 0;
  this->m_width = 0;
  this->m_textureWidth = 0;
  this->m_height = 0;
  this->m_textureHeight = 0;
  this->refCount = 0;
  this->lastUsedTick = 0;
  this->m_name[0] = '\0';
}

// OFFSET: 0x00572370
// FUNCTION: fn_02464
CSurface::~CSurface() = default;

// OFFSET: 0x00572380
// FUNCTION: fn_02465
CTexture::~CTexture() = default;

// OFFSET: 0x00572460
// FUNCTION: fn_02469
std::uint32_t CTexture::Retain() {
  this->lastUsedTick = timeGetTime();
  return ++this->refCount;
}

// OFFSET: 0x005724F0
// FUNCTION: fn_02473
std::uint32_t CTexture::Touch() {
  this->lastUsedTick = timeGetTime();
  return this->lastUsedTick;
}

// OFFSET: 0x005724E0
// FUNCTION: fn_02472
std::uint32_t CTexture::Release() {
  return --this->refCount;
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
  struct QuadVertex : RenderVertex {
    QuadVertex(float px, float py, float pz, float prhw, std::uint32_t pdiffuse, std::uint32_t pspecular, float pu, float pv) {
      this->x = px;
      this->y = py;
      this->z = pz;
      this->rhw = prhw;
      this->diffuse = pdiffuse;
      this->specular = pspecular;
      this->u = pu;
      this->v = pv;
    }
  };

  // INLINED
  __forceinline void
  SetQuad(RenderPrimitive* primitive, CTexture* texture, int x, int y, int right, int bottom, std::uint32_t color, float u0, float v0, float u1, float v1) {
    constexpr float z = 0.00001f;
    constexpr float rhw = 0.99999f;

    primitive->texture = texture;
    primitive->quad[0] = QuadVertex(static_cast<float>(x), static_cast<float>(y), z, rhw, color, 0xFFFFFFFF, u0, v0);
    primitive->quad[1] = QuadVertex(static_cast<float>(right), static_cast<float>(y), z, rhw, color, 0xFFFFFFFF, u1, v0);
    primitive->quad[2] = QuadVertex(static_cast<float>(x), static_cast<float>(bottom), z, rhw, color, 0xFFFFFFFF, u0, v1);
    primitive->quad[3] = QuadVertex(static_cast<float>(right), static_cast<float>(bottom), z, rhw, color, 0xFFFFFFFF, u1, v1);
  }
}

// OFFSET: 0x00544410
// FUNCTION: fn_01817
void CTexture::Draw(int x, int y, int width, int height, std::uint32_t color) {
  const float u0 = g_textureOffsetX / static_cast<float>(this->m_textureWidth);
  const float v0 = g_textureOffsetY / static_cast<float>(this->m_textureHeight);
  const float u1 = (static_cast<float>(width) + g_textureOffsetX) / static_cast<float>(this->m_textureWidth);
  const float v1 = (static_cast<float>(height) + g_textureOffsetY) / static_cast<float>(this->m_textureHeight);
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  SetQuad(primitive, this, x, y, x + width, y + height, color, u0, v0, u1, v1);
  g_renderer->Submit(primitive, 513);
}

// OFFSET: 0x00544610
// FUNCTION: fn_01818
void CTexture::Stretch(int x, int y, int width, int height) {
  const float u0 = g_textureOffsetX / static_cast<float>(this->m_textureWidth);
  const float v0 = g_textureOffsetY / static_cast<float>(this->m_textureHeight);
  const float u1 = (static_cast<float>(this->m_width) + g_textureOffsetX) / static_cast<float>(this->m_textureWidth);
  const float v1 = (static_cast<float>(this->m_height) + g_textureOffsetY) / static_cast<float>(this->m_textureHeight);
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  SetQuad(primitive, this, x, y, x + width, y + height, 0xFFFFFFFF, u0, v0, u1, v1);
  g_renderer->Submit(primitive, 513);
}

// OFFSET: 0x00544840
// FUNCTION: fn_01819
void CTexture::StretchTintedTo(int x, int y, int width, int height, std::uint32_t color) {
  const int right = x + width;
  const int bottom = y + height;
  const float u0 = g_textureOffsetX / static_cast<float>(this->m_textureWidth);
  const float v0 = g_textureOffsetY / static_cast<float>(this->m_textureHeight);
  const float u1 = (static_cast<float>(this->m_width) + g_textureOffsetX) / static_cast<float>(this->m_textureWidth);
  const float v1 = (static_cast<float>(this->m_height) + g_textureOffsetY) / static_cast<float>(this->m_textureHeight);
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  SetQuad(primitive, this, x, y, right, bottom, color, u0, v0, u1, v1);
  primitive->quad[3].diffuse = 0xFFFFFFFF;
  g_renderer->Submit(primitive, 513);
}
