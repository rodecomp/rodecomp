#include "UI/UILapineUpgradeBoxWnd.h"

#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeEvents.h"
#include "Contents/Items/ItemDescription.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/UILapineScrollBar.h"
#include "UI/Window/WindowManagerWindows.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x006035E0
// FUNCTION: fn_04194
void UILapineUpgradeBoxWnd::Slot10(int x, int y, std::intptr_t value) {
  ModeEvents* mode = this->m_context->mode;
  if (this->item.unknown_0000[1] > 0) {
    mode->SendModeMessage(18, 0, 0, 0, 0);
    return;
  }
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 27) {
    this->SendMsg(this, 31, drag->id, 0, 0, 0);
  }
  mode->SendModeMessage(18, 0, 0, 0, 0);
}

// OFFSET: 0x00603650
// FUNCTION: fn_04195
void UILapineUpgradeBox_IngredientInfoWnd::Slot10(int x, int y, std::intptr_t value) {
  if (this->parent) {
    this->parent->Slot10(x + this->geometry.x, y + this->geometry.y, value);
  }
}

// OFFSET: 0x006036D0
// FUNCTION: fn_04197
void UILapineUpgradeBox_IngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00603DB0
// FUNCTION: fn_04203
void UILapineUpgradeBoxWnd::Slot26(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    this->SendMsg(this, 104, this->item.unknown_0000[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603DF0
// FUNCTION: fn_04204
void UILapineUpgradeBox_IngredientInfoWnd::Slot26(int x, int y) {
  if (this->item.unknown_0000[1] > 0) {
    this->parent->SendMsg(this, 104, this->item.unknown_0000[1], 0, 0, 0);
  }
}

// OFFSET: 0x00603E20
// FUNCTION: fn_04205
void UILapineUpgradeBoxWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_world.ActiveGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 26, this->item, 1, -1);
  }
}

// OFFSET: 0x00603E80
// FUNCTION: fn_04206
void UILapineUpgradeBox_IngredientInfoWnd::Slot25(int x, int y) {
  if (this->item.unknown_0000[1] != 0) {
    ItemDescriptions descriptions{g_itemDescriptions};
    static_cast<CGameMode*>(g_world.ActiveGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 27, this->item, this->quantity, -1);
  }
}

// OFFSET: 0x00603EC0
// FUNCTION: fn_04207
void UILapineUpgradeBox_IngredientInfoWnd::Slot31(int x, int y) {
  this->selected = 1;
  this->Invalidate();
  if (this->parent) {
    this->parent->SendMsg(this, 6, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00604180
// FUNCTION: fn_04213
void UILapineUpgradeBox_IngredientInfoWnd::Slot35(int argument) {
  if (this->parent) {
    this->parent->Slot35(argument);
  }
}

// OFFSET: 0x006040E0
// FUNCTION: fn_04211
void UILapineUpgradeBoxWnd::Slot32(int x, int y) {
  if (static_cast<unsigned>(x - 101) <= 24 && static_cast<unsigned>(y - 165) <= 24 && this->item.unknown_0000[1] != 0) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00604140
// FUNCTION: fn_04212
void UILapineUpgradeBox_IngredientInfoWnd::Slot32(int x, int y) {
  if (this->item.unknown_0000[1] != 0) {
    UIWindow* window = this->m_frameContext->windows.EnsureWindow(12);
    window->SendMsg(this, 24, reinterpret_cast<std::intptr_t>(&this->item), 0, 0, 0);
  }
}

// OFFSET: 0x00605BB0
// FUNCTION: fn_04225
void UILapineUpgradeBoxWnd::SetWaiting(unsigned char value) {
  this->waiting = value;
  this->upgradeButton->SetDisabled(value);
  this->upgradeButton->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x00603680
// FUNCTION: fn_04196
void UILapineUpgradeBox_IngredientListWnd::Slot10(int x, int y, std::intptr_t value) {
  CMode* mode = g_world.ActiveGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 26) {
    this->SendMsg(this, 31, drag->id, 0, 0, 0);
  }
  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00603DA0
// FUNCTION: fn_04202
void UILapineUpgradeBox_IngredientListWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
}

// OFFSET: 0x006041A0

// OFFSET: 0x00605980

// OFFSET: 0x00603FE0
// FUNCTION: fn_04210
void UILapineUpgradeBoxWnd::Slot19() {
  UILapineUpgradeBox_IngredientInfoWnd* selected = this->FindSelected();
  if (!selected) {
    if (this->selectButton->Disabled() == 0) {
      this->selectButton->SetDisabled(1);
      this->selectButton->Invalidate();
      this->Invalidate();
    }
  } else if (this->selectButton->Disabled() == 1 && this->item.unknown_0000[1] == 0) {
    this->selectButton->SetDisabled(0);
    this->selectButton->Invalidate();
    this->Invalidate();
  }

  if (this->waiting == 0) {
    if (this->item.unknown_0000[1] != 0) {
      if (this->upgradeButton->Disabled() == 1) {
        this->upgradeButton->SetDisabled(0);
        this->upgradeButton->Invalidate();
        this->Invalidate();
      }
    } else if (this->upgradeButton->Disabled() == 0) {
      this->upgradeButton->SetDisabled(1);
      this->upgradeButton->Invalidate();
      this->Invalidate();
    }
  }

  if (this->noticeTick + this->noticeDuration < timeGetTime()) {
    this->notice = "";
  }
}
