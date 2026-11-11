#ifndef RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H
#define RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H

#include "UI/Window/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIControl.h"
#include "UI/Navigation/UINavigationWnd.h"

#include <string>

namespace AdventureGuide {
class CUICharacterInfo;
class CUIIntro;
class CUIItem;
class CUIMonster;
class CUIQuest;
}

class CUIAdventureGuide : public CUIFrameWnd {
public:
  __declspec(noinline) CUIAdventureGuide();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  virtual void Initialize();

  void OnButtonClick();
  void OnClose();
  void OnSelectTab(AdventureGuide::CUITab* tab);
  void MarkQuest(int questId);

  /* 00D8 */ CUIStaticImage* background;
  /* 00DC */ CUITitleBar* titleBar;
  /* 00E0 */ AdventureGuide::CUICharacterInfo* characterInfo;
  /* 00E4 */ CUIStaticText* titleText;
  /* 00E8 */ AdventureGuide::CUIIntro* intro;
  /* 00EC */ AdventureGuide::CUIQuest* quest;
  /* 00F0 */ AdventureGuide::CUIMonster* monster;
  /* 00F4 */ AdventureGuide::CUIItem* item;
  /* 00F8 */ CUIListBox<AdventureGuide::CUITab>* tabs;
};

namespace AdventureGuide {

class CUIPage : public CUIControl {
public:
  explicit CUIPage() : CUIControl() {
  }

  virtual void LoadPage(int first, int second) = 0;
};

class CUICharacterInfo : public CUIControl {
public:
  // OFFSET: 0x009D8F50
  // FUNCTION: fn_14273
  __declspec(noinline) explicit CUICharacterInfo() : CUIControl(), jobIcon(nullptr), level(nullptr), jobLevel(nullptr), name(nullptr) {
  }

  void OnCreate(int width, int height) override;
  void OnInit() override;

  /* 0080 */ CUIStaticImage* jobIcon;
  /* 0084 */ CUIStaticText* level;
  /* 0088 */ CUIStaticText* jobLevel;
  /* 008C */ CUIStaticText* name;
};

class CUIIntro : public CUIPage {
public:
  // OFFSET: 0x009D8FE0
  // FUNCTION: fn_14274
  __declspec(noinline) explicit CUIIntro() : CUIPage(), banner(nullptr), image(nullptr), contents(nullptr) {
  }

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void LoadPage(int first, int second) override;

  void OnSelectContents(CUITabInfo* item, const Guide_Intro::Contents& contents);
  void BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents);

  /* 0080 */ CUIStaticImage* banner;
  /* 0084 */ CUIStaticImage* image;
  /* 0088 */ CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents;
};

class CUIItem : public CUIPage {
public:
  // OFFSET: 0x009D9080
  // FUNCTION: fn_14275
  __declspec(noinline) explicit CUIItem()
      : CUIPage(), itemList(nullptr), findButton(nullptr), navigationButton(nullptr), navigationTarget(), location(-1) {
  }

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void LoadPage(int type, int id) override;
  void BindItem(CUIItemInfo* item, const CItemData& data);
  void OnSelectItem(CUIItemInfo* item, const CItemData& data);
  void RequestNavigation();
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIItemInfo, CItemData>* itemList;
  /* 0084 */ CUIImageButton* findButton;
  /* 0088 */ CUIImageButton* navigationButton;
  /* 008C */ std::string navigationTarget;
  /* 00A4 */ int location;
};

class CUIMonster : public CUIPage {
public:
  // OFFSET: 0x009D9220
  // FUNCTION: fn_14277
  __declspec(noinline) explicit CUIMonster() : CUIPage(), monsterList(nullptr), navigationButton(nullptr), navigationTarget() {
  }

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void LoadPage(int first, int second) override;

  void OnSelectMonster(CUIMonsterInfo* item, const CMonsterData& data);
  void BindMonster(CUIMonsterInfo* item, const CMonsterData& data);
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList;
  /* 0084 */ CUIImageButton* navigationButton;
  /* 0088 */ std::string navigationTarget;
};

class CUIQuest : public CUIPage {
public:
  // OFFSET: 0x009D9360
  // FUNCTION: fn_14279
  __declspec(noinline) explicit CUIQuest() : CUIPage(), questList(nullptr), findButton(nullptr), navigationButton(nullptr), navigationTarget() {
  }

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void LoadPage(int type, int id) override;

  void OnSelectQuest(CUIQuestInfo* item, const CQuestInfo& data);
  void RequestNavigation();
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList;
  /* 0084 */ CUIImageButton* findButton;
  /* 0088 */ CUIImageButton* navigationButton;
  /* 008C */ std::string navigationTarget;
};

}

template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::AddNewItem();
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::AddNewItem();
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::AddNewItem();
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::AddNewItem();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AdventureGuide::CUIIntro) == 0x8C);
static_assert(sizeof(AdventureGuide::CUIMonster) == 0xA0);
static_assert(sizeof(AdventureGuide::CUIQuest) == 0xA4);
#endif

#endif
