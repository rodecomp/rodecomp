#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct WindowPlacement;

class UIPartySettingWnd : public UIFrameWnd {
public:
  void Slot11() override;
  /* 00B4 */ unsigned char unknown_00B4[0x20];
  /* 00D4 */ WindowPlacement* m_placement;
  /* 00D8 */ unsigned char unknown_00D8[0x4];
  /* 00DC */ int m_mode;

  void Paint() override;
  void Slot25(int x, int y) override;

protected:
  UIButtonContext* m_controls;
};

class UIPartyInvitationToWnd : public UIPartySettingWnd {
public:
  void Paint() override;
};

#endif
