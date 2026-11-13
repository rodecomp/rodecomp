#include "Contents/Effects/Particles/SpriteParticle.h"
#include "Contents/Actors/ActorEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
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

// INLINED
static __forceinline bool IsParticleActive(const CSpriteParticle* particle) {
  if (0.0f == particle->life) {
    return true;
  }
  return particle->life * 60.0f > static_cast<float>(particle->elapsedFrames);
}

// OFFSET: 0x00B40B60
// FUNCTION: fn_18861
CSpriteParticle::CSpriteParticle(vector3d position, int flags)
  : CParticle(position, flags), applyActorExtent(0), applyActorLift(0), velocity({0.0f, 0.0f, 0.0f}), sprite(nullptr), action(nullptr) {
  this->Reset();
}

// OFFSET: 0x00B40D90
// FUNCTION: fn_18863
void CSpriteParticle::Reset() {
  CParticle::Reset();
  this->sprite = nullptr;
  this->action = nullptr;
  this->frameDelay = 0;
  this->spin = 0;
  this->velocity.x = 0;
  this->velocity.y = 0;
  this->velocity.z = 0;
  this->animation = 0;
  this->frame = 0;
  this->loopMode = 0;
  this->applyActorExtent = 0;
  this->applyActorLift = 0;
  this->screenOffsetX = 0;
  this->screenOffsetY = 0;
  this->actorId = 0;
}

// OFFSET: 0x00B40E30
// FUNCTION: fn_18864
void CSpriteParticle::UpdateFrame() {
  if (!this->action || this->loopMode == 3) {
    return;
  }

  const long long frameCount = static_cast<int>(this->action->FrameCount(this->animation));
  const long long delay = static_cast<long long>(this->frameDelay);
  if (0 == delay || (this->elapsedFrames >= delay && 0 == this->elapsedFrames % delay)) {
    ++this->frame;
  }

  switch (this->loopMode) {
  case MT_LOOP:
    if (this->frame >= frameCount) {
      this->frame = 0;
    }
    break;
  case MT_ONETIME:
    if (this->frame >= frameCount) {
      this->life = -1.0f;
    }
    break;
  case MT_STATIC:
    if (this->frame >= frameCount) {
      this->frame = static_cast<int>(frameCount) - 1;
    }
    break;
  }
}

// OFFSET: 0x00B40F30
// FUNCTION: fn_18865
void CSpriteParticle::Draw(char mode) {
  if (mode == 1) {
    this->UpdateFrame();
  }

  if (!IsParticleActive(this)) {
    return;
  }
  CSprRes* sprite = this->sprite;
  if (!sprite) {
    return;
  }
  CActRes* action = this->action;
  if (!action) {
    return;
  }

  const float* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values;
  float screenX;
  float screenY;
  float distance;
  g_renderer->ProjectViewport(&this->position.x, view, &screenX, &screenY, &distance);
  float scaleDistance = g_renderer->ScaleDistance(distance);
  if (CRenderObject* renderObject = FindRenderObject(this->actorId)) {
    renderObject->ScaleByFadePulse(scaleDistance);
  }

  const ActionFrame& frame = action->Frame(this->animation, this->frame);
  for (int i = 0; i < frame.cellCount; ++i) {
    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    SpriteAtlasRecord record{};
    CTexture* texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    if (!texture) {
      texture = g_renderer->atlas.Find(image, sprite->palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(image, sprite->palette, record);
      }
    } else {
      record.u0 = 0.0f;
      record.v0 = 0.0f;
      const CTexture* widthTexture = static_cast<const CTexture*>(image->surface);
      record.u1 = static_cast<float>(widthTexture->ImageWidth()) / static_cast<float>(widthTexture->TextureWidth());
      const CTexture* heightTexture = static_cast<const CTexture*>(image->surface);
      record.v1 = static_cast<float>(heightTexture->ImageHeight()) / static_cast<float>(heightTexture->TextureHeight());
    }

    this->SetQuad(this->vertices, record.v1, record.v0, (cell.mirrored & 1) == 0 ? record.u0 : record.u1, (cell.mirrored & 1) == 0 ? record.u1 : record.u0);

    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    std::memcpy(primitive->vertices, this->vertices, sizeof(this->vertices));
    primitive->texture = texture;

    if ((this->renderOptions & 0x200) != 0) {
      const int a = static_cast<int>(
        static_cast<float>(this->alpha) * (1.0f / 255.0f) *
        (static_cast<float>(cell.color >> 24) * (1.0f / 255.0f)) * 255.0f
      );
      for (int v = 0; v < 4; ++v) {
        primitive->vertices[v].diffuse =
          ((static_cast<std::uint8_t>(a) << 8 | (cell.color & 0xFF)) << 8 | (cell.color >> 8 & 0xFF)) << 8 | (cell.color >> 16 & 0xFF);
      }
    }

    const float halfWidth = static_cast<float>(image->width) * g_depthAverageHalf;
    const float halfHeight = static_cast<float>(image->height) * g_depthAverageHalf;
    const float cellScaleX = (scaleDistance * cell.scaleX) / (distance * g_renderer->viewport.scaleX);
    const float cellScaleY = (scaleDistance * cell.scaleY) / (distance * g_renderer->viewport.scaleY);

    matrix scaleMatrix;
    scaleMatrix.SetScale(cellScaleX * halfWidth, cellScaleY * halfHeight, 1.0f);
    matrix cellRotation;
    cellRotation.MakeZRotation(ToRadians(static_cast<float>(cell.rotation)));
    matrix cellTranslate;
    cellTranslate.MakeTranslate((static_cast<float>(cell.x) + halfWidth) * cellScaleX, (static_cast<float>(cell.y) + halfHeight) * cellScaleY, 0.0f);

    matrix temp;
    temp.Compose(scaleMatrix, cellRotation);
    matrix cellMatrix;
    cellMatrix.Compose(temp, cellTranslate);

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
    worldRotation.MakeZRotation(ToRadians(this->angle));
    worldRotation.AppendXRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.pitch));
    worldRotation.AppendYRotation(ToRadians(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw));
    matrix worldTranslate;
    worldTranslate.MakeTranslate(this->position.x, this->position.y, this->position.z);

    temp.Compose(worldScale, worldRotation);
    matrix world;
    world.Compose(temp, worldTranslate);
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
      g_renderer->viewport.Project(&worldVertices[v].x, view, primitive->vertices[v]);
    }

    if (CRenderObject* renderObject = FindRenderObject(this->actorId)) {
      float offsetX = this->screenOffsetX;
      float offsetY = this->screenOffsetY;
      CGameActor* actor = static_cast<CGameActor*>(renderObject);
      if (actor->GetMonsterTransformJob() <= 0) {
        if (this->applyActorExtent != 0) {
          float extent[2];
          float* p = actor->GetExtent(extent);
          offsetX += p[0];
          offsetY += p[1];
        }
        if (this->applyActorLift != 0) {
          offsetY -= static_cast<float>(static_cast<double>(*reinterpret_cast<const long long*>(&renderObject->render.hoverOffsetLow)));
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

    primitive->sourceBlend = 5;
    primitive->destinationBlend = (this->flags != 5) ? 6 : 2;
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
