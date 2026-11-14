#include "UI/Window/WindowId.h"
#include "UI/GameSettings/TalkType.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/ModeMessage.h"
#include "Render/Text/FontCache.h"
#include "Contents/Items/ItemType.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Client/Input/Mouse.h"
#include "Client/Replay/Replay.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Contents/Configuration/GameSettingsMgr.h"
#include "UI/Layout/SnapMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Shop/ItemDrawing.h"
#include "Utility/Text/FormatString.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenRectangle.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UIBmpTextButton2.h"
#include "UI/Buttons/UINaviLinkButton.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Buttons/UITextureButton.h"
#include "UI/Items/UIItemPurchaseWnd.h"
#include "UI/Items/UIParaItemShopWnd.h"
#include "UI/Items/UIItemSellWnd.h"
#include "UI/Items/UIItemSellWnd2.h"
#include "UI/Items/UIItemShopWnd.h"
#include "UI/Items/UIItemShopWnd2.h"
#include "UI/Items/UIItemStoreFindWnd.h"
#include "UI/Items/UIItemStoreSubWnd.h"
#include "UI/Items/UIItemStoreWnd.h"
#include "UI/Items/UIItemWnd.h"
#include "UI/Shop/UIMerchantItemMyShopWnd.h"
#include "UI/Shop/UIMerchantItemPurchaseWnd.h"
#include "UI/Shop/UIMerchantItemShopWnd.h"
#include "UI/Shop/UIMerchantItemWnd.h"
#include "UI/Shop/UIMerchantMirrorItemWnd.h"
#include "UI/Shop/UIMerchantShopMakeWnd.h"
#include "UI/Shop/UINoticeMessageBox_LockSellItem.h"
#include "UI/Skill/UISkillListWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UITab.h"
#include "UI/Items/UIItemStoreComboBox.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Login/UIPasswordWnd.h"
#include "Client/Session/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeDragState.h"
#include "UI/Shop/UIMerchantItemPurchaseMSGWnd.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Items/UIGradeEnchantWnd.h"
#include "UI/Items/UIRefiningWnd.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Shop/PriceCommission.h"
#include "UI/Shop/PriceColor.h"
#include "Utility/Text/GroupedInteger.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Tables/TipBoxTitles.h"
#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <list>
#include <mbstring.h>
#include <map>
#include <string>
#include <windows.h>
#include <timeapi.h>
#include "Contents/Items/ItemLink.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include "UI/Shop/UIBartermarketWnd.h"
#include "UI/Edit/UIEditPaint.inl"

#include "Resource/MSGSTRINGID.h"
// GLOBAL: gl_24653
int g_skillTooltipActive;

// GLOBAL: gl_24654
DWORD g_skillTooltipTick;


// GLOBAL: gl_25457
char g_skillListCount;

// GLOBAL: gl_25458
int g_skillUpgradeId;

// GLOBAL: gl_24651
static const std::string g_itemLinkOpen = "<ITEML>";
// GLOBAL: gl_24652
static const std::string g_itemLinkClose = "</ITEML>";
// GLOBAL: gl_24647
static const std::string g_editNaviTagBegin = "<NAVIL>";
// GLOBAL: gl_24648
static const std::string g_editNaviTagEnd = "</NAVIL>";

namespace {
#pragma pack(push, 1)
struct PACKET_CZ_INVENTORY_TAB {
  /* 0000 */ short packetType;
  /* 0002 */ unsigned short index;
  /* 0004 */ unsigned char favorite;
};
#pragma pack(pop)

static_assert(sizeof(PACKET_CZ_INVENTORY_TAB) == 5);

#pragma pack(push, 1)
struct ItemListChunkHeader {
  /* 0000 */ std::int16_t type;
  /* 0002 */ std::uint32_t size;

  ItemListChunkHeader() : type(0), size(0) {
  }

  ~ItemListChunkHeader() {
    this->type = 0;
    this->size = 0;
  }
};
#pragma pack(pop)

static_assert(sizeof(ItemListChunkHeader) == 6);

class EditLinkLayout {
public:
  virtual void Update() = 0;

protected:
  ~EditLinkLayout() = default;
};

  class LinkLayout : public EditLinkLayout {
  public:
    // INLINED
    explicit LinkLayout(UIEditCtrl& control) : m_control(control) {
    }

    // INLINED
    void Update() override {
      this->m_control.UpdateLinks();
    }

  private:
    UIEditCtrl& m_control;
  };
}

// INLINED
static void SetEditTooltip(UIEditCtrl& edit, const char* value) {
  if (value) {
    edit.tooltip = value;
  }
}

// INLINED
static void SetEditDisabled(UIEditCtrl& edit, unsigned char value) {
  edit.disabled = value;
  edit.Invalidate();
}

// INLINED
static void SetEditBackground(UIEditCtrl& edit, int red, int green, int blue) {
  edit.red = red;
  edit.green = green;
  edit.blue = blue;
  edit.Invalidate();
}

// INLINED
static void SetEditMaximum(UIEditCtrl& edit, int value) {
  edit.maximum = value;
  if (edit.maximum > 0x169) {
    edit.maximum = 0x169;
  }

  edit.text.reserve(edit.maximum);
}

// OFFSET: 0x0084F860
// FUNCTION: fn_10662
UIEditCtrl::UIEditCtrl(const UIEditCtrl& other) : UIWindow(0) {
  this->Border(other.borderRed, other.borderGreen, other.borderBlue);
  SetEditTooltip(*this, other.tooltip.c_str());
  SetEditDisabled(*this, other.disabled);
  this->preserveInput = other.preserveInput;
  SetEditMaximum(*this, other.maximum);
  this->text = other.text;
  this->anchor = other.anchor;
  this->cursor = other.cursor;
  this->mask = static_cast<signed char>(other.mask);
  this->flat = other.flat != 0;
  SetEditBackground(*this, other.red, other.green, other.blue);
  this->textColor = other.textColor;
  this->secondColor = other.secondColor;
  this->firstVisible = other.firstVisible;
  this->textY = other.textY;
  this->invalidatesParent = other.invalidatesParent;
  this->topPadding = other.topPadding;
  this->tagTextLength = other.tagTextLength;
}

// OFFSET: 0x0084FAE0
// FUNCTION: fn_10663
UIEditCtrl::UIEditCtrl() : UIWindow(0), borderRed(0), borderGreen(0), borderBlue(0), tooltip(""), disabled(0), preserveInput(0) {
  SetEditMaximum(*this, 255);
  this->cursor = 0;
  this->anchor = 0;
  this->mask = 0;
  this->flat = 1;
  this->red = 255;
  this->green = 255;
  this->blue = 255;
  this->textColor = 0;
  this->secondColor = 0xFFFFFFFF;
  this->firstVisible = 0;
  this->textY = 0;
  this->invalidatesParent = 0;
  this->topPadding = 0;
  this->tagTextLength = 0;
}

// OFFSET: 0x008517E0
// FUNCTION: fn_10694
UIEditCtrl::~UIEditCtrl() = default;

// OFFSET: 0x005C3210
// FUNCTION: fn_03552
bool UIEditCtrl::IsEditable() {
  return this->disabled == 0;
}

// OFFSET: 0x0085CCA0
// FUNCTION: fn_10807
void UIEditCtrl::Paint() {
  int backRed = this->red;
  int backGreen = this->green;
  int backBlue = this->blue;
  if (this->disabled) {
    backRed = backRed > 30 ? backRed - 30 : backRed;
    backGreen = backGreen > 30 ? backGreen - 30 : backGreen;
    if (backBlue > 30) {
      backBlue -= 30;
    }
  }

  this->textY = 0;

  if (this->flat == 0) {
    RECT rectangle{0, 0, this->geometry.width, this->geometry.height};
    this->FillSurface(RGB(this->borderRed, this->borderGreen, this->borderBlue), &rectangle);
    SetRect(&rectangle, 1, 1, this->geometry.width - 1, this->geometry.height - 1);
    this->FillSurface(RGB(backRed, backGreen, backBlue), &rectangle);
  } else {
    this->ClearSurface(RGB(backRed, backGreen, backBlue));
  }

  this->textY = this->topPadding + 2;
  this->PaintText();
}

// OFFSET: 0x0085AAB0
// FUNCTION: fn_10783
void UIEditCtrl::OnBeginEdit() {
  g_serverLanguage->ResetInput();
  g_serverLanguage->ReplaceInput(this->text.c_str());
  g_serverLanguage->SetPassword(this->mask != 0);
  this->SelectAll();
}

// OFFSET: 0x0085E3B0
// FUNCTION: fn_10826
void UIEditCtrl::OnFinishEdit(int) {
  this->Synchronize();
  this->Invalidate();
}

// OFFSET: 0x0085E400
// FUNCTION: fn_10828
void UIWhisperEditCtrl::OnFinishEdit(int retained) {
  this->AddHistory();
  this->Synchronize();
  this->Invalidate();
}

// OFFSET: 0x0085EC20
// FUNCTION: fn_10842
void UIEditCtrl::OnLBtnDown(int, int) {
  if (this->disabled == 0) {
    g_windowMgr.SetFocus(this);
  }
}

// OFFSET: 0x0085F390
// FUNCTION: fn_10860
void UIEditCtrl::OnMouseHover(int x, int y) {
  if (this->disabled != 0) {
    return;
  }

  this->OnMouseMove(x, y);

  if (!this->tooltip.empty()) {
    x = 10;
    y = -20;
    this->ToScreen(x, y);
    g_modeMgr.gameMode->ShowTooltip(this->tooltip.c_str(), x, y, -1, false, true);
  }
}

// INLINED
__forceinline void UIEditCtrl::InsertLink(const char* value) {
  const std::string name = g_navigationLinkParser->FormatNavigationLink(value, false);
  this->FitTextToWidth(name, this->geometry.width / 2);
  std::string display = "<";
  display += name;
  display += ">";
  std::string before;
  bool removed = false;
  std::size_t position = 0;
  std::size_t oldBytes = 0;

  for (auto entry = this->links.begin(); entry != this->links.end();) {
    UIWindow* window = *entry;

    if (window->geometry.id == 438) {
      removed = true;
      UINaviLinkButton* button = static_cast<UINaviLinkButton*>(window);
      const std::string linkName = g_navigationLinkParser->FormatNavigationLink(button->Link(), false);
      this->FitTextToWidth(linkName, this->geometry.width / 2);
      std::string old = "<";
      old += linkName;
      old += ">";

      oldBytes = std::strlen(old.c_str());
      position = this->text.find(old);

      if (position != std::string::npos) {
        before = this->text.substr(0, position);
      }

      this->RemoveChild(window, 1);
      entry = this->links.erase(entry);
    } else {
      ++entry;
    }
  }

  if (!_mbsstr(reinterpret_cast<const unsigned char*>(value), reinterpret_cast<const unsigned char*>(g_editNaviTagBegin.c_str())) &&
      !_mbsstr(reinterpret_cast<const unsigned char*>(value), reinterpret_cast<const unsigned char*>(g_editNaviTagEnd.c_str()))) {
    this->SetText(value);
    return;
  }

  std::string transformed;
  DisplayNavigationLink(transformed, value);
  const std::string prospective = this->text + transformed;

  if (static_cast<unsigned>(this->maximum) < prospective.size()) {
    return;
  }

  if (!removed) {
    std::string appended;
    DisplayNavigationLink(appended, value);
    this->text += appended;
  } else if (position != std::string::npos) {
    before += display;
    before += this->text.substr(position + oldBytes);
    this->text = before;
  }

  this->SetText(this->text.c_str());
  this->anchor = this->cursor = static_cast<int>(this->text.size());
  g_serverLanguage->Select(this->cursor, this->cursor);

  auto* button = new UINaviLinkButton();

  button->Configure(display.c_str(), 0x009E68F0, 0, 12, 0, 0);
  button->SetId(438);
  button->link = GetNavigationLinkParser()->ExtractNavigationLink(value);
  this->AddChild(button);
  this->links.push_back(button);
}

// OFFSET: 0x008641C0
// FUNCTION: fn_10914
int UIEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DEFPUSH:
    UIWindow::SendMsg(sender, message, a, b, c, d);
    if (this->preserveInput == 0) {
      g_serverLanguage->ResetInput();
    }
    return 0;
  case UM_TEXT_LBTN_UP:
    if (c > 0) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);

      if (window) {
        window->SendMsg(nullptr, UM_ITEMEXPLANATION, c, 0, 0, 0);
      }
    }
    break;
  case UM_INSERT_ITEM_LINK:
    this->InsertLink(reinterpret_cast<const char*>(a));
    break;
  }

  UIWindow::SendMsg(sender, message, a, b, c, d);
  return 0;
}

// OFFSET: 0x005C44E0
// FUNCTION: fn_03561
int UIEditCtrl::GetWindowType() {
  return 2;
}

// OFFSET: 0x008680D0
// FUNCTION: fn_10952
void UIEditCtrl::SetText(const char* value) {
  this->text.assign(value, std::strlen(value));
  this->Trim();

  if (g_windowMgr.GetHoverWindow() == this) {
    g_serverLanguage->ResetInput();
    g_serverLanguage->ReplaceInput(this->text.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x00856980
// FUNCTION: fn_10755
void UIEditCtrl::PaintText() {
  this->PaintTextWithPolicy(nullptr, 0, false);
}

// OFFSET: 0x00858D50
// FUNCTION: fn_10768
__declspec(noinline) int UIEditCtrl::Length() const {
  return static_cast<int>(g_windowMgr.GetHoverWindow() == this ? g_serverLanguage->text.size() : this->text.size());
}

// OFFSET: 0x00858D20
// FUNCTION: fn_10767
__declspec(noinline) const char* UIEditCtrl::Text() {
  if (g_windowMgr.GetHoverWindow() == this) {
    this->Synchronize();
  }

  return this->text.c_str();
}

// OFFSET: 0x00858D80
// FUNCTION: fn_10769
std::string UIEditCtrl::LinkedText() {
  return this->linkedText;
}

// OFFSET: 0x00858F30
// FUNCTION: fn_10771
std::string UIEditCtrl3::TakePendingText() {
  std::string result;
  for (std::size_t i = 0; i < this->pendingText.size(); ++i) {
    result.append(this->pendingText[i]);
  }
  this->pendingText.clear();
  return result;
}

// OFFSET: 0x008590E0
// FUNCTION: fn_10772
bool HasItemLinkTag(const std::string& text) {
  return text.find(g_itemLinkOpen) != std::string::npos || text.find(g_itemLinkClose) != std::string::npos;
}

// OFFSET: 0x00859170
// FUNCTION: fn_10773
int __stdcall HasNaviLinkTag(const std::string& text) {
  return text.find(g_editNaviTagBegin) != std::string::npos || text.find(g_editNaviTagEnd) != std::string::npos;
}

// OFFSET: 0x00865DD0
// FUNCTION: fn_10937
__declspec(noinline) void UIEditCtrl::Background(int red, int green, int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->Invalidate();
}

// OFFSET: 0x00865E00
// FUNCTION: fn_10938
void UIEditCtrl::Border(int red, int green, int blue) {
  this->borderRed = red;
  this->borderGreen = green;
  this->borderBlue = blue;
}

// OFFSET: 0x00865E30
// FUNCTION: fn_10939
void UIEditCtrl3::SetTagColor(std::uint32_t color) {
  for (auto it = this->entries.begin(); it != this->entries.end(); ++it) {
    for (unsigned index = 0; index < it->size(); ++index) {
      (*it)[index]->m_normalColor = color;
      (*it)[index]->m_color = color;
      (*it)[index]->m_hoverColor = color;
    }
  }
}

// INLINED
void UIEditCtrl::TextColor(std::uint32_t primary, std::uint32_t secondary) {
  this->secondColor = secondary;
  this->textColor = primary;
  this->Invalidate();
}

// OFFSET: 0x00868570
// FUNCTION: fn_10960
__declspec(noinline) void UIEditCtrl::TextColor(unsigned char red, unsigned char green, unsigned char blue, std::uint32_t secondary) {
  this->textColor = static_cast<std::uint32_t>(red) | (static_cast<std::uint32_t>(green) << 8) | (static_cast<std::uint32_t>(blue) << 16);
  this->secondColor = secondary;
  this->Invalidate();
}

// OFFSET: 0x00856EB0
// FUNCTION: fn_10756
void UIEditCtrl3::DrawBlinkingCaret(const std::uint32_t& color) {
  if (1 == timeGetTime() / 500 % 2) {
    return;
  }

  if (g_windowMgr.IsHoverOnWindow01C8Button() && !g_windowMgr.inputSuggestionsEnabled) {
    return;
  }

  this->DrawTextAt(1, 0, "|", 0, 0, 12, color, 0, 0);
}

// OFFSET: 0x00856F10
// FUNCTION: fn_10757
void UIEditCtrl3::PlaceTags(const int& x, const int& y, const int& firstLine, const int& endLine, const int& line, const int&) {
  for (auto it = this->entries.begin(); it != this->entries.end(); ++it) {
    for (unsigned index = 0; index < it->size(); ++index) {
      UIItemTagButton* tag = (*it)[index];

      if (tag->line < firstLine || tag->line > endLine - 1) {
        tag->Move(-100, -100);
      } else if (line == tag->line) {
        tag->Move(x - 1 + tag->column, y);
      }
    }
  }
}

// OFFSET: 0x00856FB0
// FUNCTION: fn_10758
void UIEditCtrl::DrawSelectionAt(int x, int y, int width) {
  const int extent = x < 3 ? x - 3 + width : width;
  const int left = x >= 3 ? x : 3;
  const int right = this->geometry.width;

  if (left < right && extent > 0) {
    unsigned red, green, blue;
    g_skinInfo.PaletteChannels(6, 2, red, green, blue);
    this->FillRegion(left, y, left + extent > right ? right - left : extent, 14, RGB(red, green, blue));
  }
}

// OFFSET: 0x00857040
// FUNCTION: fn_10759
void UIEditCtrl::DrawSelection(int x, int width) {
  const int extent = x < 3 ? x - 3 + width : width;
  const int left = x >= 3 ? x : 3;
  const int right = this->geometry.width;

  if (left < right && extent > 0) {
    unsigned red, green, blue;
    g_skinInfo.PaletteChannels(6, 2, red, green, blue);
    this->FillRegion(left, 0, left + extent > right ? right - left : extent, this->geometry.height, RGB(red, green, blue));
  }
}

// OFFSET: 0x0096F410
// FUNCTION: fn_13233
UISkillListWnd::UISkillListWnd() : UIFrameWnd(0) {
  this->buttons[0] = nullptr;
  this->buttons[1] = nullptr;
  this->hovered = -1;
  this->columns = 1;
  this->saved = nullptr;
}

// INLINED
UIEditCtrlWheel::UIEditCtrlWheel() : UIEditCtrl() {
}

UIEditCtrlWheel::~UIEditCtrlWheel() = default;

// OFFSET: 0x0098A240
// FUNCTION: fn_13408
void UIEditCtrlWheel::OnWheel(int value) {
  if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00869E30
// FUNCTION: fn_10975
void UIChatEditCtrl::PushHistory() {
  if (static_cast<int>(g_windowMgr.GetHoverWindow() == this ? g_serverLanguage->text.size() : this->text.size()) == 0) {
    return;
  }

  if (this->historyIndex == static_cast<int>(this->history.size())) {
    if (this->history.size() >= static_cast<unsigned>(this->historyLimit)) {
      --this->historyIndex;
      this->history.erase(this->history.begin());
    }
    if (g_windowMgr.GetHoverWindow() == this) {
      this->Synchronize();
    }
    this->history.push_back(std::string(this->text.c_str()));
  } else {
    if (g_windowMgr.GetHoverWindow() == this) {
      this->Synchronize();
    }
    this->history[this->historyIndex] = this->text.c_str();
  }

  ++this->historyIndex;
  if (this->historyIndex == static_cast<int>(this->history.size())) {
    this->SetText("");
  } else {
    this->SetText(this->history[this->historyIndex].c_str());
  }
}

// OFFSET: 0x0086A060
// FUNCTION: fn_10976
void UIWhisperEditCtrl::AddHistory() {
  if (static_cast<int>(g_windowMgr.GetHoverWindow() == this ? g_serverLanguage->text.size() : this->text.size()) == 0) {
    this->historyIndex = static_cast<int>(this->history.size());
    return;
  }

  for (std::size_t index = 0; index < this->history.size(); ++index) {
    if (g_windowMgr.GetHoverWindow() == this) {
      this->Synchronize();
    }
    if (this->history[index] == this->text.c_str()) {
      return;
    }
  }

  if (this->historyIndex == static_cast<int>(this->history.size())) {
    if (this->history.size() >= 30) {
      --this->historyIndex;
      this->history.erase(this->history.begin());
    }
    if (g_windowMgr.GetHoverWindow() == this) {
      this->Synchronize();
    }
    this->history.push_back(std::string(this->text.c_str()));
  } else {
    bool found = false;
    for (std::size_t index = 0; index < this->history.size(); ++index) {
      if (g_windowMgr.GetHoverWindow() == this) {
        this->Synchronize();
      }
      if (this->history[index] == this->text.c_str()) {
        this->historyIndex = static_cast<int>(index);
        found = true;
        break;
      }
    }

    if (!found) {
      if (g_windowMgr.GetHoverWindow() == this) {
        this->Synchronize();
      }
      this->history.push_back(std::string(this->text.c_str()));
      this->historyIndex = static_cast<int>(this->history.size()) - 1;
    }
  }

  if (this->historyIndex == static_cast<int>(this->history.size())) {
    this->SetText("");
  } else {
    this->SetText(this->history[this->historyIndex].c_str());
  }
}

// OFFSET: 0x0086A550
// FUNCTION: fn_10977
void UIEditCtrl::MoveCandidateWindow(int x) {
  UIWindow* window = g_windowMgr.candidateWnd;
  if (window == nullptr) {
    return;
  }

  int left = x;
  int top = this->geometry.height + 6;
  this->ToScreen(left, top);

  if (window->geometry.width + left > g_renderer->viewport.width) {
    left = g_renderer->viewport.width - window->geometry.width;
  }

  if (window->geometry.height + top > g_renderer->viewport.height) {
    top += -12 - this->geometry.height - window->geometry.height;
  }

  window->Move(left, top);
}

// OFFSET: 0x0086A5E0
// FUNCTION: fn_10978
void UIEditCtrl::MoveReadingWindow(int x) {
  UIWindow* window = g_windowMgr.keyStrokeWnd;
  if (window == nullptr) {
    return;
  }

  int left = x;
  int top = this->geometry.height + 6;
  this->ToScreen(left, top);

  if (window->geometry.width + left > g_renderer->viewport.width) {
    left = g_renderer->viewport.width - window->geometry.width;
  }

  if (window->geometry.height + top > g_renderer->viewport.height) {
    top += -12 - this->geometry.height - window->geometry.height;
  }

  window->Move(left, top);
}

// OFFSET: 0x008570D0
// FUNCTION: fn_10760
void UIBmpTextButton2::DrawLabels() {
  for (unsigned index = 0; index < this->labels.size(); ++index) {
    if (this->labels[index].text.size() == 0) {
      continue;
    }

    int x = this->labels[index].x;
    int y = this->labels[index].y;
    int shadowX = 0;
    int shadowY = 0;

    if (x == -1000) {
      const int width = this->TextWidth(this->labels[index].text.c_str(), this->labels[index].text.size(), 0, this->labels[index].height, this->labels[index].bold, 0);
      x = static_cast<int>((this->geometry.width - width) * 0.5);
    }

    if (y == -1000) {
      const int height = this->TextHeight(this->labels[index].text.c_str(), this->labels[index].text.size(), 0, this->labels[index].height, this->labels[index].bold, 0);
      y = static_cast<int>((this->geometry.height - height) * 0.5);
    }

    std::uint32_t color = this->labels[index].color;
    std::uint32_t shadowColor = 0xE6E6E6;

    switch (this->geometry.state) {
    case 0:
      shadowX = x + 1;
      shadowY = y + 1;
      shadowColor = 0xD7D7D7;
      break;
    case 1:
      shadowX = x + 1;
      shadowY = y + 1;
      shadowColor = 0xC7A796;
      break;
    case 2:
      shadowX = x;
      shadowY = y;
      shadowColor = 0xFFFFFF;
      ++x;
      ++y;
      break;
    case 3:
      color = 0xADADAD;
      shadowX = x + 1;
      shadowY = y + 1;
      shadowColor = 0xD7D7D7;
      break;
    }

    if (this->labels[index].shadow == 1) {
      this->DrawTextAt(shadowX, shadowY, this->labels[index].text.c_str(), 0, 0, this->labels[index].height, shadowColor, this->labels[index].bold, 0);
    }

    this->DrawTextAt(x, y, this->labels[index].text.c_str(), 0, 0, this->labels[index].height, color, this->labels[index].bold, 0);
  }
}

// OFFSET: 0x008572F0
// FUNCTION: fn_10761
void UITextureButton::VisitChildren() {
  CTexture* texture = nullptr;

  switch (this->geometry.state) {
  case 0:
    if (this->toggleEnabled == 1 && this->toggled == 1) {
      texture = g_texMgr.Load(this->pressedTexture.c_str(), 0, false, true, false);
    } else {
      texture = g_texMgr.Load(this->normalTexture.c_str(), 0, false, true, false);
    }
    break;
  case 1:
    texture = g_texMgr.Load(this->hoverTexture.c_str(), 0, false, true, false);
    break;
  case 2:
    texture = g_texMgr.Load(this->pressedTexture.c_str(), 0, false, true, false);
    break;
  }

  if (this->m_disabled == 1 && this->disabledTexture.size() > 0) {
    texture = g_texMgr.Load(this->disabledTexture.c_str(), 0, false, true, false);
  }

  DrawTextureRegion(
    static_cast<float>(this->geometry.x), static_cast<float>(this->geometry.y), static_cast<float>(texture->Width()),
    static_cast<float>(texture->Height()), 0xFFFFFFFF, texture
  );
}

// OFFSET: 0x00858B80
// FUNCTION: fn_10764
std::string UINaviLinkButton::Link() {
  return this->link;
}

// OFFSET: 0x00858BE0
// FUNCTION: fn_10765
int UISpriteButton::FrameHeight(int action, int frame) {
  auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->spriteName.c_str()));
  auto* actions = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->actionName.c_str()));
  return sprite->images[0][actions->Frame(action, frame).cells[0].image]->height;
}

// OFFSET: 0x00858C50
// FUNCTION: fn_10766
int UISpriteButton::FrameWidth(int action, int frame) {
  auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->spriteName.c_str()));
  auto* actions = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->actionName.c_str()));
  return sprite->images[0][actions->Frame(action, frame).cells[0].image]->width;
}

// OFFSET: 0x0097D220
// FUNCTION: fn_13325
void UINoticeMessageBox_LockSellItem::OnCreate(int, int) {
  auto* button = new UIBitmapButton();
  button->SetBitmap("유저인터페이스\\inventory\\bt_close.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\inventory\\bt_close.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\inventory\\bt_close.bmp", 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(229, 4);
  button->SetId(201);
  this->AddChild(button);
}

// OFFSET: 0x0097E3E0
// FUNCTION: fn_13327
void UIItemWnd::OnDestroy() {
}

// OFFSET: 0x00896BC0
// FUNCTION: fn_11518
void UIEditCtrl_Custom::Synchronize() {
  const int previousCursor = this->cursor;
  g_serverLanguage->CopyDisplayText(this->text);
  g_serverLanguage->Selection(this->anchor, this->cursor);
  this->Trim();

  const auto length = static_cast<unsigned>(this->text.size());
  bool clamped = false;

  if (static_cast<unsigned>(this->cursor) > length || static_cast<unsigned>(this->anchor) > length) {
    this->anchor = std::min<unsigned>(this->anchor, length);
    this->cursor = std::min<unsigned>(this->cursor, length);
    g_serverLanguage->Select(this->anchor, this->cursor);
    clamped = true;
  }

  if (this->cursor == 0) {
    this->firstVisible = 0;
    return;
  }

  std::string visible;

  if (static_cast<unsigned>(std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor)) >= this->text.size()) {
    this->firstVisible = 0;
  }

  if (this->cursor > previousCursor || clamped == true) {
    int before = 0;

    if (this->firstVisible != 0) {
      before = this->TextWidth(this->text.c_str(), this->firstVisible, 0, 12, this->labelBold, 0);
    }

    const int position = this->TextWidth(this->text.c_str(), this->cursor, 0, 12, this->labelBold, 0);

    if (position + 3 - before > this->geometry.width - 12) {
      const char* end = this->text.c_str() + this->text.size();
      const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);

      while (this->TextWidth(next, 0, 0, 12, this->labelBold, 0) + 3 > this->geometry.width - 12 && next < end) {
        next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
      }

      const auto tail = static_cast<unsigned>(std::strlen(next));
      const auto scrolled = static_cast<unsigned>(this->firstVisible + (this->cursor - previousCursor));
      this->firstVisible = std::min<unsigned>(scrolled, this->cursor - tail);
    }
  } else if (this->cursor < previousCursor) {
    if (this->TextWidth(this->text.c_str(), this->cursor, 0, 12, this->labelBold, 0) + 3 < this->geometry.width - 12 && this->firstVisible > this->cursor) {
      this->firstVisible += this->cursor - previousCursor;
    }

    this->firstVisible = std::max(this->firstVisible, 0);
  }
}

// OFFSET: 0x00972520
// FUNCTION: fn_13272
void UIItemStoreFindWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00972570
// FUNCTION: fn_13273
void UIItemStoreSubWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
  case 9:
    break;
  default:
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (static_cast<CGameMode*>(mode)->ConfirmDraggedItemUse()) {
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 7, 0, 0, 0);
}

// OFFSET: 0x00972630
// FUNCTION: fn_13274
void UIItemStoreWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  char text[250];
  if (g_session.m_buyingStoreMode) {
    if (g_session.m_buyingStoreNoPermission == 1) {
      std::memset(text, 0, 250);
      std::sprintf(text, LookupMessage(MSI_GUILD_STORAGE_PERMISSON_WARNING_MSG));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      return;
    }

    if (!g_session.m_buyingStoreDbNumber && !g_session.m_buyingStoreNoPermission) {
      std::memset(text, 0, 250);
      std::sprintf(text, LookupMessage(MSI_BARGAINSALE_TOOLWND_DB_NUMBER));
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      return;
    }
  }

  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
  case 9:
    break;
  default:
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (static_cast<CGameMode*>(mode)->ConfirmDraggedItemUse()) {
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 7, 0, 0, 0);
}

// OFFSET: 0x009727F0
// FUNCTION: fn_13275
void UIItemWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
    if (drag->sourceWindow == this->tabs) {
      PACKET_CZ_INVENTORY_TAB packet;
      packet.packetType = ID_CZ_INVENTORY_TAB;
      packet.index = static_cast<unsigned short>(drag->id);
      packet.favorite = this->tabs->TabAt(x, y) != 3;
      GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
    }
    break;
  case 2:
  case 17:
  case 18:
  case 24:
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  case 6:
  case 9:
  case 15:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 6, 0, 0, 0);
    return;
  case 14: {
    UIWindow* target = static_cast<UIWindow*>(g_windowMgr.makeTargetProcessWnd);

    if (target) {
      target->SendMsg(nullptr, UM_CLEAR_ITEM, reinterpret_cast<std::intptr_t>(drag->name.c_str()), 0, 0, 0);
    }
    break;
  }
  case 28: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_REFININGWND);

    if (window) {
      static_cast<UIRefiningWnd*>(window)->Reset();
    }
    break;
  }
  case 33: {
    UIWindow* window = g_windowMgr.LookupWindow(WID_GRADEENCHANTWND);

    if (window) {
      static_cast<UIGradeEnchantWnd*>(window)->ResetItem();
    }
    break;
  }
  }

  g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00989080
