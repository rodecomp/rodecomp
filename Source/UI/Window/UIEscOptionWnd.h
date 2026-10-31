#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UIFrameWnd.h"

class UIButton;
struct UIButtonContext;

class UIEscOptionWnd : public UIFrameWnd {
public:
  UIEscOptionWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot15(int width, int height) override;
  void LayoutButtons();

  /* 00B4 */ UIButton* m_buttons[7];
  /* 00D0 */ int m_mode;
  /* 00D4 */ int m_titleHeight;

protected:
  UIButtonContext* m_controls;
};

#endif
