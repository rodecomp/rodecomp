#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITOOLTIPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITOOLTIPWND_H

#include "UI/Window/UIFrameWnd.h"

class UIListBox;

class UIToolTipWnd : public UIFrameWnd {
public:
  UIToolTipWnd(UIWindowContext& windows);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIListBox* m_list;
};

#endif
