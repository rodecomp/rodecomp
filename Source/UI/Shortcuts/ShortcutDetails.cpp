#include "Client/Session/Session.h"
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
  const int column = this->m_grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);

  if (column == -1) {
    return;
  }

  const int index = column + 9 * g_session.m_hotkeyPage;

  if (index >= 9 * this->m_grid.rows) {
    if (g_session.ItemSlotActive()) {
      ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(this->m_itemSlots[column]->itemId), 1);

      if (item.identity[1] > 0) {
        g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      }
    }

    return;
  }

  if (this->m_slots[index]->itemId == 0) {
    return;
  }

  if (this->m_slots[index]->enabled) {
    CSkillInfo skill = g_session.LookupSkill(this->m_slots[index]->itemId);

    if (skill.MatchesShortcut(this->m_slots[index])) {
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
      this->ToScreen(originX, originY);
      const int targetX = originX + x;
      int adjustment = 0;

      if (static_cast<unsigned>(g_configuredScreenWidth) - targetX < static_cast<unsigned>(detail->geometry.width)) {
        adjustment = detail->geometry.width - (static_cast<unsigned>(g_configuredScreenWidth) - targetX);
      }

      const int rowY = y / 33 * 33;
      detail->Move(
        targetX - adjustment,
        static_cast<float>(originY) > static_cast<unsigned>(g_configuredScreenHeight) * 0.5f ? rowY - detail->geometry.height + originY
                                                                                              : originY + 33 + rowY);
    }
  } else {
    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(this->m_slots[index]->itemId), 1);

    if (item.identity[1] > 0) {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}
