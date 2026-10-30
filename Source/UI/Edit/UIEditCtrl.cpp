#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Client/Replay.h"
#include "Client/ReplayPacketRecorder.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "UI/Shop/ItemDrawing.h"
#include "Utility/Text/FormatString.h"
#include "UI/Edit/EditTextState.h"
#include "UI/Edit/EditViewport.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Edit/UIEditCtrl_Currency.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Render/Renderer.h"
#include "Render/ScreenRectangle.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Buttons/UIBmpTextButton2.h"
#include "UI/Buttons/UINaviLinkButton.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Buttons/UISpriteButton.h"
#include "UI/Buttons/UITextureButton.h"
#include "UI/Item/UIItemPurchaseWnd.h"
#include "UI/Item/UIItemSellWnd.h"
#include "UI/Item/UIItemSellWnd2.h"
#include "UI/Item/UIItemShopWnd.h"
#include "UI/Item/UIItemShopWnd2.h"
#include "UI/Item/UIItemStoreFindWnd.h"
#include "UI/Item/UIItemStoreSubWnd.h"
#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Item/UIItemWnd.h"
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
#include "UI/Buttons/UICheckBox.h"
#include "UI/Dialogs/UIPasswordWnd.h"
#include "Client/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeDragState.h"
#include "UI/Shortcuts/ShortcutVisibility.h"
#include "UI/Shop/UIMerchantItemPurchaseMSGWnd.h"
#include "UI/Lists/UITabControl.h"
#include "UI/UIGradeEnchantWnd.h"
#include "UI/UIRefiningWnd.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Text/UIStaticText.h"
#include "UI/PriceCommission.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/GroupedInteger.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24862
bool g_vendingSaleAlarm;

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

class EditLinkLayout {
public:
  virtual void Update() = 0;

protected:
  ~EditLinkLayout() = default;
};

struct EditPainting {
  std::string& text;
  const int& anchor;
  const int& cursor;
  const int& mask;
  const int& firstVisible;
  const int& y;
  const int& invalidatesParent;
  const std::uint32_t& color;
  const std::uint32_t& secondColor;
  const bool& focused;
  const bool& chatInputFocused;
  const bool& caretEnabled;
  const std::uint32_t& tick;
  const int& candidateCursor;
  const int& width;
  const int& height;
  const int& screenWidth;
  const int& screenHeight;
  CLanguage& input;
  const InputCharset& charset;
  EditViewport& viewport;
  CDC& surface;
  FontCache& fonts;
  const TextLocale& locale;
  const BitmapImage& colorChip;
  UIWindow* const& parent;
  UIWindow* const& candidateWindow;
  UIWindow* const& readingWindow;
  const int& absoluteX;
  const int& absoluteY;
  const std::list<UIWindow*>& linkWindows;
  EditLinkLayout& links;

  bool alternate = false;
  const std::string* placeholder = nullptr;
  std::uint32_t placeholderColor = 0;
  bool placeholderEnabled = false;

  void Draw();
};
// INLINED
void EditPainting::Draw() {
  TextContext context(this->surface);
  context.SetFont(0, 12, 0, 0);
  const auto measure = [&](const char* text, int bytes) { return context.Measure(text, bytes ? bytes : static_cast<int>(std::strlen(text))).cx; };
  int x = 3;

  if (this->invalidatesParent == 0 && this->firstVisible != 0) {
    x = (3 - measure(this->text.c_str(), this->firstVisible));
  }

  const auto draw = [&](const std::string& value) {
    const bool placeholder = this->alternate && !this->focused && this->placeholderEnabled && this->secondColor == 0xFFFFFFFFu;
    context.SetColor(placeholder ? this->placeholderColor : this->color);
    const std::string& displayed = placeholder && value.empty() ? *this->placeholder : value;
    context.Draw(x, this->y, displayed.c_str(), static_cast<int>(displayed.size()));

    if (this->secondColor != 0xFFFFFFFFu) {
      context.SetColor(this->secondColor);
      context.Draw((x - 1), this->y, value.c_str(), static_cast<int>(value.size()));
    }
  };
  const auto highlight = [&](int left, int extent) {
    const int start = std::max(left, 3);
    const int adjusted = left < 3 ? (extent + (left - 3)) : extent;

    if (start < this->width && adjusted > 0) {
      std::uint32_t pixel = 0x00FF0000u;

      if (this->colorChip.width > 6 && this->colorChip.height > 2) {
        pixel = this->colorChip.pixels[2 * this->colorChip.width + 6];
      }

      const std::uint32_t color = ((pixel >> 16) & 255) | (pixel & 0xFF00) | ((pixel & 255) << 16);
      RECT rectangle{start, 0, std::min((start + adjusted), this->width), this->height};
      this->surface.Fill(&rectangle, color);
    }
  };
  const auto moveCandidate = [&](UIWindow* window, int offset) {
    if (!window) {
      return;
    }

    int left = (offset + this->absoluteX);
    int top = ((this->height + 6) + this->absoluteY);

    if ((window->geometry.width + left) > this->screenWidth) {
      left = (this->screenWidth - window->geometry.width);
    }

    if ((window->geometry.height + top) > this->screenHeight) {
      top = (((top - 12) - this->height) - window->geometry.height);
    }

    window->Move(left, top);
  };
  std::string display;

  if (this->focused) {
    this->viewport.Synchronize();
    unsigned begin = static_cast<unsigned>(std::min(this->anchor, this->cursor));
    unsigned end = static_cast<unsigned>(std::max(this->anchor, this->cursor));

    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      const char* prefix = this->alternate ? this->input.candidate.c_str() : g_serverLanguage->Prefix(false);
      display = prefix;
      display += this->text;
      const auto prefixBytes = static_cast<unsigned>(std::strlen(prefix));
      begin += prefixBytes;
      end += prefixBytes + static_cast<unsigned>(this->candidateCursor);
    }

    const int left = begin == 0 ? 0 : measure(display.c_str(), static_cast<int>(begin));
    const int right = end == 0 ? 0 : measure(display.c_str(), static_cast<int>(end));

    if (this->input.cursor != this->input.anchor) {
      highlight((x + left), (right - left));
      draw(display);
    } else if (this->candidateCursor != 0) {
      moveCandidate(this->candidateWindow, left);
      moveCandidate(this->readingWindow, right);

      if (((this->tick / 500u) & 1u) == 0) {
        highlight((x + left), std::max((right - left), 2));
      }

      draw(display);
    } else {
      draw(display);

      if (((this->tick / 500u) & 1u) == 0 && (this->alternate ? this->caretEnabled : (!this->chatInputFocused || this->caretEnabled))) {
        context.SetColor(this->color);
        context.Draw(((x + left) - 2), this->y, "|", 1);
      }
    }
  } else {
    if (this->mask != 0) {
      display.assign(this->text.size(), static_cast<char>(this->mask));
    } else {
      display = this->text;
    }

    draw(display);

    if (this->invalidatesParent != 0 && this->parent) {
      this->parent->Invalidate();
    }
  }

  if (std::any_of(this->linkWindows.begin(), this->linkWindows.end(), [](const UIWindow* window) { return window->geometry.id == 438; })) {
    this->links.Update();
  }
}
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

// OFFSET: 0x0084FAE0
// FUNCTION: fn_10663
UIEditCtrl::UIEditCtrl(UIWindowContext& windows, UIEditContext& edit)
    : UIWindow(windows, 0), anchor(0), cursor(0), mask(0), maximum(255), flat(1), red(255), green(255), blue(255), borderRed(0), borderGreen(0), borderBlue(0),
      textColor(0), secondColor(0xFFFFFFFF), firstVisible(0), textY(0), invalidatesParent(0), disabled(0), unknown_00F0(0), topPadding(0), preserveInput(0),
      m_edit(&edit) {
  this->text.reserve(255);
}

