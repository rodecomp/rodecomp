#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIStaticText;
struct UIButtonContext;

class UIChatRoomTitle : public UIWindow {
public:
  explicit UIChatRoomTitle(UIWindowContext& context);
  void Slot15(int width, int height) override;
  void Paint() override;
  void ResetCursor(int x, int y) override;

  /* 007C */ UIStaticText* label;
  /* 0080 */ std::string title;
  /* 0098 */ std::string unknown_0098;
  /* 00B0 */ int roomType;

protected:
  UIButtonContext* m_controls;
};

#endif
