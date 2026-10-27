#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x00AA9B40
// FUNCTION: fn_17256
void InputFocus::Set(UIWindow* value) {
  if (this->window) {
    const bool retained = value && value->Slot06() == this->window;
    this->window->Slot24(retained);
  }

  if (value) {
    this->window = value->Slot06();

    if (this->window) {
      this->window->Slot23();
    }
  } else {
    this->window = nullptr;
  }
}

// OFFSET: 0x00AA9BC0
// FUNCTION: fn_17257
void InputFocus::First() {
  if (this->window) {
    UIWindow* parent = this->window->parent;

    for (UIWindow* child : parent->m_children) {
      if (!child->Slot05()) {
        continue;
      }

      this->window->Slot24(child->Slot06() == this->window);
      this->window = child->Slot06();

      if (this->window) {
        this->window->Slot23();
      }

      return;
    }

    this->window->Slot24(0);
  }

  this->window = nullptr;
}

// OFFSET: 0x00AA9D20
// FUNCTION: fn_17259
void InputFocus::Next() {
  if (!this->window) {
    return;
  }

  UIWindow* parent = this->window->parent;
  bool found = false;

  for (UIWindow* child : parent->m_children) {
    if (found && child->Slot05()) {
      this->window->Slot24(child->Slot06() == this->window);
      this->window = child->Slot06();

      if (this->window) {
        this->window->Slot23();
      }

      return;
    }

    if (child == this->window && child->Slot05()) {
      found = true;
    }
  }

  this->window->Slot24(0);
  this->window = nullptr;
}

// INLINED
void InputFocus::Advance() {
  if (!this->window) {
    this->Next();
    return;
  }

  UIWindow* parent = this->window->parent;
  bool found = false;

  for (UIWindow* child : parent->m_children) {
    if (found && child->Slot05()) {
      this->Next();
      return;
    }

    if (child == this->window && child->Slot05()) {
      found = true;
    }
  }

  this->First();
}

// INLINED
void InputFocus::Refresh(UIWindow* const& excludedRoot) {
  if (this->window) {
    this->window->Invalidate();
    UIWindow* root = this->window->Root();
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
