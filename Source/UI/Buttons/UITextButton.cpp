#include "Render/DeviceContext/DeviceContext.h"
#include "UI/UIBarGraph.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Buttons/UIItemMoveInfo.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIImgMenu.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Lists/UITreeView.h"
#include "UI/UIStaticItem.h"
#include "Client/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/UIBasicCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIMemberListBox.h"
#include "Client/Mode/ModeCursor.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include "UI/Buttons/UISlotBmpButton.h"
#include "UI/Buttons/UITVBitmapButton.h"
#include "UI/Buttons/UITVTextButton.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxV2Wnd.h"
#include "UI/Lists/UIItemStoreComboBox.h"
#include "UI/Lists/UIPackageBoxNameList.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "UI/Lists/TableBox.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Text/UIString.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "Client/Input/Language.h"
#include "Utility/Text/GroupedInteger.h"
#include "Client/Replay.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIListBoxNavi.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/WindowBorder.h"
#include "Resource/SkinPalette.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// GLOBAL: gl_24606
int g_screenHeight;

// OFFSET: 0x0086EEB0
// FUNCTION: fn_11085
UITextButton::UITextButton(UIWindowContext& windows, UIButtonContext& buttons) : UIButton(windows, buttons), m_outlineColor(0xFFFFFF), m_value(0) {
}

// OFFSET: 0x00874690
// FUNCTION: fn_11110
UITextButton::~UITextButton() = default;

// OFFSET: 0x0087C590
// FUNCTION: fn_11254
void UITextButton::Configure(const char* text, std::uint32_t color, int family, int height, unsigned char outline, unsigned char underline) {
  this->label = text;
  this->m_text = this->label;
  this->m_normalColor = this->m_hoverColor = this->m_color = color;
  this->m_family = family;
  this->m_height = height;
  this->m_outline = outline;
  this->m_underline = underline;
  const auto extent = static_cast<std::uint32_t>(height) * static_cast<std::uint32_t>(std::strlen(text));
  const int width = static_cast<int>(extent / 2 + 4u * outline + 12u);
  this->Resize(width, height + 2);
  this->Slot15(width, height + 2);
  this->Slot22(width, height + 2);
}

