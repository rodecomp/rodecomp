#ifndef RAGNAROK_SOURCE_UI_LISTS_UITHUMB_H
#define RAGNAROK_SOURCE_UI_LISTS_UITHUMB_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>
#include <string>

class CUIThumb : public CUIControl {
public:
  ~CUIThumb() override;
  void ResetCursor(int x, int y) override;
  void Slot53(int resolveSkin) override;

  /* 0084 */ CResPtr<CBitmapRes> top;
  /* 0088 */ CResPtr<CBitmapRes> middle;
  /* 008C */ CResPtr<CBitmapRes> bottom;
  /* 0090 */ int dragOrigin;
  /* 0094 */ unsigned char transparent;
  /* 0080 */ unsigned char resizable;
  /* 0098 */ std::string tooltip;
  /* 00B0 */ std::string topName;
  /* 00C8 */ std::string middleName;
  /* 00E0 */ std::string bottomName;
  /* 00F8 */ std::function<void(int)> onDrag;
};

class CUIVThumb : public CUIThumb {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
};

class CUIHThumb : public CUIVThumb {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIThumb, dragOrigin) == 0x90);
static_assert(offsetof(CUIThumb, tooltip) == 0x98);
static_assert(offsetof(CUIThumb, onDrag) == 0xF8);
static_assert(sizeof(CUIThumb) == 0x120);
#endif

#endif
