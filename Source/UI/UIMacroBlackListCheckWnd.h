#ifndef RAGNAROK_SOURCE_UI_UIMACROBLACKLISTCHECKWND_H
#define RAGNAROK_SOURCE_UI_UIMACROBLACKLISTCHECKWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
struct UIButtonContext;

class UIMacroBlackListCheckWnd : public UIFrameWnd {
public:
  UIMacroBlackListCheckWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ShuffleDigitButtons();
  void Activate(unsigned char state, int duration);
  void SetCode(unsigned char state, const char* code);

  /* 00B4 */ UIBitmapButton* digitButtons[10];
  /* 00DC */ UIBitmapButton* resetButton;
  /* 00E0 */ UIBitmapButton* okButton;
  /* 00E4 */ unsigned char unknown_00E4;
  /* 00E5 */ char digits[10];
  /* 00F0 */ int inputLength;
  /* 00F4 */ char input[9];
  /* 00FD */ char code[9];
  /* 0108 */ int charSizes[8];
  /* 0128 */ bool charFlags[8];
  /* 0130 */ int charColors[8];
  /* 0150 */ int codeLength;
  /* 0154 */ int duration;
  /* 0158 */ std::uint32_t startTick;

protected:
  UIButtonContext* m_controls;
};

#endif
