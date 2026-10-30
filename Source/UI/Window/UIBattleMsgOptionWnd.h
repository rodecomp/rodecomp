#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBATTLEMSGOPTIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UICheckBox;
class UIScrollBar;
struct UIButtonContext;

class UIBattleMsgOptionWnd : public UIFrameWnd {
public:
  UIBattleMsgOptionWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int RowAt(int x, int y);
  virtual void UpdateScrollBar();

  /* 00B4 */ UIScrollBar* m_scrollBar;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int m_visibleRows;
  /* 00C0 */ int m_scrollable;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::vector<const char*> messages;
  /* 00D4 */ std::string text;
  /* 00EC */ unsigned char unknown_00EC[0x1C];
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char unknown_0110;
  /* 0111 */ unsigned char unknown_0111[0x3];
  /* 0114 */ UICheckBox* m_allCheckBox;

protected:
  UIButtonContext* m_controls;
};

#endif
