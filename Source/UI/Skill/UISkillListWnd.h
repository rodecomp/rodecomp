#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLLISTWND_H

#include "Contents/Skills/SkillInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <list>

class UISkillListWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot11() override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

  __declspec(noinline) int SkillAt(int x, int y);
  __declspec(noinline) CSkillInfo GetSkillByIndex(int index);
  CSkillInfo GetSkillAt(int x, int y);
  CSkillInfo GetVisibleSkill(int row);

  /* 00BC */ unsigned char unknown_00BC[0x5C];
  /* 0118 */ int hovered;
  /* 011C */ int scroll;
  /* 0120 */ int columns;
  /* 0124 */ int visibleRows;
  /* 0128 */ int unknown_0128;
  /* 012C */ std::list<CSkillInfo> skills;
  /* 0134 */ SkillListWndInfo* saved;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UISkillListWnd, hovered) == 0x118);
static_assert(offsetof(UISkillListWnd, skills) == 0x12C);
static_assert(offsetof(UISkillListWnd, saved) == 0x134);
#endif

#endif
