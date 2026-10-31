#include "UI/Window/UITransMenuWnd.h"
#include "UI/Buttons/UIButton.h"
#include <cstring>

// OFFSET: 0x0094ED40
// FUNCTION: fn_12916
void UITransMenuWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_items.begin() == this->m_items.end()) {
    return;
  }

  const int cellHeight = (this->geometry.height - 2 * this->m_margin) / this->m_items.size();
  for (unsigned i = 0; i < this->m_items.size(); ++i) {
    std::uint32_t color = this->m_items[i].enabled ? 0xFFFFFF : 0x646464;
    if (i == this->m_selected) {
      color = 0x282828;
      const int half = this->m_margin / 2;
      this->FillRegion(0, (i - 1) * half + cellHeight * i + this->m_margin, this->geometry.width, cellHeight + half, 0xC8C8C8);
    }

    auto& controls = *this->m_controls;
    const int margin = this->m_margin;
    this->DrawTextAt(controls.fonts, controls.locale, margin, (margin / 2 + cellHeight) * i + margin, this->m_items[i].text, 0, 0, this->m_lineHeight, color, 0, 0);
  }
}

// OFFSET: 0x0094EEC0
// FUNCTION: fn_12921
void UITransMenuWnd::Slot25(int x, int y) {
  if (this->m_selected == -1) {
    return;
  }

  const TransMenuItem& item = this->m_items[this->m_selected];
  if (item.enabled) {
    this->SendMsg(nullptr, 48, item.command, 0, 0, 0);
  }
}

// OFFSET: 0x0094F050
// FUNCTION: fn_12930
void UITransMenuWnd::Slot27(int x, int y) {
  const int margin = this->m_margin;
  const int previous = this->m_selected;
  int selected = -1;
  if (margin <= y) {
    const unsigned count = this->m_items.size();
    if ((this->m_lineHeight + margin / 2) * count + margin >= static_cast<unsigned>(y)) {
      const int cellHeight = (this->geometry.height - 2 * margin) / count;
      selected = (y - margin) / cellHeight;
    }
  }

  this->m_selected = selected;
  if (previous != selected && selected != -1) {
    this->Invalidate();
  }
}

// OFFSET: 0x0094F380
// FUNCTION: fn_12937
void UITransMenuWnd::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->m_width = this->geometry.width;
  this->m_height = this->geometry.height;
}

// OFFSET: 0x0094CF00
// FUNCTION: fn_12900
void UITransMenuWnd::Slot15(int width, int height) {
  this->unknown_00C0 = this->geometry.x;
  this->unknown_00C4 = this->geometry.y;
  this->m_width = this->geometry.width;
  this->unknown_00D0 = 0;
  this->m_height = this->geometry.height;
  this->unknown_00D4 = 0x66000000;
  this->unknown_00D8 = 0;
}

// OFFSET: 0x00949B70
// FUNCTION: fn_12884
bool UITransMenuWnd::Slot07(void* output) {
  std::memcpy(output, &this->unknown_00C0, 28);
  return true;
}

// OFFSET: 0x0094A420
// FUNCTION: fn_12889
void UITransMenuWnd::Move(int x, int y) {
  this->UIWindow::Move(x, y);
  this->unknown_00C0 = this->geometry.x;
  this->unknown_00C4 = this->geometry.y;
}

// OFFSET: 0x00951440
// FUNCTION: fn_12950
int UITransMenuWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 48) {
    this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x009471E0
// FUNCTION: fn_12859
UITransMenuWnd::UITransMenuWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls) : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
  this->m_margin = 5;
  this->m_lineHeight = 12;
  this->m_selected = -1;
  this->m_items.reserve(8);
}
