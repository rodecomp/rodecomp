#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"

// OFFSET: 0x00861AB0
// FUNCTION: fn_10902
void UIEditCtrl3::UpdateScrollBar() {
  const int visible = this->geometry.height / this->unknown_0138;
  const int total = this->unknown_013C;
  int maximum = total - visible;
  if (maximum == 1 && this->unknown_0158 == 0) {
    this->unknown_0134 = 0;
    static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(0, visible, total);
    this->unknown_0158 = 0;
  } else {
    if (maximum < 0) {
      maximum = 0;
    }
    this->unknown_0134 = maximum > 0;
    if (maximum > 0) {
      static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(maximum, visible, total);
      static_cast<UIScrollBar*>(this->unknown_0130)->SetPosition(this->unknown_0158);
    } else {
      static_cast<UIScrollBar*>(this->unknown_0130)->SetRange(0, visible, total);
      this->unknown_0158 = 0;
    }
  }

  this->unknown_0130->Invalidate();
}
