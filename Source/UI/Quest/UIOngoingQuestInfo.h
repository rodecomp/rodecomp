#ifndef RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H
#define RAGNAROK_SOURCE_UI_QUEST_UIONGOINGQUESTINFO_H

#include "UI/FrameSkin.h"
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
  void Slot15(int width, int height) override;
  void Slot16() override;
  void OnFirstAction();
  void OnClose();
  virtual void Slot53();
  void sub_A55B70();
  void SetQuest(unsigned int id);

  /* 00D8 */ CUITitleBar* titleBar;
  union {
    /* 00DC */ unsigned char unknown_00DC[0x04];
    /* 00DC */ CUIStaticImage* background;
  };
  /* 00E0 */ CUIStaticText* titleText;
  /* 00E4 */ OngoingQuestInfo::CUIScrollWindow* scrollWindow;
  /* 00E8 */ CUIStaticText* dateText;
  union {
    /* 00EC */ unsigned char unknown_00EC[0x04];
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
  __declspec(noinline) explicit CUIContents(UIWindowContext& context);

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  __declspec(noinline) void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0084 */ CUIStaticText* title;
  /* 0088 */ CUITextViewer* textViewer;
  /* 0080 */ CUIStaticImage* icon;
};

class CUIRewardItem : public CUIListItem {
public:
  explicit CUIRewardItem(UIWindowContext& context);
  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  /* 00E8 */ CUIStaticImage* background;
  /* 00EC */ CUIGameItem* gameItem;
  /* 00F0 */ CUIStaticText* nameText;
  /* 00F4 */ CUIStaticText* countText;
};

class CUIRewards : public CUIControl {
public:
  __declspec(noinline) explicit CUIRewards(UIWindowContext& context);

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;

  void OnPrepareItem(CUIRewardItem* item, const CRewardItemData& data);
  void OnBindItem(CUIRewardItem* item, const CRewardItemData& data);
  __declspec(noinline) void UpdateHeight();

  // INLINED
  void EnableClip() {
    this->m_clip = 1;
  }

  /* 0084 */ CUIStaticText* title;
  /* 0088 */ CUIStaticText* firstLabel;
  /* 008C */ CUIStaticText* firstText;
  /* 0090 */ CUIStaticText* secondLabel;
  /* 0094 */ CUIStaticText* secondText;
  /* 0098 */ CUIStaticText* thirdLabel;
  /* 009C */ CUIListBoxEx<CUIRewardItem, CRewardItemData>* rewardList;
  /* 00A0 */ UINpcSprWnd* npcSprite;
  /* 0080 */ CUIStaticImage* icon;
};

class CUIScrollWindow : public CUIControl {
public:
  __declspec(noinline) explicit CUIScrollWindow(UIWindowContext& context);

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot35(int value) override;

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

  /* 0084 */ CUIStaticImage* firstLine;
  /* 0088 */ CUIContents* secondContents;
  /* 008C */ CUIStaticImage* secondLine;
  /* 0090 */ CUIRewards* rewards;
  /* 0094 */ CUIVScrollBar* scrollBar;
  /* 0080 */ CUIContents* firstContents;
};

}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(OngoingQuestInfo::CUIContents) == 0x90);
static_assert(sizeof(OngoingQuestInfo::CUIRewards) == 0xA8);
static_assert(sizeof(OngoingQuestInfo::CUIScrollWindow) == 0x9C);
static_assert(offsetof(OngoingQuestInfo::CUIRewardItem, gameItem) == 0xEC);
#endif

#endif
