#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UISPRITEBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <string>

class UISpriteButton : public UIButton {
public:
  UISpriteButton(UIWindowContext& windowContext, UIButtonContext& buttonContext);
  ~UISpriteButton() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;

  int FrameHeight(int action, int frame);
  int FrameWidth(int action, int frame);
  void SetActName(const char* name);
  void SetSprite(const char* name);

protected:
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
  void Paint() override;
};

class UISpriteButton2 : public UISpriteButton {
public:
  UISpriteButton2(UIWindowContext& windowContext, UIButtonContext& buttonContext, UIWindow* target);
  ~UISpriteButton2() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot35(int argument) override;
  void Slot47() override;
  void Slot48() override;

protected:
  /* 0114 */ UIWindow* m_target;
};

#endif
