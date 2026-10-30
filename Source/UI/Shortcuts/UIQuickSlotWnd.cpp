#include "UI/Shortcuts/UIQuickSlotWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Replay.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Skills/ClientSkills.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/WindowManagerWindows.h"

// OFFSET: 0x00930EB0
// FUNCTION: fn_12696
void UIQuickSlotWnd::Slot25(int x, int y) {
  if (x > 59 && x < this->geometry.width) {
    this->UIFrameWnd::Slot25(x, y);
    return;
  }

  if (g_replayAnimationScaling != 0 || static_cast<unsigned int>(y) > 23) {
    return;
  }

  if (x > 58 && x < this->geometry.width) {
    return;
  }

  const int index = (x - 4) / 29;
  if (x > index * 29 + 30 || index == -1) {
    return;
  }

  QuickSlotPainting& painting = *this->m_painting;
  const QuickSlotEntry entry = this->m_entries[index];
  if (entry.skill) {
    CSkillInfo skill = painting.skills.Lookup(entry.id);
    if (skill.available && skill.level) {
      if (CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode())) {
        mode->drag.BeginSkill(this->m_frameContext->windows, ResourceMgr::Instance(), 16, skill.id, skill.name, entry.count, index);
      }
    }
  } else if (entry.id != 0) {
    ITEM_INFO item = LookupClientItem(painting.inventory, static_cast<int>(entry.id), 1);
    if (item.unknown_0000[1] > 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      mode->drag.BeginItem(this->m_frameContext->windows, painting.descriptions, 16, item, item.quantity, -1);
    }
  }
}
