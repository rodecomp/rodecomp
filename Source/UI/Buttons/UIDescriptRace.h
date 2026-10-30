#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIDESCRIPTRACE_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIDESCRIPTRACE_H

#include "UI/Buttons/UIBitmapButton.h"

#include <cstdint>
#include <string>

struct UIDescriptRaceText {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ std::string names[5];
  /* 0080 */ std::string descriptions[5];
  /* 00F8 */ std::string title;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int unknown_0114;
  /* 0118 */ std::string unknown_0118;
  /* 0130 */ std::string unknown_0130;
};

// VTABLE: vt_00998
class UIDescriptRace : public UIBitmapButton {
public:
  explicit UIDescriptRace(int race);
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 0120 */ int m_race;
  /* 0124 */ UIDescriptRaceText m_text;
  /* 026C */ unsigned char unknown_026C[0x14];
  /* 0280 */ unsigned char m_noRace;
  /* 0284 */ int unknown_0284;
  /* 0288 */ std::uint32_t m_createTime;
};

extern unsigned char g_doramCreationEnabled;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIDescriptRaceText) == 0x148);
#endif

#endif
