#include "UI/UIBartermarketWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00845940

// OFFSET: 0x008459F0

// OFFSET: 0x00849030
// FUNCTION: fn_10594
void UIExpandedBarterItemPurchaseWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}

// OFFSET: 0x00849080
// FUNCTION: fn_10595
void UIExpandedBartermarketWnd::Slot35(int delta) {
  if (this->scrollEnabled == 1) {
    int step = this->rowCount - 1;
    if (step > 3) {
      step = 3;
    }
    this->SendMsg(this->scrollBar, 7, delta > 0 ? -step : step, 0, 0, 0);
  }
}
