#include "Contents/Effects/ArrowEffect.h"

#include "Client/Mode/ModeMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4BA20
// FUNCTION: fn_24277
void CArrowEffect::Reset() {
  this->render.animation = 0;
  this->startPos = this->render.position;
  this->startFrame = 0.0f;
  this->velocity.x = 0.0f;
  this->velocity.y = 0.0f;
  this->velocity.z = 0.0f;
  this->render.curMotion = 0;
  this->frameCount = 0;
}

// OFFSET: 0x00E4BE60
// FUNCTION: fn_24280
bool CArrowEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  if (this->startFrame > frame) {
    return true;
  }

  if (this->render.visible == 0) {
    this->render.visible = 1;

    vector3d target;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    if (this->targetAid == 0 || !this->GetTargetPos(this->targetAid, target)) {
      target.z = this->targetPos.z;
      target.x = this->targetPos.x;
      target.y = this->targetPos.y;
    }

    target.y -= 10.0f;
    this->velocity.z = target.z - this->render.position.z;
    this->velocity.y = target.y - this->render.position.y;
    this->velocity.x = (target.x - this->render.position.x) * 0.125f;
    this->velocity.y *= 0.125f;
    this->velocity.z *= 0.125f;

    float distance = this->CalcDist(target.x, target.y, target.z);
    if (distance < 40.0f) {
      this->speedScale = 40.0f / distance;
      this->velocity.x = this->speedScale * this->velocity.x;
      this->velocity.y = this->speedScale * this->velocity.y;
      this->velocity.z = this->speedScale * this->velocity.z;
    }

    this->Face(target.x, target.z);

    float dy = target.y - this->render.position.y;
    float dx = target.x - this->render.position.x;
    float dz = target.z - this->render.position.z;
    float inverse = 1.0f / SquareRoot(dy * dy + dx * dx + dz * dz);
    this->render.position.x += inverse * dx * 2.5f;
    this->render.position.y += inverse * dy * 2.5f;
    this->render.position.z += inverse * dz * 2.5f;
  }

  frame -= this->startFrame;
  ++this->frameCount;
  this->render.previousAnimation = this->render.animation;
  if (this->frameCount > 8) {
    ++this->render.curMotion;
    if (this->render.curMotion >= static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation))) {
      this->render.curMotion = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation)) - 1;
    }
  }

  this->render.unknown_007C = this->GetDir(this->render.angle);
  g_modeMgr.GetGameMode();
  this->render.position.x = frame * this->velocity.x + this->startPos.x;
  this->render.position.y = frame * this->velocity.y + this->startPos.y;
  this->render.position.z = frame * this->velocity.z + this->startPos.z;
  if (frame > 8.0f / this->speedScale) {
    return false;
  }

  return true;
}
