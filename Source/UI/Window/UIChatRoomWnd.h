#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

struct UIButtonContext;

class UIChatRoomWnd : public UIFrameWnd {
public:
  void Move(int x, int y) override;
  void Slot11() override;
  void Paint() override;

  /* 00B4 */ unsigned char unknown_00B4[0x6C];
  /* 0120 */ ChatRoomWndInfo* saved;

protected:
  UIButtonContext* m_controls;
};

#endif
