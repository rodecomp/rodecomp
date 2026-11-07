#include "Network/TextEncoding.h"
#include "Render/Text/FontCache.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/UIBasicCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIImgMenu.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Lists/UITreeView.h"
#include "UI/Buttons/UITVTextButton.h"
#include "UI/Buttons/UITVBitmapButton.h"
#include "UI/Window/InputFocus.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "Render/Text/TextContext.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include "UI/Buttons/UINaviLinkButton.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/TipOfTheDay.h"
#include "Contents/MessageTable.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include "UI/Gauges/UIPcGage.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Session.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/SkillInfo.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Contents/MapSkyMgr.h"
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Text/TextFitting.h"
// OFFSET: 0x0086DDA0
// FUNCTION: fn_11059
UIListBoxTitle::UIListBoxTitle() {
  this->lineHeight = 0xF;
  this->titleColor = 0x92C0FA;
}

// OFFSET: 0x0086E410
// FUNCTION: fn_11070
UIScrollBar2::UIScrollBar2() : UIScrollBar() {
  this->arrowLength = 10;
}

// OFFSET: 0x0086E470
// FUNCTION: fn_11071
UIScrollBar3::UIScrollBar3() : UIScrollBar() {
  this->arrowLength = 9;
}

// OFFSET: 0x0086E550
// FUNCTION: fn_11073
UIScrollBar::UIScrollBar() : UIWindow(0), arrowLength(13) {
  this->maximum = 0;
  this->position = 0;
  this->vertical = 1;
  this->state = 0;
  this->pressX = 0;
  this->pressY = 0;
  this->dragAmount = 0;
  this->total = 0;
  this->visible = 0;
  this->thickness = 13;
  this->minimumThumb = 1;
  this->horizontalStep = 1;
  this->verticalStep = 1;
}

UIScrollBar::~UIScrollBar() = default;

// OFFSET: 0x0086E4D0
// FUNCTION: fn_11072
UIScrollBar4::UIScrollBar4() : UIScrollBar() {
  this->thickness = 6;
  this->arrowLength = 0;
  this->m_thumbLength = 12;
}

// OFFSET: 0x0086E630
// FUNCTION: fn_11074
UIScrollBar_Custom::UIScrollBar_Custom()
    : UIScrollBar(), m_trackTop(nullptr), m_trackMiddle(nullptr), m_trackBottom(nullptr), m_arrowUp(nullptr), m_arrowDown(nullptr),
      m_thumbTop(nullptr), m_thumbMiddle(nullptr), m_thumbBottom(nullptr) {
}

// OFFSET: 0x0086F240
// FUNCTION: fn_11088
UIZeroQuestScrollBar::UIZeroQuestScrollBar() : UIScrollBar() {
  this->arrowLength = 12;
  this->thickness = 9;
}

// OFFSET: 0x00897830
// FUNCTION: fn_11535
void UIScrollBar::Resize(int width, int height) {
  this->Invalidate();

  switch (this->vertical) {
  case 0:
    height = this->thickness;
    UIWindow::Resize(width, height);
    this->OnSize(width, height);
    break;
  case 1:
    width = this->thickness;
    break;
  }

  UIWindow::Resize(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x00882430
// FUNCTION: fn_11303
bool UIScrollBar::IsTransmitMouseInput() {
  if (this->maximum == 0 && this->parent) {
    return this->parent->IsTransmitMouseInput();
  }
  return false;
}

// OFFSET: 0x00880C60
// FUNCTION: fn_11288
int UIScrollBar::ThumbLength() {
  int length = this->geometry.height;
  switch (this->vertical) {
  case 0:
    length = this->geometry.width;
    break;
  case 1:
    length = this->geometry.height;
    break;
  }
  length -= 2 * this->arrowLength;

  if (this->total != 0) {
    length = (length * this->visible) / this->total;
  }

  if (this->minimumThumb != 0 && length < 8) {
    length = 8;
  }

  return std::max(2, length);
}

// OFFSET: 0x00880AA0
// FUNCTION: fn_11281
int UIScrollBar::Position() {
  return this->position;
}

// OFFSET: 0x00880AE0
// FUNCTION: fn_11282
int UIScrollBar::Travel() {
  int length = this->geometry.height;
  switch (this->vertical) {
  case 0:
    length = this->geometry.width;
    break;
  case 1:
    length = this->geometry.height;
    break;
  }
  return length - 2 * this->arrowLength - this->ThumbLength();
}

// OFFSET: 0x00880C00
// FUNCTION: fn_11286
int UIScrollBar::ThumbOffset() {
  return (this->Travel() * this->position) / this->maximum;
}

// OFFSET: 0x008806E0
// FUNCTION: fn_11278
int UIScrollBar::Part(int x, int y) {
  if (this->maximum == 0 || x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    return -1;
  }

  switch (this->vertical) {
  case 0:
    if (x < this->arrowLength) {
      return 0;
    }
    if (x < this->arrowLength + this->ThumbOffset()) {
      return 1;
    }
    if (x < this->arrowLength + this->ThumbLength() + this->ThumbOffset()) {
      return 2;
    }
    if (x < this->geometry.width - this->arrowLength) {
      return 3;
    }
    return 4;
  case 1:
    if (y < this->arrowLength) {
      return 0;
    }
    if (y < this->arrowLength + this->ThumbOffset()) {
      return 1;
    }
    if (y < this->arrowLength + this->ThumbLength() + this->ThumbOffset()) {
      return 2;
    }
    if (y < this->geometry.height - this->arrowLength) {
      return 3;
    }
    return 4;
  default:
    return -1;
  }
}

// OFFSET: 0x00890720
// FUNCTION: fn_11425
void UIScrollBar::OnLBtnDown(int x, int y) {
  if (this->maximum == 0) {
    this->parent->OnLBtnDown((x + this->geometry.x), (y + this->geometry.y));
    return;
  }

  const int previous = this->state;
  const int part = this->Part(x, y);
  this->pressY = y;
  this->pressX = x;
  this->dragAmount = 0;

  switch (this->vertical) {
  case 0:
    switch (part) {
    case 0:
      this->parent->SendMsg(this, 8, -this->horizontalStep, 0, 0, 0);
      break;
    case 1:
      this->parent->SendMsg(this, 11, 0, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SetCapture(this);
      this->state = 6;
      break;
    case 3:
      this->parent->SendMsg(this, 12, 0, 0, 0, 0);
      break;
    case 4:
      this->parent->SendMsg(this, 8, this->horizontalStep, 0, 0, 0);
      break;
    }
    break;
  case 1:
    switch (part) {
    case 0:
      this->parent->SendMsg(this, 7, -this->verticalStep, 0, 0, 0);
      break;
    case 1:
      this->parent->SendMsg(this, 9, 0, 0, 0, 0);
      break;
    case 2:
      g_windowMgr.SetCapture(this);
      this->state = 6;
      break;
    case 3:
      this->parent->SendMsg(this, 10, 0, 0, 0, 0);
      break;
    case 4:
      this->parent->SendMsg(this, 7, this->verticalStep, 0, 0, 0);
      break;
    }
    break;
  }

  if (previous != this->state) {
    this->Invalidate();
  }
}

// OFFSET: 0x00890260
// FUNCTION: fn_11407
void UIScrollBar::OnLBtnDblClk(int x, int y) {
  this->OnLBtnDown(x, y);

  if (this->maximum == 0) {
    this->parent->OnLBtnDblClk((x + this->geometry.x), (y + this->geometry.y));
  }
}

// OFFSET: 0x008921D0
// FUNCTION: fn_11468
void UIScrollBar::OnMouseMove(int x, int y) {
  if (this->maximum == 0) {
    this->parent->OnMouseMove((x + this->geometry.x), (y + this->geometry.y));
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

  if (g_windowMgr.Capture() == this && this->parent) {
    switch (this->vertical) {
    case 0: {
      const int delta = (x - this->pressX) * (this->maximum + 1) / this->Travel() - this->dragAmount;

      if (delta != 0) {
        this->dragAmount += this->parent->SendMsg(this, 8, delta, 0, 0, 0);
      }
      break;
    }
    case 1: {
      const int delta = (y - this->pressY) * (this->maximum + 1) / this->Travel() - this->dragAmount;

      if (delta != 0) {
        this->dragAmount += this->parent->SendMsg(this, 7, delta, 0, 0, 0);
      }
      break;
    }
    }
  }
}

// OFFSET: 0x00892570
// FUNCTION: fn_11474
void UIScrollBar::ResetCursor(int x, int y) {
  if (this->maximum == 0) {
    UIWindow::ResetCursor(x, y);
  } else {
    g_modeMgr.gameMode->SetCursorAction(2);
  }
}

// OFFSET: 0x00890E90
// FUNCTION: fn_11440
void UIScrollBar::OnLBtnUp(int x, int y) {
  if (this->maximum == 0) {
    this->parent->OnLBtnUp(this->geometry.x + x, this->geometry.y + y);
  } else if (g_windowMgr.Capture() == this) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x00892C70
// FUNCTION: fn_11491
void UIScrollBar::OnRBtnUp(int x, int y) {
  if (this->maximum == 0) {
    this->parent->OnRBtnUp((x + this->geometry.x), (y + this->geometry.y));
  }
}

// OFFSET: 0x00892F20
// FUNCTION: fn_11499
void UIScrollBar::OnWheel(int value) {
  this->parent->OnWheel(value);
}

// OFFSET: 0x0088E310
// FUNCTION: fn_11381
void UIScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FFu);

  if (this->maximum == 0) {
    return;
  }

  switch (this->vertical) {
  case 0: {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1left.bmp")));
    this->DrawBitmap(1, 0, bitmap, false);
    CBitmapRes* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1mid.bmp")));
    const int interior = this->geometry.width - this->arrowLength - 14;
    const int count = interior / 13 + (interior % 13 != 0);

    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(this->arrowLength + index * 13, 0, middle, false);
    }

    this->DrawBitmap(this->geometry.width - this->arrowLength - 13, 0, middle, false);
    this->FillRegion(this->geometry.width - this->arrowLength, 0, 13, this->geometry.height, 0xFFFF00FFu);
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1right.bmp")));
    this->DrawBitmap(this->geometry.width - this->arrowLength - 1, 0, bitmap, true);

    const int thumb = this->arrowLength + this->ThumbOffset();
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1bar_left.bmp")));
    this->DrawBitmap(thumb, 0, bitmap, true);
    const int end = thumb + this->ThumbLength() - 4;

    for (int offset = thumb + 4; offset < end; offset += 4) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1bar_mid.bmp")));
      this->DrawBitmap(offset, 0, bitmap, true);
    }

    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll1bar_right.bmp")));
    this->DrawBitmap(end, 0, bitmap, true);
    break;
  }
  case 1: {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0up.bmp")));
    this->DrawBitmap(0, 1, bitmap, false);
    CBitmapRes* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0mid.bmp")));
    const int interior = this->geometry.height - this->arrowLength - 14;
    const int count = interior / 13 + (interior % 13 != 0);

    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(0, this->arrowLength + index * 13, middle, false);
    }

    this->DrawBitmap(0, this->geometry.height - this->arrowLength - 13, middle, false);
    this->FillRegion(0, this->geometry.height - this->arrowLength, this->geometry.width, 13, 0xFFFF00FFu);
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0down.bmp")));
    this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, bitmap, true);

    const int thumb = this->arrowLength + this->ThumbOffset();
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0bar_up.bmp")));
    this->DrawBitmap(0, thumb, bitmap, true);
    const int end = thumb + this->ThumbLength() - 4;

    for (int offset = thumb + 4; offset < end; offset += 4) {
      bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0bar_mid.bmp")));
      this->DrawBitmap(0, offset, bitmap, true);
    }

    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\scroll0bar_down.bmp")));
    this->DrawBitmap(0, end, bitmap, true);
    break;
  }
  }
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
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x00880800
// FUNCTION: fn_11279
int UIListBoxEx::FindLine(const std::string& text) {
  const auto it = std::find_if(this->lines.begin(), this->lines.end(), [text](const auto& entry) { return text == entry.second.text; });
  return it != this->lines.end() ? it->first : -1;
}

