#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPADDING_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPADDING_H

#include "UI/Window/UIControl.h"

#include <cstdint>

class CUIPadding : public CUIControl {
public:
  bool CanHitTest() override;
  void Paint() override;

  /* 0080 */ std::uint32_t color;
  /* 0084 */ int left;
  /* 0088 */ int top;
  /* 008C */ int right;
  /* 0090 */ int bottom;
};

#endif
