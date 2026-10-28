#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBMPRADIOBTN_H

#include "UI/Window/UIWindow.h"

#include <string>

struct UIButtonContext;

class UIBmpRadioBtn : public UIWindow {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ unsigned char unknown_007C[0x64];
  /* 00E0 */ std::string m_tooltip;
  UIButtonContext* m_controls;
};

class UIBmpRadioBtn2 : public UIBmpRadioBtn {
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
