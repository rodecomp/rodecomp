#ifndef RAGNAROK_SOURCE_UI_LISTS_UISCROLLBARNAVI_H
#define RAGNAROK_SOURCE_UI_LISTS_UISCROLLBARNAVI_H

#include "UI/Lists/UIScrollBar.h"

class UIScrollBar_Navi : public UIScrollBar {
public:
  explicit UIScrollBar_Navi(UIWindowContext& windows);
  ~UIScrollBar_Navi() override;
  void Paint() override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIScrollBar_Navi) == sizeof(UIScrollBar));
#endif

#endif
