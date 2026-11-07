#ifndef RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UISTATUSWND_H

#include "UI/Window/UIFrameWnd.h"

struct StatusWndInfo;

class UIBitmapButton;
class UIBmpTextButton;

class UIStatusWnd : public UIFrameWnd {
public:
  UIStatusWnd();
  /* 00B4 */ UIBitmapButton* m_statButtons[6];
  /* 00CC */ int m_expandedHeight;
  /* 00D0 */ StatusWndInfo* m_info;
  /* 00D4 */ int m_titleHeight;
  /* 00D8 */ int m_offsetY;
  /* 00DC */ int m_offsetX;
  /* 00E0 */ UIBitmapButton* m_extendedButtons[6];
  /* 00F8 */ UIBmpTextButton* m_expandButton;
  /* 00FC */ unsigned char m_extended;

  bool NeedsFrameRefresh() override;
  void StoreInfo() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void ValidatePlacement();
};

#endif
