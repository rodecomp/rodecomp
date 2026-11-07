#ifndef RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H
#define RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <map>

class UIMenuIcon : public UIBitmapButton {
public:
  UIMenuIcon();

  void OnMouseMove(int x, int y) override;
};

class UIMenuIconWnd : public UIFrameWnd {
public:
  UIMenuIconWnd();

  /* 00B4 */ std::map<int, UIButton*> unknown_00B4;
  /* 00BC */ std::map<int, UIButton*> unknown_00BC;
  /* 00C4 */ std::list<int> unknown_00C4;
  /* 00CC */ UIBitmapButton* unknown_00CC;
  /* 00D0 */ UIBitmapButton* unknown_00D0;
  /* 00D4 */ UIBitmapButton* unknown_00D4;

  void Move(int x, int y) override;
  bool IsTransmitMouseInput() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  UIWindow* HitTest(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  bool ToggleWindow(int id);
};

#endif
