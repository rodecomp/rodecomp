#include "Client/Mode/ModeCursor.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIImgMenu.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include <algorithm>
#include <cstdio>

// OFFSET: 0x0086E550
// FUNCTION: fn_11073
UIScrollBar::UIScrollBar(UIWindowContext& windows)
    : UIWindow(windows, 0), state(0), maximum(0), position(0), vertical(1), pressX(0), pressY(0), dragAmount(0), total(0), visible(0), arrowLength(13),
      thickness(13), minimumThumb(1), horizontalStep(1), verticalStep(1) {
}

// OFFSET: 0x00873FA0
UIScrollBar::~UIScrollBar() = default;

// OFFSET: 0x00897830
// FUNCTION: fn_11535
void UIScrollBar::Resize(int width, int height) {
  this->Invalidate();

  if (this->vertical == 0) {
    height = this->thickness;
    UIWindow::Resize(width, height);
    this->Slot22(width, height);
  } else if (this->vertical == 1) {
    width = this->thickness;
  }

  UIWindow::Resize(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00882430
// FUNCTION: fn_11303
bool UIScrollBar::Slot08() {
  return this->maximum == 0 && this->parent && this->parent->Slot08();
}

// OFFSET: 0x00880C60
// FUNCTION: fn_11288
int UIScrollBar::ThumbLength() {
  int length = ((this->vertical == 0 ? this->geometry.width : this->geometry.height) - (2 * this->arrowLength));

  if (this->total != 0) {
    length = (length * this->visible) / this->total;
  }

  if (this->minimumThumb != 0 && length < 8) {
    length = 8;
  }

  return std::max(length, 2);
}

// OFFSET: 0x00880AA0
// FUNCTION: fn_11281
int UIScrollBar::Position() {
  return this->position;
}

// OFFSET: 0x00880AE0
// FUNCTION: fn_11282
int UIScrollBar::Travel() {
  const int length = this->vertical == 0 ? this->geometry.width : this->geometry.height;
  const int thumb = this->ThumbLength();
  return ((length - (2 * this->arrowLength)) - thumb);
}

// OFFSET: 0x00880C00
// FUNCTION: fn_11286
int UIScrollBar::ThumbOffset() {
  return (this->Travel() * this->position) / this->maximum;
}

// OFFSET: 0x008806E0
// FUNCTION: fn_11278
int UIScrollBar::Part(int x, int y) {
  if (this->maximum == 0 || x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height || (this->vertical != 0 && this->vertical != 1)) {
    return -1;
  }

  const int coordinate = this->vertical == 0 ? x : y;
  const int length = this->vertical == 0 ? this->geometry.width : this->geometry.height;

  if (coordinate < this->arrowLength) {
    return 0;
  }

  if (coordinate < (this->arrowLength + this->ThumbOffset())) {
    return 1;
  }

  const int thumb = this->ThumbLength();
  const int offset = this->ThumbOffset();

  if (coordinate < ((this->arrowLength + thumb) + offset)) {
    return 2;
  }

  return coordinate >= (length - this->arrowLength) ? 4 : 3;
}

// OFFSET: 0x00890720
// FUNCTION: fn_11425
void UIScrollBar::Slot25(int x, int y) {
  UIWindow* owner = this->parent;

  if (this->maximum == 0) {
    owner->Slot25((x + this->geometry.x), (y + this->geometry.y));
    return;
  }

  const int previous = this->state;
  const int part = this->Part(x, y);
  this->pressY = y;
  this->pressX = x;
  this->dragAmount = 0;

  if (this->vertical == 0 || this->vertical == 1) {
    const bool vertical = this->vertical == 1;

    switch (part) {
    case 0:
      owner->SendMsg(this, vertical ? 7 : 8, (0 - (vertical ? this->verticalStep : this->horizontalStep)), 0, 0, 0);
      break;
    case 1:
      owner->SendMsg(this, vertical ? 9 : 11, 0, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SetCapture(this);
      this->state = 6;
      break;
    case 3:
      owner->SendMsg(this, vertical ? 10 : 12, 0, 0, 0, 0);
      break;
    case 4:
      owner->SendMsg(this, vertical ? 7 : 8, vertical ? this->verticalStep : this->horizontalStep, 0, 0, 0);
      break;
    default:
      break;
    }
  }

  if (previous != this->state) {
    this->Invalidate();
  }
}

// OFFSET: 0x00890260
// FUNCTION: fn_11407
void UIScrollBar::Slot26(int x, int y) {
  this->Slot25(x, y);

  if (this->maximum == 0) {
    this->parent->Slot26((x + this->geometry.x), (y + this->geometry.y));
  }
}

// OFFSET: 0x008921D0
// FUNCTION: fn_11468
void UIScrollBar::Slot27(int x, int y) {
  UIWindow* owner = this->parent;

  if (this->maximum == 0) {
    owner->Slot27((x + this->geometry.x), (y + this->geometry.y));
    return;
  }

  if (this->Travel() == 0) {
    return;
  }

  const int previous = this->state;

  switch (this->Part(x, y)) {
  case -1:
  case 1:
  case 3:
    this->state = 0;
    break;
  case 0:
    this->state = 1;
    break;
  case 2:
    this->state = 3;
    break;
  case 4:
    this->state = 2;
    break;
  default:
    break;
  }

  if (previous != this->state) {
    this->Invalidate();
  }

  if (g_windowMgr.Capture() == this && owner && (this->vertical == 0 || this->vertical == 1)) {
    const int distance = this->vertical == 0 ? (x - this->pressX) : (y - this->pressY);
    const int delta = ((distance * (this->maximum + 1)) / this->Travel() - this->dragAmount);

    if (delta != 0) {
      const int result = owner->SendMsg(this, this->vertical == 0 ? 8 : 7, delta, 0, 0, 0);

      this->dragAmount = (this->dragAmount + result);
    }
  }
}

// OFFSET: 0x00892570
// FUNCTION: fn_11474
void UIScrollBar::ResetCursor(int, int) {
  this->m_context->cursor.Set(this->maximum != 0 ? 2 : 0, this->m_context->cursorTick);
}

// OFFSET: 0x00890E90
// FUNCTION: fn_11440
void UIScrollBar::Slot31(int x, int y) {
  if (this->maximum != 0) {
    if (g_windowMgr.Capture() == this) {
      g_windowMgr.ReleaseCapture();
    }
  } else {
    this->parent->Slot31((x + this->geometry.x), (y + this->geometry.y));
  }
}

// OFFSET: 0x00892C70
// FUNCTION: fn_11491
void UIScrollBar::Slot33(int x, int y) {
  if (this->maximum == 0) {
    this->parent->Slot33((x + this->geometry.x), (y + this->geometry.y));
  }
}

// OFFSET: 0x00892F20
// FUNCTION: fn_11499
void UIScrollBar::Slot35(int value) {
  this->parent->Slot35(value);
}

// OFFSET: 0x0088E310
// FUNCTION: fn_11381
void UIScrollBar::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  }

  if (this->maximum == 0 || (this->vertical != 0 && this->vertical != 1)) {
    return;
  }

  constexpr char prefix[] = "유저인터페이스\\";
  const bool vertical = this->vertical == 1;
  const int length = vertical ? this->geometry.height : this->geometry.width;
  const auto draw = [&](const char* name, int coordinate, bool transparent) {
    const std::string path = std::string(prefix) + name;
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(path.c_str()));
    this->DrawBitmap(vertical ? 0 : coordinate, vertical ? coordinate : 0, bitmap ? &bitmap->image : nullptr, transparent);
  };
  draw(vertical ? "scroll0up.bmp" : "scroll1left.bmp", 1, false);
  const char* middle = vertical ? "scroll0mid.bmp" : "scroll1mid.bmp";
  const int interior = ((length - this->arrowLength) - 14);
  const int count = interior / 13 + (interior % 13 != 0);

  for (int index = 0; index < count; ++index) {
    draw(middle, ((index * 13) + this->arrowLength), false);
  }

  draw(middle, ((length - this->arrowLength) - 13), false);
  this->FillRegion(
    vertical ? 0 : (length - this->arrowLength), vertical ? (length - this->arrowLength) : 0, vertical ? this->geometry.width : 13,
    vertical ? 13 : this->geometry.height, 0xFFFF00FFu
  );
  draw(vertical ? "scroll0down.bmp" : "scroll1right.bmp", ((length - this->arrowLength) - 1), true);
  const int thumb = (this->arrowLength + this->ThumbOffset());
  draw(vertical ? "scroll0bar_up.bmp" : "scroll1bar_left.bmp", thumb, true);
  const int end = ((thumb + this->ThumbLength()) - 4);

  for (int offset = (thumb + 4); offset < end; offset = (offset + 4)) {
    draw(vertical ? "scroll0bar_mid.bmp" : "scroll1bar_mid.bmp", offset, true);
  }

  draw(vertical ? "scroll0bar_down.bmp" : "scroll1bar_right.bmp", end, true);
}

