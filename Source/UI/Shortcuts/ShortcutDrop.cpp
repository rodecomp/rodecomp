#include "Contents/Items/ItemType.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session/Session.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindow.h"
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

namespace {
  // INLINED
  __forceinline void Refresh(UIWindow& window) {
    window.SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    window.Invalidate();
  }

  // INLINED
  __forceinline void RemoveFirst(std::list<ShortcutCooldownRecord>& cooldowns, int slot) {
    for (ShortcutCooldownRecord& record : cooldowns) {
      for (std::list<CooldownState>::iterator state = record.main.begin(); state != record.main.end(); ++state) {
        if (state->slot == slot) {
          record.main.erase(state);
          return;
        }
      }
    }
  }

  // INLINED
  __forceinline std::list<ShortcutCooldownRecord>::iterator FindCooldownRecord(std::list<ShortcutCooldownRecord>& records, const std::uint32_t& id) {
    if (!g_session.IsHomunculusSkill(id) && !g_session.IsMercenarySkill(id)) {
      return std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& entry) { return entry.id == id; });
    }

    const std::uint32_t group = 9999;
    return std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& entry) { return entry.id == group; });
  }

  // INLINED
  __forceinline std::list<CooldownState>::iterator FindCooldownState(std::list<CooldownState>& states, const int& slot) {
    return std::find_if(states.begin(), states.end(), [&](const CooldownState& state) { return state.slot == slot; });
  }
}

