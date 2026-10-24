#ifndef RAGNAROK_SOURCE_UI_UIAPPLYFORPARTYWND_H
#define RAGNAROK_SOURCE_UI_UIAPPLYFORPARTYWND_H

#include "UI/UIAdvenPartyBoardWnd.h"
#include "UI/UIFrameWnd.h"

struct PartyRowContext;

struct AgencyRequestContext;
struct PartyInfoContext;

struct UIApplyForPartyContext {
  UIButtonContext& buttons;
  PartyRowContext& rows;
  AgencyRequestContext& requests;
  PartyInfoContext& completion;
  const unsigned short& codePage;
  const int& screenWidth;
  const int& screenHeight;
};

// RTTI UIApplyForPartyWnd, original x86 size 260.
class UIApplyForPartyWnd : public UIFrameWnd {
public:
  UIApplyForPartyWnd(UIWindowContext& windows, UIFrameContext& frames, UIApplyForPartyContext& application);
  ~UIApplyForPartyWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot28(int x, int y) override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;
  void SetInfo(CAdvenPartyInfo value);

  /* 00B4 */ CAdvenPartyInfo info;
  /* 0100 */ PartyBoardPlacement* saved;

private:
  UIApplyForPartyContext* m_application;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIApplyForPartyWnd) == 272);
#endif

#endif
