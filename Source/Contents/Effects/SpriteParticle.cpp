#include "Contents/Effects/SpriteParticle.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstring>

// GLOBAL: gl_24540
const float spriteShadowDepthEpsilon = 0.00001f;

// INLINED
static __forceinline float ToRadians(float degrees) {
  return (degrees / 180.0f) * 3.141592f;
}

// OFFSET: 0x00B40E30
// FUNCTION: fn_18864
void CSpriteParticle::UpdateFrame() {
  if (!this->action || this->loopMode == 3) {
    return;
  }

  const long long frameCount = static_cast<int>(this->action->FrameCount(this->animation));
  const long long delay = static_cast<long long>(this->frameDelay);
  if (delay == 0 || (this->unknown_00E8 >= delay && this->unknown_00E8 % delay == 0)) {
    ++this->frame;
  }

  switch (this->loopMode) {
  case 0:
    if (this->frame >= frameCount) {
      this->frame = 0;
    }
    break;
  case 1:
    if (this->frame >= frameCount) {
      this->life = -1.0f;
    }
    break;
  case 2:
    if (this->frame >= frameCount) {
      this->frame = static_cast<int>(frameCount) - 1;
    }
    break;
  }
}

// OFFSET: 0x00B40F30
// FUNCTION: fn_18865
void CSpriteParticle::sub_B40F30(char mode) {
  if (mode == 1) {
    this->UpdateFrame();
  }

  if ((this->life != 0.0f && static_cast<float>(this->unknown_00E8) >= this->life * 60.0f) || !this->sprite || !this->action) {
    return;
  }

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  float screenX;
  float screenY;
  float distance;
  g_renderer->ProjectViewport(&this->position.x, gameMode->camera->frame.view.values, &screenX, &screenY, &distance);
  float scaleDistance = g_renderer->ScaleDistance(distance);
  if (CRenderObject* renderObject = FindRenderObject(this->unknown_02F4)) {
    renderObject->ScaleByFadePulse(scaleDistance);
  }

  const ActionFrame& frame = this->action->Frame(this->animation, this->frame);
  for (int i = 0; i < frame.cellCount; ++i) {
    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = this->sprite->images[cell.imageType][cell.image];
    SpriteAtlasRecord record{};
    CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    if (!texture) {
      texture = g_renderer->atlas.Find(image, this->sprite->palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, this->sprite->palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      record.u0 = 0.0f;
      record.v0 = 0.0f;
      record.u1 = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
      record.v1 = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
    }

    float u0;
    float u1;
    if ((cell.mirrored & 1) == 0) {
      u0 = record.u0;
      u1 = record.u1;
    } else {
      u0 = record.u1;
      u1 = record.u0;
    }

    this->SetQuad(this->vertices, record.v1, record.v0, u0, u1);

    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    std::memcpy(primitive->vertices, this->vertices, sizeof(this->vertices));
    primitive->texture = texture;

    if ((this->unknown_00EC & 0x200) != 0) {
      const std::uint32_t a = static_cast<std::uint32_t>(
        static_cast<float>(this->alpha) * (1.0f / 255.0f) *
        static_cast<float>(cell.color >> 24) * (1.0f / 255.0f) * 255.0f
      );
      const std::uint32_t color = (((a << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8) | (cell.color >> 16 & 0xFF);
      primitive->vertices[0].diffuse = color;
      primitive->vertices[1].diffuse = color;
      primitive->vertices[2].diffuse = color;
      primitive->vertices[3].diffuse = color;
    }

    const float halfWidth = static_cast<float>(image->width) * g_depthAverageHalf;
    const float halfHeight = static_cast<float>(image->height) * g_depthAverageHalf;
    const float cellScaleX = (scaleDistance * cell.scaleX) / (distance * g_renderer->viewport.scaleX);
    const float cellScaleY = (scaleDistance * cell.scaleY) / (distance * g_renderer->viewport.scaleY);

    matrix scaleMatrix;
    scaleMatrix.SetScale(cellScaleX * halfWidth, cellScaleY * halfHeight, 1.0f);
    matrix cellRotation;
    cellRotation.MakeZRotation(ToRadians(static_cast<float>(cell.unknown_001C)));
    matrix cellTranslate;
    cellTranslate.MakeTranslate((static_cast<float>(cell.x) + halfWidth) * cellScaleX, (static_cast<float>(cell.y) + halfHeight) * cellScaleY, 0.0f);

    matrix temp1;
    temp1.Compose(scaleMatrix, cellRotation);
    matrix cellMatrix;
    cellMatrix.Compose(temp1, cellTranslate);

    vector3d cellVertices[4];
    for (int v = 0; v < 4; ++v) {
      cellVertices[v].x = 0.0f;
      cellVertices[v].y = 0.0f;
      cellVertices[v].z = 0.0f;
    }
    for (int v = 0; v < 4; ++v) {
      const float vx = primitive->vertices[v].x;
      const float vy = primitive->vertices[v].y;
      const float vz = primitive->vertices[v].z;
      cellVertices[v].x = cellMatrix.values[0] * vx + cellMatrix.values[3] * vy + cellMatrix.values[6] * vz + cellMatrix.values[9];
      cellVertices[v].y = cellMatrix.values[1] * vx + cellMatrix.values[4] * vy + cellMatrix.values[7] * vz + cellMatrix.values[10];
      cellVertices[v].z = cellMatrix.values[2] * vx + cellMatrix.values[5] * vy + cellMatrix.values[8] * vz + cellMatrix.values[11];
    }

    matrix worldScale;
    worldScale.SetScale(this->scale, this->scale, 1.0f);
    matrix worldRotation;
    worldRotation.MakeZRotation(ToRadians(*reinterpret_cast<const float*>(&this->unknown_00C4)));
    worldRotation.AppendXRotation(ToRadians(gameMode->camera->camera.pitch));
    worldRotation.AppendYRotation(ToRadians(gameMode->camera->camera.yaw));
    matrix worldTranslate;
    worldTranslate.MakeTranslate(this->position.x, this->position.y, this->position.z);

    matrix temp2;
    temp2.Compose(worldScale, worldRotation);
    matrix world;
    world.Compose(temp2, worldTranslate);
    matrix finalTransform;
    finalTransform.Compose(world, this->transform);

    vector3d worldVertices[4];
    for (int v = 0; v < 4; ++v) {
      worldVertices[v].x = 0.0f;
      worldVertices[v].y = 0.0f;
      worldVertices[v].z = 0.0f;
    }
    for (int v = 0; v < 4; ++v) {
      const float vx = cellVertices[v].x;
      const float vy = cellVertices[v].y;
      const float vz = cellVertices[v].z;
      worldVertices[v].x = finalTransform.values[0] * vx + finalTransform.values[3] * vy + finalTransform.values[6] * vz + finalTransform.values[9];
      worldVertices[v].y = finalTransform.values[1] * vx + finalTransform.values[4] * vy + finalTransform.values[7] * vz + finalTransform.values[10];
      worldVertices[v].z = finalTransform.values[2] * vx + finalTransform.values[5] * vy + finalTransform.values[8] * vz + finalTransform.values[11];
      g_renderer->viewport.Projection().Project(&worldVertices[v].x, gameMode->camera->frame.view.values, primitive->vertices[v]);
    }

    if (CRenderObject* renderObject = FindRenderObject(this->unknown_02F4)) {
      float offsetX = this->unknown_02C0;
      float offsetY = this->unknown_02C4;
      CGameActor* actor = static_cast<CGameActor*>(renderObject);
      if (actor->Slot55() < 1) {
        if (this->unknown_02BC != 0) {
          float extent[2];
          float* p = actor->GetExtent(extent);
          offsetX += p[0];
          offsetY += p[1];
        }
        if (this->unknown_02BD != 0) {
          offsetY -= static_cast<float>(static_cast<double>(*reinterpret_cast<const long long*>(&renderObject->render.unknown_0128)));
        }
      }
      offsetX *= scaleDistance;
      offsetY *= scaleDistance;
      for (int v = 0; v < 4; ++v) {
        primitive->vertices[v].x += offsetX;
        primitive->vertices[v].y += offsetY;
      }
    }

    const float depthOffset = static_cast<float>(i) * spriteShadowDepthEpsilon + this->depthOffset;
    if (depthOffset != 0.0f) {
      for (int v = 0; v < 4; ++v) {
        primitive->vertices[v].rhw += depthOffset;
        primitive->vertices[v].z = RhwToDepth(primitive->vertices[v].rhw);
      }
    }

    primitive->unknown_0018 = 5;
    primitive->unknown_001C = (this->flags != 5) ? 6 : 2;
    g_renderer->queues.Submit(primitive, static_cast<std::uint16_t>(this->flags), g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}

// OFFSET: 0x00B41D60
// FUNCTION: fn_18866
void CSpriteParticle::SetAction(const char* name) {
  if (this->action) {
    this->action->Unlock();
  }

  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
  if (this->action) {
    this->action->Lock();
  }
}

// OFFSET: 0x00B41DA0
// FUNCTION: fn_18867
void CSpriteParticle::SetSprite(const char* name) {
  if (this->sprite) {
    this->sprite->Unlock();
  }

  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));
  if (this->sprite) {
    this->sprite->Lock();
  }
}
