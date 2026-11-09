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
  __declspec(noinline) CUIQuestItem();

  void OnCreate(int width, int height) override;
  void Move(int x, int y) override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnMouseLeave() override;
  void OnInit() override;
  void Paint() override;
  void SetSelected(bool selected) override;
  void OnActiveStateChanged(bool active);

  void OnSimpleView(bool simpleView);

  /* 00E8 */ CUIStaticImage* icon;
  /* 00EC */ CUIStaticText* titleText;
  /* 00F0 */ CUIStaticText* descriptionText;
  /* 00F4 */ CUICheckBox* simpleViewCheckBox;
  /* 00F8 */ CUICheckBox* activeCheckBox;
  /* 00FC */ int questType;
  /* 0100 */ int questId;
  /* 0104 */ unsigned char pressed;
  /* 0108 */ int baseX;
  /* 010C */ int baseY;
};

class CUIQuestList : public CUIControl {
public:
  __declspec(noinline) CUIQuestList();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnInit() override;
  void OnSelectItem(CUIQuestItem* item, const unsigned long& questId);
  void BindItem(CUIQuestItem* item, const unsigned long& questId);

  /* 0080 */ CUIListBoxEx<CUIQuestItem, unsigned long>* list;
  /* 0084 */ int type;
};
}

class CUIRenewQuestUI : public CUIFrameWnd {
public:
  __declspec(noinline) CUIRenewQuestUI();
  ~CUIRenewQuestUI() override;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void Initialize() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  bool MatchesFilter(const RenewQuestInfo& info) const;

  void OnTabButton(CUITabButton* button);
  void OnProcessTabClick();
  void OnRecommendedTabClick();
  void OnHoldTabClick();
  void OnWaitTabClick();
  void OnClose();
  void OnSearchSubmit();
  void OnSearchButton();
  void OnFilterButton(UIWindow* button);
  void OnPanelButton();
  void OnCallback6();
  void OnCallback43();
  void OnCallback50();
  void OnCallback56();
  void OnCallback74();
  void OnCallback80();
  void UpdateQuestLists(unsigned int flags);
  void SelectQuest(unsigned int questId);


  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIStaticImage* background;
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
};

template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::AddNewItem();
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::Resize(int width, int height);
template <>
void CUIListBoxEx<RenewQuestUI::CUIQuestItem, unsigned long>::LoadSkin(bool resolveSkin);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestUI::CUIQuestItem) == 0x110);
static_assert(sizeof(CUIRenewQuestUI) == 0x128);
#endif

#endif
