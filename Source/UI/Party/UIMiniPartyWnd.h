#ifndef RAGNAROK_SOURCE_UI_PARTY_UIMINIPARTYWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIMINIPARTYWND_H

#include "Contents/Messenger/MessengerMember.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIMiniPartyWnd : public UIFrameWnd {
public:
  UIMiniPartyWnd(UIWindowContext& windowContext, UIFrameContext& frameContext);
  void Move(int x, int y) override;
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 00B4 */ MessengerMember m_member;
  /* 0104 */ int m_cell;
  /* 0108 */ UIWindow* m_infoWnd;
  /* 010C */ UIWindow* m_background;
  /* 0110 */ int m_sizeMode;
  /* 0114 */ std::vector<int> m_widths;
  /* 0120 */ std::string m_imageName;
};

#endif
