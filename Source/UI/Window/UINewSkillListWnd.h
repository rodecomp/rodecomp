#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H

#include "Contents/Skills/SkillInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <list>
#include <string>

struct SkillListWndInfo;
class UIScrollBar;
class UITabControl;

struct NewSkillListValue {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

class UINewSkillListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot35(int delta) override;

  std::string ValueText(int id);
  CSkillInfo VisibleSkill(int row);
  std::list<CSkillInfo>& SkillList(int tab, bool live);
  bool IsExtraTab(int tab);

protected:
  /* 00B4 */ UITabControl* m_tabs;
  /* 00B8 */ UIScrollBar* m_scrollBar;
  /* 00BC */ unsigned char unknown_00BC[0x190];
  /* 024C */ SkillListWndInfo* m_saved;
  /* 0250 */ unsigned char unknown_0250[0x4];
  /* 0254 */ int m_tab;
  /* 0258 */ int m_firstRow;
  /* 025C */ int m_columns;
  /* 0260 */ int m_visibleRows;
  /* 0264 */ unsigned char unknown_0264[0xC];
  /* 0270 */ unsigned char m_scrollable;
  /* 0271 */ unsigned char unknown_0271[0x7];
  /* 0278 */ std::list<CSkillInfo> m_lists[4];
  /* 0298 */ std::list<NewSkillListValue> m_values;
};

#endif
