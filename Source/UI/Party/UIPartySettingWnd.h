#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
struct UIButtonContext;
struct WindowPlacement;

class UIPartySettingWnd : public UIFrameWnd {
public:
  UIPartySettingWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
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
  UIPartyInvitationToWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00E0 */ UIEditCtrl* m_edit;
};

#endif
