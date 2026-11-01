#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPZOOMWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

struct TownInfo;
struct UIButtonContext;

class UIMinimapZoomWnd : public UIFrameWnd {
public:
  UIMinimapZoomWnd();
  bool Slot08() override;
  void Paint() override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void AddTownButton(int index, TownInfo town);

  /* 00B4 */ std::map<int, UIWindow*> m_zoom;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;

protected:
  UIButtonContext* m_controls;
};

#endif
