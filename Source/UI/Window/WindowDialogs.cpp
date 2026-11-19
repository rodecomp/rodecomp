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

template __declspec(noinline) std::_Tree_find_result<std::map<int, std::int64_t>::_Mybase::_Nodeptr> std::map<int, std::int64_t>::_Mybase::_Find_lower_bound<int>(const int&) const;

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
  case 0x100001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 1, 0, 0);
    return false;
  case 0x110001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 1, 0, 0);
    return false;
  case 0x120001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 1, 0, 0);
    return false;
  case 0x130001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 1, 0, 0);
    return false;
  case 0x140001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 1, 0, 0);
    return false;
  case 0x150001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 1, 0, 0);
    return false;
  case 0x160001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 1, 0, 0);
    return false;
  case 0x170001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 1, 0, 0);
    return false;
  case 0x180001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 1, 0, 0);
    return false;
  case 0x1E0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 2, 0, 0);
    return false;
  case 0x1F0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 2, 0, 0);
    return false;
  case 0x200001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 2, 0, 0);
    return false;
  case 0x210001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 2, 0, 0);
    return false;
  case 0x220001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 2, 0, 0);
    return false;
  case 0x230001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 2, 0, 0);
    return false;
  case 0x240001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 2, 0, 0);
    return false;
  case 0x250001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 2, 0, 0);
    return false;
  case 0x260001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 8, 2, 0, 0);
    return false;
  case 0x2C0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 0, 0, 0, 0);
    return false;
  case 0x2D0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 1, 0, 0, 0);
    return false;
  case 0x2E0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 2, 0, 0, 0);
    return false;
  case 0x2F0001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 3, 0, 0, 0);
    return false;
  case 0x300001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 4, 0, 0, 0);
    return false;
  case 0x310001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 5, 0, 0, 0);
    return false;
  case 0x320001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 6, 0, 0, 0);
    return false;
  case 0x330001:
    this->shortCutWnd->SendMsg(nullptr, UM_USE_SHORTEN_ITEM, 7, 0, 0, 0);
    return false;
  case 0x340001:
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
  const char* text, int type, int defaultAccept, bool replace, int timeout, const char* caption, int width, int height, unsigned char protect
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
  int response = ID_OK;

  if (g_modalImmediate == 0) {
    CMode* pump = g_modeMgr.currentMode;

    for (;;) {
      if (this->activeWindowId != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->Schedule(active);
          this->modalWindows.pop_back();
        }

        response = this->activeWindowId;

        if (this->modalWindows.empty()) {
          this->activeWindowId = 0;
        }

        break;
      }

      if (this->modalResponse != 0) {
        if (UIWindow* active = this->Top()) {
          active->Show(0);
          this->Schedule(active);
          this->modalWindows.pop_back();
        }

        response = this->modalResponse;
        this->modalResponse = 0;
        break;
      }

      if (pump) {
        pump->Update();
        pump = g_modeMgr.currentMode;
      }

      if (g_session.GetOption(20) > 0) {
        this->activeWindowId = 0;
        this->modalResponse = 0;

        while (!this->modalWindows.empty()) {
          this->Schedule(this->modalWindows.back());
          this->modalWindows.pop_back();
        }

        break;
      }

      if (g_modalImmediate != 0) {
        break;
      }
    }
  }

  this->RestoreInput();
  return response;
}
