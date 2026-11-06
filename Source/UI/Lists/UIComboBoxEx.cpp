#include "UI/Window/UIWindowMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarNavi.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

namespace {
  // INLINED
  unsigned Palette(int column, int row) {
    auto& resources = ResourceMgr::Instance();
    unsigned red;
    unsigned green;
    unsigned blue;
    SkinPaletteChannels(resources, column, row, red, green, blue);

    return RGB(red, green, blue);
  }
}

// OFFSET: 0x0086CD40
// FUNCTION: fn_11037
UIComboBoxEx::UIComboBoxEx() : UIListBoxEx(), targetId(10041) {
}

// OFFSET: 0x00872C00
// FUNCTION: fn_11136
UIComboBoxEx::~UIComboBoxEx() = default;

// OFFSET: 0x0087B5A0
// FUNCTION: fn_11243
void UIComboBoxEx::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  this->UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00886060
// FUNCTION: fn_11321
void UIComboBoxEx::Slot15(int width, int height) {
  this->UIListBoxEx::Slot15(width, height);
}

// OFFSET: 0x00897580
// FUNCTION: fn_11530
void UIComboBoxEx::Resize(int width, int height) {
  this->UIListBoxEx::Resize(width, height);
  this->topLine = 0;
  this->verticalBar->position = 0;
  this->verticalBar->Move((width - 13), 0);

  if (this->verticalOverflow != 0) {
    this->verticalBar->Invalidate();
  }
}

// OFFSET: 0x00890460
// FUNCTION: fn_11414
void UIComboBoxEx::Slot25(int, int) {
}

// OFFSET: 0x00890160
// FUNCTION: fn_11401
void UIComboBoxEx::Slot26(int, int) {
}

// OFFSET: 0x008920A0
// FUNCTION: fn_11464
void UIComboBoxEx::Slot27(int x, int y) {
  const int previous = this->selected;
  this->selected = this->LineAt(x, y);

  if (previous != this->selected) {
    this->Invalidate();
  }
}

// OFFSET: 0x00890B70
// FUNCTION: fn_11435
void UIComboBoxEx::Slot31(int x, int y) {
  this->selected = this->LineAt(x, y);
  auto& windows = g_windowMgr;

  if (UIWindow* target = windows.LookupWindow(this->targetId)) {
    target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
  } else if (this->parent) {
    this->parent->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
  }

  windows.CloseWindow(26);
}

// OFFSET: 0x00895C30
// FUNCTION: fn_11510
void UIComboBoxEx::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = std::max((count - visible), 0);
  this->verticalOverflow = overflow != 0;
  auto* bar = this->verticalBar;
  bar->visible = visible;
  bar->total = count;
  bar->maximum = overflow != 0 ? (overflow + this->bottomMargin) : 0;
  bar->Invalidate();

  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
  }

  if (overflow != 0) {
    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    } else if ((this->topLine + visible) <= this->selected) {
      this->topLine = ((this->selected - visible) + 1);
    }

    this->topLine = std::clamp(this->topLine, 0, overflow);
    this->verticalBar->position = this->topLine;
  }

  this->verticalBar->Invalidate();

  if (this->Root()) {
    this->Root()->Invalidate();
  }
}

// OFFSET: 0x00898050
// FUNCTION: fn_11545
int UIComboBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 0:
    if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
      target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
    }

    if (g_windowMgr.unknown_0214) {
      g_windowMgr.CloseWindow(26);
    }

    return 0;

  case 18:
    if (this->lines.size() == 0) {
      return 0;
    }

    --this->selected;

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;

  case 19: {
    const unsigned count = static_cast<unsigned>(this->lines.size());

    if (count == 0) {
      return 0;
    }

    this->selected = (this->selected + 1);

    if (static_cast<unsigned>(this->selected) >= count) {
      this->selected = static_cast<int>(count - 1);
    }

    {
      const int edge = ((this->bottomMargin - this->geometry.height / this->lineHeight) + this->selected);

      if (this->topLine <= edge) {
        this->topLine = (edge + 1);
      }
    }

    break;
  }

  case 31:
    this->AddLine(reinterpret_cast<const char*>(a), static_cast<unsigned>(b));
    this->Invalidate();
    return 0;

  case 83:
    this->targetId = static_cast<int>(a);
    return 0;

  case 40:
    this->selected = static_cast<int>(a);

    if (this->selected < 0) {
      this->selected = 0;
    }

    if (static_cast<unsigned>(this->selected) > this->lines.size() - 1) {
      this->selected = static_cast<int>(this->lines.size() - 1);
    }

    break;

  default:
    return this->UIListBoxEx::SendMsg(sender, message, a, b, c, d);
  }

  this->Refresh();
  this->Invalidate();
  return 0;
}

