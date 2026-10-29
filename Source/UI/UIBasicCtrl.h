#ifndef RAGNAROK_SOURCE_UI_UIBASICCTRL_H
#define RAGNAROK_SOURCE_UI_UIBASICCTRL_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"

#include <string>

struct ItemNaming;
struct ClientSkills;
struct UIButtonContext;

class UIBasicItemCtrl : public UIWindow {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ unsigned char unknown_007C[0xC];
  /* 0088 */ std::string m_tooltip;
  /* 00A0 */ ITEM_INFO m_item;
  ItemNaming* m_naming;
  UIButtonContext* m_controls;
};

class UIBasicSkillCtrl : public UIWindow {
public:
  void Slot28(int x, int y) override;

protected:
  /* 007C */ int m_skillId;
  ClientSkills* m_skills;
  UIButtonContext* m_controls;
};

std::string SkillUseTextPercent(const char* name, int level, int sp, int ap);
std::string SkillUseTextFlat(const char* name, int level, int sp, int ap);

#endif
