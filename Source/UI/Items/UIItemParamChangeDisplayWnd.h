#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMPARAMCHANGEDISPLAYWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIItemParamChangeDisplayWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_before;
  /* 00B8 */ int m_after;
  /* 00BC */ int m_beforeSecond;
  /* 00C0 */ int m_afterSecond;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
