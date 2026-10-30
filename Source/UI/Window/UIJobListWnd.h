#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIJOBLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIJobListWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