// OFFSET: 0x00880920
// FUNCTION: fn_11280
int UIImgMenu::ItemAt(int x, int y) {
  const int width = this->geometry.width - 3;
  const int height = this->geometry.height - 3;
  if (x < 1 || x >= width || y < 1 || y >= height) {
    return -1;
  }

  int index;
  if (this->horizontal != 0) {
    this->itemWidth = width / this->count;
    index = x / this->itemWidth;
  } else {
    this->itemHeight = height / this->count;
    index = y / this->itemHeight;
  }
  return std::min(index, this->count - 1);
}

// OFFSET: 0x0086ECF0
// FUNCTION: fn_11083
UITabControl::UITabControl()
    : UIWindow(0), unknown_00E8(12), flags() {
  this->unknown_007C = -1;
  this->dragX = 0;
  this->dragY = 0;
}

// OFFSET: 0x00879300
// FUNCTION: fn_11215
void UITabControl::SelectTab(int index) {
  if (index < static_cast<int>(this->labels.size()) && this->unknown_007C != index) {
    this->unknown_007C = index;
  }

  this->edit->SetText("");
  this->edit->Move(-100, -100);

  if (this->parent) {
    this->parent->SendMsg(this, 22, this->unknown_007C, 0, 0, 0);
    this->parent->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x0087A3C0
// FUNCTION: fn_11233
void UITabControl::RemoveTab(int index) {
  this->edit->SetText("");
  this->edit->Move(-100, -100);

  if (g_windowMgr.GetHoverWindow() == this->edit) {
    g_windowMgr.NotifyHoverWindow();
  }

  const int count = static_cast<int>(this->labels.size());

  for (int tab = 0; tab < count; ++tab) {
    if (tab == index) {
      this->colors.erase(this->colors.begin() + tab);
      this->labels.erase(this->labels.begin() + tab);
      this->widths.erase(this->widths.begin() + tab);
      this->flags.erase(this->flags.begin() + tab);
      this->unknown_007C = 0;
      this->FitToTabs();
      return;
    }
  }
}

// OFFSET: 0x0087A520
// FUNCTION: fn_11234
void UITabControl::ClearTabs() {
  this->labels.clear();
  this->colors.clear();
  this->widths.clear();
  this->flags.clear();
  this->unknown_007C = 0;
  this->unknown_00E8 = 12;
  this->FitToTabs();
}

// OFFSET: 0x0089C1B0
// FUNCTION: fn_11582
void UITabControl::SetFlag(int index, bool value) {
  if (index >= 0 && index < static_cast<int>(this->labels.size())) {
    this->flags[index] = value;
  }
}

// OFFSET: 0x0089C440
// FUNCTION: fn_11588
void UITabControl::AddTab(const char* label, const char* tooltip) {
  this->labels.push_back(std::string(label));
  this->tooltips.push_back(std::string());

  if (this->parent) {
    const int tabWidth = this->parent->geometry.width / this->labels.size() - this->tabPadding;

    for (std::size_t index = 0; index < this->labels.size(); ++index) {
      if (this->TextWidth(this->labels[index].c_str(), 0, 0, this->unknown_00E8, 0, 0) > tabWidth) {
        if (index != this->labels.size() - 1) {
          if (this->tooltips[index].empty()) {
            this->tooltips[index] = this->labels[index];
          }
        } else if (std::strlen(tooltip) != 0) {
          this->tooltips[index] = this->labels[index];
        }
      }

      if (!this->geometry.surface) {
        this->Resize(tabWidth, 15);
      }
      const std::string fitted = FitText(*this->geometry.surface, this->labels[index], tabWidth);
      this->labels[index] = fitted;
    }
  }

  this->unknown_007C = 0;
  this->colors.push_back(0xFFFFFFFFu);

  int width;
  if (this->layout == 0 || this->layout == 2) {
    width = this->VerticalTextHeight(label, 0, 0, this->unknown_00E8, 0);
  } else {
    width = this->TextWidth(label, 0, 0, this->unknown_00E8, 0, 0);
  }
  this->widths.push_back(width + this->tabPadding);

  if (std::strlen(tooltip) != 0) {
    this->tooltips[this->labels.size() - 1] = tooltip;
  }

  this->flags.push_back(false);
  this->RecalculateWidths();
}


// OFFSET: 0x00880B20
// FUNCTION: fn_11283
int UITabControl::TabCount() {
  return static_cast<int>(this->labels.size());
}

// OFFSET: 0x00880B40
// FUNCTION: fn_11284
int UITabControl::TabAt(int x, int y) {
  int offset;
  if (this->layout == 0 || this->layout == 2) {
    offset = y;
  } else {
    offset = x;
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
  if (index < this->history.size() && this->history[index].size() > 0) {
    return this->history[index].c_str();
  }
  return nullptr;
}

// OFFSET: 0x00882420
// FUNCTION: fn_11302
bool UIChatHisBox::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x00879F10
// FUNCTION: fn_11222
void UIChatHisBox::Clear() {
  UIListBox::Clear();
  this->unknown_00E4.erase(this->unknown_00E4.begin(), this->unknown_00E4.end());
  this->history.erase(this->history.begin(), this->history.end());
}

// OFFSET: 0x0087F430
// FUNCTION: fn_11271
void UIChatHisBox::PlaceNaviLinks(int line) {
  const int top = this->topLine;
  const unsigned int end = std::min(
    static_cast<unsigned int>(this->geometry.height / this->lineHeight - this->bottomMargin + top), static_cast<unsigned int>(this->lines.size())
  );

  for (auto it = this->naviLinks.begin(); it != this->naviLinks.end(); ++it) {
    std::vector<UINaviLinkButton*>& row = *it;
    for (unsigned int index = 0; index < row.size(); ++index) {
      UINaviLinkButton* button = row[index];
      if (button->geometry.id != 438) {
        continue;
      }

      const int buttonLine = button->line;
      if (buttonLine < top || buttonLine > static_cast<int>(end - 1)) {
        button->Move(-100, -100);
      } else if (line == buttonLine) {
        button->Move(button->column + 1, (line - top) * this->lineHeight + 2);
      }
    }
  }
}

// OFFSET: 0x0087F520
// FUNCTION: fn_11272
void UIEditCtrl_Custom::DrawLabel(int x, int y, const char* text) {
  const std::uint32_t outline = this->secondColor;
  const unsigned char bold = this->unknown_011C;
  const std::uint32_t color = this->textColor;
  if (outline == 0xFFFFFFFF) {
    this->DrawTextAt(x, y, text, 0, 0, 12, color, bold, 0);
    return;
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(0, 12, bold, 0);
  context.SetColor(color);
  context.Draw(x, y, text, 0);
  context.SetColor(outline);
  context.Draw(x - 1, y, text, 0);
}

// OFFSET: 0x0087E1F0
// FUNCTION: fn_11265
void UIEditCtrl_Custom::PaintText() {
  int x = this->unknown_0120 + 3;
  const int y = this->unknown_0124 + this->textY;
  std::string display;
  FontCache& fonts = g_fontCache;

  if (g_windowMgr.GetHoverWindow() == this) {
    this->Synchronize();
    int begin;
    int end;
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
      begin = std::min(this->anchor, this->cursor);
      end = std::max(this->anchor, this->cursor);
    } else {
      const char* prefix = g_serverLanguage->Prefix(false);
      display = prefix + this->text;
      begin = std::min(this->anchor, this->cursor) + static_cast<int>(std::strlen(prefix));
      end = std::max(this->anchor, this->cursor) + static_cast<int>(g_serverLanguage->candidate.size()) + static_cast<int>(std::strlen(prefix));
    }

    if (this->unknown_0128 == 1) {
      x = this->geometry.width - x - this->TextWidth(display.c_str(), 0, 0, 12, this->unknown_011C, 0);
    } else if (this->unknown_0128 == 2) {
      const float center = this->geometry.width * g_depthAverageHalf;
      x = static_cast<int>(center - this->TextWidth(display.c_str(), 0, 0, 12, this->unknown_011C, 0) * g_depthAverageHalf);
    }
    const int left = begin ? this->TextWidth(display.c_str(), begin, 0, 12, 0, 0) : 0;
    const int right = end ? this->TextWidth(display.c_str(), end, 0, 12, 0, 0) : 0;

    if (std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor) != 0) {
      this->DrawSelection(x + left, right - left);
      this->DrawLabel(x, y, display.c_str());
    } else if (g_serverLanguage->candidate.size() != 0) {
      this->MoveCandidateWindow(left);
      this->MoveReadingWindow(right);
      if ((timeGetTime() / 500 & 1) == 0) {
        this->DrawSelection(x + left, std::max(right - left, 2));
      }
      this->DrawLabel(x, y, display.c_str());
    } else {
      this->DrawLabel(x, y, display.c_str());
      if (IsInputSuggestionAllowed() == true && (timeGetTime() / 500 & 1) == 0) {
        this->DrawTextAt(x + left - 2, y, "|", 0, 0, 12, this->textColor, 0, 0);
      }
    }
  } else {
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      display = this->text;
    }

    if (this->unknown_0128 == 1) {
      x = this->geometry.width - x - this->TextWidth(display.c_str(), 0, 0, 12, this->unknown_011C, 0);
    } else if (this->unknown_0128 == 2) {
      const float center = this->geometry.width * g_depthAverageHalf;
      x = static_cast<int>(center - this->TextWidth(display.c_str(), 0, 0, 12, this->unknown_011C, 0) * g_depthAverageHalf);
    }
    this->DrawLabel(x, y, display.c_str());
  }

  if (this->invalidatesParent != 0 && this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0087E730
// FUNCTION: fn_11266
void UIBasicItemCtrl::DrawItemIcon(int x, int y, int, bool compact) {
  const int id = std::atoi(this->m_item.itemId.c_str());
  if (id <= 0) {
    return;
  }

  char path[128];
  g_session.IconPath(path, FormatOwnedString("%d", id).c_str(), this->m_item.identified);
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  if (bitmap) {
    this->DrawBitmap(x, y, bitmap, true);
  }

  if (this->m_item.unknown_0000[1] == 0 || this->m_item.quantity == 0) {
    return;
  }

  const std::string count = FormatOwnedString("%d", this->m_item.quantity);
  FontCache& fonts = g_fontCache;
  int textY;
  if (compact) {
    textY = y + 20;
    x += 16;
  } else {
    textY = y + 15;
    x += this->m_item.quantity < 1000 ? 14 : 0;
    this->DrawTextAt(x + 1, textY, count.c_str(), 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x - 1, textY, count.c_str(), 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x, y + 16, count.c_str(), 0, 0, 11, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x, y + 14, count.c_str(), 0, 0, 11, 0xFFFFFF, 0, 0);
  }

  this->DrawTextAt(x, textY, count.c_str(), 0, 0, 11, 0, 0, 0);
}

// OFFSET: 0x008900D0
// FUNCTION: fn_11399
void UIChatHisBox::OnLBtnDblClk(int, int) {
  const unsigned int index = static_cast<unsigned int>(this->selected);

  if (index != static_cast<unsigned int>(-1) && index < this->history.size() && this->history[index].size() > 0 && this->history[index].c_str() != nullptr) {
    this->parent->SendMsg(this, 6, 347, reinterpret_cast<std::intptr_t>(this->HistoryText(index)), 0, 0);
  }
}

// OFFSET: 0x00890430
// FUNCTION: fn_11412
void UIChatHisBox::OnLBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
}

// OFFSET: 0x00890AB0
// FUNCTION: fn_11432
void UIChatHisBox::OnLBtnUp(int, int) {
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
int UIChatHisBox::SaveChatLog(const char* path) {
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

// OFFSET: 0x0087DF00
// FUNCTION: fn_11261
void UIBasicItemCtrl::DragAndDrop(int, int, std::intptr_t argument3) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(argument3);
  if (!drag->skill && drag->identified == this->m_identified) {
    this->m_item = LookupClientItemByIdentity(g_session.m_inventoryItems, drag->id);
    if ((this->m_itemType == 21 && g_session.IsConsumableItemType(this->m_item.unknown_0000[0])) ||
        (this->m_itemType == 22 && g_session.IsEquipItemType(this->m_item.unknown_0000[0])) ||
        (this->m_itemType == 23 && g_session.IsMiscItemType(this->m_item.unknown_0000[0])) || this->m_item.unknown_0000[0] == this->m_itemType ||
        this->m_itemType == -1) {
      if ((this->m_itemMask == -1 || (this->m_item.unknown_0000[2] & this->m_itemMask) != 0) && this->m_item.unknown_0000[1] != 0) {
        if (this->Root()) {
          this->Root()->Invalidate();
          this->Root()->InvalidateChildren();
        } else {
          this->Invalidate();
        }
      }
    }
  }
  g_modeMgr.gameMode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x0087E080
// FUNCTION: fn_11262
void UIBasicSkillCtrl::DragAndDrop(int, int, std::intptr_t argument3) {
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(argument3);
  if (drag->skill) {
    std::string name = drag->skillName;
    CSkillInfo skill = g_session.LookupSkill(drag->id);
    if (skill.level && skill.available && skill.unknown_0014) {
      this->m_skillId = static_cast<int>(drag->id);
      this->m_level = drag->skillLevel;
      if (this->Root()) {
        this->Root()->Invalidate();
        this->Root()->InvalidateChildren();
      } else {
        this->Invalidate();
      }
    }
  }
  g_modeMgr.gameMode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x0087E190
// FUNCTION: fn_11263
void UIBitmapButton2::DragAndDrop(int argument1, int argument2, std::intptr_t argument3) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::DragAndDrop(argument1, argument2, argument3);
  } else if (this->parent) {
    this->parent->DragAndDrop(argument1, argument2, argument3);
  }
}

// OFFSET: 0x0087E1D0
// FUNCTION: fn_11264
void UITabControl::DragAndDrop(int argument1, int argument2, std::intptr_t argument3) {
  if (this->parent) {
    this->parent->DragAndDrop(argument1, argument2, argument3);
  }
}

// OFFSET: 0x0087F010
// FUNCTION: fn_11269
void UITabControl::DrawVerticalTab(int index) {
  const int width = this->widths[index];
  const int textHeight =
    this->VerticalTextHeight(this->labels[index].c_str(), 0, 0, this->unknown_00E8, 0);
  int y = 0;
  for (int previous = 0; previous < index; ++previous) {
    y += this->widths[previous] - 1;
  }

  const int textX = (this->geometry.width - 12) / 2;
  int x = textX + 1;
  const int textY = (width - textHeight) / 2 + (width - textHeight) % 2 + y;
  std::uint32_t color;
  if (this->unknown_007C == index) {
    this->FillTab(index, 0, y, this->geometry.width, width, this->activeFillColor);
    this->DrawTabBorder(0, y, this->geometry.width, width, this->activeBorderColor);
    color = this->activeTextColor;
    x = textX;
  } else {
    if (this->layout == 0) {
      this->FillTab(index, 1, y, this->geometry.width, width, this->inactiveFillColor);
      this->DrawTabBorder(1, y, this->geometry.width, width, this->inactiveBorderColor);
      this->FillRegion(this->geometry.width - 1, y, 1, width, this->activeBorderColor);
    } else if (this->layout == 2) {
      this->FillTab(index, 0, y, this->geometry.width - 1, width, this->inactiveFillColor);
      this->DrawTabBorder(0, y, this->geometry.width - 1, width, this->inactiveBorderColor);
      this->FillRegion(0, y, 1, width, this->activeBorderColor);
    }
    color = this->inactiveTextColor;
  }

  this->DrawVerticalText(x, textY, this->labels[index].c_str(), 0, 0, this->unknown_00E8, color, 0);
}

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
// FUNCTION: fn_11273
void UITabControl::DrawHorizontalTab(int index) {
  const int width = this->widths[index];
  const int textWidth =
    this->TextWidth(this->labels[index].c_str(), 0, 0, this->unknown_00E8, 0, 0);
  int x = 0;
  for (int previous = 0; previous < index; ++previous) {
    x += this->widths[previous] - 1;
  }

  const int textX = (width - textWidth) / 2 + (width - textWidth) % 2 + x;
  const int textY = (this->geometry.height - 12) / 2;
  int y = textY + 1;
  std::uint32_t color;
  if (this->unknown_007C == index) {
    this->FillTab(index, x, 0, width, this->geometry.height, this->activeFillColor);
    this->DrawTabBorder(x, 0, width, this->geometry.height, this->activeBorderColor);
    color = this->activeTextColor;
    y = textY;
  } else {
    if (this->layout == 1) {
      this->FillTab(index, x, 1, width, this->geometry.height, this->inactiveFillColor);
      this->DrawTabBorder(x, 1, width, this->geometry.height, this->inactiveBorderColor);
      this->FillRegion(x, this->geometry.height - 1, width, 1, this->activeBorderColor);
    } else if (this->layout == 3) {
      this->FillTab(index, x, 0, width, this->geometry.height - 1, this->inactiveFillColor);
      this->DrawTabBorder(x, 0, width, this->geometry.height - 1, this->inactiveBorderColor);
      this->FillRegion(x, 0, width, 1, this->activeBorderColor);
    }
    color = this->inactiveTextColor;
  }

  this->DrawTextAt(textX, y, this->labels[index].c_str(), 0, 0,
    this->unknown_00E8, color, 0, 0
  );
}

// OFFSET: 0x008810C0
// FUNCTION: fn_11295
void UITabControl::HideEdit() {
  this->edit->SetText("");
  this->edit->Move(-100, -100);

  if (g_windowMgr.GetHoverWindow() == this->edit) {
    g_windowMgr.NotifyHoverWindow();
  }
}

// OFFSET: 0x00899A30
// FUNCTION: fn_11559
int UITabControl::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != 0) {
    return 0;
  }

  if (sender != this->edit || this->edit->geometry.x == -100) {
    if (this->parent) {
      this->parent->SendMsg(this, 0, 0, 0, 0, 0);
    }
    return 0;
  }

  if (argument1 != 1) {
    std::string text = this->edit->Text();
    if (text.empty()) {
      return 0;
    }

    const int width = this->TextWidth(text.c_str(), 0, 0, this->unknown_00E8, 0, 0);
    if ((this->tabPadding + width) * this->TabCount() >= this->parent->geometry.width - 90) {
      g_windowMgr.Confirm(LookupMessage(1577), 0, true, false, 0, "Message", 280, 120, 0);
    } else {
      this->labels[this->unknown_007C] = text;
      const int previousWidth = this->geometry.width;
      this->RecalculateWidths();
      if (this->parent) {
        this->parent->SendMsg(this, 116, reinterpret_cast<std::intptr_t>(text.c_str()), this->unknown_007C, previousWidth > this->geometry.width ? previousWidth : 0, 0);
      }
    }
  }

  this->HideEdit();
  this->Invalidate();
  return 0;
}

// OFFSET: 0x0087CD70
// FUNCTION: fn_11257
void UIChatHisBox::DropOldestLines() {
  unsigned int half = static_cast<unsigned int>(this->lines.size()) / 2;
  for (unsigned int index = 0; index < half; ++index) {
    this->lines[index] = this->lines[index + half];
  }
  this->lines.resize(half);

  half = static_cast<unsigned int>(this->unknown_00E4.size()) / 2;
  for (int index = 0; index < static_cast<int>(half); ++index) {
    this->unknown_00E4[index] = this->unknown_00E4[index + half];
  }
  this->unknown_00E4.resize(half);

  half = static_cast<unsigned int>(this->history.size()) / 2;
  for (unsigned int index = 0; index < half; ++index) {
    this->history[index] = this->history[index + half];
  }
  this->history.resize(half);

  this->Refresh();
}

// OFFSET: 0x0087D080
// FUNCTION: fn_11259
void UIChatHisBox::ShiftNaviLinks() {
  const unsigned int half = static_cast<unsigned int>(this->lines.size()) / 2;
  std::list<std::vector<UINaviLinkButton*>>::iterator row = this->naviLinks.begin();
  while (row != this->naviLinks.end()) {
    bool removed = false;
    for (unsigned int index = 0; index < row->size(); ++index) {
      UINaviLinkButton* button = (*row)[index];
      if (button->geometry.id == 438) {
        button->line -= half;
        if (button->line < 0) {
          removed = true;
          this->RemoveChild((*row)[index], 1);
        }
      }
    }

    if (!removed) {
      ++row;
      continue;
    }

    std::vector<UINaviLinkButton*> buttons;
    buttons.swap(*row);
    row = this->naviLinks.erase(row);
  }
}

// OFFSET: 0x0087CA70
// FUNCTION: fn_11255
__declspec(noinline) void UIChatHisBox::ClearNaviLinks() {
  for (auto row = this->itemTags.begin(); row != this->itemTags.end();) {
    for (unsigned int index = 0; index < row->size(); ++index) {
      this->RemoveChild((*row)[index], 1);
    }
    std::vector<UIItemTagButton*> buttons;
    buttons.swap(*row);
    row = this->itemTags.erase(row);
  }
  this->itemTags.clear();
}

// OFFSET: 0x00879E80
// FUNCTION: fn_11221
void UIChatHisBox::ResetContents() {
  this->unknown_0100.clear();
  this->unknown_010C.clear();
  this->unknown_0118.clear();
  this->Clear();
  this->ClearNaviLinks();
  this->ShiftNaviLinks();
}

// OFFSET: 0x0087CC30
// FUNCTION: fn_11256
void UIChatHisBox::DropOldestRecords() {
  unsigned int half = static_cast<unsigned int>(this->unknown_0100.size()) / 2;
  for (unsigned int index = 0; index < half; ++index) {
    this->unknown_0100[index] = this->unknown_0100[index + half];
  }
  this->unknown_0100.resize(half);

  half = static_cast<unsigned int>(this->unknown_010C.size()) / 2;
  for (int index = 0; index < static_cast<int>(half); ++index) {
    this->unknown_010C[index] = this->unknown_010C[index + half];
  }
  this->unknown_010C.resize(half);

  half = static_cast<unsigned int>(this->unknown_0118.size()) / 2;
  for (unsigned int index = 0; index < half; ++index) {
    this->unknown_0118[index] = this->unknown_0118[index + half];
  }
  this->unknown_0118.resize(half);

  this->RebuildHistory();
}

// OFFSET: 0x0087CEC0
// FUNCTION: fn_11258
void UIChatHisBox::ShiftItemTags() {
  const unsigned int half = static_cast<unsigned int>(this->lines.size()) / 2;
  std::list<std::vector<UIItemTagButton*>>::iterator row = this->itemTags.begin();
  while (row != this->itemTags.end()) {
    bool removed = false;
    for (unsigned int index = 0; index < row->size(); ++index) {
      (*row)[index]->line -= half;
      if ((*row)[index]->line < 0) {
        removed = true;
        this->RemoveChild((*row)[index], 1);
      }
    }

    if (!removed) {
      ++row;
      continue;
    }

    std::vector<UIItemTagButton*> buttons;
    buttons.swap(*row);
    row = this->itemTags.erase(row);
  }
}

// OFFSET: 0x0087E9B0
// FUNCTION: fn_11267
int UITreeView::LayoutNodes(std::list<UITreeNode*>& nodes, int x, int y) {
  int rowHeight = 0;
  const int first = this->unknown_00A8;
  const int last = this->geometry.height / this->itemHeight + first;
  const int gap = this->itemPadding - 2;
  if (static_cast<int>(nodes.size()) <= 0) {
    return y;
  }

  for (std::list<UITreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it, ++this->itemCount) {
    UITreeNode* node = *it;
    int labelX = x;
    if (node->collapseButton) {
      labelX = node->collapseButton->BitmapWidth() + gap + x;
    }

    int bottom = y;
    if (this->itemCount >= first && this->itemCount < last) {
      if (node->icon > -1) {
        auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(node->icons[node->icon].c_str())));
        bottom = bitmap->image.height + y;
        if (bottom >= this->geometry.height) {
          continue;
        }
        this->DrawBitmap(labelX, y, bitmap, true);
        labelX += bitmap->image.width + gap;
      }

      if (bottom >= this->geometry.height) {
        continue;
      }

      UITVTextButton* label = node->labelButton;
      label->unknown_00B4 = 0;
      label->Invalidate();
      if (this->unknown_0088 && this->unknown_0088 == node) {
        node->labelButton->unknown_00B4 = 1;
        node->labelButton->Invalidate();
      }

      label = node->labelButton;
      label->unknown_00B5 = 0;
      label->m_highlightColor = 0;
      label->Invalidate();
      if (this->unknown_00AC && this->unknown_008C && this->unknown_0090 && this->unknown_00B0) {
        if (this->unknown_0090 == node) {
          label = node->labelButton;
          label->unknown_00B5 = 1;
          label->m_highlightColor = this->unknown_00B0;
          label->Invalidate();
        }
        if (node->unknown_0170 && node->unknown_0170->labelButton && node->unknown_0170->labelButton->unknown_00B5) {
          label = node->labelButton;
          label->m_highlightColor = this->unknown_00B0;
          label->unknown_00B5 = 1;
          label->Invalidate();
        }
      }

      int maxWidth = this->geometry.width - labelX - 2;
      if (this->verticalOverflow) {
        maxWidth -= this->verticalBar->geometry.width;
      }

      node->labelButton->Move(labelX, y + 2);
      label = node->labelButton;
      const int height = label->geometry.height;
      if (maxWidth != label->geometry.width && std::strcmp(label->label.c_str(), "") != 0) {
        label->unknown_00DC = label->label;
        int width;
        if (label->geometry.width > maxWidth) {
          const char* fitted = label->ClipTextToWidth(label->label.c_str(), maxWidth - 2, label->m_fontFamily, label->m_fontHeight, label->m_outlined);
          label->unknown_00DC = fitted;
          width = label->m_border ? maxWidth + 2 : maxWidth;
        } else {
          width = label->TextWidth(label->label.c_str(), 0, label->m_fontFamily, label->m_fontHeight,
                    label->m_outlined, 0
                  ) +
                  2;
          width = label->m_border ? width + 2 : width;
        }
        label->UIWindow::Resize(width, height);
      }

      if (this->editNode == node) {
        node->labelButton->Move(-100, -100);
        this->edit->Resize(node->labelButton->geometry.width, node->labelButton->geometry.height);
        this->edit->Move(labelX, y + 2);
      }

      rowHeight = node->labelButton->geometry.height + this->itemPadding;
      this->itemHeight = std::max(this->itemHeight, rowHeight);
    }

    if (bottom >= this->geometry.height) {
      continue;
    }

    if (node->expanded == 0) {
      if (this->itemCount >= first && this->itemCount < last) {
        if (node->expandButton) {
          node->expandButton->Move(x, y + 4);
        }
        y += rowHeight;
      }
    } else if (node->expanded == 1) {
      if (this->itemCount >= first && this->itemCount < last) {
        if (node->collapseButton) {
          node->collapseButton->Move(x, y + 4);
        }
        y += rowHeight;
        bottom += rowHeight;
      }

      if (static_cast<int>(node->children.size()) > 0 && bottom < this->geometry.height) {
        if (node->collapseButton) {
          x += node->collapseButton->BitmapWidth() + gap;
        }
        const int next = this->LayoutNodes(node->children, x, y);
        if (this->itemCount >= first && this->itemCount < last) {
          y = next;
        }
        if (node->collapseButton) {
          x -= node->collapseButton->BitmapWidth() + gap;
        }
      }
    }
  }
  return y;
}

