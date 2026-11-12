#ifndef RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H

#include "UI/Window/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

class CUIStaticText;
class CUITextViewer;
class CUIGameItem;
class UINpcSprWnd;

namespace OngoingQuestInfo {
class CUIScrollWindow;
}

class CUIOngoingQuestInfo : public CUIFrameWnd {
public:
  __declspec(noinline) CUIOngoingQuestInfo();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void OnFirstAction();
  void OnClose();
  virtual void Initialize();
  void UpdateHuntInfo();
  void SetQuest(unsigned int id);

  /* 00D8 */ CUITitleBar* titleBar;
  union {
    /* 00DC */ unsigned char backgroundStorage[0x04];
    /* 00DC */ CUIStaticImage* background;
  };
  /* 00E0 */ CUIStaticText* titleText;
  /* 00E4 */ OngoingQuestInfo::CUIScrollWindow* scrollWindow;
  /* 00E8 */ CUIStaticText* dateText;
  union {
    /* 00EC */ unsigned char closeButtonStorage[0x04];
    /* 00EC */ CUIImageButton* closeButton;
  };
  /* 00F0 */ unsigned int questId;
};

namespace OngoingQuestInfo {

class CRewardItemData {
public:
  /* 0000 */ int itemId;
  /* 0004 */ int count;
};

class CUIContents : public CUIControl {
public:
  __declspec(noinline) CUIContents();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;

  __declspec(noinline) void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0080 */ CUIStaticImage* icon;
  /* 0084 */ CUIStaticText* title;
  /* 0088 */ CUITextViewer* textViewer;
};

class CUIRewardItem : public CUIListItem {
public:
  CUIRewardItem();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;

  /* 00E8 */ CUIStaticImage* background;
  /* 00EC */ CUIGameItem* gameItem;
  /* 00F0 */ CUIStaticText* nameText;
  /* 00F4 */ CUIStaticText* countText;
};

class CUIRewards : public CUIControl {
public:
  __declspec(noinline) CUIRewards();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;

  void OnPrepareItem(CUIRewardItem* item, const CRewardItemData& data);
  void OnBindItem(CUIRewardItem* item, const CRewardItemData& data);
  __declspec(noinline) void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0080 */ CUIStaticImage* icon;
  /* 0084 */ CUIStaticText* title;
  /* 0088 */ CUIStaticText* firstLabel;
  /* 008C */ CUIStaticText* firstText;
  /* 0090 */ CUIStaticText* secondLabel;
  /* 0094 */ CUIStaticText* secondText;
  /* 0098 */ CUIStaticText* thirdLabel;
  /* 009C */ CUIListBoxEx<CUIRewardItem, CRewardItemData>* rewardList;
  /* 00A0 */ UINpcSprWnd* npcSprite;
};

class CUIScrollWindow : public CUIControl {
public:
  __declspec(noinline) CUIScrollWindow();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void OnWheel(int value) override;

  void OnScroll(int position);
  void SetQuest(unsigned int id);

  // INLINED
  int ContentHeight() {
    int total = 0;
    for (UIWindow* child : this->m_children) {
      if (child != this->scrollBar) {
        total += child->geometry.height;
      }
    }
    return total;
  }

  /* 0080 */ CUIContents* firstContents;
  /* 0084 */ CUIStaticImage* firstLine;
  /* 0088 */ CUIContents* secondContents;
  /* 008C */ CUIStaticImage* secondLine;
  /* 0090 */ CUIRewards* rewards;
  /* 0094 */ CUIVScrollBar* scrollBar;
};

}

template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnScroll(int position);
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::AddNewItem();
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::LoadSkin(bool resolveSkin);
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::Resize(int width, int height);

template <>
__declspec(noinline) CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::CUIListBoxEx();

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(OngoingQuestInfo::CUIContents) == 0x8C);
static_assert(sizeof(OngoingQuestInfo::CUIRewards) == 0xA4);
static_assert(sizeof(OngoingQuestInfo::CUIScrollWindow) == 0x98);
static_assert(offsetof(OngoingQuestInfo::CUIRewardItem, gameItem) == 0xEC);
#endif

#endif
