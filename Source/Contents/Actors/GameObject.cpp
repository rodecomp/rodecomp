#include "Contents/Actors/GameObject.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Camera/Camera.h"
#include "Client/Replay.h"
#include "Client/Session.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/World/World.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/SpriteQuad.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Render/Textures/SpritePixels.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

namespace {
  // STATIC: 0x0135EF80
  // GLOBAL: gl_21923
  std::uint64_t nextIdentity = 0;
}

// GLOBAL: gl_24538
float g_cotangent = 0.16f;

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
bool CGameObject::OnProcess() {
  return true;
}

// OFFSET: 0x00CBF850
// FUNCTION: fn_21113
void CGameObject::SendMsg(CGameObject*, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {
}

// OFFSET: 0x00CB20A0
// FUNCTION: fn_21101
void CGameObject::Render(int) {
}

// OFFSET: 0x00CABF10
// FUNCTION: fn_21041
int CGameObject::GetDir(float rot) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  int longitude = 0;

  if (gameMode) {
    longitude = static_cast<int>(-gameMode->camera->camera.yaw - rot);

    while (longitude < 0) {
      longitude += 360;
    }

    while (longitude >= 360) {
      longitude -= 360;
    }
  }

  return longitude;
}

// OFFSET: 0x00CAB740
// FUNCTION: fn_21029
int CGameObject::Get8Dir2(float rot, int flag) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - rot;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  if (flag != 0) {
    if (relative < 22.5f || relative > 337.5f) {
      return 0;
    }

    return static_cast<int>((static_cast<double>(relative) + 22.5) / 45.0);
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CAB7F0
// FUNCTION: fn_21030
int CGameActor::Get8Dir(float angle) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - angle;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  if ((IsPlayerJob(this->GetEffectiveJob(-1)) && (this->render.animation == 0 || this->render.animation == 8 || this->render.animation == 16)) ||
      (this->GetEffectiveJob(-1) > 45 && this->GetEffectiveJob(-1) < 1000 && this->render.animation == 0) ||
      (g_session.IsSkillIdInRange6018(this->GetEffectiveJob(-1)) &&
       (this->render.animation == 0 || this->render.animation == 8 || this->render.animation == 16))) {
    if (relative < 22.5f || relative > 337.5f) {
      return 0;
    }

    return static_cast<int>((static_cast<double>(relative) + 22.5) / 45.0);
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CAB920
// FUNCTION: fn_21031
int CGameObject::Get8Dir(float angle) {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

  if (!gameMode) {
    return 0;
  }

  float relative = -gameMode->camera->camera.yaw - angle;

  while (relative < 0.0f) {
    relative += 360.0f;
  }

  while (relative >= 360.0f) {
    relative -= 360.0f;
  }

  return static_cast<int>(relative / 45.0f);
}

// OFFSET: 0x00CB20B0
// FUNCTION: fn_21102
void CRenderObject::Render(int argument) {
  if (this->render.visible != 0) {
    this->DrawShadow(argument, this->render.scale);
    this->DrawSprite(argument, false, 0.0f);
  }
}

// OFFSET: 0x00CB20F0

// OFFSET: 0x00CB2830

// OFFSET: 0x00CB35F0
// FUNCTION: fn_21105
void CRenderObject::DrawShadow(int argument, float scale) {
  if (this->render.unknown_010C == 565 || this->render.unknown_00A4Bytes[0] == 0 || this->render.unknown_0054 == 0.0f || scale == 0.0f) {
    return;
  }

  const float* matrix = reinterpret_cast<const float*>(argument);
  const float z = this->render.position.z;
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  float position[3];
  position[0] = this->render.position.x;
  position[1] = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
  position[2] = z;
  const float depth = position[1] * matrix[5] + position[0] * matrix[2] + z * matrix[8] + matrix[11];

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(position, matrix, &screenX, &screenY, &reciprocalDepth);
  const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("shadow.spr"));
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("shadow.act"));
  const SpriteFrameCell& cell = action->Frame(0, 0).Cell(0);
  if (cell.image == -1) {
    return;
  }

  float pulse = 1.0f;
  const SpriteRleImage* image = sprite->images[cell.imageType][0];
  this->ScaleByPulse(pulse);
  const double size = pulse * scale * this->render.unknown_0054;
  const double factor = distanceScale;

  SpriteQuad quad;
  quad.top = static_cast<float>(cell.y * factor * cell.scaleY * size + screenY);
  quad.bottom = static_cast<float>(((image->unknown_0006 + 1) * image->height + cell.y) * factor * cell.scaleY * size + screenY + 1.0);
  float left = static_cast<float>(cell.x * factor * cell.scaleX * size + screenX);
  quad.right = static_cast<float>(((image->unknown_0004 + 1) * image->width + cell.x) * factor * cell.scaleX * size + screenX + 1.0);
  if (quad.top >= static_cast<float>(g_renderer->viewport.height) || quad.bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
      quad.right <= 0.0f) {
    return;
  }

  quad.left = static_cast<float>(static_cast<int>(this->render.unknown_0078)) + left;
  quad.right = static_cast<float>(static_cast<int>(this->render.unknown_0078)) + quad.right;
  quad.color = static_cast<std::uint32_t>(((static_cast<std::uint32_t>(cell.color >> 24) << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8) |
               (cell.color >> 16 & 0xFF);
  quad.bottomRhw = 1.0f / (depth - (quad.bottom - screenY) * g_cotangent) + 0.00001f;
  quad.topRhw = 1.0f / (depth - (quad.top - screenY) * g_cotangent) + 0.00001f;
  quad.angle = static_cast<float>(cell.unknown_001C);
  quad.image = image;

  if (IsScreenShakeActive()) {
    this->DrawFlippedQuad(quad, sprite->palette, 257, screenX, screenY, 0);
    return;
  }

  this->DrawQuad(quad, sprite->palette, 257, 0.0f, 0.0f, 0);
}

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
    target->SendMsg(
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