// OFFSET: 0x0087BAA0
// FUNCTION: fn_11250
void UIScrollBar::Configure(int x, int y, unsigned char vertical, int length) {
  this->vertical = vertical;

  int width;
  int height;
  switch (vertical) {
  case 0:
    width = length;
    height = this->thickness;
    break;
  case 1:
    width = this->thickness;
    height = length;
    break;
  default:
    width = length;
    height = length;
    break;
  }

  this->UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00880800
// FUNCTION: fn_11279
int UIListBoxEx::FindLine(const std::string& text) {
  const std::string key = text;
  auto it = this->lines.begin();
  for (; it != this->lines.end(); ++it) {
    if (key == it->second.text) {
      break;
    }
  }
  return it == this->lines.end() ? -1 : it->first;
}

// OFFSET: 0x00880920
// FUNCTION: fn_11280
int UIImgMenu::ItemAt(int x, int y) {
  const int width = this->geometry.width - 3;
  if (x < 1 || x >= width || y < 1) {
    return -1;
  }
  const int height = this->geometry.height - 3;
  if (y >= height) {
    return -1;
  }

  int size;
  int offset;
  if (this->horizontal != 0) {
    size = width / this->count;
    this->itemWidth = size;
    offset = x;
  } else {
    size = height / this->count;
    this->itemHeight = size;
    offset = y;
  }
  return std::min(offset / size, this->count - 1);
}

// OFFSET: 0x0086ECF0
// FUNCTION: fn_11083
UITabControl::UITabControl(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), unknown_00E8(12), unknown_00EC(0), unknown_00F0(0), unknown_00F4(0), unknown_00F8(0), m_controls(&controls) {
  this->unknown_007C = -1;
  this->dragX = 0;
  this->dragY = 0;
}

// OFFSET: 0x00880B20
// FUNCTION: fn_11283
int UITabControl::TabCount() {
  return static_cast<int>(this->labels.size());
}

// OFFSET: 0x00880B40
// FUNCTION: fn_11284
int UITabControl::TabAt(int x, int y) {
  int offset = x;
  if (this->layout == 0 || this->layout == 2) {
    offset = y;
  }
  const int count = this->TabCount();
  for (int index = 0; index < count; ++index) {
    offset -= this->widths[index];
    if (offset < 0) {
      return index;
    }
  }
  return -1;
}

// OFFSET: 0x00880BB0
// FUNCTION: fn_11285
const char* UIChatHisBox::HistoryText(unsigned int index) {
  if (index < this->history.size() && !this->history[index].empty()) {
    return this->history[index].c_str();
  }
  return nullptr;
}

// OFFSET: 0x00879F10
// FUNCTION: fn_11222
void UIChatHisBox::Clear() {
  UIListBox::Clear();
  this->unknown_00E4.erase(this->unknown_00E4.begin(), this->unknown_00E4.end());
  this->history.erase(this->history.begin(), this->history.end());
}

// OFFSET: 0x008900D0
// FUNCTION: fn_11399
void UIChatHisBox::Slot26(int, int) {
  const unsigned int index = static_cast<unsigned int>(this->selected);

  if (index == static_cast<unsigned int>(-1) || index >= this->history.size() || this->history[index].size() == 0 || this->history[index].c_str() == nullptr) {
    return;
  }

  this->parent->SendMsg(this, 6, 347, reinterpret_cast<std::intptr_t>(this->HistoryText(index)), 0, 0);
}

// OFFSET: 0x00890430
// FUNCTION: fn_11412
void UIChatHisBox::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
}

