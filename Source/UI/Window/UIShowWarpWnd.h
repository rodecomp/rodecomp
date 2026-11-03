#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISHOWWARPWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISHOWWARPWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowResponseRecorder;
class UIListBox;

class UIShowWarpWnd : public UIFrameWnd {
public:
  UIShowWarpWnd(WindowResponseRecorder& recorder);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIListBox* m_list;

protected:
  void* unknown_controls;
  void* unknown_listContext;
  WindowResponseRecorder* m_recorder;
};

#endif
