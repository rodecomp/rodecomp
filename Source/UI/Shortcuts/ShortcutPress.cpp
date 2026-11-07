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
  ShortcutGrid& grid = this->m_grid;
  const NormalItemSlot* const* slots = this->m_slots;
  const NormalItemSlot* const* itemSlots = this->m_itemSlots;
  const int columnOffset = x - 15;
  if (columnOffset >= 261 || x < 15 || y < 4) {
    this->UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (g_replayAnimationScaling) {
    return;
  }

  const int column = grid.HitTest(x, y, g_session.ItemSlotActive(), g_session.m_hotkeyPage);
  if (column == -1) {
    return;
  }

  grid.hovered = column;
  const int index = 9 * g_session.m_hotkeyPage + column;
  if (9 * grid.rows > index) {
    const NormalItemSlot& slot = *slots[index];
    if (slot.itemId == 0) {
      return;
    }

    if (slot.unknown_0000 != 0) {
      CSkillInfo skill = g_session.LookupSkill(slot.itemId);
      if (skill.MatchesShortcut(&slot) && g_modeMgr.GetGameMode()) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginSkill(g_windowMgr, ResourceMgr::Instance(), 7, skill.id, skill.name, slot.unknown_0005, index);
      }
    } else {
      ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
      if (item.unknown_0000[1] != 0 && g_modeMgr.GetGameMode()) {
        static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 7, item, item.quantity, -1);
      }
    }
  } else if (g_session.ItemSlotActive()) {
    const NormalItemSlot& slot = *itemSlots[column];
    if (slot.itemId == 0) {
      return;
    }

    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
    if (item.unknown_0000[1] != 0 && g_modeMgr.GetGameMode()) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(g_windowMgr, 7, item, item.quantity, -1);
    }
  }
}
