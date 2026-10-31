#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct RenewQuestInfo;

class CUICheckBox;
class CUIEditBoxEx;
class CUIStaticText;

namespace RenewQuestUI {
class CUIQuestItem : public CUIListItem {
public:
  __declspec(noinline) explicit CUIQuestItem(UIWindowContext& context);

  void Slot15(int width, int height) override;
  void Move(int x, int y) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot48() override;
  void Slot16() override;
  void Paint() override;
  void SetSelected(bool selected) override;
  void OnActiveStateChanged(bool active);

  void OnSimpleView(bool simpleView);

  /* 00E8 */ CUIStaticImage* icon;
  /* 00EC */ CUIStaticText* unknown_00EC;
  /* 00F0 */ CUIStaticText* unknown_00F0;
  /* 00F4 */ CUICheckBox* unknown_00F4;
  /* 00F8 */ CUICheckBox* unknown_00F8;
  /* 00FC */ int questType;
  /* 0100 */ int questId;
  /* 0104 */ unsigned char pressed;
  /* 0108 */ int baseX;
  /* 010C */ int baseY;
};

class CUIQuestList : public CUIControl {
public:
  __declspec(noinline) explicit CUIQuestList(UIWindowContext& context);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot16() override;
  void OnSelectItem(CUIQuestItem* item, const unsigned long& questId);
  void BindItem(CUIQuestItem* item, const unsigned long& questId);

  /* 0080 */ CUIListBoxEx<CUIQuestItem, unsigned long>* list;
  /* 0084 */ int type;
};
}

class CUIRenewQuestUI : public CUIFrameWnd {
public:
  __declspec(noinline) CUIRenewQuestUI();

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  virtual void Slot53(int value);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool MatchesFilter(const RenewQuestInfo& info) const;

  void OnTabButton(CUITabButton* button);
  void OnFilterButton(UIWindow* button);
  void OnPanelButton();
  void OnCallback6();
  void OnCallback43();
  void OnCallback50();
  void OnCallback56();
  void OnCallback74();
  void OnCallback80();
  void sub_A56050(unsigned int questId);
  void sub_A67290(unsigned int flags);
  void sub_A66FF0(unsigned int questId);

  /* 00E0 */ CUITab* tab;
  /* 00E4 */ CUICheckBox* checkBox;
  /* 00E8 */ CUIStaticText* filterLabel;
  /* 00EC */ CUIImageButton* closeButton;
  /* 00F0 */ std::vector<RenewQuestUI::CUIQuestList*> questLists;
  /* 00FC */ CUIStaticImage* searchBackground;
  /* 0100 */ CUITab* filterPanel;
  /* 0104 */ CUIEditBoxEx* searchEdit;
  /* 0108 */ CUIImageButton* searchButton;
  /* 010C */ int filter;
  /* 0110 */ std::string searchText;
  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIStaticImage* background;
};

template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddNewItem();
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Resize(int width, int height);
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Slot53(int resolveSkin);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestUI::CUIQuestItem) == 0x110);
static_assert(sizeof(CUIRenewQuestUI) == 0x130);
#endif

#endif
