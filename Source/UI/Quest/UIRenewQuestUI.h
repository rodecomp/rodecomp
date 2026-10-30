#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTUI_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

namespace RenewQuestUI {
class CUIQuestItem : public CUIListItem {
public:
  void Move(int x, int y) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot48() override;

  /* 00E8 */ unsigned char unknown_00E8[0x14];
  /* 00FC */ int questType;
  /* 0100 */ int questId;
  /* 0104 */ unsigned char pressed;
  /* 0108 */ int baseX;
  /* 010C */ int baseY;
};

class CUIQuestList : public CUIControl {
public:
  void Paint() override;
};
}

class CUIRenewQuestUI : public CUIFrameWnd {
public:
  void Paint() override;
  virtual void Slot53(int value);

  void OnTabButton(CUITabButton* button);
  void OnPanelButton();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RenewQuestUI::CUIQuestItem) == 0x110);
#endif

#endif
