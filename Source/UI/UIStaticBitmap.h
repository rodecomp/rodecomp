#ifndef RAGNAROK_SOURCE_UI_UISTATICBITMAP_H
#define RAGNAROK_SOURCE_UI_UISTATICBITMAP_H

#include "UI/Window/UIWindow.h"

#include <string>

struct UIButtonContext;

class UIStaticBitmap : public UIWindow {
public:
  UIStaticBitmap(UIWindowContext& windows, UIButtonContext& controls);
  ~UIStaticBitmap() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int value) override;
  void SetBitmap(const char* name);
  void SetTooltip(const char* text);

  /* 007C */ std::string bitmap;
  /* 0094 */ std::string tooltip;
  /* 00AC */ int tooltipX;
  /* 00B0 */ int tooltipY;
  /* 00B4 */ int forwardHover;

private:
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIStaticBitmap) == 192);
#endif

#endif
