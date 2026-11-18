#ifndef RAGNAROK_SOURCE_UI_CHAT_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"
#include "UI/Chat/UIChatRoomTitle.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Party/UIMiniPartyWnd.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

class UIBitmapButton;
class UIChatEditCtrl;
class UIChatHisBox;
class UIMemberListBox;
class UIResizer;
class UIStaticText;

class UIChatRoomWnd : public UIFrameWnd {
public:
  UIChatRoomWnd();
  __declspec(noinline) ~UIChatRoomWnd() override;
  void Move(int x, int y) override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SaveChatLog();
  std::vector<unsigned char> SerializeReplayData();

  /* 00B4 */ ChatRoomTitleInfo info;
  /* 00F4 */ UIResizer* resizer;
  /* 00F8 */ UIChatEditCtrl* edit;
  /* 00FC */ UIChatHisBox* history;
  /* 0100 */ UIMemberListBox* memberList;
  /* 0104 */ UIBitmapButton* closeButton;
  /* 0108 */ UIStaticText* titleText;
  /* 010C */ std::map<std::string, int> memberRoles;
  /* 0114 */ std::vector<MiniPartyInfoEntry> menuCommands;
  /* 0120 */ ChatRoomWndInfo* saved;
};

#endif