// OFFSET: 0x0087EF30
// FUNCTION: fn_11268
void UIChatHisBox::PlaceItemTags(int line) {
  const int top = this->topLine;
  const unsigned int end = std::min(
    static_cast<unsigned int>(this->geometry.height / this->lineHeight - this->bottomMargin + top), static_cast<unsigned int>(this->lines.size())
  );

  for (auto it = this->itemTags.begin(); it != this->itemTags.end(); ++it) {
    std::vector<UIItemTagButton*>& row = *it;
    for (unsigned int index = 0; index < row.size(); ++index) {
      UIItemTagButton* button = row[index];
      const int buttonLine = button->line;
      if (buttonLine < top || buttonLine > static_cast<int>(end - 1)) {
        button->Move(-100, -100);
      } else if (line == buttonLine) {
        button->Move(button->column + 1, (line - top) * this->lineHeight + 2);
      }
    }
  }
}

// OFFSET: 0x00889710
// FUNCTION: fn_11349
void UIChatHisBox::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent && this->geometry.x >= 0 && this->geometry.y >= 0) {
    this->parent->SendMsg(this, 116, 0, 0, 0, 0);
  }

  const unsigned int lineCount = static_cast<unsigned int>(this->lines.size());
  const int top = this->topLine;
  const unsigned int visible = static_cast<unsigned int>(this->geometry.height / this->lineHeight - this->bottomMargin + top);
  const unsigned int end = lineCount < visible ? lineCount : visible;

  FontCache& fonts = g_fontCache;
  for (int row = 0; top + row < static_cast<int>(end); ++row) {
    const int index = top + row;
    int x;
    if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
      const int width = this->TextWidth(this->lines[index].c_str(), 0, 0, 12, 0, 0);
      this->DrawTextAt(this->geometry.width - width - 18, this->lineHeight * row + 4, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0);
      x = this->geometry.width - width - 19;
    } else {
      this->DrawTextAt(3, this->lineHeight * row + 4, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0);
      x = 2;
    }
    this->DrawTextAt(x, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->unknown_00E4[index], 0, 0);
    this->PlaceNaviLinks(index);
    this->PlaceItemTags(index);
  }
}

