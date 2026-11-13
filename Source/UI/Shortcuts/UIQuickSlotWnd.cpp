#include "UI/Window/WindowId.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Input/InputCharset.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "UI/Window/UIBasicCtrl.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <list>
#include <string>

// OFFSET: 0x00930EB0
// FUNCTION: fn_12696
void UIQuickSlotWnd::OnLBtnDown(int x, int y) {
  if (x > 59 && x < this->geometry.width) {
    this->UIFrameWnd::OnLBtnDown(x, y);
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

  const QuickSlotEntry entry = this->m_entries[index];
  if (entry.skill) {
    CSkillInfo skill = g_session.LookupSkill(entry.id);
    if (skill.available && skill.level) {
      if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
        mode->drag.BeginSkill(g_windowMgr, ResourceMgr::Instance(), 16, skill.id, skill.name, entry.count, index);
      }
    }
  } else if (entry.id != 0) {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(entry.id), 1);
    if (item.identity[1] > 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->drag.BeginItem(g_windowMgr, 16, item, item.quantity, -1);
    }
  }
}

// OFFSET: 0x00931920
// FUNCTION: fn_12709
void UIQuickSlotWnd::OnMouseHover(int x, int y) {
  int index;
  if (static_cast<unsigned int>(y) <= 23 && (x <= 58 || x >= this->geometry.width)) {
    index = (x - 4) / 29;
    if (x > index * 29 + 30) {
      index = -1;
    }
  } else {
    index = -1;
  }

  if (index != this->m_selected) {
    this->m_selected = index;
    this->Invalidate();
  }

  if (this->m_selected == -1) {
    return;
  }

  int screenY = -17;
  int screenX = (x - 4) / 29 * 29 + 4;
  this->ToScreen(screenX, screenY);
  std::string text;
  const std::uint32_t id = this->m_entries[this->m_selected].id;
  if (id == 0) {
    text.clear();
  } else if (this->m_entries[this->m_selected].skill) {
    LuaScript& script = *g_session.m_hotkeyScript;
    CSkillInfo skill = g_session.LookupSkill(id);
    const int level = this->m_entries[this->m_selected].count;
    const int sp = skill.UseSpAmount(script, level);
    const int ap = skill.UseApAmount(script, level);
    if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
      text = SkillUseTextPercent(skill.DisplayName(), level, sp, ap);
    } else {
      text = SkillUseTextFlat(skill.DisplayName(), level, sp, ap);
    }
  } else {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(id), 1);
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity));
  }

  if (!text.empty()) {
    g_modeMgr.gameMode->ShowTooltip(text.c_str(), screenX, screenY, -1, 0, 1);
  }
}

