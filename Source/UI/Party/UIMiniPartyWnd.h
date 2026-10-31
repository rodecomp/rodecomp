#ifndef RAGNAROK_SOURCE_UI_PARTY_UIMINIPARTYWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIMINIPARTYWND_H

#include "Contents/Messenger/MessengerMember.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIPcGage;
class UIMiniPartyWnd;

struct MiniPartyInfoEntry {
  MiniPartyInfoEntry(const char* text, int value);

  /* 0000 */ std::string text;
  /* 0018 */ int value;
};

class UIMiniPartyInfoWnd : public UIFrameWnd {
public:
  UIMiniPartyInfoWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;

  /* 00B4 */ std::vector<MiniPartyInfoEntry> m_entries;
  /* 00C0 */ UIPcGage* m_gauge;
  /* 00C4 */ UIMiniPartyWnd* m_owner;
};

class UIMiniPartyWnd : public UIFrameWnd {
public:
  UIMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);
  void Slot15(int width, int height) override;
  void Move(int x, int y) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  MessengerMember GetMember();

  /* 00B4 */ MessengerMember m_member;
  /* 0104 */ int m_cell;
  /* 0108 */ UIWindow* m_infoWnd;
  /* 010C */ UIWindow* m_background;
  /* 0110 */ int m_sizeMode;
  /* 0114 */ std::vector<int> m_widths;
  /* 0120 */ std::string m_imageName;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MiniPartyInfoEntry) == 28);
#endif

#endif
