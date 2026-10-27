#include "UI/Shortcuts/ShortcutDetails.h"
#include "UI/Shortcuts/SkillDetailClose.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Window/UIWindowMgr.h"

namespace {
  // INLINED
  void ToScreen(const UIWindow& window, int& x, int& y) {
    x = (x + window.geometry.x);
    y = (y + window.geometry.y);

    for (const UIWindow* node = window.parent; node; node = node->parent) {
      x = (x + node->geometry.x);
      y = (y + node->geometry.y);
    }
  }
}

// INLINED
inline bool SkillDetailClose::Close() {
  if (this->window) {
    this->saved[46] = {this->window->geometry.x, this->window->geometry.y, this->window->geometry.width, this->window->geometry.height};
  }

  if (!this->modalStack.empty() && this->modalStack.back() != 0) {
    return false;
  }

  if (this->window == this->reference_4F1D4) {
    this->reference_4F1D4 = nullptr;
  }

  bool removed = false;

  if (this->window) {
    this->retirement.Schedule(this->window);
    this->window = nullptr;
    this->visible = 0;
    removed = true;
  }

  this->windowsById.erase(46);

  if (removed) {
    this->focus.RemoveOrder(46);
  }

  return removed;
}

// OFFSET: 0x009332D0
// FUNCTION: fn_12724
void ShortcutDetails::Open(UIWindow& window, ShortcutGrid& grid, int x, int y, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots) {
  const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);

  if (column == -1) {
    return;
  }

  const int index = 9 * this->currentRow + column;
  const int limit = 9 * grid.rows;
  const NormalItemSlot* slot;

  if (limit <= index) {
    if (!this->itemSlotActive) {
      return;
    }

    slot = itemSlots[column];
  } else {
    slot = slots[index];

    if (slot->itemId == 0) {
      return;
    }
  }

  if (limit <= index || slot->unknown_0000 == 0) {
    ItemInfo item = LookupClientItem(this->inventory, static_cast<int>(slot->itemId), 1);

    if (item.unknown_0000[1] != 0) {
      UIWindow* detail = this->windows.EnsureWindow(12);
      detail->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }

    return;
  }

  CSkillInfo skill = this->skills.Lookup(slot->itemId);

  if (!skill.MatchesShortcut(slot)) {
    return;
  }

  UIWindow* detail = this->windows.EnsureWindow(46);

  if (detail) {
    if (this->describedSkill == skill.id) {
      this->skillClose.Close();
    } else {
      detail->SendMsg(nullptr, 61, skill.id, 0, 0, 0);
    }
  }

  int originX = 0;
  int originY = 0;
  ToScreen(window, originX, originY);
  const int targetX = (originX + x);
  int adjustment = 0;

  if (this->screenWidth - static_cast<unsigned>(targetX) < static_cast<unsigned>(detail->geometry.width)) {
    adjustment = targetX + detail->geometry.width - static_cast<int>(this->screenWidth);
  }

  const int rowY = 33 * (y / 33);
  const int targetY =
    static_cast<float>(originY) <= static_cast<float>(this->screenHeight) * 0.5f ? ((originY + rowY) + 33) : ((originY + rowY) - detail->geometry.height);

  detail->Move(targetX - adjustment, targetY);
}
