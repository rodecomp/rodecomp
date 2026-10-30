#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYCONTROLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYCONTROLWND_H

#include "UI/Window/UIFrameWnd.h"

class UIReplayControlWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Paint() override;
};

#endif
