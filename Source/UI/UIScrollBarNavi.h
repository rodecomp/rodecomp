#ifndef RAGNAROK_SOURCE_UI_UISCROLLBARNAVI_H
#define RAGNAROK_SOURCE_UI_UISCROLLBARNAVI_H

#include "UI/UIScrollBar.h"

// RTTI UIScrollBar_Navi. No additional original fields.
class UIScrollBar_Navi : public UIScrollBar {
public:
  UIScrollBar_Navi(UIWindowContext& windows, FrameFocus& focus, ResourceMgr& resources);
  ~UIScrollBar_Navi() override;
  void Paint() override;

private:
  ResourceMgr* m_resources;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIScrollBar_Navi) == 196);
#endif

#endif
