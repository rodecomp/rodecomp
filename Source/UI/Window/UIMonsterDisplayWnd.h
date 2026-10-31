#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UIMonsterDisplayWnd : public UIFrameWnd {
public:
  UIMonsterDisplayWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  /* 00B4 */ std::string m_name;
  /* 00CC */ std::string unknown_00CC;
  /* 00E4 */ unsigned char unknown_00E4;

  void Slot15(int width, int height) override;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
};

#endif
