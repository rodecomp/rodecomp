#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"
#include "Contents/SpriteRidingOffset.h"
#include "Contents/SpriteRidingPolicy.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x007D4840
// FUNCTION: fn_09582
FuncDrawWNDChar::~FuncDrawWNDChar() = default;

// OFFSET: 0x007E3E60
// FUNCTION: fn_09727
void FuncDrawWNDChar::Draw(bool option) {
  std::list<int> layers = this->DrawOrder();
  this->LoadResources(layers);
  for (int layer : layers) {
    if (this->m_resources.find(layer) == this->m_resources.end()) {
      continue;
    }

    std::pair<CSprRes*, CActRes*> resources = this->m_resources[layer];
    CSprRes* sprite = resources.first;
    CActRes* act = resources.second;
    if (!sprite || !act) {
      continue;
    }

    int key = layer;
    FuncDrawOffset offset;
    if (g_session.IsDoramJob(this->m_job) == true) {
      int x = 0;
      int y = 0;
      int turn = 0;
      if (this->m_motion == 2) {
        turn = -1;
      } else if (this->m_motion == 1) {
        turn = 1;
      }

      int direction = this->m_action % 8 + turn;
      if (direction < 0) {
        direction = 7;
      }
      if (direction > 7) {
        direction = 0;
      }

      g_luaScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", this->m_layers[key], direction, g_session.SyncUnknown0E98(), &x, &y);
      offset.x = x;
      offset.y = y;
    }

    const ActionFrame* frame = &act->Frame(this->m_action, this->m_motion);
    switch (layer) {
      case 2: {
        FuncDrawOffset body = this->BodyAnchorOffset(frame, option);
        if (this->unknown_0094 == 1) {
          continue;
        }

        offset.x += body.x;
        offset.y += body.y;
        break;
      }
      case 3:
      case 4:
      case 5:
      case 6: {
        if (!this->unknown_0088 && this->m_resources.find(2) != this->m_resources.end() && this->m_resources[2].second) {
          CActRes* bodyAct = this->m_resources.find(2) != this->m_resources.end() ? this->m_resources[2].second : nullptr;
          this->BodyAnchorOffset(&bodyAct->Frame(this->m_action, this->m_motion), option);
        }

        FuncDrawOffset anchor = this->LayerAnchorOffset(layer, &frame, act);
        offset.x += anchor.x;
        offset.y += anchor.y;
        break;
      }
      case 0:
      case 7: {
        int baseMotion = this->m_motion;
        int syncedMotion = this->m_motion;
        CActRes* layerAct = this->m_resources.find(layer) != this->m_resources.end() ? this->m_resources[layer].second : nullptr;
        int action = this->m_action;
        if ((action >= 0 && action < 8) || static_cast<unsigned>(action - 16) <= 7) {
          baseMotion = 0;
        }

        CActRes* bodyAct = this->m_resources.find(1) != this->m_resources.end() ? this->m_resources[1].second : nullptr;
        g_session.GetSyncedFrame(bodyAct, layerAct, action, baseMotion, this->m_time, &syncedMotion);
        frame = &layerAct->Frame(this->m_action, syncedMotion);
        break;
      }
    }

    const std::uint32_t* palette = sprite->palette;
    if (layer == 1) {
      if (this->m_bodyStyle > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->unknown_0050))->palette;
      }
    } else if (layer == 2 && this->m_headPalette > 0) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->unknown_0068))->palette;
    }

    const SpriteFrameCell& cell = frame->Cell(this->IsSpecialLayer(layer));
    if (option == true) {
      if (cell.image == -1) {
        continue;
      }

      SpriteAtlasRecord record{};
      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
      if (cell.imageType == 0 && !g_renderer->atlas.Find(image, palette, record)) {
        g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }

      float scale = GetSkillFactor(this->m_job);
      int offsetX = static_cast<int>(static_cast<float>(offset.x) * scale);
      int offsetY = static_cast<int>(static_cast<float>(offset.y) * scale);
      UIWindow* window = this->m_window;
      window->AddSpriteCellOverlay(
        this->m_x + offsetX, this->m_y + offsetY, reinterpret_cast<int>(act), reinterpret_cast<int>(sprite), image, &cell, scale,
        static_cast<float>(cell.unknown_001C), window->Color(), palette, 1.0f
      );
    } else {
      if (cell.image == -1) {
        continue;
      }

      SpriteAtlasRecord record{};
      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
      CTexture* texture;
      if (cell.imageType == 0) {
        texture = g_renderer->atlas.Find(image, palette, record);
        if (!texture) {
          texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
        }
      } else {
        texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
      }

      int x = this->m_x + cell.x + offset.x;
      int y = cell.y + this->m_y + offset.y;
      int clip = y < 0 ? y : 0;
      int top = y < 0 ? 0 : y;
      if (clip <= top) {
        this->m_window->geometry.surface->DrawSurface(
          x, top, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f - static_cast<float>(clip)), image->width,
          image->height + clip, cell.mirrored, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
        );
      }
    }
  }
}