// FUNCTION: fn_13396
void UIItemStoreFindWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.itemWnd) {
        if (mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_BODY, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.merchantItemWnd && mode->inventoryChanged) {
        mode->inventoryChanged = 0;
        mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_CART, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.itemWnd) {
        mode->SendMsg(MM_MOVE_ITEM_TO_PERSONAL, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989210
// FUNCTION: fn_13397
void UIItemStoreSubWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.itemWnd) {
        if (mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_BODY, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.merchantItemWnd && mode->inventoryChanged) {
        mode->inventoryChanged = 0;
        mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_CART, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.itemWnd) {
        mode->SendMsg(MM_MOVE_ITEM_TO_PERSONAL, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x009893A0
// FUNCTION: fn_13398
void UIItemStoreWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.itemWnd) {
        if (mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_BODY, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.merchantItemWnd && mode->inventoryChanged) {
        mode->inventoryChanged = 0;
        mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_CART, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      mode->SendMsg(MM_MOVE_ITEM_TO_PERSONAL, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989520
// FUNCTION: fn_13399
void UIItemWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.sayDialogWnd || g_windowMgr.choose3Wnd) {
        return;
      }

      item = g_session.LookupClientItemByIdentity(item.identity[1]);
      mode->drag.BeginItem(g_windowMgr, 1, item, item.quantity, -1);
      if (mode->ConfirmDraggedItemUse()) {
        return;
      }

      if (g_windowMgr.itemStoreWnd) {
        if (mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_ADD_ITEM_TO_STORE, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
        if (mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_ADD_EXCHANGE_ITEM, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.merchantItemWnd && mode->inventoryChanged) {
        mode->inventoryChanged = 0;
        mode->SendMsg(MM_MOVE_ITEM_FROM_BODY_TO_CART, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (this->tabs) {
        mode->SendMsg(MM_INVENTORY_TAB, static_cast<int>(item.identity[1]), this->tabs->activeTabIndex == 3, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_CONTROL))) {
      if (item.identity[1] > 0 && !g_session.IsPetEgg(item.GetItemId())) {
        const int magnifiers =
          LookupClientItem(g_session.m_inventoryItems, 611, 0).quantity + LookupClientItem(g_session.m_inventoryItems, 12325, 0).quantity;
        if (magnifiers <= 0) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_UISCRIPT_ERROR_GUIDE)), 255, 0, 0, 0);
        }

        if (this->categoryFilter == 1 && !item.identified) {
          struct {
            unsigned short packetType;
            unsigned short index;
          } packet;
          packet.packetType = ID_CZ_REQ_ONECLICK_ITEMIDENTIFY;
          packet.index = static_cast<unsigned short>(item.identity[1]);
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
        }
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989960
// FUNCTION: fn_13401
void UIMerchantItemPurchaseWnd::OnRBtnDown(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.merchantItemShopWnd) {
        item = g_session.FindItem1760ByItemId(item.itemId.c_str());
        mode->BeginDragItem(13, item, item.quantity, -1);
        static_cast<UIWindow*>(g_windowMgr.merchantItemShopWnd)->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989B00
// FUNCTION: fn_13402
void UIMerchantItemShopWnd::OnRBtnDown(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.merchantItemPurchaseWnd) {
        mode->BeginDragItem(12, item, item.quantity, -1);
        static_cast<UIWindow*>(g_windowMgr.merchantItemPurchaseWnd)->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989C40
// FUNCTION: fn_13403
void UIMerchantItemWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (!g_windowMgr.LookupWindow(WID_EXCHANGEUI) && !g_windowMgr.merchantShopMakeWnd) {
        if (g_windowMgr.itemStoreWnd) {
          if (mode->inventoryChanged) {
            mode->inventoryChanged = 0;
            mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_STORE, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
          }
        } else if (g_windowMgr.itemWnd && mode->inventoryChanged) {
          mode->inventoryChanged = 0;
          mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_BODY, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
        }
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.itemWnd) {
        mode->SendMsg(MM_MOVE_ITEM_TO_PERSONAL_FROM_STORAGE, static_cast<int>(item.identity[1]), item.quantity, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989DF0
// FUNCTION: fn_13404
void UIMerchantMirrorItemWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.merchantShopMakeWnd) {
        item = g_session.FindMerchantMyShopItem(item.identity[1]);
        mode->drag.BeginItem(g_windowMgr, 11, item, item.quantity, -1);
        static_cast<UIWindow*>(g_windowMgr.merchantShopMakeWnd)->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      } else if (g_windowMgr.buyingShopMakeWnd) {
        item = g_session.FindMerchantMyShopItem(item.identity[1]);
        mode->drag.BeginItem(g_windowMgr, 20, item, item.quantity, -1);
        static_cast<UIWindow*>(g_windowMgr.buyingShopMakeWnd)->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x0096F200
// FUNCTION: fn_13231
UIMerchantShopMakeWnd::UIMerchantShopMakeWnd(int mode) : UIFrameWnd(0), mode(mode) {
  this->scrollBar = nullptr;
  this->scroll = 0;
  this->columns = 1;
  this->minimumRows = 4;
  this->scrollable = 0;
  this->saved = nullptr;
  this->editIndex = -1;
  this->notice = nullptr;
  this->initialHeight = 0;
  this->visibleRows = 4;
  this->maxVisibleRows = 4;
  this->actionButtons[0] = nullptr;
  this->actionButtons[1] = nullptr;
  this->previousPriceLength = 0;
  this->previousPriceText.clear();
  this->itemsSet = 0;
  this->suspended = 0;
  this->selectedRow = -1;
  this->selectedColumn = -1;
  this->actionButton = nullptr;
}

// OFFSET: 0x00990090
// FUNCTION: fn_13427
int UIMerchantItemMyShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETTITLE: {
    char text[40];
    std::sprintf(text, "%s : %s", LookupMessage(this->buyingShop != 0 ? MSI_BUYINGSTORE_MYSHOPWND_TITLE : MSI_ITEM_MY_STORE), g_session.storeTitle.c_str());
    this->title->SetTextFitted(text);
    return 0;
  }
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantItemMyShopWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 240 ? c : 240;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->visibleRows = static_cast<unsigned int>(newHeight - this->footerHeight - 17) / 32;
        this->columns = (newWidth - 40) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->footerHeight - 17);
        this->saleAlarm->Move(12, this->buyingShop != 0 ? this->geometry.height - 12 : this->geometry.height - 18);
        this->buttons[0]->Move(this->geometry.width - 61, this->geometry.height - 24);
        if (this->actionButton) {
          this->actionButton->Move(this->geometry.width - this->actionButton->geometry.width - 66, this->geometry.height - 24);
        }
        if (this->buyingShop != 0) {
          this->notice->Move(12, this->geometry.height - 26);
        }
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_COMMAND: {
    CMode* mode = g_modeMgr.gameMode;
    this->RecordResponse(static_cast<int>(a), b, c, d);
    switch (a) {
    case ID_CLOSE: {
      mode = g_modeMgr.gameMode;
      int windowId;
      if (this->buyingShop != 0) {
        if (this->shopOpen == 1) {
          g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1);
          mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
          goto closed;
        }

        mode->SendMsg(MM_REQ_CLOSE_BUYING_STORE, 0, 0, 0, 0);
        mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1);
        windowId = 258;
      } else {
        if (this->shopOpen == 1) {
          g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND);
          mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
          goto closed;
        }

        mode->SendMsg(MM_REQ_CLOSESTORE, 0, 0, 0, 0);
        mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND);
        windowId = 257;
      }

      {
        UIWindow* window = g_windowMgr.EnsureWindow(windowId);
        if (window) {
          window->Show(1);
          window->Move(this->geometry.x, this->geometry.y);
        }
      }

    closed:
      if (this->shopOpen == 0) {
        g_session.m_storeInfoBackup.Save(this->buyingShop);
      }
      g_session.ClearMerchantPurchaseItems();
      return 0;
    }
    case ID_CHECK:
      g_session.m_muteSellSound = !g_session.m_muteSellSound;
      return 0;
    case ID_CLOSE_OFFLINE_STORE:
      if (g_windowMgr.Confirm(LookupMessage(MSI_LIMIT_USER_MSG), 2, true, false, 0, "Message", 280, 120, 0) == ID_YES) {
        mode->SendMsg(MM_CLOSE_OFFLINE_STORE, this->offlineStoreId, 0, 0, 0);
        mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
        if (this->buyingShop == 1) {
          g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND_MODE_1);
        } else {
          g_windowMgr.CloseWindow(WID_MERCHANTITEMMYSHOPWND);
        }
        return 0;
      }
      break;
    }
    break;
  }
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList9Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1758At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_SET_BUYING_SHOP_ZENY: {
    char grouped[80];
    char text[80];
    std::sprintf(text, LookupMessage(MSI_BUYINGSTORE_MYSHOPWND_LIMITZENY), FormatGroupedInteger(static_cast<int>(a), grouped, 80));
    this->notice->SetTextAutoWidth(text);
    this->notice->Move(12, this->geometry.height - 26);
    this->buyingZeny = static_cast<int>(a);
    return 0;
  }
  case UM_SETNAID:
    this->offlineStoreId = static_cast<int>(a);
    return 0;
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (static_cast<std::uint16_t>(header.type) == 21000 || static_cast<std::uint16_t>(header.type) == 21050) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        switch (header.type) {
        case 21001:
        case 21051:
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          this->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
          return 0;
        case 21052:
          std::memcpy(&this->buyingZeny, bytes + offset, header.size);
          offset += header.size;
          this->SendMsg(nullptr, UM_SET_BUYING_SHOP_ZENY, this->buyingZeny, 0, 0, 0);
          break;
        default:
          offset += header.size;
          break;
        }
      }
    }
    return 0;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0098B980
// FUNCTION: fn_13420
int UIItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETTITLE: {
    this->cashShopMode = a;
    if (a) {
      this->quantityEdit->Move(110, this->geometry.height - 50);
    } else {
      this->quantityEdit->Move(-100, -100);
    }

    const char* format;
    if (!this->cashShopMode) {
      format = "Total : %d Zeny";
    } else if (g_serviceType == 6) {
      format = "Total : %d Koin";
    } else if (g_serviceType == 0) {
      format = g_session.m_channelCopId == 1 ? "Total : %d 한코인" : "Total : %d Cash";
    } else {
      format = "Total : %d Point";
    }

    char text[80];
    std::sprintf(text, format, 0);
    this->title->SetTextAutoWidth(text);
    return 0;
  }
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemPurchaseWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    switch (drag.type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      if (g_session.GetItem1720QuantityByItemId(drag.name.c_str())) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EQUIPITEM_OLNY_ONE)), 0xFF, 0, 0, 0);
        return 0;
      }
      break;
    }

    ITEM_INFO item;
    item.Assign(drag);
    int total = g_session.GetItem1720Total();
    int budget = g_session.m_zeny;
    if (this->cashShopMode) {
      if (g_session.GetList2Size() == 10) {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>("한 번에 10개 이상 구매 할 수 없습니다."), 0xFF, 0, 0, 0);
        return 0;
      }
      budget = g_session.m_pendingGid + g_session.m_pendingAid;
    }

    total += drag.quantity * drag.price;
    int owned = 0;
    for (std::list<ITEM_INFO>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
      if (it->itemId == item.itemId) {
        owned = it->quantity;
        break;
      }
    }

    if (owned + item.quantity > 30000) {
      item.quantity += 30000 - (owned + item.quantity);
      char text[200];
      std::sprintf(text, LookupMessage(MSI_LIMIT_BUY_ITEM2), 30000);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
    }

    if (item.quantity > 0) {
      if (budget >= total) {
        g_session.MergeItem1720ByName(item);
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      } else {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(this->cashShopMode ? MSI_BUY_CASH_FAIL_MONEY : MSI_INSUFFICIENT_MONEY)), 0xFF, 0, 0, 0);
      }
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == ID_OK) {
      if (!this->cashShopMode) {
        mode->SendMsg(MM_REQ_PURCHASE, 0, 0, 0, 0);
      } else {
        if (!g_session.GetList2Size()) {
          break;
        }

        int amount = std::atoi(this->quantityEdit->Text());
        if (amount < 0) {
          amount = 0;
        }
        if (amount > g_session.m_pendingGid) {
          amount = g_session.m_pendingGid;
        }
        if (amount > g_session.GetItem1720Total()) {
          amount = g_session.GetItem1720Total();
        }

        char text[16];
        _itoa(amount, text, 10);
        g_windowMgr.SetFocus(this->quantityEdit);
        this->quantityEdit->SetText(text);
        g_windowMgr.SetFocus(nullptr);

        const int total = g_session.GetItem1720Total();
        if (g_session.m_pendingGid + g_session.m_pendingAid < total) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUY_CASH_FAIL_MONEY)), 0xFF, 0, 0, 0);
          break;
        }

        if (amount == 0 && g_session.m_pendingAid < total) {
          if (g_windowMgr.Confirm(LookupMessage(MSI_BUY_TO_FREE_POINT), 0, true, false, 0, "Message", 280, 120, 0)) {
            break;
          }
        }

        std::string confirm;
        const int price = g_session.GetItem1720Total();
        if (amount > 0) {
          confirm = FormatOwnedString(LookupMessage(MSI_CHANGE_MATERIAL_INVSORT), price - amount, amount);
        } else {
          confirm = FormatOwnedString(LookupMessage(MSI_BILLING_INFO_URL), price);
        }

        if (g_windowMgr.Confirm(confirm.c_str(), 2, true, true, 0, "Message", 280, 120, 0) == ID_YES) {
          g_windowMgr.SetFocus(this->quantityEdit);
          this->quantityEdit->SetText("0");
          mode->SendMsg(MM_REQ_BUY_CASH_POINT, amount, 0, 0, 0);
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
        return 0;
      }
    } else if (a == ID_CANCEL) {
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
      g_windowMgr.CloseWindow(WID_ITEMPURCHASEWND);
    } else {
      if (a == ID_APPLY && this->cashShopMode) {
        int amount = std::atoi(this->quantityEdit->Text());
        if (amount < 0) {
          amount = 0;
        }
        if (amount > g_session.m_pendingGid) {
          amount = g_session.m_pendingGid;
        }

        char text[12];
        _itoa(amount, text, 10);
        this->quantityEdit->SetText(text);
        g_windowMgr.SetFocus(nullptr);
        return 0;
      }
      break;
    }

    mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    g_session.ClearTradeItems();
    return 0;
  }
  case UM_RESIZE:
    if (a == 3) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (this->geometry.height - 58) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 58);
        this->UpdateScrollBar();
      }

      this->title->Move(12, this->geometry.height - 20);
      if (this->cashShopMode) {
        this->quantityEdit->Move(110, this->geometry.height - 50);
      } else {
        this->quantityEdit->Move(-100, -100);
      }

      const int positions[2][2] = {
        {this->geometry.width - 106, this->geometry.height - 24},
        {this->geometry.width - 61, this->geometry.height - 24},
      };
      for (int i = 0; i < 2; ++i) {
        this->buttons[i]->Move(positions[i][0], positions[i][1]);
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    int total = 0;
    const int size = g_session.GetList2Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1720At(i);
      if (item.identity[1] != 0) {
        total += item.quantity * item.prices[1];
        this->items.push_back(item);
      }
    }

    const char* format;
    if (!this->cashShopMode) {
      format = "Total : %s Zeny";
    } else if (g_serviceType == 6) {
      format = "Total : %s Koin";
    } else if (g_serviceType == 0) {
      format = g_session.m_channelCopId == 1 ? "Total : %s 한코인" : "Total : %s 캐시";
    } else {
      format = "Total : %s Point";
    }

    char grouped[64];
    char text[80];
    std::sprintf(text, format, FormatGroupedInteger(total, grouped, 64));
    this->title->SetTextAutoWidth(text);
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20700) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 20701) {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
          if (window) {
            window->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }

          this->SendMsg(nullptr, UM_SETTITLE, 0, 0, 0, 0);
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          break;
        }

        offset += header.size;
      }
    }
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0098AB40
// FUNCTION: fn_13419
int UIItemDropCntWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DEFPUSH:
    if (this->m_message0 != 0 && this->geometry.visible != 0) {
      this->SendMsg(this, UM_COMMAND, this->m_message0, 0, 0, 0);
      return 0;
    }
    return 0;
  case UM_SETSTATE:
    this->geometry.state = static_cast<std::uint32_t>(a);
    if (a == 20) {
      char text[24];
      std::sprintf(text, "%d", b);
      this->m_edit->SetText(text);
      if (this->geometry.state == 20) {
        this->m_edit->SetText(IntToString(static_cast<int>(b), 0).c_str());
        return 0;
      }
    }
    return 0;
  case UM_SETTEXT:
    if (b != 0) {
      this->itemTitle->SetText(reinterpret_cast<const char*>(b), 0);
      this->Invalidate();
      return 0;
    }
    return 0;
  case UM_COMMAND: {
    if (a != ID_OK || this->m_edit->Length() == 0) {
      return 0;
    }

    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (g_windowMgr.hoverWindow == this->m_edit) {
      this->m_edit->Synchronize();
    }
    int quantity = std::atoi(this->m_edit->text.c_str());
    this->RecordResponse(184, 100, quantity, this->geometry.state);
    if (g_replayAnimationScaling != 0 && b == 100) {
      this->geometry.state = d;
      mode->replayDragState[0] = 0;
      quantity = static_cast<int>(c);
    }

    UIWindow* window;
    int windowId;
    switch (this->geometry.state) {
    case 0:
      if (quantity <= 0 || quantity > mode->drag.quantity) {
        goto invalid;
      }
      mode->SendMsg(MM_ITEMTHROW_THROUGH_DROPCNTWND, quantity, 0, 0, 0);
      break;
    case 1:
      if (quantity <= 0) {
        goto invalid;
      }
      if (quantity > 30000) {
        mode->drag.quantity = 30000;
      } else {
        mode->drag.quantity = quantity;
      }
      if (quantity >= 30000) {
        char text[200];
        std::sprintf(text, LookupMessage(MSI_LIMIT_BUY_ITEM2), 30000);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      }
      window = g_windowMgr.itemPurchaseWnd;
      goto notify;
    case 2:
    case 16: {
      const int owned = g_session.GetItem1718Quantity(mode->drag.id);
      if (owned == 0) {
        goto invalid;
      }
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      if (this->geometry.state == 2) {
        window = g_windowMgr.itemSellWnd;
      } else if (this->geometry.state == 16) {
        window = g_windowMgr.itemSellWnd2;
      } else {
        break;
      }
      goto notify;
    }
    case 3: {
      const int owned = g_session.GetItem1720QuantityByItemId(mode->drag.name.c_str());
      if (owned == 0) {
        goto invalid;
      }
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      window = g_windowMgr.itemShopWnd;
      goto notify;
    }
    case 4:
    case 15: {
      const int owned = g_session.GetItem1730Quantity(mode->drag.id);
      if (owned == 0) {
        goto invalid;
      }
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      if (this->geometry.state == 4) {
        window = g_windowMgr.itemShopWnd;
      } else if (this->geometry.state == 15) {
        window = g_windowMgr.itemShopWnd2;
      } else {
        break;
      }
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 1, 0, 0, 0);
      }
      break;
    }
    case 5: {
      if (!g_session.HasInventoryItem(mode->drag.id)) {
        goto invalid;
      }
      const int owned = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id).quantity;
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (owned != 0) {
        mode->drag.quantity = quantity <= owned ? quantity : owned;
      }
      window = g_windowMgr.LookupWindow(WID_EXCHANGEUI);
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    }
    case 6:
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (mode->drag.source == 6) {
        if (g_session.GetItem1738Quantity(mode->drag.id) != 0) {
          const int owned = g_session.FindCartItem(mode->drag.id).quantity;
          if (owned != 0) {
            mode->drag.quantity = quantity <= owned ? quantity : owned;
          }
        }
      } else if (mode->drag.source == 9) {
        if (g_windowMgr.merchantShopMakeWnd) {
          break;
        }
        if (g_session.GetItem1740Quantity(mode->drag.id) != 0) {
          const int owned = g_session.FindMerchantShopItem(mode->drag.id).quantity;
          if (owned != 0) {
            mode->drag.quantity = quantity <= owned ? quantity : owned;
          }
        }
      } else if (mode->drag.source == 15) {
        const int owned = mode->drag.quantity;
        if (owned != 0) {
          mode->drag.quantity = quantity <= owned ? quantity : owned;
        }
      }
      window = g_windowMgr.itemWnd;
      goto notify;
    case 7: {
      int owned = 0;
      if (mode->drag.source == 9) {
        const ITEM_INFO item = g_session.FindMerchantShopItem(mode->drag.id);
        if (item.identity[1] != 0) {
          owned = item.quantity;
        }
      } else {
        if (mode->drag.source != 1 || !g_session.HasInventoryItem(mode->drag.id)) {
          break;
        }
        owned = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id).quantity;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.itemStoreWnd;
      goto notify;
    }
    case 8: {
      int owned = 0;
      if (mode->drag.source == 1) {
        const ITEM_INFO item = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id);
        if (item.identity[1] != 0) {
          owned = item.quantity;
        }
      } else if (mode->drag.source == 6) {
        const ITEM_INFO item = g_session.FindCartItem(mode->drag.id);
        if (item.identity[1] != 0) {
          owned = item.quantity;
        }
      } else {
        break;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.merchantItemWnd;
      goto notify;
    }
    case 9: {
      if (mode->drag.source != 11) {
        break;
      }
      int owned = 0;
      const ITEM_INFO item = g_session.FindMerchantMyShopItem(mode->drag.id);
      if (item.identity[1] != 0) {
        owned = item.quantity;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.merchantShopMakeWnd;
      goto notify;
    }
    case 10: {
      if (mode->drag.source != 10) {
        break;
      }
      int owned = 0;
      const ITEM_INFO item = g_session.FindMerchantPurchaseSavedItem(mode->drag.id);
      if (item.identity[1] != 0) {
        owned = item.quantity;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.merchantMirrorItemWnd;
      goto notify;
    }
    case 11: {
      if (mode->drag.source != 13) {
        break;
      }
      const int owned = g_session.GetItem1760QuantityByItemId(mode->drag.name.c_str());
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.merchantItemShopWnd;
      goto notify;
    }
    case 12:
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      mode->drag.quantity = quantity;
      window = g_windowMgr.merchantItemPurchaseWnd;
      goto notify;
    case 13: {
      if (!g_session.HasInventoryItem(mode->drag.id)) {
        goto invalid;
      }
      const int owned = LookupClientItemByIdentity(g_session.m_inventoryItems, mode->drag.id).quantity;
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (owned != 0) {
        mode->drag.quantity = quantity <= owned ? quantity : owned;
      }
      window = g_windowMgr.rodexComposeWnd;
      goto notify;
    }
    case 17: {
      if (mode->drag.source != 22) {
        break;
      }
      int owned = 0;
      const ITEM_INFO item = g_session.FindMerchantMyShopItem(mode->drag.id);
      if (item.identity[1] != 0) {
        owned = item.quantity;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.buyingShopPurchaseWnd;
      goto notify;
    }
    case 18: {
      if (mode->drag.source != 21) {
        break;
      }
      int owned = 0;
      const ITEM_INFO item = g_session.FindMerchantHopeToSellItem(mode->drag.id);
      if (item.identity[1] != 0) {
        owned = item.quantity;
      }
      if (owned == 0) {
        break;
      }
      if (quantity <= 0) {
        quantity = 1;
      }
      mode->drag.quantity = quantity <= owned ? quantity : owned;
      window = g_windowMgr.purchaseMirrorItemWnd;
      goto notify;
    }
    case 19:
      if (quantity <= 0) {
        goto invalid;
      }
      if (quantity > 100) {
        mode->drag.quantity = 100;
      } else {
        mode->drag.quantity = quantity;
        if (quantity < 100) {
          goto stall;
        }
      }
      g_windowMgr.Confirm(FormatOwnedString(LookupMessage(MSI_LIMIT_BUY_ITEM2), 100).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
      break;
    case 20: {
      const int current = mode->drag.quantity;
      if (current <= 0) {
        if (quantity <= 0) {
          goto invalid;
        }
        if (current == 0) {
          goto stall;
        }
      }
      mode->drag.quantity = quantity <= current ? quantity : current;
      goto stall;
    }
    case 21: {
      if (quantity <= 0) {
        goto invalid;
      }
      if (quantity > 30000) {
        mode->drag.quantity = 30000;
      } else {
        mode->drag.quantity = quantity;
      }
      if (quantity >= 30000) {
        char text[200] = {};
        sprintf_s(text, sizeof(text), LookupMessage(MSI_LIMIT_BUY_ITEM2), 30000);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      }
      UIParaItemPurchaseWnd* purchase = dynamic_cast<UIParaItemPurchaseWnd*>(g_windowMgr.LookupWindow(WID_PARAITEMPURCHASEWND));
      if (purchase) {
        purchase->SendMsg(nullptr, UM_DROP, quantity, 0, 0, 0);
      }
      break;
    }
    case 22: {
      const int owned = g_session.GetItem1720QuantityByItemId(mode->drag.name.c_str());
      if (owned == 0) {
        goto invalid;
      }
      if (quantity <= 0) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      UIParaItemShopWnd* shop = dynamic_cast<UIParaItemShopWnd*>(g_windowMgr.LookupWindow(WID_PARAITEMSHOPWND));
      if (shop) {
        shop->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    }
    case 23: {
      if (quantity <= 0) {
        goto invalid;
      }
      const int owned = g_barterMarketItemMgr->GetAllItemValueById(mode->drag.barterId);
      if (owned <= 0) {
        goto invalid;
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      window = g_windowMgr.LookupWindow(WID_BARTERMARKETWND);
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    }
    case 24: {
      if (quantity <= 0) {
        goto invalid;
      }
      if (quantity > 30000) {
        goto tooMany;
      }
      int limit = g_barterMarketItemMgr->GetValueById(mode->drag.barterId);
      if (limit == -1) {
        limit = 30000;
      }
      if (limit != 0) {
        mode->drag.quantity = quantity <= limit ? quantity : limit;
      }
      windowId = 335;
      goto lookup;
    }
    case 25: {
      if (quantity <= 0) {
        goto invalid;
      }
      const int owned = g_expandedBarterMarketItemMgr->GetValueById(mode->drag.barterId);
      if (owned <= 0) {
        goto invalid;
      }
      if (quantity > owned) {
        quantity = owned;
      }
      mode->drag.quantity = quantity;
      window = g_windowMgr.LookupWindow(WID_EXPANDEDBARTERMARKETWND);
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    }
    case 26: {
      if (quantity <= 0) {
        goto invalid;
      }
      if (quantity > 30000) {
        goto tooMany;
      }
      int limit = g_expandedBarterMarketItemMgr->GetItemValueById(mode->drag.barterId);
      if (limit == -1) {
        limit = 30000;
      }
      if (limit != 0) {
        mode->drag.quantity = quantity <= limit ? quantity : limit;
      }
      windowId = 342;
      goto lookup;
    }
    lookup:
      window = g_windowMgr.LookupWindow(windowId);
      if (window) {
        window->SendMsg(nullptr, UM_DROP, quantity, 0, 0, 0);
      } else {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      }
      break;
    stall:
      window = g_windowMgr.LookupWindow(WID_CASHSHOP_WND);
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    tooMany:
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_LIMIT_BUY_ITEM)), 0xFF, 0, 0, 0);
    invalid:
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      break;
    notify:
      if (window) {
        window->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
      break;
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 0);
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0097C230
// FUNCTION: fn_13324
void UIMerchantShopMakeWnd::OnCreate(int width, int height) {
  int count;
  if (this->mode != 0) {
    count = g_session.m_buyingStoreSlotCount;
    this->title = LookupMessage(MSI_BUYINGSTORE_MAKEWND_TITLE);
  } else {
    count = g_session.m_vendingSlotCount;
    this->title = LookupMessage(MSI_MERCHANTSHOPMAKER);
  }

  this->titleEdit = new UIEditCtrl();
  this->titleEdit->SetSize(this->geometry.width - 66, 16);
  this->titleEdit->Move(41, 22);
  this->titleEdit->maximum = 36;
  this->titleEdit->Background(232, 232, 232);
  this->titleEdit->SetText("");
  this->AddChild(this->titleEdit);
  g_windowMgr.SetFocus(this->titleEdit);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 17, 1, this->geometry.height - 58);
  this->AddChild(this->scrollBar);

  this->slotCount = count;
  if (this->mode == 0) {
    if (count > 13) {
      this->slotCount = 13;
    }
  } else if (count > 5) {
    this->slotCount = 5;
  }
  this->columns = 1;
  this->minimumRows = 4;

  int y = 54;
  for (int index = 0; index < this->slotCount; ++index, y += 42) {
    if (this->mode != 0) {
      this->quantityEdits[index] = new UIEditCtrlWheel();
      this->quantityEdits[index]->SetSize(42, 16);
      this->quantityEdits[index]->Move(this->geometry.width - 231, y);
      this->quantityEdits[index]->maximum = 5;
      this->quantityEdits[index]->SetText("0");
      this->quantityEdits[index]->Background(232, 232, 232);
      this->AddChild(this->quantityEdits[index]);
    }

    this->priceEdits[index] = new UIEditCtrlWheel();
    this->priceEdits[index]->SetSize(94, 16);
    this->priceEdits[index]->Move(this->geometry.width - 119, y);
    this->priceEdits[index]->maximum = 14;
    this->priceEdits[index]->SetText("0");
    if (this->mode != 0) {
      this->priceEdits[index]->Background(232, 232, 232);
    } else {
      this->priceEdits[index]->disabled = 1;
      this->priceEdits[index]->Invalidate();
      this->priceEdits[index]->Background(182, 182, 182);
    }
    this->AddChild(this->priceEdits[index]);
  }

  if (this->mode != 0) {
    const int labelWidth = this->TextWidth(LookupMessage(MSI_BUYINGSTORE_MAKEWND_LIMITZENY), 0, 0, 12, 0, 0);
    this->zenyLimitEdit = new UIEditCtrlWheel();
    this->zenyLimitEdit->Move(labelWidth + 10, this->geometry.height - 17);
    this->zenyLimitEdit->SetSize(94, 16);
    this->zenyLimitEdit->maximum = 15;
    this->zenyLimitEdit->Background(232, 232, 232);
    this->zenyLimitEdit->SetText("0");
    this->AddChild(this->zenyLimitEdit);
  }

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int positions[2][2] = {{this->geometry.width - 92, this->geometry.height - 24}, {this->geometry.width - 46, this->geometry.height - 24}};
  int ids[2] = {184, 185};
  for (int index = 0; index < 2; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
    this->actionButtons[index] = button;
  }

  this->m_message0 = 184;
  if (Replay::Instance().state != 6) {
    g_session.ClearMerchantPurchaseSavedItems();
  }

  if (this->mode == 0) {
    UICheckBox* notice = new UICheckBox();
    this->notice = notice;
    notice->SetSize(140, 12);
    notice->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_QUEST_INFO_01));
    notice->FitToText();
    notice->Move(12, this->geometry.height - 20);
    notice->SetChecked(g_session.m_skipPriceWarning);
    this->AddChild(notice);
  }

  this->initialHeight = this->geometry.height;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);

  UIBitmapButton* info = new UIBitmapButton();
  info->SetBitmap("유저인터페이스\\basic_interface\\bt_shopinfo.bmp", 0, 0);
  info->SetBitmap("유저인터페이스\\basic_interface\\bt_shopinfo.bmp", 1, 0);
  info->SetBitmap("유저인터페이스\\basic_interface\\bt_shopinfo.bmp", 2, 0);
  info->SetSize(info->BitmapWidth(), info->BitmapHeight());
  info->Move(this->geometry.width - 23, 20);
  info->SetId(343);
  this->AddChild(info);
}

// OFFSET: 0x00989FE0
// FUNCTION: fn_13405
void UIMerchantShopMakeWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.merchantMirrorItemWnd) {
        item = g_session.FindMerchantPurchaseSavedItem(item.identity[1]);
        gameMode->drag.BeginItem(g_windowMgr, 10, item, item.quantity, -1);
        g_windowMgr.merchantMirrorItemWnd->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
        g_windowMgr.SetFocus(nullptr);
      } else if (g_windowMgr.buyingShopMirrorItemWnd) {
        item = g_session.FindMerchantPurchaseSavedItem(item.identity[1]);
        gameMode->drag.BeginItem(g_windowMgr, 19, item, item.quantity, -1);
        g_windowMgr.buyingShopMirrorItemWnd->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      }
    } else {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x0097E790
// FUNCTION: fn_13329
void UIItemDropCntWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x0097E3F0
// FUNCTION: fn_13328
void UIItemBaseWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  const int leftTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int tile = 0, y = 17; tile < leftTiles; ++tile, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  const int rightTiles = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int tile = 0, y = 17; tile < rightTiles; ++tile, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int rows = (this->geometry.height - 38) / 32;
  for (int row = 0, y = 17; row < rows; ++row, y += 32) {
    for (int column = 0, x = 40; column < columns; ++column, x += 32) {
      DrawItemSlotBackground(*this, x, y);
    }
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int x = visible % this->columns * 32 + 44;

    if (this->showQuantity != 0) {
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    } else {
      DrawItemIcon(*this, ResourceMgr::Instance(), x, top + 21, *it, 0);
    }

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x00981770
// FUNCTION: fn_13339
void UIMerchantItemMyShopWnd::Paint() {
  UIItemBaseWnd::Paint();

  if (this->footerHeight == 57) {
    this->DrawButtonBar(3, 0, 1);
  }

  if (this->duration > 0) {
    const int remaining = this->duration - (timeGetTime() - this->startTick);

    if (remaining > 0) {
      const int days = remaining / 86400000;
      const int hours = remaining / 3600000 % 24;
      const int minutes = remaining / 60000 % 60;
      const int seconds = remaining / 1000 % 60;

      if (this->buyingShop == 1) {
        if (remaining <= 60000) {
          this->DrawTextAt(12, this->geometry.height - 42, FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0,
            0, 11, 0xFF, 1, 0
          );
        } else {
          this->DrawTextAt(12, this->geometry.height - 42, FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0,
            0, 11, 0, 1, 0
          );
        }
      } else if (remaining <= 60000) {
        this->DrawTextAt(20, this->geometry.height - 50, FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0, 0,
          12, 0xFF, 1, 0
        );
      } else {
        this->DrawTextAt(20, this->geometry.height - 50, FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0, 0,
          12, 0, 1, 0
        );
      }
    }
  }
}

// OFFSET: 0x0097A670
// FUNCTION: fn_13319
void UIMerchantItemPurchaseMSGWnd::OnCreate(int, int) {
  this->title = new UIStaticText();
  this->title->SetSize(100, 15);
  this->title->SetTextFitted(LookupMessage(MSI_SHOP_ITEM_BAY_MAG3));
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->buyButton = new UIBitmapButton();
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy.bmp", 0, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_a.bmp", 1, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_b.bmp", 2, 0);
  this->buyButton->SetSize(this->buyButton->BitmapWidth(), this->buyButton->BitmapHeight());
  this->buyButton->Move(this->geometry.width - 106, this->geometry.height - 24);
  this->buyButton->SetId(184);
  this->AddChild(this->buyButton);

  this->cancelButton = new UIBitmapButton();
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->cancelButton->SetSize(this->cancelButton->BitmapWidth(), this->cancelButton->BitmapHeight());
  this->cancelButton->Move(this->geometry.width - 61, this->geometry.height - 24);
  this->cancelButton->SetId(185);
  this->AddChild(this->cancelButton);

  this->firstLine = new UIStaticText();
  this->firstLine->SetSize(200, 15);
  this->firstLine->SetText(LookupMessage(MSI_BASIC_EXP_MSG2), 0);
  this->firstLine->Move(10, 20);
  this->AddChild(this->firstLine);

  this->secondLine = new UIStaticText();
  this->secondLine->SetSize(200, 15);
  this->secondLine->Move(10, 40);
  this->AddChild(this->secondLine);

  this->thirdLine = new UIStaticText();
  this->thirdLine->SetSize(200, 15);
  this->thirdLine->SetText(LookupMessage(MSI_PERSONAL_INFORMATION_MSG_CHN), 0);
  this->thirdLine->Move(10, 60);
  this->AddChild(this->thirdLine);
}

// OFFSET: 0x00981990
// FUNCTION: fn_13340
void UIMerchantItemPurchaseMSGWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(false, "", 0);
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x009827C0
// FUNCTION: fn_13343
void UIMerchantItemWnd::Paint() {
  UIItemBaseWnd::Paint();

  this->DrawBitmap(10, this->geometry.height - 22, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\icon_num.bmp"))), true);

  const std::uint32_t countColor = g_session.cartCount >= g_session.cartMaxCount ? 0xFF : 0x232323;
  this->DrawTextAt(23, this->geometry.height - 19, FormatOwnedString("%d", g_session.cartCount).c_str(), 0, 0, 11, countColor, 0, 0
  );
  const int countWidth = this->TextWidth(FormatOwnedString("%d", g_session.cartCount).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(countWidth + 24, this->geometry.height - 19, FormatOwnedString("/%d", g_session.cartMaxCount).c_str(), 0, 0, 11, 0, 0, 0
  );

  this->DrawBitmap(75, this->geometry.height - 20, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\icon_weight.bmp"))), true);

  const std::uint32_t weightColor = g_session.cartWeight >= g_session.cartMaxWeight ? 0xFF : 0x232323;
  this->DrawTextAt(90, this->geometry.height - 19, FormatOwnedString("%d", g_session.cartWeight).c_str(), 0, 0, 11, weightColor, 0, 0
  );
  const int weightWidth = this->TextWidth(FormatOwnedString("%d", g_session.cartWeight).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(weightWidth + 91, this->geometry.height - 19, FormatOwnedString("/%d", g_session.cartMaxWeight).c_str(), 0, 0, 11, 0, 0, 0
  );
}

// OFFSET: 0x009834F0
// FUNCTION: fn_13345
void UINoticeMessageBox_LockSellItem::Paint() {
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\popup_itemDeal_lock.bmp"))), false);
  this->DrawCenteredText(0, 5, LookupMessage(MSI_CONNECT_SNS_WHOLE_SCREEN_SHOT), this->geometry.width, 0, 12, 0xFFFFFF, 0);
}

// OFFSET: 0x00983550
// FUNCTION: fn_13346
void UISkillListWnd::Paint() {
  CFile file;
  char text[80];

  auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(
    g_skillTooltipActive ? "유저인터페이스\\basic_interface\\titlebar_left2.bmp" : "유저인터페이스\\basic_interface\\titlebar_left.bmp"
  )));
  this->DrawBitmap(0, 0, left, false);
  auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(
    g_skillTooltipActive ? "유저인터페이스\\basic_interface\\titlebar_right2.bmp" : "유저인터페이스\\basic_interface\\titlebar_right.bmp"
  )));
  this->DrawBitmap(this->geometry.width - right->image.width, 0, right, false);
  int count = (this->geometry.width - 24) / 12 + ((this->geometry.width - 24) % 12 != 0);
  for (int x = 12; count > 0; --count, x += 12) {
    auto* middle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(
      g_skillTooltipActive ? "유저인터페이스\\basic_interface\\titlebar_mid2.bmp" : "유저인터페이스\\basic_interface\\titlebar_mid.bmp"
    )));
    this->DrawBitmap(x, 0, middle, false);
  }

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    auto* itemLeft = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, itemLeft, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    auto* itemRight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - itemRight->image.width, y, itemRight, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);

  const int slotColumns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - 38) / 32;
  int slots = slotRows;
  for (int y = 17; slots > 0; --slots, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillBlue;
  g_skinInfo.PaletteChannels(2, 2, red, green, fillBlue);
  this->FillRegion(72, 17, slotColumns * 32 - 32, slotRows * 32, RGB(red, green, fillBlue));

  if (this->skillTab == 0) {
    this->DrawTextAt(17, 4, LookupMessage(MSI_HOMUN_SKILLLIST), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, LookupMessage(MSI_HOMUN_SKILLLIST), 0, 0, 12, 0, 0, 0);
    std::sprintf(text, LookupMessage(MSI_SKILLPOINT), g_session.m_homunSkillPoint);
  } else if (this->skillTab == 1) {
    this->DrawTextAt(17, 4, LookupMessage(MSI_MER_SKILLLIST), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, LookupMessage(MSI_MER_SKILLLIST), 0, 0, 12, 0, 0, 0);
  }

  if (this->skillTab != 1) {
    this->DrawTextAt(16, this->geometry.height - 20, text, 0, 0, 12, 0, 0, 0);
  }

  int drawn = 0;
  int index = 0;
  int skillLevel;
  int spLevel;
  for (auto it = this->skills.begin(); it != this->skills.end(); ++index) {
    if (index < this->scroll * this->columns) {
      ++it;
      continue;
    }

    const int top = drawn / this->columns * 32;
    int x = drawn % this->columns * 32;
    if (this->hovered == index) {
      unsigned selectRed, selectGreen, selectBlue;
      g_skinInfo.PaletteChannels(6, 2, selectRed, selectGreen, selectBlue);
      if (it->level == 0) {
        g_skinInfo.PaletteChannels(10, 2, selectRed, selectGreen, selectBlue);
      } else if (it->spCost == 0) {
        g_skinInfo.PaletteChannels(14, 2, selectRed, selectGreen, selectBlue);
      }
      this->FillRegion(x + 104, top + 19, 164, 28, RGB(selectRed, selectGreen, selectBlue));
    }

    g_session.ItemBitmapPath(it->name, text);
    auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
    this->DrawBitmap(x + 44, top + 21, icon, true);

    int shade;
    if (it->level == 0) {
      shade = 120;
    } else {
      const char* format;
      int levelX;
      if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, static_cast<int>(it->id))) {
        std::sprintf(text, g_serviceType != 12 ? "Lv :" : "Nv :");
        this->DrawTextAt(x + 124, top + 34, text, 0, 0, 12, 0, 0, 0);
        if (this->skillTab == 0) {
          skillLevel = g_session.HomunSkillLevel(static_cast<int>(it->id));
        } else if (this->skillTab == 1) {
          skillLevel = g_session.MercSkillLevel(static_cast<int>(it->id));
        }
        if (skillLevel <= 0 || skillLevel > it->level) {
          skillLevel = it->level;
        }
        std::sprintf(text, "%2d", skillLevel);
        this->DrawTextAt(x + 149, top + 34, text, 0, 0, 12, skillLevel != it->level ? 0xFF : 0, 0, 0);
        format = "/ %2d";
        levelX = 168;
      } else {
        format = g_serviceType != 12 ? "Lv : %d" : "Nv : %d";
        levelX = 113;
      }
      std::sprintf(text, format, it->level);
      this->DrawTextAt(levelX + x, top + 34, text, 0, 0, 12, 0, 0, 0);

      if (it->type == 0) {
        std::sprintf(text, LookupMessage(MSI_PASSIVE));
      } else {
        int sp = static_cast<int>(it->spCost);
        if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, static_cast<int>(it->id))) {
          if (this->skillTab == 0) {
            spLevel = g_session.HomunSkillLevel(static_cast<int>(it->id));
          } else if (this->skillTab == 1) {
            spLevel = g_session.MercSkillLevel(static_cast<int>(it->id));
          }
          const int amount = it->UseSpAmount(*g_session.m_hotkeyScript, spLevel);
          if (amount != 0 && spLevel <= it->level) {
            sp = amount;
          }
        }
        const int ap = it->UseApAmount(*g_session.m_hotkeyScript, it->level);
        if (ap != 0) {
          std::sprintf(text, "Sp : %d , Ap : %d", sp, ap);
        } else {
          std::sprintf(text, "Sp : %d", sp);
        }
      }
      this->DrawTextAt(x + 212, top + 34, text, 0, 0, 12, 0, 0, 0);
      shade = 0;
    }

    x += 112;
    std::string name = this->FitDisplayText(this->geometry.width - x - 20, it->DisplayName(), 0, 12, 0);
    this->DrawTextAt(x, top + 22, name.c_str(), 0, 0, 12, RGB(shade, shade, shade), 0, 0);
    if (++drawn == this->visibleRows * this->columns) {
      break;
    }
    ++it;
  }
}

