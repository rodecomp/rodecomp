#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITIPOFTHEDAYWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UITipOfTheDayWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
