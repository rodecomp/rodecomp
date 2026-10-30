#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

class UITabControl;

extern std::uint8_t g_chatInputFlag;

class UINewChatWnd : public UIFrameWnd {
public:
  static void SaveChatWndInfo();

  void Resize(int width, int height) override;
  void Move(int x, int y) override;
  void Slot11() override;
  void Show(int visible) override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  bool Slot42(void* output) override;
  void SaveChatLogs();
  void RefreshTabs(bool arrange);

private:
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ int m_listWidth;
  /* 00D0 */ int m_listHeight;
  /* 00D4 */ unsigned char unknown_00D4[0x20];
  /* 00F4 */ std::vector<UIWindow*> m_tabs;
  /* 0100 */ unsigned char unknown_0100[0x8];
  /* 0108 */ int m_lockButtonX;
  /* 010C */ int m_chatModeX;
  /* 0110 */ unsigned char unknown_0110[0x4];
  /* 0114 */ int m_currentTab;
  /* 0118 */ unsigned char unknown_0118[0x4];
  /* 011C */ int m_inputOffsetY;
  /* 0120 */ int m_tabWidth;
  /* 0124 */ int m_inputAreaHeight;
  /* 0128 */ UIWindow* unknown_0128;
  /* 012C */ UIWindow* m_chatModeButton;
  /* 0130 */ unsigned char unknown_0130[0x8];
  /* 0138 */ UITabControl* m_tabControl;
};

#endif
