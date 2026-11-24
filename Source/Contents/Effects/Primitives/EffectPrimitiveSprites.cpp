#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Effects/Primitives/EffectPrim.h"
#include "Contents/Effects/Primitives/EffectPrimitiveState.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/World/World.h"
#include "Render/Projection/RendererViewport.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdint>

// OFFSET: 0x00BF58F0
// FUNCTION: fn_19997
__declspec(safebuffers) void CEffectPrim::RenderLaunchEffects(matrix&) {
  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.x = 0.0f;
  vec1.y = 0.0f;
  vec1.z = 0.0f;
  vec2.x = 0.0f;
  vec2.y = 0.0f;
  vec2.z = 0.0f;
  vec3.x = 0.0f;
  vec3.y = 0.0f;
  vec3.z = 0.0f;
  vec4.x = 0.0f;
  vec4.y = 0.0f;
  vec4.z = 0.0f;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1.x = rx * g_cosTable[45];
    vec1.y = 0.0f;
    vec1.z = rx * g_sinTable[45];
    vec2.x = rx * g_cosTable[135];
    vec2.y = 0.0f;
    vec2.z = rx * g_sinTable[135];
    vec3.x = rx * g_cosTable[225];
    vec3.y = 0.0f;
    vec3.z = rx * g_sinTable[225];
    vec4.x = rx * g_cosTable[315];
    vec4.y = 0.0f;
    vec4.z = rx * g_sinTable[315];
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const float baseX = slot.vecBPre.x;
    const float baseY = slot.vecBPre.y;
    const float baseZ = slot.vecBPre.z;
    vec1 = vector3d{baseX + vec1.x, baseY + vec1.y, baseZ + vec1.z};
    vec2.x = baseX + vec2.x;
    vec2.y = baseY + vec2.y;
    vec2.z = baseZ + vec2.z;
    vec3.x = baseX + vec3.x;
    vec3.y = baseY + vec3.y;
    vec3.z = baseZ + vec3.z;
    vec4.x = baseX + vec4.x;
    vec4.y = baseY + vec4.y;
    vec4.z = baseZ + vec4.z;

    if (vec1.y <= 0.0f && slot.process < 300 && static_cast<int>(slot.height[13]) > 0) {
      if (slot.flag1[0] == 0) {
        if (static_cast<float>(g_session.m_aid) == slot.height[10]) {
          CGameMode* gameMode = g_modeMgr.GetGameMode();
          CRagEffect* effect = gameMode->scene->player->LaunchEffect(EF_SOULLIGHT2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          if (effect) {
            vector3d position{baseX, baseY, baseZ};
            effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
          }
        } else {
          CGameMode* gameMode = g_modeMgr.GetGameMode();
          if (auto* actor = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(slot.height[10]))) {
            CRagEffect* effect = actor->LaunchEffect(EF_SOULLIGHT2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
            if (effect) {
              vector3d position{baseX, baseY, baseZ};
              effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
            }
          }
        }
      } else {
        if (static_cast<float>(g_session.m_aid) == slot.height[10]) {
          CGameMode* gameMode = g_modeMgr.GetGameMode();
          CRagEffect* effect = gameMode->scene->player->LaunchEffect(EF_SOULLIGHT3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
          if (effect) {
            vector3d position{baseX, baseY, baseZ};
            effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
          }
        } else {
          CGameMode* gameMode = g_modeMgr.GetGameMode();
          if (auto* actor = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(slot.height[10]))) {
            CRagEffect* effect = actor->LaunchEffect(EF_SOULLIGHT3, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
            if (effect) {
              vector3d position{baseX, baseY, baseZ};
              effect->SendMsg(nullptr, AM_SETPOS, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
            }
          }
        }
      }
    }
  }
}

// OFFSET: 0x00BF5D80
// FUNCTION: fn_19998
void CEffectPrim::RenderGroundSprite(matrix& wtm) {
  if (this->state.master) {
    this->render.position = this->state.master->render.position;
  }

  const float z = this->render.position.z;
  const float height = g_modeMgr.GetGameMode()->scene->terrain->Height(this->render.position.x, this->render.position.z);
  vector3d position;
  position.x = this->render.position.x;
  position.y = height;
  position.z = z;
  const float depth = position.x * wtm.values[2] + position.y * wtm.values[5] + z * wtm.values[8] + wtm.values[11];

  float screenX;
  float screenY;
  float reciprocalDepth;
  g_renderer->ProjectViewport(&position.x, wtm.values, &screenX, &screenY, &reciprocalDepth);
  float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

  if (this->state.master->state.type == EF_SOUL_REAPER_COLLECT && this->state.master && this->state.master->state.parent) {
    this->state.master->state.parent->ScaleByFadePulse(distanceScale);
  }

  CSprRes* sprite = this->GetSprite();
  CActRes* action = this->GetActRes();
  const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);

  for (int index = 0; index < frame.cellCount; ++index) {
    const SpriteFrameCell& cell = action->Frame(this->render.previousAnimation, this->render.curMotion).Cell(index);
    if (cell.image == -1) {
      return;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    const int cellX = cell.x;
    const int cellY = cell.y;
    const double factor = distanceScale;
    const float left = static_cast<float>(cellX * factor * cell.scaleX + screenX);
    const float right = static_cast<float>(((static_cast<short>(image->doubleWidth) + 1) * image->width + cellX) * factor * cell.scaleX + screenX + 1.0);
    const float lift = static_cast<float>(static_cast<double>(this->state.spriteLift));
    const float top = static_cast<float>(cellY * factor * cell.scaleY + screenY) - lift;
    const float bottom = static_cast<float>(((static_cast<short>(image->doubleHeight) + 1) * image->height + cellY) * factor * cell.scaleY + screenY + 1.0) - lift;
    if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
        right <= 0.0f) {
      return;
    }

    const std::uint32_t color = (((((cell.colorChannels.alpha << 8) + cell.colorChannels.red) << 8) + cell.colorChannels.green) << 8) + cell.colorChannels.blue;
    SpriteQuad quad;
    quad.image = image;
    quad.top = top;
    quad.left = static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) + left;
    quad.bottom = bottom;
    quad.right = static_cast<float>(static_cast<int>(this->render.spriteOffsetX)) + right;
    quad.bottomRhw = 1.0f / (depth - (bottom - screenY) * g_cotangent) + 0.00001f;
    quad.topRhw = 1.0f / (depth - (top - screenY) * g_cotangent) + 0.00001f;
    quad.angle = static_cast<float>(cell.rotation);
    const float alphaScale = static_cast<unsigned char>(static_cast<int>(this->state.alpha)) / 255.0f;
    quad.color = static_cast<std::uint32_t>(static_cast<unsigned char>(static_cast<int>(alphaScale * static_cast<float>(color >> 24)))) << 24 | (color & 0xFFFFFF);
    this->DrawQuad(quad, sprite->palette, this->state.renderFlag, 0.0f, 0.0f, 0);
  }
}

