#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H

#include "UI/Cooldowns/CooldownClock.h"
#include "UI/Cooldowns/CooldownState.h"

#include <cstddef>
#include <list>

class CooldownOwner;

struct ShortcutCooldownIcon {
  /* 0000 */ int id;
  /* 0004 */ int slot;
  /* 0008 */ int x;
  /* 000C */ int y;
};

struct ShortcutCooldownRecord {
  /* 0000 */ int id;
  /* 0004 */ std::uint32_t end;
  /* 0008 */ std::uint32_t duration;
  /* 000C */ std::list<CooldownState> main;
  /* 0014 */ std::list<CooldownState> alternate;
};

// INLINED
inline bool RotateCooldownRows(std::list<ShortcutCooldownRecord>& records, int amount, int bank, int& rotation0, int& rotation1) {
  const int rows = amount >= 4 ? amount % 4 : amount;
  if (rows <= 0) {
    return false;
  }

  if (bank == 0) {
    rotation0 += rows;
    if (rotation0 >= 4) {
      rotation0 -= 4;
    }
  } else if (bank == 1) {
    rotation1 += rows;
    if (rotation1 >= 4) {
      rotation1 -= 4;
    }
  }

  const std::uint32_t now = CooldownNow();

  for (auto entry = records.begin(); entry != records.end();) {
    if (entry->end > now) {
      for (auto state = entry->main.begin(); state != entry->main.end(); ++state) {
        state->slot -= 9 * rows;
        state->y -= 33 * rows;
        if (state->slot < 0) {
          state->slot += 36;
          state->y += 132;
        }
      }

      ++entry;
    } else {
      entry = records.erase(entry);
    }
  }

  return true;
}

void PaintAlternateCooldowns(
  std::list<ShortcutCooldownRecord>& records, const std::list<ShortcutCooldownIcon>& icons, int x, int y, CooldownOwner& owner
);

static_assert(sizeof(ShortcutCooldownIcon) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ShortcutCooldownRecord) == 28);
static_assert(offsetof(ShortcutCooldownIcon, id) == 0x0000);
static_assert(offsetof(ShortcutCooldownIcon, slot) == 0x0004);
static_assert(offsetof(ShortcutCooldownIcon, x) == 0x0008);
static_assert(offsetof(ShortcutCooldownIcon, y) == 0x000C);
static_assert(offsetof(ShortcutCooldownRecord, id) == 0x0000);
static_assert(offsetof(ShortcutCooldownRecord, end) == 0x0004);
static_assert(offsetof(ShortcutCooldownRecord, duration) == 0x0008);
static_assert(offsetof(ShortcutCooldownRecord, main) == 0x000C);
static_assert(offsetof(ShortcutCooldownRecord, alternate) == 0x0014);
#endif

#endif
