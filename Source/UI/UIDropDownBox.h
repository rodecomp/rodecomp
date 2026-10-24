#ifndef RAGNAROK_SOURCE_UI_UIDROPDOWNBOX_H
#define RAGNAROK_SOURCE_UI_UIDROPDOWNBOX_H

#include "UI/UIControl.h"

struct WindowOrdering;
// RTTI CUIDropDownBox. Its attached child is borrowed from another window;
// detachment deliberately preserves that child's parent pointer and lifetime.
class CUIDropDownBox : public CUIControl {
public:
  explicit CUIDropDownBox(UIWindowContext& context);
  ~CUIDropDownBox() override;
  void Paint() override;
  void Detach();
  bool HasChild(const UIWindow* window) const;
  bool HasOwnerChild(const UIWindow* window) const;
  void Attach(UIWindow* owner, UIWindow* child);

  /* 0080 */ UIWindow* owner;
};

// Borrowed manager popup pointer and rendered-order owner.
struct WindowDropDown {
  UIWindowContext& windows;
  WindowOrdering& ordering;
  CUIDropDownBox*& popup;

  void Show(UIWindow* owner, UIWindow* child, int x, int y);
  bool Close();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIDropDownBox) == 136);
#endif

#endif
