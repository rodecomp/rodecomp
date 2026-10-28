#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVISEARCH_H

#include "UI/Lists/UIListBox.h"

class UIListBox_NaviSearch : public UIListBox {
public:
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int value) override;
  void Clear() override;

  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
};

#endif
