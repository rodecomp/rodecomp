#ifndef RAGNAROK_SOURCE_UI_TEXT_UISTRING_H
#define RAGNAROK_SOURCE_UI_TEXT_UISTRING_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIString : public UIWindow {
public:
  explicit UIString(UIWindowContext& windows);
  void ResetCursor(int x, int y) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ std::string unknown_0084;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int unknown_00A0;
  /* 00A4 */ int unknown_00A4;
  /* 00A8 */ unsigned char unknown_00A8[0x4];
  /* 00AC */ std::string unknown_00AC;
  /* 00C4 */ std::string unknown_00C4;
  /* 00DC */ std::string unknown_00DC;
};

#endif
