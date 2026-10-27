#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIROULLETTEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIRoulletteWnd : public UIFrameWnd {
public:
  /* 00BC */ std::uint32_t unknown_00BC;
  /* 00C0 */ std::uint32_t unknown_00C0;
  /* 00C4 */ std::uint32_t unknown_00C4;
  /* 00C8 */ unsigned char m_enabled;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIRoulletteWnd, m_enabled) == 0xC8);
#endif

#endif
