#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x00AA9B40
// FUNCTION: fn_17256
void UIWindowMgr::SetFocus(UIWindow* value) {
  if (this->hoverWindow) {
    const bool retained = value && value->Slot06() == this->hoverWindow;
    this->hoverWindow->Slot24(retained);
  }

  if (value) {
    this->hoverWindow = value->Slot06();

    if (this->hoverWindow) {
      this->hoverWindow->Slot23();
    }
  } else {
    this->hoverWindow = nullptr;
  }
}

// OFFSET: 0x00AA9BC0
// FUNCTION: fn_17257
void UIWindowMgr::FocusFirst() {
  if (this->hoverWindow) {
    UIWindow* parent = this->hoverWindow->parent;

    for (UIWindow* child : parent->m_children) {
      if (!child->Slot05()) {
        continue;
      }

      this->hoverWindow->Slot24(child->Slot06() == this->hoverWindow);
      this->hoverWindow = child->Slot06();

      if (this->hoverWindow) {
        this->hoverWindow->Slot23();
      }

      return;
    }

    this->hoverWindow->Slot24(0);
  }

  this->hoverWindow = nullptr;
}

// OFFSET: 0x00AA9C70
// FUNCTION: fn_17258
void UIWindowMgr::FocusLast() {
  if (this->hoverWindow) {
    UIWindow* parent = this->hoverWindow->parent;

    for (auto it = parent->m_children.rbegin(); it != parent->m_children.rend(); ++it) {
      UIWindow* child = *it;
      if (child->Slot05()) {
        this->SetFocus(child);
        return;
      }
    }

    if (this->hoverWindow) {
      this->hoverWindow->Slot24(0);
    }
  }

  this->hoverWindow = nullptr;
}

// OFFSET: 0x00AA9D20
// FUNCTION: fn_17259
void UIWindowMgr::FocusNext() {
  if (!this->hoverWindow) {
    return;
  }

  UIWindow* parent = this->hoverWindow->parent;
  bool found = false;

  for (UIWindow* child : parent->m_children) {
    if (found && child->Slot05()) {
      this->hoverWindow->Slot24(child->Slot06() == this->hoverWindow);
      this->hoverWindow = child->Slot06();

      if (this->hoverWindow) {
        this->hoverWindow->Slot23();
      }

      return;
    }

    if (child == this->hoverWindow && child->Slot05()) {
      found = true;
    }
  }

  this->hoverWindow->Slot24(0);
  this->hoverWindow = nullptr;
}

// OFFSET: 0x00A96F60
// FUNCTION: fn_17197
bool UIWindowMgr::IsLastFocus(UIWindow* value) {
  return value && value->parent->NextFocusable(value) == nullptr;
}

// OFFSET: 0x00AA9DC0
// FUNCTION: fn_17260
void UIWindowMgr::FocusPrevious() {
  if (!this->hoverWindow) {
    return;
  }

  UIWindow* previous = this->hoverWindow->parent->PreviousFocusable(this->hoverWindow);
  if (!previous) {
    this->SetFocus(nullptr);
    return;
  }

  if (previous->Slot05()) {
    this->SetFocus(previous);
  }
}

// INLINED
void UIWindowMgr::RefreshFocus(UIWindow* const& excludedRoot) {
  if (this->hoverWindow) {
    this->hoverWindow->Invalidate();
    UIWindow* root = this->hoverWindow->Root();
    if (root && root != excludedRoot) {
      root->Invalidate();
    }
  }
}

// OFFSET: 0x00AA9E60
// FUNCTION: fn_17261
void SetPressedWindowRoot(UIWindow*& pressedRoot, UIWindow* window) {
  pressedRoot = window ? window->Root() : nullptr;
}
