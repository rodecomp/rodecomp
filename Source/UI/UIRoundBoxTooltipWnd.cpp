#include "UI/UIRoundBoxTooltipWnd.h"

#include "UI/Window/WindowBorder.h"

// OFFSET: 0x0099A3F0
// FUNCTION: fn_13542
void UIRoundBoxTooltipWnd::Paint() {
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xEFB8B8);
  DrawFilledRoundedBorder(*this, 1, 1, this->geometry.width - 2, this->geometry.height - 2, 0xEFE7E7);
}
