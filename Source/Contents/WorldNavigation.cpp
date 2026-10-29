#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/NavigationManager.h"
#include "Contents/TerrainPathSearch.h"
#include "Contents/World/WorldScene.h"

#include <windows.h>
#include <mmsystem.h>

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
  this->WorldCell(player->render.position.x, player->render.position.z, *column, *row);
}

// OFFSET: 0x00CCFC10
// FUNCTION: fn_21245
bool CGameMode::CanFindPath(int sx, int sy, int cellX, int cellY, int dx, int dy) {
  MovementRouteInterpolation pathInfo;
  return g_pathFinder.FindPath(timeGetTime(), sx, sy, cellX, cellY, dx, dy, 150, pathInfo, 15);
}
