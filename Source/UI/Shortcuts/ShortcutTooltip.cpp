#include "Client/Session/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindow.h"
#include "Client/Mode/ModeMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x00932050
// FUNCTION: fn_12711
void UIShortCutWnd::OnMouseHover(int x, int y) {
  UIWindow& window = *this;
  ShortcutGrid& grid = this->m_grid;
  const int column = this->HitTestSlot(x, y);
  if (column != grid.hovered) {
    grid.hovered = column;
    window.Invalidate();
  }

  if (grid.hovered == -1 || grid.row == -1) {
    return;
  }

  const int index = (grid.hovered + 9 * grid.row);
  if (9 * grid.rows <= index) {
    if (!g_session.ItemSlotActive()) {
      return;
    }

    const NormalItemSlot& slot = *this->m_itemSlots[grid.hovered];
    if (slot.itemId == 0) {
      return;
    }

    ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(slot.itemId), 1);
    if (item.quantity == 0) {
      return;
    }

    const std::string text = FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity);
    int tooltipX = 29 * ((x - 15) / 29) + 15;
    int tooltipY = -17;
    window.ToScreen(tooltipX, tooltipY);

    if (g_modeMgr.currentMode) {
      g_modeMgr.currentMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, -1, 0, 1);
    }

    return;
  }

  std::string shortcut = "";
  HotkeyInfo hotkey;
  const int bank = g_session.GetOption(10);
  if (bank == 0) {
    hotkey = g_session.GetHotkeyInfo(0, index);
  } else if (bank == 1) {
    hotkey = g_session.GetHotkeyInfo(3, index);
  }

  if (!hotkey.description.empty()) {
    shortcut = hotkey.text;
  }

  int tooltipX = 29 * ((x - 15) / 29) + 15;
  int tooltipY = -17;
  window.ToScreen(tooltipX, tooltipY);

  std::string text = "";
  if (this->m_slots[index]->itemId != 0) {
    if (this->m_slots[index]->enabled != 0) {
      CSkillInfo skill = g_session.LookupSkill(this->m_slots[index]->itemId);
      if (!this->MatchesShortcut(skill, this->m_slots[index])) {
        text = shortcut;
      } else {
        int level = this->m_slots[index]->value;
        if (IsIndexedSkill(static_cast<int>(skill.id)) && skill.level < level) {
          level = skill.level;
        }

        const int sp = skill.UseSpAmount(level);
        const int ap = skill.UseApAmount(level);
        if (g_session.SkillUsesPercentSp(static_cast<int>(skill.id))) {
          text = SkillTooltipPercent(shortcut.c_str(), skill.DisplayName(), level, sp, ap);
        } else {
          text = SkillTooltipFlat(shortcut.c_str(), skill.DisplayName(), level, sp, ap);
        }
      }
    } else {
      ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(this->m_slots[index]->itemId), 1);
      if (item.quantity == 0) {
        text = shortcut;
      } else {
        text = FormatOwnedString(LookupMessage(MSI_EA3), shortcut.c_str(), item.DisplayName(0).c_str(), item.quantity);
      }
    }
  } else {
    text = shortcut;
  }

  g_modeMgr.currentMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, -1, 0, 1);
}
