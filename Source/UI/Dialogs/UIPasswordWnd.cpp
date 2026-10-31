#include "UI/Dialogs/UIPasswordWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Text/UIStaticText.h"

// OFFSET: 0x0090C970
// FUNCTION: fn_12451
void UIPasswordWnd::sub_90C970(int value) {
  this->unknown_011C = value;
  if (value == 0) {
    this->unknown_0164->SetText(LookupMessage(0xa06), 0);
  }
}
