#ifndef RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

#include <string>

class UINameBalloonText : public UIBalloonText {
public:
  __declspec(noinline) UINameBalloonText(UIWindowContext& windows, BalloonTextContext& text);
  ~UINameBalloonText() override;
  void Slot19() override;
  void Paint() override;
  virtual void SetIcon(const char* name, unsigned char framed);

  /* 00A0 */ std::string iconName;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ unsigned char monsterType;
  /* 00BD */ unsigned char framed;
};

class UIVerticalNameBalloonText : public UINameBalloonText {
public:
  UIVerticalNameBalloonText(UIWindowContext& windows, BalloonTextContext& text);
  void Paint() override;
};

#endif
