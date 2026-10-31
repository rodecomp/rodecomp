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
#include "UI/UIStaticBitmap.h"
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

// OFFSET: 0x008816B0
// FUNCTION: fn_11298
void UIBmpRadioBtn2::Invalidate() {
  UIWindow::Invalidate();

  if (this->parent != nullptr && !this->parent->IsCached()) {
    this->parent->SendMsg(this, 60, 0, 0, 0, 0);
  }
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

// OFFSET: 0x008970F0
// FUNCTION: fn_11524
void UIChatHisBox::Resize(int width, int height, bool flag) {
  const int oldWidth = this->geometry.width;
  this->Resize(width, height);
  if (this->geometry.width != oldWidth && flag) {
    this->sub_89C080();
  }
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

// OFFSET: 0x00897230
// FUNCTION: fn_11525
void UITextButton::ResizeToTextClipped(int width, int height) {
  if (std::strcmp(this->label.c_str(), "") == 0) {
    return;
  }

  this->ResizeToText();
  if (this->geometry.width > width) {
    this->m_text = this->ClipTextToWidth(this->label.c_str(), width, this->m_family, this->m_height, this->m_outline);
    UIWindow::Resize(width, height);
  }
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

// OFFSET: 0x00882410
// FUNCTION: fn_11301
bool UIChatHisBox2::Slot08() {
  return false;
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

// OFFSET: 0x008977C0
// FUNCTION: fn_11534
void UIListBoxID::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Resize(12, height);
  this->verticalBar->Move(width - 13, 0);
  this->horizontalBar->Resize(width - 13, 12);
  this->horizontalBar->Move(0, height - 13);
  this->Refresh();
}

// OFFSET: 0x00897630
// FUNCTION: fn_11531
void UIComboBoxID::Resize(int width, int height) {
  UIListBoxID::Resize(width, height);
  this->topLine = 0;
  this->verticalBar->position = this->topLine;
  this->verticalBar->Move(width - 13, 0);
  if (this->verticalOverflow != 0) {
    this->verticalBar->Invalidate();
  }
}

// OFFSET: 0x00896060
// FUNCTION: fn_11514
void UIListBoxID::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = std::max((count - visible), 0);

  this->verticalOverflow = overflow != 0;
  this->verticalBar->maximum = overflow != 0 ? (this->bottomMargin + overflow) : 0;
  this->verticalBar->total = count;
  this->verticalBar->visible = visible;
  this->verticalBar->Invalidate();

  if (this->verticalBar->position > this->verticalBar->maximum) {
    this->verticalBar->position = this->verticalBar->maximum;
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x00895D80
// FUNCTION: fn_11511
void UIComboBoxID::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = std::max((count - visible), 0);

  this->verticalOverflow = overflow != 0;
  UIScrollBar* bar = this->verticalBar;
  if (overflow != 0) {
    bar->visible = visible;
    bar->maximum = this->bottomMargin + overflow;
    bar->total = count;
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
    bar->total = count;
    bar->visible = visible;
    bar->maximum = 0;
    bar->Invalidate();
    if (bar->position > bar->maximum) {
      bar->position = bar->maximum;
    }
  }

  this->verticalBar->Invalidate();
  if (this->Root()) {
    this->Root()->Invalidate();
  }
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
// FUNCTION: fn_11378
void UIScrollBar2::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  std::string path = "유저인터페이스\\basic_interface\\";
  if (this->vertical == 1) {
    this->FillRegion(0, 11, 10, this->geometry.height - 21, 0);
    if (this->state == 1) {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    } else if (this->state == 4) {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    } else {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    }

    const int thumb = this->ThumbOffset() + this->arrowLength;
    this->FillRegion(0, thumb + 1, 10, this->ThumbLength() - 1, 0xAFAAA5);
    if (this->state == 2) {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, down ? &down->image : nullptr, false);
    } else if (this->state == 5) {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, down ? &down->image : nullptr, false);
    } else {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, down ? &down->image : nullptr, false);
    }
  }
}

// OFFSET: 0x0088DBF0
// FUNCTION: fn_11379
void UIScrollBar3::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  std::string path = "유저인터페이스\\basic_interface\\";
  if (this->vertical == 1) {
    this->FillRegion(0, 10, 9, this->geometry.height - 20, 0xF1F1F1);
    if (this->state == 1) {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    } else if (this->state == 4) {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    } else {
      auto* up = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str())));
      this->DrawBitmap(0, 0, up ? &up->image : nullptr, false);
    }

    int thumb = this->ThumbOffset() + this->arrowLength;
    if (thumb < 9) {
      thumb = 9;
    }

    int length = this->ThumbLength();
    if (this->geometry.height - 20 < length) {
      length = 1;
    }

    this->FillRegion(0, thumb + 1, 9, length - 1, 0xE9E9E9);
    if (this->state == 2) {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, down ? &down->image : nullptr, false);
    } else if (this->state == 5) {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, down ? &down->image : nullptr, false);
    } else {
      auto* down = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str())));
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, down ? &down->image : nullptr, false);
    }
  }

  this->minimumThumb = 0;
}

// OFFSET: 0x0088E0B0
// FUNCTION: fn_11380
void UIScrollBar4::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  std::string path = "유저인터페이스\\basic_interface\\";
  if (this->vertical == 0) {
    auto* track = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "scrollbar.bmp").c_str())));
    this->DrawBitmap(0, 0, track ? &track->image : nullptr, true);

    const int offset = this->ThumbOffset();
    auto* thumb = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "scroll_bar.bmp").c_str())));
    this->DrawBitmap(offset, 0, thumb ? &thumb->image : nullptr, true);
  }

  this->minimumThumb = 0;
}

