#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;

class UISeekPartyWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ int m_level;
  /* 00C8 */ int m_type;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ std::uint32_t m_elapsed;
  /* 00D4 */ unsigned char unknown_00D4[0x110];
  /* 01E4 */ unsigned char m_minimized;
  UIButtonContext* m_controls;
};

#endif
