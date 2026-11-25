#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIRESIZER_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIRESIZER_H

#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>

enum ResizeDirection : int {
  RESIZE_LEFT = 0,
  RESIZE_RIGHT = 1,
  RESIZE_TOP = 2,
  RESIZE_BOTTOM = 3,
  RESIZE_TOP_LEFT = 4,
  RESIZE_TOP_RIGHT = 5,
  RESIZE_BOTTOM_LEFT = 6,
  RESIZE_BOTTOM_RIGHT = 7,
};

class UIResizer : public UIWindow {
public:
  UIResizer();
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void SetDirection(int value);
  void SetBitmapName(const char* name);
  void SetColors(std::uint32_t line, std::uint32_t background);

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
