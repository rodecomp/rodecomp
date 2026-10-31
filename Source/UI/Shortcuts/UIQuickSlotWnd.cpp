#include "UI/Shortcuts/UIQuickSlotWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/UserData/CharacterConfigSave.h"
#include "UI/UIBasicCtrl.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <list>
#include <string>

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

// OFFSET: 0x00931920
// FUNCTION: fn_12709
void UIQuickSlotWnd::Slot28(int x, int y) {
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
  QuickSlotPainting& painting = *this->m_painting;
  const std::uint32_t id = this->m_entries[this->m_selected].id;
  if (id == 0) {
    text.clear();
  } else if (this->m_entries[this->m_selected].skill) {
    LuaScript& script = *g_session.m_hotkeyScript;
    CSkillInfo skill = painting.skills.Lookup(id);
    const int level = this->m_entries[this->m_selected].count;
    const int sp = skill.UseSpAmount(script, level);
    const int ap = skill.UseApAmount(script, level);
    if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
      text = SkillUseTextPercent(skill.DisplayName(script), level, sp, ap);
    } else {
      text = SkillUseTextFlat(skill.DisplayName(script), level, sp, ap);
    }
  } else {
    ITEM_INFO item = LookupClientItem(painting.inventory, static_cast<int>(id), 1);
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{painting.descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(183), naming.DisplayName(item, 0).c_str(), item.quantity));
  }

  if (!text.empty()) {
    reinterpret_cast<ModeTooltip*>(g_charViewMode)->Show(text.c_str(), screenX, screenY, -1, 0, 1);
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

// OFFSET: 0x009118C0
// FUNCTION: fn_12520
UIQuickSlotWnd::UIQuickSlotWnd(UIWindowContext& windows, UIFrameContext& frames, QuickSlotPainting& painting)
    : UIFrameWnd(windows, frames, 0), m_selected(-1), m_painting(&painting) {
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
  case 23: {
    const NormalItemSlot* saved = &g_session.unknown_06EB;
    for (int index = 0; index < 2; ++index) {
      const NormalItemSlot source = saved[index];
      QuickSlotEntry& entry = this->m_entries[index];
      entry.id = 0;
      entry.skill = 0;
      entry.count = 0;
      if (source.itemId == 0) {
        continue;
      }

      if (source.unknown_0000 == 0) {
        ITEM_INFO item = LookupClientItem(this->m_painting->inventory, static_cast<int>(source.itemId), 1);
        if (item.unknown_0000[1] != 0 && item.identified != 0) {
          entry.id = source.itemId;
        }
        continue;
      }

      ClientSkills& skills = this->m_painting->skills;
      CSkillInfo skill = skills.bundle.Lookup(source.itemId, skills.classMode, skills.HasEffect(1717), skills.HasEffect(1716));
      if (skill.available == 0) {
        std::list<ShortcutCooldownRecord>& records = this->m_painting->cooldowns.records;
        const auto record = std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& value) { return value.id == source.itemId; });
        if (record != records.end()) {
          const auto state = std::find_if(record->alternate.begin(), record->alternate.end(), [&](const CooldownState& value) { return value.slot == index; });
          if (state != record->alternate.end()) {
            record->alternate.erase(state);
          }
        }

        skill = skills.LookupFirst(source.itemId);
        if (skill.available == 0) {
          skill = skills.LookupOther(source.itemId);
        }
        if (skill.available == 0) {
          skill = skills.LookupAdditional(source.itemId);
        }
      }

      if (skill.available != 0 && skill.level != 0) {
        entry.id = skill.id;
        entry.skill = 1;
        if (reinterpret_cast<const CharacterConfigSave*>(&g_session)->UseLevel(static_cast<int>(skill.id)) == 0) {
          entry.count = static_cast<std::int16_t>(skill.level);
        } else {
          entry.count = source.unknown_0005;
        }
      }
    }
    this->Invalidate();
    return 0;
  }

  case 34:
    this->m_saved = reinterpret_cast<QuickSlotWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    this->Move(this->m_saved->x, this->m_saved->y);
    return 0;

  case 41: {
    const QuickSlotEntry entry = this->m_entries[argument1];
    if (entry.skill == 0) {
      if (entry.id == 0) {
        return 0;
      }

      ITEM_INFO item = LookupClientItem(this->m_painting->inventory, static_cast<int>(entry.id), 1);
      if (item.unknown_0000[1] != 0) {
        CMode* mode = g_modeMgr.GetGameMode();
        switch (item.unknown_0000[0]) {
        case 0:
        case 1:
        case 2:
          mode->SendMsg(0x1B, item.unknown_0000[1], 0, 0, 0);
          break;
        case 10:
        case 16:
        case 17:
        case 19:
          if (item.unknown_0000[3] == 0x8000) {
            mode->SendMsg(0x14, 0xF, 0, 0, 0);
            mode->SendMsg(0x57, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
          }
          break;
        case 18:
          if (*reinterpret_cast<int*>(&g_session.unknown_16F4[4]) >= 0) {
            bool languagePrefix = false;
            WORD language = g_serverLanguage->language;
            ItemNaming naming{this->m_painting->descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
            const std::string text = FormatOwnedString(LookupMessage(0x4CA), naming.DisplayName(item, 0).c_str());
            if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text.c_str(), 2, false, false, 0, "Message", 0x118, 0x78, 0) == 0xBB) {
              mode->SendMsg(0x1B, item.unknown_0000[1], 0, 0, 0);
            }
          }
          break;
        default:
          if (item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0 && item.identified != 0) {
            mode->SendMsg(0x13, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
          }
          break;
        }
      }
      return 0;
    }

    CSkillInfo skill = this->m_painting->skills.Lookup(entry.id);
    if (skill.level != 0 && skill.available != 0) {
      static_cast<CMode*>(g_charViewMode)->SendMsg(0x71, reinterpret_cast<int>(&skill), entry.count, 0, 0);
    }
    return 0;
  }
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
