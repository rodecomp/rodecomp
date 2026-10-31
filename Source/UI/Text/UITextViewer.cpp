#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/ItemLinkDisplay.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIAchWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>

namespace {
  // INLINED
  unsigned Digit(unsigned char value) {
    if (value >= '0' && value <= '9') {
      return value - '0';
    }

    if (value >= 'a' && value <= 'f') {
      return value - 'a' + 10;
    }

    if (value >= 'A' && value <= 'F') {
      return value - 'A' + 10;
    }

    return 0;
  }

  // INLINED
  void ScanColors(const std::string& line, unsigned short codePage, std::uint32_t& color) {
    const char* current = line.c_str();
    const char* end = current + line.size();

    while (current < end) {
      bool marker = *current == '^' && end - current >= 6;

      for (int index = 1; marker && index <= 6; ++index) {
        const char value = current[index];
        marker = (value >= '0' && value <= '9') || (value >= 'a' && value <= 'f') || (value >= 'A' && value <= 'F');
      }

      if (marker) {
        color = (Digit(current[2]) + 16 * Digit(current[1])) | ((Digit(current[4]) + 16 * Digit(current[3])) << 8) |
                ((Digit(current[6]) + 16 * Digit(current[5])) << 16);
        current += 7;
      } else {
        current = CharNextExA(codePage, current, 0);
      }
    }
  }
}

// OFFSET: 0x0086F0A0
// FUNCTION: fn_11086
UITextViewer::UITextViewer(UIWindowContext& windows, UITextViewerContext& text)
    : UIListBox(windows, text.list), splitTipLinks(0), outline(0), bold(0), m_text(&text) {
  this->lineHeight = 18;
  constexpr char path[] = "유저인터페이스\\colorchip.bmp";
  const auto* chip = static_cast<CBitmapRes*>(text.list.resources.Load(path));
  const auto& image = chip->image;
  const std::uint32_t pixel = image.width > 18 && image.height > 2 ? image.pixels[18 + 2 * image.width] : 0x00FF0000u;
  this->backgroundColor = 0xFF000000u | ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
  this->Invalidate();

  this->unknown_00BC = 0;
}

// OFFSET: 0x008747F0
UITextViewer::~UITextViewer() = default;

// OFFSET: 0x008872A0
// FUNCTION: fn_11334
void UITextViewer::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x0087B730
// FUNCTION: fn_11248
void UITextViewEX::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
  this->lineHeight = 21;
}

// OFFSET: 0x0087A1E0
// FUNCTION: fn_11228
void UITextViewEX::Clear() {
  for (UIWindow* link : this->extraLinks) {
    this->RemoveChild(link, 1);
  }
  this->extraLinks.clear();
  this->lineLinks.clear();
  UITextViewer::Clear();
  this->Invalidate();
}

// OFFSET: 0x0087B780
// FUNCTION: fn_11249
void UITextViewer::Configure(int x, int y, int width, int height, unsigned char value) {
  UIListBox::Configure(x, y, width, height, value);
}

// OFFSET: 0x008760C0
// FUNCTION: fn_11204
void UITextViewer::AddLine(const char* text) {
  UIListBox::AddLine(text);
}

// OFFSET: 0x0087A290
// FUNCTION: fn_11229
void UITextViewer::Clear() {
  UIListBox::Clear();

  for (UITextButton* link : this->links) {
    this->RemoveChild(link, 1);
  }

  this->links.clear();
  this->Invalidate();
}

// OFFSET: 0x008909F0
// FUNCTION: fn_11429
void UITextViewer::Slot25(int, int) {
}

// OFFSET: 0x007B78C0
// FUNCTION: fn_09227
void UITextViewer::Slot26(int, int) {
}

