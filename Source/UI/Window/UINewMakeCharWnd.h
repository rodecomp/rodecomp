#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

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

#pragma pack(push, 1)
struct MakeCharacterStatus {
  /* 0000 */ std::int64_t exp;
  /* 0008 */ unsigned char unknown_0008[0x24];
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
  void Paint() override;
  void Slot19() override;
  void sub_7DBFD0();

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
  /* 0210 */ unsigned char unknown_0210[0x04];
  /* 0214 */ MakeCharacterStatus m_status;
  /* 0264 */ short m_job;
  /* 0266 */ short m_head;
  /* 0268 */ short m_costumeJob;
  /* 026A */ short unknown_026A;
  /* 026C */ short m_level;
  /* 026E */ short m_skillPoint;
  /* 0270 */ short unknown_0270;
  /* 0272 */ short m_shield;
  /* 0274 */ short unknown_0274;
  /* 0276 */ short unknown_0276;
  /* 0278 */ short m_headPalette;
  /* 027A */ short m_bodyPalette;
  /* 027C */ char m_name[24];
  /* 0294 */ unsigned char m_stats[6];
  /* 029A */ unsigned char m_slot;
  /* 029B */ unsigned char unknown_029B[0x23];
  /* 02BE */ unsigned char m_sex;
  /* 02BF */ unsigned char unknown_02BF;
  /* 02C0 */ int unknown_02C0;
  /* 02C4 */ int m_race;
  /* 02C8 */ UIWindow* m_raceViews[2];
  /* 02D0 */ int unknown_02D0[6];
  /* 02E8 */ unsigned char unknown_02E8[0x48];
  /* 0330 */ int unknown_0330[2];
  /* 0338 */ unsigned char unknown_0338;
  /* 0339 */ unsigned char unknown_0339[0x1B];
  /* 0354 */ short unknown_0354;
  /* 0356 */ unsigned char unknown_0356[0x06];
  /* 035C */ int unknown_035C;
  /* 0360 */ unsigned long m_startTime;
  /* 0364 */ int unknown_0364;
  /* 0368 */ int unknown_0368;
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
#endif

#endif
