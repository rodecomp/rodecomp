#include "Client/Session.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Resource/ResourceMgr.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIFrameWnd.h"

// OFFSET: 0x009310E0
// FUNCTION: fn_12699
void UIShortCutWnd::OnLBtnDown(int x, int y) {
  if (x - 15 >= 261 || x < 15 || y < 4) {
    this->UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (g_replayAnimationScaling) {
    return;
  }

  const int column = this->m_grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);
  if (column == -1) {
    return;
  }

  this->m_grid.hovered = column;
  const int index = 9 * g_session.m_hotkeyPage + column;
  if (9 * this->m_grid.rows <= index) {
    if (!g_session.ItemSlotActive()) {
      return;
    }

    if (this->m_itemSlots[column]->itemId == 0) {
      return;
    }

    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(this->m_itemSlots[column]->itemId), 1);
    if (item.identity[1] > 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      if (mode) {
        mode->drag.BeginItem(g_windowMgr, 7, item, item.quantity, -1);
      }
    }
  } else {
    if (this->m_slots[index]->itemId == 0) {
      return;
    }

    if (this->m_slots[index]->enabled != 0) {
      CSkillInfo skill = g_session.LookupSkill(this->m_slots[index]->itemId);
      if (skill.MatchesShortcut(this->m_slots[index])) {
        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        if (mode) {
          mode->drag.BeginSkill(g_windowMgr, ResourceMgr::Instance(), 7, skill.id, skill.name, this->m_slots[index]->value, index);
        }
      }
    } else {
      ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(this->m_slots[index]->itemId), 1);
      if (item.identity[1] > 0) {
        CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
        mode->drag.BeginItem(g_windowMgr, 7, item, item.quantity, -1);
      }
    }
  }
}
