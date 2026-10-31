#ifndef RAGNAROK_SOURCE_UI_ACCEPTANCE_UIJOINPARTYACCEPTWND_H
#define RAGNAROK_SOURCE_UI_ACCEPTANCE_UIJOINPARTYACCEPTWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UITextViewer;
struct UITextViewerContext;
struct WindowResponseRecorder;
struct NormalItemSlotWindow;

struct UIAcceptanceContext {
  UITextViewerContext& text;
  WindowResponseRecorder& recorder;
  const int& screenWidth;
  const int& screenHeight;
  const int& service;
  std::list<ITEM_INFO>& firstShopItems;
  std::list<ITEM_INFO>& secondShopItems;
  std::list<ITEM_INFO>& thirdShopItems;
  int& replayShopState;
  std::string& replayExchangeName;
};

class UIChooseSellBuyWnd : public UIFrameWnd {
public:
  UIChooseSellBuyWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIChooseSellBuyWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ std::uint32_t identity;
  /* 00B8 */ UITextViewer* viewer;

protected:
  UIAcceptanceContext* m_acceptance;
  void AddChoice(const char* name, int id, int x, int y);
  void CreateViewer(int width, int height);
};

class UIExchangeAcceptWnd : public UIChooseSellBuyWnd {
public:
  UIExchangeAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIExchangeAcceptWnd() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Slot11() override;

  /* 00BC */ NormalItemSlotWindow* saved;
};

class UIJoinPartyAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinPartyAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIJoinPartyAcceptWnd() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIAllyGuildAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIAllyGuildAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIBabyAcceptWnd2 : public UIExchangeAcceptWnd {
public:
  UIBabyAcceptWnd2(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIBabyAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIBabyAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UICoupleAcceptWnd : public UIExchangeAcceptWnd {
public:
  UICoupleAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

class UIJoinFriendAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinFriendAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

class UIJoinGuildAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinGuildAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

class UIMixAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIMixAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

class UIStarPlaceAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIStarPlaceAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

class UIYourItemWnd : public UIExchangeAcceptWnd {
public:
  UIYourItemWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIChooseSellBuyWnd) == 200);
static_assert(sizeof(UIExchangeAcceptWnd) == 204);
static_assert(sizeof(UIJoinPartyAcceptWnd) == 204);
#endif

#endif