// OFFSET: 0x008517E0
// OFFSET: 0x00852980
// FUNCTION: fn_10694
UIEditCtrl::~UIEditCtrl() = default;

// OFFSET: 0x005C3210
// FUNCTION: fn_03552
bool UIEditCtrl::Slot05() {
  return this->disabled == 0;
}

// OFFSET: 0x0085CCA0
// FUNCTION: fn_10807
void UIEditCtrl::Paint() {
  const auto component = [&](int value) { return static_cast<unsigned char>(this->disabled != 0 && value > 30 ? value - 30 : value); };
  const unsigned color = component(this->red) | (component(this->green) << 8) | (component(this->blue) << 16);
  this->textY = 0;

  if (this->geometry.surface) {
    if (this->flat == 0) {
      RECT rectangle{0, 0, this->geometry.width, this->geometry.height};
      const unsigned border = static_cast<unsigned char>(this->borderRed) | (static_cast<unsigned char>(this->borderGreen) << 8) |
                              (static_cast<unsigned char>(this->borderBlue) << 16);
      this->geometry.surface->Fill(&rectangle, border);
      rectangle = {1, 1, this->geometry.width - 1, this->geometry.height - 1};
      this->geometry.surface->Fill(&rectangle, color);
    } else {
      this->geometry.surface->Fill(nullptr, color);
    }
  }

  this->textY = this->topPadding + 2;
  this->PaintText();
}

// OFFSET: 0x0085AAB0
// FUNCTION: fn_10783
void UIEditCtrl::Slot23() {
  this->m_edit->input.ResetInput();
  this->m_edit->input.ResetInput();
  this->m_edit->input.Insert(this->text.c_str());
  this->m_edit->password = this->mask != 0;
  this->SelectAll();
}

// OFFSET: 0x0085E3B0
// FUNCTION: fn_10826
void UIEditCtrl::Slot24(int) {
  this->Synchronize();
  this->Invalidate();
}

// OFFSET: 0x0085EC20
// FUNCTION: fn_10842
void UIEditCtrl::Slot25(int, int) {
  if (this->disabled == 0) {
    this->m_edit->focus.Set(this);
  }
}

// OFFSET: 0x0085F390
// FUNCTION: fn_10860
void UIEditCtrl::Slot28(int x, int y) {
  if (this->disabled != 0) {
    return;
  }

  this->Slot27(x, y);

  if (!this->tooltip.empty()) {
    int left = 10, top = -20;

    for (const UIWindow* window = this; window; window = window->parent) {
      left += window->geometry.x;
      top += window->geometry.y;
    }

    this->m_edit->buttons.services.Tooltip(this->tooltip.c_str(), left, top, -1, false, true);
  }
}

// OFFSET: 0x008641C0
// FUNCTION: fn_10914
int UIEditCtrl::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 153) {
    this->InsertLink(reinterpret_cast<const char*>(a));
  } else if (message == 98 && c > 0) {
    UIWindow* window = this->m_edit->windows.EnsureWindow(12);

    if (window) {
      window->SendMsg(nullptr, 24, c, 0, 0, 0);
    }
  }

  UIWindow::SendMsg(sender, message, a, b, c, d);

  if (message == 0 && this->preserveInput == 0) {
    this->m_edit->input.ResetInput();
  }

  return 0;
}

// OFFSET: 0x005C44E0
// FUNCTION: fn_03561
int UIEditCtrl::Slot46() {
  return 2;
}

// OFFSET: 0x008680D0
// FUNCTION: fn_10952
void UIEditCtrl::SetText(const char* value) {
  this->text.assign(value, std::strlen(value));
  this->Trim();

  if (this->m_edit->focus.window == this) {
    this->m_edit->input.ResetInput();
    this->m_edit->input.ResetInput();
    this->m_edit->input.Insert(this->text.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0085A590
void UIEditCtrl::SelectAll() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor, this->cursor, this->maximum, focused, this->m_edit->input, g_textCodePage};
  state.SelectAll();
}

// OFFSET: 0x00856980
// FUNCTION: fn_10755
void UIEditCtrl::PaintText() {
  this->PaintTextWithPolicy(false, nullptr, 0, false);
}

// INLINED
void UIEditCtrl::PaintTextWithPolicy(bool alternate, const std::string* placeholder, std::uint32_t placeholderColor, bool placeholderEnabled) {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor, this->cursor, this->maximum, focused, this->m_edit->input, g_textCodePage};
  EditViewport viewport{
    this->text,
    this->anchor,
    this->cursor,
    this->firstVisible,
    this->geometry.width,
    g_textCodePage,
    this->m_edit->input,
    state,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale
  };
  UIWindow* parent = this->parent;
  int left = 0, top = 0;

  for (const UIWindow* window = this; window; window = window->parent) {
    left += window->geometry.x;
    top += window->geometry.y;
  }

  const bool chat = this->m_edit->input.chatInput == this || this->m_edit->otherChatInput == this;
  const DWORD tick = timeGetTime();
  LinkLayout layout(*this);
  EditPainting painting{
    this->text,
    this->anchor,
    this->cursor,
    this->mask,
    this->firstVisible,
    this->textY,
    this->invalidatesParent,
    this->textColor,
    this->secondColor,
    focused,
    chat,
    this->m_edit->input.suggestionsEnabled,
    tick,
    this->m_edit->candidateCursor,
    this->geometry.width,
    this->geometry.height,
    this->m_edit->screenWidth,
    this->m_edit->screenHeight,
    this->m_edit->input,
    this->m_edit->charset,
    viewport,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale,
    this->m_edit->colorChip,
    parent,
    this->m_edit->candidateWindow,
    this->m_edit->readingWindow,
    left,
    top,
    this->links,
    layout
  };

  painting.alternate = alternate;
  painting.placeholder = placeholder;
  painting.placeholderColor = placeholderColor;
  painting.placeholderEnabled = placeholderEnabled;
  painting.Draw();
}

// OFFSET: 0x00862FB0
void UIEditCtrl::Synchronize() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor, this->cursor, this->maximum, focused, this->m_edit->input, g_textCodePage};
  EditViewport viewport{
    this->text,
    this->anchor,
    this->cursor,
    this->firstVisible,
    this->geometry.width,
    g_textCodePage,
    this->m_edit->input,
    state,
    *this->geometry.surface,
    this->m_edit->buttons.fonts,
    this->m_edit->buttons.locale
  };
  viewport.Synchronize();
}

// OFFSET: 0x00868450
void UIEditCtrl::Trim() {
  const bool focused = this->m_edit->focus.window == this;
  EditTextState state{this->text, this->anchor, this->cursor, this->maximum, focused, this->m_edit->input, g_textCodePage};
  state.Trim();
}

// OFFSET: 0x00858D50
// FUNCTION: fn_10768
int UIEditCtrl::Length() const {
  return static_cast<int>(this->m_edit->focus.window == this ? this->m_edit->input.text.size() : this->text.size());
}

// OFFSET: 0x00858D20
// FUNCTION: fn_10767
const char* UIEditCtrl::Text() {
  if (this->m_edit->focus.window == this) {
    this->Synchronize();
  }

  return this->text.c_str();
}

