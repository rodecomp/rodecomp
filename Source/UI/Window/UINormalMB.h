#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINORMALMB_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINORMALMB_H

#include "UI/Window/UIFrameWnd.h"

#include <map>

class UITextViewer;

class UINormalMB : public UIFrameWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ UIWindow* m_owner;
  /* 00B8 */ UITextViewer* m_viewer;
  /* 00BC */ unsigned char unknown_00BC[0x30];
  /* 00EC */ unsigned char m_keepOpen;
  /* 00F0 */ std::map<int, void (*)()> m_actions;
};

#endif
