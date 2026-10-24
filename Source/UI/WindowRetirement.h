#ifndef RAGNAROK_SOURCE_UI_WINDOWRETIREMENT_H
#define RAGNAROK_SOURCE_UI_WINDOWRETIREMENT_H

#include <deque>
#include <list>
#include <map>

class UIWindow;
struct WindowAncestry;
struct WindowSnapping;

// Borrowed UIWindowMgr lists, overlay map, and direct window references.
struct WindowRetirement {
  std::list<UIWindow*>& rendered;
  std::list<UIWindow*>& retired;
  std::map<int, UIWindow*>& overlays;
  WindowSnapping& snapping;
  UIWindow*& reference_03D4;
  UIWindow*& reference_03D8;
  UIWindow*& reference_03DC;
  UIWindow*& reference_01B8;
  UIWindow*& reference_4F2AC;

  void Schedule(UIWindow* window);
  void Destroy(UIWindow* window);
  // Transient window owner. Schedule clears dependent refs
  // before appending; clear the transient pointer only after scheduling.
  void DismissTransient(UIWindow*& transient);
  // A nonnull modal top postpones destruction. Keep all pending nodes through callbacks.
  void DrainPending(
    const std::deque<UIWindow*>& modal,
    UIWindow*& reference_4F16C,
    WindowAncestry*& capture,
    WindowAncestry*& input,
    WindowAncestry*& hover
  );
};

#endif
