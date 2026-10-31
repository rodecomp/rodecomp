#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H

#include "UI/Lists/UIListBox.h"

#include <vector>

class UIListBox_NaviSearch : public UIListBox {
public:
  UIListBox_NaviSearch(UIWindowContext& windows, UIListBoxContext& list);

  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Clear() override;
  void Refresh() override;

  /* 00D4 */ std::vector<int> icons;
};

#endif