// OFFSET: 0x0089ADD0
// FUNCTION: fn_11563
int UITreeView::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 0:
    if (sender == this->edit && this->editNode) {
      this->SendMsg(nullptr, 110, 0, 1, 0, 0);
    }
    break;
  case 7:
  case 9:
  case 10: {
    this->HideNodes(this->root->children);
    const int previous = this->unknown_00A8;
    const int visible = this->geometry.height / this->itemHeight;
    if (message == 9) {
      this->unknown_00A8 = previous - visible + 1;
    } else if (message == 10) {
      this->unknown_00A8 = previous - 1 + visible;
    } else {
      this->unknown_00A8 = previous + static_cast<int>(a);
    }

    if (this->unknown_00A8 < 0) {
      this->unknown_00A8 = 0;
    }

    const int maximum = this->itemCount - visible;
    if (this->unknown_00A8 > maximum) {
      this->unknown_00A8 = maximum;
    }

    this->verticalBar->position = this->unknown_00A8;
    this->verticalBar->Invalidate();
    this->Invalidate();
    if (this->parent) {
      this->parent->Invalidate();
    }
    return this->unknown_00A8 - previous;
  }
  case 18:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, 7, -1, 0, 0, 0);
    }
    break;
  case 19:
    if (this->verticalOverflow) {
      this->SendMsg(this->verticalBar, 7, 1, 0, 0, 0);
    }
    break;
  case 97:
    this->unknown_0088 = reinterpret_cast<UITreeNode*>(a);
    if (a) {
      this->parent->SendMsg(this, 97, a, sender->geometry.x + b, sender->geometry.height / 2 + sender->geometry.y, 0);
    }
    break;
  case 98:
  case 99:
  case 100:
    this->unknown_008C = nullptr;
    this->unknown_0090 = nullptr;
    this->unknown_0088 = reinterpret_cast<UITreeNode*>(a);
    if (!a) {
      break;
    }

    if (message == 99) {
      this->parent->SendMsg(this, 99, a, sender->geometry.x + b, sender->geometry.height / 2 + sender->geometry.y, 0);
      break;
    }

    if (this->unknown_0088->children.size() != 0) {
      this->HideNodes(this->root->children);
      const int x = sender->geometry.x;
      const int y = message == 98 ? sender->geometry.y - 6 : sender->geometry.y;
      UIWindow* button;
      if (this->unknown_0088->expanded == 0) {
        this->unknown_0088->expanded = 1;
        button = this->unknown_0088->collapseButton;
      } else {
        this->unknown_0088->expanded = 0;
        button = this->unknown_0088->expandButton;
      }
      button->Move(x, y);
      this->Invalidate();
    }

    if (message == 98) {
      this->parent->SendMsg(
        this, 98, reinterpret_cast<std::intptr_t>(this->unknown_0088), sender->geometry.x + b, sender->geometry.height / 2 + sender->geometry.y, 0
      );
    }

    this->unknown_008C = nullptr;
    this->unknown_0090 = nullptr;
    break;
  case 107:
    this->unknown_008C = reinterpret_cast<UITreeNode*>(a);
    this->unknown_0088 = reinterpret_cast<UITreeNode*>(a);
    if (this->unknown_008C->draggable == 0) {
      this->unknown_008C = nullptr;
    }
    break;
  case 108:
    g_modeMgr.gameMode->SetCursorAction(0);
    if (this->unknown_008C && this->unknown_008C->id != a) {
      UITreeNode* target = reinterpret_cast<UITreeNode*>(a);
      this->unknown_0090 = target;
      if (target->container == 0) {
        this->unknown_0090 = target->unknown_0170;
      }
      this->unknown_0090->expanded = 1;
      this->Invalidate();
    }
    break;
  case 109:
    if (sender == this->parent) {
      this->unknown_0090 = nullptr;
    }

    if (this->unknown_008C) {
      if (this->unknown_008C->unknown_0170 == this->unknown_0090) {
        break;
      }
      if (this->unknown_0090 && this->unknown_0090->unknown_0170 == this->unknown_008C) {
        break;
      }

      g_modeMgr.gameMode->SetCursorAction(2);
      UITreeNode* node = this->unknown_008C;
      UITVTextButton* label = node->labelButton;
      this->RemoveChildNode(node->unknown_0170, node->id, true);
      UITreeNode* previousParent = this->unknown_008C->unknown_0170;
      this->unknown_008C->unknown_0170 = this->unknown_0090;
      UITreeNode* target = this->unknown_0090 ? this->unknown_0090 : this->root;
      target->children.push_back(this->unknown_008C);
      this->unknown_008C->labelButton = label;
      if (this->parent) {
        this->parent->SendMsg(
          this, 109, reinterpret_cast<std::intptr_t>(this->unknown_008C), reinterpret_cast<std::intptr_t>(this->unknown_0090),
          reinterpret_cast<std::intptr_t>(previousParent), 0
        );
      }
      this->HideNodes(this->root->children);
    }

    this->unknown_0088 = nullptr;
    this->unknown_008C = nullptr;
    this->unknown_0090 = nullptr;
    this->Invalidate();
    break;
  case 110:
    if (b == 0) {
      this->editNode = reinterpret_cast<UITreeNode*>(a);
      this->edit->maximum = c > 361 ? 361 : static_cast<int>(c);
      this->edit->SetText(this->editNode->name);
      g_windowMgr.SetFocus(this->edit);
      this->Invalidate();
    } else if (b == 1) {
      if (g_windowMgr.GetHoverWindow() == this->edit) {
        const std::size_t length = std::strlen(this->edit->Text());
        if (this->parent) {
          this->parent->SendMsg(this, 110, reinterpret_cast<std::intptr_t>(this->edit->Text()), length + 1, 0, 0);
        }
      }
    } else {
      const int x = this->edit->geometry.x;
      const int y = this->edit->geometry.y;
      this->edit->Move(-100, -100);
      if (c != 0) {
        std::strcpy(this->editNode->name, this->edit->Text());
        this->editNode->labelButton->SetLabel(std::string(this->edit->Text()));
        this->editNode->labelButton->ResizeToText();
      }
      this->editNode->labelButton->Move(x, y);
      this->editNode = nullptr;
    }
    break;
  default:
    break;
  }

  return 0;
}

