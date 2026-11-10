#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Buttons/UIDescriptRace.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/Jobs/JobInheritance.h"

#include <mbstring.h>

#include <cstdint>

class UIHairBitmap : public UIBitmapButton {
public:
  using UIBitmapButton::UIBitmapButton;
  UIHairBitmap();

  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnMouseCheck(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
};

class UIHairStyleButton : public UIBitmapButton {
public:
  UIHairStyleButton(int style, int sex);
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 0120 */ int m_style;
  /* 0124 */ int m_sex;
  /* 0128 */ int m_race;
  /* 012C */ UIBitmapButton* m_preview;
  /* 0130 */ bool m_selected;

  friend class UINewMakeCharWnd;
};

#pragma pack(push, 1)
struct MakeCharacterStatus {
  /* 0000 */ std::int64_t exp;
  /* 0008 */ unsigned char padding_0008[0x24];
  /* 002C */ std::int16_t jobPoint;
  /* 002E */ std::int64_t hp;
  /* 0036 */ std::int64_t maxHp;
  /* 003E */ std::int64_t sp;
  /* 0046 */ std::int64_t maxSp;
  /* 004E */ std::int16_t speed;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MakeCharacterStatus) == 0x50);
#endif

class UINewMakeCharWnd : public UIFrameWnd {
public:
  UINewMakeCharWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnProcess() override;
  void UpdatePreviewResources();
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ int m_viewX;
  /* 00B8 */ int m_viewY;
  /* 00BC */ std::string m_raceActions[5];
  /* 0134 */ std::string m_raceSprites[5];
  /* 01AC */ std::string m_raceOffsets;
  /* 01C4 */ int m_racePaletteEnabled[2];
  /* 01CC */ std::string m_racePalettes[2];
  /* 01FC */ int actionBase;
  /* 0200 */ int m_action;
  /* 0204 */ int m_frame;
  /* 0208 */ unsigned char padding_0208[0x8];
  /* 0210 */ unsigned char characterData[0x04];
  /* 0214 */ MakeCharacterStatus m_status;
  /* 0264 */ short m_job;
  /* 0266 */ short m_head;
  /* 0268 */ short m_costumeJob;
  /* 026A */ short m_weapon;
  /* 026C */ short m_level;
  /* 026E */ short m_skillPoint;
  /* 0270 */ short m_accessory;
  /* 0272 */ short m_shield;
  /* 0274 */ short accessory2;
  /* 0276 */ short accessory3;
  /* 0278 */ short m_headPalette;
  /* 027A */ short m_bodyPalette;
  /* 027C */ char m_name[24];
  /* 0294 */ unsigned char m_stats[6];
  /* 029A */ unsigned char m_slot;
  /* 029B */ unsigned char padding_029B[0x23];
  /* 02BE */ unsigned char m_sex;
  /* 02BF */ unsigned char padding_02BF;
  union {
    /* 02C0 */ int nameEditStorage;
    UIEditCtrl2* m_nameEdit;
  };
  /* 02C4 */ int m_race;
  /* 02C8 */ UIWindow* m_raceViews[2];
  union {
    /* 02D0 */ int hairStyleButtonStorage[6];
    UIHairStyleButton* m_hairStyleButtons[24];
  };
  union {
    struct {
      /* 0330 */ int hairColorButtonStorage[2];
      /* 0338 */ unsigned char hairColorButtonByte;
    };
    UIBmpRadioBtn* m_hairColorButtons[9];
  };
  union {
    /* 0354 */ short genderButtonStorage;
    UIBitmapButton* m_genderButtons[2];
  };
  /* 035C */ int previewState;
  /* 0360 */ unsigned long m_startTime;
  /* 0364 */ int previewCounter;
  /* 0368 */ int previewPhase;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UINewMakeCharWnd, m_job) == 0x264);
static_assert(offsetof(UINewMakeCharWnd, m_head) == 0x266);
static_assert(offsetof(UINewMakeCharWnd, m_costumeJob) == 0x268);
static_assert(offsetof(UINewMakeCharWnd, m_headPalette) == 0x278);
static_assert(offsetof(UINewMakeCharWnd, m_bodyPalette) == 0x27A);
static_assert(offsetof(UINewMakeCharWnd, m_sex) == 0x2BE);
static_assert(offsetof(UINewMakeCharWnd, m_race) == 0x2C4);
static_assert(offsetof(UINewMakeCharWnd, m_status) == 0x214);
static_assert(offsetof(UINewMakeCharWnd, m_slot) == 0x29A);
static_assert(offsetof(UINewMakeCharWnd, m_startTime) == 0x360);
static_assert(offsetof(UINewMakeCharWnd, m_hairStyleButtons) == 0x2D0);
static_assert(offsetof(UINewMakeCharWnd, m_hairColorButtons) == 0x330);
static_assert(offsetof(UINewMakeCharWnd, m_genderButtons) == 0x354);
#endif

#endif