// OFFSET: 0x0088E730
// FUNCTION: fn_11382
void UIScrollBar_Custom::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0 || this->vertical != 1) {
    return;
  }

  int top = 0;
  if (this->m_trackTop) {
    top = this->m_trackTop->image.height;
    this->DrawBitmap(0, 0, &this->m_trackTop->image, false);
  }

  int middle;
  if (!this->m_trackMiddle) {
    middle = 0;
  } else {
    middle = this->m_trackMiddle->image.height;
    int y = top;
    for (int index = 0; index < (this->geometry.height - top) / middle; ++index) {
      this->DrawBitmap(0, y, &this->m_trackMiddle->image, false);
      y += middle;
    }
  }

  if (this->m_trackBottom && (this->geometry.height - top) % middle > 0) {
    this->DrawBitmap(0, this->geometry.height - this->m_trackBottom->image.height, &this->m_trackBottom->image, false);
  }

  if (this->m_arrowUp) {
    this->DrawBitmap(0, 0, &this->m_arrowUp->image, true);
  }

  if (this->m_arrowDown) {
    this->DrawBitmap(0, this->geometry.height - this->m_arrowDown->image.height, &this->m_arrowDown->image, true);
  }

  const int length = this->vertical == 0 ? this->geometry.width : this->geometry.height;
  const int thumb = this->position * (length - this->arrowLength * 2 - this->ThumbLength()) / this->maximum + this->arrowLength;
  const int thumbLength = this->ThumbLength();

  int thumbTop = 0;
  if (this->m_thumbTop) {
    thumbTop = this->m_thumbTop->image.height;
    this->DrawBitmap(0, thumb, &this->m_thumbTop->image, true);
  }

  if (this->m_thumbMiddle) {
    const int step = this->m_thumbMiddle->image.height;
    int count = (thumbLength - thumbTop) / step;
    if (count > 0) {
      int y = thumb + thumbTop;
      do {
        this->DrawBitmap(0, y, &this->m_thumbMiddle->image, true);
        y += step;
      } while (--count);
    }
  }

  if (this->m_thumbBottom) {
    this->DrawBitmap(0, thumbLength - this->m_thumbBottom->image.height + thumb, &this->m_thumbBottom->image, true);
  }
}

// OFFSET: 0x0088FE20
// FUNCTION: fn_11396
void UIZeroQuestScrollBar::Paint() {
  this->ClearSurface(0xFFD9C8B8);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, 0xFFFFFFFF);

  if (this->maximum == 0) {
    return;
  }

  const auto* up = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_arrow_up.bmp"));
  this->DrawBitmap(0, 0, reinterpret_cast<const BitmapImage*>(up), true);

  const auto* down = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_arrow_down.bmp"));
  this->DrawBitmap(0, this->geometry.height - this->arrowLength, reinterpret_cast<const BitmapImage*>(down), true);

  const int extent = this->vertical == 0 ? this->geometry.width : this->geometry.height;
  int y = this->arrowLength + (this->position * (extent - this->arrowLength * 2 - this->ThumbLength())) / this->maximum;

  const auto* thumbUp = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_up.bmp"));
  this->DrawBitmap(0, y, reinterpret_cast<const BitmapImage*>(thumbUp), true);

  const int bottom = y + this->ThumbLength() - 4;
  for (y += 4; y < bottom; y += 4) {
    const auto* thumbMiddle = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_mid.bmp"));
    this->DrawBitmap(0, y, reinterpret_cast<const BitmapImage*>(thumbMiddle), true);
  }

  const auto* thumbDown = ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_down.bmp"));
  this->DrawBitmap(0, bottom, reinterpret_cast<const BitmapImage*>(thumbDown), true);
}

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
// FUNCTION: fn_11371
void UIMemberListBox::Paint() {
  this->ClearSurface((this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16));

  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  for (int index = first, row = 0; index < static_cast<int>(end); ++index, ++row) {
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight + 1, this->geometry.width, this->lineHeight, 0xC8C832);
    }

    this->DrawTextAt(
      this->m_controls->fonts, this->m_controls->locale, 2, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->unknown_00E4[index], 0, 0
    );
  }
}

// OFFSET: 0x00888C10
// FUNCTION: fn_11343
void UIBasicItemCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const char* name = "유저인터페이스\\basic_interface\\itemwin_mid.bmp";
  if (std::atoi(this->m_item.itemId.c_str()) > 0 && this->m_item.unknown_0000[1] == 0) {
    name = "유저인터페이스\\ExMacro\\no_item.bmp";
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)))->image, false);
  this->DrawItemIcon(4, 0, 0, false);
}

