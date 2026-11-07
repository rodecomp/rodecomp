#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H

#include "UI/Lists/UIListBox.h"

#include <vector>

class UIListBox_NaviSearch : public UIListBox {
public:
  UIListBox_NaviSearch();

  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void Clear() override;
  void Refresh() override;

  /* 00D4 */ std::vector<int> icons;
};

#endif
