#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMINIBTNWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMINIBTNWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIMiniBtnWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Toggle();

  /* 00B4 */ std::string m_activeImage;
  /* 00CC */ std::string m_inactiveImage;
  /* 00E4 */ int m_windowId;
};

#endif