// OFFSET: 0x00888C90
// FUNCTION: fn_11344
void UIBasicSkillCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  bool drawn = false;
  if (this->m_skillId >= 0) {
    CSkillInfo skill = this->m_skills->Lookup(static_cast<std::uint32_t>(this->m_skillId));
    if (skill.available != 0) {
      drawn = true;
      if (this->m_level > 0) {
        skill.level = this->m_level;
      }

      char text[1024];
      g_session.ItemBitmapPath(skill.name, text);
      this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)))->image, true);
      if (skill.level != 0) {
        std::sprintf(text, "%d", skill.level);
        FontCache& fonts = this->m_controls->fonts;
        const TextLocale& locale = this->m_controls->locale;
        const int x = this->geometry.width - this->TextWidth(fonts, locale, text, 0, 0, 11, 0, 0);
        this->DrawTextAt(fonts, locale, x, 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(fonts, locale, x - 2, 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(fonts, locale, x - 1, 16, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(fonts, locale, x - 1, 14, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(fonts, locale, x - 1, 15, text, 0, 0, 11, 0, 0, 0);
      }
    }
  }

  if (!drawn) {
    this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\no_skill.bmp")))->image, false);
  }
}

// OFFSET: 0x00888EA0
// FUNCTION: fn_11345
void UIBmpRadioBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const std::string* name;
  if (this->m_disabled == true) {
    name = &this->m_names[3];
  } else if (this->geometry.unknown_0030 == 1) {
    name = &this->m_names[2];
  } else if (this->m_hovered == true) {
    name = &this->m_names[1];
  } else {
    name = &this->m_names[0];
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name->c_str())))->image, false);
  if (this->m_label.size() == 0) {
    return;
  }

  FontCache& fonts = this->m_controls->fonts;
  const TextLocale& locale = this->m_controls->locale;
  if (this->m_disabled == true) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 3) {
        if (it->unknown_0018 == true) {
          this->DrawTextAt(fonts, locale, it->x, it->y, this->m_label.c_str(), 0, 0, it->height, 0xE2E2E2, 0, 0);
        }
        this->DrawTextAt(fonts, locale, it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(fonts, locale, x, y, this->m_label.c_str(), 0, 0, 12, 0xE2E2E2, 0, 0);
    this->DrawTextAt(fonts, locale, x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0x6F6F6F, 0, 0);
    return;
  }

  if (this->geometry.unknown_0030 == 1) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 2) {
        if (it->unknown_0018 == true) {
          this->DrawTextAt(fonts, locale, it->x, it->y, this->m_label.c_str(), 0, 0, it->height, 0xFFFFFF, it->bold, 0);
        }
        this->DrawTextAt(fonts, locale, it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(fonts, locale, x, y, this->m_label.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(fonts, locale, x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  if (this->m_hovered == true) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 1) {
        if (it->unknown_0018 == true) {
          this->DrawTextAt(fonts, locale, it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, 0xC7A796, it->bold, 0);
        }
        this->DrawTextAt(fonts, locale, it->x, it->y, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(fonts, locale, x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0xC7A796, 0, 0);
    this->DrawTextAt(fonts, locale, x, y, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
    if (it->id == 0) {
      if (it->unknown_0018 == true) {
        this->DrawTextAt(fonts, locale, it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, 0xD7D7D7, it->bold, 0);
      }
      this->DrawTextAt(fonts, locale, it->x, it->y, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
      return;
    }
  }

  const int x = static_cast<int>((this->geometry.width - this->TextWidth(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
  const int y = static_cast<int>((this->geometry.height - this->TextHeight(fonts, locale, this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
  this->DrawTextAt(fonts, locale, x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0xD7D7D7, 0, 0);
  this->DrawTextAt(fonts, locale, x, y, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008894A0
// FUNCTION: fn_11346
void UICaptchaImage::Paint() {
  this->DrawBitmap(0, 0, &this->m_bitmap.image, true);
}

// OFFSET: 0x00889900
// FUNCTION: fn_11350
void UIComboBox::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")))->image, false);
  const auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, &leftDown->image, false);
  const auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, &rightUp->image, false);
  const auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, &rightDown->image, false);

  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  for (int index = first, row = 0; index < static_cast<int>(end); ++index, ++row) {
    if (index == this->selected) {
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(3, row * this->lineHeight + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }

    this->DrawTextAt(
      this->m_listContext->fonts, this->m_listContext->locale, 3 - this->horizontalOffset, this->lineHeight * row + 2, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0
    );
  }

  if (this->verticalOverflow != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(this->geometry.width - 16, 0, 16, this->geometry.height, RGB(red, green, blue));
  }

  this->parent->Invalidate();
}

// OFFSET: 0x0088AEC0

// OFFSET: 0x0088F1C0
// FUNCTION: fn_11387
void UIStaticItem_Illust::Paint() {
  const std::string path = "유저인터페이스\\collection\\" + std::string(ItemResourcePath(this->m_item)) + ".bmp";
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));
  if (!bitmap) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\collection\\사과.bmp")));
    if (!bitmap) {
      return;
    }
  }

  this->DrawBitmap(0, 0, &bitmap->image, true);
}

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

// OFFSET: 0x00886AE0
// FUNCTION: fn_11328
void UIListBoxID::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar(*this->m_context);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 13), 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar(*this->m_context);
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 13), horizontalHeight);
  horizontal->Move(0, (height - 13));
  horizontal->Slot15((width - 13), horizontalHeight);
  horizontal->Slot22((width - 13), horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x00887280
// FUNCTION: fn_11332
void UITableBox::Slot15(int, int) {
}

// OFFSET: 0x008872B0
// FUNCTION: fn_11335
void UITreeView::Slot15(int width, int height) {
  auto* vertical = new UIScrollBar3(*this->m_context);
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 10), 0);
  vertical->Slot15(verticalWidth, height);
  vertical->Slot22(verticalWidth, height);
  this->AddChild(this->verticalBar);

  this->edit = new UIEditCtrl2(*this->m_context, *this->m_edit);
  this->edit->SetSize(100, 19);
  this->edit->Move(-100, -100);
  this->AddChild(this->edit);
}

// OFFSET: 0x008873F0
// FUNCTION: fn_11336
void TableBox::Paint() {
  RECT rectangle;
  rectangle.left = 1;
  rectangle.top = 1;
  rectangle.right = this->geometry.width - 1;
  rectangle.bottom = this->geometry.height - 1;
  this->FillSurface(this->borderColor, &rectangle);
}

// OFFSET: 0x008870B0
// FUNCTION: fn_11331
void UISlotBmpButton::Slot15(int, int) {
  const std::string path = "유저인터페이스\\renewalparty\\";
  char name[256];
  std::sprintf(name, "%sicon_jobs_%d.bmp", path.c_str(), 0);
  this->icon = new UIStaticBitmap(*this->m_context, *this->m_buttonContext);
  this->icon->SetBitmap(name);
  this->icon->SetSize(this->icon->geometry.width, this->icon->geometry.height);
  this->icon->Move(118, 14);
  this->AddChild(this->icon);
}

// OFFSET: 0x00899BE0
// FUNCTION: fn_11560
int UITableBox::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x008805E0
// FUNCTION: fn_11276
int UIListBoxNavi::LineOffset() {
  return this->lineOffset;
}

// OFFSET: 0x0088AB70
// FUNCTION: fn_11356
void UIComboBoxV2Wnd::Paint() {
  UIWindowContext& context = *this->m_context;
  this->ClearSurface(0xFFFF00FF);

  if (!this->text.empty()) {
    const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->text.c_str())));
    this->DrawBitmap(0, 0, &bitmap->image, false);
  }

  const std::string selected = this->SelectedText();
  this->DrawTextAt(context.fonts, context.locale, 2, 3, selected.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x00880CC0
// FUNCTION: fn_11289
std::string UIComboBoxV2Wnd::SelectedText() {
  int index = this->selected;

  if (index < 0 || index >= static_cast<int>(this->list->lines.size())) {
    return std::string();
  }

  const std::string& text = this->list->lines[index].text;
  return std::string(text.c_str(), text.size());
}

// OFFSET: 0x00880FF0
// FUNCTION: fn_11293
void UITreeView::HideNodes(std::list<UITreeNode*>& nodes) {
  if (static_cast<int>(nodes.size()) <= 0) {
    return;
  }

  for (UITreeNode* node : nodes) {
    if (node->collapseButton) {
      node->collapseButton->Move(-100, -100);
    }

    if (node->expandButton) {
      node->expandButton->Move(-100, -100);
    }

    node->labelButton->Move(-100, -100);

    if (static_cast<int>(node->children.size()) > 0) {
      this->HideNodes(node->children);
    }
  }
}

// OFFSET: 0x00881110
// FUNCTION: fn_11296
void UITreeView::AddNodeButtons(UITreeNode* node, int id) {
  const char* names[] = {"battle_option_b.bmp", "wnd_mini_b.bmp", nullptr};
  std::string path = "유저인터페이스\\basic_interface\\";

  for (int index = 0; index < 2; ++index) {
    UITVBitmapButton* button = new UITVBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((path + names[index]).c_str(), 0, 0);
    button->SetBitmap((path + names[index]).c_str(), 1, 0);
    button->SetBitmap((path + names[index]).c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(-100, -100);
    button->SetId(id);
    button->m_value = reinterpret_cast<int>(node);
    this->AddChild(button);

    if (this->itemHeight < this->itemPadding + button->BitmapHeight()) {
      this->itemHeight = this->itemPadding + button->BitmapHeight();
    }

    if (index == 0) {
      node->expandButton = button;
    } else {
      node->collapseButton = button;
    }
  }
}

// OFFSET: 0x008814C0
// FUNCTION: fn_11297
void UITreeView::AddNodeLabel(UITreeNode* node, int id) {
  UITVTextButton* button = new UITVTextButton(*this->m_context, *this->m_controls);
  button->SetSize(1, 1);
  button->SetLabel(node->name);
  button->ResizeToText();
  button->Move(-100, -100);
  button->SetId(id);
  button->m_value = reinterpret_cast<int>(node);
  this->AddChild(button);

  if (this->itemHeight < this->itemPadding + button->geometry.height) {
    this->itemHeight = this->itemPadding + button->geometry.height;
  }

  node->labelButton = button;
}

// OFFSET: 0x0089E480
// FUNCTION: fn_11601
int UITextButton::GetLinesCount() const {
  return static_cast<int>(this->m_lines.size());
}

// OFFSET: 0x00884710
// FUNCTION: fn_11309
void __stdcall Nullsub275(int, int, int, int) {
}

#include "UI/Buttons/UIDescriptRace.h"
#include "UI/Window/InputFocus.h"

// OFFSET: 0x008901E0
// FUNCTION: fn_11405
void UIListBoxID::Slot26(int, int) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->lines[this->selected].id, 0, 0);
  }
}