// OFFSET: 0x00983F80
// FUNCTION: fn_13347
void UIItemSellWnd2::OnLBtnDblClk(int x, int y) {
  if (g_windowMgr.dragAllItems == 0) {
    return;
  }

  UIWindow* target = g_windowMgr.itemShopWnd2;

  if (target) {
    this->OnLBtnDown(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->RecordDrop(x, y, mode->drag, 160);
    target->DragAndDrop(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x00983FF0
// FUNCTION: fn_13348
void UIItemSellWnd::OnLBtnDblClk(int x, int y) {
  if (g_windowMgr.dragAllItems == 0) {
    return;
  }

  UIWindow* target = g_windowMgr.itemShopWnd;

  if (target) {
    this->OnLBtnDown(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->RecordDrop(x, y, mode->drag, 22);
    target->DragAndDrop(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x00984130
// FUNCTION: fn_13351
void UIItemStoreFindWnd::OnLBtnDblClk(int, int) {
  this->SendMsg(nullptr, UM_COMMAND, ID_TALK, 0, 0, 0);
}

// OFFSET: 0x00984150
// FUNCTION: fn_13352
void UIItemStoreSubWnd::OnLBtnDblClk(int, int) {
  this->SendMsg(nullptr, UM_COMMAND, ID_TALK, 0, 0, 0);
}

// OFFSET: 0x00984170
// FUNCTION: fn_13353
void UIItemWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    this->SendMsg(this, UM_COMMAND, ID_MINI, 0, 0, 0);
    return;
  }

  if (GetAsyncKeyState(VK_SHIFT)) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] <= 0) {
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  switch (item.identity[0]) {
  case 0:
  case 1:
  case 2:
    if (item.GetItemId() == 12802 &&
        g_windowMgr.Confirm(LookupMessage(MSI_COMMAND_SETTING), 2, false, false, 0, "Message", 280, 120, 0) == ID_NO) {
      break;
    }
    mode->SendMsg(MM_USEITEM, static_cast<int>(item.identity[1]), 0, 0, 0);
    break;
  case 18:
    if (g_session.m_manner >= 0) {
      if (g_windowMgr
              .Confirm(FormatOwnedString(LookupMessage(MSI_CONFIRM_TO_OPEN_CASH_ITEM), item.DisplayName(0).c_str()).c_str(), 2, false, false, 0, "Message", 280, 120, 0) == ID_YES) {
        mode->SendMsg(MM_USEITEM, static_cast<int>(item.identity[1]), 0, 0, 0);
      }
    }
    break;
  case TYPE_ARMOR:
  case TYPE_WEAPON:
  case TYPE_BOW:
  case TYPE_BOTHHAND:
  case TYPE_ARMORTM:
  case TYPE_ARMORTB:
  case TYPE_ARMORMB:
  case TYPE_ARMORTMB:
  case TYPE_GUN:
    if (!item.identified) {
      break;
    }
    if (item.identity[1] <= 0 || item.identity[2] == 0 || item.identity[3] != 0) {
      if (!g_session.IsPetAccessory(item.GetItemId())) {
        break;
      }
    }
    if (GetAsyncKeyState(VK_CONTROL) && g_session.m_swapEquipmentMode) {
      mode->SendMsg(MM_WEAR_SWAP_EQUIP, static_cast<int>(item.identity[1]), static_cast<int>(item.identity[2]), 0, 0);
    } else if (g_windowMgr.LookupWindow(WID_REFININGWND)) {
      g_windowMgr.LookupWindow(WID_REFININGWND)->SendMsg(this, UM_REFRESHITEM, item.identity[1], 0, 0, 0);
    } else if (g_windowMgr.LookupWindow(WID_GRADEENCHANTWND)) {
      g_windowMgr.LookupWindow(WID_GRADEENCHANTWND)->SendMsg(this, UM_REFRESHITEM, item.identity[1], 0, 0, 0);
    } else {
      mode->SendMsg(MM_WEAR_EQUIP, static_cast<int>(item.identity[1]), static_cast<int>(item.identity[2]), 0, 0);
    }
    break;
  case 10:
  case 16:
  case 17:
  case 19:
    if (item.identity[1] <= 0 || item.identity[3] != 0) {
      break;
    }
    if (GetAsyncKeyState(VK_CONTROL) && g_session.m_swapEquipmentMode) {
      mode->SendMsg(MM_WEAR_SWAP_EQUIP, static_cast<int>(item.identity[1]), static_cast<int>(item.identity[2]), 0, 0);
    } else {
      mode->SendMsg(MM_WEAR_EQUIP_ARROW, static_cast<int>(item.identity[1]), static_cast<int>(item.identity[2]), 0, 0);
    }
    break;
  case 6:
    mode->SendMsg(MM_REQ_ITEMCOMPOSITION_LIST, static_cast<int>(item.identity[1]), 0, 0, 0);
    break;
  }
}

// OFFSET: 0x00985380
// FUNCTION: fn_13364
void UIMerchantItemMyShopWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIItemBaseWnd::OnLBtnDown(x, y);
  }
}

// OFFSET: 0x009886C0
// FUNCTION: fn_13386
void UIItemWnd::OnProcess() {
  if (this->filterWindow) {
    this->filterWindow->OnProcess();
  }
}

// OFFSET: 0x00995780
// FUNCTION: fn_45470
void UIItemWnd::RefreshOptionBox() {
  UIWindow* optionBox = this->optionBox;
  optionBox->geometry.state = g_serviceOptions.Get(TT_ITEM_COMPARE_ON_OFF);
  optionBox->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x009886D0
// FUNCTION: fn_13387
void UIMerchantItemMyShopWnd::OnProcess() {
  this->Invalidate();
}

// OFFSET: 0x009886E0
// FUNCTION: fn_13388
void UIMerchantItemShopWnd::OnProcess() {
  this->Invalidate();
}

// OFFSET: 0x009886F0
// FUNCTION: fn_13389
void UIMerchantShopMakeWnd::OnProcess() {
  if (this->zenyLimitEdit && this->mode) {
    std::int64_t total = 0;
    for (unsigned index = 0; index < this->items.size(); ++index) {
      char text[24] = {};
      StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), "%s", this->priceEdits[index]->Text());
      if (g_session.IsNumberWithCommas(text) == true) {
        const std::int64_t price = _atoi64(text);
        const std::int64_t quantity = _atoi64(this->quantityEdits[index]->Text());
        const double rate = PriceCommission(price) * -0.01;
        total += (price - static_cast<std::int64_t>(price * rate)) * quantity;
      }
    }

    char buffer[128] = {};
    StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), "%lld", total);
    this->zenyLimitEdit->SetText(buffer);
    this->InvalidateChildren();
    return;
  }

  if (!g_windowMgr.modalWindows.empty() && g_windowMgr.modalWindows.back()) {
    return;
  }

  int index = -1;
  for (int i = 0; i < this->slotCount; ++i) {
    if (this->priceEdits[i] == g_windowMgr.GetHoverWindow()) {
      index = i;
      break;
    }
  }

  if (this->editIndex == index) {
    if (index == -1) {
      return;
    }

    const int length = this->priceEdits[index]->Length();
    if (length == this->previousPriceLength) {
      return;
    }
    this->previousPriceLength = length;
    if (!length) {
      return;
    }

    UIEditCtrl* edit = this->priceEdits[index];
    char text[24];
    std::sprintf(text, "%s ", edit->Text());
    if (!g_session.IsNumberWithCommas(text)) {
      g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
      edit->SetText(this->previousPriceText.c_str());
      g_windowMgr.SetFocus(edit);
      return;
    }

    std::int64_t price = _atoi64(text);
    if (price < 0) {
      price = 0;
    } else if (price > 1000000000) {
      const int result = g_windowMgr.Confirm(LookupMessage(MSI_BASIC_EXP_MSG_INDONESIA), 2, true, false, 0, "Message", 280, 120, 0);
      price = 1000000000;
      if (result == ID_YES) {
        edit->SetText(_itoa(1000000000, text, 10));
      } else {
        edit->SetText(_itoa(1000000000, text, 10));
        g_windowMgr.SetFocus(edit);
      }
    }

    const PriceColors colors = PriceColor(price);
    edit->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
    edit->SetText(FormatGroupedInteger(price, text, sizeof(text)));
    this->previousPriceText = this->priceEdits[index]->Text();
    this->previousPriceLength = this->priceEdits[index]->Length();
    edit->SetCursor(edit->Length());
  } else {
    if (index != -1) {
      this->previousPriceText = this->priceEdits[index]->Text();
    }
    this->CommitPrice(index);
    this->InvalidateChildren();
  }
}

// OFFSET: 0x00988C20
// FUNCTION: fn_13390
void UINoticeMessageBox_LockSellItem::OnProcess() {
  if (this->startTick + this->duration < timeGetTime()) {
    this->SendMsg(this, UM_COMMAND, ID_CLOSE, 0, 0, 0);
  }
}

// OFFSET: 0x00988D50
// FUNCTION: fn_13392
void UIItemSellWnd2::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988E10
// FUNCTION: fn_13393
void UIItemSellWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988ED0
// FUNCTION: fn_13394
void UIItemShopWnd2::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x009898A0
// FUNCTION: fn_13400
void UIMerchantItemMyShopWnd::OnRBtnDown(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0098A220
// FUNCTION: fn_13407
void UISkillListWnd::OnRBtnUp(int, int) {
  this->SendMsg(nullptr, UM_MAKEMENU, 0, 0, 0, 0);
}

// OFFSET: 0x009739A0
// FUNCTION: fn_13291
int UISkillListWnd::SkillAt(int x, int y) {
  if (x < 40 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return -1;
  }

  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->skills.begin(); it != this->skills.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return index;
      }

      ++visible;
    }
  }

  return -1;
}

// OFFSET: 0x0098A1E0
// FUNCTION: fn_13406
void UISkillListWnd::OnRBtnDown(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->SkillAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x00974560
// FUNCTION: fn_13302
bool UISkillListWnd::NeedsFrameRefresh() {
  return true;
}

// OFFSET: 0x00974900
// FUNCTION: fn_13307
void UIItemBaseWnd::OnCreate(int, int) {
}

// OFFSET: 0x00973A20
// FUNCTION: fn_13292
ITEM_INFO UIItemBaseWnd::GetItemByIndex(int index) {
  int position = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++position) {
    if (position == index) {
      return *it;
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973AB0
// FUNCTION: fn_13293
ITEM_INFO UIItemBaseWnd::GetItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int column = (x - 40) / 32;
  const int row = (y - 17) / 32;

  if (column > this->columns - 1) {
    return ITEM_INFO(0);
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->columns * this->scroll) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return *it;
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973C20
// FUNCTION: fn_13294
ITEM_INFO UIItemStoreFindWnd::GetItemAt(int x, int y) {
  if (x < 20 || x >= 52 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int column = (x - 20) / 32;
  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->columns * this->scroll) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return *it;
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973D60
// FUNCTION: fn_13295
ITEM_INFO UIItemStoreSubWnd::GetItemAt(int x, int y) {
  if (x < 20 || x >= 52 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int column = (x - 20) / 32;
  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->columns * this->scroll) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return *it;
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973EA0
// FUNCTION: fn_13296
ITEM_INFO UIItemStoreWnd::GetItemAt(int x, int y) {
  if (x < 40 || x >= 72 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int column = (x - 40) / 32;
  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->columns * this->scroll) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return *it;
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973FE0
// FUNCTION: fn_13297
ITEM_INFO UIItemWnd::GetItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int column = (x - 40) / 32;
  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return *it;
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00974130
// FUNCTION: fn_13298
ITEM_INFO UIMerchantShopMakeWnd::GetItemAt(int x, int y) {
  if (x < 1 || x >= this->geometry.width - 20 || y < 27 || y >= this->visibleRows * 42 + 44) {
    return ITEM_INFO(0);
  }

  const int row = (y - 44) / 42;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row) {
          const ITEM_INFO& item = *it;
          if (x < 35) {
            return item;
          }

          return ITEM_INFO(0);
        }

        ++visible;
      }
    }
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00974290
// FUNCTION: fn_13299
CSkillInfo UISkillListWnd::GetSkillByIndex(int index) {
  int position = 0;
  CSkillInfo result(0);

  for (auto it = this->skills.begin(); it != this->skills.end(); ++it, ++position) {
    if (index == position) {
      result = *it;
      break;
    }
  }

  return result;
}

// OFFSET: 0x00974320
// FUNCTION: fn_13300
CSkillInfo UISkillListWnd::GetSkillAt(int x, int y) {
  if (x < 40 || x >= 72 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return CSkillInfo(0);
  }

  const int row = (y - 17) / 32;
  int visible = 0;
  int index = 0;

  for (auto it = this->skills.begin(); it != this->skills.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (visible == this->columns * row) {
        return *it;
      }

      ++visible;
    }
  }

  return CSkillInfo(0);
}

// OFFSET: 0x00974410
// FUNCTION: fn_13301
CSkillInfo UISkillListWnd::GetVisibleSkill(int row) {
  int index = 0;

  for (auto it = this->skills.begin(); it != this->skills.end(); ++it, ++index) {
    if (row + this->scroll == index) {
      return *it;
    }
  }

  return CSkillInfo(0);
}

// OFFSET: 0x00974570
// FUNCTION: fn_13303
bool UIMerchantShopMakeWnd::HasSelection() const {
  if (0 > this->selectedRow) {
    return false;
  }

  if (0 > this->selectedColumn) {
    return false;
  }

  return true;
}

#pragma inline_depth(0)

// OFFSET: 0x00974590
// FUNCTION: fn_13304
void UIMerchantShopMakeWnd::SendShopRequest() {
  if (this->mode == 1) {
    g_modeMgr.GetGameMode()->SendMsg(MM_OPEN_BUYING_STORE_EXTENDED, reinterpret_cast<int>(this->titleEdit->Text()), std::atoi(this->zenyLimitEdit->Text()), 0, 0);
    return;
  }

  g_modeMgr.GetGameMode()->SendMsg(MM_OFFLINE_STORE_CREATE, reinterpret_cast<int>(this->titleEdit->Text()), 0, 0, 0);
}

#pragma inline_depth()

// OFFSET: 0x00974820
// FUNCTION: fn_13306
void UIMerchantShopMakeWnd::AddActionButton() {
  this->actionButton = new UIBasicButton();
  this->actionButton->Configure(60, LookupMessage(MSI_GUILD_AGIT_AGIT_MESSAGE));
  this->actionButton->Move(this->geometry.width - 156, this->geometry.height - 24);
  this->actionButton->SetTooltip(LookupMessage(MSI_WARNING_UNDER_AGE_MSG1));
  this->actionButton->SetId(560);
  this->AddChild(this->actionButton);
}

// OFFSET: 0x00994800
// FUNCTION: fn_13436
void UIMerchantShopMakeWnd::SetSelection(short row, short column) {
  this->selectedRow = row;
  this->selectedColumn = column;
}

// OFFSET: 0x00995320
// FUNCTION: fn_13444
void UIItemBaseWnd::StoreInfo() {
}

// OFFSET: 0x00995330
// FUNCTION: fn_13445
void UIItemPurchaseWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995370
// FUNCTION: fn_13446
void UIItemSellWnd2::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009953B0
// FUNCTION: fn_13447
void UIItemSellWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009953F0
// FUNCTION: fn_13448
void UIItemShopWnd2::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995430
// FUNCTION: fn_13449
void UIItemShopWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995470
// FUNCTION: fn_13450
void UIItemStoreFindWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009954B0
// FUNCTION: fn_13451
void UIItemStoreSubWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009954F0
// FUNCTION: fn_13452
void UIItemStoreWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995530
// FUNCTION: fn_13453
void UIItemWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->expandedHeight = this->expandedHeight;
    this->saved->selectedTab = this->categoryFilter;
    this->saved->usableRows = this->usableFilter;
    this->saved->equipmentRows = this->equipmentFilter;
    this->saved->etcRows = this->miscFilter;
  }
}

// OFFSET: 0x009955C0
// FUNCTION: fn_13454
void UIMerchantItemMyShopWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995600
// FUNCTION: fn_13455
void UIMerchantItemPurchaseWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995640
// FUNCTION: fn_13456
void UIMerchantItemShopWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995680
// FUNCTION: fn_13457
void UIMerchantItemWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009956C0
// FUNCTION: fn_13458
void UIMerchantMirrorItemWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995700
// FUNCTION: fn_13459
void UIMerchantShopMakeWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995740
// FUNCTION: fn_13460
void UISkillListWnd::StoreInfo() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00994840
// FUNCTION: fn_13437
void UIMerchantShopMakeWnd::SetSuspended(unsigned char suspend) {
  UIWindow* window = this->mode == 1 ? g_windowMgr.buyingShopMirrorItemWnd : g_windowMgr.merchantMirrorItemWnd;
  if (!window) {
    return;
  }

  if (suspend == 1) {
    this->suspended = 1;
    this->Show(0);
    window->Show(0);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_STORE_ASSISTANT_TRADE_ITEM)), 0xFFFF, 0, 0, 0);
    return;
  }

  this->suspended = 0;
  this->Show(1);
  window->Show(1);
  this->selectedRow = -1;
  this->selectedColumn = -1;
}

// OFFSET: 0x00994CC0
// FUNCTION: fn_13440
void UIMerchantShopMakeWnd::SetItems(int count, const short* ids) {
  this->itemsSet = 1;

  UIWindow* window;
  if (this->mode == 1) {
    window = g_windowMgr.buyingShopMirrorItemWnd;
  } else {
    g_session.SetMerchantMyShopItems(count, ids);
    window = g_windowMgr.merchantMirrorItemWnd;
  }

  if (window) {
    window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00994B90
// FUNCTION: fn_13439
void UIMerchantItemMyShopWnd::OpenShop() {
  this->shopOpen = 1;
  if (!this->actionButton) {
    this->actionButton = new UIBasicButton();
    this->actionButton->Configure(42, LookupMessage(MSI_LIMIT_USER));
    this->actionButton->Move(this->geometry.width - this->actionButton->geometry.width - 66, this->geometry.height - 24);
    this->actionButton->SetId(506);
    this->AddChild(this->actionButton);
  }

  g_session.m_muteSellSound = false;
  this->saleAlarm->Show(0);

  const int previousHeight = this->footerHeight;
  this->footerHeight = 57;
  this->Resize(this->geometry.width, this->geometry.height - previousHeight + 57);
  this->SendMsg(nullptr, UM_RESIZE, 7, this->geometry.width, this->geometry.height, 0);
  this->Invalidate();
}

// OFFSET: 0x00994D10
// FUNCTION: fn_13441
void UIItemWnd::ReleaseSelection(unsigned char keep) {
  if (keep) {
    return;
  }

  for (auto it = this->items.begin(); it != this->items.end(); ++it) {
    g_session.ClearItemSelection();
    it->flags[1] = 0;
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(WID_MENUICONWND)) {
    window->SendMsg(nullptr, UM_COMMAND, ID_NOTICE, 194, 0, 0);
  }
}

// OFFSET: 0x009967F0
// FUNCTION: fn_13469
int ItemTypeSortRank(int type) {
  switch (type) {
  case 0:
  case 1:
  case 2:
    return 0;
  case 18:
    return 1;
  case TYPE_ARMOR:
  case TYPE_ARMORTM:
  case TYPE_ARMORTB:
  case TYPE_ARMORMB:
  case TYPE_ARMORTMB:
    return 2;
  case TYPE_WEAPON:
  case TYPE_BOW:
  case TYPE_BOTHHAND:
  case TYPE_GUN:
    return 3;
  case 10:
  case 16:
  case 17:
  case 19:
    return 4;
  case 6:
    return 5;
  case 3:
  case 7:
    return 6;
  default:
    return 10;
  }
}

struct ItemTypeNameLess {
  bool operator()(const ITEM_INFO& left, const ITEM_INFO& right) const;
};

// OFFSET: 0x009700C0
// FUNCTION: fn_13240
bool ItemTypeNameLess::operator()(const ITEM_INFO& left, const ITEM_INFO& right) const {
  const int leftRank = ItemTypeSortRank(left.identity[0]);
  const int rightRank = ItemTypeSortRank(right.identity[0]);
  if (leftRank == rightRank) {
    return left.DisplayName(0) < right.DisplayName(0);
  }

  return leftRank < rightRank;
}

// OFFSET: 0x00996870
// FUNCTION: fn_13470
bool UIItemStoreFindWnd::Sort() {
  if (!this->UpdateItemNames()) {
    return false;
  }
  this->items.sort();
  return true;
}

// OFFSET: 0x009968B0
// FUNCTION: fn_13471
bool UIItemStoreSubWnd::Sort2() {
  if (!this->UpdateItemNames2()) {
    return false;
  }
  this->items.sort();
  return true;
}

// OFFSET: 0x009968F0
// FUNCTION: fn_13472
bool UIItemStoreWnd::Sort() {
  if (!this->UpdateItemNames()) {
    return false;
  }
  this->items.sort();
  return true;
}

// OFFSET: 0x00995CF0
// FUNCTION: fn_13463
bool UIItemStoreFindWnd::SortReversed() {
  if (!this->UpdateItemNames()) {
    return false;
  }
  this->items.sort();
  this->items.reverse();
  return true;
}

// OFFSET: 0x00995D50
// FUNCTION: fn_13464
bool UIItemStoreSubWnd::SortReversed2() {
  if (!this->UpdateItemNames2()) {
    return false;
  }
  this->items.sort();
  this->items.reverse();
  return true;
}

// OFFSET: 0x00995DB0
// FUNCTION: fn_13465
bool UIItemStoreWnd::SortReversed() {
  if (!this->UpdateItemNames()) {
    return false;
  }
  this->items.sort();
  this->items.reverse();
  return true;
}

// OFFSET: 0x00996940
// FUNCTION: fn_13473
void UIItemStoreSubWnd::SetTabIndex(int index) {
  this->tabIndex = index;
}

// OFFSET: 0x00997080
// FUNCTION: fn_13478
void UIItemStoreWnd::SetTabChecked(int index, bool checked) {
  this->tabs[index]->SetChecked(checked);
}

// OFFSET: 0x00997160
// FUNCTION: fn_13481
void UIMerchantItemMyShopWnd::SetDuration(std::uint32_t value) {
  this->startTick = timeGetTime();
  this->duration = value;
}

// OFFSET: 0x00997180
// FUNCTION: fn_13482
void UIMerchantItemShopWnd::StartTimer(std::uint32_t value) {
  this->startTick = timeGetTime();
  this->duration = value;

  const int previousHeight = this->footerHeight;
  this->footerHeight = 57;
  this->Resize(this->geometry.width, this->geometry.height - previousHeight + 57);
  this->SendMsg(nullptr, UM_RESIZE, 3, this->geometry.width, this->geometry.height, 0);
}

// OFFSET: 0x009970A0
// FUNCTION: fn_13479
void UIItemStoreFindWnd::SetTitle(std::string title) {
  this->title->SetTextFitted(title.c_str());
}

// OFFSET: 0x00997100
// FUNCTION: fn_13480
void UIItemStoreSubWnd::SetTitle(std::string title) {
  this->title->SetTextFitted(title.c_str());
}

// OFFSET: 0x009971E0
// FUNCTION: fn_13483
void UIItemStoreFindWnd::SetSearchText(std::string text) {
  this->searchText = text;
}

// OFFSET: 0x00971490
// FUNCTION: fn_13263
int UIMerchantShopMakeWnd::CommitPrice(int next) {
  if (this->editIndex == -1 || g_session.m_skipPriceWarning == 0) {
    this->editIndex = next;
    return 1;
  }

  UIEditCtrl* edit = this->priceEdits[this->editIndex];
  char text[24];
  std::sprintf(text, "%s ", edit->Text());
  if (!g_session.IsNumberWithCommas(text)) {
    g_windowMgr.Confirm(LookupMessage(MSI_CAN_INPUT_NUMBER_ONLY), 0, true, false, 0, "Message", 280, 120, 0);
    edit->SetText("0");
    g_windowMgr.SetFocus(edit);
    edit->SelectAll();
    return 0;
  }

  std::int64_t price = _atoi64(text);
  if (price <= 0x7FFFFFFF && price < 0) {
    price = 0;
  } else if (price > 0x7FFFFFFF || price > 1000000000) {
    if (g_windowMgr.Confirm(LookupMessage(MSI_BASIC_EXP_MSG_INDONESIA), 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
      edit->SetText("0");
      g_windowMgr.SetFocus(edit);
      edit->SelectAll();
      return 0;
    }

    price = 1000000000;
    edit->SetText(_itoa(1000000000, text, 10));
    this->Invalidate();
  }

  if (price >= std::atoi(LookupMessage(MSI_WARNING_PRICE7))) {
    ITEM_INFO item = g_session.GetItem1768At(this->editIndex);
    char name[128];
    int capacity = 128;
    item.GetIdDisplayName(name, capacity, 0);
    std::string message = FormatOwnedString(LookupMessage(MSI_WARNING_PRICE4), name);

    const int high = static_cast<int>(price / std::atoi(LookupMessage(MSI_WARNING_PRICE5)));
    if (high > 0) {
      message += FormatOwnedString(LookupMessage(MSI_WARNING_PRICE6), high);
    }

    const int low = static_cast<int>(price % std::atoi(LookupMessage(MSI_WARNING_PRICE5))) / std::atoi(LookupMessage(MSI_WARNING_PRICE7));
    if (low > 0) {
      message += FormatOwnedString(LookupMessage(MSI_AMERICA_CASHSHOP_URL), low);
    }

    message += std::string(LookupMessage(MSI_AMERICA_CASHSHOP_URL_STEAM));
    if (g_windowMgr.Confirm(message.c_str(), 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
      edit->SetText("0");
      g_windowMgr.SetFocus(edit);
      edit->SelectAll();
      return 0;
    }
  }

  const PriceColors colors = PriceColor(price);
  edit->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
  FormatGroupedInteger(price, text, sizeof(text));
  edit->SetText(text);

  this->editIndex = next;
  if (next == -1) {
    g_windowMgr.SetFocus(nullptr);
    return 1;
  }

  this->priceEdits[next]->TextColor(0, 0, 0, 0xFFFFFFFF);
  g_windowMgr.SetFocus(this->priceEdits[next]);
  this->priceEdits[next]->SelectAll();
  return 1;
}

// OFFSET: 0x009719C0
// FUNCTION: fn_13264
void UIMerchantShopMakeWnd::PrintBuyList() {
  auto it = this->items.begin();
  char line[256];
  char name[256];
  char text[24];
  char price[32];
  char quantity[32];
  char grouped[32];

  std::sprintf(line, "======================  BUY LIST  ======================");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x96FFFF, 0, 0, 0);

  for (unsigned index = 0; index < this->items.size(); ++index, ++it) {
    int capacity = 256;
    it->GetIdDisplayName(name, capacity, 0);

    std::sprintf(text, "%s ", this->priceEdits[index]->Text());
    g_session.IsNumberWithCommas(text);
    if (std::atoi(text) > 99990000) {
      std::sprintf(price, "99,990,000");
    } else {
      const int value = std::atoi(text);
      std::sprintf(price, "%s", FormatGroupedInteger(value, grouped, 32));
    }

    const double rate = PriceCommission(_atoi64(text)) * 0.01;
    const std::int64_t commission = static_cast<std::int64_t>(_atoi64(text) * rate);

    std::sprintf(text, "%s ", this->quantityEdits[index]->Text());
    g_session.IsNumberWithCommas(text);
    if (std::atoi(text) > 9999) {
      std::sprintf(quantity, "9,999");
    } else {
      const int value = std::atoi(text);
      std::sprintf(quantity, "%s", FormatGroupedInteger(value, grouped, 32));
    }

    std::sprintf(line, LookupMessage(MSI_BUYINGSTORE_MAKEWND_ITEMLIST), name, price, quantity);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x5AFFFF, 0, 0, 0);

    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("Commission : 1 EA = %s Zeny", FormatGroupedInteger(commission, line, 256)).c_str()), 0x5AFFFF, 0, 0, 0);
  }

  std::sprintf(price, "%s", FormatGroupedInteger(g_session.m_zeny, grouped, 32));
  std::sprintf(line, "                                          Current Zeny : %s Z", price);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x78FFFF, 0, 0, 0);
  std::sprintf(line, "============================ ============================");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x96FFFF, 0, 0, 0);
}

// OFFSET: 0x00972100
// FUNCTION: fn_13266
void UIItemBaseWnd::DragAndDrop(int, int, std::intptr_t) {
  g_modeMgr.gameMode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00972120
// FUNCTION: fn_13267
void UIItemPurchaseWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 3) {
    switch (drag->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    default:
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 1, 0, 0, 0);
      return;
    }
  }

  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x009721F0
// FUNCTION: fn_13268
void UIItemSellWnd2::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 3) {
    if (drag->quantity != 1 && g_windowMgr.dragAllItems == 0) {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 16, 0, 0, 0);
    } else {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    }
  } else {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
  }
}

// OFFSET: 0x009722A0
// FUNCTION: fn_13269
void UIItemSellWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 3) {
    if (drag->quantity != 1 && g_windowMgr.dragAllItems == 0) {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 2, 0, 0, 0);
    } else {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    }
  } else {
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00972B50
// FUNCTION: fn_13277
void UIMerchantItemShopWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 13) {
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 11, 0, 0, 0);
}

// OFFSET: 0x00972C00
// FUNCTION: fn_13278
void UIMerchantItemWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI) || g_windowMgr.sayDialogWnd || g_windowMgr.choose3Wnd) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
  case 6:
    break;
  default:
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }

  if (static_cast<CGameMode*>(mode)->ConfirmDraggedItemUse()) {
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
  g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 8, 0, 0, 0);
}

