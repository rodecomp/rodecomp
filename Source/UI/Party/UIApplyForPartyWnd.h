#ifndef RAGNAROK_SOURCE_UI_PARTY_UIAPPLYFORPARTYWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIAPPLYFORPARTYWND_H

#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Window/UIFrameWnd.h"

class UIApplyForPartyWnd : public UIFrameWnd {
public:
  UIApplyForPartyWnd();
  ~UIApplyForPartyWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetInfo(CAdvenPartyInfo value);

  /* 00B4 */ CAdvenPartyInfo info;
  /* 0100 */ PartyBoardPlacement* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIApplyForPartyWnd) == 0x104);
#endif

#endif
