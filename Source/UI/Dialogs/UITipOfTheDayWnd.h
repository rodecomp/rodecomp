#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
class UITextViewer;

extern int g_window315Enabled;

class UITipOfTheDayWnd : public UIFrameWnd {
public:
  UITipOfTheDayWnd(UIWindowContext& windows, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UITextViewer* m_textViewer;
  /* 00B8 */ int m_tipIndex;
  /* 00BC */ int m_showTipsAtStartup;

protected:
  UIButtonContext* m_controls;
};

#endif
