#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWORDDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIWORDDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>

class UIWordDisplayWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;

  /* 00B4 */ int screenX;
  /* 00B8 */ int screenY;
  /* 00BC */ std::string text;
  /* 00D4 */ std::uint32_t aid;
};

#endif
