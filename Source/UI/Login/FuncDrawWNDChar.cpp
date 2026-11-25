#include "UI/Login/FuncDrawWNDChar.h"
#include "Client/Session/Session.h"
#include "Script/LuaScript.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x007E3860
// FUNCTION: fn_09724
FuncDrawWNDChar::FuncDrawWNDChar(
  UIWindow* window, int x, int y, unsigned char sex, short job, int hairColor, short body, short head, short bottomAccessory, short topAccessory,
  short midAccessory, int robe, unsigned char flags, int shield, int bodyStyle, int headPalette, int action, int motion, int time
)
    : m_window(window), m_x(x), m_y(y), m_sex(sex), m_job(job), m_hairColor(hairColor), m_body(body), m_head(head), m_bottomAccessory(bottomAccessory),
      m_topAccessory(topAccessory), m_midAccessory(midAccessory), m_robe(robe), m_flags(flags), m_shield(shield), m_bodyStyle(bodyStyle),
      m_headPalette(headPalette), m_action(action), m_motion(motion), m_time(time), m_resources(), bodyPaletteName(),
      headPaletteName(), m_layers(), customRendering(0) {
  this->hideHeadLayer = 0;
  this->offsetX = 0;
  this->offsetY = 0;
  g_session.m_hotkeyScript->CallTable(std::string("TB_Layer_Priority", 17), std::string("IsHelmRobe", 10), "d>b", this->m_robe, &this->m_helmRobe);
  this->m_drawOnTop = this->DrawOnTop();
  this->m_layers[WNDCHAR_LAYER_HEAD_LOW] = this->m_bottomAccessory;
  this->m_layers[WNDCHAR_LAYER_HEAD_TOP] = this->m_topAccessory;
  this->m_layers[WNDCHAR_LAYER_HEAD_MID] = this->m_midAccessory;
  this->m_layers[WNDCHAR_LAYER_HELM_ROBE] = this->m_robe;
}

