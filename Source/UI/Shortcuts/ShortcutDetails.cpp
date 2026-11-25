#include "Client/Session/Session.h"
#include "Contents/Items/ItemId.h"
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
  const int column = this->HitTestSlot(x, y);

  if (column == -1) {
    return;
  }

  const int index = column + 9 * g_session.m_hotkeyPage;

  if (9 * this->m_grid.rows <= index) {
    if (g_session.ItemSlotActive()) {
      ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(this->m_itemSlots[column]->itemId), 1);

      if (item.identity[1] > 0) {
        g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
      }
    }

    return;
  }

  if (this->m_slots[index]->itemId == ITEMID_NONE) {
    return;
  }

  if (this->m_slots[index]->enabled) {
    CSkillInfo skill = g_session.LookupSkill(this->m_slots[index]->itemId);

    if (this->MatchesShortcut(skill, this->m_slots[index])) {
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
      unsigned adjustment = 0;
      const unsigned screenWidth = g_configuredScreenWidth;
      const unsigned screenHeight = g_configuredScreenHeight;
      const unsigned targetX = static_cast<unsigned>(x) + originX;

      if (screenWidth - targetX < static_cast<unsigned>(detail->geometry.width)) {
        adjustment = detail->geometry.width - (screenWidth - targetX);
      }

      if (originY > screenHeight * 0.5f) {
        detail->Move(originX - adjustment + x, static_cast<unsigned>(y / 33) * 33 - detail->geometry.height + originY);
      } else {
        detail->Move(originX - adjustment + x, originY + 33u + static_cast<unsigned>(y / 33) * 33);
      }
    }
  } else {
    ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(this->m_slots[index]->itemId), 1);

    if (item.identity[1] > 0) {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}
