#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONHELPWND_H

#include "UI/Window/UIFrameWnd.h"

class UITextViewer;
struct UIButtonContext;
struct UITextViewerContext;

class UINavigationHelpWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;
  UITextViewerContext* m_text;
  /* 00B4 */ UITextViewer* m_viewer;
};

#endif