// OFFSET: 0x007E3E60
// FUNCTION: fn_09727
void FuncDrawWNDChar::Draw(bool option) {
  std::list<int> layers = this->DrawOrder();
  this->LoadResources(layers);
  for (std::list<int>::iterator it = layers.begin(); it != layers.end(); ++it) {
    int layer = *it;
    std::pair<CSprRes*, CActRes*> resource = this->Resource(layer);
    CSprRes* sprite = resource.first;
    CActRes* act = resource.second;
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

      int direction = this->m_action % 8;
      direction += turn;
      if (direction < 0) {
        direction = 7;
      }
      if (direction > 7) {
        direction = 0;
      }

      g_session.m_hotkeyScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", this->m_layers[key], direction, g_session.GetCurrentSex(), &x, &y);
      offset.x = x;
      offset.y = y;
    }

    const ActionFrame* frame = &act->Frame(this->m_action, this->m_motion);
    if (layer == WNDCHAR_LAYER_HEAD) {
      FuncDrawOffset body = this->BodyAnchorOffset(frame, option);
      if (this->customRendering == 1) {
        continue;
      }

      offset.y += body.y;
      offset.x += body.x;
    } else {
      switch (layer) {
        case WNDCHAR_LAYER_HEAD_MID:
        case WNDCHAR_LAYER_HEAD_TOP:
        case WNDCHAR_LAYER_HEAD_LOW:
        case WNDCHAR_LAYER_HELM_ROBE: {
          if (!this->hideHeadLayer && this->Resource(2).second) {
            this->BodyAnchorOffset(&this->Resource(2).second->Frame(this->m_action, this->m_motion), option);
          }

          FuncDrawOffset anchor = this->LayerAnchorOffset(layer, &frame, act);
          offset.x += anchor.x;
          offset.y += anchor.y;
          break;
        }
        default:
          if (layer == WNDCHAR_LAYER_ROBE_BEHIND || layer == WNDCHAR_LAYER_ROBE_FRONT) {
            int baseMotion = this->m_motion;
            int syncedMotion = this->m_motion;
            CActRes* layerAct = this->Resource(layer).second;
            int action = this->m_action;
            if ((action >= 0 && action < 8) || static_cast<unsigned>(action - 16) <= 7) {
              baseMotion = 0;
            }

            g_session.GetSyncedFrame(this->Resource(1).second, layerAct, action, baseMotion, this->m_time, &syncedMotion);
            frame = &layerAct->Frame(this->m_action, syncedMotion);
          }
          break;
      }
    }

    const std::uint32_t* palette = sprite->palette;
    switch (layer - 1) {
      case 0:
        if (this->m_bodyStyle > 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->bodyPaletteName))->palette;
        }
        break;
      case 1:
        if (this->m_headPalette > 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->headPaletteName))->palette;
        }
        break;
    }

    const SpriteFrameCell& cell = frame->Cell(this->IsSpecialLayer(layer));
    if (cell.image == -1) {
      continue;
    }

    SpriteAtlasRecord record{};
    record.u0 = 0.0f;
    record.v0 = 0.0f;
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    if (option == true) {
      if (cell.imageType == 0 && !g_renderer->atlas.Find(image, palette, record)) {
        g_renderer->atlas.Insert(image, palette, record);
      }

      float scale = g_session.GetSkillFactor(this->m_job);
      int offsetX = static_cast<int>(static_cast<float>(offset.x) * scale);
      int offsetY = static_cast<int>(static_cast<float>(offset.y) * scale);
      UIWindow* window = this->m_window;
      window->AddSpriteCellOverlay(
        this->m_x + offsetX, this->m_y + offsetY, reinterpret_cast<int>(act), reinterpret_cast<int>(sprite), image, &cell, scale,
        static_cast<float>(cell.rotation), window->Color(), palette, 1.0f
      );
    } else {
      CTexture* texture;
      if (cell.imageType == 0) {
        texture = g_renderer->atlas.Find(image, palette, record);
        if (!texture) {
          texture = g_renderer->atlas.Insert(image, palette, record);
        }
      } else {
        texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
      }

      int x = this->m_x + cell.x + offset.x;
      int y = cell.y + this->m_y + offset.y;
      int clip = y;
      int top = 0;
      if (y >= 0) {
        clip = 0;
        top = y;
      }
      if (clip <= top) {
        this->m_window->geometry.surface->DrawSurface(
          x, top, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f - static_cast<float>(clip)), image->width,
          image->height + clip, cell.mirrored, image->doubleWidth + 1, image->doubleHeight + 1, 0
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
    int priority;
    switch (layer) {
      case WNDCHAR_LAYER_ROBE_BEHIND:
        if (this->m_robe <= 0 || this->m_drawOnTop == true) {
          break;
        }
        [[fallthrough]];
      case WNDCHAR_LAYER_BODY:
        order[layer] = layer;
        break;
      case WNDCHAR_LAYER_HEAD:
        order[3] = 2;
        break;
      case WNDCHAR_LAYER_HELM_ROBE:
        if (!this->m_helmRobe) {
          break;
        }
        [[fallthrough]];
      case WNDCHAR_LAYER_HEAD_MID:
      case WNDCHAR_LAYER_HEAD_TOP:
      case WNDCHAR_LAYER_HEAD_LOW:
        if (this->m_layers[layer] > 0) {
          priority = -1;
          g_session.m_hotkeyScript->Call("GetLayerPriority", "ddd>d", 0, defaultPriorities[layer], this->m_layers[layer], &priority);
          order[priority] = layer;
        }
        break;
      case WNDCHAR_LAYER_ROBE_FRONT:
        if (this->m_robe > 0 && this->m_drawOnTop) {
          if (g_session.IsTopLayerRobe(this->m_robe) == true) {
            order[400] = layer;
          } else {
            order[2] = layer;
          }
        }
        break;
    }
  }

  std::list<int> layers;
  if (this->m_drawOnTop) {
    const std::map<int, int>::reverse_iterator end = order.rend();
    for (std::map<int, int>::reverse_iterator it = order.rbegin(); it != end; ++it) {
      const auto [priority, layer] = *it;
      if (priority < 0) {
        layers.push_back(layer);
      } else {
        layers.push_front(layer);
      }
    }
  } else {
    for (const auto [priority, layer] : order) {
      layers.push_back(layer);
    }
  }
  return layers;
}

