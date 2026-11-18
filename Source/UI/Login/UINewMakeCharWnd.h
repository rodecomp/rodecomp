#ifndef RAGNAROK_SOURCE_UI_LOGIN_UINEWMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_LOGIN_UINEWMAKECHARWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Login/UIDescriptRace.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Client/Mode/CLoginMode.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Jobs/JobInheritance.h"

#include <mbstring.h>

enum NewMakeCharButtonId : int {
  MAKECHAR_BUTTON_HAIR_STYLE = 524,
  MAKECHAR_BUTTON_GENDER = 525,
};
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

class UINewMakeCharWnd : public UIFrameWnd {
public:
  UINewMakeCharWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnProcess() override;
  void UpdatePreviewResources();
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  struct PreviewResources {
    /* 0000 */ int viewX;
    /* 0004 */ int viewY;
    /* 0008 */ std::string actions[5];
    /* 0080 */ std::string sprites[5];
    /* 00F8 */ std::string offsets;
    /* 0110 */ int paletteEnabled[2];
    /* 0118 */ std::string headPalette;
    /* 0130 */ std::string bodyPalette;
  };

  /* 00B4 */ PreviewResources m_preview;
  /* 01FC */ int actionBase;
  /* 0200 */ int m_action;
  /* 0204 */ int m_frame;
  /* 0208 */ unsigned char padding_0208[0x8];
  /* 0210 */ CharacterInfo m_character;
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
static_assert(offsetof(UINewMakeCharWnd, m_character) == 0x210);
static_assert(offsetof(UINewMakeCharWnd, m_race) == 0x2C4);
static_assert(offsetof(UINewMakeCharWnd, m_startTime) == 0x360);
static_assert(offsetof(UINewMakeCharWnd, m_hairStyleButtons) == 0x2D0);
static_assert(offsetof(UINewMakeCharWnd, m_hairColorButtons) == 0x330);
static_assert(offsetof(UINewMakeCharWnd, m_genderButtons) == 0x354);
#endif

#endif
