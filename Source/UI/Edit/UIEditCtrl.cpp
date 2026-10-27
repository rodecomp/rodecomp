#include "Client/Input/InputCharset.h"
#include "Client/Input/TextInputBuffer.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapImage.h"
#include "UI/Edit/EditTextState.h"
#include "UI/Edit/EditViewport.h"
#include "UI/Window/InputFocus.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/UIWindow.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <list>
#include <string>
#include <windows.h>
#include <timeapi.h>

namespace {
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
  TextInputBuffer& input;
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
  TextContext context(this->surface, this->fonts, this->locale);
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
      const char* prefix = this->alternate ? this->input.candidate.c_str() : this->charset.Prefix(false);
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
  this->m_edit->input.CancelComposition();
  this->m_edit->input.CancelComposition();
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

    for (const UIWindow* window = this; window; window = static_cast<const UIWindow*>(window->parent)) {
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
    this->m_edit->input.CancelComposition();
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
    this->m_edit->input.CancelComposition();
    this->m_edit->input.CancelComposition();
    this->m_edit->input.Insert(this->text.c_str());
  }

  this->SelectAll();

  if (this->parent) {
    static_cast<UIWindow*>(this->parent)->Invalidate();
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
  UIWindow* parent = static_cast<UIWindow*>(this->parent);
  int left = 0, top = 0;

  for (const UIWindow* window = this; window; window = static_cast<const UIWindow*>(window->parent)) {
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

// OFFSET: 0x00865DD0
// FUNCTION: fn_10937
void UIEditCtrl::Background(int red, int green, int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->Invalidate();
}

// OFFSET: 0x00868570
// FUNCTION: fn_10960
void UIEditCtrl::TextColor(std::uint32_t primary, std::uint32_t secondary) {
  this->secondColor = secondary;
  this->textColor = primary;
  this->Invalidate();
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
    static_cast<UIWindow*>(this->parent)->Slot35(value);
  }
}