// OFFSET: 0x00858D80
// FUNCTION: fn_10769
std::string UIEditCtrl::LinkedText() {
  return std::string(this->linkedText.c_str(), this->linkedText.size());
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
bool HasNaviLinkTag(const std::string& text) {
  return text.find(g_editNaviTagBegin) != std::string::npos || text.find(g_editNaviTagEnd) != std::string::npos;
}

// OFFSET: 0x00865DD0
// FUNCTION: fn_10937
void UIEditCtrl::Background(int red, int green, int blue) {
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

// OFFSET: 0x00868570
// FUNCTION: fn_10960
void UIEditCtrl::TextColor(std::uint32_t primary, std::uint32_t secondary) {
  this->secondColor = secondary;
  this->textColor = primary;
  this->Invalidate();
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
  const int extent = x < 3 ? width + (x - 3) : width;
  const int left = std::max(x, 3);
  const int right = this->geometry.width;

  if (left < right && extent > 0) {
    unsigned red, green, blue;
    SkinPaletteChannels(this->m_edit->buttons.resources, 6, 2, red, green, blue);
    this->FillRegion(left, y, left + extent > right ? right - left : extent, 14, RGB(red, green, blue));
  }
}

// OFFSET: 0x00857040
// FUNCTION: fn_10759
void UIEditCtrl::DrawSelection(int x, int width) {
  const int extent = x < 3 ? width + (x - 3) : width;
  const int left = std::max(x, 3);
  const int right = this->geometry.width;

  if (left < right && extent > 0) {
    unsigned red, green, blue;
    SkinPaletteChannels(this->m_edit->buttons.resources, 6, 2, red, green, blue);
    this->FillRegion(left, 0, left + extent > right ? right - left : extent, this->geometry.height, RGB(red, green, blue));
  }
}

// INLINED
UIEditCtrlWheel::UIEditCtrlWheel(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {
}

// OFFSET: 0x009701B0
UIEditCtrlWheel::~UIEditCtrlWheel() = default;

// OFFSET: 0x0098A240
// FUNCTION: fn_13408
void UIEditCtrlWheel::Slot35(int value) {
  if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x0086A550
// FUNCTION: fn_10977
void UIEditCtrl::MoveCandidateWindow(int x) {
  UIWindow* window = g_windowMgr.window_03A4;
  if (window == nullptr) {
    return;
  }

  int left = x;
  int top = this->geometry.height + 6;
  left += this->geometry.x;
  top += this->geometry.y;
  for (const UIWindow* node = this->parent; node != nullptr; node = node->parent) {
    left += node->geometry.x;
    top += node->geometry.y;
  }

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
  UIWindow* window = g_windowMgr.window_03AC;
  if (window == nullptr) {
    return;
  }

  int left = x;
  int top = this->geometry.height + 6;
  left += this->geometry.x;
  top += this->geometry.y;
  for (const UIWindow* node = this->parent; node != nullptr; node = node->parent) {
    left += node->geometry.x;
    top += node->geometry.y;
  }

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
    const BmpTextButtonLabel& label = this->labels[index];

    if (label.text.size() == 0) {
      continue;
    }

    int x = label.x;
    int y = label.y;
    int shadowX = 0;
    int shadowY = 0;

    if (x == -1000) {
      const int width = this->TextWidth(this->m_buttonContext->fonts, this->m_buttonContext->locale, label.text.c_str(), label.text.size(), 0, label.height, label.bold, 0);
      x = static_cast<int>((this->geometry.width - width) * 0.5);
    }

    if (y == -1000) {
      const int height = this->TextHeight(this->m_buttonContext->fonts, this->m_buttonContext->locale, label.text.c_str(), label.text.size(), 0, label.height, label.bold, 0);
      y = static_cast<int>((this->geometry.height - height) * 0.5);
    }

    std::uint32_t color = label.color;
    std::uint32_t shadowColor = 0xE6E6E6;

    switch (this->geometry.unknown_0030) {
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

    if (label.shadow == 1) {
      this->DrawTextAt(this->m_buttonContext->fonts, this->m_buttonContext->locale, shadowX, shadowY, label.text.c_str(), 0, 0, label.height, shadowColor, label.bold, 0);
    }

    this->DrawTextAt(this->m_buttonContext->fonts, this->m_buttonContext->locale, x, y, label.text.c_str(), 0, 0, label.height, color, label.bold, 0);
  }
}

// OFFSET: 0x008572F0
// FUNCTION: fn_10761
void UITextureButton::VisitChildren() {
  CTexture* texture = nullptr;

  switch (this->geometry.unknown_0030) {
  case 0:
    texture = g_texMgr.Load(
      (this->unknown_011C == 1 && this->unknown_011D == 1 ? this->pressedTexture : this->normalTexture).c_str(), 0, false, true, false
    );
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
    g_renderer->primitives, g_renderer->queues, static_cast<float>(this->geometry.x), static_cast<float>(this->geometry.y),
    static_cast<float>(texture->Width()), static_cast<float>(texture->Height()), 0xFFFFFFFF, texture, g_renderer->fog.supported != 0, g_depthStart,
    g_depthEnd
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
void UINoticeMessageBox_LockSellItem::Slot15(int, int) {
  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
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
void UIItemWnd::Slot18() {
}

// OFFSET: 0x00896BC0
// FUNCTION: fn_11518
void UIEditCtrl_Custom::Synchronize() {
  const int previousCursor = this->cursor;
  CLanguage& input = this->m_edit->input;
  input.CopyDisplayText(this->text);
  input.Selection(this->anchor, this->cursor);
  this->Trim();

  const auto length = static_cast<unsigned>(this->text.size());
  bool clamped = false;

  if (static_cast<unsigned>(this->cursor) > length || static_cast<unsigned>(this->anchor) > length) {
    this->anchor = std::min<unsigned>(this->anchor, length);
    this->cursor = std::min<unsigned>(this->cursor, length);
    input.Select(this->anchor, this->cursor);
    clamped = true;
  }

  if (this->cursor == 0) {
    this->firstVisible = 0;
    return;
  }

  std::string visible;

  if (static_cast<unsigned>(std::abs(input.cursor - input.anchor)) >= this->text.size()) {
    this->firstVisible = 0;
  }

  FontCache& fonts = this->m_edit->buttons.fonts;
  const TextLocale& locale = this->m_edit->buttons.locale;
  const int available = this->geometry.width - 12;

  if (this->cursor > previousCursor || clamped) {
    int before = 0;

    if (this->firstVisible != 0) {
      before = this->TextWidth(fonts, locale, this->text.c_str(), this->firstVisible, 0, 12, this->unknown_011C, 0);
    }

    const int position = this->TextWidth(fonts, locale, this->text.c_str(), this->cursor, 0, 12, this->unknown_011C, 0);

    if (position + 3 - before > available) {
      const char* end = this->text.c_str() + this->text.size();
      const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);

      while (this->TextWidth(fonts, locale, next, 0, 0, 12, this->unknown_011C, 0) + 3 > available && next < end) {
        next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
      }

      const auto tail = static_cast<unsigned>(std::strlen(next));
      const auto scrolled = static_cast<unsigned>(this->firstVisible + this->cursor - previousCursor);
      this->firstVisible = std::min<unsigned>(this->cursor - tail, scrolled);
    }
  } else if (this->cursor < previousCursor) {
    if (this->TextWidth(fonts, locale, this->text.c_str(), this->cursor, 0, 12, this->unknown_011C, 0) + 3 < available && this->firstVisible > this->cursor) {
      this->firstVisible += this->cursor - previousCursor;
    }

    this->firstVisible = std::max(this->firstVisible, 0);
  }
}

// OFFSET: 0x009727F0
// FUNCTION: fn_13275
void UIItemWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 1:
    if (reinterpret_cast<UITabControl*>(drag->unknown_038C) == this->tabs) {
      PACKET_CZ_INVENTORY_TAB packet;
      packet.packetType = ID_CZ_INVENTORY_TAB;
      packet.index = static_cast<unsigned short>(drag->id);
      packet.favorite = this->tabs->TabAt(x, y) != 3;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
    }
    break;
  case 2:
  case 17:
  case 18:
  case 24:
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  case 6:
  case 9:
  case 15:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 6, 0, 0, 0);
    return;
  case 14: {
    UIWindow* target = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0378);

    if (target) {
      target->SendMsg(nullptr, 75, reinterpret_cast<std::intptr_t>(drag->name.c_str()), 0, 0, 0);
    }
    break;
  }
  case 28: {
    UIWindow* window = g_windowMgr.LookupWindow(295);

    if (window) {
      static_cast<UIRefiningWnd*>(window)->Reset();
    }
    break;
  }
  case 33: {
    UIWindow* window = g_windowMgr.LookupWindow(343);

    if (window) {
      static_cast<UIGradeEnchantWnd*>(window)->ResetItem();
    }
    break;
  }
  }

  g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00989080
// FUNCTION: fn_13396
void UIItemStoreFindWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.unknown_01D4) {
        if (mode->unknown_05CC[2]) {
          mode->unknown_05CC[2] = 0;
          mode->SendMsg(56, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.unknown_02F4 && mode->unknown_05CC[2]) {
        mode->unknown_05CC[2] = 0;
        mode->SendMsg(78, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.unknown_01D4) {
        mode->SendMsg(333, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989210
// FUNCTION: fn_13397
void UIItemStoreSubWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.unknown_01D4) {
        if (mode->unknown_05CC[2]) {
          mode->unknown_05CC[2] = 0;
          mode->SendMsg(56, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.unknown_02F4 && mode->unknown_05CC[2]) {
        mode->unknown_05CC[2] = 0;
        mode->SendMsg(78, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.unknown_01D4) {
        mode->SendMsg(333, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x009893A0
// FUNCTION: fn_13398
void UIItemStoreWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.unknown_01D4) {
        if (mode->unknown_05CC[2]) {
          mode->unknown_05CC[2] = 0;
          mode->SendMsg(56, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
        }
      } else if (g_windowMgr.unknown_02F4 && mode->unknown_05CC[2]) {
        mode->unknown_05CC[2] = 0;
        mode->SendMsg(78, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      mode->SendMsg(333, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989C40
// FUNCTION: fn_13403
void UIMerchantItemWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (!g_windowMgr.LookupWindow(10011) && !g_windowMgr.unknown_02FC) {
        if (g_windowMgr.window_0288) {
          if (mode->unknown_05CC[2]) {
            mode->unknown_05CC[2] = 0;
            mode->SendMsg(79, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
          }
        } else if (g_windowMgr.unknown_01D4 && mode->unknown_05CC[2]) {
          mode->unknown_05CC[2] = 0;
          mode->SendMsg(77, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
        }
      }
    } else if (HIBYTE(GetAsyncKeyState(VK_SHIFT))) {
      if (g_windowMgr.unknown_01D4) {
        mode->SendMsg(335, static_cast<int>(item.unknown_0000[1]), item.quantity, 0, 0);
      }
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989DF0
// FUNCTION: fn_13404
void UIMerchantMirrorItemWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.unknown_02FC) {
        item = g_session.FindItem1750(item.unknown_0000[1]);
        ItemDescriptions descriptions{g_itemDescriptions};
        mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 11, item, item.quantity, -1);
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02FC)->SendMsg(nullptr, 38, 0, 0, 0, 0);
      } else if (g_windowMgr.unknown_040C) {
        item = g_session.FindItem1750(item.unknown_0000[1]);
        ItemDescriptions descriptions{g_itemDescriptions};
        mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 20, item, item.quantity, -1);
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_040C)->SendMsg(nullptr, 38, 0, 0, 0, 0);
      }
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00989FE0
// FUNCTION: fn_13405
void UIMerchantShopMakeWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (HIBYTE(GetAsyncKeyState(VK_MENU))) {
      if (g_windowMgr.unknown_02F8) {
        item = g_session.FindItem1768(item.unknown_0000[1]);
        ItemDescriptions descriptions{g_itemDescriptions};
        gameMode->drag.BeginItem(this->m_frameContext->windows, descriptions, 10, item, item.quantity, -1);
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_02F8)->SendMsg(nullptr, 38, 0, 0, 0, 0);
        InputFocus focus{g_windowMgr.hoverWindow};
        focus.Set(nullptr);
      } else if (g_windowMgr.unknown_0410) {
        item = g_session.FindItem1768(item.unknown_0000[1]);
        ItemDescriptions descriptions{g_itemDescriptions};
        gameMode->drag.BeginItem(this->m_frameContext->windows, descriptions, 19, item, item.quantity, -1);
        reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0410)->SendMsg(nullptr, 38, 0, 0, 0, 0);
      }
    } else {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
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
  UIButtonContext& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, "", 0);

  if (this->geometry.height == 17) {
    return;
  }

  int rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* left = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_left.bmp")));
    this->DrawBitmap(0, y, &left->image, false);
  }

  rows = (this->geometry.height - 38) / 8 + ((this->geometry.height - 38) % 8 != 0);
  for (int y = 17; rows > 0; --rows, y += 8) {
    const auto* right = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\itemwin_right.bmp")));
    this->DrawBitmap(this->geometry.width - right->image.width, y, &right->image, false);
  }

  unsigned red, green, blue;
  SkinPaletteChannels(controls.resources, 2, 2, red, green, blue);
  this->FillRegion(20, 17, (this->geometry.width - 60) % 32, this->geometry.height - 38, RGB(red, green, blue));
  this->FillRegion(20, (this->geometry.height - 38) / 32 * 32 + 17, this->geometry.width - 24, (this->geometry.height - 38) % 32, RGB(red, green, blue));
  this->DrawButtonBar(this->unknown_00B8 != 0 ? 2 : 0, 0, 1);

  const int columns = (this->geometry.width - 40) / 32;
  int slots = (this->geometry.height - 38) / 32;
  for (int y = 17; slots > 0; --slots, y += 32) {
    for (int column = 0, x = 40; column < columns; ++column, x += 32) {
      DrawItemSlotBackground(*this, x, y);
    }
  }

  ItemDescriptions descriptions{g_itemDescriptions};
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index < this->scroll * this->columns) {
      continue;
    }

    if (g_session.IsPetEgg(std::atoi(it->itemId.c_str())) && it->unknown_005D) {
      continue;
    }

    const int top = visible / this->columns * 32;
    const int x = visible % this->columns * 32 + 44;

    if (this->showQuantity != 0) {
      DrawItemWithQuantity(*this, controls.resources, descriptions, controls.fonts, controls.locale, x, top + 21, *it, 0, 0);
    } else {
      DrawItemIcon(*this, controls.resources, descriptions, x, top + 21, *it, 0);
    }

    if (it->unknown_0088 > 0) {
      this->DrawBitmap(
        x, top + 33,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", it->unknown_0088).c_str()))
        )->image,
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

  if (this->unknown_011C == 57) {
    this->DrawButtonBar(3, 0, 1);
  }

  if (this->duration > 0) {
    const int remaining = this->duration - (timeGetTime() - this->startTick);

    if (remaining > 0) {
      UIButtonContext& controls = *this->m_controls;
      const int days = remaining / 86400000;
      const int hours = remaining / 3600000 % 24;
      const int minutes = remaining / 60000 % 60;
      const int seconds = remaining / 1000 % 60;

      if (this->unknown_0100 == 1) {
        if (remaining <= 60000) {
          this->DrawTextAt(
            controls.fonts, controls.locale, 12, this->geometry.height - 42, FormatOwnedString(LookupMessage(2927), days, hours, minutes, seconds).c_str(), 0,
            0, 11, 0xFF, 1, 0
          );
        } else {
          this->DrawTextAt(
            controls.fonts, controls.locale, 12, this->geometry.height - 42, FormatOwnedString(LookupMessage(2927), days, hours, minutes, seconds).c_str(), 0,
            0, 11, 0, 1, 0
          );
        }
      } else if (remaining <= 60000) {
        this->DrawTextAt(
          controls.fonts, controls.locale, 20, this->geometry.height - 50, FormatOwnedString(LookupMessage(2927), days, hours, minutes, seconds).c_str(), 0, 0,
          12, 0xFF, 1, 0
        );
      } else {
        this->DrawTextAt(
          controls.fonts, controls.locale, 20, this->geometry.height - 50, FormatOwnedString(LookupMessage(2927), days, hours, minutes, seconds).c_str(), 0, 0,
          12, 0, 1, 0
        );
      }
    }
  }
}

// OFFSET: 0x0097A670
// FUNCTION: fn_13319
void UIMerchantItemPurchaseMSGWnd::Slot15(int, int) {
  this->title = new UIStaticText(*this->m_context, *this->m_controls);
  this->title->SetSize(100, 15);
  this->title->SetTextFitted(LookupMessage(2326));
  this->title->Move(16, 2);
  this->title->shadow = 1;
  this->AddChild(this->title);

  this->buyButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy.bmp", 0, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_a.bmp", 1, 0);
  this->buyButton->SetBitmap("유저인터페이스\\btn_buy_b.bmp", 2, 0);
  this->buyButton->SetSize(this->buyButton->BitmapWidth(), this->buyButton->BitmapHeight());
  this->buyButton->Move(this->geometry.width - 106, this->geometry.height - 24);
  this->buyButton->SetId(184);
  this->AddChild(this->buyButton);

  this->cancelButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel.bmp", 0, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_a.bmp", 1, 0);
  this->cancelButton->SetBitmap("유저인터페이스\\btn_cancel_b.bmp", 2, 0);
  this->cancelButton->SetSize(this->cancelButton->BitmapWidth(), this->cancelButton->BitmapHeight());
  this->cancelButton->Move(this->geometry.width - 61, this->geometry.height - 24);
  this->cancelButton->SetId(185);
  this->AddChild(this->cancelButton);

  this->firstLine = new UIStaticText(*this->m_context, *this->m_controls);
  this->firstLine->SetSize(200, 15);
  this->firstLine->SetText(LookupMessage(2327), 0);
  this->firstLine->Move(10, 20);
  this->AddChild(this->firstLine);

  this->secondLine = new UIStaticText(*this->m_context, *this->m_controls);
  this->secondLine->SetSize(200, 15);
  this->secondLine->Move(10, 40);
  this->AddChild(this->secondLine);

  this->thirdLine = new UIStaticText(*this->m_context, *this->m_controls);
  this->thirdLine->SetSize(200, 15);
  this->thirdLine->SetText(LookupMessage(2329), 0);
  this->thirdLine->Move(10, 60);
  this->AddChild(this->thirdLine);
}

// OFFSET: 0x00981990
// FUNCTION: fn_13340
void UIMerchantItemPurchaseMSGWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, false, "", 0);
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x009827C0
// FUNCTION: fn_13343
void UIMerchantItemWnd::Paint() {
  UIItemBaseWnd::Paint();

  UIButtonContext& controls = *this->m_controls;
  const auto* count = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\icon_num.bmp")));
  this->DrawBitmap(10, this->geometry.height - 22, &count->image, true);

  const std::uint32_t countColor = g_session.cartCount >= g_session.cartMaxCount ? 0xFF : 0x232323;
  this->DrawTextAt(
    controls.fonts, controls.locale, 23, this->geometry.height - 19, FormatOwnedString("%d", g_session.cartCount).c_str(), 0, 0, 11, countColor, 0, 0
  );
  const int countWidth = this->TextWidth(controls.fonts, controls.locale, FormatOwnedString("%d", g_session.cartCount).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(
    controls.fonts, controls.locale, countWidth + 24, this->geometry.height - 19, FormatOwnedString("/%d", g_session.cartMaxCount).c_str(), 0, 0, 11, 0, 0, 0
  );

  const auto* weight = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\icon_weight.bmp")));
  this->DrawBitmap(75, this->geometry.height - 20, &weight->image, true);

  const std::uint32_t weightColor = g_session.cartWeight >= g_session.cartMaxWeight ? 0xFF : 0x232323;
  this->DrawTextAt(
    controls.fonts, controls.locale, 90, this->geometry.height - 19, FormatOwnedString("%d", g_session.cartWeight).c_str(), 0, 0, 11, weightColor, 0, 0
  );
  const int weightWidth = this->TextWidth(controls.fonts, controls.locale, FormatOwnedString("%d", g_session.cartWeight).c_str(), 0, 0, 11, 0, 0);
  this->DrawTextAt(
    controls.fonts, controls.locale, weightWidth + 91, this->geometry.height - 19, FormatOwnedString("/%d", g_session.cartMaxWeight).c_str(), 0, 0, 11, 0, 0, 0
  );
}

// OFFSET: 0x009834F0
// FUNCTION: fn_13345
void UINoticeMessageBox_LockSellItem::Paint() {
  const auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\inventory\\popup_itemDeal_lock.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawCenteredText(0, 5, LookupMessage(2951), this->geometry.width, 0, 12, 0xFFFFFF, 0);
}

// OFFSET: 0x00983F80
// FUNCTION: fn_13347
void UIItemSellWnd2::Slot26(int x, int y) {
  if (g_windowMgr.unknown_0064 == 0) {
    return;
  }

  UIWindow* target = g_windowMgr.unknown_0254;

  if (target) {
    this->Slot25(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->m_recorder->RecordDrop(x, y, mode->drag, 160);
    target->Slot10(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x00983FF0
// FUNCTION: fn_13348
void UIItemSellWnd::Slot26(int x, int y) {
  if (g_windowMgr.unknown_0064 == 0) {
    return;
  }

  UIWindow* target = g_windowMgr.unknown_0250;

  if (target) {
    this->Slot25(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->m_recorder->RecordDrop(x, y, mode->drag, 22);
    target->Slot10(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x00984130
// FUNCTION: fn_13351
void UIItemStoreFindWnd::Slot26(int, int) {
  this->SendMsg(nullptr, 6, 232, 0, 0, 0);
}

// OFFSET: 0x00984150
// FUNCTION: fn_13352
void UIItemStoreSubWnd::Slot26(int, int) {
  this->SendMsg(nullptr, 6, 232, 0, 0, 0);
}

// OFFSET: 0x00985380
// FUNCTION: fn_13364
void UIMerchantItemMyShopWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIItemBaseWnd::Slot25(x, y);
  }
}

// OFFSET: 0x009886C0
// FUNCTION: fn_13386
void UIItemWnd::Slot19() {
  if (this->unknown_0128) {
    this->unknown_0128->Slot19();
  }
}

// OFFSET: 0x009886D0
// FUNCTION: fn_13387
void UIMerchantItemMyShopWnd::Slot19() {
  this->Invalidate();
}

// OFFSET: 0x009886E0
// FUNCTION: fn_13388
void UIMerchantItemShopWnd::Slot19() {
  this->Invalidate();
}

// OFFSET: 0x00988C20
// FUNCTION: fn_13390
void UINoticeMessageBox_LockSellItem::Slot19() {
  if (this->startTick + this->duration < timeGetTime()) {
    this->SendMsg(this, 6, 201, 0, 0, 0);
  }
}

// OFFSET: 0x00988D50
// FUNCTION: fn_13392
void UIItemSellWnd2::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988E10
// FUNCTION: fn_13393
void UIItemSellWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988ED0
// FUNCTION: fn_13394
void UIItemShopWnd2::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x009898A0
// FUNCTION: fn_13400
void UIMerchantItemMyShopWnd::Slot32(int x, int y) {
  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0098A220
// FUNCTION: fn_13407
void UISkillListWnd::Slot33(int, int) {
  this->SendMsg(nullptr, 49, 0, 0, 0, 0);
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
void UISkillListWnd::Slot32(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->SkillAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }
}

// OFFSET: 0x00974560
// FUNCTION: fn_13302
bool UISkillListWnd::Slot03() {
  return true;
}

// OFFSET: 0x00974900
// FUNCTION: fn_13307
void UIItemBaseWnd::Slot15(int, int) {
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
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
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
  if (x >= 20 && x < 52 && y > 16 && y < this->visibleRows * 32 + 17) {
    int visible = 0;
    int index = 0;

    for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
      if (index >= this->columns * this->scroll) {
        if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
          if (visible == this->columns * ((y - 17) / 32) + (x - 20) / 32) {
            return *it;
          }

          ++visible;
        }
      }
    }

    return ITEM_INFO(0);
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973D60
// FUNCTION: fn_13295
ITEM_INFO UIItemStoreSubWnd::GetItemAt(int x, int y) {
  if (x >= 20 && x < 52 && y > 16 && y < this->visibleRows * 32 + 17) {
    int visible = 0;
    int index = 0;

    for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
      if (index >= this->columns * this->scroll) {
        if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
          if (visible == this->columns * ((y - 17) / 32) + (x - 20) / 32) {
            return *it;
          }

          ++visible;
        }
      }
    }

    return ITEM_INFO(0);
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973EA0
// FUNCTION: fn_13296
ITEM_INFO UIItemStoreWnd::GetItemAt(int x, int y) {
  if (x >= 40 && x < 72 && y > 16 && y < this->visibleRows * 32 + 17) {
    int visible = 0;
    int index = 0;

    for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
      if (index >= this->columns * this->scroll) {
        if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
          if (visible == this->columns * ((y - 17) / 32) + (x - 40) / 32) {
            return *it;
          }

          ++visible;
        }
      }
    }

    return ITEM_INFO(0);
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00973FE0
// FUNCTION: fn_13297
ITEM_INFO UIItemWnd::GetItemAt(int x, int y) {
  if (x < 40 || x >= this->geometry.width - 20 || y < 17 || y >= this->visibleRows * 32 + 17) {
    return ITEM_INFO(0);
  }

  const int offset = x - 40;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->columns * this->scroll) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
        if (visible == this->columns * ((y - 17) / 32) + offset / 32) {
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

  const int offset = y - 44;
  int visible = 0;
  int index = 0;

  for (auto it = this->items.begin(); it != this->items.end(); ++it, ++index) {
    if (index >= this->scroll * this->columns) {
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
        if (visible == this->columns * (offset / 42)) {
          if (x < 35) {
            return *it;
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
  CSkillInfo result(0);
  int position = 0;

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
  if (x >= 40 && x < 72 && y > 16 && y < this->visibleRows * 32 + 17) {
    int visible = 0;
    int index = 0;

    for (auto it = this->skills.begin(); it != this->skills.end(); ++it, ++index) {
      if (index >= this->scroll * this->columns) {
        if (visible == this->columns * ((y - 17) / 32)) {
          return *it;
        }

        ++visible;
      }
    }

    return CSkillInfo(0);
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
  return this->unknown_018A >= 0 && this->unknown_018C >= 0;
}

// OFFSET: 0x00974590
// FUNCTION: fn_13304
void UIMerchantShopMakeWnd::SendShopRequest() {
  if (this->mode == 1) {
    g_modeMgr.GetGameMode()->SendMsg(298, reinterpret_cast<int>(this->unknown_00CC->Text()), std::atoi(this->unknown_0128->Text()), 0, 0);
    return;
  }

  g_modeMgr.GetGameMode()->SendMsg(296, reinterpret_cast<int>(this->unknown_00CC->Text()), 0, 0, 0);
}

// OFFSET: 0x00974820
// FUNCTION: fn_13306
void UIMerchantShopMakeWnd::AddActionButton() {
  this->unknown_0168 = new UIBasicButton(*this->m_context, *this->m_controls);
  this->unknown_0168->Configure(60, LookupMessage(3637));
  this->unknown_0168->Move(this->geometry.width - 156, this->geometry.height - 24);
  this->unknown_0168->SetTooltip(LookupMessage(3638));
  this->unknown_0168->SetId(560);
  this->AddChild(this->unknown_0168);
}

// OFFSET: 0x00994800
// FUNCTION: fn_13436
void UIMerchantShopMakeWnd::SetSelection(short row, short column) {
  this->unknown_018A = row;
  this->unknown_018C = column;
}

// OFFSET: 0x00995320
// FUNCTION: fn_13444
void UIItemBaseWnd::Slot11() {
}

// OFFSET: 0x00995330
// FUNCTION: fn_13445
void UIItemPurchaseWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995370
// FUNCTION: fn_13446
void UIItemSellWnd2::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009953B0
// FUNCTION: fn_13447
void UIItemSellWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009953F0
// FUNCTION: fn_13448
void UIItemShopWnd2::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995430
// FUNCTION: fn_13449
void UIItemShopWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995470
// FUNCTION: fn_13450
void UIItemStoreFindWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009954B0
// FUNCTION: fn_13451
void UIItemStoreSubWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x009954F0
// FUNCTION: fn_13452
void UIItemStoreWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00995530
// FUNCTION: fn_13453
void UIItemWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->unknown_0010 = this->unknown_0110;
    this->saved->unknown_0014 = this->unknown_010C;
    this->saved->unknown_0018 = this->unknown_0100;
    this->saved->unknown_001C = this->unknown_0104;
    this->saved->unknown_0020 = this->unknown_0108;
  }
}

// OFFSET: 0x009955C0
// FUNCTION: fn_13454
void UIMerchantItemMyShopWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995600
// FUNCTION: fn_13455
void UIMerchantItemPurchaseWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995640
// FUNCTION: fn_13456
void UIMerchantItemShopWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995680
// FUNCTION: fn_13457
void UIMerchantItemWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x009956C0
// FUNCTION: fn_13458
void UIMerchantMirrorItemWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995700
// FUNCTION: fn_13459
void UIMerchantShopMakeWnd::Slot11() {
  if (this->saved) {
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
  }
}

// OFFSET: 0x00995740
// FUNCTION: fn_13460
void UISkillListWnd::Slot11() {
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
  UIWindow* window = reinterpret_cast<UIWindow*>(this->mode == 1 ? g_windowMgr.unknown_0410 : g_windowMgr.unknown_02F8);
  if (!window) {
    return;
  }

  if (suspend == 1) {
    this->suspended = 1;
    this->Show(0);
    window->Show(0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(2930)), 0xFFFF, 0, 0, 0);
    return;
  }

  this->suspended = 0;
  this->Show(1);
  window->Show(1);
  this->unknown_018A = -1;
  this->unknown_018C = -1;
}

// OFFSET: 0x00994B90
// FUNCTION: fn_13439
void UIMerchantItemMyShopWnd::OpenShop() {
  this->shopOpen = 1;
  if (!this->actionButton) {
    this->actionButton = new UIBasicButton(*this->m_context, *this->m_controls);
    this->actionButton->Configure(42, LookupMessage(2925));
    this->actionButton->Move(this->geometry.width - this->actionButton->geometry.width - 66, this->geometry.height - 24);
    this->actionButton->SetId(506);
    this->AddChild(this->actionButton);
  }

  g_vendingSaleAlarm = false;
  this->saleAlarm->Show(0);

  const int previousHeight = this->unknown_011C;
  this->unknown_011C = 57;
  this->Resize(this->geometry.width, this->geometry.height - previousHeight + 57);
  this->SendMsg(nullptr, 14, 7, this->geometry.width, this->geometry.height, 0);
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
    it->unknown_0074[1] = 0;
  }

  if (UIWindow* window = g_windowMgr.LookupWindow(307)) {
    window->SendMsg(nullptr, 6, 286, 194, 0, 0);
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
  case 4:
  case 11:
  case 12:
  case 13:
  case 14:
    return 2;
  case 5:
  case 8:
  case 9:
  case 15:
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

  const int previousHeight = this->unknown_0118;
  this->unknown_0118 = 57;
  this->Resize(this->geometry.width, this->geometry.height - previousHeight + 57);
  this->SendMsg(nullptr, 14, 3, this->geometry.width, this->geometry.height, 0);
}

// OFFSET: 0x009971E0
// FUNCTION: fn_13483
void UIItemStoreFindWnd::SetSearchText(std::string text) {
  this->searchText = text;
}

// OFFSET: 0x00971490
// FUNCTION: fn_13263
int UIMerchantShopMakeWnd::CommitPrice(int next) {
  if (this->editIndex == -1 || g_session.unknown_5C00[1] == 0) {
    this->editIndex = next;
    return 1;
  }

  UIEditCtrl* edit = this->priceEdits[this->editIndex];
  WindowDialogs& dialogs = reinterpret_cast<WindowDialogs&>(g_windowMgr);
  InputFocus focus{g_windowMgr.hoverWindow};
  char text[24];
  std::sprintf(text, "%s ", edit->Text());
  if (!g_session.IsNumberWithCommas(text)) {
    dialogs.Confirm(LookupMessage(602), 0, true, false, 0, "Message", 280, 120, 0);
    edit->SetText("0");
    focus.Set(edit);
    edit->SelectAll();
    return 0;
  }

  std::int64_t price = _atoi64(text);
  if (price <= 0x7FFFFFFF && price < 0) {
    price = 0;
  } else if (price > 0x7FFFFFFF || price > 1000000000) {
    if (dialogs.Confirm(LookupMessage(2467), 2, true, false, 0, "Message", 280, 120, 0) != 187) {
      edit->SetText("0");
      focus.Set(edit);
      edit->SelectAll();
      return 0;
    }

    price = 1000000000;
    edit->SetText(_itoa(1000000000, text, 10));
    this->Invalidate();
  }

  if (price >= std::atoi(LookupMessage(2476))) {
    ITEM_INFO item = g_session.GetItem1768At(this->editIndex);
    char name[128];
    int capacity = 128;
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.Name(item, name, capacity, 0);
    std::string message = FormatOwnedString(LookupMessage(2473), name);

    const int high = static_cast<int>(price / std::atoi(LookupMessage(2474)));
    if (high > 0) {
      message += FormatOwnedString(LookupMessage(2475), high);
    }

    const int low = static_cast<int>(price % std::atoi(LookupMessage(2474))) / std::atoi(LookupMessage(2476));
    if (low > 0) {
      message += FormatOwnedString(LookupMessage(2477), low);
    }

    message += std::string(LookupMessage(2478));
    if (dialogs.Confirm(message.c_str(), 2, true, false, 0, "Message", 280, 120, 0) != 187) {
      edit->SetText("0");
      focus.Set(edit);
      edit->SelectAll();
      return 0;
    }
  }

  const PriceColors colors = PriceColor(price);
  edit->TextColor(colors.primary, colors.secondary);
  FormatGroupedInteger(price, text, sizeof(text));
  edit->SetText(text);

  this->editIndex = next;
  if (next == -1) {
    focus.Set(nullptr);
    return 1;
  }

  this->priceEdits[next]->TextColor(0, 0xFFFFFFFF);
  focus.Set(this->priceEdits[next]);
  this->priceEdits[next]->SelectAll();
  return 1;
}

// OFFSET: 0x009719C0

// OFFSET: 0x00972100

// OFFSET: 0x00972120

// OFFSET: 0x009721F0

// OFFSET: 0x009722A0
// FUNCTION: fn_13269
void UIItemSellWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 3) {
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1 || g_windowMgr.unknown_0064 != 0) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.window_01EC->SendMsg(nullptr, 13, 2, 0, 0, 0);
}

// OFFSET: 0x00972B50
// FUNCTION: fn_13277
void UIMerchantItemShopWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source != 13) {
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }

  if (drag->quantity == 1) {
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
  g_windowMgr.window_01EC->SendMsg(nullptr, 13, 11, 0, 0, 0);
}

// OFFSET: 0x0089CAE0
// FUNCTION: fn_11591
void UIEditCtrl_Custom::Trim() {
  const auto length = this->text.size();
  if (static_cast<int>(length) <= this->maximum) {
    return;
  }

  const auto codePage = static_cast<WORD>(g_textCodePage);
  const char* next = CharNextExA(codePage, this->text.c_str(), 0);
  while (length - std::strlen(next) <= static_cast<unsigned>(this->maximum)) {
    next = CharNextExA(codePage, next, 0);
  }

  const char* previous = CharPrevExA(codePage, this->text.c_str(), next, 0);
  this->text.resize(length - std::strlen(previous));

  CLanguage& input = this->m_edit->input;
  input.ResetInput();
  input.ResetInput();
  input.Insert(this->text.c_str());

  while (this->text.size() < static_cast<unsigned>(this->maximum)) {
    this->text.append(" ", 1);
  }
}

// OFFSET: 0x00972350
// FUNCTION: fn_13270
void UIItemShopWnd2::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
  case 5:
    if (this->unknown_00F8 == 0) {
      return;
    }

    if (drag->quantity == 1 || g_windowMgr.unknown_0064 != 0) {
      this->SendMsg(nullptr, 38, 1, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 15, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00972410
// FUNCTION: fn_13271
void UIItemShopWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 4:
    if (this->unknown_00F8 != 0) {
      return;
    }

    if (drag->quantity == 1) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 3, 0, 0, 0);
    return;
  case 5:
    if (this->unknown_00F8 == 0) {
      return;
    }

    if (drag->quantity == 1 || g_windowMgr.unknown_0064 != 0) {
      this->SendMsg(nullptr, 38, 1, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 4, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x009729C0
// FUNCTION: fn_13276
void UIMerchantItemPurchaseWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  if (drag->source == 12) {
    switch (drag->type) {
    case 4:
    case 5:
    case 8:
    case 9:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
      if (g_session.GetItem1760Quantity(drag->id)) {
        mode->SendMsg(18, 0, 0, 0, 0);
        g_windowMgr.SendNotice(3, 0, 0, 0, 0, 0);
        g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(119)), 0xFF, 0, 0, 0);
        return;
      }

      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    default:
      reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
      g_windowMgr.window_01EC->SendMsg(nullptr, 13, 12, 0, 0, 0);
      return;
    }
  }

  if (drag->source == 22) {
    if (drag->quantity > 1) {
      reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
      g_windowMgr.window_01EC->SendMsg(nullptr, 13, 17, 0, 0, 0);
      return;
    }

    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  }

  mode->SendMsg(18, 0, 0, 0, 0);
}

// OFFSET: 0x00972CE0
// FUNCTION: fn_13279
void UIMerchantMirrorItemWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  CMode* mode = g_modeMgr.GetGameMode();
  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 10:
  case 21:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    if (this->unknown_00F4 == 2) {
      g_windowMgr.window_01EC->SendMsg(nullptr, 13, 18, 0, 0, 0);
      return;
    }

    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 10, 0, 0, 0);
    return;
  case 19:
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  default:
    mode->SendMsg(18, 0, 0, 0, 0);
    return;
  }
}

// OFFSET: 0x00972DB0
// FUNCTION: fn_13280
void UIMerchantShopMakeWnd::Slot10(int x, int y, std::intptr_t value) {
  if (g_windowMgr.LookupWindow(10011)) {
    UIWindow::Slot10(x, y, value);
    return;
  }

  const ModeDragState* drag = reinterpret_cast<const ModeDragState*>(value);
  switch (drag->source) {
  case 11:
    if (drag->quantity == 1) {
      this->SendMsg(nullptr, 38, 0, 0, 0, 0);
      return;
    }

    reinterpret_cast<WindowVisibility&>(g_windowMgr).Show(15, 1);
    g_windowMgr.window_01EC->SendMsg(nullptr, 13, 9, 0, 0, 0);
    return;
  case 20:
    this->SendMsg(nullptr, 38, 0, 0, 0, 0);
    return;
  default:
    g_modeMgr.GetGameMode()->SendMsg(18, 0, 0, 0, 0);
    return;
  }
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
      if (!g_session.IsPetEgg(std::atoi(it->itemId.c_str())) || !it->unknown_005D) {
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
void UIItemShopWnd2::Slot26(int x, int y) {
  if (this->unknown_00F8 == 0 || g_windowMgr.unknown_0064 == 0) {
    return;
  }

  UIWindow* target = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_0260);

  if (target) {
    this->Slot25(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->m_recorder->RecordDrop(x, y, mode->drag, 161);
    target->Slot10(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x009840C0
// FUNCTION: fn_13350
void UIItemShopWnd::Slot26(int x, int y) {
  if (this->unknown_00F8 == 0 || g_windowMgr.unknown_0064 == 0) {
    return;
  }

  UIWindow* target = reinterpret_cast<UIWindow*>(g_windowMgr.unknown_025C);

  if (target) {
    this->Slot25(x, y);
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    this->m_recorder->RecordDrop(x, y, mode->drag, 24);
    target->Slot10(x, y, reinterpret_cast<std::intptr_t>(&mode->drag));
  }
}

// OFFSET: 0x009844E0
// FUNCTION: fn_13354
void UISkillListWnd::Slot26(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (this->hovered != -1) {
    CSkillInfo skill = this->GetSkillByIndex(this->hovered);

    if (skill.available && skill.level && skill.unknown_0014) {
      this->SendMsg(nullptr, 58, reinterpret_cast<std::intptr_t>(&skill), 0, 0, 0);
    }
  }
}

// OFFSET: 0x00985510
// FUNCTION: fn_13366
void UIMerchantItemShopWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  if (this->unknown_010C == 0) {
    const int previous = this->hovered;
    this->hovered = this->GetItemIndexAt(x, y);

    if (previous != this->hovered) {
      this->Invalidate();
    }

    ITEM_INFO item = this->GetItemByIndex(this->hovered);

    if (item.unknown_0000[1] > 0) {
      ItemDescriptions descriptions{g_itemDescriptions};
      static_cast<CGameMode*>(g_modeMgr.GetGameMode())->drag.BeginItem(this->m_frameContext->windows, descriptions, 12, item, 1, -1);
    }
  }
}

// OFFSET: 0x00988C60
// FUNCTION: fn_13391
void UIItemPurchaseWnd::Slot32(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x00988F90
// FUNCTION: fn_13395
void UIItemShopWnd::Slot32(int x, int y) {
  const int previous = this->hovered;
  this->hovered = this->GetItemIndexAt(x, y);

  if (previous != this->hovered) {
    this->Invalidate();
  }

  ITEM_INFO item = this->GetItemByIndex(this->hovered);

  if (item.unknown_0000[1] > 0) {
    g_modeMgr.GetGameMode();
    this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0098A260
// FUNCTION: fn_13409
void UIItemBaseWnd::Slot35(int value) {
  if (this->unknown_00E4) {
    int step = this->visibleRows - 1;
    if (step > 3) {
      step = 3;
    }

    this->SendMsg(this->scrollBar, 7, value < 0 ? step : -step, 0, 0, 0);
  }
}

// OFFSET: 0x0098A2B0
// FUNCTION: fn_13410
void UIItemStoreFindWnd::Slot35(int value) {
  if (this->unknown_00E4) {
    int step = this->visibleRows - 1;
    if (step > 3) {
      step = 3;
    }

    this->SendMsg(this->scrollBar, 7, value < 0 ? step : -step, 0, 0, 0);
  }
}

// OFFSET: 0x0098A300
// FUNCTION: fn_13411
void UIItemStoreSubWnd::Slot35(int value) {
  if (this->unknown_00E4) {
    int step = this->visibleRows - 1;
    if (step > 3) {
      step = 3;
    }

    this->SendMsg(this->scrollBar, 7, value < 0 ? step : -step, 0, 0, 0);
  }
}

// OFFSET: 0x0098A350
// FUNCTION: fn_13412
void UIMerchantShopMakeWnd::Slot35(int value) {
  if (this->unknown_0144) {
    int step = this->unknown_0140 - 1;
    if (step > 3) {
      step = 3;
    }

    this->SendMsg(this->scrollBar, 7, value < 0 ? step : -step, 0, 0, 0);
  }
}

// OFFSET: 0x00993A20
// FUNCTION: fn_13434
int UINoticeMessageBox_LockSellItem::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message != 6) {
    UIFrameWnd::SendMsg(sender, message, a, b, c, d);
    return 0;
  }

  if (a == 201 && this->parent) {
    this->parent->SendMsg(this, 6, 201, 0, 0, 0);
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
      this->m_edit->input.Selection(anchor, cursor);

      if (anchor == cursor) {
        if (cursor <= 2) {
          if (this->text[1] == ',') {
            this->m_edit->input.Select(0, 2);
          }
        } else if (this->text[cursor - 1] == ',') {
          this->m_edit->input.Select(cursor - 1, cursor - 1);
        }
      }
    } else if (a == VK_DELETE) {
      int anchor;
      int cursor;
      this->m_edit->input.Selection(anchor, cursor);

      if (anchor == cursor) {
        if (cursor == 0) {
          if (this->text[1] == ',') {
            this->m_edit->input.Select(0, 2);
          }
        } else if (this->text[cursor] == ',') {
          this->m_edit->input.Select(cursor + 1, cursor + 1);
        }
      }
    }
  }

  return UIEditCtrl2::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009845A0
// FUNCTION: fn_13355
void UIItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1720ByItemId(item.itemId.c_str());
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 4, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984700
// FUNCTION: fn_13356
void UIItemSellWnd2::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.unknown_0000[1] > 0) {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1730(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 5, item, item.quantity, -1);
  }
}

// OFFSET: 0x009848E0
// FUNCTION: fn_13357
void UIItemSellWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.unknown_0000[1] > 0) {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1730(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 5, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984AC0
// FUNCTION: fn_13358
void UIItemShopWnd2::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] == 0) {
    item = this->GetItemByIndex(this->hovered);

    if (item.unknown_0000[1] > 0) {
      this->m_frameContext->windows.EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 1, 0, 0);
    }
  }

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 3, item, owned.unknown_0000[1] > 0 ? item.quantity : 1, -1);
  }
}

// OFFSET: 0x00984CA0
// FUNCTION: fn_13359
void UIItemShopWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    const ITEM_INFO owned = LookupClientItemByIdentity(g_session.m_inventoryItems, item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 3, item, owned.unknown_0000[1] > 0 ? item.quantity : 1, -1);
  }
}

// OFFSET: 0x00984DF0
// FUNCTION: fn_13360
void UIItemStoreFindWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0 && item.unknown_0074[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1738(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 6, item, item.quantity, -1);
  }
}

// OFFSET: 0x00984F10
// FUNCTION: fn_13361
void UIItemStoreSubWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0 && item.unknown_0074[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1738(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 6, item, item.quantity, -1);
  }
}

// OFFSET: 0x00985030
// FUNCTION: fn_13362
void UIItemStoreWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0 && item.unknown_0074[2] == 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1738(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, 6, item, item.quantity, -1);
  }
}

// OFFSET: 0x009853A0
// FUNCTION: fn_13365
void UIMerchantItemPurchaseWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
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

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1760ByItemId(item.itemId.c_str());
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(this->m_frameContext->windows, descriptions, this->unknown_0108 != 0 ? 21 : 13, item, item.quantity, -1);
  }
}

// OFFSET: 0x00985830
// FUNCTION: fn_13368
void UIMerchantMirrorItemWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1750(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    mode->drag.BeginItem(
      this->m_frameContext->windows, descriptions, this->unknown_00F4 == 1 ? 20 : this->unknown_00F4 == 2 ? 22 : 11, item, item.quantity, -1
    );
  }
}

// OFFSET: 0x00985970
// FUNCTION: fn_13369
void UIMerchantShopMakeWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (g_replayAnimationScaling != 0) {
    return;
  }

  ITEM_INFO item = this->GetItemAt(x, y);

  if (item.unknown_0000[1] > 0) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    item = g_session.FindItem1768(item.unknown_0000[1]);
    ItemDescriptions descriptions{g_itemDescriptions};
    gameMode->drag.BeginItem(this->m_frameContext->windows, descriptions, this->mode != 0 ? 19 : 10, item, item.quantity, -1);
  }
}
