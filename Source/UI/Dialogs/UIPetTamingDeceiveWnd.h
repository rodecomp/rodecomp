#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPETTAMINGDECEIVEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPETTAMINGDECEIVEWND_H

#include "UI/Window/UIFrameWnd.h"

class UIPetTamingDeceiveWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ int m_state;
  /* 00BC */ std::uint32_t unknown_00BC;
  /* 00C0 */ int m_target;
  /* 00C4 */ int m_frame;
  /* 00C8 */ std::uint32_t unknown_00C8;
  /* 00CC */ std::uint32_t unknown_00CC;
  /* 00D0 */ int m_started;

  void Slot25(int x, int y) override;
};

#endif
