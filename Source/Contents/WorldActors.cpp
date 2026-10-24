#include "Contents/RenderObject.h"
#include "Contents/WorldActors.h"

// OFFSET: 0x00AC8260
// FUNCTION: fn_17668
CRenderObject* WorldActors::Find(std::uint32_t accountId) const {
  for (CRenderObject* actor : this->actors) {
    if (actor->render.accountId == accountId) {
      return actor;
    }
  }

  return nullptr;
}

// OFFSET: 0x00CAD9E0
// FUNCTION: fn_21057
bool WorldActors::TargetPosition(const float* origin, std::uint32_t accountId, float* output) const {
  CRenderObject* target;

  if (accountId == this->localAccountId) {
    target = this->localPlayer;
  } else {
    target = this->Find(accountId);
  }

  if (target) {
    output[0] = target->render.position[0];
    output[1] = target->render.position[1];
    output[2] = target->render.position[2];
    return true;
  }

  const float x = origin[0];
  const float y = origin[1];
  const float z = origin[2];
  const float deltaX = this->localPlayer->render.position[0] - x;
  const float deltaZ = this->localPlayer->render.position[2] - z;
  const float reflectedX = x - deltaX;
  const float deltaY = this->localPlayer->render.position[1] - y;
  const float reflectedZ = z - deltaZ;
  output[0] = reflectedX;
  output[1] = y - deltaY;
  output[2] = reflectedZ;
  return false;
}
