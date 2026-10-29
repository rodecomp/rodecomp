#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITVTEXTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITVTEXTBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <string>

class UITVTextButton : public UIButton {
public:
  void Slot27(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4;
  /* 00B5 */ unsigned char unknown_00B5;
  /* 00B6 */ unsigned char unknown_00B6;
  /* 00B7 */ unsigned char unknown_00B7[0x25];
  /* 00DC */ std::string unknown_00DC;
  /* 00F4 */ int m_value;
};

#endif
