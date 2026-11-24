#include "Contents/Actors/MousePointer.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/TerrainResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CAFD30
// FUNCTION: fn_21088
bool CMousePointer::OnProcess() {
  const float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
  this->render.previousAnimation = this->render.animation;
  this->render.curMotion = static_cast<int>(frame / this->render.speed) % static_cast<int>(this->render.action->FrameCount(this->render.animation));
  if (this->m_remainingFrames != 0) {
    if (--this->m_remainingFrames == 0) {
      this->render.visible = 0;
    }
  }
  return true;
}

// OFFSET: 0x00CBF860
// FUNCTION: fn_21114
void CMousePointer::SendMsg(CGameObject*, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t, std::int64_t, std::int64_t, std::int64_t) {
  if (message != 1) {
    if (message == 2) {
      if (b != 0) {
        this->m_remainingFrames = static_cast<int>(b);
      } else {
        this->render.visible = 0;
      }
    }
  } else {
    const float* const position = reinterpret_cast<const float*>(static_cast<std::intptr_t>(b));
    this->render.position.x = position[0];
    this->render.position.z = position[1];
    CGameMode* mode = g_modeMgr.GetGameMode();
    this->render.position.y = mode->scene->terrain->Height(this->render.position.x, this->render.position.z);
    this->render.visible = 1;
    this->m_remainingFrames = 0;
    if (c != 0) {
      this->render.color = 0xFFFF0000;
    } else {
      this->render.color = 0xFFFFFFFF;
    }
  }
}

