#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"

class UINewChatWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;

private:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ unsigned char unknown_00CC[0x50];
  /* 011C */ int m_inputOffsetY;
};

#endif
