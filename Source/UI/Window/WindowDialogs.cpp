#include "Client/Application/ApplicationWindow.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "UI/Dialogs/UIMessageBox.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Shortcuts/UIShortCutWnd.h"

enum DialogKeyLParam : int {
  KEYDOWN_Q = 0x100001,
  KEYDOWN_W = 0x110001,
  KEYDOWN_E = 0x120001,
  KEYDOWN_R = 0x130001,
  KEYDOWN_T = 0x140001,
  KEYDOWN_Y = 0x150001,
  KEYDOWN_U = 0x160001,
  KEYDOWN_I = 0x170001,
  KEYDOWN_O = 0x180001,
  KEYDOWN_A = 0x1E0001,
  KEYDOWN_S = 0x1F0001,
  KEYDOWN_D = 0x200001,
  KEYDOWN_F = 0x210001,
  KEYDOWN_G = 0x220001,
  KEYDOWN_H = 0x230001,
  KEYDOWN_J = 0x240001,
  KEYDOWN_K = 0x250001,
  KEYDOWN_L = 0x260001,
  KEYDOWN_Z = 0x2C0001,
  KEYDOWN_X = 0x2D0001,
  KEYDOWN_C = 0x2E0001,
  KEYDOWN_V = 0x2F0001,
  KEYDOWN_B = 0x300001,
  KEYDOWN_N = 0x310001,
  KEYDOWN_M = 0x320001,
  KEYDOWN_COMMA = 0x330001,
  KEYDOWN_PERIOD = 0x340001,
};

// OFFSET: 0x00A90950
// FUNCTION: fn_17154
bool UIWindowMgr::ProcessDialogKey(int, int key) {
  if (g_session.quitBlocked == 0 || g_session.quitAllowed != 0) {
    return false;
  }

  if (!this->shortCutWnd) {
    return false;
  }

  switch (key) {
  case KEYDOWN_Q:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 1, 0, 0);
    return false;
  case KEYDOWN_W:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 1, 0, 0);
    return false;
  case KEYDOWN_E:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 1, 0, 0);
    return false;
  case KEYDOWN_R:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 1, 0, 0);
    return false;
  case KEYDOWN_T:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 1, 0, 0);
    return false;
  case KEYDOWN_Y:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 1, 0, 0);
    return false;
  case KEYDOWN_U:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 1, 0, 0);
    return false;
  case KEYDOWN_I:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 1, 0, 0);
    return false;
  case KEYDOWN_O:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 1, 0, 0);
    return false;
  case KEYDOWN_A:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 2, 0, 0);
    return false;
  case KEYDOWN_S:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 2, 0, 0);
    return false;
  case KEYDOWN_D:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 2, 0, 0);
    return false;
  case KEYDOWN_F:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 2, 0, 0);
    return false;
  case KEYDOWN_G:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 2, 0, 0);
    return false;
  case KEYDOWN_H:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 2, 0, 0);
    return false;
  case KEYDOWN_J:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 2, 0, 0);
    return false;
  case KEYDOWN_K:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 2, 0, 0);
    return false;
  case KEYDOWN_L:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 2, 0, 0);
    return false;
  case KEYDOWN_Z:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 0, 0, 0);
    return false;
  case KEYDOWN_X:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 0, 0, 0);
    return false;
  case KEYDOWN_C:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 0, 0, 0);
    return false;
  case KEYDOWN_V:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 0, 0, 0);
    return false;
  case KEYDOWN_B:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 0, 0, 0);
    return false;
  case KEYDOWN_N:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 0, 0, 0);
    return false;
  case KEYDOWN_M:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 0, 0, 0);
    return false;
  case KEYDOWN_COMMA:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 0, 0, 0);
    return false;
  case KEYDOWN_PERIOD:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 0, 0, 0);
    return false;
  default:
    return false;
  }
}

// OFFSET: 0x00A91450
// FUNCTION: fn_17160
__declspec(noinline) UIWindow* UIWindowMgr::Top() const {
  return this->modalWindows.empty() ? nullptr : this->modalWindows.back();
}

// OFFSET: 0x00A91120
// FUNCTION: fn_17155
void UIWindowMgr::RestoreInput() {
  for (std::list<UIWindow*>::iterator it = this->rendered.begin(); it != this->rendered.end(); ++it) {
    UIWindow* window = *it;
    if (window->geometry.visible == 0) {
      continue;
    }

    for (std::list<UIWindow*>::iterator childIt = window->m_children.begin(); childIt != window->m_children.end(); ++childIt) {
      UIWindow* child = *childIt;
      if (!child->IsEditable() || child->geometry.x <= 0 || child->geometry.y <= 0) {
        continue;
      }

      if (this->hoverWindow) {
        this->hoverWindow->OnFinishEdit(child->GetEditWindow() == this->hoverWindow);
      }

      this->hoverWindow = child->GetEditWindow();

      if (this->hoverWindow) {
        this->hoverWindow->OnBeginEdit();
      }

      return;
    }
  }
}

