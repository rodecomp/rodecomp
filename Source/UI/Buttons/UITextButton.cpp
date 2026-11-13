#include "Client/Input/InputCharset.h"
#include "Contents/Skin/SkinMgr.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Render/Renderer.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "Render/Text/FontCache.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "UI/Gauges/UIBarGraph.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "Render/Text/TextContext.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Items/UIItemMoveInfo.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Lists/UIImgMenu.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Lists/UITreeView.h"
#include "UI/Buttons/UIStaticItem.h"
#include "Client/Session/Session.h"
#include "Contents/Hotkeys/HotkeyInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Window/UIBasicCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/Buttons/UIBitmapButton2.h"
#include "UI/Buttons/UISlotBmpButton.h"
#include "UI/Buttons/UIStaticBitmap.h"
#include "UI/Buttons/UITVBitmapButton.h"
#include "UI/Buttons/UITVTextButton.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIComboBoxV2Wnd.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/SelectPackageItemBox/UIPackageBoxNameList.h"
#include "UI/Lists/UIListBoxEx.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "UI/Lists/TableBox.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Text/UIString.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "Client/Input/Language.h"
#include "Utility/Text/GroupedInteger.h"
#include "Client/Input/Mouse.h"
#include "Contents/Items/ItemDescription.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/WindowBorder.h"
#include "Resource/Bitmap/SkinPalette.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Resource/MSGSTRINGID.h"

// OFFSET: 0x0086EEB0
// FUNCTION: fn_11085
UITextButton::UITextButton() : UIButton() {
  this->m_value = 0;
  this->m_outlineColor = 0xFFFFFF;
}

// OFFSET: 0x00871D60
// FUNCTION: fn_11110
UITextButton::~UITextButton() = default;

namespace {
  // INLINED
  void SetButtonLabel(UIButton& button, std::string text) {
    button.label = text;
  }

  // INLINED
  __forceinline int TabOffset(const UITabControl& control, int count) {
    int offset = 0;

    for (int index = 0; index < count; ++index) {
      offset += control.widths[index] - 1;
    }

    return offset;
  }
}

// OFFSET: 0x0087C590
// FUNCTION: fn_11254
void UITextButton::Configure(const char* text, std::uint32_t color, int family, int height, unsigned char outline, unsigned char underline) {
  SetButtonLabel(*this, text);
  this->m_text = this->label;
  this->m_normalColor = color;
  this->m_hoverColor = color;
  this->m_color = color;
  this->m_family = family;
  this->m_height = height;
  this->m_outline = outline;
  this->m_underline = underline;
  const auto extent = static_cast<std::uint32_t>(height) * static_cast<std::uint32_t>(std::strlen(text));
  const int newHeight = height + 2;
  const int width = static_cast<int>(extent / 2) + 4 * outline + 12;
  this->UIWindow::Resize(width, newHeight);
  this->OnCreate(width, newHeight);
  this->OnSize(width, newHeight);
}

// OFFSET: 0x0088F820
// FUNCTION: fn_11392
void UITextButton::Paint() {
  this->ClearSurface(0xFFFF00FFu);
  if (this->parent) {
    this->parent->Invalidate();
  }

  if (this->m_outline) {
    this->DrawOutlinedText(1, 1, this->m_text.c_str(), 0, this->m_color, this->m_outlineColor, this->m_family, this->m_height, 1, 0);
  } else {
    this->DrawTextAt(1, 1, this->m_text.c_str(), 0, this->m_family, this->m_height, this->m_color, 0, 0);
  }

  if (this->m_underline) {
    this->FillRegion(0, (this->geometry.height - 1), this->geometry.width, 1, this->m_color);
  }
}

// OFFSET: 0x00892450
// FUNCTION: fn_11471
void UITextButton::OnMouseMove(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (g_windowMgr.Capture() == this) {
    if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
      this->m_color = this->m_hoverColor;
      if (this->geometry.state != 2) {
        this->geometry.state = 2;
        this->Invalidate();
      }
    } else {
      this->m_color = this->m_normalColor;
      if (this->geometry.state != 0) {
        this->geometry.state = 0;
        this->Invalidate();
      }
    }
  } else if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    this->m_color = this->m_hoverColor;
    if (this->geometry.state != 1) {
      this->geometry.state = 1;
      this->Invalidate();
    }
  } else {
    this->m_color = this->m_normalColor;
    if (this->geometry.state != 0) {
      this->geometry.state = 0;
      this->Invalidate();
    }
  }
}

// OFFSET: 0x00892000
// FUNCTION: fn_11461
void UITextButton::OnMouseHover(int x, int y) {
  if (this->parent) {
    this->parent->OnMouseHover((x + this->geometry.x), (y + this->geometry.y));
  }

  UIButton::OnMouseHover(x, y);
}

