#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYMBWND_H

#include "Contents/Party/PartyBookingInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <string>
#include <utility>
#include <vector>

struct UIButtonContext;
struct SeekPartyMBWndInfo;
class UICheckBox;
class UIComboBoxDefWnd;
class UIEditCtrl;
class UIStaticText;

#pragma pack(push, 1)
struct SeekPartyRegistration {
  /* 0000 */ PARTY_BOOKING_DETAIL detail;
};
#pragma pack(pop)

class UISeekPartyMBWnd : public UIFrameWnd {
public:
  UISeekPartyMBWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot11() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void CreateJobChecks();
  void InitJobCategoryCombo();
  void ShowJobCategory(int category);

protected:
  /* 00BC */ UIStaticText* unknown_00BC;
  /* 00C0 */ UIStaticText* unknown_00C0;
  /* 00C4 */ UIEditCtrl* m_levelEdit;
  /* 00C8 */ UIComboBoxDefWnd* m_typeCombo;
  /* 00CC */ UIComboBoxDefWnd* m_subTypeCombo;
  /* 00D0 */ UIComboBoxDefWnd* m_jobCategoryCombo;
  /* 00D4 */ unsigned char unknown_00D4[0x30];
  /* 0104 */ std::map<unsigned short, UICheckBox*> m_jobChecks[9];
  /* 014C */ std::map<int, std::vector<std::pair<std::string, int>>> m_subTypes;
  /* 0154 */ UIWindow* unknown_0154;
  /* 0158 */ UIWindow* unknown_0158;
  /* 015C */ int m_jobCategory;
  /* 0160 */ short m_type;
  /* 0162 */ short m_subType;
  /* 0164 */ int m_restoreHeight;
  /* 0168 */ unsigned char m_minimized;
  /* 0169 */ SeekPartyRegistration m_registration;
  /* 0179 */ unsigned char unknown_0179[3];
  /* 017C */ SeekPartyMBWndInfo* m_saved;
  UIButtonContext* m_controls;
  UIStaticText* m_titleText;
  UIStaticText* m_levelText;
};

#endif
