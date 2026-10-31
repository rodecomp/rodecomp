#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
class UIStaticText;
struct UIButtonContext;
struct SeekPartyWndInfo;

extern int g_partyBookingLevelLimit;
extern int g_partyBookingLevelMax;

class UISeekPartyWnd : public UIFrameWnd {
public:
  UISeekPartyWnd();
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetJobs(const std::int16_t* jobs);

protected:
  /* 00B4 */ UIStaticText* m_label;
  /* 00B8 */ unsigned char unknown_00B8[4];
  /* 00BC */ UIBitmapButton* m_cancelButton;
  /* 00C0 */ UIBitmapButton* m_rewriteButton;
  /* 00C4 */ int m_level;
  /* 00C8 */ int m_type;
  /* 00CC */ std::uint32_t m_frameDuration;
  /* 00D0 */ std::uint32_t m_elapsed;
  /* 00D4 */ std::uint32_t m_frameElapsed;
  /* 00D8 */ std::uint32_t m_frameTick;
  /* 00DC */ unsigned char unknown_00DC[0x104];
  /* 01E0 */ int unknown_01E0;
  /* 01E4 */ unsigned char m_minimized;
  /* 01E5 */ unsigned char unknown_01E5[3];
  /* 01E8 */ SeekPartyWndInfo* m_saved;
  UIButtonContext* m_controls;
};

#endif
