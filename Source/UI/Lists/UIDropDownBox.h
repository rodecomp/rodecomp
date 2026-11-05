#ifndef RAGNAROK_SOURCE_UI_LISTS_UIDROPDOWNBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIDROPDOWNBOX_H

#include "UI/Window/UIControl.h"


class CUIDropDownBox : public CUIControl {
public:
  CUIDropDownBox();
  ~CUIDropDownBox() override;
  void Paint() override;
  void Detach();
  bool HasChild(const UIWindow* window) const;
  bool HasOwnerChild(const UIWindow* window) const;
  __declspec(noinline) void Attach(UIWindow* owner, UIWindow* child);

  /* 0080 */ UIWindow* owner;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CUIDropDownBox) == 0x84);
#endif

#endif