// OFFSET: 0x00890230
// FUNCTION: fn_11406
void UINewComboBoxCtrl::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 124, this->selected, 0, 0, 0);
  }
}

// OFFSET: 0x00890480
// FUNCTION: fn_11416
void UIDescriptRace::Slot25(int, int) {
  if (UIWindow* window = g_windowMgr.LookupWindow(278)) {
    window->SendMsg(this, 6, 204, 0, 0, 0);
  }
}

// OFFSET: 0x0088ACF0
// FUNCTION: fn_11358
void UIDescriptRace::Paint() {
  UIBitmapButton::Paint();
  this->sub_8A0B10();
}

// OFFSET: 0x00890330
// FUNCTION: fn_11410
void UITabControl::Slot26(int, int) {
  if (this->layout == 0 || this->layout == 2 || !this->unknown_00A8) {
    return;
  }

  this->edit->SetText(this->labels[this->unknown_007C].c_str());
  this->edit->Resize(this->widths[this->unknown_007C] - 4, this->geometry.height - 2);

  int x = 0;
  for (int index = 0; index < this->unknown_007C; ++index) {
    x += this->widths[index] - 1;
  }

  this->edit->Move(x + 2, 2);
  InputFocus focus{g_windowMgr.hoverWindow};
  focus.Set(this->edit);
}

