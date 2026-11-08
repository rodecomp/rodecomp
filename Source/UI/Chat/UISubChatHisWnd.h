#ifndef RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H

#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIChatHisBox;
class UIEditCtrl;
class UIResizer;
class UIWindowMgr;
struct SubChatHisWndInfo;

struct SubChatHistoryState {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ int borderStyle;
  /* 0014 */ int backgroundColor;
  /* 0018 */ int flags;
};

class UISubChatHisWnd : public UIFrameWnd {
  friend class UIWindowMgr;
  friend class UINewChatWnd;

public:
  UISubChatHisWnd();
  void Resize(int width, int height) override;
  void Move(int x, int y) override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  bool GetCustomTransBoxInfo(void* output) override;
  void DrawTransBox() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ApplyButtonLayout();
  UIChatHisBox* ChatBox() const { return m_chatBox; }
  std::string Name() const { return m_name; }

private:
  /* 00B4 */ UIChatHisBox* m_chatBox;
  /* 00B8 */ SubChatHisWndInfo* m_saved;
  /* 00BC */ UIResizer* m_resizer;
  /* 00C0 */ SubChatHistoryState m_state;
  /* 00DC */ UIBitmapButton* m_stickButton;
  /* 00E0 */ UIBitmapButton* m_optionButton;
  /* 00E4 */ UIBitmapButton* m_miniButton;
  /* 00E8 */ int m_inputHeight;
  /* 00EC */ int m_inputWidth;
  /* 00F0 */ int headerHeight;
  /* 00F4 */ int m_index;
  /* 00F8 */ bool m_options[25];
  /* 0111 */ unsigned char unknown_0111[3];
  /* 0114 */ std::string m_name;
  /* 012C */ UIEditCtrl* m_edit;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SubChatHistoryState) == 0x1C);
#endif

#endif
