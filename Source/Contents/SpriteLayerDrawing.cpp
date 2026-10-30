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
// FUNCTION: fn_04808
void SpriteLayerRenderer::QueueQuad(int layer, const SpriteQuad* quad, const std::uint32_t* palette, int blend) {
  const int direction = this->pc->render.previousAnimation - this->pc->render.animation;
  if (this->layerViews[layer] > 0) {
    int priority = -1;
    g_luaScript->Call("GetLayerPriority", "ddd>d", direction, layer, this->layerViews[layer], &priority);
    if (priority == -1) {
      return;
    }

    SpriteLayerQuad entry;
    memcpy_s(&entry.quad, sizeof(entry.quad), quad, sizeof(SpriteQuad));
    entry.palette = palette;
    entry.blend = blend;
    this->quads.insert(std::make_pair(priority, entry));
  } else {
    SpriteLayerQuad entry;
    memcpy_s(&entry.quad, sizeof(entry.quad), quad, sizeof(SpriteQuad));
    entry.palette = palette;
    entry.blend = blend;
    this->quads.emplace(static_cast<int>(this->quads.size()) + 1, entry);
  }
}

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
// FUNCTION: fn_04816
std::pair<bool, std::string> SpriteLayerRenderer::GetLayerSound(int layer, CActRes* action, int sound) {
  if (layer != 6) {
    return {false, ""};
  }

  if (sound == -1) {
    return {false, ""};
  }

  std::string name = action->Sound(sound);
  if (name.find(".wav") != std::string::npos) {
    name.clear();
  }

  return {true, name};
}

// OFFSET: 0x00641E40
// FUNCTION: fn_04818
void SpriteLayerRenderer::LoadLayers() {
  for (int layer = 0; layer < 9; ++layer) {
    CRes* sprite = this->pc->GetEquipSprite(layer);
    CRes* action = this->pc->GetEquipAction(layer);
    if (!sprite || !action) {
      continue;
    }

    if (layer == 1) {
      const ActionFrame& frame = static_cast<CActRes*>(action)->Frame(this->pc->render.previousAnimation, this->pc->render.curMotion);
      if (frame.Cell(GetSlotLayer(1)).image == -1) {
        this->unknown_006D = true;
      }

      if (StateFlag11(this->pc->stateFlags) == true) {
        if (g_session.IsDoramJob(this->pc->unknown_025C) == true) {
          sprite = ResourceMgr::Instance().Load("이팩트\\orcface_doram.spr");
          action = ResourceMgr::Instance().Load("이팩트\\orcface_doram.act");
        } else {
          sprite = ResourceMgr::Instance().Load("이팩트\\orcface.spr");
          action = ResourceMgr::Instance().Load("이팩트\\orcface.act");
        }
      }
    }

    SpriteLayerResources& resources = this->layers[layer];
    resources.sprite = static_cast<CSprRes*>(sprite);
    resources.action = static_cast<CActRes*>(action);
  }

  for (int layer = 2; layer <= 4; ++layer) {
    if (g_session.IsSpecialMonsterJob(this->layerViews[layer]) == true) {
      this->unknown_006C = true;
      return;
    }
  }
}
