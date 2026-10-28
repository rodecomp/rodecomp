#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H

#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>

class FontCache;
struct TextLocale;
struct ClientSkills;
struct ItemInfo;
struct ItemDescriptions;

struct QuickSlotPainting {
  FontCache& fonts;
  const TextLocale& locale;
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  ItemDescriptions& descriptions;
  std::list<ShortcutCooldownIcon>& icons;
  ShortcutCooldowns& cooldowns;
  CooldownDrawing& drawing;
  const CooldownClock& clock;
  RenderCommandPool& commands;
  CRenderSystemBase& system;
};

#pragma pack(push, 1)
struct QuickSlotEntry {
  /* 0000 */ unsigned char skill;
  /* 0001 */ std::uint32_t id;
  /* 0005 */ std::int16_t count;
};
#pragma pack(pop)

class UIQuickSlotWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  /* 00B4 */ int m_selected;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ QuickSlotEntry m_entries[2];
  QuickSlotPainting* m_painting;
};

static_assert(sizeof(QuickSlotEntry) == 7);

#endif