// OFFSET: 0x007E4680
// FUNCTION: fn_09728
std::list<int> FuncDrawWNDChar::DrawOrder() {
  std::map<int, int> defaultPriorities;
  defaultPriorities[3] = 4;
  defaultPriorities[4] = 3;
  defaultPriorities[5] = 2;
  defaultPriorities[6] = 8;

  std::map<int, int> order;
  for (int layer = 0; layer < 8; ++layer) {
    switch (layer) {
      case 0:
        if (this->m_robe <= 0 || this->m_drawOnTop) {
          break;
        }
        [[fallthrough]];
      case 1:
        order[layer] = layer;
        break;
      case 2:
        order[3] = 2;
        break;
      case 6:
        if (!this->m_helmRobe) {
          break;
        }
        [[fallthrough]];
      case 3:
      case 4:
      case 5:
        if (this->m_layers[layer] > 0) {
          int priority = -1;
          g_session.m_hotkeyScript->Call("GetLayerPriority", "ddd>d", 0, defaultPriorities[layer], this->m_layers[layer], &priority);
          order[priority] = layer;
        }
        break;
      case 7:
        if (this->m_robe > 0 && this->m_drawOnTop) {
          order[g_session.IsTopLayerRobe(this->m_robe) ? 400 : 2] = layer;
        }
        break;
    }
  }

  std::list<int> layers;
  if (this->m_drawOnTop) {
    for (std::map<int, int>::reverse_iterator it = order.rbegin(); it != order.rend(); ++it) {
      if (it->first < 0) {
        layers.push_back(it->second);
      } else {
        layers.push_front(it->second);
      }
    }
  } else {
    for (std::map<int, int>::iterator it = order.begin(); it != order.end(); ++it) {
      layers.push_back(it->second);
    }
  }
  return layers;
}

// OFFSET: 0x007E4FB0
// FUNCTION: fn_09729
bool FuncDrawWNDChar::DrawOnTop() {
  int job = this->m_job;
  bool drawOnTop = true;

  if (g_session.m_newRobeDrawOnTop == 1) {
    if (this->m_flags == 1) {
      job = g_session.GetRidingJob(job);
    }

    int layer = job;
    if (g_session.m_hotkeyScript->Call("_New_DrawOnTop", "ddddd>bd", this->m_robe, this->m_sex, job, this->m_action, this->m_motion, &drawOnTop, &layer) == 1) {
      return drawOnTop;
    }
  } else if (g_session.m_hotkeyScript->Call("DrawOnTop", "ddddd>b", this->m_robe, this->m_sex, job, this->m_action, this->m_motion, &drawOnTop) == 1) {
    return drawOnTop;
  }

  return true;
}

// OFFSET: 0x007E5120
// FUNCTION: fn_09730
int FuncDrawWNDChar::IsSpecialLayer(int layer) {
  const auto found = this->m_layers.find(layer);
  if (found != this->m_layers.end() && g_session.IsSpecialMonsterJob(found->second) == true) {
    return 1;
  }

  return layer == 2;
}

// OFFSET: 0x007E5190
// FUNCTION: fn_09731
FuncDrawOffset FuncDrawWNDChar::BodyAnchorOffset(const ActionFrame* frame, bool keepScale) {
  FuncDrawOffset offset;

  CActRes* act = this->m_resources.find(1) != this->m_resources.end() ? this->m_resources[1].second : nullptr;
  const ActionFrame& base = act->Frame(this->m_action, this->m_motion);
  if (frame->anchorCount > 0 && base.anchorCount > 0) {
    const ActionAnchor* frameAnchor = frame->anchors.data();
    const ActionAnchor* baseAnchor = base.anchors.data();
    if (frameAnchor->unknown_0008 == baseAnchor->unknown_0008) {
      if (this->m_shield != 0) {
        if (keepScale == true) {
          offset.x += baseAnchor->x - frameAnchor->x;
          offset.y += baseAnchor->y - frameAnchor->y;
        } else {
          offset.x = static_cast<int>(static_cast<float>(baseAnchor->x) / base.Cell(0).scaleX - static_cast<float>(frameAnchor->x) + static_cast<float>(offset.x));
          float baseY = static_cast<float>(base.anchors.data()->y);
          offset.y = static_cast<int>(baseY / base.Cell(0).scaleY - static_cast<float>(frame->anchors.data()->y) + static_cast<float>(offset.y));
        }
      } else {
        offset.x = baseAnchor->x - frameAnchor->x;
        offset.y = baseAnchor->y - frameAnchor->y;
      }

      this->unknown_0090 = offset.y;
      this->unknown_008C = offset.x;
    }
  }

  this->unknown_0088 = 1;
  return offset;
}

