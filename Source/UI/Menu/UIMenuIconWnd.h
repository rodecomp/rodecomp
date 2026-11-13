#ifndef RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H
#define RAGNAROK_SOURCE_UI_MENU_UIMENUICONWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <map>

enum MenuIconId : int {
  MENUICON_GUILD = 373,
  MENUICON_BATTLE = 376,
  MENUICON_REC = 399,
  MENUICON_NAVIGATION = 430,
  MENUICON_BANK = 461,
  MENUICON_ACHIEVEMENT = 473,
  MENUICON_MAIL = 476,
  MENUICON_OPEN = 510,
  MENUICON_TIP = 511,
  MENUICON_SHOP = 512,
  MENUICON_ATTENDANCE = 540,
  MENUICON_REPUTE = 567,
  MENUICON_ADVENTURE_GUIDE = 581,
  MENUICON_PROBABILITY = 587,
};

class UIMenuIcon : public UIBitmapButton {
public:
  UIMenuIcon();

  void OnMouseMove(int x, int y) override;
};

class UIMenuIconWnd : public UIFrameWnd {
public:
  UIMenuIconWnd();

  /* 00B4 */ std::map<int, UIButton*> menuButtons;
  /* 00BC */ std::map<int, UIButton*> notificationButtons;
  /* 00C4 */ std::list<int> notifiedCommands;
  /* 00CC */ UIBitmapButton* openButton;
  /* 00D0 */ UIBitmapButton* notificationOpenButton;
  /* 00D4 */ UIBitmapButton* closeButton;

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
