#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

#include <list>

class UISayDialogWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIWindow* m_viewer;
  /* 00B8 */ std::list<UIWindow*> m_buttons;

  void Resize(int width, int height) override;
  void Slot19() override;
  void Paint() override;
};

#endif
