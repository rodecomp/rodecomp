#ifndef RAGNAROK_SOURCE_UI_ITEMS_UINOTIFYITEMOBTAINWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UINOTIFYITEMOBTAINWND_H

#include "UI/Window/UIFrameWnd.h"

class UINotifyItemObtainWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x30];
  /* 00E4 */ std::uint32_t m_shownTick;

  bool CanHitTest() override;
  void Slot19() override;
};

#endif