// INLINED
__forceinline void UIComboBoxEx::DrawRows(bool centered) {
  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  );

  for (int index = this->topLine; index < static_cast<int>(end); index = (index + 1)) {
    auto& row = this->lines[index];
    int x = (3 - this->horizontalOffset);

    if (centered) {
      const int width = this->TextWidth(row.text.c_str(), 0, 0, 12, 0, 0);
      x = static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(width) * 0.5f);
    }

    this->DrawTextAt(x, ((this->lineHeight * (index - this->topLine)) + 2), row.text.c_str(), 0, 0, 12, row.color, 0, 0);
  }
}

// OFFSET: 0x0088A360
// FUNCTION: fn_11354
void UIComboBoxEx::Paint() {
  this->ClearSurface(Palette(6, 6));
  this->FillRegion(1, 1, (this->geometry.width - 2), (this->geometry.height - 2), Palette(2, 2));

  const auto* leftUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")));
  this->DrawBitmap(0, 0, leftUp ? &leftUp->image : nullptr, false);
  const auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, (this->geometry.height - leftDown->image.height), leftDown ? &leftDown->image : nullptr, false);
  const auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap((this->geometry.width - rightUp->image.width), 0, rightUp ? &rightUp->image : nullptr, false);
  const auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(
    (this->geometry.width - rightDown->image.width), (this->geometry.height - rightDown->image.height), rightDown ? &rightDown->image : nullptr, false
  );

  const int top = this->topLine;
  const int end = static_cast<int>(std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((top + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  ));

  for (int row = top; row < end; ++row) {
    if (row == this->selected) {
      this->FillRegion(3, ((this->lineHeight * (row - top)) + 1), (this->geometry.width - 6), (this->lineHeight - 2), Palette(6, 2));
    }
    this->DrawTextAt(3 - this->horizontalOffset, this->lineHeight * (row - top) + 2, this->lines[row].text.c_str(), 0, 0, 12, this->lines[row].color, 0, 0);
  }

  if (this->verticalOverflow != 0) {
    this->FillRegion((this->geometry.width - 16), 0, 16, this->geometry.height, Palette(2, 2));
  }

  this->parent->Invalidate();
}

// OFFSET: 0x0086CCE0
// FUNCTION: fn_11036
UIComboBoxEx2::UIComboBoxEx2() : UIComboBoxEx(), alignment(0) {
}

// OFFSET: 0x008706D0
// FUNCTION: fn_11102
UIComboBoxEx2::~UIComboBoxEx2() = default;

// OFFSET: 0x00885EE0
// FUNCTION: fn_11320
void UIComboBoxEx2::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar_Navi();
  this->verticalBar = vertical;
  const int thickness = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(thickness, height);
  vertical->Move((width - 7), 0);
  vertical->Slot15(thickness, height);
  vertical->Slot22(thickness, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 7), horizontalHeight);
  horizontal->Move(0, (height - 7));
  horizontal->Slot15((width - 7), horizontalHeight);
  horizontal->Slot22((width - 7), horizontalHeight);
  this->AddChild(this->horizontalBar);
  this->lineHeight = 18;
}

// OFFSET: 0x00897510
// FUNCTION: fn_11529
void UIComboBoxEx2::Resize(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->Refresh();
  this->verticalBar->Move((width - 7), 0);

  if (this->verticalOverflow != 0) {
    this->verticalBar->Invalidate();
  } else {
    this->verticalBar->Resize(7, height);
    this->verticalBar->Move((width - 6), 0);
  }
}

// OFFSET: 0x00889E50
// FUNCTION: fn_11353
void UIComboBoxEx2::Paint() {
  this->ClearSurface(0xC59582u);
  this->FillRegion(1, 1, (this->geometry.width - 2), (this->geometry.height - 2), 0xFCF4F0u);
  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  );

  if (this->selected >= this->topLine && this->selected < static_cast<int>(end)) {
    this->FillRegion(3, ((this->lineHeight * (this->selected - this->topLine)) + 1), (this->geometry.width - 6), (this->lineHeight - 2), 0xF8D5C4u);
  }

  if (this->alignment == 0 || this->alignment == 1) {
    this->DrawRows(this->alignment == 1);
  }

  if (this->parent) {
    this->parent->Invalidate();
  }
}