// OFFSET: 0x007E4FB0
// FUNCTION: fn_09729
bool FuncDrawWNDChar::DrawOnTop() {
  int job = this->m_job;

  if (g_session.m_newRobeDrawOnTop == 1) {
    int ridden;
    if (this->m_flags == 1) {
      ridden = g_session.GetRidingJob(job);
    } else {
      ridden = job;
    }

    bool drawOnTop = true;
    int layer = ridden;
    if (g_session.m_hotkeyScript->Call(std::string("_New_DrawOnTop", 14), "ddddd>bd", this->m_robe, this->m_sex, ridden, this->m_action, this->m_motion, &drawOnTop, &layer) == 1) {
      return drawOnTop;
    }
  } else {
    bool drawOnTop = true;
    if (g_session.m_hotkeyScript->Call(std::string("DrawOnTop", 9), "ddddd>b", this->m_robe, this->m_sex, job, this->m_action, this->m_motion, &drawOnTop) == 1) {
      return drawOnTop;
    }
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

  return layer == WNDCHAR_LAYER_HEAD;
}

// OFFSET: 0x007E5190
// FUNCTION: fn_09731
FuncDrawOffset FuncDrawWNDChar::BodyAnchorOffset(const ActionFrame* frame, bool keepScale) {
  FuncDrawOffset offset;

  CActRes* act = this->Resource(1).second;
  const ActionFrame& base = act->Frame(this->m_action, this->m_motion);
  if (frame->anchorCount > 0 && base.anchorCount > 0) {
    const ActionAnchor* baseAnchor = base.anchors.data();
    const ActionAnchor* frameAnchor = frame->anchors.data();
    if (frameAnchor->attachmentId == baseAnchor->attachmentId) {
      const int baseX = baseAnchor->x;
      if (this->m_shield != 0) {
        if (keepScale == true) {
          offset.x += baseX - frameAnchor->x;
          offset.y += baseAnchor->y - frameAnchor->y;
          this->offsetY = offset.y;
          this->offsetX = offset.x;
        } else {
          offset.x = static_cast<int>(static_cast<float>(baseX) / base.Cell(0).scaleX - static_cast<float>(frameAnchor->x) + static_cast<float>(offset.x));
          float baseY = static_cast<float>(base.anchors.data()->y);
          const ActionAnchor* currentAnchor = frame->anchors.data();
          offset.y = static_cast<int>(baseY / base.Cell(0).scaleY - static_cast<float>(currentAnchor->y) + static_cast<float>(offset.y));
          this->offsetY = offset.y;
          this->offsetX = offset.x;
        }
      } else {
        offset.x = baseX - frameAnchor->x;
        offset.y = baseAnchor->y - frameAnchor->y;
        this->offsetY = offset.y;
        this->offsetX = offset.x;
      }
    }
  }

  this->hideHeadLayer = 1;
  return offset;
}

// OFFSET: 0x007E5310
// FUNCTION: fn_09732
FuncDrawOffset FuncDrawWNDChar::LayerAnchorOffset(int layer, const ActionFrame** frame, CActRes* act) {
  const ActionFrame* current = *frame;
  FuncDrawOffset offset;

  CActRes* bodyAct = this->Resource(2).second;
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
    if (frameAnchor->attachmentId == baseAnchor->attachmentId) {
      offset.x = baseAnchor->x - frameAnchor->x;
      offset.y = baseAnchor->y - frameAnchor->y;
    }
  }

  offset.x += this->offsetX;
  if (IsIgnoredRidingState(this->m_layers[layer], this->m_job, this->m_action - this->m_action % 8, reinterpret_cast<const bool&>(this->m_flags)) == true) {
    offset.y += SpriteRidingOffset(&base, this->m_action, this->m_motion, this->m_job, this->m_sex, this->m_body);
  } else {
    offset.y += this->offsetY;
  }

  if (layer == WNDCHAR_LAYER_HEAD_TOP) {
    switch (this->m_topAccessory) {
    case 181:
    case 197:
      offset.y -= 7;
      break;
    case 200:
      if (g_session.GetCurrentSex() == 0) {
        offset.y -= 4;
      }
      break;
    }
  }

  return offset;
}