// OFFSET: 0x007B78D0
// FUNCTION: fn_09228
void UIAchListWnd::Slot25(int x, int y) {
  if (x < 0 || x > this->geometry.width - this->unknown_0094) {
    return;
  }

  const int row = y / this->unknown_0084;
  if (static_cast<unsigned>(row) >= static_cast<unsigned>(this->unknown_00AC->count)) {
    return;
  }

  this->sub_7B37F0(this->unknown_008C + row);
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x007B7920
// FUNCTION: fn_09229
void UIAchTV::Slot25(int, int) {
  if (this->unknown_00EC == 1) {
    return;
  }

  const int id = this->geometry.id;
  if (id != -1 && this->parent) {
    this->parent->SendMsg(nullptr, 145, id, 0, 0, 0);
  }
}

// OFFSET: 0x007B7960
// FUNCTION: fn_09230
void UIAchTab::Slot25(int x, int y) {
  int a = 0;
  int b = 0;
  if (!this->sub_7B4370(x, y, &a, &b)) {
    return;
  }

  this->unknown_007C = a;
  this->unknown_0080 = b;
  if (this->parent) {
    this->parent->SendMsg(this, 22, a, b, 0, 0);
    this->parent->Invalidate();
  }

  this->sub_7B7F10();
  this->Invalidate();
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x007B79F0
// FUNCTION: fn_09231
void UIAchListWnd::Slot31(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x007B7A00
// FUNCTION: fn_09232
void UIAchTab::Slot31(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x007B7A10
// FUNCTION: fn_09233
void UITextViewer::Slot31(int, int) {
}

// OFFSET: 0x0088FBB0
// FUNCTION: fn_11394
void UITextViewer::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, this->transparent != 0 ? 0xFFFF00FFu : this->backgroundColor);
  }

  if (this->transparent != 0 && this->parent) {
    this->parent->Invalidate();
  }

  int divisor = (this->lineHeight - this->bottomMargin);

  if (divisor == 0) {
    divisor = 1;
  }

  const int limit = (this->geometry.height / divisor + this->topLine);
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  auto& owner = this->m_text->list;
  std::uint32_t color = 0;

  for (int index = 0; index < this->topLine; ++index) {
    ScanColors(this->lines[index], owner.codePage, color);
  }

  for (int index = this->topLine; index < end; ++index) {
    const auto& line = this->lines[index];
    const int y = (index - this->topLine) * this->lineHeight + 4;
    const auto bytes = static_cast<unsigned>(line.size());

    if ((this->unknown_00BC & 255) != 0) {
      DrawOutlinedText(*this, 3, y, line.c_str(), bytes, 0, 0xFFFFFF, 0, 12, 0, 0);
    } else if (this->outline != 0) {
      DrawOutlinedColoredText(*this, owner.codePage, 3, y, line.c_str(), bytes, color, this->backgroundColor, 0, 12, this->bold);
    } else {
      DrawColoredText(*this, owner.codePage, 3, y, line.c_str(), bytes, color, 0, 12, false);
    }
  }
}

// OFFSET: 0x00887290
// FUNCTION: fn_11333
void UITextViewEX::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x00896100
// FUNCTION: fn_11515
void UITextViewEX::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int overflow = std::max((static_cast<int>(this->lineLinks.size()) - visible), 0);

  this->verticalOverflow = overflow != 0;
  UIScrollBar* bar = this->verticalBar;
  if (overflow != 0) {
    bar->maximum = this->bottomMargin + overflow;
    bar->total = static_cast<int>(this->lineLinks.size());
  } else {
    bar->maximum = 0;
    bar->total = static_cast<int>(this->lineLinks.size());
  }
  bar->visible = visible;
  bar->Invalidate();

  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x00874DF0
// FUNCTION: fn_11194
void UIItemTagCtrl::AddTagButton(UIWindow* button) {
  UIWindow* window = g_windowMgr.GetHoverWindow();
  if (window) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00874E10
// FUNCTION: fn_11195
void UIItemTagOnMail::AddTagButton(UIWindow* button) {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0458)) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00874E30
// FUNCTION: fn_11196
void UIItemTagOnMegaPhone::AddTagButton(UIWindow* button) {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0318)) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00899BF0
// FUNCTION: fn_11561
int UITextViewEX::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 7:
  case 9:
  case 10: {
    const int previous = this->topLine;
    const int visible = this->geometry.height / this->lineHeight;
    const int delta = message == 9 ? (1 - visible) : message == 10 ? (visible - 1) : static_cast<int>(argument1);
    this->topLine = previous + delta;

    if (this->topLine < 0) {
      this->topLine = 0;
    }

    const int maximum = this->bottomMargin + static_cast<int>(this->lineLinks.size()) - visible;
    if (static_cast<unsigned>(this->topLine) > static_cast<unsigned>(maximum)) {
      this->topLine = maximum;
    }

    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }

    return this->topLine - previous;
  }
  case 18:
  case 19:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, 7, message == 18 ? -1 : 1, 0, 0, 0);
    }
    return 0;
  case 29: {
    std::string text(reinterpret_cast<const char*>(argument1));
    for (UIWindow* link : this->extraLinks) {
      this->RemoveChild(link, 1);
    }
    this->extraLinks.clear();
    this->lineLinks.clear();
    this->SetText(text);
    this->Invalidate();
    return 0;
  }
  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
