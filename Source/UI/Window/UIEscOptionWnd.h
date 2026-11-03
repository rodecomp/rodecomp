#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIESCOPTIONWND_H

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UIFrameWnd.h"

class UIButton;

class UIEscOptionWnd : public UIFrameWnd {
public:
  UIEscOptionWnd();
  void Paint() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void LayoutButtons();

  /* 00B4 */ UIButton* m_buttons[7];
  /* 00D0 */ int m_mode;
  /* 00D4 */ int m_titleHeight;
};

#endif
