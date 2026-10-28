#include "Contents/Actors/GameObject.h"
#include "Client/Replay.h"
#include "Contents/Actors/RenderObject.h"
#include "Resource/Sprites/ActionResource.h"
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

// OFFSET: 0x00CB3F40

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

// OFFSET: 0x00CB4100

// OFFSET: 0x00CB4800
