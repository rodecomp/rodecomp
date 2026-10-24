#ifndef RAGNAROK_SOURCE_UI_UIJOINPARTYACCEPTWND_H
#define RAGNAROK_SOURCE_UI_UIJOINPARTYACCEPTWND_H

#include "Contents/ItemInfo.h"
#include "UI/UIFrameWnd.h"

class UITextViewer;
struct UITextViewerContext;
struct WindowResponseRecorder;
struct NormalItemSlotWindow;

// Borrowed owners used by the shared choice/exchange operations. Inventory
// Lists are the three temporary shop lists.
struct UIAcceptanceContext {
  UITextViewerContext& text;
  WindowResponseRecorder& recorder;
  const int& screenWidth;
  const int& screenHeight;
  const int& service;
  std::list<ItemInfo>& firstShopItems;
  std::list<ItemInfo>& secondShopItems;
  std::list<ItemInfo>& thirdShopItems;
  int& replayShopState;
  std::string& replayExchangeName;
};

// RTTI UIChooseSellBuyWnd, original size 188.
class UIChooseSellBuyWnd : public UIFrameWnd {
public:
  UIChooseSellBuyWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIChooseSellBuyWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;

  /* 00B4 */ std::uint32_t identity;
  /* 00B8 */ UITextViewer* viewer;

protected:
  UIAcceptanceContext* m_acceptance;
  void AddChoice(const char* name, int id, int x, int y);
  void CreateViewer(int width, int height);
};

// RTTI UIExchangeAcceptWnd, original size 192.
class UIExchangeAcceptWnd : public UIChooseSellBuyWnd {
public:
  UIExchangeAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIExchangeAcceptWnd() override;
  void Slot15(int width, int height) override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;

  /* 00BC */ NormalItemSlotWindow* saved;
};

// RTTI UIJoinPartyAcceptWnd, original size 192.
class UIJoinPartyAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinPartyAcceptWnd(UIWindowContext& windows, UIFrameContext& frames, UIAcceptanceContext& acceptance);
  ~UIJoinPartyAcceptWnd() override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIChooseSellBuyWnd) == 200);
static_assert(sizeof(UIExchangeAcceptWnd) == 204);
static_assert(sizeof(UIJoinPartyAcceptWnd) == 204);
#endif

#endif