// OFFSET: 0x0088C6E0
// FUNCTION: fn_11373
void UINewComboBoxCtrl::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, RGB(this->red, this->green, this->blue));

  if (!this->borderless) {
    DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, RGB(this->borderRed, this->borderGreen, this->borderBlue));
  }
}

// OFFSET: 0x0088EB70
// FUNCTION: fn_11384
void UIShowAndEditTimeCtrl::Paint() {
  UIWindowContext& context = *this->m_context;
  this->ClearSurface(0xFFFF00FF);
  this->DrawTextAt(context.fonts, context.locale, 22, 3, "m", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(context.fonts, context.locale, 60, 3, "s", 0, 0, 12, 0, 0, 0);
}

#include "UI/Buttons/UINaviLinkButton.h"

// OFFSET: 0x008906C0
// FUNCTION: fn_11423
void __fastcall sub_8906C0(UINaviLinkButton* button, void*, int x, int y) {
  button->UINaviLinkButton::Slot25(x, y);
}

// OFFSET: 0x0088AEC0
// FUNCTION: fn_11360
void UIImgMenu::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0x99A8AC);
  this->DrawBitmap(
    1, 1, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())))->image, false
  );

  if (this->hovered < 0) {
    return;
  }

  char suffix[12];
  std::sprintf(suffix, "_%d.bmp", this->hovered);
  const std::string path = this->baseName + suffix;
  const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));

  int offset = 3;
  if (this->unknown_0090[this->hovered] == 0) {
    offset = 0;
  }

  if (this->unknown_0094[this->hovered] == 0) {
    int x;
    int y;
    if (this->horizontal == 0) {
      x = 1;
      y = this->itemHeight * this->hovered + 1 + offset;
    } else {
      x = this->itemWidth * this->hovered + 1 + offset;
      y = 1;
    }

    if (bitmap) {
      this->DrawBitmap(x, y, &bitmap->image, false);
    }
  }
}

// OFFSET: 0x0089B760
// FUNCTION: fn_11571
void UIScrollBar_Custom::SetArrowBitmaps(std::string up, std::string down) {
  this->m_arrowUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(up.c_str())));
  this->m_arrowDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(down.c_str())));
  this->Invalidate();
}

// OFFSET: 0x0089B910
// FUNCTION: fn_11572
void UIScrollBar_Custom::SetThumbBitmaps(std::string top, std::string middle, std::string bottom) {
  this->m_thumbTop = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(top.c_str())));
  this->m_thumbMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(middle.c_str())));
  this->m_thumbBottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(bottom.c_str())));
  this->Invalidate();
}

#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Dialogs/UITipboxWnd.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CLoginMode.h"

// OFFSET: 0x00884720
// FUNCTION: fn_11310
void UIItemTagOnChat::PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int) {
  UIEditCtrl* edit = static_cast<UIEditCtrl*>(g_windowMgr.GetHoverWindow());
  if (!edit) {
    return;
  }

  const int y = edit->textY;
  if (x != 0) {
    button->Move(x, y - 1);
  } else {
    int bytes = 0;
    button->Move(edit->MeasureText(name.c_str(), bytes) + 2, y - 1);
  }
}

// OFFSET: 0x00884870
// FUNCTION: fn_11311
void UIItemTagOnMail::PlaceTagButton(UIWindow* button, const std::string& name, const int&, int) {
  if (!g_windowMgr.unknown_0458) {
    return;
  }

  unsigned line = 0;
  int consumed = 0;
  UIRodexComposeWnd* compose = reinterpret_cast<UIRodexComposeWnd*>(g_windowMgr.unknown_0458);
  const int left = compose->m_contents->geometry.x;
  const int top = compose->m_contents->geometry.y;
  const int lineHeight = compose->m_contents->unknown_0138;
  std::string rest(name.c_str(), name.size());
  const int firstLine = compose->m_contents->unknown_0158;
  const int visibleLines = compose->m_contents->geometry.height / lineHeight;

  for (; line < this->m_lines.size(); ++line) {
    const std::size_t length = this->m_lines[line].size();
    if (rest.size() < length) {
      break;
    }

    rest = rest.substr(length);
    unsigned bytes = 0;
    consumed += compose->MeasureText(this->m_lines[line], bytes);
  }

  unsigned bytes = 0;
  const int width = compose->MeasureText(name.c_str(), bytes);
  const int row = static_cast<int>(line) - firstLine;
  if (row < 0 || static_cast<int>(line) >= firstLine + visibleLines) {
    button->Move(1000, 1000);
  } else {
    button->Move(left + 2 + (width - consumed), row * lineHeight + top);
  }
}

// OFFSET: 0x00884C40
// FUNCTION: fn_11312
void UIItemTagOnMegaPhone::PlaceTagButton(UIWindow* button, const std::string& name, const int& x, int) {
  UINpcEditDialogWnd* owner = reinterpret_cast<UINpcEditDialogWnd*>(g_windowMgr.unknown_0318);
  if (!owner || owner->edit->geometry.id != 492) {
    return;
  }

  const int left = owner->edit->geometry.x;
  const int top = owner->edit->geometry.y;
  int position = x;
  unsigned bytes = 0;
  const int width = reinterpret_cast<UIItemTagCtrl*>(owner)->MeasureText(name.c_str(), bytes);
  if (owner->edit->geometry.width < this->textWidth + width) {
    position = 1000;
  } else if (position == 0) {
    position = left + 2 + width;
  }

  button->Move(position, top + 1);
}

