#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

class UINewChatWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Show(int visible) override;

private:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ unsigned char unknown_00CC[0x28];
  /* 00F4 */ std::vector<UIWindow*> m_tabs;
  /* 0100 */ unsigned char unknown_0100[0x1C];
  /* 011C */ int m_inputOffsetY;
};

#endif
