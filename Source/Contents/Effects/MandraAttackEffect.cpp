#include "Contents/Effects/MandraAttackEffect.h"

#include "Resource/Sprites/ActionResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4C310
// FUNCTION: fn_24285
bool CMandraAttackEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  float startFrame = this->startFrame;
  if (startFrame > frame) {
    this->render.visible = 0;
    return true;
  }

  this->render.visible = 1;
  if (!this->render.isMotionFinished) {
    int frameCount = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation));
    float elapsed = (frame - startFrame) / this->render.speed;
    this->render.curMotion = static_cast<int>(elapsed) % frameCount;
    if (elapsed / frameCount >= 1.0f) {
      this->render.isMotionFinished = true;
      this->render.curMotion = frameCount - 1;
    }
  }

  return !this->render.isMotionFinished;
}
