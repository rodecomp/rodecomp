#include "UI/Layout/SnapMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowRetirement.h"
#include "UI/Layout/WindowSnapping.h"

#include <algorithm>

// OFFSET: 0x00AA2C90
// FUNCTION: fn_17219
void WindowRetirement::Schedule(UIWindow* window) {
  if (window == this->reference_03D4) {
    this->reference_03D4 = nullptr;
  }

  if (window == this->reference_03D8) {
    this->reference_03D8 = nullptr;
  }

  if (window == this->reference_03DC) {
    this->reference_03DC = nullptr;
  }

  if (window == this->reference_01B8) {
    this->reference_01B8 = nullptr;
  }

  if (window == this->reference_4F2AC) {
    this->reference_4F2AC = nullptr;
  }

  this->retired.push_back(window);
}

// OFFSET: 0x00AA7250
// FUNCTION: fn_17237
void WindowRetirement::Destroy(UIWindow* window) {
  const bool snappingEnabled = this->snapping.enabled;

  if (!window) {
    return;
  }

  const auto rendered = std::find(this->rendered.begin(), this->rendered.end(), window);

  if (rendered != this->rendered.end()) {
    if (snappingEnabled && this->snapping.enabled) {
      this->snapping.rectangles.erase(window);
    }

    this->snapping.graph.Remove(window);
    window->Slot11();
    delete window;

    this->rendered.erase(rendered);
    return;
  }

  for (auto entry = this->overlays.begin(); entry != this->overlays.end(); ++entry) {
    if (entry->second != window) {
      continue;
    }

    window->Slot11();
    delete window;

    this->overlays.erase(entry);
    return;
  }
}

// OFFSET: 0x00AA71E0
// FUNCTION: fn_17235
void WindowRetirement::DismissTransient(UIWindow*& transient) {
  if (transient) {
    this->Schedule(transient);
    transient = nullptr;
  }
}

// OFFSET: 0x00AA4730
// FUNCTION: fn_17222
void WindowRetirement::DrainPending(
  const std::deque<UIWindow*>& modal, UIWindow*& reference_4F16C, WindowAncestry*& capture, WindowAncestry*& input, WindowAncestry*& hover
) {
  if (!modal.empty() && modal.back()) {
    return;
  }

  for (UIWindow* window : this->retired) {
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

    this->Destroy(window);
  }

  this->retired.clear();
}
