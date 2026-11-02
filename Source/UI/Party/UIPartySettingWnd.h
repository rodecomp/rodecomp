#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYSETTINGWND_H

#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;
struct UIEditContext;
struct WindowPlacement;

class UIPartySettingWnd : public UIFrameWnd {
public:
  UIPartySettingWnd(UIWindowContext& windows);
  void Slot11() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  /* 00B4 */ UIRadioBtn* m_expShareButtons[2];
  /* 00BC */ int m_expShare;
  /* 00C0 */ UIRadioBtn* m_itemShareButtons[2];
  /* 00C8 */ int m_itemShare;
  /* 00CC */ UIRadioBtn* m_itemDistributeButton;
  /* 00D0 */ int m_itemDistribute;
  /* 00D4 */ WindowPlacement* m_placement;
  /* 00D8 */ UIEditCtrl* m_nameEdit;
  /* 00DC */ int m_mode;

  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;

protected:
  void* unknown_controls;

public:
  /* 00E0 */ UIEditCtrl* m_edit;

protected:
  UIEditContext* m_editContext;
};

class UIPartyInvitationToWnd : public UIPartySettingWnd {
public:
  UIPartyInvitationToWnd(UIWindowContext& windows);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void Slot15(int width, int height) override;
};

#endif
