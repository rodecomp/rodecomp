#ifndef RAGNAROK_SOURCE_UI_LISTS_UIDROPDOWNBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIDROPDOWNBOX_H

#include "UI/Window/UIControl.h"

struct WindowOrdering;

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
