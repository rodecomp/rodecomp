#ifndef RAGNAROK_SOURCE_UI_LISTS_UITHUMB_H
#define RAGNAROK_SOURCE_UI_LISTS_UITHUMB_H

#include "UI/Window/UIControl.h"

#include <cstddef>
#include <functional>
#include <string>

class CBitmapRes;

class CUIThumb : public CUIControl {
public:
  void ResetCursor(int x, int y) override;


  /* 0084 */ unsigned char unknown_0084[0x08];
  /* 008C */ CBitmapRes* bitmap;
  /* 0090 */ int dragOrigin;
  /* 0094 */ unsigned char unknown_0094[0x04];
  /* 0098 */ std::string unknown_0098;
  /* 00B0 */ std::string unknown_00B0;
  /* 00C8 */ std::string unknown_00C8;
  /* 00E0 */ std::string unknown_00E0;
  /* 00F8 */ std::function<void(int)> onDrag;
};

class CUIVThumb : public CUIThumb {
public:
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CUIThumb, dragOrigin) == 0x90);
static_assert(offsetof(CUIThumb, onDrag) == 0xF8);
static_assert(sizeof(CUIThumb) == 0x120);
#endif

#endif