// INLINED
static __declspec(safebuffers) __forceinline void DrawTrailSpriteQuad(
  CEffectPrim& prim, const vector3d& base, float top, float left, float bottom, float right, float screenY, float depth, float depthBias,
  float angle, std::uint32_t color, const SpriteRleImage* image, const std::uint32_t* palette, int blend, float border = 0.0f
) {
  SpriteQuad quad;
  quad.image = image;
  quad.top = base.y + (top - border);
  quad.bottom = base.y + (bottom + border);
  quad.left = base.x + (left - border);
  quad.right = base.x + (right + border);
  quad.color = color;
  quad.bottomRhw = 1.0f / ((bottom - screenY) * g_tangent + depth) + depthBias;
  quad.topRhw = 1.0f / ((top - screenY) * g_tangent + depth) + depthBias;
  quad.angle = angle;
  prim.DrawQuad(quad, palette, blend, 0.0f, 0.0f, 0);
}

// OFFSET: 0x00BF6170
// FUNCTION: fn_19999
void CEffectPrim::RenderSegmentSpriteTrail(matrix& wtm) {
  CSprRes* sprite = this->GetSprite();
  CActRes* action = this->GetActRes();
  const std::uint32_t alpha = static_cast<unsigned short>(static_cast<short>(this->state.alpha));

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life <= 0 || !(this->state.alpha > 0.0f)) {
      continue;
    }

    for (int seg = 0; seg < this->state.numSegment; ++seg) {
      EffectPrimitiveRecord& record = this->state.records[seg];
      const float depth = record.position.y * wtm.values[5] + record.position.x * wtm.values[2] + record.position.z * wtm.values[8] + wtm.values[11];

      float screenX;
      float screenY;
      float reciprocalDepth;
      g_renderer->ProjectViewport(&record.position.x, wtm.values, &screenX, &screenY, &reciprocalDepth);
      const float distanceScale = g_renderer->ScaleDistance(reciprocalDepth);

      const ActionFrame& frame = action->Frame(this->render.previousAnimation, this->render.curMotion);
      for (int index = 0; index < frame.cellCount; ++index) {
        const SpriteFrameCell& cell = frame.Cell(index);
        if (cell.image == -1) {
          continue;
        }

        const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
        const double scale = record.scale;
        const float top = static_cast<float>(cell.y * scale * cell.scaleY * distanceScale + screenY);
        const float bottom = static_cast<float>(((static_cast<short>(image->doubleHeight) + 1) * image->height + cell.y) * scale * cell.scaleY * distanceScale + screenY);
        if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f) {
          continue;
        }

        float left;
        float right;
        if ((cell.mirrored & 1) == 0) {
          left = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
          right = static_cast<float>(((static_cast<short>(image->doubleWidth) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
          if (left >= static_cast<float>(g_renderer->viewport.width) || right <= 0.0f) {
            continue;
          }
        } else {
          left = static_cast<float>(((static_cast<short>(image->doubleWidth) + 1) * image->width + cell.x) * scale * cell.scaleX * distanceScale + screenX);
          right = static_cast<float>(cell.x * scale * cell.scaleX * distanceScale + screenX);
          if (right >= static_cast<float>(g_renderer->viewport.width) || left <= 0.0f) {
            continue;
          }
        }

        const std::uint32_t red = cell.colorChannels.red;
        const std::uint32_t green = cell.colorChannels.green;
        const std::uint32_t blue = cell.colorChannels.blue;
        const std::uint8_t alphaByte = static_cast<unsigned char>(alpha);
        const std::uint32_t color = (((((alphaByte << 8) + red) << 8) + green) << 8) + blue;
        const float angle = static_cast<float>(cell.rotation) + this->state.roll;
        DrawTrailSpriteQuad(*this, this->state.slots[ec].vecBPre, top, left, bottom, right, screenY, depth, 0.0001f, angle, color, image, sprite->palette, 0x101);

        const std::uint32_t paleGlow = (alpha << 24) + 0x64FF64;
        const std::uint32_t brightGlow = (alpha << 24) + 0x0AFF0A;
        DrawTrailSpriteQuad(
          *this, this->state.slots[ec].vecBPre, top, left, bottom, right, screenY, depth, 0.0002f, angle, this->state.slots[ec].flag1[0] == 2 ? brightGlow : paleGlow, image,
          sprite->palette, 5, g_effectIconOuterRadius
        );

        const std::uint8_t halfAlphaByte = static_cast<unsigned char>(alpha / 2);
        const std::uint32_t halfColor = (((((halfAlphaByte << 8) + red) << 8) + green) << 8) + blue;
        DrawTrailSpriteQuad(*this, this->state.slots[ec].vecBNow, top, left, bottom, right, screenY, depth, 0.0001f, angle, halfColor, image, sprite->palette, 0x101);
        DrawTrailSpriteQuad(
          *this, this->state.slots[ec].vecBNow, top, left, bottom, right, screenY, depth, 0.0002f, angle, this->state.slots[ec].flag1[0] == 2 ? brightGlow : paleGlow, image,
          sprite->palette, 5, g_effectIconOuterRadius
        );

        DrawTrailSpriteQuad(*this, this->state.slots[ec].vecTPre, top, left, bottom, right, screenY, depth, 0.0001f, angle, color, image, sprite->palette, 0x101);
        DrawTrailSpriteQuad(
          *this, this->state.slots[ec].vecTPre, top, left, bottom, right, screenY, depth, 0.0002f, angle, this->state.slots[ec].flag1[0] == 2 ? brightGlow : paleGlow, image,
          sprite->palette, 5, g_effectIconOuterRadius
        );
      }
    }
  }
}
