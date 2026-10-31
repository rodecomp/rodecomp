#include "UI/Shortcuts/UIQuickSlotWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x00930EB0
// FUNCTION: fn_12696
void UIQuickSlotWnd::Slot25(int x, int y) {
  if (x > 59 && x < this->geometry.width) {
    this->UIFrameWnd::Slot25(x, y);
    return;
  }

  if (g_replayAnimationScaling != 0 || static_cast<unsigned int>(y) > 23) {
    return;
  }

  if (x > 58 && x < this->geometry.width) {
    return;
  }

  const int index = (x - 4) / 29;
  if (x > index * 29 + 30 || index == -1) {
    return;
  }

  QuickSlotPainting& painting = *this->m_painting;
  const QuickSlotEntry entry = this->m_entries[index];
  if (entry.skill) {
    CSkillInfo skill = painting.skills.Lookup(entry.id);
    if (skill.available && skill.level) {
      if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
        mode->drag.BeginSkill(this->m_frameContext->windows, ResourceMgr::Instance(), 16, skill.id, skill.name, entry.count, index);
      }
    }
  } else if (entry.id != 0) {
    ITEM_INFO item = LookupClientItem(painting.inventory, static_cast<int>(entry.id), 1);
    if (item.unknown_0000[1] > 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->drag.BeginItem(this->m_frameContext->windows, painting.descriptions, 16, item, item.quantity, -1);
    }
  }
}

// OFFSET: 0x00932F80
// FUNCTION: fn_12722
void UIQuickSlotWnd::Slot32(int x, int y) {
  if (x > 59 && x < this->geometry.width) {
    return;
  }

  if (static_cast<unsigned int>(y) > 23) {
    return;
  }

  if (x > 58 && x < this->geometry.width) {
    return;
  }

  const int index = (x - 4) / 29;
  if (x > index * 29 + 30 || index == -1) {
    return;
  }

  const QuickSlotEntry entry = this->m_entries[index];
  if (entry.skill) {
    WindowManagerWindows& windows = this->m_frameContext->windows;
    UIWindow* detail = windows.EnsureWindow(46);
    if (detail) {
      if (*reinterpret_cast<std::uint32_t*>(reinterpret_cast<char*>(detail) + 0x104) == entry.id) {
        windows.CloseWindow(46);
      } else {
        detail->SendMsg(nullptr, 61, entry.id, 0, 0, 0);
      }
    }

    int originX = 0;
    int originY = 0;
    this->ToScreen(originX, originY);
    const int targetX = originX + x;
    int adjustment = 0;
    if (static_cast<unsigned int>(g_configuredScreenWidth - targetX) < static_cast<unsigned int>(detail->geometry.width)) {
      adjustment = detail->geometry.width - g_configuredScreenWidth + targetX;
    }

    const int rowY = 33 * (y / 33);
    if (static_cast<float>(originY) > static_cast<float>(static_cast<unsigned int>(g_configuredScreenHeight)) * g_depthAverageHalf) {
      detail->Move(originX - adjustment + x, rowY - detail->geometry.height + originY);
    } else {
      detail->Move(originX - adjustment + x, originY + 33 + rowY);
    }
  } else if (entry.id != 0) {
    ITEM_INFO item = LookupClientItem(this->m_painting->inventory, static_cast<int>(entry.id), 1);
    if (item.unknown_0000[1] > 0) {
      UIWindow* detail = this->m_frameContext->windows.EnsureWindow(12);
      detail->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}
