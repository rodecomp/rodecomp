#include "Client/Session.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "UI/Shortcuts/ShortcutBindings.h"
#include "UI/Shortcuts/ShortcutControlEvents.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Shortcuts/ShortcutDetails.h"
#include "UI/Shortcuts/ShortcutDrop.h"
#include "UI/Shortcuts/ShortcutLayout.h"
#include "UI/Shortcuts/ShortcutPainting.h"
#include "UI/Shortcuts/ShortcutPress.h"
#include "UI/Shortcuts/ShortcutTooltip.h"
#include "UI/Shortcuts/ShortcutUse.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatMiniWnd.h"
#include "UI/Dialogs/UIMemorialDunWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindowCallBtnWnd.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Render/Renderer.h"

#include <stdio.h>
#include <cstdlib>

namespace {}

// GLOBAL: gl_24737
int g_memorialDunWndX;

// GLOBAL: gl_24738
int g_memorialDunWndY;

// OFFSET: 0x009122B0
// FUNCTION: fn_12528
UIShortCutWnd::UIShortCutWnd(UIWindowContext& windows, UIFrameContext& frames, ShortcutWindowContext& shortcuts)
    : UIFrameWnd(windows, frames, 0), m_shortcuts(&shortcuts) {
  this->m_controls.decrease = nullptr;
  this->m_controls.increase = nullptr;
}

// OFFSET: 0x00915D90
// FUNCTION: fn_12577
UIShortCutWnd::~UIShortCutWnd() = default;


// OFFSET: 0x0091E800
// FUNCTION: fn_12629
bool UIShortCutWnd::Slot03() {
  return true;
}

// OFFSET: 0x0091EA40
// FUNCTION: fn_12632
void UIShortCutWnd::Move(int x, int y) {
  this->geometry.x = x;
  this->geometry.y = y;
  this->m_frameContext->snapping.Constrain(this, x, y, 0);
  this->geometry.x = (this->geometry.x + x);
  const int top = (this->geometry.y + y);
  this->geometry.y = top < 0 ? 0 : top;
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x00917640
void UIShortCutWnd::Slot10(int x, int y, std::intptr_t value) {
  this->m_shortcuts->drop.Drop(*this, this->m_grid, x, y, *reinterpret_cast<const ModeDragState*>(value));
}

// OFFSET: 0x00940FE0
// FUNCTION: fn_12787
void UIShortCutWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
  }
}

// OFFSET: 0x0093D220
// FUNCTION: fn_12766
void UIShortCutWnd::Show(int visible) {
  this->geometry.visible = visible;

  if (visible == 0) {
    if (this->m_frameContext->snapping.enabled) {
      this->m_frameContext->snapping.rectangles.erase(this);
    }

    return;
  }

  this->SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->Invalidate();
  this->m_frameContext->snapping.Store(this);
}

