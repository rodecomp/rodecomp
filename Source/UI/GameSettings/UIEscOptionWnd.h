#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_UIESCOPTIONWND_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_UIESCOPTIONWND_H

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UIFrameWnd.h"

class UIBitmapButton;

class UIEscOptionWnd : public UIFrameWnd {
public:
  UIEscOptionWnd();
  void Paint() override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void LayoutButtons();

  /* 00B4 */ UIBitmapButton* m_buttons[7];
  /* 00D0 */ int m_mode;
  /* 00D4 */ int m_titleHeight;
};

#endif