// OFFSET: 0x00891140
// FUNCTION: fn_11445
void UITextButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_LBTN_UP, this->geometry.id, this->m_value, 0, 0);
      g_windowMgr.ReleaseCapture();
      return;
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892B60
// FUNCTION: fn_11487
void UITextButton::OnRBtnDown(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_RBTN_UP, this->geometry.id, this->m_value, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892D60
// FUNCTION: fn_11494
void UITextButton::OnRBtnUp(int x, int y) {

  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_RBTN_UP, this->geometry.id, this->m_value, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892F30
// FUNCTION: fn_11500
void UITextButton::OnWheel(int value) {
  if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x008911D0
// FUNCTION: fn_11446
void UITreeView::OnLBtnUp(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    this->selectedNode = 0;
  }

  this->dragNode = 0;
  this->dropNode = 0;
  this->mouseInside = 0;
  this->Invalidate();

  if (this->parent) {
    this->parent->OnLBtnUp(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00891A80
// FUNCTION: fn_11455
void UIItemTagButton::OnMouseHover(int x, int y) {
  std::string tag = "<";
  tag += this->m_tooltip;
  tag += ">";

  if (this->label != tag) {
    UIButton::OnMouseHover(x, y);
  }
}

// OFFSET: 0x00891F10
// FUNCTION: fn_11459
void UIStaticItem_Illust::OnMouseHover(int, int) {
}

// OFFSET: 0x00891810
// FUNCTION: fn_11450
void UIBmpRadioBtn::OnMouseHover(int, int) {
  if (!this->m_tooltip.empty()) {
    int screenX = 0, screenY = -20;
    this->ToScreen(screenX, screenY);

    g_modeMgr.gameMode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x0089B640
// FUNCTION: fn_11570
bool UIBmpRadioBtn::SetBitmap(const char* name, int state) {
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
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
void UIBmpRadioBtn::AddText(int id, int height, std::uint32_t color, bool bold, bool drawShadow, int x, int y) {
  UIBmpRadioBtnText text;
  text.id = id;
  text.x = x;
  text.y = y;
  text.height = height;
  text.color = color;
  text.bold = bold;
  text.drawShadow = drawShadow;

  if (x == -1000) {
    const int textWidth =
      this->TextWidth(this->m_label.c_str(), static_cast<unsigned>(this->m_label.size()), 0, height, bold, 0);
    text.x = static_cast<int>((this->geometry.width - textWidth) * 0.5);
  }

  if (y == -1000) {
    const int textHeight =
      this->TextHeight(this->m_label.c_str(), static_cast<unsigned>(this->m_label.size()), 0, height, bold, 0);
    text.y = static_cast<int>((this->geometry.height - textHeight) * 0.5);
  }

  const auto it = std::find_if(this->m_texts.begin(), this->m_texts.end(), [id](const UIBmpRadioBtnText& entry) { return id == entry.id; });

  if (this->m_texts.end() != it) {
    *it = text;
  }

  this->m_texts.push_back(text);
}

// OFFSET: 0x008816B0
// FUNCTION: fn_11298
void UIBmpRadioBtn2::Invalidate() {
  UIWindow::Invalidate();

  if (this->parent != nullptr && !this->parent->IsCached()) {
    this->parent->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
  }
}

// OFFSET: 0x007C7CA0
// FUNCTION: fn_09408
void UIBmpRadioBtn2::OnMouseEnter() {
  this->m_hovered = true;
  this->Invalidate();
}

// OFFSET: 0x007C7CB0
// FUNCTION: fn_09409
void UIBmpRadioBtn2::OnMouseLeave() {
  this->m_hovered = false;
  this->Invalidate();
}

// OFFSET: 0x00891870
// FUNCTION: fn_11451
void UICaptchaImage::OnMouseHover(int, int) {
  if (this->m_tooltip.size() > 0) {
    int screenX = 0, screenY = -20;
    this->ToScreen(screenX, screenY);

    g_modeMgr.gameMode->ShowTooltip(this->m_tooltip.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x00891240
// FUNCTION: fn_11447
void UIBarGraph4::OnMouseHover(int, int) {
  if (!this->m_current || !this->m_maximum || *this->m_maximum < *this->m_current || *this->m_maximum == 0) {
    return;
  }

  int screenX = 0, screenY = -16;

  this->ToScreen(screenX, screenY);

  char text[260];
  std::memset(text, 0, sizeof(text));
  const int permille = static_cast<int>(static_cast<float>(*this->m_current) / static_cast<float>(*this->m_maximum) * 1000.0f);
  std::sprintf(text, "%.1f %%", static_cast<float>(permille) * 0.1f);
  g_modeMgr.gameMode->ShowTooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x008918D0
// FUNCTION: fn_11452
void UIINT64BarGraph::OnMouseHover(int, int) {
  if (this->m_maximum < this->m_current || 0 == this->m_maximum || this->m_maximum == 99999999) {
    return;
  }

  int screenX = 0, screenY = -16;

  this->ToScreen(screenX, screenY);

  char text[260];
  std::memset(text, 0, sizeof(text));
  const int permille = static_cast<int>(static_cast<double>(this->m_current) / static_cast<double>(this->m_maximum) * 1000.0);
  std::sprintf(text, "%.1f %%", static_cast<float>(permille) * 0.1f);
  g_modeMgr.gameMode->ShowTooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x00891A40
// FUNCTION: fn_11454
void UIItemMoveInfo::OnMouseHover(int, int) {
  g_windowMgr.EnsureWindow(WID_ROUNDBOXTOOLTIPWND)->SendMsg(nullptr, UM_SET_ITEM_MOVE_TOOLTIP, this->m_moveIndex, 0, 0, 0);
}

// OFFSET: 0x00891A10
// FUNCTION: fn_11453
void UIImgMenu::OnMouseHover(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->ItemAt(x, y);
  if (previous != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x00891F20
// FUNCTION: fn_11460
void UITabControl::OnMouseHover(int x, int y) {
  int index = this->TabAt(x, y);

  if (index == -1) {
    return;
  }

  const std::string& tooltip = this->tooltips[index];

  if (tooltip.empty() || this->labels[index] == tooltip) {
    return;
  }

  x = TabOffset(*this, index);
  y = -20;
  this->ToScreen(x, y);
  g_modeMgr.gameMode->ShowTooltip(this->tooltips[index].c_str(), x, y, -1, false, true);
}

// OFFSET: 0x00822660
// FUNCTION: fn_10280
std::string SkillUseTextPercent(const char* name, int level, int sp, int ap) {
  if (ap != 0) {
    return FormatOwnedString("%s Use Lv %d (Sp : %d%%, Ap : %d)", name, level, sp, ap);
  }

  return FormatOwnedString("%s Use Lv %d (Sp : %d%%)", name, level, sp);
}

// OFFSET: 0x008226F0
// FUNCTION: fn_10281
std::string SkillUseTextFlat(const char* name, int level, int sp, int ap) {
  if (ap != 0) {
    return FormatOwnedString("%s Use Lv %d (Sp : %d, Ap : %d)", name, level, sp, ap);
  }

  return FormatOwnedString("%s Use Lv %d (Sp : %d)", name, level, sp);
}

// OFFSET: 0x00891350
// FUNCTION: fn_11448
void UIBasicItemCtrl::OnMouseHover(int, int) {
  std::string text;

  if (std::atoi(this->m_item.itemId.c_str()) < 1) {
    text = this->m_tooltip;
  } else if (this->m_item.identity[1] > 0) {
    if (this->m_item.optionCount != 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_GATLING), this->m_item.DisplayName(0).c_str(), this->m_item.optionCount, this->m_item.quantity));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA2), this->m_item.DisplayName(0).c_str(), this->m_item.quantity));
    }
  } else {
    text = static_cast<const std::string&>(this->m_item.DisplayName(0));
  }

  int screenX = 0, screenY = 0;

  this->ToScreen(screenX, screenY);

  if (!text.empty()) {
    g_modeMgr.gameMode->ShowTooltip(text.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x008915E0
// FUNCTION: fn_11449
void UIBasicSkillCtrl::OnMouseHover(int, int) {
  if (this->m_skillId < 0) {
    return;
  }

  int screenX = 0, screenY = 0;

  this->ToScreen(screenX, screenY);

  LuaScript& script = *g_session.m_hotkeyScript;
  CSkillInfo skill = g_session.LookupSkill(static_cast<std::uint32_t>(this->m_skillId));
  const int sp = skill.UseSpAmount(script, skill.level);
  const int ap = skill.UseApAmount(script, skill.level);
  std::string text;

  if (SkillUsesPercentSp(static_cast<int>(skill.id))) {
    text = SkillUseTextPercent(skill.DisplayName(), skill.level, sp, ap);
  } else {
    text = SkillUseTextFlat(skill.DisplayName(), skill.level, sp, ap);
  }

  if (!text.empty()) {
    g_modeMgr.gameMode->ShowTooltip(text.c_str(), screenX, screenY, -1, false, true);
  }
}

// OFFSET: 0x00A8B9D0
// FUNCTION: fn_17125
void UIWindowMgr::AddRendered(UIWindow* window) {
  this->rendered.push_back(window);
}

// OFFSET: 0x00891CC0
// FUNCTION: fn_11457
void UIServerListBox::OnMouseHover(int x, int y) {
  const int index = this->LineAt(x, y);
  const int height = g_mouse.y;
  if (this->balloonTexts.begin() != this->balloonTexts.end()) {
    if (index != -1 && this->balloonTexts[index].size() != 0) {
      int screenX = 0;
      int screenY = 0;
      if (this->balloon != nullptr) {
        g_windowMgr.Schedule(this->balloon);
        this->balloon = nullptr;
      }

      this->balloon = new UIColorTransBalloonText();
      this->balloon->SetSize(8, 8);
      this->ToScreen(screenX, screenY);
      this->balloon->Move(screenX + 0x50, height - 6);
      g_windowMgr.AddRendered(this->balloon);
      this->balloon->SetText(this->balloonTexts[index].c_str(), 50);
      return;
    }

    g_windowMgr.Schedule(this->balloon);
    this->balloon = nullptr;
  }
}

// OFFSET: 0x00891BC0
// FUNCTION: fn_11456
void UIMemberListBox::OnMouseHover(int, int y) {
  const unsigned visible =
    std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + this->topLine), static_cast<unsigned>(this->lines.size()));
  const int line = this->topLine + y / this->lineHeight;
  if (line < this->topLine) {
    return;
  }

  if (line >= static_cast<int>(visible)) {
    return;
  }

  const char* text = this->lines[line].c_str();
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  if (width <= this->geometry.width - this->verticalOverflow * 15) {
    return;
  }

  int screenX = -4;
  y -= y % this->lineHeight;
  this->ToScreen(screenX, y);
  g_modeMgr.gameMode->ShowTooltip(this->lines[line].c_str(), screenX, y, -1, false, true);
}

// OFFSET: 0x00891E40
// FUNCTION: fn_11458
void UIStaticItem::OnMouseHover(int, int) {
  char text[260];
  std::memset(text, 0, sizeof(text));
  int capacity = sizeof(text);
  this->m_item.GetIdDisplayName(text, capacity, 0);

  if (std::strlen(text) == 0) {
    return;
  }

  int screenX = 0, screenY = -20;

  this->ToScreen(screenX, screenY);

  g_modeMgr.gameMode->ShowTooltip(text, screenX, screenY, -1, false, true);
}

// OFFSET: 0x00892040
// FUNCTION: fn_11462
void UIItemMoveInfo::OnMouseLeave() {
  g_windowMgr.ShowWindowById(WID_ROUNDBOXTOOLTIPWND, 0);
  this->Invalidate();
}

// OFFSET: 0x0087B550
// FUNCTION: fn_11242
void UIComboBox::Configure(int x, int y, int width, int height, unsigned char value) {
  this->transparent = value;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
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
void UIComboBox::OnCreate(int width, int height) {
  UIListBox::OnCreate(width, height);
}

// OFFSET: 0x00895AC0
// FUNCTION: fn_11509
void UIComboBox::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int overflow = 0 > static_cast<int>(this->lines.size()) - visible ? 0 : static_cast<int>(this->lines.size()) - visible;

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

      if (this->layout != 0 && this->layout != 2) {
        width = this->TextWidth(this->labels[index].c_str(), 0, 0, this->fontSize, 0, 0) + this->tabPadding;
      } else {
        width = this->VerticalTextHeight(this->labels[index].c_str(), 0, 0, this->fontSize, 0) + this->tabPadding;
      }

      if (width > widest) {
        widest = width;
      }
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
  if (oldWidth != this->geometry.width && flag) {
    this->RebuildHistory();
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

  this->displayText = this->label;
  const int width =
    this->TextWidth(this->label.c_str(), 0, this->m_fontFamily, this->m_fontHeight, this->m_outlined, 0);
  int resizedWidth = width + 2;
  int resizedHeight = this->m_fontHeight + 2;
  if (this->m_border) {
    resizedWidth += 2;
    resizedHeight += 2;
  }
  UIWindow::Resize(resizedWidth, resizedHeight);
}

// OFFSET: 0x00897960
// FUNCTION: fn_11537
void UITextButton::ResizeToText() {
  if (std::strcmp(this->label.c_str(), "") == 0) {
    return;
  }

  this->m_text = this->label;
  const int width = this->TextWidth(this->label.c_str(), 0, this->m_family, this->m_height, this->m_outline, 0);
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
__declspec(noinline) void UITabControl::FitToTabs() {
  if (this->layout != 0 && this->layout != 2) {
    this->Resize(TabOffset(*this, static_cast<int>(this->labels.size())) + 1, this->tabHeight + 1);
  } else {
    this->Resize(this->tabHeight + 1, TabOffset(*this, static_cast<int>(this->labels.size())) + 1);
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
  switch (message) {
  case UM_COMMAND: {
    if (argument1 != 273) {
      return 0;
    }

    this->Resize(
      this->geometry.width, this->geometry.height > this->auxiliaryWindow->geometry.height ? this->auxiliaryWindow->geometry.height
                                                                                         : this->auxiliaryWindow->geometry.height + this->list->geometry.height
    );

    if (this->parent) {
      this->parent->Invalidate();
    }

    this->Invalidate();
    return 0;
  }

  case UM_COMBOBOX_SELECTED:
    if (g_windowMgr.WindowPolicy(3, this->parent->geometry.id) == 0) {
      return 0;
    }

    this->SendMsg(nullptr, UM_COMMAND, ID_OPEN_MEMBER_POS_COMBO, 0, 0, 0);
    this->selected = argument1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_COMBOBOX_SELECTED, argument1, 0, 0, 0);
    }

    this->Invalidate();
    return 0;

  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x00898730
// FUNCTION: fn_11550
int UIImgMenu::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETINFO: {
    this->horizontal = argument1;
    this->count = argument2;
    std::string name(reinterpret_cast<const char*>(argument3));
    this->bitmapName = name + ".bmp";
    this->baseName = name;
    this->pressedItems = new bool[this->count];
    this->disabledItems = new bool[this->count];

    for (int index = 0; index < this->count; ++index) {
      this->pressedItems[index] = false;
      this->disabledItems[index] = false;
    }

    this->Invalidate();
    return 0;
  }

  case UM_SETID:
    this->geometry.id = argument1;
    return 0;

  case UM_SET_CALLER_WID:
    this->callerWindowId = argument1;
    return 0;

  case 105:
    this->pressedItems[argument1 - 1] = argument2 != 0;
    return 0;

  case UM_SET_DISABLE:
    this->disabledItems[argument1 - 1] = argument2 != 0;
    return 0;

  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x00897BF0
// FUNCTION: fn_11541
int UIBmpRadioBtn::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == UM_SETSTATE) {
    this->geometry.state = static_cast<std::uint32_t>(argument1);
    this->Invalidate();
  }
  return 0;
}

// OFFSET: 0x00892060
// FUNCTION: fn_11463
void UIComboBox::OnMouseMove(int x, int y) {
  const int previous = this->selected;
  this->selected = this->LineAt(x, y);

  if (previous != this->selected) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892100
// FUNCTION: fn_11465
void UIComboBoxID::OnMouseMove(int x, int y) {
  const int previous = this->selected;
  int line;

  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    line = -1;
  } else {
    const int index = y / this->lineHeight + this->topLine;
    line = index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
  }

  this->selected = line;

  if (previous != line) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892160
// FUNCTION: fn_11466
void UIImgMenu::OnMouseMove(int x, int y) {
  this->OnMouseHover(x, y);
}

// OFFSET: 0x00892170
// FUNCTION: fn_11467
void UIItemStoreComboBox::OnMouseMove(int x, int y) {
  const int previous = this->selected;
  int line;

  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    line = -1;
  } else {
    const int index = y / this->lineHeight + this->topLine;
    line = index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
  }

  this->selected = line;

  if (previous != line) {
    this->Invalidate();
  }
}

// OFFSET: 0x00892340
// FUNCTION: fn_11469
void UITVTextButton::OnMouseMove(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height) {
    if (this->geometry.state != 1) {
      this->geometry.state = 1;

      if (this->parent && this->mousePressed == 0) {
        this->parent->SendMsg(this, UM_MOUSE_HOVER_BTN, this->m_value, 0, 0, 0);
      }
    }
  } else if (this->geometry.state != 0) {
    this->geometry.state = 0;
    this->Invalidate();
    this->drawBackground = 0;
    this->highlighted = 0;
  }
}

// OFFSET: 0x008923D0
// FUNCTION: fn_11470
void UITabControl::OnMouseMove(int x, int y) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  if ((this->dragX == 0 && this->dragY == 0) || this->dragX - x > 5 || this->dragY - y > 5) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_DETACH_CHAT_TAB, this->activeTabIndex, 0, 0, 0);
    }

    this->dragX = x;
    this->dragY = y;
  }
}

// OFFSET: 0x00892520
// FUNCTION: fn_11472
void UITreeView::OnMouseMove(int x, int y) {
  this->mouseInside = x >= 0 && x < this->geometry.width && y >= 0 && y < this->geometry.height;
}

// OFFSET: 0x00892560
// FUNCTION: fn_11473
void UIImgMenu::ResetCursor(int, int) {
  g_modeMgr.gameMode->SetCursorAction(2);
}

// OFFSET: 0x008925A0
// FUNCTION: fn_11475
void UIString::ResetCursor(int, int) {
  if (!this->link.empty() || !this->image.empty()) {
    g_modeMgr.gameMode->SetCursorAction(2);
  }
}

// OFFSET: 0x0086EA50
// FUNCTION: fn_11080
UIString::UIString() : UIWindow(0), enabled(1), red(0), green(0), blue(0) {
  this->link.clear();
  this->image.clear();
  this->fontName.clear();
  this->text.clear();
}

// OFFSET: 0x008925D0
// FUNCTION: fn_11476
void UITVTextButton::ResetCursor(int x, int y) {
  if (this->m_disabled != 0) {
    UIWindow::ResetCursor(x, y);
  } else {
    g_modeMgr.gameMode->SetCursorAction(0);
  }
}

// OFFSET: 0x00892600
// FUNCTION: fn_11477
void UIEditCtrl_Currency::OnProcess() {
  if (this->formattedLength == this->text.size()) {
    return;
  }

  int anchor;
  int cursor;
  g_serverLanguage->Selection(anchor, cursor);

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

  g_serverLanguage->ResetInput();
  g_serverLanguage->ResetInput();
  g_serverLanguage->Insert(this->text.c_str());

  const int length = static_cast<int>(this->text.size());
  anchor += length / 4 - (length - anchor) / 4 - separators;
  cursor += length / 4 - (length - cursor) / 4 - separators;

  if (digits.empty()) {
    anchor = 1;
    cursor = 1;
  }

  g_serverLanguage->Select(anchor, cursor);
  this->formattedLength = this->text.size();
  this->Invalidate();
}

// OFFSET: 0x00892910
// FUNCTION: fn_11478
void UISlotBmpButton::OnProcess() {
  ++this->m_animationTick;

  if (this->m_animationTick / this->m_frameInterval > 7) {
    this->m_animationTick = 0;
  }
}

// OFFSET: 0x00892940
// FUNCTION: fn_11479
void UIBasicItemCtrl::OnRBtnDown(int, int) {
  if (std::atoi(this->m_item.itemId.c_str()) > 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
  }
}

// OFFSET: 0x00892A10
// FUNCTION: fn_11481
void UIBitmapButton2::OnRBtnDown(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_RBTN_UP, this->geometry.id, 0, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892A50
// FUNCTION: fn_11482
void UIMemberListBox::OnRBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x00892A80
// FUNCTION: fn_11483
void UISlotBmpButton::OnRBtnDown(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, 226, 0, 0);
  }
}

// OFFSET: 0x00892AB0
// FUNCTION: fn_11484
void UIStaticItem::OnRBtnDown(int, int) {
  g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&this->m_item), 0, 0, 0);
}

// OFFSET: 0x00892AE0
// FUNCTION: fn_11485
void UITVBitmapButton::OnRBtnDown(int, int) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_RBTN_UP, this->m_value, 0, 0, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892B20
// FUNCTION: fn_11486
void UITVTextButton::OnRBtnDown(int x, int y) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_RBTN_UP, this->m_value, x, y, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892BA0
// FUNCTION: fn_11488
void UIBitmapButton2::OnRBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_RBTN_UP, this->geometry.id, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00882410
// FUNCTION: fn_11301
bool UIChatHisBox2::IsTransmitMouseInput() {
  return false;
}

// OFFSET: 0x0087B4B0
// FUNCTION: fn_11240
void UIChatHisBox2::Configure(int x, int y, int width, int height, unsigned char) {
  this->transparent = 0;
  UIWindow::Resize(width, height);
  this->Move(x, y);
  this->OnCreate(width, height);
  this->OnSize(width, height);
}

// OFFSET: 0x00892C20
// FUNCTION: fn_11489
void UIChatHisBox2::OnRBtnUp(int x, int y) {
  this->parent->OnRBtnUp(x + this->geometry.x, y + this->geometry.y);
}

// OFFSET: 0x00892C50
// FUNCTION: fn_11490
void UIMemberListBox::OnRBtnUp(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00892CA0
// FUNCTION: fn_11492
void UITVBitmapButton::OnRBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_RBTN_UP, this->m_value, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00892D20
// FUNCTION: fn_11493
void UITVTextButton::OnRBtnUp(int x, int y) {
  if (this->m_disabled == 0) {
    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_RBTN_UP, this->m_value, x, y, 0);
    }

    this->Invalidate();
  }
}

// OFFSET: 0x00892DE0
// FUNCTION: fn_11495
void UIComboBoxV2Wnd::OnWheel(int value) {
  int selected = this->selected + (0 > value ? 1 : -1);
  this->selected = selected;

  const int count = static_cast<int>(this->list->lines.size());
  if (count <= selected) {
    selected = count - 1;
    this->selected = selected;
  }

  if (selected < 0) {
    this->selected = 0;
    selected = 0;
  }

  if (this->parent) {
    this->parent->SendMsg(this, UM_COMBOBOX_SELECTED, selected, 0, 0, 0);
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
  const int overflow = std::max(count - visible, 0);
  this->verticalOverflow = overflow != 0;
  auto* bar = this->verticalBar;

  if (overflow == 0) {
    bar->maximum = 0;
  } else {
    bar->maximum = this->bottomMargin + overflow;
  }

  bar->total = count;
  bar->visible = visible;
  bar->Invalidate();

  if (bar->position > bar->maximum) {
    bar->position = bar->maximum;
  }

  this->verticalBar->Invalidate();
}

// OFFSET: 0x00895D80
// FUNCTION: fn_11511
void UIComboBoxID::Refresh() {
  const int visible = this->geometry.height / this->lineHeight;
  const int count = static_cast<int>(this->lines.size());
  const int overflow = 0 > count - visible ? 0 : count - visible;

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
void UIListBoxID::OnWheel(int value) {
  if (this->verticalOverflow != 0) {
    if (value < 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00892F50
// FUNCTION: fn_11501
void UITreeView::OnWheel(int value) {
  if (this->verticalOverflow != 0) {
    if (value < 0) {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->verticalBar, UM_SCROLLVERT, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x008861E0
// FUNCTION: fn_11324
void UIItemStoreComboBox::OnCreate(int width, int height) {
  this->UIListBoxEx::OnCreate(width, height);
  this->lineHeight = 15;
}

// OFFSET: 0x00897A10
// FUNCTION: fn_11538
void UITreeView::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->verticalBar->Move(this->geometry.width - 10, 0);
  this->verticalBar->Resize(this->verticalBar->geometry.width, height);
  this->UpdateScrollBar();
}

// OFFSET: 0x008961F0
// FUNCTION: fn_11516
void UITreeView::UpdateScrollBar() {
  const int visible = this->geometry.height / this->itemHeight;
  const int count = this->itemCount;
  const int overflow = 0 > count - visible ? 0 : count - visible;
  this->verticalOverflow = overflow != 0;
  UIScrollBar* bar = this->verticalBar;
  bar->total = count;
  bar->visible = visible;
  if (overflow != 0) {
    bar->maximum = overflow;
    bar->Invalidate();
    if (bar->position > bar->maximum) {
      bar->position = bar->maximum;
    }
  } else {
    bar->maximum = 0;
    bar->Invalidate();
    if (bar->position > bar->maximum) {
      bar->position = bar->maximum;
    }
    this->scrollRow = 0;
    this->itemCount = 0;
  }

  this->verticalBar->Invalidate();

}

// OFFSET: 0x00892F90
// FUNCTION: fn_11502
void UITabControl::FillTab(int index, int x, int y, int width, int height, std::uint32_t color) {
  if (this->skipTabFill != 0) {
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
  g_session.IconPath(path, this->m_item.itemId.c_str(), this->m_item.identified);

  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  if (!bitmap) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
    if (!bitmap) {
      return;
    }
  }

  this->DrawBitmap(0, 0, bitmap, true);
}

// OFFSET: 0x0088F5C0
// FUNCTION: fn_11389
void UITVTextButton::Paint() {
  this->ClearSurface(0xFFFF00FFu);

  if (this->parent) {
    this->parent->Invalidate();
  }

  const int inset = this->m_border != 0;

  if (this->drawBackground != 0) {
    if (this->m_border != 0) {
      DrawRoundedBorder(*this, inset - 1, 0, this->geometry.width, this->geometry.height, this->m_borderColor);
    }

    if (this->m_fill != 0) {
      this->FillRegion(inset, 1, this->geometry.width - inset * 2, this->geometry.height - 2, this->m_fillColor);
    }
  }

  std::uint32_t color = this->m_textColor;

  if (this->highlighted != 0) {
    color = this->m_highlightColor;
  }

  if (this->m_outlined != 0) {
    this->DrawOutlinedText(inset + 1, 2, this->displayText.c_str(), 0, color, this->m_outlineColor, this->m_fontFamily, this->m_fontHeight, 1, 0);
  } else {
    this->DrawTextAt(inset + 1, 2, this->displayText.c_str(), 0, this->m_fontFamily, this->m_fontHeight, color, 0, 0);
  }
}

// OFFSET: 0x0088FFB0
// FUNCTION: fn_11397
void UIBasicItemCtrl::OnLBtnDblClk(int, int) {
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
void UIBasicSkillCtrl::OnLBtnDblClk(int, int) {
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
void UIComboBox::OnLBtnDblClk(int, int) {
}

// OFFSET: 0x00890170
// FUNCTION: fn_11402
void UIComboBoxID::OnLBtnDblClk(int, int) {
}

// OFFSET: 0x008902A0
// FUNCTION: fn_11408
void UISlotBmpButton::OnLBtnDblClk(int x, int y) {
  UIBitmapButton::OnLBtnDblClk(x, y);

  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, ID_OK, 0, 0, 0);
  }
}

// OFFSET: 0x008902E0
// FUNCTION: fn_11409
void UITVTextButton::OnLBtnDblClk(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.state = 1;

  if (this->parent) {
    this->parent->SendMsg(this, UM_TEXT_LBTN_DblClk, this->m_value, x, y, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00890400
// FUNCTION: fn_11411
void UIBmpRadioBtn::OnLBtnDown(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->m_value, 0, 0);
  }
}

// OFFSET: 0x00890450
// FUNCTION: fn_11413
void UIComboBox::OnLBtnDown(int, int) {
}

// OFFSET: 0x00890470
// FUNCTION: fn_11415
void UIComboBoxID::OnLBtnDown(int, int) {
}

// OFFSET: 0x008905F0
// FUNCTION: fn_11420
void UIListBoxID::OnLBtnDown(int x, int y) {
  int line;

  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    line = -1;
  } else {
    const int index = y / this->lineHeight + this->topLine;
    line = index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
  }

  this->selected = line;
  this->Invalidate();
}

// OFFSET: 0x00890640
// FUNCTION: fn_11421
void UIListBoxNavi::OnLBtnDown(int x, int y) {
  if (g_replayAnimationScaling != 0) {
    return;
  }

  this->selected = this->LineAt(x, y);

  if (this->selected != -1) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->selected, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00890690
// FUNCTION: fn_11422
void UIMemberListBox::OnLBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
  this->Invalidate();
}

// OFFSET: 0x008908A0
// FUNCTION: fn_11426
void UISlotBmpButton::OnLBtnDown(int x, int y) {
  UIBitmapButton::OnLBtnDown(x, y);

  if (this->parent) {
    this->parent->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, 0, 0, 0);
  }
}

// OFFSET: 0x008908F0
// FUNCTION: fn_11427
void UITVTextButton::OnLBtnDown(int, int) {
  if (this->m_disabled != 0) {
    return;
  }

  this->geometry.state = 2;
  this->Invalidate();

  if (this->parent) {
    this->parent->SendMsg(this, UM_BEGIN_DRAG_BTN, this->m_value, 0, 0, 0);
    this->mousePressed = 1;
  }
}

// OFFSET: 0x00890940
// FUNCTION: fn_11428
void UITabControl::OnLBtnDown(int x, int y) {
  const int index = this->TabAt(x, y);

  if (index == -1) {
    return;
  }

  if (index < static_cast<int>(this->labels.size()) && this->activeTabIndex != index) {
    this->activeTabIndex = index;
  }

  this->edit->SetText("");
  this->edit->Move(-100, -100);

  if (this->parent) {
    this->parent->SendMsg(this, UM_TAB, this->activeTabIndex, 0, 0, 0);
    this->parent->Invalidate();
  }

  this->Invalidate();
  g_windowMgr.SetCapture(this);
}

// OFFSET: 0x00890A00
// FUNCTION: fn_11430
void UITreeView::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x + this->geometry.x, y + this->geometry.y);
  }
}

// OFFSET: 0x00890A30
// FUNCTION: fn_11431
void UIBitmapButton2::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_LBTN_UP, this->geometry.id, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x00890DA0
// FUNCTION: fn_11437
void UIComboBoxV2Wnd::OnLBtnUp(int, int) {
  this->SendMsg(nullptr, UM_COMMAND, ID_OPEN_MEMBER_POS_COMBO, 0, 0, 0);
}

// OFFSET: 0x00890E30
// FUNCTION: fn_11439
void UIItemStoreComboBox::OnLBtnUp(int x, int y) {
  int line;

  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    line = -1;
  } else {
    const int index = y / this->lineHeight + this->topLine;
    line = index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
  }

  this->selected = line;

  if (this->parent) {
    this->parent->SendMsg(nullptr, UM_COMBOBOX_SELECTED, line, 0, 0, 0);
  }
}

// OFFSET: 0x00891050
// FUNCTION: fn_11442
void UITVBitmapButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0 || g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;

    if (this->parent) {
      this->parent->SendMsg(this, UM_BMP_LBTN_UP, this->m_value, 0, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x008910D0
// FUNCTION: fn_11443
void UITVTextButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (this->parent) {
    if (this->mousePressed != 0) {
      this->parent->SendMsg(this, UM_TEXT_LBTN_UP, this->m_value, x, y, 0);
    } else {
      this->parent->SendMsg(this, UM_END_DRAG_BTN, this->m_value, 0, 0, 0);
    }

    this->mousePressed = 0;
  }

  this->Invalidate();
}

// OFFSET: 0x00891130
// FUNCTION: fn_11444
void UITabControl::OnLBtnUp(int, int) {
  g_windowMgr.ReleaseCapture();
}

// OFFSET: 0x0088AD00
// FUNCTION: fn_11359
void UIINT64BarGraph::Paint() {
  if (this->m_maximum < this->m_current || this->m_maximum == 99999999) {
    return;
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(26, 2, red, green, blue);
  this->ClearSurface((green & 255) << 16 | (green & 255) << 8 | (red & 255));
  g_skinInfo.PaletteChannels(30, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, (blue & 255) << 16 | (green & 255) << 8 | (red & 255));

  if (this->m_maximum == 0) {
    return;
  }

  const double ratio = static_cast<double>(this->m_current) / static_cast<double>(this->m_maximum);

  if (0.0 == ratio) {
    return;
  }

  g_skinInfo.PaletteChannels(2, 6, red, green, blue);

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
    this->DrawOutlinedText(1, 1, this->m_text.c_str(), 0, this->m_color, this->m_outlineColor, this->m_family, this->m_height, 1, 0);
  } else {
    this->DrawTextAt(1, 1, this->m_text.c_str(), 0, this->m_family, this->m_height, this->m_color, 0, 0);
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
    this->DrawOutlinedText(1, this->textY, this->m_text.c_str(), 0, this->m_color, this->m_outlineColor, this->m_family, this->m_height, 1, 0);
  } else {
    this->DrawTextAt(1, this->textY, this->m_text.c_str(), 0, this->m_family, this->m_height, this->m_color, 0, 0);
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
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first), static_cast<unsigned>(this->lines.size()));
  std::uint32_t color = 0;

  for (int index = first, row = 0; index < static_cast<int>(end); ++index, ++row) {
    if (this->lineColors[index] == 1) {
      color = this->defaultLineColor;
    }

    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    const auto& line = this->lines[index];
    this->DrawColoredText(3 - this->horizontalOffset, this->lineHeight * row, line.c_str(), static_cast<unsigned>(line.size()), color, 0, 12, false);

    if (this->lineColors[index] == 1) {
      color = 0;
    }
  }
}

// OFFSET: 0x0088B4A0
// FUNCTION: fn_11363
void UIItemStoreComboBoxWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\bg_setting.bmp")));
  this->DrawBitmap(0, this->geometry.height - background->image.height, background, false);
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
  switch (this->vertical) {
  case 1: {
    this->FillRegion(0, 11, 10, this->geometry.height - 21, 0);
    if (this->state == 1) {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str()))), false);
    } else if (this->state == 4) {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str()))), false);
    } else {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up.bmp").c_str()))), false);
    }

    const int thumb = this->ThumbOffset() + this->arrowLength;
    this->FillRegion(0, thumb + 1, 10, this->ThumbLength() - 1, 0xAFAAA5);
    if (this->state == 2) {
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str()))), false);
    } else if (this->state == 5) {
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str()))), false);
    } else {
      this->DrawBitmap(0, this->geometry.height - this->arrowLength - 1, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down.bmp").c_str()))), false);
    }
    break;
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
  switch (this->vertical) {
  case 1: {
    this->FillRegion(0, 10, 9, this->geometry.height - 20, 0xF1F1F1);
    if (this->state == 1) {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str()))), false);
    } else if (this->state == 4) {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str()))), false);
    } else {
      this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_up1.bmp").c_str()))), false);
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
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str()))), false);
    } else if (this->state == 5) {
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str()))), false);
    } else {
      this->DrawBitmap(0, this->geometry.height - this->arrowLength, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "dialscr_down1.bmp").c_str()))), false);
    }
    break;
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
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "scrollbar.bmp").c_str()))), true);

    const int offset = this->ThumbOffset();
    this->DrawBitmap(offset, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((path + "scroll_bar.bmp").c_str()))), true);
  }

  this->minimumThumb = 0;
}

