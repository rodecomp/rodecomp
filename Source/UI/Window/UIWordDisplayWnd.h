#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWORDDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIWORDDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIWordDisplayWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ int screenX;
  /* 00B8 */ int screenY;
  /* 00BC */ std::string text;
  /* 00D4 */ std::uint32_t aid;
};

#endif