// OFFSET: 0x00917640
// FUNCTION: fn_12589
void UIShortCutWnd::DragAndDrop(int x, int y, std::intptr_t argument) {
  UIWindow& window = *this;
  ShortcutGrid& grid = this->m_grid;
  const ModeDragState& value = *reinterpret_cast<const ModeDragState*>(argument);
  const int bank = static_cast<int>(g_session.m_options.Get(10));

  if (value.skill != 0) {
    const std::string copiedName = value.skillName;
    const std::uint32_t id = value.id;
    const int column = this->HitTestSlot(x, y);

    if (column != -1) {
      if (9 * g_session.m_hotkeyPage + column == value.slot) {
        g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return;
      }

      CSkillInfo skill = g_session.LookupSkill(id);
      const int index = 9 * g_session.m_hotkeyPage + column;
      if (skill.level == 0 && skill.available == 0) {
        g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        return;
      }

      if (grid.rows <= g_session.m_hotkeyPage) {
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        }

        return;
      }

      g_session.AssignShortcut(1, skill.id, value.skillLevel, index, bank);

      if (value.slot != index) {
        RemoveFirst(g_session.m_requestTimers, index);
      }

      const auto record = FindCooldownRecord(g_session.m_requestTimers, skill.id);
      if (record != g_session.m_requestTimers.end() && record->end > timeGetTime()) {
        const auto previous = FindCooldownState(record->main, value.slot);
        const int targetX = 29 * column + 16;
        const int targetY = 33 * g_session.m_hotkeyPage + 4;

        if (previous != record->main.end()) {
          if (previous->y == targetY) {
            previous->slot = index;
            previous->x = targetX;
          } else {
            record->main.push_back(CooldownState(index, targetX, targetY, record->duration, record->end - record->duration, 0, 0));
          }
        } else {
          record->main.push_back(CooldownState(index, targetX, targetY, record->duration, record->end - record->duration, 0, 0));
        }
      }

      Refresh(window);
    } else if (value.slot != -1) {
      CSkillInfo skill = g_session.LookupSkill(id);
      const auto record = FindCooldownRecord(g_session.m_requestTimers, skill.id);

      if (record != g_session.m_requestTimers.end()) {
        const auto previous = FindCooldownState(record->main, value.slot);
        if (previous != record->main.end()) {
          record->main.erase(previous);
        }
      }

      g_session.AssignShortcut(1, 0, 0, value.slot, bank);
      Refresh(window);
    }

    g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (value.source == 6 || value.source == 9) {
    g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, value.id);
  if (item.identity[1] == 0 || item.identified == 0) {
    g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  switch (item.identity[0]) {
  case TYPE_EVENT:
  case TYPE_ARROW:
  case TYPE_AMMO:
  case TYPE_THROWWEAPON:
  case 19: {
    const int column = this->HitTestSlot(x, y);
    if (column != -1 && 9 * grid.rows <= 9 * g_session.m_hotkeyPage + column) {
      if (g_session.ItemSlotActive()) {
        g_session.AssignItemShortcut(static_cast<std::uint32_t>(item.GetItemId()), column);
        Refresh(window);
      }

      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }

      return;
    }
  }
  }

  switch (item.identity[0]) {
  case TYPE_ARMOR:
  case TYPE_WEAPON:
  case TYPE_BOW:
  case TYPE_BOTHHAND:
  case TYPE_ARMORTM:
  case TYPE_ARMORTB:
  case TYPE_ARMORMB:
  case TYPE_ARMORTMB:
  case TYPE_GUN:
    if (item.identity[2] == 0) {
      break;
    }
  case TYPE_HEAL:
  case TYPE_SCHANGE:
  case TYPE_SPECIAL:
  case TYPE_ARROW:
  case TYPE_AMMO:
  case TYPE_THROWWEAPON:
  case 19: {
    const int column = this->HitTestSlot(x, y);
    if (column != -1) {
      const int index = 9 * g_session.m_hotkeyPage + column;
      if (grid.rows <= g_session.m_hotkeyPage) {
        if (g_modeMgr.gameMode) {
          g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        }

        return;
      }

      g_session.AssignShortcut(0, static_cast<std::uint32_t>(item.GetItemId()), 0, index, bank);
      if (value.slot != index) {
        RemoveFirst(g_session.m_requestTimers, index);
      }

      Refresh(window);
    }

    break;
  }
  }

  g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

namespace {
  // INLINED
  __forceinline void RemoveAlternate(std::list<ShortcutCooldownRecord>& cooldowns, int source, int slot) {
    for (ShortcutCooldownRecord& record : cooldowns) {
      if (source == slot) {
        continue;
      }

      for (std::list<CooldownState>::iterator state = record.alternate.begin(); state != record.alternate.end(); ++state) {
        if (state->slot == slot) {
          record.alternate.erase(state);
          return;
        }
      }
    }
  }
}

// OFFSET: 0x00917100
// FUNCTION: fn_12588
void UIQuickSlotWnd::DragAndDrop(int x, int y, std::intptr_t argument) {
  const ModeDragState& value = *reinterpret_cast<const ModeDragState*>(argument);
  std::list<ShortcutCooldownRecord>& records = g_session.m_requestTimers;

  if (value.skill != 0) {
    const std::string copiedName = value.skillName;
    const int slot = this->SlotAt(x, y);
    if (slot != -1) {
      CSkillInfo skill = g_session.LookupSkill(value.id);
      if (skill.level != 0 && skill.available != 0) {
        g_session.SetHotkeyEntry(1, skill.id, value.skillLevel, slot);
        RemoveAlternate(g_session.m_requestTimers, value.slot, slot);

        const std::list<ShortcutCooldownRecord>::iterator record = FindCooldownRecord(records, skill.id);
        if (record != records.end() && timeGetTime() < record->end) {
          std::list<CooldownState>& states = record->alternate;
          const std::list<CooldownState>::iterator previous = FindCooldownState(states, value.slot);
          if (previous != states.end()) {
            previous->slot = slot;
            previous->x = this->SlotX(slot);
          } else {
            for (std::list<CooldownState>::iterator state = states.begin(); state != states.end(); ++state) {
              if (state->x == this->SlotX(slot)) {
                states.erase(state);
                break;
              }
            }
          }
        }

        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        this->Invalidate();
      }
    } else if (value.slot != -1) {
      CSkillInfo skill = g_session.LookupSkill(value.id);
      const std::list<ShortcutCooldownRecord>::iterator record = FindCooldownRecord(records, skill.id);
      if (record != records.end()) {
        std::list<CooldownState>& states = record->alternate;
        const std::list<CooldownState>::iterator previous = FindCooldownState(states, value.slot);
        if (previous != states.end()) {
          states.erase(previous);
        }
      }

      g_session.SetHotkeyEntry(1, 0, 0, value.slot);
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      this->Invalidate();
    }
  } else if (value.source != 6 && value.source != 9) {
    ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, value.id);
    if (item.identity[1] != 0 && item.identified != 0) {
      switch (item.identity[0]) {
      case TYPE_ARMOR:
      case TYPE_WEAPON:
      case TYPE_BOW:
      case TYPE_BOTHHAND:
      case TYPE_ARMORTM:
      case TYPE_ARMORTB:
      case TYPE_ARMORMB:
      case TYPE_ARMORTMB:
      case TYPE_GUN:
        if (item.identity[2] == 0) {
          break;
        }
      case TYPE_HEAL:
      case TYPE_SCHANGE:
      case TYPE_SPECIAL:
      case TYPE_ARROW:
      case TYPE_AMMO:
      case TYPE_THROWWEAPON:
      case 19: {
        const int slot = this->SlotAt(x, y);
        if (slot != -1) {
          g_session.SetHotkeyEntry(0, item.GetItemId(), 0, slot);
          RemoveAlternate(g_session.m_requestTimers, value.slot, slot);
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          this->Invalidate();
        }

        break;
      }
      }
    }
  }

  g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