// OFFSET: 0x0088E730
// FUNCTION: fn_11382
void UIScrollBar_Custom::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  switch (this->vertical) {
  case 1: {
    int top = 0;
    if (this->m_trackTop) {
      top = this->m_trackTop->image.height;
      this->DrawBitmap(0, 0, this->m_trackTop, false);
    }

    int middle;
    if (!this->m_trackMiddle) {
      middle = 0;
    } else {
      middle = this->m_trackMiddle->image.height;
      int y = top;
      for (int index = 0; index < (this->geometry.height - top) / middle; ++index) {
        this->DrawBitmap(0, y, this->m_trackMiddle, false);
        y += middle;
      }
    }

    if (this->m_trackBottom && (this->geometry.height - top) % middle > 0) {
      this->DrawBitmap(0, this->geometry.height - this->m_trackBottom->image.height, this->m_trackBottom, false);
    }

    if (this->m_arrowUp) {
      this->DrawBitmap(0, 0, this->m_arrowUp, true);
    }

    if (this->m_arrowDown) {
      this->DrawBitmap(0, this->geometry.height - this->m_arrowDown->image.height, this->m_arrowDown, true);
    }

    const int length = this->vertical == 0 ? this->geometry.width : this->geometry.height;
    const int thumb = this->position * (length - this->arrowLength * 2 - this->ThumbLength()) / this->maximum + this->arrowLength;
    const int thumbLength = this->ThumbLength();

    int thumbTop = 0;
    if (this->m_thumbTop) {
      thumbTop = this->m_thumbTop->image.height;
      this->DrawBitmap(0, thumb, this->m_thumbTop, true);
    }

    if (this->m_thumbMiddle) {
      const int step = this->m_thumbMiddle->image.height;
      int count = (thumbLength - thumbTop) / step;
      if (count > 0) {
        int y = thumb + thumbTop;
        do {
          this->DrawBitmap(0, y, this->m_thumbMiddle, true);
          y += step;
        } while (--count);
      }
    }

    if (this->m_thumbBottom) {
      this->DrawBitmap(0, thumbLength - this->m_thumbBottom->image.height + thumb, this->m_thumbBottom, true);
    }
    break;
  }
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

  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_arrow_up.bmp"))), true);

  this->DrawBitmap(0, this->geometry.height - this->arrowLength, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_arrow_down.bmp"))), true);

  int extent = this->geometry.height;
  switch (this->vertical) {
  case 0:
    extent = this->geometry.width;
    break;
  case 1:
    extent = this->geometry.height;
    break;
  }
  int y = this->arrowLength + (this->position * (extent - this->arrowLength * 2 - this->ThumbLength())) / this->maximum;

  this->DrawBitmap(0, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_up.bmp"))), true);

  const int bottom = y + this->ThumbLength() - 4;
  for (y += 4; y < bottom; y += 4) {
    this->DrawBitmap(0, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_mid.bmp"))), true);
  }

  this->DrawBitmap(0, bottom, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIRenewal\\questui\\btn_scroll_thumb_down.bmp"))), true);
}

// OFFSET: 0x00884DD0
// FUNCTION: fn_11313
void TableBox::OnCreate(int, int) {
}

// OFFSET: 0x00884DE0
// FUNCTION: fn_11314
void UIChatHisBox2::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 13, 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar2();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 13, horizontalHeight);
  horizontal->Move(0, height - 13);
  horizontal->OnCreate(width - 13, horizontalHeight);
  horizontal->OnSize(width - 13, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x00884F40
// FUNCTION: fn_11315
void UIChatHisBox3::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar3();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 10, 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar3();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 10, horizontalHeight);
  horizontal->Move(0, height - 10);
  horizontal->OnCreate(width - 10, horizontalHeight);
  horizontal->OnSize(width - 10, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x008850C0
// FUNCTION: fn_11316
void UIChatHisBox::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar2();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move(width - 13, 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar2();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize(width - 13, horizontalHeight);
  horizontal->Move(0, height - 13);
  horizontal->OnCreate(width - 13, horizontalHeight);
  horizontal->OnSize(width - 13, horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x0088C530
// FUNCTION: fn_11371
void UIMemberListBox::Paint() {
  this->ClearSurface((this->backgroundRed & 255) | ((this->backgroundGreen & 255) << 8) | ((this->backgroundBlue & 255) << 16));

  const int first = this->topLine;
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first), static_cast<unsigned>(this->lines.size()));

  for (int index = first, row = 0; index < static_cast<int>(end); ++index, ++row) {
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight + 1, this->geometry.width, this->lineHeight, 0xC8C832);
    }

    this->DrawTextAt(2, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->lineColors[index], 0, 0
    );
  }
}

// OFFSET: 0x00888C10
// FUNCTION: fn_11343
void UIBasicItemCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const char* name = std::atoi(this->m_item.itemId.c_str()) <= 0 || this->m_item.identity[1] > 0 ? "유저인터페이스\\basic_interface\\itemwin_mid.bmp"
                                                                                                : "유저인터페이스\\ExMacro\\no_item.bmp";
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  this->DrawBitmap(0, 0, bitmap, false);
  this->DrawItemIcon(4, 0, 0, false);
}

// OFFSET: 0x00888C90
// FUNCTION: fn_11344
void UIBasicSkillCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  bool drawn = false;
  if (this->m_skillId >= 0) {
    CSkillInfo skill = g_session.LookupSkill(static_cast<std::uint32_t>(this->m_skillId));
    if (skill.available != 0) {
      drawn = true;
      if (this->m_level > 0) {
        skill.level = this->m_level;
      }

      char text[1024];
      g_session.ItemBitmapPath(skill.name, text);
      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      this->DrawBitmap(0, 0, bitmap, true);
      if (skill.level != 0) {
        std::sprintf(text, "%d", skill.level);
        FontCache& fonts = g_fontCache;
        const int x = this->geometry.width - this->TextWidth(text, 0, 0, 11, 0, 0) - 1;
        this->DrawTextAt(x + 1, 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(x - 1, 15, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(x, 16, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(x, 14, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(x, 15, text, 0, 0, 11, 0, 0, 0);
      }
    }
  }

  if (!drawn) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\no_skill.bmp"))), false);
  }
}

// OFFSET: 0x00888EA0
// FUNCTION: fn_11345
void UIBmpRadioBtn::Paint() {
  this->ClearSurface(0xFFFF00FF);
  CBitmapRes* bitmap;
  if (this->m_disabled == true) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[3].c_str())));
  } else if (this->geometry.state == 1) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[2].c_str())));
  } else if (this->m_hovered == true) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[1].c_str())));
  } else {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_names[0].c_str())));
  }

  this->DrawBitmap(0, 0, bitmap, false);
  if (this->m_label.size() == 0) {
    return;
  }

  FontCache& fonts = g_fontCache;
  if (this->m_disabled == true) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 3) {
        if (it->drawShadow == true) {
          this->DrawTextAt(it->x, it->y, this->m_label.c_str(), 0, 0, it->height, 0xE2E2E2, 0, 0);
        }
        this->DrawTextAt(it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(x, y, this->m_label.c_str(), 0, 0, 12, 0xE2E2E2, 0, 0);
    this->DrawTextAt(x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0x6F6F6F, 0, 0);
    return;
  }

  if (this->geometry.state == 1) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 2) {
        if (it->drawShadow == true) {
          this->DrawTextAt(it->x, it->y, this->m_label.c_str(), 0, 0, it->height, 0xFFFFFF, it->bold, 0);
        }
        this->DrawTextAt(it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(x, y, this->m_label.c_str(), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  if (this->m_hovered == true) {
    for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
      if (it->id == 1) {
        if (it->drawShadow == true) {
          this->DrawTextAt(it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, 0xC7A796, it->bold, 0);
        }
        this->DrawTextAt(it->x, it->y, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
        return;
      }
    }

    const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
    this->DrawTextAt(x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0xC7A796, 0, 0);
    this->DrawTextAt(x, y, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
    return;
  }

  for (auto it = this->m_texts.begin(); it != this->m_texts.end(); ++it) {
    if (it->id == 0) {
      if (it->drawShadow == true) {
        this->DrawTextAt(it->x + 1, it->y + 1, this->m_label.c_str(), 0, 0, it->height, 0xD7D7D7, it->bold, 0);
      }
      this->DrawTextAt(it->x, it->y, this->m_label.c_str(), 0, 0, it->height, it->color, it->bold, 0);
      return;
    }
  }

  const int x = static_cast<int>((this->geometry.width - this->TextWidth(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
  const int y = static_cast<int>((this->geometry.height - this->TextHeight(this->m_label.c_str(), 0, 0, 12, 0, 0)) * 0.5);
  this->DrawTextAt(x + 1, y + 1, this->m_label.c_str(), 0, 0, 12, 0xD7D7D7, 0, 0);
  this->DrawTextAt(x, y, this->m_label.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x008894A0
// FUNCTION: fn_11346
void UICaptchaImage::Paint() {
  this->DrawBitmap(0, 0, &this->m_bitmap, true);
}

// OFFSET: 0x00889900
// FUNCTION: fn_11350
void UIComboBox::Paint() {
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp"))), false);
  auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, leftDown, false);
  auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, rightUp, false);
  auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, rightDown, false);

  const int first = this->topLine;
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first), static_cast<unsigned>(this->lines.size()));

  for (int index = first, row = 0; index < static_cast<int>(end); ++index, ++row) {
    if (index == this->selected) {
      g_skinInfo.PaletteChannels(6, 2, red, green, blue);
      this->FillRegion(3, row * this->lineHeight + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }

    this->DrawTextAt(3 - this->horizontalOffset, this->lineHeight * row + 2, this->lines[index].c_str(), 0, 0, 12, 0, 0, 0
    );
  }

  if (this->verticalOverflow != 0) {
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
    this->FillRegion(this->geometry.width - 16, 0, 16, this->geometry.height, RGB(red, green, blue));
  }

  this->parent->Invalidate();
}

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
void UIPackageBoxNameList::OnLBtnDown(int x, int y) {
  this->selected = this->LineAt(x, y);
  if (this->selected != -1) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->selected, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x00886AE0
// FUNCTION: fn_11328
void UIListBoxID::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 13), 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  auto* horizontal = new UIScrollBar();
  this->horizontalBar = horizontal;
  const int horizontalHeight = horizontal->thickness;
  horizontal->vertical = 0;
  horizontal->UIWindow::Resize((width - 13), horizontalHeight);
  horizontal->Move(0, (height - 13));
  horizontal->OnCreate((width - 13), horizontalHeight);
  horizontal->OnSize((width - 13), horizontalHeight);
  this->AddChild(this->horizontalBar);
}

// OFFSET: 0x00887280
// FUNCTION: fn_11332
void UITableBox::OnCreate(int, int) {
}

// OFFSET: 0x008872B0
// FUNCTION: fn_11335
void UITreeView::OnCreate(int width, int height) {
  auto* vertical = new UIScrollBar3();
  this->verticalBar = vertical;
  const int verticalWidth = vertical->thickness;
  vertical->vertical = 1;
  vertical->UIWindow::Resize(verticalWidth, height);
  vertical->Move((width - 10), 0);
  vertical->OnCreate(verticalWidth, height);
  vertical->OnSize(verticalWidth, height);
  this->AddChild(this->verticalBar);

  this->edit = new UIEditCtrl2();
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
void UISlotBmpButton::OnCreate(int, int) {
  const std::string path = "유저인터페이스\\renewalparty\\";
  char name[256];
  std::sprintf(name, "%sicon_jobs_%d.bmp", path.c_str(), 0);
  this->icon = new UIStaticBitmap();
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
  this->ClearSurface(0xFFFF00FF);

  if (!this->text.empty()) {
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->text.c_str()))), false);
  }

  this->DrawTextAt(2, 3, this->SelectedText().c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x00880CC0
// FUNCTION: fn_11289
std::string UIComboBoxV2Wnd::SelectedText() {
  const int selected = this->selected;

  if (selected < 0 || static_cast<int>(this->list->lines.size()) <= selected) {
    return std::string();
  }

  int index = selected;
  return this->list->lines[index].text;
}

// OFFSET: 0x00880FF0
// FUNCTION: fn_11293
void UITreeView::HideNodes(std::list<UITreeNode*>& nodes) {
  if (static_cast<int>(nodes.size()) <= 0) {
    return;
  }

  for (std::list<UITreeNode*>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
    if ((*it)->collapseButton) {
      (*it)->collapseButton->Move(-100, -100);
    }

    if ((*it)->expandButton) {
      (*it)->expandButton->Move(-100, -100);
    }

    (*it)->labelButton->Move(-100, -100);

    if (static_cast<int>((*it)->children.size()) > 0) {
      this->HideNodes((*it)->children);
    }
  }
}

// OFFSET: 0x00881110
// FUNCTION: fn_11296
void UITreeView::AddNodeButtons(UITreeNode* node, int id) {
  const char* names[] = {"battle_option_b.bmp", "wnd_mini_b.bmp", nullptr};
  std::string path = "유저인터페이스\\basic_interface\\";

  for (int index = 0; index < 2; ++index) {
    UITVBitmapButton* button = new UITVBitmapButton();
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
  UITVTextButton* button = new UITVTextButton();
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

#include "UI/Login/UIDescriptRace.h"
#include "UI/Window/InputFocus.h"

// OFFSET: 0x008901E0
// FUNCTION: fn_11405
void UIListBoxID::OnLBtnDblClk(int, int) {
  if (this->selected != -1) {
    this->parent->SendMsg(this, UM_COMMAND, this->geometry.id, this->lines[this->selected].id, 0, 0);
  }
}

// OFFSET: 0x00890230
// FUNCTION: fn_11406
void UINewComboBoxCtrl::OnLBtnDblClk(int, int) {
  if (this->parent) {
    this->parent->SendMsg(this, 124, this->selected, 0, 0, 0);
  }
}

// OFFSET: 0x00890480
// FUNCTION: fn_11416
void UIDescriptRace::OnLBtnDown(int, int) {
  if (UIWindow* window = g_windowMgr.LookupWindow(WID_NEWMAKECHARWND)) {
    window->SendMsg(this, UM_COMMAND, ID_SLOT, 0, 0, 0);
  }
}

// OFFSET: 0x0088ACF0
// FUNCTION: fn_11358
void UIDescriptRace::Paint() {
  UIBitmapButton::Paint();
  this->DrawRaceDescription();
}

// OFFSET: 0x00890330
// FUNCTION: fn_11410
void UITabControl::OnLBtnDblClk(int, int) {
  if (this->layout == 0 || this->layout == 2 || !this->allowTabRename) {
    return;
  }

  this->edit->SetText(this->labels[this->activeTabIndex].c_str());
  this->edit->Resize(this->widths[this->activeTabIndex] - 4, this->geometry.height - 2);

  this->edit->Move(TabOffset(*this, this->activeTabIndex) + 2, 2);
  g_windowMgr.SetFocus(this->edit);
}

// OFFSET: 0x0088C6E0
// FUNCTION: fn_11373
void UINewComboBoxCtrl::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, RGB(this->red, this->green, this->blue));

  if (!this->borderless) {
    this->DrawRectOutline(0, 0, this->geometry.width, this->geometry.height, RGB(this->borderRed, this->borderGreen, this->borderBlue));
  }
}

// OFFSET: 0x0088EB70
// FUNCTION: fn_11384
void UIShowAndEditTimeCtrl::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawTextAt(22, 3, "m", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(60, 3, "s", 0, 0, 12, 0, 0, 0);
}

#include "UI/Buttons/UINaviLinkButton.h"

// OFFSET: 0x008906C0
// FUNCTION: fn_11423
void __fastcall ActivateNaviLink(UINaviLinkButton* button, void*, int x, int y) {
  button->UINaviLinkButton::OnLBtnDown(x, y);
}

// OFFSET: 0x0088AEC0
// FUNCTION: fn_11360
void UIImgMenu::Paint() {
  this->FillRegion(0, 0, this->geometry.width, this->geometry.height, 0x99A8AC);
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  this->DrawBitmap(1, 1, background, false);

  if (this->hovered <= -1) {
    return;
  }

  char suffix[12];
  std::sprintf(suffix, "_%d.bmp", this->hovered);
  const std::string path = this->baseName + suffix;
  auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path.c_str())));

  const int offset = this->pressedItems[this->hovered] == 0 ? 0 : 3;

  if (this->disabledItems[this->hovered] == 0) {
    if (this->horizontal != 0) {
      this->DrawBitmap(this->itemWidth * this->hovered + 1 + offset, 1, bitmap, false);
    } else {
      this->DrawBitmap(1, this->itemHeight * this->hovered + 1 + offset, bitmap, false);
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
#include "UI/Login/UINewSelectCharWnd.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CLoginMode.h"

#include <format>

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
  if (!g_windowMgr.rodexComposeWnd) {
    return;
  }

  unsigned line = 0;
  int consumed = 0;
  UIRodexComposeWnd* compose = static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd);
  const int left = compose->m_contents->geometry.x;
  const int top = compose->m_contents->geometry.y;
  const int lineHeight = compose->m_contents->lineHeight;
  std::string rest = name;
  const int firstLine = compose->m_contents->firstVisibleLine;
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
  UINpcEditDialogWnd* owner = static_cast<UINpcEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd);
  if (!owner || owner->edit->geometry.id != 492) {
    return;
  }

  const int left = owner->edit->geometry.x;
  const int top = owner->edit->geometry.y;
  const int position = x;
  unsigned bytes = 0;
  const int width = reinterpret_cast<UIItemTagCtrl*>(owner)->MeasureText(name.c_str(), bytes);
  button->Move(
    this->textWidth + width > static_cast<UINpcEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->edit->geometry.width ? 1000
    : position != 0                                                                                                     ? position
                                                                                                                        : left + 2 + width,
    top + 1
  );
}

