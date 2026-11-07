#ifndef RAGNAROK_SOURCE_UI_GUILD_UIAGITWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIAGITWND_H

#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIAgitMessageBox : public UIFrameWnd {
public:
  UIAgitMessageBox();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void InitializeMessageBox(const char* text, int width, int height);

  /* 00B4 */ std::uint16_t m_agitId;
};

class UIListBoxAgit : public UIListBox {
public:
  using UIListBox::UIListBox;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
};

class UIListBoxAgitMenu : public UIListBox {
public:
  UIListBoxAgitMenu();
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;

  /* 00D4 */ int m_agitIndex;
};

#endif
