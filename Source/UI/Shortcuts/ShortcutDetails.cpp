#include "Client/Session.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Skill/UISkillDescribeWnd.h"
#include "Render/DX7/RenderSystemDX7.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009332D0
// FUNCTION: fn_12724
void UIShortCutWnd::OnRBtnDown(int x, int y) {
  UIWindow& window = *this;
  ShortcutGrid& grid = this->m_grid;
  const NormalItemSlot* const* slots = this->m_slots;
  const NormalItemSlot* const* itemSlots = this->m_itemSlots;
  const int column = grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);

  if (column == -1) {
    return;
  }

  const int index = 9 * g_session.m_hotkeyPage + column;
  const int limit = 9 * grid.rows;

  if (limit <= index) {
    if (!g_session.ItemSlotActive()) {
      return;
    }

    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(itemSlots[column]->itemId), 1);

    if (item.identity[1] != 0) {
      UIWindow* detail = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      detail->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }

    return;
  }

  const NormalItemSlot* slot = slots[index];

  if (slot->itemId == 0) {
    return;
  }

  if (slot->enabled == 0) {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot->itemId), 1);

    if (item.identity[1] != 0) {
      UIWindow* detail = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
      detail->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }

    return;
  }

  CSkillInfo skill = g_session.LookupSkill(slot->itemId);

  if (!skill.MatchesShortcut(slot)) {
    return;
  }

  UIWindow* detail = g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND);

  if (detail) {
    if (static_cast<std::uint32_t>(static_cast<UISkillDescribeWnd*>(detail)->m_skillId) == skill.id) {
      g_windowMgr.CloseWindow(WID_SKILLDESCRIBEWND);
    } else {
      detail->SendMsg(nullptr, UM_SKILLDESCRIBE, skill.id, 0, 0, 0);
    }
  }

  int originX = 0;
  int originY = 0;
  window.ToScreen(originX, originY);
  const int targetX = (originX + x);
  int adjustment = 0;

  if (static_cast<unsigned>(g_configuredScreenWidth) - static_cast<unsigned>(targetX) < static_cast<unsigned>(detail->geometry.width)) {
    adjustment = targetX + detail->geometry.width - static_cast<int>(static_cast<unsigned>(g_configuredScreenWidth));
  }

  const int rowY = 33 * (y / 33);
  const int targetY =
    static_cast<float>(originY) <= static_cast<float>(static_cast<unsigned>(g_configuredScreenHeight)) * 0.5f ? ((originY + rowY) + 33) : ((originY + rowY) - detail->geometry.height);

  detail->Move(targetX - adjustment, targetY);
}
