#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>

// GLOBAL: gl_24934
int g_searchStoreX;
// GLOBAL: gl_24935
int g_searchStoreY;

// OFFSET: 0x00CCFCE0
// FUNCTION: fn_21246
void MiniMapNoOp() {
}

// OFFSET: 0x00CCFFA0
// FUNCTION: fn_21250
void CGameMode::StepTowardWalkableCell(const int& targetX, const int& targetY, int& x, int& y) {
  const int startX = x;
  const int startY = y;
  if (targetX > startX) {
    x = startX + 1;
  } else if (targetX < startX) {
    x = startX - 1;
  }
  if (targetY > y) {
    ++y;
  } else if (targetY < y) {
    --y;
  }

  if (this->scene->terrain->Grid().Walkable(x, y)) {
    return;
  }

  const int offsets[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};
  for (int i = 0; i < 8; ++i) {
    x = startX + offsets[i][0];
    y = startY + offsets[i][1];
    if (this->scene->terrain->Grid().Walkable(x, y)) {
      return;
    }
  }
}

// OFFSET: 0x00CD0150

// OFFSET: 0x00CD0300

// OFFSET: 0x00CD0380

// OFFSET: 0x00CD1450

// OFFSET: 0x00CD2BE0
