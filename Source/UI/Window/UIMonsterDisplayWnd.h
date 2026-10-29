#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIMONSTERDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;

class UIMonsterDisplayWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::string m_name;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
