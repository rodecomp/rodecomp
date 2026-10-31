#include "UI/Dialogs/UIPasswordWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"

// GLOBAL: gl_25104
int g_passwordWndMode;

// OFFSET: 0x0090C970
// FUNCTION: fn_12451
void UIPasswordWnd::sub_90C970(int value) {
  this->unknown_011C = value;
  if (value == 0) {
    this->unknown_0164->SetText(LookupMessage(0xa06), 0);
  }
}

// OFFSET: 0x0090C9B0
// FUNCTION: fn_12452
void UIPasswordWnd::sub_90C9B0(int) {}

// OFFSET: 0x0090DE10
// FUNCTION: fn_12454
void UIPasswordWnd::sub_90DE10() {
  UIButton** slot = this->unknown_0124;
  for (int remaining = 11; remaining != 0; --remaining) {
    UIButton* button = *slot;
    button->SetDisabled(0);
    button->Invalidate();
    ++slot;
  }

  slot = this->unknown_0150;
  for (int remaining = 5; remaining != 0; --remaining) {
    UIButton* button = *slot;
    button->SetDisabled(0);
    button->Invalidate();
    ++slot;
  }

  if (g_passwordWndMode == 4 || g_passwordWndMode == 5) {
    UIButton* button = this->unknown_0150[1];
    button->SetDisabled(1);
    button->Invalidate();
    button = this->unknown_0150[2];
    button->SetDisabled(1);
    button->Invalidate();
  }
}