// OFFSET: 0x0097BC00
// FUNCTION: fn_13321
void UIMerchantItemShopWnd::OnCreate(int width, int height) {
  if (this->buyingShop) {
    this->notice = new UIStaticText();
    this->notice->SetSize(160, 15);
    char text[80];
    std::sprintf(text, LookupMessage(MSI_BUYINGSTORE_MYSHOPWND_LIMITZENY), 0);
    this->notice->SetTextAutoWidth(text);
    this->notice->Move(12, this->geometry.height - 21);
    this->AddChild(this->notice);
  }

  const unsigned rows = static_cast<unsigned>(height - this->footerHeight - 17) / 32;
  this->visibleRows = rows;
  this->columns = 1;
  this->nonItemHeight = this->geometry.height - rows * 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00854130
// FUNCTION: fn_10746
int UIEditCtrl3::DecodeLinkNumber(std::string text) {
  char digits[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int value = 0;
  const int size = static_cast<int>(text.size()) + 1;
  char* characters = new char[size];
  strcpy_s(characters, size, text.c_str());

  const int length = size - 1;
  for (int position = 0; position < length; ++position) {
    for (int digit = 0; digit < 62; ++digit) {
      if (digits[digit] != characters[position]) {
        continue;
      }

      int weight = 1;
      for (int power = 1; power < length - position; ++power) {
        weight *= 62;
      }

      value += weight * digit;
      break;
    }
  }

  if (characters) {
    delete[] characters;
  }

  return value;
}

// OFFSET: 0x008603D0
// FUNCTION: fn_10899
void UIEditCtrl3::DecodeLinkOptions(std::vector<int>& options, const char* text, int& position, const int& length, const char& separator, const char& alternate) {
  std::string value;
  while (++position < length + 1) {
    const char byte = text[position];
    if (byte >= '0') {
      value += byte;
    } else if (separator != byte && alternate != byte) {
      --position;
      options.push_back(this->DecodeLinkNumber(value));
      break;
    } else {
      options.push_back(this->DecodeLinkNumber(value));
      value.clear();
    }
  }
}

// OFFSET: 0x00860790
// FUNCTION: fn_10900
int UIEditCtrl3::DecodeLinkCards(std::vector<int>& fields, const char* text, int& position, const char& separator, const int& length, int index) {
  std::string value;
  std::vector<int> cards;
  int count = 1;
  while (++position < length + 1) {
    const char byte = text[position];
    if (byte >= '0') {
      value += byte;
    } else if (separator == byte) {
      ++count;
      cards.push_back(this->DecodeLinkNumber(value));
      value.clear();
    } else {
      --position;
      cards.push_back(this->DecodeLinkNumber(value));
      fields[index] = count;
      for (unsigned int i = 0; i < cards.size(); ++i) {
        fields[index + 1 + i] = cards[i];
      }
      break;
    }
  }

  return count;
}

// OFFSET: 0x00853840
// FUNCTION: fn_10743
std::string UIEditCtrl3::ResolveItemLinks(const std::string& source) {
  if (!_mbsstr(reinterpret_cast<const unsigned char*>(source.c_str()), reinterpret_cast<const unsigned char*>(g_itemLinkOpen.c_str()))) {
    return source;
  }

  std::string remaining = source;
  std::string result;
  int position = 0;
  int previous = 0;
  for (;;) {
    std::string link;
    if (!this->ExtractItemLink(remaining, link, position, source.c_str())) {
      break;
    }

    result += source.substr(previous, position - previous);

    std::string name;
    if (this->LinkDisplayName(link, name)) {
      name = this->BracketFittedText(name);
      result += name;
      position += static_cast<int>(link.size() + g_itemLinkClose.size() + g_itemLinkOpen.size());
    }

    previous = position;
  }

  result += remaining;
  if (result.find(g_itemLinkOpen) != std::string::npos) {
    result = result.substr(result.find(g_itemLinkOpen));
  }

  return result;
}

// OFFSET: 0x008595F0
// FUNCTION: fn_10777
std::string UIEditCtrl3::PlaceLineLinks(const char* line, std::string& text, const int* lineIndex) {
  if (text.find(g_itemLinkOpen) == std::string::npos) {
    return std::string(line, std::strlen(line));
  }

  std::string remaining(text);
  std::string result;
  std::vector<UIItemTagButton*> buttons;
  int position = 0;
  int start;

  for (;;) {
    std::string link;
    const int previous = position;
    start = previous;
    if (!this->ExtractItemLink(remaining, link, position, text.c_str())) {
      break;
    }

    if (result.size() >= std::strlen(line)) {
      break;
    }

    std::string chunk = text.substr(previous, position - previous);
    if (result.size() + chunk.size() > std::strlen(line)) {
      const int room = static_cast<int>(std::strlen(line) - result.size());
      if (room > 0) {
        result += chunk.substr(0, room);
        start = previous + room;
        break;
      }
    }

    result += chunk;

    if (link.substr(0, 6) == "00000T") {
      const int id = ItemLinkMessageId(link);
      if (g_itemTagOwner->maxTipId >= id) {
        std::string name;
        const std::string title = g_itemTagOwner->Title(id, &name, 17);
        if (name.size() + 2 + result.size() > std::strlen(line)) {
          start = previous + static_cast<int>(chunk.size());
          break;
        }

        if (title.size() != 0) {
          UIItemTagButton* button = this->CreateItemTagButton(title, name, id, this, 0x7F00E4);
          if (button) {
            buttons.push_back(button);
            button->line = *lineIndex;
            button->column = this->TextWidth(result.c_str(), 0, 0, 12, 0, 0) - this->textY;
            result += std::string(button->label);
          }
        }

        position += static_cast<int>(link.size() + g_itemLinkClose.size() + g_itemLinkOpen.size());
        continue;
      }
    }

    std::vector<int> fields;
    this->DecodeLinkFields(link, fields);
    ITEM_INFO item;
    if (this->DecodeItemLink(fields, item)) {
      const std::string name = this->BracketFittedText(item.DisplayName(0));
      if (name.size() + result.size() > std::strlen(line)) {
        start = previous + static_cast<int>(chunk.size());
        break;
      }

      UIItemTagButton* button = new (std::nothrow) UIItemTagButton();
      button->item = item;
      button->Configure(name.c_str(), 0x82F2E1, 0, 12, 0, 0);
      button->SetTooltip(item.DisplayName(0).c_str());
      button->m_hoverColor = 0x82F2E1;
      button->SetId(0x1D0);
      button->ResizeToTextClipped(this->TextWidth(name.c_str(), 0, 0, 12, 0, 0) + 1, button->geometry.height);
      this->AddChild(button);
      buttons.push_back(button);
      button->line = *lineIndex;
      button->column = this->TextWidth(result.c_str(), 0, 0, 12, 0, 0) - this->textY;
      result += name;
      position += static_cast<int>(link.size() + g_itemLinkClose.size() + g_itemLinkOpen.size());
    }
  }

  if (result.size() < std::strlen(line)) {
    const int room = static_cast<int>(std::strlen(line) - result.size());
    if (room > 0 && remaining.size() >= static_cast<unsigned>(room)) {
      result += remaining.substr(0, room);
      start += room;
    }
  }

  text = text.substr(start);
  this->entries.push_back(buttons);
  return result;
}

// OFFSET: 0x00869810
// FUNCTION: fn_10973
void UIEditCtrl3::SetLinkedText(const char* value) {
  std::string resolved = this->ResolveItemLinks(std::string(value));

  std::vector<std::string> lines;
  int width = this->geometry.width - 6;
  if (this->scrollEnabled != 0) {
    width += 3 - this->scrollBar->geometry.width;
  }
  this->WrapTextWithBreaks(resolved.c_str(), &lines, &this->lineBreaks, width);
  this->WrapLines(lines);

  this->lineCount = static_cast<int>(lines.size());
  if (this->multiline != 0) {
    this->lineCount = static_cast<int>(lines.size()) + 1;
  }
  this->UpdateScrollBar();

  if (this->scrollEnabled != 0) {
    lines.clear();
    resolved = this->ResolveItemLinks(std::string(value));
    width = this->geometry.width - 6;
    if (this->scrollEnabled != 0) {
      width += 3 - this->scrollBar->geometry.width;
    }
    this->WrapTextWithBreaks(resolved.c_str(), &lines, &this->lineBreaks, width);
    this->WrapLines(lines);
  }

  std::string text(value);
  for (std::size_t index = 0; index < lines.size(); ++index) {
    const int lineIndex = static_cast<int>(index);
    lines[index] = this->PlaceLineLinks(lines[index].c_str(), text, &lineIndex);
  }

  this->SetText(resolved.c_str());
}

// OFFSET: 0x00860C60
// FUNCTION: fn_10901
void UIEditCtrl3::WrapLines(std::vector<std::string>& lines) {
  const std::string open = "<";
  const std::string close = ">";
  std::string remaining;

  for (std::size_t index = 0; index < lines.size(); ++index) {
    std::vector<std::string> pieces;
    std::vector<bool> breaks;
    this->WrapTextWithBreaks(lines[index].c_str(), &pieces, &breaks, this->geometry.width - 6);

    if (pieces.size() > 1) {
      for (std::size_t piece = 0; piece < pieces.size(); ++piece) {
        if (piece == 0) {
          lines[index] = pieces[0];
        }

        if (index + piece < lines.size() && piece == pieces.size() - 1) {
          lines[index + piece].swap(pieces[piece].append(lines[index + piece]));
        } else if (piece != 0) {
          std::vector<std::string> merged;
          for (std::size_t line = 0; line < lines.size(); ++line) {
            if (lines[line].size() != 0) {
              merged.push_back(lines[line]);
            }
          }

          if (pieces[piece].size() != 0) {
            merged.push_back(pieces[piece]);
          }

          for (std::size_t line = index + 1; line < lines.size(); ++line) {
            if (lines[line].size() != 0) {
              merged.push_back(lines[line]);
            }
          }

          if (merged.size() != 0) {
            lines.swap(merged);
          }
        }

        if (index > 40) {
          break;
        }
      }
    }

    if (index < lines.size()) {
      remaining = lines[index];
      std::size_t position = 0;
      std::size_t found = remaining.find(close);
      while (found != std::string::npos) {
        ++found;
        position += found;
        remaining = remaining.substr(found);
        found = remaining.find(close);
      }

      found = remaining.find(open);
      if (found != std::string::npos) {
        position += found;
        remaining = remaining.substr(found);
        std::string tail = remaining.substr(1);
        found = tail.find(open);
        while (found != std::string::npos) {
          position += found;
          remaining = tail;
          tail = remaining.substr(1);
          found = tail.find(open);
        }

        if (index + 1 < lines.size() && lines[index + 1].find(close) != std::string::npos) {
          lines[index + 1].swap(remaining.append(lines[index + 1]));
          lines[index] = lines[index].substr(0, position);
        }
      }
    }
  }
}

// OFFSET: 0x00975CD0
// FUNCTION: fn_13310
void UIItemSellWnd2::OnCreate(int width, int height) {
  this->total = new UIStaticText();
  this->total->SetSize(160, 15);
  this->total->SetTextAutoWidth("");
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  UIStaticText* title = new UIStaticText();
  title->SetSize(90, 15);
  title->SetText(LookupMessage(MSI_ITEMS_FOR_CHANGE), 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(3);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_exchange", "btn_exchange_a", "btn_exchange_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
  int ids[2] = {184, 185};
  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->buttons[index]->Move(positions[index][0], positions[index][1]);
    this->buttons[index]->SetId(ids[index]);
    this->AddChild(this->buttons[index]);
  }

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x009768A0
// FUNCTION: fn_13311
void UIItemSellWnd::OnCreate(int width, int height) {
  this->total = new UIStaticText();
  this->total->SetSize(160, 15);
  char text[80];
  std::sprintf(text, "Total : %d Zeny", 0);
  this->total->SetTextAutoWidth(text);
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  UIStaticText* title = new UIStaticText();
  title->SetSize(90, 15);
  title->SetText(LookupMessage(MSI_ITEMS_FOR_SELL), 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(3);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_sell", "btn_sell_a", "btn_sell_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
  int ids[2] = {184, 185};
  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->buttons[index]->Move(positions[index][0], positions[index][1]);
    this->buttons[index]->SetId(ids[index]);
    this->AddChild(this->buttons[index]);
  }

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00977510
// FUNCTION: fn_13312
void UIItemShopWnd2::OnCreate(int width, int height) {
  this->title = new UIStaticText();
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(MSI_ITEM_STORE), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->checkBox = new UICheckBox();
  this->checkBox->SetSize(200, 12);
  this->checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_NOQUESTION_ITEMCOUNT));
  this->checkBox->Move(12, this->geometry.height - 20);
  this->checkBox->SetChecked(g_windowMgr.dragAllItems);
  this->AddChild(this->checkBox);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->extraBottomHeight - this->bottomBarHeight - 17);
  this->AddChild(this->scrollBar);

  this->columns = 1;
  this->visibleRows = (height - this->extraBottomHeight - this->bottomBarHeight - 17) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x009777B0
// FUNCTION: fn_13313
void UIItemShopWnd::OnCreate(int width, int height) {
  this->title = new UIStaticText();
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(MSI_ITEM_STORE), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->checkBox = new UICheckBox();
  this->checkBox->SetSize(200, 12);
  this->checkBox->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(MSI_NOQUESTION_ITEMCOUNT));
  this->checkBox->Move(12, this->geometry.height - 20);
  this->checkBox->SetChecked(g_windowMgr.dragAllItems);
  this->AddChild(this->checkBox);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->footerHeight - this->bottomPadding - 17);
  this->AddChild(this->scrollBar);

  this->columns = 1;
  this->visibleRows = (height - this->footerHeight - this->bottomPadding - 17) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00977A50
// FUNCTION: fn_13314
void UIItemStoreFindWnd::OnCreate(int width, int height) {
  this->scrollBar = new UIScrollBar();
  static_cast<UIScrollBar*>(this->scrollBar)->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  char text[64];
  std::sprintf(text, "서브창고");
  this->title = new UIStaticText();
  this->title->SetSize(this->geometry.width - 32, 15);
  this->title->SetTextFitted(text);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizeHandle = new UIResizer();
  this->resizeHandle->SetSize(16, 16);
  this->resizeHandle->SetDirection(7);
  this->resizeHandle->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizeHandle->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->resizeHandle->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizeHandle);

  this->closeButton = this->AddCloseButton("", 0);
  this->closeButton->Move(width - 16, 3);
}

// OFFSET: 0x00977C40
// FUNCTION: fn_13315
void UIItemStoreSubWnd::OnCreate(int width, int height) {
  this->scrollBar = new UIScrollBar();
  static_cast<UIScrollBar*>(this->scrollBar)->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  char text[64];
  std::sprintf(text, "서브창고");
  this->title = new UIStaticText();
  this->title->SetSize(this->geometry.width - 32, 15);
  this->title->SetTextFitted(text);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizeHandle = new UIResizer();
  this->resizeHandle->SetSize(16, 16);
  this->resizeHandle->SetDirection(7);
  this->resizeHandle->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizeHandle->Move(this->geometry.width - 15, this->geometry.height - 15);
  this->resizeHandle->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizeHandle);
}

// OFFSET: 0x0097BD30
// FUNCTION: fn_13322
void UIMerchantItemWnd::OnCreate(int width, int height) {
  UIStaticText* title = new UIStaticText();
  title->SetSize(90, 15);
  title->SetText(LookupMessage(MSI_MERCHANTITEM), 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  this->info = new UIStaticText();
  this->info->SetSize(220, 15);
  char text[128];
  std::sprintf(text, "Num: %d/%d Weight: %d/%d", g_session.cartCount, g_session.cartMaxCount, g_session.cartWeight, g_session.cartMaxWeight);
  this->info->SetText(text, 0);
  this->info->Move(16, this->geometry.height - 19);
  this->AddChild(this->info);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  this->closeButton = this->AddCloseButton("Alt+W", 0);
  this->columns = (width - 40) / 32;
  this->visibleRows = (height - 38) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
  this->info->Show(0);
}

// OFFSET: 0x0097BFF0
// FUNCTION: fn_13323
void UIMerchantMirrorItemWnd::OnCreate(int width, int height) {
  UIStaticText* title = new UIStaticText();
  title->SetSize(150, 15);
  int messageId;
  if (this->mode == 1) {
    messageId = 1733;
  } else if (this->mode == 2) {
    messageId = 1738;
  } else {
    messageId = 232;
  }
  const char* text = LookupMessage(messageId);
  title->SetText(text, 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  this->columns = (width - 40) / 32;
  this->visibleRows = (height - 38) / 32;
  if (this->mode == 1) {
    g_session.CollectFilteredMerchantMyShopItems();
  } else if (this->mode == 2) {
    g_session.RefreshMerchantMyShopItems();
  } else if (Replay::Instance().state != 6) {
    g_session.CopyItems1740To1750();
  }
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x0098A7A0
// FUNCTION: fn_13417
void UISkillListWnd::UpdateRowButtons() {
  for (int row = 0, y = 34; y < 258; ++row, y += 32) {
    CSkillInfo skill = this->GetVisibleSkill(row);
    int points = 0;
    if (this->skillTab == 0) {
      points = g_session.m_homunSkillPoint;
    } else if (this->skillTab == 1) {
      points = g_session.m_mercenarySkillPoint;
    }

    if (row < this->visibleRows && this->scroll + row < this->skills.size() && points != 0 && skill.available != 0 && skill.upgradable != 0) {
      this->upgradeButtons[row]->Move(78, y - 13);
    } else {
      this->upgradeButtons[row]->Move(-200, y - 17);
    }

    if (row < this->visibleRows && this->scroll + row < this->skills.size() && skill.available != 0 && skill.level != 0 &&
        CSkillInfo::UsesLevel(static_cast<int>(skill.id))) {
      this->levelButtons[row]->Move(112, y);
      this->levelLabels[row]->Move(192, y);
    } else {
      this->levelButtons[row]->Move(-200, y);
      this->levelLabels[row]->Move(-200, y);
    }
  }
}

struct ThreadFormatBuffer {
  char text[0x800];
};

// OFFSET: 0x00974630
// FUNCTION: fn_13305
const char* FormatThreadText(const char* format, ...) {
  // GLOBAL: gl_25164
  static std::map<int, ThreadFormatBuffer> buffers;
  char* buffer = buffers[reinterpret_cast<int>(GetCurrentThread())].text;
  va_list arguments;
  va_start(arguments, format);
  _vsnprintf(buffer, 0x800, format, arguments);
  va_end(arguments);
  return buffer;
}

// OFFSET: 0x00994D80
// FUNCTION: fn_13442
bool __stdcall ReturnTrue(int) {
  return true;
}

// OFFSET: 0x0089CAE0
// FUNCTION: fn_11591
void UIEditCtrl_Custom::Trim() {
  const auto length = this->text.size();
  if (static_cast<int>(length) <= this->maximum) {
    return;
  }

  const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);
  while (length - std::strlen(next) <= static_cast<unsigned>(this->maximum)) {
    next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
  }

  const char* previous = CharPrevExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), next, 0);
  this->text.resize(length - std::strlen(previous));

  g_serverLanguage->ResetInput();
  g_serverLanguage->ReplaceInput(this->text.c_str());

  while (this->text.size() < static_cast<unsigned>(this->maximum)) {
    this->text += " ";
  }
}

// OFFSET: 0x00972350
// FUNCTION: fn_13270
void UIItemShopWnd2::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
  case 5:
    if (this->interactionDisabled == 0) {
      return;
    }

    if (drag->quantity == 1 || g_windowMgr.dragAllItems != 0) {
      this->SendMsg(nullptr, UM_DROP, 1, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 15, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00972410
// FUNCTION: fn_13271
void UIItemShopWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
    if (this->sellMode != 0) {
      return;
    }

    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 3, 0, 0, 0);
    return;
  case 5:
    if (this->sellMode == 0) {
      return;
    }

    if (drag->quantity == 1 || g_windowMgr.dragAllItems != 0) {
      this->SendMsg(nullptr, UM_DROP, 1, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 4, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x009729C0
// FUNCTION: fn_13276
void UIMerchantItemPurchaseWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 12) {
    switch (drag->type) {
    case TYPE_ARMOR:
    case TYPE_WEAPON:
    case TYPE_BOW:
    case TYPE_BOTHHAND:
    case TYPE_ARMORTM:
    case TYPE_ARMORTB:
    case TYPE_ARMORMB:
    case TYPE_ARMORTMB:
    case TYPE_GUN:
      if (g_session.GetItem1760Quantity(drag->id)) {
        mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_EQUIPITEM_OLNY_ONE)), 0xFF, 0, 0, 0);
        return;
      }

      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    default:
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 12, 0, 0, 0);
      return;
    }
  }

  if (drag->source == 22) {
    if (drag->quantity > 1) {
      g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 17, 0, 0, 0);
      return;
    }

    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  }

  mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
}

// OFFSET: 0x00972CE0
// FUNCTION: fn_13279
void UIMerchantMirrorItemWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 10:
  case 21:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    if (this->mode == 2) {
      g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 18, 0, 0, 0);
      return;
    }

    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 10, 0, 0, 0);
    return;
  case 19:
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  default:
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00972DB0
// FUNCTION: fn_13280
void UIMerchantShopMakeWnd::DragAndDrop(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(WID_EXCHANGEUI)) {
    UIWindow::DragAndDrop(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 11:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
      return;
    }

    g_windowMgr.ShowWindowById(WID_ITEMDROPCNTWND, 1);
    g_windowMgr.itemDropCntWnd->SendMsg(nullptr, UM_SETSTATE, 9, 0, 0, 0);
    return;
  case 20:
    this->SendMsg(nullptr, UM_DROP, 0, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return;
  }
}

// INLINED
static void AppendReplayRecord(std::vector<unsigned char>& records, const std::vector<unsigned char>& record) {
  records.insert(records.end(), record.begin(), record.end());
}

// OFFSET: 0x00972E60
// FUNCTION: fn_13281
std::vector<unsigned char> UIItemPurchaseWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x50DC, nullptr, 0));
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x50DD, nullptr, 0));
  return records;
}

// OFFSET: 0x00972F50
// FUNCTION: fn_13282
std::vector<unsigned char> UIItemSellWnd2::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(20800, nullptr, 0));
  AppendReplayRecord(records, BuildReplayTaggedRecord(20802, &this->shopId, 4));
  AppendReplayRecord(records, BuildReplayTaggedRecord(20801, nullptr, 0));
  return records;
}

// OFFSET: 0x00973080
// FUNCTION: fn_13283
std::vector<unsigned char> UIItemSellWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x510E, nullptr, 0));
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x510F, nullptr, 0));
  return records;
}

// OFFSET: 0x00973170
// FUNCTION: fn_13284
std::vector<unsigned char> UIItemStoreWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x51A4, nullptr, 0));
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x51A5, nullptr, 0));
  return records;
}

// OFFSET: 0x00973260
// FUNCTION: fn_13285
std::vector<unsigned char> UIItemShopWnd::BuildReplayRecords() {
  std::vector<unsigned char> records;
  if (this->hovered != 0) {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21050, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21051, nullptr, 0));
  } else {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21000, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21001, nullptr, 0));
  }
  return records;
}

// OFFSET: 0x009733A0
// FUNCTION: fn_13286
std::vector<unsigned char> UIParaItemShopWnd::BuildReplayRecords() {
  std::vector<unsigned char> records;
  if (this->footerHeight != 0) {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21100, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21101, nullptr, 0));
  } else {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21150, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21151, nullptr, 0));
  }
  return records;
}

// OFFSET: 0x009734E0
// FUNCTION: fn_13287
std::vector<unsigned char> UIMerchantItemShopWnd::GetReplayData() {
  std::vector<unsigned char> records;
  if (this->buyingShop != 0) {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21250, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21253, &this->buyingZeny, 4));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21251, nullptr, 0));
  } else {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21200, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21201, nullptr, 0));
  }
  return records;
}

// OFFSET: 0x00973670
// FUNCTION: fn_13288
std::vector<unsigned char> UIMerchantMirrorItemWnd::GetReplayData() {
  std::vector<unsigned char> records;
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x5398, nullptr, 0));
  AppendReplayRecord(records, BuildReplayTaggedRecord(0x5399, nullptr, 0));
  return records;
}

// OFFSET: 0x00973760
// FUNCTION: fn_13289
std::vector<unsigned char> UIMerchantShopMakeWnd::GetReplayData() {
  std::vector<unsigned char> records;
  if (this->mode != 0) {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21350, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21351, nullptr, 0));
  } else {
    AppendReplayRecord(records, BuildReplayTaggedRecord(21300, nullptr, 0));
    AppendReplayRecord(records, BuildReplayTaggedRecord(21301, nullptr, 0));
  }
  return records;
}

// OFFSET: 0x009738A0
// FUNCTION: fn_13290
int UIItemBaseWnd::GetItemIndexAt(int x, int y) {
  if (x < 40 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return -1;
  }

  int column = (x - 40) / 32;
  const int row = (y - 17) / 32;

  if (column > this->columns - 1) {
    column = this->columns - 1;
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->damaged) {
        if (visible == this->columns * row + column) {
          return index;
        }

        ++visible;
      }
    }
  }

  return -1;
}

