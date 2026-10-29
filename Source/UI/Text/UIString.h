#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTRING_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTRING_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIString : public UIWindow {
public:
  void ResetCursor(int x, int y) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ std::string unknown_0084;
  /* 009C */ unsigned char unknown_009C[0x10];
  /* 00AC */ std::string unknown_00AC;
};

#endif
