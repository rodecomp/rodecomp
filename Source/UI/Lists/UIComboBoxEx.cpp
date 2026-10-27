#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarNavi.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

namespace {
  unsigned Palette(ResourceMgr& resources, int column, int row) {
    constexpr char path[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\colorchip.bmp";
    auto* chip = static_cast<CBitmapRes*>(resources.Load(path));
    const int x = column * 4 + 2;
    const int y = row * 4 + 2;
    const auto& image = chip->image;
    const unsigned pixel = x >= 0 && y >= 0 && x < image.width && y < image.height ? image.pixels[x + y * image.width] : 0x00FF0000u;

    return ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  }
}

// OFFSET: 0x0086CD40
// FUNCTION: fn_11037
UIComboBoxEx::UIComboBoxEx(UIWindowContext& windows, UIComboBoxContext& combo) : UIListBoxEx(windows, combo.list), targetId(10041), m_combo(&combo) {
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
  auto& windows = this->m_listContext->windows;

  if (UIWindow* target = windows.LookupWindow(this->targetId)) {
    target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
  } else if (this->parent) {
    static_cast<UIWindow*>(this->parent)->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
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
    bar->position = this->topLine;
  }

  bar->Invalidate();
  UIWindow* root = this;

  while (root->parent) {
    root = static_cast<UIWindow*>(root->parent);
  }

  root->Invalidate();
}

// OFFSET: 0x00898050
// FUNCTION: fn_11545
int UIComboBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);
  const unsigned count = static_cast<unsigned>(this->lines.size());

  switch (message) {
  case 0:
    if (UIWindow* target = this->m_listContext->windows.LookupWindow(this->targetId)) {
      target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
    }

    if (this->m_combo->popup) {
      this->m_listContext->windows.CloseWindow(26);
    }

    return 0;
  case 18:
    if (count == 0) {
      return 0;
    }

    this->selected = std::max((this->selected - 1), 0);

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    }

    break;
  case 19:
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
  case 31:
    this->AddLine(reinterpret_cast<const char*>(a), static_cast<unsigned>(b));
    this->Invalidate();
    return 0;
  case 40:
    this->selected = std::max(value, 0);

    if (static_cast<unsigned>(this->selected) > count - 1) {
      this->selected = static_cast<int>(count - 1);
    }

    break;
  case 83:
    this->targetId = value;
    return 0;
  default:
    return this->UIListBoxEx::SendMsg(sender, message, a, b, c, d);
  }

  this->Refresh();
  this->Invalidate();
  return 0;
}

// INLINED
void UIComboBoxEx::DrawRows(bool centered) {
  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  );
  auto& owner = *this->m_listContext;

  for (int index = this->topLine; index < static_cast<int>(end); index = (index + 1)) {
    auto& row = this->lines[index];
    int x = (3 - this->horizontalOffset);

    if (centered) {
      const int width = this->TextWidth(owner.fonts, owner.locale, row.text.c_str(), 0, 0, 12, 0, 0);
      x = static_cast<int>(static_cast<float>(this->geometry.width) * 0.5f - static_cast<float>(width) * 0.5f);
    }

    this->DrawTextAt(owner.fonts, owner.locale, x, ((this->lineHeight * (index - this->topLine)) + 2), row.text.c_str(), 0, 0, 12, row.color, 0, 0);
  }
}

// OFFSET: 0x0088A360
// FUNCTION: fn_11354
void UIComboBoxEx::Paint() {
  auto& resources = this->m_listContext->resources;
  this->geometry.surface->Fill(nullptr, Palette(resources, 6, 6));
  this->FillRegion(1, 1, (this->geometry.width - 2), (this->geometry.height - 2), Palette(resources, 2, 2));
  constexpr char prefix[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA\\basic_interface\\";
  const char* names[] = {"sysboxs_lu.bmp", "sysboxs_ld.bmp", "sysboxs_ru.bmp", "sysboxs_rd.bmp"};

  for (int corner = 0; corner < 4; ++corner) {
    auto* resource = static_cast<CBitmapRes*>(resources.Load((std::string(prefix) + names[corner]).c_str()));
    this->DrawBitmap(
      corner >= 2 ? (this->geometry.width - resource->image.width) : 0, (corner & 1) ? (this->geometry.height - resource->image.height) : 0, &resource->image,
      false
    );
  }

  const unsigned end = std::min(
    static_cast<unsigned>(this->lines.size()), static_cast<unsigned>((this->topLine + (this->geometry.height / this->lineHeight - this->bottomMargin)))
  );

  if (this->selected >= this->topLine && this->selected < static_cast<int>(end)) {
    this->FillRegion(
      3, ((this->lineHeight * (this->selected - this->topLine)) + 1), (this->geometry.width - 6), (this->lineHeight - 2), Palette(resources, 6, 2)
    );
  }

  this->DrawRows(false);

  if (this->verticalOverflow != 0) {
    this->FillRegion((this->geometry.width - 16), 0, 16, this->geometry.height, Palette(resources, 2, 2));
  }

  static_cast<UIWindow*>(this->parent)->Invalidate();
}

// OFFSET: 0x0086CCE0
// FUNCTION: fn_11036
UIComboBoxEx2::UIComboBoxEx2(UIWindowContext& windows, UIComboBoxContext& combo) : UIComboBoxEx(windows, combo), alignment(0) {
}

// OFFSET: 0x008706D0
// FUNCTION: fn_11102
UIComboBoxEx2::~UIComboBoxEx2() = default;

// OFFSET: 0x00885EE0
// FUNCTION: fn_11320
void UIComboBoxEx2::Slot15(int width, int height) {
  auto& list = *this->m_listContext;
  auto* vertical = new UIScrollBar_Navi(*this->m_context, list.capture, list.resources);
  this->verticalBar = vertical;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(vertical->thickness, height);
  vertical->Move((width - 7), 0);
  vertical->Slot15(vertical->thickness, height);
  vertical->Slot22(vertical->thickness, height);
  this->AddChild(vertical);

  auto* horizontal = new UIScrollBar(*this->m_context, list.capture, list.resources);
  this->horizontalBar = horizontal;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 7), horizontal->thickness);
  horizontal->Move(0, (height - 7));
  horizontal->Slot15((width - 7), horizontal->thickness);
  horizontal->Slot22((width - 7), horizontal->thickness);
  this->AddChild(horizontal);
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
  this->geometry.surface->Fill(nullptr, 0xC59582u);
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
    static_cast<UIWindow*>(this->parent)->Invalidate();
  }
}