// OFFSET: 0x007E5310
// FUNCTION: fn_09732
FuncDrawOffset FuncDrawWNDChar::LayerAnchorOffset(int layer, const ActionFrame** frame, CActRes* act) {
  const ActionFrame* current = *frame;
  FuncDrawOffset offset;

  CActRes* bodyAct = this->m_resources.find(2) != this->m_resources.end() ? this->m_resources[2].second : nullptr;
  const ActionFrame& base = bodyAct->Frame(this->m_action, this->m_motion);
  int syncedMotion = 0;
  const ActionFrame* synced = g_session.GetSyncedFrame(bodyAct, act, this->m_action, this->m_motion, this->m_time, &syncedMotion);
  if (synced && syncedMotion != this->m_motion) {
    *frame = synced;
    current = synced;
  }

  if (current->anchorCount > 0 && base.anchorCount > 0) {
    const ActionAnchor* baseAnchor = base.anchors.data();
    const ActionAnchor* frameAnchor = current->anchors.data();
    if (frameAnchor->unknown_0008 == baseAnchor->unknown_0008) {
      offset.x = baseAnchor->x - frameAnchor->x;
      offset.y = baseAnchor->y - frameAnchor->y;
    }
  }

  offset.x += this->unknown_008C;
  int ridingOffset;
  if (IsIgnoredRidingState(this->m_layers[layer], this->m_job, this->m_action - this->m_action % 8, this->m_flags) == true) {
    ridingOffset = SpriteRidingOffset(&base, this->m_action, this->m_motion, this->m_job, this->m_sex, this->m_body);
  } else {
    ridingOffset = this->unknown_0090;
  }

  offset.y += ridingOffset;
  if (layer == 4) {
    short accessory = this->m_topAccessory;
    if (accessory == 181 || accessory == 197) {
      offset.y -= 7;
    } else if (accessory == 200 && g_session.SyncUnknown0E98() == 0) {
      offset.y -= 4;
    }
  }

  return offset;
}

// OFFSET: 0x007E5A30
// FUNCTION: fn_09733
void FuncDrawWNDChar::LoadResources(const std::list<int>& layers) {
  for (int layer : layers) {
    std::string sprName;
    std::string actName;
    switch (layer) {
      case 0:
      case 7:
        if (this->m_shield == 0) {
          sprName = g_session.GetRobeSprName(this->m_job, this->m_hairColor, this->m_sex, this->m_robe, this->m_flags, this->m_body);
          actName = g_session.GetRobeActName(this->m_job, this->m_hairColor, this->m_sex, this->m_robe, this->m_flags, this->m_body);
        }
        break;
      case 1: {
        int job = this->m_flags == 1 ? g_session.GetRidingJob(this->m_job) : this->m_job;
        sprName = g_session.GetBodySprName(job, this->m_sex, this->m_body, this->m_shield);
        actName = GetBodyActionResourceName(job, this->m_sex, this->m_body, this->m_shield);
        this->unknown_0050 = g_session.GetBodyPalName(job, this->m_sex, this->m_bodyStyle, this->m_body, this->m_shield, 0);
        break;
      }
      case 2: {
        int head = this->m_head;
        sprName = g_session.GetHeadSprName(this->m_job, this->m_sex, &head);
        actName = g_session.GetHeadActName(this->m_job, this->m_sex, &head);
        this->m_head = static_cast<short>(head);
        this->unknown_0068 = g_session.GetHeadPalName(this->m_head, this->m_job, this->m_sex, this->m_headPalette);
        break;
      }
      case 3:
      case 4:
      case 5:
      case 6:
        sprName = g_session.GetAccessorySprName(this->m_job, this->m_sex, this->m_head, this->m_layers[layer]);
        actName = g_session.GetAccessoryActName(this->m_job, this->m_sex, this->m_head, this->m_layers[layer]);
        break;
    }

    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName));
    CActRes* act = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName));
    if (sprite && act) {
      std::pair<CSprRes*, CActRes*>& entry = this->m_resources[layer];
      entry.first = sprite;
      entry.second = act;
    }
  }

  for (int layer = 3; layer <= 5; ++layer) {
    if (g_session.IsSpecialMonsterJob(this->m_layers[layer]) == true) {
      this->unknown_0094 = 1;
      return;
    }
  }
}