// OFFSET: 0x0088AC80
// FUNCTION: fn_11357
void UIComboBox_BmpWnd::Paint() {
  this->ClearSurface(RGB(this->red, this->green, this->blue));
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->name.c_str()))), false);
}

// OFFSET: 0x008904C0
// FUNCTION: fn_11417
void UIItemTagButton::OnLBtnDown(int x, int y) {
  if (this->m_disabled != 0) {
    return;
  }

  if (this->m_value == 517) {
    if (UITipboxWnd* tipbox = static_cast<UITipboxWnd*>(g_windowMgr.EnsureWindow(WID_TIPBOXWND))) {
      tipbox->ShowTip(this->tipId);
    }
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = 0;
  } else {
    this->geometry.state = 1;
    if (this->parent) {
      this->parent->SendMsg(this, UM_TEXT_LBTN_UP, this->geometry.id, this->m_value, reinterpret_cast<std::intptr_t>(&this->item), 0);
      this->Invalidate();
      return;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x00890AC0
// FUNCTION: fn_11433
void UIComboBox::OnLBtnUp(int x, int y) {
  this->selected = this->LineAt(x, y);
  if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
    target->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->selected, 0, 0, 0);
    g_windowMgr.CloseWindow(WID_COMBOBOXWND);
    return;
  }

  if (this->parent) {
    this->parent->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->selected, 0, 0, 0);
  }

  g_windowMgr.CloseWindow(WID_COMBOBOXWND);
}

