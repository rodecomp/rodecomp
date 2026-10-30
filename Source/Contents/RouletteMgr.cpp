#include "Contents/RouletteMgr.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "UI/Window/UIRoulletteWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <new>
#include <utility>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24688
RouletteMgr* g_rouletteMgr = nullptr;

// OFFSET: 0x0080A310
// FUNCTION: fn_10070
RouletteMgr::RouletteMgr() {
  this->m_valid = 0;
  std::memset(&this->m_quads.current, 0, sizeof(this->m_quads.current));
  std::memset(&this->m_quads.start, 0, sizeof(this->m_quads.start));
  std::memset(&this->m_quads.target, 0, sizeof(this->m_quads.target));
  this->unknown_0004 = 0;
  this->m_frame = 0;
  this->m_cycle = 0;
  this->m_spinning = 0;
  this->m_stage = 0;
  this->unknown_0018 = 0;
  this->m_lastTick = 0;
  this->m_interval = 0;
  this->m_index = 0;
  this->m_additionItemId = 0;
  this->m_goldPoint = 0;
  this->m_silverPoint = 0;
  this->m_bronzePoint = 0;
  this->m_resultShown = 0;
  this->m_bonus = 0;
  this->m_resultItem.Reset();
  this->m_additionItem.Reset();
  this->m_valid = 1;
}

// OFFSET: 0x0080A980
// FUNCTION: fn_10072
void RouletteMgr::AddItem(RouletteSlot slot) {
  ITEM_INFO item;
  item.SetItemId(slot.itemId);
  item.quantity = slot.count;
  item.identified = 1;
  this->m_items.insert(std::make_pair(slot, item));
}

// OFFSET: 0x0080ABA0
// FUNCTION: fn_10073
void RouletteMgr::Reset() {
  std::memset(&this->m_quads, 0, sizeof(this->m_quads));
  this->m_frame = 0;
  this->m_cycle = 0;
  this->m_stage = 0;
  this->unknown_0018 = 0;
  this->m_lastTick = 0;
  this->m_interval = 0;
  this->m_spinning = 0;
}

// OFFSET: 0x0080ABF0
// FUNCTION: fn_10074
bool RouletteMgr::Create() {
  if (g_rouletteMgr == nullptr) {
    g_rouletteMgr = new (std::nothrow) RouletteMgr;
    if (g_rouletteMgr != nullptr) {
      if (g_rouletteMgr->m_valid != 0) {
        return true;
      }

      delete g_rouletteMgr;
      g_rouletteMgr = nullptr;
    }
  }

  return false;
}

// OFFSET: 0x0080ACA0
// FUNCTION: fn_10075
void RouletteMgr::Destroy() {
  if (g_rouletteMgr != nullptr) {
    delete g_rouletteMgr;
    g_rouletteMgr = nullptr;
  }
}

// OFFSET: 0x0080ACC0
// FUNCTION: fn_10076
void RouletteMgr::Draw() {
  RenderCommand* command = g_renderer->commands.Acquire();
  command->texture = g_texMgr.Default();
  command->vertices = this->m_quads.current.vertices;
  command->vertexCount = 6;
  g_renderer->queues.Submit(command, 0x201, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);

  this->Update();
}

// OFFSET: 0x0080B1D0
// FUNCTION: fn_10078
void RouletteMgr::SetAdditionItem(int itemId) {
  if (itemId == -1) {
    this->m_additionItem.Reset();
    return;
  }

  this->m_additionItemId = itemId;
  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    if (itemId == it->first.itemId) {
      this->m_additionItem = it->second;
      return;
    }
  }
}

