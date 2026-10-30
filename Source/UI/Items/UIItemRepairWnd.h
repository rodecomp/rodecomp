#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMREPAIRWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <vector>

struct UIButtonContext;
struct UIListBoxContext;
class UIListBox;

struct UIItemRepairEntry {
  /* 0000 */ std::uint32_t unknown_0000[4];
  /* 0010 */ std::uint32_t unknown_0010[2];
};

class UIItemRepairWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;

public:
  /* 00B4 */ UIListBox* m_list;
  /* 00B8 */ std::vector<int> unknown_00B8;
  /* 00C4 */ std::vector<int> unknown_00C4;
  /* 00D0 */ std::list<UIItemRepairEntry> unknown_00D0;
};

#endif
