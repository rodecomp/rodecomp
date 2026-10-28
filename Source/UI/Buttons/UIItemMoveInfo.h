#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIITEMMOVEINFO_H

#include "UI/Buttons/UITextButton.h"
#include "UI/Window/WindowManagerWindows.h"

class UIItemMoveInfo : public UITextButton {
public:
  void Slot28(int x, int y) override;

protected:
  /* 00F8 */ int m_moveIndex;

private:
  WindowManagerWindows* m_windows;
};

#endif