// OFFSET: 0x00890C20
// FUNCTION: fn_11436
void UIComboBoxID::OnLBtnUp(int x, int y) {
  int line;

  if (x < 0 || y < 0 || x >= this->geometry.width || y >= this->geometry.height) {
    line = -1;
  } else {
    const int index = y / this->lineHeight + this->topLine;
    line = index >= 0 && static_cast<unsigned>(index) < this->lines.size() ? index : -1;
  }

  this->selected = line;

  if (UIWindow* target = g_windowMgr.LookupWindow(this->targetId)) {
    const auto found = this->lineData.find(this->selected);
    if (found != this->lineData.end()) {
      target->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->lines[this->selected].id, found->second, 0, 0);
    } else {
      target->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->lines[this->selected].id, 0, 0, 0);
    }
  } else if (UIWindow* parent = this->parent) {
    const auto found = this->lineData.find(this->selected);
    if (found != this->lineData.end()) {
      parent->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->lines[this->selected].id, found->second, 0, 0);
    } else {
      parent->SendMsg(nullptr, UM_COMBOBOX_SELECTED, this->lines[this->selected].id, 0, 0, 0);
    }
  }

  g_windowMgr.CloseWindow(WID_COMBOBOXWND);
}

// OFFSET: 0x00892990
// FUNCTION: fn_11480
void UIBasicSkillCtrl::OnRBtnDown(int x, int y) {
  if (this->m_skillId < 0) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND);
  window->SendMsg(nullptr, UM_SKILLDESCRIBE, this->m_skillId, 0, 0, 0);
  int screenX = 0;
  int screenY = 0;
  this->ToScreen(screenX, screenY);
  window->Move(screenX + x + 24, screenY + y);
}

