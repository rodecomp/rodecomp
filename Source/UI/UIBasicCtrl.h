#ifndef RAGNAROK_SOURCE_UI_UIBASICCTRL_H
#define RAGNAROK_SOURCE_UI_UIBASICCTRL_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"

#include <string>

struct ItemNaming;
struct ClientSkills;
struct UIButtonContext;
class WindowManagerWindows;

class UIBasicItemCtrl : public UIWindow {
public:
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;
  void DrawItemIcon(int x, int y, int unused, bool compact);

protected:
  /* 007C */ int m_itemType;
  /* 0080 */ int m_itemMask;
  /* 0084 */ unsigned char m_identified;
  /* 0088 */ std::string m_tooltip;
  /* 00A0 */ ITEM_INFO m_item;
  ItemNaming* m_naming;
  UIButtonContext* m_controls;
  WindowManagerWindows* m_windows;
};

class UIBasicSkillCtrl : public UIWindow {
public:
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot32(int x, int y) override;

protected:
  /* 007C */ int m_skillId;
  ClientSkills* m_skills;
  UIButtonContext* m_controls;
  int m_level;
};

std::string SkillUseTextPercent(const char* name, int level, int sp, int ap);
std::string SkillUseTextFlat(const char* name, int level, int sp, int ap);

#endif
