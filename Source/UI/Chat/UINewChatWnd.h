#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <vector>

class UIChatHisBox;
class UITabControl;
class UIEditCtrl;

extern std::uint8_t g_chatInputFlag;

class UINewChatWnd : public UIFrameWnd {
public:
  UINewChatWnd();

  static void SaveChatWndInfo();

  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Move(int x, int y) override;
  void Slot11() override;
  void Show(int visible) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  bool Slot42(void* output) override;
  void Slot43() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SaveChatLogs();
  void RefreshTabs(bool arrange);
  std::string ConvertChat(std::string text);
  bool SuggestionsVisible() const;
  void ShowSuggestions(std::uint8_t visible);
  void ApplyInputLayout();
  void ApplyHeaderLayout();
  void ResizeRows(int count);
  void ToggleInput();

  /* 00B4 */ UIEditCtrl* m_inputEdit;
  /* 00B8 */ UIWindow* unknown_00B8;
  /* 00BC */ UIEditCtrl* m_chatEdit;
  /* 00C0 */ UIWindow* unknown_00C0;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ int m_listWidth;
  /* 00D0 */ int m_listHeight;
  /* 00D4 */ unsigned char unknown_00D4[0x4];
  /* 00D8 */ std::uint32_t m_listColor;
  /* 00DC */ unsigned char unknown_00DC[0x4];
  /* 00E0 */ UIWindow* m_inputButtons[3];
  /* 00EC */ unsigned char unknown_00EC[0x4];
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ std::vector<UIChatHisBox*> m_tabs;
  /* 0100 */ UIWindow* m_headerButtons[2];
  /* 0108 */ int m_lockButtonX;
  /* 010C */ int m_chatModeX;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int m_currentTab;
  /* 0118 */ int unknown_0118;
  /* 011C */ int m_inputOffsetY;
  /* 0120 */ int m_tabWidth;
  /* 0124 */ int m_inputAreaHeight;
  /* 0128 */ UIWindow* unknown_0128;
  /* 012C */ UIWindow* m_chatModeButton;
  /* 0130 */ unsigned char unknown_0130[0x8];
  /* 0138 */ UITabControl* m_tabControl;
  /* 013C */ UIWindow* m_suggestions;
};

// INLINED
inline UINewChatWnd* ChatWindow() {
  return static_cast<UINewChatWnd*>(g_windowMgr.window_01C8);
}

#endif
