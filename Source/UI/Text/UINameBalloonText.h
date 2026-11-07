#ifndef RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

#include <string>

class CActorNameCard;

class UINameBalloonText : public UIBalloonText {
public:
  __declspec(noinline) UINameBalloonText();
  ~UINameBalloonText() override;
  void OnProcess() override;
  void Paint() override;
  virtual void SetIcon(const char* name, unsigned char framed);
  virtual void SetName(const CActorNameCard& card, bool stripHash, unsigned char monsterType);
  virtual void AddNameLine(const char* text);

  /* 00A0 */ std::string iconName;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ unsigned char monsterType;
  /* 00BD */ unsigned char framed;
};

class UIVerticalNameBalloonText : public UINameBalloonText {
public:
  UIVerticalNameBalloonText();
  void Paint() override;
  void AddNameLine(const char* text) override;

  /* 00C0 */ int textHeight;
};

#endif
