#include "Render/DeviceContext/DeviceContext.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Text/TextMarkup.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIString.h"
#include "UI/Achievement/UIAchWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"

#include <algorithm>
#include <new>

// OFFSET: 0x0086F0A0
// FUNCTION: fn_11086
UITextViewer::UITextViewer() : UIListBox(), splitTipLinks(0), outline(0), bold(0) {
  this->lineHeight = 18;
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(18, 2, red, green, blue);
  const COLORREF rgb = RGB(red, green, blue);
  this->SetBackgroundColor(0xFF000000u | rgb);

  this->plainTextMode = 0;
}

UITextViewer::~UITextViewer() = default;

// OFFSET: 0x008872A0
// FUNCTION: fn_11334
void UITextViewer::OnCreate(int width, int height) {
  UIListBox::OnCreate(width, height);
}

// OFFSET: 0x0087B730
// FUNCTION: fn_11248
void UITextViewEX::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
  this->lineHeight = 21;
}

// OFFSET: 0x0087A1E0
// FUNCTION: fn_11228
void UITextViewEX::Clear() {
  for (auto linkIt = this->extraLinks.begin(), linkEnd = this->extraLinks.end(); linkIt != linkEnd; ++linkIt) {
    UIWindow* link = *linkIt;
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
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
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

  for (auto it = this->links.begin(); it != this->links.end(); ++it) {
    this->RemoveChild(*it, 1);
  }

  this->links.clear();
  this->Invalidate();
}

// OFFSET: 0x008909F0
// FUNCTION: fn_11429
void UITextViewer::OnLBtnDown(int, int) {
}

// OFFSET: 0x007B78C0
// FUNCTION: fn_09227
void UITextViewer::OnLBtnDblClk(int, int) {
}

// OFFSET: 0x007B41D0
// FUNCTION: fn_46563
inline bool UIAchListWnd::HitTestRow(int x, int y, int* row) {
  if (x < 0 || this->geometry.width - this->scrollBarWidth < x) {
    return false;
  }

  const int index = y / this->rowHeight;
  if (this->achievementList->size() <= static_cast<unsigned>(index)) {
    *row = 0;
    return false;
  }

  *row = index;
  return true;
}

// OFFSET: 0x007B78D0
// FUNCTION: fn_09228
void UIAchListWnd::OnLBtnDown(int x, int y) {
  int row;
  if (!this->HitTestRow(x, y, &row)) {
    return;
  }

  this->SelectAchievement(this->firstRow + row);
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x007B7920
// FUNCTION: fn_09229
void UIAchTV::OnLBtnDown(int, int) {
  if (this->selectionDisabled == 1) {
    return;
  }

  const int id = this->geometry.id;
  if (id != -1 && this->parent) {
    this->parent->SendMsg(nullptr, UM_SELECT_ACHIEVEMENT, id, 0, 0, 0);
  }
}

// OFFSET: 0x007B3850
// FUNCTION: fn_46564
inline void UIAchTab::NotifySelection(int group, int sub) {
  this->selectedGroup = group;
  this->selectedSub = sub;
  if (this->parent) {
    this->parent->SendMsg(this, UM_TAB, group, sub, 0, 0);
    this->parent->Invalidate();
  }

  this->UpdateScrollBar();
  this->Invalidate();
}

// OFFSET: 0x007B7960
// FUNCTION: fn_09230
void UIAchTab::OnLBtnDown(int x, int y) {
  int a = 0;
  int b = 0;
  if (!this->HitTestTab(x, y, &a, &b)) {
    return;
  }

  this->NotifySelection(a, b);
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x007B79F0
// FUNCTION: fn_09231
void UIAchListWnd::OnLBtnUp(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x007B7A00
// FUNCTION: fn_09232
void UIAchTab::OnLBtnUp(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x007B7A10
// FUNCTION: fn_09233
void UITextViewer::OnLBtnUp(int, int) {
}

// OFFSET: 0x0088FBB0
// FUNCTION: fn_11394
void UITextViewer::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(this->backgroundColor);
  }

  const int first = this->topLine;
  int divisor = this->lineHeight - this->bottomMargin;
  if (divisor == 0) {
    divisor = 1;
  }

  std::uint32_t color = 0;
  const int limit = this->geometry.height / divisor + first;
  const int end = static_cast<int>(std::min(static_cast<unsigned>(limit), static_cast<unsigned>(this->lines.size())));

  int index;
  for (index = 0; index < first; ++index) {
    const char* current = this->lines[index].c_str();
    const char* last = current + this->lines[index].size();

    while (current < last) {
      current = ParseColorCode(SegmentEnd(current, last), color);
    }
  }

  if (this->plainTextMode == 0) {
    for (index = first; index < end; ++index) {
      if (this->outline != 0) {
        this->DrawOutlinedColoredText(3, (index - first) * this->lineHeight + 4, this->lines[index].c_str(), static_cast<unsigned>(this->lines[index].size()), color, this->backgroundColor, 0, 12, this->bold);
      } else {
        this->DrawColoredText(3, (index - first) * this->lineHeight + 4, this->lines[index].c_str(), static_cast<unsigned>(this->lines[index].size()), color, 0, 12, false);
      }
    }
  } else {
    for (index = first; index < end; ++index) {
      this->DrawOutlinedText(3, (index - first) * this->lineHeight + 4, this->lines[index].c_str(), static_cast<unsigned>(this->lines[index].size()), 0, 0xFFFFFF, 0, 12, 0, 0);
    }
  }
}

// OFFSET: 0x00887290
// FUNCTION: fn_11333
void UITextViewEX::OnCreate(int width, int height) {
  UIListBox::OnCreate(width, height);
}

// INLINED
static void SetScrollRange(UIScrollBar* bar, int maximum, int visible, int total) {
  bar->maximum = maximum;
  bar->visible = visible;
  bar->total = total;
  bar->Invalidate();
  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
  }
}

// OFFSET: 0x00896100
// FUNCTION: fn_11515
void UITextViewEX::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int excess = static_cast<int>(this->lineLinks.size()) - visible;
  const int overflow = excess >= 0 ? excess : 0;

  this->verticalOverflow = overflow != 0;
  if (overflow != 0) {
    SetScrollRange(this->verticalBar, this->bottomMargin + overflow, visible, static_cast<int>(this->lineLinks.size()));
  } else {
    SetScrollRange(this->verticalBar, 0, visible, static_cast<int>(this->lineLinks.size()));
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
  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.rodexComposeWnd)) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00874E30
// FUNCTION: fn_11196
void UIItemTagOnMegaPhone::AddTagButton(UIWindow* button) {
  if (UIWindow* window = static_cast<UIWindow*>(g_windowMgr.npcTextEditDialogWnd)) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00899BF0
// FUNCTION: fn_11561
int UITextViewEX::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_UPPUSH:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, -1, 0, 0, 0);
    }
    break;

  case UM_DOWNPUSH:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 1, 0, 0, 0);
    }
    break;

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->topLine;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->topLine -= this->geometry.height / this->lineHeight - 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->topLine += this->geometry.height / this->lineHeight - 1;
    } else {
      this->topLine += static_cast<int>(argument1);
    }

    if (this->topLine < 0) {
      this->topLine = 0;
    }

    if (static_cast<unsigned>(this->topLine) > static_cast<unsigned>(static_cast<int>(this->lineLinks.size()) - this->geometry.height / this->lineHeight + this->bottomMargin)) {
      this->topLine = static_cast<int>(this->lineLinks.size()) - this->geometry.height / this->lineHeight + this->bottomMargin;
    }

    this->verticalBar->position = this->topLine;
    this->verticalBar->Invalidate();
    this->Invalidate();

    if (this->transparent != 0 && this->parent) {
      this->parent->Invalidate();
    }

    return this->topLine - previous;
  }

  case UM_SETTEXT: {
    std::string text(reinterpret_cast<const char*>(argument1));
    for (auto linkIt = this->extraLinks.begin(), linkEnd = this->extraLinks.end(); linkIt != linkEnd; ++linkIt) {
      UIWindow* link = *linkIt;
      this->RemoveChild(link, 1);
    }
    this->extraLinks.clear();
    this->lineLinks.clear();
    this->SetText(text);
    this->Invalidate();
    break;
  }

  default:
    return UIListBox::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  return 0;
}

// OFFSET: 0x00874A40
// FUNCTION: fn_11193
std::string UIItemTagCtrl::AddSkillTag(int tagId, unsigned int color) {
  std::string label;
  const std::string title = g_itemTagOwner->Title(tagId, &label, 17);

  UIItemTagButton* button = this->CreateItemTagButton(title, label, tagId, nullptr, 0x7F00E4);
  if (button) {
    this->AddTagButton(button);

    std::string link = "00000T";
    const std::string number = this->ItemLinkNumberText(tagId);
    link += number;
    link += "%0&0'0(1)0";

    ItemTagRecord record;
    record.child = button;
    StringFormat(record.number, sizeof(record.number), _TRUNCATE, "%s", number.c_str());
    StringFormat(record.name, sizeof(record.name), _TRUNCATE, "%s", label.c_str());
    StringFormat(record.link, sizeof(record.link), _TRUNCATE, "%s", link.c_str());
    this->tags.push_back(record);

    return "<" + label + ">";
  }
  return std::string();
}
