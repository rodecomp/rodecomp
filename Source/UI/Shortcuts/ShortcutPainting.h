#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPAINTING_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPAINTING_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIWindow;
class ResourceMgr;
class FontCache;
struct TextLocale;

struct ShortcutPainting {
  ResourceMgr& resources;
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
  const bool& itemSlotActive;
  const unsigned char& currentRow;

  void Paint(UIWindow& window, const ShortcutGrid& grid, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots);
};

#endif
