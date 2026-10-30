#ifndef RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H
#define RAGNAROK_SOURCE_UI_TEXT_UINAMEBALLOONTEXT_H

#include "UI/Text/UIBalloonText.h"

#include <string>

class UINameBalloonText : public UIBalloonText {
public:
  ~UINameBalloonText() override;
  void Slot19() override;
  void Paint() override;

  /* 00A0 */ std::string iconName;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ unsigned char monsterType;
  /* 00BD */ unsigned char framed;
};

class UIVerticalNameBalloonText : public UINameBalloonText {
public:
  void Paint() override;
};

#endif
