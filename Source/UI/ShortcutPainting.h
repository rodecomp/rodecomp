#ifndef RAGNAROK_SOURCE_UI_SHORTCUTPAINTING_H
#define RAGNAROK_SOURCE_UI_SHORTCUTPAINTING_H

#include "Contents/ClientSkills.h"
#include "Contents/ItemDescription.h"
#include "Contents/ItemInfo.h"
#include "Contents/NormalItemSlots.h"
#include "UI/ShortcutCooldowns.h"
#include "UI/ShortcutGrid.h"

class UIWindow;
class ResourceMgr;
class FontCache;
struct TextLocale;

// Borrowed original window/client/manager state used by the complete Paint
// operation. This does not replace the unresolved UIShortCutWnd lifecycle.
struct ShortcutPainting {
  ResourceMgr& resources;
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
  const bool& itemSlotActive;
  const unsigned char& currentRow;

  void Paint(
    UIWindow& window,
    const ShortcutGrid& grid,
    const NormalItemSlot* const* slots,
    const NormalItemSlot* const* itemSlots
  );
};

#endif
