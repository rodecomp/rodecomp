#include "Contents/SpriteLayerDrawing.h"

#include "Client/Session.h"
#include "Contents/Actors/ActorStateFlags.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/SpriteRidingOffset.h"
#include "Contents/SpriteRidingPolicy.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"

#include <cstring>

// OFFSET: 0x0063FC90

// OFFSET: 0x00640050

// OFFSET: 0x00640AD0

// OFFSET: 0x00640ED0

// OFFSET: 0x00641130
// FUNCTION: fn_04813
std::pair<int, int> SpriteLayerRenderer::GetItemOffset(int layer) {
  if (g_session.IsDoramJob(this->pc->unknown_025C) == true) {
    CPc* pc = this->pc;
    int turn = 0;
    if (pc->unknown_043C == 2) {
      turn = -1;
    } else if (pc->unknown_043C == 1) {
      turn = 1;
    }

    int direction = pc->render.previousAnimation - pc->render.animation + turn;
    if (direction < 0) {
      direction = 7;
    }
    if (direction > 7) {
      direction = 0;
    }

    int x = 0;
    int y = 0;
    g_luaScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", this->layerViews[layer], direction, pc->unknown_0260, &x, &y);
    return std::make_pair(x, y);
  }

  return std::make_pair(0, 0);
}

// OFFSET: 0x006412B0
// FUNCTION: fn_04814
const std::uint32_t* SpriteLayerRenderer::GetPalette(int layer, CSprRes* sprite, CActRes*) {
  switch (layer) {
    case 0:
      if (static_cast<int>(this->pc->render.unknown_0048) > 0) {
        return static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->pc->render.name))->palette;
      }
      break;
    case 1:
      if (this->pc->unknown_0444 > 0) {
        return static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->pc->unknown_0478))->palette;
      }
      break;
  }

  return sprite->palette;
}

// OFFSET: 0x00641420
// FUNCTION: fn_04815
std::pair<bool, SpriteLayerResources> SpriteLayerRenderer::FindLayer(int layer) {
  const auto found = this->layers.find(layer);
  if (found == this->layers.end()) {
    return std::make_pair(false, SpriteLayerResources{});
  }

  return std::make_pair(true, found->second);
}

// OFFSET: 0x00641620

// OFFSET: 0x00641E40