// OFFSET: 0x00A901E0
// FUNCTION: fn_17153
int UIWindowMgr::Confirm(
  const char* text, int type, int defaultAccept, int replace, int timeout, const char* caption, int width, int height, unsigned char protect
) {
  CGameMode* gameMode = g_modeMgr.GetGameMode();
  if (gameMode && static_cast<CGameMode*>(gameMode)->isInitializing == 1) {
    return ID_CANCEL;
  }

  if (UIWindow* active = this->modalWindows.empty() ? nullptr : this->modalWindows.back()) {
    if (active->SendMsg(nullptr, UM_IS_MESSAGE_PROTECTED, 0, 0, 0, 0) == 0 && replace) {
      active->SendMsg(nullptr, UM_CHANGEMSG, reinterpret_cast<std::intptr_t>(text), 0, 0, 0);
    }

    return ID_CANCEL;
  }

  auto* box = new UIMessageBox();
  this->replayMessageBoxWnd = box;
  box->Configure(text, type, defaultAccept, timeout, caption, width, height, 1, nullptr);

  if (protect == 1) {
    box->SendMsg(nullptr, UM_SET_MESSAGE_PROTECTED, 1, 0, 0, 0);
  }

  this->rendered.insert(this->rendered.end(), box);

  if (this->inputWindow) {
    this->persistentWindows.insert(this->inputWindow->Root());
  }

  this->inputWindow = nullptr;

  if (this->hoverWindow) {
    this->hoverWindow->OnFinishEdit(0);
  }

  this->hoverWindow = nullptr;

  int response;
  if (box == nullptr) {
    response = ID_CANCEL;
  } else {
    this->modalResponse = 0;
    this->modalWindows.push_back(box);
    if (g_modalImmediate == 0) {
      CMode* pump = g_modeMgr.currentMode;

      do {
        if (this->activeWindowId != 0) {
          if (UIWindow* active = this->Top()) {
            active->Show(0);
            this->Schedule(active);
            this->modalWindows.pop_back();
          }

          const int result = this->activeWindowId;

          if (this->modalWindows.empty()) {
            this->activeWindowId = 0;
          }

          this->RestoreInput();
          return result;
        }

        if (this->modalResponse != 0) {
          if (UIWindow* active = this->Top()) {
            active->Show(0);
            this->Schedule(active);
            this->modalWindows.pop_back();
          }

          const int result = this->modalResponse;
          this->modalResponse = 0;
          this->RestoreInput();
          return result;
        }

        if (pump) {
          pump->Update();
          pump = g_modeMgr.currentMode;
        }

        if (static_cast<int>(g_session.m_options.Get(20)) > 0) {
          this->activeWindowId = 0;
          this->modalResponse = 0;

          while (!this->modalWindows.empty()) {
            this->Schedule(this->modalWindows.back());
            this->modalWindows.pop_back();
          }

          break;
        }
      } while (g_modalImmediate == 0);
    }

    response = ID_OK;
  }

  this->RestoreInput();
  return response;
}

// OFFSET: 0x00AA20F0
// FUNCTION: fn_17211
void UIWindowMgr::AddMessageBox(
  const char* text, int type, int defaultAccept, int, int timeout, const char* caption, int width, int height, const char* icon, bool track
) {
  if (this->messageBoxes.size() > 5) {
    MessageBoxA(hWnd, "Max MessageBox Count", "ERROR", 0);
    return;
  }

  auto* box = new UIMessageBox();
  box->Configure(text, type, defaultAccept, timeout, caption, width, height, 0, icon);
  this->rendered.push_back(box);

  if (track) {
    this->messageBoxes.push_back(box);
  }

  this->inputWindow = nullptr;

  if (this->hoverWindow) {
    this->hoverWindow->OnFinishEdit(0);
  }

  this->hoverWindow = nullptr;
}

// OFFSET: 0x00AA2230
// FUNCTION: fn_17212
int UIWindowMgr::RunModal(UIWindow* window) {
  if (!window) {
    return ID_CANCEL;
  }

  if (this->inputWindow) {
    this->persistentWindows.insert(this->inputWindow->Root());
  }

  this->inputWindow = nullptr;

  if (this->hoverWindow) {
    this->hoverWindow->OnFinishEdit(0);
  }

  this->hoverWindow = nullptr;
  this->modalResponse = 0;
  this->modalWindows.push_back(window);
  int result;
  if (g_modalImmediate == 0) {
    CMode* pump = g_modeMgr.currentMode;

    do {
      if (this->activeWindowId != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->Schedule(active);
          this->modalWindows.pop_back();
        }

        const int response = this->activeWindowId;

        if (this->modalWindows.empty()) {
          this->activeWindowId = 0;
          this->RestoreInput();
          return response;
        }

        result = response;
        goto restoreInput;
      }

      if (this->modalResponse != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->Schedule(active);
          this->modalWindows.pop_back();
        }

        const int response = this->modalResponse;
        this->modalResponse = 0;
        this->RestoreInput();
        return response;
      }

      if (pump) {
        pump->Update();
        pump = g_modeMgr.currentMode;
      }

      if (static_cast<int>(g_session.m_options.Get(20)) > 0) {
        this->activeWindowId = 0;
        this->modalResponse = 0;

        while (!this->modalWindows.empty()) {
          this->Schedule(this->modalWindows.back());
          this->modalWindows.pop_back();
        }

        break;
      }
    } while (g_modalImmediate == 0);
  }

  result = ID_OK;
restoreInput:
  this->RestoreInput();
  return result;
}