// OFFSET: 0x00890AB0
// FUNCTION: fn_11432
void UIChatHisBox::Slot31(int, int) {
}

// OFFSET: 0x008973E0
// FUNCTION: fn_11527
void UIChatHisBox::Resize(int width, int height) {
  UIListBox::Resize(width, height);
  if (this->verticalOverflow) {
    const int visibleLines = this->geometry.height / this->lineHeight;
    this->topLine = static_cast<int>(this->lines.size()) - visibleLines + this->bottomMargin;
    this->verticalBar->position = this->topLine;
    this->verticalBar->Move(width - 13, 0);
    this->verticalBar->Invalidate();
  } else {
    this->topLine = 0;
    this->verticalBar->position = 0;
    this->verticalBar->Move(width - 13, 0);
  }
}

// OFFSET: 0x00897C10
// FUNCTION: fn_11542
int UIChatHisBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0089C340
// FUNCTION: fn_11585
void UIChatHisBox::Show(int visible) {
  this->geometry.visible = visible;
  if (visible) {
    this->Invalidate();
  }
}

// OFFSET: 0x0089EAD0
// FUNCTION: fn_11604
bool UIChatHisBox::SaveChatLog(const char* path) {
  std::FILE* file = std::fopen(path, "w");
  if (!file) {
    return false;
  }

  const char newline = '\n';
  const int count = static_cast<int>(this->lines.size());

  for (int index = 0; index < count; ++index) {
    const std::string& line = this->lines[index];
    std::fwrite(line.data(), line.size(), 1, file);
    std::fwrite(&newline, 1, 1, file);
  }

  std::fclose(file);
  return true;
}

