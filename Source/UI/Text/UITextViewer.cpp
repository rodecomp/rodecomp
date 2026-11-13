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
#include "UI/Achievement/UIAchWnd.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <new>

template __declspec(noinline) std::string::basic_string(const std::string&);

// OFFSET: 0x0086F0A0
// FUNCTION: fn_11086
UITextViewer::UITextViewer() : UIListBox(), splitTipLinks(0), outline(0), bold(0) {
  this->lineHeight = 18;
  unsigned red;
  unsigned green;
  unsigned blue;
  g_skinInfo.PaletteChannels(18, 2, red, green, blue);
  this->SetBackgroundColor(0xFF000000u | RGB(red, green, blue));

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

// OFFSET: 0x007B78D0
// FUNCTION: fn_09228
void UIAchListWnd::OnLBtnDown(int x, int y) {
  if (x < 0 || this->geometry.width - this->scrollBarWidth < x) {
    return;
  }

  const int row = y / this->rowHeight;
  if (static_cast<unsigned>(this->achievementList->count) <= static_cast<unsigned>(row)) {
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

// OFFSET: 0x007B7960
// FUNCTION: fn_09230
void UIAchTab::OnLBtnDown(int x, int y) {
  int a = 0;
  int b = 0;
  if (!this->HitTestTab(x, y, &a, &b)) {
    return;
  }

  this->selectedGroup = a;
  this->selectedSub = b;
  if (this->parent) {
    this->parent->SendMsg(this, UM_TAB, a, b, 0, 0);
    this->parent->Invalidate();
  }

  this->UpdateScrollBar();
  this->Invalidate();
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
  int divisor = (this->lineHeight - this->bottomMargin);

  if (divisor == 0) {
    divisor = 1;
  }

  std::uint32_t color = 0;
  const int limit = (this->geometry.height / divisor + first);
  const int end = static_cast<int>(std::min(static_cast<unsigned>(limit), static_cast<unsigned>(this->lines.size())));

  for (int index = 0; index < first; ++index) {
    const char* current = this->lines[index].c_str();
    const char* end = current + this->lines[index].size();

    while (current < end) {
      current = ParseColorCode(SegmentEnd(current, end), color);
    }
  }

  const bool plain = (this->plainTextMode & 255) != 0;

  for (int index = first; index < end; ++index) {
    const auto& line = this->lines[index];
    const int y = (index - first) * this->lineHeight + 4;
    const auto bytes = static_cast<unsigned>(line.size());

    if (plain) {
      this->DrawOutlinedText(3, y, line.c_str(), bytes, 0, 0xFFFFFF, 0, 12, 0, 0);
    } else if (this->outline != 0) {
      this->DrawOutlinedColoredText(3, y, line.c_str(), bytes, color, this->backgroundColor, 0, 12, this->bold);
    } else {
      this->DrawColoredText(3, y, line.c_str(), bytes, color, 0, 12, false);
    }
  }
}

// OFFSET: 0x00887290
// FUNCTION: fn_11333
void UITextViewEX::OnCreate(int width, int height) {
  UIListBox::OnCreate(width, height);
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
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.rodexComposeWnd)) {
    window->AddChild(button);
  }
}

// OFFSET: 0x00874E30
// FUNCTION: fn_11196
void UIItemTagOnMegaPhone::AddTagButton(UIWindow* button) {
  if (UIWindow* window = reinterpret_cast<UIWindow*>(g_windowMgr.npcTextEditDialogWnd)) {
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
    return 0;

  case UM_DOWNPUSH:
    if (this->verticalOverflow != 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 1, 0, 0, 0);
    }
    return 0;

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->topLine;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->topLine = previous - this->geometry.height / this->lineHeight + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->topLine = previous + this->geometry.height / this->lineHeight - 1;
    } else {
      this->topLine = previous + static_cast<int>(argument1);
    }

    if (this->topLine < 0) {
      this->topLine = 0;
    }

    const int maximum = this->bottomMargin + (static_cast<int>(this->lineLinks.size()) - this->geometry.height / this->lineHeight);
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

  case UM_SETTEXT: {
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

// OFFSET: 0x00874A40
// FUNCTION: fn_11193
std::string UIItemTagCtrl::AddSkillTag(int tagId, unsigned int color) {
  std::string label;
  const std::string title = g_itemTagOwner->Title(tagId, &label, 17);

  UIItemTagButton* button = this->CreateItemTagButton(title, label, tagId, nullptr, 0x7F00E4);
  if (!button) {
    return std::string();
  }

  this->AddTagButton(button);

  std::string link = "00000T";
  const std::string number = reinterpret_cast<UITextViewer*>(this)->ItemLinkNumberText(tagId);
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
