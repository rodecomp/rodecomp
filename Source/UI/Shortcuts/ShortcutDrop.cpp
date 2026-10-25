#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "UI/Shortcuts/ShortcutDrop.h"
#include "UI/Window/UIWindow.h"

#include <algorithm>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

namespace {
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

  ItemInfo item = LookupClientItemByIdentity(this->inventory, value.id);
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
        this->bindings.AssignItem(static_cast<std::uint32_t>(std::atoi(item.itemId.c_str())), column);
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
        this->bindings.Assign(0, static_cast<std::uint32_t>(std::atoi(item.itemId.c_str())), 0, index, bank, this->currentRow, this->script, *this->mode);
        if (value.slot != index) {
          RemoveFirst(this->bindings.cooldowns, index);
        }

        Refresh(window);
      }
    }
  }

  this->mode->SendModeMessage(18, 0, 0, 0, 0);
}
