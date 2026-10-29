#ifndef RAGNAROK_SOURCE_UI_UICAPTCHAREGISTERWND_H
#define RAGNAROK_SOURCE_UI_UICAPTCHAREGISTERWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
struct UIButtonContext;

class UICaptchaRegisterWnd : public UIFrameWnd {
public:
  void Paint() override;
  void SetRegisterDisabled(unsigned char disabled);

  /* 00C4 */ UIBitmapButton* registerButton;

protected:
  UIButtonContext* m_controls;
};

#endif
