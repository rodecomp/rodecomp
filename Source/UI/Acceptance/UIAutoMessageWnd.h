#ifndef RAGNAROK_SOURCE_UI_ACCEPTANCE_UIAUTOMESSAGEWND_H
#define RAGNAROK_SOURCE_UI_ACCEPTANCE_UIAUTOMESSAGEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIEditCtrl;

class UIAutoMessageWnd : public UIFrameWnd {
public:
  UIAutoMessageWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIEditCtrl* m_edit;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIAutoMessageWnd) == 0xB8);
#endif

#endif
