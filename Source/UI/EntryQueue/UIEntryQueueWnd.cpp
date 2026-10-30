#include "UI/EntryQueue/UIEntryQueueWnd.h"

// OFFSET: 0x00BA7B90
// FUNCTION: fn_19713
void UIEntryQueueWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}