// OFFSET: 0x0089BA90
// FUNCTION: fn_11576
void UISlotBmpButton::SetSlot(int slot, int fallback) {
  this->m_slot = slot;
  this->m_inRange = g_session.IsRestrictedCharacterSlot(slot == -1 ? fallback : slot);
  if (this->m_slot < 0) {
    this->icon->Show(0);
    return;
  }

  const CharacterInfo* info = reinterpret_cast<const CharacterInfo*>(g_modeMgr.gameMode->SendMsg(MM_QUERYCHARICTORINFO, this->m_slot, 0, 0, 0));
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
    this->ClearSurface(RGB(this->backgroundColor[0], this->backgroundColor[1], this->backgroundColor[2]));
  }

  const int first = this->topLine;
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first), static_cast<unsigned>(this->lines.size()));
  std::uint32_t color = 0;

  for (int index = first; index < static_cast<int>(end); ++index) {
    if (index == this->selected) {
      this->FillRegion(0, (index - first) * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    this->DrawColoredText(
      3 - this->horizontalOffset, (index - first) * this->lineHeight, this->lines[index].text.c_str(),
      static_cast<unsigned>(this->lines[index].text.size()), color, 0, 12, false
    );
  }
}

// OFFSET: 0x0088C110
// FUNCTION: fn_11370
void UIListBoxTitle::Paint() {
  if (this->transparent == 1) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(RGB(this->backgroundColor[0], this->backgroundColor[1], this->backgroundColor[2]));
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
    } else {
      const int* const titles = reinterpret_cast<const int*>(g_windowCloseValue);
      const int* const titlesEnd = reinterpret_cast<const int*>(g_savedWindowCloseValue);
      if (std::find(titles, titlesEnd, this->lines[index].id) != titlesEnd) {
        this->FillRegion(0, (index - first) * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->titleColor);
      }
    }

    this->DrawColoredText(
      3 - this->horizontalOffset, (index - first) * this->lineHeight, this->lines[index].text.c_str(),
      static_cast<unsigned>(this->lines[index].text.size()), color, 0, 12, false
    );
  }
}

