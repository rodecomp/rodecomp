#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A81C90
// FUNCTION: fn_16985
bool UIWindow::IsDescendantOf(const UIWindow* ancestor) const {
  for (const UIWindow* node = this->parent; node; node = node->parent) {
    if (node == ancestor) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x00A8C200
// FUNCTION: fn_17130
void UIWindowMgr::ForgetWindowReferences(UIWindow* removed) {
  if (this->inputWindow && (this->inputWindow == removed || this->inputWindow->IsDescendantOf(removed))) {
    this->inputWindow = nullptr;
  }

  if (this->hoverWindow && (this->hoverWindow == removed || this->hoverWindow->IsDescendantOf(removed))) {
    this->hoverWindow = nullptr;
  }

  if (this->window_01B8 && (this->window_01B8 == removed || this->window_01B8->IsDescendantOf(removed))) {
    this->window_01B8 = nullptr;
  }

  if (this->window_0A30 && this->window_0A30 == removed) {
    this->window_0A30 = nullptr;
  }

  if (this->window_4F1D4 && (this->window_4F1D4 == removed || this->window_4F1D4->IsDescendantOf(removed))) {
    this->window_4F1D4 = nullptr;
  }
}

// OFFSET: 0x00AA8F30
// FUNCTION: fn_17250
void UIWindowMgr::ClearWindow01B8() {
  this->window_01B8 = nullptr;
}
