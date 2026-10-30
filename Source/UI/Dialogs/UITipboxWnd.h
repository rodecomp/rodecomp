#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITIPBOXWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITIPBOXWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

struct TipBoxAnimation {
  /* 0000 */ std::uint32_t moveStart;
  /* 0004 */ std::uint32_t blinkStart;
  /* 0008 */ int kind;
  /* 000C */ int blink;
  /* 0010 */ float x;
  /* 0014 */ float y;
  /* 0018 */ int fromX;
  /* 001C */ int fromY;
  /* 0020 */ int toX;
  /* 0024 */ int toY;
  /* 0028 */ float duration;
  /* 002C */ bool visible;
};

struct TipBoxDot {
  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ bool visible;
};

class UITipboxWnd : public UIFrameWnd {
public:
  void ShowTip(int id);
  void TrimAt(std::string& text, const char* marker);
  void UpdateAnimations();
  bool BuildAnimations();

  /* 00B4 */ unsigned char unknown_00B4[0x28];
  /* 00DC */ std::vector<TipBoxAnimation> m_animations;
  /* 00E8 */ std::vector<TipBoxDot> m_dots;
  /* 00F4 */ std::uint32_t m_buildTime;
  /* 00F8 */ std::uint32_t m_frameTime;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(TipBoxAnimation) == 0x30);
static_assert(sizeof(TipBoxDot) == 0xC);
#endif

#endif
