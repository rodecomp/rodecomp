#ifndef RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H
#define RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <map>

class UIMenuIcon : public UIBitmapButton {
public:
  void Slot27(int x, int y) override;
};

class UIMenuIconWnd : public UIFrameWnd {
public:
  UIMenuIconWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);

  /* 00B4 */ std::map<int, UIButton*> unknown_00B4;
  /* 00BC */ std::map<int, UIButton*> unknown_00BC;
  /* 00C4 */ std::list<int> unknown_00C4;
  /* 00CC */ UIButton* unknown_00CC;
  /* 00D0 */ UIButton* unknown_00D0;
  /* 00D4 */ UIButton* unknown_00D4;

  void Move(int x, int y) override;
  bool Slot08() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  UIWindow* HitTest(int x, int y) override;

  bool ToggleWindow(int id);
};

#endif
