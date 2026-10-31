#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPARTYBOOKINGHELPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPARTYBOOKINGHELPWND_H

#include "UI/Window/UIFrameWnd.h"

class UITextViewer;
struct UIButtonContext;
struct UITextViewerContext;

class UIPartyBookingHelpWnd : public UIFrameWnd {
public:
  UIPartyBookingHelpWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UITextViewerContext& text);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
  UITextViewerContext* m_text;
  /* 00B4 */ UITextViewer* m_viewer;
};

#endif
