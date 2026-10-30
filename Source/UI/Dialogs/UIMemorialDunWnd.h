#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMEMORIALDUNWND_H

#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;
class UIBasicButton;

extern int g_memorialDunWndX;
extern int g_memorialDunWndY;

class UIMemorialDunWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void ResetDungeonInfo();
  void ArrangeButtons();

private:
  /* 00B4 */ UIBitmapButton* m_actionButton;
  /* 00B8 */ UIBasicButton* m_partyButton;
};

#endif
