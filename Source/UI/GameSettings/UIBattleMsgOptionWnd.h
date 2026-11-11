#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_UIBATTLEMSGOPTIONWND_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_UIBATTLEMSGOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UICheckBox;
class UIScrollBar;

class UIBattleMsgOptionWnd : public UIFrameWnd {
public:
  UIBattleMsgOptionWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int RowAt(int x, int y);
  virtual void UpdateScrollBar();

  /* 00B4 */ UIScrollBar* m_scrollBar;
  /* 00B8 */ int hoveredOption;
  /* 00BC */ int m_visibleRows;
  /* 00C0 */ int m_scrollable;
  /* 00C4 */ int scrollPosition;
  /* 00C8 */ std::vector<const char*> messages;
  /* 00D4 */ std::string text;
  /* 00EC */ bool optionStates[0x1C];
  /* 0108 */ int chatWindowIndex;
  /* 010C */ int allChecked;
  /* 0110 */ unsigned char useMainChatOptions;
  /* 0111 */ unsigned char padding_0111[0x3];
  /* 0114 */ UICheckBox* m_allCheckBox;
};

#endif
