#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMessage.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Skills/SkillInfo.h"

#include <windows.h>
#include <timeapi.h>

#include <algorithm>

// OFFSET: 0x00E12FC0
// FUNCTION: fn_23821
int CSession::RotateShortcut(int index, int bank) const {
  int rotation;
  if (bank == 0) {
    rotation = this->mainShortcutRow;
  } else if (bank == 1) {
    rotation = this->alternateShortcutRow;
  } else {
    return index;
  }

  if (rotation != 0) {
    const int value = index + 9 * rotation;
    return value >= 36 ? value - 36 : value;
  }
  return index;
}

// OFFSET: 0x00E26050
// FUNCTION: fn_23913
void CSession::AssignShortcut(unsigned char type, std::uint32_t id, int level, int slot, int bank) {
  const int rowStart = this->RotateShortcut(9 * this->m_hotkeyPage, bank);
  const int destination = this->RotateShortcut(slot, bank);
  CMode* modeEvents = g_modeMgr.GetGameMode();
  std::list<ShortcutCooldownRecord>& cooldowns = g_session.m_requestTimers;
  if (id == 0) {
    if (type != 0) {
      for (ShortcutCooldownRecord& record : cooldowns) {
        const auto value = std::find_if(record.main.begin(), record.main.end(), [&](const CooldownState& state) { return state.slot == slot; });
        if (value != record.main.end()) {
          record.main.erase(value);
          break;
        }
      }
    }

    if (bank == 0) {
      this->m_shortcutSlots[destination] = {};
    } else if (bank == 1) {
      this->m_secondaryShortcutSlots[destination] = {};
    }

    modeEvents->SendMsg(MM_SHORTCUT_KEY_CHANGE, destination, bank, 0, 0);
    return;
  }

  for (int index = rowStart; index < rowStart + 9; ++index) {
    const auto isDuplicate = [&](NormalItemSlot previous) {
      if (id == previous.itemId && level == previous.value && type == previous.enabled) {
        return true;
      }
      return !CSkillInfo::UsesLevel(*this->m_hotkeyScript, static_cast<int>(previous.itemId)) && id == previous.itemId && type == previous.enabled;
    };
    if (bank == 0) {
      if (!isDuplicate(this->m_shortcutSlots[index])) {
        continue;
      }
    } else if (bank == 1) {
      if (!isDuplicate(this->m_secondaryShortcutSlots[index])) {
        continue;
      }
    } else {
      continue;
    }

    const auto record =
      std::find_if(cooldowns.begin(), cooldowns.end(), [&](const ShortcutCooldownRecord& value) { return value.id == id; });
    if (record != cooldowns.end() && record->end > timeGetTime()) {
      const int position = index + 9 * this->m_hotkeyPage - rowStart;
      const auto value = std::find_if(record->main.begin(), record->main.end(), [&](const CooldownState& state) { return state.slot == position; });
      if (value != record->main.end()) {
        record->main.erase(value);
      }
    }

    if (bank == 0) {
      this->m_shortcutSlots[index] = {};
    } else if (bank == 1) {
      this->m_secondaryShortcutSlots[index] = {};
    }
    modeEvents->SendMsg(MM_SHORTCUT_KEY_CHANGE, index, bank, 0, 0);
  }

  if (bank == 0) {
    this->m_shortcutSlots[destination].value = static_cast<std::int16_t>(level);
    this->m_shortcutSlots[destination].enabled = type;
    this->m_shortcutSlots[destination].itemId = id;
  } else if (bank == 1) {
    this->m_secondaryShortcutSlots[destination].value = static_cast<std::int16_t>(level);
    this->m_secondaryShortcutSlots[destination].enabled = type;
    this->m_secondaryShortcutSlots[destination].itemId = id;
  }

  modeEvents->SendMsg(MM_SHORTCUT_KEY_CHANGE, destination, bank, 0, 0);
}

// OFFSET: 0x00E492D0
// FUNCTION: fn_24233
void CSession::AssignItemShortcut(std::uint32_t id, int slot) {
  if (id == 0) {
    this->m_normalItemSlots[slot] = {};
    return;
  }

  int found = -1;
  for (int index = 0; index < 9; ++index) {
    if (id == this->m_normalItemSlots[index].itemId) {
      found = index;
      break;
    }
  }

  if (found >= 0) {
    this->m_normalItemSlots[found] = {};
  }

  this->m_normalItemSlots[slot].value = 0;
  this->m_normalItemSlots[slot].enabled = 0;
  this->m_normalItemSlots[slot].itemId = id;
}