// OFFSET: 0x0092A190
void UIShortCutWnd::Slot15(int, int) {
  this->m_controls =
    ShortcutControls::Create(*this, *this->m_context, this->m_shortcuts->buttons, g_session.GetOption(10), this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x00932F30
// FUNCTION: fn_12720
void UIShortCutWnd::Slot19() {
  const int x = (this->m_shortcuts->cursorX - this->geometry.x);
  const int y = (this->m_shortcuts->cursorY - this->geometry.y);
  this->m_grid.hovered = this->m_grid.HitTest(x, y, this->m_shortcuts->itemSlotActive, this->m_shortcuts->currentRow);
  this->Invalidate();
}

// OFFSET: 0x0092F8E0
void UIShortCutWnd::Paint() {
  this->m_shortcuts->painting.Paint(*this, this->m_grid, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x009310E0
void UIShortCutWnd::Slot25(int x, int y) {
  this->m_shortcuts->press.Press(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x00930900
void UIShortCutWnd::Slot26(int x, int y) {
  ActivateShortcut(
    *this, this->m_grid, x, y, this->m_shortcuts->itemSlotActive, this->m_shortcuts->currentRow, this->m_slots, this->m_shortcuts->automatic
  );
}

// OFFSET: 0x00932050
void UIShortCutWnd::Slot28(int x, int y) {
  this->m_shortcuts->tooltip.Hover(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x009316F0
// FUNCTION: fn_12705
void UIShortCutWnd::Slot31(int x, int y) {
  this->UIFrameWnd::Slot31(x, y);
  this->Slot11();
}

// OFFSET: 0x009332D0
void UIShortCutWnd::Slot32(int x, int y) {
  this->m_shortcuts->details.Open(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// INLINED
inline void ShortcutControlEvents::Click(int id) {
  switch (id) {
  case 201:
    this->visibility.Show(0);
    return;

  case 216:
    if (this->window.geometry.visible == 0) {
      this->visibility.Show(1);
      return;
    }

    this->grid.rows = this->grid.rows + 1;

    if (this->grid.rows > 4) {
      this->grid.rows = 1;
      this->visibility.Show(0);
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 217:
    if (this->grid.rows > 1) {
      --this->grid.rows;
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 549: {
    const bool checked = g_session.GetOption(10) == 0;
    g_session.SetOption(10, checked ? 1 : 0, 0);
    this->controls.bank->geometry.unknown_0030 = checked ? 1 : 0;
    this->controls.bank->Invalidate();

    for (ShortcutCooldownRecord& record : this->cooldowns.records) {
      record.main.clear();
    }

    this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
    return;
  }

  case 588:
  case 589: {
    this->itemSlotActive = id == 588;
    const bool active = this->itemSlotActive;

    this->controls.showItems->Enable(active ? 0 : 1);
    this->controls.showItems->SetDisabled(active ? 1 : 0);
    this->controls.showItems->Invalidate();

    this->controls.hideItems->Enable(active ? 1 : 0);
    this->controls.hideItems->SetDisabled(active ? 0 : 1);
    this->controls.hideItems->Invalidate();

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;
  }

  default:
    return;
  }
}

// INLINED
inline void ShortcutLayout::PlaceRowButtons() {
  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
}

// INLINED
inline void ShortcutLayout::ResizeRows() {
  const int height = (this->itemSlotActive ? 43 : 10) + this->grid.rows * 33;
  if (height == this->window.geometry.height) {
    return;
  }

  this->window.Resize(this->window.geometry.width, height);

  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
  this->window.Slot11();
}

// INLINED
inline void ShortcutLayout::Restore(NormalItemSlotWindow* value) {
  this->saved = value;
  if (!value->IsValid(this->screenWidth, this->screenHeight, this->itemSlotActive)) {
    value->Reset();
  }

  const int height = value->height;
  this->window.Resize(this->window.geometry.width, height);
  const int extent = this->itemSlotActive ? height - 43 : height;
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->showItems.Enable(this->itemSlotActive ? 0 : 1);
  this->showItems.SetDisabled(this->itemSlotActive ? 1 : 0);
  this->showItems.Invalidate();
  this->hideItems.Enable(this->itemSlotActive ? 1 : 0);
  this->hideItems.SetDisabled(this->itemSlotActive ? 0 : 1);
  this->hideItems.Invalidate();
  this->grid.rows = extent / 33;
  this->PlaceRowButtons();
  this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->window.Move(value->x, value->y);
}

// OFFSET: 0x00940FE0
inline void ShortcutLayout::Persist() {
  if (!this->saved) {
    return;
  }

  this->saved->x = this->window.geometry.x;
  this->saved->y = this->window.geometry.y;
  this->saved->width = this->window.geometry.width;
  this->saved->height = this->window.geometry.height;
}

// INLINED
inline void ShortcutUse::Use(const NormalItemSlot& slot) {
  if (g_session.GetOption(5) > 0 || slot.itemId == 0) {
    return;
  }

  if (slot.unknown_0000 != 0) {
    CSkillInfo skill = this->skills.Lookup(slot.itemId);
    if (skill.MatchesShortcut(&slot)) {
      int level = slot.unknown_0005;
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      this->mode->SendModeMessage(113, reinterpret_cast<std::intptr_t>(&skill), level, 0, 0);
    }

    return;
  }

  ITEM_INFO item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  ModeEvents* mode = this->mode;

  const auto use = [&]() { mode->SendModeMessage(27, item.unknown_0000[1], 0, 0, 0); };

  switch (item.unknown_0000[0]) {
  case 0:
  case 1:
  case 2:
    if (item.GetItemId() != 12802 || this->dialogs.Confirm(this->naming.Message(2373), 2, 0, 0, "Message", 280, 120) != 188) {
      use();
    }

    break;

  case 10:
  case 16:
  case 17:
  case 19:
    if (item.unknown_0000[3] == 0x8000) {
      mode->SendModeMessage(87, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;

  case 18:
    if (this->itemConfirmationState >= 0) {
      const std::string name = this->naming.DisplayName(item, 0);
      char text[4096];

      _snprintf_s(text, sizeof(text), _TRUNCATE, this->naming.Message(1226), name.c_str());

      if (this->dialogs.Confirm(text, 2, 0, 0, "Message", 280, 120) == 187) {
        use();
      }
    }

    break;

  default:
    if (item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0 && item.identified != 0) {
      mode->SendModeMessage(19, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;
  }
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
int UIShortCutWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  ShortcutWindowContext& owner = *this->m_shortcuts;

  switch (message) {
  case 6: {
    ShortcutControlEvents events{*this, this->m_grid, this->m_controls, owner.visibility, owner.bindings.cooldowns, owner.itemSlotActive};
    events.Click(static_cast<int>(a));
    return 0;
  }

  case 14: {
    ShortcutLayout layout{
      *this,
      this->m_grid,
      this->m_saved,
      *this->m_controls.decrease,
      *this->m_controls.increase,
      *this->m_controls.showItems,
      *this->m_controls.hideItems,
      owner.itemSlotActive,
      this->m_frameContext->snapping.screenWidth,
      this->m_frameContext->snapping.screenHeight
    };
    layout.ResizeRows();
    return 0;
  }

  case 23:
    owner.bindings.Refresh(this->m_slots, this->m_itemSlots);
    this->Invalidate();
    return 0;

  case 34: {
    ShortcutLayout layout{
      *this,
      this->m_grid,
      this->m_saved,
      *this->m_controls.decrease,
      *this->m_controls.increase,
      *this->m_controls.showItems,
      *this->m_controls.hideItems,
      owner.itemSlotActive,
      this->m_frameContext->snapping.screenWidth,
      this->m_frameContext->snapping.screenHeight
    };
    layout.Restore(reinterpret_cast<NormalItemSlotWindow*>(a));
    return 0;
  }

  case 41: {
    const int index = 9 * static_cast<int>(b) + static_cast<int>(a);
    owner.use.Use(*this->m_slots[index]);
    return 0;
  }

  case 143:
    if (owner.bindings.cooldowns.RotateRows(static_cast<int>(a), static_cast<int>(b), owner.rotation0, owner.rotation1, owner.clock)) {
      this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    }

    return 0;

  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0091E9C0
// FUNCTION: fn_12631
void UINewChatWnd::Move(int x, int y) {
  this->UIFrameWnd::Move(x, y);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  this->m_inputX = screenX + 3;
  this->m_inputY = this->m_inputOffsetY + screenY;

  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_03F0)) {
    window->SendMsg(this, 6, 197, this->geometry.x + x, this->geometry.y + y, 0);
  }
}

// OFFSET: 0x0093D090
// FUNCTION: fn_12765
void UINewChatWnd::Show(int visible) {
  this->geometry.visible = visible;

  if (visible != 0) {
    this->Invalidate();
    this->m_frameContext->snapping.Store(this);
  } else if (this->m_frameContext->snapping.enabled) {
    this->m_frameContext->snapping.rectangles.erase(this);
  }

  for (UIWindow* tab : this->m_tabs) {
    tab->Show(this->geometry.visible);
  }

  for (unsigned int i = 0; i < g_windowMgr.retainedWindows.size(); ++i) {
    g_windowMgr.retainedWindows[i]->Show(this->geometry.visible);
  }
}
// OFFSET: 0x0091E960
// FUNCTION: fn_12630
void UIMemorialDunWnd::Move(int x, int y) {
  const int left = x <= g_renderer->viewport.width - this->geometry.width ? x : 0;
  const int top = y <= g_renderer->viewport.height - this->geometry.height ? y : 310;
  this->UIFrameWnd::Move(left, top);

  if (g_memorialDunWndX != left) {
    g_memorialDunWndX = left;
  }
  if (g_memorialDunWndY != top) {
    g_memorialDunWndY = top;
  }
}

// OFFSET: 0x0093CA50
// FUNCTION: fn_12762
int UISubChatMiniWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  if (a == 197) {
    this->Move(b, c - this->m_offsetY);
  } else if (a == 202) {
    this->Slot25(this->m_rowHeight * b + 1, 0);
  }
  return 0;
}

// OFFSET: 0x0093CAB0
// FUNCTION: fn_12763
int UIWindowCallBtnWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    this->m_frameContext->windows.EnsureWindow(a);
  }
  return 0;
}
