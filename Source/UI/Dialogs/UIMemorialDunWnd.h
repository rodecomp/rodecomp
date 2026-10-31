#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
class UIBasicButton;
struct UIButtonContext;

extern int g_memorialDunWndX;
extern int g_memorialDunWndY;

class UIMemorialDunWnd : public UIFrameWnd {
public:
  UIMemorialDunWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Move(int x, int y) override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ResetDungeonInfo();
  void ArrangeButtons();

private:
  /* 00B4 */ UIBitmapButton* m_actionButton;
  /* 00B8 */ UIBasicButton* m_partyButton;
  UIButtonContext* m_controls;
};

#endif
