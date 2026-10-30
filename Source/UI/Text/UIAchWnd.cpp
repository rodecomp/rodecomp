#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/UIAchWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <new>

namespace {
  // INLINED
  const BitmapImage* AchievementBitmap(const char* name) {
    return &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)))->image;
  }
}

// OFFSET: 0x007B2420
// FUNCTION: fn_09186
UIAchievementWnd::UIAchievementWnd(UIWindowContext& windows, UIFrameContext& frame, UIButtonContext& controls)
    : UIFrameWnd(windows, frame, 0), upper(nullptr), tab(nullptr), list(nullptr), group(0), sub(-1), m_controls(&controls) {
}

// OFFSET: 0x007B32B0

// OFFSET: 0x007B37F0
// FUNCTION: fn_09205
void UIAchListWnd::sub_7B37F0(int index) {
  this->selectedIndex = index;

  if (this->parent) {
    this->parent->SendMsg(this, 135, static_cast<unsigned>(index) < this->ids.size() ? this->ids[index] : -1, 0, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x007B38A0
// FUNCTION: fn_09206
void UIAchTV::Clear() {
  UITextViewer::Clear();
  this->underline = 0;
}

// OFFSET: 0x007B38C0
// FUNCTION: fn_09207
void UIAchTab::ClearGroups() {
  this->groups.clear();
  this->unknown_007C = 0;
  this->unknown_0080 = -1;
  this->Refresh();
}

// OFFSET: 0x007B3910
// FUNCTION: fn_09208
void UIAchListWnd::Create(int width, int height, int rows) {
  this->unknown_0084 = height / rows;
  this->rowCount = rows;

  this->scrollBar = new UIAchScrollBar(*this->m_context);
  this->scrollBar->Configure(width - this->unknown_0094, 0, 1, height);
  this->AddChild(this->scrollBar);

  this->buttons = new UIButton*[this->rowCount];
  for (int index = 0; index < this->rowCount; ++index) {
    this->buttons[index] = new UIButton(*this->m_context, *this->m_controls);
    this->buttons[index]->SetSize(30, 30);
    this->buttons[index]->Move(5, this->unknown_0084 * index + 5);
    this->buttons[index]->SetId(index);
    this->buttons[index]->m_drawLabel = 0;
    this->AddChild(this->buttons[index]);
  }

  this->Slot15(width, height);
  this->Resize(width, height);
}

// OFFSET: 0x007B3DA0
// FUNCTION: fn_09209
void UIAchTab::DrawGroup(int group) {
  UIButtonContext& controls = *this->m_controls;
  int y = this->groupHeight * group;
  std::string name = this->groups[group].name;
  int textY = (this->groupHeight - this->TextHeight(controls.fonts, controls.locale, name.c_str(), 0, 0, this->fontHeight, 0, 0)) / 2 + y;

  if (group > this->unknown_007C) {
    y += std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
    textY += std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
  }

  if (group == this->unknown_007C) {
    this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\tab_press.bmp"), false);
    this->DrawTextAt(controls.fonts, controls.locale, 11, textY, name.c_str(), 0, 0, this->fontHeight, this->selectedGroupColor, 1, 0);

    int index = 0;
    std::map<int, std::string>& subs = this->groups[group].subs;
    int row = 0;
    for (auto it = subs.begin(); it != subs.end(); ++it, ++index) {
      if (row >= this->visibleSubs) {
        break;
      }

      if (index >= this->scrollPos) {
        const int rowY = (group + 1) * this->groupHeight + this->subHeight * row;
        ++row;
        name = this->groups[group].subs[it->first];
        const int height = this->TextHeight(controls.fonts, controls.locale, name.c_str(), 0, 0, this->fontHeight, 0, 0);
        const int rowHeight = this->subHeight;
        std::uint32_t color;

        if (it->first == this->unknown_0080) {
          this->DrawBitmap(0, rowY, AchievementBitmap("유저인터페이스\\achievement_re\\tab_sub_press.bmp"), false);
          this->DrawBitmap(12, rowY + 7, AchievementBitmap("유저인터페이스\\achievement_re\\tab_subarrow.bmp"), true);
          color = this->selectedSubColor;
        } else {
          const char* bitmap = it->first == this->hoverSub ? "유저인터페이스\\achievement_re\\tab_sub_over.bmp" : "유저인터페이스\\achievement_re\\tab_sub_out.bmp";
          this->DrawBitmap(0, rowY, AchievementBitmap(bitmap), false);
          color = this->subColor;
        }

        this->DrawTextAt(controls.fonts, controls.locale, 26, (rowHeight - height) / 2 + rowY, name.c_str(), 0, 0, this->fontHeight, color, 0, 0);
      }
    }
  } else {
    const char* bitmap = group == this->hoverGroup ? "유저인터페이스\\achievement_re\\tab_over.bmp" : "유저인터페이스\\achievement_re\\tab_out.bmp";
    this->DrawBitmap(0, y, AchievementBitmap(bitmap), false);
    this->DrawTextAt(controls.fonts, controls.locale, 11, textY, name.c_str(), 0, 0, this->fontHeight, this->groupColor, 1, 0);
  }
}

// OFFSET: 0x007B4220
// FUNCTION: fn_09210
std::string UIAchTab::Label(int group, int sub) {
  std::string name = this->groups[group].name;
  if (sub == -1) {
    return name;
  }

  return FormatOwnedString("%s", this->groups[group].subs[sub].c_str());
}

// OFFSET: 0x007B4370
// FUNCTION: fn_09211
bool UIAchTab::sub_7B4370(int x, int y, int* a, int* b) {
  if (x < 0 || x > this->tabWidth || y < 0) {
    return false;
  }

  const int top = (this->unknown_007C + 1) * this->groupHeight;
  const int extent = std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
  if (y >= top) {
    if (y > top && y < extent + top) {
      *a = this->unknown_007C;
      *b = (y - top) / this->subHeight + this->scrollPos;
      return true;
    }

    y -= extent;
  }

  *a = y / this->groupHeight;
  *b = -1;
  return true;
}

// OFFSET: 0x007B4470
// FUNCTION: fn_09212
void UIAchievementWnd::CreateTab() {
  UIAchTab* tab = new (std::nothrow) UIAchTab(*this->m_context, *this->m_controls);
  this->tab = tab;
  tab->Slot15(0, 0);
  tab->tabWidth = 102;
  tab->groupHeight = 40;
  tab->subHeight = 26;
  tab->unknown_00AC = 0;
  tab->selectedGroupColor = 0x9C1810;
  tab->groupColor = 0x9C6B5A;
  tab->selectedSubColor = 0x9B6A59;
  tab->subColor = 0x9B6A59;
  tab->ClearGroups();

  tab->scrollBar = new UIAchScrollBar(*this->m_context);
  tab->scrollBar->Configure(tab->scrollBarX, 0, 1, tab->visibleSubs * tab->subHeight);
  tab->AddChild(tab->scrollBar);

  this->tab->Move(0, 63);
  this->AddChild(this->tab);

  this->tab->SetLabel(LookupMessage(2656), 0, -1);
  this->tab->SetLabel(LookupMessage(2657), 1, -1);
  this->tab->SetLabel(LookupMessage(2658), 1, 0);
  this->tab->SetLabel(LookupMessage(2659), 1, 1);
  this->tab->SetLabel(LookupMessage(2660), 1, 2);
  this->tab->SetLabel(LookupMessage(2661), 2, -1);
  this->tab->SetLabel(LookupMessage(2662), 2, 0);
  this->tab->SetLabel(LookupMessage(2663), 2, 1);
  this->tab->SetLabel(LookupMessage(2664), 2, 2);
  this->tab->SetLabel(LookupMessage(2665), 2, 3);
  this->tab->SetLabel(LookupMessage(2666), 2, 4);
  this->tab->SetLabel(LookupMessage(2667), 2, 5);
  this->tab->SetLabel(LookupMessage(2668), 3, -1);
  this->tab->SetLabel(LookupMessage(2669), 3, 0);
  this->tab->SetLabel(LookupMessage(2670), 3, 1);
  this->tab->SetLabel(LookupMessage(2671), 4, -1);
  this->tab->SetLabel(LookupMessage(2672), 4, 0);
  this->tab->SetLabel(LookupMessage(2673), 4, 1);
  this->tab->SetLabel(LookupMessage(2674), 5, -1);
  this->tab->SetLabel(LookupMessage(2675), 5, 0);
  this->tab->SetLabel(LookupMessage(2676), 5, 1);
  this->tab->SetLabel(LookupMessage(2677), 5, 2);
  this->tab->SetLabel(LookupMessage(2678), 6, -1);
}

// OFFSET: 0x007B65F0
// FUNCTION: fn_09218
void UIAchScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  const BitmapImage* middle = AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_bg_mid.bmp");
  for (int index = 0; index < this->geometry.height / this->thickness; ++index) {
    this->DrawBitmap(0, this->thickness * index, middle, false);
  }

  this->DrawBitmap(0, this->geometry.height - this->thickness, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_bg_down.bmp"), false);
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_arrow_up.bmp"), true);
  this->DrawBitmap(0, this->geometry.height - this->arrowLength, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_arrow_down.bmp"), true);

  int y = this->ThumbOffset() + this->arrowLength;
  this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_up.bmp"), true);

  const int end = this->ThumbLength() - 4 + y;
  while ((y += 4) < end) {
    this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_mid.bmp"), true);
  }

  this->DrawBitmap(0, end, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_down.bmp"), true);
}

// OFFSET: 0x007B6DB0
// FUNCTION: fn_09220
void UIAchTV::Paint() {
  if (this->transparent == 1) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(this->backgroundColor);
  }

  const int rows = this->lineHeight - this->bottomMargin == 0 ? this->geometry.height : this->geometry.height / this->lineHeight - this->bottomMargin;
  const unsigned end = std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(rows + this->topLine));
  UIListBoxContext& owner = *this->m_listContext;

  int row = 0;
  for (int index = this->topLine; index < static_cast<int>(end); ++index, ++row) {
    const std::string& line = this->lines[index];
    if (this->outlineColor == 0) {
      this->DrawTextAt(owner.fonts, owner.locale, 3, this->lineHeight * row + 4, line.c_str(), line.size(), 0, 12, this->textColor, 0, 0);
    } else {
      DrawOutlinedText(*this, 3, this->lineHeight * row + 4, line.c_str(), line.size(), this->textColor, this->outlineColor, 0, 12, 0, 0);
    }

    if (this->underline == 1) {
      const std::uint32_t color = this->textColor;
      const int width = this->TextWidth(owner.fonts, owner.locale, this->lines[index].c_str(), 0, 0, 12, 0, 0);
      this->FillRegion(2, this->lineHeight * row + 10, width + 1, 1, color);
    }
  }
}

// OFFSET: 0x007B6F60
// FUNCTION: fn_09221
void UIAchTab::Paint() {
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_tab.bmp"), false);

  if (this->parent) {
    this->parent->SendMsg(this, 116, 0, 0, 0, 0);
  }

  for (auto it = this->groups.begin(); it != this->groups.end(); ++it) {
    this->DrawGroup(it->first);
  }
}

// OFFSET: 0x007B7650
// FUNCTION: fn_09224
void UIAchievementWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2646), 0);
}

