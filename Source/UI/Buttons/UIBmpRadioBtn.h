#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H

#include "UI/Window/UIWindow.h"

#include <string>

struct UIButtonContext;

class UIBmpRadioBtn : public UIWindow {
public:
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 007C */ unsigned char unknown_007C[0x60];
  /* 00DC */ bool m_hovered;
  /* 00DD */ unsigned char unknown_00DD[3];
  /* 00E0 */ std::string m_tooltip;
  UIButtonContext* m_controls;
  /* 00FC */ int m_value;
};

class UIBmpRadioBtn2 : public UIBmpRadioBtn {
public:
  void Slot47() override;
  void Slot48() override;
};

class UICaptchaImage : public UIWindow {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ unsigned char unknown_007C[0x124];
  /* 01A0 */ std::string m_tooltip;
  UIButtonContext* m_controls;
};

#endif
