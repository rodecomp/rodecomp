#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPETTAMINGDECEIVEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPETTAMINGDECEIVEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIPetTamingDeceiveWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_delay;
  /* 00B8 */ int m_state;
  /* 00BC */ int m_tick;
  /* 00C0 */ int m_target;
  /* 00C4 */ int m_frame;
  /* 00C8 */ int m_maxDelay;
  /* 00CC */ std::uint32_t m_finishTime;
  /* 00D0 */ int m_started;

  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
};

#endif
