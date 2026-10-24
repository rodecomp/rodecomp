#include "UI/ShortcutDetails.h"
#include "UI/SkillDetailClose.h"
#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"

#include <bit>

namespace {

  int Add(int left, int right) {
    return std::bit_cast<int>(static_cast<unsigned>(left) + static_cast<unsigned>(right));
  }

  void ToScreen(const UIWindow& window, int& x, int& y) {
    x = Add(x, window.geometry.x);
    y = Add(y, window.geometry.y);

    for (const WindowAncestry* node = window.parent; node; node = node->parent) {
      const UIWindow* owner = static_cast<const UIWindow*>(node);
      x = Add(x, owner->geometry.x);
      y = Add(y, owner->geometry.y);
    }
  }

}

// OFFSET: 0x009332D0
// FUNCTION: fn_12724
void ShortcutDetails::Open(
  UIWindow& window,
  ShortcutGrid& grid,
  int x,
  int y,
  const NormalItemSlot* const* slots,
  const NormalItemSlot* const* itemSlots
) {
  const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);

  if (column == -1) {
    return;
  }

  const int index = 9 * this->currentRow + column;
  const int limit = std::bit_cast<int>(9u * static_cast<unsigned>(grid.rows));
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
    ItemInfo item = LookupClientItem(this->inventory, std::bit_cast<int>(slot->itemId), 1);

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
  const int targetX = Add(originX, x);
  unsigned adjustment = 0;

  if (this->screenWidth - static_cast<unsigned>(targetX) < static_cast<unsigned>(detail->geometry.width)) {
    adjustment = static_cast<unsigned>(targetX) + detail->geometry.width - this->screenWidth;
  }

  const int rowY = std::bit_cast<int>(33u * static_cast<unsigned>(y / 33));
  const int targetY = static_cast<float>(originY) <= static_cast<float>(this->screenHeight) * 0.5f
                        ? Add(Add(originY, rowY), 33)
                        : Add(Add(originY, rowY), -detail->geometry.height);

  detail->Move(std::bit_cast<int>(static_cast<unsigned>(targetX) - adjustment), targetY);
}
