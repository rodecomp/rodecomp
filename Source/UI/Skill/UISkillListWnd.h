#ifndef RAGNAROK_SOURCE_UI_SKILL_UISKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_SKILL_UISKILLLISTWND_H

#include "Contents/Skills/SkillInfo.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <list>
#include <vector>

class UISkillListWnd : public UIFrameWnd {
public:
  UISkillListWnd(UIWindowContext& windows, UIFrameContext& frames);
  bool Slot03() override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int value) override;

  __declspec(noinline) int SkillAt(int x, int y);
  __declspec(noinline) CSkillInfo GetSkillByIndex(int index);
  CSkillInfo GetSkillAt(int x, int y);
  CSkillInfo GetVisibleSkill(int row);
  void UpdateScrollBar();
  void UpdateRowButtons();

  /* 00BC */ UIWindow* scrollBar;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ UIWindow* upgradeButtons[7];
  /* 00E0 */ UIWindow* levelButtons[7];
  /* 00FC */ UIWindow* levelLabels[7];
  /* 0118 */ int hovered;
  /* 011C */ int scroll;
  /* 0120 */ int columns;
  /* 0124 */ int visibleRows;
  /* 0128 */ int unknown_0128;
  /* 012C */ std::list<CSkillInfo> skills;
  /* 0134 */ SkillListWndInfo* saved;
  /* 0138 */ std::vector<MiniPartyInfoEntry> unknown_0138;
  /* 0144 */ int skillTab;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UISkillListWnd, scrollBar) == 0xBC);
static_assert(offsetof(UISkillListWnd, unknown_00C0) == 0xC0);
static_assert(offsetof(UISkillListWnd, hovered) == 0x118);
static_assert(offsetof(UISkillListWnd, skills) == 0x12C);
static_assert(offsetof(UISkillListWnd, saved) == 0x134);
static_assert(offsetof(UISkillListWnd, upgradeButtons) == 0xC4);
static_assert(offsetof(UISkillListWnd, skillTab) == 0x144);
#endif

#endif
