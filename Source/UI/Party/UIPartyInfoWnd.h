#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYINFOWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYINFOWND_H

#include "Contents/Party/AdvenPartyInfo.h"
#include "UI/Buttons/UIBitmapButton.h"

enum class PartyRole {
  Tanker,
  Dealer,
  Healer,
  Assist
};

enum AdvenPartyType : int {
  ADVEN_PARTY_ALL = 0,
  ADVEN_PARTY_QUEST = 1,
  ADVEN_PARTY_FIELD = 2,
  ADVEN_PARTY_DUNGEON = 3,
  ADVEN_PARTY_MEMORIAL_DUNGEON = 4,
  ADVEN_PARTY_EDEN_GROUP = 5,
  ADVEN_PARTY_OTHER = 6,
};

const char* PartyRoleBitmap(PartyRole role, unsigned char selected);

class UIPartyInfoWnd : public UIBitmapButton {
public:
  UIPartyInfoWnd();
  ~UIPartyInfoWnd() override;
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void SetInfo(const CAdvenPartyInfo& value);
  void Reset();
  CAdvenPartyInfo* Info();
  static __declspec(noinline) const char* TypeName(int type);

  /* 0120 */ std::string memo;
  /* 0138 */ std::string levels;
  /* 0150 */ int type;
  /* 0154 */ int tanker;
  /* 0158 */ int dealer;
  /* 015C */ int healer;
  /* 0160 */ int assist;
  /* 0164 */ std::string name;
  /* 017C */ CAdvenPartyInfo info;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIPartyInfoWnd) == 0x1C8);
#endif

#endif
