#include "Client/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/ModeTooltip.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UINewSelectCharWnd.h"

#include <stdio.h>

namespace {
  // INLINED
  void ScreenPosition(const UIWindow& window, int& x, int& y) {
    x = (x + window.geometry.x);
    y = (y + window.geometry.y);

    for (const UIWindow* node = window.parent; node; node = node->parent) {
      x = (x + node->geometry.x);
      y = (y + node->geometry.y);
    }
  }
}

// OFFSET: 0x00932050
// FUNCTION: fn_12711
void UIShortCutWnd::Slot28(int x, int y) {
  UIWindow& window = *this;
  ShortcutGrid& grid = this->m_grid;
  const bool itemSlotActive = this->m_shortcuts->itemSlotActive;
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
    if (item.unknown_0000[1] == 0) {
      return;
    }

    const std::string name = item.DisplayName(0);
    char text[4096];
    _snprintf_s(text, sizeof(text), _TRUNCATE, LookupMessage(183), name.c_str(), item.unknown_0000[1]);
    ScreenPosition(window, tooltipX, tooltipY);

    if (g_charViewMode) {
      reinterpret_cast<ModeTooltip*>(g_charViewMode)->Show(text, tooltipX, tooltipY, -1, 0, 1);
    }

    return;
  }

  HotkeyInfo hotkey;
  const int bank = g_session.GetOption(10);
  if (bank == 0) {
    hotkey = GetHotkeyInfo(*g_session.m_hotkeyScript, 0, index);
  } else if (bank == 1) {
    hotkey = GetHotkeyInfo(*g_session.m_hotkeyScript, 3, index);
  }

  std::string shortcut;
  if (!hotkey.description.empty()) {
    shortcut = std::move(hotkey.text);
  }

  ScreenPosition(window, tooltipX, tooltipY);
  std::string text;
  const NormalItemSlot& slot = *this->m_slots[index];

  if (slot.itemId == 0) {
    text = shortcut;
  } else if (slot.unknown_0000 != 0) {
    CSkillInfo skill = ClientSkills{
        g_session.m_skillBundle,
        g_session.m_homunSkills,
        g_session.m_mercSkills,
        g_session.m_additionalSkills,
        reinterpret_cast<const int&>(g_session.m_jobFlags),
        g_activeSkillEffects
      }.Lookup(slot.itemId);
    if (!skill.MatchesShortcut(&slot)) {
      text = shortcut;
    } else {
      int level = slot.unknown_0005;
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      const int sp = skill.UseSpAmount(*g_session.m_hotkeyScript, level);
      const int ap = skill.UseApAmount(*g_session.m_hotkeyScript, level);
      text = SkillTooltip(shortcut.c_str(), skill.DisplayName(*g_session.m_hotkeyScript), level, sp, ap, SkillUsesPercentSp(static_cast<int>(skill.id)));
    }
  } else {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
    if (item.unknown_0000[1] == 0) {
      text = shortcut;
    } else {
      const std::string name = item.DisplayName(0);
      char formatted[4096];
      _snprintf_s(formatted, sizeof(formatted), _TRUNCATE, LookupMessage(184), shortcut.c_str(), name.c_str(), item.unknown_0000[1]);
      text = formatted;
    }
  }

  reinterpret_cast<ModeTooltip*>(g_charViewMode)->Show(text.c_str(), tooltipX, tooltipY, -1, 0, 1);
}