// OFFSET: 0x0089E570
// FUNCTION: fn_11602
bool UITreeView::DeleteChildNodes(UITreeNode* node) {
  if (!node) {
    return false;
  }

  int remaining = static_cast<int>(node->children.size());
  if (remaining <= 0) {
    return false;
  }

  for (auto it = node->children.begin(); it != node->children.end(); ++it) {
    UITreeNode* child = *it;
    if (child->children.size() != 0) {
      this->DeleteChildNodes(child);
    }

    if ((*it)->collapseButton) {
      (*it)->collapseButton->Move(-100, -100);
      (*it)->collapseButton = nullptr;
    }

    if ((*it)->expandButton) {
      (*it)->expandButton->Move(-100, -100);
      (*it)->expandButton = nullptr;
    }

    (*it)->labelButton->Move(-100, -100);
    --remaining;
    (*it)->labelButton = nullptr;

    if (node->children.size() == 0) {
      if (node->collapseButton) {
        node->collapseButton->Move(-100, -100);
        node->collapseButton = nullptr;
      }

      if (node->expandButton) {
        node->expandButton->Move(-100, -100);
        node->expandButton = nullptr;
      }
    }

    delete *it;

    if (remaining == 0) {
      node->children.clear();
      return true;
    }
  }

  return true;
}