// OFFSET: 0x0089C210
// FUNCTION: fn_11583
void UIScrollBar::SetPosition(int value) {
  this->position = value;
}

// OFFSET: 0x0089C220
// FUNCTION: fn_11584
void UIScrollBar::SetRange(int maximum, int visible, int total) {
  this->maximum = maximum;
  this->visible = visible;
  this->total = total;
  this->Invalidate();

  if (this->position > this->maximum) {
    this->position = this->maximum;
  }
}

// OFFSET: 0x0087E190
// FUNCTION: fn_11263
void UIBitmapButton2::Slot10(int argument1, int argument2, std::intptr_t argument3) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(argument1, argument2, argument3);
  } else if (this->parent) {
    this->parent->Slot10(argument1, argument2, argument3);
  }
}

// OFFSET: 0x0087E1D0
// FUNCTION: fn_11264
void UITabControl::Slot10(int argument1, int argument2, std::intptr_t argument3) {
  if (this->parent) {
    this->parent->Slot10(argument1, argument2, argument3);
  }
}

// OFFSET: 0x0087F010

// OFFSET: 0x0087F1D0
// FUNCTION: fn_11270
void UITabControl::DrawTabBorder(int x, int y, int width, int height, std::uint32_t color) {
  if (this->layout == 1) {
    this->FillRegion(x + 2, y, width - 4, 1, color);
    this->FillRegion(x + 1, y + 1, 1, 1, color);
    this->FillRegion(x, y + 2, 1, height - 2, color);
    this->FillRegion(width - 2 + x, y + 1, 1, 1, color);
    this->FillRegion(width - 1 + x, y + 2, 1, height - 2, color);
  } else if (this->layout == 3) {
    this->FillRegion(x, y, 1, height - 2, color);
    this->FillRegion(x + 1, y + height - 2, 1, 1, color);
    this->FillRegion(width - 1 + x, y, 1, height - 2, color);
    this->FillRegion(x + 2, y + height - 1, width - 4, 1, color);
    this->FillRegion(width - 2 + x, y + height - 2, 1, 1, color);
  } else if (this->layout == 0) {
    this->FillRegion(x + 2, y, width - 2, 1, color);
    this->FillRegion(x + 1, y + 1, 1, 1, color);
    this->FillRegion(x, y + 2, 1, height - 4, color);
    this->FillRegion(x + 1, height - 2 + y, 1, 1, color);
    this->FillRegion(x + 2, height - 1 + y, width - 2, 1, color);
  } else if (this->layout == 2) {
    this->FillRegion(x, y, width - 2, 1, color);
    this->FillRegion(x + width - 2, y + 1, 1, 1, color);
    this->FillRegion(x + width - 1, y + 2, 1, height - 4, color);
    this->FillRegion(x, height - 1 + y, width - 2, 1, color);
    this->FillRegion(x + width - 2, height - 2 + y, 1, 1, color);
  }
}

// OFFSET: 0x0087F610
