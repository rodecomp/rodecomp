#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMTITLE_H

#include "UI/Window/UIWindow.h"

#include <string>

class UIStaticText;
struct UIButtonContext;
struct ForbiddenWords;

struct ChatRoomTitleInfo {
  /* 0000 */ std::string title;
  /* 0018 */ std::string unknown_0018;
  /* 0030 */ int type;
  /* 0034 */ int memberCount;
  /* 0038 */ int memberLimit;
  /* 003C */ int id;
};

class UIChatRoomTitle : public UIWindow {
  friend class UIWindowMgr;

public:
  explicit UIChatRoomTitle(UIWindowContext& context);
  void Slot15(int width, int height) override;
  void Slot26(int x, int y) override;
  void Paint() override;
  void ResetCursor(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 007C */ UIStaticText* label;
  /* 0080 */ std::string title;
  /* 0098 */ std::string unknown_0098;
  /* 00B0 */ int roomType;
  /* 00B4 */ int memberCount;
  /* 00B8 */ int memberLimit;

protected:
  /* 00BC */ int roomId;
  /* 00C0 */ short unknown_00C0;
  UIButtonContext* m_controls;
  ForbiddenWords* m_forbiddenWords;
};

#endif