// OFFSET: 0x0089E6F0
// FUNCTION: fn_11603
int UITreeView::RemoveChildNode(UITreeNode* parent, int id, bool keepButtons) {
  if (!parent) {
    parent = this->root;
  }

  if (static_cast<int>(parent->children.size()) <= 0) {
    return 0;
  }

  for (auto it = parent->children.begin(); it != parent->children.end(); ++it) {
    if ((*it)->id != id) {
      continue;
    }

    if (!keepButtons) {
      if ((*it)->children.size() != 0) {
        this->DeleteChildNodes(*it);
      }

      if ((*it)->collapseButton) {
        (*it)->collapseButton->Move(-100, -100);
        (*it)->collapseButton = nullptr;
      }

      if ((*it)->expandButton) {
        (*it)->expandButton->Move(-100, -100);
        (*it)->expandButton = nullptr;
      }
    }

    (*it)->labelButton->Move(-100, -100);
    (*it)->labelButton = nullptr;
    UITreeNode* removed = *it;
    parent->children.erase(it);
    delete removed;

    if (parent->children.size() == 0) {
      if (parent->collapseButton) {
        parent->collapseButton->Move(-100, -100);
        parent->collapseButton = nullptr;
      }

      if (parent->expandButton) {
        parent->expandButton->Move(-100, -100);
        parent->expandButton = nullptr;
      }
    }

    return 1;
  }

  return 0;
}

