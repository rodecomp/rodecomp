#include "UI/Window/UIEnums.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/Textures/Surface.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>

template __declspec(noinline) std::_Tree_find_result<std::map<UIWindow*, SnapRectangle>::_Mybase::_Nodeptr> std::map<UIWindow*, SnapRectangle>::_Mybase::_Find_lower_bound<UIWindow*>(UIWindow* const&) const;

// OFFSET: 0x00AA2C90
// FUNCTION: fn_17219
__declspec(noinline) void UIWindowMgr::Schedule(UIWindow* window) {
  if (window == this->newSelectCharWnd) {
    this->newSelectCharWnd = nullptr;
  }

  if (window == this->selCharForUServerWnd) {
    this->selCharForUServerWnd = nullptr;
  }

  if (window == this->changeNameWnd) {
    this->changeNameWnd = nullptr;
  }

  if (window == this->previousMouseWnd) {
    this->previousMouseWnd = nullptr;
  }

  if (window == this->waitDBSaveWnd) {
    this->waitDBSaveWnd = nullptr;
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
      const auto snap = this->snapRectangles.find(window);
      if (snap != this->snapRectangles.end()) {
        this->snapRectangles.erase(snap);
      }
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
  if (this->transientWnd) {
    this->Schedule(this->transientWnd);
    this->transientWnd = nullptr;
  }
}

// OFFSET: 0x00A8F840
// FUNCTION: fn_17145
void UIWindowMgr::DestroyBackgroundSurface() {
  if (this->backgroundSurface) {
    delete this->backgroundSurface;
    this->backgroundSurface = nullptr;
  }
}

// OFFSET: 0x00A8FB80
// FUNCTION: fn_17147
void UIWindowMgr::SubmitActiveDialog() {
  if (this->choose3Wnd) {
    this->choose3Wnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->sayDialogWnd) {
    this->sayDialogWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->sayDialogOverlayWnd) {
    this->sayDialogOverlayWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->comboBoxWnd) {
    this->comboBoxWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->comboBoxIDWnd) {
    this->comboBoxIDWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->chooseWarpWnd) {
    this->chooseWarpWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->makeTargetListWnd) {
    this->makeTargetListWnd->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else if (this->hoverWindow) {
    this->hoverWindow->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
  } else {
    for (auto entry = this->rendered.begin(); entry != this->rendered.end(); ++entry) {
      if ((*entry)->IsFrameWnd()) {
        (*entry)->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
      }
    }
  }
}
