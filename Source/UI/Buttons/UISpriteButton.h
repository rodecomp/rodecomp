#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <string>

class UISpriteButton : public UIButton {
public:
  UISpriteButton();
  __declspec(noinline) ~UISpriteButton() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;

  int FrameHeight(int action, int frame);
  int FrameWidth(int action, int frame);
  void SetActName(const char* name);
  __declspec(noinline) void SetSprite(const char* name);
  void SetPalette(const char* name);

public:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;

public:
  /* 00CC */ std::string spriteName;
  /* 00E4 */ std::string actionName;

protected:
  /* 00FC */ std::string unknown_00FC;
};

class UIHeadsprButton : public UISpriteButton {
public:
  using UISpriteButton::UISpriteButton;
  UIHeadsprButton();
  void Paint() override;
};

class UISpriteButton2 : public UISpriteButton {
public:
  __declspec(noinline) UISpriteButton2(UIWindow* target);
  ~UISpriteButton2() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnWheel(int argument) override;
  void OnMouseEnter() override;
  void OnMouseLeave() override;

protected:
  /* 0114 */ UIWindow* m_target;
};

#endif
