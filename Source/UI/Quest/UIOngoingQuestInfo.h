#ifndef RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

class CUIOngoingQuestInfo : public CUIFrameWnd {
public:
  void OnFirstAction();
};

namespace OngoingQuestInfo {

class CRewardItemData;

class CUIContents : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;

  /* 0084 */ unsigned char unknown_0084[0x08];
};

class CUIRewardItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;
};

class CUIRewards : public CUIControl {
public:
  void Paint() override;

  void OnPrepareItem(CUIRewardItem* item, const CRewardItemData& data);

  /* 0084 */ unsigned char unknown_0084[0x20];
};

class CUIScrollWindow : public CUIControl {
public:
  void Paint() override;
  void Slot35(int value) override;

  void OnScroll(int position);

  /* 0084 */ unsigned char unknown_0084[0x10];
  /* 0094 */ UIWindow* scrollBar;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(OngoingQuestInfo::CUIContents) == 0x8C);
static_assert(sizeof(OngoingQuestInfo::CUIRewards) == 0xA4);
static_assert(sizeof(OngoingQuestInfo::CUIScrollWindow) == 0x98);
#endif

#endif
