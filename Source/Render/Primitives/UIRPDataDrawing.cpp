#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Render/Renderer.h"
#include "Render/Primitives/UIRPData.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

// INLINED
static void SetTextureRegion(RenderPrimitive* primitive, float left, float top, float right, float bottom, float u0, float v0, float u1, float v1,
                            std::uint32_t color) {
  primitive->quad[0].x = left;
  primitive->quad[0].y = top;
  primitive->quad[0].u = u0;
  primitive->quad[0].v = v0;
  primitive->quad[1].x = right;
  primitive->quad[1].y = top;
  primitive->quad[1].u = u1;
  primitive->quad[1].v = v0;
  primitive->quad[2].x = left;
  primitive->quad[2].y = bottom;
  primitive->quad[2].u = u0;
  primitive->quad[2].v = v1;
  primitive->quad[3].x = right;
  primitive->quad[3].y = bottom;
  primitive->quad[3].u = u1;
  primitive->quad[3].v = v1;
  for (RenderVertex& vertex : primitive->quad) {
    vertex.z = 0.000001f;
    vertex.rhw = 0.999999f;
    vertex.diffuse = color;
  }
}

// OFFSET: 0x00812ED0
// FUNCTION: fn_45595
void UIRPData::DrawCroppedTexture(int x, int y, CTexture* texture, int sourceLeft, int sourceTop, int sourceRight, int sourceBottom, std::uint32_t color) {
  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  if (!texture) {
    primitive->texture = g_texMgr.Default();
  } else {
    primitive->texture = texture;
  }
  texture->Width();
  texture->Height();

  const float textureWidth = static_cast<float>(texture->TextureWidth());
  const float u0 = (static_cast<float>(sourceLeft) + g_textureOffsetX) / textureWidth;
  const float u1 = (static_cast<float>(sourceRight) + g_textureOffsetX) / textureWidth;
  const float textureHeight = static_cast<float>(texture->TextureHeight());
  const float v0 = (static_cast<float>(sourceTop) + g_textureOffsetY) / textureHeight;
  const float v1 = (static_cast<float>(sourceBottom) + g_textureOffsetY) / textureHeight;
  const int right = sourceRight - sourceLeft + x;
  const int bottom = sourceBottom - sourceTop + y;
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].u = u0;
  primitive->quad[0].v = v0;
  primitive->quad[1].x = static_cast<float>(right);
  primitive->quad[1].y = static_cast<float>(y);
  primitive->quad[1].u = u1;
  primitive->quad[1].v = v0;
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(bottom);
  primitive->quad[2].u = u0;
  primitive->quad[2].v = v1;
  primitive->quad[3].x = static_cast<float>(right);
  primitive->quad[3].y = static_cast<float>(bottom);
  primitive->quad[3].u = u1;
  primitive->quad[3].v = v1;
  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
    primitive->quad[i].diffuse = color;
  }

  this->m_primitives.push_back(primitive);
}

// OFFSET: 0x008130A0
// FUNCTION: fn_10121
void UIRPData::DrawStretched(int x, int y, CTexture* texture, std::uint32_t color, int width, int height) {
  if (!texture) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->texture = texture;
  const int right = x + width;
  const int bottom = y + height;
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
  primitive->quad[0].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
  primitive->quad[1].x = static_cast<float>(right);
  primitive->quad[1].y = static_cast<float>(y);
  primitive->quad[1].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
  primitive->quad[1].v = g_textureOffsetY / static_cast<float>(texture->TextureHeight());
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(bottom);
  primitive->quad[2].u = g_textureOffsetX / static_cast<float>(texture->TextureWidth());
  primitive->quad[2].v = (static_cast<float>(texture->Height()) + g_textureOffsetX) / static_cast<float>(texture->TextureHeight());
  primitive->quad[3].x = static_cast<float>(right);
  primitive->quad[3].y = static_cast<float>(bottom);
  primitive->quad[3].u = (static_cast<float>(texture->Width()) + g_textureOffsetX) / static_cast<float>(texture->TextureWidth());
  primitive->quad[3].v = (static_cast<float>(texture->Height()) + g_textureOffsetX) / static_cast<float>(texture->TextureHeight());
  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].diffuse = color;
  }

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
  }

  this->m_primitives.push_back(primitive);
}

