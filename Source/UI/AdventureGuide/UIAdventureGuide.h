#ifndef RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H
#define RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Navigation/UINavigationV4Wnd.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UINavigationWnd.h"

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
  void Slot15(int width, int height) override;
  void Slot16() override;
  virtual void Slot53();

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

  virtual void Slot54(int first, int second) = 0;
};

class CUICharacterInfo : public CUIControl {
public:
  // OFFSET: 0x009D8F50
  // FUNCTION: fn_14273
  explicit CUICharacterInfo() : CUIControl(), jobIcon(nullptr), level(nullptr), jobLevel(nullptr), name(nullptr) {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;

  /* 0080 */ CUIStaticImage* jobIcon;
  /* 0084 */ CUIStaticText* level;
  /* 0088 */ CUIStaticText* jobLevel;
  /* 008C */ CUIStaticText* name;
};

class CUIIntro : public CUIPage {
public:
  // OFFSET: 0x009D8FE0
  // FUNCTION: fn_14274
  explicit CUIIntro() : CUIPage(), banner(nullptr), image(nullptr), contents(nullptr) {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

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
  explicit CUIItem()
      : CUIPage(), itemList(nullptr), unknown_0084(nullptr), unknown_0088(nullptr), unknown_008C(), location(-1) {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot54(int type, int id) override;
  void BindItem(CUIItemInfo* item, const CItemData& data);
  void OnSelectItem(CUIItemInfo* item, const CItemData& data);
  void RequestNavigation();
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIItemInfo, CItemData>* itemList;
  /* 0084 */ CUIImageButton* unknown_0084;
  /* 0088 */ CUIImageButton* unknown_0088;
  /* 008C */ std::string unknown_008C;
  /* 00A4 */ int location;
};

class CUIMonster : public CUIPage {
public:
  // OFFSET: 0x009D9220
  // FUNCTION: fn_14277
  explicit CUIMonster() : CUIPage(), monsterList(nullptr), unknown_0084(nullptr), unknown_0088() {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

  void OnSelectMonster(CUIMonsterInfo* item, const CMonsterData& data);
  void BindMonster(CUIMonsterInfo* item, const CMonsterData& data);
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList;
  /* 0084 */ CUIImageButton* unknown_0084;
  /* 0088 */ std::string unknown_0088;
};

class CUIQuest : public CUIPage {
public:
  // OFFSET: 0x009D9360
  // FUNCTION: fn_14279
  explicit CUIQuest() : CUIPage(), questList(nullptr), unknown_0084(nullptr), unknown_0088(nullptr), unknown_008C() {
  }

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot54(int type, int id) override;

  void OnSelectQuest(CUIQuestInfo* item, const CQuestInfo& data);
  void RequestNavigation();
  void SearchNavigation();

  /* 0080 */ CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList;
  /* 0084 */ CUIImageButton* unknown_0084;
  /* 0088 */ CUIImageButton* unknown_0088;
  /* 008C */ std::string unknown_008C;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AdventureGuide::CUIIntro) == 0x8C);
static_assert(sizeof(AdventureGuide::CUIMonster) == 0xA0);
static_assert(sizeof(AdventureGuide::CUIQuest) == 0xA4);
#endif

#endif