// OFFSET: 0x00932F80
// FUNCTION: fn_12722
void UIQuickSlotWnd::OnRBtnDown(int x, int y) {
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

  const std::uint32_t id = this->m_entries[index].id;
  if (this->m_entries[index].skill) {
    WindowManagerWindows& windows = g_windowMgr;
    UIWindow* detail = windows.EnsureWindow(WID_SKILLDESCRIBEWND);
    if (detail) {
      if (*reinterpret_cast<std::uint32_t*>(reinterpret_cast<char*>(detail) + 0x104) == id) {
        windows.CloseWindow(WID_SKILLDESCRIBEWND);
      } else {
        detail->SendMsg(nullptr, UM_SKILLDESCRIBE, id, 0, 0, 0);
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
  } else if (id != 0) {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(id), 1);
    if (item.identity[1] > 0) {
      UIWindow* detail = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      detail->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x009118C0
// FUNCTION: fn_12520
UIQuickSlotWnd::UIQuickSlotWnd()
    : UIFrameWnd(0), m_selected(-1) {
  for (int i = 0; i < 2; ++i) {
    this->m_entries[i].skill = 0;
    this->m_entries[i].id = 0;
    this->m_entries[i].count = 0;
  }
}

// OFFSET: 0x00938850
// FUNCTION: fn_12752
int UIQuickSlotWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_REFRESHITEM: {
    const NormalItemSlot* saved = &g_session.quickSlot;
    for (int index = 0; index < 2; ++index) {
      const NormalItemSlot source = saved[index];
      QuickSlotEntry& entry = this->m_entries[index];
      entry.id = 0;
      entry.skill = 0;
      entry.count = 0;
      if (source.itemId == 0) {
        continue;
      }

      if (source.enabled != 0) {
        CSkillInfo skill = g_session.m_skillBundle.Lookup(source.itemId);
        if (skill.available == 0) {
          std::list<ShortcutCooldownRecord>& records = g_session.m_requestTimers;
          const auto record = std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& value) { return value.id == source.itemId; });
          if (record != records.end()) {
            const auto state = std::find_if(record->alternate.begin(), record->alternate.end(), [&](const CooldownState& value) { return value.slot == index; });
            if (state != record->alternate.end()) {
              record->alternate.erase(state);
            }
          }
        }
        if (skill.available == 0) {
          skill = g_session.LookupHomunSkill(source.itemId);
        }
        if (skill.available == 0) {
          skill = g_session.LookupMercSkill(source.itemId);
        }
        if (skill.available == 0) {
          skill = g_session.LookupAdditionalSkill(source.itemId);
        }

        if (skill.available != 0 && skill.level != 0) {
          entry.id = skill.id;
          entry.skill = 1;
          if (g_session.UseLevel(static_cast<int>(skill.id)) == 0) {
            entry.count = static_cast<std::int16_t>(skill.level);
          } else {
            entry.count = source.value;
          }
        }
      } else {
        ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(source.itemId), 1);
        if (item.identity[1] != 0 && item.identified != 0) {
          entry.id = source.itemId;
        }
      }
    }
    this->Invalidate();
    return 0;
  }

  case UM_SETINFO:
    this->m_saved = reinterpret_cast<QuickSlotWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;

  case UM_USE_SHORTEN_ITEM: {
    const QuickSlotEntry entry = this->m_entries[argument1];
    if (entry.skill == 0) {
      if (entry.id == 0) {
        return 0;
      }

      ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(entry.id), 1);
      if (item.identity[1] != 0) {
        CMode* mode = g_modeMgr.GetGameMode();
        switch (item.identity[0]) {
        case UM_DEFPUSH:
        case UM_CANCELPUSH:
        case UM_TOGGLEPUSH:
          mode->SendMsg(MM_USEITEM, item.identity[1], 0, 0, 0);
          break;
        case UM_UPPUSH:
          if (g_session.m_manner >= 0) {
            if (g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_CONFIRM_TO_OPEN_CASH_ITEM), item.DisplayName(0).c_str()).c_str(), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
              mode->SendMsg(MM_USEITEM, item.identity[1], 0, 0, 0);
            }
          }
          break;
        case UM_SCROLLVERTPAGEDOWN:
        case UM_OPENCHATWND:
        case UM_LOADINGPERCENT:
        case UM_DOWNPUSH:
          if (item.identity[3] == 0x8000) {
            mode->SendMsg(MM_TAKE_OFF_EQUIP, 0xF, 0, 0, 0);
            mode->SendMsg(MM_WEAR_EQUIP_ARROW, item.identity[1], item.identity[2], 0, 0);
          }
          break;
        default:
          if (item.identity[2] != 0 && item.identity[3] == 0 && item.identified != 0) {
            mode->SendMsg(MM_WEAR_EQUIP, item.identity[1], item.identity[2], 0, 0);
          }
          break;
        }
      }
      return 0;
    }

    CSkillInfo skill = g_session.LookupSkill(entry.id);
    if (skill.level != 0 && skill.available != 0) {
      g_modeMgr.gameMode->SendMsg(MM_USING_SKILL, reinterpret_cast<int>(&skill), entry.count, 0, 0);
    }
    return 0;
  }
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
