#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Client/Session.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Shortcuts/ShortcutDrop.h"
#include "UI/Window/UIWindow.h"
#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

namespace {
  // INLINED
  bool Grouped(std::uint32_t id) {
    return id - 8000u <= 60 || id - 8200u <= 41 || id - 8400u <= 57;
  }

  // INLINED
  void Refresh(UIWindow& window) {
    window.SendMsg(nullptr, 23, 0, 0, 0, 0);
    window.Invalidate();
  }

  // INLINED
  void RemoveFirst(ShortcutCooldowns& cooldowns, int slot) {
    for (ShortcutCooldownRecord& record : cooldowns.records) {
      const auto value = std::find_if(record.main.begin(), record.main.end(), [&](const CooldownState& state) { return state.slot == slot; });
      if (value != record.main.end()) {
        record.main.erase(value);
        return;
      }
    }
  }
}

// OFFSET: 0x00917640
// FUNCTION: fn_12589
void ShortcutDrop::Drop(UIWindow& window, ShortcutGrid& grid, int x, int y, const ModeDragState& value) {
  const int bank = this->bindings.mode;

  if (value.skill != 0) {
    const std::string copiedName = value.skillName;
    const std::uint32_t id = value.id;
    const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);
    const int index = 9 * this->currentRow + column;
    const int originalSlot = value.slot;

    if (column != -1) {
      if (index == originalSlot) {
        this->mode->SendModeMessage(18, 0, 0, 0, 0);
        return;
      }

      CSkillInfo skill = this->bindings.skills.Lookup(id);
      if ((skill.level == 0 && skill.available == 0) || grid.rows <= this->currentRow) {
        if (this->mode) {
          this->mode->SendModeMessage(18, 0, 0, 0, 0);
        }

        return;
      }

      this->bindings.Assign(1, skill.id, value.skillLevel, index, bank, this->currentRow, this->script, *this->mode);

      if (originalSlot != index) {
        RemoveFirst(this->bindings.cooldowns, index);
      }

      const std::uint32_t key = Grouped(skill.id) ? 9999 : skill.id;

      const auto record =
        std::find_if(this->bindings.cooldowns.records.begin(), this->bindings.cooldowns.records.end(), [&](const ShortcutCooldownRecord& entry) {
          return entry.id == key;
        });

      if (record != this->bindings.cooldowns.records.end() && record->end > timeGetTime()) {
        const auto previous = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == originalSlot; });
        const int targetX = 29 * column + 16;
        const int targetY = 33 * this->currentRow + 4;

        if (previous != record->main.end() && previous->y == targetY) {
          previous->slot = index;
          previous->x = targetX;
        } else {
          record->main.emplace_back(index, targetX, targetY, record->duration, record->end - record->duration, 0, 0);
        }
      }

      Refresh(window);
    } else if (originalSlot != -1) {
      CSkillInfo skill = this->bindings.skills.Lookup(id);
      const std::uint32_t key = Grouped(skill.id) ? 9999 : skill.id;
      const auto record =
        std::find_if(this->bindings.cooldowns.records.begin(), this->bindings.cooldowns.records.end(), [&](const ShortcutCooldownRecord& entry) {
          return entry.id == key;
        });

      if (record != this->bindings.cooldowns.records.end()) {
        const auto previous = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == originalSlot; });
        if (previous != record->main.end()) {
          record->main.erase(previous);
        }
      }

      this->bindings.Assign(1, 0, 0, originalSlot, bank, this->currentRow, this->script, *this->mode);
      Refresh(window);
    }

    this->mode->SendModeMessage(18, 0, 0, 0, 0);
    return;
  }

  if (value.source == 6 || value.source == 9) {
    this->mode->SendModeMessage(18, 0, 0, 0, 0);
    return;
  }

  ITEM_INFO item = LookupClientItemByIdentity(this->inventory, value.id);
  if (item.unknown_0000[1] == 0 || item.identified == 0) {
    this->mode->SendModeMessage(18, 0, 0, 0, 0);
    return;
  }

  const unsigned type = item.unknown_0000[0];
  bool consumed = false;

  if (type == 3 || type == 10 || type == 16 || type == 17 || type == 19) {
    const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);
    if (column != -1 && 9 * grid.rows <= 9 * this->currentRow + column) {
      if (this->itemSlotActive) {
        this->bindings.AssignItem(static_cast<std::uint32_t>(item.GetItemId()), column);
        Refresh(window);
      }

      consumed = true;
    }
  }

  if (!consumed) {
    bool allowed = type <= 2 || type == 10 || type == 16 || type == 17 || type == 19;
    if (type == 4 || type == 5 || type == 8 || type == 9 || (type >= 11 && type <= 15)) {
      allowed = item.unknown_0000[2] != 0;
    }

    if (allowed) {
      const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);
      if (column != -1 && grid.rows > this->currentRow) {
        const int index = 9 * this->currentRow + column;
        this->bindings.Assign(0, static_cast<std::uint32_t>(item.GetItemId()), 0, index, bank, this->currentRow, this->script, *this->mode);
        if (value.slot != index) {
          RemoveFirst(this->bindings.cooldowns, index);
        }

        Refresh(window);
      }
    }
  }

  this->mode->SendModeMessage(18, 0, 0, 0, 0);
}