// OFFSET: 0x0088A7D0
// FUNCTION: fn_11355
void UIComboBoxID::Paint() {
  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(6, 6, red, green, blue);
  this->ClearSurface(RGB(red, green, blue));
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, RGB(red, green, blue));

  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_lu.bmp"))), false);
  auto* leftDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ld.bmp")));
  this->DrawBitmap(0, this->geometry.height - leftDown->image.height, leftDown, false);
  auto* rightUp = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_ru.bmp")));
  this->DrawBitmap(this->geometry.width - rightUp->image.width, 0, rightUp, false);
  auto* rightDown = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\sysboxs_rd.bmp")));
  this->DrawBitmap(this->geometry.width - rightDown->image.width, this->geometry.height - rightDown->image.height, rightDown, false);

  const int first = this->topLine;
  unsigned end = static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + first);
  if (this->lines.size() < end) {
    end = static_cast<unsigned>(this->lines.size());
  }

  for (int index = first; index < static_cast<int>(end); ++index) {
    if (index == this->selected) {
      g_skinInfo.PaletteChannels(6, 2, red, green, blue);
      this->FillRegion(3, (index - first) * this->lineHeight + 1, this->geometry.width - 6, this->lineHeight - 2, RGB(red, green, blue));
    }

    this->DrawTextAt(3 - this->horizontalOffset, (index - first) * this->lineHeight + 2, this->lines[index].text.c_str(), 0, 0, 12,
      0, 0, 0
    );
  }

  if (this->verticalOverflow != 0) {
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
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

        this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path))), false);
      }
    }

    for (int index = first; index < static_cast<int>(end); ++index) {
      this->DrawTextAt(3 - this->horizontalOffset, (index - first) * this->lineHeight + 2, this->lines[index].text.c_str(), 0,
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

  for (int row = 0; row + first < static_cast<int>(end); ++row) {
    const int index = row + first;
    if (index == this->selected) {
      this->FillRegion(0, row * this->lineHeight, this->geometry.width, this->lineHeight - 1, this->selectionColor);
    }

    const int x = 3 - this->horizontalOffset;
    int y = this->lineHeight * row;
    if (this->centerVertically == 1) {
      const int lineHeight = this->lineHeight;
      const int height = this->TextHeight(this->lines[index].c_str(), 0, 0, 12, 0, 0);
      y = static_cast<int>(static_cast<float>(lineHeight - height) * 0.5f + static_cast<float>(row * lineHeight));
    }

    const std::string& line = this->lines[index];
    this->DrawStyledColoredText(
      x, y, line.c_str(), static_cast<unsigned>(line.size()), color, this->fontFamily, this->fontHeight, this->bold, this->italic, false
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
    if (percent <= 0.0f) {
      return;
    }
    const std::uint32_t color = static_cast<float>(this->m_threshold) > percent ? this->colors[4] : this->colors[2];
    this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width - 2) * percent / 100.0f), this->geometry.height - 2, color);
  }
}

