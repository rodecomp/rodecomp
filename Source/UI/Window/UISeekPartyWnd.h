#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;
struct SeekPartyWndInfo;

class UISeekPartyWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot19() override;
  void Paint() override;
  void SetJobs(const std::int16_t* jobs);

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x10];
  /* 00C4 */ int m_level;
  /* 00C8 */ int m_type;
  /* 00CC */ std::uint32_t m_frameDuration;
  /* 00D0 */ std::uint32_t m_elapsed;
  /* 00D4 */ std::uint32_t m_frameElapsed;
  /* 00D8 */ std::uint32_t m_frameTick;
  /* 00DC */ unsigned char unknown_00DC[0x108];
  /* 01E4 */ unsigned char m_minimized;
  /* 01E5 */ unsigned char unknown_01E5[3];
  /* 01E8 */ SeekPartyWndInfo* m_saved;
  UIButtonContext* m_controls;
};

#endif
