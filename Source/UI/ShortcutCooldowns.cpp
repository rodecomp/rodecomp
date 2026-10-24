#include "UI/ShortcutCooldowns.h"

#include <algorithm>
#include <bit>
#include <cstring>
#include <windows.h>
// clang-format off
#include <mmsystem.h>
// clang-format on

namespace {
  // OFFSET: 0x00E1DCA0
  // FUNCTION: fn_23875
  bool IsFirstRange(unsigned id) {
    return id - 8000u <= 60;
  }
  // OFFSET: 0x00E1DD20
  // FUNCTION: fn_23878
  bool IsOtherRange(unsigned id) {
    return id - 8200u <= 41 || id - 8400u <= 57;
  }
}

// OFFSET: 0x00D42070
// FUNCTION: fn_21827
void ShortcutCooldowns::LoadSnapshot(
  const void* packet,
  const std::list<ShortcutCooldownIcon>& mainIcons,
  const std::list<ShortcutCooldownIcon>& alternateIcons,
  int replayMode,
  const std::uint32_t& serverTick
) {
  this->records.clear();
  const auto* bytes = static_cast<const unsigned char*>(packet);
  std::int16_t size;
  std::memcpy(&size, bytes + 2, sizeof(size));
  const int count = (size - 2) / 10;
  for (int index = 0; index < count; ++index) {
    const unsigned char* source = bytes + 4 + index * 10;
    std::uint16_t id;
    std::uint32_t duration;
    std::uint32_t remaining;
    std::memcpy(&id, source, sizeof(id));
    std::memcpy(&duration, source + 2, sizeof(duration));
    std::memcpy(&remaining, source + 6, sizeof(remaining));
    const std::uint32_t now = replayMode == 1 ? serverTick : timeGetTime();
    ShortcutCooldownRecord value{id, now + remaining, duration, {}, {}};
    for (const ShortcutCooldownIcon& icon : mainIcons) {
      if (icon.id == id) {
        value.main.emplace_back(icon.slot, icon.x, icon.y, duration, now, duration - remaining, 0);
      }
    }
    for (const ShortcutCooldownIcon& icon : alternateIcons) {
      if (icon.id == id) {
        value.alternate.emplace_back(icon.slot, icon.x, icon.y, duration, now, duration - remaining, 0);
      }
    }
    // Original copies both lists into the record; do not alias temporary values.
    this->records.push_back(value);
  }
}

