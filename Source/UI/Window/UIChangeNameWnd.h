#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIChangeNameWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
