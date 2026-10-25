#ifndef RAGNAROK_SOURCE_UI_PARTY_UIREGISTERPARTYWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIREGISTERPARTYWND_H

#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Window/UIFrameWnd.h"

class UIComboBoxDef2Wnd;
class UIEditCtrl;
class UIJobSlotButton;
class UIStaticText;
class UIBasicButton;
struct ForbiddenWords;
struct AgencyRequestContext;
struct RegisterPartyContext;
struct UIEditContext;
struct UIComboBoxHeaderContext;

struct UIRegisterPartyContext {
  UIButtonContext& buttons;
  UIEditContext& edits;
  UIComboBoxHeaderContext& combos;
  ForbiddenWords& forbidden;
  AgencyRequestContext& requests;
  RegisterPartyContext& completion;
  const int& playerLevel;
  const int& partyMember;
  const int& screenWidth;
  const int& screenHeight;
  const std::string& characterName;
  const unsigned short& codePage;
};

class UIRegisterPartyWnd : public UIFrameWnd {
public:
  UIRegisterPartyWnd(UIWindowContext& windows, UIFrameContext& frames, UIRegisterPartyContext& registration);
  ~UIRegisterPartyWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetInfo(CAdvenPartyInfo value);

  /* 00B4 */ UIComboBoxDef2Wnd* types;
  /* 00B8 */ UIEditCtrl* minimum;
  /* 00BC */ UIEditCtrl* maximum;
  /* 00C0 */ UIEditCtrl* memo;
  /* 00C4 */ UIJobSlotButton* jobs[4];
  /* 00D4 */ int selectedType;
  /* 00D8 */ UIStaticText* character;
  /* 00DC */ UIBasicButton* recruit;
  /* 00E0 */ UIBasicButton* change;
  /* 00E4 */ PartyBoardPlacement* saved;

private:
  UIRegisterPartyContext* m_registration;
  const char* Text(UIEditCtrl& control) const;
  void NormalizeLevels();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRegisterPartyWnd) == 244);
#endif

#endif