// OFFSET: 0x007E5A30
// FUNCTION: fn_09733
void FuncDrawWNDChar::LoadResources(const std::list<int>& layers) {
  for (auto it = layers.begin(); it != layers.end(); ++it) {
    const int& layer = *it;
    __declspec(align(8)) volatile __int64 aligned = 1;
    std::string sprName;
    std::string actName;
    (void)aligned;
    switch (layer) {
      case WNDCHAR_LAYER_BODY: {
        int job = this->m_flags == 1 ? g_session.GetRidingJob(this->m_job) : this->m_job;
        sprName = static_cast<const std::string&>(g_session.GetBodySprName(job, this->m_sex, this->m_body, this->m_shield));
        actName = static_cast<const std::string&>(g_session.GetBodyActionResourceName(job, this->m_sex, this->m_body, this->m_shield));
        this->bodyPaletteName = static_cast<const std::string&>(g_session.GetBodyPalName(job, this->m_sex, this->m_bodyStyle, this->m_body, this->m_shield, 0));
        break;
      }
      case WNDCHAR_LAYER_HEAD: {
        int head = this->m_head;
        sprName = static_cast<const std::string&>(g_session.GetHeadSprName(this->m_job, this->m_sex, &head));
        actName = static_cast<const std::string&>(g_session.GetHeadActName(this->m_job, this->m_sex, &head));
        this->m_head = static_cast<short>(head);
        this->headPaletteName = static_cast<const std::string&>(g_session.GetHeadPalName(this->m_head, this->m_job, this->m_sex, this->m_headPalette));
        break;
      }
      case WNDCHAR_LAYER_HEAD_MID:
      case WNDCHAR_LAYER_HEAD_TOP:
      case WNDCHAR_LAYER_HEAD_LOW:
      case WNDCHAR_LAYER_HELM_ROBE:
        sprName = static_cast<const std::string&>(g_session.GetAccessorySprName(this->m_job, this->m_sex, this->m_head, this->m_layers[layer]));
        actName = static_cast<const std::string&>(g_session.GetAccessoryActName(this->m_job, this->m_sex, this->m_head, this->m_layers[layer]));
        break;
      case WNDCHAR_LAYER_ROBE_BEHIND:
      case WNDCHAR_LAYER_ROBE_FRONT:
        if (this->m_shield == 0) {
          sprName = g_session.GetRobeSprName(this->m_job, this->m_hairColor, this->m_sex, this->m_robe, this->m_flags, this->m_body);
          actName = g_session.GetRobeActName(this->m_job, this->m_hairColor, this->m_sex, this->m_robe, this->m_flags, this->m_body);
        }
        break;
    }

    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName));
    CActRes* act = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName));
    if (sprite && act) {
      std::pair<CSprRes*, CActRes*>& entry = this->m_resources.try_emplace(layer).first->second;
      entry.second = act;
      entry.first = sprite;
    }
  }

  for (int layer = 3; layer <= 5; ++layer) {
    if (g_session.IsSpecialMonsterJob(this->m_layers[layer]) == true) {
      this->customRendering = 1;
      return;
    }
  }
}
