#include "UI/Window/UIEnums.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>

extern template std::_List_node<UIWindow*, void*>* std::list<UIWindow*>::_Emplace<UIWindow* const&>(std::_List_node<UIWindow*, void*>* const, UIWindow* const&);

// OFFSET: 0x00AA2C90
// FUNCTION: fn_17219
__declspec(noinline) void UIWindowMgr::Schedule(UIWindow* window) {
  if (window == this->window_03D4) {
    this->window_03D4 = nullptr;
  }

  if (window == this->window_03D8) {
    this->window_03D8 = nullptr;
  }

  if (window == this->window_03DC) {
    this->window_03DC = nullptr;
  }

  if (window == this->window_01B8) {
    this->window_01B8 = nullptr;
  }

  if (window == this->window_4F2AC) {
    this->window_4F2AC = nullptr;
  }

  this->retired.push_back(window);
}

// OFFSET: 0x00AA7250
// FUNCTION: fn_17237
void UIWindowMgr::Destroy(UIWindow* window) {
  const bool snappingEnabled = g_serviceOptions.Get(141) != 0;

  if (!window) {
    return;
  }

  const auto rendered = std::find(this->rendered.begin(), this->rendered.end(), window);

  if (rendered != this->rendered.end()) {
    if (snappingEnabled && g_serviceOptions.Get(141) != 0) {
      this->snapRectangles.erase(window);
    }

    g_snapMgr.Remove(*rendered);
    (*rendered)->StoreInfo();
    delete *rendered;

    this->rendered.erase(rendered);
    return;
  }

  for (auto entry = this->overlays.begin(); entry != this->overlays.end(); ++entry) {
    if (entry->second != window) {
      continue;
    }

    entry->second->StoreInfo();
    delete entry->second;

    this->overlays.erase(entry);
    return;
  }
}

// OFFSET: 0x00AA71E0
// FUNCTION: fn_17235
void UIWindowMgr::DismissTransient() {
  if (this->window_4F250) {
    this->Schedule(this->window_4F250);
    this->window_4F250 = nullptr;
  }
}

// OFFSET: 0x00A8F840
// FUNCTION: fn_17145
void UIWindowMgr::DestroyWindow0478() {
  if (this->backgroundSurface) {
    delete this->backgroundSurface;
    this->backgroundSurface = nullptr;
  }
}

// OFFSET: 0x00A8FB80
// FUNCTION: fn_17147
void UIWindowMgr::SubmitActiveDialog() {
  UIWindow* window = this->choose3Wnd;
  if (window || (window = this->sayDialogWnd) != nullptr || (window = this->sayDialogOverlayWnd) != nullptr || (window = this->comboBoxWnd) != nullptr ||
      (window = this->comboBoxIDWnd) != nullptr || (window = this->chooseWarpWnd) != nullptr || (window = this->makeTargetListWnd) != nullptr ||
      (window = this->hoverWindow) != nullptr) {
    window->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return;
  }

  for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
    if ((*entry)->IsFrameWnd()) {
      (*entry)->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    }
  }
}
