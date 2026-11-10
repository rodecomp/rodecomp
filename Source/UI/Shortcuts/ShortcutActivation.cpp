#include "UI/Window/UIEnums.h"
#include "Client/Session.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x00930900
// FUNCTION: fn_12689
void UIShortCutWnd::OnLBtnDblClk(int x, int y) {
  const int column = this->m_grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);
  if (column == -1) {
    return;
  }

  const unsigned index = 9u * g_session.m_hotkeyPage + static_cast<unsigned>(column);
  const int limit = 9 * this->m_grid.rows;
  if (limit <= static_cast<int>(index)) {
    return;
  }

  const NormalItemSlot& slot = *this->m_slots[index];
  if (slot.enabled != 0 && g_session.m_automaticSkills.Contains(static_cast<std::uint16_t>(slot.itemId))) {
    g_session.SetOption(g_session.m_automaticSkills.CancelOption(slot.itemId), 1, 0);
  } else {
    g_session.m_automaticSkills.DisableAll();
  }

  this->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, column, g_session.m_hotkeyPage, 0, 0);
}
