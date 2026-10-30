#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIVIEWCAMERAINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIVIEWCAMERAINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;

class UIViewCameraInfoWnd : public UIFrameWnd {
public:
  bool Slot08() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
};

#endif