// OFFSET: 0x0088AC80
// FUNCTION: fn_11357
void UIComboBox_BmpWnd::Paint() {
  this->ClearSurface(RGB(this->red, this->green, this->blue));
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->name.c_str())))->image, false);
}

// OFFSET: 0x008904C0
// FUNCTION: fn_11417
void UIItemTagButton::Slot25(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (this->m_value == 517) {
    if (UITipboxWnd* tipbox = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(315))) {
      tipbox->ShowTip(this->unknown_01F8);
    }
    return;
  }

  if (x < 0 || this->geometry.width <= x || y < 0 || this->geometry.height <= y) {
    this->geometry.unknown_0030 = 0;
  } else {
    this->geometry.unknown_0030 = 1;
    if (this->parent) {
      this->parent->SendMsg(this, 98, this->geometry.id, this->m_value, reinterpret_cast<std::intptr_t>(this->unknown_00F8), 0);
      this->Invalidate();
      return;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x00890AC0
// FUNCTION: fn_11433
void UIComboBox::Slot31(int x, int y) {
  this->selected = this->LineAt(x, y);
  if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
    target->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
    g_windowMgr.CloseWindow(26);
    return;
  }

  if (this->parent) {
    this->parent->SendMsg(nullptr, 39, this->selected, 0, 0, 0);
  }

  g_windowMgr.CloseWindow(26);
}

// OFFSET: 0x00890C20
// FUNCTION: fn_11436
void UIComboBoxID::Slot31(int x, int y) {
  int line = -1;

  if (x >= 0 && y >= 0 && x < this->geometry.width && y < this->geometry.height) {
    const int index = y / this->lineHeight + this->topLine;

    if (index >= 0 && static_cast<unsigned>(index) < this->lines.size()) {
      line = index;
    }
  }

  this->selected = line;

  if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
    int extra = 0;
    const auto found = this->unknown_00C4.find(this->selected);

    if (found != this->unknown_00C4.end()) {
      extra = found->second;
    }

    target->SendMsg(nullptr, 39, this->lines[this->selected].id, extra, 0, 0);
  } else if (this->parent) {
    int extra = 0;
    const auto found = this->unknown_00C4.find(this->selected);

    if (found != this->unknown_00C4.end()) {
      extra = found->second;
    }

    this->parent->SendMsg(nullptr, 39, this->lines[this->selected].id, extra, 0, 0);
  }

  g_windowMgr.CloseWindow(26);
}

// OFFSET: 0x00892990
// FUNCTION: fn_11480
void UIBasicSkillCtrl::Slot32(int x, int y) {
  if (this->m_skillId < 0) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(46);
  window->SendMsg(nullptr, 61, this->m_skillId, 0, 0, 0);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  window->Move(screenX + x + 24, screenY + y);
}

// OFFSET: 0x0089BA90
// FUNCTION: fn_11576
void UISlotBmpButton::SetSlot(int slot, int fallback) {
  this->m_slot = slot;
  this->m_inRange = g_session.IsWithinUnknown59C0(slot == -1 ? fallback : slot);
  if (this->m_slot < 0) {
    this->icon->Show(0);
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, this->m_slot, 0, 0, 0));
  if (!info) {
    this->icon->Show(0);
    return;
  }

  const std::string directory = "유저인터페이스\\renewalparty\\";
  char path[256];
  std::sprintf(path, "%sicon_jobs_%d.bmp", directory.c_str(), info->job);
  this->icon->Show(1);
  this->icon->SetBitmap(path);
}

#include "Network/TextEncoding.h"

// OFFSET: 0x0088BCD0
// FUNCTION: fn_11368
void UIListBoxID::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(RGB(this->unknown_007C[0], this->unknown_007C[4], this->unknown_007C[8]));
  }

  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  std::uint32_t color = 0;
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  for (int index = first; index < static_cast<int>(end); ++index) {
    if (index == this->selected) {
      this->FillRegion(0, (index - first) * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    DrawColoredText(
      *this, static_cast<unsigned short>(g_textCodePage), 3 - this->horizontalOffset, (index - first) * this->lineHeight, this->lines[index].text.c_str(),
      static_cast<unsigned>(this->lines[index].text.size()), color, 0, 12, false
    );
  }
}

// OFFSET: 0x0088A7D0
// FUNCTION: fn_11355
void UIComboBoxID::Paint() {
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp")))->image, false);
  const auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, &leftDown->image, false);
  const auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, &rightUp->image, false);
  const auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, &rightDown->image, false);

  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  for (int index = first; index < static_cast<int>(end); ++index) {
    if (index == this->selected) {
      SkinPaletteChannels(ResourceMgr::Instance(), 6, 2, red, green, blue);
      this->FillRegion(3, (index - first) * this->lineHeight + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }

    this->DrawTextAt(
      this->m_context->fonts, this->m_context->locale, 3 - this->horizontalOffset, (index - first) * this->lineHeight + 2, this->lines[index].text.c_str(), 0, 0, 12,
      0, 0, 0
    );
  }

  if (this->verticalOverflow != 0) {
    SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
    this->FillRegion(this->geometry.width - 16, 0, 16, this->geometry.height, RGB(red, green, blue));
  }

  this->parent->Invalidate();
}

