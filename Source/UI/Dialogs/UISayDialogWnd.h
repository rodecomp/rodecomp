#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

#include <list>

class UISayDialogWnd : public UIFrameWnd {
public:
  /* 00B4 */ UIWindow* m_viewer;
  /* 00B8 */ std::list<UIWindow*> m_buttons;
  /* 00C0 */ unsigned char unknown_00C0[0x0C];
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ WindowPlacement* m_placement;
  /* 00D4 */ int unknown_00D4;

  void Slot15(int width, int height) override;
  void Resize(int width, int height) override;
  void Slot11() override;
  void Slot19() override;
  void Paint() override;
};

#endif
