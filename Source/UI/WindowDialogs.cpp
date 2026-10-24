#include "UI/UIMessageBox.h"
#include "UI/WindowDialogs.h"
#include "UI/WindowRetirement.h"

// OFFSET: 0x00A91450
// FUNCTION: fn_17160
UIWindow* WindowDialogs::Top() const {
  return this->modal.empty() ? nullptr : this->modal.back();
}

// OFFSET: 0x00A91120
// FUNCTION: fn_17155
void WindowDialogs::RestoreInput() {
  for (UIWindow* window : this->dialog.retirement.rendered) {
    if (window->geometry.visible == 0) {
      continue;
    }

    for (UIWindow* child : window->m_children) {
      if (!child->Slot05() || child->geometry.x <= 0 || child->geometry.y <= 0) {
        continue;
      }

      if (this->input) {
        static_cast<UIWindow*>(this->input)->Slot24(child->Slot06() == this->input);
      }

      this->input = child->Slot06();

      if (this->input) {
        static_cast<UIWindow*>(this->input)->Slot23();
      }

      return;
    }
  }
}

// OFFSET: 0x00A901E0
// FUNCTION: fn_17153
int WindowDialogs::Confirm(
  const char* text,
  int type,
  bool defaultAccept,
  bool replace,
  int timeout,
  const char* caption,
  int width,
  int height,
  unsigned char protect
) {
  if (this->suppressed) {
    return 185;
  }

  if (UIWindow* active = this->Top()) {
    if (active->SendMsg(nullptr, 158, 0, 0, 0, 0) == 0 && replace) {
      active->SendMsg(nullptr, 57, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    }

    return 185;
  }

  auto* box = new UIMessageBox(this->windows, this->frames, this->dialog);
  this->latest = box;
  box->Configure(text, type, defaultAccept, timeout, caption, width, height, 1, nullptr);

  if (protect == 1) {
    box->SendMsg(nullptr, 157, 1, 0, 0, 0);
  }

  this->dialog.retirement.rendered.push_front(box);

  if (this->capture) {
    this->suspendedRoots.insert(static_cast<UIWindow*>(this->capture)->Root());
  }

  this->capture = nullptr;

  if (this->input) {
    static_cast<UIWindow*>(this->input)->Slot24(0);
  }

  this->input = nullptr;
  this->alternateResult = 0;
  this->modal.push_back(box);
  int response = 184;

  if (this->immediate == 0) {
    ModalFramePump* pump = this->mode;

    for (;;) {
      if (this->result != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->dialog.retirement.Schedule(active);
          this->modal.pop_back();
        }

        response = this->result;

        if (this->modal.empty()) {
          this->result = 0;
        }

        break;
      }

      if (this->alternateResult != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->dialog.retirement.Schedule(active);
          this->modal.pop_back();
        }

        response = this->alternateResult;
        this->alternateResult = 0;
        break;
      }

      if (pump) {
        pump->Frame();
        pump = this->mode;
      }

      const auto option = this->frameOptions.find(20);

      if (option != this->frameOptions.end() && option->second > 0) {
        this->result = 0;
        this->alternateResult = 0;

        while (!this->modal.empty()) {
          this->dialog.retirement.Schedule(this->modal.back());
          this->modal.pop_back();
        }

        break;
      }

      if (this->immediate != 0) {
        break;
      }
    }
  }

  this->RestoreInput();
  return response;
}
