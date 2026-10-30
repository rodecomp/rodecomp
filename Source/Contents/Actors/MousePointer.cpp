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

// OFFSET: 0x00CBF860
// FUNCTION: fn_21114
void CMousePointer::SendMsg(CGameObject*, int message, int a, int b, int c, int d, int e, int, int, int, int, int, int, int, int) {
  if (message == 1 && a == 0) {
    const float* const position = reinterpret_cast<const float*>(b);
    this->render.position.x = position[0];
    this->render.position.z = position[1];
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->render.position.y = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
    this->render.visible = 1;
    this->m_remainingFrames = 0;
    if ((d | e) != 0) {
      this->render.color = 0xFFFF0000;
    } else {
      this->render.color = 0xFFFFFFFF;
    }
  } else if (message == 2 && a == 0) {
    if ((b | c) != 0) {
      this->m_remainingFrames = b;
    } else {
      this->render.visible = 0;
    }
  }
}
