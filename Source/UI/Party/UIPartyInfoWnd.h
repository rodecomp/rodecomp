#ifndef RAGNAROK_SOURCE_UI_PARTY_UIPARTYINFOWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIPARTYINFOWND_H

#include "Contents/Party/AdvenPartyInfo.h"
#include "UI/Buttons/UIBitmapButton.h"

class UIPartyInfoWnd : public UIBitmapButton {
public:
  UIPartyInfoWnd(UIWindowContext& windows);
  ~UIPartyInfoWnd() override;
  void Paint() override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;
  void SetInfo(const CAdvenPartyInfo& value);
  void Reset();
  CAdvenPartyInfo* Info();
  static const char* TypeName(int type);

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
static_assert(sizeof(UIPartyInfoWnd) == 464);
#endif

#endif
