#include "Contents/Configuration/ServiceOptions.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowRetirement.h"

#include <algorithm>

// OFFSET: 0x00AA2C90
// FUNCTION: fn_17219
void UIWindowMgr::Schedule(UIWindow* window) {
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
    (*rendered)->Slot11();
    delete *rendered;

    this->rendered.erase(rendered);
    return;
  }

  for (auto entry = this->overlays.begin(); entry != this->overlays.end(); ++entry) {
    if (entry->second != window) {
      continue;
    }

    entry->second->Slot11();
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

// INLINED
void WindowRetirement::DrainPending(
  const std::deque<UIWindow*>& modal, UIWindow*& reference_4F16C, WindowAncestry*& capture, WindowAncestry*& input, WindowAncestry*& hover
) {
  if (!modal.empty() && modal.back()) {
    return;
  }

  for (UIWindow* window : this->windows.retired) {
    if (reference_4F16C == window) {
      reference_4F16C = nullptr;
    }

    if (capture && (capture == window || capture->IsDescendantOf(window))) {
      capture = nullptr;
    }

    if (input && (input == window || input->IsDescendantOf(window))) {
      input = nullptr;
    }

    if (hover && (hover == window || hover->IsDescendantOf(window))) {
      hover = nullptr;
    }

    this->windows.Destroy(window);
  }

  this->windows.retired.clear();
}
