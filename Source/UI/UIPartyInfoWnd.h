#ifndef RAGNAROK_SOURCE_UI_UIPARTYINFOWND_H
#define RAGNAROK_SOURCE_UI_UIPARTYINFOWND_H

#include "Contents/AdvenPartyInfo.h"
#include "UI/UIBitmapButton.h"

// Original client friend/guild/name-tooltip operations remain at their owners.
class PartyRowServices {
public:
  virtual bool IsFriend(const std::string& name) = 0;
  virtual bool IsGuildMember(std::uint32_t accountId) = 0;
  virtual std::string CharacterCode(std::uint32_t characterId) = 0;
  virtual void ShowCharacter(const char* name, const char* relationship, const char* code) = 0;

protected:
  ~PartyRowServices() = default;
};

struct PartyRowContext {
  const std::uint32_t& accountId;
  const std::uint32_t& characterId;
  PartyRowServices& services;
};

// RTTI UIPartyInfoWnd, original x86 size 456.
class UIPartyInfoWnd : public UIBitmapButton {
public:
  UIPartyInfoWnd(UIWindowContext& windows, UIButtonContext& buttons, PartyRowContext& row);
  ~UIPartyInfoWnd() override;
  void Paint() override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;
  void SetInfo(const CAdvenPartyInfo& value);
  void Reset();
  static const char* TypeName(int type, ButtonServices& services);

  /* 0120 */ std::string memo;
  /* 0138 */ std::string levels;
  /* 0150 */ int type;
  /* 0154 */ int tanker;
  /* 0158 */ int dealer;
  /* 015C */ int healer;
  /* 0160 */ int assist;
  /* 0164 */ std::string name;
  /* 017C */ CAdvenPartyInfo info;

private:
  PartyRowContext* m_row;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIPartyInfoWnd) == 468);
#endif

#endif