// OFFSET: 0x007B7680
// FUNCTION: fn_09225
void UICustomGageBar::Paint() {
  if (this->value < 0 || this->maximum < 0 || this->value > this->maximum) {
    return;
  }

  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, this->outerBorderColor);
  DrawRectangleBorder(*this, 1, 1, this->geometry.width - 2, this->geometry.height - 2, this->innerBorderColor);
  this->FillRegion(2, 2, this->geometry.width - 4, this->geometry.height - 4, this->backgroundColor);

  const float percent = static_cast<float>(this->value) * 100.0f / static_cast<float>(this->maximum);
  if (percent > 0.0f) {
    this->FillRegion(2, 2, static_cast<int>(percent) * this->geometry.width / 100, this->geometry.height - 4, this->barColor);
  }
}

// OFFSET: 0x007B7B60
// FUNCTION: fn_09236
void UIAchTab::Slot27(int x, int y) {
  int sub = 0;
  int group = 0;
  this->sub_7B4370(x, y, &group, &sub);
  this->hoverGroup = group;
  this->hoverSub = sub;
  this->Invalidate();
}

// OFFSET: 0x007B7D90
// FUNCTION: fn_09239
void UIAchListWnd::Slot35(int delta) {
  if (!this->scrollable) {
    return;
  }

  if (delta < 0) {
    this->SendMsg(this->scrollBar, 7, 3, 0, 0, 0);
    return;
  }

  this->SendMsg(this->scrollBar, 7, -3, 0, 0, 0);
}

