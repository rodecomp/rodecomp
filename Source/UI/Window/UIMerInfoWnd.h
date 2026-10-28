#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMERINFOWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMerInfoWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
