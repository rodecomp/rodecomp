#ifndef RAGNAROK_SOURCE_UI_ACCEPTANCE_UIJOINPARTYACCEPTWND_H
#define RAGNAROK_SOURCE_UI_ACCEPTANCE_UIJOINPARTYACCEPTWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UITextViewer;
struct NormalItemSlotWindow;

struct UIAcceptanceContext {
  void* unknown_text;
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
  UIChooseSellBuyWnd(UIAcceptanceContext& acceptance);
  ~UIChooseSellBuyWnd() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;

  /* 00B4 */ std::uint32_t identity;
  /* 00B8 */ UITextViewer* viewer;

protected:
  UIAcceptanceContext* m_acceptance;
  void AddChoice(const char* name, int id, int x, int y);
  void CreateViewer(int width, int height);
};

class UIExchangeAcceptWnd : public UIChooseSellBuyWnd {
public:
  UIExchangeAcceptWnd(UIAcceptanceContext& acceptance);
  ~UIExchangeAcceptWnd() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void Slot11() override;
  std::vector<unsigned char> Slot44() override;

  /* 00BC */ NormalItemSlotWindow* saved;
};

class UIJoinPartyAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinPartyAcceptWnd(UIAcceptanceContext& acceptance);
  ~UIJoinPartyAcceptWnd() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;
};

class UIAllyGuildAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIAllyGuildAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIBabyAcceptWnd2 : public UIExchangeAcceptWnd {
public:
  UIBabyAcceptWnd2(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;
};

class UIBabyAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIBabyAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;
};

class UICoupleAcceptWnd : public UIExchangeAcceptWnd {
public:
  UICoupleAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIJoinFriendAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinFriendAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;

  /* 00C0 */ int m_accountId;
  /* 00C4 */ int m_characterId;
};

class UIJoinGuildAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIJoinGuildAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;
};

class UIMixAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIMixAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00C0 */ unsigned char m_kind;
};

class UIStarPlaceAcceptWnd : public UIExchangeAcceptWnd {
public:
  UIStarPlaceAcceptWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> Slot44() override;

  /* 00C0 */ char m_place;
};

class UIYourItemWnd : public UIExchangeAcceptWnd {
public:
  UIYourItemWnd(UIAcceptanceContext& acceptance);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00C0 */ int m_index;
  /* 00C4 */ int m_location;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIChooseSellBuyWnd) == 200);
static_assert(sizeof(UIExchangeAcceptWnd) == 204);
static_assert(sizeof(UIJoinPartyAcceptWnd) == 204);
#endif

#endif
