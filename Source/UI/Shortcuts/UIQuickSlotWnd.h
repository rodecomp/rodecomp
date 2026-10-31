#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_UIQUICKSLOTWND_H

#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>

class FontCache;
struct TextLocale;
struct ClientSkills;
struct ITEM_INFO;
struct ItemDescriptions;
class AutomaticSkills;

struct QuickSlotPainting {
  FontCache& fonts;
  const TextLocale& locale;
  ClientSkills& skills;
  const std::list<ITEM_INFO>& inventory;
  ItemDescriptions& descriptions;
  std::list<ShortcutCooldownIcon>& icons;
  ShortcutCooldowns& cooldowns;
  CooldownDrawing& drawing;
  const CooldownClock& clock;
  RenderCommandPool& commands;
  CRenderSystemBase& system;
  AutomaticSkills& automatic;
};

#pragma pack(push, 1)
struct QuickSlotEntry {
  /* 0000 */ unsigned char skill;
  /* 0001 */ std::uint32_t id;
  /* 0005 */ std::int16_t count;
};
#pragma pack(pop)

struct QuickSlotWndInfo;

class UIQuickSlotWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t argument) override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  int SlotX(int slot);
  int SlotAt(int x, int y);

protected:
  /* 00B4 */ int m_selected;
  /* 00B8 */ QuickSlotWndInfo* m_saved;
  /* 00BC */ QuickSlotEntry m_entries[2];
  QuickSlotPainting* m_painting;
};

static_assert(sizeof(QuickSlotEntry) == 7);

#endif
