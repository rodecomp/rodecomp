#include "Client/Mode/ModeMgr.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <cstdlib>
#include <windows.h>

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(tagRECT) == 0x10);
static_assert(offsetof(tagRECT, left) == 0x0);
static_assert(offsetof(tagRECT, top) == 0x4);
static_assert(offsetof(tagRECT, right) == 0x8);
static_assert(offsetof(tagRECT, bottom) == 0xC);
#endif

// OFFSET: 0x00CA9580
// FUNCTION: fn_21007
float __stdcall CRenderObject::Heading(float x, float z, float targetX, float targetZ) {
  const float deltaX = targetX - x;
  const float deltaZ = targetZ - z;
  float angle = (static_cast<float>(std::atan2(static_cast<double>(deltaX), static_cast<double>(-deltaZ))) / 3.141592f) * 180.0f;
  if (angle >= 360.0f) {
    angle -= 360.0f;
  }

  if (angle < 0.0f) {
    angle += 360.0f;
  }

  return angle;
}

// OFFSET: 0x00CA9610
// FUNCTION: fn_21008
void CRenderObject::Face(float x, float z) {
  const float deltaX = x - this->render.position.x;
  const float deltaZ = z - this->render.position.z;
  if (0.0f != deltaX || 0.0f != deltaZ) {
    float angle = (static_cast<float>(std::atan2(static_cast<double>(deltaX), static_cast<double>(-deltaZ))) / 3.141592f) * 180.0f;
    this->render.angle = angle;
    if (angle >= 360.0f) {
      angle -= 360.0f;
      this->render.angle = angle;
    }

    if (angle < 0.0f) {
      this->render.angle = angle + 360.0f;
    }
  }
}

// OFFSET: 0x00CA96C0
// FUNCTION: fn_21009
float CRenderObject::CalcDist(float x, float z) const {
  const float deltaX = x - this->render.position.x;
  const float deltaZ = z - this->render.position.z;
  return SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
}

// OFFSET: 0x00CA9700
// FUNCTION: fn_21010
float CRenderObject::CalcDist(float x, float y, float z) const {
  const float deltaX = x - this->render.position.x;
  const float deltaZ = z - this->render.position.z;
  const float deltaY = y - this->render.position.y;
  const float horizontal = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  return SquareRoot(horizontal * horizontal + deltaY * deltaY);
}

