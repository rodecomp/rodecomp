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
  UISkillListWnd();
  bool NeedsFrameRefresh() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  __declspec(noinline) int SkillAt(int x, int y);
  __declspec(noinline) CSkillInfo GetSkillByIndex(int index);
  CSkillInfo GetSkillAt(int x, int y);
  CSkillInfo GetVisibleSkill(int row);
  void UpdateScrollBar();
  void UpdateRowButtons();

  /* 00B4 */ UIWindow* resizer;
  /* 00B8 */ UIWindow* scrollBar;
  /* 00BC */ UIWindow* buttons[2];
  /* 00C4 */ UIWindow* upgradeButtons[7];
  /* 00E0 */ UIWindow* levelButtons[7];
  /* 00FC */ UIWindow* levelLabels[7];
  /* 0118 */ int hovered;
  /* 011C */ int scroll;
  /* 0120 */ int columns;
  /* 0124 */ int visibleRows;
  /* 0128 */ int hasVerticalOverflow;
  /* 012C */ std::list<CSkillInfo> skills;
  /* 0134 */ SkillListWndInfo* saved;
  /* 0138 */ std::vector<MiniPartyInfoEntry> contextMenuEntries;
  /* 0144 */ int skillTab;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UISkillListWnd, scrollBar) == 0xB8);
static_assert(offsetof(UISkillListWnd, buttons) == 0xBC);
static_assert(offsetof(UISkillListWnd, hovered) == 0x118);
static_assert(offsetof(UISkillListWnd, skills) == 0x12C);
static_assert(offsetof(UISkillListWnd, saved) == 0x134);
static_assert(offsetof(UISkillListWnd, upgradeButtons) == 0xC4);
static_assert(offsetof(UISkillListWnd, skillTab) == 0x144);
#endif

#endif
