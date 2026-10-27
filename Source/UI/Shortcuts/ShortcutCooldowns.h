#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H

#include "UI/Cooldowns/CooldownDrawing.h"

#include <cstddef>
#include <list>

struct ShortcutCooldownIcon {
  /* 0000 */ unsigned id;
  /* 0004 */ int slot;
  /* 0008 */ int x;
  /* 000C */ int y;
};

struct ShortcutCooldownRecord {
  /* 0000 */ unsigned id;
  /* 0004 */ std::uint32_t end;
  /* 0008 */ std::uint32_t duration;
  /* 000C */ std::list<CooldownState> main;
  /* 0014 */ std::list<CooldownState> alternate;
};

struct ShortcutCooldowns {
  /* 0000 */ std::list<ShortcutCooldownRecord> records;

  void LoadSnapshot(
    const void* packet, const std::list<ShortcutCooldownIcon>& mainIcons, const std::list<ShortcutCooldownIcon>& alternateIcons, int replayMode,
    const std::uint32_t& serverTick
  );
  void ApplyStart(
    const void* packet, const std::list<ShortcutCooldownIcon>& mainIcons, const std::list<ShortcutCooldownIcon>& alternateIcons, int replayMode,
    const std::uint32_t& serverTick
  );
  bool RotateRows(int amount, int bank, int& rotation0, int& rotation1, const CooldownClock& clock);

  void PaintMain(
    const std::list<ShortcutCooldownIcon>& icons, int rows, int x, int y, CooldownDrawing& drawing, const CooldownClock& clock, RenderCommandPool& pool,
    CRenderSystemBase& system
  );
  void PaintAlternate(
    const std::list<ShortcutCooldownIcon>& icons, int x, int y, CooldownDrawing& drawing, const CooldownClock& clock, RenderCommandPool& pool,
    CRenderSystemBase& system
  );

private:
  void Paint(
    const std::list<ShortcutCooldownIcon>& icons, std::list<CooldownState> ShortcutCooldownRecord::* values, int limit, int x, int y, CooldownDrawing& drawing,
    const CooldownClock& clock, RenderCommandPool& pool, CRenderSystemBase& system
  );
};

static_assert(sizeof(ShortcutCooldownIcon) == 16);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ShortcutCooldownRecord) == 28);
static_assert(sizeof(ShortcutCooldowns) == 8);
static_assert(offsetof(ShortcutCooldownIcon, id) == 0x0000);
static_assert(offsetof(ShortcutCooldownIcon, slot) == 0x0004);
static_assert(offsetof(ShortcutCooldownIcon, x) == 0x0008);
static_assert(offsetof(ShortcutCooldownIcon, y) == 0x000C);
static_assert(offsetof(ShortcutCooldownRecord, id) == 0x0000);
static_assert(offsetof(ShortcutCooldownRecord, end) == 0x0004);
static_assert(offsetof(ShortcutCooldownRecord, duration) == 0x0008);
static_assert(offsetof(ShortcutCooldownRecord, main) == 0x000C);
static_assert(offsetof(ShortcutCooldownRecord, alternate) == 0x0014);
static_assert(offsetof(ShortcutCooldowns, records) == 0x0000);
#endif

#endif