// OFFSET: 0x0089E470
// FUNCTION: fn_11600
bool UIGage::CanHitTest() {
  return false;
}

// OFFSET: 0x0087C3F0
// FUNCTION: fn_11253
void UITabControl::Configure(
  int layout, int tabHeight, int tabPadding, unsigned char uniformWidths, unsigned char unknownA9, unsigned char unknownA8, std::uint32_t activeFillColor,
  std::uint32_t inactiveFillColor, std::uint32_t activeBorderColor, std::uint32_t inactiveBorderColor, std::uint32_t activeTextColor,
  std::uint32_t inactiveTextColor
) {
  this->OnCreate(0, 0);

  this->layout = layout;
  this->tabHeight = tabHeight;
  this->tabPadding = tabPadding;
  this->uniformWidths = uniformWidths;
  this->unknown_00A9 = unknownA9;
  this->unknown_00A8 = unknownA8;
  this->activeFillColor = activeFillColor ? activeFillColor : 0xFFFFFF;
  this->inactiveFillColor = inactiveFillColor ? inactiveFillColor : 0xFFFFFF;
  this->activeBorderColor = activeBorderColor ? activeBorderColor : 0xC3C3C3;
  this->inactiveBorderColor = inactiveBorderColor ? inactiveBorderColor : 0xC3C3C3;
  this->activeTextColor = activeTextColor;
  this->inactiveTextColor = inactiveTextColor;
  this->ClearTabs();

  this->edit = new UIEditCtrl2();
  this->edit->SetSize(100, 19);
  this->edit->Move(-100, -100);
  this->AddChild(this->edit);
}

// OFFSET: 0x0087D240
// FUNCTION: fn_11260
bool UIItemTagCtrl::RemoveInvalidTag(std::list<ItemTagRecord>::iterator& entry) {
  if (!this->geometry.surface) {
    this->Resize(this->tagWidth / 2, 15);
  }
  const std::string fitted = FitText(*this->geometry.surface, entry->name, this->tagWidth / 2);
  std::string remaining = this->taggedText;
  const int bytes = static_cast<int>(std::strlen(fitted.c_str()));
  int found = static_cast<int>(remaining.find(fitted));

  if (found == -1) {
    this->RemoveTagChild(entry->child);
    entry = this->tags.erase(entry);
    return true;
  }

  int absolute = found;
  int count = 0;

  do {
    std::string before;
    if (found > 0) {
      before = remaining.substr(found - 1, 1);
    }

    std::string after;
    if (found + bytes >= 0) {
      after = remaining.substr(found + bytes, 1);
    }

    bool atCursor;
    if (before.find("<") != std::string::npos && after.find(">") == std::string::npos) {
      atCursor = (!g_windowMgr.unknown_0458 || static_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents == g_windowMgr.GetHoverWindow()) &&
                 absolute + bytes == g_serverLanguage->cursor;
    } else if (before.find("<") == std::string::npos && after.find(">") != std::string::npos) {
      atCursor = (!g_windowMgr.unknown_0458 || static_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458)->m_contents == g_windowMgr.GetHoverWindow()) &&
                 absolute == g_serverLanguage->cursor;
    } else if (before.find("<") != std::string::npos && after.find(">") != std::string::npos) {
      ++count;
      if (count > entry->index) {
        return false;
      }
      atCursor = false;
    } else {
      remaining = remaining.substr(found + bytes);
      absolute = (absolute == found ? absolute : absolute + found) + bytes;
      found = static_cast<int>(remaining.find(fitted));
      continue;
    }

    if (atCursor) {
      remaining = before.find("<") != std::string::npos ? this->taggedText.substr(0, absolute - 1) : this->taggedText.substr(0, absolute);
      int end = absolute;
      if (after.find(">") != std::string::npos) {
        ++end;
      }
      remaining += this->taggedText.substr(end + bytes);
      g_serverLanguage->ResetInput();
      g_serverLanguage->Insert(remaining.c_str());
      this->RemoveTagChild(entry->child);
      entry = this->tags.erase(entry);
      return true;
    }

    if (entry->index == 0) {
      break;
    }

    remaining = remaining.substr(found + bytes);
    found = static_cast<int>(remaining.find(fitted));
    if (found == -1) {
      return false;
    }
    absolute += found + bytes;
  } while (found != -1);

  return false;
}

#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/TipBoxTitles.h"
#include "UI/Text/NavigationLink.h"

#include <mbstring.h>

// GLOBAL: gl_24662
std::string g_itemLinkTagBegin = "<ITEML>";
// GLOBAL: gl_24663
std::string g_itemLinkTagEnd = "</ITEML>";
// GLOBAL: gl_24658
std::string g_naviLinkTagBegin = "<NAVIL>";
// GLOBAL: gl_24659
std::string g_naviLinkTagEnd = "</NAVIL>";

// OFFSET: 0x00882460
// FUNCTION: fn_11305
std::string UIChatHisBox::RenderItemLinks(const char* lineText, std::string& source) {
  if (source.find(g_itemLinkTagBegin) == std::string::npos) {
    return lineText;
  }

  std::string remaining = source;
  std::string result;
  std::vector<UIItemTagButton*> buttons;
  unsigned position = 0;
  unsigned previous = 0;

  for (;;) {
    std::string encoded;
    if (!this->Next(remaining, encoded, position, source.c_str()) || result.size() >= std::strlen(lineText)) {
      break;
    }

    const std::string prefix = source.substr(previous, position - previous);
    if (result.size() + prefix.size() > std::strlen(lineText)) {
      const int available = static_cast<int>(std::strlen(lineText) - result.size());
      if (available > 0) {
        result += prefix.substr(0, available);
        previous += available;
        break;
      }
    }
    result += prefix;

    int titleId = 0;
    if (encoded.substr(0, 6) == "00000T" && (titleId = ItemLinkMessageId(encoded)) <= g_itemTagOwner->unknown_0118) {
      std::string label;
      const std::string title = g_itemTagOwner->Title(titleId, &label, 17);
      if (!title.empty()) {
        UIItemTagButton* button = this->CreateItemTagButton(title, label, titleId, this, 0x7F00E4);
        if (button) {
          buttons.push_back(button);
          button->line = static_cast<int>(this->lines.size());
          button->column = this->TextWidth(result.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;
          result += button->GetLine(0);
        }
      }
      position += static_cast<unsigned>(encoded.size() + g_itemLinkTagEnd.size() + g_itemLinkTagBegin.size());
    } else {
      std::vector<std::int32_t> fields;
      DecodeItemLinkFields(encoded, fields);
      ITEM_INFO item;
      if (ItemFromLinkFields(fields, item)) {
        const std::string label = this->BracketName(item.DisplayName(0));
        if (result.size() + label.size() > std::strlen(lineText)) {
          previous += static_cast<unsigned>(prefix.size());
          break;
        }

        UIItemTagButton* button = new (std::nothrow) UIItemTagButton();
        if (button) {
          std::memcpy(button->unknown_00F8, &item, sizeof(item));
          button->Configure(label.c_str(), 0x82F2E1, 0, 12, 0, 0);
          button->SetTooltip(item.DisplayName(0).c_str());
          button->m_hoverColor = 0x82F2E1;
          button->SetId(0x1D0);
          button->ResizeToTextClipped(
              this->TextWidth(label.c_str(), 0, 0, 12, 0, 0) + 1,
              button->geometry.height);

          const std::string unknownName = LookupMessage(581);
          if (label.find(unknownName) != std::string::npos) {
            std::uint32_t creator[2];
            std::memcpy(creator, item.unknown_001C + 8, sizeof(creator));
            button->unknown_01F8 = creator[0] + (creator[1] << 16);
            g_session.AddUnknown7F94(button->unknown_01F8);
          }

          this->AddChild(button);
          buttons.push_back(button);
          button->line = static_cast<int>(this->lines.size());
          button->column = this->TextWidth(result.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;
          result += label;
          position += static_cast<unsigned>(encoded.size() + g_itemLinkTagEnd.size() + g_itemLinkTagBegin.size());
        }
      }
    }

    previous = position;
  }

  if (result.size() < std::strlen(lineText)) {
    const int available = static_cast<int>(std::strlen(lineText) - result.size());
    if (available > 0 && static_cast<unsigned>(available) <= remaining.size()) {
      result += remaining.substr(0, available);
      previous += available;
    }
  }

  source = source.substr(previous);
  this->itemTags.push_back(buttons);
  return result;
}

// OFFSET: 0x00883460
// FUNCTION: fn_11306
std::string UIChatHisBox::RenderNavigationLinks(const char* text, std::string& source) {
  if (_mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>("<")) == nullptr &&
      _mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(">")) == nullptr) {
    const std::size_t position = source.find(text);
    if (position != std::string::npos) {
      source = source.substr(position + std::strlen(text));
    }
    return std::string(text);
  }

  std::vector<UINaviLinkButton*> buttons;
  std::string remaining = source;
  std::string result;
  std::string link;

  for (;;) {
    const std::size_t begin = remaining.find(g_naviLinkTagBegin);
    if (begin == std::string::npos) {
      break;
    }

    result.clear();
    result += remaining.substr(0, begin);
    source = source.substr(begin);
    link = remaining.substr(begin);
    const std::size_t end = link.find(g_naviLinkTagEnd);
    if (end == std::string::npos) {
      break;
    }
    link = link.substr(0, end + g_naviLinkTagEnd.size());

    const std::string name = FormatNavigationLink(link, false);
    std::string label = "<";
    const int width = this->geometry.width / 2;
    if (this->geometry.surface == nullptr) {
      this->Resize(width, 15);
    }
    label += FitText(*this->geometry.surface, name, width);
    label += ">";

    std::string tail;
    const std::string line(text);
    const std::size_t position = line.find(label);
    if (position != std::string::npos) {
      tail = line.substr(position + label.size());
    }

    UINaviLinkButton* button = new (std::nothrow) UINaviLinkButton();
    if (button) {
      button->link = link;
      button->Configure(label.c_str(), 0x82F2E1, 0, 12, 0, 0);
      if (label.size() - 2 < name.size()) {
        button->SetTooltip(name.c_str());
      }
      button->m_hoverColor = 0x82F2E1;
      button->SetId(438);
      const int height = button->geometry.height;
      button->ResizeToTextClipped(
          this->TextWidth(label.c_str(), 0, 0, 12, 0, 0) + 1, height);
      this->AddChild(button);
      buttons.push_back(button);
      button->line = static_cast<int>(this->lines.size());
      button->column = this->TextWidth(result.c_str(), 0, 0, 12, 0, 0) - this->horizontalOffset;

      result += label;
      result += tail;
      source = source.substr(tail.size() + label.size());
      remaining = source;
    }
  }

  if (!buttons.empty()) {
    this->naviLinks.push_back(buttons);
  }
  return result;
}