namespace {
  // INLINED
  void RemoveAlternate(ShortcutCooldowns& cooldowns, int source, int slot) {
    for (ShortcutCooldownRecord& record : cooldowns.records) {
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
void UIQuickSlotWnd::Slot10(int x, int y, std::intptr_t argument) {
  QuickSlotPainting& painting = *this->m_painting;
  const ModeDragState& value = *reinterpret_cast<const ModeDragState*>(argument);
  std::list<ShortcutCooldownRecord>& records = painting.cooldowns.records;

  if (value.skill != 0) {
    const std::string copiedName(value.skillName.c_str(), value.skillName.size());
    const int slot = this->SlotAt(x, y);
    if (slot != -1) {
      CSkillInfo skill = painting.skills.Lookup(value.id);
      if (skill.level != 0 && skill.available != 0) {
        g_session.SetHotkeyEntry(1, skill.id, value.skillLevel, slot);
        RemoveAlternate(painting.cooldowns, value.slot, slot);

        const std::uint32_t key = Grouped(skill.id) ? 9999 : skill.id;
        const std::list<ShortcutCooldownRecord>::iterator record =
          std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& entry) { return entry.id == key; });
        if (record != records.end() && timeGetTime() < record->end) {
          std::list<CooldownState>& states = record->alternate;
          const std::list<CooldownState>::iterator previous =
            std::find_if(states.begin(), states.end(), [&](const CooldownState& state) { return state.slot == value.slot; });
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

        this->SendMsg(nullptr, 23, 0, 0, 0, 0);
        this->Invalidate();
      }
    } else if (value.slot != -1) {
      CSkillInfo skill = painting.skills.Lookup(value.id);
      const std::uint32_t key = Grouped(skill.id) ? 9999 : skill.id;
      const std::list<ShortcutCooldownRecord>::iterator record =
        std::find_if(records.begin(), records.end(), [&](const ShortcutCooldownRecord& entry) { return entry.id == key; });
      if (record != records.end()) {
        std::list<CooldownState>& states = record->alternate;
        const std::list<CooldownState>::iterator previous =
          std::find_if(states.begin(), states.end(), [&](const CooldownState& state) { return state.slot == value.slot; });
        if (previous != states.end()) {
          states.erase(previous);
        }
      }

      g_session.SetHotkeyEntry(1, 0, 0, value.slot);
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
      this->Invalidate();
    }
  } else if (value.source != 6 && value.source != 9) {
    ITEM_INFO item = LookupClientItemByIdentity(painting.inventory, value.id);
    if (item.unknown_0000[1] != 0 && item.identified != 0) {
      switch (item.unknown_0000[0]) {
      case 4:
      case 5:
      case 8:
      case 9:
      case 11:
      case 12:
      case 13:
      case 14:
      case 15:
        if (item.unknown_0000[2] == 0) {
          break;
        }
      case 0:
      case 1:
      case 2:
      case 10:
      case 16:
      case 17:
      case 19: {
        const int slot = this->SlotAt(x, y);
        if (slot != -1) {
          g_session.SetHotkeyEntry(0, item.GetItemId(), 0, slot);
          RemoveAlternate(painting.cooldowns, value.slot, slot);
          this->SendMsg(nullptr, 23, 0, 0, 0, 0);
          this->Invalidate();
        }

        break;
      }
      }
    }
  }

  this->m_context->mode->SendModeMessage(18, 0, 0, 0, 0);
}