// OFFSET: 0x008134B0
// FUNCTION: fn_10122
void UIRPData::DrawNamed(int x, int y, std::string name, std::uint32_t color, int width, int height) {
  CTexture* texture = g_texMgr.Load(name.c_str(), 0, true, true, false);
  if (!texture) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->texture = texture;
  const int imageWidth = texture->Width();
  const int imageHeight = texture->Height();
  if (!width) {
    width = texture->Width();
  }

  if (!height) {
    height = texture->Height();
  }

  const int textureWidth = texture->TextureWidth();
  const int textureHeight = texture->TextureHeight();
  primitive->quad[0].x = static_cast<float>(x);
  primitive->quad[0].y = static_cast<float>(y);
  primitive->quad[0].u = g_textureOffsetX / static_cast<float>(textureWidth);
  primitive->quad[0].v = g_textureOffsetY / static_cast<float>(textureHeight);
  primitive->quad[1].x = static_cast<float>(x + width);
  primitive->quad[1].y = static_cast<float>(y);
  if (width > imageWidth) {
    primitive->quad[1].u = (static_cast<float>(imageWidth) + g_textureOffsetX) / static_cast<float>(textureWidth);
  } else {
    primitive->quad[1].u = (static_cast<float>(width) + g_textureOffsetX) / static_cast<float>(textureWidth);
  }

  primitive->quad[1].v = g_textureOffsetY / static_cast<float>(textureHeight);
  primitive->quad[2].x = static_cast<float>(x);
  primitive->quad[2].y = static_cast<float>(y + height);
  primitive->quad[2].u = g_textureOffsetX / static_cast<float>(textureWidth);
  if (height > imageHeight) {
    primitive->quad[2].v = (static_cast<float>(imageHeight) + g_textureOffsetY) / static_cast<float>(textureHeight);
  } else {
    primitive->quad[2].v = (static_cast<float>(height) + g_textureOffsetX) / static_cast<float>(textureHeight);
  }

  primitive->quad[3].x = static_cast<float>(x + width);
  primitive->quad[3].y = static_cast<float>(y + height);
  if (width > imageWidth) {
    primitive->quad[3].u = (static_cast<float>(imageWidth) + g_textureOffsetX) / static_cast<float>(textureWidth);
  } else {
    primitive->quad[3].u = (static_cast<float>(width) + g_textureOffsetX) / static_cast<float>(textureWidth);
  }

  primitive->quad[3].v = height > imageHeight ? (static_cast<float>(imageHeight) + g_textureOffsetY) / static_cast<float>(textureHeight) : (static_cast<float>(height) + g_textureOffsetX) / static_cast<float>(textureHeight);

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].diffuse = color;
  }

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
  }

  this->m_primitives.push_back(primitive);
}

// OFFSET: 0x00813740
// FUNCTION: fn_45596
void UIRPData::DrawNamedScaledRegion(float x, float y, std::string name, std::uint32_t color, int width, int height, float scalePercent, int sourceX,
                                    int sourceY) {
  CTexture* texture = g_texMgr.Load(name.c_str(), 0, true, true, false);
  if (!texture) {
    return;
  }

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  primitive->texture = texture;
  const int imageWidth = texture->Width();
  const int imageHeight = texture->Height();
  if (!width) {
    width = texture->Width();
  }
  if (!height) {
    height = texture->Height();
  }

  const unsigned backingWidth = texture->TextureWidth();
  const float scale = g_fireflyArcLength / scalePercent;
  const float textureWidth = static_cast<float>(backingWidth);
  const float textureHeight = static_cast<float>(texture->TextureHeight());

  primitive->quad[0].x = x;
  primitive->quad[0].y = y;
  primitive->quad[0].u = static_cast<float>(sourceX) / textureWidth;
  primitive->quad[0].v = static_cast<float>(sourceY) / textureHeight;
  primitive->quad[1].x = static_cast<float>(width) + x;
  primitive->quad[1].y = y;
  primitive->quad[1].u = width > imageWidth ? static_cast<float>(sourceX + imageWidth) / textureWidth : static_cast<float>(sourceX + width) / textureWidth;
  primitive->quad[1].v = static_cast<float>(sourceY) / textureHeight;
  primitive->quad[2].x = x;
  primitive->quad[2].y = static_cast<float>(height) + y;
  primitive->quad[2].u = static_cast<float>(sourceX) / textureWidth;
  primitive->quad[2].v = height > imageHeight ? static_cast<float>(sourceY + imageHeight) / textureHeight : static_cast<float>(sourceY + height) / textureHeight;

  primitive->quad[3].x = static_cast<float>(width) + x;
  primitive->quad[3].y = static_cast<float>(height) + y;
  primitive->quad[3].u = width > imageWidth ? static_cast<float>(sourceX + imageWidth) / textureWidth : static_cast<float>(sourceX + width) / textureWidth;

  primitive->quad[3].v = height > imageHeight ? static_cast<float>(sourceY + imageHeight) / textureHeight : static_cast<float>(sourceY + height) / textureHeight;

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].diffuse = color;
  }

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
  }

  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].u *= scale;
    primitive->quad[i].v *= scale;
  }

  this->m_primitives.push_back(primitive);
}

// OFFSET: 0x00813A00
// FUNCTION: fn_10123
void UIRPData::SetSolid(int width, int height, std::uint32_t color) {
  this->m_primitives.clear();

  RenderPrimitive* primitive = g_renderer->AcquirePrimitive();
  const float right = static_cast<float>(width);
  primitive->texture = nullptr;
  primitive->quad[0].x = 0.0f;
  primitive->quad[0].y = 0.0f;
  primitive->quad[1].y = 0.0f;
  primitive->quad[2].x = 0.0f;
  primitive->quad[1].x = right;
  primitive->quad[3].x = right;
  primitive->quad[2].y = static_cast<float>(height);
  primitive->quad[3].y = static_cast<float>(height);
  for (int i = 0; i < 4; ++i) {
    primitive->quad[i].u = 0.0f;
    primitive->quad[i].v = 0.0f;
    primitive->quad[i].z = 0.000001f;
    primitive->quad[i].rhw = 0.999999f;
    primitive->quad[i].diffuse = color;
  }

  this->m_primitives.push_back(primitive);
}
