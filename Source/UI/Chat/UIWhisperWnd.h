#ifndef RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct UIButtonContext;
class UIChatHisBox;

struct WhisperLogLine {
  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t unknown_0018;
};

class UIWhisperWnd : public UIFrameWnd {
public:
  UIWhisperWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ std::string m_target;
  /* 00CC */ std::string m_name;
  /* 00E4 */ unsigned char unknown_00E4[0x8];
  /* 00EC */ UIChatHisBox* m_chatHistory;
  /* 00F0 */ unsigned char unknown_00F0[0x4];
  /* 00F4 */ std::vector<WhisperLogLine> m_log;
  /* 0100 */ int unknown_0100;
  /* 0104 */ std::string m_code;

  void Move(int x, int y) override;
  void Paint() override;
  void Slot26(int x, int y) override;
  void Slot33(int x, int y) override;
  bool Slot42(void* output) override;
  void Slot43() override;

  void SaveChatLog();

protected:
  UIButtonContext* m_controls;
};

#endif
