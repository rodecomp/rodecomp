#include "UI/Lists/UITagListBox.h"

// OFFSET: 0x009B7A10
// FUNCTION: fn_13869
void UITagListBox::AddLine(const char* text) {
  this->UIListBox::AddLine(text);
}

// OFFSET: 0x009B81C0
// FUNCTION: fn_13871
void UITagListBox::Clear() {
  this->UIListBox::Clear();
  for (UIWindow* window : this->m_tagWindows) {
    this->RemoveChild(window, 1);
  }
  this->m_tagWindows.clear();
  this->Invalidate();
}

// OFFSET: 0x009B9E80
// FUNCTION: fn_13879
void UITagListBox::Slot25(int, int) {
}

// OFFSET: 0x009B9E70
// FUNCTION: fn_13878
void UITagListBox::Slot26(int, int) {
}

// OFFSET: 0x009B9E90
// FUNCTION: fn_13880
void UITagListBox::Slot31(int, int) {
}
