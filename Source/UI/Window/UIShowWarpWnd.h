#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISHOWWARPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISHOWWARPWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct WindowResponseRecorder;
struct UIListBoxContext;
class UIListBox;

class UIShowWarpWnd : public UIFrameWnd {
public:
  UIShowWarpWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIListBoxContext& lists, WindowResponseRecorder& recorder);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIListBox* m_list;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;
  WindowResponseRecorder* m_recorder;
};

#endif
