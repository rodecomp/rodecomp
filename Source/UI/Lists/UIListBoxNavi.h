#ifndef RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVI_H
#define RAGNAROK_SOURCE_UI_LISTS_UILISTBOXNAVI_H

#include "UI/Lists/UIListBox.h"

#include <cstdint>
#include <vector>

class UIListBoxNavi : public UIListBox {
public:
  UIListBoxNavi(UIWindowContext& windows, UIListBoxContext& list);
  void Paint() override;
  void Slot25(int x, int y) override;
  void Clear() override;
  /* 00D4 */ std::uint32_t defaultLineColor;
  /* 00D8 */ std::vector<std::uint32_t> lineColors;
  /* 00E4 */ int lineOffset;

  void SetDefaultLineColor(std::uint32_t color);
  void SetLineColor(int line, std::uint32_t color);
  void ResetLineColors();
  void SetLineOffset(int offset);
  int LineOffset();
};

#endif
