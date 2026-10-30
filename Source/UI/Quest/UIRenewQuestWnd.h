#ifndef RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTWND_H
#define RAGNAROK_SOURCE_UI_QUEST_UIRENEWQUESTWND_H

#include "UI/Lists/UIScrollBar.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <string>
#include <vector>

class UIBitmapButton;
class UICheckBox;
class UIStaticBitmap;
struct UIButtonContext;

class UIRecommededQuestScrollWnd : public UIScrollWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot35(int delta) override;

  /* 00B0 */ int unknown_00B0;
  /* 00B4 */ int unknown_00B4;
};

class UIRenewDetailQuestInfoWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 00B4 */ UIWindow* m_ongoingWnd;
  /* 00B8 */ UIRecommededQuestScrollWnd* m_recommendedWnd;
  /* 00BC */ UIBitmapButton* m_closeButton;
  /* 00C0 */ unsigned int m_questId;
  /* 00C4 */ std::string m_timeText;
  /* 00DC */ std::string m_imageName;
  /* 00F4 */ bool m_recommended;

protected:
  UIButtonContext* m_controls;
};

class UIRenewQuestScrollBar : public UIScrollBar {
public:
  void Paint() override;
};

struct RenewQuestRowControls {
  /* 0000 */ UICheckBox* checkBoxes[9];
  /* 0024 */ UICheckBox* lockBoxes[9];
  /* 0048 */ UIStaticBitmap* icons[9];
};

class UIRenewQuestWnd : public UIFrameWnd {
public:
  UIRenewQuestWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  /* 00B4 */ UIWindow* m_tabs;
  /* 00B8 */ UIRenewQuestScrollBar* m_scrollBar;
  /* 00BC */ UICheckBox* m_optionCheckBox;
  /* 00C0 */ UIBitmapButton* m_closeButton;
  /* 00C4 */ RenewQuestRowControls m_rows;
  /* 0130 */ int m_tab;
  /* 0134 */ int m_scroll;
  /* 0138 */ int m_selected;
  /* 013C */ bool m_scrollable;
  /* 0140 */ int m_pressed;
  /* 0144 */ std::vector<unsigned int> m_questIds[4];

protected:
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIRecommededQuestScrollWnd, unknown_00B0) == 0xB0);
static_assert(sizeof(UIRecommededQuestScrollWnd) == 0xB8);
static_assert(sizeof(UIRenewQuestScrollBar) == sizeof(UIScrollBar));
static_assert(offsetof(RenewQuestRowControls, lockBoxes) == 0x24);
static_assert(offsetof(RenewQuestRowControls, icons) == 0x48);
static_assert(sizeof(RenewQuestRowControls) == 0x6C);
#endif

#endif