// OFFSET: 0x00984050
// FUNCTION: fn_13349
void UIItemShopWnd2::OnLBtnDblClk(int x, int y) {
  if (this->interactionDisabled == 0 || g_windowMgr.dragAllItems == 0) {
    return;
  }

  UIWindow* target = static_cast<UIWindow*>(g_windowMgr.itemSellWnd2);

  if (target) {
    this->OnLBtnDown(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->RecordDrop(x, y, mode->drag, 161);
    target->DragAndDrop(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x009840C0
// FUNCTION: fn_13350
void UIItemShopWnd::OnLBtnDblClk(int x, int y) {
  if (this->sellMode == 0 || g_windowMgr.dragAllItems == 0) {
    return;
  }

  UIWindow* target = static_cast<UIWindow*>(g_windowMgr.itemSellWnd);

  if (target) {
    this->OnLBtnDown(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->RecordDrop(x, y, mode->drag, 24);
    target->DragAndDrop(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x00985AA0
// FUNCTION: fn_13370
void UISkillListWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  const int previous = this->hovered;
  this->hovered = this->SkillAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  CSkillInfo skill = this->GetSkillAt(x, y);

  if (skill.available && skill.level && skill.type) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (mode) {
      mode->BeginDragSkill(8, skill.id, skill.name, g_session.SkillLevel(skill.id), -1);
    }
  }
}

// OFFSET: 0x009844E0
// FUNCTION: fn_13354
void UISkillListWnd::OnLBtnDblClk(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (this->hovered != -1) {
    CSkillInfo skill = this->GetSkillByIndex(this->hovered);

    if (skill.available && skill.level && skill.spCost) {
      this->SendMsg(nullptr, UM_USESKILL, reinterpret_cast<std::intptr_t>(&skill), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00985510
// FUNCTION: fn_13366
void UIMerchantItemShopWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  if (this->buyingShop == 0) {
    const int previous = this->hovered;
    this->hovered = this->GetItemIndexAt(x, y);

    if (previous != this->hovered) {
      this->Invalidate();
    }

    ITEM_INFO item = this->GetItemByIndex(this->hovered);

    if (item.identity[1] > 0) {
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->BeginDragItem(12, item, 1, -1);
    }
  }
}

// OFFSET: 0x00988C60
// FUNCTION: fn_13391
void UIItemPurchaseWnd::OnRBtnDown(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988F90
// FUNCTION: fn_13395
void UIItemShopWnd::OnRBtnDown(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    g_modeMgr.GetGameMode();
    g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0098A260
// FUNCTION: fn_13409
void UIItemBaseWnd::OnWheel(int value) {
  if (this->scrollOverflow) {
    const int step = std::min(this->visibleRows - 1, 3);
    if (value < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00993A80
// FUNCTION: fn_13435
int UISkillListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (g_skillTooltipActive == 1) {
    const int elapsed = timeGetTime() - g_skillTooltipTick;
    if (elapsed > 1000 || elapsed < 0) {
      g_skillTooltipActive = 0;
      this->Invalidate();
    }
  }

  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<SkillListWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    return 0;
  case UM_RESIZE:
    if (a == 3) {
      int width = b <= 320 ? b : 320;
      int height = c <= 288 ? c : 288;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }
      width = (width - 280) / 32 * 32 + 280;
      height = (height - 120) / 32 * 32 + 120;
      if (width != this->geometry.width || height != this->geometry.height) {
        this->Resize(width, height);
        this->columns = 1;
        this->visibleRows = (height - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->buttons[0]->Move(this->geometry.width - 107, this->geometry.height - 24);
        this->buttons[1]->Move(this->geometry.width - 61, this->geometry.height - 24);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        int row = 0;
        for (int y = 17; y < 241; y += 32, ++row) {
          if (row < this->visibleRows && static_cast<unsigned>(this->scroll + row) < this->skills.size()) {
            this->upgradeButtons[row]->Move(53, y);
          } else {
            this->upgradeButtons[row]->Move(-200, y);
          }
        }
        this->UpdateScrollBar();
        this->UpdateRowButtons();
      }
      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_USESKILL: {
    const CSkillInfo* skill = reinterpret_cast<const CSkillInfo*>(a);
    if (skill->level != 0) {
      CMode* mode = g_modeMgr.gameMode;
      int level = 0;
      if (this->skillTab == 0) {
        level = g_session.HomunSkillLevel(skill->id);
      } else if (this->skillTab == 1) {
        level = g_session.MercSkillLevel(skill->id);
      }
      mode->SendMsg(MM_USING_SKILL, a, level, 0, 0);
      return 0;
    }
    break;
  }
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE:
      if (this->skillTab == 0) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_0);
        return 0;
      }
      if (this->skillTab == 1) {
        g_windowMgr.CloseWindow(WID_SKILLLISTWND_SKILLTAB_1);
        return 0;
      }
      break;
    case ID_OK:
      if (this->hovered != -1) {
        CSkillInfo skill = this->GetSkillByIndex(this->hovered);
        if (skill.available != 0 && skill.level != 0 && skill.spCost != 0) {
          this->SendMsg(nullptr, UM_USESKILL, reinterpret_cast<std::intptr_t>(&skill), 0, 0, 0);
        }
        return 0;
      }
      break;
    case ID_LVLUP1:
    case ID_LVLUP2:
    case ID_LVLUP3:
    case ID_LVLUP4:
    case ID_LVLUP5:
    case ID_LVLUP6:
    case ID_LVLUP7: {
      if (this->skillTab == 0) {
        if (g_session.m_homunSkillPoint == 0) {
          return 0;
        }
      } else if (this->skillTab == 1) {
        if (g_session.m_mercenarySkillPoint == 0) {
          return 0;
        }
      }
      const int index = this->scroll - 0xEF + a;
      if (g_skillTooltipActive == 0) {
        CSkillInfo skill = this->GetSkillByIndex(index);
        g_skillUpgradeId = skill.id;
        if (skill.available != 0) {
          g_modeMgr.gameMode->SendMsg(MM_UPGRADE_SKILLLEVEL, skill.id, 0, 0, 0);
        }
      } else {
        CSkillInfo skill = this->GetSkillByIndex(index);
        if (g_skillUpgradeId == static_cast<int>(skill.id) && skill.available != 0) {
          g_modeMgr.gameMode->SendMsg(MM_UPGRADE_SKILLLEVEL, skill.id, 0, 0, 0);
        }
      }
      return 0;
    }
    case ID_LVLSELECTDN1:
    case ID_LVLSELECTDN2:
    case ID_LVLSELECTDN3:
    case ID_LVLSELECTDN4:
    case ID_LVLSELECTDN5:
    case ID_LVLSELECTDN6:
    case ID_LVLSELECTDN7: {
      CSkillInfo skill = this->GetSkillByIndex(this->scroll - 0xFD + a);
      if (skill.available != 0) {
        const int id = skill.id;
        int current;
        if (this->skillTab == 0) {
          current = g_session.HomunSkillLevel(id);
        } else if (this->skillTab == 1) {
          current = g_session.MercSkillLevel(id);
        }
        int level = current - 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        if (this->skillTab == 0) {
          g_session.SetOption(8, 1, 0);
          g_session.SetHomunSkillLevel(id, level);
        } else if (this->skillTab == 1) {
          g_session.SetMercSkillLevel(id, level);
        }
        this->Invalidate();
      }
      return 0;
    }
    case ID_LVLSELECTUP1:
    case ID_LVLSELECTUP2:
    case ID_LVLSELECTUP3:
    case ID_LVLSELECTUP4:
    case ID_LVLSELECTUP5:
    case ID_LVLSELECTUP6:
    case ID_LVLSELECTUP7: {
      CSkillInfo skill = this->GetSkillByIndex(this->scroll - 0x104 + a);
      if (skill.available != 0) {
        const int id = skill.id;
        int current;
        if (this->skillTab == 0) {
          current = g_session.HomunSkillLevel(id);
        } else if (this->skillTab == 1) {
          current = g_session.MercSkillLevel(id);
        }
        int level = current + 1;
        if (level <= 0) {
          level = 1;
        }
        if (level > skill.level) {
          level = skill.level;
        }
        if (this->skillTab == 0) {
          g_session.SetOption(8, 1, 0);
          g_session.SetHomunSkillLevel(id, level);
        } else if (this->skillTab == 1) {
          g_session.SetMercSkillLevel(id, level);
        }
        this->Invalidate();
      }
      return 0;
    }
    }
    break;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scroll;
    int position;
    if (message == UM_SCROLLVERTPAGEUP) {
      position = previous - this->visibleRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      position = this->visibleRows - 1 + previous;
    } else {
      position = a + previous;
    }
    this->scroll = position;
    if (previous != position) {
      this->UpdateScrollBar();
      this->UpdateRowButtons();
      this->Invalidate();
    }
    return this->scroll - previous;
  }
  case UM_REFRESHITEM: {
    this->skills.clear();
    int count = 0;
    if (this->skillTab == 0) {
      count = static_cast<int>(g_session.m_homunSkills.size());
    } else if (this->skillTab == 1) {
      count = static_cast<int>(g_session.m_mercSkills.size());
    }

    char upgradable = 0;
    for (int index = 0; index < count; ++index) {
      CSkillInfo skill;
      if (this->skillTab == 0) {
        skill = g_session.HomunSkillAt(index);
      } else if (this->skillTab == 1) {
        skill = g_session.MercSkillAt(index);
      }

      if (skill.available != 0) {
        if (skill.upgradable != 0 || skill.level != 0) {
          this->skills.push_back(skill);
          ++upgradable;
        }
        if (CSkillInfo::UsesLevel(*g_session.m_hotkeyScript, skill.id) && skill.level != 0) {
          int current = 0;
          if (this->skillTab == 0) {
            current = g_session.HomunSkillLevel(skill.id);
          } else if (this->skillTab == 1) {
            current = g_session.MercSkillLevel(skill.id);
          }
          if (current <= 0 || current > skill.level) {
            if (this->skillTab == 0) {
              g_session.SetHomunSkillLevel(skill.id, skill.level);
            } else if (this->skillTab == 1) {
              g_session.SetMercSkillLevel(skill.id, skill.level);
            }
          }
        }
      }
    }

    if (upgradable != g_skillListCount) {
      if (g_skillTooltipTick != 0) {
        g_skillTooltipActive = 1;
      }
      g_skillListCount = upgradable;
      g_skillTooltipTick = timeGetTime();
    }
    this->Invalidate();
    this->UpdateScrollBar();
    this->UpdateRowButtons();
    return 0;
  }
  case UM_MAKEMENU:
    this->contextMenuEntries.clear();
    if (g_windowMgr.menuWnd) {
      g_windowMgr.CloseWindow(WID_MENUWND);
    }
    if (this->hovered != -1) {
      CSkillInfo skill = this->GetSkillByIndex(this->hovered);
      if (skill.available != 0) {
        this->contextMenuEntries.push_back(MiniPartyInfoEntry(std::string(LookupMessage(MSI_SEE_INFORMATION_OF_SKILL)).c_str(), 3));
      }
    }
    if (this->contextMenuEntries.size()) {
      UIWindow* menu = g_windowMgr.EnsureWindow(WID_MENUWND);
      for (unsigned index = 0; index < this->contextMenuEntries.size(); ++index) {
        menu->SendMsg(nullptr, UM_ADDITEM, reinterpret_cast<std::intptr_t>(this->contextMenuEntries[index].text.c_str()), 0, 0, 0);
      }
      if (this->skillTab == 0) {
        menu->SendMsg(nullptr, UM_SET_CALLER_WID, 0x72, 0, 0, 0);
      } else if (this->skillTab == 1) {
        menu->SendMsg(nullptr, UM_SET_CALLER_WID, 0x7E, 0, 0, 0);
      }
      menu->SendMsg(nullptr, UM_ADJUSTSIZE, 0, 0, 0, 0);
      menu->Move(
        std::min(std::max(g_mouse.x + 1, 1), g_renderer->viewport.width - menu->geometry.width),
        std::min(std::max(g_mouse.y + 1, 1), g_renderer->viewport.height - menu->geometry.height)
      );
      return 0;
    }
    break;
  case UM_MENU_SELECTED:
    if (this->hovered != -1) {
      CSkillInfo skill = this->GetSkillByIndex(this->hovered);
      if (skill.available != 0) {
        const int kind = this->contextMenuEntries[a].value;
        if (kind == 3) {
          g_windowMgr.EnsureWindow(WID_SKILLDESCRIBEWND)->SendMsg(nullptr, UM_SKILLDESCRIBE, skill.id, 0, 0, 0);
        } else if (kind == 0xE) {
          g_windowMgr.EnsureWindow(WID_SKILLNAMECHANGEWND)->SendMsg(nullptr, UM_SET_SKILL_INFO, skill.id, 0, 0, 0);
        }
      }
      return 0;
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
  return 0;
}

// OFFSET: 0x0097D300
// FUNCTION: fn_13326
void UISkillListWnd::OnCreate(int width, int height) {
  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(3);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_use", "btn_use_a", "btn_use_b"}, {"btn_close", "btn_close_a", "btn_close_b"}};
  int positions[2][2] = {{this->geometry.width - 107, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
  int ids[2] = {184, 201};
  for (int index = 0; index < 2; ++index) {
    UIBitmapButton* button = new UIBitmapButton();
    this->buttons[index] = button;
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;
  this->AddBaseButton();
  this->AddCloseButton("Alt+S", 0);

  {
    int upgradeIds[7] = {239, 240, 241, 242, 243, 244, 245};
    std::string normal("유저인터페이스\\basic_interface\\skill_up_a.bmp");
    std::string over("유저인터페이스\\basic_interface\\skill_up_b.bmp");
    std::string pressed("유저인터페이스\\basic_interface\\skill_up_c.bmp");
    int row = 0;
    for (int y = 21; y < 245; y += 32, ++row) {
      UIBitmapButton* button = new UIBitmapButton();
      this->upgradeButtons[row] = button;
      button->SetBitmap(normal.c_str(), 0, 0);
      button->SetBitmap(over.c_str(), 1, 0);
      button->SetBitmap(pressed.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(78, y);
      button->SetId(upgradeIds[row]);
      this->AddChild(button);
    }
  }

  {
    int levelDownIds[7] = {253, 254, 255, 256, 257, 258, 259};
    std::string left("유저인터페이스\\basic_interface\\arw_left.bmp");
    int row = 0;
    for (int y = 34; y < 258; y += 32, ++row) {
      UIBitmapButton* button = new UIBitmapButton();
      this->levelButtons[row] = button;
      button->SetBitmap(left.c_str(), 0, 0);
      button->SetBitmap(left.c_str(), 1, 0);
      button->SetBitmap(left.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(112, y);
      button->SetId(levelDownIds[row]);
      this->AddChild(button);
    }
  }

  {
    int levelUpIds[7] = {260, 261, 262, 263, 264, 265, 266};
    std::string right("유저인터페이스\\basic_interface\\arw_right.bmp");
    int row = 0;
    for (int y = 34; y < 258; y += 32, ++row) {
      UIBitmapButton* button = new UIBitmapButton();
      this->levelLabels[row] = button;
      button->SetBitmap(right.c_str(), 0, 0);
      button->SetBitmap(right.c_str(), 1, 0);
      button->SetBitmap(right.c_str(), 2, 0);
      button->SetSize(button->BitmapWidth(), button->BitmapHeight());
      button->Move(192, y);
      button->SetId(levelUpIds[row]);
      this->AddChild(button);
    }
  }

  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x0098A3A0
// FUNCTION: fn_13413
void UISkillListWnd::OnWheel(int value) {
  if (this->hasVerticalOverflow) {
    const int step = std::min(this->visibleRows - 1, 3);
    if (value < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0098A6F0
// FUNCTION: fn_13416
void UISkillListWnd::UpdateScrollBar() {
  UIScrollBar* bar = static_cast<UIScrollBar*>(this->scrollBar);
  if (!bar) {
    return;
  }

  const int visible = this->visibleRows;
  const int rows = (this->skills.size() % this->columns != 0) + this->skills.size() / this->columns;
  int overflow = rows - visible;
  if (overflow < 0) {
    overflow = 0;
  }

  this->hasVerticalOverflow = overflow != 0;
  if (overflow != 0) {
    bar->SetRange(overflow, visible, rows);
    int position = this->scroll;
    if (position < 0) {
      this->scroll = 0;
      position = 0;
    }

    const int last = rows - this->visibleRows;
    if (position > last) {
      this->scroll = last;
      position = last;
    }

    static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(position);
  } else {
    this->scroll = 0;
    bar->SetRange(0, visible, rows);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x0098A3F0
// FUNCTION: fn_13414
void UIMerchantShopMakeWnd::UpdateLayout() {
  if (!this->scrollBar) {
    return;
  }

  const int top = this->initialHeight - this->minimumRows * 42;
  int rows = (g_renderer->viewport.height - top) / 42;
  this->maxVisibleRows = rows;
  if (rows > this->slotCount) {
    this->maxVisibleRows = this->slotCount;
    rows = this->slotCount;
  }

  const int count = static_cast<int>(this->items.size());
  if (count >= this->minimumRows) {
    UIScrollBar* bar = static_cast<UIScrollBar*>(this->scrollBar);
    if (count > rows) {
      this->scrollable = 1;
      bar->SetRange(count - rows, rows, count);
      int position = this->scroll;
      if (position < 0) {
        this->scroll = 0;
        position = 0;
      }

      const int last = count - this->maxVisibleRows;
      if (position > last) {
        this->scroll = last;
        position = last;
      }

      static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(position);
    } else {
      this->scrollable = 0;
      this->scroll = 0;
      bar->SetRange(0, rows, count);
      rows = count;
    }

    this->Resize(this->geometry.width, rows * 42 + top);
    if (this->mode == 0) {
      this->notice->Move(12, this->geometry.height - 20);
    }

    for (int index = 0; index < 2; ++index) {
      this->actionButtons[index]->Move(this->actionButtons[index]->geometry.x, this->geometry.height - 24);
    }

    if (this->actionButton) {
      this->actionButton->Move(this->actionButton->geometry.x, this->geometry.height - 24);
    }

    this->scrollBar->Resize(1, this->geometry.height - 58);
  }

  this->visibleRows = std::max((this->geometry.height - top) / 42, this->minimumRows);
  this->scrollBar->Invalidate();
}

// OFFSET: 0x0098A2B0
// FUNCTION: fn_13410
void UIItemStoreFindWnd::OnWheel(int value) {
  if (this->scrollOverflow) {
    const int step = std::min(this->visibleRows - 1, 3);
    if (value < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0098A300
// FUNCTION: fn_13411
void UIItemStoreSubWnd::OnWheel(int value) {
  if (this->scrollOverflow) {
    const int step = std::min(this->visibleRows - 1, 3);
    if (value < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0098A350
// FUNCTION: fn_13412
void UIMerchantShopMakeWnd::OnWheel(int value) {
  if (this->scrollable) {
    const int step = std::min(this->minimumRows - 1, 3);
    if (value < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, step, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -step, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0098A5A0
// FUNCTION: fn_13415
void UIItemBaseWnd::UpdateScrollBar() {
  UIScrollBar* bar = static_cast<UIScrollBar*>(this->scrollBar);
  if (!bar) {
    return;
  }

  const int visible = this->visibleRows;
  const int rows = (this->items.size() % this->columns != 0) + this->items.size() / this->columns;
  int overflow = rows - visible;
  if (overflow < 0) {
    overflow = 0;
  }

  this->scrollOverflow = overflow != 0;
  if (overflow != 0) {
    bar->SetRange(overflow, visible, rows);
    int position = this->scroll;
    if (position < 0) {
      this->scroll = 0;
      position = 0;
    }

    const int last = rows - this->visibleRows;
    if (position > last) {
      this->scroll = last;
      position = last;
    }

    static_cast<UIScrollBar*>(this->scrollBar)->SetPosition(position);
  } else {
    this->scroll = 0;
    bar->SetRange(0, visible, rows);
  }

  this->scrollBar->Invalidate();
}

// OFFSET: 0x0098A920
// FUNCTION: fn_13418
int UIItemBaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 240 ? c : 240;
      if (width < 232) {
        width = 232;
      }
      if (height < 102) {
        height = 102;
      }

      const int rowsHeight = (height - 38) / 32 * 32;
      const int newHeight = rowsHeight + 38;
      const int columnsWidth = (width - 280) / 32 * 32;
      const int newWidth = columnsWidth + 280;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = (newWidth - 40) / 32;
        this->visibleRows = rowsHeight / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 17);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->scroll = previous - this->visibleRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->scroll = this->visibleRows - 1 + previous;
    } else {
      this->scroll = a + previous;
    }

    if (previous != this->scroll) {
      this->UpdateScrollBar();
      this->Invalidate();
    }

    return this->scroll - previous;
  }
  case UM_PAGEDOWNPUSH:
    if (this->scrollOverflow == 1) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, this->visibleRows, 0, 0, 0);
      return 0;
    }
    break;
  case UM_PAGEUPPUSH:
    if (this->scrollOverflow == 1) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -this->visibleRows, 0, 0, 0);
    }
    break;
  case UM_COMMAND:
  case UM_REFRESHITEM:
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00991880
// FUNCTION: fn_13431
int UIMerchantItemWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantItemWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    if (a == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_MERCHANTITEMWND);
      return 0;
    }
    break;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->drag.source == 1) {
      mode->SendMsg(MM_MOVE_ITEM_FROM_BODY_TO_CART, mode->drag.id, mode->drag.quantity, 0, 0);
    } else if (mode->drag.source == 6) {
      mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_CART, mode->drag.id, mode->drag.quantity, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 240 ? c : 240;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = (newWidth - 40) / 32;
        this->visibleRows = (newHeight - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      this->info->Move(16, this->geometry.height - 19);
      this->closeButton->Move(this->geometry.width - 14, 3);
      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList6Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1740At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_SETCOUNTINFO: {
    char text[128];
    std::sprintf(text, "Num: %d/%d Weight: %d/%d", g_session.cartCount, g_session.cartMaxCount, g_session.cartWeight, g_session.cartMaxWeight);
    this->info->SetText(text, 0);
    this->Invalidate();
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00991C40
// FUNCTION: fn_13432
int UIMerchantMirrorItemWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    ITEM_INFO item;
    item.Assign(drag);
    CSession* lists = &g_session;

    switch (drag.source) {
    case 10:
      if (g_session.GetItem1768Quantity(drag.id)) {
        lists->AddMerchantMyShopItem(item, 0, 1);
        lists->RemoveMerchantPurchaseSavedItem(item, 0);
      }
      break;
    case 19:
      if (lists->AddMerchantMyShopItem(item, this->mode, 1)) {
        lists->RemoveMerchantPurchaseSavedItem(item, this->mode != 0);
      }
      break;
    case 21:
      lists->AddMerchantMyShopItem(item, this->mode, 1);
      lists->RemoveMerchantHopeToSellItem(item);
      if (g_windowMgr.buyingShopPurchaseWnd) {
        static_cast<UIWindow*>(g_windowMgr.buyingShopPurchaseWnd)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      break;
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantMirrorItemWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 240 ? c : 240;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = (newWidth - 40) / 32;
        this->visibleRows = (newHeight - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList8Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1750At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 21400) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 21401) {
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          break;
        }

        offset += header.size;
      }
    }
    break;
  }
  case UM_COMMAND:
    break;
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0098C5A0
// FUNCTION: fn_13421
int UIItemSellWnd2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemSellWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    ITEM_INFO item;
    item.Assign(drag);
    if (g_session.HasShopItemIdentity(drag.id)) {
      CSession* lists = &g_session;
      lists->AddStorageItem(item);
      lists->ReduceShopItemQuantity(item);
      if (g_windowMgr.itemShopWnd2) {
        g_windowMgr.itemShopWnd2->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    const int count = g_session.GetList4Size();
    CMode* mode = g_modeMgr.gameMode;
    if (a == ID_OK) {
      if (count > 0) {
        const int result = g_windowMgr.Confirm(LookupMessage(MSI_MIX_ACCEPT), 2, true, false, 0, "Message", 280, 120, 0);
        if (result == ID_YES) {
          mode->SendMsg(MM_ITEMLISTWIN_RES, this->shopId, 0, 0, 0);
        } else {
          mode->SendMsg(MM_CANCEL_ITEMLISTWIN, this->shopId, 0, 0, 0);
        }
        g_windowMgr.CloseWindow(WID_ITEMSHOPWND2);
        g_windowMgr.CloseWindow(WID_ITEMSELLWND2);
        g_session.ClearTradeItems();
        return 0;
      }
    } else if (a == ID_CANCEL) {
      mode->SendMsg(MM_CANCEL_ITEMLISTWIN, this->shopId, 0, 0, 0);
      g_windowMgr.CloseWindow(WID_ITEMSHOPWND2);
      g_windowMgr.CloseWindow(WID_ITEMSELLWND2);
      g_session.ClearTradeItems();
      return 0;
    }
    break;
  }
  case UM_RESIZE:
    if (a == 3) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      const int positions[2][2] = {
        {this->geometry.width - 106, this->geometry.height - 24},
        {this->geometry.width - 61, this->geometry.height - 24},
      };
      for (int i = 0; i < 2; ++i) {
        this->buttons[i]->Move(positions[i][0], positions[i][1]);
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    if (a) {
      this->shopId = a;
    }

    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList4Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1730At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20800) {
      UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND2);
      if (window) {
        window->SendMsg(nullptr, UM_SETTITLE, 1, 0, 0, 0);
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        switch (header.type) {
        case 20801:
          return 0;
        case 20802:
          std::memcpy(&this->shopId, bytes + offset, header.size);
          offset += header.size;
          window->SendMsg(nullptr, UM_REFRESHITEM, this->shopId, 0, 0, 0);
          break;
        default:
          offset += header.size;
          break;
        }
      }
    }
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00990880
// FUNCTION: fn_13429
int UIMerchantItemPurchaseWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantItemPurchaseWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_SETNAID:
    this->storeId = b;
    this->merchantId = a;
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    ITEM_INFO item;
    item.Assign(drag);

    ITEM_INFO found;
    const int ownedCount = g_session.GetList10Size();
    int owned = 0;
    for (int i = 0; i < ownedCount; ++i) {
      found = g_session.GetItem1760At(i);
      if (found.identity[1] == item.identity[1]) {
        owned = found.quantity;
        break;
      }
    }

    ITEM_INFO stock;
    const int stockCount = g_session.GetList7Size();
    int limit = 0;
    for (int i = 0; i < stockCount; ++i) {
      stock = g_session.GetItem1748At(i);
      bool same;
      if (this->buyingStore) {
        same = std::atoi(stock.itemId.c_str()) == std::atoi(item.itemId.c_str());
      } else {
        same = stock.identity[1] == item.identity[1];
      }
      if (same) {
        limit = stock.quantity;
        break;
      }
    }

    char text[256];
    int total = owned + item.quantity;
    if (limit < total) {
      total = limit;
      item.quantity = limit - owned;
      std::sprintf(text, LookupMessage(this->buyingStore ? MSI_BUYINGSTORE_SELLWND_LIMITNUM : MSI_CAN_BUY_ITEM_NUM), limit);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 255, 0, 0, 0);
    }
    if (total > 30000) {
      item.quantity = 30000 - owned;
      std::sprintf(text, LookupMessage((this->buyingStore != 0) + 1704), 30000);
      g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 255, 0, 0, 0);
    }

    if (item.quantity > 0) {
      const long long pending = g_session.GetItem1760Total();
      const int price = static_cast<int>(drag.price);
      if (this->buyingStore) {
        if (static_cast<long long>(price * item.quantity) + g_session.m_zeny + pending > 0x7FFFFFFF) {
          g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_OVER_OR_LOW_MONEY)), 255, 0, 0, 0);
        } else {
          g_session.AddMerchantHopeToSellItem(item);
          g_session.RemoveMerchantMyShopItem(item, 2, 1);
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          if (g_windowMgr.purchaseMirrorItemWnd) {
            g_windowMgr.purchaseMirrorItemWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }
        }
      } else if (g_session.m_zeny >= static_cast<long long>(price) * item.quantity + pending) {
        g_session.AddMerchantHopeToSellItem(item);
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      } else {
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_INSUFFICIENT_MONEY)), 255, 0, 0, 0);
      }
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == ID_OK) {
      if (this->buyingStore) {
        int sum = 0;
        for (std::list<ITEM_INFO>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
          if (it->identity[1] != 0) {
            sum += static_cast<int>(it->prices[1]) * it->quantity;
          }
        }
        if (sum > static_cast<UIMerchantItemShopWnd*>(g_windowMgr.buyingShopItemsWnd)->buyingZeny) {
          g_windowMgr.Confirm(LookupMessage(MSI_BUYINGSTORE_SELLWND_LACKBUYERZENY), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
        mode->SendMsg(MM_REQ_TRADE_BUYING_STORE, this->merchantId, this->storeId, 0, 0);
      } else {
        static_cast<UIMerchantItemPurchaseMSGWnd*>(g_windowMgr.EnsureWindow(WID_MERCHANTITEMPURCHASEMSGWND))->SetPurchase(this->merchantId, this->storeId);
      }

      mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      if (this->buyingStore) {
        g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND_MODE_1);
        g_windowMgr.CloseWindow(WID_MERCHANTMIRRORITEMWND_MODE_2);
      } else {
        g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND);
        g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEWND);
      }
      return 0;
    }
    if (a == ID_CANCEL) {
      int first;
      int second;
      if (this->buyingStore) {
        g_windowMgr.CloseWindow(WID_MERCHANTITEMSHOPWND_MODE_1);
        second = 179;
        first = 178;
      } else {
        second = 44;
        first = 43;
      }
      g_windowMgr.CloseWindow(first);
      g_windowMgr.CloseWindow(second);
      mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
      g_session.ClearItems1760And1748();
    }
    return 0;
  }
  case UM_RESIZE:
    if (a == 3) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->visibleRows = (newHeight - 38) / 32;
      }

      this->total->Move(12, this->geometry.height - 20);
      int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
      for (int i = 0; i < 2; ++i) {
        this->buttons[i]->Move(positions[i][0], positions[i][1]);
      }
      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    int sum = 0;
    const int count = g_session.GetList10Size();
    for (int i = 0; i < count; ++i) {
      ITEM_INFO item = g_session.GetItem1760At(i);
      if (item.identity[1] != 0) {
        sum += item.quantity * static_cast<int>(item.prices[1]);
        this->items.push_back(item);
      }
    }

    char grouped[64];
    char text[80];
    std::sprintf(text, "Total : %s Zeny", FormatGroupedInteger(sum, grouped, 64));
    this->total->SetTextAutoWidth(text);
    this->visibleRows = std::max(this->items.size(), 4u);
    this->Resize(this->geometry.width, this->visibleRows * 32 + this->nonItemHeight);
    for (int i = 0; i < 2; ++i) {
      this->buttons[i]->Move(this->buttons[i]->geometry.x, this->geometry.height - 24);
    }
    this->total->Move(12, this->geometry.height - 21);
    this->Invalidate();
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 21150 || header.type == 21100) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        switch (header.type) {
        case 21101:
          this->SendMsg(nullptr, UM_SETNAID, 0, 0, 0, 0);
          [[fallthrough]];
        case 21151:
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          return 0;
        default:
          offset += header.size;
          break;
        }
      }
    }
    return 0;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00991370
// FUNCTION: fn_13430
int UIMerchantItemShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantItemShopWndInfo*>(a);
    if (!this->saved->IsValid(this->buyingShop != 0)) {
      this->saved->Reset(this->buyingShop != 0);
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_SETNAID:
    this->ownerAid = a;
    return 0;
  case UM_SETTITLE:
    this->titleText->SetTextAutoWidth(LookupMessage(MSI_SHOPITEMS));
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (this->buyingShop == 0) {
      ITEM_INFO item;
      item.Assign(mode->drag);
      g_session.RemoveMerchantHopeToSellItem(item);
      if (g_windowMgr.merchantItemPurchaseWnd) {
        static_cast<UIWindow*>(g_windowMgr.merchantItemPurchaseWnd)->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_RESIZE:
    if (a == 3) {
      int width;
      if (this->buyingShop) {
        width = b <= 426 ? b : 426;
      } else {
        width = b <= 320 ? b : 320;
      }
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->visibleRows = static_cast<unsigned>(newHeight - this->footerHeight - 17) / 32;
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList7Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1748At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->visibleRows = std::max(this->items.size(), 4u);
    this->Resize(this->geometry.width, this->visibleRows * 32 + this->nonItemHeight);
    this->Invalidate();
    return 0;
  }
  case UM_SET_BUYING_SHOP_ZENY: {
    this->buyingZeny = a;
    char grouped[80];
    char text[80];
    std::sprintf(text, LookupMessage(MSI_BUYINGSTORE_MYSHOPWND_LIMITZENY), FormatGroupedInteger(static_cast<int>(a), grouped, 80));
    this->notice->SetTextAutoWidth(text);
    this->notice->Move(12, this->geometry.height - 21);
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (21200 == header.type || 21250 == header.type) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        switch (header.type) {
        case 21200:
        case 21250:
          return 0;
        case 21253:
          std::memcpy(&this->buyingZeny, bytes + offset, header.size);
          offset += header.size;
          break;
        default:
          offset += header.size;
          break;
        }
      }
    }
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0097AA00
// FUNCTION: fn_13320
void UIMerchantItemPurchaseWnd::OnCreate(int width, int height) {
  this->total = new UIStaticText();
  this->total->SetSize(160, 15);
  char text[80];
  std::sprintf(text, "Total : %d Zeny", 0);
  this->total->SetTextAutoWidth(text);
  this->total->Move(12, this->geometry.height - 21);
  this->AddChild(this->total);

  UIStaticText* title = new UIStaticText();
  title->SetSize(160, 15);
  title->SetText(LookupMessage(this->buyingStore ? MSI_ITEMS_FOR_SELL : MSI_ITEMS_FOR_BUY_FROM_MERCHANT), 0);
  title->Move(16, 2);
  title->shadow = 1;
  this->AddChild(title);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
  int ids[2] = {184, 185};
  if (this->buyingStore) {
    const char* names[2][3] = {{"btn_sell", "btn_sell_a", "btn_sell_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    for (int index = 0; index < 2; ++index) {
      this->buttons[index] = new UIBitmapButton();
      UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
      this->buttons[index]->Move(positions[index][0], positions[index][1]);
      this->buttons[index]->SetId(ids[index]);
      this->AddChild(this->buttons[index]);
    }
  } else {
    const char* names[2][3] = {{"btn_buy", "btn_buy_a", "btn_buy_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
    for (int index = 0; index < 2; ++index) {
      this->buttons[index] = new UIBitmapButton();
      UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
      button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
      button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
      button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
      this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
      this->buttons[index]->Move(positions[index][0], positions[index][1]);
      this->buttons[index]->SetId(ids[index]);
      this->AddChild(this->buttons[index]);
    }
  }

  this->visibleRows = (height - 38) / 32;
  this->columns = 1;
  this->nonItemHeight = this->geometry.height - this->visibleRows * 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00979920
// FUNCTION: fn_13318
void UIMerchantItemMyShopWnd::OnCreate(int width, int height) {
  if (this->buyingShop) {
    this->notice = new UIStaticText();
    this->notice->SetSize(160, 15);
    char text[80];
    std::sprintf(text, LookupMessage(MSI_BUYINGSTORE_MYSHOPWND_LIMITZENY), 0);
    this->notice->SetTextAutoWidth(text);
    this->notice->Move(12, this->geometry.height - 26);
    this->AddChild(this->notice);
  }

  this->title = new UIStaticText();
  this->title->SetSize(240, 15);
  this->title->SetText(LookupMessage(this->buyingShop ? MSI_BUYINGSTORE_MYSHOPWND_TITLE : MSI_CONNECT_SNS_SUCCESS_SEND), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - this->footerHeight - 17);
  this->AddChild(this->scrollBar);

  this->saleAlarm = new UICheckBox();
  this->saleAlarm->SetSize(240, 12);
  this->saleAlarm->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(this->buyingShop ? MSI_RODEX_FOWARD : MSI_DELETE_DUE_DATE));
  this->saleAlarm->FitToText();
  this->saleAlarm->Move(12, this->buyingShop ? this->geometry.height - 12 : this->geometry.height - 18);
  this->saleAlarm->SetChecked(g_session.m_muteSellSound);
  this->AddChild(this->saleAlarm);

  const std::string prefix("유저인터페이스\\");
  const char* names[1][3] = {{"btn_close", "btn_close_a", "btn_close_b"}};
  int positions[1][2] = {{this->geometry.width - 61, this->geometry.height - 24}};
  int ids[1] = {201};
  for (int index = 0; index < 1; ++index) {
    this->buttons[index] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->buttons[index]->Move(positions[index][0], positions[index][1]);
    this->buttons[index]->SetId(ids[index]);
    this->AddChild(this->buttons[index]);
  }

  this->columns = (width - 40) / 32;
  this->visibleRows = static_cast<unsigned>(height - this->footerHeight - 17) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x00993A20
// FUNCTION: fn_13434
int UINoticeMessageBox_LockSellItem::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    UIFrameWnd::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  if (a == ID_CLOSE && this->parent) {
    this->parent->SendMsg(this, UM_COMMAND, ID_CLOSE, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x00898650
// FUNCTION: fn_11549
int UIEditCtrl_Currency::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == WM_KEYDOWN) {
    if (a == VK_BACK) {
      int anchor;
      int cursor;
      g_serverLanguage->Selection(anchor, cursor);

      if (anchor == cursor) {
        if (cursor <= 2) {
          if (this->text[1] == ',') {
            g_serverLanguage->Select(0, 2);
          }
        } else if (this->text[cursor - 1] == ',') {
          g_serverLanguage->Select(cursor - 1, cursor - 1);
        }
      }
    } else if (a == VK_DELETE) {
      int anchor;
      int cursor;
      g_serverLanguage->Selection(anchor, cursor);

      if (anchor == cursor) {
        if (cursor == 0) {
          if (this->text[1] == ',') {
            g_serverLanguage->Select(0, 2);
          }
        } else if (this->text[cursor] == ',') {
          g_serverLanguage->Select(cursor + 1, cursor + 1);
        }
      }
    }
  }

  return UIEditCtrl2::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009845A0
// FUNCTION: fn_13355
void UIItemPurchaseWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1720ByItemId(item.itemId.c_str());
    mode->BeginDragItem(4, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984700
// FUNCTION: fn_13356
void UIItemSellWnd2::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.identity[1] > 0) {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindStorageItem(item.identity[1]);
    mode->BeginDragItem(5, item, item.quantity, -1);
  }
}

// OFFSET: 0x009848E0
// FUNCTION: fn_13357
void UIItemSellWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.identity[1] > 0) {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindStorageItem(item.identity[1]);
    mode->BeginDragItem(5, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984AC0
// FUNCTION: fn_13358
void UIItemShopWnd2::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.identity[1] > 0) {
      g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND)->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, item.identity[1]);
    if (owned.identity[1] > 0) {
      mode->drag.BeginItem(g_windowMgr, 3, item, item.quantity, -1);
    } else {
      mode->drag.BeginItem(g_windowMgr, 3, item, 1, -1);
    }
  }
}

// OFFSET: 0x00984CA0
// FUNCTION: fn_13359
void UIItemShopWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, item.identity[1]);
    if (owned.identity[1] > 0) {
      mode->BeginDragItem(3, item, item.quantity, -1);
    } else {
      mode->BeginDragItem(3, item, 1, -1);
    }
  }
}

// OFFSET: 0x00984DF0
// FUNCTION: fn_13360
void UIItemStoreFindWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0 && item.flags[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindCartItem(item.identity[1]);
    mode->BeginDragItem(6, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984F10
// FUNCTION: fn_13361
void UIItemStoreSubWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0 && item.flags[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindCartItem(item.identity[1]);
    mode->BeginDragItem(6, item, item.quantity, -1);
  }
}

// OFFSET: 0x00985030
// FUNCTION: fn_13362
void UIItemStoreWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0 && item.flags[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindCartItem(item.identity[1]);
    mode->BeginDragItem(6, item, item.quantity, -1);
  }
}

// OFFSET: 0x009853A0
// FUNCTION: fn_13365
void UIMerchantItemPurchaseWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1760ByItemId(item.itemId.c_str());
    if (this->buyingStore != 0) {
      mode->BeginDragItem(21, item, item.quantity, -1);
    } else {
      mode->BeginDragItem(13, item, item.quantity, -1);
    }
  }
}

// OFFSET: 0x00985830
// FUNCTION: fn_13368
void UIMerchantMirrorItemWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindMerchantMyShopItem(item.identity[1]);
    if (this->mode == 1) {
      mode->drag.BeginItem(g_windowMgr, 20, item, item.quantity, -1);
    } else if (this->mode == 2) {
      mode->drag.BeginItem(g_windowMgr, 22, item, item.quantity, -1);
    } else {
      mode->drag.BeginItem(g_windowMgr, 11, item, item.quantity, -1);
    }
  }
}

// OFFSET: 0x00985970
// FUNCTION: fn_13369
void UIMerchantShopMakeWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
    return;
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindMerchantPurchaseSavedItem(item.identity[1]);
    if (this->mode != 0) {
      gameMode->drag.BeginItem(g_windowMgr, 19, item, item.quantity, -1);
    } else {
      gameMode->drag.BeginItem(g_windowMgr, 10, item, item.quantity, -1);
    }
  }
}

// OFFSET: 0x00985B90
// FUNCTION: fn_13371
void UIItemPurchaseWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char currency[12];
    sprintf(currency, "Zeny");
    if (this->cashShopMode) {
      if (g_serviceType == 6) {
        sprintf(currency, "Koin");
      } else if (g_serviceType == 0) {
        if (g_session.m_channelCopId == 1) {
          sprintf(currency, "한코인");
        } else {
          sprintf(currency, LookupMessage(MSI_CASH));
        }
      } else {
        sprintf(currency, "Point");
      }
    }

    char price[64];
    if (item.prices[0] != item.prices[1]) {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %d -> %s %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4), item.prices[0],
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %s %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x00985ED0
// FUNCTION: fn_13372
void UIItemSellWnd2::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char price[64];
    if (item.prices[0] != item.prices[1]) {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %d -> %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4), item.prices[0],
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x00986190
// FUNCTION: fn_13373
void UIItemSellWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] == 0) {
    return;
  }

  int tooltipX = (x - 40) / 32 * 32 + 40;
  int tooltipY = (y - 17) / 32 * 32 + 1;
  this->ToScreen(tooltipX, tooltipY);

  std::string text;
  char price[64];
  if (item.prices[0] != item.prices[1]) {
    if (item.identified == 1 && item.optionCount != 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA12), item.DisplayName(0).c_str(), item.optionCount, item.quantity, LookupMessage(MSI_EA4),
                               item.prices[0], FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), "Zeny"));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %d -> %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4), item.prices[0],
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    }
  } else {
    if (item.identified == 1 && item.optionCount != 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_TAIWAN_CASH_LEMITEMIN), item.DisplayName(0).c_str(), item.optionCount, item.quantity, LookupMessage(MSI_EA4),
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), "Zeny"));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    }
  }

  g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
}

// OFFSET: 0x009865C0
// FUNCTION: fn_13374
void UIItemShopWnd2::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] == 0) {
    return;
  }

  int tooltipX = (x - 40) / 32 * 32 + 40;
  int tooltipY = (y - 17) / 32 * 32 + 1;
  this->ToScreen(tooltipX, tooltipY);

  std::string text;
  char price[64];
  if (this->interactionDisabled != 0) {
    if (item.prices[0] != item.prices[1]) {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %d -> %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4), item.prices[0],
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    }
  } else {
    if (item.prices[0] != item.prices[1]) {
      text = static_cast<const std::string&>(FormatOwnedString("%s %d -> %s Zeny", item.DisplayName(0).c_str(), item.prices[0],
                               FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    } else {
      text = static_cast<const std::string&>(FormatOwnedString("%s %s Zeny", item.DisplayName(0).c_str(), FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
    }
  }

  g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
}

// OFFSET: 0x00986970
// FUNCTION: fn_13375
void UIItemShopWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char currency[12];
    sprintf(currency, "Zeny");
    if (this->showFooter) {
      if (g_serviceType == 6) {
        sprintf(currency, "Koin");
      } else if (g_serviceType == 0) {
        if (g_session.m_channelCopId == 1) {
          sprintf(currency, "한코인");
        } else {
          sprintf(currency, LookupMessage(MSI_CASH));
        }
      } else {
        sprintf(currency, "Point");
      }
    }

    char price[64];
    if (this->sellMode) {
      if (item.prices[0] != item.prices[1]) {
        if (item.identified == 1 && item.optionCount != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA12), item.DisplayName(0).c_str(), item.optionCount, item.quantity, LookupMessage(MSI_EA4),
                                   item.prices[0], FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %d -> %s %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4), item.prices[0],
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        }
      } else {
        if (item.identified == 1 && item.optionCount != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_TAIWAN_CASH_LEMITEMIN), item.DisplayName(0).c_str(), item.optionCount, item.quantity, LookupMessage(MSI_EA4),
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("%s %d %s %s %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        }
      }
    } else {
      if (item.prices[0] != item.prices[1]) {
        if (item.identified == 1 && item.optionCount != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DRESSROOM_HEARCOLOR), item.DisplayName(0).c_str(), item.optionCount, item.prices[0],
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("%s %d -> %s %s", item.DisplayName(0).c_str(), item.prices[0],
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        }
      } else {
        if (item.identified == 1 && item.optionCount != 0) {
          text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_DRESSROOM_HEARSTYLE), item.DisplayName(0).c_str(), item.optionCount,
                                   FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64), currency));
        } else {
          text = static_cast<const std::string&>(FormatOwnedString("%s %s %s", item.DisplayName(0).c_str(), FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64),
                                   currency));
        }
      }
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x00987000
// FUNCTION: fn_13376
void UIItemStoreFindWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                        g_session.GetItemNameColor(item), 0, 1);
    } else {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00987200
// FUNCTION: fn_13377
void UIItemStoreSubWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                        g_session.GetItemNameColor(item), 0, 1);
    } else {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00987400
// FUNCTION: fn_13378
void UIItemStoreWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                        g_session.GetItemNameColor(item), 0, 1);
    } else {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00987600
// FUNCTION: fn_13379
void UIItemWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_GATLING), item.DisplayName(0).c_str(), item.optionCount, item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    } else {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString(LookupMessage(MSI_EA2), item.DisplayName(0).c_str(), item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00987850
// FUNCTION: fn_13380
void UIMerchantItemMyShopWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    std::string text;
    char price[64];
    if (this->buyingShop != 0) {
      text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_MYSHOPWND_ITEMINFO), item.DisplayName(0).c_str(), FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64),
                               item.quantity));
    } else {
      if (item.identified == 1 && item.optionCount != 0) {
        text = static_cast<const std::string&>(FormatOwnedString(LookupMessage(MSI_EA11), item.DisplayName(0).c_str(), item.optionCount,
                                 FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
      } else {
        text = static_cast<const std::string&>(FormatOwnedString("%s %s Zeny", item.DisplayName(0).c_str(), FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64)));
      }
    }

    g_modeMgr.gameMode->ShowTooltip(text.c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x00987B90
// FUNCTION: fn_13381
void UIMerchantItemPurchaseWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    char price[64];
    g_modeMgr.gameMode
        ->ShowTooltip(FormatOwnedString("%s %d %s %s Zeny", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4),
                                 FormatGroupedInteger(static_cast<int>(item.prices[1]), price, 64))
                   .c_str(),
               tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x00987D40
// FUNCTION: fn_13382
void UIMerchantItemShopWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX, tooltipY,
                        g_session.GetItemNameColor(item), 0, 1);
    } else {
      CMode* mode = g_modeMgr.gameMode;
      mode->ShowTooltip(item.DisplayName(0).c_str(), tooltipX, tooltipY, g_session.GetItemNameColor(item), 0, 1);
    }
    return;
  }

  item = this->GetItemByIndex(this->GetItemIndexAt(x, y));
  if (item.identity[1] == 0) {
    return;
  }

  int tooltipX = 72;
  int tooltipY = (y - 17) / 32 * 32 + 1;
  this->ToScreen(tooltipX, tooltipY);

  if (item.identified == 1 && item.optionCount != 0) {
    g_modeMgr.gameMode
        ->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAILED_MAIL_LIMIT_CNT), item.DisplayName(0).c_str(), item.optionCount).c_str(), tooltipX - 2, tooltipY + 24,
               g_session.GetItemNameColor(item), 0, 1);
  } else {
    g_modeMgr.gameMode->ShowTooltip(item.DisplayName(0).c_str(), tooltipX - 2, tooltipY + 24, g_session.GetItemNameColor(item), 0, 1);
  }
}

// OFFSET: 0x009880F0
// FUNCTION: fn_13383
void UIMerchantItemWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString(LookupMessage(MSI_FAIL_NEED_EQUIPPED_GUN_GATLING), item.DisplayName(0).c_str(), item.optionCount, item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    } else {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString("%s %d %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4)).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00988340
// FUNCTION: fn_13384
void UIMerchantMirrorItemWnd::OnMouseHover(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);
  if (item.identity[1] > 0) {
    int tooltipX = (x - 40) / 32 * 32 + 40;
    int tooltipY = (y - 17) / 32 * 32 + 1;
    this->ToScreen(tooltipX, tooltipY);

    if (item.identified == 1 && item.optionCount != 0) {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString(LookupMessage(MSI_EA10), item.DisplayName(0).c_str(), item.optionCount, item.quantity).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    } else {
      g_modeMgr.gameMode
          ->ShowTooltip(FormatOwnedString("%s %d %s", item.DisplayName(0).c_str(), item.quantity, LookupMessage(MSI_EA4)).c_str(), tooltipX, tooltipY,
                 g_session.GetItemNameColor(item), 0, 1);
    }
  }
}

// OFFSET: 0x00859200
// FUNCTION: fn_10774
bool IsInputSuggestionAllowed() {
  if (g_windowMgr.IsHoverOnWindow01C8Button() && !g_windowMgr.inputSuggestionsEnabled) {
    return false;
  }

  return true;
}

// OFFSET: 0x00860080
// FUNCTION: fn_10898
bool UIEditCtrl::DecodeItemLink(const std::vector<int>& values, ITEM_INFO& item) {
  if (values.empty()) {
    return false;
  }

  ITEM_DESC description = FindItemDescription(values[2]);
  item.SetName(description.identifiedName);
  std::string name(description.identifiedName);
  LowercaseText(name);
  if (name.find("unknown item") != std::string::npos) {
    return false;
  }

  item.identity[2] = values[0];
  item.SetItemId(values[2]);
  if (values[1] != 0) {
    item.identity[0] = 4;
  }

  if (values.size() > 3) {
    item.refiningLevel = values[3];
  }

  item.sprite = values[4];
  int cardCount = 0;
  item.grade = static_cast<std::int16_t>(values[5]);
  if (values.size() > 6) {
    cardCount = values[6];
    for (int i = 1; i < cardCount + 1; ++i) {
      int index = i - 1;
      item.SetCard(values[6 + i], index);
    }
  }

  if (static_cast<unsigned>(cardCount + 8) < values.size()) {
    item.optionCount = values[cardCount + 8];
    int optionCount = item.optionCount + 1;
    for (int i = 1; i < optionCount; ++i) {
      const int base = cardCount + 9 + (i - 1) * 3;
      ItemOption option;
      option.id = static_cast<std::int16_t>(values[base]);
      option.value = static_cast<std::int16_t>(values[base + 2]);
      option.parameter = static_cast<unsigned char>(values[base + 1]);
      int index = i - 1;
      item.SetOption(option, index);
    }
  }

  item.identified = 1;
  return true;
}

// OFFSET: 0x0096E650
// FUNCTION: fn_13214
UIItemBaseWnd::UIItemBaseWnd()
    : UIFrameWnd(0) {
  this->resizer = nullptr;
  this->scrollBar = nullptr;
  this->scroll = 0;
  this->scrollOverflow = 0;
  this->showQuantity = 1;
  this->alternateButtonBar = 0;
  this->buttons[0] = nullptr;
  this->buttons[1] = nullptr;
  this->buttons[2] = nullptr;
  this->buttons[3] = nullptr;
  this->buttons[4] = nullptr;
  this->scroll = 0;
}

// OFFSET: 0x0096E750
// FUNCTION: fn_13215
UIItemDropCntWnd::UIItemDropCntWnd() : UIFrameWnd(0) {
}

// OFFSET: 0x0096E7B0
// FUNCTION: fn_13216
UIItemPurchaseWnd::UIItemPurchaseWnd() : UIItemBaseWnd() {
  this->hovered = -1;
  this->alternateButtonBar = 1;
  this->cashShopMode = 0;
  this->saved = nullptr;
}

// OFFSET: 0x0096E830
// FUNCTION: fn_13217
UIItemSellWnd2::UIItemSellWnd2() : UIItemBaseWnd() {
  this->hovered = -1;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
  this->shopId = 0;
}

// OFFSET: 0x0096E8B0
// FUNCTION: fn_13218
UIItemSellWnd::UIItemSellWnd() : UIItemBaseWnd() {
  this->hovered = -1;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
}

// OFFSET: 0x0096E930
// FUNCTION: fn_13219
UIItemShopWnd2::UIItemShopWnd2() : UIItemBaseWnd() {
  this->hovered = -1;
  this->showQuantity = 0;
  this->interactionDisabled = 0;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
  this->bottomBarHeight = 29;
  this->extraBottomHeight = 0;
}

// OFFSET: 0x0096E9D0
// FUNCTION: fn_13220
UIItemShopWnd::UIItemShopWnd() : UIItemBaseWnd() {
  this->hovered = -1;
  this->showQuantity = 0;
  this->sellMode = 0;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
  this->bottomPadding = 29;
  this->footerHeight = 0;
  this->showFooter = 0;
}

// OFFSET: 0x0096EA80
// FUNCTION: fn_13221
UIItemStoreFindWnd::UIItemStoreFindWnd() : UIItemBaseWnd() {
  this->saved = nullptr;
  this->scrollBar = nullptr;
  this->scroll = 0;
  this->columns = 1;
  this->visibleRows = 3;
  this->scrollOverflow = 0;
  this->padding_0120 = 18;
}

// OFFSET: 0x0096EB70
// FUNCTION: fn_13222
UIItemStoreSubWnd::UIItemStoreSubWnd() : UIItemBaseWnd() {
  this->saved = nullptr;
  this->scrollBar = nullptr;
  this->scroll = 0;
  this->columns = 1;
  this->visibleRows = 3;
  this->scrollOverflow = 0;
  this->padding_011C = 18;
}

// OFFSET: 0x0096EC60
// FUNCTION: fn_13223
UIItemStoreWnd::UIItemStoreWnd() : UIItemBaseWnd() {
  this->m_sortOptionA = 0;
  this->m_sortOptionB = 0;
  this->m_sortOptionC = 0;
  this->m_sortMode = 0;
  this->showQuantity = 1;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
  this->m_sortOptionD = 0;
  this->m_sortOptionE = 0;
  this->searchEdit = nullptr;
  for (int i = 0; i < 8; ++i) {
    this->m_tabFlags[i] = 0;
    this->tabs[i] = nullptr;
    this->m_tabWindows[i] = 0;
  }
  this->m_searchWindow = 0;
  this->m_itemCount = 0;
  this->m_capacity = 0;
  this->m_hideCapacity = 0;
}

// OFFSET: 0x00977E10
// FUNCTION: fn_13316
void UIItemStoreWnd::OnCreate(int width, int height) {
  this->title = new UIStaticText();
  this->title->SetSize(70, 15);
  this->title->SetText(LookupMessage(g_session.m_buyingStoreMode ? MSI_BARGAINSALE_TOOLWND_DB_NAME : MSI_ITEMS_THAT_IS_STORED), 0);
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(3);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  this->tabBar = new UITab();
  this->tabBar->SetSize(20, 235);
  this->tabBar->Move(0, 18);
  this->tabBar->tabCount = 8;
  this->tabBar->style = 3;
  this->tabBar->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
  this->AddChild(this->tabBar);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 69);
  this->AddChild(this->scrollBar);

  this->searchEdit = new UIBitmapEditCtrl;
  this->searchEdit->SetSize(122, 18);
  this->searchEdit->SetBitmapName("유저인터페이스\\bg_search.bmp");
  this->searchEdit->Move(65, this->geometry.height - 23);
  this->searchEdit->maximum = 20;
  this->AddChild(this->searchEdit);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_close", "btn_close_a", "btn_close_b"}, {"bt_search_normal", "bt_search_over", "bt_search_press"}};
  int positions[2][2] = {{220, this->geometry.height - 23}, {185, this->geometry.height - 23}};
  int ids[2] = {201, 365};
  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->buttons[index]->Move(positions[index][0], positions[index][1]);
    this->buttons[index]->SetId(ids[index]);
    this->AddChild(this->buttons[index]);
  }

  int tabIds[8] = {146, 147, 148, 149, 150, 151, 152, 309};
  const char* tabNames[8][2] = {{"소비-1", "소비-2"},     {"캐시-1", "캐시-2"}, {"방어구-1", "방어구-2"}, {"무기-1", "무기-2"},
                                {"의상-1", "의상-2"},     {"투사체-1", "투사체-2"}, {"카드-1", "카드-2"}, {"기타-1", "기타-2"}};
  for (int index = 0; index < 8; ++index) {
    this->tabs[index] = new UICheckBox();
    this->tabs[index]->SetBitmaps(
      (prefix + "basic_interface\\" + tabNames[index][0] + ".bmp").c_str(), (prefix + "basic_interface\\" + tabNames[index][1] + ".bmp").c_str()
    );
    this->tabs[index]->SetSize(23, 23);
    this->tabs[index]->Move(index * 26 + 5, this->geometry.height - 52);
    this->tabs[index]->SetChecked(0);
    this->tabs[index]->SetId(tabIds[index]);

    int tooltip;
    if (index < 4) {
      tooltip = index + 1465;
    } else if (index == 4) {
      tooltip = 3151;
    } else {
      tooltip = index + 1464;
    }
    this->tabs[index]->SetTooltip(LookupMessage(tooltip));
    this->AddChild(this->tabs[index]);
  }

  this->m_message0 = 365;
  this->m_capacityText = "";
  this->columns = 1;
  this->visibleRows = (height - 74) / 32;
  this->AddCloseButton("", 0);

  this->sortCombo = new UIItemStoreComboBoxWnd();
  this->sortCombo->pink = 0;
  this->sortCombo->SetSize(62, 18);
  this->sortCombo->Move(214, this->geometry.height - 52);
  this->AddChild(this->sortCombo);
  this->sortCombo->AddOption(LookupMessage(MSI_HOMUN_SELF_FEEDING), 0);
  this->sortCombo->AddOption(LookupMessage(MSI_CHEATDEFENDER_FILE_CHECK_FAIL), 0);
  this->sortCombo->AddOption(LookupMessage(MSI_ITEMMOVERESTRICT_WHITELIST), 0);

  const bool sortModes[3] = {g_session.m_itemDefaultSort, g_session.m_itemAscendingOrder, g_session.m_itemDescendingOrder};
  for (int index = 0; index < 3; ++index) {
    if (sortModes[index] == true) {
      this->sortCombo->Select(index);
      this->SendMsg(this->sortCombo, UM_COMBOBOX_SELECTED, index, 0, 0, 0);
      break;
    }

    if (index == 2) {
      this->sortCombo->Select(0);
      this->SendMsg(this->sortCombo, UM_COMBOBOX_SELECTED, 0, 0, 0, 0);
      break;
    }
  }
}

