#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Replay.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/SpriteQuad.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

namespace {
  // STATIC: 0x0135EF80
  // GLOBAL: gl_21923
  std::uint64_t nextIdentity = 0;
}

// OFFSET: 0x00CA8600
// FUNCTION: fn_20989
CGameObject::CGameObject() : identity(++nextIdentity) {
}

// OFFSET: 0x00CA8EC0
// OFFSET: 0x00CA9230
// FUNCTION: fn_20995
CGameObject::~CGameObject() = default;

// OFFSET: 0x00CAFD20
// FUNCTION: fn_21087
bool CGameObject::Update() {
  return true;
}

// OFFSET: 0x00CBF850
// FUNCTION: fn_21113
void CGameObject::SendObjectMessage(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CB20A0
// FUNCTION: fn_21101
void CGameObject::Draw(int) {
}

// OFFSET: 0x00CAB920
// FUNCTION: fn_21031
int CGameObject::Direction(float angle, const CameraDirectionContext& camera) {
  if (camera.modeType != 1) {
    return 0;
  }

  const float negative = -(*camera.cameraAngle);
  float relative = negative - angle;

  while (relative < 0) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  return TruncateToInt(relative / 45.0f);
}

// OFFSET: 0x00CB20B0
// FUNCTION: fn_21102
void CRenderObject::Draw(int argument) {
  if (this->render.visible != 0) {
    this->DrawShadow(argument, this->render.scale);
    this->DrawSprite(argument, false, 0.0f);
  }
}

// OFFSET: 0x00CB20F0

// OFFSET: 0x00CB2830

// OFFSET: 0x00CB35F0

// OFFSET: 0x00CB39A0
// FUNCTION: fn_21106
void CRenderObject::DrawQuad(const SpriteQuad& quad, const std::uint32_t* palette, int blend, float offsetX, float offsetY, int depthOffset) {
  SpriteAtlasRecord record{};
  const SpriteRleImage* image = quad.image;
  CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
  if (!texture) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    record.u0 = 0.0f;
    record.v0 = 0.0f;
    record.u1 = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
    record.v1 = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
  }

  const double depthBias = depthOffset * 0.000001;
  const double rhwBias = depthOffset * 0.0002;
  const float bottomRhw = static_cast<float>(rhwBias + quad.bottomRhw);
  const float topRhw = static_cast<float>(quad.topRhw + rhwBias);
  const float bottomZ = RhwToDepth(static_cast<float>(depthBias + quad.bottomRhw));
  const float topZ = RhwToDepth(static_cast<float>(quad.topRhw + depthBias));

  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  primitive->texture = texture;

  if (quad.angle != 0.0f) {
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

  if (*palette == 0xD77B) {
    record.u1 -= 0.002f;
  }

  vertices[0].rhw = topRhw;
  vertices[0].z = topZ;
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[0].diffuse = quad.color;
  vertices[1].rhw = topRhw;
  vertices[1].z = topZ;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[1].diffuse = quad.color;
  vertices[2].rhw = bottomRhw;
  vertices[2].z = bottomZ;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[2].diffuse = quad.color;
  vertices[3].rhw = bottomRhw;
  vertices[3].z = bottomZ;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;
  vertices[3].diffuse = quad.color;

  primitive->unknown_0018 = 5;
  primitive->unknown_001C = blend != 5 ? 6 : 2;
  primitive->unknown_0014 = g_renderer->twoPass != 1;
  g_renderer->queues.Submit(primitive, static_cast<std::uint16_t>(blend | this->render.unknown_0124), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}

// OFFSET: 0x00CB3F40
// FUNCTION: fn_21107
void CRenderObject::DrawFlippedQuad(SpriteQuad& quad, const std::uint32_t* palette, int blend, float x, float y, int depthOffset) {
  quad.angle = 180.0f;
  this->DrawQuad(quad, palette, blend, (x - quad.right) + (x - quad.left), (y - quad.bottom) + (y - quad.top), depthOffset);
}


// OFFSET: 0x00CB3FA0
// FUNCTION: fn_21108
void CRenderObject::SetActionSpeed(float speed) {
  speed *= this->render.action->Speed(this->render.animation);

  if (g_replayAnimationScaling != 0) {
    speed *= Replay::Instance().AnimationScale();
  }

  if (speed > 0.0f) {
    this->render.speed = speed;
  }
}

// OFFSET: 0x00CB3FF0
// FUNCTION: fn_21109
void CRenderObject::SendValueMessage(int value, CGameObject* target) {
  if (target) {
    const std::int64_t wide = value;
    target->SendObjectMessage(
      nullptr, 29, 0, static_cast<int>(wide), static_cast<int>(wide >> 32), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    );
  }
}

// OFFSET: 0x00CB4070
// FUNCTION: fn_21110
void CGameActor::ApplyPendingUnknown0408() {
  const int pending = this->unknown_0408;
  this->unknown_0404 = pending;
  this->unknown_0408 = 0;
  this->render.isLieOnGround = false;
  this->render.unknown_00A4Bytes[1] = 1;
  this->render.isMotionFreezed = false;
  this->render.unknown_0114 = 0;

  int value;
  if (pending == 1) {
    value = 0x1104;
  } else if (pending == 2) {
    value = 0x1105;
  } else {
    value = this->unknown_025C;
  }
  this->Slot21(value);
  this->Slot32();

  if (this->render.unknown_0070 != 3) {
    this->render.unknown_0070 = 0;
    this->Slot15(0, 1);
  }
}

// OFFSET: 0x00CB4100

// OFFSET: 0x00CB4800
