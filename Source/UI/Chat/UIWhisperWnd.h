#ifndef RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UIWHISPERWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct WindowPlacement;
class UIBitmapButton;
class UIChatEditCtrl;
class UIChatHisBox;
class UIResizer;

struct WhisperLogLine {
  /* 0000 */ std::string text;
  /* 0018 */ std::uint32_t commandId;
};

class UIWhisperWnd : public UIFrameWnd {
public:
  UIWhisperWnd();

  /* 00B4 */ std::string m_target;
  /* 00CC */ std::string m_name;
  /* 00E4 */ UIResizer* m_resizer;
  /* 00E8 */ UIChatEditCtrl* m_edit;
  /* 00EC */ UIChatHisBox* m_chatHistory;
  /* 00F0 */ UIBitmapButton* m_closeButton;
  /* 00F4 */ std::vector<WhisperLogLine> m_log;
  /* 0100 */ WindowPlacement* placement;
  /* 0104 */ std::string m_code;

  void Move(int x, int y) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDblClk(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  bool GetCustomTransBoxInfo(void* output) override;
  void DrawTransBox() override;
  void StoreInfo() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  void SaveChatLog();
};

#endif
