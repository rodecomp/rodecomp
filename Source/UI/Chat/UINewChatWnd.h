#ifndef RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UINEWCHATWND_H

#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdint>
#include <vector>

class UIChatHisBox;
class UITabControl;
class UIEditCtrl;
class UIChatEditCtrl;
class UIWhisperEditCtrl;
class UIResizer;
class UIBitmapButton;

class UINewChatWnd : public UIFrameWnd {
public:
  UINewChatWnd();

  static void SaveChatWndInfo();

  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Move(int x, int y) override;
  void StoreInfo() override;
  void Show(int visible) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  bool GetCustomTransBoxInfo(void* output) override;
  void DrawTransBox() override;
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
  /* 00B8 */ UIResizer* m_inputResizer;
  /* 00BC */ UIChatEditCtrl* m_chatEdit;
  /* 00C0 */ UIWhisperEditCtrl* m_whisperEdit;
  /* 00C4 */ int m_inputX;
  /* 00C8 */ int m_inputY;
  /* 00CC */ int m_listWidth;
  /* 00D0 */ int m_listHeight;
  /* 00D4 */ unsigned char m_primaryStateStorage[0x4];
  /* 00D8 */ std::uint32_t m_listColor;
  /* 00DC */ unsigned char m_secondaryStateStorage[0x4];
  /* 00E0 */ UIWindow* m_inputButtons[3];
  /* 00EC */ unsigned char padding_00EC[0x4];
  /* 00F0 */ int m_menuMode;
  /* 00F4 */ std::vector<UIChatHisBox*> m_tabs;
  /* 0100 */ UIWindow* m_headerButtons[2];
  /* 0108 */ int m_lockButtonX;
  /* 010C */ int m_chatModeX;
  /* 0110 */ int m_maxTabs;
  /* 0114 */ int m_currentTab;
  /* 0118 */ int m_savedRowCount;
  /* 011C */ int m_inputOffsetY;
  /* 0120 */ int m_tabWidth;
  /* 0124 */ int m_inputAreaHeight;
  /* 0128 */ UIBitmapButton* m_headerButton;
  /* 012C */ UIBitmapButton* m_chatModeButton;
  /* 0130 */ unsigned char padding_0130[0x8];
  /* 0138 */ UITabControl* m_tabControl;
  /* 013C */ UIWindow* m_suggestions;
};

// INLINED
inline UINewChatWnd* ChatWindow() {
  return static_cast<UINewChatWnd*>(g_windowMgr.chatWnd);
}

#endif