// OFFSET: 0x0088F820
// FUNCTION: fn_11392
void UITextButton::Paint() {
  if (this->geometry.surface) {
    this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  }

  if (this->parent) {
    this->parent->Invalidate();
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(this->m_family, this->m_height, this->m_outline != 0, 0);
  const int bytes = static_cast<int>(std::strlen(this->m_text.c_str()));

  if (this->m_outline != 0) {
    if (this->HasTextBackground()) {
      context.SetBackground(this->TextBackground(), OPAQUE);
    }

    context.SetColor(this->m_outlineColor);
    context.Draw(0, 1, this->m_text.c_str(), bytes);
    context.Draw(2, 1, this->m_text.c_str(), bytes);
    context.Draw(1, 0, this->m_text.c_str(), bytes);
    context.Draw(1, 2, this->m_text.c_str(), bytes);
  }

  context.SetColor(this->m_color);
  context.Draw(1, 1, this->m_text.c_str(), bytes);

  if (this->m_outline != 0 && this->HasTextBackground()) {
    context.SetBackground(this->TextBackground(), TRANSPARENT);
  }

  if (this->m_underline != 0) {
    this->FillRegion(0, (this->geometry.height - 1), this->geometry.width, 1, this->m_color);
  }
}

// OFFSET: 0x00892450
// FUNCTION: fn_11471
void UITextButton::Slot27(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  const bool inside = x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height;
  const unsigned state = inside ? (g_windowMgr.Capture() == this ? 2 : 1) : 0;
  this->m_color = inside ? this->m_hoverColor : this->m_normalColor;

  if (this->geometry.unknown_0030 != state) {
    this->geometry.unknown_0030 = state;
    this->Invalidate();
  }
}

// OFFSET: 0x00892000
// FUNCTION: fn_11461
void UITextButton::Slot28(int x, int y) {
  if (this->parent) {
    this->parent->Slot28((x + this->geometry.x), (y + this->geometry.y));
  }

  UIButton::Slot28(x, y);
}

// OFFSET: 0x00891140
// FUNCTION: fn_11445
void UITextButton::Slot31(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 98, this->geometry.id, this->m_value, 0, 0);
      g_windowMgr.ReleaseCapture();
      return;
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892B60
// FUNCTION: fn_11487
void UITextButton::Slot32(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, 99, this->geometry.id, this->m_value, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892D60
// FUNCTION: fn_11494
void UITextButton::Slot33(int x, int y) {

  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 99, this->geometry.id, this->m_value, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892F30
// FUNCTION: fn_11500
void UITextButton::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x008911D0
// FUNCTION: fn_11446
void UITreeView::Slot31(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    this->unknown_0088 = 0;
  }

  this->unknown_008C = 0;
  this->unknown_0090 = 0;
  this->unknown_00AC = 0;
  this->Invalidate();

  if (this->parent) {
    this->parent->Slot31(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00891A80
// FUNCTION: fn_11455
void UIItemTagButton::Slot28(int x, int y) {
  std::string tag = "<";
  tag += this->m_tooltip;
  tag += ">";

  if (this->label != tag) {
    UIButton::Slot28(x, y);
  }
}

// OFFSET: 0x00891F10
// FUNCTION: fn_11459
void UIStaticItem_Illust::Slot28(int, int) {
}

// OFFSET: 0x00891810
// FUNCTION: fn_11450
void UIBmpRadioBtn::Slot28(int, int) {
  if (!this->m_tooltip.empty()) {
    int screenX = 0, screenY = -20;

    for (const UIWindow* window = this; window; window = window->parent) {
      screenX = (screenX + window->geometry.x);
      screenY = (screenY + window->geometry.y);
    }

    this->m_controls->services.Tooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x0089B640
// FUNCTION: fn_11570
bool UIBmpRadioBtn::SetBitmap(const char* name, int state) {
  const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (!bitmap) {
    return false;
  }

  if (bitmap->image.width > this->geometry.width) {
    this->geometry.width = bitmap->image.width;
  }

  if (bitmap->image.height > this->geometry.height) {
    this->geometry.height = bitmap->image.height;
  }

  this->Resize(this->geometry.width, this->geometry.height);

  switch (state) {
  case 0:
    this->m_names[0] = name;
    break;
  case 1:
    this->m_names[1] = name;
    break;
  case 2:
    this->m_names[2] = name;
    break;
  case 3:
    this->m_names[3] = name;
    break;
  }

  return true;
}

// OFFSET: 0x0089BA00
// FUNCTION: fn_11573
void UITextButton::CenterIn(int width, int height) {
  UIWindow::Move(
    static_cast<int>((width - this->geometry.width) * 0.5 + this->geometry.x), static_cast<int>((height - this->geometry.height) * 0.5 + this->geometry.y)
  );
}

// OFFSET: 0x0089CA70
// FUNCTION: fn_11590
void UITVTextButton::SetLabel(std::string text) {
  this->label = text;
  this->ResizeToText();
}

// OFFSET: 0x0089CC30
// FUNCTION: fn_11592
void UIBmpRadioBtn::AddText(int id, int height, std::uint32_t color, bool bold, bool unknown, int x, int y) {
  UIBmpRadioBtnText text;
  text.id = id;
  text.x = x;
  text.y = y;
  text.height = height;
  text.bold = bold;
  text.color = color;
  text.unknown_0018 = unknown;

  if (x == -1000) {
    const int textWidth =
      this->TextWidth(this->m_controls->fonts, this->m_controls->locale, this->m_label.c_str(), static_cast<unsigned>(this->m_label.size()), 0, height, bold, 0);
    text.x = static_cast<int>((this->geometry.width - textWidth) * 0.5);
  }

  if (y == -1000) {
    const int textHeight =
      this->TextHeight(this->m_controls->fonts, this->m_controls->locale, this->m_label.c_str(), static_cast<unsigned>(this->m_label.size()), 0, height, bold, 0);
    text.y = static_cast<int>((this->geometry.height - textHeight) * 0.5);
  }

  auto it = this->m_texts.begin();
  for (; it != this->m_texts.end(); ++it) {
    if (it->id == id) {
      break;
    }
  }

  if (it != this->m_texts.end()) {
    *it = text;
  }

  this->m_texts.push_back(text);
}

// OFFSET: 0x007C7CA0
// FUNCTION: fn_09408
void UIBmpRadioBtn2::Slot47() {
  this->m_hovered = true;
  this->Invalidate();
}

// OFFSET: 0x007C7CB0
// FUNCTION: fn_09409
void UIBmpRadioBtn2::Slot48() {
  this->m_hovered = false;
  this->Invalidate();
}

// OFFSET: 0x00891870
// FUNCTION: fn_11451
void UICaptchaImage::Slot28(int, int) {
  if (!this->m_tooltip.empty()) {
    int screenX = 0, screenY = -20;

    for (const UIWindow* window = this; window; window = window->parent) {
      screenX = (screenX + window->geometry.x);
      screenY = (screenY + window->geometry.y);
    }

    this->m_controls->services.Tooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x00891240
// FUNCTION: fn_11447
void UIBarGraph4::Slot28(int, int) {
  if (!this->m_current || !this->m_maximum || *this->m_maximum < *this->m_current || *this->m_maximum == 0) {
    return;
  }

  int screenX = 0, screenY = -16;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  char text[260];
  std::memset(text, 0, sizeof(text));
  const int permille = static_cast<int>(static_cast<float>(*this->m_current) / static_cast<float>(*this->m_maximum) * 1000.0f);
  std::sprintf(text, "%.1f %%", static_cast<float>(permille) * 0.1f);
  this->m_controls->services.Tooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x008918D0
// FUNCTION: fn_11452
void UIINT64BarGraph::Slot28(int, int) {
  if (this->m_maximum < this->m_current || this->m_maximum == 0 || this->m_maximum == 99999999) {
    return;
  }

  int screenX = 0, screenY = -16;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  char text[260];
  std::memset(text, 0, sizeof(text));
  const int permille = static_cast<int>(static_cast<double>(this->m_current) / static_cast<double>(this->m_maximum) * 1000.0);
  std::sprintf(text, "%.1f %%", static_cast<float>(permille) * 0.1f);
  this->m_controls->services.Tooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x00891A40
// FUNCTION: fn_11454
void UIItemMoveInfo::Slot28(int, int) {
  this->m_windows->EnsureWindow(276)->SendMsg(nullptr, 147, this->m_moveIndex, 0, 0, 0);
}

// OFFSET: 0x00891A10
// FUNCTION: fn_11453
void UIImgMenu::Slot28(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->ItemAt(x, y);
  if (previous != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x00891F20
// FUNCTION: fn_11460
void UITabControl::Slot28(int x, int y) {
  int index = this->TabAt(x, y);

  if (index == -1) {
    return;
  }

  const std::string& tooltip = this->tooltips[index];

  if (tooltip.empty() || this->labels[index] == tooltip) {
    return;
  }

  int screenX = 0;

  for (int i = 0; i < index; ++i) {
    screenX = (screenX - 1 + this->widths[i]);
  }

  int screenY = -20;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  this->m_controls->services.Tooltip(this->tooltips[index].c_str(), screenX, screenY, -1, false, true);
}

// OFFSET: 0x00822660
// FUNCTION: fn_10280
std::string SkillUseTextPercent(const char* name, int level, int sp, int ap) {
  if (ap == 0) {
    return FormatOwnedString("%s Use Lv %d (Sp : %d%%)", name, level, sp);
  }

  return FormatOwnedString("%s Use Lv %d (Sp : %d%%, Ap : %d)", name, level, sp, ap);
}

// OFFSET: 0x008226F0
// FUNCTION: fn_10281
std::string SkillUseTextFlat(const char* name, int level, int sp, int ap) {
  if (ap == 0) {
    return FormatOwnedString("%s Use Lv %d (Sp : %d)", name, level, sp);
  }

  return FormatOwnedString("%s Use Lv %d (Sp : %d, Ap : %d)", name, level, sp, ap);
}

// OFFSET: 0x00891350
// FUNCTION: fn_11448
void UIBasicItemCtrl::Slot28(int, int) {
  std::string text;

  if (std::atoi(this->m_item.itemId.c_str()) < 1) {
    text = this->m_tooltip;
  } else if (this->m_item.unknown_0000[1] == 0) {
    text = this->m_naming->DisplayName(this->m_item, 0);
  } else if (this->m_item.unknown_0098 != 0) {
    text = FormatOwnedString(LookupMessage(2551), this->m_naming->DisplayName(this->m_item, 0).c_str(), this->m_item.unknown_0098, this->m_item.quantity);
  } else {
    text = FormatOwnedString(LookupMessage(183), this->m_naming->DisplayName(this->m_item, 0).c_str(), this->m_item.quantity);
  }

  int screenX = 0, screenY = 0;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  if (!text.empty()) {
    this->m_controls->services.Tooltip(text.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x008915E0
// FUNCTION: fn_11449
void UIBasicSkillCtrl::Slot28(int, int) {
  if (this->m_skillId < 0) {
    return;
  }

  int screenX = 0, screenY = 0;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  LuaScript& script = *g_session.m_hotkeyScript;
  CSkillInfo skill = this->m_skills->Lookup(static_cast<std::uint32_t>(this->m_skillId));
  const int sp = skill.UseSpAmount(script, skill.level);
  const int ap = skill.UseApAmount(script, skill.level);
  std::string text;

  if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
    text = SkillUseTextPercent(skill.DisplayName(script), skill.level, sp, ap);
  } else {
    text = SkillUseTextFlat(skill.DisplayName(script), skill.level, sp, ap);
  }

  if (!text.empty()) {
    this->m_controls->services.Tooltip(text.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x00A8B9D0
// FUNCTION: fn_17125
void UIWindowMgr::AddRendered(UIWindow* window) {
  this->rendered.push_back(window);
}

// OFFSET: 0x00891CC0
// FUNCTION: fn_11457
void UIListBox::Slot28(int x, int y) {
  UIColorTransBalloonText*& hover = *reinterpret_cast<UIColorTransBalloonText**>(reinterpret_cast<char*>(this) + 0xE0);
  const int index = this->LineAt(x, y);
  const int height = g_screenHeight;
  if (this->lines.begin() != this->lines.end()) {
    if (index != -1 && this->lines[index].size() != 0) {
      int screenY = 0;
      int screenX = 0;
      if (hover != nullptr) {
        g_windowMgr.Schedule(hover);
        hover = nullptr;
      }

      UIColorTransBalloonText* balloon = new UIColorTransBalloonText(*this->m_context, *reinterpret_cast<BalloonTextContext*>(this->m_listContext));
      hover = balloon;
      balloon->SetSize(8, 8);

      for (const UIWindow* window = this; window != nullptr; window = window->parent) {
        screenX += window->geometry.x;
        screenY += window->geometry.y;
      }

      balloon->Move(screenX + 0x50, height - 6);
      g_windowMgr.AddRendered(balloon);
      balloon->SetText(this->lines[index].c_str(), 50);
      return;
    }

    g_windowMgr.Schedule(hover);
    hover = nullptr;
  }
}

// OFFSET: 0x00891BC0
// FUNCTION: fn_11456
void UIMemberListBox::Slot28(int, int y) {
  const int line = this->topLine + y / this->lineHeight;
  if (line < this->topLine) {
    return;
  }

  unsigned visible = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + this->topLine);
  if (this->lines.size() < visible) {
    visible = static_cast<unsigned>(this->lines.size());
  }

  if (line >= static_cast<int>(visible)) {
    return;
  }

  const char* text = this->lines[line].c_str();
  const int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, text, 0, 0, 12, 0, 0);
  if (width <= this->geometry.width - this->verticalOverflow * 15) {
    return;
  }

  int screenX = -4;
  int screenY = y - y % this->lineHeight;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  this->m_controls->services.Tooltip(this->lines[line].c_str(), screenX, screenY, -1, false, true);
}

// OFFSET: 0x00891E40
// FUNCTION: fn_11458
void UIStaticItem::Slot28(int, int) {
  char text[260];
  std::memset(text, 0, sizeof(text));
  int capacity = sizeof(text);
  this->m_descriptions->Name(this->m_item, text, capacity, 0);

  if (std::strlen(text) == 0) {
    return;
  }

  int screenX = 0, screenY = -20;

  for (const UIWindow* window = this; window; window = window->parent) {
    screenX = (screenX + window->geometry.x);
    screenY = (screenY + window->geometry.y);
  }

  this->m_controls->services.Tooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x00892040
// FUNCTION: fn_11462
void UIItemMoveInfo::Slot48() {
  this->m_visibility->Show(276, 0);
  this->Invalidate();
}

// OFFSET: 0x0087B550
// FUNCTION: fn_11242
void UIComboBox::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x008974A0
// FUNCTION: fn_11528
void UIComboBox::Resize(int width, int height) {
  UIListBox::Resize(width, height);
  this->topLine = 0;
  this->verticalBar->position = this->topLine;
  if (this->verticalOverflow) {
    this->verticalBar->Move(width - 13, 0);
    this->verticalBar->Invalidate();
  } else {
    this->verticalBar->Move(width - 13, 0);
  }
}

// OFFSET: 0x00885240
// FUNCTION: fn_11317
void UIComboBox::Slot15(int width, int height) {
  UIListBox::Slot15(width, height);
}

// OFFSET: 0x00895AC0
// FUNCTION: fn_11509
void UIComboBox::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int overflow = std::max(static_cast<int>(this->lines.size()) - visible, 0);

  this->verticalOverflow = overflow != 0;
  UIScrollBar* bar = this->verticalBar;
  if (overflow != 0) {
    bar->maximum = this->bottomMargin + overflow;
    bar->total = static_cast<int>(this->lines.size());
    bar->visible = visible;
    bar->Invalidate();
    if (bar->position > bar->maximum) {
      bar->position = bar->maximum;
    }

    if (this->selected < this->topLine) {
      this->topLine = this->selected;
    } else if (this->topLine + visible <= this->selected) {
      this->topLine = this->selected - visible + 1;
    }

    if (this->topLine < 0) {
      this->topLine = 0;
    }

    if (this->topLine > overflow) {
      this->topLine = overflow;
    }

    this->verticalBar->position = this->topLine;
  } else {
    bar->maximum = 0;
    bar->total = static_cast<int>(this->lines.size());
    bar->visible = visible;
    bar->Invalidate();
    if (bar->position > bar->maximum) {
      bar->position = bar->maximum;
    }
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x00896FA0
// FUNCTION: fn_11523
void UITabControl::RecalculateWidths() {
  if (this->uniformWidths) {
    int widest = 0;

    for (int index = 0; index < static_cast<int>(this->labels.size()); ++index) {
      int width;

      if (this->layout == 0 || this->layout == 2) {
        width = this->VerticalTextHeight(this->m_controls->fonts, this->m_controls->locale, this->labels[index].c_str(), 0, 0, this->unknown_00E8, 0);
      } else {
        width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, this->labels[index].c_str(), 0, 0, this->unknown_00E8, 0, 0);
      }

      widest = std::max(width + this->tabPadding, widest);
    }

    for (int index = 0; index < static_cast<int>(this->labels.size()); ++index) {
      this->widths[index] = widest;
    }
  }

  this->FitToTabs();
}

// OFFSET: 0x00897370
// FUNCTION: fn_11526
void UIChatHisBox3::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Resize(9, height);
  this->verticalBar->Move(width - 10, 0);
  this->horizontalBar->Resize(width - 10, 9);
  this->horizontalBar->Move(0, height - 10);
  this->Refresh();
}

// OFFSET: 0x008978A0
// FUNCTION: fn_11536
void UITVTextButton::ResizeToText() {
  if (std::strcmp(this->label.c_str(), "") == 0) {
    return;
  }

  this->unknown_00DC = this->label;
  const int width =
    this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, this->label.c_str(), 0, this->m_fontFamily, this->m_fontHeight, this->m_outlined, 0);
  const int border = this->m_border ? 2 : 0;
  UIWindow::Resize(width + 2 + border, this->m_fontHeight + 2 + border);
}

// OFFSET: 0x00897960
// FUNCTION: fn_11537
void UITextButton::ResizeToText() {
  if (std::strcmp(this->label.c_str(), "") == 0) {
    return;
  }

  this->m_text = this->label;
  const int width = this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, this->label.c_str(), 0, this->m_family, this->m_height, this->m_outline, 0);
  UIWindow::Resize(width + 2, this->m_height + 2);
}

// OFFSET: 0x00897A50
// FUNCTION: fn_11539
void UITabControl::FitToTabs() {
  int length = 0;

  for (int index = 0; index < static_cast<int>(this->labels.size()); ++index) {
    length += this->widths[index] - 1;
  }

  if (this->layout == 0 || this->layout == 2) {
    this->Resize(this->tabHeight + 1, length + 1);
  } else {
    this->Resize(length + 1, this->tabHeight + 1);
  }

  this->Invalidate();
}

// OFFSET: 0x00897BE0
// FUNCTION: fn_11540
int TableBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008984E0
// FUNCTION: fn_11547
int UIComboBoxV2Wnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 != 273) {
      return 0;
    }

    int height = this->unknown_0098->geometry.height;

    if (this->geometry.height <= height) {
      height += this->list->geometry.height;
    }

    this->Resize(this->geometry.width, height);

    if (this->parent) {
      this->parent->Invalidate();
    }

    this->Invalidate();
    return 0;
  }

  if (message != 39) {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (WindowPolicy(g_windowMgr.policy[0].actions, g_replayAnimationScaling != 0, 3, this->parent->geometry.id) == 0) {
    return 0;
  }

  this->SendMsg(nullptr, 6, 273, 0, 0, 0);
  this->selected = argument1;

  if (this->parent) {
    this->parent->SendMsg(this, 39, argument1, 0, 0, 0);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x00898730
// FUNCTION: fn_11550
int UIImgMenu::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 34: {
    this->horizontal = argument1;
    this->count = argument2;
    std::string name(reinterpret_cast<const char*>(argument3));
    this->bitmapName = name + ".bmp";
    this->baseName = name;
    this->unknown_0090 = new bool[this->count];
    this->unknown_0094 = new bool[this->count];

    for (int index = 0; index < this->count; ++index) {
      this->unknown_0090[index] = false;
      this->unknown_0094[index] = false;
    }

    this->Invalidate();
    return 0;
  }
  case 83:
    this->unknown_00C8 = argument1;
    return 0;
  case 102:
    this->geometry.id = argument1;
    return 0;
  case 105:
    this->unknown_0090[argument1 - 1] = argument2 != 0;
    return 0;
  case 106:
    this->unknown_0094[argument1 - 1] = argument2 != 0;
    return 0;
  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x00897BF0
// FUNCTION: fn_11541
int UIBmpRadioBtn::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 13) {
    this->geometry.unknown_0030 = static_cast<std::uint32_t>(argument1);
    this->Invalidate();
  }
  return 0;
}

// OFFSET: 0x00892060
// FUNCTION: fn_11463
void UIComboBox::Slot27(int x, int y) {
  const int previous = this->selected;
  this->selected = this->LineAt(x, y);

  if (previous != this->selected) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892100
// FUNCTION: fn_11465
void UIComboBoxID::Slot27(int x, int y) {
  const int previous = this->selected;
  int line = -1;

  if (x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    const int index = y / this->lineHeight + this->topLine;

    if (index >= 0 && static_cast<unsigned>(index) < this->lines.size()) {
      line = index;
    }
  }

  this->selected = line;

  if (previous != line) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892160
// FUNCTION: fn_11466
void UIImgMenu::Slot27(int x, int y) {
  this->Slot28(x, y);
}

// OFFSET: 0x00892170
// FUNCTION: fn_11467
void UIItemStoreComboBox::Slot27(int x, int y) {
  const int previous = this->selected;
  int line = -1;

  if (x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    const int index = y / this->lineHeight + this->topLine;

    if (index >= 0 && static_cast<unsigned>(index) < this->lines.size()) {
      line = index;
    }
  }

  this->selected = line;

  if (previous != line) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892340
// FUNCTION: fn_11469
void UITVTextButton::Slot27(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    if (this->geometry.unknown_0030 != 1) {
      this->geometry.unknown_0030 = 1;

      if (this->parent && this->unknown_00B6 == 0) {
        this->parent->SendMsg(this, 108, this->m_value, 0, 0, 0);
      }
    }
  } else if (this->geometry.unknown_0030 != 0) {
    this->geometry.unknown_0030 = 0;
    this->Invalidate();
    this->unknown_00B4 = 0;
    this->unknown_00B5 = 0;
  }
}

// OFFSET: 0x008923D0
// FUNCTION: fn_11470
void UITabControl::Slot27(int x, int y) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  if ((this->dragX == 0 && this->dragY == 0) || this->dragX - x > 5 || this->dragY - y > 5) {
    if (this->parent) {
      this->parent->SendMsg(this, 117, this->unknown_007C, 0, 0, 0);
    }

    this->dragX = x;
    this->dragY = y;
  }
}

// OFFSET: 0x00892520
// FUNCTION: fn_11472
void UITreeView::Slot27(int x, int y) {
  this->unknown_00AC = x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height;
}

// OFFSET: 0x00892560
// FUNCTION: fn_11473
void UIImgMenu::ResetCursor(int, int) {
  this->m_context->cursor.Set(2, this->m_context->cursorTick);
}

// OFFSET: 0x008925A0
// FUNCTION: fn_11475
void UIString::ResetCursor(int, int) {
  if (!this->unknown_0084.empty() || !this->unknown_00AC.empty()) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  }
}

// OFFSET: 0x0086EA50
// FUNCTION: fn_11080
UIString::UIString(UIWindowContext& windows) : UIWindow(windows, 0), unknown_0080(1), unknown_009C(0), unknown_00A0(0), unknown_00A4(0) {
  this->unknown_0084.clear();
  this->unknown_00AC.clear();
  this->unknown_00C4.clear();
  this->unknown_00DC.clear();
}

// OFFSET: 0x008925D0
// FUNCTION: fn_11476
void UITVTextButton::ResetCursor(int x, int y) {
  if (this->m_disabled != 0) {
    UIWindow::ResetCursor(x, y);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}

// OFFSET: 0x00892600
// FUNCTION: fn_11477
void UIEditCtrl_Currency::Slot19() {
  if (this->formattedLength == this->text.size()) {
    return;
  }

  CLanguage& input = this->m_edit->input;
  int anchor;
  int cursor;
  input.Selection(anchor, cursor);

  std::string digits = this->text;
  int separators = 0;
  int index = 0;

  for (std::string::iterator it = digits.begin(); it != digits.end(); ++it, ++index) {
    if (this->text[index] == ',' && index < anchor) {
      ++separators;
    }

    if (*it < '0' || *it > '9') {
      it = digits.erase(it);

      if (it == digits.end()) {
        break;
      }
    }
  }

  char formatted[260];
  std::memset(formatted, 0, sizeof(formatted));
  FormatGroupedInteger(_atoi64(digits.c_str()), formatted, sizeof(formatted));
  this->text = formatted;
  this->Trim();

  input.ResetInput();
  input.ResetInput();
  input.Insert(this->text.c_str());

  const int length = static_cast<int>(this->text.size());
  anchor += length / 4 - (length - anchor) / 4 - separators;
  cursor += length / 4 - (length - cursor) / 4 - separators;

  if (digits.empty()) {
    anchor = 1;
    cursor = 1;
  }

  input.Select(anchor, cursor);
  this->formattedLength = this->text.size();
  this->Invalidate();
}

// OFFSET: 0x00892910
// FUNCTION: fn_11478
void UISlotBmpButton::Slot19() {
  ++this->unknown_0134;

  if (this->unknown_0134 / this->unknown_0136 > 7) {
    this->unknown_0134 = 0;
  }
}

// OFFSET: 0x00892940
// FUNCTION: fn_11479
void UIBasicItemCtrl::Slot32(int, int) {
  if (std::atoi(this->m_item.itemId.c_str()) > 0) {
    this->m_windows->EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
  }
}

// OFFSET: 0x00892A10
// FUNCTION: fn_11481
void UIBitmapButton2::Slot32(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, 101, this->geometry.id, 0, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892A50
// FUNCTION: fn_11482
void UIMemberListBox::Slot32(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x00892A80
// FUNCTION: fn_11483
void UISlotBmpButton::Slot32(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 6, this->geometry.id, 226, 0, 0);
  }
}

// OFFSET: 0x00892AB0
// FUNCTION: fn_11484
void UIStaticItem::Slot32(int, int) {
  this->m_windows->EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
}

// OFFSET: 0x00892AE0
// FUNCTION: fn_11485
void UITVBitmapButton::Slot32(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, 101, this->m_value, 0, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892B20
// FUNCTION: fn_11486
void UITVTextButton::Slot32(int x, int y) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, 99, this->m_value, x, y, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892BA0
// FUNCTION: fn_11488
void UIBitmapButton2::Slot33(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 101, this->geometry.id, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x0087B4B0
// FUNCTION: fn_11240
void UIChatHisBox2::Configure(int x, int y, int width, int height, unsigned char) {
  this->transparent = 0;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->Slot15(width, height);
  this->Slot22(width, height);
}

// OFFSET: 0x00892C20
// FUNCTION: fn_11489
void UIChatHisBox2::Slot33(int x, int y) {
  this->parent->Slot33(x + this->geometry.x, y + this->geometry.y);
}

// OFFSET: 0x00892C50
// FUNCTION: fn_11490
void UIMemberListBox::Slot33(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 49, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00892CA0
// FUNCTION: fn_11492
void UITVBitmapButton::Slot33(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 101, this->m_value, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892D20
// FUNCTION: fn_11493
void UITVTextButton::Slot33(int x, int y) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, 99, this->m_value, x, y, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892DE0
// FUNCTION: fn_11495
void UIComboBoxV2Wnd::Slot35(int value) {
  this->selected += value < 0 ? 1 : -1;
  const int count = static_cast<int>(this->list->lines.size());

  if (this->selected >= count) {
    this->selected = count - 1;
  }

  if (this->selected < 0) {
    this->selected = 0;
  }

  if (this->parent) {
    this->parent->SendMsg(this, 39, this->selected, 0, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x0087A080
// FUNCTION: fn_11225
void UIListBoxID::Clear() {
  this->selected = -1;
  this->contentWidth = 0;
  this->horizontalOffset = 0;
  this->topLine = 0;
  this->verticalOverflow = 0;
  this->bottomMargin = 0;
  this->lines.clear();
}

// OFFSET: 0x00892EE0
// FUNCTION: fn_11498
void UIListBoxID::Slot35(int value) {
  if (this->verticalOverflow != 0) {
    if (value < 0) {
      this->SendMsg(this->verticalBar, 7, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->verticalBar, 7, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00892F50
// FUNCTION: fn_11501
void UITreeView::Slot35(int value) {
  if (this->verticalOverflow != 0) {
    if (value < 0) {
      this->SendMsg(this->verticalBar, 7, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->verticalBar, 7, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x008861E0
// FUNCTION: fn_11324
void UIItemStoreComboBox::Slot15(int width, int height) {
  this->UIListBoxEx::Slot15(width, height);
  this->lineHeight = 15;
}

// OFFSET: 0x00897A10
// FUNCTION: fn_11538
void UITreeView::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Move(this->geometry.width - 10, 0);
  this->verticalBar->Resize(this->verticalBar->geometry.width, height);
  this->Slot53();
}

// OFFSET: 0x008961F0
// FUNCTION: fn_11516
void UITreeView::Slot53() {
  const int visible = this->geometry.height / this->itemHeight;
  const int count = this->itemCount;
  const int excess = count - visible;
  const int overflow = excess < 0 ? 0 : excess;
  this->verticalOverflow = overflow != 0;
  this->verticalBar->SetRange(overflow, visible, count);

  if (!overflow) {
    this->unknown_00A8 = 0;
    this->itemCount = 0;
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x00892F90
// FUNCTION: fn_11502
void UITabControl::FillTab(int index, int x, int y, int width, int height, std::uint32_t color) {
  if (this->unknown_00A9 != 0) {
    return;
  }

  const std::uint32_t tabColor = this->colors[index];

  if (tabColor != 0xFFFFFFFFu) {
    color = tabColor;
  }

  if (this->layout == 1) {
    this->FillRegion(x + 1, y + 1, width - 2, height - 1, color);
  } else if (this->layout == 3) {
    this->FillRegion(x + 1, y, width - 2, height - 1, color);
  } else if (this->layout == 0) {
    this->FillRegion(x + 1, y + 1, width - 1, height - 2, color);
  } else if (this->layout == 2) {
    this->FillRegion(x, y + 1, width - 1, height - 2, color);
  }
}

// OFFSET: 0x0088F100
// FUNCTION: fn_11386
void UIStaticItem::Paint() {
  char path[260];
  std::memset(path, 0, sizeof(path));
  this->m_descriptions->IconPath(path, this->m_item.itemId.c_str(), this->m_item.identified);

  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  if (!bitmap) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
    if (!bitmap) {
      return;
    }
  }

  this->DrawBitmap(0, 0, &bitmap->image, true);
}

// OFFSET: 0x0088F5C0
// FUNCTION: fn_11389
void UITVTextButton::Paint() {
  this->ClearSurface(0xFFFF00FFu);

  if (this->parent) {
    this->parent->Invalidate();
  }

  const int inset = this->m_border != 0;

  if (this->unknown_00B4 != 0) {
    if (this->m_border != 0) {
      DrawRoundedBorder(*this, inset - 1, 0, this->geometry.width, this->geometry.height, this->m_borderColor);
    }

    if (this->m_fill != 0) {
      this->FillRegion(inset, 1, this->geometry.width - inset * 2, this->geometry.height - 2, this->m_fillColor);
    }
  }

  std::uint32_t color = this->m_textColor;

  if (this->unknown_00B5 != 0) {
    color = this->m_highlightColor;
  }

  if (this->m_outlined != 0) {
    DrawOutlinedText(*this, inset + 1, 2, this->unknown_00DC.c_str(), 0, color, this->m_outlineColor, this->m_fontFamily, this->m_fontHeight, 1, 0);
  } else {
    this->DrawTextAt(this->m_buttonContext->fonts, this->m_buttonContext->locale, inset + 1, 2, this->unknown_00DC.c_str(), 0, this->m_fontFamily, this->m_fontHeight, color, 0, 0);
  }
}

// OFFSET: 0x0088FFB0
// FUNCTION: fn_11397
void UIBasicItemCtrl::Slot26(int, int) {
  this->m_item = ITEM_INFO(0);

  if (this->Root()) {
    this->Root()->Invalidate();
    this->Root()->InvalidateChildren();
  } else {
    this->Invalidate();
  }
}

// OFFSET: 0x00890080
// FUNCTION: fn_11398
void UIBasicSkillCtrl::Slot26(int, int) {
  this->m_skillId = -1;

  if (this->Root()) {
    this->Root()->Invalidate();
    this->Root()->InvalidateChildren();
  } else {
    this->Invalidate();
  }
}

// OFFSET: 0x00890150
// FUNCTION: fn_11400
void UIComboBox::Slot26(int, int) {
}

// OFFSET: 0x00890170
// FUNCTION: fn_11402
void UIComboBoxID::Slot26(int, int) {
}

// OFFSET: 0x008902A0
// FUNCTION: fn_11408
void UISlotBmpButton::Slot26(int x, int y) {
  UIBitmapButton::Slot26(x, y);

  if (this->parent) {
    this->parent->SendMsg(this, 6, 184, 0, 0, 0);
  }
}

// OFFSET: 0x008902E0
// FUNCTION: fn_11409
void UITVTextButton::Slot26(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.unknown_0030 = 1;

  if (this->parent) {
    this->parent->SendMsg(this, 97, this->m_value, x, y, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00890400
// FUNCTION: fn_11411
void UIBmpRadioBtn::Slot25(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->m_value, 0, 0);
  }
}

// OFFSET: 0x00890450
// FUNCTION: fn_11413
void UIComboBox::Slot25(int, int) {
}

// OFFSET: 0x00890470
// FUNCTION: fn_11415
void UIComboBoxID::Slot25(int, int) {
}

// OFFSET: 0x008905F0
// FUNCTION: fn_11420
void UIListBoxID::Slot25(int x, int y) {
  int line = -1;

  if (x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    const int index = y / this->lineHeight + this->topLine;

    if (index >= 0 && static_cast<unsigned>(index) < this->lines.size()) {
      line = index;
    }
  }

  this->selected = line;
  this->Invalidate();
}

// OFFSET: 0x00890640
// FUNCTION: fn_11421
void UIListBoxNavi::Slot25(int x, int y) {
  if (g_replayAnimationScaling != 0) {
    return;
  }

  this->selected = this->LineAt(x, y);

  if (this->selected != -1) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00890690
// FUNCTION: fn_11422
void UIMemberListBox::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x008908A0
// FUNCTION: fn_11426
void UISlotBmpButton::Slot25(int x, int y) {
  UIBitmapButton::Slot25(x, y);

  if (this->parent) {
    this->parent->SendMsg(this, 13, 0, 0, 0, 0);
    this->parent->SendMsg(this, 6, this->geometry.id, 0, 0, 0);
  }
}

// OFFSET: 0x008908F0
// FUNCTION: fn_11427
void UITVTextButton::Slot25(int, int) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.unknown_0030 = 2;
  this->Invalidate();

  if (this->parent) {
    this->parent->SendMsg(this, 107, this->m_value, 0, 0, 0);
    this->unknown_00B6 = 1;
  }
}

// OFFSET: 0x00890940
// FUNCTION: fn_11428
void UITabControl::Slot25(int x, int y) {
  const int index = this->TabAt(x, y);

  if (index == -1) {
    return;
  }

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
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x00890A00
// FUNCTION: fn_11430
void UITreeView::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00890A30
// FUNCTION: fn_11431
void UIBitmapButton2::Slot31(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 100, this->geometry.id, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00890DA0
// FUNCTION: fn_11437
void UIComboBoxV2Wnd::Slot31(int, int) {
  this->SendMsg(nullptr, 6, 273, 0, 0, 0);
}

// OFFSET: 0x00890E30
// FUNCTION: fn_11439
void UIItemStoreComboBox::Slot31(int x, int y) {
  int line = -1;

  if (x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    const int index = y / this->lineHeight + this->topLine;

    if (index >= 0 && static_cast<unsigned>(index) < this->lines.size()) {
      line = index;
    }
  }

  this->selected = line;

  if (this->parent) {
    this->parent->SendMsg(nullptr, 39, line, 0, 0, 0);
  }
}

// OFFSET: 0x00891050
// FUNCTION: fn_11442
void UITVBitmapButton::Slot31(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;

    if (this->parent) {
      this->parent->SendMsg(this, 100, this->m_value, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x008910D0
// FUNCTION: fn_11443
void UITVTextButton::Slot31(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (this->parent) {
    if (this->unknown_00B6 != 0) {
      this->parent->SendMsg(this, 98, this->m_value, x, y, 0);
    } else {
      this->parent->SendMsg(this, 109, this->m_value, 0, 0, 0);
    }

    this->unknown_00B6 = 0;
  }

  this->Invalidate();
}

// OFFSET: 0x00891130
// FUNCTION: fn_11444
void UITabControl::Slot31(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x0088AD00
// FUNCTION: fn_11359
void UIINT64BarGraph::Paint() {
  if (this->m_maximum < this->m_current || this->m_maximum == 99999999) {
    return;
  }

  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 26, 2, red, green, blue);
  this->ClearSurface((green & 255) << 16 | (green & 255) << 8 | (red & 255));
  SkinPaletteChannels(ResourceMgr::Instance(), 30, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, (blue & 255) << 16 | (green & 255) << 8 | (red & 255));

  if (this->m_maximum == 0) {
    return;
  }

  const double ratio = static_cast<double>(this->m_current) / static_cast<double>(this->m_maximum);

  if (ratio == 0.0) {
    return;
  }

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 6, red, green, blue);

  if (this->m_plainColor == 0 && 25.0 > ratio * 100.0) {
    red = 255;
    green = 0;
    blue = 0;
  }

  this->FillRegion(
    1, 1, static_cast<int>((this->geometry.width - 2) * ratio), this->geometry.height - 2, (blue & 255) << 16 | (green & 255) << 8 | (red & 255)
  );
}

// OFFSET: 0x0088B070
// FUNCTION: fn_11361
void UIItemMoveInfo::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->parent) {
    this->parent->Invalidate();
  }

  if (this->m_outline != 0) {
    DrawOutlinedText(*this, 1, 1, this->m_text.c_str(), 0, this->m_color, this->m_outlineColor, this->m_family, this->m_height, 1, 0);
  } else {
    auto& controls = *this->m_buttonContext;
    this->DrawTextAt(controls.fonts, controls.locale, 1, 1, this->m_text.c_str(), 0, this->m_family, this->m_height, this->m_color, 0, 0);
  }

  if (this->m_underline != 0) {
    this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, this->m_color);
  }
}

// OFFSET: 0x0088B4E0
// FUNCTION: fn_11364
void UIItemTagButton::Paint() {
  this->ClearSurface(0xFFFF00FF);

  if (this->parent) {
    this->parent->Invalidate();
  }

  if (this->m_outline != 0) {
    DrawOutlinedText(*this, 1, this->textY, this->m_text.c_str(), 0, this->m_color, this->m_outlineColor, this->m_family, this->m_height, 1, 0);
  } else {
    auto& controls = *this->m_buttonContext;
    this->DrawTextAt(controls.fonts, controls.locale, 1, this->textY, this->m_text.c_str(), 0, this->m_family, this->m_height, this->m_color, 0, 0);
  }

  if (this->m_underline != 0) {
    this->FillRegion(0, this->geometry.height - 1, this->geometry.width, 1, this->m_color);
  }
}

// OFFSET: 0x0088BFB0
// FUNCTION: fn_11369
void UIListBoxNavi::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);

    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface((this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16));
  }

  const int first = this->topLine;
  const int limit = this->geometry.height / this->lineHeight - this->bottomMargin + first;
  const int end = static_cast<int>(std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(limit)));
  std::uint32_t color = 0;
  auto& owner = *this->m_listContext;

  for (int index = first, row = 0; index < end; ++index, ++row) {
    if (this->lineColors[index] == 1) {
      color = this->defaultLineColor;
    }

    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    const auto& line = this->lines[index];
    DrawColoredText(*this, owner.codePage, 3 - this->horizontalOffset, this->lineHeight * row, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false);

    if (this->lineColors[index] == 1) {
      color = 0;
    }
  }
}

// OFFSET: 0x0088B4A0
// FUNCTION: fn_11363
void UIItemStoreComboBoxWnd::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\bg_setting.bmp")));
  this->DrawBitmap(0, this->geometry.height - background->image.height, &background->image, false);
}

// OFFSET: 0x00884700
// FUNCTION: fn_11308
void UIImgMenu::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00899A20
// FUNCTION: fn_11558
int UIString::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x009B4EF0
// FUNCTION: fn_13834
const std::string& UITextButton::GetLine(std::size_t index) const {
  try {
    return this->m_lines.at(index);
  } catch (...) {
    return g_emptyString;
  }
}

// OFFSET: 0x0088D750

// OFFSET: 0x0088DBF0

// OFFSET: 0x0088E0B0

// OFFSET: 0x0088E730

// OFFSET: 0x0088FE20

// OFFSET: 0x00884DD0
// FUNCTION: fn_11313
void TableBox::Slot15(int, int) {
}

// OFFSET: 0x00884DE0
// FUNCTION: fn_11314
void UIChatHisBox2::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar(*this->m_context);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 13, 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar2(*this->m_context);
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 13, horizontalHeight);
  horizontal->Move(0, height - 13);
  horizontal->Slot15(width - 13, horizontalHeight);
  horizontal->Slot22(width - 13, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x00884F40
// FUNCTION: fn_11315
void UIChatHisBox3::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar3(*this->m_context);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 10, 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar3(*this->m_context);
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 10, horizontalHeight);
  horizontal->Move(0, height - 10);
  horizontal->Slot15(width - 10, horizontalHeight);
  horizontal->Slot22(width - 10, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x008850C0
// FUNCTION: fn_11316
void UIChatHisBox::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar2(*this->m_context);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 13, 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar2(*this->m_context);
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 13, horizontalHeight);
  horizontal->Move(0, height - 13);
  horizontal->Slot15(width - 13, horizontalHeight);
  horizontal->Slot22(width - 13, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x0088C530

// OFFSET: 0x0088AEC0

// OFFSET: 0x0088F1C0

// OFFSET: 0x0088F810
// FUNCTION: fn_11391
void UITableBox::Paint() {
  this->ClearSurface(0);
}

// OFFSET: 0x008906D0
// FUNCTION: fn_11424
void UIPackageBoxNameList::Slot25(int x, int y) {
  this->selected = this->LineAt(x, y);
  if (this->selected != -1) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->selected, 0, 0);
  }

  this->Invalidate();
}
