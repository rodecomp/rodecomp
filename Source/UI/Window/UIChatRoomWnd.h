#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

struct UIButtonContext;
struct UIEditContext;
struct UIListBoxContext;
class UIBitmapButton;
class UIChatEditCtrl;
class UIChatHisBox;
class UIMemberListBox;
class UIResizer;
class UIStaticText;

class UIChatRoomWnd : public UIFrameWnd {
public:
  UIChatRoomWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Move(int x, int y) override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void SaveChatLog();

  /* 00B4 */ std::string title;
  /* 00CC */ std::string unknown_00CC;
  /* 00E4 */ unsigned char unknown_00E4[0x10];
  /* 00F4 */ UIResizer* resizer;
  /* 00F8 */ UIChatEditCtrl* edit;
  /* 00FC */ UIChatHisBox* history;
  /* 0100 */ UIMemberListBox* memberList;
  /* 0104 */ UIBitmapButton* closeButton;
  /* 0108 */ UIStaticText* titleText;
  /* 010C */ std::map<std::string, int> unknown_010C;
  /* 0114 */ std::vector<std::pair<std::string, int>> unknown_0114;
  /* 0120 */ ChatRoomWndInfo* saved;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  UIListBoxContext* m_listContext;
};

#endif
