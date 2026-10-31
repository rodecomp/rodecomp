#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"
#include "Contents/SpriteRidingOffset.h"
#include "Contents/SpriteRidingPolicy.h"
#include "Resource/Sprites/ActionResource.h"

// OFFSET: 0x007D4840
// FUNCTION: fn_09582
FuncDrawWNDChar::~FuncDrawWNDChar() = default;

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
