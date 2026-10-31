#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Ground/GroundGeometry.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/SpriteGeometry.h"
#include "Render/Textures/SpritePixels.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <cstdlib>
#include <windows.h>

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
  if (deltaX != 0.0f || deltaZ != 0.0f) {
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
bool CRenderObject::ProjectCell(
  int unused, int layer, float scale, const SpriteFrameCell& cell, float x, float y, const float* position, float shift, float& bottomDepth,
  float& topDepth, const SpriteRleImage& image, SpriteCellQuad& quad, tagRECT& bounds, SpriteColor& color, int offsetX, int offsetY, int flags
) {
  const float depthBias = this->render.unknown_005C * g_cameraScaleX;
  const double scaleY = static_cast<double>(scale * cell.scaleY * this->render.scale);
  const double scaleX = static_cast<double>(scale * cell.scaleX * this->render.scale);
  float top = static_cast<float>(static_cast<double>(cell.y + offsetY) * scaleY + static_cast<double>(y));
  float bottom = static_cast<float>(
    static_cast<double>((image.unknown_0006 + 1) * image.height + cell.y + offsetY) * scaleY + static_cast<double>(y) + 1.0
  );
  float left;
  float right;

  if (cell.mirrored & 1) {
    left = static_cast<float>(static_cast<double>((image.unknown_0004 + 1) * image.width + cell.x + offsetX) * scaleX + static_cast<double>(x));
    right = static_cast<float>(static_cast<double>(offsetX + cell.x) * scaleX + static_cast<double>(x) + 1.0);
    if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || right >= static_cast<float>(g_renderer->viewport.width) ||
        left <= 0.0f) {
      return false;
    }
  } else {
    left = static_cast<float>(static_cast<double>(cell.x + offsetX) * scaleX + static_cast<double>(x));
    right = static_cast<float>(static_cast<double>((image.unknown_0004 + 1) * image.width + cell.x + offsetX) * scaleX + static_cast<double>(x) + 1.0);
    if (top >= static_cast<float>(g_renderer->viewport.height) || bottom <= 0.0f || left >= static_cast<float>(g_renderer->viewport.width) ||
        right <= 0.0f) {
      return false;
    }
  }

  if ((this->render.flags & 0x80) != 0) {
    const short phase = this->render.unknown_00F4;
    float factor = 0.0f;
    if (phase < 19) {
      factor = g_sinTable[phase * 5] * 0.3f;
    } else if (phase < 21) {
      factor = 0.3f;
    } else if (phase < 24) {
      factor = static_cast<float>(23 - phase) * 0.1f;
    }
    factor *= bottom - top;
    top -= factor;
    bottom -= factor;
  }

  if ((this->render.flags & 0x20) != 0) {
    const short phase = this->render.unknown_00F4;
    float factor = 1.0f;
    if (phase < 4) {
      factor = (100.0f - static_cast<float>(phase) * 20.0f) * 0.01f;
    } else if (phase < 34) {
      int remainder = std::rand() % 6;
      if (remainder < 0) {
        remainder = 0;
      }
      factor = 0.4f - static_cast<float>(remainder) * 0.01f;
    }
    top += (bottom - top) * (1.0f - factor);
  }

  if (this->render.unknown_00A2 != 0) {
    bottomDepth = (static_cast<float>(unused) * g_cameraScaleY + 0.1f - g_cameraScaleY * 20.0f) +
                  static_cast<float>(static_cast<int>(this->render.unknown_0080)) * g_cameraScaleY;
    topDepth = (static_cast<float>(unused) * g_cameraScaleY + 0.1f - g_cameraScaleY * 20.0f) +
               static_cast<float>(static_cast<int>(this->render.unknown_0080)) * g_cameraScaleY;
  } else {
    const int depthLayer = static_cast<int>(this->render.unknown_0080) + 3 + layer;
    const float height = bottom - y;
    const float rise = top - y;
    if (this->render.isLieOnGround) {
      bottomDepth = 1.0f / (position[2] - height * g_cotangent) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      topDepth = 1.0f / (position[2] - rise * g_cotangent) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    } else if (static_cast<unsigned char>(flags) != 0) {
      bottomDepth = 1.0f / (height * 0.06f + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      topDepth = 1.0f / (rise * 0.06f + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    } else {
      bottomDepth = 1.0f / (height * g_tangent + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
      topDepth = 1.0f / (rise * g_tangent + position[2]) + static_cast<float>(depthLayer) * g_cameraScaleY + depthBias;
    }
  }

  quad.left = left + shift;
  quad.top = top;
  quad.bottom = bottom;
  quad.right = right + shift;
  quad.bottomDepth = bottomDepth;
  quad.topDepth = topDepth;
  quad.angle = static_cast<float>(cell.unknown_001C + static_cast<int>(this->render.unknown_007C));
  bounds.left = static_cast<LONG>(left);
  bounds.top = static_cast<LONG>(top);
  bounds.right = static_cast<LONG>(right);
  bounds.bottom = static_cast<LONG>(bottom);

  float lightRed = 1.0f;
  float lightGreen = 1.0f;
  float lightBlue = 1.0f;
  if (this->render.unknown_00B4 == 1 && g_serviceOptions.Get(0x77)) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    GroundGeometry* ground = mode->scene->ground;
    if (ground->Slot10()) {
      int column = 0;
      int row = 0;
      mode->WorldCell(this->render.position.x, this->render.position.z, column, row);
      ground->Lighting(column, row, lightRed, lightGreen, lightBlue);
    }
  }

  if (this->render.isSingleColor && this->render.overrideColor != 0xFFFF0000) {
    color = *reinterpret_cast<SpriteColor*>(&this->render.overrideColor);
  } else {
    color.alpha = static_cast<std::uint8_t>(
      static_cast<int>((static_cast<float>((cell.color >> 24) & 0xFF) * static_cast<float>((this->render.color >> 24) & 0xFF)) / 255.0f)
    );
    color.red = static_cast<std::uint8_t>(
      static_cast<int>((static_cast<float>(cell.color & 0xFF) * static_cast<float>((this->render.color >> 16) & 0xFF) * lightRed) / 255.0f)
    );
    color.green = static_cast<std::uint8_t>(
      static_cast<int>((static_cast<float>((cell.color >> 8) & 0xFF) * static_cast<float>((this->render.color >> 8) & 0xFF) * lightGreen) / 255.0f)
    );
    color.blue = static_cast<std::uint8_t>(
      static_cast<int>((static_cast<float>((cell.color >> 16) & 0xFF) * static_cast<float>(this->render.color & 0xFF) * lightBlue) / 255.0f)
    );
  }
  quad.color = color;

  if ((this->render.flags & 0x40) != 0) {
    const short phase = this->render.unknown_00F4;
    if (phase <= 36) {
      float angle = 180.0f - g_sinTable[90 + phase * 5] * 180.0f;
      quad.angle = angle;
      if (angle >= 360.0f) {
        quad.angle = angle - 360.0f;
        return true;
      }
    } else {
      quad.angle = 0.0f;
    }
  }

  return true;
}

// OFFSET: 0x00CAD9E0
// FUNCTION: fn_21057
bool CRenderObject::GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const {
  const CWorld* world = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
  const CRenderObject* player = reinterpret_cast<const CRenderObject*>(world->player);

  if (targetAid == g_session.m_aid) {
    targetPos = {player->render.position.x, player->render.position.y, player->render.position.z};
    return true;
  }

  if (const CRenderObject* actor = world->GetGameActorByAID(targetAid)) {
    targetPos = {actor->render.position.x, actor->render.position.y, actor->render.position.z};
    return true;
  }

  const vector3d& position = this->render.position;
  targetPos = {
    position.x - (player->render.position.x - position.x), position.y - (player->render.position.y - position.y),
    position.z - (player->render.position.z - position.z)
  };
  return false;
}
