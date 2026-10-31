#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UIMonsterDisplayWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::string m_name;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
};

#endif
