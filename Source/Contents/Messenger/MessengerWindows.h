#ifndef RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERWINDOWS_H
#define RAGNAROK_SOURCE_CONTENTS_MESSENGER_MESSENGERWINDOWS_H

#include "Contents/Messenger/MessengerMember.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIJobIconButton;
class UIPcGage;
class UIScrollBar;
class UIStaticText;

struct MiniPartyIconPosition {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int size;
};

class UIDragMiniPartyWnd : public UIFrameWnd {
public:
  UIDragMiniPartyWnd(MessengerMember member);
  ~UIDragMiniPartyWnd() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnUp(int argument1, int argument2) override;
  void MoveGauge(int x, int y);
  void RemoveGauge();
  void SetDragStartPosition(int first, int second);
  void SetGaugeValues(int current, int maximum);

  /* 00B4 */ MessengerMember member;
  /* 0104 */ UIPcGage* gauge;
  /* 0108 */ int memberIndex;
  /* 010C */ int dragStartX;
  /* 0110 */ int dragStartY;
  /* 0114 */ int iconIndex;
  /* 0118 */ std::vector<MiniPartyIconPosition> iconPositions;
};

class UIFriendOptionWnd : public UIFrameWnd {
public:
  UIFriendOptionWnd();
  ~UIFriendOptionWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ FriendOptionWndInfo* saved;
  /* 00B8 */ UICheckBox* option1Box;
  /* 00BC */ UICheckBox* option2Box;
  /* 00C0 */ UICheckBox* option0Box;
  /* 00C4 */ UIBitmapButton* closeButton;
};

class UIMessengerGroupWnd : public UIFrameWnd {
public:
  UIMessengerGroupWnd();
  ~UIMessengerGroupWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  MessengerMember MemberAt(int x, int y);
  void OpenMiniPartyWnd(int cell, const MessengerMember& member);
  void DropMember(int x, int y, int index);
  MessengerMember MemberAtIndex(int index);
  void UpdateScroll();
  void LayoutButtons();
  void UpdateGauges();
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ MessengerGroupWndInfo* saved;
  /* 00B8 */ UIScrollBar* scrollBar;
  /* 00BC */ UIWindow* tabs[2];
  /* 00C4 */ UIWindow* closeButton;
  /* 00C8 */ UIBitmapButton* buttons[11];
  /* 00F4 */ UIWindow* resizeButton;
  /* 00F8 */ UIStaticText* titleText;
  /* 00FC */ std::list<MessengerMember> members;
  /* 0104 */ std::vector<MiniPartyInfoEntry> menuItems;
  /* 0110 */ int scrollable;
  /* 0114 */ int scrollTop;
  /* 0118 */ int columns;
  /* 011C */ int visibleRows;
  /* 0120 */ int selected;
  /* 0124 */ int rowHeight;
  /* 0128 */ UIPcGage* gauges[40];
  /* 01C8 */ UIJobIconButton* jobIcons[40];
  /* 0268 */ int iconIndex;
  /* 026C */ std::vector<MiniPartyIconPosition> iconPositions;
  /* 0278 */ UIBitmapButton* iconModeButtons[2];
  /* 0280 */ unsigned char hiddenButtons[11];
  /* 028C */ int mode;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIDragMiniPartyWnd, member) == 0xB4);
static_assert(sizeof(UIFriendOptionWnd) == 0xC8);
static_assert(sizeof(UIMessengerGroupWnd) == 0x290);
#endif

#endif