// OFFSET: 0x0080B270
// FUNCTION: fn_10079
void RouletteMgr::StartSpin(int x, int y, int originX, int originY) {
  this->SetQuad(x, y, originX, originY);
  this->m_quads.start = this->m_quads.current;

  const float distance = (8 - this->m_stage) * 36.0f;
  for (int i = 0; i < 6; ++i) {
    this->m_quads.target.vertices[i] = this->m_quads.current.vertices[i];
    this->m_quads.target.vertices[i].x = this->m_quads.current.vertices[i].x - distance;
  }

  this->m_lastTick = timeGetTime();
  this->m_interval = 35;
  this->m_frame = 0;
  this->m_cycle = 0;
  this->m_spinning = 1;
  this->m_resultShown = 0;

  g_windowMgr.unknown_0464->m_enabled = 0;
  if (g_windowMgr.unknown_0464) {
    g_windowMgr.unknown_0464->SendMsg(nullptr, 60, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0080B490
// FUNCTION: fn_10080
void RouletteMgr::SetQuad(int x, int y, int originX, int originY) {
  if (g_renderer->viewport.width == 800 && g_renderer->viewport.height == 600) {
    y += 3;
    originY += 3;
  }

  const float left = x - 18.0f + originX;
  const float right = x + 18.0f + originX;
  const float top = y - 18.0f + originY + 17.0f;
  const float bottom = y + 18.0f + originY + 17.0f;

  RenderVertex* vertices = this->m_quads.current.vertices;
  vertices[0].x = left;
  vertices[0].y = bottom;
  vertices[1].x = right;
  vertices[1].y = bottom;
  vertices[2].x = right;
  vertices[2].y = top;
  vertices[3] = vertices[0];
  vertices[4] = vertices[2];
  vertices[5].x = left;
  vertices[5].y = top;

  for (int i = 0; i < 6; ++i) {
    vertices[i].z = 0.000001f;
    vertices[i].rhw = 0.999999f;
    vertices[i].u = 0.0f;
    vertices[i].v = 0.0f;
    vertices[i].diffuse = 0x44FFFFFF;
  }
}

// OFFSET: 0x0080B680
// FUNCTION: fn_10081
void RouletteMgr::SetResult(unsigned short index, unsigned char shown, unsigned char bonus) {
  this->m_index = index;
  this->m_resultShown = shown;
  this->m_bonus = bonus;

  for (auto it = this->m_items.begin(); it != this->m_items.end(); ++it) {
    if (this->m_stage == it->first.row && this->m_index == it->first.column) {
      this->m_resultItem = it->second;
    }
  }
}

// OFFSET: 0x0080ADC0
// FUNCTION: fn_10077
void RouletteMgr::Update() {
  if (this->m_spinning != 1) {
    return;
  }

  if (this->m_cycle > 6) {
    if (this->m_frame >= 8 - this->m_stage - this->m_index) {
      this->m_resultShown = 1;
      const int originY = static_cast<int>(g_renderer->viewport.height * 0.5 - 240.0);
      const int originX = static_cast<int>(g_renderer->viewport.width * 0.5 - 320.0);
      this->SetQuad((this->m_stage + this->m_index) * 36 + 173, 341 - this->m_stage * 40, originX, originY);
      g_windowMgr.unknown_0464->m_enabled = 1;
      g_windowMgr.unknown_0464->Invalidate();
      this->m_spinning = 0;
      return;
    }

    if (timeGetTime() - this->m_lastTick < this->m_interval) {
      return;
    }
  } else {
    if (timeGetTime() - this->m_lastTick < this->m_interval) {
      return;
    }

    if (this->m_frame >= 8 - this->m_stage) {
      this->m_quads.current = this->m_quads.start;
      const float interval = static_cast<float>(this->m_interval);
      this->m_interval = static_cast<unsigned long>(interval * 0.34f + interval);
      ++this->m_cycle;
      this->m_frame = 0;
      return;
    }
  }

  for (int index = 0; index < 6; ++index) {
    if (this->m_quads.current.vertices[index].x > this->m_quads.target.vertices[index].x) {
      this->m_quads.current.vertices[index].x -= 36.0f;
    }
  }

  const DWORD now = timeGetTime();
  ++this->m_frame;
  this->m_lastTick = now;
}
