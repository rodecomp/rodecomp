#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/NavigationManager.h"
#include "Contents/World/WorldScene.h"

// OFFSET: 0x00B961E0
// FUNCTION: fn_19581
bool CNavigationManager::PlayerCell(int* column, int* row) {
  CGameMode* mode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  if (mode == nullptr) {
    return false;
  }
  mode->PlayerCell(column, row);
  return true;
}

// OFFSET: 0x00CD5510
// FUNCTION: fn_21282
void CGameMode::PlayerCell(int* column, int* row) const {
  const auto* player = reinterpret_cast<const CRenderObject*>(this->scene->player);
  this->WorldCell(player->render.position[0], player->render.position[2], *column, *row);
}
