#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

struct RenewQuestInfo;

namespace RenewQuestUI {
class CUIQuestItem : public CUIListItem {
public:
  void Move(int x, int y) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot48() override;
  void Slot16() override;

  void OnSimpleView(bool simpleView);

  /* 00E8 */ unsigned char unknown_00E8[0x0C];
  /* 00F4 */ CUIButton* unknown_00F4;
  /* 00F8 */ CUIButton* unknown_00F8;
  /* 00FC */ int questType;
  /* 0100 */ int questId;
  /* 0104 */ unsigned char pressed;
  /* 0108 */ int baseX;
  /* 010C */ int baseY;
};

class CUIQuestList : public CUIControl {
public:
  void Paint() override;
  void Slot16() override;

  /* 0080 */ CUIListBoxEx<CUIQuestItem, unsigned long>* list;
};
}

class CUIRenewQuestUI : public CUIFrameWnd {
public:
  void Paint() override;
  virtual void Slot53(int value);
  bool MatchesFilter(const RenewQuestInfo& info) const;

  void OnTabButton(CUITabButton* button);
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

  /* 00E0 */ unsigned char unknown_00E0[0x2C];
  /* 010C */ int filter;
  /* 0110 */ std::string searchText;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestUI::CUIQuestItem) == 0x110);
static_assert(sizeof(CUIRenewQuestUI) == 0x128);
#endif

#endif
