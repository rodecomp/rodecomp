#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYBOOKINGHELPWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYBOOKINGHELPWND_H

#include "UI/Window/UIFrameWnd.h"

class UITextViewer;

class UIPartyBookingHelpWnd : public UIFrameWnd {
public:
  UIPartyBookingHelpWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00B4 */ UITextViewer* m_viewer;
};

#endif