// OFFSET: 0x00894090
// FUNCTION: fn_11508
void UIChatHisBox::RewrapLinkLines(std::vector<std::string>& wrapped, int& mode) {
  const std::string open = "<";
  const std::string close = ">";
  std::string remaining = "";
  for (unsigned int line = 0; line < wrapped.size(); ++line) {
    if (wrapped.size() > 23) {
      break;
    }

    std::vector<std::string> split;
    this->WrapLinkText(&mode, &split, wrapped[line]);
    if (split.size() > 1) {
      for (unsigned int part = 0; part < split.size(); ++part) {
        if (part == 0) {
          wrapped[line] = split[part];
        }
        if (line + part < wrapped.size() && part == split.size() - 1) {
          wrapped[line + part].swap(split[part].append(wrapped[line + part]));
        } else if (part != 0) {
          std::vector<std::string> rebuilt;
          for (unsigned int index = 0; index < wrapped.size(); ++index) {
            if (!wrapped[index].empty()) {
              rebuilt.push_back(wrapped[index]);
            }
          }
          if (!split[part].empty()) {
            rebuilt.push_back(split[part]);
          }
          for (unsigned int next = line + 1; next < wrapped.size(); ++next) {
            if (!wrapped[next].empty()) {
              rebuilt.push_back(wrapped[next]);
            }
          }
          wrapped.swap(rebuilt);
        }
      }
    }

    remaining = wrapped[line];
    unsigned int offset = 0;
    std::size_t position = remaining.find(close);
    while (position != std::string::npos) {
      ++position;
      offset += static_cast<unsigned int>(position);
      remaining = remaining.substr(position);
      position = remaining.find(close);
    }

    position = remaining.find(open);
    if (position != std::string::npos) {
      offset += static_cast<unsigned int>(position);
      remaining = remaining.substr(position);
      std::string tail = remaining.substr(1);
      position = tail.find(open);
      while (position != std::string::npos) {
        offset += static_cast<unsigned int>(position);
        remaining = tail;
        tail = remaining.substr(1);
        position = tail.find(open);
      }

      if (line + 1 < wrapped.size() && wrapped[line + 1].find(close) != std::string::npos) {
        remaining.append(wrapped[line + 1]);
        wrapped[line + 1].swap(remaining);
        wrapped[line] = wrapped[line].substr(0, offset);
      }
    }
  }
}

// OFFSET: 0x0089DC30
// FUNCTION: fn_11598
void UIChatHisBox::AddItemLinkLine(const char* text, int& color, int& mode) {
  if (!_mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(g_itemLinkTagBegin.c_str())) ||
      !_mbsstr(reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(g_itemLinkTagEnd.c_str()))) {
    this->UIListBox::AddLine(text);
    return;
  }

  std::vector<std::string> wrapped;
  const std::string converted = this->Convert(text);
  this->WrapLinkText(&mode, &wrapped, converted);
  this->RewrapLinkLines(wrapped, mode);
  std::string source = text;
  for (unsigned int line = 0; line < wrapped.size(); ++line) {
    wrapped[line] = this->RenderItemLinks(wrapped[line].c_str(), source);
    if (line != 0) {
      this->unknown_00E4.push_back(color);
      this->history.push_back(std::string());
    }
    this->UIListBox::AddLine(wrapped[line].c_str());
  }
}

// OFFSET: 0x0089E0F0
// FUNCTION: fn_11599
void UIChatHisBox::AddNavigationLinkLine(const char* text, int& color, int& mode) {
  if (!g_navigationLinkParser->HasLink(text)) {
    this->UIListBox::AddLine(text);
    return;
  }

  std::vector<std::string> wrapped;
  std::string display;
  DisplayNavigationLink(display, text);
  this->WrapLinkText(&mode, &wrapped, display);
  this->RewrapLinkLines(wrapped, mode);
  std::string source = text;
  for (unsigned int line = 0; line < wrapped.size(); ++line) {
    wrapped[line] = this->RenderNavigationLinks(wrapped[line].c_str(), source);
    if (line != 0) {
      this->unknown_00E4.push_back(color);
    }
    this->UIListBox::AddLine(wrapped[line].c_str());
  }
}

// OFFSET: 0x0086DBD0
// FUNCTION: fn_11057
UIListBoxID::UIListBoxID()
    : UIWindow(0), lines(), selected(0), topLine(0), horizontalOffset(0), contentWidth(0), verticalOverflow(0),
      bottomMargin(0), verticalBar(nullptr), horizontalBar(nullptr), transparent(0), lineHeight(16) {
  int* background = reinterpret_cast<int*>(this->unknown_007C);
  background[0] = 255;
  background[1] = 255;
  background[2] = 255;
  const SkinPaletteCell cell = {3, 2};
  this->selectionColor = SkinPaletteColor(ResourceMgr::Instance(), cell, g_skinInfo.Selected());
}

// OFFSET: 0x0087C2A0
// FUNCTION: fn_11252
void UIString::SetText(int width, int height, std::string text) {
  this->SetSize(width, height);
  if (text.size() == 1 && text[0] == '\n') {
    this->Resize(5, 5);
    this->unknown_00DC = text;
    return;
  }

  this->ParseTags(text);
  if (this->unknown_00AC.size() != 0) {
    this->Resize(this->TextWidth(this->unknown_00DC.c_str(), static_cast<unsigned>(this->unknown_00DC.size()), 0, 12, 0, 0) + 24, 24);
  } else {
    this->Resize(
      this->TextWidth(this->unknown_00DC.c_str(), static_cast<unsigned>(this->unknown_00DC.size()), 0, 12, 0, 0),
      this->TextHeight(this->unknown_00DC.c_str(), static_cast<unsigned>(this->unknown_00DC.size()), 0, 12, 0, 0)
    );
  }
}


