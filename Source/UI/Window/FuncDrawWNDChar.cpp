#include "UI/Window/FuncDrawWNDChar.h"
#include "Client/Session.h"
#include "Script/LuaScript.h"

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
