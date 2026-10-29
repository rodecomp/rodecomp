#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIMinimapZoomWnd : public UIFrameWnd {
public:
  bool Slot08() override;
  void Paint() override;
  void Slot25(int x, int y) override;

protected:
  UIButtonContext* m_controls;
};

#endif
