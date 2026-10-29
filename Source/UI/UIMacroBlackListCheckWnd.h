#ifndef RAGNAROK_SOURCE_UI_UIMACROBLACKLISTCHECKWND_H
#define RAGNAROK_SOURCE_UI_UIMACROBLACKLISTCHECKWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
struct UIButtonContext;

class UIMacroBlackListCheckWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;

  /* 00B4 */ UIBitmapButton* digitButtons[10];
  /* 00DC */ UIBitmapButton* resetButton;
  /* 00E0 */ UIBitmapButton* okButton;
  /* 00E4 */ unsigned char unknown_00E4;
  /* 00E5 */ char digits[10];

protected:
  UIButtonContext* m_controls;
};

#endif
