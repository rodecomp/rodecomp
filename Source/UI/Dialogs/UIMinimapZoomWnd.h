#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

struct UIButtonContext;

class UIMinimapZoomWnd : public UIFrameWnd {
public:
  UIMinimapZoomWnd();
  bool Slot08() override;
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 00B4 */ std::map<int, int> m_zoom;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;

protected:
  UIButtonContext* m_controls;
};

#endif
