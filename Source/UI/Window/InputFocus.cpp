#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x00AA9B40
// FUNCTION: fn_17256
void UIWindowMgr::SetFocus(UIWindow* value) {
  if (this->hoverWindow) {
    const bool retained = value && value->GetEditWindow() == this->hoverWindow;
    this->hoverWindow->OnFinishEdit(retained);
  }

  if (value) {
    this->hoverWindow = value->GetEditWindow();

    if (this->hoverWindow) {
      this->hoverWindow->OnBeginEdit();
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

    for (auto it = parent->m_children.begin(); it != parent->m_children.end(); ++it) {
      UIWindow* child = *it;
      if (child->IsEditable()) {
        this->SetFocus(child);
        return;
      }
    }

    if (this->hoverWindow) {
      this->hoverWindow->OnFinishEdit(0);
    }
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
      if (child->IsEditable()) {
        this->SetFocus(child);
        return;
      }
    }

    if (this->hoverWindow) {
      this->hoverWindow->OnFinishEdit(0);
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

  UIWindow* next = this->hoverWindow->parent->NextFocusable(this->hoverWindow);
  if (next) {
    if (next->IsEditable()) {
      this->SetFocus(next);
    }
  } else {
    this->SetFocus(nullptr);
  }
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
  if (previous) {
    if (previous->IsEditable()) {
      this->SetFocus(previous);
    }
  } else {
    this->SetFocus(nullptr);
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

// INLINED
void SetPressedWindowRoot(UIWindow*& pressedRoot, UIWindow* window) {
  pressedRoot = window ? window->Root() : nullptr;
}

// OFFSET: 0x00AA9E60
// FUNCTION: fn_17261
void UIWindowMgr::SetPressedWindowRoot(UIWindow* window) {
  UIWindow* root = window;
  if (root != nullptr) {
    while (root->parent != nullptr) {
      root = root->parent;
    }
  }
  this->window_4F1D4 = root;
}
