#ifndef RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPRESS_H
#define RAGNAROK_SOURCE_UI_SHORTCUTS_SHORTCUTPRESS_H

#include "Contents/Skills/ClientSkills.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutGrid.h"

class UIFrameWnd;
class WindowManagerWindows;
class ResourceMgr;
struct ModeDragState;

struct ShortcutPress {
  ClientSkills& skills;
  const std::list<ItemInfo>& inventory;
  ItemDescriptions& descriptions;
  ResourceMgr& resources;
  WindowManagerWindows& windows;
  ModeDragState*& drag;
  const bool& restricted;
  const bool& itemSlotActive;
  unsigned char& currentRow;

  void Press(UIFrameWnd& window, ShortcutGrid& grid, int x, int y, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots);
};

#endif
