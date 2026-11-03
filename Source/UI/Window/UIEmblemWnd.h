#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIEMBLEMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIEMBLEMWND_H

#include "UI/Window/UIWindow.h"

#include <cstdint>

class UIEmblemWnd : public UIWindow {
public:
  UIEmblemWnd();
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ int screenX;
  /* 0080 */ int screenY;
  /* 0084 */ std::uint32_t aid;
  /* 0088 */ std::uint32_t guildId;
};

#endif
