#ifndef RAGNAROK_SOURCE_UI_UINAVILINKBUTTON_H
#define RAGNAROK_SOURCE_UI_UINAVILINKBUTTON_H

#include "UI/UITextButton.h"

class NavigationDestination {
public:
  virtual void Navigate(std::string map, int x, int y) = 0;

protected:
  ~NavigationDestination() = default;
};

class UINaviLinkButton : public UITextButton {
public:
  UINaviLinkButton(UIWindowContext& windows, UIButtonContext& buttons, NavigationDestination& navigation);
  ~UINaviLinkButton() override;
  void Slot25(int x, int y) override;
  std::string link;

private:
  NavigationDestination* m_navigation;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UINaviLinkButton) == 284);
#endif

#endif