// OFFSET: 0x007B7DD0
// FUNCTION: fn_09240
void UIAchTab::Slot35(int delta) {
  if (!this->scrollable) {
    return;
  }

  if (delta < 0) {
    this->SendMsg(this->scrollBar, 7, 1, 0, 0, 0);
    return;
  }

  this->SendMsg(this->scrollBar, 7, -1, 0, 0, 0);
}

// OFFSET: 0x007B7E10
// FUNCTION: fn_09241
void UIAchListWnd::UpdateScroll() {
  UIAchListInfo* info = this->unknown_00AC;
  if (!info || !this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int overflow = std::max(info->count - rows, 0);
  this->scrollable = overflow > 0;

  if (overflow < 1) {
    this->unknown_008C = 0;
    this->scrollBar->SetRange(0, rows, info->count);
  } else {
    this->scrollBar->SetRange(overflow, rows, info->count);
    if (this->unknown_008C < 0) {
      this->unknown_008C = 0;
      this->scrollBar->SetPosition(0);
    } else {
      if (this->unknown_008C > overflow) {
        this->unknown_008C = overflow;
      }
      this->scrollBar->SetPosition(this->unknown_008C);
    }
  }

  this->scrollBar->Invalidate();

  for (int index = 0; index < this->rowCount; ++index) {
    UIButton* button = this->buttons[index];
    if (static_cast<unsigned>(this->unknown_008C + index) < static_cast<unsigned>(this->unknown_00AC->count)) {
      button->SetDisabled(0);
    } else {
      button->SetDisabled(1);
    }
    button->Invalidate();
  }
}

// OFFSET: 0x007B7F10
// FUNCTION: fn_09242
void UIAchTab::sub_7B7F10() {
  if (!this->scrollBar) {
    return;
  }

  const int count = static_cast<int>(this->groups[this->unknown_007C].subs.size());
  const int visible = this->visibleSubs;
  const int overflow = std::max(count - visible, 0);
  this->scrollable = overflow > 0;

  if (overflow < 1) {
    this->scrollPos = 0;
    this->scrollBar->SetRange(0, visible, count);
  } else {
    this->scrollBar->SetRange(overflow, visible, count);
    if (this->scrollPos < 0) {
      this->scrollPos = 0;
      this->scrollBar->SetPosition(0);
    } else {
      if (this->scrollPos > overflow) {
        this->scrollPos = overflow;
      }
      this->scrollBar->SetPosition(this->scrollPos);
    }
  }

  this->scrollBar->Move(0, (this->unknown_007C + 1) * this->groupHeight);
  this->scrollBar->Invalidate();
}

// OFFSET: 0x007B7FE0
// FUNCTION: fn_09243
void UIAchTab::Refresh() {
  int height = static_cast<int>(this->groups.size()) * this->groupHeight;
  for (auto it = this->groups.begin(); it != this->groups.end(); ++it) {
    height += static_cast<int>(this->groups[it->first].subs.size()) * this->subHeight;
  }

  this->Resize(this->tabWidth, height + 1);
  this->Invalidate();
}

// OFFSET: 0x007B81B0
// FUNCTION: fn_09246
int UIAchTab::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message != 7 && message != 9 && message != 10) {
    return 0;
  }

  const int previous = this->scrollPos;
  if (message == 9) {
    this->scrollPos = previous - this->visibleSubs + 1;
  } else {
    if (message == 10) {
      argument1 = this->visibleSubs - 1;
    }
    this->scrollPos = previous + argument1;
  }

  if (previous != this->scrollPos) {
    this->sub_7B7F10();
    this->Invalidate();
  }

  return this->scrollPos - previous;
}