// OFFSET: 0x00CA9770
// FUNCTION: fn_21011
bool CRenderObject::PrepareSprite(
  int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float* bottomDepth,
  float* topDepth, const SpriteRleImage* image, SpriteQuad* quad, tagRECT* bounds, std::uint32_t* color, int offsetX, int offsetY, int flags
) {
  const float depthBias = this->render.distanceScale * g_cameraScaleX;
  const double scaleY = static_cast<double>(scale * cell.scaleY * this->render.scale);
  const double scaleX = static_cast<double>(scale * cell.scaleX * this->render.scale);
  float top = static_cast<float>(static_cast<double>(cell.y + offsetY) * scaleY + static_cast<double>(y));
  float bottom = static_cast<float>(
    static_cast<double>((image->doubleHeight + 1) * image->height + cell.y + offsetY) * scaleY + static_cast<double>(y) + 1.0
  );
  float left;
  float right;

  if (cell.mirrored & 1) {
    left = static_cast<float>(static_cast<double>((image->doubleWidth + 1) * image->width + cell.x + offsetX) * scaleX + static_cast<double>(x));
    right = static_cast<float>(static_cast<double>(offsetX + cell.x) * scaleX + static_cast<double>(x) + 1.0);
    if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || right >= static_cast<float>(g_renderer->viewport.width) ||
        left <= 0.0f) {
      return false;
    }
  } else {
    left = static_cast<float>(static_cast<double>(cell.x + offsetX) * scaleX + static_cast<double>(x));
    right = static_cast<float>(static_cast<double>((image->doubleWidth + 1) * image->width + cell.x + offsetX) * scaleX + static_cast<double>(x) + 1.0);
    if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
        right <= 0.0f) {
      return false;
    }
  }

  if ((this->render.flags & 0x80) != 0) {
    const short phase = this->render.effectPhase;
    float factor = 0.0f;
    if (phase <= 18) {
      factor = g_sinTable[phase * 5] * 0.3f;
    } else if (phase <= 20) {
      factor = 0.3f;
    } else if (phase <= 23) {
      factor = static_cast<float>(23 - phase) * 0.1f;
    }
    factor *= bottom - top;
    top -= factor;
    bottom -= factor;
  }

  if ((this->render.flags & 0x20) != 0) {
    const short phase = this->render.effectPhase;
    float factor = 1.0f;
    if (phase <= 3) {
      factor = (100.0f - static_cast<float>(phase) * 20.0f) * 0.01f;
    } else if (phase <= 33) {
      int remainder = std::rand() % 6;
      if (remainder < 0) {
        remainder = 0;
      }
      factor = 0.4f - static_cast<float>(remainder) * 0.01f;
    }
    top += (bottom - top) * (1.0f - factor);
  }

  if (this->render.flatDepth != 0) {
    *bottomDepth = (static_cast<float>(unused) * g_cameraScaleY + 0.1f - g_cameraScaleY * 20.0f) +
                   static_cast<float>(static_cast<int>(this->render.depthLayer)) * g_cameraScaleY;
    *topDepth = (static_cast<float>(unused) * g_cameraScaleY + 0.1f - g_cameraScaleY * 20.0f) +
                static_cast<float>(static_cast<int>(this->render.depthLayer)) * g_cameraScaleY;
  } else {
    const int depthLayer = static_cast<int>(this->render.depthLayer) + 3 + layer;
    const float height = bottom - y;
    const float rise = top - y;
    if (this->render.isLieOnGround) {
      *bottomDepth = 1.0f / (position[2] - height * g_cotangent) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      *topDepth = 1.0f / (position[2] - rise * g_cotangent) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    } else if (static_cast<unsigned char>(flags) != 0) {
      *bottomDepth = 1.0f / (height * 0.06f + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      *topDepth = 1.0f / (rise * 0.06f + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    } else {
      *bottomDepth = 1.0f / (height * g_tangent + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      *topDepth = 1.0f / (rise * g_tangent + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    }
  }

  quad->left = left + shift;
  quad->top = top;
  quad->bottom = bottom;
  quad->right = right + shift;
  quad->bottomRhw = *bottomDepth;
  quad->topRhw = *topDepth;
  quad->angle = static_cast<float>(cell.rotation + static_cast<int>(this->render.spriteAngle));
  bounds->left = static_cast<int>(left);
  bounds->top = static_cast<int>(top);
  bounds->right = static_cast<int>(right);
  bounds->bottom = static_cast<int>(bottom);

  float lightRed = 1.0f;
  float lightGreen = 1.0f;
  float lightBlue = 1.0f;
  if (this->render.useGroundLighting == 1 && g_serviceOptions.Get(TT_LIGHTMAP_ON_OFF)) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    GroundGeometry* ground = mode->scene->ground;
    if (ground->IsNewVer()) {
      int column = 0;
      int row = 0;
      mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
      ground->Lighting(column, row, lightRed, lightGreen, lightBlue);
    }
  }

  const float alpha = static_cast<float>((this->render.color >> 24) & 0xFF);
  const float red = static_cast<float>((this->render.color >> 16) & 0xFF) * lightRed;
  const float green = static_cast<float>((this->render.color >> 8) & 0xFF) * lightGreen;
  const float blue = static_cast<float>(this->render.color & 0xFF) * lightBlue;
  if (this->render.isSingleColor && this->render.overrideColor != 0xFFFF0000) {
    *color = this->render.overrideColor;
  } else {
    unsigned char* channels = reinterpret_cast<unsigned char*>(color);
    channels[3] = static_cast<std::uint8_t>(static_cast<int>((static_cast<float>((cell.color >> 24) & 0xFF) * alpha) / 255.0f));
    channels[2] = static_cast<std::uint8_t>(static_cast<int>((static_cast<float>(cell.color & 0xFF) * red) / 255.0f));
    channels[1] = static_cast<std::uint8_t>(static_cast<int>((static_cast<float>((cell.color >> 8) & 0xFF) * green) / 255.0f));
    channels[0] = static_cast<std::uint8_t>(static_cast<int>((static_cast<float>((cell.color >> 16) & 0xFF) * blue) / 255.0f));
  }
  quad->color = *color;

  if ((this->render.flags & 0x40) != 0) {
    const short phase = this->render.effectPhase;
    if (phase <= 36) {
      float angle = 180.0f - g_sinTable[90 + phase * 5] * 180.0f;
      quad->angle = angle;
      if (angle >= 360.0f) {
        quad->angle = angle - 360.0f;
        return true;
      }
    } else {
      quad->angle = 0.0f;
    }
  }

  return true;
}

// OFFSET: 0x00CAD9E0
// FUNCTION: fn_21057
bool CRenderObject::GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const {
  const CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  bool found = true;

  if (targetAid == g_session.m_aid) {
    const vector3d& playerPos = mode->scene->player->render.position;
    targetPos.x = playerPos.x;
    targetPos.y = playerPos.y;
    targetPos.z = playerPos.z;
  } else if (const CRenderObject* actor = mode->scene->GetGameActorByAID(targetAid)) {
    targetPos.x = actor->render.position.x;
    targetPos.y = actor->render.position.y;
    targetPos.z = actor->render.position.z;
  } else {
    found = false;
    const vector3d& playerPos = mode->scene->player->render.position;
    const vector3d& position = this->render.position;
    const vector3d delta = {playerPos.x - position.x, playerPos.y - position.y, playerPos.z - position.z};
    targetPos.x = position.x - delta.x;
    targetPos.y = position.y - delta.y;
    targetPos.z = position.z - delta.z;
  }

  return found;
}
