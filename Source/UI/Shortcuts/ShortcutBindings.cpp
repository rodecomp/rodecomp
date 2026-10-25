#include "Client/Mode/ModeEvents.h"
#include "Script/LuaScript.h"
#include "UI/Shortcuts/ShortcutBindings.h"

#include <windows.h>
#include <timeapi.h>

#include <algorithm>

// OFFSET: 0x00E12FC0
// FUNCTION: fn_23821
int ShortcutBindings::Rotate(int index, int bank) const {
  if (bank != 0 && bank != 1) {
    return index;
  }

  const int rotation = bank == 0 ? this->rotation0 : this->rotation1;
  if (rotation == 0) {
    return index;
  }

  const int value = index + 9 * rotation;
  return value < 36 ? value : value - 36;
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
void ShortcutBindings::Refresh(NormalItemSlot** slots, NormalItemSlot** itemSlots) {
  for (int index = 0; index < 36; ++index) {
    const int source = this->Rotate(index, this->mode);
    if (this->mode == 0 || this->mode == 1) {
      slots[index] = &this->banks[this->mode][source];
    }

    NormalItemSlot& slot = *slots[index];
    if (slot.itemId == 0 || slot.unknown_0000 == 0) {
      continue;
    }

    CSkillInfo skill = this->skills.bundle.Lookup(slot.itemId, this->skills.classMode, this->skills.HasEffect(1717), this->skills.HasEffect(1716));
    if (skill.available == 0) {
      const auto record = std::find_if(this->cooldowns.records.begin(), this->cooldowns.records.end(), [&](const ShortcutCooldownRecord& value) {
        return value.id == slot.itemId;
      });
      if (record != this->cooldowns.records.end()) {
        const auto value = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == index; });
        if (value != record->main.end()) {
          record->main.erase(value);
        }
      }

      skill = this->skills.LookupFirst(slot.itemId);
      if (skill.available == 0) {
        skill = this->skills.LookupOther(slot.itemId);
      }

      if (skill.available == 0) {
        skill = this->skills.LookupAdditional(slot.itemId);
      }
    }

    if (skill.available != 0 && skill.level != 0 && this->levels.Lookup(static_cast<int>(skill.id)) == 0) {
      slot.unknown_0005 = static_cast<std::int16_t>(skill.level);
    }
  }

  for (int index = 0; index < 9; ++index) {
    itemSlots[index] = &this->items[index];
  }
}

// OFFSET: 0x00E26050
// FUNCTION: fn_23913
void ShortcutBindings::Assign(
  unsigned char type, std::uint32_t id, int level, int slot, int bank, const unsigned char& currentRow, LuaScript& script, ModeEvents& modeEvents
) {
  const int rowStart = this->Rotate(9 * currentRow, bank);
  const int destination = this->Rotate(slot, bank);
  if (id == 0) {
    if (type != 0) {
      bool erased = false;

      for (ShortcutCooldownRecord& record : this->cooldowns.records) {
        const auto value = std::find_if(record.main.begin(), record.main.end(), [&](const CooldownState& state) { return state.slot == slot; });
        if (value != record.main.end()) {
          record.main.erase(value);
          erased = true;
        }

        if (erased) {
          break;
        }
      }
    }

    if (bank == 0 || bank == 1) {
      this->banks[bank][destination] = {};
    }

    modeEvents.SendModeMessage(313, destination, bank, 0, 0);
    return;
  }

  for (int offset = 0; offset < 9; ++offset) {
    if (bank != 0 && bank != 1) {
      continue;
    }

    const int index = rowStart + offset;
    const NormalItemSlot previous = this->banks[bank][index];
    bool duplicate = previous.itemId == id && previous.unknown_0005 == level && previous.unknown_0000 == type;

    if (!duplicate) {
      const bool usesLevel = CSkillInfo::UsesLevel(script, static_cast<int>(previous.itemId));
      duplicate = !usesLevel && previous.itemId == id && previous.unknown_0000 == type;
    }

    if (!duplicate) {
      continue;
    }

    const auto record =
      std::find_if(this->cooldowns.records.begin(), this->cooldowns.records.end(), [&](const ShortcutCooldownRecord& value) { return value.id == id; });
    if (record != this->cooldowns.records.end() && record->end > timeGetTime()) {
      const int position = index + 9 * currentRow - rowStart;
      const auto value = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == position; });
      if (value != record->main.end()) {
        record->main.erase(value);
      }
    }

    this->banks[bank][index] = {};
    modeEvents.SendModeMessage(313, index, bank, 0, 0);
  }

  if (bank == 0 || bank == 1) {
    NormalItemSlot& value = this->banks[bank][destination];
    value.unknown_0005 = static_cast<std::int16_t>(level);
    value.unknown_0000 = type;
    value.itemId = id;
  }

  modeEvents.SendModeMessage(313, destination, bank, 0, 0);
}

// OFFSET: 0x00E492D0
// FUNCTION: fn_24233
void ShortcutBindings::AssignItem(std::uint32_t id, int slot) {
  if (id != 0) {
    for (int index = 0; index < 9; ++index) {
      if (this->items[index].itemId == id) {
        this->items[index] = {};
        break;
      }
    }

    this->items[slot].unknown_0005 = 0;
    this->items[slot].unknown_0000 = 0;
    this->items[slot].itemId = id;
  } else {
    this->items[slot] = {};
  }
}
