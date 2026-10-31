#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UITipOfTheDayWnd : public UIFrameWnd {
public:
  UITipOfTheDayWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
