#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

struct UIButtonContext;
class UIChatHisBox;

class UIChatRoomWnd : public UIFrameWnd {
public:
  UIChatRoomWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Move(int x, int y) override;
  void Slot11() override;
  void Paint() override;
  void SaveChatLog();

  /* 00B4 */ std::string title;
  /* 00CC */ std::string unknown_00CC;
  /* 00E4 */ unsigned char unknown_00E4[0x18];
  /* 00FC */ UIChatHisBox* history;
  /* 0100 */ unsigned char unknown_0100[0xC];
  /* 010C */ std::map<std::string, int> unknown_010C;
  /* 0114 */ std::vector<std::pair<std::string, int>> unknown_0114;
  /* 0120 */ ChatRoomWndInfo* saved;

protected:
  UIButtonContext* m_controls;
};

#endif
