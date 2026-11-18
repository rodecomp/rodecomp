#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBASICCTRL_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBASICCTRL_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIWindow.h"

#include <cstdint>
#include <string>

#include "UI/Window/WindowManagerWindows.h"

class UIBasicItemCtrl : public UIWindow {
public:
  UIBasicItemCtrl();
  __declspec(noinline) ~UIBasicItemCtrl() override;
  void SetItemById(int id, unsigned char identified);
  void SetItemByIdentity(std::uint32_t identity, unsigned char identified);
  void DragAndDrop(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void DrawItemBackground(int x, int y);
  void DrawItemIcon(int x, int y, int unused, bool compact);

protected:
  /* 007C */ int m_itemType;
  /* 0080 */ int m_itemMask;
  /* 0084 */ unsigned char m_identified;
  /* 0088 */ std::string m_tooltip;
  /* 00A0 */ ITEM_INFO m_item;
};

class UIBasicSkillCtrl : public UIWindow {
public:
  UIBasicSkillCtrl();
  __declspec(noinline) ~UIBasicSkillCtrl() override;
  void DragAndDrop(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;

protected:
  /* 007C */ int m_skillId;
  /* 0080 */ int m_level;
};

std::string SkillUseTextPercent(const char* name, int level, int sp, int ap);
std::string SkillUseTextFlat(const char* name, int level, int sp, int ap);

#endif
