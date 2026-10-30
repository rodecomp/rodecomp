#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPASSWORDWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UIItemDropCntWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIEditCtrl* m_edit;
  /* 00B8 */ std::uint32_t unknown_00B8;

  void Paint() override;
};

class UIPasswordWnd : public UIItemDropCntWnd {
public:
  /* 00BC */ int unknown_00BC;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
