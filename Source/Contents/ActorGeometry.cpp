#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/World/WorldScene.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>

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
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
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
float CRenderObject::HorizontalDistance(float x, float z) const {
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
  return SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
}

// OFFSET: 0x00CA9700
// FUNCTION: fn_21010
float CRenderObject::Distance(float x, float y, float z) const {
  const float deltaX = x - this->render.position[0];
  const float deltaZ = z - this->render.position[2];
  const float deltaY = y - this->render.position[1];
  const float horizontal = SquareRoot(deltaZ * deltaZ + deltaX * deltaX);
  return SquareRoot(horizontal * horizontal + deltaY * deltaY);
}

// OFFSET: 0x00CAD9E0
// FUNCTION: fn_21057
bool CRenderObject::GetTargetPos(std::uint32_t targetAid, vector3d& targetPos) const {
  const WorldScene* world = static_cast<CGameMode*>(g_world.ActiveGameMode())->scene;
  const CRenderObject* player = reinterpret_cast<const CRenderObject*>(world->player);

  if (targetAid == g_session.m_aid) {
    targetPos = {player->render.position[0], player->render.position[1], player->render.position[2]};
    return true;
  }

  if (const CRenderObject* actor = world->GetGameActorByAID(targetAid)) {
    targetPos = {actor->render.position[0], actor->render.position[1], actor->render.position[2]};
    return true;
  }

  const float* position = this->render.position;
  targetPos = {
    position[0] - (player->render.position[0] - position[0]), position[1] - (player->render.position[1] - position[1]),
    position[2] - (player->render.position[2] - position[2])
  };
  return false;
}
