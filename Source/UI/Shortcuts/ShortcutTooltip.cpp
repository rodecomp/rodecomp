#include "Client/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindow.h"
#include "Client/Mode/ModeMgr.h"
#include "Utility/Text/FormatString.h"

// OFFSET: 0x00932050
// FUNCTION: fn_12711
void UIShortCutWnd::Slot28(int x, int y) {
  UIWindow& window = *this;
  ShortcutGrid& grid = this->m_grid;
  const bool itemSlotActive = g_session.ItemSlotActive();
  const int column = grid.HitTest(x, y, itemSlotActive, g_session.m_hotkeyPage);
  if (column != grid.hovered) {
    grid.hovered = column;
    window.Invalidate();
  }

  if (grid.hovered == -1 || grid.row == -1) {
    return;
  }

  const int index = (grid.hovered + 9 * grid.row);
  int tooltipX = 29 * ((x - 15) / 29) + 15;
  int tooltipY = -17;

  if (9 * grid.rows <= index) {
    if (!itemSlotActive) {
      return;
    }

    const NormalItemSlot& slot = *this->m_itemSlots[grid.hovered];
    if (slot.itemId == 0) {
      return;
    }

    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
    if (item.quantity == 0) {
      return;
    }

    const std::string text = FormatOwnedString(LookupMessage(183), item.DisplayName(0).c_str(), item.quantity);
    window.ToScreen(tooltipX, tooltipY);

    if (g_modeMgr.gameMode) {
      g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, -1, 0, 1);
    }

    return;
  }

  std::string shortcut;
  HotkeyInfo hotkey;
  const int bank = g_session.GetOption(10);
  if (bank == 0) {
    hotkey = GetHotkeyInfo(*g_session.m_hotkeyScript, 0, index);
  } else if (bank == 1) {
    hotkey = GetHotkeyInfo(*g_session.m_hotkeyScript, 3, index);
  }

  if (!hotkey.description.empty()) {
    shortcut = hotkey.text;
  }

  window.ToScreen(tooltipX, tooltipY);
  std::string text;
  const NormalItemSlot& slot = *this->m_slots[index];

  if (slot.itemId == 0) {
    text = shortcut;
  } else if (slot.unknown_0000 != 0) {
    CSkillInfo skill = g_session.LookupSkill(slot.itemId);
    if (!skill.MatchesShortcut(&slot)) {
      text = shortcut;
    } else {
      int level = slot.unknown_0005;
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      const int sp = skill.UseSpAmount(*g_session.m_hotkeyScript, level);
      const int ap = skill.UseApAmount(*g_session.m_hotkeyScript, level);
      if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
        text = SkillTooltipPercent(shortcut.c_str(), skill.DisplayName(*g_session.m_hotkeyScript), level, sp, ap);
      } else {
        text = SkillTooltipFlat(shortcut.c_str(), skill.DisplayName(*g_session.m_hotkeyScript), level, sp, ap);
      }
    }
  } else {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
    if (item.quantity == 0) {
      text = shortcut;
    } else {
      text = FormatOwnedString(LookupMessage(184), shortcut.c_str(), item.DisplayName(0).c_str(), item.quantity);
    }
  }

  g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, -1, 0, 1);
}
