#include "Contents/Actors/Item.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Resource/World/TerrainResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00D8B5D0
// FUNCTION: fn_22250
bool CItem::OnProcess() {
  const float elapsed = static_cast<float>((timeGetTime() - this->render.unknown_008C) / 24.0);
  this->render.isSingleColor = static_cast<int>(elapsed) % 92 >= 90;

  if (this->isFalling) {
    this->render.position.y = (elapsed * 0.083f + this->fallVelocity) * elapsed + this->fallOrigin;
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (this->render.position.y > mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z)) {
      this->render.position.y = mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
      this->isFalling = 0;
    }
  }

  return true;
}

// OFFSET: 0x00D8B6D0
// FUNCTION: fn_22251
void CItem::Render(int argument) {
  if (this->render.visible) {
    this->DrawShadow(argument, this->render.scale * 0.4f);
    this->DrawSprite(argument, false, 0.0f);
  }
}
