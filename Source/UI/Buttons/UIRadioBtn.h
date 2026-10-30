#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIRADIOBTN_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>

struct UIButtonContext;

class UIRadioBtn : public UIWindow {
public:
  ~UIRadioBtn() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ std::string text;
  /* 0094 */ int enabled;
  /* 0098 */ int highlighted;
  /* 009C */ int value;
  /* 00A0 */ std::string tooltip;
  /* 00B8 */ std::string checkedBitmap;
  /* 00D0 */ std::string uncheckedBitmap;
  /* 00E8 */ int fontHeight;
  /* 00EC */ std::uint32_t textColor;

protected:
  UIButtonContext* m_controls;
};

#endif
