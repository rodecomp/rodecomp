#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

extern std::uint8_t g_chatInputFlag;

class UINewChatWnd : public UIFrameWnd {
public:
  void Resize(int width, int height) override;
  void Move(int x, int y) override;
  void Show(int visible) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  bool Slot42(void* output) override;

private:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ int m_listWidth;
  /* 00D0 */ int m_listHeight;
  /* 00D4 */ unsigned char unknown_00D4[0x20];
  /* 00F4 */ std::vector<UIWindow*> m_tabs;
  /* 0100 */ unsigned char unknown_0100[0x1C];
  /* 011C */ int m_inputOffsetY;
};

#endif