// OFFSET: 0x00D41990
// FUNCTION: fn_21826
void ShortcutCooldowns::ApplyStart(
  const void* packet,
  const std::list<ShortcutCooldownIcon>& mainIcons,
  const std::list<ShortcutCooldownIcon>& alternateIcons,
  int replayMode,
  const std::uint32_t& serverTick
) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  std::uint16_t id;
  std::uint32_t duration;
  std::memcpy(&id, bytes + 2, sizeof(id));
  std::memcpy(&duration, bytes + 4, sizeof(duration));
  const std::uint32_t now = replayMode == 1 ? serverTick : timeGetTime();
  ShortcutCooldownRecord value{id, now + duration, duration, {}, {}};
  const auto find = [&]() {
    return std::find_if(this->records.begin(), this->records.end(), [&](const ShortcutCooldownRecord& record) {
      return record.id == id;
    });
  };
  for (const ShortcutCooldownIcon& icon : mainIcons) {
    if (icon.id != id) {
      continue;
    }
    const auto entry = find();
    if (entry == this->records.end()) {
      value.main.emplace_back(icon.slot, icon.x, icon.y, duration, now, 0, 0);
      this->records.push_back(value);
    } else {
      const auto slot = std::find_if(entry->main.begin(), entry->main.end(), [&](const CooldownState& state) {
        return state.slot == icon.slot;
      });
      if (slot == entry->main.end()) {
        // Adding a new slot does not update the existing record's expiry.
        entry->main.emplace_back(icon.slot, icon.x, icon.y, duration, now, 0, 0);
      } else {
        this->records.erase(entry);
        // The original local lists accumulate across this packet's icons.
        value.main.emplace_back(icon.slot, icon.x, icon.y, duration, now, 0, 0);
        this->records.push_back(value);
      }
    }
  }
  for (const ShortcutCooldownIcon& icon : alternateIcons) {
    if (icon.id == id && find() == this->records.end()) {
      value.alternate.emplace_back(icon.slot, icon.x, icon.y, duration, now, 0, 0);
      this->records.push_back(value);
    }
  }
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
bool ShortcutCooldowns::RotateRows(int amount, int bank, int& rotation0, int& rotation1, const CooldownClock& clock) {
  const int rows = amount < 4 ? amount : amount % 4;
  if (rows <= 0) {
    return false;
  }
  const auto add = [](int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  };
  if (bank == 0 || bank == 1) {
    int& rotation = bank == 0 ? rotation0 : rotation1;
    rotation = add(rotation, rows);
    if (rotation >= 4) {
      rotation = add(rotation, -4);
    }
  }
  const std::uint32_t now = clock.Now();
  for (auto entry = this->records.begin(); entry != this->records.end();) {
    if (entry->end <= now) {
      entry = this->records.erase(entry);
      continue;
    }
    for (CooldownState& state : entry->main) {
      state.slot = add(state.slot, -9 * rows);
      state.y = add(state.y, -33 * rows);
      if (state.slot < 0) {
        state.slot = add(state.slot, 36);
        state.y = add(state.y, 132);
      }
    }
    ++entry;
  }
  return true;
}

// OFFSET: 0x0092F8E0
// FUNCTION: fn_12680
void ShortcutCooldowns::PaintMain(
  const std::list<ShortcutCooldownIcon>& icons,
  int rows,
  int x,
  int y,
  CooldownDrawing& drawing,
  const CooldownClock& clock,
  RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  const int limit = std::bit_cast<int>(static_cast<unsigned>(rows) * 33u + 4u);
  this->Paint(icons, &ShortcutCooldownRecord::main, limit, x, y, drawing, clock, pool, system);
}

// OFFSET: 0x0092D660
// FUNCTION: fn_12671
void ShortcutCooldowns::PaintAlternate(
  const std::list<ShortcutCooldownIcon>& icons,
  int x,
  int y,
  CooldownDrawing& drawing,
  const CooldownClock& clock,
  RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  this->Paint(icons, &ShortcutCooldownRecord::alternate, 0, x, y, drawing, clock, pool, system);
}

void ShortcutCooldowns::Paint(
  const std::list<ShortcutCooldownIcon>& icons,
  std::list<CooldownState> ShortcutCooldownRecord::* values,
  int limit,
  int x,
  int y,
  CooldownDrawing& drawing,
  const CooldownClock& clock,
  RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  const std::uint32_t now = clock.Now();
  for (auto entry = this->records.begin(); entry != this->records.end();) {
    // Original uses absolute unsigned comparison, not signed tick subtraction.
    if (entry->end <= now) {
      entry = this->records.erase(entry);
      continue;
    }
    std::list<CooldownState>& states = (*entry).*values;
    for (const ShortcutCooldownIcon& icon : icons) {
      if (
        entry->id == icon.id || (entry->id == 577 && !IsFirstRange(icon.id)) ||
        (entry->id == 9999 && (IsFirstRange(icon.id) || IsOtherRange(icon.id)))
      ) {
        const auto previous = std::find_if(states.begin(), states.end(), [&](const CooldownState& state) {
          return state.slot == icon.slot;
        });
        if (previous == states.end()) {
          states.emplace_back(icon.slot, icon.x, icon.y, entry->duration, entry->end - entry->duration, 0, 0);
        }
      }
    }
    for (CooldownState& state : states) {
      drawing.Draw(0, 0, 12, 12, state, limit, x, y, clock, pool, system);
    }
    ++entry;
  }
}
