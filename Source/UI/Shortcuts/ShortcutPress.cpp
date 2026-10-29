#include "Client/Mode/ModeDragState.h"
#include "UI/Shortcuts/ShortcutPress.h"
#include "UI/Window/UIFrameWnd.h"

// OFFSET: 0x009310E0
// FUNCTION: fn_12699
void ShortcutPress::Press(UIFrameWnd& window, ShortcutGrid& grid, int x, int y, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots) {
  const int columnOffset = x - 15;
  if (columnOffset >= 261 || x < 15 || y < 4) {
    window.UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->restricted) {
    return;
  }

  const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);
  if (column == -1) {
    return;
  }

  grid.hovered = column;
  const int index = 9 * this->currentRow + column;
  if (9 * grid.rows > index) {
    const NormalItemSlot& slot = *slots[index];
    if (slot.itemId == 0) {
      return;
    }

    if (slot.unknown_0000 != 0) {
      CSkillInfo skill = this->skills.Lookup(slot.itemId);
      if (skill.MatchesShortcut(&slot) && this->drag) {
        this->drag->BeginSkill(this->windows, this->resources, 7, skill.id, skill.name, slot.unknown_0005, index);
      }
    } else {
      ITEM_INFO item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
      if (item.unknown_0000[1] != 0) {
        this->drag->BeginItem(this->windows, this->descriptions, 7, item, item.quantity, -1);
      }
    }
  } else if (this->itemSlotActive) {
    const NormalItemSlot& slot = *itemSlots[column];
    if (slot.itemId == 0) {
      return;
    }

    ITEM_INFO item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
    if (item.unknown_0000[1] != 0 && this->drag) {
      this->drag->BeginItem(this->windows, this->descriptions, 7, item, item.quantity, -1);
    }
  }
}