// OFFSET: 0x0088B120
// FUNCTION: fn_11362
void UIItemStoreComboBox::Paint() {
  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  if (first < static_cast<int>(end)) {
    char path[260];
    for (int index = first; index < static_cast<int>(end); ++index) {
      if (index == this->selected) {
        if (!this->pink) {
          std::sprintf(path, "유저인터페이스\\basic_interface\\combobox_blue%d.bmp", this->selected);
        } else if (this->pink == true) {
          std::sprintf(path, "유저인터페이스\\basic_interface\\combobox_pink%d.bmp", this->selected);
        }

        this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)))->image, false);
      }
    }

    for (int index = first; index < static_cast<int>(end); ++index) {
      this->DrawTextAt(
        this->m_listContext->fonts, this->m_listContext->locale, 3 - this->horizontalOffset, (index - first) * this->lineHeight + 2, this->lines[index].text.c_str(), 0,
        0, 12, this->lines[index].color, 0, 0
      );
    }
  }

  this->Invalidate();
}

// OFFSET: 0x0088B5A0
// FUNCTION: fn_11365
void UIListBox2::Paint() {
  if (this->transparent != 0) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(RGB(this->backgroundRed, this->backgroundGreen, this->backgroundBlue));
  }

  const int first = this->topLine;
  const unsigned count = static_cast<unsigned>(this->lines.size());
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  std::uint32_t color = this->textColor;
  if (count < end) {
    end = count;
  }

  auto& owner = *this->m_listContext;
  for (int row = 0; row + first < static_cast<int>(end); ++row) {
    const int index = row + first;
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    const int x = 3 - this->horizontalOffset;
    int y = this->lineHeight * row;
    if (this->centerVertically == 1) {
      const int lineHeight = this->lineHeight;
      const int height = this->TextHeight(owner.fonts, owner.locale, this->lines[index].c_str(), 0, 0, 12, 0, 0);
      y = static_cast<int>(static_cast<float>(lineHeight - height) * 0.5f + static_cast<float>(row * lineHeight));
    }

    const std::string& line = this->lines[index];
    DrawStyledColoredText(
      *this, owner.codePage, x, y, line.c_str(), static_cast<unsigned>(line.size()), color, this->fontFamily, this->fontHeight, this->bold, this->italic, false
    );
  }
}

#include "UI/Gauges/UIPcGage.h"

// OFFSET: 0x0088C630
// FUNCTION: fn_11372
void UIMonsterGage::Paint() {
  this->ClearSurface(this->colors[0]);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->colors[1]);
  if (this->maximum > 0) {
    const float percent = static_cast<float>(this->current) * 100.0f / static_cast<float>(this->maximum);
    if (0.0f < percent) {
      const std::uint32_t color = static_cast<float>(this->m_threshold) > percent ? this->colors[4] : this->colors[2];
      this->FillRegion(
        1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * percent / 100.0f), this->geometry.height - 2, color
      );
    }
  }
}

// OFFSET: 0x0088C750
// FUNCTION: fn_11374
void UIPcGage::Paint() {
  this->ClearSurface(this->colors[0]);
  if (this->unknown_009C == 0) {
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->colors[1]);
    if (this->maximum != 0) {
      const float percent = static_cast<float>(this->current * 100 / this->maximum);
      if (0.0f != percent) {
        const int width = static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f);
        if (25.0f > percent) {
          this->FillRegion(1, 1, width, this->geometry.height - 2, this->colors[4]);
        } else {
          this->FillRegion(1, 1, width, this->geometry.height - 2, this->colors[2]);
        }
      }
    }
  } else {
    this->FillRegion(1, 1, this->geometry.width - 2, (this->geometry.height - 3) / 2, this->colors[1]);
    this->FillRegion(1, this->geometry.height / 2 + 1, this->geometry.width - 2, (this->geometry.height - 3) / 2, this->colors[1]);
    if (this->maximum != 0 && this->unknown_00AC != 0) {
      const float percent = static_cast<float>(this->current * 100 / this->maximum);
      const float second = static_cast<float>(this->unknown_00A8 * 100 / this->unknown_00AC);
      if (percent != 0.0f) {
        this->FillRegion(
          1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), (this->geometry.height - 3) / 2,
          25.0f > percent ? this->colors[4] : this->colors[2]
        );
      }
      if (second != 0.0f) {
        this->FillRegion(
          1, this->geometry.height / 2 + 1, static_cast<int>(static_cast<float>(this->geometry.width) * second / 100.0f - 2.0f),
          (this->geometry.height - 3) / 2, this->colors[3]
        );
      }
    }
  }
}

// OFFSET: 0x0088D6C0
// FUNCTION: fn_11377
void UIRechargeGage::Paint() {
  this->ClearSurface(this->colors[0]);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->colors[1]);
  if (this->maximum != 0) {
    const float percent = static_cast<float>(this->current * 100 / this->maximum);
    if (0.0f != percent) {
      this->FillRegion(
        1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), this->geometry.height - 2, this->colors[2]
      );
    }
  }
}

// OFFSET: 0x00890DC0
// FUNCTION: fn_11438
void UIImgMenu::Slot31(int x, int y) {
  this->hovered = this->ItemAt(x, y);
  if (this->hovered != -1 && !this->unknown_0094[this->hovered]) {
    if (UIWindow* target = g_windowMgr.LookupWindow(this->unknown_00C8)) {
      target->SendMsg(nullptr, 48, this->hovered, this->geometry.id, 0, 0);
    }
    reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x7A);
  }
}

#include "Contents/Items/ItemInfo.h"

