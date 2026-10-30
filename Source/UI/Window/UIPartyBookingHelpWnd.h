#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPARTYBOOKINGHELPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPARTYBOOKINGHELPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIPartyBookingHelpWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
