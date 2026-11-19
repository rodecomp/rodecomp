#include "Contents/Actors/CharacterRender.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/Renderer.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "Utility/Math/AffineMatrix.h"

namespace {
struct PrimitivePoolAccess : RenderPrimitivePool {
  // INLINED
  inline RenderPrimitive* AcquireLocal() {
    if (this->m_commands.end() == this->m_current) {
      this->m_commands.emplace_back();
      this->m_current = this->m_commands.end();
      return &this->m_commands.back();
    }

    RenderPrimitive* command = &*this->m_current;
    ++this->m_current;
    new (command) RenderPrimitive;

    return command;
  }
};
}

// OFFSET: 0x00811FC0
// FUNCTION: fn_10110
RenderPrimitive* CCharacterRender::BuildSpriteQuad(const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY) const {
  SpriteAtlasRecord record{};
  const SpriteRleImage* image = quad.image;
  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
  if (!texture) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(image, palette, record);
    }
  } else {
    record.u0 = 0.0f;
    record.v0 = 0.0f;
    record.u1 = static_cast<float>(static_cast<const CTexture*>(image->surface)->ImageWidth()) /
                static_cast<float>(static_cast<const CTexture*>(image->surface)->TextureWidth());
    record.v1 = static_cast<float>(static_cast<const CTexture*>(image->surface)->ImageHeight()) /
                static_cast<float>(static_cast<const CTexture*>(image->surface)->TextureHeight());
  }

  const float bottomZ = RhwToDepth(quad.bottomRhw);
  const float topZ = RhwToDepth(quad.topRhw);
  RenderPrimitive* primitive = static_cast<PrimitivePoolAccess&>(g_renderer->primitives).AcquireLocal();
  RenderVertex* vertices = primitive->vertices;
  primitive->texture = texture;

  if (quad.angle > 0.0f) {
    const float cosine = ClientCosine(static_cast<int>(quad.angle));
    const float sine = ClientSine(static_cast<int>(quad.angle));
    const float halfWidth = (quad.right - quad.left) * 0.5f;
    const float halfHeight = (quad.bottom - quad.top) * 0.5f;
    const float centerX = quad.left + halfWidth + offsetX;
    const float centerY = quad.top + halfHeight + offsetY;
    const float minX = -halfWidth;
    const float minY = -halfHeight;

    vertices[0].x = minX * cosine - minY * sine + centerX + 0.5f;
    vertices[0].y = minY * cosine + minX * sine + centerY + 0.5f;
    vertices[1].x = halfWidth * cosine - minY * sine + centerX + 0.5f;
    vertices[1].y = minY * cosine + halfWidth * sine + centerY + 0.5f;
    vertices[2].x = minX * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[2].y = halfHeight * cosine + minX * sine + centerY + 0.5f;
    vertices[3].x = halfWidth * cosine - halfHeight * sine + centerX + 0.5f;
    vertices[3].y = halfHeight * cosine + halfWidth * sine + centerY + 0.5f;
  } else {
    vertices[0].x = quad.left + 0.5f;
    vertices[0].y = quad.top + 0.5f;
    vertices[1].x = quad.right + 0.5f;
    vertices[1].y = quad.top + 0.5f;
    vertices[2].x = quad.left + 0.5f;
    vertices[2].y = quad.bottom + 0.5f;
    vertices[3].x = quad.right + 0.5f;
    vertices[3].y = quad.bottom + 0.5f;
  }

  vertices[0].z = topZ;
  vertices[0].rhw = quad.topRhw;
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[0].diffuse = quad.color;
  vertices[1].z = topZ;
  vertices[1].rhw = quad.topRhw;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[1].diffuse = quad.color;
  vertices[2].z = bottomZ;
  vertices[2].rhw = quad.bottomRhw;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[2].diffuse = quad.color;
  vertices[3].z = bottomZ;
  vertices[3].rhw = quad.bottomRhw;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;
  vertices[3].diffuse = quad.color;

  primitive->sourceBlend = 5;
  primitive->destinationBlend = blend != 5 ? 6 : 2;

  return primitive;
}
