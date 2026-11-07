#ifndef RAGNAROK_SOURCE_UI_UISTATICBITMAP_H
#define RAGNAROK_SOURCE_UI_UISTATICBITMAP_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIStaticBitmap : public UIWindow {
public:
  UIStaticBitmap();
  ~UIStaticBitmap() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int value) override;
  void SetBitmap(const char* name);
  void SetTooltip(const char* text);

  /* 007C */ std::string bitmap;
  /* 0094 */ std::string tooltip;
  /* 00AC */ int tooltipX;
  /* 00B0 */ int tooltipY;
  /* 00B4 */ int forwardHover;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIStaticBitmap) == 0xB8);
#endif

#endif
