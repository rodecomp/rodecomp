#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSKILLLISTWND_H

#include "Contents/Skills/SkillInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <list>
#include <string>
#include <utility>

struct SkillListWndInfo;
struct UIButtonContext;
class UIBitmapButton;
class UICheckBox;
class UIScrollBar;
class UITabControl;

struct NewSkillListValue {
  /* 0000 */ int id;
  /* 0004 */ int value;
};

class UINewSkillListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  std::string ValueText(int id);
  CSkillInfo VisibleSkill(int row);
  std::list<CSkillInfo>& SkillList(int tab, bool live);
  bool IsExtraTab(int tab);
  CSkillInfo SkillAt(int index, bool live);
  std::pair<CSkillInfo*, bool> SkillPtr(int index, bool live);
  void InitTabs();
  void ResetLists();
  bool IsSkillArea(int x, int y);
  void sub_9B2CD0();
  void UpdateScrollBar();
  void LayoutButtons();
  int SkillIndexAt(int x, int y);
  bool sub_9B3EB0(int skillId);
  void sub_9AE930();
  void sub_9AF300();
  void sub_9AF960();
  void sub_9AE730();

public:
  /* 00B4 */ UITabControl* m_tabs;
  /* 00B8 */ UIScrollBar* m_scrollBar;
  /* 00BC */ UIBitmapButton* m_btnHelp;
  /* 00C0 */ UIBitmapButton* m_btnMinimize;
  /* 00C4 */ UIBitmapButton* m_btnClose;
  /* 00C8 */ UIBitmapButton* m_btnLevelUp[10];
  /* 00F0 */ UIBitmapButton* m_btnSkills[42];
  /* 0198 */ UIBitmapButton* m_btnSkillUps[42];
  /* 0240 */ UIBitmapButton* m_btnApply;
  /* 0244 */ UIBitmapButton* m_btnCancel;
  /* 0248 */ UICheckBox* m_btnDetail;
  /* 024C */ SkillListWndInfo* m_saved;
  /* 0250 */ int m_selectedSkill;
  /* 0254 */ int m_tab;
  /* 0258 */ int m_firstRow;
  /* 025C */ int m_columns;
  /* 0260 */ int m_visibleRows;
  /* 0264 */ int m_topMargin;
  /* 0268 */ int m_bottomMargin;
  /* 026C */ int m_skillPoints;
  /* 0270 */ unsigned char m_scrollable;
  /* 0271 */ unsigned char m_needsRedraw;
  /* 0272 */ unsigned char m_locked;
  /* 0273 */ unsigned char unknown_0273;
  /* 0274 */ std::uint32_t m_lastTick;
  /* 0278 */ std::list<CSkillInfo> m_lists[4];
  /* 0298 */ std::list<NewSkillListValue> m_values;
  /* 02A0 */ std::list<NewSkillListValue> m_unknown_02A0;
  /* 02A8 */ bool m_showDetail;
  /* 02A9 */ unsigned char unknown_02A9[3];
  /* 02AC */ int m_detailId;

protected:
  UIButtonContext* m_controls;
};

#endif
