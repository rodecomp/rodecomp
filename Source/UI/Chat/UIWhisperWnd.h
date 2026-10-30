#ifndef RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;
class UIChatHisBox;

class UIWhisperWnd : public UIFrameWnd {
public:
  /* 00B4 */ std::string m_target;
  /* 00CC */ std::string m_name;
  /* 00E4 */ unsigned char unknown_00E4[0x8];
  /* 00EC */ UIChatHisBox* m_chatHistory;
  /* 00F0 */ unsigned char unknown_00F0[0x14];
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
