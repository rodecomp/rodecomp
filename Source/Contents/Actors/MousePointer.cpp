#include "Contents/Actors/MousePointer.h"

#include "Resource/Sprites/ActionResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CAFD30
// FUNCTION: fn_21088
bool CMousePointer::OnProcess() {
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.previousAnimation = this->render.animation;
  this->render.curMotion = static_cast<int>(frame / this->render.speed) % static_cast<int>(this->render.action->FrameCount(this->render.animation));
  if (this->m_remainingFrames != 0) {
    if (--this->m_remainingFrames == 0) {
      this->render.visible = 0;
    }
  }
  return true;
}
