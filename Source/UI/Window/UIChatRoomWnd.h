#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <string>

struct UIButtonContext;
class UIChatHisBox;

class UIChatRoomWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot11() override;
  void Paint() override;
  void SaveChatLog();

  /* 00B4 */ std::string title;
  /* 00CC */ unsigned char unknown_00CC[0x30];
  /* 00FC */ UIChatHisBox* history;
  /* 0100 */ unsigned char unknown_0100[0x20];
  /* 0120 */ ChatRoomWndInfo* saved;

protected:
  UIButtonContext* m_controls;
};

#endif
