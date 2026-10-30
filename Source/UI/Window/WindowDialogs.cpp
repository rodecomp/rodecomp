#include "Client/Application/ApplicationWindow.h"
#include "UI/Window/UIMessageBox.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/UIWindowMgr.h"

// GLOBAL: gl_24604
int g_dialogKeyboardEnabled;

// GLOBAL: gl_24605
int g_dialogKeyboardBlocked;

// OFFSET: 0x00A90950
// FUNCTION: fn_17154
bool UIWindowMgr::ProcessDialogKey(int, int key) {
  if (g_dialogKeyboardEnabled == 0 || g_dialogKeyboardBlocked != 0) {
    return false;
  }

  if (!this->window_01E8) {
    return false;
  }

  switch (key) {
  case 0x100001:
    this->window_01E8->SendMsg(nullptr, 41, 0, 1, 0, 0);
    return false;
  case 0x110001:
    this->window_01E8->SendMsg(nullptr, 41, 1, 1, 0, 0);
    return false;
  case 0x120001:
    this->window_01E8->SendMsg(nullptr, 41, 2, 1, 0, 0);
    return false;
  case 0x130001:
    this->window_01E8->SendMsg(nullptr, 41, 3, 1, 0, 0);
    return false;
  case 0x140001:
    this->window_01E8->SendMsg(nullptr, 41, 4, 1, 0, 0);
    return false;
  case 0x150001:
    this->window_01E8->SendMsg(nullptr, 41, 5, 1, 0, 0);
    return false;
  case 0x160001:
    this->window_01E8->SendMsg(nullptr, 41, 6, 1, 0, 0);
    return false;
  case 0x170001:
    this->window_01E8->SendMsg(nullptr, 41, 7, 1, 0, 0);
    return false;
  case 0x180001:
    this->window_01E8->SendMsg(nullptr, 41, 8, 1, 0, 0);
    return false;
  case 0x1E0001:
    this->window_01E8->SendMsg(nullptr, 41, 0, 2, 0, 0);
    return false;
  case 0x1F0001:
    this->window_01E8->SendMsg(nullptr, 41, 1, 2, 0, 0);
    return false;
  case 0x200001:
    this->window_01E8->SendMsg(nullptr, 41, 2, 2, 0, 0);
    return false;
  case 0x210001:
    this->window_01E8->SendMsg(nullptr, 41, 3, 2, 0, 0);
    return false;
  case 0x220001:
    this->window_01E8->SendMsg(nullptr, 41, 4, 2, 0, 0);
    return false;
  case 0x230001:
    this->window_01E8->SendMsg(nullptr, 41, 5, 2, 0, 0);
    return false;
  case 0x240001:
    this->window_01E8->SendMsg(nullptr, 41, 6, 2, 0, 0);
    return false;
  case 0x250001:
    this->window_01E8->SendMsg(nullptr, 41, 7, 2, 0, 0);
    return false;
  case 0x260001:
    this->window_01E8->SendMsg(nullptr, 41, 8, 2, 0, 0);
    return false;
  case 0x2C0001:
    this->window_01E8->SendMsg(nullptr, 41, 0, 0, 0, 0);
    return false;
  case 0x2D0001:
    this->window_01E8->SendMsg(nullptr, 41, 1, 0, 0, 0);
    return false;
  case 0x2E0001:
    this->window_01E8->SendMsg(nullptr, 41, 2, 0, 0, 0);
    return false;
  case 0x2F0001:
    this->window_01E8->SendMsg(nullptr, 41, 3, 0, 0, 0);
    return false;
  case 0x300001:
    this->window_01E8->SendMsg(nullptr, 41, 4, 0, 0, 0);
    return false;
  case 0x310001:
    this->window_01E8->SendMsg(nullptr, 41, 5, 0, 0, 0);
    return false;
  case 0x320001:
    this->window_01E8->SendMsg(nullptr, 41, 6, 0, 0, 0);
    return false;
  case 0x330001:
    this->window_01E8->SendMsg(nullptr, 41, 7, 0, 0, 0);
    return false;
  case 0x340001:
    this->window_01E8->SendMsg(nullptr, 41, 8, 0, 0, 0);
    return false;
  default:
    return false;
  }
}

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
        this->input->Slot24(child->Slot06() == this->input);
      }

      this->input = child->Slot06();

      if (this->input) {
        this->input->Slot23();
      }

      return;
    }
  }
}

// OFFSET: 0x00A901E0
// FUNCTION: fn_17153
int WindowDialogs::Confirm(
  const char* text, int type, bool defaultAccept, bool replace, int timeout, const char* caption, int width, int height, unsigned char protect
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
    this->suspendedRoots.insert(this->capture->Root());
  }

  this->capture = nullptr;

  if (this->input) {
    this->input->Slot24(0);
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

// OFFSET: 0x00AA20F0
// FUNCTION: fn_17211
void WindowDialogs::AddMessageBox(
  const char* text, int type, bool defaultAccept, int, int timeout, const char* caption, int width, int height, const char* icon, bool track
) {
  std::vector<UIWindow*>& boxes = this->dialog.retirement.messageBoxes;

  if (boxes.size() > 5) {
    MessageBoxA(hWnd, "Max MessageBox Count", "ERROR", 0);
    return;
  }

  auto* box = new UIMessageBox(this->windows, this->frames, this->dialog);
  box->Configure(text, type, defaultAccept, timeout, caption, width, height, 0, icon);
  this->dialog.retirement.rendered.push_back(box);

  if (track) {
    boxes.push_back(box);
  }

  this->capture = nullptr;

  if (this->input) {
    this->input->Slot24(0);
  }

  this->input = nullptr;
}

// OFFSET: 0x00AA2230
// FUNCTION: fn_17212
int WindowDialogs::RunModal(UIWindow* window) {
  if (!window) {
    return 185;
  }

  if (this->capture) {
    this->suspendedRoots.insert(this->capture->Root());
  }

  this->capture = nullptr;

  if (this->input) {
    this->input->Slot24(0);
  }

  this->input = nullptr;
  this->alternateResult = 0;
  this->modal.push_back(window);
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
