#include "Contents/HotkeyInfo.h"
#include "UI/ShortcutTooltip.h"
#include "UI/UIButton.h"
#include "UI/UIWindow.h"

#include <bit>
#include <cstdio>

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

// OFFSET: 0x00932050
// FUNCTION: fn_12711
void ShortcutTooltip::Hover(
  UIWindow& window,
  ShortcutGrid& grid,
  int x,
  int y,
  const NormalItemSlot* const* slots,
  const NormalItemSlot* const* itemSlots
) {
  const int column = grid.HitTest(x, y, this->itemSlotActive, this->currentRow);
  if (column != grid.hovered) {
    grid.hovered = column;
    window.Invalidate();
  }
  if (grid.hovered == -1 || grid.row == -1) {
    return;
  }
  const int index = Add(grid.hovered, std::bit_cast<int>(9u * static_cast<unsigned>(grid.row)));
  int tooltipX = Add(std::bit_cast<int>(29u * static_cast<unsigned>((x - 15) / 29)), 15);
  int tooltipY = -17;
  if (std::bit_cast<int>(9u * static_cast<unsigned>(grid.rows)) <= index) {
    if (!this->itemSlotActive) {
      return;
    }
    const NormalItemSlot& slot = *itemSlots[grid.hovered];
    if (slot.itemId == 0) {
      return;
    }
    ItemInfo item = LookupClientItem(this->inventory, std::bit_cast<int>(slot.itemId), 1);
    if (item.unknown_0000[1] == 0) {
      return;
    }
    const std::string name = this->naming.DisplayName(item, 0);
    char text[4096];
    _snprintf_s(text, sizeof(text), _TRUNCATE, this->naming.Message(183), name.c_str(), item.unknown_0000[1]);
    ToScreen(window, tooltipX, tooltipY);
    if (this->services) {
      this->services->Tooltip(text, tooltipX, tooltipY, -1, false, true);
    }
    return;
  }
  HotkeyInfo hotkey;
  const int bank = this->options.Get(10);
  if (bank == 0) {
    hotkey = GetHotkeyInfo(this->script, 0, index);
  } else if (bank == 1) {
    hotkey = GetHotkeyInfo(this->script, 3, index);
  }
  std::string shortcut;
  if (!hotkey.description.empty()) {
    shortcut = std::move(hotkey.text);
  }
  ToScreen(window, tooltipX, tooltipY);
  std::string text;
  const NormalItemSlot& slot = *slots[index];
  if (slot.itemId == 0) {
    text = shortcut;
  } else if (slot.unknown_0000 != 0) {
    CSkillInfo skill = this->skills.Lookup(slot.itemId);
    if (!skill.MatchesShortcut(&slot)) {
      text = shortcut;
    } else {
      int level = std::bit_cast<std::int16_t>(slot.unknown_0005);
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }
      const int sp = skill.UseSpAmount(this->script, level);
      const int ap = skill.UseApAmount(this->script, level);
      text = SkillTooltip(
        shortcut.c_str(), skill.DisplayName(this->script), level, sp, ap,
        SkillUsesPercentSp(std::bit_cast<int>(skill.id))
      );
    }
  } else {
    ItemInfo item = LookupClientItem(this->inventory, std::bit_cast<int>(slot.itemId), 1);
    if (item.unknown_0000[1] == 0) {
      text = shortcut;
    } else {
      const std::string name = this->naming.DisplayName(item, 0);
      char formatted[4096];
      _snprintf_s(
        formatted, sizeof(formatted), _TRUNCATE, this->naming.Message(184), shortcut.c_str(), name.c_str(),
        item.unknown_0000[1]
      );
      text = formatted;
    }
  }
  this->services->Tooltip(text.c_str(), tooltipX, tooltipY, -1, false, true);
}