// OFFSET: 0x009792B0
// FUNCTION: fn_13317
void UIItemWnd::OnCreate(int width, int height) {
  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(7);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 17, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  this->baseButton = this->AddBaseButton();
  this->minimizeButton = this->AddMinimizeButton("Ctrl+E", 0);
  this->closeButton = this->AddCloseButton("Alt+E", 0);
  this->expandedHeight = this->geometry.height;
  this->columns = (width - 40) / 32;
  this->visibleRows = (height - 38) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);

  this->tabs = new (std::nothrow) UITabControl();
  this->tabs->Configure(0, 17, 10, 1, 0, 1, 0, 0, 0, 0, 0, 0);
  this->tabs->Move(3, 18);
  this->AddChild(this->tabs);
  const char* labels[4] = {LookupMessage(MSI_STORE_TABNAME_0), LookupMessage(MSI_INVENTORY_TABNAME_1), LookupMessage(MSI_STORE_TABNAME_6), LookupMessage(MSI_INVENTORY_TABNAME_3)};
  for (int index = 0; index < 4; ++index) {
    this->tabs->AddTab(labels[index], "");
  }
  const SkinPaletteCell cell = {5, 2};
  this->tabs->colors[3] = SkinPaletteColor(cell, g_skinInfo.Selected());
  this->tabs->Invalidate();

  this->lockDropBox = new (std::nothrow) UICheckBox();
  this->lockDropBox->SetSize(14, 11);
  this->lockDropBox->SetBitmaps("유저인터페이스\\inventory\\item_drop_lock_on.bmp", "유저인터페이스\\inventory\\item_drop_lock_off.bmp");
  this->lockDropBox->Move(79, height - 16);
  this->lockDropBox->SetChecked(g_session.m_lockItemDrop);
  this->lockDropBox->SetTooltip(LookupMessage(MSI_RODEX_GENERAL));
  this->AddChild(this->lockDropBox);

  UICheckBox* compare = new (std::nothrow) UICheckBox();
  this->optionBox = compare;
  compare->SetSize(12, 12);
  static_cast<UICheckBox*>(this->optionBox)->SetBitmaps("유저인터페이스\\inventory\\item_compare_on.bmp", "유저인터페이스\\inventory\\item_compare_off.bmp");
  this->optionBox->SetId(456);
  this->optionBox->Move(103, height - 17);
  static_cast<UICheckBox*>(this->optionBox)->SetChecked(g_serviceOptions.Get(TT_ITEM_COMPARE_ON_OFF));
  static_cast<UICheckBox*>(this->optionBox)->SetTooltip(LookupMessage(MSI_CONNECT_SNS_WND));
  this->AddChild(this->optionBox);

  this->countButton = new (std::nothrow) UIBitmapButton();
  this->countButton->SetBitmap("유저인터페이스\\inventory\\icon_num.bmp", 0, 0);
  this->countButton->SetBitmap("유저인터페이스\\inventory\\icon_num.bmp", 1, 0);
  this->countButton->SetBitmap("유저인터페이스\\inventory\\icon_num.bmp", 2, 0);
  this->countButton->SetSize(this->countButton->BitmapWidth(), this->countButton->BitmapHeight());
  this->countButton->Move(23, height - 17);
  this->countButton->SetId(545);
  this->countButton->SetTooltip(LookupMessage(MSI_INVENTORY_EXTEND_FAILED_ANOTHER_WORK));
  this->AddChild(this->countButton);

  UIBitmapButton* lockDeal = new UIBitmapButton();
  this->dealLockButton = lockDeal;
  lockDeal->SetBitmap(g_session.m_blockPrivateTab != 1 ? "유저인터페이스\\inventory\\bt_itemDeal_lock_off.bmp" : "유저인터페이스\\inventory\\bt_itemdeal_lock_on.bmp", 0, 0);
  static_cast<UIBitmapButton*>(this->dealLockButton)->SetBitmap("유저인터페이스\\inventory\\bt_itemDeal_lock_over.bmp", 1, 0);
  static_cast<UIBitmapButton*>(this->dealLockButton)->SetBitmap("유저인터페이스\\inventory\\bt_itemDeal_lock_press.bmp", 2, 0);
  this->dealLockButton->SetSize(static_cast<UIBitmapButton*>(this->dealLockButton)->BitmapWidth(), static_cast<UIBitmapButton*>(this->dealLockButton)->BitmapHeight());
  this->dealLockButton->Move(124, this->geometry.height - 19);
  this->dealLockButton->SetTooltip(LookupMessage(MSI_CONNECT_SNS_PART_SCREEN_SHOT));
  this->dealLockButton->SetId(507);
  this->AddChild(this->dealLockButton);

  this->filterButton = new UIBitmapButton();
  this->RefreshSortButton();
  this->filterButton->SetSize(static_cast<UIBitmapButton*>(this->filterButton)->BitmapWidth(), static_cast<UIBitmapButton*>(this->filterButton)->BitmapHeight());
  this->filterButton->Move(157, this->geometry.height - 19);
  this->filterButton->SetId(364);
  this->AddChild(this->filterButton);
}

// OFFSET: 0x0096ED90
// FUNCTION: fn_13224
UIItemWnd::UIItemWnd() : UIItemBaseWnd() {
  this->dealLockButton = 0;
  this->filterWindow = nullptr;
  this->filterButton = 0;
  this->resizer = nullptr;
  this->scrollBar = nullptr;
  this->saved = nullptr;
  this->usableFilter = 0;
  this->equipmentFilter = 0;
  this->miscFilter = 0;
  this->categoryFilter = 0;
  this->scroll = 0;
  this->scrollOverflow = 0;
}

// OFFSET: 0x0096EE60
// FUNCTION: fn_13225
UIMerchantItemMyShopWnd::UIMerchantItemMyShopWnd(int value) : UIItemBaseWnd() {
  this->buyingShop = value;
  this->startTick = 0;
  this->duration = 0;
  this->actionButton = nullptr;
  this->footerHeight = 21;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
  this->shopOpen = 0;
  this->offlineStoreId = 0;
}

// OFFSET: 0x0096EF10
// FUNCTION: fn_13226
UIMerchantItemPurchaseMSGWnd::UIMerchantItemPurchaseMSGWnd()
    : UIFrameWnd(0) {
  this->title = nullptr;
  this->firstLine = nullptr;
  this->thirdLine = nullptr;
  this->secondLine = nullptr;
  this->buyButton = nullptr;
  this->cancelButton = nullptr;
  this->purchaseIndex = 0;
  this->purchaseAmount = 0;
}

// OFFSET: 0x0096EFC0
// FUNCTION: fn_13227
UIMerchantItemPurchaseWnd::UIMerchantItemPurchaseWnd(int value) : UIItemBaseWnd() {
  this->buyingStore = value;
  this->alternateButtonBar = 1;
  this->merchantId = 0;
  this->saved = nullptr;
  this->hovered = -1;
  g_session.ClearMerchantHopeToSellItems();
  this->nonItemHeight = 0;
}

// OFFSET: 0x0096F060
// FUNCTION: fn_13228
UIMerchantItemShopWnd::UIMerchantItemShopWnd(int value) : UIItemBaseWnd() {
  this->buyingZeny = 0;
  this->buyingShop = value;
  this->startTick = 0;
  this->duration = 0;
  this->alternateButtonBar = 1;
  this->hovered = -1;
  this->saved = nullptr;
  this->nonItemHeight = 0;
  this->footerHeight = 21;
}

// OFFSET: 0x0096F110
// FUNCTION: fn_13229
UIMerchantItemWnd::UIMerchantItemWnd() : UIItemBaseWnd() {
  this->alternateButtonBar = 1;
  this->saved = nullptr;
}

// OFFSET: 0x0096F180
// FUNCTION: fn_13230
UIMerchantMirrorItemWnd::UIMerchantMirrorItemWnd(int value) : UIItemBaseWnd() {
  this->mode = value;
  this->alternateButtonBar = 1;
  this->saved = nullptr;
}

// OFFSET: 0x0096F3A0
// FUNCTION: fn_13232
UINoticeMessageBox_LockSellItem::UINoticeMessageBox_LockSellItem() : UIFrameWnd(0) {
  this->startTick = timeGetTime();
  this->duration = 2000;
}

// OFFSET: 0x0096FB70
// FUNCTION: fn_13238
UIItemStoreWnd::~UIItemStoreWnd() {
  g_session.m_buyingStoreMode = 0;
}

// OFFSET: 0x0096FC10
// FUNCTION: fn_13239
UIItemWnd::~UIItemWnd() = default;

// OFFSET: 0x0098D1F0
// FUNCTION: fn_13423
int UIItemShopWnd2::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (!static_cast<ItemShopWndInfo*>(this->saved)->IsValid()) {
      static_cast<ItemShopWndInfo*>(this->saved)->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND:
    if (a == ID_CHECK) {
      g_windowMgr.dragAllItems = b;
      return 0;
    }
    break;
  case UM_DROP:
    switch (a) {
    case 0: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      ITEM_INFO item;
      item.Assign(mode->drag);
      g_session.RemoveItem1720ByName(item);
      if (g_windowMgr.itemPurchaseWnd) {
        g_windowMgr.itemPurchaseWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }
    case 1: {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      ITEM_INFO item;
      item.Assign(mode->drag);
      CSession* lists = &g_session;
      lists->MergeShopItem(item);
      lists->RemoveStorageItem(item);
      if (g_windowMgr.itemSellWnd2) {
        g_windowMgr.itemSellWnd2->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }
    }
    break;
  case UM_SETTITLE:
    this->interactionDisabled = 1;
    this->showQuantity = 1;
    this->title->SetTextAutoWidth(LookupMessage(MSI_ITEMSYOUCANCHANGE));
    this->checkBox->m_disabled = 0;
    return 0;
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - this->extraBottomHeight - this->bottomBarHeight - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->extraBottomHeight - this->bottomBarHeight - 17);
        this->checkBox->Move(12, this->geometry.height - 20);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList1Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1718At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0098D690
// FUNCTION: fn_13424
int UIItemShopWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemShopWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    if (a == ID_CHECK) {
      if (g_replayAnimationScaling != 0) {
        this->checkBox->geometry.state = b;
        this->checkBox->Invalidate();
      }

      g_windowMgr.dragAllItems = b;
      return 0;
    }
    break;
  }
  case UM_DROP:
    if (a == 0) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      ITEM_INFO item;
      item.Assign(mode->drag);
      g_session.RemoveItem1720ByName(item);
      if (g_windowMgr.itemPurchaseWnd) {
        g_windowMgr.itemPurchaseWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }
    if (a == 1) {
      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      ITEM_INFO item;
      item.Assign(mode->drag);
      CSession* lists = &g_session;
      lists->MergeShopItem(item);
      lists->RemoveStorageItem(item);
      if (g_windowMgr.itemSellWnd) {
        g_windowMgr.itemSellWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
      return 0;
    }
    break;
  case UM_SETTITLE:
    this->showFooter = b;
    if (a) {
      this->sellMode = 1;
      this->showQuantity = 1;
      this->title->SetTextAutoWidth(LookupMessage(MSI_ITEMSYOUCANSELL));
      this->checkBox->m_disabled = 0;
    } else {
      this->sellMode = 0;
      this->showQuantity = 0;
      this->title->SetTextAutoWidth(LookupMessage(MSI_SHOPITEMS));
      this->checkBox->m_disabled = 1;
    }

    this->footerHeight = this->showFooter != 0 ? 20 : 0;
    {
      const int height = this->geometry.height - this->bottomPadding - this->footerHeight - 17;
      this->visibleRows = height / 32;
      this->scrollBar->Resize(this->geometry.width - 15, height);
    }
    this->Invalidate();
    return 0;
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - this->bottomPadding - this->footerHeight - 17) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - this->bottomPadding - this->footerHeight - 17);
        this->checkBox->Move(12, this->geometry.height - 20);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList1Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1718At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    this->Invalidate();
    this->UpdateScrollBar();
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x0098E0D0
// FUNCTION: fn_45489
int UIItemStoreSubWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemStoreFindSlotInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset(this->tabIndex);
    }

    this->Move(this->saved->x, this->saved->y);
    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->drag.source == 1) {
      mode->SendMsg(MM_ADD_ITEM_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
    } else if (mode->drag.source == 9) {
      mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList5Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1738At(i);
      if (item.identity[1] == 0) {
        continue;
      }

      item.saleData[0] = g_itemInfoMgr->IsCostume(std::stoi(item.itemId));
      const int type = static_cast<int>(item.identity[0]);
      bool matches = false;
      if (item.saleData[0] != 0) {
        matches = this->tabIndex == 4;
      } else {
        switch (this->tabIndex) {
        case 0:
          matches = type == 0 || type == 1 || type == 2;
          break;
        case 1:
          matches = type == 18;
          break;
        case 2:
          switch (type) {
          case TYPE_ARMOR:
          case TYPE_ARMORTM:
          case TYPE_ARMORTB:
          case TYPE_ARMORMB:
          case TYPE_ARMORTMB:
            matches = true;
            break;
          }
          break;
        case 3:
          switch (type) {
          case TYPE_WEAPON:
          case TYPE_BOW:
          case TYPE_BOTHHAND:
          case TYPE_GUN:
            matches = true;
            break;
          }
          break;
        case 5:
          matches = type == 10 || type == 16 || type == 17 || type == 19;
          break;
        case 6:
          matches = type == 6;
          break;
        case 7:
          matches = type == 3 || type == 7;
          break;
        }
      }

      if (matches) {
        this->items.push_back(item);
      }
    }

    if (g_session.m_itemAscendingOrder) {
      this->Sort2();
    } else if (g_session.m_itemDescendingOrder) {
      this->SortReversed2();
    }

    if (!this->nameFilter.empty() && !this->FilterByName2(std::string(this->nameFilter.c_str(), this->nameFilter.size()))) {
      return 0;
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 140) {
        width = 140;
      }
      if (height < 145) {
        height = 145;
      }

      const int newWidth = (width - 140) / 32 * 32 + 140;
      const int newHeight = (height - 160) / 32 * 32 + 145;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->scroll = 0;
        this->Resize(newWidth, newHeight);
        this->resizeHandle->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->visibleRows = (newHeight - 38) / 32;
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  case UM_SETCOUNTINFO: {
    char text[64];
    std::sprintf(text, "%d/%d", a, b);
    this->footerText = text;
    this->Invalidate();
    return 0;
  }
  case UM_SEARCH_ITEMS:
    if (!this->nameFilter.empty() && this->FilterByName2(std::string(this->nameFilter.c_str(), this->nameFilter.size()))) {
      this->Invalidate();
      this->UpdateScrollBar();
    }
    return 0;
  case UM_SORT_ITEMS:
    if (g_session.m_itemDefaultSort == true) {
      this->SendMsg(nullptr, UM_RESET_ITEM_SORT, 0, 0, 0, 0);
      return 0;
    }

    this->sortEnabled = 1;
    if (g_session.m_itemAscendingOrder == true) {
      this->Sort2();
    } else if (g_session.m_itemDescendingOrder == true) {
      this->SortReversed2();
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  case UM_RESET_ITEM_SORT:
    this->sortEnabled = 0;
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    return 0;
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x0098E6C0
// FUNCTION: fn_45490
int UIItemStoreWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETCOUNTINFO:
    this->m_itemCount = a;
    this->m_capacity = b;
    this->m_capacityText = FormatThreadText("%d/%d", a, b);
    this->Invalidate();
    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemStoreFindListInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->m_sortOptionA = 3;
    this->m_sortOptionB = 3;
    this->m_sortOptionC = 3;
    this->m_sortOptionD = 3;
    this->m_sortOptionE = 4;
    for (int index = 0; index < 8; ++index) {
      if ((this->saved->openTabs & (1 << index)) != 0 && this->tabs[index]) {
        this->tabs[index]->SetChecked(1);
      }
    }

    this->Invalidate();
    return 0;
  case UM_COMMAND:
    switch (a) {
    case ID_CLOSE: {
      const int openTabs = this->saved->openTabs;
      for (int index = 0; index < 7; ++index) {
        g_windowMgr.CloseWindow(index + WID_ITEMSTORESUBWND_TAB_0);
      }
      g_windowMgr.CloseWindow(WID_ITEMSTORESUBWND_TAB_7);
      this->saved->openTabs = openTabs;
      g_windowMgr.CloseWindow(WID_ITEMSTOREFINDWND);
      this->m_searchWindow = nullptr;
      if (g_modeMgr.gameMode) {
        g_modeMgr.gameMode->SendMsg(MM_CLOSE_STORE, 0, 0, 0, 0);
      }
      break;
    }
    case ID_SORT_ITEMS:
      this->Sort();
      this->Invalidate();
      this->UpdateScrollBar();
      static_cast<UIButton*>(this->buttons[1])->Enable(0);
      static_cast<UIButton*>(this->buttons[3])->Enable(1);
      this->buttons[3]->Move(this->geometry.width - 48, this->geometry.height - 52);
      for (int id = 146; id < 153; ++id) {
        if (g_windowMgr.LookupWindow(id)) {
          g_windowMgr.LookupWindow(id)->SendMsg(nullptr, UM_SORT_ITEMS, 0, 0, 0, 0);
        }
      }
      if (g_windowMgr.LookupWindow(WID_ITEMSTORESUBWND_TAB_7)) {
        g_windowMgr.LookupWindow(WID_ITEMSTORESUBWND_TAB_7)->SendMsg(nullptr, UM_SORT_ITEMS, 0, 0, 0, 0);
      }
      if (this->m_searchWindow) {
        this->m_searchWindow->SendMsg(nullptr, UM_SORT_ITEMS, 0, 0, 0, 0);
      }
      break;
    case ID_PREV:
      this->m_searchFlags[0] = 0;
      static_cast<UIButton*>(this->buttons[1])->Enable(1);
      static_cast<UIButton*>(this->buttons[3])->Enable(0);
      this->buttons[1]->Move(this->geometry.width - 105, this->geometry.height - 24);
      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      for (int index = 0; index < 7; ++index) {
        if (this->m_tabWindows[index]) {
          this->m_tabWindows[index]->SendMsg(nullptr, UM_RESET_ITEM_SORT, 0, 0, 0, 0);
        }
      }
      this->buttons[1]->Move(220, this->geometry.height - 50);
      break;
    case 365: {
      std::string text(this->searchEdit->Text());
      if (!text.empty()) {
        UIItemStoreFindWnd* window = static_cast<UIItemStoreFindWnd*>(g_windowMgr.EnsureWindow(WID_ITEMSTOREFINDWND));
        this->m_searchWindow = window;
        window->SetSearchText(std::string(text.c_str(), text.size()));
        this->m_searchWindow->SendMsg(nullptr, UM_REFRESHITEM, 365, 0, 0, 0);
        this->m_searchWindow->SendMsg(nullptr, UM_SEARCH_ITEMS, 0, 0, 0, 0);
        this->searchEdit->SetText("");
      }
      break;
    }
    case WID_ITEMSTORESUBWND_TAB_0:
    case WID_ITEMSTORESUBWND_TAB_1:
    case WID_ITEMSTORESUBWND_TAB_2:
    case WID_ITEMSTORESUBWND_TAB_3:
    case WID_ITEMSTORESUBWND_TAB_4:
    case WID_ITEMSTORESUBWND_TAB_5:
    case WID_ITEMSTORESUBWND_TAB_6: {
      UICheckBox* tab = this->tabs[a - WID_ITEMSTORESUBWND_TAB_0];
      const int index = a - WID_ITEMSTORESUBWND_TAB_0;
      if (!tab) {
        break;
      }

      if (tab->geometry.state != 0) {
        this->m_tabWindows[index] = nullptr;
        this->m_tabWindows[index] = g_windowMgr.EnsureWindow(a);
        if (this->m_tabWindows[index]) {
          this->m_tabWindows[index]->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
        this->saved->openTabs |= 1 << index;
        break;
      }

      g_windowMgr.CloseWindow(a);
      if (this->m_tabWindows[index]) {
        this->saved->openTabs ^= 1 << index;
        this->m_tabWindows[index] = nullptr;
      }
      break;
    }
    case WID_ITEMSTORESUBWND_TAB_7:
      if (!this->tabs[7]) {
        break;
      }

      if (this->tabs[7]->geometry.state != 0) {
        this->m_tabWindows[7] = nullptr;
        this->m_tabWindows[7] = g_windowMgr.EnsureWindow(WID_ITEMSTORESUBWND_TAB_7);
        if (this->m_tabWindows[7]) {
          this->m_tabWindows[7]->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
        this->saved->openTabs |= 0x80;
        break;
      }

      g_windowMgr.CloseWindow(WID_ITEMSTORESUBWND_TAB_7);
      if (this->m_tabWindows[7]) {
        this->saved->openTabs ^= 0x80;
        this->m_tabWindows[7] = nullptr;
      }
      break;
    case 133:
      if (!this->m_searchWindow) {
        this->m_searchWindow = g_windowMgr.EnsureWindow(WID_ITEMSTOREFINDWND);
        std::string text(this->searchEdit->Text());
        if (!text.empty() && this->FilterByName(std::string(text.c_str(), text.size()))) {
          static_cast<UIItemStoreFindWnd*>(this->m_searchWindow)->SetSearchText(std::string(text.c_str(), text.size()));
          this->m_searchWindow->SendMsg(nullptr, UM_SEARCH_ITEMS, 0, 0, 0, 0);
        }
      }
      break;
    }
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (mode->drag.source == 1) {
      mode->SendMsg(MM_ADD_ITEM_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
    } else if (mode->drag.source == 9) {
      mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_RESIZE:
    if (a == 3) {
      int height = c;
      const int maximum = g_windowMgr.screenHeight * 7 / 10;
      if (height > maximum) {
        height = maximum;
      }
      if (height < 458) {
        height = 458;
      }

      const int newHeight = (height - 458) / 32 * 32 + 458;
      if (newHeight != this->geometry.height) {
        this->Resize(this->geometry.width, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - 74) / 32;
        this->buttons[0]->Move(220, this->geometry.height - 23);
        this->buttons[1]->Move(185, this->geometry.height - 23);
        for (int index = 0; index < 8; ++index) {
          this->tabs[index]->Move(index * 26 + 5, this->geometry.height - 52);
        }
        this->sortCombo->Move(214, this->geometry.height - 52);
        this->searchEdit->Move(77, this->geometry.height - 23);
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 74);
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList5Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1738At(i);
      if (item.identity[1] == 0) {
        continue;
      }

      item.saleData[0] = g_itemInfoMgr->IsCostume(std::stoi(item.itemId));
      const int type = static_cast<int>(item.identity[0]);
      bool matches = false;
      if (item.saleData[0] != 0) {
        matches = this->m_sortMode == 4;
      } else {
        switch (this->m_sortMode) {
        case 0:
          matches = type == 0 || type == 1 || type == 2;
          break;
        case 1:
          matches = type == 18;
          break;
        case 2:
          switch (type) {
          case TYPE_ARMOR:
          case TYPE_ARMORTM:
          case TYPE_ARMORTB:
          case TYPE_ARMORMB:
          case TYPE_ARMORTMB:
            matches = true;
            break;
          }
          break;
        case 3:
          switch (type) {
          case TYPE_WEAPON:
          case TYPE_BOW:
          case TYPE_BOTHHAND:
          case TYPE_GUN:
            matches = true;
            break;
          }
          break;
        case 5:
          matches = type == 10 || type == 16 || type == 17 || type == 19;
          break;
        case 6:
          matches = type == 6;
          break;
        case 7:
          matches = type == 3 || type == 7;
          break;
        }
      }

      if (matches) {
        this->items.push_back(item);
      }
    }

    if (g_session.m_itemAscendingOrder) {
      this->Sort();
    } else if (g_session.m_itemDescendingOrder) {
      this->SortReversed();
    }

    std::string text = this->searchEdit->Text();
    if (!text.empty() && a == 365) {
      if (this->FilterByName(std::string(text.c_str(), text.size()))) {
        this->Invalidate();
        this->UpdateScrollBar();
        for (int index = 0; index < 7; ++index) {
          UIWindow* window = g_windowMgr.LookupWindow(index + WID_ITEMSTORESUBWND_TAB_0);
          if (window) {
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }
        }
        UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMSTORESUBWND_TAB_7);
        if (window) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
        window = g_windowMgr.LookupWindow(WID_ITEMSTOREFINDWND);
        if (window) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
      }
    } else {
      for (int index = 0; index < 7; ++index) {
        UIWindow* window = g_windowMgr.LookupWindow(index + WID_ITEMSTORESUBWND_TAB_0);
        if (window) {
          window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        }
      }
      UIWindow* window = g_windowMgr.LookupWindow(WID_ITEMSTORESUBWND_TAB_7);
      if (window) {
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      window = g_windowMgr.LookupWindow(WID_ITEMSTOREFINDWND);
      if (window) {
        window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }
      this->Invalidate();
      this->UpdateScrollBar();
    }
    return 0;
  }
  case UM_TAB:
    this->m_sortMode = a;
    if (this->m_sortMode < 0) {
      this->m_sortMode = 0;
    } else if (this->m_sortMode > 7) {
      this->m_sortMode = 7;
    }

    this->Invalidate();
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    return 0;
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20900) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 20901) {
          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          break;
        }

        offset += header.size;
      }
    }
    return 0;
  }
  case UM_COMBOBOX_SELECTED:
    if (sender == this->sortCombo) {
      g_session.m_itemDefaultSort = a == 0;
      g_session.m_itemAscendingOrder = a == 1;
      g_session.m_itemDescendingOrder = a == 2;
      if (a == 0) {
        this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
        return 0;
      }

      if (a == 1) {
        this->Sort();
      } else if (a == 2) {
        this->SortReversed();
      }

      for (int index = 0; index < 8; ++index) {
        if (this->m_tabWindows[index]) {
          this->m_tabWindows[index]->SendMsg(nullptr, UM_SORT_ITEMS, 0, 0, 0, 0);
        }
      }
      if (this->m_searchWindow) {
        this->m_searchWindow->SendMsg(nullptr, UM_SORT_ITEMS, 0, 0, 0, 0);
      }

      this->Invalidate();
      this->UpdateScrollBar();
    }
    return 0;
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x00859470
// FUNCTION: fn_10776
void UIChatEditCtrl::InsertItemLink(const ITEM_INFO& item) {
  if (!this->itemTags) {
    this->itemTags = new (std::nothrow) UIItemTagOnChat();
    if (!this->itemTags) {
      return;
    }
  }

  if (!this->itemTags->CanInsertTag(item.DisplayName(0), this->geometry.width)) {
    return;
  }

  this->itemTags->taggedText = this->text;
  g_serverLanguage->ReplaceInput(this->itemTags->AppendTag(item.DisplayName(0)).c_str());
  this->itemTags->AddItemTag(item, 0x12B8A4);
}

// OFFSET: 0x00980340
// FUNCTION: fn_13335
void UIItemStoreFindWnd::Paint() {
  this->DrawTitleBar(true, "", 0);

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int rows = (this->geometry.height - 38) / 32;
  int slots = rows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 10, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, rows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int left = visible % this->columns * 32 + 10;
    const int width = this->geometry.width - left - 66;
    const int x = left + 4;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(left + 40, top + 29, width, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  this->DrawTextAt(16, this->geometry.height - 20, this->statusText.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x00980610
// FUNCTION: fn_13336
void UIItemStoreSubWnd::Paint() {
  this->DrawTitleBar(true, "", 0);

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int rows = (this->geometry.height - 38) / 32;
  int slots = rows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 10, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, rows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int left = visible % this->columns * 32 + 10;
    const int width = this->geometry.width - left - 66;
    const int x = left + 4;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(left + 40, top + 29, width, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  this->DrawTextAt(16, this->geometry.height - 20, this->footerText.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0097E7A0
// FUNCTION: fn_13330
void UIItemPurchaseWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int height = this->geometry.height;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(40, 17, this->geometry.width - 50, (height - 38) / 32 * 32, RGB(red, green, blue));

  int slots = (height - 58) / 32;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);

    int nameWidth = 110;
    const int amount = static_cast<int>(it->prices[0]);
    if (amount != static_cast<int>(it->prices[1])) {
      if (amount >= 10000) {
        nameWidth = 45;
      } else if (amount >= 1000) {
        nameWidth = 55;
      } else if (amount >= 100) {
        nameWidth = 70;
      } else if (amount >= 10) {
        nameWidth = 80;
      }
    }
    this->DrawWrappedItemName(column + 76, top + 29, nameWidth, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    char currency[10];
    StringFormat(currency, 10, static_cast<std::size_t>(-1), "Z");
    if (this->cashShopMode != 0) {
      const char* name;
      if (g_serviceType == 6) {
        name = "Koin";
      } else if (g_serviceType == 0) {
        if (g_session.m_channelCopId == 1) {
          name = "한코인";
        } else {
          name = LookupMessage(MSI_CASH);
        }
      } else {
        name = "Point";
      }
      sprintf(currency, name);
    }

    char price[64];
    char text[64];
    int length;
    if (it->prices[0] != it->prices[1]) {
      length = sprintf(text, " %d -> %s %s", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 64), currency);
    } else {
      length = sprintf(text, " %s %s", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 64), currency);
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  if (this->cashShopMode != 0) {
    this->DrawTextAt(12, this->geometry.height - 48, LookupMessage(MSI_USE_FREE_POINT), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(179, this->geometry.height - 48, LookupMessage(MSI_CASH), 0, 0, 12, 0, 0, 0);
    this->FillRegion(109, this->geometry.height - 51, 66, 18, 0x8E8E8E);
  }
}

// OFFSET: 0x0097F220
// FUNCTION: fn_13332
void UIItemSellWnd::Paint() {
  char price[64];
  char text[64];
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - 38) / 32;
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);

    int nameWidth = 100;
    const int amount = static_cast<int>(it->prices[0]);
    if (amount != static_cast<int>(it->prices[1])) {
      if (amount >= 10000) {
        nameWidth = 45;
      } else if (amount >= 1000) {
        nameWidth = 55;
      } else if (amount >= 100) {
        nameWidth = 70;
      } else if (amount >= 10) {
        nameWidth = 80;
      }
    }
    this->DrawWrappedItemName(column + 76, top + 29, nameWidth, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    int length;
    if (it->prices[0] == it->prices[1]) {
      length = sprintf(text, " %s Zeny", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 64));
    } else {
      length = sprintf(text, " %d -> %s Zeny", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 64));
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0097FBF0
// FUNCTION: fn_13334
void UIItemShopWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - this->bottomPadding - 17) / 8 + ((this->geometry.height - this->bottomPadding - 17) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - this->bottomPadding - 17) / 8 + ((this->geometry.height - this->bottomPadding - 17) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - this->bottomPadding - 17, RGB(red, green, blue));
  this->FillRegion(
    20, (this->geometry.height - this->bottomPadding - 17) / 32 * 32 + 17, this->geometry.width - 24,
    (this->geometry.height - this->bottomPadding - 17) % 32, RGB(red, green, blue)
  );
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  int slotRows = (this->geometry.height - this->footerHeight - this->bottomPadding - 17) / 32;
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  if (this->showFooter != 0) {
    slotRows = (this->geometry.height - this->bottomPadding - 17) / 32;
  }
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(red, green, blue));
  if (this->showFooter != 0) {
    this->FillRegion(40, slotRows * 32 - 15, 32, 32, RGB(red, green, blue));
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);

    int nameWidth = 100;
    const int amount = static_cast<int>(it->prices[0]);
    if (amount != static_cast<int>(it->prices[1])) {
      if (amount >= 10000) {
        nameWidth = 45;
      } else if (amount >= 1000) {
        nameWidth = 55;
      } else if (amount >= 100) {
        nameWidth = 70;
      } else if (amount >= 10) {
        nameWidth = 80;
      }
    }
    this->DrawWrappedItemName(column + 76, top + 29, nameWidth, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    char currency[8];
    sprintf(currency, "Z");
    if (this->showFooter != 0) {
      const char* name;
      if (g_serviceType == 6) {
        name = "K";
      } else if (g_serviceType == 0) {
        name = g_session.m_channelCopId != 1 ? "C" : "H";
      } else {
        name = "P";
      }
      sprintf(currency, name);
    }

    char price[64];
    char text[64];
    int length;
    if (it->prices[0] != it->prices[1]) {
      length = sprintf(text, " %d -> %s %s", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 64), currency);
    } else {
      length = sprintf(text, " %s %s", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 64), currency);
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  if (this->showFooter != 0) {
    int message;
    if (g_serviceType == 0 && g_session.m_channelCopId == 1) {
      message = 1290;
    } else {
      message = 1210;
    }

    char text[352];
    const int aid = g_session.m_pendingAid;
    const char* name = this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0);
    int length = sprintf(text, LookupMessage(message), name, aid);
    this->DrawTextAt(12, this->geometry.height - 21, text, length, 0, 12, 0, 0, 0);

    const int gid = g_session.m_pendingGid;
    name = this->ClipTextToWidth(g_session.DecodePlayerName(), 100, 0, 12, 0);
    length = sprintf(text, LookupMessage(MSI_POINT_SHOP2), name, gid);
    this->DrawTextAt(12, this->geometry.height - this->bottomPadding - 21, text, length, 0, 12, 0, 0, 0);
  }
}

