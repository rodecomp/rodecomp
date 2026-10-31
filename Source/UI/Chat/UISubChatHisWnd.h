#ifndef RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H

#include "Contents/Configuration/ServiceConfiguration.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIChatHisBox;
class UIEditCtrl;
class UIResizer;
class UIWindowMgr;
struct SubChatHisWndInfo;
struct UIButtonContext;
struct UIEditContext;
struct UIListBoxContext;

struct SubChatHistoryState {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
};

class UISubChatHisWnd : public UIFrameWnd {
  friend class UIWindowMgr;

public:
  UISubChatHisWnd();
  UISubChatHisWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edit, UIListBoxContext& list);
  void Move(int x, int y) override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  bool Slot42(void* output) override;
  void Slot43() override;
  UIChatHisBox* ChatBox() const { return m_chatBox; }
  std::string Name() const { return m_name; }

private:
  /* 00B4 */ UIChatHisBox* m_chatBox;
  /* 00B8 */ SubChatHisWndInfo* m_saved;
  /* 00BC */ UIResizer* m_resizer;
  /* 00C0 */ SubChatHistoryState m_state;
  /* 00DC */ UIWindow* m_stickButton;
  /* 00E0 */ UIWindow* m_optionButton;
  /* 00E4 */ UIWindow* m_miniButton;
  /* 00E8 */ int m_inputHeight;
  /* 00EC */ int m_inputWidth;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int m_index;
  /* 00F8 */ bool m_options[25];
  /* 0111 */ unsigned char unknown_0111[3];
  /* 0114 */ std::string m_name;
  /* 012C */ UIEditCtrl* m_edit;
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  UIListBoxContext* m_listContext;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SubChatHistoryState) == 0x1C);
#endif

#endif