// OFFSET: 0x007B8230
// FUNCTION: fn_09247
int UIAchUpperWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && argument1 == 215) {
    for (int index = 0; index < 2; ++index) {
      this->buttons[index]->Invalidate();
      if (sender == this->buttons[index]) {
        this->buttons[index]->SendMsg(this, 13, 1, 0, 0, 0);
        this->selected = index;
        if (this->parent) {
          this->parent->SendMsg(this, 22, -1, -1, 0, 0);
        }
      } else {
        this->buttons[index]->SendMsg(this, 13, 0, 0, 0, 0);
      }
    }
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x007B8C20
// FUNCTION: fn_09251
void UIToolTipBitmapWnd::Set(const char* name, const char* text, int item) {
  if (text) {
    this->tooltip = std::string(text);
  }

  this->bitmapName = name;
  this->bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (this->bitmap) {
    this->Resize(this->bitmap->image.width, this->bitmap->image.height);
  }

  this->itemId = item;
}

// OFFSET: 0x007B8DC0
// FUNCTION: fn_09252
void UIAchSummaryWnd::Show(int visible) {
  this->geometry.visible = visible;
  for (UIWindow* button : this->buttons) {
    button->Slot27(-1, -1);
  }
}

// OFFSET: 0x007B8FA0
// FUNCTION: fn_09254
void UIAchTab::SetLabel(const char* text, int group, int sub) {
  if (sub == -1) {
    this->groups[group].name = text;
  } else {
    this->groups[group].subs[sub] = text;
  }

  this->unknown_007C = 0;
  this->unknown_0080 = -1;
  this->Refresh();
}
