#include "Contents/Actors/GameObject.h"
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
