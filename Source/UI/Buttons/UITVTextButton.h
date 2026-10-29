#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITVTEXTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITVTEXTBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <cstdint>
#include <string>

class UITVTextButton : public UIButton {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4;
  /* 00B5 */ unsigned char unknown_00B5;
  /* 00B6 */ unsigned char unknown_00B6;
  /* 00B7 */ unsigned char m_border;
  /* 00B8 */ unsigned char m_fill;
  /* 00B9 */ unsigned char unknown_00B9[0x3];
  /* 00BC */ std::uint32_t m_borderColor;
  /* 00C0 */ std::uint32_t m_fillColor;
  /* 00C4 */ std::uint32_t m_textColor;
  /* 00C8 */ std::uint32_t m_outlineColor;
  /* 00CC */ std::uint32_t m_highlightColor;
  /* 00D0 */ unsigned char m_outlined;
  /* 00D1 */ unsigned char unknown_00D1[0x3];
  /* 00D4 */ int m_fontHeight;
  /* 00D8 */ int m_fontFamily;
  /* 00DC */ std::string unknown_00DC;
  /* 00F4 */ int m_value;
};

#endif