// OFFSET: 0x0088C750
// FUNCTION: fn_11374
void UIPcGage::Paint() {
  this->ClearSurface(this->colors[0]);
  if (this->displayMode == 0) {
    this->FillRegion(1, 1, this->geometry.width - 2, this->geometry.height - 2, this->colors[1]);
    if (this->maximum != 0) {
      const float percent = static_cast<float>(this->current * 100 / this->maximum);
      if (0.0f != percent) {
        if (25.0f > percent) {
          this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), this->geometry.height - 2, this->colors[4]);
        } else {
          this->FillRegion(1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), this->geometry.height - 2, this->colors[2]);
        }
      }
    }
  } else {
    this->FillRegion(1, 1, this->geometry.width - 2, (this->geometry.height - 3) / 2, this->colors[1]);
    this->FillRegion(1, this->geometry.height / 2 + 1, this->geometry.width - 2, (this->geometry.height - 3) / 2, this->colors[1]);
    if (this->maximum != 0 && this->secondaryMaximum != 0) {
      const float percent = static_cast<float>(this->current * 100 / this->maximum);
      const float second = static_cast<float>(this->secondaryValue * 100 / this->secondaryMaximum);
      if (percent != 0.0f) {
        if (25.0f > percent) {
          this->FillRegion(
            1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), (this->geometry.height - 3) / 2, this->colors[4]
          );
        } else {
          this->FillRegion(
            1, 1, static_cast<int>(static_cast<float>(this->geometry.width) * percent / 100.0f - 2.0f), (this->geometry.height - 3) / 2, this->colors[2]
          );
        }
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
void UIImgMenu::OnLBtnUp(int x, int y) {
  this->hovered = this->ItemAt(x, y);
  if (this->hovered != -1 && !this->disabledItems[this->hovered]) {
    if (UIWindow* target = g_windowMgr.LookupWindow(this->callerWindowId)) {
      target->SendMsg(nullptr, UM_MENU_SELECTED, this->hovered, this->geometry.id, 0, 0);
    }
    g_windowMgr.CloseWindow(0x7A);
  }
}

#include "Contents/Items/ItemInfo.h"

// OFFSET: 0x00890EE0
// FUNCTION: fn_11441
void UIString::OnLBtnUp(int, int) {
  if (!this->link.empty()) {
    UIWindow* window = g_windowMgr.EnsureWindow(WID_MONSTERDISPLAYWND);
    window->SendMsg(nullptr, UM_CARDITEMILLUSTRATE, reinterpret_cast<std::intptr_t>(this->link.c_str()), 1, 0, 0);
    UIWindow* frame = g_windowMgr.LookupWindow(WID_RENEWQUESTUI);
    UIWindow* parent = this->parent;
    window->Move(
      frame->geometry.x + (parent->geometry.x + 100 + this->geometry.x), frame->geometry.y + (parent->geometry.y - 60 + this->geometry.y)
    );
  } else if (!this->image.empty()) {
    ITEM_INFO item;
    item.itemId = this->image;
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
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

      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\icon_new.bmp")));
      this->DrawBitmap(1, static_cast<unsigned short>(offset) + 2, bitmap, false);
    }
  }
}

#include "UI/Text/UITextViewer.h"

namespace {
  // INLINED
  std::string StringText(const UIString& string) {
    return string.text;
  }

  // INLINED
  std::string StringImage(const UIString& string) {
    return string.image;
  }
}

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
  for (auto it = this->lineLinks.begin(), end = this->lineLinks.end(); it != end; ++it, ++line) {
    std::vector<UITextButton*> buttons = *it;
    if (line >= top) {
      int x = 0;
      for (UITextButton* button : buttons) {
        UIString* text = reinterpret_cast<UIString*>(button);
        if (StringText(*text) != "\n") {
          text->Show(1);
          x = text->enabled == 0 ? x : x - 5;
          if (x < 0) {
            x = 0;
          }
          const bool hasImage = !StringImage(*text).empty();
          if (hasImage) {
            text->Move(x, y - 7);
          } else {
            text->Move(x, y);
          }
          x = x + 5 + text->geometry.width;
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

#include "UI/Dialogs/UIRPImageWnd.h"

// OFFSET: 0x0088D560
// FUNCTION: fn_11376
void UIRPImageWnd::Paint() {
  this->SetSolid(g_renderer->primitives, this->geometry.width, this->geometry.height, 0);
  for (int i = 0; i < this->m_imageCount; ++i) {
    this->DrawNamed(g_renderer->primitives, 0, 0, this->m_imageName, 0xFFFFFFFF, 0, 0);
  }
}

// OFFSET: 0x0086EBB0
// FUNCTION: fn_11081
UITVBitmapButton::UITVBitmapButton()
    : UIBitmapButton(), m_value(0) {
}

// OFFSET: 0x0086EC10
// FUNCTION: fn_11082
UITVTextButton::UITVTextButton() : UIButton() {
  this->drawBackground = 0;
  this->highlighted = 0;
  this->mousePressed = 0;
  this->m_border = 0;
  this->m_value = 0;
  this->m_fill = 0;
  this->m_fillColor = 0xFFFFFF;
  this->m_textColor = 0;
  this->m_outlineColor = 0xFFFFFF;
  this->m_highlightColor = 0;
  this->m_outlined = 0;
  this->m_fontHeight = 12;
  this->m_fontFamily = 0;
}

#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Tables/TipOfTheDay.h"

// OFFSET: 0x0086DE10
// FUNCTION: fn_11060
UIMemberListBox::UIMemberListBox() : UIChatHisBox2() {
  this->backgroundRed = 255;
  this->backgroundGreen = 255;
  this->backgroundBlue = 255;
}

// OFFSET: 0x008894B0
// FUNCTION: fn_11347
void UIChatHisBox2::Paint() {
  this->ClearSurface(RGB(this->backgroundRed, this->backgroundGreen, this->backgroundBlue));

  const int top = this->topLine;
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + top), static_cast<unsigned>(this->lines.size()));

  FontCache& fonts = g_fontCache;
  for (int row = 0; top + row < static_cast<int>(end); ++row) {
    const int index = top + row;
    if (g_serviceType == 19 && g_tipOfTheDayEnglish) {
      const int width = this->TextWidth(this->lines[index].c_str(), 0, 0, 12, 0, 0);
      this->DrawTextAt(this->geometry.width - width - 19, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->lineColors[index], 0, 0);
    } else {
      this->DrawTextAt(2, this->lineHeight * row + 3, this->lines[index].c_str(), 0, 0, 12, this->lineColors[index], 0, 0);
    }
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
  const unsigned end = std::min(static_cast<unsigned>(this->geometry.height / this->lineHeight - this->bottomMargin + top), static_cast<unsigned>(this->lines.size()));

  FontCache& fonts = g_fontCache;
  for (int index = top; index < static_cast<int>(end); ++index) {
    this->DrawTextAt(2, this->lineHeight * (index - top) + 3, this->lines[index].c_str(), 0, 0, this->fontHeight, this->lineColors[index], 0, 0);
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
  this->UpdateScrollBar();
}

// OFFSET: 0x0088F450
// FUNCTION: fn_11388
void UIString::Paint() {
  if (this->text.empty()) {
    return;
  }

  std::uint32_t color = (static_cast<std::uint32_t>(static_cast<unsigned char>(this->blue)) << 16) |
                        (static_cast<std::uint32_t>(static_cast<unsigned char>(this->green)) << 8) |
                        static_cast<std::uint32_t>(static_cast<unsigned char>(this->red));

  const char* cur = this->text.c_str();
  const char* end = cur + this->text.size();

  if (!this->image.empty()) {
    char path[128];
    std::memset(path, 0, sizeof(path));
    g_session.IconPath(path, this->image.c_str(), 0);
    CRes* res = ResourceMgr::Instance().Load(Resolve(path));
    if (!res) {
      return;
    }
    this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(res), true);
    while (cur < end) {
      cur = ParseColorCode(SegmentEnd(cur, end), color);
    }
    this->DrawColoredText(24, 7, this->text.c_str(), static_cast<unsigned>(this->text.size()), color, 0, 12, false);
  } else {
    this->DrawColoredText(0, 0, this->text.c_str(), static_cast<unsigned>(this->text.size()), color, 0, 12, false);
  }
}

// OFFSET: 0x00899780
// FUNCTION: fn_11557
int UISlotBmpButton::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_SETSTATE:
    this->m_selected = argument1 != 0;
    break;

  case UM_SET_CHARACTER_SLOT_STYLE: {
    this->m_displayMode = static_cast<int>(argument1);
    const char* slot = argument1 != 0 ? "slot" : "slot2";
    const char* img = this->m_inRange ? "img_buy" : "img_";
    const std::string base = std::format("유저인터페이스\\select_character_ver3\\{}{}_", img, slot).c_str();

    this->SetBitmap(std::format("{}normal.bmp", base).c_str(), 0, 0);
    this->SetBitmap(std::format("{}over.bmp", base).c_str(), 1, 0);
    this->SetBitmap(std::format("{}press.bmp", base).c_str(), 2, 0);
    break;
  }

  default:
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}

#include "UI/Text/TextWrapping.h"
#include "Utility/Timestamp.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/CLoginMode.h"

// OFFSET: 0x0088EBC0
// FUNCTION: fn_11385
void UISlotBmpButton::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->UIBitmapButton::Paint();

  if (this->m_disabled == 0) {
    if (this->m_displayMode == 0) {
      if (this->m_selected == 1) {
        CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\select_character_ver3\\img_slot2_select.bmp")));
        if (this->m_inRange) {
          bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\select_character_ver3\\img_buyslot2_select.bmp")));
        }
        if (bitmap) {
          this->DrawBitmap(0, 0, bitmap, false);
        }
      }
    } else if (this->m_displayMode == 1 && this->m_selected == 1) {
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(
        std::format("유저인터페이스\\select_character_ver3\\img_{}_select{}.bmp", this->m_inRange ? "buyslot" : "slot", this->m_animationTick / this->m_frameInterval).c_str()
      )));
      if (bitmap) {
        this->DrawBitmap(0, 0, bitmap, false);
      }
    }
  }

  if (this->m_index == 0 && g_session.characterSlotState == 0) {
    CBitmapRes* balloon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\select_character_ver3\\balloon.bmp")));
    this->DrawBitmap(this->geometry.width / 2 - 0x2e, 0x1a, balloon, false);

    std::vector<std::string> lines;
    WrapPlainText(LookupMessage(MSI_ERROR_MDENGEON_IS_FULL), lines, 0x14);
    int y = 0x22;
    for (unsigned index = 0; index < lines.size(); ++index) {
      this->DrawCenteredText(0x21, y, lines[index].c_str(), 0x5c, 0, 0xb, 0xffffff, 0);
      y += 0xd;
    }
  }

  if (this->m_slot >= 0) {
    CharacterInfo* info = reinterpret_cast<CharacterInfo*>(g_modeMgr.gameMode->SendMsg(MM_QUERYCHARICTORINFO, this->m_slot, 0, 0, 0));
    if (info) {
      if (info->nameChangeCount > 0 || info->slotChangeCount > 0) {
        if (this->m_inRange) {
          CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\select_character_ver3\\buyslot_add_ons.bmp")));
          this->DrawBitmap(0, 0, bitmap, true);
        } else {
          CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Add_Ons.bmp")));
          this->DrawBitmap(0x13, 0x19, bitmap, false);
        }
      }

      if (info->deleteDate != 0) {
        Timestamp expiry(info->deleteDate);
        const std::uint32_t color = expiry.IsBeforeNow() ? 0xff0000 : 0xff;
        const TIMESTAMP_STRUCT date = expiry.ToSqlTimestamp();
        const std::string period = FormatOwnedString(LookupMessage(MSI_INCOMPLETE), date.month, date.day);
        const std::string time = FormatOwnedString(LookupMessage(MSI_PRIVATE_AIRPLANE_FAILED_USE_ITEM), date.hour, date.minute, date.second);
        const int periodWidth = this->TextWidth(period.c_str(), 0, 0, 0xb, 0, 0);
        this->DrawTextAt(0x4f - periodWidth / 2, 0x28, period.c_str(), 0, 0, 0xb, color, 0, 0);
        const int timeWidth = this->TextWidth(time.c_str(), 0, 0, 0xb, 0, 0);
        this->DrawTextAt(0x4f - timeWidth / 2, 0x34, time.c_str(), 0, 0, 0xb, color, 0, 0);
      }
    }
  }
}

// OFFSET: 0x0086D5F0
// FUNCTION: fn_11049
UIItemTagButton::UIItemTagButton() {
  this->textY = 1;
  this->tipId = 0;
}

