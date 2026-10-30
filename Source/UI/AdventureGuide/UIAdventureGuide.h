#ifndef RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H
#define RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

#include <string>

class CUIAdventureGuide : public CUIFrameWnd {
public:
  virtual void Slot53();

  void OnButtonClick();
  void OnSelectTab(AdventureGuide::CUITab* tab);
  void MarkQuest(int questId);

  /* 00D8 */ unsigned char unknown_00D8[0x0C];
  /* 00E4 */ CUIStaticText* titleText;
  /* 00E8 */ unsigned char unknown_00E8[0x10];
  /* 00F8 */ CUIListBox<AdventureGuide::CUITab>* tabs;
};

namespace AdventureGuide {

class CUIPage : public CUIControl {
public:
  virtual void Slot54(int first, int second) = 0;
};

class CUICharacterInfo : public CUIControl {
public:
  void Slot16() override;

  /* 0084 */ CUIStaticText* level;
  /* 0088 */ CUIStaticText* jobLevel;
  /* 008C */ CUIStaticText* name;
  /* 0080 */ CUIStaticImage* jobIcon;
};

class CUIIntro : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

  void OnSelectContents(CUITabInfo* item, const Guide_Intro::Contents& contents);
  void BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents);

  /* 0084 */ CUIStaticImage* image;
  /* 0088 */ CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents;
};

class CUIItem : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int type, int id) override;

  void OnSelectItem(CUIItemInfo* item, const CItemData& data);

  /* 0084 */ UIWindow* unknown_0084;
  /* 0088 */ UIWindow* unknown_0088;
  /* 008C */ std::string unknown_008C;
  /* 00A4 */ int location;
  /* 0080 */ CUIListBoxEx<CUIItemInfo, CItemData>* itemList;
};

class CUIMonster : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

  void OnSelectMonster(CUIMonsterInfo* item, const CMonsterData& data);
  void BindMonster(CUIMonsterInfo* item, const CMonsterData& data);

  /* 0084 */ UIWindow* unknown_0084;
  /* 0088 */ std::string unknown_0088;
  /* 0080 */ CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList;
};

class CUIQuest : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int type, int id) override;

  void OnSelectQuest(CUIQuestInfo* item, const CQuestInfo& data);

  /* 0084 */ UIWindow* unknown_0084;
  /* 0088 */ UIWindow* unknown_0088;
  /* 008C */ std::string unknown_008C;
  /* 0080 */ CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AdventureGuide::CUIIntro) == 0x8C);
static_assert(sizeof(AdventureGuide::CUIMonster) == 0xA4);
static_assert(sizeof(AdventureGuide::CUIQuest) == 0xA8);
#endif

#endif
