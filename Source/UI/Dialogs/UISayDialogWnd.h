#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISAYDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

#include <list>

class UISayDialogWnd : public UIFrameWnd {
public:
  UISayDialogWnd(UIWindowContext& windows);
  /* 00B4 */ UIWindow* m_viewer;
  /* 00B8 */ std::list<UIWindow*> m_buttons;
  /* 00C0 */ std::list<UIWindow*> unknown_00C0;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ WindowPlacement* m_placement;
  /* 00D4 */ int unknown_00D4;

  void Slot15(int width, int height) override;
  void Resize(int width, int height) override;
  void Slot11() override;
  void Slot19() override;
  void Paint() override;
  std::vector<unsigned char> Slot44() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int ButtonX(UIBitmapButton* button);
};

#endif
