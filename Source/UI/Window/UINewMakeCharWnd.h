#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

class UIHairBitmap : public UIBitmapButton {
public:
  using UIBitmapButton::UIBitmapButton;

  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot29(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIHairStyleButton : public UIBitmapButton {
public:
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 0120 */ int m_style;
  /* 0124 */ int m_sex;
  /* 0128 */ int m_race;
  /* 012C */ UIBitmapButton* m_preview;
  /* 0130 */ bool m_selected;
};

class UINewMakeCharWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot19() override;

protected:
  /* 00B4 */ int m_viewX;
  /* 00B8 */ int m_viewY;
  /* 00BC */ std::string m_raceActions[5];
  /* 0134 */ std::string m_raceSprites[5];
  /* 01AC */ std::string m_raceOffsets;
  /* 01C4 */ int m_racePaletteEnabled[2];
  /* 01CC */ std::string m_racePalettes[2];
  /* 01FC */ int unknown_01FC;
  /* 0200 */ int m_action;
  /* 0204 */ int m_frame;
  /* 0208 */ unsigned char unknown_0208[0xBC];
  /* 02C4 */ int m_race;
  /* 02C8 */ UIWindow* m_raceViews[2];
};

#endif
