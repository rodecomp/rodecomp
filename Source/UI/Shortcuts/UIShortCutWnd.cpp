#include "Client/ClientOptions.h"
#include "Client/Mode/ModeDragState.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "UI/Shortcuts/ShortcutBindings.h"
#include "UI/Shortcuts/ShortcutControlEvents.h"
#include "UI/Shortcuts/ShortcutDetails.h"
#include "UI/Shortcuts/ShortcutDrop.h"
#include "UI/Shortcuts/ShortcutLayout.h"
#include "UI/Shortcuts/ShortcutPainting.h"
#include "UI/Shortcuts/ShortcutPress.h"
#include "UI/Shortcuts/ShortcutTooltip.h"
#include "UI/Shortcuts/ShortcutUse.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Layout/WindowSnapping.h"

namespace {}

// OFFSET: 0x009122B0
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
// FUNCTION: fn_12589
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
// FUNCTION: fn_12654
void UIShortCutWnd::Slot15(int, int) {
  this->m_controls =
    ShortcutControls::Create(*this, *this->m_context, this->m_shortcuts->buttons, this->m_shortcuts->options.Get(10), this->m_slots, this->m_itemSlots);
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
// FUNCTION: fn_12680
void UIShortCutWnd::Paint() {
  this->m_shortcuts->painting.Paint(*this, this->m_grid, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x009310E0
// FUNCTION: fn_12699
void UIShortCutWnd::Slot25(int x, int y) {
  this->m_shortcuts->press.Press(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x00930900
// FUNCTION: fn_12689
void UIShortCutWnd::Slot26(int x, int y) {
  ActivateShortcut(
    *this, this->m_grid, x, y, this->m_shortcuts->itemSlotActive, this->m_shortcuts->currentRow, this->m_slots, this->m_shortcuts->automatic,
    this->m_shortcuts->options
  );
}

// OFFSET: 0x00932050
// FUNCTION: fn_12711
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
// FUNCTION: fn_12724
void UIShortCutWnd::Slot32(int x, int y) {
  this->m_shortcuts->details.Open(*this, this->m_grid, x, y, this->m_slots, this->m_itemSlots);
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
int UIShortCutWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  ShortcutWindowContext& owner = *this->m_shortcuts;

  switch (message) {
  case 6: {
    ShortcutControlEvents events{*this, this->m_grid, this->m_controls, owner.visibility, owner.options, owner.bindings.cooldowns, owner.itemSlotActive};
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
