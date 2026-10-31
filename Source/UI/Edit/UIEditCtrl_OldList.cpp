#include "UI/Edit/UIEditCtrl.h"

// OFFSET: 0x0084FCC0
// FUNCTION: fn_10664
UIEditCtrl_OldList::UIEditCtrl_OldList(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit), historyIndex(-1) {
  this->textY = 3;
}
