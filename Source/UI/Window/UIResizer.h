#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIRESIZER_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIRESIZER_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>

class UIResizer : public UIWindow {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void SetDirection(int value);

  /* 007C */ std::string bitmapName;
  /* 0094 */ int startX;
  /* 0098 */ int startY;
  /* 009C */ int direction;
  /* 00A0 */ int parentWidth;
  /* 00A4 */ int parentHeight;
  /* 00A8 */ std::uint32_t lineColor;
  /* 00AC */ std::uint32_t backgroundColor;
};

#endif
