#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTCOOLDOWNS_H

#include "UI/Cooldowns/CooldownDrawing.h"

#include <list>

struct ShortcutCooldownIcon {
  unsigned id;
  int slot;
  int x;
  int y;
};

struct ShortcutCooldownRecord {
  unsigned id;
  std::uint32_t end;
  std::uint32_t duration;
  std::list<CooldownState> main;
  std::list<CooldownState> alternate;
};

struct ShortcutCooldowns {
  std::list<ShortcutCooldownRecord> records;

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
#endif

#endif
