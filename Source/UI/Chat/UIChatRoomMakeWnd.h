#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMMAKEWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMMAKEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class UIButton;
class UIEditCtrl;
class UIRadioBtn;
class UIStaticText;

struct ChatRoomSetting {
  /* 0000 */ std::string title;
  /* 0018 */ std::string password;
  /* 0030 */ int isPublic;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int limit;
  /* 003C */ int roomId;
};

class UIChatRoomMakeWnd : public UIFrameWnd {
public:
  UIChatRoomMakeWnd();
  ~UIChatRoomMakeWnd() override;
  void Paint() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ std::string m_title;
  /* 00CC */ UIEditCtrl* m_titleEdit;
  /* 00D0 */ UIEditCtrl* m_passwordEdit;
  /* 00D4 */ UIRadioBtn* m_publicButtons[2];
  /* 00DC */ UIStaticText* m_limitText;
  /* 00E0 */ UIStaticText* m_typeText;
  /* 00E4 */ UIButton* m_closeButton;
  /* 00E8 */ int m_public;
  /* 00EC */ int m_listKind;
  /* 00F0 */ int m_limit;
  /* 00F4 */ int m_type;
  /* 00F8 */ std::vector<std::string> m_limitLabels;
  /* 0104 */ std::vector<std::string> m_typeLabels;
  /* 0110 */ std::vector<std::uint32_t> m_limits;
  /* 011C */ std::vector<std::uint32_t> m_types;
};

class UIChatRoomChangeWnd : public UIChatRoomMakeWnd {
public:
  UIChatRoomChangeWnd();
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ChatRoomSetting) == 0x40);
static_assert(offsetof(ChatRoomSetting, roomId) == 0x3C);
static_assert(offsetof(UIChatRoomMakeWnd, m_titleEdit) == 0xCC);
static_assert(offsetof(UIChatRoomMakeWnd, m_types) == 0x11C);
#endif

#endif
