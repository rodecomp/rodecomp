#ifndef RAGNAROK_SOURCE_UI_UIROUNDBOXTOOLTIPWND_H
#define RAGNAROK_SOURCE_UI_UIROUNDBOXTOOLTIPWND_H

#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIFrameWnd.h"

class UIColoringTextViewer : public UIListBox {
public:
  UIColoringTextViewer(UIWindowContext& windows, UIListBoxContext& list);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00D4 */ std::uint32_t color;
  /* 00D8 */ int lineLimit;
  /* 00DC */ std::uint32_t backgroundColor;
};

class UIRoundBoxTooltipWnd : public UIFrameWnd {
public:
  UIRoundBoxTooltipWnd(UIWindowContext& windows, UIFrameContext& frames, UIListBoxContext& list);
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00B4 */ UIColoringTextViewer* viewer;

protected:
  UIListBoxContext* m_listContext;
};

#endif