// OFFSET: 0x009808E0
// FUNCTION: fn_13337
void UIItemStoreWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 3 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - 74) / 32;
  if (!this->m_hideCapacity) {
    this->scrollBar->Show(0);
    unsigned fillRed, fillGreen, fillBlue;
    g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
    this->FillRegion(40, 17, columns * 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));
    DrawRoundedBorder(*this, 55, 131, 187, 139, 0xCE9484);
    DrawFilledRoundedBorder(*this, 56, 132, 185, 137, 0xFFE7DE);
    DrawFilledRoundedBorder(*this, 61, 137, 175, 127, 0xFFFFFF);
    auto* poring = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\notice_poring.bmp")));
    this->DrawBitmap(116, 151, poring, true);
    this->DrawCenteredText(20, 210, LookupMessage(MSI_RODEX_BUTTON_ANSWER), 257, 0, 12, 0, 0);
    this->DrawCenteredText(20, 236, LookupMessage(MSI_CLOSE_SETUP), 257, 0, 15, 0xC89482, 1);
    this->DrawCenteredText(20, 272, LookupMessage(MSI_USE_WORLDMAP_WITH_PERIOD_MSG2), 257, 0, 12, 0x6464DC, 1);
    return;
  }

  this->scrollBar->Show(1);
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int column = visible % this->columns * 32;
    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(column + 80, top + 29, 180, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  auto* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\icon_num.bmp")));
  this->DrawBitmap(6, this->geometry.height - 21, icon, true);
  const std::uint32_t color = this->m_itemCount >= this->m_capacity ? 0xFF : 0x232323;
  this->DrawTextAt(22, this->geometry.height - 19, FormatOwnedString("%d", this->m_itemCount).c_str(), 0, 0, 11, color, 0, 0);
  const int width = this->TextWidth(FormatOwnedString("%d", this->m_itemCount).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(width + 23, this->geometry.height - 19, FormatOwnedString("/%d", this->m_capacity).c_str(), 0, 0, 11, 0, 0, 0);
}

// OFFSET: 0x009819C0
// FUNCTION: fn_13341
void UIMerchantItemPurchaseWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - 38) / 32;
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawWrappedItemName(column + 76, top + 29, 90, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    char price[64];
    char text[64];
    int length;
    if (it->prices[0] == it->prices[1]) {
      length = sprintf(text, " %s Zeny", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 64));
    } else {
      length = sprintf(text, " %d -> %s Zeny", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 64));
    }
    this->DrawRightAlignedText(this->geometry.width - 18, top + 29, text, length, 0, 12, 0, 0);
    ++visible;
  }
}

// OFFSET: 0x00982A80
// FUNCTION: fn_13344
void UIMerchantShopMakeWnd::Paint() {
  this->ClearSurface(0xFF);
  this->DrawTitleBar(false, "", 0);

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  int barHeight;
  if (this->mode != 0) {
    barHeight = this->DrawButtonBar(2, 0, 2);
  } else {
    this->DrawButtonBar(2, 0, 1);
  }

  for (int i = 0; i < this->slotCount; ++i) {
    if (this->mode != 0) {
      if (i < this->visibleRows) {
        this->priceEdits[i]->Show(1);
        this->quantityEdits[i]->Show(1);
      } else {
        this->priceEdits[i]->Show(0);
        this->quantityEdits[i]->Show(0);
      }
    } else if (i < this->visibleRows) {
      this->priceEdits[i]->Show(1);
    } else {
      this->priceEdits[i]->Show(0);
    }
  }

  for (int i = 0; i < this->slotCount; ++i) {
    bool hidden = true;
    if (this->scroll <= i && i < this->visibleRows + this->scroll) {
      hidden = false;
      this->priceEdits[i]->Move(this->geometry.width - 119, (i - this->scroll) * 42 + 54);
    }
    this->priceEdits[i]->Show(hidden ^ 1);
    if (this->mode != 0) {
      this->quantityEdits[i]->Move(this->geometry.width + hidden * 340 - 221, (i - this->scroll) * 42 + 54);
    }
  }

  const char* message;
  int messageY;
  if (this->mode != 0) {
    int count = this->items.size();
    if (static_cast<unsigned>(count) < static_cast<unsigned>(this->minimumRows)) {
      count = this->minimumRows;
    }

    for (int i = 0, y = 72; i < count; ++i, y += 42) {
      this->DrawTextAt(this->geometry.width - 155, y - 15, LookupMessage(MSI_BUYINGSTORE_MAKEWND_PRICE), 0, 0, 12, 0, 0, 0);
      this->DrawTextAt(this->geometry.width - 176, y - 15, LookupMessage(MSI_EA4), 0, 0, 12, 0, 0, 0);
      DrawItemSlotBackground(*this, 3, y - 18);
      if (i >= this->slotCount) {
        continue;
      }

      char text[24] = {};
      std::strcpy(text, this->priceEdits[i]->Text());
      if (g_session.IsNumberWithCommas(text) != 1) {
        continue;
      }

      std::int64_t price = _atoi64(text);
      if (price <= 0) {
        continue;
      }

      const int x = this->geometry.width - 155;
      this->DrawTextAt(x, y, LookupMessage(MSI_MAKE_CHAT_ROOM), 0, 0, 12, 0, 0, 0);
      const int width = this->TextWidth(LookupMessage(MSI_MAKE_CHAT_ROOM), 0, 0, 12, 0, 0);
      const double rate = PriceCommission(price) * 0.01;
      price += static_cast<std::int64_t>(price * rate);
      FormatGroupedInteger(price, text, 24);
      const PriceColors colors = PriceColor(price);
      if (colors.secondary == 0xFFFFFFFF) {
        this->DrawTextAt(width + x, y, text, 0, 0, 12, colors.primary, 0, 0);
      } else {
        this->DrawEmboldenedText(width + x, y, text, colors.primary, colors.secondary, 0, 0, 12);
      }
    }

    this->DrawTextAt(17, 4, LookupMessage(MSI_BUYINGSTORE_MAKEWND_TITLE), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, LookupMessage(MSI_BUYINGSTORE_MAKEWND_TITLE), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(2, 25, LookupMessage(MSI_STALL_NAME), 0, 0, 12, 0, 0, 0);

    const int labelWidth = this->TextWidth(LookupMessage(MSI_BUYINGSTORE_MAKEWND_LIMITZENY), 0, 0, 12, 0, 0) + 10;
    this->zenyLimitEdit->Move(labelWidth, this->geometry.height - barHeight + 5);
    this->DrawTextAt(5, this->geometry.height - barHeight + 5, LookupMessage(MSI_BUYINGSTORE_MAKEWND_LIMITZENY), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(this->zenyLimitEdit->geometry.width + this->zenyLimitEdit->geometry.x + 5, this->geometry.height - barHeight + 5,
      "Zeny", 0, 0, 12, 0, 0, 0
    );

    char grouped[16] = {};
    char text[32] = {};
    FormatGroupedInteger(g_session.m_zeny, grouped, 16);
    sprintf(text, "%s Zeny", grouped);
    this->DrawTextAt(5, this->geometry.height - barHeight * 2 + 5, LookupMessage(MSI_BUYINGSTORE_MAKEWND_MYZENY), 0, 0, 12, 0, 0, 0);
    this->DrawRightAlignedText(155, this->geometry.height - barHeight * 2 + 5, text, 0, 0, 12, 0, 0);

    std::uint32_t color;
    if (this->itemsSet == 1) {
      sprintf(text, "Max Weight : %3d", g_session.m_maxWeight);
      color = 0;
    } else {
      sprintf(text, "Weight : %3d / %3d", g_session.m_weight, g_session.m_maxWeight);
      color = g_session.m_weight * 100 / (g_session.m_maxWeight < 1 ? 1 : g_session.m_maxWeight) < 50 ? 0 : 0xFF;
    }
    this->DrawRightAlignedText(this->geometry.width - 20, this->geometry.height - barHeight * 2 + 5, text, 0, 0, 12, color, 0);

    message = LookupMessage(MSI_NOTIFY_JOBEXP_INT64);
    messageY = this->geometry.height - 72;
  } else {
    for (int i = 0, y = 72; i < this->visibleRows; ++i, y += 42) {
      this->DrawTextAt(this->geometry.width - 165, y - 15, LookupMessage(MSI_SELLPRICE), 0, 0, 12, 0, 0, 0);
      DrawItemSlotBackground(*this, 3, y - 18);
      if (i >= this->slotCount) {
        continue;
      }

      char text[24] = {};
      std::strcpy(text, this->priceEdits[this->scroll + i]->Text());
      if (g_session.IsNumberWithCommas(text) != 1) {
        continue;
      }

      std::int64_t price = _atoi64(text);
      if (price <= 0) {
        continue;
      }

      const int x = this->geometry.width - 165;
      this->DrawTextAt(x, y, LookupMessage(MSI_MAKE_CHAT_ROOM), 0, 0, 12, 0, 0, 0);
      const int width = this->TextWidth(LookupMessage(MSI_MAKE_CHAT_ROOM), 0, 0, 12, 0, 0);
      const double rate = PriceCommission(price) * -0.01;
      price += static_cast<std::int64_t>(price * rate);
      FormatGroupedInteger(price, text, 24);
      const PriceColors colors = PriceColor(price);
      if (colors.secondary == 0xFFFFFFFF) {
        this->DrawTextAt(width + x, y, text, 0, 0, 12, colors.primary, 0, 0);
      } else {
        this->DrawEmboldenedText(width + x, y, text, colors.primary, colors.secondary, 0, 0, 12);
      }
    }

    this->DrawTextAt(17, 4, LookupMessage(MSI_MERCHANTSHOPMAKER), 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 3, LookupMessage(MSI_MERCHANTSHOPMAKER), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(2, 25, LookupMessage(MSI_STALL_NAME), 0, 0, 12, 0, 0, 0);

    message = LookupMessage(MSI_CANNOT_JOIN_CHARACTER_HAVING_LOW_SKILL);
    messageY = this->geometry.height - 43;
  }
  this->DrawTextAt(5, messageY, message, 0, 0, 12, 0xFF, 0, 0);

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 42;
    const int column = visible % this->columns * 42;
    const int x = column + 7;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 48, *it, 0, 0);
    this->DrawWrappedItemName(column + 39, top + 57, 100, &*it, 0, 0, 12);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 60,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0098F700
// FUNCTION: fn_13426
int UIItemWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case 456:
      Singleton<CGameSettingsMgr>::GetInstance()->ToggleOption(264);
      return 0;
    case ID_TOGGLE_ITEM_TRADE_LOCK: {
      const char* normal;
      const char* over;
      const char* press;
      if (g_session.m_blockPrivateTab == 1) {
        if (this->filterWindow) {
          this->SendMsg(this->filterWindow, UM_COMMAND, ID_CLOSE, 0, 0, 0);
        }
        g_session.m_blockPrivateTab = 0;
        press = "유저인터페이스\\inventory\\bt_itemDeal_lock_press.bmp";
        over = "유저인터페이스\\inventory\\bt_itemDeal_lock_over.bmp";
        normal = "유저인터페이스\\inventory\\bt_itemDeal_lock_off.bmp";
      } else {
        if (!this->filterWindow) {
          this->filterWindow = new UINoticeMessageBox_LockSellItem();
          this->filterWindow->SetSize(242, 26);
          this->filterWindow->Move(30, this->geometry.height - 50);
          this->AddChild(this->filterWindow);
        }
        g_session.m_blockPrivateTab = 1;
        press = "유저인터페이스\\inventory\\bt_itemDeal_lock_on_press.bmp";
        over = "유저인터페이스\\inventory\\bt_itemDeal_lock_on_over.bmp";
        normal = "유저인터페이스\\inventory\\bt_itemDeal_lock_on.bmp";
      }
      UIBitmapButton* button = static_cast<UIBitmapButton*>(this->dealLockButton);
      button->SetBitmap(normal, 0, 0);
      button->SetBitmap(over, 1, 0);
      button->SetBitmap(press, 2, 0);
      this->dealLockButton->Invalidate();
      this->Invalidate();
      return 0;
    }
    case ID_EXTEND_INVENTORY:
      if (CMode* mode = g_modeMgr.GetGameMode()) {
        mode->SendMsg(MM_REQ_OPEN_MSGBOX_EXTEND_BODYITEM_SIZE, 0, 0, 0, 0);
      }
      return 0;
    case ID_SORT_ITEMS:
      if (this->categoryFilter != 3) {
        return 0;
      }
      g_session.m_sortPrivateTab = !g_session.m_sortPrivateTab;
      this->RefreshSortButton();
      this->RefreshItems();
      return 0;
    case ID_CLOSE:
      if (this->filterWindow == sender) {
        this->RemoveChild(this->filterWindow, 0);
        this->filterWindow = nullptr;
        this->Invalidate();
      } else {
        g_windowMgr.CloseWindow(WID_ITEMWND);
      }
      return 0;
    case ID_MINI: {
      const int height = this->geometry.height;
      if (height == 17) {
        this->Resize(this->geometry.width, this->expandedHeight);
      } else {
        this->expandedHeight = height;
        this->Resize(this->geometry.width, 17);
      }
      g_windowMgr.StoreSnapRectangle(this);
      g_windowMgr.Promote(this);
      const int delta = this->geometry.height - height;
      if (delta != 0) {
        g_snapMgr.MoveEdge(this, 0, delta, 3);
      }
      return 0;
    }
    case ID_CHECK:
      g_session.m_lockItemDrop = !g_session.m_lockItemDrop;
      return 0;
    default:
      return 0;
    }
  case UM_RESIZE: {
    if (a != 7) {
      return 0;
    }
    int width = b <= 320 ? b : 320;
    int height = c <= 240 ? c : 240;
    if (width < 232) {
      width = 232;
    }
    if (height < 102) {
      height = 102;
    }
    const int tabHeight = this->tabs->geometry.height;
    if (height < tabHeight + 38) {
      height = tabHeight + 38;
    }
    const int rowsHeight = tabHeight + (height - tabHeight - 38) / 32 * 32;
    const int newWidth = (width - 280) / 32 * 32 + 280;
    if (newWidth != this->geometry.width || rowsHeight + 38 != this->geometry.height) {
      this->Resize(newWidth, rowsHeight + 38);
      this->columns = (newWidth - 40) / 32;
      this->visibleRows = rowsHeight / 32;
      this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
      this->scrollBar->Move(this->geometry.width - 15, 17);
      this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
      this->UpdateScrollBar();
      this->Invalidate();
      const int positions[3] = {3, this->geometry.width - 28, this->geometry.width - 14};
      UIBitmapButton* buttons[3] = {this->baseButton, this->minimizeButton, this->closeButton};
      for (int index = 0; index < 3; ++index) {
        buttons[index]->Move(positions[index], 3);
      }
    }
    this->expandedHeight = this->geometry.height;
    g_windowMgr.StoreSnapRectangle(this);
    this->lockDropBox->Move(79, this->geometry.height - 16);
    this->optionBox->Move(103, this->geometry.height - 17);
    this->countButton->Move(23, this->geometry.height - 17);
    this->dealLockButton->Move(124, this->geometry.height - 19);
    if (this->filterWindow) {
      this->filterWindow->Move(30, this->geometry.height - 50);
    }
    this->filterButton->Move(157, this->geometry.height - 19);
    return 0;
  }
  case UM_TAB:
    this->categoryFilter = static_cast<int>(a);
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    return 0;
  case UM_REFRESHITEM:
    if (this->categoryFilter == 3) {
      this->RefreshItems();
      return 0;
    }
    this->items.erase(this->items.begin(), this->items.end());
    for (int i = 0; i < g_session.GetList0Size(); ++i) {
      ITEM_INFO item = g_session.GetInventoryItemAt(i);
      if (item.identity[1] == 0) {
        continue;
      }
      const unsigned type = item.identity[0];
      bool include = false;
      if (item.flags[0] != 0) {
        include = this->categoryFilter == 3;
      } else if (this->categoryFilter == 0) {
        if (type <= 2 || type == 18) {
          if (this->usableFilter == 0) {
            include = type == 0;
          } else if (this->usableFilter == 2) {
            include = type == 1 || type == 2;
          } else if (this->usableFilter == 3) {
            include = true;
          }
        }
      } else if (this->categoryFilter == 1) {
        switch (type) {
        case TYPE_ARMOR:
        case TYPE_WEAPON:
        case TYPE_BOW:
        case TYPE_BOTHHAND:
        case TYPE_ARMORTM:
        case TYPE_ARMORTB:
        case TYPE_ARMORMB:
        case TYPE_ARMORTMB:
        case TYPE_GUN:
          if (this->equipmentFilter == 0) {
            include = type == 5 || type == 9 || type == 8;
          } else if (this->equipmentFilter == 1) {
            include = type == 4 || type == 11 || type == 12 || type == 13 || type == 14;
          } else if (this->equipmentFilter == 3) {
            include = true;
          }
          break;
        default:
          break;
        }
      } else if (this->categoryFilter == 2) {
        if (type == 6 || type == 3 || type == 7 || type == 10 || type == 16 || type == 17 || type == 19) {
          include = this->miscFilter == 4;
        }
      }
      if (include) {
        this->items.push_back(item);
      }
    }
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemWndInfo*>(a);
    if (this->saved->IsValid() == 0) {
      this->saved->Reset();
    }
    this->SendMsg(nullptr, UM_RESIZE, 7, this->saved->width, this->saved->expandedHeight, 0);
    this->categoryFilter = this->saved->selectedTab;
    this->usableFilter = 3;
    this->equipmentFilter = 3;
    this->miscFilter = 4;
    this->tabs->activeTabIndex = this->saved->selectedTab;
    this->Invalidate();
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    if (this->saved->height != this->saved->expandedHeight) {
      this->SendMsg(nullptr, UM_COMMAND, ID_MINI, 0, 0, 0);
    }
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_DROP: {
    CMode* mode = g_modeMgr.GetGameMode();
    ModeDragState& drag = static_cast<CGameMode*>(mode)->drag;
    switch (drag.source) {
    case 2:
      mode->SendMsg(MM_TAKE_OFF_EQUIP, drag.slot, 0, 0, 0);
      break;
    case 6:
      mode->SendMsg(MM_MOVE_ITEM_FROM_STORE_TO_BODY, static_cast<int>(drag.id), drag.quantity, 0, 0);
      break;
    case 9:
      mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_BODY, static_cast<int>(drag.id), drag.quantity, 0, 0);
      break;
    case 15:
      if (g_windowMgr.rodexComposeWnd) {
        g_windowMgr.rodexComposeWnd->SendMsg(nullptr, UM_DECITEM, static_cast<std::intptr_t>(drag.id), drag.quantity, 0, 0);
      }
      break;
    case 17:
      mode->SendMsg(MM_TAKE_OFF_SWAP_EQUIP, drag.slot, 0, 0, 0);
      break;
    case 18:
      mode->SendMsg(MM_TAKE_OFF_SWAP_EQUIP, drag.slot, 1, 0, 0);
      break;
    case 24:
      mode->SendMsg(MM_TAKE_OFF_EQUIP, drag.slot, 1, 0, 0);
      break;
    default:
      break;
    }
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x009957C0
// FUNCTION: fn_13461
void UIItemWnd::RefreshSortButton() {
  std::string name = "유저인터페이스\\inventory\\bt_sort";
  if (!g_session.m_sortPrivateTab) {
    name += "_off";
  }

  static_cast<UIBitmapButton*>(this->filterButton)->SetBitmap(std::format("{}.bmp", name).c_str(), 0, 0);
  static_cast<UIBitmapButton*>(this->filterButton)->SetBitmap(std::format("{}_over.bmp", name).c_str(), 1, 0);
  static_cast<UIBitmapButton*>(this->filterButton)->SetBitmap(std::format("{}_press.bmp", name).c_str(), 2, 0);
  this->filterButton->SetTooltip(std::format("{} {}", LookupMessage(MSI_BABY_INDEPENDENCE_RODEX_ERASED), g_session.m_sortPrivateTab ? "ON" : "OFF").c_str());
  this->Invalidate();
}

// OFFSET: 0x00990810
// FUNCTION: fn_13428
int UIMerchantItemPurchaseMSGWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != UM_COMMAND) {
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  switch (a) {
  case ID_OK:
    g_modeMgr.gameMode->SendMsg(MM_PC_PURCHASE_ITEMLIST_FROMMC, this->purchaseIndex, this->purchaseAmount, 0, 0);
  case ID_CANCEL:
    g_windowMgr.CloseWindow(WID_MERCHANTITEMPURCHASEMSGWND);
    g_session.ClearItems1760And1748();
    break;
  }
  return 0;
}

// OFFSET: 0x00995170
// FUNCTION: fn_13443
void UIItemWnd::RefreshItems() {
  this->items.clear();
  for (int i = 0; i < g_session.GetList0Size(); ++i) {
    ITEM_INFO item = g_session.GetInventoryItemAt(i);
    if (item.flags[0] == 1) {
      this->items.push_back(item);
    }
  }

  if (g_session.m_sortPrivateTab) {
    this->items.sort(ItemTypeNameLess());
  }
  this->Invalidate();
  this->UpdateScrollBar();
}

// OFFSET: 0x00995E10
// FUNCTION: fn_13466
bool UIItemStoreFindWnd::FilterByName(std::string query) {
  if (!this->UpdateItemNames()) {
    return false;
  }

  for (auto it = this->items.begin(); it != this->items.end();) {
    ITEM_INFO item = *it;
    const std::string name = item.displayName;
    auto next = std::next(it);
    if (!_mbsstr(reinterpret_cast<const unsigned char*>(name.c_str()), reinterpret_cast<const unsigned char*>(query.c_str()))) {
      this->items.erase(it);
    }
    it = next;
  }
  return true;
}

// OFFSET: 0x00996090
// FUNCTION: fn_13467
bool UIItemStoreSubWnd::FilterByName2(std::string query) {
  if (!this->UpdateItemNames2()) {
    return false;
  }

  for (auto it = this->items.begin(); it != this->items.end();) {
    ITEM_INFO item = *it;
    const std::string name = item.displayName;
    if (name.find(query) == std::string::npos) {
      it = this->items.erase(it);
    } else {
      ++it;
    }
  }
  return true;
}

// OFFSET: 0x00996440
// FUNCTION: fn_13468
bool UIItemStoreWnd::FilterByName(std::string query) {
  if (!this->UpdateItemNames()) {
    return false;
  }

  for (auto it = this->items.begin(); it != this->items.end();) {
    ITEM_INFO item = *it;
    const std::string name = item.displayName;
    if (name.find(query) == std::string::npos) {
      it = this->items.erase(it);
    } else {
      ++it;
    }
  }
  return true;
}

// OFFSET: 0x00996950
// FUNCTION: fn_13474
bool UIItemStoreFindWnd::UpdateItemNames() {
  if (this->items.empty()) {
    return false;
  }

  for (std::list<ITEM_INFO>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    char text[256];
    char* highlight = nullptr;
    char* cursor = text;
    std::vector<char*> breaks;
    ITEM_INFO item = *it;
    std::size_t remaining = 0x100;
    int highlightColor;
    this->BuildItemName(&item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);
    it->SetName(text);
  }
  return true;
}

// OFFSET: 0x00996AF0
// FUNCTION: fn_13475
bool UIItemStoreSubWnd::UpdateItemNames2() {
  if (this->items.empty()) {
    return false;
  }

  for (std::list<ITEM_INFO>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    char text[256];
    char* highlight = nullptr;
    char* cursor = text;
    std::vector<char*> breaks;
    ITEM_INFO item = *it;
    std::size_t remaining = 0x100;
    int highlightColor;
    this->BuildItemName(&item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);
    it->SetName(text);
  }
  return true;
}

// OFFSET: 0x00996C90
// FUNCTION: fn_13476
bool UIItemStoreWnd::UpdateItemNames() {
  if (this->items.empty()) {
    return false;
  }

  for (std::list<ITEM_INFO>::iterator it = this->items.begin(); it != this->items.end(); ++it) {
    char text[256];
    char* highlight = nullptr;
    char* cursor = text;
    std::vector<char*> breaks;
    ITEM_INFO item = *it;
    std::size_t remaining = 0x100;
    int highlightColor;
    this->BuildItemName(&item, &highlightColor, &breaks, &cursor, &remaining, &highlight, 1, 0);
    it->SetName(text);
  }
  return true;
}

// OFFSET: 0x00996E30
// FUNCTION: fn_13477
void UIMerchantItemPurchaseMSGWnd::SetPurchase(int value, int amount) {
  this->purchaseIndex = value;
  this->purchaseAmount = amount;

  int total = 0;
  const int count = g_session.GetList10Size();
  for (int i = 0; i < count; ++i) {
    ITEM_INFO item;
    item = g_session.GetItem1760At(i);
    if (item.identity[1] > 0) {
      total += item.quantity * item.prices[0];
    }
  }

  const std::string text = IntToSignedString(total, true, false) + std::string(LookupMessage(MSI_CARDWIN));
  this->secondLine->SetText(text.c_str(), 0);
  this->Invalidate();
}

// OFFSET: 0x00857420
// FUNCTION: fn_10762
void UIEditCtrl3::DecodeLinkFields(const std::string& link, std::vector<int>& fields) {
  char delimiters[] = "!#$%&'()*+,-/";
  const int length = static_cast<int>(link.size()) + 1;
  if (length < 6) {
    return;
  }

  char* text = new char[length];
  strcpy_s(text, length, link.c_str());

  std::string value;
  int optionCount = 0;
  std::vector<int> options;
  int cardCount = 0;

  fields.reserve(23);
  for (int i = 23; i != 0; --i) {
    fields.push_back(0);
  }

  fields[6] = -1;
  value.assign(text, 5);
  fields[0] = this->DecodeLinkNumber(std::move(value));
  value.clear();
  value += text[5];
  fields[1] = this->DecodeLinkNumber(std::move(value));
  value.clear();

  int field = 2;
  for (int position = 6; static_cast<unsigned>(position) < link.size() + 1; ++position) {
    const char byte = text[position];
    if (byte >= '0') {
      value += byte;
      continue;
    }

    for (unsigned int kind = 0; kind < 14; ++kind) {
      if (delimiters[kind] != byte) {
        continue;
      }

      if (kind == 7) {
        if (!value.empty()) {
          fields[field] = this->DecodeLinkNumber(std::move(value));
        }

        const int size = static_cast<int>(link.size());
        cardCount = this->DecodeLinkCards(fields, text, position, delimiters[7], size, 6);
      } else if (kind == 9) {
        ++optionCount;
        if (!value.empty()) {
          fields[field] = this->DecodeLinkNumber(std::move(value));
        }

        const int size = static_cast<int>(link.size());
        this->DecodeLinkOptions(options, text, position, size, delimiters[10], delimiters[11]);
      } else if (!value.empty()) {
        fields[field] = this->DecodeLinkNumber(std::move(value));
      }

      value.clear();
      field = kind;
      break;
    }
  }

  if (optionCount) {
    fields[cardCount + 8] = optionCount;
    for (unsigned int i = 0; i < options.size(); ++i) {
      fields[cardCount + 9 + i] = options[i];
    }
  }

  delete[] text;
}

// OFFSET: 0x00859220
// FUNCTION: fn_10775
bool UIEditCtrl3::LinkDisplayName(const std::string& encoded, std::string& output) {
  const bool isTag = encoded.substr(0, 6) == "00000T";
  if (isTag) {
    const int id = ItemLinkMessageId(encoded);
    if (g_itemTagOwner->maxTipId >= id) {
      g_itemTagOwner->Title(id, &output, 17);
      return true;
    }

    return false;
  }

  std::vector<int> fields;
  this->DecodeLinkFields(encoded, fields);
  ITEM_INFO item;
  bool decoded;
  if (this->DecodeItemLink(fields, item)) {
    output = item.DisplayName(0);
    decoded = true;
  } else {
    decoded = false;
  }

  return decoded;
}
// OFFSET: 0x00981EE0
// FUNCTION: fn_13342
void UIMerchantItemShopWnd::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  unsigned rows = static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) / 8 +
                  (static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) % 8 != 0);
  for (int y = 17; rows != 0; --rows, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) / 8 +
         (static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) % 8 != 0);
  for (int y = 17; rows != 0; --rows, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - this->footerHeight - 17, RGB(red, green, blue));
  this->FillRegion(
    20, static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) / 32 * 32 + 17, this->geometry.width - 24,
    static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) % 32, RGB(red, green, blue)
  );
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const unsigned slotRows = static_cast<unsigned>(this->geometry.height - this->footerHeight - 17) / 32;
  unsigned slots = slotRows;
  for (int y = 17; slots != 0; --slots, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  {
    std::string actor = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->ActorName(this->ownerAid);
    const char* label = LookupMessage(this->buyingShop == 0 ? MSI_ITEM_MERCHANT_STORE : MSI_BUYINGSTORE_SHOPWND_TITLE);
    char title[128];
    if (!actor.empty()) {
      sprintf(title, "%s - %s", label, actor.c_str());
    } else {
      g_windowMgr.RegisterWindow(this);
      sprintf(title, "%s", label);
    }
    this->DrawTextAt(17, 3, title, 0, 0, 12, 0xFFFFFF, 0, 0);
    this->DrawTextAt(16, 2, title, 0, 0, 12, 0, 0, 0);
  }

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    const int left = column + 40;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(left + 32, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    char price[128];
    int x;
    if (this->buyingShop != 0) {
      sprintf(price, LookupMessage(MSI_EA), it->quantity);
      const int quantityX = this->geometry.width - this->TextWidth("  100,000,000Z", 0, 0, 12, 0, 0) -
                            this->TextWidth(price, 0, 0, 12, 0, 0) - 18;
      this->DrawTextAt(quantityX, top + 29, price, 0, 0, 12, 0xFF, 0, 0);
      x = left + 4;
      DrawItemIcon(*this, ResourceMgr::Instance(), x, top + 21, *it, 0);
    } else {
      x = column + 44;
      DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    }

    if (it->grade > 0) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    int width = this->TextWidth("Z", 0, 0, 12, 0, 0);
    this->DrawTextAt(this->geometry.width - width - 18, top + 29, "Z", 0, 0, 12, 0, 0, 0);

    char text[128];
    if (it->prices[0] != it->prices[1]) {
      sprintf(text, " %d -> %s ", it->prices[0], FormatGroupedInteger(static_cast<int>(it->prices[1]), price, 128));
    } else {
      sprintf(text, " %s ", FormatGroupedInteger(static_cast<int>(it->prices[0]), price, 128));
    }
    width += this->TextWidth(text, 0, 0, 12, 0, 0);
    this->DrawAmount(static_cast<int>(it->prices[0]), text, this->geometry.width - width - 18, top + 29);

    int nameWidth = this->geometry.width - width - left - 66;
    if (nameWidth > 130) {
      nameWidth = 130;
    }
    this->DrawWrappedItemName(column + 76, top + 29, nameWidth, &*it, 0, 0, 12);
    ++visible;
  }

  if (this->footerHeight == 57) {
    this->DrawButtonBar(3, 0, 1);
  }

  if (this->duration != 0) {
    const int remaining = static_cast<int>(this->startTick + this->duration - timeGetTime());
    if (remaining > 0) {
      const int days = remaining / 86400000;
      const int hours = remaining / 3600000 % 24;
      const int minutes = remaining / 60000 % 60;
      const int seconds = remaining / 1000 % 60;
      if (remaining <= 60000) {
        this->DrawTextAt(20, this->geometry.height - 50,
          FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0, 0, 12, 0xFF, 1, 0
        );
      } else {
        this->DrawTextAt(20, this->geometry.height - 50,
          FormatOwnedString(LookupMessage(MSI_SELECT_SHOP_POSITION), days, hours, minutes, seconds).c_str(), 0, 0, 12, 0, 1, 0
        );
      }

      if (this->buyingShop == 1) {
        this->notice->Move(170, this->geometry.height - 20);
      }
    }
  }
}

// OFFSET: 0x00857B60
// FUNCTION: fn_10763
bool UIEditCtrl3::ExtractItemLink(std::string& text, std::string& link, int& position, const char* source) {
  std::size_t close = text.find(g_itemLinkClose);
  if (close == std::string::npos) {
    return false;
  }

  do {
    text = text.substr(0, close);
    std::size_t open = text.find(g_itemLinkOpen);
    if (open == std::string::npos) {
      position += g_itemLinkClose.size();
      text = source;
      text = text.substr(position);
      return true;
    }

    close = text.find(g_itemLinkClose);
    if (open != std::string::npos) {
      for (;;) {
        position += open;
        text = text.substr(open + g_itemLinkOpen.size());
        open = text.find(g_itemLinkOpen);
        close = text.find(g_itemLinkClose);
        if (open == std::string::npos) {
          break;
        }

        position += g_itemLinkOpen.size();
      }

      link = text;
      text = source;
      std::string name = "";
      if (!this->LinkDisplayName(link, name)) {
        position += link.size() + g_itemLinkOpen.size() + g_itemLinkClose.size();
        text = text.substr(position);
        return true;
      }

      text = text.substr(position + link.size() + g_itemLinkOpen.size() + g_itemLinkClose.size());
    }
  } while (close != std::string::npos);

  return true;
}

// OFFSET: 0x00971D30
// FUNCTION: fn_13265
void UIMerchantShopMakeWnd::PrintSellList() {
  auto it = this->items.begin();
  char line[256];
  char name[256];
  char text[24];
  char price[32];
  char grouped[32];

  std::sprintf(line, "======================  SALE LIST  ======================");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x96FFFF, 0, 0, 0);
  std::memset(name, 0, sizeof(name));

  for (unsigned index = 0; index < this->items.size(); ++index, ++it) {
    const ITEM_INFO& item = *it;
    char* cursor = name;
    int capacity = 256;

    if (item.refiningLevel != 0) {
      cursor += item.Refine(cursor, capacity, 1);
    }

    if (item.HasCards()) {
      cursor += item.Prefixes(cursor, capacity, 1);
      cursor += item.GetIdDisplayName(cursor, capacity, 1);
      cursor += item.Suffixes(cursor, capacity, 1);
      item.Slots(cursor, capacity, 1);
    } else if (item.HasSpecialCard()) {
      cursor += item.Star(cursor, capacity, 1);
      cursor += item.Creator(cursor, capacity, 1);
      cursor += item.Element(cursor, capacity, 1);
      item.GetIdDisplayName(cursor, capacity, 1);
    } else {
      cursor += item.GetIdDisplayName(cursor, capacity, 1);
      item.Slots(cursor, capacity, 1);
    }

    std::sprintf(text, "%s ", this->priceEdits[index]->Text());
    g_session.IsNumberWithCommas(text);
    if (std::atoi(text) > 1000000000) {
      std::sprintf(price, "1,000,000,000");
    } else {
      const int value = std::atoi(text);
      std::sprintf(price, "%s", FormatGroupedInteger(value, grouped, 32));
    }

    std::sprintf(line, "    %s  :  %s  Zeny", name, price);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x5AFFFF, 0, 0, 0);

    const double rate = PriceCommission(_atoi64(text)) * 0.01;
    const std::int64_t commission = static_cast<std::int64_t>(_atoi64(text) * rate);
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("Commission : 1 EA = %s Zeny", FormatGroupedInteger(commission, line, 256)).c_str()), 0x5AFFFF, 0, 0, 0);
  }

  std::sprintf(price, "%s", FormatGroupedInteger(g_session.m_zeny, grouped, 32));
  std::sprintf(line, "                                          Current Zeny : %s Z", price);
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x78FFFF, 0, 0, 0);
  std::sprintf(line, "============================ ============================");
  g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(line), 0x96FFFF, 0, 0, 0);
}

// OFFSET: 0x00985160
// FUNCTION: fn_13363
void UIItemWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    if (item.flags[2] == 0) {
      item = LookupClientItemByIdentity(g_session.m_inventoryItems, item.identity[1]);
      mode->drag.BeginItem(g_windowMgr, 1, item, item.quantity, -1);

      if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
        if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
          if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
            static_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->InsertItemTag(item);
          } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
            static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
          } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
            static_cast<UIChatEditCtrl*>(ChatWindow()->m_chatEdit)->InsertItemLink(item);
          } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
            static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->InsertItemTag(item);
          } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
            static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
          }
        }
      }
    }
  }
}

