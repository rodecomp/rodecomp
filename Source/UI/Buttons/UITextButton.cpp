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

#include <cstdio>
#include <cstring>
#include <string>

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
