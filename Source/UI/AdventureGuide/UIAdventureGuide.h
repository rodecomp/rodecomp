#ifndef RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H
#define RAGNAROK_SOURCE_UI_ADVENTUREGUIDE_UIADVENTUREGUIDE_H

#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

#include <string>

namespace AdventureGuide {

class CUIPage : public CUIControl {
public:
  virtual void Slot54(int first, int second) = 0;
};

class CUIIntro : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

  /* 0084 */ CUIStaticImage* image;
  /* 0088 */ CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents;
};

class CUIItem : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;

  /* 0084 */ unsigned char unknown_0084[0x08];
  /* 008C */ std::string unknown_008C;
  /* 00A4 */ unsigned char unknown_00A4[0x04];
};

class CUIMonster : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;
  void Slot54(int first, int second) override;

  /* 0084 */ UIWindow* unknown_0084;
  /* 0088 */ std::string unknown_0088;
  /* 0080 */ CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList;
};

class CUIQuest : public CUIPage {
public:
  void Slot16() override;
  void Paint() override;

  /* 0084 */ unsigned char unknown_0084[0x08];
  /* 008C */ std::string unknown_008C;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AdventureGuide::CUIIntro) == 0x8C);
static_assert(sizeof(AdventureGuide::CUIItem) == 0xA8);
static_assert(sizeof(AdventureGuide::CUIMonster) == 0xA4);
static_assert(sizeof(AdventureGuide::CUIQuest) == 0xA4);
#endif

#endif
