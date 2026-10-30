#ifndef RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H
#define RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <map>

class UIMenuIconWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::map<int, int> unknown_00B4;
  /* 00BC */ std::map<int, int> unknown_00BC;
  /* 00C4 */ std::list<int> unknown_00C4;
  /* 00CC */ UIWindow* unknown_00CC;
  /* 00D0 */ UIWindow* unknown_00D0;
  /* 00D4 */ int unknown_00D4;

  void Move(int x, int y) override;
  bool Slot08() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  UIWindow* HitTest(int x, int y) override;
};

#endif
