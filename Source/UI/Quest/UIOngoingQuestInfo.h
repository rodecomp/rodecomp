#ifndef RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

class CUIStaticText;
class CUITextViewer;
class CUIGameItem;

class CUIOngoingQuestInfo : public CUIFrameWnd {
public:
  void Slot16() override;
  void OnFirstAction();
  virtual void Slot53();
  void sub_A55B70();

  /* 00D8 */ CUITitleBar* titleBar;
};

namespace OngoingQuestInfo {

class CRewardItemData {
public:
  /* 0000 */ int itemId;
  /* 0004 */ int count;
};

class CUIContents : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;

  void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0084 */ CUIStaticText* title;
  /* 0088 */ CUITextViewer* textViewer;
};

class CUIRewardItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ unsigned char unknown_00E8[0x04];
  /* 00EC */ CUIGameItem* gameItem;
  /* 00F0 */ CUIStaticText* nameText;
  /* 00F4 */ CUIStaticText* countText;
};

class CUIRewards : public CUIControl {
public:
  void Paint() override;

  void OnPrepareItem(CUIRewardItem* item, const CRewardItemData& data);
  void OnBindItem(CUIRewardItem* item, const CRewardItemData& data);
  void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0084 */ CUIStaticText* title;
  /* 0088 */ unsigned char unknown_0088[0x14];
  /* 009C */ CUIListBoxEx<CUIRewardItem, CRewardItemData>* rewardList;
  /* 00A0 */ unsigned char unknown_00A0[0x04];
};

class CUIScrollWindow : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
  void Slot35(int value) override;

  void OnScroll(int position);

  /* 0084 */ unsigned char unknown_0084[0x04];
  /* 0088 */ CUIContents* secondContents;
  /* 008C */ unsigned char unknown_008C[0x04];
  /* 0090 */ CUIRewards* rewards;
  /* 0094 */ UIWindow* scrollBar;
  /* 0080 */ CUIContents* firstContents;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(OngoingQuestInfo::CUIContents) == 0x8C);
static_assert(sizeof(OngoingQuestInfo::CUIRewards) == 0xA4);
static_assert(sizeof(OngoingQuestInfo::CUIScrollWindow) == 0x9C);
static_assert(offsetof(OngoingQuestInfo::CUIRewardItem, gameItem) == 0xEC);
#endif

#endif
