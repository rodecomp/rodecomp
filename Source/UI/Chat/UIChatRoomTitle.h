#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIChatRoomTitle : public UIWindow {
public:
  explicit UIChatRoomTitle(UIWindowContext& context);
  void Paint() override;
  void ResetCursor(int x, int y) override;

  /* 007C */ int unknown_007C;
  /* 0080 */ std::string title;
  /* 0098 */ std::string unknown_0098;
  /* 00B0 */ int roomType;
};

#endif