// OFFSET: 0x00890EE0
// FUNCTION: fn_11441
void UIString::Slot31(int, int) {
  if (!this->unknown_0084.empty()) {
    UIWindow* window = g_windowMgr.EnsureWindow(201);
    window->SendMsg(nullptr, 62, reinterpret_cast<std::intptr_t>(this->unknown_0084.c_str()), 1, 0, 0);
    UIWindow* frame = g_windowMgr.LookupWindow(10008);
    UIWindow* parent = this->parent;
    window->Move(
      frame->geometry.x + (parent->geometry.x + 100 + this->geometry.x), frame->geometry.y + (parent->geometry.y - 60 + this->geometry.y)
    );
  } else if (!this->unknown_00AC.empty()) {
    ITEM_INFO item;
    item.itemId = this->unknown_00AC;
    g_windowMgr.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0088F6C0
// FUNCTION: fn_11390
void UITabControl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->SendMsg(this, 116, 0, 0, 0, 0);
  }

  for (int index = 0; index < static_cast<int>(this->labels.size()); ++index) {
    const int layout = this->layout;
    if (layout == 1 || layout == 3) {
      this->DrawHorizontalTab(index);
    } else if (layout == 0 || layout == 2) {
      this->DrawVerticalTab(index);
    }

    if (this->flags.size() >= static_cast<unsigned>(static_cast<int>(this->labels.size())) && this->flags[index]) {
      int offset = 0;
      for (int previous = 0; previous < index; ++previous) {
        offset += this->widths[previous] - 1;
      }

      const auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\icon_new.bmp")));
      this->DrawBitmap(1, static_cast<unsigned short>(offset) + 2, bitmap ? &bitmap->image : nullptr, false);
    }
  }
}

#include "UI/Text/UITextViewer.h"

// OFFSET: 0x0088F8D0
// FUNCTION: fn_11393
void UITextViewEX::Paint() {
  if (this->transparent) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(this->backgroundColor);
  }

  const int top = this->topLine;
  int y = 7;
  int line = 0;
  for (auto it = this->lineLinks.begin(); it != this->lineLinks.end(); ++it, ++line) {
    std::vector<UITextButton*> buttons = *it;
    if (line >= top) {
      int x = 0;
      for (UITextButton* button : buttons) {
        UIString* text = reinterpret_cast<UIString*>(button);
        if (text->unknown_00DC != "\n") {
          text->Show(1);
          x = text->unknown_0080 == 0 ? x : x - 5;
          if (x < 0) {
            x = 0;
          }
          text->Move(x, !text->unknown_00AC.empty() ? y - 7 : y);
          x = text->geometry.width + x + 5;
        }
      }
      y += 21;
    } else {
      for (UITextButton* button : buttons) {
        button->Show(0);
      }
    }
  }

  this->Refresh();
}

#include "UI/Window/UIRPImageWnd.h"

// OFFSET: 0x0088D560
// FUNCTION: fn_11376
void UIRPImageWnd::Paint() {
  this->SetSolid(this->m_context->primitives, this->geometry.width, this->geometry.height, 0);
  for (int i = 0; i < this->m_imageCount; ++i) {
    this->DrawNamed(this->m_context->primitives, 0, 0, this->m_imageName.c_str(), 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x0086EBB0
// FUNCTION: fn_11081
UITVBitmapButton::UITVBitmapButton()
    : UIBitmapButton(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIButtonContext*>(nullptr)), m_value(0) {
}

// OFFSET: 0x0086EC10

#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/TipOfTheDay.h"

// OFFSET: 0x0086DE10

// OFFSET: 0x008894B0
// FUNCTION: fn_11347
void UIChatHisBox2::Paint() {
  this->ClearSurface(RGB(this->backgroundRed, this->backgroundGreen, this->backgroundBlue));

  const int top = this->topLine;
  unsigned int end = static_cast<unsigned int>(this->geometry.height / this->lineHeight - this->bottomMargin + top);
  if (static_cast<unsigned int>(this->lines.size()) < end) {
    end = static_cast<unsigned int>(this->lines.size());
  }

  FontCache& fonts = this->m_listContext->fonts;
  const TextLocale& locale = this->m_listContext->locale;
  for (int row = 0; top + row < static_cast<int>(end); ++row) {
    const int index = top + row;
    int x;
    if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
      const int width = this->TextWidth(fonts, locale, this->lines[index].c_str(), 0, 0, 12, 0, 0);
      x = this->geometry.width - width - 19;
    } else {
      x = 2;
    }
    this->DrawTextAt(fonts, locale, x, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->unknown_00E4[index], 0, 0);
    this->PlaceNaviLinks(index);
    this->PlaceItemTags(index);
  }
}

// OFFSET: 0x00889600
// FUNCTION: fn_11348
void UIChatHisBox3::Paint() {
  if (this->transparent) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(RGB(this->UIListBox::backgroundRed, this->UIListBox::backgroundGreen, this->UIListBox::backgroundBlue));
  }

  const int top = this->topLine;
  unsigned int end = static_cast<unsigned int>(this->geometry.height / this->lineHeight - this->bottomMargin + top);
  if (static_cast<unsigned int>(this->lines.size()) < end) {
    end = static_cast<unsigned int>(this->lines.size());
  }

  FontCache& fonts = this->m_listContext->fonts;
  const TextLocale& locale = this->m_listContext->locale;
  for (int row = 0; top + row < static_cast<int>(end); ++row) {
    const int index = top + row;
    this->DrawTextAt(fonts, locale, 2, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, this->unknown_0134, this->unknown_00E4[index], 0, 0);
    this->PlaceNaviLinks(index);
    this->PlaceItemTags(index);
  }
}

// OFFSET: 0x0088FDC0
// FUNCTION: fn_11395
void UITreeView::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->Invalidate();
  }

  this->HideNodes(this->root->children);
  this->itemCount = 0;
  this->LayoutNodes(this->root->children, 0, 0);
  this->Slot53();
}
