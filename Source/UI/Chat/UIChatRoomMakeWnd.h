#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMMAKEWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMMAKEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

class UIChatRoomMakeWnd : public UIFrameWnd {
public:
  UIChatRoomMakeWnd(UIWindowContext& windows, UIFrameContext& frames);

  /* 00B4 */ std::string m_title;
  /* 00CC */ unsigned char unknown_00CC[0x2C];
  /* 00F8 */ std::vector<std::uint32_t> unknown_00F8;
  /* 0104 */ std::vector<std::uint32_t> unknown_0104;
  /* 0110 */ std::vector<std::uint32_t> unknown_0110;
  /* 011C */ std::vector<std::uint32_t> unknown_011C;
};

class UIChatRoomChangeWnd : public UIChatRoomMakeWnd {
public:
  UIChatRoomChangeWnd(UIWindowContext& windows, UIFrameContext& frames);
};

#endif