// OFFSET: 0x00985610
// FUNCTION: fn_13367
void UIMerchantItemWnd::OnLBtnDown(int x, int y) {
  if (y < 17) {
    UIFrameWnd::OnLBtnDown(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.identity[1] > 0 && !g_windowMgr.merchantShopMakeWnd && item.flags[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindMerchantShopItem(item.identity[1]);
    mode->BeginDragItem(9, item, item.quantity, -1);

    if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.GetHoverWindow() && item.identified != 0) {
        if (g_windowMgr.GetHoverWindow()->geometry.id == 492) {
          static_cast<UINpcTextEditDialogWnd*>(g_windowMgr.npcTextEditDialogWnd)->InsertItemTag(item);
        } else if (g_windowMgr.GetHoverWindow()->geometry.id == 490) {
          static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
        } else if (g_windowMgr.GetHoverWindow()->geometry.id == 493) {
          static_cast<UIChatEditCtrl*>(ChatWindow()->m_chatEdit)->InsertItemLink(item);
        } else if (g_windowMgr.GetHoverWindow()->geometry.id == 491) {
          static_cast<UIRodexComposeWnd*>(g_windowMgr.rodexComposeWnd)->InsertItemTag(item);
        } else if (g_windowMgr.GetHoverWindow()->geometry.id == 494) {
          static_cast<UIChatEditCtrl*>(g_windowMgr.GetHoverWindow())->InsertItemLink(item);
        }
      }
    }
  }
}

// OFFSET: 0x00988590
// FUNCTION: fn_13385
void UISkillListWnd::OnMouseHover(int x, int y) {
  if (g_skillTooltipActive == 1) {
    const int elapsed = timeGetTime() - g_skillTooltipTick;
    if (elapsed > 1000 || elapsed < 0) {
      g_skillTooltipActive = 0;
      this->Invalidate();
    }
  }

  const int index = this->SkillAt(x, y);
  CSkillInfo skill = this->GetSkillByIndex(index);
  const char* name = skill.DisplayName();

  if (std::strlen(name) != 0) {
    int tooltipY = (index - this->scroll) * 32 + 17;
    int tooltipX = 112;
    if (this->TextWidth(name, 0, 0, 12, 0, 0) > this->geometry.width - 132 && x > tooltipX) {
      this->ToScreen(tooltipX, tooltipY);
      g_modeMgr.gameMode->ShowTooltip(name, tooltipX - 8, tooltipY, 0xFFFFFFFF, 0, 1);
    }
  }
}

// OFFSET: 0x0097EDE0
// FUNCTION: fn_13331
void UIItemSellWnd2::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - 38) / 32;
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawFittedItemName(column + 76, top + 29, &*it, 0, 0, 12, 0);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0097F790
// FUNCTION: fn_13333
void UIItemShopWnd2::Paint() {
  this->DrawTitleBar(false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - this->bottomBarHeight - 17) / 8 + ((this->geometry.height - this->bottomBarHeight - 17) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, left, false);
  }

  rows = (this->geometry.height - this->bottomBarHeight - 17) / 8 + ((this->geometry.height - this->bottomBarHeight - 17) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  g_skinInfo.PaletteChannels(2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - this->bottomBarHeight - 17, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - this->bottomBarHeight - 17) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - this->bottomBarHeight - 17) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->alternateButtonBar != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  const int slotRows = (this->geometry.height - this->extraBottomHeight - this->bottomBarHeight - 17) / 32;
  int slots = slotRows;
  for (int i = 0, y = 17; i < slots; ++i, y += 32) {
    DrawItemSlotBackground(*this, 40, y);
  }

  unsigned fillRed, fillGreen, fillBlue;
  g_skinInfo.PaletteChannels(2, 2, fillRed, fillGreen, fillBlue);
  this->FillRegion(72, 17, columns * 32 - 32, slotRows * 32, RGB(fillRed, fillGreen, fillBlue));

  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int column = visible % this->columns * 32;
    const int top = visible / this->columns * 32;
    if (this->hovered == index) {
      unsigned hoverRed, hoverGreen, hoverBlue;
      g_skinInfo.PaletteChannels(6, 2, hoverRed, hoverGreen, hoverBlue);
      this->FillRegion(column + 72, top + 19, 191, 28, RGB(hoverRed, hoverGreen, hoverBlue));
    }

    const int x = column + 44;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, top + 21, *it, 0, 0);
    this->DrawFittedItemName(column + 76, top + 29, &*it, 0, 0, 12, 0);

    if (0 < it->grade) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }
}

// OFFSET: 0x0098CBF0
// FUNCTION: fn_13422
int UIItemSellWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemSellWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ModeDragState& drag = mode->drag;
    ITEM_INFO item;
    item.Assign(drag);
    if (g_session.HasShopItemIdentity(drag.id)) {
      CSession* lists = &g_session;
      lists->AddStorageItem(item);
      lists->ReduceShopItemQuantity(item);
      if (g_windowMgr.itemShopWnd) {
        g_windowMgr.itemShopWnd->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
      }

      this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_COMMAND: {
    this->RecordResponse(static_cast<int>(a), b, c, d);
    CMode* mode = g_modeMgr.gameMode;
    if (a == ID_OK) {
      if (g_session.HasRequiredItems()) {
        mode->SendMsg(MM_REQ_SELL, 0, 0, 0, 0);
      }
    } else if (a != ID_CANCEL) {
      break;
    }

    g_windowMgr.CloseWindow(WID_ITEMSHOPWND);
    g_windowMgr.CloseWindow(WID_ITEMSELLWND);
    mode->SendMsg(MM_RELEASE_QUEST, 0, 0, 0, 0);
    g_session.ClearTradeItems();
    return 0;
  }
  case UM_RESIZE:
    if (a == 3) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 232) {
        width = 232;
      }
      if (height < 120) {
        height = 120;
      }

      const int newWidth = (width - 280) / 32 * 32 + 280;
      const int newHeight = (height - 120) / 32 * 32 + 120;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->Resize(newWidth, newHeight);
        this->columns = 1;
        this->visibleRows = (newHeight - 38) / 32;
        this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->UpdateScrollBar();
      }

      this->total->Move(12, this->geometry.height - 20);
      const int positions[2][2] = {
        {this->geometry.width - 106, this->geometry.height - 24},
        {this->geometry.width - 61, this->geometry.height - 24},
      };
      for (int i = 0; i < 2; ++i) {
        this->buttons[i]->Move(positions[i][0], positions[i][1]);
      }

      g_windowMgr.StoreSnapRectangle(this);
      return 0;
    }
    break;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    int totalPrice = 0;
    const int size = g_session.GetList4Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1730At(i);
      if (item.identity[1] != 0) {
        totalPrice += item.quantity * item.prices[1];
        this->items.push_back(item);
      }
    }

    char grouped[64];
    char text[80];
    std::sprintf(text, "Total : %s Zeny", FormatGroupedInteger(totalPrice, grouped, sizeof(grouped)));
    this->total->SetTextAutoWidth(text);
    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const auto* bytes = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    header = *reinterpret_cast<const ItemListChunkHeader*>(bytes);
    unsigned offset = 6;
    if (header.type == 20750) {
      while (static_cast<unsigned>(b) > offset) {
        header = *reinterpret_cast<const ItemListChunkHeader*>(bytes + offset);
        offset += 6;
        if (header.type == 20751) {
          UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMSHOPWND);
          if (window) {
            window->SendMsg(nullptr, UM_SETTITLE, 1, 0, 0, 0);
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }

          this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          break;
        }

        offset += header.size;
      }
    }
    break;
  }
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00980F50
// FUNCTION: fn_13338
void UIItemWnd::Paint() {
  this->DrawTitleBar(true, LookupMessage(MSI_ITEM), 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows;
  if (this->geometry.height > 120) {
    rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
    for (int i = 0, y = 17; i < rows; ++i, y += 8) {
      auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
      this->DrawBitmap(0, y, left, false);
    }
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int i = 0, y = 17; i < rows; ++i, y += 8) {
    auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, right, false);
  }

  unsigned red, green, blue;
  if (this->categoryFilter == 3 && g_session.m_blockPrivateTab == 1) {
    g_skinInfo.PaletteChannels(0, 8, red, green, blue);
    this->FillRegion(20, 17, this->geometry.width - 21, this->geometry.height - 38, RGB(red, green, blue));
  } else {
    g_skinInfo.PaletteChannels(2, 2, red, green, blue);
    this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
    this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  }

  if (this->filterWindow) {
    if (this->categoryFilter == 3 && g_session.m_blockPrivateTab == 1) {
      this->filterWindow->Show(1);
    } else {
      this->filterWindow->Show(0);
    }
  }
  if (this->dealLockButton) {
    if (this->categoryFilter == 3) {
      this->dealLockButton->Enable(1);
    } else {
      this->dealLockButton->Enable(0);
    }
  }
  if (this->filterButton) {
    if (this->categoryFilter == 3) {
      this->filterButton->Enable(1);
    } else {
      this->filterButton->Enable(0);
    }
  }

  this->DrawButtonBar(0, 0, 1);
  this->FillRegion(20, 17, 1, this->geometry.height - 38, 0xC0C0C0);
  this->FillRegion(18, this->geometry.height - 21, 2, 1, 0xFFFFFF);

  const int slotColumns = (this->geometry.width - 40) / 32;
  int slotRows = (this->geometry.height - 38) / 32;
  for (int i = 0, y = 17; i < slotRows; ++i, y += 32) {
    for (int j = 0, x = 40; j < slotColumns; ++j, x += 32) {
      if (this->categoryFilter == 3 && g_session.m_blockPrivateTab == 1) {
        auto* lock = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\itemwin_mid_lock.bmp")));
        this->DrawBitmap(x, y, lock, false);
      } else {
        DrawItemSlotBackground(*this, x, y);
      }
    }
  }

  int visible = 0;
  int index = 0;
  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int y = visible / this->columns * 32 + 17;
    const int x = visible % this->columns * 32 + 40;
    if (it->flags[1]) {
      if (this->categoryFilter == 3 && g_session.m_blockPrivateTab == 1) {
        auto* mark = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\itemwin_new_lock.bmp")));
        this->DrawBitmap(x, y, mark, false);
      } else {
        this->DrawNewItemMark(x, y);
      }
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  visible = 0;
  index = 0;
  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->damaged) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int x = visible % this->columns * 32 + 44;
    const int y = top + 21;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), x, y, *it, 0, 0);

    if (it->grade > 0) {
      this->DrawBitmap(
        x, top + 33,
        static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->grade).c_str()))
        ),
        true
      );
    }

    if (g_session.HasSwapEquipmentItem(it->identity[1])) {
      CTexture* background = g_texMgr.Load("유저인터페이스\\swap_equipment\\bg_change.bmp", 0, false, true, false);
      if (background) {
        this->geometry.surface->DrawScaledSurface(
          x, y, 24, 24, background, background->Width(), background->Height(), 1.0f, 1.0f, 0x22FFFFFF, false, 0, 0, 0, 0
        );
      }

      CTexture* icon = g_texMgr.Load("유저인터페이스\\swap_equipment\\ico_change.bmp", 0, false, true, false);
      if (icon) {
        this->geometry.surface->DrawScaledSurface(
          x, y, icon->Width(), icon->Height(), icon, icon->Width(), icon->Height(), 1.0f, 1.0f, 0xFFFFFFFF, false, 0, 0, 0, 0
        );
      }
    }

    if (++visible == this->visibleRows * this->columns) {
      break;
    }
  }

  const int count = g_session.GetList0Size() + g_session.CountDistinctEquippedItems() + g_session.CountDistinctCostumeItems();
  const std::uint32_t color = count >= g_session.extraInventorySlots + 100 ? 0xFF : 0x232323;
  this->DrawTextAt(35, this->geometry.height - 15, FormatOwnedString("%d", count).c_str(), 0, 0, 11, color, 0, 0);
  const int width = this->TextWidth(FormatOwnedString("%d", count).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(width + 36, this->geometry.height - 15, FormatOwnedString("/%d", g_session.extraInventorySlots + 100).c_str(), 0, 0, 11, 0, 0, 0
  );
}

// OFFSET: 0x00974910
// FUNCTION: fn_13308
void UIItemDropCntWnd::OnCreate(int, int) {
  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(80, 16);
  this->m_edit->Move(15, 22);
  this->m_edit->maximum = 6;
  this->m_edit->Background(232, 232, 232);
  this->AddChild(this->m_edit);

  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  char text[40];
  std::sprintf(text, "%d", gameMode->drag.quantity);
  this->m_edit->SetText(text);
  g_windowMgr.SetFocus(this->m_edit);
  this->m_edit->SelectAll();

  const char* dragName = gameMode->DragItemName();
  std::string name;
  if (dragName) {
    ITEM_INFO item;
    item.itemId = dragName;
    item.refiningLevel = 0;
    item.identified = gameMode->drag.identified;
    name = item.DisplayName(0);
  }

  auto* title = new UIStaticText();
  this->itemTitle = title;
  title->SetSize(100, 15);
  title->Move(15, 6);
  title->SetText(name.c_str(), 0);
  this->AddChild(title);

  const std::string prefix("유저인터페이스\\");
  const int x = this->geometry.width - 55;
  const int y = this->geometry.height - 32;
  auto* button = new UIBitmapButton();
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(x, y);
  button->SetId(184);
  this->AddChild(button);
  this->m_message0 = 184;
  this->geometry.state = 0;
}

// OFFSET: 0x00975050
// FUNCTION: fn_13309
void UIItemPurchaseWnd::OnCreate(int width, int height) {
  this->quantityEdit = new UIEditCtrl();
  this->quantityEdit->SetSize(64, 16);
  this->quantityEdit->maximum = 9;
  this->quantityEdit->Move(-100, -100);
  this->quantityEdit->Background(232, 232, 232);
  this->quantityEdit->SetText("0");
  this->AddChild(this->quantityEdit);
  g_windowMgr.SetFocus(this->quantityEdit);
  this->m_message0 = 271;

  this->title = new UIStaticText();
  this->title->SetSize(160, 15);
  this->title->Move(12, this->geometry.height - 21);
  this->AddChild(this->title);

  UIStaticText* caption = new UIStaticText();
  caption->SetSize(160, 15);
  caption->SetText(LookupMessage(MSI_ITEMS_FOR_BUY), 0);
  caption->Move(16, 2);
  caption->shadow = 1;
  this->AddChild(caption);

  this->resizer = new UIResizer();
  this->resizer->SetSize(16, 16);
  static_cast<UIResizer*>(this->resizer)->SetDirection(3);
  static_cast<UIResizer*>(this->resizer)->SetBitmapName("유저인터페이스\\btn_resize.bmp");
  this->resizer->Move(this->geometry.width - 15, this->geometry.height - 15);
  static_cast<UIResizer*>(this->resizer)->SetColors(0xFFFF00FF, 0xFFFF00FF);
  this->AddChild(this->resizer);

  UIScrollBar* scrollBar = new UIScrollBar();
  this->scrollBar = scrollBar;
  scrollBar->Configure(this->geometry.width - 15, 14, 1, this->geometry.height - 38);
  this->AddChild(this->scrollBar);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_buy", "btn_buy_a", "btn_buy_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int positions[2][2] = {{this->geometry.width - 106, this->geometry.height - 24}, {this->geometry.width - 61, this->geometry.height - 24}};
  int ids[2] = {184, 185};
  for (int index = 0; index < 2; ++index) {
    this->buttons[index] = new UIBitmapButton();
    UIBitmapButton* button = static_cast<UIBitmapButton*>(this->buttons[index]);
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    this->buttons[index]->SetSize(button->BitmapWidth(), button->BitmapHeight());
    this->buttons[index]->Move(positions[index][0], positions[index][1]);
    this->buttons[index]->SetId(ids[index]);
    this->AddChild(this->buttons[index]);
  }

  this->columns = 1;
  this->visibleRows = (height - 38) / 32;
  this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
}

// OFFSET: 0x0098DBF0
// FUNCTION: fn_13425
int UIItemStoreFindWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<ItemStoreFindWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    this->SendMsg(nullptr, UM_RESIZE, 3, this->saved->width, this->saved->height, 0);
    return 0;
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int size = g_session.GetList5Size();
    for (int i = 0; i < size; ++i) {
      ITEM_INFO item = g_session.GetItem1738At(i);
      if (item.identity[1] != 0) {
        this->items.push_back(item);
      }
    }

    if (g_session.m_itemAscendingOrder) {
      this->Sort();
    }
    if (g_session.m_itemDescendingOrder) {
      this->SortReversed();
    }
    if (!this->searchText.empty()) {
      this->FilterByName(std::string(this->searchText.c_str(), this->searchText.size()));
    }

    this->Invalidate();
    this->UpdateScrollBar();
    return 0;
  }
  case UM_COMMAND:
    if (a == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_ITEMSTOREFINDWND);
      static_cast<UIItemStoreWnd*>(g_windowMgr.LookupWindow(WID_ITEMSTOREWND))->m_searchWindow = 0;
      return 0;
    }
  case UM_RESIZE:
    if (a == 7) {
      int width = b <= 320 ? b : 320;
      int height = c <= 336 ? c : 336;
      if (width < 140) {
        width = 140;
      }
      if (height < 145) {
        height = 145;
      }

      const int newWidth = (width - 140) / 32 * 32 + 140;
      const int newHeight = (height - 160) / 32 * 32 + 145;
      if (newWidth != this->geometry.width || newHeight != this->geometry.height) {
        this->scroll = 0;
        this->Resize(newWidth, newHeight);
        this->resizeHandle->Move(this->geometry.width - 15, this->geometry.height - 15);
        this->scrollBar->Move(this->geometry.width - 15, 14);
        this->scrollBar->Resize(this->scrollBar->geometry.width, this->geometry.height - 38);
        this->closeButton->Move(this->geometry.width - 16, 3);
        this->visibleRows = (newHeight - 38) / 32;
        this->UpdateScrollBar();
      }

      g_windowMgr.StoreSnapRectangle(this);
    }
    return 0;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    switch (mode->drag.source) {
    case 1:
      mode->SendMsg(MM_ADD_ITEM_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
      break;
    case 9:
      mode->SendMsg(MM_MOVE_ITEM_FROM_CART_TO_STORE, mode->drag.id, mode->drag.quantity, 0, 0);
      break;
    }

    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    return 0;
  }
  case UM_RESET_ITEM_SORT:
    this->sorted = false;
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    return 0;
  case UM_SETCOUNTINFO: {
    char text[64];
    std::sprintf(text, "%d/%d", a, b);
    this->statusText.assign(text, std::strlen(text));
    this->Invalidate();
    return 0;
  }
  case UM_SORT_ITEMS:
    if (g_session.m_itemDefaultSort == true) {
      this->SendMsg(nullptr, UM_RESET_ITEM_SORT, 0, 0, 0, 0);
      return 0;
    }

    this->sorted = true;
    if (g_session.m_itemAscendingOrder == true) {
      this->Sort();
    } else if (g_session.m_itemDescendingOrder == true) {
      this->SortReversed();
    }
    break;
  case UM_SEARCH_ITEMS:
    if (this->searchText.empty()) {
      return 0;
    }
    if (!this->FilterByName(std::string(this->searchText.c_str(), this->searchText.size()))) {
      return 0;
    }
    break;
  default:
    return UIItemBaseWnd::SendMsg(sender, message, a, b, c, d);
  }

  this->Invalidate();
  this->UpdateScrollBar();
  return 0;
}

// OFFSET: 0x009948E0
// FUNCTION: fn_13438
void UIItemDropCntWnd::Show(int visible) {
  this->geometry.visible = visible;
  if (visible) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    char text[40];
    std::sprintf(text, "%d", gameMode->drag.quantity);
    this->m_edit->SetText(text);
    g_windowMgr.SetFocus(this->m_edit);
    this->m_edit->SelectAll();

    const char* dragName = gameMode->DragItemName();
    std::string name;
    if (dragName) {
      ITEM_INFO item;
      item.itemId = dragName;
      item.identified = gameMode->drag.identified;
      item.refiningLevel = 0;
      name = item.DisplayName(0);
    }

    this->itemTitle->SetTextFittedCodePage(name.c_str());
    this->Invalidate();
    g_windowMgr.StoreSnapRectangle(this);
  } else {
    if (this->m_edit == g_windowMgr.GetHoverWindow() && ChatWindow()) {
      ChatWindow()->SendMsg(nullptr, UM_GET_EDIT_FOCUS, 0, 0, 0, 0);
    }
    g_windowMgr.RemoveSnapRectangle(this);
  }
}

// OFFSET: 0x00992130
// FUNCTION: fn_13433
int UIMerchantShopMakeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  int error;
  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<MerchantShopMakeWndInfo*>(a);
    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }
    this->Move(this->saved->x, this->saved->y);
    break;
  case UM_DROP: {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    ITEM_INFO item;
    item.Assign(mode->drag);
    if (mode->drag.source == 20) {
      if (this->slotCount > g_session.GetList11Size()) {
        if (!g_session.AddMerchantPurchaseSavedItem(item, 1, 1)) {
          g_windowMgr.Confirm(LookupMessage(MSI_BUYINGSTORE_MAKEWND_DUPLICATEITEM), 0, true, false, 0, "Message", 280, 120, 0);
        } else {
          g_session.RemoveMerchantMyShopItem(item, 1, 1);
        }
      }
      int index = std::min(std::max(g_session.GetList11Size() - 1, 0), 4);
      if (this->priceEdits[index]) {
        if (this->priceEdits[index]->Length() == 0) {
          this->priceEdits[index]->SetText("0");
          g_windowMgr.SetFocus(this->priceEdits[index]);
          this->priceEdits[index]->SelectAll();
        } else if (this->quantityEdits[index]->Length() == 0) {
          this->quantityEdits[index]->SetText("0");
          g_windowMgr.SetFocus(this->quantityEdits[index]);
          this->quantityEdits[index]->SelectAll();
        }
      }
    } else if (mode->drag.source == 11) {
      if (item.quantity + g_session.GetItem1768Quantity(mode->drag.id) > 30000) {
        item.quantity = 30000 - g_session.GetItem1768Quantity(mode->drag.id);
        char text[200];
        std::sprintf(text, LookupMessage(MSI_LIMIT_SELL_ITEM), 30000);
        g_windowMgr.SendNotice(UIM_OPENCHATWND, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(text), 0xFF, 0, 0, 0);
      }
      if (item.quantity != 0) {
        const bool full = this->slotCount <= g_session.GetList11Size();
        bool movable;
        if (!full) {
          movable = g_session.GetItem1750Quantity(mode->drag.id) != 0;
        } else {
          movable = g_session.GetItem1750Quantity(mode->drag.id) != 0 && g_session.GetItem1768Quantity(mode->drag.id) != 0;
        }
        if (movable) {
          g_session.AddMerchantPurchaseSavedItem(item, 0, 1);
          g_session.RemoveMerchantMyShopItem(item, 0, 1);
        }
        int index = g_session.GetList11Size() - 1;
        bool fresh = true;
        std::string price;
        ITEM_INFO owned = g_session.FindMerchantPurchaseSavedItem(mode->drag.id);
        if (owned.identity[1] != 0) {
          index = 0;
          for (const ITEM_INFO& old : this->items) {
            if (old.identity[1] == owned.identity[1]) {
              fresh = false;
              price = this->priceEdits[index]->Text();
              break;
            }
            ++index;
          }
        }
        index = std::min(std::max(index, 0), 12);
        UIEditCtrl* edit = this->priceEdits[index];
        if (edit) {
          if (this->mode != 0) {
            if (edit->Length() == 0) {
              edit->SetText("0");
              g_windowMgr.SetFocus(edit);
              edit->SelectAll();
            }
          } else if (!full) {
            edit->Background(232, 232, 232);
            edit->disabled = 0;
            edit->Invalidate();
            edit->SetText(fresh ? "0" : price.c_str());
            g_windowMgr.SetFocus(edit);
            edit->SelectAll();
          }
        }
      }
    }
    mode->SendMsg(MM_CANCEL_DRAG, 0, 0, 0, 0);
    break;
  }
  case UM_REFRESHITEM: {
    this->items.erase(this->items.begin(), this->items.end());
    const int count = g_session.GetList11Size();
    for (int i = 0; i < count; ++i) {
      ITEM_INFO item = g_session.GetItem1768At(i);
      if (item.identity[1] > 0) {
        this->items.push_back(item);
      }
    }
    if (a > 0 && this->mode == 0) {
      int index = static_cast<int>(a) - 1;
      if (index <= this->editIndex) {
        --this->editIndex;
      }
      if (index == static_cast<int>(this->items.size())) {
        const PriceColors colors = PriceColor(0);
        this->priceEdits[index]->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
        this->priceEdits[index]->SetText("0");
        g_windowMgr.SetFocus(index == 0 ? this->titleEdit : this->priceEdits[index - 1]);
      } else {
        g_windowMgr.SetFocus(this->priceEdits[index]);
      }
      UIEditCtrl* edit = this->priceEdits[this->items.size()];
      edit->Background(182, 182, 182);
      edit->disabled = 1;
      edit->Invalidate();
      char text[24] = {};
      for (; index < this->slotCount - 1; ++index) {
        this->priceEdits[index]->SetText(this->priceEdits[index + 1]->Text());
        this->priceEdits[index + 1]->SetText("0");
        UIEditCtrl* edit = this->priceEdits[index];
        std::sprintf(text, "%s ", edit->Text());
        g_session.IsNumberWithCommas(text);
        const PriceColors colors = PriceColor(_atoi64(text));
        edit->TextColor(GetRValue(colors.primary), GetGValue(colors.primary), GetBValue(colors.primary), colors.secondary);
        UIEditCtrl* next = this->priceEdits[index + 1];
        std::sprintf(text, "%s ", next->Text());
        g_session.IsNumberWithCommas(text);
        const PriceColors nextColors = PriceColor(_atoi64(text));
        next->TextColor(GetRValue(nextColors.primary), GetGValue(nextColors.primary), GetBValue(nextColors.primary), nextColors.secondary);
      }
      this->InvalidateChildren();
    }
    this->UpdateLayout();
    this->Invalidate();
    break;
  }
  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->scroll;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->scroll = previous - this->minimumRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->scroll = previous + this->minimumRows - 1;
    } else {
      this->scroll = previous + static_cast<int>(a);
    }
    if (previous != this->scroll) {
      if (this->scrollBar) {
        const int count = static_cast<int>(this->items.size());
        const int maximum = std::max(count - this->maxVisibleRows, 0);
        this->scrollable = maximum != 0;
        UIScrollBar* bar = static_cast<UIScrollBar*>(this->scrollBar);
        if (maximum != 0) {
          bar->SetRange(maximum, this->maxVisibleRows, count);
          if (this->scroll < 0) {
            this->scroll = 0;
          }
          if (this->scroll > count - this->maxVisibleRows) {
            this->scroll = count - this->maxVisibleRows;
          }
          bar->SetPosition(this->scroll);
        } else {
          this->scroll = 0;
          bar->SetRange(0, this->maxVisibleRows, count);
        }
        bar->Invalidate();
      }
      this->Invalidate();
    }
    return this->scroll - previous;
  }
  case UM_REPLAY_FRAMED_DATA: {
    const unsigned char* data = reinterpret_cast<const unsigned char*>(a);
    ItemListChunkHeader header;
    std::memcpy(&header, data, sizeof(header));
    if (header.type == 21300 || header.type == 21350) {
      unsigned offset = 6;
      while (offset < static_cast<unsigned>(b)) {
        std::memcpy(&header, data + offset, sizeof(header));
        offset += 6;
        switch (header.type) {
        case 21301:
        case 21351: {
          UIWindow* window = g_windowMgr.EnsureWindow(header.type == 21301 ? WID_MERCHANTMIRRORITEMWND : WID_MERCHANTMIRRORITEMWND_MODE_1);
          if (window) {
            window->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
          }
          return 0;
        }
        default:
          offset += header.size;
          break;
        }
      }
    }
    break;
  }
  case UM_COMMAND:
    switch (a) {
    case ID_CANCEL: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (this->mode != 0) {
        if (this->itemsSet == 1) {
          if (this->suspended == 1) {
            this->SetSuspended(0);
          } else {
            mode->SendMsg(MM_CANCEL_BUYING_STORE_MAKE, 0, 0, 0, 0);
          }
        } else {
          mode->SendMsg(MM_REQ_OPEN_BUYING_STORE, 0, 0, 0, 0);
          g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND_MODE_1);
        }
      } else if (this->itemsSet == 1) {
        if (this->suspended == 1) {
          this->SetSuspended(0);
        } else {
          mode->SendMsg(MM_CANCEL_VENDING_MAKE, 0, 0, 0, 0);
        }
      } else {
        mode->SendMsg(MM_REQ_OPENSTORE, reinterpret_cast<std::intptr_t>(this->titleEdit->Text()), 0, 0, 0);
        g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
      }
      this->RecordResponse(185, static_cast<int>(b), static_cast<int>(c), static_cast<int>(d));
      break;
    }
    case ID_OK: {
      CMode* mode = g_modeMgr.GetGameMode();
      if (g_windowMgr.HasTitleAtActorCell(g_session.m_aid)) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SWAP_EQUIPITEM_EQUIPMENTSET)), 0xFF, 0, 0, 0);
        break;
      }
      if (g_windowMgr.chatRoomWnd || g_windowMgr.chatRoomMakeWnd) {
        g_windowMgr.Confirm(LookupMessage(MSI_CANT_OPEN_STORE_WHILE_CHAT), 0, true, false, 0, "Message", 280, 120, 0);
        if (this->itemsSet == 1) {
          if (this->mode == 1) {
            g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_BUYING_STORE_MAKE, 0, 0, 0, 0);
          } else {
            g_modeMgr.GetGameMode()->SendMsg(MM_CANCEL_VENDING_MAKE, 0, 0, 0, 0);
          }
        } else {
          g_windowMgr.CloseWindow(WID_MERCHANTSHOPMAKEWND);
        }
        break;
      }
      if (this->mode == 0 && !this->CommitPrice(-1)) {
        break;
      }
      BOOL freeItem = FALSE;
      BOOL highPrice = FALSE;
      if (this->mode != 0 && this->items.empty()) {
        error = 1724;
        goto showError;
      }
      auto current = this->items.begin();
      for (int i = 0; i < this->slotCount; ++i) {
        char text[24];
        std::sprintf(text, "%s ", this->priceEdits[i]->Text());
        if (!g_session.IsNumberWithCommas(text)) {
          error = 602;
          goto showError;
        }
        const std::int64_t price = std::max(std::min(_atoi64(text), static_cast<std::int64_t>(0x7FFFFFFF)), static_cast<std::int64_t>(0));
        if (this->mode == 0) {
          if (this->items.empty()) {
            error = 2494;
            goto showError;
          }
          ITEM_INFO item = g_session.GetItem1768At(i);
          if (static_cast<unsigned>(i) < this->items.size()) {
            if (item.identity[1] == 0) {
              return 0;
            }
            if (item.identified == 0) {
              error = 603;
              goto showError;
            }
            if (price == 0) {
              freeItem = TRUE;
            }
          }
          if (!g_session.m_skipPriceWarning && static_cast<unsigned>(i) < this->items.size() && price > 1000000000) {
            highPrice = TRUE;
          }
        } else {
          if (static_cast<unsigned>(i) >= this->items.size()) {
            break;
          }
          if (price == 0) {
            const std::string name = current->DisplayName(0);
            const std::string message = FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_MAKEWND_INPUTPRICE), name.c_str());
            g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
            return 0;
          }
          if (price > 99990000) {
            const std::string name = current->DisplayName(0);
            const std::string message = FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_MAKEWND_REINPUTPRICE), name.c_str());
            g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
            return 0;
          }
          ++current;
        }
      }
      if (this->mode == 0) {
        if (freeItem && g_windowMgr.Confirm(LookupMessage(MSI_YOU_HAVE_FREE_ITEM_ON_SHOP), 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
          break;
        }
        if (!g_session.m_skipPriceWarning && highPrice &&
            g_windowMgr.Confirm(LookupMessage(MSI_BASIC_EXP_MSG_INDONESIA), 2, true, false, 0, "Message", 280, 120, 0) != ID_YES) {
          break;
        }
      }
      if (this->mode != 0) {
        current = this->items.begin();
        for (int i = 0; i < this->slotCount; ++i) {
          char text[24];
          std::sprintf(text, "%s ", this->quantityEdits[i]->Text());
          if (!g_session.IsNumberWithCommas(text)) {
            error = 602;
            goto showError;
          }
          if (static_cast<unsigned>(i) >= this->items.size()) {
            break;
          }
          const int quantity = std::max(std::atoi(text), 0);
          if (quantity == 0) {
            const std::string name = current->DisplayName(0);
            const std::string message = FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_MAKEWND_INPUTNUM), name.c_str());
            g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
            return 0;
          }
          if (std::atoi(text) + current->quantity > 9999) {
            const std::string name = current->DisplayName(0);
            const std::string message = FormatOwnedString(LookupMessage(MSI_BUYINGSTORE_MAKEWND_REINPUTNUM), name.c_str());
            g_windowMgr.Confirm(message.c_str(), 0, true, false, 0, "Message", 280, 120, 0);
            return 0;
          }
          ++current;
        }
        const std::int64_t budget = _atoi64(this->zenyLimitEdit->Text());
        if (budget > g_session.m_zeny) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_SAVE_BUY_STORE_ITEMLIST)), 0x6464FF, 0, 0, 0);
          break;
        }
        if (budget > 0x7FFFFFFF) {
          g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(MSI_BUYINGSTORE_OVERFLOW_MONEY)), 0x6464FF, 0, 0, 0);
          break;
        }
      }
      if (this->titleEdit->Length() == 0) {
        error = 225;
        goto showError;
      }
      if (!g_forbiddenWords.Allows(this->titleEdit->Text())) {
        error = 14;
        goto showError;
      }
      for (int i = 0; i < this->slotCount; ++i) {
        char text[24];
        int price = 0;
        std::sprintf(text, "%s ", this->priceEdits[i]->Text());
        g_session.IsNumberWithCommas(text);
        if (this->priceEdits[i]->Length() != 0) {
          price = std::atoi(text);
        }
        if (g_serviceType == 2 && this->priceEdits[i]->Length() == 0 && this->priceEdits[i]->Text() != nullptr) {
          error = 602;
          goto showError;
        }
        g_session.SetMerchantPurchaseSavedPrice(i, price);
      }
      if (this->mode != 0) {
        for (int i = 0; i < this->slotCount; ++i) {
          char text[24];
          int quantity = 0;
          std::sprintf(text, "%s ", this->quantityEdits[i]->Text());
          g_session.IsNumberWithCommas(text);
          if (this->quantityEdits[i]->Length() != 0) {
            quantity = std::atoi(text);
          }
          if (g_serviceType == 2 && this->quantityEdits[i]->Length() == 0 && this->quantityEdits[i]->Text() != nullptr) {
            error = 602;
            goto showError;
          }
          g_session.SetItem1768Quantity(i, quantity);
        }
        if (this->itemsSet == 1) {
          this->SetSuspended(1);
        } else {
          const int budget = static_cast<int>(_atoi64(this->zenyLimitEdit->Text()));
          mode->SendMsg(MM_REQ_OPEN_BUYING_STORE, reinterpret_cast<std::intptr_t>(this->titleEdit->Text()), 1, budget, 0);
        }
      } else {
        std::int64_t total = g_session.m_zeny;
        for (std::int64_t i = 0; i < g_session.GetList11Size(); ++i) {
          ITEM_INFO item = g_session.GetItem1768At(static_cast<int>(i));
          const std::int64_t price = static_cast<int>(item.prices[1]);
          const std::int64_t quantity = item.quantity;
          total += price * quantity;
        }
        if (total > 0x7FFFFFFF) {
          error = 3826;
          goto showError;
        }
        if (this->itemsSet == 1) {
          this->SetSuspended(1);
        } else {
          mode->SendMsg(MM_REQ_OPENSTORE, reinterpret_cast<std::intptr_t>(this->titleEdit->Text()), 1, 0, 0);
        }
      }
      this->RecordText(0, this->titleEdit->Text());
      const std::string title(this->titleEdit->Text());
      g_session.m_storeInfoBackup.title = title;
      break;
    }
    case ID_CHECK:
      g_session.m_skipPriceWarning = !g_session.m_skipPriceWarning;
      break;
    case ID_RESTORE_STORE_INFO:
      this->actionButton->SetDisabled(1);
      this->actionButton->Invalidate();
      g_session.m_storeInfoBackup.Load(g_session.m_aid, g_session.m_gid, std::string(g_session.m_accountName.c_str()), this->mode);
      break;
    case ID_HELP:
      if (!g_windowMgr.LookupWindow(WID_NOTEWND)) {
        g_windowMgr.EnsureWindow(WID_NOTEWND);
      }
      break;
    }
    break;
  }
  return 0;
showError:
  g_windowMgr.Confirm(LookupMessage(error), 0, true, false, 0, "Message", 280, 120, 0);
  return 0;
}
